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
#include "nvdm_modem_port.h"
#include "nvdm_modem_internal.h"


/********************************************************
 * Macro & Define
 *
 ********************************************************/
#define AREA_IDX    (nvdm_modem_port_get_working_area(true) >> 1)

/********************************************************
 * Enum & Structures & Global variable
 *
 ********************************************************/
typedef struct {
    data_item_header_t *data_item_headers;
    uint32_t sum_data_item_headers;
    uint32_t write_sequence_number;
} data_item_handle_t;

/* Declare data item handler for each area*/
static data_item_handle_t data_handle[(NVDM_MODEM_AREA_END >> 1) + 1];

static uint8_t  g_working_buffer[NVDM_BUFFER_SIZE];
uint32_t g_modem_max_data_item_num;
uint32_t g_modem_max_data_item_size;
uint32_t g_modem_max_group_name_size;
uint32_t g_modem_max_data_item_name_size;

/********************************************************
 * Function declaration
 *
 ********************************************************/

static void data_item_header_print_info(data_item_header_t *header)
{
    nvdm_modem_port_log_info("data item header info show below:");
    nvdm_modem_port_log_info(" status: 0x%02x", header->status);
    nvdm_modem_port_log_info(" pnum: %d", header->pnum);
    nvdm_modem_port_log_info(" offset: 0x%04x", header->offset);
    nvdm_modem_port_log_info(" sequence_number: %d", header->sequence_number);
    nvdm_modem_port_log_info(" group_name_size: %d", header->group_name_size);
    nvdm_modem_port_log_info(" data_item_name_size: %d", header->data_item_name_size);
    nvdm_modem_port_log_info(" value_size: %d", header->value_size);
    nvdm_modem_port_log_info(" index: %d", header->index);
    nvdm_modem_port_log_info(" type: %d", header->type);
    nvdm_modem_port_log_info(" hash_name: 0x%08x", header->hash_name);
}

static int32_t find_empty_data_item()
{
    data_item_handle_t *handle = &data_handle[AREA_IDX];
    int32_t i;

    for (i = 0; i < g_modem_max_data_item_num; i++) {
        if (handle->data_item_headers[i].value_size == 0) {
            return i;
        }
    }

    return -1;
}

static void nvdm_modem_dump_data_items()
{
    data_item_handle_t *handle = &data_handle[AREA_IDX];
    data_item_header_t *headers = handle->data_item_headers;
    uint32_t i;
    uint8_t gname[g_modem_max_group_name_size], dname[g_modem_max_data_item_name_size];

    nvdm_modem_port_log_notice("[Dump data start]: area=%d", AREA_IDX);
    for (i=0; i<g_modem_max_data_item_num; i++) {
        if (headers[i].status == DATA_ITEM_STATUS_EMPTY || headers[i].status == 0) continue;

        nvdm_modem_peb_read_data(headers[i].pnum, headers[i].offset+DATA_ITEM_HEADER_SIZE, gname, headers[i].group_name_size);
        nvdm_modem_peb_read_data(headers[i].pnum, headers[i].offset+DATA_ITEM_HEADER_SIZE+headers[i].group_name_size, dname, headers[i].data_item_name_size);

        gname[headers[i].group_name_size] = 0;
        dname[headers[i].data_item_name_size] = 0;

        nvdm_modem_port_log_notice(" [Data:%3d]status=0x%02x, pnum=%02d, verno=%d, offset=%04x, size=%3d, g_name=%s, d_name=%s\r\n",
                                                    i,
                                                    headers[i].status,
                                                    headers[i].pnum,
                                                    headers[i].verno,
                                                    headers[i].offset,
                                                    headers[i].value_size,
                                                    gname,
                                                    dname);
    }
    nvdm_modem_port_log_notice("[Dump data end]: area=%d", AREA_IDX);

}

static uint16_t nvdm_modem_calculate_checksum(uint16_t checksum, const uint8_t *buffer, int32_t size)
{
    uint8_t *byte_checksum;
    int32_t i;

    byte_checksum = (uint8_t *)&checksum;

    for (i = 0; i < size; i++) {
        if (i & 0x01) {
            *(byte_checksum + 1) += *(buffer + i);
        } else {
            *byte_checksum += *(buffer + i);
        }
    }

    return checksum;
}

