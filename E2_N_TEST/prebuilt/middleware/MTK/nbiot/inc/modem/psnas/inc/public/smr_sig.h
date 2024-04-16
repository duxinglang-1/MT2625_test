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
 * File Description:                                                      */

/** \file
 * Eutra/Utra/GPRS PS Signal Data Type definitions for the SMR Interface
 **************************************************************************/

#ifndef SMR_SIG_H
#define SMR_SIG_H

/***************************************************************************
* Nested Include Files
***************************************************************************/

#include <system.h>
#include <gpqos.h>
#include <pdn_typ.h>
#include <pdp_typ.h>
#include <ptidef.h>
#include <sml3_typ.h>
#include <comp_typ.h>
#include <gpsapi.h>
#include <psdbearerdef.h>

/** \defgroup SigSmr SMR Signalling Interface
 * \ingroup Prd3gDmNas
 * \ingroup Sig
 * This document specifies the signal interface between Application
 * Background (AB) and Session Management (SM).
 * The interface between AB and SM is known as the SMR interface.
 * It describes the signals sent between these two entities in terms of data
 * structures and data values, and provides a summary of the purpose of each
 * signal, when it may be expected to occur, and how it relates to other
 * signals on the same interface.
 *
 * @{
 */

/** \defgroup SigSmrContext SMR Interface Context
 * This section defines the context of the SMR interface.
 *
 * @{
 * The SMR interface is illustrated in the Figure 1 below:
 *
 * \image html smr_interface_context_diagram.png "Figure 1: SMR Interface"
 */
/** @} */ /* End of SigSmrContext group */

/***************************************************************************
*   Manifest Constants
***************************************************************************/

/***************************************************************************
*   Macro Functions
***************************************************************************/

/***************************************************************************
*   Types
***************************************************************************/

/** Engineering mode data related to a PSD bearer context.
 */
typedef struct SmEngineeringModeInfoTag
{
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** TRUE if this is a default PSD bearer context, FALSE if a dedicated
     * PSD bearer context */
    Boolean                 defaultPsdBearer;
    /** PDN address */
    PdnAddress              pdnAddress;
    /** Indicates if APN is present */
    Boolean                 apnPresent;
    /** APN */
    AccessPointName         apn;
    /** Indicates if QoS is present */
    Boolean                 qosPresent;
    /** QoS */
    QualityOfService        qos;
    /** Indicates if APN-AMBR is present */
    Boolean                 apnAmbrPresent;
    /** APN-AMBR */
    ApnAmbr                 apnAmbr;
    /** Indicates if traffic flow template (TFT) is present */
    Boolean                 tftPresent;
    /** TFT */
    TrafficFlowTemplate     tft;
    /** Radio priority level (2G only) */
    RadioPriorityLevel      rpl;
    /** LLC SAPI (2G only) */
    LlcSapi                 sapi;
    /** Indicates if data compression is enabled and if it is enabled, the type of data
     * compression used (Only applicable for SNDCP - 2G)
     */
    DataCompType            dataComp;
    /** Indicates if header compression is enabled and if it is enabled, the type
     * of header compression used
     */
    HeaderCompType          headerComp;
}
SmEngineeringModeInfo;

/** \defgroup SigSmrSignals  SMR Primitives
 * Primitives exchanged between AB and SM.
 * This section defines primitives exchanged between AB and SM
 *
 * @{
 *
 */
/** This signal is sent from SM to AB to indicate a dedicated bearer activation request
 *  from the network.
 *
 *  In LTE: This originates from an ACTIVATE DEDICATED EPS BEARER CONTEXT REQUEST message.
 *
 *  In 2G/3G: This originates from an ACTIVATE SECONDARY PDP CONTEXT ACCEPT message.
 */
typedef struct SmrActivateDedicatedBearerIndTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Linked EPS bearer id or NSAPI */
    Int8                    linkedPsdBearerId;
    /** Indicates if QoS is present */
    Boolean                 qosPresent;
    /** QoS */
    QualityOfService        qos;
    /** Indicates if traffic flow template (TFT) is present */
    Boolean                 tftPresent;
    /** TFT */
    TrafficFlowTemplate     tft;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
SmrActivateDedicatedBearerInd;

