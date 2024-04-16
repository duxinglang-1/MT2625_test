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

#include "apb_proxy_utility.h"
#include "apb_proxy_packet_encoder.h"
#include "apb_proxy_packet_def.h"
#include "FreeRTOS.h"
#include "apb_proxy_log.h"
#include "string.h"
#include "apb_proxy_packet_decoder.h"

/*************************************************************************************************
*                             Local Function's protype                                           *
**************************************************************************************************/
uint32_t apb_proxy_encode_get_error_code(apb_proxy_at_cmd_result_code_t result_code);
/*************************************************************************************************
*                             Public Function's Implementation                                   *
**************************************************************************************************/
/**************************************************************************
 * @brief         Encode AT command register message into AP Bridge packet.
 * @param[in]     p_cmd_item_tbl : points to AT command handler table.
 * @param[in]     item_count : AT command item count.
 * @param[in/out] p_packet_data : encoded AP Bridge packet.
 * @return        how many at cmd's names are been encoded into the packet.
 **************************************************************************/
uint32_t apb_proxy_encode_at_cmd_reg_packet(apb_proxy_cmd_hdlr_item_t *p_cmd_item_tbl,
                                            uint32_t item_count, apb_proxy_packet_t *p_packet_data)
{
    uint32_t offset = 0;
    uint32_t index = 0;
    apb_proxy_cmd_hdlr_item_t *p_cmd_item;
    /*The lenght of "AT" string.*/
    const uint32_t at_cmd_common_header_size = 2;
    uint32_t at_cmd_size = 0;
    uint32_t encoded_cmd_count = 0;
    apb_proxy_cmd_hdlr_table_t *p_cmd_tbl = apb_proxy_get_cmd_hdlr_table_info();
    configASSERT(p_cmd_item_tbl != NULL);
    configASSERT(p_packet_data != NULL);
    configASSERT(item_count != 0);

    /*step1: calculate the total needed length.*/
    p_packet_data->length = 0U;
    p_packet_data->length = p_packet_data->length + APB_PROXY_AT_CMD_REG_HEADER_SIZE;

    for (index = 0; index < item_count; index++) {
        p_cmd_item = p_cmd_item_tbl + index;
        at_cmd_size = strlen(p_cmd_item->p_cmd_head) + 1 - at_cmd_common_header_size;
        if ((p_packet_data->length + at_cmd_size) < AP_BRIDGE_PACKET_MAX_SIZE) {
            p_packet_data->length = p_packet_data->length + at_cmd_size;
        } else {
            /*The packet has reached the max count of the packet.*/
            break;
        }
    }

    encoded_cmd_count = index;
    if (0 == encoded_cmd_count) {
        return 0;
    }

    /*step2: encode the AP Bridge packet data.*/
    p_packet_data->pdata = (uint8_t *)pvPortMalloc(p_packet_data->length);

    if (NULL == p_packet_data->pdata) {
        return 0;
    }

    apb_proxy_set_uint32(p_packet_data->pdata, &offset, APBRIDGE_CODE_REGISTER_COMMAND, p_packet_data->length);
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, p_packet_data->length, p_packet_data->length);
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, p_cmd_tbl->item_size, p_packet_data->length);
    /*set the cmd count of this packet.*/
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, encoded_cmd_count, p_packet_data->length);

    /*add the at cmd string into the packet.*/
    for (index = 0; index < encoded_cmd_count; index ++) {
        p_cmd_item = p_cmd_item_tbl + index;
        at_cmd_size = strlen(p_cmd_item->p_cmd_head) + 1 - at_cmd_common_header_size;
        memcpy(p_packet_data->pdata + offset, (p_cmd_item->p_cmd_head + at_cmd_common_header_size)
               , at_cmd_size);
        offset = offset + at_cmd_size;
    }

    return encoded_cmd_count;
}

/*************************************************************************
 * @brief         Encode AT command results into AP Bridge packet.
 * @param[in]     pResult : points to AT command results.
 * @param[in/out] p_packet_data : encoded AP Bridge packet.
 * @return        APB_PROXY_STATUS_OK : encoded successfully, APB_PROXY_STATUS_ERROR : encoded failed.
 *************************************************************************/
