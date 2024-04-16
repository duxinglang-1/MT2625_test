/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
 * Without the prior written permission of MediaTek and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
 * if you have agreed to and been bound by the applicable license agreement with
 * MediaTek ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
 * please cease any access or use of MediaTek Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

#ifdef MTK_NVDM_MODEM_ENABLE

/*
 * [regions of PEBs]
 * we maintain NVDM_PEB_COUNT logic erase blocks, including main PEBs and reserved PEBs.
 * main PEBs are used to store data items header and actural data.
 * reserved PEBs are used in garbage collection to store data item's content merged from other main dirty PEBs.
 *
 * [composition of every PEB]
 * dirty data + valid data + free space
 *
 * [Flow for PEB allocation]
 * 1, find most best fit PEB with required free space;
 * 2, if found, return number and offset of PEB;
 * 3, if not found, begin to start garbage collection;
 * 4, scan PEBs to find victims, and it's criteria is:
 *      - find erase count of PEB less than average erase count to much;
 *      - try to merge PEBs to reserved PEBs;
 * 5, sort victims PEBs with valid data items size;
 * 6, try to merge PEBs to reserved PEBs;
 *      - at least two PEBs can be merge into one reserved PEB;
 * 7, if merge fail, directlly reclaim least valid size space PEB;
 *      - we should report this situation;
 */

#include "nvdm_modem.h"
#include "nvdm_modem_port.h"
#include "nvdm_modem_internal.h"

/********************************************************
 * Macro & Define
 *
 ********************************************************/
#if 0 /*Profiling*/
    #define EXEC_TIME_GPT(TASK)	do{	\
			uint32_t start, end;	\
			hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &start);	\
			TASK	\
			hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &end);	\
			printf("  [PROFILE][PEB_RD_DATA]GPT_TIME=%d\r\n", end-start);	\
		}while(0)

    static uint32_t gpt_count[20] = {0};
    #define TIMESTAMP_REC(I)	do{\
								hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_count[I]);	\
							}while(0)

    #define TIMESTAMP_DUMP(I)	do{	\
								uint32_t i; \
								for (i=1; i<=I; i++)	\
									printf("[GPT DUMP]%ld=>%ld(%ld)\r\n", i, gpt_count[i], gpt_count[i]-gpt_count[i-1]);	\
							}while(0)	

#else
    #define EXEC_TIME_GPT(TASK)	do{TASK}while(0);
	#define TIMESTAMP_REC(I)	do{}while(0)
	#define TIMESTAMP_DUMP(I)	do{}while(0)
#endif

#define AREA_IDX    (nvdm_modem_port_get_working_area(true) >> 1)

#define PEB_ERASE_COUNT_CRITERION 5
#define NVDM_MAX_USAGE_RATIO 70
/********************************************************
 * Enum & Structures & Global variable
 *
 ********************************************************/
typedef struct {
    uint16_t free;
    uint16_t dirty;
    int32_t is_reserved;
    uint32_t erase_count;
} peb_info_t;

typedef struct {
    peb_info_t *pebs_info;          /* status of every peb */
    uint32_t   valid_data_size;     /* current valid data size in all main pebs  */
    uint32_t   total_avail_space;   /* total avail space for storge data items */
    uint32_t   nvdm_modem_peb_count;/* PEB count cunstom by user */
} peb_handle_t;

static peb_handle_t peb_handle[(NVDM_MODEM_AREA_END >> 1) + 1];

/* status of nvdm module initialization */
bool g_nvdm_modem_init_status = false;
/* PEB size decided by flash device */
uint32_t g_nvdm_modem_peb_size;

/********************************************************
 * Function declaration
 *
 ********************************************************/
static void nvdm_modem_peb_print_info()
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    uint32_t i;

    nvdm_modem_port_log_notice("region info show below:area=%d", AREA_IDX);
    nvdm_modem_port_log_notice("%s%12s%12s%12s%12s", "peb","free","dirty","erase_count","is_reserved");
    for (i = 0; i < handle->nvdm_modem_peb_count; i++) {
        nvdm_modem_port_log_notice("%d%12d%12d%12d%12d",
                                 i,
                                 handle->pebs_info[i].free,
                                 handle->pebs_info[i].dirty,
                                 handle->pebs_info[i].erase_count,
                                 handle->pebs_info[i].is_reserved);
    }
    nvdm_modem_port_log_notice("valid_data_size = %d", handle->valid_data_size);
}
static void nvdm_modem_peb_header_print_info(uint32_t peb_index, peb_header_t *peb_hdr)
{
    nvdm_modem_port_log_info(" peb header(%d) info show below:", peb_index);
    nvdm_modem_port_log_info(" magic: %08x", peb_hdr->magic);
    nvdm_modem_port_log_info(" erase_count: %08x", peb_hdr->erase_count);
    nvdm_modem_port_log_info(" status: %02x", peb_hdr->status);
    nvdm_modem_port_log_info(" peb_reserved: %02x", peb_hdr->peb_reserved);
    nvdm_modem_port_log_info(" version: %02x", peb_hdr->version);
}

