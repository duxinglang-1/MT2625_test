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
#include "tel_conn_mgr_util.h"
#include "tel_conn_mgr_bearer_info.h"

#define TEL_CONN_MGR_SLEEP_MANAGER_NAME "tcm_sleep_manager"

tel_conn_mgr_mutex_hdl_t tel_conn_mgr_global_mutex = NULL;

#ifndef USE_SYSLOG
tel_conn_mgr_mutex_hdl_t tel_conn_mgr_log_mutex = NULL;
#endif


#if 0
/* The size of dst buffer should at least be len. If dst wants to be treated as a string, the size should be at least len + 1. */
tel_conn_mgr_bool tel_conn_mgr_str_conv_to_upper_case(char *dst, const char *src, unsigned int len)
{    
    if (!dst || !str || !len)
    {
        return TEL_CONN_MGR_FALSE;
    }
    
    while (len-- && *str)
    {
        if (*str >= 'a' && *str <= 'z')
        {
            *dst = *str - 32;
        }
        else
        {
            *dst = *str;
        }

        dst++;
        str++;
    }

    return TEL_CONN_MGR_TRUE;
}
#endif


tel_conn_mgr_bool tel_conn_mgr_is_special_cid(int cid)
{
#ifdef TEL_CONN_MGR_ENABLE_INACTIVATE
    return TEL_CONN_MGR_SPECIAL_CID == cid;
#else
    return TEL_CONN_MGR_FALSE;
#endif
}


tel_conn_mgr_bool tel_conn_mgr_is_cid_valid(int cid)
{
    if (TEL_CONN_MGR_MIN_CID <= cid && TEL_CONN_MGR_MAX_CID >= cid)
    {
        return TEL_CONN_MGR_TRUE;
    }

    return tel_conn_mgr_is_special_cid(cid);
}


tel_conn_mgr_bool tel_conn_mgr_is_special_idx(unsigned int idx)
{
#ifdef TEL_CONN_MGR_ENABLE_INACTIVATE
    return TEL_CONN_MGR_SPECIAL_IDX == idx;
#else
    return TEL_CONN_MGR_FALSE;
#endif
}


tel_conn_mgr_bool tel_conn_mgr_is_idx_valid(unsigned int idx)
{
    if (0 <= idx && TEL_CONN_MGR_BEARER_TYPE_MAX_NUM > idx)
    {
        return TEL_CONN_MGR_TRUE;
    }

    return tel_conn_mgr_is_special_idx(idx);
}


/* Compare the first len bytes of two strings without considering the letter case.
  * Return: TEL_CONN_MGR_TRUE    equal
  *            TEL_CONN_MGR_FALSE   not equal
  */
tel_conn_mgr_bool tel_conn_mgr_str_case_equal(const char *s1, const char *s2, unsigned int len)
{

    #ifdef MTK_CREATE_DEFAULT_APN
    tel_conn_mgr_default_pdn_struct default_pdn;
    if (tel_conn_mgr_default_get_apn_flag() == 1) {//default have get, when user send"", should return ok.
       // if (strcmp(s2,"") == 0 || strcmp(s1,"") == 0) {
       default_pdn = tel_conn_mgr_get_default_pdn_content();
       
       TEL_CONN_MGR_LOG_INFO("defautl:%s,%s,%s!!!!\n", s1,s2,default_pdn.apn);
        if (strcmp(s2,"") == 0) {
            
            TEL_CONN_MGR_LOG_INFO("resend!!!!\n");
            return TEL_CONN_MGR_TRUE;
        } else if (strcmp(s2, default_pdn.apn) == 0) {
       
            TEL_CONN_MGR_LOG_INFO("resend2!!!!\n");
            return TEL_CONN_MGR_TRUE;
        }

    }
    #endif
    if (!s1 || !s2 || !len)
    {
        return (s1 == s2 || !len) ? TEL_CONN_MGR_TRUE : TEL_CONN_MGR_FALSE;
    }
    if (strcmp(s1,s2) == 0) {
        
        TEL_CONN_MGR_LOG_INFO("=====\r\n");
        return TEL_CONN_MGR_TRUE;
    }
    
    do
    {
        if (!(*s1 == *s2 || (*s1 >= 'A' && *s1 <= 'Z' && (*s1 + 32) == *s2) || 
            (*s1 >= 'a' && *s1 <= 'z' && (*s1 - 32) == *s2)))
        {
        
             TEL_CONN_MGR_LOG_INFO("%d\r\n",len);
            break;
        }
        
        s1++;
        s2++;
    } while (--len && *s1 && *s2);

    return (!len || (!*s1 && !*s2)) ? TEL_CONN_MGR_TRUE : TEL_CONN_MGR_FALSE;
}


