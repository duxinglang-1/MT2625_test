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

#ifndef __APBRIDGE_H__
#define __APBRIDGE_H__

#ifdef __cplusplus
extern "C" {
#endif

/** \defgroup ApBridgeInterface AP-MD AP Bridge Interface
 * This section introduces the AP Bridge interface which is used between Application Processor (AP) and Modem (MD).
 * The AP Bridge interface is used on an RPC channel between the on-chip AP domain and the MD doamin.
 *
 * AP Bridge is used for the following purposes:
 * - Enable AP domain specific AT commands
 *   - AT command parsing is done by Modem ATCI
 *   - AT command execution is done by the on-chip AP domain
 * - Enable user data exchange between external AP and on-chip AP domain
 *   - A UART connection is used to connect to an external AP
 *   - User data for the on-chip AP domain is routed through AP Bridge as the UART is connected to the Modem
 *
 * Dynamic switching between AT command exchange and user data exchange is supported:
 * - An AP domain specific AT command is used for this
 * - The interface also supports more than one channel for user data exchange
 *
 * The following MSC shows successful AT command registration:
 *\msc
 *  "AP","MD";
 *
 *  "AP" -> "MD" [label="Register Command", url="\ref APBRIDGE_Register_Command"];
 *  "AP" <- "MD" [label="Command Registered", url="\ref APBRIDGE_Command_Registered"];
 *\endmsc
 *
 * The following MSC shows failed AT command registration:
 *\msc
 *  "AP","MD";
 *
 *  "AP" -> "MD" [label="Register Command", url="\ref APBRIDGE_Register_Command"];
 *  "AP" <- "MD" [label="Registration Failed", url="\ref APBRIDGE_Registration_Failed"];
 *\endmsc
 *
 * The following MSC shows an AT command with intermediate response:
 *\msc
 *  "AP","MD";
 *
 *  "AP" <- "MD" [label="AT Command ( Request Type=Set command )", url="\ref APBRIDGE_AT_Command"];
 *  "AP" -> "MD" [label="AT Response ( Result Code=Proceeding )", url="\ref APBRIDGE_AT_Response"];
 *  "AP" -> "MD" [label="AT Response ( Result Code=OK )", url="\ref APBRIDGE_AT_Response"];
 *\endmsc
 *
 * The following MSC shows an unsolicited AT response:
 *\msc
 *  "AP","MD";
 *
 *  "AP" -> "MD" [label="AT Response ( Result Code=Unsolicited )", url="\ref APBRIDGE_AT_Response"];
 *\endmsc
 *
 * The following MSC shows user data exchange:
 *\msc
 *  "AP","MD";
 *
 *  "AP" <- "MD" [label="User Data", url="\ref APBRIDGE_User_Data"];
 *  "AP" <- "MD" [label="User Data", url="\ref APBRIDGE_User_Data"];
 *  "AP" -> "MD" [label="Xoff", url="\ref APBRIDGE_Xoff"];
 *  ...;
 *  "AP" -> "MD" [label="Xon", url="\ref APBRIDGE_Xon"];
 *  "AP" <- "MD" [label="User Data", url="\ref APBRIDGE_User_Data"];
 *  "AP" <- "MD" [label="User Data", url="\ref APBRIDGE_User_Data"];
 *\endmsc
 *
 *
 * @{
 */



/** \defgroup ApBridgePacketFormat Packet Format
 * This section describes the format of the packets exchanged on the AP Bridge interface.
 *
 * \section APBRIDGE_Packet_Format Generic Packet Format
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                             Code                              |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Length                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |    Data ...
 *          +-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - All fields are transmitted in little-endian.
 * - Code – The Code field identifies the kind of AP Bridge packet.
 *             The Code field can have the following values:
 *              - 1 - AT Command (sent from MD to AP) (#APBRIDGE_CODE_AT_COMMAND)
 *              - 2 - AT Response (sent from AP to MD) (#APBRIDGE_CODE_AT_RESPONSE)
 *              - 3 - User Data (#APBRIDGE_CODE_USER_DATA)
 *              - 4 - Xon (#APBRIDGE_CODE_XON)
 *              - 5 - Xoff (#APBRIDGE_CODE_XOFF)
 *              - 6 - Register Command (sent from AP to MD) (#APBRIDGE_CODE_REGISTER_COMMAND)
 *              - 7 - Command Registered (sent from MD to AP) (#APBRIDGE_CODE_COMMAND_REGISTERED)
 *              - 8 - Registration Failed (sent from MD to AP) (#APBRIDGE_CODE_REGISTRATION_FAILED)
 * - Length – Total packet length including this header (in bytes)
 * - Data – Packet data (this field is optional and can be of arbitrary length)
 *
 *
 * \section APBRIDGE_AT_Command AT Command
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Code=1                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Length                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                          Channel ID                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                          Command ID                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                         Request Type                          |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |    TLVs ...
 *          +-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - Length – Total packet length including this header (in bytes)
 * - Channel ID – Identifier of the channel in case of multi-channel support.
 *                   Channel IDs are assigned by MD.
 *                   This can be, for example, the channelNumber to the external AP.
 * - Command ID – The identifier of the AT command.
 * - Request Type – The execution type requested for this AT command.
 *                     The Request Type field can have the following values:
 *                     - 1 - Set command (+CMD=) (usually used to set configuration value(s)) (#APBRIDGE_REQUEST_TYPE_SET)
 *                     - 2 - Read command (+CMD?) (usually used to query current setting) (#APBRIDGE_REQUEST_TYPE_READ)
 *                     - 3 - Test command (+CMD=?) (usually used to query allowed parameters) (#APBRIDGE_REQUEST_TYPE_TEST)
 *                     - 4 - Custom command (any non-standard command format) (#APBRIDGE_REQUEST_TYPE_CUSTOM)
 * - TLV(s) – Parameters provided with the AT command.
 *               The order of the TLVs is the order of parameters within the AT command. \see ApBridgeTlvFormat
 *
 *
 * \section APBRIDGE_AT_Response AT Response
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Code=2                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Length                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                          Channel ID                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                          Command ID                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                          Result Code                          |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                          Error Code                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |    TLVs ...
 *          +-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - Length – Total packet length including this header (in bytes)
 * - Channel ID – Identifier of the channel in case of multi-channel support.
 *                   This is usually a copy to the value in the AT Command packet.
 * - Command ID – The identifier of the AT command.
 * - Result Code – The Result Code field can have the following values:
 *                    - 1 - OK (success) (#APBRIDGE_RESULT_OK)
 *                    - 2 - CONNECT (data connection setup successful) (#APBRIDGE_RESULT_CONNECT)
 *                    - 3 - RING (ring indication) (#APBRIDGE_RESULT_RING)
 *                    - 4 - NO CARRIER (data connection ended/setup failure) (#APBRIDGE_RESULT_NO_CARRIER)
 *                    - 5 - ERROR (failure) (#APBRIDGE_RESULT_ERROR)
 *                    - 6 - NO DIALTONE (voice call network failure) (#APBRIDGE_RESULT_NO_DIALTONE)
 *                    - 7 - BUSY (busy with another command) (#APBRIDGE_RESULT_BUSY)
 *                    - 8 - NO ANSWER (voice call user not answering) (#APBRIDGE_RESULT_NO_ANSWER)
 *                    - 9 - Proceeding (intermediate response) (#APBRIDGE_RESULT_PROCEEDING)
 *                    - 10 - Unsolicited (unsolicited response) (#APBRIDGE_RESULT_UNSOLICITED)
 * - Error Code – An error cause value in case of an error or 0xFFFFFFFF if not used (#APBRIDGE_ERROR_NOT_USED).
 *                   This can be, for example, a CME error code.
 * - TLV(s) – Parameters provided with the AT response.
 *               The order of the TLVs is the order of parameters within the AT response. \see ApBridgeTlvFormat
 *
 *
 * \section APBRIDGE_User_Data User Data
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Code=3                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Length                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                          Channel ID                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |    User Data ...
 *          +-+-+-+-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - Length – Total packet length including this header (in bytes)
 * - Channel ID – Identifier of the channel in case of multi-channel support.
 * - User Data – The user data.
 *
 *
 * \section APBRIDGE_Xon Xon
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Code=4                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                           Length=12                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                          Channel ID                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - Channel ID – Identifier of the channel in case of multi-channel support.
 *
 *
 * \section APBRIDGE_Xoff Xoff
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Code=5                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                           Length=12                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                          Channel ID                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - Channel ID – Identifier of the channel in case of multi-channel support.
 *
 *
 * \section APBRIDGE_Register_Command Register Command
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Code=6                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Length                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                          Command ID                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |    String TLV ...
 *          +-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - Length – Total packet length including this header (in bytes)
 * - Command ID – The identifier of the AT command. Each AT command has a unique identifier.
 *                   Command IDs are assigned by AP.
 * - String TLV – The AT command in string format.
 *
 *
 * \section APBRIDGE_Command_Registered Command Registered
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Code=7                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                           Length=12                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                          Command ID                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - Command ID – The identifier of the AT command.
 *
 *
 * \section APBRIDGE_Registration_Failed Registration Failed
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Code=8                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                           Length=12                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                          Command ID                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - Command ID – The identifier of the AT command.
 *
 * @{
 */

/* Code field */
#define APBRIDGE_CODE_AT_COMMAND                   (1)     /**< AT Command (sent from MD to AP) */
#define APBRIDGE_CODE_AT_RESPONSE                  (2)     /**< AT Response (sent from AP to MD) */
#define APBRIDGE_CODE_USER_DATA                    (3)     /**< User Data */
#define APBRIDGE_CODE_XON                          (4)     /**< Xon */
#define APBRIDGE_CODE_XOFF                         (5)     /**< Xoff */
#define APBRIDGE_CODE_REGISTER_COMMAND             (6)     /**< Register Command (sent from AP to MD) */
#define APBRIDGE_CODE_COMMAND_REGISTERED           (7)     /**< Command Registered (sent from MD to AP) */
#define APBRIDGE_CODE_REGISTRATION_FAILED          (8)     /**< Registration Failed (sent from MD to AP) */
#define APBRIDGE_CODE_DATA_MODE_CLOSED_IND         (9)     /**< Data mode closed indication (sent from MD to AP) */
#define APBRIDGE_CODE_DATA_MODE_TEMP_DEACTIVE_IND  (10)     /**< Data mode temporally deactived indication (sent from MD to AP) */
#define APBRIDGE_CODE_DATA_MODE_RESUME_REQ         (11)     /**< Resume the data mode (sent from AP to MD) */
#define APBRIDGE_CODE_DATA_MODE_CLOSE_REQ          (12)     /**< Data mode close request (sent from AP to MD) */
#define APBRIDGE_CODE_DATA_MODE_RESUMED_IND        (13)     /**< Resumed the data mode indication(sent from MD to AP) */

/* Command ID field */
#define APBRIDGE_COMMAND_INVALID            (0)     /**< Invalid AT Command */

/* Request Type field */
#define APBRIDGE_REQUEST_TYPE_ACTION        (1)     /**< Action command(+CMD)*/
#define APBRIDGE_REQUEST_TYPE_SET           (2)     /**< Set command (+CMD=) (usually used to set configuration value(s)) */
#define APBRIDGE_REQUEST_TYPE_READ          (3)     /**< Read command (+CMD?) (usually used to query current setting) */
#define APBRIDGE_REQUEST_TYPE_TEST          (4)     /**< Test command (+CMD=?) (usually used to query allowed parameters) */
#define APBRIDGE_REQUEST_TYPE_CUSTOM        (5)     /**< Custom command (any non-standard command format) */

/* Result Code field */
#define APBRIDGE_RESULT_OK                  (1)     /**< OK (success) */
#define APBRIDGE_RESULT_CONNECT             (2)     /**< CONNECT (data connection setup successful) */
#define APBRIDGE_RESULT_RING                (3)     /**< RING (ring indication) */
#define APBRIDGE_RESULT_NO_CARRIER          (4)     /**< NO CARRIER (data connection ended/setup failure) */
#define APBRIDGE_RESULT_ERROR               (5)     /**< ERROR (failure) */
#define APBRIDGE_RESULT_NO_DIALTONE         (6)     /**< NO DIALTONE (voice call network failure) */
#define APBRIDGE_RESULT_BUSY                (7)     /**< BUSY (busy with another command) */
#define APBRIDGE_RESULT_NO_ANSWER           (8)     /**< NO ANSWER (voice call user not answering) */
#define APBRIDGE_RESULT_PROCEEDING          (9)     /**< Proceeding (intermediate response) */
#define APBRIDGE_RESULT_UNSOLICITED         (10)    /**< Unsolicited (unsolicited response) */
#define APBRIDGE_RESULT_CUSTOM_CONNECT      (11)    /**< The user fills hte customed response output when data connection sets up. */
#define APBRIDGE_RESULT_CUSTOM_ERROR        (12)    /**< Custom error final response */
/* Error Code field */
#define APBRIDGE_ERROR_NOT_USED                                 (0xFFFFFFFF)        /**< Error Code field not used */
#define APBRIDGE_ERROR_CME_PHONE_FAILURE                        (0)                 /**< phone failure */
#define APBRIDGE_ERROR_CME_OPERATION_NOT_ALLOWED                (3)                 /**< operation not allowed */
#define APBRIDGE_ERROR_CME_OPERATION_NOT_SUPPORTED              (4)                 /**< operation not supported */
#define APBRIDGE_ERROR_CME_SIM_NOT_INSERTED                     (10)                /**< SIM not inserted */
#define APBRIDGE_ERROR_CME_INCORRECT_PASSWORD                   (16)                /**< incorrect password */
#define APBRIDGE_ERROR_CME_MEMORY_FULL                          (20)                /**< memory full */
#define APBRIDGE_ERROR_CME_MEMORY_FAILURE                       (23)                /**< memory failure */
#define APBRIDGE_ERROR_CME_LONG_TEXT                            (24)                /**< text string too long */
#define APBRIDGE_ERROR_CME_INVALID_TEXT_CHARS                   (25)                /**< invalid characters in text string */
#define APBRIDGE_ERROR_CME_NO_NETWORK_SERVICE                   (30)                /**< no network service */
#define APBRIDGE_ERROR_CME_NETWORK_TIMEOUT                      (31)                /**< network timeout */
#define APBRIDGE_ERROR_CME_EMERGENCY_ONLY                       (32)                /**< network not allowed - emergency calls only */
#define APBRIDGE_ERROR_CME_UNKNOWN                              (100)               /**< unknown */
#define APBRIDGE_ERROR_CME_PSD_SERVICES_NOT_ALLOWED             (107)               /**< psd services not allowed */
#define APBRIDGE_ERROR_CME_PLMN_NOT_ALLOWED                     (111)               /**< plmn not allowed */
#define APBRIDGE_ERROR_CME_LOCATION_AREA_NOT_ALLOWED            (112)               /**< location area not allowed */
#define APBRIDGE_ERROR_CME_ROAMING_NOT_ALLOWED                  (113)               /**< roaming not allowed in this location area */
#define APBRIDGE_ERROR_CME_SERVICE_OPTION_NOT_SUPPORTED         (132)               /**< service option not supported */
#define APBRIDGE_ERROR_CME_SERVICE_OPTION_NOT_SUBSCRIBED        (133)               /**< requested service option not subscribed */
#define APBRIDGE_ERROR_CME_SERVICE_OPTION_OUT_OF_ORDER          (134)               /**< service option temporarily out of order */
#define APBRIDGE_ERROR_CME_UNSPECIFIED_PSD_ERROR                (148)               /**< unspecified psd error */
#define APBRIDGE_ERROR_CME_PDP_AUTHENTIFICATION_ERROR           (149)               /**< PDP authentication failure */

/** @} */ /* End of ApBridgePacketFormat group */

/** \defgroup ApBridgeTlvFormat TLV Format
 * This section describes the format of the TLVs exchanged on the AP Bridge interface.
 *
 * \section APBRIDGE_TLV_Format Generic TLV Format
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *                                                              +-+-+-+-+-+-+
 *                                                           ... Padding    |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                             Type                              |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Length                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |    Data ...
 *          +-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - All fields are transmitted in little-endian.
 * - Padding – Zero to 3 bytes of value zero added in front of the TLV
 *                in order to make the TLV start at a 32bit boundary.
 *                The Type field is considered to be the first field of each TLV.
 * - Type – The Type field indicates the type of AP Bridge TLV.
 *             The Type field can have the following values:
 *              - 1 - Integer (#APBRIDGE_TLV_TYPE_INTEGER)
 *              - 2 - String (#APBRIDGE_TLV_TYPE_STRING)
 *              - 3 - Void (parameter not provided on AT interface) (#APBRIDGE_TLV_TYPE_VOID)
 * - Length – Total length of the TLV (in bytes) including Type field, Length field and optional data
 *               but excluding preceding padding
 * - Data – Configuration data (this field is optional and can be of arbitrary length)
 *
 *
 * \section APBRIDGE_TLV_Integer Integer
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Type=1                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Length                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |    Integer ...
 *          +-+-+-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - Length – Total length of the TLV (in bytes) including Type field, Length field and Integer field
 * - Integer – Either a 32bit signed integer or a 64bit signed integer
 *
 *
 * \section APBRIDGE_TLV_String String
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Type=2                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Length                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |    String ...
 *          +-+-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - Length – Total length of the TLV (in bytes) including Type field, Length field and String field
 * - String – Zero or more bytes of a stream of bytes
 *
 *
 * \section APBRIDGE_TLV_Void Void
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Type=3                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                           Length=8                            |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *    \endcode
 *
 *
 * @{
 */

/* Type field */
#define APBRIDGE_TLV_TYPE_INTEGER           (1)     /**< Integer */
#define APBRIDGE_TLV_TYPE_STRING            (2)     /**< String */
#define APBRIDGE_TLV_TYPE_VOID              (3)     /**< Void (parameter not provided on AT interface) */

/** @} */ /* End of ApBridgeTlvFormat group */

/** @} */ /* End of ApBridgeInterface group */

#ifdef __cplusplus
}
#endif


#endif /* !__APBRIDGE_H__ */

