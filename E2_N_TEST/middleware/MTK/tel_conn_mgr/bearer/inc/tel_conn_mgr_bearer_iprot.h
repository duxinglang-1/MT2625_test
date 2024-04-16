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

#ifndef __TEL_CONN_MGR_BEARER_IPROT_H__
#define __TEL_CONN_MGR_BEARER_IPROT_H__

#include "tel_conn_mgr_platform.h"
#include "tel_conn_mgr_common.h"
#include "tel_conn_mgr_util.h"

#if 0
#define TEL_CONN_MGR_MAX_IPV4_ADDR_LEN    (15)
#define TEL_CONN_MGR_MIN_IPV4_ADDR_LEN    (7)
#endif

/* Update tel_conn_mgr_key_at_cmd_type_enum definition if act/deact at cmd changes */
typedef enum {
    TEL_CONN_MGR_AT_CMD_TYPE_NONE,
    TEL_CONN_MGR_AT_CMD_TYPE_ATE0,
    TEL_CONN_MGR_AT_CMD_TYPE_CTZR_ON,
    TEL_CONN_MGR_AT_CMD_TYPE_CFUN_OFF,
    TEL_CONN_MGR_AT_CMD_TYPE_CFUN_ON,
    TEL_CONN_MGR_AT_CMD_TYPE_CGEREP,	/* Enable unsolicated result codes(URC) or not. */
    TEL_CONN_MGR_AT_CMD_TYPE_CMEE,      /* 10 */
    TEL_CONN_MGR_AT_CMD_TYPE_ESLP_DISABLE,
    TEL_CONN_MGR_AT_CMD_TYPE_EIND,
    TEL_CONN_MGR_AT_CMD_TYPE_CGREG,
    TEL_CONN_MGR_AT_CMD_TYPE_CEREG,
    TEL_CONN_MGR_AT_CMD_TYPE_CEREG_QUERY,
    TEL_CONN_MGR_AT_CMD_TYPE_CGREG_QUERY,
    TEL_CONN_MGR_AT_CMD_TYPE_ERAT,
    TEL_CONN_MGR_AT_CMD_TYPE_CGDCONT,
    TEL_CONN_MGR_AT_CMD_TYPE_CGAUTH,
    TEL_CONN_MGR_AT_CMD_TYPE_CGDATA,
    TEL_CONN_MGR_AT_CMD_TYPE_CGCONTRDP,
    TEL_CONN_MGR_AT_CMD_TYPE_CGACT_DEACT,	/* GPRS Deactive */
    TEL_CONN_MGR_AT_CMD_TYPE_CGACT_QUERY,

    TEL_CONN_MGR_AT_CMD_TYPE_MAX
} tel_conn_mgr_at_cmd_type_enum;


/* Update this enum definition if act/deact at cmd changes */
typedef enum {
    TEL_CONN_MGR_KEY_AT_CMD_TYPE_NONE = TEL_CONN_MGR_AT_CMD_TYPE_NONE,
    TEL_CONN_MGR_KEY_AT_CMD_TYPE_ACT = TEL_CONN_MGR_AT_CMD_TYPE_CGDATA,
    TEL_CONN_MGR_KEY_AT_CMD_TYPE_DEACT = TEL_CONN_MGR_AT_CMD_TYPE_CGACT_DEACT
}tel_conn_mgr_key_at_cmd_type_enum;


/* Update  tel_conn_mgr_is_valid_at_cmd_class() when any at_cmd_class is added/removed/modified. */
typedef enum {
    TEL_CONN_MGR_AT_CMD_CLASS_NONE = 0,
    TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE = 0x01,
    TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE = 0x02,
    TEL_CONN_MGR_AT_CMD_CLASS_GET_RELEVANT_INFO = 0x04,
    TEL_CONN_MGR_AT_CMD_CLASS_DEACTIVATE = 0x08,

    TEL_CONN_MGR_AT_CMD_CLASS_ALL = 0xFF
} tel_conn_mgr_at_cmd_class_enum;


