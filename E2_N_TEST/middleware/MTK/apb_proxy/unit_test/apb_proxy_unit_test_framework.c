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

#include "apb_proxy_unit_test_framework.h"
#include "stdlib.h"
/*test case's implementation header.*/
#include "apb_proxy_utility_unit_test.h"
#include "apb_proxy_queue_unit_test.h"
#include "apb_proxy_packet_encoder_unit_test.h"
#include "apb_proxy_packet_decoder_unit_test.h"

typedef struct {
    char *p_unit_test_name;
    unit_test_proc test_process;
} unit_test_case_t;

static unit_test_case_t g_unit_test_suite[] = {
    /*unit test cases for apb_proxy_utility.*/
    {"apb_proxy_set_uint32_unit_test", apb_proxy_set_uint32_unit_test},
    {"apb_proxy_set_uint64_unit_test", apb_proxy_set_uint64_unit_test},
    {"apb_proxy_get_uint32_unit_test", apb_proxy_get_uint32_unit_test},
    {"apb_proxy_get_uint64_unit_test", apb_proxy_get_uint64_unit_test},
    {"apb_proxy_set_int64_unit_test", apb_proxy_set_int64_unit_test},
    {"apb_proxy_set_int32_unit_test", apb_proxy_set_int32_unit_test},
    {"apb_proxy_get_int32_unit_test", apb_proxy_get_int32_unit_test},
    {"apb_proxy_get_int64_unit_test", apb_proxy_get_int64_unit_test},
    /*unit test cases for apb_proxy_queue*/
    {"apb_proxy_queue_unit_test", apb_proxy_queue_unit_test},
    /*unit test cases for apb_proxy_packet_encoder*/
    {"apb_proxy_encode_at_cmd_reg_packet_unit_test", apb_proxy_encode_at_cmd_reg_packet_unit_test},
    {"apb_proxy_encode_at_cmd_result_packet_unit_test", apb_proxy_encode_at_cmd_result_packet_unit_test},
    {"apb_proxy_encode_user_data_packet_unit_test", apb_proxy_encode_user_data_packet_unit_test},
    {"apb_proxy_encode_xon_packet_unit_test", apb_proxy_encode_xon_packet_unit_test},
    {"apb_proxy_encode_xoff_packet_unit_test", apb_proxy_encode_xoff_packet_unit_test},
    {"apb_proxy_fill_channel_id_to_packet_unit_test", apb_proxy_fill_channel_id_to_packet_unit_test},
    {"apb_proxy_get_at_cmd_result_code_unit_test", apb_proxy_get_at_cmd_result_code_unit_test},
    /*unit test cases for apb_proxy_packet_decoder*/
    {"apb_proxy_get_packet_type_unit_test", apb_proxy_get_packet_type_unit_test},
    {"apb_proxy_decode_at_cmd_req_msg_unit_test", apb_proxy_decode_at_cmd_req_msg_unit_test},
    {"apb_proxy_decode_at_cmd_reg_result_msg_unit_test", apb_proxy_decode_at_cmd_reg_result_msg_unit_test},
    {"apb_proxy_decode_userdata_msg_unit_test", apb_proxy_decode_userdata_msg_unit_test}
};

void execute_unit_test(void)
{
    uint32_t index = 0;
    uint32_t test_failed_count = 0;
    uint32_t test_count = sizeof(g_unit_test_suite) / sizeof(unit_test_case_t);
    unit_test_case_t *p_test_case = NULL;

    printf("begin to execute_unit_test\n\r");
    for (index = 0; index < test_count; index++) {
        p_test_case = g_unit_test_suite + index;
        if ((p_test_case->test_process)() == true) {
            printf("unit test: %s passed\n\r", g_unit_test_suite[index].p_unit_test_name);
        } else {
            test_failed_count ++;
            printf("unit test: %s failed\n\r", g_unit_test_suite[index].p_unit_test_name);
        }
    }
    printf("**********unit test: report****************\n\r");
    printf("**** unit test: %d failed ****\n\r", test_failed_count);
    printf("**** unit test: %d passed ****\n\r", test_count - test_failed_count);
}

