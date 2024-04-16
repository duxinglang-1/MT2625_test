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
 *    Application Layer Background Layer SIM Support Signal Definitions
 **************************************************************************/

#ifndef ABSI_SIG_H
#define ABSI_SIG_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#if !defined(ABSI_TYP_H)
#   include "absi_typ.h"      /* PS AL BL SIM Support                      */
#endif

#if !defined(ALSI_SIG_H)
#   include "alsi_sig.h"      /* PS AL to SIM Manager Signal Definitions   */
#endif


/** \defgroup SigAbsi  ABSI Signalling Interface
 * \ingroup NasApex
 * \ingroup Sig
 * This section covers the APEX Subscriber Identity Module Support
 * interface and the Background Layer SIM Support procedure.
 * @{
 */
/***************************************************************************
 * Manifest Constants
 **************************************************************************/
/** Maximum ME Personalisation data in signal */
#define APEX_SIM_MAX_MEP_IN_SIG 12

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/** The ApexSimReadCnlReq signal requests a read of the co-operative
 * network lists contained in the SIM.
 */
typedef struct ApexSimReadCnlReqTag   /*14092*/
{
   /** Sender's task ID */
   TaskId       taskId;
   /** The reference number that the Background Layer is to
    * return in the response signal (ApexSimReadCnlCnf).
    */
   Int16        commandRef;
}
ApexSimReadCnlReq;

/** The ApexSimReadCnlCnf signal is sent in response to a
 * ApexSimReadCnlReq signal and contains the co-operative
 * network list from the SIM.
 */
typedef struct ApexSimReadCnlCnfTag  /*14092*/
{
  /** Sender's task ID */
  TaskId                          taskId;
  /** Copied from the ApexSimReadCnlReq signal */
  Int16                           commandRef;
  /** The status of the read, set to SIM_REQ_OK if the data is valid */
  SimRequestStatus                requestStatus;
  /** Contains an array of SimCooperativeNetList objects  which contains
   * contains the following info: PLMN, network subset, serviceProviderId, and corporateId
   */
  SimCnlData                      cnlData;
}
ApexSimReadCnlCnf;

#if defined (UPGRADE_CMMB)
typedef struct ApexSimCmmbAuthReqTag
{
/** Sender's task ID */
  TaskId              taskId;
  /** Copied from the ApexSimReadCnlReq signal */
  Int16               commandRef;
  CmmbSecurity        cmmbSecurity;     /** indicates that it is 2G authentication or UMTS authentication. */
  Rand                rand;             /** rand is the random challenge and is from BSF to be used to authenticate. */
  KeySequence         cksn;             /** the key sequence number */
  /** ks-input is the 128bit random number generated by BSF to be used to authenticate.
   * It is only used for 2G authentication.
   */
  CmmbKsInput         ksInput;
  /** the authentication token required by the SIM to calculate
   * the user authentication response and is only UMTS authentication.
   */
  Aut                 autn;
}
ApexSimCmmbAuthReq;

typedef struct ApexSimCmmbAuthCnfTag
{
  TaskId              taskId;
  /** Number copied from the ApexSimChvFunctionReq */
  Int16               commandRef;
  CmmbSecurity        cmmbSecurity;     /** indicates that it is 2G authentication or UMTS authentication */
  SimRequestStatus    requestStatus;    /** indicates the status of the request */
  Res                 res;              /** the user authentication response */
  KeySequence         cksn;             /** the key sequence number */
  Aut                 auts;             /** the authentication token. If the authentication is failure, it is returned */
  /** the ciphering key */
  CipheringKey        kc;
  /** the cipher key */
  CipherKey           ck;
  /** the integrity key */
  IntegrityKey        ik;
}
ApexSimCmmbAuthCnf;
#endif


/** Sent by ABSI to FL to fetch the CHV1 password.
 * This signal is sent to the Controlling Task when the Background
 * Layer requires a CHV1 or unblock CHV1 when a SIM is inserted.
 */
typedef struct ApexSimGetChvIndTag
{
  /** Sender's task ID */
  TaskId                taskId;
  /** Identifies  whether the data is for CHV1 or CHV2 */
  SimChvNum             chvNum;
  /** Indicates if it is for an unblock of CHV password  */
  Boolean               chvBlocked;
  /** Indicates if CHV1 is enabled */
  Boolean               chv1Enabled;
  /** Holds the status of the CHV1 */
  SimChvStatus          chv1Status;
  /** Holds the status of the CHV2 */
  SimChvStatus          chv2Status;
  /** Holds the status of the unblock CHV1 */
  SimChvStatus          unblockChv1Status;
  /** Holds the status of the unblock CHV2 */
  SimChvStatus          unblockChv2Status;
  /** Details of SIM’s preferred language list. */
  LanguagePrefList      languagePrefList;   /* 0201-14905 */
  /** extended language preference list (read from EF ELP on the SIM)*/
  SimExtLangPrefList    extLangPrefList;
}
ApexSimGetChvInd;


/** FL's response to ABSI's request for the CHV1/CHV1 unblock password(s).
 * This signal is sent by the Controlling Task in response to an ApexSimGetChvInd signal.
 * If this signal is for an unblock CHV1 password then it must also include a new value
 * for the CHV1
 */
typedef struct ApexSimGetChvRspTag
{
  /** Responding Task’s ID */
  TaskId                taskId;
  /** SIM CHV1 value */
  SimChvNum             chvNum;
  /** indicates if it is for an unblock CHV1 password */
  Boolean               chvBlocked;
  /** Indicates that the CHV has already been verified by other means
   * ApexSimGenAccessReq/Cnf for instance (If this flag is set to TRUE,
   * the chvValue/ chvUnblockValue fields will be ignored).
   */
  Boolean               chvValueAlreadyVerified;  /*added for job 104913*/
  /** value of the selected CHV  */
  SimChvValue           chvValue;
  /** nly used if chvBlocked is TRUE, when it contains the unblock value
   * for the selected CHV; in which case chvValue is the new CHV password.
   */
  SimChvValue           chvUnblockValue;
}
ApexSimGetChvRsp;

/*added for job 104913*/
/** This signal is sent when the status of one of the PIN has
 * been modified by (for example) the AT+CSIM command.
 */
typedef struct ApexSimChvStatusIndTag
{
   /** Sender's task ID */
   TaskId                          taskId;
   /** CHV number (CHV1 or CHV2)*/
   SimChvNum                       chvNum;
   /** Flag to indicate whether CHV1 is enabled */
   Boolean                         chv1Enabled;
   /** Status of the CHV */
   SimChvStatus                    chvStatus;
   /** Status of the CHV */
   SimChvStatus                    unblockChvStatus;
} ApexSimChvStatusInd;


/** This primitive is sent by ABSI to FL to let it know that the SIM is now good.
 * This signal is sent to controlling/registered tasks when the SIM is OK and CHV(s)
 * have been verified (if required). It carries with it most of the data (available only once)
 * returned by AlsiSimInsertedInd and AlsiSimInitialiseCnf.
 */
typedef struct ApexSimOkIndTag
{
  /** Always TASK_BL_ID */
  TaskId                taskId;
#if defined (FEA_KOR_LGUP)
  CardType              cardType;
#endif
  /** The status of PIN1 (from AlsiAppStartedInd) */
  SimUiccPinStatus      pin1Status;
  /** The status of PIN2 (from AlsiAppStartedInd) */
  SimUiccPinStatus      pin2Status;
  /** The status of unblock PIN1 (from AlsiAppStartedInd) */
  SimUiccPinStatus      unblockPin1Status;
  /** The status of unblock PIN2 (from AlsiAppStartedInd) */
  SimUiccPinStatus      unblockPin2Status;
  /** Whether or not universal PIN is supported */
  Boolean               universalPinSupportedByCard;
  /** The status of universal PIN */
  SimUiccPinStatus      universalPinStatus;
  /** The status of universal unblock PIN */
  SimUiccPinStatus      unblockUniversalPinStatus;
  /** Type of service table (2G or 3G)
\assoc UNION \ref serviceTable */
  ServiceTableType      serviceTableType;
  /** SIM Service Table or USIM Service Table */
  ServiceTable          serviceTable;
  /** Key reference for PIN1 */
  SimUiccKeyRefValue    pin1KeyReference;
  /** Key reference for PIN2 */
  SimUiccKeyRefValue    pin2KeyReference;
  /** Language preference list */
  LanguagePrefList      languagePrefList;
  /** Extended language preference list (read from EF ELP on the SIM)*/
  SimExtLangPrefList    extLangPrefList;
  /** Indicates if PIN2 is required for ACM maximum access */
  Boolean               pin2RequiredAcmMax;
  /** Indicates if PIN2 is required for ACM  access */
  Boolean               pin2RequiredAcm;
  /** Indicates if PIN2 is required for PUCT  access */
  Boolean               pin2RequiredPuct;
/** Whether the FDN is on or not. */
  Boolean               fdnIsEnabled;
  /** Whether the BDN is on or not.*/
  Boolean               bdnIsEnabled;
  /** APN control list*/
  Boolean               aclIsEnabled;
   /** whether cphsData is Present*/
  Boolean               cphsDataPresent;
  /** phase of SIM (SIM PHASE 2, PHASE 2+, or 3G) */
  SimPhase              phase;
  /** International mobile subscriber id */
  Imsi                  imsi;
  /** Location Information (read from EF LOCI) */
  LocationInformation   locInfo;
  /** Period attempts to look for a higher priority PLMN,
   * units of minutes (0 - 480)
   */
  Int16                 hplmnSearchPeriod;
  /** Administation data */
  SimAdminData          admin;
  /** Acting home plmn for enhanced network selection features */
  SimPlmn               actingHplmn;
  /** SIM number */
  Int8                  selectedSim;
  /** SIM already known to handset */
  Boolean               knownSim;
  /*0109-12571*/
  /** Card ID number */
  SimIccid               iccid;
  /** Whether the SIM/UICC supports SIM toolkit */
  Boolean               proactiveSim;
  /** Number of subscriber profiles */
  Int8                  numSubscriberProfiles;
#if defined (ENABLE_DUAL_SIM_SOLUTION)
 /** the selected SIM Holder*/
  SimHolderType    simHolder;
  /** the number of SIM holders present 1 or 2 */
  Int8             numSimHolders;
#endif
}
ApexSimOkInd;


/** This signal is sent to the Background Layer procedures when the SIM is OK.
 * Internal BL signal to let procedures know that the SIM is OK.
 * It is sent whenever an ApexSimOkInd is sent.
 *
 * BL procedures are expected to fetch SIM data from ABSI by function call */
typedef struct AbsiSimOkSigTag
{
  /** ID of the requesting task */
  TaskId                taskId;
}
AbsiSimOkSig;


