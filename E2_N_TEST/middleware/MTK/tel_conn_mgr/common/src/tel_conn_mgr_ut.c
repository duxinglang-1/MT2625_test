/* Copyright Statement:
 *
 * (C) 2005-2017 MediaTek Inc. All rights reserved.
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
 * the License Agreement ("Permitted User"). If you are not a Permitted User,
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

#include "tel_conn_mgr_common.h"

#if defined(TEL_CONN_MGR_UT) || defined(TEL_CONN_MGR_IT)

#include "task.h"
#include "tel_conn_mgr_app_api.h"
#include "tel_conn_mgr_bearer_iprot.h"
#include "tel_conn_mgr_ut.h"
#ifdef TEL_CONN_MGR_APB_SUPPORT
#include "apb_proxy.h"
#endif

/************************************UT IT common************************************/

#define TEL_CONN_MGR_UT_TASK_QUEUE_MAX_SIZE (10)

tel_conn_mgr_ret_enum tel_conn_mgr_ut_task_queue_create(tel_conn_mgr_queue_hdl_t *queue)
{
    if (!queue)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }
    
    if (*queue)
    {
        TEL_CONN_MGR_LOG_WARN("Task queue has been created.");
        return TEL_CONN_MGR_RET_OK;
    }
    
    *queue = tel_conn_mgr_queue_create(TEL_CONN_MGR_UT_TASK_QUEUE_MAX_SIZE,
                                       sizeof(tel_conn_mgr_msg_struct *));

    if (!(*queue))
    {
        TEL_CONN_MGR_LOG_ERR("Failed to create queue.");
        return TEL_CONN_MGR_RET_NOT_ENOUGH_MEMORY;
    }

    return TEL_CONN_MGR_RET_OK;
}


void tel_conn_mgr_ut_task_queue_free(tel_conn_mgr_queue_hdl_t queue)
{
    if (queue)
    {
        tel_conn_mgr_queue_delete(queue);
    }    
}


void tel_conn_mgr_ut_task_create(tel_conn_mgr_queue_hdl_t *queue,
                                         tel_conn_mgr_task_hdl_t *task,
                                         void *main_func,                                             
                                         char *task_name,
                                         int stack_size,
                                         int prio,
                                         void *user_data)
{
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;

    if (!queue || !task || !main_func)
    {
        return;
    }
    
    ret = tel_conn_mgr_ut_task_queue_create(queue);
    if (TEL_CONN_MGR_RET_OK != ret)
    {
        return;
    }
    
    if (!(*task))
    {
        tel_conn_mgr_task_create(main_func,
                                 task_name,
                                 stack_size/sizeof(TEL_CONN_MGR_PORT_STACK_TYPE),
                                 user_data,
                                 prio,
                                 task);        
    }

    if (!(*task))
    {
        tel_conn_mgr_ut_task_queue_free(*queue);
        TEL_CONN_MGR_LOG_ERR("Fail to create %s task.", task_name);
    }
}


#ifdef TEL_CONN_MGR_UT
/************************************UT only (RIL simulation)**************************************/
#define TEL_CONN_MGR_UT_RIL_TASK_NAME "ril_ut"
#define TEL_CONN_MGR_UT_RIL_TASK_STACKSIZE (2 * 1024)
#define TEL_CONN_MGR_UT_RIL_TASK_PRIO TASK_PRIORITY_ABOVE_NORMAL

#define MSG_ID_TEL_CONN_MGR_UT_RIL_SEND_AT_CMD_REQ    0x300

typedef int (*rsp_callback)(ril_result_code_t code,
                             ril_request_mode_t mode,
                             char *payload,
                             uint32_t payload_len,
                             void *param,
                             void* user_data);

typedef struct
{
    int msg_id;
    int data_channel;
    int cid;
    tel_conn_mgr_at_cmd_type_enum cmd_type;
    void *user_data;
    ril_cmd_response_callback_t callback;
}tel_conn_mgr_ut_ril_msg_struct;

typedef struct
{
    ril_result_code_t rsp;
    tel_conn_mgr_at_cmd_type_enum cmd_type;
}tel_conn_mgr_ut_ril_at_cmd_rsp_struct;


tel_conn_mgr_task_hdl_t tel_conn_mgr_ut_ril_task_hdl = NULL;
tel_conn_mgr_queue_hdl_t tel_conn_mgr_ut_ril_task_queue_hdl = NULL;
ril_event_callback_t tel_conn_mgr_ut_ril_urc_callback = NULL;
ril_cid_state_struct_t tel_conn_mgr_ut_ril_bearer_state[TEL_CONN_MGR_BEARER_TYPE_MAX_NUM] = {TEL_CONN_MGR_FALSE};


tel_conn_mgr_nw_reg_status_struct tel_conn_mgr_ut_ril_cereg_ok = {
#ifdef TEL_CONN_MGR_ENABLE_CHECK_CEREG
    2,
#endif
    TEL_CONN_MGR_NW_REG_STAT_REGED_HMNW,
    TEL_CONN_MGR_NW_REG_ACT_UTRAN};

tel_conn_mgr_nw_reg_status_struct tel_conn_mgr_ut_ril_cereg_trying = {
#ifdef TEL_CONN_MGR_ENABLE_CHECK_CEREG
    2,
#endif
    TEL_CONN_MGR_NW_REG_STAT_TRYING,
    TEL_CONN_MGR_NW_REG_ACT_UTRAN};

tel_conn_mgr_nw_reg_status_struct tel_conn_mgr_ut_ril_cereg_fail = {
#ifdef TEL_CONN_MGR_ENABLE_CHECK_CEREG
    2,
#endif
    TEL_CONN_MGR_NW_REG_STAT_UNKNOWN,
    TEL_CONN_MGR_NW_REG_ACT_UTRAN};

/* Control the responses. */
tel_conn_mgr_ut_ril_at_cmd_rsp_struct tel_conn_mgr_ut_ril_at_cmd_rsp1[] = {
    {RIL_RESULT_CODE_OK, TEL_CONN_MGR_AT_CMD_TYPE_CEREG},
    {RIL_RESULT_CODE_OK, TEL_CONN_MGR_AT_CMD_TYPE_CGEREP},
    {RIL_RESULT_CODE_OK, TEL_CONN_MGR_AT_CMD_TYPE_CFUN_ON},
    
    {RIL_RESULT_CODE_OK, TEL_CONN_MGR_AT_CMD_TYPE_CEREG_QUERY},
    {RIL_RESULT_CODE_OK, TEL_CONN_MGR_AT_CMD_TYPE_CGACT_QUERY},
    //{RIL_RESULT_CODE_ERROR, TEL_CONN_MGR_AT_CMD_TYPE_CGACT_QUERY},
    {RIL_RESULT_CODE_OK, TEL_CONN_MGR_AT_CMD_TYPE_CGDCONT},
    {RIL_RESULT_CODE_OK, TEL_CONN_MGR_AT_CMD_TYPE_CGAUTH},
    {RIL_RESULT_CODE_CONNECT, TEL_CONN_MGR_AT_CMD_TYPE_CGDATA},
    
    {RIL_RESULT_CODE_OK, TEL_CONN_MGR_AT_CMD_TYPE_CGCONTRDP},

    {RIL_RESULT_CODE_OK, TEL_CONN_MGR_AT_CMD_TYPE_CGACT_DEACT},

    {RIL_RESULT_CODE_OK, TEL_CONN_MGR_AT_CMD_TYPE_NONE}
};

tel_conn_mgr_nw_reg_status_struct *tel_conn_mgr_ut_cereg_query_rsp = &tel_conn_mgr_ut_ril_cereg_trying;

tel_conn_mgr_bool tel_conn_mgr_ut_ril_urc_before_rsp = TEL_CONN_MGR_FALSE;


ril_result_code_t tel_conn_mgr_ut_ril_get_rsp_by_cmd_type(tel_conn_mgr_at_cmd_type_enum cmd_type)
{
    int i = -1;
    tel_conn_mgr_ut_ril_at_cmd_rsp_struct *at_cmd_rsp = NULL;

    at_cmd_rsp = tel_conn_mgr_ut_ril_at_cmd_rsp1;
    
    if (!at_cmd_rsp && TEL_CONN_MGR_AT_CMD_TYPE_NONE != cmd_type)
    {
        return RIL_RESULT_CODE_NULL;
    }

    while (at_cmd_rsp[++i].cmd_type != TEL_CONN_MGR_AT_CMD_TYPE_NONE &&
           at_cmd_rsp[i].cmd_type != cmd_type);

    if (TEL_CONN_MGR_AT_CMD_TYPE_NONE == at_cmd_rsp[i].cmd_type )
    {
        return RIL_RESULT_CODE_NULL;
    }

    return at_cmd_rsp[i].rsp;        
}


