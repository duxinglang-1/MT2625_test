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
 * RVCHSIGI.C
 * Incoming signal handlers for change control manager
 **************************************************************************/

#define MODULE_NAME "RVCHSIGI"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif

#include <stdio.h>
#include <rvgncpb.h>
#include <abgl_sig.h>
#include <rvcfg.h>
#include <gkisig.h>

#if !defined (RVDATA_H)
#  include <rvdata.h>
#endif
#if !defined (RVCHSIGI_H)
#  include <rvchsigi.h>
#endif
#if !defined (RVCHSIGO_H)
#  include <rvchsigo.h>
#endif

#include <rvsleep.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  ApexShRegisterTaskCnf  apexShRegisterTaskCnf;

#if defined (FEA_PHONEBOOK)  
  ApexLmReadyInd         apexLmReadyInd;
#endif /* FEA_PHONEBOOK */

  ApexSmReadyInd         apexSmReadyInd;
  ApexGlReadyInd         apexGlReadyInd;
  ApexEmReadyInd         apexEmReadyInd;
  ApexSimOkInd           apexSimOkInd;
  ApexShChangeControlInd apexShChangeControlInd;
  ApexShChangeControlCnf apexShChangeControlCnf;
};

 /***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

 /***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexShRegisterTaskCnf
 *
 * Parameters:      ApexShRegisterTaskCnf - BL procedure registration
 *                                          confirmation signal:
 *                                          SIG_APEX_SH_REGISTER_TASK_CNF
 *
 * Returns:         Nothing
 *
 * Description:     Sets the BL control state machine ready for change control
 *                  requests to be actioned.
 *-------------------------------------------------------------------------*/

void vgSigApexShRegisterTaskCnf (const SignalBuffer *signalBuffer)
{
  ApexShRegisterTaskCnf     *sig_p                      = &signalBuffer->sig->apexShRegisterTaskCnf;
  ChManagerContext_t        *chManagerContext_p         = ptrToChManagerContext ();
  GeneralGenericContext_t   *generalGenericContext_p    = ptrToGeneralGenericContext (); 

  /* if succesfully registered that set control flag */

  if (sig_p->isRegistered == TRUE)
  {
    if (chManagerContext_p->vgControl[sig_p->procId].sendsReadyInd == TRUE)
    {
      /* we are anticipating a ready indication to follow */
      chManagerContext_p->vgControl[sig_p->procId].state = NOT_READY;
    }
    else
    {
      chManagerContext_p->vgControl[sig_p->procId].state = CONTROL_NOT;

      /* deal with any pending signals to be sent */
      vgChManProcessPendingSignal ((VgChangeControlElements)sig_p->procId);

      if (PROCEDURE_BL_PD_ID == sig_p->procId &&
          WAITING_FOR_DEFAULT_APN == generalGenericContext_p->initDataFromABPDState)
      {
        vgChManContinueAction(findFirstEnabledChannel(), SIG_APEX_ABPD_APN_READ_REQ);
      }
    }
  }
  else
  {
    chManagerContext_p->vgControl[sig_p->procId].state = UNREGISTERED;

    /* if registering to send a signal return error code */

    if (chManagerContext_p->vgControl[sig_p->procId].pendingSignal != NON_SIGNAL)
    {
      /* run post action action */
      vgChManPostAction (
       chManagerContext_p->vgControl[sig_p->procId].pendingSignal,
        chManagerContext_p->vgControl[sig_p->procId].entity,
         FALSE);

      /* if running a command then set result code */
      if (chManagerContext_p->vgControl[sig_p->procId].pendingCommand == TRUE)
      {
        setResultCode (chManagerContext_p->vgControl[sig_p->procId].entity,
                        VG_CME_UNABLE_TO_GET_CONTROL);
      }

      chManagerContext_p->vgControl[sig_p->procId].pendingSignal  = NON_SIGNAL;
      chManagerContext_p->vgControl[sig_p->procId].pendingCommand = FALSE;
      /* don't reset entity, this ensures the last entity to have control is remembered */

    }
  }
}

#if defined (FEA_PHONEBOOK)
/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmReadyInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_READY_IND
 *
 * Returns:     Nothing
 *
 * Description: Modifies BL change control state machine depending on whether
 *              the LM BL procedure is ready.
 *----------------------------------------------------------------------------*/

void vgSigApexLmReadyInd (const SignalBuffer *signalBuffer)
{
    ApexLmReadyInd      *sig_p = &signalBuffer->sig->apexLmReadyInd;
    ChManagerContext_t  *chManagerContext_p = ptrToChManagerContext ();
    SleepManContext_t   *sleepManContext_p  = ptrToSleepManContext();
    Boolean             continueAction = FALSE;

    switch (sig_p->status)
    {
        case LM_REQ_OK:
            {
                chManagerContext_p->vgControl[CC_LIST_MANAGEMENT].state = CONTROL_NOT;
                continueAction = TRUE;
            }
            break;

        case LM_REQ_ALPHA_OK:
            {
                /* do nothing - not quite ready for requests */
            }
            break;

        default:
            {
                chManagerContext_p->vgControl[CC_LIST_MANAGEMENT].state = SUSPENDED;
                continueAction = TRUE;
            }
            break;
    }

    if (continueAction == TRUE)
    {
        /* deal with any pending signals to be sent */
        vgChManProcessPendingSignal (CC_LIST_MANAGEMENT);
    }

    sleepManContext_p->needLmReadyInd = FALSE;
    RvWakeUpCompleteCheck();

}
#endif /* FEA_PHONEBOOK */

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSmReadyInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_SM_READY_IND
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Modifies BL change control state machine depending on whether
 *              the SM BL proc is ready.
 *----------------------------------------------------------------------------*/

