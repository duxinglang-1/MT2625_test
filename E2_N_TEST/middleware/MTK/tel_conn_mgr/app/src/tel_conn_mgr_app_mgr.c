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
#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
#include "hal_rtc_external.h"
#endif
#include "tel_conn_mgr_bearer_info.h"
#include "tel_conn_mgr_app_mgr.h"
#include "tel_conn_mgr_util.h"
#include "tel_conn_mgr_bearer_util.h"



#ifndef TEL_CONN_MGR_SUPPORT_CALLBACK
#define TEL_CONN_MGR_MIN_VALID_APP_ID    (1)
#define TEL_CONN_MGR_MAX_VALID_APP_ID    (500)

/* App id range for message feedback */
#define TEL_CONN_MGR_MIN_VALID_MSG_APP_ID TEL_CONN_MGR_MIN_VALID_APP_ID
#define TEL_CONN_MGR_MAX_VALID_MSG_APP_ID TEL_CONN_MGR_MAX_VALID_APP_ID
#else
#define TEL_CONN_MGR_MIN_VALID_APP_ID    (1)
#define TEL_CONN_MGR_MAX_VALID_APP_ID    (1024)

/* App id range for message feedback */
#define TEL_CONN_MGR_MIN_VALID_MSG_APP_ID TEL_CONN_MGR_MIN_VALID_APP_ID
#define TEL_CONN_MGR_MAX_VALID_MSG_APP_ID (500)

/* App id range for callback feedback */
#define TEL_CONN_MGR_MIN_VALID_CB_APP_ID (TEL_CONN_MGR_MAX_VALID_MSG_APP_ID + 1)
#define TEL_CONN_MGR_MAX_VALID_CB_APP_ID TEL_CONN_MGR_MAX_VALID_APP_ID
#endif

#define TEL_CONN_MGR_APP_SLEEP_MANAGER_NAME "tel_conn_mgr_app_sleep_manager"

tel_conn_mgr_app_cntx_struct *tel_conn_mgr_app_cntx = NULL;


/* This API should be called after tel_conn_mgr_bearer_init()
  * If Error is returned, tel_conn_mgr_app_deinit() needs to be called to free the memory. */
tel_conn_mgr_ret_enum tel_conn_mgr_app_init(void)
{
#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
    rtc_power_on_result_t deep_sleep_bootup = 0;
    tel_conn_mgr_bearer_info_struct *bearer_info = NULL;
#endif

    if (tel_conn_mgr_app_cntx)
    {
        TEL_CONN_MGR_LOG_ERR("app_cntx has been initialized.");
        return TEL_CONN_MGR_RET_OK;
    }

    tel_conn_mgr_app_cntx = (tel_conn_mgr_app_cntx_struct *)tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_app_cntx_struct));
    if (!tel_conn_mgr_app_cntx)
    {
        return TEL_CONN_MGR_RET_NOT_ENOUGH_MEMORY;
    }

#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
    tel_conn_mgr_app_cntx->sleep_manager_handle = tel_conn_mgr_set_sleep_handle(TEL_CONN_MGR_APP_SLEEP_MANAGER_NAME);
    if (TEL_CONN_MGR_SLEEP_LOCK_INVALID_ID == tel_conn_mgr_app_cntx->sleep_manager_handle)
    {
        TEL_CONN_MGR_LOG_ERR("Failed to create the sleep manager for app");
        return TEL_CONN_MGR_RET_ERROR;
    }

    /* Update app state */
    deep_sleep_bootup = rtc_power_on_result_external();
    if (deep_sleep_bootup == DEEP_SLEEP || deep_sleep_bootup == DEEPER_SLEEP)
    {
        for (int i = 0; i < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; i++)
        {
            bearer_info = tel_conn_mgr_bearer_info_find_by_idx(i);
            /* If bearer_state is not active, bearer_info is not used. */
            if (!bearer_info || !bearer_info->is_used)
            {
                continue;
            }
            
            tel_conn_mgr_app_cntx->app_state[i] = TEL_CONN_MGR_APP_STATE_ACTIVE;
        }
    }
#endif

    return TEL_CONN_MGR_RET_OK;
}


void tel_conn_mgr_app_lock_sleep(void)
{
#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
    tel_conn_mgr_general_lock_sleep(tel_conn_mgr_app_cntx->sleep_manager_handle,
                                    &tel_conn_mgr_app_cntx->sleep_lock_count);
#endif
}


void tel_conn_mgr_app_unlock_sleep(void)
{
#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
    tel_conn_mgr_general_unlock_sleep(tel_conn_mgr_app_cntx->sleep_manager_handle,
                                    &tel_conn_mgr_app_cntx->sleep_lock_count);
#endif
}