static bool nvdm_modem_peb_header_is_validate(peb_header_t *peb_header, int32_t is_empty)
{
    if (peb_header->magic != PEB_HEADER_MAGIC) {
        return false;
    }

    if (peb_header->erase_count == ERASE_COUNT_MAX) {
        return false;
    }

    if (peb_header->version != NVDM_VERSION) {
        return false;
    }

    if (is_empty) {
        if ((peb_header->status != PEB_STATUS_EMPTY) ||
                (peb_header->peb_reserved != 0xFF)) {
            return false;
        }
    } else {
        if (peb_header->peb_reserved == 0xFF) {
            return false;
        }
    }

    return true;
}

void nvdm_modem_peb_read_header(int32_t pnum, peb_header_t *peb_header)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];

    uint8_t buf[PEB_HEADER_SIZE];

    if (pnum >= handle->nvdm_modem_peb_count) {
        nvdm_modem_port_log_error("pnum=%d", pnum);
        return;
    }

    nvdm_modem_peb_io_read(pnum, 0, buf, PEB_HEADER_SIZE);

    if (peb_header) {
        *peb_header = *(peb_header_t *)buf;
    }
}

void nvdm_modem_peb_write_data(int32_t pnum, int32_t offset, const uint8_t *buf, int32_t len)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    int32_t ret;
    peb_header_t peb_header;

    if (pnum >= handle->nvdm_modem_peb_count) {
        nvdm_modem_port_log_error("pnum=%d", pnum);
        return;
    }
    if (offset >= g_nvdm_modem_peb_size - PEB_HEADER_SIZE) {
        nvdm_modem_port_log_error("offset=0x%x", offset);
        return;
    }
    if (len > g_nvdm_modem_peb_size - PEB_HEADER_SIZE) {
        nvdm_modem_port_log_error("len=%d", len);
        return;
    }

    /*
     * We write to the data area of the physical eraseblock. Make
     * sure it has valid EC headers.
     */
    nvdm_modem_peb_read_header(pnum, &peb_header);
    ret = nvdm_modem_peb_header_is_validate(&peb_header, 0);
    if (ret == false) {
        nvdm_modem_port_log_error("magic=0x%x, erase_count=0x%x, status=0x%x, peb_reserved=0x%x",
                                  peb_header.magic, peb_header.erase_count,
                                  peb_header.status, peb_header.peb_reserved);
        return;
    }

    nvdm_modem_peb_io_write(pnum, PEB_HEADER_SIZE + offset, buf, len);
}

void nvdm_modem_peb_read_data(int32_t pnum, int32_t offset, uint8_t *buf, int32_t len)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    if (pnum >= handle->nvdm_modem_peb_count) {
        nvdm_modem_port_log_error("pnum=%d", pnum);
        return;
    }
    if (offset >= g_nvdm_modem_peb_size - PEB_HEADER_SIZE) {
        nvdm_modem_port_log_error("offset=0x%x", offset);
        return;
    }
    if (len > g_nvdm_modem_peb_size - PEB_HEADER_SIZE) {
        nvdm_modem_port_log_error("len=%d", len);
        return;
    }

    nvdm_modem_peb_io_read(pnum, PEB_HEADER_SIZE + offset, buf, len);
}

void nvdm_modem_peb_update_status(int32_t pnum, peb_status_t status)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    if (pnum >= handle->nvdm_modem_peb_count) {
        nvdm_modem_port_log_error("pnum=%d", pnum);
        return;
    }

    nvdm_modem_peb_io_write(pnum, PEB_STATUS_OFFSET, (uint8_t *)&status, 1);
}

static void nvdm_modem_peb_write_hdr_erase_count(int32_t pnum, uint32_t erase_count)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    if (pnum >= handle->nvdm_modem_peb_count) {
        nvdm_modem_port_log_error("pnum=%d", pnum);
        return;
    }

    nvdm_modem_peb_io_write(pnum, PEB_ERASE_COUNT_OFFSET, (uint8_t *)&erase_count, 4);
}

static void nvdm_modem_peb_write_hdr_peb_reserved(int32_t pnum)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    uint8_t unreserved_mark = PEB_UNRESERVED;

    if (pnum >= handle->nvdm_modem_peb_count) {
        nvdm_modem_port_log_error("pnum=%d", pnum);
        return;
    }

    nvdm_modem_peb_io_write(pnum, PEB_RESERVED_OFFSET, &unreserved_mark, 1);
}

static void nvdm_modem_peb_write_hdr_magic_number(int32_t pnum)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    uint32_t magic_number;

    if (pnum >= handle->nvdm_modem_peb_count) {
        nvdm_modem_port_log_error("pnum=%d", pnum);
        return;
    }

    magic_number = PEB_HEADER_MAGIC;
    nvdm_modem_peb_io_write(pnum, PEB_MAGIC_OFFSET, (uint8_t *)&magic_number, 4);
}

