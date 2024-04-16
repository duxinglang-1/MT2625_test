/* Copyright Statement:
 *
 * (C) 2005-2017  MediaTek Inc. All rights reserved.
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

#include "fota_protocol.h"
#include "hal_flash.h"
#include "memory_map.h"
#include "syslog.h"
#include "FreeRTOS.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <hal_sha.h>
#include "fota_version_controller.h"
#include "fota.h"

/*********************************************************************************************
 *                               Local Macro Definition                                      *
 *********************************************************************************************/
#define FOTA_RESERVED_PARTITION_START_ADDRESS    FOTA_RESERVED_BASE
#define FOTA_RESERVED_PARTITION_END_ADDRESS     (FOTA_RESERVED_BASE + FOTA_RESERVED_LENGTH - FLASH_BASE)
#define FOTA_HEADER_MAGIC_PATTERN                0x004D4D4D
#define FOTA_TRIGGER_FLAG_ADDRESS               (FOTA_RESERVED_PARTITION_END_ADDRESS - 512)
#define FOTA_UPDATE_INFO_RESERVE_SIZE           (512)      /*512byts */
#define FLASH_BLOCK_SIZE                        (1 << 12)
#define FOTA_UPDATE_BACKUP_INFO_START_ADDRESS   (FOTA_TRIGGER_FLAG_ADDRESS + sizeof(fota_update_info_t))
#define FOTA_HEADER_GET_MAGIC(magic_ver)        ((magic_ver)&0x00FFFFFF)
#define FOTA_HEADER_GET_VER(magic_ver)          ((magic_ver)>>24)

#define FOTA_SIGNATURE_SIZE                     (20)
#define FOTA_UPDATE_INFO_RESERVE_SIZE           (512)
#define FOTA_UPDATING_MARKER                    (0x544e5546)
#define FOTA_BIN_NUMBER_MAX                     4

#define FOTA_MAIN_BIN_TYPE_VALUE                1
#define FOTA_MAIN_BIN_SEC_HEADER_TYPE_VALUE     2
#define FOTA_GNSS_BIN_TYPE_VALUE                3

#define FOTA_ERR(fmt,arg...)   LOG_E(fota, "[FOTA]: "fmt,##arg)
#define FOTA_WARN(fmt,arg...)  LOG_W(fota, "[FOTA]: "fmt,##arg)
#define FOTA_DBG(fmt,arg...)   LOG_I(fota,"[FOTA]: "fmt,##arg)

/******************************************************************************************
 *                               Type Definitions                                         *
 *****************************************************************************************/
/* this structure contains 1. update information for DM, 2. marker for power loss recovery */
typedef struct {
    /* version of structure */
    uint32_t m_ver;
    /* update information for DM*/
    uint32_t m_error_code;
    /* the behavior of bootloader for this error */
    uint32_t m_behavior;    /* 0 : reboot, 1 : hang */
    /* check if DM has read */
    uint32_t m_is_read;     /* 0 : read, 1 : not read */
    /* marker for power loss recovery , 32 is FOTA_MARKER_REGION_SIZE*/
    char  m_marker[32];
    /* reserved & make the structure 64 bytes */
    uint32_t reserved[4];
} fota_update_info_t;

typedef struct {
    uint32_t m_bin_offset;
    uint32_t m_bin_start_addr;
    uint32_t m_bin_length;
    uint32_t m_partition_length;
    uint32_t m_sig_offset;
    uint32_t m_sig_length;
    uint32_t m_is_compressed;
    uint32_t m_version_info[16];
    uint32_t m_bin_type;
    uint8_t m_bin_reserved[4];
} fota_bin_info_t;

typedef struct {
    uint32_t m_magic_ver;
    uint32_t m_bin_num;
    fota_bin_info_t m_bin_info[FOTA_BIN_NUMBER_MAX];
} fota_header_info_t;

/*********************************************************************************************
 *                               Static Variants Definition                                  *
 *********************************************************************************************/

static fota_header_info_t g_fota_header;
/*********************************************************************************************
 *                               Local Function's Definition                                 *
 *********************************************************************************************/
static bool fota_parse_header(uint32_t package_address);
static bool fota_integrity_check(uint32_t bin_number);
/******************************************************************************************
 *                               Public Function's Implementations                        *
 ******************************************************************************************/
