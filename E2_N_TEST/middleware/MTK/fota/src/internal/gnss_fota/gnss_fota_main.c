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

#include "gnss_fota_main.h"
#include "gnss_fota_brom.h"
#include "gnss_fota_da.h"
#include "gnss_fota_log.h"
#include "hal_wdt.h"

/***************************************************************************************************
*                             Public Functions Implementation                                      *
***************************************************************************************************/
/***************************************************************************************************
 * @brief     GNSS FOTA main processor.
 * @param[in] p_fota_param - refer to gnss_fota_parameter_t
 * @return    Return the GNSS FOTA final result. Refer to gnss_fota_result_t.
 **************************************************************************************************/
gnss_fota_result_t gnss_fota_main(gnss_fota_parameter_t* p_fota_param)
{
    gnss_brom_result_t brom_result = GNSS_FOTA_BROM_OK;
    gnss_da_result_t da_result = GNSS_DA_OK;
    gnss_fota_brom_arg_t brom_arg;
    gnss_fota_da_parameter_t da_arg;

    if (NULL == p_fota_param){
        return GNSS_FOTA_PARAMETER_ERROR;
    }

    brom_arg.gnss_brom_progress = p_fota_param->gnss_update_progress;
    brom_arg.gnss_da_bin_handler = p_fota_param->gnss_da_data;
    brom_result = gnss_fota_brom_processor(&brom_arg);
    if (brom_result != GNSS_FOTA_BROM_OK){
        gnss_fota_log_error("GNSS BROM ERROR: ERROR CODE = 0x%x .", brom_result);
        return GNSS_FOTA_BROM_ERROR;
    }
    gnss_fota_log_info("Go to GNSS DA Processor!!!");
    da_arg.gnss_bin_handle = p_fota_param->gnss_update_data;
    da_arg.gnss_da_progress = p_fota_param->gnss_update_progress;
    hal_wdt_disable(HAL_WDT_DISABLE_MAGIC);
    da_result = gnss_fota_da_processor(&da_arg);
    hal_wdt_enable(HAL_WDT_ENABLE_MAGIC);
    if (da_result != GNSS_DA_OK){
        gnss_fota_log_error("GNSS DA ERROR: ERROR CODE = %d .", da_result);
        return GNSS_FOTA_DA_ERROR;
    }

    gnss_fota_log_info("GNSS FOTA SUCCESSFULLY");
    return GNSS_FOTA_OK;
}