static void nvdm_modem_peb_write_hdr_version(int32_t pnum)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    uint8_t version = NVDM_VERSION;

    if (pnum >= handle->nvdm_modem_peb_count) {
        nvdm_modem_port_log_error("pnum=%d", pnum);
        return;
    }

    nvdm_modem_peb_io_write(pnum, PEB_VERSION_OFFSET, &version, 1);
}

int32_t nvdm_modem_peb_activing(int32_t pnum)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];

    if (handle->pebs_info[pnum].free == (g_nvdm_modem_peb_size - PEB_HEADER_SIZE)) {
        nvdm_modem_peb_update_status(pnum, PEB_STATUS_ACTIVING);
        nvdm_modem_port_poweroff(9);
        nvdm_modem_peb_write_hdr_peb_reserved(pnum);
        handle->pebs_info[pnum].is_reserved = 0;
        return 1;
    }

    return 0;
}

static void nvdm_modem_peb_reclaim(int32_t pnum)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];

    nvdm_modem_peb_update_status(pnum, PEB_STATUS_ERASING);
    nvdm_modem_port_poweroff(10);
    nvdm_modem_peb_io_erase(pnum);
    nvdm_modem_peb_write_hdr_magic_number(pnum);
    nvdm_modem_peb_write_hdr_erase_count(pnum, ++(handle->pebs_info[pnum].erase_count));
    nvdm_modem_peb_write_hdr_version(pnum);
    nvdm_modem_peb_update_status(pnum, PEB_STATUS_EMPTY);
    nvdm_modem_port_poweroff(11);
    handle->pebs_info[pnum].is_reserved = 1;
    handle->pebs_info[pnum].dirty = 0;
    handle->pebs_info[pnum].free = (g_nvdm_modem_peb_size - PEB_HEADER_SIZE);
}

/* relocate one or more pebs with large of dirty data to one new empty peb */
static void nvdm_modem_relocate_pebs(int32_t *source_pebs, int32_t source_peb_count)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    int32_t i, offset;
    int32_t target_peb;
    uint32_t least_erase_count, total_valid_data;

    /* there is a special case that all source_pebs cantain no valid data at all.
     * So we just reclaim these pebs and erase them.
     */
    total_valid_data = 0;
    for (i = 0; i < source_peb_count; i++) {
        total_valid_data += (g_nvdm_modem_peb_size - PEB_HEADER_SIZE) - handle->pebs_info[source_pebs[i]].dirty - handle->pebs_info[source_pebs[i]].free;
    }
    if (total_valid_data == 0) {
        nvdm_modem_port_log_notice("found no valid data in reclaiming pebs when relocate_pebs()");
        for (i = 0; i < source_peb_count; i++) {
            nvdm_modem_peb_reclaim(source_pebs[i]);
        }

        return;
    }

    /* mark source pebs which is needed relocation */
    for (i = 0; i < source_peb_count; i++) {
        nvdm_modem_peb_update_status(source_pebs[i], PEB_STATUS_RECLAIMING);
        nvdm_modem_port_poweroff(12);
    }

    /* search a target peb and mark it */
    least_erase_count = ERASE_COUNT_MAX;
    target_peb = handle->nvdm_modem_peb_count;
    for (i = 0; i < handle->nvdm_modem_peb_count; i++) {
        if (!handle->pebs_info[i].is_reserved) {
            continue;
        }
        if (least_erase_count > handle->pebs_info[i].erase_count) {
            target_peb = i;
            least_erase_count = handle->pebs_info[i].erase_count;
        }
    }
    if (target_peb >= handle->nvdm_modem_peb_count) {
        nvdm_modem_port_log_error("target_peb=%d", target_peb);
        return;
    }
    nvdm_modem_port_log_info("found a target peb(%d) for reclaiming", target_peb);
    nvdm_modem_peb_update_status(target_peb, PEB_STATUS_ACTIVING);
    nvdm_modem_port_poweroff(13);
    nvdm_modem_peb_write_hdr_peb_reserved(target_peb);
    handle->pebs_info[target_peb].is_reserved = 0;
    nvdm_modem_peb_update_status(target_peb, PEB_STATUS_TRANSFERING);
    nvdm_modem_port_poweroff(14);

    /* begin transfer data from source pebs to target peb */
    offset = 0;
    for (i = 0; i < source_peb_count; i++) {
        offset = nvdm_modem_data_item_migration(source_pebs[i], target_peb, offset);
    }

    /* We can't dirrectly update status of discard pebs to PEB_STATUS_ACTIVED,
     * or we can't deal with those pebs in init flow if power-off happen below.
     */
    nvdm_modem_peb_update_status(target_peb, PEB_STATUS_TRANSFERED);
    nvdm_modem_port_poweroff(15);
    handle->pebs_info[target_peb].is_reserved = 0;

    /* put back all discard pebs to wear-leaving */
    for (i = 0; i < source_peb_count; i++) {
        nvdm_modem_peb_reclaim(source_pebs[i]);
    }

    /* Now we can update status of target peb to PEB_STATUS_ACTIVED safety */
    nvdm_modem_peb_update_status(target_peb, PEB_STATUS_ACTIVED);
    nvdm_modem_port_poweroff(16);
}

