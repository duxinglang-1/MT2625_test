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
 * File Description                                                      */
/** \file
 *   Application Layer Background Layer Mobility Management
 *   Type Definitions
 **************************************************************************/

#ifndef ABMM_TYP_H
#define ABMM_TYP_H

/***************************************************************************
 * Nested Include Files
 ***************************************************************************/

#if !defined(APPLAYER_H)
# include <applayer.h>
#endif  /* !(APPLAYER_H) */

#if !defined(MMR_SIG_H)
# include <mmr_sig.h>     /* Has Plmn type definition                  */
#endif  /* !(MMR_SIG_H) */

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/* These tokens are used to distinguish the two types of strings          */
#define NETWORK_NAME_TOKEN            ('p')
#define COUNTRY_INITIALS_TOKEN        ('c')

/* the max number of PLMNs returned to FL for PLMN selection.
** Note that NUM_PLMNS is now defined in grrmrtyp.h */
#define NUM_PLMN_DETAILS              NUM_PLMNS

/* Defines for FDD lists for MCC stored in NVRAM */
#define NVRAM_NUM_FDD_LISTS           3
#define NVRAM_NUM_TDD_LISTS           NVRAM_NUM_FDD_LISTS
#define UMTS_LIST_SIZE                20

#define INVALID_FDD_INDEX             255
#define INVALID_TDD_INDEX             INVALID_FDD_INDEX

#define HPLMN_FDD_INDEX               254
#define HPLMN_TDD_INDEX               HPLMN_FDD_INDEX

#define NVRAM_NUM_EUTRA_LISTS         6
#define EUTRA_LIST_SIZE               20
#define INVALID_EUTRA_INDEX           255
#define HPLMN_EUTRA_INDEX             254

/* The number of BA Lists stored in NVRAM */
#if defined(NVRAM_TEST)
# define BA_LIST_FILE_SIZE            3
#else  /* !(NVRAM_TEST) */
# define BA_LIST_FILE_SIZE            4
#endif  /* !(NVRAM_TEST) */

#define ABMM_MMR_LOG_SIZE             10
#define ABMM_EVENT_LOG_SIZE           10

/* Maximum number of preferred bands read form Modem configuration */
#define MAX_NUMBER_OF_PREFERRED_BANDS   6
/* Maximum length(32 + 1) of preferred bands read form Modem configuration */
#define MAX_LENGTH_OF_PREFERRED_APN     33

/* Maximum number of operator's preferred information table item  */
#define MAX_NUM_OF_OPT_PREFERRED_INFO    32


/* Maximum number of preferred frequency point list */
#define MAX_NUMBER_OF_PREFERRED_EARFCNS   ERRC_MAX_REGULAR_FREQUENCY

/* Maximum number of preferred bands which configured by user via AT command */
#define MAX_NUM_USER_CFG_BAND_LIST    4

/* Maximum number of preferred bands which configured by user via AT command */
#define MAX_NUM_NVRAM_CFG_BAND_LIST    4

/* Maximum number of EHPLMN which configured by user via AT command */
#define MAX_NUM_USER_CFG_EHPLMN_LIST  3

/* Number of searches to be done */
#define MAX_NUMBER_OF_PLMN_SEARCH   10

/*The default number of NSPS searche to be done*/
#define NAS_NSPS_RECOVERY_SEARCH_NUM_OF_STEPS_DEFAULT   6

/*The default number of NSPS searche to be done for LGUP */
#define LGUP_RECOVERY_SEARCH_NUM_OF_STEPS_DEFAULT       10

/*The default number of NSPS start step which NO service for LGUP */
#define LGUP_RECOVERY_SEARCH_STEP_FROM_NO_SVC_DEFAULT   4


