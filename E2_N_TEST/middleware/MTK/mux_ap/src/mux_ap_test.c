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
#include "syslog.h"
#include "mux_ap.h"

#ifdef MUX_AP_UT

log_create_module(mux_ap_test, PRINT_LEVEL_INFO);

#if 0
#define MUX_AP_TEST_LOGI(fmt, args...)   LOG_I(mux_ap_test, "[MUX_AP_TEST] "fmt, ##args)
#else
#define MUX_AP_TEST_LOGI(fmt, args...)   printf("[MUX_AP_TEST] "fmt"\r\n", ##args)
#endif

#define MUX_AP_TEST_TASK_NAME           "MUX_AP_TEST"
#define MUX_AP_TEST_TASK_STACK_SIZE     (1024)
#define MUX_AP_TEST_TASK_PRIORITY       (TASK_PRIORITY_NORMAL)

#define MUX_AP_TEST_QUEUE_LENGTH        (10)
#define MUX_AP_TEST_AT_STRING_LEN       (256)
#define MUX_AP_TEST_DATA_PACKET_LEN     (1500)

#define MUX_AP_TEST_TASK_NAME2          "MUX_AP_TEST2"
#define MUX_AP_TEST_TASK_STACK_SIZE2    (1024)
#define MUX_AP_TEST_TASK_PRIORITY2      (TASK_PRIORITY_HIGH)

#define MUX_AP_TEST_QUEUE_LENGTH2       (10)

#define MUX_AP_TEST_USER_DATA           (0x10000000)
#define MUX_AP_TEST_TX_USER_DATA        (0x20000000)
#define MUX_AP_TEST_RX_USER_DATA        (0x30000000)

typedef struct {
    TaskHandle_t task_handle;
    QueueHandle_t queue_handle;
} mux_ap_test_context_t;

static mux_ap_test_context_t g_mux_ap_test_context;
static mux_ap_test_context_t *const g_mux_ap_test_ptr = &g_mux_ap_test_context;

static mux_ap_test_context_t g_mux_ap_test_context2;
static mux_ap_test_context_t *const g_mux_ap_test_ptr2 = &g_mux_ap_test_context2;

typedef enum {
    MUX_AP_TEST_MSG_CHANNEL_ENABLED,
    MUX_AP_TEST_MSG_CHANNEL_DISABLED,
    MUX_AP_TEST_MSG_SEND_COMPLETED,
    MUX_AP_TEST_MSG_RECEIVE_COMPLETED,
    MUX_AP_TEST_MSG_MAX
} mux_ap_test_msg_t;

typedef struct {
    mux_ap_test_msg_t msg_id;
    void *param;
} mux_ap_test_message_t;

static bool mux_ap_test_send_message(mux_ap_test_msg_t msg_id, void *param)
{
    mux_ap_test_message_t message;

    message.msg_id = msg_id;
    message.param = param;

    if (xQueueSend(g_mux_ap_test_ptr->queue_handle, &message, portMAX_DELAY) != pdPASS) {
        configASSERT(0);
        return false;
    }

    return true;
}

static bool mux_ap_test_send_message2(mux_ap_test_msg_t msg_id, void *param)
{
    mux_ap_test_message_t message;

    message.msg_id = msg_id;
    message.param = param;

    if (xQueueSend(g_mux_ap_test_ptr2->queue_handle, &message, portMAX_DELAY) != pdPASS) {
        configASSERT(0);
        return false;
    }

    return true;
}