void tel_conn_mgr_app_deinit(void)
{
    if (tel_conn_mgr_app_cntx)
    {
        tel_conn_mgr_free(tel_conn_mgr_app_cntx);
        tel_conn_mgr_app_cntx = NULL;
    }
}


tel_conn_mgr_app_cntx_struct *tel_conn_mgr_app_get_cntx(void)
{
    return tel_conn_mgr_app_cntx;
}


unsigned int tel_conn_mgr_gen_app_id(tel_conn_mgr_bool *overflowed)
{
    static unsigned int last_used = TEL_CONN_MGR_MIN_VALID_MSG_APP_ID - 1;
    static tel_conn_mgr_bool overflowed_internal = TEL_CONN_MGR_FALSE;

    assert(TEL_CONN_MGR_MIN_VALID_MSG_APP_ID > 0);
    assert(TEL_CONN_MGR_MIN_VALID_MSG_APP_ID <= TEL_CONN_MGR_MAX_VALID_MSG_APP_ID);
    if (!overflowed_internal && (last_used == TEL_CONN_MGR_MAX_VALID_MSG_APP_ID))
    {
        overflowed_internal = TEL_CONN_MGR_TRUE;
    }

    if (overflowed)
    {
        *overflowed = overflowed_internal;
    }

    if (TEL_CONN_MGR_MIN_VALID_MSG_APP_ID > ((++last_used) % (TEL_CONN_MGR_MAX_VALID_MSG_APP_ID + 1)))
    {
        last_used = TEL_CONN_MGR_MIN_VALID_MSG_APP_ID;        
    }

    return last_used;
}


#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
unsigned int tel_conn_mgr_gen_cb_app_id(tel_conn_mgr_bool *overflowed)
{
    static unsigned int last_used = TEL_CONN_MGR_MIN_VALID_CB_APP_ID - 1;
    static tel_conn_mgr_bool overflowed_internal = TEL_CONN_MGR_FALSE;

    assert(TEL_CONN_MGR_MIN_VALID_CB_APP_ID > 0);
    assert(TEL_CONN_MGR_MIN_VALID_CB_APP_ID <= TEL_CONN_MGR_MAX_VALID_CB_APP_ID);
    if (!overflowed_internal && (last_used == TEL_CONN_MGR_MAX_VALID_CB_APP_ID))
    {
        overflowed_internal = TEL_CONN_MGR_TRUE;
    }

    if (overflowed)
    {
        *overflowed = overflowed_internal;
    }

    if (TEL_CONN_MGR_MIN_VALID_CB_APP_ID > ((++last_used) % (TEL_CONN_MGR_MAX_VALID_CB_APP_ID + 1)))
    {
        last_used = TEL_CONN_MGR_MIN_VALID_CB_APP_ID;        
    }

    return last_used;
}
#endif

tel_conn_mgr_bool tel_conn_mgr_is_app_id_used(unsigned int app_id)
{
    tel_conn_mgr_thread_info_struct *thread_info_tmp = tel_conn_mgr_app_cntx ? tel_conn_mgr_app_cntx->thread_info_list : NULL;
    tel_conn_mgr_app_info_struct *app_info = NULL;
    int i = 0;
    
    while (thread_info_tmp)
    {
        for (i = 0; i < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; i++)
        {
            app_info = thread_info_tmp->app_info[i];
            while (app_info)
            {
                if (app_info->app_id == app_id)
                {
                    return TEL_CONN_MGR_TRUE;
                }

                app_info = app_info->next;
            }
        }
        thread_info_tmp = thread_info_tmp->next;
    }

    return TEL_CONN_MGR_FALSE;

#if 0

    return app_id & 0x80000000 ? TEL_CONN_MGR_TRUE : TEL_CONN_MGR_FALSE;
#endif
}


unsigned int tel_conn_mgr_get_an_unused_app_id(void)
{
    tel_conn_mgr_bool overflowed = TEL_CONN_MGR_FALSE;
    unsigned int app_id = tel_conn_mgr_gen_app_id(&overflowed);
    int i = 0;

    TEL_CONN_MGR_LOG_INFO("%s, app_id:%d, overflowed:%d", __FUNCTION__, app_id, overflowed);
    if (overflowed)
    {
        for (i = TEL_CONN_MGR_MIN_VALID_APP_ID; i <= TEL_CONN_MGR_MAX_VALID_APP_ID; i++)
        {
            if (!tel_conn_mgr_is_app_id_used(app_id))
            {
                return app_id;
            }

            app_id = tel_conn_mgr_gen_app_id(NULL);
        }

        return 0;
    }

    return app_id;
}


