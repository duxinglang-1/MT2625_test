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

#include "ril.h"
#include "ril_internal_use.h"
#include "ril_log.h"
#include "ril_cmds_def.h"
#include "ril_task.h"
#include "ril_cmds_27007.h"
#include "ril_cmds_27005.h"
#include "ril_cmds_v250.h"
#include "ril_cmds_proprietary.h"
#include "ril_cmds_common.h"
#include "ril_channel_config.h"
#include "ril_utils.h"

#if defined(__RIL_UT_TEST_CASES__)

typedef int32_t (*cmd_ut_hdlr_t)(char *param, uint32_t param_num);

typedef struct {
    char *cmd;
    void *cmd_ut_hdlr;
    uint32_t func_params_num;
    bool need_mode;
    ril_cmd_response_callback_t rsp_cb;
} ril_ut_cmd_item_t;

extern bool ril_channel_response_hdlr(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);
extern bool ril_channel_urc_hdlr(char *cmd_buf, uint32_t cmd_buf_len);
extern int32_t ril_urc_ut_callback(ril_urc_id_t event_id,void * param,uint32_t param_len);

int32_t ril_ut_send_CGACT(int32_t mode, void *func_ptr, char *param_array[], uint32_t param_num, ril_cmd_response_callback_t rsp_cb)
{
    int32_t i;
    ril_pdp_context_activate_or_deactivate_req_t req;
    int32_t *cid_array = NULL;
    ril_status_t ret;

    req.state = RIL_OMITTED_INTEGER_PARAM;
    req.cid_array = NULL;
    req.cid_array_len = 0;

    if (param_num > 0) {
        if (!strcmp(param_array[0], "oint")) {
            req.state = RIL_OMITTED_INTEGER_PARAM;
        } else {
            req.state = atoi(param_array[0]);
        }
    }

    if (param_num > 1) {
        cid_array = ril_mem_malloc(sizeof(int32_t) * (param_num - 1));
        if (cid_array == NULL) {
            return -1;
        }

        for (i = 0; i < param_num - 1; i++) {
            cid_array[i] = atoi(param_array[i + 1]);
        }

        req.cid_array = cid_array;
        req.cid_array_len = param_num - 1;
    }
    
    ret = ril_request_pdp_context_activate_or_deactivate((ril_request_mode_t)mode,
                                                        &req,
                                                        rsp_cb,
                                                        NULL);
    RIL_LOGI("ril api call result: %d\r\n", (int)ret);

    if (cid_array != NULL) {
        ril_mem_free(cid_array);
    }
    return 0;
}


int32_t ril_ut_send_CGDATA(int32_t mode, void *func_ptr, char *param_array[], uint32_t param_num, ril_cmd_response_callback_t rsp_cb)
{
    int32_t i;
    ril_enter_data_state_req_t req;
    int32_t *cid_array = NULL;
    char *p;
    ril_status_t ret;

    req.l2p = RIL_OMITTED_STRING_PARAM;
    req.cid_array = NULL;
    req.cid_array_len = 0;

    if (param_num > 0) {
        if (!strcmp(param_array[0], "ostr")) {
            req.l2p = (void *)RIL_OMITTED_STRING_PARAM;
        } else if (*(param_array[0]) == '\"') {
            p = param_array[0];
            p++;
            req.l2p = strsep(&p, "\"");
        } else {
            req.l2p = param_array[0];
        }
    }

    if (param_num > 1) {
        cid_array = ril_mem_malloc(sizeof(int32_t) * (param_num - 1));
        if (cid_array == NULL) {
            return -1;
        }

        for (i = 0; i < param_num - 1; i++) {
            cid_array[i] = atoi(param_array[i + 1]);
        }

        req.cid_array = cid_array;
        req.cid_array_len = param_num - 1;
    }
    
    ret = ril_request_enter_data_state((ril_request_mode_t)mode,
                                                        &req,
                                                        rsp_cb,
                                                        NULL,
                                                        -1);
    RIL_LOGI("ril api call result: %d\r\n", (int)ret);

    if (cid_array != NULL) {
        ril_mem_free(cid_array);
    }
    return 0;
}


int32_t ril_ut_send_CGPADDR(int32_t mode, void *func_ptr, char *param_array[], uint32_t param_num, ril_cmd_response_callback_t rsp_cb)
{
    int32_t i;
    ril_show_pdp_address_req_t req;
    int32_t *cid_array = NULL;
    ril_status_t ret;

    req.cid_array = NULL;
    req.cid_array_len = 0;

    if (param_num > 0) {
        cid_array = ril_mem_malloc(sizeof(int32_t) * param_num);
        if (cid_array == NULL) {
            return -1;
        }

        for (i = 0; i < param_num; i++) {
            cid_array[i] = atoi(param_array[i]);
        }

        req.cid_array = cid_array;
        req.cid_array_len = param_num;
    }
    
    ret = ril_request_show_pdp_address((ril_request_mode_t)mode,
                                                        &req,
                                                        rsp_cb,
                                                        NULL);
    RIL_LOGI("ril api call result: %d\r\n", (int)ret);

    if (cid_array != NULL) {
        ril_mem_free(cid_array);
    }
    return 0;
}


