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
#include "apb_proxy_fota_cmd.h"
#include "FreeRTOS.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "syslog.h"

log_create_module(fota_cmd, PRINT_LEVEL_INFO);
/******************************************************************************************
 *                               Macro's Definitions                                      *
 ******************************************************************************************/
#define FOTA_CMD_ERR(fmt,arg...)   LOG_E(fota_cmd, "[FOTA_CMD]: "fmt,##arg)
#define FOTA_CMD_WARN(fmt,arg...)  LOG_W(fota_cmd, "[FOTA_CMD]: "fmt,##arg)
#define FOTA_CMD_DBG(fmt,arg...)   LOG_I(fota_cmd,"[FOTA_CMD]: "fmt,##arg)

#define FOTA_CMD_PARAM_MAX_LEN               12
#define FOTA_DL_PARAM_MAX_LEN                512
#define FOTA_DL_PARAM_MIN_LEN                12 /* The parameter format:1,"http://   "*/
#define FOTA_DL_CM4_PACKAGE_TYPE             1  /* The download type is CM4 FOTA package. */
#define FOTA_DL_GNSS_PACKAGE_TYPE            2  /* The download type is GNSS FOTA package. */
#define FOTA_MAIN_BIN_TYPE                   1
#define FOTA_GNSS_BIN_TYPE                   2
/******************************************************************************************
 *                               Local Variants Definitions                               *
 *****************************************************************************************/
static char fota_help[] = "AT+FOTA: trigger FOTA binary update flag.\r\nAT+FOTA=\"reboot\": reboot the device\r\nAT+FOTA?: read FOTA result.";
static char fota_dl_help[] = "AT+FOTADL=1,\"MT2625 FOTA package URL\"\r\nAT+FOTADL=2,\"GNSS package URL\"";
static apb_proxy_cmd_id_t g_fota_cmd_id = APB_PROXY_INVALID_CMD_ID;
/******************************************************************************************
 *                               Local Function Definitions                               *
 *****************************************************************************************/
static void fota_event_info(uint32_t result_code);
static void fota_progress_info(uint32_t precent);
/******************************************************************************************
 *                               Public Function Impelmentation                           *
 *****************************************************************************************/
