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

#include "apb_proxy_event_handler.h"
#include "apb_proxy_at_cmd_tbl.h"
#include "FreeRTOS.h"
#include "apb_proxy.h"
#include "apb_proxy_msg_queue_def.h"
#include "apb_proxy_context_manager.h"
#include "apb_proxy_packet_def.h"
#include "apb_proxy_log.h"
#include "apb_proxy_packet_encoder.h"
#include "apb_proxy_packet_decoder.h"
#include "string.h"
#include "apb_proxy_utility.h"
#include "hal_sleep_manager.h"

/***********************************************************************************************
**                        AP Bridge Events launch from AP Bridge Proxy                         *
************************************************************************************************/
/*********************************************************
 * @brief      AP Bridge Proxy register AT command event handler.
 * @param[in]  None
 * @return     None
 ***************************************************************/

void apb_proxy_register_at_cmd_req_handler(void)
{
    apb_proxy_at_cmd_reg_cxt_t *p_cmd_reg_cxt = apb_proxy_get_cmd_reg_cxt_pointer();
    apb_proxy_cmd_hdlr_table_t  *p_cmd_hdlr_tbl = apb_proxy_get_cmd_hdlr_table_info();
    apb_proxy_packet_t apb_packet = {0};
    uint32_t encoded_cmd_count;

    encoded_cmd_count = apb_proxy_encode_at_cmd_reg_packet(p_cmd_hdlr_tbl->p_item_tbl + p_cmd_reg_cxt->base_item_index,
                        p_cmd_hdlr_tbl->item_size - p_cmd_reg_cxt->registered_count,
                        &apb_packet);
    if (0 != encoded_cmd_count) {
        p_cmd_reg_cxt->item_count = encoded_cmd_count;
        apb_proxy_send_packet_to_modem(&apb_packet);
    } else {
        apb_proxy_log_error("encode reg error.");
        if (apb_packet.pdata != NULL) {
            p_cmd_reg_cxt->item_count = p_cmd_reg_cxt->item_count - encoded_cmd_count;
            vPortFree(apb_packet.pdata);
            apb_packet.pdata = NULL;
        }
    }
}

/***************************************************************
**              AP Bridge Events come from APP                **
****************************************************************/

/****************************************************************
 * @brief      APP executed AT command result event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ****************************************************************/
void apb_proxy_ap_at_cmd_result_ind_handler(apb_proxy_event_t *p_apb_proxy_event)
{
    apb_proxy_cmd_result_event_t *p_cmd_result = &(p_apb_proxy_event->event_data.cmd_result_ind);
    apb_proxy_at_cmd_context_t cmd_context;
    bool need_to_send_packet = true;
    uint32_t result_code = apb_proxy_get_at_cmd_result_code(&(p_cmd_result->result_packet));
    //apb_proxy_data_mode_t*  data_mode_ctx = NULL;

    if (result_code != APBRIDGE_RESULT_UNSOLICITED) {
        if (apb_proxy_get_at_cmd_context(&cmd_context, p_cmd_result->cmd_id, p_cmd_result->channel_id) == APB_PROXY_STATUS_OK) {
            //data_mode_ctx = apb_proxy_get_data_mode_context(p_cmd_result->channel_id);
            if ((result_code != APBRIDGE_RESULT_PROCEEDING)
                 && ((result_code != APBRIDGE_RESULT_CONNECT))
                 && (result_code != APBRIDGE_RESULT_CUSTOM_CONNECT)
                 && (apb_proxy_get_data_mode_state(p_cmd_result->channel_id) == APB_PROXY_DATA_MODE_DEACTIVATED)) {
                apb_proxy_delete_at_cmd_context(p_cmd_result->cmd_id, p_cmd_result->channel_id);
            }
        } else {
            apb_proxy_log_error("ctx error: cmd:%lu, ch: %lu.", p_cmd_result->cmd_id, p_cmd_result->channel_id);
            need_to_send_packet = false;
        }
    }

    if (true == need_to_send_packet) {
        apb_proxy_send_packet_to_modem(&(p_cmd_result->result_packet));
    } else {
        vPortFree(p_cmd_result->result_packet.pdata);
        p_cmd_result->result_packet.pdata = NULL;
    }
}
/*****************************************************************
 * @brief      APP requests to set up data mode event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 *****************************************************************/
