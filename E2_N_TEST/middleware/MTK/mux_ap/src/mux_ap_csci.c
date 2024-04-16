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
#include "syslog.h"
#include "mux_ap.h"
#include "mux_ap_private.h"
#include "csci.h"
#include "csci_message.h"
#include "hal_nvic_internal.h"

#define MUX_AP_CSCI_BLOCK_SIZE          (64)
#define MUX_AP_CSCI_QUEUE_LENGTH        (48)

#define MUX_AP_CSCI_CHANNEL_ENABLE_REQUEST_MESSAGE_HEADER_SIZE  (16)
#define MUX_AP_CSCI_CHANNEL_ENABLE_CONFIRM_MESSAGE_HEADER_SIZE  (12)
#define MUX_AP_CSCI_CHANNEL_DISABLED_MESSAGE_HEADER_SIZE        (12)
#define MUX_AP_CSCI_LINE_STATUS_MESSAGE_HEADER_SIZE             (36)
#define MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE               (12)

#define MUX_AP_CSCI_MESSAGE_HEADER \
    uint32_t length; \
    uint32_t channel; \
    uint32_t type;

typedef struct {
    MUX_AP_CSCI_MESSAGE_HEADER
} mux_ap_csci_generic_message_t;

typedef struct {
    MUX_AP_CSCI_MESSAGE_HEADER
    uint32_t channel_type;
} mux_ap_csci_channel_enable_request_t;

typedef struct {
    MUX_AP_CSCI_MESSAGE_HEADER
} mux_ap_csci_channel_enable_confirm_t;

typedef struct {
    MUX_AP_CSCI_MESSAGE_HEADER
} mux_ap_csci_channel_disabled_t;

typedef struct {
    MUX_AP_CSCI_MESSAGE_HEADER
    uint32_t dtr;
    uint32_t dsr;
    uint32_t rts;
    uint32_t cts;
    uint32_t dcd;
    uint32_t ri;
} mux_ap_csci_line_status_t;

typedef struct {
    MUX_AP_CSCI_MESSAGE_HEADER
    uint8_t packet[1];
} mux_ap_csci_user_data_t;

typedef struct mux_ap_csci_data_node_t {
    uint32_t channel;
    union {
        struct {
            uint32_t channel_type;
        } data_type1;
        struct {
            uint32_t rts;
        } data_type2;
        struct {
            uint8_t *packet;
            uint32_t length;
            mux_ap_callback_t user_callback;
            void *user_data;
            bool is_ip_data;
        } data_type3;
    } node_data;
    uint32_t node_type;
    struct mux_ap_csci_data_node_t *next;
} mux_ap_csci_data_node_t;

static mux_ap_csci_data_node_t *g_mux_ap_csci_resend_list;
static uint32_t g_mux_ap_csci_resend_number;

static bool mux_ap_csci_resend_list_is_empty(void)
{
    if (g_mux_ap_csci_resend_list == NULL) {
        return true;
    } else {
        return false;
    }
}

static void mux_ap_csci_add_to_resend_list(mux_ap_csci_data_node_t *data_node)
{
    if (g_mux_ap_csci_resend_list == NULL) {
        g_mux_ap_csci_resend_list = data_node;
    } else {
        mux_ap_csci_data_node_t *traverse = g_mux_ap_csci_resend_list;
        while (traverse->next != NULL) {
            traverse = traverse->next;
        }
        traverse->next = data_node;
    }

    mux_ap_sleep_manager_lock_sleep(MUX_AP_SLEEP_MANAGER_LOCK_CATEGORY_RESEND_LIST);
}

static void mux_ap_csci_add_channel_enable_request_to_resend_list(uint32_t channel, uint32_t channel_type)
{
    mux_ap_csci_data_node_t *data_node;
    data_node = (mux_ap_csci_data_node_t *)pvPortMalloc(sizeof(mux_ap_csci_data_node_t));
    configASSERT(data_node != NULL);
    data_node->channel = channel;
    data_node->node_data.data_type1.channel_type = channel_type;
    data_node->node_type = CSCI_MESSAGE_TYPE_CHANNEL_ENABLE_REQUEST;
    data_node->next = NULL;
    g_mux_ap_csci_resend_number++;
    MUX_AP_LOGI("mux_ap_csci_add_to_resend_list(1) = %d", g_mux_ap_csci_resend_number);
    mux_ap_csci_add_to_resend_list(data_node);
}

static void mux_ap_csci_add_line_status_to_resend_list(uint32_t channel, uint32_t rts)
{
    mux_ap_csci_data_node_t *data_node;
    data_node = (mux_ap_csci_data_node_t *)pvPortMalloc(sizeof(mux_ap_csci_data_node_t));
    configASSERT(data_node != NULL);
    data_node->channel = channel;
    data_node->node_data.data_type2.rts = rts;
    data_node->node_type = CSCI_MESSAGE_TYPE_LINE_STATUS;
    data_node->next = NULL;
    g_mux_ap_csci_resend_number++;
    MUX_AP_LOGI("mux_ap_csci_add_to_resend_list(2) = %d", g_mux_ap_csci_resend_number);
    mux_ap_csci_add_to_resend_list(data_node);
}

