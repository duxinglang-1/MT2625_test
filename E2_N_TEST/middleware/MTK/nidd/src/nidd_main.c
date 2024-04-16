/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
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

#include "string.h"
#include "mux_ap.h"

#include "nidd_internal.h"

#ifdef NIDD_UT
#include "tel_conn_mgr_common.h"
#endif


static nidd_context_struct g_nidd_cnt_ = {0};
nidd_context_struct* g_nidd_cnt = &g_nidd_cnt_;
static TaskHandle_t nidd_task_handle = NULL;


void nidd_process(void *arg);
void nidd_mux_callback(mux_ap_event_t event, void *param);
nidd_ret_t nidd_channel_activated(nidd_channel_activate_struct* channel_activate);
nidd_ret_t nidd_channel_deactivated(nidd_channel_deactivate_struct* channel_deactivate);
nidd_ret_t nidd_mux_send(nidd_mux_send_data_t* send_data);
nidd_ret_t nidd_mux_send_complete(nidd_mux_send_complete_t* send_data);
nidd_ret_t nidd_received_data(nidd_receive_data_struct* receive_data);

extern void ril_channel_mux_callback(mux_ap_event_t event, void *param);


log_create_module(nidd, PRINT_LEVEL_INFO);
#define LOGE(fmt,arg...)   LOG_E(nidd, "[NIDD MAIN]: "fmt,##arg)
#define LOGW(fmt,arg...)   LOG_W(nidd, "[NIDD MAIN]: "fmt,##arg)
#define LOGI(fmt,arg...)   LOG_I(nidd, "[NIDD MAIN]: "fmt,##arg)

nidd_ret_t nidd_init()
{
    if (nidd_task_handle == NULL) {
        xTaskCreate(
            nidd_process,
            NIDD_THREAD_NAME,
            NIDD_THREAD_STACKSIZE / sizeof(portSTACK_TYPE),
            NULL,
            NIDD_THREAD_PRIO,
            &nidd_task_handle);

        if (nidd_task_handle == NULL) {
            LOGW("nidd_init() error. \r\n");
            return NIDD_RET_ERROR;
        }
    } else {
        LOGW("nidd_init() has been ready. \r\n");
        return NIDD_RET_ERROR;
    }

    g_nidd_cnt->queue_id = xQueueCreate(NIDD_QUEUE_LENGTH, sizeof(nidd_general_msg_t));
    g_nidd_cnt->channel_list = NULL;
    g_nidd_cnt->count = 0;

    return NIDD_RET_OK;
}

void nidd_process(void *arg)
{
    nidd_general_msg_t msg_queue_data;

    for (;;) {
        if (xQueueReceive((QueueHandle_t)(g_nidd_cnt->queue_id), &msg_queue_data, portMAX_DELAY) == pdPASS) {

            LOGI("nidd_process() handle input message, msg type = %d\r\n", msg_queue_data.msg_id);

            switch(msg_queue_data.msg_id) {
                case NIDD_MSG_CHANNEL_ACTIVATE_IND: {
                    nidd_channel_activated((nidd_channel_activate_struct*)(msg_queue_data.msg_data));
                }
                    break;

                case NIDD_MSG_CHANNEL_DEACTIVATE_IND: {
                    nidd_channel_deactivated((nidd_channel_deactivate_struct*)(msg_queue_data.msg_data));
                }
                    break;

                case NIDD_MSG_MUX_DATA_IND: {
                    nidd_received_data((nidd_receive_data_struct*)(msg_queue_data.msg_data));
                }
                    break;

                case NIDD_MSG_MUX_SEND_DATA_REQ: {
                    nidd_mux_send((nidd_mux_send_data_t*)(msg_queue_data.msg_data));
                }
                    break;

                case NIDD_MSG_MUX_SEND_COMPLETE_IND: {
                    nidd_mux_send_complete((nidd_mux_send_complete_t*)(msg_queue_data.msg_data));
                }
                    break;

            #if 0
                case MSG_ID_TEL_CONN_MGR_ACTIVATION_RSP: {
                    tel_conn_mgr_activation_rsp_struct *bearer_active_msg = (tel_conn_mgr_activation_rsp_struct *)msg_queue_data.msg_data;
                    LOGI("tel_conn_mgr activate response: %d", bearer_active_msg->result);
                }
                break;
                case MSG_ID_TEL_CONN_MGR_DEACTIVATION_RSP: {
                    tel_conn_mgr_deactivation_rsp_struct *bearer_deactive_msg = (tel_conn_mgr_deactivation_rsp_struct *)msg_queue_data.msg_data;
                    LOGI("tel_conn_mgr deactivate response: %d", bearer_deactive_msg->result);
                }
                break;
            #endif
                default:
                    break;
            }

        }
    }
}

