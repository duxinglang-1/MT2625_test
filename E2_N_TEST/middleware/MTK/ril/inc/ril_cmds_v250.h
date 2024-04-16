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

#ifndef __RIL_CMDS_250_H__
#define __RIL_CMDS_250_H__

#include "ril_general_types.h"

extern int32_t ril_cmd_send_basic_hdlr(uint32_t channel_id, char *buf, ril_request_info_t *info);

extern int32_t ril_cmd_send_head_only_hdlr(uint32_t channel_id, char *buf, ril_request_info_t *info);

extern int32_t ril_response_dial(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_command_echo(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_terminates_a_connection(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_request_identification_info(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_set_volume_of_speaker(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_return_to_online_data(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_select_pulse_dialing(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_set_result_suppression_mode(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_automatic_answer(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_command_line_termination_char(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_response_formatting_char(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_command_line_editing_char(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_pause_before_blind_dialing(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_connection_completion_timeout(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_comma_dial_modifier_time(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_automatic_disconnect_delay(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_select_tone_dialling(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_dce_response_format(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_result_code_selection(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_reset_to_default_config(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_set_to_factory_defined_config(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_fixed_dte_rate(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_dte_dce_character_framing(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_bit_data_compression(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_complete_capabilities_list(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_escape_from_data_mode(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_reissue_the_last_command_given(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_request_packet_domain_service(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_redial(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_monitor_speaker_mode(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_automode_enable(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_ring_counter(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_escape_character(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_escape_prompt_delay(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_delay_to_dtr(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_extended_result_codes(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_received_line_signal_detector(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_data_terminal_ready(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_flow_control_option(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_view_current_profile(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_save_current_profile(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_data_compression_reporting(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_global_obj_identification(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_product_serial_number_identification(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_dte_dce_local_flow_control(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

extern int32_t ril_response_dte_dce_local_rate_reporting(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

// UT test case
extern int32_t ril_response_ut_callback_v250(ril_cmd_response_t * cmd_response);


#endif /*__RIL_CMDS_250_H__*/

