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
#include "ril_cmds_def.h"
#include "ril_task.h"
#include "ril_utils.h"
#include "ril_log.h"
#include "ril_cmds_common.h"

#include "ril_cmds_v250.h"
#include "ril_cmds_common.h"


ril_status_t ril_basic_request_send(
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

    ril_send_message(RIL_MSG_REQUEST, info, sizeof(ril_request_info_t));
    return RIL_STATUS_SUCCESS;
}

int32_t ril_cmd_send_basic_hdlr(uint32_t channel_id, char *buf, ril_request_info_t *info)
{
    ril_cmd_item_t *item = get_at_cmd_item((ril_cmd_id_t)info->request_id);
    ril_param_node_t *list_head = (ril_param_node_t *)info->request_param;
    ril_param_desc_t *param;
    int32_t ret;
    bool is_first_param = true;

    /* consturct command head */
    snprintf(buf, RIL_BUF_SIZE - 1, "%s%s", AT_PREFIX, item->cmd_head);

    /* append param */
    do {
        ret = ril_param_list_get(&list_head, &param);
        if (ret >= 0) {
            if (is_first_param) {
                is_first_param = false;
            } else {
                strcat(buf, ",");
            }
            if (param->data_type == RIL_PARAM_TYPE_INTEGER) {
                char str_buf[20]; /* 20 bytes is enough */
                memset(str_buf, 0x00, 20);
                snprintf(str_buf, 20, "%d", (int)param->data.val);
                strcat(buf, str_buf);
            } else if (param->data_type == RIL_PARAM_TYPE_STRING) {
                strcat(buf, param->data.str);
            } else if (param->data_type == RIL_PARAM_TYPE_VOID) {
                // Do nothing
            } else {
                configASSERT(0);
            }
        }
    } while (ret >= 0);
    remove_redundant_comma_space(buf);
    strcat(buf, "\r\n");

    return strlen(buf);

}


int32_t ril_cmd_send_head_only_hdlr(uint32_t channel_id, char *buf, ril_request_info_t *info)
{
    ril_cmd_item_t *item = get_at_cmd_item((ril_cmd_id_t)info->request_id);

    /* consturct command head */
    snprintf(buf, RIL_BUF_SIZE - 1, "%s", item->cmd_head);

    return strlen(buf);
}


/*ATE*/
ril_status_t ril_request_command_echo(int32_t type, ril_cmd_response_callback_t callback, void *user_data)
{
    int32_t ret;
    ril_param_node_t *list_head;
    if (type == RIL_OMITTED_INTEGER_PARAM) {
        return ril_basic_request_send(RIL_ACTIVE_MODE, RIL_CMD_ID_E, NULL, (void *)callback, user_data);
    }
    // if (!(type == 0 || type == 1)){
    //     return RIL_STATUS_INVALID_PARAM;
    // }
    if ((ret = ril_param_list_create(&list_head)) < 0) {
        return RIL_STATUS_FAIL;
    }

    if ((ret = ril_param_list_add_int(list_head, type)) < 0) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    }
    return ril_basic_request_send(RIL_EXECUTE_MODE, RIL_CMD_ID_E, list_head, (void *)callback, user_data);
}


/*ATI*/
ril_status_t ril_request_identification_info(ril_cmd_response_callback_t callback, void *user_data)
{
    return ril_basic_request_send(RIL_ACTIVE_MODE, RIL_CMD_ID_I, NULL, (void *)callback, user_data);
}


#ifndef __RIL_CMD_SET_SLIM_ENABLE__
/*ATO*/
ril_status_t ril_request_return_to_online_data(int32_t type, ril_cmd_response_callback_t callback, void *user_data)
{
    int32_t ret;
    ril_param_node_t *list_head;
    if (type == RIL_OMITTED_INTEGER_PARAM) {
        return ril_basic_request_send(RIL_ACTIVE_MODE, RIL_CMD_ID_O, NULL, (void *)callback, user_data);
    }

    if ((ret = ril_param_list_create(&list_head)) < 0) {
        return RIL_STATUS_FAIL;
    }

    if ((ret = ril_param_list_add_int(list_head, type)) < 0) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    }
    return ril_basic_request_send(RIL_EXECUTE_MODE, RIL_CMD_ID_O, list_head, (void *)callback, user_data);
}


