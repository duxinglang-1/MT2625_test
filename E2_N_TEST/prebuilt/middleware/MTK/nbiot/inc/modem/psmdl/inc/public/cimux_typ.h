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
 * Middleware Type definitions for the CIMUX Interface.
 **************************************************************************/

#ifndef CIMUX_TYP_H
#define CIMUX_TYP_H

#if defined (UPGRADE_GENERIC_MUX)

/***************************************************************************
 * Nested Include Files
 ***************************************************************************/
#if defined (UPGRADE_NASMDL2)
#include <conn_typ.h>
#else
#include <muxconn_typ.h>
#endif

/***************************************************************************
 *   Manifest Constants
 ***************************************************************************/

/***************************************************************************
 *   Macro Functions
 ***************************************************************************/

/***************************************************************************
 *   Enumeration Types
 ***************************************************************************/

/** \addtogroup SigCimux
 *
 * @{
 */
/** Indicates data connection type. */
typedef enum DataConnTypeTag
{
#if defined (FEA_PPP)
    /** PSD connection using PPP. */
    PSD_PPP,
#endif /* FEA_PPP */
#if defined (UPGRADE_NASMDL2)
    /** PSD connection using Packet Transport. */
    PSD_PACKET_TRANSPORT,
#endif
    /** PSD connection type for AP Bridge. */
    PSD_AP_BRIDGE

} DataConnType;



/** Behavior after DTR drop. */
typedef enum ActionOnDtrDropTag
{
    /** No action is performed. */
    IGNORE_DTR_DROP                 = 0,

    /** If the channel in not in COMMAND mode then the channel is switched back to COMMAND mode and
     * the data connection on the channel is kept (\link
     * #CiMuxSwitchedToCmdModeIndTag::closeDataConn closeDataConn \endlink set to FALSE); the
     * user is able to switch back to DATA mode by sending an ATO command.
     */
    ESCAPE_ON_DTR_DROP              = 1,

    /** If the channel in not in COMMAND mode then the channel is switched back to COMMAND mode and
     * the Data connection on the channel is terminated (\link
     * #CiMuxSwitchedToCmdModeIndTag::closeDataConn closeDataConn \endlink set to TRUE).
     */
    CLOSE_ON_DTR_DROP               = 2
} ActionOnDtrDrop;



/** Transparency mechanism used by 27.010 multiplexer.
 * This enumeration type indicates the current or the requested transpatency mechanism used by
 * the 27.010 multiplexer.
 * \note Our implementation only supports basic option.
 */
typedef enum MuxOperationTag
{
    /** 27.010 multiplexer uses basic option. */
    BASIC_OPTION                    = 0,

    /** 27.010 multiplexer uses advanced option. */
    ADVANCED_OPTION                 = 1
} MuxOperation;



/** Frame type used for 27.010 multiplexer control channel.
 * This enumeration type indicates the current or the requested frame type used for the 27.010
 * multiplexer control channel.
 * \note Our implementation only supports UIH frames.
 */
typedef enum MuxSubsetTag
{
    /** UIH frames only. */
    UIH_FRAMES                      = 0,

    /** UI frames only. */
    UI_FRAMES                       = 1,

    /** I frames only. */
    I_FRAMES                        = 2
} MuxSubset;

/***************************************************************************
 *   Structure Types
 ***************************************************************************/

/** Defines the data connection options for PS data using PPP.
 * This struct configures PS data connections which use PPP.
 * \note Currently MUX only supports all parameters set to TRUE or all parameters set to FALSE.
 */
typedef struct DataConnOptionsTag
{
    /** Defines if PPP flag detection is used.
     * If \a pppFlagDetection is set to FALSE, MUX does not perform PPP flag detection in Rx
     * directions. If \a pppFlagDetection is set to TRUE, PPP flag detection is performed
     * as per PPP standards (RFC1662) and only complete PPP frames are forwarded to PPP.
     */
    Boolean                     pppFlagDetection;

    /** Defines if PPP byte stuffing is used.
     * If \a pppByteStuffing is set to FALSE, byte stuffing is not  used for the PPP connection in
     * both Rx and Tx directions. If \a pppByteStuffing is set to TRUE, byte stuffing is performed
     * as per PPP standards (RFC1662). If \a pppByteStuffing is set to TRUE, PPP flag detection
     * is performed as well regardless of the setting of \a pppFlagDetection.
     */
    Boolean                     pppByteStuffing;

    /** Defines if PPP FCS checking is used.
     * If \a pppFcsChecking is set to FALSE, FCS checking is not needed for PPP in Rx direction and
     * FCS insertion is not performed in Tx direction. If \a pppFcsChecking is set to TRUE, FCS
     * checking is performed in Rx and FCS insertion is performed Tx direction as per PPP
     * standards (RFC1662). If \a pppFcsChecking is set to TRUE, PPP flag detection
     * is performed as well regardless of the setting of \a pppFlagDetection.
     */
    Boolean                     pppFcsChecking;
} DataConnOptions;



