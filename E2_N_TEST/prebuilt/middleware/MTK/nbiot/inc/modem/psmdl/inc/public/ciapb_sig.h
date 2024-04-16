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
/*
 **************************************************************************/
/** \file
 * Middleware Signal definitions for the CIAPB Interface.
 **************************************************************************/

#ifndef CIAPB_SIG_H
#define CIAPB_SIG_H


/***************************************************************************
 * Nested Include Files
 ***************************************************************************/

#include <ciapb_typ.h>
#include <apbridge.h>


/** \defgroup SigCiapb CIAPB Signalling Interface
 * \ingroup SigCiex
 * \ingroup Sig
 * Signaling interface between ATCI and AP Bridge.
 * This section defines all signals exchanged between ATCI and AP Bridge,
 * hereinafter called the CIAPB interface.
 *
 * The CIAPB interface is used for two purposes:
 * - AT command exchange between the on-chip AP domain and ATCI
 * - User data connection setup
 *
 * The context of the CIAPB interface when exchanging AT commands is shown in the Figure below:
 *
 * \image html CIAPB_interface_AT_commands.png
 *
 * The context of the CIAPB interface when setting up a user data connection is shown in the Figure below:
 *
 * \image html CIAPB_interface_data_conn_setup.png
 *
 * @{
 */

/***************************************************************************
 *   Manifest Constants
 ***************************************************************************/

/** Defines maximum number of \link #CiApbTlvTag TLVs \endlink per \link #CiApbAtCommandReqTag CiApbAtCommandReq \endlink signal.
 * This value is used in \link #CiApbAtCommandReqTag CiApbAtCommandReq \endlink signals and
 * \link #CiApbAtResponseIndTag CiApbAtResponseInd \endlink signals. It defines the maximum number of
 * \link #CiApbTlvTag TLVs \endlink that can be transferred per signal.
 */
#define CIAPB_MAX_TLVS_PER_SIGNAL           (16)



/** Maximum length of AT data string per signal.
 * For debug purposes only ATCI also transmit the original AT data string.
 * This value defines the maximum length of the AT data string transmitted in a
 * \link #CiApbAtCommandReqTag CiApbAtCommandReq \endlink signal.
 */
#define CIAPB_MAX_AT_DATA_LENGTH            (128)

/***************************************************************************
 * Signals
 ***************************************************************************/

/** \defgroup SigCiapbAtregister Signals for AT command registration
 * Registration signals for AT commands that are executed on the on-chip AP domian.
 * These registration signal are used to register AT commands with ATCI.
 * Once registered these AT commands can be executed on the on-chip AP domian.
 *
 * The following MSC shows successful AT command registration:
 *\msc
 *  hscale = "2";
 *  "MUX","AP Bridge","ATCI";
 *
 *  "MUX"       -> "AP Bridge" [label="MdtUnitdataInd ( Register Command packet )",   url="\ref MdtUnitdataIndTag"];
 *  "AP Bridge" -> "ATCI"      [label="CiApbRegisterAtCommandInd",                    url="\ref CiApbRegisterAtCommandIndTag"];
 *  "AP Bridge" <- "ATCI"      [label="CiApbRegisterAtCommandRsp ( success=TRUE )",   url="\ref CiApbRegisterAtCommandRspTag"];
 *  "MUX"       <- "AP Bridge" [label="MdtUnitdataReq ( Command Registered packet )", url="\ref MdtUnitdataReqTag"];
 *\endmsc
 *
 * The following MSC shows failed AT command registration:
 *\msc
 *  hscale = "2";
 *  "MUX","AP Bridge","ATCI";
 *
 *  "MUX"       -> "AP Bridge" [label="MdtUnitdataInd ( Register Command packet )",    url="\ref MdtUnitdataIndTag"];
 *  "AP Bridge" -> "ATCI"      [label="CiApbRegisterAtCommandInd",                     url="\ref CiApbRegisterAtCommandIndTag"];
 *  "AP Bridge" <- "ATCI"      [label="CiApbRegisterAtCommandRsp ( success=FALSE )",   url="\ref CiApbRegisterAtCommandRspTag"];
 *  "MUX"       <- "AP Bridge" [label="MdtUnitdataReq ( Registration Failed packet )", url="\ref MdtUnitdataReqTag"];
 *\endmsc
 *
 * @{
 */