fota_result_t fota_trigger_main_bin_update(void)
{
    hal_flash_status_t ret;
    fota_result_t fota_result = FOTA_COMMON_ERROR;
    uint32_t pattern = FOTA_HEADER_MAGIC_PATTERN;
    bool status = false;
    uint32_t index = 0;
    status = fota_parse_header(FOTA_RESERVED_PARTITION_START_ADDRESS);
    FOTA_DBG("FOTA parse header:%d\r\n", status);
    if (status != true){
        return FOTA_ERROR_WRONG_PACKAGE;
    }else{
        for(index = 0; index < g_fota_header.m_bin_num; index ++){
            if (g_fota_header.m_bin_info[index].m_bin_type == FOTA_MAIN_BIN_TYPE_VALUE){
                if (fota_version_controller_check(g_fota_header.m_bin_info[index].m_version_info[1]) == false){
                    return FOTA_ERROR_SW_VERSION_INVALID;
                }else{
                    status = true;
                }
                break;
            }
        }
    }
    if (false == status){
        return FOTA_ERROR_WRONG_PACKAGE;
    }
    ret = hal_flash_init();
    if (ret < HAL_FLASH_STATUS_OK){
        FOTA_ERR("ERR code %d", ret);
        return FOTA_COMMON_ERROR;
    }

    /* erase the last 4k block in fota reserved partition */
    ret = hal_flash_erase(FOTA_RESERVED_PARTITION_END_ADDRESS - 4096, HAL_FLASH_BLOCK_4K);
    if (ret < HAL_FLASH_STATUS_OK){
        FOTA_ERR("ERR code %d", ret);
        return FOTA_COMMON_ERROR;
    }

    /* write pattern into 512 bytes ahead of the reserved partition end address */
    FOTA_DBG("flag address is 0x%x", FOTA_TRIGGER_FLAG_ADDRESS);
    ret = hal_flash_write(FOTA_TRIGGER_FLAG_ADDRESS, (const uint8_t*)&pattern, sizeof(uint32_t));
    if (ret < HAL_FLASH_STATUS_OK){
        FOTA_ERR("ERR code %d", ret);
        return FOTA_COMMON_ERROR;
    }
    return FOTA_OK;
}

void fota_clear_history(void)
{
    hal_flash_status_t flash_op_result;
    flash_op_result = hal_flash_erase(FOTA_RESERVED_BASE + FOTA_RESERVED_LENGTH - FLASH_BLOCK_SIZE - FLASH_BASE, HAL_FLASH_BLOCK_4K);
    if (flash_op_result != HAL_FLASH_STATUS_OK) {
        FOTA_ERR("ERR code %d", flash_op_result);
    }
}

bool fota_is_executed(void)
{
    fota_update_info_t *update_info_ptr;
    uint8_t update_info[sizeof(fota_update_info_t)];
    bool updated = false;
    hal_flash_status_t flash_op_result;
    uint32_t update_result;
    bool result = false;

    flash_op_result = hal_flash_read(FOTA_RESERVED_BASE + FOTA_RESERVED_LENGTH - FOTA_UPDATE_INFO_RESERVE_SIZE - FLASH_BASE,
                                     update_info, sizeof(fota_update_info_t));
    if (flash_op_result != HAL_FLASH_STATUS_OK) {
        FOTA_ERR("ERR code %d", flash_op_result);
        return result;
    }
    for (int i = 0; i < sizeof(fota_update_info_t); i++) {
        if (update_info[i] != 0xFF) {
            updated = true;
            break;
        }
    }

    if (updated == true) {
        result = true;
    }else{
        FOTA_ERR("no update info");
        result = false;
    }
    FOTA_DBG("fota_is_executed:%d\r\n", result);
    return result;
}


bool fota_read_fota_result(fota_bl_status_t* p_result_code)
{
    fota_update_info_t *update_info_ptr;
    uint8_t update_info[sizeof(fota_update_info_t)];
    bool updated = false;
    hal_flash_status_t flash_op_result;
    uint32_t update_result;
    bool result = false;
    configASSERT(p_result_code != NULL);

    flash_op_result = hal_flash_read(FOTA_RESERVED_BASE + FOTA_RESERVED_LENGTH - FOTA_UPDATE_INFO_RESERVE_SIZE - FLASH_BASE,
                                     update_info, sizeof(fota_update_info_t));
    if (flash_op_result != HAL_FLASH_STATUS_OK) {
        FOTA_ERR("ERR code %d", flash_op_result);
        return result;
    }
    update_info_ptr = (fota_update_info_t *)update_info;
    update_result = update_info_ptr->m_error_code;
    *p_result_code = (fota_bl_status_t)update_result;
    return true;
}