/**
* please fill this table first before test your command.
* format: | command head | ril api | the num of command parameters | if need pass ril_request_mode_t | response cb |
* notice: 1. when calculate the num of params, please ignore ril_request_mode_t, rsp_callback and user data.
*            2. if the num of params is more than 5, please group your parameters as structure.
*            3. if no need to pass "mode", please set the 4th column as false, e.g. some v.250 command didn't export param "mode".
*            4. if response cb is NULL, will invoke ut_response_common_cb, and only print final result code in syslog.
*            5. if the number of parameters is dynamical, please create special sending function for this case. e.g. +CGACT
*/
static ril_ut_cmd_item_t s_ut_at_cmd_tbl[] = {


    /* command spec 27.007 */
    {"+CGMI",     (void *)ril_request_manufacturer_identification                                   , 0, true,  ril_response_ut_callback_27007},
    {"+CGMM",     (void *)ril_request_model_identification                                          , 0, true,  ril_response_ut_callback_27007},
    {"+CGMR",     (void *)ril_request_revision_identification                                       , 0, true,  ril_response_ut_callback_27007},
    {"+CGSN",     (void *)ril_request_serial_number                                                 , 1, true,  ril_response_ut_callback_27007},
    {"+CSCS",     (void *)ril_request_select_charcter_set                                           , 1, true,  ril_response_ut_callback_27007},
    {"+CIMI",     (void *)ril_request_imsi                                                          , 0, true,  ril_response_ut_callback_27007},
    {"+CMUX",     (void *)ril_request_multiplexing_mode                                             , 9, true,  ril_response_ut_callback_27007},
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {"+CR",       (void *)ril_request_service_reporting_control                                     , 1, true,  ril_response_ut_callback_27007},
#endif
    {"+CEER",     (void *)ril_request_extended_error_report                                         , 0, true,  ril_response_ut_callback_27007},
    {"+CNUM",     (void *)ril_request_subscriber_number                                             , 0, true,  ril_response_ut_callback_27007},
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {"+CREG",     (void *)ril_request_network_registration                                          , 1, true,  ril_response_ut_callback_27007},
#endif
    {"+COPS",     (void *)ril_request_plmn_selection                                                , 4, true,  ril_response_ut_callback_27007},
    {"+CLCK",     (void *)ril_request_facility_lock                                                 , 4, true,  ril_response_ut_callback_27007},
    {"+CPWD",     (void *)ril_request_change_password                                               , 3, true,  ril_response_ut_callback_27007},
    {"+CPOL",     (void *)ril_request_preferred_plmn_list                                           , 7, true,  ril_response_ut_callback_27007},
    {"+CPLS",     (void *)ril_request_selection_of_preferred_plmn_list                              , 1, true,  ril_response_ut_callback_27007},
    {"+COPN",     (void *)ril_request_read_operator_names                                           , 0, true,  ril_response_ut_callback_27007},
    {"+CFUN",     (void *)ril_request_set_phone_functionality                                       , 2, true,  ril_response_ut_callback_27007},
    {"+CPIN",     (void *)ril_request_enter_pin                                                     , 2, true,  ril_response_ut_callback_27007},
    {"+CSQ",      (void *)ril_request_signal_quality                                                , 0, true,  ril_response_ut_callback_27007},
    {"+CCLK",     (void *)ril_request_clock                                                         , 1, true,  ril_response_ut_callback_27007},
    {"+CSIM",     (void *)ril_request_generic_sim_access                                            , 2, true,  ril_response_ut_callback_27007},
    {"+CRSM",     (void *)ril_request_restricted_sim_access                                         , 7, true,  ril_response_ut_callback_27007},
    {"+CMAR",     (void *)ril_request_master_reset                                                  , 1, true,  ril_response_ut_callback_27007},
    {"+CTZU",     (void *)ril_request_automatic_time_zone_update                                    , 1, true,  ril_response_ut_callback_27007},
    {"+CTZR",     (void *)ril_request_time_zone_reporting                                           , 1, true,  ril_response_ut_callback_27007},
    {"+CGPIAF",   (void *)ril_request_printing_ip_address_format                                    , 4, true,  ril_response_ut_callback_27007},
    {"+CPINR",    (void *)ril_request_remaining_pin_retries                                         , 1, true,  ril_response_ut_callback_27007},
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {"+CSUS",     (void *)ril_request_set_card_slot                                                 , 1, true,  ril_response_ut_callback_27007},
#endif
    {"+CESQ",     (void *)ril_request_extended_signal_quality                                       , 0, true,  ril_response_ut_callback_27007},
    {"+CMEE",     (void *)ril_request_report_mobile_termination_error                               , 1, true,  ril_response_ut_callback_27007},
    {"+CGDCONT",  (void *)ril_request_define_pdp_context                                            , 12, true,  ril_response_ut_callback_27007},
    {"+CGATT",    (void *)ril_request_ps_attach_or_detach                                           , 1, true,  ril_response_ut_callback_27007},
    {"+CGACT",    (void *)ril_request_pdp_context_activate_or_deactivate                            , 1, true,  ril_response_ut_callback_27007},
    {"+CGDATA",   (void *)ril_request_enter_data_state                                              , 1, true,  ril_response_ut_callback_27007},
    {"+CGPADDR",  (void *)ril_request_show_pdp_address                                              , 1, true,  ril_response_ut_callback_27007},
    {"+CGEREP",   (void *)ril_request_packet_domain_event_reporting                                 , 2, true,  ril_response_ut_callback_27007},
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {"+CGREG",    (void *)ril_request_gprs_network_registration_status                              , 1, true,  ril_response_ut_callback_27007},
#endif
    {"+CEREG",    (void *)ril_request_eps_network_registration_status                               , 1, true,  ril_response_ut_callback_27007},
    {"+CGCONTRDP", (void *)ril_request_pdp_context_read_dynamic_parameters                          , 1, true,  ril_response_ut_callback_27007},
    {"+CGDEL",    (void *)ril_request_delete_non_active_pdp_contexts                                , 1, true,  ril_response_ut_callback_27007},
    {"+CGAUTH",   (void *)ril_request_define_pdp_context_authentication_parameters                  , 4, true,  ril_response_ut_callback_27007},
    {"+CIPCA",    (void *)ril_request_initial_pdp_context_activation                                , 2, true,  ril_response_ut_callback_27007},
    {"+CPSMS",    (void *)ril_request_power_saving_mode_setting                                     , 5, true,  ril_response_ut_callback_27007},
    {"+CCIOTOPT", (void *)ril_request_ciot_optimisation_configuration                               , 3, true,  ril_response_ut_callback_27007},
    {"+CEDRXS",   (void *)ril_request_eDRX_setting                                                  , 3, true,  ril_response_ut_callback_27007},
    {"+CEDRXRDP", (void *)ril_request_read_eDRX_dynamic_parameters                                  , 0, true,  ril_response_ut_callback_27007},
    {"+CGAPNRC",  (void *)ril_request_APN_rate_control                                              , 1, true,  ril_response_ut_callback_27007},
    {"+CSCON",    (void *)ril_request_signaling_connection_status                                   , 1, true,  ril_response_ut_callback_27007},
    {"+CCHO",     (void *)ril_request_open_uicc_logical_channel                                     , 1, true,  ril_response_ut_callback_27007},
    {"+CCHC",     (void *)ril_request_close_uicc_logical_channel                                    , 1, true,  ril_response_ut_callback_27007},
    {"+CGLA",     (void *)ril_request_generic_uicc_logical_channel_access                           , 3, true,  ril_response_ut_callback_27007},
    {"+CRCES",    (void *)ril_request_read_coverage_enhancement_status                              , 0, true,  ril_response_ut_callback_27007},

#if defined(__RIL_SMS_COMMAND_SUPPORT__)
    /* command spec 27.005 */
    {"+CSMS"     , (void *)ril_request_select_message_service                                       , 1, true,  ril_response_ut_callback_27005},
    {"+CPMS"     , (void *)ril_request_preferred_message_storage                                    , 3, true,  ril_response_ut_callback_27005},
    {"+CMGF"     , (void *)ril_request_message_format                                               , 1, true,  ril_response_ut_callback_27005},
    {"+CSCA"     , (void *)ril_request_service_centre_address                                       , 2, true,  ril_response_ut_callback_27005},
    {"+CSMP"     , (void *)ril_request_set_text_mode_parameters                                     , 4, true,  ril_response_ut_callback_27005},
    {"+CSDH"     , (void *)ril_request_show_text_mode_parameters                                    , 1, true,  ril_response_ut_callback_27005},
    {"+CSAS"     , (void *)ril_request_save_settings                                                , 1, true,  ril_response_ut_callback_27005},
    {"+CRES"     , (void *)ril_request_restore_settings                                             , 1, true,  ril_response_ut_callback_27005},
    {"+CNMI"     , (void *)ril_request_new_message_indication                                       , 5, true,  ril_response_ut_callback_27005},
    {"+CMGLP"    , (void *)ril_request_list_messages_pdu                                            , 1, true,  ril_response_ut_callback_27005},
    {"+CMGRP"    , (void *)ril_request_read_message_pdu                                             , 1, true,  ril_response_ut_callback_27005},
    {"+CMGSP"    , (void *)ril_request_send_message_pdu                                             , 2, true,  ril_response_ut_callback_27005},
    {"+CMGWP"    , (void *)ril_request_write_message_pdu                                            , 3, true,  ril_response_ut_callback_27005},
    {"+CMGD"     , (void *)ril_request_delete_message                                               , 2, true,  ril_response_ut_callback_27005},
    {"+CMGCP"    , (void *)ril_request_send_command_pdu                                             , 2, true,  ril_response_ut_callback_27005},
    {"+CNMAP"    , (void *)ril_request_new_message_ack_pdu                                          , 3, true,  ril_response_ut_callback_27005},
    {"+CMSSP"    , (void *)ril_request_send_message_from_storage_pdu                                , 3, true,  ril_response_ut_callback_27005},
    {"+CMGLT"    , (void *)ril_request_list_messages_txt                                            , 1, true,  ril_response_ut_callback_27005},
    {"+CMGRT"    , (void *)ril_request_read_message_txt                                             , 1, true,  ril_response_ut_callback_27005},
    {"+CMGST"    , (void *)ril_request_send_message_txt                                             , 3, true,  ril_response_ut_callback_27005},
    {"+CMGWT"    , (void *)ril_request_write_message_txt                                            , 4, true,  ril_response_ut_callback_27005},
    {"+CMGCT"    , (void *)ril_request_send_command_txt                                             , 7, true,  ril_response_ut_callback_27005},
    {"+CNMAT"    , (void *)ril_request_new_message_ack_txt                                          , 0, true,  ril_response_ut_callback_27005},
    {"+CMSST"    , (void *)ril_request_send_message_from_storage_txt                                , 3, true,  ril_response_ut_callback_27005},
#endif /* __RIL_SMS_COMMAND_SUPPORT__ */

    /* command spec v250 */
    {"E"         , (void *)ril_request_command_echo                                                 , 1, false, ril_response_ut_callback_v250},
    {"I"         , (void *)ril_request_identification_info                                          , 0, false, ril_response_ut_callback_v250},
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {"O"         , (void *)ril_request_return_to_online_data                                        , 1, false, ril_response_ut_callback_v250},
    {"Q"         , (void *)ril_request_set_result_suppression_mode                                  , 1, false, ril_response_ut_callback_v250},
    {"S3"        , (void *)ril_request_command_line_termination_char                                , 0, false,  ril_response_ut_callback_v250},
    {"S4"        , (void *)ril_request_response_formatting_char                                     , 0, false,  ril_response_ut_callback_v250},
    {"S5"        , (void *)ril_request_command_line_editing_char                                    , 1, true,  ril_response_ut_callback_v250},
    {"S7"        , (void *)ril_request_connection_completion_timeout                                , 1, true,  ril_response_ut_callback_v250},
    {"S10"       , (void *)ril_request_automatic_disconnect_delay                                   , 1, true,  ril_response_ut_callback_v250},
    {"X"         , (void *)ril_request_result_code_selection                                        , 1, false, ril_response_ut_callback_v250},
    {"Z"         , (void *)ril_request_reset_to_default_config                                      , 1, false, ril_response_ut_callback_v250},
    {"&F"        , (void *)ril_request_set_to_factory_defined_config                                , 1, false, ril_response_ut_callback_v250},
    {"+GCAP"     , (void *)ril_request_complete_capabilities_list                                   , 0, true,  ril_response_ut_callback_v250},
    {"S25"       , (void *)ril_request_delay_to_dtr                                                 , 1, true,  ril_response_ut_callback_v250},
#endif

    /* proprietary commands */
    {"*MLTS"     , (void *)ril_request_get_local_timestamp_and_network_info                         , 1, true,  ril_response_ut_callback_proprietary},
    {"*MSIMINS"  , (void *)ril_request_sim_inserted_status_reporting                                , 1, true,  ril_response_ut_callback_proprietary},
    {"*MSPN"     , (void *)ril_request_get_service_provider_name_from_sim                           , 0, true,  ril_response_ut_callback_proprietary },
    {"*MUNSOL"   , (void *)ril_request_extra_unsolicited_indications                                , 2, true,  ril_response_ut_callback_proprietary},
    {"*MGCOUNT"  , (void *)ril_request_packet_domain_counters                                       , 2, true,  ril_response_ut_callback_proprietary},
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {"*MGSINK"   , (void *)ril_request_send_packet_to_discard                                       , 5, true,  ril_response_ut_callback_proprietary},
    {"*MCGDEFCONT", (void *)ril_request_pdn_connection_set_default_psd_attach                       , 5, true,  ril_response_ut_callback_proprietary},
    {"*MGTCSINK" , (void *)ril_request_send_tcpip_packet_to_discard                                 , 5, true,  ril_response_ut_callback_proprietary},
    {"*MSACL"    , (void *)ril_request_control_ACL_feature                                          , 2, true,  ril_response_ut_callback_proprietary},
    {"*MLACL"    , (void *)ril_request_display_ACL_list                                             , 2, true,  ril_response_ut_callback_proprietary},
    {"*MWACL"    , (void *)ril_request_write_ACL_entry                                              , 3, true,  ril_response_ut_callback_proprietary},
    {"*MDACL"    , (void *)ril_request_delete_ACL_entry                                             , 2, true,  ril_response_ut_callback_proprietary},
    {"*MSMEXTRAINFO" , (void *)ril_request_control_extra_info_on_sms                                , 1, true,  ril_response_ut_callback_proprietary},
    {"*MSMEXTRAUNSOL" , (void *)ril_request_control_extra_unsolicited_messages                      , 1, true,  ril_response_ut_callback_proprietary},
    {"*MSMSTATUS", (void *)ril_request_obtain_status_of_sms_functionality                           , 0, true,  ril_response_ut_callback_proprietary},
    {"*MMGI"     , (void *)ril_request_control_sms_unsolicited_indication                           , 2, true,  ril_response_ut_callback_proprietary},
    {"*MMGRW"    , (void *)ril_request_sms_location_rewrite                                         , 1, true,  ril_response_ut_callback_proprietary},
    {"*MMGSCP"   , (void *)ril_request_sms_location_status_change_pdu                               , 2, true,  ril_response_ut_callback_proprietary},
    {"*MMGSCT"   , (void *)ril_request_sms_location_status_change_txt                               , 2, true,  ril_response_ut_callback_proprietary},
#endif
    {"*MUPIN"    , (void *)ril_request_uicc_pin_information_access                                  , 5, true,  ril_response_ut_callback_proprietary},
    {"*MUAPP"    , (void *)ril_request_uicc_application_list_access                                 , 2, true,  ril_response_ut_callback_proprietary},
    {"*MSST"     , (void *)ril_request_read_sst_ust_from_usim                                       , 0, true,  ril_response_ut_callback_proprietary},
    {"*MABORT"   , (void *)ril_request_abort_mm_related_at_command                                  , 0, true,  ril_response_ut_callback_proprietary},
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {"*MCAL"     , (void *)ril_request_radio_call                                                   , 6, true,  ril_response_ut_callback_proprietary},
    {"*MNON"     , (void *)ril_request_network_operator_name                                        , 1, true,  ril_response_ut_callback_proprietary},
    {"*MOPL"     , (void *)ril_request_network_operator_plmn_list                                   , 1, true,  ril_response_ut_callback_proprietary},
    {"*MMUX"     , (void *)ril_request_mux_configuration                                            , 4, true,  ril_response_ut_callback_proprietary},
    {"*MROUTEMMI" , (void *)ril_request_mmi_at_channel_routing_configuration                        , 3, true,  ril_response_ut_callback_proprietary},
    {"*MCEERMODE" , (void *)ril_request_ceer_response_mode                                          , 1, true,  ril_response_ut_callback_proprietary},
    {"*MFTRCFG"  , (void *)ril_request_modem_feature_configuration                                  , 3, true,  ril_response_ut_callback_proprietary},
#endif
    {"^HVER"     , (void *)ril_request_request_hw_version                                           , 0, true,  ril_response_ut_callback_proprietary},
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
    {"^MODE"     , (void *)ril_request_indicate_system_mode                                         , 1, true,  ril_response_ut_callback_proprietary},
    {"^SYSINFO"  , (void *)ril_request_request_system_information                                   , 0, true,  ril_response_ut_callback_proprietary},
    {"^SYSCONFIG" , (void *)ril_request_configure_system_reference                                  , 5, true,  ril_response_ut_callback_proprietary},
    {"^CARDMODE" , (void *)ril_request_request_sim_usim_mode                                        , 0, true,  ril_response_ut_callback_proprietary},
    {"^SPN"      , (void *)ril_request_read_service_provider_name                                   , 1, true,  ril_response_ut_callback_proprietary},
    {"*MSTLOCK"  , (void *)ril_request_STK_registering                                              , 2, true,  ril_response_ut_callback_proprietary},
    {"*MSTPD"    , (void *)ril_request_STK_terminal_profile_download                                , 2, true,  ril_response_ut_callback_proprietary},
    {"*MSTMODE"  , (void *)ril_request_STK_output_format_setting                                    , 1, true,  ril_response_ut_callback_proprietary},
    {"*MSTICREC" , (void *)ril_request_STK_obtain_icon_record                                       , 1, true,  ril_response_ut_callback_proprietary},
    {"*MSTICIMG" , (void *)ril_request_STK_obtain_icon_image                                        , 3, true,  ril_response_ut_callback_proprietary},
    {"*MSTGC"    , (void *)ril_request_STK_parameters_associated_with_proactive_command             , 1, true,  ril_response_ut_callback_proprietary},
    {"*MSTCR"    , (void *)ril_request_STK_inform_response_to_proactive_command                     , 3, true,  ril_response_ut_callback_proprietary},
    {"*MSTMS"    , (void *)ril_request_STK_menu_selection                                           , 2, true,  ril_response_ut_callback_proprietary},
    {"*MSTEV"    , (void *)ril_request_STK_monitored_event_occurence                                , 10, true, ril_response_ut_callback_proprietary},
    {"*MICCID"   , (void *)ril_request_read_usim_iccid                                              , 0, true,  ril_response_ut_callback_proprietary},
    {"*MHOMENW"  , (void *)ril_request_display_home_network_information                             , 0, true,  ril_response_ut_callback_proprietary},
    {"*MCSIMLOCK" , (void *)ril_request_lock_csim_access                                            , 1, true,  ril_response_ut_callback_proprietary},
    {"*MEMMREEST" , (void *)ril_request_emm_re_establishment_test                                   , 0, true,  ril_response_ut_callback_proprietary},
    {"*MAPNURI"  , (void *)ril_request_apn_rate_control_indication                                  , 1, true,  ril_response_ut_callback_proprietary},
    {"*MPLMNURI" , (void *)ril_request_plmn_rate_control_indication                                 , 1, true,  ril_response_ut_callback_proprietary},
    {"*MPDI"     , (void *)ril_request_packet_discard_indication                                    , 1, true,  ril_response_ut_callback_proprietary},
    {"*MNBIOTDT" , (void *)ril_request_nbiot_data_type                                              , 3, true,  ril_response_ut_callback_proprietary},
    {"*MNBIOTRAI" , (void *)ril_request_nbiot_release_assistance_indication                         , 1, true,  ril_response_ut_callback_proprietary},
    {"*MNVMQ"    , (void *)ril_request_nvdm_status_query                                            , 1, true,  ril_response_ut_callback_proprietary},
    {"*MNVMAUTH" , (void *)ril_request_nvdm_access_security_authorization                           , 2, true,  ril_response_ut_callback_proprietary},
    {"*MNVMW"    , (void *)ril_request_nvdm_data_write                                              , 6, true,  ril_response_ut_callback_proprietary},
    {"*MNVMR"    , (void *)ril_request_nvdm_data_read                                               , 4, true,  ril_response_ut_callback_proprietary},
    {"*MNVMGET"  , (void *)ril_request_nvdm_get_all_data_item_id                                    , 0, true,  ril_response_ut_callback_proprietary},
    {"*MNVMIVD"  , (void *)ril_request_nvdm_data_item_invalidate                                    , 3, true,  ril_response_ut_callback_proprietary},
    {"*MNVMRSTONE" , (void *)ril_request_nvdm_data_item_factory_reset                               , 3, true,  ril_response_ut_callback_proprietary},
    {"*MNVMRST"  , (void *)ril_request_nvdm_factory_reset_all_data_item                             , 0, true,  ril_response_ut_callback_proprietary},
    {"*MNVMMDNQ" , (void *)ril_request_nvdm_query_num_of_mini_dump                                  , 0, true,  ril_response_ut_callback_proprietary},
    {"*MNVMMDR"  , (void *)ril_request_nvdm_read_mini_dump                                          , 3, true,  ril_response_ut_callback_proprietary},
    {"*MNVMMDC"  , (void *)ril_request_nvdm_clean_mini_dump                                         , 1, true,  ril_response_ut_callback_proprietary},
    {"+IDCFREQ"  , (void *)ril_request_set_idc_frequency_range                                      , 6, true,  ril_response_ut_callback_proprietary},
    {"+IDCPWRBACKOFF", (void *)ril_request_set_tx_power_back_off                                    , 1, true,  ril_response_ut_callback_proprietary},
    {"+IDCTX2GPS", (void *)ril_request_generate_periodic_tx_for_gps                                 , 4, true,  ril_response_ut_callback_proprietary},
#endif
    {"*MCALDEV"  , (void *)ril_request_enter_exit_rf_calibration_state                              , 1, true,  ril_response_ut_callback_proprietary},
    {"*MATWAKEUP", (void *)ril_request_set_modem_wakeup_indication                                  , 1, true,  ril_response_ut_callback_proprietary},
    {"*MBAND",     (void *)ril_request_query_modem_operating_band                                   , 0, true,  ril_response_ut_callback_proprietary},
    {"*MENGINFO",  (void *)ril_request_query_network_state                                          , 1, true,  ril_response_ut_callback_proprietary},
    {"*MFRCLLCK",  (void *)ril_request_frequency_and_cell_lock                                      , 4, true,  ril_response_ut_callback_proprietary},
    {"*MSPCHSC",   (void *)ril_request_set_scrambling_algorithm_for_npdsch                          , 1, true,  ril_response_ut_callback_proprietary},
    {"*MDPDNP",    (void *)ril_request_default_pdn_parameter                                        , 1, true,  ril_response_ut_callback_proprietary},
    {"*MEDRXCFG",  (void *)ril_request_eDRX_configuration                                           , 4, true,  ril_response_ut_callback_proprietary},
    {"*MCELLINFO", (void *)ril_request_serving_and_neighbor_cell_info                               , 0, true,  ril_response_ut_callback_proprietary},

    /* customized command */
    {"+CUSTOMCMD", (void *)ril_request_custom_command                                               , 1, false, (ril_cmd_response_callback_t)ril_response_ut_callback_custom_command},

};


