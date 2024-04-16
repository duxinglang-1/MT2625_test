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

#include <stdbool.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "task_def.h"
#include "queue.h"
#include "semphr.h"
#include "syslog.h"
#include "mux_ap.h"
#include "mux_ap_private.h"
#include "mux_ap_csci.h"
#include "mux_ap_test.h"
#include "csci.h"
#include "csci_message.h"
#include "memory_attribute.h"
#include "hal_rtc_internal.h"
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_platform.h"
#include "hal_rtc_external.h"

log_create_module(mux_ap, PRINT_LEVEL_INFO);

#define MUX_AP_TASK_NAME                "MUX_AP"
#define MUX_AP_TASK_STACK_SIZE          (1024 * 2)
#define MUX_AP_TASK_PRIORITY            (TASK_PRIORITY_HIGH)

#define MUX_AP_QUEUE_LENGTH             (200)
#define MUX_AP_CHANNEL_NUM              (MUX_AP_AT_AND_DATA_CHANNEL_NUM + MUX_AP_AP_BRIDGE_CHANNEL_NUM + MUX_AP_SIMAT_CHANNEL_NUM + MUX_AP_SOFTSIM_CHANNEL_NUM + MUX_AP_LBS_CHANNEL_NUM)

#ifdef MUX_AP_UT
#define MUX_MD_TASK_NAME                "MUX_MD"
#define MUX_MD_TASK_STACK_SIZE          (1024 * 2)
#define MUX_MD_TASK_PRIORITY            (TASK_PRIORITY_SOFT_REALTIME)

#define MUX_MD_QUEUE_LENGTH             (20)
#endif

typedef struct {
    mux_ap_channel_type_t channel_type;
    mux_ap_channel_id_t channel_id;
    mux_ap_callback_t user_callback;
    void *user_data;
    bool is_ready;
    bool is_tx_flow_control;
    bool is_rx_flow_control;
} mux_ap_channel_manager_t;

typedef struct {
    TaskHandle_t task_handle;
    QueueHandle_t queue_handle;
    SemaphoreHandle_t mutex_handle;
    bool is_channel_manager_init;
    uint8_t sleep_handle;
    uint8_t lock_category;
    bool is_locking;
    bool is_receiving_data;
    mux_ap_channel_manager_t channel_manager[MUX_AP_CHANNEL_NUM];
} mux_ap_context_t;

static mux_ap_context_t g_mux_ap_context;
static mux_ap_context_t *const g_mux_ap_ptr = &g_mux_ap_context;

typedef struct {
    mux_ap_callback_t user_callback;
} mux_ap_channel_manager_retention_t;

typedef struct {
    mux_ap_channel_manager_retention_t channel_manager[MUX_AP_CHANNEL_NUM];
} mux_ap_context_retention_t;

static ATTR_ZIDATA_IN_RETSRAM mux_ap_context_retention_t g_mux_ap_context_retention;
static ATTR_RWDATA_IN_RETSRAM mux_ap_context_retention_t *const g_mux_ap_retention_ptr = &g_mux_ap_context_retention;

#ifdef MUX_AP_UT
typedef struct {
    TaskHandle_t task_handle;
    QueueHandle_t queue_handle;
} mux_md_context_t;

static mux_md_context_t g_mux_md_context;
static mux_md_context_t *const g_mux_md_ptr = &g_mux_md_context;
#endif

typedef enum {
    MUX_AP_MSG_SEND_CHANNEL_ENABLE_REQUEST,
    MUX_AP_MSG_SEND_LINE_STATUS,
    MUX_AP_MSG_SEND_USER_DATA,
    MUX_AP_MSG_RESEND,
    MUX_AP_MSG_RECEIVE_DATA,
    MUX_AP_MSG_RECEIVE_DATA_FROM_RECEIVE_LIST,
#ifdef MUX_AP_UT
    MUX_MD_MSG_RECEIVE_DATA,
#endif
    MUX_AP_MSG_MAX
} mux_ap_msg_t;

typedef struct {
    mux_ap_msg_t msg_id;
    void *param;
} mux_ap_message_t;

static bool mux_ap_send_message(mux_ap_msg_t msg_id, void *param)
{
    mux_ap_message_t message;

    message.msg_id = msg_id;
    message.param = param;

    if (xQueueSend(g_mux_ap_ptr->queue_handle, &message, 0) != pdPASS) {
        configASSERT(0);
        return false;
    }

    return true;
}

