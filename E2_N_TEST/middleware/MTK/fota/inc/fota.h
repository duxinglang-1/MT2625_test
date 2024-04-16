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

#ifndef __FOTA_API_H__
#define __FOTA_API_H__

#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *@addtogroup FOTA
 *@{
 * This section introduces the FOTA interface APIs including terms and acronyms, supported features, software architecture, details on how to use this interface, FOTA function groups, enums, structures and functions.
 * This interface writes into a pre-defined register flag. The bootloader enters the update process based on the value written in the flag. The return error code indicates if the flag is written successfully or not. 
 * After the update is complete, the UA automatically resets the register flag.
 * Note, it is recommended to call the reboot API after calling the FOTA interface, because fota_trigger_update() does not contain reboot functionality.
 * There is no dedicated FOTA read or write SDK API, it is suggested to use HAL flash SDK API to write data to specified address of flash.
 * Regarding to the FOTA update flow & usage, please refer to the Firmware Update Developers Guide located under [sdk_root]/doc folder.
 *
 *
 * @section fota_terms Terms and acronyms
 *
 * |Terms                   |Details                                                                 |
 * |------------------------------|------------------------------------------------------------------------|
 * |\b FOTA                       | Firmware Over-The-Air (FOTA) is a Mobile Software Management technology in which the operating firmware of a mobile device is wirelessly upgraded and updated by its manufacturer. Please refer to <a href="https://www.techopedia.com/definition/24236/firmware-over-the-air-fota"> FOTA in Techopedia.</a>|
 * |\b UA                         | Update Agent performs the actual FOTA update on devices. It operates as stand-alone application in bootloader and replaces the old firmware with the new one.|
 *
 *@}
 */

typedef enum {
    FOTA_OK                      = 0,
    FOTA_COMMON_ERROR            = 1,
    FOTA_BUSY                    = 2,
    FOTA_ERROR_LOADING_HEADER    = 3,
    FOTA_ERROR_HEADER_FORMAT     = 4,
    FOTA_ERROR_LOADING_BODY      = 5,
    FOTA_ERROR_WRITE_TO_PARTITON = 6,
    FOTA_ERROR_LOADING_CHECKSUM  = 7,
    FOTA_ERROR_CHECKSUM_VERIFY   = 8,
    FOTA_ERROR_LOADING_MARKER    = 9,
    FOTA_ERROR_ERASE_MARKER      = 10,
    FOTA_ERROR_SEC_CHECK_ERROR   = 11,
    FOTA_ERROR_DOWNLOAD_FAILED   = 12,
    FOTA_ERROR_NW_DEACTIVE       = 13,
    FOTA_ERROR_OPERATION_INVALID = 14,
    FOTA_ERROR_PARAMETER_INVALID = 15,
    FOTA_ERROR_WRONG_PACKAGE     = 16,
    FOTA_ERROR_SW_VERSION_INVALID = 17,
    FOTA_ERROR_BROM_ERROR         = 18,
    FOTA_ERROR_DA_ERROR           = 19,
    FOTA_ERROR_NW_ACTIVEING       = 20
}fota_result_t;

typedef enum {
    FOTA_MAIN_BIN   = 1,
    FOTA_GNSS_BIN   = 2,
    FOTA_ALL_BIN    = 3,
    FOTA_INVLID_BIN = 0xFF
}fota_image_t;

typedef enum {
    FOTA_MSG_DOWNLOAD_PROGRESS_IND,
    FOTA_MSG_DOWNLOAD_RESULT_IND,
    FOTA_MSG_UPGRADE_PROGRESS_IND,
    FOTA_MSG_UPGRADE_RESULT_IND
}fota_msg_event_t;

typedef union {
    uint32_t progress;
    fota_result_t fota_result;
}fota_msg_event_info_t;

/*The valid value should be larger than zero.*/
typedef uint32_t fota_event_handle_t;

typedef void (*fota_event_indication_t)(fota_msg_event_t event, fota_msg_event_info_t* info);

void fota_init(void);

fota_event_handle_t fota_register_event(fota_event_indication_t call_back);

void fota_deregister_event(fota_event_handle_t handle);

fota_result_t fota_download_image(fota_image_t image, uint8_t* image_uri);

fota_result_t fota_trigger_upgrade(fota_image_t image);

fota_result_t fota_read_upgrade_result(void);

fota_result_t fota_trigger_reboot(void);


#ifdef __cplusplus
}
#endif
#endif /* __FOTA_API_H__ */

