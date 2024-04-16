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
#define NVDM_MAX_OTP_DATA_ITEMS     (20)
#define NVDM_OTP_HEADER_LENGTH      sizeof(otp_data_header_t)
#define NVDM_OTP_DATA_NAME_LENGTH   (8)
#define NVDM_OTP_BUFFER_SIZE        (32)

/********************************************************
 * Enum & Structures & Global variable
 *
 ********************************************************/
typedef struct {
    data_item_status_t status;  /* status of data item */
    uint8_t index; /* index for data item */
    uint16_t offset; /* offset in PEB where data item record begin */
    uint16_t value_size; /* size of data item's content */
    uint16_t reserved; /* Reserved */
    uint8_t data_name[NVDM_OTP_DATA_NAME_LENGTH]; /* name of this data item */
} __attribute__((packed)) otp_data_header_t;

static otp_data_header_t g_otp_data_headers[NVDM_MAX_OTP_DATA_ITEMS] = {0};
static uint16_t g_otp_length = 0;
static uint16_t g_otp_free = 0;
static uint16_t g_otp_sum_data_headers = 0;
static uint8_t g_working_buffer[NVDM_OTP_BUFFER_SIZE];

extern bool g_nvdm_modem_init_status;
/********************************************************
 * Function declaration
 *
 ********************************************************/
static uint16_t calculate_checksum(uint16_t checksum, const uint8_t *buffer, int32_t size)
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

static int32_t find_empty_data_item()
{
    int32_t i;

    for (i = 0; i < NVDM_MAX_OTP_DATA_ITEMS; i++) {
        if (g_otp_data_headers[i].value_size == 0) {
            return i;
        }
    }

    return -1;
}

static uint16_t calculate_data_item_checksum(otp_data_header_t *header)
{
    int32_t i, offset, fragment;
    uint16_t checksum;
    uint8_t *working_buffer = g_working_buffer;

    checksum = 0;

    /* checksum for data item's header
        * skip frist byte because it's not calculated by checksum.
        */
    checksum = calculate_checksum(checksum, &header->index, NVDM_OTP_HEADER_LENGTH - 1);

    /* add checksum for data item's value */
    offset = header->offset + NVDM_OTP_HEADER_LENGTH;
    fragment = header->value_size / NVDM_OTP_BUFFER_SIZE;
    for (i = 0; i < fragment; i++) {
        memset(working_buffer, 0, NVDM_OTP_BUFFER_SIZE);
        nvdm_modem_port_otp_read(offset, working_buffer, NVDM_OTP_BUFFER_SIZE);
        checksum = calculate_checksum(checksum, working_buffer, NVDM_OTP_BUFFER_SIZE);
        offset += NVDM_OTP_BUFFER_SIZE;
    }
    if (header->value_size % NVDM_OTP_BUFFER_SIZE) {
        memset(working_buffer, 0, NVDM_OTP_BUFFER_SIZE);
        nvdm_modem_port_otp_read(offset, working_buffer, header->value_size % NVDM_OTP_BUFFER_SIZE);
        checksum = calculate_checksum(checksum, working_buffer, header->value_size % NVDM_OTP_BUFFER_SIZE);
    }

    return checksum;
}

static int32_t search_data_item_by_name(const char *data_item_name)
{
    int32_t i;

    for (i = 0; i < NVDM_MAX_OTP_DATA_ITEMS; i++) {
        if (g_otp_data_headers[i].value_size == 0) {
            continue;
        }
        if (strncmp(&g_otp_data_headers[i].data_name[0], data_item_name, NVDM_OTP_DATA_NAME_LENGTH - 1) == 0) {
            return i;
        }
    }

    return -1;
}

static int32_t nvdm_modem_check_data_content(uint8_t *buffer, otp_data_header_t *pheader)
{
    uint32_t offset;
    int i, j, fragment;

    offset = pheader->offset + NVDM_OTP_HEADER_LENGTH;
    fragment = pheader->value_size / NVDM_OTP_BUFFER_SIZE;
    for (i = 0; i < fragment; i++) {
        nvdm_modem_port_otp_read(offset, g_working_buffer, NVDM_OTP_BUFFER_SIZE);
        for (j=0; j < NVDM_OTP_BUFFER_SIZE; j++) {
            if (g_working_buffer[j] != *(uint8_t *)(buffer + i*NVDM_OTP_BUFFER_SIZE + j))
                return -1;
        }
        offset += NVDM_OTP_BUFFER_SIZE;
    }
    if (pheader->value_size % NVDM_OTP_BUFFER_SIZE) {
        nvdm_modem_port_otp_read(offset, g_working_buffer, pheader->value_size % NVDM_OTP_BUFFER_SIZE);
        for (j=0; j < (pheader->value_size % NVDM_OTP_BUFFER_SIZE); j++) {
            if (g_working_buffer[j] != *(uint8_t *)(buffer + fragment*NVDM_OTP_BUFFER_SIZE + j))
                return -1;
        }
    }
    return 0;
}

