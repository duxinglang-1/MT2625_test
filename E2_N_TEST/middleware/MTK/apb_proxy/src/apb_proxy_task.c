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
* AP Bridge Proxy task.
* 1. The task receives all the AP Bridge packets from
*     AP Bridge in modem domain, and then decode the AP Bridge packets to
*     structures and forwards the decoded data to APP.
* 2. The task receives all the structure data from APP.
*     AP Bridge proxy will encodes these data structures into
*     AP Bridge packets and then forwards the data to AP Bridge in modem domain.
**************************************************************************/

#include "apb_proxy_task.h"
#include "stdint.h"
#include "stdbool.h"
#include "mux_ap.h"
#include "apb_proxy_msg_queue_def.h"
#include "apb_proxy_context_manager.h"
#include "apb_proxy_event_handler.h"
#include "apb_proxy_packet_def.h"
#include "apb_proxy_packet_decoder.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS.h"
#include "apb_proxy_log.h"
#include "task_def.h"
#include "string.h"
#include "hal_sleep_manager.h"

log_create_module(apb_proxy, PRINT_LEVEL_INFO);
/*****************************************************************
** AP Bridge Task Definitions
******************************************************************/
#define APB_PROXY_TASK_NAME       "APB_PROXY"
#define APB_PROXY_CMD_TASK_NAME   "APB_CMD"
#define APB_PROXY_TASK_STACKSIZE  (1024 * 8) /*unit byte*/
#define APB_PROXY_TASK_PRIORITY    TASK_PRIORITY_ABOVE_NORMAL
/****************************************************************
** Local Function Prototype
*****************************************************************/
static void apb_proxy_mux_callback(mux_ap_event_t event, void *pdata);
static void apb_proxy_dispatch_event(apb_proxy_event_t *p_apb_proxy_event);
static void apb_proxy_process_event(apb_proxy_event_t *p_apb_proxy_event);
static void apb_proxy_task(void* p_param);
static void apb_proxy_cmd_task(void* p_param);

/****************************************************
** Public Function Implementation
*****************************************************/
/**********************************************************
 * @brief     AP Bridge Proxy init function will create AP
 *            Bridge Proxy task and AP Bridge Proxy will begin
 *            to run.
 * @param[in] None
 * @return    None.
 **********************************************************/
void apb_proxy_init(void)
{
    TaskHandle_t task_handle = NULL;
    TaskHandle_t task_handle2 = NULL;
    /*Before entering task, create AP Bridge related message queue.*/
    apb_proxy_msg_queue_init();
    if (pdPASS != xTaskCreate(apb_proxy_task,
                              APB_PROXY_TASK_NAME,
                              APB_PROXY_TASK_STACKSIZE / sizeof(portSTACK_TYPE),
                              NULL,
                              APB_PROXY_TASK_PRIORITY,
                              &task_handle)) {
        apb_proxy_log_error("create Task ERR");
        return;
    }
    if (pdPASS != xTaskCreate(apb_proxy_cmd_task,
                              APB_PROXY_CMD_TASK_NAME,
                              APB_PROXY_TASK_STACKSIZE / sizeof(portSTACK_TYPE),
                              NULL,
                              APB_PROXY_TASK_PRIORITY,
                              &task_handle)) {
        apb_proxy_log_error("create Task ERR");
        return;
    }
}
/**********************************************************
 * @brief     Send AP Bridge event to AP Bridge Proxy task.
 * @param[in] pevent points to the event buffer.
 * @return    only when the AP Bridge message queue is full,
 *            error will be returned.
 *********************************************************/
apb_proxy_status_t apb_proxy_send_msg_to_ap_bridge_proxy(apb_proxy_event_t *pevent)
{
    if (xQueueSend(g_apb_proxy_external_rx_queue, pevent, 0U) != pdPASS) {
        apb_proxy_log_error("ext queue full");
        return APB_PROXY_STATUS_ERROR;
    }
    return APB_PROXY_STATUS_OK;
}
/****************************************************
** Local Function Implementation
****************************************************/

static void apb_proxy_cmd_task(void* p_param)
{
    apb_proxy_exe_cmd_t received_msg;

    while (1) {
         memset(&received_msg, 0, sizeof(received_msg));
        if (xQueueReceive(g_apb_proxy_cmd_queue, &received_msg, portMAX_DELAY)) {
            if (received_msg.cmd_handler != NULL) {
                (received_msg.cmd_handler)(&(received_msg.cmd_param));
                vPortFree(received_msg.share_buf);
            }
        }
    }
}

