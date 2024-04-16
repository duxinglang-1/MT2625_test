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
 * Signal definitions for signals sent across the VG-MUX interface.
 **************************************************************************/

#ifndef VGMX_SIG_H
#define VGMX_SIG_H
#if !defined (UPGRADE_GENERIC_MUX)
/** \defgroup SigVgMx VG-MUX Signalling Interface
 * \ingroup Sig
 * \ingroup PrdDataServices
 * These signal are used by the CI Task to configure the multiplexer.
 *
 * @{
 */

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <vgmx_typ.h>

#if defined (FEA_PPP)
#include <dspppsig.h>
#include <cippplimits.h>
#endif /* FEA_PPP*/
#include <gpqos.h>

#include <cimux_sig.h>
#include <sml3_typ.h>
#include <pdp_typ.h>
#include <alsi_sig.h>
#include <rvcrdata.h>
#if defined (USE_BMM_ALLOC_MEMORY)
#include <bmm.h>
#include <bmm_sig.h>
#else
#include <tmm_sig.h>
#endif
#if defined (ON_PC) && defined (VG_MUX_DTE_PC_ONLY_VERSION)
#include <vgmxdte.h>
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

/** AT data request/indication.
 * This signal is used to transmit data between the CI Task and the
 * current Multiplexer Task.
 */
typedef struct VgmuxAtDataReqTag
{
  VgmuxChannelNumber channelNumber;             /**< The multiplexer channel. */
  Boolean            unsolicited;               /**< Indicates whether the message is unsolicited */
  Int8               length;                    /**< The length of the data in the signal. 
\assoc ARRAY \ref data */
  Int8               data[VG_MUX_AT_MAX_DATA];  /**< The data to transfer. */
} VgmuxAtDataReq, VgmuxAtDataInd;

/** AT data response.
 * This signal is used to confirm the receipt of AT data Ind.
 */
typedef struct VgmuxAtDataRspTag
{
  VgmuxChannelNumber channelNumber; /**< The multiplexer channel. */
} VgmuxAtDataRsp;

/** AT data confirm.
 * This signal is used to confirm the receipt of AT data Req.
 */
typedef struct VgmuxAtDataCnfTag
{
  VgmuxChannelNumber channelNumber; /**< The multiplexer channel. */
} VgmuxAtDataCnf;

/** Multiplexer Initialise request.
 * This signal is used to request that a multiplexer is initialised. The
 * multiplexer will attempt to establish communication channels.
 * \see #VgmuxInitialiseCnf
 */

typedef struct VgmuxInitialiseReqTag
{
  VgmuxId             multiplexer;       /**< The multiplexer type. */
  VgmxMuxIndex        muxIndex;          /**< The multiplexer Index.  */
  VgmxComPortSettings comPortSettings;   /**< COM port settings.      */
} VgmuxInitialiseReq;

/** Multiplexer Initialise/Close signals.
 * These signals are used to manage multiplexer closure and confirm
 * multiplexer initialisation.
 */
typedef struct VgmuxInitialiseCnfTag
{

  VgmuxId      multiplexer;  /**< The multiplexer identifier. */
  VgmxMuxIndex muxIndex;     /**< Multiplexer Index.          */
} VgmuxInitialiseCnf, VgmuxCloseReq, VgmuxCloseCnf, VgmuxCloseInd;

/** The scale factor used to convert the guard period to milliseconds. */
#define VG_MUX_GUARD_PERIOD_TO_MILLISECONDS  (20)

/** The maximum length of the escape sequence used when deassigning a channel. */
#define VG_MUX_DEASSIGN_ESC_LEN              (VG_MUX_AT_MAX_DATA)


/** Open Data Connection request.
 * This signal is used to open a data connection.
 */
typedef struct VgmuxOpenDataConnReqTag
{
  VgmuxChannelNumber atChannel;         /**< The AT command channel associated with the data
                                         * connection. */
  VgmuxDomain        domain;            /**< The series of identifiers of the data connection. */                      
  Int8               connId;            /**< The identifier of the opened Data Connection. */
  Int8               escChar;           /**< The escape character (usually a '+') to use when
                                         * exiting data mode. */
  Int8               guardPeriod;       /**< The guard period, in 100mS units, to use when
                                         * detecting the escape sequence in 100mS units. */
  Int8               terminator;        /**< The command terminator character (usually a carriage
                                         * return). */
  VgDsMode           mode;              /**< The data or command mode to assign to the channel.
                                         * Supported in Version 4 or later multiplexers. */
  KiTaskId           dataClientTaskId;  /**< The client task for which the channel assignment is
                                         * for. Supported in Version 4 or later multiplexers. */
  TmmPoolId          poolId;            /**< Tmm Pool Id to be used for specific connection */
} VgmuxOpenDataConnReq;

/** Open Data Connection confirmation.
 * This signal is used to confirm/reject Open Data Connection request.
 */
typedef struct VgmuxOpenDataConnCnfTag
{
  VgmuxChannelNumber atChannel;         /**< The AT command channel associated with the data
                                         * connection. */
  VgmuxDomain        domain;            /**< The series of identifiers of the data connection. */                      
  Int8               connId;            /**< The identifier of the opened Data Connection. */
  VgmxOpenConnStatus status;            /**< Result of the Open Data Connection request */
} VgmuxOpenDataConnCnf;

/** Channel Deassignment request.
 * This signal is used to remove the association between an AT command
 * channel and a data channel.
 */
typedef struct VgmuxCloseDataConnReqTag
{
  /** The AT command channel associated with the data channel. */
  VgmuxChannelNumber atChannel;
  VgmuxDomain        domain;  /**< The series of identifiers of the data connection. */
  Int8               connId;  /**< The identifier of the Data Connection to be closed */
  /** The escape sequence to send when deassigning the data channel. */
  Int8               escapeSequence[VG_MUX_DEASSIGN_ESC_LEN];
  /** The escape sequence length. */
  Int8               escapeLength;
} VgmuxCloseDataConnReq;

/** Configure ComPort request.
 * This signal is used to configure Com port settings.
 */
typedef struct VgmuxConfigureComPortReqTag
{ 
  /** Whether the configuration is set to default */
  Boolean setToDefault;
  /** Multiplexer Index.          */
  VgmxMuxIndex muxIndex;
  /** Com port settings */
  VgmxComPortSettings settings;
} VgmuxConfigureComPortReq, VgmuxConfigureComPortInd;

/** Port Rate Indication.
 * This signal is used to indicate about the changed Port Rate
 */
typedef struct VgmuxPortRateIndTag
{
  /** Multiplexer Index.          */
  VgmxMuxIndex muxIndex;
  /** New Port rate */
  VgmxPortRate portRate;
} VgmuxPortRateInd;

/** Channel Enable Indication.
 * This signal is used to enable AT channel
 */
typedef struct VgmuxEnableChannelIndTag
{
  /** The index of multiplexer which enables the channel */
  VgmxMuxIndex muxIndex;
  /** Transaction identifier */
  Int16        refNum;
} VgmuxEnableChannelInd;

/** Channel Enable Response.
 * This signal is used to confirm the enabling of an AT channel
 */
typedef struct VgmuxEnableChannelRspTag
{
  /** Transaction identifier */
  Int16              refNum;
  /** Channel number assigned to enabled channel */
  VgmuxChannelNumber channelNumber;
} VgmuxEnableChannelRsp;
/** Channel Disable Indication.
 * This signal is used to disable AT channel
 */
typedef struct VgmuxDisableChannelIndTag
{
  /** AT Channel number */
  VgmuxChannelNumber channelNumber;
} VgmuxDisableChannelInd;


/** Channel Disable Response.
 * This signal is used to confirm the disabling of AT channel
 */
typedef struct VgmuxDisableChannelRspTag
{
  /** AT Channel number */
  VgmuxChannelNumber channelNumber;
} VgmuxDisableChannelRsp;

/** Switch Com Port Request.
 * This signal is used to request the COM port switch
 */
typedef struct VgmuxSwitchComPortReqTag
{
 /** The index of multiplexer which is to be switched to another COM port */
  VgmxMuxIndex muxIndex;
  /** Transaction identifier */
  Int16 cmdRef;
  /** The index of a new COM Port */
  VgmxComPortIndex portIndex;
} VgmuxSwitchComPortReq;

/** Switch Com Port Confirmation.
 * This signal is used to comfirm the COM port being switched
 */
typedef struct VgmuxSwitchComPortCnfTag
{
  /** Transaction identifier */
  Int16 cmdRef;
  /** Status of the request */
  VgmxSwitchComPortStatus status;
} VgmuxSwitchComPortCnf;

