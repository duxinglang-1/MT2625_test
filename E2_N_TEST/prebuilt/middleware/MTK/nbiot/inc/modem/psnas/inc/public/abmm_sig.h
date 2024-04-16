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
 *   Signal Definitions
 **************************************************************************/

#ifndef ABMM_SIG_H
#define ABMM_SIG_H

/***************************************************************************
 * Nested Include Files
 ***************************************************************************/

# if !defined (APPLAYER_H)
# include "applayer.h"
# endif /* !(APPLAYER_H) */

# if !defined (ABMM_TYP_H)
# include "abmm_typ.h"
# endif /* !(ABMM_TYP_H) */

# if !defined (MMR_SIG_H)
# include "mmr_sig.h"
# endif /* !(MMR_SIG_H) */

# if !defined (ALSI_SIG_H)
# include "alsi_sig.h"   /* Need for Bcd Definition */
# endif /* !(ALSI_SIG_H) */

#include "nvstatus_typ.h"   /* for NvStatus: NVRAM_OK etc. */

/** \defgroup SigAbmm  ABMM Signalling Interface
 * \ingroup NasApex
 * \ingroup Sig
 * Application Layer Background Layer Mobility Management interface.
 * This section describes the operation and use of the Mobility Management procedure,
 * including the structure of APEX Mobility Management signals.
 * These are the users of the Mobility Management procedure and access facilities
 * via the APEX interface. All APEX messages related to the Mobility Management
 * procedure have the prefix ApexMm.
 * @{
 */
/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/** Upper limit of the number of reads that can be requested via the
 * ApexMmReadPlmnSelReq, ApexMmReadPlmnListReq
 * interfaces respectively.
 */
#define PLMN_READ_LIMIT      5
# if PLMN_READ_LIMIT > SIM_PLMN_LIST_SIZE
# error "PLMN_SEL_READ_LIMIT must be less than or equal to SIM_PLMN_LIST_SIZE"
# endif /* (PLMN_READ_LIMIT > SIM_PLMN_LIST_SIZE) */

/** Upper limit of the number of reads that can be requested via the
 * ApexMmReadPlmnListReq.
 */
#define PLMN_LIST_READ_LIMIT 5

#define NUM_OF_BAND_OPTIONS  5  /**< 5 of these for (PGSM or EGSM), DCS, PCS,
                                 * (PGSM_DCS or EGSM_DCS), GSM_PCS.
                                 */
/* Job137602: Modify the NUM_OF_BAND_OPTIONS to GSM,FDD,TDD128 */
#define NUM_OF_RATS             NUMBER_OF_SUPPORTED_RATS       /* defined in l13_typ.h */

#define GSM_BAND_MODE_OFFSET    (NMODE_GSM_OFFSET)
#define FDD_BAND_MODE_OFFSET    (NMODE_UMTS_FDD_OFFSET)
#define TDD_BAND_MODE_OFFSET    (NMODE_UMTS_TDD128_OFFSET)
#define LTE_TD_BAND_MODE_OFFSET (NMODE_LTE_TD_OFFSET)
#define LTE_FDD_BAND_MODE_OFFSET (NMODE_LTE_FDD_OFFSET)

#define SATELLITE_MCC        901

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/*typedef enum MmRequestStatusTag  moved to abmm_sig.h                      */

/** EEPROM request status element */
typedef enum MmEepromReqStatusTag
{
    /** Request successful */
    MM_EEPROM_REQ_OK,
    /** EEPROM has reported error */
    MM_EEPROM_REQ_ERROR,
    /** Invalid message parameter  */
    MM_EEPROM_REQ_BAD_PARAM,
    /** Requesting task is not the controlling task  */
    MM_EEPROM_REQ_NOT_CNTL_TASK,
    /** Number of entries in MmEepromReqStatus */
    MM_NUMBER_OF_EEPROM_REQ_STATUS
}
MmEepromReqStatus;

/** Specifies the type of editing action to be performed
 * in the apexMmWritePlmnSelReq message.
 */
typedef enum PlmnMenuActionTag
{
    /** Insert entry in preferred PLMN list */
    PLMN_SEL_INSERT,
    /** Append entry to preferred PLMN list */
    PLMN_SEL_APPEND,
    /** Delete entry from preferred PLMN list */
    PLMN_SEL_DELETE,
    /** Add entry to user-defined PLMN list */
    PLMN_UD_ADD,
    /** Change entry to user-defined PLMN list */
    PLMN_UD_CHANGE,
    /** Delete entry from user-defined PLMN list */
    PLMN_UD_DELETE
}
PlmnMenuAction;

/** Specifies GPRS Attach At Power Up options.            */
typedef enum ApexMmGAPowerUpOptionTag
{
    ABMM_GAPWON_DEFAULT,       /**< Use the settings from abcfg.c           */
    ABMM_GAPWON_ATTACH,        /**< Always attach GPRS at power up          */
    ABMM_GAPWON_DONT_ATTACH,   /**< Never attach GPRS at power up           */
    ABMM_GAPWON_USE_HISTORY,   /**< Use last service status from NVRAM      */
    ABMM_GAPWON_MAX_OPTIONS    /**< GPRS Attach At Power Up maximum options */
}
ApexMmGAPowerUpOption;

/** ABMM PLMN list status element */
typedef enum AbmmPlmnListStatusTag
{
    /** List of All PLMNs is good */
    PLMN_LIST_OK,
    /** Lower layer was busy */
    PLMN_LIST_NOT_ALLOWED,
    /** SIM problem */
    PLMN_LIST_SIM_NOK,
    /** List not available */
    PLMN_LIST_NOT_FOUND
}AbmmPlmnListStatus;

#ifdef ABMM_UNIT_TEST
/** Modes for ABMM Unit Testing  */
typedef enum AbmmUnitTestModeTag
{
  ABMM_UNIT_TEST_MODE_WAKEUP_FROM_SLEEP,  /**< UT mode for testing waking-up from deep sleep */
  ABMM_UNIT_TEST_MODE_GO_TO_SLEEP,        /**< UT mode for testing going to deep sleep */
  ABMM_UNIT_TEST_MODE_MAN_ATTACH
} AbmmUnitTestMode;
#endif /* ABMM_UNIT_TEST */

typedef enum MobileIDNameTag
{
    NV_IMEI,
    NV_SN,
    NV_SVN,
    NV_IMEISV,
    NV_NULL
}
MobileIDName;