static void mux_ap_csci_add_user_data_to_resend_list(uint32_t channel, uint8_t *packet, uint32_t length, mux_ap_callback_t user_callback, void *user_data, bool is_ip_data)
{
    mux_ap_csci_data_node_t *data_node;
    data_node = (mux_ap_csci_data_node_t *)pvPortMalloc(sizeof(mux_ap_csci_data_node_t));
    configASSERT(data_node != NULL);
    data_node->channel = channel;
    data_node->node_data.data_type3.packet = packet;
    data_node->node_data.data_type3.length = length;
    data_node->node_data.data_type3.user_callback = user_callback;
    data_node->node_data.data_type3.user_data = user_data;
    data_node->node_data.data_type3.is_ip_data = is_ip_data;
    if (is_ip_data) {
        data_node->node_type = CSCI_MESSAGE_TYPE_USER_DATA_IP;
    } else {
        data_node->node_type = CSCI_MESSAGE_TYPE_USER_DATA;
    }
    data_node->next = NULL;
    g_mux_ap_csci_resend_number++;
    MUX_AP_LOGI("mux_ap_csci_add_to_resend_list(3) = %d", g_mux_ap_csci_resend_number);
    mux_ap_csci_add_to_resend_list(data_node);
}

static void mux_ap_csci_remove_channel_enable_request_from_resend_list(uint32_t channel, uint32_t channel_type)
{
    mux_ap_csci_data_node_t *traverse = g_mux_ap_csci_resend_list;
    mux_ap_csci_data_node_t *previous = NULL;
    while (traverse != NULL) {
        if (traverse->channel == channel &&
                traverse->node_type == CSCI_MESSAGE_TYPE_CHANNEL_ENABLE_REQUEST &&
                traverse->node_data.data_type1.channel_type == channel_type) {
            if (previous == NULL) {
                g_mux_ap_csci_resend_list = traverse->next;
            } else {
                previous->next = traverse->next;
            }
            vPortFree(traverse);
            g_mux_ap_csci_resend_number--;
            MUX_AP_LOGI("mux_ap_csci_remove_from_resend_list(1) = %d", g_mux_ap_csci_resend_number);
            break;
        }
        previous = traverse;
        traverse = traverse->next;
    }
}

static void mux_ap_csci_remove_line_status_from_resend_list(uint32_t channel, uint32_t rts)
{
    mux_ap_csci_data_node_t *traverse = g_mux_ap_csci_resend_list;
    mux_ap_csci_data_node_t *previous = NULL;
    while (traverse != NULL) {
        if (traverse->channel == channel &&
                traverse->node_type == CSCI_MESSAGE_TYPE_LINE_STATUS &&
                traverse->node_data.data_type2.rts == rts) {
            if (previous == NULL) {
                g_mux_ap_csci_resend_list = traverse->next;
            } else {
                previous->next = traverse->next;
            }
            vPortFree(traverse);
            g_mux_ap_csci_resend_number--;
            MUX_AP_LOGI("mux_ap_csci_remove_from_resend_list(2) = %d", g_mux_ap_csci_resend_number);
            break;
        }
        previous = traverse;
        traverse = traverse->next;
    }
}

static void mux_ap_csci_remove_user_data_from_resend_list(uint32_t channel, uint8_t *packet, uint32_t length, void *user_data)
{
    mux_ap_csci_data_node_t *traverse = g_mux_ap_csci_resend_list;
    mux_ap_csci_data_node_t *previous = NULL;
    while (traverse != NULL) {
        if (traverse->channel == channel &&
                (traverse->node_type == CSCI_MESSAGE_TYPE_USER_DATA || traverse->node_type == CSCI_MESSAGE_TYPE_USER_DATA_IP) &&
                traverse->node_data.data_type3.packet == packet &&
                traverse->node_data.data_type3.length == length &&
                traverse->node_data.data_type3.user_data == user_data) {
            if (previous == NULL) {
                g_mux_ap_csci_resend_list = traverse->next;
            } else {
                previous->next = traverse->next;
            }
            vPortFree(traverse);
            g_mux_ap_csci_resend_number--;
            MUX_AP_LOGI("mux_ap_csci_remove_from_resend_list(3) = %d", g_mux_ap_csci_resend_number);
            break;
        }
        previous = traverse;
        traverse = traverse->next;
    }
}

static mux_ap_csci_data_node_t *mux_ap_csci_get_resend_list(void)
{
    return g_mux_ap_csci_resend_list;
}

void mux_ap_csci_init(void)
{
    csci_status_t status = csci_init(MUX_AP_CSCI_BLOCK_SIZE, MUX_AP_CSCI_QUEUE_LENGTH);
    MUX_AP_LOGI("csci_init() = %d", (int)status);
    configASSERT(status == CSCI_STATUS_OK);
}

