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


#include "stdlib.h"

#include "ril.h"
#include "ril_internal_use.h"
#include "ril_log.h"
#include "ril_utils.h"
#include "ril_cmds_def.h"
#include "ril_cmds_27007.h"
#include "ril_cmds_proprietary.h"

#include "apb_proxy.h"
#include "apb_proxy_ril_ut_cmd.h"
#include "FreeRTOS.h"
#include "timers.h"

extern int32_t ril_ut_cmd_send_dispatch(char *param_array[], uint32_t param_num);
extern int32_t ril_ut_cmd_response_dispatch(int32_t channel_id, char *cmd_buf);
extern int32_t ril_ut_cmd_urc_dispatch(char *cmd_buf);
extern int32_t ril_urc_dummy_callback(ril_urc_id_t event_id, void *param, uint32_t param_len);
extern int32_t ril_urc_ut_callback(ril_urc_id_t event_id, void *param, uint32_t param_len);


static TimerHandle_t ril_ut_channel_test_timer = NULL;

static char *next_tok_ext(char **p_cur)
{
    char *ret = NULL;

    if (*p_cur == NULL) {
        ret = NULL;
    } else if (**p_cur == '"') {
        ret = *p_cur;
        (*p_cur)++;
        *p_cur = strchr(*p_cur, '"');
        strsep(p_cur, ",");
    } else {
        ret = strsep(p_cur, ",");
    }

    return ret;
}


static void ril_ut_channel_test_timer_handle( TimerHandle_t tmr )
{
    if (ril_ut_channel_test_timer && xTimerIsTimerActive(ril_ut_channel_test_timer) != pdFALSE) {
        xTimerStop(ril_ut_channel_test_timer, 0);
    }

    /* PD group */
    ril_request_ps_attach_or_detach(RIL_READ_MODE, RIL_OMITTED_INTEGER_PARAM, ril_response_ut_callback_27007, NULL);
    ril_request_pdp_context_activate_or_deactivate(RIL_READ_MODE, NULL, ril_response_ut_callback_27007, NULL);
    /* MM group */
    //ril_request_plmn_selection(RIL_EXECUTE_MODE, 3, 2, RIL_OMITTED_STRING_PARAM, RIL_OMITTED_INTEGER_PARAM, ril_response_ut_callback_27007, NULL);
    ril_request_plmn_selection(RIL_READ_MODE, RIL_OMITTED_INTEGER_PARAM, RIL_OMITTED_INTEGER_PARAM, RIL_OMITTED_STRING_PARAM, RIL_OMITTED_INTEGER_PARAM, ril_response_ut_callback_27007, NULL);
    ril_request_preferred_plmn_list(RIL_READ_MODE, NULL, ril_response_ut_callback_27007, NULL);
    //ril_request_query_network_state(RIL_READ_MODE, RIL_OMITTED_INTEGER_PARAM, ril_response_ut_callback_proprietary, NULL);
    /* n/a group */
    ril_request_eps_network_registration_status(RIL_READ_MODE, RIL_OMITTED_INTEGER_PARAM, ril_response_ut_callback_27007, NULL);
    ril_request_manufacturer_identification(RIL_READ_MODE, ril_response_ut_callback_27007, NULL);

    xTimerStart(ril_ut_channel_test_timer, 0);
}