tel_conn_mgr_ret_enum tel_conn_mgr_act_rsp_init(tel_conn_mgr_bearer_info_struct *bearer_info,
                                                       tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep,
                                                       tel_conn_mgr_bool result,
                                                       tel_conn_mgr_err_cause_enum cause,
                                                       tel_conn_mgr_activation_rsp_struct **act_rsp)
{
    tel_conn_mgr_activation_rsp_struct *act_rsp_tmp = NULL;
    
    if (!act_rsp || (*act_rsp) || !bearer_info || !ds_keep)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    act_rsp_tmp = tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_activation_rsp_struct));

    if (!act_rsp_tmp)
    {
        TEL_CONN_MGR_LOG_ERR("Failed to create act_rsp buffer.");
        return TEL_CONN_MGR_RET_NOT_ENOUGH_MEMORY;
    }
    else
    {
        act_rsp_tmp->msg_id = MSG_ID_TEL_CONN_MGR_ACTIVATION_RSP;
        act_rsp_tmp->bearer_type = bearer_info->bearer_type;
        act_rsp_tmp->sim_id = bearer_info->sim_id;
        if (result)
        {
            act_rsp_tmp->pdp_type = ds_keep->activated_pdp_type;    /* Actual pdp_type that is activated. */
        }
        strncpy(act_rsp_tmp->apn, ds_keep->apn, TEL_CONN_MGR_APN_MAX_LEN);
        act_rsp_tmp->result = result;
        act_rsp_tmp->cause = cause;        
    }

    *act_rsp = act_rsp_tmp;
    return TEL_CONN_MGR_RET_OK;
}


void tel_conn_mgr_act_rsp_deinit(tel_conn_mgr_activation_rsp_struct *act_rsp)
{
    if (act_rsp)
    {
        tel_conn_mgr_free(act_rsp);
    }
}


tel_conn_mgr_ret_enum tel_conn_mgr_deact_rsp_init(tel_conn_mgr_bearer_info_struct *bearer_info,
                                                          tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep,
                                                          tel_conn_mgr_bool result,
                                                          tel_conn_mgr_err_cause_enum cause,
                                                          tel_conn_mgr_deactivation_rsp_struct **deact_rsp)
{
    tel_conn_mgr_deactivation_rsp_struct *deact_rsp_tmp = NULL;
        
    if (!deact_rsp || (*deact_rsp) || !bearer_info || !ds_keep)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    deact_rsp_tmp = tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_deactivation_rsp_struct));

    if (!deact_rsp_tmp)
    {
        TEL_CONN_MGR_LOG_ERR("Failed to create deact_rsp buffer.");
        return TEL_CONN_MGR_RET_NOT_ENOUGH_MEMORY;
    }
    else
    {
        deact_rsp_tmp->msg_id = MSG_ID_TEL_CONN_MGR_DEACTIVATION_RSP;
        deact_rsp_tmp->bearer_type = bearer_info->bearer_type;
        deact_rsp_tmp->sim_id = bearer_info->sim_id;
        deact_rsp_tmp->pdp_type = ds_keep->activated_pdp_type;    /* Actual pdp_type that is activated. */
        strncpy(deact_rsp_tmp->apn, ds_keep->apn, TEL_CONN_MGR_APN_MAX_LEN);
        deact_rsp_tmp->result = result;
        deact_rsp_tmp->cause = cause;        
    }

    *deact_rsp = deact_rsp_tmp;
    return TEL_CONN_MGR_RET_OK;    
}


void tel_conn_mgr_deact_rsp_deinit(tel_conn_mgr_deactivation_rsp_struct *deact_rsp)
{
    if (deact_rsp)
    {
        tel_conn_mgr_free(deact_rsp);
    }
}


tel_conn_mgr_ret_enum tel_conn_mgr_deact_ind_init(tel_conn_mgr_bearer_info_struct *bearer_info,
                                                         tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep,
                                                         tel_conn_mgr_err_cause_enum cause,
                                                         tel_conn_mgr_deactivation_ind_struct **deact_ind)
{
    tel_conn_mgr_deactivation_ind_struct *deact_ind_tmp = NULL;
        
    if (!deact_ind || (*deact_ind) || !bearer_info || !ds_keep)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    deact_ind_tmp = tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_deactivation_ind_struct));

    if (!deact_ind_tmp)
    {
        TEL_CONN_MGR_LOG_ERR("Failed to create deact_ind buffer.");
        return TEL_CONN_MGR_RET_NOT_ENOUGH_MEMORY;
    }
    else
    {
        deact_ind_tmp->msg_id = MSG_ID_TEL_CONN_MGR_DEACTIVATION_IND;
        deact_ind_tmp->bearer_type = bearer_info->bearer_type;
        deact_ind_tmp->sim_id = bearer_info->sim_id;
        deact_ind_tmp->pdp_type = ds_keep->activated_pdp_type;    /* Actual pdp_type that is activated. */
        strncpy(deact_ind_tmp->apn, ds_keep->apn, TEL_CONN_MGR_APN_MAX_LEN);
        deact_ind_tmp->cause = cause;        
    }

    *deact_ind = deact_ind_tmp;
    return TEL_CONN_MGR_RET_OK;    
}


void tel_conn_mgr_deact_ind_deinit(tel_conn_mgr_deactivation_ind_struct *deact_ind)
{
    if (deact_ind)
    {
        tel_conn_mgr_free(deact_ind);
    }
}


/* Send msg to tel conn mgr task. */
tel_conn_mgr_bool tel_conn_mgr_send_msg_to_tcm_task(tel_conn_mgr_msg_struct *msg)
{
    return tel_conn_mgr_send_msg(tel_conn_mgr_task_queue_get(), msg);
}


