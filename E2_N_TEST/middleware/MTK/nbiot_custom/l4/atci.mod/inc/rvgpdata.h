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
 * File Description
 * ----------------
 * Exported data for the gprs sub-sys
 **************************************************************************/

#ifndef RVGPDATA_H
#define RVGPDATA_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>


#include <gpcntr.h>

#include <abpd_sig.h>

#include <rvsystem.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/
#if defined(FEA_TEMP_REL14_ABPD_DUAL_PRIORITY)
/** Indicates the NAS signalling priority
 */
typedef enum NasSigPrioTypeTag
{
    VG_MS_CONFIGURED_FOR_LOW_PRIO,           /**< Low priority configured in MS */
    VG_MS_NOT_CONFIGURED_FOR_LOW_PRIO        /**< MS is not configured for low priority */
} VgNasSigPrioType;
#endif

/* PSD bearer information definitions */
/* The information in this structure is use in the ApbdDialReq message and also
 * copied from the AbpdConnectedInd information.
 */
typedef struct VgPsdBearerInfoTag
{
  /** Channel Number associated with the cid */
  Int32                     channelNumber;
  /** Specifies the type of connection */
  AbpdPdpConnType           connType;
  /** Connection ID */
  Int8                      connId;
  /** PSD Bearer ID (NSAPI for 2G/3G, EPS bearer ID for LTE) */
  Int8                      psdBearerId;
  /** Username and password.  Required when connType is Packet Transport */
  PsdUser                   psdUser;
  /** Has PSD context information been altered by SIM as part of SIM toolkit
   * CC of PSD activation
   */
  Boolean                   modifiedBySim;
  /** Bitmap of modifications to the PSD bearer by the SIM
   */
  Int8                      simMods;

  /** Requested APN present */
  Boolean                   reqApnPresent;
  /** Requested APN in network format */
  AccessPointName           reqApn;
  /** Requested APN in plain text format. */
  TextualAccessPointName    reqTextualApn;
  /** Negotiated APN present */
  Boolean                   negApnPresent;
  /** Negotiated APN in network format. It may have been changed by the SIM in ABPD */
  AccessPointName           negApn;
  /** Negotiated APN in plain text format. It may have been changed by the SIM in ABPD */
  TextualAccessPointName    negTextualApn;
  /** IP Address requested by ATCI */
  PdnAddress                reqPdnAddress;
  /** IP Address, DNS and Gateway addresses assigned by the network */
  PdnConnectionAddressInfo  pdnConnectionAddressInfo;

#if defined (FEA_QOS_TFT)
  /** Requested QoS present */
  Boolean                   reqQosPresent;
  /** Requested QoS */
  QualityOfService          requiredQos;

  /** Negotiated QoS present */
  Boolean                   negQosPresent;
  /** Negotiated QoS */
  QualityOfService          negotiatedQos;

  /** Requested Traffic Flow Template present */
  Boolean                   reqTftPresent;
  /** Requested Traffic Flow Template */
  TrafficFlowTemplate       requiredTft;

  /** Negotiated Traffic Flow Template present */
  Boolean                   negTftPresent;
  /** Negotiated Traffic Flow Template */
  TrafficFlowTemplate       negotiatedTft;
#endif /* FEA_QOS_TFT */

  /** Maximum bit rates present for APN */
  Boolean                   apnAmbrPresent;
  /** Maximum bit rates for APN */
  ApnAmbr                   apnAmbr;

  /** Header Compression setting */
  HeaderCompType            headerComp;
  /** Data Compression setting */
  DataCompType              dataComp;

#if defined (FEA_DEDICATED_BEARER)      
  /** Whether this context is a secondary context (2G/3G) / dedicated bearer for LTE */
  Boolean                   secondaryContext;
  /** Associated primary PDP context / default EPS bearer connection ID if this is a
   * secondary PDP context / dedicated EPS bearer */
  Int8                      primaryConnId;
  /** Assocated primary PDP context / default EPS bearer CID */
  Int8                      primaryCid;
  /** Assocated primary PDP context / default EPS bearer PSD bearer ID */
  Int8                      primaryPsdBearerId;
#endif /* FEA_DEDICATED_BEARER */

  /** Specifies the flow control mechanism */
  FlowCtrlType              flowControlType;

  MtuSizeRequested          ipv4LinkMTURequest;
  MtuSizeRequested          nonIPLinkMTURequest;

  Boolean                   ipv4MtuSizePresent;
  Int16                     ipv4LinkMTU;     /* IPV4 MTU size */
  Boolean                   nonIPMtuSizePresent;
  Int16                     nonIPLinkMTU;    /* Non-IP MTU size */

#if defined(FEA_TEMP_REL14_ABPD_DUAL_PRIORITY)
  VgNasSigPrioType          nasSigPriority;
#endif
} VgPsdBearerInfo;



