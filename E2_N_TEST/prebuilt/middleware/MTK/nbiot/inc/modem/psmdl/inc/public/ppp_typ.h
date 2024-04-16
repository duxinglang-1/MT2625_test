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
 **************************************************************************
 * File Description:                                                     */
/** \file
 * PPP definitions - Point to Point Protocol information is used in various
 * different interface signals
 *
 ***************************************************************************/

#ifndef PPP_TYP_H
#define PPP_TYP_H


/**** NESTED INCLUDE FILES *************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif

/**** CONSTANTS ************************************************************/

/**** TYPEDEFS *************************************************************/

/**  PPP connection type element */
typedef enum PppConnectionTypeTag
{
   /* TODO: Remove NO_PPP_NEGOTIATION for UPGRADE_NASMDL2 */
   /** No negotiation of PPP configuration options */
   NO_PPP_NEGOTIATION,
   /** Negotiation of PPP configuration options with standard PPP */
   PPP_NEGOTIATION,
   /** Negotiation of PPP configuration options with Core-PPP */
   PPP_NEGOTIATION_CORE
}
PppConnectionType;

/** The possible types of authentication to be used in PPP.
 * These values must not be changed as they are used for the
 * AT+CGAUTH AT command
 */
typedef enum PppAuthTypeTag
{
    /** None */
    PPP_AUTH_NONE = 0,
    /** Protocol C023 */
    PPP_AUTH_PAP,
    /** Protocol C223, algorithm 05 */
    PPP_AUTH_CHAP,
    /** Protocol C223, algorithm 80 */
    PPP_AUTH_MS_CHAP_V1,
    /** The number of authentication elements */
    PPP_AUTH_NUM
}
PppAuthType;

/** PPP hang-up type.  It determines the reason for hanging-up.
 */
typedef enum PppHangupTypeTag
{
    /** ATH command received from PC */
    HANGUP_TYPE_ATH,
    /** DTR handshake control line from the PC is dropped */
    HANGUP_TYPE_DTR_DROPPED
}
PppHangupType;

/** This enumeration lists the timers and counters that can be configured in PPP
 * using AT*MPPPCONFIG command. It is the first parameter (index) for the
 * AT command.
 */
typedef enum PppConfigIndexTag
{
    /** Delay after sending CONNECT response and starting PPP protocol state machine */
    PPP_CFG_INDEX_START_TIMER,                /* Guard timer */
    /** Guard timer for the time between starting LCP and reaching LCP Opened state */
    PPP_CFG_INDEX_LCP_NEGOTIATION_GUARD_TIMER, /* Guard timer */
    /** Time between (re)sending LCP-Configure-Req and receiving LCP-Configure-Ack/Nak/Rej */
    PPP_CFG_INDEX_LCP_CONFIG_TIMER,
    /** Time between (re)sending LCP-Terminate-Req and receiving LCP-Terminate-Ack */
    PPP_CFG_INDEX_LCP_TERMIN_TIMER,
    /** Time between sending each Echo request */
    PPP_CFG_INDEX_ECHO_REQUEST_TIMER,
    /** CHAP authentication guard timer */
    PPP_CFG_INDEX_CHAP_GUARD_TIMER,           /* Guard timer */
    /** Time between (re)sending CHAP-Authentication-Req and receiving CHAP-Authentication-Res */
    PPP_CFG_INDEX_CHAP_RETRY_TIMER,
    /** PAP authentication guard timer */
    PPP_CFG_INDEX_PAP_GUARD_TIMER,            /* Guard timer */
    /** Time between (re)sending PAP-Authentication-Req and receiving PAP-Authentication-Res */
    PPP_CFG_INDEX_PAP_RETRY_TIMER,
    /** Guard timer for the time between starting IPCP and reaching IPCP Opened state */
    PPP_CFG_INDEX_IPCP_NEGOTIATION_GUARD_TIMER, /* Guard timer */
    /** Time between (re)sending IPCP-Configure-Req and receiving IPCP-Configure-Ack/Nak/Rej */
    PPP_CFG_INDEX_IPCP_CONFIG_TIMER,
    /** Time between (re)sending IPCP-Terminate-Req and receiving IPCP-Terminate-Ack */
    PPP_CFG_INDEX_IPCP_TERMIN_TIMER,
    /** Time between (re)sending IPV6CP-Configure-Req and receiving IPV6CP-Configure-Ack/Nak/Rej */
    PPP_CFG_INDEX_IPV6CP_CONFIG_TIMER,
    /** Time between (re)sending IPV6CP-Terminate-Req and receiving IPV6CP-Terminate-Ack */
    PPP_CFG_INDEX_IPV6CP_TERMIN_TIMER,
    /** Number of PPP configuration indices */
    PPP_CFG_INDEX_NUM
}
PppConfigIndex;