static bool mux_ap_send_message_from_isr(mux_ap_msg_t msg_id, void *param)
{
    mux_ap_message_t message;
    BaseType_t xHigherPriorityTaskWoken;

    message.msg_id = msg_id;
    message.param = param;

    if (xQueueSendFromISR(g_mux_ap_ptr->queue_handle, &message, &xHigherPriorityTaskWoken) != pdPASS) {
        configASSERT(0);
        return false;
    }

    return true;
}

#ifdef MUX_AP_UT
static bool mux_md_send_message_from_isr(mux_ap_msg_t msg_id, void *param)
{
    mux_ap_message_t message;
    BaseType_t xHigherPriorityTaskWoken;

    message.msg_id = msg_id;
    message.param = param;

    if (xQueueSendFromISR(g_mux_md_ptr->queue_handle, &message, &xHigherPriorityTaskWoken) != pdPASS) {
        configASSERT(0);
        return false;
    }

    return true;
}
#endif

typedef struct {
    mux_ap_channel_id_t channel_id;
    mux_ap_channel_type_t channel_type;
} mux_ap_channel_enable_request_t;

typedef struct {
    mux_ap_channel_id_t channel_id;
    bool rts_on;
} mux_ap_line_status_t;

typedef struct {
    mux_ap_channel_id_t channel_id;
    uint8_t *data_buffer;
    uint32_t buffer_length;
    mux_ap_callback_t user_callback;
    void *user_data;
    bool is_ip_data;
} mux_ap_user_data_t;

typedef struct {
    mux_ap_channel_id_t channel_id;
} mux_ap_resume_to_receive_t;

static void mux_ap_send_channel_enable_request(mux_ap_channel_id_t channel_id, mux_ap_channel_type_t channel_type)
{
    mux_ap_channel_enable_request_t *channel_enable_request = (mux_ap_channel_enable_request_t *)pvPortMalloc(sizeof(mux_ap_channel_enable_request_t));

    configASSERT(channel_enable_request != NULL);
    channel_enable_request->channel_id = channel_id;
    channel_enable_request->channel_type = channel_type;

    mux_ap_send_message(MUX_AP_MSG_SEND_CHANNEL_ENABLE_REQUEST, channel_enable_request);
}

void mux_ap_send_line_status(mux_ap_channel_id_t channel_id, bool rts_on)
{
    mux_ap_line_status_t *line_status = (mux_ap_line_status_t *)pvPortMalloc(sizeof(mux_ap_line_status_t));

    configASSERT(line_status != NULL);
    line_status->channel_id = channel_id;
    line_status->rts_on = rts_on;

    mux_ap_send_message(MUX_AP_MSG_SEND_LINE_STATUS, line_status);
}

static void mux_ap_send_user_data(mux_ap_channel_id_t channel_id, const uint8_t *data_buffer, uint32_t buffer_length, mux_ap_callback_t user_callback, void *user_data, bool is_ip_data)
{
    mux_ap_user_data_t *send_data = (mux_ap_user_data_t *)pvPortMalloc(sizeof(mux_ap_user_data_t));

    configASSERT(send_data != NULL);
    send_data->channel_id = channel_id;
    send_data->data_buffer = (uint8_t *)data_buffer;
    send_data->buffer_length = buffer_length;
    send_data->user_callback = user_callback;
    send_data->user_data = user_data;
    send_data->is_ip_data = is_ip_data;

    mux_ap_send_message(MUX_AP_MSG_SEND_USER_DATA, send_data);
}

static void mux_ap_csci_callback(csci_callback_event_t event, void *user_data)
{
    UBaseType_t uxSavedInterruptStatus;

    switch (event) {
        case CSCI_EVENT_READY_TO_READ:
            uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
            if (g_mux_ap_ptr->is_receiving_data) {
                portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);
                break;
            } else {
                g_mux_ap_ptr->is_receiving_data = true;
            }
            portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);
            mux_ap_send_message_from_isr(MUX_AP_MSG_RECEIVE_DATA, NULL);
            break;

        case CSCI_EVENT_READY_TO_WRITE:
            mux_ap_send_message_from_isr(MUX_AP_MSG_RESEND, NULL);
            break;

        default:
            break;
    }
}

#ifdef MUX_AP_UT
static void mux_md_csci_callback(csci_callback_event_t event, void *user_data)
{
    if (event == CSCI_EVENT_READY_TO_READ) {
        mux_md_send_message_from_isr(MUX_MD_MSG_RECEIVE_DATA, NULL);
    }
}
#endif