/** This signal is sent by ABSI to FL to inform it that the SIM is no good.
 * The ApexSimNokInd signal is sent to the Contolling/Registered Tasks when
 * a bad SIM is detected, required CHV codes are not secretCodeInitialised
 * on the SIM, communications problems, or CHV1 block code retries have all
 * been used.
 *
 * This signal will report the preferred language list if the SIM is present
 * but has failed some checks (e.g. personalisation checks) or if the SIM has
 * been removed.
 *
 * This signal may also be sent if the network has rejected the SIM, in which
 * case the reason field will be set to SIM_NOK_NETWORK_REJECT. In this particular
 * case, the phonebook can still be accessed.
 */
typedef struct ApexSimNokIndTag
{
  /** ID of the requesting task */
  TaskId                taskId;
  /** Reason why the currently inserted SIM is not valid/working */
  SimNokReason          reason;
  /** Whether or not PUK retries expired */
  Boolean               puk1RetriesExpired;
  /** SIM preferred language list */
  LanguagePrefList      languagePrefList;   /* 0202-13707 */
  /** Extended language preference list */
  SimExtLangPrefList    extLangPrefList;
#if defined (ENABLE_DUAL_SIM_SOLUTION)
 /** the selected SIM Holder*/
  SimHolderType    simHolder;
  /** the number of SIM holders present 1 or 2 */
  Int8             numSimHolders;
#endif

}
ApexSimNokInd,
AbsiSimNokSig;


/** This signal is sent to ABSI by FL handlers.
 * The ApexSimChvFunctionReq signal is sent by the Controlling Task to
 * Background Layer task to perform a CHV function (Verify, Change, Unblock, Enable, Disable).
 * ABSI sends CHV info to ALSI for verification.
 */
typedef struct ApexSimChvFunctionReqTag
{
  /** ID of the requesting task. */
  TaskId                taskId;
  /** Number assigned by the Controlling Task */
  Int16                 commandRef;
  /** Verify, change, disable, enable, or unblock */
  SimChvFunction        chvFunction;
  /** CHV1 or CHV2. */
  SimChvNum             chvNum;
  /** The CHV or unblock CHV for unblock functions */
  SimChvValue           chvValue;
  /** The new CHV (for change or unblock functions)*/
  SimChvValue           newChvValue;
}
ApexSimChvFunctionReq;


/** ABSI's response to FL's attempt to verify CHV info.
 * is sent by the Background Layer task to the Foreground Layer
 * task to indicate the success or otherwise of a requested CHV function.
 * See SimRequestStatus, SimChvFunction, SimChvNum and SimChvStatus.
 */
typedef struct ApexSimChvFunctionCnfTag
{
  /** Always TASK_BL_ID */
  TaskId                taskId;
  /** Number copied from the ApexSimChvFunctionReq */
  Int16                 commandRef;
  /** The result of the ApexSimChvFunctionReq */
  SimRequestStatus      requestStatus;
  /** Verify, change, disable, enable or unblock. */
  SimChvFunction        chvFunction;
  /** CHV1 or CHV2. */
  SimChvNum             chvNum;
  /** Indicates if CHV function request was successful */
  Boolean               chvFunctionSuccess;
  /** Number of remaining retries and initialisation status */
  SimChvStatus          chvStatus;
  /** Number of remaining retries and initialisation status */
  SimChvStatus          unblockChvStatus;
}
ApexSimChvFunctionCnf;

/*9907-4645*/
/** New signal to tel all FL layers any changes in CHV state. */
typedef struct ApexSimChvFunctionIndTag
{
  /** Always TASK_BL_ID */
  TaskId                taskId;
  /** CHV functions */
  SimChvFunction        chvFunction;
  /** Identifies which CHV  (CHV1/CHV2) */
  SimChvNum             chvNum;
 /** Whether or not requested CHV function was successful */
  Boolean               chvFunctionSuccess;
  /** Status information related to a CHV */
  SimChvStatus          chvStatus;
  /** Status information related to an unblock CHV */
  SimChvStatus          unblockChvStatus;
}
ApexSimChvFunctionInd;

/** This signal is sent to ABSI by FL handlers.
 * This signal is sent by the Controlling Task to
 * Background Layer task to perform a PIN function (Verify, Change, Unblock, Enable, Disable).
 * ABSI sends PIN info to ALSI for verification.
 */
typedef struct ApexSimPinFunctionReqTag
{
    /** ID of the requesting task */
    TaskId                          taskId;
    /** Number assigned by the Controlling Task */
    Int16                           commandRef;
    /** Verify, change, disable, enable, or unblock */
    SimUiccPinFunction              pinFunction;
    /** Pin key reference value */
    SimUiccKeyRefValue              pinKeyReference;
    /** The alternate PIN key reference. */
    SimUiccKeyRefValue              altPinKeyReference;
    /** PIN value */
    SimUiccPinValue                 pinValue;
    /** Directory to select */
    SimDirId                        localPinDirId;
    /** Used for CHANGE & UNBLOCK commands */
    SimUiccPinValue                 newPinValue;
}
ApexSimPinFunctionReq;

/** ABSI's response to FL's attempt to verify PIN info.
 * is sent by the Background Layer task to the Foreground Layer
 * task to indicate the success or otherwise of a requested PIN function.
 */
typedef struct ApexSimPinFunctionCnfTag
{
    /** Always TASK_BL_ID */
    TaskId                          taskId;
    /** Number copied from the ApexSimPinFunctionReq */
    Int16                           commandRef;
    /** The result of the ApexSimPinFunctionReq */
    SimRequestStatus                requestStatus;
    /** Verify, change, disable, enable or unblock. */
    SimUiccPinFunction              pinFunction;
    /** Pin key reference value */
    SimUiccKeyRefValue              pinKeyReference;
    /** The alternate PIN key reference. */
    SimUiccKeyRefValue              altPinKeyReference;
    /** Indicates if PIN function request was successful */
    Boolean                         pinFunctionSuccess;
    /** Number of remaining retries and initialisation status */
    SimUiccPinStatus                pinStatus;
    /** The status of alternate PIN */
    SimUiccPinStatus                altPinStatus;
    /** The status of unblock PIN */
    SimUiccPinStatus                unblockPinStatus;
    /** Whether or not universal Pin should be verified */
    Boolean                         verifyUniversalPin;
}
ApexSimPinFunctionCnf;

/** Signal to tell all FL layers any changes in PIN state */
typedef struct ApexSimPinFunctionIndTag
{
  /** Always TASK_BL_ID */
  TaskId                taskId;
  /** PIN functions */
  SimUiccPinFunction    pinFunction;
  /** Pin key reference value */
  SimUiccKeyRefValue    pinKeyReference;
  /** The alternate PIN key reference. */
  SimUiccKeyRefValue    altPinKeyReference;
  /** Indicates if PIN function request was successful */
  Boolean               pinFunctionSuccess;
  /** Number of remaining retries and initialisation status */
  SimUiccPinStatus      pinStatus;
  /** The status of unblock PIN */
  SimUiccPinStatus      unblockPinStatus;
  /** The status of alternate PIN */
  SimUiccPinStatus      altPinStatus;
}
ApexSimPinFunctionInd;


/** This signal is sent by BL (using ABSI function) to FL,
 * indicating a problem with the SIM.
 * This signal is generated by the Background Layer SIM
 * Support procedure when an AlsixxCnf requestStatus result
 * indicates a serious (but not necessarily fatal) problem with the SIM.
 */
typedef struct ApexSimFaultIndTag
{
  /** Always TASK_BL_ID */
  TaskId                taskId;
  /** The error returned in the AlsixxCnf signal */
  SimRequestStatus      simFault;
  /** The file on which the SIM error occurred */
  SimEfId               simFileId;
  /** Type of operation on which the SIM error occurred */
  OperationType         opType;
}
ApexSimFaultInd;

/* ME personalisation signals AF <-> AB */
/** This signal is provided by the Background Layer as a
 * response to an ApexSimMepStatusReq signal. This signal
 * contains the current status of each of the five possible
 * MEP locks described.
 */
typedef struct ApexSimMepStatusCnfTag
{
  /** Always TASK_BL_ID */
  TaskId                       taskId;
  /** Copied from the ApexSimMepStatusReq signal */
  Int16                        commandRef;
  /** The lock status for each MEP type */
  MepStateElement              lockStates[NUM_MEP_PERSONALISATIONS];
} ApexSimMepStatusCnf;

/** This signal is used to request the current status of all
 * the Mobile Equipment Personalisation (MEP) lock types from
 * the Background Layer.
 */
typedef struct ApexSimMepStatusReqTag
{
  /** Always TASK_BL_ID */
  TaskId                       taskId;
  /** The reference number that the Background Layer is to
   * return in the response signal (ApexSimMepStatusCnf)
   */
  Int16                        commandRef;
} ApexSimMepStatusReq;

/** This signal is used to request an MEP operation on an individual MEP lock type. */
typedef struct ApexSimMepReqTag
{
  /** Always TASK_BL_ID */
  TaskId                      taskId;
  /** The reference number that the Background Layer is to
   * return in the response signal (ApexSimMepCnf)
   */
  Int16                       commandRef;
  /** Selects the MEP lock type (SIM, Network etc.) for the specified operation. */
  AbsiMepSelector             selector;
  /** This is the password to use for the selected operation type (where required).*/
  Password                    key;
  /** This is the new password to use for change password.*/
  Password                    newkey;
  /** Defines the operation to be performed on the given lock type.*/
  AbsiMepOperation            operation;
} ApexSimMepReq;

/** This signal contains the data from the Background Layer in
 * response to a previous ApexSimMepReq.
 */
typedef struct ApexSimMepCnfTag
{
  /** Always TASK_BL_ID */
  TaskId                      taskId;
  /** Copies from ApexSimMepReq */
  Int16                       commandRef;
  /** Determines the outcome of the request.*/
  AbsiMepResult               result;
} ApexSimMepCnf;

/** GetMepData is used by the AB to get the ME data from the AF */
#if 0
typedef struct ApexGetMepDataReqTag
{
  /** Sender's task ID */
  TaskId                      taskId;
  /** Command reference */
  Int16                       commandRef;
}
ApexGetMepDataReq;

typedef struct ApexGetMepDataCnfTag
{
  /** Sender's task ID */
  TaskId                      taskId;
  /** Command reference */
  Int16                       commandRef;
  MePersonalisationData       mePersonalisationData;
}
ApexGetMepDataCnf;
#endif

typedef struct ApexSimOpenLogicalChannelReqTag
{
  /** Always TASK_BL_ID */
  TaskId                    taskId;
  /** Command reference */
  Int16                     commandRef;
  /** The length of DFname */
  Int16                     length;
  /** Holds the DFname data */
  Int8                      dfName[SIM_MAX_DF_NAME_SIZE];
} ApexSimOpenLogicalChannelReq;


