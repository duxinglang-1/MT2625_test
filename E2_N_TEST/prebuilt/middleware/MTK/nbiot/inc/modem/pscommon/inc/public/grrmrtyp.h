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
 *      types common to mmr_sig.h and rr_sig.h
 **************************************************************************/

#ifndef GRRMRTYP_H
#define GRRMRTYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

# if !defined (SIR_TYP_H)
#include     <sir_typ.h>
# endif /* !(SIR_TYP_H) */

# if !defined (L13_TYP_H)
#include <l13_typ.h>
# endif /* !(L13_TYP_H) */

# if defined (UPGRADE_3G)
#  if !defined (U1_TYP_H)
#include <u1_typ.h>
#  endif /* !(U1_TYP_H) */

#if defined (UPGRADE_LTE)
#include <eps_asn.h>
#include <eps_typ.h>
#endif

#  if !defined (UMPH_TYP_H)
#include <umph_typ.h>
#  endif /* !(UMPH_TYP_H) */
# endif /* UPGRADE_3G */

#include <mpsi_typ.h>

#include <ubnd_cfg.h>

/****************************************************************************
 * Macros
 ****************************************************************************/
#define INVALID_MCC  0xffff
#define INVALID_MNC  0xffff

/* maximum number of PLMNs reported in response to a PLMN list request */
# if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
#define     NUM_PLMNS                   20
# else /* UPGRADE_3G && UPGRADE_GGE */
#define     NUM_PLMNS                   12
# endif /* UPGRADE_3G && UPGRADE_GGE */

#define MAX_NAS_TX_QUEUE_INFO_SIZE     5

# if defined (UPGRADE_ADDIT_ENG_INFO)
/* The engineering mode control now uses a 32-bit control field to turn specific eng. mode information
** on and off. Bits 0 to 7 contain general engineering mode control .... */
#define ENG_INFO_DISABLED               0x00000000
#define ENG_INFO_SELECTIVE_OFF          0x00000001
#define ENG_INFO_CONTINUOUS             0x00000002
#define ENG_INFO_SINGLE_SHOT            0x00000004

/* .. bits 8 - 15 contain state specific control e.g. idle, dedicated or PTM */
#define ENG_INFO_IDLE_STATE             0x00000100
#define ENG_INFO_DED_STATE              0x00000200
#define ENG_INFO_PTM                    0x00000400

/* bits 16 to 31 contain the requested info which is task specific. */
/* Access stratum control */
#define ENG_ACCESS_STRATUM_SCELL        0x00010000  /* Serving cell information */
#define ENG_ACCESS_STRATUM_NCELL        0x00020000  /* Neighbour cell information */
#define ENG_ACCESS_STRATUM_NCELL_TA     0x00040000

/* NAS control */
#define ENG_MM_INFO                     0x00010000
#define ENG_SM_INFO                     0x00020000
#define ENG_LLC_INFO                    0x00040000

/* Common usage for eng. mode */
#define ENG_MODE_IDLE_INFO  (ENG_ACCESS_STRATUM_SCELL | ENG_ACCESS_STRATUM_NCELL | ENG_INFO_IDLE_STATE | ENG_INFO_CONTINUOUS)
#define ENG_MODE_DED_INFO   (ENG_ACCESS_STRATUM_SCELL | ENG_ACCESS_STRATUM_NCELL | ENG_INFO_DED_STATE | ENG_INFO_CONTINUOUS)
#define ENG_MODE_GPRS_INFO  (ENG_INFO_PTM | ENG_INFO_CONTINUOUS)
# endif /* (UPGRADE_ADDIT_ENG_INFO) */

# if defined (UPGRADE_3G)
#define MAX_STORED_SRCH_UARFCN          20
#define MAX_STORED_SRCH_FDD_UARFCN      MAX_STORED_SRCH_UARFCN
# endif /* UPGRADE_3G */

#define SRCH_CTRL_MANUAL_SRCH_BIT       0x0001  /* user requested PLMN search */
#define SRCH_CTRL_EXHAUSTIVE_SRCH_BIT   0x0002  /* search whole band for PLMNs */
#define SRCH_CTRL_DISABLE_BAND_SELECT   0x0004  /* search bandMode given in PlmnListReq */
#define SRCH_CTRL_BA_LIST_SRCH_BIT      0x0008  /* only search UARFCNs given in PlmnListReq */

/****************************************************************************
 * Types
 ****************************************************************************/

#define R97_CAPAB_BIT               0x00
#define R99_CAPAB_BIT               0x01
#define EGPRS_CAPAB_BIT             0x02


typedef enum PktSwitchedCapabTag
{
    PKT_SWTCHD_R97_CAPAB        = R97_CAPAB_BIT,
    PKT_SWTCHD_R99_CAPAB        = R99_CAPAB_BIT,
    PKT_SWTCHD_EDGE_CAPAB       = EGPRS_CAPAB_BIT,
    PKT_SWTCHD_R99_EDGE_CAPAB   = R99_CAPAB_BIT + EGPRS_CAPAB_BIT,
    LAST_PKT_SWTCHD_CAPAB       = 0xff
}
PktSwitchedCapab;

# if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
/** Typedef of GprsSupportedBits */
typedef Int32 GprsSupportedBits;
/** Typedef of ThreeDigitMncEncodeBits */
typedef Int32 ThreeDigitMncEncodeBits;
# else /* UPGRADE_3G && UPGRADE_GGE */
/** Typedef of GprsSupportedBits */
typedef Int16 GprsSupportedBits;
/** Typedef of ThreeDigitMncEncodeBits */
typedef Int16 ThreeDigitMncEncodeBits;
# endif /* UPGRADE_3G && UPGRADE_GGE */