typedef enum {
    TEL_CONN_MGR_MODEM_STATE_NOT_READY,
    TEL_CONN_MGR_MODEM_STATE_READY,
    TEL_CONN_MGR_MODEM_STATE_EXCEPTION,

    TEL_CONN_MGR_MODEM_STATE_MAX
} tel_conn_mgr_modem_state_enum;


/* Udpate tel_conn_mgr_is_wait_urc_state() if any WAIT_URC state is added/removed/modified. */
typedef enum
{
    TEL_CONN_MGR_BEARER_STATE_NONE = 0x0,
        
#ifdef TEL_CONN_MGR_ENABLE_INACTIVATE
    TEL_CONN_MGR_BEARER_STATE_INACTIVATING = 0x20,
#endif
    TEL_CONN_MGR_BEARER_STATE_INACTIVE = 0x02,
    
    TEL_CONN_MGR_BEARER_STATE_ACTIVATING = 0x04,
    TEL_CONN_MGR_BEARER_STATE_ACTIVE = 0x08,
    
    TEL_CONN_MGR_BEARER_STATE_DEACTIVATING = 0x10,

    TEL_CONN_MGR_BEARER_STATE_ACTIVATING_WAIT_ACT_URC = 0x200,
    TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_WAIT_DEACT_URC = 0x400,

    TEL_CONN_MGR_BEARER_STATE_ACTIVATING_ACTED = 0x800,
    TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_DEACTED = 0x1000,

    TEL_CONN_MGR_BEARER_STATE_NEED_DEACT = 0x2000,

    TEL_CONN_MGR_BEARER_STATE_MAX = 0xFFFF
}tel_conn_mgr_bearer_state_enum;

typedef enum
{
    TEL_CONN_MGR_BEARER_WAIT_EVENT_NONE = 0,
    TEL_CONN_MGR_BEARER_WAIT_EVENT_INACTIVE_DONE = 0x01,
    TEL_CONN_MGR_BEARER_WAIT_EVENT_NW_REG_RESULT = 0x02,

    TEL_CONN_MGR_BEARER_WAIT_EVENT_ALL = 0xFF
}tel_conn_mgr_bearer_wait_event_enum;



#define TEL_CONN_MGR_BEARER_STATE_ACTIVATING_ALL (TEL_CONN_MGR_BEARER_STATE_ACTIVATING | \
                                                  TEL_CONN_MGR_BEARER_STATE_ACTIVATING_WAIT_ACT_URC | \
                                                  TEL_CONN_MGR_BEARER_STATE_ACTIVATING_ACTED)

#define TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_ALL (TEL_CONN_MGR_BEARER_STATE_DEACTIVATING | \
                                                    TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_WAIT_DEACT_URC | \
                                                    TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_DEACTED)



typedef enum {
    TEL_CONN_MGR_NW_REG_STAT_NOT_REGED,  /* not registered, and not searching */
    TEL_CONN_MGR_NW_REG_STAT_REGED_HMNW,  /* registered, home network */
    TEL_CONN_MGR_NW_REG_STAT_TRYING,  /* not registered, trying or searching */
    TEL_CONN_MGR_NW_REG_STAT_REG_DENIED,  /* registration denied */
    TEL_CONN_MGR_NW_REG_STAT_UNKNOWN,  /* Unknown */
    TEL_CONN_MGR_NW_REG_STAT_REGED_ROAMING,  /* registered, roaming */
    TEL_CONN_MGR_NW_REG_STAT_SMS_ONLY_HMNW,  /* "SMS only", home network */
    TEL_CONN_MGR_NW_REG_STAT_SMS_ONLY_ROAMING,  /* "SMS only", roaming */
    TEL_CONN_MGR_NW_REG_STAT_EMERGENCY_ONLY,  /* emergency bearer services only  */
    TEL_CONN_MGR_NW_REG_STAT_REGED_CSFB_NPREF_HMNW,  /* registered for "CSFB not preferred", home network (not applicable)*/
    TEL_CONN_MGR_NW_REG_STAT_REGED_CSFB_NPREF_ROAMING,  /* registered for "CSFB not preferred", roaming (not applicable)*/

    TEL_CONN_MGR_NW_REG_STAT_MAX = 0xFF
} tel_conn_mgr_nw_reg_stat_enum;

