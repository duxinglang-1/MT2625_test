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
 * Types for EPS layer 3 messages
 *  **************************************************************************/

#ifndef L3EPS_TYP_H
#define L3EPS_TYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if defined (UPGRADE_LTE)
#include <l3gp_typ.h>
#include <pdn_typ.h>

/****************************************************************************
 * Macros
 ****************************************************************************/

/**************************************************************************
 * Manifest Constants
 **************************************************************************/

/** This is the maximum number of TAIs that are considered valid from the IE */
#define TAI_LIST_MAX_SIZE           16
/** RAW TAI List IE size (excluding IEI and length) */
#define RAW_TAI_LIST_IE_SIZE        96

/** 24.301 9.9.3.21
 * No NAS key is available
 */
#define NAS_KSI_NO_KEY_AVAILABLE    0x07

/** 24.301 9.9.3.22
 * Maximum length of NAS message container data
 */
#define MAX_NAS_MESSAGE_CONTAINER_SIZE  251

#define MAX_NUM_EPS_BEARER_IDS       16

/** 24.301 9.9.4.22
 * Maximum length of Header Compress container data
 */

#define HEADER_COMPRESS_CONTEXT_MAX_SIZE   251

#ifdef FEA_TEMP_REL14_HASH
/** 24.301 9.9.3.50
 * Hash MME
 */
#define SMC_HASH_MME_SIZE 8
#define MAX_REPLAYED_MESSAGE_CONTAINER_SIZE 400
#endif /*FEA_TEMP_REL14_HASH */

/* This could be 65535, but this needs to be cut for ERRC interface to be smaller than 1600.
     6 bytes are needed for security header, 1 for PD, 1 for message type,
     1 for Generic Message container type, 2 for
     Generic Message Conatiner data*/
#define MAX_GENERIC_NAS_MESSAGE_SIZE 1589

/****************************************************************************
 * Types
 ****************************************************************************/

/*****************************************************************
 * Types used in LTE (EPS) EMM messages
 *****************************************************************/

typedef enum EpsEdrxTypeTag
{
    USER_SETTING_EDRX_OFF           = 0,
    USER_SETTING_EDRX_ON            = 1,
    USER_EDRX_VALUE_NOT_SET         = 2
}
EpsEdrxType;


typedef enum EpsSecurityHeaderTypeTag
{
    /** plain NAS message */
    NOT_SECURITY_PROTECTED           = 0,
    /** integrity protected message                   */
    INTEGRITY_PROTECTED              = 1,
    /** integrity protected and ciphered message      */
    INTEGRITY_PROTECTED_AND_CIPHERED = 2,
    /** integrity protected and a new EPS context
        only used for SECURITY MODE COMMAND message   */
    INTEGRITY_AND_NEW_EPS_CONTEXT    = 3,
    /** integrity protected and ciphered message with
        a new EPS context        - only used for
        SECURITY MODE COMPLETE message                */
    INTEGRITY_CIPHER_AND_NEW_CONTEXT = 4,
    /** integrity protected and partially ciphered NAS message.
       Used only for a CONTROL PLANE SERVICE_REQUEST message*/
    INTEGRITY_PROTECTED_AND_PARTIALLY_CIPHERED = 5,
    /** security header for SERVICE REQUEST message   */
    SECURITY_HEADER_SERVICE_REQUEST  = 0xc
}
EpsSecurityHeaderType;


/** 24.301 9.9.3.0A
 * Additional update result
 * Provides additional information about the result of a combined attach
 * procedure or a combined tracking area updating procedure
 */
typedef enum AdditionalUpdateResultTag
{
    /** No additional information */
    AUR_NO_ADDITIONAL_INFO      = 0,
    /** CS Fallback not preferred */
    AUR_CS_FB_NOT_PREF          = 1,
    /** SMS only */
    AUR_SMS_ONLY                = 2,
    /** Reserved */
    AUR_RESERVED                = 3
}
AdditionalUpdateResult,
/** Additional update result IE */
AdditionalUpdateResultElement;

/** 24.301 9.9.3.0B
 * Additional update type
 * Provides additional information about the type of request for
 * a combined attach or a combined tracking area updating procedure.
 */
typedef enum AdditionalUpdateTypeValueTag
{
    /** No additional information */
    AUT_NO_ADDITIONAL_INFO      = 0,
    /** SMS only */
    AUT_SMS_ONLY                = 1
}
AdditionalUpdateTypeValue;

typedef enum PreferredNwCiotOpt
{
    /** No additional information */
    PCIOT_NO_ADDITIONAL_INFO      = 0,
    /** CP optimization preferred */
    PCIOT_CP_OPT                  = 1,
    /** UP optimization preferred */
    PCIOT_UP_OPT                  = 2
}
PreferredNwCiotOpt;


/** Additional update type IE */
typedef struct AdditionalUpdateTypeElementTag
{
    /** additional update type */
    AdditionalUpdateTypeValue additionalUpdateTypeValue;
    /** "signalling active" flag(SAF) */
    Boolean saf;
    /** Preferred CIoT network behaviour */
    PreferredNwCiotOpt preferredNwCiotOpt;
}
AdditionalUpdateTypeElement;


/** 24.301 9.9.3.4B
 * SMS services status
 * Indicates the status of avaibility for SMS services requested by
 * the UE during normal attach or tracking area updating.
 */
typedef enum SMSServicesStatusTag
{
    /** SMS services not available */
    SMS_SERVICES_NOT_AVAIL         = 0,
    /** SMS services not available in this PLMN */
    SMS_SERVICES_NOT_AVAIL_IN_PLMN = 1,
    /** Network failure */
    SMS_NETWORK_FAILURE            = 2,
    /** Congestion */
    SMS_CONGESTION                 = 3
}
SMSServicesStatus,
/* SMS services status IE */
SMSServicesStatusElement;

/** 24.301 9.9.3.4
 * Authentication response parameter
 * Provides the network with the authentication response calculated
 * in the USIM.
 */
typedef Res
AuthParamResElement;

/** 24.301 9.9.3.7
 * Type of detach in the UE to network direction.
 * All other values are interpreted as combined EPS/IMSI detach.
 */
typedef enum EpsMoTypeOfDetachTag
{
    /** EPS detach */
    MO_DET_EPS_DETACH                   = 1,
    /** IMSI detach */
    MO_DET_IMSI_DETACH                  = 2,
    /** Combined EPS/IMSI detach */
    MO_DET_COMBINED_EPS_IMSI_DETACH     = 3,
    /** Reserved */
    MO_DET_RESERVED1                    = 6,
    /** Reserved */
    MO_DET_RESERVED2                    = 7
}
EpsMoTypeOfDetach;

/** 24.301 9.9.3.7
 * Detach type IE for network to UE detach
 */
typedef struct EpsMoDetachTypeElementTag
{
    /** Type of detach */
    EpsMoTypeOfDetach       typeOfDetach;
    /** Switch off flag */
    Boolean                 switchOff;
}
EpsMoDetachTypeElement,
EpsMoDetachInfo;

/** 24.301 9.9.3.7
 * Type of detach in the network to UE direction.
 * All other values are interpreted as re-attach not required.
 */
typedef enum EpsMtDetachTypeElementTag
{
    /** Re-attach required */
    MT_DET_REATTACH_REQ                 = 1,
    /** Re-attach not required */
    MT_DET_REATTACH_NOT_REQ             = 2,
    /** IMSI detach */
    MT_DET_IMSI_DETACH                  = 3,
    /** Reserved */
    MT_DET_RESERVED1                    = 6,
    /** Reserved */
    MT_DET_RESERVED2                    = 7
}
/** Detach type IE for network to UE detach */
EpsMtDetachTypeElement;




/** 24.301 9.9.3.10
 * EPS attach result
 */
typedef enum EpsAttachResultTag
{
    /** EPS only */
    EPS_ATT_RES_EPS_ONLY_ATTACH = 1,
    /** Combined EPS/IMSI attach */
    EPS_ATT_RES_COMBINED_EPS_IMSI_ATTACH = 2
}
EpsAttachResult,
/** EPS attach result IE */
EpsAttachResultElement;

/** 24.301 9.9.3.11
 * EPS attach type
 */
typedef enum EpsAttachTypeTag
{
    /** EPS attach */
    EPS_ATT_TYPE_EPS_ATTACH                 = 1,
    /** Combined EPS/IMSI attach */
    EPS_ATT_TYPE_COMBINED_EPS_IMSI_ATTACH   = 2,
    /** Reserved */
    EPS_ATT_TYPE_RESERVED1                  = 6,
    /** Reserved */
    EPS_ATT_TYPE_RESERVED2                  = 7
}
EpsAttachType,
/** EPS attach type IE */
EpsAttachTypeElement;

/** 24.301 9.9.3.12A
 * EPS network feature support
 * Indicates whether certain features are supported by the network
 */
typedef struct EpsNetworkFeatureSupportTag
{
    /* Feature support IE, octet 3 support bits present */
    Boolean                 epsFeatureSupportOctet3Present;
    /** C-plane CIoT support  */
    Boolean                 cpciotSupport;
    /** EMM-REGISTERED wo PDN connection(Attach wo PDN) support  */
    Boolean                 erwopdnSupport;
    /* Feature support IE, octet 4 support bits present */
    Boolean                 epsFeatureSupportOctet4Present;
    /** U-plane CIoT support  */
    Boolean                 upciotSupport;
    /** S1-U data support  */
    Boolean                 s1udataSupport;
    /** ROHC support  */
    Boolean                 hcciotSupport;
    /** ePCO support  */
    Boolean                 epcoSupport;
    /** Restriction on enhanced coverage (RestrictEC) support  */
    Boolean                 restricEcSupport;
}
EpsNetworkFeatureSupport,
/** Network feature support IE */
EpsNetworkFeatureSupportElement;

/** 24.301 9.9.3.26A Extended EMM cause
*/
typedef enum EutranAllowedInfoTag
{
    EXT_EUTRAN_ALLOWED      = 0,
    EXT_EUTRAN_NOT_ALLOWED  = 1
}
EutranAllowedInfo;

typedef enum EpsOptimizationInfoTag
{
    NO_EPS_OPTIMIZATION_INFORMATION           = 0,
    REQUESTED_EPS_OPTIMIZATION_NOT_SUPPORTED  = 1
}
EpsOptimizationInfo;