/** Defines the configuration options of a channel.
 * This structure contains the channel configuration options. In case the 27.010 multiplexer is
 * running there is one such structure per logical channel.
 */
typedef struct ChannelConfOptionsTag
{
    /** Behavior of the DCD line.
     * If \a dcdFollows is set to TRUE, the DCD line (Data Carrier Detect) is only set to ON if
     * there is an active data connection on the channel. If \a dcdFollows  is set to FALSE, DCD
     * is always set to ON. \a dcdFollows is driven by AT&C command. ( Default = TRUE )
     */
    Boolean                     dcdFollows;

    /** Behavior after DTR drop.
     * \a actionOnDtrDrop defines behaviour after a transition from ON to OFF of the DTR line
     * (Data Terminal Ready) and after the #dtrIgnoreTime has elapsed. \a actionOnDtrDrop is
     * driven by AT&D command. ( Default = \link #ActionOnDtrDropTag::CLOSE_ON_DTR_DROP
     * CLOSE_ON_DTR_DROP \endlink)
     */
    ActionOnDtrDrop             actionOnDtrDrop;

    /** DTR drop ignore time. (range 0-255)
     * \a dtrIgnoreTime (in 10ms units) specifies the time a DTR drop is ignored. After a
     * transition from ON to OFF of the DTR line (Data Terminal Ready) \a dtrIgnoreTime specifies
     * the time the DTR line must stay in OFF condition before the action specified in
     * #actionOnDtrDrop is performed. \a dtrIgnoreTime is driven by ATS25 command. ( Default = 5 )
     */
    Int8                        dtrIgnoreTime;

    /** Behavior after escape sequence.
     * If \a escapeOnEscSeq is set to TRUE, the channel is switched to COMMAND mode if an escape
     * sequence is detected. If \a escapeOnEscSeq is set to FALSE, escape sequence detection is
     * turned off on the channel. ATCI sets \a escapeOnEscSeq to FALSE if #escChar is set to an
     * invalid value (#escChar > 127). ( Default = TRUE )
     * \note Escape sequence detection is only used if \link #MuxConfigOptionsTag::escMonitoring
     * escMonitoring \endlink is set to TRUE. If \link #MuxConfigOptionsTag::escMonitoring
     * escMonitoring \endlink is set to FALSE escape sequence detection is always turned off on the
     * channel regardless of the setting of \a escapeOnEscSeq.
     */
    Boolean                     escapeOnEscSeq;

    /** Escape sequence character.
     * \a escChar is the character used for the escape sequence. \a escChar is driven by ATS2
     * command. Only values in the range 0-127 are valid. ATCI will turn off escape sequence
     * detection (#escapeOnEscSeq set to FALSE) if \a escChar is set to an invalid value.
     * ( Default = 43 )
     */
    Int8                        escChar;

    /** Guard period of escape sequence. (range 0-255)
     * \a escGuardPeriod (in 20ms units) determines the time that forms a guard period before and
     * after three escape sequence characters. In order to distinguish an escape sequence from
     * just three #escChar in the data stream there is timing associated to the three #escChar of
     * an escape sequence. The time between the last byte of the data stream and the first
     * #escChar must be at least \a escGuardPeriod AND the time between each #escChar of the
     * escape sequence must be less than \a escGuardPeriod AND no other byte is received after the
     * third #escChar for the time of \a escGuardPeriod. \a escGuardPeriod is driven by ATS12
     * command. ( Default = 50 )
     */
    Int8                        escGuardPeriod;
} ChannelConfOptions;



/** 27.010 multiplexer parameters.
 * This structure contains parameters derived from AT+CMUX command which are used to start the
 * 27.010 multiplexer. (The format of the AT+CMUX command is defined in 3GPP 27.007, 5.7.)
 */
