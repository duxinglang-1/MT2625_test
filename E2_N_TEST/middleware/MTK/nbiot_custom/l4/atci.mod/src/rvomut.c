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
 * RVOMUT.C
 * Operations manager utility module
 **************************************************************************/

#define MODULE_NAME "RVOMUT"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvsystem.h>
#include <rvomut.h>
#include <rvcimxut.h>
#include <rvutil.h>
#include <rvoman.h>

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

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

/* connection enable functions */
static Boolean vgOpManIdleModeEnHandler ( const VgmuxChannelNumber entity,
                                           const ConnectionClass_t  reqCon );
static Boolean vgOpManPsdOnlyModeEnHandler ( const VgmuxChannelNumber entity,
                                              const ConnectionClass_t  reqCon );
/* connection disable functions */
static Boolean vgOpManPsdOnlyModeDsHandler ( const VgmuxChannelNumber entity,
                                              const ConnectionClass_t  reqCon );

/* control to handle change in call control states */
static Boolean  vgOpManModifyCcState (const VgmuxChannelNumber entity,
                                       const ConnectionClass_t  connectionClass,
                                        const Boolean  enableConnection);

/***************************************************************************
 * Local Functions
 ***************************************************************************/

 /***************************************************************************
 * Static Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:    vgOpManIdleModeEnHandler
 *
 * Parameters:  entity - channel number requesting a connection
 *              reqCon - type of call connection wanted
 *
 * Returns:     Boolean - indicating whether connection allowed
 *
 * Description: Function is used to change the call control state to a
 * new state based on a connection class.  Modifies the data in the
 * entity context store when in Idle.
 *
 * When in idle we can make any connection.
 *
 *-------------------------------------------------------------------------*/

