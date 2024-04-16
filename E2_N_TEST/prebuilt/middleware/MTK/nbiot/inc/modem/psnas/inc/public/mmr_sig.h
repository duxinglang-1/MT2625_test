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
 *  File Description :                                                   */
/** \file
 *      application layer - Mm interface (registration)
 **************************************************************************/

#ifndef MMR_SIG_H
#define MMR_SIG_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

# if !defined (KI_TYP_H)
# include     <ki_typ.h>
# endif /* !(KI_TYP_H) */

# if !defined (MM_COMM_H)
# include     <mm_comm.h>
# endif /* !(MM_COMM_H) */

#include     <gmm_comm.h>

# include     <emm_comm.h>

# if defined (UPGRADE_GGE)
# include     <llm_sig.h>
# if !defined (GRR_SIG_H)
# include     <grr_sig.h>
# endif /* !(GRR_SIG_H) */
# endif /* (UPGRADE_GGE) */

# if !defined (GRRMRTYP_H)
# include     <grrmrtyp.h>
# endif /* !(GRRMRTYP_H) */

# if !defined (MNL3_TYP_H)
# include     <mnl3_typ.h>
# endif /* !(MNL3_TYP_H) */

# if !defined (L3_TYP_H)
# include     <l3_typ.h>
# endif /* !(L3_TYP_H) */

# if defined (UPGRADE_3G)
# include     <rrc_sig.h>
# endif /* (UPGRADE_3G) */

# include <eps_asn.h>
# include <eps_typ.h>
#if defined (EMM_UNIT_TESTING)
#ifdef MTK_NVDM_MODEM_ENABLE
#include "modem_conf_nvram_def.h"
#include "nvdm_modem_default_config.h"
#endif
#endif /* EMM_UNIT_TESTING */

/** \defgroup SigMmr MMR Signalling Interface
 * \ingroup Prd3gDmNas
 * \ingroup Sig
 * Signalling Interface between application layer - Mm/GMM interface (registration).
 * This section defines the signals exchanged between MM/GMM and application layer tasks.
 * This interface allows PLMN list retrieval and communication of various ME data to the protocol stack.
 * This section incorporates requirements found in [3GPP TS 04.07].
 * The diagram below shows the context of MMR
 *
 * \image html MMxx_Interface.png
 * @{
 */


/****************************************************************************
 * Macros
 ****************************************************************************/
#if defined (UPGRADE_3G)
/* CUSTOM_COMMAND */
#define MAX_NUM_CUSTOM_COMMAND_BIT      128
#define MAX_NUM_CUSTOM_COMMAND          16

#define UNUSED_MCC          (0xFFF)
#define UNUSED_MNC          (0xFF)

#define MAX_NFM_PAR_VALUE 7

/****************************************************************************
 * Types
 ****************************************************************************/
/** Identifies the custom command */
typedef enum CustomCommandTag
{
    CUSTOM_COMMAND_CONFIG_PS_COT                = 0,
    CUSTOM_COMMAND_CONFIG_RF_COT                = 1,
    CUSTOM_COMMAND_CONFIG_BYPASS_IWAP_CELL      = 2,
    CUSTOM_COMMAND_CONFIG_USIM_COT              = 3,
    CUSTOM_COMMAND_CONFIG_DISABLE_INTEG_CHECK   = 4,
    CUSTOM_COMMAND_REALNW_TESTNW_CONFLICT       = 8,
    CUSTOM_COMMAND_CONFIG_GCF                   = 64,
    CUSTOM_COMMAND_CONFIG_NUMBER,
    CUSTOM_COMMAND_INVALID_CONFIG    = MAX_NUM_CUSTOM_COMMAND_BIT
}
CustomCommand;
#endif

/** Identifies the status of the service.
 * Where the status indicates GPRS this term covers the relevant packet switched
 * service (can be GPRS or EPS services depending upon the access technology being used).
 */
typedef enum ServiceStatusTag
{
    /** Used internally by (G)MM. Implies that the MS is
     * registered to the network (normal service)
     */
    PLMN_NORMAL,                  /* MM in idle, normal service */
    /** The MS is able to make emergency calls only */
    PLMN_EMERGENCY_ONLY,          /* MM in some state, but with emergency only */
    /** No PLMN is available. The MS is unable to make or receive calls */
    PLMN_NO_SERVICE,              /* MM in NULL state */
    /** (G)(E)MM has attempted to location update in the network which has been
     * unsuccessful because it has been unable to get or keep a dedicated
     * signalling channel.(G)(E)MM will re-try after a timeout. If (G)(E)MM has
     * re-tried location updating for the last time, MM will issue an
     * MmrNRegInd with a status value other than PLMN_ACCESS_DIFFICULTY.
     * The application software may wait until it has RECEIVED an MmrRegInd
     * with a status value other than PLMN_ACCESS_DIFFICULTY before attempting
     * to try registration to another PLMN, or it may try registration to
     * another PLMN as soon as it receives the PLMN_ACCESS_DIFFICULTY status.
     * In the latter case, the previous registration attempt will be aborted.
     */
    PLMN_ACCESS_DIFFICULTY,       /* normal; could be in updated or not updated
                                     Internal to GMM.
                                  */
    /** The MS is forbidden access to the PLMN. The application software must
     * store the FPLMN on the SIM if it is performing automatic PLMN selection.
     * The MS is able to make emergency calls only.
     */
    PLMN_FORBIDDEN,               /* emergency; MM in limited service state */
    /** The MS is forbidden access to the PLMN in this location area (or tracking area
     * if operating in an LTE NetworkMode)
     * The MS is able to make emergency calls only.
     */
    PLMN_LA_FORBIDDEN,            /* emergency; MM in limited service state */

    PLMN_LA_FORBIDDEN_NO_SUITABLE_CELL,            /* emergency; MM in limited service state */

    /** National roaming is forbidden; The MS is able to make emergency calls only */
    PLMN_NAT_ROAM_FORBIDDEN,      /* emergency; MM in limited service state */
    /** CS service is not allowed until the MS is powered off/on
     * or the SIM has been removed/inserted. The MS is able to make emergency calls only.
     * In a GPRS enabled MS the serviceType field in the MmrNregInd indicates which
     * services are no longer available and MS may be allowed to continue with GPRS service.
     * If operating in LTE mode  - for EPS services - no further PLMN access is allowed until
     * power off or new USIM inserted.
     */
    PLMN_ILLEGAL_MS,              /* emergency; MM in limited service state and
                                     no further PLMN access allowed until power
                                     off or new SIM inserted */
    /** as PLMN_ILLEGAL_MS */
    PLMN_ILLEGAL_ME,
    /** as PLMN_ILLEGAL_MS for all except combined EPS sessions (with IMSI attach for SMS over SGs)
    * in that case only the non-EPS services are forbidden until MS is powered off/on or new USIM
    * is inserted.
    */
    PLMN_IMSI_UNKNOWN_IN_HLR,
    /** The application software should not attempt to register to any
     * PLMN until the MS is powered off/on, or a SIM has been removed/inserted; The
     * MS is able to make emergency calls only.
     */
    PLMN_AUTHENTICATION_FAILURE,  /* as PLMN_ILLEGAL_MS */
    /** Packet switched service (GPRS or EPS) is currently unavailable.The application may attempt to
     * re-attach to this service.
     */
    PLMN_GPRS_FAILED,             /* GPRS or EPS "emergency" */
    /**Packet switched (GPRS or EPS) service is not allowed.The application software should not attempt
     * to access these services until the MS is powered off/on, or a SIM has been removed/inserted.
     */
    PLMN_GPRS_NOT_ALLOWED,        /* GPRS (EPS) service not allowed - SIM (USIM) is invalid for this service */
    /** The MS is forbidden access to the requested packet switched service (GPRS or EPS) on this PLMN.The application
     * software must store the FPLMN in a list of forbidden PLMNs for this service.
     */
    PLMN_GPRS_FORBIDDEN           /* packet switched service not allowed in PLMN */
    /** if a registration failure is because of an ESM failure as part of the EPS attach procedure that
    * will be indicated by this ServiceStatus.
    */
    ,UE_ESM_FAILURE,
     ATTACH_INCOMPLETE
}
ServiceStatus;