apb_proxy_status_t apb_proxy_fota_cmd_hdlr(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result;
    fota_result_t fota_result_code;
    configASSERT(p_parse_cmd != NULL);
    char error_info[32] = {0};
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.pdata = NULL;
    cmd_result.length = 0;
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            fota_result_t fota_result_code = fota_read_upgrade_result();
            if (fota_result_code != FOTA_OK){
                snprintf(error_info, sizeof(error_info), "Error: %ld", fota_result_code);
                cmd_result.pdata = error_info;
                cmd_result.length = strlen((const char*)error_info);
            }else{
                cmd_result.result_code = APB_PROXY_RESULT_OK;
            }
            fota_clear_history();
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            fota_result_t fota_result;
            fota_result = fota_trigger_upgrade(FOTA_ALL_BIN);
            FOTA_CMD_DBG("fota trigger result = %d\r\n", fota_result);
            if (fota_result == FOTA_OK){
                cmd_result.result_code = APB_PROXY_RESULT_OK;
            }else if (fota_result == FOTA_BUSY){
                cmd_result.result_code = APB_PROXY_RESULT_BUSY;
            }else{
                snprintf(error_info, sizeof(error_info), "Error: %d", fota_result);
                cmd_result.pdata = error_info;
                cmd_result.length = strlen(error_info);
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            }
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            uint32_t index = 0;
            char* fota_buffer = (char*)pvPortMalloc(FOTA_CMD_PARAM_MAX_LEN);
            char* p_parameter = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;
            bool reboot = false;
            uint32_t fota_type = 0;
            if (fota_buffer == NULL){
                break;
            }

            memset(fota_buffer, 0, FOTA_CMD_PARAM_MAX_LEN);
            if ((p_parse_cmd->string_len - p_parse_cmd->parse_pos) <= (FOTA_CMD_PARAM_MAX_LEN - 1)){
                if (*p_parameter == '\"'){
                    for(index = 1; (index < (FOTA_CMD_PARAM_MAX_LEN - 1)) && (index < (p_parse_cmd->string_len - p_parse_cmd->parse_pos));
                        index ++){
                        if (p_parameter[index] == '\"'){
                                break;
                        }
                        fota_buffer[index - 1] = p_parameter[index];
                    }
                    if (strcmp(fota_buffer, "reboot") == 0){
                        reboot = true;
                        fota_result_code = fota_trigger_reboot();
                        if (fota_result_code == FOTA_OK){
                            cmd_result.result_code = APB_PROXY_RESULT_OK;
                        }else if(fota_result_code == FOTA_BUSY){
                            cmd_result.result_code = APB_PROXY_RESULT_BUSY;
                        }else{
                            snprintf(error_info, sizeof(error_info), "Error: %ld", fota_result_code);
                            cmd_result.pdata = error_info;
                            cmd_result.length = strlen((const char*)error_info);
                        }
                    }
                }
            }

            if (fota_buffer != NULL){
                vPortFree(fota_buffer);
                fota_buffer = NULL;
            }
            /*other format*/
            if (reboot == false){
                fota_image_t fota_image;
                fota_result_t fota_result;
                fota_type = atoi(p_parameter);
                FOTA_CMD_DBG("fota_type = %d\r\n", fota_type);
                if (fota_type == FOTA_MAIN_BIN_TYPE){
                    fota_image = FOTA_MAIN_BIN;
                }else if (fota_type == FOTA_GNSS_BIN_TYPE){
                    fota_image = FOTA_GNSS_BIN;
                }else {
                    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                    break;
                }
                fota_result = fota_trigger_upgrade(fota_image);
                FOTA_CMD_DBG("fota trigger result = %d\r\n", fota_result);
                if (fota_result == FOTA_OK){
                    cmd_result.result_code = APB_PROXY_RESULT_OK;
                }else if (fota_result == FOTA_BUSY){
                    cmd_result.result_code = APB_PROXY_RESULT_BUSY;
                }else{
                    snprintf(error_info, sizeof(error_info), "Error: %d", fota_result);
                    cmd_result.pdata = error_info;
                    cmd_result.length = strlen(error_info);
                    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                }

            }
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            cmd_result.pdata = fota_help;
            cmd_result.length = strlen((const char*)fota_help);
            cmd_result.result_code = APB_PROXY_RESULT_OK;
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
    return APB_PROXY_STATUS_OK;
}
apb_proxy_status_t apb_proxy_fota_dl_cmd_hdlr(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result;
    fota_result_t fota_result_code;
    char error_info[32] = {0};
    configASSERT(p_parse_cmd != NULL);
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.pdata = NULL;
    cmd_result.length = 0;
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION: {
            uint32_t index = 0;
            bool is_param_right = false;
            char* p_para_buf = NULL;
            char temp_buf[4] = {0};
            uint32_t package_type = 0;
            uint32_t para_len = p_parse_cmd->string_len - p_parse_cmd->parse_pos;
            char* p_parameter = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;
            fota_image_t image_type;
            bool be_found = false;
            if ((para_len < FOTA_DL_PARAM_MAX_LEN) && (para_len >= FOTA_DL_PARAM_MIN_LEN)){
                if ((*(p_parameter + 1)) == ','){
                    temp_buf[0] = *p_parameter;
                    package_type = atoi(temp_buf);
                    switch(package_type){
                        case FOTA_DL_CM4_PACKAGE_TYPE:{
                            is_param_right = true;
                            image_type = FOTA_MAIN_BIN;
                            break;
                        }
                        case FOTA_DL_GNSS_PACKAGE_TYPE:{
                            is_param_right = true;
                            image_type = FOTA_GNSS_BIN;
                            break;
                        }
                        default:
                            break;
                    }
                }

                if (is_param_right == true){
                    p_para_buf = (char*)pvPortMalloc(para_len + 1);
                    if (p_para_buf == NULL){
                        break;
                    }
                    memset(p_para_buf, 0, para_len + 1);
                    for(index = 0; index < para_len; index ++){
                        if (p_parameter[index] == '\"'){
                            p_parameter = p_parameter + index + 1;
                            para_len = para_len - (index + 1);
                            break;
                        }
                    }
                    for(index = 0; index < para_len; index ++){
                        if (p_parameter[index] == '\"'){
                            be_found = true;
                            p_para_buf[index] = '\0';
                            break;
                        }else{
                            p_para_buf[index] = p_parameter[index];
                        }
                    }
                    if (be_found == true){
                        g_fota_cmd_id = p_parse_cmd->cmd_id;
                        fota_result_code = fota_download_image(image_type, p_para_buf);
                        if (fota_result_code == FOTA_OK){
                            cmd_result.result_code = APB_PROXY_RESULT_OK;
                        }else if(fota_result_code == FOTA_BUSY){
                            cmd_result.result_code = APB_PROXY_RESULT_BUSY;
                        }else{
                            snprintf(error_info, sizeof(error_info), "Error: %ld", fota_result_code);
                            cmd_result.pdata = error_info;
                            cmd_result.length = strlen((const char*)error_info);
                        }
                    }
                    if (p_para_buf != NULL){
                        vPortFree(p_para_buf);
                        p_para_buf = NULL;
                    }
                }
            }
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            cmd_result.pdata = fota_dl_help;
            cmd_result.length = strlen((const char*)fota_dl_help);
            cmd_result.result_code = APB_PROXY_RESULT_OK;
            break;
        }
        case APB_PROXY_CMD_MODE_READ:
        case APB_PROXY_CMD_MODE_ACTIVE:
        case APB_PROXY_CMD_MODE_INVALID:
        default: {
            break;
        }
    }
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}