#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
unsigned int tel_conn_mgr_get_an_unused_cb_app_id(void)
{
    tel_conn_mgr_bool overflowed = TEL_CONN_MGR_FALSE;
    unsigned int app_id = tel_conn_mgr_gen_cb_app_id(&overflowed);
    int i = 0;

    TEL_CONN_MGR_LOG_INFO("%s, app_id:%d, overflowed:%d", __FUNCTION__, app_id, overflowed);
    if (overflowed)
    {
        for (i = TEL_CONN_MGR_MIN_VALID_APP_ID; i <= TEL_CONN_MGR_MAX_VALID_APP_ID; i++)
        {
            if (!tel_conn_mgr_is_app_id_used(app_id))
            {
                return app_id;
            }

            app_id = tel_conn_mgr_gen_cb_app_id(NULL);
        }

        return 0;
    }

    return app_id;
}
#endif

tel_conn_mgr_app_info_struct *tel_conn_mgr_gen_app_info(unsigned int *app_id,
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
                                                                tel_conn_mgr_bool is_registered,
#endif
                                                                tel_conn_mgr_pdp_type_enum pdp_type,
                                                                tel_conn_mgr_bool is_act_req_cached,
                                                                tel_conn_mgr_act_enum act)
{
    tel_conn_mgr_app_info_struct *app_info = NULL;

    if (!app_id || pdp_type <= TEL_CONN_MGR_PDP_TYPE_NONE || pdp_type >= TEL_CONN_MGR_PDP_TYPE_MAX)
    {
        return NULL;
    }

#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
    if (is_registered)
    {
        *app_id = tel_conn_mgr_get_an_unused_cb_app_id();
        TEL_CONN_MGR_LOG_INFO("get callback app id:%d", *app_id);
    }
    else
#endif
    {
        *app_id = tel_conn_mgr_get_an_unused_app_id();
        TEL_CONN_MGR_LOG_INFO("get message app id:%d", *app_id);
    }

    if (!(*app_id))
    {
        return NULL;
    }

    app_info = tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_app_info_struct));
    if (!app_info)
    {
        *app_id = 0;

        TEL_CONN_MGR_LOG_INFO("fail to alloc mem for app info");
        return NULL;
    }
    
    app_info->app_id = *app_id;
    app_info->pdp_type = pdp_type;
    app_info->is_act_req_cached = is_act_req_cached;
    app_info->act = act;

    return app_info;
}


tel_conn_mgr_thread_info_struct *tel_conn_mgr_thread_info_find(tel_conn_mgr_queue_hdl_t queue_hdl)
{
    tel_conn_mgr_thread_info_struct *thread_info_tmp = tel_conn_mgr_app_cntx ? tel_conn_mgr_app_cntx->thread_info_list : NULL;

    if (!queue_hdl || !thread_info_tmp)
    {
        return NULL;
    }

    while (thread_info_tmp)
    {
        if (thread_info_tmp->queue_hdl == queue_hdl)
        {
            return thread_info_tmp;
        }
        thread_info_tmp = thread_info_tmp->next;
    }

    return NULL;
}


/* The app_info count of the thread's specific bearer type. */
int tel_conn_mgr_thread_bearer_app_info_count(tel_conn_mgr_thread_info_struct *thd_info,
                                                           int bearer_info_idx)
{
    tel_conn_mgr_app_info_struct *app_info = NULL;
    int count = 0;
    
    if (!thd_info || 
        !tel_conn_mgr_is_idx_valid(bearer_info_idx) ||
        tel_conn_mgr_is_special_idx(bearer_info_idx))
    {
        return -1;
    }
    
    app_info = thd_info->app_info[bearer_info_idx];    

    while (app_info)
    {
        count++;
        app_info = app_info->next;
    }

    return count;
}


tel_conn_mgr_ret_enum tel_conn_mgr_free_app_info(tel_conn_mgr_thread_info_struct *thread_info,
                                                 tel_conn_mgr_app_info_struct *app_info,
                                                 unsigned int app_info_idx)
{
    tel_conn_mgr_thread_info_struct *thread_info_tmp = thread_info;
    int i = 0;

    TEL_CONN_MGR_LOG_INFO("idx:%d, app_info:%x", app_info_idx, (unsigned int)app_info);

    if (thread_info_tmp)
    {
        if (app_info)
        {
            if (!tel_conn_mgr_is_idx_valid(app_info_idx) ||
                tel_conn_mgr_is_special_idx(app_info_idx))
            {
                
                return TEL_CONN_MGR_RET_INVALID_PARAM;
            }
            
            tel_conn_mgr_list_remove((tel_conn_mgr_template_list_struct **)&thread_info_tmp->app_info[app_info_idx],
                                     (tel_conn_mgr_template_list_struct *)app_info);
        }

        /* Check if there is still app_info for this thread. */
        for (i = 0; i < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; i++)
        {
            if (thread_info_tmp->app_info[i])
            {
                break;
            }
        }

        /* Free thread_info for there is no app_info. */
        if (TEL_CONN_MGR_BEARER_TYPE_MAX_NUM <= i && tel_conn_mgr_app_cntx)
        {
            tel_conn_mgr_list_remove((tel_conn_mgr_template_list_struct **)&tel_conn_mgr_app_cntx->thread_info_list,
                                     (tel_conn_mgr_template_list_struct *)thread_info_tmp);

            tel_conn_mgr_free(thread_info_tmp);
        }
    }
    
    if (app_info)
    {
        tel_conn_mgr_free(app_info);
    }    

    return TEL_CONN_MGR_RET_OK;
}


