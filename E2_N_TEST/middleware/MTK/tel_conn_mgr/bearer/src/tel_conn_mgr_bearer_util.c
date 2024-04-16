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

#include "tel_conn_mgr_bearer_list_mgr.h"
#include "tel_conn_mgr_common.h"
#include "tel_conn_mgr_bearer_at_cmd_util.h"
#include "tel_conn_mgr_bearer_util.h"
#include "tel_conn_mgr_bearer_iprot.h"
#include "tel_conn_mgr_bearer_timer.h"
#include "tel_conn_mgr_bearer_util.h"
#include "tel_conn_mgr_util.h"

#ifdef TEL_CONN_MGR_INFORM_TCPIP_NIDD
#include "nbnetif.h"
#include "nidd_gprot.h"
#include "ril.h"
#endif

/**
 * Clear at_cmd_to_send_list, at_cmd_to_recv_list, class_to_excute, class_to_finish only
 */
tel_conn_mgr_ret_enum tel_conn_mgr_bearer_terminate_general_ing_state(int cid,
                                                                                   tel_conn_mgr_at_cmd_class_enum at_cmd_class,
                                                                                   void **callback)
{
    tel_conn_mgr_class_list_struct *class_list_node = NULL;
    tel_conn_mgr_at_cmd_list_struct *at_cmd_list_node = NULL;
    tel_conn_mgr_bool class_list_node_found = TEL_CONN_MGR_FALSE, at_cmd_list_node_found = TEL_CONN_MGR_FALSE;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_NOT_FOUND;
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);

    if (!tel_conn_mgr_is_cid_valid(cid) ||
        !tel_conn_mgr_is_valid_at_cmd_class(at_cmd_class) ||
        !at_cmd_flow_helper)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    if (callback)
    {
        *callback = NULL;
    }

    /* The at_cmd_list_node is in one or none of the at cmd lists. */
    /* Remove from  at_cmd_flow_helper->at_cmd_to_send_list */
    at_cmd_list_node = (tel_conn_mgr_at_cmd_list_struct *)tel_conn_mgr_find_list_node(
                            (tel_conn_mgr_bearer_template_list_struct *)at_cmd_flow_helper->at_cmd_to_send_list,
                            cid, at_cmd_class);
    if (at_cmd_list_node)
    {
        at_cmd_list_node_found = TEL_CONN_MGR_TRUE;
        tel_conn_mgr_list_remove((tel_conn_mgr_template_list_struct **)&at_cmd_flow_helper->at_cmd_to_send_list,
                               (tel_conn_mgr_template_list_struct *)at_cmd_list_node);
        tel_conn_mgr_at_cmd_list_node_free(at_cmd_list_node);
        at_cmd_list_node = NULL;
    }
    else /* Set at cmd node in at_cmd_to_recv_list to be expired, which will be removed in tel_conn_mgr_at_cmd_rsp_process() */
    {
        at_cmd_list_node = (tel_conn_mgr_at_cmd_list_struct *)tel_conn_mgr_find_list_node(
                            (tel_conn_mgr_bearer_template_list_struct *)at_cmd_flow_helper->at_cmd_to_recv_list,
                            cid, at_cmd_class);
        if (at_cmd_list_node)
        {
            at_cmd_list_node_found = TEL_CONN_MGR_TRUE;
            at_cmd_list_node->is_expired = TEL_CONN_MGR_TRUE;
            /* Allow to send at cmd before the rsp received. */
            at_cmd_flow_helper->current_at_cmd_type = TEL_CONN_MGR_AT_CMD_TYPE_NONE;
        }
    }

    /* The class_list_node is in one or none of the class lists. */
    /* Remove from class_to_finish list. */
    class_list_node = (tel_conn_mgr_class_list_struct *)tel_conn_mgr_find_list_node(
                            (tel_conn_mgr_bearer_template_list_struct *)at_cmd_flow_helper->class_to_finish,
                            cid, at_cmd_class);
    if (class_list_node)
    {
        class_list_node_found = TEL_CONN_MGR_TRUE;
        tel_conn_mgr_list_remove((tel_conn_mgr_template_list_struct **)&at_cmd_flow_helper->class_to_finish,
                               (tel_conn_mgr_template_list_struct *)class_list_node);        
    }
    else /* Remove from class_to_excute list. */
    {
        class_list_node = (tel_conn_mgr_class_list_struct *)tel_conn_mgr_find_list_node(
                                (tel_conn_mgr_bearer_template_list_struct *)at_cmd_flow_helper->class_to_excute,
                                cid, at_cmd_class);
        if (class_list_node)
        {
            class_list_node_found = TEL_CONN_MGR_TRUE;
            tel_conn_mgr_list_remove((tel_conn_mgr_template_list_struct **)&at_cmd_flow_helper->class_to_excute,
                                   (tel_conn_mgr_template_list_struct *)class_list_node);
        }
    }

    /* Clear ing state */
    if (at_cmd_list_node_found || class_list_node_found)
    {
        ret = TEL_CONN_MGR_RET_OK;

        if (class_list_node)
        {
            if (tel_conn_mgr_is_special_cid(cid))
            {
                TEL_CONN_MGR_LOG_ERR("INACTIVEATE class with class_list_node");
            }
            else if (class_list_node->callback && callback)
            {
                *callback = class_list_node->callback;
            }
            
            tel_conn_mgr_class_list_node_free(class_list_node);
        }
    }

    return ret;    
}