tel_conn_mgr_ret_enum tel_conn_mgr_send_act_result_msg_to_app(tel_conn_mgr_bearer_info_struct *bearer_info,
                                                                           tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep,
                                                                           tel_conn_mgr_bool result,
                                                                           tel_conn_mgr_err_cause_enum cause,
                                                                           unsigned int app_ids[TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM],
                                                                           tel_conn_mgr_queue_hdl_t queue_hdl)
{
    tel_conn_mgr_activation_rsp_struct *act_rsp = NULL;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;

    TEL_CONN_MGR_LOG_INFO("bearer_info:%x, app_ids[0]:%d, result:%d, cause:%d, queue_hdl:%x", 
                          (unsigned int)bearer_info, app_ids[0], result, cause, (unsigned int)queue_hdl);
    
    if (!bearer_info || !ds_keep || !app_ids[0])
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    ret = tel_conn_mgr_act_rsp_init(bearer_info,
                                    ds_keep,
                                    result,
                                    cause,
                                    &act_rsp);

    if (act_rsp)
    {
        memcpy(act_rsp->app_id, app_ids, TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM * sizeof(unsigned int));
        /*TEL_CONN_MGR_LOG_INFO("app_id[0]:%d, app_id[1]:%d, app_id[2]:%d, app_id[3]:%d, app_id[4]:%d",
                              app_ids[0], app_ids[1], app_ids[2], app_ids[3], app_ids[4]);*/
        if (!tel_conn_mgr_send_msg(queue_hdl, (void *)act_rsp))
        {
            ret = TEL_CONN_MGR_RET_ERROR;
            tel_conn_mgr_act_rsp_deinit(act_rsp);
        }
    }

    return ret;
}


tel_conn_mgr_ret_enum tel_conn_mgr_send_deact_result_msg_to_app(tel_conn_mgr_bearer_info_struct *bearer_info,
                                                                           tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep,
                                                                           tel_conn_mgr_bool result,
                                                                           tel_conn_mgr_err_cause_enum cause,
                                                                           unsigned int app_id,
                                                                           tel_conn_mgr_queue_hdl_t queue_hdl)
{
    tel_conn_mgr_deactivation_rsp_struct *deact_rsp = NULL;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;

    TEL_CONN_MGR_LOG_INFO("bearer_info:%x, app_id:%d, result:%d, cause:%d, queue_hdl:%x", 
                          (unsigned int)bearer_info, app_id, result, cause, (unsigned int)queue_hdl);
    
    if (!bearer_info || !ds_keep || !app_id)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    ret = tel_conn_mgr_deact_rsp_init(bearer_info,
                                      ds_keep,
                                      result,
                                      cause,
                                      &deact_rsp);

    if (deact_rsp)
    {
        deact_rsp->app_id = app_id;
        if (!tel_conn_mgr_send_msg(queue_hdl, (void *)deact_rsp))
        {
            ret = TEL_CONN_MGR_RET_ERROR;
            tel_conn_mgr_deact_rsp_deinit(deact_rsp);
        }
    }

    return ret;
}


tel_conn_mgr_ret_enum tel_conn_mgr_send_deact_ind_msg_to_app(tel_conn_mgr_bearer_info_struct *bearer_info,
                                                                           tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep,
                                                                           tel_conn_mgr_err_cause_enum cause,
                                                                           unsigned int app_ids[TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM],
                                                                           tel_conn_mgr_queue_hdl_t queue_hdl)
{
    tel_conn_mgr_deactivation_ind_struct *deact_ind = NULL;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    
    TEL_CONN_MGR_LOG_INFO("bearer_info:%x, app_ids[0]:%d, cause:%d, queue_hdl:%x", 
                          (unsigned int)bearer_info, app_ids[0], cause, (unsigned int)queue_hdl);
    
    if (!bearer_info || !app_ids[0] || !ds_keep)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    ret = tel_conn_mgr_deact_ind_init(bearer_info,
                                      ds_keep,
                                      cause,
                                      &deact_ind);

    if (deact_ind)
    {
        memcpy(deact_ind->app_id, app_ids, TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM);
        if (!tel_conn_mgr_send_msg(queue_hdl, (void *)deact_ind))
        {
            ret = TEL_CONN_MGR_RET_ERROR;
            tel_conn_mgr_deact_ind_deinit(deact_ind);
        }
    }

    return ret;
}


tel_conn_mgr_ret_enum tel_conn_mgr_list_insert(tel_conn_mgr_template_list_struct **list, tel_conn_mgr_template_list_struct *list_node)
{
    tel_conn_mgr_template_list_struct *list_node_tmp = NULL;

    //TEL_CONN_MGR_LOG_INFO("%s", __FUNCTION__);
        
    if (!list || !list_node || list_node->next)
    {
        TEL_CONN_MGR_LOG_INFO("%s Invalid param", __FUNCTION__);
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }    
    
    if (!(*list))
    {
        *list = list_node;
    }
    else
    {
        list_node_tmp = *list;
        while (list_node_tmp->next)
        {
            list_node_tmp = list_node_tmp->next;
        }

        list_node_tmp->next = list_node;                
    }

    //TEL_CONN_MGR_LOG_INFO("%s return", __FUNCTION__);

    return TEL_CONN_MGR_RET_OK;    
}