/*ATQ*/
ril_status_t ril_request_set_result_suppression_mode(int32_t type, ril_cmd_response_callback_t callback, void *user_data)
{
    int32_t ret;
    ril_param_node_t *list_head;
    if (type == RIL_OMITTED_INTEGER_PARAM) {
        return ril_basic_request_send(RIL_ACTIVE_MODE, RIL_CMD_ID_Q, NULL, (void *)callback, user_data);
    }

    //if (!(type == 0 || type == 1)){
    //    return RIL_STATUS_INVALID_PARAM;
    //}

    if ((ret = ril_param_list_create(&list_head)) < 0) {
        return RIL_STATUS_FAIL;
    }

    if ((ret = ril_param_list_add_int(list_head, type)) < 0) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    }
    return ril_basic_request_send(RIL_EXECUTE_MODE, RIL_CMD_ID_Q, list_head, (void *)callback, user_data);
}


/*ATS3*/
ril_status_t ril_request_command_line_termination_char(ril_cmd_response_callback_t callback, void *user_data)
{
    return ril_basic_request_send(RIL_READ_MODE, RIL_CMD_ID_S3, NULL, (void *)callback, user_data);
#if 0  /* command line termination character setting will influence RIL parser so that only allow query, not allow to set. */
    int32_t ret;
    ril_param_node_t *list_head;

    if (mode == RIL_ACTIVE_MODE) {
        return RIL_STATUS_INVALID_PARAM;
    }

    if (value == RIL_OMITTED_INTEGER_PARAM) {
        return ril_basic_request_send(mode, RIL_CMD_ID_S3, NULL, (void *)callback, user_data);
    }

    //if (mode == RIL_EXECUTE_MODE && value > 127){
    //    return RIL_STATUS_INVALID_PARAM;
    //}

    if ((ret = ril_param_list_create(&list_head)) < 0) {
        return RIL_STATUS_FAIL;
    }

    if ((ret = ril_param_list_add_int(list_head, value)) < 0) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    }
    return ril_basic_request_send(mode, RIL_CMD_ID_S3, list_head, (void *)callback, user_data);
#endif
}

/*ATS4*/
ril_status_t ril_request_response_formatting_char(ril_cmd_response_callback_t callback, void *user_data)
{
    return ril_basic_request_send(RIL_READ_MODE, RIL_CMD_ID_S4, NULL, (void *)callback, user_data);
#if 0    /* response format character setting will influence RIL parser so that only allow query, not allow to set. */
    int32_t ret;
    ril_param_node_t *list_head;

    if (mode == RIL_ACTIVE_MODE) {
        return RIL_STATUS_INVALID_PARAM;
    }

    if (value == RIL_OMITTED_INTEGER_PARAM) {
        return ril_basic_request_send(mode, RIL_CMD_ID_S4, NULL, (void *)callback, user_data);
    }

    //if (mode == RIL_EXECUTE_MODE && value > 127){
    //    return RIL_STATUS_INVALID_PARAM;
    //}

    if ((ret = ril_param_list_create(&list_head)) < 0) {
        return RIL_STATUS_FAIL;
    }

    if ((ret = ril_param_list_add_int(list_head, value)) < 0) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    }
    return ril_basic_request_send(mode, RIL_CMD_ID_S4, list_head, (void *)callback, user_data);
#endif
}

/*ATS5*/
ril_status_t ril_request_command_line_editing_char(ril_request_mode_t mode, int32_t value, ril_cmd_response_callback_t callback, void *user_data)
{
    int32_t ret;
    ril_param_node_t *list_head;

    if (mode == RIL_ACTIVE_MODE) {
        return RIL_STATUS_INVALID_PARAM;
    }

    if (value == RIL_OMITTED_INTEGER_PARAM) {
        return ril_basic_request_send(mode, RIL_CMD_ID_S5, NULL, (void *)callback, user_data);
    }

    // if (mode == RIL_EXECUTE_MODE && value > 127){
    //     return RIL_STATUS_INVALID_PARAM;
    // }

    if ((ret = ril_param_list_create(&list_head)) < 0) {
        return RIL_STATUS_FAIL;
    }

    if ((ret = ril_param_list_add_int(list_head, value)) < 0) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    }
    return ril_basic_request_send(mode, RIL_CMD_ID_S5, list_head, (void *)callback, user_data);
}


/*ATS7*/
ril_status_t ril_request_connection_completion_timeout(ril_request_mode_t mode, int32_t value, ril_cmd_response_callback_t callback, void *user_data)
{
    int32_t ret;
    ril_param_node_t *list_head;

    if (mode == RIL_ACTIVE_MODE) {
        return RIL_STATUS_INVALID_PARAM;
    }

    if (value == RIL_OMITTED_INTEGER_PARAM) {
        return ril_basic_request_send(mode, RIL_CMD_ID_S7, NULL, (void *)callback, user_data);
    }

    //if (mode == RIL_EXECUTE_MODE && (value < 2 || value > 255)){
    //    return RIL_STATUS_INVALID_PARAM;
    //}

    if ((ret = ril_param_list_create(&list_head)) < 0) {
        return RIL_STATUS_FAIL;
    }

    if ((ret = ril_param_list_add_int(list_head, value)) < 0) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    }
    return ril_basic_request_send(mode, RIL_CMD_ID_S7, list_head, (void *)callback, user_data);
}


