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
#include "tel_conn_mgr_bearer_util.h"
#include "tel_conn_mgr_util.h"
#include "tel_conn_mgr_bearer_util.h"
#include "tel_conn_mgr_bearer_at_cmd_util.h"
#include "tel_conn_mgr_bearer_cache.h"


tel_conn_mgr_bool tel_conn_mgr_bearer_cache_is_wait_event_group_valid(int wait_event_group)
{
    if(wait_event_group & 
       (TEL_CONN_MGR_BEARER_WAIT_EVENT_INACTIVE_DONE |
        TEL_CONN_MGR_BEARER_WAIT_EVENT_NW_REG_RESULT))
    {
        return TEL_CONN_MGR_TRUE;
    }

    return TEL_CONN_MGR_FALSE;
}


tel_conn_mgr_bool tel_conn_mgr_bearer_cache_is_at_cmd_class_group_valid(int at_cmd_class_group)
{
    /* INACTIVE class is not allowed to be cached. */
    if(at_cmd_class_group & 
       (TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE|
        TEL_CONN_MGR_AT_CMD_CLASS_GET_RELEVANT_INFO|
        TEL_CONN_MGR_AT_CMD_CLASS_DEACTIVATE))
    {
        return TEL_CONN_MGR_TRUE;
    }

    return TEL_CONN_MGR_FALSE;
}


/* Remove it from class_to_excute list, stop timer, invoke callback, free node of class_to_excute. */
static void tel_conn_mgr_cache_remove_class_list_node(tel_conn_mgr_class_list_struct *class_list_node,
                                                                    unsigned int bearer_info_idx,
                                                                    tel_conn_mgr_err_cause_enum cause)
{
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_idx(bearer_info_idx);
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_idx(bearer_info_idx);
    tel_conn_mgr_activate_callback activate_callback = NULL;
    tel_conn_mgr_deactivate_callback deactivate_callback = NULL;
    int cid = class_list_node ? class_list_node->cid : (TEL_CONN_MGR_MIN_CID - 1);

    if (!bearer_state || !at_cmd_flow_helper || !class_list_node ||
        !tel_conn_mgr_is_idx_valid(bearer_info_idx) ||
        tel_conn_mgr_is_special_idx(bearer_info_idx))
    {
        return;
    }

#ifdef TEL_CONN_MGR_ACTIVATION_GUARD_TIMER_SUPPORT
    if (class_list_node->at_cmd_class == TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE ||
        class_list_node->at_cmd_class == TEL_CONN_MGR_AT_CMD_CLASS_GET_RELEVANT_INFO)
    {
        if (TEL_CONN_MGR_ERR_CAUSE_TIMEOUT != cause)
        {
            tel_conn_mgr_timer_stop(TEL_CONN_MGR_MODME_TIMER_ID_ACTIVATION, class_list_node->cid);
        }
    }
#endif

#ifdef TEL_CONN_MGR_DEACTIVATION_GUARD_TIMER_SUPPORT
    if (class_list_node->at_cmd_class == TEL_CONN_MGR_AT_CMD_CLASS_DEACTIVATE)
    {
        if (TEL_CONN_MGR_ERR_CAUSE_TIMEOUT != cause)
        {
            tel_conn_mgr_timer_stop(TEL_CONN_MGR_MODME_TIMER_ID_DEACTIVATION, class_list_node->cid);
        }
    }
#endif

    tel_conn_mgr_list_remove((tel_conn_mgr_template_list_struct **)&at_cmd_flow_helper->class_to_excute,
                             (tel_conn_mgr_template_list_struct *)class_list_node);

    tel_conn_mgr_revert_bearer_state_for_class_start(bearer_state,
                                                     class_list_node->at_cmd_class);
    if (class_list_node->callback)
    {
        if (TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE == class_list_node->at_cmd_class ||
            TEL_CONN_MGR_AT_CMD_CLASS_GET_RELEVANT_INFO == class_list_node->at_cmd_class)
        {
            activate_callback = class_list_node->callback;            
        }
        else if (TEL_CONN_MGR_AT_CMD_CLASS_DEACTIVATE == class_list_node->at_cmd_class)
        {
            deactivate_callback = class_list_node->callback;            
        }
    }

    tel_conn_mgr_class_list_node_free(class_list_node);

    if (activate_callback)
    {
        activate_callback(TEL_CONN_MGR_FALSE,
                          cause,
                          cid,
                          TEL_CONN_MGR_FALSE);
        tel_conn_mgr_bearer_unlock_sleep();
    }

    if (deactivate_callback)
    {
        deactivate_callback(TEL_CONN_MGR_FALSE,
                            cause,
                            cid);
        tel_conn_mgr_bearer_unlock_sleep();
    }

    tel_conn_mgr_bearer_info_free(bearer_info_idx);    
}


