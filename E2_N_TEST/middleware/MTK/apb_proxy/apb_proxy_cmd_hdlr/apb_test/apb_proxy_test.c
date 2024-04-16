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
#include "apb_proxy_test.h"
#include "FreeRTOS.h"
#include "string.h"
#include "apb_proxy_log.h"

static apb_proxy_data_conn_id_t g_current_conn_id = 0;
static apb_proxy_cmd_id_t g_current_cmd_id = 0;

/*The callback function used for user data mode.*/
static void apb_proxy_at_cmd3_data_mode_event_callback(apb_proxy_event_type_t event, void *pdata);

apb_proxy_status_t apb_proxy_at_testcmd1_event_handler(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    configASSERT(p_parse_cmd != NULL);
    apb_proxy_at_cmd_result_t cmd_result;
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
            /*do something*/
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
    cmd_result.result_code = APB_PROXY_RESULT_OK;
    cmd_result.pdata = "AP Bridge Proxy result";
    cmd_result.length = strlen("AP Bridge Proxy result");
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);

    /*send URC.*/
    cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
    cmd_result.pdata = "AP Bridge Proxy URC Test";
    cmd_result.length = strlen((const char*)(cmd_result.pdata));
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;

}

apb_proxy_status_t apb_proxy_at_testcmd2_event_handler(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    configASSERT(p_parse_cmd != NULL);
    apb_proxy_at_cmd_result_t cmd_result;
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
            /*do something*/
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
    cmd_result.result_code = APB_PROXY_ERROR_CME_NO_NETWORK_SERVICE;
    cmd_result.pdata = NULL;
    cmd_result.length = 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_at_testcmd3_event_handler(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    configASSERT(p_parse_cmd != NULL);
    apb_proxy_at_cmd_result_t cmd_result;
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
            /*do something*/
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
    cmd_result.result_code = APB_PROXY_RESULT_CONNECT;
    cmd_result.pdata = NULL;
    cmd_result.length = 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    if (apb_proxy_send_at_cmd_result(&cmd_result) == APB_PROXY_STATUS_OK){
        g_current_cmd_id = p_parse_cmd->cmd_id;
        g_current_conn_id = apb_proxy_create_data_mode(apb_proxy_at_cmd3_data_mode_event_callback, p_parse_cmd->cmd_id);
        if (g_current_conn_id == 0){
            apb_proxy_log_error("apb testcmd3: failed to go to data mode");
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            apb_proxy_send_at_cmd_result(&cmd_result);
        }
    }else{
        cmd_result.result_code = APB_PROXY_RESULT_ERROR;
        apb_proxy_send_at_cmd_result(&cmd_result);
    }

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_at_testcmd4_event_handler(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    configASSERT(p_parse_cmd != NULL);
    apb_proxy_at_cmd_result_t cmd_result;
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
            /*do something*/
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
    cmd_result.result_code = APB_PROXY_RESULT_CUSTOM_ERROR;
    cmd_result.pdata = "+CIS ERROR: 1000";
    cmd_result.length = strlen((const char*)(cmd_result.pdata));
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}

/*The callback function used for user data mode.*/
static void apb_proxy_at_cmd3_data_mode_event_callback(apb_proxy_event_type_t event, void *pdata)
{
    apb_proxy_at_cmd_result_t cmd_result;
    uint8_t user_data[100] = {0};
    apb_proxy_data_mode_event_t* data_mode_event = (apb_proxy_data_mode_event_t*)pdata;
    switch (event) {
        case APB_PROXY_USER_DATA_IND: {
            uint32_t index = 0;
            apb_proxy_user_data_t *p_user_data = &(data_mode_event->event_data.user_data);
            apb_proxy_user_data_t new_user_data;
            memcpy(user_data, p_user_data->pbuffer, p_user_data->length);
            user_data[p_user_data->length] = '\r';
            user_data[p_user_data->length + 1] = '\n';
            new_user_data.pbuffer = user_data;
            new_user_data.length = p_user_data->length + 2;
            for (index = 0; index < 20; index ++){
                if(apb_proxy_send_user_data(g_current_conn_id, &new_user_data) != APB_PROXY_STATUS_OK){
                   apb_proxy_log_error("Send user data failed.");
                }
            }
            configASSERT(apb_proxy_close_data_mode(g_current_conn_id) == APB_PROXY_STATUS_OK);
            g_current_conn_id = 0;
            cmd_result.result_code = APB_PROXY_RESULT_NO_CARRIER;
            cmd_result.pdata = NULL;
            cmd_result.length = 0;
            cmd_result.cmd_id = g_current_cmd_id;
            /*send final result.*/
            if (apb_proxy_send_at_cmd_result(&cmd_result) == APB_PROXY_STATUS_OK){
                apb_proxy_log_info("send NO CARRIER OK");
            }else{
                apb_proxy_log_info("send NO CARRIER ERROR");
            }
            break;
        }
        case APB_PROXY_STOP_SEND_USER_DATA: {
            break;
        }
        case APB_PROXY_RESUME_SEND_USER_DATA: {
            if (apb_proxy_close_data_mode(g_current_conn_id) != APB_PROXY_STATUS_OK){
                apb_proxy_log_info("close data mode: OK.");
            }else{
                apb_proxy_log_info("close data mode: ERROR.");
                g_current_conn_id = 0;
            }
            break;
        }
        default: {
            configASSERT(0);
            break;
        }
    }
}

apb_proxy_status_t apb_proxy_apbcmdtest_event_handler(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result;
    uint8_t* p_out_buffer = NULL;
    uint32_t offset = 0;
    configASSERT(p_parse_cmd != NULL);
    p_out_buffer = (uint8_t *)pvPortMalloc(APB_PROXY_MAX_DATA_SIZE);
    configASSERT(p_out_buffer != NULL);
    memset(p_out_buffer, 0, APB_PROXY_MAX_DATA_SIZE);
    cmd_result.result_code = APB_PROXY_RESULT_OK;
    cmd_result.pdata = NULL;
    cmd_result.length = 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    memcpy(p_out_buffer, p_parse_cmd->string_ptr, p_parse_cmd->name_len);
    offset = strlen((const char*)p_out_buffer);
    p_out_buffer[offset] = ':';
    offset++;

    memcpy(p_out_buffer + offset, "cmd=", strlen("cmd="));
    offset = strlen((const char*)p_out_buffer);
    p_out_buffer[offset] = '\"';
    offset++;
    /*The char '\r' is removed.*/
    memcpy(p_out_buffer + offset, p_parse_cmd->string_ptr, strlen((const char*)(p_parse_cmd->string_ptr)) - 1);
    offset = strlen((const char*)p_out_buffer);
    p_out_buffer[offset] = '\"';
    offset++;
    p_out_buffer[offset] = ';';
    offset++;

    snprintf((char*)(p_out_buffer + offset), APB_PROXY_MAX_DATA_SIZE - offset, "cmd_len=%lu", p_parse_cmd->string_len -1);
    offset = strlen((const char*)p_out_buffer);
    p_out_buffer[offset] = ';';
    offset++;

    snprintf((char*)(p_out_buffer + offset), APB_PROXY_MAX_DATA_SIZE - offset, "name_len=%lu", p_parse_cmd->name_len);
    offset = strlen((const char*)p_out_buffer);
    p_out_buffer[offset] = ';';
    offset++;

    snprintf((char*)(p_out_buffer + offset), APB_PROXY_MAX_DATA_SIZE - offset, "parse_pos=%lu", p_parse_cmd->parse_pos);
    offset = strlen((const char*)p_out_buffer);
    p_out_buffer[offset] = ';';
    offset++;
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            snprintf((char*)(p_out_buffer + offset), APB_PROXY_MAX_DATA_SIZE - offset, "mode=\"%s\"", "READ");
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            snprintf((char*)(p_out_buffer + offset), APB_PROXY_MAX_DATA_SIZE - offset, "mode=\"%s\"", "ACTIVE");
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            snprintf((char*)(p_out_buffer + offset), APB_PROXY_MAX_DATA_SIZE - offset, "mode=\"%s\"", "EXECUTE");
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            snprintf((char*)(p_out_buffer + offset), APB_PROXY_MAX_DATA_SIZE - offset, "mode=\"%s\"", "TEST");
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            snprintf((char*)(p_out_buffer + offset), APB_PROXY_MAX_DATA_SIZE - offset, "mode=\"%s\"", "INVALID");
            break;
        }
        default: {
            break;
        }

    }
    cmd_result.pdata = p_out_buffer;
    cmd_result.length = strlen((const char*)p_out_buffer);
    apb_proxy_send_at_cmd_result(&cmd_result);
    vPortFree(p_out_buffer);
    p_out_buffer = NULL;
    return APB_PROXY_STATUS_OK;

}
apb_proxy_status_t apb_proxy_apbresulttest_event_handler(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result;
    uint8_t* p_cmd_param = NULL;
    uint8_t* p_out_buffer = NULL;
    uint32_t index = 0;
    configASSERT(p_parse_cmd != NULL);
    cmd_result.result_code = APB_PROXY_RESULT_OK;
    cmd_result.pdata = NULL;
    cmd_result.length = 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    p_out_buffer = (uint8_t *)pvPortMalloc(APB_PROXY_MAX_DATA_SIZE);
    configASSERT(p_out_buffer != NULL);
    memset(p_out_buffer, 0, APB_PROXY_MAX_DATA_SIZE);

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
            bool found = false;
            uint32_t result_code;
            if (p_parse_cmd->parse_pos >= p_parse_cmd->string_len){
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                break;
            }
            p_cmd_param = (uint8_t*)(p_parse_cmd->string_ptr + p_parse_cmd->parse_pos);
            result_code = atoi((const char*)p_cmd_param);
            if ((result_code <= APB_PROXY_ERROR_CME_PDP_AUTHENTIFICATION_ERROR)
                && (result_code >= APB_PROXY_RESULT_OK)){
                cmd_result.result_code = (apb_proxy_at_cmd_result_code_t)(result_code);
            }else{
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            }
            if ((APB_PROXY_RESULT_CONNECT == cmd_result.result_code)
               || (APB_PROXY_RESULT_PROCEEDING == cmd_result.result_code)
               || (APB_PROXY_RESULT_UNSOLICITED == cmd_result.result_code)
               || (APB_PROXY_RESULT_CUSTOM_CONNECT == cmd_result.result_code)
               || (APB_PROXY_RESULT_CUSTOM_ERROR == cmd_result.result_code)){
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            }

            for(index = p_parse_cmd->parse_pos; index < p_parse_cmd->string_len; index ++){
                if ((*p_cmd_param) == ','){
                    found = true;
                    break;
                }
                p_cmd_param++;
            }
            if (true == found){
                p_cmd_param++;
                memcpy(p_out_buffer, p_cmd_param, p_parse_cmd->string_len - p_parse_cmd->parse_pos -1);
                cmd_result.pdata = p_out_buffer;
                cmd_result.length = strlen((const char*)p_out_buffer);
            }

            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
        default: {
            break;
      }

    }
    apb_proxy_send_at_cmd_result(&cmd_result);
    vPortFree(p_out_buffer);
    p_out_buffer = NULL;
    return APB_PROXY_STATUS_OK;
}

