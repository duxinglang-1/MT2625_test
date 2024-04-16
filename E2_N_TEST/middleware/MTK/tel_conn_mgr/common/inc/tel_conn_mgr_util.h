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

#ifndef __TEL_CONN_MGR_UTIL_H__
#define __TEL_CONN_MGR_UTIL_H__

#include "tel_conn_mgr_platform.h"
#include "tel_conn_mgr_common.h"
#include "tel_conn_mgr_app_mgr.h"
#ifdef MTK_CREATE_DEFAULT_APN
#include "tel_conn_mgr_default_pdn_link.h"
#endif


#define TEL_CONN_MGR_MIN_CID    (1)
#define TEL_CONN_MGR_MAX_CID    (TEL_CONN_MGR_MIN_CID + TEL_CONN_MGR_BEARER_TYPE_MAX_NUM - 1)
#define TEL_CONN_MGR_SPECIAL_CID    (0xEE)
#define TEL_CONN_MGR_SPECIAL_IDX    (TEL_CONN_MGR_BEARER_TYPE_MAX_NUM + 1)


typedef enum
{
    TEL_CONN_MGR_APP_MSG_TYPE_NONE,
    TEL_CONN_MGR_APP_MSG_TYPE_ACTIVATION,
    TEL_CONN_MGR_APP_MSG_TYPE_ACTIVE_DEACTIVATION,
    TEL_CONN_MGR_APP_MSG_TYPE_PASSIVE_DEACTIVATION,

    TEL_CONN_MGR_APP_MSG_TYPE_MAX
}tel_conn_mgr_app_msg_type_enum;


typedef struct tel_conn_mgr_template_list
{
    struct tel_conn_mgr_template_list *next;    
}tel_conn_mgr_template_list_struct;

#if 0
#define TEL_CONN_MGR_GLOBAL_MUTEX_LOCK (tel_conn_mgr_global_mutex_lock())
#define TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK (tel_conn_mgr_global_mutex_unlock())
#else
#define TEL_CONN_MGR_GLOBAL_MUTEX_LOCK (tel_conn_mgr_global_mutex_lock((__FUNCTION__)))
#define TEL_CONN_MGR_GLOBAL_MUTEX_UNLOCK (tel_conn_mgr_global_mutex_unlock((__FUNCTION__)))
#endif

#ifndef USE_SYSLOG
#define TEL_CONN_MGR_LOG_LOCK (tel_conn_mgr_mutex_lock(tel_conn_mgr_log_mutex))
#define TEL_CONN_MGR_LOG_UNLOCK (tel_conn_mgr_mutex_unlock(tel_conn_mgr_log_mutex))
#else
#define TEL_CONN_MGR_LOG_LOCK
#define TEL_CONN_MGR_LOG_UNLOCK
#endif


#define TEL_CONN_MGR_GLOBAL_LOCK_SLEEP tel_conn_mgr_global_lock_sleep()
#define TEL_CONN_MGR_GLOBAL_UNLOCK_SLEEP tel_conn_mgr_global_unlock_sleep()

tel_conn_mgr_bool tel_conn_mgr_is_special_cid(int cid);

tel_conn_mgr_bool tel_conn_mgr_is_cid_valid(int cid);

tel_conn_mgr_bool tel_conn_mgr_is_special_idx(unsigned int idx);

tel_conn_mgr_bool tel_conn_mgr_is_idx_valid(unsigned int idx);

tel_conn_mgr_bool tel_conn_mgr_str_case_equal(const char *s1, const char *s2, unsigned int len);

void tel_conn_mgr_global_mutex_lock(const char *caller);

void tel_conn_mgr_global_mutex_unlock(const char *caller);

tel_conn_mgr_bool tel_conn_mgr_send_msg_to_tcm_task(tel_conn_mgr_msg_struct *msg);

tel_conn_mgr_ret_enum tel_conn_mgr_list_insert(tel_conn_mgr_template_list_struct **list,
                                                     tel_conn_mgr_template_list_struct *list_node);

/* The node will not be freed in this API. */
tel_conn_mgr_ret_enum tel_conn_mgr_list_remove(tel_conn_mgr_template_list_struct **list,
                                                      tel_conn_mgr_template_list_struct *list_node);

tel_conn_mgr_queue_hdl_t tel_conn_mgr_task_queue_get(void);

tel_conn_mgr_ret_enum tel_conn_mgr_util_init(void);

tel_conn_mgr_ret_enum tel_conn_mgr_notify_app(tel_conn_mgr_app_msg_type_enum app_msg_type,
                                                     int cid,
                                                     tel_conn_mgr_bool result,
                                                     tel_conn_mgr_err_cause_enum cause,
                                                     tel_conn_mgr_thread_info_struct *specified_thread_info,
                                                     tel_conn_mgr_app_info_struct *specified_app_info);

void tel_conn_mgr_update_app_state_for_activation(tel_conn_mgr_bool result,                                                              
                                                              int cid,
                                                              tel_conn_mgr_bool need_deact);

void tel_conn_mgr_update_app_state_for_active_deactivation(int cid);

void tel_conn_mgr_update_app_state_for_passive_deactivation(int cid);

unsigned int tel_conn_mgr_convt_cid_to_bearer_info_idx(int cid);

int tel_conn_mgr_convt_bearer_info_idx_to_cid(unsigned int bearer_info_idx);

tel_conn_mgr_ret_enum tel_conn_mgr_app_init(void);

void tel_conn_mgr_app_deinit(void);

void tel_conn_mgr_util_deinit(void);

tel_conn_mgr_ret_enum tel_conn_mgr_get_cid_by_app_id(int *cid, unsigned int app_id);

void tel_conn_mgr_general_lock_sleep(unsigned char sleep_manager_handle,
                                               int *sleep_lock_count);

void tel_conn_mgr_general_unlock_sleep(unsigned char sleep_manager_handle,
                                                 int *sleep_lock_count);

#endif /* __TEL_CONN_MGR_UTIL_H__ */

