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

#ifndef __MUX_AP_PRIVATE_H__
#define __MUX_AP_PRIVATE_H__

#include <stdint.h>
#include <stdbool.h>

#include "syslog.h"
#include "mux_ap.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined (MTK_DEBUG_LEVEL_NONE)
#define MUX_AP_LOGI(fmt, args...)   LOG_I(mux_ap, "[MUX_AP] "fmt, ##args)
#define MUX_AP_LOGW(fmt, args...)   LOG_W(mux_ap, "[MUX_AP] "fmt, ##args)
#define MUX_AP_LOGE(fmt, args...)   LOG_E(mux_ap, "[MUX_AP] "fmt, ##args)
#else
#define MUX_AP_LOGI(fmt, args...)   printf("[MUX_AP] "fmt"\r\n", ##args)
#define MUX_AP_LOGW(fmt, args...)   printf("[MUX_AP] "fmt"\r\n", ##args)
#define MUX_AP_LOGE(fmt, args...)   printf("[MUX_AP] "fmt"\r\n", ##args)
#endif

#define MUX_AP_SLEEP_MANAGER_LOCK_CATEGORY_RESEND_LIST  1
#define MUX_AP_SLEEP_MANAGER_LOCK_CATEGORY_RECEIVE_LIST 2

void mux_ap_send_line_status(mux_ap_channel_id_t channel_id, bool rts_on);

void mux_ap_channel_manager_set_ready(mux_ap_channel_id_t channel_id, bool is_ready);

void mux_ap_data_manager_send_completed(mux_ap_channel_id_t channel_id, uint8_t *data_buffer, uint32_t buffer_length, void *user_data);

void mux_ap_data_manager_resend(void);

bool mux_ap_channel_manager_is_stop_to_send(mux_ap_channel_id_t channel_id);

void mux_ap_channel_manager_set_stop_to_send(mux_ap_channel_id_t channel_id, bool is_stop_to_send);

void mux_ap_data_manager_prepare_to_receive(mux_ap_event_prepare_to_receive_t *prepare_to_receive);

void mux_ap_data_manager_receive_completed(mux_ap_channel_id_t channel_id, uint8_t *data_buffer, uint32_t buffer_length, void *user_data, mux_ap_callback_t user_callback);

void mux_ap_data_manager_continue_to_receive(void);

bool mux_ap_data_manager_is_stop_to_receive(mux_ap_channel_id_t channel_id);

void mux_ap_data_manager_resume_to_receive_from_receive_list(mux_ap_channel_id_t channel_id);

void mux_ap_data_manager_clear_is_receiving_data(void);

mux_ap_callback_t mux_ap_data_manager_get_user_callback(mux_ap_channel_id_t channel_id);

void mux_ap_sleep_manager_lock_sleep(uint8_t lock_category);

void mux_ap_sleep_manager_unlock_sleep(uint8_t lock_category);

#ifdef __cplusplus
}
#endif

#endif /* __MUX_AP_PRIVATE_H__ */