nidd_ret_t nidd_create_nidd_channel(uint32_t* nidd_id, char* apn, nidd_event_handler callback)
{
    nidd_channel_struct* nidd_channel;
    char* apn_name = NULL;
    uint8_t apn_name_len = 0;

    LOGI("nidd_create_nidd_channel apn: %s, %p\r\n", apn, callback);

    if (g_nidd_cnt->count >= NIDD_MAX_CNT)
        return NIDD_RET_ERROR;

    apn_name = nidd_mem_alloc(NIDD_APN_NAME_MAX_LEN + 1);
    if (apn_name == NULL)
        return NIDD_RET_NO_MEMORY;

    apn_name_len = strlen(apn);

    if (apn_name_len > NIDD_APN_NAME_MAX_LEN) {
        nidd_mem_free(apn_name);
        return NIDD_RET_ERROR;
    }

    strncpy(apn_name, apn, apn_name_len);
    apn_name[apn_name_len] = '\0';

    nidd_channel = nidd_mem_alloc(sizeof(nidd_channel_struct));
    if (nidd_channel) {
        nidd_channel->apn = apn_name;
        nidd_channel->callback = callback;
        nidd_channel->status = NIDD_STATUS_INIT;

        g_nidd_cnt->count++;
        nidd_channel->nidd_id = g_nidd_cnt->count;
        nidd_channel->next = g_nidd_cnt->channel_list;
        g_nidd_cnt->channel_list = nidd_channel;

        (*nidd_id) = nidd_channel->nidd_id;

        LOGI("nidd_create_nidd_channel success nidd_id: %d \r\n", nidd_channel->nidd_id);

        return NIDD_RET_OK;
    }

    nidd_mem_free(apn_name);
    return NIDD_RET_NO_MEMORY;
}

nidd_ret_t nidd_destory_nidd_channel(uint32_t nidd_id, char* apn, nidd_event_handler callback)
{
    nidd_channel_struct* nidd_channel = g_nidd_cnt->channel_list;

    LOGI("nidd_destory_nidd_channel apn %s \r\n", apn);

    if (nidd_channel->nidd_id == nidd_id &&
        0 == strncasecmp(nidd_channel->apn, apn, strlen(apn)) &&
        nidd_channel->callback == callback) {
        g_nidd_cnt->channel_list = nidd_channel->next;
        nidd_mem_free(nidd_channel->apn);
        nidd_mem_free(nidd_channel);

        return NIDD_RET_OK;
    }

    while(nidd_channel->next) {
        nidd_channel_struct* nidd_channel_next = nidd_channel->next;
        if (nidd_channel_next->nidd_id == nidd_id &&
            0 == strncasecmp(nidd_channel->apn, apn, strlen(apn)) &&
            nidd_channel_next->callback == callback) {

            nidd_channel->next = nidd_channel_next->next;
            nidd_mem_free(nidd_channel_next->apn);
            nidd_mem_free(nidd_channel_next);

            return NIDD_RET_OK;
        }
        nidd_channel = nidd_channel->next;
    }

    return NIDD_RET_ERROR;
}


