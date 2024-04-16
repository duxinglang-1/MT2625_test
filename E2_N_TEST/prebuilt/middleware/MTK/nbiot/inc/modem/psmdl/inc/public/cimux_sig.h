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
 * Middleware Signal definitions for the CIMUX Interface.
 **************************************************************************/

#ifndef CIMUX_SIG_H
#define CIMUX_SIG_H

#if defined (UPGRADE_GENERIC_MUX)
#if defined (UPGRADE_NASMDL2)

/***************************************************************************
 * Nested Include Files
 ***************************************************************************/

#include <mux_cfg.h>
#include <cimux_typ.h>

/** \defgroup SigCimux CIMUX Signalling Interface
 * \ingroup SigCiex
 * \ingroup Sig
 * Signaling interface between ATCI and MUX.
 * This section defines all signals exchanged between ATCI and MUX, hereinafter called the CIMUX
 * interface.
 *
 * The context of the CIMUX interface is shown in the Figure below:
 *
 * \image html Middleware2_Interfaces.png
 * @{
 */

/***************************************************************************
* Signals
***************************************************************************/

/** \defgroup SigCimuxAtdata Signals for AT data exchange
 * AT data signals and their associated flow control signals.
 * Each AT data signal requires a flow control signal as an acknowledgement before the next AT data
 * signal can be sent.
 *
 * The following MSC shows this for AT data from ATCI to MUX:
 *\msc
 * msc {
 *  "MUX","ATCI";
 *
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataReq ( channelNumber, ..., data )"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataCnf ( channelNumber )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataReq ( channelNumber, ..., data )"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataCnf ( channelNumber )"];
 * }
 *\endmsc
 *
 * The following MSC shows the same principle for AT data from MUX to ATCI:
 *\msc
 * msc {
 *  "MUX","ATCI";
 *
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataInd ( channelNumber, ..., data )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataRsp ( channelNumber )"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataInd ( channelNumber, ..., data )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataRsp ( channelNumber )"];
 * }
 *\endmsc
 *
 * @{
 */

/** ATCI transmits AT data to MUX.
 * This signal is used to transmit AT data from ATCI to MUX.
 * ATCI requires a confirm signal before it sends the next data signal.
 */
typedef struct CiMuxAtDataReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;

    /** Data is RC */
    Boolean                     isUrc;
    /** Data is SMS URC */
    Boolean                     isSmsUrc;

    /** Length of the relevant AT data in the signal. */
    Int32                       length;

    /** Array which contains the AT data.
     * The length of the AT data array is defined by #CIMUX_MAX_AT_DATA_LENGTH.
     */
    Int8                        data [CIMUX_MAX_AT_DATA_LENGTH];
} CiMuxAtDataReq;



/** MUX confirms transmission of AT data.
 * This signal is sent by MUX to ATCI to acknowledge successful transmission of AT data. Each
 * \link #CiMuxAtDataReqTag CiMuxAtDataReq \endlink needs to be acknowledged by a
 * \a CiMuxAtDataCnf before the next \link #CiMuxAtDataReqTag CiMuxAtDataReq \endlink can be sent.
 */
typedef struct CiMuxAtDataCnfTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;
} CiMuxAtDataCnf;



/** MUX transmits AT data to ATCI.
 * This signal is used to transmit AT data from MUX to ATCI.
 * MUX requires a response signal before it sends the next data signal.
 */
typedef struct CiMuxAtDataIndTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;

    /** Length of the relevant AT data in the signal. */
    Int32                       length;

    /** Array which contains the AT data.
     * The length of the AT data array is defined by #CIMUX_MAX_AT_DATA_LENGTH.
     */
    Int8                        data [CIMUX_MAX_AT_DATA_LENGTH];
} CiMuxAtDataInd;



/** ATCI responds after processing of AT data.
 * This signal is sent by ATCI to MUX to acknowledge successful processing of AT data. Each
 * \link #CiMuxAtDataIndTag CiMuxAtDataInd \endlink needs to be acknowledged by a
 * \a CiMuxAtDataRsp before the next \link #CiMuxAtDataIndTag CiMuxAtDataInd \endlink can be sent.
 */
typedef struct CiMuxAtDataRspTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;
} CiMuxAtDataRsp;

/** @} */ /* End of SigCimuxAtdata group */



/** \defgroup SigCimuxDataconn Routing signals
 * Signals that switch between COMMAND and DATA mode of a channel.
 * These signals change the routing of data. MUX performs this routing.
 *
 * The following MSC is an example of such a change of routing:
 *\msc
 * msc {
 *  hscale = "1.75";
 *  "TE","MUX","ATCI";
 *
 *  ---              [label="-- COMMAND mode --"];
 *  "TE"   -> "MUX"  [label="ATD..."];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataInd ( ATD... )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataRsp ( )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataReq ( CONNECT )"];
 *  "TE"   <- "MUX"  [label="CONNECT"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataCnf ( )"];
 *  "MUX"  <- "ATCI" [label="CiMuxOpenDataConnReq ( )"];
 *  "TE"   <- "MUX"  [label="DCD line ON"];
 *  "MUX"  -> "ATCI" [label="CiMuxOpenDataConnCnf ( success=TRUE )"];
 *  ---              [label="-- DATA mode --"];
 *  ...;
 *  "TE"   -> "MUX"  [label="+++"];
 *  "MUX"  -> "ATCI" [label="CiMuxSwitchedToCmdModeInd ( )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataReq ( OK )"];
 *  "TE"   <- "MUX"  [label="OK"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataCnf ( )"];
 *  ---              [label="-- COMMAND mode --"];
 *  ...;
 *  "TE"   -> "MUX"  [label="ATO"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataInd ( ATO )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataRsp ( )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataReq ( OK )"];
 *  "TE"   <- "MUX"  [label="OK"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataCnf ( )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtoCmdReq ( )"];
 *  ---              [label="-- DATA mode --"];
 *  ...;
 *  "MUX"  <- "ATCI" [label="CiMuxCloseDataConnReq ( )"];
 *  "TE"   <- "MUX"  [label="DCD line OFF"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataReq ( NO CARRIER )"];
 *  "TE"   <- "MUX"  [label="NO CARRIER"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataCnf ( )"];
 *  ---              [label="-- COMMAND mode --"];
 * }
 *\endmsc
 *
 * The channel is in COMMAND mode. The TE sends an ATD command to switch into DATA mode. The DCD
 * line is set to ON condition to indicate that the data connection is established. The TE sends
 * an escape sequence to temporarily switch back to COMMAND mode. The DCD line stays in ON
 * condition since the data connection is still alive. The TE sends an ATO command to switch back
 * to DATA mode. The communication peer hangs up. The data connection is terminated and the DCD
 * line is set back to OFF condition.
 *
 * @{
 */

/** Return to DATA mode after ATO.
 * This signal is used to perform the ATO command operation, switching from COMMAND mode back to
 * the appropriate DATA mode. The data connection settings provided via
 * \link #CiMuxOpenDataConnReqTag CiMuxOpenDataConnReq \endlink are re-applied for the data
 * connection.
 */
