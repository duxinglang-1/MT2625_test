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
 * rvccsigi.c
 * Incoming signal converter for the Call Control Sub-System.
 **************************************************************************/

#define MODULE_NAME "RVCCSIGI"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <gkisig.h>
#include <rvsystem.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvccsigi.h>
#include <rvccut.h>
#include <rvpdsigo.h>
#include <rvpdsigi.h>
#include <rvgput.h>
#include <rvgnsigi.h>
#include <rvchman.h>
#include <rvoman.h>
#include <rvomtime.h>
#include <rvcimxsot.h>
#include <rvcrhand.h>
#include <rvccut.h>
#include <rvcimxut.h>
#include <rvmmut.h>
#include <rvprof.h>
#include <rvcmux.h>
#include <rvgnut.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  CiMuxAtDataInd                ciMuxAtDataInd;
};


/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static Boolean vgActivePsdConnection (const VgmuxChannelNumber entity);
 
/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:    vgActivePsdConnection
 *
 * Parameters:  entity - channel to check for PSD connection
 *
 * Returns:     TRUE if at least one PSD connection, FALSE otherwise.
 *
 * Description: Checks if there is any active PSD connections on an entity.
 *              This is not checked for contexts
 *              activated with AT+CGACT (i.e. with no data connection).
 *-------------------------------------------------------------------------*/
static Boolean vgActivePsdConnection (const VgmuxChannelNumber entity)
{
  OpmanContext_t          *opManContext_p     = ptrToOpManContext (entity);
  Boolean                 activeConnection = FALSE;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (opManContext_p != PNULL, entity, 0, 0);
#endif
  /* check for PSD session on this channel */
  if (opManContext_p->numberOfCallConnections > 0)
  {
    if (
#if defined (FEA_PPP)      
        (opManContext_p->callInfo.vgClass == PPP_CONNECTION) ||
#endif /* FEA_PPP */
        (opManContext_p->callInfo.vgClass == PT_CONNECTION))
    {
      switch (opManContext_p->callInfo.vgState)
      {
        case CONNECTION_ON_LINE:
        {
          activeConnection = TRUE;
          break;
        }
        default:
        {
          /* not online */
          break;
        }
      }
    }
  }

  return (activeConnection);
}

/*--------------------------------------------------------------------------
 * Function:    vgContinueHangupRequest
 *
 * Parameters:  entity         - channel on which call is being made
 *              activeCallList - current call information
 *
 * Returns:     nothing
 *
 * Description: Performs disconnections required by the ATH (hangup) command.
 *              For PSD connections this is only
 *              applicable to PPP and Packet Transport connection types.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgContinueHangupRequest (const VgmuxChannelNumber entity)
{

  OpmanContext_t            *opManContext_p          = ptrToOpManContext (entity);

  CallContext_t             *callContext_p           = ptrToCallContext (entity);


  Boolean                   psdCallsToBeDisconnected = FALSE;
  ResultCode_t              result                   = RESULT_CODE_PROCEEDING;

  GprsContext_t         *gprsContext_p        = ptrToGprsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(gprsContext_p != PNULL, entity, 0, 0);

  FatalCheck(callContext_p != PNULL, entity, 0, 0);

  FatalCheck(opManContext_p != PNULL, entity, 0, 0);
#endif
  if (ATH_PARAM_DISCONNECT_MO == callContext_p->athParam)
  {
    setResultCode (entity, RESULT_CODE_OK);
    result = RESULT_CODE_OK;
  }
  else
  {

    /* search through the Active Call List and determine ownership of each call
     * and whether it needs to be terminated */
    psdCallsToBeDisconnected = vgActivePsdConnection (entity);

    /* check for PSD session on this channel */
    if (opManContext_p->numberOfCallConnections > 0)
    {

      if (
#if defined (FEA_PPP)        
          (opManContext_p->callInfo.vgClass == PPP_CONNECTION) ||
#endif /* FEA_PPP */          
          (opManContext_p->callInfo.vgClass == PT_CONNECTION))
      {
        switch (opManContext_p->callInfo.vgState)
        {
          case CONNECTION_ON_LINE:
            gprsContext_p->disconnectionItem = opManContext_p->callInfo.vgIdent;
            gprsContext_p->vgHangupType = VG_HANGUP_TYPE_ATH;
            gprsContext_p->vgHangupCid  = vgFindCidWithDataConnLinkedToEntity(entity);
            result = vgChManContinueAction (entity, SIG_APEX_ABPD_HANGUP_REQ);
            setResultCode (entity, result);
            psdCallsToBeDisconnected = TRUE;
            break;

          /* job128946: prevent premature disabling of channel used for PSD connection */
          case CONNECTION_DISCONNECTING:
            if (gprsContext_p->vgHangupType == VG_HANGUP_TYPE_DTR_DROPPED)
            {
              /* ATH has occurred while disconnecting after DTR low; delay the result */
              /* code until CiPppNoCarrierReq received to prevent disabling of entity */
              setResultCode (entity, RESULT_CODE_PROCEEDING);
              psdCallsToBeDisconnected = TRUE;
            }
            break;

          default:
            /* not online */
            break;
        }
      }
    }

    /* no calls to hang up, just return OK */
    if (psdCallsToBeDisconnected == FALSE)
    {
      setResultCode (entity, RESULT_CODE_OK);
      result = RESULT_CODE_OK;
    }
  }
  return (result);
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/