apb_proxy_status_t apb_proxy_encode_at_cmd_result_packet(apb_proxy_at_cmd_result_t *presult,
                                                         apb_proxy_packet_t *p_packet_data)
{
    uint32_t offset = 0;
    configASSERT(presult != NULL);
    configASSERT(p_packet_data != NULL);

    /*step1: calculate the total needed length.*/
    p_packet_data->length = 0U;
    p_packet_data->length = p_packet_data->length + APB_PROXY_AT_CMD_RSP_HEADER_SIZE;

    if ((presult->length != 0U) && (presult->pdata != NULL)) {
        p_packet_data->length = p_packet_data->length + APB_PROXY_TLV_TYPE_FIELD_SIZE;
        p_packet_data->length = p_packet_data->length + APB_PROXY_TLV_LENGTH_FIELD_SIZE;
        p_packet_data->length = p_packet_data->length + presult->length;
    }

    /*step2: encode the AP Bridge packet data.*/
    p_packet_data->pdata = (uint8_t *)pvPortMalloc(p_packet_data->length);

    if (NULL == p_packet_data->pdata) {
        return APB_PROXY_STATUS_ERROR;
    }

    apb_proxy_set_uint32(p_packet_data->pdata, &offset, APBRIDGE_CODE_AT_RESPONSE, p_packet_data->length);
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, p_packet_data->length, p_packet_data->length);
    if (presult->cmd_id == APB_PROXY_INVALID_CMD_ID){
        apb_proxy_set_uint32(p_packet_data->pdata, &offset, 0xFFFFFFFF, p_packet_data->length);
        apb_proxy_set_uint32(p_packet_data->pdata, &offset, 0xFFFFFFFF, p_packet_data->length);
    }else{
        apb_proxy_set_uint32(p_packet_data->pdata, &offset, APB_PROXY_GET_CHANNEL_ID(presult->cmd_id), p_packet_data->length);
        apb_proxy_set_uint32(p_packet_data->pdata, &offset, APB_PROXY_GET_CMD_ID_IN_MODEM(presult->cmd_id), p_packet_data->length);
    }
    if (presult->result_code <= APB_PROXY_RESULT_CUSTOM_ERROR){
        apb_proxy_set_uint32(p_packet_data->pdata, &offset, (uint32_t)presult->result_code, p_packet_data->length);
        apb_proxy_set_uint32(p_packet_data->pdata, &offset, APBRIDGE_ERROR_NOT_USED, p_packet_data->length);
    }else{
        apb_proxy_set_uint32(p_packet_data->pdata, &offset, APBRIDGE_RESULT_ERROR, p_packet_data->length);
        apb_proxy_set_uint32(p_packet_data->pdata, &offset,
                             apb_proxy_encode_get_error_code(presult->result_code),
                             p_packet_data->length);
    }
    if ((presult->length != 0U) && (presult->pdata != NULL)) {
        apb_proxy_set_uint32(p_packet_data->pdata, &offset, APBRIDGE_TLV_TYPE_STRING , p_packet_data->length);
        apb_proxy_set_uint32(p_packet_data->pdata, &offset, presult->length, p_packet_data->length);
        memcpy(p_packet_data->pdata + offset, presult->pdata, presult->length);
    }
    return APB_PROXY_STATUS_OK;

}
/*************************************************************************
 * @brief         Encode user data into AP Bridge packet.
 * @param[in]     puser_data : points to user data structure.
 * @param[in]     channel_id : channel ID.
 * @param[in/out] p_packet_data : encoded AP Bridge packet.
 * @return        APB_PROXY_STATUS_OK : encoded successfully, APB_PROXY_STATUS_ERROR : encoded failed.
 *************************************************************************/
