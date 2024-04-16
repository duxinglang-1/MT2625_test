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
 *  File Description : DM NV Manager Task Configuration
 **************************************************************************/
#ifndef DMNVMCFG_H
#define DMNVMCFG_H

#include <system.h>

# if defined (USE_TTP_AL)

/***************************************************************************
 * Include Files
 **************************************************************************/
#include <kitqid.h>    /* For TaskId, QueueId types */
#include <ut_codec.h>
#include <mmr_sig.h>
#include <mncb_sig.h>
#include <abem_typ.h>
#include <absi_sig.h>
#include <abmm_sig.h>
#include <abcb_typ.h>
#ifndef GEN_NVDM_DATABASE_FOR_PC
#include <abcc_typ.h>
#include <ablm_typ.h>
#endif
#if defined (UPGRADE_NASMDL2)
#include <abpd_sig.h>
#endif /* UPGRADE_NASMDL2 */
#include <abgl_typ.h>
#include <absm_typ.h>
#include <abrpm_typ.h>

/***************************************************************************
 * Defines
 **************************************************************************/

/*0202-15445 Definitions for APN storage file */
#define MAX_NUM_PPP_USER_DATA    1 /* Max number of name password pairs for PPP */
#if defined (UPGRADE_NASMDL2)
/* TODO: This value can be removed once ATCI has been updated not to use it */
#define MAX_NUM_APN_RECORDS      1 /* Max number of APNs stored in NVRAM.  Must always be 1  */
#else /* UPGRADE_NASMDL2 */
#if defined (UPGRADE_NVRAM_OPTIMIZE)
#define MAX_NUM_APN_RECORDS      0 /* Max number of APNs stored in NVRAM  */
#else
#define MAX_NUM_APN_RECORDS      5 /* Max number of APNs stored in NVRAM  */
#endif
#endif /* else UPGRADE_NASMDL2 */

/*NOTE: This is the maximum packed BCD number length which can be stored */
/*      in NVRAM                                                    */
/*0211-21755 Inrease the dial number length that can be stored in NVRAM. Note
 *that this constant is client configurable for BL object customers
 */
#define NVRAM_MAX_DIAL_NUMBER_LENGTH SIM_MAX_DIAL_NUMBER_LENGTH

/*0003-6232 Array of MEP networks now stored here so that the size of the array
 * can be changed by object only customers. The constant below may be changed
 * in order to vary the number of networks to which the ME can be locked.
 */
#define NUM_MEP_NETWORKS     3


/****************************************************************************
 * Macros
 ****************************************************************************/
/* none */

 /****************************************************************************
 * Type Definitions
 ****************************************************************************/

typedef struct AbsiMwiDataTag
{
    /** The Message Waiting Information File */
    SimMBMsgWaitingInfo     msgWaitingInfo[SIM_NUM_MAILBOX_INFO_GROUPS];
    /** Additional CPHS Auxiliary Voice message waiting indication*/
    Boolean                 msgWaitingAux;
    /** Additional CPHS Data message waiting indication*/
    Boolean                 msgWaitingData;
} AbsiMwiData;

/* NVRAM data for ABSM */
typedef struct SmsParametersTag
{
  Int8              validityPeriod; /*CR:9909-5176*/
  SmsAddress        scAddr;
  ProtInterWorking  protocolId;
} SmsParameters;


typedef struct AbsmDataAreaTag
{
  Int8        msgRef;    /* used by SMS when Phase 1 SIM */
  Boolean     memCap;
  SmsRoute    smsRoute;
  /* smsParameters file */
  SmsParameters smsParameters;

  Boolean     localMemCapFull;
  Boolean     smssAvailable;
  Boolean     smsCountFinished;
  Int8        freeRecordCount;
  Int8        numberOfSmsRecords;
  SimSmRecordStatus  smRecordStatus [MAX_SMS_RECORD];
}
AbsmDataArea;

typedef struct AbshDataAreaTag
{
  Int16    randomiseSeed;      /* used to seed the random number generator on power up */
}
AbshDataArea;

typedef struct AbmmWriteableDataTag
{
    Boolean               manualModeEngaged;
    SimPlmn               plmn;              /* The manually selected PLMN */
    ServiceType           serviceType;       /* Last service type: CS, GPRS or COMBINED */
    ApexMmGAPowerUpOption gaPwOnOption;      /* GPRS Attach At Power On settings */
    Boolean               powerUpProtoStack; /* 0107-11639 if true then protocol stack not started at power on */
    Boolean               powerUpSim;        /* 0205-17817 */
    EutraBand             usedBands[MAX_SUPPORTED_NBIOT_BAND_NUM];         /* last used band(s) from access stratum */
}
AbmmWriteableData;

typedef struct ManualPlmnDataTag
{
    Boolean     manualModeEngaged;
    Plmn        plmn;
}
ManualPlmnData;

