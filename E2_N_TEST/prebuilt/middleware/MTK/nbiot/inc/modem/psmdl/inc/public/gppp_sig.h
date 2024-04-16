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
 * Definition of signals sent between the PPP task and the AB task ABPD
 * module.
 **************************************************************************/

#ifndef GPPP_SIG_H
#define GPPP_SIG_H

/**** NESTED INCLUDE FILES *************************************************/

#include <gpsystem.h>
#include <sml3_typ.h>    /* for gsmcause */
#include <cimux_typ.h>   /* for flow-control definition */
#include <pdp_typ.h>
#include <pdn_typ.h>
#include <ppp_typ.h>

/** \defgroup SigGppp GPPP Signalling Interface
 * \ingroup PrdMW
 * \ingroup Sig
 * Interface between the Point to Point Protocol (PPP) task and the
 * Application Background (AB) task ABPD module.
 * This section defines the interface between the PPP task and
 * the AB task. This interface is known as GPPP.
 *
 * The PPP task is a slave to the AB task.  ABPD is the main controller
 * of PSD connections and if the connection with the PC is via PPP then
 * ABPD instructs the PPP task at the appropriate times when to start or end
 * negotiations with the peer PPP on the PC.  PPP can also indicate to ABPD
 * if a disconnection via the PPP protocol is received from the peer PPP on the
 * PC.
 * The PPP task provides a mechanism through which PSD functionality
 * can be offered to a terminal that would normally be able to communicate
 * to the Internet with a PPP dial-up link (e.g. a standard Windows PC).
 * PPP is implemented according to RFC 1661 and RFC 1662.
 *
 * The context of GPPP is shown in Figure below:
 *
 * \image html GPPP_interface.png
 * @{
 */
/**** CONSTANTS ************************************************************/

/**** ENUMS ****************************************************************/

/**** TYPEDEFS *************************************************************/

/** This signal is sent by ABPD to PPP to request that PPP start negotiation
 * with the PPP peer on the PC.
 */
typedef struct GpppNegStartReqTag
{
    /** Flow control mechanism.  PPP needs to know if we are using S/W
     * flow control in order to manage escaping of XON/XOFF characters.
     */
    FlowCtrlType        flowControlType;
    /** Specifies the type of PPP connection */
    PppConnectionType   connectionType;
    /** Channel Number */
    Int32               channelNumber;
#if defined (ENABLE_PPP_RAW_LOGGING)
    /** Raw logging level */
    Int8                rawLoggingLevel;
    /** Raw logger task */
    TaskId              rawLoggerTask;
#endif
}
GpppNegStartReq;

/** This signal is sent by PPP ABPD to confirm that that PPP negotiation
 * with the PPP peer on the PC is now completed.
 */
typedef struct GpppNegStartCnfTag
{
    /** Whether negotiations was successful or not */
    Boolean     success;    
    /** If success == FALSE, indicate the failure cause */
    GsmCause    cause;    
    /** Channel Number */
    Int32       channelNumber;
}
GpppNegStartCnf;

/** This signal is sent by PPP to APBD to indicate that ABPD can activate the
 * PSD connection with the information supplied by the PPP peer running on the
 * PC (username, password, DNS addresses, IP Address).
 */
typedef struct GpppPsdActivateIndTag
{
    /** Username/password */
    PsdUser                   psdUser;
    /** IP Address, DNS and Gateway addresses */
    PdnConnectionAddressInfo  pdnConnectionAddressInfo;    
    /** Channel Number */
    Int32                     channelNumber;
}
GpppPsdActivateInd;

/** This signal is sent by ABPD to PPP to indicate that ABPD has now activated
 * the PSD connection and supplies the relevent information to PPP to send to
 * the peer PPP on the PC (DNS addresses, IP address).
 */
typedef struct GpppPsdActivateRspTag
{
    /** IP Address, DNS and Gateway addresses */
    PdnConnectionAddressInfo  pdnConnectionAddressInfo;    
    /** Channel Number */
    Int32                     channelNumber;
}
GpppPsdActivateRsp;

/** This signal is sent by ABPD to PPP to indicate that the PPP connection
 * information (username and password) are incorrect and so we must indicate to
 * the peer PPP on the PC to terminate the PPP connection.
 */
typedef struct GpppPsdActivateRejTag
{
    /** The cause of reject */
    GsmCause    cause;    
    /** Channel Number */
    Int32       channelNumber;
}
GpppPsdActivateRej;

/** This signal is sent by ABPD to PPP to request that PPP terminate the
 * connection with the PPP peer on the PC.
 */
typedef struct GpppPsdDeactivateReqTag
{
    /** Channel Number */
    Int32       channelNumber;
}
GpppPsdDeactivateReq;

/** This signal is sent by PPP to ABPD to confirm that the peer PPP
 * on the PC has now responded to the modem request to terminate the PPP
 * connection.
 */
typedef struct GpppPsdDeactivateCnfTag
{
    /** Channel Number */
    Int32       channelNumber;
}
GpppPsdDeactivateCnf;

/** This signal is sent by PPP to ABPD to indicate that the PPP connection
 * with the peer on the PC has been deactivated.  This would normally occur
 * when the PC initiates a PPP disconnect.
 */
typedef struct GpppPsdDeactivateIndTag
{
    /** The cause of deactivation */
    GsmCause    cause;    
    /** Channel Number */
    Int32       channelNumber;
}
GpppPsdDeactivateInd;

/** Loopback request.
 * Sent from ABPD to PPP to enable or disable PSD PPP loopback for test purposes.
 * This signal is forwarded by ABPD from original signal sent by ATCI task.
 */
typedef struct PppLoopbackReqTag
GpppLoopbackReq;

/** Sent from PPP to ABPD to confirm the previous GpppLoopbackReq signal.
 * This signal is then forwarded by ABPD to ATCI task.
 */
typedef struct PppLoopbackCnfTag
GpppLoopbackCnf;

/** Configuration request sent from ABPD to PPP to
 * change parameters for PPP timers.
 * The original request is sent by ATCI task to ABPD.
 */
typedef struct PppConfigReqTag
GpppConfigReq;

/** This signal is sent as a response to GpppConfigReq.
 * This response is then forwared to the ATCI task by ABPD.
 */
typedef struct PppConfigCnfTag
GpppConfigCnf;

/** Sent from APB to PPP to configure the authentication mode for PSD connections.
 * The original request is sent by ATCI task to ABPD.
 */
typedef struct PppConfigAuthReqTag
GpppConfigAuthReq;

/** Sent from PPP to ABPD in response to a GpppConfigAuthReq.
 * Confirmation of the success or failure of the request
 * to change the PPP authentication mode for PSD connections.
 * This response is then forwared to the ATCI task by ABPD.
 */
typedef struct PppConfigAuthCnfTag
GpppConfigAuthCnf;

/** @} */ /* End of SigGpcippp group */
#endif

/* END OF FILE */