/** UE/NW preference for CIoT EPS optimization */
typedef enum CiotPreferenceTag
{
    IOT_NO_PREFERENCE   = 0,    /* No preference */
    IOT_CP_PREFERENCE   = 1,    /* Preference for control plane CIoT EPS optimization */
    IOT_UP_PREFERENCE   = 2     /* Preference for user plane CIoT EPS optimization */
}CiotPreference;

/** UE/NW capability for CIoT EPS feature */
typedef enum SupportedIotOptionsTag
{
    IOT_NOT_SUPPORTED   = 0,    /* No support */
    IOT_CP_SUPPORTED    = 1,    /* Support for control plane CIoT EPS optimization */
    IOT_UP_SUPPORTED    = 2,    /* Support for user plane CIoT EPS optimization */
    IOT_BOTH_SUPPORTED  = 3     /* Support for both control plane CIoT EPS optimization and user plane CIoT EPS optimization */
}SupportedIotOptions;

typedef struct EdrxInfoTag
{
    Boolean userEdrxSupport;
    Int8    userEdrxValue;
    Boolean nwEdrxSupport;
    Int8    nwEdrxValue;
    Int8    pagingTimeWindow;
    Boolean userPtwPresent;
    Int8    userPtw;
}
EdrxInfo;

typedef struct PsmInfoTag
{
    /* Active timer (T3324) value. This is in binary format,
           for example as 0010 0110 including timer base value and multiplier */
    Int8    activeTime;
    /* Extended periodic TAU(T3412) value. This is in binary format,
           for example as 0010 0110 including timer base value and multiplier */
    Int8    periodicTau;
}
PsmInfo;

typedef struct PsmResponseInfoTag
{
    /* If this value is set then both active timer(T3324) and periodic TAU(T3412) are taken into use.
       If this is off, both activeTime and periodicTau is off. */
    Boolean    requestedPsmSupport;
    /* PSM values that has been requested from network. This is in binary format. */
    PsmInfo    requestedPsmValues;
    /* PSM values that has been received from network. This is in binary format. */
    PsmInfo    nwPsmValues;
}
PsmResponseInfo;

typedef struct PsmRequestInfoTag
{
    /* Take default PSM values into use. */
    Boolean resetToDefaults;
    /* If this value is set then both active timer(T3324) and periodic TAU(T3412) are taken into use.
       If this is off, both activeTime and periodicTau is off. */
    Boolean psmSupport;
    Boolean activeTimePresent;
    Boolean periodicTauPresent;
    PsmInfo psmValues;
}
PsmRequestInfo;

 /** PSM values */
typedef enum PsmStatusTag
{
    PSM_STARTED,    /* PSM state has been entered */
    PSM_START_SEARCHING,  /* Cell search needs to be started to be able to wake up from PSM */
    PSM_ENDED /* PSM is ended and NSPS can be entered if needed  */
}
PsmStatus;

#ifdef FEA_NFM
 /** NFM active request */
typedef struct WriteNfmReqTag
{
    Boolean nfmActivePresent;
    Boolean nfmActive;
    Boolean startTimerActivePresent;
    Boolean startTimerActive;
    Boolean stParPresent;
    Int16   stPar;
    Boolean stTmPresent;
    uint8_t  stTm;       /*for config test mode*/
}
WriteNfmReq;

/** NFM configuration request */
typedef struct WriteNfmConfReqTag
{
    Boolean nfmParPresent[MAX_NFM_PAR_VALUE];
    Int16 nfmPar[MAX_NFM_PAR_VALUE];
    Boolean stParPresent;
    Int16 stPar;
}
WriteNfmConfReq;

/** NFM active response */
typedef struct ReadNfmCnfTag
{
    Boolean nfmActive;
    Boolean startTimerActive;
}
ReadNfmCnf;

/** NFM configuration response */
typedef struct ReadNfmConfCnfTag
{
    Int16 nfmPar[MAX_NFM_PAR_VALUE];
    Int16 stPar;
}
ReadNfmConfCnf;