typedef struct CiMuxAtoCmdReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;
} CiMuxAtoCmdReq;



/** Close data connection and return to COMMANMD mode.
 * This signal is used to Close the data connection on a specific channel. The channel returns to
 * COMMAND mode as a result of this signal being processed by MUX.
 */
typedef struct CiMuxCloseDataConnReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;
} CiMuxCloseDataConnReq;



/** Indication that the data connection is closed and the channel is back in COMMAND mode.
 * This signal is used to indicate to ATCI that the the peer host has requested to close the data
 * connection on this channel (e.g. on reception of a DTR drop when
 * \link #ChannelConfOptionsTag::actionOnDtrDrop actionOnDtrDrop \endlink is set to
 * \link #ActionOnDtrDropTag::CLOSE_ON_DTR_DROP CLOSE_ON_DTR_DROP \endlink).
 * As a result the channel has switched from DATA mode to COMMAND mode. The user can enter
 * AT commands now.
 */
typedef struct CiMuxClosedDataConnIndTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;
} CiMuxClosedDataConnInd;



/** Open data connection and enter DATA mode.
 * This signal is used to open a data connection on the specified channel. If the command can be
 * processed successfully, the channel will switch from COMMAND mode to DATA mode.
 */
typedef struct CiMuxOpenDataConnReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;

    /** Data connection options.
     * The data connection options are only valid if #dataConnType is set to
     * \link #DataConnTypeTag::PSD_PPP PSD_PPP \endlink.
     */
    DataConnOptions             dataConnOptions;

    /** Data connection type.
     * This parameter determines the data connection type.
     */
    DataConnType                dataConnType;
} CiMuxOpenDataConnReq;



/** Result of the attempt to open a data connection.
 * This signal is used to confirm or reject \link #CiMuxOpenDataConnReqTag CiMuxOpenDataConnReq
 * \endlink.
 */
typedef struct CiMuxOpenDataConnCnfTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;

    /** Result of the attempt to open a data connection.
     * Contains the result of the attempt to open the data connection. If it is set to TRUE, the
     * channel is in DATA mode.
     */
    Boolean                     success;
} CiMuxOpenDataConnCnf;



/** Indication that the channel is back in COMMAND mode but the data connection must be kept alive.
 * This signal is used to indicate to ATCI that the channel has switched from DATA mode to COMMAND
 * mode (as a result of escape sequence being detected or DTR being dropped by the peer Host). The
 * user can enter AT commands now. However, the data connection is still alive and the user can
 * switch back to DATA mode by sending an ATO command
 * (see \link #CiMuxAtoCmdReqTag CiMuxAtoCmdReq \endlink).
 */
typedef struct CiMuxSwitchedToCmdModeIndTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;
} CiMuxSwitchedToCmdModeInd;

/** @} */ /* End of SigCimuxDataconn group */



/** \defgroup SigCimuxChannel Channel control signals
 * Signals for channel creation, destruction and configuration.
 * This section defines the signals that control creation, destruction and configuration of a
 * channel.
 *
 * The following MSC is an example for creating and destroying channels:
 *\msc
 * msc {
 *  hscale = "1.8";
 *  "TE","MUX","ATCI";
 *
 *  ---              [label="-- both sides run 27.010 multiplexer --"];
 *  "TE"   -> "MUX"  [label="SABM ( DLCI=1 )"];
 *  "MUX"  -> "ATCI" [label="CiMuxChannelEnableInd ( channelNumber=0, ... )"];
 *  "MUX"  <- "ATCI" [label="CiMuxChannelEnableRsp ( channelNumber=0, ..., channelAccept=TRUE )"];
 *  "TE"   <- "MUX"  [label="UA ( DLCI=1 )"];
 *  "TE"   -> "MUX"  [label="SABM ( DLCI=2 )"];
 *  "MUX"  -> "ATCI" [label="CiMuxChannelEnableInd ( channelNumber=1, ... )"];
 *  "MUX"  <- "ATCI" [label="CiMuxChannelEnableRsp ( channelNumber=1, ..., channelAccept=TRUE )"];
 *  "TE"   <- "MUX"  [label="UA ( DLCI=2 )"];
 *  "TE"   -> "MUX"  [label="SABM ( DLCI=3 )"];
 *  "MUX"  -> "ATCI" [label="CiMuxChannelEnableInd ( channelNumber=2, ... )"];
 *  "MUX"  <- "ATCI" [label="CiMuxChannelEnableRsp ( channelNumber=2, ..., channelAccept=FALSE )"];
 *  "TE"   <- "MUX"  [label="DM ( DLCI=3 )"];
 *  "TE"   -> "MUX"  [label="DTR line drop"];
 *  "MUX"  -> "ATCI" [label="CiMuxChannelDisabledInd ( channelNumber=1 )"];
 *  "MUX"  <- "ATCI" [label="CiMuxChannelDisabledRsp ( channelNumber=1 )"];
 *  "MUX"  -> "ATCI" [label="CiMuxChannelDisabledInd ( channelNumber=0 )"];
 *  "MUX"  <- "ATCI" [label="CiMuxChannelDisabledRsp ( channelNumber=0 )"];
 *  "TE"   <- "MUX"  [label="DCD line OFF"];
 *  "MUX"  -> "ATCI" [label="CiMuxChannelEnableInd ( channelNumber=0, ... )"];
 *  "MUX"  <- "ATCI" [label="CiMuxChannelEnableRsp ( channelNumber=0, ..., channelAccept=TRUE )"];
 *  ---              [label="-- COMMAND mode --"];
 * }
 *\endmsc
 *
 * Both sides run a 27.010 multiplexer. MUX enables a new channel for each logical channel the TE
 * establishes. ATCI must accept the newly enabled channel by setting
 * \link #CiMuxChannelEnableRspTag::channelAccept channelAccept \endlink to TRUE. If ATCI is unable
 * to handle the new channel it sets \link #CiMuxChannelEnableRspTag::channelAccept channelAccept
 * \endlink to FALSE and MUX will reject the establishment of the channel. The TE drops the DTR
 * line to switch back to COMMAND mode. MUX, therefore, stops using the 27.010 multiplexer
 * protocol and disables all logical channels. MUX also sets the DCD line back to OFF condition.
 * Now the channel is available for AT commands again. MUX, therefore, enables the channel to
 * ATCI. A channel number of a previously disabled channel can be reused for any new channel to be
 * enabled. Now the channel is in COMMAND mode again.
 *
 * @{
 */

/** Triggers the destruction of a channel.
 * This signal is sent by MUX to ATCI to disable a previously enabled channel. On reception of
 * this signal ATCI will remove this channel from its data base and respond with a
 * \link #CiMuxChannelDisabledRspTag CiMuxChannelDisabledRsp \endlink signal.
 */
typedef struct CiMuxChannelDisabledIndTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;
} CiMuxChannelDisabledInd;



/** ATCI finished the destruction of a channel.
 * ATCI sends this signal to MUX on reception of a
 * \link #CiMuxChannelDisabledIndTag CiMuxChannelDisabledInd \endlink signal in order to
 * indicated that is has finished removing the channel from its data base.
 */
typedef struct CiMuxChannelDisabledRspTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;
} CiMuxChannelDisabledRsp;



/** Indicates the creation of a new channel.
 * This signal is sent by MUX to enable a channel. ATCI handles enabled channels only.
 */
typedef struct CiMuxChannelEnableIndTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;

    /** Communication port settings.
     * Contains the current communication port settings of the channel.
     */
    ComPortSettings             comPortSettings;

    /** Channel Configuration Options.
     * Contains the current channel configuration options of the channel.
     */
    ChannelConfOptions          channelConfOptions;

    /* Indicates if channel connection is of type External  */
    Boolean                     isExtConnection;
} CiMuxChannelEnableInd;



/** Result of the attempt to create a new channel.
 * This signal is sent by ATCI to acknowledge or reject \link #CiMuxChannelEnableIndTag
 * CiMuxChannelEnableInd \endlink. If ATCI acknowledges the new channel this signal is sent when
 * ATCI has completed setting up the channel and when it is ready to receive
 * \link #CiMuxAtDataIndTag CiMuxAtDataInd \endlink signals on it.
 */
typedef struct CiMuxChannelEnableRspTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;

    /** Communication port settings.
     * Contains the requested communication port settings of the channel.
     */
    ComPortSettings             comPortSettings;

    /** Channel Configuration Options.
     * Contains the requested channel configuration options of the channel.
     */
    ChannelConfOptions          channelConfOptions;

    /** Result of the attempt to create a new channel.
     * Contain the result of the attempt to create a new channel. If \a channelAccept is set to
     * TRUE, ATCI is able to handle the new channel. If \a channelAccept is set to FALSE, ATCI
     * rejects handling the new channel and MUX closes the channel.
     */
    Boolean                     channelAccept;
} CiMuxChannelEnableRsp;



/** Configuration of a channel.
 * This signal is used to change communication port settings and channel configuration options of
 * a channel.
 */
typedef struct CiMuxConfigureChannelReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;

    /** Communication port settings.
     * Contains the requested communication port settings of the channel.
     */
    ComPortSettings             comPortSettings;

    /** Channel Configuration Options.
     * Contains the requested channel configuration options of the channel.
     */
    ChannelConfOptions          channelConfOptions;
} CiMuxConfigureChannelReq;



/** Set Ring Indicator OFF.
 * This signal is used by ATCI to set the Ring Indicator (RI) line of a channel to OFF condition.
 */
typedef struct CiMuxRingIndicatorOffReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;
} CiMuxRingIndicatorOffReq;



/** Set Ring Indicator ON.
 * This signal is used by ATCI to set the Ring Indicator (RI) line of a channel to ON condition.
 */
typedef struct CiMuxRingIndicatorOnReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;
} CiMuxRingIndicatorOnReq;

/** @} */ /* End of SigCimuxChannel group */



/** \defgroup SigCimuxMux Multiplexer control signals
 * Signals to configure a multiplexer instance.
 * This section defines the signals that control the configuration of a multiplexer instance
 * as well as the start of the 27.010 multiplexer protocol. This includes signals to retrieve the
 * current configuration and the current version.
 *
 * The following MSC shows the typical sequence to start the 27.010 multiplexer protocol:
 *\msc
 * msc {
 *  hscale = "1.75";
 *  "TE","MUX","ATCI";
 *
 *  ---              [label="-- COMMAND mode --"];
 *  "TE"   -> "MUX"  [label="AT+CMUX=0, ...."];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataInd ( AT+CMUX=0, .... )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataRsp ( )"];
 *  "MUX"  <- "ATCI" [label="CiMuxCheckCmuxCmdParamsReq ( cmuxCmdParams )"];
 *  "MUX"  -> "ATCI" [label="CiMuxCheckCmuxCmdParamsCnf ( cmuxCmdParams, parametersValid=TRUE )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataReq ( OK )"];
 *  "TE"   <- "MUX"  [label="OK"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataCnf ( )"];
 *  "MUX"  <- "ATCI" [label="CiMuxStart27010MuxReq ( cmuxCmdParams )"];
 *  "TE"   <- "MUX"  [label="DCD line ON"];
 *  "MUX"  -> "ATCI" [label="CiMuxChannelDisabledInd ( )"];
 *  "MUX"  <- "ATCI" [label="CiMuxChannelDisabledRsp ( )"];
 *  ---              [label="-- both sides run 27.010 multiplexer --"];
 *  "TE"   -> "MUX"  [label="SABM (DLCI=0)"];
 *  "TE"   <- "MUX"  [label="UA (DLCI=0)"];
 * }
 *\endmsc
 *
 * The channel is in COMMAND mode. The user sends an AT+CMUX command to start the 27.010
 * multiplexer on this channel. ATCI asks MUX to verify the parameters, supplied in AT+CMUX. The
 * OK response is a trigger for the TE to start the 27.010 multiplexer. When the 27.010
 * multiplexer is started MUX disables the channel because it is used for the multiplexer protocol
 * now. The DCD line is set to ON condition. The TE may use this as an alternative trigger to
 * start the 27.010 multiplexer. Now both sides run a 27.010 multiplexer and the TE can establish
 * logical channels.
 *
 * @{
 */

/** Request of a CMUX parameters check.
 * This signal is used by ATCI to request MUX to check the parameters, supplied in an AT+CMUX
 * command. The user sends the AT+CMUX command to start the 27.010 multiplexer. Before the 27.010
 * multiplexer can be started MUX needs to validate that the parameters of the AT+CMUX command are
 * supported and that the 27.010 multiplexer can be started on this channel.
 */
typedef struct CiMuxCheckCmuxCmdParamsReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;

    /** Parameters that need to be validated.
     * Parameters of the AT+CMUX command that need to be checked for validity.
     */
    CmuxCmdParams               cmuxCmdParams;
} CiMuxCheckCmuxCmdParamsReq;



/** Result of the CMUX parameters check.
 * This signal is used as a reply to \link #CiMuxCheckCmuxCmdParamsReqTag CiMuxCheckCmuxCmdParamsReq
 * \endlink signal.
 */
typedef struct CiMuxCheckCmuxCmdParamsCnfTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;

    /** Parameters that have been validated.
     * Parameters of the AT+CMUX command that have been provided with \link
     * #CiMuxCheckCmuxCmdParamsReqTag CiMuxCheckCmuxCmdParamsReq \endlink . MUX is returning these
     * parameters here in order to simplify processing in ATCI.
     */
    CmuxCmdParams               cmuxCmdParams;

    /** Result of the CMUX parameters check.
     * \a parametersValid is set to TRUE if the parameters provided in
     * \link #CiMuxCheckCmuxCmdParamsReqTag CiMuxCheckCmuxCmdParamsReq \endlink are supported by MUX and
     * the 27.010 multiplexer can be started on this channel. \a parametersValid is set to FALSE
     * if one parameter in \link #CiMuxCheckCmuxCmdParamsReqTag CiMuxCheckCmuxCmdParamsReq \endlink is
     * not supported by MUX or if \link #CiMuxCheckCmuxCmdParamsReqTag CiMuxCheckCmuxCmdParamsReq
     * \endlink is received on a channel which belongs to a mutiplexer instance which already runs
     * the 27.010 multiplexer protocol. In case \a parametersValid is set to TRUE ATCI will issue
     * \link #CiMuxStart27010MuxReqTag CiMuxStart27010MuxReq \endlink to start the 27.010
     * multiplexer protocol on this instance.
     */
    Boolean                     parametersValid;
} CiMuxCheckCmuxCmdParamsCnf;



