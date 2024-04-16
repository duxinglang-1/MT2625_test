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

#include "ril_general_types.h"

#include "task.h"
#include "queue.h"
#include "timers.h"

#include "hal_sleep_manager.h"

#include "task_def.h"

#include "mux_ap.h"
#include "ril_task.h"
#include "ril_cmds_def.h"
#include "ril_cmds_common.h"
#include "ril_utils.h"
#include "ril_log.h"
#include "ril_channel_config.h"
#include "ril_general_setting.h"

#include "hal_gpt.h"
#include "hal_rtc.h"
#include "hal_rtc_external.h"


//#include "frgkicfg.h"
//#include "gkisig.h"

/**************************************************************
 * Macro                                                                                                *
 **************************************************************/
/* UT option */
//#define __RIL_SLEEP_LOCK_DEBUG__


#define RIL_TASK_NAME  "ril"
#define RIL_TASK_PRIORITY TASK_PRIORITY_ABOVE_NORMAL
#define RIL_TASK_STACK_SIZE (4*1024)
#define RIL_QUEUE_LENGTH (30)

#define BUF_HEADER_PATTERN    (0xF1F1F1F1)
#define BUF_FOOTER_PATTERN    (0xF2F2F2F2)

#define RIL_SLEEP_HANDLE    ("ril_sleep")

/**************************************************************
 * Structure, enum definition                                                                   *
 **************************************************************/

QueueHandle_t ril_queue;
ril_channel_cntx_t ril_channel[RIL_MAX_CHNL];
ril_cntx_t ril_cntx;
static uint8_t ril_sleep_handle;
static bool ril_sleep_locked = false;

/* ril task */
static void ril_task(void *arg);
static void ril_task_event_handler(ril_message_type_t msg, void *data, uint32_t len);
static int32_t ril_request_dispatch(void *buf, uint32_t len);
static void ril_response_dispatch(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);
static void ril_urc_dispatch(char *cmd_buf, uint32_t cmd_buf_len);
static bool ril_is_idle_state(void);
static bool ril_channel_buf_is_valid(uint32_t channel_id);
static uint32_t ril_get_curr_time_in_ms(void);
static void ril_check_busy_channel_status(int32_t *force_free_channel_id, ril_func_group_t func_group, ril_cmd_type_t cmd_type);
static bool ril_check_channel_type_is_all_busy(ril_cmd_type_t cmd_type);
static int32_t ril_request_pending_list_push(ril_func_group_t func_group, ril_cmd_type_t cmd_type, int32_t channel_id, ril_request_info_t *info);
static int32_t ril_request_pending_list_pop();

/**************************************************************
 * Function                                                                                            *
 **************************************************************/
void ril_init()
{
    TaskHandle_t xCreatedRilTask;
    mux_ap_status_t mux_status = MUX_AP_STATUS_OK;
    uint32_t magic;
    int32_t i = 1;
    ril_channel_cntx_t *channel;

    /* should create queue earlier than register mux channel, because mux callback will be invoked immediately if wake from deep sleep and send channel enabled message to RIL */
    ril_queue = xQueueCreate(RIL_QUEUE_LENGTH, sizeof(ril_message_t));
    if ( ril_queue == NULL ) {
        RIL_LOGE("failed to create dfu queue!\r\n");
        return;
    }

    while (i <= RIL_MAX_CHNL && mux_status == MUX_AP_STATUS_OK) {
        mux_status = mux_ap_register_callback(MUX_AP_CHANNEL_TYPE_AT_AND_DATA, ril_channel_mux_callback, (void *)i);
        channel = &ril_channel[i - 1];
        channel->channel_id = i;
        magic = BUF_HEADER_PATTERN;
        memcpy(channel->rx_buf, &magic, sizeof(uint32_t));
        memcpy(channel->tx_buf, &magic, sizeof(uint32_t));
        magic = BUF_FOOTER_PATTERN;
        memcpy(channel->rx_buf + RIL_BUF_SIZE + 4, &magic, sizeof(uint32_t));
        memcpy(channel->tx_buf + RIL_BUF_SIZE + 4, &magic, sizeof(uint32_t));
        channel->tx_buf_ptr = channel->tx_buf + 4;
        channel->rx_buf_ptr = channel->rx_buf + 4;
        channel->tx_pos = 0;
        channel->rx_pos = 0;
        channel->rx_long_str_len = 0;
        channel->rx_long_str_ptr = NULL;
        channel->reserved = 0;
        channel->recv_forbidden = 0;

        if (i >= RIL_AT_DATA_CHANNEL_ID_START && i <= RIL_AT_DATA_CHANNEL_ID_END) {
            channel->type = RIL_CMD_TYPE_AT_DATA;
        } else if (i == RIL_URC_CHANNEL_ID) {
            channel->type = RIL_CMD_TYPE_URC;
        } else {
            configASSERT(0);
        }
        channel->state = RIL_CHANNEL_STATE_UNAVAILABLE;
        i++;
    }
    RIL_LOGI("mux callback register status: %d\r\n", mux_status);

    configASSERT(ril_cmds_order_is_valid());

    ril_sleep_handle = hal_sleep_manager_set_sleep_handle(RIL_SLEEP_HANDLE);
    memset(&ril_cntx, 0x00, sizeof(ril_cntx_t));

    ril_init_event_callback_table();
    ril_general_setting_init();

    xTaskCreate(ril_task,
                RIL_TASK_NAME,
                RIL_TASK_STACK_SIZE / ((uint32_t)sizeof(StackType_t)),
                NULL, RIL_TASK_PRIORITY,
                &xCreatedRilTask);
}


static void ril_task(void *arg)
{
    ril_message_t message;
    RIL_LOGI("enter ril task.\r\n");

    //if (rtc_power_on_result_external() != DEEP_SLEEP && rtc_power_on_result_external() != DEEPER_SLEEP) {
    //    /* need configure AT format setting if normal bootup */
    //    ril_general_setting();
    //}

    while (1) {
        if (pdPASS == xQueueReceive(ril_queue, (void *)&message, portMAX_DELAY)) {
            ril_task_event_handler(message.msg, message.buf, message.len);
            memset((void *) &message, 0, sizeof(ril_message_t));
        }
    }
}