static void mux_ap_message_processing(void *arg)
{
    mux_ap_message_t message;

    while (1) {
        if (xQueueReceive(g_mux_ap_ptr->queue_handle, &message, portMAX_DELAY)) {
            switch (message.msg_id) {
                case MUX_AP_MSG_SEND_CHANNEL_ENABLE_REQUEST: {
                    mux_ap_channel_enable_request_t *channel_enable_request = (mux_ap_channel_enable_request_t *)message.param;
                    if (channel_enable_request->channel_type == MUX_AP_CHANNEL_TYPE_AT_AND_DATA) {
                        mux_ap_csci_send_channel_enable_request(channel_enable_request->channel_id, CSCI_CHANNEL_TYPE_AT_AND_DATA, false);
                    } else if (channel_enable_request->channel_type == MUX_AP_CHANNEL_TYPE_AP_BRIDGE) {
                        mux_ap_csci_send_channel_enable_request(channel_enable_request->channel_id, CSCI_CHANNEL_TYPE_AP_BRIDGE, false);
                    } else if (channel_enable_request->channel_type == MUX_AP_CHANNEL_TYPE_SIMAT) {
                        mux_ap_csci_send_channel_enable_request(channel_enable_request->channel_id, CSCI_CHANNEL_TYPE_BIP, false);
                    } else if (channel_enable_request->channel_type == MUX_AP_CHANNEL_TYPE_SOFTSIM) {
                        mux_ap_csci_send_channel_enable_request(channel_enable_request->channel_id, CSCI_CHANNEL_TYPE_SOFTSIM, false);
                    } else if (channel_enable_request->channel_type == MUX_AP_CHANNEL_TYPE_LBS) {
                        mux_ap_csci_send_channel_enable_request(channel_enable_request->channel_id, CSCI_CHANNEL_TYPE_LBS, false);
                    } else {
                        configASSERT(0);
                    }
                }
                break;

                case MUX_AP_MSG_SEND_LINE_STATUS: {
                    mux_ap_line_status_t *line_status = (mux_ap_line_status_t *)message.param;
                    if (line_status->rts_on) {
                        mux_ap_csci_send_line_status(line_status->channel_id, CSCI_CHANNEL_LINE_ON, false);
                    } else {
                        mux_ap_csci_send_line_status(line_status->channel_id, CSCI_CHANNEL_LINE_OFF, false);
                    }
                }
                break;

                case MUX_AP_MSG_SEND_USER_DATA: {
                    mux_ap_user_data_t *send_data = (mux_ap_user_data_t *)message.param;
                    mux_ap_csci_send_user_data(send_data->channel_id, send_data->data_buffer, send_data->buffer_length, send_data->user_callback, send_data->user_data, send_data->is_ip_data, false);
                }
                break;

                case MUX_AP_MSG_RESEND: {
                    mux_ap_csci_resend();
                }
                break;

                case MUX_AP_MSG_RECEIVE_DATA: {
                    mux_ap_csci_receive_data();
                }
                break;

                case MUX_AP_MSG_RECEIVE_DATA_FROM_RECEIVE_LIST: {
                    mux_ap_resume_to_receive_t *resume_to_receive = (mux_ap_resume_to_receive_t *)message.param;
                    mux_ap_csci_receive_data_from_receive_list(resume_to_receive->channel_id);
                }
                break;

#ifdef MUX_AP_UT
                case MUX_MD_MSG_RECEIVE_DATA: {
                    mux_md_csci_receive_data();
                }
                break;
#endif

                default:
                    break;
            }

            if (message.param != NULL) {
                vPortFree(message.param);
            }
        }
    }
}

#ifdef MUX_AP_UT
static void mux_md_message_processing(void *arg)
{
    mux_ap_message_t message;

    while (1) {
        if (xQueueReceive(g_mux_md_ptr->queue_handle, &message, portMAX_DELAY)) {
            MUX_AP_LOGI("msg_id = %d", (int)message.msg_id);

            switch (message.msg_id) {
                case MUX_MD_MSG_RECEIVE_DATA: {
                    mux_md_csci_receive_data();
                }
                break;

                default:
                    break;
            }

            if (message.param != NULL) {
                vPortFree(message.param);
            }
        }
    }
}
#endif

#ifdef MUX_AP_UT
void mux_md_init(void)
{
    csci_status_t status = csci_md_register_callback(mux_md_csci_callback, NULL);
    MUX_AP_LOGI("csci_md_register_callback() = %d", (int)status);
    configASSERT(status == CSCI_STATUS_OK);

    if (g_mux_md_ptr->queue_handle == NULL) {
        g_mux_md_ptr->queue_handle = xQueueCreate(MUX_MD_QUEUE_LENGTH, sizeof(mux_ap_message_t));
        configASSERT(g_mux_md_ptr->queue_handle != NULL);
    }

    if (g_mux_md_ptr->task_handle == NULL) {
        xTaskCreate(mux_md_message_processing,
                    MUX_MD_TASK_NAME,
                    MUX_MD_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                    NULL,
                    MUX_MD_TASK_PRIORITY,
                    &g_mux_md_ptr->task_handle);
        configASSERT(g_mux_md_ptr->task_handle != NULL);
    }
}
#endif