typedef struct ExtendedEmmCauseElementTag
{
    EutranAllowedInfo       eutraAllowed;
    EpsOptimizationInfo     epsOptInfo;
}
ExtendedEmmCauseElement;

/** 24.301 9.9.3.13 EpsUpdateResult
*/
typedef enum EpsUpdateResultTag
{
    TA_UPDATED                                = 0,
    COMBINED_TA_LA_UPDATED                    = 1,
    TA_UPDATED_AND_ISR_ACTIVATED              = 4,
    COMBINED_TA_LA_UPDATED_ISR_ACTIVATED      = 5
}
EpsUpdateResult;

/** 24.301 9.9.3.14
 * EPS update type value
*/
typedef enum EpsUpdateTypeTag
{
    TA_UPDATING                             = 0,
    COMBINED_TA_LA_UPDATING                 = 1,
    COMBINED_TA_LA_UPDATING_AND_IMSI_ATTACH = 2,
    PERIODIC_TA_UPDATING                    = 3,
    UNUSED_TA_1                             = 4,
    UNUSED_TA_2                             = 5
}
EpsUpdateType;

/** 24.301 table 9.9.3.14.1
 * EPS update type element
*/
typedef struct EpsUpdateTypeElementTag
{
    EpsUpdateType           epsUpdateType;
    Boolean                 active;
}
EpsUpdateTypeElement;

/** 24.301 9.9.3.19
 * KSI and sequence number
 */
typedef struct KsiAndSeqNumberElementTag
{
    /** Sequence number (short) */
    Int8    seqNumber;
    /** KSI */
    Int8    ksi;
}
KsiAndSeqNumberElement;

/** 24.301 9.9.3.21
 * Type of security context (TSC)
 */
typedef enum TypeOfSecurityContextTag
{
    /** Native security context (for KSI-ASME) */
    TSC_NATIVE_SECURITY_CTXT    = 0,
    /** Mapped security context (for KSI-SGSN) */
    TSC_MAPPED_SECURITY_CTXT    = 1
}
TypeOfSecurityContext;

/** 24.301 9.9.3.21
 * NAS key set identifier
 */
typedef struct NasKeySetIdentifierTag
{
    /** Type of security context */
    TypeOfSecurityContext   tsc;
    /** NAS key set identifier  */
    Int8                    nasKsi;
}
NasKeySetIdentifierElement;

/** 24.301 9.9.3.22
 * NAS message container
 */
typedef struct NasMessageContainerElementTag
{
    /** NAS message container length */
    Int16       length;
    /** NAS message container data */
    Int8        data[MAX_NAS_MESSAGE_CONTAINER_SIZE];
}
NasMessageContainerElement;

/** 24.301 9.9.3.23
 * NAS security algorithms
 */
typedef struct EpsNasSecurityAlgorithmsElementTag
{
    /** Type of integrity protection algorithm */
    EpsIntegAlgorithm       epsIntegAlgorithm;
    /** Type of ciphering algorithm  */
    EpsEncryptAlgorithm     epsEncryptAlgorithm;
}
EpsNasSecurityAlgorithmsElement;

/** 24.301 9.9.3.43
 * NAS message container
 */
typedef struct GenericNasMessageContainerTag
{
    /** NAS message container length */
    Int16       length;
    /** NAS message container data */
    Int8        data[MAX_GENERIC_NAS_MESSAGE_SIZE];
}
GenericNasMessageContainer;

/** 24.301 9.9.3.25
 * Nonce
 */
typedef Int32 Nonce, NonceElement;

#ifdef FEA_TEMP_REL14_HASH
/** 24.301 9.9.3.50
 * Hash MME
 */
typedef struct HashElementTag
{
    /** NAS message container data */
    Int8        data[SMC_HASH_MME_SIZE];
}
HashElement;

typedef struct ReplayedNasMsgElementTag
{
    /** NAS message container length */
    Int16       length;
    /** NAS message container data */
    Int8        data[MAX_REPLAYED_MESSAGE_CONTAINER_SIZE]; /* This is Attach request or TAU request, currently
                                                              TAU max size is 324, attach is smaller. */
}ReplayedNasMsgElement;
#endif /* FEA_TEMP_REL14_HASH */

/** Indicates if the decoding of the TAI list IE has been successful
 * or too many TAIs have been found (max=16) in the IE. In the
 * overflow case, the remaining octets of the IE are ignored.
 * NOTE: This is an internal type used only within the UE.
 */
typedef enum EpsTaiListDecodeResultTag
{
    /** TAI list IE decoding successful */
    EPS_TAI_LIST_DEC_SUCCESS,
    /** Too many TAIs in the TAI list IE */
    EPS_TAI_LIST_DEC_OVERFLOW
}
EpsTaiListDecodeResult;

/** Structure for holding a list of Tracking Area Identities
 */
typedef struct TaiListTag
{
    /** How many valid TAIs are in the array
\assoc ARRAY \ref tai */
    Int8       length;
    /* List of TAIs */
    Lai        tai[TAI_LIST_MAX_SIZE];
}
TaiList;

/** 24.301 9.9.3.33
 * Tracking area identity list
 * Used to transfer a list of tracking areas from the network to the UE
 */
typedef struct TaiListElementTag
{
    /** Indicates if the decoding has been successful or if too many TAIs
     * have been found in IE
     */
    EpsTaiListDecodeResult  taiListDecodeResult;
    /** The decoded TAI list (may have been truncated if maximum number of TAIs
     * is exceeded) */
    TaiList                 taiListArray;
    /** Raw IE */
    Int8                    rawIe[RAW_TAI_LIST_IE_SIZE];
}
TaiListElement;

/** 24.301 9.9.3.33
 * Type of list (TAI list)
 */
typedef enum TaiListTypeTag
{
    /** List of TACs belonging to one PLMN, with non-consecutive TAC values */
    TAI_LIST_TYPE_000   = 0,
    /** List of TACs belonging to one PLMN, with consecutive TAC values */
    TAI_LIST_TYPE_001   = 1,
    /** List of TAIs belonging to different PLMNs */
    TAI_LIST_TYPE_010   = 2
}
TaiListType;

/** 24.301 9.9.3.34
 * EPS encryption algorithms supported
 */
typedef struct EpsEncCapabilityTag
{
    /** EEA0 support */
    Boolean                 EpsEncSupport_EEA0;
    /** 128-EEA1 support */
    Boolean                 EpsEncSupport_128EEA1;
    /** 128-EEA2 support */
    Boolean                 EpsEncSupport_128EEA2;
    /** EEA3 support */
    Boolean                 EpsEncSupport_128EEA3;
    /** EEA4 support */
    Boolean                 EpsEncSupport_EEA4;
    /** EEA5 support */
    Boolean                 EpsEncSupport_EEA5;
    /** EEA6 support */
    Boolean                 EpsEncSupport_EEA6;
    /** EEA7 support */
    Boolean                 EpsEncSupport_EEA7;
}
EpsEncCapability;

/** 24.301 9.9.3.34
 * EPS integrity algorithms supported
 */
typedef struct EpsIntCapabilityTag
{
    /** 128-EIA1 support */
    Boolean                 EpsIntSupport_128EIA1;
    /** 128-EIA2 support */
    Boolean                 EpsIntSupport_128EIA2;
    /** EIA3 support */
    Boolean                 EpsIntSupport_128EIA3;
    /** EIA4 support */
    Boolean                 EpsIntSupport_EIA4;
    /** EIA5 support */
    Boolean                 EpsIntSupport_EIA5;
    /** EIA6 support */
    Boolean                 EpsIntSupport_EIA6;
    /** EIA7 support */
    Boolean                 EpsIntSupport_EIA7;
}
EpsIntCapability;

/** 24.301 9.9.3.34
 * UMTS encryption algorithms supported
 */
typedef struct UmtsEncCapabilityTag
{
    /** UEA0 support */
    Boolean                 UmtsEncSupport_UEA0;
    /** UEA1 support */
    Boolean                 UmtsEncSupport_UEA1;
    /** UEA2 support */
    Boolean                 UmtsEncSupport_UEA2;
    /** UEA3 support */
    Boolean                 UmtsEncSupport_UEA3;
    /** UEA4 support */
    Boolean                 UmtsEncSupport_UEA4;
    /** UEA5 support */
    Boolean                 UmtsEncSupport_UEA5;
    /** UEA6 support */
    Boolean                 UmtsEncSupport_UEA6;
    /** UEA7 support */
    Boolean                 UmtsEncSupport_UEA7;
}
UmtsEncCapability;

/** 24.301 9.9.3.34
 * UMTS integrity algorithms supported
 */
typedef struct UmtsIntCapabilityTag
{
    /** UIA1 support */
    Boolean                 UmtsIntSupport_UIA1;
    /** UIA2 support */
    Boolean                 UmtsIntSupport_UIA2;
    /** UIA3 support */
    Boolean                 UmtsIntSupport_UIA3;
    /** UIA4 support */
    Boolean                 UmtsIntSupport_UIA4;
    /** UIA5 support */
    Boolean                 UmtsIntSupport_UIA5;
    /** UIA6 support */
    Boolean                 UmtsIntSupport_UIA6;
    /** UIA7 support */
    Boolean                 UmtsIntSupport_UIA7;
}
UmtsIntCapability;

/** 24.301 9.9.3.34
 * UCS2 support
 */
typedef enum Ucs2SupportTag
{
    /** The UE has a preference for the default alphabet
     * (defined in 3GPP TS 23.038) over UCS2 (see ISO/IEC 10646)
     */
    UCS2_SUPP_DEF_ALPHABET_PREFERRED    = 0,
    /** The UE has no preference between the use of the default alphabet
     * and the use of UCS2
     */
    UCS2_SUPP_NO_PREFERENCE             = 1
}
Ucs2Support;

/** EPS specifics for the UE network capability IE that are stored
 *  in NVRAM and passed to EMM in the MeData - UMTS specifics needed
 *  for the EpsUeNetworkCapabilityElement are supplied through
 *  UmtsMobileEquipmentData-radioAccessCapability-SecurityCapability
 */
