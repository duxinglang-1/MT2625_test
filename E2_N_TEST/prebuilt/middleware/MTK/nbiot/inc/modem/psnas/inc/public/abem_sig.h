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
 *    Application Layer Background Layer Engineering Mode Signal Definitions
 **************************************************************************/

#ifndef ABEM_SIG_H
#define ABEM_SIG_H

#include <abem_typ.h>

#include <mmr_sig.h>

#if defined (UPGRADE_NASMDL2)
#include <smr_sig.h>
/* TODO: Remove following under #else part of UPGRADE_NASMDL2 */
#endif /* UPGRADE_NASMDL2 */
#include <smab_sig.h>

/***************************************************************************
 * Nested Include Files
 **************************************************************************/
/** \defgroup SigAbem ABEM Signalling Interface
 * \ingroup NasApex
 * \ingroup Sig
 * Application Layer Background Layer Engineering Mode Signal Definitions.
 * This section details the Mediatek Engineering Mode signals that are required for debug.
 * The module that handles engineering mode data passed up from the protocol
 * stack is ABEM (Application Background Engineering Mode).  This module requires
 * information to be passed up from various tasks in the protocol stack in order
 * to make protocol stack data available at the ABEM interface.
 * @{
 */

/***************************************************************************
 * Manifest Constants
 **************************************************************************/


/***************************************************************************
 * Typed Constants
 **************************************************************************/


/***************************************************************************
 * Type Definitions
 **************************************************************************/

/** Logger Data Request.
 * Sent from FL to BL when the state of the ASP logger is to be changed
 */
typedef struct ApexEmAspLoggerReqTag
{
  /** The sending task ID */
  TaskId          taskId;
  /** Whether the logging is on or off */
  Boolean         activate;
}
ApexEmAspLoggerReq;

/** Confirm an ApexEmAspLoggerReq.
 * Sent from BLto FL in response to the logger state change request
 */
typedef struct ApexEmAspLoggerCnfTag
{
  /** The sending task ID */
  TaskId            taskId;
  /** Indicates the result of the request */
  EmRequestStatus   requestStatus;
  /** Whether or not the ASP logger is active */
  Boolean           aspLoggerActive;
}
ApexEmAspLoggerCnf;

/** Change in engineering Info Request.
 * Sent from FL to BL when the engineering information required is changed */
typedef struct ApexEmInfoReqTag
{
  /** The sending task ID */
  TaskId          taskId;
  /** Revised engineering info requirements */
  EngineeringData engineeringData;
}
ApexEmInfoReq;

/** Confirm the change to the requesting foreground layer.
 * This signal is Sent from BL to FL to confirm success or failure of the request
 */
typedef struct ApexEmInfoCnfTag
{
  /** Sending task ID */
  TaskId          taskId;
  /** Indicates the result of the request */
  EmRequestStatus requestStatus;
  /** Copied from the request */
  EngineeringData engineeringData;
}
ApexEmInfoCnf;

/** Indicates the foreground layer(s) what engineering info is now active.
 * Sent from BL to FL when the engineering information required has been changed
 */
typedef struct ApexEmReadyIndTag
{
  /** Sending task ID */
  TaskId               taskId;
  /** Indicates the result of the request */
  EmRequestStatus      requestStatus;
  /** Revised engineering info requirements */
  EngineeringData      engineeringData;
  /** Determine whether the logger is on or off */
  Boolean              aspLoggerActive;
}
ApexEmReadyInd;

#if defined (UPGRADE_GGE)
/** This signal is sent while in idle mode.
 * This signal provides information on the serving cell and neighbour cells
 * for engineering purposes
 */
typedef struct ApexEmGrrIdleInfoIndTag
{
  /** The sending task ID */
  TaskId               taskId;
  /** Holds data sent in GrrTestIdleInfoInd */
  EmGrrTestIdleInfo    grrTestIdleInfoInd;  /* See note above */
}
ApexEmGrrIdleInfoInd;

