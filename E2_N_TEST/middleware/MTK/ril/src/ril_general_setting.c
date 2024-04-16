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
#include "ril_general_setting.h"
#include "ril_cmds_common.h"
#include "ril_channel_config.h"
#include "ril_task.h"

#ifdef MTK_NVDM_ENABLE
#include "nvdm.h"
#endif

typedef void (*setting_func_ptr)(int32_t channel_id, int32_t flow_idx);
typedef struct {
    setting_func_ptr setting_func;
    void *setting_callback;
} ril_general_setting_item_t;

void ril_general_setting_send_ATE0(int32_t channel_id, int32_t flow_idx);
void ril_general_setting_send_CMEE(int32_t channel_id, int32_t flow_idx);
void ril_general_setting_send_CFUN(int32_t channel_id,int32_t flow_idx);
void ril_general_setting_send_COPS(int32_t channel_id,int32_t flow_idx);
void ril_general_setting_send_MATWAKEUP(int32_t channel_id,int32_t flow_idx);

int32_t ril_general_setting_callback_ATE0(ril_cmd_response_t *cmd_response);
int32_t ril_general_setting_callback_CMEE(ril_cmd_response_t *cmd_response);
int32_t ril_general_setting_callback_CFUN(ril_cmd_response_t *cmd_response);
int32_t ril_general_setting_callback_COPS(ril_cmd_response_t *cmd_response);
int32_t ril_general_setting_callback_MATWAKEUP(ril_cmd_response_t *cmd_response);

#ifdef MTK_CREATE_DEFAULT_APN
void ril_general_setting_send_CEREG(int32_t channel_id,int32_t flow_idx);
int32_t ril_general_setting_callback_CEREG(ril_cmd_response_t *cmd_response);
void ril_general_setting_send_MDPDNP(int32_t channel_id,int32_t flow_idx);
int32_t ril_general_setting_callback_MDPDNP(ril_cmd_response_t *cmd_response);
#endif

int32_t ril_general_setting_format_config_callback(ril_cmd_response_t *cmd_response);
int32_t ril_general_setting_special_command_callback(ril_cmd_response_t *cmd_response);


int32_t ril_general_setting_flow_start(ril_general_setting_flow_type_t type, int32_t channel_id);
int32_t ril_general_setting_flow_next_step(ril_general_setting_flow_type_t type,int32_t channel_id,int32_t curr_step);


/* command format setting for each channel */
ril_general_setting_item_t ril_general_setting_format_config_flow[] = {
    /* ATE0 */
    {ril_general_setting_send_ATE0, (void *)ril_general_setting_format_config_callback},
    /* AT+CMEE=1 */
    {ril_general_setting_send_CMEE, (void *)ril_general_setting_format_config_callback}
};

/* special command sending */
ril_general_setting_item_t ril_general_setting_special_command_flow[] = {
#ifdef MTK_CREATE_DEFAULT_APN
    /* AT+CEREG=2 */
    {ril_general_setting_send_CEREG,     (void *)ril_general_setting_special_command_callback},
    /* AT*MDPDNP=1 */
    {ril_general_setting_send_MDPDNP,    (void *)ril_general_setting_special_command_callback},
#endif
    /* AT*MATWAKEUP */
    {ril_general_setting_send_MATWAKEUP, (void *)ril_general_setting_special_command_callback},
    /* AT+CFUN=1 */
    {ril_general_setting_send_CFUN,      (void *)ril_general_setting_special_command_callback},
    /* AT+COPS=0 */
    {ril_general_setting_send_COPS,      (void *)ril_general_setting_special_command_callback}
};


/* used to indicate command format setting is done or not */
static bool is_format_setting_done;
static uint32_t channel_format_config_bitmask;
//uint8_t MATWAKEUP_result = 0;


#define CONSTRUCT_USER_DATA(_channel_id, _flow_idx)    ((((uint16_t)_flow_idx) << 16) | ((uint16_t)_channel_id))
#define GET_FLOW_IDX_FROM_USER_DATA(_user_data)    (((uint32_t)_user_data) >> 16)
#define GET_CHANNEL_ID_FROM_USER_DATA(_user_data)    (((uint32_t)_user_data) & 0x0000FFFF)
#define GENERAL_SETTING_FORMAT_CONFIG_FLOW_STEP_NUMS    ((sizeof(ril_general_setting_format_config_flow))/(sizeof(ril_general_setting_item_t)))
#define GENERAL_SETTING_SPECIAL_COMMAND_FLOW_STEP_NUMS    ((sizeof(ril_general_setting_special_command_flow))/(sizeof(ril_general_setting_item_t)))