void mux_ap_init(void)
{
    csci_status_t status;

    if (g_mux_ap_ptr->queue_handle == NULL) {
        g_mux_ap_ptr->queue_handle = xQueueCreate(MUX_AP_QUEUE_LENGTH, sizeof(mux_ap_message_t));
        configASSERT(g_mux_ap_ptr->queue_handle != NULL);
    }

    mux_ap_csci_init();

    status = csci_ap_register_callback(mux_ap_csci_callback, NULL);
    MUX_AP_LOGI("csci_ap_register_callback() = %d", (int)status);
    configASSERT(status == CSCI_STATUS_OK);

#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_mux_ap_ptr->sleep_handle = hal_sleep_manager_set_sleep_handle(MUX_AP_TASK_NAME);
    configASSERT(g_mux_ap_ptr->sleep_handle != SLEEP_LOCK_INVALID_ID);
#endif

    if (g_mux_ap_ptr->task_handle == NULL) {
        xTaskCreate(mux_ap_message_processing,
                    MUX_AP_TASK_NAME,
                    MUX_AP_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                    NULL,
                    MUX_AP_TASK_PRIORITY,
                    &g_mux_ap_ptr->task_handle);
        configASSERT(g_mux_ap_ptr->task_handle != NULL);
    }

#ifdef MUX_AP_UT
#if 0
    mux_md_init();
#endif
    mux_ap_test_init();
#endif
}

static bool mux_ap_channel_type_is_valid(mux_ap_channel_type_t channel_type)
{
    return (channel_type < MUX_AP_CHANNEL_TYPE_MAX);
}