/* Cancel on-going INIT/ACTIVATE/DEACTIVATE. */
#ifdef TEL_CONN_MGR_ENABLE_INACTIVATE
tel_conn_mgr_ret_enum tel_conn_mgr_bearer_terminate_initing_state(int cid, unsigned short cause)
{
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;

    if (!tel_conn_mgr_is_special_cid(cid) ||
        !at_cmd_flow_helper ||
        !bearer_state)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    ret = tel_conn_mgr_bearer_terminate_general_ing_state(cid, TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE, NULL);

    (*bearer_state) &= ~TEL_CONN_MGR_BEARER_STATE_INACTIVATING;

    return ret;
}
#endif


tel_conn_mgr_ret_enum tel_conn_mgr_bearer_terminate_activating_state(int cid,
                                                                                 tel_conn_mgr_bool result,
                                                                                 unsigned short cause,
                                                                                 tel_conn_mgr_bool need_deact)
{
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_NOT_FOUND;
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);
    tel_conn_mgr_activate_callback activate_callback = NULL;
    unsigned bearer_info_idx = tel_conn_mgr_convt_cid_to_bearer_info_idx(cid);
    tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_cid(cid);
#if defined(TEL_CONN_MGR_SUPPORT_DEEP_SLEEP) && !defined(TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM)
    char nvdm_bearer_info_group[20] = {0};
    tel_conn_mgr_bool is_used = TEL_CONN_MGR_TRUE;
#endif
        
    TEL_CONN_MGR_LOG_INFO("terminate_activating cid:%d result:%d cause:%d need_deact:%d", cid, result, cause, need_deact);

    if (!tel_conn_mgr_is_cid_valid(cid) || 
        tel_conn_mgr_is_special_cid(cid) ||
        !tel_conn_mgr_is_idx_valid(bearer_info_idx) ||
        tel_conn_mgr_is_special_idx(bearer_info_idx) ||
        !bearer_state ||
        !at_cmd_flow_helper ||
        !ds_keep ||
        (result && need_deact) ||
        (TEL_CONN_MGR_BEARER_STATE_NEED_DEACT == (*bearer_state) && !need_deact))
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

#ifdef TEL_CONN_MGR_ACTIVATION_GUARD_TIMER_SUPPORT
    if (TEL_CONN_MGR_ERR_CAUSE_TIMEOUT != cause)
    {
        tel_conn_mgr_timer_stop(TEL_CONN_MGR_MODME_TIMER_ID_ACTIVATION, cid);
    }
