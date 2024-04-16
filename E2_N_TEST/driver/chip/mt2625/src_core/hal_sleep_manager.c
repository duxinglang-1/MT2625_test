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

#include "hal_sleep_manager.h"
#ifdef HAL_SLEEP_MANAGER_ENABLED

#include "hal_sleep_manager_internal.h"
#include "hal_sleep_manager_platform.h"
#include "hal_spm.h"
#include "hal_pmu.h"
#include "hal_log.h"
#include "hal_gpt.h"
#include "hal_rtc.h"
#include "hal_rtc_internal.h"
#include "assert.h"
#include "hal_cache.h"
#ifdef MTK_NVDM_MODEM_ENABLE
#include "nvdm.h"
#include "memory_map.h"
#endif

#ifdef HAL_RTC_FEATURE_SW_TIMER
ATTR_RWDATA_IN_RETSRAM uint32_t sleep_rtc_handle;    // please do not set init value
#endif

extern uint32_t rtc_get_banks_in_active(void);
extern void hal_module_sleep_register_callback(void);

void Deep_Sleep_GPT_CB()
{
    hal_gpt_stop_timer(DEEP_SLEEP_GPT);
}

uint8_t hal_sleep_manager_set_sleep_handle(const char *handle_name)
{
    uint8_t index;
    index = sleep_management_get_lock_handle(handle_name);
    return index;
}

hal_sleep_manager_status_t hal_sleep_manager_acquire_sleeplock(uint8_t handle_index, hal_sleep_lock_t level)
{
    if (sleep_management_sleeplock_control(handle_index, level, LOCK_SLEEP) == 0) {
        return HAL_SLEEP_MANAGER_OK;
    } else {
        return HAL_SLEEP_MANAGER_ERROR;
    }
}

hal_sleep_manager_status_t hal_sleep_manager_release_sleeplock(uint8_t handle_index, hal_sleep_lock_t level)
{
    if (sleep_management_sleeplock_control(handle_index, level, UNLOCK_SLEEP) == 0) {
        return HAL_SLEEP_MANAGER_OK;
    } else {
        return HAL_SLEEP_MANAGER_ERROR;
    }
}

hal_sleep_manager_status_t hal_sleep_manager_release_sleep_handle(uint8_t handle_index)
{
    sleep_management_release_lock_handle(handle_index);
    return HAL_SLEEP_MANAGER_OK;
}

bool hal_sleep_manager_get_sleep_lock_status(hal_sleep_lock_t level, uint32_t* info_high, uint32_t* info_low)
{
    return sleep_management_get_lock_sleep_info(level, info_high, info_low);
}

uint32_t hal_sleep_manager_sleep_driver_dump_handle_name(void)
{
    sleep_management_get_lock_sleep_handle_list();
    return 0;
}

bool hal_sleep_manager_is_sleep_lock_locked(hal_sleep_lock_t level)
{
    return sleep_management_is_sleep_level_locked(level);
}

bool hal_sleep_manager_is_sleep_handle_alive(uint8_t handle_index)
{
    return sleep_management_check_handle_status(handle_index);
}

#ifdef HAL_SLEEP_MANAGER_SUPPORT_POWER_OFF
void hal_sleep_manager_enter_power_off_mode()
{
    hal_rtc_enter_rtc_mode();
}
#endif

#ifdef HAL_RTC_FEATURE_SW_TIMER
void rtc_wakeup_timer_callback(void *user_data)
{
}
#endif

hal_sleep_manager_status_t hal_sleep_manager_set_sleep_time(uint32_t sleep_time_ms)
{
    hal_gpt_status_t ret_status;
    hal_sleep_mode_t mode = sleep_manager_get_sleep_mode();

#ifdef HAL_RTC_FEATURE_SW_TIMER
    if ((mode == HAL_SLEEP_MODE_DEEP_SLEEP) ||
        (mode == HAL_SLEEP_MODE_DEEPER_SLEEP)) {
        rtc_sw_timer_status_t rtc_status;

        rtc_status = rtc_sw_timer_delete(sleep_rtc_handle);
        rtc_status = rtc_sw_timer_create(&sleep_rtc_handle, sleep_time_ms/100, false, rtc_wakeup_timer_callback);
        if (rtc_status != RTC_SW_TIMER_STATUS_OK) {
            log_hal_error("ERROR : RTC create timer fail(%d)", rtc_status);
            assert(0);
            return HAL_SLEEP_MANAGER_ERROR;
        }
        rtc_status = rtc_sw_timer_start(sleep_rtc_handle);
        if (rtc_status != RTC_SW_TIMER_STATUS_OK) {
            log_hal_error("ERROR : RTC start timer fail(%d)", rtc_status);
            assert(0);
            return HAL_SLEEP_MANAGER_ERROR;
        }
    } else
#endif
    {
        hal_gpt_register_callback(DEEP_SLEEP_GPT, (hal_gpt_callback_t)Deep_Sleep_GPT_CB, NULL);
        if (sleep_time_ms > HAL_GPT_MAXIMUM_MS_TIMER_TIME) {
            sleep_time_ms = HAL_GPT_MAXIMUM_MS_TIMER_TIME;
        }
        hal_gpt_stop_timer(DEEP_SLEEP_GPT);
        ret_status = hal_gpt_start_timer_ms(DEEP_SLEEP_GPT, sleep_time_ms, HAL_GPT_TIMER_TYPE_ONE_SHOT);
        if (ret_status != HAL_GPT_STATUS_OK) {
            log_hal_error("ERROR : Deep Sleep GPT Start Fail(%d)", ret_status);
            assert(0);
            return HAL_SLEEP_MANAGER_ERROR;
        }
    }

    return HAL_SLEEP_MANAGER_OK;
}

void hal_sleep_manager_enter_sleep_mode(hal_sleep_mode_t mode)
{
    uint64_t elapsed_tick;

    if (mode == HAL_SLEEP_MODE_IDLE) {
        __asm volatile("dsb");
        __asm volatile("wfi");
        __asm volatile("isb");
        hal_gpt_stop_timer(DEEP_SLEEP_GPT);
    } else if (mode == HAL_SLEEP_MODE_LIGHT_SLEEP) {
        sleep_management_enter_light_sleep();
        hal_gpt_stop_timer(DEEP_SLEEP_GPT);
    } else if (mode == HAL_SLEEP_MODE_DEEP_SLEEP) {
        //RTC Deep sleep API
        sleep_management_suspend_callback(HAL_SLEEP_MODE_DEEP_SLEEP);
        rtc_get_elapsed_tick_sram(false, &elapsed_tick);
        hal_cache_invalidate_all_cache_lines();
        rtc_set_sys_oper_mode(RTC_SYS_DS_SM);
        hal_rtc_enter_retention_mode();
    } else if (mode == HAL_SLEEP_MODE_DEEPER_SLEEP) {
        sleep_management_suspend_callback(HAL_SLEEP_MODE_DEEPER_SLEEP);
        rtc_get_elapsed_tick_sram(false, &elapsed_tick);
        hal_cache_invalidate_all_cache_lines();
        sleep_managerment_backup_rtcram();
        //RTC Deeper sleep API
        rtc_set_sys_oper_mode(RTC_SYS_DP_SM);
        hal_rtc_enter_rtc_mode();
    }
}

void hal_sleep_manager_register_suspend_callback(hal_sleep_manager_callback_t callback, void *data, uint32_t mode)
{
    sleep_management_register_suspend_callback(SLEEP_BACKUP_RESTORE_USER, callback, data, mode);
}

void hal_sleep_manager_register_resume_callback(hal_sleep_manager_callback_t callback, void *data, uint32_t mode)
{
    if ((mode & HAL_SLEEP_MODE_DEEP_SLEEP) ||
        (mode & HAL_SLEEP_MODE_DEEPER_SLEEP)) {
        printf("register deep or deeper sleep resume callback is meaningless\r\n");
        return;
    }

    sleep_management_register_resume_callback(SLEEP_BACKUP_RESTORE_USER, callback, data, mode);
}

#ifdef HAL_SLEEP_MANAGER_SUPPORT_WAKEUP_SOURCE_CONFIG
hal_sleep_manager_status_t hal_sleep_manager_enable_wakeup_pin(hal_sleep_manager_wakeup_source_t pin)
{
    spm_unmask_wakeup_source(pin);
    return HAL_SLEEP_MANAGER_OK;
}

hal_sleep_manager_status_t hal_sleep_manager_disable_wakeup_pin(hal_sleep_manager_wakeup_source_t pin)
{
    spm_mask_wakeup_source(pin);
    return HAL_SLEEP_MANAGER_OK;
}
#endif

hal_sleep_manager_status_t hal_sleep_manager_init()
{
    log_hal_info("hal_sleep_manager_init start\n");

    sleep_management_low_power_init_setting();

    spm_init(0);
    spm_kick_start();
    spm_control_mtcmos(SPM_MTCMOS_SDIO_SLV, SPM_MTCMOS_PWR_DISABLE);

    if (hal_gpt_init(DEEP_SLEEP_GPT) != HAL_GPT_STATUS_OK) {
        log_hal_info("ERROR : Deep Sleep GPT Init Fail");
        return HAL_SLEEP_MANAGER_ERROR;
    }

    pmu_init();

    hal_module_sleep_register_callback();

    return HAL_SLEEP_MANAGER_OK;
}

#endif /* HAL_SLEEP_MANAGER_ENABLED */
