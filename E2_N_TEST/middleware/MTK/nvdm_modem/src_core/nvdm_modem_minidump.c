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

#include "nvdm_modem.h"
#include "nvdm_modem_internal.h"
#include "nvdm_modem_port.h"
#include "nvdm_modem_minidump.h"

#ifdef MTK_MINI_DUMP_ENABLE
/********************************************************
 * Macro & Define
 *
 ********************************************************/
#define MINIDUMP_STATIC_ALLOCATION

#define GET_MINIDUMP_PEB(I,P)       (I * g_peb_num_per_minidump + P)
#define MINIDUMP_INDEX_RESERVED     0xFF

#define MINIDUMP_HEADER_MAGIC       0x44554D50
/********************************************************
 * Enum & Structures & Global variable
 *
 ********************************************************/
typedef struct {
    uint32_t magic;         /* erase header magic number */
    uint16_t data_len;      /* mini-dump length */
    uint8_t index;          /* index for mini-dump items */
    uint8_t reserved;       /* reserved */
} __attribute__((packed)) minidump_header_t;

#define MINIDUMP_HEADER_SIZE    sizeof(minidump_header_t)

static bool g_nvdm_minidump_status = false;
static uint8_t g_peb_num_per_minidump;
static uint8_t g_minidump_number;
static uint8_t g_minidump_curr_index = 0;
#ifdef MINIDUMP_STATIC_ALLOCATION
static uint8_t g_minidump_array[10] = {MINIDUMP_INDEX_RESERVED,};
#else
static uint8_t *g_minidump_array = NULL;
#endif
/********************************************************
 * Function declaration
 *
 ********************************************************/
static void nvdm_modem_dump_minidump_header(uint8_t index)
{
    minidump_header_t header;
    nvdm_modem_peb_io_read(GET_MINIDUMP_PEB(index, 0), 0, (uint8_t *)&header, MINIDUMP_HEADER_SIZE);
    nvdm_modem_port_log_info("[MINIDUMP:%x]", index);
    nvdm_modem_port_log_info("  magic=%d", header.magic);
    nvdm_modem_port_log_info("  data_len=%d", header.data_len);
    nvdm_modem_port_log_info("  index=%d", header.index);
}

static void nvdm_modem_sort_mini_dump_index(uint8_t *buf, uint8_t num)
{
    uint32_t i, j, tmp;

    for (i = 0; i < num - 1; i++) {
        for (j = i + 1; j < num; j++) {
            if (buf[j] == MINIDUMP_INDEX_RESERVED) continue;
            if (buf[j] > buf[i] || buf[i] == MINIDUMP_INDEX_RESERVED) {
                tmp = buf[j];
                buf[j] = buf[i];
                buf[i] = tmp;
            }
        }
    }
}

static int16_t nvdm_modem_find_minidump_index(uint8_t rank)
{
    uint8_t i, buf[g_minidump_number];
    
    for (i=0; i<g_minidump_number; i++)
        buf[i] = g_minidump_array[i];

    /* Find index by rank from newest to oldest */
    nvdm_modem_sort_mini_dump_index(buf, g_minidump_number);
    
    for (i=0; i<g_minidump_number; i++)
        if (buf[rank] == g_minidump_array[i])
            break;
    return i;
}

nvdm_modem_status_t nvdm_modem_query_mini_dump_number_internal(uint8_t *dump_number)
{
    int i, num = 0;

    if (!g_nvdm_minidump_status) {
        nvdm_modem_mini_dump_init();
    }

    for (i = 0; i < g_minidump_number; i++) {
        if (g_minidump_array[i] != MINIDUMP_INDEX_RESERVED) {
            num++;
        }
    }
    *dump_number = num;
    return NVDM_MODEM_STATUS_OK;
}

