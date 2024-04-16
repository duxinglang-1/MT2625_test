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

#ifndef __HAL_KEYPAD_POWERKEY_INTERNAL_H__
#define __HAL_KEYPAD_POWERKEY_INTERNAL_H__

#include "hal_keypad.h"
#ifdef HAL_KEYPAD_FEATURE_POWERKEY
#include "hal_keypad_internal.h"
#include "hal_eint.h"
#include "hal_pmu.h"
#include "mt2625.h"
#include <assert.h>
#include "hal_log.h"
#include "hal_gpt.h"
#include "hal_gpt_internal.h"
#include "memory_attribute.h"


#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************/
#define POWERKEY_BUFFER_SIZE            (32)
#define POWERKEY_STATUS_LONG_PRESS      (0x02)
#define POWERKEY_STATUS_LONG_RELEASE    (0x01)

#define POWERKEY_STATUS_PRESS           (0x02)
#define POWERKEY_STATUS_RELEASE         (0x01)

#define POWERKEY_DEFAUL_REPEAT_TIME     (1000)
#define POWERKEY_DEFAUL_LONGPRESS_TIME  (2000)
#define POWERKEY_EINT_NUMBER            (31)
#define POWERKEY_PMU_KEY_STATUS         (0xA2070A98)


/***************************************************************/
typedef struct {
    hal_powerkey_callback_t callback;
    void *user_data;
} powerkey_callback_context_t;

typedef struct {
    uint32_t            registerd_data;
    uint32_t            release_count;
    uint32_t            timer_handle;
    bool                has_initilized;
    powerkey_callback_context_t powerkey_callback;
    uint32_t            longpress_time;
    uint32_t            repeat_time;
} powerkey_context_t;

typedef struct {
    hal_keypad_powerkey_event_t data[POWERKEY_BUFFER_SIZE];
    uint8_t write_index;
    uint8_t read_index;
    uint8_t press_count;
} powerkey_buffer_t;


typedef struct {
    uint32_t timer_handle;
    hal_keypad_key_state_t current_state;
} powerkey_state_t;

/********* varible extern *************/
extern powerkey_context_t powerkey_context;

/******** funtion extern **************/

uint32_t powerkey_get_buffer_data_size(void);
uint32_t powerkey_get_buffer_left_size(void);
void powerkey_pop_one_key_from_buffer(hal_keypad_powerkey_event_t *key_event);
void powerkey_push_one_key_to_buffer(hal_keypad_key_state_t state, uint32_t data);
void powerkey_process_handler(void);
void powerkey_get_sleep_handle(void);
void powerkey_lock_sleep(void);
void powerkey_unlock_sleep(void);
void powerkey_process_repeat_and_longpress(uint32_t *powerkey_type);



#ifdef __cplusplus
}
#endif

#endif /* HAL_KEYPAD_FEATURE_POWERKEY */
#endif /* _HAL_KEYPAD_POWERKEY_INTERNAL_H_ */