/** NFM status response */
typedef struct ReadNfmStatusCnfTag
{
    Int16 remStartTimerValue;       /* Remaining start timer value in seconds */
    Int16 remBackOffTimerValue;     /* Remaining back-off timer value in seconds */
}
ReadNfmStatusCnf;
#endif /* FEA_NFM */


/** Search ind parameters */
typedef enum SearchIndStatusTag
{
    ABMM_NO_ACTIONS,    /* Init value */
    ABMM_START_SEARCH   /* PLMN search/Cell selection needed */
}
SearchIndStatus;

/** MM/RR engineering info enabled/disabled data */
typedef struct EngineeringDataTag
{
#if defined(UPGRADE_TWGGE_NRW)
    /* FR9805-2463: MM/RR engineering info enabled/disabled data */
    /** Defined NAS test mode */
    Int32   nasTestMode;
    /** Defined AS test mode */
    Int32   accStratumTestMode;
    /** Defined UMTS engineering mode using bit flags which are
     * defined in upscode\urrmtcem.h
     */
    Int32   umtsEngMode;
    /** Whether or not MM test mode is active */
    Boolean mmTestModeActive;
    /** Whether or not RR test mode is active dedicated mode */
    Boolean rrDedTestModeActive;
    /** Whether or not test mode is active in idle mode */
    Boolean rrIdleTestModeActive;
    /** Whether or not GPRS engineering test mode is active */
    Boolean gprsEngTestModeActive;
    /** Whether or not SM engineering test mode is enabled */
    Boolean smTestModeActive;
    /** Whether or not UMTS engineering mode is active */
    Boolean umtsEngModeActive;
#else
    /* FR9805-2463: MM/RR engineering info enabled/disabled data */
# if defined (UPGRADE_ADDIT_ENG_INFO)
    /** Defined NAS test mode */
    Int32   nasTestMode;
    /** Defined AS test mode */
    Int32   accStratumTestMode;
#  if defined (UPGRADE_3G)
    /** Defined UMTS engineering mode using bit flags which are
     * defined in upscode\urrmtcem.h
     */
    Int32   umtsEngMode;
#  endif /* (UPGRADE_3G) */
# else /* !(UPGRADE_ADDIT_ENG_INFO) */
    /** Whether or not MM test mode is active */
    Boolean mmTestModeActive;
    /** Whether or not RR test mode is active dedicated mode */
    Boolean rrDedTestModeActive;
    /** Whether or not test mode is active in idle mode */
    Boolean rrIdleTestModeActive;
    /** Whether or not GPRS engineering test mode is active */
    Boolean gprsEngTestModeActive;
    /** Whether or not SM engineering test mode is enabled */
    Boolean smTestModeActive;
#  if defined (UPGRADE_3G)
    /** Whether or not UMTS engineering mode is active */
    Boolean umtsEngModeActive;
#  endif /* (UPGRADE_3G) */
# endif /* !(UPGRADE_ADDIT_ENG_INFO) */
#endif
}
EngineeringData;

/** Contains information about the MS capabilities which
 * the protocol software needs to know
 */
typedef struct MobileEquipmentDataTag
{
    /** The aspects of MS;
     * mobiles when communicating to the network will initially indicate their classmark,
     * this includes service, equipment and radio capabilities
     */
    MsClassmarkElement      classmark;
    /** Traffic channel types supported */
    MsCapability            msCapability;
    /** The International Mobile Equipment Identity is a unique identifier allocated to each ME*/
    MobileIdElement         imei_old;
    /** Allows the two digit software version number to be added to the standard
     * IMEI for transmission to the network when IMEISV is requested
     */
    Svn                     svn_old;

    /** MS network capability for UE supporting GSM/UMTS - To be removed */
    MsNetworkCapability     msNetworkCapability;
    /** Specifies the service type that is preferred */
    ServiceType             preferredServiceType;

    /** EPS capabilities for UE network capability IE(24.301 9.9.3.34) */
    EpsUeNetworkCapability   epsUeNetworkCapability;
    /*Serial number*/
    SernNumber               SnId_old;
}
MobileEquipmentData;

/** Contains information about mobile id info **/
typedef struct MobileIDTag
{
    /** The International Mobile Equipment Identity is a unique identifier allocated to each ME*/
    MobileIdElement         imei;

    /*Serial number*/
    SernNumber              SnId;
    Svn                     svn;
}
MobileID;


/** Contains information about the MS capabilities which
 * the protocol software needs to know
 */
typedef struct MmrMeDataTag
{
    /** The aspects of MS;
     * mobiles when communicating to the network will initially indicate their classmark,
     * this includes service, equipment and radio capabilities
     */
    MsClassmarkElement      classmark;
    /** Traffic channel types supported */
    MsCapability            msCapability;
    /** The International Mobile Equipment Identity is a unique identifier allocated to each ME*/
    MobileIdElement         imei;
    /** Allows the two digit software version number to be added to the standard
     * IMEI for transmission to the network when IMEISV is requested
     */
    Svn                     svn;

    /** MS network capability for UE supporting GSM/UMTS - To be removed */
    MsNetworkCapability     msNetworkCapability;
    /** Specifies the service type that is preferred */
    ServiceType             preferredServiceType;

    /** EPS capabilities for UE network capability IE(24.301 9.9.3.34) */
    EpsUeNetworkCapability   epsUeNetworkCapability;
    /*Serial number*/
    SernNumber               SnId;
}
MmrMeData;

/** This is used as state information when processing equivalent PLMN lists */
typedef enum EquivalentPlmnListStatusTag
{
    EPLMN_NOT_PRESENT, /**< No ePLMNs available - may have all been forbidden or no list received */
    EPLMN_INIT,       /**< Stored list received from ABMM */
    EPLMN_RECEIVED,   /**< New list received - need to send it to ABMM */
    EPLMN_PROCESSING, /**< Waiting for list to be checked for forbidden PLMNs by ABMM */
    EPLMN_VALID       /**< List valid; PLMNs checked and numPlmn > 0 */
}
EquivalentPlmnListStatus;

