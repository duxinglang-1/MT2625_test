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
 * Types common to mnxx_sig.h mmxx_sig.h rr_sig.h and alsi_sig.h.
 **************************************************************************/

#ifndef SIL3_TYP_H
#define SIL3_TYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#include <system.h>

/****************************************************************************
 * Macros
 ****************************************************************************/

#define     MAX_SUB_NO_LENGTH           20

/****************************************************************************
 * Types
 ****************************************************************************/
/** Assignor/Assignee (octet 5b) */
typedef enum AssignorAssigneeTag
{
    /** Message originator is "default assignee" */
    DEFAULT_ASSIGNEE = 0,
    /** Message originator is "assignor only"*/
    ASSIGNOR_ONLY = 1
}
AssignorAssignee;

/** Type of asymmetry */
typedef enum AsymmetryIndicationTag
{
    /** Symmetry preffered */
    SYMMETRY_PREFERRED = 0,
    /** Unused in the code */
    DOWNLINK_BIASED_PREFERRED = 1,
    /** Unused in the code */
    UPLINK_BIASED_PREFERRED = 2
}
AsymmetryIndication;

/** Specifies the type of the number */
typedef enum  BcdNumberTypeTag
{
    /** Type unknown */
    NUM_TYPE_UNKNOWN            =   0,
    /** International number */
    NUM_TYPE_INTERNATIONAL      =   1,
    /** National number */
    NUM_TYPE_NATIONAL           =   2,
    /** network specific number */
    NUM_TYPE_NETWORK_SPEC       =   3,
    /** dedicated access, short code */                 
    NUM_TYPE_DEDIC_PAD          =   4
}
BcdNumberType;

/** Numbering plan identification, it applies only for type of 
 * number = 000, 001, 010 and 100.
 */
typedef enum BcdNumberPlanTag
{
    /** Unknown */
    BCD_PLAN_UNKNOWN            =   0,
    /** ISDN/telephony numbering plan (Rec E-164/E-163) */                                             
    BCD_PLAN_E163_E164          =   1,
    /** data numbering plan (Recommendation X-121)*/
    BCD_PLAN_X121_DATA          =   3,
    /** telex numbering plan (Recommendation F-69) */
    BCD_PLAN_F96_TELEX          =   4,
    /** National numbering plan */                       
    BCD_PLAN_NATIONAL           =   8,
    /** private numbering plan   */                      
    BCD_PLAN_PRIVATE            =   9
}
BcdNumberPlan;

/** Specifies the type of channel requirement */
typedef enum ChannelRequirementTag
{
    /** Reserved */
    RESERVED_CHAN_REQUIREMENT   =   0,
    /** full rate support only MS */
    CHAN_FULL_RATE              =   1,
    /** dual rate support MS/half rate preferred */
    CHAN_DUAL_HALF_PREF         =   2,
    /** dual rate support MS/full rate preferred */
    CHAN_DUAL_FULL_PREF         =   3,
    /** Channel not required */
    CHAN_NOT_REQUIRED           =   4
}
ChannelRequirement;

/** Specifies transfer capability */
typedef enum TransferCapabilityTag
{
    /** Speech */
    BEARER_CAPAB_SPEECH         =   0,
    /** unrestricted digital information */
    BEARER_CAPAB_DIG_UNRESTRICTED = 1,
    /** 3.1 KHz audio, ex PLMN */
    BEARER_CAPAB_3100HZ_AUDIO   =   2,
    /** facsimile group 3 */
    BEARER_CAPAB_FAX_GROUP_3    =   3,
    /** Other ITC  */
    BEARER_CAPAB_OTHER_ITC      =   5,

    BEARER_CAPAB_AUX_SPEECH   =   6
}
TransferCapability;

/** Data Sytructure */
typedef enum DataStructureTag
{
    /** Service data unit integrity */
    KEEP_SDU_STRUCTURE          =   0,
    /** Unstructured */
    UNSTRUCTURED                =   3
}
DataStructure;