/** Indicates the reason why ATCI or other upper layer task is terminating the
 * PSD connection
 */
typedef enum VgHangupTypeTag
{
    VG_HANGUP_TYPE_ATH,                      /**< ATH command received from PC */
    VG_HANGUP_TYPE_DTR_DROPPED               /**< DTR handshake control line from the PC is dropped */
} VgHangupType;

typedef struct VgPlmnUplinkRateControlInfoTag
{
  /** Serving PLMN Rate Control Value Present */
  Boolean                   plmnRateControlPresent;
  /** Serving PLMN Rate Control Value (3GPP TS 24.301, 9.9.4.28) */
  /* \assoc PRESENT \ref plmnRateControlValue */
  Int16                     plmnRateControlValue;  
}VgPlmnUplinkRateControlInfo;

typedef struct VgApnUplinkRateControlInfoTag
{
  /** APN Rate Control Parameters */
  Boolean                   apnRateControlPresent;
  /** Additional exception reports at maximum rate reached allowed or not */
  Boolean                   apnRateControlAdditionalExceptionReportsAllowed;
  /** Uplink time unit */
  Int8                      apnRateControluplinkTimeUnit;
  /** Maximum uplink rate */
  Int32                     apnRateControlMaxUplinkRate; 

}VgApnUplinkRateControlInfo;

typedef struct VgPsdStatusInfoTag
{
  Boolean           profileDefined;
  Boolean           cgdcontDefined;
  Boolean           cgqreqDefined;
  Boolean           cgqminDefined;
  Boolean           isActive;
  Int8              cid;

#if defined (FEA_MT_PDN_ACT)
  Boolean           mtActivatedDedicatedBearer;
#endif

#if defined (FEA_QOS_TFT)
  Boolean           cgeqosDefined;
#endif /* FEA_QOS_TFT */

  VgPsdBearerInfo   psdBearerInfo;

#if defined (FEA_QOS_TFT)
  Boolean           modifyPending;                /* This is used to indicate if
                                                   * someone has changed the QoS/TFT
                                                   * attributes of a context whilst it
                                                   * is active - it is not the same
                                                   * as pendingContextModification below
                                                   */
  Boolean           pendingContextModification;   /* This indicates if a request for a
                                                   * context modification is pending a
                                                   * response from ABPD.
                                                   */
  VgmuxChannelNumber  vgModifyChannel;            /* This indicates channel number for a
                                                   * context modification is pending a
                                                   * response from ABPD.
                                                   */
#endif /* FEA_QOS_TFT */

  Boolean           pendingContextActivation;     /* This indicates a request for
                                                   * a context activation is pending a
                                                   * response from ABPD.
                                                   */
  Boolean           pendingDataConnectionActivation;
                                                  /* This indicates a request for
                                                   * a data connection for an already
                                                   * activated context pending a
                                                   * response from ABPD.
                                                   * This is when a context has already
                                                   * been activated with no data connection
                                                   * using AT+CGACT,
                                                   * and we then try to connect a data
                                                   * connection using ATD*99# or
                                                   * AT+CGDATA using the same CID already
                                                   * activated.
                                                   */
  Boolean           pendingContextDeactivation;    /* This indicates a request for
                                                   * a context deactivation is pending a
                                                   * response from ABPD.
                                                   */
  Boolean           pendingUnsolicitedContextActivation;
                                                  /* This indicates a request for
                                                   * an MT context activation is pending a
                                                   * response from ABPD. (2G/3G only)
                                                   */
#if defined (FEA_DEDICATED_BEARER)                                                   
  Int8              secondaryContextCidPendingActivation;
                                                  /* This is the CID of the secondary
                                                   * context which was pending this
                                                   * primary CID (if it is a primary CID)
                                                   * activation before we could activate
                                                   * this secondary CID.
                                                   * Set to CID_NUMBER_UNKNOWN if there
                                                   * is none.
                                                   */
#endif /* FEA_DEDICATED_BEARER */                                                   

  ReportType        countReportType;
  Int16             countReportPeriod;
  Boolean           cgdscontDefined;
  Boolean           cgeqreqDefined;
  Boolean           cgeqminDefined;
  Boolean           cgtftDefined;
  
  VgApnUplinkRateControlInfo  apnUplinkRateControlInfo;
} VgPsdStatusInfo;