apb_proxy_status_t apb_proxy_encode_user_data_packet(apb_proxy_user_data_t *puser_data,
                                                     uint32_t channel_id,
                                                     apb_proxy_packet_t *p_packet_data)
{
    uint32_t offset = 0;
    configASSERT(puser_data != NULL);
    configASSERT(p_packet_data != NULL);

    /*step1: calculate the total needed length.*/
    p_packet_data->length = 0U;
    p_packet_data->length = p_packet_data->length + APB_PROXY_USER_DATA_HEADER_SIZE;
    p_packet_data->length = p_packet_data->length + puser_data->length;

    /*step2: encode the AP Bridge packet data.*/
    p_packet_data->pdata = (uint8_t *)pvPortMalloc(p_packet_data->length);
    if (NULL == p_packet_data->pdata) {
        return APB_PROXY_STATUS_ERROR;
    }
    p_packet_data->channel_id = channel_id;
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, APBRIDGE_CODE_USER_DATA, APB_PROXY_USER_DATA_HEADER_SIZE);
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, p_packet_data->length, APB_PROXY_USER_DATA_HEADER_SIZE);
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, channel_id, APB_PROXY_USER_DATA_HEADER_SIZE);
    memcpy(p_packet_data->pdata + offset, puser_data->pbuffer, puser_data->length);
    return APB_PROXY_STATUS_OK;

}
/*************************************************************************************************
 * @brief         Encode Xon AP Bridge packet.
 * @param[in]     channel_id    : channel ID
 * @param[in/out] p_packet_data : encoded AP Bridge packet.
 * @return        APB_PROXY_STATUS_OK : encoded successfully, APB_PROXY_STATUS_ERROR : encoded failed.
 *************************************************************************************************/
apb_proxy_status_t apb_proxy_encode_xon_packet(uint32_t channel_id, apb_proxy_packet_t *p_packet_data)
{
    uint32_t offset = 0;
    configASSERT(p_packet_data != NULL);

    p_packet_data->length = APB_PROXY_XON_PACKET_SIZE;
    p_packet_data->pdata = (uint8_t *)pvPortMalloc(p_packet_data->length);

    if (NULL == p_packet_data->pdata) {
        return APB_PROXY_STATUS_ERROR;
    }
    memset(p_packet_data->pdata, 0 , p_packet_data->length);
    p_packet_data->channel_id = channel_id;
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, APBRIDGE_CODE_XON, p_packet_data->length);
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, p_packet_data->length, p_packet_data->length);
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, channel_id, p_packet_data->length);
    return APB_PROXY_STATUS_OK;
}
/*************************************************************************
 * @brief          Encode Xoff AP Bridge packet.
 * @param[in]      channel_id    : channel ID
 * @param[in/out]  p_packet_data : encoded AP Bridge packet.
 * @return         APB_PROXY_STATUS_OK : encoded successfully, APB_PROXY_STATUS_ERROR : encoded failed.
 *************************************************************************/
apb_proxy_status_t apb_proxy_encode_xoff_packet(uint32_t channel_id, apb_proxy_packet_t *p_packet_data)
{
    uint32_t offset = 0;
    configASSERT(p_packet_data != NULL);

    p_packet_data->length = APB_PROXY_XOFF_PACKET_SIZE;
    p_packet_data->pdata = (uint8_t *)pvPortMalloc(p_packet_data->length);

    if (NULL == p_packet_data->pdata) {
        return APB_PROXY_STATUS_ERROR;
    }
    p_packet_data->channel_id = channel_id;
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, APBRIDGE_CODE_XOFF, p_packet_data->length);
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, p_packet_data->length, p_packet_data->length);
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, channel_id, p_packet_data->length);

    return APB_PROXY_STATUS_OK;
}
/*************************************************************************
 * @brief          Encode close data mode request AP Bridge packet.
 * @param[in]      channel_id    : channel ID
 * @param[in/out]  p_packet_data : encoded AP Bridge packet.
 * @return         APB_PROXY_STATUS_OK : encoded successfully, APB_PROXY_STATUS_ERROR : encoded failed.
 *************************************************************************/
apb_proxy_status_t apb_proxy_encode_close_data_mode_req_packet(uint32_t channel_id, apb_proxy_packet_t *p_packet_data)
{
    uint32_t offset = 0;
    configASSERT(p_packet_data != NULL);

    p_packet_data->length = APB_PROXY_DATA_MODE_CLOSE_REQ_PACKET_SIZE;
    p_packet_data->pdata = (uint8_t *)pvPortMalloc(p_packet_data->length);

    if (NULL == p_packet_data->pdata) {
        return APB_PROXY_STATUS_ERROR;
    }
    p_packet_data->channel_id = channel_id;
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, APBRIDGE_CODE_DATA_MODE_CLOSE_REQ, p_packet_data->length);
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, p_packet_data->length, p_packet_data->length);
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, channel_id, p_packet_data->length);

    return APB_PROXY_STATUS_OK;
}
/*************************************************************************
 * @brief          Encode resume data mode request AP Bridge packet.
 * @param[in]      channel_id    : channel ID
 * @param[in/out]  p_packet_data : encoded AP Bridge packet.
 * @return         APB_PROXY_STATUS_OK : encoded successfully, APB_PROXY_STATUS_ERROR : encoded failed.
 *************************************************************************/
