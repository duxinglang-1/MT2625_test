/* Copyright Statement:
 *
 * (C) 2005-2017  MediaTek Inc. All rights reserved.
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

/**************************************************************************
* File Description
* ----------------
*
* Define all the internal used data types for AP Bridge proxy.
**************************************************************************/

#ifndef __APB_PROXY_DATA_TYPE__H__
#define __APB_PROXY_DATA_TYPE__H__

#include "mux_ap.h"
#include "stdint.h"
#include "stdbool.h"
#include "apb_proxy.h"

#ifdef __cplusplus
extern "C" {
#endif
/****************************************************************************
** AP Bridge internal Macro definitions
****************************************************************************/
#ifndef APB_INVALID_CMD_ID
#define APB_INVALID_CMD_ID 0xFFFFFFFF
#endif

#ifndef APB_INVALID_CHANNEL_ID
#define APB_INVALID_CHANNEL_ID 0xFFFFFFFF
#endif
/***************************************************************************
** AP Bridge internal event types
***************************************************************************/
typedef enum {
    /*Events come from MUX adapter.*/
    APB_PROXY_MUX_EVENT_CHANNEL_ENABLED,
    APB_PROXY_MUX_EVENT_CHANNEL_DISABLED,
    APB_PROXY_MUX_EVENT_SEND_COMPLETED,
    APB_PROXY_MUX_EVENT_RECEIVE_COMPLETED,
    /*Events come from APP*/
    APB_PROXY_AP_SET_UP_DATA_MODE_REQ,
    APB_PROXY_AP_CLOSE_DATA_MODE_IND,
    APB_PROXY_AP_REG_AT_CMD_REQ,
    APB_PROXY_AP_AT_CMD_RESULT_IND,
    APB_PROXY_AP_SEND_USER_DATA_REQ,
    APB_PROXY_AP_XON_REQ,
    APB_PROXY_AP_XOFF_REQ,
    APB_PROXY_AP_DATA_MODE_RESUME_REQ,
    APB_PROXY_AP_DATA_MODE_CLOSE_REQ,
    /*Events come from modem.*/
    APB_PROXY_MD_REG_AT_CMD_RSP,
    APB_PROXY_MD_AT_CMD_REQ,
    APB_PROXY_MD_USER_DATA_IND,
    APB_PROXY_MD_XON_REQ,
    APB_PROXY_MD_XOFF_REQ,
    APB_PROXY_MD_DATA_MODE_CLOSED_IND,
    APB_PROXY_MD_DATA_MODE_TEMP_DEACTIVE_IND,
    APB_PROXY_MD_DATA_MODE_RESUMED_IND
} apb_proxy_internal_event_type_t;

typedef struct {
    uint32_t           channel_id;
    uint32_t           length;
    uint8_t           *pdata;
} apb_proxy_packet_t;

typedef struct {
    uint32_t           cmd_id; /*The command ID in modem.*/
    uint32_t           channel_id;
    apb_proxy_packet_t result_packet;
} apb_proxy_cmd_result_event_t;

typedef struct {
    uint32_t cmd_id; /*The command ID in modem.*/
    uint32_t channel_id;
    apb_proxy_data_mode_event_callback_t callback;
} apb_proxy_data_mode_req_t;

typedef union {
    mux_ap_event_channel_enabled_t   channel_enabled_event;
    mux_ap_event_channel_disabled_t  channel_disabled_event;
    mux_ap_event_send_completed_t    send_completed_event;
    mux_ap_event_receive_completed_t receive_completed_event;
    apb_proxy_packet_t               packetdata;
    apb_proxy_data_mode_req_t        data_mode_req;
    apb_proxy_cmd_result_event_t     cmd_result_ind;
    uint32_t channel_id;
} apb_proxy_event_data_t;

typedef struct {
    apb_proxy_internal_event_type_t event_type;
    apb_proxy_event_data_t event_data;
} apb_proxy_event_t;

/** @ The structure defines AT command register result.*/
typedef struct {
    uint32_t  base_cmd_id;     /**< the begining cmd id which modem atci allocated */
    uint32_t  registered_count;/**< registered at cmd count.*/
    bool      result;          /**< AT Command register result */
} apb_proxy_register_result_t;

typedef struct {
    apb_proxy_at_cmd_handler cmd_handler;
    apb_proxy_parse_cmd_param_t cmd_param;
    void* share_buf;
} apb_proxy_exe_cmd_t;

#ifdef __cplusplus
}
#endif

#endif/*__APB_PROXY_DATA_TYPE__H__*/