void nvdm_modem_otp_init(void)
{
    int32_t otp_offset, otp_dirty, otp_valid;
    uint16_t i, otp_data_length;
    otp_data_header_t curr_header;
    data_item_status_t status;

    otp_offset = 0;
    otp_dirty = 0;
    otp_valid = 0;
    nvdm_modem_port_get_otp_length(&g_otp_length);
    memset(g_otp_data_headers, 0, NVDM_MAX_OTP_DATA_ITEMS * NVDM_OTP_HEADER_LENGTH);

    /*read all data items*/
    while(otp_offset < g_otp_length - NVDM_OTP_HEADER_LENGTH) {
        nvdm_modem_port_otp_read(otp_offset, (uint8_t *)&curr_header, NVDM_OTP_HEADER_LENGTH);
        otp_data_length = NVDM_OTP_HEADER_LENGTH + curr_header.value_size + DATA_ITEM_CHECKSUM_SIZE;
        switch (curr_header.status) {
            case DATA_ITEM_STATUS_EMPTY:
                g_otp_free += (g_otp_length - otp_dirty - otp_valid);
                return;
            case DATA_ITEM_STATUS_WRITING:
                /* we can't belive data item header if we found it's writting,
                              * so just mark rest of space is dirty.
                            */
                g_otp_free = 0;
                return;
            case DATA_ITEM_STATUS_VALID:
                otp_offset += NVDM_OTP_HEADER_LENGTH + curr_header.value_size + DATA_ITEM_CHECKSUM_SIZE;
                otp_valid += otp_data_length;
                break;
            case DATA_ITEM_STATUS_DELETE:
                otp_dirty += otp_data_length;
                otp_offset += otp_data_length;
                continue;
            default:
                nvdm_modem_port_log_notice("[OTP]offset=0x%x", otp_offset);
                return;
        }

        /* update data items */
        memcpy(&g_otp_data_headers[curr_header.index], &curr_header, NVDM_OTP_HEADER_LENGTH);
        g_otp_sum_data_headers++;
        if (g_otp_sum_data_headers > NVDM_MAX_OTP_DATA_ITEMS) {
            nvdm_port_log_error("too many data items in otp region\n");
            return;
        }
    }
    /* If there is dark space exist, it should also be considered as free space. */
    if (otp_offset >= (g_otp_length - NVDM_OTP_HEADER_LENGTH)) {
        g_otp_free += (g_otp_length - otp_dirty - otp_valid);
    }
    return;
}

