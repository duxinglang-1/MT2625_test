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
 * The CI Task.  All signals are received here and sent to the appropriate
 * managers and sub-systems.
 * Two classes of signals are received:
 * (1) Solicited, these contain the entity id.
 * (2) Unsolicited, these may be accompanied by a registration signal.  If not
 * Raven makes an attempt to send it to the correct sub-system using the
 * signal's base.  If all fails its sent to all sub-systems and all entities.
 *
 * The task receives a signal of the input queue and checks for high priority
 * signals.  An example of this is the XON/XOFF signals.  All signals are
 * then read off the queue and either processed or popped onto the internal
 * queue.  The final stage is to process all the signals on the internal queue.
 **************************************************************************/

#define MODULE_NAME "RVMAIN"

/***************************************************************************
 * Include Files
 ***************************************************************************/
#include <nbiot_modem_common_config.h>

#include <system.h>
#include <ttconfig.h>
#include <kernel.h>
#include <rvsystem.h>
#include <rvcrman.h>
#include <rvoman.h>
#include <rvutil.h>
#include <rvpfss.h>
#include <rvgnss.h>
#include <rvcimux.h>
#include <cimux_sig.h>

#if defined (FEA_PPP)
#include <dspppsig.h>
#endif /* FEA_PPP */
#if defined(MTK_NBIOT_TARGET_BUILD)&& defined(HAL_WDT_MODULE_ENABLED)
#include <hal_wdt.h>
#endif
#include <rvstk.h>
#include <rvgpss.h>
#include <rvmmss.h>
#if defined (ENABLE_AT_ENG_MODE)
#include <rvemss.h>
#endif
#include <rvctss.h>
#include <rvtsss.h>
#include <rvtsut.h>
#include <rvccss.h>
#include <rvchman.h>
#include <rvtssigo.h>
#include <rvnvram.h>
#include <rvssss.h>
#include <rvmsss.h>
#include <rvslss.h>
#include <rvapss.h>
#include <rvcimxut.h>

#if defined (USE_L4MM_ALLOC_MEMORY)
#include <l4mm_api.h>
#else /* USE_L4MM_ALLOC_MEMORY */
#if defined (USE_BMM_ALLOC_MEMORY)
#include <bmm.h>
#else
#include <tmm.h>
#endif
#endif /* USE_L4MM_ALLOC_MEMORY */

#include <rvslut.h>
#include <rvcrhand.h>
#include <rvomtime.h>

#include <rvcmux.h>
#include <rvsleep.h>

#if defined (UPGRADE_SHARE_MEMORY)
#include <r2_hal.h>
#include <t1muxshmdrv.h>
#endif /* UPGRADE_SHARE_MEMORY */
#include <frhsl.h>
#if defined (UPGRADE_SHMCL_SOLUTION)
#include <muxconn_at.h>      /* For at command data */
#endif /* UPGRADE_SHMCL_SOLUTION  */

#ifdef ENABLE_AP_BRIDGE_FEATURE
#include <ciapb_sig.h>
#endif

#include <psc_api.h>
/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/***************************************************************************
 * Signal definitions
 ***************************************************************************/
union Signal
{
  CiRunAtCommandInd          ciRunAtCommandInd;
  VgCiSsRegistrationInd      vgCiSsRegistrationInd;
  CiEmptyTxCacheInd          ciEmptyTxCacheInd;
  CirmDataInd                cirmDataInd;
  CiUserProfLoadedInd        ciUserProfLoadedInd;
  CiMuxChannelEnableInd      ciMuxChannelEnableInd;
  CiMuxAtDataInd             ciMuxAtDataInd;
  CiMuxAtDataCnf             ciMuxAtDataCnf;
  CiMuxChannelDisabledInd    ciMuxChannelDisabledInd;
  CiMuxConfigureMuxCnf       ciMuxConfigureMuxCnf;
  CiMuxClosedDataConnInd 	ciMuxClosedDataConnInd;
  CiMuxSwitchedToCmdModeInd  ciMuxSwitchedToCmdModeInd;
  CiMuxCheckCmuxCmdParamsCnf ciMuxCheckCmuxCmdParamsCnf;
  CiMuxReadCmuxCmdParamsCnf  ciMuxReadCmuxCmdParamsCnf;
  CiMuxOpenDataConnCnf       ciMuxOpenDataConnCnf;
  CiMuxVersionCnf            ciMuxVersionCnf;
  KiTimerExpiry              kiTimerExpiry;
  Anrm2ReadDataCnf           anrm2ReadDataCnf;
  
#if defined (FEA_PPP)
  DsPppConfigReq             dsPppConfigReq;
#endif /* FEA_PPP */

#if defined (ENABLE_L23_DEBUG)
  CiDebugInd                 ciDebugInd;
#endif
#ifdef ENABLE_AP_BRIDGE_FEATURE
  CiApbAtResponseInd         ciApbAtResponseInd;
  CiApbRegisterAtCommandInd  ciApbRegisterAtCommandInd;
#endif
};

/***************************************************************************
 * Types
 ***************************************************************************/

/***************************************************************************
 * Macros
***************************************************************************/

/***************************************************************************
 * Global Variables
***************************************************************************/
#if defined (ENABLE_ATCI_UNIT_TEST)
extern Boolean rebootAtci;
#endif

static KiUnitQueue  ciHPInternalQueue;
static KiUnitQueue  ciMPInternalQueue;
static KiUnitQueue  ciLPInternalQueue;
static KiUnitQueue  ciDPInternalQueue;
static KiUnitQueue  ciSMInternalQueue;


/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/
static Boolean sendToSubSystem (SignalBuffer  *signal_p,
                                 const VoyagerSubsystems_t  ss,
                                  const VgmuxChannelNumber  entity);
static Boolean isCommonUnsolicitedSignal (const SignalBuffer  *signal_p);
static Boolean signalEntityCheck (const VgmuxChannelNumber  entity,
                                   const SignalBuffer  *signal_p);
static void waitForTaskInitialise (void);
static void pushSignalOnInternalQueue (SignalBuffer  *receivedSignal);
static void knownEntityRouter (SignalBuffer  *signal_p);
Boolean isEsosend(const VgmuxChannelNumber entity);
extern  void ackDataInd (const VgmuxChannelNumber entity);
extern void InitialiseSleepManContextData(ControlResetCause powerOnCause);

/***************************************************************************
 * Local Functions
 ***************************************************************************/

 /****************************************************************************
 *
 * Function:    isEsosend
 *
 * Parameters:  SignalBuffer *signal_p
 *
 * Returns:     Nothing
 *
 * Description: This function is used to decide whether we should allow a
 * signal to be be processed.  Some signals are used when an entity is
 * is enabled in which case we identify these and set the allowed flag
 * to true.  all the others will only be processed when the entity is active.
 *
 ****************************************************************************/

Boolean isEsosend(const VgmuxChannelNumber entity)
{

    ScanParseContext_t      *scanParseContext_p = ptrToScanParseContext (entity);
    CommandLine_t      *commandBuffer_p;      
    char tempString[9];    
    tempString[8]='\0';
    char ESOSEND[]="+ESOSEND";
    
    commandBuffer_p = &scanParseContext_p->nextCommand;
    memcpy (&tempString,
             &commandBuffer_p->character,
             8);
    for(int i=0;i<8;i++)
    {
        if(toupper(tempString[i])!=ESOSEND[i])
        {
            return FALSE;
        }
    }

    return TRUE;

}


/****************************************************************************
 *
 * Function:    isCommonUnsolicitedSignal
 *
 * Parameters:  SignalBuffer *signal_p
 *
 * Returns:     bolean
 *
 * Description: This function is used to decide whether we should allow a
 * signal to be processed only one time for all available entities.
 *
 ****************************************************************************/

static Boolean isCommonUnsolicitedSignal (const SignalBuffer  *signal_p)
{
  Boolean              CommonSignal = FALSE;
  VgmuxChannelNumber   channelNumber;

  /* Consider signal as Common unsolicited event if there is no active entity
   * and not from a call control element */
  if(   (vgChManCheckSignalDirection (*(signal_p->type), &channelNumber) == FALSE)
      || (vgOpManCheckSignalDirection (*(signal_p->type), &channelNumber) == FALSE))
  {
      switch (*signal_p->type)
      {
          /* signals that we know about and we may receive before the cimux has
           * enabled itself */
          case SIG_APEX_MM_RSSI_IND:
          case SIG_APEX_MM_BAND_IND:
#if defined(ENABLE_AT_ENG_MODE)
          case SIG_APEX_MM_CIPHER_IND:
#endif
          case SIG_APEX_MM_NETWORK_STATE_IND:
          {
               CommonSignal = TRUE;
               break;
          }

          default:
          {
               CommonSignal = FALSE;
               break;
          }
      }
  }

  return (CommonSignal);
}

/****************************************************************************
 *
 * Function:    signalEntityCheck
 *
 * Parameters:  SignalBuffer *signal_p
 *              VgmuxChannelNumber entity
 *
 * Returns:     Nothing
 *
 * Description: This function is used to decide whether we should allow a
 * signal to be be processed.  Some signals are used when an entity is
 * is enabled in which case we identify these and set the allowed flag
 * to true.  all the others will only be processed when the entity is active.
 *
 ****************************************************************************/

static Boolean signalEntityCheck (const VgmuxChannelNumber entity,
                                   const SignalBuffer  *signal_p)
{
  Boolean                 allowed = FALSE;

  switch (*signal_p->type)
  {
    /* signals that we know about but may receive before the cimux has
     * enabled itself */
    case SIG_VGCI_SS_REGISTRATION_IND:
    case SIG_INITIALISE:
    case SIG_APEX_SIM_OK_IND:
    case SIG_APEX_SIM_NOK_IND:
    case SIG_APEX_SIM_APP_STARTED_IND:
    case SIG_APEX_SIM_GET_PIN_IND:
    case SIG_APEX_SIM_CHV_STATUS_IND:
    case SIG_APEX_SIM_GET_CHV_IND:

#if defined (FEA_PHONEBOOK)      
    case SIG_APEX_LM_READY_IND:
#endif /* FEA_PHONEBOOK */

    case SIG_APEXGL_READY_IND:
    case SIG_APEX_SIM_CHV_FUNCTION_IND:
    case SIG_ANRM2_READ_DATA_CNF: /* allow the read but not the write */
    case SIG_APEX_SH_REGISTER_TASK_CNF:
    case SIG_CIMUX_CHANNEL_DISABLED_IND:
    case SIG_CIMUX_CHANNEL_ENABLE_IND:
    case SIG_APEX_ABPD_CHANNEL_ALLOC_CNF:
    case SIG_APEX_SM_READY_IND:     /* SMS initialisation */
    case SIG_APEX_SM_READ_SMSP_CNF:  /* SMS initialisation */
    case SIG_APEX_SH_CHANGE_CONTROL_IND:
    case SIG_TIMER_EXPIRY:
    case SIG_CIMUX_AT_DATA_IND:
    case SIG_CIMUX_AT_DATA_CNF:
    case SIG_APEX_MM_RSSI_IND:
    case SIG_APEX_MM_BAND_IND:
#if defined(ENABLE_AT_ENG_MODE)
    case SIG_APEX_MM_CIPHER_IND:
#endif
   case SIG_APEX_MM_NETWORK_STATE_IND:
   case SIG_APEX_MM_PSM_STATUS_IND:
    {
      if ( entity < CI_MAX_ENTITIES )
      {
        allowed = TRUE;
      }
      break;
    }

    /* signals that we will only receive after the cimux has enabled itself + any others */
    case SIG_CI_RUN_AT_COMMAND_IND:
    default:
    {
      if ( entity < CI_MAX_ENTITIES )
      {
        allowed = isEntityActive (entity);
      }
      break;
    }
  }
  return (allowed);
}