nvdm_modem_status_t nvdm_modem_clean_mini_dump_data_internal(uint8_t dump_index)
{
    int16_t idx;
    uint8_t peb;

    if (!g_nvdm_minidump_status) {
        nvdm_modem_mini_dump_init();
    }

    if (dump_index >= g_minidump_number) {
        nvdm_modem_port_log_notice("[MDUMP][ERR]CLEAN dump parameter failed");
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    idx = nvdm_modem_find_minidump_index(dump_index);
    if (g_minidump_array[idx] == MINIDUMP_INDEX_RESERVED) {
        nvdm_modem_port_log_notice("[MDUMP][ERR]dump_index no-exist:%d", dump_index);
        return NVDM_MODEM_STATUS_ITEM_NOT_FOUND;        
    }

    for (peb = 0; peb < g_peb_num_per_minidump; peb++) {
        nvdm_modem_peb_io_erase(GET_MINIDUMP_PEB(idx, peb));
    }

    g_minidump_array[idx] = MINIDUMP_INDEX_RESERVED;

    return NVDM_MODEM_STATUS_OK;
}

nvdm_modem_status_t nvdm_modem_write_mini_dump_data_internal(uint8_t *buffer, uint16_t size)
{
    minidump_header_t header;
    uint32_t offset, len;
    uint8_t idx, peb;

    if (!g_nvdm_minidump_status) {
        nvdm_modem_mini_dump_init();
    }

    if (!buffer || !size || size > (g_peb_num_per_minidump * g_nvdm_modem_peb_size - MINIDUMP_HEADER_SIZE)) {
        nvdm_modem_port_log_notice("[MDUMP][ERR]write size error");
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    /* Find free or oldest dump index */
    idx = nvdm_modem_find_minidump_index(g_minidump_number-1);

    /* Erase dump pebs */
    for (peb = 0; peb < g_peb_num_per_minidump; peb++) {
        nvdm_modem_peb_io_erase(GET_MINIDUMP_PEB(idx, peb));
    }

    /* Erase & Write dump data */
    offset = 0;
    for (peb = 0; peb < g_peb_num_per_minidump && offset < size; peb++) {
        if (peb == 0) {
            len = size > (g_nvdm_modem_peb_size - MINIDUMP_HEADER_SIZE) ? (g_nvdm_modem_peb_size - MINIDUMP_HEADER_SIZE) : size;
            nvdm_modem_peb_io_write(GET_MINIDUMP_PEB(idx, peb), MINIDUMP_HEADER_SIZE, buffer + offset, len);
        } else {
            len = (size - offset) > g_nvdm_modem_peb_size ? g_nvdm_modem_peb_size : (size - offset);
            nvdm_modem_peb_io_write(GET_MINIDUMP_PEB(idx, peb), 0, buffer + offset, len);
        }
        offset += len;
    }

    /* Write header */
    header.magic = MINIDUMP_HEADER_MAGIC;
    header.data_len = size;
    header.index = g_minidump_curr_index;
    header.reserved = 0xFF;
    nvdm_modem_peb_io_write(GET_MINIDUMP_PEB(idx, 0), 0, (uint8_t *)&header, MINIDUMP_HEADER_SIZE);
    g_minidump_array[idx] = g_minidump_curr_index;
    g_minidump_curr_index = (g_minidump_curr_index+1) == MINIDUMP_INDEX_RESERVED ? 0 : (g_minidump_curr_index+1);

    nvdm_modem_dump_minidump_header(idx);
    return NVDM_MODEM_STATUS_OK;
}

nvdm_modem_status_t nvdm_modem_read_mini_dump_data_internal(uint8_t dump_index, uint16_t offset, uint8_t *buffer, uint16_t *size)
{
    minidump_header_t mheader;
    uint8_t dump_idx;
    uint16_t peb_offset, peb_idx, peb_read_len, peb_rest_size, read_size, buf_idx = 0;

    if (!g_nvdm_minidump_status) {
        nvdm_modem_mini_dump_init();
    }

    if (dump_index > g_minidump_number || !buffer || !*size) {
        nvdm_modem_port_log_notice("[MDUMP][ERR]read para error(%d,%d,0x%x,%d)", dump_index, offset, buffer, *size);
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    /* reach end of dump data*/
    if (offset >= (g_peb_num_per_minidump * g_nvdm_modem_peb_size - MINIDUMP_HEADER_SIZE)) {
        *size = 0;
        return NVDM_MODEM_STATUS_OK;
    }

    /* find index */
    dump_idx = nvdm_modem_find_minidump_index(dump_index);
   
    /* check header*/
    nvdm_modem_peb_io_read(GET_MINIDUMP_PEB(dump_idx, 0), 0, (uint8_t *)&mheader, MINIDUMP_HEADER_SIZE);
    if (mheader.magic != MINIDUMP_HEADER_MAGIC || mheader.index == MINIDUMP_INDEX_RESERVED) {
        nvdm_modem_port_log_notice("[MDUMP][ERR]read item not found");
        return NVDM_MODEM_STATUS_ITEM_NOT_FOUND;
    }

    /* read dump*/
    peb_offset = (offset + MINIDUMP_HEADER_SIZE) % g_nvdm_modem_peb_size;
    peb_idx = (offset + MINIDUMP_HEADER_SIZE) / g_nvdm_modem_peb_size;
    read_size = (mheader.data_len - offset) > *size ? *size : (mheader.data_len - offset);

    for (; (peb_idx < g_peb_num_per_minidump) && (buf_idx < read_size); peb_idx++) {
        peb_rest_size = (g_nvdm_modem_peb_size - peb_offset);

        peb_read_len = peb_rest_size > (read_size - buf_idx) ? (read_size - buf_idx) : peb_rest_size;
        nvdm_modem_peb_io_read(GET_MINIDUMP_PEB(dump_idx, peb_idx), peb_offset, (buffer + buf_idx), peb_read_len);

        buf_idx += peb_read_len;
        peb_offset = 0;
    }
    
    *size = read_size;

    return NVDM_MODEM_STATUS_OK;
}

void nvdm_modem_mini_dump_init(void)
{
    int i;
    minidump_header_t mheader;
    uint32_t peb_count;

    if (g_nvdm_minidump_status) {
        return;
    }

    g_minidump_number = (uint8_t)nvdm_modem_port_get_peb_config(&peb_count);
    if (g_minidump_number <= 0) return;
    g_peb_num_per_minidump = peb_count / g_minidump_number;
#ifdef MINIDUMP_STATIC_ALLOCATION
    if (g_minidump_number > sizeof(g_minidump_array)/sizeof(g_minidump_array[0])) {
        g_minidump_number = sizeof(g_minidump_array)/sizeof(g_minidump_array[0]);
    }
#else
    g_minidump_array = nvdm_modem_port_malloc(g_minidump_number * sizeof(uint8_t));
    if (!g_minidump_array) {
        nvdm_modem_port_log_error("[MDUMP][ERR]init failed:mem_alloc");
        return;
    }
#endif
    for (i = 0; i < g_minidump_number; i++) {
        nvdm_modem_peb_io_read(GET_MINIDUMP_PEB(i, 0), 0, (uint8_t *)&mheader, MINIDUMP_HEADER_SIZE);
        if (mheader.magic != MINIDUMP_HEADER_MAGIC || mheader.index >= MINIDUMP_INDEX_RESERVED) {
            g_minidump_array[i] = MINIDUMP_INDEX_RESERVED;
        } else {
            g_minidump_array[i] = mheader.index;
        }
        if (mheader.index != MINIDUMP_INDEX_RESERVED && mheader.index >= g_minidump_curr_index) {
            g_minidump_curr_index = mheader.index + 1;
        }
    }

    nvdm_modem_port_log_notice("minidump_init:dump_num=%d,dump_peb_num=%d,curr_idx=%d", g_minidump_number, g_peb_num_per_minidump, g_minidump_curr_index);

    g_nvdm_minidump_status = true;
}

#endif /*MTK_MINI_DUMP_ENABLE*/
#endif /*MTK_NVDM_MODEM_ENABLE*/

