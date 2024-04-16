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

#include "mux_ap.h"
#include "FreeRTOS.h"
#include "apb_proxy_log.h"
#include "apb_proxy_utility.h"
#include "apb_proxy_packet_def.h"
#include "stdlib.h"

/********************************************************************************************
 *                                         Unit Test Data
 ********************************************************************************************/

static mux_ap_callback_t g_ap_bridge_call_back = NULL;
static mux_ap_channel_id_t g_ap_bridge_channel_id = 1U;
static uint32_t g_cmd1_channel_id = 12;
static uint32_t g_cmd2_channel_id = 9;
static uint32_t g_cmd3_channel_id = 10;
static bool g_data_mode_actived = false;
static void mux_ap_stub_process_packet(uint8_t *data_buffer, uint32_t buffer_length);
static void mux_ap_stub_at_cmd_register_rsp(void);
static void mux_ap_stub_at_cmd_register_req(uint8_t *data_buffer, uint32_t buffer_length);
static void mux_ap_stub_user_data_req(uint8_t *data_buffer, uint32_t buffer_length);
static void mux_ap_stub_at_cmd1_req(void);
static void mux_ap_stub_at_cmd2_req(void);
/*for user data mode test case.*/
static void mux_ap_stub_at_cmd3_req(void);
static void mux_ap_stub_send_xon_req(void);
static void mux_ap_stub_send_xoff_req(void);

static void mux_ap_stub_at_cmd_register_req(uint8_t *data_buffer, uint32_t buffer_length)
{
    uint32_t offset = 0;
    char *p_cmd1_string = NULL;
    char *p_cmd2_string = NULL;
    char *p_cmd3_string = NULL;
    /*check packet type.*/
    configASSERT(apb_proxy_get_uint32(data_buffer, &offset) == APBRIDGE_CODE_REGISTER_COMMAND);
    /*check packet length.*/
    configASSERT(apb_proxy_get_uint32(data_buffer, &offset) == buffer_length);
    /*check register total command counts in AP Brigde proxy.*/
    configASSERT(apb_proxy_get_uint32(data_buffer, &offset) == 3);
    /*check how many at commands are in this packet.*/
    configASSERT(apb_proxy_get_uint32(data_buffer, &offset) == 3);
    p_cmd1_string = data_buffer + offset;
    printf("comand1: %s \n\r", data_buffer + offset);
    while (*(data_buffer + offset)) {
        offset++;
    }
    p_cmd2_string = data_buffer + offset + 1;
    printf("comand2: %s \n\r", data_buffer + offset + 1);
    offset = offset + 1;
    while (*(data_buffer + offset)) {
        offset++;
    }
    p_cmd3_string = data_buffer + offset + 1;
    printf("comand3: %s \n\r", data_buffer + offset + 1);

}

static void mux_ap_stub_at_cmd_register_rsp(void)
{
    /*encode the response packet.*/
    uint32_t offset = 0;
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

    mux_ap_event_prepare_to_receive_t prepare_receive_event;
    prepare_receive_event.buffer_length = APB_PROXY_AT_CMD_REG_RSP_PACKET_SIZE;
    prepare_receive_event.channel_id = g_ap_bridge_channel_id;
    prepare_receive_event.data_buffer = NULL;
    prepare_receive_event.user_data = NULL;
    /*send event to inform AP Bridge allocate buffer.*/
    g_ap_bridge_call_back(MUX_AP_EVENT_PREPARE_TO_RECEIVE, &prepare_receive_event);
    configASSERT(prepare_receive_event.data_buffer != NULL);
    configASSERT(prepare_receive_event.user_data == NULL);
    memcpy(prepare_receive_event.data_buffer, p_encoded_data, APB_PROXY_AT_CMD_REG_RSP_PACKET_SIZE);
    mux_ap_event_receive_completed_t receive_completed_event;
    receive_completed_event.buffer_length = APB_PROXY_AT_CMD_REG_RSP_PACKET_SIZE;
    receive_completed_event.channel_id = g_ap_bridge_channel_id;
    receive_completed_event.data_buffer = prepare_receive_event.data_buffer;
    receive_completed_event.user_data = NULL;
    /*inform AP Bridge data is ready for its buffer.*/
    g_ap_bridge_call_back(MUX_AP_EVENT_RECEIVE_COMPLETED, &receive_completed_event);
    vPortFree(p_encoded_data);
    p_encoded_data = NULL;
}