void mux_ap_csci_send_channel_enable_request(uint32_t channel, uint32_t channel_type, bool resend)
{
    mux_ap_csci_channel_enable_request_t *channel_enable_request;
    uint32_t block_length, block_num;
    csci_status_t status;

    MUX_AP_LOGI("[TX -> CHANNEL_ENABLE_REQUEST] channel = %d, channel_type = %d", (int)channel, (int)channel_type);

    block_length = MUX_AP_CSCI_CHANNEL_ENABLE_REQUEST_MESSAGE_HEADER_SIZE;
    channel_enable_request = (mux_ap_csci_channel_enable_request_t *)pvPortMalloc(block_length);

    configASSERT(channel_enable_request != NULL);
    channel_enable_request->length = block_length;
    channel_enable_request->channel = channel;
    channel_enable_request->type = CSCI_MESSAGE_TYPE_CHANNEL_ENABLE_REQUEST;
    channel_enable_request->channel_type = channel_type;

    if (block_length % MUX_AP_CSCI_BLOCK_SIZE == 0) {
        block_num = block_length / MUX_AP_CSCI_BLOCK_SIZE;
    } else {
        block_num = block_length / MUX_AP_CSCI_BLOCK_SIZE + 1;
    }

    status = csci_ap_send_blocks_to_md((const uint8_t *)channel_enable_request, block_num);
    if (status == CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_SPACE) {
        if (!resend) {
            mux_ap_csci_add_channel_enable_request_to_resend_list(channel, channel_type);
        }
        vPortFree(channel_enable_request);
        MUX_AP_LOGE("csci_ap_send_blocks_to_md() == CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_SPACE");
        return;
    }

    configASSERT(status == CSCI_STATUS_OK);
    csci_ap_indicate_data_available_to_md();

    if (resend) {
        mux_ap_csci_remove_channel_enable_request_from_resend_list(channel, channel_type);
    }

    vPortFree(channel_enable_request);
}

void mux_ap_csci_send_line_status(uint32_t channel, uint32_t rts, bool resend)
{
    mux_ap_csci_line_status_t *line_status;
    uint32_t block_length, block_num;
    csci_status_t status;

    MUX_AP_LOGI("[TX -> LINE_STATUS] channel = %d, rts = %d", (int)channel, (int)rts);

    block_length = MUX_AP_CSCI_LINE_STATUS_MESSAGE_HEADER_SIZE;
    line_status = (mux_ap_csci_line_status_t *)pvPortMalloc(block_length);

    configASSERT(line_status != NULL);
    line_status->length = block_length;
    line_status->channel = channel;
    line_status->type = CSCI_MESSAGE_TYPE_LINE_STATUS;
    line_status->dtr = CSCI_CHANNEL_LINE_ON;
    line_status->rts = rts;

    if (block_length % MUX_AP_CSCI_BLOCK_SIZE == 0) {
        block_num = block_length / MUX_AP_CSCI_BLOCK_SIZE;
    } else {
        block_num = block_length / MUX_AP_CSCI_BLOCK_SIZE + 1;
    }

    status = csci_ap_send_blocks_to_md((const uint8_t *)line_status, block_num);
    if (status == CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_SPACE) {
        if (!resend) {
            mux_ap_csci_add_line_status_to_resend_list(channel, rts);
        }
        vPortFree(line_status);
        MUX_AP_LOGE("csci_ap_send_blocks_to_md() == CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_SPACE");
        return;
    }

    configASSERT(status == CSCI_STATUS_OK);
    csci_ap_indicate_data_available_to_md();

    if (resend) {
        mux_ap_csci_remove_line_status_from_resend_list(channel, rts);
    }

    vPortFree(line_status);
}

void mux_ap_csci_send_user_data(uint32_t channel, uint8_t *packet, uint32_t length, mux_ap_callback_t user_callback, void *user_data, bool is_ip_data, bool resend)
{
    mux_ap_csci_user_data_t *send_data;
    uint32_t block_length, block_num;
    csci_status_t status;

    configASSERT(packet != NULL && length != 0);

    if (mux_ap_channel_manager_is_stop_to_send(channel)) {
        if (!resend) {
            mux_ap_csci_add_user_data_to_resend_list(channel, packet, length, user_callback, user_data, is_ip_data);
        }
        MUX_AP_LOGI("mux_ap_channel_manager_is_stop_to_send() == true");
        return;
    }

    MUX_AP_LOGI("[TX -> USER_DATA] channel = %d, packet = 0x%x, length = %d, user_data = 0x%x, is_ip_data = %d", (int)channel, (unsigned int)packet, (int)length, (unsigned int)user_data, (int)is_ip_data);

    block_length = MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE + length;
    send_data = (mux_ap_csci_user_data_t *)pvPortMalloc(block_length);

    configASSERT(send_data != NULL);
    send_data->length = block_length;
    send_data->channel = channel;
    if (is_ip_data) {
        send_data->type = CSCI_MESSAGE_TYPE_USER_DATA_IP;
    } else {
        send_data->type = CSCI_MESSAGE_TYPE_USER_DATA;
    }
    memcpy(send_data->packet, packet, length);

    if (block_length % MUX_AP_CSCI_BLOCK_SIZE == 0) {
        block_num = block_length / MUX_AP_CSCI_BLOCK_SIZE;
    } else {
        block_num = block_length / MUX_AP_CSCI_BLOCK_SIZE + 1;
    }

    status = csci_ap_send_blocks_to_md((const uint8_t *)send_data, block_num);
    if (status == CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_SPACE) {
        if (!resend) {
            mux_ap_csci_add_user_data_to_resend_list(channel, packet, length, user_callback, user_data, is_ip_data);
        }
        vPortFree(send_data);
        MUX_AP_LOGE("csci_ap_send_blocks_to_md() == CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_SPACE");
        return;
    }

    configASSERT(status == CSCI_STATUS_OK);
    csci_ap_indicate_data_available_to_md();

    mux_ap_data_manager_send_completed(channel, packet, length, user_data);

    if (resend) {
        mux_ap_csci_remove_user_data_from_resend_list(channel, packet, length, user_data);
    }

    vPortFree(send_data);
}