static Boolean vgOpManIdleModeEnHandler ( const VgmuxChannelNumber entity,
                                           const ConnectionClass_t reqCon )
{
  OpmanContext_t  *opManContext_p = ptrToOpManContext (entity);
  Boolean         accepted = TRUE;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(opManContext_p != PNULL, entity, 0, 0);
#endif
  switch ( reqCon )
  {
#if defined (FEA_PPP)    
    case PPP_CONNECTION:
#endif /* FEA_PPP */      
    case PT_CONNECTION:
    {
      opManContext_p->currentCcontrolState = PSD_ONLY;
      break;
    }
    default:
    {
      accepted = FALSE;
      break;
    }
  }
  return (accepted);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgOpManPsdOnlyModeEnHandler
 *
 * Parameters:  entity - channel number requesting a connection
 *              reqCon - type of call connection wanted
 *
 * Returns:     Boolean - indicating whether connection allowed
 *
 * Description: Function is used to change the call control state to a
 * new state based on a connection class.  Modifies the data in the
 * entity context store when in PSD only.
 *
 * When we are in PSD state then we can allow CSV or a single CSD calls only.
 *
 *-------------------------------------------------------------------------*/

static Boolean vgOpManPsdOnlyModeEnHandler ( const VgmuxChannelNumber entity,
                                              const ConnectionClass_t  reqCon )
{
  Boolean           accepted = TRUE;

  switch ( reqCon )
  {
    default:
    {
      accepted = FALSE;
      break;
    }
  }

  return (accepted);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgOpManPsdOnlyModeDsHandler
 *
 * Parameters:  entity - channel number requesting a connection
 *              reqCon - type of call connection wanted
 *
 * Returns:     Boolean - indicating whether connection allowed
 *
 * Description:
 *
 * Just PSD running so thats all we can disable.
 *
 *-------------------------------------------------------------------------*/

static Boolean vgOpManPsdOnlyModeDsHandler ( const VgmuxChannelNumber entity,
                                              const ConnectionClass_t  reqCon )
{
  OpmanContext_t*  opManContext_p = ptrToOpManContext (entity);
  Boolean          accepted = TRUE;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(opManContext_p != PNULL, entity, 0, 0);
#endif
  switch ( reqCon )
  {
#if defined (FEA_PPP)
    case PPP_CONNECTION:
#endif /* FEA_PPP */      
    case PT_CONNECTION:
    {
      opManContext_p->currentCcontrolState = ALL_IDLE;
      break;
    }
    default:
    {
      accepted = FALSE;
      break;
    }
  }
  return (accepted);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgOpManModifyCcState
 *
 * Parameters:  entity - channel number requesting a connection
 *              connectionClass - type of call connection wanted
 *              enableConnection - whether to enable or disable connection
 *
 * Returns:     Boolean - indicating whether modification was successful
 *
 * Description: Functions purpose is to control the state changes for call
 * control.  Given a connection type we look at the current state and decide
 * whether the connection can be made.  if it can then the new state is enabled.
 *
 * The look-up table maps the current state for an entity, the required connection
 * to the handler.  The handler then decides on the new state given the
 * conditions.
 *
 * The function op entity cross check is used to evaluate all the entities.  Each
 * entity records its current state but we must use all the entity data to make
 * a complete decision.
 *
 *-------------------------------------------------------------------------*/

static Boolean  vgOpManModifyCcState (const VgmuxChannelNumber entity,
                                       const ConnectionClass_t  connectionClass,
                                        const Boolean  enableConnection)
{

# define ENABLE  (TRUE)
# define DISABLE (FALSE)

  typedef Boolean (*VgOpManCcSigReq)(const VgmuxChannelNumber entity,
                                      const ConnectionClass_t reqCon);
  typedef struct StateControllerTag
  {
    Boolean                typeOfConnection;
    CiCallControlStates_t  state;
    VgOpManCcSigReq        procFunc;
  } stateController_t;

  static const stateController_t ccProcAction[] =
  {
    { ENABLE,  ALL_IDLE,    vgOpManIdleModeEnHandler     },
    { ENABLE,  PSD_ONLY,    vgOpManPsdOnlyModeEnHandler  },  /* make a connection when the state is PSD only */
    /* disable connections */
    { DISABLE,  ALL_IDLE,    PNULL                       },
    { DISABLE,  PSD_ONLY,    vgOpManPsdOnlyModeDsHandler }   /* kill a connection when the state is PSD only */
  };

#define NUM_ACTIONS (sizeof (ccProcAction) / sizeof (stateController_t))

  OpmanContext_t*   opManContext_p;
  Boolean           accepted = TRUE;

  Int8              aloop;

  if (((accepted == TRUE) || (enableConnection == FALSE)) &&
      (isEntityActive(entity) == TRUE))
  {
    accepted = FALSE;
    opManContext_p = ptrToOpManContext (entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(opManContext_p != PNULL, entity, 0, 0);
#endif
    for (aloop = 0; (aloop < NUM_ACTIONS) && (accepted == FALSE); aloop++ )
    {
      if ((ccProcAction [aloop].typeOfConnection == enableConnection) &&
          (ccProcAction [aloop].state == opManContext_p->currentCcontrolState))
      {
        accepted = TRUE;
        if ( ccProcAction [aloop].procFunc != PNULL )
        {
          accepted = (ccProcAction [aloop].procFunc) (entity, connectionClass);
          if (accepted == TRUE)
          {
            if (enableConnection == TRUE)
            {
              opManContext_p->vgLastCallConnectionType = connectionClass;

              vgOpManResetCallInfo (&opManContext_p->callInfo);

              opManContext_p->callInfo.vgClass = connectionClass;

              opManContext_p->numberOfCallConnections += 1;
            }
            else
            {
              /* find call that is offline */
              if (opManContext_p->numberOfCallConnections > 0)
              {
                if (((opManContext_p->callInfo.vgState == CONNECTION_OFF_LINE) ||
                     (opManContext_p->callInfo.vgIdent == NEVER_USED_USER_CALL_ID))&&
                    (opManContext_p->callInfo.vgClass == connectionClass))
                {
                  /* wipe information in last record */
                  vgOpManResetCallInfo (&opManContext_p->callInfo);
                  opManContext_p->numberOfCallConnections -= 1;
                }                    
              }

            }
          }
        }
        else
        {
          accepted = FALSE;
        }
      }
    }
  }

  return (accepted);
}

/***************************************************************************
 * Exported Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:    vgOpManResetCallInfo
 *
 * Parameters:
 *
 * Returns:
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/

void vgOpManResetCallInfo (ConnectionInformation_t *callInfo)
{
  callInfo->vgClass = CONNECTION_TERMINATOR;
  callInfo->vgState = CONNECTION_OFF_LINE;
  callInfo->vgIdent = NEVER_USED_USER_CALL_ID;
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgOpManUtilsEnableConnection
 *
 * Parameters:  entity           - channel number requesting a connection
 *              connectionClass  - the connection that needs to be established.
 *                                 For example, a CSV call or an NVRAM operation
 *              enableConnection - whether the call ss wants to enable the
 * connection or release it.
 *
 * Returns:     Boolean
 *
 * Description:
 *
 * Design spec:  Interface to allow an external subsystem to get control of
 * a resource.  This fuction should be called before the resource is
 * configured.
 *
 * The request is classed as either a call control issue or a peripheral issue.
 * Call control refers to PSD and peripherals NVRAM/SIM/.
 *
 *-------------------------------------------------------------------------*/

Boolean  vgOpManUtilsEnableDisableConnection (const VgmuxChannelNumber entity,
                                               const ConnectionClass_t  connectionClass,
                                                const Boolean  enableConnection)
{
  Boolean                 accepted = FALSE;
  OpmanGenericContext_t*  opManGenericContext_p = ptrToOpManGenericContext ();

  if ( connectionClass < END_OF_CALL_CONTROL_CLASSES )
  {
    accepted = vgOpManModifyCcState (entity, connectionClass, enableConnection);
  }
  else
  {

    if ( enableConnection == TRUE )
    {
      if ( opManGenericContext_p->peripheralControlBuffer [connectionClass] == FALSE )
      {
        opManGenericContext_p->peripheralControlBuffer [connectionClass] = TRUE;
        accepted = TRUE;
      }
    }
    else
    {
      if ( opManGenericContext_p->peripheralControlBuffer [connectionClass] == TRUE )
      {
        opManGenericContext_p->peripheralControlBuffer [connectionClass] = FALSE;
        accepted = TRUE;
      }
    }
  }

  return (accepted);
}
/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