/** This signal is sent from AB to SM as a negative response to a dedicated bearer activation request
 *  from the network.
 *
 *  In LTE: This is the response for an ACTIVATE DEDICATED EPS BEARER CONTEXT REQUEST message.
 *  ACTIVATE DEDICATED EPS BEARER CONTEXT REJECT message is sent to the network.
 *
 *  In 2G/3G: This is the response for an ACTIVATE SECONDARY PDP CONTEXT ACCEPT message,
 *  DEACTIVATE PDP CONTEXT REQUEST message is sent to the network.
 */
typedef struct SmrActivateDedicatedBearerRejTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Indicates the cause value */
    GsmCause                smCause;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
SmrActivateDedicatedBearerRej;

/** This signal is sent from AB to SM as a positive response to a dedicated bearer activation request
 *  from the network.
 *
 *  In LTE: This is the response for an ACTIVATE DEDICATED EPS BEARER CONTEXT REQUEST message.
 *  ACTIVATE DEDICATED EPS BEARER CONTEXT ACCEPT message is sent to the network.
 *
 *  In 2G/3G: This is the response for an ACTIVATE SECONDARY PDP CONTEXT ACCEPT message, but
 *  in this case no message is sent to the network.
 */
typedef struct SmrActivateDedicatedBearerRspTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
SmrActivateDedicatedBearerRsp;

/** This signal is sent from SM to AB to indicate a default bearer activation request
 *  from the network.
 *
 *  In LTE: This originates from an ACTIVATE DEFAULT EPS BEARER CONTEXT REQUEST message.
 *
 *  In 2G/3G: This originates from an ACTIVATE PDP CONTEXT ACCEPT message.
 */
typedef struct SmrActivateDefaultBearerIndTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** PDN address */
    PdnAddress              pdnAddress;
    /** Indicates if QoS is present */
    Boolean                 qosPresent;
    /** QoS */
    QualityOfService        qos;
    /** Indicates if APN is present */
    Boolean                 apnPresent;
    /** APN */
    AccessPointName         apn;
    /** Indicates if APN-AMBR is present */
    Boolean                 apnAmbrPresent;
    /** APN-AMBR */
    ApnAmbr                 apnAmbr;
    /** Indicates if the cause value is present */
    Boolean                 smCausePresent;
    /** Indicates the cause value */
    GsmCause                smCause;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
    /** Indicates if the serving PLMN rate control value is present */
    Boolean                 servingPlmnRateControlPresent;
    /** Serving PLMN rate control value */
    Int16                   servingPlmnRateControlValue;
}
SmrActivateDefaultBearerInd;

/** This signal is sent from AB to SM as a negative response to a default bearer activation request
 *  from the network.
 *
 *  In LTE: This is the response for an ACTIVATE DEFAULT EPS BEARER CONTEXT REQUEST message.
 *  ACTIVATE DEFAULT EPS BEARER CONTEXT REJECT message is sent to the network.
 *
 *  In 2G/3G: This is the response for an ACTIVATE PDP CONTEXT ACCEPT message,
 *  DEACTIVATE PDP CONTEXT REQUEST message is sent to the network.
 */
typedef struct SmrActivateDefaultBearerRejTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Indicates the cause value */
    GsmCause                smCause;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
SmrActivateDefaultBearerRej;

/** This signal is sent from AB to SM as a positive response to a default bearer activation request
 *  from the network.
 *
 *  In LTE: This is the response for an ACTIVATE DEFAULT EPS BEARER CONTEXT REQUEST message.
 *  ACTIVATE DEFAULT EPS BEARER CONTEXT ACCEPT message is sent to the network.
 *
 *  In 2G/3G: This is the response for an ACTIVATE PDP CONTEXT ACCEPT message, but
 *  in this case no message is sent to the network.
 */
typedef struct SmrActivateDefaultBearerRspTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
SmrActivateDefaultBearerRsp;

/** This signal is sent from SM to AB to indicate that a bearer resource allocation
 *  procedure has been completed and the PTI used in that procedure is freed.
 *
 *  In LTE: This completes a BEARER RESOURCE ALLOCATION procedure.
 *
 *  In 2G/3G: This completes an ACTIVATE SECONDARY PDP CONTEXT procedure.
 */
typedef struct SmrBearerResourceAllocCnfTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
}
SmrBearerResourceAllocCnf;

/** This signal is sent from SM to AB to indicate that the network has rejected a
 *  bearer resource allocation request.
 *
 *  In LTE: This originates from a BEARER RESOURCE ALLOCATION REJECT message.
 *
 *  In 2G/3G: This originates from an ACTIVATE SECONDARY PDP CONTEXT REJECT message.
 */
typedef struct SmrBearerResourceAllocRejTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Indicates the cause value */
    GsmCause                smCause;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
