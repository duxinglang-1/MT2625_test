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
#include "apb_proxy_packet_def.h"
#include "FreeRTOS.h"
#include "apb_proxy_data_type.h"
#include "apb_proxy.h"
#include "apb_proxy_context_manager.h"
#include "apb_proxy_log.h"
#include "string.h"

/******************************************************
 ******************************************************
 ** AP Bridge packet decoder in AP Bridge Proxy.*******
 ******************************************************
 ******************************************************/

typedef union {
    int64_t  integer64bit;
    int32_t  integer32bit;
    uint8_t *string_p;
} apb_proxy_tlv_data_t;

typedef struct {
    uint32_t       type;
    uint32_t       length;
    apb_proxy_tlv_data_t data;
} apb_proxy_tlv_t;

#define LENGTH_OF_NULL 1
/*****************************************************
 ** Local Function Prototype
 *****************************************************/
#if 0
static apb_proxy_status_t apb_proxy_decode_tlv(uint8_t *pdata, apb_proxy_tlv_t *ptlv,
                                   uint32_t *poffset, uint32_t buffer_size);
#endif
/****************************************************
 ** Public Function Implementation
 *****************************************************/

/******************************************************************
 * @brief      Get the AP Bridge packet type.
 * @param[in]  p_apb_proxy_packet points to AP Bridge packet data buffer.
 * @return     Return the AP Bridge packet type.
 ******************************************************************/
uint32_t apb_proxy_get_packet_type(apb_proxy_packet_t *p_apb_proxy_packet)
{
    /*The packet type's code is in the fixed position.*/
    uint32_t offset = 0;
    configASSERT( p_apb_proxy_packet != NULL);
    configASSERT( p_apb_proxy_packet->pdata != NULL);

    if (p_apb_proxy_packet->length < APB_PROXY_MSG_TYPE_FIELD_SIZE) {
        configASSERT(false);
        return 0xFFFFFFFF;
    }
    return apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset);
}
/******************************************************************
 * @brief      Get channel ID.
 * @param[in]  p_apb_proxy_packet points to AP Bridge packet data buffer.
 * @return     Return the channel ID.
 ******************************************************************/
uint32_t apb_proxy_get_channel_id(apb_proxy_packet_t *p_apb_proxy_packet)
{
    /*The packet type's code is in the fixed position.*/
    uint32_t offset = APB_PROXY_CHANNEL_ID_OFFSET;
    configASSERT( p_apb_proxy_packet != NULL);
    configASSERT( p_apb_proxy_packet->pdata != NULL);

    if (p_apb_proxy_packet->length < (APB_PROXY_CHANNEL_ID_OFFSET + APB_PROXY_CHANNEL_ID_FIELD_SIZE)) {
        configASSERT(false);
        return 0xFFFFFFFF;
    }
    return apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset);
}
/*******************************************************************
 * @brief         Decode the AT Command request packet into structure.
 * @param[in]     p_apb_proxy_packet : points to AP Bridge packet data buffer.
 * @param[in/out] p_cmd_req : points to the decoded result.
 * @return        APB_PROXY_STATUS_OK : decode successfully, APB_PROXY_STATUS_ERROR : decode failed.
 *******************************************************************/