/* PSD error reporting prompt reference */
typedef enum VgEREPPromptRefTag
{
  EREP_PROMPT_NW_DETACH = 0,
  EREP_PROMPT_ME_DETACH,
  EREP_PROMPT_NW_CLASS,
  EREP_PROMPT_ME_CLASS,
  EREP_PROMPT_NW_PRIM_B_ACT,
  EREP_PROMPT_ME_PRIM_B_ACT,
  EREP_PROMPT_NW_SEC_B_ACT,
  EREP_PROMPT_ME_SEC_B_ACT,
  EREP_PROMPT_NW_PRIM_B_DEACT,
  EREP_PROMPT_ME_PRIM_B_DEACT,
  EREP_PROMPT_NW_SEC_B_DEACT,
  EREP_PROMPT_ME_SEC_B_DEACT,
  EREP_PROMPT_NW_MODIFY,
  EREP_PROMPT_ME_MODIFY,
  EREP_PROMPT_REJECT,
  EREP_PROMPT_REACT
} VgEREPPromptRef;

typedef enum VgEREPEventTypeTag
{
  EREP_EVENT_INFORMATIONAL = 0,
  EREP_EVENT_ACKNOWLEDGEMENT
} VgEREPEventType;

typedef enum VgEREPMePrimBActReasonTag
{
  EREP_REASON_IPV4_ONLY_ALLOWED = 0,
  EREP_REASON_IPV6_ONLY_ALLOWED,
  EREP_REASON_SINGLE_ADDR_BEARERS_ONLY_ALLOWED,
  EREP_REASON_SINGLE_ADDR_BEARERS_ONLY_ALLOWED_AND_UE_INIT_SEC_ADDR_TYPE_BEARER_FAILED,
  NUM_OF_EREP_PRIM_B_ACT_REASON
} VgEREPMePrimBActReason;

typedef enum VgEREPModifyReasonTag
{
  EREP_REASON_TFT_CHANGED = 1,
  EREP_REASON_QOS_CHANGED,
  EREP_REASON_TFT_AND_QOS_CHANGED,
  NUM_OF_EREP_MODIFY_REASON
} VgEREPModifyReason;

/* CGREG definitions */
typedef enum VgCGREGModeTag
{
  VG_CGREG_DISABLED = 0,
  VG_CGREG_ENABLED,
  VG_CGREG_ENABLED_WITH_LOCATION_INFO,
  VG_CGREG_NUMBER_OF_SETTINGS
} VgCGREGMode;

typedef struct VgCGREGDataTag
{
  Boolean           state;     /* GPRS connection status */
  VgMnLocation      regStatus; /* Registration status - will be copied
                                * from the AbmmGprsRegStatus in to
                                * VgMnLocation type - they map one to one
                                * with additional values for LTE
                                */
  Lac               lac;       /* location area code     */
  Int32             cellId;    /* cell identifier        */
  AccessTechnologyId accessTechnology; /* access Technology */
  Rac               rac;       /* routing area code     */
} VgCGREGData;

/* CEREG definitions */
typedef enum VgCEREGModeTag
{
  VG_CEREG_DISABLED = 0,
  VG_CEREG_ENABLED,
  VG_CEREG_ENABLED_WITH_LOCATION_INFO,
  VG_CEREG_ENABLED_LOC_INFO_AND_CAUSE,
  VG_CEREG_ENABLED_INFO_FOR_PSM,
  VG_CEREG_ENABLED_PSM_INFO_AND_CAUSE,
  VG_CEREG_NUMBER_OF_SETTINGS
} VgCEREGMode;
typedef enum VgMRPMModeTag
{
  VG_MRPM_DISABLED = 0,
  VG_MRPM_ENABLED,
  VG_MRPM_NUMBER_OF_SETTINGS
} VgMRPMMode;
typedef struct VgCEREGDataTag
{
  Boolean           state;     /* GPRS connection status */
  VgMnLocation      regStatus; /* Registration status - will be copied
                                * from the AbmmGprsRegStatus in to
                                * VgMnLocation type - they map one to one
                                * with additional values for LTE
                                */
  Lac               tac;       /* tracking area code     */
  Int32             cellId;    /* cell identifier        */
  AccessTechnologyId accessTechnology; /* access Technology */
  Rac               rac;       /* routing  area code     */
  RejectCauseType   causeType;
  EmmCause          rejectCause;
  Boolean           psmInfoPresent;
  Int8              activeTime;
  Int8              periodicTau;
} VgCEREGData;

/* CGEREP definitions */