typedef struct CmuxCmdParamsTag
{
    /** Transparency mechanism.
     * Transparency mechanism used by both 27.010 multiplexers.
     * ( Default = \link #MuxOperationTag::BASIC_OPTION BASIC_OPTION \endlink )
     */
    MuxOperation                mode;

    /** Control channel frame type.
     * This parameter defines the frame type of the 27.010 multiplexer control channel.
     * ( Default = \link #MuxSubsetTag::UIH_FRAMES UIH_FRAMES \endlink )
     */
    MuxSubset                   subset;

    /** Transmission rate.
     * Transmission rate on the link between the two 27.010 multiplexers.
     * ( Default = \link #PortSpeedTag::PORTSPEED_9600 PORTSPEED_9600 \endlink )
     */
    PortSpeed                   portSpeed;

    /** Maximum control channel frame size. (range 31-4096)
     * Maximum 27.010 multiplexer frame size of frames of the 27.010 multiplexer control channel.
     * ( Default = 31 ).
     */
    Int32                       n1;

    /** Acknowledgement time. (range 1-255)
     * This parameter defines the amount of time (in 10ms units) the 27.010 multiplexer waits for
     * an acknowledgement on the 27.010 multiplexer control channel.
     * ( Default = 10 )
     * \note Our implementation only supports basic option. In this case an acknowledgement with
     * regards to this parameter is only required for the establishment and the release of the
     * 27.010 multiplexer control channel. Since our implementation does not initiate the
     * establishment nor the release of the 27.010 multiplexer control channel this parameter is
     * only used to validate #t2.
     */
    Int8                        t1;

    /** Maximum number of retransmissions on the control channel. (range 0-100)
     * Maximum number of retransmissions of frames on the 27.010 multiplexer control channel.
     * ( Default = 3 )
     */
    Int8                        n2;

    /** Control channel response time. (range 2-255)
     * This parameter defines the amount of time (in 10ms units) the 27.010 multiplexer waits for
     * a response on the 27.010 multiplexer control channel. \a t2 must be longer than #t1.
     * ( Default = 30 )
     */
    Int8                        t2;

    /** Wake-up response time. (range 1-255)
     * This parameter defines the amount of time (in 1second units) the 27.010 multiplexer waits for
     * the peer 27.010 multiplexer to wake-up.
     * ( Default = 10 )
     */
    Int8                        t3;

    /** Error recovery window size. (range 1-7)
     * Window size, for Advanced operation with Error Recovery options.
     * ( Default = 2 )
     * \note Out implementation only supports basic option. Error recovey is not supported in
     * basic option. This parameter is, therefore, ignored.
     */
    Int8                        k;
} CmuxCmdParams;



/** Multiplexer configuration options.
 * This structure contains the configuration options for the multiplexer. The parameters are
 * derived from AT*MMUX command. These options apply to a whole MUX instance. This means in case
 * the 27.010 multiplexer protocol is running all logical channels of the 27.010 multiplexer are
 * affected by these options. These options are not specific to just one or a few logical channels.
 */
