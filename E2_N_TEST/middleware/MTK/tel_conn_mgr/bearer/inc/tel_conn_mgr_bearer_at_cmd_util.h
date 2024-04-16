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

#ifndef __TEL_CONN_MGR_BARER_AT_CMD_UTIL_H__
#define __TEL_CONN_MGR_BARER_AT_CMD_UTIL_H__

#include "tel_conn_mgr_platform.h"
#include "tel_conn_mgr_common.h"
#include "tel_conn_mgr_util.h"
#include "tel_conn_mgr_bearer_list_mgr.h"
#include "tel_conn_mgr_bearer_iprot.h"
#include "tel_conn_mgr_bearer_info.h"


#define TEL_CONN_MGR_AT_CMD_RSP_MAX_LEN (60)

/* NULL-terminator is not included */
#define TEL_CONN_MGR_IPV4_STR_MAX_LEN   (15)
#define TEL_CONN_MGR_IPV6_STR_MAX_LEN   (63)


#if 0
typedef enum {
    TEL_CONN_MGR_RESPONSE_OK,
    TEL_CONN_MGR_RESPONSE_ERROR = -1,
    TEL_CONN_MGR_RESPONSE_UNKNOW = -2,
    TEL_CONN_MGR_RESPONSE_INVALID = -3,
    
    TEL_CONN_MGR_RESPONSE_MAX
} tel_conn_mgr_response_type_enum;
#endif

typedef struct tel_conn_mgr_bearer_ip_info
{
    struct tel_conn_mgr_bearer_ip_info *next;
    int is_ipv6;
    char local_addr[TEL_CONN_MGR_IPV6_STR_MAX_LEN + 1];
    char subnet_mask[TEL_CONN_MGR_IPV6_STR_MAX_LEN + 1];
    char gw_addr[TEL_CONN_MGR_IPV6_STR_MAX_LEN + 1];
    char dns_prim_addr[TEL_CONN_MGR_IPV6_STR_MAX_LEN + 1];
    char dns_sec_addr[TEL_CONN_MGR_IPV6_STR_MAX_LEN + 1];
    int32_t ipv4_mtu; /**< Indicates the IPv4 MTU size in octets. */
    int32_t nonip_mtu; /**< Indicates the Non-IP MTU size in octects. */
}tel_conn_mgr_bearer_ip_info_struct;


typedef struct
{
    unsigned int msg_id;  /* MSG_ID_TEL_CONN_MGR_BEARER_AT_CMD_RSP_IND */
    int cid;
    tel_conn_mgr_at_cmd_type_enum at_cmd_type;
    tel_conn_mgr_bool result;
    void *rsp_detail;
}tel_conn_mgr_bearer_at_cmd_rsp_ind_struct;


#if defined(TEL_CONN_MGR_SUPPORT_DEEP_SLEEP) && !defined(TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM)
/* Group Name */
#define TEL_CONN_MGR_NVDM_COMMON_GROUP  "tcm_common"
#define TEL_CONN_MGR_NVDM_BEARER_INFO_GROUP_BASE  "tcm_bearer_info_%d"

/* Item Name */
#define TEL_CONN_MGR_NVDM_COMMON_NW_REG_STATUS  "nw_reg_status"
#define TEL_CONN_MGR_NVDM_BEARER_INFO_IS_USED  "is_used"
#define TEL_CONN_MGR_NVDM_BEARER_INFO_CID  "cid"
#define TEL_CONN_MGR_NVDM_BEARER_INFO_PDP_TYPE  "pdp_type"
#define TEL_CONN_MGR_NVDM_BEARER_INFO_ACTIVATED_PDP_TYPE  "activated_pdp_type"
#define TEL_CONN_MGR_NVDM_BEARER_INFO_APN  "apn"
#define TEL_CONN_MGR_NVDM_BEARER_INFO_DATA_CHANNEL_ID  "data_chn_id"
#define TEL_CONN_MGR_NVDM_BEARER_INFO_BEARER_STATE  "bearer_state"
#endif


/* the idx of the arraies should be aligned with bearer_info array. */
typedef struct
{
    /* modem_state may be phase-out for RIL will not report it. */
    tel_conn_mgr_modem_state_enum modem_state;
#ifndef TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM
    tel_conn_mgr_nw_reg_status_struct nw_reg_status;  /* Updated by CGREG URC */
#endif
    tel_conn_mgr_bearer_info_struct bearer_info[TEL_CONN_MGR_BEARER_TYPE_MAX_NUM];
    tel_conn_mgr_bearer_state_enum bearer_state; /* For init class only. */
    tel_conn_mgr_at_cmd_flow_helper_struct init_class_helper;
#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
    unsigned char sleep_manager_handle;
    int sleep_lock_count;
#endif
}tel_conn_mgr_bearer_cntx_struct;


tel_conn_mgr_ret_enum tel_conn_mgr_send_next_at_cmd(int cid);


tel_conn_mgr_ret_enum tel_conn_mgr_send_at_cmds_by_class(int cid,
                                            tel_conn_mgr_at_cmd_class_enum cmd_class,
                                            tel_conn_mgr_at_cmd_type_enum cmd_type_begin,
                                            void *callback);


#if 0
tel_conn_mgr_response_type_enum tel_conn_mgr_parse_response_data(unsigned char *payload, unsigned int length);
#endif

tel_conn_mgr_bearer_cntx_struct *tel_conn_mgr_bearer_get_cntx(void);

tel_conn_mgr_nw_reg_status_struct *tel_conn_mgr_bearer_get_nw_reg_status(void);

tel_conn_mgr_at_cmd_type_enum tel_conn_mgr_get_next_at_cmd_by_class(tel_conn_mgr_at_cmd_class_enum cmd_class, tel_conn_mgr_at_cmd_type_enum cmd_type);

void tel_conn_mgr_bearer_at_cmd_rsp_ind_free(tel_conn_mgr_bearer_at_cmd_rsp_ind_struct *at_cmd_rsp_ind);

//tel_conn_mgr_ret_enum tel_conn_mgr_ready_process(void);

//tel_conn_mgr_ret_enum tel_conn_mgr_exception_process(void);

//int tel_conn_mgr_at_cmd_get_app_id(void);

tel_conn_mgr_ret_enum tel_conn_mgr_at_cmd_rsp_process(tel_conn_mgr_bearer_at_cmd_rsp_ind_struct *at_cmd_rsp_ind);

tel_conn_mgr_ret_enum tel_conn_mgr_at_cmd_init(void);

void tel_conn_mgr_at_cmd_deinit(void);

tel_conn_mgr_bool tel_conn_mgr_is_at_cmd_in_class_group(tel_conn_mgr_at_cmd_type_enum at_cmd_type,
                                                       tel_conn_mgr_at_cmd_class_enum at_cmd_class);

tel_conn_mgr_ret_enum tel_conn_mgr_get_bearer_relevant_info(int cid);

#endif /* __TEL_CONN_MGR_BARER_AT_CMD_UTIL_H__ */