typedef enum VgCGEREPModeTag
{
  EREP_MODE_0, /* disable error reporting */
  EREP_MODE_1  /* enable error reporting  */
} VgCGEREPMode;

typedef enum VgCGEREPBfrTag
{
  EREP_BUFFER_CLEAR = 0,
  EREP_BUFFER_FLUSH     /* current not supported */
} VgCGEREPBfr;

typedef struct VgCGEREPPdpContextTag
{
  Int8              psdBearerId;
  PdnAddress        pdnAddress;
  Boolean           active;
  Int8              cid;
  ReliabilityClass  reliabilityClass;
} VgCGEREPPdpContext;

typedef struct VgMDPDNPDataTag
{
  TextualAccessPointName   defaultApnFromNw;
  PdnType                  defaultPdnTypeFromNw;
}
VgMDPDNPData;

typedef struct VgCSODCPDataTag
{
  Int8              connId;
  Int32             userDataLength;
  uint8_t*          userData;
}
VgCSODCPData;

typedef struct VgMGCOUNTDataTag
{
  VgmuxChannelNumber      vgReportEntity;      /* Record which channel we want to report/reset the
                                         * packet counters for.
                                         */
  Int8                    vgCounterCid;      /* Record which CID we want to report/reset the
                                         * packet counters for.
                                         */
}
VgMGCOUNTData;

#if defined (FEA_UPDIR)
/* MUPDIR definitions */
typedef enum VgMUPDIRModeTag
{
  VG_MUPDIR_DISABLED = 0,
  VG_MUPDIR_ENABLED,
  VG_MUPDIR_NUMBER_OF_SETTINGS
} VgMUPDIRMode;

typedef struct VgMUPDIRDataTag
{
  VgmuxChannelNumber  vgMupdiEntity;      /* Record which channel we want to report URC *MUPDI */
  PdnAddress          srcIpAddress;   /* Source IP address */
  PdnAddress          dstIpAddress;   /* Destination IP address */
  Boolean             srcPortPresent; /* If present souce port */
  Boolean             dstPortPresent; /* If destination souce port */
  Int16               srcPort;        /** Source port */
  Int16               dstPort;        /** Destination port */
  Int16               msgIdOffset;    /** Offset in byte for message Id. Start from 0 */
  Int8                msgIdSize;      /** Size in byte for message Id. Range: 1 or 2 */
  Int8                patternId;      /* Pattern ID to be deactivated */
}
VgMUPDIRData;
#endif

typedef struct GprsServiceStateTag
{
  Boolean          valid;
  Boolean          gprsAttached;

  /* TODO: For NB-IOT Store current band mode */
  ServiceType      abmmCurrentService;
} GprsServiceState;


typedef struct VgCGACLDataTag
{
  Boolean                setAcl;
  Int8                   startField;
  Int8                   endField;
  TextualAccessPointName apn;
  } VgCGACLData;

#if defined (FEA_MT_PDN_ACT)
typedef struct VgMMTPDPCIDDataTag
{
  Boolean enabled;
  Int8    cid;
} VgMMTPDPCIDData;
#endif /* FEA_MT_PDN_ACT */

/* MGSINK and MGTCSINK definitions */
typedef struct VgMGSINKDataTag
{
  Int8                      psdBearerId;
  TextualPdnAddress         pdnAddress;
  Boolean                   mgsinkActive;
  Boolean                   mgtcsinkActive;
  Int16                     packetSize;
  Int16                     packetCount;
  Int16                     port;
  Int16                     packetId;
  Int32                     sequenceNumber;
  Int16                     packet;      /* Remaining data */
  Int16                     segment;     /* Size of current segment */
  struct VgMGSINKDataTag    *next;       /* Link to next MGSINK/MGTCSINK request (if any) */
} VgMGSINKData;



typedef struct VgReqMNbiotDtDataTag
{
  AbpdApnDataTypeEntity     apnDataTypeEntity [MAX_NUM_CONN_IDS];
}VgReqMNbiotDtData;

/* MDPDNP definitions */
typedef enum VgMDPDNPModeTag
{
  VG_MDPDNP_DISABLED = 0,
  VG_MDPDNP_ENABLED,
  VG_MDPDNP_NUMBER_OF_SETTINGS
} VgMDPDNPMode;

/***********************************************/
/* Data structures for static memory structure */
/* Which are dynamically used by ATCI          */
/***********************************************/
typedef struct InUsePdnMemDataItemTag
{
  Boolean           inUse;
  VgPsdStatusInfo   vgPsdStatusInfo;
}InUsePdnMemDataItem_t;
  
/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

#endif  /* RVGPDATA_H */

/* END OF FILE */


