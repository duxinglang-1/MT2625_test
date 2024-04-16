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

#ifdef HAL_KEYPAD_FEATURE_POWERKEY
#include "hal_keypad_powerkey_internal.h"
#ifdef HAL_SLEEP_MANAGER_ENABLED
#include "hal_sleep_manager.h"
#include "hal_sleep_driver.h"

const static char *powerkey_lock_sleep_name = "Powerkey";
static uint8_t powerkey_lock_sleep_handle;

#define POWERKEY_GET_SLEEP_HANDLE() \
do{ \
    powerkey_lock_sleep_handle = hal_sleep_manager_set_sleep_handle(powerkey_lock_sleep_name); \
    if(powerkey_lock_sleep_handle == INVALID_SLEEP_HANDLE ) { \
        log_hal_error("[pwk]:get sleep handle failed\r\n"); \
    } \
}while(0)

#define POWERKEY_RELEASE_SLEEP_HANDLE() \
do{ \
    hal_sleep_manager_release_sleep_handle(powerkey_lock_sleep_handle); \
}while(0)

#define POWERKEY_LOCK_SLEEP() \
do{ \
    hal_sleep_manager_acquire_sleeplock(powerkey_lock_sleep_handle, HAL_SLEEP_LOCK_ALL); \
}while(0)

#define POWERKEY_UNLOCK_SLEEP() \
do{ \
    hal_sleep_manager_release_sleeplock(powerkey_lock_sleep_handle, HAL_SLEEP_LOCK_ALL); \
}while(0)

#else
#define POWERKEY_GET_SLEEP_HANDLE()
#define POWERKEY_RELEASE_SLEEP_HANDLE()
#define POWERKEY_LOCK_SLEEP()
#define POWERKEY_UNLOCK_SLEEP()
#endif


powerkey_buffer_t powerkey_buffer;
powerkey_state_t  powerkey_state;

void powerkey_push_one_key_to_buffer(hal_keypad_key_state_t state, uint32_t data)
{
    powerkey_buffer.data[powerkey_buffer.write_index].state    = state;
    powerkey_buffer.data[powerkey_buffer.write_index].key_data = data;
    powerkey_buffer.write_index++;
    powerkey_buffer.write_index &=  POWERKEY_BUFFER_SIZE - 1;
}

void powerkey_pop_one_key_from_buffer(hal_keypad_powerkey_event_t *key_event)
{
    key_event->state     = powerkey_buffer.data[powerkey_buffer.read_index].state;
    key_event->key_data  = powerkey_buffer.data[powerkey_buffer.read_index].key_data;
    powerkey_buffer.read_index++;
    powerkey_buffer.read_index &=  POWERKEY_BUFFER_SIZE - 1;
}

uint32_t powerkey_get_buffer_left_size(void)
{
    if (powerkey_buffer.write_index >= powerkey_buffer.read_index) {

        return (POWERKEY_BUFFER_SIZE - powerkey_buffer.write_index + powerkey_buffer.read_index);
    } else {
        return (powerkey_buffer.read_index - powerkey_buffer.write_index);

    }
}

uint32_t powerkey_get_buffer_data_size(void)
{
    return (POWERKEY_BUFFER_SIZE - powerkey_get_buffer_left_size());

}

void powerkey_get_sleep_handle(void)
{
    POWERKEY_GET_SLEEP_HANDLE();
}

void powerkey_lock_sleep(void)
{
    POWERKEY_LOCK_SLEEP();
	log_hal_info("powerkey_lock_sleep\r\n");
}

void powerkey_unlock_sleep(void)
{
    POWERKEY_UNLOCK_SLEEP();
	log_hal_info("powerkey_unlock_sleep\r\n");
}