void apb_proxy_create_data_mode_handler(apb_proxy_event_t *p_apb_proxy_event)
{
    configASSERT(p_apb_proxy_event != NULL);
    bool set_up_result = false;
    apb_proxy_data_mode_req_t* p_mode_req = &(p_apb_proxy_event->event_data.data_mode_req);
    apb_proxy_data_mode_t* data_mode_context= apb_proxy_get_data_mode_context(p_mode_req->channel_id);
    if (data_mode_context == NULL) {
        return;
    }
    if (apb_proxy_create_user_data_queue(p_mode_req->channel_id) == APB_PROXY_STATUS_OK) {
        apb_proxy_at_cmd_result_t cmd_result;
        apb_proxy_event_t apb_proxy_result_event;
        apb_proxy_cmd_result_event_t* p_cmd_result_ind = &(apb_proxy_result_event.event_data.cmd_result_ind);
        apb_proxy_packet_t* p_packet_data = &(p_cmd_result_ind->result_packet);
        data_mode_context->data_mode_callback = p_mode_req->callback;
        apb_proxy_result_event.event_type = APB_PROXY_AP_AT_CMD_RESULT_IND;
        cmd_result.cmd_id = APB_PROXY_CMD_ID(p_mode_req->channel_id, p_mode_req->cmd_id);
        if (data_mode_context->data_mode_res_code == APB_PROXY_RESULT_CUSTOM_CONNECT){
            cmd_result.result_code = APB_PROXY_RESULT_CUSTOM_CONNECT;
            cmd_result.length = data_mode_context->data_mode_custom_result_len;
            if (cmd_result.length == 0) {
                cmd_result.pdata = NULL;
            } else {
                cmd_result.pdata = data_mode_context->data_mode_custom_resp;
            }
        } else {
            cmd_result.result_code = APB_PROXY_RESULT_CONNECT;
            cmd_result.pdata = NULL;
            cmd_result.length = 0;
        }
        if (apb_proxy_encode_at_cmd_result_packet(&cmd_result, p_packet_data) == APB_PROXY_STATUS_OK) {
            p_cmd_result_ind->cmd_id = p_mode_req->cmd_id;
            p_cmd_result_ind->channel_id = p_mode_req->channel_id;
            if (apb_proxy_queue_push_msg(g_apb_proxy_internal_mi_tx_queue, &apb_proxy_result_event) == APB_PROXY_STATUS_OK) {
                set_up_result = true;
                data_mode_context->data_mode_state = APB_PROXY_DATA_MODE_ACTIVATED;
            } else {
                vPortFree(p_packet_data->pdata);
                p_packet_data->pdata = NULL;
                apb_proxy_log_error("mi tx full.");
            }
        }
    }

    if (false == set_up_result) {
        apb_proxy_delete_user_data_queue(p_mode_req->channel_id);
        apb_proxy_clear_data_mode_context(p_mode_req->channel_id);
        apb_proxy_log_error("Create Data Mode Failed\r\n");
    }
}
/****************************************************************************************
 * @brief      APP requests to close data mode event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ****************************************************************************************/