/** Configure multiplexer instance.
 * This signal is used to change multiplexer configuration options. The signal is issued after
 * AT*MMUX command is received. Since ATCI task is responsible for interpreting AT commands, it
 * will notify the MUX task about the request.
 */
typedef struct CiMuxConfigureMuxReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;

    /** New multiplexer configuration */
    MuxConfigOptions            muxConfigOptions;
} CiMuxConfigureMuxReq;



/** Result of the attempt to configure a multiplexer instance.
 * This signal is used to confirm or reject a \link #CiMuxConfigureMuxReqTag CiMuxConfigureMuxReq
 * \endlink signal. MUX will reject \link #CiMuxConfigureMuxReqTag CiMuxConfigureMuxReq \endlink
 * if the provided parameters are not supported or if \link #CiMuxConfigureMuxReqTag
 * CiMuxConfigureMuxReq \endlink is sent on a logical 27.010 multiplexer channel.
 */
typedef struct CiMuxConfigureMuxCnfTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;

    /** Result of the attempt to configure a multiplexer instance.
     * If \a success is set to TRUE, the new configuration in \link #CiMuxConfigureMuxReqTag
     * CiMuxConfigureMuxReq \endlink was accepted and applied by MUX. If \a success is set to
     * FALSE, the configuration in \link #CiMuxConfigureMuxReqTag CiMuxConfigureMuxReq \endlink
     * was rejected by the MUX; ATCI will respond with an error to the AT*MMUX command in this
     * case.
     */
    Boolean                     success;
} CiMuxConfigureMuxCnf;



/** Request current CMUX parameters.
 * This signal is used by ATCI to obtain the current 27.010 multiplexer parameters to provide a
 * response to AT+CMUX? Query.
 */
typedef struct CiMuxReadCmuxCmdParamsReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;
} CiMuxReadCmuxCmdParamsReq;



/** Returns current CMUX parameters.
 * This signal is used as a reply to a \link #CiMuxReadCmuxCmdParamsReqTag CiMuxReadCmuxCmdParamsReq
 * \endlink signal. In case the multiplexer instance runs the 27.010 multiplexer protocol the
 * current multiplexer parameters are returned to ATCI.
 */
typedef struct CiMuxReadCmuxCmdParamsCnfTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;

    /** Current CMUX parameters.
     * \a cmuxCmdParams are set to the values used by the currently running 27.010 multiplexer.
     * \a cmuxCmdParams is only relevant if \a active27010 is set to TRUE.
     */
    CmuxCmdParams               cmuxCmdParams;

    /** Indication if 27.010 multiplexer is running.
     * \a active27010 is set to TRUE, if the multiplexer instance to which the channel belongs
     * runs the 27.010 multiplexer protocol.
     */
    Boolean                     active27010;
} CiMuxReadCmuxCmdParamsCnf;



/** Start 27.010 multiplexer protocol.
 * This signal is used to start the 27.010 multiplexer protocol. The request will be performed for
 * the multiplexer instance to which the channel belongs. MUX will ensure that all AT data,
 * received on the channel in \link #CiMuxAtDataReqTag CiMuxAtDataReq \endlink signals from ATCI,
 * is transmitted towards the peer Host before the 27.010 multiplexer is started. MUX will also
 * ensure that the channel uses RTS/CTS flow control before the 27.010 multiplexer is started.
 * \note No parameter validity checking is performed by MUX on receipt of this signal; it is
 * assumed that parameters were verified using \link #CiMuxCheckCmuxCmdParamsReqTag
 * CiMuxCheckCmuxCmdParamsReq \endlink signal.
 */
typedef struct CiMuxStart27010MuxReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;

    /** CMUX parameters.
     * \a cmuxCmdParams holds the values to be used by the 27.010 multiplexer protocol.
     */
    CmuxCmdParams               cmuxCmdParams;
} CiMuxStart27010MuxReq;



/** Query current MUX version.
 * This signal is used by ATCI to query the current MUX version.
 */
typedef struct CiMuxVersionReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;
} CiMuxVersionReq;



/** Returns current MUX version.
 * This signal is used as a reply to a \link #CiMuxVersionReqTag CiMuxVersionReq
 * \endlink signal. MUX provides its current version to ATCI.
 */
typedef struct CiMuxVersionCnfTag
{
    /** Number assigned to the channel (range 0-255) */
    Int32                       channelNumber;

    /** Current major version number of MUX.
     * The major version number increases when the new MUX version is not backward compatible
     * with the previous version of MUX.
     */
    Int32                       versionMajor;

    /** Current minor version number of MUX.
     * The minor version number increases when the new MUX version provides new features compared
     * to the previous version and the new MUX version is backward compatible with the previous
     * one. If \a versionMajor increases then \a versionMinor is set to zero.
     */
    Int32                       versionMinor;

    /** Current build version number of MUX.
     * The build version number increases when the new MUX version provides bug fixes compared to
     * the previous version. If \a versionMajor increases or \a versionMinor increases then
     * \a versionBuild is set to zero.
     */
    Int32                       versionBuild;
} CiMuxVersionCnf;

/** @} */ /* End of SigCimuxMux group */

/** @} */ /* End of SigCimux group */

#else /* UPGRADE_NASMDL2 */

/***************************************************************************
 * Nested Include Files
 ***************************************************************************/
#include <mux_cfg.h>
#include <cimux_typ.h>


/** \defgroup SigCiMux CIMUX Signalling Interface
 * \ingroup SigCiex
 * \ingroup Sig
 * Signaling interface between ATCI and MUX.
 * This section defines all signals exchanged between ATCI and MUX, hereinafter called the CIMUX
 * interface.
 *
 * The context of the CIMUX interface is shown in the Figure below:
 *
 * \image html Middleware_Interface.png
 * @{
 */
/***************************************************************************
* Signals
***************************************************************************/
/** \defgroup SigCimuxAtdata Signals for AT data exchange
 * AT data signals and their associated flow control signals.
 * Each AT data signal requires a flow control signal as an acknowledgement before the next AT data
 * signal can be sent.
 *
 * The following MSC shows this for AT data from ATCI to MUX:
 *\msc
 * msc {
 *  "MUX","ATCI";
 *
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataReq ( channelNumber, ..., data )"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataCnf ( channelNumber )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataReq ( channelNumber, ..., data )"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataCnf ( channelNumber )"];
 * }
 *\endmsc
 *
 * The following MSC shows the same principle for AT data from MUX to ATCI:
 *\msc
 * msc {
 *  "MUX","ATCI";
 *
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataInd ( channelNumber, ..., data )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataRsp ( channelNumber )"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataInd ( channelNumber, ..., data )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataRsp ( channelNumber )"];
 * }
 *\endmsc
 *
 * @{
 */