/** receive signal level */
typedef SignedInt16 ReceiveLevel;

#if defined (UPGRADE_NASMDL2)
#if defined (UPGRADE_3G)
/** RSCP measurenemt in 1/8dBm units */
typedef SignedInt16 UtraRscp;
/** Ec/No Level in 1/8dBm units */
typedef SignedInt16 UtraEcNo;
#endif /* UPGRADE_3G */
#endif /* UPGRADE_NASMDL2 */

/** Network band element */
typedef enum NetworkBandTag
{
    /** Unknown network (The network is not one of the following) */
    UNKNOWN_NETWORK,
    /** GSM network */
    GSM_NETWORK,
    /** DCS network */
    DCS_NETWORK,
    /** GSM and DCS network */
    GSM_DCS_NETWORK,
    /** PCS network */
    PCS_NETWORK,
    /** GSM and PCS network */
    GSM_PCS_NETWORK,
    /** GSM 850 network */
    GSM850_NETWORK,
    /** PCS and GSM850 network */
    GSM850_PCS_NETWORK,
    /** DCS and GSM850 network */
    GSM850_DCS_NETWORK,
    /** End of GERAN networks */
    END_OF_GERAN_NETWORKS,
    /** For single band configurations, when RAT is unknown  */
    UMTS_NETWORK,
    /** For single band configurations, when band is unknown  */
    UMTS_FDD_NETWORK,
    /** UMTS FDD band 1 network */
    UMTS_FDD_BAND_1_NETWORK,
    /** UMTS FDD band 2 network */
    UMTS_FDD_BAND_2_NETWORK,
    /** UMTS FDD band 3 network */
    UMTS_FDD_BAND_3_NETWORK,
    /** UMTS FDD band 4 network */
    UMTS_FDD_BAND_4_NETWORK,
    /** UMTS FDD band 5 network */
    UMTS_FDD_BAND_5_NETWORK,
    /** UMTS FDD band 6 network */
    UMTS_FDD_BAND_6_NETWORK,
    /** UMTS FDD band 7 network */
    UMTS_FDD_BAND_7_NETWORK,
    /** UMTS FDD band 8 network */
    UMTS_FDD_BAND_8_NETWORK,
    /** UMTS FDD band 9 network */
    UMTS_FDD_BAND_9_NETWORK,
    /** End of UTRAN FDD networks */
    END_OF_UTRA_FDD_NETWORKS,
    /** UMTS TDD128 network */
    UMTS_TDD128_NETWORK,
    /** UMTS TDD128 band A lower network */
    UMTS_TDD128_BAND_A_LOWER_NETWORK,
    /** UMTS TDD128 band A upper network */
    UMTS_TDD128_BAND_A_UPPER_NETWORK,
    /** UMTS TDD128 band B lower network */
    UMTS_TDD128_BAND_B_LOWER_NETWORK,
    /** UMTS TDD128 band B upper network */
    UMTS_TDD128_BAND_B_UPPER_NETWORK,
    /** UMTS TDD128 band C network */
    UMTS_TDD128_BAND_C_NETWORK,
    /** UMTS TDD128 band 19 network */
    UMTS_TDD128_BAND_F_NETWORK,
    /** UMTS TDD128 band 23 network */
    UMTS_TDD128_BAND_E_NETWORK,
    /** End of UTRAN TDD128 networks */
    END_OF_UTRA_TDD128_NETWORKS
#if defined (UPGRADE_LTE)
    ,
    /** For single band configurations when RAT is unknown */
    LTE_NETWORK,
    /** For single band configurations, when band is unknown */
    LTE_FDD_NETWORK,
    /** LTE FDD Network bands see TS 36.101 section 5.5
    40 bands cover the complete LTE Network - lowest 32 are FDD bands*/
    LTE_FDD_BAND_1_NETWORK,
    LTE_FDD_BAND_2_NETWORK,
    LTE_FDD_BAND_3_NETWORK,
    LTE_FDD_BAND_4_NETWORK,
    LTE_FDD_BAND_5_NETWORK,
    LTE_FDD_BAND_6_NETWORK,
    LTE_FDD_BAND_7_NETWORK,
    LTE_FDD_BAND_8_NETWORK,
    LTE_FDD_BAND_9_NETWORK,
    LTE_FDD_BAND_10_NETWORK,
    LTE_FDD_BAND_11_NETWORK,
    LTE_FDD_BAND_12_NETWORK,
    LTE_FDD_BAND_13_NETWORK,
    LTE_FDD_BAND_14_NETWORK,
    LTE_FDD_BAND_15_NETWORK,
    LTE_FDD_BAND_16_NETWORK,
    LTE_FDD_BAND_17_NETWORK,
    LTE_FDD_BAND_18_NETWORK,
    LTE_FDD_BAND_19_NETWORK,
    LTE_FDD_BAND_20_NETWORK,
    LTE_FDD_BAND_21_NETWORK,
    LTE_FDD_BAND_22_NETWORK,
    LTE_FDD_BAND_23_NETWORK,
    LTE_FDD_BAND_24_NETWORK,
    LTE_FDD_BAND_25_NETWORK,
    LTE_FDD_BAND_26_NETWORK,
    LTE_FDD_BAND_27_NETWORK,
    LTE_FDD_BAND_28_NETWORK,
    LTE_FDD_BAND_29_NETWORK,
    LTE_FDD_BAND_30_NETWORK,
    LTE_FDD_BAND_31_NETWORK,
    LTE_FDD_BAND_32_NETWORK,
    /** End of EUTRA FDD networks */
    END_OF_EUTRA_FDD_NETWORKS,
    /** For single band configurations, when band is unknown */
    LTE_TD_NETWORK,
    /** LTE TD Network bands -these are band 33 to 40 in the LTE bands defined in
    TS 36.101 section 5.5 */
    LTE_TD_BAND_33_NETWORK,
    LTE_TD_BAND_34_NETWORK,
    LTE_TD_BAND_35_NETWORK,
    LTE_TD_BAND_36_NETWORK,
    LTE_TD_BAND_37_NETWORK,
    LTE_TD_BAND_38_NETWORK,
    LTE_TD_BAND_39_NETWORK,
    LTE_TD_BAND_40_NETWORK,
    /** End of EUTRA TD networks */
    END_OF_EUTRA_TD_NETWORKS
#endif /* UPGRADE_LTE */
}
NetworkBand;