/*The default repeat times of PH1 recovery search for LGUP */
#define LGUP_PH1_RECOVERY_DEFAULT_SEARCH_RPEATS_TIMES      10
/*The default repeat times of PH2 recovery search for LGUP */
#define LGUP_PH2_RECOVERY_DEFAULT_SEARCH_RPEATS_TIMES      2
/*The default repeat times of PH2 recovery search for LGUP */
#define LGUP_PH3_RECOVERY_DEFAULT_SEARCH_RPEATS_TIMES      2

#define LGUP_RECOVERY_DEFAULT_SEARCH_RPEATS_TIMES          1

/* 1 second */
#define NSPS_SEARCH_1_SECOND_PERIOD                        1
/* 1 minute, unit is second. */
#define NSPS_SEARCH_1_MINUTE_PERIOD                        60
/* 1 minute, unit is second. */
#define NSPS_SEARCH_1_HOUR_PERIOD                          3600


#define SLOTS_TO_GPRS_SMS(slots) (slots * 4 - 1)
#define GPRS_SMS_TO_SM(sms)      ((sms * 2 + 2) / 3)    /* +2 to round up */

#if !defined(GPRS_MULTISLOT_CLASS)
/* Default is class 12 */
# define GPRS_SWITCH_MEASURE_SWITCH   SLOTS_TO_GPRS_SMS (2)
# define GPRS_SWITCH_MEASURE          GPRS_SMS_TO_SM (GPRS_SWITCH_MEASURE_SWITCH)
#else  /* !!(GPRS_MULTISLOT_CLASS) */
# if (GPRS_MULTISLOT_CLASS == 1)
#  define GPRS_SWITCH_MEASURE_SWITCH  SLOTS_TO_GPRS_SMS (4)
#  define GPRS_SWITCH_MEASURE         GPRS_SMS_TO_SM (GPRS_SWITCH_MEASURE_SWITCH)
# else
#  if (GPRS_MULTISLOT_CLASS == 12)
#   define GPRS_SWITCH_MEASURE_SWITCH SLOTS_TO_GPRS_SMS (2)
#   define GPRS_SWITCH_MEASURE        GPRS_SMS_TO_SM (GPRS_SWITCH_MEASURE_SWITCH)
#  else
#   define GPRS_SWITCH_MEASURE_SWITCH SLOTS_TO_GPRS_SMS (3)
#   define GPRS_SWITCH_MEASURE        GPRS_SMS_TO_SM (GPRS_SWITCH_MEASURE_SWITCH)
#  endif
# endif
#endif  /* !!(GPRS_MULTISLOT_CLASS) */


/***************************************************************************
 * Type Definitions
 **************************************************************************/
/** Determines if ABMM detached was triggered or not */
typedef enum AbmmDetachTrigTag
{
    /** ApexMmNetworkStateInd signal not triggered by a GPRS detach. */
    GPRS_NO_DETACH_TRIG = 0,
    /** ApexMmNetworkStateInd signal triggered by a handset initiated GPRS detach request */
    GPRS_ME_DETACH_TRIG = 1,
    /** ApexMmNetworkStateInd signal triggered by a network initiated GPRS detach request */
    GPRS_NW_DETACH_TRIG = 2
}
AbmmDetachTrig;

/** GPRS Registration Status element */
typedef enum AbmmGprsRegStatusTag
{
    GPRS_STAT_NOT_REG,  /**< not trying to obtain GPRS service */
    GPRS_STAT_REG_HOME, /**< have GPRS service on HPLMN */
    GPRS_STAT_SEARCHING, /**< want GPRS service & performing PLMN search or waiting
                           for response to MmrRegReq with GPRS */
    GPRS_STAT_REG_DEN,  /**< Got MmrNregInd for GPRS */
    GPRS_STAT_UNKN,     /**< Unknown Status */
    GPRS_STAT_REG_ROAM  /**< have GPRS service on non-HPLMN */
}
AbmmGprsRegStatus;