/*ATS10*/
ril_status_t ril_request_automatic_disconnect_delay(ril_request_mode_t mode, int32_t value, ril_cmd_response_callback_t callback, void *user_data)
{
    int32_t ret;
    ril_param_node_t *list_head;

    if (mode == RIL_ACTIVE_MODE) {
        return RIL_STATUS_INVALID_PARAM;
    }

    if (value == RIL_OMITTED_INTEGER_PARAM) {
        return ril_basic_request_send(mode, RIL_CMD_ID_S10, NULL, (void *)callback, user_data);
    }

    //if (mode == RIL_EXECUTE_MODE && (value < 2 || value > 254)){
    //    return RIL_STATUS_INVALID_PARAM;
    //}

    if ((ret = ril_param_list_create(&list_head)) < 0) {
        return RIL_STATUS_FAIL;
    }

    if ((ret = ril_param_list_add_int(list_head, value)) < 0) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    }
    return ril_basic_request_send(mode, RIL_CMD_ID_S10, list_head, (void *)callback, user_data);
}


/*ATV*/
#if 0 // not support
ril_status_t ril_request_dce_response_format(int32_t type, void *callback, void *user_data)
{
    int32_t ret;
    ril_param_node_t *list_head;
    RIL_LOGE("ril_request_dce_response_format:%d\r\n", type);
    if (type == RIL_OMITTED_INTEGER_PARAM) {
        return ril_basic_request_send(RIL_ACTIVE_MODE, RIL_CMD_ID_V, NULL, (void *)callback, user_data);
    }
    //if (!(type == 0 || type == 1)){
    //    return RIL_STATUS_INVALID_PARAM;
    //}
    if ((ret = ril_param_list_create(&list_head)) < 0) {
        return RIL_STATUS_FAIL;
    }

    if ((ret = ril_param_list_add_int(list_head, type)) < 0) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    }
    return ril_basic_request_send(RIL_EXECUTE_MODE, RIL_CMD_ID_V, list_head, (void *)callback, user_data);
}
#endif

/*ATX*/
ril_status_t ril_request_result_code_selection(int32_t type, ril_cmd_response_callback_t callback, void *user_data)
{
    int32_t ret;
    ril_param_node_t *list_head;
    if (type == RIL_OMITTED_INTEGER_PARAM) {
        return ril_basic_request_send(RIL_ACTIVE_MODE, RIL_CMD_ID_X, NULL, (void *)callback, user_data);
    }
    if ((ret = ril_param_list_create(&list_head)) < 0) {
        return RIL_STATUS_FAIL;
    }

    if ((ret = ril_param_list_add_int(list_head, type)) < 0) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    }
    return ril_basic_request_send(RIL_EXECUTE_MODE, RIL_CMD_ID_X, list_head, (void *)callback, user_data);
}

/*ATZ*/
ril_status_t ril_request_reset_to_default_config(int32_t type, ril_cmd_response_callback_t callback, void *user_data)
{
    int32_t ret;
    ril_param_node_t *list_head;
    if (type == RIL_OMITTED_INTEGER_PARAM) {
        return ril_basic_request_send(RIL_ACTIVE_MODE, RIL_CMD_ID_Z, NULL, (void *)callback, user_data);
    }
    if ((ret = ril_param_list_create(&list_head)) < 0) {
        return RIL_STATUS_FAIL;
    }

    if ((ret = ril_param_list_add_int(list_head, type)) < 0) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    }
    return ril_basic_request_send(RIL_EXECUTE_MODE, RIL_CMD_ID_Z, list_head, (void *)callback, user_data);
}

/*AT&F*/
ril_status_t ril_request_set_to_factory_defined_config(int32_t type, ril_cmd_response_callback_t callback, void *user_data)
{
    int32_t ret;
    ril_param_node_t *list_head;
    if (type == RIL_OMITTED_INTEGER_PARAM) {
        return ril_basic_request_send(RIL_ACTIVE_MODE, RIL_CMD_ID_AF, NULL, (void *)callback, user_data);
    }
    if ((ret = ril_param_list_create(&list_head)) < 0) {
        return RIL_STATUS_FAIL;
    }

    if ((ret = ril_param_list_add_int(list_head, type)) < 0) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    }
    return ril_basic_request_send(RIL_EXECUTE_MODE, RIL_CMD_ID_AF, list_head, (void *)callback, user_data);
}