typedef struct EpsUeNetworkCapabilityTag
{
    /** EPS encryption algorithms supported */
    EpsEncCapability        epsEncCapability;
    /** EPS integrity algorithms supported */
    EpsIntCapability        epsIntCapability;
}
EpsUeNetworkCapability;

/** 24.301 9.9.3.34
 * UE network capability
 * Used to provide the network with information concerning aspects of the UE
 * related to EPS or interworking with GPRS.
 */
typedef struct EpsUeNetworkCapabilityElementTag
{
    /** EPS encryption algorithms supported */
    EpsEncCapability        epsEncCapability;
    /** EPS integrity algorithms supported */
    EpsIntCapability        epsIntCapability;
    /** UMTS encryption algorithms supported */
    UmtsEncCapability       umtsEncCapability;
    /** UMTS integrity algorithms supported */
    UmtsIntCapability       umtsIntCapability;
    /** UCS2 support */
    Ucs2Support             ucs2Support;
    /** 1xSRVCC capability  */
    Boolean                 srvccSupport;
    /** c-plane CIoT support  */
    Boolean                 cpciotSupport;
    /** u-plane CIoT support  */
    Boolean                 upciotSupport;
    /** S1-U data support  */
    Boolean                 s1udataSupport;
    /** EMM-REGISTERED wo PDN connection(Attach wo PDN) support  */
    Boolean                 erwopdnSupport;
    /** ROHC support  */
    Boolean                 hcciotSupport;
    /** ePCO support  */
    Boolean                 epcoSupport;
    /** multiple DRB support */
    Boolean                 mDrbSupport;
#ifdef FEA_TEMP_REL14_EMM_REST_EC
    /** Restriction on use of enhanced coverage support */
    Boolean                 restEcSupport;
#endif /* FEA_TEMP_REL14_EMM_REST_EC */
#ifdef FEA_TEMP_REL14_EMM_CP_CONGESTION_CONTROL
    /** CP backoff */
    Boolean                 cpBackoff;
#endif /* FEA_TEMP_REL14_EMM_CP_CONGESTION_CONTROL */
#ifdef FEA_LPP_SUPPORT
    Boolean                 lcsSupport;
    Boolean                 lppSupport;
#endif /* FEA_LPP_SUPPORT */
}
EpsUeNetworkCapabilityElement;

/** 24.301 9.9.3.36
 * GPRS encryption algorithms supported
 */
typedef struct GprsEncCapabilityTag
{
    /** GEA1 support */
    Boolean                 GprsEncSupport_GEA1;
    /** GEA2 support */
    Boolean                 GprsEncSupport_GEA2;
    /** GEA3 support */
    Boolean                 GprsEncSupport_GEA3;
    /** GEA4 support */
    Boolean                 GprsEncSupport_GEA4;
    /** GEA5 support */
    Boolean                 GprsEncSupport_GEA5;
    /** GEA6 support */
    Boolean                 GprsEncSupport_GEA6;
    /** GEA7 support */
    Boolean                 GprsEncSupport_GEA7;
}
GprsEncCapability;

/** 24.301 9.9.3.36
 * UE security capability
 * Used by the network to indicate which security algorithms are supported
 * by the UE in S1 mode, Iu mode and Gb mode.
 */
typedef struct EpsUeSecurityCapabilityElementTag
{
    /** EPS encryption algorithms supported */
    EpsEncCapability        epsEncCapability;
    /** EPS integrity algorithms supported */
    EpsIntCapability        epsIntCapability;
    /** Indicates if UMTS security fields umtsEncCapability and umtsIntCapability
     * are present
     */
    Boolean                 umtsSecCapabilityPresent;
    /** UMTS encryption algorithms supported */
    UmtsEncCapability       umtsEncCapability;
    /** UMTS integrity algorithms supported */
    UmtsIntCapability       umtsIntCapability;
    /** Indicates if GPRS security fields are present */
    Boolean                 gprsSecCapabilityPresent;
    /** GPRS encryption algorithms supported */
    GprsEncCapability       gprsEncCapability;
}
EpsUeSecurityCapabilityElement;

/** CS/PS Mode of operation of the UE - required for LTE multimode.
 * The mode specifies if the UE is data-centric or voice centric and whether
 * it supports voice calls or not.
 * The enum values are as per those specified in 3GPP 27.007.
 */
typedef enum CsPsModeTag
{
    /** UE registers only to EPS services and is data-centric */
    PS_MODE_2          = 0,
    /** UE registers to both EPS and non-EPS services and is voice-centric */
    CS_PS_MODE_1       = 1,
    /** UE registers to both EPS and non-EPS services and is data-centric */
    CS_PS_MODE_2       = 2,
    /** UE registers only to EPS services and is voice-centric */
    PS_MODE_1          = 3,
    /** Invalid CS/PS mode */
    CS_PS_MODE_INVALID = 4
} CsPsMode;

/** 24.301 9.9.3.47
 * "Active" flag of CP service type IE
 */
typedef enum ActiveFlagOfCpServiceTypeTag
{
    /** No radio bearer establishment requested */
    NO_RADIO_BEARER_EST_REQ    = 0,
    /** Radio bearer establishment requested */
    RADIO_BEARER_EST_REQ       = 1,
} ActiveFlagOfCpServiceType;

/** 24.301 9.9.3.47
 * Control plane service type value
 */
typedef enum EpsCpServiceTypeValueTag
{
    /** MO request */
    MOBILE_ORIGINATING_REQUEST              = 0,
    /** MT request */
    MOBILE_TERMINATING_REQUEST              = 1,
} EpsCpServiceTypeValue;

/** 24.301 9.9.3.47
 * Control plane service type.
 * The purpose of the Control plane service type information element is to specify
 * the purpose of the CONTROL PLANE SERVICE REQUEST message.
 */

typedef struct EpsCpServiceTypeElementTag
{
    /** "Active" flag */
    ActiveFlagOfCpServiceType   activeflag;
    /** CP Service type value  */
    EpsCpServiceTypeValue       serviceTypeValue;
}
EpsCpServiceTypeElement;

/*****************************************************************
 * LTE EMM messages
 *****************************************************************/
/** 24.301 8.2.1
 * Attach accept
 * This message is sent by the network to the UE to indicate that
 * the corresponding attach request has been accepted.
 * ESM message container is mandatory in this message.
 */
typedef struct EpsAttachAcceptTag
{
    /** EPS attach result */
    EpsAttachResultElement          epsAttachResult;
    /** T3412 value */
    GprsTimerElement                t3412Timer;
    /** TAI list */
    TaiListElement                  taiList;
    /** Indicates if GUTI (EPS mobile id) is present
\assoc PRESENT \ref guti */
    Boolean                         gutiPresent;
    /** May be included to assign a GUTI to the UE during attach or combined EPS/IMSI attach */
    MobileIdElement                 guti;
#ifdef FEA_COMBINED_PROCEDURES
    /** Indicates if Location area identification is present
\assoc PRESENT \ref locationAreaId */
    Boolean                         locationAreaIdPresent;
    /** Location area identification */
    LocationAreaIdElement           locationAreaId;
    /** Indicates if MS identity is present.
\assoc PRESENT \ref msId */
    Boolean                         msIdPresent;
    /** May be included to assign or unassign a new TMSI to a UE during a combined attach */
    MobileIdElement                 msId;
    /** Indicates if EMM cause is present
\assoc PRESENT \ref emmCause */
    Boolean                         emmCausePresent;
    /** Shall be included when IMSI attach for non-EPS services is not successful during
     * a combined EPS/IMSI attach procedure */
    GsmCause                        emmCause;
#endif /* FEA_COMBINED_PROCEDURES */
    /** Indicates if T3402 value is present
\assoc PRESENT \ref t3402Timer */
    Boolean                         t3402TimerPresent;
    /** T3402 value, if not included, the UE shall use the default value */
    GprsTimerElement                t3402Timer;
    /** Indicates if T3423 value is present
\assoc PRESENT \ref t3423Timer */
    Boolean                         t3423TimerPresent;
    /** T3423 value, if not included, the UE shall use the default value */
    GprsTimerElement                t3423Timer;
    /** May be included in order to assign a new equivalent PLMNs list to a UE.
     * If not included numPlmn field is set to 0.
     */
    EquivalentPlmnListElement       equivalentPlmnList;
    /** Indicates if a list of emergency numbers is present.
\assoc PRESENT \ref emergencyNumberList */
    Boolean                         emergencyNumberListPresent;
    /** May be included to indicate a list of emergency numbers valid within
     * the same MCC as in the cell on which this IE is received.
     */
    EmergencyNumberList             emergencyNumberList;
    /** Indicates if EPS network feature support is present.
\assoc PRESENT \ref epsNetworkFeatureSupport */
    Boolean                         epsNetworkFeatureSupportPresent;
    /** May be included to inform the UE of the support of certain features.
     * If this is not included then the respective features are not supported.
     */
    EpsNetworkFeatureSupportElement epsNetworkFeatureSupport;
    /** Indicates if additional update result is present.
\assoc PRESENT \ref additionalUpdateResult */
    Boolean                         additionalUpdateResultPresent;
    /** May be included to provide the UE with additional information about
     * the result of a combined attach procedure if the procedure was successful
     * for EPS services and non-EPS services, or for EPS services and SMS-only.
     */
    AdditionalUpdateResultElement   additionalUpdateResult;
    /** Indicates if t3412 Extended Timer is present.
 \assoc PRESENT \ref t3412ExtendedTimer */
    Boolean                         t3412ExtendedTimerPresent;
    GprsTimer3Element               t3412ExtendedTimer;
    /** Indicates if t3324 is present.
 \assoc PRESENT \ref t3324Timer */
    Boolean                         t3324TimerPresent;
    GprsTimerElement                t3324Timer;
    /** Indicates if eDRX is present.
\assoc PRESENT \ref edrxElement */
    Boolean                         edrxPresent;
    /** The UE shall include this IE if UE is configured to use eDRX */
    EdrxElement                     edrxElement;
    /** Indicates if sms services status is present.
\assoc PRESENT \ref smsServicesStatus */
    Boolean                         smsServicesStatusPresent;
    /** May be included to provide the status of availability for SMS
     * services requested by the UE during Attach.
     */
    SMSServicesStatusElement        smsServicesStatus;

#ifdef FEA_TEMP_REL14_DCN
    /** Indicates if DCN ID IE is present
\assoc PRESENT \ref dcnId */
    Boolean                         dcnIdPresent;
    /* Dedicated core Network identification */
    Int16                           dcnId;
#endif /* FEA_TEMP_REL14_DCN */

#ifdef FEA_TEMP_REL14_EMM_CP_CONGESTION_CONTROL
    /** Indicates if T3448 value is present.
    \assoc PRESENT \ref t3448Timer */
    Boolean                         t3448TimerPresent;
    /** T3448 value */
    GprsTimerElement                t3448Timer;
#endif /* FEA_TEMP_REL14_EMM_CP_CONGESTION_CONTROL  */
}
EpsAttachAccept;

