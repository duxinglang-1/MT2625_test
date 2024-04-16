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
 * Background Shell signals
 **************************************************************************/

#ifndef ABSH_SIG_H
#define ABSH_SIG_H

#if !defined (ABSH_TYP_H)
#include <absh_typ.h>
#endif

#if !defined (ALSI_SIG_H)
#include <alsi_sig.h>   /* typedef for simrequeststatus */
#endif

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

/** \defgroup SigApexSh  ABSH Signalling Interface
 * \ingroup NasApex
 * \ingroup Sig 
 * This document covers the rules that Foreground Layer tasks follow when 
 * they wish to make use of Background Layer, single-user procedures.
 * This section describes the way in which multiple Foreground Layer tasks 
 * manage the control of the Background Layer. The Background Layer has the 
 * concept of a Controlling Task, however existing Foreground Tasks do not 
 * make use of this in a co-operative manner. For instance they make assumptions
 * about whether they have control and they refuse to relinquish control when they 
 * could otherwise do so. 
 *
 * This section looks at single-user procedures and describes when the associated 
 * Foreground Layer tasks should take control and when they should relinquish it.
 * @{
 */


/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/* #define LIFE_THE_UNIVERSE_AND_EVERYTHING 42 */

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/* const char alphabet[] = "ABCDEF...."; */

/***************************************************************************
 * Type Definitions
 **************************************************************************/


/** The ApexAlsiGenericCnfTag signal is a generic signal which allows the 
 * procedure ID to be extracted from the commandRef in a general purpose 
 * signal handler.
 * This signal definition is used to extract the commandRef from
 * all Alsi...Cnf/Req signals.  The BL SHell treats all(ish) Alsi Cnf/Req
 * signals as if they were ApexAlsiGenericCnf signals so that it can extract
 * the ProcId, associated with the commandRef (and also so ABSI can  extract
 * the simRequestStatus).  The ABSH then uses the ProcId  to send the Alsi
 * signal to the appropriate BL procedure, which will then switch on/use the
 * real signal type/structure.
 * \note The ProcId must always be inserted/removed into/from a commandRef using
 * the abglConstructCommandRef()/abglDeconstructCommandRef() functions.
*/
typedef struct ApexAlsiGenericCnfTag
{
  /** Command reference */
  Int16             commandRef;
  /** Indicates the result of the request */
  SimRequestStatus  simRequestStatus;
} ApexAlsiGenericCnf;

/** The ApexShRegisterTaskReq signal registers or deregisters the given Foreground 
 * Layer task with the Background Layer for the specified procedure, according to 
 * the value of isRegistered.
 * Each task that wishes to receive Ind signals from a Background Layer procedure 
 * must register with the Background Layer using an ApexShRegisterTaskReq signal. 
 * The same signal is used to turn off these signals. The Background Layer confirms 
 * registration in an ApexShRegisterTaskCnf.
 *
 * The ApexShRegisterTaskReq signal is handled by the Background Layer Shell.
 *
 * The Background Layer Shell maintains a list of tasks that have registered for each 
 * Background Layer procedure.
 */
typedef struct ApexShRegisterTaskReqTag
{
  /** Requesting task ID */
  TaskId            taskId;
  /**  Background layer procedure to be registered with */
  ProcId            procId;
  /** Whether the requesting task to be registered or deregistered. */
  Boolean           isRegistered;
} ApexShRegisterTaskReq;

/** The ApexShRegisterTaskCnf signal confirms the task is 
 * registered or deregistered.
 */
typedef struct ApexShRegisterTaskCnfTag
{
  /** Sending task ID */
  TaskId            taskId;
  /** Requesting task Id */
  TaskId            otherTaskId;
  /** Copied from corresonding request */
  ProcId            procId;
  /** Indicates if task has been registered/deregistered */
  Boolean           isRegistered;
  /** Indicates the major version of the APEX interface */
  Int16             apexMajorVersionNumber;
  /** Indicates the minor version of the APEX interface */
  Int16             apexMinorVersionNumber;
  /** Indicates the revision of the APEX interface */
  Int16             apexRevisionNumber;
} ApexShRegisterTaskCnf;

/** The ApexShChangeControlReq signal requests control of the 
 * procedure to be given to the newControlTaskId.
 */
typedef struct ApexShChangeControlReqTag
{
  /** Initiator task ID */
  TaskId            taskId;
  /** Set to the task that is to be given control */
  TaskId            newControlTaskId;
  /**  Background layer procedure to control */
  ProcId            procId;
  /** If this is set, the Background Layer gives control to the new task immediately. */
  Boolean           isImmediate;
} ApexShChangeControlReq;