apb_proxy_status_t apb_proxy_decode_at_cmd_req_msg(apb_proxy_packet_t *p_apb_proxy_packet,
                                                   apb_proxy_parse_cmd_param_t *p_cmd_req)
{
    uint32_t offset = 0U;
    uint8_t *p_userdata = p_apb_proxy_packet->pdata;
    uint32_t user_data_len = 0U;
    uint32_t command_id = 0U;
    uint32_t channel_id = 0U;
    uint32_t request_type = 0U;
    uint32_t leftsize = 0U;
    uint32_t tlv_type = 0U;
    uint32_t tlv_lenth = 0U;

    configASSERT( p_apb_proxy_packet != NULL);
    configASSERT( p_apb_proxy_packet->pdata != NULL);
    configASSERT( p_cmd_req != NULL);
    if (p_apb_proxy_packet->length < APB_PROXY_AT_CMD_REQ_PACKET_HEADER_SIZE) {
        return APB_PROXY_STATUS_ERROR;
    }
    if (apb_proxy_get_uint32(p_userdata, &offset) != APBRIDGE_CODE_AT_COMMAND) {
        return APB_PROXY_STATUS_ERROR;
    }
    user_data_len = apb_proxy_get_uint32(p_userdata, &offset);
    channel_id = apb_proxy_get_uint32(p_userdata, &offset);
    command_id = apb_proxy_get_uint32(p_userdata, &offset);
    request_type = apb_proxy_get_uint32(p_userdata, &offset);
    if (user_data_len > APB_PROXY_AT_CMD_REQ_PACKET_HEADER_SIZE) {
        /*For At command request packet, it only have one string TLV type.*/
        leftsize = user_data_len - APB_PROXY_AT_CMD_REQ_PACKET_HEADER_SIZE;
        if (leftsize < APB_PROXY_TLV_MIN_SIZE) {
            return APB_PROXY_STATUS_ERROR;
        }
        tlv_type = apb_proxy_get_uint32(p_userdata, &offset);
        leftsize = leftsize - APB_PROXY_TLV_TYPE_FIELD_SIZE;
        tlv_lenth = apb_proxy_get_uint32(p_userdata, &offset);
        leftsize = leftsize - APB_PROXY_TLV_LENGTH_FIELD_SIZE;
        if (leftsize < tlv_lenth) {
            return APB_PROXY_STATUS_ERROR;
        }
        if (tlv_type != APBRIDGE_TLV_TYPE_STRING) {
            return APB_PROXY_STATUS_ERROR;
        }
        p_cmd_req->cmd_id = APB_PROXY_CMD_ID(channel_id, command_id);
        p_cmd_req->mode = (apb_proxy_cmd_mode_t)request_type;
        p_cmd_req->string_ptr = ((char*)p_userdata) + offset;
        p_cmd_req->string_len = tlv_lenth - LENGTH_OF_NULL;
        return APB_PROXY_STATUS_OK;
    } else {
        return APB_PROXY_STATUS_ERROR;
    }
    return APB_PROXY_STATUS_ERROR;
}

/*****************************************************************************
 * @brief         Decode the AT Command register result packet into structure.
 * @param[in]     p_apb_proxy_packet : points to AP Bridge packet data buffer.
 * @param[in/out] p_register_result : points to the decoded result.
 * @return        APB_PROXY_STATUS_OK : decode successfully, APB_PROXY_STATUS_ERROR : decode failed.
 ******************************************************************************/
apb_proxy_status_t apb_proxy_decode_at_cmd_reg_result_msg(apb_proxy_packet_t *p_apb_proxy_packet,
        apb_proxy_register_result_t *p_register_result)
{
    uint32_t offset = 0U;
    configASSERT( p_apb_proxy_packet != NULL);
    configASSERT( p_apb_proxy_packet->pdata != NULL);
    configASSERT( p_apb_proxy_packet->length != 0);
    configASSERT( p_register_result != NULL);

    if (p_apb_proxy_packet->length < APB_PROXY_AT_CMD_REG_RSP_PACKET_SIZE) {
        return APB_PROXY_STATUS_ERROR;
    }

    if (apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset) != APBRIDGE_CODE_COMMAND_REGISTERED) {
        return APB_PROXY_STATUS_ERROR;
    }

    (void)apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset);
    p_register_result->base_cmd_id = apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset);
    p_register_result->registered_count = apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset);
    p_register_result->result = apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset);
    return APB_PROXY_STATUS_OK;
}

/******************************************************************************
 * @brief         Decode the user data packet into structure.
 * @param[in]     p_apb_proxy_packet : points to AP Bridge packet data buffer.
 * @param[in/out] p_userdata : points to the decoded result.
 * @return        APB_PROXY_STATUS_OK : decode successfully, APB_PROXY_STATUS_ERROR : decode failed.
 *****************************************************************************/
