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

#ifndef __GNSS_FOTA_DA__H__
#define __GNSS_FOTA_DA__H__

#include "gnss_fota_common.h"

typedef enum {
    GNSS_DA_OK = 0,
    GNSS_DA_WRONG_PARAMETER  = -1,
    GNSS_DA_READ_DATA_FAILED = -2,
    GNSS_DA_SEND_DATA_FAILED = -3,
    GNSS_DA_SAVE_UNCHANGE_DATA_FAILED = -4,
    GNSS_DA_ERASE_FLASH_FAILED = -5,
    GNSS_DA_READ_CONT_CHAR_FAILED = -6,
    GNSS_DA_RETRANSMISSION_FAILED = -7,
    GNSS_DA_RECOVER_DATA_FAILED = -8,
    GNSS_DA_SET_MEMORY_BLOCK_FAILED = -9,
    GNSS_DA_CHECKSUM_ERROR = -10,
    GNSS_DA_NO_RESPONSE = -11,
    GNSS_DA_SOC_CHECK_FAIL = -12,
    GNSS_DA_HW_ERROR = -13,
    GNSS_DA_SYNC_INCORRECT = -14,
    GNSS_DA_VERSION_INCORRECT = -15,
    GNSS_DA_UNKNOWN_FLASH_DEVICE = -16,
    GNSS_DA_OUT_OF_MEMORY        = -17
} gnss_da_result_t;

typedef struct {
    gnss_bin_handle_t           gnss_bin_handle;  /* The handle is used to get GNSS binary. */
    gnss_fota_update_progress_t gnss_da_progress; /* update GNSS BIN progress.*/
} gnss_fota_da_parameter_t;
/**************************************************************************************************
 * @brief     Commucation with GNSS DA to transfer the GNSS binary to GNSS chip.
 *            And then GNSS DA will write the binary to GNSS flash's right partition.
 * @param[in] p_parameter - refer to gnss_fota_da_parameter_t
 * @return    Return the GNSS FOTA final result. Refer to gnss_da_result_t.
 **************************************************************************************************/
gnss_da_result_t gnss_fota_da_processor(gnss_fota_da_parameter_t* p_parameter);

#endif/*__GNSS_FOTA_DA__H__*/