void apb_proxy_close_data_mode_handler(apb_proxy_event_t *p_apb_proxy_event)
{
    apb_proxy_event_t apb_proxy_event;
    apb_proxy_packet_t apb_packet = {0};
    uint32_t channel_id = 0xFFFFFFFF;
    apb_proxy_data_mode_t* data_mode_context = NULL;
    configASSERT(p_apb_proxy_event != NULL);

    channel_id = p_apb_proxy_event->event_data.channel_id;
    data_mode_context = apb_proxy_get_data_mode_context(channel_id);

    if (data_mode_context == NULL) {
        return;
    }

    while (apb_proxy_queue_pop_msg(data_mode_context->data_tx_queue_from_ap, &apb_proxy_event) == APB_PROXY_STATUS_OK ) {
        apb_proxy_ap_userdata_handler(&apb_proxy_event);
    }
    apb_proxy_delete_user_data_queue(channel_id);
    apb_proxy_clear_data_mode_context(channel_id);
    if (apb_proxy_encode_close_data_mode_req_packet(channel_id, &apb_packet) == APB_PROXY_STATUS_OK) {
        apb_proxy_send_packet_to_modem(&apb_packet);
    } else {
        apb_proxy_log_error("failed encode close data mode req\r\n");
    }
}
/*****************************************************************
 * @brief      APP requests to send user data event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 *****************************************************************/
void apb_proxy_ap_userdata_handler(apb_proxy_event_t *p_apb_proxy_event)
{
    apb_proxy_packet_t *p_packet_data = &(p_apb_proxy_event->event_data.packetdata);
    apb_proxy_data_mode_t* data_mode_context= apb_proxy_get_data_mode_context(p_packet_data->channel_id);
    apb_proxy_data_mode_event_t data_mode_event = {0};
    configASSERT(p_apb_proxy_event != NULL);
    if (apb_proxy_queue_get_available_space(data_mode_context->data_tx_queue_from_ap) < APB_PROXY_USER_DATA_QUEUE_CONTROL_SPACE_SIZE) {
        apb_proxy_log_info("user TX queue flow control on");
        if (data_mode_context->apb_proxy_request_stop_data == false) {
            data_mode_event.conn_id = APB_PROXY_CMD_ID(data_mode_context->data_mode_channel_id, data_mode_context->data_mode_cmd_id);
            data_mode_context->data_mode_callback(APB_PROXY_STOP_SEND_USER_DATA, &data_mode_event);
            data_mode_context->apb_proxy_request_stop_data = true;
        }
    } else if (apb_proxy_queue_get_available_space(data_mode_context->data_tx_queue_from_ap) > APB_PROXY_USER_DATA_QUEUE_CONTROL_SPACE_SIZE) {
        if ((data_mode_context->apb_proxy_request_stop_data == true)
            && (data_mode_context->modem_request_stop_data == false)) {
            data_mode_event.conn_id = APB_PROXY_CMD_ID(data_mode_context->data_mode_channel_id, data_mode_context->data_mode_cmd_id);
            data_mode_context->data_mode_callback(APB_PROXY_RESUME_SEND_USER_DATA, &data_mode_event);
            data_mode_context->apb_proxy_request_stop_data = false;
        }
    }
    apb_proxy_send_packet_to_modem(p_packet_data);
}
/*********************************************************************
 * @brief      APP request to resume sending user data event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ********************************************************************/
void apb_proxy_ap_xon_req_handler(apb_proxy_event_t *p_apb_proxy_event)
{
    apb_proxy_packet_t *p_packet_data = &(p_apb_proxy_event->event_data.packetdata);
    apb_proxy_data_mode_t* data_mode_context= apb_proxy_get_data_mode_context(p_packet_data->channel_id);
    if (data_mode_context == NULL) {
        return;
    }
    if ((data_mode_context->app_request_stop_data == true) &&
        (data_mode_context->apb_proxy_request_stop_data == false)) {
        apb_proxy_send_packet_to_modem(p_packet_data);
    }
    data_mode_context->app_request_stop_data = false;
}
/***********************************************************************
 * @brief      APP request to stop sending user data event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ************************************************************************/
void apb_proxy_ap_xoff_req_handler(apb_proxy_event_t *p_apb_proxy_event)
{
    apb_proxy_packet_t *p_packet_data = &(p_apb_proxy_event->event_data.packetdata);
    apb_proxy_data_mode_t* data_mode_context= apb_proxy_get_data_mode_context(p_packet_data->channel_id);
    if (data_mode_context == NULL) {
        return;
    }
    if ((data_mode_context->app_request_stop_data == false) &&
        (data_mode_context->apb_proxy_request_stop_data == false)) {
        apb_proxy_send_packet_to_modem(p_packet_data);
    }
    data_mode_context->app_request_stop_data = true;
}