/** Rate adaption (octet 5) */
typedef enum RateAdaptionTag
{
    /** no rate adaption  */
    NO_RATE_ADAPTION            =   0,
    /** V.110/X.30 rate adaptation  */
    V110_X30_RATE_ADAPTION      =   1,
    /** ITU-T X.31 flag stuffing */
    CCITT_X31_FLAG_STUFFING     =   2,
    /** Other rate adaption  */
    OTHER_RATE_ADAPTION         =   3,
    /** Number of rates */
    NUM_RATE_ADAPTIONS
}
RateAdaption;

/** Signalling access protocol (octet 5) */
typedef enum SigAccessProtocolTag
{
    /** I-440/450  */
    I_440_450                   =   1,
    /** X.32  */
    X32                         =   6
}
SigAccessProtocol;

/** Specifies the number of stop bits */
typedef enum NoOfStopBitsTag
{
    /** 1 bit (This value is also used in the case of synchronous mode) */
    STOP_BITS_1                 =   0,
    /** 2 bits  */
    STOP_BITS_2                 =   1
}
NoOfStopBits;

/** Specifies the number of data */
typedef enum NoOfDataBitsTag
{
    /** 7 bits  */
    DATA_BITS_7                 =   0,
    /** 8 bits (this value is also used in the case of bit oriented protocols)  */
    DATA_BITS_8                 =   1
}
NoOfDataBits;

/** Indicates the user rate */
typedef enum UserRateTag
{
    /** For some 3g calls the user rate is irrelevant, and some
       networks just set the field to 0.  The value 0 should not be
       used for MO calls, however, since some (other?) networks reject
       such a value, even if it is irrelevant. */
    USER_RATE_RESERVED_0        =   0,
    /** 300 bit/s  */
    USER_RATE_300               =   1,
    /** 1200 bit/s  */
    USER_RATE_1200              =   2,
    /** 2400 bit/s  */
    USER_RATE_2400              =   3,
    /** 4800 bit/s  */
    USER_RATE_4800              =   4,
    /** 9600 bit/s  */
    USER_RATE_9600              =   5,
    /** 12000 bit/s  */
    USER_RATE_12000             =   6,
    /*job137331: Fix Anite TC 11.1.2 Add user rate 7 for V.23 */
    /** 12000/75 bit/s */
    USER_RATE_12000_75          =   7
}
UserRate;

/** In band/Out of band negotiation (octet 5b) */
typedef enum InBandOutOfBandTag
{
    /** Negotiation is done in-band using logical link zero */
    IN_BAND_LOGICAL_LINK_ZERO_NEGOTIATION = 0,
    /** Negotiation is done with USER INFORMATION messages on a temporary
        signalling connection. */
    USER_INFORMATION_NEGOTIATION  = 1
}
InBandOutOfBand;

/** Specifies the intermediate rate */
typedef enum IntermediateRateTag
{
    /** Reserved */
    INTERMED_RATE_NOT_USED      =   0,
    /** 8 kbit/s */
    INTERMED_RATE_8000          =   2,
    /** 16 kbit/s */
    INTERMED_RATE_16000         =   3
}
IntermediateRate;

/** Parity information */
typedef enum ParityInformationTag
{
    /** Odd */
    GSM_PARITY_ODD                  =   0,
    /** Even */
    GSM_PARITY_EVEN                 =   2,
    /** None */
    GSM_PARITY_NONE                 =   3,
    /** Forced to 0 */
    GSM_PARITY_FORCED_TO_0          =   4,
    /** Forced to 1 */
    GSM_PARITY_FORCED_TO_1          =   5
}
ParityInformation;

/** Connection element */
typedef enum ConnectionTag
{
    /**  Transparent  */
    CON_EL_TRANSPARENT          =   0,
    /** non transparent (RLP) */
    CON_EL_NON_TRANSPARENT      =   1,
    /** both, transparent preferred  */
    CON_EL_TRANSP_PREF          =   2,
    /** both, non transparent preferred  */
    CON_EL_NON_TRANSP_PREF      =   3
}
ConnectionElement;