static void ril_task_event_handler(ril_message_type_t msg, void *data, uint32_t len)
{
    uint32_t channel_id;
    ril_response_info_t *info;
    int32_t handled;

    RIL_LOGI("%s, msg: %d\r\n", __FUNCTION__, msg);
    hal_sleep_manager_status_t sleep_status;

    if (!ril_sleep_locked) {
        if ((sleep_status = hal_sleep_manager_acquire_sleeplock(ril_sleep_handle, HAL_SLEEP_LOCK_DEEP)) != HAL_SLEEP_MANAGER_OK) {
            RIL_LOGE("fail to lock sleep handle\r\n");
        } else {            
            ril_sleep_locked = true;
#ifdef __RIL_SLEEP_LOCK_DEBUG__
            RIL_LOGI("ril lock sleep handle\r\n");
#endif
        }
    }

    switch (msg) {
        case RIL_MSG_REQUEST:
            handled = ril_request_dispatch(data, len);
            if (handled == 0) {
                ril_mem_free(data);
            }
            break;
        case RIL_MSG_RESPONSE:
            info = (ril_response_info_t *)data;
            channel_id = info->channel_id;
            ril_response_dispatch(channel_id, info->cmd_buf, info->cmd_buf_len);
            ril_mem_free(info->cmd_buf);
            ril_mem_free(info);
            break;
        case RIL_MSG_URC:
            info = (ril_response_info_t *)data;
            ril_urc_dispatch(info->cmd_buf, info->cmd_buf_len);
            ril_mem_free(info->cmd_buf);
            ril_mem_free(info);
            break;
        case RIL_MSG_CHANNEL_ENABLED:
            channel_id = (uint32_t)data;
            if (rtc_power_on_result_external() != DEEP_SLEEP && rtc_power_on_result_external() != DEEPER_SLEEP) {
                /* normal bootup */
                ril_channel[channel_id - 1].state = RIL_CHANNEL_STATE_READY;
                ril_general_setting_flow_start(RIL_GENERAL_SETTING_FLOW_TYPE_FORMAT_CONFIG, channel_id);
            } else {
                /* wakeup from deep sleep */
#ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
                ril_channel[channel_id - 1].state = RIL_CHANNEL_STATE_READY;
#else
                ril_channel[channel_id - 1].state = RIL_CHANNEL_STATE_WAIT_TO_WAKEUP;
#endif
            }
            break;
        case RIL_MSG_CHANNEL_DISABLED:
            channel_id = (uint32_t)data;
            ril_channel[channel_id - 1].state = RIL_CHANNEL_STATE_UNAVAILABLE;
            break;
        case RIL_MSG_SEND_COMPLETE:
            channel_id = (uint32_t)data;
            ril_channel[channel_id - 1].tx_pos = 0;
            memset(ril_channel[channel_id - 1].tx_buf_ptr, 0x00, RIL_BUF_SIZE);
            break;
        case RIL_MSG_DATA_CHANNEL_RESERVED:
            channel_id = (uint32_t)data;
            ril_channel[channel_id - 1].reserved = 1;
            break;
        case RIL_MSG_DATA_CHANNEL_UNRESERVED:
            channel_id = (uint32_t)data;
            ril_channel[channel_id - 1].reserved = 0;
            if (ril_channel[channel_id - 1].state == RIL_CHANNEL_STATE_UNAVAILABLE) {
                ril_channel[channel_id - 1].state = RIL_CHANNEL_STATE_READY;
            }
            break;
        default:
            RIL_LOGE("No support message.\r\n");
            //configASSERT(0);
            break;
    }

    /* handle holding requests */
    ril_request_pending_list_pop();

    if (ril_is_idle_state() && ril_sleep_locked) {
        if ((sleep_status = hal_sleep_manager_release_sleeplock(ril_sleep_handle, HAL_SLEEP_LOCK_DEEP)) != HAL_SLEEP_MANAGER_OK) {
            RIL_LOGE("fail to unlock sleep handle\r\n");
        } else {
            ril_sleep_locked = false;
#ifdef __RIL_SLEEP_LOCK_DEBUG__
            RIL_LOGI("ril unlock sleep handle\r\n");
#endif
        }
    }
}


