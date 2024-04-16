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

#include "tel_conn_mgr_platform.h"
#include "tel_conn_mgr_common.h"
#include "tel_conn_mgr_bearer_at_cmd_util.h"
#include "tel_conn_mgr_bearer_list_mgr.h"
#include "tel_conn_mgr_util.h"
#include "tel_conn_mgr_bearer_info.h"
#include "tel_conn_mgr_bearer_urc.h"
#include "tel_conn_mgr_app_mgr.h"
#include "tel_conn_mgr_bearer_api.h"
#include "tel_conn_mgr_bearer_msg_hdlr.h"


void tel_conn_mgr_bearer_activate_cb(tel_conn_mgr_bool result,
                                              tel_conn_mgr_err_cause_enum cause,
                                              int cid,
                                              tel_conn_mgr_bool need_deact)
{

    if (!tel_conn_mgr_is_cid_valid(cid) || tel_conn_mgr_is_special_cid(cid))
    {
        return;
    }

    if (TEL_CONN_MGR_RET_DUPLICATION == result)
    {
        assert(0);
        // TODO: handle duplication case
    }

    tel_conn_mgr_notify_app(TEL_CONN_MGR_APP_MSG_TYPE_ACTIVATION,
                            cid,
                            result,
                            cause,
                            NULL,
                            NULL);

    tel_conn_mgr_update_app_state_for_activation(result,
                                                 cid,
                                                 need_deact);
}


void tel_conn_mgr_bearer_process_reactive_state(tel_conn_mgr_bool deact_result,
                                                            int cid)
{    
    tel_conn_mgr_app_cntx_struct *app_cntx = tel_conn_mgr_app_get_cntx();
    tel_conn_mgr_thread_info_struct *thread_info_tmp = app_cntx ? app_cntx->thread_info_list : NULL;
    tel_conn_mgr_thread_info_struct *thread_info_tmp_next = NULL;
    tel_conn_mgr_app_info_struct *app_info_tmp = NULL, *app_info_tmp_next = NULL;
    int bearer_info_idx = tel_conn_mgr_convt_cid_to_bearer_info_idx(cid);
    tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_idx(bearer_info_idx);
    
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    tel_conn_mgr_bool need_send_act_req = TEL_CONN_MGR_FALSE;
    tel_conn_mgr_app_state_enum app_state = tel_conn_mgr_app_get_app_state(bearer_info_idx);

    TEL_CONN_MGR_LOG_INFO("app_state:%d, result:%d, cid:%d", app_state, deact_result, cid);
    if (TEL_CONN_MGR_APP_STATE_REACTIVE != app_state)
    {
        return;
    }
    
    if (!app_cntx || !ds_keep ||
        !tel_conn_mgr_is_idx_valid(bearer_info_idx) ||
        tel_conn_mgr_is_special_idx(bearer_info_idx))
    {
        TEL_CONN_MGR_LOG_INFO("app_cntx:%x, ds_keep:%x, idx:%d",
                              (unsigned int)app_cntx, (unsigned int)ds_keep, bearer_info_idx);
        return;
    }    

    while (thread_info_tmp)
    {
        thread_info_tmp_next = thread_info_tmp->next;
        app_info_tmp = thread_info_tmp->app_info[bearer_info_idx];

        while (app_info_tmp)
        {
            app_info_tmp_next = app_info_tmp->next;
            TEL_CONN_MGR_LOG_INFO("app_id:%d, act:%d, deact_result:%d",
                                  app_info_tmp->app_id, app_info_tmp->act, deact_result);
            if (TEL_CONN_MGR_ACT_REACT == app_info_tmp->act)
            {
                app_info_tmp->act = TEL_CONN_MGR_ACT_ACT;
                if (!deact_result)
                {
                    ret = tel_conn_mgr_is_pdp_type_compatible(app_info_tmp->pdp_type,
                                                              ds_keep->pdp_type,
                                                              ds_keep->activated_pdp_type,
                                                              TEL_CONN_MGR_APP_STATE_ACTIVE);
                    if (TEL_CONN_MGR_RET_OK != ret)
                    {
                        tel_conn_mgr_notify_app(TEL_CONN_MGR_APP_MSG_TYPE_ACTIVATION,
                                                cid,
                                                TEL_CONN_MGR_FALSE,
                                                TEL_CONN_MGR_ERR_CAUSE_PDP_TYPE_NOT_COMPATIBLE,
                                                thread_info_tmp,
                                                app_info_tmp);
                        TEL_CONN_MGR_LOG_INFO("%d", __LINE__);
                    }
                    else
                    {
                        TEL_CONN_MGR_LOG_INFO("%d", __LINE__);
                        need_send_act_req = TEL_CONN_MGR_TRUE;                    
                    }
                }
                else
                {
                    TEL_CONN_MGR_LOG_INFO("%d", __LINE__);
                    need_send_act_req = TEL_CONN_MGR_TRUE;
                }
            }

            app_info_tmp = app_info_tmp_next;
        }
        
        thread_info_tmp = thread_info_tmp_next;
    }

    if (TEL_CONN_MGR_TRUE == need_send_act_req)
    {
        TEL_CONN_MGR_LOG_INFO("%d", __LINE__);
        tel_conn_mgr_app_update_app_state(bearer_info_idx, TEL_CONN_MGR_APP_STATE_ACTIVATING);
        tel_conn_mgr_send_act_req(cid);
    }
    else
    {
        TEL_CONN_MGR_LOG_INFO("%d", __LINE__);
        tel_conn_mgr_app_update_app_state(bearer_info_idx, TEL_CONN_MGR_APP_STATE_INACTIVE);
    }

    TEL_CONN_MGR_LOG_INFO("need_send_act_req:%d", need_send_act_req);
}