nidd_ret_t nidd_received_channel_activated(char* apn, uint32_t channel_id)
{
    nidd_channel_struct* nidd_channel = g_nidd_cnt->channel_list;
    LOGI("nidd_received_channel_activated apn %s, channel_id %d \r\n", apn);

    while(nidd_channel) {
        LOGI("nidd_id: %d, apn: %s", nidd_channel->nidd_id, nidd_channel->apn);
        if (0 == strncasecmp(nidd_channel->apn, apn, strlen(apn))) {
            if (nidd_channel->status != NIDD_STATUS_ACTIVATED) {
                nidd_general_msg_t data_ind;
                nidd_channel_activate_struct* channel_activate = NULL;

                channel_activate = nidd_mem_alloc(sizeof(nidd_channel_activate_struct));

                if (channel_activate == NULL) {
                    return NIDD_RET_NO_MEMORY;
                }

                nidd_channel->channel_id = channel_id;

                channel_activate->apn = nidd_channel->apn;
                channel_activate->channel_id = nidd_channel->channel_id;
                channel_activate->nidd_id = nidd_channel->nidd_id;
                data_ind.msg_id = NIDD_MSG_CHANNEL_ACTIVATE_IND;
                data_ind.msg_data = (void*)channel_activate;

                xQueueSend((QueueHandle_t)g_nidd_cnt->queue_id, &data_ind, 0);
                return NIDD_RET_OK;
            }  else {
                LOGI("nidd_received_channel_activated status error !!!");
                return NIDD_RET_STATUS_ERROR;
            }
        }

        nidd_channel = nidd_channel->next;
    }

    return NIDD_RET_ERROR;
}

nidd_ret_t nidd_received_channel_deactivated(char* apn, uint32_t channel_id)
{
    nidd_channel_struct* nidd_channel = g_nidd_cnt->channel_list;
    LOGI("nidd_received_channel_deactivated apn %s channel_id %d \r\n", apn, channel_id);

    while(nidd_channel) {
        if (0 == strncasecmp(nidd_channel->apn, apn, strlen(apn)) &&
            nidd_channel->channel_id == channel_id) {
            if (nidd_channel->status == NIDD_STATUS_ACTIVATED) {
                nidd_general_msg_t data_ind;
                nidd_channel_deactivate_struct* channel_deactivate = NULL;

                channel_deactivate = nidd_mem_alloc(sizeof(nidd_channel_deactivate_struct));

                if (channel_deactivate == NULL) {
                    return NIDD_RET_NO_MEMORY;
                }

                channel_deactivate->apn = nidd_channel->apn;
                channel_deactivate->channel_id = nidd_channel->channel_id;
                channel_deactivate->nidd_id = nidd_channel->nidd_id;
                data_ind.msg_id = NIDD_MSG_CHANNEL_DEACTIVATE_IND;
                data_ind.msg_data = (void*)channel_deactivate;

                xQueueSend((QueueHandle_t)g_nidd_cnt->queue_id, &data_ind, 0);
                return NIDD_RET_OK;
            } else {
                LOGI("nidd_received_channel_deactivated status error !!!");
                return NIDD_RET_STATUS_ERROR;
            }
        }

        nidd_channel = nidd_channel->next;
    }

    return NIDD_RET_ERROR;
}


void nidd_bearer_info_ind(nidd_bearer_info_struct *bearer_info)
{
    if (bearer_info->is_activated)
        nidd_received_channel_activated(bearer_info->apn, bearer_info->channel_id);
    else
        nidd_received_channel_deactivated(bearer_info->apn, bearer_info->channel_id);
}