#endif    
    
    ret = tel_conn_mgr_bearer_terminate_general_ing_state(cid,
                                                          TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE,
                                                          (void *)&activate_callback);
    TEL_CONN_MGR_LOG_INFO("terminate ACTIVATE ret:%d, callback:%x", ret, (unsigned int)activate_callback);
    if (TEL_CONN_MGR_RET_OK != ret)
    {
        ret = tel_conn_mgr_bearer_terminate_general_ing_state(cid,
                                                          TEL_CONN_MGR_AT_CMD_CLASS_GET_RELEVANT_INFO,
                                                          (void *)&activate_callback);
        TEL_CONN_MGR_LOG_INFO("terminate GET_RELEVANT_INFO ret:%d, callback:%x", ret, (unsigned int)activate_callback);
    }

    if (TEL_CONN_MGR_BEARER_STATE_NEED_DEACT != (*bearer_state))
    {
        (*bearer_state) &= ~TEL_CONN_MGR_BEARER_STATE_ACTIVATING_ALL;
#if defined(TEL_CONN_MGR_SUPPORT_DEEP_SLEEP) && !defined(TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM)
        sprintf(nvdm_bearer_info_group, 
                TEL_CONN_MGR_NVDM_BEARER_INFO_GROUP_BASE,
                bearer_info_idx);
#endif
        if (result)
        {
            (*bearer_state) |= TEL_CONN_MGR_BEARER_STATE_ACTIVE;
#ifdef TEL_CONN_MGR_INFORM_TCPIP_NIDD
            if (TEL_CONN_MGR_PDP_TYPE_NIDD == ds_keep->pdp_type)
            {
                /* Notify NIDD */
                nidd_bearer_info_struct nidd_bearer_info = {0};
                
                nidd_bearer_info.is_activated = 1;
                strncpy(nidd_bearer_info.apn, ds_keep->apn, NIDD_APN_NAME_MAX_LEN);
                nidd_bearer_info.channel_id = ds_keep->data_channel_id;                
                nidd_bearer_info_ind(&nidd_bearer_info);
            }
#endif

#if defined(TEL_CONN_MGR_SUPPORT_DEEP_SLEEP) && !defined(TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM)
            // Save bearer state into nvdm
            tel_conn_mgr_nvdm_write(nvdm_bearer_info_group,
                                 TEL_CONN_MGR_NVDM_BEARER_INFO_IS_USED,
                                 NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                 (const uint8_t *)&is_used,
                                 sizeof(tel_conn_mgr_bool));

            tel_conn_mgr_nvdm_write(nvdm_bearer_info_group,
                                 TEL_CONN_MGR_NVDM_BEARER_INFO_CID,
                                 NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                 (const uint8_t *)&ds_keep->cid,
                                 sizeof(int));

            tel_conn_mgr_nvdm_write(nvdm_bearer_info_group,
                                 TEL_CONN_MGR_NVDM_BEARER_INFO_PDP_TYPE,
                                 NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                 (const uint8_t *)&ds_keep->pdp_type,
                                 sizeof(tel_conn_mgr_pdp_type_enum));

            tel_conn_mgr_nvdm_write(nvdm_bearer_info_group,
                                 TEL_CONN_MGR_NVDM_BEARER_INFO_ACTIVATED_PDP_TYPE,
                                 NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                 (const uint8_t *)&ds_keep->activated_pdp_type,
                                 sizeof(tel_conn_mgr_pdp_type_enum));

            tel_conn_mgr_nvdm_write(nvdm_bearer_info_group,
                                 TEL_CONN_MGR_NVDM_BEARER_INFO_APN,
                                 NVDM_DATA_ITEM_TYPE_STRING,
                                 (const uint8_t *)ds_keep->apn,
                                 TEL_CONN_MGR_APN_MAX_LEN);
            
            tel_conn_mgr_nvdm_write(nvdm_bearer_info_group,
                                 TEL_CONN_MGR_NVDM_BEARER_INFO_DATA_CHANNEL_ID,
                                 NVDM_DATA_ITEM_TYPE_STRING,
                                 (const uint8_t *)&ds_keep->data_channel_id,
                                 sizeof(int));
            
            tel_conn_mgr_nvdm_write(nvdm_bearer_info_group,
                                 TEL_CONN_MGR_NVDM_BEARER_INFO_BEARER_STATE,
                                 NVDM_DATA_ITEM_TYPE_STRING,
                                 (const uint8_t *)&ds_keep->bearer_state,
                                 sizeof(tel_conn_mgr_bearer_type_enum));
#endif
        }
        else
        {
            (*bearer_state) = TEL_CONN_MGR_BEARER_STATE_INACTIVE;
#if defined(TEL_CONN_MGR_SUPPORT_DEEP_SLEEP) && !defined(TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM)
            is_used = TEL_CONN_MGR_FALSE;
            tel_conn_mgr_nvdm_write(nvdm_bearer_info_group,
                                 TEL_CONN_MGR_NVDM_BEARER_INFO_IS_USED,
                                 NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                 (const uint8_t *)&is_used,
                                 sizeof(tel_conn_mgr_bool));
#endif
        }
    }

    TEL_CONN_MGR_LOG_INFO("terminate activating bearer_state:%x, cid:%d, data_channel_id:%d",
                          *bearer_state, cid,
                          ds_keep->data_channel_id);
    if (!result && !need_deact)
    {
        /* Deactivation AT CMDs are not sent over data channel. 
                 * However, in case new activation is requested during deactivation, lock the data channel until deactivation done.
                 * Besides, deactivation might fail.
                 */
#ifndef TEL_CONN_MGR_UT
        ril_set_data_channel_reserved(ds_keep->data_channel_id, TEL_CONN_MGR_FALSE);
#endif
    }
    
    if (activate_callback)
    {
        activate_callback(result, cause, cid, need_deact);
    }

    tel_conn_mgr_bearer_info_free(bearer_info_idx);

    tel_conn_mgr_bearer_unlock_sleep();

    return ret;
}