/**
  * Special cid or TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE can not be cached.
  */
tel_conn_mgr_ret_enum tel_conn_mgr_cache_add_class(int cid,
                                                            tel_conn_mgr_at_cmd_class_enum cmd_class,
                                                            tel_conn_mgr_at_cmd_type_enum cmd_type_begin,
                                                            void *callback,
                                                            int wait_event_group)
{
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);
    tel_conn_mgr_class_list_struct *cb_list_node = NULL;
    tel_conn_mgr_bearer_cntx_struct *bearer_cntx = tel_conn_mgr_bearer_get_cntx();

    TEL_CONN_MGR_LOG_INFO("%s, cid:%d, cmd_class:%d, cmd_type_begin:%d, cb:%x", __FUNCTION__, cid, cmd_class, cmd_type_begin, (unsigned int)callback);

    /* cmd_type_begin is unsigned integer. It is pointless to compare it with TEL_CONN_MGR_AT_CMD_TYPE_NONE. */
    if (!tel_conn_mgr_is_valid_at_cmd_class(cmd_class) ||
        TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE == cmd_class ||
        TEL_CONN_MGR_AT_CMD_TYPE_MAX <= cmd_type_begin ||
        !tel_conn_mgr_is_cid_valid(cid) ||
        tel_conn_mgr_is_special_cid(cid) ||
        !callback ||
        !bearer_state ||
        !at_cmd_flow_helper)
    {
        TEL_CONN_MGR_LOG_INFO("Invalid param. %s", __FUNCTION__);
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    if (!bearer_cntx || TEL_CONN_MGR_MODEM_STATE_READY != bearer_cntx->modem_state)
    {
        TEL_CONN_MGR_LOG_INFO("state error");
        return TEL_CONN_MGR_RET_WRONG_STATE;
    }

    if (TEL_CONN_MGR_AT_CMD_TYPE_NONE != cmd_type_begin)
    {
        if (!tel_conn_mgr_is_at_cmd_in_class_group(cmd_type_begin, cmd_class))
        {
            TEL_CONN_MGR_LOG_INFO("Type and class do not match. class:%d, type:%d \r\n", cmd_class, cmd_type_begin);
            return TEL_CONN_MGR_RET_INVALID_PARAM;
        }
    }

    // TODO: duplicate detection maybe
    tel_conn_mgr_update_bearer_state_for_class_start(bearer_state, cmd_class);

    cb_list_node = tel_conn_mgr_class_list_node_create(cid,
                                                       cmd_class,
                                                       callback,
                                                       wait_event_group);
    if (!cb_list_node)
    {
        ret = TEL_CONN_MGR_RET_NOT_ENOUGH_MEMORY;
    }
    else
    {
        ret = tel_conn_mgr_list_insert((tel_conn_mgr_template_list_struct **)&at_cmd_flow_helper->class_to_excute,
                                       (tel_conn_mgr_template_list_struct *)cb_list_node);
    }

    if (TEL_CONN_MGR_RET_OK != ret)
    {
        tel_conn_mgr_revert_bearer_state_for_class_start(bearer_state, cmd_class);

        if (cb_list_node)
        {
            tel_conn_mgr_class_list_node_free(cb_list_node);
        }        
    }

    TEL_CONN_MGR_LOG_INFO("%s class:%d, ret:%d \r\n", __FUNCTION__, cmd_class, ret);
    return ret;
}


