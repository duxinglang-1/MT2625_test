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
#ifndef __NIDD_INTERNAL__
#define __NIDD_INTERNAL__

#include "stdint.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "syslog.h"
#include "task.h"
#include "queue.h"

#include "nidd_gprot.h"

typedef enum {
    NIDD_MSG_CHANNEL_ACTIVATE_IND,
    NIDD_MSG_CHANNEL_DEACTIVATE_IND,
    NIDD_MSG_MUX_DATA_IND,
    NIDD_MSG_MUX_SEND_DATA_REQ,
    NIDD_MSG_MUX_SEND_COMPLETE_IND,

    NIDD_MSG_MAX
} nidd_message_t;

typedef enum {
    NIDD_STATUS_INIT = 0,
    NIDD_STATUS_ACTIVATED,
    NIDD_STATUS_DEACTIVATED,

    NIDD_STATUS_MAX
} nidd_status_t;


typedef struct _nidd_channel_struct {
    char*                       apn;
    uint32_t                    nidd_id;
    uint32_t                    channel_id;
    uint8_t                     is_sending;
    nidd_status_t               status;
    nidd_event_handler          callback;
    struct _nidd_channel_struct* next;
} nidd_channel_struct;

typedef struct _nidd_context_struct {
    QueueHandle_t        queue_id;
    uint8_t              count;
    nidd_channel_struct* channel_list;
} nidd_context_struct;

typedef struct _nidd_general_msg_t {
    uint8_t      msg_id;
    void*        msg_data;
} nidd_general_msg_t;

typedef struct _nidd_mux_send_data_t {
    uint32_t                  channel_id;
    uint8_t*                  data_buffer;
    uint32_t                  data_length;
} nidd_mux_send_data_t;

typedef struct _nidd_mux_send_complete_t {
    uint8_t*                  data_buffer;
    void*                     user_data;
} nidd_mux_send_complete_t;



#define NIDD_THREAD_NAME                     ("nidd")
#define NIDD_THREAD_STACKSIZE                (4 * 1024)
#define NIDD_THREAD_PRIO                     (TASK_PRIORITY_ABOVE_NORMAL)  // The more the number, the higher the priority in FreeRTOS

#define NIDD_MAX_CNT                         (2)  //(5)
#define NIDD_QUEUE_LENGTH                    (10)

extern nidd_channel_struct* nidd_get_channel_by_apn(char* apn);
extern nidd_channel_struct* nidd_get_channel_by_channelid(uint32_t channel_id);
extern nidd_channel_struct* nidd_get_channel_by_id(uint32_t nidd);

extern void* nidd_mem_alloc(uint32_t size);
extern void nidd_mem_free(void *buf);
#endif /* __NIDD_INTERNAL__ */