/* lock cell command handle mode*/
typedef enum MmLockArfcnCmdModeTag
{
    MM_LOCK_ARFCN_CMD_MODE_READ,    /* Read lock cell/frequency info */
    MM_LOCK_ARFCN_CMD_MODE_SET,     /* Set lock cell/frequency info  */
    MM_LOCK_ARFCN_CMD_MODE_INVALID

}MmLockArfcnCmdMode;

/** Request to read the user-defined PLMN list from NVRAM.*/
typedef struct MmReadPlmnReqTag
{
    /** Requesting task ID.*/
    TaskId taskId;
    /** Index at which to start reading.*/
    Int16  startField;
    /** Number of entries to return. */
    Int16  numEntriesDesired;
}
MmReadPlmnReq;

/** The ApexMmReadUdPlmnListCnf signal is a response to the request to read
 * the user-defined PLMN list from NVRAM.
 */
typedef struct MmReadPlmnCnfTag
{
    /** Always TASK_BL_ID */
    TaskId  taskId;
    /** FALSE if no valid data returned */
    Boolean plmnArrayIsValid;
    /** Copied from request */
    Int16   startField;
    /** Entries in plmnArray[]*/
    Int16   numEntriesRead;
    /** Total number of entries in the PLMN lists. */
    Int16   totalNumEntries;
    /** List of PLMNs read */
    PlmnId  plmnArray[PLMN_READ_LIMIT];
}
MmReadPlmnCnf;

/** The ApexMmNetworkStateInd signal indicates to the Controlling
 * Task/MMI that the level of service has altered. Where applicable,
 * the textual name of the currently camped/registered PLMN and country is given.
 */
typedef struct ApexMmNetworkStateIndTag
{
    /** Always TASK_BL_ID (since ABMM generates this signal).*/
    TaskId                   taskId;
    /** Level of service currently available */
    ServiceStatus            serviceStatus;
    /** Indicates PLMN selection or camping is in progress when TRUE
     * (only valid if serviceStatus is PLMN_NO_SERVICE) .
     */
    Boolean                  isSelecting;
    /** TRUE if roaming (i.e not on Home network).*/
    Boolean                  currentlyRoaming;
    /** The expanded network name is valid.
\assoc PRESENT \ref name */
    Boolean                  validNetworkName;
    /** PLMN numerical values */
    Plmn                     plmn;
    /** PLMN name as a text string and it is valid only if validNetworkName is TRUE,
     * if the SIM is CPHS enabled and it contains an operator name, this will be sent
     * in this field in preference to the PLMN name if currentlyRoaming is set to FALSE.
     */
    PlmnName                 name;    /*FR9901-3682 - send plmn in signal*/
    /** TRUE if Enhanced Operator Name String is used */
    Boolean                  nameIsEons;
    /** TRUE if it is a 3 digit MNCTRUE if using 3 digits for MNC */
    Boolean                  threeDigitMnc;
    /** If TRUE the handset has been temporarily deregistered from the network so that
     * no signalling can take place (flight mode)
     */
    Boolean                  isSuspended;
    /** Tells the foreground what the current RAT is. */
    NetworkModes             networkMode;

    /** Indicates if the current PLMN is in the same country as the Home PLMN
     * (This was added to enable CPHS Home Country Roaming).
     */
    Boolean                  inHomeCountry;
    /** Indicates if the current PLMN was selected manually as opposed to
     * being automatically selected by the registration procedure.
     * This enables CPHS Home Country Roaming, which requires the
     * Foreground LayerMMI / Voyager Op to display different data if the
     * PLMN has been selected manually.
     */
    Boolean                  manuallySelected;
    /** Indicates if all available PLMNs are forbidden. */
    Boolean                  fplmnOnlyAvailable; /* 0210-21162 */
    /** Indicates if NetworkInd is the result
     * of MO or MT detach Req (or neither)
     */
    AbmmDetachTrig           abmmDetachTrig;
    /** Determines whether the handset is registered for CS service, GPRS service, or both. */
    ServiceType              serviceType;
    /** Used to pass current LAI or TAI to application  */
    Lai                      lai;
    /** Used to pass cellId to application */
    Int32                    cellId;
    /** Report if ABMM is doing manual or automatic network selection
     * (NOTE this is subtley different from manuallySelected)
     */
    Boolean                  isInManualMode; /* 0001-5774 */
    /** Additional update result (e.g. SMS only) we received in Attach Accept
     * for a combined attach procedure
     */
    AdditionalUpdateResult   additionalUpdateResult;
    /** User requested eDRX info and from network received eDRX parameters
    */
    EdrxInfo                 edrxInfo;
    /** PSM data received from the network
      * (PsmInfo is defined in the file mmr_sig.h)*/
    PsmInfo                  nwPsm;
    /** EMM reject cause and cause type */
    EmmCause                 emmCause;
    RejectCauseType          causeType;
    /** The current PDN connection setting during attach */
    Boolean                  attachWithoutPdn;
    /** Network capability for CIoT EPS feature */
    SupportedIotOptions      supportedNwOptions;
    /** Eps Network Feature support IE data, 24.301 9.9.3.12A */
    EpsNetworkFeatureSupport epsNwFeatureSupport;
    /** Extended EMM cause, TS 24.301 chapter 9.9.3.26A, IE indicates additional information
      * associated with EMM cause */
    ExtendedEmmCauseElement  extEmmCause;
    /** EMM registration type */
    RegisterType             emmRegType;

    /* TODO: Remove when all dependencies solved */
    /** GPRS Registration Status, as displayed in AT+CGREG */
    AbmmGprsRegStatus        gprsRegStatus;
    /** Determines GPRS cell information */
    GprsCellInfo             gprsCellInfo; /* 0211-21546 */
    /** Whether cphsOpName is present.
\ assoc PRESENT \ ref cphsOpName */
    Boolean                  cphsOpNamePresent;                 /* 16037 space for the CPHS data if it's present */
#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** CPHS Operator Name from SIM */
    SimCphsOperatorName      cphsOpName;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** Whether cphsOpNameShortForm is present.
\ assoc PRESENT \ ref cphsOpNameShortForm */
    Boolean                  cphsOpNameShortFormPresent;
#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** Short form of CPHS Operator Name from SIM. */
    SimCphsOperatorNameShort cphsOpNameShortForm;               /* 16037 end */
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** Whether present default APN and pdn type*/
    Boolean                  defaultApnPresent;
    /* APN assigned by network for default PDN connection */
    TextualAccessPointName   defaultApnFromNw;
    /* PDN type assigned by network for default PDN connection */
    PdnType                  defaultPdnTypeFromNw;
#ifdef FEA_NFM
    Int16                    remainingNfmStartTimerValue;
    Int16                    remainingNfmBackOffTimerValue;
    Boolean                  nfmActive;
#endif /* FEA_NFM */
}
ApexMmNetworkStateInd; /*FR9805-2416 - removed AbmmNetworkStateSig */

