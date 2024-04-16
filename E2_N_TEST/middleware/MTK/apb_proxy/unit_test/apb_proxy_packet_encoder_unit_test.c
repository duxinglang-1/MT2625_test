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

#include "apb_proxy_packet_encoder_unit_test.h"
#include "apb_proxy_packet_encoder.h"
#include "stdlib.h"
#include "apb_proxy_utility.h"
#include "FreeRTOS.h"
#include "apb_proxy_packet_def.h"

bool apb_proxy_encode_at_cmd_reg_packet_unit_test(void)
{
    apb_proxy_cmd_hdlr_table_t *p_cmd_tbl = apb_proxy_get_cmd_hdlr_table_info();
    apb_proxy_packet_t apb_proxy_packet;
    uint32_t index = 0;

    uint8_t expected_data[] = {
        /*packet type*/
        0x06, 0x00, 0x00, 0x00,
        /*packet length*/
        0x2E, 0x00, 0x00, 0x00,
        /*AT Command total counts in AP Bridge Proxy*/
        0x03, 0x00, 0x00, 0x00,
        /*AT Command Count*/
        0x03, 0x00, 0x00, 0x00,
        /*AT Command data*/
        0x2B, 0x54, 0x45, 0x53,
        0x54, 0x43, 0x4D, 0x44,
        0x31, 0x00, 0x2B, 0x54,
        0x45, 0x53, 0x54, 0x43,
        0x4D, 0x44, 0x32, 0x00,
        0x2B, 0x54, 0x45, 0x53,
        0x54, 0x43, 0x4D, 0x44,
        0x33, 0x00
    };

    configASSERT(p_cmd_tbl->item_size == apb_proxy_encode_at_cmd_reg_packet(p_cmd_tbl->p_item_tbl,
                 p_cmd_tbl->item_size, &apb_proxy_packet));
    configASSERT(apb_proxy_packet.length == sizeof(expected_data));
    configASSERT(apb_proxy_packet.pdata != NULL);
    for (index = 0; index < apb_proxy_packet.length; index++) {
        configASSERT(expected_data[index] == (*(apb_proxy_packet.pdata + index)));
    }
    vPortFree(apb_proxy_packet.pdata);
    apb_proxy_packet.pdata = NULL;

    return true;
}
bool apb_proxy_encode_at_cmd_result_packet_unit_test(void)
{
    apb_proxy_packet_t apb_proxy_packet;
    apb_proxy_at_cmd_result_t cmd_result;
    uint32_t index = 0;
    cmd_result.result_code = APB_PROXY_RESULT_OK;
    cmd_result.pdata = "OK";
    cmd_result.length = strlen("OK") + 1;
    cmd_result.cmd_id = 565;
    uint8_t expected_data[] = {
        /*packet type*/
        0x02, 0x00, 0x00, 0x00,
        /*packet length*/
        0x23, 0x00, 0x00, 0x00,
        /*channel id*/
        0xFF, 0xFF, 0xFF, 0xFF,
        /*command id*/
        0x35, 0x02, 0x00, 0x00,
        /*result code*/
        0x01, 0x00, 0x00, 0x00,
        /*error code*/
        0xFF, 0xFF, 0xFF, 0xFF,
        /*TLV type*/
        0x02, 0x00, 0x00, 0x00,
        /*TLV length*/
        0x03, 0x00, 0x00, 0x00,
        /*TLV value*/
        0x4F, 0x4B, 0x00
    };

    configASSERT(APB_PROXY_STATUS_OK == apb_proxy_encode_at_cmd_result_packet(&cmd_result, &apb_proxy_packet));
    configASSERT(apb_proxy_packet.length == sizeof(expected_data));
    configASSERT(apb_proxy_packet.pdata != NULL);
    for (index = 0; index < apb_proxy_packet.length; index++) {
        configASSERT(expected_data[index] == (*(apb_proxy_packet.pdata + index)));
    }
    vPortFree(apb_proxy_packet.pdata);
    apb_proxy_packet.pdata = NULL;

    return true;
}
bool apb_proxy_encode_user_data_packet_unit_test(void)
{
    apb_proxy_packet_t apb_proxy_packet;
    apb_proxy_user_data_t input_user_data;
    uint32_t index = 0;
    uint8_t user_data[] = {
        0x01, 0x22, 0x99, 0x23,
        0x13, 0x0A, 0x33, 0x00,
        0x10, 0x00, 0x97, 0x00,
        0x00
    };
    uint8_t expected_data[] = {
        /*packet type*/
        0x03, 0x00, 0x00, 0x00,
        /*packet length*/
        0x19, 0x00, 0x00, 0x00,
        /*channel id*/
        0xFF, 0xFF, 0xFF, 0xFF,
        /*user data*/
        0x01, 0x22, 0x99, 0x23,
        0x13, 0x0A, 0x33, 0x00,
        0x10, 0x00, 0x97, 0x00,
        0x00
    };

    input_user_data.length = sizeof(user_data);
    input_user_data.pbuffer = user_data;
    configASSERT(APB_PROXY_STATUS_OK == apb_proxy_encode_user_data_packet(&input_user_data, &apb_proxy_packet));
    configASSERT(apb_proxy_packet.length == sizeof(expected_data));
    configASSERT(apb_proxy_packet.pdata != NULL);
    for (index = 0; index < apb_proxy_packet.length; index++) {
        configASSERT(expected_data[index] == (*(apb_proxy_packet.pdata + index)));
    }
    vPortFree(apb_proxy_packet.pdata);
    apb_proxy_packet.pdata = NULL;
    return true;
}
bool apb_proxy_encode_xon_packet_unit_test(void)
{
    apb_proxy_packet_t apb_proxy_packet;
    uint32_t index = 0;
    uint8_t apb_proxy_xon_packet[APB_PROXY_XON_PACKET_SIZE] = {
        0x04, 0x00, 0x00, 0x00,
        0x0C, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF
    };
    configASSERT(APB_PROXY_STATUS_OK == apb_proxy_encode_xon_packet(&apb_proxy_packet));
    configASSERT(apb_proxy_packet.length == sizeof(apb_proxy_xon_packet));
    configASSERT(apb_proxy_packet.pdata != NULL);
    for (index = 0; index < apb_proxy_packet.length; index++) {
        configASSERT(apb_proxy_xon_packet[index] == (*(apb_proxy_packet.pdata + index)));
    }
    vPortFree(apb_proxy_packet.pdata);
    apb_proxy_packet.pdata = NULL;
    return true;
}
bool apb_proxy_encode_xoff_packet_unit_test(void)
{
    apb_proxy_packet_t apb_proxy_packet;
    uint32_t index = 0;
    uint8_t apb_proxy_xoff_packet[APB_PROXY_XON_PACKET_SIZE] = {
        0x05, 0x00, 0x00, 0x00,
        0x0C, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF
    };
    configASSERT(APB_PROXY_STATUS_OK == apb_proxy_encode_xoff_packet(&apb_proxy_packet));
    configASSERT(apb_proxy_packet.length == sizeof(apb_proxy_xoff_packet));
    configASSERT(apb_proxy_packet.pdata != NULL);
    for (index = 0; index < apb_proxy_packet.length; index++) {
        configASSERT(apb_proxy_xoff_packet[index] == (*(apb_proxy_packet.pdata + index)));
    }
    vPortFree(apb_proxy_packet.pdata);
    apb_proxy_packet.pdata = NULL;
    return true;
}
bool apb_proxy_fill_channel_id_to_packet_unit_test(void)
{
    /*at cmd response AP Bridge packet.*/
    const uint32_t channel_id = 12;
    apb_proxy_packet_t apb_proxy_packet;
    uint32_t *ptemp = NULL;
    uint8_t test_data[] = {
        /*packet type*/
        0x02, 0x00, 0x00, 0x00,
        /*packet length*/
        0x23, 0x00, 0x00, 0x00,
        /*channel id*/
        0xFF, 0xFF, 0xFF, 0xFF,
        /*command id*/
        0x35, 0x02, 0x00, 0x00,
        /*result code*/
        0x01, 0x00, 0x00, 0x00,
        /*error code*/
        0xFF, 0xFF, 0xFF, 0xFF,
        /*TLV type*/
        0x02, 0x00, 0x00, 0x00,
        /*TLV length*/
        0x03, 0x00, 0x00, 0x00,
        /*TLV value*/
        0x4F, 0x4B, 0x00
    };

    apb_proxy_packet.pdata = test_data;
    apb_proxy_packet.length = sizeof(test_data);
    configASSERT(apb_proxy_fill_channel_id_to_packet(&apb_proxy_packet, channel_id) == APB_PROXY_STATUS_OK);
    ptemp = (uint32_t *)(apb_proxy_packet.pdata + APB_PROXY_AT_CMD_RSP_CHANNEL_ID_OFFSET);
    configASSERT((*ptemp) == channel_id);
    return true;
}
bool apb_proxy_get_at_cmd_result_code_unit_test(void)
{
    apb_proxy_packet_t apb_proxy_packet;
    uint8_t test_data[] = {
        /*packet type*/
        0x02, 0x00, 0x00, 0x00,
        /*packet length*/
        0x23, 0x00, 0x00, 0x00,
        /*channel id*/
        0xFF, 0xFF, 0xFF, 0xFF,
        /*command id*/
        0x35, 0x02, 0x00, 0x00,
        /*result code*/
        0x01, 0x00, 0x00, 0x00,
        /*error code*/
        0xFF, 0xFF, 0xFF, 0xFF,
        /*TLV type*/
        0x02, 0x00, 0x00, 0x00,
        /*TLV length*/
        0x03, 0x00, 0x00, 0x00,
        /*TLV value*/
        0x4F, 0x4B, 0x00
    };
    apb_proxy_packet.pdata = test_data;
    apb_proxy_packet.length = sizeof(test_data);
    configASSERT(apb_proxy_get_at_cmd_result_code(&apb_proxy_packet) == 1);
    return true;
}