/** ATCI transmits AT data to MUX.
 * This signal is used to transmit AT data from ATCI to MUX.
 * ATCI requires a confirm signal before it sends the next data signal.
 */
typedef struct CiMuxAtDataReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;

    /** Length of the relevant AT data in the signal. 
\assoc ARRAY \ref data */
    Int16                       length;

    /** Array which contains the AT data.
     * The length of the AT data array is defined by #CIMUX_MAX_AT_DATA_LENGTH.
     */
    Int8                        data [CIMUX_MAX_AT_DATA_LENGTH];
} CiMuxAtDataReq;

/** MUX confirms transmission of AT data.
 * This signal is sent by MUX to ATCI to acknowledge successful transmission of AT data. Each
 * \link #CiMuxAtDataReqTag CiMuxAtDataReq \endlink needs to be acknowledged by a
 * \a CiMuxAtDataCnf before the next \link #CiMuxAtDataReqTag CiMuxAtDataReq \endlink can be sent.
 */
typedef struct CiMuxAtDataCnfTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;
} CiMuxAtDataCnf;

/** MUX transmits AT data to ATCI.
 * This signal is used to transmit AT data from MUX to ATCI.
 * MUX requires a response signal before it sends the next data signal.
 */
typedef struct CiMuxAtDataIndTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;

    /** Length of the relevant AT data in the signal. 
\assoc ARRAY \ref data */
    Int16                       length;

    /** Array which contains the AT data.
     * The length of the AT data array is defined by #CIMUX_MAX_AT_DATA_LENGTH.
     */
    Int8                        data [CIMUX_MAX_AT_DATA_LENGTH];
} CiMuxAtDataInd;

/** ATCI responds after processing of AT data.
 * This signal is sent by ATCI to MUX to acknowledge successful processing of AT data. Each
 * \link #CiMuxAtDataIndTag CiMuxAtDataInd \endlink needs to be acknowledged by a
 * \a CiMuxAtDataRsp before the next \link #CiMuxAtDataIndTag CiMuxAtDataInd \endlink can be sent.
 */
typedef struct CiMuxAtDataRspTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;
} CiMuxAtDataRsp;

/** @} */ /* End of SigCimuxAtdata group */

/** \defgroup SigCimuxDataconn Routing signals
 * Signals that switch between COMMAND and DATA mode of a channel.
 * These signals change the routing of data. MUX performs this routing.
 *
 * The following MSC is an example of such a change of routing:
 *\msc
 * msc {
 *  hscale = "1.75";
 *  "TE","MUX","ATCI";
 *
 *  ---              [label="-- COMMAND mode --"];
 *  "TE"   -> "MUX"  [label="ATD..."];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataInd ( ATD... )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataRsp ( )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataReq ( CONNECT )"];
 *  "TE"   <- "MUX"  [label="CONNECT"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataCnf ( )"];
 *  "MUX"  <- "ATCI" [label="CiMuxOpenDataConnReq ( )"];
 *  "TE"   <- "MUX"  [label="DCD line ON"];
 *  "MUX"  -> "ATCI" [label="CiMuxOpenDataConnCnf ( success=TRUE )"];
 *  ---              [label="-- DATA mode --"];
 *  "TE"   -> "MUX"  [label="+++"];
 *  "MUX"  -> "ATCI" [label="CiMuxSwitchedToCmdModeInd ( closeDataConn=FALSE )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataReq ( OK )"];
 *  "TE"   <- "MUX"  [label="OK"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataCnf ( )"];
 *  ---              [label="-- COMMAND mode --"];
 *  "TE"   -> "MUX"  [label="ATO"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataInd ( ATO )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataRsp ( )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataReq ( OK )"];
 *  "TE"   <- "MUX"  [label="OK"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataCnf ( )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtoCmdReq ( )"];
 *  ---              [label="-- DATA mode --"];
 *  "MUX"  <- "ATCI" [label="CiMuxCloseDataConnReq ( )"];
 *  "TE"   <- "MUX"  [label="DCD line OFF"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataReq ( NO CARRIER )"];
 *  "TE"   <- "MUX"  [label="NO CARRIER"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataCnf ( )"];
 *  ---              [label="-- COMMAND mode --"];
 * }
 *\endmsc
 *
 * The channel is in COMMAND mode. The TE sends an ATD command to switch into DATA mode. The DCD
 * line is set to ON condition to indicate that the data connection is established. The TE sends
 * an escape sequence to temporarily switch back to COMMAND mode. The DCD line stays in ON
 * condition since the data connection is still alive. The TE sends an ATO command to switch back
 * to DATA mode. The communication peer hangs up. The data connection is terminated and the DCD
 * line is set back to OFF condition.
 *
 * @{
 */

/** Return to DATA mode after ATO.
 * This signal is used to perform the ATO command operation, switching from COMMAND mode back to
 * the appropriate DATA mode. The data connection settings provided via
 * \link #CiMuxOpenDataConnReqTag CiMuxOpenDataConnReq \endlink are re-applied for the data
 * connection.
 */
typedef struct CiMuxAtoCmdReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;
} CiMuxAtoCmdReq;

/** Close data connection and return to COMMANMD mode.
 * This signal is used to Close the data connection on a specific channel. The channel returns to
 * COMMAND as a result of this signal being processed by MUX.
 */
typedef struct CiMuxCloseDataConnReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;
} CiMuxCloseDataConnReq;

/** Open data connection and enter DATA mode.
 * This signal is used to open a data connection on the specified channel. If the command can be
 * processed successfully, the channel will switch from COMMAND mode to DATA mode.
 */
typedef struct CiMuxOpenDataConnReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;

    /** Data connection ID. (range 0-255)
     * Internal ID used by several tasks to identify the data connection.
     */
    Int8                        connId;

    /** Data connection options.
     * The data connection options are only valid if #dataConnType is set to
     * \link #DataConnTypeTag::PSD_PPP PSD_PPP \endlink.
     */
    DataConnOptions             dataConnOptions;

    /** Data connection type.
     * This parameter determines the data connection type.
     */
    DataConnType                dataConnType;
} CiMuxOpenDataConnReq;

/** Result of the attempt to open a data connection.
 * This signal is used to confirm or reject \link #CiMuxOpenDataConnReqTag CiMuxOpenDataConnReq
 * \endlink.
 */
typedef struct CiMuxOpenDataConnCnfTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;

    /** Result of the attempt to open a data connection.
     * Contains the result of the attempt to open the data connection. If it is set to TRUE, the
     * channel is in DATA mode.
     */
    Boolean                     success;
} CiMuxOpenDataConnCnf;

/** Indication that the channel is back in COMMAND mode.
 * This signal is used to indicate to ATCI that the channel has switched from DATA mode to COMMAND
 * mode (as a result of escape sequence being detected or DTR being dropped by the peer Host). The
 * user can enter AT commands now.
 */
