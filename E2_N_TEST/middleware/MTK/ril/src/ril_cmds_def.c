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

#include "ril.h"
#include "ril_cmds_def.h"

#include "ril_cmds_common.h"
#include "ril_cmds_27007.h"
#include "ril_cmds_27005.h"
#include "ril_cmds_v250.h"
#include "ril_cmds_proprietary.h"

#include "ril_log.h"

#define RIL_DEFAULT_TIMEOUT_IN_MS    (3000)

#if 0
static const ril_result_code_info_t s_result_codes[] = {
#  include <ril_result_code.h>
};
#endif


static const ril_cmd_item_t s_at_cmds_tbl[] = {
    /* id | cmd head | func group | cmd type  | timeout in msec | send hdlr | rsp parser hdlr */
    {RIL_CMD_ID_CGMI,      "+CGMI",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_manufacturer_identification},
    {RIL_CMD_ID_CGMM,      "+CGMM",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_model_identification},
    {RIL_CMD_ID_CGMR,      "+CGMR",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_revision_identification},
    {RIL_CMD_ID_CGSN,      "+CGSN",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_serial_number},
    {RIL_CMD_ID_CSCS,      "+CSCS",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_select_charcter_set},
    {RIL_CMD_ID_CIMI,      "+CIMI",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_imsi},
    {RIL_CMD_ID_CMUX,      "+CMUX",      RIL_MUX,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_multiplexing_mode},
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {RIL_CMD_ID_CR,        "+CR",        RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_service_reporting_control},
#endif
    {RIL_CMD_ID_CEER,      "+CEER",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_ceer_extended_error_report},
    {RIL_CMD_ID_CNUM,      "+CNUM",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_subscriber_number},
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {RIL_CMD_ID_CREG,      "+CREG",      RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_creg_network_registration, RIL_INFO_RSP_READ_MODE, false},
#endif
    {RIL_CMD_ID_COPS,      "+COPS",      RIL_MM,   RIL_CMD_TYPE_AT_DATA, (3600* 1000),              NULL,                      ril_response_cops_plmn_selection},
    {RIL_CMD_ID_CLCK,      "+CLCK",      RIL_SIM,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_clck_facility_lock},
    {RIL_CMD_ID_CPWD,      "+CPWD",      RIL_SIM,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cpwd_change_password},
    {RIL_CMD_ID_CPOL,      "+CPOL",      RIL_MM,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cpol_preferred_plmn_list},
    {RIL_CMD_ID_CPLS,      "+CPLS",      RIL_MM,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cpls_selection_of_preferred_plmn_list},
    {RIL_CMD_ID_COPN,      "+COPN",      RIL_MM,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_copn_read_operator_names},
    {RIL_CMD_ID_CFUN,      "+CFUN",      RIL_PM,   RIL_CMD_TYPE_AT_DATA, (60 * 1000),               NULL,                      ril_response_cfun_set_phone_functionality},
    {RIL_CMD_ID_CPIN,      "+CPIN",      RIL_SIM,  RIL_CMD_TYPE_URC,     (10 * 1000),               NULL,                      ril_response_cpin_enter_pin, RIL_INFO_RSP_READ_MODE, false},
    {RIL_CMD_ID_CSQ,       "+CSQ",       RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_csq_signal_quality},
    {RIL_CMD_ID_CCLK,      "+CCLK",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cclk_clock},
    {RIL_CMD_ID_CSIM,      "+CSIM",      RIL_SIM,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_csim_generic_sim_access},
    {RIL_CMD_ID_CRSM,      "+CRSM",      RIL_SIM,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_crsm_restricted_sim_access},
    {RIL_CMD_ID_CMAR,      "+CMAR",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cmar_master_reset},
    {RIL_CMD_ID_CTZU,      "+CTZU",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_ctzu_automatic_time_zone_update},
    {RIL_CMD_ID_CTZR,      "+CTZR",      RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_ctzr_time_zone_reporting, RIL_INFO_RSP_READ_MODE, false},
    {RIL_CMD_ID_CGPIAF,    "+CGPIAF",    RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cgpiaf_printing_ip_address_format},
    {RIL_CMD_ID_CPINR,     "+CPINR",     RIL_SIM,  RIL_CMD_TYPE_AT_DATA, (5 * 1000),                NULL,                      ril_response_cpinr_remaining_pin_retries},
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {RIL_CMD_ID_CSUS,      "+CSUS",      RIL_SIM,  RIL_CMD_TYPE_AT_DATA, (10 * 1000),               NULL,                      ril_response_csus_set_card_slot},
#endif
    {RIL_CMD_ID_CESQ,      "+CESQ",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cesq_extended_signal_quality},
    {RIL_CMD_ID_CMEE,      "+CMEE",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cmee_report_mobile_termination_error},
    {RIL_CMD_ID_CGDCONT,   "+CGDCONT",   RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cgdcont_define_pdp_context},
    {RIL_CMD_ID_CGATT,     "+CGATT",     RIL_PD,   RIL_CMD_TYPE_AT_DATA, (60 * 1000),               NULL,                      ril_response_cgatt_ps_attach_or_detach},
    {RIL_CMD_ID_CGACT,     "+CGACT",     RIL_PD,   RIL_CMD_TYPE_AT_DATA, (60 * 1000),               NULL,                      ril_response_cgact_pdp_context_activate_or_deactivate},
    {RIL_CMD_ID_CGDATA,    "+CGDATA",    RIL_PD,   RIL_CMD_TYPE_AT_DATA, (60 * 1000),                NULL,                      ril_response_cgdata_enter_data_state},
    {RIL_CMD_ID_CGPADDR,   "+CGPADDR",   RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cgpaddr_show_pdp_address},
    {RIL_CMD_ID_CGEREP,    "+CGEREP",    RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cgerep_packet_domain_event_reporting, RIL_INFO_RSP_READ_MODE, false},
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {RIL_CMD_ID_CGREG,     "+CGREG",     RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cgreg_gprs_network_registration_status, RIL_INFO_RSP_READ_MODE, false},
#endif
    {RIL_CMD_ID_CEREG,     "+CEREG",     RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cereg_eps_network_registration_status, RIL_INFO_RSP_READ_MODE, false},
    {RIL_CMD_ID_CGCONTRDP, "+CGCONTRDP", RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cgcontrdp_pdp_context_read_dynamic_parameters},
    {RIL_CMD_ID_CGDEL,     "+CGDEL",     RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cgdel_delete_non_active_pdp_contexts},
    {RIL_CMD_ID_CGAUTH,    "+CGAUTH",    RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cgauth_define_pdp_context_authentication_parameters},
    {RIL_CMD_ID_CIPCA,     "+CIPCA",     RIL_PD,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cipca_initial_pdp_context_activation},
    {RIL_CMD_ID_CPSMS,     "+CPSMS",     RIL_MM,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_cpsms_power_saving_mode_setting},
    {RIL_CMD_ID_CCIOTOPT,  "+CCIOTOPT",  RIL_MM,   RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_ciot_optimisation_configuration, RIL_INFO_RSP_READ_MODE, false},
    {RIL_CMD_ID_CEDRXS,    "+CEDRXS",    RIL_MM,   RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_eDRX_setting, RIL_INFO_RSP_READ_MODE, true},
    {RIL_CMD_ID_CEDRXRDP,  "+CEDRXRDP",  RIL_MM,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_read_eDRX_dynamic_parameters},
    {RIL_CMD_ID_CGAPNRC,   "+CGAPNRC",   RIL_PD,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_APN_rate_control},
    {RIL_CMD_ID_CSCON,     "+CSCON",     RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_signaling_connection_status, RIL_INFO_RSP_READ_MODE, false},
    {RIL_CMD_ID_CCHO,      "+CCHO",      RIL_SIM,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_open_uicc_logical_channel},
    {RIL_CMD_ID_CCHC,      "+CCHC",      RIL_SIM,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_close_uicc_logical_channel},
    {RIL_CMD_ID_CGLA,      "+CGLA",      RIL_SIM,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_generic_uicc_logical_channel_access},
    {RIL_CMD_ID_CRCES,     "+CRCES",     RIL_MM,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_read_coverage_enhancement_status, RIL_INFO_RSP_READ_MODE, false},

#if defined(__RIL_SMS_COMMAND_SUPPORT__)
    /* command spec 27.005 */
    {RIL_CMD_ID_CSMS,      "+CSMS",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_select_message_service},
    {RIL_CMD_ID_CPMS,      "+CPMS",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_preferred_message_storage},
    {RIL_CMD_ID_CMGF,      "+CMGF",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_message_format},
    {RIL_CMD_ID_CSCA,      "+CSCA",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_service_centre_address},
    {RIL_CMD_ID_CSMP,      "+CSMP",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_set_text_mode_parameters},
    {RIL_CMD_ID_CSDH,      "+CSDH",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_show_text_mode_parameters},
    {RIL_CMD_ID_CSAS,      "+CSAS",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_save_settings},
    {RIL_CMD_ID_CRES,      "+CRES",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_restore_settings},
    {RIL_CMD_ID_CNMI,      "+CNMI",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_new_message_indication},
    {RIL_CMD_ID_CMGL_PDU,  "+CMGL",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, 5000,                      NULL,                      ril_response_list_messages_pdu},
    {RIL_CMD_ID_CMGR_PDU,  "+CMGR",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, 5000,                      NULL,                      ril_response_read_message_pdu},
    {RIL_CMD_ID_CMGS_PDU,  "+CMGS",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, 60000,                     ril_cmd_send_send_message_pdu, ril_response_send_message_pdu},
    {RIL_CMD_ID_CMGW_PDU,  "+CMGW",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, ril_cmd_send_write_message_pdu, ril_response_write_message_pdu},
    {RIL_CMD_ID_CMGD,      "+CMGD",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_delete_message},
    {RIL_CMD_ID_CMGC_PDU,  "+CMGC",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, ril_cmd_send_send_command_pdu, ril_response_send_command_pdu},
    {RIL_CMD_ID_CNMA_PDU,  "+CNMA",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, ril_cmd_send_new_message_ack_pdu, ril_response_new_message_ack_pdu},
    {RIL_CMD_ID_CMSS_PDU,  "+CMSS",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_send_message_from_storage_pdu},
    {RIL_CMD_ID_CMGL_TXT,  "+CMGL",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, 5000,                      NULL,                      ril_response_list_messages_txt},
    {RIL_CMD_ID_CMGR_TXT,  "+CMGR",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, 5000,                      NULL,                      ril_response_read_message_txt},
    {RIL_CMD_ID_CMGS_TXT,  "+CMGS",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, 60000,                     ril_cmd_send_send_message_txt, ril_response_send_message_txt},
    {RIL_CMD_ID_CMGW_TXT,  "+CMGW",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, ril_cmd_send_write_message_txt, ril_response_write_message_txt},
    {RIL_CMD_ID_CMGC_TXT,  "+CMGC",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, ril_cmd_send_send_command_txt, ril_response_send_command_txt},
    {RIL_CMD_ID_CNMA_TXT,  "+CNMA",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_new_message_ack_txt},
    {RIL_CMD_ID_CMSS_TXT,  "+CMSS",      RIL_SMS,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_send_message_from_storage_txt},
#endif /* __RIL_SMS_COMMAND_SUPPORT__ */

    /* command spec v250 */
    {RIL_CMD_ID_E,         "E",          RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, ril_cmd_send_basic_hdlr,   ril_response_command_echo},
    {RIL_CMD_ID_I,         "I",          RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, ril_cmd_send_basic_hdlr,   ril_response_request_identification_info},
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {RIL_CMD_ID_O,         "O",          RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, ril_cmd_send_basic_hdlr,   ril_response_return_to_online_data},
    {RIL_CMD_ID_Q,         "Q",          RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, ril_cmd_send_basic_hdlr,   ril_response_set_result_suppression_mode},
    {RIL_CMD_ID_S3,        "S3",         RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_command_line_termination_char},
    {RIL_CMD_ID_S4,        "S4",         RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_response_formatting_char},
    {RIL_CMD_ID_S5,        "S5",         RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_command_line_editing_char},
    {RIL_CMD_ID_S7,        "S7",         RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_connection_completion_timeout},
    {RIL_CMD_ID_S10,       "S10",        RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_automatic_disconnect_delay},
    {RIL_CMD_ID_V,         "V",          RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      NULL}, // Not support
    {RIL_CMD_ID_X,         "X",          RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, ril_cmd_send_basic_hdlr,   ril_response_result_code_selection},
    {RIL_CMD_ID_Z,         "Z",          RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, ril_cmd_send_basic_hdlr,   ril_response_reset_to_default_config},
    {RIL_CMD_ID_AF,        "&F",         RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_set_to_factory_defined_config},
    {RIL_CMD_ID_GCAP,      "+GCAP",      RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_complete_capabilities_list},
    {RIL_CMD_ID_S25,       "S25",        RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL,                      ril_response_delay_to_dtr},
#endif

    /* command spec proprietary */
    {RIL_CMD_ID_MLTS,              "*MLTS",           RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_get_local_timestamp_and_network_info, (RIL_INFO_RSP_ACTIVE_MODE | RIL_INFO_RSP_READ_MODE), false},
    {RIL_CMD_ID_MSIMINS,           "*MSIMINS",        RIL_SIM,  RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_sim_inserted_status_reporting, RIL_INFO_RSP_READ_MODE, false},
    {RIL_CMD_ID_MSPN,              "*MSPN",           RIL_MM,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_get_service_provider_name_from_sim},
    {RIL_CMD_ID_MUNSOL,            "*MUNSOL",         RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_extra_unsolicited_indications, RIL_INFO_RSP_NONE, false},
    {RIL_CMD_ID_MGCOUNT,           "*MGCOUNT",        RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_packet_domain_counters, RIL_INFO_RSP_NONE, false},
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {RIL_CMD_ID_MGSINK,            "*MGSINK",         RIL_PD,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_send_packet_to_discard},
    {RIL_CMD_ID_MCGDEFCONT     ,   "*MCGDEFCONT"    , RIL_PD,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_pdn_connection_set_default_psd_attach},
    {RIL_CMD_ID_MGTCSINK       ,   "*MGTCSINK"      , RIL_PD,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_send_tcpip_packet_to_discard           },
    {RIL_CMD_ID_MSACL          ,   "*MSACL"         , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_control_ACL_feature                             },
    {RIL_CMD_ID_MLACL          ,   "*MLACL"         , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_display_ACL_list                                },
    {RIL_CMD_ID_MWACL          ,   "*MWACL"         , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_write_ACL_entry                                 },
    {RIL_CMD_ID_MDACL          ,   "*MDACL"         , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_delete_ACL_entry                                },
    {RIL_CMD_ID_MSMEXTRAINFO   ,   "*MSMEXTRAINFO"  , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_control_extra_info_on_sms                       },
    {RIL_CMD_ID_MSMEXTRAUNSOL  ,   "*MSMEXTRAUNSOL" , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_control_extra_unsolicited_messages, RIL_INFO_RSP_READ_MODE, false},
    {RIL_CMD_ID_MSMSTATUS      ,   "*MSMSTATUS"     , RIL_SMS,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_obtain_status_of_sms_functionality              },
    {RIL_CMD_ID_MMGI           ,   "*MMGI"          , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_control_sms_unsolicited_indication, (RIL_INFO_RSP_READ_MODE | RIL_INFO_RSP_EXECUTE_MODE), true},
    {RIL_CMD_ID_MMGRW          ,   "*MMGRW"         , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_sms_location_rewrite                            },
    {RIL_CMD_ID_MMGSC_PDU      ,   "*MMGSC"         , RIL_SMS,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_sms_location_status_change                      },
    {RIL_CMD_ID_MMGSC_TXT      ,   "*MMGSC"         , RIL_SMS,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_sms_location_status_change                      },
#endif
    {RIL_CMD_ID_MUPIN          ,   "*MUPIN"         , RIL_SIM,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_uicc_pin_information_access                     },
    {RIL_CMD_ID_MUAPP          ,   "*MUAPP"         , RIL_SIM,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_uicc_application_list_access                    },
    {RIL_CMD_ID_MSST           ,   "*MSST"          , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_read_sst_ust_from_usim                          },
    {RIL_CMD_ID_MABORT         ,   "*MABORT"        , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_abort_mm_related_at_command                     },
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {RIL_CMD_ID_MCAL           ,   "*MCAL"          , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_radio_call                                      },
    {RIL_CMD_ID_MNON           ,   "*MNON"          , RIL_SIM,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_network_operator_name                           },
    {RIL_CMD_ID_MOPL           ,   "*MOPL"          , RIL_SIM,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_network_operator_plmn_list                      },
    {RIL_CMD_ID_MMUX           ,   "*MMUX"          , RIL_MUX,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_mux_configuration                               },
    {RIL_CMD_ID_MROUTEMMI      ,   "*MROUTEMMI"     , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_mmi_at_channel_routing_configuration            },
    {RIL_CMD_ID_MCEERMODE      ,   "*MCEERMODE"     , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_ceer_response_mode                              },
    {RIL_CMD_ID_MFTRCFG        ,   "*MFTRCFG"       , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_modem_feature_configuration                     },
#endif
    {RIL_CMD_ID_HVER           ,   "^HVER"          , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_request_hw_version                              },
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {RIL_CMD_ID_MODE           ,   "^MODE"          , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_indicate_system_mode, (RIL_INFO_RSP_ACTIVE_MODE | RIL_INFO_RSP_READ_MODE), false},
    {RIL_CMD_ID_SYSINFO        ,   "^SYSINFO"       , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_request_system_information                      },
    {RIL_CMD_ID_SYSCONFIG      ,   "^SYSCONFIG"     , RIL_MM,   RIL_CMD_TYPE_AT_DATA, 60000,                     NULL, ril_response_configure_system_reference                      },
    {RIL_CMD_ID_CARDMODE       ,   "^CARDMODE"      , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_request_sim_usim_mode                           },
    {RIL_CMD_ID_SPN            ,   "^SPN"           , RIL_SIM,  RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_read_service_provider_name                      },
    {RIL_CMD_ID_MSTLOCK        ,   "*MSTLOCK"       , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_STK_registering, RIL_INFO_RSP_NONE, false       },
    {RIL_CMD_ID_MSTPD          ,   "*MSTPD"         , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_STK_terminal_profile_download, RIL_INFO_RSP_NONE, false},
    {RIL_CMD_ID_MSTMODE        ,   "*MSTMODE"       , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_STK_output_format_setting, RIL_INFO_RSP_READ_MODE, false},
    {RIL_CMD_ID_MSTICREC       ,   "*MSTICREC"      , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_STK_obtain_icon_record, RIL_INFO_RSP_EXECUTE_MODE, true},
    {RIL_CMD_ID_MSTICIMG       ,   "*MSTICIMG"      , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_STK_obtain_icon_image, RIL_INFO_RSP_EXECUTE_MODE, false},
    {RIL_CMD_ID_MSTGC          ,   "*MSTGC"         , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_STK_parameters_associated_with_proactive_command, RIL_INFO_RSP_EXECUTE_MODE, false},
    {RIL_CMD_ID_MSTCR          ,   "*MSTCR"         , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_STK_inform_response_to_proactive_command, RIL_INFO_RSP_NONE, false},
    {RIL_CMD_ID_MSTMS          ,   "*MSTMS"         , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_STK_menu_selection, RIL_INFO_RSP_NONE, false},
    {RIL_CMD_ID_MSTEV          ,   "*MSTEV"         , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_STK_monitored_event_occurence, RIL_INFO_RSP_NONE, false},
#endif
    {RIL_CMD_ID_MICCID         ,   "*MICCID"        , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_read_usim_iccid                                 },
    {RIL_CMD_ID_MHOMENW        ,   "*MHOMENW"       , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_display_home_network_information                },
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {RIL_CMD_ID_MCSIMLOCK      ,   "*MCSIMLOCK"     , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_lock_csim_access, RIL_INFO_RSP_READ_MODE, false },
    {RIL_CMD_ID_MEMMREEST      ,   "*MEMMREEST"     , RIL_MM,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_emm_re_establishment_test                       },
    {RIL_CMD_ID_MAPNURI        ,   "*MAPNURI"       , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_apn_rate_control_indication, RIL_INFO_RSP_READ_MODE, true},
    {RIL_CMD_ID_MPLMNURI       ,   "*MPLMNURI"      , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_plmn_rate_control_indication, RIL_INFO_RSP_READ_MODE, false},
    {RIL_CMD_ID_MPDI           ,   "*MPDI"          , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_packet_discard_indication, RIL_INFO_RSP_READ_MODE, false},
    {RIL_CMD_ID_MNBIOTDT       ,   "*MNBIOTDT"      , RIL_PD,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_nbiot_data_type                                 },
    {RIL_CMD_ID_MNBIOTRAI      ,   "*MNBIOTRAI"     , RIL_PD,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_nbiot_release_assistance_indication             },
    {RIL_CMD_ID_MNVMQ          ,   "*MNVMQ"         , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_nvdm_status_query                               },
    {RIL_CMD_ID_MNVMAUTH       ,   "*MNVMAUTH"      , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_nvdm_access_security_authorization              },
    {RIL_CMD_ID_MNVMW          ,   "*MNVMW"         , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_nvdm_data_write                                 },
    {RIL_CMD_ID_MNVMR          ,   "*MNVMR"         , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_nvdm_data_read                                  },
    {RIL_CMD_ID_MNVMGET        ,   "*MNVMGET"       , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_nvdm_get_all_data_item_id                       },
    {RIL_CMD_ID_MNVMIVD        ,   "*MNVMIVD"       , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_nvdm_data_item_invalidate                       },
    {RIL_CMD_ID_MNVMRSTONE     ,   "*MNVMRSTONE"    , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_nvdm_data_item_factory_reset                    },
    {RIL_CMD_ID_MNVMRST        ,   "*MNVMRST"       , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_nvdm_factory_reset_all_data_item                },
    {RIL_CMD_ID_MNVMMDNQ       ,   "*MNVMMDNQ"      , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_nvdm_query_num_of_mini_dump                     },
    {RIL_CMD_ID_MNVMMDR        ,   "*MNVMMDR"       , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_nvdm_read_mini_dump                             },
    {RIL_CMD_ID_MNVMMDC        ,   "*MNVMMDC"       , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_nvdm_clean_mini_dump                            },
    {RIL_CMD_ID_IDCFREQ        ,   "+IDCFREQ"       , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_set_idc_frequency_range, RIL_INFO_RSP_NONE, false},
    {RIL_CMD_ID_IDCPWRBACKOFF  ,   "+IDCPWRBACKOFF" , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_set_tx_power_back_off                           },
    {RIL_CMD_ID_IDCTX2GPS      ,   "+IDCTX2GPS"     , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_generate_periodic_tx_for_gps                    },
#endif
    {RIL_CMD_ID_MCALDEV        ,   "*MCALDEV"       , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_enter_exit_rf_calibration_state                 },
    {RIL_CMD_ID_MATWAKEUP      ,   "*MATWAKEUP"     , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_set_modem_wakeup_indication, RIL_INFO_RSP_READ_MODE, false},
    {RIL_CMD_ID_MBAND          ,   "*MBAND"         , RIL_MM,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_query_modem_operating_band                      },
    {RIL_CMD_ID_MENGINFO       ,   "*MENGINFO"      , RIL_MM,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_query_network_state                             },
    {RIL_CMD_ID_MFRCLLCK       ,   "*MFRCLLCK"      , RIL_MM,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_frequency_and_cell_lock                         },
    {RIL_CMD_ID_MSPCHSC        ,   "*MSPCHSC"       , RIL_MM,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_set_scrambling_algorithm_for_npdsch             },
    {RIL_CMD_ID_MDPDNP         ,   "*MDPDNP"        , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_default_pdn_parameter, RIL_INFO_RSP_READ_MODE, false},
    {RIL_CMD_ID_MEDRXCFG       ,   "*MEDRXCFG"      , RIL_MM,   RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_eDRX_configuration, RIL_INFO_RSP_READ_MODE, true},
    {RIL_CMD_ID_MCELLINFO      ,   "*MCELLINFO"     , RIL_MM,   RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_serving_and_neighbor_cell_info},
    {RIL_CMD_ID_MUPDIR         ,   "*MUPDIR"        , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, NULL, ril_response_indicate_packet_to_track, RIL_INFO_RSP_NONE, false},

    {RIL_CMD_ID_CUSTOM_CMD     ,   "CUSTOM_CMD"     , RIL_NONE, RIL_CMD_TYPE_AT_DATA, RIL_DEFAULT_TIMEOUT_IN_MS, ril_cmd_send_custom_command, ril_response_custom_command           },
    {RIL_CMD_ID_CUSTOM_CMD_URC ,   "CUSTOM_CMD_URC" , RIL_NONE, RIL_CMD_TYPE_URC,     RIL_DEFAULT_TIMEOUT_IN_MS, ril_cmd_send_custom_command, ril_response_custom_command           }
};