typedef struct ApexSimOpenLogicalChannelCnfTag
  {
    /** Always TASK_BL_ID */
    TaskId                    taskId;
    /** Command reference */
    Int16                     commandRef;
    /** Determines the outcome of the request. */
    SimRequestStatus          requestStatus;
    /** session identifier */
    Int8                      sessionId;
  } ApexSimOpenLogicalChannelCnf;


typedef struct ApexSimCloseLogicalChannelReqTag
{
  /** Always TASK_BL_ID */
  TaskId                    taskId;
  /** Command reference */
  Int16                     commandRef;
  /** session to be closed. */
  Int8                      sessionId;
} ApexSimCloseLogicalChannelReq;


typedef struct ApexSimCloseLogicalChannelCnfTag
{
  /** Always TASK_BL_ID */
  TaskId                    taskId;
  /** Command reference */
  Int16                     commandRef;
  /** Determines the outcome of the request. */
  SimRequestStatus          requestStatus;
} ApexSimCloseLogicalChannelCnf;

typedef struct ApexSimLogicalChannelAccessReqTag
{
    /** Always TASK_BL_ID */
    TaskId                    taskId;
    /** Command Reference */
    Int16                     commandRef;
    /** session to be accessed. */
    Int8                      sessionId;
    /** The length of command */
    Int16                     length;
    /** Holds the command */
    Int8                      command[SIM_MAX_MSG_SIZE];
    /** A path (to select some directories and file) can be specified.
     * They will be selected before the raw command is passed to the SIM/UICC
     */
    SimUiccPath               path;
}ApexSimLogicalChannelAccessReq;

typedef struct ApexSimLogicalChannelAccessCnfTag
{ 
    /** Always TASK_BL_ID */
    TaskId                  taskId;
    /** Command Reference */
    Int16                   commandRef;
    /** Determines the outcome of the request. */
    SimRequestStatus        requestStatus;
    /** The length of response */
    Int16                   length;
    /** Holds the response */
    Int8                    response[SIM_MAX_MSG_SIZE];
}ApexSimLogicalChannelAccessCnf;



#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
/** This signal requests the status of the Call Forward Flags (CFF) contained in the SIM.*/
typedef struct ApexSimReadCffReqTag
{
  /** Always TASK_BL_ID */
  TaskId      taskId;
  /** The reference number that the Background Layer is to
   * return in the response signal (ApexSimReadCffCnf).
   */
  Int16     commandRef;
}ApexSimReadCffReq;

/** signal is sent in response to an ApexSimReadCffReq signal and
 * contains the state of the Call Forward Unconditional flags held
 * in the SIM.
 */
typedef struct ApexSimReadCffCnfTag
{
  /** Always TASK_BL_ID */
  TaskId      taskId;
  /** Copied from the ApexSimReadCffReq signal */
  Int16     commandRef;
  /** The status of the read, set to SIM_REQ_OK if the data is valid */
  SimRequestStatus                requestStatus;
  /** The CFF data. This field is valid only if the requestStatus
   * field contains SIM_REQ_OK. This will always contain data for
   * the Line1 and Line2; it also contains a flag which indicates
   * whether the data and fax CFF are in use.
   */
  SimCphsCallForwardFlags         cphsCff;
} ApexSimReadCffCnf;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

typedef enum SimParamTypeTag
{
    SIM_ECC,        /** Get the ECC code from (U)SIM */
    SIM_SST,        /** Get the SST from (U)SIM */
    SIM_GID         /** Get the GID1 and GID2 from (U)SIM */
}SimParamType;

/** This signal requests the parameters contained in the (U)SIM.*/
typedef struct ApexSimReadSimParamReqTag
{
  /** Always TASK_BL_ID */
  TaskId        taskId;
  /** The reference number that the Background Layer is to
   * return in the response signal (ApexSimReadSimParamCnf).
   */
  Int16         commandRef;
  SimParamType  simParamType;
}ApexSimReadSimParamReq;

typedef struct SimEccTag
{
    /** The total number of ECC in SIM */
    Int8                totalEccsOnSim;
    /** Lists emergency call codes */
    SimEccRecord        ecc[SIM_MAX_ECC_NUMS];
}SimEcc;

typedef struct SimSstTag
{
    /** Type of service table (2G or 3G)
\assoc UNION \ref serviceTable */
    ServiceTableType    serviceTableType;
    /** SIM Service Table or USIM Service Table */
    ServiceTable        serviceTable;
    /** Fixed dialling number capability enabled */
    Boolean             fdnEnabled;
    /** Barred dialling number capability enabled */
    Boolean             bdnEnabled;
    /** Access Point Names control list enabled*/
    Boolean             aclEnabled;
}SimSst;

typedef struct SimGidTag
{
    /** The length of GID1 */
    Int8                length1;
    /** The content of GID1 */
    Int8                gid1[SIM_MAX_GID_SIZE];
    /** The length of GID2 */
    Int8                length2;
    /** The content of GID2 */
    Int8                gid2[SIM_MAX_GID_SIZE];
}SimGid;

typedef union SimParamTag
{
    SimEcc          simEcc;
    SimSst          simSst;
    SimGid          simGid;
}SimParam;

typedef enum SimParamResultTag
{
    SIM_PARAM_OK,           /** All is OK for reading (U)SIM parameters */
    SIM_PARAM_INVALID_PARAM,/** The input parameter is invalid */
    SIM_PARAM_GID_NOT_AVAIL,/** The GID is not available */
    SIM_PARAM_ECC_INVALID   /** No valid ECC code */
}SimParamResult;

/** signal is sent in response to an ApexSimReadSimParamReq signal and
 * contains the parameters held in the SIM.
 */
typedef struct ApexSimReadSimParamCnfTag
{
  /** Always TASK_BL_ID */
  TaskId            taskId;
  /** Copied from the ApexSimReadSimParamReq signal */
  Int16             commandRef;
  /** The result of reading parameters of (U)SIM
\assoc UNION \ref simParam */
  SimParamResult    simParamResult;
 /** The SIM parameters data. */
  SimParam                        simParam;
} ApexSimReadSimParamCnf;

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
/** This signal requests the writing of the Call Forward Unconditional
 * Flags held in the SIM.
 */
typedef struct ApexSimWriteCffReqTag
{
  /** Sender's task ID */
  TaskId                          taskId;
  /** The reference number that the Background Layer is to return in
   * the response signal
   */
  Int16                           commandRef;
  /** The CFF data. This should always contain data for the
   * Line1 and Line2, it also contains a flag which indicates
   * whether the data and fax CFF are in use.
   */
  SimCphsCallForwardFlags         cphsCff;
}ApexSimWriteCffReq;

/** This signal is sent in response to a ApexSimWriteCffReq
 * signal and contains a flag to indicate if the write was successful.
 */
typedef struct ApexSimWriteCffCnfTag
{
  /** Always TASK_BL_ID */
  TaskId                          taskId;
  /** Copied from the ApexSimWriteCffReq signal */
  Int16                           commandRef;
  /** Indicates the success or otherwise of the writing operation.*/
  Boolean                         cffDataStoredOk;
  /** Repeat the request values*/
  SimCphsCallForwardFlags         cphsCff;
}ApexSimWriteCffCnf;

/** This signal requests the status of the Voice
 * Message Waiting Flags (VMWF) contained in the SIM.
 */
typedef struct ApexSimReadVmwfReqTag
{
  /** Always TASK_BL_ID */
  TaskId      taskId;
  /** The reference number that the Background Layer is
   * to return in the response signal (ApexSimReadVmwfCnf)
   */
  Int16     commandRef;
}ApexSimReadVmwfReq;

/** This signal is sent in response to a ApexSimReadVmwfReq signal
 * and contains the state of the voice message waiting flags held in the SIM.
 */
typedef struct ApexSimReadVmwfCnfTag
{
  /** Always TASK_BL_ID */
  TaskId      taskId;
  /** Copied from the ApexSimReadVmwfReq signal */
  Int16     commandRef;
  /** The status of the read, set to SIM_REQ_OK if the data is valid.*/
  SimRequestStatus                requestStatus;
  /** The voice message waiting flag data. */
  SimCphsVoiceMsgWaitingFlags     cphsVmwf;
} ApexSimReadVmwfCnf;

/** The ApexSimWriteVmwfReq signal requests the writing of the
 * voice message waiting flags held in the SIM.
 */
typedef struct ApexSimWriteVmwfReqTag
{
  /** Sender's task ID */
  TaskId                          taskId;
  /** The reference number that the Background Layer is to return
   * in the response signal (ApexSimWriteVmwfCnf)
   */
  Int16                           commandRef;
  /** The voice message waiting flag data */
  SimCphsVoiceMsgWaitingFlags     cphsVmwf;
}ApexSimWriteVmwfReq;

/** This signal is sent in response
 * to a ApexSimWriteVmwfReq signal and contains a flag to
 * indicate if the write was successful.
 */
typedef struct ApexSimWriteVmwfCnfTag
{
  /** Always TASK_BL_ID */
  TaskId                          taskId;
  /** Copied from the ApexSimWriteVmwfReq signal */
  Int16                           commandRef;
  /** Indicates the success or otherwise of the writing operation */
  Boolean                         vmwfDataStoredOk;
}ApexSimWriteVmwfCnf;

/** This signal requests the status of the CPHS information stored on the SIM. */
typedef struct ApexSimReadCspReqTag
{
  /** TaskId of sending task */
  TaskId      taskId;
  /** The reference number that the Background Layer is to
   * return in the response (ApexSimReadCphsInfoCnf)
   */
  Int16     commandRef;
}ApexSimReadCspReq;

/** This signal is sent as a response to ApexSimReadCspReq.
 * This signal indicates whether or not the corresponding request
 * succeeded and if so it contains the required data.
 * If successful it returns all of the CSP data held in the Customer Service Profile file.
 */
typedef struct ApexSimReadCspCnfTag
{
  /** Always TASK_BL_ID */
  TaskId                          taskId;
  /** A copy of the command reference given in the corresponding request. */
  Int16                           commandRef;
  /** An indication of whether or not the request was successful
   * (The value of SIM_REQ_OK indicates success)
   */
  SimRequestStatus                requestStatus;
  /** Whether Customer Service Profile is present */
  Boolean                         cphsCspPresent;
  /** Customer service profile data */
  SimCphsCustServProfile          cphsCsp;
} ApexSimReadCspCnf;

/** Request to write CSP entry */
typedef struct ApexSimWriteCspReqTag
{
  /** Sender's task ID */
  TaskId                          taskId;
  /** Command Reference */
  Int16                           commandRef;
  /** Indicates the number of entries to write */
  Int8                            entryNumber;
  /** CSP entry to be written */
  SimCphsCspEntry                 cspEntry;
}ApexSimWriteCspReq;