nidd_ret_t nidd_send_data(uint32_t nidd_id, void* data, uint32_t length)
{
    uint8_t* data_buf =  NULL;
    nidd_general_msg_t data_ind;
    nidd_mux_send_data_t* send_data = NULL;
    nidd_channel_struct* channel = NULL;

    LOGI("nidd_send_data nidd_id %d length %d \r\n", nidd_id, length);

    channel = nidd_get_channel_by_id(nidd_id);

    if (channel == NULL) {
        return NIDD_RET_ERROR;
    }

    if (channel->status != NIDD_STATUS_ACTIVATED) {
        LOGI("return NIDD_RET_STATUS_ERROR");
        return NIDD_RET_STATUS_ERROR;
    }

    data_buf = nidd_mem_alloc(length + 1);

    if (data_buf == NULL) {
        LOGI("return NIDD_RET_NO_MEMORY 1");
        return NIDD_RET_NO_MEMORY;
    }
    strncpy((char *)data_buf, data, length);
    data_buf[length] = '\0';

    send_data = nidd_mem_alloc(sizeof(nidd_mux_send_data_t));

    if (send_data == NULL) {
        nidd_mem_free(data_buf);
        LOGI("return NIDD_RET_NO_MEMORY 2");
        return NIDD_RET_NO_MEMORY;
    }

    send_data->channel_id = channel->channel_id;
    send_data->data_buffer = data_buf;
    send_data->data_length = length;
    data_ind.msg_id = NIDD_MSG_MUX_SEND_DATA_REQ;
    data_ind.msg_data = (void*)send_data;
    xQueueSend((QueueHandle_t)g_nidd_cnt->queue_id, &data_ind, 0);
    return NIDD_RET_WOULDBLOCK;
}

void nidd_mux_callback(mux_ap_event_t event, void *param)
{
    LOGI("event %d", event);

    switch(event) {
        case MUX_AP_EVENT_CHANNEL_ENABLED:
            break;

        case MUX_AP_EVENT_CHANNEL_DISABLED:
            break;

        case MUX_AP_EVENT_SEND_COMPLETED: {
            mux_ap_event_send_completed_t* send_completed = (mux_ap_event_send_completed_t*)param;
            nidd_channel_struct* nidd_channel = NULL;
            nidd_general_msg_t data_ind;
            nidd_mux_send_complete_t* send_completed_ind = NULL;

            if (send_completed == NULL)
                return;

            nidd_channel = nidd_get_channel_by_channelid(send_completed->channel_id);

            if (nidd_channel == NULL)
                return;

            send_completed_ind = nidd_mem_alloc(sizeof(nidd_mux_send_complete_t));

            if (send_completed_ind == NULL)
                return;

            send_completed_ind->data_buffer = send_completed->data_buffer;
            send_completed_ind->user_data = send_completed->user_data;
            data_ind.msg_id = NIDD_MSG_MUX_SEND_COMPLETE_IND;
            data_ind.msg_data = (void*)send_completed_ind;
            xQueueSend((QueueHandle_t)g_nidd_cnt->queue_id, &data_ind, 0);
            LOGI("MUX_AP_EVENT_SEND_COMPLETED \r\n");
        }
            break;

        case MUX_AP_EVENT_PREPARE_TO_RECEIVE: {
            nidd_channel_struct* nidd_channel = NULL;
            mux_ap_event_prepare_to_receive_t* prepare_receive = (mux_ap_event_prepare_to_receive_t*)param;

            if (prepare_receive == NULL)
                return;

            nidd_channel = nidd_get_channel_by_channelid(prepare_receive->channel_id);

            if (nidd_channel == NULL || nidd_channel->status != NIDD_STATUS_ACTIVATED)
                return;

            prepare_receive->data_buffer = (uint8_t*)nidd_mem_alloc(prepare_receive->buffer_length);

            if (prepare_receive->data_buffer) {
                LOGI("MUX_AP_EVENT_PREPARE_TO_RECEIVE \r\n");
            } else {
                LOGI("no memory \r\n");
            }
        }
            break;

        case MUX_AP_EVENT_RECEIVE_COMPLETED: {
            nidd_channel_struct* nidd_channel = NULL;
            nidd_general_msg_t data_ind;
            mux_ap_event_receive_completed_t* receive_completed = (mux_ap_event_receive_completed_t*)param;
            nidd_receive_data_struct* receive_data = NULL;

            if (receive_completed == NULL)
                return;

            nidd_channel = nidd_get_channel_by_channelid(receive_completed->channel_id);

            if (nidd_channel == NULL || nidd_channel->status != NIDD_STATUS_ACTIVATED) {
                // TODO:
                nidd_mem_free(receive_completed->data_buffer);
                return;
            }

            receive_data = nidd_mem_alloc(sizeof(nidd_receive_data_struct));

            if (receive_data == NULL) {
                nidd_mem_free(receive_completed->data_buffer);
                return;
            }

            receive_data->channel_id = receive_completed->channel_id;
            receive_data->data = receive_completed->data_buffer;
            receive_data->length = strlen((char *)receive_completed->data_buffer);

            data_ind.msg_id = NIDD_MSG_MUX_DATA_IND;
            data_ind.msg_data = (void*)receive_data;

            xQueueSend((QueueHandle_t)g_nidd_cnt->queue_id, &data_ind, 0);
            LOGI("MUX_AP_EVENT_RECEIVE_COMPLETED \r\n");
        }
            break;

        default:
            break;
    }
}