/***********************************************************************
 * @brief      APP request to resume data mode.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ************************************************************************/
void apb_proxy_ap_resume_data_mode_req_handler(apb_proxy_event_t *p_apb_proxy_event)
{
    apb_proxy_packet_t *p_packet_data = &(p_apb_proxy_event->event_data.packetdata);
    if (p_apb_proxy_event == NULL) {
        return;
    }
    apb_proxy_send_packet_to_modem(p_packet_data);
}

/****************************************************
 **      AP Bridge Events come from Modem          **
 ****************************************************/

/*************************************************************************
 * @brief      Modem informs the AT command register result event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 **************************************************************************/
void apb_proxy_md_register_at_cmd_rsp_handler(apb_proxy_event_t *p_apb_proxy_event)
{
    apb_proxy_register_result_t at_cmd_register;
    apb_proxy_at_cmd_reg_cxt_t *p_cmd_reg_cxt = apb_proxy_get_cmd_reg_cxt_pointer();
    apb_proxy_cmd_hdlr_table_t  *p_cmd_hdlr_tbl = apb_proxy_get_cmd_hdlr_table_info();
    apb_proxy_cmd_hdlr_item_t *p_cmd_hdlr_item = NULL;
    uint32_t index = 0;
    configASSERT(p_apb_proxy_event != NULL);
    if (apb_proxy_decode_at_cmd_reg_result_msg(&(p_apb_proxy_event->event_data.packetdata), &at_cmd_register) == APB_PROXY_STATUS_OK) {
        if ( true == at_cmd_register.result ) {
            if (at_cmd_register.registered_count != p_cmd_reg_cxt->item_count) {
                apb_proxy_log_error("cmd reg failed");
            } else {
                /*sync the command id with the command id in modem.*/
                for (index = 0; index < at_cmd_register.registered_count; index++ ) {
                    p_cmd_hdlr_item = p_cmd_hdlr_tbl->p_item_tbl + p_cmd_reg_cxt->base_item_index + index;
                    p_cmd_hdlr_item->cmd_id = at_cmd_register.base_cmd_id + index;
                }
                p_cmd_reg_cxt->registered_count = p_cmd_reg_cxt->registered_count + at_cmd_register.registered_count;
                p_cmd_reg_cxt->base_item_index = p_cmd_reg_cxt->base_item_index + p_cmd_reg_cxt->registered_count;
                if (p_cmd_reg_cxt->registered_count == p_cmd_hdlr_tbl->item_size) {
                    p_cmd_reg_cxt->is_all_cmd_registered = true;
                    #ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
                    apb_proxy_log_info("store the cmd data\r\n");
                    apb_proxy_store_cmd_data();
                    #endif
                } else {
                    apb_proxy_log_info("register more.");
                    apb_proxy_register_at_cmd_req_handler();
                }
            }
        } else {
            apb_proxy_log_error("register failed!!");
        }
    } else {
        apb_proxy_log_error("decode reg error.");
    }
    vPortFree(p_apb_proxy_event->event_data.packetdata.pdata);
    p_apb_proxy_event->event_data.packetdata.pdata = NULL;
}

/****************************************************************************
 * @brief      Modem sends the AT command execution request event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ****************************************************************************/