/* nvdm info definition for at+cfun, at+cops sending */
#define NVDM_GROUP_NAME_AUTO_SEND    ("RIL_CFG")
#define NVDM_ITEM_NAME_AUTO_SEND     ("AUTO_SEND_CFUN_BY_AP")
#define NVDM_VALUE_AUTO_SEND_ON      ("01")
#define NVDM_VALUE_AUTO_SEND_OFF     ("00")


void ril_general_setting_format_init_bitmask()
{
    int32_t i;
    channel_format_config_bitmask = 0;
    for (i = RIL_URC_CHANNEL_ID; i <= RIL_AT_DATA_CHANNEL_ID_END; i++) {
        channel_format_config_bitmask |= (1 << i);
    }
}


void ril_general_setting_format_clear_bitmask(int32_t channel_id)
{
    channel_format_config_bitmask &= (~(1 << channel_id));
    if (channel_format_config_bitmask == 0) {
        ril_request_pending_list_resume();
    }
}


int32_t ril_general_setting_format_config_callback(ril_cmd_response_t *cmd_response)
{
    int32_t channel_id = (int32_t)GET_CHANNEL_ID_FROM_USER_DATA(cmd_response->user_data);
    int32_t flow_idx = (int32_t)GET_FLOW_IDX_FROM_USER_DATA(cmd_response->user_data);
    RIL_LOGI("channel format config, channel_id: %d, flow_idx: %d, result_code: %d\r\n", (int)channel_id, (int)flow_idx, (int)cmd_response->res_code);
    switch (cmd_response->cmd_id) {
        case RIL_CMD_ID_E:
        case RIL_CMD_ID_CMEE: {
            if (cmd_response->res_code == RIL_RESULT_CODE_OK) {
                if (ril_general_setting_flow_next_step(RIL_GENERAL_SETTING_FLOW_TYPE_FORMAT_CONFIG, channel_id, flow_idx) < 0) {
                    if (is_format_setting_done == false) {
                        is_format_setting_done = true;
                        ril_general_setting_flow_start(RIL_GENERAL_SETTING_FLOW_TYPE_SPECIAL_COMMAND, channel_id);
                    }

                    ril_general_setting_format_clear_bitmask(channel_id);
                }
            }
        }
        break;
    }
    return 0;
}


int32_t ril_general_setting_special_command_callback(ril_cmd_response_t *cmd_response)
{    
    int32_t flow_idx = (int32_t)cmd_response->user_data;
    RIL_LOGI("special command sending, flow_idx: %d, result_code: %d\r\n", (int)flow_idx, (int)cmd_response->res_code);
    switch (cmd_response->cmd_id) {
        case RIL_CMD_ID_MATWAKEUP:
        case RIL_CMD_ID_CFUN:
        case RIL_CMD_ID_CEREG:
        case RIL_CMD_ID_COPS: {
            if (cmd_response->res_code == RIL_RESULT_CODE_OK) {            
                ril_general_setting_flow_next_step(RIL_GENERAL_SETTING_FLOW_TYPE_SPECIAL_COMMAND, 0, flow_idx);
            }
        }
        break;

        default:
            break;
    }
    return 0;
}


void ril_general_setting_send_MATWAKEUP(int32_t channel_id, int32_t flow_idx)
{
    ril_request_set_modem_wakeup_indication(RIL_EXECUTE_MODE,
                                            1,
                                            (ril_cmd_response_callback_t)ril_general_setting_special_command_flow[flow_idx].setting_callback,
                                            (void *)flow_idx);

}


int32_t ril_general_setting_callback_MATWAKEUP(ril_cmd_response_t *cmd_response)
{   
    int32_t flow_idx = (int32_t)cmd_response->user_data;
    if (cmd_response->res_code == RIL_RESULT_CODE_OK) {            
        ril_general_setting_flow_next_step(RIL_GENERAL_SETTING_FLOW_TYPE_SPECIAL_COMMAND, 0, flow_idx);
        //MATWAKEUP_result = 100;
    }
    return 0;
}