/** This is when timer expiry information is needed by EMM or ABMM */
typedef enum TimerExpiryIdTag
{
    NO_TIMER_INFO,  /* Initial value */
    T3245,          /* Timer T3245 expired */
    EMM_T3247,      /* Timer T3247 expired */
    EMM_NFM_START_TIMER, /* NFM start timer expired */
    EMM_NFM_BACK_OFF_TIMER /* NFM back off timer expired */
}
TimerExpiryId;

/** Type of registration EMM is doing */
typedef enum RegisterTypeTag
{
  REG_TYPE_EPS_ATTACH,
  REG_TYPE_COMB_EPS_IMSI_ATTACH,
  REG_TYPE_TA_UPDATING,
  REG_TYPE_COMB_TA_LA_UPDATING,
  REG_TYPE_COMB_TA_LA_UPDATING_WITH_IMSI_ATTACH,
  REG_TYPE_PERIODIC_UPDATING,
  REG_TYPE_NOT_ATTACH_OR_TAU
}
RegisterType;

/****************************************************************************
 * Signal types
 ****************************************************************************/
/** Sent from ABMM to UMM at power on to send mobile equipment specific
 * data normally stored in NVRAM down the stack
 * in LTE mode this information is sent to EMM
 */
typedef struct MmrMeDataReqTag
{
    /** Holds ME data */
    MmrMeData meData;
}
MmrMeDataReq;

/** Sent from ABMM to UMM to request that the ME registers on a PLMN.
 * pdpActivation is used to inform UMM whether any SM signalling is to
 * be performed with the network after a GPRS Attach, so UMM can set the
 * Follow On Request in the Attach Request.
 * For LTE operation this signal is received in EMM and indicates if a combined
 * attach - EPS and IMSI - should be performed so that both EPS services and SMS
 * over SGs are supported orif only an EPS attach - for packet switched services
 - is required.
 */
typedef struct MmrRegReqTag
{
    /** PLMN to be camped on */
    Plmn                requested_plmn;

    /** Search type */
    EutraSearchType     search_type;

    /** Number of frequency bands to be searched
\assoc ARRAY \ref band */
    Int8                band_num;

    /** List of frequency bands which ERRC should
      * scan through first in deep search */
    EutraBand           band[MAX_SUPPORTED_NBIOT_BAND_NUM];
    /** Frequency lock presence
\assoc PRESENT \ref lockInfo */
    Boolean             lockInfo_present;
    /** Frequency lock info(, structure declared in as_nas_common_typ.h) */
    CellLockInfo        lockInfo;
    /** Required service.
     *For LTE operation, if SMS over SGs is required this will set to
     * COMBINED_SERVICES. If only single mode LTE is required this will be set to GPRS_SERVICE.
     */
    ServiceType         serviceType;
    /** Current PDN connection setting during attach
      * If TRUE attach is done without PDN connection */
    Boolean             attachWithoutPdn;

    /** Current whether manual plmn select 
      * If TRUE, plmn select was Manual plmn */
    Boolean               isManualPlmnSelect;

    /** Current RPM status
      * If TRUE RPM enable, FALSE RPM feature disable */
    Boolean             rpmEnable;
}
MmrRegReq;

/** Sent from ABMM to UMM to request detachment from one or more services.
 * Request to de-register from the network. Must be issued to (G)MM whenever
 * the SIM is removed or when the power to the MS is about to be removed.
 *
 * The application layer must ensure that all CM connections have been released
 *  before issuing the de-registration request. (G)MM may have to issue an IMSI_DETACH
 * message to the network, which may take some time. We advise that the power is not
 * removed until 1 second has elapsed since issuing the MmrNregReq.
 * for LTE builds this signal may be received and processed by (U)(G)(E)MM depending upon the
 * current network mode
 */

typedef struct MmrNregReqTag
{
    /** Whether or not the SIM is present */
    Boolean     simPresent;
    /** EMM_TO_BE_DONE_OPTIONAL: Whether or no t he state is silent shutdown */
    Boolean     silentDetach;
    /** Whether or not power is down */
    Boolean     powerDown;
    /** The type of service provided by the network*/
    /* For LTE this may be for EPS services or non-EPS services (SMS over SGs) or both.*/
    ServiceType serviceType;
    /** Reason for deactivation is NSPS/OOS */
    Boolean     reasonIsNsps;
}
MmrNregReq;

/** Confirmation that the MS has de-registered from the network for the
 * indicated service.
 * Sent from UMM to ABMM to confirm detachment from one or more services.
 * for LTE builds this signal may be sent by (U)(G)(E)MM depending upon the
 * current network mode
 */
typedef struct MmrNregCnfTag
{
    /** Service that has been de-registered */
    ServiceType serviceType;
    EdrxInfo    edrx;
    PsmInfo     nwPsm;
}
MmrNregCnf;

/** Sent from UMM to ABMM to indicate loss of one or more services on a PLMN
 * for LTE builds this signal may be sent by (U)(G)(E)MM depending upon the
 * current network mode
 * (given by serviceType). Can be a response to MmrRegReq if a requested service
 * could not be obtained, or sent at any time if a previously available service
 * is lost. The equivalent PLMN list is included so that ABMM can check if the
 * list has been deleted due to network reject.
 */
typedef struct MmrNregIndTag
{
    /** Why service(s) lost */
    ServiceStatus            status;
    /** Selected LAI */
    /** for LTE modes this is selected TAI */
    Lai                      selectedLocationArea;
    /** Defines the cell identity. */
    EutraAsn_CellIdentity    epsCellId;
    /** Service available on cell */
    ServiceType              serviceType;
    /** The status of equivalent PLMN (Included here in case list
      * deleted due to a reject) */
    EquivalentPlmnListStatus ePlmnListStatus;
    /** List of equivalent PLMNs */
    EquivalentPlmnList       ePlmnList;
    /** reattach as part of MT DETACH and REATTACH */
    Boolean                  pendingReattach;
    /** Whether MS detached by network */
    /* or if in LTE Mode then indicates network detached EPS services.
      */
    Boolean                  gprsMtDetach;
    EdrxInfo                 edrx;
    /* EMM cause is added to this signal, because ABMM needs it for
      * the timer T3245 handling */
    EmmCause                 emmCause;
    ExtendedEmmCauseElement  extEmmCause;
    PsmInfo                  nwPsm;
    Boolean                  integrityProtected;
    /** EMM registration type */
    RegisterType             emmRegType;
#ifdef FEA_NFM
    Int16                    remainingNfmStartTimerValue;
    Int16                    remainingNfmBackOffTimerValue;
    Boolean                  nfmActive;
#endif /* FEA_NFM */
}
MmrNregInd;

