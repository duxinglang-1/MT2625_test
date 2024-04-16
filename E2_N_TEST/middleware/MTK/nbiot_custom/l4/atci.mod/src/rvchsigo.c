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
 * File Description
 * ----------------
 *
 * RVCHSIGO.C
 * Change control manager out signals
 **************************************************************************/

#define MODULE_NAME "RVCHSIGO"

#define MODULE_NAME "RVCHSIGO"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif

#include <gkisig.h>

#if !defined (RVUTIL_H)
#  include <rvutil.h>
#endif
#if !defined (RVDATA_H)
#  include <rvdata.h>
#endif
#if !defined (RVCHMAN_H)
#  include <rvchman.h>
#endif
#if !defined (RVCHSIGO_H)
#  include <rvchsigo.h>
#endif

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  ApexShRegisterTaskReq   apexShRegisterTaskReq;
  ApexShChangeControlReq  apexShChangeControlReq;
  ApexShChangeControlRsp  apexShChangeControlRsp;
};

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexShRegisterTaskReq
 *
 * Parameters:      ProcId - background layer procedure to be registered with
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SH_REGISTER_TASK_REQ signal to the
 *                  background layer. This allows us to gain control of
 *                  this BL procedure at a later time and also we will recieve
 *                  unsolicited signals about the functionality of the
 *                  procedure.
 *-------------------------------------------------------------------------*/

void vgSigApexShRegisterTaskReq (ProcId procId,
                                  const VgmuxChannelNumber entity)
{
  SignalBuffer          signalBuffer = kiNullBuffer;
  ApexShRegisterTaskReq *request_p;

  sendSsRegistrationSignal (CHANGE_CONTROL_MANAGER,
                             entity,
                              SIG_APEX_SH_REGISTER_TASK_CNF);

  KiCreateZeroSignal (SIG_APEX_SH_REGISTER_TASK_REQ,
                       sizeof (ApexShRegisterTaskReq),
                        &signalBuffer);

  request_p = (ApexShRegisterTaskReq *)signalBuffer.sig;
  request_p->taskId       = VG_CI_TASK_ID;
  request_p->procId       = procId;
  request_p->isRegistered = TRUE;

  KiSendSignal (TASK_BL_ID, &signalBuffer);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexShChangeControlReq
 *
 * Parameters:      TaskId - task ID that we want control to be given to
 *                  ProcId - background layer procedure to get control of
 *                  Boolean - whether we are gaining or releasing control
 *                  VgmuxChannelNumber - entity which sent request
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SH_CHANGE_CONTROL_REQ to the
 *                  background layer
 *
 *-------------------------------------------------------------------------*/

void vgSigApexShChangeControlReq (TaskId taskCtrl,
                                   ProcId proc,
                                    Boolean isImmediate,
                                     const VgmuxChannelNumber entity)
{
  SignalBuffer           signalBuffer = kiNullBuffer;
  ApexShChangeControlReq *request_p;

  sendSsRegistrationSignal (CHANGE_CONTROL_MANAGER,
                             entity,
                              SIG_APEX_SH_CHANGE_CONTROL_CNF);

  KiCreateZeroSignal (SIG_APEX_SH_CHANGE_CONTROL_REQ,
                       sizeof (ApexShChangeControlReq),
                        &signalBuffer);

  request_p                   = (ApexShChangeControlReq *) signalBuffer.sig;
  request_p->taskId           = VG_CI_TASK_ID;
  request_p->newControlTaskId = taskCtrl;
  request_p->procId           = proc;
  request_p->isImmediate      = isImmediate;

  KiSendSignal (ALSH_TASK_ID, &signalBuffer);
}



/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexShChangeControlRsp
 *
 * Parameters:      ChangeControlStatus - response status
 *                  TaskId - task ID that sent request
 *                  TaskId - task ID that needs control
 *                  ProcId - background layer procedure
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SH_CHANGE_CONTROL_RSP to the
 *                  background layer giving up control if we are not using it
 *
 *-------------------------------------------------------------------------*/

void vgSigApexShChangeControlRsp (ChangeControlStatus status,
                                   TaskId initTask,
                                    TaskId newTask,
                                     ProcId proc)
{
  SignalBuffer           signalBuffer = kiNullBuffer;
  ApexShChangeControlRsp *request_p;

  KiCreateZeroSignal (SIG_APEX_SH_CHANGE_CONTROL_RSP,
                       sizeof (ApexShChangeControlRsp),
                        &signalBuffer);

  request_p                      = (ApexShChangeControlRsp *) signalBuffer.sig;
  request_p->taskId              = VG_CI_TASK_ID;
  request_p->changeControlStatus = status;
  request_p->initiatingTaskId    = initTask;
  request_p->newControlTaskId    = newTask;
  request_p->procId              = proc;

  KiSendSignal (ALSH_TASK_ID, &signalBuffer);
}


/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */



