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
 * **************************************************************************
 *  File Description :
 **************************************************************************/

#ifndef SIMDATA_H
#define SIMDATA_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <ki_typ.h>
#include <gkiqueue.h>

#if !defined (MMSI_SIG_H)
#include    <mmsi_sig.h>
#endif

#if !defined (ALSI_SIG_H)
#include    <alsi_sig.h>
#endif

#if !defined (ALSA_SIG_H)
#include    <alsa_sig.h>
#endif


#if !defined (SIR_TYP_H)
#include    <sir_typ.h>
#endif

#if !defined (L1SI_SIG_H)
#include    <l1si_sig.h>
#endif


/***************************************************************************
 * Manifest Constants
 **************************************************************************/

#define SIM_SW_SIZE              2   /* Length of Status Words.         */

#define SIM_DATALEN_SELECT       2   /* \ Sizes for the data portion of */
#define SIM_DATALEN_STATUS       34  /* | command and response APDU`s   */
#define SIM_DATALEN_INCREASE     3   /* | (Application Protocol Data    */
#define SIM_DATALEN_VERIFY_CHV   8   /* | Units, see GSM 11.11 sec. 9.  */
#define SIM_DATALEN_CHANGE_CHV   16  /* |                               */
#define SIM_DATALEN_DISABLE_CHV  8   /* |                               */
#define SIM_DATALEN_ENABLE_CHV   8   /* |                               */
#define SIM_DATALEN_UNBLOCK_CHV  16  /* |                               */
#define SIM_DATALEN_RUN_GSM_ALG  16  /* /                               */

#define SIM_UNDEFINED_RECORD     0

#define SIM_STATUS_REQUEST_TIME     SECONDS_TO_TICKS(cfSimStatusRequestTime)
#define SIM_CLOCK_OFF_TIME          MILLISECONDS_TO_TICKS(10)
#if defined ( ENABLE_USIM_EMU_COOLART_SCRIPTS)
#define SIM_WAIT_SCRIPT_TIME          SECONDS_TO_TICKS(20)
#endif
#define SIM_UICC_PRESENCE_DETECTION_TIME SECONDS_TO_TICKS(60)

/* User values for identifying the three SIM manager timers */

#define SIM_CLOCK_ID                0x0010  /* Clock Off timer.        */
#define SIM_STATUS_ID               0x0020  /* Status request timer.   */
#define SIM_RESPONSE_ID             0x0030  /* Layer 1 response timer. */
#if defined ( ENABLE_USIM_EMU_COOLART_SCRIPTS)
#define SIM_WAITSCRIPT_ID           0x0040  /* Wait script send response timer. */
#endif
#if defined (UPGRADE_REL8_USIM)
# if defined (TEST_EXTERNAL_TIMER)
#define SIM_EXTERNAL_LOCK_TIME      SECONDS_TO_TICKS(30)/* external lock applied for up to 30 seconds*/
# else
#define SIM_EXTERNAL_LOCK_TIME      SECONDS_TO_TICKS(3)/* external lock only applied for up to 3 seconds*/
# endif /* test external timer */
#define SIM_LOCK_ID                 0x0050    /* guard timer for external lock */
#endif
#define SIM_PRESENCE_ID             0x0060    /* presence detection timer */

#define SIM_WAIT_DS_ID              0x0070    /* psc prepare to deep sleep timer */

#define SIM_WAIT_DS_TIME            SECONDS_TO_TICKS(60)/* deep sleep prepare time out 60 seconds*/

#define  SIM_LAC_DELETED_PHASE1  0x0000       /* FR00387 */
#define  SIM_LAC_DELETED_PHASE2  0xfffe       /* FR00387 */

#define  MOBILE_LAC_DELETED      0xfffe       /* FR00387 */

#define SIM_CPHS_INFO_NUM_ALPHA_SIZE_INVALID   0xFF

/* defines for the size of parameters used in the
 * authentication proceedure  - moved from usimemu.c as info is needed for EPS authentication checks*/
#define RAND_SIZE    16                   /* 16 */
#define AUTH_SIZE    RAND_SIZE            /* 16 */
#define AMF_SIZE     2                    /* 2 */
#define MAC_SIZE     8                    /* 8 */
#define XDOUT_SIZE   RAND_SIZE            /* 16 */
#define CDOUT_SIZE   MAC_SIZE             /* 8 */
#define SQN_SIZE     MAC_SIZE - AMF_SIZE  /* 6 */
#define AK_SIZE      SQN_SIZE             /* 6 */
#define AUTS_SIZE    SQN_SIZE + MAC_SIZE  /* 14 */

