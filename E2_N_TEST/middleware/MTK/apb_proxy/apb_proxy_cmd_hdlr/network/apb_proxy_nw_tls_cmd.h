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

#ifndef __APB_PROXY_NW_TLS_CMD_H__
#define __APB_PROXY_NW_TLS_CMD_H__


#include "apb_proxy.h"

//#define TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
#define TLS_AT_CMD_SUPPORT_DATA_MODE
#endif

//#define TLS_AT_CMD_UT

/* Define 0 to disable logging, define 1 to enable logging. */
#if defined(__MTK_NBIOT_SLIM__)
#define TLS_AT_CMD_DEBUG 0
#else
#ifndef MTK_DEBUG_LEVEL_NONE
#define TLS_AT_CMD_DEBUG 1
#else
#define TLS_AT_CMD_DEBUG 0
#endif
#endif

/* The following content is used inside the TLS_AT_CMD module. */
#if TLS_AT_CMD_DEBUG
/* Dump the received data if this macro is defined. */
#define TLS_AT_CMD_LOG_DUMP_MEM

#define TLS_AT_CMD_LOG_INFO(x, ...)    do {LOG_I(apb_tls, x, ##__VA_ARGS__);} while(0)
#define TLS_AT_CMD_LOG_WARN(x, ...)    do {LOG_W(apb_tls, x, ##__VA_ARGS__);} while(0)
#define TLS_AT_CMD_LOG_ERR(x, ...)    do {LOG_E(apb_tls, x, ##__VA_ARGS__);} while(0)
#else
#define TLS_AT_CMD_LOG_INFO(x, ...)
#define TLS_AT_CMD_LOG_WARN(x, ...)
#define TLS_AT_CMD_LOG_ERR(x, ...)
#endif


typedef enum
{
    APB_PROXY_TLS_RET_CONNECT = 1,
    APB_PROXY_TLS_RET_OK = 0,
    APB_PROXY_TLS_RET_ERROR = -1,
    APB_PROXY_TLS_RET_INVALID_FORMAT = -2,
    APB_PROXY_TLS_RET_INVALID_PARAM = -3,
    APB_PROXY_TLS_RET_INSUFFICIENT_MEMORY = -4,
    APB_PROXY_TLS_RET_WRONG_STATE = -5,
    APB_PROXY_TLS_RET_NOT_FOUND = -6,

}apb_proxy_tls_ret_enum;


apb_proxy_status_t apb_proxy_hdlr_etlscfg_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd);

apb_proxy_status_t apb_proxy_hdlr_etlsconn_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd);

apb_proxy_status_t apb_proxy_hdlr_etlsclose_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd);

apb_proxy_status_t apb_proxy_hdlr_etlssend_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd);

apb_proxy_status_t apb_proxy_hdlr_etlsrecv_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd);

#endif /* __APB_PROXY_NW_TLS_CMD_H__ */