/* FR9805-2401: The following three structures better describe the capabilities of a MS.*/

/** Describes the capabilities of a MS (Used in RR to accept or reject channel
 * types and modes).
 */
typedef enum SpeechSupportCapabTag
{
    CAP_NO_SPEECH,                          /**< No speech support - data/signalling only MS */
    CAP_VERSION1,                           /**< Speech version 1 only */
    CAP_VERSION1_AND_VERSION2,              /**< Speech version 1 and version 2 (EFR) supported */
# if defined (UPGRADE_AMR_PS)
    CAP_VERSION1_AND_VERSION2_AND_VERSION3, /**< Speech version 1, version 2 (EFR) and version 3 (AMR) supported */
# endif /* (UPGRADE_AMR_PS) */
    /** Last speech capability */
    LAST_SPEECH_CAP
}
SpeechSupportCapab;

/** Indicates data rate supported by MS*/
typedef enum DataSupportCapabTag
{
    /** Not support data */
    CAP_NO_DATA,
    /** up to data rate 9600 bits/s */
    CAP_UP_TO_9600,
    /** up to data rate 14400 bits/s */
    CAP_UP_TO_14400
}
DataSupportCapab;

/** Defines capability of MS. Indicates quality of service That can
 * be supported by the mobile station. */
typedef struct MsCapabilityTag
{
    /** Indicates what type of speech the MS supports*/
    SpeechSupportCapab    speechSupportCapab;
    /** Indicates what type of data the MS supports*/
    DataSupportCapab      dataSupportCapab;
    Boolean               halfRateSpeechSupported; /**< If set then MS MUST have TCH_H + Half Rate Codec support */
    Boolean               halfRateDataSupported;   /**< If set then MS MUST have TCH_H + Half Rate data support */
    Boolean               voiceGroupCallService;   /**< For future expansion */
    Boolean               voiceBroadcastService;   /**< For future expansion */
    Int8                  hscsdMultislotClass;     /**< For future expansion - 0xFF if absent */
}
MsCapability;

#if defined(UPGRADE_NVRAM_OPTIMIZE) && defined(UPGRADE_NVRAM_ENHANCED)
typedef struct MsVarCapabilityVarTag
{
    /** Indicates what type of speech the MS supports*/
    SpeechSupportCapab    speechSupportCapab;
    /** Indicates what type of data the MS supports*/
    DataSupportCapab      dataSupportCapab;
    Boolean               halfRateSpeechSupported; /**< If set then MS MUST have TCH_H + Half Rate Codec support */
    Boolean               halfRateDataSupported;   /**< If set then MS MUST have TCH_H + Half Rate data support */
}
MsCapabilityVar;
#endif
/** Defines required services. */
typedef enum ServiceTypeTag
{
    /** CS service. */
    CIRCUIT_SWITCHED_SERVICE = 0,
#if defined (UPGRADE_LTE)
    /** Packet switched service.
    *This is single mode EPS SERVICE for LTE operation */
#else
    /** GPRS service */
#endif
    GPRS_SERVICE             = 1,
#if defined (UPGRADE_LTE)
    /** Combined service.
    *This is EPS and SMS over SGs for LTE operation */
#else
    /** Combined service. */
#endif
    COMBINED_SERVICE         = 2,
    /** No serice available. */
    NO_SERVICES_AVAILABLE    = 3
}
ServiceType;

/** Determines the ability of MS to handle GSM voice calls and GPRS connections*/
typedef enum GprsMsClassTag
{
    /** MS is  capable of simultaneous voice and data transmission*/
    GPRS_CLASS_A = 0,
    /** MS supports one type at a time, and switch automatically between data and voice */
    GPRS_CLASS_B = 1,
   /** MS supports one type at a time, and require user intervention to switch between data and voice*/
    GPRS_CLASS_C = 2
}
GprsMsClass;

/** Indicates the period in which the MS must continuously
 * decode the CCCH or the PCCH blocks.
 */