/** MM request status element which indicates the result of the request */
typedef enum MmRequestStatusTag
{
    MM_REQ_OK,                    /**< Request successful */
    MM_REQ_SIM_ERROR,             /**< Sim has reported error */
    MM_REQ_SIM_FULL,              /**< The preferred PLMN list is full */
    MM_REQ_INVALID_PARAM,         /**< Invalid message parameter */
    MM_REQ_NOT_CONTROL_TASK,      /**< Requesting task is not the controlling task*/
    MM_REQ_WRITE_ERROR,           /**< Unable to write data */ /* FR9910-5323 moved from inside MULTI_BAND */
    MM_REQ_INVALID_BAND_MODE,     /**< Bad band mode requested*/
    MM_REQ_NOT_READY,             /**< MM not ready for foreground layer */
    MM_REQ_RECORD_NOT_FOUND,      /**< Unable to find a BaList record */
    MM_REQ_READ_ERROR,            /**< Unable to read data */
    MM_REQ_CALL_IN_PROGRESS,      /**< Cannot do what is asked because a call is in progress */
    MM_REQ_SUSPEND_FAILED,        /**< MM cannot carry out a suspend right now */
    MM_REQ_INCOMPATIBLE_STATE,    /**< ABMM cannot carry out a suspend right now */
    MM_REQ_INVALID_SERVICE_TYPE,  /**< Combination of requested service type
                                  * and GPRS MS class is not permitted.
                                  */
    MM_REQ_OPERATOR_NOT_FOUND,    /**< PLMN (MCC/MNC) could not be identified from PLMN name string */
    MM_REQ_REGISTER_FAIL,
    MM_NUMBER_OF_REQ_STATUS       /**< Number of elements in MmRequestStatus */

}
MmRequestStatus;

typedef enum PlmnSelectResultTag
{
    PLMN_SELECT_OK,     /**< registration was attempted - and it worked      */
    PLMN_SELECT_FAIL,   /**< registration was attempted, but it failed       */
    PLMN_SELECT_BUSY,   /**< request is refused because PLMN registration
                         * selection is already in progress
                         */
    PLMN_SELECT_REFUSED, /**< request refused because the outside events have
                          * conspired to make registration impossible e.g
                          * SIM removed, powering down, etc.
                          */
    PLMN_SELECT_NO_CONTROL, /**< request refused because requesting task does
                             * not have control of ABMM
                             */
    PLMN_SELECT_INVALID,   /**< invalid PLMN              */
    NUM_PLMN_SELECTS,     /**< Number of elements */

    PLMN_SELECT_NULL      /**< PLMN selected is null */
}
PlmnSelectResult;

typedef enum RejectCauseTypeTag
{
    REJECT_CAUSE_TYPE_EMM,
    REJECT_CAUSE_TYPE_SPECIFIC
}
RejectCauseType;

typedef enum DeactReasonTypeTag
{
    DEACT_REASON_INIT_VALUE,
    DEACT_REASON_NSPS_SEARCH,
    DEACT_REASON_CELL_LOCK,
    DEACT_REASON_MAN_REQUEST,
    DEACT_REASON_POWER_DOWN,
    DEACT_REASON_SIM_REFRESH
}
DeactReasonType;

typedef enum MmPsmStatusTag
{
    MM_EXIT_PSM_STATE,
    MM_ENTER_PSM_STATE,
    MM_NUMBER_OF_PSM_STATE
}
MmPsmStatus;


#if defined (ENABLE_L23_DEBUG)
#define NUM_OF_PLMN_LISTS    NUMBER_OF_SUPPORTED_RATS
#endif /* ENABLE_L23_DEBUG */

typedef enum ActionTypeTag
{
    NO_ACTION,
    ACTION_RETRY_PLMN,
    ACTION_ABANDON_REG
}ActionType;