typedef int32_t (*func_ptr_param0)(int32_t mode, void *rsp_cb, void *user_data);
typedef int32_t (*func_ptr_param1)(int32_t mode, void *param1, void *rsp_cb, void *user_data);
typedef int32_t (*func_ptr_param2)(int32_t mode, void *param1, void *param2, void *rsp_cb, void *user_data);
typedef int32_t (*func_ptr_param3)(int32_t mode, void *param1, void *param2, void *param3, void *rsp_cb, void *user_data);
typedef int32_t (*func_ptr_param4)(int32_t mode, void *param1, void *param2, void *param3, void *param4, void *rsp_cb, void *user_data);
typedef int32_t (*func_ptr_param5)(int32_t mode, void *struct_param, void *rsp_cb, void *user_data);
typedef int32_t (*func_ptr_param0_no_mode)(void *rsp_cb, void *user_data);
typedef int32_t (*func_ptr_param1_no_mode)(void *param1, void *rsp_cb, void *user_data);
typedef int32_t (*func_ptr_param2_no_mode)(void *param1, void *param2, void *rsp_cb, void *user_data);
typedef int32_t (*func_ptr_param3_no_mode)(void *param1, void *param2, void *param3, void *rsp_cb, void *user_data);
typedef int32_t (*func_ptr_param4_no_mode)(void *param1, void *param2, void *param3, void *param4, void *rsp_cb, void *user_data);
typedef int32_t (*func_ptr_param5_no_mode)(void *struct_param, void *rsp_cb, void *user_data);