/** AP domain requests AT command registration.
 * This signal is used to register an AT command with ATCI for execution on the on-chip AP domian.
 */
typedef struct CiApbRegisterAtCommandIndTag
{
    /** The total AT command count which will be registered by AP in future.
     */
    Int32                       totalCmdCount;
    /** The AT command count which will be reigstered in this request.
     */
    Int32                       cmdCountInTheReq;
#ifdef ENABLE_AP_BRIDGE_UNIT_TEST
    Int32                       bufferLen;
    Int8                        pAtCmdsData[1500];
#else
    /*The buffer length which pAtCmdsData points to.*/
    Int32                       bufferLen;
    /**
     * The AT commands' string data, the buffer will be managed by ATCI.
     */
    Int8*                       pAtCmdsData;
#endif
} CiApbRegisterAtCommandInd;



/** Result of the attempt to register an AT command.
 * This signal is used to confirm or reject
 * \link #CiApbRegisterAtCommandIndTag CiApbRegisterAtCommandInd \endlink.
 */
typedef struct CiApbRegisterAtCommandRspTag
{
    /** The beginning identifier of the AT command allocated by ATCI.*/
    Int32                       baseCmdId;

    /** How many AT Commands have been registered in ATCI.*/
    Int32                       registeredCount;

    /** Result of the attempt to register an AT command with ATCI.
     * Contains the result of the attempt to register an AT command with ATCI
     * for execution on the on-chip AP domian.
     * If it is set to TRUE, the AT command is registered successfully.
     */
    Boolean                     success;
} CiApbRegisterAtCommandRsp;

/** @} */ /* End of SigCiapbAtregister group */



/** \defgroup SigCiapbAtdata Signals for AT data exchange
 * AT data signals for AT commands that are executed on the on-chip AP domian.
 * AT data signal are used to exchange AT commands and AT responses with the AP domain.
 * ATCI performs AT command parsing and syntax checks and
 * forwards the AT command parameters to the AP Bridge entity.
 * The AP Bridge entity translates the provided information into RPC calls
 * which are executed on the AP domain.
 *
 * The following MSC shows an AT command with intermediate response:
 *\msc
 *  hscale = "2";
 *  "MUX","AP Bridge","ATCI";
 *
 *  "AP Bridge" <- "ATCI"      [label="CiApbAtCommandReq ( requestType=Set command )",                     url="\ref CiApbAtCommandReqTag"];
 *  "MUX"       <- "AP Bridge" [label="MdtUnitdataReq ( AT Command packet ( Request Type=Set command ) )", url="\ref MdtUnitdataReqTag"];
 *  "MUX"       -> "AP Bridge" [label="MdtUnitdataInd ( AT Response packet ( Result Code=Proceeding ) )",  url="\ref MdtUnitdataIndTag"];
 *  "AP Bridge" -> "ATCI"      [label="CiApbAtResponseInd ( resultCode=Proceeding )",                      url="\ref CiApbAtResponseIndTag"];
 *  "MUX"       -> "AP Bridge" [label="MdtUnitdataInd ( AT Response packet ( Result Code=OK ) )",          url="\ref MdtUnitdataIndTag"];
 *  "AP Bridge" -> "ATCI"      [label="CiApbAtResponseInd ( resultCode=OK )",                              url="\ref CiApbAtResponseIndTag"];
 *\endmsc
 *
 * The following MSC shows an unsolicited response:
 *\msc
 *  hscale = "2";
 *  "MUX","AP Bridge","ATCI";
 *
 *  "MUX"       -> "AP Bridge" [label="MdtUnitdataInd ( AT Response packet ( Result Code=Unsolicited ) )", url="\ref MdtUnitdataIndTag"];
 *  "AP Bridge" -> "ATCI"      [label="CiApbAtResponseInd ( resultCode=Unsolicited )",                     url="\ref CiApbAtResponseIndTag"];
 *\endmsc
 *
 * @{
 */