/*AT+GCAP*/
ril_status_t ril_request_complete_capabilities_list(ril_request_mode_t mode, ril_cmd_response_callback_t callback, void *user_data)
{
    return ril_basic_request_send(mode, RIL_CMD_ID_GCAP, NULL, (void *)callback, user_data);
}


/*ATS25*/
ril_status_t ril_request_delay_to_dtr(ril_request_mode_t mode, int32_t value, ril_cmd_response_callback_t callback, void *user_data)
{
    int32_t ret;
    ril_param_node_t *list_head;

    if (mode == RIL_ACTIVE_MODE) {
        return RIL_STATUS_INVALID_PARAM;
    }

    if (value == RIL_OMITTED_INTEGER_PARAM) {
        return ril_basic_request_send(mode, RIL_CMD_ID_S25, NULL, (void *)callback, user_data);
    }

    if ((ret = ril_param_list_create(&list_head)) < 0) {
        return RIL_STATUS_FAIL;
    }

    if ((ret = ril_param_list_add_int(list_head, value)) < 0) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    }
    return ril_basic_request_send(mode, RIL_CMD_ID_S25, list_head, (void *)callback, user_data);
}
#endif /* __RIL_CMD_SET_SLIM_ENABLE__ */

/***************************************************************************/
/*                                                  Response Begin                                                       */
/***************************************************************************/

int32_t ril_response_basic_command(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}

int32_t ril_response_basic_with_connect(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    int32_t ret = -1;
    ril_channel_cntx_t *channel_p = ril_get_channel(channel_id);
    char *line = NULL;
    char *cur_pos = cmd_buf;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;
    ril_connect_rsp_t response = {0};
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;
    bool need_parse = true;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    /* parse reponse */
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        }
        line = at_get_one_line(&cur_pos);
    }

    if (line != NULL &&
            res_code == RIL_RESULT_CODE_CONNECT &&
            cmd_buf[strlen("CONNECT")] != '\r') {
        response.connect_text = line + strlen("CONNECT ");
        ret = 0;
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;
}


int32_t ril_response_normal_int(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    int32_t ret = -1;
    ril_channel_cntx_t *channel_p = ril_get_channel(channel_id);
    char *line;
    char *cur_pos = cmd_buf;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;
    ril_normal_int_rsp_t response = {0};
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;
    bool need_parse = true;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    /* parse reponse */
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (channel_p->curr_request_mode == RIL_READ_MODE
                    && !str_starts_with(line, "AT")) {

                ret = at_tok_nextint(&line, &response.value);
                if (ret < 0) {
                    break;
                }
            }
        }
        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;
}


int32_t ril_response_normal_str(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    int32_t ret = -1;
    ril_channel_cntx_t *channel_p = ril_get_channel(channel_id);
    char *line;
    char *cur_pos = cmd_buf;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;
    ril_normal_str_rsp_t response = {0};
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;
    bool need_parse = true;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    /* parse reponse */
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &response.text);
            if (ret < 0) {
                break;
            }
        }
        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;
}


/*ATE*/
int32_t ril_response_command_echo(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_basic_command(channel_id, cmd_buf, cmd_buf_len);
}


/*ATI*/
int32_t ril_response_request_identification_info(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    int32_t ret = -1;
    ril_channel_cntx_t *channel_p = ril_get_channel(channel_id);
    char *line;
    char *cur_pos = cmd_buf;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;
    ril_cmd_response_t cmd_response;
    ril_identification_info_rsp_t response = {0};
    ril_cmd_response_callback_t rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;
    bool need_parse = true;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }   

    /* parse reponse */
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            response.identification_info = line;
            ret = 0;
        }
        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;
}


/*ATO*/
int32_t ril_response_return_to_online_data(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_basic_with_connect(channel_id, cmd_buf, cmd_buf_len);
}


/*ATQ*/
int32_t ril_response_set_result_suppression_mode(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_basic_command(channel_id, cmd_buf, cmd_buf_len);
}


/*ATS3*/
int32_t ril_response_command_line_termination_char(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_normal_int(channel_id, cmd_buf, cmd_buf_len);
}

/*ATS4*/
int32_t ril_response_response_formatting_char(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_normal_int(channel_id, cmd_buf, cmd_buf_len);
}

/*ATS5*/
int32_t ril_response_command_line_editing_char(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_normal_int(channel_id, cmd_buf, cmd_buf_len);
}