typedef struct CiMuxSwitchedToCmdModeIndTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;

    /** Request to close the data connection as well.
     * If \a closeDataConn is set to TRUE, the peer host has requested to close the data connection
     * on this channel (e.g. on reception of a DTR drop when
     * \link #ChannelConfOptionsTag::actionOnDtrDrop actionOnDtrDrop \endlink is set to
     * \link #ActionOnDtrDropTag::CLOSE_ON_DTR_DROP CLOSE_ON_DTR_DROP \endlink). If
     * \a closeDataConn is set to FALSE,
     * the data connection must be kept alive, so the user can switch back to DATA mode by sending
     * an ATO command (see \link #CiMuxAtoCmdReqTag CiMuxAtoCmdReq \endlink).
     */
    Boolean                     closeDataConn;
} CiMuxSwitchedToCmdModeInd;

/** @} */ /* End of SigCimuxDataconn group */

/** \defgroup SigCimuxChannel Channel control signals
 * Signals for channel creation, destruction and configuration.
 * This section defines the signals that control creation, destruction and configuration of a
 * channel.
 *
 * The following MSC is an example for creating and destroying channels:
 *\msc
 * msc {
 *  hscale = "1.8";
 *  "TE","MUX","ATCI";
 *
 *  ---              [label="-- both sides run 27.010 multiplexer --"];
 *  "TE"   -> "MUX"  [label="SABM ( DLCI=1 )"];
 *  "MUX"  -> "ATCI" [label="CiMuxChannelEnableInd ( channelNumber=0, ... )"];
 *  "MUX"  <- "ATCI" [label="CiMuxChannelEnableRsp ( channelNumber=0, ..., channelAccept=TRUE )"];
 *  "TE"   <- "MUX"  [label="UA ( DLCI=1 )"];
 *  "TE"   -> "MUX"  [label="SABM ( DLCI=2 )"];
 *  "MUX"  -> "ATCI" [label="CiMuxChannelEnableInd ( channelNumber=1, ... )"];
 *  "MUX"  <- "ATCI" [label="CiMuxChannelEnableRsp ( channelNumber=1, ..., channelAccept=TRUE )"];
 *  "TE"   <- "MUX"  [label="UA ( DLCI=2 )"];
 *  "TE"   -> "MUX"  [label="SABM ( DLCI=3 )"];
 *  "MUX"  -> "ATCI" [label="CiMuxChannelEnableInd ( channelNumber=2, ... )"];
 *  "MUX"  <- "ATCI" [label="CiMuxChannelEnableRsp ( channelNumber=2, ..., channelAccept=FALSE )"];
 *  "TE"   <- "MUX"  [label="DM ( DLCI=3 )"];
 *  "TE"   -> "MUX"  [label="DTR line drop"];
 *  "MUX"  -> "ATCI" [label="CiMuxChannelDisabledInd ( channelNumber=1 )"];
 *  "MUX"  -> "ATCI" [label="CiMuxChannelDisabledInd ( channelNumber=0 )"];
 *  "TE"   <- "MUX"  [label="DCD line OFF"];
 *  "MUX"  -> "ATCI" [label="CiMuxChannelEnableInd ( channelNumber=0, ... )"];
 *  "MUX"  <- "ATCI" [label="CiMuxChannelEnableRsp ( channelNumber=0, ..., channelAccept=TRUE )"];
 *  ---              [label="-- COMMAND mode --"];
 * }
 *\endmsc
 *
 * Both sides run a 27.010 multiplexer. MUX enables a new channel for each logical channel the TE
 * establishes. ATCI must accept the newly enabled channel by setting
 * \link #CiMuxChannelEnableRspTag::channelAccept channelAccept \endlink to TRUE. If ATCI is unable
 * to handle the new channel it sets \link #CiMuxChannelEnableRspTag::channelAccept channelAccept
 * \endlink to FALSE and MUX will reject the establishment of the channel. The TE drops the DTR
 * line to switch back to COMMAND mode. MUX, therefore, stops using the 27.010 multiplexer
 * protocol and disables all logical channels. MUX also sets the DCD line back to OFF condition.
 * Now the channel is available for AT commands again. MUX, therefore, enables the channel to
 * ATCI. A channel number of a previously disabled channel can be reused for any new channel to be
 * enabled. Now the channel is in COMMAND mode again.
 *
 * @{
 */

/** Indicates the destruction of a channel.
 * This signal is sent by MUX to ATCI to disable a previously enabled channel. As a result this
 * channel ceases to exist from the point of view of ATCI.
 */
typedef struct CiMuxChannelDisabledIndTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;
} CiMuxChannelDisabledInd;

/** Indicates the creation of a new channel.
 * This signal is sent by MUX to enable a channel. ATCI handles enabled channels only.
 */
typedef struct CiMuxChannelEnableIndTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;

    /** Communication port settings.
     * Contains the current communication port settings of the channel.
     */
    ComPortSettings             comPortSettings;

    /** Channel Configuration Options.
     * Contains the current channel configuration options of the channel.
     */
    ChannelConfOptions          channelConfOptions;
} CiMuxChannelEnableInd;

/** Result of the attempt to create a new channel.
 * This signal is sent by ATCI to acknowledge or reject \link #CiMuxChannelEnableIndTag
 * CiMuxChannelEnableInd \endlink. If ATCI acknowledges the new channel this signal is sent when
 * ATCI has completed setting up the channel and when it is ready to receive
 * \link #CiMuxAtDataIndTag CiMuxAtDataInd \endlink signals on it.
 */
typedef struct CiMuxChannelEnableRspTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;

    /** Communication port settings.
     * Contains the requested communication port settings of the channel.
     */
    ComPortSettings             comPortSettings;

    /** Channel Configuration Options.
     * Contains the requested channel configuration options of the channel.
     */
    ChannelConfOptions          channelConfOptions;

    /** Result of the attempt to create a new channel.
     * Contain the result of the attempt to create a new channel. If \a channelAccept is set to
     * TRUE, ATCI is able to handle the new channel. If \a channelAccept is set to FALSE, ATCI
     * rejects handling the new channel and MUX closes the channel.
     */
    Boolean                     channelAccept;
} CiMuxChannelEnableRsp;

/** Configuration of a channel.
 * This signal is used to change communication port settings and channel configuration options of
 * a channel.
 */
typedef struct CiMuxConfigureChannelReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;

    /** Communication port settings.
     * Contains the requested communication port settings of the channel.
     */
    ComPortSettings             comPortSettings;

    /** Channel Configuration Options.
     * Contains the requested channel configuration options of the channel.
     */
    ChannelConfOptions          channelConfOptions;
} CiMuxConfigureChannelReq;

/** Set Ring Indicator OFF.
 * This signal is used by ATCI to set the Ring Indicator (RI) line of a channel to OFF condition.
 */
typedef struct CiMuxRingIndicatorOffReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;
} CiMuxRingIndicatorOffReq;

/** Set Ring Indicator ON.
 * This signal is used by ATCI to set the Ring Indicator (RI) line of a channel to ON condition.
 */
typedef struct CiMuxRingIndicatorOnReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;
} CiMuxRingIndicatorOnReq;

/** @} */ /* End of SigCimuxChannel group */

