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

#include "ril_general_types.h"

#include "ril.h"
#include "ril_internal_use.h"
#include "ril_task.h"
#include "ril_cmds_def.h"
#include "ril_utils.h"
#include "ril_log.h"
#include "ril_cmds_common.h"
#include "ril_channel_config.h"
#include "ril_general_setting.h"
#include "ril_utils.h"

const char *active_mode_format = "%s%s\r\n";
const char *read_mode_format = "%s%s?\r\n";
const char *test_mode_format = "%s%s=?\r\n";

void remove_redundant_comma_space(char *buf)
{
    int32_t str_len;
    int32_t pos = -1;

    /* find the ending of cmd string */
    str_len = strlen(buf);
    if (str_len > 0 ) {
        pos = str_len - 1;
    }
    while ((buf[pos] == ',' || buf[pos] == ' ') && pos >= 0) {
        //RIL_LOGI("remove redundant char\r\n");
        buf[pos--] = '\0';
    }
}


ril_status_t ril_request_send(
    ril_request_mode_t mode,
    ril_cmd_id_t cmd_id,
    ril_param_node_t *param,
    void *callback,
    void *user_data)
{
    ril_request_info_t *info = ril_mem_malloc(sizeof(ril_request_info_t));
    if (info == NULL) {
        return RIL_STATUS_FAIL;
    }

    info->mode = mode;
    info->request_id = cmd_id;
    info->request_param = (ril_param_node_t *) param;
    info->rsp_callback = callback;
    info->user_data = user_data;
    info->channel_id = -1;

    ril_send_message(RIL_MSG_REQUEST, info, sizeof(ril_request_info_t));
    return RIL_STATUS_SUCCESS;
}


ril_status_t ril_request_send_via_channel(
    ril_request_mode_t mode,
    ril_cmd_id_t cmd_id,
    ril_param_node_t *param,
    void *callback,
    void *user_data,
    int32_t channel_id)
{
    ril_request_info_t *info = ril_mem_malloc(sizeof(ril_request_info_t));
    if (info == NULL) {
        return RIL_STATUS_FAIL;
    }

    info->mode = mode;
    info->request_id = cmd_id;
    info->request_param = (ril_param_node_t *) param;
    info->rsp_callback = callback;
    info->user_data = user_data;
    info->channel_id = channel_id;
    ril_send_message(RIL_MSG_REQUEST, info, sizeof(ril_request_info_t));
    return RIL_STATUS_SUCCESS;
}


ril_status_t ril_request_send_via_channel_prior(
    ril_request_mode_t mode,
    ril_cmd_id_t cmd_id,
    ril_param_node_t *param,
    void *callback,
    void *user_data,
    int32_t channel_id)
{
    ril_request_info_t *info = ril_mem_malloc(sizeof(ril_request_info_t));
    if (info == NULL) {
        return RIL_STATUS_FAIL;
    }

    info->mode = mode;
    info->request_id = cmd_id;
    info->request_param = (ril_param_node_t *) param;
    info->rsp_callback = callback;
    info->user_data = user_data;
    info->channel_id = channel_id;
    ril_send_message_int(RIL_MSG_REQUEST, info, sizeof(ril_request_info_t));
    return RIL_STATUS_SUCCESS;
}


/**
 * @brief    add a integer parameter to list.
 * @param[in] list_head    point to the head of parameter list.
 * @param[in] param    integer value.
 * @param[in] can_be_empty    integer value. 0 means param is optional but can not empty if the param exist. 1 means param is optional and can be empty. 2 param must be there and param should be valid.
 * @return    0 mean success, -1 mean failure, -2 means ending
 */