/** Specifies The type of the modem */
typedef enum ModemTypeTag
{
    /** None */
    MODEM_TYPE_NONE             =   0,
    /** V.21 */
    MODEM_TYPE_V21              =   1,
    /** V.22 */
    MODEM_TYPE_V22              =   2,
    /** V.22 bis */
    MODEM_TYPE_V22BIS           =   3,
    /** V.23 */
    /*job137331: Fix*/
    /** Anite TC 11-1-2 Add modem type 7 for V-23 */
    MODEM_TYPE_V23              =   4,
    /** V.26 ter */
    MODEM_TYPE_V26TER           =   5,
    /** V.32 */
    MODEM_TYPE_V32              =   6,
    /** modem for undefined interface */
    MODEM_TYPE_UNDEFINED_IF     =   7,
    /** autobauding type 1 */
    MODEM_TYPE_AUTOBAUD_1       =   8
}
ModemType;

/** User information layer 2 protocol (octet 7)*/
typedef enum Layer2ProtocolTag
{
    /** Recommendation X.25, link level */
    L2_PROTOCOL_X25             =   6,
    /** ISO 6429, codeset 0 (DC1/DC3) */
    L2_PROTOCOL_ISO6429         =   8,
    /** Reserved: was allocated but never used in earlier phases of the protocol*/
    L2_PROTOCOL_X75             =   9,
    /** Videotex profile 1 */
    L2_PROTOCOL_VIDEOTEX_1      =   10,
    /** COPnoFlCt (Character oriented Protocol with no Flow Control
        mechanism)*/
    L2_PROTOCOL_COP_NO_FLCT     =   12,
    /** X.75 layer 2 modified (CAPI) */
    L2_PROTOCOL_X75_CAPI        =   13
}
Layer2Protocol;

/** Specifies logical link identifier negotiation */
typedef enum LliTag
{
    /** Default, LLI=256 only */
    DEFAULT_256_LLI = 0,
    /** Full protocol negotiation */
    FULL_PROTOCOL_NEGOTIATION_LLI = 1
}
Lli;

/** Specifies the mode of operation */
typedef enum ModeOfOperationTag
{
    /** Bit transparent mode of operation */
    BIT_TRANSPARENT_MODE_OF_OPERATION = 0,
    /** Protocol sensitive mode of operation */
    PROTOCOL_SENSITIVE_MODE_OF_OPERATION = 1
}
ModeOfOperation;


/** Odd or even data frame indicator.
 * This enumerator is used by the RTL1 interface to tell the data services
 * stack whether data is in the first or second half of a data frame. This
 * is required for 4.8 non-transparent data.
 */
typedef enum OddEvenIndicatorTag
{
    /** even number of address signals  */
    EVEN                        =   0,
    /** odd number of address signals */              
    ODD                         =   1
}
OddEvenIndicator;

/** If the value "Other ITC" is not signalled in the field ITC then 
 * the contents of this field shall be ignored.
 */
typedef enum OtherItcTag
{
    /** Restricted digital information */
    OTHER_ITC_RESTRICTED_DIGITAL_INFORMATION = 0
    /* all other values are reserved */
}
OtherItc;

/** If the value Other rate adaption is not signalled in the field 
 * Rate adaption then the contents of this field shall be ignored.
 */
typedef enum OtherRateAdaption
{
    /** V.120 */
    V120_RATE_ADAPTION = 0,
    /** H223_H245_RATE_ADAPTION */
    H223_H245_RATE_ADAPTION = 1,
    /** PIAFS_RATE_ADAPTION */
    PIAFS_RATE_ADAPTION = 2
    /* all other values are reserved */
}
OtherRateAdaption;

/** Identifies the type of subaddress */ 
typedef enum SubAddressTypeTag
{
    /** NSAP (X.213/ISO 8348 AD2) */                   
    SUBADDR_NSAP_X213           =   0,
    /** User specified  */                             
    SUBADDR_USER_SPECIFIED      =   2
}
SubAddressType;