static const ril_urc_cmd_item_t s_urc_cmds_tbl[] = {
    {RIL_URC_ID_TEST,      "+TEST",      RIL_ALL,   ril_urc_dummy_hdlr},

    /* v27.007 command spec */
    {RIL_URC_ID_CREG,      "+CREG",      RIL_ALL,   ril_urc_creg_network_registration              },
    {RIL_URC_ID_CTZR,      "+CTZR",      RIL_ALL,   ril_urc_ctzr_time_zone_reporting               },
    {RIL_URC_ID_CGEV,      "+CGEV",      RIL_ALL,   ril_urc_cgev_packet_domain_event_reporting   },
    {RIL_URC_ID_CGREG,     "+CGREG",     RIL_ALL,   ril_urc_cgreg_gprs_network_registration_status },
    {RIL_URC_ID_CEREG,     "+CEREG",     RIL_ALL,   ril_urc_cereg_eps_network_registration_status  },
    {RIL_URC_ID_CCIOTOPT,  "+CCIOTOPT",  RIL_MM,    ril_urc_ciot_optimisation_configuration  },
    {RIL_URC_ID_CEDRXP,    "+CEDRXP",    RIL_ALL,   ril_urc_eDRX_setting  },        
    {RIL_URC_ID_CSCON,     "+CSCON",     RIL_ALL,   ril_urc_signaling_connection_status  },
    {RIL_URC_ID_CPIN,      "+CPIN",      RIL_ALL,   ril_urc_enter_pin},

#if defined(__RIL_SMS_COMMAND_SUPPORT__)
    /* SMS command spec */
    {RIL_URC_ID_CMTI,      "+CMTI",      RIL_ALL,   ril_urc_new_message_indication_txt},
    {RIL_URC_ID_CMT,       "+CMT",       RIL_ALL,   ril_urc_new_message_indication_pdu},
#endif

    /* proprietary command spec */
    {RIL_URC_ID_MLTS,      "*MLTS",      RIL_ALL,   ril_urc_get_local_timestamp_and_network_info},
    {RIL_URC_ID_MSIMINS,   "*MSIMINS",   RIL_ALL,   ril_urc_SIM_inserted_status_reporting},
    {RIL_URC_ID_MSQN,      "*MSQN",      RIL_ALL,   ril_urc_signal_quality_report},
    {RIL_URC_ID_MGCOUNT,   "*MGCOUNT",   RIL_ALL,   ril_urc_packet_domain_counters},
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {RIL_URC_ID_MSMEXTRAUNSOL, "+SMREADY", RIL_ALL, ril_urc_control_extra_unsolicited_messages},
    {RIL_URC_ID_MMGI,      "*MMGI",      RIL_ALL,   ril_urc_control_sms_unsolicited_indication},
    {RIL_URC_ID_MSMPUKBLKD, "*MSMPUKBLKD", RIL_ALL, ril_urc_sim_puk_blocked_unsolicited_indication},

    {RIL_URC_ID_MODE,      "^MODE",      RIL_ALL,   ril_urc_indicate_system_mode},
    {RIL_URC_ID_MSTC,      "*MSTC",      RIL_ALL,   ril_urc_STK_proactive_command_indication},
    {RIL_URC_ID_MSTUD,     "*MSTUD",     RIL_ALL,   ril_urc_STK_unsolicited_data},
    {RIL_URC_ID_MCCST,     "*MCCST",     RIL_ALL,   ril_urc_STK_unsolicited_response_for_call_control},
    {RIL_URC_ID_MCSIMLOCK, "*MCSIMLOCK", RIL_ALL,   ril_urc_lock_csim_access},
    {RIL_URC_ID_MAPNURI,   "*MAPNURI",   RIL_ALL,   ril_urc_apn_rate_control_indication},
    {RIL_URC_ID_MPLMNURI,  "*MPLMNURI",  RIL_ALL,   ril_urc_plmn_rate_control_indication},
    {RIL_URC_ID_MPDI,      "*MPDI",      RIL_ALL,   ril_urc_packet_discard_indication},
    {RIL_URC_ID_IDCFREQ,   "+IDCSTATUS", RIL_ALL,   ril_urc_set_idc_frequency_range},
#endif

    /* internal used */
    {RIL_URC_ID_MATREADY,  "*MATREADY",  RIL_ALL,   ril_urc_modem_ready_indication},
    {RIL_URC_ID_MATWAKEUP, "*MATWAKEUP", RIL_ALL,   ril_urc_set_modem_wakeup_indication},
    {RIL_URC_ID_MDPDNP,    "*MDPDNP",    RIL_ALL,   ril_urc_default_pdn_parameter},
    {RIL_URC_ID_MUPDIR,    "*MUPDIR",    RIL_ALL,   ril_urc_indicate_packet_to_track},
    {RIL_URC_ID_MUPDI,     "*MUPDI",     RIL_ALL,   ril_urc_indicate_packet_delivery_status}
};