/** Confirmation (or not) that the CSP entry has been written. */
typedef struct ApexSimWriteCspCnfTag
{
  /** Always TASK_BL_ID */
  TaskId                          taskId;
  /** Copied from ApexSimWriteCspReq */
  Int16                           commandRef;
  /** Determines the outcome of the request.*/
  SimRequestStatus          requestStatus;
}ApexSimWriteCspCnf;

/** The ApexSimReadCphsInfoReq signal requests the status of
 * the CPHS information stored on the SIM.
 */
typedef struct ApexSimReadCphsInfoReqTag
{
  /** TaskId of sending task */
  TaskId                          taskId;
  /** The reference number that the Background Layer is to return in
   * the response (ApexSimReadCphsInfoCnf)
   */
  Int16                           commandRef;
}ApexSimReadCphsInfoReq;

/** The ApexSimReadCphsInfoCnf signal is sent in response
 * to a ApexSimReadCphsInfoReq signal and contains a flag
 * to indicate if the read was successful.
 */
typedef struct ApexSimReadCphsInfoCnfTag
{
  /** Always TASK_BL_ID */
  TaskId                          taskId;
  /** Copied from the ApexSimReadCphsInfoReq signal */
  Int16                           commandRef;
  /** Indicates the presence or otherwise of the CPHS data */
  Boolean                         cphsDataPresent;
  /** Contains the phase of the CPHS data. */
  SimCphsPhase                    cphsPhase;
  /** The Service Table indicates which parts of the CPHS
   * functionality are presently available on the SIM.
   */
  SimCphsServiceTable             cphsService;
  /** Whether the Operator name is present */
  Boolean                         cphsOpNamePresent;            /*16037 start*/
  /** Operator name itself */
  SimCphsOperatorName             cphsOpName;
  /** Whether the short form of the op name is present */
  Boolean                         cphsOpNameShortFormPresent;
  /** Short form of op name itself */
  SimCphsOperatorNameShort        cphsOpNameShortForm;
  /** Whether mailbox numbers are present */
  Boolean                         cphsMnPresent;
  /** Mailbox numbers */
  SimCphsMailboxNumbers           cphsMn;
  /** Whether Voice message waiting flags are present */
  Boolean                         cphsVmwFlagsPresent;
  /** Voice mail waiting flags */
  SimCphsVoiceMsgWaitingFlags     cphsVmwFlags;
  /** Whether call forward flags are present */
  Boolean                         cphsCffPresent;
  /** Call forward flags */
  SimCphsCallForwardFlags         cphsCff;
  /** Whether Customer Service Profile is present */
  Boolean                         cphsCspPresent;
  /** Customer service profile */
  SimCphsCustServProfile          cphsCsp;

}ApexSimReadCphsInfoCnf;

/** The ApexSimCphsReadyInd signal is sent in response to
 * an AlsiCphsDataInd signal and sent to all foreground
 * layers registered to procedure PROCEDURE_BL_SIM_ID.
 * It is intended to show that the CPHS data is ready to be read.
 */
typedef struct ApexSimCphsReadyIndTag
{
  /** Always TASK_BL_ID */
  TaskId                          taskId;
  /** PLMN mode bit read from the USIM.
   * This field will only be set to a defined value (0 or 1)
   * if the USIM is a ENS USIM.
   * Set to PLMN_MODE_BIT_UNDEFINED otherwise.
   */
  SimPlmnModeBit                    plmnModeBit;

  CphsInitData cphsInitData;

} ApexSimCphsReadyInd;

/** Returns information about the Information Number file.
 * This signal returns information about the Information Number file as follows: <UL>
 * <LI> the size of each record
 * <LI> the number of records
 * <LI> the file ID (can be 0x6F19 or 0xEA01)
 * <LI> (optionally) the maximum alpha ID length
 * <LI> (optionally) the maximum dial string length.</UL>
 *
 * The last two items are optional in the confirmation signal
 * because if the file does not contain any valid records these
 * sizes cannot be worked out.
 */
typedef struct ApexSimInfoNumStatusReqTag
{
  /** Always TASK_BL_ID.*/
  TaskId                          taskId;
  /** A command reference which is mirrored in the confirmation signal.*/
  Int16                           commandRef;
} ApexSimInfoNumStatusReq;

/** This signal is sent as a response to a ApexSimInfoNumStatusReq.
 * This signal indicates whether or not the corresponding
 * request succeeded and if so it contains the required data.
 * If successful it returns some general information about the
 * Information Numbers file.
 */
typedef struct ApexSimInfoNumStatusCnfTag
{
  /** Always TASK_BL_ID.*/
  TaskId                          taskId;
  /** A copy of the command reference given in the corresponding request.*/
  Int16                           commandRef;
  /** An indication of whether or not the request was successful
   * (The value of SIM_REQ_OK indicates success, all other values
   * indicate failure).
   */
  SimRequestStatus                requestStatus;
  /** The size of each record in the Information Numbers file.*/
  Int8                            recordSize;
  /** The total number of records in the file. */
  Int8                            numRecords;
  /** The file ID, can be either SIM_EF_CPHS_INFO_NUM or
   * SIM_EF_CPHS_INFO_NUM_OLD.
   */
  SimEfId                         efId;
  /** A flag to indicate whether or not the next two fields are valid.*/
  Boolean                         lengthsPresent;
  /** The maximum length an alpha ID can be for this file. */
  Int8                            maxAlphaIdLength;
  /** The maximum length a dial string can be for this file without
   * overflowing into an extension record.
   */
  Int8                            maxDialStringLength;
} ApexSimInfoNumStatusCnf;

/** This signal requests the SIM Manager
 * to read and decode the Information Numbers.
 * The ApexSimListInfoNumReq signal causes the SIM Manager
 * to read and decode the next 10 valid Information Numbers
 * (empty records are skipped over), starting from the given
 * start record. The data is returned in the ApexSimListInfoNumCnf signal.
 */
typedef struct ApexSimListInfoNumReqTag
{
  /** Always TASK_BL_ID. */
  TaskId                          taskId;
  /** A command reference which is mirrored in the confirmation signal.*/
  Int16                           commandRef;
  /** The record number from which to start looking for valid records
   *(Record numbering starts at 1 for the first record).
   */
  Int8                            startRecord;
} ApexSimListInfoNumReq;

/** Indicates the outcome of the corresponding request.
 * The ApexSimListInfoNumCnf signal indicates whether
 * or not the corresponding request succeeded and if so
 * it contains the required data. If successful it returns a
 * list of up to the next 10 valid Information Numbers.
 *
 * If this value is not equal to SIM_CPHS_MAX_INFONUM_LIST_SIZE then
 * the calling entity can assume that there are no more valid entries
 * in the Information Numbers file.
 */
typedef struct ApexSimListInfoNumCnfTag
{
  /** Always TASK_BL_ID.*/
  TaskId                          taskId;
  /** A copy of the command reference given in the corresponding request.*/
  Int16                           commandRef;
  /** An indication of whether or not the request was successful
   * (The value of SIM_REQ_OK indicates success, all other values
   * indicate failure)
   */
  SimRequestStatus                requestStatus;
  /** The number of valid entries in the list array. */
  Int8                            listSize;
  /** The record numbers of the Information Number entries stored
   * in the list array.
   */
  Int8                            recordNumbers[SIM_CPHS_MAX_INFONUM_LIST_SIZE];
  /** An array of up to the next 10 valid Information Number entries */
  SimCphsInfoNumEntry             list[SIM_CPHS_MAX_INFONUM_LIST_SIZE];
} ApexSimListInfoNumCnf;

/** This signal reads a single Information Number record. */
typedef struct ApexSimReadInfoNumReqTag
{
  /** Always TASK_BL_ID. */
  TaskId                          taskId;
  /** A command reference that is mirrored in the confirmation signal.*/
  Int16                           commandRef;
  /** This request does not support seeking (performSeek equal to FALSE).*/
  SimRecordDefinition             recordDefinition;
} ApexSimReadInfoNumReq;

/** Determines the outcome of the request.
 * This signal indicates whether or not the corresponding
 * request succeeded and if so it contains the required data.
 * If successful it returns the Information Number data from
 * the requested record.
 */
typedef struct ApexSimReadInfoNumCnfTag
{
  /** Always TASK_BL_ID. */
  TaskId                          taskId;
  /** A copy of the command reference given in the corresponding request.*/
  Int16                           commandRef;
  /** An indication of whether or not the request was successful
   * (The value of SIM_REQ_OK indicates success, all other values
   * indicate failure)
   */
  SimRequestStatus                requestStatus;

  /** The record number of the read Information Number entry.*/
  Int8                            recordNumber;
  /** This contains the read Information Number data if the request
   * was successful.
   */
  SimCphsInfoNumEntry             infoNum;
} ApexSimReadInfoNumCnf;

/** This signal is used to write either a number or index
 * entry into the Information Number file.
 */
typedef struct ApexSimWriteInfoNumReqTag
{
  /** Always TASK_BL_ID. */
  TaskId                          taskId;
  /** A command reference that is mirrored in the confirmation signal.*/
  Int16                           commandRef;
  /** This request does not support seeking (performSeek equal to FALSE).*/
  SimRecordDefinition             recordDefinition;
  /** This field indicates whether or not the contents of the
   * infoNum field are valid and if this is set to FALSE this is
   * an indication that the indicated record should be marked as unused.
   */
  Boolean                         dataValid;
  /** The Information Number entry to write */
  SimCphsInfoNumEntry             infoNum;
} ApexSimWriteInfoNumReq;

/** Determines the outcome of the write request.
 * This signal is used to confirm a write request
 * to the Information Number file.
 */
typedef struct ApexSimWriteInfoNumCnfTag
{
  /** Always TASK_BL_ID.*/
  TaskId                          taskId;
  /** A copy of the command reference given in the corresponding request.*/
  Int16                           commandRef;
  /** An indication of whether or not the request was successful
   * (The value of SIM_REQ_OK indicates success, all other values
   * indicate failure)
   */
  SimRequestStatus                requestStatus;
  /** If the requestStatus field indicates
   * failure this field indicates if it was
   * because of lack of free extension records
   * in the EXT1 file.
   */
  Boolean                         noFreeExtRecords;
  /** The record number of the written Information Number entry. */
  Int8                            recordNumber;
  /** This contains the Information Number data that was requested to be written. */
  SimCphsInfoNumEntry             infoNum;
} ApexSimWriteInfoNumCnf;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/** Requests a read of the Service Provider Name contained
 * in the SIM.
 */
typedef struct ApexSimReadSpnReqTag
{
  /** Sender's task ID */
  TaskId                          taskId;
  /** The reference number that the Background Layer is to
   * return in the response signal (ApexSimReadSpnCnf).
   */
  Int16                           commandRef;
}ApexSimReadSpnReq;

/** Holds the Service Provider Name data from the SIM.
 * This signal is sent in response to a ApexSimReadSpnReq signal
 * and contains the Service Provider Name data from the SIM.
 * The data contains a flag indicating whether or not to display the
 * PLMN and Service Provider Name (SPN) length and contents.
 */