void ril_general_setting_send_CFUN(int32_t channel_id, int32_t flow_idx)
{
    char flag = 0;

#ifdef MTK_NVDM_ENABLE
    nvdm_status_t status;    
    uint32_t size = 1;

    if ((status = nvdm_read_data_item(NVDM_GROUP_NAME_AUTO_SEND, NVDM_ITEM_NAME_AUTO_SEND, (uint8_t *)&flag, &size)) != NVDM_STATUS_OK) {
        flag = '0';
        size = 1;
        status = nvdm_write_data_item(NVDM_GROUP_NAME_AUTO_SEND,
                                    NVDM_ITEM_NAME_AUTO_SEND,
                                    NVDM_DATA_ITEM_TYPE_STRING,
                                    (uint8_t *)&flag,
                                    size);
    }
#endif
    RIL_LOGI("nvdm flag: %d\r\n", flag);

    if (flag == '1') {
        ril_request_set_phone_functionality(RIL_EXECUTE_MODE,
                                            1,
                                            RIL_OMITTED_INTEGER_PARAM,
                                            (ril_cmd_response_callback_t)ril_general_setting_special_command_flow[flow_idx].setting_callback,
                                            (void *)flow_idx);
    }
}


int32_t ril_general_setting_callback_CFUN(ril_cmd_response_t *cmd_response)
{   
    int32_t flow_idx = (int32_t)cmd_response->user_data;
    if (cmd_response->res_code == RIL_RESULT_CODE_OK) {            
        ril_general_setting_flow_next_step(RIL_GENERAL_SETTING_FLOW_TYPE_SPECIAL_COMMAND, 0, flow_idx);
    }
    return 0;
}


void ril_general_setting_send_COPS(int32_t channel_id, int32_t flow_idx)
{
    ril_request_plmn_selection(RIL_EXECUTE_MODE,
                                0,
                                RIL_OMITTED_INTEGER_PARAM,
                                RIL_OMITTED_STRING_PARAM,
                                RIL_OMITTED_INTEGER_PARAM,
                                ril_general_setting_callback_COPS,
                                (void *)flow_idx);
}


int32_t ril_general_setting_callback_COPS(ril_cmd_response_t *cmd_response)
{
    int32_t flow_idx = (int32_t)cmd_response->user_data;
    if (cmd_response->res_code == RIL_RESULT_CODE_OK) {            
        ril_general_setting_flow_next_step(RIL_GENERAL_SETTING_FLOW_TYPE_SPECIAL_COMMAND, 0, flow_idx);
    }
    return 0;
}


#ifdef MTK_CREATE_DEFAULT_APN
void ril_general_setting_send_CEREG(int32_t channel_id, int32_t flow_idx)
{
    ril_request_eps_network_registration_status(RIL_EXECUTE_MODE,
                                2,
                                ril_general_setting_callback_CEREG,
                                (void *)flow_idx);
}


int32_t ril_general_setting_callback_CEREG(ril_cmd_response_t *cmd_response)
{
    int32_t flow_idx = (int32_t)cmd_response->user_data;
    if (cmd_response->res_code == RIL_RESULT_CODE_OK) {            
        ril_general_setting_flow_next_step(RIL_GENERAL_SETTING_FLOW_TYPE_SPECIAL_COMMAND, 0, flow_idx);
    }
    return 0;
}


void ril_general_setting_send_MDPDNP(int32_t channel_id, int32_t flow_idx)
{
    ril_request_default_pdn_parameter(RIL_EXECUTE_MODE,
                                1,
                                ril_general_setting_callback_MDPDNP,
                                (void *)flow_idx);
}


int32_t ril_general_setting_callback_MDPDNP(ril_cmd_response_t *cmd_response)
{
    int32_t flow_idx = (int32_t)cmd_response->user_data;
    if (cmd_response->res_code == RIL_RESULT_CODE_OK) {            
        ril_general_setting_flow_next_step(RIL_GENERAL_SETTING_FLOW_TYPE_SPECIAL_COMMAND, 0, flow_idx);
    }
    return 0;
}
#endif


void ril_general_setting_send_CMEE(int32_t channel_id, int32_t flow_idx)
{
    ril_param_node_t *list_head;
    if (channel_id < RIL_URC_CHANNEL_ID || channel_id > RIL_AT_DATA_CHANNEL_ID_END) {
        return;
    }
    if (flow_idx >= GENERAL_SETTING_FORMAT_CONFIG_FLOW_STEP_NUMS) {
        return;
    }

    ril_param_list_create(&list_head);
    ril_param_list_add_int(list_head, 1);
    ril_request_send_via_channel_prior(
        RIL_EXECUTE_MODE,
        RIL_CMD_ID_CMEE,
        list_head,
        (void *)ril_general_setting_format_config_flow[flow_idx].setting_callback,
        (void *)CONSTRUCT_USER_DATA(channel_id, flow_idx),
        channel_id);
    ril_request_pending_list_suspend();
}