/** 24.301 8.2.2
 * Attach complete
 * Attach Complete message does not have any IEs except ESM Message Container,
 * which is mandatory.
 */
typedef struct EpsAttachCompleteTag
{
    /** This message does not have any additional IE */
    Int8                            empty;
}
EpsAttachComplete;

/** 24.301 8.2.3
 * Attach reject
 * This message is sent by the network to the UE to indicate that the corresponding
 * attach request has been rejected.
 */
typedef struct EpsAttachRejectTag
{
    /** Cause for reject */
    GsmCause                        emmCause;
    /** Indicates if ESM message container IE is present */
    Boolean                         esmMessageContainerPresent;
    /** Indicates if t3346 is present.
 \assoc PRESENT \ref t3346Timer */
    Boolean                         t3346TimerPresent;
    GprsTimerElement                t3346Timer;
    /** Indicates if t3402 is present.
 \assoc PRESENT \ref t3402Timer */
    Boolean                         t3402TimerPresent;
    GprsTimerElement                t3402Timer;
    /** Indicates if extendedEmmCause is present.
 \assoc PRESENT \ref extendedEmmCause */
    Boolean                         extendedEmmCausePresent;
    ExtendedEmmCauseElement         extendedEmmCause;

}
EpsAttachReject;

/** 24.301 8.2.4
 * Attach request
 * This message is sent by the UE to the network to request an attach
 */
typedef struct EpsAttachRequestTag
{
    /* Attach type */
    EpsAttachTypeElement            attachType;
    /** NAS key set identifier */
    NasKeySetIdentifierElement      nasKsi;
    /** Old GUTI or IMSI */
    MobileIdElement                 oldGutiOrImsi;
    /** UE network capability */
    EpsUeNetworkCapabilityElement   epsUeNetworkCapability;
    /** Indicates if last visited registered TAI is present.
\assoc PRESENT \ref lastVisitedRegTai */
    Boolean                         lastVisitedRegTaiPresent;
    /** Last visited registered TAI */
    LocationAreaIdElement           lastVisitedRegTai;
    /** Indicates if MS network capability is present.
\assoc PRESENT \ref msNetworkCapability */
    Boolean                         msNetworkCapabilityPresent;
    /** A UE supporting A/Gb mode or Iu mode shall include this IE
     * to indicate its capabilities to the network */
    MsNetworkCapabilityElement      msNetworkCapability;
    /** Indicates if old location area identification is present.
\assoc PRESENT \ref oldLocationAreaId */
    Boolean                         oldLocationAreaIdPresent;
    /** The UE shall include this IE during a combined attach procedure
     * if it has a valid location area identification
     */
    LocationAreaIdElement           oldLocationAreaId;
    /** Indicates if TMSI status is present.
\assoc PRESENT \ref tmsiStatus */
#ifdef FEA_COMBINED_PROCEDURES
    Boolean                         tmsiStatusPresent;
    /** The UE shall include this IE during combined attach procedure
     * if it has no valid TMSI available.
     */
    TmsiStatusElement               tmsiStatus;
    /** Indicates if Mobile station classmark 2 is present.
     * This IE shall be included if the UE supports SRVCC to GERAN or
     * UTRAN (see 3GPP TS 23.216 [8]), or if the UE is performing
     * a combined attach procedure.*/
    Boolean                         msClassMark2Present;
    /** If present Mobile station classmark 2 and 3 are stored here */
    MsClassmarkElement              msClassmark;
    Boolean                         tmsiBasedNriContainerPresent;
    TmsiBasedNriContainerElement    nriContainerValue;
#endif /* FEA_COMBINED_PROCEDURES */
    /** Indicates if additional update type is present.
\assoc PRESENT \ref additionalUpdateType */
    Boolean                         additionalUpdateTypePresent;
    /** The UE shall include this IE if the UE requests SMS-only */
    AdditionalUpdateTypeElement     additionalUpdateType;
    /** Indicates if device properties is present.
\assoc PRESENT \ref deviceProperties */
    Boolean                         devicePropertiesPresent;
    /** The UE shall include this IE if UE is configured for NAS signalling
    low priority */
    DevicePropertiesElement         deviceProperties;
    /** Indicates if old Guti type is present.
\assoc PRESENT \ref oldGutiType */
    Boolean                         oldGutiTypePresent;
    OldGutiTypeElement              oldGutiType;
    /** Indicates if msNetworkFeatureSupport is present.
\assoc PRESENT \ref msNetworkFeatureSupport */
    Boolean                         msNetworkFeatureSupportPresent;
    MsNetworkFeatureSupportElement  msNetworkFeatureSupport;
    /** Indicates if t3324 is present.
\assoc PRESENT \ref t3324Timer */
    Boolean                         t3324TimerPresent;
    GprsTimerElement                t3324Timer;
    /** Indicates if t3412 Extended Timer is present.
\assoc PRESENT \ref t3412ExtendedTimer */
    Boolean                         t3412ExtendedTimerPresent;
    GprsTimer3Element               t3412ExtendedTimer;
    /** Indicates if eDRX is present.
\assoc PRESENT \ref edrxValue */
    Boolean                         edrxPresent;
    /** The UE shall include this IE if UE uses eDRX */
    EdrxElement                     edrxValue;

}
EpsAttachRequest;

/** 24.301 8.2.5
 * Authentication failure
 * This message is sent by the UE to the network to indicate that
 * authentication of the network has failed.
 */
typedef struct EpsAuthenticationFailureTag
{
    /** Cause for failure */
    GsmCause                        emmCause;
    /** Whether or not authentication failure parameter is present.
\assoc PRESENT \ref authFailureParameter */
    Boolean                         authFailureParameterPresent;
    /** Authentication failure parameter */
    Aut                             authFailureParameter;
}
EpsAuthenticationFailure;

/** 24.301 8.2.6
 * Authentication reject
 * This message is sent by the network to the UE to indicate that
 * the authentication procedure has failed and that the UE shall abort
 * all activities.
 */
typedef struct EpsAuthenticationRejectTag
{
    /** This message does not have any IE */
    Int8                            empty;
}
EpsAuthenticationReject;

/** 24.301 8.2.7
 * Authentication request
 * This message is sent by the network to the UE to initiate
 * authentication of the UE identity.
 */
typedef struct EpsAuthenticationRequestTag
{
    /** NAS key set identifier */
    NasKeySetIdentifierElement      nasKsi;
    /** Authentication parameter RAND (EPS challenge) */
    AuthParamRandElement            authParamRand;
    /** Authentication parameter AUTN (EPS challenge) */
    AuthParamAutnElement            authParamAutn;
}
EpsAuthenticationRequest;

/** 24.301 8.2.8
 * Authentication response
 * This message is sent by the UE to the network to deliver
 * a calculated authentication response to the network.
 */
typedef struct EpsAuthenticationResponseTag
{
    /** Authentication response parameter */
    AuthParamResElement             authParamRes;
}
EpsAuthenticationResponse;

/** 24.301 8.2.10
 * Detach accept
 * This message is sent by the network or UE to indicate that
 * the detach procedure has been completed.
 * Detach Accept messages (MO and MT) do not have any IEs.
 */
typedef struct EpsDetachAcceptTag
{
    /** This message does not have any additional IE */
    Int8                            empty;
}
EpsDetachAccept;

/** 24.301 8.2.11.1
 * Detach request (UE originating detach) - MO detach
 * This message is sent by the UE to request the release of an EMM context.
 */
typedef struct EpsMoDetachRequestTag
{
    /** Detach type */
    EpsMoDetachTypeElement          detachType;
    /** NAS key set identifier */
    NasKeySetIdentifierElement      nasKsi;
    /** GUTI or IMSI */
    MobileIdElement                 gutiOrImsi;
}
EpsMoDetachRequest;

/** 24.301 8.2.11.2
 * Detach request (UE terminated detach) - MT detach
 * This message is sent by the network to request the release of an EMM context.
 */
typedef struct EpsMtDetachRequestTag
{
    /** Detach type */
    EpsMtDetachTypeElement          detachType;
    /** Indicates if EMM cause is present.
\assoc PRESENT \ref emmCause */
    Boolean                         emmCausePresent;
    /** EMM cause */
    GsmCause                        emmCause;
}
EpsMtDetachRequest;

/** 24.301 8.2.12
 * Downlink NAS Transport
 * This message is sent by the network to the UE in order to carry an SMS message in encapsulated format.
 */
typedef struct EpsDownlinkNasTransportTag
{
    /** NAS message container */
    NasMessageContainerElement      nasMessageContainer;
}
EpsDownlinkNasTransport;

/** 24.301 8.2.13
 * EMM Information
 * this message is sent by the network at any time whilst EMM context is established
 * to send certain information to the UE.
 */