typedef enum GprsNonDrxModeTag
{
    /** no non-DRX mode after transfer state */
    DRX_AFTER_TRANSFER                = 0,
    /** maximum  1 sec non-DRX mode after transfer state*/
    NONDRX_FOR_1_SEC_AFTER_TRANSFER   = 1,
    /** maximum  2 sec non-DRX mode after transfer state*/
    NONDRX_FOR_2_SEC_AFTER_TRANSFER   = 2,
    /** maximum  4 sec non-DRX mode after transfer state*/
    NONDRX_FOR_4_SEC_AFTER_TRANSFER   = 3,
    /** maximum  8 sec non-DRX mode after transfer state*/
    NONDRX_FOR_8_SEC_AFTER_TRANSFER   = 4,
    /** maximum 16 sec non-DRX mode after transfer state*/
    NONDRX_FOR_16_SEC_AFTER_TRANSFER  = 5,
    /** maximum 32 sec non-DRX mode after transfer state*/
    NONDRX_FOR_32_SEC_AFTER_TRANSFER  = 6,
    /** maximum 64 sec non-DRX mode after transfer state*/
    NONDRX_FOR_64_SEC_AFTER_TRANSFER  = 7
}
GprsNonDrxMode;

/** Defines GPRS network mode. */
typedef enum GprsNetworkModeTag
{
    /** Mode 1*/
    GPRS_NETWORK_MODE_1           = 0,
    /** Mode 2*/
    GPRS_NETWORK_MODE_2           = 1,
    /** Mode 3*/
    GPRS_NETWORK_MODE_3           = 2,
    /** Reserved */
    GPRS_NETWORK_MODE_RESERVED    = 3
}
GprsNetworkMode;

# if defined (UPGRADE_3G)
/** Defines Discontinuous Reception Cycle which is an individual
 * time interval between monitoring Paging Occasion for a specific UE
 */
typedef enum UPsDrxParameterTag
{
    /** CN Specific DRX cycle length coefficient not
     * specified by the MS, ie. the system information
     * value 'CN domain specific DRX cycle length' is used.
     */
    UPS_DRX_COEFF_USE_SI = 0,          /* use value from SI */
    /** FRAMes64. */
    UPS_DRX_COEFF_6      = 6,
    /** FRAMes128. */
    UPS_DRX_COEFF_7      = 7,
    /** FRAMes256. */
    UPS_DRX_COEFF_8      = 8,
    /** FRAMes512. */
    UPS_DRX_COEFF_9      = 9
}
UPsDrxParameter;
# endif /* UPGRADE_3G */

/** GPRS cell information element */
typedef struct GprsCellInfoTag
{
    /** Identifies a routing area within a location area for a GSM-MAP
     * type of PLMN.
     */
    Rac                 rac;
    /** Specifies the network mode of GPRS */
    GprsNetworkMode     gprsNetworkMode;
    /** Whether or not EGPRS is supported */
    Boolean             egprsSupported;
}
GprsCellInfo;

/** Defines DRX parameter values. Defines DRX parameter to allow the MS
 * to power down significant amounts of its internal circuitry for a high
 * percentage of the time when it is in the idle mode.
 */
typedef struct GprsDrxParametersTag
{
    /** Contains the binary coded value of the SPLIT PG CYCLE CODE
     * (1 octet) from which SPLIT PG CYCLE value is derived as shown
     * in 24.008.
     */
    Int8            splitPgCycleCode;
    /** Indicates whether MS supports the split pg cycle option on CCCH */
    Boolean         splitOnCcch;
    /** Indicates how long MS stays non-DRX mode after last transfer. */
    GprsNonDrxMode  nonDrxTimer;
# if defined (UPGRADE_3G) || defined(UPGRADE_TWGGE_NRW)
    /** Represents PS DRX cycle length coefficient */
    UPsDrxParameter psDrxParameter;
#endif
}
GprsDrxParameters,
/** Defines DRX parameter values. Defines DRX parameter to allow the MS
 * to power down significant amounts of its internal circuitry for a high
 * percentage of the time when it is in the idle mode.
 */
GprsDrxParametersElement;

/** Defines GPRS capability */
typedef struct GprsCapabilityTag
{
    /** Indicates that MS supports the use of multiple
     * timeslots and it ranges form 1 to 45
     */
    Int8                multiSlotClass;
    /** Determines GPRS class type (A GPRS MS may operate in one of the following MS
     * operation modes A-B-C).
     */
    GprsMsClass         msClass;
    /** Indicates whether Extended Dynamic Allocation Capability for
     * GPRS is implemented or not.
     */
    Boolean             extDynamicAllocation;
    /** The SMS field (4 bits)indicates the time needed for the mobile station to
     * switch from one radio channel to another, perform a neighbor cell
     * power measurement, and the switch from that radio channel to another
     * radio channel \note This field is not used by the network and may be excluded
     * by the MS
     */
    Int8                switchMeasureSwitch;
    /** The SM field indicates the time needed for the mobile station to
     * switch from one radio channel to another and perform a neighbour
     * cell power measurement \note This field is not used by the network and
     * may be excluded by the MS.
     */
    Int8                switchMeasure;
    /** Defines discontinuous reception parameters */
    GprsDrxParameters   drxParameters;
    /** This field indicates whether the MS supports the GERAN Feature Package 1*/
    Boolean             geran4Fp1SupportEnabled;
# if defined (UPGRADE_EDGE)
    /** Indicates if EGPRS is supported */
    Boolean             egprsSupportEnabled;
# endif /* (UPGRADE_EDGE) */
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
    /** PS Handover Capability */
    Boolean             psHoCapabSupportedEnabled;
#endif /* (UPGRADE_3G_FDD) && (UPGRADE_REL6) */
}
GprsCapability;

