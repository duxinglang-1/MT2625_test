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

#ifndef __APB_PROXY__H__
#define __APB_PROXY__H__

/**
 * @addtogroup APB_PROXY
 * @{
 * This section describes the AP Bridge Proxy APIs.
 *
 * User can register some customized AT commands into AP Bridge proxy.
 * When external AP or PC tool sends the registered AT commands,
 * AP Bridge proxy will forward the AT command request to the registered command handler by callback function.
 * After user executes the AT command, the AT command results can be sent to external AP
 * or PC tool by calling AP Bridge Proxy's APIs.
 * There are command mode and data mode for AT command. By default, it is always in command mode.
 * If the user wants to transfer lots of binary data, data transfer has better performance in data mode.
 * When in data mode, the data will not go through ATCI module shown in AP Bridge Architecture diagram.
 * If the At command's result is CONNECT, user can begin to set up data mode.
 * Please note: only one channel can go to data mode at the same time.
 *
 * Terms and Acronyms
 * ======
 * |Terms                         |Details                                                                 |
 * |------------------------------|------------------------------------------------------------------------|
 * |\b APB/apb                    | AP bridge (APB) is used to connect the command and user data between application and modem domains.
 * |\b MUX                        | Multiplexer (MUX) represents the adaption layer between application and modem over cross side communication interface, not 3GPP TS 27.010 protocol. |
 * |\b CME                        | Cellular mobile equipment. |
 * |\b PLMN                       | Public land mobile network. |
 * |\b PDP                        | Packet data protocol. |
 * |\b PSD                        | Packet switched data. |
 *
 * @section apb_proxy_architecture AP Bridge Architecture
 * - <B> The APB architecture in command mode is shown below: </B>
 * @image html apb_proxy_architecture_command_mode.png
 *
 * - <B> The APB architecture in data mode is shown below: </B>
 * @image html apb_proxy_architecture_data_mode.png
 *
 * @section apb_proxy_apis How to use this module
 * - <B>How to process an AT command request </B>
 *   - Step 1. Write a command handler
 *    @code
 *    apb_proxy_status_t apb_proxy_example_at_command_handler(apb_proxy_parse_cmd_param_t *p_parse_cmd)
 *    {
 *        configASSERT(p_parse_cmd != NULL);
 *        apb_proxy_at_cmd_result_t cmd_result;
 *        // Please note, if the output result is short, stack buffer is also supported.
 *        uint8_t* p_out_buffer = NULL;
 *        // Initialize cmd_result.
 *        cmd_result.result_code = APB_PROXY_RESULT_OK;
 *        cmd_result.pdata = NULL;
 *        cmd_result.length = 0;
 *        cmd_result.cmd_id = p_parse_cmd->cmd_id;
 *        switch (p_parse_cmd->mode) {
 *            case APB_PROXY_CMD_MODE_READ: {
 *                p_out_buffer = (uint8_t *)pvPortMalloc(buffer_size);
 *                memset(p_out_buffer, 0, buffer_size);
 *                // Do something. Put the custom output data into p_out_buffer.
 *                break;
 *            }
 *            case APB_PROXY_CMD_MODE_ACTIVE: {
 *                p_out_buffer = (uint8_t *)pvPortMalloc(buffer_size);
 *                memset(p_out_buffer, 0, buffer_size);
 *                // Do something. Put the custom output data into p_out_buffer.
 *                break;
 *            }
 *            case APB_PROXY_CMD_MODE_EXECUTION: {
 *                p_out_buffer = (uint8_t *)pvPortMalloc(buffer_size);
 *                memset(p_out_buffer, 0, buffer_size);
 *                // Do something. Put the custom output data into p_out_buffer.
 *                break;
 *            }
 *            case APB_PROXY_CMD_MODE_TESTING: {
 *                p_out_buffer = (uint8_t *)pvPortMalloc(buffer_size);
 *                memset(p_out_buffer, 0, buffer_size);
 *                // Do something. Put the custom output data into p_out_buffer.
 *                break;
 *            }
 *            case APB_PROXY_CMD_MODE_INVALID: {
 *                p_out_buffer = (uint8_t *)pvPortMalloc(buffer_size);
 *                memset(p_out_buffer, 0, buffer_size);
 *                // Do something. Put the custom output data into p_out_buffer.
 *                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
 *                break;
 *            }
 *            default: {
 *                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
 *                break;
 *            }
 *        }
 *        cmd_result.result_code = APB_PROXY_RESULT_OK;
 *        if (p_out_buffer != NULL){
 *            cmd_result.pdata = p_out_buffer;
 *            cmd_result.length = strlen(cmd_result.pdata);
 *        }
 *        cmd_result.cmd_id = p_parse_cmd->cmd_id;
 *        if (apb_proxy_send_at_cmd_result(&cmd_result) == APB_PROXY_STATUS_ERROR){
 *            // Failed to send the AT command result.
 *        }
 *        // Free the allocated buffer.
 *        if (p_out_buffer != NULL){
 *            vPortFree((void *)p_out_buffer);
 *            p_out_buffer = NULL;
 *        }
 *
 *        // Send an unsolicited message.
 *        cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
 *        cmd_result.pdata = "AP Bridge Proxy URC Test";
 *        cmd_result.length = strlen(cmd_result.pdata);
 *        cmd_result.cmd_id = p_parse_cmd->cmd_id;
 *        if (apb_proxy_send_at_cmd_result(&cmd_result) == APB_PROXY_STATUS_ERROR){
 *            // Failed to send the AT command result.
 *        }
 *        return APB_PROXY_STATUS_OK;
 *    }
 *    @endcode
 *
 *   - Step 2. Add the AT command definition into command table in "apb_proxy_at_cmd_tbl.c".
 *    @code
 *    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EXAMPLE", apb_proxy_example_at_command_handler)
 *    @endcode
 * - <B>How to create data mode and transfer binary data</B>
 *   - Step 1. Write a command handler
 *    @code
 *    apb_proxy_status_t apb_proxy_example2_at_command_handler(apb_proxy_parse_cmd_param_t *p_parse_cmd)
 *    {
 *        configASSERT(p_parse_cmd != NULL);
 *        apb_proxy_at_cmd_result_t cmd_result;
 *        // Initialize cmd_result.
 *        cmd_result.result_code = APB_PROXY_RESULT_OK;
 *        cmd_result.pdata = NULL;
 *        cmd_result.length = 0;
 *        cmd_result.cmd_id = p_parse_cmd->cmd_id;
 *        switch (p_parse_cmd->mode) {
 *            case APB_PROXY_CMD_MODE_READ: {
 *                // Do something.
 *                cmd_result.result_code = APB_PROXY_RESULT_CONNECT; // Go to data mode.
 *                break;
 *            }
 *            case APB_PROXY_CMD_MODE_ACTIVE: {
 *                // Do something.
 *                cmd_result.result_code = APB_PROXY_RESULT_CONNECT; // Go to data mode.
 *                break;
 *            }
 *            case APB_PROXY_CMD_MODE_EXECUTION: {
 *                // Do something.
 *                cmd_result.result_code = APB_PROXY_RESULT_CONNECT; // Go to data mode.
 *                break;
 *            }
 *            case APB_PROXY_CMD_MODE_TESTING: {
 *                // Do something.
 *                cmd_result.result_code = APB_PROXY_RESULT_CONNECT; // Go to data mode.
 *                break;
 *            }
 *            case APB_PROXY_CMD_MODE_INVALID: {
 *                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
 *                break;
 *            }
 *            default: {
 *                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
 *                break;
 *            }
 *        }
 *        if (apb_proxy_send_at_cmd_result(&cmd_result) == APB_PROXY_STATUS_ERROR){
 *            // Failed to send the AT command result.
 *        }
 *        return APB_PROXY_STATUS_OK;
 *    }
 *    @endcode
 *
 *   - Step 2. Add the AT command definition into command table in "apb_proxy_at_cmd_tbl.c".
 *    @code
 *    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EXAMPLE2", apb_proxy_example2_at_command_handler)
 *    @endcode
 *   - Step 3. Write a data mode event callback.
 *   @code
 *   static void app_data_mode_event_call_back(apb_proxy_event_type_t event, void *pdata)
 *   {
 *       apb_proxy_at_cmd_result_t cmd_result;
 *       switch (event) {
 *           case APB_PROXY_USER_DATA_IND: {
 *               // Process user data sent from an external AP.
 *               break;
 *           }
 *           case APB_PROXY_STOP_SEND_USER_DATA: {
 *               // Flow control event: stop sending more data to the APB proxy.
 *               break;
 *           }
 *           case APB_PROXY_RESUME_SEND_USER_DATA: {
 *               // Flow control event: resume sending data to the APB proxy.
 *               break;
 *           }
 *           default: {
 *               configASSERT(0);
 *               break;
 *           }
 *       }
 *   }
 *   @endcode
 *
 *   - Step 4. Switch data mode, transfer user data, close data mode.
 *   @code
 *   apb_proxy_data_conn_id_t conn_id = 0;
 *   conn_id = apb_proxy_create_data_mode(app_data_mode_event_call_back, cmd_id);
 *   if (conn_id == 0){
 *      // Data mode creation has failed.
 *   }
 *   if (apb_proxy_send_user_data(conn_id, pdata) != APB_PROXY_STATUS_OK){
 *      // Sending user data has failed.
 *   }
 *   // Close the data mode and return to the command mode.
 *   if (apb_proxy_close_data_mode(conn_id) != APB_PROXY_STATUS_OK){
 *      // Closing data mode has failed.
 *   }
 *
 *   // Send the final result for this command.
 *   apb_proxy_at_cmd_result_t cmd_result;
 *   cmd_result.result_code = APB_PROXY_RESULT_NO_CARRIER;
 *   cmd_result.pdata = NULL;
 *   cmd_result.length = 0;
 *   cmd_result.cmd_id = cmd_id;
 *   if (apb_proxy_send_at_cmd_result(&cmd_result) == APB_PROXY_STATUS_OK){
 *       // Send the final result for this command completed successfully.
 *   }else{
 *       // Failed to send the command's final result.
 *   }
 *   @endcode
 */

