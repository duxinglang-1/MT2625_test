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

#ifndef __APB_PROXY_PACKET_ENCODER__H__
#define __APB_PROXY_PACKET_ENCODER__H__


#include "stdint.h"
#include "stdbool.h"
#include "apb_proxy.h"
#include "apb_proxy_data_type.h"
#include "apb_proxy_at_cmd_tbl.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************
 *****************************************************
 ** AP Bridge packet encoder in AP Bridge Proxy.******
 *****************************************************
 *****************************************************/

/**************************************************************************
 * @brief         Encode AT command register message into AP Bridge packet.
 * @param[in]     p_cmd_item_tbl : points to AT command handler table.
 * @param[in]     item_count : AT command item count.
 * @param[in/out] p_packet_data : encoded AP Bridge packet.
 * @return        how many at cmd's names are been encoded into the packet.
 **************************************************************************/
uint32_t apb_proxy_encode_at_cmd_reg_packet(apb_proxy_cmd_hdlr_item_t *p_cmd_item_tbl,
                                            uint32_t item_count, apb_proxy_packet_t *p_packet_data);
/*************************************************************************
 * @brief         Encode AT command results into AP Bridge packet.
 * @param[in]     presult : points to AT command results.
 * @param[in/out] p_packet_data : encoded AP Bridge packet.
 * @return        APB_PROXY_STATUS_OK : encoded successfully, APB_PROXY_STATUS_ERROR : encoded failed.
 *************************************************************************/
apb_proxy_status_t apb_proxy_encode_at_cmd_result_packet(apb_proxy_at_cmd_result_t *presult,
                                                         apb_proxy_packet_t *p_packet_data);
/*************************************************************************
 * @brief         Encode user data into AP Bridge packet.
 * @param[in]     puser_data : points to user data structure.
 * @param[in]     channel_id : channel ID.
 * @param[in/out] p_packet_data : encoded AP Bridge packet.
 * @return        APB_PROXY_STATUS_OK : encoded successfully, APB_PROXY_STATUS_ERROR : encoded failed.
 *************************************************************************/
apb_proxy_status_t apb_proxy_encode_user_data_packet(apb_proxy_user_data_t *puser_data,
                                                     uint32_t channel_id,
                                                     apb_proxy_packet_t *p_packet_data);
/*************************************************************************
 * @brief         Encode Xon AP Bridge packet.
 * @param[in]     channel_id    : channel ID
 * @param[in/out] p_packet_data : encoded AP Bridge packet.
 * @return        APB_PROXY_STATUS_OK : encoded successfully, APB_PROXY_STATUS_ERROR : encoded failed.
 *************************************************************************/
apb_proxy_status_t apb_proxy_encode_xon_packet(uint32_t channel_id, apb_proxy_packet_t *p_packet_data);
/*************************************************************************
 * @brief          Encode Xoff AP Bridge packet.
 * @param[in]      channel_id    : channel ID
 * @param[in/out]  p_packet_data : encoded AP Bridge packet.
 * @return         APB_PROXY_STATUS_OK : encoded successfully, APB_PROXY_STATUS_ERROR : encoded failed.
 *************************************************************************/
apb_proxy_status_t apb_proxy_encode_xoff_packet(uint32_t channel_id, apb_proxy_packet_t *p_packet_data);
/*************************************************************************
 * @brief          Encode close data mode request AP Bridge packet.
 * @param[in]      channel_id    : channel ID
 * @param[in/out]  p_packet_data : encoded AP Bridge packet.
 * @return         APB_PROXY_STATUS_OK : encoded successfully, APB_PROXY_STATUS_ERROR : encoded failed.
 *************************************************************************/
apb_proxy_status_t apb_proxy_encode_close_data_mode_req_packet(uint32_t channel_id, apb_proxy_packet_t *p_packet_data);
/*************************************************************************
 * @brief          Encode resume data mode request AP Bridge packet.
 * @param[in]      channel_id    : channel ID
 * @param[in/out]  p_packet_data : encoded AP Bridge packet.
 * @return         APB_PROXY_STATUS_OK : encoded successfully, APB_PROXY_STATUS_ERROR : encoded failed.
 *************************************************************************/
apb_proxy_status_t apb_proxy_encode_resume_data_mode_req_packet(uint32_t channel_id, apb_proxy_packet_t *p_packet_data);
/*************************************************************************
 * @brief          update the channel Id in AP Bridge packet.
 * @param[in]      channel_id : the channel id for the AT command or user data
 * @param[in/out]  p_packet_data : encoded AP Bridge packet.
 * @return         APB_PROXY_STATUS_OK : encoded successfully, APB_PROXY_STATUS_ERROR : encoded failed.
 **************************************************************************/
apb_proxy_status_t apb_proxy_fill_channel_id_to_packet(apb_proxy_packet_t *p_packet_data, uint32_t channel_id);
/**************************************************************************
 * @brief          read AT command result code from AP Bridge packet.
 * @param[in/out]  p_packet_data : AP Bridge packet.
 * @return         The AT command's result code.
 **************************************************************************/
uint32_t apb_proxy_get_at_cmd_result_code(apb_proxy_packet_t *p_packet_data);

#ifdef __cplusplus
}
#endif

#endif/*__APB_PROXY_PACKET_ENCODER__H__*/