void tel_conn_mgr_ut_ril_send_cereg_urc(tel_conn_mgr_nw_reg_status_struct *nw_reg_status)
{
    if (nw_reg_status && tel_conn_mgr_ut_ril_urc_callback)
    {
        ril_gprs_network_registration_status_urc_t cereg_urc = {0};
        
        cereg_urc.act = nw_reg_status->act;
        cereg_urc.stat = nw_reg_status->stat;
        tel_conn_mgr_ut_ril_urc_callback(RIL_URC_ID_CEREG, 
                                         (void *)&cereg_urc, 
                                         sizeof(cereg_urc));
    }
    else
    {
        TEL_CONN_MGR_LOG_ERR("Fail to send cgreg urc");
    }
}


void tel_conn_mgr_ut_ril_set_bearer_status(int cid, tel_conn_mgr_bool act)
{
    int i = -1;

    for (i = 0; i < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; i++)
    {
        if (tel_conn_mgr_ut_ril_bearer_state[i].cid == cid)
        {
            tel_conn_mgr_ut_ril_bearer_state[i].state = act ? 1 : 0;
            break;
        }
    }

    if (i >= TEL_CONN_MGR_BEARER_TYPE_MAX_NUM)
    {
        for (i = 0; i < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; i++)
        {
            tel_conn_mgr_ut_ril_bearer_state[i].cid = cid;
            tel_conn_mgr_ut_ril_bearer_state[i].state = act ? 1 : 0;
            break;
        }
    }
}


void tel_conn_mgr_ut_ril_send_cgev_urc(ril_urc_cgev_type_enum cgev_urc_type, int cid)
{
    if (tel_conn_mgr_ut_ril_urc_callback)
    {
        ril_cgev_event_reporting_urc_t cgev_urc = {0};
        cgev_urc.response_type = cgev_urc_type;
        cgev_urc.response1.cid = cid;
        //cgev_urc.response2.reason = 0;

        cgev_urc.response2.reason = RIL_OMITTED_INTEGER_PARAM;

        tel_conn_mgr_ut_ril_set_bearer_status(cid, RIL_URC_TYPE_ME_PDN_ACT == cgev_urc_type);

        tel_conn_mgr_ut_ril_urc_callback(RIL_URC_ID_CGEV, 
                                         (void *)&cgev_urc, 
                                         sizeof(cgev_urc));
    }
    else
    {
        TEL_CONN_MGR_LOG_ERR("Fail to send cgev urc");
    }
}


void tel_conn_mgr_ut_ril_at_cmd_req_msg_hdlr(tel_conn_mgr_ut_ril_msg_struct *msg)
{
    ril_cmd_response_t response = {0};
    
    if (!msg || !msg->user_data || !msg->callback)
    {
        TEL_CONN_MGR_LOG_INFO("Invalid param");
        return;
    }

    TEL_CONN_MGR_LOG_INFO("%s, cmd_type:%d, user_data:%x", __FUNCTION__, msg->cmd_type, (unsigned int)msg->user_data);

    memset(&response, 0, sizeof(ril_cmd_response_t));
    
    response.mode = RIL_EXECUTE_MODE;
    response.res_code = tel_conn_mgr_ut_ril_get_rsp_by_cmd_type(msg->cmd_type);
    response.user_data = msg->user_data;
    
    switch (msg->cmd_type)
    {
        case TEL_CONN_MGR_AT_CMD_TYPE_CEREG:
        {
            if (tel_conn_mgr_ut_ril_urc_before_rsp)
            {
                tel_conn_mgr_ut_ril_send_cereg_urc(&tel_conn_mgr_ut_ril_cereg_ok);                
            }
            
            response.cmd_id = RIL_CMD_ID_CEREG;
            msg->callback(&response);

            if (!tel_conn_mgr_ut_ril_urc_before_rsp)
            {
                tel_conn_mgr_ut_ril_send_cereg_urc(&tel_conn_mgr_ut_ril_cereg_ok);                
            }
            
            break;
        }

#ifdef TEL_CONN_MGR_ENABLE_CHECK_CEREG
        case TEL_CONN_MGR_AT_CMD_TYPE_CEREG_QUERY:
        {
            ril_eps_network_registration_status_rsp_t param = {0};

            param.n = tel_conn_mgr_ut_cereg_query_rsp->n;
            param.stat = tel_conn_mgr_ut_cereg_query_rsp->stat;
            param.act = tel_conn_mgr_ut_cereg_query_rsp->act;

            response.cmd_id = RIL_CMD_ID_CEREG;
            response.mode = RIL_READ_MODE;
            response.cmd_param = &param;
            msg->callback(&response);            

            vTaskDelay(100);
            tel_conn_mgr_ut_ril_send_cereg_urc(&tel_conn_mgr_ut_ril_cereg_ok);
            break;
        }
#endif

        case TEL_CONN_MGR_AT_CMD_TYPE_CGACT_QUERY:
        {
            ril_pdp_context_activate_or_deactivate_rsp_t param = {0};
            int i = 0;
            ril_cid_state_struct_t cid_state[TEL_CONN_MGR_BEARER_TYPE_MAX_NUM];
            ril_result_code_t code = tel_conn_mgr_ut_ril_get_rsp_by_cmd_type(msg->cmd_type);

            if (RIL_RESULT_CODE_OK == code)
            {
                param.array_num = 0;
                
                for (i = 0; i < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; i++)
                {
                    if (tel_conn_mgr_is_cid_valid(tel_conn_mgr_ut_ril_bearer_state[i].cid) &&
                        !tel_conn_mgr_is_special_cid(tel_conn_mgr_ut_ril_bearer_state[i].cid))
                    {
                        cid_state[param.array_num].cid = tel_conn_mgr_ut_ril_bearer_state[i].cid;
                        cid_state[param.array_num].state = tel_conn_mgr_ut_ril_bearer_state[i].state;
                        param.array_num++;
                        TEL_CONN_MGR_LOG_INFO("cid_state[%d].cid:%d, cid_state[%d].state:%d",
                                              param.array_num, cid_state[param.array_num].cid,
                                              param.array_num, cid_state[param.array_num].state);
                    }
                }
                
                if (param.array_num)
                {
                    param.cid_state = cid_state;
                }

                TEL_CONN_MGR_LOG_INFO("cid_num:%d", param.array_num);
                
                response.cmd_id = RIL_CMD_ID_CGACT;
                response.res_code = code;
                response.mode = RIL_READ_MODE;
                response.cmd_param = &param;            
                msg->callback(&response);
                
            }
            else
            {
                response.cmd_id = RIL_CMD_ID_CGACT;
                response.res_code = code;
                response.mode = RIL_READ_MODE;
                msg->callback(&response);
            }
            break;
        }

        case TEL_CONN_MGR_AT_CMD_TYPE_CFUN_ON:
        {
            response.cmd_id = RIL_CMD_ID_CFUN;
            msg->callback(&response);
            
            break;
        }
            
        case TEL_CONN_MGR_AT_CMD_TYPE_CGEREP:
        {
            response.cmd_id = RIL_CMD_ID_CGEREP;
            msg->callback(&response);
            
            break;
        }
        
        case TEL_CONN_MGR_AT_CMD_TYPE_CGDCONT:
        {
            response.cmd_id = RIL_CMD_ID_CGDCONT;
            msg->callback(&response);
            
            break;
        }
        
        case TEL_CONN_MGR_AT_CMD_TYPE_CGAUTH:
        {
            response.cmd_id = RIL_CMD_ID_CGAUTH;
            msg->callback(&response);
            
            break;
        }

        case TEL_CONN_MGR_AT_CMD_TYPE_CGACT_DEACT:
        case TEL_CONN_MGR_AT_CMD_TYPE_CGDATA:
        {
            ril_urc_cgev_type_enum cgev_urc_type = RIL_URC_TYPE_ME_PDN_ACT;

            if (TEL_CONN_MGR_AT_CMD_TYPE_CGACT_DEACT == msg->cmd_type)
            {
                cgev_urc_type = RIL_URC_TYPE_ME_PDN_DEACT;
            }
            
            if (tel_conn_mgr_ut_ril_urc_before_rsp)
            {
                tel_conn_mgr_ut_ril_send_cgev_urc(cgev_urc_type, msg->cid);
            }

            if (TEL_CONN_MGR_AT_CMD_TYPE_CGACT_DEACT == msg->cmd_type)
            {
                response.cmd_id = RIL_CMD_ID_CGACT;
            }
            else
            {
                response.cmd_id = RIL_CMD_ID_CGDATA;
            }
            msg->callback(&response);            

            if (!tel_conn_mgr_ut_ril_urc_before_rsp)
            {
                tel_conn_mgr_ut_ril_send_cgev_urc(cgev_urc_type, msg->cid);
            }
            break;   
        }

        case TEL_CONN_MGR_AT_CMD_TYPE_CGCONTRDP:
        {
            ril_pdp_context_read_dynamic_parameters_rsp_t param = {0};
            ril_pdp_context_entry_struct_t pdp_context[2] = {0};

            param.pdp_context = pdp_context;
            pdp_context[0].cid = msg->cid;
            pdp_context[0].local_addr_and_subnet_mask = "192.168.0.2.255.255.255.0";
            pdp_context[0].gw_addr = "192.168.0.1";
            pdp_context[0].dns_prim_addr = "192.168.0.1";
            pdp_context[0].dns_sec_addr = "0.0.0.0";
            pdp_context[1].cid = msg->cid;
            pdp_context[1].local_addr_and_subnet_mask = "0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.5.255.255.255.255.255.255.255.255.255.255.255.255.255.255.255.0";
            pdp_context[1].gw_addr = "2000::1";
            pdp_context[1].dns_prim_addr = "2000::1";
            pdp_context[1].dns_sec_addr = "::";

            param.array_num = 2;

            response.cmd_id = RIL_CMD_ID_CGCONTRDP;
            response.cmd_param = &param;
            msg->callback(&response);            
            break;
        }

        default:
            break;
    }
}