/** The ApexMmRssiInd signal passes a received signal level values
 * to all tasks registered for MM procedures.
 */
typedef struct ApexMmRssiIndTag
{
    /** Always TASK_BL_ID. */
    TaskId      taskId;

    /** RSRP of the serving cell, Value range: -160 - -40, Unit: dBm*/
    int16_t      rsrp;

    /** RSRQ of the serving cell, Value range: -40 - 0, Unit: dB */
    int8_t       rsrq;

    /** RSSI of the serving cell, Value range: -120 - -25, Unit: dBm */
    int16_t      rssi;

    /** Uplink BER of the serving cell, Value range: 0 - 1000, Unit: (*10)% */
    uint16_t    ulber;
    
    /** Downlink BER of the serving cell, Value range: 0 - 1000, Unit: (*10)% */
    uint16_t    dlber;
}
ApexMmRssiInd;

/** The ApexMmPlmnListReq signal is used to request a list of all
 * available PLMNs. The HPLMN, RPLMN, FPLMNs and weak signal PLMNs will be flagged.
 */
typedef struct ApexMmPlmnListReqTag
{
    /** Requesting task ID (e.g TASK_FL_ID) */
    TaskId      taskId;

    /** Indicates which access technologies to search for PLMNs
     * (combinations of GSM, UMTS FDD, UMTS TDD, LTE TD and LTE FDD).
     */
    NetworkModes networkModesToSearch;
}
ApexMmPlmnListReq;

/** Send a list of available  PLMNs.
 * The ApexMmPlmnListCnf signal provides an ordered list of available
 * PLMNs to the Controlling Task in response to a previous ApexMmPlmnListReq signal.
 */
typedef struct ApexMmPlmnListCnfTag
{
    /** Always TASK_BL_ID */
    TaskId             taskId;
    Boolean            plmnArrayIsValid;  /**< FALSE if ABMM could not fetch the
                                           * available list e.g because the
                                           * PS is not initialised yet.
                                           */
    /** An array of PLMNs for presentation to the user for selection. */
    PlmnArrayStruct    plmnArray;
    /** The status of PLMN list */
    AbmmPlmnListStatus status;
}
ApexMmPlmnListCnf;

/** The ApexMmPlmnSelectReq signal indicates to Mobility Management
 * that the user wishes to try and register onto a specific PLMN (manual PLMN selection)
 * or to initiate automatic PLMN selection.
 * The corresponding response (failure or success) will be indicated via an ApexMmPlmnSelectCnf.
 * Note that this response may take up to 15 seconds.
 */
typedef struct ApexMmPlmnSelectReqTag
{
    /** ID of requesting task. */
    TaskId      taskId;
    /** TRUE if Mobility Management is to use the manual PLMN selection mode algorithm
     * FALSE if Mobility Management is to use the automatic PLMN selection mode algorithm.

\assoc PRESENT \ref requestedPlmn */
    Boolean     useManualMode;
    /** PLMN on which to attempt registration (only if useManualMode is TRUE). */
    Plmn        requestedPlmn;

    /** Is only applicable in manual mode where user can
     * specify an access technology for the requested PLMN
     */
    NetworkModes modeToPlmnSelect;

    /* 0206-18245 new field returnToRplmn.*/
    /**If useManualMode is FALSE and returnToRplmn is TRUE, then abmm
     * tries RPLMN first instead of doing search for usual user requested
     * automatic network selection (This could be used to return to the RPLMN
     * if a manual network selection failed on a forbidden network).
     */
    Boolean     returnToRplmn;     /* 0206-18245 */
}
ApexMmPlmnSelectReq;

/** The ApexMmPlmnSelectCnf signal contains the result of a previous
 * selection request via an ApexMmPlmnSelectReq signal.
 */
typedef struct ApexMmPlmnSelectCnfTag
{
    /** Always TASK_BL_ID */
    TaskId           taskId;
    /** The outcome of the select request */
    PlmnSelectResult result;
    ServiceStatus    serviceStatus; /**< PLMN_xx values.  For Manual requests,
                                     * the full range of codes can be returned
                                     * (and should NOT be interpreted as the
                                     * new N/W State).  This is because only
                                     * a single registration attempt was made.
                                     * For auto modes, the PLMN_... codes
                                     * relate to the net result of trying all
                                     * available PLMNs.
                                     */
    /** Whether or not to use the manual PLMN selection mode algorithm
     * (Copied from the request).

\assoc PRESENT \ref requestedPlmn */
    Boolean          useManualMode;
    /** PLMN on which to attempt registration (Copied from the request) */
    Plmn             requestedPlmn;
    /** For successful PLMN selection this indicates the registered PLMN */
    PlmnName         selectedName;  /* Filled in as the actual name structure */
    /** Level of service achieved */
    ServiceType      serviceType;   /* 0207-19059 */

    /** The current access technology used. */
    NetworkModes     networkMode;
}
/** Contains the result of a previous selection request via an ApexMmPlmnSelectReq signal.
 * This signal is only sent when the results for circuit switched and packet switched
 * registration arrive separately in the Background Layer. The requesting task may receive
 * zero or one ApexMmPlmnSelectInd before the ApexMmPlmnSelectCnf that signals the end of
 * the selection request. The contents are identical to the ApexMmPlmnSelectCnf.
 */
ApexMmPlmnSelectInd,
ApexMmPlmnSelectCnf;

/** The ApexMmReadPlmnSelReq signal is a request to read the preferred
 * PLMN list from the SIM. The list is read in blocks by specifying the
 * required start field and the number of entries to read, as the ApexMmReadPlmnSelCnf
 * can only contain up to PLMN_READ_LIMIT entries.
 */
typedef struct ApexMmReadPlmnSelReqTag
{
    /** Requesting task ID */
    TaskId           taskId;
    /** Index at which to start reading */
    Int16            startField;
    /** Number of entries to return */
    Int8             numEntriesDesired;
    /** The PLMN selector */
    AbmmPlmnSelector plmnSelector;
} ApexMmReadPlmnSelReq;

/** The ApexMmReadPlmnSelCnf signal is a response to the request to read
 * the preferred PLMN list from the SIM.
 */