void apb_proxy_md_at_cmd_req_handler(apb_proxy_event_t *p_apb_proxy_event)
{
    apb_proxy_parse_cmd_param_t at_cmd_req = {0};
    apb_proxy_at_cmd_context_t apb_proxy_at_cmd_context;
    apb_proxy_cmd_hdlr_item_t *p_cmd_hdlr_item = NULL;
    bool exe_cmd = false;
    if (apb_proxy_decode_at_cmd_req_msg(&(p_apb_proxy_event->event_data.packetdata),
                                        &at_cmd_req) == APB_PROXY_STATUS_OK) {
        p_cmd_hdlr_item = apb_proxy_get_at_hdlr_item(APB_PROXY_GET_CMD_ID_IN_MODEM(at_cmd_req.cmd_id));
        if (p_cmd_hdlr_item != NULL) {
            at_cmd_req.name_len = strlen(p_cmd_hdlr_item->p_cmd_head);
            switch(at_cmd_req.mode){
                case APB_PROXY_CMD_MODE_ACTIVE:{
                    at_cmd_req.parse_pos = at_cmd_req.name_len;
                    break;
                }
                case APB_PROXY_CMD_MODE_EXECUTION:
                case APB_PROXY_CMD_MODE_READ:{
                    at_cmd_req.parse_pos = at_cmd_req.name_len + 1;
                    break;
                }
                case APB_PROXY_CMD_MODE_TESTING:{
                    at_cmd_req.parse_pos = at_cmd_req.name_len + 2;
                    break;
                }
                case APB_PROXY_CMD_MODE_INVALID:
                default:{
                    configASSERT(0);
                    break;
                }
            }
            apb_proxy_at_cmd_context.channel_id = APB_PROXY_GET_CHANNEL_ID(at_cmd_req.cmd_id);
            apb_proxy_at_cmd_context.cmd_id = APB_PROXY_GET_CMD_ID_IN_MODEM(at_cmd_req.cmd_id);
            apb_proxy_at_cmd_context.callback = p_cmd_hdlr_item->cmd_hdlr;
            apb_proxy_at_cmd_context.used = true;
            if (at_cmd_req.string_ptr != NULL){
                apb_proxy_log_info("AT Req:%s", at_cmd_req.string_ptr);
            }else{
                apb_proxy_log_error("AT data is NULL.");
            }
            if (apb_proxy_save_at_cmd_context(&apb_proxy_at_cmd_context) != APB_PROXY_STATUS_OK) {
                apb_proxy_log_error("save cmd ctx ERR");
            } else {
                if (apb_proxy_at_cmd_context.callback != NULL) {
                    apb_proxy_exe_cmd_t cmd;
                    exe_cmd = true;
                    memset(&cmd, 0, sizeof(cmd));
                    cmd.cmd_handler = apb_proxy_at_cmd_context.callback;
                    cmd.share_buf = p_apb_proxy_event->event_data.packetdata.pdata;
                    memcpy(&(cmd.cmd_param), &at_cmd_req, sizeof(at_cmd_req));
                    xQueueSend(g_apb_proxy_cmd_queue, &cmd, 0U);
                } else {
                    apb_proxy_log_error("at callback ERR");
                }
            }
        }else{
            apb_proxy_log_error("find cmd info ERR.");
        }
    } else {
        apb_proxy_log_error("decode at req ERR.");
    }
    if (exe_cmd == false) {
        vPortFree(p_apb_proxy_event->event_data.packetdata.pdata);
        p_apb_proxy_event->event_data.packetdata.pdata = NULL;
    }
}
/****************************************************************************
 * @brief      Modem sends user data to App event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ***************************************************************************/