/** 0-9 and 15 (for unused) */
typedef Int8
Bcd;                        

/** Defines the rate and version of speech */
typedef enum SpeechVersionTag
{
    /** GSM full rate speech version 1*/
    FULL_RATE_SPEECH_VERSION_1 = 0,
    /** GSM full rate speech version 2*/
    FULL_RATE_SPEECH_VERSION_2 = 2,
    /** GSM full rate speech version 3*/
    FULL_RATE_SPEECH_VERSION_3 = 4,
    /** GSM half rate speech version 1*/
    HALF_RATE_SPEECH_VERSION_1 = 1,
    /** GSM half rate speech version 2*/
    HALF_RATE_SPEECH_VERSION_2 = 3,
    /** GSM half rate speech version 3*/
    HALF_RATE_SPEECH_VERSION_3 = 5,
    /** TBD */
    SPEECH_VERSION_TBD         = 255
}
SpeechVersion;

#define NUM_SPEECH_VERSIONS    6

/** Indicates Speech version */
typedef struct SpeechVersionIndTag
{
    /** Indicates the number of versions */
    Int8            numVersions;
    /** Defines the rate and version of speech */
    SpeechVersion   version[NUM_SPEECH_VERSIONS];
}
SpeechVersionInd;

/** Defines any other modem type specified */
typedef enum OtherModemTypeTag
{
    /** no other modem type specified in this field */
    OTHER_MODEM_TYPE_NONE             =   0,
    /** V.34  */
    OTHER_MODEM_TYPE_V34              =   2
}
OtherModemType;

/** Indicates fixed network user rate */
typedef enum FixedNetworkUserRateTag
{
  /** Fixed network user rate not applicable/No 
   * meaning is associated with this value.*/
  INVALID_FNUR    = 0,
  /** 9600 bit/s Recommendation X.1 and V.110 */
  FNUR_9600       = 1,
  /** 14400 bit/s Recommendation X.1 and V.110*/
  FNUR_14400      = 2,
  /** 19200 bit/s Recommendation X.1 and V.110*/
  FNUR_19200      = 3,
  /** 28800 bit/s Recommendation X.1 and V.110 */
  FNUR_28800      = 4,
  /** 38400 bit/s Recommendation X.1 and V.110 */
  FNUR_38400      = 5,
  /** 48000 bit/s Recommendation X.1 and V.110(synch)*/
  FNUR_48000      = 6, /* gsm only */
  /** 56000 bit/s Recommendation X.1 and V.110(synch) /bit transparent */
  FNUR_56000      = 7,
  /** 64000 bit/s bit transparent */
  FNUR_64000      = 8,
  /** 33600 bit/s for UMTS only*/
  FNUR_33600      = 9, 
  /** 32600 bit/s */
  FNUR_32000      = 10,
  /** 31200 bit/s for UMTS only */
  FNUR_31200      = 11, 
  /** Number of elements in FixedNetworkUserRate */
  NUM_FNURS
}
FixedNetworkUserRate;

/** Indicates which channel coding is acceptable */
typedef enum AcceptableChannelCodingsTag
{
  /** TCH/F4.8-14.4-9.6 not acceptable */
  ACC_NONE              = 0,
  /** Only TCH/F4.8 acceptable*/
  ACC_4_8_ONLY          = 1,
  /** Only TCH/F9.6 acceptable*/
  ACC_9_6_ONLY          = 2,
  /** Only TCH/F4.8 and 9.6 acceptable*/
  ACC_4_8_AND_9_6_ONLY  = 3,
  /** Only TCH/F14.4 acceptable*/
  ACC_14_4_ONLY         = 8,
  /** Only TCH/F4.8 and 14.4 acceptable*/
  ACC_14_4_AND_4_8_ONLY = 9,
  /** Only TCH/F14.4 and 9.6 acceptable*/
  ACC_14_4_AND_9_6_ONLY = 10,
  ACC_ALL               = 11 /**< but see AcceptableChannelCodingsExtended */
}
AcceptableChannelCodings;

