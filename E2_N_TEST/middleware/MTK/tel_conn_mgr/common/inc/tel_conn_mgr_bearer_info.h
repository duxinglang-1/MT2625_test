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

#ifndef __TEL_CONN_MGR_BEARER_INFO_H__
#define __TEL_CONN_MGR_BEARER_INFO_H__

#include "tel_conn_mgr_platform.h"
#include "tel_conn_mgr_common.h"
#include "tel_conn_mgr_bearer_iprot.h"

#define TEL_CONN_MGR_MIN_VALID_CHANNEL_ID  (1)

/* Below information should be kept at deep sleep stage. */
typedef struct
{
    tel_conn_mgr_pdp_type_enum pdp_type;
    tel_conn_mgr_pdp_type_enum activated_pdp_type;
    char apn[TEL_CONN_MGR_APN_MAX_LEN + 1];
    int cid;
    int data_channel_id;
    tel_conn_mgr_bearer_state_enum bearer_state;
}tel_conn_mgr_bearer_info_ds_keep_struct;


typedef struct
{
    tel_conn_mgr_bearer_type_enum bearer_type;
    tel_conn_mgr_sim_id_enum sim_id;    
    tel_conn_mgr_pdp_type_enum reactive_pdp_type;
    char username[TEL_CONN_MGR_USERNAME_MAX_LEN + 1];
    char password[TEL_CONN_MGR_PASSWORD_MAX_LEN + 1];
    tel_conn_mgr_bool is_used;
#ifndef TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM
    tel_conn_mgr_bearer_info_ds_keep_struct ds_keep; /* else these information are stored in g_ds_keep[]. */
#endif
    tel_conn_mgr_at_cmd_flow_helper_struct at_cmd_flow_helper;
}tel_conn_mgr_bearer_info_struct;


tel_conn_mgr_bearer_info_struct *tel_conn_mgr_bearer_info_find_free_slot(unsigned int *bearer_info_idx);


void tel_conn_mgr_bearer_info_free(unsigned int bearer_info_idx);


tel_conn_mgr_bearer_info_struct *tel_conn_mgr_bearer_info_find_by_info(tel_conn_mgr_bearer_type_enum bearer_type,
                                                                      tel_conn_mgr_sim_id_enum sim_id,
                                                                      tel_conn_mgr_pdp_type_enum pdp_type,
                                                                      char *apn,
                                                                      unsigned int *bearer_info_idx);

tel_conn_mgr_bearer_info_struct *tel_conn_mgr_bearer_info_find_by_idx(int bearer_info_idx);

tel_conn_mgr_bearer_info_struct *tel_conn_mgr_bearer_info_find_by_cid(int cid);

tel_conn_mgr_bearer_info_ds_keep_struct *tel_conn_mgr_bearer_info_get_ds_keep_by_cid(int cid);

tel_conn_mgr_bearer_info_ds_keep_struct *tel_conn_mgr_bearer_info_get_ds_keep_by_idx(unsigned int idx);
tel_conn_mgr_bearer_info_ds_keep_struct *tel_conn_mgr_bearer_info_get_ds_keep_by_idx_ds(unsigned int idx);

tel_conn_mgr_bearer_state_enum *tel_conn_mgr_get_bearer_state_by_cid(int cid);

tel_conn_mgr_bearer_state_enum *tel_conn_mgr_get_bearer_state_by_idx(unsigned int bearer_info_idx);

tel_conn_mgr_at_cmd_flow_helper_struct *tel_conn_mgr_find_at_cmd_flow_helper_by_idx(unsigned int bearer_info_idx);

tel_conn_mgr_at_cmd_flow_helper_struct *tel_conn_mgr_find_at_cmd_flow_helper_by_cid(int cid);

void tel_conn_mgr_bearer_info_ds_keep_init(void);

#endif /* __TEL_CONN_MGR_BEARER_INFO_H__ */