/*ATS7*/
int32_t ril_response_connection_completion_timeout(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_normal_int(channel_id, cmd_buf, cmd_buf_len);
}


/*ATS10*/
int32_t ril_response_automatic_disconnect_delay(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_normal_int(channel_id, cmd_buf, cmd_buf_len);
}


/*ATV*/
#if 0 // not support
int32_t ril_response_dce_response_format(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_basic_command(channel_id, cmd_buf, cmd_buf_len);
}
#endif

/*ATX*/
int32_t ril_response_result_code_selection(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_basic_command(channel_id, cmd_buf, cmd_buf_len);
}

/*ATZ*/
int32_t ril_response_reset_to_default_config(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_basic_command(channel_id, cmd_buf, cmd_buf_len);
}

/*AT&F*/
int32_t ril_response_set_to_factory_defined_config(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_basic_command(channel_id, cmd_buf, cmd_buf_len);
}

/*AT+GCAP*/
int32_t ril_response_complete_capabilities_list(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_normal_str(channel_id, cmd_buf, cmd_buf_len);
}


/*ATS25*/
int32_t ril_response_delay_to_dtr(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_normal_int(channel_id, cmd_buf, cmd_buf_len);
}


/*************************************************
 *                 UT test case
 *************************************************/
#if defined(__RIL_UT_TEST_CASES__)
int32_t ril_response_ut_callback_v250(ril_cmd_response_t *cmd_response)
{
    RIL_LOGUT("final result code: %d\r\n", cmd_response->res_code);
    RIL_LOGUT("request mode: %d\r\n", cmd_response->mode);
    if (cmd_response->mode == RIL_TEST_MODE &&
        cmd_response->test_mode_str != NULL &&
        cmd_response->test_mode_str_len > 0) {
        RIL_LOGDUMPSTRUT("test mode str: %s\r\n", cmd_response->test_mode_str_len, cmd_response->test_mode_str);        
    } else {
        switch (cmd_response->cmd_id) {
            case RIL_CMD_ID_E: {
                // no parameter.
            }
            break;
            
            case RIL_CMD_ID_I: {
                ril_identification_info_rsp_t *param = (ril_identification_info_rsp_t *)cmd_response->cmd_param;
                if (param != NULL && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("identification_info: %s\r\n", param->identification_info ? param->identification_info : UT_OMITTED_PARAM);
                }
            }
            break;

#ifndef __RIL_CMD_SET_SLIM_ENABLE__
            case RIL_CMD_ID_O: {
                ril_connect_rsp_t *param = (ril_connect_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_CONNECT) {
                    RIL_LOGUT("connect_text: %s\r\n", param->connect_text ? param->connect_text : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_Q: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_S3: {
                ril_normal_int_rsp_t *param = (ril_normal_int_rsp_t *)cmd_response->cmd_param;
                if (param != NULL && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("value: %d\r\n", (int)param->value);
                }
            }
            break;

            case RIL_CMD_ID_S4: {
                ril_normal_int_rsp_t *param = (ril_normal_int_rsp_t *)cmd_response->cmd_param;
                if (param != NULL && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("value: %d\r\n", (int)param->value);
                }
            }
            break;
            
            case RIL_CMD_ID_S5: {
                ril_normal_int_rsp_t *param = (ril_normal_int_rsp_t *)cmd_response->cmd_param;
                if (param != NULL && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("value: %d\r\n", (int)param->value);
                }
            }
            break;

            case RIL_CMD_ID_S7: {
                ril_normal_int_rsp_t *param = (ril_normal_int_rsp_t *)cmd_response->cmd_param;
                if (param != NULL && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("value: %d\r\n", (int)param->value);
                }
            }
            break;

            case RIL_CMD_ID_S10: {
                ril_normal_int_rsp_t *param = (ril_normal_int_rsp_t *)cmd_response->cmd_param;
                if (param != NULL && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("value: %d\r\n", (int)param->value);
                }
            }
            break;
            
            case RIL_CMD_ID_X: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_Z: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_AF: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_GCAP: {
                ril_normal_str_rsp_t *param = (ril_normal_str_rsp_t *)cmd_response->cmd_param;
                if (param != NULL && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("text: %s\r\n", param->text ? param->text : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_S25: {
                ril_normal_int_rsp_t *param = (ril_normal_int_rsp_t *)cmd_response->cmd_param;
                if (param != NULL && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("value: %d\r\n", (int)param->value);
                }
            }
            break;
#endif /* __RIL_CMD_SET_SLIM_ENABLE__ */

            default:
                break;
        }
    }
    return 0;
}
#endif /* __RIL_UT_TEST_CASES__ */