void apb_proxy_md_userdata_ind_handler(apb_proxy_event_t *p_apb_proxy_event)
{
    uint32_t channel_id = 0;
    apb_proxy_data_mode_t* data_mode_ctx = NULL;
    apb_proxy_data_mode_event_t data_mode_event = {0};
    if (apb_proxy_decode_userdata_msg(&(p_apb_proxy_event->event_data.packetdata), &(data_mode_event.event_data.user_data)) == APB_PROXY_STATUS_OK) {
         channel_id = apb_proxy_get_channel_id(&(p_apb_proxy_event->event_data.packetdata));
         data_mode_ctx = apb_proxy_get_data_mode_context(channel_id);
         if ((data_mode_ctx != NULL) && (data_mode_ctx->data_mode_callback != NULL)) {
             apb_proxy_log_info("user data from channel:%d\r\n", channel_id);
             data_mode_event.conn_id = APB_PROXY_CMD_ID(data_mode_ctx->data_mode_channel_id, data_mode_ctx->data_mode_cmd_id);
             (data_mode_ctx->data_mode_callback)(APB_PROXY_USER_DATA_IND, &data_mode_event);
             if (apb_proxy_queue_get_available_space(data_mode_ctx->data_rx_queue_from_md) > (APB_PROXY_USER_DATA_RX_QUEUE_SIZE - APB_PROXY_USER_DATA_QUEUE_CONTROL_SPACE_SIZE)) {
                 if ((data_mode_ctx->apb_proxy_request_stop_data == true) && (data_mode_ctx->app_request_stop_data == false)) {
                     apb_proxy_packet_t apb_packet = {0};
                     if (apb_proxy_encode_xon_packet(data_mode_ctx->data_mode_channel_id, &apb_packet) == APB_PROXY_STATUS_OK) {
                         apb_proxy_log_info("xon to modem\r\n");
                         apb_proxy_send_packet_to_modem(&apb_packet);
                         data_mode_ctx->apb_proxy_request_stop_data = false;
                     }
                 }
             }
         }
    } else {
        apb_proxy_log_error("decode user data ERR.");
    }
    vPortFree(p_apb_proxy_event->event_data.packetdata.pdata);
    p_apb_proxy_event->event_data.packetdata.pdata = NULL;
}
/****************************************************************************
 * @brief      Modem sends data mode closed indicationi event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ***************************************************************************/
void apb_proxy_md_data_mode_closed_ind_handler(apb_proxy_event_t *p_apb_proxy_event)
{
    uint32_t channel_id = 0xFFFF;
    apb_proxy_context_t *p_apb_proxy_context = apb_proxy_get_apb_proxy_context_pointer();
    if (apb_proxy_decode_data_mode_closed_ind_msg(&(p_apb_proxy_event->event_data.packetdata), &channel_id) == APB_PROXY_STATUS_OK) {
        apb_proxy_event_t apb_proxy_event;
        apb_proxy_data_mode_event_callback_t data_mode_callback = NULL;
        apb_proxy_data_mode_t* data_mode_ctx = apb_proxy_get_data_mode_context(channel_id);
        apb_proxy_data_mode_event_t data_mode_event = {0};
        apb_proxy_log_info("data mode closed ind:%d\r\n", channel_id);
        if (data_mode_ctx == NULL) {
            return;
        }
        if (NULL != data_mode_ctx->data_tx_queue_from_ap) {
            while (apb_proxy_queue_pop_msg(data_mode_ctx->data_tx_queue_from_ap, &apb_proxy_event) == APB_PROXY_STATUS_OK ) {
                apb_proxy_packet_t *p_packet_data = &(p_apb_proxy_event->event_data.packetdata);
                vPortFree(p_packet_data->pdata);
                p_packet_data->pdata = NULL;
            }
        }
        data_mode_callback = data_mode_ctx->data_mode_callback;
        data_mode_event.conn_id = APB_PROXY_CMD_ID(data_mode_ctx->data_mode_channel_id, data_mode_ctx->data_mode_cmd_id);
        if (data_mode_callback != NULL) {
            data_mode_callback(APB_PROXY_DATA_MODE_CLOSED_IND, &data_mode_event);
        }
        apb_proxy_delete_at_cmd_context(data_mode_ctx->data_mode_cmd_id, data_mode_ctx->data_mode_channel_id);
        apb_proxy_delete_user_data_queue(channel_id);
        apb_proxy_clear_data_mode_context(channel_id);
        if (true == p_apb_proxy_context->be_sleep_locked){
            if (hal_sleep_manager_release_sleeplock(p_apb_proxy_context->apb_sleep_handle,
                HAL_SLEEP_LOCK_DEEP) == HAL_SLEEP_MANAGER_OK){
                p_apb_proxy_context->be_sleep_locked = false;
                apb_proxy_log_info("Rel sl_lock.");
            }else{
                apb_proxy_log_error("Failed to rel sl_lock.");
            }
        }
    } else {
        apb_proxy_log_error("decode closed data mode ind ERR.");
    }


    vPortFree(p_apb_proxy_event->event_data.packetdata.pdata);
    p_apb_proxy_event->event_data.packetdata.pdata = NULL;
}