void tel_conn_mgr_ut_ril_task_main(void *param)
{
    tel_conn_mgr_ut_ril_msg_struct *msg = NULL;

    TEL_CONN_MGR_LOG_INFO("ril_ut task is running. queue_hdl:%x, task_hdl:%s",
                          tel_conn_mgr_ut_ril_task_queue_hdl, tel_conn_mgr_ut_ril_task_hdl);
    
    while (tel_conn_mgr_ut_ril_task_queue_hdl)
    {
        if (tel_conn_mgr_queue_receive(tel_conn_mgr_ut_ril_task_queue_hdl, &msg, TEL_CONN_MAX_PORT_MAX_DELAY))
        {
            switch (msg->msg_id)
            {
                case MSG_ID_TEL_CONN_MGR_UT_RIL_SEND_AT_CMD_REQ:
                {
                    tel_conn_mgr_ut_ril_at_cmd_req_msg_hdlr(msg);
                    break;
                }                
                
                default:
                    break;
            }

            tel_conn_mgr_free(msg);            
            msg = NULL;
        }

        vTaskDelay(10);
    }

    tel_conn_mgr_ut_task_queue_free(tel_conn_mgr_ut_ril_task_queue_hdl);

    tel_conn_mgr_ut_ril_task_hdl = NULL;
    tel_conn_mgr_task_delete(NULL);
}


void tel_conn_mgr_ut_ril_send_msg_to_ril_task(tel_conn_mgr_at_cmd_type_enum cmd_type,
                                                             int cid, 
                                                             int data_channel, 
                                                             void *user_data,
                                                             void *callback)
{
    tel_conn_mgr_ut_ril_msg_struct *msg = tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_ut_ril_msg_struct));
    tel_conn_mgr_bool ret = TEL_CONN_MGR_FALSE;

    TEL_CONN_MGR_LOG_INFO("cmd_type:%d, cid:%d, data_channel:%d, user_data:%x", cmd_type, cid, data_channel, user_data);
    if (!msg)
    {
        TEL_CONN_MGR_LOG_ERR("Fail to allocate memory for msg.");
        return;
    }
    
    msg->msg_id = MSG_ID_TEL_CONN_MGR_UT_RIL_SEND_AT_CMD_REQ;
    msg->cid = cid;
    msg->data_channel = data_channel;
    msg->user_data = user_data;
    msg->cmd_type = cmd_type;
    msg->callback = (ril_cmd_response_callback_t)callback;
    ret = tel_conn_mgr_send_msg(tel_conn_mgr_ut_ril_task_queue_hdl, (void *)msg);
    if (!ret)
    {
        TEL_CONN_MGR_LOG_ERR("Fail to send msg to ril task. msg queue hdl:%x", tel_conn_mgr_ut_ril_task_queue_hdl);
    }
}


ril_status_t ril_request_define_pdp_context_ut(ril_request_mode_t mode,
                                                       ril_pdp_context_entity_t *req,
                                                       ril_cmd_response_callback_t callback, 
                                                       void* user_data,
                                                       int32_t channel_id)
{

    tel_conn_mgr_ut_ril_send_msg_to_ril_task(TEL_CONN_MGR_AT_CMD_TYPE_CGDCONT,
                                                req->cid, channel_id, user_data, (void *)callback);

    return RIL_STATUS_SUCCESS;
}



ril_status_t ril_request_define_pdp_context_authentication_parameters_ut(ril_request_mode_t mode,
                                                                int32_t cid,
                                                                int32_t auth_port,
                                                                char* userid,
                                                                char* password,
                                                                ril_cmd_response_callback_t callback, 
                                                                void* user_data,
                                                                int32_t channel_id)
{
    tel_conn_mgr_ut_ril_send_msg_to_ril_task(TEL_CONN_MGR_AT_CMD_TYPE_CGAUTH,
                                                cid, channel_id, user_data, (void *)callback);

    return RIL_STATUS_SUCCESS;
}


ril_status_t ril_request_enter_data_state_ut(ril_request_mode_t mode,
                                                ril_enter_data_state_req_t *req,
                                                ril_cmd_response_callback_t callback, 
                                                void* user_data,
                                                int32_t channel_id)
{
    tel_conn_mgr_ut_ril_send_msg_to_ril_task(TEL_CONN_MGR_AT_CMD_TYPE_CGDATA,
                                                req->cid_array[0], channel_id, user_data, (void *)callback);

    return RIL_STATUS_SUCCESS;
}


ril_status_t ril_request_eps_network_registration_status_ut(ril_request_mode_t mode,
                                                                       int32_t n,
                                                                       ril_cmd_response_callback_t callback, 
                                                                       void* user_data)
{
    if (RIL_READ_MODE == mode)
    {
        tel_conn_mgr_ut_ril_send_msg_to_ril_task(TEL_CONN_MGR_AT_CMD_TYPE_CEREG_QUERY,
                                                    -1, -1, user_data, (void *)callback);
    }
    else
    {
        tel_conn_mgr_ut_ril_send_msg_to_ril_task(TEL_CONN_MGR_AT_CMD_TYPE_CEREG,
                                                    -1, -1, user_data, (void *)callback);
    }
    

    return RIL_STATUS_SUCCESS;
}


ril_status_t ril_request_packet_domain_event_reporting_ut(ril_request_mode_t mode,
                                                                      int32_t cmode,
                                                                      int32_t bfr,
                                                                      ril_cmd_response_callback_t callback, 
                                                                      void* user_data)
{
    tel_conn_mgr_ut_ril_send_msg_to_ril_task(TEL_CONN_MGR_AT_CMD_TYPE_CGEREP,
                                                -1, -1, user_data, (void *)callback);

    return RIL_STATUS_SUCCESS;
}


ril_status_t ril_request_set_phone_functionality_ut(ril_request_mode_t mode,
                                                            int32_t cmode,
                                                            int32_t bfr,
                                                            ril_cmd_response_callback_t callback, 
                                                            void* user_data)
{
    tel_conn_mgr_ut_ril_send_msg_to_ril_task(TEL_CONN_MGR_AT_CMD_TYPE_CFUN_ON,
                                                -1, -1, user_data, (void *)callback);

    return RIL_STATUS_SUCCESS;
}