tel_conn_mgr_ret_enum tel_conn_mgr_cache_wake_up_class(int at_cmd_class_group,
                                                                   int wait_event_group)
{
    tel_conn_mgr_class_list_struct *class_list_node = NULL, *class_list_node_next = NULL;
    unsigned int bearer_info_idx = TEL_CONN_MGR_BEARER_TYPE_MAX_NUM;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    tel_conn_mgr_bearer_state_enum *bearer_state = NULL;
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = NULL;
    tel_conn_mgr_activate_callback activate_callback = NULL;
    tel_conn_mgr_deactivate_callback deactivate_callback = NULL;
    int cid = TEL_CONN_MGR_MIN_CID - 1;

    TEL_CONN_MGR_LOG_INFO("%s, wake up class:%x, wait_event_group:%x",
                           __FUNCTION__, at_cmd_class_group, wait_event_group);

    if (!tel_conn_mgr_bearer_cache_is_wait_event_group_valid(wait_event_group) ||
        !tel_conn_mgr_bearer_cache_is_at_cmd_class_group_valid(at_cmd_class_group))
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    /* Caching INIT class is not allowed. */
    for (bearer_info_idx = 0; bearer_info_idx < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; bearer_info_idx++)
    {
        bearer_state = tel_conn_mgr_get_bearer_state_by_idx(bearer_info_idx);
        at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_idx(bearer_info_idx);    
        class_list_node = (bearer_state && at_cmd_flow_helper) ? at_cmd_flow_helper->class_to_excute : NULL;
        TEL_CONN_MGR_LOG_INFO("class_list_node:%x", (unsigned int)class_list_node);
        while (class_list_node)
        {
            class_list_node_next = class_list_node->next;
            if (!tel_conn_mgr_is_valid_at_cmd_class(class_list_node->at_cmd_class))
            {
                /* Remove the cached class */                
                tel_conn_mgr_cache_remove_class_list_node(class_list_node,
                                                          bearer_info_idx,
                                                          TEL_CONN_MGR_ERR_CAUSE_UNKNOWN);
                class_list_node = class_list_node_next;
                TEL_CONN_MGR_LOG_WARN("Invalid at cmd class:%d", class_list_node->at_cmd_class);
                continue;
            }

            TEL_CONN_MGR_LOG_INFO("at_cmd_class:%x, at_cmd_class_group:%x", class_list_node->at_cmd_class, at_cmd_class_group);
            
            TEL_CONN_MGR_LOG_INFO("class_list_node->wait_event_group:%x, wait_event_group:%x", class_list_node->wait_event_group, wait_event_group);

            if (class_list_node->at_cmd_class & at_cmd_class_group)
            {   
                if (class_list_node->wait_event_group & wait_event_group)
                {
                    class_list_node->wait_event_group &= ~wait_event_group;

                    TEL_CONN_MGR_LOG_INFO("final wait_event_group:%x", class_list_node->wait_event_group);
                    if (!tel_conn_mgr_bearer_cache_is_wait_event_group_valid(class_list_node->wait_event_group))
                    {
                        /* Wake up cached class. */
                        tel_conn_mgr_list_remove((tel_conn_mgr_template_list_struct **)&at_cmd_flow_helper->class_to_excute,
                                                 (tel_conn_mgr_template_list_struct *)class_list_node);
                        
                        TEL_CONN_MGR_LOG_INFO("wake up node:%x, cid:%d, class:%d, cmd:%d",
                                              class_list_node,
                                              class_list_node->cid,
                                              class_list_node->at_cmd_class,
                                              class_list_node->cmd_type_begin);

                        ret = tel_conn_mgr_send_at_cmds_by_class(class_list_node->cid,
                                                                 class_list_node->at_cmd_class,
                                                                 class_list_node->cmd_type_begin,
                                                                 NULL);
                        if (TEL_CONN_MGR_RET_OK != ret)
                        {
                            tel_conn_mgr_revert_bearer_state_for_class_start(bearer_state, class_list_node->at_cmd_class);
                            
                            if (class_list_node->callback)
                            {
                                if (TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE == class_list_node->at_cmd_class ||
                                    TEL_CONN_MGR_AT_CMD_CLASS_GET_RELEVANT_INFO == class_list_node->at_cmd_class)
                                {
                                    activate_callback = class_list_node->callback;            
                                }
                                else if (TEL_CONN_MGR_AT_CMD_CLASS_DEACTIVATE == class_list_node->at_cmd_class)
                                {
                                    deactivate_callback = class_list_node->callback;            
                                }
                            }
                            
                            cid = class_list_node->cid;
                            tel_conn_mgr_class_list_node_free(class_list_node);

                            if (activate_callback)
                            {
                                activate_callback(TEL_CONN_MGR_FALSE,
                                                  TEL_CONN_MGR_ERR_CAUSE_AT_CMD_EXCEPTION,
                                                  cid,
                                                  TEL_CONN_MGR_FALSE);
                            }

                            if (deactivate_callback)
                            {
                                deactivate_callback(TEL_CONN_MGR_FALSE,
                                                    TEL_CONN_MGR_ERR_CAUSE_AT_CMD_EXCEPTION,
                                                    cid);
                            }

                            tel_conn_mgr_bearer_info_free(bearer_info_idx);

                            if (activate_callback)
                            {
                                tel_conn_mgr_bearer_unlock_sleep();
                            }

                            if (deactivate_callback)
                            {
                                tel_conn_mgr_bearer_unlock_sleep();
                            }
                        }
                        else
                        {
                            tel_conn_mgr_list_insert((tel_conn_mgr_template_list_struct **)&at_cmd_flow_helper->class_to_finish,
                                                     (tel_conn_mgr_template_list_struct *)class_list_node);
                            TEL_CONN_MGR_LOG_INFO("wake up class:%d, bearer_state:%x, cid:%d",
                                                  class_list_node->at_cmd_class, *bearer_state, class_list_node->cid);
                        }
                    }
                }                
            }

            class_list_node = class_list_node_next;
            //TEL_CONN_MGR_LOG_INFO("idx:%d", bearer_info_idx);
        }
        //TEL_CONN_MGR_LOG_INFO("while done. idx:%d", bearer_info_idx);
    }

    return TEL_CONN_MGR_RET_OK;
}