void powerkey_process_repeat_and_longpress(uint32_t *powerkey_type)
{
    hal_gpt_status_t ret_gpt;

    if (powerkey_get_buffer_left_size() > 2) {
        if (powerkey_state.current_state == HAL_KEYPAD_KEY_PRESS) {
            powerkey_state.current_state = HAL_KEYPAD_KEY_LONG_PRESS;
        } else {
            powerkey_state.current_state = HAL_KEYPAD_KEY_REPEAT;
        }

        /*start timer*/
        ret_gpt = hal_gpt_sw_start_timer_ms(\
                                            powerkey_state.timer_handle, \
                                            powerkey_context.repeat_time, \
                                            (hal_gpt_callback_t)powerkey_process_repeat_and_longpress, \
                                            (void *)(&powerkey_context.registerd_data));
        if (ret_gpt != HAL_GPT_STATUS_OK) {
            log_hal_info("[pwk][timer_cb]start timer error,ret = %d, handle = 0x%x\r\n", (int)ret_gpt, (int)powerkey_state.timer_handle);
        }

        powerkey_push_one_key_to_buffer(powerkey_state.current_state, powerkey_context.registerd_data);
        log_hal_info("[pwk][timer_cb]key state = %d, key_data = 0x%x\r\n", (int)powerkey_state.current_state, (int)powerkey_context.registerd_data);
        if (powerkey_context.powerkey_callback.callback != 0) {
            powerkey_context.powerkey_callback.callback(powerkey_context.powerkey_callback.user_data);
        }
    } else {
        log_hal_info("[pwk][timer_cb]No root left\r\n");
    }

}

ATTR_TEXT_IN_TCM void powerkey_process_handler(void)
{
    hal_gpt_status_t ret_gpt;

    if ((pmu_get_register_value(POWERKEY_PMU_KEY_STATUS, 1, 13) == 0x01) && (powerkey_state.current_state == HAL_KEYPAD_KEY_RELEASE)) {
        POWERKEY_LOCK_SLEEP();
        if (powerkey_get_buffer_left_size() > 2) {
            powerkey_state.current_state = HAL_KEYPAD_KEY_PRESS;
            /*start timer*/
            ret_gpt = hal_gpt_sw_start_timer_ms(powerkey_state.timer_handle, \
                                                powerkey_context.longpress_time, \
                                                (hal_gpt_callback_t)powerkey_process_repeat_and_longpress, NULL);

            if (ret_gpt != HAL_GPT_STATUS_OK) {
                log_hal_info("[pwk]start timer error,ret = %d, handle = 0x%x\r\n", \
                             (unsigned int)ret_gpt, \
                             (unsigned int)powerkey_state.timer_handle);
                hal_eint_unmask((hal_eint_number_t)POWERKEY_EINT_NUMBER);
                return;
            }
            powerkey_push_one_key_to_buffer(powerkey_state.current_state, powerkey_context.registerd_data);
        }
    } else {
        if (powerkey_get_buffer_left_size() > 0) {
            if (HAL_KEYPAD_KEY_RELEASE == powerkey_state.current_state) {
                log_hal_info("[pwk]press already released\r\n");
                hal_eint_unmask((hal_eint_number_t)POWERKEY_EINT_NUMBER);
                return;
            }
            powerkey_state.current_state = HAL_KEYPAD_KEY_RELEASE;
            hal_gpt_sw_stop_timer_ms(powerkey_state.timer_handle);
            powerkey_push_one_key_to_buffer(powerkey_state.current_state, powerkey_context.registerd_data);
        }
        POWERKEY_UNLOCK_SLEEP();
    }

    if (powerkey_context.has_initilized == true && powerkey_context.powerkey_callback.callback != 0) {
        powerkey_context.powerkey_callback.callback(powerkey_context.powerkey_callback.user_data);
    } else {
        log_hal_info("[pwk]powerkey not inited error\r\n");
    }

    hal_eint_unmask((hal_eint_number_t)POWERKEY_EINT_NUMBER);
}


#endif //HAL_KEYPAD_FEATURE_POWERKEY