static int32_t ril_request_dispatch(void *buf, uint32_t len)
{
    int32_t channel_id = -1;
    ril_param_node_t *list_head;
    mux_ap_status_t mux_status = MUX_AP_STATUS_CHANNEL_NOT_READY;
    ril_channel_cntx_t *channel;
    uint32_t suggested_timeout;
    ril_cmd_send_hdlr_t send_hdlr;
    int32_t ret;
    int32_t ret_val = 0;
    int32_t cmd_str_len = 0;
    ril_request_info_t *info = (ril_request_info_t *)buf;
    uint32_t id = info->request_id;
    ril_cmd_item_t *cmd_item = get_at_cmd_item((ril_cmd_id_t)id);
    ril_cmd_type_t cmd_type = cmd_item->cmd_type;
    ril_cmd_response_callback_t usr_cb = (ril_cmd_response_callback_t)info->rsp_callback;

    if (cmd_item->func_group & ril_cntx.busy_func_group_mask) {
        RIL_LOGI("current func group is busy, func_group: %d\r\n", cmd_item->func_group);
        /* find func busy channel, calc elapsed_time, and force to free if timeout */
        
        ril_check_busy_channel_status(&channel_id, cmd_item->func_group, -1);
        if (channel_id < RIL_URC_CHANNEL_ID || channel_id > RIL_AT_DATA_CHANNEL_ID_END) {
            ril_request_pending_list_push(cmd_item->func_group, cmd_item->cmd_type, info->channel_id, info);
            return -1;
        }
    } else if (info->channel_id >= RIL_URC_CHANNEL_ID && info->channel_id <= RIL_AT_DATA_CHANNEL_ID_END) {
        /* should use specified channel */
        if (ril_channel[info->channel_id - 1].state == RIL_CHANNEL_STATE_READY) {
            channel_id = info->channel_id;
        } else {
            ril_request_pending_list_push(cmd_item->func_group, cmd_item->cmd_type, info->channel_id, info);
            //ril_cntx.busy_channel_mask |= cmd_type;
            return -1;
        }
    } else {
        /* get free channel first */
        channel_id = ril_alloc_channel(cmd_type, false);

        if (channel_id < 0) {
            ret_val = -1;
            goto final;
        } else if (channel_id == 0) {
            /* return BUSY to caller */
            ril_request_pending_list_push(cmd_item->func_group, cmd_item->cmd_type, info->channel_id, info);
            ril_cntx.busy_channel_mask |= cmd_type;
            return -1;
        } else if (channel_id > RIL_AT_DATA_CHANNEL_ID_END) {
            RIL_LOGE("Invalid channel id\r\n");
            configASSERT(0);
            ret_val = -1;
            goto final;
        }
    }

    /* construct cmd string */
    channel = &ril_channel[channel_id - 1];
    suggested_timeout = cmd_item->timeout_msec;
    send_hdlr = cmd_item->send_hdlr;

    if (send_hdlr) {
        /* if has special construct hdlr, call it */
        cmd_str_len = send_hdlr(channel_id, channel->tx_buf_ptr + channel->tx_pos, info);
    } else {
        /* otherwise, call common construct hdlr */
        cmd_str_len = ril_cmd_send_common_hdlr(channel_id, channel->tx_buf_ptr + channel->tx_pos, info);
    }
    /* send data by MUX API */
    if (cmd_str_len > 0) {
        ril_cmd_type_t sent_channel_type;
        
        /* record request info, if send fail, report to caller */
        channel->state = RIL_CHANNEL_STATE_BUSY;
        channel->curr_request_id = info->request_id;
        channel->curr_request_mode = info->mode;
        channel->usr_rsp_cb = info->rsp_callback;
        channel->usr_data = info->user_data;
        channel->timestamp = ril_get_curr_time_in_ms();
        channel->elapsed_time = suggested_timeout;
        channel->tx_pos = cmd_str_len + 1;

        /* set function group mask */
        ril_cntx.busy_func_group_mask |= cmd_item->func_group;
        sent_channel_type = ril_channel[channel_id - 1].type;
        if (ril_check_channel_type_is_all_busy(sent_channel_type)) {
            RIL_LOGI("channel type(%d) is busy", sent_channel_type);
            ril_cntx.busy_channel_mask |= sent_channel_type;
        }

        RIL_LOGCT("mux channel %d, send command data: %s\r\n", (int)channel_id, channel->tx_buf_ptr);
        mux_status = mux_ap_send_data(channel_id, (const uint8_t *)channel->tx_buf_ptr, cmd_str_len + 1, 0);
        RIL_LOGI("send command data done, ret: %d\r\n", mux_status);
    }

    if (mux_status != MUX_AP_STATUS_OK) {
        /* mux sen fail, should notify to user and clean channel context */
        ril_cmd_response_t cmd_response;
        RIL_CONSTRUCT_ERROR_RESPONSE(cmd_response, info->request_id, info->mode, info->user_data, RIL_RESULT_CODE_MUX_SEND_ERROR);
        usr_cb(&cmd_response);
        ril_free_channel(channel_id);
        ril_cntx.busy_func_group_mask &= ~(cmd_item->func_group);
        ril_cntx.busy_channel_mask &= ~(cmd_item->cmd_type);
    }

final:
    /* free request info structure, malloc by RIL API */
    list_head = (ril_param_node_t *)info->request_param;
    if (list_head != NULL) {
        if ((ret = ril_param_list_delete(list_head)) < 0) {
            RIL_LOGE("failed to delete param list, ret: %d\r\n", (int)ret);
        }
    }
    return ret_val;
}



static void ril_response_dispatch(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    ril_cmd_item_t *cmd_item;
    ril_cmd_rsp_hdlr_t rsp_hdlr;
    ril_channel_cntx_t *channel_p = &ril_channel[channel_id - 1];
    uint32_t cmd_id = channel_p->curr_request_id;
    int32_t ret = 0;

    RIL_LOGDUMPSTRCT("response buf: %s\r\n", cmd_buf_len, cmd_buf);

    if (cmd_id == 0) {
        RIL_LOGE("unexpected response, drop it\r\n");
        return;
    }
    cmd_item = get_at_cmd_item((ril_cmd_id_t)cmd_id);
    rsp_hdlr = cmd_item->rsp_hdlr;
    RIL_LOGCT("channel_id: %lu, cmd_id: %lu, cmd head: %s\r\n", channel_id, cmd_id, cmd_item->cmd_head);
    if (rsp_hdlr) {
        ret = (rsp_hdlr)(channel_id, cmd_buf, cmd_buf_len);
    }

    /* check if rx buffer is corruped or not */
    configASSERT(ril_channel_buf_is_valid(channel_id));

    if (ret == 0) {
        /* clear function group mask */
        ril_cntx.busy_func_group_mask &= ~(cmd_item->func_group);
        ril_cntx.busy_channel_mask &= ~(channel_p->type);

        /* free channel */
        ril_free_channel(channel_id);
    }
}


static void ril_urc_dispatch(char *cmd_buf, uint32_t cmd_buf_len)
{
    ril_urc_cmd_item_t *urc_item;
    ril_urc_id_t urc_id;
    int32_t ret;
    char *cmd_head = NULL;
    uint32_t cmd_head_len = 0;

    RIL_LOGDUMPSTRCT("urc buf: %s\r\n", cmd_buf_len, cmd_buf);
    /* unsolicited result code */
    if ((ret = at_tok_get_at_head(cmd_buf, &cmd_head, &cmd_head_len, NULL)) < 0) {
        RIL_LOGE("failed to get at head, ret: %d\r\n", (int)ret);
        return;
    }

    urc_id = find_urc_cmd_id(cmd_head, cmd_head_len);

    if (urc_id == RIL_URC_ID_INVALID) {
        RIL_LOGE("cannot find this unsolicited code.\r\n");
        return;
    }

    urc_item = get_urc_cmd_item(urc_id);
    urc_item->urc_hdlr(urc_id, cmd_buf, cmd_buf_len);
}


