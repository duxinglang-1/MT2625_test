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

#include "apb_proxy.h"
#include "FreeRTOS.h"
#include "string.h"
#include "stdint.h"
#include "stdbool.h"
#include "gnss_fota_log.h"
#include "task.h"
#include "memory_map.h"
#include "hal_pinmux_define.h"
#include "hal_gpio.h"
#include "hal_uart.h"
#include "gnss_fota_main.h"
#include "gnss_fota_bin.h"
#include "gnss_app.h"
#include "hal_wdt.h"
#include "timers.h"
#include "apb_proxy_fota_context.h"

static apb_proxy_cmd_id_t g_cmd_id = APB_PROXY_INVALID_CMD_ID;
static TimerHandle_t gnss_fota_reboot_timer = NULL;
static bool is_gnss_fota_processing = false;

static void gps_fota_task_main(void *arg);
static void gps_fota_task_start(void);
static void gps_fota_send_final_result(int32_t result);
static void gnss_fota_update_progress(gnss_fota_state_t fota_state, uint32_t percent);
static void gnss_fota_reboot_timer_hdlr(TimerHandle_t timer_id)
{
    hal_wdt_status_t ret;
    hal_wdt_config_t wdt_config;
    wdt_config.mode = HAL_WDT_MODE_RESET;
    wdt_config.seconds = 1;
    hal_wdt_init(&wdt_config);
    ret = hal_wdt_software_reset();
    if (ret != HAL_WDT_STATUS_OK){
        gnss_fota_log_error("watch dog reset error.");
    }
}

static void gnss_fota_reboot_device(uint32_t delay_msec)
{
    if (gnss_fota_reboot_timer && (xTimerIsTimerActive(gnss_fota_reboot_timer) != pdFALSE)) {
        xTimerStop(gnss_fota_reboot_timer, 0);
    } else {
        gnss_fota_reboot_timer = xTimerCreate( "fota_reboot_timer",
                            (delay_msec/portTICK_PERIOD_MS),
                            pdFALSE,
                            NULL,
                            gnss_fota_reboot_timer_hdlr);
    }

    xTimerStart(gnss_fota_reboot_timer, 0);
}

apb_proxy_status_t gnss_fota_set_bin_len_handler(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result;
    configASSERT(p_parse_cmd != NULL);
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.pdata = NULL;
    cmd_result.length = 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION: {
            char* p_param = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;
            uint32_t binary_len = atoi((const char*)p_param);
            if (binary_len > 0){
                apb_proxy_fota_context_init();
                apb_proxy_fota_set_binary_info(FOTA_RESERVED_BASE, binary_len);
                cmd_result.result_code = APB_PROXY_RESULT_OK;
            }
            break;
        }
        case APB_PROXY_CMD_MODE_READ:
        case APB_PROXY_CMD_MODE_ACTIVE:
        case APB_PROXY_CMD_MODE_TESTING:
        case APB_PROXY_CMD_MODE_INVALID:
        default:{
            break;
        }
    }
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t gnss_power_on_handler(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result;
    configASSERT(p_parse_cmd != NULL);
    cmd_result.result_code = APB_PROXY_RESULT_OK;
    cmd_result.pdata = NULL;
    cmd_result.length = 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            (void)hal_pinmux_set_function(HAL_GPIO_8, HAL_GPIO_8_GPIO8); // Set the pin to GPIO mode.
            (void)hal_gpio_set_direction(HAL_GPIO_8, HAL_GPIO_DIRECTION_OUTPUT);
            (void)hal_gpio_set_output(HAL_GPIO_8, HAL_GPIO_DATA_HIGH);
            (void)hal_pinmux_set_function(HAL_GPIO_30, HAL_GPIO_30_CLKO3); // Set the pin to GPIO mode.
            hal_gpio_set_clockout(HAL_GPIO_30, HAL_GPIO_CLOCK_MODE_32K);
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID:
        default:{
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
    }
    apb_proxy_send_at_cmd_result(&cmd_result);

return APB_PROXY_STATUS_OK;

}