typedef struct PlmnInfoTag
{
    /** Numerical identity of PLMN. */
    Plmn        plmn;
    /** TRUE if it is a 3 digit MNC */
    Boolean     threeDigitMnc;
    /** TRUE if the MS is currently registered on this PLMN. */
    Boolean     isRegisteredPlmn;
    /** TRUE is the PLMN is in the Forbidden PLMN list on the SIM. */
    Boolean     isForbiddenPlmn;
    /** TRUE if this is the HPLMN. */
    Boolean     isHomePlmn;
    /** TRUE if this is PLMN has a low signal level.*/
    Boolean     isWeakPlmn;
    /** An indication of the bands the PLMN is operating on
     * ( The band a PLMN is visible in, only valid
     * for the results of a PLMN search).
     */
    EutraBand   networkBand;
    /** if GPRS available for this PLMN and current cell
     * (TRUE if the PLMN supports GPRS service).
     */
    Boolean     gprsSupported;    /* 0107-11499  */
    /** TRUE if the MS is not allowed to use GPRS service on that PLMN.*/
    Boolean     isGprsForbidden;  /* 0207-18651 */
    /** The LAC or TAC of the PLMN. */
    Lac         lac;
}
PlmnInfo;

/** FullPlmnDetails gives extensive information about a PLMN, including a
 * textual representation of the PLMN.
 */
typedef struct FullPlmnDetailsTag
{
    /** The name of the PLMN */
    PlmnName name;
    /** Information */
    PlmnInfo plmnInfo;
}
FullPlmnDetails;

/** PlmnArrayStruct element */
typedef struct PlmnArrayStructTag
{
    /** Number of PLMNs in the fullPlmnDetails
\assoc ARRAY \ref fullPlmnDetails */
    Int16           numEntries;
    /** Detail information for each PLMN */
    FullPlmnDetails fullPlmnDetails [NUM_PLMN_DETAILS];
    /** Country code of SIM's HPLMN */
    Mcc             homeMcc;
}
PlmnArrayStruct;

typedef struct BaListInfoTag
{
    Boolean               isValid;
    Lai                   lai;
    Int8                  channelData[BA_ARRAY_SIZE];
}
BaListInfo;

/* This is a list of FDD UARFCNs detected in a country.
 * It can be used to prime the search list in the access stratum */
typedef struct UmtsListForMccTag
{
    Boolean isValid;
    Mcc     mcc;
    Mnc     mncLastUpdated;
/** \assoc ARRAY \ref UArfcnList */
    Int8    numUArfcn;
    UUARFCN UArfcnList[UMTS_LIST_SIZE];
}
UmtsListForMcc;

/* This is a list of LTE EARFCNs detected in a country.
 * It can be used to prime the search list in the access stratum */
typedef struct EutraListForMccTag
{
    Boolean     isValid;
    Mcc         mcc;
    Mnc         mncLastUpdated;
    Int8        numEutraArfcn;
    EutraArfcn  eutraArfcnList[EUTRA_LIST_SIZE];
}
EutraListForMcc;

/*
 * 0107-11765 types for ABMM debug signals
 * moved AbmmState, AbmmEvent, AbmmRefType, AbmmRegState from abmm_int.h so
 * they can be used in signals.
 */

/** ABMM overall states */
typedef enum AbmmStateTag
{
    ABMM_STATE_NULL,            /**< State is NULL */
    ABMM_STATE_NO_SERVICE,      /**< No service available */
    ABMM_STATE_SELECTING,       /**< trying to register on a PLMN as part of a
                                  *  PLMN selection process
                                  */
    ABMM_STATE_NORMAL_IDLE,     /**< registered */
    ABMM_STATE_DETACHING,       /**< Silent detach*/
    ABMM_STATE_POWERING_DOWN,   /**< powering down the MS */

/* added for 0204-17315 */
    ABMM_STATE_WAIT_DEREGISTER, /**< Waiting for an NRegCnf */
    ABMM_STATE_DEREGISTERED,    /**< Deregistered - do nothing */

    ABMM_STATE_WAIT_SUSPEND,    /**< Waiting for an NRegCnf */
    ABMM_STATE_SUSPENDED,       /**< Suspended - do nothing */
    ABMM_STATE_WAIT_RESUME,     /**< Waiting for an MmResumeReq */
    NUM_ABMM_STATES             /**< Number of elements in AbmmState */
}
AbmmState;