int32_t ut_response_common_cb(ril_cmd_response_t *cmd_response)
{
    RIL_LOGI("%s enter\r\n", __FUNCTION__);

    RIL_LOGI("final result code: %d\r\n", cmd_response->res_code);
    RIL_LOGI("ril request mode %d\r\n", cmd_response->mode);

    return 0;
}


int32_t ut_hdlr_call_param0(int32_t mode, void *func_ptr, bool need_mode, void *rsp_cb)
{
    int32_t ret;

    if (!need_mode) {
        ret = ((func_ptr_param0_no_mode)func_ptr)((void *)rsp_cb, NULL);
    } else {
        ret = ((func_ptr_param0)func_ptr)(mode, (void *)rsp_cb, NULL);
    }
    return ret;
}

int32_t ut_hdlr_call_param1(int32_t mode, void *func_ptr, char *param_array[], bool need_mode, void *rsp_cb)
{
    int32_t ret;
    void *param;
    char *p;
    if (param_array[0] == NULL) {
        return -1;
    } else if (!strcmp(param_array[0], "ostr")) {
        param = (void *)RIL_OMITTED_STRING_PARAM;
    } else if (!strcmp(param_array[0], "oint")) {
        param = (void *)RIL_OMITTED_INTEGER_PARAM;
    } else if (*(param_array[0]) == '\"') {
        p = param_array[0];
        p++;
        param = (void *)strsep(&p, "\"");
    } else {
        param = (void *)atoi(param_array[0]);
    }

    if (!need_mode) {
        ret = ((func_ptr_param1_no_mode)func_ptr)(param, (void *)rsp_cb, NULL);
    } else {
        ret = ((func_ptr_param1)func_ptr)(mode, param, (void *)rsp_cb, NULL);
    }
    return ret;
}