int32_t ril_param_list_add_int_optional(const ril_param_node_t *list_head, int32_t param, int32_t can_be_empty)
{
    int32_t ret = 0;
    if (can_be_empty == 2) {
        if (param == RIL_OMITTED_INTEGER_PARAM || ril_param_list_add_int(list_head, param) < 0) {
            ret = -1;
        }
    } else if (param != RIL_OMITTED_INTEGER_PARAM) {
        if (ril_param_list_add_int(list_head, param) < 0) {
            ret = -1;
        }
    } else {
        if (can_be_empty == 1) {
            if (ril_param_list_add_void(list_head) < 0) {
                ret = -1;
            }
        } else {
            ret = -2;
        }
    }
    return ret;
}


/**
 * @brief    add a integer parameter to list.
 * @param[in] list_head    point to the head of parameter list.
 * @param[in] param    integer value.
 * @param[in] can_be_empty    integer value. 0 means param is optional but can not empty if the param exist. 1 means param is optional and can be empty. 2 param must be there and param should be valid.
 * @return    0 mean success, -1 mean failure, -2 means ending
 */
int32_t ril_param_list_add_hexint_optional(const ril_param_node_t *list_head, int32_t param, int32_t can_be_empty)
{
    int32_t ret = 0;
    if (can_be_empty == 2) {
        if (param == RIL_OMITTED_INTEGER_PARAM || ril_param_list_add_hexint(list_head, param) < 0) {
            ret = -1;
        }
    } else if (param != RIL_OMITTED_INTEGER_PARAM) {
        if (ril_param_list_add_hexint(list_head, param) < 0) {
            ret = -1;
        }
    } else {
        if (can_be_empty == 1) {
            if (ril_param_list_add_void(list_head) < 0) {
                ret = -1;
            }
        } else {
            ret = -2;
        }
    }
    return ret;
}


/**
 * @brief    add a string parameter to list.
 * @param[in] list_head    point to the head of parameter list.
 * @param[in] param    point to the string.
 * @return    0 mean success, -1 mean failure, -2 means ending
 */
int32_t ril_param_list_add_string_optional(const ril_param_node_t *list_head, char *param, int32_t can_be_empty)
{
    int32_t ret = 0;
    if (can_be_empty == 2) {
        if (param == RIL_OMITTED_STRING_PARAM || ril_param_list_add_string(list_head, param, true) < 0) {
            ret = -1;
        }
    } else if (param != RIL_OMITTED_STRING_PARAM) {
        if (ril_param_list_add_string(list_head, param, true) < 0) {
            ret = -1;
        }
    } else {
        if (can_be_empty == 1) {
            if (ril_param_list_add_void(list_head) < 0) {
                ret = -1;
            }
        } else {
            ret = -2;
        }
    }
    return ret;
}


/**
 * @brief    add a string parameter to list.
 * @param[in] list_head    point to the head of parameter list.
 * @param[in] param    point to the string.
 * @return    0 mean success, -1 mean failure, -2 means ending
 */
int32_t ril_param_list_add_digital_string_optional(const ril_param_node_t *list_head, char *param, int32_t can_be_empty)
{
    int32_t ret = 0;
    if (can_be_empty == 2) {
        if (param == RIL_OMITTED_STRING_PARAM || ril_param_list_add_string(list_head, param, false) < 0) {
            ret = -1;
        }
    } else if (param != RIL_OMITTED_STRING_PARAM) {
        if (ril_param_list_add_string(list_head, param, false) < 0) {
            ret = -1;
        }
    } else {
        if (can_be_empty == 1) {
            if (ril_param_list_add_void(list_head) < 0) {
                ret = -1;
            }
        } else {
            ret = -2;
        }
    }
    return ret;
}