tel_conn_mgr_ret_enum tel_conn_mgr_add_app_info(tel_conn_mgr_queue_hdl_t queue_hdl,
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
                                                        tel_conn_mgr_register_handle_t reg_hdl,
#endif
                                                        tel_conn_mgr_pdp_type_enum pdp_type,
                                                        tel_conn_mgr_bool is_act_req_cached,
                                                        tel_conn_mgr_act_enum act,
                                                        unsigned int app_info_idx,
                                                        unsigned int *app_id,
                                                        tel_conn_mgr_thread_info_struct **thread_info,
                                                        tel_conn_mgr_app_info_struct **app_info)
{
    //tel_conn_mgr_thread_info_struct *thread_info_tmp = thread_info ? *thread_info : NULL;
    tel_conn_mgr_thread_info_struct *thread_info_tmp = NULL;
    tel_conn_mgr_app_info_struct *app_info_tmp = app_info ? *app_info : NULL;
    int count = 0;

#if 0
    TEL_CONN_MGR_LOG_INFO("queue_hdl:%x, thread_info:%x, *thread_info:%x, app_id:%x", 
                          queue_hdl, thread_info, *thread_info, app_id);

    TEL_CONN_MGR_LOG_INFO("*app_id:%d, pdp_type:%d, app_info_idx:%d", 
                          *app_id, pdp_type, app_info_idx);
#endif


    if (!thread_info || *thread_info || !app_id || *app_id ||
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
        (reg_hdl && queue_hdl) || (!reg_hdl && !queue_hdl) ||
#else
        !queue_hdl ||
#endif
        TEL_CONN_MGR_PDP_TYPE_NONE >= pdp_type || TEL_CONN_MGR_PDP_TYPE_MAX <= pdp_type ||
        !tel_conn_mgr_is_idx_valid(app_info_idx) ||
        tel_conn_mgr_is_special_idx(app_info_idx))
    {
        TEL_CONN_MGR_LOG_INFO("queue_hdl:%x, thread_info:%x, *thread_info:%d",
                              (unsigned int)queue_hdl, (unsigned int)thread_info, (int)*thread_info);
        TEL_CONN_MGR_LOG_INFO("app_id:%x, *app_id:%d, pdp_type:%d",
                              (unsigned int)app_id, *app_id, pdp_type);
        TEL_CONN_MGR_LOG_INFO("app_info_idx:%d, is_idx_valid:%d, is_special_idx:%d",
                              app_info_idx,
                              tel_conn_mgr_is_idx_valid(app_info_idx),
                              tel_conn_mgr_is_special_idx(app_info_idx));        
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    *app_id = 0;

#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
    if (reg_hdl)    
    {
        thread_info_tmp = NULL;
    }
    else
#endif
    {
        thread_info_tmp = tel_conn_mgr_thread_info_find(queue_hdl);
    }

    if (!thread_info_tmp)
    {
        thread_info_tmp = tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_thread_info_struct));
        if (!thread_info_tmp)
        {
            return TEL_CONN_MGR_RET_NOT_ENOUGH_MEMORY;
        }
    }
    else
    {
        count = tel_conn_mgr_thread_bearer_app_info_count(thread_info_tmp, app_info_idx);
        TEL_CONN_MGR_LOG_INFO("count:%d", count);
        if (count < 0 || count >= TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM)
        {
            return count < 0 ? TEL_CONN_MGR_RET_ERROR : TEL_CONN_MGR_RET_LIMIT_RESOURCE;            
        }
    }

    app_info_tmp = tel_conn_mgr_gen_app_info(app_id,
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
                                             reg_hdl ? TEL_CONN_MGR_TRUE : TEL_CONN_MGR_FALSE,
#endif
                                             pdp_type, is_act_req_cached, act);
    if (!app_info_tmp)
    {
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
        if (!thread_info_tmp->queue_hdl && !thread_info_tmp->reg_hdl)
#else
        if (!thread_info_tmp->queue_hdl)
#endif
        {
            /* thread_info_tmp is new allocated. */
            tel_conn_mgr_free(thread_info_tmp);         
        }

        TEL_CONN_MGR_LOG_INFO("%s %d", __FUNCTION__, __LINE__);
        return TEL_CONN_MGR_RET_ERROR;
    }

    /* Add app_info to list. */
    tel_conn_mgr_list_insert((tel_conn_mgr_template_list_struct **)&thread_info_tmp->app_info[app_info_idx],
                             (tel_conn_mgr_template_list_struct *)app_info_tmp);
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
    if (!thread_info_tmp->queue_hdl && !thread_info_tmp->reg_hdl)