/* Result: deactivation ok or not. */
tel_conn_mgr_ret_enum tel_conn_mgr_bearer_terminate_deactivating_state(int cid, tel_conn_mgr_bool result, unsigned short cause)
{
    tel_conn_mgr_deactivate_callback deactivate_callback = NULL;
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_NOT_FOUND;
    unsigned bearer_info_idx = tel_conn_mgr_convt_cid_to_bearer_info_idx(cid);
    tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_cid(cid);

    if (!tel_conn_mgr_is_cid_valid(cid) ||
        tel_conn_mgr_is_special_cid(cid) ||
        !at_cmd_flow_helper ||
        !bearer_state || !ds_keep)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

#ifdef TEL_CONN_MGR_DEACTIVATION_GUARD_TIMER_SUPPORT
    if (TEL_CONN_MGR_ERR_CAUSE_TIMEOUT != cause)
    {
        tel_conn_mgr_timer_stop(TEL_CONN_MGR_MODME_TIMER_ID_DEACTIVATION, cid);
    }
#endif

    ret = tel_conn_mgr_bearer_terminate_general_ing_state(cid,
                                                    TEL_CONN_MGR_AT_CMD_CLASS_DEACTIVATE,
                                                    (void *)&deactivate_callback);
    (*bearer_state) &= ~TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_ALL;
    if (result)
    {
#if defined(TEL_CONN_MGR_SUPPORT_DEEP_SLEEP) && !defined(TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM)
        /* Save bearer state into nvdm */
        tel_conn_mgr_bool is_used = TEL_CONN_MGR_FALSE;
        char nvdm_bearer_info_group[20] = {0};

        sprintf(nvdm_bearer_info_group, 
                TEL_CONN_MGR_NVDM_BEARER_INFO_GROUP_BASE,
                bearer_info_idx);

        tel_conn_mgr_nvdm_write(nvdm_bearer_info_group,
                             TEL_CONN_MGR_NVDM_BEARER_INFO_IS_USED,
                             NVDM_DATA_ITEM_TYPE_RAW_DATA,
                             (const uint8_t *)&is_used,
                             sizeof(tel_conn_mgr_bool));
#endif
        *bearer_state = TEL_CONN_MGR_BEARER_STATE_INACTIVE;

#ifdef TEL_CONN_MGR_INFORM_TCPIP_NIDD
        if (TEL_CONN_MGR_PDP_TYPE_NIDD == ds_keep->pdp_type)
        {
            /* Notify NIDD */
            nidd_bearer_info_struct nidd_bearer_info = {0};
            
            nidd_bearer_info.is_activated = 0;
            strncpy(nidd_bearer_info.apn, ds_keep->apn, NIDD_APN_NAME_MAX_LEN);
            nidd_bearer_info.channel_id = ds_keep->data_channel_id;                
            nidd_bearer_info_ind(&nidd_bearer_info);
        }
        else
        {
            /* Notify TCPIP */
            nbiot_bearer_info_struct netinfo = {0};
            
            netinfo.is_activated = 0;
            netinfo.cid = cid;
            netinfo.channel_id = ds_keep->data_channel_id;
            nb_netif_bearer_info_ind(&netinfo);
        }
        /* should notify to ril after lwip, because ril is higher than tel_conn_mgr and at cmd sending maybe interrupt. */
        ril_set_data_channel_reserved(ds_keep->data_channel_id, TEL_CONN_MGR_FALSE);
#endif
    }

    if (deactivate_callback)
    {
        deactivate_callback(result, cause, cid);
    }

    tel_conn_mgr_bearer_info_free(bearer_info_idx);

    tel_conn_mgr_bearer_unlock_sleep();

    return ret;
}