#if defined(__RIL_UT_TEST_CASES__)
apb_proxy_status_t apb_proxy_hdlr_ril_ut_send(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    apb_proxy_at_cmd_result_t response = {0};
    char *cmd_string = NULL;
    char *param_pArray[20] = {0};
    uint32_t param_num = 0;
    int32_t ret_val = 0;
    int32_t i = 0;

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+TESTCMD=?
            response.result_code = APB_PROXY_RESULT_OK;
            break;

        case APB_PROXY_CMD_MODE_READ:    // rec: AT+TESTCMD?
            response.result_code = APB_PROXY_RESULT_OK;
            break;

        case APB_PROXY_CMD_MODE_ACTIVE:  // rec: AT+TESTCMD
            // assume the active mode is invalid and we will return "ERROR"
            response.result_code = APB_PROXY_RESULT_OK;
            break;

        case APB_PROXY_CMD_MODE_EXECUTION: // rec: AT+TESTCMD=<p1>  the handler need to parse the parameters
            //parsing the parameter

            do {
                cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    RIL_LOGE("Invalid parameter\r\n");
                    ret_val = -1;
                    break;
                }
                cmd_string++;
                RIL_LOGI("cmd string: %s\r\n", cmd_string);

            #if 0 // strtok cannot solve this case: "17/10/21,15:30:00+08", the comma between quotation mark will be considered as delimiter, but it's fault.
                param = strtok(cmd_string, ",\r\n");

                while (param != NULL && i < 20) {
                    strcpy(p, param);
                    param_pArray[i++] = p;
                    p = p + strlen(p) + 1;

                    param = strtok(NULL, ",\r\n");
                    param_num++;
                }
            #endif
                strtok(cmd_string, "\r\n");
                while (cmd_string != NULL && i < 20) {
                    param_pArray[i++] = next_tok_ext(&cmd_string);
                    param_num++;
                }

                /* dump cmd string of parameter */
                for (i = 0; i < param_num; i++) {
                    RIL_LOGI("param_pArray[%d]: %s", (int)i, param_pArray[i]);
                }

                ril_ut_cmd_send_dispatch(param_pArray, param_num);
            } while (0);

            if (ret_val == 0) {
                response.result_code = APB_PROXY_RESULT_OK; // ATCI will help append "OK" at the end of resonse buffer
            } else {
                RIL_LOGE("will return ERROR to command sender\r\n");
                response.result_code = APB_PROXY_RESULT_ERROR; // ATCI will help append "ERROR" at the end of resonse buffer
            }
            break;

        default :
            response.result_code = APB_PROXY_RESULT_OK;
            break;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);
    return APB_PROXY_STATUS_OK;
}


apb_proxy_status_t apb_proxy_hdlr_ril_ut_response(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    apb_proxy_at_cmd_result_t response = {0};
    char *param = NULL;

    char *cmd_string = NULL;
    int32_t channel_id = -1;
    int32_t ret_val = 0;

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+TESTCMD=?
            response.result_code = APB_PROXY_RESULT_OK;
            break;

        case APB_PROXY_CMD_MODE_READ:    // rec: AT+TESTCMD?
            response.result_code = APB_PROXY_RESULT_OK;
            break;

        case APB_PROXY_CMD_MODE_ACTIVE:  // rec: AT+TESTCMD
            // assume the active mode is invalid and we will return "ERROR"
            response.result_code = APB_PROXY_RESULT_OK;
            break;

        case APB_PROXY_CMD_MODE_EXECUTION: // rec: AT+TESTCMD=<p1>  the handler need to parse the parameters
            //parsing the parameter

            do {
                cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    RIL_LOGE("Invalid parameter\r\n");
                    ret_val = -1;
                    break;
                }
                cmd_string++;
                RIL_LOGI("cmd string: %s\r\n", cmd_string);
                param = strsep(&cmd_string, ",");

                if (param != NULL) {
                    channel_id = atoi(param);
                }

                /* dump cmd string of parameter */
                RIL_LOGI("channel id: %ld, cmd buffer: %s", channel_id, cmd_string);

                ril_ut_cmd_response_dispatch(channel_id, cmd_string);
            } while (0);


            if (ret_val == 0) {
                response.result_code = APB_PROXY_RESULT_OK; // ATCI will help append "OK" at the end of resonse buffer
            } else {
                response.result_code = APB_PROXY_RESULT_ERROR; // ATCI will help append "ERROR" at the end of resonse buffer
            }
            param = NULL;
            break;

        default :
            response.result_code = APB_PROXY_RESULT_OK;
            break;
    }
    
    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);
    return APB_PROXY_STATUS_OK;
}