int32_t ril_cmd_send_common_hdlr(uint32_t channel_id, char *buf, ril_request_info_t *info)
{
    ril_cmd_item_t *item = get_at_cmd_item((ril_cmd_id_t)info->request_id);
    ril_param_node_t *list_head = (ril_param_node_t *)info->request_param;
    ril_param_desc_t *param;
    int32_t ret;
    bool is_1st_param = true;

    switch (info->mode) {
        case RIL_EXECUTE_MODE:
            /* consturct command head */
            snprintf(buf, RIL_BUF_SIZE - 1, "%s%s=", AT_PREFIX, item->cmd_head);

            /* append  params */
            do {
                ret = ril_param_list_get(&list_head, &param);
                if (ret >= 0) {
                    //RIL_LOGI("get param, data type: %d\r\n", param->data_type);
                    if (param->data_type == RIL_PARAM_TYPE_INTEGER) {
                        //RIL_LOGI("append int param: %d\r\n", param->data.val);
                        ril_cmd_string_param_int_append(buf, param->data.val, !is_1st_param);
                    } else if (param->data_type == RIL_PARAM_TYPE_STRING) {
                        //RIL_LOGI("append string param: %s\r\n", param->data.str);
                        ril_cmd_string_param_string_append(buf, param->data.str, !is_1st_param);
                    } else if (param->data_type == RIL_PARAM_TYPE_HEXINTEGER) {
                        ril_cmd_string_param_hexint_append(buf, param->data.val, !is_1st_param);
                    } else if (param->data_type == RIL_PARAM_TYPE_DIGITAL_STRING) {
                        ril_cmd_string_param_digital_string_append(buf, param->data.str, !is_1st_param);
                    } else if (param->data_type == RIL_PARAM_TYPE_VOID) {
                        //RIL_LOGI("append void param\r\n");
                        ril_cmd_string_param_void_append(buf, !is_1st_param);
                    } else {
                        configASSERT(0);
                    }
                    is_1st_param = false;
                }
            } while (ret >= 0);
            remove_redundant_comma_space(buf);
            strcat(buf, "\r\n");
            break;
        case RIL_ACTIVE_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, active_mode_format, AT_PREFIX, item->cmd_head);
            break;
        case RIL_READ_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, read_mode_format, AT_PREFIX, item->cmd_head);
            break;
        case RIL_TEST_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, test_mode_format, AT_PREFIX, item->cmd_head);
            break;
        default:
            configASSERT(0);
    }

    //RIL_LOGI("[Normal] ril_cmd_send_common_hdlr:%d:[%s]\r\n", strlen(buf), buf);

    return strlen(buf);

}


int32_t ril_response_common_no_parameter(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    int32_t ret = -1;
    void *response = NULL;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;

    cur_pos = cmd_buf;
    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            /* no information response */
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }

    return 0;
}


int32_t ril_urc_dummy_hdlr(ril_urc_id_t event_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    /* do nothing */
    //RIL_LOGI("%s enter\r\n", __FUNCTION__);

    ril_notify_event(event_id, cmd_buf, cmd_buf_len);
    return 0;
}


/* internal use unsol reporting */
int32_t ril_urc_cpin_indication(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return 0;
}


ril_status_t ril_request_custom_command(char *cmd_string,
        ril_custom_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    ril_cmd_id_t cmd_id;
    char cmd_head_buf[20];
    char *cmd_head;
    uint32_t cmd_head_len;
    ril_request_mode_t mode;
    bool urc_support = false;
    ril_cmd_item_t *cmd_item;   

    memset(cmd_head_buf, 0x0, 20);
    cmd_head = NULL;
    cmd_head_len = 0;
    do {
        if ((ret = at_tok_get_at_head(cmd_string, &cmd_head, &cmd_head_len, &mode)) < 0) {
            break;
        }

        if (cmd_head_len >= 20) {
            break;
        }

        cmd_id = find_at_cmd_id(cmd_head, cmd_head_len);
        cmd_item = get_at_cmd_item(cmd_id);
        if (cmd_item != NULL) {
            urc_support = cmd_item->cmd_type == RIL_CMD_TYPE_URC ? true: false;
        }
        
        if ((ret = ril_param_list_create(&list_head)) < 0) {
            break;
        }

        memcpy(cmd_head_buf, cmd_head, cmd_head_len);
        cmd_head_buf[cmd_head_len] = '\0';
        
        if ((ret = ril_param_list_add_int_optional(list_head, cmd_id, 2)) < 0) {
            break;
        }

        if ((ret = ril_param_list_add_string_optional(list_head, cmd_head_buf, 2)) < 0) {
            break;
        }

        if ((ret = ril_param_list_add_string_optional(list_head, cmd_string, 2)) < 0) {
            break;
        }
    } while (0);

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        cmd_id = urc_support ? RIL_CMD_ID_CUSTOM_CMD_URC : RIL_CMD_ID_CUSTOM_CMD;
        return ril_request_send(mode, cmd_id, list_head, (void *)callback,  user_data);
    }
}