typedef struct UserDefPlmnListTag
{
  PlmnId        nvramPlmnList[NVRAM_PLMN_LIST_SIZE];
}
UserDefPlmnList;

typedef struct EquivPlmnsTag
{
    Boolean            ePlmnValid;
    EquivalentPlmn     plmn;            /* PLMN on which list was received */
    EquivalentPlmnList ePlmnList;       /* up to 5 equivalent PLMNs */
}
EquivPlmns;


#if defined(UPGRADE_TWGGE_NRW)

typedef struct MeEhplmnTag
{
    SimPlmn                 simPlmn;
    AccessTechnologyId      perferredRat;
    AccessTechnologyId      notPerferredRat;
}MeEhplmn;

typedef struct MeEhplmnListTag
{
    Int8     numEhplmn;
    MeEhplmn ehplmnList[MAX_NUM_EHPLMN_LIST_IN_NVRAM];
}
MeEhplmnList;

#else

#if defined(ENABLE_EHPLMN) && defined(ENABLE_CMCC_FEATURE)
typedef struct MeEhplmnTag
{
    SimPlmn                 simPlmn;
    AccessTechnologyId      perferredRat;
    AccessTechnologyId      notPerferredRat;
}MeEhplmn;

typedef struct MeEhplmnListTag
{
    Int8     numEhplmn;
    MeEhplmn ehplmnList[MAX_NUM_EHPLMN_LIST_IN_NVRAM];
}
MeEhplmnList;
#endif

#  if defined (UPGRADE_3G)

#    if defined (UPGRADE_LTE)
typedef struct EutraListDataTag
{
  EutraListForMcc    nvramEutraListForHplmn;
  EutraListForMcc    nvramEutraListForMcc[NVRAM_NUM_EUTRA_LISTS];
}
EutraListData;

#    endif /* (UPGRADE_LTE) */

#  endif /* UPGRADE_3G */
#endif

typedef struct CbSettingsTag
{
    Language          lpList[MAX_LP_ENTRIES];
    Boolean           isCbDisabled;       /* 0204-17030 TRUE implies CB is disabled whatever the CBMI list */
}
CbSettings;

/** The data contains in a record of the CBMI file*/
typedef struct CbCmbiFileRecordTag
{
    /** The CBMI filter list bitmap.
    *   One byte per CBMI, set to 1 if we want receive messages with such messageId*/
    Int16   cbmiList[CBMI_PAGE_SIZE];
}
CbCmbiFileRecord;

typedef struct BandOptionsTag
{
  BandMode    bandMode[NUM_OF_BAND_OPTIONS]; /* The modes of operation the user is allowed    */
                                             /* to select (set to INVALID_BAND_MODE if empty).*/
}
BandOptions;

#if !defined (UPGRADE_NASMDL2)
typedef struct PppAuthenticationTag
{
  Int8    numberOfUsers;
  PppUser pppUser [MAX_NUM_PPP_USER_DATA];
} PppAuthentication;
#endif /* !UPGRADE_NASMDL2 */

/* Fatal Handset Error log structures */
typedef struct FatalErrorTag
{
  AlErrorClass  alErrorClass;
  AlErrorCode   alErrorCode;
  ProcId        procId;
}
FatalError;


/* an Alpha Identifier for a Dial Number */
typedef struct Anrm2AlphaIdentifierTag
{
/** \assoc ARRAY \ref data */
    Int8        length;
    Int8        data[MAX_AL_DN_ALPHA_TAG + 1];
}
Anrm2AlphaIdentifier;

/* format of a Dial Number as stored in NVRAM. The format can be changed by */
/* the removal of certain fields to fit more numbers into the NVRAM space   */
/* available                                                                */

typedef struct Anrm2AbbreviatedDialNumberTag
{
  Anrm2AlphaIdentifier             alphaId;
  /* set FALSE if SSC string etc. */
  Boolean                         isDiallingNumber;
  /* Type of Number (TON) */
  BcdNumberType                   typeOfNumber;
  /* Numbering Plan Id (NPI) */
  BcdNumberPlan                   numberPlan;
/** \assoc ARRAY \ref dialString */
  Int8                            dialStringLength;
  Bcd                             dialString[NVRAM_MAX_DIAL_NUMBER_LENGTH];
#  if defined (ANRM_DN_BEARER_CAP)
  Boolean                         bearerCapabilityDefined;
  BearerCapability                bearerCapability;
#  endif /* (ANRM_DN_BEARER_CAP) */
#  if defined (ANRM_DN_SUB_ADDRESS)
  Boolean                         subAddressDefined;
  CalledPartySubaddress           calledPartySubaddress;
#  endif /* (ANRM_DN_SUB_ADDRESS) */
  Imsi                            imsi; /* 0006-6836 */
}
Anrm2AbbreviatedDialNumber;