/****************************************************************************
 *
 * Function:    sendToSubSystem
 *
 * Parameters:  SignalBuffer       *signal_p
 *
 * Returns:     Nothing
 *
 * Description:  This function is used to distribute signals to sub-systems.
 * The interface controllers are called to see if they are interested in the
 * data.  If a sub-system decides that it is for it and it knows that it is
 * of no interest to others then it sets the accecpted flag accordingly.
 *
 ****************************************************************************/

static Boolean sendToSubSystem (SignalBuffer  *signal_p,
                                 const VoyagerSubsystems_t  ss,
                                  const VgmuxChannelNumber entity)
{
  Boolean accepted = FALSE;

  switch (ss)
  {
    case MUX:
    {
      accepted = vgMuxInterfaceController (signal_p, entity);
      break;
    }
    case NVRAM:
    {
      accepted = vgNvramInterfaceController (signal_p, entity);
      break;
    }
    case COMMAND_RESPONSE_MANAGER:
    {
      accepted = vgCrmanInterfaceController (signal_p, entity);
      break;
    }
    case OPERATIONS_MANAGER:
    {
      accepted = vgOpmanInterfaceController (signal_p, entity);
      break;
    }
    case CHANGE_CONTROL_MANAGER:
    {
      accepted = vgChManInterfaceController (signal_p, entity);
      break;
    }
    case MOBILITY:
    {
      accepted = vgMmssInterfaceController (signal_p, entity);
      break;
    }
    case SIM_LOCK:
    {
      accepted = vgSlssInterfaceController (signal_p, entity);
      break;
    }
#if defined (DEVELOPMENT_VERSION)
    case TEST:
    {
      accepted = vgTestInterfaceController (signal_p, entity);
      break;
    }
#endif
    case SMS:
    {
      accepted = vgMsssInterfaceController (signal_p, entity);
      break;
    }
    case SIM_TOOLKIT:
    {
      accepted = vgStkInterfaceController (signal_p, entity);
      break;
    }
    case PACKET_DATA:
    {
      accepted = vgGpssInterfaceController (signal_p, entity);
      break;
    }
    case CALL_CONTROL:
    {
      accepted = vgCcssInterfaceController (signal_p, entity);
      break;
    }
    case GENERAL:
    {
      accepted = vgGnssInterfaceController (signal_p, entity);
      break;
    }
    case PROFILE:
    {
      accepted = vgPfssInterfaceController (signal_p, entity);
      break;
    }
    case SUPPLEMENTARY:
    {
      accepted = vgSsssInterfaceController (signal_p, entity);
      break;
    }

    case SLEEP_MANAGER:
    {
      accepted = VgSleepManInterfaceController (signal_p, entity);
      break;
    }
#ifdef ENABLE_AP_BRIDGE_FEATURE
    /*Please keep AP Bridge subsystem as the last one.
      When AP Bridge is not ready at the start up process,AP Bridge
      will buffer the AT Commands, until AP Bridge subsystem is ready.*/
    case APBRIDGE:
    {
      accepted = vgApssInterfaceController (signal_p, entity);
      break;
    }
#endif
    default:
    {
      accepted = FALSE;
      break;
    }
  }

  return (accepted);
}

/****************************************************************************
 *
 * Function:    unknownEntityRouter
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 * Description:  This function is used to transfer signals to sub-systems.  An
 * initial check is made to see if a ss has registered a signal.  If it has then
 * the signal will only be sent to that ss.  If no registered ss can be found
 * then the signal is sent to all the entities and all the ss in those
 * entities.
 *
 * The second stage of this function uses the match found flag.  this will only
 * set to true if the sub-system knows that the signal is only relevant to it.
 * no other ss will then be called.  however, all the entities will be called so
 * it is necessary for the sub-system to decide whether for a given ss, the
 * signal should be processed or not.  The MUX is an example of this.
 *
 ****************************************************************************/