/** This signal is sent while in dedicated mode.
 * This signal provides information on the serving cell and neighbour cells during
 * dedicated mode for engineering purposes
 */
typedef struct ApexEmGrrDedInfoIndTag
{
  /** The sending task ID */
  TaskId               taskId;
  /** Holds data sent in GrrTestDedInfoInd */
  EmGrrTestDedInfo     grrTestDedInfoInd;   /* See note above */
}
ApexEmGrrDedInfoInd;

/** This signal is sent on entry into idle mode.
 * This signal provides information on the serving cell and neighbour cells
 * for engineering purposes
 */
typedef struct ApexEmGrrIdleSCellIndTag
{
  /** The sending task ID */
  TaskId               taskId;
  /** Holds data sent in GrrTestIdleSCellInd*/
  EmGrrIdleSCellInfo   grrTestIdleSCellInd;  /* See note above */
}
ApexEmGrrIdleSCellInd;

/** This signal is sent on entry into dedicated mode.
 * This signal provides information on the configuration of the traffic
 * channel for engineering purposes.
 */
typedef struct ApexEmGrrDedSCellIndTag
{
  /** The sending task ID */
  TaskId               taskId;
  /** Holds data sent inGrrTestDedScellInd */
  EmGrrDedSCellInfo    grrTestDedSCellInd;   /* See note above */
}
ApexEmGrrDedSCellInd;
#endif

/** Indicates that some engineering info (for instance LAI) has changed.
 * This signal is sent if any of the information in the signal changes in GMM
 */
typedef struct ApexEmMmrInfoIndTag
{
  /** The sending task ID */
  TaskId               taskId;
  /** Holds MMR engineering mode information */
  MmrInfoInd           mmrInfoInd;
}
ApexEmMmrInfoInd;

#if defined (UPGRADE_GGE)
/** This signal is sent whenever the RLC-MAC engineering mode information
 * is received by GRR. The signal contains the RLC-MAC information plus
 * some further GRR GPRS data
 */
typedef struct ApexEmGpRlcmacInfoIndTag
{
  /** The sending task ID */
  TaskId                 taskId;
  /** Contains RLC-MAC information plus some GRR GPRS data */
  EmGrrTestGprsEngInfo   grrTestGprsInfoInd;
}
ApexEmGpRlcmacInfoInd;
#endif

/** Inform changes to PDP context / EPS Bearer.
 * Send an indication from BL to FL when PDP context / EPS Bearer information
 * changes
 */
typedef struct ApexEmPdSmInfoIndTag
{
  /** The sending tsk ID */
  TaskId                 taskId;
   /** Carries SM engineering mode information from SM task */
  SmrTestInfoInd         smrTestInfoInd;
}
ApexEmPdSmInfoInd;

#if defined (UPGRADE_GGE)
/** Indication of the status information to FL for the Engineering Display.
 * This signal is sent from BL to FL containing LLC engineering mode information */
typedef struct ApexEmGpLlcInfoIndTag
{
  /** The sending task ID */
  TaskId                 taskId;
  /** Contains MmrLlcInfoInd signal */
  MmrLlcInfoInd          mmrLlcInfoInd;
}
ApexEmGpLlcInfoInd;
#endif


#if defined (UPGRADE_3G)
/** Indication of RRC information to FL for the Engineering Display.
 * This signal is sent from BL to FL containing RRC engineering mode information */
typedef struct ApexEmRrcInfoIndTag
{
  /** The sending task ID */
  TaskId               taskId;
  /** Carries RRC engineering mode information */
  EmRrcEngInfoInd      rrcEngInfoInd;
}
ApexEmRrcInfoInd;
#endif /* UPGRADE_3G */

/** Production test indication.
 * Inform all upper layers of any production test functions which may be
 *  necessary
 */