/** ABMM overall event */
typedef enum AbmmEventTag
{
    /** No event available */
    ABMM_EVENT_NO_EVENT,
    /** Attempting to deregister from a service that we are not registered for it.  */
    ABMM_EVENT_DEREGISTERED,    /* 1*/
    /** Event failed */
    ABMM_EVENT_FAILED,
    /** Loss of service while call active */
    ABMM_EVENT_LOST_REG,
    /** PLMN search timer is expired */
    ABMM_EVENT_PLMN_TIMEOUT,
    /** HPLMN search timer is expired */
    ABMM_EVENT_HPLMN_TIMEOUT,   /* 5 */
    /** Power down event */
    ABMM_EVENT_POWER_DOWN,
    /** Power down timeout event */
    ABMM_EVENT_POWER_DOWN_TIMEOUT,
    /** Finish reading ConfigData event */
    ABMM_EVENT_READ_CONFIG_DATA_DONE,
    /** Registration event */
    ABMM_EVENT_REGISTERED,
    /** Resume event */
    ABMM_EVENT_RESUME,      /* 10 */
    /** SIM is not OK */
    ABMM_EVENT_SIM_NOK,
    /** SIM is OK */
    ABMM_EVENT_SIM_OK,
    /** Suspend event */
    ABMM_EVENT_SUSPEND,
    ABMM_EVENT_USER_REQ_PLMN,
    /** Detach error event */
    ABMM_EVENT_DETACH_ERROR,    /* 15 */
    /** Retry PLMN event */
    ABMM_EVENT_RETRY_PLMN,
/* added for 0204-17315 */
   /** Deregister request event */
    ABMM_EVENT_DEREGISTER,
    ABMM_EVENT_ATTACH,      /**< used when attached on GPRS or CS and want to attach CS or GPRS */
    ABMM_EVENT_DETACH,      /**< used when attached combined and want to detach GPRS (or CS)    */
    /** SIM is OK and new HPLMN event */
    ABMM_EVENT_SIM_OK_NEW_HPLMN,    /* 20 */
    /** Number of ABMM events */
    NUM_ABMM_EVENTS,
    /** Event is NULl */
    ABMM_EVENT_NULL
}
AbmmEvent;

/* different actions that can be carried out in the course of doing PLMN */
/* selection                                                            */
typedef enum AbmmRegStateTag
{
    ABMM_REG_STATE_NULL,
    ABMM_REG_STATE_RPLMN,
    ABMM_REG_STATE_PREF_HPLMN,
    ABMM_REG_STATE_PREF_AVAILABLE_LIST,   /*search for all visible PLMNs */
    ABMM_REG_STATE_NEXT_PREF_PLMN,
    ABMM_REG_STATE_NEXT_NOT_PREF_PLMN,
    ABMM_REG_STATE_MANUAL_PLMN,    /* specified in Manual User Selection */
    ABMM_REG_STATE_SILENT_DETACH,
    ABMM_REG_STATE_REAL_DETACH,
    ABMM_REG_STATE_REAL_DETACH_2,
    ABMM_REG_STATE_DETACH_GPRS,           /* used when detaching from GPRS only */
    ABMM_REG_STATE_SEARCH_PREF_LIST,      /* 0210-21039 used for MmrPlmnListReq with requested PLMN in */
    ABMM_REG_STATE_EPLMN,
    NUM_ABMM_REG_ACTIONS
}
AbmmRegState;

