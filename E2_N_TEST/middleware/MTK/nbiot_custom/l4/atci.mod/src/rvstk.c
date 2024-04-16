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
 * This module is the interface for the CI SIM Toolkit protocol functions.
 * The module receives IND and CNF signals from the CI task and processes
 * those signals. For IND signals the accessory will either be informed by
 * an unsolicited result code or the accessory will need to comply with the
 * STK protocol.
 **************************************************************************/

#define MODULE_NAME "RVSTK"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif
#if !defined (RVSTKSOT_H)
#  include <rvstksot.h>
#endif
#if !defined (RVSTK_H)
#  include <rvstk.h>
#endif
#if !defined (RVSTKPD_H)
#  include <rvstkpd.h>
#endif
#if !defined (RVOMTIME_H)
#  include <rvomtime.h>
#endif
#if !defined (RVSTKAT_H)
#  include <rvstkat.h>
#endif
#if !defined (RVSTKTP_H)
#  include <rvstktp.h>
#endif
#if !defined (RVUTIL_H)
#  include <rvutil.h>
#endif
#if !defined (RVSTKRNAT_H)
#  include <rvstkrnat.h>
#endif
#if !defined (RVSTKRD_H)
#  include <rvstkrd.h>
#endif
#if !defined (RVDATA_H)
#  include <rvdata.h>
#endif
#if !defined (ABSI_SIG_H)
#  include <absi_sig.h>
#endif
#if !defined (AFSA_SIG_H)
#  include <afsa_sig.h>
#endif
#if !defined (ABST_SIG_H)
#  include <abst_sig.h>
#endif
#if !defined (VGMX_SIG_H)
#  include <vgmx_sig.h>
#endif
#if !defined (RVOMAN_H)
#  include <rvoman.h>
#endif
#if !defined (RVCRHAND_H)
#  include <rvcrhand.h>
#endif
#if !defined (RVSTKCC_H)
#  include <rvstkcc.h>
#endif
#if !defined (GKIMEM_H)
#include <gkimem.h>
#endif
#include <rvcimxut.h>


/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  CiRunAtCommandInd        ciRunAtCommandInd;
  ApexSimOkInd             apexSimOkInd;
  ApexStDisplayAlphaIdInd  apexStDisplayAlphaIdInd;
  AfsaMenuSelectionCnf     afsaMenuSelectionCnf;
  AfsaListImageRecCnf      afsaListImageRecCnf;
  AfsaReadImageRecCnf      afsaReadImageRecCnf;
  AfsaReadImageInstDataCnf afsaReadImageInstDataCnf;
  ApexStCcStatusInd        apexStCcStatusInd;
  CiUserProfLoadedInd      ciUserProfLoadedInd;
};

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* Data store contains a mapping from AT commands to AT command functions.*/

static const AtCmdControl stkAtCommandTable[] =
{
  {ATCI_CONST_CHAR_STR "*MSTCR",   vgStkMSTCR,   VG_AT_STK_MSTCR,
    (AtCmdAccessMask) (AT_CMD_ACCESS_SIM_READY_AND_PRESENT)},
  {ATCI_CONST_CHAR_STR "*MSTEV",   vgStkMSTEV,   VG_AT_STK_MSTEV,
    (AtCmdAccessMask) (AT_CMD_ACCESS_SIM_READY_AND_PRESENT)},
  {ATCI_CONST_CHAR_STR "*MSTGC",   vgStkMSTGC,   VG_AT_STK_MSTGC,
    (AtCmdAccessMask) (AT_CMD_ACCESS_SIM_READY_AND_PRESENT)},
  {ATCI_CONST_CHAR_STR "*MSTMODE", vgStkMSTMODE, VG_AT_STK_MSTMODE, AT_CMD_ACCESS_NONE },
  {ATCI_CONST_CHAR_STR "*MSTMS",   vgStkMSTMS,   VG_AT_STK_MSTMS,
    (AtCmdAccessMask) (AT_CMD_ACCESS_SIM_READY_AND_PRESENT)},
  {ATCI_CONST_CHAR_STR "*MSTPD",   vgStkMSTPD,   VG_AT_STK_MSTPD,  AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MSTLOCK", vgStkMSTLOCK, VG_AT_STK_MSTLOCK, AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MSTICREC", vgStkMSTICREC, VG_AT_STK_MSTICREC,
    (AtCmdAccessMask) (AT_CMD_ACCESS_SIM_READY_AND_PRESENT)},
  {ATCI_CONST_CHAR_STR "*MSTICIMG", vgStkMSTICIMG, VG_AT_STK_MSTICIMG,
    (AtCmdAccessMask) (AT_CMD_ACCESS_SIM_READY_AND_PRESENT)},
  {PNULL,             PNULL,       VG_AT_LAST_CODE,  AT_CMD_ACCESS_NONE}
};

  
 #define MAX_IMAGE_INSTANCE_DATA (256)
 #define MAX_IMAGE_INSTANCES     (10)
 #define CS_COLOUR_VALUE         (1)
 #define CS_BASIC_VALUE          (0)