/** Sent from UMM to ABMM to indicate that the ME has registered on a PLMN.
 * - in LTE mode this is sent from EMM -
 * MmrRegInd is used to indicate cell reselection.
 */
typedef struct MmrRegXxxTag
{
    /** TAI that the MS is registered in. If the UE is both EPS and IMSI attached
    * the IMSI attach location information is containted in imsiAttachLai
    */
    Lai                         selectedTrackingArea;
    /** Defines the cell identity. */
    EutraAsn_CellIdentity       cellIdentity;
    /** Tells if the PLMN supports attach without PDN connectivity */
    Boolean                     attach_without_pdn_connectivity_supported;
    /** Frequency band of camped on cell */
    EutraBand                   band;
    /** Service available on cell */
    ServiceType                 serviceType;
    /** defines the selected core network operator -
     * from the GUTI */
    Plmn                        coreNetworkId;
    /** Defines if there is IMSI attach information
    \assoc PRESENT \ref imsiAttachLai */
    Boolean                     imsiAttachLaiValid;
    /** defined the IMSI attach LAI */
    Lai                         imsiAttachLai;
    /** Tracking area Identity List */
    TaiList                     taiList;
    /** reattach as part of MT DETACH and REATTACH */
    Boolean                     reattached;
    /** Additional update result (e.g. SMS only) we received in Attach Accept
     * for a combined attach procedure
     */
    AdditionalUpdateResult      additionalUpdateResult;
    /** Indicates the satatus of equivalent PLMN */
    EquivalentPlmnListStatus    ePlmnListStatus;
    /** The list of equivalent PLMNs */
    EquivalentPlmnList          ePlmnList;
    /** Edrx */
    EdrxInfo                    edrx;
    /** PSM data */
    PsmInfo                     nwPsm;
    /** Eps Network Feature support IE data */
    EpsNetworkFeatureSupport    epsNwFeatureSupport;
    /**  Status of availability for SMS services requested by the UE during Attach or TAU */
     Boolean                    smsServicesStatusPresent;
     SMSServicesStatusElement   smsServicesStatus;
    /** EMM registration type */
    RegisterType                emmRegType;
#ifdef FEA_NFM
    Int16                       remainingNfmBackOffTimerValue;
    Boolean                     nfmActive;
#endif /* FEA_NFM */
}
MmrRegInd,
/** Sent from UMM to ABMM to indicate that the ME has registered on a PLMN.
 * MmrRegCnf is sent in response to MmrRegReq.
 */
MmrRegCnf;


/** Sent from ABMM to EMM when user wants to change EDRX value by using
 * AT commands.
 */
typedef struct MmrSetEdrxReqTag
{
    Boolean resetToDefaults;
    Boolean userEdrxSupport;
    Int8    userEdrxValue;
    Boolean userPtwPresent;
    Int8    userPtw;
}
MmrSetEdrxReq;

/** Sent from ABMM to EMM when user wants to know EDRX support by using AT
 * commands.
 */
typedef struct MmrReadEdrxReqTag
{
    Int8 empty;
}
MmrReadEdrxReq;

/** Sent from ABMM to EMM. With this message ABMM informs EMM about timer expiry.
 */
typedef struct MmrAbmmTimerExpiryTag
{
    TimerExpiryId timerId;
}
MmrAbmmTimerExpiry;

/** Sent from EMM to ABMM after user has requested EDRX values by using
 * AT commands.
 */
typedef struct MmrReadEdrxCnfTag
{
    EdrxInfo edrx;
}
MmrReadEdrxCnf;

/** Sent from EMM to ABMM after user has requested setting of EDRX values by using
 * AT commands.
 */
typedef struct MmrSetEdrxCnfTag
{
    Boolean success;
}
MmrSetEdrxCnf;

/** With this signal, ATCI requests EMM via ABMM to alter CIoT EPS feature. */
typedef struct MmrWriteIotOptCfgReqTag
{
    /** If set IoT optimization values are set to default values */
    Boolean             resetToDefault;
    /** Presence value for ciotPreference. If field value is FALSE then
     *  ciotPreference parameter is not used. */
    Boolean             ciotPreferenceValid;
    /** UE preference for IoT optimozation */
    CiotPreference      ciotPreference;
    /** Precense value for supportedUeOptions parameter.
      * If FALSE then supportedUeOptions is not used. */
    Boolean             supportedUeOptionsValid;
    /* UE support for IoT optimozation */
    SupportedIotOptions supportedUeOptions;
}
MmrWriteIotOptCfgReq;

/** Sent from EMM to ABMM after user has requested setting of CIoT values by using
 * AT commands.
 */
typedef struct MmrWriteIotOptCfgCnfTag
{
    Boolean success;
}
MmrWriteIotOptCfgCnf;


/** With this signal ATCI requests UE CIoT EPS values from EMM via ABMM.*/
typedef struct MmrReadIotOptCfgReqTag
{
    /** Requesting task ID. */
    Int8          empty;
}
MmrReadIotOptCfgReq;

/** With this signal ABMM sends UE CIoT EPS optimization values to ATCI */
typedef struct MmrReadIotOptCfgCnfTag
{
    /** UE preferred optimization */
    CiotPreference      ciotPreference;
    /** UE capability for CIoT EPS feature. */
    SupportedIotOptions supportedUeOptions;
}
MmrReadIotOptCfgCnf;

/** Sent from ABMM to EMM when user wants to read PSM value by using
 * AT commands.
 */