static uint16_t nvdm_modem_calculate_data_item_checksum(data_item_header_t *header, int32_t pnum, int32_t position, uint8_t *data_buf, uint16_t data_size)
{
    int32_t i, offset, fragment;
    uint16_t checksum;
    uint8_t *working_buffer = g_working_buffer;

    checksum = 0;

    /* checksum for data item's header
        * skip frist byte because it's not calculated by checksum.
        */
    checksum = nvdm_modem_calculate_checksum(checksum, &header->pnum, DATA_ITEM_HEADER_SIZE - 1);

    /* add checksum for group name and data item name */
    offset = position;
    nvdm_modem_peb_read_data(pnum, offset, working_buffer, header->group_name_size + header->data_item_name_size);
    checksum = nvdm_modem_calculate_checksum(checksum, working_buffer, header->group_name_size + header->data_item_name_size);

    /* add checksum for data item's value */
    if (data_buf && data_size != 0) {
        checksum = nvdm_modem_calculate_checksum(checksum, data_buf, data_size);
    }else {
        offset += header->group_name_size + header->data_item_name_size;
        fragment = header->value_size / NVDM_BUFFER_SIZE;
        for (i = 0; i < fragment; i++) {
            memset(working_buffer, 0, NVDM_BUFFER_SIZE);
            nvdm_modem_peb_read_data(pnum, offset, working_buffer, NVDM_BUFFER_SIZE);
            checksum = nvdm_modem_calculate_checksum(checksum, working_buffer, NVDM_BUFFER_SIZE);
            offset += NVDM_BUFFER_SIZE;
        }
        if (header->value_size % NVDM_BUFFER_SIZE) {
            memset(working_buffer, 0, NVDM_BUFFER_SIZE);
            nvdm_modem_peb_read_data(pnum, offset, working_buffer, header->value_size % NVDM_BUFFER_SIZE);
            checksum = nvdm_modem_calculate_checksum(checksum, working_buffer, header->value_size % NVDM_BUFFER_SIZE);
        }
    }

    return checksum;
}

static int32_t nvdm_modem_search_data_item_by_name(const char *group_name, const char *data_item_name, uint32_t *hasename)
{
    data_item_handle_t *handle = &data_handle[AREA_IDX];
    data_item_header_t *header;
    int32_t i, len = 0;
    uint32_t hash, a = 63689, b = 378551;
    char str[64];

    for (i=0; *(group_name+i)!='\0'; i++)
        str[i] = *(group_name+i);
    len += i;
    for (i=0; *(data_item_name+i)!='\0'; i++)
        str[i+len] = *(data_item_name+i);
    len += i;
    str[len] = '\0';

    hash = 0;
    for (i = 0; i < len; i++) {
        hash = hash * a + str[i];
        a = a * b;
    }

    if (hasename != NULL) {
        *hasename = hash;
    }
    nvdm_modem_port_log_info("hashname = 0x%08x", hash);

    for (i = 0; i < g_modem_max_data_item_num; i++) {
        //nvdm_modem_port_log_info("(%d) value_size=%d, status=0x%x", i, handle->data_item_headers[i].value_size, handle->data_item_headers[i].status);
        if (handle->data_item_headers[i].value_size == 0) {
            continue;
        }
        if (hash == handle->data_item_headers[i].hash_name) {
            header = &handle->data_item_headers[i];
			nvdm_modem_peb_read_data(header->pnum,
                                     header->offset + DATA_ITEM_HEADER_SIZE,
                                     (uint8_t *)str,
                                     header->group_name_size + header->data_item_name_size);
			if (strncmp(str, group_name, header->group_name_size)) {
                continue;
            }
			if (strncmp(str+header->group_name_size, data_item_name, header->data_item_name_size) == 0) {
                return i;
            }
        }
    }

    return -1;
}