typedef struct ApexSimReadSpnCnfTag
{
  /** Always TASK_BL_ID */
  TaskId                          taskId;
  /** Copied from the ApexSimReadSpnReq signal */
  Int16                           commandRef;
   /** An indication of whether or not the request was successful
   * (The value of SIM_REQ_OK indicates success, all other values
   * indicate failure)
   */
  SimRequestStatus                requestStatus;
  /** Contains a flag indicating whether or not to display
   * the PLMN and the SPN itself.
   */
  SimSpnData                      spnData;
}ApexSimReadSpnCnf;

/** Requests to read the Service Provider Display Information */
typedef struct ApexSimReadSpdiReqTag
{
    /** Sender's task ID */
    TaskId                          taskId;
    /** Command Reference */
    Int16                           commandRef;
    /** This indicates the first desired entry (only valid entries count)*/
    Int16                           startField;
}
ApexSimReadSpdiReq;

/** Determines the outcome of the read request.
 * Returns a  list of valid PLMNs from the SPDI file
 * starting from the startField record.
 */
typedef struct ApexSimReadSpdiCnfTag
{
    /** Sender's task ID */
    TaskId                          taskId;
    /** Copied from ApexSimReadSpdiReq */
    Int16                           commandRef;
    /** An indication of whether or not the request was successful
     * (The value of SIM_REQ_OK indicates success, all other values
     * indicate failure)
     */
    SimRequestStatus                requestStatus;
    /** The number of valid PLMN entries*/
    Int16                           numValidEntries;
    /** First valid entry we requested*/
    Int16                           startField;
    /** The list of valid PLMNs in SPDI */
    SimSpdiList                     spdiList;
}
ApexSimReadSpdiCnf;

/** Request to read CFIS file.
 * A signal to request reading of the specified record from the EF CFIS
 * (Call Forwarding Indication Status) file.
 */
typedef struct ApexSimReadCfisReqTag
{
    /** Sender's task ID */
    TaskId                          taskId;
    /** Command Reference */
    Int16                           commandRef;
    /** Subscriber profile */
    Int8                            subscriberProfile;
}
ApexSimReadCfisReq;

/** Returns the result of the request to read the specified record from the
 * EF CFIS (Call Forwarding Indication Status) file.
 */
typedef struct ApexSimReadCfisCnfTag
{
    /** Sender's task ID */
    TaskId                          taskId;
    /** Command reference copied from ApexSimReadCfisReq */
    Int16                           commandRef;
    /** An indication of whether or not the request was successful
     * (The value of SIM_REQ_OK indicates success, all other values
     * indicate failure)
     */
    SimRequestStatus                requestStatus;
     /** Subscriber profile */
    Int8                            subscriberProfile;
    /** CFIS data that has been read*/
    SimCfisData                     cfisData;
}
ApexSimReadCfisCnf;

/** Request to write CFIS file.
 * A signal to request writing of information to the specified
 * record from the EF CFIS (Call Forwarding Indication Status) file.
 */
typedef struct ApexSimWriteCfisReqTag
{
    /** Sender's task ID */
    TaskId                          taskId;
    /** Command Reference */
    Int16                           commandRef;
    /** Subscriber profile */
    Int8                            subscriberProfile;
    /** CFIS file to write. */
    SimCfisData                     cfisData;

}
ApexSimWriteCfisReq;

/** Returns the result of the request to write the specified data to
 * the defined record on the EF CFIS (Call Forwarding Indication Status) file.
 */
typedef struct ApexSimWriteCfisCnfTag
{
    /** Sender's task ID */
    TaskId                          taskId;
    /** Command Reference */
    Int16                           commandRef;
    /** An indication of whether or not the request was successful
     * (The value of SIM_REQ_OK indicates success, all other values
     * indicate failure)
     */
    SimRequestStatus                requestStatus;
    /** Subscriber profile */
    Int8                            subscriberProfile;
}
ApexSimWriteCfisCnf;

/** Request to delete CFIS file.
 * A signal to request deleting of the specified
 * record from the EF CFIS (Call Forwarding Indication Status) file.
 */
typedef struct ApexSimDeleteCfisReqTag
{
    /** Sender's task ID */
    TaskId                          taskId;
    /** Command Reference */
    Int16                           commandRef;
    /** Subscriber profile */
    Int8                            subscriberProfile;
}
ApexSimDeleteCfisReq;

/** Returns the result of the request to delete the specified record
 * from the EF CFIS (Call Forwarding Indication Status) file.
 */
typedef struct ApexSimDeleteCfisCnfTag
{
    /** Sender's task ID */
    TaskId                          taskId;
    /** Copied form ApexSimDeleteCfisReq */
    Int16                           commandRef;
     /** An indication of whether or not the request was successful
   * (The value of SIM_REQ_OK indicates success, all other values
   * indicate failure)
   */
    SimRequestStatus                requestStatus;
    /** Subscriber profile */
    Int8                            subscriberProfile;
}
ApexSimDeleteCfisCnf;


/** Request a list of valid OPL Records in EF OPL from the offset specified in startField.*/
typedef struct ApexSimListOplReqTag
{
  /** Sender's task ID */
  TaskId                          taskId;
  /** Command Reference */
  Int16                           commandRef;
  /** Specifies the first entry desired */
  Int8                            startField;
}
ApexSimListOplReq;

/** Confirmation of the AlsiListOplReq. This primitive is sent as a response to
 * AlsiListOplReq. If the request was successful,
 * it returns the list up to a maximum of SIM_OPL_LIST_SIZE of valid OPL records .
 */
typedef struct ApexSimReadOplCnfTag
{
  /** Sender's task ID */
  TaskId                          taskId;
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** Next field to fetch */
  Int8                            nextStartField;
  /** Specifies how many entries there are in total */
  Int8                            totalEntries;
  /** Requested OPL Records */
  SimOplData                      oplData;
}
ApexSimListOplCnf;

/** Request a list of valid PNN Records in EF PNN from the offset specified in startField. */
typedef struct ApexSimListPnnReqTag
{
  /** Sender's task ID */
  TaskId                          taskId;
  /** Command Reference */
  Int16                           commandRef;
  /** Offset to start from */
  Int8                            startField;
}
ApexSimListPnnReq;

/** Confirmation of the AlsiListPnnReq; if the request was successful,
 * it returns the list up to a maximum of SIM_PNN_LIST_SIZE of valid PNN records .
 */
typedef struct ApexSimListPnnCnfTag
{
  /** Sender's task ID */
  TaskId                          taskId;
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** Specifies how many entries there are in total */
  Int8                            totalEntries;
  /** Next field to fetch */
  Int8                            nextStartField;
  /** Requested PNN records */
  SimPnnData                      pnnData;
}
ApexSimListPnnCnf;


/** This signal requests a reading of the defined subscriber profile (record)
 * from the Message Waiting Information File(EF_MWIS 6FCA)
 */
typedef struct ApexSimReadMwisReqTag
{
    /** Sender's task ID */
    TaskId                        taskId;
    /** Assume at present that there is only one subscriber profile
     * so we are accessing record number 1
     */
    Int16                         commandRef;
    /** Subscriber profile */
    Int8                          subscriberProfile;
}ApexSimReadMwisReq;

/** Determines the outcome of the ApexSimReadMwisReq request.
 * This signal reports on the success/failure of reading a subscriber
 * profile from the Message Waiting Information File and if successful,
 * passes the decoded information read from the specified profile (record).
 */
typedef struct ApexSimReadMwisCnfTag
{
    /** Sender's task ID */
    TaskId                       taskId;
    /** Command reference copied form ApexSimReadMwisReq */
    Int16                        commandRef;
    /** Subscriber profile */
    Int8                         subscriberProfile;
    /** An indication of whether or not the request was successful
     * (The value of SIM_REQ_OK indicates success, all other values
     * indicate failure)
     */
    SimRequestStatus             requestStatus;
#if defined (UPGRADE_REL6)
    /** indicates if valid videomail info  - optional field - is present on the USIM*/
    Boolean                      videoMailMsgWSupported;
#endif
    /** The Message Waiting Information File */
    SimMBMsgWaitingInfo          msgWaitingInfo[SIM_NUM_MAILBOX_INFO_GROUPS];
}ApexSimReadMwisCnf;

/** This signal requests to read the global MWI status
 */
typedef struct ApexSimReadSystemMwiReqTag
{
    /** Sender's task ID */
    TaskId                        taskId;
    /** Command reference */
    Int16                         commandRef;
}ApexSimReadSystemMwiReq;

/** Determines the outcome of the ApexSimReadMwisReq request.
 * This signal reports on the success/failure of reading the global MWI status
 */
typedef struct ApexSimReadSystemMwiCnfTag
{
    /** Sender's task ID */
    TaskId                       taskId;
    /** Command reference copied form ApexSimReadMwisReq */
    Int16                        commandRef;
    /** An indication of whether or not the request was successful */
    SimRequestStatus             requestStatus;
    /** The Message Waiting Information content */
    SimMBMsgWaitingInfo          msgWaitingInfo[SIM_NUM_MAILBOX_INFO_GROUPS];
    /** Additional CPHS Auxiliary Voice message waiting indication*/
    Boolean                      msgWaitingAux;
    /** Additional CPHS Data message waiting indication*/
    Boolean                      msgWaitingData;
}ApexSimReadSystemMwiCnf;

/** Requests the update of a defined subscriber
 * profile (record) from the Message Waiting Information File.
 */
typedef struct ApexSimWriteMwisReqTag
{
    /** Sender's task ID */
    TaskId                       taskId;
    /** Command reference */
    Int16                        commandRef;
    /** Subscriber profile */
    Int8                         subscriberProfile;
    /** Message Waiting Information */
    SimMBMsgWaitingInfo          msgWaitingInfo[SIM_NUM_MAILBOX_INFO_GROUPS];
}ApexSimWriteMwisReq;


/** Determines the outcome of the ApexSimWriteMwisReq request.
 * This signal reports on the success/failure of writing the
 * Message Waiting Information for the defined subscriber profile
 * (currently only one subscriber profile is supported).
 */
typedef struct ApexSimWriteMwisCnfTag
{
    /** Always TASK_BL_ID */
    TaskId                       taskId;
    /** Copied from ApexSimWriteMwisReq */
    Int16                        commandRef;
    /** Subscriber profile */
    Int8                         subscriberProfile;
    /** An indication of whether or not the request was successful
     * (The value of SIM_REQ_OK indicates success, all other values
     * indicate failure)
     */
#if defined (UPGRADE_REL6)
   /** Whether the optional field for videoMail was available and written to */
   Boolean                       videoMailMsgWaitingWritten;
#endif
    SimRequestStatus             requestStatus;
}ApexSimWriteMwisCnf;


/** This signal requests a reading of the defined subscriber profile (record)
 * from the Mailbox Information File(EF_MBI 6FC9)
 */