#if defined ( ENABLE_USIM_RTC)
/* define rtc ram data in simData */
#define SIM_MANAGER_RTC_RAM_DATA \
    Int8                sim_manager_start_of_rtc_data;\
    TaskId              simAppToolkitTaskId;\
    Int16               changeTargetCommandRef;\
    SimUiccAppData      uiccAppBasicChannel;\
    Boolean             cardIsUicc;\
    SimUiccPinStatus    universalPin;\
    SimUiccPinStatus    unblockUniversalPin;\
    SimUiccPinStatus    pinAppStatus[MAX_NUM_PIN_APP];\
    SimUiccPinStatus    unblockPinAppStatus[MAX_NUM_PIN_APP];\
    SimVoltageCapab     simVoltageCapab;\
    SimUiccSecEnvironment currentSe;\
    Int16               adfUsimId;\
    SimRemovedCause     removeCause;\
    Boolean             stkCard;\
    SimAdminData        admin;\
    AccessClass         accessClass;\
    Boolean             fdnEnabled;\
    Boolean             readWriteInvalidatedAdn;\
    Boolean             bdnEnabled;\
    Boolean             aclEnabled;\
    Boolean             ehplmnListValid;\
    Int16               totalNumEhplmns;\
    SimRecord           currSimRecord;\
    Boolean             extRecordChecked;\
    Band                currentBand;\
    Int8                fatalCounter;\
    Boolean             eDRXAllowed;\
    SimUiccResumeToken  resumeToken;\
    Boolean             epsLocInfoValid;\
    UsimEpsLocInfo      epsLocInfo;\
    Boolean             eDrxEnabled;\
    Boolean             psmDataFull;\
    Int32               end_of_psm

/* define nv ram data in simData */
#define SIM_MANAGER_NV_RAM_DATA\
    Int8                sim_manager_start_of_nv_data;\
    SimUiccPhoneBook    globalPhoneBook;\
    ServiceTable        serviceTable;\
    Imsi                imsi;\
    SimIccid            iccid;\
    SimPlmn             actingHplmn;\
    SimPlmnList         ehplmn;\
    Boolean             uiccSuspensionAllowed;\
    Boolean             universalPinSupportedByCard;\
    Int8                oplRecordsNum;\
    Int16               aclDataLength;\
    Int16               totalNumPlmns;\
    Int16               totalNumFplmns;\
    Int16               totalNumOplmns;\
    Int16               totalNumHplmns;\
    Int16               totalNumEhplmnsInUicc;\
    SimatSetupMenuInfo  simatSetupMenuInfo;\
    Boolean             DTagInNVRam;\
    Int32               end_of_nv
#else
/* define nv ram data in simData */
#define SIM_MANAGER_NV_RAM_DATA\
    Int8                sim_manager_start_of_nv_data;\
    SimUiccPhoneBook    globalPhoneBook;\
    ServiceTable        serviceTable;\
    Imsi                imsi;\
    SimIccid            iccid;\
    SimPlmn             actingHplmn;\
    SimPlmnList         ehplmn;\
    Boolean             uiccSuspensionAllowed;\
    Boolean             universalPinSupportedByCard;\
    Int8                oplRecordsNum;\
    Int16               aclDataLength;\
    Int16               totalNumPlmns;\
    Int16               totalNumFplmns;\
    Int16               totalNumOplmns;\
    Int16               totalNumHplmns;\
    Int16               totalNumEhplmnsInUicc;\
    SimatSetupMenuInfo  simatSetupMenuInfo;\
    Boolean             DTagInNVRam;\
    TaskId              simAppToolkitTaskId;\
    Int16               changeTargetCommandRef;\
    SimUiccAppData      uiccAppBasicChannel;\
    Boolean             cardIsUicc;\
    SimUiccPinStatus    universalPin;\
    SimUiccPinStatus    unblockUniversalPin;\
    SimUiccPinStatus    pinAppStatus[MAX_NUM_PIN_APP];\
    SimUiccPinStatus    unblockPinAppStatus[MAX_NUM_PIN_APP];\
    SimVoltageCapab     simVoltageCapab;\
    SimUiccSecEnvironment currentSe;\
    Int16               adfUsimId;\
    SimRemovedCause     removeCause;\
    Boolean             stkCard;\
    SimAdminData        admin;\
    AccessClass         accessClass;\
    Boolean             fdnEnabled;\
    Boolean             readWriteInvalidatedAdn;\
    Boolean             bdnEnabled;\
    Boolean             aclEnabled;\
    Boolean             ehplmnListValid;\
    Int16               totalNumEhplmns;\
    SimRecord           currSimRecord;\
    Boolean             extRecordChecked;\
    Band                currentBand;\
    Int8                fatalCounter;\
    Boolean             eDRXAllowed;\
    SimUiccResumeToken  resumeToken;\
    Boolean             epsLocInfoValid;\
    UsimEpsLocInfo      epsLocInfo;\
    Boolean             eDrxEnabled;\
    Boolean             psmDataFull;\
    Int32               end_of_nv