tel_conn_mgr_ret_enum tel_conn_mgr_cache_remove_class(int at_cmd_class_group,
                                                                 int wait_event_group,
                                                                 tel_conn_mgr_err_cause_enum cause)
{
    tel_conn_mgr_class_list_struct *class_list_node = NULL, *class_list_node_next = NULL;
    unsigned int bearer_info_idx = TEL_CONN_MGR_BEARER_TYPE_MAX_NUM;
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = NULL;

    TEL_CONN_MGR_LOG_INFO("%s", __FUNCTION__);
    
    if (!tel_conn_mgr_bearer_cache_is_wait_event_group_valid(wait_event_group) ||
        !tel_conn_mgr_bearer_cache_is_at_cmd_class_group_valid(at_cmd_class_group))
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    /* Caching INIT class is not allowed. */
    for (bearer_info_idx = 0; bearer_info_idx < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; bearer_info_idx++)
    {
        at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_idx(bearer_info_idx);
        class_list_node = at_cmd_flow_helper ? at_cmd_flow_helper->class_to_excute : NULL;
        while (class_list_node)
        {
            class_list_node_next = class_list_node->next;
            
            if (class_list_node->at_cmd_class & at_cmd_class_group)
            {
                if (class_list_node->wait_event_group & wait_event_group)
                {
                    class_list_node->wait_event_group &= ~wait_event_group;

                    /* Clear cached class even if there's other wait event. */
                    tel_conn_mgr_cache_remove_class_list_node(class_list_node,
                                                    bearer_info_idx,
                                                    cause);
                }                
            }

            class_list_node = class_list_node_next;
        }
    }

    return TEL_CONN_MGR_RET_OK;
}