tel_conn_mgr_ret_enum tel_conn_mgr_bearer_terminate_active_state(int cid, unsigned short cause, tel_conn_mgr_bool need_deact)
{
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);
    tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_cid(cid);
    unsigned bearer_info_idx = tel_conn_mgr_convt_cid_to_bearer_info_idx(cid);

    if (!bearer_state || !at_cmd_flow_helper || !ds_keep)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    (*bearer_state) &= ~TEL_CONN_MGR_BEARER_STATE_ACTIVE;
    (*bearer_state) |= TEL_CONN_MGR_BEARER_STATE_INACTIVE;    
#if defined(TEL_CONN_MGR_SUPPORT_DEEP_SLEEP) && !defined(TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM)
    // TODO: Save bearer state into nvdm
#endif

#ifdef TEL_CONN_MGR_INFORM_TCPIP_NIDD
    if (TEL_CONN_MGR_PDP_TYPE_NIDD == ds_keep->pdp_type)
    {
        /* Notify NIDD */
        nidd_bearer_info_struct nidd_bearer_info = {0};
        
        nidd_bearer_info.is_activated = 0;
        strncpy(nidd_bearer_info.apn, ds_keep->apn, NIDD_APN_NAME_MAX_LEN);
        nidd_bearer_info.channel_id = ds_keep->data_channel_id;                
        nidd_bearer_info_ind(&nidd_bearer_info);
    }
    else
    {
        /* Notify TCPIP */
        nbiot_bearer_info_struct netinfo = {0};
        
        netinfo.is_activated = 0;
        netinfo.cid = cid;
        netinfo.channel_id = ds_keep->data_channel_id;
        nb_netif_bearer_info_ind(&netinfo);
    }
    /* should notify to ril after lwip, because ril is higher than tel_conn_mgr and at cmd sending maybe interrupt. */
    ril_set_data_channel_reserved(ds_keep->data_channel_id, TEL_CONN_MGR_FALSE);
#endif

    /* Send deactivation IND MSG */
    tel_conn_mgr_notify_app(TEL_CONN_MGR_APP_MSG_TYPE_PASSIVE_DEACTIVATION,
                            cid,
                            TEL_CONN_MGR_FALSE,
                            cause,
                            NULL,
                            NULL);

    tel_conn_mgr_update_app_state_for_passive_deactivation(cid);

    tel_conn_mgr_bearer_info_free(bearer_info_idx);

    return TEL_CONN_MGR_RET_OK;
}


/* tel_conn_mgr_err_cause_enum. 
  * need_deact is only valid for ACTIVATE and GET_RELEVANT_INFO classes.
  */
tel_conn_mgr_ret_enum tel_conn_mgr_bearer_turn_into_inactive_state_by_cid(int cid,
                                                                                   unsigned short cause,
                                                                                   tel_conn_mgr_bool need_deact)
{
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);
    unsigned int bearer_info_idx = tel_conn_mgr_convt_cid_to_bearer_info_idx(cid);

    if (!bearer_state || !at_cmd_flow_helper || !tel_conn_mgr_is_cid_valid(cid))
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

#ifdef TEL_CONN_MGR_ENABLE_INACTIVATE
    if (tel_conn_mgr_is_special_cid(cid))
    {
        if (TEL_CONN_MGR_BEARER_STATE_INACTIVATING & (*bearer_state))
        {
            ret = tel_conn_mgr_bearer_terminate_initing_state(cid, cause);
        }
    }
    else