SmrBearerResourceAllocRej;

/** This signal is sent from AB to SM to initiate a bearer resource allocation
 *  procedure.
 *
 *  In LTE: This triggers a BEARER RESOURCE ALLOCATION procedure. BEARER RESOURCE
 *  ALLOCATION REQUEST message is sent to the network.
 *
 *  In 2G/3G: This triggers an ACTIVATE SECONDARY PDP CONTEXT procedure. ACTIVATE
 *  SECONDARY PDP CONTEXT REQUEST message is sent to the network.
 */
typedef struct SmrBearerResourceAllocReqTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** NSAPI allocated by ABPD required for 2G/3G only.  For LTE is it set to PSD_BEARER_ID_UNASSIGNED */
    Int8                    psdBearerId;
    /** Linked EPS bearer id or NSAPI */
    Int8                    linkedPsdBearerId;
    /** Indicates if traffic flow template (TFT) is present */
    Boolean                 tftPresent;
    /** TFT */
    TrafficFlowTemplate     tft;
    /** Indicates if QoS is present */
    Boolean                 qosPresent;
    /** QoS */
    QualityOfService        qos;
    /** Indicates if minimum QoS is present */
    Boolean                 minQosPresent;
    /** Minimum QoS */
    QualityOfService        minQos;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
    /** Indicates if data compression is enabled and if it is enabled, the type of data
     * compression used (Only applicable for SNDCP - 2G)
     */
    DataCompType            dataComp;
    /** Indicates if header compression is enabled and if it is enabled, the type
     * of header compression used
     */
    HeaderCompType          headerComp;
}
SmrBearerResourceAllocReq;

/** This signal is sent from SM to AB to indicate that a bearer resource modification
 *  procedure has been completed and the PTI used in that procedure is freed.
 *
 *  In LTE: This completes a BEARER RESOURCE MODIFICATION procedure.
 *
 *  In 2G/3G: This completes a MODIFY PDP CONTEXT (MS initiated) procedure.
 */
typedef struct SmrBearerResourceModifyCnfTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
}
SmrBearerResourceModifyCnf;

/** This signal is sent from SM to AB to indicate that the network has rejected a
 *  bearer resource modification request.
 *
 *  In LTE: This originates from a BEARER RESOURCE MODIFICATION REJECT message.
 *
 *  In 2G/3G: This originates from an MODIFY PDP CONTEXT REJECT message.
 */
typedef struct SmrBearerResourceModifyRejTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Indicates the cause value */
    GsmCause                smCause;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
    /** Indicates if the back-off timer is present */
    Boolean                 backOffTimerPresent;
    /** Back-off timer */
    GprsTimer3Element       backOffTimer;
    /* Indicates if the re-attempt indicator is present */
    Boolean                 reAttemptIndicatorPresent;
    /* Re-attempt Indicator */
    ReAttemptIndicator      reAttemptIndicator;
}
SmrBearerResourceModifyRej;

/** This signal is sent from AB to SM to initiate a bearer resource modification
 *  procedure.
 *
 *  In LTE: This triggers a BEARER RESOURCE MODIFICATION procedure. BEARER RESOURCE
 *  MODIFICATION REQUEST message is sent to the network.
 *
 *  In 2G/3G: This triggers a MODIFY PDP CONTEXT (MS initiated) procedure or
 *  DEACTIVATE PDP CONTEXT for secondary PDP context procedure. MODIFY
 *  PDP CONTEXT REQUEST or DEACTIVATE PDP CONTEXT REQUEST message is sent to the network.
 */
typedef struct SmrBearerResourceModifyReqTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Indicates if traffic flow template (TFT) is present */
    Boolean                 tftPresent;
    /** TFT */
    TrafficFlowTemplate     tft;
    /** Indicates if QoS is present */
    Boolean                 qosPresent;
    /** QoS */
    QualityOfService        qos;
    /** Indicates if minimum QoS is present */
    Boolean                 minQosPresent;
    /** Minimum QoS */
    QualityOfService        minQos;
    /** Indicates if the cause value is present, required when the UE
     * requests the release of a dedicated bearer resource */
    Boolean                 smCausePresent;
    /** Indicates the cause value */
    GsmCause                smCause;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
SmrBearerResourceModifyReq;

/** This signal is sent from SM to AB to inform AB of SM configuration.
 */
typedef struct SmrConfigIndTag
{
    /** Maximum MT PSD bearer contexts allowed (in 2G/3G) */
    Int8                    maxMtPsdBearerContextsAllowed;
}
SmrConfigInd;