nvdm_modem_status_t nvdm_modem_write_otp_data(const char *data_item_name,
        const uint8_t *buffer,
        uint32_t size)
{
    int32_t index, append, added_size, offset, old_offset;
    uint16_t checksum;
    data_item_status_t status;
    otp_data_header_t *pheader;

    nvdm_modem_port_log_info("nvdm_write_otp_data_item: begin to write");

    if ((data_item_name == NULL) ||
            (buffer == NULL) ||
            (size == 0)) {
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    if (strlen(data_item_name) > (NVDM_OTP_DATA_NAME_LENGTH-1) ) {
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    if (g_nvdm_modem_init_status == false) {
        return NVDM_MODEM_STATUS_ERROR;
    }

    //data_item_name_size = strlen(data_item_name) + 1;

    index = search_data_item_by_name(data_item_name);
    nvdm_modem_port_log_info("find_data_item_by_hashname return %d", index);
    if (index < 0) {
        append = 1;
        /* find a empty position to fill in */
        index = find_empty_data_item();
        if (index < 0 ) {
            nvdm_port_log_error("too many data items in nvdm region\n");
            return NVDM_MODEM_STATUS_ERROR;
        }
    } else {
        append = 0;
    }

    if ((size + NVDM_OTP_HEADER_LENGTH + DATA_ITEM_CHECKSUM_SIZE) > g_otp_free) {
        nvdm_modem_port_log_notice("peb free space is not enough\n");
        return NVDM_MODEM_STATUS_INSUFFICIENT_SPACE;
    }

    offset = g_otp_length - g_otp_free;
    pheader = &g_otp_data_headers[index];

    if (append) {
        nvdm_modem_port_log_info("new data item append");
        pheader->value_size = size;
        pheader->index = index;
        pheader->offset = offset;
        strncpy(&pheader->data_name[0], data_item_name, strlen(data_item_name));
        g_otp_sum_data_headers++;
    } else {
        nvdm_modem_port_log_info("old data item overwrite");

        if (size == pheader->value_size) {
            /* If no change, return without update*/
            if (nvdm_modem_check_data_content(buffer, pheader) == 0)
                return NVDM_MODEM_STATUS_OK;
        }
        old_offset = pheader->offset;
        pheader->offset = offset;
        pheader->value_size = size;
    }

    if (g_otp_sum_data_headers > NVDM_MAX_OTP_DATA_ITEMS) {
        nvdm_port_log_error("too many data items in nvdm region\n");
        return NVDM_MODEM_STATUS_ERROR;
    }

    nvdm_modem_port_log_info("data_item_name = %s, size = %d", data_item_name, size);

    /* calculate checksum for new data item copy */
    checksum = 0;
    /* NVDM_OTP_HEADER_LENGTH-1 must be power of 2 */
    checksum = calculate_checksum(checksum, &pheader->index, NVDM_OTP_HEADER_LENGTH - 1);
    checksum = calculate_checksum(checksum, buffer, size);

    /* set status of data item to writing */
    status = DATA_ITEM_STATUS_WRITING;
    nvdm_modem_port_otp_write(offset, (uint8_t *)&status, 1);
    /* write header of data item (not including status) */
    offset += 1;
    nvdm_modem_port_otp_write(offset, &pheader->index, NVDM_OTP_HEADER_LENGTH - 1); 
    /* write value of data item */
    offset += NVDM_OTP_HEADER_LENGTH - 1;
    nvdm_modem_port_otp_write(offset, (uint8_t *)buffer, size);
    /* write checksum of data item */
    offset += size;
    nvdm_modem_port_otp_write(offset, (uint8_t *)&checksum, DATA_ITEM_CHECKSUM_SIZE);
    /* set status of data item to valid */
    offset -= pheader->value_size + NVDM_OTP_HEADER_LENGTH;
    status = DATA_ITEM_STATUS_VALID;
    nvdm_modem_port_otp_write(offset, (uint8_t *)&status, 1);

    g_otp_free -= (size + NVDM_OTP_HEADER_LENGTH + DATA_ITEM_CHECKSUM_SIZE);

    if (!append) {
        /* because we have write new copy successfully,
              * so we can invalidate old copy now!
              */
        status = DATA_ITEM_STATUS_DELETE;
        nvdm_modem_port_otp_write(old_offset, (uint8_t *)&status, 1);
    }

    nvdm_modem_port_log_info("nvdm_write_otp_data_item: write end");

    return NVDM_MODEM_STATUS_OK;
}

nvdm_modem_status_t nvdm_modem_read_otp_data(const char *data_item_name,
        uint8_t *buffer,
        uint32_t *size)
{
    int32_t index;
    uint32_t offset;
    uint16_t checksum1, checksum2;

    nvdm_modem_port_log_info("nvdm_modem_read_otp_data: begin to read");

    if ((data_item_name == NULL) ||
            (buffer == NULL) ||
            (size == NULL) ||
            (*size == 0)) {
        if (size != NULL) {
            *size = 0;
        }
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    if (strlen(data_item_name) > NVDM_OTP_DATA_NAME_LENGTH-1) {
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    if (g_nvdm_modem_init_status == false) {
        return NVDM_MODEM_STATUS_ERROR;
    }

    index = search_data_item_by_name(data_item_name);
    if (index < 0) {
        *size = 0;
        return NVDM_MODEM_STATUS_ITEM_NOT_FOUND;
    }

    /* check whether buffer size is enough */
    if (*size < g_otp_data_headers[index].value_size) {
        *size = 0;
        return NVDM_MODEM_STATUS_INVALID_PARAMETER;
    }

    /* verify checksum of date item */
    checksum1 = calculate_data_item_checksum(&g_otp_data_headers[index]);
    offset = g_otp_data_headers[index].offset + NVDM_OTP_HEADER_LENGTH + g_otp_data_headers[index].value_size;
    nvdm_modem_port_otp_read(offset, (uint8_t *)&checksum2, DATA_ITEM_CHECKSUM_SIZE);
    if (checksum1 != checksum2) {
        *size = 0;
        return NVDM_MODEM_STATUS_INCORRECT_CHECKSUM;
    }

    /* checksum is ok, so read it to user buffer */
    offset = g_otp_data_headers[index].offset + NVDM_OTP_HEADER_LENGTH;
    nvdm_modem_port_otp_read(offset, buffer, g_otp_data_headers[index].value_size);
    buffer += g_otp_data_headers[index].value_size;
    memset(buffer, 0, *size - g_otp_data_headers[index].value_size);

    *size = g_otp_data_headers[index].value_size;

    nvdm_modem_port_log_info("data_item_name = %s, size = %d", data_item_name, g_otp_data_headers[index].value_size);

    return NVDM_MODEM_STATUS_OK;
}

nvdm_modem_status_t nvdm_modem_lock_otp(void)
{
    nvdm_modem_port_otp_lock();
    return NVDM_MODEM_STATUS_OK;
}


#endif /* MTK_NVDM_MODEM_ENABLE */