ril_status_t ril_request_pdp_context_activate_or_deactivate_ut(ril_request_mode_t mode,
                                                                           ril_pdp_context_activate_or_deactivate_req_t *req,
                                                                           ril_cmd_response_callback_t callback, 
                                                                           void* user_data)
{
    if (RIL_READ_MODE == mode)
    {
        tel_conn_mgr_ut_ril_send_msg_to_ril_task(TEL_CONN_MGR_AT_CMD_TYPE_CGACT_QUERY,
                                                    -1, -1, user_data, (void *)callback);    
    }
    else
    {
        tel_conn_mgr_ut_ril_send_msg_to_ril_task(TEL_CONN_MGR_AT_CMD_TYPE_CGACT_DEACT,
                                                     req->cid_array[0], -1, user_data, (void *)callback);    
    }
    
    return RIL_STATUS_SUCCESS;
}


ril_status_t ril_request_pdp_context_read_dynamic_parameters_ut(ril_request_mode_t mode,
                                                                               int32_t cid,
                                                                               ril_cmd_response_callback_t callback, 
                                                                               void* user_data)
{    
    tel_conn_mgr_ut_ril_send_msg_to_ril_task(TEL_CONN_MGR_AT_CMD_TYPE_CGCONTRDP,
                                                cid, -1, user_data, (void *)callback);

    return RIL_STATUS_SUCCESS;
}


ril_status_t ril_register_event_callback_ut(uint32_t group_mask, ril_event_callback_t callback)
{
    tel_conn_mgr_ut_ril_urc_callback = callback;
    
    return RIL_STATUS_SUCCESS;
}


ril_status_t ril_deregister_event_callback_ut(ril_event_callback_t callback)
{
    tel_conn_mgr_ut_ril_urc_callback = NULL;

    return RIL_STATUS_SUCCESS;
}

#endif /* TEL_CONN_MGR_UT */


/*****************************APP*****************************/
tel_conn_mgr_bool tel_conn_mgr_ut_app_task1_is_running = TEL_CONN_MGR_FALSE;
tel_conn_mgr_task_hdl_t tel_conn_mgr_ut_app_task1_hdl = NULL;
tel_conn_mgr_queue_hdl_t tel_conn_mgr_ut_app_task1_queue_hdl = NULL;
tel_conn_mgr_bool tel_conn_mgr_ut_app_task2_is_running = TEL_CONN_MGR_FALSE;
tel_conn_mgr_task_hdl_t tel_conn_mgr_ut_app_task2_hdl = NULL;
tel_conn_mgr_queue_hdl_t tel_conn_mgr_ut_app_task2_queue_hdl = NULL;
int end_func_enter_time = 0;
#ifdef TEL_CONN_MGR_APB_SUPPORT
unsigned int tel_conn_mgr_ut_apb_cmd_id = 0;
#endif

typedef struct
{
    int msg_id;
    int test_case_num;
    int data; /* used as result for MSG_ID_TEL_CONN_MGR_UT_APP_TEST_CASE_END and
                               used as app_id idx for MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ */
    int app_pdp_type;
    char app_apn[TEL_CONN_MGR_APN_MAX_LEN];
}tel_conn_mgr_ut_app_msg_struct;


typedef struct
{
    int act_api_ret;
    int deact_api_ret;
    int act_api_msg_result;
    int deact_api_msg_result;
    int test_case_num;
}tel_conn_mgr_ut_app_result_struct;


#if 0
/* Test in MFI Lab */
#define TEL_CONN_MGR_UT_APN1 "internet"
#define TEL_CONN_MGR_UT_USERNAME1 "web"
#define TEL_CONN_MGR_UT_PASSWORD1 "password"


#define TEL_CONN_MGR_UT_APN2 "Net"
#define TEL_CONN_MGR_UT_USERNAME2 "web"
#define TEL_CONN_MGR_UT_PASSWORD2 "password"
#else
/* Test for CT SIM Card */
#define TEL_CONN_MGR_UT_APN1 "ctnet"
#define TEL_CONN_MGR_UT_USERNAME1 "ctnet@mycdma.cn"
#define TEL_CONN_MGR_UT_PASSWORD1 "vnet.mobi"

#define TEL_CONN_MGR_UT_APN2 "internet"
#define TEL_CONN_MGR_UT_USERNAME2 "web"
#define TEL_CONN_MGR_UT_PASSWORD2 "password"
#endif


void tel_conn_mgr_ut_app_send_msg_to_app_task(int msg_id,
                                                             int test_case_num,
                                                             int data, /* result or app_id idx */
                                                             tel_conn_mgr_pdp_type_enum app_pdp_type,
                                                             char *app_apn,
                                                             tel_conn_mgr_queue_hdl_t queue_hdl)
{
    tel_conn_mgr_ut_app_msg_struct *msg = tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_ut_app_msg_struct));

    if (!queue_hdl || !msg)
    {
        return;
    }

    msg->msg_id = msg_id;
    msg->test_case_num = test_case_num;
    msg->data = data;    
    msg->app_pdp_type = app_pdp_type;
    if (app_apn)
    {
        strncpy(msg->app_apn, app_apn, TEL_CONN_MGR_APN_MAX_LEN - 1);
    }

    tel_conn_mgr_send_msg(queue_hdl, (void *)msg); 
}


void tel_conn_mgr_ut_app_send_msg_to_app_task1(int msg_id,
                                               int test_case_num,
                                               int data, /* result or app_id idx*/
                                               tel_conn_mgr_pdp_type_enum app_pdp_type,
                                               char *app_apn)
{
    tel_conn_mgr_ut_app_send_msg_to_app_task(msg_id,
                                             test_case_num,
                                             data,
                                             app_pdp_type,
                                             app_apn,
                                             tel_conn_mgr_ut_app_task1_queue_hdl);
}

void tel_conn_mgr_ut_app_send_msg_to_app_task2(int msg_id,
                                               int test_case_num,
                                               int data, /* result or app_id idx*/
                                               tel_conn_mgr_pdp_type_enum app_pdp_type,
                                               char *app_apn)                                                             
{
    tel_conn_mgr_ut_app_send_msg_to_app_task(msg_id,
                                             test_case_num,
                                             data,
                                             app_pdp_type,
                                             app_apn,
                                             tel_conn_mgr_ut_app_task2_queue_hdl);
}


tel_conn_mgr_bool tel_conn_mgr_ut_is_test_ready(void)
{
    if (tel_conn_mgr_ut_app_task1_is_running && 
        tel_conn_mgr_ut_app_task2_is_running)
    {
        return TEL_CONN_MGR_TRUE;
    }

    return TEL_CONN_MGR_FALSE;
}


#ifdef TEL_CONN_MGR_APB_SUPPORT
void tel_conn_mgr_ut_set_apb_cmd_id(unsigned int apb_cmd_id)
{
    tel_conn_mgr_ut_apb_cmd_id = apb_cmd_id;
}
#endif


void tel_conn_mgr_ut_test_case_run(int test_case_num)
{
    tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_TEST_CASE_START,
                                              test_case_num,
                                              -1,
                                              TEL_CONN_MGR_PDP_TYPE_NONE,
                                              NULL);

    tel_conn_mgr_ut_app_send_msg_to_app_task2(MSG_ID_TEL_CONN_MGR_UT_APP_TEST_CASE_START,
                                              test_case_num,
                                              -1,
                                              TEL_CONN_MGR_PDP_TYPE_NONE,
                                              NULL);
}


void tel_conn_mgr_ut_test_case_end(int app_id, int test_case_num, int result)
{
    static tel_conn_mgr_bool test_end_printed = TEL_CONN_MGR_FALSE;
#ifdef TEL_CONN_MGR_APB_SUPPORT
    apb_proxy_at_cmd_result_t apb_urc = {0};
    char apb_urc_string[25] = {0};
#endif
    if (!end_func_enter_time)
    {
        /* New test case. Reset the value. */
        test_end_printed = TEL_CONN_MGR_FALSE;
    }

    end_func_enter_time++;
    
    TEL_CONN_MGR_LOG_INFO("app_id:%d, test_case_num:%d, result:%d, enter_time:%d, printed:%d",
                          app_id, test_case_num, result, end_func_enter_time, test_end_printed);

    if (test_end_printed)
    {
        TEL_CONN_MGR_LOG_WARN("line:%d", __LINE__);
        return;
    }

    if (5 == test_case_num ||
        7 == test_case_num)
    {
        if (!result || 2 == end_func_enter_time)
        {
            TEL_CONN_MGR_LOG_WARN("line:%d result:%d app_id:%d", __LINE__, result, app_id);
            goto exit;
        }

        TEL_CONN_MGR_LOG_WARN("line:%d", __LINE__);
        return;
    }
    
exit:
    TEL_CONN_MGR_LOG_INFO("*************Test %d End. result:%d********************",
                          test_case_num, result);
    test_end_printed = TEL_CONN_MGR_TRUE;
    TEL_CONN_MGR_LOG_INFO("printed:%d", test_end_printed);

#ifdef TEL_CONN_MGR_APB_SUPPORT
    apb_urc.result_code = APB_PROXY_RESULT_UNSOLICITED;
    /* +TCMTEST:<test case num>,<result> */
    sprintf(apb_urc_string, "%d,%d", test_case_num, result);
    apb_urc.pdata = apb_urc_string;
    apb_urc.length = strlen(apb_urc.pdata) + 1;
    apb_urc.cmd_id = tel_conn_mgr_ut_apb_cmd_id;
    tel_conn_mgr_ut_set_apb_cmd_id(0);
    apb_proxy_send_at_cmd_result(&apb_urc);
#endif
}