/** \defgroup SigCimuxMux Multiplexer control signals
 * Signals to configure a multiplexer instance.
 * This section defines the signals that control the configuration of a multiplexer instance
 * as well as the start of the 27.010 multiplexer protocol. This includes signals to retrieve the
 * current configuration and the current version.
 *
 * The following MSC shows the typical sequence to start the 27.010 multiplexer protocol:
 *\msc
 * msc {
 *  hscale = "1.75";
 *  "TE","MUX","ATCI";
 *
 *  ---              [label="-- COMMAND mode --"];
 *  "TE"   -> "MUX"  [label="AT+CMUX=0, ...."];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataInd ( AT+CMUX=0, .... )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataRsp ( )"];
 *  "MUX"  <- "ATCI" [label="CiMuxCheckCmuxCmdParamsReq ( cmuxCmdParams )"];
 *  "MUX"  -> "ATCI" [label="CiMuxCheckCmuxCmdParamsCnf ( cmuxCmdParams, parametersValid=TRUE )"];
 *  "MUX"  <- "ATCI" [label="CiMuxAtDataReq ( OK )"];
 *  "TE"   <- "MUX"  [label="OK"];
 *  "MUX"  -> "ATCI" [label="CiMuxAtDataCnf ( )"];
 *  "MUX"  <- "ATCI" [label="CiMuxStart27010MuxReq ( cmuxCmdParams )"];
 *  "TE"   <- "MUX"  [label="DCD line ON"];
 *  "MUX"  -> "ATCI" [label="CiMuxChannelDisabledInd ( )"];
 *  ---              [label="-- both sides run 27.010 multiplexer --"];
 *  "TE"   -> "MUX"  [label="SABM (DLCI=0)"];
 *  "TE"   <- "MUX"  [label="UA (DLCI=0)"];
 * }
 *\endmsc
 *
 * The channel is in COMMAND mode. The user sends an AT+CMUX command to start the 27.010
 * multiplexer on this channel. ATCI asks MUX to verify the parameters, supplied in AT+CMUX. The
 * OK response is a trigger for the TE to start the 27.010 multiplexer. When the 27.010
 * multiplexer is started MUX disables the channel because it is used for the multiplexer protocol
 * now. The DCD line is set to ON condition. The TE may use this as an alternative trigger to
 * start the 27.010 multiplexer. Now both sides run a 27.010 multiplexer and the TE can establish
 * logical channels.
 *
 * @{
 */

/** Request of a CMUX parameters check.
 * This signal is used by ATCI to request MUX to check the parameters, supplied in an AT+CMUX
 * command. The user sends the AT+CMUX command to start the 27.010 multiplexer. Before the 27.010
 * multiplexer can be started MUX needs to validate that the parameters of the AT+CMUX command are
 * supported and that the 27.010 multiplexer can be started on this channel.
 */
typedef struct CiMuxCheckCmuxCmdParamsReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;

    /** Parameters that need to be validated.
     * Parameters of the AT+CMUX command that need to be checked for validity.
     */
    CmuxCmdParams               cmuxCmdParams;
} CiMuxCheckCmuxCmdParamsReq;

/** Result of the CMUX parameters check.
 * This signal is used as a reply to \link #CiMuxCheckCmuxCmdParamsReqTag CiMuxCheckCmuxCmdParamsReq
 * \endlink signal.
 */
typedef struct CiMuxCheckCmuxCmdParamsCnfTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;

    /** Parameters that have been validated.
     * Parameters of the AT+CMUX command that have been provided with \link
     * #CiMuxCheckCmuxCmdParamsReqTag CiMuxCheckCmuxCmdParamsReq \endlink . MUX is returning these
     * parameters here in order to simplify processing in ATCI.
     */
    CmuxCmdParams               cmuxCmdParams;

    /** Result of the CMUX parameters check.
     * \a parametersValid is set to TRUE if the parameters provided in
     * \link #CiMuxCheckCmuxCmdParamsReqTag CiMuxCheckCmuxCmdParamsReq \endlink are supported by MUX and
     * the 27.010 multiplexer can be started on this channel. \a parametersValid is set to FALSE
     * if one parameter in \link #CiMuxCheckCmuxCmdParamsReqTag CiMuxCheckCmuxCmdParamsReq \endlink is
     * not supported by MUX or if \link #CiMuxCheckCmuxCmdParamsReqTag CiMuxCheckCmuxCmdParamsReq
     * \endlink is received on a channel which belongs to a mutiplexer instance which already runs
     * the 27.010 multiplexer protocol. In case \a parametersValid is set to TRUE ATCI will issue
     * \link #CiMuxStart27010MuxReqTag CiMuxStart27010MuxReq \endlink to start the 27.010
     * multiplexer protocol on this instance.
     */
    Boolean                     parametersValid;
} CiMuxCheckCmuxCmdParamsCnf;

/** Configure multiplexer instance.
 * This signal is used to change multiplexer configuration options. The signal is issued after
 * AT*MMUX command is received. Since ATCI task is responsible for interpreting AT commands, it
 * will notify the MUX task about the request.
 */
typedef struct CiMuxConfigureMuxReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;

    /** New multiplexer configuration */
    MuxConfigOptions            muxConfigOptions;
} CiMuxConfigureMuxReq;

/** Result of the attempt to configure a multiplexer instance.
 * This signal is used to confirm or reject a \link #CiMuxConfigureMuxReqTag CiMuxConfigureMuxReq
 * \endlink signal. MUX will reject \link #CiMuxConfigureMuxReqTag CiMuxConfigureMuxReq \endlink
 * if the provided parameters are not supported or if \link #CiMuxConfigureMuxReqTag
 * CiMuxConfigureMuxReq \endlink is sent on a logical 27.010 multiplexer channel.
 */
typedef struct CiMuxConfigureMuxCnfTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;

    /** Result of the attempt to configure a multiplexer instance.
     * If \a success is set to TRUE, the new configuration in \link #CiMuxConfigureMuxReqTag
     * CiMuxConfigureMuxReq \endlink was accepted and applied by MUX. If \a success is set to
     * FALSE, the configuration in \link #CiMuxConfigureMuxReqTag CiMuxConfigureMuxReq \endlink
     * was rejected by the MUX; ATCI will respond with an error to the AT*MMUX command in this
     * case.
     */
    Boolean                     success;
} CiMuxConfigureMuxCnf;

/** Request current CMUX parameters.
 * This signal is used by ATCI to obtain the current 27.010 multiplexer parameters to provide a
 * response to AT+CMUX? Query.
 */
typedef struct CiMuxReadCmuxCmdParamsReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;
} CiMuxReadCmuxCmdParamsReq;

/** Returns current CMUX parameters.
 * This signal is used as a reply to a \link #CiMuxReadCmuxCmdParamsReqTag CiMuxReadCmuxCmdParamsReq
 * \endlink signal. In case the multiplexer instance runs the 27.010 multiplexer protocol the
 * current multiplexer parameters are returned to ATCI.
 */
typedef struct CiMuxReadCmuxCmdParamsCnfTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;

    /** Current CMUX parameters.
     * \a cmuxCmdParams are set to the values used by the currently running 27.010 multiplexer.
     * \a cmuxCmdParams is only relevant if \a active27010 is set to TRUE.
     */
    CmuxCmdParams               cmuxCmdParams;

    /** Indication if 27.010 multiplexer is running.
     * \a active27010 is set to TRUE, if the multiplexer instance to which the channel belongs
     * runs the 27.010 multiplexer protocol.
     */
    Boolean                     active27010;
} CiMuxReadCmuxCmdParamsCnf;