static void mux_ap_stub_at_cmd1_req(void)
{
    /*encode at command request packet message.*/
    char *p_string = "AT+TESTCMD1";
    uint8_t *p_encoded_data = NULL;
    uint32_t offset = 0;
    uint32_t total_length = APB_PROXY_AT_CMD_REQ_PACKET_HEADER_SIZE + APB_PROXY_TLV_TYPE_FIELD_SIZE
                            + APB_PROXY_TLV_LENGTH_FIELD_SIZE + strlen(p_string) + 1;
    p_encoded_data = (uint8_t *)pvPortMalloc(total_length);
    configASSERT(p_encoded_data != NULL);

    /*set message type.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   APBRIDGE_CODE_AT_COMMAND,
                   total_length);

    /*set message total length.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   total_length,
                   total_length);

    /*set command channel ID.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   12,
                   total_length);

    /*set cmd id.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   565,
                   APB_PROXY_AT_CMD_REG_RSP_PACKET_SIZE);

    /*set at cmd request type.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   APBRIDGE_REQUEST_TYPE_ACTION,
                   total_length);

    /*set tlv type.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   APBRIDGE_TLV_TYPE_STRING,
                   total_length);

    /*set tlv length.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   strlen(p_string) + 1,
                   total_length);
    /*set tlv value*/
    configASSERT((offset + strlen(p_string) + 1) == total_length);
    memcpy(p_encoded_data + offset, p_string, strlen(p_string) + 1);

    mux_ap_event_prepare_to_receive_t prepare_receive_event;
    prepare_receive_event.buffer_length = total_length;
    prepare_receive_event.channel_id = g_ap_bridge_channel_id;
    prepare_receive_event.data_buffer = NULL;
    prepare_receive_event.user_data = NULL;
    /*inform AP Bridge allocate buffer to save new data.*/
    g_ap_bridge_call_back(MUX_AP_EVENT_PREPARE_TO_RECEIVE, &prepare_receive_event);
    configASSERT(prepare_receive_event.data_buffer != NULL);
    configASSERT(prepare_receive_event.user_data == NULL);
    memcpy(prepare_receive_event.data_buffer, p_encoded_data, total_length);

    mux_ap_event_receive_completed_t receive_completed_event;
    receive_completed_event.buffer_length = total_length;
    receive_completed_event.channel_id = g_ap_bridge_channel_id;
    receive_completed_event.data_buffer = prepare_receive_event.data_buffer;
    receive_completed_event.user_data = NULL;
    /*inform AP Bridge data is ready.*/
    g_ap_bridge_call_back(MUX_AP_EVENT_RECEIVE_COMPLETED, &receive_completed_event);
    vPortFree(p_encoded_data);
    p_encoded_data = NULL;

}