void mux_ap_csci_resend(void)
{
    mux_ap_csci_data_node_t *data_node;

    if (mux_ap_csci_resend_list_is_empty()) {
        mux_ap_sleep_manager_unlock_sleep(MUX_AP_SLEEP_MANAGER_LOCK_CATEGORY_RESEND_LIST);
        MUX_AP_LOGI("mux_ap_csci_resend_list_is_empty() == true");
        return;
    }

    data_node = mux_ap_csci_get_resend_list();

    configASSERT(data_node != NULL);

    switch (data_node->node_type) {
        case CSCI_MESSAGE_TYPE_CHANNEL_ENABLE_REQUEST: {
            mux_ap_csci_send_channel_enable_request(data_node->channel, data_node->node_data.data_type1.channel_type, true);
        }
        break;

        case CSCI_MESSAGE_TYPE_LINE_STATUS: {
            mux_ap_csci_send_line_status(data_node->channel, data_node->node_data.data_type2.rts, true);
        }
        break;

        case CSCI_MESSAGE_TYPE_USER_DATA:
        case CSCI_MESSAGE_TYPE_USER_DATA_IP: {
            mux_ap_csci_data_node_t *traverse = data_node;
            while (traverse != NULL) {
                if (!mux_ap_channel_manager_is_stop_to_send(traverse->channel)) {
                    break;
                }
                traverse = traverse->next;
            }

            if (traverse != NULL) {
                mux_ap_callback_t user_callback = mux_ap_data_manager_get_user_callback(data_node->channel);
                data_node = traverse;
                if (user_callback == data_node->node_data.data_type3.user_callback) {
                    mux_ap_csci_send_user_data(data_node->channel, data_node->node_data.data_type3.packet, data_node->node_data.data_type3.length, data_node->node_data.data_type3.user_callback, data_node->node_data.data_type3.user_data, data_node->node_data.data_type3.is_ip_data, true);
                } else {
                    MUX_AP_LOGI("user_callback = 0x%x, user_callback = 0x%x", (unsigned int)user_callback, (unsigned int)data_node->node_data.data_type3.user_callback);
                    mux_ap_data_manager_send_completed(data_node->channel, data_node->node_data.data_type3.packet, data_node->node_data.data_type3.length, data_node->node_data.data_type3.user_data);
                    mux_ap_csci_remove_user_data_from_resend_list(data_node->channel, data_node->node_data.data_type3.packet, data_node->node_data.data_type3.length, data_node->node_data.data_type3.user_data);
                }
            } else {
                MUX_AP_LOGE("all in flow control, resend failed...");
                return;
            }
        }
        break;

        default:
            break;
    }

    mux_ap_data_manager_resend();
}

#ifdef MUX_AP_UT
void mux_md_csci_send_channel_enable_confirm(uint32_t channel)
{
    mux_ap_csci_channel_enable_confirm_t *channel_enable_confirm;
    uint32_t block_length, block_num;
    csci_status_t status;

    block_length = MUX_AP_CSCI_CHANNEL_ENABLE_CONFIRM_MESSAGE_HEADER_SIZE;
    channel_enable_confirm = (mux_ap_csci_channel_enable_confirm_t *)pvPortMalloc(block_length);

    channel_enable_confirm->length = block_length;
    channel_enable_confirm->channel = channel;
    channel_enable_confirm->type = CSCI_MESSAGE_TYPE_CHANNEL_ENABLE_CONFIRM;

    if (block_length % MUX_AP_CSCI_BLOCK_SIZE == 0) {
        block_num = block_length / MUX_AP_CSCI_BLOCK_SIZE;
    } else {
        block_num = block_length / MUX_AP_CSCI_BLOCK_SIZE + 1;
    }

    status = csci_md_send_blocks_to_ap((const uint8_t *)channel_enable_confirm, block_num);
    MUX_AP_LOGI("csci_md_send_blocks_to_ap(%d) = %d", (int)block_num, (int)status);
    if (status == CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_SPACE) {
        vPortFree(channel_enable_confirm);
        MUX_AP_LOGE("csci_md_send_blocks_to_ap() == CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_SPACE");
        return;
    }

    configASSERT(status == CSCI_STATUS_OK);
    csci_md_indicate_data_available_to_ap();

    vPortFree(channel_enable_confirm);
}