/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static void processProtocolSignalClass (const SignalBuffer signal,
                                         const StkCommandId commandId,
                                          const VgmuxChannelNumber entity);
static void processUnsolSignalClass (const SignalBuffer signal,
                                      const StkCommandId commandId,
                                       const VgmuxChannelNumber entity);
static void vgStkAfsaReadImageRecCnf (const SignalBuffer signal,
                                       const VgmuxChannelNumber entity);
static void vgStkAfsaReadImageInstDataCnf (const SignalBuffer signal,
                                            const VgmuxChannelNumber entity);
static void vgInitialiseStk (void);

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:    processProtocolSignalClass
 *
 * Parameters:  signal    - signal with protocol information
 *              commandId - command identifier
 *              entity    - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Handles the processing for the protocol class of
 *              signal indications.
 *
 *-------------------------------------------------------------------------*/

static void processProtocolSignalClass (const SignalBuffer signal,
                                         const StkCommandId commandId,
                                          const VgmuxChannelNumber entity)
{
  CommandLine_t *commandBuffer_p = PNULL;
  StkEntityGenericData_t *stkGenericContext_p = ptrToStkGenericContext ();

  KiAllocZeroMemory(    sizeof(CommandLine_t),
                        (void **)&commandBuffer_p);

  if (vgStkSetUpData (signal, commandId, entity) == TRUE)
  { 
    VgmuxChannelNumber printEntity = entity; 
    if(isEntityMmiNotUnsolicited(entity))
    {
      printEntity = vgGetMmiUnsolicitedChannel(); 
    }
 
    vgPutNewLine  (printEntity);
    vgPrintf      (printEntity, (const Char*)"*MSTC: %02u", commandId);
    vgPutNewLine  (printEntity);
    vgFlushBuffer (printEntity);

    if (stkGenericContext_p->responseTimerEnabled) 
    {
      vgCiStartTimer (TIMER_TYPE_STK_CNF_CONFIRM, entity);    
    }
  }
  else
  {
    (void) vgStkCreateSendRspSignal (commandBuffer_p, FALSE, entity);
  }

  KiFreeMemory( (void **)&commandBuffer_p);
}

/*--------------------------------------------------------------------------
 *
 * Function:    processUnsolSignalClass
 *
 * Parameters:  signal    - signal with protocol information
 *              commandId - command identifier
 *              entity    - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Handles the processing for the unsolicited class of
 *              signal indications.
 *
 *-------------------------------------------------------------------------*/

static void processUnsolSignalClass (const SignalBuffer signal,
                                      const StkCommandId commandId,
                                       const VgmuxChannelNumber entity)
{
  CommandLine_t *commandBuffer_p = PNULL;

  KiAllocZeroMemory(    sizeof(CommandLine_t),
                        (void **)&commandBuffer_p);

  if (vgStkSetUpData (signal, commandId, entity) == TRUE)
  {
  }

  /* received the signal, set up and sent the data, now
  send a terminal response to the SIM */
  commandBuffer_p->length = 0;

  (void) vgStkCreateSendRspSignal (commandBuffer_p, FALSE, entity);

  vgStkSetDataToInValid ();

  KiFreeMemory( (void **)&commandBuffer_p);
}

 /*--------------------------------------------------------------------------
 *
 * Function:    vgStkAfsaReadImageRecCnf
 *
 * Parameters:  signal    - signal with image record information
 *              entity    - mux channel number
 *
 * Returns:     nothing
 *
 * Description: displays selected image record information.
 *
 *-------------------------------------------------------------------------*/

