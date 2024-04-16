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

/* AP Bridge packet decoder in AP Bridge Proxy.*/

#ifndef __APB_PROXY_PACKET_DECODER__H__
#define __APB_PROXY_PACKET_DECODER__H__

#include "stdint.h"
#include "stdbool.h"
#include "apb_proxy.h"
#include "apb_proxy_data_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************
 * @brief      Get the AP Bridge packet type.
 * @param[in]  p_apb_proxy_packet points to AP Bridge packet data buffer.
 * @return     Return the AP Bridge packet type.
 ******************************************************************/
uint32_t apb_proxy_get_packet_type(apb_proxy_packet_t *p_apb_proxy_packet);
/******************************************************************
 * @brief      Get channel ID.
 * @param[in]  p_apb_proxy_packet points to AP Bridge packet data buffer.
 * @return     Return the channel ID.
 ******************************************************************/
uint32_t apb_proxy_get_channel_id(apb_proxy_packet_t *p_apb_proxy_packet);
/*******************************************************************
 * @brief         Decode the AT Command request packet into structure.
 * @param[in]     p_apb_proxy_packet : points to AP Bridge packet data buffer.
 * @param[in/out] pCmdReq : points to the decoded result.
 * @return        APB_PROXY_STATUS_OK  : decode successfully, APB_PROXY_STATUS_ERROR  : decode failed.
 *******************************************************************/
apb_proxy_status_t apb_proxy_decode_at_cmd_req_msg(apb_proxy_packet_t *p_apb_proxy_packet,
                                                   apb_proxy_parse_cmd_param_t *p_cmd_req);
/*****************************************************************************
 * @brief         Decode the AT Command register result packet into structure.
 * @param[in]     p_apb_proxy_packet : points to AP Bridge packet data buffer.
 * @param[in/out] p_register_result : points to the decoded result.
 * @return        APB_PROXY_STATUS_OK  : decode successfully, APB_PROXY_STATUS_ERROR  : decode failed.
 ******************************************************************************/
apb_proxy_status_t apb_proxy_decode_at_cmd_reg_result_msg(apb_proxy_packet_t *p_apb_proxy_packet,
                                                          apb_proxy_register_result_t *p_register_result);
/******************************************************************************
 * @brief         Decode the user data packet into structure.
 * @param[in]     p_apb_proxy_packet : points to AP Bridge packet data buffer.
 * @param[in/out] p_userdata : points to the decoded result.
 * @return        APB_PROXY_STATUS_OK  : decode successfully, APB_PROXY_STATUS_ERROR  : decode failed.
 *****************************************************************************/
apb_proxy_status_t apb_proxy_decode_userdata_msg(apb_proxy_packet_t *p_apb_proxy_packet,
                                                 apb_proxy_user_data_t *p_userdata);
/******************************************************************************
 * @brief         Decode the data mode closed indication packet into structure.
 * @param[in]     p_apb_proxy_packet : points to AP Bridge packet data buffer.
 * @param[in/out] p_userdata : points to the decoded result.
 * @return        APB_PROXY_STATUS_OK  : decode successfully, APB_PROXY_STATUS_ERROR  : decode failed.
 *****************************************************************************/
apb_proxy_status_t apb_proxy_decode_data_mode_closed_ind_msg(apb_proxy_packet_t *p_apb_proxy_packet,
                                                             uint32_t* channel_id);
/******************************************************************************
 * @brief         Decode the data mode deactive indication packet into structure.
 * @param[in]     p_apb_proxy_packet : points to AP Bridge packet data buffer.
 * @param[in/out] p_userdata : points to the decoded result.
 * @return        APB_PROXY_STATUS_OK  : decode successfully, APB_PROXY_STATUS_ERROR  : decode failed.
 *****************************************************************************/
apb_proxy_status_t apb_proxy_decode_data_mode_temp_deactive_ind_msg(apb_proxy_packet_t *p_apb_proxy_packet,
                                                                    uint32_t* channel_id);
/******************************************************************************
 * @brief         Decode the data mode resumed indication packet into structure.
 * @param[in]     p_apb_proxy_packet : points to AP Bridge packet data buffer.
 * @param[in/out] p_userdata : points to the decoded result.
 * @return        APB_PROXY_STATUS_OK  : decode successfully, APB_PROXY_STATUS_ERROR  : decode failed.
 *****************************************************************************/
apb_proxy_status_t apb_proxy_decode_data_mode_resumed_ind_msg(apb_proxy_packet_t *p_apb_proxy_packet,
                                                              uint32_t* channel_id);

#ifdef __cplusplus
}
#endif

#endif/*__APB_PROXY_PACKET_DECODER__H__*/