/****************************************************
* @brief     AP Bridge Proxy task entry function.
* @param[in] None
* @return    None.
****************************************************/
static void apb_proxy_task(void* p_param)
{
    apb_proxy_event_t received_msg;
    bool read_event = false;
    bool externl_event = false;
    apb_proxy_context_t * p_apb_context = apb_proxy_get_apb_proxy_context_pointer();
    UBaseType_t available_queue_space = 0;

    apb_proxy_context_manager_init();
    p_apb_context->apb_sleep_handle = hal_sleep_manager_set_sleep_handle(APB_PROXY_TASK_NAME);
    if (0xFF == p_apb_context->apb_sleep_handle){
        configASSERT(0);
    }

    /*Register AP Bridge channel in MUX adapter.*/
    if (MUX_AP_STATUS_OK != mux_ap_register_callback(MUX_AP_CHANNEL_TYPE_AP_BRIDGE, apb_proxy_mux_callback, NULL)) {
        apb_proxy_log_error("Reg MUX Failed.");
        configASSERT(0);
    }

    while (1) {
        if (xQueueReceive(g_apb_proxy_external_rx_queue, &received_msg, portMAX_DELAY)) {
            if (false == p_apb_context->be_sleep_locked){
                if (hal_sleep_manager_acquire_sleeplock(p_apb_context->apb_sleep_handle,
                    HAL_SLEEP_LOCK_DEEP) == HAL_SLEEP_MANAGER_OK){
                    p_apb_context->be_sleep_locked = true;
                    apb_proxy_log_info("sleep lock");
                }else{
                    apb_proxy_log_error("sleep lock Failed.");
                }
            }
            apb_proxy_dispatch_event(&received_msg);
            read_event = true;

            while (true == read_event) {
                /*check whether there is any messages in external queue.*/
                if (xQueueReceive(g_apb_proxy_external_rx_queue, &received_msg, 0U)) {
                    if (false == p_apb_context->be_sleep_locked){
                        if (hal_sleep_manager_acquire_sleeplock(p_apb_context->apb_sleep_handle,
                            HAL_SLEEP_LOCK_DEEP) == HAL_SLEEP_MANAGER_OK){
                            p_apb_context->be_sleep_locked = true;
                            apb_proxy_log_info("sleep lock OK.");
                        }else{
                            apb_proxy_log_error("sleep lock Failed.");
                        }
                    }
                    apb_proxy_dispatch_event(&received_msg);
                    externl_event = true;
                }

                available_queue_space = uxQueueSpacesAvailable(g_apb_proxy_external_rx_queue);
                apb_proxy_log_info("external queue left = %d\r\n", available_queue_space);
                if ((true == externl_event) && (available_queue_space < APB_PROXY_EXT_QUEUE_WATER_LEVEL_SIZE)) {
                    /* To guarantee the external queue is not full.*/
                    externl_event = false;
                    apb_proxy_log_info("apb external queue is busy\r\n");
                    continue;
                }

                if (apb_proxy_get_event_from_internal_msg_queue(&received_msg) == APB_PROXY_STATUS_OK) {
                    apb_proxy_log_info("get event from internal queue\r\n");
                    apb_proxy_process_event(&received_msg);
                } else {
                    /*The internal message queue is empty.*/
                    read_event = false;
                    apb_proxy_log_info("internal queue is empty\r\n");
                    if ((true == p_apb_context->be_sleep_locked)
                        && (apb_proxy_get_active_cmd_count() == 0U)){
                        /*No message needs to be handled now.*/
                        if (hal_sleep_manager_release_sleeplock(p_apb_context->apb_sleep_handle,
                            HAL_SLEEP_LOCK_DEEP) == HAL_SLEEP_MANAGER_OK){
                            p_apb_context->be_sleep_locked = false;
                            apb_proxy_log_info("Rel sl_lock.");
                        }else{
                            apb_proxy_log_error("Failed to rel sl_lock.");
                        }
                    }
                }
            }
        }
    }
}
static void apb_proxy_dispatch_event(apb_proxy_event_t *p_apb_proxy_event)
{
    apb_proxy_context_t *apb_proxy_context_p = apb_proxy_get_apb_proxy_context_pointer();

    switch (p_apb_proxy_event->event_type) {
        case APB_PROXY_MUX_EVENT_CHANNEL_ENABLED:
        case APB_PROXY_MUX_EVENT_CHANNEL_DISABLED:
        case APB_PROXY_MUX_EVENT_SEND_COMPLETED: {
            if (apb_proxy_queue_push_msg(g_apb_proxy_internal_hi_rx_queue, p_apb_proxy_event) == APB_PROXY_STATUS_ERROR) {
                apb_proxy_log_error("hi rx FULL");
                configASSERT(0);
            }
            break;
        }
        case APB_PROXY_MUX_EVENT_RECEIVE_COMPLETED: {
            apb_proxy_packet_t apbPacket;
            mux_ap_event_receive_completed_t *p_event_data = &(p_apb_proxy_event->event_data.receive_completed_event);
            if (apb_proxy_context_p->channel_id != p_event_data->channel_id) {
                apb_proxy_log_error("wrong channel ID");
            }
            apbPacket.length = p_event_data->buffer_length;
            apbPacket.pdata = p_event_data->data_buffer;
            /*convert the event type.*/
            p_apb_proxy_event->event_data.packetdata.pdata = apbPacket.pdata;
            p_apb_proxy_event->event_data.packetdata.length = apbPacket.length;
            switch (apb_proxy_get_packet_type(&apbPacket)) {
                case APBRIDGE_CODE_COMMAND_REGISTERED: {
                    p_apb_proxy_event->event_type = APB_PROXY_MD_REG_AT_CMD_RSP;
                    if (apb_proxy_queue_push_msg(g_apb_proxy_internal_mi_rx_queue, p_apb_proxy_event) == APB_PROXY_STATUS_ERROR) {
                        apb_proxy_log_error("mi rx FULL");
                        configASSERT(0);
                    }
                    break;
                }
                case APBRIDGE_CODE_AT_COMMAND: {
                    p_apb_proxy_event->event_type = APB_PROXY_MD_AT_CMD_REQ;
                    if (apb_proxy_queue_push_msg(g_apb_proxy_internal_mi_rx_queue, p_apb_proxy_event) == APB_PROXY_STATUS_ERROR) {
                        apb_proxy_log_error("mi rx FULL");
                        configASSERT(0);
                    }
                    break;
                }
                case APBRIDGE_CODE_USER_DATA: {
                    uint32_t channel_id = 0;
                    apb_proxy_data_mode_t* data_mode_ctx = NULL;
                    p_apb_proxy_event->event_type = APB_PROXY_MD_USER_DATA_IND;
                    channel_id = apb_proxy_get_channel_id(&apbPacket);
                    apb_proxy_log_info("received user data from channel:%d\r\n", channel_id);
                    data_mode_ctx = apb_proxy_get_data_mode_context(channel_id);
                    if (data_mode_ctx == NULL) {
                        vPortFree(apbPacket.pdata);
                        apbPacket.pdata = NULL;
                        apb_proxy_log_error("did not find data mode context\r\n");
                        break;
                    }
                    if (data_mode_ctx->data_rx_queue_from_md!= NULL) {
                        if (apb_proxy_queue_push_msg(data_mode_ctx->data_rx_queue_from_md, p_apb_proxy_event) == APB_PROXY_STATUS_ERROR) {
                            apb_proxy_log_error("user rx FULL");
                            vPortFree(apbPacket.pdata);
                            apbPacket.pdata = NULL;
                        }
                        if (apb_proxy_queue_get_available_space(data_mode_ctx->data_rx_queue_from_md) < APB_PROXY_USER_DATA_QUEUE_CONTROL_SPACE_SIZE) {
                            if (data_mode_ctx->apb_proxy_request_stop_data == false) {
                                apb_proxy_packet_t apb_packet = {0};
                                apb_proxy_log_info("apb proxy request flow control\r\n");
                                if (apb_proxy_encode_xoff_packet(data_mode_ctx->data_mode_channel_id, &apb_packet) == APB_PROXY_STATUS_OK) {
                                    apb_proxy_send_packet_to_modem(&apb_packet);
                                    data_mode_ctx->apb_proxy_request_stop_data = true;
                                }
                            }
                        }
                    } else {
                        apb_proxy_log_error("user rx is null.");
                        vPortFree(apbPacket.pdata);
                        apbPacket.pdata = NULL;
                    }
                    break;
                }
                case APBRIDGE_CODE_XON: {
                    p_apb_proxy_event->event_type = APB_PROXY_MD_XON_REQ;
                    if (apb_proxy_queue_push_msg(g_apb_proxy_internal_hi_rx_queue, p_apb_proxy_event) == APB_PROXY_STATUS_ERROR) {
                        apb_proxy_log_error("hi rx FULL");
                        configASSERT(0);
                    }
                    break;
                }
                case APBRIDGE_CODE_XOFF: {
                    p_apb_proxy_event->event_type = APB_PROXY_MD_XOFF_REQ;
                    if (apb_proxy_queue_push_msg(g_apb_proxy_internal_hi_rx_queue, p_apb_proxy_event) == APB_PROXY_STATUS_ERROR) {
                        apb_proxy_log_error("hi rx FULL");
                        configASSERT(0);
                    }
                    break;
                }
                case APBRIDGE_CODE_DATA_MODE_CLOSED_IND: {
                    p_apb_proxy_event->event_type = APB_PROXY_MD_DATA_MODE_CLOSED_IND;
                    if (apb_proxy_queue_push_msg(g_apb_proxy_internal_hi_rx_queue, p_apb_proxy_event) == APB_PROXY_STATUS_ERROR) {
                        apb_proxy_log_error("hi rx FULL");
                        configASSERT(0);
                    }
                    break;
                }
                case APBRIDGE_CODE_DATA_MODE_TEMP_DEACTIVE_IND: {
                    p_apb_proxy_event->event_type = APB_PROXY_MD_DATA_MODE_TEMP_DEACTIVE_IND;
                    if (apb_proxy_queue_push_msg(g_apb_proxy_internal_hi_rx_queue, p_apb_proxy_event) == APB_PROXY_STATUS_ERROR) {
                        apb_proxy_log_error("hi rx FULL");
                        configASSERT(0);
                    }
                    break;
                }
                case APBRIDGE_CODE_DATA_MODE_RESUMED_IND: {
                    p_apb_proxy_event->event_type = APB_PROXY_MD_DATA_MODE_RESUMED_IND;
                    if (apb_proxy_queue_push_msg(g_apb_proxy_internal_hi_rx_queue, p_apb_proxy_event) == APB_PROXY_STATUS_ERROR) {
                        apb_proxy_log_error("hi rx FULL");
                        configASSERT(0);
                    }
                    break;
                }
                default: {
                    apb_proxy_log_error("wrong apb type:%d\r\n", apb_proxy_get_packet_type(&apbPacket) );
                    break;
                }

            }
            break;
        }
        case APB_PROXY_AP_REG_AT_CMD_REQ:
        case APB_PROXY_AP_AT_CMD_RESULT_IND:
        case APB_PROXY_AP_CLOSE_DATA_MODE_IND: {
            if (apb_proxy_queue_push_msg(g_apb_proxy_internal_mi_tx_queue, p_apb_proxy_event) == APB_PROXY_STATUS_ERROR) {
                apb_proxy_log_error("mi tx FULL");
                configASSERT(0);
            }
            break;
        }

        case APB_PROXY_AP_SEND_USER_DATA_REQ: {
            uint32_t channel_id = p_apb_proxy_event->event_data.packetdata.channel_id;
            apb_proxy_data_mode_t* data_mode_ctx = apb_proxy_get_data_mode_context(channel_id);
            if (data_mode_ctx == NULL) {
                vPortFree(p_apb_proxy_event->event_data.packetdata.pdata);
                p_apb_proxy_event->event_data.packetdata.pdata = NULL;
                break;
            }
            if (data_mode_ctx->data_tx_queue_from_ap!= NULL) {
                if (apb_proxy_queue_push_msg(data_mode_ctx->data_tx_queue_from_ap, p_apb_proxy_event) == APB_PROXY_STATUS_ERROR) {
                    apb_proxy_log_error("user tx FULL");
                    vPortFree(p_apb_proxy_event->event_data.packetdata.pdata);
                    p_apb_proxy_event->event_data.packetdata.pdata = NULL;
                }
            } else {
                apb_proxy_log_error("user tx is null.");
                vPortFree(p_apb_proxy_event->event_data.packetdata.pdata);
                p_apb_proxy_event->event_data.packetdata.pdata = NULL;
            }
            break;
        }
        case APB_PROXY_AP_XON_REQ:
        case APB_PROXY_AP_XOFF_REQ:
        case APB_PROXY_AP_SET_UP_DATA_MODE_REQ:
        case APB_PROXY_AP_DATA_MODE_RESUME_REQ: {
            if (apb_proxy_queue_push_msg(g_apb_proxy_internal_hi_tx_queue, p_apb_proxy_event) == APB_PROXY_STATUS_ERROR) {
                apb_proxy_log_error("hi tx FULL");
                configASSERT(0);
            }
            break;
        }
        default: {
            configASSERT(0);
            break;
        }
    }

}