typedef struct ApexMmReadPlmnSelCnfTag
{
    /** Always TASK_BL_ID */
    TaskId           taskId;
    /** FALSE if no valid data returned */
    Boolean          plmnArrayIsValid;
    /** Copied from request */
    Int16            startField;
    /** Entries in plmnArray */
    Int8             numEntriesRead;
    /** This indicates the number of VALID entries in the cache*/
    Int16            totalNumEntries;
    /** Total number of entries on the SIM.
     * This indicates the number of entries that the SIM/USIM can hold.
     * This number is only of interest for the user preferred PLMNs.
     * Other preferred PLMNs files cannot be written.
     */
    Int16            totalNumSimEntries;
    /** Holds the list of PLMNs which are read */
    PlmnId           plmnArray[PLMN_READ_LIMIT];
    /** The PLMN selector */
    AbmmPlmnSelector plmnSelector;
} ApexMmReadPlmnSelCnf;

/** Request to update the preferred PLMN list.
 * The ApexMmWritePlmnSelReq signal is a request to update the preferred PLMN
 * list on the SIM. PLMNs may be added, deleted, appended or changed.
 *
 * The PLMN list on the SIM contains numerical representations of PLMNs
 * (i.e. the Mobile Country Code and Mobile Network Code). In the signal
 * the PLMN can be identified using the numerical form in plmnNumber, or
 * by the name in PlmnName. In the latter case, ABMM will look up the numerical
 * form by searching the PLMN lists. If the name cannot be found then the write will fail.
 */
typedef struct ApexMmWritePlmnSelReqTag
{
    /** Requesting task ID. */
    TaskId         taskId;
    /** Gives the index at which to insert or delete the PLMN
     * (Only used when writeAction is insert or delete).
     */
    Int16          entryNumber;
    /** Specifies the type of write action to perform */
    PlmnMenuAction writeAction;
    /** If plmnName is valid */
    Boolean        plmnNamePresent;
    /** Specifies the format of PLMN name */
    PlmnName       plmnName;
    /** If plmnNumber is valid */
    Boolean        plmnNumberPresent;
    /** Numerical form that identifies a PLMN */
    Plmn           plmnNumber;
    /** TRUE if it is a 3 digit MNC */
    Boolean        threeDigitMnc;
}
ApexMmWritePlmnSelReq;

/** The ApexMmWritePlmnSelCnf signal is a response to the
 * request to update the preferred PLMN list on the SIM.
 */
typedef struct ApexMmWritePlmnSelCnfTag
{
    /** Always TASK_BL_ID */
    TaskId          taskId;
    /** Determines the outcome of the request */
    MmRequestStatus writeResult;
}
ApexMmWritePlmnSelCnf;

/** The ApexMmPlmnListStatsReq signal is a request for statistics
 * on the PLMN lists, for instance the user-defined PLMN list in
 * NVRAM and the standard PLMN list in ROM.
 */
typedef struct ApexMmPlmnListStatsReqTag
{
    /** Requesting task ID. */
    TaskId taskId;
}
ApexMmPlmnListStatsReq;

/** The ApexMmPlmnListStatsCnf signal is a response
 * to the request for statistics on the PLMN lists.
 */
typedef struct ApexMmPlmnListStatsCnfTag
{
    /** Always TASK_BL_ID */
    TaskId          taskId;
    /** Always MM_REQ_OK */
    MmRequestStatus status;
    /** Total number of PLMNs in the PLMN lists (for instance sum of numbers below).*/
    Int16           maxNumPlmns;
    /** Total number of PLMNs in the standard PLMN list in ROM. */
    Int16           maxNumFixedPlmns;
    /** Maximum number of user-defined PLMNs that can be stored in
   * NVRAM (NVRAM_PLMN_LIST_SIZE, default of 5).
   */
    Int16           maxNumUserDefPlmns;
}
ApexMmPlmnListStatsCnf;

/** The ApexMmBandInd signal is sent by the Background
 * Layer when a band change has occurred and at
 * start-up when the operating band mode has first been established.
 */
typedef struct ApexMmBandIndTag
{
     /** Always TASK_BL_ID. */
    TaskId          taskId;
}
ApexMmBandInd;

/** The ApexMmReadBandModeReq signal is used to read the current band mode. */
typedef struct ApexMmReadBandModeReqTag
{
    /** Always TASK_BL_ID. */
    TaskId taskId;
}
ApexMmReadBandModeReq;

/** The ApexMmReadBandModeCnf signal confirms a read of the current band mode.*/
typedef struct ApexMmReadBandModeCnfTag
{
    /** Always TASK_BL_ID.*/
    TaskId         taskId;
    /** Frequency band of camped on cell */
    EutraBand      band;


    /* TODO: Can be removed when all dependencies solved */
    Boolean        listIsValid;      /**< FALSE if ABMM could not fetch the
                                      * Band information or GPRS class/service type.*/
   /**  All RAT bands supplied in current and supported BandMode arrays.
    * To incorporate LTE band modes the bit map needs to be extended in size to Int32.
    */
    Int32          currentBandMode[NUM_OF_RATS];
    Int32          supportedBandMode[NUM_OF_RATS];
    /** indicates the current user mode access technology. */
    NetworkModes   currentUserMode;
    /** Indicates the current PLMN access technology. */
    NetworkModes   networkMode;

     /** Determines the type of service (CS service, PS service, or both) */
    ServiceType    serviceType;
    /** Indicates Class A, B or C. */
    GprsMsClass    gprsMsClass;
    /** Multislot class */
    Int8    MultislotClass;

    /** CS/PS Mode */
    CsPsMode       csPsMode;

    AbmmNetworkModePreference   acqOrder;

}
ApexMmReadBandModeCnf;

/* Following singal is used to set or clear frequency/cell lock */
typedef struct ApexMmLockArfcnReqTag
{
    TaskId          taskId;
    /** Lock Arfcn command mode **/
    MmLockArfcnCmdMode  lockArfcnCmdMode;
    /* Lock status field indicates if frequency lock is set or released */
    Boolean         lockStatus;
    /** Frequency lock info(, structure declared in as_nas_common_typ.h) */
    CellLockInfo    lockInfo;
}
ApexMmLockArfcnReq;

/** Following siganl is response signal for frequency lock request
  * if parameter are not valid, writeResult is FALSE */