typedef struct MmrReadPsmConfReqTag
{
    Int8 empty;
}
MmrReadPsmConfReq;

/** Sent from EMM to ABMM after user has requested to read PSM values by using
 * AT commands.
 */
typedef struct MmrReadPsmConfCnfTag
{
    PsmResponseInfo psm;
}
MmrReadPsmConfCnf;

typedef enum UeInfoTypeTag
{
  UE_INFO_TYPE_T3412,
  /* add other information type here */
  UE_INFO_TYPE_INVALID
}
UeInfoType;

typedef union UeInfoBodyTag
{
  /* periodic TAU(T3412) value, the unit is seconds, such as 60s */
  Int32  t3412;
  /* add other information body here */
}
UeInfoBody;

/** Sent from ABMM to EMM when user wants to read UE information*/
typedef struct MmrReadUeInfoReqTag
{
    UeInfoType ueInfoType;
}
MmrReadUeInfoReq;

/** Sent from EMM to ABMM after user has requested to read UE information
 */
typedef struct MmrReadUeInfoCnfTag
{
 /** The ueInfoType
  \assoc UNION \ref ueInfoBody */
  UeInfoType ueInfoType;
  UeInfoBody ueInfoBody;
}
MmrReadUeInfoCnf;


/** Sent from EMM to ABMM after user has requested setting of PSM values by using
 * AT commands.
 */
typedef struct MmrWritePsmConfReqTag
{
    PsmRequestInfo psm;
}
MmrWritePsmConfReq;

/** Sent from EMM to ABMM after user has requested setting of PSM values by using
 * AT commands.
 */
typedef struct MmrWritePsmConfCnfTag
{
    Boolean success;
}
MmrWritePsmConfCnf;

/** Sent from EMM to ABMM to inform when PSM status changes
 */
typedef struct MmrPsmStatusIndTag
{
    PsmStatus psmStatus;
}
MmrPsmStatusInd;

#ifdef FEA_NFM
/** Sent from ABMM to EMM when user wants to write NFM values by using
 * AT commands.
 */
typedef struct MmrWriteNfmReqTag
{
    WriteNfmReq writeNfmReq;
}
MmrWriteNfmReq;

/** Sent from EMM to ABMM after user has requested to write NFM values by using
 * AT commands.
 */
typedef struct MmrWriteNfmCnfTag
{
    Boolean success;
}
MmrWriteNfmCnf;

/** Sent from ABMM to EMM when user wants to write NFM configuration by using
 * AT commands.
 */
typedef struct MmrWriteNfmConfReqTag
{
    WriteNfmConfReq writeNfmConfReq;
}
MmrWriteNfmConfReq;

/** Sent from EMM to ABMM after user has requested to write NFM configuration by using
 * AT commands.
 */
typedef struct MmrWriteNfmConfCnfTag
{
    Boolean success;
}
MmrWriteNfmConfCnf;


/** Sent from ABMM to EMM when user wants to read NFM values by using
 * AT commands.
 */
typedef struct MmrReadNfmReqTag
{
    Int8 empty;
}
MmrReadNfmReq;

/** Sent from EMM to ABMM after user has requested to read NFM values by using
 * AT commands.
 */
typedef struct MmrReadNfmCnfTag
{
    ReadNfmCnf readNfmCnf;
}
MmrReadNfmCnf;

/** Sent from ABMM to EMM when user wants to read current NFM config by using
 * AT commands.
 */
typedef struct MmrReadNfmConfReqTag
{
    Int8 empty;
}
MmrReadNfmConfReq;

/** Sent from EMM to ABMM after user has requested to read NFM configuration by using
 * AT commands.
 */
typedef struct MmrReadNfmConfCnfTag
{
    ReadNfmConfCnf readNfmConfCnf;
}
MmrReadNfmConfCnf;

/** Sent from ABMM to EMM when user wants to read status of NFM timers  */
typedef struct MmrReadNfmStatusReqTag
{
    Int8 empty;
}
MmrReadNfmStatusReq;

/** Sent from EMM to ABMM to inform status of NFM timers */
typedef struct MmrReadNfmStatusCnfTag
{
    ReadNfmStatusCnf readNfmStatusCnf;
}
MmrReadNfmStatusCnf;

#endif /* FEA_NFM */

/** Sent from EMM to ABMM to indicate that PLMN search/cell celect is needed.
 */
typedef struct MmrSearchIndTag
{
    SearchIndStatus searchStatus;
}
MmrSearchInd;

/** Sent from EMM to ABMM to inform specific timer expiration
 */
typedef struct MmrEmmTimerExpiryIndTag
{
    TimerExpiryId  timerId;
}
MmrEmmTimerExpiryInd;

/** This signal is used to pass a processed list of equivalent PLMNs to (G)MM.
 * Sent from ABMM to UMM and contains equivalent PLMN list with forbidden PLMNs removed.
 */
typedef struct MmrEquivPlmnListReqTag
{
    /** LAI on which list received */
    /** TAI if in LTE mode */
    Lai                lai;
    /** Updated list of equivalent PLMNs */
    EquivalentPlmnList ePlmnList;
}
MmrEquivPlmnListReq;

# if defined (ENABLE_EHPLMN)
/** This signal is used to pass a processed list of EHPLMNs to (G)MM.
 * Sent from ABMM to UMM and contains EHPLMN list.
 */
typedef struct MmrEhplmnListReqTag
{
    /** Indicates number of PLMN in the list
\assoc ARRAY \ref plmnList */
    Int8                numPlmnList;
    /** Lists EHPLMN */
    EquivalentPlmn      plmnList[MAX_EHPLMNS];   /* EHPLMN list if exists, or Acting PLMN if exist, or HPLMN from SIM */
}
MmrEhplmnListReq;
# endif /* (ENABLE_EHPLMN) */

/* 9802-1981 */
/** Sent from UMM to ABMM when MM engineering mode activated to
 * transfer information about the cell.
 * This signal is sent on change of cell when engineering mode operation in (G)MM is activated.
 * It indicates the Location Area ID and Cell Id of the serving cell.
 */