nidd_ret_t nidd_channel_activated(nidd_channel_activate_struct* channel_activate)
{
    nidd_channel_struct* channel = nidd_get_channel_by_channelid(channel_activate->channel_id);
    LOGI("nidd_channel_activated \r\n");

    if (channel == NULL) {
        nidd_mem_free(channel_activate);
        return NIDD_RET_ERROR;
    }

    channel->status = NIDD_STATUS_ACTIVATED;

    LOGI("nidd_received_data callback \r\n");
    mux_ap_change_callback(channel->channel_id, nidd_mux_callback);
    ril_channel_forbid_data_receiving(channel->channel_id, false);
    if(channel->callback != NULL) {
        channel->callback(NIDD_EVENT_CHANNEL_ACTIVATE_IND, channel_activate);
    }
    nidd_mem_free(channel_activate);

    return NIDD_RET_OK;
}

nidd_ret_t nidd_channel_deactivated(nidd_channel_deactivate_struct* channel_deactivate)
{
    nidd_channel_struct* channel = nidd_get_channel_by_channelid(channel_deactivate->channel_id);
    LOGI("nidd_channel_deactivated \r\n");

    if (channel == NULL) {
        nidd_mem_free(channel_deactivate);
        return NIDD_RET_ERROR;
    }

    channel->status = NIDD_STATUS_DEACTIVATED;

    LOGI("nidd_received_data callback \r\n");
    mux_ap_change_callback(channel->channel_id, ril_channel_mux_callback);
    ril_channel_forbid_data_receiving(channel->channel_id, false);
    channel->callback(NIDD_EVENT_CHANNEL_DEACTIVATE_IND, channel_deactivate);

    nidd_mem_free(channel_deactivate);

    return NIDD_RET_OK;
}


nidd_ret_t nidd_mux_send(nidd_mux_send_data_t* send_data)
{
    mux_ap_status_t status;

    if (send_data ==  NULL)
        return NIDD_RET_ERROR;

    status = mux_ap_send_data_ex(send_data->channel_id, send_data->data_buffer, send_data->data_length, NULL, true);
    LOGI("nidd_mux_send status: %d \r\n  data: %s \r\n", status, send_data->data_buffer);

    nidd_mem_free(send_data);

    return NIDD_RET_OK;
}

nidd_ret_t nidd_mux_send_complete(nidd_mux_send_complete_t* send_complete)
{
    LOGI("nidd_mux_send_complete \r\n");
    if (send_complete ==  NULL)
        return NIDD_RET_ERROR;

    nidd_mem_free(send_complete->data_buffer);
    nidd_mem_free(send_complete);

    return NIDD_RET_OK;
}

nidd_ret_t nidd_received_data(nidd_receive_data_struct* receive_data)
{
    nidd_channel_struct* channel = NULL;

    LOGI("nidd_received_data \r\n");

    if (receive_data == NULL)
        return NIDD_RET_ERROR;

    channel = nidd_get_channel_by_channelid(receive_data->channel_id);

    if (channel == NULL || channel->status != NIDD_STATUS_ACTIVATED) {
        nidd_mem_free(receive_data->data);
        nidd_mem_free(receive_data);
        return NIDD_RET_ERROR;
    }

    LOGI("nidd_received_data \r\n %s \r\n", receive_data);
    channel->callback(NIDD_EVENT_DATA_IND, receive_data);

    nidd_mem_free(receive_data->data);
    nidd_mem_free(receive_data);

    return NIDD_RET_OK;
}