static uint32_t total_length = 0;
static uint32_t received_length = 0;
static apb_proxy_data_conn_id_t g_apb_data_test_conn_id = 0;
static apb_proxy_cmd_id_t g_apb_data_test_cmd_id = 0;

static void apb_proxy_apbdatatest_data_mode_event_callback(apb_proxy_event_type_t event, void *pdata)
{
    apb_proxy_at_cmd_result_t cmd_result;
    apb_proxy_data_mode_event_t* data_mode_event = (apb_proxy_data_mode_event_t*)pdata;
    apb_proxy_log_info("data mode callback , conn:%d, event:%d\r\n", data_mode_event->conn_id, event);
    configASSERT(pdata != NULL);
    switch (event) {
        case APB_PROXY_USER_DATA_IND: {
            apb_proxy_user_data_t *p_user_data = &(data_mode_event->event_data.user_data);
            received_length += p_user_data->length;
            if(apb_proxy_send_user_data(g_apb_data_test_conn_id, p_user_data) != APB_PROXY_STATUS_OK){
               apb_proxy_log_error("Send user data failed.");
            }

            if (received_length == total_length){
                configASSERT(apb_proxy_close_data_mode(g_apb_data_test_conn_id) == APB_PROXY_STATUS_OK);
                g_apb_data_test_conn_id = 0;
                cmd_result.result_code = APB_PROXY_RESULT_NO_CARRIER;
                cmd_result.pdata = NULL;
                cmd_result.length = 0;
                cmd_result.cmd_id = g_apb_data_test_cmd_id;
                /*send final result.*/
                if (apb_proxy_send_at_cmd_result(&cmd_result) == APB_PROXY_STATUS_OK){
                    apb_proxy_log_info("send NO CARRIER OK");
                }else{
                    apb_proxy_log_info("send NO CARRIER ERROR");
                }
            }else if (received_length > total_length){
                configASSERT(apb_proxy_close_data_mode(g_apb_data_test_conn_id) == APB_PROXY_STATUS_OK);
                g_apb_data_test_conn_id = 0;
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                cmd_result.pdata = NULL;
                cmd_result.length = 0;
                cmd_result.cmd_id = g_apb_data_test_cmd_id;
                /*send final result.*/
                if (apb_proxy_send_at_cmd_result(&cmd_result) != APB_PROXY_STATUS_OK){
                    apb_proxy_log_info("Failed to send result out.");
                }
            }
            break;
        }
        case APB_PROXY_STOP_SEND_USER_DATA: {
            apb_proxy_log_info("stop send user data\r\n");
            break;
        }
        case APB_PROXY_RESUME_SEND_USER_DATA: {
            apb_proxy_log_info("resume send user data\r\n");
            break;
        }
        case APB_PROXY_DATA_MODE_CLOSED_IND: {

            break;
        }
        case APB_PROXY_DATA_MODE_TEMP_DEACTIVEED_IND: {
            apb_proxy_log_info("data mode temp deactived\r\n");
            break;
        }
        case APB_PROXY_DATA_MODE_RESUMED_IND: {
            apb_proxy_log_info("data mode resumed\r\n");
            break;
        }
        default: {
            configASSERT(0);
            break;
        }
    }
}

