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
 * Outgoing signal handlers for the Sim lock Sub-System.
 *
 * Procedures simply send a signal. The signal is created, its expected
 * returning signal is registered, contents filled and then it is sent.
 **************************************************************************/

#define MODULE_NAME "RVSLSIGO"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <gkisig.h>

#include <rvutil.h>
#include <rvdata.h>
#include <rvslsigo.h>
#include <rvchman.h>
#include <dmpm_sig.h>
#include <rvcfg.h>
#include <rvccut.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  ApexSimGetChvRsp        apexSimGetChvRsp;
  ApexPmModeChangeReq     apexPmModeChangeReq;
  ApexSimGenAccessReq     apexSimGenAccessReq;
  ApexSimGenAccessReq     apexSimReadSimParamReq;
#if defined (ENABLE_DUAL_SIM_SOLUTION)
  ApexSimSelectReq        apexSimSelectReq;
#endif
  ApexPmPowerGoingDownRsp apexPmPowerGoingDownRsp;

  ApexSimOpenLogicalChannelReq  apexSimOpenLogicalChannelReq;
  ApexSimCloseLogicalChannelReq apexSimCloseLogicalChannelReq;
  ApexSimLogicalChannelAccessReq apexSimLogicalChannelAccessReq;

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
* Function:    vgSigApexSimGetChvRsp
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends a SIG_APEX_SIM_GET_CHV_RSP to the
*              background containing the user entered pin number and PUK
*              codes.
*-------------------------------------------------------------------------*/

void vgSigApexSimGetChvRsp(const VgmuxChannelNumber entity)
{
  SignalBuffer            sigBuff                    = kiNullBuffer;
  ApexSimGetChvRsp        *request_p;
  SimLockGenericContext_t *simLockGenericContext_p  = ptrToSimLockGenericContext ();

  FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

  KiCreateZeroSignal (SIG_APEX_SIM_GET_CHV_RSP,
                       sizeof (ApexSimGetChvRsp),
                        &sigBuff);

  /* Initialise signal and send.... */
  request_p              = (ApexSimGetChvRsp *) sigBuff.sig;
  request_p->taskId      = VG_CI_TASK_ID;
  request_p->chvNum      = simLockGenericContext_p->chvNum;
  {
     request_p->chvValueAlreadyVerified = FALSE;   /*job 104913*/
     request_p->chvBlocked  = simLockGenericContext_p->simBlocked;
     memcpy( &request_p->chvValue.value[0],
              simLockGenericContext_p->pinCode,
              SIM_CHV_LENGTH);
     memcpy( &request_p->chvUnblockValue.value[0],
             simLockGenericContext_p->pukCode,
             SIM_CHV_LENGTH);
  }



  KiSendSignal (TASK_BL_ID, &sigBuff);

}
/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimGetPinRsp
 *
 * Parameters:  VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Function which sends a SIG_APEX_SIM_GET_PIN_RSP to the
 *              background containing the user entered pin number and PUK
 *              codes.
 *-------------------------------------------------------------------------*/

void vgSigApexSimGetPinRsp(const VgmuxChannelNumber entity)
{
  SignalBuffer          sigBuff = kiNullBuffer;
  ApexSimGetPinRsp      *request_p;
  SimLockGenericContext_t *simLockGenericContext_p  = ptrToSimLockGenericContext ();

  FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

  KiCreateZeroSignal (SIG_APEX_SIM_GET_PIN_RSP,
                       sizeof (ApexSimGetPinRsp),
                        &sigBuff);

  /* Initialise signal and send.... */
  request_p              = (ApexSimGetPinRsp *) sigBuff.sig;
  request_p->taskId      = VG_CI_TASK_ID;
  request_p->pinKeyReference      = simLockGenericContext_p->keyRef;
  request_p->pinValueAlreadyVerified = FALSE;   /*job 104913*/
  request_p->pinBlocked  = simLockGenericContext_p->simBlocked;
  memcpy( &request_p->pinValue.value[0],
          simLockGenericContext_p->pinCode,
          SIM_PIN_LENGTH);
  memcpy( &request_p->pinUnblockValue.value[0],
          simLockGenericContext_p->pukCode,
          SIM_PIN_LENGTH);

  KiSendSignal (TASK_BL_ID, &sigBuff);

}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimUsimAppStartReq
 *
 * Parameters:  VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Function which sends a SIG_APEX_SIM_USIM_APP_START_REQ to the
 *              background.
 *-------------------------------------------------------------------------*/