typedef struct ApexSimReadMbiReqTag
{
    /** Sender's task ID */
    TaskId                        taskId;
    Int16                         commandRef;
    /** Subscriber profile */
    Int8                          subscriberProfile;
}ApexSimReadMbiReq;

/** Determines the outcome of the ApexSimReadMbiReq request.
 * This signal reports on the success/failure of reading a subscriber
 * profile from the Mailbox Information File and if successful,
 * passes the decoded information read from the specified profile (record).
 */
typedef struct ApexSimReadMbiCnfTag
{
    /** Sender's task ID */
    TaskId                       taskId;
    /** Command reference copied form ApexSimReadMbiReq */
    Int16                        commandRef;
    /** Subscriber profile */
    Int8                         subscriberProfile;
    /** An indication of whether or not the request was successful
       * (The value of SIM_REQ_OK indicates success, all other values
       * indicate failure)
       */
    SimRequestStatus             requestStatus;
    /*whether the optional videomail information is present in the mailbox info */
#if defined (UPGRADE_REL6)
    Boolean                      videoMailInfoSupported;
#endif
    /** The Mailbox Information File */
    SimMailboxInfo               mailboxInfo[SIM_NUM_MAILBOX_INFO_GROUPS];
}ApexSimReadMbiCnf;

/** Requests the update of a defined subscriber
 * profile (record) from the Mailbox Information File.
 */
typedef struct ApexSimWriteMbiReqTag
{
    /** Sender's task ID */
    TaskId                       taskId;
    /** Command reference */
    Int16                        commandRef;
    /** Subscriber profile */
    Int8                         subscriberProfile;
    /** Mailbox Information */
    SimMailboxInfo               mailboxInfo[SIM_NUM_MAILBOX_INFO_GROUPS];
}ApexSimWriteMbiReq;


/** Determines the outcome of the ApexSimWriteMbiReq request.
 * This signal reports on the success/failure of writing the
 * Mailbox Information for the defined subscriber profile
 * (currently only one subscriber profile is supported).
 */
typedef struct ApexSimWriteMbiCnfTag
{
    /** Always TASK_BL_ID */
    TaskId                       taskId;
    /** Copied from ApexSimWriteMwisReq */
    Int16                        commandRef;
    /** Subscriber profile */
    Int8                         subscriberProfile;
#if defined (UPGRADE_REL6)
    /** Whether the optional field for videoMail was available and written to */
    Boolean                      videoMailInfoWritten;
#endif

    /** An indication of whether or not the request was successful
      * (The value of SIM_REQ_OK indicates success, all other values
      * indicate failure)
      */
    SimRequestStatus             requestStatus;
}ApexSimWriteMbiCnf;

/** This signal is a request to read the accumulated call timer
 */
typedef struct ApexSimReadAccCallTimerReqTag
{
  /** The sending task ID */
  TaskId            taskId;
  /** Command reference */
  Int16             commandRef;
  /** Whether incoming or outgoing calls */
  Boolean           isOutgoing;
}
ApexSimReadAccCallTimerReq;

/** This signal is used to report the contents of the accumulated call
 * timer file either for incoming or outgoing calls
 */
typedef struct ApexSimReadAccCallTimerCnfTag
{
  /** Sending task ID (TASK_BL_ID )*/
  TaskId            taskId;
  /** Command reference */
  Int16             commandRef;
  /** Indicates the outcome of the request */
  SimRequestStatus  requestStatus;
  /** Whether incoming or outgoing calls */
  Boolean           isOutgoing;
  /** Accumulated call timer */
  Int32             accCallDuration;
}
ApexSimReadAccCallTimerCnf;

/** This signal is a request to reset the accumulated call timer
 */
typedef struct ApexSimResetAccCallTimerReqTag
{
  /** The sending task ID */
  TaskId            taskId;
  /** Command reference */
  Int16             commandRef;
  /** Whether incoming or outgoing calls */
  Boolean           isOutgoing;
}
ApexSimResetAccCallTimerReq;

/** This signal is used to reset the value of the accumulated call
 * timer either for incoming or outgoing calls
 */
typedef struct ApexSimResetAccCallTimerCnfTag
{
  /** Sending task ID (TASK_BL_ID )*/
  TaskId            taskId;
  /** Command reference */
  Int16             commandRef;
  /** Indicates the outcome of the request */
  SimRequestStatus  requestStatus;
  /** Whether incoming or outgoing calls */
  Boolean           isOutgoing;
}
ApexSimResetAccCallTimerCnf;


/** This signal is used to report a modification in the contents of the
 * accumulated call timer file either for incoming or outgoing calls
 */
typedef struct ApexSimAccCallTimerChangedIndTag
{
  /** Sending task ID (TASK_BL_ID )*/
  TaskId            taskId;
  /** Whether incoming or outgoing calls */
  Boolean           isOutgoing;
}
ApexSimAccCallTimerChangedInd;




/** This signal is sent by the Controlling Task to Background Layer task
 * to perform a SIM command (SELECT, READ BINARY, READ RECORD,etc )
 */
typedef struct ApexSimGenAccessReqTag
{
  /** ID of the requesting task */
  TaskId                          taskId;
  /** Number assigned by the Controlling Task */
  Int16                           commandRef;
  /** Length of the raw command */
  Int16                           length;
  /** Raw command */
  Int8                            command[SIM_MAX_MSG_SIZE];
  SimUiccPath                     path;
} ApexSimGenAccessReq;

/** This  signal is sent by the Background Layer task
 * to the Foreground Layer task to indicate the result
 * of the command.
 */
typedef struct ApexSimGenAccessCnfTag
{
  /** Always TASK_BL_ID */
  Int16                           commandRef;
  /** Length of the raw response from the SIM.*/
  Int16                           length;
  /** Raw response from the SIM */
  Int8                            command[SIM_MAX_MSG_SIZE];
  /*job 101560: new fields to convey the intermediate response from the SIM */
  /** Indicate whether or not the file specified in the request
   * was successfully selected and if the response data in the
   * command field is valid.
   */
  Boolean                         success;  /* 0205-17637 */
} ApexSimGenAccessCnf;

typedef ApexSimGenAccessReq ApexSimRawApduAccessReq;

typedef ApexSimGenAccessCnf ApexSimRawApduAccessCnf;

/** Indicates that the SIM application has been stopped */
typedef struct ApexSimAppStoppedIndTag
{
    /** Record number of EF_DIR (indicates the stopped USIM application)*/
    Int8                            recordNum;
    /** Info about the stopped app*/
    SimUiccAidLabel                 applicationInfo;

} ApexSimAppStoppedInd;

/** Indicates that a SIM has been inserted or that a SIM is present at power on.  */
typedef struct ApexSimAppStartedIndTag
{
    /* 0209-20294:*/
    /** Card inserted status */
    SimInsertStatus                 status;
    /** TRUE if 3G card*/
    Boolean                         cardIsUicc;
    SimPhase                        phase;
    /** extended language preference list */
    SimExtLangPrefList              extLangPrefList;
    /** prefered language list */
    LanguagePrefList                languagePrefList;
    /** Indicates if the UICC support universal PIN
     * (Fields below are only valid if the card is a UICC
     * but not valid for GSM SIM cards)
     */
    Boolean                         universalPinSupportedByCard;
    /** Application PIN (could also be universal PIN)*/
    SimUiccKeyRefValue              pin1KeyRef;
    /** PIN2 also called local PIN*/
    SimUiccKeyRefValue              pin2KeyRef;
    /** Whether the universal PIN should be verified or not
     * (This depends upon whether the application PIN is also enabled,
     * and also depends whether the universal PIN has already been verified
     * in a previous session)
     */
    Boolean                         verifyUniversalPin;
    /** Record number of EF_DIR (indicates the started USIM application)*/
    Int8                            aidRecordNum;
    /** Info about the started app*/
    SimUiccAidLabel                 applicationLab;
} ApexSimAppStartedInd;


/** This signal tells the SIM manager which USIM application should be activated*/
typedef struct ApexSimUsimAppStartReqTag
{
    /** Sender's task ID */
    TaskId      taskId;
    /** Command reference */
    Int16       commandRef;
    /** Record number of EF_DIR (indicates the selected USIM application)*/
    Int8        recordNum;
} ApexSimUsimAppStartReq;

/** Confirmation to the ApexSimUsimAppStartReq.*/
typedef struct ApexSimUsimAppStartCnfTag
{
    /** Sender's task ID */
    TaskId                          taskId;
    /** Copied from ApexSimUsimAppStartReq */
    Int16                           commandRef;
    /** An indication of whether or not the request was successful
     * (The value of SIM_REQ_OK indicates success, all other values
     * indicate failure)
     */
    SimRequestStatus                requestStatus;
    /** The DIR record number of the started aplication */
    Int8                            recordNum;
}
ApexSimUsimAppStartCnf;

/** Request to read EF DIR file.
 * This signal is sent to read the content of EF DIR on the SIM
 * EF_DIR lists the application present on the SIM card
 */
typedef struct ApexSimReadDirReqTag
{
    /** Sender's task ID */
    TaskId                          taskId;
    /** Command reference */
    Int16                           commandRef;
    /** Defines the record number in EF DIR */
    Int8                            recordNum;
    /** Number of entries to read, use 0 to read the maximum*/
    Int8                            numEntriesDesired;
} ApexSimReadDirReq;

/** Confirmation to the ApexSimReadDirReq.
 * This signal lists all the applications present on the SIM card*/
typedef struct ApexSimReadDirCnfTag
{
    /** Sender's task ID */
    TaskId                          taskId;
    /** Copied from ApexSimReadDirReq */
    Int16                           commandRef;
    /** An indication of whether or not the request was successful
     * (The value of SIM_REQ_OK indicates success, all other values
     * indicate failure)
     */
    SimRequestStatus                requestStatus;
    /** TotalEntries in EF DIR*/
    Int8                            totalEntries;
    /** The record number */
    Int8                            recordNum;
    /** The list of applications*/
    SimUiccApplicationList          applicationList;
    /** Number of entries that was requested*/
    Int8                            numEntriesDesired;
}
ApexSimReadDirCnf;

/** Request to terminate current actived session.
 * This signal is sent to terminate current actived session.
 */
typedef struct ApexTerminateSessionReqTag
{
    /** Sender's task ID */
    TaskId                          taskId;
    /** Command Reference */
    Int16                           commandRef;
    /** Whether or not to start MM */
    Boolean                         restartMM;
}
ApexTerminateSessionReq;

/** Confirmation to the ApexTerminateSessionReq.
 * This signal is response to terminate current actived session.
 */

typedef struct ApexTerminateSessionCnfTag
{
    /** Sender's task ID */
    TaskId                          taskId;
    /** Command Reference */
    Int16                           commandRef;
    /** An indication of whether or not the request was successful
    * (The value of SIM_REQ_OK indicates success, all other values
    * indicate failure)
    */
    SimRequestStatus                requestStatus;
}
ApexTerminateSessionCnf;