apb_proxy_status_t apb_proxy_apbdatatest_cmd_handler(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result;
    configASSERT(p_parse_cmd != NULL);
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.pdata = NULL;
    cmd_result.length = 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION: {
            char* p_parameter = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;
            if (p_parse_cmd->parse_pos < p_parse_cmd->string_len){
                total_length = (uint32_t)atoi(p_parameter);
                if (total_length > 0){
                    cmd_result.result_code = APB_PROXY_RESULT_CONNECT;
                    cmd_result.pdata = NULL;
                    cmd_result.length = 0;
                    if (apb_proxy_send_at_cmd_result(&cmd_result) == APB_PROXY_STATUS_OK){
                        g_apb_data_test_cmd_id = p_parse_cmd->cmd_id;
                        g_apb_data_test_conn_id = apb_proxy_create_data_mode(apb_proxy_apbdatatest_data_mode_event_callback, p_parse_cmd->cmd_id);
                        if (g_apb_data_test_conn_id == 0){
                            apb_proxy_log_error("apb data mode test: failed to go to data mode");
                            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                            apb_proxy_send_at_cmd_result(&cmd_result);
                        }
                        received_length = 0;
                    }else{
                        cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                        apb_proxy_send_at_cmd_result(&cmd_result);
                    }
                }else{
                    /* Send "ERROR" out. */
                    if (apb_proxy_current_channel_data_mode_actived(g_apb_data_test_conn_id) == true) {
                        cmd_result.result_code = APB_PROXY_RESULT_OK;
                        apb_proxy_send_at_cmd_result(&cmd_result);
                        apb_proxy_close_data_mode(g_apb_data_test_conn_id);
                        cmd_result.result_code = APB_PROXY_RESULT_NO_CARRIER;
                        apb_proxy_send_at_cmd_result(&cmd_result);
                    } else {
                        apb_proxy_send_at_cmd_result(&cmd_result);
                    }
                }
            }else{
                apb_proxy_send_at_cmd_result(&cmd_result);
            }
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            apb_proxy_send_at_cmd_result(&cmd_result);
            break;
        }
        case APB_PROXY_CMD_MODE_READ:
        case APB_PROXY_CMD_MODE_ACTIVE: {
            apb_proxy_log_info("resume data mode by AT cmd \r\n");
            apb_proxy_resume_data_mode(g_apb_data_test_conn_id);
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID:
        default: {
            /* ERROR is outputted. */
            apb_proxy_send_at_cmd_result(&cmd_result);
            break;
        }
    }
    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_apbdataclose_cmd_handler(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result;
    configASSERT(p_parse_cmd != NULL);
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.pdata = NULL;
    cmd_result.length = 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION: {
            if (apb_proxy_current_channel_data_mode_actived(g_apb_data_test_conn_id) == false) {
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                apb_proxy_send_at_cmd_result(&cmd_result);
                return APB_PROXY_STATUS_OK;
            }
            else {
                cmd_result.result_code = APB_PROXY_RESULT_OK;
                apb_proxy_send_at_cmd_result(&cmd_result);
            }
            if (apb_proxy_close_data_mode(g_apb_data_test_conn_id) == APB_PROXY_STATUS_OK) {
                apb_proxy_log_info("close data mode\r\n");
                g_apb_data_test_conn_id = 0;
                cmd_result.result_code = APB_PROXY_RESULT_NO_CARRIER;
                cmd_result.pdata = NULL;
                cmd_result.length = 0;
                cmd_result.cmd_id = g_apb_data_test_cmd_id;
                /*send final result.*/
                if (apb_proxy_send_at_cmd_result(&cmd_result) == APB_PROXY_STATUS_OK){
                    apb_proxy_log_info("send NO CARRIER OK");
                }else{
                    apb_proxy_log_info("send NO CARRIER ERROR");
                }
            }
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            apb_proxy_send_at_cmd_result(&cmd_result);
            break;
        }
        case APB_PROXY_CMD_MODE_READ:
        case APB_PROXY_CMD_MODE_ACTIVE:
        case APB_PROXY_CMD_MODE_INVALID:
        default: {
            /* ERROR is outputted. */
            apb_proxy_send_at_cmd_result(&cmd_result);
            break;
        }
    }
    return APB_PROXY_STATUS_OK;
}