mux_ap_status_t mux_ap_register_callback(mux_ap_channel_type_t channel_type, mux_ap_callback_t user_callback, void *user_data)
{
    uint32_t i;

    MUX_AP_LOGI("channel_type = %d, user_callback = 0x%x, user_data = 0x%x", (int)channel_type, (unsigned int)user_callback, (unsigned int)user_data);

    if (!mux_ap_channel_type_is_valid(channel_type) || user_callback == NULL) {
        MUX_AP_LOGE("MUX_AP_STATUS_INVALID_PARAMETER");
        return MUX_AP_STATUS_INVALID_PARAMETER;
    }

    if (g_mux_ap_ptr->mutex_handle == NULL) {
        g_mux_ap_ptr->mutex_handle = xSemaphoreCreateMutex();
        configASSERT(g_mux_ap_ptr->mutex_handle != NULL);
    }

    xSemaphoreTake(g_mux_ap_ptr->mutex_handle, portMAX_DELAY);

    if (!g_mux_ap_ptr->is_channel_manager_init) {
        g_mux_ap_ptr->is_channel_manager_init = true;

        MUX_AP_LOGI("channel manager init");

        /* channel_type :   channel_id */
        /* AT & Data    :   1 */
        /* AT & Data    :   2 */
        /* AT & Data    :   3 */
        /* AT & Data    :   4 */
        /* AT & Data    :   5 */
        /* AT & Data    :   6 */
        /* AT & Data    :   7 */
        /* AP Bridge    :   8 */
        /* SIMAT        :   9 */
        /* SoftSIM      :   10 */
        /* LBS          :   11 */

        for (i = 0; i < MUX_AP_AT_AND_DATA_CHANNEL_NUM; i++) {
            g_mux_ap_ptr->channel_manager[i].channel_type = MUX_AP_CHANNEL_TYPE_AT_AND_DATA;
        }
        for (i = 0; i < MUX_AP_AP_BRIDGE_CHANNEL_NUM; i++) {
            g_mux_ap_ptr->channel_manager[i + MUX_AP_AT_AND_DATA_CHANNEL_NUM].channel_type = MUX_AP_CHANNEL_TYPE_AP_BRIDGE;
        }
        for (i = 0; i < MUX_AP_SIMAT_CHANNEL_NUM; i++) {
            g_mux_ap_ptr->channel_manager[i + MUX_AP_AT_AND_DATA_CHANNEL_NUM + MUX_AP_AP_BRIDGE_CHANNEL_NUM].channel_type = MUX_AP_CHANNEL_TYPE_SIMAT;
        }
        for (i = 0; i < MUX_AP_SOFTSIM_CHANNEL_NUM; i++) {
            g_mux_ap_ptr->channel_manager[i + MUX_AP_AT_AND_DATA_CHANNEL_NUM + MUX_AP_AP_BRIDGE_CHANNEL_NUM + MUX_AP_SIMAT_CHANNEL_NUM].channel_type = MUX_AP_CHANNEL_TYPE_SOFTSIM;
        }
        for (i = 0; i < MUX_AP_LBS_CHANNEL_NUM; i++) {
            g_mux_ap_ptr->channel_manager[i + MUX_AP_AT_AND_DATA_CHANNEL_NUM + MUX_AP_AP_BRIDGE_CHANNEL_NUM + MUX_AP_SIMAT_CHANNEL_NUM + MUX_AP_SOFTSIM_CHANNEL_NUM].channel_type = MUX_AP_CHANNEL_TYPE_LBS;
        }
        for (i = 0; i < MUX_AP_CHANNEL_NUM; i++) {
            g_mux_ap_ptr->channel_manager[i].channel_id = i + 1;
            g_mux_ap_ptr->channel_manager[i].is_tx_flow_control = true;
        }
    }

    if (rtc_power_on_result_external() != DEEP_SLEEP && rtc_power_on_result_external() != DEEPER_SLEEP) {
        /* COLD-BOOT case: normal init */
        for (i = 0; i < MUX_AP_CHANNEL_NUM; i++) {
            if (g_mux_ap_ptr->channel_manager[i].channel_type == channel_type) {
                if (g_mux_ap_ptr->channel_manager[i].user_callback == NULL) {
                    g_mux_ap_ptr->channel_manager[i].user_callback = user_callback;
                    g_mux_ap_ptr->channel_manager[i].user_data = user_data;
                    g_mux_ap_retention_ptr->channel_manager[i].user_callback = user_callback;

                    xSemaphoreGive(g_mux_ap_ptr->mutex_handle);

                    MUX_AP_LOGI("channel_id = %d, channel_type = %d", (int)g_mux_ap_ptr->channel_manager[i].channel_id, (int)channel_type);

                    mux_ap_send_channel_enable_request(g_mux_ap_ptr->channel_manager[i].channel_id, channel_type);

                    return MUX_AP_STATUS_OK;
                }
            }
        }
    } else {
        /* DEEP-SLEEP case: data retention process */
        for (i = 0; i < MUX_AP_CHANNEL_NUM; i++) {
            if (g_mux_ap_ptr->channel_manager[i].channel_type == channel_type) {
                if (g_mux_ap_ptr->channel_manager[i].user_callback == NULL) {
                    g_mux_ap_ptr->channel_manager[i].user_callback = g_mux_ap_retention_ptr->channel_manager[i].user_callback;
                    g_mux_ap_ptr->channel_manager[i].user_data = user_data;

                    xSemaphoreGive(g_mux_ap_ptr->mutex_handle);

                    MUX_AP_LOGI("channel_id = %d, channel_type = %d", (int)g_mux_ap_ptr->channel_manager[i].channel_id, (int)channel_type);

                    mux_ap_channel_manager_set_ready(g_mux_ap_ptr->channel_manager[i].channel_id, true);

                    return MUX_AP_STATUS_OK;
                }
            }
        }
    }

    xSemaphoreGive(g_mux_ap_ptr->mutex_handle);

    MUX_AP_LOGE("MUX_AP_STATUS_NO_FREE_CHANNEL");

    return MUX_AP_STATUS_NO_FREE_CHANNEL;
}

mux_ap_status_t mux_ap_change_callback(mux_ap_channel_id_t channel_id, mux_ap_callback_t user_callback)
{
    uint32_t i;

    MUX_AP_LOGI("channel_id = %d, user_callback = 0x%x", (int)channel_id, (unsigned int)user_callback);

    if (user_callback == NULL) {
        MUX_AP_LOGE("MUX_AP_STATUS_INVALID_PARAMETER");
        return MUX_AP_STATUS_INVALID_PARAMETER;
    }

    for (i = 0; i < MUX_AP_CHANNEL_NUM; i++) {
        if (g_mux_ap_ptr->channel_manager[i].channel_id == channel_id) {
            configASSERT(g_mux_ap_ptr->channel_manager[i].user_callback != NULL);
            g_mux_ap_ptr->channel_manager[i].user_callback = user_callback;
            g_mux_ap_retention_ptr->channel_manager[i].user_callback = user_callback;

            return MUX_AP_STATUS_OK;
        }
    }

    MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_FOUND");

    return MUX_AP_STATUS_CHANNEL_NOT_FOUND;
}