void vgSigApexSimUsimAppStartReq(const VgmuxChannelNumber entity)
{
    SignalBuffer            sigBuff                  = kiNullBuffer;
    ApexSimUsimAppStartReq *request_p                = PNULL;
    SimLockContext_t       *simLockContext_p = ptrToSimLockContext(entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
    FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

    sendSsRegistrationSignal(   SIM_LOCK,
                                entity,
                                SIG_APEX_SIM_USIM_APP_START_CNF);

    KiCreateZeroSignal (SIG_APEX_SIM_USIM_APP_START_REQ,
                        sizeof (ApexSimUsimAppStartReq),
                        &sigBuff);

    /* Initialise signal and send.... */
    request_p                       = (ApexSimUsimAppStartReq *) sigBuff.sig;
    request_p->taskId               = VG_CI_TASK_ID;
    request_p->recordNum            = simLockContext_p->dirReqStartRecord;
    request_p->commandRef           = (Int16)entity;

    KiSendSignal (TASK_BL_ID, &sigBuff);
}/* vgSigApexSimUsimAppStartReq */

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimReadDirReq
 *
 * Parameters:  VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Function which sends a SIG_APEX_SIM_READ_DIR_REQ to the
 *              background.
 *-------------------------------------------------------------------------*/
void vgSigApexSimReadDirReq(const VgmuxChannelNumber entity)
{
    SignalBuffer          sigBuff           = kiNullBuffer;
    ApexSimReadDirReq    *request_p         = PNULL;
    SimLockContext_t     *simLockContext_p  = ptrToSimLockContext (entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
    FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

    sendSsRegistrationSignal(   SIM_LOCK,
                                entity,
                                SIG_APEX_SIM_READ_DIR_CNF);

    KiCreateZeroSignal (SIG_APEX_SIM_READ_DIR_REQ,
                        sizeof (ApexSimReadDirReq),
                        &sigBuff
                       );

    /* Initialise signal and send.... */
    request_p                       = (ApexSimReadDirReq *) sigBuff.sig;
    request_p->taskId               = VG_CI_TASK_ID;
    request_p->recordNum            = simLockContext_p->dirReqStartRecord;
    request_p->numEntriesDesired    = simLockContext_p->dirReqNumRecord;
    request_p->commandRef           = (Int16)entity;

    KiSendSignal (TASK_BL_ID, &sigBuff);
}/* vgSigApexSimReadDirReq */


 /*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexPmModeChangeReq
 *
 * Parameters:      VgmuxChannelNumber - entity which sent request
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_PM_CHANGE_MODE_REQ to the AB. This does a
 *                  power-up or power-down the phone (does not actually
 *                  fully power off phone)
 *-------------------------------------------------------------------------*/
void vgSigApexPmModeChangeReq (const VgmuxChannelNumber entity)

{
  SignalBuffer        signalToSend = kiNullBuffer;
  ApexPmModeChangeReq *request_p;
  SimLockContext_t    *simLockContext_p = ptrToSimLockContext(entity);
  VgCFUNData          *vgCFUNData;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
  vgCFUNData = &(simLockContext_p->vgCFUNData);

  FatalAssert (vgChManCheckHaveControl (CC_POWER_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (SIM_LOCK,
                             entity,
                              SIG_APEX_PM_MODE_CHANGE_CNF);

  KiCreateZeroSignal (SIG_APEX_PM_MODE_CHANGE_REQ,
                       sizeof(ApexPmModeChangeReq),
                        &signalToSend);

  request_p         = (ApexPmModeChangeReq *) signalToSend.sig;
  request_p->taskId = VG_CI_TASK_ID;

  /* Mode change reset parameter */
  request_p->newState.resetNow = vgCFUNData->resetNow;

  if (vgCFUNData->resetNow == TRUE)
  {
    request_p->newState.protoStackState.makePersistent = FALSE;
    request_p->newState.simState.makePersistent        = FALSE;
  }
  else
  {
    request_p->newState.protoStackState.makePersistent = TRUE;
    request_p->newState.simState.makePersistent        = TRUE;
  }

  /* Set power up/down parameters for SIM and protocol stack */
  request_p->newState.simState.powerUp        = vgCFUNData->powerUpSim;
  request_p->newState.protoStackState.powerUp = vgCFUNData->powerUpProtoStack;

  KiSendSignal (cfRvAbTaskId, &signalToSend);
}


 /*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexPmPowerGoingDownRsp
 *
 * Parameters:      none
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_PM_POWER_GOING_DOWN_RSP to the AB. This is
 *                  sent immediately after the CI Task receives a power going
 *                  down indication.
 *-------------------------------------------------------------------------*/
void vgSigApexPmPowerGoingDownRsp (void)

{
  SignalBuffer             signalToSend = kiNullBuffer;
  ApexPmPowerGoingDownRsp* request_p;

  KiCreateZeroSignal (SIG_APEX_PM_POWER_GOING_DOWN_RSP,
                       sizeof(ApexPmPowerGoingDownRsp),
                        &signalToSend);

  request_p         = (ApexPmPowerGoingDownRsp *) signalToSend.sig;
  request_p->taskId = VG_CI_TASK_ID;

  KiSendSignal (cfRvAbTaskId, &signalToSend);
}



 /*--------------------------------------------------------------------------
 *
 * Function:        vgSendApexSimGenAccessReq
 *
 * Parameters:      VgmuxChannelNumber - mux channel number
 *                  Int16              - length of SIM command
 *                  Int8*              - pointer to command data
 *                  Int16              - EF id
 *                  Int16              - DIR id
 *                  Int16              - rootDir id
 *                  Boolean            - responseWanted
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SIM_GEN_ACCESS_REQ to the ABSI task.
 *                  This signal contains a generic command for the SIM card.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSendApexSimGenAccessReq (const VgmuxChannelNumber entity,
                                        Int16 length,
                                        const Int8 *src_p,
                                        Int16 efId,
                                        Int16 dirId,
                                        Int16 rootDirId,
                                        /* job134856: add handling for <pathid> field */
                                        const Int8 *path_p,
                                        Int8 pathLength)
{
  SimLockContext_t *simLockContext_p = ptrToSimLockContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
  simLockContext_p->simGenAccess.length     = length;
  simLockContext_p->simGenAccess.efId       = efId;
  simLockContext_p->simGenAccess.dirId      = dirId;
  simLockContext_p->simGenAccess.rootDirId  = rootDirId;
  simLockContext_p->simGenAccess.commandRef = (Int16)entity;
  /* job134856: add handling for <pathid> field */
  if (pathLength > 0)
  {
    memcpy (&simLockContext_p->simGenAccess.path.data[0], path_p, pathLength);
  }
  simLockContext_p->simGenAccess.path.length = pathLength;

  memcpy (&simLockContext_p->simGenAccess.commandData[0], src_p, length);

  return (vgChManContinueAction (entity, SIG_APEX_SIM_GEN_ACCESS_REQ));
}

 /*--------------------------------------------------------------------------
 *
 * Function:        vgSendApexSimLogicalChannelAccessReq
 *
 * Parameters:      VgmuxChannelNumber - mux channel number
 *                  Int16              - length of SIM command
 *                  Int8*              - pointer to command data
 *                  Int16              - EF id
 *                  Int16              - DIR id
 *                  Int16              - rootDir id
 *                  Boolean            - responseWanted
 * Returns:         Nothing
 *
 * Description:     Sends a APEX_SIM_LOGICAL_CHANNEL_ACCESS_REQ to the ABSI task.
 *                  This signal contains a generic command for the SIM card.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSendApexSimLogicalChannelAccessReq (const VgmuxChannelNumber entity,
                                        Int16 length,
                                        Int8  sessionId,
                                        const Int8 *src_p,
                                        Int16 efId,
                                        Int16 dirId,
                                        Int16 rootDirId,
                                        /* job134856: add handling for <pathid> field */
                                        const Int8 *path_p,
                                        Int8 pathLength)
{
  SimLockContext_t *simLockContext_p = ptrToSimLockContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
  simLockContext_p->simLogicalChannelAccess.length     = length;
  simLockContext_p->simLogicalChannelAccess.sessionId  = sessionId;
  simLockContext_p->simLogicalChannelAccess.efId       = efId;
  simLockContext_p->simLogicalChannelAccess.dirId      = dirId;
  simLockContext_p->simLogicalChannelAccess.rootDirId  = rootDirId;
  simLockContext_p->simLogicalChannelAccess.commandRef = (Int16)entity;

  if (pathLength > 0)
  {
    memcpy (&simLockContext_p->simLogicalChannelAccess.path.data[0], path_p, pathLength);
  }
  simLockContext_p->simLogicalChannelAccess.path.length = pathLength;

  memcpy (&simLockContext_p->simLogicalChannelAccess.commandData[0], src_p, length);

  return (vgChManContinueAction (entity, APEX_SIM_LOGICAL_CHANNEL_ACCESS_REQ));
}

/*--------------------------------------------------------------------------
*
* Function:        vgSigApexSimApduAccessReq
*
* Parameters:      apdu - APDU content
*                  length - APDU length
*
* Returns:         Whether APDU is successfully sent to background layer
*
* Description:     Sends a SIG_APEX_SIM_RAW_APDU_ACCESS_REQ to the ABSI task.
*                  This signal contains a raw APDU command for the SIM card.
*
*-------------------------------------------------------------------------*/
Int8 vgSigApexSimApduAccessReq(Int8 *apdu, Int16 length)
{
    SignalBuffer              sigBuff = kiNullBuffer;
    ApexSimRawApduAccessReq  *apexSimRawApduAccessReq_p;

    if (length == 0)
    {
        return 1;
    }
    else if (length > SIM_MAX_MSG_SIZE)
    {
        return 2;
    }
    else
    {
        KiCreateZeroSignal(SIG_APEX_SIM_RAW_APDU_ACCESS_REQ,
                              sizeof(ApexSimRawApduAccessReq),
                           &sigBuff);

        apexSimRawApduAccessReq_p = (ApexSimRawApduAccessReq *)sigBuff.sig;

        apexSimRawApduAccessReq_p->taskId       = VG_CI_TASK_ID;
        apexSimRawApduAccessReq_p->commandRef   = VGMUX_CHANNEL_INVALID;
        if (length > SIM_MAX_MSG_SIZE)
        {
            apexSimRawApduAccessReq_p->length = SIM_MAX_MSG_SIZE;
        }
        else
        {
            apexSimRawApduAccessReq_p->length = length;
        }
        memcpy(apexSimRawApduAccessReq_p->command, apdu, apexSimRawApduAccessReq_p->length);

        KiSendSignal(TASK_BL_ID, &sigBuff);

        return 0;
    }
}

 /*--------------------------------------------------------------------------
 *
 * Function:        vgSendApexSimGenAccessReq
 *
 * Parameters:      VgmuxChannelNumber - mux channel number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SIM_GEN_ACCESS_REQ to the ABSI task.
 *                  This signal contains a generic command for the SIM card.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSimGenAccessReq (const VgmuxChannelNumber entity)
{
  SignalBuffer               sigBuff = kiNullBuffer;
  ApexSimGenAccessReq        *request_p;
  SimLockContext_t           *simLockContext_p = ptrToSimLockContext(entity);
  Int8                       pathOffset = 0;
  GeneralContext_t           *generalGenericContext_p = ptrToGeneralContext (entity);

#if defined (FEA_PHONEBOOK)
  VgLmData                   *vgLmData;
#endif /* FEA_PHONEBOOK */
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(generalGenericContext_p != PNULL, entity, 0, 0);
  FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
#if defined (FEA_PHONEBOOK)
  vgLmData = &generalGenericContext_p->vgLmData;
#endif /* FEA_PHONEBOOK */

  FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

  sendSsRegistrationSignal (SIM_LOCK,
                             entity,
                              SIG_APEX_SIM_GEN_ACCESS_CNF);

  /* Create the signal */
  KiCreateZeroSignal (SIG_APEX_SIM_GEN_ACCESS_REQ,
                       sizeof (ApexSimGenAccessReq),
                        &sigBuff);

  /* initialise the body pointer */
  request_p = (ApexSimGenAccessReq *) sigBuff.sig;

  /* Fill in signal */
  request_p->taskId      = VG_CI_TASK_ID;
  request_p->commandRef  = simLockContext_p->simGenAccess.commandRef;
  request_p->length      = simLockContext_p->simGenAccess.length;
  /* job134856: add handling for <pathid> field */
  if (simLockContext_p->simGenAccess.path.length > 0)
  {
    memcpy (request_p->path.data,
            simLockContext_p->simGenAccess.path.data,
            simLockContext_p->simGenAccess.path.length);
    pathOffset = simLockContext_p->simGenAccess.path.length;
  }
  else
  {
    if (simLockContext_p->simGenAccess.rootDirId != SIM_DIR_INVALID)
    {
      request_p->path.data[pathOffset++]  =  (Int8) ((simLockContext_p->simGenAccess.rootDirId & 0xff00) >> 8);
      request_p->path.data[pathOffset++]  =  (Int8) (simLockContext_p->simGenAccess.rootDirId & 0x00ff);
    }

    if (simLockContext_p->simGenAccess.dirId != SIM_DIR_INVALID)
    {
#if defined (FEA_PHONEBOOK)
      if (simLockContext_p->simGenAccess.dirId == SIM_DIR_DF_PHONEBOOK)
      {
        if(vgLmData->phoneBook == DIAL_LIST_ADN_GLB)
        {
            request_p->path.data[pathOffset++]  =  0x7F;
            request_p->path.data[pathOffset++]  =  0x10;
        }
        else if(vgLmData->phoneBook == DIAL_LIST_ADN_APP)
        {
            request_p->path.data[pathOffset++]  =  0x7F;
            request_p->path.data[pathOffset++]  =  0xFF;
        }
      }
#endif /* FEA_PHONEBOOK */

      request_p->path.data[pathOffset++]  =  (Int8) ((simLockContext_p->simGenAccess.dirId & 0xff00) >> 8);
      request_p->path.data[pathOffset++]  =  (Int8) (simLockContext_p->simGenAccess.dirId & 0x00ff);
    }
  }

  if (simLockContext_p->simGenAccess.efId != SIM_EF_INVALID)
  {
    request_p->path.data[pathOffset++]  =  (Int8) ((simLockContext_p->simGenAccess.efId & 0xff00) >> 8);
    request_p->path.data[pathOffset++]  =  (Int8) (simLockContext_p->simGenAccess.efId & 0x00ff);
  }

  request_p->path.length = pathOffset;

  memcpy (request_p->command,
          simLockContext_p->simGenAccess.commandData,
          simLockContext_p->simGenAccess.length);

  /* send to background layer */
  KiSendSignal (TASK_BL_ID, &sigBuff);
}


#if defined (ENABLE_DUAL_SIM_SOLUTION)
void vgSigApexSimSelectReq(const VgmuxChannelNumber entity)
{
  SignalBuffer         sigBuff = kiNullBuffer;
  ApexSimSelectReq     *request_p;
  SimLockContext_t     *simLockContext_p = ptrToSimLockContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
  FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

  sendSsRegistrationSignal (SIM_LOCK,
                             entity,
                              SIG_APEX_SIM_SELECT_CNF);

  /* Create the signal */
  KiCreateZeroSignal (SIG_APEX_SIM_SELECT_REQ,
                       sizeof (ApexSimSelectReq),
                        &sigBuff);

  /* initialise the body pointer */
  request_p = (ApexSimSelectReq *) sigBuff.sig;
  /* Fill in signal */
  request_p->taskId      = VG_CI_TASK_ID;
  request_p->commandRef  = (Int16)entity;
  request_p->simHolder  = simLockContext_p->simHolderToSelect;
  /* send to background layer */
  KiSendSignal (TASK_BL_ID, &sigBuff);
}
#endif

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexListPnnReq
 *
 * Parameters:      VgmuxChannelNumber - mux channel number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SIM_LIST_PNN_REQ to the ABSI task.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexListPnnReq(const VgmuxChannelNumber entity)
{
  SignalBuffer           sigBuff = kiNullBuffer;
  ApexSimListPnnReq      *request_p;
  SimLockContext_t       *simLockContext_p = ptrToSimLockContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
  FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

  sendSsRegistrationSignal (SIM_LOCK,
                             entity,
                              APEX_SIM_LIST_PNN_CNF);

  /* Create the signal */
  KiCreateZeroSignal (APEX_SIM_LIST_PNN_REQ,
                       sizeof (ApexSimListPnnReq),
                        &sigBuff);

  /* initialise the body pointer */
  request_p = (ApexSimListPnnReq *) sigBuff.sig;
  /* Fill in signal */
  request_p->taskId      = VG_CI_TASK_ID;
  request_p->commandRef  = (Int16)entity;
  request_p->startField  = simLockContext_p->startField;

  /* send to background layer */
  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexListOplReq
 *
 * Parameters:      VgmuxChannelNumber - mux channel number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SIM_LIST_OPL_REQ to the ABSI task.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexListOplReq(const VgmuxChannelNumber entity)
{
  SignalBuffer           sigBuff = kiNullBuffer;
  ApexSimListOplReq      *request_p;
  SimLockContext_t       *simLockContext_p = ptrToSimLockContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
  FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

  sendSsRegistrationSignal (SIM_LOCK,
                             entity,
                              APEX_SIM_LIST_OPL_CNF);

  /* Create the signal */
  KiCreateZeroSignal (APEX_SIM_LIST_OPL_REQ,
                       sizeof (ApexSimListOplReq),
                        &sigBuff);

  /* initialise the body pointer */
  request_p = (ApexSimListOplReq *) sigBuff.sig;
  /* Fill in signal */
  request_p->taskId      = VG_CI_TASK_ID;
  request_p->commandRef  = (Int16)entity;
  request_p->startField  = simLockContext_p->startField;

  /* send to background layer */
  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexReadSimParamReq
 *
 * Parameters:      VgmuxChannelNumber - mux channel number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SIM_READ_SIM_PARAM_REQ to the ABSI task.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexReadSimParamReq(const VgmuxChannelNumber entity)
{
  SignalBuffer             sigBuff = kiNullBuffer;
  ApexSimReadSimParamReq  *request_p;
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();

  FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

  sendSsRegistrationSignal (SIM_LOCK,
                             entity,
                              APEX_SIM_READ_SIM_PARAM_CNF);

  /* Create the signal */
  KiCreateZeroSignal (APEX_SIM_READ_SIM_PARAM_REQ,
                       sizeof (ApexSimReadSimParamReq),
                        &sigBuff);

  /* initialise the body pointer */
  request_p = (ApexSimReadSimParamReq *) sigBuff.sig;
  /* Fill in signal */
  request_p->taskId      = VG_CI_TASK_ID;
  request_p->commandRef  = (Int16)entity;

  switch (simLockGenericContext_p->simReadParamType)
  {
    case SIM_READ_PARAM_SST:
        request_p->simParamType = SIM_SST;
        break;
    case SIM_READ_PARAM_GID:
        request_p->simParamType = SIM_GID;
        break;
    default:
        FatalCheck(FALSE, entity, simLockGenericContext_p->simReadParamType, 0);
        break;
  }

  /* send to background layer */
  KiSendSignal (TASK_BL_ID, &sigBuff);
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexReadMwisReq
 *
 * Parameters:      VgmuxChannelNumber - mux channel number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SIM_READ_SYSTEM_MWI_REQ to the ABSI task.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexReadSystemMwiReq(const VgmuxChannelNumber entity)
{
    SignalBuffer                sigBuff = kiNullBuffer;
    ApexSimReadSystemMwiReq    *request_p;
    SimLockContext_t           *simLockContext_p = ptrToSimLockContext(entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
    FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

    sendSsRegistrationSignal(   SIM_LOCK,
                                entity,
                                SIG_APEX_SIM_READ_SYSTEM_MWI_CNF);

    /* Create the signal */
    KiCreateZeroSignal( SIG_APEX_SIM_READ_SYSTEM_MWI_REQ,
                        sizeof (ApexSimReadSystemMwiReq),
                        &sigBuff);

    /* initialise the body pointer */
    request_p = (ApexSimReadSystemMwiReq *) sigBuff.sig;
    /* Fill in signal */
    request_p->taskId      = VG_CI_TASK_ID;
    request_p->commandRef  = (Int16)entity;

    /* send to background layer */
    KiSendSignal (TASK_BL_ID, &sigBuff);
}

#if defined (SIM_EMULATION_ON)
/*--------------------------------------------------------------------------
 *
 * Function:        vgSigAlsiWriteUsimEmuFileReq
 *
 * Parameters:      VgmuxChannelNumber - mux channel number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_ALSI_WRITE_USIM_EMU_FILE_REQ to the USIM
 *                  Manager task.
 *
 *-------------------------------------------------------------------------*/
void vgSigAlsiWriteUsimEmuFileReq(const VgmuxChannelNumber entity)
{
    SignalBuffer               sigBuff = kiNullBuffer;
    AlsiWriteUsimEmuFileReq    *request_p;
    SimLockGenericContext_t    *simLockGenericContext_p = ptrToSimLockGenericContext();

    FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

    /* Register for response signal */
    sendSsRegistrationSignal(   SIM_LOCK,
                                entity,
                                SIG_ALSI_WRITE_USIM_EMU_FILE_CNF);

    /* Create the signal */
    KiCreateZeroSignal( SIG_ALSI_WRITE_USIM_EMU_FILE_REQ,
                        sizeof (AlsiWriteUsimEmuFileReq),
                        &sigBuff);

    /* initialise the body pointer */
    request_p = (AlsiWriteUsimEmuFileReq *) sigBuff.sig;
    /* Fill in signal */
    *request_p = simLockGenericContext_p->alsiWriteUsimEmuFileReq;

    /* send to USIM Manager task */
    KiSendSignal (SIM_TASK_ID, &sigBuff);

}
#endif /* SIM_EMULATION_ON */

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSimCsimLockReq
 *
 * Parameters:      VgmuxChannelNumber - mux channel number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SIM_CSIM_LOCK_REQ to the Background layer
 *                               task.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSimCsimLockReq      (const VgmuxChannelNumber entity)
{
  SignalBuffer              sigBuff = kiNullBuffer;
  ApexSimCsimLockReq        *request_p;
  SimLockContext_t          *simLockContext_p = ptrToSimLockContext(entity);

#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
  FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

  sendSsRegistrationSignal (SIM_LOCK,
                            entity,
                            SIG_APEX_SIM_CSIM_LOCK_CNF);

  /* Create the signal */
  KiCreateZeroSignal (SIG_APEX_SIM_CSIM_LOCK_REQ,
                      sizeof (ApexSimCsimLockReq),
                      &sigBuff);

  /* initialise the body pointer */
  request_p = (ApexSimCsimLockReq *) sigBuff.sig;
  /* Fill in signal */
  request_p->taskId      = VG_CI_TASK_ID;
  request_p->commandRef  = (Int16)entity;
  request_p->lock = !simLockContext_p->csimLocked;

  /* send to background layer */
  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSimOpenLogicalChannelReq
 *
 * Parameters:      VgmuxChannelNumber - mux channel number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a APEX_SIM_OPEN_LOGICAL_CHANNEL_REQ to the Background layer
 *                               task.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSimOpenLogicalChannelReq      (const VgmuxChannelNumber entity)
{
    SignalBuffer                          sigBuff = kiNullBuffer;
    ApexSimOpenLogicalChannelReq         *request_p;
    SimLockContext_t                     *simLockContext_p = ptrToSimLockContext(entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
    FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

    sendSsRegistrationSignal (SIM_LOCK,
                            entity,
                            APEX_SIM_OPEN_LOGICAL_CHANNEL_CNF);

    /* Create the signal */
    KiCreateZeroSignal (APEX_SIM_OPEN_LOGICAL_CHANNEL_REQ,
                      sizeof (ApexSimOpenLogicalChannelReq),
                      &sigBuff);

    /* initialise the body pointer */
    request_p = (ApexSimOpenLogicalChannelReq *) sigBuff.sig;
    /* Fill in signal */
    request_p->taskId      = VG_CI_TASK_ID;
    request_p->commandRef  = (Int16)entity;
    request_p->length      = simLockContext_p->vgUiccLogicChannelData.dfNameLength;

    memcpy(request_p->dfName, simLockContext_p->vgUiccLogicChannelData.dfName, request_p->length);

    /* send to background layer */
    KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSimCloseLogicalChannelReq
 *
 * Parameters:      VgmuxChannelNumber - mux channel number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a APEX_SIM_CLOSE_LOGICAL_CHANNEL_REQ to the Background layer
 *                               task.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSimCloseLogicalChannelReq      (const VgmuxChannelNumber entity)
{
    SignalBuffer                        sigBuff = kiNullBuffer;
    ApexSimCloseLogicalChannelReq      *request_p;
    SimLockContext_t                   *simLockContext_p = ptrToSimLockContext(entity);

#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
    FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

    sendSsRegistrationSignal (SIM_LOCK,
                            entity,
                            APEX_SIM_CLOSE_LOGICAL_CHANNEL_CNF);

    /* Create the signal */
    KiCreateZeroSignal (APEX_SIM_CLOSE_LOGICAL_CHANNEL_REQ,
                      sizeof (ApexSimCloseLogicalChannelReq),
                      &sigBuff);

    /* initialise the body pointer */
    request_p = (ApexSimCloseLogicalChannelReq *) sigBuff.sig;
    /* Fill in signal */
    request_p->taskId      = VG_CI_TASK_ID;
    request_p->commandRef  = (Int16)entity;
    request_p->sessionId   = simLockContext_p->vgUiccLogicChannelData.sessionId;
    /* send to background layer */
    KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSimLogicalChannelAccessReq
 *
 * Parameters:      VgmuxChannelNumber - mux channel number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a APEX_SIM_LOGICAL_CHANNEL_ACCESS_REQ to the Background layer
 *                               task.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSimLogicalChannelAccessReq(const VgmuxChannelNumber entity)
{
  SignalBuffer                  sigBuff = kiNullBuffer;
  ApexSimLogicalChannelAccessReq    *request_p;
  SimLockContext_t              *simLockContext_p = ptrToSimLockContext(entity);
  Int8                       pathOffset = 0;

#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
  FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

  sendSsRegistrationSignal (SIM_LOCK,
                            entity,
                            APEX_SIM_LOGICAL_CHANNEL_ACCESS_CNF);

  /* Create the signal */
  KiCreateZeroSignal (APEX_SIM_LOGICAL_CHANNEL_ACCESS_REQ,
                      sizeof (ApexSimLogicalChannelAccessReq),
                      &sigBuff);

  /* initialise the body pointer */
  request_p = (ApexSimLogicalChannelAccessReq *) sigBuff.sig;
  /* Fill in signal */
  request_p->taskId      = VG_CI_TASK_ID;
  request_p->commandRef  = (Int16)entity;
  request_p->sessionId = simLockContext_p->simLogicalChannelAccess.sessionId;
  request_p->length = simLockContext_p->simLogicalChannelAccess.length;

  /* job134856: add handling for <pathid> field */
  if (simLockContext_p->simLogicalChannelAccess.path.length > 0)
  {
    memcpy (request_p->path.data,
            simLockContext_p->simLogicalChannelAccess.path.data,
            simLockContext_p->simLogicalChannelAccess.path.length);
    pathOffset = simLockContext_p->simLogicalChannelAccess.path.length;
  }
  else
  {
    if (simLockContext_p->simLogicalChannelAccess.rootDirId != SIM_DIR_INVALID)
    {
      request_p->path.data[pathOffset++]  =  (Int8) ((simLockContext_p->simLogicalChannelAccess.rootDirId & 0xff00) >> 8);
      request_p->path.data[pathOffset++]  =  (Int8) (simLockContext_p->simLogicalChannelAccess.rootDirId & 0x00ff);
    }

    if (simLockContext_p->simLogicalChannelAccess.dirId != SIM_DIR_INVALID)
    {
      request_p->path.data[pathOffset++]  =  (Int8) ((simLockContext_p->simLogicalChannelAccess.dirId & 0xff00) >> 8);
      request_p->path.data[pathOffset++]  =  (Int8) (simLockContext_p->simLogicalChannelAccess.dirId & 0x00ff);
    }
  }

  if (simLockContext_p->simLogicalChannelAccess.efId != SIM_EF_INVALID)
  {
    request_p->path.data[pathOffset++]  =  (Int8) ((simLockContext_p->simLogicalChannelAccess.efId & 0xff00) >> 8);
    request_p->path.data[pathOffset++]  =  (Int8) (simLockContext_p->simLogicalChannelAccess.efId & 0x00ff);
  }

  request_p->path.length = pathOffset;

  memcpy (request_p->command,
          simLockContext_p->simLogicalChannelAccess.commandData,
          simLockContext_p->simLogicalChannelAccess.length);

  /* send to background layer */
  KiSendSignal (TASK_BL_ID, &sigBuff);
}
/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimReadMsisdnReq
 *
 * Parameters:  VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Function which sends a SIG_APEX_SIM_READ_MSISDN_REQ to the
 *              background.
 *-------------------------------------------------------------------------*/
void vgSigApexSimReadMsisdnReq(const VgmuxChannelNumber entity)
{
    SignalBuffer          sigBuff           = kiNullBuffer;
    ApexSimReadMsisdnReq    *request_p         = PNULL;
    MobilityContext_t      *mobilityContext_p = ptrToMobilityContext ();


    FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

    sendSsRegistrationSignal(   SIM_LOCK,
                                entity,
                                SIG_APEX_SIM_READ_MSISDN_CNF);

    KiCreateZeroSignal (SIG_APEX_SIM_READ_MSISDN_REQ,
                        sizeof (ApexSimReadMsisdnReq),
                        &sigBuff
                       );

    /* Initialise signal and send.... */
    request_p                       = (ApexSimReadMsisdnReq *) sigBuff.sig;
    request_p->taskId               = VG_CI_TASK_ID;
    request_p->recordNumber         = mobilityContext_p->cnumDatarecordNumber;
    request_p->commandRef           = (Int16)entity;

    KiSendSignal (TASK_BL_ID, &sigBuff);
}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

