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

#include "apb_proxy.h"
#include "stdlib.h"
#include "apb_proxy_task.h"
#include "FreeRTOS.h"
#include "apb_proxy_log.h"
#include "apb_proxy_context_manager.h"
#include "apb_proxy_packet_decoder.h"
#include "apb_proxy_packet_encoder.h"
#include "string.h"
#include "apb_proxy_utility.h"
#include "apb_proxy_msg_queue_def.h"
#include "apb_proxy_packet_def.h"


/**
 * @brief             The APP calls this function to send an AT command result to the APB proxy.
 *                    This is a none-blocking function. This function can be used for multi-tasking.
 * @param[in] presult A pointer to the AT command result structure.
 * @return            If the result is #APB_PROXY_STATUS_OK, the APB will guarantee the data is sent out successfully.
 *                    If the result is #APB_PROXY_STATUS_ERROR, there are four causes for the error.
 *                    (1) not enough heap memory, (2) more than one command try to switch to data mode,
 *                    (3) before closing the data mode, it tried to send command result, (4) invalid command ID.
 */
apb_proxy_status_t apb_proxy_send_at_cmd_result(apb_proxy_at_cmd_result_t *presult)
{
    apb_proxy_event_t apb_proxy_event;
    apb_proxy_cmd_result_event_t* p_cmd_result_event = &(apb_proxy_event.event_data.cmd_result_ind);
    apb_proxy_packet_t* p_packet_data = &(p_cmd_result_event->result_packet);
    configASSERT(presult != NULL);

    if (presult->length > (APB_PROXY_MAX_DATA_SIZE - APB_PROXY_AT_CMD_RSP_HEADER_SIZE)){
        return APB_PROXY_STATUS_ERROR;
    }

    if ((uxQueueSpacesAvailable(g_apb_proxy_external_rx_queue) <= APB_PROXY_RESERVE_QUEUE_SIZE)
        && ((presult->result_code == APB_PROXY_RESULT_UNSOLICITED) || (presult->result_code == APB_PROXY_RESULT_PROCEEDING))){
        /*Reserve some spaces for AT command final responses.*/
        apb_proxy_log_error("drop urc or intermediate\r\n");
        return APB_PROXY_STATUS_ERROR;
    }

    if (APB_PROXY_RESULT_RING == presult->result_code){
        /* NBIOT does not support RING. */
        presult->result_code = APB_PROXY_RESULT_ERROR;
    }

    if ((APB_PROXY_RESULT_CONNECT == presult->result_code)
        || (APB_PROXY_RESULT_CUSTOM_CONNECT == presult->result_code)) {
        /* Pre-set the data mode as active state. */
        if (apb_proxy_create_data_mode_context(APB_PROXY_GET_CMD_ID_IN_MODEM(presult->cmd_id),
                                               APB_PROXY_GET_CHANNEL_ID(presult->cmd_id), presult->result_code,
                                               presult->length, presult->pdata) == false) {
            return APB_PROXY_STATUS_ERROR;
        } else {
            /*In Ap bridge, when the data mode is really set up,
              "CONNECT" data will be sent out by AP Bridge.*/
            return APB_PROXY_STATUS_OK;
        }
    }

    apb_proxy_event.event_type = APB_PROXY_AP_AT_CMD_RESULT_IND;
    if (presult->cmd_id == APB_PROXY_INVALID_CMD_ID){
        p_cmd_result_event->cmd_id = 0xFFFFFFFF;
        p_cmd_result_event->channel_id = 0xFFFFFFFF;
    }else{
        p_cmd_result_event->cmd_id = APB_PROXY_GET_CMD_ID_IN_MODEM(presult->cmd_id);
        p_cmd_result_event->channel_id = APB_PROXY_GET_CHANNEL_ID(presult->cmd_id);
    }
    if (apb_proxy_encode_at_cmd_result_packet(presult, p_packet_data) == APB_PROXY_STATUS_ERROR) {
        apb_proxy_log_error("encode cmd result failed.");
        return APB_PROXY_STATUS_ERROR;
    }

    if (apb_proxy_send_msg_to_ap_bridge_proxy(&apb_proxy_event) == APB_PROXY_STATUS_ERROR) {
        vPortFree(p_packet_data->pdata);
        p_packet_data->pdata = NULL;
        return APB_PROXY_STATUS_ERROR;
    }

    return APB_PROXY_STATUS_OK;
}
/**
 * @brief               If the AT command's result is "CONNECT", the APP requests APB proxy to set up
 *                      data connection (data mode) to transfer user data or binary data. Before calling this function,
 *                      APP must call #apb_proxy_send_at_cmd_result() to send the command result "CONNECT".
 * @param[in] call_back APB proxy will use the callback to inform data mode related events. Please refer to #apb_proxy_event_type_t.
 * @param[in] cmd_id    AT command to set to data mode.
 * @return              If data connection is successfully created, return a non-zero value.
 *                      If data connection failed to be created, return 0.
 */