/** This signal is sent to read the content of EF MMSICP or MMSUCP on the SIM.
 * MMS Issuer Connectivity Parameters or MMS User Connectivity Parameters
 */
typedef struct ApexSimReadMmscpReqTag
{
  /** Sender's task ID */
  TaskId                          taskId;
  /** Command reference */
  Int16                           commandRef;
  /** There could be several Connectivity Parameters objects in the file,
   * so this indicates which one is required. Starts from value 0.
   */
  Int8                            startField;
  /* Type of Connectivity Parameters: either Issuer or User Connectivity Parameters */
  SimMmsCpType                    type;
} ApexSimReadMmscpReq;

/** Sent as aresponse to a ApexSimReadMmscpReq */
typedef struct ApexSimReadMmscpCnfTag
{
  /** TASK_BL_ID */
  TaskId                          taskId;
  /** Command reference copied from ApexSimReadMmscpReq */
  Int16                           commandRef;
  /** An indication of whether or not the request was successful
   * (The value of SIM_REQ_OK indicates success, all other values
   * indicate failure)
   */
  SimRequestStatus                requestStatus;
  /** There could be several Connectivity Parameters objects in the file,
   * so this indicates which one is required. Starts from value 0. */
  Int8                            startField;
   /** Type of Connectivity Parameters: either Issuer or User Connectivity Parameters */
   SimMmsCpType                   type;
   /** Relay/ server address */
   SimRelayServerAddress          relayServerAddress;
   /** First interface to core network and bearer */
   SimIfToCoreNetAndBearer        ifToCoreNetAndBearer;
   /** Gateway */
   SimMmsGateway                  gateway;
} ApexSimReadMmscpCnf;


/** This signal is sent to read the content of EF MMSN on the SIM
* EF_MMSN contains the MMS notifications.
*/
typedef struct ApexSimReadMmsnReqTag
{
  /** Sender's task ID */
  TaskId                          taskId;
  /** Command reference */
  Int16                           commandRef;
  /** Defines the record number in EF MMSN, starts from value 1 */
  Int8                            recordNumber;

} ApexSimReadMmsnReq;

/** This signal reads MMS notifications from the SIM*/
typedef struct ApexSimReadMmsnCnfTag
{
    /** Sender's task ID */
    TaskId                          taskId;
    /** Comand reference */
    Int16                           commandRef;
    /** An indication of whether or not the request was successful
    * (The value of SIM_REQ_OK indicates success, all other values
    * indicate failure)
    */
    SimRequestStatus                requestStatus;
    /**  defines the record number in EF MMSN, starts from value 1. */
    Int8                            recordNumber;
    /** Indicates the status */
    MmsStatus                       status;
    /** Indicates the implementation of MMS. */
    MmsImplementation               implementation;
    /** raw MMS notification. */
    MmsNotification                 notification;
    /** Total number of MMSN records available on the SIM/USIM. */
   Int8                             numMmsnRecords;
}
ApexSimReadMmsnCnf;

/** This signal requests the update of a record on MMS Notifications
 * File EF MMSN (0x6FCE).
 */
typedef struct ApexSimWriteMmsnReqTag
{
  /** Sender's task ID */
  TaskId                          taskId;
  /** Command reference */
  Int16                           commandRef;
  /** defines the record number in EF MMSN, starts from value 1 */
  Int8                            recordNumber;
  /** Indicates the status */
  MmsStatus                       status;
  /** Indicates the implementation of MMS. */
  MmsImplementation               implementation;
  /** If set to FALSE, only the status will be updated.
   * The notification will remain unchanged.*/
  Boolean                         updateNotification;
  /** Raw notification data, contains a pointer to the data.
   * Memory allocated by the application layer,
   * and freed by USIM manager after writing it to the SIM.
   * If there is no data, dataLength should be set to 0,
   * and the pointer should be set to PNULL */
  MmsNotification                 notification;
} ApexSimWriteMmsnReq;

/** This signal updates MMS notifications records on the SIM. */
typedef struct ApexSimWriteMmsnCnfTag
{
    /** sender's task ID */
    TaskId                          taskId;
    /** Copied from ApexSimWriteMmsnReq */
    Int16                           commandRef;
    /** An indication of whether or not the request was successful
    * (The value of SIM_REQ_OK indicates success, all other values
    * indicate failure)
    */
    SimRequestStatus                requestStatus;
    /** defines the record number in EF MMSN, starts from value 1. */
    Int8                            recordNumber;
    /** Total number of MMSN records available on the SIM/USIM. */
    Int8                            numMmsnRecords;
}
ApexSimWriteMmsnCnf;

/** Request to delete MMSN file.
 * A signal to request deleting of the specified
 * record from the EF MMSN (MMS Notification) file.
 */
typedef struct ApexSimDeleteMmsnReqTag
{
  /** Sender's task ID */
  TaskId                          taskId;
  /** Comand reference */
  Int16                           commandRef;
  /** Defines the record number in EF MMSN, starts from value 1 */
  Int8                            recordNumber;
} ApexSimDeleteMmsnReq;

/** This signal deletes MMS notifications records on the SIM*/
typedef struct ApexSimDeleteMmsnCnfTag
{
    /** Sender's task ID */
    TaskId                          taskId;
    /** Copied from ApexSimDeleteMmsnReq */
    Int16                           commandRef;
    /** An indication of whether or not the request was successful
    * (The value of SIM_REQ_OK indicates success, all other values
    * indicate failure)
    */
    SimRequestStatus                requestStatus;
    /** Defines the record number in EF MMSN, starts from value 1. */
    Int8                            recordNumber;
    /** Total number of MMSN records available on the SIM/USIM. */
    Int8                             numMmsnRecords;
}
ApexSimDeleteMmsnCnf;

/** SIM get PIN */
typedef struct ApexSimGetPinIndTag
{
  /** TASK_BL_ID */
  TaskId                taskId;
  /** PIN key reference */
  SimUiccKeyRefValue    pinKeyReference;
  /** Indicates if PIN is blocked. */
  Boolean               pinBlocked;
  /** Indicates the status of PIN */
  SimUiccPinStatus      pinStatus;
  /** Indicates the status of unblocked PIN */
  SimUiccPinStatus      unblockPinStatus;
  /** List of prefered language*/
  LanguagePrefList      languagePrefList;
  /** extended language preference list */
  SimExtLangPrefList    extLangPrefList;
}
ApexSimGetPinInd;


/** FL's response to ABSI's request for the PIN1 or unblock PIN1 password  */
typedef struct ApexSimGetPinRspTag
{
  /** TASK_FL_ID */
  TaskId                taskId;
  /** PIN key reference */
  SimUiccKeyRefValue    pinKeyReference;
  /** Indicates that the PIN has already been verified by other means
   * ApexSimGenAccessReq/Cnf for instance (If this flag is set to TRUE,
   * the pinValue/ pinUnblockValue fields will be ignored).
   */
  Boolean               pinValueAlreadyVerified;
  /** Indicates whether we are sending the PIN or PUK code */
  Boolean               pinBlocked;
  /** PIN Value (PIN1 /PIN2) */
  SimUiccPinValue       pinValue;
  /** Unblock PIN Value */
  SimUiccPinValue       pinUnblockValue;
}
ApexSimGetPinRsp;

/** This signal to read MMS user preferences.
 * This signal requests a reading of a record on MMS User
 * Preferences File(EF_MMSUP 6FD1)
 */
typedef struct ApexSimReadMmsupReqTag
{
  /** Sender's task ID */
  TaskId                taskId;
  /** Command reference */
  Int16                 commandRef;
  /** The record number to be read */
  Int8                  recordNumber;

} ApexSimReadMmsupReq;

/** Determines the outcome of the ApexSimReadMmsupReq request.
 * This signal reports on the success/failure of reading a record
 * from the MMS User Preference File and if successful, passes the
 * decoded information read from the specified record of the MMS
 * User Preferences File. requested in AlsiReadMmsupReq.
 */
typedef struct ApexSimReadMmsupCnfTag
{
  /** Sender's task ID */
  TaskId                taskId;
  /** Command reference copies from AlsiReadMmsupReq */
  Int16                 commandRef;
   /** An indication of whether or not the request was successful
   * (The value of SIM_REQ_OK indicates success, all other values
   * indicate failure)
   */
  SimRequestStatus      requestStatus;
  /** The record number read */
  Int8                  recordNumber;
  /** Indicates the implementation of MMS. */
  MmsImplementation     implementation;
  /** The MMS Profile Name present on the SIM/UICC. */
  SimMmsupProfileName   profileName;
  /** MMS user preferences info */
  SimMmsup              mmsup;
  /** Number of MMSUP records on the SIM */
  Int8                  numMmsupRecords;

} ApexSimReadMmsupCnf;

/** Signal to update MMS user preferences.
 * This signal requests the update of a record on MMS
 * User Preferences File(EF_MMSUP 6FD1)
 */
typedef struct ApexSimWriteMmsupReqTag
{
  /** Sender's task ID */
  TaskId                taskId;
  /** Command reference */
  Int16                 commandRef;
  /** The record number to be written */
  Int8                  recordNumber;
  /** Indicates the implementation of MMS. */
  MmsImplementation     implementation;
  /** The MMS Profile Name present on the SIM/UICC. */
  SimMmsupProfileName   profileName;
  /** MMS user preferences information */
  SimMmsup              mmsup;
} ApexSimWriteMmsupReq;

/** This signal reports on the success/failure of the request
 * to update a record on the MMS User Preferences File
 */
typedef struct ApexSimWriteMmsupCnfTag
{
  /** Sender's task ID */
  TaskId                taskId;
  /** Command reference copied form ApexSimWriteMmsupReq */
  Int16                 commandRef;
  /** An indication of whether or not the request was successful
   * (The value of SIM_REQ_OK indicates success, all other values
   * indicate failure)
   */
  SimRequestStatus      requestStatus;
  /** The record number written */
  Int8                  recordNumber;
  /** Number of MMSUP records on the SIM */
  Int8                  numMmsupRecords;
} ApexSimWriteMmsupCnf;

/** Signal to delete MMS user preferences record.
 * This signal is sent to request the deletion of a
 * specified record on the MMS User Preferences File
 */
typedef struct ApexSimDeleteMmsupReqTag
{
  /** Sender's task ID */
  TaskId                taskId;
  /** Command reference */
  Int16                 commandRef;
  /** The record number to be deleted */
  Int8                  recordNumber;
} ApexSimDeleteMmsupReq;

/** This signal reports on the success/failure of the request to
 * delete a record on the MMS User Preferences File
 */