#endif

/***************************************************************************
 * Macros
 **************************************************************************/

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/*
-----------------------------------------------------------------------------
SimCommand enum:

These values reflect the command instruction codes used when sending
a command to the SIM. see GSM 11.11 section 9.2
-----------------------------------------------------------------------------
*/
typedef enum SimCommandTag
{
    SIM_COMM_SELECT             = 0xa4,
    SIM_COMM_STATUS             = 0xf2,
    SIM_COMM_SLEEP              = 0xfa,
    SIM_COMM_READ_BINARY        = 0xb0,
    SIM_COMM_UPDATE_BINARY      = 0xd6,
    SIM_COMM_READ_RECORD        = 0xb2,
    SIM_COMM_UPDATE_RECORD      = 0xdc,
    SIM_COMM_SEEK               = 0xa2,
    SIM_COMM_INCREASE           = 0x32,
    SIM_COMM_VERIFY_CHV         = 0x20,
    SIM_COMM_CHANGE_CHV         = 0x24,
    SIM_COMM_DISABLE_CHV        = 0x26,
    SIM_COMM_ENABLE_CHV         = 0x28,
    SIM_COMM_UNBLOCK_CHV        = 0x2c,
    SIM_COMM_INVALIDATE         = 0x04,
    SIM_COMM_REHABILITATE       = 0x44,
    SIM_COMM_RUN_GSM_ALGORITHM  = 0x88,
    SIM_COMM_MANAGE_CHANNEL     = 0x70,
    SIM_COMM_GET_CHALLENGE      = 0x84,
    SIM_COMM_GET_RESPONSE       = 0xc0,
    /* Phase 2+ cmmands for SIM Toolkit implementation */
    SIM_COMM_TERMINAL_PROFILE   = 0x10,
    SIM_COMM_ENVELOPE           = 0xC2,
    SIM_COMM_FETCH              = 0x12,
    SIM_COMM_TERMINAL_RESPONSE  = 0x14,
    /* Rel 14 features */
    SIM_COMM_UICC_SUSPEND       = 0x76
}
SimCommand;

/*only the files which may be read
 * using a mode different from SIM_REC_RWMODE_CURRENT_ABSOLUTE
 * need to be listed*/
typedef enum SimStoredInfoTag
{
    SIM_EF_INFO_PBR_LOCAL,   /* Under USIM directory*/
    SIM_EF_INFO_DIR,
    SIM_EF_INFO_PBR_GLOBAL,  /* Under TELECOM directory*/
    SIM_EF_INFO_ECC,
    SIM_EF_INFO_EXT5,
    SIM_EF_INFO_EXT7,
    SIM_EF_INFO_CFIS,
    SIM_EF_INFO_ICI,
    SIM_EF_INFO_OCI,
    SIM_EF_INFO_ICT,
    SIM_EF_INFO_OCT,
    SIM_EF_INFO_CCP2,
    SIM_EF_INFO_ARR,
    SIM_EF_INFO_ACM,
    SIM_EF_INFO_ADN,
    SIM_EF_INFO_FDN,
    SIM_EF_INFO_SMS,
    SIM_EF_INFO_CCP,
    SIM_EF_INFO_MSISDN,
    SIM_EF_INFO_SMSP,
    SIM_EF_INFO_LND,
    SIM_EF_INFO_EXT1,
    SIM_EF_INFO_EXT2,
    SIM_EF_INFO_CPS,
    SIM_EF_INFO_NIA,
    SIM_EF_INFO_SDN,
    SIM_EF_INFO_EXT3,
    SIM_EF_INFO_BDN,
    SIM_EF_INFO_EXT4,
    SIM_EF_INFO_SMSR,
    SIM_EF_INFO_CPHS_MN,                /* CPHS specific */
    SIM_EF_INFO_CPHS_INFO_NUM,          /* CPHS specific */
    SIM_EF_INFO_HZ_CACHE1,
    SIM_EF_INFO_HZ_CACHE2,
    SIM_EF_INFO_HZ_CACHE3,
    SIM_EF_INFO_HZ_CACHE4,
    SIM_EF_INFO_PNN,
    SIM_EF_INFO_OPL,
    SIM_EF_INFO_MBI,
    SIM_EF_INFO_MWIS,
    SIM_EF_INFO_MBDN,
    SIM_EF_INFO_EXT6,
    SIM_EF_INFO_NUM_ENTRIES
} SimStoredInfo;