apb_proxy_data_conn_id_t apb_proxy_create_data_mode(apb_proxy_data_mode_event_callback_t call_back,
                                                    apb_proxy_cmd_id_t cmd_id)
{
    apb_proxy_event_t apb_proxy_event = {0};
    apb_proxy_channel_id_t channel_id = APB_PROXY_GET_CHANNEL_ID(cmd_id);
    configASSERT(call_back != NULL);
    /*the command sends "CONNECT", we have pre-set data mode as active.*/
    if (apb_proxy_get_data_mode_state(APB_PROXY_GET_CHANNEL_ID(cmd_id)) != APB_PROXY_DATA_MODE_ACTIVATING){
        return APB_PROXY_INVALID_DATA_CONN_ID;
    }

    apb_proxy_event.event_type = APB_PROXY_AP_SET_UP_DATA_MODE_REQ;
    apb_proxy_event.event_data.data_mode_req.callback = call_back;
    apb_proxy_event.event_data.data_mode_req.cmd_id = APB_PROXY_GET_CMD_ID_IN_MODEM(cmd_id);;
    apb_proxy_event.event_data.data_mode_req.channel_id = APB_PROXY_GET_CHANNEL_ID(cmd_id);
    if (apb_proxy_send_msg_to_ap_bridge_proxy(&apb_proxy_event) == APB_PROXY_STATUS_OK){
        /* return data mode identifier for specific application.*/
        return (apb_proxy_data_conn_id_t)cmd_id;
    }else{
        return APB_PROXY_INVALID_DATA_CONN_ID;
    }
}
/**
 * @brief             This function closes the data mode connection.
 *                    The APP must call #apb_proxy_send_at_cmd_result() to send the
 *                    final result (#APB_PROXY_RESULT_NO_CARRIER) of the AT command to close data mode connection.
 * @param[in] conn_id Please refer to #apb_proxy_data_conn_id_t.
 * @return            If the result is #APB_PROXY_STATUS_ERROR, the cause would be: connection ID is not valid,
 *                    data mode is not active, or message queue is full.
 */
apb_proxy_status_t apb_proxy_close_data_mode(apb_proxy_data_conn_id_t conn_id)
{
    apb_proxy_event_t apb_proxy_event;
    apb_proxy_cmd_id_t proxy_cmd_id = (apb_proxy_cmd_id_t)(conn_id);

    if (apb_proxy_get_data_mode_state(APB_PROXY_GET_CHANNEL_ID(proxy_cmd_id)) == APB_PROXY_DATA_MODE_DEACTIVATED) {
        return APB_PROXY_STATUS_ERROR;
    }

    apb_proxy_request_close_data_mode(APB_PROXY_GET_CHANNEL_ID(conn_id));
    apb_proxy_event.event_type = APB_PROXY_AP_CLOSE_DATA_MODE_IND;
    apb_proxy_event.event_data.channel_id = APB_PROXY_GET_CHANNEL_ID(conn_id);
    return apb_proxy_send_msg_to_ap_bridge_proxy(&apb_proxy_event);
}

/**
 * @brief                The APP calls this function to send user data to the APB proxy.
 *                       This is a none-blocking function.
 * @param[in] conn_id    Please refer to #apb_proxy_data_conn_id_t
 * @param[in] puser_data A pointer to the user data structure.
 * @return               If the result is #APB_PROXY_STATUS_OK, APB proxy will guarantee to send the data out.
 *                       If the result is #APB_PROXY_STATUS_ERROR, the cause would be: connection is not valid,
 *                       data mode is not active, or message queue is full.
 */