typedef struct ApexMmLockArfcnCnfTag
{
    /** Always TASK_BL_ID. */
    TaskId          taskId;
    /** Whether write was successful */
    MmRequestStatus writeResult;
    /*assoc Present lockInfo*/
    Boolean         lockInfoPresent;
    /** Frequency lock info(, structure declared in as_nas_common_typ.h) */
    CellLockInfo    lockInfo;
}
ApexMmLockArfcnCnf;

/** following signal is used to request cell information from NAS/AS */
typedef struct ApexMmUeStatsReqTag
{
    TaskId                  taskId;
    /** Request type */
    RequestedModemInfo      requestedInfo;
}ApexMmUeStatsReq;

/** Following signal includes the current cell information */
typedef struct ApexMmUeStatsCnfTag
{
    /** Always TASK_BL_ID. */
    TaskId          taskId;
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
}ApexMmUeStatsCnf;

/** following signal is used to request cell information from NAS/AS */
typedef struct ApexMmLocCellInfoReqTag
{
    TaskId                  taskId;
}ApexMmLocCellInfoReq;

/** Following signal includes the serving cell and neighbour cell information */
typedef struct ApexMmLocCellInfoCnfTag
{
    /** Always TASK_BL_ID. */
    TaskId          taskId;
    
    /** ERRC state info */
    ErrcStateEngInfo                    errcStateEngInfo;

    /** \assoc PRESENT \ref servingCellInfo */
    bool                                servingCellInfoPresent;
    LocationCellInfo                    servingCellInfo;

    /** \assoc ARRAY \ref neighbourCellInfo */
    uint8_t                             neighbourCellInfoNum;
    LocationCellInfo                    neighbourCellInfo[NCELL_INFO_MAX_NUM];
}ApexMmLocCellInfoCnf;

/** Following signal is used to start or stop EMM status indication */
typedef struct ApexMmCsconReqTag
{
    TaskId          taskId;
    /** If result code = 1, start indication. If result code = 0 stop indication */
    Boolean         resultCode;
}
ApexMmCsconReq;

/** Response signal for for EMM status indication request */
typedef struct ApexMmCsconCnfTag
{
    /** Always TASK_BL_ID. */
    TaskId          taskId;
    /** Status is false if EMM cannot indicate status, for example if SIM is not valid */
    Boolean         status;
    /*return current EMM connection status to ATCI*/
    Boolean         emmConStatus;
}
ApexMmCsconCnf;

/** With this signal the current EMM status is indicated */
typedef struct ApexMmCsconIndTag
{
    /** Always TASK_BL_ID. */
    TaskId          taskId;
    /** The current EMM status, TRUE = EMM connected, otherwise IDLE */
    Boolean         status;
}
ApexMmCsconInd;


/** The ApexMmSuspendReq signal indicates to Mobility Management that the user
 * wishes to suspend operation on the current PLMN. The ABMM state is saved and
 * the Protocol Stack put into a state where network access is prevented.
 * When an ApexMmResumeReq is received then ABMM will attempt to restore service
 * to what it was before suspension.
 *
 * If silent is FALSE then ABMM will detach from the network and the Protocol
 * Stack will enter limited service state.
 *
 * If silent is TRUE then the Protocol Stack will be immediately deactivated
 * without detaching from the network. The handset should not be left in this
 * state for a long time as the PLMN may implicitly detach the handset if periodic
 * location registration procedures are not carried out by the handset.
 *
 * <B> IMPORTANT: </B> <UL>
 * <LI> The suspend signal with silent=TRUE must be used with great care!  In this,
 * suspend mode lower layers are put into an abnormal state where RR acts as a
 * buffer layer between Layer 1 and MM so that every request from MM is bounced
 * and any activity from below is ignored.  The purpose of this state is to prevent
 * the lower layers attempting to establish while other (radio) hardware is
 * active.
 * If, for instance, a periodic location update timer goes off in MM, RR will
 * bounce the request to establish from MM and MM will try again 15 seconds later,
 * etc, and so if the suspended state is maintained for very long the mobile may
 * be assumed to have been switched off by the network(and therefore not be paged)
 * when it's really still there.  It is recommended that the silent suspend time is as
 * short as possible, e.g. shorter than 15 seconds.  Obviously, the mobile will
 * not respond to paging during suspension. </UL>
 */
typedef struct ApexMmSuspendReqTag
{
    /** Requesting task ID */
    TaskId  taskId;
    /** If TRUE the handset will not detach from the network before suspending.*/
    Boolean silent;
}
ApexMmSuspendReq;

/** The ApexMmSuspendCnf signal is a response to the request to suspend services. */
typedef struct ApexMmSuspendCnfTag
{
    /** Always TASK_BL_ID */
    TaskId          taskId;
    /** The outcome of the request */
    MmRequestStatus requestStatus;
}
ApexMmSuspendCnf;

/** Request to resume operation.
 * The ApexMmResumeReq signal indicates to Mobility Management that
 * the user wishes to resume operation. ABMM continues the previous PLMN
 * selection procedure, or initiates a camp or registration procedure,
 * depending on the state stored when ABMM was suspended.
 */
typedef struct ApexMmResumeReqTag
{
    /** Requesting task ID */
    TaskId taskId;
}
ApexMmResumeReq;

/** Send a resume confirm.
 * The ApexMmResumeCnf signal is a response to the request to resume services.
 */
typedef struct ApexMmResumeCnfTag
{
    /** Always TASK_BL_ID */
    TaskId          taskId;
    /** The outcome of the request. */
    MmRequestStatus requestStatus;
}
ApexMmResumeCnf;

/* added for 0204-17315 */
/** The ApexMmDeregisterReq signal indicates to Mobility Management
 * that the user wishes to deregister all services from the currently
 * selected PLMN. The corresponding response (failure or success) will
 * be indicated via an ApexMmDeregisterCnf.
 */
typedef struct ApexMmDeregisterReqTag
{
    /** Requesting task ID */
    TaskId taskId;
}
ApexMmDeregisterReq;

/** The ApexMmDeregisterCnf signal is a response to the request to deregister services.
 * This signal indicates success or failure of the request.
 */
typedef struct ApexMmDeregisterCnfTag
{
    /** Always TASK_BL_ID */
    TaskId          taskId;
    /** The outcome of the request */
    MmRequestStatus requestStatus;
}
ApexMmDeregisterCnf;

/** Request to resume operation. */
typedef struct AbmmResumeReqTag
{
    /** The sending task ID */
    TaskId  taskId;
    /** Whether the state of SIM is OK */
    Boolean simIsOK;
    /** Whether power up SIM */
    Boolean simIsPowerUp;
}
AbmmResumeReq;

