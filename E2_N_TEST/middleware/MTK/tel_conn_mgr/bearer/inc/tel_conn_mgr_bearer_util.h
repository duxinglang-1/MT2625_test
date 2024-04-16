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

#ifndef __TEL_CONN_MGR_BEARER_UTIL_H__
#define __TEL_CONN_MGR_BEARER_UTIL_H__

#include "tel_conn_mgr_platform.h"
#include "tel_conn_mgr_bearer_iprot.h"


#ifdef TEL_CONN_MGR_ENABLE_INACTIVATE
tel_conn_mgr_ret_enum tel_conn_mgr_bearer_terminate_initing_state(int cid, unsigned short cause);
#endif

tel_conn_mgr_ret_enum tel_conn_mgr_bearer_terminate_activating_state(int cid,
                                                                                 tel_conn_mgr_bool result,
                                                                                 unsigned short cause,
                                                                                 tel_conn_mgr_bool need_deact);

tel_conn_mgr_bool tel_conn_mgr_is_bearer_inactive(tel_conn_mgr_bearer_state_enum bearer_state);

tel_conn_mgr_bool tel_conn_mgr_is_bearer_active(tel_conn_mgr_bearer_state_enum bearer_state);

tel_conn_mgr_bool tel_conn_mgr_is_bearer_activating(tel_conn_mgr_bearer_state_enum bearer_state);

tel_conn_mgr_bool tel_conn_mgr_is_bearer_deactivating(tel_conn_mgr_bearer_state_enum bearer_state);

tel_conn_mgr_bool tel_conn_mgr_is_nw_registered(void);

tel_conn_mgr_bool tel_conn_mgr_is_nw_registration_failed(void);


#ifdef TEL_CONN_MGR_ACTIVATION_GUARD_TIMER_SUPPORT
void tel_conn_mgr_activation_timeout_hdlr(unsigned int timer_id, int cid);
#endif


#ifdef TEL_CONN_MGR_DEACTIVATION_GUARD_TIMER_SUPPORT
void tel_conn_mgr_deactivation_timeout_hdlr(unsigned int timer_id, int cid);
#endif

tel_conn_mgr_bool tel_conn_mgr_is_valid_at_cmd_class(tel_conn_mgr_at_cmd_class_enum at_cmd_class);


tel_conn_mgr_ret_enum tel_conn_mgr_bearer_terminate_deactivating_state(int cid, tel_conn_mgr_bool result, unsigned short cause);

void tel_conn_mgr_update_bearer_state_for_class_start(tel_conn_mgr_bearer_state_enum *bearer_state,
                                                                   tel_conn_mgr_at_cmd_class_enum at_cmd_class);

void tel_conn_mgr_revert_bearer_state_for_class_start(tel_conn_mgr_bearer_state_enum *bearer_state,
                                                                  tel_conn_mgr_at_cmd_class_enum at_cmd_class);

void tel_conn_mgr_bearer_turn_into_inactive_state_all(unsigned short cause, tel_conn_mgr_bool need_deact);

tel_conn_mgr_ret_enum tel_conn_mgr_bearer_turn_into_inactive_state_by_cid(int cid,
                                                                                   unsigned short cause,
                                                                                   tel_conn_mgr_bool need_deact);
void tel_conn_mgr_bearer_lock_sleep(void);


void tel_conn_mgr_bearer_unlock_sleep(void);

tel_conn_mgr_bool tel_conn_mgr_is_cid_active(int cid);

#endif /* __TEL_CONN_MGR_BEARER_UTIL_H__ */