#if defined(UPGRADE_NVRAM_OPTIMIZE) && defined(UPGRADE_NVRAM_ENHANCED)
typedef struct GprsCapabilityVarTag
{
    /** Indicates that MS supports the use of multiple
     * timeslots and it ranges form 1 to 45
     */
    Int8                multiSlotClass;
    /** Determines GPRS class type (A GPRS MS may operate in one of the following MS
     * operation modes A-B-C).
     */
    GprsMsClass         msClass;
    /** Indicates whether Extended Dynamic Allocation Capability for
     * GPRS is implemented or not.
     */
    Boolean             extDynamicAllocation;
    /** The SMS field (4 bits)indicates the time needed for the mobile station to
     * switch from one radio channel to another, perform a neighbor cell
     * power measurement, and the switch from that radio channel to another
     * radio channel \note This field is not used by the network and may be excluded
     * by the MS
     */
    Int8                switchMeasureSwitch;
    /** The SM field indicates the time needed for the mobile station to
     * switch from one radio channel to another and perform a neighbour
     * cell power measurement \note This field is not used by the network and
     * may be excluded by the MS.
     */
    Int8                switchMeasure;
    /** Defines discontinuous reception parameters */
    GprsDrxParameters   drxParameters;
    /** This field indicates whether the MS supports the GERAN Feature Package 1*/
    Boolean             geran4Fp1SupportEnabled;
#if defined(UPGRADE_TWGGE_NRW)
    /** Indicates if EGPRS is supported */
    Boolean             egprsSupportEnabled;

    /** PS Handover Capability */
    Boolean             psHoCapabSupportedEnabled;
#else
# if defined (UPGRADE_EDGE)
    /** Indicates if EGPRS is supported */
    Boolean             egprsSupportEnabled;
# endif /* (UPGRADE_EDGE) */

#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
    /** PS Handover Capability */
    Boolean             psHoCapabSupportedEnabled;
#endif /* (UPGRADE_3G_FDD) && (UPGRADE_REL6) */
#endif
}
GprsCapabilityVar;
#endif
#if defined(UPGRADE_TWGGE_NRW)
typedef struct EgprsCapabilityTag
{
    /** Indicates that MS supports the use of multiple
     * timeslots and it ranges form 1 to 45
     */
    Int8                multiSlotClass;
    /** Whether or not Extended Dynamic Allocation is supported;
     * Extended Dynamic Allocation used to allow higher uplink throughput.*/
    Boolean             extDynamicAllocation;
    /** Whether or not PSK is supported on uplink */
    Boolean             pskOnUpLinkSupported;
}
EgprsCapability;
#else
# if defined (UPGRADE_EDGE)
/** EGPRS capability */
typedef struct EgprsCapabilityTag
{
    /** Indicates that MS supports the use of multiple
     * timeslots and it ranges form 1 to 45
     */
    Int8                multiSlotClass;
    /** Whether or not Extended Dynamic Allocation is supported;
     * Extended Dynamic Allocation used to allow higher uplink throughput.*/
    Boolean             extDynamicAllocation;
    /** Whether or not PSK is supported on uplink */
    Boolean             pskOnUpLinkSupported;
}
EgprsCapability;
# endif /* (UPGRADE_EDGE) */
#endif

/** RXQUAL enumeration, values must be sequential from 0 to 7.
*/
typedef enum ReceiveQualityTag
{
   /** Receive signal quality 0 */
   RXQUAL_0 = 0,
   /** Receive signal quality 1 */
   RXQUAL_1,
   /** Receive signal quality 2 */
   RXQUAL_2,
   /** Receive signal quality 3 */
   RXQUAL_3,
   /** Receive signal quality 4 */
   RXQUAL_4,
   /** Receive signal quality 5 */
   RXQUAL_5,
   /** Receive signal quality 6 */
   RXQUAL_6,
   /** Receive signal quality 7 */
   RXQUAL_7
}
ReceiveQuality;

/** Selected parameters element */
typedef struct SelectionParametersTag
{
    /** Indicates if requested PLMN contains valid information.
\assoc PRESENT \ref requestedPlmn
\assoc PRESENT \ref requestedPlmn */
    Boolean                         requestedPlmnValid;
    /** MCC, MNC and bit map giving access technology to use. */
    Plmn                            requestedPlmn;
    BaList                          baList;             /**<  BCCH allocation list from NVRAM or 3G SIM*/
    Si2BaList                       si2BaList;          /**< BCCH allocation list from 2G SIM */
    /* see 03.22 4.4.3.1 manual PLMN selection   */
    /** Indicates whether user selected PLMN. */
    Boolean                         plmnManuallySelected;
    /** Indicates supported bands */
    BandMode                        bandMode;
}
SelectionParameters;

/** Radio information of received signal */
typedef struct RadioInfoTag
{
    /** Indicates receive signal level */
    ReceiveLevel                    receiveLevel;
    /** Receive signal quality */
    ReceiveQuality                  receiveQuality;
#if defined (UPGRADE_NASMDL2)
#if defined (UPGRADE_3G)
    /** RSCP measurement in 1/8 dBm units */
    UtraRscp                        utraRscp;
    /** Ec/No Level in 1/8 dBm units */
    UtraEcNo                        utraEcNo;
#endif /* UPGRADE_3G */
#if defined (UPGRADE_LTE)
    /** The RSRQ measurement in 1/8 dB units.
     *  Range is -19.5 to -3.
     */
    EutraRsrq                       eutraRsrq;
    /** The RSRP measurement in 1/8 dBm units.
     *  Range is -140 to -44.
     */
    EutraRsrp                       eutraRsrp;
#endif /* UPGRADE_LTE */
#endif /* UPGRADE_NASMDL2 */
    /** Access Technology */
    AccessTechnologyId              accessTechnology;
}
RadioInfo;