int32_t ut_hdlr_call_param2(int32_t mode, void *func_ptr, char *param_array[], bool need_mode, void *rsp_cb)
{
    int32_t ret;
    void *param[2];
    int32_t i;
    char *p;
    for (i = 0; i < 2; i++) {
        if (param_array[i] == NULL) {
            return -1;
        } else if (!strcmp(param_array[i], "ostr")) {
            param[i] = (void *)RIL_OMITTED_STRING_PARAM;
        } else if (!strcmp(param_array[i], "oint")) {
            param[i] = (void *)RIL_OMITTED_INTEGER_PARAM;
        } else if (*(param_array[i]) == '\"') {
            p = param_array[i];
            p++;
            param[i] = (void *)strsep(&p, "\"");
        } else {
            param[i] = (void *)atoi(param_array[i]);
        }
    }

    if (!need_mode) {
        ret = ((func_ptr_param2_no_mode)func_ptr)(param[0], param[1], (void *)rsp_cb, NULL);
    } else {
        ret = ((func_ptr_param2)func_ptr)(mode, param[0], param[1], (void *)rsp_cb, NULL);
    }
    return ret;
}

int32_t ut_hdlr_call_param3(int32_t mode, void *func_ptr, char *param_array[], bool need_mode, void *rsp_cb)
{
    int32_t ret;
    void *param[3];
    int32_t i;
    char *p;
    for (i = 0; i < 3; i++) {
        if (param_array[i] == NULL) {
            return -1;
        } else if (!strcmp(param_array[i], "ostr")) {
            param[i] = (void *)RIL_OMITTED_STRING_PARAM;
        } else if (!strcmp(param_array[i], "oint")) {
            param[i] = (void *)RIL_OMITTED_INTEGER_PARAM;
        } else if (*(param_array[i]) == '\"') {
            p = param_array[i];
            p++;
            param[i] = (void *)strsep(&p, "\"");
        } else {
            param[i] = (void *)atoi(param_array[i]);
        }
    }

    if (!need_mode) {
        ret = ((func_ptr_param3_no_mode)func_ptr)(param[0], param[1], param[2], (void *)rsp_cb, NULL);
    } else {
        ret = ((func_ptr_param3)func_ptr)(mode, param[0], param[1], param[2], (void *)rsp_cb, NULL);
    }
    return ret;
}