void mux_md_csci_send_line_status(uint32_t channel, uint32_t cts)
{
    mux_ap_csci_line_status_t *line_status;
    uint32_t block_length, block_num;
    csci_status_t status;

    block_length = MUX_AP_CSCI_LINE_STATUS_MESSAGE_HEADER_SIZE;
    line_status = (mux_ap_csci_line_status_t *)pvPortMalloc(block_length);

    line_status->length = block_length;
    line_status->channel = channel;
    line_status->type = CSCI_MESSAGE_TYPE_LINE_STATUS;
    line_status->dsr = CSCI_CHANNEL_LINE_ON;
    line_status->cts = cts;
    line_status->dcd = CSCI_CHANNEL_LINE_OFF;
    line_status->ri = CSCI_CHANNEL_LINE_OFF;

    if (block_length % MUX_AP_CSCI_BLOCK_SIZE == 0) {
        block_num = block_length / MUX_AP_CSCI_BLOCK_SIZE;
    } else {
        block_num = block_length / MUX_AP_CSCI_BLOCK_SIZE + 1;
    }

    status = csci_md_send_blocks_to_ap((const uint8_t *)line_status, block_num);
    MUX_AP_LOGI("csci_md_send_blocks_to_ap(%d) = %d", (int)block_num, (int)status);
    if (status == CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_SPACE) {
        vPortFree(line_status);
        MUX_AP_LOGE("csci_md_send_blocks_to_ap() == CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_SPACE");
        return;
    }

    configASSERT(status == CSCI_STATUS_OK);
    csci_md_indicate_data_available_to_ap();

    vPortFree(line_status);
}

void mux_md_csci_send_user_data(uint32_t channel, uint8_t *packet, uint32_t length)
{
    mux_ap_csci_user_data_t *user_data;
    uint32_t block_length, block_num;
    csci_status_t status;

    configASSERT(packet != NULL && length != 0);

    block_length = MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE + length;
    user_data = (mux_ap_csci_user_data_t *)pvPortMalloc(block_length);

    user_data->length = block_length;
    user_data->channel = channel;
    user_data->type = CSCI_MESSAGE_TYPE_USER_DATA;
    memcpy(user_data->packet, packet, length);

    if (block_length % MUX_AP_CSCI_BLOCK_SIZE == 0) {
        block_num = block_length / MUX_AP_CSCI_BLOCK_SIZE;
    } else {
        block_num = block_length / MUX_AP_CSCI_BLOCK_SIZE + 1;
    }

    status = csci_md_send_blocks_to_ap((const uint8_t *)user_data, block_num);
    MUX_AP_LOGI("csci_md_send_blocks_to_ap(%d) = %d", (int)block_num, (int)status);
    if (status == CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_SPACE) {
        vPortFree(user_data);
        MUX_AP_LOGE("csci_md_send_blocks_to_ap() == CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_SPACE");
        return;
    }

    configASSERT(status == CSCI_STATUS_OK);
    csci_md_indicate_data_available_to_ap();

    vPortFree(user_data);
}
#endif

static mux_ap_csci_data_node_t *g_mux_ap_csci_receive_list;

static bool mux_ap_csci_receive_list_is_empty(void)
{
    if (g_mux_ap_csci_receive_list == NULL) {
        return true;
    } else {
        return false;
    }
}

static void mux_ap_csci_add_to_receive_list(mux_ap_csci_data_node_t *data_node)
{
    if (g_mux_ap_csci_receive_list == NULL) {
        g_mux_ap_csci_receive_list = data_node;
    } else {
        mux_ap_csci_data_node_t *traverse = g_mux_ap_csci_receive_list;
        while (traverse->next != NULL) {
            traverse = traverse->next;
        }
        traverse->next = data_node;
    }

    mux_ap_sleep_manager_lock_sleep(MUX_AP_SLEEP_MANAGER_LOCK_CATEGORY_RECEIVE_LIST);
}

static void mux_ap_csci_add_user_data_to_receive_list(uint32_t channel, uint8_t *packet, uint32_t length, mux_ap_callback_t user_callback)
{
    mux_ap_csci_data_node_t *data_node;
    data_node = (mux_ap_csci_data_node_t *)pvPortMalloc(sizeof(mux_ap_csci_data_node_t));
    configASSERT(data_node != NULL);
    data_node->channel = channel;
    data_node->node_data.data_type3.packet = packet;
    data_node->node_data.data_type3.length = length;
    data_node->node_data.data_type3.user_callback = user_callback;
    data_node->node_data.data_type3.user_data = NULL;
    data_node->node_type = CSCI_MESSAGE_TYPE_USER_DATA;
    data_node->next = NULL;
    mux_ap_csci_add_to_receive_list(data_node);
}