/** This signal is sent from SM to AB to indicate a bearer deactivation request
 *  from the network.
 *
 *  In LTE: This originates from a DEACTIVATE EPS BEARER CONTEXT REQUEST message.
 *
 *  In 2G/3G: This originates from a DEACTIVATE PDP CONTEXT ACCEPT message.
 */
typedef struct SmrDeactivateBearerIndTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Indicates the cause value */
    GsmCause                smCause;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
    /** Indicates if the T3396 timer is present */
    Boolean                 t3396TimerPresent;
    /** T3396 timer */
    GprsTimer3Element       t3396Timer;
}
SmrDeactivateBearerInd;

/** This signal is sent from AB to SM as response to a bearer deactivation request
 *  from the network.
 *
 *  In LTE: This is the response for an DEACTIVATE EPS BEARER CONTEXT REQUEST message.
 *  DEACTIVATE EPS BEARER CONTEXT ACCEPT message is sent to the network.
 *
 *  In 2G/3G: This is the response for an DEACTIVATE PDP CONTEXT ACCEPT message, but
 *  in this case no message is sent to the network.
 */
typedef struct SmrDeactivateBearerRspTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
SmrDeactivateBearerRsp;

/** This signal is sent from SM to AB to indicate a bearer modification request
 *  from the network.
 *
 *  In LTE: This originates from an MODIFY EPS BEARER CONTEXT REQUEST message.
 *
 *  In 2G/3G: This originates from a MODIFY PDP CONTEXT REQUEST (Network to MS direction) message.
 */
typedef struct SmrModifyBearerIndTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Linked EPS bearer id or NSAPI */
    Int8                    linkedPsdBearerId;
    /** Indicates if QoS is present */
    Boolean                 qosPresent;
    /** QoS */
    QualityOfService        qos;
    /** Indicates if traffic flow template (TFT) is present */
    Boolean                 tftPresent;
    /** TFT */
    TrafficFlowTemplate     tft;
    /** Indicates if APN-AMBR is present */
    Boolean                 apnAmbrPresent;
    /** APN-AMBR */
    ApnAmbr                 apnAmbr;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
    /** PDN address */
    PdnAddress              pdnAddress;
}
SmrModifyBearerInd;

/** This signal is sent from AB to SM as a negative response to a bearer modification request
 *  from the network.
 *
 *  In LTE: This is the response for an MODIFY EPS BEARER CONTEXT REQUEST message.
 *  MODIFY EPS BEARER CONTEXT REJECT message is sent to the network.
 *
 *  In 2G/3G: This is the response for a MODIFY PDP CONTEXT REQUEST (Network to MS direction)
 *  message. MODIFY PDP CONTEXT REJECT message is sent to the network.
 */
typedef struct SmrModifyBearerRejTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Indicates the cause value */
    GsmCause                smCause;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
SmrModifyBearerRej;

/** This signal is sent from AB to SM as a positive response to a bearer modification request
 *  from the network.
 *
 *  In LTE: This is the response for an MODIFY EPS BEARER CONTEXT REQUEST message.
 *  MODIFY EPS BEARER CONTEXT ACCEPT message is sent to the network.
 *
 *  In 2G/3G: This is the response for a MODIFY PDP CONTEXT REQUEST (Network to MS direction)
 *  message. MODIFY PDP CONTEXT ACCEPT message is sent to the network.
 */
typedef struct SmrModifyBearerRspTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
SmrModifyBearerRsp;

/** This signal is sent from SM to AB to indicate that a PDN connectivity procedure
 *  has been completed and the PTI used in that procedure is freed.
 *
 *  In LTE: This completes a PDN CONNECTIVITY procedure.
 *
 *  In 2G/3G: This completes an ACTIVATE PDP CONTEXT procedure.
 */
typedef struct SmrPdnConnectCnfTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
}
SmrPdnConnectCnf;

/** This signal is sent from SM to AB to indicate a PDP context activation request
 *  from the network (MT PDP Context request), only valid for 2G/3G.
 *
 *  In LTE: Not applicable.
 *
 *  In 2G/3G: This originates from a REQUEST PDP CONTEXT ACTIVATION message.
 */