mux_ap_status_t mux_ap_send_data(mux_ap_channel_id_t channel_id, const uint8_t *data_buffer, uint32_t buffer_length, void *user_data)
{
    return mux_ap_send_data_ex(channel_id, data_buffer, buffer_length, user_data, false);
}

mux_ap_status_t mux_ap_send_data_ex(mux_ap_channel_id_t channel_id, const uint8_t *data_buffer, uint32_t buffer_length, void *user_data, bool is_ip_data)
{
    uint32_t i;

    MUX_AP_LOGI("channel_id = %d, data_buffer = 0x%x, buffer_length = %d, user_data = 0x%x, is_ip_data = %d", (int)channel_id, (unsigned int)data_buffer, (int)buffer_length, (unsigned int)user_data, (int)is_ip_data);

    if (data_buffer == NULL || buffer_length == 0) {
        MUX_AP_LOGE("MUX_AP_STATUS_INVALID_PARAMETER");
        return MUX_AP_STATUS_INVALID_PARAMETER;
    }

    for (i = 0; i < MUX_AP_CHANNEL_NUM; i++) {
        if (g_mux_ap_ptr->channel_manager[i].channel_id == channel_id) {
            if (!g_mux_ap_ptr->channel_manager[i].is_ready) {
                MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_READY");
                return MUX_AP_STATUS_CHANNEL_NOT_READY;
            }

            mux_ap_send_user_data(channel_id, data_buffer, buffer_length, g_mux_ap_ptr->channel_manager[i].user_callback, user_data, is_ip_data);

            return MUX_AP_STATUS_OK;
        }
    }

    MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_FOUND");

    return MUX_AP_STATUS_CHANNEL_NOT_FOUND;
}

mux_ap_status_t mux_ap_stop_to_receive(mux_ap_channel_id_t channel_id)
{
    uint32_t i;

    MUX_AP_LOGI("channel_id = %d", (int)channel_id);

    for (i = 0; i < MUX_AP_CHANNEL_NUM; i++) {
        if (g_mux_ap_ptr->channel_manager[i].channel_id == channel_id) {
            if (!g_mux_ap_ptr->channel_manager[i].is_ready) {
                MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_READY");
                return MUX_AP_STATUS_CHANNEL_NOT_READY;
            }

            xSemaphoreTake(g_mux_ap_ptr->mutex_handle, portMAX_DELAY);
            g_mux_ap_ptr->channel_manager[i].is_rx_flow_control = true;
            xSemaphoreGive(g_mux_ap_ptr->mutex_handle);
            mux_ap_send_line_status(channel_id, false);

            return MUX_AP_STATUS_OK;
        }
    }

    MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_FOUND");

    return MUX_AP_STATUS_CHANNEL_NOT_FOUND;
}

mux_ap_status_t mux_ap_resume_to_receive(mux_ap_channel_id_t channel_id)
{
    uint32_t i;

    MUX_AP_LOGI("channel_id = %d", (int)channel_id);

    for (i = 0; i < MUX_AP_CHANNEL_NUM; i++) {
        if (g_mux_ap_ptr->channel_manager[i].channel_id == channel_id) {
            if (!g_mux_ap_ptr->channel_manager[i].is_ready) {
                MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_READY");
                return MUX_AP_STATUS_CHANNEL_NOT_READY;
            }

            xSemaphoreTake(g_mux_ap_ptr->mutex_handle, portMAX_DELAY);
            g_mux_ap_ptr->channel_manager[i].is_rx_flow_control = false;
            xSemaphoreGive(g_mux_ap_ptr->mutex_handle);
            mux_ap_data_manager_resume_to_receive_from_receive_list(channel_id);

            return MUX_AP_STATUS_OK;
        }
    }

    MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_FOUND");

    return MUX_AP_STATUS_CHANNEL_NOT_FOUND;
}

void mux_ap_resume_to_send(void)
{
    uint32_t i;

    MUX_AP_LOGI("*MATWAKEUP is coming");

    if (g_mux_ap_ptr->mutex_handle == NULL) {
        g_mux_ap_ptr->mutex_handle = xSemaphoreCreateMutex();
        configASSERT(g_mux_ap_ptr->mutex_handle != NULL);
    }

    xSemaphoreTake(g_mux_ap_ptr->mutex_handle, portMAX_DELAY);

    for (i = 0; i < MUX_AP_CHANNEL_NUM; i++) {
        g_mux_ap_ptr->channel_manager[i].is_tx_flow_control = false;
    }

    xSemaphoreGive(g_mux_ap_ptr->mutex_handle);

    mux_ap_data_manager_resend();
}