/*--------------------------------------------------------------------------
 *
 * Function:    vgSetApexGsmCallReleaseError
 *
 * Parameters:  apexErrorCode - the APEX call error code
 *              gsmCausePresent - if the GSM call error code is present
 *              gsmErrorCode  - the GSM call error code
 *              entity - channel on which call was lost
 *
 * Returns:     Nothing
 *
 * Description: Sets the current call release error to errorCode and the
 *              error type to apex.  In addition, set the GSM cause for when
 *              AT*MCEERMODE=1 (numeric mode).
 *-------------------------------------------------------------------------*/

void vgSetApexGsmCallReleaseError (const ApexCauseType      apexErrorCode,
                                   const Boolean            gsmCausePresent,
                                   const GsmCause           gsmErrorCode,
                                   const ProtocolDiscriminator pd,
                                   const VgmuxChannelNumber entity)
{
  CallContext_t *callContext_p = ptrToCallContext (entity);

  FatalCheck (callContext_p != PNULL, entity, 0, 0);

  /* sets the call release error code and error code type */
  callContext_p->vgErrorType = CI_CALL_RELEASE_ERROR_APEX;
  callContext_p->vgApexCallReleaseError = apexErrorCode;
  if (gsmCausePresent == TRUE)
  {
    if ((pd == PD_CC) || (pd == PD_SM))
    {
      callContext_p->vgGsmCauseCallReleaseError = gsmErrorCode;
    }
    else if ((pd == PD_MM) || (pd == PD_GMM))
    {
      callContext_p->vgGsmCauseCallReleaseError = (GsmCause)CAUSE_AT_CALL_IMPOSSIBLE;
    }
    else
    {
      callContext_p->vgGsmCauseCallReleaseError = (GsmCause)CAUSE_AT_CALL_LOWER_FAIL;
    }
  }
  else
  {
    /* We didn't get a GSM cause so we had better just set it to
     * NORMAL_UNSPECIFIED.
     */
    callContext_p->vgGsmCauseCallReleaseError = CAUSE_NORMAL_UNSPECIFIED;

  }
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSetTimerCallReleaseError
 *
 * Parameters:  errorCode - the timer call error code
 *              entity - channel on which call was lost
 *
 * Returns:     Nothing
 *
 * Description: Sets the current call release error to errorCode and the
 *              error type to timer
 *-------------------------------------------------------------------------*/

void vgSetTimerCallReleaseError (const CiTimerError_t errorCode,
                                  const VgmuxChannelNumber entity)
{
  CallContext_t *callContext_p = ptrToCallContext (entity);

  FatalCheck (callContext_p != PNULL, entity, 0, 0);

  /* sets the call release error code and error code type */
  callContext_p->vgErrorType = CI_CALL_RELEASE_ERROR_TIMER;
  callContext_p->vgTimerCallReleaseError = errorCode;
}

#if defined (FEA_MT_PDN_ACT)
/*--------------------------------------------------------------------------
 *
 * Function:    vgStartRinging
 *
 * Parameters:  entity        - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: starts ring timer and associated events
 *-------------------------------------------------------------------------*/

void vgStartRinging (const VgmuxChannelNumber entity)
{
  VgmuxChannelNumber channel_index;

    /* reset s-register ring counter */
  for(channel_index=0; channel_index < CI_MAX_ENTITIES; channel_index++)
  {
     if (isEntityActive (channel_index))
     {
         setProfileValue (channel_index, PROF_S1, 0);
     }
  }

  vgSendCiMuxSetRing (entity, TRUE);

  /* display first ring indcation */
  vgRINGINGTimerExpiry (entity);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgDisplayRING
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     -
 *
 * Description: Display RING on the AT interface.
 *
 *-------------------------------------------------------------------------*/

void vgDisplayRING (const VgmuxChannelNumber entity,
                    const char *serviceTypeStr_p,
                    const char *extServiceTypStr_p)
{
  VgmuxChannelNumber profileEntity = entity;

/* display ring indication */
  for(profileEntity=0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
  {
    if(!isEntityMmiNotUnsolicited(profileEntity))
    {
      if (isEntityActive (profileEntity))
      {
        /* update S1 profile value */
        setProfileValue (profileEntity,
                         PROF_S1,
                         (Int8) (getProfileValue (profileEntity, PROF_S1) + 1));

        /*
         * If entity is either IDLE or running, then send out the unsolicited event.  However, if it is set to ENTITY_RUNNING
         * then the call cannot be answered until the previous AT command has completed.
         */
        if ((getEntityState (profileEntity) == ENTITY_IDLE) || (getEntityState (profileEntity) == ENTITY_RUNNING))
        {
          sendUnsolicitedResultCodeToCrm (profileEntity, RESULT_CODE_RING);
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCcAutoAnswer
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     whether we should auto-answer cs calls
 *
 * Description: returns the same
 *
 *-------------------------------------------------------------------------*/

Boolean vgCcAutoAnswer (const VgmuxChannelNumber entity, VgmuxChannelNumber* runEntity)
{
  Boolean autoAnswer = FALSE;
  VgmuxChannelNumber profileEntity = 0;

  while ((!autoAnswer) && (profileEntity < CI_MAX_ENTITIES))
  {
    if (isEntityActive (profileEntity))
    {
      /* Notice the unpleasant interaction with gprs which the spec 27.007
       requires: if CGAUTO=2 then cs calls cannot be answered (manually
       or automatically) */
      autoAnswer =
          getProfileValue (profileEntity, PROF_CGAUTO) != CGAUTO_MODE_MODEM_COMPATIBILITY_PS_ONLY &&
         (getProfileValue (profileEntity, PROF_S0) > 0) &&
         (getProfileValue (profileEntity, PROF_S1) >= getProfileValue (profileEntity, PROF_S0));

    }

    if (autoAnswer)
    {
      *runEntity = profileEntity;
    }
    profileEntity++;
  }

  return (autoAnswer);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgGetRunEntity
 *
 * Parameters:  sig -- signal
 *
 * Returns: get the entity which most recently handled the given
 * signal, or failing that the first enabled
 *
 * Description: returns the same
 *
 *-------------------------------------------------------------------------*/

VgmuxChannelNumber vgGetRunEntity (SignalId sig)
{
  VgmuxChannelNumber runEntity = VGMUX_CHANNEL_INVALID;
  Boolean            getRunEntity;

  /* see which entity had control last */
  getRunEntity = vgChManCheckSignalDirection (sig, &runEntity);
  /* if a suitable channel is not found then get the first enabled
   * channel to answer */
  if (!getRunEntity)
  {
    runEntity = findFirstEnabledChannel ();
  }
  return (runEntity);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCcRINGINGTimerExpiry
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     nothing
 *
 * Description: displays the RING unsolicited event notification
 *
 *-------------------------------------------------------------------------*/

static void vgCcRINGINGTimerExpiry (const VgmuxChannelNumber entity)
{
  VgmuxChannelNumber      runEntity = VGMUX_CHANNEL_INVALID;
  CallContext_t           *callContext_p = ptrToCallContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (callContext_p != PNULL, entity, 0, 0);
#endif
  /* the check that there is still an incoming call has already been
     done */
  /* Send both normal and extended string to vgDisplayRing - then that function
   * decides which string to send to which channel.
   */
  vgDisplayRING (entity,"","");

  /* check if auto-answer is on and the required number of rings has been
   * reached */
  if (vgCcAutoAnswer (entity,&runEntity))
  {
    /* Check validity of channel returned.... */
    if (runEntity == VGMUX_CHANNEL_INVALID)
    {
      setResultCode (entity, RESULT_CODE_OK);
    }
    else
    {
      /* run the ATA command on the appropriate channel */
      vgCiRunAtCommandInd (runEntity, (Char *)"A");
    }
  }

}

/*--------------------------------------------------------------------------
 *
 * Function:    vgRINGINGTimerExpiry
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     nothing
 *
 * Description: displays the RING unsolicited event notification
 *
 *-------------------------------------------------------------------------*/

void vgRINGINGTimerExpiry (const VgmuxChannelNumber entity)
{
  Boolean somethingIncoming = TRUE;

  if (vgPdIncomingPdpContextActivation (entity))
  {
    vgPdRINGINGTimerExpiry (entity);
  }
  else
  {
    somethingIncoming = FALSE;
  }

  if (somethingIncoming)
  {
    /* start RING timer again */
    vgCiStartTimer (TIMER_TYPE_RINGING, entity);
  }
}
#endif /* FEA_MT_PDN_ACT */

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