void tel_conn_mgr_ut_app_task1_main(void *param)
{
    tel_conn_mgr_msg_struct *msg = NULL;
    tel_conn_mgr_ut_app_msg_struct *app_msg = NULL;
    unsigned int app_id[6] = {0}, test_case_num = 0;;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_OK;
    tel_conn_mgr_pdp_type_enum activated_pdp_type[5] = {TEL_CONN_MGR_PDP_TYPE_NONE};
    int act_req_time = 0, deact_req_time = 0, act_rsp_time = 0;

    TEL_CONN_MGR_LOG_INFO("app_task1 is running. queue_hdl:%x, task_hdl:%x",
                          (unsigned int)tel_conn_mgr_ut_app_task1_queue_hdl, (unsigned int)tel_conn_mgr_ut_app_task1_hdl);

    tel_conn_mgr_ut_app_task1_is_running = TEL_CONN_MGR_TRUE;

#ifdef TEL_CONN_MGR_UT
    while (!tel_conn_mgr_ut_is_test_ready())
    {
        TEL_CONN_MGR_LOG_INFO("Wait for test ready.");
        vTaskDelay(100);
    }
#endif

    test_case_num = (int)param;
    if (test_case_num)
    {
        TEL_CONN_MGR_LOG_INFO("Run test case:%d after task creation.", test_case_num);
        tel_conn_mgr_ut_test_case_run(test_case_num);
    }
   
    
    while (tel_conn_mgr_ut_app_task1_queue_hdl)
    {
        if (tel_conn_mgr_queue_receive(tel_conn_mgr_ut_app_task1_queue_hdl, &msg, TEL_CONN_MAX_PORT_MAX_DELAY))
        {
            app_msg = (tel_conn_mgr_ut_app_msg_struct *)msg;
            switch (msg->msg_id)
            {
                case MSG_ID_TEL_CONN_MGR_UT_APP_TEST_CASE_START:
                {
                    act_req_time = 0;
                    deact_req_time = 0;
                    act_rsp_time = 0;
                    end_func_enter_time = 0;
                    test_case_num = ((tel_conn_mgr_ut_app_msg_struct *)msg)->test_case_num;
                    TEL_CONN_MGR_LOG_INFO("*************Test %d Start********************", test_case_num);
                    switch (test_case_num)
                    {
                        case 8:
                        {
                            /* Activate with invalid parameter(s) */
                            tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_ACT_REQ,
                                                                      test_case_num,
                                                                      -1,
                                                                      TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                      TEL_CONN_MGR_UT_APN1);
                            break;
                        }

                        case 9:
                        {
                            /* Dectivate with invalid parameter(s) */
                            tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                      test_case_num,
                                                                      -1,
                                                                      TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                      TEL_CONN_MGR_UT_APN1);
                            break;
                        }
                        
                        case 1:
                        case 2:
                        case 3:
                        case 10:
                        case 11:
                        case 5:
                        case 12:
                        case 6:
                        case 13:
                        case 7:
                        case 4:
                        case 17:
                        case 18:
                        {
                            /* Activate with valid parameter(s) */
                            tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_ACT_REQ,
                                                                      test_case_num,
                                                                      -1,
                                                                      TEL_CONN_MGR_PDP_TYPE_IP,
                                                                      TEL_CONN_MGR_UT_APN1);
                            if (3 == test_case_num ||
                                10 == test_case_num ||
                                11 == test_case_num ||
                                18 == test_case_num) 
                            {
                                /* Activate the same PDP context */
                                tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_ACT_REQ,
                                                                          test_case_num,
                                                                          -1,
                                                                          TEL_CONN_MGR_PDP_TYPE_IP,
                                                                          TEL_CONN_MGR_UT_APN1);
                            }

                            if (18 == test_case_num)
                            {
                                tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                          test_case_num,
                                                                          0,
                                                                          TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                          NULL);

                                tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_ACT_REQ,
                                                                          test_case_num,
                                                                          -1,
                                                                          TEL_CONN_MGR_PDP_TYPE_IP,
                                                                          TEL_CONN_MGR_UT_APN1);
                            }

                            //if (17 == test_case_num)
                            if (0)
                            {
                                tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                          test_case_num,
                                                                          0,
                                                                          TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                          NULL);

                                vTaskDelay(50);
                                tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_ACT_REQ,
                                                                          test_case_num,
                                                                          -1,
                                                                          TEL_CONN_MGR_PDP_TYPE_IPV6,
                                                                          TEL_CONN_MGR_UT_APN1);
                            }

                            if (6 == test_case_num)
                            {
                                /* A different PDP context */
                                tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_ACT_REQ,
                                                                          test_case_num,
                                                                          -1,
                                                                          TEL_CONN_MGR_PDP_TYPE_IP,
                                                                          TEL_CONN_MGR_UT_APN2);
                            } 
                                                        
                            if (10 == test_case_num ||
                                11 == test_case_num)
                            {
                                vTaskDelay(10);
                                tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                          test_case_num,
                                                                          1,
                                                                          TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                          NULL);
                            }
                             
                            if (2 == test_case_num ||
                                10 == test_case_num ||
                                7 == test_case_num)
                            {
                                vTaskDelay(10);
                                tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                          test_case_num,
                                                                          0,
                                                                          TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                          NULL);
                            }
                            break;
                        }

                        case 14:
                        case 15:
                        case 16:
                        {
                            int max_app = 5;
                            if (15 == test_case_num)
                            {
                                max_app = 6;
                            }
                            
                            for (int i = 0; i < max_app; i++)
                            {
                                tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_ACT_REQ,
                                                                          test_case_num,
                                                                          -1,
                                                                          TEL_CONN_MGR_PDP_TYPE_IP,
                                                                          TEL_CONN_MGR_UT_APN2);
                            }

                            if (16 == test_case_num)
                            {
                                max_app = 0;
                                tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_ACT_REQ,
                                                                          test_case_num,
                                                                          -1,
                                                                          TEL_CONN_MGR_PDP_TYPE_IP,
                                                                          TEL_CONN_MGR_UT_APN1);
                            }
                            else if (14 == test_case_num)
                            {
                                max_app = 5;
                            }
                            else
                            {
                                max_app = 4;
                            }

                            for (int i = 0; i < max_app; i++)
                            {
                                tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                          test_case_num,
                                                                          i,
                                                                          TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                          NULL);
                            }
                            
                            break;
                        }
                        
                        default:
                            break;
                    }
                    break;
                }

                case MSG_ID_TEL_CONN_MGR_UT_APP_TEST_CASE_END:
                {
                    tel_conn_mgr_ut_test_case_end(1, test_case_num, app_msg->data);
                    break;
                }
                
                case MSG_ID_TEL_CONN_MGR_UT_APP_ACT_REQ:
                {
                    int result = 1, test_case_end = 0;
                    
                    /* Activate */
                    if (0 == strcmp(app_msg->app_apn, TEL_CONN_MGR_UT_APN1))
                    {
                        ret = tel_conn_mgr_activate(TEL_CONN_MGR_BEARER_TYPE_NBIOT,
                                                    TEL_CONN_MGR_SIM_ID_1,
                                                    app_msg->app_pdp_type,
                                                    app_msg->app_apn,
                                                    TEL_CONN_MGR_UT_USERNAME1,
                                                    TEL_CONN_MGR_UT_PASSWORD1,
                                                    tel_conn_mgr_ut_app_task1_queue_hdl,
                                                    &app_id[act_req_time],
                                                    (tel_conn_mgr_pdp_type_enum *)&activated_pdp_type[act_req_time]);
                    }
                    else
                    {
                        ret = tel_conn_mgr_activate(TEL_CONN_MGR_BEARER_TYPE_NBIOT,
                                                    TEL_CONN_MGR_SIM_ID_1,
                                                    app_msg->app_pdp_type,
                                                    app_msg->app_apn,
                                                    TEL_CONN_MGR_UT_USERNAME2,
                                                    TEL_CONN_MGR_UT_PASSWORD2,
                                                    tel_conn_mgr_ut_app_task1_queue_hdl,
                                                    &app_id[act_req_time],
                                                    (tel_conn_mgr_pdp_type_enum *)&activated_pdp_type[act_req_time]);
                    }
                    
                    TEL_CONN_MGR_LOG_INFO("[1]activate API ret:%d, app_id:%d, pdp_type:%d",
                        ret, app_id[act_req_time], activated_pdp_type[act_req_time]);
                    
                    if (TEL_CONN_MGR_RET_OK != ret &&
                        TEL_CONN_MGR_RET_WOULDBLOCK != ret)
                    {
                        result = 0;
                        test_case_end = 1;
                    }
                    
                    if (8 == test_case_num)
                    {
                        test_case_end = 1;
                        if (TEL_CONN_MGR_RET_OK != ret &&
                            TEL_CONN_MGR_RET_WOULDBLOCK != ret)
                        {
                            result = 1;
                        }
                    }
                    else if (12 == test_case_num)
                    {
                        if (1 == act_req_time && result)
                        {
                            tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                      test_case_num,
                                                                      0,
                                                                      TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                      NULL);
                        
                        
                            vTaskDelay(10);
                            tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                      test_case_num,
                                                                      1,
                                                                      TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                      NULL);
                        
                        }
                    }
                    else if (15 == test_case_num)
                    {
                        if (5 == act_req_time && !result)
                        {
                            test_case_end = 0;
                        }
                    }                    
                    
                    if (test_case_end)
                    {
                        tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_TEST_CASE_END,
                                                                  app_msg->test_case_num,
                                                                  result,
                                                                  TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                  NULL);                        
                    }

                    act_req_time++;
                    break;
                }

                case MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ:
                {
                    int result = 0, test_case_end = 0, deact_app_id = 0;

                    /* Deactivate */
                    if (0 <= app_msg->data &&
                        6 > app_msg->data)
                    {
                        deact_app_id = app_id[app_msg->data];
                    }
                    else
                    {
                        /* Invalid app_id for test case 2. */
                        deact_app_id = 110;
                    }
                    
                    ret = tel_conn_mgr_deactivate(deact_app_id);
                    TEL_CONN_MGR_LOG_INFO("[1]deact_api app_id:%d, ret:%d", deact_app_id, ret);

                    if (TEL_CONN_MGR_RET_OK != ret &&
                        TEL_CONN_MGR_RET_WOULDBLOCK != ret)
                    {
                        test_case_end = 1;
                    }

                    if (9 == test_case_num)
                    {
                        test_case_end = 1;
                        if (TEL_CONN_MGR_RET_NOT_FOUND == ret)
                        {
                            result = 1;
                        }
                    }

                    if ((0 == deact_req_time &&
                        (3 == test_case_num ||
                         10 == test_case_num ||
                         11 == test_case_num ||
                         12 == test_case_num ||
                         13 == test_case_num)) ||
                         ((14 == test_case_num || 15 == test_case_num ||
                           16 == test_case_num) &&
                         0 <= deact_req_time && 3 >= deact_req_time))
                    {
                        if (TEL_CONN_MGR_RET_IS_HOLD == ret)
                        {
                            test_case_end = 0;
                        }
                    }
                    
                    if (test_case_end)
                    {
                        tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_TEST_CASE_END,
                                                                  test_case_num,
                                                                  result,
                                                                  TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                  NULL);                        
                    }

                    deact_req_time++;
                    break;
                }

                case MSG_ID_TEL_CONN_MGR_ACTIVATION_RSP:
                {
                    tel_conn_mgr_activation_rsp_struct *act_msg = (tel_conn_mgr_activation_rsp_struct *)msg;
                    int result = 0, test_case_end = 0;
                    
                    TEL_CONN_MGR_LOG_INFO("[1]msg id:%d", msg->msg_id);
                    TEL_CONN_MGR_LOG_INFO("[1]bearer_type:%d, sim_id:%d, pdp_type:%d", 
                                          act_msg->bearer_type,
                                          act_msg->sim_id,
                                          act_msg->pdp_type);
                    TEL_CONN_MGR_LOG_INFO("[1]apn:%s", act_msg->apn);
                    TEL_CONN_MGR_LOG_INFO("[1]result:%d, cause:%d", act_msg->result, act_msg->cause);
                    TEL_CONN_MGR_LOG_INFO("[1]app_id[0]:%d, app_id[1]:%d, app_id[2]:%d, app_id[3]:%d, app_id[4]:%d",
                                          act_msg->app_id[0], act_msg->app_id[1], 
                                          act_msg->app_id[2], act_msg->app_id[3],
                                          act_msg->app_id[4]);
                    
                    result = act_msg->result;
                    if (!result)
                    {
                        test_case_end = 1;
                    }
                    
                    if (result && (1 == test_case_num ||
                        11 == test_case_num ||
                        13 == test_case_num ||
                        4 == test_case_num ||
                        5 == test_case_num))
                    {
                        tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                  test_case_num,
                                                                  0,
                                                                  TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                  NULL);
                    }
                    else if (15 == test_case_num)
                    {
                        if (result && 4 == act_rsp_time)
                        {
                            tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                      test_case_num,
                                                                      act_rsp_time,
                                                                      TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                      NULL);
                        }
                        else if (0 <= act_rsp_time && 3 >= act_rsp_time)
                        {
                            if (result)
                            {
                                result = 0;
                                test_case_end = 1;
                            }
                            else
                            {
                                test_case_end = 0;
                            }
                        }
                        
                        TEL_CONN_MGR_LOG_INFO("[1]test_case_end:%d, result:%d", test_case_end, result);
                    }                    
                    else if (2 == test_case_num ||
                             10 == test_case_num ||
                             7 == test_case_num ||
                             (17 == test_case_num && (0 == act_rsp_time || 1 == act_rsp_time)) ||
                             (14 == test_case_num && 0 <= act_rsp_time && 3 >= act_rsp_time))
                    {
                        if (result)
                        {
                            result = 0;
                            test_case_end = 1;
                        }
                        else
                        {
                            test_case_end = 0;
                            if (17 == test_case_num && 1 == act_rsp_time)
                            {
                                tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_ACT_REQ,
                                                                          test_case_num,
                                                                          -1,
                                                                          TEL_CONN_MGR_PDP_TYPE_IPV6,
                                                                          TEL_CONN_MGR_UT_APN1);
                            }
                        }
                    }
                    else if (3 == test_case_num)
                    {
                        if (1 == act_rsp_time)
                        {
                            result = 0;
                            test_case_end = 1;
                        }
                        else if (result)
                        {
                            if (!act_msg->app_id[0] || !act_msg->app_id[1])
                            {
                                result = 0;
                                test_case_end = 1;
                            }
                            else
                            {
                                tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                          test_case_num,
                                                                          0,
                                                                          TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                          NULL);
                                
                                tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                          test_case_num,
                                                                          1,
                                                                          TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                          NULL);
                            }
                        }
                    }
                    else if (11 == test_case_num)
                    {
                        if (0 == act_rsp_time && !result)
                        {
                            test_case_end = 0;
                        }
                        else if (1 == act_rsp_time)
                        {
                            tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                      test_case_num,
                                                                      0,
                                                                      TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                      NULL);
                        }
                    }
                    else if (12 == test_case_num)
                    {
                        if (result)
                        {
                            tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_ACT_REQ,
                                                                      test_case_num,
                                                                      -1,
                                                                      TEL_CONN_MGR_PDP_TYPE_IP,
                                                                      TEL_CONN_MGR_UT_APN1);
                        }
                    }
                    else if (6 == test_case_num)
                    {
                        if (result)
                        {
                            if (0 == act_rsp_time)
                            {
                                tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                          test_case_num,
                                                                          0,
                                                                          TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                          NULL);
                            }
                            else
                            {
                                tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                          test_case_num,
                                                                          1,
                                                                          TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                          NULL);
                            }
                            
                        }
                    }
                    else if (result && 16 == test_case_num && 1 == act_rsp_time)
                    {
                        for (int i = 0; i < 6; i++)
                        {
                            tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                      test_case_num,
                                                                      i,
                                                                      TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                      NULL);
                        }
                    }                    
                                        
                    if (test_case_end)
                    {
                        tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_TEST_CASE_END,
                                                                  test_case_num,
                                                                  result,
                                                                  TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                  NULL);

                        
                    }                    

                    act_rsp_time++;
                    break;
                }

                case MSG_ID_TEL_CONN_MGR_DEACTIVATION_RSP:
                {
                    tel_conn_mgr_deactivation_rsp_struct *deact_msg = (tel_conn_mgr_deactivation_rsp_struct *)msg;
                    int result = 0;
                    
                    TEL_CONN_MGR_LOG_INFO("[1]msg id:%d", msg->msg_id);
                    TEL_CONN_MGR_LOG_INFO("[1]bearer_type:%d, sim_id:%d, pdp_type:%d", 
                                          deact_msg->bearer_type,
                                          deact_msg->sim_id,
                                          deact_msg->pdp_type);
                    TEL_CONN_MGR_LOG_INFO("[1]apn:%s", deact_msg->apn);
                    TEL_CONN_MGR_LOG_INFO("[1]result:%d, cause:%d", deact_msg->result, deact_msg->cause);
                    TEL_CONN_MGR_LOG_INFO("[1]app_id:%d", deact_msg->app_id);

                    result = deact_msg->result;

                    if (17 == test_case_num)
                    {
                        break;
                    }
                    
                    tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_TEST_CASE_END,
                                                              app_msg->test_case_num,
                                                              result,
                                                              TEL_CONN_MGR_PDP_TYPE_NONE,
                                                              NULL);
                    break;
                }

                case MSG_ID_TEL_CONN_MGR_DEACTIVATION_IND:
                {
                    tel_conn_mgr_deactivation_ind_struct *deact_ind = (tel_conn_mgr_deactivation_ind_struct *)msg;
                    TEL_CONN_MGR_LOG_INFO("[1]bearer_type:%d, sim_id:%d, pdp_type:%d", 
                                          deact_ind->bearer_type,
                                          deact_ind->sim_id,
                                          deact_ind->pdp_type);
                    TEL_CONN_MGR_LOG_INFO("[1]apn:%s", deact_ind->apn);
                    TEL_CONN_MGR_LOG_INFO("[1]cause:%d", deact_ind->cause);
                    TEL_CONN_MGR_LOG_INFO("[1]app_id[0]:%d, app_id[1]:%d, app_id[2]:%d, app_id[3]:%d, app_id[4]:%d",
                                          deact_ind->app_id[0], deact_ind->app_id[1], 
                                          deact_ind->app_id[2], deact_ind->app_id[3],
                                          deact_ind->app_id[4]);
                    break;
                }
                
                default:
                    break;
            }

            tel_conn_mgr_free(msg);
            msg = NULL;
        }
        vTaskDelay(10);
    }

    tel_conn_mgr_ut_task_queue_free(tel_conn_mgr_ut_app_task1_queue_hdl);

    tel_conn_mgr_ut_app_task1_hdl = NULL;
    tel_conn_mgr_task_delete(NULL);
}