#else
    if (!thread_info_tmp->queue_hdl)
#endif
    {
        /* thread_info_tmp is new allocated. */
        thread_info_tmp->queue_hdl = queue_hdl;
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
        thread_info_tmp->reg_hdl = reg_hdl ;
#endif
        /* Add thread_info to list. */
        tel_conn_mgr_list_insert((tel_conn_mgr_template_list_struct **)&tel_conn_mgr_app_cntx->thread_info_list,
                                 (tel_conn_mgr_template_list_struct *)thread_info_tmp);

    }

    *thread_info = thread_info_tmp;
    *app_info = app_info_tmp;
    
    return TEL_CONN_MGR_RET_OK;
}


tel_conn_mgr_ret_enum tel_conn_mgr_get_app_info(unsigned int app_id,
                                                        tel_conn_mgr_thread_info_struct **thread_info,
                                                        tel_conn_mgr_app_info_struct **app_info,
                                                        unsigned int *bearer_info_idx)
{
    tel_conn_mgr_thread_info_struct *thread_info_tmp = tel_conn_mgr_app_cntx ? tel_conn_mgr_app_cntx->thread_info_list : NULL;
    tel_conn_mgr_app_info_struct *app_info_tmp = NULL;
    unsigned int idx = 0;
    
    if (!app_id || !thread_info || !app_info ||
        *thread_info || *app_info)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    *bearer_info_idx = TEL_CONN_MGR_BEARER_TYPE_MAX_NUM;

    while (thread_info_tmp)
    {
        for (idx = 0; idx < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; idx++)
        {
            app_info_tmp = thread_info_tmp->app_info[idx];
            while (app_info_tmp)
            {
                if (app_info_tmp->app_id == app_id)
                {
                    *bearer_info_idx = idx;
                    *app_info = app_info_tmp;
                    *thread_info = thread_info_tmp;
                    return TEL_CONN_MGR_RET_OK;
                }
                app_info_tmp = app_info_tmp->next;
            }
        }
        thread_info_tmp = thread_info_tmp->next;
    }

    return TEL_CONN_MGR_RET_NOT_FOUND;    
}


tel_conn_mgr_bool tel_conn_mgr_does_specified_app_exist(unsigned int excluded_app_id,
                                                                   int bearer_info_idx,
                                                                   tel_conn_mgr_act_enum act)
{
    tel_conn_mgr_thread_info_struct *thread_info_tmp = tel_conn_mgr_app_cntx ? tel_conn_mgr_app_cntx->thread_info_list : NULL;
    tel_conn_mgr_app_info_struct *app_info_tmp = NULL;
    
    if (!tel_conn_mgr_is_idx_valid(bearer_info_idx) ||
        tel_conn_mgr_is_special_idx(bearer_info_idx))
    {
        TEL_CONN_MGR_LOG_INFO("1111######");
        return TEL_CONN_MGR_FALSE;
    }    

#if 0
    while (thread_info_tmp)
    {
        app_info_tmp = thread_info_tmp->app_info[bearer_info_idx];

        while (app_info_tmp)
        {
            TEL_CONN_MGR_LOG_ERR("excluded_app_id:%d, app_id:%d, act:%d, app_act:%d, app_info:%x",
                                 excluded_app_id, app_info_tmp->app_id, act, app_info_tmp->act, app_info_tmp);            

            app_info_tmp = app_info_tmp->next;
        }
        
        thread_info_tmp = thread_info_tmp->next;
    }

    thread_info_tmp = tel_conn_mgr_app_cntx ? tel_conn_mgr_app_cntx->thread_info_list : NULL;
#endif

    while (thread_info_tmp)
    {
        app_info_tmp = thread_info_tmp->app_info[bearer_info_idx];

        while (app_info_tmp)
        {
            if (app_info_tmp->app_id != excluded_app_id &&
                (app_info_tmp->act == act || TEL_CONN_MGR_ACT_NONE == act))
            {
               
               TEL_CONN_MGR_LOG_INFO("######");
                return TEL_CONN_MGR_TRUE;
            }

            app_info_tmp = app_info_tmp->next;
        }
        
        thread_info_tmp = thread_info_tmp->next;
    }

    return TEL_CONN_MGR_FALSE;
}


