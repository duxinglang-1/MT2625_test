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

#include "tel_conn_mgr_util.h"
#include "tel_conn_mgr_bearer_info.h"
#include "tel_conn_mgr_app_mgr.h"
#include "tel_conn_mgr_app_api.h"


tel_conn_mgr_register_handle_t tel_conn_mgr_register_callback(tel_conn_mgr_cb callback)
{
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
    int i = 0;
    tel_conn_mgr_app_cntx_struct *app_cntx = tel_conn_mgr_app_get_cntx();
    
    if (!callback || !app_cntx)
    {
        return NULL;
    }

    for (i = 0; i < TEL_CONN_MGR_REG_INFO_MAX_NUM; i++)
    {    
        if (NULL == app_cntx->reg_info[i].reg_hdl)
        {
            memset(&app_cntx->reg_info[i], 0, sizeof(tel_conn_mgr_reg_info_struct));
            /* reg_hdl is just an identifier with no other meaning. Here use the address of app_cntx->reg_info[i] as the unique identifier. */
            app_cntx->reg_info[i].reg_hdl = (tel_conn_mgr_register_handle_t)&app_cntx->reg_info[i];
            app_cntx->reg_info[i].callback = callback;
            return app_cntx->reg_info[i].reg_hdl;
        }        
    }
#endif
     return NULL;
}


tel_conn_mgr_bool tel_conn_mgr_deregister_callback(tel_conn_mgr_register_handle_t reg_hdl)
{
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK    
    int i = 0;
    tel_conn_mgr_app_cntx_struct *app_cntx = tel_conn_mgr_app_get_cntx();

    if (!reg_hdl || !app_cntx)
    {
        return TEL_CONN_MGR_FALSE;
    }

    for (i = 0; i < TEL_CONN_MGR_REG_INFO_MAX_NUM; i++)
    {
        if (reg_hdl == app_cntx->reg_info[i].reg_hdl)
        {
            memset(&app_cntx->reg_info[i], 0, sizeof(tel_conn_mgr_reg_info_struct));            
            return TEL_CONN_MGR_TRUE;
        }        
    }
#endif
    return TEL_CONN_MGR_FALSE;
}


