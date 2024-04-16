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

#include "apb_proxy.h"
#include "apb_proxy_nw_cmd.h"
#include "apb_proxy_nw_cmd_util.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sntp.h"
#include <stdlib.h>
#include <stdio.h>
#include "syslog.h"

#ifdef APBSOC_MODULE_PRINTF
#define NW_LOG(fmt, ...)               printf("[APB SNTP] "fmt, ##__VA_ARGS__)
#else
#define NW_LOG(fmt, ...)               LOG_I(apbnw, "[APB SNTP] "fmt, ##__VA_ARGS__)
#endif

// TODO: deep sleep
static char* server = NULL; 
static int is_finish = 1;

void sntp_callback_f (hal_rtc_time_t time)
{
    char result_buf[50] = {0};
    apb_proxy_at_cmd_result_t cmd_result = {0};
    uint16_t milli_sec = 0;
    
    NW_LOG("sntp callback");
    

    // TODO:
    is_finish = 1;
    sntp_stop();
    if (server) {
        _free_buffer(server);
        server = NULL;
    }

#if (PRODUCT_VERSION == 2625)
    milli_sec = time.rtc_milli_sec;
#endif

    snprintf(result_buf, 50, "+ESNTP:%d,%d,%d,%d,%d,%d,%d",time.rtc_year, time.rtc_mon, time.rtc_day,
            time.rtc_hour, time.rtc_min, time.rtc_sec, milli_sec);

    cmd_result.pdata = result_buf;
    cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
    cmd_result.length = strlen(cmd_result.pdata);
    cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
    apb_proxy_send_at_cmd_result(&cmd_result);
}

static void sntp_loop_task(void *not_used)
{
    NW_LOG("enter sntp atcmd task");
#if SNTP_SERVER_DNS
    sntp_setservername(0, server);
#else
    addr = ipaddr_addr(server);
    if (addr != IPADDR_NONE) {
        sntp_setserver(0, &addr);
    }
#endif
    sntp_init();
    sntp_set_callback(sntp_callback_f);

    vTaskDelete(NULL);
}

apb_proxy_status_t apb_proxy_hdlr_sntp_start_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;
    char* start = NULL;
    char* temp = NULL;
    int32_t param_cnt;
    NW_LOG("sntp start: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;
            NW_LOG("start %s", start);

            if (is_finish == 0) {
                NW_LOG("SNTP not finished from last");
                goto exit;
            }

            param_cnt = apb_nw_util_parse_all_parameters(start, NULL, "s", &temp);
            if (temp == NULL || param_cnt < 1) {
                NW_LOG("Cannot get parameter");
                goto exit;
            }

            server = _alloc_buffer(strlen(temp) + 1);
            if (server == NULL){
                NW_LOG("memory error");
                goto exit;
            }
            memcpy(server, temp, strlen(temp));

            NW_LOG("server %s", server);
            is_finish = 0;
            xTaskCreate(sntp_loop_task,
                    "sntp_atcmd",
                    3 * 1024 / sizeof(portSTACK_TYPE),
                    NULL,
                    TASK_PRIORITY_NORMAL,
                    NULL); 
            NW_LOG("create sntp atcmd task");

            cmd_result.result_code = APB_PROXY_RESULT_OK;
            ret = APB_PROXY_STATUS_OK;

            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
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

    exit:    
    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
    return ret;
}

apb_proxy_status_t apb_proxy_hdlr_sntp_stop_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;
    NW_LOG("sntp stop [%s]", p_parse_cmd->string_ptr);

    if (is_finish == 0) {
        sntp_stop();

        if (server) {
            _free_buffer(server);
            server = NULL;
        }
    }

    is_finish = 1;

    // delete sntp atcmd task
    cmd_result.pdata = NULL;
    cmd_result.result_code = APB_PROXY_RESULT_OK;
    ret = APB_PROXY_STATUS_OK;
    cmd_result.length = 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
    return ret;
}