#if 0
tel_conn_mgr_ret_enum tel_conn_mgr_free_bearer_related_app_info(int bearer_info_idx,
                                                                            tel_conn_mgr_act_enum act)
{
    tel_conn_mgr_thread_info_struct *thread_info_tmp = tel_conn_mgr_app_cntx ? tel_conn_mgr_app_cntx->thread_info_list : NULL;
    tel_conn_mgr_thread_info_struct *thread_info_next = NULL;
    tel_conn_mgr_app_info_struct *app_info_tmp = NULL, *app_info_next = NULL;
    tel_conn_mgr_queue_hdl_t queue_hdl = NULL;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_OK;
    unsigned int act_app_ids[TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM] = {0};
    unsigned int deact_app_ids[TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM] = {0};
    int i = -1;
    
    if (!tel_conn_mgr_is_idx_valid(bearer_info_idx) ||
        tel_conn_mgr_is_special_idx(bearer_info_idx))
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    while (thread_info_tmp)
    {
        /* tel_conn_mgr_free_app_info() may set thread_info_tmp to NULL. */
        thread_info_next = thread_info_tmp->next;
        memset(act_app_ids, 0, sizeof(act_app_ids));
        memset(deact_app_ids, 0, sizeof(deact_app_ids));
        queue_hdl = thread_info_tmp->queue_hdl;
        app_info_tmp = thread_info_tmp->app_info[bearer_info_idx];
        while (app_info_tmp)
        {
            /* app_info_tmp may be freed by tel_conn_mgr_free_app_info() */
            app_info_next = app_info_tmp->next;
            if (TEL_CONN_MGR_ACT_NONE == act ||
                (TEL_CONN_MGR_ACT_ACT == act && act == app_info_tmp->act) ||
                (TEL_CONN_MGR_ACT_DEACT == act && act == app_info_tmp->act))
            {
                if (TEL_CONN_MGR_ACT_NONE == act ||
                    (TEL_CONN_MGR_ACT_ACT == act && act == app_info_tmp->act))
                {
                    i = 0;
                    while (TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM > i)
                    {
                        if (act_app_ids[i])
                        {
                            i++;
                        }
                        else
                        {
                            break;
                        }
                    }

                    if (TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM > i)
                    {
                        act_app_ids[i] = app_info_tmp->app_id;
                    }
                    else
                    {
                        TEL_CONN_MGR_LOG_WARN("[act]The num of app info exceeds max num:%d", TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM);
                    }
                }
                else if (TEL_CONN_MGR_ACT_NONE == act ||
                         (TEL_CONN_MGR_ACT_DEACT == act && act == app_info_tmp->act))
                {
                    i = 0;
                    while (TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM > i)
                    {
                        if (deact_app_ids[i])
                        {
                            i++;
                        }
                        else
                        {
                            break;
                        }
                    }

                    if (TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM > i)
                    {
                        deact_app_ids[i] = app_info_tmp->app_id;
                    }
                    else
                    {
                        TEL_CONN_MGR_LOG_WARN("[deact]The num of app info exceeds max num:%d", TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM);
                    }
                }

                ret = tel_conn_mgr_free_app_info(thread_info_tmp,
                                                 app_info_tmp,
                                                 bearer_info_idx);
                if (TEL_CONN_MGR_RET_OK != ret)
                {
                    TEL_CONN_MGR_LOG_ERR("Failed to free app info. ret:%d", ret);                    
                }
            }

            app_info_tmp = app_info_next;
        }

        if (act_app_ids[0])
        {
            // TODO: send ACT result MSG to APP
        }
        
        if (deact_app_ids[0])
        {
            // TODO: send DEACT result MSG to APP
        }

        thread_info_tmp = thread_info_next;
    }
}
#endif


tel_conn_mgr_app_state_enum tel_conn_mgr_app_get_app_state(unsigned int idx)
{
    tel_conn_mgr_app_state_enum app_state = TEL_CONN_MGR_APP_STATE_INACTIVE;
    
    if (!tel_conn_mgr_is_idx_valid(idx) ||
        tel_conn_mgr_is_special_idx(idx))
    {
        return TEL_CONN_MGR_APP_STATE_MAX;
    }

    app_state = tel_conn_mgr_app_cntx ? tel_conn_mgr_app_cntx->app_state[idx] : TEL_CONN_MGR_APP_STATE_MAX;
    
    TEL_CONN_MGR_LOG_INFO("app_state:%d, idx:%d", app_state, idx);
    return app_state;
}


