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

#include "apb_proxy_msg_queue_def.h"
#include "apb_proxy_data_type.h"
#include "apb_proxy_context_manager.h"
#include "FreeRTOS.h"
#include "apb_proxy_log.h"


/****************************************************
 **  External Message Queue   ***********************
 ****************************************************/

QueueHandle_t g_apb_proxy_cmd_queue = NULL;

#ifndef APB_PROXY_CMD_QUEUE_MAX_SIZE
#define APB_PROXY_CMD_QUEUE_MAX_SIZE (8)
#endif

/* The FressRTOS queue is used to receive all the external task's event/signal .*/
QueueHandle_t g_apb_proxy_external_rx_queue = NULL;

#ifndef APB_PROXY_RX_QUEUE_MAX_SIZE
#define APB_PROXY_RX_QUEUE_MAX_SIZE (64U + APB_PROXY_RESERVE_QUEUE_SIZE)
#endif

/****************************************************
 **            Internal Message Queue              **
 ****************************************************/

/****************************************************
 ** The internal high priority message queue.       *
 ** Decicated for control message.                  *
 ****************************************************/
/*Messages which will be sent to modem/MUX.*/
apb_proxy_queue_handle_t g_apb_proxy_internal_hi_tx_queue = NULL;
#define APB_PROXY_HIGH_PRI_TX_QUEUE_SIZE (16U + APB_PROXY_RESERVE_QUEUE_SIZE)

/*Messages which are received from modem/MUX.*/
apb_proxy_queue_handle_t g_apb_proxy_internal_hi_rx_queue = NULL;
#define APB_PROXY_HIGH_PRI_RX_QUEUE_SIZE (16U + APB_PROXY_RESERVE_QUEUE_SIZE)
/*****************************************************
 ** The internal middle priority message queue.      *
 ** Dedicated for AT command related message.        *
 *****************************************************/
/*Messages which will be sent to modem/MUX.*/
apb_proxy_queue_handle_t g_apb_proxy_internal_mi_tx_queue = NULL;
#define APB_PROXY_MID_PRI_TX_QUEUE_SIZE (32U + APB_PROXY_RESERVE_QUEUE_SIZE)

/*Messages which are received from modem/MUX.*/
apb_proxy_queue_handle_t g_apb_proxy_internal_mi_rx_queue = NULL;
#define APB_PROXY_MID_PRI_RX_QUEUE_SIZE (4U + APB_PROXY_RESERVE_QUEUE_SIZE)

/****************************************************
 ** Local Function prototype.                       *
 ****************************************************/
static apb_proxy_status_t apb_proxy_get_event_from_tx_queue(void *pout_msg);
static apb_proxy_status_t apb_proxy_get_event_from_rx_queue(void *pout_msg);
/****************************************************
 ** Public Function Implementation.                **
 ****************************************************/
/********************************************************************
 * @brief     Create needed message queues in AP Bridge Proxy .
 *  Note:     The function can only be called once.
 * @param[in] None
 * @return    None
 ********************************************************************/
void apb_proxy_msg_queue_init(void)
{
    g_apb_proxy_external_rx_queue = xQueueCreate(APB_PROXY_RX_QUEUE_MAX_SIZE, sizeof(apb_proxy_event_t) );
    if (NULL == g_apb_proxy_external_rx_queue) {
        configASSERT(0);
    }

    g_apb_proxy_cmd_queue = xQueueCreate(APB_PROXY_CMD_QUEUE_MAX_SIZE, sizeof(apb_proxy_exe_cmd_t) );
    if (NULL == g_apb_proxy_cmd_queue) {
        configASSERT(0);
    }

    /* create internal used message queue.*/
    g_apb_proxy_internal_hi_tx_queue = apb_proxy_queue_create(sizeof(apb_proxy_event_t), APB_PROXY_HIGH_PRI_TX_QUEUE_SIZE);
    if (NULL == g_apb_proxy_internal_hi_tx_queue) {
        configASSERT(0);
    }

    g_apb_proxy_internal_hi_rx_queue = apb_proxy_queue_create(sizeof(apb_proxy_event_t), APB_PROXY_HIGH_PRI_RX_QUEUE_SIZE);
    if (NULL == g_apb_proxy_internal_hi_rx_queue) {
        configASSERT(0);
    }

    g_apb_proxy_internal_mi_tx_queue = apb_proxy_queue_create(sizeof(apb_proxy_event_t), APB_PROXY_MID_PRI_TX_QUEUE_SIZE);
    if (NULL == g_apb_proxy_internal_mi_tx_queue) {
        configASSERT(0);
    }

    g_apb_proxy_internal_mi_rx_queue = apb_proxy_queue_create(sizeof(apb_proxy_event_t), APB_PROXY_MID_PRI_RX_QUEUE_SIZE);
    if (NULL == g_apb_proxy_internal_mi_rx_queue) {
        configASSERT(0);
    }
}

