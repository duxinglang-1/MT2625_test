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

#ifndef __APB_PROXY_QUEUE__H__
#define __APB_PROXY_QUEUE__H__

#include "stdint.h"
#include "stdbool.h"
#include "apb_proxy.h"

#ifdef __cplusplus
extern "C" {
#endif

/*AP Bridge internal used message queue handle.*/
typedef void *apb_proxy_queue_handle_t;

/*************************************************************************
 * @brief     Create AP Bridge internal message queue.
 * @param[in] item_size: the size of every message.
 * @param[in] queue_size: the max message that the queue can hold.
 * @return    When the queue is successfully created, a none-NULL handle
 *            will be returned.
 **************************************************************************/
apb_proxy_queue_handle_t apb_proxy_queue_create(uint8_t item_size, uint8_t queue_size);
/**************************************************************************
 * @brief      Delete an existed message queue.
 * @param[in]  the created message queue handle.
 * @return     None
 **************************************************************************/
void apb_proxy_queue_delete(apb_proxy_queue_handle_t apb_proxy_queue);
/*************************************************************************
 * @brief      Get how many messages can be saved into current queue.
 * @param[in]  the created message queue handle.
 * @return     Return how many messages can be saved into current queue.
 *************************************************************************/
uint32_t apb_proxy_queue_get_available_space(apb_proxy_queue_handle_t apb_proxy_queue);
/*****************************************************************************
 * @brief      Get how many messages have been saved into current queue.
 * @param[in]  the created message queue handle.
 * @return     Return how many messages are saved in current queue.
 ******************************************************************************/
uint32_t apb_proxy_queue_get_occupied_space(apb_proxy_queue_handle_t apb_proxy_queue);
/******************************************************************************
 * @brief      Get current queue's capacity.
 * @param[in]  the created message queue handle.
 * @return     Return current queue's capacity.
 *****************************************************************************/
uint32_t apb_proxy_queue_get_capacity(apb_proxy_queue_handle_t apb_proxy_queue);
/***********************************************************************************
 * @brief      Push messages into queue's tail.
 * @param[in]  apb_proxy_queue : the created message queue handle.
 * @param[in]  pmsg : message data's address
 * @return     APB_PROXY_STATUS_OK : push successfully, APB_PROXY_STATUS_ERROR : failed to push the data to queue.
 ***********************************************************************************/
apb_proxy_status_t apb_proxy_queue_push_msg(apb_proxy_queue_handle_t apb_proxy_queue, void *pmsg);
/*************************************************************************************
 * @brief          Pop messages from queue's front end.
 * @param[in]      queueHandle : the created message queue handle.
 * @param[in/out]  pmsg : message data's address
 * @return         APB_PROXY_STATUS_OK: pop successfully, APB_PROXY_STATUS_ERROR: failed to get message data from queue.
 *************************************************************************************/
apb_proxy_status_t apb_proxy_queue_pop_msg(apb_proxy_queue_handle_t apb_proxy_queue, void *pmsg);
/*************************************************************************************
 * @brief      Push messages into queue's front end.
 * @param[in]  apb_proxy_queue : the created message queue handle.
 * @param[in]  pmsg : message data's address
 * @return     APB_PROXY_STATUS_OK: push successfully, APB_PROXY_STATUS_ERROR: failed to save the message into queue.
 ***************************************************************************************/
apb_proxy_status_t apb_proxy_queue_push_msg_to_front(apb_proxy_queue_handle_t apb_proxy_queue, void *pmsg);

#ifdef __cplusplus
}
#endif

#endif/*__APB_PROXY_QUEUE__H__*/