static Boolean unknownEntityRouter (SignalBuffer *signal_p)
{
  Boolean              matchFound = FALSE;
  Int8                 ssIndex;
  Int8                 eIndex, eEndIndex;
  Int8                 rIndex;
  VoyagerSubsystems_t  ssCode;
  Boolean              codeFound = FALSE;
  VgmuxChannelNumber   channelNumber;

  for ( eIndex = 0;
       ( ( eIndex < CI_MAX_ENTITIES ) &&  ( matchFound == FALSE ) );
        eIndex++ )
  {
    /* If Voyager has not been enabled then we may refuse to process some signals */
    if (signalEntityCheck ((VgmuxChannelNumber)eIndex, signal_p) == TRUE)
    {
      ssCode = getSsCodeForEntity (*signal_p->type,
                                   (VgmuxChannelNumber)eIndex,
                                     &rIndex,  /* index into regitration table */
                                      &codeFound);
      /* This signal has been registered so we know where to send this one */
      if (codeFound == TRUE)
      {
        switch (*signal_p->type)
        {
          /* COPN at 1200 asserts with an AMX error so we reschedule if we are flow controlled.
                    * This occurs when extended COPN commands are supplied at low (1200) baud rates */
          case SIG_APEX_EM_PLMN_TEST_CNF:
          {
            ChannelContext_t  *channelContext_p = ptrToChannelContext ((VgmuxChannelNumber)eIndex);

            FatalCheck(channelContext_p != PNULL, eIndex, 0, 0);

            {
              sendToSubSystem (signal_p,
                               ssCode,
                               (VgmuxChannelNumber)eIndex);
              resetRegTypeForEntity ((VgmuxChannelNumber)eIndex, rIndex);
            }
            break;
          }
          default: /* signals managed here do not need to consider flow control */
          {
            sendToSubSystem (signal_p,
                             ssCode,
                             (VgmuxChannelNumber)eIndex);
            resetRegTypeForEntity ((VgmuxChannelNumber)eIndex, rIndex);
            break;
          }
        }
        matchFound = TRUE; /* matched with a recorded solicited signal */
      }
    }
  }

  /* Check to find last entity which had control over signal source.
   * Send to all sub-systems with that entity. */
  if (matchFound == FALSE)
  {
    if ((vgChManCheckSignalDirection (*(signal_p->type), &channelNumber) == TRUE) ||
        (vgOpManCheckSignalDirection (*(signal_p->type), &channelNumber) == TRUE))
    {
      if (signalEntityCheck (channelNumber, signal_p) == TRUE)
      {
        for (ssIndex = (SS_START + 1);
             ((ssIndex < SS_END) && (matchFound == FALSE));
               ssIndex++)
        {
          matchFound = sendToSubSystem (signal_p,
                                        (VoyagerSubsystems_t)ssIndex, /* SS id */
                                          channelNumber);
        }
        /* the signal has not been accepted by any sub-system, but has been
         * processed by at least once */
        matchFound = TRUE;
      }
    }
  }

  /* Unsolicited signal so we have to send it to all entities and all sub-systems */
  if (matchFound == FALSE)
  {
    /* Check which unsolicited signal should be sent
     * to all entities and all sub-systems */
    if (!isCommonUnsolicitedSignal(signal_p))
    {
        /* Define End index for all entities */
        eEndIndex = CI_MAX_ENTITIES;
    }
    else
    {
        /* Define End index to send Unsolicited signal to one entity only */
        eEndIndex = 1;
    }

    for (eIndex = 0; eIndex < eEndIndex; eIndex++)
    {
        if (signalEntityCheck ((VgmuxChannelNumber)eIndex, signal_p) == TRUE)
        {
          matchFound = FALSE;

          for (ssIndex = (SS_START + 1);(ssIndex < SS_END && matchFound == FALSE); ssIndex++)
          {
            matchFound = sendToSubSystem (signal_p,
                                          (VoyagerSubsystems_t)ssIndex, /* SS id */
                                            (VgmuxChannelNumber)eIndex);
          }
          if (matchFound == TRUE)
          {

          }
        } /* not enabled */
    }
  }
  return (matchFound);
}

/****************************************************************************
 *
 * Function:    knownEntityRouter
 *
 * Parameters:  SignalBuffer       *signal_p
 *
 * Returns:     Nothing
 *
 * Description: The following function will call all the sub-systems with the
 * signal until one of the signals accepts the signal.  For example, the
 * initialise signal is sent to all the sub-systems.  Te entity id is not
 * important, the initialise signal is used to initialise all the entity
 * profile data.  The at data signal applies to a particular entity and that
 * entity must have been enabled.
 *
 ****************************************************************************/