void apb_proxy_fota_event_ind(fota_msg_event_t event, fota_msg_event_info_t* p_event_info)
{
    apb_proxy_at_cmd_result_t cmd_result;
    char info[32] = {0};
    cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
    cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
    if (p_event_info == NULL){
        return;
    }
    switch(event){
        case FOTA_MSG_DOWNLOAD_PROGRESS_IND:{
            snprintf(info, sizeof(info), "+FOTADL: %lu%%", p_event_info->progress);
            break;
        }
        case FOTA_MSG_DOWNLOAD_RESULT_IND:{
            if (p_event_info->fota_result != FOTA_OK){
                snprintf(info, sizeof(info), "+FOTADL: Error:%d", (uint32_t)(p_event_info->fota_result));
            }else{
                snprintf(info, sizeof(info), "+FOTADL: %s", "OK");
            }
            break;
        }
        case FOTA_MSG_UPGRADE_PROGRESS_IND:{
            snprintf(info, sizeof(info), "+FOTA: %lu%%", p_event_info->progress);
            break;
        }
        case FOTA_MSG_UPGRADE_RESULT_IND:{
            if (p_event_info->fota_result != FOTA_OK){
                snprintf(info, sizeof(info), "+FOTA: Error:%d", (uint32_t)(p_event_info->fota_result));
            }else{
                snprintf(info, sizeof(info), "+FOTA: %s", "OK");
            }
            break;
        }
        default:{
            break;
        }
    }

    cmd_result.pdata = (void*)info;
    cmd_result.length = strlen((const char*)info);
    apb_proxy_send_at_cmd_result(&cmd_result);
}

/******************************************************************************************
 *                               Local Function Impelmentation                            *
 *****************************************************************************************/
static void fota_progress_info(uint32_t precent)
{
    apb_proxy_at_cmd_result_t cmd_result;
    char info[32] = {0};
    cmd_result.cmd_id = g_fota_cmd_id;
    cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
    snprintf(info, sizeof(info), "%lu%%", precent);
    cmd_result.pdata = (void*)info;
    cmd_result.length = strlen((const char*)info);
    apb_proxy_send_at_cmd_result(&cmd_result);
}

static void fota_event_info(uint32_t result_code)
{
    apb_proxy_at_cmd_result_t cmd_result;
    char error_info[32] = {0};
    cmd_result.cmd_id = g_fota_cmd_id;
    cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
    if (g_fota_cmd_id == APB_PROXY_INVALID_CMD_ID){
        if (result_code != 0){
            snprintf(error_info, sizeof(error_info), "+FOTA: Error:%d", result_code);
        }else{
            snprintf(error_info, sizeof(error_info), "+FOTA: %s", "OK");
        }
    }else{
        if (result_code != 0){
            snprintf(error_info, sizeof(error_info), "Error:%d", result_code);
        }else{
            snprintf(error_info, sizeof(error_info), "%s", "OK");
        }
    }

    cmd_result.pdata = (void*)error_info;
    cmd_result.length = strlen((const char*)error_info);
    apb_proxy_send_at_cmd_result(&cmd_result);
}