void vgSigApexSmReadyInd (const SignalBuffer *signalBuffer)
{
  ApexSmReadyInd      *sig_p = &signalBuffer->sig->apexSmReadyInd;
  ChManagerContext_t  *chManagerContext_p = ptrToChManagerContext ();
  SleepManContext_t   *sleepManContext_p  = ptrToSleepManContext();
  Boolean             continueAction = FALSE;

  switch (sig_p->requestStatus)
  {
    case SM_REQ_OK:
    case SM_REQ_SM_SEND_READY:
    case SM_REQ_SM_SNDRCV_READY:
    {
      chManagerContext_p->vgControl[CC_SHORT_MESSAGE_SERVICE].state = CONTROL_NOT;
      continueAction = TRUE;
      break;
    }

    default:
    {
      chManagerContext_p->vgControl[CC_SHORT_MESSAGE_SERVICE].state = SUSPENDED;
      continueAction = TRUE;
      break;
    }
  }

  if (continueAction == TRUE)
  {
    /* deal with any pending signals to be sent */
    vgChManProcessPendingSignal (CC_SHORT_MESSAGE_SERVICE);
  }

  sleepManContext_p->needSmReadyInd = FALSE;
  RvWakeUpCompleteCheck();
}


/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexEmReadyInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_EM_READY_IND
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Modifies BL change control state machine depending on whether
 *              the EM BL procedure is ready.
 *----------------------------------------------------------------------------*/

