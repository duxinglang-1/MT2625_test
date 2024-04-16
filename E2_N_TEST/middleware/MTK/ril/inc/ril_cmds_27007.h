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

#ifndef __RIL_CMDS_27007_H__
#define __RIL_CMDS_27007_H__

#include "ril_general_types.h"


/* AT cmd: AT+CGMI */
extern int32_t ril_response_manufacturer_identification(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+CGMM */
extern int32_t ril_response_model_identification(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+CGMR */
extern int32_t ril_response_revision_identification(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+CGSN */
extern int32_t ril_response_serial_number(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+CSCS */
extern int32_t ril_response_select_charcter_set(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+CIMI */
extern int32_t ril_response_imsi(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+CMUX */
extern int32_t ril_response_multiplexing_mode(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+CR */
extern int32_t ril_response_service_reporting_control(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+CEER */
extern int32_t ril_response_ceer_extended_error_report(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CNUM */
extern int32_t ril_response_subscriber_number(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+CREG */
extern int32_t ril_response_creg_network_registration(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+COPS */
extern int32_t ril_response_cops_plmn_selection(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CLCK */
extern int32_t ril_response_clck_facility_lock(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CPWD */
extern int32_t ril_response_cpwd_change_password(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CPOL */
extern int32_t ril_response_cpol_preferred_plmn_list(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CPLS */
extern int32_t ril_response_cpls_selection_of_preferred_plmn_list(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+COPN */
extern int32_t ril_response_copn_read_operator_names(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CFUN */
extern int32_t ril_response_cfun_set_phone_functionality(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CPIN */
extern int32_t ril_response_cpin_enter_pin(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CSQ */
extern int32_t ril_response_csq_signal_quality(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CCLK */
extern int32_t ril_response_cclk_clock(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CSIM */
extern int32_t ril_response_csim_generic_sim_access(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CRSM */
extern int32_t ril_response_crsm_restricted_sim_access(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CMAR */
extern int32_t ril_response_cmar_master_reset(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CTZU */
extern int32_t ril_response_ctzu_automatic_time_zone_update(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CTZR */
extern int32_t ril_response_ctzr_time_zone_reporting(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CGPIAF */
extern int32_t ril_response_cgpiaf_printing_ip_address_format(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CPINR */
extern int32_t ril_response_cpinr_remaining_pin_retries(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CSUS */
extern int32_t ril_response_csus_set_card_slot(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CESQ */
extern int32_t ril_response_cesq_extended_signal_quality(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CMEE */
extern int32_t ril_response_cmee_report_mobile_termination_error(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CGDCONT */
extern int32_t ril_response_cgdcont_define_pdp_context(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CGTFT */
extern int32_t ril_response_cgtft_traffic_flow_template(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CGATT */
extern int32_t ril_response_cgatt_ps_attach_or_detach(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CGACT */
extern int32_t ril_response_cgact_pdp_context_activate_or_deactivate(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CGDATA */
extern int32_t ril_response_cgdata_enter_data_state(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CGPADDR */
extern int32_t ril_response_cgpaddr_show_pdp_address(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CGEREP */
extern int32_t ril_response_cgerep_packet_domain_event_reporting(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CGREG */
extern int32_t ril_response_cgreg_gprs_network_registration_status(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CEREG */
extern int32_t ril_response_cereg_eps_network_registration_status(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CGCONTRDP */
extern int32_t ril_response_cgcontrdp_pdp_context_read_dynamic_parameters(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CGDEL */
extern int32_t ril_response_cgdel_delete_non_active_pdp_contexts(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CGAUTH */
extern int32_t ril_response_cgauth_define_pdp_context_authentication_parameters(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CIPCA */
extern int32_t ril_response_cipca_initial_pdp_context_activation(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CPSMS */
extern int32_t ril_response_cpsms_power_saving_mode_setting(uint32_t channel_id, char *cmd_buf,  uint32_t cmd_buf_len);

/* AT cmd: AT+CCIOTOPT*/
extern int32_t ril_response_ciot_optimisation_configuration(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+CEDRXS */
extern int32_t ril_response_eDRX_setting(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+CEDRXRDP */
extern int32_t ril_response_read_eDRX_dynamic_parameters(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+CGAPNRC */
extern int32_t ril_response_APN_rate_control(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+CSCON */
extern int32_t ril_response_signaling_connection_status(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+CCHO */
extern int32_t ril_response_open_uicc_logical_channel(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+CCHC */
extern int32_t ril_response_close_uicc_logical_channel(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+CGLA */
extern int32_t ril_response_generic_uicc_logical_channel_access(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+CRCES */
extern int32_t ril_response_read_coverage_enhancement_status(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);


/* URC: +CREG */
extern int32_t ril_urc_creg_network_registration(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* URC: +CTZR */
extern int32_t ril_urc_ctzr_time_zone_reporting(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* URC: +CGEV*/
extern int32_t ril_urc_cgev_packet_domain_event_reporting(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* URC: +CGREG */
extern int32_t ril_urc_cgreg_gprs_network_registration_status(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* URC: +CEREG */
extern int32_t ril_urc_cereg_eps_network_registration_status(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* URC: +CCIOTOPT */
extern int32_t ril_urc_ciot_optimisation_configuration(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* URC: +CEDRXP */
extern int32_t ril_urc_eDRX_setting(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* URC: +CPIN */
extern int32_t ril_urc_enter_pin(ril_urc_id_t cmd_id,char * cmd_buf,uint32_t cmd_buf_len);

/* URC: +CSCON */
extern int32_t ril_urc_signaling_connection_status(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);


/*************************************************
 *                 UT test case
 *************************************************/
extern int32_t ril_response_ut_callback_27007(ril_cmd_response_t *cmd_response);

#endif /* __RIL_CMDS_27007_H__ */