static void mux_ap_csci_remove_user_data_from_receive_list(uint32_t channel, uint8_t *packet, uint32_t length)
{
    mux_ap_csci_data_node_t *traverse = g_mux_ap_csci_receive_list;
    mux_ap_csci_data_node_t *previous = NULL;
    while (traverse != NULL) {
        if (traverse->channel == channel &&
                traverse->node_type == CSCI_MESSAGE_TYPE_USER_DATA &&
                traverse->node_data.data_type3.packet == packet &&
                traverse->node_data.data_type3.length == length) {
            if (previous == NULL) {
                g_mux_ap_csci_receive_list = traverse->next;
            } else {
                previous->next = traverse->next;
            }
            vPortFree(traverse->node_data.data_type3.packet);
            vPortFree(traverse);
            break;
        }
        traverse = traverse->next;
    }
}

static mux_ap_csci_data_node_t *mux_ap_csci_get_receive_list(void)
{
    return g_mux_ap_csci_receive_list;
}

static bool mux_ap_csci_search_in_receive_list(uint32_t channel)
{
    if (g_mux_ap_csci_receive_list == NULL) {
        return false;
    } else {
        mux_ap_csci_data_node_t *traverse = g_mux_ap_csci_receive_list;
        while (traverse != NULL) {
            if (traverse->channel == channel) {
                return true;
            }
            traverse = traverse->next;
        }
        return false;
    }
}

void mux_ap_csci_receive_data(void)
{
    mux_ap_csci_generic_message_t *generic_message;
    uint32_t block_length, block_num;
    csci_status_t status;

    hal_nvic_disable_irq(CSCI_AP_READ_IRQn);
    block_num = csci_ap_get_number_of_waiting_receive_blocks();
    if (block_num == 0) {
        mux_ap_data_manager_clear_is_receiving_data();
        hal_nvic_enable_irq(CSCI_AP_READ_IRQn);
        MUX_AP_LOGI("csci_ap_get_number_of_waiting_receive_blocks() == 0");
        return;
    }

    hal_nvic_enable_irq(CSCI_AP_READ_IRQn);

    block_length = MUX_AP_CSCI_BLOCK_SIZE;
    generic_message = (mux_ap_csci_generic_message_t *)pvPortMalloc(block_length);

    configASSERT(generic_message != NULL);
    status = csci_ap_receive_blocks_from_md((uint8_t *)generic_message, 1);
    configASSERT(status == CSCI_STATUS_OK);

    switch (generic_message->type) {
        case CSCI_MESSAGE_TYPE_CHANNEL_ENABLE_CONFIRM: {
            mux_ap_csci_channel_enable_confirm_t *channel_enable_confirm = (mux_ap_csci_channel_enable_confirm_t *)generic_message;
            configASSERT(channel_enable_confirm->length == MUX_AP_CSCI_CHANNEL_ENABLE_CONFIRM_MESSAGE_HEADER_SIZE);
            MUX_AP_LOGI("[RX -> CHANNEL_ENABLE_CONFIRM] channel = %d", (int)channel_enable_confirm->channel);
            mux_ap_send_line_status(channel_enable_confirm->channel, true);
            mux_ap_channel_manager_set_ready(channel_enable_confirm->channel, true);
        }
        break;

        case CSCI_MESSAGE_TYPE_CHANNEL_DISABLED: {
            mux_ap_csci_channel_disabled_t *channel_disabled = (mux_ap_csci_channel_disabled_t *)generic_message;
            configASSERT(channel_disabled->length == MUX_AP_CSCI_CHANNEL_DISABLED_MESSAGE_HEADER_SIZE);
            MUX_AP_LOGI("[RX -> CHANNEL_DISABLED] channel = %d", (int)channel_disabled->channel);
            mux_ap_channel_manager_set_ready(channel_disabled->channel, false);
        }
        break;

        case CSCI_MESSAGE_TYPE_USER_DATA: {
            mux_ap_csci_user_data_t *user_data = (mux_ap_csci_user_data_t *)generic_message;
            configASSERT(user_data->length >= MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE);
            MUX_AP_LOGI("[RX -> USER_DATA] channel = %d, length = %d", (int)user_data->channel, (int)(user_data->length - MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE));

            block_length = user_data->length - MUX_AP_CSCI_BLOCK_SIZE;
            if (block_length % MUX_AP_CSCI_BLOCK_SIZE == 0) {
                block_num = block_length / MUX_AP_CSCI_BLOCK_SIZE;
            } else {
                block_num = block_length / MUX_AP_CSCI_BLOCK_SIZE + 1;
            }

            if (user_data->length == MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE) {
                MUX_AP_LOGE("empty user data");
                break;
            } else {
                mux_ap_event_prepare_to_receive_t prepare_to_receive;
                uint32_t user_case;
                mux_ap_callback_t user_callback;

                prepare_to_receive.channel_id = user_data->channel;
                prepare_to_receive.data_buffer = NULL;
                prepare_to_receive.buffer_length = (block_num + 1) * MUX_AP_CSCI_BLOCK_SIZE;
                prepare_to_receive.user_data = NULL;

                user_callback = mux_ap_data_manager_get_user_callback(user_data->channel);
                mux_ap_data_manager_prepare_to_receive(&prepare_to_receive);
                if (prepare_to_receive.data_buffer != NULL) {
                    user_case = 1;
                } else if (mux_ap_data_manager_is_stop_to_receive(user_data->channel)) {
                    prepare_to_receive.data_buffer = (uint8_t *)pvPortMalloc((block_num + 1) * MUX_AP_CSCI_BLOCK_SIZE);
                    configASSERT(prepare_to_receive.data_buffer != NULL);
                    user_case = 2;
                } else {
                    prepare_to_receive.data_buffer = (uint8_t *)pvPortMalloc((block_num + 1) * MUX_AP_CSCI_BLOCK_SIZE);
                    configASSERT(prepare_to_receive.data_buffer != NULL);
                    user_case = 3;
                    MUX_AP_LOGE("discard user data due to AP not providing the memory");
                }

                if (user_data->length <= MUX_AP_CSCI_BLOCK_SIZE) {
                    memcpy(prepare_to_receive.data_buffer, user_data->packet, user_data->length - MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE);
                    if (user_case == 1) {
                        mux_ap_data_manager_receive_completed(user_data->channel, prepare_to_receive.data_buffer, user_data->length - MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE, prepare_to_receive.user_data, user_callback);
                    } else if (user_case == 2) {
                        mux_ap_csci_add_user_data_to_receive_list(user_data->channel, prepare_to_receive.data_buffer, user_data->length - MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE, user_callback);
                    } else if (user_case == 3) {
                        vPortFree(prepare_to_receive.data_buffer);
                    }
                    break;
                }

                memcpy(prepare_to_receive.data_buffer, user_data->packet, MUX_AP_CSCI_BLOCK_SIZE - MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE);

                status = csci_ap_receive_blocks_from_md(prepare_to_receive.data_buffer + MUX_AP_CSCI_BLOCK_SIZE - MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE, block_num);
                configASSERT(status == CSCI_STATUS_OK);

                if (user_case == 1) {
                    mux_ap_data_manager_receive_completed(user_data->channel, prepare_to_receive.data_buffer, user_data->length - MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE, prepare_to_receive.user_data, user_callback);
                } else if (user_case == 2) {
                    mux_ap_csci_add_user_data_to_receive_list(user_data->channel, prepare_to_receive.data_buffer, user_data->length - MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE, user_callback);
                } else if (user_case == 3) {
                    vPortFree(prepare_to_receive.data_buffer);
                }
            }
        }
        break;

        case CSCI_MESSAGE_TYPE_LINE_STATUS: {
            mux_ap_csci_line_status_t *line_status = (mux_ap_csci_line_status_t *)generic_message;
            configASSERT(line_status->length == MUX_AP_CSCI_LINE_STATUS_MESSAGE_HEADER_SIZE);
            MUX_AP_LOGI("[RX -> LINE_STATUS] channel = %d, cts = %d", (int)line_status->channel, (int)line_status->cts);
            if (line_status->cts == CSCI_CHANNEL_LINE_ON) {
                mux_ap_channel_manager_set_stop_to_send(generic_message->channel, false);
                mux_ap_data_manager_resend();
            } else {
                mux_ap_channel_manager_set_stop_to_send(generic_message->channel, true);
            }
        }
        break;

        default:
            break;
    }

    vPortFree(generic_message);
    mux_ap_data_manager_continue_to_receive();
}