/****************************************************************************
 * @brief      Modem sends data mode temp deactived event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ***************************************************************************/
void apb_proxy_md_data_mode_temp_deactived_ind_handler(apb_proxy_event_t *p_apb_proxy_event)
{
    uint32_t channel_id = 0;
    apb_proxy_data_mode_event_t data_mode_event = {0};
    if (apb_proxy_decode_data_mode_temp_deactive_ind_msg(&(p_apb_proxy_event->event_data.packetdata), &channel_id) == APB_PROXY_STATUS_OK) {
        apb_proxy_data_mode_event_callback_t data_mode_callback = NULL;
        apb_proxy_data_mode_t* data_mode_ctx = apb_proxy_get_data_mode_context(channel_id);
        apb_proxy_log_info("data mode deactive ind:%d\r\n", channel_id);
        if (data_mode_ctx == NULL) {
            return;
        }
        data_mode_callback = data_mode_ctx->data_mode_callback;
        data_mode_event.conn_id = APB_PROXY_CMD_ID(data_mode_ctx->data_mode_channel_id, data_mode_ctx->data_mode_cmd_id);
        data_mode_ctx->data_mode_state = APB_PROXY_DATA_MODE_TEMP_DEACTIVATED;
        data_mode_callback(APB_PROXY_DATA_MODE_TEMP_DEACTIVEED_IND, &data_mode_event);
    } else {
        apb_proxy_log_error("decode closed data mode ind ERR.");
    }

    vPortFree(p_apb_proxy_event->event_data.packetdata.pdata);
    p_apb_proxy_event->event_data.packetdata.pdata = NULL;

}

/****************************************************************************
 * @brief      Modem sends data mode resumed indication event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ***************************************************************************/
void apb_proxy_md_data_mode_resumed_ind_handler(apb_proxy_event_t *p_apb_proxy_event)
{
    uint32_t channel_id = 0;
    apb_proxy_data_mode_event_t data_mode_event = {0};
    if (apb_proxy_decode_data_mode_resumed_ind_msg(&(p_apb_proxy_event->event_data.packetdata), &channel_id) == APB_PROXY_STATUS_OK) {
        apb_proxy_data_mode_event_callback_t data_mode_callback = NULL;
        apb_proxy_data_mode_t* data_mode_ctx = apb_proxy_get_data_mode_context(channel_id);
        apb_proxy_log_info("data mode resumed ind:%d\r\n", channel_id);
        if (data_mode_ctx == NULL) {
            return;
        }
        data_mode_callback = data_mode_ctx->data_mode_callback;
        data_mode_event.conn_id = APB_PROXY_CMD_ID(data_mode_ctx->data_mode_channel_id, data_mode_ctx->data_mode_cmd_id);
        data_mode_ctx->data_mode_state = APB_PROXY_DATA_MODE_ACTIVATED;
        data_mode_callback(APB_PROXY_DATA_MODE_RESUMED_IND, &data_mode_event);
    } else {
        apb_proxy_log_error("decode closed data mode ind ERR.");
    }

    vPortFree(p_apb_proxy_event->event_data.packetdata.pdata);
    p_apb_proxy_event->event_data.packetdata.pdata = NULL;
}
/***************************************************************************
 * @brief      Modem request to resume sending user data event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 ****************************************************************************/
void apb_proxy_md_xon_req_handler(apb_proxy_event_t *p_apb_proxy_event)
{
    uint32_t channel_id = apb_proxy_get_channel_id(&(p_apb_proxy_event->event_data.packetdata));
    apb_proxy_data_mode_event_t data_mode_event = {0};
    apb_proxy_data_mode_t* data_mode_ctx = apb_proxy_get_data_mode_context(channel_id);
    if (data_mode_ctx != NULL) {
        if (data_mode_ctx->data_mode_state == APB_PROXY_DATA_MODE_ACTIVATED) {
            data_mode_ctx->modem_request_stop_data = false;
            if (data_mode_ctx->apb_proxy_request_stop_data == false) {
                data_mode_event.conn_id = APB_PROXY_CMD_ID(data_mode_ctx->data_mode_channel_id, data_mode_ctx->data_mode_cmd_id);
                data_mode_ctx->data_mode_callback(APB_PROXY_RESUME_SEND_USER_DATA, &data_mode_event);
            }
        }
    }
    vPortFree(p_apb_proxy_event->event_data.packetdata.pdata);
    p_apb_proxy_event->event_data.packetdata.pdata = NULL;
}
/*****************************************************************************
 * @brief      Modem request to stop sending user data event handler.
 * @param[in]  p_apb_proxy_event points to the event content.
 * @return     None
 */
