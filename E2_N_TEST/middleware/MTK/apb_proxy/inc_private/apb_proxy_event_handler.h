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

#ifndef __APB_PROXY_EVENT_HANDLER__H__
#define __APB_PROXY_EVENT_HANDLER__H__


/****************************************************
 *           AP Bridge Event Handler                *
 ****************************************************/

#include "stdint.h"
#include "stdbool.h"
#include "apb_proxy_data_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************
**     AP Bridge launches from AP Bridge Proxy     **
****************************************************/
/*********************************************************
 * @brief      AP Bridge Proxy register AT command event handler.
 * @param[in]  None
 * @return     None
 *********************************************************/
void apb_proxy_register_at_cmd_req_handler(void);
/****************************************************
 **         AP Bridge Events come from APP         **
 ****************************************************/
/****************************************************************
 * @brief      APP executed AT command result event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ****************************************************************/
void apb_proxy_ap_at_cmd_result_ind_handler(apb_proxy_event_t *p_apb_proxy_event);
/*****************************************************************
 * @brief      APP requests to send user data event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 *****************************************************************/
void apb_proxy_ap_userdata_handler(apb_proxy_event_t *p_apb_proxy_event);
/*****************************************************************
 * @brief      APP requests to set up data mode event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 *****************************************************************/
void apb_proxy_create_data_mode_handler(apb_proxy_event_t *p_apb_proxy_event);
/*****************************************************************
 * @brief      APP requests to close data mode event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 *****************************************************************/
void apb_proxy_close_data_mode_handler(apb_proxy_event_t *p_apb_proxy_event);
/*********************************************************************
 * @brief      APP request to resume sending user data event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ********************************************************************/
void apb_proxy_ap_xon_req_handler(apb_proxy_event_t *p_apb_proxy_event);
/***********************************************************************
 * @brief      APP request to stop sending user data event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ************************************************************************/
void apb_proxy_ap_xoff_req_handler(apb_proxy_event_t *p_apb_proxy_event);
/***********************************************************************
 * @brief      APP request to resume data mode.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ************************************************************************/
void apb_proxy_ap_resume_data_mode_req_handler(apb_proxy_event_t *p_apb_proxy_event);
/****************************************************
**    AP Bridge Events come from Modem             **
*****************************************************/
/*************************************************************************
 * @brief      Modem informs the AT command register result event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 **************************************************************************/
void apb_proxy_md_register_at_cmd_rsp_handler(apb_proxy_event_t *p_apb_proxy_event);
/****************************************************************************
 * @brief      Modem sends the AT command execution request event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ****************************************************************************/
void apb_proxy_md_at_cmd_req_handler(apb_proxy_event_t *p_apb_proxy_event);
/****************************************************************************
 * @brief      Modem sends user data to App event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ***************************************************************************/
void apb_proxy_md_userdata_ind_handler(apb_proxy_event_t *p_apb_proxy_event);
/****************************************************************************
 * @brief      Modem sends data mode closed indicationi event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ***************************************************************************/
void apb_proxy_md_data_mode_closed_ind_handler(apb_proxy_event_t *p_apb_proxy_event);
/***************************************************************************
 * @brief      Modem request to resume sending user data event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ****************************************************************************/
void apb_proxy_md_xon_req_handler(apb_proxy_event_t *p_apb_proxy_event);
/*****************************************************************************
 * @brief      Modem request to stop sending user data event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 */
void apb_proxy_md_xoff_req_handler(apb_proxy_event_t *p_apb_proxy_event);
/****************************************************************************
 * @brief      Modem sends data mode temp deactived event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ***************************************************************************/
void apb_proxy_md_data_mode_temp_deactived_ind_handler(apb_proxy_event_t *p_apb_proxy_event);
/****************************************************************************
 * @brief      Modem sends data mode resumed indication event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ***************************************************************************/
void apb_proxy_md_data_mode_resumed_ind_handler(apb_proxy_event_t *p_apb_proxy_event);
/*****************************************************
 **              User Data Flow Control Processor   **
 *****************************************************/
/**************************************************************************
 * @brief     AP Bridge user data flow control processor.
 * @param[in] None
 * @return    None
 **************************************************************************/
void apb_proxy_userdata_flow_control_processor(void);

void apb_proxy_send_packet_to_modem(apb_proxy_packet_t *p_apb_packet);
#ifdef __cplusplus
}
#endif

#endif/*__APB_PROXY_EVENT_HANDLER__H__*/