typedef struct EpsEmmInformationTag
{
    /** Whether or not full network name is present
    \assoc PRESENT \ref fullNetworkName */
    Boolean                   fullNetworkNamePresent;
    /** Indicates full network name information */
    NetworkName               fullNetworkName;
    /** Indicates the abbreviated name of the network
    \assoc PRESENT \ref shortNetworkName */
    Boolean                   shortNetworkNamePresent;
    /** Indicates short network name information */
    NetworkName               shortNetworkName;
    /** Indicates if location time zone is present
    \assoc PRESENT \ref locTimeZone */
    Boolean                   locTimeZonePresent;
    /** The purpose of this parameter is to encode the
     * offset between universal time and local time in steps
     * of 15 minutes.
     */
    LocTimeZone               locTimeZone;
    /** whether the universal time and local time zone is present
    \assoc PRESENT \ref uniTimeAndLocTimeZone */
    Boolean                   uniTimeAndLocTimeZonePresent;
    /** The purpose of the timezone is to encode the offset between universal
     * time and local time in steps of 15 minutes and the purpose of the time
     * part of this information element is to encode the universal time at which
     * this information element may have been sent by the network
    */
    UniTimeAndLocTimeZone     uniTimeAndLocTimeZone;
    /** Indicates if daylight saving time is present
    \assoc PRESENT \ref networkDaylightSavingTime */
    Boolean                   networkDaylightSavingTimePresent;
    /** Indicates the value that has been used to adjust the local time zone */
    NetworkDaylightSavingTime networkDaylightSavingTime;
}
EpsEmmInformation;

/** 24.301 8.2.14
 * EMM Status
 * this message is sent by the UE or network at any time to report error conditions.
 */
typedef struct EpsEmmStatusTag
{
    /** EMM cause */
    GsmCause                        emmCause;
}
EpsEmmStatus;


/** 24.301 8.2.16
* GUTI Reallocation Command
*This message is sent by the network to the UE to reallocate a GUTI and optionally to
provide a new TAI list.
*/
typedef struct EpsGutiReallocationCommandTag
{
    /** new value for GUTI */
    MobileIdElement                 guti;
    /** Indicates if a new TAI list is present.
       \assoc PRESENT \ref taiList */
    Boolean                         taiListPresent;
    /** TAI list */
    TaiListElement                  taiList;

#ifdef FEA_TEMP_REL14_DCN
    /** Indicates if DCN ID IE is present
\assoc PRESENT \ref dcnId */
    Boolean                         dcnIdPresent;
    /* Dedicated core Network identification */
    Int16                           dcnId;
#endif /* FEA_TEMP_REL14_DCN */
}
EpsGutiReallocationCommand;


/** 24.301 8.2.17
* GUTI Reallocation Complete
* This message is sent by the UE to the network to indicate that reallocation of a GUTI
* has taken place. The signal has no content.
*/
typedef struct EpsGutiReallocationCompleteTag
{
    Int8                            empty;
}
EpsGutiReallocationComplete;


/** 24.301 8.2.18
 * Identity request
 * This message is sent by the network to the UE to request the UE to provide
 * the specified identity.
 */
typedef struct EpsIdentityRequestTag
{
    /** Identity type */
    IdTypeElement                   idType;
}
EpsIdentityRequest;

/** 24.301 8.2.19
 * Identity response
 * This message is sent by the UE to the network in response to
 * an IDENTITY REQUEST message and provides the requested identity.
 */
typedef struct EpsIdentityResponseTag
{
    /** Mobile identity */
    MobileIdElement                 mobileId;
}
EpsIdentityResponse;

/** 24.301 8.2.20
 * Security mode command
 * This message is sent by the network to the UE to establish
 * NAS signalling security.
 */
typedef struct EpsSecurityModeCommandTag
{
    /** Mobile identity */
    EpsNasSecurityAlgorithmsElement nasSecurityAlgorithms;
    /** NAS key set identifier */
    NasKeySetIdentifierElement      nasKsi;
    /** NAS key set identifier */
    EpsUeSecurityCapabilityElement  ueSecurityCapability;
    /** Whether or not IMEISV request is present.
\assoc PRESENT \ref imeisvRequest */
    Boolean                         imeisvRequestPresent;
    /** Whether or not IMEISV is requested */
    Boolean                         imeisvRequest;
    /** Whether or not replayed nonce UE is present
\assoc PRESENT \ref replayedNonceUe */
    Boolean                         replayedNonceUePresent;
    /** Replayed nonce UE */
    NonceElement                    replayedNonceUe;
    /** Whether or not nonce MME is present.
\assoc PRESENT \ref nonceMme */
    Boolean                         nonceMmePresent;
    /** Nonce MME */
    NonceElement                    nonceMme;
#ifdef FEA_TEMP_REL14_HASH
    /** Whether or not Hash MME is present.
\assoc PRESENT \ref hashMme */
    Boolean                        hashMmePresent;
    /** Hash MME */
    HashElement                    hashMme;
#endif /* FEA_TEMP_REL14_HASH */
}
EpsSecurityModeCommand;

/** 24.301 8.2.21
 * Security mode complete
 * This message is sent by the UE to the network in response to
 * a SECURITY MODE COMMAND message.
 */
typedef struct EpsSecurityModeCompleteTag
{
    /** Indicates if IMEISV is present.
\assoc PRESENT \ref imeisv */
    Boolean                         imeisvPresent;
    /** IMEISV */
    MobileIdElement                 imeisv;
#ifdef FEA_TEMP_REL14_HASH
    /** Indicates if Replayed Nas Message is present.
\assoc PRESENT \ref replayedNasMsg */
    Boolean                         replayedNasMsgPresent;
    ReplayedNasMsgElement           replayedNasMsg;
#endif /* FEA_TEMP_REL14_HASH */
}
EpsSecurityModeComplete;

/** 24.301 8.2.22
 * Security mode reject
 * This message is sent by the UE to the network to indicate that
 * the corresponding security mode command has been rejected.
 */
typedef struct EpsSecurityModeRejectTag
{
    /** EMM cause */
    GsmCause                        emmCause;
}
EpsSecurityModeReject;

/** 24.301 8.2.24
 * Service Reject
 * This message is sent by the network to the UE in order to reject
 * the service request procedure.
 */
typedef struct EpsServiceRejectTag
{
    /** EMM cause */
    GsmCause                        emmCause;
    /** Indicates if T3442 value is present.
    \assoc PRESENT \ref t3442Timer */
    Boolean                         t3442TimerPresent;
    /** T3442 value, if not included, the UE shall use the default value */
    GprsTimerElement                t3442Timer;
    /** Indicates if T3346 value is present.
    \assoc PRESENT \ref t3346Timer */
    Boolean                         t3346TimerPresent;
    /** T3346 value */
    GprsTimerElement                t3346Timer;
#ifdef FEA_TEMP_REL14_EMM_CP_CONGESTION_CONTROL
    /** Indicates if T3448 value is present.
    \assoc PRESENT \ref t3448Timer */
    Boolean                         t3448TimerPresent;
    /** T3448 value */
    GprsTimerElement                t3448Timer;
#endif /* FEA_TEMP_REL14_EMM_CP_CONGESTION_CONTROL  */
}
EpsServiceReject;


/** 24.301 8.2.26
 * Tracking area update accept sent by network to UE to provide UE with EPS MM related data
 * in repsonse to a TAU request
 */
typedef struct EpsTrackingAreaUpdateAcceptTag
{
    EpsUpdateResult                 epsUpdateResult;
    /** Indicates if an update for the value of T3412 is present.
    \assoc PRESENT \ref t3412Value */
    Boolean                         t3412ValuePresent;
    /* updated value for timer value for T3412 - periodic TAU */
    GprsTimer                       t3412Value;
    /** Indicates if GUTI is present.
    \assoc PRESENT \ref guti */
    Boolean                         gutiPresent;
    /** new value for GUTI */
    MobileIdElement                 guti;
    /** Indicates if a new TAI list is present.
    \assoc PRESENT \ref taiList */
    Boolean                         taiListPresent;
    /** TAI list */
    TaiListElement                  taiList;
    /** indicates if EPS bearer context status IE is present.
    \assoc PRESENT \ref psdBearerContextStatus */
    Boolean                         psdBearerContextStatusPresent;
    /** indicates what PSD Bearer Contexts are active for UE in the network */
    PsdBearerContextStatus          psdBearerContextStatus;
#ifdef FEA_COMBINED_PROCEDURES
    /** Indicates if the  Location Area Identification is present.
    \assoc PRESENT \ref locationAreaId */
    Boolean                         locationAreaIdPresent;
    /** included to assign a new LAI to a UE during a combined TA/LA */
    Lai                             locationAreaId;
    /** Indicates if MS identity is present.
    \assoc PRESENT \ref msId */
    Boolean                         msIdPresent;
    /**included to assign or unassign a new TMSI during a combined LA/TA update */
    MobileIdElement                 msId;
    /** Indicates if EMM cause is present
    \assoc PRESENT \ref emmCause */
    Boolean                         emmCausePresent;
    /** included if the combined TAU was only successful for EPS services */
    GsmCause                        emmCause;
#endif /* FEA_COMBINED_PROCEDURES */
    /** Indicates if an update for the value of T3402 is present.
    \assoc PRESENT \ref t3402Value */
    Boolean                         t3402ValuePresent;
    /* updated value for timer value for T3402 */
    GprsTimer                       t3402Value;
    /** Indicates if an update for the value of T3423 is present.
    \assoc PRESENT \ref t3423Value */
    Boolean                         t3423ValuePresent;
    /* updated value for timer value for T3423 */
    GprsTimer                       t3423Value;
     /** included to assign a new equivalent PLMNs list to a UE.
     * If not included numPlmn field is set to 0.
     */
    EquivalentPlmnListElement       equivalentPlmnList;
    /** Indicates if a list of emergency numbers is present.
    \assoc PRESENT \ref emergencyNumberList */
    Boolean                         emergencyNumberListPresent;
    /** May be included to indicate a list of emergency numbers valid within
     * the same MCC as in the cell on which this IE is received.
     */
    EmergencyNumberList             emergencyNumberList;
    /** Indicates if EPS network feature support is present.
    \assoc PRESENT \ref epsNetworkFeatureSupport */
    Boolean                         epsNetworkFeatureSupportPresent;
    /** included to inform the UE of the support of certain features.
     * If this is not included then the respective features are not supported.
     */
    EpsNetworkFeatureSupportElement epsNetworkFeatureSupport;
    /** Indicates if additional update result is present.
    \assoc PRESENT \ref additionalUpdateResult */
    Boolean                         additionalUpdateResultPresent;
    /** May be included to provide the UE with additional information about
     * the result of a combined TAU if the procedure was successful
     * for EPS services and non-EPS services, or for EPS services and SMS-only.
     */
    AdditionalUpdateResultElement   additionalUpdateResult;
    /** Indicates if t3412 Extended Timer is present.
    \assoc PRESENT \ref t3412ExtendedTimer */
    Boolean                         t3412ExtendedTimerPresent;
    GprsTimer3Element               t3412ExtendedTimer;
    /** Indicates if t3324 is present.
    \assoc PRESENT \ref t3324Timer */
    Boolean                         t3324TimerPresent;
    GprsTimerElement                t3324Timer;
    /** Indicates if eDRX is present.
    \assoc PRESENT \ref edrxElement */
    Boolean                         edrxPresent;
    /** The UE shall include this IE if UE uses eDRX */
    EdrxElement                     edrxElement;
    /** Indicates if Header Compression configuration status is present.
    \assoc PRESENT \ref headerComprConfStatus */
    Boolean                         headerComprConfStatusPresent;
    HeaderCompressionStatus         headerComprConfStatus;
    /** Indicates if sms services status is present.
    \assoc PRESENT \ref smsServicesStatus */
    Boolean                         smsServicesStatusPresent;
    /** Shall be included when normal TAU for EPS services and SMS only was
      * successful for EPS services only.
     */
    SMSServicesStatusElement        smsServicesStatus;

#ifdef FEA_TEMP_REL14_DCN
    /** Indicates if DCN ID IE is present
\assoc PRESENT \ref dcnId */
    Boolean                         dcnIdPresent;
    /* Dedicated core Network identification */
    Int16                           dcnId;
#endif /* FEA_TEMP_REL14_DCN */

#ifdef FEA_TEMP_REL14_EMM_CP_CONGESTION_CONTROL
    /** Indicates if T3448 value is present.
    \assoc PRESENT \ref t3448Timer */
    Boolean                         t3448TimerPresent;
    /** T3448 value */
    GprsTimerElement                t3448Timer;
#endif /* FEA_TEMP_REL14_EMM_CP_CONGESTION_CONTROL  */

}
EpsTrackingAreaUpdateAccept;