#endif /* TEL_CONN_MGR_ENABLE_INACTIVATE */
    {
        /* non-TEL_CONN_MGR_SPECIAL_CID will not do INIT. */

        if (tel_conn_mgr_is_bearer_activating(*bearer_state))
        {
            ret = tel_conn_mgr_bearer_terminate_activating_state(cid, TEL_CONN_MGR_FALSE, cause, need_deact);
        }
        else if (tel_conn_mgr_is_bearer_deactivating(*bearer_state))
        {
            ret = tel_conn_mgr_bearer_terminate_deactivating_state(cid, TEL_CONN_MGR_TRUE, cause);
        }
        else if (tel_conn_mgr_is_bearer_active(*bearer_state))
        {
            ret = tel_conn_mgr_bearer_terminate_active_state(cid, cause, need_deact);
        }
        else
        {
            TEL_CONN_MGR_LOG_WARN("unexpected bearer state:%x", *bearer_state);
            tel_conn_mgr_bearer_info_free(bearer_info_idx);
        }
    }

    return ret;
}


void tel_conn_mgr_bearer_turn_into_inactive_state_all(unsigned short cause, tel_conn_mgr_bool need_deact)
{
    unsigned int bearer_info_idx = 0;
    int cid = TEL_CONN_MGR_MIN_CID - 1;

#ifdef TEL_CONN_MGR_ENABLE_INACTIVATE
    tel_conn_mgr_bearer_turn_into_inactive_state_by_cid(TEL_CONN_MGR_SPECIAL_CID,
                                                        cause,
                                                        TEL_CONN_MGR_FALSE);
#endif

    for (bearer_info_idx = 0; bearer_info_idx < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; bearer_info_idx++)
    {
        cid = tel_conn_mgr_convt_bearer_info_idx_to_cid(bearer_info_idx);
        tel_conn_mgr_bearer_turn_into_inactive_state_by_cid(cid, cause, TEL_CONN_MGR_FALSE);
    }
}


tel_conn_mgr_bool tel_conn_mgr_is_bearer_active(tel_conn_mgr_bearer_state_enum bearer_state)
{
    if (bearer_state & TEL_CONN_MGR_BEARER_STATE_ACTIVE)
    {
        return TEL_CONN_MGR_TRUE;
    }

    return TEL_CONN_MGR_FALSE;
}


tel_conn_mgr_bool tel_conn_mgr_is_bearer_inactive(tel_conn_mgr_bearer_state_enum bearer_state)
{
    if (bearer_state & TEL_CONN_MGR_BEARER_STATE_INACTIVE)
    {
        return TEL_CONN_MGR_TRUE;
    }

    return TEL_CONN_MGR_FALSE;
}


tel_conn_mgr_bool tel_conn_mgr_is_bearer_activating(tel_conn_mgr_bearer_state_enum bearer_state)
{
    if (bearer_state & TEL_CONN_MGR_BEARER_STATE_ACTIVATING)
    {
        return TEL_CONN_MGR_TRUE;
    }

    return TEL_CONN_MGR_FALSE;
}


tel_conn_mgr_bool tel_conn_mgr_is_bearer_deactivating(tel_conn_mgr_bearer_state_enum bearer_state)
{
    if (bearer_state & TEL_CONN_MGR_BEARER_STATE_DEACTIVATING)
    {
        return TEL_CONN_MGR_TRUE;
    }

    return TEL_CONN_MGR_FALSE;
}


tel_conn_mgr_bool tel_conn_mgr_is_nw_registered(void)
{
    tel_conn_mgr_nw_reg_status_struct *nw_reg_sta = tel_conn_mgr_bearer_get_nw_reg_status();

    if (nw_reg_sta &&
        (TEL_CONN_MGR_NW_REG_STAT_REGED_HMNW == nw_reg_sta->stat ||
        TEL_CONN_MGR_NW_REG_STAT_REGED_ROAMING == nw_reg_sta->stat ||
        TEL_CONN_MGR_NW_REG_STAT_SMS_ONLY_HMNW == nw_reg_sta->stat ||
        TEL_CONN_MGR_NW_REG_STAT_SMS_ONLY_ROAMING == nw_reg_sta->stat))
    {
        TEL_CONN_MGR_LOG_INFO("%s, stat:%d true", __FUNCTION__, nw_reg_sta->stat);
        return TEL_CONN_MGR_TRUE;
    }

    TEL_CONN_MGR_LOG_INFO("%s, stat:%d false", __FUNCTION__, nw_reg_sta->stat);

    return TEL_CONN_MGR_FALSE;
}