static void mux_ap_stub_at_cmd2_req(void)
{
    /*encode at command request packet message.*/
    char *p_string = "AT+TESTCMD2";
    uint8_t *p_encoded_data = NULL;
    uint32_t offset = 0;
    uint32_t total_length = APB_PROXY_AT_CMD_REQ_PACKET_HEADER_SIZE + APB_PROXY_TLV_TYPE_FIELD_SIZE
                            + APB_PROXY_TLV_LENGTH_FIELD_SIZE + strlen(p_string) + 1;
    p_encoded_data = (uint8_t *)pvPortMalloc(total_length);
    configASSERT(p_encoded_data != NULL);

    /*set message type.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   APBRIDGE_CODE_AT_COMMAND,
                   total_length);

    /*set message total length.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   total_length,
                   total_length);

    /*set command channel ID.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   9,
                   total_length);

    /*set cmd id.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   566,
                   APB_PROXY_AT_CMD_REG_RSP_PACKET_SIZE);

    /*set at cmd request type.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   APBRIDGE_REQUEST_TYPE_ACTION,
                   total_length);

    /*set tlv type.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   APBRIDGE_TLV_TYPE_STRING,
                   total_length);

    /*set tlv length.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   strlen(p_string) + 1,
                   total_length);
    /*set tlv value*/
    configASSERT((offset + strlen(p_string) + 1) == total_length);
    memcpy(p_encoded_data + offset, p_string, strlen(p_string) + 1);

    mux_ap_event_prepare_to_receive_t prepare_receive_event;
    prepare_receive_event.buffer_length = total_length;
    prepare_receive_event.channel_id = g_ap_bridge_channel_id;
    prepare_receive_event.data_buffer = NULL;
    prepare_receive_event.user_data = NULL;
    /*inform AP Bridge allocate buffer to save new data.*/
    g_ap_bridge_call_back(MUX_AP_EVENT_PREPARE_TO_RECEIVE, &prepare_receive_event);
    configASSERT(prepare_receive_event.data_buffer != NULL);
    configASSERT(prepare_receive_event.user_data == NULL);
    memcpy(prepare_receive_event.data_buffer, p_encoded_data, total_length);

    mux_ap_event_receive_completed_t receive_completed_event;
    receive_completed_event.buffer_length = total_length;
    receive_completed_event.channel_id = g_ap_bridge_channel_id;
    receive_completed_event.data_buffer = prepare_receive_event.data_buffer;
    receive_completed_event.user_data = NULL;
    /*inform AP Bridge data is ready.*/
    g_ap_bridge_call_back(MUX_AP_EVENT_RECEIVE_COMPLETED, &receive_completed_event);
    vPortFree(p_encoded_data);
    p_encoded_data = NULL;

}

/*for user data mode test case.*/
static void mux_ap_stub_at_cmd3_req(void)
{
    /*encode at command request packet message.*/
    char *p_string = "AT+TESTCMD3";
    uint8_t *p_encoded_data = NULL;
    uint32_t offset = 0;
    uint32_t total_length = APB_PROXY_AT_CMD_REQ_PACKET_HEADER_SIZE + APB_PROXY_TLV_TYPE_FIELD_SIZE
                            + APB_PROXY_TLV_LENGTH_FIELD_SIZE + strlen(p_string) + 1;
    p_encoded_data = (uint8_t *)pvPortMalloc(total_length);
    configASSERT(p_encoded_data != NULL);

    /*set message type.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   APBRIDGE_CODE_AT_COMMAND,
                   total_length);

    /*set message total length.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   total_length,
                   total_length);

    /*set command channel ID.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   10,
                   total_length);

    /*set cmd id.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   567,
                   APB_PROXY_AT_CMD_REG_RSP_PACKET_SIZE);

    /*set at cmd request type.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   APBRIDGE_REQUEST_TYPE_ACTION,
                   total_length);

    /*set tlv type.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   APBRIDGE_TLV_TYPE_STRING,
                   total_length);

    /*set tlv length.*/
    apb_proxy_set_uint32(p_encoded_data, &offset,
                   strlen(p_string) + 1,
                   total_length);
    /*set tlv value*/
    configASSERT((offset + strlen(p_string) + 1) == total_length);
    memcpy(p_encoded_data + offset, p_string, strlen(p_string) + 1);

    mux_ap_event_prepare_to_receive_t prepare_receive_event;
    prepare_receive_event.buffer_length = total_length;
    prepare_receive_event.channel_id = g_ap_bridge_channel_id;
    prepare_receive_event.data_buffer = NULL;
    prepare_receive_event.user_data = NULL;
    /*inform AP Bridge allocate buffer to save new data.*/
    g_ap_bridge_call_back(MUX_AP_EVENT_PREPARE_TO_RECEIVE, &prepare_receive_event);
    configASSERT(prepare_receive_event.data_buffer != NULL);
    configASSERT(prepare_receive_event.user_data == NULL);
    memcpy(prepare_receive_event.data_buffer, p_encoded_data, total_length);

    mux_ap_event_receive_completed_t receive_completed_event;
    receive_completed_event.buffer_length = total_length;
    receive_completed_event.channel_id = g_ap_bridge_channel_id;
    receive_completed_event.data_buffer = prepare_receive_event.data_buffer;
    receive_completed_event.user_data = NULL;
    /*inform AP Bridge data is ready.*/
    g_ap_bridge_call_back(MUX_AP_EVENT_RECEIVE_COMPLETED, &receive_completed_event);
    vPortFree(p_encoded_data);
    p_encoded_data = NULL;
}