void tel_conn_mgr_ut_app_task2_main(void *param)
{
    tel_conn_mgr_msg_struct *msg = NULL;
    tel_conn_mgr_ut_app_msg_struct *app_msg = NULL;
    unsigned int app_id[2] = {0}, test_case_num = 0;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_OK;
    tel_conn_mgr_pdp_type_enum activated_pdp_type[2] = {TEL_CONN_MGR_PDP_TYPE_NONE};
    int act_req_time = 0, deact_req_time = 0;

    TEL_CONN_MGR_LOG_INFO("app_task2 is running. queue_hdl:%x, task_hdl:%x",
                          (unsigned int)tel_conn_mgr_ut_app_task2_queue_hdl,
                          (unsigned int)tel_conn_mgr_ut_app_task2_hdl);    
    
    tel_conn_mgr_ut_app_task2_is_running = TEL_CONN_MGR_TRUE;
    
    while (tel_conn_mgr_ut_app_task2_queue_hdl)
    {
        if (tel_conn_mgr_queue_receive(tel_conn_mgr_ut_app_task2_queue_hdl, &msg, TEL_CONN_MAX_PORT_MAX_DELAY))
        {
            app_msg = (tel_conn_mgr_ut_app_msg_struct *)msg;
            switch (msg->msg_id)
            {
                case MSG_ID_TEL_CONN_MGR_UT_APP_TEST_CASE_START:
                {
                    act_req_time = 0;
                    deact_req_time = 0;
                    test_case_num = ((tel_conn_mgr_ut_app_msg_struct *)msg)->test_case_num;
                    TEL_CONN_MGR_LOG_INFO("*************Test %d Start********************", test_case_num);
                    switch (test_case_num)
                    {
                        case 5:
                        case 7:
                        {
                            tel_conn_mgr_ut_app_send_msg_to_app_task2(MSG_ID_TEL_CONN_MGR_UT_APP_ACT_REQ,
                                                                      test_case_num,
                                                                      -1,
                                                                      TEL_CONN_MGR_PDP_TYPE_IP,
                                                                      TEL_CONN_MGR_UT_APN2);
                            if (7 == test_case_num)
                            {
                                tel_conn_mgr_ut_app_send_msg_to_app_task2(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                      test_case_num,
                                                                      0,
                                                                      TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                      NULL);
                            }
                            break;
                        }

                        case 13:
                        case 4:
                        {
                            tel_conn_mgr_ut_app_send_msg_to_app_task2(MSG_ID_TEL_CONN_MGR_UT_APP_ACT_REQ,
                                                                      test_case_num,
                                                                      -1,
                                                                      TEL_CONN_MGR_PDP_TYPE_IP,
                                                                      TEL_CONN_MGR_UT_APN1);
                            if (4 == test_case_num)
                            {
                                tel_conn_mgr_ut_app_send_msg_to_app_task2(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                          test_case_num,
                                                                          0,
                                                                          TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                          NULL);
                            }
                            break;
                        }
                        
                        default:
                            break;
                    }
                    break;
                }

                case MSG_ID_TEL_CONN_MGR_UT_APP_TEST_CASE_END:
                {
                    tel_conn_mgr_ut_test_case_end(2, test_case_num, app_msg->data);                                       
                    break;
                }
                
                case MSG_ID_TEL_CONN_MGR_UT_APP_ACT_REQ:
                {
                    int result = 0, test_case_end = 0;
                    /* Activate */
                    if (0 == strcmp(app_msg->app_apn, TEL_CONN_MGR_UT_APN1))
                    {
                        ret = tel_conn_mgr_activate(TEL_CONN_MGR_BEARER_TYPE_NBIOT,
                                                    TEL_CONN_MGR_SIM_ID_1,
                                                    app_msg->app_pdp_type ,
                                                    app_msg->app_apn ,
                                                    TEL_CONN_MGR_UT_USERNAME1,
                                                    TEL_CONN_MGR_UT_PASSWORD1,
                                                    tel_conn_mgr_ut_app_task2_queue_hdl,
                                                    &app_id[act_req_time],
                                                    (tel_conn_mgr_pdp_type_enum *)&activated_pdp_type[act_req_time]);
                    }
                    else
                    {
                        ret = tel_conn_mgr_activate(TEL_CONN_MGR_BEARER_TYPE_NBIOT,
                                                    TEL_CONN_MGR_SIM_ID_1,
                                                    app_msg->app_pdp_type ,
                                                    app_msg->app_apn ,
                                                    TEL_CONN_MGR_UT_USERNAME2,
                                                    TEL_CONN_MGR_UT_PASSWORD2,
                                                    tel_conn_mgr_ut_app_task2_queue_hdl,
                                                    &app_id[act_req_time],
                                                    &activated_pdp_type[act_req_time]);
                    }
                    TEL_CONN_MGR_LOG_INFO("[2]activate API ret:%d, app_id:%d, pdp_type:%d",
                        ret, app_id[act_req_time], activated_pdp_type[act_req_time]);

                    if (TEL_CONN_MGR_RET_OK != ret &&
                        TEL_CONN_MGR_RET_WOULDBLOCK != ret)
                    {
                        test_case_end = 1;
                    }
                    
                    if (test_case_end)
                    {
                        tel_conn_mgr_ut_app_send_msg_to_app_task2(MSG_ID_TEL_CONN_MGR_UT_APP_TEST_CASE_END,
                                                                  app_msg->test_case_num,
                                                                  result,
                                                                  TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                  NULL);
                    }
                    
                    act_req_time++;
                    break;
                }

                case MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ:
                {
                    int result = 0, test_case_end = 0, deact_app_id = 400; 

                    /* Deactivate */
                    if (0 <= app_msg->data &&
                        5 > app_msg->data)
                    {
                        deact_app_id = app_id[app_msg->data];
                    }
                    else
                    {
                        /* Invalid app_id for test case 2. */
                        deact_app_id = 110;
                    }
                     
                    ret = tel_conn_mgr_deactivate(deact_app_id);
                    TEL_CONN_MGR_LOG_INFO("[2]deact_api app_id:%d, ret:%d", deact_app_id, ret);

                    if (TEL_CONN_MGR_RET_OK !=ret &&
                        TEL_CONN_MGR_RET_WOULDBLOCK != ret)
                    {
                        test_case_end = 1;
                    }

                    if (4 == test_case_num)
                    {
                        if (TEL_CONN_MGR_RET_IS_HOLD == ret)
                        {
                            test_case_end = 0;
                        }
                    }
                    
                    if (test_case_end)
                    {
                        tel_conn_mgr_ut_app_send_msg_to_app_task2(MSG_ID_TEL_CONN_MGR_UT_APP_TEST_CASE_END,
                                                                  test_case_num,
                                                                  result,
                                                                  TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                  NULL); 
                    }               
                    
                    deact_req_time++;
                    break;
                }

                case MSG_ID_TEL_CONN_MGR_ACTIVATION_RSP:
                {
                    tel_conn_mgr_activation_rsp_struct *act_msg = (tel_conn_mgr_activation_rsp_struct *)msg;
                    int result = 0, test_case_end = 0;
                    static int act_rsp_time = 0;
                    
                    TEL_CONN_MGR_LOG_INFO("[2]msg id:%d", msg->msg_id);
                    TEL_CONN_MGR_LOG_INFO("[2]bearer_type:%d, sim_id:%d, pdp_type:%d", 
                                          act_msg->bearer_type,
                                          act_msg->sim_id,
                                          act_msg->pdp_type);
                    TEL_CONN_MGR_LOG_INFO("[2]apn:%s", act_msg->apn);
                    TEL_CONN_MGR_LOG_INFO("[2]result:%d, cause:%d", act_msg->result, act_msg->cause);
                    TEL_CONN_MGR_LOG_INFO("[2]app_id[0]:%d, app_id[1]:%d, app_id[2]:%d, app_id[3]:%d, app_id[4]:%d",
                                          act_msg->app_id[0], act_msg->app_id[1], 
                                          act_msg->app_id[2], act_msg->app_id[3],
                                          act_msg->app_id[4]);
                    
                    result = act_msg->result;
                    if (!result)
                    {
                        test_case_end = 1;
                    }

                    if (5 == test_case_num ||
                        13 == test_case_num)
                    {
                        if (result)
                        {
                            tel_conn_mgr_ut_app_send_msg_to_app_task2(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                      test_case_num,
                                                                      0,
                                                                      TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                      NULL);
                        }
                    }
                    else if (7 == test_case_num ||
                             4 == test_case_num)
                    {
                        if (result)
                        {
                            result = 0;
                            test_case_end = 1;
                        }
                        else
                        {
                            result = 1;
                            test_case_end = 0;
                        }
                    }
                                        
                    if (test_case_end)
                    {
                        tel_conn_mgr_ut_app_send_msg_to_app_task2(MSG_ID_TEL_CONN_MGR_UT_APP_TEST_CASE_END,
                                                                  test_case_num,
                                                                  result,
                                                                  TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                  NULL);

                        
                    }                    

                    if (2 == act_rsp_time)
                    {
                        act_rsp_time = 0;
                    }
                    break;
                }

                case MSG_ID_TEL_CONN_MGR_DEACTIVATION_RSP:
                {
                    tel_conn_mgr_deactivation_rsp_struct *deact_msg = (tel_conn_mgr_deactivation_rsp_struct *)msg;
                    int result = 0;

                    TEL_CONN_MGR_LOG_INFO("[2]msg id:%d", msg->msg_id);
                    TEL_CONN_MGR_LOG_INFO("[2]bearer_type:%d, sim_id:%d, pdp_type:%d", 
                                          deact_msg->bearer_type,
                                          deact_msg->sim_id,
                                          deact_msg->pdp_type);
                    TEL_CONN_MGR_LOG_INFO("[2]apn:%s", deact_msg->apn);
                    TEL_CONN_MGR_LOG_INFO("[2]result:%d, cause:%d", deact_msg->result, deact_msg->cause);
                    TEL_CONN_MGR_LOG_INFO("[2]app_id:%d", deact_msg->app_id);

                    result = deact_msg->result;
                    
                    tel_conn_mgr_ut_app_send_msg_to_app_task2(MSG_ID_TEL_CONN_MGR_UT_APP_TEST_CASE_END,
                                                              app_msg->test_case_num,
                                                              result,
                                                              TEL_CONN_MGR_PDP_TYPE_NONE,
                                                              NULL);
                    break;
                }

                case MSG_ID_TEL_CONN_MGR_DEACTIVATION_IND:
                {
                    tel_conn_mgr_deactivation_ind_struct *deact_ind = (tel_conn_mgr_deactivation_ind_struct *)msg;
                    TEL_CONN_MGR_LOG_INFO("[2]bearer_type:%d, sim_id:%d, pdp_type:%d", 
                                          deact_ind->bearer_type,
                                          deact_ind->sim_id,
                                          deact_ind->pdp_type);
                    TEL_CONN_MGR_LOG_INFO("[2]apn:%s", deact_ind->apn);
                    TEL_CONN_MGR_LOG_INFO("[2]cause:%d", deact_ind->cause);
                    TEL_CONN_MGR_LOG_INFO("[2]app_id[0]:%d, app_id[1]:%d, app_id[2]:%d, app_id[3]:%d, app_id[4]:%d",
                                          deact_ind->app_id[0], deact_ind->app_id[1], 
                                          deact_ind->app_id[2], deact_ind->app_id[3],
                                          deact_ind->app_id[4]);
                    break;
                }
                
                default:
                    break;
            }

            tel_conn_mgr_free(msg);
            msg = NULL;
        }
        vTaskDelay(10);
    }

    tel_conn_mgr_ut_task_queue_free(tel_conn_mgr_ut_app_task2_queue_hdl);

    tel_conn_mgr_ut_app_task2_hdl = NULL;
    tel_conn_mgr_task_delete(NULL);
}