typedef struct SmrPdnConnectIndTag
{
    /** PDN address */
    PdnAddress              pdnAddress;
    /** Indicates if APN is present */
    Boolean                 apnPresent;
    /** APN */
    AccessPointName         apn;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
SmrPdnConnectInd;

/** This signal is sent from SM to AB to indicate that the network has rejected a
 *  PDN connectivity request, or from AB to SM to reject a MT PDP Context request
 *  in 2G/3G.
 *
 *  In LTE: This originates from a PDN CONNECTIVITY REJECT message.
 *
 *  In 2G/3G: This originates from an ACTIVATE PDP CONTEXT REJECT message (SM to AB
 *  direction), or this is to reject a REQUEST PDP CONTEXT ACTIVATION message,
 *  and REQUEST PDP CONTEXT ACTIVATION REJECT message is sent to the network (AB to
 *  SM direction).
 */
typedef struct SmrPdnConnectRejTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Required for 2G/3G only for MT PDP context activation reject by ABPD.
     * PDN address identifies the MT PDP context
     */
    PdnAddress              pdnAddress;
     /** Required for 2G/3G only for MT PDP context activation reject by ABPD.
      * Indicates if APN is present
      */
    Boolean                 apnPresent;
    /** Required for 2G/3G only for MT PDP context activation reject by ABPD. APN
     * used to identify the MT PDP context
     */
    AccessPointName         apn;
    /** Indicates the cause value */
    GsmCause                smCause;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
    /** Indicates if the back-Off timer is present */
    Boolean                 backOffTimerPresent;
    /** Back-off timer */
    GprsTimer3Element       backOffTimer;
    /* Indicates if the re-attempt indicator is present */
    Boolean                 reAttemptIndicatorPresent;
    /** Re-attempt Indicator */
    ReAttemptIndicator      reAttemptIndicator;
    /** Security Header Type from EMM's pdu */
    EpsSecurityHeaderType   securityHeadertype;
}
SmrPdnConnectRej;

/** This signal is sent from AB to SM to initiate a PDN connectivity procedure.
 *
 *  In LTE: This triggers a PDN CONNECTIVITY procedure. PDN CONNECTIVITY REQUEST
 *  message is sent to the network.
 *
 *  In 2G/3G: This triggers an ACTIVATE PDP CONTEXT procedure. ACTIVATE
 *  PDP CONTEXT REQUEST message is sent to the network.
 */
typedef struct SmrPdnConnectReqTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** NSAPI allocated by ABPD required for 2G/3G only.  For LTE is it set to PSD_BEARER_ID_UNASSIGNED */
    Int8                    psdBearerId;
    /** PDN address */
    PdnAddress              pdnAddress;
    /** Indicates if QoS is present */
    Boolean                 qosPresent;
    /** QoS */
    QualityOfService        qos;
    /** Indicates if minimum QoS is present */
    Boolean                 minQosPresent;
    /** Minimum QoS */
    QualityOfService        minQos;
    /** Indicates if APN is present */
    Boolean                 apnPresent;
    /** APN */
    AccessPointName         apn;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
    /** Indicates if UL PCO shall be sent ciphered */
    Boolean                 pcoCipheringNeeded;
    /** Indicates if data compression is enabled and if it is enabled, the type of data
     * compression used (Only applicable for SNDCP - 2G)
     */
    DataCompType            dataComp;
    /** Indicates if header compression is enabled and if it is enabled, the type
     * of header compression used
     */
    HeaderCompType          headerComp;
    /** Indicates if device properties is present. */
    Boolean                 devicePropertiesPresent;
    /** The UE shall include this IE if UE is configured for NAS signalling
    * low priority
    */
    DevicePropertiesElement deviceProperties;
}
SmrPdnConnectReq;

/** This signal is sent from SM to AB to indicate that a PDN disconnect procedure
 *  has been completed and the PTI used in that procedure is freed.
 *
 *  In LTE: This completes a PDN DISCONNECT procedure (even if it is rejected by network).
 *
 *  In 2G/3G: This completes a DEACTIVATE PDP CONTEXT procedure.
 */
typedef struct SmrPdnDisconnectCnfTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** Indicates if the cause value is present, required when the network
     * has rejected the disconnect */
    Boolean                 smCausePresent;
    /** Indicates the cause value */
    GsmCause                smCause;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
SmrPdnDisconnectCnf;

/** This signal is sent from AB to SM to initiate a PDN disconnect procedure.
 *
 *  In LTE: This triggers a PDN DISCONNECT procedure. PDN DISCONNECT REQUEST
 *  message is sent to the network.
 *
 *  In 2G/3G: This triggers an DEACTIVATE PDP CONTEXT procedure. DEACTIVATE
 *  PDP CONTEXT REQUEST message is sent to the network.
 */
typedef struct SmrPdnDisconnectReqTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Indicates the cause value */
    GsmCause                smCause;
    /** Indicates if the protocol configuration options is present */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
SmrPdnDisconnectReq;

/** This signal is sent from SM to AB to request a PDN Connect Request during attach
 * procedure, only valid for LTE.
 *
 *  In LTE: AB responds with PDN Connect Request if attachWithoutPdn is FALSE.
 *
 *  In 2G/3G: Not applicable.
 */
typedef struct SmrPdnInfoIndTag
{
    /** Indicates if attached is done without PDN */
    Boolean                 attachWithoutPdn;
}
SmrPdnInfoInd;

/** This signal is sent from AB to SM to reject a PDN Connect Request from SM
 *  during attach procedure, only valid for LTE.
 *
 *  In LTE: This will cause the attach procedure to stop.
 *
 *  In 2G/3G: Not applicable.
 */
typedef struct SmrPdnInfoRejTag
{
    /** Indicates the cause value */
    GsmCause                cause;
}
SmrPdnInfoRej;

/** This signal is sent from SM to AB to indicate reception of a status
 *  message from the network.
 *
 *  In LTE: This originates from an ESM STATUS message.
 *
 *  In 2G/3G: This originates from an SM STATUS message.
 */
typedef struct SmrStatusIndTag
{
    /** Procedure Transaction Identity */
    Int8                    pti;
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Indicates the cause value */
    GsmCause                smCause;
}
SmrStatusInd;

/** This signal is sent from SM to AB to inform AB of changes to the
 * engineering data if SM engineering mode has been enabled, after every
 * PSD bearer context activation, deactivation, or modification.
 */
typedef struct SmrTestInfoIndTag
{
    /* Workaround! This signal shall be cleaned from the code  */
    /* as its not used by ESM and its size exceeds 20KB.       */
    /*                                                         */
    /* Defined now as an empty signal.                         */
    /*                                                         */
    Int8                    empty;

    /** Indicates the number of PSD bearers   */
    /*  Int8                    numPsdBearers; */
    /** Engineering data information */
    /*  SmEngineeringModeInfo   smEngineeringModeInfo[MAX_NUM_PSD_BEARERS]; */
}
SmrTestInfoInd;

/** This signal is sent from AB to ESM to configure behaviour
 * of the ESM protocol.
 *
 */
typedef struct SmrBearerConfigureReqTag
{
    /** Release Assistance Indication */
    Int8                    releaseAssistanceInd;
}
SmrBearerConfigureReq;

/** This signal is sent from ESM to AB as response for the configuration
 * request.
 *
 */
typedef struct SmrBearerConfigureCnfTag
{
    /** Indicates if the Release Assistance Indication has been sent in UL */
    Boolean                 raiSent;
}
SmrBearerConfigureCnf;


/** TFT structure sent from ESM to AB
 *
 */
typedef struct SmrTftInfoTag
{
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Indicates if traffic flow template (TFT) is present */
    Boolean                 tftPresent;
    /** TFT */
    TrafficFlowTemplate     tft;
}
SmrTftInfo;

/** This signal is sent from ESM to AB to initialise
 *  TFTs after waking up from sleep
 *
 */
typedef struct SmrInitialiseTftIndTag
{
    SmrTftInfo              tftInfo[MAX_NUM_OF_STORED_CONTEXTS];
}
SmrInitialiseTftInd;


/** QoS structure sent from ESM to AB
 *
 */
typedef struct SmrQosInfoTag
{
    /** EPS bearer id or NSAPI */
    Int8                    psdBearerId;
    /** Indicates if QoS is present */
    Boolean                 qosPresent;
    /** QoS */
    QualityOfService        qos;
}
SmrQosInfo;

/** This signal is sent from ESM to AB to initialise
 *  QoS parameters after waking from sleep
 *
 */
typedef struct SmrInitialiseQosIndTag
{
    SmrQosInfo              qosInfo[MAX_NUM_OF_STORED_CONTEXTS];
}
SmrInitialiseQosInd;

/** This signal is sent from AB to ESM to request
 *  initialisation in start up
 *
 */
typedef struct SmrInitEsmReqTag
{
    /** An empty signal */
    Int8                    empty;
}
SmrInitEsmReq;

/** @} */ /* End of SigSmrSignals group */

/** @} */ /* End of SigSmr group */

#endif /* SMR_SIG_H */

/* END OF FILE */

