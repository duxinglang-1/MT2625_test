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

#include "memory_attribute.h"

#include "ril.h"
#include "ril_log.h"
#include "ril_cmds_def.h"

/* rserve 80 bytes RTC RAM to store URC callback information */
#define RIL_EVENT_CALLBACK_MAX_USER    (10)

typedef struct {
    uint32_t group_mask;
    ril_event_callback_t callback;
} ril_event_callback_node_t;

ATTR_RWDATA_IN_RETSRAM ril_event_callback_node_t ril_event_callback_list[RIL_EVENT_CALLBACK_MAX_USER] = {{0}};

ril_status_t ril_register_event_callback(uint32_t group_mask, ril_event_callback_t callback)
{
    int32_t i;
    int32_t available_idx = -1;
    for (i = 0; i < RIL_EVENT_CALLBACK_MAX_USER; i++) {
        if (ril_event_callback_list[i].callback == callback) {
            /* callback has been registered already */
            return RIL_STATUS_SUCCESS;
        }

        if (available_idx < 0 && ril_event_callback_list[i].callback == NULL) {
            /* find available entry */
            available_idx = i;
        }
    }

    if (available_idx == -1) {
        RIL_LOGE("register callback is full.\r\n");
        return RIL_STATUS_FAIL;
    } else {
        ril_event_callback_list[available_idx].group_mask = group_mask;
        ril_event_callback_list[available_idx].callback = callback;
        return RIL_STATUS_SUCCESS;
    }
}


ril_status_t ril_deregister_event_callback(ril_event_callback_t callback)
{
    int32_t i;
    for (i = 0; i < RIL_EVENT_CALLBACK_MAX_USER; i++) {
        if (ril_event_callback_list[i].callback == callback) {
            ril_event_callback_list[i].callback = 0;
            ril_event_callback_list[i].group_mask = 0;
            return RIL_STATUS_SUCCESS;
        }
    }

    if (i == RIL_EVENT_CALLBACK_MAX_USER) {
        RIL_LOGE("callback not found.\r\n");
    }

    return RIL_STATUS_FAIL;
}


void ril_notify_event(uint32_t event_id, void *param, uint32_t param_len)
{
    ril_urc_cmd_item_t *urc_item;
    uint32_t group_mask;
    int32_t i;

    urc_item = get_urc_cmd_item((ril_urc_id_t)event_id);
    group_mask = urc_item->func_group;
    for (i = 0; i < RIL_EVENT_CALLBACK_MAX_USER; i++) {
        if (ril_event_callback_list[i].callback != NULL && ril_event_callback_list[i].group_mask & group_mask) {
            ril_event_callback_list[i].callback((ril_urc_id_t)event_id, param, param_len);
        }
    }
}

void ril_init_event_callback_table()
{
    memset(&ril_event_callback_list, 0x00, sizeof(ril_event_callback_list));
}