void tel_conn_mgr_ut_init(int app_num, int test_case_num)
{
    TEL_CONN_MGR_LOG_INFO("app_num:%d", app_num);

#ifdef TEL_CONN_MGR_UT
    if (!tel_conn_mgr_ut_ril_task_queue_hdl)
    {
        TEL_CONN_MGR_LOG_INFO("create ril task");
        tel_conn_mgr_ut_task_create(&tel_conn_mgr_ut_ril_task_queue_hdl,
                                    &tel_conn_mgr_ut_ril_task_hdl,
                                    tel_conn_mgr_ut_ril_task_main,
                                    TEL_CONN_MGR_UT_RIL_TASK_NAME,
                                    TEL_CONN_MGR_UT_RIL_TASK_STACKSIZE,
                                    TEL_CONN_MGR_UT_RIL_TASK_PRIO,
                                    NULL);
    }
#endif

    if (app_num > 0)
    {
        if (!tel_conn_mgr_ut_app_task1_queue_hdl)
        {
            tel_conn_mgr_ut_task_create(&tel_conn_mgr_ut_app_task1_queue_hdl,
                                        &tel_conn_mgr_ut_app_task1_hdl,
                                        tel_conn_mgr_ut_app_task1_main,
                                        "app_task1",
                                        (2 * 1024),
                                        TASK_PRIORITY_NORMAL,
                                        (void *)test_case_num);
        }
    }
    
    if (app_num >= 2)
    {
        if (!tel_conn_mgr_ut_app_task2_queue_hdl)
        {
            tel_conn_mgr_ut_task_create(&tel_conn_mgr_ut_app_task2_queue_hdl,
                                        &tel_conn_mgr_ut_app_task2_hdl,
                                        tel_conn_mgr_ut_app_task2_main,
                                        "app_task2",
                                        (2 * 1024),
                                        TASK_PRIORITY_NORMAL,
                                        (void *)test_case_num);
        }
    }
}

#endif /* defined(TEL_CONN_MGR_UT) || defined(TEL_CONN_MGR_IT) */