/** Indicates which extended channel coding is acceptable */
typedef enum AcceptableChannelCodingsExtendedTag
{
  /** None */
  ACCE_NONE               = 0,
  /** Only TCH/F43.2 acceptable*/
  ACCE_43_2_ONLY          = 1,
  /** Only TCH/F32 acceptable*/
  ACCE_32_0_ONLY          = 2,
  /** Only TCH/F43.2 and 32 acceptable*/
  ACCE_43_2_AND_32_0_ONLY = 3,
  /** Only TCH/F28.8 acceptable*/
  ACCE_28_8_ONLY          = 4,
  /** Only TCH/F43.2 and 28.8 acceptable*/
  ACCE_43_2_AND_28_8_ONLY = 5,
  /** Only TCH/F32 and 28.8 acceptable*/
  ACCE_32_0_AND_28_8_ONLY = 6,
  ACCE_ALL                = 7 /**< but see AcceptableChannelCodings */
}
AcceptableChannelCodingsExtended;

/** Indicates the number of traffic channels (octet 6e)*/
typedef enum MaxNumberOfTrafficChannelsTag
{
  /** 1 traffic channel */
  TCH_1     = 0,
  /** 2 traffic channels */
  TCH_2     = 1,
  /** 3 traffic channels */
  TCH_3     = 2,
  /** 4 traffic channels */
  TCH_4     = 3,
  /** 5 traffic channels */
  TCH_5     = 4,
  /** 6 traffic channels */
  TCH_6     = 5,
  /** 7 traffic channels */
  TCH_7     = 6,
  /** 8 traffic channels */
  TCH_8     = 7
}
MaxNumberOfTrafficChannels;

/** User initiated modification indication (octet 6f)*/
typedef enum UserInitiatedModificationTag
{
  /** User initiated modification not allowed/required*/
  UIMI_NOT_ALLOWED    = 0,
  /** User initiated modification up to 1 TCH/F allowed/may be requested */
  UIMI_TCH_1          = 1,
  /** User initiated modification up to 2 TCH/F allowed/may be requested */
  UIMI_TCH_2          = 2,
  /** User initiated modification up to 3 TCH/F allowed/may be requested */
  UIMI_TCH_3          = 3,
  /** User initiated modification up to 4 TCH/F allowed/may be requested */
  UIMI_TCH_4          = 4
}
UserInitiatedModification;

/** Wanted air interface user rate (octet 6f)*/
typedef enum WantedAirInterfaceUserRateTag
{
    /** Air interface user rate not applicable/No 
     * meaning associated with this value
     */
    INVALID_AIUR       = 0,
    /** 9.6 kbit/s */
    AIUR_9600          = 1,
    /** 14.4 kbit/s */
    AIUR_14400         = 2,
    /** 19.2 kbit/s */
    AIUR_19200         = 3,
    /** 28.8 kbit/s */
    AIUR_28800         = 5,
    /** 38.4 kbit/s */
    AIUR_38400         = 6,
    /** 43.2 kbit/s */
    AIUR_43200         = 7,
    /** 57.6 kbit/s */
    AIUR_57600         = 8
}
WantedAirInterfaceUserRate;

/** The bearer capability is a type 4 information element with a 
 * minimum length of 3 octets and a maximum length of 15 octets.
 */