/* used to index array of lists of ABMM_REG_ACTIONs - for each ABMM_SEL_TYPE_ */
/* there is an (ordered) set of actions to carry out as part of the PLMN      */
/* selection process                                                          */
typedef enum AbmmRegTypeTag
{
    ABMM_SEL_TYPE_NULL,
    ABMM_SEL_TYPE_MANUAL_USER,          /*1*/
    ABMM_SEL_TYPE_AUTO_USER_1,
    ABMM_SEL_TYPE_AUTO_NOT_REGISTERED,
    ABMM_SEL_TYPE_NEW_SIM,
    ABMM_SEL_TYPE_LOST_REGISTRATION,    /*5*/
    ABMM_SEL_TYPE_TIMED,
    ABMM_SEL_TYPE_TIMED_HPLMN,
    ABMM_SEL_TYPE_TIMED_HPLMN_2, /* 0107-11562 */
    ABMM_SEL_TYPE_TIMED_HPLMN_3, /* 0107-11562 */
    ABMM_SEL_TYPE_SILENT_DETACH,        /* 10 */
    ABMM_SEL_TYPE_DETACH,
    ABMM_SEL_TYPE_RETRY_PLMN,
    ABMM_SEL_TYPE_GPRS_ATTACH,
    ABMM_SEL_TYPE_GPRS_ATTACH_2,
    ABMM_SEL_TYPE_GPRS_DETACH,          /* 15*/
    ABMM_SEL_TYPE_NEW_HPLMN,
    ABMM_SEL_TYPE_RESUME_IDLE,
    ABMM_SEL_TYPE_NEW_SIM_HPLMN,
    ABMM_SEL_TYPE_TIMED_NO_RPLMN,
    ABMM_SEL_TYPE_MANUAL_USER_FPLMN,    /* 20 */
    ABMM_SEL_TYPE_MANUAL_USER_TIMED,
    ABMM_SEL_TYPE_PSM_START_SEARCH,
    ABMM_SEL_TYPE_FREQ_LOCK,
    NUM_ABMM_SEL_TYPES
}
AbmmRegType;

/** ABMM debug information type element */
typedef enum AbmmDebugInfoTypeTag
{
    /** Debug the state of ABMM */
    ABMM_DEBUG_STATE,
    /** Log MMR */
    ABMM_DEBUG_MMR_LOG,
    /** Logging the event */
    ABMM_DEBUG_EVENT_LOG,
    /** Types of debug information */
    NUM_ABMM_DEBUG_INFO_TYPES
} AbmmDebugInfoType;

/** The status of debug information used to confirm the request of debugging */
typedef enum AbmmDebugInfoStatusTag
{
    /** debug is OK */
    ABMM_DEBUG_OK,
    /** Unknown type of debug */
    ABMM_DEBUG_UNKNOWN_INFO_REQ,
    /** Only for DEVELOPMENT_VERSION */
    ABMM_DEBUG_DEV_VERSION_ONLY
} AbmmDebugInfoStatus;

/** Information related to the state of ABMM */
typedef struct AbmmStateInfoTag
{
    /** Current ABMM state */
    AbmmState     currentState;
    /** Previous ABMM state */
    AbmmState     previousState;
    /** Last event */
    AbmmEvent     lastEvent;
    /** Registration type */
    AbmmRegType   regType;
    /** Registration status */
    AbmmRegState  regState;
    /** Current PLMN service status */
    ServiceStatus serviceStatus;
    /** Last PLMN service status */
    ServiceStatus lastServiceStatus;
    /** Last registered PLMN */
    Plmn          lastRplmn;
    /** The last network we attempted to register in */
    Plmn          lastAttemptedPlmn;
    /** Whether or not last registered PLMN is valid */
    Boolean       lastRplmnIsValid;
    /** Current PLMN */
    Plmn          currentPlmn;
    /** PLMN selection timer */
    KiTimer       plmnSelectTimer;
    /** HPLMN search timer */
    KiTimer       hplmnSelectTimer;
    /** Whether or not GPRS information is present */
    Boolean       gprsInfoPresent;
    /** Determines the type of service */
    ServiceType   serviceType;
    ServiceType   availableServiceType;    /**< Record of which service(s) we have successfully
                                              registered for */
    ServiceType   lastRequestedServiceType; /**< Record of the service type last requested */
} AbmmStateInfo;
# if defined (ENABLE_L23_DEBUG) || defined (ENABLE_ABMM_DEBUG)
typedef struct RegStatusInfoTag
{
    AbmmState     currentState;
    AbmmEvent     event;
    AbmmRegType   regType;
    AbmmRegState  regState;
    ServiceStatus serviceStatus;
    ServiceStatus lastServiceStatus;
    Boolean       rplmnIsValid;
    Plmn          rplmn;
    Plmn          lastAttemptedPlmn;
    Plmn          currentPlmn;
    ServiceType   serviceType;
    ServiceType   availableServiceType;
    ServiceType   lastRequestedServiceType;
} RegStatusInfo;