/** 24.301 8.2.27
 * Tracking area update complete sent by UE to network in response to TAU accept
 * if GUTI has been changed or new TMSI assigned. Signal has no content.
 */
typedef struct EpsTrackingAreadUpdateCompleteTag
{
    Int8                            empty;
}
EpsTrackingAreaUpdateComplete;

/** 24.301 8.2.28
 * Tracking area update reject sent by network to UE to reject the TAU procedure
 */
typedef struct EpsTrackingAreadUpdateRejectTag
{
    /** EMM cause */
    GsmCause                        emmCause;
    /** Indicates if t3346 is present.
 \assoc PRESENT \ref t3346Timer */
    Boolean                         t3346TimerPresent;
    GprsTimerElement                t3346Timer;
    /** Indicates if extendedEmmCause is present
 \assoc PRESENT \ref extendedEmmCause */
    Boolean                         extendedEmmCausePresent;
    ExtendedEmmCauseElement         extendedEmmCause;
}
EpsTrackingAreaUpdateReject;

/** 24.301 8.2.29
 * Tracking area update request sent by UE to network to update registration of actual
 * TA of the UE in the network or periodically notify the availablity of the UE to
 * the network
 */
typedef struct EpsTrackingAreadUpdateRequestTag
{
    EpsUpdateTypeElement            tauType;
    /** NAS key set identifier */
    NasKeySetIdentifierElement      nasKsi;
    /** Old GUTI */
    MobileIdElement                 oldGuti;
    /** Whether optional non-current native NAS key set identifier is present.
    \assoc PRESENT \ref nonCurrentNasKsi */
    Boolean                         nonCurrentNasKsiPresent;
    /** Non-current native NAS key set identifier */
    NasKeySetIdentifierElement      nonCurrentNasKsi;
    /** Indicates if additional GUTI is present.
    \assoc PRESENT \ref addnGuti */
    Boolean                         addnGutiPresent;
    /** Additional GUTI */
    MobileIdElement                 addnGuti;
    /** Indicates if epsUeNetworkCapability is present.
    \assoc PRESENT \ref epsUeNetworkCapability */
    Boolean                         epsUeNetworkCapabilityPresent;
    /** UE network capability */
    EpsUeNetworkCapabilityElement   epsUeNetworkCapability;
    /** Indicates if the Last registered Tracking Area Identity is present.
    \assoc PRESENT \ref lastRegTai */
    Boolean                         lastRegTaiPresent;
    /** Last registered TAI */
    Lai                             lastRegTai;
    /** whether UE radio capability info update is needed */
    Boolean                         ueRadioCapUpdateNeeded;
    /** indicates if EPS bearer context status IE is present.
    \assoc PRESENT \ref epsBearerContextStatus */
    Boolean                         epsBearerContextStatusPresent;
    /** Eps Bearer Context Status */
    PsdBearerContextStatus          epsBearerContextStatus;
    /** Indicates if MS network capability is present.
    \assoc PRESENT \ref msNetworkCapability */
    Boolean                         msNetworkCapabilityPresent;
    /** A UE supporting A/Gb mode or Iu mode shall include this IE
     * to indicate its capabilities to the network */
    MsNetworkCapabilityElement      msNetworkCapability;
    /** Indicates if the Old Location Area Identity is present.
    \assoc PRESENT \ref oldLocationAreaId*/
    Boolean                         oldLocationAreaIdPresent;
    /** Old Location Area Identity */
    Lai                             oldLocationAreaId;
#ifdef FEA_COMBINED_PROCEDURES
    /** Indicates if TMSI status is present.
    \assoc PRESENT \ref tmsiStatus */
    Boolean                         tmsiStatusPresent;
    /** tmsiStatus     */
    TmsiStatusElement               tmsiStatus;
    /** Indicates if Mobile station classmark 2 is present.
     * This IE shall be included if the UE supports SRVCC to GERAN or
     * UTRAN (see 3GPP TS 23.216 [8]), or if the UE is performing
     * a combined tau procedure.*/
    Boolean                         msClassMark2Present;
    /** If present Mobile station classmark 2 is stored here */
    MsClassmarkElement              msClassmark;
    Boolean                         tmsiBasedNriContainerPresent;
    TmsiBasedNriContainerElement    nriContainerValue;
#endif /* FEA_COMBINED_PROCEDURES */
    /** Indicates if additional update type is present.
    \assoc PRESENT \ref additionalUpdateType */
    Boolean                         additionalUpdateTypePresent;
    /** The UE shall include this IE if the UE requests SMS-only */
    AdditionalUpdateTypeElement     additionalUpdateType;
  /** Indicates if old Guti is present.
    \assoc PRESENT \ref oldGutiType */
    Boolean                         oldGutiTypePresent;
    OldGutiTypeElement              oldGutiType;
    /** Indicates if device properties is present.
    \assoc PRESENT \ref deviceProperties */
    Boolean                         devicePropertiesPresent;
    /** The UE shall include this IE if UE is configured for NAS signalling
    low priority */
    DevicePropertiesElement         deviceProperties;
    /** Indicates if msNetworkFeatureSupport is present.
    \assoc PRESENT \ref msNetworkFeatureSupport */
    Boolean                         msNetworkFeatureSupportPresent;
    MsNetworkFeatureSupportElement  msNetworkFeatureSupport;
    /** Indicates if t3324 is present.
     \assoc PRESENT \ref t3324Timer */
    Boolean                         t3324TimerPresent;
    GprsTimerElement                t3324Timer;
    /** Indicates if t3412 Extended Timer is present.
     \assoc PRESENT \ref t3412ExtendedTimer */
    Boolean                         t3412ExtendedTimerPresent;
    GprsTimer3Element               t3412ExtendedTimer;
    /** Indicates if eDRX is present.
     \assoc PRESENT \ref edrxValue */
    Boolean                         edrxPresent;
    /** The UE shall include this IE if UE uses eDRX */
    EdrxElement                     edrxValue;
}
EpsTrackingAreaUpdateRequest;

/** 24.301 8.2.30
 * Uplink NAS Transport
 * This message is sent by the UE to the network in order to carry an SMS message in encapsulated format.
 */
typedef struct EpsUplinkNasTransportTag
{
    /** NAS message container */
    NasMessageContainerElement      nasMessageContainer;
}
EpsUplinkNasTransport;

/** 24.301 8.2.31 and 8.2.32
 * Uplink and downlink Generic NAS Transport
 * This message is sent by the UE to the network in order to carry an LPP or LCS message in encapsulated format.
 */
typedef struct EpsGenericNasTransportTag
{
    /** NAS message container */
    Int8                             genericMessageContainerType;
    GenericNasMessageContainer       genericMessageContainer;
}
EpsGenericNasTransport;

/** 24.301 8.2.33
 * Control Plane Service Request
 * This message is sent by the UE to the network when the UE is using EPS services
 * with control plane CIoT EPS optimization.
 */
typedef struct EpsCpServiceRequestTag
{
    /* CP service type */
    EpsCpServiceTypeElement         cpServiceType;
    /** NAS key set identifier */
    NasKeySetIdentifierElement      nasKsi;
    /* ESM message container present */
    Boolean                         esmMessageContainerPresent;
    /** indicates if NAS message container status IE is present.
       \assoc PRESENT \ref nasMessageContainer */
    Boolean                         nasMessageContainerPresent;
    /** NAS message container */
    NasMessageContainerElement      nasMessageContainer;
    /** indicates if EPS bearer context status IE is present.
       \assoc PRESENT \ref epsBearerContextStatus */
    Boolean                         epsBearerContextStatusPresent;
    /** Eps Bearer Context Status */
    PsdBearerContextStatus          epsBearerContextStatus;
    /** Indicates if device properties is present.
       \assoc PRESENT \ref deviceProperties */
    Boolean                         devicePropertiesPresent;
    /** The UE shall include this IE if UE is configured for NAS signalling
       low priority */
    DevicePropertiesElement         deviceProperties;

}
EpsCpServiceRequest;

