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
 * RVOMAN.C
 * Operations manager interface controller module
 **************************************************************************/

#define MODULE_NAME "RVOMAN"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvoman.h>
#include <afnv_typ.h>
#include <rvnvram.h>
#include <rvdata.h>
#include <vgmx_sig.h>
#include <rvcimux.h>
#include <rvcimxut.h>
#include <rvomut.h>
#include <rvomtime.h>
#include <ki_sig.h>
#if defined (DEVELOPMENT_VERSION)
#  include <rvtsut.h>
#endif

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  KiTimerExpiry          kiTimerExpiry;
};


/***************************************************************************
 * Types
 ***************************************************************************/

/***************************************************************************
 * Variables
 ***************************************************************************/

/***************************************************************************
 * Macros
 ***************************************************************************/

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static void  initialiseOpManSsGenericData (void);

/***************************************************************************
 * Local Functions
 ***************************************************************************/

 /***************************************************************************
 * Static Functions
 ***************************************************************************/

 /*--------------------------------------------------------------------------
 *
 * Function:    initialiseOpManSsGenericData
 *
 * Parameters:  none
 *
 * Returns:     nothing
 *
 * Description: Initialise all the Operations manager generic control data.
 *
 *-------------------------------------------------------------------------*/

static void  initialiseOpManSsGenericData (void)
{
  OpmanGenericContext_t*  opManGenericContext_p = ptrToOpManGenericContext ();
  Int8                    connectionIndex;
  Int8                    eIndex;

  for (connectionIndex = 0;
        connectionIndex < CONNECTION_TERMINATOR;
         connectionIndex++ )
  {
    opManGenericContext_p->peripheralControlBuffer [connectionIndex] = FALSE;
  }

  for (eIndex = 0;
        eIndex < CI_MAX_ENTITIES;
         eIndex++)
  {
    opManGenericContext_p->channelNeedsEnabling [eIndex] = FALSE;
  }

  for (eIndex = 0;
        eIndex < CI_MAX_ENTITIES;
         eIndex++)
  {
    opManGenericContext_p->channelState [eIndex].dcd         = 0;
    opManGenericContext_p->channelState [eIndex].ri          = 0;
    opManGenericContext_p->channelState [eIndex].aBreak      = FALSE;
    opManGenericContext_p->channelState [eIndex].breakLength = 0;
    opManGenericContext_p->channelState [eIndex].assigned    = FALSE;
    opManGenericContext_p->channelState [eIndex].isDisconnecting = FALSE;
  }

  timerInitialise ();

  /*
   * We need to keep track of the number of enabled channels as it cannot exceed
   * MAX_NUM_AT_CHANNELS_TO_ALLOC.
   */
  opManGenericContext_p->numberOfEnabledChannels = 0;
}

 /*--------------------------------------------------------------------------
 *
 * Function:    vgOpManInitialise
 *
 * Parameters:  entity - channel number to initialise
 *
 * Returns:     nothing
 *
 * Description: Initialise all the Operations manager control data for channel.
 *
 *-------------------------------------------------------------------------*/