apb_proxy_status_t fota_gnss_update_handler(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result;
    char fota_buffer[] = "AT+GNSSFOTA: trigger GNSS binary update.\r\nAT+GNSSFOTA=\"URL\": download the GNSS binary.\r\nAT+GNSSFOTA=\"poweron\": power on GNSS.\r\nAT+GNSSFOTA=\"poweroff\": power off GNSS.";

    configASSERT(p_parse_cmd != NULL);
    cmd_result.result_code = APB_PROXY_RESULT_OK;
    cmd_result.pdata = NULL;
    cmd_result.length = 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            if (false == is_gnss_fota_processing){
                is_gnss_fota_processing = true;
                cmd_result.result_code = APB_PROXY_RESULT_OK;
                memset(fota_buffer, 0, sizeof(fota_buffer));
                strcpy(fota_buffer, "GNSS FOTA will begin.");
                cmd_result.pdata = fota_buffer;
                cmd_result.length = strlen((const char*)fota_buffer);
                g_cmd_id = p_parse_cmd->cmd_id;
                gps_fota_task_start();
            }else{
                cmd_result.result_code = APB_PROXY_RESULT_BUSY;
            }
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            uint32_t index = 0;
            char fota_buffer[32] = {0};
            char* p_parameter = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;
            if (is_gnss_fota_processing == true){
                cmd_result.result_code = APB_PROXY_RESULT_BUSY;
                break;
            }
            memset(fota_buffer, 0, sizeof(fota_buffer));
            if (*p_parameter == '\"'){
                for(index = 1; (index < sizeof(fota_buffer)) && (index < (p_parse_cmd->string_len - p_parse_cmd->parse_pos));
                    index ++){
                    if (p_parameter[index] == '\"'){
                        break;
                    }
                    fota_buffer[index - 1] = p_parameter[index];
                }
                if (strcmp(fota_buffer, "reboot") == 0){
                    gnss_fota_reboot_device(3000);
                }
                else if (strcmp(fota_buffer, "poweron") == 0){
                    /* Enable power.*/
                    (void)hal_pinmux_set_function(HAL_GPIO_8, HAL_GPIO_8_GPIO8); // Set the pin to GPIO mode.
                    (void)hal_gpio_set_direction(HAL_GPIO_8, HAL_GPIO_DIRECTION_OUTPUT);
                    (void)hal_gpio_set_output(HAL_GPIO_8, HAL_GPIO_DATA_HIGH);
                    /* Enable 32K output. */
                    (void)hal_pinmux_set_function(HAL_GPIO_30, HAL_GPIO_30_CLKO3); // Set the pin to GPIO mode.
                    hal_gpio_set_clockout(HAL_GPIO_30, HAL_GPIO_CLOCK_MODE_32K);
                }else if (strcmp(fota_buffer, "poweroff") == 0){
                    /* Disable power.*/
                    (void)hal_pinmux_set_function(HAL_GPIO_8, HAL_GPIO_8_GPIO8);
                    (void)hal_gpio_set_direction(HAL_GPIO_8, HAL_GPIO_DIRECTION_OUTPUT);
                    (void)hal_gpio_set_output(HAL_GPIO_8, HAL_GPIO_DATA_LOW);
                    /* Disable 32K output. */
                    (void)hal_pinmux_set_function(HAL_GPIO_30, HAL_GPIO_30_GPIO30);
                    (void)hal_gpio_set_direction(HAL_GPIO_30, HAL_GPIO_DIRECTION_OUTPUT);
                    (void)hal_gpio_set_output(HAL_GPIO_30, HAL_GPIO_DATA_LOW);
                }else{
                    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                }
            }else{
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            }
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            cmd_result.pdata = fota_buffer;
            cmd_result.length = strlen(cmd_result.pdata);
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID:
        default:{
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
    }
    apb_proxy_send_at_cmd_result(&cmd_result);

    return APB_PROXY_STATUS_OK;
}

static void gnss_fota_update_progress(gnss_fota_state_t fota_state, uint32_t percent)
{
    apb_proxy_at_cmd_result_t cmd_result;
    char buffer[32] = {0};
    cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
    cmd_result.pdata = NULL;
    cmd_result.length = 0;
    cmd_result.cmd_id = g_cmd_id;
    switch(fota_state){
        case GNSS_BROM_STATE: {
            snprintf(buffer, sizeof(buffer), "LOAD DA:%lu%%", percent);
            break;
        }
        case GNSS_DA_STATE: {
            snprintf(buffer, sizeof(buffer), "UPDATE GNSS:%lu%%", percent);
            break;
        }
        default: {
            snprintf(buffer, sizeof(buffer), "INVALID STATE");
            break;
        }
    }
    cmd_result.pdata = (void*)buffer;
    cmd_result.length = strlen((const char*)buffer);
    apb_proxy_send_at_cmd_result(&cmd_result);
}

static void gps_fota_task_main(void *arg)
{
    gnss_fota_parameter_t gnss_fota_param;
    gnss_fota_result_t result = GNSS_FOTA_OK;
    gnss_fota_param.gnss_da_data.gnss_fota_bin_init = gnss_fota_da_bin_init;
    gnss_fota_param.gnss_da_data.gnss_fota_get_bin_length = gnss_fota_get_da_bin_length;
    gnss_fota_param.gnss_da_data.gnss_fota_get_data = gnss_fota_get_da_data;

    gnss_fota_param.gnss_update_data.gnss_fota_bin_init = gnss_fota_gnss_bin_init;
    gnss_fota_param.gnss_update_data.gnss_fota_get_bin_length = gnss_fota_get_gnss_bin_length;
    gnss_fota_param.gnss_update_data.gnss_fota_get_data = gnss_fota_get_gnss_data;

    /* update progress is NULL, no progress data output.*/
    gnss_fota_param.gnss_update_progress= gnss_fota_update_progress;
    gnss_demo_app_stop();
    result = gnss_fota_main(&gnss_fota_param);
    gps_fota_send_final_result(result);
    is_gnss_fota_processing = false;
    vTaskDelete(NULL);
}

static void gps_fota_task_start(void)
{
    xTaskCreate(gps_fota_task_main,
                FOTA_TASK_NAME,
                FOTA_TASK_STACKSIZE / ((uint32_t)sizeof(StackType_t)),
                NULL, FOTA_TASK_PRIORITY, NULL);
}

static void gps_fota_send_final_result(int32_t result)
{
    apb_proxy_at_cmd_result_t cmd_result;
    char buffer[16] = {0};
    cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
    cmd_result.pdata = "OK";
    cmd_result.length = strlen((const char*)(cmd_result.pdata));
    cmd_result.cmd_id = g_cmd_id;

    if (result != GNSS_FOTA_OK){
        snprintf(buffer, sizeof(buffer), "ERROR CODE:%ld", result);
        cmd_result.pdata = (void*)buffer;
        cmd_result.length = strlen((const char*)buffer);
    }
    apb_proxy_send_at_cmd_result(&cmd_result);
}