static void mux_ap_stub_user_data_req(uint8_t *data_buffer, uint32_t buffer_length)
{
    configASSERT(data_buffer != NULL);
    configASSERT(buffer_length != 0);
    uint32_t offset = 0;
    /*check packet type.*/
    configASSERT(apb_proxy_get_uint32(data_buffer, &offset) == APBRIDGE_CODE_USER_DATA);
    /*check packet length.*/
    configASSERT(apb_proxy_get_uint32(data_buffer, &offset) == buffer_length);
    /*check channel id*/
    configASSERT(apb_proxy_get_uint32(data_buffer, &offset) == g_cmd3_channel_id);

    mux_ap_event_prepare_to_receive_t prepare_receive_event;
    prepare_receive_event.buffer_length = buffer_length;
    prepare_receive_event.channel_id = g_ap_bridge_channel_id;
    prepare_receive_event.data_buffer = NULL;
    prepare_receive_event.user_data = NULL;
    /*inform AP Bridge allocate buffer to save new data.*/
    g_ap_bridge_call_back(MUX_AP_EVENT_PREPARE_TO_RECEIVE, &prepare_receive_event);
    printf("AP Bridge give user data buffer: 0x%x\n\r", prepare_receive_event.data_buffer);
    configASSERT(prepare_receive_event.data_buffer != NULL);
    configASSERT(prepare_receive_event.user_data == NULL);
    memcpy(prepare_receive_event.data_buffer, data_buffer, buffer_length);

    mux_ap_event_receive_completed_t receive_completed_event;
    receive_completed_event.buffer_length = prepare_receive_event.buffer_length;
    receive_completed_event.channel_id = g_ap_bridge_channel_id;
    receive_completed_event.data_buffer = prepare_receive_event.data_buffer;
    receive_completed_event.user_data = NULL;
    /*inform AP Bridge data is ready.*/
    g_ap_bridge_call_back(MUX_AP_EVENT_RECEIVE_COMPLETED, &receive_completed_event);

}