tel_conn_mgr_ret_enum tel_conn_mgr_app_update_app_state(unsigned int idx,
                                                                   tel_conn_mgr_app_state_enum app_state)
{
    //TEL_CONN_MGR_LOG_INFO("idx:%d, app_state:%d", idx, app_state);
    if (!tel_conn_mgr_is_idx_valid(idx) ||
        tel_conn_mgr_is_special_idx(idx) ||
        TEL_CONN_MGR_APP_STATE_INACTIVE > app_state ||
        TEL_CONN_MGR_APP_STATE_MAX <= app_state ||
        !tel_conn_mgr_app_cntx)
    {
        #if 0
        TEL_CONN_MGR_LOG_INFO("is_idx_valid:%d", tel_conn_mgr_is_idx_valid(idx));
        TEL_CONN_MGR_LOG_INFO("is_special_idx:%d", tel_conn_mgr_is_special_idx(idx));
        TEL_CONN_MGR_LOG_INFO("%d", TEL_CONN_MGR_APP_STATE_INACTIVE > app_state);
        TEL_CONN_MGR_LOG_INFO("%d", TEL_CONN_MGR_APP_STATE_MAX <= app_state);
        TEL_CONN_MGR_LOG_INFO("tel_conn_mgr_app_cntx:%x", tel_conn_mgr_app_cntx);
        #endif
        TEL_CONN_MGR_LOG_ERR("Invalid param");
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    tel_conn_mgr_app_cntx->app_state[idx] = app_state;
    TEL_CONN_MGR_LOG_INFO("app_state[%d]:%d", idx, tel_conn_mgr_app_cntx->app_state[idx]);
    return TEL_CONN_MGR_RET_OK;
}


tel_conn_mgr_ret_enum tel_conn_mgr_is_pdp_type_compatible(tel_conn_mgr_pdp_type_enum pdp_type_new,
                                                          tel_conn_mgr_pdp_type_enum pdp_type_used,
                                                          tel_conn_mgr_pdp_type_enum activated_pdp_type,
                                                          tel_conn_mgr_app_state_enum app_state)
{
    if (TEL_CONN_MGR_PDP_TYPE_NONE >= pdp_type_new || TEL_CONN_MGR_PDP_TYPE_MAX <= pdp_type_new ||
        TEL_CONN_MGR_PDP_TYPE_NONE >= pdp_type_used || TEL_CONN_MGR_PDP_TYPE_MAX <= pdp_type_used ||
        TEL_CONN_MGR_PDP_TYPE_NONE > activated_pdp_type || TEL_CONN_MGR_PDP_TYPE_MAX <= activated_pdp_type ||
        TEL_CONN_MGR_APP_STATE_INACTIVE > app_state || TEL_CONN_MGR_APP_STATE_MAX <= app_state)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    switch (app_state)
    {
        case TEL_CONN_MGR_APP_STATE_INACTIVE:
        {
            return TEL_CONN_MGR_RET_WRONG_STATE;
        }
        
        case TEL_CONN_MGR_APP_STATE_ACTIVATING:
        {
            if ((TEL_CONN_MGR_PDP_TYPE_NIDD == pdp_type_new && pdp_type_used == pdp_type_new) ||
                (TEL_CONN_MGR_PDP_TYPE_IP == pdp_type_used && (TEL_CONN_MGR_PDP_TYPE_IP == pdp_type_new ||
                 TEL_CONN_MGR_PDP_TYPE_IPV4V6 == pdp_type_new)) || (TEL_CONN_MGR_PDP_TYPE_IPV6 == pdp_type_used &&
                 (TEL_CONN_MGR_PDP_TYPE_IPV6 == pdp_type_new || TEL_CONN_MGR_PDP_TYPE_IPV4V6 == pdp_type_new)) ||
                 TEL_CONN_MGR_PDP_TYPE_IPV4V6 == pdp_type_used)
            
            {
                return TEL_CONN_MGR_RET_OK;
            }
        }
        
        case TEL_CONN_MGR_APP_STATE_REACTIVE:
        {
            if (TEL_CONN_MGR_PDP_TYPE_NIDD == pdp_type_new)
            {
                return TEL_CONN_MGR_RET_WRONG_STATE;
            }
            
            if ((TEL_CONN_MGR_PDP_TYPE_IP == pdp_type_used && (TEL_CONN_MGR_PDP_TYPE_IP == pdp_type_new ||
                 TEL_CONN_MGR_PDP_TYPE_IPV4V6 == pdp_type_new)) || (TEL_CONN_MGR_PDP_TYPE_IPV6 == pdp_type_used &&
                 (TEL_CONN_MGR_PDP_TYPE_IPV6 == pdp_type_new || TEL_CONN_MGR_PDP_TYPE_IPV4V6 == pdp_type_new)) ||
                 TEL_CONN_MGR_PDP_TYPE_IPV4V6 == pdp_type_used)
            
            {
                return TEL_CONN_MGR_RET_OK;
            }
            break;
        }
        
        case TEL_CONN_MGR_APP_STATE_DEACTIVATING:
        case TEL_CONN_MGR_APP_STATE_ACTIVE:
        {
            if ((TEL_CONN_MGR_PDP_TYPE_NIDD == pdp_type_new && pdp_type_used == pdp_type_new) ||
                (TEL_CONN_MGR_PDP_TYPE_IP == pdp_type_used && (TEL_CONN_MGR_PDP_TYPE_IP == pdp_type_new ||
                 TEL_CONN_MGR_PDP_TYPE_IPV4V6 == pdp_type_new)) || (TEL_CONN_MGR_PDP_TYPE_IPV6 == pdp_type_used &&
                 (TEL_CONN_MGR_PDP_TYPE_IPV6 == pdp_type_new || TEL_CONN_MGR_PDP_TYPE_IPV4V6 == pdp_type_new)) ||
                 (TEL_CONN_MGR_PDP_TYPE_IPV4V6 == pdp_type_used && (TEL_CONN_MGR_PDP_TYPE_IPV4V6 == activated_pdp_type ||
                 (TEL_CONN_MGR_PDP_TYPE_IP == activated_pdp_type && (TEL_CONN_MGR_PDP_TYPE_IP == pdp_type_new ||
                 TEL_CONN_MGR_PDP_TYPE_IPV4V6 == pdp_type_new)) || (TEL_CONN_MGR_PDP_TYPE_IPV6 == activated_pdp_type &&
                 (TEL_CONN_MGR_PDP_TYPE_IPV6 == pdp_type_new || TEL_CONN_MGR_PDP_TYPE_IPV4V6 == pdp_type_new)) ||
                  TEL_CONN_MGR_PDP_TYPE_NONE == activated_pdp_type)))
            {
                return TEL_CONN_MGR_RET_OK;            
            }
            break;
        }

        default:
            break;
    }

    return TEL_CONN_MGR_RET_INCOMPATIBLE;
}


tel_conn_mgr_ret_enum tel_conn_mgr_send_act_req(int cid)
{
    tel_conn_mgr_activation_req_struct *act_req = NULL;

    TEL_CONN_MGR_LOG_INFO("%s, cid:%d", __FUNCTION__, cid);

    if (!tel_conn_mgr_is_cid_valid(cid) || tel_conn_mgr_is_special_cid(cid))
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    act_req = tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_activation_req_struct));
    if (!act_req)
    {
        return TEL_CONN_MGR_RET_NOT_ENOUGH_MEMORY;
    }

    act_req->msg_id = MSG_ID_TEL_CONN_MGR_ACTIVATION_REQ;
    act_req->cid = cid;
    
    if (!tel_conn_mgr_send_msg_to_tcm_task((tel_conn_mgr_msg_struct *)act_req))
    {
        tel_conn_mgr_free(act_req);
        return TEL_CONN_MGR_RET_ERROR;
    }

    tel_conn_mgr_bearer_lock_sleep();
    return TEL_CONN_MGR_RET_OK;
}