/** ATCI transmits AT data to AP Bridge.
 * This signal is used to transmit AT data from ATCI to AP Bridge.
 */
typedef struct CiApbAtCommandReqTag
{
    /** The identifier of the channel.
     * The identifier is used to distinguish between AT commands sent on different channels
     * case of multi-channel support.
     */
    Int32                       channelId;

    /** For debug purposes only.
     * Array which contains the original AT data string.
     * The length of the AT data array is defined by #CIAPB_MAX_AT_DATA_LENGTH.
     */
#ifdef ENABLE_AP_BRIDGE_FEATURE_DEBUG
    Int8                        atString [CIAPB_MAX_AT_DATA_LENGTH];
#endif

    /** The identifier of the AT command.
     * Each AT command has a unique identifier on the \ref ApBridgeInterface.
     */
    Int32                       commandId;

    /** The execution type requested for this AT command.
     * The Request Type field can have the following values:
     * - 1 - Set command (+CMD=) (usually used to set configuration value(s)) (#APBRIDGE_REQUEST_TYPE_SET)
     * - 2 - Read command (+CMD?) (usually used to query current setting) (#APBRIDGE_REQUEST_TYPE_READ)
     * - 3 - Test command (+CMD=?) (usually used to query allowed parameters) (#APBRIDGE_REQUEST_TYPE_TEST)
     * - 4 - Custom command (any non-standard command format) (#APBRIDGE_REQUEST_TYPE_CUSTOM)
     *
     * These values are defined in the \ref ApBridgeInterface.
     */
    Int32                       requestType;

    /** Indicates number of TLVs provided */
    Int32                       numberOfTlvs;

    /** Array which contains the TLVs.
     * The length of the array is defined by #CIAPB_MAX_TLVS_PER_SIGNAL.
     */
    CiApbTlv                    tlv [CIAPB_MAX_TLVS_PER_SIGNAL];
} CiApbAtCommandReq;



/** AP Bridge transmits AT data to ATCI.
 * This signal is used to transmit AT data from AP Bridge to ATCI.
 */
typedef struct CiApbAtResponseIndTag
{
    /** The identifier of the channel.
     * The identifier is used to distinguish between AT commands sent on different channels
     * case of multi-channel support.
     */
    Int32                       channelId;

    /** The identifier of the AT command.
     * Each AT command has a unique identifier on the \ref ApBridgeInterface.
     */
    Int32                       commandId;

    /** Result of this AT response.
     * The Result Code field can have the following values:
     * - 1 - OK (success) (#APBRIDGE_RESULT_OK)
     * - 2 - CONNECT (data connection setup successful) (#APBRIDGE_RESULT_CONNECT)
     * - 3 - RING (ring indication) (#APBRIDGE_RESULT_RING)
     * - 4 - NO CARRIER (data connection ended/setup failure) (#APBRIDGE_RESULT_NO_CARRIER)
     * - 5 - ERROR (failure) (#APBRIDGE_RESULT_ERROR)
     * - 6 - NO DIALTONE (voice call network failure) (#APBRIDGE_RESULT_NO_DIALTONE)
     * - 7 - BUSY (busy with another command) (#APBRIDGE_RESULT_BUSY)
     * - 8 - NO ANSWER (voice call user not answering) (#APBRIDGE_RESULT_NO_ANSWER)
     * - 9 - Proceeding (intermediate response) (#APBRIDGE_RESULT_PROCEEDING)
     * - 10 - Unsolicited (unsolicited response) (#APBRIDGE_RESULT_UNSOLICITED)
     * - 11 - Customed response (#APBRIDGE_RESULT_CUSTOM_ERROR)
     *
     * These values are defined in the \ref ApBridgeInterface.
     */
    Int32                       resultCode;

    /** An error cause value in case of an error.
     * This can be, for example, a CME error code.
     * The Error Code field is set to 0xFFFFFFFF if not used (#APBRIDGE_ERROR_NOT_USED).
     * Error cause values are defined in the \ref ApBridgeInterface.
     */
    Int32                       errorCode;

    /** Indicates number of TLVs provided */
    Int32                       numberOfTlvs;

    /** Array which contains the TLVs.
     * The length of the array is defined by #CIAPB_MAX_TLVS_PER_SIGNAL.
     */
    CiApbTlv                    tlv [CIAPB_MAX_TLVS_PER_SIGNAL];
} CiApbAtResponseInd;