static void mux_ap_stub_send_xon_req(void)
{
    uint8_t xon_packet_data[APB_PROXY_XON_PACKET_SIZE];
    uint32_t offset = 0;
    apb_proxy_set_uint32(xon_packet_data, &offset, APBRIDGE_CODE_XON, APB_PROXY_XON_PACKET_SIZE);
    apb_proxy_set_uint32(xon_packet_data, &offset, APB_PROXY_XON_PACKET_SIZE, APB_PROXY_XON_PACKET_SIZE);
    apb_proxy_set_uint32(xon_packet_data, &offset, g_cmd3_channel_id, APB_PROXY_XON_PACKET_SIZE);

    mux_ap_event_prepare_to_receive_t prepare_receive_event;
    prepare_receive_event.buffer_length = APB_PROXY_XON_PACKET_SIZE;
    prepare_receive_event.channel_id = g_ap_bridge_channel_id;
    prepare_receive_event.data_buffer = NULL;
    prepare_receive_event.user_data = NULL;
    /*inform AP Bridge allocate buffer to save new data.*/
    g_ap_bridge_call_back(MUX_AP_EVENT_PREPARE_TO_RECEIVE, &prepare_receive_event);
    configASSERT(prepare_receive_event.data_buffer != NULL);
    configASSERT(prepare_receive_event.user_data == NULL);
    memcpy(prepare_receive_event.data_buffer, xon_packet_data, APB_PROXY_XON_PACKET_SIZE);

    mux_ap_event_receive_completed_t receive_completed_event;
    receive_completed_event.buffer_length = prepare_receive_event.buffer_length;
    receive_completed_event.channel_id = g_ap_bridge_channel_id;
    receive_completed_event.data_buffer = prepare_receive_event.data_buffer;
    receive_completed_event.user_data = NULL;
    /*inform AP Bridge data is ready.*/
    g_ap_bridge_call_back(MUX_AP_EVENT_RECEIVE_COMPLETED, &receive_completed_event);
}
static void mux_ap_stub_send_xoff_req(void)
{
    uint8_t xon_packet_data[APB_PROXY_XOFF_PACKET_SIZE];
    uint32_t offset = 0;
    apb_proxy_set_uint32(xon_packet_data, &offset, APBRIDGE_CODE_XOFF, APB_PROXY_XOFF_PACKET_SIZE);
    apb_proxy_set_uint32(xon_packet_data, &offset, APBRIDGE_CODE_XOFF, APB_PROXY_XOFF_PACKET_SIZE);
    apb_proxy_set_uint32(xon_packet_data, &offset, g_cmd3_channel_id, APB_PROXY_XOFF_PACKET_SIZE);

    mux_ap_event_prepare_to_receive_t prepare_receive_event;
    prepare_receive_event.buffer_length = APB_PROXY_XOFF_PACKET_SIZE;
    prepare_receive_event.channel_id = g_ap_bridge_channel_id;
    prepare_receive_event.data_buffer = NULL;
    prepare_receive_event.user_data = NULL;
    /*inform AP Bridge allocate buffer to save new data.*/
    g_ap_bridge_call_back(MUX_AP_EVENT_PREPARE_TO_RECEIVE, &prepare_receive_event);
    configASSERT(prepare_receive_event.data_buffer != NULL);
    configASSERT(prepare_receive_event.user_data == NULL);
    memcpy(prepare_receive_event.data_buffer, xon_packet_data, APB_PROXY_XOFF_PACKET_SIZE);

    mux_ap_event_receive_completed_t receive_completed_event;
    receive_completed_event.buffer_length = prepare_receive_event.buffer_length;
    receive_completed_event.channel_id = g_ap_bridge_channel_id;
    receive_completed_event.data_buffer = prepare_receive_event.data_buffer;
    receive_completed_event.user_data = NULL;
    /*inform AP Bridge data is ready.*/
    g_ap_bridge_call_back(MUX_AP_EVENT_RECEIVE_COMPLETED, &receive_completed_event);
}