typedef struct ApexSimDeleteMmsupCnfTag
{
  /** Sender's task ID */
  TaskId                taskId;
  /** Copied from ApexSimDeleteMmsupReq */
  Int16                 commandRef;
  /** An indication of whether or not the request was successful
   * (The value of SIM_REQ_OK indicates success, all other values
   * indicate failure)
   */
  SimRequestStatus      requestStatus;
  /** The record number deleted */
  Int8                  recordNumber;
  /** The number of record on the MMS User Preferences File that has been deleted */
  Int8                  numMmsupRecords;
} ApexSimDeleteMmsupCnf;

/*0206-18152 */
/** Signal used to resume SIM access at start up
 * Used by the controlling task to instruct the BL to
 * continue with the start up activities which will have
 * been paused after the SIM was declared OK.
 */
typedef struct ApexSimResumeReqTag
{
  /** Sender's task ID */
  TaskId taskId;
} ApexSimResumeReq;

 /** Sent by FL (using ABSI function) to ABSI, indicating a
  *  change of destination task to/from the SIM Driver
  */
typedef struct ApexSimChangeTargetTaskIdReqTag
{
  /** Sender's task ID */
  TaskId                taskId;
  /** Command reference */
  Int16                 commandRef;
  /** ID of the target task */
  TaskId                targetTaskId;
  /** Whether or not restore original task ID */
  Boolean               restoreOriginalTask;
}
ApexSimChangeTargetTaskIdReq;

/** Indicates the success or failuure of the request.
 * Sent as a response to a AlsiChangeTargetTaskIdReq
 */
typedef struct ApexSimChangeTargetTaskIdCnfTag
{
  /** Sender's task ID  */
  TaskId                taskId;
  /** Copied from ApexSimChangeTargetTaskIdReq */
  Int16                 commandRef;
  /** An indication of whether or not the request was successful
   * (The value of SIM_REQ_OK indicates success, all other values
   * indicate failure)
   */
  SimRequestStatus      requestStatus;
}
ApexSimChangeTargetTaskIdCnf;

/** Request to write ME Personalisation data on the NVRAM */
typedef struct ApexSimWriteMepNetworkIdReqTag
{
  /** Sender's task ID */
  TaskId                taskId;
  /** Comand reference */
  Int16                 commandRef;
  /** Indicates if PLMN to be added */
  Boolean               add;
  /** if add is FALSE and noOfPlmn is 0, then all the entries are deleted */
  Int8                  noOfPlmn;
  /** List PLMN to be added */
  SimPlmn               plmnToAdd[APEX_SIM_MAX_MEP_IN_SIG];
}
ApexSimWriteMepNetworkIdReq;

/** Confirmation of success or failure of the coresponding request */
typedef struct ApexSimWriteMepNetworkIdCnfTag
{
  /** TASK_BL_ID */
  TaskId                taskId;
  /** Copied from ApexSimWriteMepNetworkIdReq */
  Int16                 commandRef;
  /** An indication of whether or not the request was successful
   * (The value of SIM_REQ_OK indicates success, all other values
   * indicate failure)
   */
  SimRequestStatus      requestStatus;
}
ApexSimWriteMepNetworkIdCnf;

/** Request to read ME Personalisation data. */
typedef struct ApexSimReadMepNetworkIdReqTag
{
  /** Sender task ID */
  TaskId                taskId;
  /** Command reference */
  Int16                 commandRef;
  /** PLMN to start reading from */
  Int16                 startPlmn;
}
ApexSimReadMepNetworkIdReq;

/** Confirmation of success or failure of the coresponding request */
typedef struct ApexSimReadMepNetworkIdCnfTag
{
  /** TASK_BL_ID */
  TaskId                taskId;
  /** Copied from ApexSimReadMepNetworkIdReq */
  Int16                 commandRef;
  /** An indication of whether or not the request was successful
   * (The value of SIM_REQ_OK indicates success, all other values
   * indicate failure)
   */
  SimRequestStatus      requestStatus;
  /** The number of PLMN that has been read */
  Int8                  noOfPlmn;
  /** Holds the list of PLMN */
  SimPlmn               plmn[APEX_SIM_MAX_MEP_IN_SIG];
}
ApexSimReadMepNetworkIdCnf;

/*----------------------------------------------------
 * Homezone related signals
 * These signals were added so that the application
 * layer can access the Homezone info on the SIM.
 * --------------------------------------------------*/
/** Allows to read the Homezone parameters stored on the SIM.
 * The homezone parameters file is protected by ADM code,
 * so will only be updated by the SIM itself,
 * so that' s why we don' t have a signal to update this file
 */

typedef struct ApexSimReadHZParamsReqTag
{
   /** ID of the requesting task */
   TaskId               taskId;
   /** Number assigned by the Controlling Task */
   Int16                commandRef;
} ApexSimReadHZParamsReq;

/** This signal returns the Homezone parameters.*/
typedef struct ApexSimReadHZParamsCnfTag
{
   /** TASK_BL_ID */
   TaskId                  taskId;
   /** Copied from the ApexSimReadHZParamsReq */
   Int16                   commandRef;
   /** An indication of whether or not the request was successful */
   SimRequestStatus        requestStatus;
   /** Homezone parameters */
   SimHomeZoneEFParameters hzParameters;
} ApexSimReadHZParamsCnf;

/** Allows to read the Homezone cache stored on the SIM. */
typedef struct ApexSimReadHZCacheReqTag
{
   /** ID of the requesting task */
   TaskId                  taskId;
   /** Number assigned by the Controlling Task */
   Int16                   commandRef;
   /** The cache file to read*/
   SimHomeZoneCacheId      cacheId;
} ApexSimReadHZCacheReq;

/** This signal returns the Homezone cache. */
typedef struct ApexSimReadHZCacheCnfTag
{
   /** Always set to TASK_BL_ID */
   TaskId                  taskId;
   /** Copied from the ApexSimReadHZCacheReq */
   Int16                   commandRef;
    /** An indication of whether or not the request was successful */
   SimRequestStatus        requestStatus;
   /** The cache file we've read*/
   SimHomeZoneCacheId      cacheId;
   /** Homezone cache */
   SimHomeZoneCache        hzCache;
} ApexSimReadHZCacheCnf;

/** Allows to update the Homezone cache stored on the SIM.  */
typedef struct ApexSimWriteHZCacheReqTag
{
   /** ID of the requesting task */
   TaskId                  taskId;
   /** Number assigned by the Controlling Task */
   Int16                   commandRef;
   /** The cache file to update*/
   SimHomeZoneCacheId      cacheId;
   /** Homezone cache to write */
   SimHomeZoneCache        hzCache;
} ApexSimWriteHZCacheReq;

/** This signal confirms whether or not the Homezone cache was updated.*/
typedef struct ApexSimWriteHZCacheCnfTag
{
  /** Always TASK_BL_ID */
  TaskId                   taskId;
  /** Copied from the ApexSimReadHZCacheReq */
  Int16                    commandRef;
   /** An indication of whether or not the request was successful */
  SimRequestStatus         requestStatus;
  /** The cache file we've updated*/
  SimHomeZoneCacheId       cacheId;
} ApexSimWriteHZCacheCnf;

typedef struct ApexSimEmuSimReqTag
{
  /** Always TASK_BL_ID */
  TaskId                  taskId;
  /** Copied from the ApexSimReadHZCacheReq */
  Int16                    commandRef;
  Boolean                ok;
}ApexSimEmuSimReq;

typedef struct ApexSimEmuSimCnfTag
{
  /** Always TASK_BL_ID */
  TaskId                   taskId;
  /** Copied from the ApexSimReadHZCacheReq */
  Int16                    commandRef;
  Boolean                 ok;
}ApexSimEmuSimCnf;

#if defined (UPGRADE_REL8_USIM)

typedef struct ApexSimCsimLockReqTag
{
  /** ID of the requesting task */
  TaskId                  taskId;
  /** Command reference */
    Int16                 commandRef;
  /** whether to lock or unlock for CSIM operation */
  Boolean                 lock;
}ApexSimCsimLockReq;

typedef struct ApexSimCsimLockCnfTag
{
  /** Always TASK_BL_ID */
  TaskId                  taskId;
  /** Copied from the ApexSimCsimLockReq */
  Int16                   commandRef;
  /** whether the locking/unlocking was successful */
  Boolean                 success;
}ApexSimCsimLockCnf;

typedef struct ApexSimCsimLockIndTag
{
  /** Sending task ID (TASK_BL_ID )*/
  TaskId                  taskId;
  /** state of the Csim lock on USIM manager */
  Boolean                 locked;
}ApexSimCsimLockInd;
#endif /* UPGRADE_REL8_USIM */

#if defined (ENABLE_DUAL_SIM_SOLUTION)
/**this signal is sent to the background layer to initiate selection of a particular SIM/UICC*/
typedef struct ApexSimSelectReqTag
{
  /** ID of the requesting task */
  TaskId                  taskId;
  /** Number assigned by the Controlling Task */
  Int16                   commandRef;
  /* which of the two SIMs this selection is for */
  SimHolderType           simHolder;
}ApexSimSelectReq;

/** this signal returns whether the SIm Holder selection was successful or not */
typedef struct ApexSimSelectCnfTag
{
  /** Always TASK_BL_ID */
  TaskId                  taskId;
  /** Number assigned by the Controlling Task */
  Int16                   commandRef;
  /* Copy of value supplied in request signal */
  SimHolderType           simHolder;
  /* whether the selection was successful or not */
  Boolean                 success;
  Boolean                 powerUpProtoStack;
  Boolean                 powerUpSim;
}ApexSimSelectCnf;

#endif

typedef struct ApexSimReadMsisdnReqTag
{
  /** ID of the requesting task */
  TaskId                  taskId;
  /** Number assigned by the Controlling Task */
  Int16                   commandRef;
  /** Requested record number */
  Int8                    recordNumber;
}ApexSimReadMsisdnReq;

typedef struct ApexSimReadMsisdnCnfTag
{
  /** Always TASK_BL_ID */
  TaskId                  taskId;
  /** Number assigned by the Controlling Task */
  Int16                   commandRef;
  /** Requested record number */
  Int8                    recordNumber;
  /** Request read status */
  SimRequestStatus        requestStatus;
  /** Number read         */
  SimDialNumber           dialNumber;
  /** Whether there are more numbers to read */
  Boolean                 more;
}ApexSimReadMsisdnCnf;

#if defined (UPGRADE_CONN_MDT_NB)
#include <simat_proxy_common.h>
#define MAX_DEBUG_MDT_DATA  120

typedef struct ApexSimDebugMdtUnitDataTag
{
  /** received data from SIMAT proxy */
  simat_msg_t msgType;
  Boolean     recvData;
  Int32       dataLen;
  Int8        data[MAX_DEBUG_MDT_DATA];
}ApexSimDebugMdtUnitData;
#endif


/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
 /** @} */ /* End of SigAbsi group */
#endif

/* END OF FILE */