/** Outcomes the PLMN search procedure. */
typedef enum PlmnListStatusTag
{
    /** search was success. */
    PLMN_LIST_SUCCESS = 0,
    /** search has faild */
    PLMN_LIST_FAILED  = 1,
    /** search has aborted */
    PLMN_LIST_ABORTED = 2
}
PlmnListStatus;

/** Defines service available. */
typedef enum ActivateModeTag
{
    /** Activate mode normal*/
    AM_NORMAL,
    /** Activate mode for Emergency only. */
    AM_EMERGENCY_ONLY,
#if defined (UPGRADE_LTE)
    /** Activate mode for forbidden LA or TA */
#else
    /** Activate mode for forbidden LA*/
#endif
    AM_FORBIDDEN_LA,
#if defined (UPGRADE_LTE)
    /** Activate mode for forbidden NAT LA or TA*/
#else
    /** Activate mode for forbidden NAT LA*/
#endif
    AM_FORBIDDEN_NAT_LA,
    /** Activate mode for update. */
    AM_ATTEMPTING_TO_UPDATE
}
ActivateMode;

/** Indicate the status. */
typedef enum ActivateStatusTag
{
    /** Success. */
    ACT_SUCCESS,
    /** Emergency only*/
    ACT_EMERGENCY_ONLY,
#if defined (UPGRADE_LTE)
    /** Forbidden LA or TA. */
#else
    /** Forbidden LA. */
#endif
    ACT_FORBIDDEN_LA,
#if defined (UPGRADE_LTE)
    /** Forbidden NAT LA or TA. */
#else
    /** Forbidden NAT LA. */
#endif
    ACT_FORBIDDEN_NAT_LA,
    /** No service available. */
    ACT_NO_SERVICE,
    /** LAST_ACTIVATE_STATUS  */
    LAST_ACTIVATE_STATUS
}
ActivateStatus;

# if defined (UPGRADE_3G)
/** Contains set of frequencies.*/
typedef struct UarfcnListTag
{
    /** Indicates the number of frequencies
\assoc ARRAY \ref uArfcn */
    Int8                numUarfcn;
    /** Lists the frequnecies. */
    UUARFCN             uArfcn[MAX_STORED_SRCH_UARFCN];
}
UarfcnList;
#if defined(UPGRADE_TWGGE_NRW)
# if defined(UPGRADE_3G)
/** Lists primary scrambling codes available in a defined frequency */
typedef struct UUMTS_FddFrequencyListInfoTag
{
    /** Indicates the downlink frequency of the cell. */
    UUARFCN                             uarfcn_dl;
    /** Indicates the number of scrambling codes.
\assoc ARRAY \ref primaryScramblingCode */
    Int8                                numberOfScramblingCodes;
    /** List scrambling codes. */
    UPrimaryScramblingCode              primaryScramblingCode [MAX_STORED_SCRAMBLING_CODES];
}
UUMTS_FddFrequencyListInfo;

/** Defines FDD cell information */
typedef struct FddCellInformationTag
{
    /** Indicates The number of FDD frequecies
\assoc ARRAY \ref frequencyListInfo */
    Int8                                numberOfFddFrequencies;
    /** lists information for a set of frequencies */
    UUMTS_FddFrequencyListInfo          frequencyListInfo [MAX_FDD_INTER_FREQUENCY_CELLS];
}
FddCellInformation;

typedef struct UUMTS_Tdd128FrequencyListInfoTag
{
    UUARFCN                             uarfcn;
/** \assoc ARRAY \ref cellParametersID */
    Int8                                numberOfCellParametersId;
    UCellParametersID                   cellParametersID[MAX_STORED_CELLPARAMETERSID];/* modified by Aaron.cai according to IFT doc 2008-08-19*/
}
UUMTS_Tdd128FrequencyListInfo;

/** Cell informarion related to TDD128 */
typedef struct Tdd128CellInformationTag
{
    /** The number of TDD128 frequencies
\assoc ARRAY \ref frequencyListInfo */
    Int8                                numberOfTdd128Frequencies;
    /** Holds the list of TDD128 frequencies */
    UUMTS_Tdd128FrequencyListInfo       frequencyListInfo [MAX_TDD128_FREQUENCY_CELLS];
}
Tdd128CellInformation;
# endif
#else
#  if defined (UPGRADE_3G_FDD)
/** Lists primary scrambling codes available in a defined frequency */
typedef struct UUMTS_FddFrequencyListInfoTag
{
    /** Indicates the downlink frequency of the cell. */
    UUARFCN                             uarfcn_dl;
    /** Indicates the number of scrambling codes.
\assoc ARRAY \ref primaryScramblingCode */
    Int8                                numberOfScramblingCodes;
    /** List scrambling codes. */
    UPrimaryScramblingCode              primaryScramblingCode [MAX_STORED_SCRAMBLING_CODES];
}
UUMTS_FddFrequencyListInfo;