void ril_send_message(ril_message_type_t msg, void *buf, uint32_t len)
{
    BaseType_t ret;
    ril_message_t message;
    message.msg = msg;
    message.buf = buf;

    ret = xQueueSend(ril_queue, (void *)&message, 0);
    if (pdPASS != ret) {
        RIL_LOGE("failed to send queue\r\n");
        configASSERT(0);
    }
}


void ril_send_message_int(ril_message_type_t msg, void *buf, uint32_t len)
{
    BaseType_t ret;
    ril_message_t message;
    message.msg = msg;
    message.buf = buf;

    /* insert message to front of queue */
    ret = xQueueSendToFront(ril_queue, (void *)&message, 0);
    if (pdPASS != ret) {
        RIL_LOGE("failed to send queue to front\r\n");
        configASSERT(0);
    }
}


void ril_send_packet_message(ril_message_type_t msg,
                             uint32_t channel_id,
                             char *packet,
                             uint32_t packet_len)
{
    char *cmd_buf = ril_mem_malloc(packet_len + 1);
    ril_response_info_t *info = ril_mem_malloc(sizeof(ril_response_info_t));

    RIL_LOGI("%s enter, channel_id: %lu, msg: %d, packet_len: %lu\r\n", __FUNCTION__, channel_id, msg, packet_len);
    if (cmd_buf == NULL || info == NULL) {
        RIL_LOGE("fail to alloc heap\r\n");
        configASSERT(0);
        ril_mem_free(cmd_buf);
        ril_mem_free(info);
        return;
    }

    memcpy(cmd_buf, packet, packet_len);
    cmd_buf[packet_len] = '\0'; // append '\0' to the tail
    info->channel_id = channel_id;
    info->cmd_buf = cmd_buf;
    info->cmd_buf_len = packet_len + 1;

    ril_send_message(msg, info, sizeof(ril_response_info_t));
}


static bool ril_is_idle_state()
{
    int32_t i;
    for (i = RIL_URC_CHANNEL_ID; i < RIL_AT_DATA_CHANNEL_ID_END; i++) {
        if (ril_channel[i - 1].state == RIL_CHANNEL_STATE_BUSY) {
            return false;
        }
    }
    return true;
}


static bool ril_channel_buf_is_valid(uint32_t channel_id)
{
    ril_channel_cntx_t *channel_p = &ril_channel[channel_id - 1];
    uint8_t *buf_p = (uint8_t *)channel_p->rx_buf;
    if ((*((uint32_t *)buf_p) == BUF_HEADER_PATTERN) &&
        (*((uint32_t *)(buf_p + RIL_BUF_SIZE + 4)) == BUF_FOOTER_PATTERN) &&
        (channel_p->rx_buf_ptr == (char *)buf_p + 4)) {
        return true;
    } else {
        return false;
    }
}

static uint32_t ril_get_curr_time_in_ms(void)
{
    uint32_t count = 0;
    uint64_t count64 = 0;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &count);
    count64 = (((uint64_t)count) * 1000) >> 16;
    return (uint32_t)count64;
}


ril_channel_cntx_t *ril_get_channel(uint32_t channel_id)
{
    return &ril_channel[channel_id - 1];
}


int32_t ril_free_channel(uint32_t channel_id)
{
    ril_channel_cntx_t *channel = &ril_channel[channel_id - 1];

    vTaskSuspendAll();

    channel->state = RIL_CHANNEL_STATE_READY;
    channel->curr_request_id = 0;
    channel->curr_request_mode = 0;
    channel->timestamp = 0;
    channel->elapsed_time = 0;
    memset(channel->tx_buf_ptr, 0x00, RIL_BUF_SIZE);
    channel->tx_pos = 0;
    channel->rx_pos = 0;
    channel->usr_rsp_cb = NULL;
    channel->usr_data = NULL;

    xTaskResumeAll();
    return 0;
}


int32_t ril_alloc_channel(ril_cmd_type_t cmd_type, bool reverse)
{
    uint32_t start, end;
    uint32_t i;
    bool found = false;

    switch (cmd_type) {
        case RIL_CMD_TYPE_URC:
            start = end = RIL_URC_CHANNEL_ID;
            break;
        case RIL_CMD_TYPE_AT_DATA:
            start = RIL_AT_DATA_CHANNEL_ID_START;
            end = RIL_AT_DATA_CHANNEL_ID_END;
            break;
        default:
            RIL_LOGE("Not support cmd type.\r\n");
            return -1;
    }

    if (reverse) {
        for (i = end; i >= start; i--) {
            if (ril_channel[i - 1].state == RIL_CHANNEL_STATE_READY &&
                    ril_channel[i - 1].reserved == 0) {
                found = true;
                break;
            }
        }
    } else {
        for (i = start; i <= end; i++) {
            if (ril_channel[i - 1].state == RIL_CHANNEL_STATE_READY &&
                    ril_channel[i - 1].reserved == 0) {
                found = true;
                break;
            }
        }
    }

    if (found) {
        RIL_LOGI("find free channel: %d\r\n", (int)i);
        return i;
    } else {
        int32_t available_channel = -1;
        RIL_LOGI("check busy channel's timeout status.\r\n");
        /* calc elapsed_time, force to free timeout channel */
        ril_check_busy_channel_status(&available_channel, 0, cmd_type);
        if (available_channel == -1) {
            RIL_LOGI("all channels are busy\r\n");
            return 0;
        } else {
            return available_channel;
        }
    }
}