apb_proxy_status_t apb_proxy_encode_resume_data_mode_req_packet(uint32_t channel_id, apb_proxy_packet_t *p_packet_data)
{
    uint32_t offset = 0;
    configASSERT(p_packet_data != NULL);

    p_packet_data->length = APB_PROXY_DATA_MODE_RESUME_REQ_PACKET_SIZE;
    p_packet_data->pdata = (uint8_t *)pvPortMalloc(p_packet_data->length);

    if (NULL == p_packet_data->pdata) {
        return APB_PROXY_STATUS_ERROR;
    }
    p_packet_data->channel_id = channel_id;
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, APBRIDGE_CODE_DATA_MODE_RESUME_REQ, p_packet_data->length);
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, p_packet_data->length, p_packet_data->length);
    apb_proxy_set_uint32(p_packet_data->pdata, &offset, channel_id, p_packet_data->length);

    return APB_PROXY_STATUS_OK;
}
/*************************************************************************
 * @brief          update the channel Id in AP Bridge packet.
 * @param[in]      channel_id : the channel id for the AT command or user data
 * @param[in/out]  p_packet_data : encoded AP Bridge packet.
 * @return         APB_PROXY_STATUS_OK : encoded successfully, APB_PROXY_STATUS_ERROR : encoded failed.
 **************************************************************************/
apb_proxy_status_t apb_proxy_fill_channel_id_to_packet(apb_proxy_packet_t *p_packet_data, uint32_t channel_id)
{
    uint32_t offset = 0U;
    configASSERT(p_packet_data != NULL);
    configASSERT(p_packet_data->pdata != NULL);
    configASSERT(p_packet_data->length != 0U);

    switch (apb_proxy_get_packet_type(p_packet_data)) {
        case APBRIDGE_CODE_AT_RESPONSE: {
            offset = APB_PROXY_AT_CMD_RSP_CHANNEL_ID_OFFSET;
            break;
        }
        case APBRIDGE_CODE_USER_DATA: {
            offset = APB_PROXY_USER_DATA_CHANNEL_ID_OFFSET;
            break;
        }
        case APBRIDGE_CODE_XON: {
            offset = APB_PROXY_XON_CHANNEL_ID_OFFSET;
            break;
        }
        case APBRIDGE_CODE_XOFF: {
            offset = APB_PROXY_XON_CHANNEL_ID_OFFSET;
            break;
        }
        default: {
            apb_proxy_log_error("wrong packet type!");
            configASSERT(0);
            return APB_PROXY_STATUS_ERROR;
        }

    }

    apb_proxy_set_uint32(p_packet_data->pdata, &offset, channel_id, p_packet_data->length);
    return APB_PROXY_STATUS_OK;

}
/**************************************************************************
 * @brief          read AT command result code from AP Bridge packet.
 * @param[in/out]  p_packet_data : AP Bridge packet.
 * @return         The AT command's result code.
 **************************************************************************/