static void knownEntityRouter (SignalBuffer *signal_p)
{
  Boolean             matchFound = FALSE;
  Int8                i;
  VgmuxChannelNumber  entity = VGMUX_CHANNEL_INVALID;

  /* extract the entity from the signal */
  switch (*signal_p->type)
  {
    case SIG_CIMUX_AT_DATA_IND:
    {
      entity = (VgmuxChannelNumber)signal_p->sig->ciMuxAtDataInd.channelNumber;
      break;
    }
#if defined(ENABLE_CIRM_DATA_IND)
    case SIG_CIRM_DATA_IND:
    {
      entity = signal_p->sig->cirmDataInd.channelNumber;
      break;
    }
#endif
    case SIG_CI_RUN_AT_COMMAND_IND:
    {
      GeneralContext_t  *generalContext_p;
      ChannelContext_t   *channelContext_p = ptrToChannelContext (signal_p->sig->ciRunAtCommandInd.entity);  
      entity = signal_p->sig->ciRunAtCommandInd.entity;
      /* When there is concatenated AT command string received, ATCI will analyze it
       * step by step, occasionally ATCI may receive SIG_CIMUX_CHANNEL_DISABLED_IND during
       * the analyze procedure, we should not process the SIG_CI_RUN_AT_COMMAND_IND in
       * such situation. */
      if (isEntityActive (entity) == TRUE)
      {
        generalContext_p = ptrToGeneralContext (entity);

#if defined(MTK_NBIOT_TARGET_BUILD)&& defined(HAL_WDT_MODULE_ENABLED)
        if(0 == entity)
        {
           hal_wdt_feed(HAL_WDT_FEED_MAGIC);
        }
#endif

#if defined (ATCI_SLIM_DISABLE)

        FatalCheck (generalContext_p != PNULL, entity, 0, 0);
#endif
        if (getProfileValue (entity, PROF_VERBOSE) == REPORTING_DISABLED)
        {
          generalContext_p->omitFirstNewLine = TRUE;
        }
        else
        {
          generalContext_p->omitFirstNewLine = FALSE;
        }
      }
      
      if(isEsosend(entity))
      {

          if (getEntityState (entity) != ENTITY_RUNNING)
          {
             setEntityState (entity, ENTITY_RUNNING);
          }
          matchFound = sendToSubSystem (signal_p, APBRIDGE, entity);
          return;
          break;
      }
      break;
    }
    case SIG_ANRM2_READ_DATA_CNF:
    {
      entity = (VgmuxChannelNumber)(signal_p->sig->anrm2ReadDataCnf.commandRef);
      break;
    } 
    case SIG_CIMUX_CHANNEL_ENABLE_IND:
    {
      entity = (VgmuxChannelNumber)signal_p->sig->ciMuxChannelEnableInd.channelNumber;
      break;
    }
    case SIG_CIMUX_CHANNEL_DISABLED_IND:
    {
      entity = (VgmuxChannelNumber)signal_p->sig->ciMuxChannelDisabledInd.channelNumber;
      break;
    }
    case SIG_CIMUX_CHECK_CMUX_CMD_PARAMS_CNF:
    {
      entity = (VgmuxChannelNumber)signal_p->sig->ciMuxCheckCmuxCmdParamsCnf.channelNumber;
      break;
    }
    case SIG_CIMUX_READ_CMUX_CMD_PARAMS_CNF:
    {
      entity = (VgmuxChannelNumber)signal_p->sig->ciMuxReadCmuxCmdParamsCnf.channelNumber;
      break;
    }
    case SIG_CIMUX_CONFIGURE_MUX_CNF:
    {
      entity = (VgmuxChannelNumber)signal_p->sig->ciMuxConfigureMuxCnf.channelNumber;
      break;
    }
    case SIG_CIMUX_SWITCHED_TO_CMD_MODE_IND:
    {
      entity = (VgmuxChannelNumber)signal_p->sig->ciMuxSwitchedToCmdModeInd.channelNumber;
      break;
    }
    case SIG_CIMUX_CLOSED_DATA_CONN_IND:
    {
      entity = (VgmuxChannelNumber)signal_p->sig->ciMuxClosedDataConnInd.channelNumber;
      break;
    }
    case SIG_CIMUX_AT_DATA_CNF:
    {
      entity = (VgmuxChannelNumber)signal_p->sig->ciMuxAtDataCnf.channelNumber;
      break;
    }
    case SIG_CIMUX_OPEN_DATA_CONN_CNF:
    {
      entity = (VgmuxChannelNumber)signal_p->sig->ciMuxOpenDataConnCnf.channelNumber;
      break;
    }
    case SIG_VGCI_SS_REGISTRATION_IND:
    {
      entity = signal_p->sig->vgCiSsRegistrationInd.entity;
      break;
    }
    case SIG_CI_USER_PROF_LOADED_IND:
    {
      entity = signal_p->sig->ciUserProfLoadedInd.entity;
      break;
    }
    case SIG_TIMER_EXPIRY:
    {
      entity = (VgmuxChannelNumber)signal_p->sig->kiTimerExpiry.userValue;
      break;
    }
#if defined (FEA_PPP)    
    case SIG_DS_PPP_CONFIG_REQ:
    {
      entity = (VgmuxChannelNumber)signal_p->sig->dsPppConfigReq.connId;
      break;
    }
#endif /* FEA_PPP */

    case SIG_APEX_SIM_OK_IND:
    case SIG_APEX_SIM_NOK_IND:
    case SIG_APEX_SIM_CHV_FUNCTION_IND:
    {
      /* unless the change control manager doesn't know what to do with the
       * signal use command channel 1 */
      if (vgChManCheckSignalDirection (*(signal_p->type), &entity) == FALSE)
      {
        entity = VGMUX_CHANNEL_COMMAND_1;
      }
      break;
    }
    case SIG_APEX_SIM_APP_STARTED_IND:
      entity = VGMUX_CHANNEL_COMMAND_1;
      break;

    case SIG_INITIALISE:
    {
      entity = findFirstEnabledChannel ();
      break;
    }
#ifdef ENABLE_AP_BRIDGE_FEATURE
    case SIG_CIAPB_DATA_MODE_RESUME_REQ:
    case SIG_CIAPB_REGISTER_AT_COMMAND_IND:
    {
      sendToSubSystem (signal_p, APBRIDGE, VGMUX_CHANNEL_INVALID);
      matchFound = TRUE;
      return;
      break;
    }
    case SIG_CIAPB_AT_RESPONSE_IND:
    {
      entity = (VgmuxChannelNumber)signal_p->sig->ciApbAtResponseInd.channelId;
      sendToSubSystem (signal_p, APBRIDGE, entity);
      matchFound = TRUE;
      return;
      break;
    }
#endif
#if !defined (USE_BMM_ALLOC_MEMORY)
    case SIG_UT_MEM_ABOVE_HWM_IND:
    case SIG_UT_MEM_BELOW_LWM_IND:
#endif
    default:
    {
      entity = findFirstEnabledChannel ();
      break;
    }
  }

  /* may receive signals when no entities are enabled, need to ensure entity
   * used not invalid */
  if (entity == VGMUX_CHANNEL_INVALID)
  {
    entity = VGMUX_CHANNEL_COMMAND_1;
  }

  /* if the entity is active then call it */
  if (signalEntityCheck (entity, signal_p) == TRUE)
  {
    for (i = (SS_START + 1);
         (i < SS_END && matchFound == FALSE);
           i++)
    {
      if (sendToSubSystem (signal_p,
                            (VoyagerSubsystems_t)i,
                              entity) == TRUE)
      {
        matchFound = TRUE;
      }
    }
  } /* should we call this entity */
}

/****************************************************************************
 *
 * Function:    pushSignalOnInternalQueue
 *
 * Parameters:  void
 *
 * Returns:     Nothing
 *
 * Description: Process a signal received of the external queue.  If its a
 * flow control signal or a cirm data ind that is echo based then process
 * asap.  We must add these to the HP queue.  If its a standard cirm data
 * ind signal then we should pop it onto the MP queue.  These signals
 * should be processed before we parse any more run at command signals.
 * Any thing else goies onto the LP queue, we process this when the CIMUX
 * is not busy; nothing on the external queue.
 *
 ****************************************************************************/