uint32_t ril_get_free_data_channel()
{
    return ril_alloc_channel(RIL_CMD_TYPE_AT_DATA, true);
}


ril_status_t ril_set_data_channel_reserved(uint32_t channel_id, bool is_reserved)
{
    if (channel_id < RIL_AT_DATA_CHANNEL_ID_START || channel_id > RIL_AT_DATA_CHANNEL_ID_END) {
        return RIL_STATUS_FAIL;
    }

    if (is_reserved) {
        ril_send_message(RIL_MSG_DATA_CHANNEL_RESERVED, (void *)channel_id, sizeof(uint32_t));
    } else {
        ril_send_message(RIL_MSG_DATA_CHANNEL_UNRESERVED, (void *)channel_id, sizeof(uint32_t));
    }
    return RIL_STATUS_SUCCESS;
}


ril_status_t ril_channel_forbid_data_receiving(uint32_t channel_id, bool enabled)
{
    RIL_LOGW("channel(%d) data receiving forbidden flag(%d)", channel_id, enabled);
    ril_channel[channel_id - 1].recv_forbidden = enabled ? 1 : 0;
    return RIL_STATUS_SUCCESS;
}


void ril_check_busy_channel_status(int32_t *force_free_channel_id, ril_func_group_t func_group, ril_cmd_type_t cmd_type)
{
    int32_t i;
    uint32_t current_time;
    ril_cmd_item_t *pending_cmd;
    bool check_all = false;
    if (func_group == 0 && cmd_type == 0) {
        check_all = true;
    }
    
    for (i = RIL_URC_CHANNEL_ID; i <= RIL_AT_DATA_CHANNEL_ID_END; i++) {        
        if (ril_channel[i - 1].state == RIL_CHANNEL_STATE_BUSY) {
            ril_cmd_response_callback_t usr_cb = (ril_cmd_response_callback_t)ril_channel[i - 1].usr_rsp_cb;
            pending_cmd = get_at_cmd_item((ril_cmd_id_t)ril_channel[i - 1].curr_request_id);
            
            if (check_all || 
                func_group & pending_cmd->func_group ||
                cmd_type & pending_cmd->cmd_type) {
                current_time = ril_get_curr_time_in_ms();
                if ((ril_channel[i - 1].timestamp + ril_channel[i - 1].elapsed_time < current_time) ||
                        (ril_channel[i - 1].timestamp > current_time)) {
                    /* channel response timeout, force to free this channel and report to caller */                
                    RIL_LOGI("force to free timeout channel, channel_id: %d, curr_request_id: %d\r\n", (int)i, (int)ril_channel[i - 1].curr_request_id);
                    if (usr_cb) {
                        ril_cmd_response_t cmd_response;                        
                        RIL_CONSTRUCT_ERROR_RESPONSE(cmd_response, ril_channel[i - 1].curr_request_id, ril_channel[i - 1].curr_request_mode, ril_channel[i - 1].usr_data, RIL_RESULT_CODE_RESPONSE_TIMEOUT);
                        (usr_cb)(&cmd_response);
                    }
                    ril_free_channel(i);
                    ril_cntx.busy_func_group_mask &= ~(pending_cmd->func_group);
                    ril_cntx.busy_channel_mask &= ~(pending_cmd->cmd_type);

                    if (!check_all) {
                        if (force_free_channel_id != NULL) {
                            *force_free_channel_id = i;
                        }
                        return;
                    }
                }
            }
        }
    }
}



void ril_init_channel_after_deep_sleep()
{
    int32_t i;
    for (i = RIL_URC_CHANNEL_ID; i <= RIL_AT_DATA_CHANNEL_ID_END; i++) {
        /* RIL channel must be marked as READY after *MATWAKE URC reporting. */
        if (ril_channel[i - 1].state == RIL_CHANNEL_STATE_WAIT_TO_WAKEUP) {
            ril_channel[i - 1].state = RIL_CHANNEL_STATE_READY;
        }
    }
    ril_cntx.busy_channel_mask = 0;
    ril_cntx.busy_func_group_mask = 0;
}


bool ril_check_channel_type_is_all_busy(ril_cmd_type_t cmd_type)
{
    int32_t start, end, i;
    switch (cmd_type) {
        case RIL_CMD_TYPE_URC:
            start = end = RIL_URC_CHANNEL_ID;
            break;
        case RIL_CMD_TYPE_AT_DATA:
            start = RIL_AT_DATA_CHANNEL_ID_START;
            end = RIL_AT_DATA_CHANNEL_ID_END;
            break;
        default:
            return false;
    }
    for (i = start; i <= end; i++) {
        if (ril_channel[i - 1].state == RIL_CHANNEL_STATE_READY) {
            return false;
        }
    }
    return true;
}


bool ril_request_pending_list_is_suspended()
{
    return ril_cntx.suspend_pending_items;
}


void ril_request_pending_list_suspend()
{
    RIL_LOGI("pending list suspend");
    ril_cntx.suspend_pending_items = true;
}


void ril_request_pending_list_resume()
{
    RIL_LOGI("pending list resume");
    ril_cntx.suspend_pending_items = false;
}


int32_t ril_request_pending_list_push(ril_func_group_t func_group,
                                      ril_cmd_type_t cmd_type,
                                      int32_t channel_id,
                                      ril_request_info_t *info)
{
    ril_request_pending_node_t *node = ril_cntx.ril_request_pending_list;
    ril_request_pending_node_t *curr_node = ril_mem_malloc(sizeof(ril_request_pending_node_t));
    if (curr_node == NULL) {
        return -1;
    }
    curr_node->func_group = func_group;
    curr_node->cmd_type = cmd_type;
    curr_node->specified_channel_id = channel_id;
    curr_node->info = info;
    curr_node->next = NULL;

    if (node == NULL) {
        ril_cntx.ril_request_pending_list = curr_node;
    } else {
        while (node->next != NULL) {
            node = node->next;
        }
        node->next = curr_node;
    }
    ril_cntx.num_pending_items++;
    RIL_LOGI("pending list push, cmd_head: %s\r\n", get_at_cmd_item((ril_cmd_id_t)info->request_id)->cmd_head);

    return 0;
}