typedef enum SimEfOperationsTag
{
    SIM_READ_BINARY,
    SIM_UPDATE_BINARY,
    SIM_READ_RECORD,
    SIM_UPDATE_RECORD,
    SIM_SEEK,
    SIM_INCREASE
} SimEfOperations;

typedef enum SimManagerStateTag
{
    SIM_MGR_NULL_STATE,
    SIM_MGR_IDLE_STATE,
    SIM_MGR_ACTIVE_STATE,
    SIM_MGR_HALF_AWAKE_STATE,
    SIM_MGR_HIDDEN_RESET_STATE
}
SimManagerState;

typedef enum USIMPSMStateTag
{
    SIM_MGR_PSM_STATE_NONE,
    SIM_MGR_PSM_STATE_WRITE,
    SIM_MGR_PSM_STATE_READ
}
USIMPSMState;

typedef enum PSMStateTag
{
    PSM_STATE_NORMAL_WAKEUP,      /* PSC_COLD_BOOT */
    PSM_STATE_DEEP_SLEEP_WAKEUP,  /* PSC_DEEP_SLEEP */
    PSM_STATE_DEEP_SLEEP_PREPARE, /* ready to go sleep */
    PSM_STATE_WARM_WAKEUP         /* normal run continues */
}
PSMState;

typedef enum SimatMenuStateTag
{
    SIMAT_MENU_NULL,        /* no menu */
    SIMAT_MENU_VALID,       /* setup menu received */
    SIMAT_MENU_FAKE_SEND    /* send fake menu to SIMAT */
}
SimatMenuState;

/* job 101859 structure added to handle change to Int16 for */
/* SimGenAccessReq path directories and file                */

typedef struct SimGenAccessSelPathTag
{
    Int16    currentRootDirId;
    Int16    currentDirId;
    Int16    currentEfId;

}
SimGenAccessSelPath;

/***************************************************************************
 * Type Definitions
 **************************************************************************/
/*
-----------------------------------------------------------------------------
SimSwStatus struct:

Whenever a command is transmitted to the SIM, the SIM returns two bytes
called SW1 and SW2. The contents and interpretation of these is command
specfic. This structure holds all the necessary data.
-----------------------------------------------------------------------------
*/
typedef struct SimSwStatusTag
{
    SimCommand      command;    /* the command resulting in SW1, SW2 */
    SimSwStatusCode status;     /* the decoded status code           */
    Int8            sw1;        /* Raw SW1                           */
    Int8            sw2;        /* Raw SW2                           */
}
SimSwStatus;


/*job 100935: if the AlsiListDialNumReq was interrupted, we keep the list of  the     */
/* valid entries we have read so far, so we don't have to read all the records again. */
/*The following structure is only used on GSM SIMs                                    */

typedef struct SimDialnumEntriesTag     /* job 103546 */
{
   SimDialNumberFile  efId;
   /*only valid on GSM SIMs*/
   Int16              lastEntryRead;
   Int16              validEntries [SIM_ALPHA_LIST_SIZE];
   Int8               numValidEntries;
}
SimDialnumEntries;

/* The following structure will contain the result of the SEARCH RECORD command   */
/* performed on a dialling number file.                                          */
/* SEARCH RECORD is not supported by GSM SIMS, thus this is valid for UICC only  */
typedef struct SimUiccInvalidEntriesTag
{
   /*following fields only valid on UICCs*/
   SimDialNumberFile   efId;
   Boolean             searchRecordDone;
   Int8                numInvalidEntries;
   Int8               *invalidRecords_p;
} SimUiccInvalidEntries;