#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 *  @defgroup apb_proxy_define Define
 *  @{
 */
/** @brief Definition for invalid AT command ID values. */
#ifndef APB_PROXY_INVALID_CMD_ID
#define APB_PROXY_INVALID_CMD_ID 0xFFFFFFFF
#endif

/** @brief Definition for the maximum number of bytes to send through the APB proxy. The unit is in bytes. */
#ifndef APB_PROXY_MAX_DATA_SIZE
#define APB_PROXY_MAX_DATA_SIZE   1500U
#endif

/** @brief This macro is used to define an AT command. */
#ifndef AT_CMD_HDLR_ITEM_ENTRY_DEF
#define AT_CMD_HDLR_ITEM_ENTRY_DEF(cmd_name, cmd_handler) \
                                  {cmd_name, cmd_handler, APB_PROXY_INVALID_CMD_ID},
#endif

/**
 * @}
 */

/**
 * @defgroup apb_proxy_typedef Typedef
 * @{
 */

/** @brief AT command identifier.*/
typedef uint32_t apb_proxy_cmd_id_t;

/** @brief Data connection identifier. Invalid data connection ID is 0xFFFF.*/
typedef uint32_t apb_proxy_data_conn_id_t;

/** @brief AT&data channel identifier.*/
typedef uint32_t apb_proxy_channel_id_t;

#ifndef APB_PROXY_INVALID_DATA_CONN_ID
#define APB_PROXY_INVALID_DATA_CONN_ID 0xFFFFFFFF
#endif

#ifndef APB_PROXY_INVALID_CHANNEL_ID
#define APB_PROXY_INVALID_CHANNEL_ID 0xFFFFFFFF
#endif

#ifndef APB_PROXY_GET_CHANNEL_ID
/* Generate the AT command ID for application. */
#define APB_PROXY_GET_CHANNEL_ID(apb_proxy_cmd_id) ((apb_proxy_cmd_id) >> 24)
#endif

/**
 * @}
 */

/**
 * @defgroup apb_proxy_enum Enumerations
 * @{
 */

/** @brief This enumeration defines the APB proxy API execution result. */
typedef enum {
      APB_PROXY_STATUS_ERROR = 0, /**< The error occurred in APB proxy. */
      APB_PROXY_STATUS_OK    = 1  /**< The operation completed successfully. */
} apb_proxy_status_t;

/** @brief This enumeration defines APB event type for data mode. */
typedef enum {
    APB_PROXY_USER_DATA_IND,                  /**< New user data is ready. */
    APB_PROXY_STOP_SEND_USER_DATA,            /**< Stop sending user data for data flow control. */
    APB_PROXY_RESUME_SEND_USER_DATA,          /**< Resume sending user data for data flow control. */
    APB_PROXY_DATA_MODE_CLOSED_IND,           /**< Low layer has closed the data mode. */
    APB_PROXY_DATA_MODE_TEMP_DEACTIVEED_IND,  /**< Low layer has closed the data mode temporarily. */
    APB_PROXY_DATA_MODE_RESUMED_IND           /**< Low layer has resumed to data mode. */
} apb_proxy_event_type_t;

/** @brief This enumeration defines the AT command operation mode. */
typedef enum {
    APB_PROXY_CMD_MODE_ACTIVE    = 1,   /**< Active mode command, such as "AT+CMD". */
    APB_PROXY_CMD_MODE_EXECUTION = 2,   /**< Execute mode command, such as "AT+CMD=<op>". */
    APB_PROXY_CMD_MODE_READ      = 3,   /**< Read mode command, such as "AT+CMD?". */
    APB_PROXY_CMD_MODE_TESTING   = 4,   /**< Test mode command, such as "AT+CMD=?". */
    APB_PROXY_CMD_MODE_INVALID   = 0xFF /**< The input command doesn't belong to any of the four types. */
} apb_proxy_cmd_mode_t;

/** @brief This enumeration defines the AT Command result code.
           The output string for the specific result code is also provided.
           For example, when the result code is #APB_PROXY_RESULT_NO_CARRIER, the output string is "NO CARRIER".
           Please note, there is no output string for #APB_PROXY_RESULT_PROCEEDING and #APB_PROXY_RESULT_UNSOLICITED. */
typedef enum {
    APB_PROXY_RESULT_OK                                   = 1,  /**< OK (The operation completed successfully.) */
    APB_PROXY_RESULT_CONNECT                              = 2,  /**< CONNECT (Data connection setup completed successfully.) */
    APB_PROXY_RESULT_RING                                 = 3,  /**< RING (Ring indication. Not supported in NBIOT.) */
    APB_PROXY_RESULT_NO_CARRIER                           = 4,  /**< NO CARRIER (Data connection ended or setup has failed.) */
    APB_PROXY_RESULT_ERROR                                = 5,  /**< ERROR (An error occurred.) */
    APB_PROXY_RESULT_NO_DIALTONE                          = 6,  /**< NO DIALTONE (Voice call network has failed.) */
    APB_PROXY_RESULT_BUSY                                 = 7,  /**< BUSY (The system is busy with another command.) */
    APB_PROXY_RESULT_NO_ANSWER                            = 8,  /**< NO ANSWER (Voice call user not answering.) */
    APB_PROXY_RESULT_PROCEEDING                           = 9,  /**< No corresponding output string (Intermediate response.) */
    APB_PROXY_RESULT_UNSOLICITED                          = 10, /**< No corresponding output string (Unsolicited response.) */
    APB_PROXY_RESULT_CUSTOM_CONNECT                       = 11, /**< The user fills hte customed response output when data connection sets up. */
    APB_PROXY_RESULT_CUSTOM_ERROR                         = 12, /**< The user must fill the customed error output. This is final response code. */
    APB_PROXY_ERROR_CME_PHONE_FAILURE                     = 13, /**< +CME ERROR: phone failure. */
    APB_PROXY_ERROR_CME_OPERATION_NOT_ALLOWED             = 14, /**< +CME ERROR: operation not allowed. */
    APB_PROXY_ERROR_CME_OPERATION_NOT_SUPPORTED           = 15, /**< +CME ERROR: operation not supported. */
    APB_PROXY_ERROR_CME_SIM_NOT_INSERTED                  = 16, /**< +CME ERROR: SIM not inserted. */
    APB_PROXY_ERROR_CME_INCORRECT_PASSWORD                = 17, /**< +CME ERROR: incorrect password. */
    APB_PROXY_ERROR_CME_MEMORY_FULL                       = 18, /**< +CME ERROR: memory full. */
    APB_PROXY_ERROR_CME_MEMORY_FAILURE                    = 19, /**< +CME ERROR: memory failure. */
    APB_PROXY_ERROR_CME_LONG_TEXT                         = 20, /**< +CME ERROR: text string too long. */
    APB_PROXY_ERROR_CME_INVALID_TEXT_CHARS                = 21, /**< +CME ERROR: invalid characters in text string. */
    APB_PROXY_ERROR_CME_NO_NETWORK_SERVICE                = 22, /**< +CME ERROR: no network service. */
    APB_PROXY_ERROR_CME_NETWORK_TIMEOUT                   = 23, /**< +CME ERROR: network timeout. */
    APB_PROXY_ERROR_CME_EMERGENCY_ONLY                    = 24, /**< +CME ERROR: network not allowed - emergency calls only. */
    APB_PROXY_ERROR_CME_UNKNOWN                           = 25, /**< +CME ERROR: unknown. */
    APB_PROXY_ERROR_CME_PSD_SERVICES_NOT_ALLOWED          = 26, /**< +CME ERROR: psd services not allowed. */
    APB_PROXY_ERROR_CME_PLMN_NOT_ALLOWED                  = 27, /**< +CME ERROR: plmn not allowed. */
    APB_PROXY_ERROR_CME_LOCATION_AREA_NOT_ALLOWED         = 28, /**< +CME ERROR: location area not allowed. */
    APB_PROXY_ERROR_CME_ROAMING_NOT_ALLOWED               = 29, /**< +CME ERROR: roaming not allowed in this location area. */
    APB_PROXY_ERROR_CME_SERVICE_OPTION_NOT_SUPPORTED      = 30, /**< +CME ERROR: service option not supported. */
    APB_PROXY_ERROR_CME_SERVICE_OPTION_NOT_SUBSCRIBED     = 31, /**< +CME ERROR: requested service option not subscribed. */
    APB_PROXY_ERROR_CME_SERVICE_OPTION_OUT_OF_ORDER       = 32, /**< +CME ERROR: service option temporarily out of order. */
    APB_PROXY_ERROR_CME_UNSPECIFIED_PSD_ERROR             = 33, /**< +CME ERROR: unspecified psd error. */
    APB_PROXY_ERROR_CME_PDP_AUTHENTIFICATION_ERROR        = 34, /**< +CME ERROR: PDP authentication failure. */
    APB_PROXY_INVALID_RESULT_CODE                         = 0xFFFFFFFF /**< Invalid result code. */
} apb_proxy_at_cmd_result_code_t;
/**
 * @}
 */

/**
 * @defgroup apb_proxy_struct Structures
 * @{
 */

/** @brief The structure defines the AT command information details. */
typedef struct {
    apb_proxy_cmd_id_t   cmd_id;      /**< AT Command identifier, please refer to #apb_proxy_cmd_id_t. */
    char                *string_ptr;  /**< AT command's raw data. For example: "AT+cmdtest1=1\r". */
    uint32_t             string_len;  /**< AT command raw data length. */
    uint32_t             name_len;    /**< AT command header length. For example, in "AT+EXAMPLE=1,2,3", name_len = 10 (without symbol "="). */
    uint32_t             parse_pos;   /**< The length after detecting the AT command mode. */
    apb_proxy_cmd_mode_t mode;        /**< For more information, please refer to #apb_proxy_cmd_mode_t. */
} apb_proxy_parse_cmd_param_t;

/** @brief The structure defines the AT command execution results.*/
typedef struct {
    apb_proxy_cmd_id_t              cmd_id;      /**< AT command identifier. */
    apb_proxy_at_cmd_result_code_t  result_code; /**< AT command result. */
    void                           *pdata;       /**< The caller is responsible to manage the memory pdata pointed to.
                                                      AT command extra result string excluding result code and error code.
                                                      If no extra result string, the value is NULL. */
    uint32_t                        length;      /**< Total length of the result string including '\0'.If no extra result string, the value is 0.
                                                      The maximum value is #APB_PROXY_MAX_DATA_SIZE.*/
} apb_proxy_at_cmd_result_t;

/** @brief The structure defines user data contents.*/
typedef struct {
    void         *pbuffer; /**< The sender is responsible to manage the memory.*/
    uint32_t      length;  /**< User data length. The maximum value is #APB_PROXY_MAX_DATA_SIZE.*/
} apb_proxy_user_data_t;

typedef struct {
    apb_proxy_data_conn_id_t conn_id;
    union {
        apb_proxy_user_data_t user_data;
    }event_data;
} apb_proxy_data_mode_event_t;
/**
 * @}
 */
/**
 * @addtogroup apb_proxy_typedef Typedef
 * @{
 */

/**
 * @brief           The APP registers a callback function to the APB using #apb_proxy_create_data_mode()
 *                  to transfer user data or binary data. Don't do any CPU heavy calculation.
 * @param[in] event The triggerred event type for data mode.
 * @param[in] pdata A pointer to the event data. The APB proxy is responsible for managing the memory
 *                  which the pointer points to.
 * @return          None.
 */
typedef void (*apb_proxy_data_mode_event_callback_t)(apb_proxy_event_type_t event, apb_proxy_data_mode_event_t* pdata);

/**
 * @brief                 The APP registers a callback function for a specific AT command.
 *                        Don't do any CPU heavy calculation. Don't call timer delay function.
 * @param[in] p_parse_cmd A pointer to AT command data. APB proxy is responsible for managing
 *                        the memory which the pointer points to.
 * @return                None.
 */
typedef apb_proxy_status_t (*apb_proxy_at_cmd_handler)(apb_proxy_parse_cmd_param_t *p_parse_cmd);

/**
 * @}
 */

/**
 * @brief             The APP calls this function to send an AT command result to the APB proxy.
 *                    This is a none-blocking function. This function can be used for multi-tasking.
 * @param[in] presult A pointer to the AT command result structure.
 * @return            If the result is #APB_PROXY_STATUS_OK, the APB will guarantee the data is sent out successfully.
 *                    If the result is #APB_PROXY_STATUS_ERROR, there are four causes for the error.
 *                    (1) not enough heap memory, (2) more than one command try to switch to data mode,
 *                    (3) before closing the data mode, it tried to send command result, (4) invalid command ID.
 */
apb_proxy_status_t apb_proxy_send_at_cmd_result(apb_proxy_at_cmd_result_t *presult);

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
                                                    apb_proxy_cmd_id_t cmd_id);
