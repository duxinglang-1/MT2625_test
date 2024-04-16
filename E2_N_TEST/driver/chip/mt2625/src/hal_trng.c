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

#include "hal_trng.h"

#ifdef HAL_TRNG_MODULE_ENABLED

#include "hal_trng_internal.h"
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_internal.h"
#include "hal_nvic.h"
#include "hal_nvic_internal.h"

#ifdef HAL_SLEEP_MANAGER_ENABLED
static sleep_management_lock_request_t trng_sleep_handle = SLEEP_LOCK_TRNG;
static hal_sleep_lock_t trng_sleeplock_level = HAL_SLEEP_LOCK_ALL;
#endif

#define TRNG_INIT  1

#define TRNG_DEINIT  0

volatile static uint32_t trng_init_status = TRNG_DEINIT;

static hal_trng_status_t trng_check_and_set_busy(void) {
    uint32_t saved_mask;
    uint32_t busy_status;
    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
    if(trng_init_status == TRNG_INIT){
        busy_status = HAL_TRNG_STATUS_ERROR;
    }
    else {
        trng_init_status = TRNG_INIT;
        busy_status = HAL_TRNG_STATUS_OK;
    }
    hal_nvic_restore_interrupt_mask(saved_mask);
    return busy_status;
}

static hal_trng_status_t trng_set_idle(void) {
    uint32_t saved_mask;
    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
    trng_init_status = TRNG_DEINIT;
    hal_nvic_restore_interrupt_mask(saved_mask);
    return HAL_TRNG_STATUS_OK;
}

hal_trng_status_t hal_trng_init(void)
{
    hal_trng_status_t busy_status;
    busy_status = trng_check_and_set_busy();
    if (HAL_TRNG_STATUS_ERROR == busy_status) {
        return HAL_TRNG_STATUS_ERROR;
    }
    trng_init();
    return HAL_TRNG_STATUS_OK;
}

hal_trng_status_t hal_trng_deinit(void)
{
    trng_deinit();
    trng_set_idle();
    return HAL_TRNG_STATUS_OK;
}



hal_trng_status_t hal_trng_get_generated_random_number(uint32_t *random_number)
{
    uint32_t generate_data = 0;
    uint32_t saved_mask;
#ifdef HAL_SLEEP_MANAGER_ENABLED
    hal_sleep_manager_acquire_sleeplock(trng_sleep_handle,trng_sleeplock_level);
#endif

    trng_config_timeout_limit(0xFFF);
    trng_enable_mode(true, true, true);

    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
    trng_start();
    generate_data = trng_get_random_data();
    trng_stop();
    hal_nvic_restore_interrupt_mask(saved_mask);

    if (generate_data == 0) {
        *random_number = 0;
#ifdef HAL_SLEEP_MANAGER_ENABLED
        hal_sleep_manager_release_sleeplock(trng_sleep_handle,trng_sleeplock_level);
#endif
        return  HAL_TRNG_STATUS_ERROR;
    } else {
        *random_number = generate_data;
    }
#ifdef HAL_SLEEP_MANAGER_ENABLED
    hal_sleep_manager_release_sleeplock(trng_sleep_handle,trng_sleeplock_level);
#endif
    return HAL_TRNG_STATUS_OK;
}

#endif /*HAL_TRNG_MODULE_ENABLED*/