/** Defines FDD cell information */
typedef struct FddCellInformationTag
{
    /** Indicates The number of FDD frequecies
\assoc ARRAY \ref frequencyListInfo */
    Int8                                numberOfFddFrequencies;
    /** lists information for a set of frequencies */
    UUMTS_FddFrequencyListInfo          frequencyListInfo [MAX_FDD_INTER_FREQUENCY_CELLS];
}
FddCellInformation;
#  endif /* (UPGRADE_3G_FDD) */

#  if defined (UPGRADE_3G_TDD128)
typedef struct UUMTS_Tdd128FrequencyListInfoTag
{
    UUARFCN                             uarfcn;
/** \assoc ARRAY \ref cellParametersID */
    Int8                                numberOfCellParametersId;
    UCellParametersID                   cellParametersID[MAX_STORED_CELLPARAMETERSID];/* modified by Aaron.cai according to IFT doc 2008-08-19*/
}
UUMTS_Tdd128FrequencyListInfo;

/** Cell informarion related to TDD128 */
typedef struct Tdd128CellInformationTag
{
    /** The number of TDD128 frequencies
\assoc ARRAY \ref frequencyListInfo */
    Int8                                numberOfTdd128Frequencies;
    /** Holds the list of TDD128 frequencies */
    UUMTS_Tdd128FrequencyListInfo       frequencyListInfo [MAX_TDD128_FREQUENCY_CELLS];
}
Tdd128CellInformation;
#  endif /* (UPGRADE_3G_TDD128) */
#endif
#  if defined (UPGRADE_3G_FDD)
typedef struct FddCellInfoAndBandsTag
{
    Boolean                               fddUArfcnListOnlySearch;
    FddCellInformation                    fddCellInformation;
#if defined (UPGRADE_FDD_MULTIBAND)
    FddBandMode                           fddBandMode;
#endif /* UPGRADE_FDD_MULTIBAND */
#if defined (PRODUCT_CALIBRATION_TEST_SWITCH)
    Boolean                              shouldLockUArfcn;
#endif
}
FddCellInfoAndBands;
#  endif /* (UPGRADE_3G_FDD) */

#  if defined (UPGRADE_3G_TDD128)
typedef struct Tdd128CellInfoAndBandsTag
{
    Boolean                              tdd128UArfcnListOnlySearch;
    Tdd128CellInformation                tdd128CellInformation;
    Tdd128BandMode                       tdd128BandMode;
#if defined (PRODUCT_CALIBRATION_TEST_SWITCH)
    Boolean                              shouldLockUArfcn;
#endif
}
Tdd128CellInfoAndBands;
#  endif /* (UPGRADE_3G_TDD128) */

/** Determines bands based on the handsetModes (GSM only, FDD only, TDD only, GSM+FDD or GSM+TDD) */
typedef union CellInfoAndBandsUnionTag
{
#  if defined (UPGRADE_GGE)
    Int8                                gsm;        /**< use if handsetModes is NMODE_GSM in genie  */
#  else /* !(UPGRADE_GGE)*/
    Int8                                dummy_1;
#  endif /* !(UPGRADE_GGE)*/
#  if defined (UPGRADE_3G_FDD)
    FddCellInfoAndBands                 fdd;        /**< use if handsetModes is NMODE_UMTS_FDD and NMODE_GSM_UMTS_FDD  */
#  else /* !(UPGRADE_3G_FDD) */
    Int8                                dummy_2;
#  endif /* !(UPGRADE_3G_FDD) */
#  if defined (UPGRADE_3G_TDD128)
    Tdd128CellInfoAndBands              tdd128;     /**< use if handsetModes is NMODE_UMTS_TDD128 and NMODE_GSM_UMTS_TDD128 */
#  else /* !(UPGRADE_3G_TDD128) */
    Int8                                dummy_3;
#  endif /* !(UPGRADE_3G_TDD128) */
#  if defined (UPGRADE_3G_FDD)  && defined (UPGRADE_GGE)
    FddCellInfoAndBands                 gsmfdd;     /**< use if handsetModes is NMODE_GSM_UMTS_FDD in genie */
#  else /* !(UPGRADE_3G_FDD)  || !(UPGRADE_GGE) */
    Int8                                dummy_4;
#  endif /* !(UPGRADE_3G_FDD)  || !(UPGRADE_GGE) */
#  if defined (UPGRADE_3G_TDD128)  && defined (UPGRADE_GGE)
    Tdd128CellInfoAndBands              gsmtdd128;  /**< use if handsetModes is NMODE_GSM_UMTS_TDD128 in genie */
#  else /* !(UPGRADE_3G_TDD128)  || !(UPGRADE_GGE) */
    Int8                                dummy_5;
#  endif /* !(UPGRADE_3G_TDD128)  || !(UPGRADE_GGE) */
}
CellInfoAndBandsUnion;