/**
 * @brief             This function closes the data mode connection.
 *                    The APP must call #apb_proxy_send_at_cmd_result() to send the
 *                    final result (#APB_PROXY_RESULT_NO_CARRIER) of the AT command to close data mode connection.
 * @param[in] conn_id Please refer to #apb_proxy_data_conn_id_t.
 * @return            If the result is #APB_PROXY_STATUS_ERROR, the cause would be: connection ID is not valid,
 *                    data mode is not active, or message queue is full.
 */
apb_proxy_status_t apb_proxy_close_data_mode(apb_proxy_data_conn_id_t conn_id);
/**
 * @brief                The APP calls this function to send user data to the APB proxy.
 *                       This is a none-blocking function.
 * @param[in] conn_id    Please refer to #apb_proxy_data_conn_id_t
 * @param[in] puser_data A pointer to the user data structure.
 * @return               If the result is #APB_PROXY_STATUS_OK, APB proxy will guarantee to send the data out.
 *                       If the result is #APB_PROXY_STATUS_ERROR, the cause would be: connection is not valid,
 *                       data mode is not active, or message queue is full.
 */
apb_proxy_status_t apb_proxy_send_user_data(apb_proxy_data_conn_id_t conn_id, apb_proxy_user_data_t *puser_data);
/**
 * @brief             The APP requests APB proxy to stop sending user data for flow control.
 *                    Call this function, if the APP cannot process more user data.
 *                    This is a non-blocking function.
 * @param     conn_id Please refer to #apb_proxy_data_conn_id_t
 * @return            If the result is #APB_PROXY_STATUS_OK, APB proxy will guarantee to send the data out.
 *                    If the result is #APB_PROXY_STATUS_ERROR, the cause would be: connection is not valid,
 *                    data mode is not active, or message queue is full.
 */