static void nvdm_modem_garbage_reclaim_pebs(int32_t found_blocks, int32_t *peb_list)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    int32_t i, sum_valid, merge_cnt, merge_start, merge_loop;

    /* try to merge PEBs to reserved PEBs if
        * at least two PEBs can be merge into one reserved PEB.
        */
    merge_start = 0;
    merge_loop = 1;
    do {
        sum_valid = 0;
        merge_cnt = 0;
        for (i = merge_start; i < found_blocks; i++) {
            sum_valid += (g_nvdm_modem_peb_size - PEB_HEADER_SIZE) - handle->pebs_info[peb_list[i]].dirty - handle->pebs_info[peb_list[i]].free;
            if (sum_valid > (g_nvdm_modem_peb_size - PEB_HEADER_SIZE)) {
                break;
            }
            merge_cnt++;
        }

        nvdm_modem_port_log_notice("found_blocks=%d, merge_cnt=%d, merge_loop=%d, sum_valid=%d", found_blocks, merge_cnt, merge_loop, sum_valid);
        /* now let us start merging operation */
        for (i = merge_start; i < merge_start + merge_cnt; i++) {
            nvdm_modem_port_log_notice("merge peb %d", peb_list[i]);
        }
        nvdm_modem_relocate_pebs(&peb_list[merge_start], merge_cnt);

        /* if merge loop reach limit or merge hard, stop merge attemption again. */
        if (merge_cnt < 2 || --merge_loop == 0) {
            break;
        }

        merge_start += merge_cnt;
    } while (merge_start < found_blocks);
}

/* NVDM will start garbage collection for PEBs
 * with PEB_ERASE_COUNT_CRITERION less than average erase count
 */