/** List of information about cell element */
typedef struct NcellListTag
{
    /** Indicates if list of information about cell is valid */
#  if defined (UPGRADE_GGE)
    /**
\assoc PRESENT \ref channelData */
#  endif /* UPGRADE_GGE */
    Boolean            isValid;
#  if defined (UPGRADE_GGE)
    /** Holds the list of channels */
    Int8               channelData[BA_ARRAY_SIZE];
    /** Indicates whether channels 512-810 in channelData[] are DCS or PCS */
    Boolean            isDcsBand;
    /** Additional GSM network parameters */
    Si2BaList          si2BaList;
#  endif /* UPGRADE_GGE */
#  if defined (UPGRADE_3G_FDD)
    /** Cell information about FDD cell */
    FddCellInformation fddCellInformation;
    /** Information about FDD cell */
    UarfcnList         fddUArfcnSrchList;
#  endif /* (UPGRADE_3G_FDD) */
#   if defined (UPGRADE_3G_TDD128)
    /** Cell information about TDD128 cell */
    Tdd128CellInformation tdd128CellInformation;
    /** Information about TDD128 cell */
    UarfcnList         tdd128UArfcnSrchList;
#   endif /* (UPGRADE_3G_TDD128) */
}
NcellList;
# endif /* UPGRADE_3G */

/** MM system information elements */
typedef struct MmSystemInformationTag
{
    /** LAI; Identifies uniquely a location area for a GSM-MAP type of PLMN */
#if defined (UPGRADE_LTE)
    /** This can be a Tracking area identity when in LTE mode               */
#endif
    LocationAreaIdElement           lai;
    /** Indicates if attached detach is allowed.
     * IMSI attach detach allowed flag coded as a 1 bit number
     * contained in the LSB of the second byte.
     */
    Boolean                         attachDetachAllowed;
    /** Coded as an 8 bit number contained in the first octet. */
    TmsiTime                        tmsiTime;
    /** BCCH allocation list */
    BaList                          baList;
    /** Indicates which band to use */
    Band                            band;
    /** SI2 BCCH allocation list */
    Si2BaList                       si2BaList;
    /** Access class */
    Int16                           accessClass;
#if defined (UPGRADE_REL6) && defined (UPGRADE_3G_FDD)
    /** indicate if the DSAC present in the cell
     \assoc PRESENT \ref barredDomainSpecificAccessClasses */
    Boolean                         barredDomainSpecificAccessClassesPresent;
    Int16                           barredDomainSpecificAccessClasses[maxCNdomains];
#endif /* (UPGRADE_REL6) && (UPGRADE_3G_FDD) */
    /** Cell information related to GPRS */
    GprsCellInfo                    gprsCellInfo;
    /** TRUE if MSC is R99+, FALSE if R98- */
    Boolean                         mscR99OrLater;
    PktSwitchedCapab                pktSwitchedCapab;
    /** Indicates which band to use */
    BandMode                        bandMode;
#if defined (UPGRADE_LTE)
    PlmnSysInfoList                 plmnIdentityList;
#endif
# if defined (UPGRADE_3G)
    /** Holds a list of frequencies */
    UarfcnList                      uArfcnBaList;
# endif /* (UPGRADE_3G) */
#if defined (UPGRADE_3G_HSDPA)
     /** Whether or not HSDPA Cell Indicator is present.
     * The intent of this parameter in SIB5/SIB5bis is to allow UEs to display a visual cue
     * that HSDPA functionality is available in the current cell
     */
    Boolean                         hsdpaCellIndicator;
#endif /* UPGRADE_3G_HSDPA */
#if defined (UPGRADE_3G_HSUPA)
    /** Whether or not HSUPA Cell Indicator is present.
     * The intent of this parameter in SIB5/SIB5bis is to allow UEs to display a visual cue
     * that HSUPA functionality is available in the current cell
     */
    Boolean                         hsupaCellIndicator;
#endif /* UPGRADE_3G_HSUPA */
#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
#  if defined (UPGRADE_3G_TDD128)
            /** Whether or not MBMS Cell Indicator is present.
             * The intent of this parameter is to allow UEs to display a visual cue
             * that MBMS functionality is available in the current cell
             */
            Boolean                   mbmsCellIndicator;
#  endif /*UPGRADE_3G_TDD128*/
# endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */
}
MmSystemInformation;

/** Indicates the deactivation cause */
typedef enum AccStratumDeactCauseTag
{
    /** Deactivation because of power-off */
    DEACT_POWER_OFF         = 0,
    /** Deactivation because of silent sutdown */
    DEACT_SILENT_SHUTDOWN   = 1,
    /** Deactivation because RAT has changed */
    DEACT_RAT_CHANGE        = 2,
    /** Deactivation because of inactive RAT */
    DEACT_INACTIVE_RAT      = 3,
    DEACT_NULL              = 4,
    /** Deactivation because of entering PSM */
    DEACT_PSM               = 5,
    DEACT_NSPS              = 6
}
AccStratumDeactCause;

/** GPRS ready state element */
typedef enum GprsReadyStateTag
{
    /** GMM GPRS in idle state */
    GMM_GPRS_IDLE_STATE    = 0,
    /** GMM GPRS in standby state */
    GMM_GPRS_STANDBY_STATE = 1,
    /** GMM GPRS in ready state */
    GMM_GPRS_READY_STATE   = 2
}
GprsReadyState;

/** Indicates the Access stratum capability information */
typedef enum AccStratumCapabInfoTag
{
    /** GPRS is allowed */
    GPRS_ALLOWED            = 0,
    /** GPRS signalling only */
    GPRS_SIGNALLING_ONLY    = 1,
    /** GPRS not allowed */
    GPRS_NOT_ALLOWED        = 2
}
AccStratumCapabInfo;

typedef struct CipheringKeyInfoTag
{
    KeySequence     cksn;
    Int8            kc[CIPHERING_KEY_SIZE];
}
CipheringKeyInfo;

#endif /* !(GRRMRTYP_H) */

/* END OF FILE */