typedef struct ApexEmProductionTestIndTag
{
  /** The sending task ID */
  TaskId               taskId;
  /** Run keypad test on production line     */
  Boolean              testKeypadOnStart;
  /** Run display test on production line    */
  Boolean              testDisplayOnStart;
  /** Enter L1 null state on production line */
  Boolean              testLayer1OnStart;
  /** Run LED test on production line         */
  Boolean              testLedOnStart;
#if !defined (REMOVE_PROD_SERIAL_NUM)
  /** Contains serial number */
  Char    serialNumber[SERIAL_NUMBER_LEN];
#endif
  /** This boolean may be used to lockout an IMEI change*/
  Boolean              imeiLockout;   /*CR:9910-5354*/
}
ApexEmProductionTestInd;

/*CR:9910-5277*/
/** Reset indication.
 * Indication that NVRAM has been reset. */
typedef struct ApexEmResetNvramIndTag
{
  /** The sending task ID */
  TaskId               sourceTaskId;
}
ApexEmResetNvramInd;

/** This signal is sent as a response to ApexEmResetNvramInd.
 * This signal idicates the success of reseting the NVRAM
 */
typedef struct ApexEmResetNvramRspTag
{
  /** Whether or not the NVRAM is successfuly reset */
  Boolean               successful;
}
ApexEmResetNvramRsp;

/** Sent from FL to BL.
 * This signal is sent in order to obtain the requested PLMN.
 */
typedef struct ApexEmPlmnTestReqTag
{
  /** The sending task ID */
  TaskId               taskId;
  /** Indicates MCC/MNC order */
  EmPlmnTestType       plmnOrder;
  /** The requested PLMN */
  Plmn                 requestedPlmn;
}
ApexEmPlmnTestReq;

/** This signal is sent as a response to a ApexEmPlmnTestReq.
 * Read a PLMN from the background configuration data and send a
 * signal back containing the PLMN name - matching against the MNC and MCC
 */
typedef struct ApexEmPlmnTestCnfTag
{
  /** The sending task ID */
  TaskId               taskId;
  /** Whether or not PLMN is found */
  Boolean              plmnFound;
  /** Copied from the request */
  Plmn                 requestedPlmn;
  /** Filled in as the actual name structure */
  PlmnName             selectedName;
}
ApexEmPlmnTestCnf;

/** Engineering mode mandatory command type */
typedef enum ApexEmForceCommandTag
{
    EM_LOCK_CELL,
    EM_UNLOCK_CELL,
    EM_LOCK_ARFCN,
    EM_UNLOCK_ARFCN,
    EM_FULL_SCAN,
    EM_SPECIFIED_ARFCN_SCAN,
    EM_TRIGGER_HANDOVER,
    EM_HANDOVER_PERMITTED,
    EM_TRIGGER_CELL_RESELECTION,
    EM_SELECT_A_CELL_ONE_TIME,  
    EM_POWER_SAVING,
    EM_NO_COMMAND
}
ApexEmForceCommand;

/** Engineering mode Force command type */
typedef enum ApexEmForceCommandFailCauseTag
{
    EM_CELL_NORMAL_SERVICE = 0,

    /** Command can NOT be overrided. */
    EM_CMD_OVERRIDED,

    /** Command is not permitted because of CSR status.*/
    EM_CMD_FORBIDDEN,

    /** ACCEPTABLE cell */
    EM_CELL_LIMITED_SERVICE,

    /** Target cell can'be camped on suitable */
    EM_CELL_NOT_CAMP,

    /** Target cell is not valid  */
    EM_CELL_NOT_VALID,

    /** In first step, full Rssi scan only support in IDLE MODE,
    * not support connected and deactivated mode
    */
    EM_UE_NOT_IN_IDLE_MODE,

    /** In first step, full Rssi scan only support in CSRR Proc,
    * not support CSRC,CSRS,CSRP,NOT_PROC
    */
    EM_UE_NOT_IN_CSRR_PROC,

    /** In first step, if RBC,CER should process SIR msg or
    * pending request exist, full Rssi scan will be blocked
    */
    EM_SCAN_BLOCKED,
    /** Indicate command can't be processed in current state.
      */
    EM_STATE_MISMATCH,

    /** Indicate target cell is not requested one.
      */
    EM_OCCURRED_BUT_CELL_MISMATCH,

    /** Indicate current camp on cell is locked.
      */
    EM_CELL_LOCKED,

    /** Indicate current camp on cell whose arfcn is locked.
      */
    EM_ARFCN_LOCKED,

    /** Indicate request handover is not occurred during specified timer.
      */
    EM_NOT_OCCURRED_WHEN_TIMER_EXPIRY,

    /** Indicate target cell's PLMN is not belongs to serving cell's PLMN or EPLMNs.
      */
    EM_PLMN_MISMATCH,

    /** Can be used for all commands,
      * Indicate command process failure for unspecified cause.
      */
    EM_UNSPECIFIED_CAUSE,

    /** Indicate system mode mismatch.
      */
    EM_SYSMODE_MISMATCH,

    /** Indicate operation is not allowed for ABMM is in selecting state.
      */
    EM_OPERATION_NOT_ALLOWED

}
ApexEmForceCommandFailCause;