static void mux_ap_test_callback(mux_ap_event_t event_id, void *param)
{
    switch (event_id) {
        case MUX_AP_EVENT_CHANNEL_ENABLED: {
            mux_ap_event_channel_enabled_t *channel_enabled = (mux_ap_event_channel_enabled_t *)pvPortMalloc(sizeof(mux_ap_event_channel_enabled_t));
            memcpy(channel_enabled, param, sizeof(mux_ap_event_channel_enabled_t));
            mux_ap_test_send_message(MUX_AP_TEST_MSG_CHANNEL_ENABLED, channel_enabled);
            break;
        }

        case MUX_AP_EVENT_CHANNEL_DISABLED: {
            mux_ap_event_channel_disabled_t *channel_disabled = (mux_ap_event_channel_disabled_t *)pvPortMalloc(sizeof(mux_ap_event_channel_disabled_t));
            memcpy(channel_disabled, param, sizeof(mux_ap_event_channel_disabled_t));
            mux_ap_test_send_message(MUX_AP_TEST_MSG_CHANNEL_DISABLED, channel_disabled);
            break;
        }

        case MUX_AP_EVENT_SEND_COMPLETED: {
            mux_ap_event_send_completed_t *send_completed = (mux_ap_event_send_completed_t *)pvPortMalloc(sizeof(mux_ap_event_send_completed_t));
            memcpy(send_completed, param, sizeof(mux_ap_event_send_completed_t));
            mux_ap_test_send_message(MUX_AP_TEST_MSG_SEND_COMPLETED, send_completed);
            break;
        }

        case MUX_AP_EVENT_PREPARE_TO_RECEIVE: {
            mux_ap_event_prepare_to_receive_t *prepare_to_receive = (mux_ap_event_prepare_to_receive_t *)param;
            if (1) {
                prepare_to_receive->data_buffer = pvPortMalloc(prepare_to_receive->buffer_length + 1);
                prepare_to_receive->user_data = (void *)(MUX_AP_TEST_RX_USER_DATA + prepare_to_receive->channel_id);
            }
            break;
        }

        case MUX_AP_EVENT_RECEIVE_COMPLETED: {
            mux_ap_event_receive_completed_t *receive_completed = (mux_ap_event_receive_completed_t *)pvPortMalloc(sizeof(mux_ap_event_receive_completed_t));
            memcpy(receive_completed, param, sizeof(mux_ap_event_receive_completed_t));
            mux_ap_test_send_message(MUX_AP_TEST_MSG_RECEIVE_COMPLETED, receive_completed);
            break;
        }

        default:
            break;
    }
}

static void mux_ap_test_callback2(mux_ap_event_t event_id, void *param)
{
    switch (event_id) {
        case MUX_AP_EVENT_CHANNEL_ENABLED: {
            mux_ap_event_channel_enabled_t *channel_enabled = (mux_ap_event_channel_enabled_t *)pvPortMalloc(sizeof(mux_ap_event_channel_enabled_t));
            memcpy(channel_enabled, param, sizeof(mux_ap_event_channel_enabled_t));
            mux_ap_test_send_message2(MUX_AP_TEST_MSG_CHANNEL_ENABLED, channel_enabled);
            break;
        }

        case MUX_AP_EVENT_CHANNEL_DISABLED: {
            mux_ap_event_channel_disabled_t *channel_disabled = (mux_ap_event_channel_disabled_t *)pvPortMalloc(sizeof(mux_ap_event_channel_disabled_t));
            memcpy(channel_disabled, param, sizeof(mux_ap_event_channel_disabled_t));
            mux_ap_test_send_message2(MUX_AP_TEST_MSG_CHANNEL_DISABLED, channel_disabled);
            break;
        }

        case MUX_AP_EVENT_SEND_COMPLETED: {
            mux_ap_event_send_completed_t *send_completed = (mux_ap_event_send_completed_t *)pvPortMalloc(sizeof(mux_ap_event_send_completed_t));
            memcpy(send_completed, param, sizeof(mux_ap_event_send_completed_t));
            mux_ap_test_send_message2(MUX_AP_TEST_MSG_SEND_COMPLETED, send_completed);
            break;
        }

        case MUX_AP_EVENT_PREPARE_TO_RECEIVE: {
            mux_ap_event_prepare_to_receive_t *prepare_to_receive = (mux_ap_event_prepare_to_receive_t *)param;
            if (1) {
                prepare_to_receive->data_buffer = pvPortMalloc(prepare_to_receive->buffer_length + 1);
                prepare_to_receive->user_data = (void *)(MUX_AP_TEST_RX_USER_DATA + prepare_to_receive->channel_id);
            }
            break;
        }

        case MUX_AP_EVENT_RECEIVE_COMPLETED: {
            mux_ap_event_receive_completed_t *receive_completed = (mux_ap_event_receive_completed_t *)pvPortMalloc(sizeof(mux_ap_event_receive_completed_t));
            memcpy(receive_completed, param, sizeof(mux_ap_event_receive_completed_t));
            mux_ap_test_send_message2(MUX_AP_TEST_MSG_RECEIVE_COMPLETED, receive_completed);
            break;
        }

        default:
            break;
    }
}