static void nvdm_modem_garbage_collection_peb()
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    int32_t i, j, max;
    int32_t cur_valid, tmp_valid;
    uint32_t cur_erase_count, tmp_erase_count;
    int32_t found_blocks, non_reserved_pebs;
    int32_t *peb_list, tmp_peb;
    uint64_t mean_erase_count;

    nvdm_modem_port_log_notice("start garbage collection!!!");

    nvdm_modem_peb_print_info();

    peb_list = (int32_t *)nvdm_modem_port_malloc(handle->nvdm_modem_peb_count * sizeof(int32_t));
    if (peb_list == NULL) {
        nvdm_modem_port_log_error("peb_list alloc fail");
        return;
    }

    /* Method 1: Reclaim PEBs containing no valid data. */
    found_blocks = 0;
    for (i = 0; i < handle->nvdm_modem_peb_count; i++) {
        if (handle->pebs_info[i].is_reserved) {
            continue;
        }
        cur_valid = (g_nvdm_modem_peb_size - PEB_HEADER_SIZE) - handle->pebs_info[i].dirty - handle->pebs_info[i].free;
        if (cur_valid == 0) {
            peb_list[found_blocks++] = i;
        }
    }
    if (found_blocks) {
        nvdm_modem_port_log_notice("[M1]reclaim blocks select by empty PEBs found_blocks=%d", found_blocks);
        nvdm_modem_port_log_notice("[M1]reclaim peb_list(empty PEBs): ");
        for (i = 0; i < found_blocks; i++) {
            nvdm_modem_port_log_notice("[M1]list:peb_%d", peb_list[i]);
        }
        /* begin to reclaim those pebs */
        nvdm_modem_garbage_reclaim_pebs(found_blocks, peb_list);
        /* free the memory alloc before */
        nvdm_modem_port_free(peb_list);
        /* the attemption is successfully, return. */
        return;
    }

    /* Method 2: Reclaim PEBs by erase count. */
    /* 1. scan all non-reserved pebs to calculate average erase counter. */
    mean_erase_count = 0;
    non_reserved_pebs = 0;
    for (i = 0; i < handle->nvdm_modem_peb_count; i++) {
        if (handle->pebs_info[i].is_reserved) {
            continue;
        }
        mean_erase_count += handle->pebs_info[i].erase_count;
        non_reserved_pebs++;
    }
    if (non_reserved_pebs > 0) {
    mean_erase_count /= non_reserved_pebs;
    }
    else {
        nvdm_modem_port_log_error("non_reserved_pebs = %d", non_reserved_pebs);
    }    
    nvdm_modem_port_log_info("[M2]non_reserved_pebs = %d", non_reserved_pebs);
    nvdm_modem_port_log_info("[M2]mean_erase_count = %d", mean_erase_count);
    /* 2. find all PEBs with erase counter below threshold. */
    found_blocks = 0;
    for (i = 0; i < handle->nvdm_modem_peb_count; i++) {
        if (handle->pebs_info[i].is_reserved) {
            continue;
        }
        if (handle->pebs_info[i].erase_count < (mean_erase_count * (100 - PEB_ERASE_COUNT_CRITERION)) / 100) {
            peb_list[found_blocks++] = i;
        }
    }
    /* 3. sort victims PEBs with erase count */
    if (found_blocks > 1) {
        nvdm_modem_port_log_info("[M2]reclaim blocks select by erase count = %d", found_blocks);
        nvdm_modem_port_log_info("[M2]reclaim peb_list(no-sort): ");
        for (i = 0; i < found_blocks; i++) {
            nvdm_modem_port_log_info("[M2]%d", peb_list[i]);
        }
        for (i = 0; i < found_blocks; i++) {
            cur_erase_count = handle->pebs_info[peb_list[i]].erase_count;
            max = i;
            for (j = i; j < found_blocks; j++) {
                tmp_erase_count = handle->pebs_info[peb_list[j]].erase_count;
                if (cur_erase_count > tmp_erase_count) {
                    cur_erase_count = tmp_erase_count;
                    max = j;
                }
            }
            if (i != max) {
                tmp_peb = peb_list[max];
                peb_list[max] = peb_list[i];
                peb_list[i] = tmp_peb;
            }
        }
        nvdm_modem_port_log_notice("[M2]reclaim peb_list(sort): ");
        for (i = 0; i < found_blocks; i++) {
            nvdm_modem_port_log_notice("[[M2]]peb:%d, erase_cnt=%d", peb_list[i], handle->pebs_info[peb_list[i]].erase_count);
        }
        /* 4. begin to reclaim those pebs */
        nvdm_modem_garbage_reclaim_pebs(found_blocks, peb_list);
        /* free the memory alloc before */
        nvdm_modem_port_free(peb_list);
        /* the attemption is successfully, return. */
        return;
    }

    /* Method 3: Reclaim PEBs by valid size. */
    /* 1. find all non researved PEBs. */
    found_blocks = 0;
    for (i = 0; i < handle->nvdm_modem_peb_count; i++) {
        if (handle->pebs_info[i].is_reserved) {
            continue;
        }
        peb_list[found_blocks++] = i;
    }
    nvdm_modem_port_log_notice("[M3]reclaim blocks select by valid size = %d", found_blocks);
    nvdm_modem_port_log_notice("[M3]reclaim peb_list(no-sort): ");
    for (i = 0; i < found_blocks; i++) {
        nvdm_modem_port_log_notice("[M3]%d", peb_list[i]);
    }
    /* 2. sort victims PEBs with valid data items size. */
    for (i = 0; i < found_blocks; i++) {
        cur_valid = (g_nvdm_modem_peb_size - PEB_HEADER_SIZE) - handle->pebs_info[peb_list[i]].dirty - handle->pebs_info[peb_list[i]].free;
        max = i;
        for (j = i; j < found_blocks; j++) {
            tmp_valid = (g_nvdm_modem_peb_size - PEB_HEADER_SIZE) - handle->pebs_info[peb_list[j]].dirty - handle->pebs_info[peb_list[j]].free;
            if (cur_valid > tmp_valid) {
                cur_valid = tmp_valid;
                max = j;
            }
        }
        if (i != max) {
            tmp_peb = peb_list[max];
            peb_list[max] = peb_list[i];
            peb_list[i] = tmp_peb;
        }
    }
    nvdm_modem_port_log_notice("[M3]reclaim peb_list(sort): ");
    for (i = 0; i < found_blocks; i++) {
        nvdm_modem_port_log_notice("[M3]%d, valid_size=%d", peb_list[i], handle->pebs_info[peb_list[i]].dirty - handle->pebs_info[peb_list[i]].free);
    }
    /* 3. begin to reclaim those pebs */
    nvdm_modem_garbage_reclaim_pebs(found_blocks, peb_list);

    /* free the memory alloc before */
    nvdm_modem_port_free(peb_list);

    nvdm_modem_peb_print_info();
}

static int32_t nvdm_modem_find_free_peb(int32_t size)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    int32_t i, reserved_peb = -1, reserved_peb_cnt, target_peb = -1;
    int32_t min_free_space = (g_nvdm_modem_peb_size - PEB_HEADER_SIZE);
    uint32_t least_erase_count;

    /* find in non-reserved pebs frist */
    least_erase_count = ERASE_COUNT_MAX;
    reserved_peb_cnt = 0;
    for (i = 0; i < handle->nvdm_modem_peb_count; i++) {
        if (handle->pebs_info[i].is_reserved) {
            reserved_peb_cnt++;
            if (least_erase_count > handle->pebs_info[i].erase_count) {
                reserved_peb = i;
                least_erase_count = handle->pebs_info[i].erase_count;
            }
            continue;
        }
        if (handle->pebs_info[i].free > size) {
            if ((target_peb < 0) || (handle->pebs_info[i].free < min_free_space)) {
                min_free_space = handle->pebs_info[i].free;
                target_peb = i;
            }
        }
    }

    if (target_peb >= 0) {
        return target_peb;
    }

    /* use reserved peb if we have (exclude backup peb) */
    if (reserved_peb_cnt > NVDM_RESERVED_PEB_COUNT) {
        target_peb = reserved_peb;
    }

    nvdm_modem_port_log_notice("find_free_peb: target_peb = %d, reserved_peb = %d, reserved_peb_cnt = %d\n", target_peb, reserved_peb, reserved_peb_cnt);

    return target_peb;
}