/** XON/XOFF indications and requests.
 * These signals are used to perform flow control with AT command data
 * exchanged between the CI Task and the Multiplexer Task.
 */
typedef struct VgmuxXoffIndTag
{
  VgmuxChannelNumber  channelNumber;  /**< The multiplexer channel to perform flow control on. */
} VgmuxXoffInd, VgmuxXonInd, VgmuxXoffReq, VgmuxXonReq;

#if defined (FEA_PPP)
/** PPP Configure request.
 * This signal is used to configure a multiplexer channel for PPP data
 * communications.
 */
typedef struct VgmuxPppConfigReqTag
{
  VgmuxId             multiplexer;    /**< Multiplexer identifier. */
  DsPppConfigReq      pppConfig;      /**< The PPP configuration to apply. */
} VgmuxPppConfigReq;


typedef struct VgmuxPppFlushReqTag
{
    Int8 connId;
    Int8 domain;
}
VgmuxPppFlushReq;
#endif /* FEA_PPP */

/** ATO Command request.
 * This signal is used to perform the ATO command operation, switching from
 * command mode back to data mode.
 */
typedef struct VgmuxAtoCommandReqTag
{
  VgmuxChannelNumber channelNumber; /**< The multiplexer channel on which the ATO command was
                                     * issued. */
  VgmuxDomain        domain;        /**< The series of identifiers of the data connection. */
  Int8               connId;           /**< The identifier of the Data Connection to be closed. */
} VgmuxAtoCommandReq;

/** Received Line Status indication.
 * This signal is sent by the multiplexer to indicate the current line status
 * of a multiplexer channel.
 */
typedef struct VgmuxReceivedLineStatusIndTag
{
  VgmuxChannelNumber channelNumber;  /**< The multiplexer channel associated with supplied line
                                      * status information. */
  Int8               dtr;            /**< The DTR line status: 0 indicates low, 1 indicates high. */
  Int8               rfr;            /**< The RFR/RTS line status: 0 indicates low, 1 indicates
                                      * high. */
  Boolean            aBreak;         /**< TRUE if a break signal was detected. */
  Int8               breakLength;    /**< The break signal length (0-7) in 200mS units. */
} VgmuxReceivedLineStatusInd;

/** Transmit Line Status request.
 * This signal is sent by the CI Task to request a change in line status to
 * be communicated for the specified channel.
 */
typedef struct VgmuxTransmitLineStatusReqTag
{
  VgmuxChannelNumber channelNumber;  /**< The multiplexer channel associated with supplied line
                                      * status information. */
  Int8               dsr;            /**< The DSR line status. */
  Int8               cts;            /**< The CTS (Clear To Send) line status. */
  Int8               ri;             /**< The RI (Ring Indicator) line status. */
  Int8               dcd;            /**< The DCD (Data Carrier Detect) line status. */
  Boolean            aBreak;         /**< TRUE if multiplexer should generate a break signal. */
  Int8               breakLength;    /**< The break signal length (0-7) in 200mS units. */
} VgmuxTransmitLineStatusReq;

/** Multiplexer Start Timer request.
 * The signal sent by the multiplexer to itself to start timers.
 */
typedef struct VgmuxStartTimerReqTag
{
  Int8               timerType;      /**< The type of timer to start. */
  VgmuxChannelNumber channelNumber;  /**< The channel associated with the timer. */
} VgmuxStartTimerReq;

/* The signals used by the PC implementation of the DTE interface which   */
/* uses signals so that multiplexers can be tested on the PC using Genie  */

#if defined (ON_PC) && defined (VG_MUX_DTE_PC_ONLY_VERSION)

/** DTE Debug Event indication. */
typedef struct VgmuxDteDebugEventIndTag
{
  VgmuxDtePcEventType eventType;                      /**< PC Event Type. */
  Int16               length;                         /**< Length of supplied data. */
  Int8                data[VG_MUX_DTE_TX_BUFF_SIZE];  /**< Event data. */
} VgmuxDteDebugEventInd;

/** DTE Receive Data request. */
typedef struct VgmuxDteRxDataReqTag
{
  Int8  length;                         /**< Length of received data. */
  Int8  data[VG_MUX_DTE_RX_BUFF_SIZE];  /**< Received data. */
} VgmuxDteRxDataReq;
#endif


/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

/** @} */ /* End of SigVgMx group */
#endif /* UPGRADE_GENERIC_MUX */
#endif /* VGMX_SIG_H */

/* END OF FILE */