typedef enum
{
    TEL_CONN_MGR_NW_REG_ACT_GSM = 0,
    TEL_CONN_MGR_NW_REG_ACT_UTRAN = 2,
    TEL_CONN_MGR_NW_REG_ACT_GSM_EGPRS,
    TEL_CONN_MGR_NW_REG_ACT_UTRAN_HSDPA,
    TEL_CONN_MGR_NW_REG_ACT_UTRAN_HSUPA,
    TEL_CONN_MGR_NW_REG_ACT_UTRAN_HSDPA_HSUPA,
    TEL_CONN_MGR_NW_REG_ACT_EUTRAN,

    TEL_CONN_MGR_NW_REG_ACT_MAX = 0xFF    
}tel_conn_mgr_nw_reg_act_enum;

#if 0
typedef void (*tel_conn_mgr_send_at_cmds_by_class_callback)(tel_conn_mgr_ret_enum result,
                                                                   tel_conn_mgr_err_cause_enum cause,
                                                                   int cid,
                                                                   tel_conn_mgr_at_cmd_class_enum at_cmd_class);
#endif

typedef struct
{
    #ifdef TEL_CONN_MGR_ENABLE_CHECK_CEREG
    unsigned char n;
    #endif
    tel_conn_mgr_nw_reg_stat_enum stat;
    tel_conn_mgr_nw_reg_act_enum act;
}tel_conn_mgr_nw_reg_status_struct;


/* The first three members of the structure must be the same as the first three of tel_conn_mgr_bearer_template_list_struct to
    be able to use APIs with tel_conn_mgr_bearer_template_list_struct type. */
typedef struct tel_conn_mgr_at_cmd_list
{
    struct tel_conn_mgr_at_cmd_list *next;
    tel_conn_mgr_at_cmd_class_enum at_cmd_class;
    int cid;
    tel_conn_mgr_at_cmd_type_enum at_cmd_type;
    tel_conn_mgr_bool is_expired;
}tel_conn_mgr_at_cmd_list_struct;


/* The first three members of the structure must be the same as the first three of tel_conn_mgr_bearer_template_list_struct to
    be able to use APIs with tel_conn_mgr_bearer_template_list_struct type. */
typedef struct tel_conn_mgr_class_list
{
    struct tel_conn_mgr_class_list* next;
    tel_conn_mgr_at_cmd_class_enum at_cmd_class;
    int cid;
    tel_conn_mgr_at_cmd_type_enum cmd_type_begin;    
    void *callback;
    tel_conn_mgr_bearer_wait_event_enum wait_event_group;
}tel_conn_mgr_class_list_struct;


typedef struct
{
    tel_conn_mgr_at_cmd_type_enum current_at_cmd_type;    // Only be updated by at cmd send/receive and modem exception. 
    tel_conn_mgr_key_at_cmd_type_enum last_key_at_cmd_type; /* NONE, CGDATA, CGACT_DEACT */
    tel_conn_mgr_at_cmd_list_struct *at_cmd_to_send_list;     // Wait for sending
    tel_conn_mgr_at_cmd_list_struct *at_cmd_to_recv_list;     // Wait for response    
    tel_conn_mgr_class_list_struct *class_to_excute;    // Callback using cid and at_cmd_class to distinguish
    tel_conn_mgr_class_list_struct *class_to_finish;
}tel_conn_mgr_at_cmd_flow_helper_struct;


typedef void (*tel_conn_mgr_activate_callback)(tel_conn_mgr_bool result,
                                                    tel_conn_mgr_err_cause_enum cause,
                                                    int cid,
                                                    tel_conn_mgr_bool need_deact);


typedef void (*tel_conn_mgr_deactivate_callback)(tel_conn_mgr_bool result,
                                                      tel_conn_mgr_err_cause_enum cause,
                                                      int cid);


#endif /* __TEL_CONN_MGR_BEARER_IPROT_H__ */