static void pushSignalOnInternalQueue (SignalBuffer  *receivedSignal)
{


  switch (*receivedSignal->type)
  {
    case SIG_CIMUX_AT_DATA_IND:
#ifdef ENABLE_AP_BRIDGE_FEATURE
    case SIG_CIAPB_REGISTER_AT_COMMAND_IND:
#endif
    {
      KiEnqueue( &ciHPInternalQueue, receivedSignal );
      break;
    }
#if defined(ENABLE_CIRM_DATA_IND)
    case SIG_CIRM_DATA_IND:
    {
      if (receivedSignal->sig->cirmDataInd.classOfData == ECHO_DATA)
      {
        KiEnqueue( &ciHPInternalQueue, receivedSignal );
      }
      else
      {
        KiEnqueue( &ciMPInternalQueue, receivedSignal );
      }
      break;
    }
#endif
    case SIG_CI_RUN_AT_COMMAND_IND:
    {
                   
        if(isEsosend(receivedSignal->sig->ciRunAtCommandInd.entity))
        {
            KiEnqueue( &ciHPInternalQueue, receivedSignal );
        }
        else
        {
            KiEnqueue( &ciLPInternalQueue, receivedSignal );       
        }
     break;
    }
    default:
    {

      /* proecess the rest of the signals in order */

        KiEnqueue( &ciLPInternalQueue, receivedSignal );

      break;
    }
  }
}

/****************************************************************************
 *
 * Function:    vgSignalDelayTimerExpiry
 *
 * Parameters:  entity - channel number
 *
 * Returns:     Nothing
 *
 * Description: Removes signal off delayed priority unit queue
 *
 ****************************************************************************/

void vgSignalDelayTimerExpiry (const VgmuxChannelNumber entity)
{
  SignalBuffer receivedSignal = kiNullBuffer;

  PARAMETER_NOT_USED(entity);

  /* check if queue still has pending signals */
  if (KiOnQueue (&ciDPInternalQueue))
  {
    /* remove first signal from queue */
    KiDequeue (&ciDPInternalQueue, &receivedSignal);

    /* put it on appropriate queue to be processed */
    pushSignalOnInternalQueue (&receivedSignal);

    /* start a new timer if signals still left on queue */
    if (KiOnQueue (&ciDPInternalQueue))
    {
      /* start delay timer off */
      vgCiStartTimer (TIMER_TYPE_SIGNAL_DELAY, entity);
    }
  }
  else
  {
    FatalParam (entity, 0, 0);
  }
}

/****************************************************************************
 *
 * Function:    vgExecuteDelayedAbsmIndSignals
 *
 * Parameters:  Nothing
 *
 * Returns:     Nothing
 *
 * Description: Put all the delayed ABSM indirection signal on the
 *              execution queue after the unsollicited channel has
 *              been released.
 *
 ****************************************************************************/
void vgExecuteDelayedAbsmIndSignals( void)
{
    SignalBuffer    receivedSignal = kiNullBuffer;
    Int32           numSignal;
    Int32           i;

    /* Only do operations if at least one signal has been delayed*/
    if( KiOnQueue( &ciSMInternalQueue) == TRUE)
    {
        /*  The delayed signal must be exectued right after the
        *   the SM unsollicited signal is free, so we have to
        *   insert the SM delayed signal in the top of the queue
        *   and to do such thing we have to dequeue all the existing signal
        *   and requeue it after the SM delayed ones*/
        numSignal = KiNumOnQueue( &ciLPInternalQueue);

        /* Process all the delayed SM signal*/
        while( KiOnQueue( &ciSMInternalQueue) == TRUE)
        {
            /* remove first signal from queue */
            KiDequeue( &ciSMInternalQueue, &receivedSignal);

            /* Insert it on the execution queue*/
            pushSignalOnInternalQueue( &receivedSignal);
        }

        for( i=0; i<numSignal; i++)
        {
            /* remove first signal from queue */
            KiDequeue( &ciLPInternalQueue, &receivedSignal);

            /* Insert it on the execution queue, at the end*/
            KiEnqueue( &ciLPInternalQueue, &receivedSignal);
        }
    }
}

/****************************************************************************
 *
 * Function:    waitForTaskInitialise
 *
 * Parameters:  void
 *
 * Returns:     Nothing
 *
 * Description: Just spin round until we get the initialise signal.
 * If we receive a boot registration signal then we store the task id of the
 * caller and notify them when VOP has a channel enabled/disabled.
 *
 ****************************************************************************/

static void waitForTaskInitialise (void)
{

  typedef enum SignalQueueStateTag
  {
    WAITING_FOR_INITIALISE_SIGNAL,
    PROCESSING
  } SignalQueueStateTag_t;

  SignalQueueStateTag_t  qState = WAITING_FOR_INITIALISE_SIGNAL;
  SignalBuffer           receivedSignal;

  /* spin round waiting for the task initialise signal */
  while (qState == WAITING_FOR_INITIALISE_SIGNAL)
  {
    receivedSignal = kiNullBuffer;
    KiReceiveSignal (VG_CI_QUEUE_ID, &receivedSignal);

    if (*receivedSignal.type == SIG_INITIALISE)
    {
      knownEntityRouter (&receivedSignal);
      qState = PROCESSING;
    }
    KiDestroySignal (&receivedSignal);
  }

}

#if defined (ON_PC)
/****************************************************************************
 *
 * Function:    VgCiTaskExitRoutine
 *
 * Parameters:  void
 *
 * Returns:     Nothing
 *
 * Description: Allows Borland to exit the task.
 *
 ****************************************************************************/

static void VgCiTaskExitRoutine(void)
{
}
#endif

/***************************************************************************
 * Processes
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:        VgCiTask
 *
 * Parameters:      None
 *
 * Returns:         Nothing
 *
 * Description:     Handles GKI signals coming into the CI Task
 *
 *-------------------------------------------------------------------------*/

KI_ENTRY_POINT VgCiTask (void);

KI_SINGLE_TASK (VgCiTask, VG_CI_QUEUE_ID, VG_CI_TASK_ID)