int32_t ut_hdlr_call_param4(int32_t mode, void *func_ptr, char *param_array[], bool need_mode, void *rsp_cb)
{
    int32_t ret;
    void *param[4];
    int32_t i;
    char *p;
    for (i = 0; i < 4; i++) {
        if (param_array[i] == NULL) {
            return -1;
        } else if (!strcmp(param_array[i], "ostr")) {
            param[i] = (void *)RIL_OMITTED_STRING_PARAM;
        } else if (!strcmp(param_array[i], "oint")) {
            param[i] = (void *)RIL_OMITTED_INTEGER_PARAM;
        } else if (*(param_array[i]) == '\"') {
            p = param_array[i];
            p++;
            param[i] = (void *)strsep(&p, "\"");
        } else {
            param[i] = (void *)atoi(param_array[i]);
        }
    }

    if (!need_mode) {
        ret = ((func_ptr_param4_no_mode)func_ptr)(param[0], param[1], param[2], param[3], (void *)rsp_cb, NULL);
    } else {
        ret = ((func_ptr_param4)func_ptr)(mode, param[0], param[1], param[2], param[3], (void *)rsp_cb, NULL);
    }
    return ret;
}

int32_t ut_hdlr_call_param5(int32_t mode, void *func_ptr, char *param_array[], uint32_t param_num, bool need_mode, void *rsp_cb)
{
    int32_t ret;
    void *param[40];
    int32_t i;
    char *p;
    for (i = 0; i < param_num; i++) {
        if (param_array[i] == NULL) {
            return -1;
        } else if (!strcmp(param_array[i], "ostr")) {
            param[i] = (void *)RIL_OMITTED_STRING_PARAM;
        } else if (!strcmp(param_array[i], "oint")) {
            param[i] = (void *)RIL_OMITTED_INTEGER_PARAM;
        } else if (*(param_array[i]) == '\"') {
            p = param_array[i];
            p++;
            param[i] = (void *)strsep(&p, "\"");
        } else {
            param[i] = (void *)atoi(param_array[i]);
        }
    }

    if (!need_mode) {
        ret = ((func_ptr_param5_no_mode)func_ptr)(&param[0], (void *)rsp_cb, NULL);
    } else {
        ret = ((func_ptr_param5)func_ptr)(mode, &param[0], (void *)rsp_cb, NULL);
    }
    return ret;
}


