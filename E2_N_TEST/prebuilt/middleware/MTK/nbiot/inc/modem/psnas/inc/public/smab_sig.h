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
 * definition of signals sent between the SM task and ABGP (Application
 * Background Layer GPRS task)
 * These signals are primarily used to set up a mobile terminated session
 * and to monitor the quality of service of an existing session.
 **************************************************************************/

#ifndef SMAB_SIG_H
#define SMAB_SIG_H

/**** NESTED INCLUDE FILES *************************************************/

#include <gpsystem.h>
#include <gpnsapi.h>
#include <gpsapi.h>
#include <gpqos.h>
#include <pdp_typ.h>
#include <gpcntr.h>


/** \defgroup SigAbgpsm GPABGPSM Signalling Interface
 * \ingroup Prd3gDmNas
 * \ingroup Sig
 * Interface between the SM task and ABGP (Application Background
 * Layer task GPRS module).
 * This section defines signaling interface between the SM task and ABGP 
 * (Application Background Layer task GPRS module). This interface is known as GPABGPSM.
 *
 * The GPABGPSM signals are primarily used to set up a mobile terminated PDP context
 * and to monitor the quality of service of an existing session.
 * @{
 */

/**** TYPEDEFS *************************************************************/
/** Engineering mode information element. 
 * Engineering data information related to a PDP context which 
 * has activated, deactivated or modified.
 */
typedef struct SmEngModeInfoTag
{
    /** NSAPI of the defined context */
    Nsapi               nsapi;
    /** PDP type number of the defined context */
    PdpTypeNumber       pdpTypeNumber;
    /** PDP address of the defined context  */
    PdpAddress          pdpAddress;
    /** Indicates if APN is present */
    Boolean             apnPresent;
    /** APN */
    AccessPointName     apn;
    /** Radio priority level */
    RadioPriorityLevel  rpl;
    /** Negotiated QoS */
    QualityOfService    negotiatedQos;
    /** SAPI */
    LlcSapi             sapi;
}
SmEngModeInfo;

/**** signal definitions ****/

/** This signal is sent by SM to inform ABGP of changes to the current quality of
 * service of a context activation. When ABGP receives this signal it sends
 * an ApexAbgpQosInd signal to all registered foreground layer tasks. When an
 * NSAPI is initially allocated the receipt of the first AbgpSmQosInd signal
 * indicates to ABGP that the context has been activated.
*/
typedef struct AbgpSmQosIndTag
{
    /** Identifies the established context whom the QoS has changed */
    Nsapi               nsapi;
    /** New QoS */
    QualityOfService    qos;
    /** PDP type number */
    PdpTypeNumber       pdpTypeNumber;
    /** PDP address */
    PdpAddress          pdpAddress;
}
AbgpSmQosInd;


/** Indication of that the network wishes to set up a session.
 * This signal is sent by SM indicating that the GPRS network wishes to
 * set up a context of the specified type. ABGP will allocate an NSAPI and
 * send ApexAbgpSetupInd signals to all registered foreground layer tasks.
 */
typedef struct AbgpSmRegPdpActivateIndTag
{
    /** PDP type number for the context to allocate */
    PdpTypeNumber       pdpTypeNumber;
    /** PDP address for the context to allocate */
    PdpAddress          pdpAddress;
    /** Indicates if APN is present 
\assoc PRESENT \ref apn */
    Boolean             apnPresent;
    /** APN to be used for any new PDP contexts */
    AccessPointName     apn;
}
AbgpSmRegPdpActivateInd;

/** Determines the outcome of the context activation request */
typedef enum ActRspStatusTag
{
    /** Activation response was OK */
    ACT_RSP_OK,
    /** No Free NSAPIs available*/
    ACT_RSP_NO_FREE_NSAPIS,
    /** BL is not ready */
    ACT_RSP_BL_IS_NOT_READY,
    /** Timer has expired */
    ACT_RSP_SETUP_IND_TIMER_EXPIRED,
    /** Foreground rejected */
    ACT_RSP_FG_REJECTED
}
ActRspStatus;

/** This is a response from a ABGP to an AbgpSmRegPdpActivateInd. 
 * This signal is sent by ABGP to SM either as a result of a foreground
 * layer task accepting a context activation request, (in which the
 * reason will be ACT_RSP_OK) or as a result of a failure to accept a
 * context activation request (other reasons). A rejection may occur if
 * no foreground layer task accepts the request or if there are no free
 * NSAPIs. If the activation request is accepted, nsapi will contain the
 * NSAPI that SM should use in all future references to this context and
 * taskId will contain the TaskId of the foreground layer task that is
 * willing to accept the request. If the signal indicates a failure to
 * accept the signal (reason not ACT_RSP_OK), nsapi and taskId are
 * undefined.
 */
typedef struct AbgpSmRegPdpActivateRspTag
{
    /** Indicates the result of the request (the request is 
     * accepted iff the cause is ACT_RSP_OK)
     */
    ActRspStatus            reason;
    /** Task ID of the foreground layer task that is
     * willing to accept the request (if the signal indicates 
     * a failure taskId is undefined)
     */
    TaskId                  pdpTask;
    /** NSAPI that SM should use in all future references to 
     * this context (if the signal indicates a failure NSAPI is 
     * undefined)
     */
    Nsapi                   nsapi;
    /** PDP type number for the allocated context */ 
    PdpTypeNumber           pdpTypeNumber;
    /** PDP address for the allocated context */ 
    PdpAddress              pdpAddress;
    /** Indicates if APN is present 
\assoc PRESENT \ref apn */
    Boolean                 apnPresent;
    /** APN to be used for any new PDP contexts */
    AccessPointName         apn;
}
AbgpSmRegPdpActivateRsp;

/** This signal is sent by SM to inform ABGP of changes to the 
 * engineering data.
 * This signal is used to report relevant PDP context information 
 * if SM engineering mode has been enabled. It is passed up to the 
 * background layer after every PDP context activation, deactivation 
 * or modification.
 */
typedef struct AbgpSmTestInfoIndTag
{
    /** Indicates the number of NSAPIs 
\assoc ARRAY \ref smEngModeInfo */
    Int8                numNsapis;
    /** Engineering data information */
    SmEngModeInfo       smEngModeInfo[MAX_NUM_NSAPIS];
}
AbgpSmTestInfoInd;

/** Sends an indication to ABGP with SM configuration. 
 * This signal is sent by SM to inform ABGP of SM configuration.
 */
typedef struct AbgpSmConfigIndTag
{
    /** Maximum MT PDP activations allowed */
    Int8             maxMtPdpActAllowed;
}
AbgpSmConfigInd;

/*
Counter Signals.
*/

/** Data volume counters report request to SM */
typedef struct ReportCounterReqTag
AbgpSmReportCounterReq;

/** Data volume counters reset request to SM */
typedef struct ResetCounterReqTag
AbgpSmResetCounterReq;

/** Data volume counters reported from SM to ABGP */
typedef struct CounterIndTag
AbgpSmCounterInd;

/** @} */ /* End of SigAbgpsm group */
#endif


/* END OF FILE */