bool fota_get_gnss_binary_info(uint32_t* start_address, uint32_t* length)
{
    bool result = false;
    uint32_t index = 0;
    if (fota_parse_header(FOTA_RESERVED_PARTITION_START_ADDRESS) == true){
        for (index = 0; index < FOTA_BIN_NUMBER_MAX; index++){
            if (g_fota_header.m_bin_info[index].m_bin_type == FOTA_GNSS_BIN_TYPE_VALUE){
                if (fota_integrity_check(index) == true){
                    *start_address = FOTA_RESERVED_PARTITION_START_ADDRESS + g_fota_header.m_bin_info[index].m_bin_offset;
                    *length = g_fota_header.m_bin_info[index].m_bin_length;
                    result = true;
                }
                break;
            }
        }
    }
    return result;
}

fota_image_t fota_get_current_available_package()
{
    fota_image_t image_type = FOTA_INVLID_BIN;
    uint32_t index = 0;
    if (fota_parse_header(FOTA_RESERVED_PARTITION_START_ADDRESS) == true){
        for (index = 0; index < g_fota_header.m_bin_num; index++){
            switch(g_fota_header.m_bin_info[index].m_bin_type){
                case FOTA_MAIN_BIN_TYPE_VALUE: {
                    image_type = FOTA_MAIN_BIN;
                    break;
                }
                case FOTA_GNSS_BIN_TYPE_VALUE: {
                    image_type = FOTA_GNSS_BIN;
                    break;
                }
                default:{
                    break;
                }
            }
        }
    }
    return image_type;
}

/******************************************************************************************
 *                               Local Function's Implementations                         *
 ******************************************************************************************/
static bool fota_parse_header(uint32_t package_address)
{
    bool  result = true;
    hal_flash_status_t flash_status;
    uint32_t start_address = 0;
    void *fota_head_buf;
    int i;
    hal_sha_status_t sha_status;
    hal_sha1_context_t sha1_context;
    uint8_t header_sha1_checksum[64] = {0};
    uint8_t header_checksum[FOTA_SIGNATURE_SIZE];

    if (package_address > FLASH_BASE){
        start_address = package_address - FLASH_BASE;
    }else{
        start_address = package_address;
    }
    fota_head_buf = &g_fota_header;
    flash_status = hal_flash_read(start_address, (uint8_t *)fota_head_buf, sizeof(fota_header_info_t));
    if (flash_status == HAL_FLASH_STATUS_OK) {
        /* calculate header checksum */
        sha_status = hal_sha1_init(&sha1_context);
        if ( sha_status != HAL_SHA_STATUS_OK) {
            result = false;
            return result;
        }
        sha_status = hal_sha1_append(&sha1_context, fota_head_buf, sizeof(fota_header_info_t));
        if (sha_status != HAL_SHA_STATUS_OK) {
            result = false;
            return result;
        }
        sha_status = hal_sha1_end(&sha1_context, header_sha1_checksum);
        if ( sha_status != HAL_SHA_STATUS_OK) {
            result = false;
            return result;
        }
        /* read header checksum */
        flash_status = hal_flash_read(start_address + sizeof(fota_header_info_t), header_checksum, FOTA_SIGNATURE_SIZE);
        if ( flash_status != HAL_FLASH_STATUS_OK) {
            result = false;
            return result;
        }

        /* compare checksum */
        for (i = 0; i < HAL_SHA1_DIGEST_SIZE; i++) {
            if (header_sha1_checksum[i] != header_checksum[i]) {
                FOTA_ERR("header integrity check fail\r\n");
                result = false;
                return result;
            }
        }

        /* dump fota header */
        FOTA_DBG("fota_head.m_magic_ver                    = %x \n\r", g_fota_header.m_magic_ver);
        FOTA_DBG("fota_head.m_bin_num                      = %x \n\r", g_fota_header.m_bin_num);
        for (i = 0; i < FOTA_BIN_NUMBER_MAX; i++) {
            FOTA_DBG("g_fota_header.m_bin_info[%d].m_bin_length     = %x \n\r", i, g_fota_header.m_bin_info[i].m_bin_length);
            FOTA_DBG("g_fota_header.m_bin_info[%d].m_bin_offset     = %x \n\r", i, g_fota_header.m_bin_info[i].m_bin_offset);
            FOTA_DBG("g_fota_header.m_bin_info[%d].m_bin_start_addr = %x \n\r", i, g_fota_header.m_bin_info[i].m_bin_start_addr);
            FOTA_DBG("g_fota_header.m_bin_info[%d].m_partition_len  = %x \n\r", i, g_fota_header.m_bin_info[i].m_partition_length);
            FOTA_DBG("g_fota_header.m_bin_info[%d].m_sig_length     = %x \n\r", i, g_fota_header.m_bin_info[i].m_sig_length);
            FOTA_DBG("g_fota_header.m_bin_info[%d].m_sig_offset     = %x \n\r", i, g_fota_header.m_bin_info[i].m_sig_offset);
            FOTA_DBG("g_fota_header.m_bin_info[%d].m_is_compressed  = %x \n\r", i, g_fota_header.m_bin_info[i].m_is_compressed);
            FOTA_DBG("g_fota_header.m_bin_info[%d].m_version_info[0]  = %d \n\r", i, g_fota_header.m_bin_info[i].m_version_info[0]);
            FOTA_DBG("g_fota_header.m_bin_info[%d].m_version_info[1]  = %d \n\r", i, g_fota_header.m_bin_info[i].m_version_info[1]);
            FOTA_DBG("g_fota_header.m_bin_info[%d].m_bin_type  = %d \n\r", i, g_fota_header.m_bin_info[i].m_bin_type);
        }

        if ( g_fota_header.m_bin_num > FOTA_BIN_NUMBER_MAX ) {
            result = false;
            return result;
        }

        if ( FOTA_HEADER_GET_MAGIC(g_fota_header.m_magic_ver) != FOTA_HEADER_MAGIC_PATTERN ) {
            result = false;
            return result;
        }
    } else {
        FOTA_ERR("FOTA check header error");
    }
    return result;
}