int32_t ril_ut_cmd_send_dispatch(char *param_array[], uint32_t param_num)
{
    int32_t i;
    int32_t ret = -1;
    ril_ut_cmd_item_t *item = NULL;
    bool found = false;
    void *func_ptr = NULL;
    ril_cmd_response_callback_t rsp_cb = NULL;
    bool need_mode = true;
    int32_t ril_mode = 1;
    char *param1, *cmd_head;

    if (param_array == NULL || param_array[0] == NULL || param_num < 2) {
        RIL_LOGE("Invalid cmd buf\r\n");
        return -1;
    }

    RIL_LOGUT("The number of input parameter is: %d\r\n", (int)param_num - 2);
    /* get AT head from parameter 1 */
    param1 = param_array[0];
    if (*param1 == '"') {
        param1++;
        cmd_head = strsep(&param1, "\"");
    } else {
        cmd_head = param1;
        RIL_LOGW("Parameter 1 format is invalid, modem returns ERROR\r\n");
    }

    ril_mode = atoi(param_array[1]);
    if (ril_mode < RIL_EXECUTE_MODE || ril_mode > RIL_TEST_MODE) {
        RIL_LOGE("Invalid request mode, ril_mode: %d\r\n", (int)ril_mode);
        return -1;
    }

    for (i = 0; i < (sizeof(s_ut_at_cmd_tbl) / sizeof(ril_ut_cmd_item_t)); i++) {
        item = &s_ut_at_cmd_tbl[i];
        if (!strcmp(item->cmd, cmd_head)) {
            found = true;
            break;
        }
    }

    if (found == false) {
        RIL_LOGE("Cannot find this cmd.\r\n");
        return -1;
    } else {
        func_ptr = item->cmd_ut_hdlr;
        rsp_cb = item->rsp_cb;
        if (rsp_cb == NULL) {
            rsp_cb = (ril_cmd_response_callback_t)ut_response_common_cb;
        }
        need_mode = item->need_mode;

        /* special commands, cannot use AT command to test directly. */
        if (ril_mode == RIL_EXECUTE_MODE) {
            if (!strcmp(item->cmd, "+CGACT")) {
                ril_ut_send_CGACT(ril_mode, func_ptr, &param_array[2], param_num - 2, rsp_cb);
                return 0;
            }

            if (!strcmp(item->cmd, "+CGDATA")) {
                ril_ut_send_CGDATA(ril_mode, func_ptr, &param_array[2], param_num - 2, rsp_cb);
                return 0;
            }

            if (!strcmp(item->cmd, "+CGPADDR")) {
                ril_ut_send_CGPADDR(ril_mode, func_ptr, &param_array[2], param_num - 2, rsp_cb);
                return 0;
            } 
        }
        
        RIL_LOGUT("The number of function parameter MUST be: %d\r\n", (int)item->func_params_num);
        switch (item->func_params_num) {
            case 0:
                ret = ut_hdlr_call_param0(ril_mode, func_ptr, need_mode, (void *)rsp_cb);
                break;
            case 1:
                if (1 <= param_num - 2) {
                    ret = ut_hdlr_call_param1(ril_mode, func_ptr, &param_array[2], need_mode, (void *)rsp_cb);
                } else {
                    RIL_LOGW("the num of parameters don't match\r\n");
                }
                break;
            case 2:
                if (2 <= param_num - 2) {
                    ret = ut_hdlr_call_param2(ril_mode, func_ptr, &param_array[2], need_mode, (void *)rsp_cb);
                } else {
                    RIL_LOGW("the num of parameters don't match\r\n");
                }

                break;
            case 3:
                if (3 <= param_num - 2) {
                    ret = ut_hdlr_call_param3(ril_mode, func_ptr, &param_array[2], need_mode, (void *)rsp_cb);
                } else {
                    RIL_LOGW("the num of parameters don't match\r\n");
                }
                break;
            case 4:
                if (4 <= param_num - 2) {
                    ret = ut_hdlr_call_param4(ril_mode, func_ptr, &param_array[2], need_mode, (void *)rsp_cb);
                } else {
                    RIL_LOGW("the num of parameters don't match\r\n");
                }
                break;
            default:
                if (item->func_params_num <= param_num - 2) {
                    ret = ut_hdlr_call_param5(ril_mode, func_ptr, &param_array[2], param_num - 2, need_mode, (void *)rsp_cb);
                } else {
                    RIL_LOGW("the num of parameters don't match\r\n");
                }
                break;
        }
    }
    RIL_LOGUT("ril api call result: %d\r\n", (int)ret);

    ril_register_event_callback(RIL_GROUP_MASK_ALL, ril_urc_ut_callback);

    return ret;
}