apb_proxy_status_t apb_proxy_hdlr_ril_ut_urc(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    apb_proxy_at_cmd_result_t response = {0};

    char *cmd_string = NULL;
    int32_t ret_val = 0;

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+TESTCMD=?
            response.result_code = APB_PROXY_RESULT_OK;
            break;

        case APB_PROXY_CMD_MODE_READ:    // rec: AT+TESTCMD?
            response.result_code = APB_PROXY_RESULT_OK;
            break;

        case APB_PROXY_CMD_MODE_ACTIVE:  // rec: AT+TESTCMD
            // assume the active mode is invalid and we will return "ERROR"
            response.result_code = APB_PROXY_RESULT_OK;
            break;

        case APB_PROXY_CMD_MODE_EXECUTION: // rec: AT+TESTCMD=<p1>  the handler need to parse the parameters
            //parsing the parameter

            do {
                cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    RIL_LOGE("Invalid parameter\r\n");
                    ret_val = -1;
                    break;
                }
                cmd_string++;

                /* dump cmd string of parameter */
                RIL_LOGI("cmd buffer: %s", cmd_string);

                ril_ut_cmd_urc_dispatch(cmd_string);
            } while (0);


            if (ret_val == 0) {
                response.result_code = APB_PROXY_RESULT_OK;
            } else {
                response.result_code = APB_PROXY_RESULT_ERROR;
            }
            break;

        default :
            response.result_code = APB_PROXY_RESULT_OK;
            break;
    }
    
    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);
    return APB_PROXY_STATUS_OK;
}


apb_proxy_status_t apb_proxy_hdlr_ril_ut_test_func(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    apb_proxy_at_cmd_result_t response = {0};
    
    ril_register_event_callback(RIL_ALL, ril_urc_ut_callback);

    response.result_code = APB_PROXY_RESULT_OK;
    
    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);
    return APB_PROXY_STATUS_OK;
}