/* allocate a logic erase block with at least free space size */
int32_t nvdm_modem_space_allocation(int32_t alloc_size, int32_t added_size, int32_t *poffset)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    int32_t target_peb = -1;

    target_peb = nvdm_modem_find_free_peb(alloc_size);
    if (target_peb < 0) {
        nvdm_modem_garbage_collection_peb();
        target_peb = nvdm_modem_find_free_peb(alloc_size);
    }

    if (target_peb >= 0) {
        *poffset = (g_nvdm_modem_peb_size - PEB_HEADER_SIZE) - handle->pebs_info[target_peb].free;
        handle->valid_data_size += added_size;
    }

    return target_peb;
}

/* This function decides max avail size of NVDM's region.
 * Normally we consider two factors:
 *  - Max size of data item during all user defined data items.
 *  - Limit total size of data items so that garbage collection don't happen too frequently.
 */
static uint32_t nvdm_modem_calculate_total_avail_space()
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    uint32_t max_reasonable_size, criteria1, criteria2;

    criteria1 = (g_nvdm_modem_peb_size - PEB_HEADER_SIZE) *
                (handle->nvdm_modem_peb_count - NVDM_RESERVED_PEB_COUNT) *
                NVDM_MAX_USAGE_RATIO / 100;
    criteria2 = (handle->nvdm_modem_peb_count - NVDM_RESERVED_PEB_COUNT) *
                (g_nvdm_modem_peb_size - PEB_HEADER_SIZE - g_modem_max_data_item_size - DATA_ITEM_HEADER_SIZE - DATA_ITEM_CHECKSUM_SIZE - g_modem_max_group_name_size - g_modem_max_data_item_name_size - 2);
#if 0
    if (criteria1 > criteria2) {
        max_reasonable_size = criteria2;
    } else {
        max_reasonable_size = criteria1;
    }
#else
    max_reasonable_size = criteria1;
#endif

    nvdm_modem_port_log_info("total avail space = %d\n", max_reasonable_size);

    return max_reasonable_size;
}

bool nvdm_modem_space_is_enough(int32_t size)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    nvdm_modem_port_log_info("space_is_enough: valid_data_size = %d, new add size = %d\n", handle->valid_data_size, size);

    if ((handle->valid_data_size + size) > handle->total_avail_space) {
        nvdm_modem_port_log_notice("(%d)space_is_enough: (%d,%d,%d)\n", AREA_IDX, handle->valid_data_size, size, handle->total_avail_space);
        return false;
    }

    return true;
}

static bool nvdm_modem_check_peb_integrity(int32_t pnum, peb_header_t *peb_hdr)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];

    if ((peb_hdr->magic == 0xFFFFFFFF) && (peb_hdr->status == PEB_STATUS_VIRGIN)) {
        return true;
    } else if (peb_hdr->magic == PEB_HEADER_MAGIC) {
        return true;
    } else {
        /*Power drop during eraseing procedure might cause memory cell stay at unstable status, need to erase again*/
        nvdm_modem_port_log_notice("PEB%d:integrity check failed, erase again", pnum);
        nvdm_modem_peb_io_erase(pnum);
        return false;
    }

    return true;
}

void nvdm_modem_peb_add_drity(int32_t pnum, int32_t drity)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    handle->pebs_info[pnum].dirty += drity;
}

void nvdm_modem_peb_add_free(int32_t pnum, int32_t free)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    handle->pebs_info[pnum].free += free;
}

void nvdm_modem_peb_sub_free(int32_t pnum, int32_t free)
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    handle->pebs_info[pnum].free -= free;
}