#include "hal_gpt.h"

static unsigned int mux_ap_test_get_log_time_in_ms(void)
{
    uint32_t count = 0;
    uint64_t count64 = 0;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &count);
    count64 = ((uint64_t)count) * 1000 / 32768;
    return (unsigned int)count64;
}

static void mux_ap_test_message_processing(void *arg)
{
    mux_ap_test_message_t message;

    while (1) {
        if (xQueueReceive(g_mux_ap_test_ptr->queue_handle, &message, portMAX_DELAY)) {
            MUX_AP_TEST_LOGI("msg_id = %d", message.msg_id);

            switch (message.msg_id) {
                case MUX_AP_TEST_MSG_CHANNEL_ENABLED: {
                    mux_ap_event_channel_enabled_t *channel_enabled = (mux_ap_event_channel_enabled_t *)message.param;
                    MUX_AP_TEST_LOGI("channel_id = %d, user_data = 0x%x", (int)channel_enabled->channel_id, (unsigned int)channel_enabled->user_data);
                    if (1) {
                        char *data_buffer = pvPortMalloc(MUX_AP_TEST_AT_STRING_LEN);
                        data_buffer[0] = 'A';
                        data_buffer[1] = 'T';
                        data_buffer[2] = '+';
                        data_buffer[3] = 'T';
                        data_buffer[4] = 'E';
                        data_buffer[5] = 'S';
                        data_buffer[6] = 'T';
                        data_buffer[7] = 'C';
                        data_buffer[8] = 'M';
                        data_buffer[9] = 'D';
                        data_buffer[10] = '=';
                        data_buffer[11] = '\"';
                        memset(data_buffer + 12, 0x30 + channel_enabled->channel_id, 85);
                        data_buffer[97] = '\"';
                        data_buffer[98] = '\r';
                        data_buffer[99] = '\0';
                        MUX_AP_TEST_LOGI("#%d send time = %d", (int)channel_enabled->channel_id, (int)mux_ap_test_get_log_time_in_ms());
                        mux_ap_send_data(channel_enabled->channel_id, (const uint8_t *)data_buffer, 100, (void *)(MUX_AP_TEST_TX_USER_DATA + channel_enabled->channel_id));
                    }
                }
                break;

                case MUX_AP_TEST_MSG_CHANNEL_DISABLED: {
                    mux_ap_event_channel_disabled_t *channel_disabled = (mux_ap_event_channel_disabled_t *)message.param;
                    MUX_AP_TEST_LOGI("channel_id = %d, user_data = 0x%x", (int)channel_disabled->channel_id, (unsigned int)channel_disabled->user_data);
                }
                break;

                case MUX_AP_TEST_MSG_SEND_COMPLETED: {
                    mux_ap_event_send_completed_t *send_completed = (mux_ap_event_send_completed_t *)message.param;
                    MUX_AP_TEST_LOGI("channel_id = %d, data_buffer = 0x%x, user_data = 0x%x", (int)send_completed->channel_id, (unsigned int)send_completed->data_buffer, (unsigned int)send_completed->user_data);
                    MUX_AP_TEST_LOGI("#%d send completed time = %d", (int)send_completed->channel_id, (int)mux_ap_test_get_log_time_in_ms());
                    if (1) {
                        MUX_AP_TEST_LOGI("#%d send AT request [%d] completed", (int)send_completed->channel_id, (int)send_completed->buffer_length);
                        vPortFree(send_completed->data_buffer);
                    }
                }
                break;

                case MUX_AP_TEST_MSG_RECEIVE_COMPLETED: {
                    mux_ap_event_receive_completed_t *receive_completed = (mux_ap_event_receive_completed_t *)message.param;
                    MUX_AP_TEST_LOGI("channel_id = %d, data_buffer = 0x%x, buffer_length = %d, user_data = 0x%x", (int)receive_completed->channel_id, (unsigned int)receive_completed->data_buffer, (int)receive_completed->buffer_length, (unsigned int)receive_completed->user_data);
                    MUX_AP_TEST_LOGI("#%d receive completed time = %d", (int)receive_completed->channel_id, (int)mux_ap_test_get_log_time_in_ms());
                    if (1) {
                        receive_completed->data_buffer[receive_completed->buffer_length] = '\0';
                        MUX_AP_TEST_LOGI("#%d receive AT response [%d] completed", (int)receive_completed->channel_id, (int)receive_completed->buffer_length);
                        if (receive_completed->buffer_length < 100) {
                            MUX_AP_TEST_LOGI("#%d receive AT response [\"%s\"] completed", (int)receive_completed->channel_id, receive_completed->data_buffer);
                        }
                        vPortFree(receive_completed->data_buffer);
                    }
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

static void mux_ap_test_message_processing2(void *arg)
{
    mux_ap_test_message_t message;

    while (1) {
        if (xQueueReceive(g_mux_ap_test_ptr2->queue_handle, &message, portMAX_DELAY)) {
            MUX_AP_TEST_LOGI("msg_id = %d", message.msg_id);

            switch (message.msg_id) {
                case MUX_AP_TEST_MSG_CHANNEL_ENABLED: {
                    uint32_t i;
                    mux_ap_event_channel_enabled_t *channel_enabled = (mux_ap_event_channel_enabled_t *)message.param;
                    MUX_AP_TEST_LOGI("channel_id = %d, user_data = 0x%x", (int)channel_enabled->channel_id, (unsigned int)channel_enabled->user_data);
                    for (i = 0; i < 2; i++) {
                        char *data_buffer = pvPortMalloc(MUX_AP_TEST_DATA_PACKET_LEN);
                        data_buffer[0] = 0x30 + i;
                        memset(data_buffer + 1, 0x30 + channel_enabled->channel_id, 1023);
                        MUX_AP_TEST_LOGI("#%d send time = %d", (int)channel_enabled->channel_id, (int)mux_ap_test_get_log_time_in_ms());
                        mux_ap_send_data(channel_enabled->channel_id, (const uint8_t *)data_buffer, 1024, (void *)(MUX_AP_TEST_TX_USER_DATA + channel_enabled->channel_id));
                    }
                }
                break;

                case MUX_AP_TEST_MSG_CHANNEL_DISABLED: {
                    mux_ap_event_channel_disabled_t *channel_disabled = (mux_ap_event_channel_disabled_t *)message.param;
                    MUX_AP_TEST_LOGI("channel_id = %d, user_data = 0x%x", (int)channel_disabled->channel_id, (unsigned int)channel_disabled->user_data);
                }
                break;

                case MUX_AP_TEST_MSG_SEND_COMPLETED: {
                    mux_ap_event_send_completed_t *send_completed = (mux_ap_event_send_completed_t *)message.param;
                    MUX_AP_TEST_LOGI("channel_id = %d, data_buffer = 0x%x, user_data = 0x%x", (int)send_completed->channel_id, (unsigned int)send_completed->data_buffer, (unsigned int)send_completed->user_data);
                    MUX_AP_TEST_LOGI("#%d send completed time = %d", (int)send_completed->channel_id, (int)mux_ap_test_get_log_time_in_ms());
                    if (1) {
                        MUX_AP_TEST_LOGI("#%d send AT request [%d] completed", (int)send_completed->channel_id, (int)send_completed->buffer_length);
                        vPortFree(send_completed->data_buffer);
                    }
                }
                break;

                case MUX_AP_TEST_MSG_RECEIVE_COMPLETED: {
                    mux_ap_event_receive_completed_t *receive_completed = (mux_ap_event_receive_completed_t *)message.param;
                    MUX_AP_TEST_LOGI("channel_id = %d, data_buffer = 0x%x, buffer_length = %d, user_data = 0x%x", (int)receive_completed->channel_id, (unsigned int)receive_completed->data_buffer, (int)receive_completed->buffer_length, (unsigned int)receive_completed->user_data);
                    MUX_AP_TEST_LOGI("#%d receive completed time = %d", (int)receive_completed->channel_id, (int)mux_ap_test_get_log_time_in_ms());
                    if (1) {
                        receive_completed->data_buffer[receive_completed->buffer_length] = '\0';
                        MUX_AP_TEST_LOGI("#%d receive AT response [%d] completed", (int)receive_completed->channel_id, (int)receive_completed->buffer_length);
                        if (receive_completed->buffer_length < 100) {
                            MUX_AP_TEST_LOGI("#%d receive AT response [\"%s\"] completed", (int)receive_completed->channel_id, receive_completed->data_buffer);
                        }
                        vPortFree(receive_completed->data_buffer);
                    }
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

void mux_ap_test_init2(void)
{
    mux_ap_status_t status = mux_ap_register_callback(MUX_AP_CHANNEL_TYPE_SIMAT, mux_ap_test_callback2, (void *)MUX_AP_TEST_USER_DATA);
    configASSERT(status == MUX_AP_STATUS_OK);

    if (g_mux_ap_test_ptr2->queue_handle == NULL) {
        g_mux_ap_test_ptr2->queue_handle = xQueueCreate(MUX_AP_TEST_QUEUE_LENGTH2, sizeof(mux_ap_test_message_t));
        configASSERT(g_mux_ap_test_ptr2->queue_handle != NULL);
    }

    if (g_mux_ap_test_ptr2->task_handle == NULL) {
        xTaskCreate(mux_ap_test_message_processing2,
                    MUX_AP_TEST_TASK_NAME2,
                    MUX_AP_TEST_TASK_STACK_SIZE2 / sizeof(portSTACK_TYPE),
                    NULL,
                    MUX_AP_TEST_TASK_PRIORITY2,
                    &g_mux_ap_test_ptr2->task_handle);
        configASSERT(g_mux_ap_test_ptr2->task_handle != NULL);
    }
}

void mux_ap_test_init(void)
{
    uint32_t i;
    mux_ap_status_t status;

    for (i = 0; i < MUX_AP_AT_AND_DATA_CHANNEL_NUM; i++) {
        status = mux_ap_register_callback(MUX_AP_CHANNEL_TYPE_AT_AND_DATA, mux_ap_test_callback, (void *)(MUX_AP_TEST_USER_DATA + i));
        configASSERT(status == MUX_AP_STATUS_OK);
    }

    if (g_mux_ap_test_ptr->queue_handle == NULL) {
        g_mux_ap_test_ptr->queue_handle = xQueueCreate(MUX_AP_TEST_QUEUE_LENGTH, sizeof(mux_ap_test_message_t));
        configASSERT(g_mux_ap_test_ptr->queue_handle != NULL);
    }

    if (g_mux_ap_test_ptr->task_handle == NULL) {
        xTaskCreate(mux_ap_test_message_processing,
                    MUX_AP_TEST_TASK_NAME,
                    MUX_AP_TEST_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                    NULL,
                    MUX_AP_TEST_TASK_PRIORITY,
                    &g_mux_ap_test_ptr->task_handle);
        configASSERT(g_mux_ap_test_ptr->task_handle != NULL);
    }

    mux_ap_test_init2();
}

#endif /* MUX_AP_UT */

