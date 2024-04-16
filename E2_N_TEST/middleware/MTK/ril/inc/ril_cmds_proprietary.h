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

#ifndef __RIL_CMDS_PROPRIETARY_H__
#define __RIL_CMDS_PROPRIETARY_H__
#include "ril_general_types.h"

/* AT cmd: AT*MLTS */
extern int32_t ril_response_get_local_timestamp_and_network_info(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSIMINS */
extern int32_t ril_response_sim_inserted_status_reporting(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSPN */
extern int32_t ril_response_get_service_provider_name_from_sim(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MUNSOL */
extern int32_t ril_response_extra_unsolicited_indications(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSQF */
extern int32_t ril_response_set_filter_for_CSQN_notifications(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MGCOUNT */
extern int32_t ril_response_packet_domain_counters(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MLOOPPSD */
extern int32_t ril_response_activate_PSD_loopback_mode(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MGSINK */
extern int32_t ril_response_send_packet_to_discard(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MCGUSNPWD */
extern int32_t ril_response_MCGUSNPWD_pd_connection_config_username_pwd(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MCGDEFCONT */
extern int32_t ril_response_pdn_connection_set_default_psd_attach(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MGTCSINK */
extern int32_t ril_response_send_tcpip_packet_to_discard(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSACL */
extern int32_t ril_response_control_ACL_feature(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MLACL */
extern int32_t ril_response_display_ACL_list(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MWACL */
extern int32_t ril_response_write_ACL_entry(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MDACL */
extern int32_t ril_response_delete_ACL_entry(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSMEXTRAINFO */
extern int32_t ril_response_control_extra_info_on_sms(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSMEXTRAUNSOL */
extern int32_t ril_response_control_extra_unsolicited_messages(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSMSTATUS */
extern int32_t ril_response_obtain_status_of_sms_functionality(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MMGI */
extern int32_t ril_response_control_sms_unsolicited_indication(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MMGRW */
extern int32_t ril_response_sms_location_rewrite(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MMGSC */
extern int32_t ril_response_sms_location_status_change(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MUPIN */
extern int32_t ril_response_uicc_pin_information_access(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MUAPP */
extern int32_t ril_response_uicc_application_list_access(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSIMHSEL */
extern int32_t ril_response_sim_holder_select(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSST */
extern int32_t ril_response_read_sst_ust_from_usim(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MGID */
extern int32_t ril_response_read_gid_from_sim(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MABORT */
extern int32_t ril_response_abort_mm_related_at_command(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MCAL */
extern int32_t ril_response_radio_call(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MNON */
extern int32_t ril_response_network_operator_name(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MOPL */
extern int32_t ril_response_network_operator_plmn_list(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MMUX */
extern int32_t ril_response_mux_configuration(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MROUTEMMI */
extern int32_t ril_response_mmi_at_channel_routing_configuration(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MCEERMODE */
extern int32_t ril_response_ceer_response_mode(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MFTRCFG */
extern int32_t ril_response_modem_feature_configuration(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT^HVER */
extern int32_t ril_response_request_hw_version(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT^MODE */
extern int32_t ril_response_indicate_system_mode(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT^SYSINFO */
extern int32_t ril_response_request_system_information(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT^SYSCONFIG */
extern int32_t ril_response_configure_system_reference(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT^CARDMODE */
extern int32_t ril_response_request_sim_usim_mode(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT^SPN */
extern int32_t ril_response_read_service_provider_name(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSTLOCK */
extern int32_t ril_response_STK_registering(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSTPD */
extern int32_t ril_response_STK_terminal_profile_download(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSTMODE */
extern int32_t ril_response_STK_output_format_setting(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSTICREC */
extern int32_t ril_response_STK_obtain_icon_record(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSTICIMG */
extern int32_t ril_response_STK_obtain_icon_image(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSTC */
extern int32_t ril_response_STK_proactive_command_indication(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSTGC */
extern int32_t ril_response_STK_parameters_associated_with_proactive_command(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd:AT*MSTCR */
extern int32_t ril_response_STK_inform_response_to_proactive_command(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSTUD */
extern int32_t ril_response_STK_unsolicited_data(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSTMS */
extern int32_t ril_response_STK_menu_selection(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSTEV */
extern int32_t ril_response_STK_monitored_event_occurence(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MCCST */
extern int32_t ril_response_STK_unsolicited_response_for_call_control(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MICCID */
extern int32_t ril_response_read_usim_iccid(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MHOMENW */
extern int32_t ril_response_display_home_network_information(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MCSIMLOCK */
extern int32_t ril_response_lock_csim_access(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MEMMREEST */
extern int32_t ril_response_emm_re_establishment_test(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MAPNURI */
extern int32_t ril_response_apn_rate_control_indication(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MPLMNURI */
extern int32_t ril_response_plmn_rate_control_indication(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MPDI */
extern int32_t ril_response_packet_discard_indication(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MNBIOTDT */
extern int32_t ril_response_nbiot_data_type(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MNBIOTRAI */
extern int32_t ril_response_nbiot_release_assistance_indication(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MNVMQ */
extern int32_t ril_response_nvdm_status_query(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MNVMAUTH */
extern int32_t ril_response_nvdm_access_security_authorization(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MNVMW */
extern int32_t ril_response_nvdm_data_write(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MNVMR */
extern int32_t ril_response_nvdm_data_read(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MNVMGET */
extern int32_t ril_response_nvdm_get_all_data_item_id(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MNVMIVD */
extern int32_t ril_response_nvdm_data_item_invalidate(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MNVMRSTONE */
extern int32_t ril_response_nvdm_data_item_factory_reset(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MNVMRST */
extern int32_t ril_response_nvdm_factory_reset_all_data_item(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MNVMMDNQ */
extern int32_t ril_response_nvdm_query_num_of_mini_dump(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MNVMDNR */
extern int32_t ril_response_nvdm_read_mini_dump(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MNVMDNC */
extern int32_t ril_response_nvdm_clean_mini_dump(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+IDCFREQ */
extern int32_t ril_response_set_idc_frequency_range(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+IDCPWRBACKOFF */
extern int32_t ril_response_set_tx_power_back_off(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT+IDCTX2GPS */
extern int32_t ril_response_generate_periodic_tx_for_gps(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MCALDEV */
extern int32_t ril_response_enter_exit_rf_calibration_state(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MATWAKEUP */
extern int32_t ril_response_set_modem_wakeup_indication(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MBAND */
extern int32_t ril_response_query_modem_operating_band(uint32_t channel_id,char * cmd_buf,uint32_t cmd_buf_len);

/* AT cmd: AT*MENGINFO */
extern int32_t ril_response_query_network_state(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MFRCLLCK */
extern int32_t ril_response_frequency_and_cell_lock(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MSPCHSC */
extern int32_t ril_response_set_scrambling_algorithm_for_npdsch(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MDPDNP */
extern int32_t ril_response_default_pdn_parameter(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MEDRXCFG */
extern int32_t ril_response_eDRX_configuration(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MCELLINFO */
extern int32_t ril_response_serving_and_neighbor_cell_info(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT cmd: AT*MUPDIR */
extern int32_t ril_response_indicate_packet_to_track(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);



/* AT URC: *MLTS  */
extern int32_t ril_urc_get_local_timestamp_and_network_info(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: *MSIMINS */
extern int32_t ril_urc_SIM_inserted_status_reporting(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: *MGCOUNT */
extern int32_t ril_urc_packet_domain_counters(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: *MSMEXTRAUNSOL , +SMREADY */
extern int32_t ril_urc_control_extra_unsolicited_messages(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: *MMGI */
extern int32_t ril_urc_control_sms_unsolicited_indication(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: *MSMPUKBLKD */
extern int32_t ril_urc_sim_puk_blocked_unsolicited_indication(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);


/* AT URC: ^MODE */
extern int32_t ril_urc_indicate_system_mode(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: *MSTC */
extern int32_t ril_urc_STK_proactive_command_indication(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: *MSTUD */
extern int32_t ril_urc_STK_unsolicited_data(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: *MCCST */
extern int32_t ril_urc_STK_unsolicited_response_for_call_control(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: *MCSIMLOCK */
extern int32_t ril_urc_lock_csim_access(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: *MAPNURI */
extern int32_t ril_urc_apn_rate_control_indication(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: *MPLMNURI */
extern int32_t ril_urc_plmn_rate_control_indication(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: *MPDI */
extern int32_t ril_urc_packet_discard_indication(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: +IDCSTATUS */
extern int32_t ril_urc_set_idc_frequency_range(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: *MSQN */
extern int32_t ril_urc_signal_quality_report(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: *MATWAKEUP */
extern int32_t ril_urc_set_modem_wakeup_indication(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: *MATREADY */
extern int32_t ril_urc_modem_ready_indication(ril_urc_id_t cmd_id,char * cmd_buf,uint32_t cmd_buf_len);

/* AT URC: *MDPDNP */
extern int32_t ril_urc_default_pdn_parameter(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: *MUPDIR */
extern int32_t ril_urc_indicate_packet_to_track(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* AT URC: *MUPDI */
extern int32_t ril_urc_indicate_packet_delivery_status(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

/* UT test cases callback function */
extern int32_t ril_response_ut_callback_proprietary(ril_cmd_response_t *cmd_response);

#endif /* __RIL_CMDS_PROPRIETARY_H__ */