int32_t ril_ut_cmd_response_dispatch(int32_t channel_id, char *cmd_buf)
{
    int32_t cmd_buf_len;
    ril_channel_cntx_t *cntx_p;

    if (cmd_buf == NULL || cmd_buf[0] == '\0') {
        RIL_LOGE("Invalid cmd buf\r\n");
        return -1;
    }

    if (channel_id < RIL_URC_CHANNEL_ID || channel_id > RIL_AT_DATA_CHANNEL_ID_END) {
        RIL_LOGE("Invalid channel id, channel_id: %d\r\n", (int)channel_id);
        return -1;
    }

    cntx_p = ril_get_channel(channel_id);
    cmd_buf_len = strlen(cmd_buf);
    memcpy(cntx_p->rx_buf_ptr, cmd_buf, cmd_buf_len);
    cntx_p->rx_pos = cmd_buf_len;
    ril_channel_response_hdlr(channel_id, cntx_p->rx_buf_ptr, cntx_p->rx_pos);

    return 0;
}


int32_t ril_ut_cmd_urc_dispatch(char *cmd_buf)
{
    int32_t cmd_buf_len;
    ril_channel_cntx_t *cntx_p;

    if (cmd_buf == NULL || cmd_buf[0] == '\0') {
        RIL_LOGE("Invalid cmd buf\r\n");
        return -1;
    }

    cntx_p = ril_get_channel(RIL_URC_CHANNEL_ID);
    cmd_buf_len = strlen(cmd_buf);
    memcpy(cntx_p->rx_buf_ptr, cmd_buf, cmd_buf_len);
    cntx_p->rx_pos = cmd_buf_len;
    ril_channel_urc_hdlr(cntx_p->rx_buf_ptr, cntx_p->rx_pos);

    return 0;
}

int32_t ril_urc_ut_callback(ril_urc_id_t event_id, void *param, uint32_t param_len)
{
    ril_urc_cmd_item_t *urc_item;
    if ((urc_item = get_urc_cmd_item(event_id)) != NULL) {
        RIL_LOGI("urc parsing reporting, event_id: %d, urc: %s\r\n", (int)event_id, urc_item->cmd_head);
    } else {
        return -1;
    }

    switch (event_id) {
        case RIL_URC_ID_CREG: {
            ril_network_registration_urc_t *response = (ril_network_registration_urc_t *)param;
            RIL_LOGI("stat:%d\r\n", (int) response->stat);
            RIL_LOGI("lac:%d\r\n", (int) response->lac);
            RIL_LOGI("ci:%d\r\n", (int) response->ci);
            RIL_LOGI("cause_type:%d\r\n", (int) response->cause_type);
            RIL_LOGI("reject_cause:%d\r\n", (int) response->reject_cause);
            break;
        }
        case RIL_URC_ID_CTZR: {
            ril_time_zone_reporting_urc_t *response = (ril_time_zone_reporting_urc_t *)param;
            RIL_LOGI("tz:%s\r\n", response->tz);
            RIL_LOGI("dst:%d\r\n", (int) response->dst);
            RIL_LOGI("time:%s\r\n", response->time);
            break;
        }
        case RIL_URC_ID_CGEV: {
            ril_cgev_event_reporting_urc_t *response = (ril_cgev_event_reporting_urc_t *)param;
            RIL_LOGI("stat:%d\r\n", (int) response->response_type);
            RIL_LOGI("response1:%d\r\n", (int) response->response1.cid);
            RIL_LOGI("response2:%d\r\n", (int) response->response2.cid);
            RIL_LOGI("response3:%d\r\n", (int) response->response3.cid);
            break;
        }
        case RIL_URC_ID_CGREG: {
            ril_gprs_network_registration_status_urc_t *response = (ril_gprs_network_registration_status_urc_t *)param;
            RIL_LOGI("stat: %d\r\n", (int) response->stat);
            RIL_LOGI("lac: %d\r\n", (int) response->lac);
            RIL_LOGI("ci: %d\r\n", (int) response->ci);
            RIL_LOGI("act: %d\r\n", (int) response->act);
            RIL_LOGI("rac: %d\r\n", (int) response->rac);
            RIL_LOGI("cause_type: %d\r\n", (int) response->cause_type);
            RIL_LOGI("reject_cause: %d\r\n", (int) response->reject_cause);
            RIL_LOGI("active_time: %d\r\n", (int) response->active_time);
            RIL_LOGI("periodic_rau: %d\r\n", (int) response->periodic_rau);
            RIL_LOGI("gprs_readytimer: %d\r\n", (int) response->gprs_readytimer);
            break;
        }
        case RIL_URC_ID_CEREG: {
            ril_eps_network_registration_status_urc_t *response = (ril_eps_network_registration_status_urc_t *)param;
            RIL_LOGI("stat: %d\r\n", (int) response->stat);
            RIL_LOGI("tac: %d\r\n", (int) response->tac);
            RIL_LOGI("ci: %d\r\n", (int) response->ci);
            RIL_LOGI("act: %d\r\n", (int) response->act);
            RIL_LOGI("cause_type: %d\r\n", (int) response->cause_type);
            RIL_LOGI("reject_cause: %d\r\n", (int) response->reject_cause);
            break;
        }
#ifndef __RIL_CMD_SET_SLIM_ENABLE__
        case RIL_URC_ID_MSTC: {
            ril_STK_proactive_command_indication_urc_t *response = (ril_STK_proactive_command_indication_urc_t *)param;
            RIL_LOGI("cmdid: %d\r\n", (int) response->cmdid);
            break;
        }
#endif
        case RIL_URC_ID_MGCOUNT: {
            ril_packet_domain_counters_urc_t *response = (ril_packet_domain_counters_urc_t *)param;
            RIL_LOGI("cid: %d\r\n", (int) response->cid);
            RIL_LOGI("ul: %ld*2^32 + %ld\r\n", (int32_t)(response->ul >> 32), (int32_t)(response->ul & 0xFFFFFFFF));
            RIL_LOGI("dl: %ld*2^32 + %ld\r\n", (int32_t)(response->dl >> 32), (int32_t)(response->dl & 0xFFFFFFFF));
            RIL_LOGI("pdp_type: %s\r\n", (int) response->pdp_type);
            break;
        }
        
        default:
            break;
    }
    return 0;
}
#endif /* __RIL_UT_TEST_CASES__ */