apb_proxy_status_t apb_proxy_hdlr_ril_ut_send_custom_command(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    apb_proxy_at_cmd_result_t response = {0};

    char *cmd_string = NULL;
    char **p_cur = NULL;
    int32_t ret_val = 0;    
    char *param_pArray[4] = {0};
    int32_t i;

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+TESTCMD=?
            response.result_code = APB_PROXY_RESULT_OK;
            break;

        case APB_PROXY_CMD_MODE_READ:    // rec: AT+TESTCMD?
            response.result_code = APB_PROXY_RESULT_OK;
            break;

        case APB_PROXY_CMD_MODE_ACTIVE:  // rec: AT+TESTCMD
            // assume the active mode is invalid and we will return "ERROR"
            response.result_code = APB_PROXY_RESULT_OK;
            break;

        case APB_PROXY_CMD_MODE_EXECUTION: // rec: AT+TESTCMD=<p1>  the handler need to parse the parameters
            //parsing the parameter

            do {
                cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    RIL_LOGE("Invalid parameter\r\n");
                    ret_val = -1;
                    break;
                }
                cmd_string++;

                /* dump cmd string of parameter */
                RIL_LOGI("cmd buffer: %s", cmd_string);
                p_cur = &cmd_string;
                param_pArray[0] = "+CUSTOMCMD";
                param_pArray[1] = "0";

                skip_white_space(p_cur);
                if (**p_cur == '"') {
                    param_pArray[2] = *p_cur;
                    (*p_cur)++;
                    (*p_cur) = strchr((*p_cur), '"');
                    if (*p_cur != NULL && (*((*p_cur) + 1)) == ',') {
                        (*p_cur)++;
                        **p_cur = '\0';
                        (*p_cur)++;
                    } else {
                        ret_val = -1;
                        break;
                    }
                } else {
                    ret_val = -1;
                    break;
                }
                
                if ((ret_val = at_tok_nextstr(p_cur, &param_pArray[3])) < 0) {
                    RIL_LOGE("Failed to get param3\r\n");
                    break;
                }

                /* dump cmd string of parameter */
                for (i = 0; i < 4; i++) {
                    RIL_LOGI("param_pArray[%ld]: %s", i, param_pArray[i]);
                }
                ril_ut_cmd_send_dispatch(param_pArray, 4);
            } while (0);


            if (ret_val == 0) {
                response.result_code = APB_PROXY_RESULT_OK;
            } else {
                response.result_code = APB_PROXY_RESULT_ERROR;
            }
            break;

        default :
            response.result_code = APB_PROXY_RESULT_OK;
            break;
    }
    
    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);
    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_ril_ut_channel_test(apb_proxy_parse_cmd_param_t *parse_cmd)
{   
    apb_proxy_at_cmd_result_t response = {0};
    char *p_cur, *p_last;
    char *param[2];
    const char *delim = ",\r\n";
    
    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+TESTCMD=?
        case APB_PROXY_CMD_MODE_READ:    // rec: AT+TESTCMD?
        case APB_PROXY_CMD_MODE_ACTIVE:  // rec: AT+TESTCMD
             response.result_code = APB_PROXY_RESULT_OK;
            break;
        case APB_PROXY_CMD_MODE_EXECUTION:
            if (ril_ut_channel_test_timer == NULL) {
                ril_ut_channel_test_timer = xTimerCreate( "ril_test_timer",
                                        (1000/portTICK_PERIOD_MS), /* interval 1 second. */
                                        pdFALSE,
                                        NULL,
                                        ril_ut_channel_test_timer_handle);
            }
            p_cur = parse_cmd->string_ptr + parse_cmd->name_len;
            if (*p_cur == '=') {
                p_cur++;
            }
            param[0] = strtok_r(p_cur, delim, &p_last);
            //param[1] = strtok_r(NULL, delim, &p_last);           
            //repeat = (int32_t)atoi(param[1]);
            if (!strcmp(param[0], "START")) {
                xTimerStart(ril_ut_channel_test_timer, 0);
            } else if (!strcmp(param[0], "STOP")) {
                xTimerStop(ril_ut_channel_test_timer, 0);
            } else {
                response.result_code = APB_PROXY_RESULT_ERROR;
                RIL_LOGW("invalid parameter\r\n");
                break;
            }
            response.result_code = APB_PROXY_RESULT_OK;
            break;
        default:
            response.result_code = APB_PROXY_RESULT_OK;
            break;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);
    return APB_PROXY_STATUS_OK;
}
#else
apb_proxy_status_t apb_proxy_hdlr_ril_ut_send(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    apb_proxy_at_cmd_result_t response = {0};
    response.result_code = APB_PROXY_RESULT_ERROR;
    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);
    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_ril_ut_response(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    apb_proxy_at_cmd_result_t response = {0};
    response.result_code = APB_PROXY_RESULT_ERROR;
    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);
    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_ril_ut_urc(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    apb_proxy_at_cmd_result_t response = {0};
    response.result_code = APB_PROXY_RESULT_ERROR;
    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);
    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_ril_ut_test_func(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    apb_proxy_at_cmd_result_t response = {0};
    response.result_code = APB_PROXY_RESULT_ERROR;
    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);
    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_ril_ut_send_custom_command(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    apb_proxy_at_cmd_result_t response = {0};
    response.result_code = APB_PROXY_RESULT_ERROR;
    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);
    return APB_PROXY_STATUS_OK;
}
apb_proxy_status_t apb_proxy_hdlr_ril_ut_channel_test(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    apb_proxy_at_cmd_result_t response = {0};
    response.result_code = APB_PROXY_RESULT_ERROR;
    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);
    return APB_PROXY_STATUS_OK;
}
#endif

