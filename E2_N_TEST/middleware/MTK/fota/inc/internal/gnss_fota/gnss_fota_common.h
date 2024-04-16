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

#ifndef __GNSS_FOTA_COMMON__H__
#define __GNSS_FOTA_COMMON__H__

#include "stdint.h"
#include "stdbool.h"
#include "FreeRTOS.h"

/* There are two main stages in GNSS FOTA process.*/
typedef enum {
    GNSS_BROM_STATE, /* GNSS FOTA BROM communcation state. */
    GNSS_DA_STATE    /* GNSS FOTA DA communcation state. */
}gnss_fota_state_t;

/**************************************************************************************************
 * @brief     callback for informing current state's progress.
 * @param[in] fota_state - refer to gnss_fota_state_t
 * @param[in] percent    - The progress percent for current fota state.
 * @return    None.
 **************************************************************************************************/
typedef void (*gnss_fota_update_progress_t)(gnss_fota_state_t fota_state, uint32_t percent);
/**************************************************************************************************
 * @brief     callback function for init bin handler's internal context.
 * @param[in] None
 * @return    None.
 **************************************************************************************************/
typedef bool (*gnss_fota_bin_init_t)(void);
/**************************************************************************************************
 * @brief     callback function for get binary's total length.
 * @param[in] None
 * @return    return the binary's total length.
 **************************************************************************************************/
typedef uint32_t (*gnss_fota_get_bin_length_t)(void);
/**************************************************************************************************
 * @brief     callback function for copying binary's data into p_buffer.
 *            There are contexts to store how much data has been readed.
 * @param[in] p_buffer - the user must provide the buffer to contain the data.
 * @param[in] length   - excepted data length .
 * @return    return the actual data length putted into buffer.
 **************************************************************************************************/
typedef uint32_t (*gnss_fota_get_data_t)(uint8_t* p_buffer, uint32_t length);

/*Abstracted GNSS binary handler to get GNSS binary from NOR FLASH or RAM or file system*/
typedef struct {
    gnss_fota_bin_init_t       gnss_fota_bin_init;
    gnss_fota_get_bin_length_t gnss_fota_get_bin_length;
    gnss_fota_get_data_t       gnss_fota_get_data;
}gnss_bin_handle_t;
#endif
