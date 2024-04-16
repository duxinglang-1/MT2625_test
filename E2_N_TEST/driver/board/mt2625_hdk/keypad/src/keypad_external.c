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
#ifdef HAL_KEYPAD_MODULE_EXTERN_KEY_ENABLED
#include "keypad_custom.h"
#include "apb_proxy.h"
//#include "apb_proxy_example.h"
#include "FreeRTOS.h"
#include <string.h>
#include <stdio.h>

#define APB_PROXY_EX_KP_CMD_ID     0xF0E0D0C0B

#ifdef HAL_KEYPAD_FEATURE_POWERKEY
void powerkey_send_keyevent(void)
{
    hal_keypad_status_t ret;
    hal_keypad_powerkey_event_t powerkey_event;
    volatile int  key_data  = 0;
    volatile int  key_state = 0;
    volatile uint32_t key_position = 0;
    apb_proxy_at_cmd_result_t cmd_result;
    char *string[5] = {"release", "press", "longpress", "repeat", "pmu_longpress"};
    uint32_t loop_count = 0;

    while (1) {
        if (loop_count++ > 32) {
            printf("[ex_kp]powerkey loop dead\r\n");
        }

        ret = hal_keypad_get_key(&powerkey_event);

        if (ret == HAL_KEYPAD_STATUS_ERROR) {
            printf("[ex_kp]normal no key in buffer\r\n");
            break;
        }

        key_position  = powerkey_event.key_data; //key postion in register
        key_state	   = powerkey_event.state;
        printf("[ex_kp]KEY position:[%d], data:[%d]\r\n", (int)keypad_event.key_data, (int)key_data, (char *)string[key_state]);

        cmd_result.error_code = APB_PROXY_ERROR_NOT_USED;	   //????
        cmd_result.result_code = APB_PROXY_RESULT_OK;
        //cmd_result.pdata = "p=OK!";
        snprintf((char *)cmd_result.pdata, sizeof(cmd_result.pdata), "position:[%d], stata:[%d]\r\n", (int)key_position, (int)key_state, (int)key_data);
        cmd_result.length = strlen(cmd_result.pdata) + 1;
        cmd_result.cmd_id = APB_PROXY_EX_KP_CMD_ID;
        apb_proxy_send_at_cmd_result(&cmd_result);
    }
}
#endif



void key_send_keyevent(void)
{

    hal_keypad_status_t ret;
    hal_keypad_event_t keypad_event;
    volatile int  key_data  = 0;
    volatile int  key_state = 0;
    volatile uint32_t key_position = 0;
    apb_proxy_at_cmd_result_t cmd_result;
    uint32_t loop_count = 0;
    char *string[5] = {"release", "press", "longpress", "repeat", "pmu_longpress"};

    while (1) {
        if (loop_count++ > 32) {
            printf("[ex_kp]powerkey loop dead\r\n");
        }
        ret = hal_keypad_get_key(&keypad_event);
        if (ret == HAL_KEYPAD_STATUS_ERROR) {
            printf("[ex_kp]normal no key in buffer\r\n");
            break;
        }

        key_position  = keypad_event.key_data; //key postion in register
        key_state     = keypad_event.state;
        key_data      = keypad_custom_translate_keydata(keypad_event.key_data);


        printf("[ex_kp]KEY position:[%d], data:[%d], state:[%s]\r\n", (int)keypad_event.key_data, (int)key_data, (char *)string[key_state]);

        cmd_result.error_code = APB_PROXY_ERROR_NOT_USED;     //????
        cmd_result.result_code = APB_PROXY_RESULT_OK;
        //cmd_result.pdata = "p=OK!";
        snprintf((char *)cmd_result.pdata, sizeof(cmd_result.pdata), "position:[%d], stata:[%d], data:[%d]\r\n", (int)key_position, (int)key_state, (int)key_data);
        cmd_result.length = strlen(cmd_result.pdata) + 1;
        cmd_result.cmd_id = APB_PROXY_EX_KP_CMD_ID;
        apb_proxy_send_at_cmd_result(&cmd_result);
    }
}


void keypad_external_callback(void)
{
    key_send_keyevent();
}


#ifdef HAL_KEYPAD_FEATURE_POWERKEY
void power_keypad_external_callback(void)
{
    powerkey_send_keyevent();
}
#endif

bool keypad_external_keypad_enable(void)
{
    hal_keypad_status_t ret_kp;
    keypad_custom_init();
    ret_kp = hal_keypad_register_callback((hal_keypad_callback_t)keypad_external_callback, NULL);
    if (HAL_KEYPAD_STATUS_OK != ret_kp) {
        printf("[ex_kp]kp register callback fail\r\n");
        return false;
    }
#ifdef HAL_KEYPAD_FEATURE_POWERKEY
    ret_kp = hal_keypad_powerkey_register_callback((hal_keypad_callback_t)power_keypad_external_callback, NULL);
    if (HAL_KEYPAD_STATUS_OK != ret_kp) {
        printf("[ex_kp]power_kp register callback fail\r\n");
        return false;
    }
#endif

    ret_kp = hal_keypad_enable();
    if (HAL_KEYPAD_STATUS_OK != ret_kp) {
        printf("[ex_kp]kp register callback fail\r\n");
        return false;
    }

    return true;
}




void apb_proxy_at_send_kp_event_handler(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    configASSERT(p_parse_cmd != NULL);
    apb_proxy_at_cmd_result_t cmd_result;
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {

            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            cmd_result.error_code = APB_PROXY_ERROR_NOT_USED;     //????
            cmd_result.result_code = APB_PROXY_RESULT_OK;
            cmd_result.pdata = NULL;//"OK!";
            cmd_result.length = 0;//strlen(cmd_result.pdata) + 1;
            cmd_result.cmd_id = APB_PROXY_EX_KP_CMD_ID;
            apb_proxy_send_at_cmd_result(&cmd_result);

            // check atcmd from 2503

            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            /*send URC.*/
            cmd_result.error_code = APB_PROXY_ERROR_NOT_USED;     //????
            cmd_result.result_code = APB_PROXY_RESULT_OK;
            cmd_result.pdata = NULL;
            cmd_result.length = 0;
            cmd_result.cmd_id = APB_PROXY_EX_KP_CMD_ID;
            apb_proxy_send_at_cmd_result(&cmd_result);
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }
        default: {
            break;
        }
    }
}

#endif
#endif