int32_t ril_general_setting_callback_CMEE(ril_cmd_response_t *cmd_response)
{

    int32_t channel_id = (int32_t)GET_CHANNEL_ID_FROM_USER_DATA(cmd_response->user_data);
    int32_t flow_idx = (int32_t)GET_FLOW_IDX_FROM_USER_DATA(cmd_response->user_data);
    if (cmd_response->res_code == RIL_RESULT_CODE_OK) {
        if ((ril_general_setting_flow_next_step(RIL_GENERAL_SETTING_FLOW_TYPE_FORMAT_CONFIG, channel_id, flow_idx) < 0)
            && is_format_setting_done == false) {
            is_format_setting_done = true;
            ril_general_setting_flow_start(RIL_GENERAL_SETTING_FLOW_TYPE_SPECIAL_COMMAND, channel_id);
        }
    }
    return 0;
}


void ril_general_setting_send_ATE0(int32_t channel_id, int32_t flow_idx)
{
    ril_param_node_t *list_head;
    if (channel_id < RIL_URC_CHANNEL_ID || channel_id > RIL_AT_DATA_CHANNEL_ID_END) {
        return;
    }
    if (flow_idx >= GENERAL_SETTING_FORMAT_CONFIG_FLOW_STEP_NUMS) {
        return;
    }
    ril_param_list_create(&list_head);
    ril_param_list_add_int(list_head, 0);
    ril_request_send_via_channel_prior(
        RIL_EXECUTE_MODE,
        RIL_CMD_ID_E,
        list_head,
        (void *)ril_general_setting_format_config_flow[flow_idx].setting_callback,
        (void *)CONSTRUCT_USER_DATA(channel_id, flow_idx),
        channel_id);
    ril_request_pending_list_suspend();
}


int32_t ril_general_setting_callback_ATE0(ril_cmd_response_t *cmd_response)
{
    int32_t channel_id = (int32_t)GET_CHANNEL_ID_FROM_USER_DATA(cmd_response->user_data);
    int32_t flow_idx = (int32_t)GET_FLOW_IDX_FROM_USER_DATA(cmd_response->user_data);
    if (cmd_response->res_code == RIL_RESULT_CODE_OK) {
        ril_general_setting_flow_next_step(RIL_GENERAL_SETTING_FLOW_TYPE_FORMAT_CONFIG, channel_id, flow_idx);
    }
    return 0;
}


int32_t ril_general_setting_flow_start(ril_general_setting_flow_type_t type, int32_t channel_id)
{
    switch (type) {
        case RIL_GENERAL_SETTING_FLOW_TYPE_FORMAT_CONFIG:
            (ril_general_setting_format_config_flow[0].setting_func)(channel_id, 0);
            break;
        case RIL_GENERAL_SETTING_FLOW_TYPE_SPECIAL_COMMAND:
            (ril_general_setting_special_command_flow[0].setting_func)(channel_id, 0);
            break;
        default:
            break;
    }
    return 0;
}


int32_t ril_general_setting_flow_next_step(ril_general_setting_flow_type_t type, int32_t channel_id, int32_t curr_step)
{
    int32_t flow_idx = curr_step;
    int32_t ret_val = -1;
    flow_idx++;
    switch (type) {
        case RIL_GENERAL_SETTING_FLOW_TYPE_FORMAT_CONFIG:
            if (flow_idx < GENERAL_SETTING_FORMAT_CONFIG_FLOW_STEP_NUMS) {
                (ril_general_setting_format_config_flow[flow_idx].setting_func)(channel_id, flow_idx);
                ret_val = 0;
            }
            break;
        case RIL_GENERAL_SETTING_FLOW_TYPE_SPECIAL_COMMAND:
            if (flow_idx < GENERAL_SETTING_SPECIAL_COMMAND_FLOW_STEP_NUMS) {
                (ril_general_setting_special_command_flow[flow_idx].setting_func)(channel_id, flow_idx);
                ret_val = 0;
            }
            break;
        default:
            break;
    }
    return ret_val;
    
}


void ril_general_setting_init()
{
    is_format_setting_done = false;
    ril_general_setting_format_init_bitmask();
}


void ril_general_setting()
{
    int32_t i;
    RIL_LOGI("ril general setting\r\n");
    is_format_setting_done = false;

    for (i = 0; i < RIL_MAX_CHNL; i++) {
        /* config ATE0 */
        //ril_param_list_create(&list_head[i]);
        //ril_param_list_add_int(list_head[i], 0);
        ril_general_setting_flow_start(RIL_GENERAL_SETTING_FLOW_TYPE_FORMAT_CONFIG, i + 1);
        //ril_request_send_via_channel(RIL_EXECUTE_MODE, RIL_CMD_ID_E, list_head[i], (void *)ril_general_setting_format_config_flow[i], (void *)(i + 1), i + 1);
    }
}