apb_proxy_status_t apb_proxy_stop_send_user_data(apb_proxy_data_conn_id_t conn_id);
/**
 * @brief             The APP requests APB to resume sending user data.
 *                    Call this function to process more user data in the APP.
 *                    This is a none-blocking function.
 * @param     conn_id Please refer to #apb_proxy_data_conn_id_t
 * @return            If the result is #APB_PROXY_STATUS_OK, APB proxy will guarantee to send the data out.
 *                    If the result is #APB_PROXY_STATUS_ERROR, the cause would be: connection is not valid,
 *                    data mode is not active, or message queue is full.
 */
apb_proxy_status_t apb_proxy_resume_send_user_data(apb_proxy_data_conn_id_t conn_id);
/**
 * @brief             Chech whether the current channel is in data mode state.
 * @param     apb_id  The id can be #apb_proxy_data_conn_id_t or # apb_proxy_cmd_id_t.
 * @return            If the result is true, the current channel is in data mode state.
 *                    If the result is false, the current channel is not in data mode state.
 */
bool apb_proxy_current_channel_data_mode_actived(uint32_t apb_id);
/**
 * @brief             If the current data mode has been switched to command mode temperarily,
 *                    the data mode can be switched back by calling this function.
 * @param     conn_id Please refer to #apb_proxy_data_conn_id_t
 * @return            If the result is #APB_PROXY_STATUS_OK, data mode is switched successfully.
 *                    If the result is #APB_PROXY_STATUS_ERROR, failed to switch to data mode.
 */
apb_proxy_status_t apb_proxy_resume_data_mode(apb_proxy_data_conn_id_t conn_id);
#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif/*__APB_PROXY__H__*/