apb_proxy_status_t apb_proxy_send_user_data(apb_proxy_data_conn_id_t conn_id, apb_proxy_user_data_t *puser_data)
{
    apb_proxy_event_t apb_proxy_event;
    apb_proxy_cmd_id_t proxy_cmd_id = (apb_proxy_cmd_id_t)conn_id;
    apb_proxy_packet_t* p_packet = &(apb_proxy_event.event_data.packetdata);
    configASSERT(NULL != puser_data);
    configASSERT(NULL != puser_data->pbuffer);
    configASSERT(0U != puser_data->length);

    if (puser_data->length > (APB_PROXY_MAX_DATA_SIZE - APB_PROXY_USER_DATA_HEADER_SIZE)){
        return APB_PROXY_STATUS_ERROR;
    }

    if (apb_proxy_get_data_mode_state(APB_PROXY_GET_CHANNEL_ID(proxy_cmd_id)) != APB_PROXY_DATA_MODE_ACTIVATED) {
        return APB_PROXY_STATUS_ERROR;
    }

    apb_proxy_event.event_type = APB_PROXY_AP_SEND_USER_DATA_REQ;

    if (apb_proxy_encode_user_data_packet(puser_data, APB_PROXY_GET_CHANNEL_ID(proxy_cmd_id),
                                          p_packet) == APB_PROXY_STATUS_ERROR) {
        apb_proxy_log_error("encode user data failed.");
        return APB_PROXY_STATUS_ERROR;
    }

    if (apb_proxy_send_msg_to_ap_bridge_proxy(&apb_proxy_event) == APB_PROXY_STATUS_ERROR) {
        vPortFree(p_packet->pdata);
        p_packet->pdata = NULL;
        return APB_PROXY_STATUS_ERROR;
    }
    return APB_PROXY_STATUS_OK;
}

/**
 * @brief             The APP requests APB proxy to stop sending user data for flow control.
 *                    Call this function, if the APP cannot process more user data.
 *                    This is a non-blocking function.
 * @param     conn_id Please refer to #apb_proxy_data_conn_id_t
 * @return            If the result is #APB_PROXY_STATUS_OK, APB proxy will guarantee to send the data out.
 *                    If the result is #APB_PROXY_STATUS_ERROR, the cause would be: connection is not valid,
 *                    data mode is not active, or message queue is full.
 */
apb_proxy_status_t apb_proxy_stop_send_user_data(apb_proxy_data_conn_id_t conn_id)
{
    apb_proxy_event_t apb_proxy_event;
    apb_proxy_cmd_id_t proxy_cmd_id = (apb_proxy_cmd_id_t)conn_id;
    apb_proxy_packet_t* p_packet = &(apb_proxy_event.event_data.packetdata);

    if (apb_proxy_get_data_mode_state(APB_PROXY_GET_CHANNEL_ID(proxy_cmd_id)) != APB_PROXY_DATA_MODE_ACTIVATED) {
        return APB_PROXY_STATUS_ERROR;
    }

    apb_proxy_event.event_type = APB_PROXY_AP_XOFF_REQ;
    if (apb_proxy_encode_xoff_packet(APB_PROXY_GET_CHANNEL_ID(proxy_cmd_id), p_packet) == APB_PROXY_STATUS_ERROR) {
        apb_proxy_log_error("encode Xoff failed.");
        return APB_PROXY_STATUS_ERROR;
    }

    if (apb_proxy_send_msg_to_ap_bridge_proxy(&apb_proxy_event) == APB_PROXY_STATUS_ERROR) {
        vPortFree(apb_proxy_event.event_data.packetdata.pdata);
        apb_proxy_event.event_data.packetdata.pdata = NULL;
        return APB_PROXY_STATUS_ERROR;
    }
    return APB_PROXY_STATUS_OK;
}

/**
 * @brief             The APP requests APB to resume sending user data.
 *                    Call this function to process more user data in the APP.
 *                    This is a none-blocking function.
 * @param     conn_id Please refer to #apb_proxy_data_conn_id_t
 * @return            If the result is #APB_PROXY_STATUS_OK, APB proxy will guarantee to send the data out.
 *                    If the result is #APB_PROXY_STATUS_ERROR, the cause would be: connection is not valid,
 *                    data mode is not active, or message queue is full.
 */