tel_conn_mgr_bool tel_conn_mgr_is_nw_registration_failed(void)
{
    tel_conn_mgr_nw_reg_status_struct *nw_reg_sta = tel_conn_mgr_bearer_get_nw_reg_status();

    if (nw_reg_sta &&
        (TEL_CONN_MGR_NW_REG_STAT_REGED_HMNW == nw_reg_sta->stat ||
        TEL_CONN_MGR_NW_REG_STAT_REGED_ROAMING == nw_reg_sta->stat ||
        TEL_CONN_MGR_NW_REG_STAT_TRYING == nw_reg_sta->stat ||
        //TEL_CONN_MGR_NW_REG_STAT_UNKNOWN == nw_reg_sta->stat ||
        TEL_CONN_MGR_NW_REG_STAT_SMS_ONLY_HMNW == nw_reg_sta->stat ||
        TEL_CONN_MGR_NW_REG_STAT_SMS_ONLY_ROAMING == nw_reg_sta->stat ||
        TEL_CONN_MGR_NW_REG_STAT_MAX == nw_reg_sta->stat))
    {
        TEL_CONN_MGR_LOG_INFO("%s, stat:%d false", __FUNCTION__, nw_reg_sta->stat);
        return TEL_CONN_MGR_FALSE;
    }

    TEL_CONN_MGR_LOG_INFO("%s, stat:%d true", __FUNCTION__, nw_reg_sta->stat);

    return TEL_CONN_MGR_TRUE;
}


#ifdef TEL_CONN_MGR_ACTIVATION_GUARD_TIMER_SUPPORT
void tel_conn_mgr_activation_timeout_hdlr(unsigned int timer_id, int cid)
{
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);
    unsigned int bearer_info_idx = tel_conn_mgr_convt_cid_to_bearer_info_idx(cid);

    if (timer_id != TEL_CONN_MGR_MODME_TIMER_ID_ACTIVATION ||
        !tel_conn_mgr_is_idx_valid(bearer_info_idx) ||
        tel_conn_mgr_is_special_cid(bearer_info_idx) ||
        !bearer_state ||
        !at_cmd_flow_helper)
    {
        return;
    }

    (*bearer_state) &= ~TEL_CONN_MGR_BEARER_STATE_ACTIVATING_ALL;

    if (TEL_CONN_MGR_KEY_AT_CMD_TYPE_ACT == at_cmd_flow_helper->last_key_at_cmd_type ||
        TEL_CONN_MGR_KEY_AT_CMD_TYPE_ACT == at_cmd_flow_helper->current_at_cmd_type)
    {
        tel_conn_mgr_bearer_terminate_activating_state(cid, TEL_CONN_MGR_FALSE, TEL_CONN_MGR_ERR_CAUSE_TIMEOUT, TEL_CONN_MGR_TRUE);
    }
    else
    {
        tel_conn_mgr_bearer_terminate_activating_state(cid, TEL_CONN_MGR_FALSE, TEL_CONN_MGR_ERR_CAUSE_TIMEOUT, TEL_CONN_MGR_FALSE);
    }
}
#endif



#ifdef TEL_CONN_MGR_DEACTIVATION_GUARD_TIMER_SUPPORT
void tel_conn_mgr_deactivation_timeout_hdlr(unsigned int timer_id, int cid)
{
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);
    unsigned int bearer_info_idx = tel_conn_mgr_convt_cid_to_bearer_info_idx(cid);

    if (timer_id != TEL_CONN_MGR_MODME_TIMER_ID_DEACTIVATION ||
        !tel_conn_mgr_is_idx_valid(bearer_info_idx) ||
        tel_conn_mgr_is_special_cid(bearer_info_idx) ||
        !bearer_state ||
        !at_cmd_flow_helper)
    {
        return;
    }

    (*bearer_state) &= ~TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_ALL;

    tel_conn_mgr_bearer_terminate_deactivating_state(cid, TEL_CONN_MGR_FALSE, TEL_CONN_MGR_ERR_CAUSE_TIMEOUT);
}
#endif


tel_conn_mgr_bool tel_conn_mgr_is_valid_at_cmd_class(tel_conn_mgr_at_cmd_class_enum at_cmd_class)
{
    if (TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE == at_cmd_class ||
        TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE == at_cmd_class ||
        TEL_CONN_MGR_AT_CMD_CLASS_GET_RELEVANT_INFO == at_cmd_class ||
        TEL_CONN_MGR_AT_CMD_CLASS_DEACTIVATE == at_cmd_class)
    {
        return TEL_CONN_MGR_TRUE;
    }
    
    return TEL_CONN_MGR_FALSE;
}