/**
 * Engineering mode command result information.
 */
typedef union ApexEmCommandResultInfoTag
{
    /** For command 'FULL_SCAN',
     * Full band scan result information list.
     */
    EmScanResultList            fullScanResultList;

    /** For command 'SPECIFIED_ARFCN_SCAN',
      * Carries the contents of specified arfcn scan result information.
      */
    EmSpecifiedScanResultList   specifiedScanResultList;
}
ApexEmCommandResultInfo;

typedef union ApexEmCommandInfoTag
{
    /** For command 'LOCK_CELL' */
    EmLockCellInfo                      lockCellInfo;

    Int8                                dummy1;

    /** For command 'LOCK_ARFCN' */
    EmLockArfcnInfo                     lockArfcnInfo;

    Int8                                dummy2;

    Int8                                dummy3;

    /** For command 'SPECIFIED_ARFCN_SCAN' */
    EmSpecifiedArfcnScanInfo            specifiedArfcnScanInfo;

    /** For command 'TRIGGER_HANDOVER' */
    EmTriggerHandoverInfo               triggerHandoverInfo;

    /** For command 'HANDOVER_PERMITTED' */
    EmHandoverPermittedInfo             handoverPermittedInfo;

    /** For command 'TRIGGER_CELL_RESELECTION' */
    EmTriggerCellReselectionInfo        triggerCellReselectionInfo;

    /** For command 'SELECT_A_CELL_ONE_TIME' */
    EmSelectACellOneTimeInfo            selectACellOneTimeInfo;

    EmCmdPowerSavingInfo                powerSavingInfo;

    Int8                                dummy4;
}
ApexEmCommandInfo;

/** Engineering mode force command.
 * Sent from ATCI to BL when the neet to force to change something in engineering mode */
typedef struct ApexEmInfoForceCmdReqTag
{
           /** force handle command
\assoc UNION \ref commandInfo */
    ApexEmForceCommand forceHandleCommand;

    /** define the user selected mode and active RAT */
    NetworkModes        sysMode;

    /** Carries the contents of the force locked select cell inf */
    ApexEmCommandInfo  commandInfo;
}
ApexEmInfoForceCmdReq;

/** This signal is sent as a response to a ApexEmInfoForceCmdReq.
 */
typedef struct ApexEmInfoForceCmdCnfTag
{
   /** Set to TRUE if force command is successful handle is required */
    Boolean                        commandSuccessHandle;

   /** Carries the contents of the force command return error cause */
    ApexEmForceCommandFailCause    commandFailCause;

       /** force handle command
\assoc UNION \ref scanResultList */
    ApexEmForceCommandResultType   commandResultType;

   /** Carries the contents of the scan result of cell frequency and RSCP */
    ApexEmCommandResultInfo        scanResultList;
}
ApexEmInfoForceCmdCnf;

/***************************************************************************
 *  Macros
 **************************************************************************/
/** @} */ /* End of SigAbem group */

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

#endif

/* END OF FILE */