apb_proxy_status_t apb_proxy_resume_send_user_data(apb_proxy_data_conn_id_t conn_id)
{
    apb_proxy_event_t apb_proxy_event;
    apb_proxy_cmd_id_t proxy_cmd_id = (apb_proxy_cmd_id_t)conn_id;

    if (apb_proxy_get_data_mode_state(APB_PROXY_GET_CHANNEL_ID(proxy_cmd_id)) != APB_PROXY_DATA_MODE_ACTIVATED) {
        return APB_PROXY_STATUS_ERROR;
    }

    apb_proxy_event.event_type = APB_PROXY_AP_XON_REQ;
    if (apb_proxy_encode_xon_packet(APB_PROXY_GET_CHANNEL_ID(proxy_cmd_id), &(apb_proxy_event.event_data.packetdata)) == APB_PROXY_STATUS_ERROR) {
        apb_proxy_log_error("encode Xon failed.");
        return APB_PROXY_STATUS_ERROR;
    }

    if (apb_proxy_send_msg_to_ap_bridge_proxy(&apb_proxy_event) == APB_PROXY_STATUS_ERROR) {
        vPortFree(apb_proxy_event.event_data.packetdata.pdata);
        apb_proxy_event.event_data.packetdata.pdata = NULL;
        return APB_PROXY_STATUS_ERROR;
    }

    return APB_PROXY_STATUS_OK;
}

/**
 * @brief             Chech whether the current channel is in data mode state.
 * @param     apb_id  The id can be #apb_proxy_data_conn_id_t or # apb_proxy_cmd_id_t.
 * @return            If the result is true, the current channel is in data mode state.
 *                    If the result is false, the current channel is not in data mode state.
 */
bool apb_proxy_current_channel_data_mode_actived(uint32_t apb_id)
{
    uint32_t channel_id = APB_PROXY_GET_CHANNEL_ID(apb_id);
    if (apb_proxy_get_data_mode_state(channel_id) != APB_PROXY_DATA_MODE_DEACTIVATED) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief             If the current data mode has been switched to command mode temperarily,
 *                    the data mode can be switched back by calling this function.
 * @param     conn_id Please refer to #apb_proxy_data_conn_id_t
 * @return            If the result is #APB_PROXY_STATUS_OK, data mode is switched successfully.
 *                    If the result is #APB_PROXY_STATUS_ERROR, failed to switch to data mode.
 */
apb_proxy_status_t apb_proxy_resume_data_mode(apb_proxy_data_conn_id_t conn_id)
{
    apb_proxy_event_t apb_proxy_event;
    apb_proxy_cmd_id_t proxy_cmd_id = (apb_proxy_cmd_id_t)conn_id;
    uint32_t channel_id = APB_PROXY_GET_CHANNEL_ID(proxy_cmd_id);
    if (apb_proxy_get_data_mode_state(channel_id) != APB_PROXY_DATA_MODE_TEMP_DEACTIVATED) {
        return APB_PROXY_STATUS_ERROR;
    }

    apb_proxy_event.event_type = APB_PROXY_AP_DATA_MODE_RESUME_REQ;
    if (apb_proxy_encode_resume_data_mode_req_packet(APB_PROXY_GET_CHANNEL_ID(proxy_cmd_id), &(apb_proxy_event.event_data.packetdata)) == APB_PROXY_STATUS_ERROR) {
        apb_proxy_log_error("encode resume req failed.");
        return APB_PROXY_STATUS_ERROR;
    }

    if (apb_proxy_send_msg_to_ap_bridge_proxy(&apb_proxy_event) == APB_PROXY_STATUS_ERROR) {
        vPortFree(apb_proxy_event.event_data.packetdata.pdata);
        apb_proxy_event.event_data.packetdata.pdata = NULL;
        return APB_PROXY_STATUS_ERROR;
    }
    apb_proxy_set_data_mode_state(APB_PROXY_GET_CHANNEL_ID(proxy_cmd_id), APB_PROXY_DATA_MODE_ACTIVATED);
    return APB_PROXY_STATUS_OK;
}
