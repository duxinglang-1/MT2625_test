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

#include "apb_proxy_at_cmd_tbl.h"
#include "apb_proxy_test.h"
#include "apb_proxy_ril_ut_cmd.h"
#ifdef MTK_ATCI_APB_PROXY_NETWORK_ENABLE
#include "apb_proxy_nw_cmd.h"
#include "apb_proxy_nw_socket_cmd.h"
#include "apb_proxy_nw_lwm2m_cmd.h"
#include "apb_proxy_nw_mqtt_cmd.h"
#include "apb_proxy_nw_sntp_cmd.h"
#include "apb_proxy_nw_httpclient_cmd.h"
#include "apb_proxy_nw_onenet_cmd.h"
#include "apb_proxy_nw_dm_cmd.h"
#include "apb_proxy_nw_tmo_cmd.h"
#include "apb_proxy_nw_ctm2m_cmd.h"
#include "apb_proxy_nw_ctiot_cmd.h"
#ifdef MTK_COAP_SUPPORT
#include "apb_proxy_nw_coap_cmd.h"
#endif
#ifndef MTK_MBED_TLS_DISABLE
#include "apb_proxy_nw_tls_cmd.h"
#endif
#endif
#ifdef ATCI_APB_PROXY_ADAPTER_ENABLE
#include "apb_proxy_atci_adapter_cmd.h"
#endif
#ifdef APB_PROXY_PROJECT_COMMAND_ENABLE
#include "apb_proxy_cmd_handler.h"
#endif
#ifdef APB_PROXY_FOTA_CMD_ENABLE
#include "apb_proxy_fota_cmd.h"
#endif
#ifdef MTK_SOCKET_PROXY_ENABLE
#include "soc_proxy_cmd.h"
#endif
#if defined(__NB_IOT_GSM_DMDS__) || defined(__NB_IOT_GSM_DMSS__) || defined(__NB_IOT_GSM_DMSS_ENH__)
#include "apb_proxy_modem_switch_cmd.h"
#endif
#include "FreeRTOS.h"

/*Please let the cmd_id field as zero, AP Brige will generate the command id.*/
static apb_proxy_cmd_hdlr_item_t g_apb_proxy_at_cmd_handler_table[] = {
#ifdef APB_PROXY_PROJECT_COMMAND_ENABLE
    #include "apb_proxy_cmd_def.h"
#endif/*APB_PROXY_PROJECT_COMMAND_ENABLE*/
#ifdef ENABLE_APB_PROXY_UNIT_TEST
    #include "apb_proxy_test_at_cmd_def.h"
#else
    #include "apb_proxy_test_at_cmd_def.h"
    /*adding more AT command's info here.*/
    #include "apb_proxy_ril_ut_cmd_def.h"
#ifdef MTK_ATCI_APB_PROXY_NETWORK_ENABLE
    #include "apb_proxy_nw_cmd_def.h"
#endif
#ifdef ATCI_APB_PROXY_ADAPTER_ENABLE
    #include "apb_proxy_atci_adapter_cmd_def.h"
#endif
#endif
#ifdef APB_PROXY_FOTA_CMD_ENABLE
    #include "apb_proxy_fota_cmd_def.h"
#endif
#ifdef MTK_SOCKET_PROXY_ENABLE
    #include "soc_proxy_cmd_def.h"
#endif
#if defined(__NB_IOT_GSM_DMDS__) || defined(__NB_IOT_GSM_DMSS__) || defined(__NB_IOT_GSM_DMSS_ENH__)
    #include "apb_proxy_modem_switch_cmd_def.h"
#endif
    {NULL, NULL, APB_PROXY_INVALID_CMD_ID}
};

static apb_proxy_cmd_hdlr_table_t g_apb_proxy_cmd_handler_table_config = {
    g_apb_proxy_at_cmd_handler_table,
    (sizeof(g_apb_proxy_at_cmd_handler_table) / sizeof(apb_proxy_cmd_hdlr_item_t)) - 1
};

/**********************************************************
* @brief     get the AT command handler table info.
* @param[in] None
* @return    the pointer which points to the AT command handler table info.
*            The returned pointer is always not NULL.
**********************************************************/
apb_proxy_cmd_hdlr_table_t *apb_proxy_get_cmd_hdlr_table_info(void)
{
    return &g_apb_proxy_cmd_handler_table_config;
}