void mux_ap_csci_receive_data_from_receive_list(uint32_t channel)
{
    mux_ap_csci_data_node_t *data_node;

    if (mux_ap_csci_receive_list_is_empty()) {
        mux_ap_sleep_manager_unlock_sleep(MUX_AP_SLEEP_MANAGER_LOCK_CATEGORY_RECEIVE_LIST);
        MUX_AP_LOGI("mux_ap_csci_receive_list_is_empty() == true");
        return;
    }

    data_node = mux_ap_csci_get_receive_list();

    configASSERT(data_node != NULL);

    if (data_node->node_type == CSCI_MESSAGE_TYPE_USER_DATA) {
        mux_ap_csci_data_node_t *traverse = data_node;
        while (traverse != NULL) {
            if (traverse->channel == channel) {
                if (mux_ap_data_manager_is_stop_to_receive(traverse->channel)) {
                    MUX_AP_LOGE("mux_ap_data_manager_is_stop_to_receive() == true");
                    return;
                } else {
                    break;
                }
            }
            traverse = traverse->next;
        }

        data_node = traverse;

        if (data_node != NULL) {
            mux_ap_event_prepare_to_receive_t prepare_to_receive;

            prepare_to_receive.channel_id = channel;
            prepare_to_receive.data_buffer = NULL;
            prepare_to_receive.buffer_length = data_node->node_data.data_type3.length;
            prepare_to_receive.user_data = NULL;

            mux_ap_data_manager_prepare_to_receive(&prepare_to_receive);
            if (prepare_to_receive.data_buffer == NULL) {
                MUX_AP_LOGE("prepare_to_receive.data_buffer == NULL");
                return;
            }

            memcpy(prepare_to_receive.data_buffer, data_node->node_data.data_type3.packet, data_node->node_data.data_type3.length);

            mux_ap_data_manager_receive_completed(channel, prepare_to_receive.data_buffer, prepare_to_receive.buffer_length, prepare_to_receive.user_data, data_node->node_data.data_type3.user_callback);
            mux_ap_csci_remove_user_data_from_receive_list(channel, data_node->node_data.data_type3.packet, data_node->node_data.data_type3.length);

            if (!mux_ap_csci_search_in_receive_list(channel)) {
                MUX_AP_LOGI("mux_ap_csci_search_in_receive_list() == false");
                mux_ap_send_line_status(channel, true);
            }

            mux_ap_data_manager_resume_to_receive_from_receive_list(channel);
        } else {
            MUX_AP_LOGE("no data queued on this channel, receive failed...");
        }
    }
}