typedef struct ActionInfoTag
{
/** \assoc UNION \ref regActionInfo */
    ActionType actionType;
    union
    {
        Boolean retryPlmnPresent;
        Boolean abandonReg;
    } regActionInfo;
}ActionInfo;

typedef struct RegCtrlInfoTag
{
    AbmmDetachTrig abmmDetachTrig;
    Boolean        csAllowed;
    Boolean        gprsAllowed;
    Boolean        pdpActivation;
    ActionInfo     actionInfo;
}RegCtrlInfo;

typedef struct PlmnManualSearchDataTag
{
    PlmnInfo    plmnInfo;
    SimPlmn     simPlmn;             /* PLMN in SIM format */
    Boolean     isAvailable;
    Boolean     isPreferred;         /* PLMN is on SIMs preferred list         */
                                  /* used to prevent PLMNs on Pref & Avail  */
                                  /* lists being considered for selection   */
                                  /* twice                                  */
    /* 0107-11499 UPGRADE_GPRS: moved gprsSupported into PlmnInfo */
    Boolean     isNatRoamForbidden;  /* 0207-19330 */
    Boolean     duplicateEntry;      /* 0210-21058 TRUE if PLMN occurs more than
                                   * once in list */
    Boolean     isEplmn;             /* is the PLMN in the equivalent PLMN list */
}PlmnManualSearchData;

typedef struct PlmnListInfoTag
{
    Boolean     gotAvailableList;
    Boolean     manualKnownAvailable;
    Boolean     rplmnKnownAvailable;
    Boolean     manualModeEngaged;
    Boolean     manualPlmnDataValid;
    Boolean     plmnManuallySelected;
    SignedInt16 searchPlmnNum;
    SimPlmn     manualPlmn;
    Int16       numSearchPlmns;
    PlmnManualSearchData plmnManualSearchData[NUM_PLMN_DETAILS + 1];
}PlmnListInfo;
#endif

/** Networg informtion used fro logging purpose */
typedef struct LogNetworkInfoTag
{
    /** Mobile Country Code */
    Mcc         mcc;
    /** Mobile Network Code */
    Mnc         mnc;
    /** Service type */
    ServiceType serviceType;
} LogNetworkInfo;

/** Logging information used for debugging */
typedef struct AbmmMmrLogTag
{
    /** Identifies the signal */
    SignalId signalId;
    /** Log the status of the PLMN */
    union {
        /** Identifies the status of the service */
        ServiceStatus  serviceStatus;
        /** The result of PLMN search */
        PlmnSearchResult result;
    } status;
    /** Provides information or debugging purpose */
    union {
        /** Network information */
        LogNetworkInfo networkInfo;
        /** Indicates error information */
        MmrErrorInd    errorInfo;
    } info;
} AbmmMmrLog;

/** Debug log: ABMM event */
typedef struct AbmmEventLogTag
{
    /** ABMM event */
    AbmmEvent event;
    /** ABMM state */
    AbmmState state;
} AbmmEventLog;

/* Added for 0802-18991 */
typedef struct AbmmPowerModeStateTag
{
    Boolean powerUpSim;
    Boolean powerUpProtoStack;
    Boolean simNeedPostPowerUp;
    Boolean stackNeedPostPowerUp;
    Boolean stackSuspended;
} AbmmPowerModeState;