nvdm_modem_status_t nvdm_modem_read_data_item(const char *group_name,
        const char *data_item_name,
        nvdm_modem_data_item_type_t *type,
        uint8_t *buffer,
        uint32_t *size)
{
    data_item_handle_t *handle = &data_handle[AREA_IDX];
    data_item_header_t *header;
    int32_t index;
    uint32_t offset;
    uint16_t checksum1, checksum2;

    nvdm_modem_port_log_notice("nvdm_modem_read_data_item: begin to read:[%s,%s,%d]", group_name, data_item_name, AREA_IDX);

    if ((group_name == NULL) ||
            (data_item_name == NULL) ||
            (buffer == NULL) ||
            (size == NULL) ||
            (*size == 0)) {
        if (size != NULL) {
            *size = 0;
        }
        nvdm_modem_port_log_notice("NV_RD:INVALID_PARA1");
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    if ((strlen(group_name) > g_modem_max_group_name_size) ||
            (strlen(data_item_name) > g_modem_max_data_item_name_size)) {
        nvdm_modem_port_log_notice("NV_RD:INVALID_PARA2");
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    if (g_nvdm_modem_init_status == false) {
        nvdm_modem_port_log_notice("NV_RD:INVALID_PARA3");
        return NVDM_MODEM_STATUS_ERROR;
    }

    index = nvdm_modem_search_data_item_by_name(group_name, data_item_name, NULL);
    if (index < 0) {
        *size = 0;
        nvdm_modem_port_log_notice("NV_RD:ITEM_NOT_FOUND");
        return NVDM_MODEM_STATUS_ITEM_NOT_FOUND;
    }
    header = &handle->data_item_headers[index];

    /* check whether buffer size is enough */
    if (*size < handle->data_item_headers[index].value_size) {
        *size = 0;
        nvdm_modem_port_log_notice("NV_RD:INVALID_PARA4");
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

	/* Read data to user buffer */
	offset = header->offset + DATA_ITEM_HEADER_SIZE + header->group_name_size + header->data_item_name_size;
	nvdm_modem_peb_read_data(header->pnum, offset, buffer, header->value_size);

    /* verify checksum of date item */
    checksum1 = nvdm_modem_calculate_data_item_checksum(header,
                                             header->pnum,
                                             header->offset + DATA_ITEM_HEADER_SIZE,
                                             buffer,
                                             header->value_size);

    offset = header->offset + DATA_ITEM_HEADER_SIZE + header->group_name_size + header->data_item_name_size + header->value_size;
    nvdm_modem_peb_read_data(header->pnum, offset, (uint8_t *)&checksum2, DATA_ITEM_CHECKSUM_SIZE);
    if (checksum1 != checksum2) {
        memset(buffer, 0, *size);
        *size = 0;
        nvdm_modem_port_log_notice("NV_RD:CHECKSUM_FAILED");
        return NVDM_MODEM_STATUS_INCORRECT_CHECKSUM;
    }

    memset(buffer + header->value_size, 0, *size - header->value_size);
    *size = header->value_size;

    if (type) {
        *type = handle->data_item_headers[index].type;
    }

    nvdm_modem_port_log_info("group_name = %s, data_item_name = %s, size = %d", group_name, data_item_name, handle->data_item_headers[index].value_size);

    return NVDM_MODEM_STATUS_OK;
}

nvdm_modem_status_t nvdm_modem_write_data_item(const char *group_name,
        const char *data_item_name,
        nvdm_modem_data_item_type_t type,
        const uint8_t *buffer,
        uint32_t size)
{
    data_item_handle_t *handle = &data_handle[AREA_IDX];
    int32_t append, peb_status_update;
    int32_t added_size, alloc_size, group_name_size, data_item_name_size;
    int32_t index, pnum, old_pnum, offset, old_offset;
    uint32_t hashname;
    uint16_t checksum;
    uint8_t *working_buffer;
    data_item_header_t *p_data_item_header;
    data_item_header_t data_item_header;
    data_item_status_t status;
    int32_t ret;

    nvdm_modem_port_log_notice("nvdm_modem_write_data_item: begin to write:[%s,%s,%d]", group_name, data_item_name, AREA_IDX);

    if ((group_name == NULL) ||
            (data_item_name == NULL) ||
            (buffer == NULL) ||
            (size > g_modem_max_data_item_size) ||
            (size == 0)) {
        nvdm_modem_port_log_notice("NV_WR:INVALID_PARA1");
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    if ((type != NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA) &&
            (type != NVDM_MODEM_DATA_ITEM_TYPE_STRING)) {
        nvdm_modem_port_log_notice("NV_WR:INVALID_PARA2");
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    if ((strlen(group_name) > g_modem_max_group_name_size) ||
            (strlen(data_item_name) > g_modem_max_data_item_name_size)) {
        nvdm_modem_port_log_notice("NV_WR:INVALID_PARA3");
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    if (g_nvdm_modem_init_status == false) {
        nvdm_modem_port_log_notice("NV_WR:UN_INIT");
        return NVDM_MODEM_STATUS_ERROR;
    }

    group_name_size = strlen(group_name) + 1;
    data_item_name_size = strlen(data_item_name) + 1;

    index = nvdm_modem_search_data_item_by_name(group_name, data_item_name, &hashname);
    nvdm_modem_port_log_info("find_data_item_by_hashname return %d", index);
    if (index < 0) {
        append = 1;
        /* find a empty position to fill in */
        index = find_empty_data_item();
    } else {
        append = 0;
    }

    /* check whether we have enough free space for append */
    if (append) {
        added_size = size + group_name_size + data_item_name_size + DATA_ITEM_HEADER_SIZE + DATA_ITEM_CHECKSUM_SIZE;
    } else {
        added_size = (int32_t)size - (int32_t)handle->data_item_headers[index].value_size;
    }
    ret = nvdm_modem_space_is_enough(added_size);
    if (ret == false) {
        nvdm_modem_port_log_notice("peb free space is not enough\n");
        nvdm_modem_dump_data_items();
        nvdm_modem_port_log_notice("NV_WR:INSUFF_SPACE1");
        return NVDM_MODEM_STATUS_INSUFFICIENT_SPACE;
    }

    /* find a peb with require free space to place data item */
    alloc_size = size + group_name_size + data_item_name_size + DATA_ITEM_HEADER_SIZE + DATA_ITEM_CHECKSUM_SIZE;
    pnum = nvdm_modem_space_allocation(alloc_size, added_size, &offset);
    if (pnum < 0) {
        nvdm_modem_port_log_notice("space_allocation fail:%d", alloc_size);
        nvdm_modem_dump_data_items();
        nvdm_modem_port_log_notice("NV_WR:INSUFF_SPACE2");
        return NVDM_MODEM_STATUS_INSUFFICIENT_SPACE;
    }

    if (append) {
        nvdm_modem_port_log_info("new data item append");
        p_data_item_header = &handle->data_item_headers[index];
#ifdef MTK_NVDM_OTA_SUPPORT
        p_data_item_header->verno = nvdm_get_modem_item_verno(group_name, data_item_name);
#endif
        p_data_item_header->reserved = 0xFF;
        p_data_item_header->type = type;
        p_data_item_header->hash_name = hashname;
        p_data_item_header->value_size = size;
        p_data_item_header->index = index;
        handle->sum_data_item_headers++;
        p_data_item_header->sequence_number = ++handle->write_sequence_number;
        p_data_item_header->pnum = pnum;
        p_data_item_header->group_name_size = group_name_size;
        p_data_item_header->data_item_name_size = data_item_name_size;
        p_data_item_header->offset = offset;
        data_item_header_print_info(p_data_item_header);
    } else {
        nvdm_modem_port_log_info("old data item overwrite");
        p_data_item_header = &handle->data_item_headers[index];
        p_data_item_header->sequence_number = ++handle->write_sequence_number;
        old_pnum = p_data_item_header->pnum;
        p_data_item_header->pnum = pnum;
        old_offset = p_data_item_header->offset;
        p_data_item_header->offset = offset;
        p_data_item_header->value_size = size;
        data_item_header_print_info(&handle->data_item_headers[index]);
    }

    if (handle->sum_data_item_headers > g_modem_max_data_item_num) {
        nvdm_modem_port_log_error("too many data items in nvdm region\n");
        return NVDM_MODEM_STATUS_ERROR;
    }

    nvdm_modem_port_log_info("group_name = %s, data_item_name = %s, size = %d", group_name, data_item_name, size);

    /* calculate checksum for new data item copy */
    checksum = 0;
    /* DATA_ITEM_HEADER_SIZE-1 must be power of 2 */
    checksum = nvdm_modem_calculate_checksum(checksum, &p_data_item_header->pnum, DATA_ITEM_HEADER_SIZE - 1);
    working_buffer = g_working_buffer;
    memcpy(working_buffer, group_name, group_name_size);
    working_buffer += group_name_size;
    memcpy(working_buffer, data_item_name, data_item_name_size);
    working_buffer -= group_name_size;
    checksum = nvdm_modem_calculate_checksum(checksum, working_buffer, (group_name_size + data_item_name_size));
    checksum = nvdm_modem_calculate_checksum(checksum, buffer, size);

    /* this peb is frist written, so status of PEB need to be modified */
    if (nvdm_modem_peb_activing(pnum)) {
        peb_status_update = 1;
    } else {
        peb_status_update = 0;
    }

    nvdm_modem_peb_sub_free(pnum, alloc_size);

    /* set status of data item to writing */
    status = DATA_ITEM_STATUS_WRITING;
    nvdm_modem_peb_write_data(pnum, offset, (uint8_t *)&status, 1);
    nvdm_modem_port_poweroff(1);
    /* write header of data item (not including status) */
    offset += 1;
    nvdm_modem_peb_write_data(pnum, offset, &p_data_item_header->pnum, DATA_ITEM_HEADER_SIZE - 1);
    /* write group name and data item name */
    offset += DATA_ITEM_HEADER_SIZE - 1;
    nvdm_modem_peb_write_data(pnum, offset, working_buffer, group_name_size + data_item_name_size);
    /* write value of data item */
    offset += group_name_size + data_item_name_size;
    nvdm_modem_peb_write_data(pnum, offset, (uint8_t *)buffer, size);
    /* write checksum of data item */
    offset += size;
    nvdm_modem_peb_write_data(pnum, offset, (uint8_t *)&checksum, DATA_ITEM_CHECKSUM_SIZE);
    /* set status of data item to valid */
    offset -= p_data_item_header->value_size + DATA_ITEM_HEADER_SIZE + group_name_size + data_item_name_size;
    status = DATA_ITEM_STATUS_VALID;
    nvdm_modem_peb_write_data(pnum, offset, (uint8_t *)&status, 1);
    p_data_item_header->status = DATA_ITEM_STATUS_VALID;
    nvdm_modem_port_poweroff(2);

    if (peb_status_update) {
        /* now we have at least one data item in PEB,
              * so update it's status to PEB_STATUS_ACTIVED
              */
        nvdm_modem_peb_update_status(pnum, PEB_STATUS_ACTIVED);
        nvdm_modem_port_poweroff(3);
    }

    if (!append) {
        /* because we have write new copy successfully,
              * so we can invalidate old copy now!
              */
        status = DATA_ITEM_STATUS_DELETE;
        nvdm_modem_peb_write_data(old_pnum, old_offset, (uint8_t *)&status, 1);
        nvdm_modem_port_poweroff(4);
        /* mark drity for old copy */
        nvdm_modem_peb_read_data(old_pnum, old_offset, (uint8_t *)&data_item_header, DATA_ITEM_HEADER_SIZE);
        size = DATA_ITEM_HEADER_SIZE +
               data_item_header.group_name_size +
               data_item_header.data_item_name_size +
               data_item_header.value_size +
               DATA_ITEM_CHECKSUM_SIZE;
        nvdm_modem_peb_add_drity(old_pnum, size);
    }

    return NVDM_MODEM_STATUS_OK;
}

void nvdm_modem_data_item_scan(int32_t pnum)
{
    data_item_handle_t *handle = &data_handle[AREA_IDX];
    int32_t offset, oldpnum;
    int32_t peb_drity, peb_valid;
    data_item_header_t data_item_header;
    /*uint16_t checksum1, checksum2;*/
    uint16_t size;
    static int32_t abnormal_data_item = -1;
    data_item_status_t status;
    int32_t i;
    uint8_t group_name[16];
    uint8_t data_item_name[32];

    nvdm_modem_port_log_info("scanning pnum(%d) to analysis data item info", pnum);
    offset = 0;
    peb_drity = 0;
    peb_valid = 0;
    /* scan entire peb content */
    while (offset < (g_nvdm_modem_peb_size - PEB_HEADER_SIZE - DATA_ITEM_HEADER_SIZE)) {
        nvdm_modem_peb_read_data(pnum, offset, (uint8_t *)&data_item_header, DATA_ITEM_HEADER_SIZE);
        data_item_header_print_info(&data_item_header);
        size = DATA_ITEM_HEADER_SIZE + data_item_header.group_name_size + data_item_header.data_item_name_size + data_item_header.value_size + DATA_ITEM_CHECKSUM_SIZE;
        switch (data_item_header.status) {
            case DATA_ITEM_STATUS_EMPTY:
                nvdm_modem_peb_add_free(pnum, (g_nvdm_modem_peb_size - PEB_HEADER_SIZE) - peb_drity - peb_valid);
                return;
            case DATA_ITEM_STATUS_WRITING:
                /* we can't belive data item header if we found it's writting,
                          * so just mark rest of space is dirty.
                          */
                nvdm_modem_peb_add_drity(pnum, (g_nvdm_modem_peb_size - PEB_HEADER_SIZE) - peb_drity - peb_valid);
                return;
            case DATA_ITEM_STATUS_VALID:
                break;
            case DATA_ITEM_STATUS_DELETE:
                peb_drity += size;
                offset += size;
                nvdm_modem_peb_add_drity(pnum, size);
                continue;
            default:
                nvdm_modem_port_log_error("pnum=%d, offset=0x%x", pnum, offset);
                return;
        }

        if (data_item_header.pnum != pnum) {
            status = DATA_ITEM_STATUS_DELETE;
            nvdm_modem_peb_write_data(pnum,
                                      offset,
                                      (uint8_t *)&status,
                                      1);
            peb_drity += size;
            offset += size;
            nvdm_modem_peb_add_drity(pnum, size);
            continue;
        }

#ifdef MTK_NVDM_OTA_SUPPORT
        /* verify data version after OTA */
        for(i=0; i<sizeof(group_name); i++) group_name[i] = 0;
        for(i=0; i<sizeof(data_item_name); i++) data_item_name[i] = 0;

        nvdm_modem_peb_read_data(pnum, data_item_header.offset + DATA_ITEM_HEADER_SIZE, &group_name[0], data_item_header.group_name_size);
        nvdm_modem_peb_read_data(pnum, data_item_header.offset + DATA_ITEM_HEADER_SIZE + data_item_header.group_name_size, &data_item_name[0], data_item_header.data_item_name_size);

        if (data_item_header.verno != nvdm_get_modem_item_verno((char *)group_name, (char *)data_item_name)) {
            /* change the status of data item in flash from valid to delete */
            status = DATA_ITEM_STATUS_DELETE;

            nvdm_modem_peb_write_data(pnum,
                           data_item_header.offset,
                           (uint8_t *)&status,
                           1);

            nvdm_modem_port_log_notice("[%s,%s]Deleter old NV after OTA\n", group_name , data_item_name);
            /* recalculate the dirty value of that PEB */
            peb_drity += size;
            offset += size;
            nvdm_modem_peb_add_drity(pnum, size);
            continue;
        }
#endif

#if 0 /* Ignore checksum inspection to speed up booting time, this can be done again durning READ operation*/
        /* verify checksum of data item */
        checksum1 = nvdm_modem_calculate_data_item_checksum(&data_item_header, pnum, data_item_header.offset + DATA_ITEM_HEADER_SIZE, NULL, 0);
        offset += DATA_ITEM_HEADER_SIZE + data_item_header.group_name_size + data_item_header.data_item_name_size + data_item_header.value_size;
        nvdm_modem_peb_read_data(pnum, offset, (uint8_t *)&checksum2, DATA_ITEM_CHECKSUM_SIZE);
        offset += DATA_ITEM_CHECKSUM_SIZE;
        if (checksum1 != checksum2) {
            nvdm_modem_port_log_info("detect checksum error\n");
            peb_drity += size;
            nvdm_modem_peb_add_drity(pnum, size);
            continue;
        }
#else
        offset += DATA_ITEM_HEADER_SIZE + data_item_header.group_name_size + data_item_header.data_item_name_size + data_item_header.value_size + DATA_ITEM_CHECKSUM_SIZE;
#endif
        /* update max write sequence number */
        if (handle->write_sequence_number < data_item_header.sequence_number) {
            handle->write_sequence_number = data_item_header.sequence_number;
        }
        /* update count of data items */
        if (handle->data_item_headers[data_item_header.index].sequence_number == MAX_WRITE_SEQUENCE_NUMBER) {
            /* we find this frist time */
            memcpy(&handle->data_item_headers[data_item_header.index], &data_item_header, sizeof(data_item_header_t));
            handle->sum_data_item_headers++;
            if (handle->sum_data_item_headers > g_modem_max_data_item_num) {
                nvdm_modem_port_log_error("too many data items in nvdm region\n");
                return;
            }
            peb_valid += size;
        } else {
            /* we found it before, so compare sequence number of them
                    * this is possible that new copy is total update
                    * but old copy has not been invalidated when power-off happen.
                    */
            nvdm_modem_port_log_info("detect two valid copy of data item");
            nvdm_modem_port_log_info("copy1(pnum=%d, offset=0x%04x), copy2(pnum=%d, offset=0x%04x)\n",
                                     handle->data_item_headers[data_item_header.index].pnum,
                                     handle->data_item_headers[data_item_header.index].offset,
                                     data_item_header.pnum,
                                     data_item_header.offset);
            if ((abnormal_data_item > 0) ||
                    (handle->data_item_headers[data_item_header.index].sequence_number == data_item_header.sequence_number)) {
                /* this should only happen once at most */
                nvdm_modem_port_log_error("abnormal_data_item = %d", abnormal_data_item);
                return;
            }
            abnormal_data_item = 1;
            if (handle->data_item_headers[data_item_header.index].sequence_number < data_item_header.sequence_number) {
                /* we find new copy, so mark data staus of old data as delete */
                status = DATA_ITEM_STATUS_DELETE;
                nvdm_modem_peb_write_data(handle->data_item_headers[data_item_header.index].pnum,
                                          handle->data_item_headers[data_item_header.index].offset,
                                          (uint8_t *)&status,
                                          1);
                nvdm_modem_port_poweroff(5);
                /* add valid info */
                peb_valid += size;
                /* add dirty info */
                oldpnum = handle->data_item_headers[data_item_header.index].pnum;
                size = handle->data_item_headers[data_item_header.index].value_size +
                       handle->data_item_headers[data_item_header.index].group_name_size +
                       handle->data_item_headers[data_item_header.index].data_item_name_size +
                       DATA_ITEM_CHECKSUM_SIZE + DATA_ITEM_HEADER_SIZE;
                nvdm_modem_peb_add_drity(oldpnum, size);
                /* if we found old copy in same peb, we must substract it's size from peb_valid */
                if (oldpnum == pnum) {
                    peb_valid -= size;
                }
                memcpy(&handle->data_item_headers[data_item_header.index], &data_item_header, sizeof(data_item_header_t));

                /* if we found it in the same peb last time */
                if (oldpnum == pnum) {
                    peb_drity += size;
                }
            } else {
                /* we find old copy, so mark it as delete directly */
                status = DATA_ITEM_STATUS_DELETE;
                nvdm_modem_peb_write_data(data_item_header.pnum,
                                          data_item_header.offset,
                                          (uint8_t *)&status,
                                          1);
                nvdm_modem_port_poweroff(6);
                peb_drity += size;
                nvdm_modem_peb_add_drity(pnum, size);
            }
        }
    }
    /* If there is dark space exist, it should also be considered as free space. */
    if (offset >= (g_nvdm_modem_peb_size - PEB_HEADER_SIZE - DATA_ITEM_HEADER_SIZE)) {
        nvdm_modem_peb_add_free(pnum, (g_nvdm_modem_peb_size - PEB_HEADER_SIZE) - peb_drity - peb_valid);
    }
}

void nvdm_modem_data_item_init(void)
{
    data_item_handle_t *handle;
    uint32_t i, num;;

    g_modem_max_data_item_num = nvdm_modem_port_get_data_item_config(&g_modem_max_data_item_size, &g_modem_max_group_name_size, &g_modem_max_data_item_name_size);
    if (g_modem_max_data_item_size > MAX_DATA_ITEM_SIZE) {
        nvdm_modem_port_log_error("Max size of data item must less than or equal to 2048 bytes");
        return;
    }

    for (num = 0; num < sizeof(data_handle) / sizeof(data_handle[0]); num++) {
        handle = &data_handle[num];
        handle->data_item_headers = (data_item_header_t *)nvdm_modem_port_malloc(g_modem_max_data_item_num * sizeof(data_item_header_t));
        if (handle->data_item_headers == NULL) {
            nvdm_modem_port_log_error("alloc data_item_headers fail");
            return;
        }

        memset(handle->data_item_headers, 0, g_modem_max_data_item_num * sizeof(data_item_header_t));
        for (i = 0; i < g_modem_max_data_item_num; i++) {
            handle->data_item_headers[i].sequence_number = MAX_WRITE_SEQUENCE_NUMBER;
        }
        handle->write_sequence_number = 0;
        handle->sum_data_item_headers = 0;
    }
}

static void nvdm_modem_data_migration(int32_t src_pnum, int32_t src_offset,
                                      int32_t dst_pnum, int32_t dst_offset, int32_t size)
{
    int32_t i, delta, fragment;
    uint8_t *working_buffer = g_working_buffer;

    fragment = size / NVDM_BUFFER_SIZE;
    delta = 0;
    for (i = 0; i < fragment; i++) {
        memset(working_buffer, 0, NVDM_BUFFER_SIZE);
        nvdm_modem_peb_read_data(src_pnum, src_offset + delta, working_buffer, NVDM_BUFFER_SIZE);
        nvdm_modem_peb_write_data(dst_pnum, dst_offset + delta, working_buffer, NVDM_BUFFER_SIZE);
        delta += NVDM_BUFFER_SIZE;
    }
    if (size % NVDM_BUFFER_SIZE) {
        memset(working_buffer, 0, NVDM_BUFFER_SIZE);
        nvdm_modem_peb_read_data(src_pnum, src_offset + delta, working_buffer, size % NVDM_BUFFER_SIZE);
        nvdm_modem_peb_write_data(dst_pnum, dst_offset + delta, working_buffer, size % NVDM_BUFFER_SIZE);
    }
}

int32_t nvdm_modem_data_item_migration(int32_t src_pnum, int32_t dst_pnum, int32_t offset)
{
    data_item_handle_t *handle = &data_handle[AREA_IDX];
    data_item_header_t data_item_header;
    data_item_status_t status;
    int32_t pos, size;
    uint16_t checksum;

    /* search valid data item */
    pos = 0;
    while (pos < (g_nvdm_modem_peb_size - PEB_HEADER_SIZE - DATA_ITEM_HEADER_SIZE)) {
        nvdm_modem_peb_read_data(src_pnum, pos, (uint8_t *)&data_item_header, DATA_ITEM_HEADER_SIZE);
        switch (data_item_header.status) {
            case DATA_ITEM_STATUS_WRITING:
            case DATA_ITEM_STATUS_EMPTY:
                /* no more data item after it, just return */
                return offset;
            case DATA_ITEM_STATUS_DELETE:
                /* do nothing, just skip it to find next data item.
                          * data item is marked as delete status, it must be an old copy.
                          */
                pos += DATA_ITEM_HEADER_SIZE +
                       data_item_header.group_name_size + data_item_header.data_item_name_size +
                       data_item_header.value_size +
                       DATA_ITEM_CHECKSUM_SIZE;
                break;
            case DATA_ITEM_STATUS_VALID:
                if (handle->data_item_headers[data_item_header.index].sequence_number != data_item_header.sequence_number) {
                    /* find old copy, this should not happen,
                                 * because it's fixed in init phase.
                                 */
                    nvdm_modem_port_log_error("old_src_pnum=%d, old_pos=0x%x, new_src_pnum=%d, new_pos=0x%x",
                                              src_pnum, pos,
                                              handle->data_item_headers[data_item_header.index].pnum,
                                              handle->data_item_headers[data_item_header.index].offset);
                    return 0;
                    /* update offset for next write */
                    //pos += DATA_ITEM_HEADER_SIZE + data_item_header.value_size + DATA_ITEM_CHECKSUM_SIZE;
                } else {
                    /* find up-to-date copy, so migrate it to target peb update header */
                    handle->data_item_headers[data_item_header.index].pnum = dst_pnum;
                    handle->data_item_headers[data_item_header.index].offset = offset;

                    /* calculate new checksum */
                    checksum = nvdm_modem_calculate_data_item_checksum(&handle->data_item_headers[data_item_header.index], src_pnum, data_item_header.offset + DATA_ITEM_HEADER_SIZE, NULL, 0);

                    /* mark writ of beginning */
                    status = DATA_ITEM_STATUS_WRITING;
                    nvdm_modem_peb_write_data(dst_pnum, offset, (uint8_t *)&status, 1);
                    nvdm_modem_port_poweroff(7);

                    /* write header of data item */
                    nvdm_modem_peb_write_data(dst_pnum, offset + 1, &handle->data_item_headers[data_item_header.index].pnum, DATA_ITEM_HEADER_SIZE - 1);

                    /* write group name, data item name and value of data item */
                    nvdm_modem_data_migration(src_pnum, pos + DATA_ITEM_HEADER_SIZE, dst_pnum, offset + DATA_ITEM_HEADER_SIZE, data_item_header.group_name_size + data_item_header.data_item_name_size + data_item_header.value_size);

                    /* write checksum of data item */
                    nvdm_modem_peb_write_data(dst_pnum, offset + DATA_ITEM_HEADER_SIZE + data_item_header.group_name_size + data_item_header.data_item_name_size + data_item_header.value_size, (uint8_t *)&checksum, DATA_ITEM_CHECKSUM_SIZE);

                    /* mark write of end */
                    status = DATA_ITEM_STATUS_VALID;
                    nvdm_modem_peb_write_data(dst_pnum, offset, (uint8_t *)&status, 1);
                    nvdm_modem_port_poweroff(8);

                    size = DATA_ITEM_HEADER_SIZE + data_item_header.group_name_size + data_item_header.data_item_name_size + data_item_header.value_size + DATA_ITEM_CHECKSUM_SIZE;

                    /* substract free size of target peb */
                    nvdm_modem_peb_sub_free(dst_pnum, size);

                    /* update offset for next write */
                    offset += size;
                    pos += size;
                }
                break;
            default:
                nvdm_modem_port_log_error("src_pnum=%d, pos=0x%x", src_pnum, pos);
                return 0;
        }
    }

    return offset;
}

nvdm_modem_status_t nvdm_modem_get_data_item_size(const char *group_name,
        const char *data_item_name,
        uint32_t *size)
{
    data_item_handle_t *handle = &data_handle[AREA_IDX];
    int32_t index;

    if ((group_name == NULL) || (data_item_name == NULL)) {
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    if ((strlen(group_name) > g_modem_max_group_name_size) ||
            (strlen(data_item_name) > g_modem_max_data_item_name_size)) {
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    if (g_nvdm_modem_init_status == false) {
        return NVDM_MODEM_STATUS_ERROR;
    }

    index = nvdm_modem_search_data_item_by_name(group_name, data_item_name, NULL);
    if (index < 0) {
        *size = 0;
        return NVDM_MODEM_STATUS_ITEM_NOT_FOUND;
    }

    *size = handle->data_item_headers[index].value_size;
    return NVDM_MODEM_STATUS_OK;
}

nvdm_modem_status_t nvdm_modem_delete_data_item(const char *group_name,
        const char *data_item_name)
{
    data_item_handle_t *handle = &data_handle[AREA_IDX];
    data_item_header_t *header;
    data_item_status_t status;
    int32_t size, index;

    nvdm_modem_port_log_info("nvdm_modem_delete_data_item: begin to invalidate:[%s,%s,%d]", group_name, data_item_name, AREA_IDX);

    if ((group_name == NULL) || (data_item_name == NULL)) {
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    if ((strlen(group_name) > g_modem_max_group_name_size) ||
            (strlen(data_item_name) > g_modem_max_data_item_name_size)) {
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    if (g_nvdm_modem_init_status == false) {
        return NVDM_MODEM_STATUS_ERROR;
    }

    index = nvdm_modem_search_data_item_by_name(group_name, data_item_name, NULL);
    if (index < 0) {
        return NVDM_MODEM_STATUS_ITEM_NOT_FOUND;
    }

    header = &handle->data_item_headers[index];
    status = DATA_ITEM_STATUS_DELETE;
    nvdm_modem_peb_write_data(header->pnum, header->offset, (uint8_t *)&status, 1);
    header->status = DATA_ITEM_STATUS_DELETE;
    nvdm_modem_port_poweroff(20);
    /* mark drity for invalid data */
    size = DATA_ITEM_HEADER_SIZE +
           header->group_name_size +
           header->data_item_name_size +
           header->value_size +
           DATA_ITEM_CHECKSUM_SIZE;
    nvdm_modem_peb_add_drity(header->pnum, size);
    /* free the data item header in memory */
    header->value_size = 0;
    handle->sum_data_item_headers--;

    return NVDM_MODEM_STATUS_OK;
}

nvdm_modem_status_t nvdm_modem_is_data_item_found(const char *group_name,
        const char *data_item_name)
{
    int32_t index;

    index = nvdm_modem_search_data_item_by_name(group_name, data_item_name, NULL);

    return (index < 0) ? NVDM_MODEM_STATUS_ITEM_NOT_FOUND : NVDM_MODEM_STATUS_OK;
}

nvdm_modem_status_t nvdm_modem_query_data_item_count(uint32_t *count)
{
    data_item_handle_t *handle = &data_handle[AREA_IDX];
    uint32_t i, cnt = 0;;

    for (i = 0; i < g_modem_max_data_item_num; i++) {
        if (handle->data_item_headers[i].value_size == 0) {
            continue;
        }
        cnt++;
    }
    *count = cnt;
    return NVDM_MODEM_STATUS_OK;
}

nvdm_modem_status_t nvdm_modem_query_all_data_item(nvdm_modem_data_item_info_t *info_list,
        uint32_t count)
{
    data_item_handle_t *handle = &data_handle[AREA_IDX];
    data_item_header_t *header = handle->data_item_headers;
    uint8_t  pnum, size, *working_buffer = g_working_buffer;;
    uint16_t offset;
    uint32_t i,idx=0;

    if (!info_list) {
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    nvdm_modem_query_data_item_count(&i);
    if (i > count) {
        return NVDM_MODEM_STATUS_INSUFFICIENT_SPACE;
    }

    for (i = 0; i < g_modem_max_data_item_num; i++) {
        if (header[i].value_size == 0) {
            continue;
        }

        pnum = header[i].pnum;
        offset = header[i].offset + DATA_ITEM_HEADER_SIZE;
        size = header[i].group_name_size + header[i].data_item_name_size;
        nvdm_modem_peb_read_data(pnum, offset, working_buffer, size);
        memcpy(info_list[idx].group_name, working_buffer, header[i].group_name_size);
        memcpy(info_list[idx].data_item_name, (working_buffer + header[i].group_name_size), header[i].data_item_name_size);
        info_list[idx].area = nvdm_modem_port_get_working_area(true);
        idx++;
    }
    return NVDM_MODEM_STATUS_OK;
}

#endif

