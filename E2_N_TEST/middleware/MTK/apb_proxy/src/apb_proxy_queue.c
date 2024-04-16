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

#include "apb_proxy_queue.h"
#include "apb_proxy_log.h"
#include "FreeRTOS.h"
#include "apb_proxy_log.h"
#include "string.h"


/**************************************************************************
*                  Internal Used Structure Definition                     *
**************************************************************************/

/************************
 ---------------------- *
 |frontIndex          | *
 ---------------------- *
 |tailIndex           | *
 ---------------------- *
 |itemSize            | *
 ---------------------- *
 |queueSize           | *
 ---------------------- *
 |queue data buffer   | *
*************************/

/*The queue will be implemented as circle queue.*/
typedef struct {
    uint8_t  front_index;    /**< point to the front item of the queue*/
    uint8_t  tail_index;     /**< point to the tail item of the queue, the next available position.*/
    uint8_t  item_size;      /**< The memory size of very item for the queue*/
    uint8_t  capacity;       /**< The max counts of the queue item for the queue*/
    uint8_t  occupied_count; /**< The message counts in the queue.*/
    uint8_t *pdata;          /**< The queue item's buffer*/
} apb_proxy_queue_t;

/**************************************************************************
*                Public  Function Implementation                          *
**************************************************************************/

/*************************************************************************
 * @brief     Create AP Bridge internal message queue.
 * @param[in] item_size: the size of every message.
 * @param[in] queue_size: the max message that the queue can hold.
 * @return    When the queue is successfully created, a none-NULL handle
 *            will be returned.
 **************************************************************************/
apb_proxy_queue_handle_t apb_proxy_queue_create(uint8_t item_size, uint8_t queue_size)
{
    apb_proxy_queue_t *queue_handle = NULL;
    configASSERT(item_size != 0);
    configASSERT(queue_size != 0);
    queue_handle = (apb_proxy_queue_t *)pvPortMalloc(sizeof(apb_proxy_queue_t) + (item_size * queue_size));
    if (queue_handle != NULL) {
        queue_handle->front_index = 0U;
        queue_handle->tail_index = 0U;
        queue_handle->item_size = item_size;
        queue_handle->capacity = queue_size;
        queue_handle->occupied_count = 0U;
        queue_handle->pdata = (uint8_t *)queue_handle + sizeof(apb_proxy_queue_t);
    }
    return (apb_proxy_queue_handle_t)queue_handle;
}
/**************************************************************************
 * @brief      Delete an existed message queue.
 * @param[in]  the created message queue handle.
 * @return     None
 **************************************************************************/
void apb_proxy_queue_delete(apb_proxy_queue_handle_t apb_proxy_queue)
{
    configASSERT(apb_proxy_queue != NULL);
    vPortFree((void *)apb_proxy_queue);
}
/*************************************************************************
 * @brief      Get how many messages can be saved into current queue.
 * @param[in]  the created message queue handle.
 * @return     Return how many messages can be saved into current queue.
 *************************************************************************/
uint32_t apb_proxy_queue_get_available_space(apb_proxy_queue_handle_t apb_proxy_queue)
{
    apb_proxy_queue_t *queue_handle = NULL;
    configASSERT(apb_proxy_queue != NULL);
    queue_handle = (apb_proxy_queue_t *)(apb_proxy_queue);
    return queue_handle->capacity - queue_handle->occupied_count;
}
/*****************************************************************************
 * @brief      Get how many messages have been saved into current queue.
 * @param[in]  the created message queue handle.
 * @return     Return how many messages are saved in current queue.
 ******************************************************************************/
uint32_t apb_proxy_queue_get_occupied_space(apb_proxy_queue_handle_t apb_proxy_queue)
{
    apb_proxy_queue_t *queue_handle = NULL;
    configASSERT(apb_proxy_queue != NULL);
    queue_handle = (apb_proxy_queue_t *)(apb_proxy_queue);
    return queue_handle->occupied_count;
}
/******************************************************************************
 * @brief      Get current queue's capacity.
 * @param[in]  the created message queue handle.
 * @return     Return current queue's capacity.
 *****************************************************************************/