/******************************************************************************
 * @brief     Create user data message queue in AP Bridge Proxy .
 *   Note:    The function can only be called when the data mode is switched on.
 * @param[in] The specific channel for creating data mode.
 * @return    APB_PROXY_STATUS_OK : successfully created user data queue.
 ********************************************************************************/
apb_proxy_status_t apb_proxy_create_user_data_queue(uint32_t channel_id)
{
    apb_proxy_data_mode_t* data_mode_context = apb_proxy_get_data_mode_context(channel_id);
    if (data_mode_context == NULL) {
        return APB_PROXY_STATUS_ERROR;
    }
    if ((data_mode_context->data_tx_queue_from_ap != NULL) || (data_mode_context->data_rx_queue_from_md != NULL)) {
        apb_proxy_log_error("user data queue is not deleted.\r\n");
        return APB_PROXY_STATUS_ERROR;
    }

    data_mode_context->data_tx_queue_from_ap = apb_proxy_queue_create(sizeof(apb_proxy_event_t), APB_PROXY_USER_DATA_TX_QUEUE_SIZE);
    if (NULL == data_mode_context->data_tx_queue_from_ap) {
        return APB_PROXY_STATUS_ERROR;
    }

    data_mode_context->data_rx_queue_from_md = apb_proxy_queue_create(sizeof(apb_proxy_event_t), APB_PROXY_USER_DATA_RX_QUEUE_SIZE);
    if (NULL == data_mode_context->data_rx_queue_from_md) {
        /*Delete allocated TX queue.*/
        apb_proxy_queue_delete(data_mode_context->data_tx_queue_from_ap);
        data_mode_context->data_tx_queue_from_ap = NULL;
        return APB_PROXY_STATUS_ERROR;
    }
    return APB_PROXY_STATUS_OK;
}
/************************************************************************************
 * @brief     Create user data message queue in AP Bridge Proxy .
 *   Note:    The function can only be called when the data mode is switched off.
 *            All the left messages in the queue will be dropped.
 * @param[in] The specific channel for the data mode.
 * @return    None
 ************************************************************************************/
void apb_proxy_delete_user_data_queue(uint32_t channel_id)
{
    
    apb_proxy_data_mode_t* data_mode_context = apb_proxy_get_data_mode_context(channel_id);
    if (data_mode_context == NULL) {
        return;
    }

    if (NULL == data_mode_context->data_tx_queue_from_ap) {
        apb_proxy_log_warning("user tx queue was del");
        return;
    }
    apb_proxy_queue_delete(data_mode_context->data_tx_queue_from_ap);
    data_mode_context->data_tx_queue_from_ap = NULL;

    if (NULL == data_mode_context->data_rx_queue_from_md) {
        apb_proxy_log_warning("user rx was del");
        return;
    }
    apb_proxy_queue_delete(data_mode_context->data_rx_queue_from_md);
    data_mode_context->data_rx_queue_from_md = NULL;
}
/************************************************************************************
 * @brief         Get an event from message queue.
 * @param[in/out] pout_msg : the event from message queue.
 * @return        APB_PROXY_STATUS_OK: get an event, APB_PROXY_STATUS_ERROR: the internal message queue is empty.
 ***********************************************************************************/
apb_proxy_status_t apb_proxy_get_event_from_internal_msg_queue(void *pout_msg)