static void apb_proxy_mux_callback(mux_ap_event_t event, void *pdata)
{
    /*!!!!!Please note: the function is executed in MUX Adapter task.*/
    apb_proxy_event_t apb_proxy_event;
    mux_ap_event_prepare_to_receive_t *p_prepare_receive_event = NULL;
    configASSERT(pdata != NULL);

    switch (event) {
        case MUX_AP_EVENT_CHANNEL_ENABLED: {
            apb_proxy_event.event_type = APB_PROXY_MUX_EVENT_CHANNEL_ENABLED;
            memcpy(&(apb_proxy_event.event_data.channel_enabled_event), pdata, sizeof(mux_ap_event_channel_enabled_t));
            if (xQueueSend(g_apb_proxy_external_rx_queue, &apb_proxy_event, 0U) != pdPASS) {
                apb_proxy_log_error("ext rx full");
            }
            break;
        }
        case MUX_AP_EVENT_CHANNEL_DISABLED: {
            apb_proxy_event.event_type = APB_PROXY_MUX_EVENT_CHANNEL_DISABLED;
            memcpy(&(apb_proxy_event.event_data.channel_disabled_event), pdata, sizeof(mux_ap_event_channel_disabled_t));
            if (xQueueSend(g_apb_proxy_external_rx_queue, &apb_proxy_event, 0U) != pdPASS) {
                apb_proxy_log_error("ext rx full");
            }
            break;
        }
        case MUX_AP_EVENT_SEND_COMPLETED: {
            apb_proxy_event.event_type = APB_PROXY_MUX_EVENT_SEND_COMPLETED;
            memcpy(&(apb_proxy_event.event_data.send_completed_event), pdata, sizeof(mux_ap_event_send_completed_t));
            if (xQueueSend(g_apb_proxy_external_rx_queue, &apb_proxy_event, 0U) != pdPASS) {
                apb_proxy_log_error("ext rx full");
            }
            break;
        }
        case MUX_AP_EVENT_PREPARE_TO_RECEIVE: {
            p_prepare_receive_event = (mux_ap_event_prepare_to_receive_t *)pdata;
            /* To avoid task concurrent issue, assuming that MUX will guarrantee
               the channel id is the right channel id.*/
            if (p_prepare_receive_event->buffer_length > 0U) {
                p_prepare_receive_event->data_buffer = (uint8_t *)pvPortMalloc(p_prepare_receive_event->buffer_length);
                if (NULL == p_prepare_receive_event->data_buffer) {
                    apb_proxy_log_error("mem failed");
                    configASSERT(0);
                }
            } else {
                apb_proxy_log_error("buf len zero");
                configASSERT(0);
            }
            break;
        }
        case MUX_AP_EVENT_RECEIVE_COMPLETED: {
            mux_ap_event_receive_completed_t* rec_comp = (mux_ap_event_receive_completed_t*)pdata;
            apb_proxy_event.event_type = APB_PROXY_MUX_EVENT_RECEIVE_COMPLETED;
            memcpy(&(apb_proxy_event.event_data.receive_completed_event), pdata, sizeof(mux_ap_event_receive_completed_t));
            if (xQueueSend(g_apb_proxy_external_rx_queue, &apb_proxy_event, 0U) != pdPASS) {
                apb_proxy_log_error("ext rx FULL");
                vPortFree(rec_comp->data_buffer);
                rec_comp->data_buffer = NULL;
            }
            break;
        }
        default: {
            break;
        }
    }
}