static void mux_ap_stub_process_packet(uint8_t *data_buffer, uint32_t buffer_length)
{
    configASSERT(data_buffer != NULL);
    configASSERT(buffer_length != 0);
    uint32_t offset = 0;

    switch (apb_proxy_get_uint32(data_buffer, &offset)) {
        case APBRIDGE_CODE_AT_RESPONSE: {
            uint32_t length = apb_proxy_get_uint32(data_buffer, &offset);
            uint32_t channel_id = apb_proxy_get_uint32(data_buffer, &offset);
            uint32_t cmd_id = apb_proxy_get_uint32(data_buffer, &offset);
            uint32_t result_code = apb_proxy_get_uint32(data_buffer, &offset);
            uint32_t error_code = apb_proxy_get_uint32(data_buffer, &offset);
            uint32_t tlv_type = apb_proxy_get_uint32(data_buffer, &offset);
            uint32_t tlv_len = apb_proxy_get_uint32(data_buffer, &offset);
            char *p_result_string = data_buffer + offset;
            printf("the channel_id = %d\n\r", channel_id);
            printf("result string: %s\n\r", p_result_string);

            if (channel_id == g_cmd1_channel_id) {
                configASSERT(cmd_id == 565);
                configASSERT(result_code == APBRIDGE_RESULT_OK);
                configASSERT(error_code == 0xFFFFFFFF);
                configASSERT(tlv_type == APBRIDGE_TLV_TYPE_STRING);
                configASSERT(tlv_len == (strlen(p_result_string) + 1));
            } else if (channel_id == g_cmd2_channel_id) {
                configASSERT(cmd_id == 566);
                configASSERT(result_code == APBRIDGE_RESULT_ERROR);
                configASSERT(error_code == 0xFFFFFFFF);
                configASSERT(tlv_type == APBRIDGE_TLV_TYPE_STRING);
                configASSERT(tlv_len == (strlen(p_result_string) + 1));
            } else if (channel_id == g_cmd3_channel_id) {
                if (g_data_mode_actived == false) {
                    configASSERT(cmd_id == 567);
                    configASSERT(result_code == APBRIDGE_RESULT_CONNECT);
                    configASSERT(error_code == 0xFFFFFFFF);
                    configASSERT(tlv_type == APBRIDGE_TLV_TYPE_STRING);
                    configASSERT(tlv_len == (strlen(p_result_string) + 1));
                    g_data_mode_actived = true;
                } else {
                    configASSERT(cmd_id == 567);
                    configASSERT(result_code == APBRIDGE_RESULT_OK);
                    configASSERT(error_code == 0xFFFFFFFF);
                    configASSERT(tlv_type == APBRIDGE_TLV_TYPE_STRING);
                    configASSERT(tlv_len == (strlen(p_result_string) + 1));
                    g_data_mode_actived = false;
                }
            } else {
                configASSERT(0);
            }
            break;
        }
        case APBRIDGE_CODE_USER_DATA: {
            mux_ap_stub_user_data_req(data_buffer, buffer_length);
            break;
        }
        case APBRIDGE_CODE_XON: {
            uint32_t length = apb_proxy_get_uint32(data_buffer, &offset);
            uint32_t channel_id = apb_proxy_get_uint32(data_buffer, &offset);
            configASSERT(12 == length);
            configASSERT(g_cmd3_channel_id == channel_id);
            mux_ap_stub_send_xoff_req();
            mux_ap_stub_send_xon_req();
            break;
        }
        case APBRIDGE_CODE_XOFF: {
            uint32_t length = apb_proxy_get_uint32(data_buffer, &offset);
            uint32_t channel_id = apb_proxy_get_uint32(data_buffer, &offset);
            configASSERT(12 == length);
            configASSERT(g_cmd3_channel_id == channel_id);
            break;
        }
        case APBRIDGE_CODE_REGISTER_COMMAND: {
            mux_ap_stub_at_cmd_register_req(data_buffer, buffer_length);
            mux_ap_stub_at_cmd_register_rsp();
            mux_ap_stub_at_cmd1_req();
            mux_ap_stub_at_cmd2_req();
            mux_ap_stub_at_cmd3_req();
            break;
        }
        default: {
            configASSERT(0);
            break;
        }
    }
}

/********************************************************************************************
 *                                         Unit Test Data End
 ********************************************************************************************/

/**
* @brief                   This function initializes the MUX event task and context. Called by bootup AP only.
* @return                  None.
*/
void mux_ap_init(void)
{
    apb_proxy_log_info("mux_ap_init");
}

/**
 * @brief                   This function sets the MUX event callback for a registered AP.
 * @param[in] channel_type  is the MUX channel type.
 * @param[in] user_callback is the user callback to listen to the MUX event.
 * @param[in] user_data     is the user data and will pass back in #mux_ap_event_channel_enabled_t and #mux_ap_event_channel_disabled_t.
 * @return                  If successful, return #MUX_AP_STATUS_OK, otherwise return an error code defined in #mux_ap_status_t.
 */
mux_ap_status_t mux_ap_register_callback(mux_ap_channel_type_t channel_type, mux_ap_callback_t user_callback, void *user_data)
{
    configASSERT(MUX_AP_CHANNEL_TYPE_AP_BRIDGE == channel_type);
    configASSERT(user_callback != NULL);
    apb_proxy_log_info("mux_ap_register_callback");

    g_ap_bridge_call_back = user_callback;
    mux_ap_event_channel_enabled_t channel_enable_event;
    channel_enable_event.channel_id = g_ap_bridge_channel_id;
    channel_enable_event.user_data = NULL;
    g_ap_bridge_call_back(MUX_AP_EVENT_CHANNEL_ENABLED, &channel_enable_event);
    return MUX_AP_STATUS_OK;
}