void tel_conn_mgr_update_bearer_state_for_class_start(tel_conn_mgr_bearer_state_enum *bearer_state,
                                                                   tel_conn_mgr_at_cmd_class_enum at_cmd_class)
{
    tel_conn_mgr_bearer_state_enum bearer_state_tmp = TEL_CONN_MGR_BEARER_STATE_MAX;
    
    if (!bearer_state ||
        !tel_conn_mgr_is_valid_at_cmd_class(at_cmd_class))
    {
        return;
    }

    bearer_state_tmp = *bearer_state;

    switch (at_cmd_class)
    {
        case TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE:
        {
            bearer_state_tmp |= TEL_CONN_MGR_BEARER_STATE_INACTIVATING;
            break;
        }
        
        case TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE:
        {
            bearer_state_tmp &= ~TEL_CONN_MGR_BEARER_STATE_INACTIVE;
            bearer_state_tmp |= TEL_CONN_MGR_BEARER_STATE_ACTIVATING;
            break;
        }
        
        case TEL_CONN_MGR_AT_CMD_CLASS_GET_RELEVANT_INFO:
        {
            if (!(TEL_CONN_MGR_BEARER_STATE_ACTIVATING & bearer_state_tmp))
            {
                TEL_CONN_MGR_LOG_WARN("BEARER_STATE_ACTIVATING is not set at GET_RELEVANT_INFO");
                bearer_state_tmp |= TEL_CONN_MGR_BEARER_STATE_ACTIVATING;
            }
            break;
        }

        case TEL_CONN_MGR_AT_CMD_CLASS_DEACTIVATE:
        {
            bearer_state_tmp |= TEL_CONN_MGR_BEARER_STATE_DEACTIVATING;
            break;
        }

        default:
            break;
    }

    *bearer_state = bearer_state_tmp;
}


void tel_conn_mgr_revert_bearer_state_for_class_start(tel_conn_mgr_bearer_state_enum *bearer_state,
                                                                  tel_conn_mgr_at_cmd_class_enum at_cmd_class)
{
    tel_conn_mgr_bearer_state_enum bearer_state_tmp = TEL_CONN_MGR_BEARER_STATE_MAX;
    
    if (!bearer_state ||
        !tel_conn_mgr_is_valid_at_cmd_class(at_cmd_class))
    {
        return;
    }
    
    bearer_state_tmp = *bearer_state;

    switch (at_cmd_class)
    {
        case TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE:
        {
            bearer_state_tmp &= ~TEL_CONN_MGR_BEARER_STATE_INACTIVATING;
            break;
        }
        
        case TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE:
        {
            bearer_state_tmp |= TEL_CONN_MGR_BEARER_STATE_INACTIVE;
            bearer_state_tmp &= ~TEL_CONN_MGR_BEARER_STATE_ACTIVATING;
            break;
        }
        
        case TEL_CONN_MGR_AT_CMD_CLASS_GET_RELEVANT_INFO:
        {
            break;
        }

        case TEL_CONN_MGR_AT_CMD_CLASS_DEACTIVATE:
        {
            bearer_state_tmp &= ~TEL_CONN_MGR_BEARER_STATE_DEACTIVATING;
            break;
        }

        default:
            break;
    }

    *bearer_state = bearer_state_tmp;
}


void tel_conn_mgr_bearer_lock_sleep(void)
{
#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
    tel_conn_mgr_general_lock_sleep(tel_conn_mgr_bearer_get_cntx()->sleep_manager_handle,
                                    &(tel_conn_mgr_bearer_get_cntx()->sleep_lock_count));
#endif
}


void tel_conn_mgr_bearer_unlock_sleep(void)
{
#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
    tel_conn_mgr_general_unlock_sleep(tel_conn_mgr_bearer_get_cntx()->sleep_manager_handle,
                                    &(tel_conn_mgr_bearer_get_cntx()->sleep_lock_count));
#endif
}


tel_conn_mgr_bool tel_conn_mgr_is_cid_active(int cid)
{
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    if (bearer_state)
    {
        return tel_conn_mgr_is_bearer_active(*bearer_state);
    }

    return TEL_CONN_MGR_FALSE;
}