void tel_conn_mgr_bearer_deactivate_cb(tel_conn_mgr_bool result,
                                                 tel_conn_mgr_err_cause_enum cause,
                                                 int cid)
{
    /* Send deactivation RSP MSG */
    tel_conn_mgr_notify_app(TEL_CONN_MGR_APP_MSG_TYPE_ACTIVE_DEACTIVATION,
                            cid,
                            result,
                            cause,
                            NULL,
                            NULL);

    tel_conn_mgr_update_app_state_for_active_deactivation(cid);

    tel_conn_mgr_bearer_process_reactive_state(result, cid);
}


void tel_conn_mgr_act_req_hdlr(void *msg)
{
    tel_conn_mgr_activation_req_struct *act_req = (tel_conn_mgr_activation_req_struct *)msg;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_OK;
    unsigned char bearer_info_idx = act_req ? tel_conn_mgr_convt_cid_to_bearer_info_idx(act_req->cid) : TEL_CONN_MGR_BEARER_TYPE_MAX_NUM;

    TEL_CONN_MGR_LOG_INFO("%s, %x", __FUNCTION__, (unsigned int)msg);
    if (!msg)
    {
        return;
    }

    TEL_CONN_MGR_GLOBAL_MUTEX_LOCK;
    ret = tel_conn_mgr_bearer_activate(act_req->cid,
                                       tel_conn_mgr_bearer_activate_cb);

    if (TEL_CONN_MGR_RET_WOULDBLOCK != ret)
    {
        tel_conn_mgr_bearer_activate_cb(TEL_CONN_MGR_RET_OK == ret ? TEL_CONN_MGR_TRUE : TEL_CONN_MGR_FALSE,
                                        TEL_CONN_MGR_RET_OK == ret ? TEL_CONN_MGR_ERR_CAUSE_NONE : TEL_CONN_MGR_ERR_CAUSE_UNKNOWN,
                                        act_req->cid,
                                        TEL_CONN_MGR_FALSE);
        
        tel_conn_mgr_bearer_info_free(bearer_info_idx);
        tel_conn_mgr_bearer_unlock_sleep();
    }

    TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
}


void tel_conn_mgr_deact_req_hdlr(void *msg)
{
    tel_conn_mgr_deactivation_req_struct *deact_req = (tel_conn_mgr_deactivation_req_struct *)msg;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_OK;
    unsigned char bearer_info_idx = deact_req ? tel_conn_mgr_convt_cid_to_bearer_info_idx(deact_req->cid) : TEL_CONN_MGR_BEARER_TYPE_MAX_NUM;

    TEL_CONN_MGR_LOG_INFO("%s, %x", __FUNCTION__, (unsigned int)msg);

    if (!msg)
    {
        return;
    }

    TEL_CONN_MGR_GLOBAL_MUTEX_LOCK;
    ret = tel_conn_mgr_bearer_deactivate(deact_req->cid, tel_conn_mgr_bearer_deactivate_cb);
    
    if (TEL_CONN_MGR_RET_WOULDBLOCK != ret)
    {
        tel_conn_mgr_bearer_deactivate_cb(TEL_CONN_MGR_RET_OK == ret ? TEL_CONN_MGR_TRUE : TEL_CONN_MGR_FALSE,
                                          TEL_CONN_MGR_RET_OK == ret ? TEL_CONN_MGR_ERR_CAUSE_NONE : TEL_CONN_MGR_ERR_CAUSE_UNKNOWN,
                                          deact_req->cid);
        tel_conn_mgr_bearer_info_free(bearer_info_idx);
        tel_conn_mgr_bearer_unlock_sleep();
    }

    TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
}


void tel_conn_mgr_at_cmd_rsp_ind_hdlr(void *msg)
{
    tel_conn_mgr_bearer_at_cmd_rsp_ind_struct *at_cmd_rsp_ind = (tel_conn_mgr_bearer_at_cmd_rsp_ind_struct *)msg;

    if (!msg)
    {
        return;
    }

    TEL_CONN_MGR_GLOBAL_MUTEX_LOCK;
    tel_conn_mgr_at_cmd_rsp_process(at_cmd_rsp_ind);
    TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
}


void tel_conn_mgr_bearer_urc_ind_hdlr(void *msg)
{
    tel_conn_mgr_bearer_urc_ind_struct *urc_ind = (tel_conn_mgr_bearer_urc_ind_struct *)msg;
    
    TEL_CONN_MGR_LOG_INFO("%s, %x", __FUNCTION__, (unsigned int)msg);

    if (!msg)
    {
        return;
    }

    TEL_CONN_MGR_GLOBAL_MUTEX_LOCK;
    tel_conn_mgr_urc_process(urc_ind);
    TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
}