{
    configASSERT(pout_msg != NULL);
    static bool process_rx_event = true;
    apb_proxy_status_t got_event = APB_PROXY_STATUS_ERROR;

    if (true == process_rx_event) {
        got_event = apb_proxy_get_event_from_rx_queue(pout_msg);
    } else {
        got_event = apb_proxy_get_event_from_tx_queue(pout_msg);
    }

    if (APB_PROXY_STATUS_OK == got_event) {
        process_rx_event = !process_rx_event;
        return APB_PROXY_STATUS_OK;
    } else {
        if (true == process_rx_event) {
            got_event = apb_proxy_get_event_from_tx_queue(pout_msg);
        } else {
            got_event = apb_proxy_get_event_from_rx_queue(pout_msg);
        }
    }
    return got_event;
}

/****************************************************
  ** Local Function Implementation.
  ****************************************************/
static apb_proxy_status_t apb_proxy_get_event_from_tx_queue(void *pout_msg)
{
    configASSERT(pout_msg != NULL);
    apb_proxy_context_t *apb_proxy_context_p = apb_proxy_get_apb_proxy_context_pointer();
    apb_proxy_status_t got_event = APB_PROXY_STATUS_ERROR;

    if (apb_proxy_queue_pop_msg(g_apb_proxy_internal_hi_tx_queue, pout_msg) == APB_PROXY_STATUS_OK) {
        got_event = APB_PROXY_STATUS_OK;
    } else if (apb_proxy_queue_pop_msg(g_apb_proxy_internal_mi_tx_queue, pout_msg) == APB_PROXY_STATUS_OK) {
        got_event = APB_PROXY_STATUS_OK;
    } else {
        apb_proxy_data_mode_t* data_mode_ctx = NULL;
        uint32_t index = 0;
        for(index = 0; index < APB_PROXY_SUPPORT_DATA_MODE_MAX; index ++) {
            data_mode_ctx = apb_proxy_context_p->apb_data_mode_context + index;
            if ((data_mode_ctx->data_mode_state != APB_PROXY_DATA_MODE_DEACTIVATED) &&
                (data_mode_ctx->data_tx_queue_from_ap != NULL)) {
                if (apb_proxy_queue_pop_msg(data_mode_ctx->data_tx_queue_from_ap, pout_msg) == APB_PROXY_STATUS_OK) {
                    got_event = APB_PROXY_STATUS_OK;
                    break;
                }
            }
        }
    }

    return got_event;
}

static apb_proxy_status_t apb_proxy_get_event_from_rx_queue(void *pout_msg)
{
    apb_proxy_status_t got_event = APB_PROXY_STATUS_ERROR;
    apb_proxy_context_t *apb_proxy_context_p = apb_proxy_get_apb_proxy_context_pointer();
    configASSERT(pout_msg != NULL);

    if (apb_proxy_queue_pop_msg(g_apb_proxy_internal_hi_rx_queue, pout_msg) == APB_PROXY_STATUS_OK) {
        got_event = APB_PROXY_STATUS_OK;
    } else if (apb_proxy_queue_pop_msg(g_apb_proxy_internal_mi_rx_queue, pout_msg) == APB_PROXY_STATUS_OK) {
        got_event = APB_PROXY_STATUS_OK;
    } else {
      apb_proxy_data_mode_t* data_mode_ctx = NULL;
      uint32_t index = 0;
      for(index = 0; index < APB_PROXY_SUPPORT_DATA_MODE_MAX; index ++) {
          data_mode_ctx = apb_proxy_context_p->apb_data_mode_context + index;
          if ((data_mode_ctx->data_mode_state != APB_PROXY_DATA_MODE_DEACTIVATED) &&
             (data_mode_ctx->data_rx_queue_from_md != NULL)) {
              if (apb_proxy_queue_pop_msg(data_mode_ctx->data_rx_queue_from_md, pout_msg) == APB_PROXY_STATUS_OK) {
                  apb_proxy_log_info("pop msg from user data queue\r\n");
                  got_event = APB_PROXY_STATUS_OK;
                  break;
              }
          }
      }
    }
    return got_event;
}