/** The AbmmResumeCnf signal is a response to the request to deregister services.
 * This signal indicates success or failure of the request.
 */
typedef struct AbmmResumeCnfTag
{
    /** The sending task ID */
    TaskId          taskId;
    /** The outcome of the request */
    MmRequestStatus requestStatus;
}
AbmmResumeCnf;

/** The ApexMmAbortPlmnListReq signal is used to cancel a PLMN
 * list request. Note that this is the only signal that can be used
 * to cancel a PLMN search; the ApexMmPlmnSelectReq should not be used.
 */
typedef struct ApexMmAbortPlmnListReqTag
{
    /** Requesting task ID. */
    TaskId taskId;
}
ApexMmAbortPlmnListReq;

/** The ApexMmAbortPlmnListCnf is a response to a request
 * to cancel a PLMN list request.
 */
typedef struct ApexMmAbortPlmnListCnfTag
{
    /** Always TASK_BL_ID */
    TaskId          taskId;
    /** Outcome of the request
     * (In this case the status can only be either
     * MM_REQ_OK or MM_REQ_NOT_CONTROL_TASK)
     */
    MmRequestStatus requestStatus;
}
ApexMmAbortPlmnListCnf;

/** The ApexMmNetworkInfoInd signal passes the layer 3 MM
 * information message received from the network to
 * all registered foreground layersMMI / Voyager Op.
 * It is an exact copy of the MmrNetworkInfoInd signal sent
 * from MM to ABMM.
 */
typedef struct ApexMmNetworkInfoIndTag
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
    /** Whether LSA is present
\assoc PRESENT \ref lsaIdentity */
    Boolean                   lsaIdentityPresent;
    /** uniquely identifies a LSA.*/
    Int32                     lsaIdentity;
}
ApexMmNetworkInfoInd;

/** This signal is used to provide a Ciphering Indicator as specified */
typedef struct ApexMmCipherIndTag
{
    /** Circuit switched ciphering enabled (TRUE if MS is currently
     * in dedicated mode and ciphering is active).
     */
    Boolean                csCipheringOn;
    /** Last CS ciphering algorithm used */
    CipherAlgorithm        csAlgorithm;
    /** Packet switched ciphering enabled ( TRUE if GPRS ciphering
     * has been enabled. MS may or may not be in packet mode) .
     */
    Boolean                psCipheringOn;
    /** Last GPRS ciphering algorithm used */
    GprsCipheringAlgorithm psAlgorithm;
}
ApexMmCipherInd;

/** Request to write Power On Options */
typedef struct ApexMmWritePwonOptionsReqTag
{
    /** The sending task ID */
    TaskId                taskId;
    /** Command reference */
    Int16                 commandRef;
    /** GPRS attached power up options */
    ApexMmGAPowerUpOption gaOption;
}  ApexMmWritePwonOptionsReq;

/** Indication of success or failure of the request */
typedef struct ApexMmWritePwonOptionsCnfTag
{
    /** The sending task ID */
    TaskId          taskId;
    /** Command reference copied from ApexMmWritePwonOptionsReq. */
    Int16           commandRef;
    /** Outcome of the request */
    MmRequestStatus requestStatus;
}  ApexMmWritePwonOptionsCnf;

/** Request to read Power On Options. */
typedef struct ApexMmReadPwonOptionsReqTag
{
    /** The sending task ID */
    TaskId taskId;
    /** Command reference */
    Int16  commandRef;
}   ApexMmReadPwonOptionsReq;

/** This signal send read power up options confirm to the foreground layer. */
typedef struct ApexMmReadPwonOptionsCnfTag
{
    /** The sending task ID (TASK_BL_ID) */
    TaskId                taskId;
    /** Command reference copied from corresponding request. */
    Int16                 commandRef;
    /** Outcome of the request */
    MmRequestStatus       requestStatus;
    /** GPRS Attach At Power Up options */
    ApexMmGAPowerUpOption gaOption;
}  ApexMmReadPwonOptionsCnf;

#if defined (ENABLE_L23_DEBUG) || defined (ENABLE_ABMM_DEBUG)

typedef struct AbmmRegStatusDebugInfoIndTag
{
    RegStatusInfo    regStatusInfo;
    RegCtrlInfo      regCtrlInfo;
    PlmnListInfo     plmnListInfo;
}AbmmRegStatusDebugInfoInd;
#endif

/* 0107-11765*/
/** ABMM debug signal */
typedef struct AbmmDebugInfoReqTag
{
    /** Sending task ID */
    TaskId            taskId;
    /** Command reference */
    Int16             commandRef;
    /** The type of ABMM debug information */
    AbmmDebugInfoType debugInfoType;
} AbmmDebugInfoReq;

/** Confirmation of the AbmmDebugInfoReq request */
typedef struct AbmmDebugInfoCnfTag
{
    /** Sending task ID */
    TaskId              taskId;
    /** Command reference copied from the corresponding request */
    Int16               commandRef;
    /** The type of ABMM debug information copied from the corresponding request
\assoc UNION \ref debugInfo */
    AbmmDebugInfoType   debugInfoType;
    /** The result of the request */
    AbmmDebugInfoStatus status;
    /** Debug information */
    AbmmDebugInfo       debugInfo;
} AbmmDebugInfoCnf;

/** This signal is send when user has used AT-commend to change eDRX values */
typedef struct ApexMmSetEdrxReqTag
{
    /** Requesting task Id */
    TaskId          taskId;
    /** Define if eDRX is supported or not */
    Boolean         userEdrxSupport;
    /** User requested eDRX value */
    Int8            userEdrxValue;
    /** Define if PTW is present or not */
    Boolean         userPtwPresence;
    /** User requested PTW value */
    Int8            userPtwValue;
    /** User requested to use default values */
    Boolean         resetToDefaultValues;
}
ApexMmSetEdrxReq;

/** This signal is sent as response for ApexMmSetEdrxReq */
typedef struct ApexMmSetEdrxCnfTag
{
    /** The sending task Id */
    TaskId          taskId;
    /** Status field, indicates if EMM could take values in use or not */
    Boolean         status;
}
ApexMmSetEdrxCnf;

/** This signal is sent when user has asked used eDRX values */
typedef struct ApexMmReadEdrxReqTag
{
    /** Requesting task Id */
    TaskId          taskId;
}
ApexMmReadEdrxReq;

/** This signal is sent as response for ApexMmReadEdrxReq */
typedef struct ApexMmReadEdrxCnfTag
{
    /** The sending task Id */
    TaskId          taskId;
    /** Used user and network eDRX values */
    EdrxInfo        edrxInfo;
}
ApexMmReadEdrxCnf;