/** Start 27.010 multiplexer protocol.
 * This signal is used to start the 27.010 multiplexer protocol. The request will be performed for
 * the multiplexer instance to which the channel belongs. MUX will ensure that all AT data,
 * received on the channel in \link #CiMuxAtDataReqTag CiMuxAtDataReq \endlink signals from ATCI,
 * is transmitted towards the peer Host before the 27.010 multiplexer is started. MUX will also
 * ensure that the channel uses RTS/CTS flow control before the 27.010 multiplexer is started.
 * \note No parameter validity checking is performed by MUX on receipt of this signal; it is
 * assumed that parameters were verified using \link #CiMuxCheckCmuxCmdParamsReqTag
 * CiMuxCheckCmuxCmdParamsReq \endlink signal.
 */
typedef struct CiMuxStart27010MuxReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;

    /** CMUX parameters.
     * \a cmuxCmdParams holds the values to be used by the 27.010 multiplexer protocol.
     */
    CmuxCmdParams               cmuxCmdParams;
} CiMuxStart27010MuxReq;

/** Query current MUX version.
 * This signal is used by ATCI to query the current MUX version.
 */
typedef struct CiMuxVersionReqTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;
} CiMuxVersionReq;

/** Returns current MUX version.
 * This signal is used as a reply to a \link #CiMuxVersionReqTag CiMuxVersionReq
 * \endlink signal. MUX provides its current version to ATCI.
 */
typedef struct CiMuxVersionCnfTag
{
    /** Number assigned to the channel (range 0-255) */
    Int8                        channelNumber;

    /** Current major version number of MUX.
     * The major version number increases when the new MUX version is not backward compatible
     * with the previous version of MUX.
     */
    Int16                       versionMajor;

    /** Current minor version number of MUX.
     * The minor version number increases when the new MUX version provides new features compared
     * to the previous version and the new MUX version is backward compatible with the previous
     * one. If \a versionMajor increases then \a versionMinor is set to zero.
     */
    Int16                       versionMinor;

    /** Current build version number of MUX.
     * The build version number increases when the new MUX version provides bug fixes compared to
     * the previous version. If \a versionMajor increases or \a versionMinor increases then
     * \a versionBuild is set to zero.
     */
    Int16                       versionBuild;
} CiMuxVersionCnf;

/** Returns reserved channel number. 
 *  This signal is a response to the empty signal request (SIG_CIMUX_RESERVE_CHANNEL_REQ ) to 
 *  reserve a channel for the ATCI to use internally so the MUX will not use or respond to any 
 *  commands or data on the assigned channel. If there is no channel available to reserve then 
 *  the \a success flag is set to FALSE. 
 */
typedef struct CiMuxReserveChannelCnfTag
{
    /** The number of the reserved channel.
     * This value is only valid if #success is set to TRUE. 
     */ 
    Int8                        channelNumber;

    /** Reports if the channel has been successfully reserved. */
    Boolean                     success; 
} CiMuxReserveChannelCnf;

/** Request to free a previously reserved channel. 
 * This signal from ATCI to free a previously reserved channel that is no longer needed for ATCI
 * internal operations. 
*/
typedef struct CiMuxFreeReservedChannelReqTag
{
    /** The number of the reserved channel now available for MUX use. */
    Int8                        channelNumber;
}  CiMuxFreeReservedChannelReq;

/** Reports if the freeing has been successful.
 * A response signal to the ATCI informing if the requested channel number has been successfully
 * freed. If the channel number sent in the request signal was not a previously reserved channel
 * the \a success flag is set to FALSE. 
*/
typedef struct CiMuxFreeReservedChannelCnfTag
{
    /** The number of the reserved channel now available for MUX use. */
    Int8                        channelNumber;

    /** This indicates if the reserved channel has been freed. */
    Boolean                     success;  
} CiMuxFreeReservedChannelCnf;

/** @} */ /* End of SigCimuxMux group */

/** @} */ /* End of SigCimux group */

#endif /* else UPGRADE_NASMDL2 */
#else /* UPGRADE_GENERIC_MUX */

/** \defgroup SigCiMux CI-MUX Signalling Interface
 * \ingroup Sig
 * \ingroup SigCiex
 * These signal are used by the CI Task to configure the multiplexer
 * communication port, detect data to command mode switches, and support a
 * boot registration mechanism.
 *
 * @{
 */

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <cimux_typ.h>
#include <sil3_typ.h>
#if !defined (UPGRADE_GENERIC_MUX)
#include <vgmx_typ.h>
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/
/** Boot Registration request.
 * This signal is used request an indication whenever the current multiplexer
 * changes or when an AT command channel becomes available.
 * \see #SigCiBootRegisterTaskInd
 */
typedef struct SigCiBootRegisterTaskReqTag
{
  TaskId  registeredTask;  /**< The task id to send #SigCiBootRegisterTaskInd signals to */
} SigCiBootRegisterTaskReq;

/** Boot Register indication.
 * This signal is used to inform a registered task of a change in the current
 * multiplexer or when an AT command channel becomes available.
 * \see #SigCiBootRegisterTaskReq
 */
typedef struct SigCiBootRegisterTaskIndTag
{
  Boolean         vopIsReady;         /**< TRUE if an AT command channel has been enabled */
  VgmuxId         activeMultiplexer;  /**< The identifier of the current active multiplexer */
  VgmxMuxIndex     muxIndex;          /**< Multiplexer Index */
  VgmxComPortIndex portIndex;         /**< COM Port Index */
} SigCiBootRegisterTaskInd;

/** Data and Command Mode Switch request.
 * This is signal used to request a multiplexer mode change. This involves
 * switching between AT command mode and GSM/GPRS data mode operation.
 * /note The CI Task does not make use of this signal.
 */
typedef struct CidsSwitchReqTag
{
  VgDsMode  newMode;        /**< The multiplexer mode we are switching to */
  Int8      escChar;        /**< The escape character to use when switching from data mode to
                             * command mode. This is typically set to the '+' character. */
  Int16     escGuard;       /**< The guard period, in 100ms units, used when detecting the escape
                             * sequence */
  Boolean   isATInterface;  /**< The switch request is issued from the CI Task */
  Char      terminator;     /**< The command terminator character to use. This is typically
                             * set to the carriage return character */
} CidsSwitchReq;


/** Switch from Data Mode to command Mode indication.
 * This signal is used by the standard multiplexer to indicate a particular
 * channel needs to switch from data mode to command mode.
 */
typedef struct CidsSwitchStandardIndTag
{
  VgmuxChannelNumber  channelNumber;  /**< The multiplexer channel which has detected the escape
                                       * sequence and wants to go to command mode */
} CidsSwitchStandardInd;

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

/** @} */ /* End of SigCiMux group */

#endif /* else UPGRADE_GENERIC_MUX */

#endif /* !CIMUX_SIG_H */

/* End of cimux_sig.h */