apb_proxy_status_t apb_proxy_decode_userdata_msg(apb_proxy_packet_t *p_apb_proxy_packet,
                                                 apb_proxy_user_data_t *p_userdata)
{
    uint32_t length = 0U;
    uint32_t offset = 0U;
    uint32_t channel_id = 0U;
    configASSERT( p_apb_proxy_packet != NULL);
    configASSERT( p_apb_proxy_packet->pdata != NULL);
    configASSERT( p_userdata != NULL );

    if (p_apb_proxy_packet->length < APB_PROXY_USER_DATA_HEADER_SIZE) {
        return APB_PROXY_STATUS_ERROR;
    }

    if (apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset) != APBRIDGE_CODE_USER_DATA) {
        return APB_PROXY_STATUS_ERROR;
    }

    length = apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset);
    if (length != p_apb_proxy_packet->length) {
        return APB_PROXY_STATUS_ERROR;
    }
    channel_id = apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset);
    p_userdata->length = length - APB_PROXY_USER_DATA_HEADER_SIZE;
    p_userdata->pbuffer = p_apb_proxy_packet->pdata + APB_PROXY_USER_DATA_HEADER_SIZE;
    return APB_PROXY_STATUS_OK;
}

/******************************************************************************
 * @brief         Decode the data mode closed indication packet into structure.
 * @param[in]     p_apb_proxy_packet : points to AP Bridge packet data buffer.
 * @param[in/out] p_userdata : points to the decoded result.
 * @return        APB_PROXY_STATUS_OK  : decode successfully, APB_PROXY_STATUS_ERROR  : decode failed.
 *****************************************************************************/
apb_proxy_status_t apb_proxy_decode_data_mode_closed_ind_msg(apb_proxy_packet_t *p_apb_proxy_packet,
                                                             uint32_t* channel_id)
{
    uint32_t length = 0U;
    uint32_t offset = 0U;
    configASSERT( p_apb_proxy_packet != NULL);
    configASSERT( p_apb_proxy_packet->pdata != NULL);
    configASSERT( channel_id != NULL );

    if (p_apb_proxy_packet->length != APB_PROXY_DATA_MODE_CLOSED_IND_PACKET_SIZE) {
        return APB_PROXY_STATUS_ERROR;
    }

    if (apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset) != APBRIDGE_CODE_DATA_MODE_CLOSED_IND) {
        return APB_PROXY_STATUS_ERROR;
    }

    length = apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset);
    if (length != p_apb_proxy_packet->length) {
        return APB_PROXY_STATUS_ERROR;
    }

    *channel_id = apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset);
    return APB_PROXY_STATUS_OK;
}
/******************************************************************************
 * @brief         Decode the data mode deactive indication packet into structure.
 * @param[in]     p_apb_proxy_packet : points to AP Bridge packet data buffer.
 * @param[in/out] p_userdata : points to the decoded result.
 * @return        APB_PROXY_STATUS_OK  : decode successfully, APB_PROXY_STATUS_ERROR  : decode failed.
 *****************************************************************************/
apb_proxy_status_t apb_proxy_decode_data_mode_temp_deactive_ind_msg(apb_proxy_packet_t *p_apb_proxy_packet,
                                                                    uint32_t* channel_id)
{
    uint32_t length = 0U;
    uint32_t offset = 0U;
    configASSERT( p_apb_proxy_packet != NULL);
    configASSERT( p_apb_proxy_packet->pdata != NULL);
    configASSERT( channel_id != NULL );

    if (p_apb_proxy_packet->length != APB_PROXY_DATA_MODE_TEMP_DEACTIVE_IND_PACKET_SIZE) {
        return APB_PROXY_STATUS_ERROR;
    }

    if (apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset) != APBRIDGE_CODE_DATA_MODE_TEMP_DEACTIVE_IND) {
        return APB_PROXY_STATUS_ERROR;
    }

    length = apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset);
    if (length != p_apb_proxy_packet->length) {
        return APB_PROXY_STATUS_ERROR;
    }

    *channel_id = apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset);
    return APB_PROXY_STATUS_OK;
}