/** 24.301 8.2.34
* Service Accept
* This message is sent by the network in response to the CONTROL PLANE SERVICE REQUEST message.
*/
typedef struct EpsServiceAcceptTag
{
    /** indicates if EPS bearer context status IE is present.
       \assoc PRESENT \ref epsBearerContextStatus */
    Boolean                         epsBearerContextStatusPresent;
    /** Eps Bearer Context Status */
    PsdBearerContextStatus          epsBearerContextStatus;
#ifdef FEA_TEMP_REL14_EMM_CP_CONGESTION_CONTROL
    /** Indicates if T3448 value is present.
    \assoc PRESENT \ref t3448Timer */
    Boolean                         t3448TimerPresent;
    /** T3448 value */
    GprsTimerElement                t3448Timer;
#endif /* FEA_TEMP_REL14_EMM_CP_CONGESTION_CONTROL  */
}
EpsServiceAccept;

/*****************************************************************
 * Types used in LTE  ESM messages
 *****************************************************************/


typedef struct ReAttemptIndicatorTag
{
    Int8           eplmnc;
    Int8           ratc;
}
ReAttemptIndicator;

/** 24.301 9.9.4.22 */
typedef struct SetUpParametersContainerTag
{
    /** \assoc ARRAY \ref setUpData */
    Int16                   length;
    Int8                    setUpData [HEADER_COMPRESS_CONTEXT_MAX_SIZE];
}
SetUpParametersContainer;

/** 24.301 9.9.4.22 */
typedef struct HeaderCompressConfigElementTag
{
    /** Rohc profile IP */
       Boolean roHC_0x0104_IP_Support ;
    /** Rohc profile ESP/IP */
    Boolean roHC_0x0103_ESP_IP_Support;
    /** Rohc profile UPD/IP */
    Boolean roHC_0x0102_UDP_IP_Support;
    /** Rohc profile TCP/IP */
    Boolean roHC_0x0006_TCP_IP_Support;
    /** Rohc profile IP */
    Boolean roHC_0x0004_IP_Support;
    /** Rohc profile ESP/IP */
    Boolean roHC_0x0003_ESP_IP_Support;
    /** Rohc profile UPD/IP */
    Boolean roHC_0x0002_UDP_IP_Support;
    /** Max Cid , 1-16383 */
    Int16   max_cid;
    /** Additional header compression context setup parameters present */
    Boolean addHeaderCompressSetUpPresent;
    /** Additional header compression context setup parameters type */
    Int8    addHeaderCompressSetUpParametersType;
     /** Additional header compression context setup parameters container */
    SetUpParametersContainer    setUpParametersContainer;
}
HeaderCompressConfigElement;

/** 24.301 9.9.4.25 - DDX */
typedef enum DownlinkDataExpectedTag
{
    /** No information regarding DDX */
    DDX_NO_INFORMATION                  = 0,
    /** No further UL or DL data transmission subsequent to the uplink data transmission is expected */
    DDX_NO_FURTHER_UL_OR_DL             = 1,
    /** Only a single DL data transmission and no further UL data transmission subsequent
    to the uplink data transmission is expected */
    DDX_ONLY_SINGLE_DL_DATA_EXPECTED    = 2,
    /** Reserved */
    DDX_RESERVED                        = 3
}
DownlinkDataExpected;

/*****************************************************************
 * LTE ESM messages
 *****************************************************************/
/** 24.301 8.3.1
 * Activate dedicated EPS bearer context accept
 * This message is sent by the UE to the network to acknowledge
 * activation of a dedicated EPS bearer context associated with
 * the same PDN address(es) and APN as an already active EPS
 * bearer context.
 */