static void vgStkAfsaReadImageRecCnf (const SignalBuffer       signal,
                                      const VgmuxChannelNumber entity)
{

  Int8                   instances;
  AfsaReadImageRecCnf    *readRecSig = PNULL;
  Int8                   instCount;
  Int8                   codingScheme;

  FatalAssert (getCommandId (entity) == VG_AT_STK_MSTICREC);
  readRecSig = &signal.sig->afsaReadImageRecCnf;

  if (readRecSig->requestStatus == SIM_REQ_OK )
  {
    instances = readRecSig->image.numOfInstances;
    if (instances > MAX_IMAGE_INSTANCES)
    {
      instances = MAX_IMAGE_INSTANCES;
    }
    vgPutNewLine  (entity);
    vgPrintf(entity, (const Char*) "*MSTICREC: %u, %u",readRecSig->recordNum, instances);
    vgPutNewLine  (entity);
    if (instances > 0 )
    {
      for (instCount = 0; instCount <instances; instCount++ )
      {
        if ( readRecSig->image.instance[instCount].codingScheme == SIM_IMAGE_COLOUR_CODING )
        {
          codingScheme = CS_COLOUR_VALUE;
        }
        else
        {
          codingScheme = CS_BASIC_VALUE;
        }    
        vgPrintf(entity, (const Char*) "*MSTICREC: %u,%u,%u,",readRecSig->image.instance[instCount].width, 
                                                              readRecSig->image.instance[instCount].height,
                                                              codingScheme );
        vgPut16BitHex ( entity, (Int16)(readRecSig->image.instance[instCount].efId) );
        vgPrintf( entity,(const Char*) ",%u,%u",readRecSig->image.instance[instCount].offset, 
                                                readRecSig->image.instance[instCount].dataLength ); 
        vgPutNewLine (entity);
      }
    }
    vgFlushBuffer (entity);
    setResultCode (entity, RESULT_CODE_OK);
  }
  else
  {
    setResultCode (entity, RESULT_CODE_ERROR);
  }
}
 /*--------------------------------------------------------------------------
 *
 * Function:    vgStkAfsaReadImageInstDataCnf
 *
 * Parameters:  signal    - signal with image record information
 *              entity    - mux channel number
 *
 * Returns:     nothing
 *
 * Description: displays selected image instance information.
 *
 *-------------------------------------------------------------------------*/

static void vgStkAfsaReadImageInstDataCnf (const SignalBuffer signal,
                                       const VgmuxChannelNumber entity)
{


  AfsaReadImageInstDataCnf    *readRecSig = PNULL;
  Int16                        count;
  Int16                        length;
  
  readRecSig = &signal.sig->afsaReadImageInstDataCnf;
  FatalAssert (getCommandId (entity) == VG_AT_STK_MSTICIMG);
  if (readRecSig->requestStatus == SIM_REQ_OK )
  {
    length = readRecSig->dataLength;
    if (length > MAX_IMAGE_INSTANCE_DATA)
    {
      length = MAX_IMAGE_INSTANCE_DATA;
    }
    /* write the length parameter */
    vgPutNewLine  (entity);
    vgPrintf(entity, (const Char*) "*MSTICIMG: %u, ",length);

    for ( count = 0; count < length; count++)
    {
        vgPut8BitHex(  entity, readRecSig->data[count]);
    }
    vgPutNewLine  (entity);
    vgFlushBuffer (entity);
    setResultCode (entity, RESULT_CODE_OK);
  }
  else
  {
    setResultCode (entity, RESULT_CODE_ERROR);
  }
}
    