static bool fota_integrity_check(uint32_t bin_number)
{
    hal_sha_status_t sha_status;
    bool result = true;
    hal_flash_status_t flash_status;
    uint32_t bin_address = 0;
    uint32_t sig_address = 0;
    uint32_t source;
    uint8_t  *buffer;
    uint32_t length = 1024;
    hal_sha1_context_t sha1_context;
    uint32_t bin_counter;
    uint8_t bin_sha1_checksum[64] = {0};
    int i;
    uint8_t fota_checksum[FOTA_SIGNATURE_SIZE] = {0};

    buffer = pvPortMalloc(length);
    if (buffer == NULL){
        return false;
    }
    /* calculate os bin checksum */
    sha_status = hal_sha1_init(&sha1_context);
    if ( sha_status != HAL_SHA_STATUS_OK) {
        result = false;
        vPortFree(buffer);
        buffer = NULL;
        return result;
    }
    bin_counter = g_fota_header.m_bin_info[bin_number].m_bin_length;
    bin_address = g_fota_header.m_bin_info[bin_number].m_bin_offset + FOTA_RESERVED_PARTITION_START_ADDRESS - FLASH_BASE;

    while (bin_counter != 0) {

        if (bin_counter >= length) {
            flash_status = hal_flash_read(bin_address, buffer, length);
            if (flash_status == HAL_FLASH_STATUS_OK) {
                sha_status = hal_sha1_append(&sha1_context, buffer, length);
            }
            bin_counter -= length;
            bin_address += length;
        } else {
            flash_status = hal_flash_read(bin_address, buffer, bin_counter);
            if (flash_status == HAL_FLASH_STATUS_OK) {
                sha_status = hal_sha1_append(&sha1_context, buffer, bin_counter);
            }
            bin_counter = 0;
        }


        if (flash_status != HAL_FLASH_STATUS_OK) {
            result = false;
            break;
        }

        if (sha_status != HAL_SHA_STATUS_OK) {
            result = false;
            break;
        }

    }
    sha_status = hal_sha1_end(&sha1_context, bin_sha1_checksum);
    if ( result != true) {
        vPortFree(buffer);
        buffer = NULL;
        return result;
    }

    if (sha_status != HAL_SHA_STATUS_OK) {
        vPortFree(buffer);
        buffer = NULL;
        result = false;
        return result;
    }

    /* read checksum from fota bin */
    sig_address = g_fota_header.m_bin_info[bin_number].m_sig_offset + FOTA_RESERVED_PARTITION_START_ADDRESS - FLASH_BASE;

    flash_status = hal_flash_read(sig_address, fota_checksum, g_fota_header.m_bin_info[bin_number].m_sig_length);
    if ( flash_status != HAL_FLASH_STATUS_OK) {
        vPortFree(buffer);
        buffer = NULL;
        result = false;
        return result;
    }

    /* compare checksum */
    for (i = 0; i < HAL_SHA1_DIGEST_SIZE; i++) {
        if (fota_checksum[i] != bin_sha1_checksum[i]) {
            FOTA_ERR("integrity check fail");
            result = false;
        }
    }

    if (result == true) {
        FOTA_DBG("integrity check pass");
    }

    vPortFree(buffer);
    buffer = NULL;
    return result;
}