static void nvdm_modem_peb_scan()
{
    peb_handle_t *handle = &peb_handle[AREA_IDX];
    int32_t i, j, ret;
    peb_header_t peb_hdr;
    uint8_t peb_status;
    int32_t reclaim_idx, *reclaiming_peb;
    int32_t transfering_peb, transfered_peb;
    uint32_t g_mean_erase_count = 0;

    reclaiming_peb = (int32_t *)nvdm_modem_port_malloc(handle->nvdm_modem_peb_count * sizeof(int32_t));
    if (reclaiming_peb == NULL) {
        nvdm_modem_port_log_error("reclaiming_peb alloc fail");
        return;
    }
    memset(reclaiming_peb, 0, handle->nvdm_modem_peb_count * sizeof(int32_t));

    for (i = 0; i < handle->nvdm_modem_peb_count; i++) {
        handle->pebs_info[i].erase_count = ERASE_COUNT_MAX;
    }

    nvdm_modem_port_log_info("scan and verify peb headers");
    reclaim_idx = 0;
    transfering_peb = -1;
    transfered_peb = -1;
    for (i = 0; i < handle->nvdm_modem_peb_count; i++) {
        nvdm_modem_peb_read_header(i, &peb_hdr);
        if (nvdm_modem_check_peb_integrity(i, &peb_hdr) == false) {
            nvdm_modem_peb_read_header(i, &peb_hdr);
        }
        nvdm_modem_port_log_info("before verify peb header");
        nvdm_modem_peb_header_print_info(i, &peb_hdr);
        peb_status = peb_hdr.status;
        switch (peb_status) {
            case PEB_STATUS_ACTIVING:
                /* need erase and erase count is valid */
                nvdm_modem_peb_io_erase(i);
                nvdm_modem_peb_write_hdr_magic_number(i);
                nvdm_modem_peb_write_hdr_erase_count(i, ++(peb_hdr.erase_count));
                nvdm_modem_peb_write_hdr_version(i);
                handle->pebs_info[i].erase_count = peb_hdr.erase_count;
                nvdm_modem_peb_update_status(i, PEB_STATUS_EMPTY);
                nvdm_modem_port_poweroff(17);
                handle->pebs_info[i].is_reserved = 1;
                break;
            case PEB_STATUS_TRANSFERING:
            case PEB_STATUS_TRANSFERED:
            case PEB_STATUS_RECLAIMING:
                ret = nvdm_modem_peb_header_is_validate(&peb_hdr, 0);
                if (ret == false) {
                    nvdm_modem_port_log_error("peb_header validate fail, pnum=%d", i);
                    nvdm_modem_port_free(reclaiming_peb);
                    return;
                }
                handle->pebs_info[i].erase_count = peb_hdr.erase_count;
                /* we just mark those pebs, and deal with them after init complete. */
                if (peb_status == PEB_STATUS_TRANSFERING) {
                    if (transfering_peb >= 0) {
                        nvdm_modem_port_log_error("find more than one transfering peb, frist=%d, second=%d", transfering_peb, i);
                        nvdm_modem_port_free(reclaiming_peb);
                        return;
                    }
                    transfering_peb = i;
                } else if (peb_status == PEB_STATUS_TRANSFERED) {
                    if (transfered_peb >= 0) {
                        nvdm_modem_port_log_error("find more than one transfered peb, frist=%d, second=%d", transfered_peb, i);
                        nvdm_modem_port_free(reclaiming_peb);
                        return;
                    }
                    transfered_peb = i;
                } else {
                    /* there may be multiple reclaiming pebs exist */
                    reclaiming_peb[reclaim_idx++] = i;
                }
                break;
            case PEB_STATUS_ACTIVED:
                ret = nvdm_modem_peb_header_is_validate(&peb_hdr, 0);
                if (ret == false) {
                    nvdm_modem_port_log_error("peb_header validate fail, pnum=%d", i);
                    nvdm_modem_port_free(reclaiming_peb);
                    return;
                }
                handle->pebs_info[i].erase_count = peb_hdr.erase_count;
                break;
            case PEB_STATUS_EMPTY:
                ret = nvdm_modem_peb_header_is_validate(&peb_hdr, 1);
                if (ret == false) {
                    nvdm_modem_port_log_error("peb_header validate fail, pnum=%d", i);
                    nvdm_modem_port_free(reclaiming_peb);
                    return;
                }
                handle->pebs_info[i].erase_count = peb_hdr.erase_count;
                handle->pebs_info[i].is_reserved = 1;
                break;
            case PEB_STATUS_VIRGIN:
            case PEB_STATUS_ERASING:
            default:
                /* need erase and erase count is invalid */
                nvdm_modem_peb_io_erase(i);
                nvdm_modem_peb_write_hdr_magic_number(i);
                nvdm_modem_peb_write_hdr_version(i);
                handle->pebs_info[i].is_reserved = 1;
        }
        nvdm_modem_port_log_info("after verify peb header");
        nvdm_modem_peb_read_header(i, &peb_hdr);
        nvdm_modem_peb_header_print_info(i, &peb_hdr);
    }
    //nvdm_modem_peb_print_info();
    nvdm_modem_port_log_info("transfering_peb = %d", transfering_peb);
    nvdm_modem_port_log_info("transfered_peb = %d", transfered_peb);
    for (i = 0; i < reclaim_idx; i++) {
        nvdm_modem_port_log_info("reclaiming_peb[%d] = %d", i, reclaiming_peb[i]);
    }

    /* update erase count for unknown pebs */
    nvdm_modem_port_log_info("update erase count for unknown pebs");
    g_mean_erase_count = 0;
    for (i = 0; i < handle->nvdm_modem_peb_count; i++) {
        if (handle->pebs_info[i].erase_count != ERASE_COUNT_MAX) {
            g_mean_erase_count += handle->pebs_info[i].erase_count;
        }
    }
    g_mean_erase_count /= handle->nvdm_modem_peb_count;
    for (i = 0; i < handle->nvdm_modem_peb_count; i++) {
        if (handle->pebs_info[i].erase_count == ERASE_COUNT_MAX) {
            /* peb header need to update here if erase count is invalid */
            nvdm_modem_peb_write_hdr_erase_count(i, g_mean_erase_count);
            handle->pebs_info[i].erase_count = g_mean_erase_count;
            nvdm_modem_peb_update_status(i, PEB_STATUS_EMPTY);
            nvdm_modem_port_poweroff(18);
        }
    }
    //nvdm_modem_peb_print_info();

    /* scan all non-reserved pebs including reclaiming pebs and transfering peb */
    nvdm_modem_port_log_info("scan all non-reserved pebs including reclaiming pebs and transfering peb");
    for (i = 0; i < handle->nvdm_modem_peb_count; i++) {
        /* skip transfering peb */
        if (i == transfering_peb) {
            continue;
        }
        /* skip reclaiming pebs and use reference from transfered peb if it exist */
        if (transfered_peb >= 0) {
            for (j = 0; j < reclaim_idx; j++) {
                if (i == reclaiming_peb[j]) {
                    break;
                }
            }
            if (j < reclaim_idx) {
                continue;
            }
        }
        if (handle->pebs_info[i].is_reserved == 0) {
            /* reclaiming, transfered and active pebs can be scanned here */
            nvdm_modem_data_item_scan(i);
        } else {
            handle->pebs_info[i].free = (g_nvdm_modem_peb_size - PEB_HEADER_SIZE);
        }
    }

    /* deal with break from garbage collection */
    if ((reclaim_idx > 0) && (transfered_peb < 0)) {
        /* when power-off last time, data transfering is going on.
             * so we just restart garbage collection breaked by last power-off.
             */
        nvdm_modem_port_log_info("found a peb in transfering status");
        if (transfering_peb >= 0) {
            nvdm_modem_peb_reclaim(transfering_peb);
        }
        nvdm_modem_relocate_pebs(reclaiming_peb, reclaim_idx);
    } else if (transfered_peb >= 0) {
        /* when power-off last time, data transfer is complete,
             * but source pebs maybe have not put back to wear-weaving yet
             */
        nvdm_modem_port_log_info("found a peb in transfered status");
        for (i = 0; i < reclaim_idx; i++) {
            nvdm_modem_peb_reclaim(reclaiming_peb[i]);
        }
        nvdm_modem_peb_update_status(transfered_peb, PEB_STATUS_ACTIVED);
        nvdm_modem_port_poweroff(19);
    } else {
        if ((reclaim_idx > 0) || (transfered_peb >= 0) || (transfering_peb >= 0)) {
            nvdm_modem_port_log_error("reclaim_idx=%d, transfered_peb=%d, transfering_peb=%d",
                                      reclaim_idx, transfered_peb, transfering_peb);
            nvdm_modem_port_free(reclaiming_peb);
            return;
        }
    }

    /* calculate total valid data size */
    nvdm_modem_port_free(reclaiming_peb);
    nvdm_modem_port_log_info("calculate total valid data size");
    handle->valid_data_size = 0;
    for (i = 0; i < handle->nvdm_modem_peb_count; i++) {
        handle->valid_data_size += (g_nvdm_modem_peb_size - PEB_HEADER_SIZE) - handle->pebs_info[i].free - handle->pebs_info[i].dirty;
    }
}