/** Parameters for PPP timer */
typedef struct PppTimerConfigTag
{
    /** Index to the timer  */
    PppConfigIndex    index;
    /** Final value */
    Int32             finalValue;
    /** Initial value */
    Int32             initValue;
    /** Number of tries */
    Int8              tries;
}
PppTimerConfig;

/** Loopback request.
 * Send to enable or disable PPP loopback for test purposes.
 */
typedef struct PppLoopbackReqTag
{
    /** Used to enable (TRUE) or disable (FALSE) the loopback mode */
    Boolean     enableLoopback;
    /** The number of duplicated DL packets that will be transmitted by
     * MS per received UL packet (if > 1, a packet burst is sent, consisting
     * of numDlXmit packets)
     */
    Int8        numDlXmit;
    /** Sets the loopback mode: 0 - normal loopback, 1 - DL only, 2 - UL only */
    Int8        loopbackMode;
    /** The time in milliseconds between each packet or packet burst
     * in DL mode; if 0, packets are sent continuously; default is 0
     */
    Int16       dlTimeout;
    /** Packet size in bytes for DL packets transmitted by MS (in mode 1) or
     * UL packets received by MS (in mode 2)
     */
    Int16       packetLength;
    /** The total number of packets or packet bursts transmitted by MS in
     * DL (in mode 1) or received by MS (in mode 2), 0 means infinite
     */
    Int16       totalNumDlXmit;
    /** If TRUE, PPP checks the whole packet, otherwise just length
     */
    Boolean     loopbackUlChecking;
    /** If TRUE, byte stuffing is enabled, this changes the pattern
     */
    Boolean     byteStuffingEnabled;
    /** Channel Number */
    Int32       channelNumber;
}
PppLoopbackReq;

/** Sent to confirm the previous PppLoopbackReq signal.
 */
typedef struct PppLoopbackCnfTag
{
    /** Shows whether the operation was successful or not */
    Boolean     success;
    /** Channel Number */
    Int32       channelNumber;
}
PppLoopbackCnf;

/** Configuration request sent to change parameters for PPP timers.
 */
typedef struct PppConfigReqTag
{
    /** If TRUE, update the value, else get the value */
    Boolean           updateMode;
    /** Updated parameters for PPP timers (used only if
     * updateMode is TRUE )
     */
    PppTimerConfig    pppTimerConfig;
}
PppConfigReq;

/** This signal is sent as a response to PppConfigReq.
 */
typedef struct PppConfigCnfTag
{
    /** Whether or not the request was successful */
    Boolean           success;
    /** If TRUE, values were updated, else values were read */
    Boolean           updateMode;
    /** Not present if updateMode */
    PppTimerConfig    pppTimerConfig[PPP_CFG_INDEX_NUM];
}
PppConfigCnf;

/** Sent to configure the authentication mode for PSD connections.
 */
typedef struct PppConfigAuthReqTag
{
    /** If TRUE, update the value, otherwise get the value */
    Boolean       updateMode;
    /** The type of authentication to be configured */
    PppAuthType   pppAuthType;
    /** Channel Number */
    Int32         channelNumber;
}
PppConfigAuthReq;

/** Sent in response to a PppConfigAuthReq.
 * Confirmation of the success or failure of the request
 * to change the PPP authentication mode for PSD connections.
 */
typedef struct PppConfigAuthCnfTag
{
    /** Whether or not the request was successful */
    Boolean       success;
    /** If TRUE, update the value, otherwise get the value */
    Boolean       updateMode;
    /** The type of authentication */
    PppAuthType   pppAuthType;
    /** Channel Number */
    Int32         channelNumber;
}
PppConfigAuthCnf;

/** PPP link time settings element */
typedef struct PppLinkTimeSettingsTag
{
  /** Indicates if APN is present
\assoc PRESENT \ref apn */
  Boolean                 apnPresent;
  /** APN */
  AccessPointName         apn;
  /** Whether or not the configutation options are present
\assoc PRESENT \ref pppConfigOpts */
  Boolean                 pppConfigOptsPresent;
  /** This IE is included in the message when the MS provides protocol
   * configuration options for the external PDN
   */
  ProtocolConfigOptions   pppConfigOpts;
}
PppLinkTimeSettings;


#endif /* PPP_TYP_H */

/* END OF FILE */