void mux_ap_channel_manager_set_ready(mux_ap_channel_id_t channel_id, bool is_ready)
{
    uint32_t i;

    for (i = 0; i < MUX_AP_CHANNEL_NUM; i++) {
        if (g_mux_ap_ptr->channel_manager[i].channel_id == channel_id) {
            g_mux_ap_ptr->channel_manager[i].is_ready = is_ready;
            configASSERT(g_mux_ap_ptr->channel_manager[i].user_callback != NULL);
            if (is_ready) {
                mux_ap_event_channel_enabled_t channel_enabled;
                channel_enabled.channel_id = channel_id;
                channel_enabled.user_data = g_mux_ap_ptr->channel_manager[i].user_data;
                g_mux_ap_ptr->channel_manager[i].user_callback(MUX_AP_EVENT_CHANNEL_ENABLED, &channel_enabled);
            } else {
                mux_ap_event_channel_disabled_t channel_disabled;
                channel_disabled.channel_id = channel_id;
                channel_disabled.user_data = g_mux_ap_ptr->channel_manager[i].user_data;
                g_mux_ap_ptr->channel_manager[i].user_callback(MUX_AP_EVENT_CHANNEL_DISABLED, &channel_disabled);
            }
            return;
        }
    }

    MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_FOUND");
}

void mux_ap_data_manager_send_completed(mux_ap_channel_id_t channel_id, uint8_t *data_buffer, uint32_t buffer_length, void *user_data)
{
    uint32_t i;

    for (i = 0; i < MUX_AP_CHANNEL_NUM; i++) {
        if (g_mux_ap_ptr->channel_manager[i].channel_id == channel_id) {
            if (!g_mux_ap_ptr->channel_manager[i].is_ready) {
                MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_READY");
            }

            configASSERT(g_mux_ap_ptr->channel_manager[i].user_callback != NULL);
            if (1) {
                mux_ap_event_send_completed_t send_completed;
                send_completed.channel_id = channel_id;
                send_completed.data_buffer = data_buffer;
                send_completed.buffer_length = buffer_length;
                send_completed.user_data = user_data;
                g_mux_ap_ptr->channel_manager[i].user_callback(MUX_AP_EVENT_SEND_COMPLETED, &send_completed);
            }
            return;
        }
    }

    MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_FOUND");
}

void mux_ap_data_manager_resend(void)
{
    mux_ap_send_message(MUX_AP_MSG_RESEND, NULL);
}

bool mux_ap_channel_manager_is_stop_to_send(mux_ap_channel_id_t channel_id)
{
    uint32_t i;

    for (i = 0; i < MUX_AP_CHANNEL_NUM; i++) {
        if (g_mux_ap_ptr->channel_manager[i].channel_id == channel_id) {
            return g_mux_ap_ptr->channel_manager[i].is_tx_flow_control;
        }
    }

    MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_FOUND");
    return false;
}

void mux_ap_channel_manager_set_stop_to_send(mux_ap_channel_id_t channel_id, bool is_stop_to_send)
{
    uint32_t i;

    for (i = 0; i < MUX_AP_CHANNEL_NUM; i++) {
        if (g_mux_ap_ptr->channel_manager[i].channel_id == channel_id) {
            xSemaphoreTake(g_mux_ap_ptr->mutex_handle, portMAX_DELAY);
            g_mux_ap_ptr->channel_manager[i].is_tx_flow_control = is_stop_to_send;
            xSemaphoreGive(g_mux_ap_ptr->mutex_handle);
            configASSERT(g_mux_ap_ptr->channel_manager[i].user_callback != NULL);
            if (1) {
                mux_ap_event_flow_control_t flow_control;
                flow_control.channel_id = channel_id;
                flow_control.on_off = !is_stop_to_send;
                g_mux_ap_ptr->channel_manager[i].user_callback(MUX_AP_EVENT_FLOW_CONTROL, &flow_control);
            }
            return;
        }
    }

    MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_FOUND");
}

void mux_ap_data_manager_prepare_to_receive(mux_ap_event_prepare_to_receive_t *prepare_to_receive)
{
    uint32_t i;

    for (i = 0; i < MUX_AP_CHANNEL_NUM; i++) {
        if (g_mux_ap_ptr->channel_manager[i].channel_id == prepare_to_receive->channel_id) {
            if (!g_mux_ap_ptr->channel_manager[i].is_ready) {
                MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_READY");
            }

            configASSERT(g_mux_ap_ptr->channel_manager[i].user_callback != NULL);
            if (1) {
                g_mux_ap_ptr->channel_manager[i].user_callback(MUX_AP_EVENT_PREPARE_TO_RECEIVE, prepare_to_receive);
            }
            return;
        }
    }

    MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_FOUND");
}