int32_t ril_cmd_send_custom_command(uint32_t channel_id, char *buf, ril_request_info_t *info)
{
    //ril_cmd_item_t *item = get_at_cmd_item((ril_cmd_id_t)info->request_id);
    ril_param_node_t *list_head = (ril_param_node_t *)info->request_param;
    ril_param_desc_t *param;
    uint32_t cmd_head_len;
    ril_cmd_id_t cmd_id;
    ril_cmd_item_t *cmd_item;
    char *cmd_head;
    int32_t ret;
    
    /* 1st param is cmd id */
    if ((ret = ril_param_list_get(&list_head, &param)) < 0) {
        return -1;
    }
    cmd_id = (ril_cmd_id_t)param->data.val;
    if (cmd_id > 0 && cmd_id < RIL_CMD_ID_INVALID) {
        cmd_item = get_at_cmd_item(cmd_id);
        /* existing command, apply default setting. */
        ril_cntx.custom_cmd_info_mask = cmd_item->info_rsp_mask;
        ril_cntx.custom_cmd_is_multiline = cmd_item->is_multiline;
    } else {
        /* real customized command, apply the widest parsing pattern. */
        ril_cntx.custom_cmd_info_mask = RIL_INFO_RSP_READ_MODE | RIL_INFO_RSP_EXECUTE_MODE | RIL_INFO_RSP_ACTIVE_MODE;
        ril_cntx.custom_cmd_is_multiline = true;
    }

    /* 2nd param is cmd head */
    if ((ret = ril_param_list_get(&list_head, &param)) < 0) {
        return -1;
    }
    cmd_head_len = strlen(param->data.str);
    cmd_head = (char *)ril_mem_malloc(cmd_head_len + 1);
    memcpy(cmd_head, param->data.str, cmd_head_len);
    cmd_head[cmd_head_len] = '\0';
    ril_cntx.custom_cmd_head = cmd_head;

    /* 3rd param is cmd string */
    if ((ret = ril_param_list_get(&list_head, &param)) < 0) {
        return -1;
    }
    strncpy(buf, param->data.str, RIL_BUF_SIZE - 1);


    strcat(buf, "\r\n");

    return strlen(buf);
}


int32_t ril_response_custom_command(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    ril_channel_cntx_t *channel_p;
    ril_custom_cmd_response_callback_t rsp_cb;

    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_custom_cmd_response_callback_t)channel_p->usr_rsp_cb;

    /* free custom command context */
    if (ril_cntx.custom_cmd_head != NULL) {
        ril_mem_free(ril_cntx.custom_cmd_head);
    }
    ril_cntx.custom_cmd_info_mask = 0;
    ril_cntx.custom_cmd_is_multiline = false;
    
    if (rsp_cb) {
        (rsp_cb)(cmd_buf, cmd_buf_len, channel_p->usr_data);
    }
    return 0;
}


#if defined(__RIL_UT_TEST_CASES__)
int32_t ril_response_ut_callback_custom_command(char *rsp_str, uint32_t rsp_str_len, void *user_data)
{
    RIL_LOGI("%s enter\r\n", __FUNCTION__);
    RIL_LOGDUMPSTR("rsp_str: %s", rsp_str_len, rsp_str);    
    RIL_LOGI("user data: 0x%x\r\n", (unsigned int)user_data);
    return 0;
}
#endif