tel_conn_mgr_ret_enum tel_conn_mgr_activate_int(tel_conn_mgr_bearer_type_enum bearer_type,
                                         tel_conn_mgr_sim_id_enum sim_id,
                                         tel_conn_mgr_pdp_type_enum pdp_type,
                                         char *apn,
                                         char *username,
                                         char *password,
                                         tel_conn_mgr_queue_hdl_t queue_hdl,
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
                                         void *reg_hdl,
#endif
                                         unsigned int *app_id,
                                         tel_conn_mgr_pdp_type_enum *activated_pdp_type)
{
    tel_conn_mgr_bearer_info_struct *bearer_info = NULL;
    tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep = NULL;
    tel_conn_mgr_thread_info_struct *thread_info = NULL;
    tel_conn_mgr_app_info_struct *app_info = NULL;
    unsigned int bearer_info_idx = TEL_CONN_MGR_BEARER_TYPE_MAX_NUM;
    tel_conn_mgr_bool is_bearer_info_new = TEL_CONN_MGR_FALSE;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    tel_conn_mgr_app_state_enum old_app_state = TEL_CONN_MGR_APP_STATE_MAX;

    TEL_CONN_MGR_LOG_INFO("bearer_type:%d, sim_id:%d, pdp_type:%d", bearer_type, sim_id, pdp_type);
    TEL_CONN_MGR_LOG_INFO("apn:%s", apn ? apn : "");
    TEL_CONN_MGR_LOG_INFO("username:%s, password:%s",
                          username ? username : "",
                          password ? password : "");
    if (!app_id || !apn || strlen(apn) > TEL_CONN_MGR_APN_MAX_LEN ||
        (username && strlen(username) > TEL_CONN_MGR_USERNAME_MAX_LEN) ||
        (password && strlen(password) > TEL_CONN_MGR_PASSWORD_MAX_LEN) ||
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
        (reg_hdl && queue_hdl) || (!reg_hdl && !queue_hdl) ||
#endif
        (!username && password) || !activated_pdp_type)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    TEL_CONN_MGR_GLOBAL_MUTEX_LOCK;

    *app_id = 0;
    
    *activated_pdp_type = TEL_CONN_MGR_PDP_TYPE_NONE;

    /* Two connection requests will be regarded as to activate the same bearer if bearer_type, sim_id and the apn are the same. */
    bearer_info = tel_conn_mgr_bearer_info_find_by_info(bearer_type, sim_id, pdp_type, apn, &bearer_info_idx);
    ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_idx(bearer_info_idx);

    old_app_state = bearer_info ? tel_conn_mgr_app_get_app_state(bearer_info_idx) : TEL_CONN_MGR_APP_STATE_INACTIVE;

    /* Bearer does not exist. */
    if ((bearer_info && (TEL_CONN_MGR_APP_STATE_INACTIVE > old_app_state ||
        TEL_CONN_MGR_APP_STATE_MAX <= old_app_state)) ||
        ((!bearer_info || !ds_keep) &&
         old_app_state != TEL_CONN_MGR_APP_STATE_INACTIVE))
    {
        TEL_CONN_MGR_LOG_ERR("unexpected app_state:%d", old_app_state);
        TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
        return TEL_CONN_MGR_RET_WRONG_STATE;
    }

    tel_conn_mgr_app_lock_sleep();
    
    TEL_CONN_MGR_LOG_INFO("old_app_state:%d", old_app_state);
    switch (old_app_state)
    {
        case TEL_CONN_MGR_APP_STATE_INACTIVE:
        {
            if (bearer_info)
            {
                TEL_CONN_MGR_LOG_ERR("Find bearer info at INACTIVE");
            }
            else
            {
                bearer_info = tel_conn_mgr_bearer_info_find_free_slot(&bearer_info_idx);
                if (!bearer_info)
                {
                    tel_conn_mgr_app_unlock_sleep();
                    TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
                    return TEL_CONN_MGR_RET_LIMIT_RESOURCE;
                }

                is_bearer_info_new = TEL_CONN_MGR_TRUE;
            }

            memset(bearer_info, 0, sizeof(tel_conn_mgr_bearer_info_struct));
            bearer_info->is_used = TEL_CONN_MGR_TRUE;
            ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_idx(bearer_info_idx);
            if (!ds_keep)
            {
                bearer_info->is_used = TEL_CONN_MGR_FALSE;
                tel_conn_mgr_app_unlock_sleep();
                TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
                return TEL_CONN_MGR_RET_NOT_FOUND;
            }
            memset(ds_keep, 0, sizeof(tel_conn_mgr_bearer_info_ds_keep_struct));
            ds_keep->bearer_state = TEL_CONN_MGR_BEARER_STATE_INACTIVE;
            ds_keep->data_channel_id = TEL_CONN_MGR_MIN_VALID_CHANNEL_ID - 1;
            bearer_info->bearer_type = bearer_type;
            bearer_info->sim_id = sim_id;
            strncpy(ds_keep->apn, apn, TEL_CONN_MGR_APN_MAX_LEN);
            if (username)
            {
                strncpy(bearer_info->username, username, TEL_CONN_MGR_USERNAME_MAX_LEN);
            }
            if (password)
            {
                strncpy(bearer_info->password, password, TEL_CONN_MGR_PASSWORD_MAX_LEN);
            }
            ds_keep->pdp_type = pdp_type;
            ds_keep->cid = tel_conn_mgr_convt_bearer_info_idx_to_cid(bearer_info_idx);

            ret = tel_conn_mgr_add_app_info(queue_hdl,
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
                                            reg_hdl,
#endif
                                            pdp_type, TEL_CONN_MGR_FALSE, TEL_CONN_MGR_ACT_ACT,
                                            bearer_info_idx, app_id, &thread_info, &app_info);
            if (TEL_CONN_MGR_RET_OK != ret)
            {
                TEL_CONN_MGR_LOG_INFO("add_app_info fail ret:%d", ret);
                goto exit;
            }

            tel_conn_mgr_app_update_app_state(bearer_info_idx, TEL_CONN_MGR_APP_STATE_ACTIVATING);

            //TEL_CONN_MGR_LOG_INFO("bearer_info_idx:%d, app_state:%d", bearer_info_idx, tel_conn_mgr_app_get_app_state(bearer_info_idx));
            ret = tel_conn_mgr_send_act_req(ds_keep->cid);
            if (TEL_CONN_MGR_RET_OK != ret)
            {
                TEL_CONN_MGR_LOG_INFO("send_act_req fail ret:%d", ret);
                goto exit;
            }

            tel_conn_mgr_app_unlock_sleep();
            TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
            return TEL_CONN_MGR_RET_WOULDBLOCK;
        }

        case TEL_CONN_MGR_APP_STATE_ACTIVATING:
        {
            ret = tel_conn_mgr_is_pdp_type_compatible(pdp_type,
                                                      ds_keep->pdp_type,
                                                      ds_keep->activated_pdp_type,
                                                      old_app_state);
            if (TEL_CONN_MGR_RET_OK != ret)
            {
                tel_conn_mgr_app_unlock_sleep();
                TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
                return ret;
            }
            
            ret = tel_conn_mgr_add_app_info(queue_hdl,
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
                                            reg_hdl,
#endif
                                            pdp_type, TEL_CONN_MGR_FALSE, TEL_CONN_MGR_ACT_ACT,
                                            bearer_info_idx, app_id, &thread_info, &app_info);
            if (TEL_CONN_MGR_RET_OK != ret)
            {
                goto exit;
            }

            tel_conn_mgr_app_unlock_sleep();
            TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
            return TEL_CONN_MGR_RET_WOULDBLOCK;
        }

        case TEL_CONN_MGR_APP_STATE_ACTIVE:
        {
            ret = tel_conn_mgr_is_pdp_type_compatible(pdp_type,
                                                      ds_keep->pdp_type,
                                                      ds_keep->activated_pdp_type,
                                                      old_app_state);
            if (TEL_CONN_MGR_RET_OK != ret)
            {
                tel_conn_mgr_app_unlock_sleep();
                TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
                return ret;
            }
            
            ret = tel_conn_mgr_add_app_info(queue_hdl,
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
                                            reg_hdl,
#endif
                                            pdp_type, TEL_CONN_MGR_FALSE, TEL_CONN_MGR_ACT_ACT,
                                            bearer_info_idx, app_id, &thread_info, &app_info);
            if (TEL_CONN_MGR_RET_OK != ret)
            {
                goto exit;
            }
            
            *activated_pdp_type = ds_keep->activated_pdp_type;
            tel_conn_mgr_app_unlock_sleep();
            TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
            return TEL_CONN_MGR_RET_OK;
        }

        case TEL_CONN_MGR_APP_STATE_DEACTIVATING:
        {
            ret = tel_conn_mgr_is_pdp_type_compatible(pdp_type,
                                                      ds_keep->pdp_type,
                                                      TEL_CONN_MGR_PDP_TYPE_NONE,
                                                      old_app_state);
            /* Turn into activation if pdp type is compatible. Otherwise, turn into reactive. */
            if (TEL_CONN_MGR_RET_OK == ret)
            {
                ret = tel_conn_mgr_add_app_info(queue_hdl,
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
                                                reg_hdl,
#endif
                                                pdp_type, TEL_CONN_MGR_FALSE, TEL_CONN_MGR_ACT_ACT,
                                                bearer_info_idx, app_id, &thread_info, &app_info);
                if (TEL_CONN_MGR_RET_OK != ret)
                {
                    goto exit;
                }

                tel_conn_mgr_app_update_app_state(bearer_info_idx, TEL_CONN_MGR_APP_STATE_ACTIVATING);

                ret = tel_conn_mgr_send_act_req(ds_keep->cid);
                if (TEL_CONN_MGR_RET_OK != ret)
                {
                    goto exit;
                }
            }
            else
            {
                ret = tel_conn_mgr_add_app_info(queue_hdl,
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
                                                reg_hdl,
#endif
                                                pdp_type, TEL_CONN_MGR_FALSE, TEL_CONN_MGR_ACT_REACT,
                                                bearer_info_idx, app_id, &thread_info, &app_info);
                if (TEL_CONN_MGR_RET_OK != ret)
                {
                    goto exit;
                }

                bearer_info->reactive_pdp_type = pdp_type;

                tel_conn_mgr_app_update_app_state(bearer_info_idx, TEL_CONN_MGR_APP_STATE_REACTIVE);
            }

            tel_conn_mgr_app_unlock_sleep();
            TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
            return TEL_CONN_MGR_RET_WOULDBLOCK;
        }

        case TEL_CONN_MGR_APP_STATE_REACTIVE:
        {
            ret = tel_conn_mgr_is_pdp_type_compatible(pdp_type,
                                                      bearer_info->reactive_pdp_type,
                                                      TEL_CONN_MGR_PDP_TYPE_NONE,
                                                      old_app_state);
            if (TEL_CONN_MGR_RET_OK != ret)
            {
                tel_conn_mgr_app_unlock_sleep();
                TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
                return ret;
            }
            
            ret = tel_conn_mgr_add_app_info(queue_hdl,
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
                                            reg_hdl,
#endif
                                            pdp_type, TEL_CONN_MGR_FALSE, TEL_CONN_MGR_ACT_REACT,
                                            bearer_info_idx, app_id, &thread_info, &app_info);
            if (TEL_CONN_MGR_RET_OK != ret)
            {
                goto exit;
            }

            tel_conn_mgr_app_unlock_sleep();
            TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
            return TEL_CONN_MGR_RET_WOULDBLOCK;
        }

        default:
        {
            ret = TEL_CONN_MGR_RET_ERROR;
            TEL_CONN_MGR_LOG_ERR("Won't reach here.");
            goto exit;
        }
    }        

exit:

    if (TEL_CONN_MGR_RET_OK != ret && TEL_CONN_MGR_RET_WOULDBLOCK != ret)
    {
        tel_conn_mgr_app_update_app_state(bearer_info_idx, old_app_state);

        if (thread_info || app_info)
        {
            tel_conn_mgr_free_app_info(thread_info, app_info, bearer_info_idx);
        }

        if (is_bearer_info_new)
        {
            tel_conn_mgr_bearer_info_free(bearer_info_idx);
        }
    }

    tel_conn_mgr_app_unlock_sleep();
    TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
    return ret;
}