typedef struct MmrInfoIndTag
{
    /** Used to indicate No Service, Emergency Service, Normal service, etc */
    ServiceStatus      serviceStatus;
    /** LAI of the serving cell */
    /** TAI if in LTE mode */
    Lai                lai;
    /** ID of the serving cell */
    UCellIdentity      cellId;
    /** Band information */
    Band               band;
    /** Band mode bitmap indicating supported bands */
    BandMode           bandMode;
    /** Active RAT (GSM, FDD or TDD) */
    /* or LTE TD or LTE FDD.*/
    NetworkModes       networkMode;
    /** Timeout value in minute */
    Int16              t3212inMinutes;
    /** Whether or not IMSI attach detach is allowed */
    Boolean            attachDetachAllowed;
    /** TMSI of the current PLMN */
    Tmsi               tmsi;
    /** Indicates the state of MM */
    MmState            mmState;
    /** The type of service provided by the serving cell */
    ServiceType        serviceType;
    /** Cell information of GPRS */
    GprsCellInfo       gprsCellInfo;
    /** Current PTMSI */
    Int32              pTmsi;
    /** T3312 in minutes */
    Int16              t3312inMinutes;
    /** The state of GMM */
    GmmState           gmmState;
# if defined (UPGRADE_GGE)
    /** Current TLLI;
     * the Temporary Logical Link Identifier (TLLI) provides a signalling address for
     * communication between the MS (Mobile Station) and the SGSN (Serving GPRS Support Node)
     */
    Int32              tlli;
    /** The status of GPRS (Idle, Standby or Ready) */
    GprsReadyState     gprsReadyState;
    /** T3314 in seconds */
    Int16              readyTimerValueInSecs;
# endif /* (UPGRADE_GGE) */
    /** Whether or not ePlmnList contains valid information */
    Boolean            ePlmnListValid;
    /** Holds the list of equivalent PLMN */
    EquivalentPlmnList ePlmnList;
}
MmrInfoInd;

/**  Returned in response to an MmrPlmnListReq signal.
 * This signal contains information on PLMNs found up to and
 * including the PLMN specified in the MmrPlmnListReq and is
 * sent when the specified PLMN is found during a search.
 * If MM is currently attempting to register to a PLMN then
 * mmRetryPlmnPresent will be TRUE and the PLMN will be given
 * in mmRetryPlmn. ABMM should not attempt to register to this
 * PLMN, but should allow the MM procedure to continue. ABMM may
 * attempt to register to any higher priority PLMNs that are in list.
 */
typedef struct MmrPlmnListIndTag
{
    /** List of PLMN found during a search */
    PlmnList       list;
    /** Whether mmRetryPlmn valid (set if MM timer is running in
     * order to retry registration)

\assoc PRESENT \ref mmRetryPlmn */
    Boolean        mmRetryPlmnPresent;
    /** Identifies PLMN that MM is trying to register on */
    Lai            mmRetryPlmn;                      /* use LAI so know length of MNC */
}
MmrPlmnListInd;
/** Returned in response to an MmrPlmnListReq signal.
 * If status indicates success then list contains all PLMNs found during the search.
 * mmRetryPlmnPresent and mmRetryPlmn are as descibed for MmrPlmnListInd.
 */
typedef struct MmrPlmnListCnfTag
{
    /** PLMN search result
     */
    PlmnSearchResult    result;
    /** List of PLMN found during a search */
    PlmnList            list;
    /** Whether mmRetryPlmn valid (set if MM timer is running in
     * order to retry registration)

\assoc PRESENT \ref mmRetryPlmn */
    Boolean             mmRetryPlmnPresent;
    /** Identifies PLMN that MM is trying to register on */
    Lai                 mmRetryPlmn; /* use LAI so know length of MNC */
}
MmrPlmnListCnf;

/** Used to indicate error in UMM to ABMM.
 * Sent if the application layer issues an MmrRegReq, MmrNregReq,
 * MmrCampReq or an MmrPlmnListReq while (G)MM is in a state such
 * that the request cannot be processed.
 */
typedef struct MmrErrorIndTag
{
    /** The cause of sending error indication */
    MmErrorCause mmErrorCause;
    /** The state EMM was in when it received a request that it could not service */
    EmmState     emmState;
    /** Information on the current mode of operation */
    BandMode     bandMode;
}
MmrErrorInd;

#if defined (ENABLE_L23_DEBUG)

/** Used to indicate EMM state transition */
typedef struct EmmStateDebugIndTag
{
  /**EMM old state */
  EmmState         oldState;
  /**EMM new state */
  EmmState         newState;
}
EmmStateDebugInd;
#endif /*ENABLE_L23_DEBUG*/

/** Request for a list of the available PLMNs.
 * Sent from ABMM to UMM to request list of visible PLMNs.
 * sent to EMM if in LTE mode
 * If a valid PLMN is included in the signal then the search should
 * stop as soon as this PLMN has been found.
 */
typedef struct MmrPlmnListReqTag
{
    /** Requested PLMN presence.
\assoc PRESENT \ref requestedPlmn */
    Boolean             requestedPlmnPresent;

    /** PLMN which EMM wants to find */
    Plmn                requestedPlmn;

    /** Search type presence
\assoc PRESENT \ref search_type */
    Boolean             search_type_present;

    /** Type of search of requested PLMN */
    EutraSearchType     search_type;

    /** wether high priority PLMN searching.
\TRUE,trigger high priority plmn search,  otherwise, it is not HPPLMN search*/
    Boolean             isHpplmnSearch;

    /** Number of frequency bands to be scanned.
\assoc ARRAY \ref band */
    Int8                band_num;

    /** List of frequency bands which ERRC should
        scan through first in deep search. */
    EutraBand           band[MAX_SUPPORTED_NBIOT_BAND_NUM];

    /** Frequency lock presence
\assoc PRESENT \ref lockInfo */
    Boolean             lockInfo_present;
    /** Frequency lock info(, structure declared in as_nas_common_typ.h) */
    CellLockInfo        lockInfo;
}
MmrPlmnListReq;

/* This signal is used to indicate EMM connection status to ABMM */
typedef struct MmrCsconIndTag
{
    /** The cirrent EMM connection status */
    Boolean             connectionStatus;
}
MmrCsconInd;