/** Provides information for debugging purpose */
typedef union AbmmDebugInfo
{
    /** The state of ABMM information */
    AbmmStateInfo stateInfo;
    /** Debug log */
    AbmmMmrLog    mmrLog[ABMM_MMR_LOG_SIZE];
    /** Log ABMM event*/
    AbmmEventLog  eventLog[ABMM_EVENT_LOG_SIZE];
    /** Dummy value */
    Int8          dummy;
} AbmmDebugInfo;

/** ABMM PLMN selector element */
typedef enum AbmmPlmnSelectorTag
{
    ABMM_USER_SELECTOR = 0,    /**< The information can come from EF PLMN SEL or EF PLMN w
                                * ACT file on the SIM
                                */
    ABMM_OPERATOR_SELECTOR,    /**< The info comes from OPLMN w ACT*/
    ABMM_HPLMN_SELECTOR        /**< The info comes from HPLMN w ACT*/
} AbmmPlmnSelector;


/** Acquiring Order */
typedef enum AbmmNetworkModePreferenceTag
{
  ABMM_ACCESS_ANY,
  ABMM_ACCESS_GSM_FIRST,
  ABMM_ACCESS_3G_FIRST,
}AbmmNetworkModePreference;

/*operate prefert info*/
typedef struct AbmmOperatorPrefInfoTag
{
    Plmn            operatorPlmn;        /**< Operator PLMN . */
    char            preferApnName[MAX_LENGTH_OF_PREFERRED_APN];     /*APN NAME*/
    Int32           supportBandsByOperator; /** support bands by operator's,
                                                bands bitmap, see errc_bands.h for definition*/
    NbiotBand       preferBandList[MAX_NUMBER_OF_PREFERRED_BANDS];      /*prefer band Set, frist entity is the Hihgest priority*/
}AbmmOperatorPrefInfo;

/**/
typedef struct NasOperatorPrefInfoNvDataTag
{
    /*perator's Preferred Information Table */
    AbmmOperatorPrefInfo  operatorPrefInfoTable[MAX_NUM_OF_OPT_PREFERRED_INFO];  
    
}NasOperatorPrefInfoNvData;


/* NSPS Steps numbers*/
typedef enum RepeatNbrOfNspsStepTag
{
    NAS_NSPS_STEP_REPEAT_0,
    NAS_NSPS_STEP_REPEAT_1,
    NAS_NSPS_STEP_REPEAT_2,
    NAS_NSPS_STEP_REPEAT_3,
    NAS_NSPS_STEP_REPEAT_4,
    NAS_NSPS_STEP_REPEAT_5,
    NAS_NSPS_STEP_REPEAT_6,
    NAS_NSPS_STEP_REPEAT_7,
    NAS_NSPS_STEP_REPEAT_8,
    NAS_NSPS_STEP_REPEAT_9,
    NAS_NSPS_STEP_REPEAT_10,
    NAS_NSPS_MAX_STEPS
}RepeatNbrOfNspsStep;

/*NSPS steps information*/
typedef struct NasNspsStepSearchInfoTag
{
    /** NPPS search type, 
     * TRUE is EUTRA_SEARCH_TYPE_DEEP
     * FALSE is EUTRA_SEARCH_TYPE_SHALLOW
     */
    Int8        nspsSearchType;   
    /** Repeat searched number of the NSPS step */
    Int8        nspsRepeatNum;
    /** Time interval of per repeat search, unit is second*/
    Int32       nspsSearchPeriod;
}
NasNspsStepSearchInfo;

/*NSPS recovery  coverage search */
typedef struct nasNspsSearchCfgDataTag
{
    /** number of NSPS steps */
    Int8                    nbrOfNspsSteps; 
    /** */
    NasNspsStepSearchInfo   nspsStepTbl[MAX_NUMBER_OF_PLMN_SEARCH];
    
}nasNspsSearchCfgData;

/***************************************************************************
 *  Macros
 **************************************************************************/

#endif /* !(ABMM_TYP_H) */

/* END OF FILE */