tel_conn_mgr_ret_enum tel_conn_mgr_activate(tel_conn_mgr_bearer_type_enum bearer_type,
                                         tel_conn_mgr_sim_id_enum sim_id,
                                         tel_conn_mgr_pdp_type_enum pdp_type,
                                         char *apn,
                                         char *username,
                                         char *password,
                                         tel_conn_mgr_queue_hdl_t queue_hdl,
                                         unsigned int *app_id,
                                         tel_conn_mgr_pdp_type_enum *activated_pdp_type)
{
    return tel_conn_mgr_activate_int(bearer_type,
                                     sim_id,
                                     pdp_type,
                                     apn,
                                     username,
                                     password,
                                     queue_hdl,
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
                                     NULL,
#endif
                                     app_id,
                                     activated_pdp_type);
}


tel_conn_mgr_ret_enum tel_conn_mgr_activate_ext(tel_conn_mgr_bearer_type_enum bearer_type,
                                                       tel_conn_mgr_sim_id_enum sim_id,
                                                       tel_conn_mgr_pdp_type_enum pdp_type,
                                                       char *apn,
                                                       char *username,
                                                       char *password,
                                                       tel_conn_mgr_register_handle_t reg_hdl,
                                                       unsigned int *app_id,
                                                       tel_conn_mgr_pdp_type_enum *activated_pdp_type)
{
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
    return tel_conn_mgr_activate_int(bearer_type,
                                     sim_id,
                                     pdp_type,
                                     apn,
                                     username,
                                     password,
                                     NULL,
                                     reg_hdl,
                                     app_id,
                                     activated_pdp_type);
#else
    return TEL_CONN_MGR_RET_ERROR;
#endif
}