typedef struct EpsActDedicatedEpsBearerContextAcceptMessageTag
{
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
EpsActDedicatedEpsBearerContextAcceptMessage;

/** 24.301 8.3.2
 * Activate dedicated EPS bearer context reject
 * This message is sent by UE to the network to reject activation
 * of a dedicated EPS bearer context.
 */
typedef struct EpsActDedicatedEpsBearerContextRejectMessageTag
{
    /** Cause value for the reject */
    GsmCause                esmCause;
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
EpsActDedicatedEpsBearerContextRejectMessage;

/** 24.301 8.3.3
 * Activate dedicated EPS bearer context request
 * This message is sent by the network to the UE to request
 * activation of a dedicated EPS bearer context associated
 * with the same PDN address(es) and APN as an already active
 * default EPS bearer context.
 */
typedef struct EpsActDedicatedEpsBearerContextRequestMessageTag
{
    /** Linked EPS bearer identity */
    Int8                    linkedEpsBearerId;
    /** EPS quality of service (AND Negotiated QoS if present) */
    QualityOfService        qos;
    /** Traffic flow template */
    TrafficFlowTemplate     tft;
    /** Indicates if transaction identifier is present.
\assoc PRESENT \ref ti */
    Boolean                 tiPresent;
    /** Transaction identifier - coded same as Linked TI */
    LinkedTi                ti;
    /** Indicates if negotiated LLC SAPI is present.
\assoc PRESENT \ref negotiatedLlcSapi */
    Boolean                 negotiatedLlcSapiPresent;
    /** Negotiated LLC SAPI */
    LlcSapi                 negotiatedLlcSapi;
    /** Indicates if radio priority level is present.
\assoc PRESENT \ref rpl */
    Boolean                 rplPresent;
    /** Radio priority level */
    RadioPriorityLevel      rpl;
    /** Indicates if packet flow identifier is present.
\assoc PRESENT \ref packetFlowId */
    Boolean                 packetFlowIdPresent;
    /** Packet Flow Identifier */
    PacketFlowId            packetFlowId;
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
EpsActDedicatedEpsBearerContextRequestMessage;

/** 24.301 8.3.4
 * Activate default EPS bearer context accept
 * This message is sent by the UE to the network to acknowledge
 * activation of a default EPS bearer context.
 */
typedef struct EpsActDefaultEpsBearerContextAcceptMessageTag
{
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
EpsActDefaultEpsBearerContextAcceptMessage;

/** 24.301 8.3.5
 * Activate default EPS bearer context reject
 * This message is sent by UE to the network to reject activation
 * of a default EPS bearer context.
 */
typedef struct EpsActDefaultEpsBearerContextRejectMessageTag
{
    /** ESM cause value for the reject */
    GsmCause                esmCause;
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
EpsActDefaultEpsBearerContextRejectMessage;

/** 24.301 8.3.6
 * Activate default EPS bearer context request
 * This message is sent by the network to the UE to request activation
 * of a default EPS bearer context.
 */
typedef struct EpsActDefaultEpsBearerContextRequestMessageTag
{
    /** EPS quality of service (and Negotiated QoS if present) */
    QualityOfService        qos;
    /** Access point name */
    AccessPointName         apn;
    /** PDN address */
    PdnAddress              pdnAddress;
    /** Indicates if transaction identifier is present.
\assoc PRESENT \ref ti */
    Boolean                 tiPresent;
    /** Transaction identifier - coded same as Linked TI */
    LinkedTi                ti;
    /** Indicates if negotiated LLC SAPI is present.
\assoc PRESENT \ref negotiatedLlcSapi */
    Boolean                 negotiatedLlcSapiPresent;
    /** Negotiated LLC SAPI */
    LlcSapi                 negotiatedLlcSapi;
    /** Indicates if radio priority level is present.
\assoc PRESENT \ref rpl */
    Boolean                 rplPresent;
    /** Radio priority level */
    RadioPriorityLevel      rpl;
    /** Indicates if packet flow identifier is present.
\assoc PRESENT \ref packetFlowId */
    Boolean                 packetFlowIdPresent;
    /** Packet Flow Identifier */
    PacketFlowId            packetFlowId;
    /** Indicates if APN aggregate maximum bit rate is present.
\assoc PRESENT \ref apnAmbr */
    Boolean                 apnAmbrPresent;
    /** APN aggregate maximum bit rate */
    ApnAmbr                 apnAmbr;
    /** Indicates if ESM cause is present.
\assoc PRESENT \ref esmCause */
    Boolean                 esmCausePresent;
    /** ESM cause value */
    GsmCause                esmCause;
    Boolean                 cpOnlyIndication;
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
    /** Indicates if Header compression configuration is present.
\assoc PRESENT \ref headerCompressConfig */
    Boolean                     headerCompressConfigPresent;
    /** May be included to inform the UE of the support of certain features.
     * If this is not included then the respective features are not supported.
     */
    HeaderCompressConfigElement headerCompressConfig;
/** Indicates if Serving PLMN Rate Control is present.
\assoc PRESENT \ref servingPlmnRateControl */
    Boolean                 servingPlmnRateControlPresent;
    /** Serving PLMN Rate Control */
    ServingPlmnRateControl  servingPlmnRateControl;
}
EpsActDefaultEpsBearerContextRequestMessage;

/** 24.301 8.3.7
 * Bearer resource allocation reject
 * This message is sent by the network to the UE to reject
 * the allocation of a dedicated bearer resource.
 */
typedef struct EpsBearerResourceAllocRejectMessageTag
{
    /** ESM cause value for the reject */
    GsmCause                esmCause;
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
EpsBearerResourceAllocRejectMessage;

/** 24.301 8.3.8
 * Bearer resource allocation request
 * This message is sent by the UE to the network to request
 * the allocation of a dedicated bearer resource.
 */
typedef struct EpsBearerResourceAllocRequestMessageTag
{
    /** Linked EPS bearer identity */
    Int8                    linkedEpsBearerId;
    /** Traffic flow aggregate (coded same as TFT) */
    TrafficFlowTemplate     tft;
    /** Required traffic flow QoS (EPS QoS) */
    QualityOfService        qos;
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
EpsBearerResourceAllocRequestMessage;

/** 24.301 8.3.9
 * Bearer resource modification reject
 * This message is sent by the network to the UE to reject
 * the modification of a dedicated bearer resource.
 */
typedef struct EpsBearerResourceModifyRejectMessageTag
{
    /** ESM cause value for the reject */
    GsmCause                esmCause;
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
    /** Indicates if Back-OFF timer is present.
\assoc PRESENT \ref backOffTimer */
    Boolean                 backOffTimerPresent;
    /** back-off timer*/
    GprsTimer3Element       backOffTimer;
   /** Indicates if reAttemtIndication is present.
\assoc PRESENT \ref reAttemptIndicator */
    Boolean                 reAttemptIndicatorPresent;
    /** Re-Attempt Indicator */
    ReAttemptIndicator     reAttemptIndicator;
}
EpsBearerResourceModifyRejectMessage;

/** 24.301 8.3.10
 * Bearer resource modification request
 * This message is sent by the UE to the network to request
 * the modification of a dedicated bearer resource.
 */
typedef struct EpsBearerResourceModifyRequestMessageTag
{
    /** EPS bearer identity for packet filter (i.e. the EPS bearer
     * context associated with the modification)
     */
    Int8                    packetFilterEpsBearerId;
    /** Traffic flow aggregate (coded same as TFT) */
    TrafficFlowTemplate     tft;
    /** Indicates if required traffic flow QoS (EPS QoS) is present.
\assoc PRESENT \ref qos */
    Boolean                 qosPresent;
    /** Required traffic flow QoS (EPS QoS) */
    QualityOfService        qos;
    /** Indicates if ESM cause is present.
\assoc PRESENT \ref esmCause */
    Boolean                 esmCausePresent;
    /** ESM cause value for the modification */
    GsmCause                esmCause;
    /** Indicates if Header compression configuration is present.
\assoc PRESENT \ref headerCompressConfig */
    Boolean                     headerCompressConfigPresent;
    /** May be included to inform the UE of the support of certain features.
     * If this is not included then the respective features are not supported.
     */
    HeaderCompressConfigElement headerCompressConfig;
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
EpsBearerResourceModifyRequestMessage;

/** 24.301 8.3.11
 * Deactivate EPS bearer context accept
 * This message is sent by the UE to acknowledge deactivation of
 * the EPS bearer context requested in the corresponding Deactivate
 * EPS bearer context request message.
 */
typedef struct EpsDeactEpsBearerContextAcceptMessageTag
{
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
EpsDeactEpsBearerContextAcceptMessage;

/** 24.301 8.3.12
 * Deactivate EPS bearer context request
 * This message is sent by the network to request deactivation
 * of an active EPS bearer context.
 */
typedef struct EpsDeactEpsBearerContextRequestMessageTag
{
    /** ESM cause value for the deactivation */
    GsmCause                esmCause;
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
    /** Indicates if T3396Timer is present.
\assoc PRESENT \ref t3396Timer */
    Boolean                 t3396TimerPresent;
    /** t3396Timer */
    GprsTimer3Element       t3396Timer;
}
EpsDeactEpsBearerContextRequestMessage;

/** 24.301 8.3.13
 * ESM information request
 * This message is sent by the network to the UE to request the UE
 * to provide ESM information, i.e. protocol configuration options
 * or APN or both.
 */
typedef struct EpsEsmInfoRequestMessageTag
{
    /** This message does not have any additional IE */
    Int8                    empty;
}
EpsEsmInfoRequestMessage;

/** 24.301 8.3.14
 * ESM information response
 * This message is sent by the UE to the network in response to
 * an ESM INFORMATION REQUEST message and provides the requested
 * ESM information.
 */
typedef struct EpsEsmInfoResponseMessageTag
{
    /** Indicates if APN is present.
\assoc PRESENT \ref apn */
    Boolean                 apnPresent;
    /** Access point name */
    AccessPointName         apn;
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
EpsEsmInfoResponseMessage;

/** 24.301 8.3.15
 * ESM status
 * This message is sent by the network or the UE to pass information
 * on the status of the indicated EPS bearer context and report
 * certain error conditions.
 */
typedef struct EpsEsmStatusMessageTag
{
    /** ESM cause value */
    GsmCause                esmCause;
}
EpsEsmStatusMessage;

/** 24.301 8.3.16
 * Modify EPS bearer context accept
 * This message is sent by the UE to the network to acknowledge
 * the modification of an active EPS bearer context.
 */
typedef struct EpsModifyEpsBearerContextAcceptMessageTag
{
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
EpsModifyEpsBearerContextAcceptMessage;

/** 24.301 8.3.17
 * Modify EPS bearer context reject
 * This message is sent by the UE or the network to reject
 * a modification of an active EPS bearer context.
 */
typedef struct EpsModifyEpsBearerContextRejectMessageTag
{
    /** Cause value for the reject */
    GsmCause                esmCause;
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
EpsModifyEpsBearerContextRejectMessage;

/** 24.301 8.3.18
 * Modify EPS bearer context request
 * This message is sent by the network to the UE to request
 * modification of an active EPS bearer context.
 */
typedef struct EpsModifyEpsBearerContextRequestMessageTag
{
    /** Indicates if QoS is present.
\assoc PRESENT \ref qos */
    Boolean                 qosPresent;
    /** New EPS quality of service (AND new pre-Rel8 QoS if present) */
    QualityOfService        qos;
    /** Indicates if TFT is present.
\assoc PRESENT \ref tft */
    Boolean                 tftPresent;
    /** Traffic flow template */
    TrafficFlowTemplate     tft;
    /** Indicates if negotiated LLC SAPI is present.
\assoc PRESENT \ref negotiatedLlcSapi */
    Boolean                 negotiatedLlcSapiPresent;
    /** Negotiated LLC SAPI */
    LlcSapi                 negotiatedLlcSapi;
    /** Indicates if radio priority level is present.
\assoc PRESENT \ref rpl */
    Boolean                 rplPresent;
    /** Radio priority level */
    RadioPriorityLevel      rpl;
    /** Indicates if packet flow identifier is present.
\assoc PRESENT \ref packetFlowId */
    Boolean                 packetFlowIdPresent;
    /** Packet Flow Identifier */
    PacketFlowId            packetFlowId;
    /** Indicates if APN aggregate maximum bit rate is present.
\assoc PRESENT \ref apnAmbr */
    Boolean                 apnAmbrPresent;
    /** APN aggregate maximum bit rate */
    ApnAmbr                 apnAmbr;
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
    /** Indicates if Header compression configuration is present.
\assoc PRESENT \ref headerCompressConfig */
    Boolean                     headerCompressConfigPresent;
    /** May be included to inform the UE of the support of certain features.
     * If this is not included then the respective features are not supported.
     */
    HeaderCompressConfigElement headerCompressConfig;
}
EpsModifyEpsBearerContextRequestMessage;

/** 24.301 8.3.19
 * PDN connectivity reject
 * This message is sent by the network to the UE to reject establishment
 * of a PDN connection.
 */
typedef struct EpsPdnConnectivityRejectMessageTag
{
    /** ESM cause value for the reject */
    GsmCause                esmCause;
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
    /** Indicates if Back-OFF timer is present.
\assoc PRESENT \ref backOffTimer */
    Boolean                 backOffTimerPresent;
    /** back-off timer*/
    GprsTimer3Element       backOffTimer;
   /** Indicates if reAttemtIndication is present.
\assoc PRESENT \ref reAttemptIndicator */
    Boolean                 reAttemptIndicatorPresent;
    /** Re-Attempt Indicator */
    ReAttemptIndicator     reAttemptIndicator;

}
EpsPdnConnectivityRejectMessage;

/** 24.301 8.3.20
 * PDN connectivity request
 * This message is sent by the UE to the network to initiate establishment
 * of a PDN connection.
 */
typedef struct EpsPdnConnectivityRequestMessageTag
{
    /** Request type (Initial, Handover) */
    RequestType             requestType;
    /** PDN type (IPv4, IPv6, or IPv4v6) */
    PdnType                 pdnType;
    /** Indicates if ESM information transfer flag needs to be sent.
     * The UE shall include this IE in the PDN CONNECTIVITY REQUEST message
     * sent during the attach procedure if the UE has protocol configuration
     * options that need to be transferred security protected or wishes to
     * provide an access point name for the PDN connection to be established
     * during the attach procedure.
     * */
    Boolean                 esmInformationTransfer;
    /** Indicates if APN is present.
\assoc PRESENT \ref apn */
    Boolean                 apnPresent;
    /** Access point name */
    AccessPointName         apn;
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
    /** Indicates if device properties is present.
\assoc PRESENT \ref deviceProperties */
    Boolean                         devicePropertiesPresent;
    /** The UE shall include this IE if UE is configured for NAS signalling
    low priority */
    DevicePropertiesElement         deviceProperties;
    /** Indicates if Header compression configuration is present.
\assoc PRESENT \ref headerCompressConfig */
    Boolean                     headerCompressConfigPresent;
    /** May be included to inform the UE of the support of certain features.
     * If this is not included then the respective features are not supported.
     */
    HeaderCompressConfigElement headerCompressConfig;
}
EpsPdnConnectivityRequestMessage;

/** 24.301 8.3.21
 * PDN disconnect reject
 * This message is sent by the network to the UE to reject release
 * of a PDN connection.
 */
typedef struct EpsPdnDisconnectRejectMessageTag
{
    /** ESM cause value for the reject */
    GsmCause                esmCause;
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
EpsPdnDisconnectRejectMessage;

/** 24.301 8.3.22
 * PDN disconnect request
 * This message is sent by the UE to the network to initiate release
 * of a PDN connection.
 */
typedef struct EpsPdnDisconnectRequestMessageTag
{
    /** Linked EPS bearer identity */
    Int8                    linkedEpsBearerId;
    /** Indicates if protocol configuration options are present.
\assoc PRESENT \ref pco */
    Boolean                 pcoPresent;
    /** Protocol configuration options */
    ProtocolConfigOptions   pco;
}
EpsPdnDisconnectRequestMessage;

/** 24.301 8.3.25
 *  ESM DATA TRANSPORT
 *  This message is sent by the UE or the network in order to carry user data
 *  in an encapsulated format.
 */
typedef struct EpsEsmDataTransportMessageTag
{
    /** Indicates if Release assistance indication is present */
    /** \assoc PRESENT \ref releaseAssistanceInd */
    Boolean               releaseAssistanceIndPresent;

    Int8                  releaseAssistanceInd;
    /** User data container  */

    UserDataContainer     userDataContainer;
}
EpsEsmDataTransportMessage;

/** 24.301 8.3.12A
 *  ESM DUMMY MESSAGE
 *  This message is sent by the UE or the network in an ESM message container information element
 *  during an attach procedure, if the UE does not request for PDN connection.
 */
typedef struct EpsEsmDummyMessageTag
{
    /** This message does not have any additional IE */
    Int8                            empty;
}
EpsEsmDummyMessage;


#endif  /* UPGRADE_LTE */

#endif /* L3EPS_TYP_H */
/* END OF FILE */
