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

#ifndef __APB_PROXY_MSG_QUEUE_DEF__H__
#define __APB_PROXY_MSG_QUEUE_DEF__H__

#include "apb_proxy_queue.h"
#include "FreeRTOS.h"
#include "queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************
 *      All Message Queue in AP Bridge Proxy        **
 *****************************************************/
#ifndef APB_PROXY_RESERVE_QUEUE_SIZE
#define APB_PROXY_RESERVE_QUEUE_SIZE 4U
#endif

#ifndef APB_PROXY_EXT_QUEUE_WATER_LEVEL_SIZE
#define APB_PROXY_EXT_QUEUE_WATER_LEVEL_SIZE 32
#endif

#ifndef APB_PROXY_USER_DATA_QUEUE_CONTROL_SPACE_SIZE
#define APB_PROXY_USER_DATA_QUEUE_CONTROL_SPACE_SIZE (24U)
#endif

/*************************************************************
 ** The internal low priority message queue.                 *
 ** Dedicated for user data case.                            *
 ** The queue is created only when data connection is set up.*
 ** When data connection is switched off,                    *
 ** user data queue will be deleted.                         *
 **************************************************************/
/*Messages which will be sent to modem/MUX.*/
#ifndef APB_PROXY_USER_DATA_TX_QUEUE_SIZE
#define APB_PROXY_USER_DATA_TX_QUEUE_SIZE (64U)
#endif

#ifndef APB_PROXY_USER_DATA_RX_QUEUE_SIZE
/*Messages which are received from modem/MUX.*/
#define APB_PROXY_USER_DATA_RX_QUEUE_SIZE (64U)
#endif

/*****************************************************
 **  External Message Queue Declare ******************
 *****************************************************/
extern QueueHandle_t g_apb_proxy_external_rx_queue;
extern QueueHandle_t g_apb_proxy_cmd_queue;
/****************************************************
 ** Internal Message Queue Declare ******************
 ****************************************************/
extern apb_proxy_queue_handle_t g_apb_proxy_internal_hi_tx_queue;
extern apb_proxy_queue_handle_t g_apb_proxy_internal_hi_rx_queue;
extern apb_proxy_queue_handle_t g_apb_proxy_internal_mi_tx_queue;
extern apb_proxy_queue_handle_t g_apb_proxy_internal_mi_rx_queue;

/****************************************************
 ** Public Function Prototype
 ****************************************************/

/********************************************************************
 * @brief     Create needed message queues in AP Bridge Proxy .
 *  Note:     The function can only be called once.
 * @param[in] None
 * @return    None
 ********************************************************************/
void apb_proxy_msg_queue_init(void);
/******************************************************************************
 * @brief     Create user data message queue in AP Bridge Proxy .
 *   Note:    The function can only be called when the data mode is switched on.
 * @param[in] The specific channel for creating data mode.
 * @return    APB_PROXY_STATUS_OK : successfully created user data queue.
 ********************************************************************************/
apb_proxy_status_t apb_proxy_create_user_data_queue(uint32_t channel_id);
/************************************************************************************
 * @brief     Create user data message queue in AP Bridge Proxy .
 *   Note:    The function can only be called when the data mode is switched off.
 *            All the left messages in the queue will be dropped.
 * @param[in] The specific channel for the data mode.
 * @return    None
 ************************************************************************************/
void apb_proxy_delete_user_data_queue(uint32_t channel_id);
/************************************************************************************
 * @brief         Get an event from message queue.
 * @param[in/out] pout_msg : the event from message queue.
 * @return        APB_PROXY_STATUS_OK : get an event, APB_PROXY_STATUS_ERROR: the internal message queue is empty.
 ***********************************************************************************/
apb_proxy_status_t apb_proxy_get_event_from_internal_msg_queue(void *pout_msg);

#ifdef __cplusplus
}
#endif

#endif/*__APB_PROXY_MSG_QUEUE_DEF__H__*/