void mux_ap_data_manager_receive_completed(mux_ap_channel_id_t channel_id, uint8_t *data_buffer, uint32_t buffer_length, void *user_data, mux_ap_callback_t user_callback)
{
    uint32_t i;

    for (i = 0; i < MUX_AP_CHANNEL_NUM; i++) {
        if (g_mux_ap_ptr->channel_manager[i].channel_id == channel_id) {
            if (!g_mux_ap_ptr->channel_manager[i].is_ready) {
                MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_READY");
            }

            configASSERT(user_callback != NULL);
            if (1) {
                mux_ap_event_receive_completed_t receive_completed;
                receive_completed.channel_id = channel_id;
                receive_completed.data_buffer = data_buffer;
                receive_completed.buffer_length = buffer_length;
                receive_completed.user_data = user_data;
                user_callback(MUX_AP_EVENT_RECEIVE_COMPLETED, &receive_completed);
            }
            return;
        }
    }

    MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_FOUND");
}

void mux_ap_data_manager_continue_to_receive(void)
{
    mux_ap_send_message(MUX_AP_MSG_RECEIVE_DATA, NULL);
}

bool mux_ap_data_manager_is_stop_to_receive(mux_ap_channel_id_t channel_id)
{
    uint32_t i;

    for (i = 0; i < MUX_AP_CHANNEL_NUM; i++) {
        if (g_mux_ap_ptr->channel_manager[i].channel_id == channel_id) {
            return g_mux_ap_ptr->channel_manager[i].is_rx_flow_control;
        }
    }

    MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_FOUND");
    return false;
}

void mux_ap_data_manager_resume_to_receive_from_receive_list(mux_ap_channel_id_t channel_id)
{
    mux_ap_resume_to_receive_t *resume_to_receive = (mux_ap_resume_to_receive_t *)pvPortMalloc(sizeof(mux_ap_resume_to_receive_t));

    configASSERT(resume_to_receive != NULL);
    resume_to_receive->channel_id = channel_id;

    mux_ap_send_message(MUX_AP_MSG_RECEIVE_DATA_FROM_RECEIVE_LIST, resume_to_receive);
}

void mux_ap_data_manager_clear_is_receiving_data(void)
{
    g_mux_ap_ptr->is_receiving_data = false;
}

mux_ap_callback_t mux_ap_data_manager_get_user_callback(mux_ap_channel_id_t channel_id)
{
    uint32_t i;

    for (i = 0; i < MUX_AP_CHANNEL_NUM; i++) {
        if (g_mux_ap_ptr->channel_manager[i].channel_id == channel_id) {
            return g_mux_ap_ptr->channel_manager[i].user_callback;
        }
    }

    MUX_AP_LOGE("MUX_AP_STATUS_CHANNEL_NOT_FOUND");
    return NULL;
}

void mux_ap_sleep_manager_lock_sleep(uint8_t lock_category)
{
#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_mux_ap_ptr->lock_category |= lock_category;
    MUX_AP_LOGI("sleep_handle = 0x%x, lock_category = %d, is_locking = %d", g_mux_ap_ptr->sleep_handle, g_mux_ap_ptr->lock_category, g_mux_ap_ptr->is_locking);

    if (g_mux_ap_ptr->is_locking == false) {
        g_mux_ap_ptr->is_locking = true;
        MUX_AP_LOGI("hal_sleep_manager_acquire_sleeplock()");
        hal_sleep_manager_acquire_sleeplock(g_mux_ap_ptr->sleep_handle, HAL_SLEEP_LOCK_DEEP);
    }
#endif
}

void mux_ap_sleep_manager_unlock_sleep(uint8_t lock_category)
{
#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_mux_ap_ptr->lock_category &= ~lock_category;
    MUX_AP_LOGI("sleep_handle = 0x%x, lock_category = %d, is_locking = %d", g_mux_ap_ptr->sleep_handle, g_mux_ap_ptr->lock_category, g_mux_ap_ptr->is_locking);

    if (g_mux_ap_ptr->lock_category != 0) {
        MUX_AP_LOGW("lock_category != 0");
        return;
    }

    if (g_mux_ap_ptr->is_locking == true) {
        g_mux_ap_ptr->is_locking = false;
        MUX_AP_LOGI("hal_sleep_manager_release_sleeplock()");
        hal_sleep_manager_release_sleeplock(g_mux_ap_ptr->sleep_handle, HAL_SLEEP_LOCK_DEEP);
    }
#endif
}