#ifdef MUX_AP_UT
void mux_md_csci_receive_data(void)
{
    mux_ap_csci_generic_message_t *generic_message;
    uint32_t block_length, block_num;
    csci_status_t status;

    block_num = csci_md_get_number_of_waiting_receive_blocks();
    if (block_num == 0) {
        MUX_AP_LOGI("csci_md_get_number_of_waiting_receive_blocks() == 0");
        return;
    }

    block_length = MUX_AP_CSCI_BLOCK_SIZE;
    generic_message = (mux_ap_csci_generic_message_t *)pvPortMalloc(block_length);

    status = csci_md_receive_blocks_from_ap((uint8_t *)generic_message, 1);
    MUX_AP_LOGI("csci_md_receive_blocks_from_ap(%d) = %d", 1, (int)status);
    configASSERT(status == CSCI_STATUS_OK);

    switch (generic_message->type) {
        case CSCI_MESSAGE_TYPE_CHANNEL_ENABLE_REQUEST: {
            mux_ap_csci_channel_enable_request_t *channel_enable_request = (mux_ap_csci_channel_enable_request_t *)generic_message;
            configASSERT(channel_enable_request->length == MUX_AP_CSCI_CHANNEL_ENABLE_REQUEST_MESSAGE_HEADER_SIZE);
            MUX_AP_LOGI("channel = %d, type = %d", (int)channel_enable_request->channel, (int)channel_enable_request->type);
            mux_md_csci_send_channel_enable_confirm(channel_enable_request->channel);
        }
        break;

        case CSCI_MESSAGE_TYPE_USER_DATA: {
            mux_ap_csci_user_data_t *user_data = (mux_ap_csci_user_data_t *)generic_message;
            configASSERT(user_data->length >= MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE);
            MUX_AP_LOGI("channel = %d, type = %d, length = %d", (int)user_data->channel, (int)user_data->type, (int)(user_data->length - MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE));

            block_length = user_data->length - MUX_AP_CSCI_BLOCK_SIZE;
            if (block_length % MUX_AP_CSCI_BLOCK_SIZE == 0) {
                block_num = block_length / MUX_AP_CSCI_BLOCK_SIZE;
            } else {
                block_num = block_length / MUX_AP_CSCI_BLOCK_SIZE + 1;
            }

            if (user_data->length == MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE) {
                MUX_AP_LOGW("empty user data");
                break;
            } else {
                uint8_t *data_buffer = (uint8_t *)pvPortMalloc((block_num + 1) * MUX_AP_CSCI_BLOCK_SIZE);
                if (user_data->length <= MUX_AP_CSCI_BLOCK_SIZE) {
                    memcpy(data_buffer, user_data->packet, user_data->length - MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE);
                    mux_md_csci_send_user_data(user_data->channel, data_buffer, user_data->length - MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE);
                    vPortFree(data_buffer);
                    break;
                }

                memcpy(data_buffer, user_data->packet, MUX_AP_CSCI_BLOCK_SIZE - MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE);

                status = csci_md_receive_blocks_from_ap(data_buffer + MUX_AP_CSCI_BLOCK_SIZE - MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE, block_num);
                MUX_AP_LOGI("csci_md_receive_blocks_from_ap(%d) = %d", (int)block_num, (int)status);
                configASSERT(status == CSCI_STATUS_OK);

                mux_md_csci_send_user_data(user_data->channel, data_buffer, user_data->length - MUX_AP_CSCI_USER_DATA_MESSAGE_HEADER_SIZE);
                vPortFree(data_buffer);
            }
        }
        break;

        case CSCI_MESSAGE_TYPE_LINE_STATUS: {
            mux_ap_csci_line_status_t *line_status = (mux_ap_csci_line_status_t *)generic_message;
            configASSERT(line_status->length == MUX_AP_CSCI_LINE_STATUS_MESSAGE_HEADER_SIZE);
            MUX_AP_LOGI("channel = %d, type = %d, rts = %d", (int)line_status->channel, (int)line_status->type, (int)line_status->rts);
            if (line_status->rts == CSCI_CHANNEL_LINE_ON) {
                mux_md_csci_send_line_status(generic_message->channel, CSCI_CHANNEL_LINE_ON);
            }
        }
        break;

        default:
            break;
    }

    vPortFree(generic_message);
}
#endif