/******************************************************************************
 * @brief         Decode the data mode resumed indication packet into structure.
 * @param[in]     p_apb_proxy_packet : points to AP Bridge packet data buffer.
 * @param[in/out] p_userdata : points to the decoded result.
 * @return        APB_PROXY_STATUS_OK  : decode successfully, APB_PROXY_STATUS_ERROR  : decode failed.
 *****************************************************************************/
apb_proxy_status_t apb_proxy_decode_data_mode_resumed_ind_msg(apb_proxy_packet_t *p_apb_proxy_packet,
                                                              uint32_t* channel_id)
{
    uint32_t length = 0U;
    uint32_t offset = 0U;
    configASSERT( p_apb_proxy_packet != NULL);
    configASSERT( p_apb_proxy_packet->pdata != NULL);
    configASSERT( channel_id != NULL );

    if (p_apb_proxy_packet->length != APB_PROXY_DATA_MODE_RESUMED_IND_PACKET_SIZE) {
        return APB_PROXY_STATUS_ERROR;
    }

    if (apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset) != APBRIDGE_CODE_DATA_MODE_RESUMED_IND) {
        return APB_PROXY_STATUS_ERROR;
    }

    length = apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset);
    if (length != p_apb_proxy_packet->length) {
        return APB_PROXY_STATUS_ERROR;
    }

    *channel_id = apb_proxy_get_uint32(p_apb_proxy_packet->pdata, &offset);
    return APB_PROXY_STATUS_OK;
}
/****************************************************
** Local Function Implementation
*****************************************************/
/*bufferSize : the available buffer which can be used to decode the TLV.*/
#if 0
static apb_proxy_status_t apb_proxy_decode_tlv(uint8_t *pdata, apb_proxy_tlv_t *ptlv,
                                   uint32_t *poffset, uint32_t buffer_size)
{
    configASSERT(pdata != NULL);
    configASSERT(ptlv != NULL);
    configASSERT(poffset != NULL);
    uint32_t leftsize = buffer_size;

    if (leftsize < APB_PROXY_TLV_MIN_SIZE) {
        return APB_PROXY_STATUS_ERROR;
    }

    ptlv->type = apb_proxy_get_uint32(pdata, poffset);
    leftsize = leftsize - APB_PROXY_TLV_TYPE_FIELD_SIZE;
    ptlv->length = apb_proxy_get_uint32(pdata, poffset);
    leftsize = leftsize - APB_PROXY_TLV_LENGTH_FIELD_SIZE;
    if (leftsize < ptlv->length) {
        return APB_PROXY_STATUS_ERROR;
    }

    switch (ptlv->type) {
        case APBRIDGE_TLV_TYPE_INTEGER: {
            if (ptlv->length == sizeof(int32_t)) {
                ptlv->data.integer32bit = apb_proxy_get_int32(pdata, poffset);
            } else if (ptlv->length == sizeof(int64_t)) {
                ptlv->data.integer64bit = apb_proxy_get_int64(pdata, poffset);
            } else {
                return APB_PROXY_STATUS_ERROR;
            }
            return APB_PROXY_STATUS_OK;
            break;
        }
        case APBRIDGE_TLV_TYPE_STRING: {
            ptlv->data.string_p = (uint8_t *)pvPortMalloc(ptlv->length);
            if (NULL == ptlv->data.string_p) {
                return APB_PROXY_STATUS_ERROR;
            }
            memcpy(ptlv->data.string_p, pdata, ptlv->length);
            *poffset = *poffset + ptlv->length;
            /*memory alignment*/
            *poffset = *poffset + 3U - ((*poffset + 3U) % sizeof(uint32_t));
            return APB_PROXY_STATUS_OK;
            break;
        }
        case APBRIDGE_TLV_TYPE_VOID:
        default: {
            apb_proxy_log_info("invalid AP Bridge packet type.");
            break;
        }
    }

    return APB_PROXY_STATUS_ERROR;
}
#endif