/* format of a Dial Number as stored in NVRAM. The format can be changed by */
/* the removal of certain fields to fit more numbers into the NVRAM space   */
/* available                                                                */
typedef struct Anrm2AbbreviatedDialNumber2Tag
{
  Anrm2AlphaIdentifier             alphaId;
  /* set FALSE if SSC string etc. */
  Boolean                         isDiallingNumber;
  /* Type of Number (TON) */
  BcdNumberType                   typeOfNumber;
  /* Numbering Plan Id (NPI) */
  BcdNumberPlan                   numberPlan;
  Boolean                         hiddenEntry;
  Int8                            dialStringLength;
  Bcd                             dialString[NVRAM_MAX_DIAL_NUMBER_LENGTH];
#  if defined (ANRM_DN_BEARER_CAP)
  Boolean                         bearerCapabilityDefined;
  BearerCapability                bearerCapability;
#  endif /* (ANRM_DN_BEARER_CAP) */
#  if defined (ANRM_DN_SUB_ADDRESS)
  Boolean                         subAddressDefined;
  CalledPartySubaddress           calledPartySubaddress;
#  endif /* (ANRM_DN_SUB_ADDRESS) */
  RtcDate                         logDate;
  RtcTime                         logTime;
  Imsi                            imsi; /* 0006-6836 */
}
Anrm2AbbreviatedDialNumber2;

/*0003-6232 Include NVRAM block that stores the MEP data, RAM copy is now different*/
/*0105-10503 Move position of the networkIds to the end of the structure so that it
 * can be of "variable" lenght. That is, the BL library can be built with one size
 * and ANRM2 can be built with a different size.
 */
typedef struct MePersonalisationDataTag
{
  Int8      personalisationsActivated; /* Bit field - masks required to select personalisations */
  Int8      personalisationsEnabled;   /* Bit field - masks required to select personalisations */
  Password  personalisationKeys [NUM_MEP_PERSONALISATIONS];
  Int8      keyRetries[NUM_MEP_PERSONALISATIONS];  /* no of retries remaining */
  Bcd       networkSubsetId[NUM_MEP_SUBSET_DIGITS];
  Int8      serviceproviderId;
  Int8      corporateId;
  Imsi      simId;
  SimPlmn   networkIds[NUM_MEP_NETWORKS];
} MePersonalisationData;

#if defined(UPGRADE_TWGGE_NRW)


typedef struct Anrm2Rel6DynamicsCapTag
{
    /** Whether E-DCH transport channels are supported*/
    Boolean edchSupported;
    /** Whether FDPCH are supported*/
    Boolean fdpchSupported;
}Anrm2Rel6DynamicsCap;

#else
#if defined (UPGRADE_3G_FDD) && defined(UPGRADE_REL6) && defined(REL6_DYNAMICS_CAPABILITIES)
typedef struct Anrm2Rel6DynamicsCapTag
{
    /** Whether E-DCH transport channels are supported*/
    Boolean edchSupported;
    /** Whether FDPCH are supported*/
    Boolean fdpchSupported;
}Anrm2Rel6DynamicsCap;
#endif
#endif
/******************************************************************************
 * For MMI2 Phase 2 - We keep track of the last 3 SIM cards which we have seen
 *****************************************************************************/
#define MAX_KNOWN_SIMS  3
typedef struct KnownSimIdentTag
{
  Boolean   valid;          /* This SIM entry is valid  */
  Imsi      simId;          /* Unique Identifier of SIM */
}KnownSimIdent;

typedef struct KnownSimDataTag
{
#if defined (ENABLE_DUAL_SIM_SOLUTION) || defined(UPGRADE_TWGGE_NRW)
  Int8          numSimHolders;
  SimHolderType simHolder;   /* which of the SIm Holders was last selected */
#endif
  Int8          simHistory[MAX_KNOWN_SIMS];
  KnownSimIdent simIdent[MAX_KNOWN_SIMS];
}KnownSimData;

/******************************************************************************
 * Data to keep track of SMS Timestamps for outgoing messages
 *****************************************************************************/
typedef struct SmsTimeStampRecordTag
{
  Int8          simOwner;       /* Index of owning SIM */
  Int8          recordNumber;   /* SIM Record number   */
  RtcDate       recordDate;     /* Date                */
  RtcTime       recordTime;     /* Time                */
}
SmsTimeStampRecord;

typedef struct SmsTimeStampDataTag
{
  SmsTimeStampRecord    timeStampRecord[MAX_SMS_TIMESTAMPS];
}
SmsTimeStampData;

/****************************************************************************
 * Variables
 ****************************************************************************/


/****************************************************************************
 * Functions
 ****************************************************************************/
/* none */

# endif /* (USE_TTP_AL) */
#endif /* !(DMNVMCFG_H) */
/* END Of FILE */