void  vgOpManInitialise (const VgmuxChannelNumber entity, Boolean clearCidOwner)
{

  OpmanContext_t  *opManContext_p;

  /* initialise general context data for all entities */
  opManContext_p = ptrToOpManContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(opManContext_p != PNULL, entity, 0, 0);
#endif
  opManContext_p->currentCcontrolState = ALL_IDLE;
  opManContext_p->numberOfCallConnections = 0;
  opManContext_p->vgLastCallConnectionType = CONNECTION_TERMINATOR;
  vgOpManResetCallInfo (&opManContext_p->callInfo);

  if (clearCidOwner)
  {
    /* Make sure all CIDs which may have been associated with this channel are un-associated */
    vgOpManMakeAllCidsAvailable(entity);
  }
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/


 /*--------------------------------------------------------------------------
 *
 * Function:    vgOpManStkOnLine
 *
 * Parameters:  entity  - mux channel number
 *
 * Returns:     Boolean - whether channel is doing an STK session
 *
 * Description: Determines whether channel should allow STK interaction
 *
 *-------------------------------------------------------------------------*/

Boolean vgOpManStkOnLine (const VgmuxChannelNumber entity)
{
   StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

   return (Boolean)(stkGenericContext_p->registeredForStk [entity] == TRUE);
}

 /*--------------------------------------------------------------------------
 *
 * Function:    vgOpManCheckStkRegistration
 *
 * Parameters:  entity  - mux channel number
 *
 * Returns:     Boolean - whether channel is STK registered
 *
 * Description: Given an entity, the one that has supplied the *STPD command we
 *              check to see if we can allow the request.  If the entity is
 *              already registered then we always allow it becuase it may be
 *              trying to de-register.  However, if it is not registered then
 *              we need to make sure that another entity is not already
 *              registered.
 *
 *-------------------------------------------------------------------------*/

Boolean vgOpManCheckStkRegistration (const VgmuxChannelNumber entity)
{
  Boolean  okToRegister = TRUE;
  Int8     eIndex;

  for (eIndex = 0; (eIndex < CI_MAX_ENTITIES && okToRegister == TRUE); eIndex++)
  {
    /* forget about the entity that is trying to de/register and check the others */
    if ( ( entity != eIndex ) && ( vgOpManStkOnLine ((VgmuxChannelNumber)eIndex) == TRUE) )
    {
      okToRegister = FALSE;
    }
  }

  return (okToRegister);
}



/*--------------------------------------------------------------------------
 *
 * Function:    vgOpManCidAvailable
 *
 * Parameters:  cid    - context id
 *              entity - mux channel number
 *
 * Returns:     Boolean - whether channel is doing an STK session
 *
 * Description: Checks whether the requesting entity is allowed to modify
 *              the parameters for the given cid.
 *
 *-------------------------------------------------------------------------*/

Boolean vgOpManCidAvailable (Int8 cid,
                             const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo       *cidData_p = gprsGenericContext_p->cidUserData[cid];
  Boolean               result = FALSE;

  if (cidData_p->profileDefined == FALSE)
  {
    result = TRUE;
  }
  else
  {
    if ((gprsGenericContext_p->cidOwner[cid] == entity) ||
        (gprsGenericContext_p->cidOwner[cid] == VGMUX_CHANNEL_INVALID))
    {
      result = TRUE;
    }
  }
  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgOpManMakeCidAvailable
 *
 * Parameters:  cid - CID to free up
 *              entity - mux channel number
 *
 * Returns:     Nothing
 *
 * Description: Allow other entities to modify the parameters for a cid that
 *              was assigned to this channel.  This is when, for example the
 *              PDP context has been terminated or when there was an error
 *              (e.g. APN incorrect) on PDP context activation.
 *
 *-------------------------------------------------------------------------*/
void    vgOpManMakeCidAvailable     (Int8 cid,
                                     const VgmuxChannelNumber entity)
{ 
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext();

  /* Make sure the entity and cid match properly  */
  if (gprsGenericContext_p->cidOwner[cid] == entity)
  {
    gprsGenericContext_p->cidOwner[cid] = VGMUX_CHANNEL_INVALID;
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgOpManMakeAllCidsAvailable
 *
 * Parameters:  
 *              entity - mux channel number
 *
 * Returns:     Nothing
 *
 * Description: Allow other entities to modify the parameters for any cids that
 *              were assigned to this channel.
 *
 *-------------------------------------------------------------------------*/
void    vgOpManMakeAllCidsAvailable     (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext();
  Int8                 cid;
    
  for (cid = 0; cid < MAX_NUMBER_OF_CIDS; cid++)
  {
    if (gprsGenericContext_p->cidOwner[cid] == entity)
    {
      gprsGenericContext_p->cidOwner[cid] = VGMUX_CHANNEL_INVALID;
    }
  }
}

 /*--------------------------------------------------------------------------
 *
 * Function:    vgOpManCidDefined
 *
 * Parameters:  cid    - context id to be checked
 *
 * Returns:     Boolean - whether cid is defined
 *
 * Description: Checks whether the cid has been defined.  This is used by
 *              AT+CGQREQ and AT+CGQMIN to check whether the profile has
 *              been created by AT+CGDCONT.
 *
 *-------------------------------------------------------------------------*/

Boolean vgOpManCidDefined (Int8 cid)
{
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo      *cidData_p = gprsGenericContext_p->cidUserData[cid];
  Boolean retVal = FALSE;

  /* cid can only be defined by cgdcont/cgdscont commands */
  if(cidData_p != PNULL)
  {
    if (cidData_p->profileDefined &&
        (cidData_p->cgdcontDefined
         || cidData_p->cgdscontDefined
         ))
    {
      retVal = TRUE;
    }
  }

  return (retVal);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgOpManCidActive
 *
 * Parameters:  cid    - context id to be checked
 *
 * Returns:     Boolean - whether cid is active
 *
 * Description: Checks whether a cid is active
 *
 *-------------------------------------------------------------------------*/

Boolean vgOpManCidActive (Int8 cid)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext();
  Boolean               rsp = FALSE;

  if ((gprsGenericContext_p->cidUserData[cid] != PNULL) &&
      (gprsGenericContext_p->cidUserData[cid]->isActive == TRUE))
  {
    rsp = TRUE;
  }
   
  return (rsp);
}

 /*--------------------------------------------------------------------------
 *
 * Function:    vgOpManAnyCallsPresent
 *
 * Parameters:  nothing
 *
 * Returns:     Boolean - connection allowed
 *
 * Description: Attempts to allocates a connection
 *
 *-------------------------------------------------------------------------*/

Boolean vgOpManAnyCallsPresent ( void )
{
  OpmanContext_t *opManContext_p;
  Boolean callsFound = FALSE;
  Int8    eIndex;

  /* check active entities for incompatible activities */
  for (eIndex = 0;
      (eIndex < CI_MAX_ENTITIES) && (callsFound == FALSE);
       eIndex++)
  {
    if (isEntityActive ((VgmuxChannelNumber)eIndex) == TRUE)
    {
      /* check for call connections */
      opManContext_p = ptrToOpManContext ((VgmuxChannelNumber)eIndex);
#if defined (ATCI_SLIM_DISABLE)

      FatalCheck(opManContext_p != PNULL, eIndex, 0, 0);
#endif
      if (opManContext_p->numberOfCallConnections > 0)
      {
        callsFound = TRUE;
      }
    }
  }

  return (callsFound);
}

 /*--------------------------------------------------------------------------
 *
 * Function:    vgOpManAllocateConnection
 *
 * Parameters:  entity         - mux channel number
 *              thisConnection - connection type to allocate
 *
 * Returns:     Boolean - connection allowed
 *
 * Description: Attempts to allocates a connection
 *
 *-------------------------------------------------------------------------*/

Boolean vgOpManAllocateConnection (const VgmuxChannelNumber entity,
                                   const ConnectionClass_t thisConnection)
{
  return (vgOpManUtilsEnableDisableConnection (entity,
                                                thisConnection,
                                                 TRUE));
}

 /*--------------------------------------------------------------------------
 *
 * Function:    vgOpManDropConnection
 *
 * Parameters:  entity         - mux channel number
 *              thisConnection - connection type to drop
 *
 * Returns:     Boolean - connection allowed
 *
 * Description: Attempts to de-allocates a connection
 *
 *-------------------------------------------------------------------------*/

Boolean vgOpManDropConnection (const VgmuxChannelNumber entity,
                                const ConnectionClass_t thisConnection)
{
  return (vgOpManUtilsEnableDisableConnection (entity,
                                                thisConnection,
                                                 FALSE));
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgOpManCheckSignalDirection
 *
 * Parameters:  signalId - signal id of signal to check
 *              entity   - channel number
 *
 * Returns:     nothing
 *
 * Description: determines of incoming unregistered indication signal
 *              is from a call control element - it then returns
 *              the current entity which has control
 *-------------------------------------------------------------------------*/

Boolean vgOpManCheckSignalDirection (const SignalId signalId,
                                      VgmuxChannelNumber *entity)
{
  Boolean               matchFound = FALSE;
  SignalInterfaceBases  signalBase;

  /* get signal base */
  signalBase = (SignalInterfaceBases)KI_SIGNAL_BASE_FROM_SIGNALID (signalId);

  /* find matching change control element */
  switch (signalBase)
  {
    case VGEX_SIGNAL_BASE:
    {
      matchFound = (findFirstEnabledChannel() != VGMUX_CHANNEL_INVALID);
      break;
    }
    default:
    {
      /* signal not from task with access controlled by operations manager*/
      *entity = VGMUX_CHANNEL_INVALID;
      break;
    }
  }

  return (matchFound);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgOpManGetConnectionInfo
 *
 * Parameters:  entity     - mux channel number
 *              userCallId - call identifier
 *              callInfo   - call information
 *
 * Returns:     Boolean - whether connection not found
 *
 * Description: Gets information on requested connection
 *-------------------------------------------------------------------------*/

Boolean vgOpManGetConnectionInfo (const VgmuxChannelNumber entity,
                                   const UserCallId userCallId,
                                    ConnectionInformation_t **callInfo)
{
  OpmanContext_t  *opManContext_p;
  Boolean         found = FALSE;

  if (isEntityActive(entity) == TRUE)
  {
    opManContext_p = ptrToOpManContext(entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(opManContext_p != PNULL, entity, 0, 0);
#endif
    if (opManContext_p->numberOfCallConnections > 0)
    {
      /* check for PSD matches */
      if ((opManContext_p->callInfo.vgIdent == userCallId) &&
          (opManContext_p->callInfo.vgIdent >= MIN_PSD_USER_CALL_ID) &&
          (opManContext_p->callInfo.vgIdent <= MAX_PSD_USER_CALL_ID) &&
          (
#if defined (FEA_PPP)
           (opManContext_p->callInfo.vgClass == PPP_CONNECTION) ||
#endif /* FEA_PPP */           
           (opManContext_p->callInfo.vgClass == PT_CONNECTION)))
      {
        /* we have found the slot so set the found flag */
        found = TRUE;
        *callInfo = &opManContext_p->callInfo;
      }
    }

    /* if the call has not been registered then we find a free slot */
    if ((found == FALSE) && (opManContext_p->numberOfCallConnections > 0))
    {
      /* we have not been able to find a record of the user id for the call,
       * find first free PSD slot */

      /* find entry with no call id */
      if (opManContext_p->callInfo.vgIdent == NEVER_USED_USER_CALL_ID)
      {
        if (
#if defined (FEA_PPP)          
            (opManContext_p->callInfo.vgClass == PPP_CONNECTION) ||
#endif /* FEA_PPP */           
            (opManContext_p->callInfo.vgClass == PT_CONNECTION))

        {
          opManContext_p->callInfo.vgIdent = userCallId;
          *callInfo = &opManContext_p->callInfo;
          found = TRUE;
        }
      }
    }
  }

  return (found);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgOpManSetConnectionStatus
 *
 * Parameters:  entity          - mux channel number
 *              userCallId      - call identifier
 *              connectionState - new connection state
 *
 * Returns:     Boolean - whether connection not found
 *
 * Description: Sets the connection status of a PSD connection
 *-------------------------------------------------------------------------*/

Boolean vgOpManSetConnectionStatus (const VgmuxChannelNumber entity,
                                     const UserCallId         userCallId,
                                      const ConnectionState_t  connectionState)
{
  Boolean                  found;
  ConnectionInformation_t  *callInfo;

  /* get connection information */

  found = vgOpManGetConnectionInfo (entity, userCallId, &callInfo);

  /* if found then set the connection state as requested */

  if (found == TRUE)
  {
    callInfo->vgState = connectionState;
  }

  return (found);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgOpmanInterfaceController
 *
 * Parameters:  signal_p - structure containing incoming signal
 *              entity   - mux channel number
 *
 * Returns:     Boolean  - indicates whether the sub-system has recognised and
 *                         procssed the signal given.
 *
 * Description: determines action for received signals
 *-------------------------------------------------------------------------*/

Boolean vgOpmanInterfaceController (const SignalBuffer *signal_p,
                                     const VgmuxChannelNumber entity)
{
  Boolean accepted = FALSE;

  PARAMETER_NOT_USED(entity);

  switch (*signal_p->type)
  {
    case SIG_INITIALISE:
    {
      initialiseOpManSsGenericData ();
      break;
    }
    case SIG_TIMER_EXPIRY:
    {
      vgCiProcessTimerExpiry (signal_p->sig->kiTimerExpiry.userValue,
                               signal_p->sig->kiTimerExpiry.timerId);
      accepted = TRUE;
      break;
    }
    case SIG_CIMUX_CHANNEL_ENABLE_IND:
    {
      vgOpManInitialise (entity, TRUE);
      break;
    }
    default:
    {
      break;
    }
  }
  return (accepted);
}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