//#define RESULT_CODE_INFO_NUMS    (sizeof(s_result_codes) / sizeof(ril_result_code_info_t))
#define AT_CMD_NUMS    (sizeof(s_at_cmds_tbl) / sizeof(ril_cmd_item_t))
#define URC_CMD_NUMS    (sizeof(s_urc_cmds_tbl) / sizeof(ril_urc_cmd_item_t))


ril_cmd_item_t *get_at_cmd_table()
{
    return (ril_cmd_item_t *)&s_at_cmds_tbl[0];
}

ril_urc_cmd_item_t *get_urc_cmd_table()
{
    return (ril_urc_cmd_item_t *)&s_urc_cmds_tbl[0];
}


ril_cmd_item_t *get_at_cmd_item(ril_cmd_id_t cmd_id)
{
    if (cmd_id <= 0 || cmd_id >= RIL_CMD_ID_INVALID) {
        return NULL;
    } else {
        return (ril_cmd_item_t *)&s_at_cmds_tbl[cmd_id - 1];
    }
}


ril_urc_cmd_item_t *get_urc_cmd_item(ril_urc_id_t urc_id)
{
    if (urc_id <= 0 || urc_id >= RIL_URC_ID_INVALID) {
        return NULL;
    } else {
        return (ril_urc_cmd_item_t *)&s_urc_cmds_tbl[urc_id - 1];
    }
}


