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

#ifndef __TEL_CONN_MGR_UT_H__
#define __TEL_CONN_MGR_UT_H__

#include "tel_conn_mgr_common.h"

#if defined(TEL_CONN_MGR_UT) || defined(TEL_CONN_MGR_IT)
#include "ril_cmds_def.h"
#include "ril.h"


#define MSG_ID_TEL_CONN_MGR_UT_APP_ACT_REQ    0x400
#define MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ    0x401
#define MSG_ID_TEL_CONN_MGR_UT_APP_TEST_CASE_START      0x402
#define MSG_ID_TEL_CONN_MGR_UT_APP_TEST_CASE_END      0x403


#ifdef TEL_CONN_MGR_UT
/* tel_conn_mgr task */
#ifndef TEL_CONN_MGR_TASK_NAME
#define TEL_CONN_MGR_TASK_NAME              ("tel_conn_mgr")
#define TEL_CONN_MGR_TASK_STACKSIZE         (4 * 1024)
#define TEL_CONN_MGR_TASK_PRIO              TASK_PRIORITY_NORMAL
#endif
#endif


void tel_conn_mgr_ut_app_send_msg_to_app_task1(int msg_id,
                                               int test_case_num,
                                               int data, /* result or app_id idx*/
                                               tel_conn_mgr_pdp_type_enum app_pdp_type,
                                               char *app_apn);

void tel_conn_mgr_ut_init(int app_num, int test_case_num);


#ifdef TEL_CONN_MGR_UT
ril_status_t ril_request_define_pdp_context_ut(ril_request_mode_t mode,
                                                       ril_pdp_context_entity_t *req,
                                                       ril_cmd_response_callback_t callback, 
                                                       void* user_data,
                                                       int32_t channel_id);

ril_status_t ril_request_define_pdp_context_authentication_parameters_ut(ril_request_mode_t mode,
                                                                int32_t cid,
                                                                int32_t auth_port,
                                                                char* userid,
                                                                char* password,
                                                                ril_cmd_response_callback_t callback, 
                                                                void* user_data,
                                                                int32_t channel_id);

ril_status_t ril_request_enter_data_state_ut(ril_request_mode_t mode,
                                                ril_enter_data_state_req_t *req,
                                                ril_cmd_response_callback_t callback, 
                                                void* user_data,
                                                int32_t channel_id);


ril_status_t ril_request_eps_network_registration_status_ut(ril_request_mode_t mode,
                                                                       int32_t n,
                                                                       ril_cmd_response_callback_t callback, 
                                                                       void* user_data);

ril_status_t ril_request_packet_domain_event_reporting_ut(ril_request_mode_t mode,
                                                                      int32_t cmode,
                                                                      int32_t bfr,
                                                                      ril_cmd_response_callback_t callback, 
                                                                      void* user_data);

ril_status_t ril_request_pdp_context_activate_or_deactivate_ut(ril_request_mode_t mode,
                                                                           ril_pdp_context_activate_or_deactivate_req_t *req,
                                                                           ril_cmd_response_callback_t callback, 
                                                                           void* user_data);

ril_status_t ril_request_pdp_context_read_dynamic_parameters_ut(ril_request_mode_t mode,
                                                                               int32_t cid,
                                                                               ril_cmd_response_callback_t callback, 
                                                                               void* user_data);

ril_status_t ril_register_event_callback_ut(uint32_t group_mask, ril_event_callback_t callback);

ril_status_t ril_deregister_event_callback_ut(ril_event_callback_t callback);
#endif  /* TEL_CONN_MGR_UT */

tel_conn_mgr_bool tel_conn_mgr_ut_is_test_ready(void);

void tel_conn_mgr_ut_test_case_run(int test_case_num);

#ifdef TEL_CONN_MGR_APB_SUPPORT
void tel_conn_mgr_ut_set_apb_cmd_id(unsigned int apb_cmd_id);
#endif

#endif /* defined(TEL_CONN_MGR_UT) || defined(TEL_CONN_MGR_IT) */

#endif /* __TEL_CONN_MGR_UT_H__ */