/* The node will not be freed in this API. */
tel_conn_mgr_ret_enum tel_conn_mgr_list_remove(tel_conn_mgr_template_list_struct **list, tel_conn_mgr_template_list_struct *list_node)
{
    tel_conn_mgr_template_list_struct *list_node_tmp = NULL;

    //TEL_CONN_MGR_LOG_INFO("%s", __FUNCTION__);

    if (!list || !(*list) || !list_node)
    {
        TEL_CONN_MGR_LOG_INFO("%s Invalid Param", __FUNCTION__);
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    if (*list == list_node)
    {
        *list = (*list)->next;
    }
    else
    {
        list_node_tmp = *list;
        while (list_node_tmp && list_node_tmp->next != list_node)
        {
            list_node_tmp = list_node_tmp->next;
        }

        if (list_node_tmp)
        {
            list_node_tmp->next = list_node->next;
        }
        else
        {
            TEL_CONN_MGR_LOG_INFO("%s Not Found", __FUNCTION__);
            return TEL_CONN_MGR_RET_NOT_FOUND;
        }
    }

    list_node->next = NULL;

    //TEL_CONN_MGR_LOG_INFO("%s return", __FUNCTION__);
    
    return TEL_CONN_MGR_RET_OK;
}


tel_conn_mgr_ret_enum tel_conn_mgr_global_mutex_create(void)
{
    if (tel_conn_mgr_global_mutex)
    {
        TEL_CONN_MGR_LOG_WARN("Global mutex has been created.");
        return TEL_CONN_MGR_RET_OK;
    }
    
    tel_conn_mgr_global_mutex = tel_conn_mgr_mutex_create();
    return tel_conn_mgr_global_mutex ? TEL_CONN_MGR_RET_OK : TEL_CONN_MGR_RET_ERROR;
}


void tel_conn_mgr_global_mutex_free(void)
{
    if (tel_conn_mgr_global_mutex)
    {
        tel_conn_mgr_mutex_free(tel_conn_mgr_global_mutex);
        tel_conn_mgr_global_mutex = NULL;
    }
}


void tel_conn_mgr_global_mutex_lock(const char *caller)
{
    TEL_CONN_MGR_LOG_INFO("%s, lock mutex", caller);
    tel_conn_mgr_mutex_lock(tel_conn_mgr_global_mutex);
    TEL_CONN_MGR_LOG_INFO("%s, lock mutex done", caller);
}


void tel_conn_mgr_global_mutex_unlock(const char *caller)
{
    TEL_CONN_MGR_LOG_INFO("%s, unlock mutex", caller);
    tel_conn_mgr_mutex_unlock(tel_conn_mgr_global_mutex);
}


tel_conn_mgr_ret_enum tel_conn_mgr_util_init(void)
{
    TEL_CONN_MGR_LOG_INFO("%s", __FUNCTION__);

    #ifndef USE_SYSLOG
    if (!tel_conn_mgr_log_mutex)
    {
        tel_conn_mgr_log_mutex = tel_conn_mgr_mutex_create();
        if (!tel_conn_mgr_log_mutex)
        {
            return TEL_CONN_MGR_RET_ERROR;
        }
    }
    #endif
    
    return tel_conn_mgr_global_mutex_create();
}


void tel_conn_mgr_util_deinit(void)
{
    TEL_CONN_MGR_LOG_INFO("%s", __FUNCTION__);
    #ifndef USE_SYSLOG
    if (tel_conn_mgr_log_mutex)
    {
        tel_conn_mgr_mutex_free(tel_conn_mgr_log_mutex);
    }
    #endif    
    tel_conn_mgr_global_mutex_free();

    /* There's no API for unset sleep manager. */
}


void tel_conn_mgr_general_lock_sleep(unsigned char sleep_manager_handle,
                                               int *sleep_lock_count)
{
#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
    TEL_CONN_MGR_LOG_INFO("lock sleep, sleep_manager_handle:%d, lock_count:%d",
                          sleep_manager_handle, sleep_lock_count ? *sleep_lock_count : -1);

    if (sleep_lock_count)
    {
        if (!(*sleep_lock_count))        
        {
            if (TEL_CONN_MGR_SLEEP_MANAGER_OK != tel_conn_mgr_lock_sleep(sleep_manager_handle))
            {
                TEL_CONN_MGR_LOG_WARN("lock sleep error.");
                return;
            }
            TEL_CONN_MGR_LOG_WARN("lock sleep ok.");
        }

        (*sleep_lock_count)++;
    }
#endif
}


void tel_conn_mgr_general_unlock_sleep(unsigned char sleep_manager_handle,
                                                 int *sleep_lock_count)
{
#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
    TEL_CONN_MGR_LOG_INFO("unlock sleep, sleep_manager_handle:%d, lock_count:%d",
                          sleep_manager_handle, sleep_lock_count ? *sleep_lock_count : -1);
    if (sleep_lock_count)
    {
        if (0 < *sleep_lock_count)
        {
            (*sleep_lock_count)--;
        
            if (!(*sleep_lock_count))        
            {
                if (TEL_CONN_MGR_SLEEP_MANAGER_OK != tel_conn_mgr_unlock_sleep(sleep_manager_handle))
                {
                    TEL_CONN_MGR_LOG_WARN("unlock sleep error.");
                    return;
                }
                TEL_CONN_MGR_LOG_WARN("unlock sleep ok.");
            }
        }
    }
#endif
}


#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
tel_conn_mgr_reg_info_struct *tel_conn_mgr_find_reg_info(tel_conn_mgr_register_handle_t reg_hdl)
{
    int i = 0;
    tel_conn_mgr_app_cntx_struct *app_cntx = tel_conn_mgr_app_get_cntx();
    
    if (!reg_hdl || !app_cntx)
    {
        TEL_CONN_MGR_LOG_ERR("%s, invalid param.", __FUNCTION__);
        return NULL;
    }

    for (i = 0; i < TEL_CONN_MGR_REG_INFO_MAX_NUM; i++)
    {
        if (reg_hdl == app_cntx->reg_info[i].reg_hdl)
        {
            TEL_CONN_MGR_LOG_ERR("%s, found.", __FUNCTION__);
            return &app_cntx->reg_info[i];
        }
    }

    TEL_CONN_MGR_LOG_ERR("%s, not found.", __FUNCTION__);
    return NULL;
}
#endif


tel_conn_mgr_ret_enum tel_conn_mgr_notify_app(tel_conn_mgr_app_msg_type_enum app_msg_type,
                                                     int cid,
                                                     tel_conn_mgr_bool result,
                                                     tel_conn_mgr_err_cause_enum cause,
                                                     tel_conn_mgr_thread_info_struct *specified_thread_info,
                                                     tel_conn_mgr_app_info_struct *specified_app_info)
{
    tel_conn_mgr_thread_info_struct *thread_info_tmp = NULL, *thread_info_next = NULL;
    tel_conn_mgr_app_info_struct *app_info_tmp = NULL, *app_info_next = NULL;
    tel_conn_mgr_app_cntx_struct *app_cntx = tel_conn_mgr_app_get_cntx();
    unsigned char bearer_info_idx = tel_conn_mgr_convt_cid_to_bearer_info_idx(cid);
    tel_conn_mgr_bearer_info_struct *bearer_info = tel_conn_mgr_bearer_info_find_by_cid(cid);
    tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_cid(cid);
    tel_conn_mgr_queue_hdl_t queue_hdl = NULL;
    unsigned int app_ids[TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM] = {0};
    int last_idx = 0;
    tel_conn_mgr_act_enum act = TEL_CONN_MGR_ACT_ACT;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
    tel_conn_mgr_register_handle_t reg_hdl = NULL;
    tel_conn_mgr_reg_info_struct *reg_info = NULL;
#endif

    TEL_CONN_MGR_LOG_INFO("app_msg_type:%d, cid:%d, result:%d, cause:%d",
                          app_msg_type, cid, result, cause);
    
    if ((specified_thread_info && !specified_app_info) ||
        (!specified_thread_info && specified_app_info) ||
        !tel_conn_mgr_is_cid_valid(cid) ||
        tel_conn_mgr_is_special_cid(cid) ||
        !tel_conn_mgr_is_idx_valid(bearer_info_idx) ||
        tel_conn_mgr_is_special_idx(bearer_info_idx) ||
        !bearer_info ||
        !ds_keep)
    {
        ret = TEL_CONN_MGR_RET_INVALID_PARAM;
        TEL_CONN_MGR_LOG_INFO("ret:%d", ret);
        return ret;
    }

    if (TEL_CONN_MGR_APP_MSG_TYPE_ACTIVE_DEACTIVATION == app_msg_type ||
        TEL_CONN_MGR_APP_MSG_TYPE_PASSIVE_DEACTIVATION == app_msg_type ||
        (TEL_CONN_MGR_APP_MSG_TYPE_ACTIVATION == app_msg_type &&
        !result && TEL_CONN_MGR_ERR_CAUSE_DEACT_REQ == cause))
    {
        /* When activation is terminated by deact req, the act for app has been updated to DEACT. */
        act = TEL_CONN_MGR_ACT_DEACT;
    }

    TEL_CONN_MGR_LOG_INFO("target act:%d (phase-out)", act);
    
    if (!specified_app_info)
    {
        thread_info_tmp = app_cntx ? app_cntx->thread_info_list : NULL;
        //TEL_CONN_MGR_LOG_INFO("thread_info_tmp:%x", thread_info_tmp);
        while (thread_info_tmp)
        {
            /* tel_conn_mgr_free_app_info() may set thread_info_tmp to NULL. */
            thread_info_next = thread_info_tmp->next;
            memset(app_ids, 0, sizeof(app_ids));
            queue_hdl = thread_info_tmp->queue_hdl;
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
            reg_hdl = thread_info_tmp->reg_hdl;
#endif
            app_info_tmp = thread_info_tmp->app_info[bearer_info_idx];
            last_idx = 0;
            //TEL_CONN_MGR_LOG_INFO("app_info_tmp:%x, idx:%d", app_info_tmp, bearer_info_idx);
            while (app_info_tmp)
            {
                /* app_info_tmp may be freed by tel_conn_mgr_free_app_info() */
                app_info_next = app_info_tmp->next;
                TEL_CONN_MGR_LOG_INFO("app_id:%d, act:%d", app_info_tmp->app_id, app_info_tmp->act);
                //if (act == app_info_tmp->act) 
                if (TEL_CONN_MGR_ACT_ACT == app_info_tmp->act ||
                    TEL_CONN_MGR_ACT_DEACT == app_info_tmp->act)
                {
                    if (TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM > last_idx)
                    {
                        app_ids[last_idx++] = app_info_tmp->app_id;
                    }
                    else
                    {
                        TEL_CONN_MGR_LOG_WARN("The num of app info exceeds max num:%d", TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM);
                    }

                    if (!((TEL_CONN_MGR_APP_MSG_TYPE_ACTIVATION == app_msg_type && result) ||
                        TEL_CONN_MGR_ERR_CAUSE_DEACT_REQ == cause))
                    {
                        ret = tel_conn_mgr_free_app_info(thread_info_tmp,
                                                         app_info_tmp,
                                                         bearer_info_idx);

                        if (TEL_CONN_MGR_RET_OK != ret)
                        {
                            app_info_tmp->act = TEL_CONN_MGR_ACT_NONE;
                            TEL_CONN_MGR_LOG_ERR("Failed to free app info. ret:%d", ret);
                        }
                    }
                }                
                
                app_info_tmp = app_info_next;
            }

            /*TEL_CONN_MGR_LOG_INFO("app_id[0]:%d, app_id[1]:%d, app_id[2]:%d, app_id[3]:%d, app_id[4]:%d",
                              app_ids[0], app_ids[1], app_ids[2], app_ids[3], app_ids[4]);*/

            TEL_CONN_MGR_LOG_INFO("app_ids[0]:%d, total_app_ids:%d, queue_hdl:%x", app_ids[0], last_idx, (unsigned int)queue_hdl);
            if (app_ids[0])
            {
                switch (app_msg_type)
                {
                    case TEL_CONN_MGR_APP_MSG_TYPE_ACTIVATION:
                    {
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
                        TEL_CONN_MGR_LOG_INFO("Try to activation feedback using callback. reg_hdl:%x", reg_hdl);
                        if (reg_hdl)
                        {
                            reg_info = tel_conn_mgr_find_reg_info(reg_hdl);
                            if (reg_info && reg_info->callback)
                            {
                                reg_info->callback(app_ids[0],
                                                   TEL_CONN_MGR_INFO_TYPE_ACTIVATION,
                                                   result,
                                                   cause,
                                                   ds_keep->activated_pdp_type);
                            }
                        }
#endif
                        if (queue_hdl)
                        {
                            ret = tel_conn_mgr_send_act_result_msg_to_app(bearer_info,
                                                                          ds_keep,
                                                                          result,
                                                                          cause,
                                                                          app_ids,
                                                                          queue_hdl);
                        }
                        break;
                    }

                    case TEL_CONN_MGR_APP_MSG_TYPE_ACTIVE_DEACTIVATION:
                    {                        
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
                        TEL_CONN_MGR_LOG_INFO("Try to active deactivation feedback using callback. reg_hdl:%x", reg_hdl);
                        if (reg_hdl)
                        {
                            reg_info = tel_conn_mgr_find_reg_info(reg_hdl);
                            if (reg_info && reg_info->callback)
                            {
                                reg_info->callback(app_ids[0],
                                                   TEL_CONN_MGR_INFO_TYPE_ACTIVE_DEACTIVATION,
                                                   result,
                                                   cause,
                                                   ds_keep->activated_pdp_type);
                            }
                        }
#endif
                        if (queue_hdl)
                        {
                            ret = tel_conn_mgr_send_deact_result_msg_to_app(bearer_info,
                                                                            ds_keep,
                                                                            result,
                                                                            cause,
                                                                            app_ids[0],
                                                                            queue_hdl);
                        }
                        break;
                    }

                    case TEL_CONN_MGR_APP_MSG_TYPE_PASSIVE_DEACTIVATION:
                    {
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
                        TEL_CONN_MGR_LOG_INFO("Try to passive deactivation feedback using callback. reg_hdl:%x", reg_hdl);
                        if (reg_hdl)
                        {
                            reg_info = tel_conn_mgr_find_reg_info(reg_hdl);
                            if (reg_info && reg_info->callback)
                            {
                                reg_info->callback(app_ids[0],
                                                   TEL_CONN_MGR_INFO_TYPE_PASSIVE_DEACTIVATION,
                                                   result,
                                                   cause,
                                                   ds_keep->activated_pdp_type);
                            }
                        }
#endif

                        if (queue_hdl)
                        {
                            ret = tel_conn_mgr_send_deact_ind_msg_to_app(bearer_info,
                                                                         ds_keep,
                                                                         cause,
                                                                         app_ids,
                                                                         queue_hdl);
                        }
                        break;
                    }

                    default:
                        break;
                }
            }

            thread_info_tmp = thread_info_next;
        }
    }
    else
    {
        memset(app_ids, 0, sizeof(app_ids));
        app_ids[0] = specified_app_info->app_id;
        queue_hdl = specified_thread_info->queue_hdl;
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
        reg_hdl =  specified_thread_info->reg_hdl;
#endif
        if (!((TEL_CONN_MGR_APP_MSG_TYPE_ACTIVATION == app_msg_type && result) ||
            TEL_CONN_MGR_ERR_CAUSE_DEACT_REQ == cause))
        {
            ret = tel_conn_mgr_free_app_info(specified_thread_info,
                                             specified_app_info,
                                             bearer_info_idx);

            if (TEL_CONN_MGR_RET_OK != ret)
            {
                specified_app_info->act = TEL_CONN_MGR_ACT_NONE;
                TEL_CONN_MGR_LOG_ERR("Failed to free app info. ret:%d", ret);
            }
        }        
        
        switch (app_msg_type)
        {
            case TEL_CONN_MGR_APP_MSG_TYPE_ACTIVATION:
            {
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
                TEL_CONN_MGR_LOG_INFO("Try to activation feedback using callback. reg_hdl:%x", reg_hdl);
                if (reg_hdl)
                {
                    reg_info = tel_conn_mgr_find_reg_info(reg_hdl);
                    if (reg_info && reg_info->callback)
                    {
                        reg_info->callback(app_ids[0],
                                           TEL_CONN_MGR_INFO_TYPE_ACTIVATION,
                                           result,
                                           cause,
                                           ds_keep->activated_pdp_type);
                    }
                }
#endif
                if (queue_hdl)
                {
                    ret = tel_conn_mgr_send_act_result_msg_to_app(bearer_info,
                                                                  ds_keep,
                                                                  result,
                                                                  cause,
                                                                  app_ids,
                                                                  queue_hdl);
                }
                break;
            }

            case TEL_CONN_MGR_APP_MSG_TYPE_ACTIVE_DEACTIVATION:
            {
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
                TEL_CONN_MGR_LOG_INFO("Try to active deactivation feedback using callback. reg_hdl:%x", reg_hdl);
                if (reg_hdl)
                {
                    reg_info = tel_conn_mgr_find_reg_info(reg_hdl);
                    if (reg_info && reg_info->callback)
                    {
                        reg_info->callback(app_ids[0],
                                           TEL_CONN_MGR_INFO_TYPE_ACTIVE_DEACTIVATION,
                                           result,
                                           cause,
                                           ds_keep->activated_pdp_type);
                    }
                }
#endif
                if (queue_hdl)
                {
                    ret = tel_conn_mgr_send_deact_result_msg_to_app(bearer_info,
                                                                    ds_keep,
                                                                    result,
                                                                    cause,
                                                                    app_ids[0],
                                                                    queue_hdl);
                }
                break;
            }

            case TEL_CONN_MGR_APP_MSG_TYPE_PASSIVE_DEACTIVATION:
            {
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
                TEL_CONN_MGR_LOG_INFO("Try to passive deactivation feedback using callback. reg_hdl:%x", reg_hdl);

                if (reg_hdl)
                {
                    reg_info = tel_conn_mgr_find_reg_info(reg_hdl);
                    if (reg_info && reg_info->callback)
                    {
                        reg_info->callback(app_ids[0],
                                           TEL_CONN_MGR_INFO_TYPE_PASSIVE_DEACTIVATION,
                                           result,
                                           cause,
                                           ds_keep->activated_pdp_type);
                    }
                }
#endif
                if (queue_hdl)
                {
                    ret = tel_conn_mgr_send_deact_ind_msg_to_app(bearer_info,
                                                                 ds_keep,                        
                                                                 cause,
                                                                 app_ids,
                                                                 queue_hdl);
                }
                break;
            }

            default:
                break;
        }
    }

    return ret;
}


void tel_conn_mgr_update_app_state_for_activation(tel_conn_mgr_bool result,                                                              
                                                              int cid,
                                                              tel_conn_mgr_bool need_deact)
{
    tel_conn_mgr_app_state_enum app_state = TEL_CONN_MGR_APP_STATE_MAX;
    unsigned int idx = tel_conn_mgr_convt_cid_to_bearer_info_idx(cid);
    
    if (!tel_conn_mgr_is_idx_valid(idx) ||
         tel_conn_mgr_is_special_idx(idx))
    {
        return;
    }

    app_state = tel_conn_mgr_app_get_app_state(idx);
    switch (app_state)
    {
        case TEL_CONN_MGR_APP_STATE_ACTIVATING:
        {
            if (result)
            {
                tel_conn_mgr_app_update_app_state(idx, TEL_CONN_MGR_APP_STATE_ACTIVE);
            }
            else
            {
                if (need_deact)
                {
                    tel_conn_mgr_app_update_app_state(idx, TEL_CONN_MGR_APP_STATE_DEACTIVATING);
                    tel_conn_mgr_send_deact_req(cid);
                }
                else
                {
                    tel_conn_mgr_app_update_app_state(idx, TEL_CONN_MGR_APP_STATE_INACTIVE);
                }
            }

            break;
        }

        case TEL_CONN_MGR_APP_STATE_DEACTIVATING:
        case TEL_CONN_MGR_APP_STATE_REACTIVE:
        {
            /* Nothing to do for DEACT REQ in the queue. */
            TEL_CONN_MGR_LOG_INFO("ACT RESULT with app state:%d", app_state);
            break;
        }

        default:
        {
            /* Wrong app state. */
            TEL_CONN_MGR_LOG_WARN("ACT RESULT with app state:%d", app_state);
            break;
        }
    }

}


void tel_conn_mgr_update_app_state_for_active_deactivation(int cid)
{
    tel_conn_mgr_app_state_enum app_state = TEL_CONN_MGR_APP_STATE_MAX;
    unsigned int idx = tel_conn_mgr_convt_cid_to_bearer_info_idx(cid);

    if (!tel_conn_mgr_is_idx_valid(idx) ||
         tel_conn_mgr_is_special_idx(idx))
    {
        return;
    }
    
    app_state = tel_conn_mgr_app_get_app_state(idx);
    switch (app_state)
    {
        case TEL_CONN_MGR_APP_STATE_DEACTIVATING:
        {
            /* No matter the result is true or false, update the app state to INACTIVE. */
            tel_conn_mgr_app_update_app_state(idx, TEL_CONN_MGR_APP_STATE_INACTIVE);

            break;
        }

        case TEL_CONN_MGR_APP_STATE_ACTIVATING:
        case TEL_CONN_MGR_APP_STATE_REACTIVE:
        {
            /* Nothing to do for ACT REQ in the queue. */
            TEL_CONN_MGR_LOG_INFO("DEACT RESULT with app state:%d", app_state);
            break;
        }

        default:
        {
            /* Wrong app state. */
            TEL_CONN_MGR_LOG_ERR("%s, Wrong app state:%d", __FUNCTION__, app_state);
            break;
        }
    }
}


void tel_conn_mgr_update_app_state_for_passive_deactivation(int cid)
{
    tel_conn_mgr_app_state_enum app_state = TEL_CONN_MGR_APP_STATE_MAX;
    unsigned int idx = tel_conn_mgr_convt_cid_to_bearer_info_idx(cid);

    if (!tel_conn_mgr_is_idx_valid(idx) ||
         tel_conn_mgr_is_special_idx(idx))
    {
        return;
    }
    
    app_state = tel_conn_mgr_app_get_app_state(idx);
    switch (app_state)
    {
        case TEL_CONN_MGR_APP_STATE_ACTIVE:
        {
            tel_conn_mgr_app_update_app_state(idx, TEL_CONN_MGR_APP_STATE_INACTIVE);

            break;
        }

        default:
        {
            /* Wrong app state. */
            TEL_CONN_MGR_LOG_ERR("%s, Wrong app state:%d", __FUNCTION__, app_state);
            break;
        }
    }
}


unsigned int tel_conn_mgr_convt_cid_to_bearer_info_idx(int cid)
{
    if (tel_conn_mgr_is_cid_valid(cid))
    {
        if (tel_conn_mgr_is_special_cid(cid))
        {
            return TEL_CONN_MGR_SPECIAL_IDX;
        }
        else
        {
            return cid - 1;
        }
    }

    return TEL_CONN_MGR_BEARER_TYPE_MAX_NUM;
}


int tel_conn_mgr_convt_bearer_info_idx_to_cid(unsigned int bearer_info_idx)
{
    if (tel_conn_mgr_is_idx_valid(bearer_info_idx))
    {
        return bearer_info_idx + TEL_CONN_MGR_MIN_CID;
    }
    else if (tel_conn_mgr_is_special_idx(bearer_info_idx))
    {
        return TEL_CONN_MGR_SPECIAL_CID;
    }

    return TEL_CONN_MGR_MAX_CID + 1;
}


tel_conn_mgr_ret_enum tel_conn_mgr_get_cid_by_app_id(int *cid, unsigned int app_id)
{
    tel_conn_mgr_thread_info_struct *thread_info = NULL;
    tel_conn_mgr_app_info_struct *app_info = NULL;
    unsigned int bearer_info_idx = 0;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;

    if (!cid)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    ret = tel_conn_mgr_get_app_info(app_id, &thread_info, &app_info, &bearer_info_idx);
    if (TEL_CONN_MGR_RET_OK == ret)
    {
        *cid = tel_conn_mgr_convt_bearer_info_idx_to_cid(bearer_info_idx);
        if (!tel_conn_mgr_is_cid_valid(*cid) || tel_conn_mgr_is_special_cid(*cid))
        {
            *cid = 0;
            ret = TEL_CONN_MGR_RET_ERROR;
        }
    }

    return ret;
}


#ifndef USE_SYSLOG
void tel_conn_mgr_log_mutex_lock(char *caller)
{
    tel_conn_mgr_mutex_lock(tel_conn_mgr_log_mutex);
}


void tel_conn_mgr_log_mutex_unlock(char *caller)
{
    tel_conn_mgr_mutex_unlock(tel_conn_mgr_log_mutex);
}
#endif