ril_cmd_id_t find_at_cmd_id(char *cmd_head, uint32_t cmd_head_len)
{
    int32_t i;
    ril_cmd_item_t *at_cmds_tbl;
    at_cmds_tbl = get_at_cmd_table();

    for (i = 0; i < AT_CMD_NUMS; i++) {
        if (!strncmp(at_cmds_tbl[i].cmd_head, cmd_head, cmd_head_len)) {
            break;
        }
    }

    if (AT_CMD_NUMS <= i) {
        return RIL_CMD_ID_INVALID;
    }
    return (ril_cmd_id_t)(i + 1);
}


ril_urc_id_t find_urc_cmd_id(char *cmd_head, uint32_t cmd_head_len)
{
    int32_t i;
    ril_urc_cmd_item_t *urc_cmds_tbl;
    urc_cmds_tbl = get_urc_cmd_table();

    for (i = 0; i < URC_CMD_NUMS; i++) {
        int32_t tbl_cmd_len = strlen(urc_cmds_tbl[i].cmd_head);
        if (!strncmp(urc_cmds_tbl[i].cmd_head, cmd_head, cmd_head_len)
            && tbl_cmd_len == cmd_head_len) {
            break;
        }
    }

    if (URC_CMD_NUMS <= i) {
        return RIL_URC_ID_INVALID;
    }

    return (ril_urc_id_t)(i + 1);
}


#if 0  // NOT support verbose error code so that no need to implement this function.
ril_result_code_t get_result_code_by_numeric(int32_t numeric)
{
    int32_t i;
    bool found = false;
    for (i = 0; i < RESULT_CODE_INFO_NUMS; i++) {
        if (s_result_codes[i].numeric == numeric) {
            found = true;
            break;
        }
    }

    if (found) {
        return s_result_codes[i].enumerate;
    } else {
        return RIL_RESULT_CODE_UNDEFINED;
    }
}
#endif


bool ril_cmds_order_is_valid()
{
    int32_t i;
    bool ret = true;
    for (i = 0; i < AT_CMD_NUMS; i++) {
        if (s_at_cmds_tbl[i].cmd_id != (i + 1)) {
            ret = false;
            RIL_LOGE("commands order is wrong, i: %d\r\n", (int)i);
            break;
        }
    }

    return ret;
}

