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

#ifndef __TEL_CONN_MGR_APP_MGR_H__
#define __TEL_CONN_MGR_APP_MGR_H__

#include "tel_conn_mgr_platform.h"
#include "tel_conn_mgr_common.h"


#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
#define TEL_CONN_MGR_REG_INFO_MAX_NUM    (16)
#endif


typedef enum
{
    TEL_CONN_MGR_ACT_NONE,
    TEL_CONN_MGR_ACT_ACT,
    TEL_CONN_MGR_ACT_DEACT,
    TEL_CONN_MGR_ACT_REACT,
    TEL_CONN_MGR_ACT_MAX
}tel_conn_mgr_act_enum;


typedef enum
{
    TEL_CONN_MGR_APP_STATE_INACTIVE,
    TEL_CONN_MGR_APP_STATE_ACTIVATING,
    TEL_CONN_MGR_APP_STATE_ACTIVE,
    TEL_CONN_MGR_APP_STATE_DEACTIVATING,
    TEL_CONN_MGR_APP_STATE_REACTIVE,

    TEL_CONN_MGR_APP_STATE_MAX
}tel_conn_mgr_app_state_enum;


typedef struct tel_conn_mgr_app_info
{
    struct tel_conn_mgr_app_info *next;
    int app_id;
    tel_conn_mgr_pdp_type_enum pdp_type;
    tel_conn_mgr_bool is_act_req_cached;
    tel_conn_mgr_act_enum act;
}tel_conn_mgr_app_info_struct;


/* The index of the app_info is corresponding to the one of the bearer_info. */
typedef struct tel_conn_mgr_thread_info
{
    struct tel_conn_mgr_thread_info *next;
    tel_conn_mgr_queue_hdl_t queue_hdl;      // Message queue_handle. One of queue_handle and reg_hdl should be NULL.
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
    tel_conn_mgr_register_handle_t reg_hdl;    // Callback to tell the result. One of queue_hdl and reg_hdl should be NULL.
#endif
    /* Each item of the app_info array is related to the item of the app_state array with the same index. */
    tel_conn_mgr_app_info_struct *app_info[TEL_CONN_MGR_BEARER_TYPE_MAX_NUM];
}tel_conn_mgr_thread_info_struct;


#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
typedef struct
{
    tel_conn_mgr_register_handle_t reg_hdl;
    tel_conn_mgr_cb callback;
}tel_conn_mgr_reg_info_struct;
#endif


typedef struct
{    
    tel_conn_mgr_thread_info_struct *thread_info_list;    
    tel_conn_mgr_app_state_enum app_state[TEL_CONN_MGR_BEARER_TYPE_MAX_NUM];
#ifdef TEL_CONN_MGR_SUPPORT_CALLBACK
    tel_conn_mgr_reg_info_struct reg_info[TEL_CONN_MGR_REG_INFO_MAX_NUM];
#endif
#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
    unsigned char sleep_manager_handle;
    int sleep_lock_count;
#endif
}tel_conn_mgr_app_cntx_struct;


tel_conn_mgr_app_state_enum tel_conn_mgr_app_get_app_state(unsigned int idx);


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
                                                        tel_conn_mgr_app_info_struct **app_info);

tel_conn_mgr_ret_enum tel_conn_mgr_app_update_app_state(unsigned int idx,
                                                        tel_conn_mgr_app_state_enum app_state);

tel_conn_mgr_ret_enum tel_conn_mgr_get_app_info(unsigned int app_id,
                                                        tel_conn_mgr_thread_info_struct **thread_info,
                                                        tel_conn_mgr_app_info_struct **app_info,
                                                        unsigned int *bearer_info_idx);

tel_conn_mgr_ret_enum tel_conn_mgr_free_app_info(tel_conn_mgr_thread_info_struct *thread_info,
                                                        tel_conn_mgr_app_info_struct *app_info,
                                                        unsigned int app_info_idx);

tel_conn_mgr_ret_enum tel_conn_mgr_is_pdp_type_compatible(tel_conn_mgr_pdp_type_enum pdp_type_new,
                                                          tel_conn_mgr_pdp_type_enum pdp_type_used,
                                                          tel_conn_mgr_pdp_type_enum activated_pdp_type,
                                                          tel_conn_mgr_app_state_enum app_state);

tel_conn_mgr_ret_enum tel_conn_mgr_send_act_req(int cid);

tel_conn_mgr_app_cntx_struct *tel_conn_mgr_app_get_cntx(void);


tel_conn_mgr_ret_enum tel_conn_mgr_send_deact_req(int cid);


tel_conn_mgr_bool tel_conn_mgr_does_specified_app_exist(unsigned int excluded_app_id,
                                                                   int bearer_info_idx,
                                                                   tel_conn_mgr_act_enum act);

tel_conn_mgr_ret_enum tel_conn_mgr_app_update_app_act(int app_info_idx,
                                                                 tel_conn_mgr_act_enum target_act,
                                                                 tel_conn_mgr_act_enum new_act);

void tel_conn_mgr_app_lock_sleep(void);

void tel_conn_mgr_app_unlock_sleep(void);

#endif /* __TEL_CONN_MGR_APP_MGR_H__ */