typedef struct BearerCapabilityTag
{
    /** Indicates the type of channel requirement (octet 3) */
    ChannelRequirement              channelRequirement;
    /** Indicates if Packet or circuit mode */
    Boolean                         packetMode;
    /** Provides transfer capability information */
    TransferCapability              transferCapability;
    /** Indicates if Speech version id present */
    Boolean                         speechVersionPresent;
    /** Defines Speech version indication  */
    SpeechVersionInd                speechVersionInd;
#if defined (UPGRADE_CTM)
    /** Indicates if CTM text telephony is supported */
    Boolean                         ctm;
#endif /* UPGRADE_CTM */
    /* octet 4 if channel requirement is not speech */
    /** data compression allowed */
    Boolean                         dataCompressionAllowed;
    /** Determines the structure */
    DataStructure                   structure;
    /** Indicates if full or half duplex mode */
    Boolean                         fullDuplexMode;
    /** Negotiation of Intermediate Rate Requested */
    Boolean                         nirr;       /* FR 9803-2115 */
    /** octet 5 if channel requirement is not speech */
    RateAdaption                    rateAdaption;
    /** Access Protocol */
    SigAccessProtocol               sigAccessProtocol;
    /* octet 5a.  For conditions for when these are encoded see
       24.008-3b0 10.5.4.5.1 */
    /** otherItc only applicable if transferCapability is Other */
    OtherItc                        otherItc;
    /** otherRateAdaption is only applicable if rateAdaption is Other */
    OtherRateAdaption               otherRateAdaption;
    /* octet 5b: V.120 stuff */
    /** Indicates if rate adaption header included */
    Boolean                         hdr;
    /** Indicates if Multiple frame establishment supported */
    Boolean                         multiframe;
    /** Defines mode of operation */
    ModeOfOperation                 modeOfOperation;
    /** Logical link identifier negotiation */
    Lli                             lli;
    /** Assignor/Assignee */
    AssignorAssignee                assignorAssignee;
    /** In band/Out of band negotiation  */
    InBandOutOfBand                 inBandOutOfBand;
    /** octet 6 if channel requirement is not speech */
    Boolean                         isAsynchronous;
    /** octet 6a if channel requirement is not speech */
    NoOfStopBits                    noOfStopBits;
    /** Number of data bits excluding parity bit if present */
    NoOfDataBits                    noOfDataBits;
    /** Defines the user rate */
    UserRate                        userRate;
    /** octet 6b if channel requirement is not speech */
    IntermediateRate                intermediateRate;
    /** Requires to send data with network independent clock */
    Boolean                         requiresToTxWithNic;
    /** can accept data with network independent clock 
     * (sender does support this optional procedure) 
     */
    Boolean                         canRxWithNic;
    /** Parity information */
    ParityInformation               parityInformation;
    /** octet 6c if channel requirement is not speech */
    ConnectionElement               connectionElement;
    /** Indicates modem type */
    ModemType                       modemType;
    /*-----------------3/10/99 11:37AM-----------------*/
    /** This is set TRUE if the multislot bearers below are
     * present.
     */
    Boolean                         multislotPresent;
    /** octet 6d if channel requirement is not speech */
    OtherModemType                  otherModemType;
    /** Fixed network user rate (octet 6d) */
    FixedNetworkUserRate            fixedNetworkUserRate;
    /** octet 6e if channel requirement is not speech */
    AcceptableChannelCodings        acceptableChannelCodings;
    /** Indicates the maximum nuber of traffic channels */
    MaxNumberOfTrafficChannels      maxNumberOfTrafficChannels;
    /** octet 6f if channel requirement is not speech */
    Boolean                         octet6fPresent;
    /** User initiated modification indication*/
    UserInitiatedModification       userInitiatedModification;
    /** Wanted air interface user rate */
    WantedAirInterfaceUserRate      wantedAirInterfaceUserRate;
    /* octet 6g */
    /** octet6gPresent determines whether 6g is encoded, however */
    /* it will not be encoded if octet6fPresent is FALSE */
    Boolean                         octet6gPresent;
    /** Determines the extended channel coding */
    AcceptableChannelCodingsExtended acceptableChannelCodingsExtended;
    /** Indicates type of asymmetry */
    AsymmetryIndication             asymmetryIndication;
    /** octet 7 optional if channel requirement is not speech 
\assoc PRESENT \ref layer2Protocol */
    Boolean                         layer2ProtocolPresent;
    /** User information layer 2 protocol */
    Layer2Protocol                  layer2Protocol;
}
BearerCapabilityElement,
/** A transmission function which the UE requests to the network */
BearerCapability;