tel_conn_mgr_ret_enum tel_conn_mgr_send_deact_req(int cid)
{
    tel_conn_mgr_deactivation_req_struct *deact_req = NULL;

    TEL_CONN_MGR_LOG_INFO("%d", cid);
    if (!tel_conn_mgr_is_cid_valid(cid) || tel_conn_mgr_is_special_cid(cid))
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    deact_req = tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_deactivation_req_struct));
    if (!deact_req)
    {
        return TEL_CONN_MGR_RET_NOT_ENOUGH_MEMORY;
    }

    deact_req->msg_id = MSG_ID_TEL_CONN_MGR_DEACTIVATION_REQ;
    deact_req->cid = cid;
    
    if (!tel_conn_mgr_send_msg_to_tcm_task((tel_conn_mgr_msg_struct *)deact_req))
    {
        tel_conn_mgr_free(deact_req);
        return TEL_CONN_MGR_RET_ERROR;
    }

    tel_conn_mgr_bearer_lock_sleep();
    return TEL_CONN_MGR_RET_OK;
}    

tel_conn_mgr_ret_enum tel_conn_mgr_app_update_app_act(int app_info_idx,
                                                tel_conn_mgr_act_enum target_act,
                                                tel_conn_mgr_act_enum new_act)
{
    tel_conn_mgr_thread_info_struct *thread_info_tmp = tel_conn_mgr_app_cntx ? tel_conn_mgr_app_cntx->thread_info_list : NULL;
    tel_conn_mgr_app_info_struct *app_info_tmp = NULL;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_NOT_FOUND;
    
    if (!tel_conn_mgr_app_cntx || !tel_conn_mgr_is_idx_valid(app_info_idx) ||
        tel_conn_mgr_is_special_idx(app_info_idx))
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }    

    while (thread_info_tmp)
    {
        app_info_tmp = thread_info_tmp->app_info[app_info_idx];

        while (app_info_tmp)
        {
            if (target_act == app_info_tmp->act)
            {
                app_info_tmp->act = new_act;
                ret = TEL_CONN_MGR_RET_OK;
            }

            app_info_tmp = app_info_tmp->next;
        }
        
        thread_info_tmp = thread_info_tmp->next;
    }

    return ret;
}