int32_t ril_request_pending_list_pop()
{
    ril_request_pending_node_t *node, *prev_node;

    if (ril_request_pending_list_is_suspended()) {
        /* if suspend pending list, no need to pop */
        RIL_LOGI("pending list pop skip");
        return -1;
    }

    prev_node = node = ril_cntx.ril_request_pending_list;

    //if (node != NULL) {
        /* if have pending command, should check whether timeout happened for busy channel. */
        ril_check_busy_channel_status(NULL, 0, 0);
    //}
    
    for (; node; prev_node = node, node = node->next) {
        //RIL_LOGI("node_request_id: %d, node type: %d, node_func_group: %d, node_chnl_id: %d\r\n",
        //        node->info->request_id,
        //        node->cmd_type,
        //        node->func_group,
        //        node->specified_channel_id);
        //RIL_LOGI("busy_func_group_mask: 0x%x, busy_channel_mask: 0x%x\r\n", ril_cntx.busy_func_group_mask, ril_cntx.busy_channel_mask);
        if (node->func_group & ril_cntx.busy_func_group_mask) {
            continue;
        }

        if (node->specified_channel_id >= RIL_URC_CHANNEL_ID &&
                node->specified_channel_id <= RIL_AT_DATA_CHANNEL_ID_END) {

            if (ril_channel[node->specified_channel_id - 1].state != RIL_CHANNEL_STATE_READY) {
                continue;
            }
        } else {
            if (node->cmd_type & ril_cntx.busy_channel_mask) {
                continue;
            }
        }

        /* find avaliable node, this node could insert to message queue */
        break;
    }

    if (node != NULL) {
        RIL_LOGI("pending list pop, cmd_head: %s\r\n", get_at_cmd_item((ril_cmd_id_t)node->info->request_id)->cmd_head);
        /* delete node from pending list */
        if (prev_node == node) {
            ril_cntx.ril_request_pending_list = node->next;
        } else {
            prev_node->next = node->next;
        }

#if 0
        BaseType_t ret;
        ril_message_t message;
        message.msg = RIL_MSG_REQUEST;
        message.buf = node->info;

        /* insert message to front of queue */
        ret = xQueueSendToFront(ril_queue, (void *)&message, 0);
        if (pdPASS != ret) {
            RIL_LOGE("failed to send queue to front\r\n");
            configASSERT(0);
        }
#endif
        ril_send_message_int(RIL_MSG_REQUEST, node->info, sizeof(ril_request_info_t));

        ril_mem_free(node);
        ril_cntx.num_pending_items--;
    }

    return 0;
}


bool ril_channel_urc_is_match_response_pattern(char *cur_pos)
{
    char *cmd_head;
    uint32_t info_rsp_mask;
    ril_cmd_item_t *cmd_item;

    if ((cmd_item = get_at_cmd_item((ril_cmd_id_t)ril_channel[RIL_URC_CHANNEL_ID - 1].curr_request_id)) == NULL) {
        return false;
    }
    if (cmd_item->cmd_id == RIL_CMD_ID_CUSTOM_CMD_URC || cmd_item->cmd_id == RIL_CMD_ID_CUSTOM_CMD) {
        cmd_head = ril_cntx.custom_cmd_head;
        info_rsp_mask = ril_cntx.custom_cmd_info_mask;
    } else {
        cmd_head = cmd_item->cmd_head;
        info_rsp_mask = cmd_item->info_rsp_mask;
        
    }
    if (cmd_head != NULL && 
        str_starts_with(cur_pos, cmd_head) &&
        ((1 << (ril_channel[RIL_URC_CHANNEL_ID - 1].curr_request_mode)) & info_rsp_mask)) {
        return true;
    } else {
        return false;
    }
}


void ril_replace_special_term(char *cmd_buf, const char *pattern, const char *replace_pattern, int32_t pattern_len)
{
    char *delim, *ptr;
    ptr = cmd_buf;

    delim = strstr(ptr, pattern);
    while (delim != NULL) {
        strncpy(delim, replace_pattern, pattern_len);
        ptr += pattern_len;
        delim = strstr(ptr, pattern);
    }
}


void ril_precheck_special_term(char *cmd_buf)
{
    const char *echo_pattern = "ATE0\r";
    const char *echo_replace_pattern = "\r\nE\r\n";
    int32_t echo_pattern_len = strlen(echo_replace_pattern);

    //const char *input_pattern = "\r\n> ";
    //const char *input_replace_pattern = "\r\n\r\n";
    //int32_t input_pattern_len = strlen(input_replace_pattern);

    ril_replace_special_term(cmd_buf, echo_pattern, echo_replace_pattern, echo_pattern_len);
    //ril_replace_special_term(cmd_buf, input_pattern, input_replace_pattern, input_pattern_len);
}


