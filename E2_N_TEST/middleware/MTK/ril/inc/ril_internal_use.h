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

#ifndef __RIL_INTERNAL_USE_H__
#define __RIL_INTERNAL_USE_H__
#include "ril_general_types.h"
#include "ril.h"

/* enable UT test cases */
//#define __RIL_UT_TEST_CASES__

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************
 *                 proprietary command spec (internal use)
 ******************************************************************/

/* AT cmd: AT*MGCOUNT */
typedef struct {
    int32_t cid;
    int32_t state;
    int32_t period;
} ril_packet_domain_counters_struct_t;

typedef struct {
    int32_t cid;
    int64_t ul;
    int64_t dl;
    char *pdp_type;
} ril_packet_domain_counters_urc_t;

extern ril_status_t ril_request_packet_domain_counters(ril_request_mode_t mode,
        int32_t action,
        int32_t cid,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT*MGSINK */
typedef struct {
    int32_t nsapi;
    int32_t pcksize;
    int32_t pckcount;
    char *address;
    int32_t port;
} ril_send_packet_to_discard_param_t;

extern ril_status_t ril_request_send_packet_to_discard(ril_request_mode_t mode,
        ril_send_packet_to_discard_param_t *param,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT*MGTCSINK */
typedef struct {
    int32_t nsapi;
    int32_t pcktsize;
    int32_t pcktcount;
    int32_t address;
    int32_t port;
} ril_send_send_tcpip_packet_to_discard_req_t;

extern ril_status_t ril_request_send_tcpip_packet_to_discard(ril_request_mode_t mode,
        ril_send_send_tcpip_packet_to_discard_req_t *config,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT*MCAL */
#define RIL_RADIO_CALL_REQ_MAX_PARAM_COUNT 10
typedef struct {
    char *cmd_group;
    int32_t item_index;
    int32_t function_index;
    int32_t param_count;
    int32_t param_array[RIL_RADIO_CALL_REQ_MAX_PARAM_COUNT];
    int32_t check_sum;
} ril_radio_call_req_t;
typedef struct {
    char *cmd_group;
    int32_t item_index;
    int32_t function_index;
    int32_t status_code;
    int32_t param_count;
    int32_t param_array[RIL_RADIO_CALL_REQ_MAX_PARAM_COUNT];
    int32_t check_sum;
} ril_radio_call_rsp_t;

extern ril_status_t ril_request_radio_call(ril_request_mode_t mode,
        ril_radio_call_req_t *req,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT^HVER */
typedef struct {
    char *hw_version;
} ril_request_hw_version_rsp_t;

extern ril_status_t ril_request_request_hw_version(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT^MODE */
/* AT URC: ^MODE */
typedef struct {
    int32_t sys_mode;
} ril_indicate_system_mode_urc_t;
typedef struct {
    int32_t enable;
    int32_t sys_mode;
} ril_indicate_system_mode_rsp_t;

extern ril_status_t ril_request_indicate_system_mode(ril_request_mode_t mode,
        int32_t enable,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT^SYSINFO */
typedef struct {
    int32_t srv_status;
    int32_t srv_domain;
    int32_t roam_status;
    int32_t sys_mode;
    int32_t sim_state;
    int32_t reserve;
} ril_request_system_information_rsp_t;

extern ril_status_t ril_request_request_system_information(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT^SYSCONFIG */
typedef struct {
    int32_t mode;
    int32_t acqorder;
    int32_t roam;
    int32_t srvdomain;
} ril_configure_system_reference_rsp_t, ril_configure_system_reference_req_t;

extern ril_status_t ril_request_configure_system_reference(ril_request_mode_t mode,
        ril_configure_system_reference_req_t *req,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT^CARDMODE */
typedef struct {
    int32_t sim_type;
} ril_request_sim_usim_mode_rsp_t;

extern ril_status_t ril_request_request_sim_usim_mode(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT^SPN */
typedef struct {
    int32_t disp_rplmn;
    int32_t coding;
    int32_t spn_name;
} ril_read_service_provider_name_rsp_t;

extern ril_status_t ril_request_read_service_provider_name(ril_request_mode_t mode,
        int32_t spn_type,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT*MEMMREEST */
extern ril_status_t ril_request_emm_re_establishment_test(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT*MNVMQ */
typedef struct {
    char *chip_name;
    int32_t needs_security_authentication;
} ril_nvdm_status_query_rsp_t;

extern ril_status_t ril_request_nvdm_status_query(ril_request_mode_t mode,
        char *tool_version,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT*MNVMAUTH */
typedef struct {
    int32_t auth_result;
} ril_nvdm_access_security_authorization_rsp_t;

extern ril_status_t ril_request_nvdm_access_security_authorization(ril_request_mode_t mode,
        int32_t cert_data_length,
        char *certificate_bytes_data,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT*MNVMW*/
typedef struct {
    int32_t write_status;
} ril_nvdm_data_write_rsp_t;
typedef struct {
    int32_t area_info;
    char *group_id;
    char *data_item_id;
    int32_t data_type;
    int32_t length;
    char *data;
} ril_nvdm_data_write_req_t;

extern ril_status_t ril_request_nvdm_data_write(ril_request_mode_t mode,
        ril_nvdm_data_write_req_t *req_param,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT*MNVMR */
typedef struct {
    int32_t read_status;
    int32_t area_info;
    char *group_id;
    char *data_item_id;
    int32_t data_type;
    int32_t length;
    char *data;
} ril_nvdm_data_read_rsp_t;

extern ril_status_t ril_request_nvdm_data_read(ril_request_mode_t mode,
        int32_t area_info,
        char *group_id,
        char *data_item_id,
        int32_t length,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT*MNVMGET */
typedef struct {
    int32_t area_info;
    char *group_id;
    char *data_item_id;
} ril_nvdm_get_all_data_item_id_entry_t;
typedef struct {
    int32_t status;
    int32_t entry_num;
    ril_nvdm_get_all_data_item_id_entry_t *param_list;
} ril_nvdm_get_all_data_item_id_rsp_t;

extern ril_status_t ril_request_nvdm_get_all_data_item_id(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT*MNVMIVD */
typedef struct {
    int32_t status;
} ril_nvdm_data_item_invalidate_rsp_t;

extern ril_status_t ril_request_nvdm_data_item_invalidate(ril_request_mode_t mode,
        int32_t area_info,
        char *group_id,
        char *data_item_id,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT*MNVMRSTONE */
typedef struct {
    int32_t status;
} ril_nvdm_data_item_factory_reset_rsp_t;

extern ril_status_t ril_request_nvdm_data_item_factory_reset(ril_request_mode_t mode,
        int32_t area_info,
        char *group_id,
        char *data_item_id,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT*MNVMRST */
typedef struct {
    int32_t status;
} ril_nvdm_factory_reset_all_data_item_rsp_t;

extern ril_status_t ril_request_nvdm_factory_reset_all_data_item(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT*MNVMMDNQ */
typedef struct {
    int32_t status;
    int32_t mini_dump_num;
} ril_nvdm_query_num_of_mini_dump_rsp_t;

extern ril_status_t ril_request_nvdm_query_num_of_mini_dump(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT*MNVMMDR */
typedef struct {
    int32_t status;
    int32_t mini_dump_idx;
    int32_t offset;
    int32_t data_length;
    char *data;
} ril_nvdm_read_mini_dump_rsp_t;

extern ril_status_t ril_request_nvdm_read_mini_dump(ril_request_mode_t mode,
        int32_t mini_dump_idx,
        int32_t offset,
        int32_t data_length,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT*MNVMMDC */
typedef struct {
    int32_t status;
    int32_t mini_dump_idx;
} ril_nvdm_clean_mini_dump_rsp_t;

extern ril_status_t ril_request_nvdm_clean_mini_dump(ril_request_mode_t mode,
        int32_t mini_dump_idx,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT+IDCFREQ */
/* AT URC: +IDCSTATUS */
typedef struct {
    int32_t tx_status;
    int32_t carrier_freq;
    int32_t tx_power;
} ril_set_idc_frequency_range_urc_t;
typedef struct {
    int32_t freq1_start;
    int32_t freq1_stop;
    int32_t freq2_start;
    int32_t freq2_stop;
    int32_t freq3_start;
    int32_t freq3_stop;
} ril_set_idc_frequency_range_req_t;

extern ril_status_t ril_request_set_idc_frequency_range(ril_request_mode_t mode,
        ril_set_idc_frequency_range_req_t *req_param,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT+IDCPWRBACKOFF */
extern ril_status_t ril_request_set_tx_power_back_off(ril_request_mode_t mode,
        int32_t attenuation_power,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT+IDCTX2GPS */
extern ril_status_t ril_request_generate_periodic_tx_for_gps(ril_request_mode_t mode,
        int32_t request_mode,
        int32_t freq,
        int32_t tx_pwr,
        int32_t period,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT*MCALDEV */
typedef struct {
    int32_t caldev_state;
} ril_enter_exit_rf_calibration_state_rsp_t;

extern ril_status_t ril_request_enter_exit_rf_calibration_state(ril_request_mode_t mode,
        int32_t caldev_state,
        ril_cmd_response_callback_t callback,
        void *user_data);


/* AT cmd: AT*MUPDIR */
typedef struct {
    int32_t mode;
    int32_t pattern_id;
    char *ip_type;
    char *src_ip;
    int32_t src_udp_port;
    char *dst_ip;
    int32_t dst_udp_port;
    int32_t msg_id_offset;
    int32_t msg_id_size;
} ril_indicate_packet_to_track_req_t;

typedef struct {
    int32_t pattern_id;
} ril_indicate_packet_to_track_urc_t;

typedef struct {
    int32_t pattern_id;
    int32_t status;
    int32_t num_msg_id;
    int32_t *msg_id_list;
} ril_indicate_packet_delivery_status_urc_t;

extern ril_status_t ril_request_indicate_packet_to_track(ril_request_mode_t mode,
        ril_indicate_packet_to_track_req_t *req_param,
        ril_cmd_response_callback_t callback,
        void *user_data);
/* for customized command routine self-test */
extern ril_status_t ril_request_custom_command(char *cmd_string,
        ril_custom_cmd_response_callback_t callback,
        void *user_data);



#ifdef __cplusplus
}
#endif
#endif /* __RIL_INTERNAL_USE_H__ */