/* Update waited URC for certain cached class */
tel_conn_mgr_ret_enum tel_conn_mgr_cache_udpate_wait_event(tel_conn_mgr_at_cmd_class_enum at_cmd_class_group,
                                                                  tel_conn_mgr_bearer_state_enum old_wait_event_group,
                                                                  tel_conn_mgr_bearer_state_enum new_wait_event_group)
{
    tel_conn_mgr_class_list_struct *class_list_node = NULL;
    unsigned int bearer_info_idx = TEL_CONN_MGR_BEARER_TYPE_MAX_NUM;
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = NULL;

    TEL_CONN_MGR_LOG_INFO("%s, wake up class:%d, wait_event_group:%x",
                     __FUNCTION__, at_cmd_class_group, old_wait_event_group);

    if (!tel_conn_mgr_bearer_cache_is_wait_event_group_valid(old_wait_event_group) ||
        !tel_conn_mgr_bearer_cache_is_wait_event_group_valid(new_wait_event_group) ||
        !tel_conn_mgr_bearer_cache_is_at_cmd_class_group_valid(at_cmd_class_group))
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    for (bearer_info_idx = 0; bearer_info_idx < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; bearer_info_idx++)
    {
        at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_idx(bearer_info_idx);
        class_list_node = at_cmd_flow_helper ? at_cmd_flow_helper->class_to_excute : NULL;
        while (class_list_node)
        {
            if (class_list_node->at_cmd_class & at_cmd_class_group)
            {
                if (class_list_node->wait_event_group & old_wait_event_group)
                {
                    class_list_node->wait_event_group &= ~old_wait_event_group;
                    class_list_node->wait_event_group |= new_wait_event_group;
                    TEL_CONN_MGR_LOG_INFO("update event cid:%d class:%d evt:%x old evt:%x, new evt:%x.",
                        class_list_node->cid, class_list_node->at_cmd_class,
                        class_list_node->wait_event_group,
                        old_wait_event_group, new_wait_event_group);
                }
            }

            class_list_node = class_list_node->next;
        }
    }

    return TEL_CONN_MGR_RET_OK;
}


tel_conn_mgr_bool tel_conn_mgr_cache_is_class_cached(int cid, tel_conn_mgr_at_cmd_class_enum at_cmd_class)
{
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);
    tel_conn_mgr_class_list_struct *tmp_node = NULL;
    
    if (!tel_conn_mgr_is_valid_at_cmd_class(at_cmd_class) ||
        tel_conn_mgr_is_special_cid(cid) ||
        !tel_conn_mgr_is_cid_valid(cid) ||
        !at_cmd_flow_helper)
    {
        TEL_CONN_MGR_LOG_INFO("Parameter error. %d", __LINE__);
        return TEL_CONN_MGR_FALSE;
    }

    tmp_node = at_cmd_flow_helper->class_to_excute;

    while (tmp_node)
    {
        if (tmp_node->at_cmd_class == at_cmd_class &&
            tmp_node->cid == cid)
        {
            return TEL_CONN_MGR_TRUE;
        }

        tmp_node = tmp_node->next;
    }

    return TEL_CONN_MGR_FALSE;
}