typedef struct MuxConfigOptionsTag
{
    /** Indicates if DLC data is segmented.
     * \a muxChannelDataSegmented indicates if data received on logical 27.010 multiplexer
     * channels is segmented. This option only applies if the 27.010 multiplexer protocol is
     * running. If \a muxChannelDataSegmented is set to TRUE then MUX assumes that data received
     * on logical 27.010 multiplexer channels is segmented. This means MUX will perform reassembly
     * of data frames received on these channels. If \a muxChannelDataSegmented is set to FALSE
     * then MUX assumes that data received on logical 27.010 multiplexer channels is unsegmented.
     * MUX will, therefore, not perform any reassembly of received data frames and expects
     * complete data frames from the counterpart 27.010 multiplexer (one data frame per 27.010
     * multiplexer frame). This applies mainly to PPP frames on PSD connections sent on a logical
     * 27.010 multiplexer channel.
     */
    Boolean                     muxChannelDataSegmented;

    /** Indicates if Connection Layer data is segmented.
     * \a connDataSegmented indicates if data received from the Connection Level is segmented. If
     * \a connDataSegmented is set to TRUE then MUX assumes that data received from the Connection
     * Level is segmented. This means MUX will perform reassembly of frames received from the
     * Connection Level. If \a connDataSegmented is set to FALSE then MUX assumes that data
     * received from the Connection Level is unsegmented. MUX will, therefore, not perform any
     * reassembly of received frames and expects complete frames from the Connection Level (one
     * frame per \link #MuxConnRxDataIndTag MuxConnRxDataInd \endlink signal). This applies mainly
     * to 27.010 multiplexer frames if MUX runs the 27.010 multiplexer protocol and it applies to
     * PPP frames on PSD connections if MUX does not run the 27.010 multiplexer protocol.
     */
    Boolean                     connDataSegmented;

    /** Escape sequence monitoring.
     * If \a escMonitoring is set to TRUE then MUX performs escape sequence detection on each
     * channel depending on the setting of \link #ChannelConfOptionsTag::escapeOnEscSeq
     * escapeOnEscSeq \endlink of the channel. If \a escMonitoring is set to FALSE then escape
     * sequence detection is turned off on all channels handled by the multiplexer instance
     * regardless of the setting of \link #ChannelConfOptionsTag::escapeOnEscSeq escapeOnEscSeq
     * \endlink.
     */
    Boolean                     escMonitoring;

    /** Multiplexer frame FCS checking.
     * This option only applies when MUX runs the 27.010 multiplexer protocol. If \a fcsChecking
     * is set to TRUE then MUX will check the FCS of received 27.010 multiplexer frames. If
     * \a fcsChecking is set to FALSE then MUX expects an error-free link to its counterpart
     * 27.010 multiplexer and it does not perform FCS checking on received 27.010 multiplexer
     * frames.
     */
    Boolean                     fcsChecking;
} MuxConfigOptions;

/** @} */ /* End of SigCimux group */

#endif /* UPGRADE_GENERIC_MUX */

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

/***************************************************************************
 * Manifest Constants
 **************************************************************************/
#if !defined (UPGRADE_NASMDL2)
#define VG_MAX_AT_DATA_IN_SIGNAL_LENGTH  (75)
                                        /**< The maximum length of data that can be sent to the
                                          * Command Response Manager in the signal #CirmDataInd. */

#define VG_PRINTF_CONV_BUFFER     (VG_MAX_AT_DATA_IN_SIGNAL_LENGTH * 8)
                                        /**< Max temporary buffer length to hold strings which are
                                          * constructed for AT response or unsolicited event output
                                          * to the Command Response Manager */


#define SOLICITED_MAX_NUM_ITEMS   (20)  /**< The maximum number of signals that an entity can
                                         * register to receive at any time. */
#endif /* !UPGRADE_NASMDL2 */
/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

#if !defined (UPGRADE_GENERIC_MUX)

/** Flow Control Type.
 * This enumeration is used to specify the flow control mechanism applied for
 * multiplexer communications.
 */
typedef enum VgFlowControlTypeTag
{
  /** No flow control. */
  VG_FC_NONE = 0,
  /** Use XON/XOFF flow control, don't pass characters on to
   * data stack when TE is receiving data from TA. */
  VG_FC_XON_XOFF_NO_PASS = 1,
  /** Use RTS/CTS flow control. */
  VG_FC_RTS_CTS = 2,
  /** Use XON/XOFF flow control, pass characters on to data
   * stack when TE is receiving data from TA. */
  VG_FC_XON_XOFF_PASS_THROUGH = 3,
  /** The total number of flow control mechanisms supported. */
  VG_NUMBER_OF_FC_METHODS
} VgFlowControlType;

/** Data and Command Mode Type.
 * This enumeration is used when specifying a change between data and command
 * modes for a channel or multiplexer.
 */
typedef enum VgDsModeTag
{
  VG_DS_BYTE_MODE = 0,
  VG_DS_DATA_MODE = VG_DS_BYTE_MODE,   /**< bytewise data mode. */
  VG_DS_COMMAND_MODE,    /**< AT command mode. */
#if defined (FEA_PPP)
  VG_DS_PPP_MODE,       /**< PPP data mode. */
#endif /* FEA_PPP */
  VG_DS_NUMBER_OF_MODES  /**< The total number of modes supported. */
} VgDsMode;

#endif /* !UPGRADE_GENERIC_MUX */

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/


#endif /* !CIMUX_TYP_H */

/* End of cimux_typ.h */