/** The ApexShChangeControlInd signal is an indication to the current 
 * Controlling Task that a request has been made that there should be 
 * a new Controlling Task.
 */
typedef struct ApexShChangeControlIndTag
{
  /** The sending task Id */
  TaskId            taskId;
  /** Set to requesting task ID */
  TaskId            initiatingTaskId;
  /** Task that is given control */
  TaskId            newControlTaskId;
  /**  Background layer procedure to control copied from the 
   * corresponding request. 
   */
  ProcId            procId;
} ApexShChangeControlInd;

/** The ApexShChangeControlRsp signal is the response from the 
 * current Controlling Task saying whether it agrees with the 
 * request from the new task.
 */
typedef struct ApexShChangeControlRspTag
{
  /** The sending task ID */
  TaskId                taskId;
  /** Indicates the result of the request. */
  ChangeControlStatus   changeControlStatus;
  /** Requesting task ID */
  TaskId                initiatingTaskId;
  /** Task that is given control. */
  TaskId                newControlTaskId;
  /**  Background layer procedure to control. */
  ProcId                procId;
} ApexShChangeControlRsp;

/** This signal is sent to the initiator to confirm the success 
 * or failure of the request. 
 * The ApexShChangeControlCnf signal confirms to the task that 
 * initiated the request whether the change of control will happen or not.
 */
typedef struct ApexShChangeControlCnfTag
{
  /** The sending task ID */
  TaskId                taskId;
  /** Indicates the result of the request. */
  ChangeControlStatus   changeControlStatus;
  /** Requesting task ID */
  TaskId                initiatingTaskId;
  /** Task that is given control. */
  TaskId                newControlTaskId;
  /**  Background layer procedure to control. */
  ProcId                procId;
} ApexShChangeControlCnf;

/** The ApexShGotControlInd signal is an indication sent to all 
 * registered tasks when control has been assigned to a new Controlling Task.
 */
typedef struct ApexShGotControlIndTag
{
  /** The sending task ID */
  TaskId                taskId;
  /** Requesting task ID */
  TaskId                initiatingTaskId;
  /** Task that is given control. */
  TaskId                newControlTaskId;
  /**  Background layer procedure to control. */
  ProcId                procId;
} ApexShGotControlInd;

/** The AbshTaskRegisteredSig signal is internal to the Background Layer. 
 * This signal is sent when a Foreground Layer task has registered for a 
 * procedure and will cause indication signals to be sent to the Foreground 
 * Layer task.
 */
typedef struct AbshTaskRegisteredSigTag
{
  /** Copied from ApexShRegisterTaskReq */
  TaskId                taskId;
  /**  Background layer procedure to be registered with
   * (Copied from ApexShRegisterTaskReq). 
   */
  ProcId                procId;
} AbshTaskRegisteredSig;

/** This is a local reset signal. 
 * The AbshLocalResetSig signal used to reset the lower layers. 
 * The lower layers must be in an idle state before this reset signal 
 * can be handled.
 */
typedef struct AbshLocalResetSigTag   /* FR9802-2076 */
{
  /** TASK_BL_ID */
  TaskId                taskId;
  /** set to mobility management procedure */
  ProcId                procId;
  /** The cause for a local reset of the system */
  AbshLocalResetCause   abshLocalResetCause;
} AbshLocalResetSig;


#if defined (ON_PC)
/** Request the background layer to start timer.
 * This signal is sent to the background layer (from the background layer) 
 * which will start the timer.
 * This signal is used in unit testing (hence ON_PC) to make unit testing 100%
 * deterministic.
 */
typedef struct AbshStartTimerSigTag
{
  /** Pointer to timer information */
  KiTimer *timerToStart_p;
} AbshStartTimerSig;

/** Request the background layer to stop timer.
 * This signal is sent to the background layer (from the background layer) 
 * which will stop the timer.
 * This signal is used in unit testing (hence ON_PC) to make unit testing 100%
 * deterministic.
 */
typedef struct AbshStopTimerSigTag
{
  /** Pointer to timer information */
  KiTimer *timerToStop_p;
} AbshStopTimerSig;
#endif


/***************************************************************************
 *  Macros
 **************************************************************************/


/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

/** @} */ /* End of SigApexSh group */

#endif

/* END OF FILE */