bool ril_channel_urc_hdlr(char *cmd_buf, uint32_t cmd_buf_len)
{
    ril_message_type_t msg;
    char *curr_packet = NULL;
    char *next_packet = NULL;
    //char *cmd_buf = ril_channel[RIL_URC_CHANNEL_ID - 1].rx_buf_ptr;
    //uint32_t cmd_buf_len = ril_channel[RIL_URC_CHANNEL_ID - 1].rx_pos;
    uint32_t curr_packet_len;
    char *cur_pos;
    bool no_wait_response = false;
    bool further_match = false;
    ril_cmd_item_t *cmd_item = get_at_cmd_item((ril_cmd_id_t)ril_channel[RIL_URC_CHANNEL_ID - 1].curr_request_id);

    if (ril_channel[RIL_URC_CHANNEL_ID - 1].curr_request_id == 0) {
        no_wait_response = true;
    }

    ril_precheck_special_term(cmd_buf);
    curr_packet = cmd_buf;
    do {
        /* retrieve one line of string, and figure out its length */
        next_packet = at_get_next_packet(curr_packet);
        if (next_packet == NULL) {
            curr_packet_len = cmd_buf_len - (curr_packet - cmd_buf);

            if (curr_packet[curr_packet_len - 2] != '\r' || curr_packet[curr_packet_len - 1] != '\n') {
                if (is_cmd_echo(curr_packet, cmd_item->cmd_head)) {
                    /* echo string, ignore */
                    return true;
                } else {
                    /* next packet is empty, but the tail is not <CR><LF>, so it may be incomplete buf */
                    RIL_LOGI("the tail of curr packet is not <CR><LF>");
                    return false;
                }
            }
        } else {
            curr_packet_len = next_packet - curr_packet;
        }

        if (no_wait_response) {
            /* not wait any response, all strings should be URC */
            msg = RIL_MSG_URC;
        } else {
            cur_pos = curr_packet;
            skip_white_space(&cur_pos);
            if (is_final_response(cur_pos, NULL)) {
                /* this line is final response pattern, it must be an AT response */
                msg = RIL_MSG_RESPONSE;
            } else {
                if (ril_channel_urc_is_match_response_pattern(cur_pos)) {
                    /* start with "+CXXX:", and it's equal to the cmd head that is waiting for its AT response  */
                    /* it's read mode or test mode, so response may includes information response */

                    do {
                        further_match = false;
                        if (next_packet == NULL) {
                            /* next packet is empty, so it may be incomplete buf, need to wait final response */
                            RIL_LOGI("cmd buf is incomplete!\r\n");
                            return false;
                        } else {
                            cur_pos = next_packet;
                            skip_white_space(&cur_pos);
                            if (is_final_response(cur_pos, NULL)) {
                                /* next packet is final response pattern, so it should combine with the prev packet together as an AT response */
                                msg = RIL_MSG_RESPONSE;
                                next_packet = at_get_next_packet(cur_pos);
                                if (next_packet == NULL) {
                                    curr_packet_len = cmd_buf_len - (curr_packet - cmd_buf);
                                } else {
                                    curr_packet_len = next_packet - curr_packet;
                                }
                            } else {
                                /* next packet isn't final response pattern */
                                if (!cmd_item->is_multiline) {
                                    /* if multiline response doesn't exist, current packet should be an URC */
                                    msg = RIL_MSG_URC;
                                } else if (ril_channel_urc_is_match_response_pattern(cur_pos)) {
                                    /* multiline response case, need further match */
                                    further_match = true;
                                    next_packet = at_get_next_packet(cur_pos);
                                } else {
                                    /* next packet is not matched with response pattern, current packet should be an URC */
                                    msg = RIL_MSG_URC;
                                }
                            }
                        }
                    } while (further_match);

                } else {
                    if (is_cmd_echo(cmd_buf, cmd_item->cmd_head)) {
                        /* echo string, ignore */
                        return true;
                    }

                    /* start with "+CXXX:", but it's not equal to the cmd head that is waiting for its AT response */
                    /* or it's execute mode or active mode, so we expect there is no information response to wait */
                    /* so in thise case, current packet should be an URC */
                    msg = RIL_MSG_URC;
                }
            }
        }

        ril_send_packet_message(msg,
                                RIL_URC_CHANNEL_ID,
                                curr_packet,
                                curr_packet_len);
        if (next_packet != NULL) {
            RIL_LOGI("move next packet to head, cmd_buf_len(%d), curr_packet_len(%d)", cmd_buf_len, curr_packet_len);
            memmove(curr_packet, next_packet, cmd_buf_len - curr_packet_len);
            curr_packet[cmd_buf_len - curr_packet_len] = '\0';
        }
        cmd_buf_len -= curr_packet_len;
        ril_channel[RIL_URC_CHANNEL_ID - 1].rx_pos = cmd_buf_len;
        RIL_LOGI("curr packet done, left len(%d)", cmd_buf_len);
    } while (cmd_buf_len > 0);

    return true;
}


bool ril_channel_response_hdlr(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    ril_message_type_t msg;
    ril_cmd_item_t *cmd_item;
    ril_result_code_t res_code;
    char *line = cmd_buf;

    ril_precheck_special_term(line);
    if (is_cmd_response(line, &res_code)) {
        /* AT response */
        if (res_code == RIL_RESULT_CODE_CONNECT) {
            /* this channel has been changed to data mode, forbid data receiving temporary */
            RIL_LOGW("forbid data receiving in channel(%d)", channel_id);
            ril_channel_forbid_data_receiving(channel_id, true);
        }
        msg = RIL_MSG_RESPONSE;
    } else {
        if (is_default_enabled_urc(line)) {
            RIL_LOGW("default enabled urc was unnecessary to handle in AT channel");
            return true;
        } else {
            /* check if it is command echo */
            cmd_item = get_at_cmd_item((ril_cmd_id_t)ril_channel[channel_id - 1].curr_request_id);
            if (cmd_item == NULL) {
                RIL_LOGW("this is an unexpected response, drop it!\r\n");
                return true;
            }
            if (is_cmd_echo(line, cmd_item->cmd_head)) {
                return true;
            } else {
                /* command is incomplete, don't clear rx buf */
                RIL_LOGW("cmd buf is incomplete\r\n");
                return false;
            }
        }
    }

    ril_send_packet_message(msg,
                            channel_id,
                            cmd_buf, // ril_channel[channel_id - 1].rx_buf_ptr,
                            cmd_buf_len); // ril_channel[channel_id - 1].rx_pos);
    return true;
}