/** @} */ /* End of SigCiapbAtdata group */



/** \defgroup SigCiapbDataconn User Data Connection Setup signals
 * Signals that manage setting up a user data connection between
 * an external AP and the on-chip AP domain.
 * These signals configure creation and destruction of a data connection
 * between an external AP and the AP Bridge.
 *
 * The following MSC is an example of a user data connection setup:
 *\msc
 *  "MUX","AP Bridge","ATCI";
 *
 *  "AP Bridge" <- "ATCI"      [label="CiApbOpenDataConnReq", url="\ref CiApbOpenDataConnReqTag"];
 *  "MUX"       <- "AP Bridge" [label="MdtCapabilitiesReq",   url="\ref MdtCapabilitiesReqTag"];
 *  "MUX"       -> "AP Bridge" [label="MdtSetupInd",          url="\ref MdtSetupIndTag"];
 *  ...;
 *\endmsc
 *
 * The following MSC is an example of a user data connection destruction:
 *\msc
 *  "MUX","AP Bridge","ATCI";
 *
 *  "AP Bridge" <- "ATCI"      [label="CiApbCloseDataConnReq", url="\ref CiApbCloseDataConnReqTag"];
 *  "MUX"       <- "AP Bridge" [label="MdtCloseDownReq",       url="\ref MdtCloseDownReqTag"];
 *\endmsc
 *
 *
 * @{
 */

/** Close user data connection.
 * This signal is used to close the user data connection. ATCI gets informed by MUX
 * when the connection is successfully closed.
 */
typedef struct CiApbCloseDataConnReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;
} CiApbCloseDataConnReq;

/** Open user data connection.
 * This signal is used to open the user data connection. ATCI gets informed by MUX
 * when the connection is successfully opened.
 */
typedef struct CiApbOpenDataConnReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;
} CiApbOpenDataConnReq;

/** AP Bridge sends this flow control signal to itself when it receives flow control
* callback message from L4mm.
*/
typedef struct ApbL4mmFlowControlIndTag
{
    /** Transaction ID that is used by client to determine
     * if the callback event shall be handled or not. Only events which
     * transaction ID is bigger than the biggest transaction ID received
     * so far shall be handled.
     */
    Int8 transactionId;

    /** isFillingUp TRUE if AP Bridge Pool buffer is getting full (event 1 triggered),
     * and FALSE if the memory occupation is decreasing (event 2 triggered).
     */
    Boolean isFillingUp;
} ApbL4mmFlowControlInd;

/** @} */ /* End of SigCiapbDataconn group */

/** AP Bridge sends the indication to AP Bridge AP domain..
*/
typedef struct ApbDataModeCloseIndTag
{
    Int32 channelNumber;
} ApbDataModeCloseInd;

/** AP Bridge sends the indication to AP Bridge AP domain..
*/
typedef struct ApbDataModeTempDeactivatedIndTag
{
    Int32 channelNumber;
} ApbDataModeTempDeactivatedInd;

/** AP Bridge sends the indication to AP Bridge AP domain..
*/
typedef struct ApbDataModeResumedIndTag
{
    Int32 channelNumber;
} ApbDataModeResumedInd;

/** AP Bridge sends the indication to AP Bridge AP domain..
*/
typedef struct ApbDataModeResumeReqTag
{
    Int32 channelNumber;
} ApbDataModeResumeReq;

/** @} */ /* End of SigCiapbDataconn group */


/** @} */ /* End of SigCiapb group */

#endif /* !CIAPB_SIG_H */

/* End of ciapb_sig.h */