void apb_proxy_md_xoff_req_handler(apb_proxy_event_t *p_apb_proxy_event)
{
    uint32_t channel_id = apb_proxy_get_channel_id(&(p_apb_proxy_event->event_data.packetdata));
    apb_proxy_data_mode_event_t data_mode_event = {0};
    apb_proxy_data_mode_t* data_mode_ctx = apb_proxy_get_data_mode_context(channel_id);

    if (data_mode_ctx != NULL) {
        if (data_mode_ctx->data_mode_state == APB_PROXY_DATA_MODE_ACTIVATED) {
            data_mode_ctx->modem_request_stop_data = true;
            data_mode_event.conn_id = APB_PROXY_CMD_ID(data_mode_ctx->data_mode_channel_id, data_mode_ctx->data_mode_cmd_id);
            data_mode_ctx->data_mode_callback(APB_PROXY_STOP_SEND_USER_DATA, &data_mode_event);
        }
    }
    vPortFree(p_apb_proxy_event->event_data.packetdata.pdata);
    p_apb_proxy_event->event_data.packetdata.pdata = NULL;

}
/****************************************************
**     User Data Flow Control Processor            **
****************************************************/
/*************************************************************
 * @brief     AP Bridge user data flow control processor.
 * @param[in] None
 * @return    None
 *************************************************************/
void apb_proxy_userdata_flow_control_processor(void)
{
    apb_proxy_data_mode_t* data_mode_ctx = NULL;
    uint32_t index = 0;
    apb_proxy_data_mode_event_t data_mode_event = {0};
    apb_proxy_context_t *p_apb_proxy_context = apb_proxy_get_apb_proxy_context_pointer();
    for(index = 0; index < APB_PROXY_SUPPORT_DATA_MODE_MAX; index ++) {
        data_mode_ctx = p_apb_proxy_context->apb_data_mode_context + index;
        if ((data_mode_ctx->data_mode_state != APB_PROXY_DATA_MODE_DEACTIVATED) &&
            (true == data_mode_ctx->apb_proxy_request_stop_data)) {
            if (apb_proxy_queue_get_available_space(data_mode_ctx->data_tx_queue_from_ap) >= (apb_proxy_queue_get_capacity(data_mode_ctx->data_tx_queue_from_ap) / 2)) {
                data_mode_event.conn_id = APB_PROXY_CMD_ID(data_mode_ctx->data_mode_channel_id, data_mode_ctx->data_mode_cmd_id);
                data_mode_ctx->data_mode_callback(APB_PROXY_RESUME_SEND_USER_DATA, &data_mode_event);
            }
        }
    }
}
void apb_proxy_send_packet_to_modem(apb_proxy_packet_t *p_apb_packet)
{
    apb_proxy_context_t *p_apb_proxy_context = apb_proxy_get_apb_proxy_context_pointer();
    configASSERT(p_apb_packet != NULL);
    configASSERT(p_apb_packet->pdata != NULL);
    configASSERT(p_apb_packet->length != 0);

    if (mux_ap_send_data(p_apb_proxy_context->channel_id, p_apb_packet->pdata,
                         p_apb_packet->length, 0U) != MUX_AP_STATUS_OK) {
        /*Currently, MUX AP always returns OK.*/
        apb_proxy_log_error("data out ERR");
    } else {
        apb_proxy_log_info("APB[OUT]");
    }
}

