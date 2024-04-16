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

#include "apb_proxy_packet_decoder_unit_test.h"
#include "apb_proxy_ap_packet_decoder.h"
#include "stdlib.h"
#include "apb_proxy_utility.h"
#include "FreeRTOS.h"
#include "apb_proxy_packet_def.h"

bool apb_proxy_get_packet_type_unit_test(void)
{
    uint8_t apb_proxy_xon_packet[APB_PROXY_XON_PACKET_SIZE] = {
        0x04, 0x00, 0x00, 0x00,
        0x0C, 0x00, 0x00, 0x00,
        0x0A, 0x00, 0x00, 0x00
    };
    uint32_t offset = 0;
    apb_proxy_packet_t apb_proxy_packet;
    apb_proxy_packet.pdata = apb_proxy_xon_packet;
    apb_proxy_packet.length = sizeof(apb_proxy_xon_packet);
    configASSERT(apb_proxy_get_packet_type(&apb_proxy_packet) == APBRIDGE_CODE_XON);
    return true;
}
bool apb_proxy_decode_at_cmd_req_msg_unit_test(void)
{
    char *p_cmd_string = "AT+TESTCMD=10";
    apb_proxy_packet_t apb_proxy_packet;
    apb_proxy_parse_cmd_param_t decoded_cmd_info;
    uint32_t offset = 0;
    uint32_t decoded_channel = 0;
    const uint32_t channel_id = 9;
    const uint32_t cmd_id = 565;
    const uint32_t request_type = 1;

    /*calculate the total length for AP Bridge packet.*/
    apb_proxy_packet.length = APB_PROXY_AT_CMD_REQ_PACKET_HEADER_SIZE;
    apb_proxy_packet.length = apb_proxy_packet.length + APB_PROXY_TLV_TYPE_FIELD_SIZE + APB_PROXY_TLV_LENGTH_FIELD_SIZE;
    apb_proxy_packet.length = apb_proxy_packet.length + strlen(p_cmd_string) + 1;
    apb_proxy_packet.pdata = (uint8_t *)pvPortMalloc(apb_proxy_packet.length);
    configASSERT(NULL != apb_proxy_packet.pdata);
    memset(apb_proxy_packet.pdata, 0, apb_proxy_packet.length);

    apb_proxy_set_uint32(apb_proxy_packet.pdata, &offset, APBRIDGE_CODE_AT_COMMAND, APB_PROXY_AT_CMD_REQ_PACKET_HEADER_SIZE);
    apb_proxy_set_uint32(apb_proxy_packet.pdata, &offset, apb_proxy_packet.length, APB_PROXY_AT_CMD_REQ_PACKET_HEADER_SIZE);
    apb_proxy_set_uint32(apb_proxy_packet.pdata, &offset, channel_id, APB_PROXY_AT_CMD_REQ_PACKET_HEADER_SIZE);
    apb_proxy_set_uint32(apb_proxy_packet.pdata, &offset, cmd_id, APB_PROXY_AT_CMD_REQ_PACKET_HEADER_SIZE);
    apb_proxy_set_uint32(apb_proxy_packet.pdata, &offset, request_type, APB_PROXY_AT_CMD_REQ_PACKET_HEADER_SIZE);

    apb_proxy_set_uint32(apb_proxy_packet.pdata, &offset, APBRIDGE_TLV_TYPE_STRING, apb_proxy_packet.length);
    apb_proxy_set_uint32(apb_proxy_packet.pdata, &offset, strlen(p_cmd_string) + 1, apb_proxy_packet.length);
    memcpy(apb_proxy_packet.pdata + offset, p_cmd_string, strlen(p_cmd_string) + 1);

    configASSERT(apb_proxy_decode_at_cmd_req_msg(&apb_proxy_packet, &decoded_cmd_info, &decoded_channel) == APB_PROXY_STATUS_OK);
    configASSERT(decoded_cmd_info.cmd_id == cmd_id);
    configASSERT(decoded_cmd_info.mode == request_type);
    configASSERT(decoded_channel == channel_id);
    configASSERT(strcmp(decoded_cmd_info.string_ptr, p_cmd_string) == 0);

    vPortFree(apb_proxy_packet.pdata);
    apb_proxy_packet.pdata = NULL;
    return true;
}
bool apb_proxy_decode_at_cmd_reg_result_msg_unit_test(void)
{
    /*encode the response packet.*/
    uint32_t offset = 0;
    apb_proxy_packet_t apb_proxy_packet;
    apb_proxy_register_result_t register_result;
    uint8_t *p_encoded_data = (uint8_t *)pvPortMalloc(APB_PROXY_AT_CMD_REG_RSP_PACKET_SIZE);
    configASSERT(p_encoded_data != NULL);

    /*set message type.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   APBRIDGE_CODE_COMMAND_REGISTERED,
                   APB_PROXY_AT_CMD_REG_RSP_PACKET_SIZE);
    /*set message total length.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   APB_PROXY_AT_CMD_REG_RSP_PACKET_SIZE,
                   APB_PROXY_AT_CMD_REG_RSP_PACKET_SIZE);
    /*set command base cmd ID.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   565,
                   APB_PROXY_AT_CMD_REG_RSP_PACKET_SIZE);
    /*set registered command count.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   3,
                   APB_PROXY_AT_CMD_REG_RSP_PACKET_SIZE);
    /*set registered result.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   1,
                   APB_PROXY_AT_CMD_REG_RSP_PACKET_SIZE);
    configASSERT(APB_PROXY_AT_CMD_REG_RSP_PACKET_SIZE == offset);
    apb_proxy_packet.pdata = p_encoded_data;
    apb_proxy_packet.length = APB_PROXY_AT_CMD_REG_RSP_PACKET_SIZE;
    configASSERT(apb_proxy_decode_at_cmd_reg_result_msg(&apb_proxy_packet,
                 &register_result) == APB_PROXY_STATUS_OK);
    configASSERT(register_result.base_cmd_id == 565);
    configASSERT(register_result.registered_count == 3);
    configASSERT(register_result.result == true);

    vPortFree(p_encoded_data);
    p_encoded_data = NULL;
    return true;
}
bool apb_proxy_decode_userdata_msg_unit_test(void)
{
    apb_proxy_packet_t apb_proxy_packet;
    apb_proxy_user_data_t decoded_data;
    uint8_t *p_user_data = NULL;
    uint8_t *p_decoded_user_data = NULL;
    uint32_t index = 0;
    uint8_t user_data_packet[] = {
        /*packet type*/
        0x03, 0x00, 0x00, 0x00,
        /*packet length*/
        0x19, 0x00, 0x00, 0x00,
        /*channel id*/
        0x0A, 0x00, 0x00, 0x00,
        /*user data*/
        0x01, 0x22, 0x99, 0x23,
        0x13, 0x0A, 0x33, 0x00,
        0x10, 0x00, 0x97, 0x00,
        0x00
    };
    apb_proxy_packet.pdata = user_data_packet;
    apb_proxy_packet.length = sizeof(user_data_packet);
    configASSERT(apb_proxy_decode_userdata_msg(&apb_proxy_packet, &decoded_data) == APB_PROXY_STATUS_OK);
    configASSERT(decoded_data.length == 13);
    configASSERT(decoded_data.pbuffer != NULL);
    p_user_data = user_data_packet + APB_PROXY_USER_DATA_HEADER_SIZE;
    p_decoded_user_data = (uint8_t *)(decoded_data.pbuffer);
    for (index = 0; index < decoded_data.length; index ++) {
        configASSERT( p_user_data[index] == p_decoded_user_data[index]);
    }
    return true;
}