/*--------------------------------------------------------------------------
 *
 * Function:    vgInitialiseStk
 *
 * Parameters:  none
 *
 * Returns:     nothing
 *
 * Description: Calls the initialisation functions.
 *
 *-------------------------------------------------------------------------*/
 static void vgInitialiseStk ()
{
  vgStkInitialiseStkUtil ();
  vgInitialiseStkData ();
  vgInitialiseStkAt ();
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/


/*--------------------------------------------------------------------------
 *
 * Function:    vgStkTimerExpiry
 *
 * Parameters:  entity  - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Function is called when the stk timer expires.  The timer
 *              is started when the accessory is informed that a proactive
 *              command has been received from the SIM AT task.  The timer
 *              is cancelled when the user sends the *MSTCR AT command.
 *
 *-------------------------------------------------------------------------*/

void vgStkTimerExpiry (const VgmuxChannelNumber entity)
{
  StkEntityGenericData_t *stkGenericContext_p = ptrToStkGenericContext ();
  CommandLine_t          *commandBuffer_p = PNULL;

  KiAllocZeroMemory(    sizeof(CommandLine_t),
                        (void **)&commandBuffer_p);

  vgStkSetDataToInValid ();

  /* set timeout to TRUE, this will send the tp off to the SIM */
  commandBuffer_p->length = 0;

  (void) vgStkCreateSendRspSignal (commandBuffer_p,
                                    TRUE,
                                     stkGenericContext_p->registeredStkEntity);

  KiFreeMemory( (void **)&commandBuffer_p);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgCiSTKToneTimerExpired
 *
 * Parameters:  entity  - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Timer expiry causes L1AM signal to be issued to cease playing
 *              the current tone. Unsolicted Result code *MSTTONE: 0 indicates
 *              AT*MSTCR can now be used to generate the Terminal Response.
 *              This is essential for STK test case 27.22.4.5.
 *
 *-------------------------------------------------------------------------*/

void vgCiSTKToneTimerExpired (const VgmuxChannelNumber entity)
{
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  PARAMETER_NOT_USED(entity);

  if (stkGenericContext_p->vgToneIsCallTone)
  {
    stkGenericContext_p->vgToneIsCallTone = FALSE;
  }
  else
  {
  }

  /* Send Unsol result code to indicate AT*MSTCR can now be used */
  vgPutNewLine  (entity);
  vgPrintf      (entity, (const Char*) "*MSTTONE: 0");
  vgPutNewLine  (entity);
  vgFlushBuffer (entity);
}



/*--------------------------------------------------------------------------
 *
 * Function:    vgStkInterfaceController
 *
 * Parameters:  signal_p - signal to be processed
 *              entity   - mux channel number
 *
 * Returns:     none
 *
 * Description: Handles the SIM Initialise signal when the SIM is inserted.
 *              Also, CNF signals following a regitration change and IND
 *              signals containing proactive data.  Two classes of IND signals
 *              are handle, unsolicited and proactive.  The MMI AFSA
 *              interface signals use the stk protocol and the others just
 *              send an usolicited result to the accessory.
 *
 *-------------------------------------------------------------------------*/

Boolean vgStkInterfaceController (const SignalBuffer *signal_p,
                                   const VgmuxChannelNumber entity)
{
  Boolean            accepted = FALSE;
  StkCommandId       commandId;
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();
  
  switch (*signal_p->type)
  {
    case SIG_APEX_SIM_OK_IND:
    {
      if (signal_p->sig->apexSimOkInd.proactiveSim == TRUE)
      {
        stkGenericContext_p->modemSupportingStk = TRUE;
      }
      else
      {
        stkGenericContext_p->modemSupportingStk = FALSE;  
         
        if(isEntityMmiNotUnsolicited(entity))
        {
          vgPutNewLine  (vgGetMmiUnsolicitedChannel());
          vgPrintf      (vgGetMmiUnsolicitedChannel(),
                         (const Char*)"*MSTC: %u",
                         STK_NOT_PROACTIVE);
          vgPutNewLine  (vgGetMmiUnsolicitedChannel());
          vgSetCirmDataIndIsUrc(vgGetMmiUnsolicitedChannel(), TRUE);
          vgFlushBuffer (vgGetMmiUnsolicitedChannel());
        }
        else
        {
          /* indicate SIM is not proactive */
          vgPutNewLine  (entity);
          vgPrintf      (entity,
                         (const Char*)"*MSTC: %u",
                         STK_NOT_PROACTIVE);
          vgPutNewLine  (entity);
          vgSetCirmDataIndIsUrc(entity, TRUE);
          vgFlushBuffer (entity);
        }
      }
      stkGenericContext_p->simHasPassedPinCheck = TRUE;

      /* Not accepted since another sub-system also processes the signal */
      break;
    }
    case SIG_AFSA_ST_REGISTER_TASK_CNF:
    {
      /* only receive this for the registered entity */
      vgStkAfsaStRegisteredTaskCnf (*signal_p, entity);
      accepted = TRUE;
      break;
    }
    case SIG_AFSA_MMI_PROFILE_UPDATE_CNF :
    {
      vgStkAfsaMmiProfileUpdateCnf (*signal_p, entity);
      accepted = TRUE;
      break;  
    }

    case SIG_AFSA_READ_MMI_PROFILE_CNF :
    {
      vgStkAfsaReadMmiProfileCnf (*signal_p, entity);
      accepted = TRUE;
      break;
    }              
    /* P4 Job # 117871 Fix for a CMCC sim card that has a duff menu select item */
    case SIG_AFSA_MENU_SELECTION_CNF:
    {
      /* End the proactive session if it hasn't started after the menu selection */
      if (vgOpManStkOnLine (entity) == TRUE) 
      {
        stkGenericContext_p->proactiveSessionStarted = 
                    signal_p->sig->afsaMenuSelectionCnf.proactiveSessionStarted;
        if (signal_p->sig->afsaMenuSelectionCnf.proactiveSessionStarted == FALSE)
        {  
          if(isEntityMmiNotUnsolicited(entity))
          {
            vgPutNewLine  (vgGetMmiUnsolicitedChannel());
            vgPrintf      (vgGetMmiUnsolicitedChannel(),
                          (const Char*)"*MSTC: %u",
                          STK_END_OF_TRANSACTION);
            vgPutNewLine  (vgGetMmiUnsolicitedChannel());
            vgFlushBuffer (vgGetMmiUnsolicitedChannel());
          }
          else
          {
            /* indicate SIM is not proactive */
            vgPutNewLine  (entity);
            vgPrintf      (entity,
                          (const Char*)"*MSTC: %u",
                          STK_END_OF_TRANSACTION);
            vgPutNewLine  (entity);
            vgFlushBuffer (entity);
          }  
        }
      }
      accepted = TRUE;
      break;
    }

    case SIG_AFSA_READ_IMAGE_REC_CNF:
    {
      vgStkAfsaReadImageRecCnf (*signal_p, entity); 
      accepted = TRUE;
      break;
    } 
           
    case SIG_AFSA_LIST_IMAGE_REC_CNF:
    {
      FatalAssert (getCommandId (entity) == VG_AT_STK_MSTICREC);
      vgPutNewLine  (entity);
      if ( signal_p->sig->afsaListImageRecCnf.requestStatus == SIM_REQ_OK)
      {
        vgPrintf      (entity,
                      (const Char*)"*MSTICREC: %u",
                       signal_p->sig->afsaListImageRecCnf.listSize);
        vgPutNewLine  (entity);
        setResultCode (entity, RESULT_CODE_OK);
      }
      else
      {
        setResultCode (entity, RESULT_CODE_ERROR);
      }  
      accepted = TRUE;
      break;    
    }
            
    case SIG_AFSA_READ_IMAGE_INST_DATA_CNF:
    {
      vgStkAfsaReadImageInstDataCnf (*signal_p, entity);
      accepted = TRUE;
      break;
    }

    /* special case for RUN AT COMMAND proactive command - 
     * not a protocol signal or an unsolicited signal - 
     * unsolicted info to the MMI and then the AT command
     * needs to be run within the ATCI and the result of this sent back to the
     * SIMAT */
    case SIG_AFSA_RUN_AT_COMMAND_IND :
    {
      if (vgOpManStkOnLine (entity) == TRUE)
      {
        vgAfsaRunAtCommandInd (*signal_p, entity);
        accepted = TRUE;
      }
      break;
    } 
    /* this signal is used to establish when the STK entity has enabled a channel so it can run an 
     * at command within a proactive command  */
    case SIG_CI_USER_PROF_LOADED_IND:
    {
      if (( stkGenericContext_p->runAtCmdState == STATE_ENABLING_CHANNEL) &&
      (stkGenericContext_p->atCommandData.cmdEntity == entity))  
      {
         setTaskInitiated(stkGenericContext_p->atCommandData.cmdEntity, TRUE);
         stkGenericContext_p->runAtCmdState = STATE_CHANNEL_ENABLED;
         vgStkProcessProactiveCommandRunAtCommand();
      }
      break;
    } 

    case SIG_APEX_ST_PROACTIVE_SESSION_END_IND:
    {
      /* we only process this event if were registered and this is the
         registered entity */
      if (vgOpManStkOnLine (entity) == TRUE)
      {
        stkGenericContext_p->proactiveSessionStarted = FALSE;
        
        if(isEntityMmiNotUnsolicited(entity))
        { 
          vgPutNewLine  (vgGetMmiUnsolicitedChannel());
          vgPrintf      (vgGetMmiUnsolicitedChannel(),
                         (const Char*)"*MSTC: %u",
                         STK_END_OF_TRANSACTION);
          vgPutNewLine  (vgGetMmiUnsolicitedChannel());
          vgFlushBuffer (vgGetMmiUnsolicitedChannel());
        }
        else
        {
          /* indicate SIM is not proactive */
          vgPutNewLine  (entity);
          vgPrintf      (entity,
                         (const Char*)"*MSTC: %u",
                         STK_END_OF_TRANSACTION);
          vgPutNewLine  (entity);
          vgFlushBuffer (entity);
        }   
        
        /* as the proactve session is ended we need to check if there was a 
         * channel enabled for internal RUN_AT_COMMAND proactive commands
         * if there is then it should be freed now*/
        if (( stkGenericContext_p->runAtCmdState == STATE_CHANNEL_ENABLED) &&
        ( stkGenericContext_p->atCommandData.cmdEntity != VGMUX_CHANNEL_INVALID)) 
        {
          vgStkDisableReservedEntity (entity, FALSE);   
        }
        accepted = TRUE;
      }
      break;
    }

    case SIG_APEX_ST_CC_STATUS_IND:  /* check for call control modifications */
    {
      if (vgOpManStkOnLine (entity) == TRUE)
      {
        vgStkCheckCallControlStatus(*signal_p, entity );
        accepted = TRUE;   /* only this sub-system processes this signal */
      }
      break;
    }

    case SIG_INITIALISE:
    {
      vgInitialiseStk ();
      break;
    }
    case SIG_CI_RUN_AT_COMMAND_IND:
    {
      /* process all commands, leave it to the handers to validate the data */
      accepted = parseCommandBuffer (stkAtCommandTable, entity);
      break;
    }
#if defined(UPGRADE_MTNET)
    case SIG_CI_RESET_IND:
#endif
    case SIG_CI_CFUN_SET_IND:
    {
      /* need to re-initialise any terminal profile data  if we are in a 
       * power-down state */
      vgReinitialiseStk();
      break;
    }
            
    default: /*  handle all the IND signals */
    {
      if (vgStkIsDataValid () == TRUE)
      {
        /* we will receive an indication and the next IND should only be
           sent after we have responded so this should never happen */
      }
      else
      {
        /* must have sent the TP and received the SIM initialise signals */
        if (vgOpManStkOnLine (entity) == TRUE)
        {
          switch (vgStkSignalToCmdId (*signal_p, &commandId))
          {
            case STK_PROTOCOL_REQ:
            {
              processProtocolSignalClass (*signal_p, commandId, entity);
              break;
            }
            case STK_UNSOLICITED_REQ:
            {
              processUnsolSignalClass (*signal_p, commandId, entity);
              break;
            }
            case STK_DISPLAY_ALPHA_ID_REQ:
            {
              if ((signal_p->sig->apexStDisplayAlphaIdInd.type == SIMAT_OP_ALPHA_ID) ||
                  (signal_p->sig->apexStDisplayAlphaIdInd.type == SIMAT_SD_ALPHA_ID) ||
              (signal_p->sig->apexStDisplayAlphaIdInd.type == SIMAT_SE_ALPHA_ID) ||
              (signal_p->sig->apexStDisplayAlphaIdInd.type == SIMAT_RE_ALPHA_ID))
              {
                processProtocolSignalClass (*signal_p, commandId, entity);
              }
              else
              {
                processUnsolSignalClass (*signal_p, commandId, entity);
              }
              break;
            }
            default: /* nothing to do */
            {
              break;
            }
          }
        }
      }
      break;
    }
  }

  return (accepted);
}




/* END OF FILE */