/**
 * @brief                   This function changes the MUX event callback for another registered AP.
 * @param[in] channel_id    is the MUX channel ID which is assigned uniquely.
 * @param[in] user_callback is the user callback to listen to the MUX event.
 * @return                  If successful, return #MUX_AP_STATUS_OK, otherwise return an error code defined in #mux_ap_status_t.
 */
mux_ap_status_t mux_ap_change_callback(mux_ap_channel_id_t channel_id, mux_ap_callback_t user_callback)
{
    apb_proxy_log_error("AP Bridge cannot call mux_ap_change_callback");
    configASSERT(0);
    return MUX_AP_STATUS_OK;
}

/**
 * @brief                   This function sends the data on the specified MUX channel.
 * @param[in] channel_id    is the MUX channel ID which is assigned uniquely.
 * @param[in] data_buffer   is the data buffer.
 * @param[in] buffer_length is the buffer length.
 * @param[in] user_data     is the user data (AP defined) and will pass back in #mux_ap_event_send_completed_t.
 * @return                  If successful, return #MUX_AP_STATUS_OK, otherwise return an error code defined in #mux_ap_status_t.
 *                          If successful, AP will also receive #MUX_AP_EVENT_SEND_COMPLETED.
 */
mux_ap_status_t mux_ap_send_data(mux_ap_channel_id_t channel_id, const uint8_t *data_buffer, uint32_t buffer_length, void *user_data)
{
    configASSERT(channel_id == g_ap_bridge_channel_id);
    configASSERT(data_buffer != NULL);
    configASSERT(buffer_length != 0);
    configASSERT(user_data == NULL);
    apb_proxy_log_info("mux_ap_send_data");
    mux_ap_stub_process_packet(data_buffer, buffer_length);
    mux_ap_event_send_completed_t send_completed_event;
    send_completed_event.channel_id = channel_id;
    send_completed_event.data_buffer = data_buffer;
    send_completed_event.user_data = NULL;
    g_ap_bridge_call_back(MUX_AP_EVENT_SEND_COMPLETED, &send_completed_event);
    return MUX_AP_STATUS_OK;
}

/**
 * @brief                   This function stops to receive the data on the specified MUX channel.
 * @note                    When AP can't provide the buffer to receive the data, it immediately calls this API optionally and MUX will buffer 4K user data for this channel.
 *                          AP will also be responsible to call #mux_ap_resume_to_receive() when it detects available memory threshold is reached.
 *                          And MUX will notify AP the buffered user data one by one.
 * @param[in] channel_id    is the MUX channel ID which is assigned uniquely.
 * @return                  If successful, return #MUX_AP_STATUS_OK, otherwise return an error code defined in #mux_ap_status_t.
 * @sa                      mux_ap_resume_to_receive().
 */
mux_ap_status_t mux_ap_stop_to_receive(mux_ap_channel_id_t channel_id)
{
    apb_proxy_log_error("AP Bridge cannot call mux_ap_stop_to_receive");
    configASSERT(0);
    return MUX_AP_STATUS_OK;
}

/**
 * @brief                   This function resumes to receive the data on the specified MUX channel.
 * @param[in] channel_id    is the MUX channel ID which is assigned uniquely.
 * @return                  If successful, return #MUX_AP_STATUS_OK, otherwise return an error code defined in #mux_ap_status_t.
 * @sa                      mux_ap_stop_to_receive().
 */
mux_ap_status_t mux_ap_resume_to_receive(mux_ap_channel_id_t channel_id)
{
    apb_proxy_log_error("AP Bridge cannot call mux_ap_resume_to_receive");
    configASSERT(0);
    return MUX_AP_STATUS_OK;
}