void vgSigApexEmReadyInd (const SignalBuffer *signalBuffer)
{
  ApexEmReadyInd      *sig_p = &signalBuffer->sig->apexEmReadyInd;
  ChManagerContext_t  *chManagerContext_p = ptrToChManagerContext ();
  Boolean             continueAction = FALSE;

  switch (sig_p->requestStatus)
  {
    case EM_REQ_OK:
    {
      chManagerContext_p->vgControl[CC_ENGINEERING_MODE].state = CONTROL_NOT;
      continueAction = TRUE;
      break;
    }
    default:
    {
      chManagerContext_p->vgControl[CC_ENGINEERING_MODE].state = SUSPENDED;
      continueAction = TRUE;
      break;
    }
  }

  if (continueAction == TRUE)
  {
    /* deal with any pending signals to be sent */
    vgChManProcessPendingSignal (CC_ENGINEERING_MODE);
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_SIM_OK_IND
 *                                   SIG_APEX_SIM_NOK_IND
 *                                   SIG_APEX_SIM_GET_CHV_IND
 * Returns:     Nothing
 *
 * Description: Modifies BL change control state machine depending on whether
 *              the SIM is ready.
 *-------------------------------------------------------------------------*/

void vgSigApexSimInd (const SignalBuffer *signalBuffer)
{
  ChManagerContext_t  *chManagerContext_p = ptrToChManagerContext ();

  PARAMETER_NOT_USED(signalBuffer);

  chManagerContext_p->vgControl[CC_SUBSCRIBER_IDENTITY_MODULE].state = CONTROL_NOT;

  /* deal with any pending signals to be sent */
  vgChManProcessPendingSignal (CC_SUBSCRIBER_IDENTITY_MODULE);
}


/*----------------------------------------------------------------------------
 *
 * Function:    vgApexShChangeControlInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_SH_CHANGE_CONTROL_IND
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: If another task requests control and we are not using the
 *              resource then give it up
 *----------------------------------------------------------------------------*/

void vgSigApexShChangeControlInd (const SignalBuffer *signalBuffer)
{
  ApexShChangeControlInd *sig_p = &signalBuffer->sig->apexShChangeControlInd;

  ChManagerContext_t *chManagerContext_p = ptrToChManagerContext ();

  /* if we're not using procedure then relinquish control */
  if (chManagerContext_p->vgControl[sig_p->procId].state == CONTROL_NOT)
  {
    vgSigApexShChangeControlRsp (CC_STATUS_OK,
                                  sig_p->initiatingTaskId,
                                   sig_p->newControlTaskId,
                                    sig_p->procId);
  }
  else
  {
    vgSigApexShChangeControlRsp (CC_STATUS_REFUSED_BY_CURRENT_CONTROL,
                                  sig_p->initiatingTaskId,
                                   sig_p->newControlTaskId,
                                    sig_p->procId);
  }
}


/*----------------------------------------------------------------------------
 *
 * Function:    vgApexShChangeControlCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_SH_CHANGE_CONTROL_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Modifies BL change control state machine depending on success
 *              of change control request.
 *----------------------------------------------------------------------------*/

void vgSigApexShChangeControlCnf (const SignalBuffer *signalBuffer)
{
  ApexShChangeControlCnf *sig_p = &signalBuffer->sig->apexShChangeControlCnf;

  ChManagerContext_t *chManagerContext_p = ptrToChManagerContext ();

  /* check control status in signal */
  if ((sig_p->changeControlStatus == CC_STATUS_OK) ||
      (sig_p->changeControlStatus == CC_STATUS_ALREADY_IN_CONTROL) )
  {
    /* obtaining control */
    if (sig_p->newControlTaskId == VG_CI_TASK_ID)
    {
      /* now have control */
      chManagerContext_p->vgControl[sig_p->procId].state = CONTROL_GOT;

      /* deal with any pending signals to be sent */
      vgChManProcessPendingSignal ((VgChangeControlElements)sig_p->procId);
    }
    else
    {
      /* releasing control */
      chManagerContext_p->vgControl[sig_p->procId].state = CONTROL_NOT;
      /* don't reset entity, this ensures the last entity to have control is
       * remembered */
    }
  }
  else /* failed to get control of requested background layer procedure */
  {
    /* run post action action */
    vgChManPostAction (
     chManagerContext_p->vgControl[sig_p->procId].pendingSignal,
      chManagerContext_p->vgControl[sig_p->procId].entity,
       FALSE);

    /* if running a command then set result code */
    if (chManagerContext_p->vgControl[sig_p->procId].pendingCommand == TRUE)
    {
      setResultCode (chManagerContext_p->vgControl[sig_p->procId].entity,
                      VG_CME_UNABLE_TO_GET_CONTROL);
    }

    chManagerContext_p->vgControl[sig_p->procId].state          = CONTROL_NOT;
    chManagerContext_p->vgControl[sig_p->procId].pendingSignal  = NON_SIGNAL;
    chManagerContext_p->vgControl[sig_p->procId].pendingCommand = FALSE;
    /* don't reset entity, this ensures the last entity to have control is remembered */
  }
}

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexGlReadyInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_GL_READY_IND
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Modifies BL change control state machine depending on whether
 *              the FL BL proc is ready.
 *----------------------------------------------------------------------------*/

void vgSigApexGlReadyInd (const SignalBuffer *signalBuffer)
{
    ApexGlReadyInd             *sig_p                   = &signalBuffer->sig->apexGlReadyInd;
    ChManagerContext_t         *chManagerContext_p      = ptrToChManagerContext ();
    Boolean                     continueAction          = FALSE;
    GeneralGenericContext_t    *generalGenericContext_p = ptrToGeneralGenericContext();
    OpmanGenericContext_t*      opManGenericContext_p   = ptrToOpManGenericContext ();
    SleepManContext_t          *sleepManContext_p       = ptrToSleepManContext();
    SignalBuffer                signal                  = kiNullBuffer;
    Int8                        eIndex                  = 0;

    switch (sig_p->status)
    {
        case GL_REQUEST_OK:
        {
            chManagerContext_p->vgControl[CC_GENERAL_MODULE].state = CONTROL_NOT;
            continueAction = TRUE;

            /* Store features information*/
            generalGenericContext_p->vgMFtrCfgData.nvramCfgVar      = sig_p->featureConfig;
            generalGenericContext_p->vgMFtrCfgData.currentCfgVar    = sig_p->featureConfig;
            generalGenericContext_p->vgMFtrCfgData.initialised      = TRUE;

            /* Initiliase the AT configuration*/
            cfRvSmHandleConcatSms = sig_p->featureConfig.smHandleConcatSms;

            /* schedule any outstanding ENABLE request */
            for(    eIndex = 0;
                    eIndex < CI_MAX_ENTITIES;
                    eIndex++)
            {
                if ( opManGenericContext_p->channelNeedsEnabling [eIndex] == TRUE )
                {
                    CiMuxChannelEnableInd *req_p = PNULL;
                    signal = kiNullBuffer;

                    KiCreateZeroSignal( SIG_CIMUX_CHANNEL_ENABLE_IND,
                                        sizeof (CiMuxChannelEnableInd),
                                        &signal);

                    req_p = (CiMuxChannelEnableInd*) signal.sig;
                    req_p->channelNumber = opManGenericContext_p->channelEnableIndChannelNumber[eIndex];

                    KiSendSignal (VG_CI_TASK_ID, &signal);

                    opManGenericContext_p->channelNeedsEnabling [eIndex] = FALSE;
                }
            }
        }
        break;

        default:
        {
            chManagerContext_p->vgControl[CC_GENERAL_MODULE].state = SUSPENDED;
            continueAction = TRUE;
        }
        break;
    }

    if (continueAction == TRUE)
    {
        /* deal with any pending signals to be sent */
        vgChManProcessPendingSignal (CC_GENERAL_MODULE);
    }

    sleepManContext_p->needGlReadyInd = FALSE;
    RvWakeUpCompleteCheck();

}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */


