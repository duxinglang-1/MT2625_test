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

#include "hal_keypad.h"

#ifdef HAL_KEYPAD_MODULE_ENABLED

#include "hal_keypad_internal.h"
#include "hal_log.h"
#ifdef HAL_EINT_MODULE_ENABLED
#include "hal_eint.h"
#include "hal_eint_internal.h"
#endif

keypad_context_t   keypad_context;

#ifdef HAL_KEYPAD_FEATURE_POWERKEY
#include "hal_keypad_powerkey_internal.h"
extern powerkey_state_t  powerkey_state;
powerkey_context_t powerkey_context;
extern powerkey_buffer_t powerkey_buffer;
#endif

hal_keypad_status_t hal_keypad_init(const hal_keypad_config_t *keypad_config)
{

    if (keypad_context.has_initilized == true) {
        return HAL_KEYPAD_STATUS_ERROR;
    }

    memset(&keypad_context, 0, sizeof(keypad_context_t));
    memset(&keypad_buffer,  0,  sizeof(keypad_buffer_t));

    NVIC_DisableIRQ(KP_IRQn);
    //keypad->KP_EN = KEYPAD_FLGA_EN;

    if (keypad_config->debounce != 0) {
        keypad->KP_DEBOUNCE = keypad_config->debounce * KEYPAD_DEBOUNCE_MS_UNIT;
    } else {
        /* default use 16 ms */
        keypad->KP_DEBOUNCE = KEYPAD_DEFAULT_DEBOUNCE;
    }

    keypad->KP_SEL = (uint32_t)(keypad_config->key_map.column_bitmap << KEYPAD_KP_SEL_COL_OFFSET) + \
                     (uint32_t)(keypad_config->key_map.row_bitmap << KEYPAD_KP_SEL_ROW_OFFSET) + \
                     (uint32_t)(keypad_config->mode);

    /* default col and row high pulse stage 6 clock, and a pulse total 12 clock. */
    if (keypad_config->mode == HAL_KEYPAD_MODE_DOUBLE_KEY) {
        keypad->KP_SCAN_TIMING  = KEYPAD_DEFAULT_SCAN_TIMING;
    }

    /* initilize bitmap */
    keypad_saved_bitmap.register_value[0] = KEYPAD_DEFAULT_BIT_MAPPING;
    keypad_saved_bitmap.register_value[1] = KEYPAD_DEFAULT_BIT_MAPPING;

    /* repeat time setting*/
    if (keypad_config->repeat_time == 0) {
        keypad_context.repeat_time = KEYPAD_DEFAUL_REPEAT_TIME;
    } else {
        keypad_context.repeat_time = keypad_config->repeat_time;
    }

    /* longpress time setting*/
    if (keypad_config->longpress_time == 0) {
        keypad_context.longpress_time = KEYPAD_DEFAUL_LONGPRESS_TIME;
    } else {
        keypad_context.longpress_time = keypad_config->longpress_time;
    }

    keypad_eint_wakeup_init();

    keypad_context.has_initilized = true;

    return HAL_KEYPAD_STATUS_OK;

}

hal_keypad_status_t hal_keypad_deinit(void)
{
    if (keypad_context.has_initilized != true) {
        return HAL_KEYPAD_STATUS_ERROR;
    }

    if (keypad_get_buffer_data_size() != 0) {
        return HAL_KEYPAD_STATUS_ERROR;
    }

    if (keypad_context.is_running == true) {
        return HAL_KEYPAD_STATUS_ERROR;
    }

    /* disable keypad module, clear column and row bitmap */
    keypad->KP_SEL = 0;

    keypad_context.has_initilized = false;

    return HAL_KEYPAD_STATUS_OK;

}


hal_keypad_status_t hal_keypad_enable(void)
{
    if (keypad_context.has_initilized != true) {
        return HAL_KEYPAD_STATUS_ERROR;
    }
    keypad->KP_EN = KEYPAD_FLGA_EN;

    return HAL_KEYPAD_STATUS_OK;
}

hal_keypad_status_t hal_keypad_disable(void)
{
    keypad->KP_EN &= ~KEYPAD_FLGA_EN;

    return HAL_KEYPAD_STATUS_OK;
}


hal_keypad_status_t hal_keypad_set_scan_timing(const hal_keypad_scan_timing_t *keypad_scan_timing)
{
    if (keypad_scan_timing == NULL) {
        return HAL_KEYPAD_INVALID_PARAMETER;
    }

    if (keypad_scan_timing->column_scan_div <= keypad_scan_timing->column_high_pulse) {
        log_hal_info("[keypad][err]column_scan_div=%d is not over than column_high_pulse=%d\r\n", \
                     keypad_scan_timing->column_scan_div, \
                     keypad_scan_timing->column_high_pulse);
        return HAL_KEYPAD_INVALID_PARAMETER;
    }

    if (keypad_scan_timing->row_scan_div <= keypad_scan_timing->row_high_pulse) {
        log_hal_info("[keypad][err]row_scan_div=%d is not over than row_high_pulse=%d\r\n", \
                     keypad_scan_timing->row_scan_div, \
                     keypad_scan_timing->row_high_pulse);
        return HAL_KEYPAD_INVALID_PARAMETER;
    }

    keypad->KP_SCAN_TIMING = (uint32_t)(keypad_scan_timing->column_high_pulse << KEYPAD_KP_SCAN_TIMING_COL_HIGH_OFFSET) + \
                             (uint32_t)(keypad_scan_timing->row_high_pulse    << KEYPAD_KP_SCAN_TIMING_ROW_HIGH_OFFSET) + \
                             (uint32_t)(keypad_scan_timing->column_scan_div   << KEYPAD_KP_SCAN_TIMING_COL_DIV_OFFSET) + \
                             (uint32_t)(keypad_scan_timing->row_scan_div      << KEYPAD_KP_SCAN_TIMING_ROW_DIV_OFFSET);

    return  HAL_KEYPAD_STATUS_OK;
}


hal_keypad_status_t hal_keypad_get_scan_timing(hal_keypad_scan_timing_t *keypad_scan_timing)
{
    uint32_t temp_data;

    if (keypad_scan_timing == NULL) {
        return HAL_KEYPAD_INVALID_PARAMETER;
    }

    temp_data = keypad->KP_SCAN_TIMING;

    keypad_scan_timing->column_high_pulse = (temp_data >> KEYPAD_KP_SCAN_TIMING_COL_HIGH_OFFSET) & 0x0f;
    keypad_scan_timing->row_high_pulse    = (temp_data >> KEYPAD_KP_SCAN_TIMING_ROW_HIGH_OFFSET) & 0x0f;
    keypad_scan_timing->column_scan_div   = (temp_data >> KEYPAD_KP_SCAN_TIMING_COL_DIV_OFFSET)  & 0x0f;
    keypad_scan_timing->row_scan_div      = (temp_data >> KEYPAD_KP_SCAN_TIMING_ROW_DIV_OFFSET)  & 0x0f;

    return HAL_KEYPAD_STATUS_OK;
}

hal_keypad_status_t hal_keypad_set_debounce(const uint32_t *keypad_debounce)
{
    if ((*keypad_debounce * KEYPAD_DEBOUNCE_MS_UNIT) > KEYPAD_DEBOUNCE_MAX) {
        return HAL_KEYPAD_INVALID_PARAMETER;
    }

    keypad->KP_DEBOUNCE = *keypad_debounce * KEYPAD_DEBOUNCE_MS_UNIT;

    return HAL_KEYPAD_STATUS_OK;
}

hal_keypad_status_t hal_keypad_get_debounce(uint32_t *keypad_debounce)
{
    *keypad_debounce = (keypad->KP_DEBOUNCE / KEYPAD_DEBOUNCE_MS_UNIT);
    return HAL_KEYPAD_STATUS_OK;
}

hal_keypad_status_t hal_keypad_register_callback(hal_keypad_callback_t callback, void *user_data)
{
    if (keypad_context.has_initilized != true) {
        return HAL_KEYPAD_STATUS_ERROR;
    }

    if (callback == NULL) {
        return HAL_KEYPAD_STATUS_ERROR;
    }

    keypad_context.keypad_callback.callback  = callback;
    keypad_context.keypad_callback.user_data = user_data;

    keypad_nvic_register();

    return HAL_KEYPAD_STATUS_OK;
}


hal_keypad_status_t hal_keypad_get_key(hal_keypad_event_t *keypad_event)
{
    if (keypad_get_buffer_data_size() <= 0) {
        log_hal_info("[keypad][get key]normal key no data in buffer\r\n");
        return HAL_KEYPAD_STATUS_ERROR;
    }

    keypad_pop_one_key_from_buffer(keypad_event);
    log_hal_info("[keypad[get key]normal key state=%d, position=%d\r\n", (int)keypad_event->state, (int)keypad_event->key_data);

    return HAL_KEYPAD_STATUS_OK;
}

/************************************ powerkey **************************/
#ifdef HAL_KEYPAD_FEATURE_POWERKEY

hal_keypad_status_t hal_keypad_powerkey_init(uint32_t powerkey_data)
{
    hal_eint_config_t key_eint_cfg;
    hal_gpt_status_t ret_gpt;

    memset(&key_eint_cfg, 0, sizeof(hal_eint_config_t));
    if (powerkey_context.has_initilized == true) {
        return HAL_KEYPAD_STATUS_ERROR;
    }

    if (powerkey_data != DEVICE_KEY_NONE) {
        memset(&powerkey_context, 0, sizeof(powerkey_context_t));
        memset(&powerkey_state, 0, sizeof(powerkey_state_t));
        memset(&powerkey_buffer, 0, sizeof(powerkey_buffer_t));

        /* powerkey repeat time setting*/
        if (powerkey_context.repeat_time == 0) {
            powerkey_context.repeat_time = POWERKEY_DEFAUL_REPEAT_TIME;
        }

        /* powerkey longpress time setting*/
        if (powerkey_context.longpress_time == 0) {
            powerkey_context.longpress_time = POWERKEY_DEFAUL_LONGPRESS_TIME;
        }

        powerkey_context.registerd_data = powerkey_data;
        powerkey_context.has_initilized = true;
    }
    /*Get sleep handler*/
    powerkey_get_sleep_handle();

    /*Get gpt sw timer*/
    ret_gpt = hal_gpt_sw_get_timer(&powerkey_state.timer_handle);
    if (ret_gpt != HAL_GPT_STATUS_OK) {
        printf("[pwk]get timer handle error, ret = %d, handle = 0x%x\r\n", \
               (unsigned int)ret_gpt, \
               (unsigned int)powerkey_state.timer_handle);
        return HAL_KEYPAD_STATUS_ERROR;
    }

    /*Check If Pressed*/
    if ((pmu_get_register_value(POWERKEY_PMU_KEY_STATUS, 1, 13) == 0x01) && (powerkey_state.current_state == HAL_KEYPAD_KEY_RELEASE)) {
        powerkey_lock_sleep();
        if (powerkey_get_buffer_left_size() > 2) {
            powerkey_state.current_state = HAL_KEYPAD_KEY_PRESS;
            /*start timer*/
            ret_gpt = hal_gpt_sw_start_timer_ms(powerkey_state.timer_handle, \
                                                powerkey_context.longpress_time, \
                                                (hal_gpt_callback_t)powerkey_process_repeat_and_longpress, NULL);

            if (ret_gpt != HAL_GPT_STATUS_OK) {
                printf("[pwk][key_init]start timer error,ret = %d, handle = 0x%x\r\n", \
                       (unsigned int)ret_gpt, \
                       (unsigned int)powerkey_state.timer_handle);
            }
            powerkey_push_one_key_to_buffer(powerkey_state.current_state, powerkey_context.registerd_data);
        }
    }

    /*Configure EINIT*/
    key_eint_cfg.trigger_mode = HAL_EINT_EDGE_FALLING_AND_RISING;
    key_eint_cfg.debounce_time = 0;
    hal_eint_mask((hal_eint_number_t)POWERKEY_EINT_NUMBER);
    hal_eint_init((hal_eint_number_t)POWERKEY_EINT_NUMBER, &key_eint_cfg);
    hal_eint_register_callback((hal_eint_number_t)POWERKEY_EINT_NUMBER, (hal_eint_callback_t)powerkey_process_handler, NULL);

    return HAL_KEYPAD_STATUS_OK;

}

hal_keypad_status_t hal_keypad_powerkey_deinit(void)
{
    if (powerkey_context.has_initilized != true) {
        return HAL_KEYPAD_STATUS_ERROR;
    }

    if (powerkey_buffer.press_count) {
        log_hal_info("[keypad][deinit]Can not deinit Powerkey as it is still pressed!\r\n");
        return HAL_KEYPAD_STATUS_ERROR;
    }
    if (powerkey_get_buffer_data_size() != 0) {
        log_hal_info("[keypad][deinit]powerkey has data in buffer\r\n");
        return HAL_KEYPAD_STATUS_ERROR;
    }
    hal_eint_mask((hal_eint_number_t)POWERKEY_EINT_NUMBER);
    powerkey_context.has_initilized = false;

    /*Stop and free gpt sw timer*/
    hal_gpt_sw_stop_timer_ms(powerkey_state.timer_handle);
    hal_gpt_sw_free_timer(powerkey_state.timer_handle);

    return HAL_KEYPAD_STATUS_OK;
}

hal_keypad_status_t hal_keypad_powerkey_register_callback(hal_powerkey_callback_t callback, void *user_data)
{
    if (powerkey_context.has_initilized != true) {
        return HAL_KEYPAD_STATUS_ERROR;
    }

    if (callback == NULL) {
        return HAL_KEYPAD_INVALID_PARAMETER;
    }
    /*Make sure the EINT is mask*/
    hal_eint_mask((hal_eint_number_t)POWERKEY_EINT_NUMBER);

    powerkey_context.powerkey_callback.callback  = callback;
    powerkey_context.powerkey_callback.user_data = user_data;
    eint_ack_interrupt((hal_eint_number_t)POWERKEY_EINT_NUMBER);

    /*Check still pressed*/
    if ((pmu_get_register_value(POWERKEY_PMU_KEY_STATUS, 1, 13) == 0x01) && (powerkey_state.current_state == HAL_KEYPAD_KEY_PRESS)) {
        if (powerkey_context.has_initilized == true && powerkey_context.powerkey_callback.callback != 0) {
            powerkey_context.powerkey_callback.callback(powerkey_context.powerkey_callback.user_data);
        }
    } else {
        hal_gpt_sw_stop_timer_ms(powerkey_state.timer_handle);
        /*Clear status and buffer*/
        memset(&powerkey_buffer, 0, sizeof(powerkey_buffer_t));
        powerkey_state.current_state = HAL_KEYPAD_KEY_RELEASE;
        powerkey_unlock_sleep();
    }
    hal_eint_unmask((hal_eint_number_t)POWERKEY_EINT_NUMBER);

    return HAL_KEYPAD_STATUS_OK;
}


hal_keypad_status_t hal_keypad_powerkey_get_key(hal_keypad_powerkey_event_t *powerkey_event)
{
    if (powerkey_get_buffer_data_size() <= 0) {
        log_hal_info("[keypad][get key]powerkey no data in buffer\r\n");
        return HAL_KEYPAD_STATUS_ERROR;
    }

    powerkey_pop_one_key_from_buffer(powerkey_event);
    log_hal_info("[keypad[get key]power key state=%d, position=%d\r\n", (int)powerkey_event->state, (int)powerkey_event->key_data);

    return HAL_KEYPAD_STATUS_OK;
}
#endif /* HAL_KEYPAD_FEATURE_POWERKEY*/
#endif /* HAL_KEYPAD_MODULE_ENABLED */