void ril_channel_mux_callback(mux_ap_event_t event, void *param)
{
    mux_ap_channel_id_t channel_id;

    switch (event) {
        case MUX_AP_EVENT_CHANNEL_ENABLED: {
            mux_ap_event_channel_enabled_t *p = (mux_ap_event_channel_enabled_t *)param;
            channel_id = p->channel_id;
            ril_send_message(RIL_MSG_CHANNEL_ENABLED, (void *)channel_id, sizeof(uint32_t));
            break;
        }
        case MUX_AP_EVENT_CHANNEL_DISABLED: {
            mux_ap_event_channel_disabled_t *p = (mux_ap_event_channel_disabled_t *)param;
            channel_id = p->channel_id;
            ril_send_message(RIL_MSG_CHANNEL_DISABLED, (void *)channel_id, sizeof(uint32_t));
            break;
        }
        case MUX_AP_EVENT_SEND_COMPLETED: {
            /* free tx buffer */
            mux_ap_event_send_completed_t *p = (mux_ap_event_send_completed_t *)param;
            channel_id = p->channel_id;
            ril_send_message(RIL_MSG_SEND_COMPLETE, (void *)channel_id, sizeof(uint32_t));
            break;
        }
        case MUX_AP_EVENT_PREPARE_TO_RECEIVE: {
            /* assign rx buffer */
            mux_ap_event_prepare_to_receive_t *p = (mux_ap_event_prepare_to_receive_t *)param;
            ril_channel_cntx_t *channel_ptr; 

            channel_id = p->channel_id;
            channel_ptr = &ril_channel[channel_id - 1]; 
            if (p->buffer_length + ril_channel[channel_id - 1].rx_pos <= RIL_BUF_SIZE - 1) {
                /* clean rx buffer before receiving data from mux layer. */
                memset((uint8_t *)(ril_channel[channel_id - 1].rx_buf_ptr + ril_channel[channel_id - 1].rx_pos), 0x00, p->buffer_length + 1);
                p->data_buffer = (uint8_t *)(ril_channel[channel_id - 1].rx_buf_ptr + ril_channel[channel_id - 1].rx_pos);

            } else {
                /* rx buffer is not enough. */
                int32_t hold_buf_len;
                if (channel_ptr->rx_long_str_len == 0 && channel_ptr->rx_long_str_ptr == NULL) {
                    hold_buf_len = channel_ptr->rx_pos;
                    channel_ptr->rx_long_str_len = channel_ptr->rx_pos;
                    channel_ptr->rx_long_str_ptr = ril_mem_malloc(channel_ptr->rx_long_str_len + p->buffer_length + 1);
                    memcpy(channel_ptr->rx_long_str_ptr, channel_ptr->rx_buf_ptr, channel_ptr->rx_pos);
                } else {
                    hold_buf_len = channel_ptr->rx_long_str_len;
                    channel_ptr->rx_long_str_ptr = ril_mem_realloc(channel_ptr->rx_long_str_ptr, channel_ptr->rx_long_str_len + p->buffer_length + 1);
                }
                RIL_LOGI("prepared_buffer: 0x%x, required_buffer_length: %d, hold_buf_len: %d\r\n", channel_ptr->rx_long_str_ptr, p->buffer_length, hold_buf_len);
                p->data_buffer = (uint8_t *)channel_ptr->rx_long_str_ptr + hold_buf_len;
            }
            break;
        }
        case MUX_AP_EVENT_RECEIVE_COMPLETED: {
            bool need_clear_rx = false;
            mux_ap_event_receive_completed_t *p = (mux_ap_event_receive_completed_t *)param;
            ril_channel_cntx_t *channel_ptr;
            char *cmd_buf;
            uint32_t cmd_buf_len;

            channel_id = p->channel_id;
            channel_ptr = &ril_channel[channel_id - 1];
            RIL_LOGI("receive completed, channel_id: %d, receive length: %lu\r\n", (int)channel_id, p->buffer_length);
            
            RIL_LOGDUMPSTR("received buff: %s\r\n", p->buffer_length, p->data_buffer);
            //M_FrGkiString (0xEC1B, GKI_RIL_INFO, "[RIL] received buf: %s", p->data_buffer);
           
            if (channel_ptr->recv_forbidden) {
                ril_result_code_t res_code;
                RIL_LOGE("drop data!!! due to channel(%d) data receiving forbidden", channel_id);
                if (is_cmd_response(channel_ptr->rx_buf_ptr, &res_code)) {
                    if (res_code != RIL_RESULT_CODE_CONNECT) {
                        RIL_LOGCT("channel(%d) forbidden flag recovery", channel_id);
                        ril_channel_forbid_data_receiving(channel_id, false);
                    }
                }
                need_clear_rx = false;
            }

            if (!channel_ptr->recv_forbidden) {
                vTaskSuspendAll();
                if (channel_ptr->rx_long_str_len == 0) {
                    channel_ptr->rx_pos += p->buffer_length;
                    cmd_buf = channel_ptr->rx_buf_ptr;
                    cmd_buf_len = channel_ptr->rx_pos;
                } else {
                    channel_ptr->rx_long_str_len += p->buffer_length;
                    cmd_buf = channel_ptr->rx_long_str_ptr;
                    cmd_buf_len = channel_ptr->rx_long_str_len;
                }
                cmd_buf[cmd_buf_len] = '\0';
                xTaskResumeAll();

                if (channel_id == RIL_URC_CHANNEL_ID) {
                    need_clear_rx = ril_channel_urc_hdlr(cmd_buf, cmd_buf_len);
                } else {
                    need_clear_rx = ril_channel_response_hdlr(channel_id, cmd_buf, cmd_buf_len);
                }
            }

            vTaskSuspendAll();
            if (need_clear_rx) {
                memset(channel_ptr->rx_buf_ptr, 0x00, RIL_BUF_SIZE);
                channel_ptr->rx_pos = 0;
                if (channel_ptr->rx_long_str_len != 0) {
                    ril_mem_free(channel_ptr->rx_long_str_ptr);
                    channel_ptr->rx_long_str_len = 0;
                    channel_ptr->rx_long_str_ptr = NULL;
                }
            }
            xTaskResumeAll();
            configASSERT(ril_channel_buf_is_valid(channel_id));
            break;
        }
        default:
            RIL_LOGE("Not support mux event\r\n");
            //configASSERT(0);
            break;
    }
}