nvdm_modem_status_t nvdm_modem_init(void)
{
    peb_handle_t *handle;
    int num;

    nvdm_modem_port_log_info("nvdm_modem_init:%d", g_nvdm_modem_init_status);
    
    if (g_nvdm_modem_init_status == true) {
        return NVDM_MODEM_STATUS_ERROR;
    }

    nvdm_modem_port_poweroff_time_set();

    nvdm_modem_data_item_init();

    for (num = 0; num < sizeof(peb_handle) / sizeof(peb_handle[0]); num++) {
        nvdm_modem_port_set_working_area((nvdm_modem_area_t)(0x1 << num));
        handle = &peb_handle[num];

        nvdm_modem_port_log_info("======area:%d", 0x1 << num);
        
        g_nvdm_modem_peb_size = nvdm_modem_port_get_peb_config(&handle->nvdm_modem_peb_count);
        if (handle->nvdm_modem_peb_count < 2) {
            nvdm_modem_port_log_error("Count of PEB for NVDM region must greater than or equal to 2");
            return NVDM_MODEM_STATUS_ERROR;
        }

        handle->total_avail_space = nvdm_modem_calculate_total_avail_space();
        handle->pebs_info = nvdm_modem_port_malloc(handle->nvdm_modem_peb_count * sizeof(peb_info_t));
        if (handle->pebs_info == NULL) {
            nvdm_modem_port_log_error("alloc peb_info fail:num=%d", num);
            return NVDM_MODEM_STATUS_ERROR;
        }
        memset(handle->pebs_info, 0, handle->nvdm_modem_peb_count * sizeof(peb_info_t));

        nvdm_modem_peb_scan();
        //nvdm_modem_peb_print_info();

        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    }

    nvdm_modem_port_mutex_create();

	nvdm_modem_otp_init();

    g_nvdm_modem_init_status = true;

    nvdm_modem_port_log_info("nvdm modem init finished\n");

    return NVDM_MODEM_STATUS_OK;
}

#endif