uint32_t apb_proxy_queue_get_capacity(apb_proxy_queue_handle_t apb_proxy_queue)
{
    apb_proxy_queue_t *queue_handle = NULL;
    configASSERT(apb_proxy_queue != NULL);
    queue_handle = (apb_proxy_queue_t *)(apb_proxy_queue);
    return queue_handle->capacity;
}
/***********************************************************************************
 * @brief      Push messages into queue's tail.
 * @param[in]  apb_proxy_queue : the created message queue handle.
 * @param[in]  pmsg : message data's address
 * @return     APB_PROXY_STATUS_OK: push successfully, APB_PROXY_STATUS_ERROR: failed to push the data to queue.
 ***********************************************************************************/
apb_proxy_status_t apb_proxy_queue_push_msg(apb_proxy_queue_handle_t apb_proxy_queue, void *pmsg)
{
    apb_proxy_queue_t *queue_handle = NULL;
    uint8_t *pdst_Address = NULL;
    configASSERT(apb_proxy_queue != NULL);
    configASSERT(pmsg != NULL);

    queue_handle = (apb_proxy_queue_t *)(apb_proxy_queue);
    if (queue_handle->occupied_count == queue_handle->capacity) {
        apb_proxy_log_error("msg queue:: full");
        return APB_PROXY_STATUS_ERROR;
    }
    if (0U == queue_handle->occupied_count) {
        queue_handle->front_index = 0;
        queue_handle->tail_index = 0;;
    }

    pdst_Address = queue_handle->pdata + (queue_handle->tail_index * queue_handle->item_size);
    memcpy(pdst_Address, pmsg, queue_handle->item_size);
    /*move to next available space.*/
    queue_handle->tail_index = (queue_handle->tail_index + 1) % (queue_handle->capacity);
    queue_handle->occupied_count++;
    return APB_PROXY_STATUS_OK;
}
/*************************************************************************************
 * @brief          Pop messages from queue's front end.
 * @param[in]      queueHandle : the created message queue handle.
 * @param[in/out]  pmsg : message data's address
 * @return         APB_PROXY_STATUS_OK: pop successfully, APB_PROXY_STATUS_ERROR: failed to get message data from queue.
 *************************************************************************************/
apb_proxy_status_t apb_proxy_queue_pop_msg(apb_proxy_queue_handle_t apb_proxy_queue, void *pmsg)
{
    apb_proxy_queue_t *queue_handle = NULL;
    uint8_t *psrc_address = NULL;
    configASSERT(apb_proxy_queue != NULL);
    configASSERT(pmsg != NULL);

    queue_handle = (apb_proxy_queue_t *)(apb_proxy_queue);
    if (0U == queue_handle->occupied_count) {
        return APB_PROXY_STATUS_ERROR;
    }

    psrc_address = queue_handle->pdata + (queue_handle->front_index * queue_handle->item_size);
    memcpy(pmsg, psrc_address, queue_handle->item_size);
    queue_handle->occupied_count --;
    if (0 == queue_handle->occupied_count) {
        queue_handle->front_index = 0;
        queue_handle->tail_index = 0;
    } else {
        /*move to next available msg in the front end.*/
        queue_handle->front_index = (queue_handle->front_index + 1) % (queue_handle->capacity);
    }
    return APB_PROXY_STATUS_OK;

}
/*************************************************************************************
 * @brief      Push messages into queue's front end.
 * @param[in]  apb_proxy_queue : the created message queue handle.
 * @param[in]  pmsg : message data's address
 * @return     APB_PROXY_STATUS_OK: push successfully, APB_PROXY_STATUS_ERROR : failed to save the message into queue.
 ***************************************************************************************/
apb_proxy_status_t apb_proxy_queue_push_msg_to_front(apb_proxy_queue_handle_t apb_proxy_queue, void *pmsg)
{
    configASSERT(apb_proxy_queue != NULL);
    configASSERT(pmsg != NULL);

    apb_proxy_queue_t *queue_handle = NULL;
    uint8_t *pdst_address = NULL;

    queue_handle = (apb_proxy_queue_t *)(apb_proxy_queue);
    if (queue_handle->occupied_count == queue_handle->capacity) {
        apb_proxy_log_error("msg queue:: full!");
        return APB_PROXY_STATUS_ERROR;
    }
    if (0U == queue_handle->occupied_count) {
        queue_handle->tail_index = 0;
        queue_handle->front_index = 0;;
    }

    /*find the available buffer before the front point.*/
    queue_handle->front_index = (queue_handle->front_index - 1) % (queue_handle->capacity);
    pdst_address = queue_handle->pdata + (queue_handle->front_index * queue_handle->item_size);
    memcpy(pdst_address, pmsg, queue_handle->item_size);
    queue_handle->occupied_count++;
    return APB_PROXY_STATUS_OK;
}