/** With this signal, ATCI requests EMM via ABMM to alter CIoT EPS feature. */
typedef struct ApexWriteIotOptCfgReqTag
{
    /** Requesting task ID. */
    TaskId              taskId;
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
ApexWriteIotOptCfgReq;

/** This message is response signal for ApexWriteIotOptCfgReq */
typedef struct ApexWriteIotOptCfgCnfTag
{
    /** The sending task Id */
    TaskId          taskId;
}
ApexWriteIotOptCfgCnf;

/** With this signal ATCI requests CIoT EPS values form EMM via ABMM.*/
typedef struct ApexReadIotOptCfgReqTag
{
    /** Requesting task ID. */
    TaskId          taskId;
}
ApexReadIotOptCfgReq;

/** With this signal ABMM sends CIoT EPS optimization values to ATCI */
typedef struct ApexReadIotOptCfgCnfTag
{
    /** The sending task Id */
    TaskId              taskId;
    /** UE preferred optimization */
    CiotPreference      ciotPreference;
    /** UE capability for CIoT EPS feature. */
    SupportedIotOptions supportedUeOptions;
}
ApexReadIotOptCfgCnf;

/** Sent from ATCI to ABMM when user wants to read PSM values by using
 * AT commands.
 */
typedef struct ApexReadPsmConfReqTag
{
    /** Requesting task ID. */
    TaskId          taskId;
}
ApexReadPsmConfReq;

/** Sent from ABMM to ATCI, signal includes requested and network PSM parameters.
 *  (PsmResponseInfo is defined in the file mmr_sig.h)
 */
typedef struct ApexReadPsmConfCnfTag
{
    /** The sending task Id */
    TaskId              taskId;
    /** Requested and NW PSM parameters */
    PsmResponseInfo psm;
}
ApexReadPsmConfCnf;

/** Sent from ATCI to ABMM when user wants to read UE information indicated by information type.
 */
typedef struct ApexReadUeInfoReqTag
{
    /** Requesting task ID. */
    TaskId          taskId;
    UeInfoType      ueInfoType;
}
ApexReadUeInfoReq;

/** Sent from ABMM to ATCI, signal includes requested UE information indicated by information type */
typedef struct ApexReadUeInfoCnfTag
{
    /** The sending task Id */
    TaskId            taskId;
    /** The ueInfoType
     \assoc UNION \ref ueInfoBody */
    UeInfoType        ueInfoType;
    UeInfoBody        ueInfoBody;
}
ApexReadUeInfoCnf;


/** Sent from ACTI to ABMM after user has requested setting of PSM values by using
 * AT commands.
 *  (PsmRequestInfo is defined in the file mmr_sig.h)
 */
typedef struct ApexWritePsmConfReqTag
{
    /** Requesting task ID. */
    TaskId          taskId;
    /** User requested PSM parameters */
    PsmRequestInfo psm;
}
ApexWritePsmConfReq;

/** Sent from ABMM to ATCI as a response for user PSM configuration
 */
typedef struct ApexWritePsmConfCnfTag
{
    /** The sending task Id */
    TaskId              taskId;
    /** Success code, if EMM has accepted the changes or not */
    Boolean success;
}
ApexWritePsmConfCnf;


#ifdef FEA_NFM

/** By using of AT command this signal is sent from ATCI to ABMM to
 *switch the NFM on and off.
 *  (WriteNfmReq is defined in the file mmr_sig.h)
 */
typedef struct ApexWriteNfmReqTag
{
    /** The sending task Id */
    TaskId              taskId;
    WriteNfmReq       writeNfmReq;
}
ApexWriteNfmReq;

/** Sent from ABMM to ATCI as a response for AT NFM configuration
 */
typedef struct ApexWriteNfmCnfTag
{
    /** The sending task Id */
    TaskId              taskId;
    Boolean             success;
}
ApexWriteNfmCnf;

/** By using of AT command this signal is sent from ATCI to ABMM to
 *  query for a report of the currently stored parameters.
 *  (WriteNfmReq is defined in the file mmr_sig.h)
 */

typedef struct ApexReadNfmReqTag
{
    /** The sending task Id */
    TaskId              taskId;
}
ApexReadNfmReq;

/** Sent from ABMM to ATCI as a response for AT NFM parameters query.
 */
typedef struct ApexReadNfmCnfTag
{
    /** The sending task Id */
    TaskId              taskId;
    ReadNfmCnf          readNfmCnf;
}
ApexReadNfmCnf;

/** By using of AT command this signal is sent from ATCI to ABMM to
 *  query for a report of the remaining time for NFM timers.
 */

typedef struct ApexReadNfmStatusReqTag
{
    /** The sending task Id */
    TaskId              taskId;
}
ApexReadNfmStatusReq;

/** Sent from ABMM to ATCI as a response for AT NFM status query.
 */
typedef struct ApexReadNfmStatusCnfTag
{
    /** The sending task Id */
    TaskId              taskId;
    ReadNfmStatusCnf    readNfmStatusCnf;
}
ApexReadNfmStatusCnf;

/** By using of AT command this signal is sent from ATCI to ABMM
 *  to configure the Back-off Base intervals.
 *  (WriteNfmConfReq is defined in the file mmr_sig.h)
 */
typedef struct ApexWriteNfmConfReqTag
{
    /** The sending task Id */
    TaskId              taskId;
    WriteNfmConfReq     writeNfmConfReq;
}
ApexWriteNfmConfReq;

/** Sent from ABMM to ATCI as a response for AT NFM Back-off configuration.
 */
typedef struct ApexWriteNfmConfCnfTag
{
    /** The sending task Id */
    TaskId              taskId;
    Boolean             success;
}
ApexWriteNfmConfCnf;

/** By using of AT command this signal is sent from ATCI to ABMM to
 *  query for a report of the currently stored parameters.
 *  (ReadNfmConfReq is defined in the file mmr_sig.h)
 */
typedef struct ApexReadNfmConfReqTag
{
    /** The sending task Id */
    TaskId              taskId;
}
ApexReadNfmConfReq;

/** Sent from ABMM to ATCI as a response for AT NFM parameters query.
 */
typedef struct ApexReadNfmConfCnfTag
{
    /** The sending task Id */
    TaskId              taskId;
    ReadNfmConfCnf      readNfmConfCnf;
}
ApexReadNfmConfCnf;

#endif /* FEA_NFM */


/** Sent when user has requested change for attach with/without PDN connecton,
 * using AT-command +CIPCA
 * With this signal, ATCI requests EMM to use the user requested setting for attach.
 */
typedef struct ApexMmWriteAttachPdnCfgReqTag
{
    /** Requesting task ID. */
    TaskId          taskId;
    /** User requested PDN connection setting during attach
      * If TRUE attach is done without PDN connection */
    Boolean         attachWithoutPdn;
}
ApexMmWriteAttachPdnCfgReq;

/**Sent as a response for the signal SIG_APEX_MM_WRITE_ATTACH_PDN_CONF_REQ
 * With this signal, EMM indicates that new PDN connection setting is taken into use.
 */
typedef struct ApexMmWriteAttachPdnCfgCnfTag
{
    /** The sending task Id */
    TaskId              taskId;
    /** Success code, if EMM has accepted the change or not */
    Boolean success;
}
ApexMmWriteAttachPdnCfgCnf;

/** Sent when user has requested the current setting for attach with/without PDN connecton,
 * using AT-command +CIPCA
 */
typedef struct ApexMmReadAttachPdnCfgReqTag
{
    /** Requesting task ID. */
    TaskId          taskId;
}
ApexMmReadAttachPdnCfgReq;

/**Sent as a response for the signal SIG_APEX_MM_READ_ATTACH_PDN_CONF_REQ
 * With this signal ABMM indicates the current PDN connection setting.
 */
typedef struct ApexMmReadAttachPdnCfgCnfTag
{
    /** The sending task Id */
    TaskId              taskId;
    /** Current PDN connection setting during attach
      * If TRUE attach is done without PDN connection */
    Boolean         attachWithoutPdn;
}
ApexMmReadAttachPdnCfgCnf;

/**
 * This signal is used to indicate the current serving PLMN to ABPD
 */
typedef struct AbmmCurrentPlmnIndTag
{
    Plmn    currentPlmn;
}
AbmmCurrentPlmnInd;

/** The ApexMmWriteMobileIDReq signal is used to update IMEI/SN.
*/
typedef struct ApexMmWriteMobileIdReqTag
{
    /** Requesting task ID().  */
    TaskId taskId;
    /**IMEI ID length */
    Int16          digitImeiArraySize;
    Int16          digitSNArraySize;

    Int8           digitMobileId[MAX_UE_ID_LENGTH];
}
ApexMmWriteMobileIdReq;

/** The ApexMmWriteMobileIDCnf is a response to
 * a update IMEI/SN Request
 */
typedef struct ApexMmWriteMobileIdCnfTag
{
    TaskId          taskId;
    /** Outcome of the request
     * (In this case the status can only be either
     * MM_REQ_OK or MM_REQ_NOT_CONTROL_TASK)
     */
    MmRequestStatus requestStatus;
}
ApexMmWriteMobileIdCnf;

/** The ApexMmReadMobileIDReq signal is used to update IMEI/SN/SVN.
*/
typedef struct ApexMmReadMobileIdReqTag
{
    /** Requesting task ID().  */
    TaskId taskId;

    MobileIDName name;
}
ApexMmReadMobileIdReq;

/** The ApexMmReadMobileIDCnf is a response to
 * a update IMEI/SN/SVN Request
 */
typedef struct ApexMmReadMobileIdCnfTag
{
    TaskId          taskId;
    MmRequestStatus requestStatus;
    /** Outcome of the request
     * (In this case the status can only be either
     * MM_REQ_OK or MM_REQ_NOT_CONTROL_TASK)
     */
    MobileID mobileID;
}
ApexMmReadMobileIdCnf;

/*The ApexMmSearchBandListReq is a signal
 *to set prefer band list by user.
 */
 typedef struct ApexMmSearchBandListReqTag
 {
    TaskId      taskId;
    /*The number of band search list
    * if it is 0, notify abmm to clear user configure preferred band list*/
    Int8        numSearchBand;
    /*user config perfer search band list*/
    NbiotBand   searchBandList[MAX_NUM_USER_CFG_BAND_LIST];

 }ApexMmSearchBandListReq;

/** The ApexMmSearchBandListCnf is a response to
 * a update user set perferred searched band list.
 */
typedef struct ApexMmSearchBandListCnfTag
{
    TaskId          taskId;

    Boolean         result;
}
ApexMmSearchBandListCnf;

typedef struct ApexMmPsmStatusIndTag
{
    TaskId          taskId;

    MmPsmStatus     psmStatus;
}
ApexMmPsmStatusInd;

typedef struct ApexMmSetEhplmnReqTag
{
    TaskId      taskId;

    /*The number of EHPLMN list
    * if it is 0, notify abmm to clear user configure EHPLMN list*/
    Int8        numEhplmn;
    /*user config EHPLMN list*/
    Plmn        ehplmnList[MAX_NUM_USER_CFG_EHPLMN_LIST];
}ApexMmSetEhplmnReq;

typedef struct ApexMmOosaStatusIndTag
{
    /** Always TASK_BL_ID. */
    TaskId          taskId;

   /** bit0: whether enable enter OOSA, True: enter OOSA; */
    uint8_t         oosaStatus;
}
ApexMmOosaStatusInd;


/**
 * This signal is used to configure whether did start HPLMN search timer when roaming
 */
typedef struct ApexMmDisableHplmnSearchReqTag
{
    TaskId          taskId;
    /** TRUE: would not start HPLMN search timer while roaming, 
    *  FALSE: would start HPLMN  search timer while roaming
    */
    Boolean         isDisableHplmnSearch;
}ApexMmDisableHplmnSearchReq;

typedef struct ApexMmDisableHplmnSearchCnfTag
{
    TaskId          taskId;
    Boolean         result;
}ApexMmDisableHplmnSearchCnf;

#ifdef ABMM_UNIT_TEST
/**
 * This signal is used to request ABMM to do deep sleep preparations
 * or do deep sleep wake-up tasks
 */
typedef struct AbmmSetUnitTestModeReqTag
{
  AbmmUnitTestMode          abmmUnitTestMode;
}
AbmmSetUnitTestModeReq;
/**
 * This signal is used to check ABMM after deep sleep preparations
 */
typedef struct AbmmCheckUnitTestModeIndTag
{
  uint32_t          remainingCause42Time;
}
AbmmCheckUnitTestModeInd;
#endif /* ABMM_UNIT_TEST */

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

/** @} */ /* End of SigAbmm group */

#endif /* !(ABMM_SIG_H) */

/* END OF FILE */