tel_conn_mgr_ret_enum tel_conn_mgr_deactivate(unsigned int app_id)
{
    tel_conn_mgr_thread_info_struct *thread_info = NULL;
    tel_conn_mgr_app_info_struct *app_info = NULL;
    tel_conn_mgr_bearer_info_struct *bearer_info = NULL;
    tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep = NULL;
    unsigned int bearer_info_idx = TEL_CONN_MGR_BEARER_TYPE_MAX_NUM;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    tel_conn_mgr_app_state_enum old_app_state = TEL_CONN_MGR_APP_STATE_MAX;
    int cid = TEL_CONN_MGR_MIN_CID - 1;

    TEL_CONN_MGR_LOG_INFO("app_id:%d", app_id);
    
    TEL_CONN_MGR_GLOBAL_MUTEX_LOCK;
    tel_conn_mgr_app_lock_sleep();
    
    ret = tel_conn_mgr_get_app_info(app_id, &thread_info, &app_info, &bearer_info_idx);
    if (TEL_CONN_MGR_RET_OK != ret)
    {
        tel_conn_mgr_app_unlock_sleep();
        TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
        return ret;
    }

    bearer_info = tel_conn_mgr_bearer_info_find_by_idx(bearer_info_idx);
    ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_idx(bearer_info_idx);
    if (!bearer_info || !ds_keep)
    {
        tel_conn_mgr_free_app_info(thread_info, app_info, bearer_info_idx);
        
        tel_conn_mgr_app_unlock_sleep();
        TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
        return TEL_CONN_MGR_RET_NOT_FOUND;
    }
    
    old_app_state = tel_conn_mgr_app_get_app_state(bearer_info_idx);
    
    TEL_CONN_MGR_LOG_INFO("old_app_state:%d,app_info->act:%d,idx:%d", old_app_state, app_info->act, bearer_info_idx);
    switch (old_app_state)
    {
        case TEL_CONN_MGR_APP_STATE_INACTIVE:
        {
            TEL_CONN_MGR_LOG_WARN("Deactivate at INVACTIVE state.");
            tel_conn_mgr_free_app_info(thread_info, app_info, bearer_info_idx);
            
            tel_conn_mgr_app_unlock_sleep();
            TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
            return TEL_CONN_MGR_RET_ERROR;
        }

        case TEL_CONN_MGR_APP_STATE_ACTIVATING:
        case TEL_CONN_MGR_APP_STATE_ACTIVE:
        {
            if (TEL_CONN_MGR_ACT_ACT != app_info->act)
            {
                TEL_CONN_MGR_LOG_WARN("Wrong action:%d.", app_info->act);                
                tel_conn_mgr_free_app_info(thread_info, app_info, bearer_info_idx);
                
                tel_conn_mgr_app_unlock_sleep();
                TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
                return TEL_CONN_MGR_RET_ERROR;
            }

            /* Send DEACT REQ MSG to tel conn mgr task. */
            if (!tel_conn_mgr_does_specified_app_exist(app_id, bearer_info_idx, TEL_CONN_MGR_ACT_ACT))
            {
                /* Bearer Part will terminate the activating and notify app. */
                ret = tel_conn_mgr_send_deact_req(ds_keep->cid);                
                if (TEL_CONN_MGR_RET_OK == ret)
                {
                    tel_conn_mgr_app_update_app_state(bearer_info_idx, TEL_CONN_MGR_APP_STATE_DEACTIVATING);
                    app_info->act = TEL_CONN_MGR_ACT_DEACT;
                    
                    tel_conn_mgr_app_unlock_sleep();
                    TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
                    return TEL_CONN_MGR_RET_WOULDBLOCK;
                }
                else
                {
                    //tel_conn_mgr_app_update_app_state(bearer_info_idx, old_app_state);
                    //tel_conn_mgr_free_app_info(thread_info, app_info, bearer_info_idx);
                    TEL_CONN_MGR_LOG_ERR("Failed to send deact req. ret:%d", ret);

                    ret = TEL_CONN_MGR_RET_ERROR;
                }
            }
            else
            {
                /* Notify App that Activation fails for it is cancelled. */
                if (TEL_CONN_MGR_APP_STATE_ACTIVATING == old_app_state)
                {
                    cid = tel_conn_mgr_convt_bearer_info_idx_to_cid(bearer_info_idx);
                    if (tel_conn_mgr_is_cid_valid(cid) && !tel_conn_mgr_is_special_cid(cid))
                    {
                        tel_conn_mgr_notify_app(TEL_CONN_MGR_APP_MSG_TYPE_ACTIVATION,
                                                cid,
                                                TEL_CONN_MGR_FALSE,
                                                TEL_CONN_MGR_ERR_CAUSE_DEACT_REQ,
                                                thread_info,
                                                app_info);
                        
                    }
                }
                ret = tel_conn_mgr_free_app_info(thread_info, app_info, bearer_info_idx);
                TEL_CONN_MGR_LOG_ERR("tel_conn_mgr_free_app_info() ret:%d", ret);
                ret = TEL_CONN_MGR_RET_IS_HOLD;
            }

            tel_conn_mgr_app_unlock_sleep();
            TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
            return ret;
        }

        case TEL_CONN_MGR_APP_STATE_DEACTIVATING:
        {
            if (TEL_CONN_MGR_ACT_DEACT == app_info->act)
            {
                TEL_CONN_MGR_LOG_ERR("Duplicated deactivation request.");
                
                tel_conn_mgr_app_unlock_sleep();
                TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
                return TEL_CONN_MGR_RET_DUPLICATION;
            }
            else
            {
                TEL_CONN_MGR_LOG_ERR("Wrong action:%d.", app_info->act);
                tel_conn_mgr_free_app_info(thread_info, app_info, bearer_info_idx);
                
                tel_conn_mgr_app_unlock_sleep();
                TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
                return TEL_CONN_MGR_RET_ERROR;
            }
        }

        case TEL_CONN_MGR_APP_STATE_REACTIVE:
        {
            TEL_CONN_MGR_LOG_INFO("pdp type:%d at REACTIVE", ds_keep->pdp_type);
            
            if (TEL_CONN_MGR_ACT_REACT == app_info->act)
            {
                if (!tel_conn_mgr_does_specified_app_exist(app_id, bearer_info_idx, TEL_CONN_MGR_ACT_REACT))
                {
                    bearer_info->reactive_pdp_type = TEL_CONN_MGR_PDP_TYPE_NONE;
                    tel_conn_mgr_app_update_app_state(bearer_info_idx, TEL_CONN_MGR_APP_STATE_DEACTIVATING);
                }

                tel_conn_mgr_free_app_info(thread_info, app_info, bearer_info_idx);
                
                tel_conn_mgr_app_unlock_sleep();
                TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
                return TEL_CONN_MGR_RET_OK;
            }
            else if (TEL_CONN_MGR_ACT_DEACT == app_info->act)
            {
                TEL_CONN_MGR_LOG_ERR("Duplicated deactivation request.");
                
                tel_conn_mgr_app_unlock_sleep();
                TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
                return TEL_CONN_MGR_RET_DUPLICATION;
            }
            else
            {
                TEL_CONN_MGR_LOG_ERR("Wrong action:%d.", app_info->act);
                tel_conn_mgr_free_app_info(thread_info, app_info, bearer_info_idx);
                
                tel_conn_mgr_app_unlock_sleep();
                TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
                return TEL_CONN_MGR_RET_ERROR;
            }
        }

        default:
        {
            break;
        }
    }

    tel_conn_mgr_free_app_info(thread_info, app_info, bearer_info_idx);
    
    tel_conn_mgr_app_unlock_sleep();
    TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK;
    return TEL_CONN_MGR_RET_ERROR;
}