static void apb_proxy_process_event(apb_proxy_event_t *p_apb_proxy_event)
{
    apb_proxy_context_t *p_apb_proxy_context = apb_proxy_get_apb_proxy_context_pointer();
    configASSERT(p_apb_proxy_event != NULL);
    switch (p_apb_proxy_event->event_type) {
        /*Events come from MUX Adapter.*/
        case APB_PROXY_MUX_EVENT_CHANNEL_ENABLED: {
            apb_proxy_at_cmd_reg_cxt_t *p_cmd_reg_cxt = apb_proxy_get_cmd_reg_cxt_pointer();
            p_apb_proxy_context->channel_id = p_apb_proxy_event->event_data.channel_enabled_event.channel_id;
            p_apb_proxy_context->apb_proxy_channel_enabled = true;
            if (false == p_cmd_reg_cxt->is_all_cmd_registered) {
                apb_proxy_register_at_cmd_req_handler();
            }
            break;
        }
        case APB_PROXY_MUX_EVENT_CHANNEL_DISABLED: {
            p_apb_proxy_context->channel_id = p_apb_proxy_event->event_data.channel_disabled_event.channel_id;
            p_apb_proxy_context->apb_proxy_channel_enabled = false;
            break;
        }
        case APB_PROXY_MUX_EVENT_SEND_COMPLETED: {
            vPortFree(p_apb_proxy_event->event_data.send_completed_event.data_buffer);
            p_apb_proxy_event->event_data.send_completed_event.data_buffer = NULL;
            /*check whether there is more memory now.*/
            //apb_proxy_userdata_flow_control_processor();
            break;
        }
        /*Events come from modem.*/
        case APB_PROXY_MD_REG_AT_CMD_RSP: {
            apb_proxy_md_register_at_cmd_rsp_handler(p_apb_proxy_event);
            break;
        }
        case APB_PROXY_MD_AT_CMD_REQ: {
            apb_proxy_md_at_cmd_req_handler(p_apb_proxy_event);
            break;
        }
        case APB_PROXY_MD_USER_DATA_IND: {
            apb_proxy_md_userdata_ind_handler(p_apb_proxy_event);
            break;
        }
        case APB_PROXY_MD_XON_REQ: {
            apb_proxy_md_xon_req_handler(p_apb_proxy_event);
            break;
        }
        case APB_PROXY_MD_XOFF_REQ: {
            apb_proxy_md_xoff_req_handler(p_apb_proxy_event);
            break;
        }
        case APB_PROXY_MD_DATA_MODE_CLOSED_IND: {
            apb_proxy_md_data_mode_closed_ind_handler(p_apb_proxy_event);
            break;
        }
        case APB_PROXY_MD_DATA_MODE_TEMP_DEACTIVE_IND: {
            apb_proxy_md_data_mode_temp_deactived_ind_handler(p_apb_proxy_event);
            break;
        }
        case APB_PROXY_MD_DATA_MODE_RESUMED_IND: {
            apb_proxy_md_data_mode_resumed_ind_handler(p_apb_proxy_event);
            break;
        }
        /*Events come from APP.*/
        case APB_PROXY_AP_SET_UP_DATA_MODE_REQ: {
            apb_proxy_create_data_mode_handler(p_apb_proxy_event);
            break;
        }
        case APB_PROXY_AP_XON_REQ: {
            apb_proxy_ap_xon_req_handler(p_apb_proxy_event);
            break;
        }
        case APB_PROXY_AP_DATA_MODE_RESUME_REQ: {
            apb_proxy_ap_resume_data_mode_req_handler(p_apb_proxy_event);
            break;
        }
        case APB_PROXY_AP_XOFF_REQ: {
            apb_proxy_ap_xoff_req_handler(p_apb_proxy_event);
            break;
        }
        case APB_PROXY_AP_AT_CMD_RESULT_IND: {
            apb_proxy_ap_at_cmd_result_ind_handler(p_apb_proxy_event);
            break;
        }
        case APB_PROXY_AP_SEND_USER_DATA_REQ: {
            apb_proxy_ap_userdata_handler(p_apb_proxy_event);
            break;
        }
        case APB_PROXY_AP_CLOSE_DATA_MODE_IND: {
            apb_proxy_close_data_mode_handler(p_apb_proxy_event);
            break;
        }
        default: {
            apb_proxy_log_error("wrong event type.");
            configASSERT(0);
            break;
        }
    }
}

