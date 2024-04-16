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
 * RVCHCUT.C
 * Change control manager custom tables
 **************************************************************************/

#define MODULE_NAME "RVCHCUT"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#if !defined (RVCHCUT_H)
#  include <rvchcut.h>
#endif
#if !defined (RVCIMXUT_H)
#  include <rvcimxut.h>
#endif

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

/***************************************************************************
 * Types
 ***************************************************************************/

typedef void (*VgChCustSigReq)(VgmuxChannelNumber  entity);

typedef struct ContinueActionsTag
{
  SignalId        signalId;
  VgChCustSigReq  procFunc;
} ContinueActionsCustomerTable_t;

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* the table below matches up the signal type to the relevant target background
 * layer procedure and the procedure to call to send the signal when control
 * has been given */

static const ContinueActionsCustomerTable_t continueActionsCustomerTable[] =
{
  { SIG_SYS_DUMMY, PNULL }
};

#define NUM_CONTINUE_ACTIONS (sizeof(continueActionsCustomerTable) / sizeof(ContinueActionsCustomerTable_t))


/***************************************************************************
 * Variables
 ***************************************************************************/

/***************************************************************************
 * Macros
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:        continueActionsCustomerList
 *
 * Parameters:      entity   - channel wanting to send signal
 *                  signalId - type of signal to be sent to BL
 *
 * Returns:         Boolean: has signalId matched any element in the table?
 *
 * Description:     Sends a send a signal to the application background layer
 *                  after ascertaining whether voyager op has control of the
 *                  relevant BL procedure
 *
 *-------------------------------------------------------------------------*/

Boolean continueActionsCustomerList (const VgmuxChannelNumber entity,
                                      const SignalId signalId,
                                       ResultCode_t  *result)
{
  Int8                     aIndex;
  Boolean                  matched = FALSE;
  Boolean                  commandRunning = FALSE;
  ChManagerContext_t       *chManagerContext_p = ptrToChManagerContext ();
  VgChangeControlElements  element;

  *result = RESULT_CODE_PROCEEDING;

  /* loops through continueAction table searching for requested signal
   * information */

  for (aIndex = 0;
       (aIndex < NUM_CONTINUE_ACTIONS) && (matched == FALSE);
         aIndex++ )
  {
    if (continueActionsCustomerTable[aIndex].signalId == signalId)
    {
      /* the signal has been matched, perform checks on control */
      matched = TRUE;

      if (vgChManFindElement (signalId, &element) == FALSE)
      {
        FatalParam (entity, signalId, 0);
        *result = RESULT_CODE_ERROR;
      }
      else
      {
        /* if no pending signal then check entity state to determine if
         * command is running */
        if (chManagerContext_p->vgControl[element].pendingSignal == NON_SIGNAL)
        {
          /* check if we are running a command */
          if (getEntityState (entity) == ENTITY_RUNNING)
          {
            commandRunning = TRUE;
          }
        }
        else
        {
          /* a signal is pending, use pendingCommand flag */
          commandRunning = chManagerContext_p->vgControl[element].pendingCommand;
        }

        if (vgChManCheckHaveControl (element, entity) == TRUE)
        {
          if (chManagerContext_p->vgControl[element].pendingCompletion == FALSE)
          {
            /* send signal requested */
            (continueActionsCustomerTable[aIndex].procFunc) (entity);
            /* if no command running then release control after sending signal */
            if (commandRunning == FALSE)
            {
              vgChManReleaseControlOfElement ((VgChangeControlElements)element, entity);
            }
            /* reset pending signal */
            chManagerContext_p->vgControl[element].pendingSignal  = NON_SIGNAL;
            chManagerContext_p->vgControl[element].pendingCommand = FALSE;
          }
          else /* entity has control but an operation is currently running already */
          {
            *result = VG_CME_UNABLE_TO_GET_CONTROL;
            /* run post action action */
            vgChManPostAction (signalId, entity, FALSE);
          }
        }
        else
        {
          if (chManagerContext_p->vgControl[element].isSingleUser == TRUE)
          { /* single user procs */
            /* change control signals must be sent to BL */
            if (vgChManGetControl (element, entity) == TRUE)
            {
              chManagerContext_p->vgControl[element].pendingSignal  = signalId;
              chManagerContext_p->vgControl[element].pendingCommand = commandRunning;
            }
            else
            {
              *result = VG_CME_UNABLE_TO_GET_CONTROL;
              /* run post action action */
              vgChManPostAction (signalId, entity, FALSE);
            }
          }
          else
          { /* multi user procs */
            /* may assume control straight away and send signals to BL */
            if (chManagerContext_p->vgControl[element].state == CONTROL_NOT)
            {
              chManagerContext_p->vgControl[element].state          = CONTROL_GOT;
              chManagerContext_p->vgControl[element].entity         = entity;
              chManagerContext_p->vgControl[element].pendingSignal  = NON_SIGNAL;
              chManagerContext_p->vgControl[element].pendingCommand = FALSE;
              /* send requested signal */
              (continueActionsCustomerTable[aIndex].procFunc) (entity);
              /* run post action action */
              *result = vgChManPostAction (signalId, entity, TRUE);
              /* if no command running then release control after sending signal */
              if (commandRunning == FALSE)
              {
                vgChManReleaseControlOfElement ((VgChangeControlElements)element, entity);
              }
            }
            else
            {
              if (vgChManGetControl (element, entity) == TRUE)
              { /* maybe unregistered */
                chManagerContext_p->vgControl[element].pendingSignal  = signalId;
                chManagerContext_p->vgControl[element].pendingCommand = commandRunning;
              }
              else
              {
                *result = VG_CME_UNABLE_TO_GET_CONTROL;
                /* run post action action */
                vgChManPostAction (signalId, entity, FALSE);
              }
            }
          }
        }
      }
    }
  }

  return (matched);

}