typedef struct SimatSetupMenuInfoTag{
  SimatMenuState        menuReceivedState;
  SimDataBlock          cmdDataBlock;
  Int16                 byteOffset;
  /* command details */
  Int8                  commNumber;
  Int8                  commQualifier;
  Boolean               compReqd;   /* comprehension required flag */
  //SimatCommandDetails   commDetails;
}
SimatSetupMenuInfo;

/*
-----------------------------------------------------------------------------
SimManagerData struct:

This structure holds common data used by most of the procedures throughout
the SIM manager. Notably; this structure holds the queue ids and the Task
ids of the SIM manager task and other tasks with which it communicates
through the real time kernel interface.
-----------------------------------------------------------------------------
*/

typedef struct SimManagerDataTag
{
#if defined ( ENABLE_USIM_RTC)
/* Data block for RTC Ram data */
  SIM_MANAGER_RTC_RAM_DATA;
#endif
/* Data block for NV Ram data */
  SIM_MANAGER_NV_RAM_DATA;

  TaskId              myTaskId;           /* the sim manager processes task id  */
  QueueId             readQueueId;        /* the sim manager task signal queue  */

  TaskId              applicationTaskId;  /* the application task               */
  TaskId              layer1TaskId;       /* The layer 1 process                */
  TaskId              layer3MmTaskId;     /* The layer 3 MM process             */
  //TaskId              simAppToolkitTaskId;/* the SIM Application Toolkit task   */
  TaskId              originalLayer1TaskId; /* Storage for the original Layer 1 TaskId */
  //Int16               changeTargetCommandRef; /* Storage for command ref */
  SimManagerState     state;              /* Sim Manager current state          */
  KiUnitQueue         appQueue;           /* Storage for app layer signals      */
  KiUnitQueue         delayedQueue;       /* Storage for delayed app layer sigs */
  KiUnitQueue         mmQueue;            /* Storage for MM layer signals       */
  KiUnitQueue         l1Queue;            /* Storage for L1 layer signals       */
  KiUnitQueue         alsaReqQueue;       /* job 106677 : Storage for ALSA REQ signals       */
  KiUnitQueue         resetQueue;         /* Storage for hidden reset       */
#if defined (UPGRADE_REL8_USIM)
  KiUnitQueue         externalReqQueue;   /* queue for external access commands and lock on and off */
  KiTimer             externalLockTimer;  /* guard timer for external lock being on  - 3 seconds */
#endif
  KiTimer             statusRequestTimer; /* Timer to get SIM status every 30 s */
  KiTimer             clockOffTimer;      /* Timer to get SIM status every 30 s */
#if defined ( ENABLE_USIM_EMU_COOLART_SCRIPTS)
  KiTimer             waitScriptTimer;      /* Timer to wait script to send SIG_L1SI_SIM_IMAG_PTR_RSP to PS */
  Boolean             configSimDataRequest;    /* scripts request to config sim data for case */
#endif/*ENABLE_USIM_EMU_COOLART_SCRIPTS*/
  KiTimer             presenceDetectionTimer; /* Timer to presence detection; no network activity in 30 second go to sleep */
  KiTimer             waitPscDeepSleepTimer; /* psc requested stop timers, after a minute start timers */
  SimUiccAppData*     uiccApp[MAX_NUM_LOGICAL_CHANNELS]; /* applications on the UICC. Information specific to a logical channel */
  //Boolean             cardIsUicc;             /* Is this a UICC card or not ? */
  /*When a PIN is enabled on one logical Channel, it is also enabled on other channels. */
  /*Also, when a PIN is verified on one logical Channel, it is also verified on other channels. */
  //SimUiccPinStatus    universalPin;
  //SimUiccPinStatus    unblockUniversalPin;
  //SimUiccPinStatus    pinAppStatus[MAX_NUM_PIN_APP];         /* status of the application PINs (valid accross the SIM) */
  //SimUiccPinStatus    unblockPinAppStatus[MAX_NUM_PIN_APP];  /* status of the unblock PINs */
  //SimUiccPhoneBook    globalPhoneBook;    /*global phoneBook Info*/
  SimUiccPhoneBook    localPhoneBook;     /*local phoneBook Info under ADF_USIM directory*/
  //SimIccid            iccid;              /* need iccid to calculate PBId */
  //ServiceTable        serviceTable;       /* USIM service table, Info from EF UST or if GSM, Service from EF SST */
  Boolean             clockOffHigh;       /* clock stop mode allowed at high level */
  Boolean             clockOffLow;        /* clock stop mode allowed at low level  */
  Int8                appPowerConsumption;/* application power consumption */
  //SimVoltageCapab     simVoltageCapab;
#if defined (UPGRADE_REL6)
  Boolean             lowImpDriverAvail;  /* low impedence Driver available on I/O line. decoded but not currently used by ME */
#endif
  SimDirStatusData    dfInUseStatus;      /*the status of the current DF in use: i.e. USIM or GSM directory  */
  //SimUiccSecEnvironment currentSe;        /*job 101373: current Security Environement for USIM application*/
  //Int16               adfUsimId;          /*job 101373: Id of the ADF USIM: different on all SIMs*/
  SimUiccPath         simGenAccessPath;   /*currently selected file path for simGenAccessReq */
  //SimRemovedCause     removeCause;        /* Reason for sim removal             */
  Int8                layer1Reference;    /* layer 1 command reference          */
  SimClockFreq        clockFreq;          /* Frequency of SIM clock             */

  SimBaudRateFactor   baudRateFactor;
  SimClockRateFactor  clockRateFactor;
  Int8                            WI;
  SimUiccTransmissionProtocol     transProtocol;    /* job 100951 */
  Int8                            extraGuardTime;
  Int8                            cardInfoFieldSize;
  Int8                            charWaitingInt;
  Int8                            blockWaitingInt;
  //Boolean                         universalPinSupportedByCard;    /*does the card support universal PIN ?*/
  //Boolean                         stkCard;  /*does the card support a SIM toolkit application ?*/
  Boolean             clockOffAllowed;    /* Can SIM clock be switched off      */
  SimPhase            phase;              /* GSM phase capability of SIM        */
  AlsiReadNasConfigCnf nasConfig;         /* Nas config data */
  //SimAdminData        admin;              /* Admin data                         */
  //Imsi                imsi;               /* International Mobile Subsriber Id  */
  //SimPlmn             actingHplmn;        /* MNC and MCC + flag to indicate whether MNC is on 2 or 3 digits   */

  //AccessClass         accessClass;        /* Data from ACC file                 */
  //Boolean             fdnEnabled;         /* Fixed Dialing Number status        */
  //Boolean             readWriteInvalidatedAdn;
  //Boolean             bdnEnabled;         /* Barred Dialling Number Status      */
  //Boolean             aclEnabled;         /*Access point names Control List*/
#if defined (UPGRADE_REL8_USIM)
  Boolean             csimLocked;
#endif
//# if defined (ENABLE_EHPLMN)
  //Boolean             ehplmnListValid;
  //Int16               totalNumEhplmns;
  //SimPlmnList         ehplmn;
//# endif
  //SimRecord            currSimRecord;
  SimEfInfo            efInfo[SIM_EF_INFO_NUM_ENTRIES];
  /*the following contains information about the current record for different files*/
  SimUiccCurrRecordInfo currentRecordInfo;
  SimUiccInvalidEntries invalidEntries;
#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
  Int8                  totalEccsOnSim;  /*number of valid Eccs on SIM/USIM*/
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
  /*
  ** Request signal from AL or MM.
  */
  SignalBuffer        requestSignal;

  /*
  ** Response signal to AL or MM.
  */
  SignalBuffer        responseSignal;

  /*
  ** Signals used when processing the Task queue to find high priority
  ** signals.
  */
  SignalBuffer        endOfQueueSig;
  SignalBuffer        endOfQueueSigCopy;

  /*
  ** Data Block used for SIM commands.
  */
  SimDataBlock        cmdDataBlock;

  /*
  ** Flag to indicate that an MM request has been received while processing
  ** an ALSI request. The SIM manager will then terminate the ALSI request
  ** and process the MM request, the ALSI request is added to the delayed
  ** unit queue.
  */
  Boolean             pendingHighPriorityReq;

  /* Save result of check, is iccid same as in previous power off UICC iccid */
  Boolean             isUiccAsInPowerOff;

  /*
  ** Are we using SIM emu or SIM drv
  */
  Boolean             emulateSim;

  /*
  ** Is the SIM clock off
  */
  Boolean             simClockOff;

  /*
  ** SIM voltage indicated in an L1siCardPresentInd signal
  */
  SimVoltage          simVoltage;

  /*
  ** SIM Voltage levels supported by the SIM Driver/Interface. The SIM Driver sends this
  ** information at startup in an L1siConfigInd signal.
  */
  Boolean             simdrvSupportedVoltages[SIM_NUM_VOLTAGE_LEVELS];
#if defined (ENABLE_DUAL_SIM_SOLUTION)
/** the currently selected SIM Holder */
  SimHolderType       simHolder;
/** the number of SIM holders detected by the SIM drivers */
  Int8                numSimHolders;
  Boolean             waitingForSelectCnf;
  Int16               simSelectCommRef;
#endif
#if defined (SIM_ONLY_PURGE_ONCE)
  /*
  ** Bitmap to indicate whether or not the different SIMs extension record files have been
  ** purged this sesssion or not.
  ** bit1 set means that EXT1 has been purged
  ** bit2 set means that EXT2 has been purged
  ** bit3 set means that EXT4 has been purged
  */
  Int8             extFilesPurged;
#endif
  //Boolean          extRecordChecked;
  /*
  ** If a phase 1 DCS1800 SIM is inserted, need to
  ** look for data in the DCS directory of the SIM
  ** rather than the GSM directory.  This variable
  ** contains the identifier of the directory
  ** found and in active use on the SIM.
  */
  SimDirId            dfInUse;

  Boolean             connectionEstablished;  /* used to inform SIM if connection est. */

  Boolean             pendingFetch;              /* There is a pending FETCH command */
  Int8                pendingFetchSize;          /* Amount of data to FETCH */
  Boolean             proactivePollingSupported; /* is Polling supported? */
  Boolean             proactivePollingActive;    /* proactive polling is activated */
  FrameTicks          proactivePollingPeriod;    /* time between polling requests */
  /*
  ** Do we send STATUS Commands to SIM in Idle Mode for test cases 27.22.xx
  ** from GSM 11.10-4? (required for SIM Simulator to send proactive commands)
  */
  Boolean             forceStatusCommands;
  Boolean             mmiIconSupportAvailable; /* job108644: add indication of icon support */
  Boolean             enhNwkSelectSupported; /* SIM supports enhanced network selection?*/
#if defined (UPGRADE_LTE)
  /* If the USIM doesn't support EPS files, need to read and write to NVRAM */

  /* Waiting for Security data for MmSimReadDataCnf */
  Boolean             pendingNvEpsReadSecurityMmRsp;
  /* Waiting for Loci data for MmSimReadDataCnf */
  Boolean             pendingNvEpsReadLociMmRsp;
  /* Waiting for IMSI data for MmSimReadDataCnf */
  Boolean             pendingNvEpsReadImsiMmRsp;
  /* Used for collecting and storing the response data from SIM and NVRAM for MmSimReadDataCnf */
  MmSimReadDataCnf    *epsMmSimReadDataCnf_p;

  /* Waiting for write confimation of Security data for MmSimWriteDataCnf */
  Int8                pendingNvEpsWriteSecurityMmRsp;
  /* Waiting for write confimation of Loci data for MmSimWriteDataCnf */
  Int8                pendingNvEpsWriteLociMmRsp;
  /* Waiting for write confimation of IMSI data for MmSimWriteDataCnf */
  Int8                pendingNvEpsWriteImsiMmRsp;
  /* Used for storing the success flag for MmSimWriteDataCnf */
  MmSimWriteDataCnf   epsMmSimWriteDataCnf;

  /* Waiting for Loci data for AlsiAppInitialiseCnf */
  Boolean             pendingNvEpsReadLociAlsiRsp;
  /* Waiting for IMSI data for AlsiAppInitialiseCnf */
  Boolean             pendingNvEpsReadImsiAlsiRsp;
  /* Used for collecting and storing the response data from SIM and NVRAM for AlsiAppInitialiseCnf */
  AlsiAppInitialiseCnf  *alsiAppInitialiseCnf_p;
#endif

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
/* CPHS related fields */
  Boolean              cphsDataPresent;       /* CPHS data is supported on SIM */
  SimCphsPhase         cphsPhase;             /* CPHS phase capability of SIM */
  SimCphsServiceTable  cphsService;           /* info on optional CPHS files */
  SimEfId              cphsInfoNumFile;       /* Info Num file can be one of two values. */
  Int8                 cphsInfoNumAlphaSize;  /* Need to know Alpha size to encode number. */

  Boolean              hzDataPresent;         /* HZ data is present on SIM */
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

  SimDirId             rootDir;

  //Band                 currentBand; /* FR0102-9447: SIM handling of band dependant MNC encoding */
  SimDialnumEntries    dialnumEntries;   /*job 100935 & job 103546: if the AlsiListDialNumReq was interrupted, we keep the list of
                                          *valid entries, so we don't have to read all the records again. */
  Boolean              isPoweringDown;
  Boolean              highSpeedSimSupported;    /*indicates whether the USIM/SIM drivers support High Speed SIM*/
  /*If the following flag is set to TRUE, we will choose T=0 protocol even if the SIM/USIM supports T=1*/
  Boolean              disableTequal1;
  Boolean              clockTimerQueued;       /* Flag to indicated if a Clock timer expiry is queued */
  TimerId              queuedTimerId;          /* Id of queued timer */
  //Int8                   fatalCounter;      /*remember the times of Fatal sim error*/

/* Power Save mode data */
  USIMPSMState         usimPSMStatus;     /* state of PSM cycle: none, write, read */
  Boolean              updateNVRamData;  /* Is NVRam update needed before Deep Sleep */
  UsimPSMSimData       usimPSM_NVRam;    /* Power Saving Mode NVRam data structure */
#if defined ( ENABLE_USIM_RTC)
  UsimPSMRtcSimData    usimPSM_RTCRam;   /* Power Saving Mode RtcRam data structure */
#endif
/*
PSM_DISABLED   0 ; deep sleep disabled
NORMAL_POWERON 1 ; normal power on
PSM_WAKEUP     2 ; deep sleep wakeup
DEEP_SLEEP     3 ; deep sleep on going
*/
  Int8                 psmState;
  //Boolean              eDRXAllowed; /* Savein rtc ram ef_ad UICC power off in eDRX state */

  /* PSC constraint setting */
  Int8                simPscConsSet; /* to save current psc state, no multible sets of constraints */

  //SimUiccResumeToken  resumeToken; /* save UICC suspend resume token */
  //Boolean             uiccSuspensionAllowed; /* Is UICC suspend allowed, checked from EF_UMPC */

  Boolean         needHiddenReset;

  /* for emu SIM in half awake state */
  Boolean         lastApnEnabled;

  AlsiAppInitialiseReq  reinitReq;
  Boolean               initReqReceived; /* received during fake init */
  Boolean               receivedRealInitReq; /* received during hidden reset*/
  KiUnitQueue           alsaTrQueue;       /* job 106677 : Storage for ALSA REQ signals       */
  SimManagerState       lastState;
  SimManagerState       recoveryState;
  Int8                  fastRecoveryCount;
  Boolean               utkRefreshStarted;
  Boolean               sleepPoweredOff;  /* card is powered off for preparing deep sleep. */
  Int32                 logicalChannelOpenedMap;  /* Indicate which channel has been opened. */
}
SimManagerData;

#if defined ( ENABLE_USIM_RTC)
/* Structure for RTCRam allocator this data is copied to simdata in retention */
typedef struct RtcSimDataTag{
    SIM_MANAGER_RTC_RAM_DATA;
}
RtcSimData;
#endif

/* Structure for NVRam allocator this data is copied to simdata in retention */
typedef struct NvSimDataTag{
    SIM_MANAGER_NV_RAM_DATA;
}
NvSimData;

/* Structure for NVRam allocator this data for Half awake */
typedef struct NvSimHalfAwakeDataTag{
    /* Data for SIG_ALSI_APP_STARTED_IND */
    Int8                 efDirRecordNum;     /**< Record in EF DIR where this USIM application is referenced*/
    SimUiccAidLabel      label;              /**< Label */

    SimUiccKeyRefValue   pin1Ref;       /*PIN1*/
    SimUiccKeyRefValue   localPin;      /*PIN2*/
    SimUiccPinStatus     pin2Status;    /**< status of the PIN2 (local PIN) */
    SimUiccPinStatus     unblockPin2Status;  /**< status of the UNBLOCK PIN2 (local PIN) */

    Boolean              apnEnabled;
    SimUiccAid           aid;
}
NvSimHalfAwakeData;

#endif