uint32_t apb_proxy_get_at_cmd_result_code(apb_proxy_packet_t *p_packet_data)
{
    configASSERT(p_packet_data != NULL);
    configASSERT(p_packet_data->pdata != NULL);
    configASSERT(p_packet_data->length != 0U);
    uint32_t offset = APB_PROXY_AT_CMD_RSP_RESULT_OFFSET;
    if (apb_proxy_get_packet_type(p_packet_data) == APBRIDGE_CODE_AT_RESPONSE) {
        return apb_proxy_get_uint32(p_packet_data->pdata, &offset);
    } else {
        return 0xFFFFFFFF;
    }
}
/*************************************************************************************************
*                             Local Function's Implementation                                    *
**************************************************************************************************/
uint32_t apb_proxy_encode_get_error_code(apb_proxy_at_cmd_result_code_t result_code)
{
    uint32_t error_code = APBRIDGE_ERROR_NOT_USED;
    switch(result_code){
        case APB_PROXY_ERROR_CME_PHONE_FAILURE:{
            error_code = APBRIDGE_ERROR_CME_PHONE_FAILURE;
            break;
        }
        case APB_PROXY_ERROR_CME_OPERATION_NOT_ALLOWED:{
            error_code = APBRIDGE_ERROR_CME_OPERATION_NOT_ALLOWED;
            break;
        }
        case APB_PROXY_ERROR_CME_OPERATION_NOT_SUPPORTED:{
            error_code = APBRIDGE_ERROR_CME_OPERATION_NOT_SUPPORTED;
            break;
        }
        case APB_PROXY_ERROR_CME_SIM_NOT_INSERTED:{
            error_code = APBRIDGE_ERROR_CME_SIM_NOT_INSERTED;
            break;
        }
        case APB_PROXY_ERROR_CME_INCORRECT_PASSWORD:{
            error_code = APBRIDGE_ERROR_CME_INCORRECT_PASSWORD;
            break;
        }
        case APB_PROXY_ERROR_CME_MEMORY_FULL:{
            error_code = APBRIDGE_ERROR_CME_MEMORY_FULL;
            break;
        }
        case APB_PROXY_ERROR_CME_MEMORY_FAILURE:{
            error_code = APBRIDGE_ERROR_CME_MEMORY_FAILURE;
            break;
        }
        case APB_PROXY_ERROR_CME_LONG_TEXT:{
            error_code = APBRIDGE_ERROR_CME_LONG_TEXT;
            break;
        }
        case APB_PROXY_ERROR_CME_INVALID_TEXT_CHARS:{
            error_code = APBRIDGE_ERROR_CME_INVALID_TEXT_CHARS;
            break;
        }
        case APB_PROXY_ERROR_CME_NO_NETWORK_SERVICE:{
            error_code = APBRIDGE_ERROR_CME_NO_NETWORK_SERVICE;
            break;
        }
        case APB_PROXY_ERROR_CME_NETWORK_TIMEOUT:{
            error_code = APBRIDGE_ERROR_CME_NETWORK_TIMEOUT;
            break;
        }
        case APB_PROXY_ERROR_CME_EMERGENCY_ONLY:{
            error_code = APBRIDGE_ERROR_CME_EMERGENCY_ONLY;
            break;
        }
        case APB_PROXY_ERROR_CME_UNKNOWN:{
            error_code = APBRIDGE_ERROR_CME_UNKNOWN;
            break;
        }
        case APB_PROXY_ERROR_CME_PSD_SERVICES_NOT_ALLOWED:{
            error_code = APBRIDGE_ERROR_CME_PSD_SERVICES_NOT_ALLOWED;
            break;
        }
        case APB_PROXY_ERROR_CME_PLMN_NOT_ALLOWED:{
            error_code = APBRIDGE_ERROR_CME_PLMN_NOT_ALLOWED;
            break;
        }
        case APB_PROXY_ERROR_CME_LOCATION_AREA_NOT_ALLOWED:{
            error_code = APBRIDGE_ERROR_CME_LOCATION_AREA_NOT_ALLOWED;
            break;
        }
        case APB_PROXY_ERROR_CME_ROAMING_NOT_ALLOWED:{
            error_code = APBRIDGE_ERROR_CME_ROAMING_NOT_ALLOWED;
            break;
        }
        case APB_PROXY_ERROR_CME_SERVICE_OPTION_NOT_SUPPORTED:{
            error_code = APBRIDGE_ERROR_CME_SERVICE_OPTION_NOT_SUPPORTED;
            break;
        }
        case APB_PROXY_ERROR_CME_SERVICE_OPTION_NOT_SUBSCRIBED:{
            error_code = APBRIDGE_ERROR_CME_SERVICE_OPTION_NOT_SUBSCRIBED;
            break;
        }
        case APB_PROXY_ERROR_CME_SERVICE_OPTION_OUT_OF_ORDER:{
            error_code = APBRIDGE_ERROR_CME_SERVICE_OPTION_OUT_OF_ORDER;
            break;
        }
        case APB_PROXY_ERROR_CME_UNSPECIFIED_PSD_ERROR:{
            error_code = APBRIDGE_ERROR_CME_UNSPECIFIED_PSD_ERROR;
            break;
        }
        case APB_PROXY_ERROR_CME_PDP_AUTHENTIFICATION_ERROR:{
            error_code = APBRIDGE_ERROR_CME_PDP_AUTHENTIFICATION_ERROR;
            break;
        }
        default:{
            break;
        }
    }
    return error_code;
}