KI_ENTRY_POINT VgCiTask (void)
{

  SmsCommonContext_t  *smsCommonContext_p = ptrToSmsCommonContext ();
  SleepManContext_t   *sleepManContext_p  = ptrToSleepManContext();

#if defined (UPGRADE_SHARE_MEMORY) || defined(UPGRADE_SHMCL_SOLUTION)
  MobilityContext_t *mobilityContext_p    = ptrToMobilityContext ();
  AtDataType        atDataType;
#endif /* UPGRADE_SHARE_MEMORY || UPGRADE_SHMCL_SOLUTION */

  Int8               eIndex;
  Boolean            externalQueue;
  Boolean            signalRead;
  Boolean            signalProcessed;
  SignalBuffer       receivedSignal;
#if defined (ENABLE_L23_DEBUG)
  SignalBuffer       ciDebugInd = kiNullBuffer;
#endif
  if (psc_get_wakeup_reason() == PSC_DEEP_SLEEP)
  {
      InitialiseSleepManContextData(RESET_WAKEUP_FROM_DEEP_SLEEP);
  }

  /* Active lock reservation and registration of atci deep sleep callback function to PSC */
  psc_register_sleep_callback(PSC_CLIENT_ATCI, RvDeepSleepCb);
  psc_reserve_active_lock(PSC_CLIENT_ATCI);

  if (psc_get_wakeup_reason() == PSC_DEEP_SLEEP)
  {
    /* We woke up from deep sleep so we need to prevent sleep until fully awake */

    /* can't allow deep sleep until fully awake */
    psc_set_constraint(PSC_CLIENT_ATCI, PSC_CONSTRAINT_ALL, TRUE);

    /* other wakeup functionality will be triggered by receipt of SIG_INITIALSE */
  }
  
#if !defined (USE_L4MM_ALLOC_MEMORY)
#if defined (ON_PC)
  /* Required for nodeBSIM PC builds. */
  TmmInit();
#endif
#endif /* USE_L4MM_ALLOC_MEMORY */

#if defined (ENABLE_ATCI_UNIT_TEST)
  while(1)
  {
    if (rebootAtci)
    {
      vgResetAllAtciStructures();
    }    
#endif
  vgCiLocalTimerInit();
  /* we need to wait for the initialise signal and then initialise the internal queue */
  waitForTaskInitialise ();

  /* now if we are waking from deep sleep we need to update the context data with the
  data stored in RTC and NVRAM*/

#if defined (ENABLE_ATCI_UNIT_TEST)
  printf("RVMAIN: Power on cause: %d", sleepManContext_p->powerOnCause);
#endif
  
  if (sleepManContext_p->powerOnCause == RESET_NORMAL_POWER_ON)
  {
    RvInitialiseNVRamMemStore();
  }  
  else if (sleepManContext_p->powerOnCause == RESET_WAKEUP_FROM_DEEP_SLEEP)
  {
     RvWakeUpFromSleep();
  }
 
#if defined (ENABLE_ATCI_UNIT_TEST)
  if (rebootAtci)
  {
    rebootAtci = FALSE;
  }
  else
  {
#endif
  /* initialise the internal queues */
  ciHPInternalQueue = kiNullUnitQueue;
  ciMPInternalQueue = kiNullUnitQueue;
  ciLPInternalQueue = kiNullUnitQueue;
  ciDPInternalQueue = kiNullUnitQueue;

#if defined (ENABLE_ATCI_UNIT_TEST)
  }
  
  while (!rebootAtci)
#else
  for (;;)
#endif    
  {
    receivedSignal = kiNullBuffer;

    /* wait for a signal and then add the signal to the appropriate queue */
    KiReceiveSignal (VG_CI_QUEUE_ID, &receivedSignal);
    pushSignalOnInternalQueue (&receivedSignal);

    signalRead = TRUE;
    externalQueue = TRUE;

    while ( signalRead == TRUE )
    {

      signalProcessed = FALSE;

      /* Make sure no signals are requeued unless we call the SMS subsystem and it sets the flag. */
      smsCommonContext_p->requeueSignal = FALSE;
      /* pop a signal off the external queue if possible */
      if ( KiReceiveSignalPoll ( VG_CI_QUEUE_ID, &receivedSignal ) == TRUE )
      {
        pushSignalOnInternalQueue (&receivedSignal);
        externalQueue = TRUE;
      }

      /* if there is a signal on any queue then we must process it, if not then wait on external queue */
      if ( ( KiOnQueue (&ciHPInternalQueue) == TRUE ) ||
           ( KiOnQueue (&ciMPInternalQueue) == TRUE ) ||
           ( KiOnQueue (&ciLPInternalQueue) == TRUE ) )
      {

        if ( KiOnQueue (&ciHPInternalQueue) == TRUE )
        {
          /* read a signal from the internal HP ci queue */
          KiDequeue( &ciHPInternalQueue, &receivedSignal);
          signalProcessed = TRUE;
        }
        else if ( ( externalQueue == FALSE ) && ( KiOnQueue (&ciMPInternalQueue) == TRUE ) )
        {
          /* read a signal from the internal MP ci queue but only if the external queue is empty */
          KiDequeue( &ciMPInternalQueue, &receivedSignal);
          signalProcessed = TRUE;
        }
        else if ( ( externalQueue == FALSE ) && ( KiOnQueue (&ciLPInternalQueue) == TRUE ) )
        {
          /* read a signal from the internal LP ci queue but only if the external queue is empty */
          KiDequeue( &ciLPInternalQueue, &receivedSignal);
          signalProcessed = TRUE;
        }

        if (signalProcessed == TRUE)
        {

          switch (*receivedSignal.type)
          {
            /* For these signals the entity is contained within the signal.  We call this function
             * to send the signal to all sub-systems within the entity */ 
            case SIG_CI_RUN_AT_COMMAND_IND:
            case SIG_CI_USER_PROF_LOADED_IND:
            case SIG_APEX_SIM_OK_IND:
            case SIG_APEX_SIM_NOK_IND:
            case SIG_APEX_SIM_CHV_FUNCTION_IND:
            case SIG_APEX_SIM_APP_STARTED_IND: 
            case SIG_VGCI_SS_REGISTRATION_IND: 
#if defined(ENABLE_CIRM_DATA_IND)
            case SIG_CIRM_DATA_IND:
#endif
#if defined (FEA_PPP)
            case SIG_DS_PPP_CONFIG_REQ:
#endif /* FEA_PPP */              
            case SIG_CIMUX_AT_DATA_CNF:
            case SIG_CIMUX_AT_DATA_IND:
            case SIG_CIMUX_CHANNEL_DISABLED_IND:
            case SIG_CIMUX_CHANNEL_ENABLE_IND:
            case SIG_CIMUX_CHECK_CMUX_CMD_PARAMS_CNF:
            case SIG_CIMUX_CONFIGURE_MUX_CNF:
            case SIG_CIMUX_OPEN_DATA_CONN_CNF:
            case SIG_CIMUX_READ_CMUX_CMD_PARAMS_CNF:
            case SIG_CIMUX_SWITCHED_TO_CMD_MODE_IND:
            case SIG_CIMUX_CLOSED_DATA_CONN_IND:
            case SIG_TIMER_EXPIRY:

#if !defined (USE_BMM_ALLOC_MEMORY)
            case SIG_UT_MEM_ABOVE_HWM_IND:
            case SIG_UT_MEM_BELOW_LWM_IND:
#endif
            /* job106958: prevent duplicated processing of ApexShChangeControlInd */
            case SIG_APEX_SH_CHANGE_CONTROL_IND:
            case SIG_ANRM2_READ_DATA_CNF:
#ifdef ENABLE_AP_BRIDGE_FEATURE
            case SIG_CIAPB_REGISTER_AT_COMMAND_IND:
            case SIG_CIAPB_AT_RESPONSE_IND:
            case SIG_CIAPB_DATA_MODE_RESUME_REQ:
#endif
            {
              knownEntityRouter (&receivedSignal);
              break;
            }                
            default:
            {
              /* unsolicited or solicted signal with a registration signal */
              if ( unknownEntityRouter (&receivedSignal) == FALSE )
              {
                signalProcessed = FALSE;
              }
              break;
            }
          }   /*end of switch*/

#if defined (ENABLE_L23_DEBUG)
          KiCreateSignal (SIG_CI_DEBUG_IND,
                          sizeof (CiDebugInd),
                          &ciDebugInd);

          ciDebugInd.sig->ciDebugInd.receiveSigType = *receivedSignal.type;
#endif
          for (eIndex = 0;
               ((eIndex < CI_MAX_ENTITIES) && (signalProcessed == TRUE));
                 eIndex++)
          {
            if (isEntityActive ((VgmuxChannelNumber)eIndex) == TRUE)
            {
              checkForCommandCompleted ((VgmuxChannelNumber)eIndex);
              vgFlushBuffer((VgmuxChannelNumber)eIndex);
            }
          }
          /* SMS has recorded this signal on one of its queues, it will tidy
           * it up later */
          if( smsCommonContext_p->requeueSignal == TRUE)
          {
            /* Delay the signal while the unsollicited channel is busy*/
            KiEnqueue( &ciSMInternalQueue, &receivedSignal);
          }
          else
          {
            /* tidy up the memory created for the signals */
            KiDestroySignal ( &receivedSignal );
          }

#if defined (ENABLE_L23_DEBUG)
          ciDebugInd.sig->ciDebugInd.requestStatus  = (Int32) smsCommonContext_p->smSimState.requestStatus;
          ciDebugInd.sig->ciDebugInd.unsolicitedChannelBusy = smsCommonContext_p->unsolicitedChannelBusy;
          KiLogSignal (&ciDebugInd, TRUE);
#endif
        }
        else
        {
        }
      } /* data in internal queue */
      else
      {
        signalRead = FALSE;
      }

      externalQueue = FALSE;

#if defined (UPGRADE_SHARE_MEMORY)  || defined(UPGRADE_SHMCL_SOLUTION)
      /* Here creg/cgreg data will be checked and confirm whether these data
      ** has be updated to share memory
      */
      if(mobilityContext_p->cregUpdated == FALSE)
      {
        atDataType.cmdType                          = CREG_CMD;
        atDataType.atData.cregData.accessTechnology = mobilityContext_p->cregData.accessTechnology;
        atDataType.atData.cregData.cellId           = mobilityContext_p->cregData.cellId;
        atDataType.atData.cregData.lac              = mobilityContext_p->cregData.lac;
        atDataType.atData.cregData.state            = mobilityContext_p->cregData.state;
        atDataType.atData.cregData.serviceStatus    = mobilityContext_p->cregData.serviceStatus;
        if(DriWriteUnsolictedInd(&atDataType) == FALSE)
        {
          mobilityContext_p->cregUpdated = TRUE;
        }
      }
      if(mobilityContext_p->cgregUpdated == FALSE)
      {
        atDataType.cmdType                           = CGREG_CMD;
        atDataType.atData.cgregData.accessTechnology = mobilityContext_p->cgregData.accessTechnology;
        atDataType.atData.cgregData.cellId           = mobilityContext_p->cgregData.cellId;
        atDataType.atData.cgregData.lac              = mobilityContext_p->cgregData.lac;
        atDataType.atData.cgregData.state            = mobilityContext_p->cgregData.state;
        if(DriWriteUnsolictedInd(&atDataType) == FALSE )
        {
          mobilityContext_p->cgregUpdated = TRUE;
        }
      }

#endif /* UPGRADE_SHARE_MEMORY || UPGRADE_SHMCL_SOLUTION */

    } /* while signals on a queue*/
  } /* loop forever */
#if defined (ENABLE_ATCI_UNIT_TEST)
  }
#endif
} /* END OF FILE */
