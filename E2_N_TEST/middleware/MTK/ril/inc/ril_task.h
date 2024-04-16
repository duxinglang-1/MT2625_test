/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
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
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
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

#ifndef __RIL_TASK_H__
#define __RIL_TASK_H__
#include "ril_general_types.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include "ril.h"
#include "ril_cmds_def.h"

#include "mux_ap.h"


#define RIL_BUF_SIZE    (1*1024)


typedef enum {
    RIL_MSG_REQUEST,
    RIL_MSG_RESPONSE,
    RIL_MSG_URC,
    RIL_MSG_CHANNEL_ENABLED,
    RIL_MSG_CHANNEL_DISABLED,
    RIL_MSG_SEND_COMPLETE,
    RIL_MSG_DATA_CHANNEL_RESERVED,
    RIL_MSG_DATA_CHANNEL_UNRESERVED,
} ril_message_type_t;

typedef struct {
    ril_message_type_t msg;
    void *buf;
    uint32_t len;
} ril_message_t;


typedef enum {
    RIL_CHANNEL_STATE_READY,
    RIL_CHANNEL_STATE_BUSY,
    RIL_CHANNEL_STATE_UNAVAILABLE,
    RIL_CHANNEL_STATE_WAIT_TO_WAKEUP
} ril_channel_state_t;

typedef struct {
    int32_t channel_id;
    ril_cmd_type_t type;
    ril_channel_state_t state;
    uint32_t curr_request_id;
    uint32_t curr_request_mode;
    void *usr_rsp_cb;
    void *usr_data;
    uint32_t timestamp;
    uint32_t elapsed_time;
    char tx_buf[RIL_BUF_SIZE + 8]; // add magic number length for header, footer 
    char *tx_buf_ptr;
    int32_t tx_pos;
    char rx_buf[RIL_BUF_SIZE + 8]; // add magic number length for header, footer
    char *rx_buf_ptr;
    int32_t rx_pos;
    char *rx_long_str_ptr;
    int32_t rx_long_str_len;
    int32_t reserved;
    int32_t recv_forbidden;
} ril_channel_cntx_t;

typedef struct _ril_request_pending_node_t {
    ril_cmd_type_t cmd_type;
    ril_func_group_t func_group;
    int32_t specified_channel_id;
    ril_request_info_t *info;
    struct _ril_request_pending_node_t *next;
} ril_request_pending_node_t;

typedef struct {
    uint32_t busy_func_group_mask;
    uint32_t busy_channel_mask;
    int32_t num_pending_items;
    ril_request_pending_node_t *ril_request_pending_list;
    char *custom_cmd_head;
    uint32_t custom_cmd_info_mask;
    bool custom_cmd_is_multiline;
    bool suspend_pending_items;
} ril_cntx_t;


extern ril_cntx_t ril_cntx;

extern void ril_init();
extern ril_channel_cntx_t *ril_get_channel(uint32_t channel_id);
extern int32_t ril_free_channel(uint32_t channel_id);
extern int32_t ril_alloc_channel(ril_cmd_type_t cmd_type, bool reverse);
extern void ril_channel_mux_callback(mux_ap_event_t event, void *param);
extern void ril_send_message(ril_message_type_t msg, void *buf, uint32_t len);
extern void ril_send_message_int(ril_message_type_t msg, void *buf, uint32_t len);
extern void ril_notify_event(uint32_t event_id, void *param, uint32_t param_len);
extern void ril_init_event_callback_table();
extern void ril_init_channel_after_deep_sleep();
extern void ril_request_pending_list_suspend();
extern void ril_request_pending_list_resume();

#ifdef __cplusplus
}
#endif
#endif /* __RIL_TASK_H__ */