/* Following signal is used to request cell information from NAS/AS */
typedef struct MmrAsNasInfoReqTag
{
    /** Request type */
    RequestedModemInfo      requestedInfo;
}MmrAsNasInfoReq;

/* Following signal is used to forward cell information from NAS/AS to ABMM/ATCI */
typedef struct MmrAsNasInfoCnfTag
{
    /** \assoc PRESENT \ref cellInfo */
    bool                    cellInfoPresent;
    /** Serving Cell info from AS */
    ServingCellRadioInfo    cellInfo;

    /** \assoc ARRAY \ref neighbourCellRadioInfo */
    Int8                    neighbourCellRadioInfoNum;
    /** Neighbour Cell info from AAS */
    NeighbourCellRadioInfo  neighbourCellRadioInfo[NCELL_RADIO_INFO_MAX_NUM];

    /** \assoc PRESENT \ref dataTransferInfo */
    bool                                dataTransferInfoPresent;
    /** Error rate and throughput parameters */
    DataTransferInfo                    dataTransferInfo;

    /** ERRC state info */
    ErrcStateEngInfo                    errcStateEngInfo;
}MmrAsNasInfoCnf;

typedef EmptySignal MmrLocCellInfoReq;

typedef struct MmrLocCellInfoCnfTag
{
    ErrcStateEngInfo                    errcStateEngInfo;

    bool                                servingCellInfoPresent;
    LocationCellInfo                    servingCellInfo;

    uint8_t                             neighbourCellInfoNum;
    LocationCellInfo                    neighbourCellInfo[NCELL_INFO_MAX_NUM];
} MmrLocCellInfoCnf;

/** Sent to UMM to enable or disable GRR and MM engineering data.
 * This signal is a request to enable engineering mode functions.
 * If (G)MM engineering mode is switched on, (G)MM will respond
 * with MmrInfoInd signals whenever the lai, cellId or Band is
 * changed.
*/
typedef struct MmrEngInfoReqTag
{   /* FR9805-2463 */
    /** MM/RR engineering information used to enable/disable data */
    EngineeringData engineeringData;
}
MmrEngInfoReq;

# if defined (UPGRADE_GGE)
/** Pass the status information to AL for the Engineering Display */
typedef struct MmrLlcInfoIndTag
{
    /** The type of LLC status
\assoc UNION \ref info */
    LlcStatusType type;
    /** LLC status information */
    LlcStatusInfo info;
}
MmrLlcInfoInd;
# endif /* (UPGRADE_GGE) */


/** Sent from ABMM to UMM to indicate change in classmark data.
 * This signal should be sent to (G)MM whenever the MS classmark data changes.
 * For example, if the MS is plugged into a booster, the RF power capability will
 * increase. If (G)MM receives an MmrClassmarkReq while a dedicated connection exists,
 * a CLASSMARK message is sent to the network. (G)MM will accept the MmrClassmarkReq
 * signal in any state.
 *
 * Note: This signal is not currently used.
 */
typedef struct MmrClassmarkReqTag
{
    /** Indicates the aspects of MS;
     * mobiles when communicating to the network will initially indicate their classmark,
     * this includes service, equipment and radio capabilities
     */
    MsClassmarkElement msClassmark;
}
MmrClassmarkReq;

/** Sent from (U)(G)(E)MM to ABMM to transfer contents of a Layer 3(G)(E)MM_INFORMATION
 * message from the network.
 * This signal transfers the MM INFORMATION or GMM INFORMATION EMM INFORMATION message to
 * the application layer This message can contain the Network name and time zone.
 */
typedef struct MmInformationMessageTag MmrNetworkInfoInd;

/** MmrCustomConfigReq
 *  BL sends the custom config request to enable/disable
 */typedef struct MccCustomConfigReqTag
{
    Int8 bitMask[MAX_NUM_CUSTOM_COMMAND];
}
MccCustomConfigReq;

/* MmrSimInfoNotifyReq
 * AB sent to EMM with sim status and supported band by operator
 */
typedef struct MmrSimInfoNotifyReqTag
{
    Boolean     simReady;
    Boolean     supportedBandByOperatorPresent;
    Int32       supportedBandByOperator;
}MmrSimInfoNotifyReq;

#if defined (EMM_UNIT_TESTING)

typedef struct EmmUtConfigReqTag
{
#ifdef MTK_NVDM_MODEM_ENABLE
    Boolean             psmSupportPresent;
    Boolean             psmSupport;
    Boolean             activeTimePresent;
    uint32_t            activeTime;
    Boolean             edrxSupportPresent;
    Boolean             edrxSupport;
    Boolean             edrxValuePresent;
    uint32_t            edrxValue;
    Boolean             t3412ValuePresent;
    uint32_t            t3412Value;
    Boolean             psmT3324ValuePresent;
    uint32_t            psmT3324Value;
    Boolean             defaultCpUpPdnPreferencePresent;
    ModemCfg_DEFAULT_CP_UP_PDN_PREFERENCE defaultCpUpPdnPreference;
    Boolean             defaultHcciotSupportPresent;
    Boolean             defaultHcciotSupport;
    Boolean             mdrbSupportPresent;
    Boolean             mdrbSupport;
    Boolean             uplaneSupportPresent;
    Boolean             uplaneSupport;
#else
    Boolean             foo; /* Dummy member to avoid compilation error.
                                                 * Structure currently used only when
                                                 * MTK_NVDM_MODEM_ENABLE is enabled.  */
#endif
}
EmmUtConfigReq;


/* EMM_TO_BE_DONE: These can be used in future

EMM information for unit testing


typedef struct EmmUtInfoTag
{
    Int32               info;
}
EmmUtInfo;


typedef struct EmmUtConfigCnfTag
{
    EmmUtInfo           info;
}
EmmUtConfigCnf;

*/
#endif

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
Plmn abmmGetMostPreferredPlmn ( Mcc requestedMcc );

/** @} */ /* End of SigMmr group */
#endif /* !(MMR_SIG_H) */
/* END OF FILE */

