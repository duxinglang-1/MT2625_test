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

#include "apb_proxy_queue_unit_test.h"
#include "apb_proxy_queue.h"
#include "FreeRTOS.h"

typedef struct {
    uint32_t item1;
    uint8_t  item2;
    uint32_t item3;
} apb_proxy_item_test_t;

bool apb_proxy_queue_unit_test(void)
{
    uint32_t queue_capacity = 20;
    uint32_t index = 0;
    apb_proxy_item_test_t test_msg1 =
    {100, 9 , 2017};
    apb_proxy_item_test_t test_msg2 =
    {2077, 135 , 2016};
    apb_proxy_item_test_t readed_msg;
    apb_proxy_queue_handle_t queue_handle = apb_proxy_queue_create(sizeof(apb_proxy_item_test_t), queue_capacity);
    configASSERT(queue_handle != NULL);
    configASSERT(apb_proxy_queue_get_available_space(queue_handle) == queue_capacity);
    configASSERT(apb_proxy_queue_get_occupied_space(queue_handle) == 0);
    configASSERT(apb_proxy_queue_get_capacity(queue_handle) == queue_capacity);

    /*push one message test*/
    configASSERT(apb_proxy_queue_push_msg(queue_handle, &test_msg1) == APB_PROXY_STATUS_OK);
    configASSERT(apb_proxy_queue_get_available_space(queue_handle) == (queue_capacity - 1));
    configASSERT(apb_proxy_queue_get_occupied_space(queue_handle) == 1);
    configASSERT(apb_proxy_queue_get_capacity(queue_handle) == queue_capacity);
    /*popup one message test*/
    configASSERT(apb_proxy_queue_pop_msg(queue_handle, &readed_msg) == APB_PROXY_STATUS_OK);
    configASSERT(readed_msg.item1 == test_msg1.item1);
    configASSERT(readed_msg.item2 == test_msg1.item2);
    configASSERT(readed_msg.item3 == test_msg1.item3);
    configASSERT(apb_proxy_queue_get_available_space(queue_handle) == queue_capacity);
    configASSERT(apb_proxy_queue_get_occupied_space(queue_handle) == 0);
    configASSERT(apb_proxy_queue_get_capacity(queue_handle) == queue_capacity);
    /*push two message test*/
    configASSERT(apb_proxy_queue_push_msg(queue_handle, &test_msg1) == APB_PROXY_STATUS_OK);
    configASSERT(apb_proxy_queue_push_msg(queue_handle, &test_msg2) == APB_PROXY_STATUS_OK);
    configASSERT(apb_proxy_queue_get_available_space(queue_handle) == (queue_capacity - 2));
    configASSERT(apb_proxy_queue_get_occupied_space(queue_handle) == 2);
    configASSERT(apb_proxy_queue_get_capacity(queue_handle) == queue_capacity);
    /*pop up two message test*/
    configASSERT(apb_proxy_queue_pop_msg(queue_handle, &readed_msg) == APB_PROXY_STATUS_OK);
    configASSERT(apb_proxy_queue_pop_msg(queue_handle, &readed_msg) == APB_PROXY_STATUS_OK);
    configASSERT(readed_msg.item1 == test_msg2.item1);
    configASSERT(readed_msg.item2 == test_msg2.item2);
    configASSERT(readed_msg.item3 == test_msg2.item3);

    for (index = 0; index < queue_capacity; index++) {
        configASSERT(apb_proxy_queue_push_msg(queue_handle, &test_msg1) == APB_PROXY_STATUS_OK);
    }

    configASSERT(apb_proxy_queue_get_available_space(queue_handle) == 0);
    configASSERT(apb_proxy_queue_get_occupied_space(queue_handle) == queue_capacity);
    configASSERT(apb_proxy_queue_get_capacity(queue_handle) == queue_capacity);
    /*message queue full test.*/
    configASSERT(apb_proxy_queue_push_msg(queue_handle, &test_msg1) == APB_PROXY_STATUS_ERROR);
    configASSERT(apb_proxy_queue_get_available_space(queue_handle) == 0);
    configASSERT(apb_proxy_queue_get_occupied_space(queue_handle) == queue_capacity);
    configASSERT(apb_proxy_queue_get_capacity(queue_handle) == queue_capacity);

    for (index = 0; index < queue_capacity; index++) {
        configASSERT(apb_proxy_queue_pop_msg(queue_handle, &readed_msg) == APB_PROXY_STATUS_OK);
    }

    configASSERT(apb_proxy_queue_pop_msg(queue_handle, &readed_msg) == APB_PROXY_STATUS_ERROR);

    apb_proxy_queue_delete(queue_handle);
    queue_handle = NULL;

    return true;
}