#if defined (UPGRADE_REL5)
/** BackupBearerCapability element.
 * The bearer capability is a type 4 information element
 */
typedef struct BackupBearerCapabilityTag
{
    /** octet 3 */
    Boolean                         packetMode;
    /** Determines the transfer capability */
    TransferCapability              transferCapability;
    /** octet 4 if channel requirement is not speech */
    DataStructure                   structure;
    /** Halh or full duplex mode */
    Boolean                         fullDuplexMode;
    /** octet 5 if channel requirement is not speech */
    RateAdaption                    rateAdaption;
    /** Determines the signalling acces protocol */
    SigAccessProtocol               sigAccessProtocol;
    /** octet 5a.  For conditions for when these are encoded see
     * 24.008-3b0 10.5.4.5.1 */
    /** otherItc only applicable if transferCapability is Other */
    OtherItc                        otherItc;
    /** otherRateAdaption is only applicable if rateAdaption is Other */
    OtherRateAdaption               otherRateAdaption;
    /** octet 6 if channel requirement is not speech */
    Boolean                         isAsynchronous;
    /** octet 6a if channel requirement is not speech */
    NoOfStopBits                    noOfStopBits;
    /** Number of data bits */
    NoOfDataBits                    noOfDataBits;
    /** Specifies the user rate */
    UserRate                        userRate;
    /** octet 6b if channel requirement is not speech */
    IntermediateRate                intermediateRate;
    /** octet 5b V110, Requires to send data with network independent clock */ 
    Boolean                         requiresToTxWithNic;
    /** Indicates if can accept data with Network Independent Clock 
     * (sender does support this optional procedure)
     */
    Boolean                         canRxWithNic;
    /** Determines if odd or even parity */
    ParityInformation               parityInformation;
    /** octet 6c if channel requirement is not speech */
    ConnectionElement               connectionElement;
    /** Specifies the modem type */
    ModemType                       modemType;
    /**
      This is set TRUE if the multislot bearers below are
      present.
     */
    Boolean                         multislotPresent;
    /* octet 6d if channel requirement is not speech */
    OtherModemType                  otherModemType;
    /** Fixed network user rate */
    FixedNetworkUserRate            fixedNetworkUserRate;
    /** octet 6f if channel requirement is not speech */
    UserInitiatedModification       userInitiatedModification;
    /** octet 7 optional if channel requirement is not speech */
    Boolean                         layer2ProtocolPresent;
    /** Layer2 protocol to use*/
    Layer2Protocol                  layer2Protocol;
}
BackupBearerCapabilityElement,
BackupBearerCapability;
#endif

/** subaddress information element. Indicates the type of subaddress
 * and lists its contents */
typedef struct SubaddressTag
{
    /** Length of subaddress contents 
\assoc ARRAY \ref info */
    Int8                            infoLength;
    /** Identifies the type of subaddress */ 
    SubAddressType                  type;
    /** Odd/even indicator */
    OddEvenIndicator                oddOrEven;
    /** Lists Subaddress information */           
    Int8                            info[MAX_SUB_NO_LENGTH];
}
CalledPartySubaddressElement,
/** Calling party subaddress element. */
CallingPartySubaddressElement,
/** The purpose of the connected subaddress information element 
 * is to identify a subaddress associated with the connected 
 * party of a call.
 */
ConnectedSubaddressElement,
/** The purpose of the Called party subaddress is to identify the subaddress of the 
 * called party of a call. For the definition of a subaddress see Rec. ITU-T I.330.
 */
CalledPartySubaddress,
/** The purpose of the Calling party subaddress is to identify a subaddress 
 * associated with the origin of a call. For the definition of a subaddress 
 * see Rec. ITU-T I.330.
 */
CallingPartySubaddress,
/** Identifies a subaddress associated with the redirecting party. */
RedirectingPartySubAddress,
/** Identifies a subaddress associated with the connected party of a call.*/
ConnectedSubaddress;

#endif
/* END OF FILE */





























