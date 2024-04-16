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
 * The cimux is responsible for dealing with all the AT input from the mux.
 * This sub-system includes an Rx singular queue. Also, handles all the
 * code for registration of un-solicited signals.
 **************************************************************************/

#define MODULE_NAME "RVCIMUX"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <gkisig.h>
#include <gkimem.h>
#include <rvsystem.h>
#include <vgmx_sig.h>
#include <rvutil.h>
#include <rvcimux.h>
#include <rvoman.h>
#include <rvchman.h>
#include <rvcmux.h>
#include <rvtssigo.h>
#include <rvccut.h>
#include <rvcimxms.h>
#include <rvgncpb.h>
#include <rvstkrnat.h>
#include <rvcimxsot.h>
#include <rvcimxut.h>
#include <rvomtime.h>
#if defined(DEVELOPMENT_VERSION)
#include <rvtsut.h>
#endif
#include <ciapex_sig.h>
#include <vgmxatq.h>
#include <rvcrhand.h>
#include <rvccsigi.h>

#include <rvgput.h>
#include <rvpdsigo.h>
#include <rvaput.h>

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

#define UPPER_A   ('A')
#define LOWER_A   ('a')
#define UPPER_T   ('T')
#define LOWER_T   ('t')
#define FSLASH    ('/')

/* Mux command information table */
static const AtCmdControl mxAtCommandTable[] =
{
  {ATCI_CONST_CHAR_STR "+CMUX",    vgProcCMUX,    VG_AT_CMUX,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MMUX",    vgProcMMUX,    VG_AT_MMUX,      AT_CMD_ACCESS_NONE},
  {PNULL,                   PNULL,         VG_AT_LAST_CODE, AT_CMD_ACCESS_NONE}
};

/***************************************************************************
 * Signal definitions
 ***************************************************************************/
union Signal
{
    CirmDataInd                 cirmDataInd;
    CiRunAtCommandInd           ciRunAtCommandInd;
    CiDataEntryModeInd          ciDataEntryModeInd;
    CiMuxChannelEnableInd       ciMuxChannelEnableInd;
    CiMuxAtDataInd              ciMuxAtDataInd;
    CiMuxAtDataRsp              ciMuxAtDataRsp;
    CiMuxCheckCmuxCmdParamsCnf  ciMuxCheckCmuxCmdParamsCnf;
    CiMuxConfigureMuxCnf        ciMuxConfigureMuxCnf;
    CiMuxOpenDataConnCnf        ciMuxOpenDataConnCnf;
    CiMuxReadCmuxCmdParamsCnf   ciMuxReadCmuxCmdParamsCnf;
    CiMuxStart27010MuxReq       ciMuxStart27010MuxReq;
    CiMuxSwitchedToCmdModeInd   ciMuxSwitchedToCmdModeInd;
    CiMuxClosedDataConnInd      ciMuxClosedDataConnInd;
    CiMuxVersionCnf             ciMuxVersionCnf;
    VgCiSsRegistrationInd       vgCiSsRegistrationInd;
};

/***************************************************************************
 * Types
 ***************************************************************************/

typedef SolicitedSignalRecord_t SolicitedBuffer [SOLICITED_MAX_NUM_ITEMS];

/***************************************************************************
 * Variables
 ***************************************************************************/
TaskId vgDataTaskId = MUX_TASK_ID;

static SolicitedBuffer solicitedBuffer [CI_MAX_ENTITIES];

/***************************************************************************
 * Macros
 ***************************************************************************/

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static void processMuxInput (const SignalBuffer *signal_p,
                                            const VgmuxChannelNumber entity);
static ScanResults_t scan (const VgmuxChannelNumber entity,
                                            Char  *atStringToScan,
                                            Int16  *atLength);
static Boolean checkPreemptiveCmd (const VgmuxChannelNumber entity);
static void executeCommand (const VgmuxChannelNumber entity);
static Boolean flushTheCache (const VgmuxChannelNumber entity);
static void doSsRegistrationInd (const SignalBuffer *signal_p);
static void muxDataAvailable (const SignalBuffer *signal_p);
static Boolean processEnableEntitySignal (const VgmuxChannelNumber entity);

static void initialiseEntities             (void);
static void addToAtCache                   (const VgmuxChannelNumber  entity,
                                             SignalBuffer targetQueue [],
                                              Int8 *targetQueueLength,
                                               const SignalBuffer *signalBuffer_p);

static void cimuxPerformNextAction  (const VgmuxChannelNumber entity);

static void initialiseCimuxData     (const VgmuxChannelNumber entity);

static void resetTheScanBuffer      (const VgmuxChannelNumber entity);

extern void ackDataInd (const VgmuxChannelNumber entity);

extern Boolean vgCcAutoAnswer (const VgmuxChannelNumber entity,
                               VgmuxChannelNumber* runEntity);
#ifndef NO_EMMI_INTERFACE
extern void KiSetEmmiLogByPass(Boolean bypass);
#endif

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/****************************************************************************
 *
 * Function:    resetTheScanBuffer
 *
 * Parameters:  VgmuxChannelNumber entity
 *
 * Returns:     Nothing
 *
 * Description: This function is called when an AT command completes and we
 * need to reset the AT scan buffer.  This will be called on the following
 * conditions:
 * a) When the current AT completes and there is no more data in the string
 * process.
 * b) If the AT command is too long - as determined by the CR terminator
 * c) If the command separator is not correct and there is more data
 * d) If any command completes with an error result code
 *
 ****************************************************************************/
static void resetTheScanBuffer (const VgmuxChannelNumber entity)
{
  ScanParseContext_t *scanParseContext_p = ptrToScanParseContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (scanParseContext_p != PNULL, entity, 0, 0);
#endif
#if defined(ATCI_ENABLE_DYN_AT_BUFF)
  if (scanParseContext_p->nextCommand.character != PNULL)
  {
    memset (&scanParseContext_p->nextCommand.character[0],
            NULL_CHAR,
             COMMAND_LINE_SIZE + NULL_TERMINATOR_LENGTH);
  }    
#else
  memset (&scanParseContext_p->nextCommand.character[0],
          NULL_CHAR,
           sizeof (scanParseContext_p->nextCommand.character));
#endif
  scanParseContext_p->nextCommand.length = 0;
  scanParseContext_p->nextCommand.position = 0;

#if defined(ATCI_ENABLE_DYN_AT_BUFF)
  /* Now free the AT command buffer */
  vgFreeAtCmdBuffer(entity);
#endif  
}


/****************************************************************************
 *
 * Function:    processMuxInput
 *
 * Parameters:  SignalBuffer  *signal_p
 *              VgmuxChannelNumber entity
 *
 * Returns:     Nothing
 *
 * Description: Process the supplied data string.  Check for the CR and back
 *              space chars.
 *              Validates the command ON receipt of the CR, checks for
 *                AT<cr> -- output OK
 *                A<cr>, any error sequence  -- output error
 *                AT+<valid seq> -- run comd
 *                <cr>           -- just ignore it
 *              The at string is shifted so that the AT prefx is removed before
 *              being sent to the command parsers.
 *              When the signal has been completely decoded then we
 *              destroy it.
 *
 ****************************************************************************/

static void processMuxInput (const SignalBuffer *signal_p,
                              const VgmuxChannelNumber entity)
{
  ChannelContext_t    *channelContext_p   = ptrToChannelContext (entity);
  ScanParseContext_t  *scanParseContext_p = ptrToScanParseContext (entity);
  Int16               *scanLength = &scanParseContext_p->nextCommand.length;
  Boolean             foundCR = FALSE;


  Int32               index;
  CiMuxAtDataInd      *ciMuxAtDataInd = &signal_p->sig->ciMuxAtDataInd;
#if defined (ENABLE_LONG_AT_CMD_RSP)
  /* TODO: Store echoString in global data */
#endif  
  Char                echoString[CIMUX_MAX_AT_DATA_LENGTH] = {0};
  Int16               echoLength = 0;
  Int8                charCode;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (scanParseContext_p != PNULL, entity, 0, 0);
  FatalCheck (channelContext_p != PNULL, entity, 0, 0);
#endif
  /* scan all the chars in the signal unless we find the CR if a CR is found then
   * shift the data down to the start of the string */

  for (index = 0;
       (index < ciMuxAtDataInd->length) &&(foundCR == FALSE);
        index++)
  {
    charCode = ciMuxAtDataInd->data[index];

    /* if we have exceeded the max length of a command string then reset */
    if (*scanLength == COMMAND_LINE_SIZE)
    {
      *scanLength = 0;
      /* clear up memory */
      resetTheScanBuffer (entity);
      echoChar (echoString, charCode, &echoLength, entity);
    }
    else
    {

      if ((applyDataMask (charCode, entity) == TRUE)
          || (charCode == getProfileValue (entity, PROF_S5))
          || (charCode == getProfileValue (entity, PROF_S3)))
      {      
        if (charCode == getProfileValue (entity, PROF_S3)) /* is this ch a CR char */
        {
#if defined(ATCI_ENABLE_DYN_AT_BUFF)
         if (scanParseContext_p->nextCommand.character == PNULL)
         {
           vgAllocAtCmdBuffer(entity);
         }
#endif         
          scanParseContext_p->nextCommand.character[scanParseContext_p->nextCommand.length++] = charCode;

          setCommandId      (entity, VG_AT_NO_COMMAND);
          setCommandName    (entity, (Char *) "");
          setConnectionType (entity, CONNECTION_TERMINATOR);
          setTaskInitiated  (entity, FALSE);
          /* scan the data and amend to remove and data that prefixes the command */
          scanParseContext_p->scanResult = scan ( entity,
           scanParseContext_p->nextCommand.character,
            &scanParseContext_p->nextCommand.length );


          /* only echo <cr> when used in conjunction with other characters */
          if (scanParseContext_p->scanResult != ONLY_CR_FOUND)
          {
            echoChar (echoString,
                       getProfileValue (entity, PROF_S3),
                        &echoLength,
                         entity);

            foundCR = TRUE;  /* handled the CR so thats it for now */
          }

          /* parsed the command, scan it so lets run it */
          executeCommand (entity);
        }        
        /* is this a backspace char */
        else if ((charCode == getProfileValue (entity, PROF_S5))
            || (charCode == getProfileValue (entity, PROF_S4)))
        {
 
          if (scanParseContext_p->nextCommand.length > 0)
          {
            echoChar (echoString,
                       getProfileValue (entity, PROF_S5),
                        &echoLength,
                         entity);
            echoChar (echoString,
                       SPACE_CHAR,
                        &echoLength,
                         entity);
            echoChar (echoString,
                       getProfileValue (entity, PROF_S5),
                        &echoLength,
                         entity);

            scanParseContext_p->nextCommand.length--;
            scanParseContext_p->nextCommand.
                character[scanParseContext_p->nextCommand.length] = NULL_CHAR;

#if defined(ATCI_ENABLE_DYN_AT_BUFF)
            /* Have we now deleted the entire string? */
            if (scanParseContext_p->nextCommand.length == 0)
            {
              resetTheScanBuffer (entity);
            }
#endif            
          }
        }
        else /* all other chars including the ; */
        {
          echoChar (echoString, charCode, &echoLength, entity);

          {
#if defined (ATCI_ENABLE_DYN_AT_BUFF)
            if (*scanLength == 0)
            {
              /* We are adding the first character - so we must allocate the buffer for AT command here
               */
              vgAllocAtCmdBuffer(entity);
            }
#endif
            scanParseContext_p->nextCommand.character[*scanLength] = charCode;
            *scanLength += 1;
          }
        }
      }
    } /* IF we have space */
  }

  /* deal with the signal on the q.  if we have processed all the data in the signal
     then pop it otherwise shift the data down. */
  if (index == ciMuxAtDataInd->length)
  {
    /* this signal is now empty */
    popFromAtCache (&channelContext_p->at.fromMuxCache [0],
                      &channelContext_p->at.fromMuxCacheCount);
    if(entity == 0)
    {
        if((channelContext_p->actDataIndNoReply)&&(channelContext_p->at.fromMuxCacheCount< CIVAL_CIMUX_AT_QUEUE_FLOWCONTROL_DEACTIVE_SIZE))
        {
            channelContext_p->actDataIndNoReply = FALSE;
            ackDataInd (entity);                  
        }
    }

  }
  else
  {
    /* we need to shuffle the data in this signal down */
    memcpy (&ciMuxAtDataInd->data[0],
             &ciMuxAtDataInd->data[index],
              ciMuxAtDataInd->length - index);
    ciMuxAtDataInd->length = ciMuxAtDataInd->length - index;
  }

  if (echoLength > 0)
  {
    sendEchoDataToCrm (echoString, echoLength, entity);
  }
}


/****************************************************************************
 *
 * Function:    scan
 *
 * Parameters:  VgmuxChannelNumber entity
 *              Char  *atStringToScan
 *              Int16  *atLength
 *
 * Returns:     Nothing
 *
 * Description: This function validates the AT command string after a CR has
 * been detected.  Checks for the following cases:
 *  a) Just a CR in the first byte (result is that CI ignores it)
 *  b) An AT sequence of the same case
 *      - Just AT (result is OK is returned)
 *      - AT+ (result is that the command parsers are called)
 *  c) An error in the sequence.
 *
 * Function checks all the bytes looking for a valid AT command.
 * If "aTaTAT" is supplied then the last AT will generate an OK
 * result code.
 *
 ****************************************************************************/

static ScanResults_t scan ( const VgmuxChannelNumber entity,
                             Char  *atStringToScan,
                              Int16  *atLength)
{
  ScanParseContext_t *scanParseContext_p = ptrToScanParseContext (entity);

  ScanResults_t  scanResult = AWAITING_A;
  Boolean        runCommand = FALSE;
//  Int16          index = 0;
//  Int16          atStartPos = 0;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (scanParseContext_p != PNULL, entity, 0, 0);
#endif
  if (atStringToScan [0] == (Char)getProfileValue (entity, PROF_S3))
  {
    return ONLY_CR_FOUND;
  }
  else
  {
      if(((scanParseContext_p->nextCommand.character[0]!=LOWER_A)&&(scanParseContext_p->nextCommand.character[0]!=UPPER_A))
        ||((scanParseContext_p->nextCommand.character[1]!=UPPER_T)&&(scanParseContext_p->nextCommand.character[1]!=LOWER_T)))
      {
          return AWAITING_A;
      }
      else if (scanParseContext_p->nextCommand.character[2] == (Char)getProfileValue (entity, PROF_S3))
      {
          return ONLY_AT_FOUND;
      }
      else
      {
       /* we need to shuffle the AT data to remove the AT prefix */
          scanParseContext_p->nextCommand.length = scanParseContext_p->nextCommand.length -2;
          memcpy (&scanParseContext_p->nextCommand.character[0],
                 &scanParseContext_p->nextCommand.character[2],
                  scanParseContext_p->nextCommand.length);

          scanParseContext_p->nextCommand.character[scanParseContext_p->nextCommand.length]=NULL_CHAR;
          scanParseContext_p->nextCommand.character[scanParseContext_p->nextCommand.length+1]=NULL_CHAR;
          scanResult = RUN_AT_COMMAND;
      }
    }

  return (scanResult);

}


/****************************************************************************
 *
 * Function:    checkPreemptiveCmd
 *
 * Parameters:  VgmuxChannelNumber  -  entity
 *
 * Returns:     Boolean : whether a preemptive command is found
 *
 * Description:
 *
 ****************************************************************************/
static Boolean checkPreemptiveCmd (const VgmuxChannelNumber entity)
{
  ChannelContext_t    *channelContext_p     = ptrToChannelContext (entity);
  ScanParseContext_t  *scanParseContext_p   = ptrToScanParseContext (entity);
  Boolean             foundCmd              = FALSE;
  Int16               tempStringLength      = 0;
  Char                tempString[STRING_LENGTH_40];
  Int16               index, cmdStrLength;
  Int8                *charCode;
  CiMuxAtDataInd      *ciMuxAtDataInd;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (channelContext_p != PNULL, entity, 0, 0);
  FatalCheck (scanParseContext_p != PNULL, entity, 0, 0);
#endif
  channelContext_p->canRunMabortCmd = FALSE;

  ciMuxAtDataInd = &(channelContext_p->at.fromMuxCache[0].sig->ciMuxAtDataInd);

  if (scanParseContext_p->nextCommand.length + ciMuxAtDataInd->length <= STRING_LENGTH_40)
  {
    memcpy (&(tempString[0]), scanParseContext_p->nextCommand.character, scanParseContext_p->nextCommand.length);
    memcpy (&(tempString[scanParseContext_p->nextCommand.length]), ciMuxAtDataInd->data, ciMuxAtDataInd->length);
    tempStringLength = scanParseContext_p->nextCommand.length + ciMuxAtDataInd->length;
  }
  else if (scanParseContext_p->nextCommand.length >= STRING_LENGTH_40)
  {
    memcpy (&(tempString[0]), scanParseContext_p->nextCommand.character, STRING_LENGTH_40);
    tempStringLength = STRING_LENGTH_40;
  }
  else
  {
    memcpy (&(tempString[0]), scanParseContext_p->nextCommand.character, scanParseContext_p->nextCommand.length);
    memcpy (&(tempString[scanParseContext_p->nextCommand.length]), ciMuxAtDataInd->data, STRING_LENGTH_40 - scanParseContext_p->nextCommand.length);
    tempStringLength = STRING_LENGTH_40;
  }

  cmdStrLength = vgStrLen ("AT*MABORT");

  for (index = 0;
       (index < tempStringLength - cmdStrLength) && (foundCmd == FALSE);
        index++)
  {
    charCode = &(tempString[index]);
    if ((charCode[cmdStrLength] == getProfileValue (entity, PROF_S3)) &&
        (vgStrNCmp(charCode, "AT*MABORT", cmdStrLength) == 0))
    {
      /* Only AT+COPS can be aborted with AT*MABORT */
      if (getCommandId (entity) == VG_AT_MM_COPS)
      {
        foundCmd = TRUE;
        channelContext_p->canRunMabortCmd = TRUE;
      
      }
    }
  }

  return foundCmd;
}

/****************************************************************************
 *
 * Function:    VgmuxChannelNumber
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description: This command is used to run a command based on the type of command.
 * The type has been generated by the scan function.  Types of command are:
 *  <CR>   Just a carridge return, we need do nothing
 *  AT     An AT is ok so we just return OK
 *  ERROR  Its and error case, no AT found.
 *  AT+    AT with some data, we need to run this so we send it to the parsers.
 *
 ****************************************************************************/

static void executeCommand (const VgmuxChannelNumber entity)
{
  ScanParseContext_t *scanParseContext_p = ptrToScanParseContext (entity);
  SignalBuffer       signalBuffer = kiNullBuffer;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (scanParseContext_p != PNULL, entity, 0, 0);
#endif
  switch (scanParseContext_p->scanResult)
  {
    case RUN_AT_COMMAND: /* an AT command to run */
    {
      /* don't need to register signal because the entity is contained within
       * the signal  */
      KiCreateSignal (SIG_CI_RUN_AT_COMMAND_IND,
                           sizeof (CiRunAtCommandInd),
                            &signalBuffer);

      signalBuffer.sig->ciRunAtCommandInd.entity = entity;
      signalBuffer.sig->ciRunAtCommandInd.activeAtCommand.length = scanParseContext_p->nextCommand.length;
      signalBuffer.sig->ciRunAtCommandInd.activeAtCommand.position = scanParseContext_p->nextCommand.position;

      /* Only copy first 200 characters of AT command to be executed */
      memcpy (&signalBuffer.sig->ciRunAtCommandInd.activeAtCommand.character,
               &scanParseContext_p->nextCommand.character,
               SHORT_COMMAND_LINE_SIZE);

      KiSendSignal (VG_CI_TASK_ID, &signalBuffer);

      setEntityState (entity, ENTITY_PROCESSING);

      /* job106314: need to store command type (basic or extended syntax) */
      /* for use when parsing possible next command in a concatenated */
      /* command string */
      if ((scanParseContext_p->nextCommand.character[0] == PLUS_CHAR) ||
          (scanParseContext_p->nextCommand.character[0] == STAR_CHAR) ||
          (scanParseContext_p->nextCommand.character[0] == HAT_CHAR))
      {
        scanParseContext_p->currentCommandType = EXTENDED_SYNTAX;
      }
      else
      {
        scanParseContext_p->currentCommandType = BASIC_SYNTAX;
      }
      break;
    }
    case ONLY_CR_FOUND: /* just a CR */
    {
      setEntityState (entity, ENTITY_IDLE);
      scanParseContext_p->nextCommand.length = 0;

#if defined (ATCI_ENABLE_DYN_AT_BUFF)
      resetTheScanBuffer(entity);
#endif
      break;
    }
    case ONLY_AT_FOUND: /* just an AT */
    {
      setEntityState (entity, ENTITY_RUNNING);
      setResultCode (entity, RESULT_CODE_OK);
      scanParseContext_p->nextCommand.length = 0;
#if defined (ATCI_ENABLE_DYN_AT_BUFF)
      resetTheScanBuffer(entity);
#endif
      break;
    }
    default: /* error  */
    {
      setEntityState (entity, ENTITY_RUNNING);
      setResultCode (entity, RESULT_CODE_ERROR);
      scanParseContext_p->nextCommand.length = 0;
#if defined (ATCI_ENABLE_DYN_AT_BUFF)
      resetTheScanBuffer(entity);
#endif
      break;
    }
  }
}


/****************************************************************************
 *
 * Function:    flushTheCache
 *
 * Parameters:  SignalBuffer       *signal_p
 *
 * Returns:     Boolean : whether the cache is empty or not
 *
 * Description:  Given an entity id check if the entity has finished processing
 * the current AT command.  If it has then get the next command, either from
 * the AT command string or from the cache.
 *
 ****************************************************************************/

static Boolean flushTheCache (const VgmuxChannelNumber entity)
{
  ChannelContext_t   *channelContext_p   = ptrToChannelContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (channelContext_p != PNULL, entity, 0, 0);
#endif
  /* are there signals in the entities cache */
  if (channelContext_p->at.fromMuxCacheCount != 0)
  {
    do
    {
      processMuxInput (&channelContext_p->at.fromMuxCache[0], entity);
    }
    while ((getEntityState (entity) != ENTITY_PROCESSING) &&
           (channelContext_p->at.fromMuxCacheCount != 0));
  }

  return ((Boolean)(channelContext_p->at.fromMuxCacheCount == 0));
}

/****************************************************************************
 *
 * Function:    doSsRegistrationInd
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 * Description: This function is used to register a signal with the registration
 * lookup table. The table records signal types for all entities.  When a solicted
 * signal arrives at the Voyager task then the table is checked to see if the signal
 * has been registered. The table records the sub-system id so that we know where to
 * send the signal.
 *
 ****************************************************************************/

static void doSsRegistrationInd (const SignalBuffer *signal_p)
{
  VgCiSsRegistrationInd signal = signal_p->sig->vgCiSsRegistrationInd;

  Int8 index = 0;

  /*
   * Find the next slot.
   */
  while ((solicitedBuffer [signal.entity][index].type != 0) &&
         (index < SOLICITED_MAX_NUM_ITEMS))
  {
    index += 1;
  }

  /* all elements used so we should flag this */
  FatalAssert ( index != SOLICITED_MAX_NUM_ITEMS );

  /*
   * Set index to last point in array if there is no room - this will overwrite the last value.
   */
  if (index >= SOLICITED_MAX_NUM_ITEMS)
  {

    index = SOLICITED_MAX_NUM_ITEMS-1;
  }

  /* if there is not enough room then all we can do is just overwrite the last */
  solicitedBuffer [signal.entity][index].type   = signal.signalId;
  solicitedBuffer [signal.entity][index].ssCode = signal.ssCode;

#if defined(DEVELOPMENT_VERSION)
  vgTsDoubleTypeCheck (signal.entity);
#endif
}

/*--------------------------------------------------------------------------
 *
 * Function:    processOpenDataConnCnf.
 *
 * Parameters:  entity  - mux channel number.
 *              Boolean - Open Data Connection Confirmation
 *
 * Returns:     Boolean - success of processing mux data connection
 *
 * Description: Processing of Open Data Connection Confirmation signal.
 *
 *-------------------------------------------------------------------------*/
static Boolean  processOpenDataConnCnf (const VgmuxChannelNumber entity,
                                        const Boolean            success)
{
#ifdef ENABLE_AP_BRIDGE_FEATURE
  ApBridgeDataModeContext_t* apBridgeDataModeContext_p = vgApbGetDataModeContext(entity);
  if ((getCommandId(entity) > (VG_AT_AP_BRIDGE_BASE - 1))
       && (getCommandId(entity) < (VG_AT_AP_BRIDGE_END + 1))
       && (apBridgeDataModeContext_p != NULL))
  {
    /*The command is for AP Bridge.*/
    if (success == TRUE)
    {
      apBridgeDataModeContext_p->dataModeState = RV_APB_DATA_MODE_ACTIVATED;
    }
    apBridgeDataModeContext_p->pendingOpenDataConnCnf = FALSE;
  }
  else
#endif
  {
    GprsContext_t *gprsContext_p = ptrToGprsContext (entity);
    CallContext_t *callContext_p = ptrToCallContext (entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
    FatalCheck (callContext_p != PNULL, entity, 0, 0);
#endif
    if (success == FALSE)
    {
       if (gprsContext_p->connectionActive == TRUE)
       {
         gprsContext_p->connectionActive = FALSE;
       }
    }
    /* Check if we were waiting for this in order to send a response back to
     * ABPD
     */
    if (gprsContext_p->pendingOpenDataConnCnf == TRUE)
    {
      if (success == TRUE)
      {
        vgApexAbpdConnectRsp (entity);
      }
      else
      {
        vgApexAbpdConnectRej (entity);
      }
      gprsContext_p->pendingOpenDataConnCnf = FALSE;
    }
  }
  return (TRUE);
}


/****************************************************************************
 *
 * Function:    muxDataAvailable
 *
 * Parameters:  VgmuxChannelNumber  channelNumber,
 *              SignalBuffer       *signal_p
 *
 * Returns:     Nothing
 *
 * Description: Add the signals contents to the cache of data we have
 *              received from the multiplexer but not yet processed
 *
 ****************************************************************************/

static void muxDataAvailable (const SignalBuffer *signal_p)
{
  CiMuxAtDataInd     *ciMuxAtDataInd   = &signal_p->sig->ciMuxAtDataInd;
  VgmuxChannelNumber channelNumber     = ciMuxAtDataInd->channelNumber;
#if defined (FEA_MT_PDN_ACT) 
  VgmuxChannelNumber runChannelNumber  = VGMUX_CHANNEL_INVALID;
#endif
  ChannelContext_t   *channelContext_p = ptrToChannelContext (channelNumber);
  SleepManContext_t  *sleepManContext_p  = ptrToSleepManContext();
  Boolean            preemptiveCmdFound     = FALSE;


  /* Assert if we are receiving data before ATCI is ready */

  if((sleepManContext_p->atciInWakeupState)&&(0==channelNumber))
  {
      setResultCode (channelNumber, RESULT_CODE_BUSY); 
      return;
  }
  else
  {
      FatalCheck (!sleepManContext_p->atciInWakeupState,
                  sleepManContext_p->needNetworkStateInd,
                  sleepManContext_p->needSimNok + sleepManContext_p->needSimOk + sleepManContext_p->needSmReadyInd + sleepManContext_p->needGlReadyInd,
                  sleepManContext_p->numPsdBearerStatusIndsNeeded);
  }
#if defined (ATCI_SLIM_DISABLE)
  FatalCheck (channelContext_p != PNULL, channelNumber, 0, 0);
#endif
  addToAtCache (channelNumber,
                 &channelContext_p->at.fromMuxCache [0],
                  &channelContext_p->at.fromMuxCacheCount,
                   signal_p);

  preemptiveCmdFound = checkPreemptiveCmd(channelNumber);

  switch (getEntityState (channelNumber))
  {
    case ENTITY_NOT_ENABLED:
    {
      break;
    }
    case ENTITY_IDLE:
    {
      processMuxInput (&channelContext_p->at.fromMuxCache[0], channelNumber);
      break;
    }
    case ENTITY_PROCESSING:
    {
      break;
    }
    case ENTITY_RUNNING:
    {
      switch (channelContext_p->dataEntryMode)
      {
        case DATA_ENTRY_SMS_MESSAGE:
        {
          processSmsInput (&channelContext_p->at.fromMuxCache[0], channelNumber);
          break;
        }
        case DATA_ENTRY_AT_COMMAND:
        {
          if (preemptiveCmdFound == TRUE)
          {
            processMuxInput (&channelContext_p->at.fromMuxCache[0], channelNumber);
          }
#if defined (FEA_MT_PDN_ACT)
          /* If has set auto-anser CC incoming call, ATA command will be generated by CC timer,
             in this case ATA shouldn't be aborted.
           */
          if (!((getCommandId (channelNumber) == VG_AT_CC_A) &&
                (TRUE == vgCcAutoAnswer (channelNumber,&runChannelNumber)) &&
                (runChannelNumber != VGMUX_CHANNEL_INVALID)))
#endif /* FEA_MT_PDN_ACT */
          {
            vgCiAbortAtCommand (channelNumber);
          }
          break;
        }
        default:
        {
          /* Invalid data entry mode */
          FatalParam (channelContext_p->dataEntryMode, channelNumber, 0);
          /* switch back to default AT entry mode */
        //  channelContext_p->dataEntryMode = DATA_ENTRY_AT_COMMAND;
          break;
        }
      }
      break;
    }
    default:
    {
      /* invalid entity state */
      FatalParam (getEntityState (channelNumber), channelNumber, 0);
      /* immediately disable entity */
     // setEntityState (channelNumber, ENTITY_NOT_ENABLED);
      break;
    }
  }
}


/****************************************************************************
 *
 * Function:    initialiseCimuxData
 *
 * Parameters:  SignalBuffer       *signal_p
 *
 * Returns:     Nothing
 *
 * Description: Initialise entity data, most likely called when an entity is
 * enabled.
 *
 ****************************************************************************/

static void initialiseCimuxData (const VgmuxChannelNumber entity)
{
  ChannelContext_t        *channelContext_p = ptrToChannelContext (entity);
  ScanParseContext_t      *scanParseContext_p   = ptrToScanParseContext (entity);
  OpmanGenericContext_t*  opManGenericContext_p = ptrToOpManGenericContext ();
  EntityMobilityContext_t *entityMobilityContext_p = ptrToEntityMobilityContext (entity);
  Int8                    cacheSize;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (scanParseContext_p != PNULL, entity, 0, 0);
  FatalCheck (channelContext_p != PNULL, entity, 0, 0);
#endif
  /* initialise the cimux cache data */
  for (cacheSize = 0; cacheSize < AT_CACHE_SIZE; cacheSize++)
  {
    channelContext_p->at.fromMuxCache[cacheSize] = kiNullBuffer;
    channelContext_p->at.toMuxCache[cacheSize] = kiNullBuffer;
  }
  channelContext_p->at.toMuxCacheCount = 0;
  channelContext_p->at.fromMuxCacheCount = 0;
  channelContext_p->at.waitingForCnf = FALSE;

  channelContext_p->iprOrIcfSettingsChange = FALSE;
  channelContext_p->delayedSignalId = NON_SIGNAL;
  
  channelContext_p->channelDisablingLocally = FALSE;
  channelContext_p->canRunMabortCmd = FALSE;

  channelContext_p->dataEntryMode  = DATA_ENTRY_AT_COMMAND;
  channelContext_p->actDataIndNoReply =FALSE;
  /* initialise the scan buffer for this entity */
  scanParseContext_p->nextCommand.length = 0;
  scanParseContext_p->nextCommand.position = 0;
#if defined (ATCI_ENABLE_DYN_AT_BUFF)
  scanParseContext_p->nextCommand.character = PNULL;
#endif

  /* initialise the enable channel buffer */
  opManGenericContext_p->channelNeedsEnabling [entity] = FALSE;

  entityMobilityContext_p->msqnState.lastSentLevel     = VG_CESQ_INVALID_RXLEV;
  entityMobilityContext_p->msqnState.lastSentRsrq      = VG_CESQ_INVALID_RSRQ;
  entityMobilityContext_p->msqnState.lastSentRsrp      = VG_CESQ_INVALID_RSRP;
   
}

/****************************************************************************
 *
 * Function:    processEnableEntitySignal
 *
 * Parameters:  SignalBuffer       *signal_p
 *
 * Returns:     Nothing
 *
 * Description: The MUX has asked us to enable a CI entity.  Ask the Op Man
 * to allocate us the NVRAM resource.  If we get it then perform the
 * NVRAM access.  If the request cannot be processed then we schedule it.
 * When the NVRAM CNF is received the request will be processed.
 *
 ****************************************************************************/

static Boolean processEnableEntitySignal (const VgmuxChannelNumber entity)
{

  Boolean                 accepted = TRUE;
  OpmanGenericContext_t*  opManGenericContext_p = ptrToOpManGenericContext ();
  EntityContextData_t*    entityData_p = PNULL;

  if (isEntityActive ((VgmuxChannelNumber)entity) == FALSE)
  {
    /*
     * Check that we have not allocated all channels already and so we should not try to enable
     * anymore
     */
    if (opManGenericContext_p->numberOfEnabledChannels >= ATCI_MAX_NUM_ENABLED_AT_CHANNELS)
    {
        WarnParam (entity, opManGenericContext_p->numberOfEnabledChannels, 0);
        /*
         * Send reject to MUX.
         */
        vgSendCiMuxChannelEnableRsp (entity, FALSE);
    }
    /* if we have successfully read the data into nvram then enable the entity.
       if not then we wait for the op man to do it. */
    else if (vgOpManAllocateConnection (entity, NVRAM_CONNECTION) == TRUE)
    {
       /* get pointer to current entity memory */
       entityData_p = ptrToEntityContextData (entity);

       /* if memory not yet allocated then allocate it */
       if  (PNULL == entityData_p)
       {        
         entityData_p = vgAllocateRamToEntity();
       }

       /* memory is successfully allocated, so continue to set entity up */
       if (PNULL != entityData_p)
       {
         allocateMemToEntity (entity, (void *)entityData_p);
         opManGenericContext_p->numberOfEnabledChannels++;

         /* read the nvram data for this entity */
         vgNvramDoNvramAccess (READ_REQUEST,
                                entity,
                                 vgCiGetAnrm2DataName(entity),
                                  VG_NVRAM_READ_ONLY);

         /* initialise all the entity data */
         initialiseCimuxData (entity);

         /* allow the other sub-systems to initialise their entity data */
         accepted = FALSE;

       } /* PNULL != entityData_p */
       else
       {
         /* Channel enable failed - so send negative response to the MUX. */
         WarnParam(entity, 0, 0);
         vgSendCiMuxChannelEnableRsp(entity, FALSE);
       }
    }
    else
    {
      /* channel enable request, which we cannot handle so schedule it */
      /* we accept this enable request to stop the other su-systems initialing their channel data */
      opManGenericContext_p->channelNeedsEnabling [entity] = TRUE;
    }
  }
  else
  {
     /*Init the status.*/
     opManGenericContext_p->channelState [entity].isDisconnecting = FALSE;

     /* Channel already open, so simply send response, this will allow COT PC scripts test to run */
     vgSendCiMuxChannelEnableRsp(entity, TRUE);
  }
  return (accepted);
}

/****************************************************************************
 *
 * Function:    handleCiMuxChannelDisableInd
 *
 * Parameters:  SignalBuffer       *signal_p
 *
 * Returns:     Nothing
 *
 * Description: The MUX has asked us to disable a CI entity.  We must release
 * and allocated memory and set the entity to the correct state.
 *
 ****************************************************************************/

static void handleCiMuxChannelDisableInd (const VgmuxChannelNumber entity)
{
  OpmanGenericContext_t  *opManGenericContext_p = ptrToOpManGenericContext ();
  Boolean                callsActive;
  GprsContext_t          *gprsContext_p         = ptrToGprsContext(entity);  

  if (isEntityActive (entity) == TRUE)
  {
    /* disconnect all calls established by this channel */
    callsActive = vgCcDisconnectAllCalls (entity);

    /* Check if we were waiting for this in order to send a response back to
     * ABPD
     */
    if ((gprsContext_p != PNULL) && (gprsContext_p->pendingOpenDataConnCnf == TRUE))
    {
      vgApexAbpdConnectRej (entity);
      gprsContext_p->pendingOpenDataConnCnf = FALSE;
      callsActive = TRUE;
    }

    deleteSolicitedSignalRecordForEntity (entity);

    if ( callsActive )
    {
      opManGenericContext_p->channelState [entity].isDisconnecting = TRUE;
    }
    else
    {
      /* call state is offline, then entity can be clear */
      if (vgCallIsOfflineState(entity) == TRUE)
      {
        vgClearEntity ( entity);
      }
      else
      {
        WarnParam(entity, 0, 0);
      }
    }
  }
}

/****************************************************************************
 *
 * Function:    addToAtCache
 *
 * Parameters:  VgmuxChannelNumber  channelNumber,
                SignalBuffer       *signal_p
 *
 * Returns:     Nothing
 *
 * Description: Add the signals contents to the cache of data we have
 *              received from the multiplexer but not yet processed
 *
 ****************************************************************************/
static void addToAtCache (const VgmuxChannelNumber entity,
                          SignalBuffer             targetQueue [],
                          Int8                    *targetQueueLength,
                          const SignalBuffer      *signalBuffer_p)
{
  CiMuxAtDataInd    *sig_p, *signal_p;
  Int32 len;

  PARAMETER_NOT_USED (entity);

  FatalAssert (signalBuffer_p != PNULL);
  FatalAssert (signalBuffer_p->type != PNULL);
  FatalAssert (*(signalBuffer_p->type) == SIG_CIMUX_AT_DATA_IND);
  FatalAssert (entity < CI_MAX_ENTITIES);
  signal_p  = &signalBuffer_p->sig->ciMuxAtDataInd;

  /* first queue element so just push the signal */
  if (*targetQueueLength == 0)
  {
    targetQueue [*targetQueueLength] = kiNullBuffer;

    KiCreateSignal (SIG_CIMUX_AT_DATA_IND,
                       sizeof (CiMuxAtDataInd),
                      &targetQueue [*targetQueueLength]);

    sig_p = &targetQueue [*targetQueueLength].sig->ciMuxAtDataInd;
    memcpy (sig_p, signal_p, sizeof (CiMuxAtDataInd));

    *targetQueueLength += 1;
  }
  else
  {
    len   = signal_p->length;
    sig_p = &targetQueue [*targetQueueLength - 1].sig->ciMuxAtDataInd;
    /*
     * enough space in last signal to add the data
     */
    if (len < (CIMUX_MAX_AT_DATA_LENGTH - sig_p->length))
    {
      memcpy (&sig_p->data[sig_p->length], signal_p->data, signal_p->length);
      sig_p->length += signal_p->length;
    }
    else
    {
      len = CIMUX_MAX_AT_DATA_LENGTH - sig_p->length;
      memcpy (&sig_p->data[sig_p->length], signal_p->data, len);
      sig_p->length += len;

      /* append data to the last signal in the queue and then
       * push the new signal onto the Queue
       */
      if (*targetQueueLength < CFGVAL_VGMUX_AT_QUEUE_SIZE)
      {
        memcpy (signal_p->data, &signal_p->data[len], signal_p->length - len);
        signal_p->length = signal_p->length - len;

        targetQueue [*targetQueueLength] = kiNullBuffer;

        KiCreateSignal (SIG_CIMUX_AT_DATA_IND,
                            sizeof (CiMuxAtDataInd),
                           &targetQueue [*targetQueueLength]);

        sig_p = &targetQueue [*targetQueueLength].sig->ciMuxAtDataInd;
        memcpy (sig_p, signal_p, sizeof (CiMuxAtDataInd));

        *targetQueueLength += 1;
      }
      else
      {
        /*
         * Queue is full so ignore this signal
         */
        WarnParam (entity, 0, 0);
  
      }
    }
  }
}

/****************************************************************************
 *
 * Function:    handleCiMuxCheckCmuxCmdParamsCnf
 *
 * Parameters: CiMuxCheckCmuxCmdParamsCnf ciMuxCheckCmuxCmdParamsCnf
 *
 * Returns:     Nothing
 *
 * Description: Handle Checking of Mux parameters
 *****************************************************************************/
static void handleCiMuxCheckCmuxCmdParamsCnf (const VgmuxChannelNumber  entity,
                                        const CiMuxCheckCmuxCmdParamsCnf ciMuxCheckCmuxCmdParamsCnf)
{
  ChannelContext_t    *channelContext_p   = ptrToChannelContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (channelContext_p != PNULL, entity, 0, 0);
#endif
  if (ciMuxCheckCmuxCmdParamsCnf.parametersValid == TRUE)
  {
    /*
     * Save the CMUX parameters OKed by the MUX before starting the timer which
     * will delay MUX activation until after the OK has been sent out.
     */
    channelContext_p->cmuxCmdParams = ciMuxCheckCmuxCmdParamsCnf.cmuxCmdParams;

    setResultCode (entity, RESULT_CODE_OK);

    /*
     * Delay sending the SIG_CIMUX_START_27010MUX_REQ to allow the OK response to get to
     * PC or MMI.
     */
    vgCiStartTimer (TIMER_TYPE_CMUX_ACTIVATION, entity);
  }
  else
  {
    setResultCode (entity, RESULT_CODE_ERROR);
  }
}

/****************************************************************************
 *
 * Function:    handleCiMuxReadCmuxCmdParamsCnf
 *
 * Parameters:  CmuxCmdParams cmuxCmdParams
 *
 * Returns:     Nothing
 *
 * Description: Handle Mux Command Parameters confirmation.
 *****************************************************************************/
static void handleCiMuxReadCmuxCmdParamsCnf (const VgmuxChannelNumber  entity,
                                          const CiMuxReadCmuxCmdParamsCnf ciMuxReadCmuxCmdParamsCnf)
{
  CmuxCmdParams cmuxCmdParams = ciMuxReadCmuxCmdParamsCnf.cmuxCmdParams;
  ResultCode_t result = RESULT_CODE_OK;
  MobilityContext_t   *mobilityContext_p   = ptrToMobilityContext ();
  ChManagerContext_t      *chManagerContext_p       = ptrToChManagerContext ();

  vgPutNewLine (entity);

  if (ciMuxReadCmuxCmdParamsCnf.active27010 == TRUE)
  {
     vgPrintf (entity,
                (const Char*)"+CMUX: %d,%d,%d,%d,%d,%d,%d,%d,%d",
                 cmuxCmdParams.mode,
                 cmuxCmdParams.subset,
                 cmuxCmdParams.portSpeed,
                 cmuxCmdParams.n1,
                 cmuxCmdParams.t1,
                 cmuxCmdParams.n2,
                 cmuxCmdParams.t2,
                 cmuxCmdParams.t3,
                 cmuxCmdParams.k);
  }
  else
  {
     vgPrintf (entity, (const Char*)"+CMUX: -1");
  }

  vgPutNewLine (entity);

  if (getCommandId(entity) == VG_AT_PF_VIEW)
  {
    /* Always sent request to AB even if it looks like nothing has changed */
    chManagerContext_p->isImmediate = TRUE;
    mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
    result = vgSmsSigOutApexReadSmspReq(entity, VG_SMSP_READ_BLOCK_QUERY);
  }
  else
  {
    vgPrintf (entity, (const Char*)"OK");
    vgPutNewLine (entity);
  }

  setResultCode (entity, result);
}

/****************************************************************************
 *
 * Function:    handleCiMuxConfigureMuxCnf
 *
 * Parameters:  CiMuxConfigureMuxCnf configMuxCnf
 *
 * Returns:     Nothing
 *
 * Description: Handle Configure Mux confirmation.
 *****************************************************************************/
static void handleCiMuxConfigureMuxCnf (const VgmuxChannelNumber  entity,
                                        const CiMuxConfigureMuxCnf configMuxCnf)
{
  if (configMuxCnf.success == TRUE)
  {
    setResultCode (entity, RESULT_CODE_OK);
  }
  else
  {
    setResultCode (entity, RESULT_CODE_ERROR);
  }
}

/****************************************************************************
 *
 * Function:    handleCiMuxSwitchedToCmdModeInd
 *
 * Parameters:  CiMuxSwitchedToCmdModeInd switchToCmdModeInd
 *
 * Returns:     Nothing
 *
 * Description: Indicates the channel has swtiched from DATA mode to COMMAND
 * mode as a result of the multiplexer detecting an escape sequence (connection
 * is kept active) or the host has drop DTR (close the connection).
 *****************************************************************************/
static void handleCiMuxSwitchedToCmdModeInd (const VgmuxChannelNumber entity,
                                             const CiMuxSwitchedToCmdModeInd switchToCmdModeInd)
{
  OpmanContext_t       *opManContext_p = ptrToOpManContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (opManContext_p != PNULL, entity, 0, 0);
#endif
  if (vgDoesEntityHaveSeparateDataChannel(entity) == FALSE)
  {
    /*
    * Must set correct result code once we have switched back to command mode.  This will
    * generate an "OK" string on the ATCI interface.
    */
    switch (getCommandId (entity))
    {
      case VG_AT_CC_O:
      case VG_AT_CC_D:
      case VG_AT_CC_DL:
      case VG_AT_GP_D:
#if defined (FEA_PPP)        
      case VG_AT_GP_MLOOPPSD:
#endif /* FEA_PPP */        
#if defined (FEA_MT_PDN_ACT)
      case VG_AT_CC_A:
      case VG_AT_GP_CGANS:
#endif /* FEA_MT_PDN_ACT */
      case VG_AT_GP_CGDATA:
      {
        setResultCode (entity, RESULT_CODE_OK);
        break;
      }
      default:
      {
        break;
      }
    }
    if ((getCommandId (entity) >= VG_AT_AP_BRIDGE_BASE) && (getCommandId (entity) <= VG_AT_AP_BRIDGE_END))
    {
      setConnectionType(entity, CONNECTION_TERMINATOR);
      setResultCode (entity, RESULT_CODE_OK);
      vgApbChannelSwitchedToCmdMode(entity);
    }
  }

  else
  {
    /* For channels which have a separate data channel we should never receive
     * this signal.
     */
    FatalParam(entity, getCommandId (entity), 0);
  }
  
}

/****************************************************************************
 *
 * Function:    handleCiMuxClosedDataConnInd
 *
 * Parameters:  CiMuxClosedDataConnInd closedDataConnInd
 *
 * Returns:     Nothing
 *
 * Description: Indicates that the data connection is closed and the channel
 * is back in COMMAND mode. This signal is used to indicate to ATCI that the
 * the peer host has requested to close the data connection on this channel
 * (e.g. on reception of a DTR drop).

 *****************************************************************************/
static void handleCiMuxClosedDataConnInd     (const VgmuxChannelNumber entity,
                                             const CiMuxClosedDataConnInd closedDataConnInd)
{
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  OpmanContext_t       *opManContext_p = ptrToOpManContext (entity);
  GprsContext_t        *gprsContext_p = ptrToGprsContext(entity);
  ApBridgeDataModeContext_t* apBridgeDataModeContext_p = vgApbGetDataModeContext((VgmuxChannelNumber)(closedDataConnInd.channelNumber));

#if defined (ATCI_SLIM_DISABLE)
  FatalCheck (opManContext_p != PNULL, entity, 0, 0);
  FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
#endif
  if (apBridgeDataModeContext_p != NULL)
  {
    vgApbCiCloseDataConnIndHandler(closedDataConnInd.channelNumber);
    return;
  }

  /*
   * Must set correct result code once we have switched back to command mode.  This will
   * generate an "OK" string on the ATCI interface.
   */
  switch (getCommandId (entity))
  {
    case VG_AT_CC_O:
    case VG_AT_CC_D:
    case VG_AT_CC_DL:
    case VG_AT_GP_D:
#if defined (FEA_PPP)      
    case VG_AT_GP_MLOOPPSD:
#endif /* FEA_PPP */      
#if defined (FEA_MT_PDN_ACT)
    case VG_AT_CC_A:
    case VG_AT_GP_CGANS:
#endif /* FEA_MT_PDN_ACT */
    case VG_AT_GP_CGDATA:
    {
      /* Only generate OK if we are not already attempting to hang this connection up.
       * This covers race condition where we may have got a CLOSED_DATA_CONN_IND
       * mid disconnect
       */
      if (!((gprsContext_p->vgHangupCid != CID_NUMBER_UNKNOWN) &&
          (gprsGenericContext_p->cidUserData[gprsContext_p->vgHangupCid]->pendingContextDeactivation)))
      {
        setResultCode (entity, RESULT_CODE_OK);
      }
      break;
    }
    default:
    {
      break;
    }
  }

  /*
   * DTR dropped and the MUX has closed the data connection as AT&D2 was set.
   *
   */

  if (opManContext_p->numberOfCallConnections > 0)
  {
    switch (opManContext_p->callInfo.vgClass)
    {
#if defined (FEA_PPP)      
      case PPP_CONNECTION:
#endif /* FEA_PPP */        
      case PT_CONNECTION:
      {
        switch (opManContext_p->callInfo.vgState)
        {
          case CONNECTION_DIALLING:
          case CONNECTION_CONNECTING:
          case CONNECTION_ON_LINE:
          {
#if defined (ATCI_SLIM_DISABLE)
            FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
#endif
            gprsContext_p->disconnectionItem             = opManContext_p->callInfo.vgIdent;
            gprsContext_p->connectionActive              = FALSE;
            
            gprsContext_p->vgHangupType                  = VG_HANGUP_TYPE_DTR_DROPPED;
            gprsContext_p->vgHangupCid                   = vgFindCidWithDataConnLinkedToEntity(entity);
            
            setResultCode (entity, vgChManContinueAction (entity, SIG_APEX_ABPD_HANGUP_REQ));
            break;
          }
          default:
          {
            /* not online */
            break;
          }
        }
        break;
      }
      default:
      {
        /* no connected data call, so no action required */
        break;
      }
    }
  }
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/****************************************************************************
 *
 * Function:    getSsCodeForEntity
 *
 * Parameters:  Int16   type
 *
 * Returns:     Boolean
 *
 * Description: Uses the registration look up table to extract the sub-system
 * id given an entity id and a signal type.
 *
 ****************************************************************************/

SolicitedSignalRecord_t *getSolicitedSignalsForEntity (const VgmuxChannelNumber entity)
{
  return (solicitedBuffer[entity]);
}


/****************************************************************************
 *
 * Function:    getSsCodeForEntity
 *
 * Parameters:  Int16   type
 *
 * Returns:     VoyagerSubsystems_t
 *
 * Description: Uses the registration look up table to extract the sub-system
 * id given an entity id and a signal type.
 *
 ****************************************************************************/

VoyagerSubsystems_t getSsCodeForEntity (const SignalId  thisType,
                                         const VgmuxChannelNumber entity,
                                          Int8 *regIndex,
                                           Boolean *codeFound)
{
  Int8                  bufferIndex;
  VoyagerSubsystems_t   subsystem = SS_START;

  *codeFound = FALSE;

  for ( bufferIndex = 0;
         (bufferIndex < SOLICITED_MAX_NUM_ITEMS) && (*codeFound == FALSE);
          bufferIndex++ )
  {
    if (solicitedBuffer [entity][bufferIndex].type == thisType)
    {
      *codeFound = TRUE;
      *regIndex = bufferIndex;
    }
  }

  if (*codeFound == TRUE)
  {
    subsystem = solicitedBuffer [entity][*regIndex].ssCode;
  }
  return (subsystem);
}

/****************************************************************************
 *
 * Function:    resetRegTypeForEntity
 *
 * Parameters:  SignalBuffer       *signal_p
 *
 * Returns:     Nothing
 *
 * Description: We have received a signal, checked the registration look-up
 * table and processed the signal.  Now we have finished with it then we
 * need to blank the element off in the table.  We do this by copying down
 * the data in the table.
 *
 ****************************************************************************/

void resetRegTypeForEntity (const VgmuxChannelNumber entity,
                             const Int8 signalIndex)
{
  Int8 bufferIndex = 0;

  /* handle resetting the last element in the list */
  if ( signalIndex + 1 == SOLICITED_MAX_NUM_ITEMS )
  {
    solicitedBuffer [entity][signalIndex].type = NON_SIGNAL;
  }
  /* handle all elements in the list that are in range */
  else if ( signalIndex < SOLICITED_MAX_NUM_ITEMS )
  {
    /* copy down and reset the buffer */
    for (bufferIndex = signalIndex;
          bufferIndex < (SOLICITED_MAX_NUM_ITEMS - 1);
           bufferIndex++)
    {
      solicitedBuffer [entity][bufferIndex] = solicitedBuffer [entity][bufferIndex + 1];
    }

    /*
     * Find the next empty slot index value
     */
    bufferIndex = 0;
    while (solicitedBuffer [entity][bufferIndex].type != (Int16)0 &&
            bufferIndex < SOLICITED_MAX_NUM_ITEMS)
    {
      bufferIndex++;
    }

    FatalCheck (bufferIndex < SOLICITED_MAX_NUM_ITEMS, entity, 0, 0);
    solicitedBuffer [entity][bufferIndex].type   = NON_SIGNAL;
    solicitedBuffer [entity][bufferIndex].ssCode = SS_START;
  }
}

/****************************************************************************
 *
 * Function:    cimuxPerfomNextAction
 *
 * Parameters:  VgmuxChannelNumber entity
 *
 * Returns:     Nothing
 *
 * Description:
 *
 ****************************************************************************/

static void cimuxPerformNextAction (const VgmuxChannelNumber entity)
{

  ScanParseContext_t *scanParseContext_p = ptrToScanParseContext (entity);
  ChannelContext_t   *channelContext_p   = ptrToChannelContext (entity);


  SignalBuffer       signal = kiNullBuffer;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (scanParseContext_p != PNULL, entity, 0, 0);
  FatalCheck (channelContext_p != PNULL, entity, 0, 0);
#endif
  {

    if (scanParseContext_p->nextCommand.length == 0)
    {
      /* send the result code to the cr manager for the current transaction.
       * only send the result code after a complete command has been completed,
       * this will handle the AT&V&V&V case.  Only one result code should be
       * output */
      sendResultCodeToCrm (getResultCode (entity), entity);

      setEntityState (entity, ENTITY_IDLE);
      setCommandId (entity, VG_AT_NO_COMMAND);
      setCommandName (entity, (Char *) "");
      setConnectionType (entity, CONNECTION_TERMINATOR);
      setTaskInitiated (entity, FALSE);

      /* Reset the scan buffer */
      resetTheScanBuffer (entity);
      flushTheCache (entity);

    }
    else
    {
      /* job106314: modify 2nd parameter for function call */
      if (checkForExtendedCommandSeparator (
           entity,
            scanParseContext_p) == TRUE)
      {
        setEntityState (entity, ENTITY_PROCESSING);

        KiCreateSignal (SIG_CI_RUN_AT_COMMAND_IND,
                             sizeof (CiRunAtCommandInd),
                              &signal);

        signal.sig->ciRunAtCommandInd.entity = entity;

        signal.sig->ciRunAtCommandInd.activeAtCommand.length = scanParseContext_p->nextCommand.length;
        signal.sig->ciRunAtCommandInd.activeAtCommand.position = scanParseContext_p->nextCommand.position;

        /* Only copy first 200 characters of AT command to be executed */
        memcpy (&signal.sig->ciRunAtCommandInd.activeAtCommand.character,
               &scanParseContext_p->nextCommand.character,
               SHORT_COMMAND_LINE_SIZE);

        KiSendSignal (VG_CI_TASK_ID,&signal);

        /* job106314: need to store command type (basic or extended syntax) */
        /* for use when parsing possible next command in a concatenated */
        /* command string */
        if ((scanParseContext_p->nextCommand.character[0] == PLUS_CHAR) ||
            (scanParseContext_p->nextCommand.character[0] == STAR_CHAR) ||
            (scanParseContext_p->nextCommand.character[0] == HAT_CHAR))


        {
          scanParseContext_p->currentCommandType = EXTENDED_SYNTAX;
        }
        else
        {
          scanParseContext_p->currentCommandType = BASIC_SYNTAX;
        }
      }
      else
      {
        /* send the result code to the cr manager for the current transaction */
        sendResultCodeToCrm (getResultCode (entity), entity);

        /* reset the command buffer data and the entity state flags */
        setEntityState    (entity, ENTITY_IDLE);
        setCommandId      (entity, VG_AT_NO_COMMAND);
        setCommandName    (entity, (Char *) "");
        setConnectionType (entity, CONNECTION_TERMINATOR);
        setTaskInitiated  (entity, FALSE);

        /* reset the scan buffer */
        resetTheScanBuffer (entity);
      }
    }
  }
}

/****************************************************************************
 *
 * Function:    checkForCommandCompleted
 *
 * Parameters:  SignalBuffer       *signal_p
 *
 * Returns:     Nothing
 *
 * Description:  Given an entity id check if the entity has finished processing
 * the current AT command.  If it has then get the next command, either from
 * the AT command string or from the cache.
 *
 * if the signal has caused entity scan state to move to error then we can only
 * bin the current record and pop another from the cache.
 * if proceeding then we just wait for the next signal to arrive and hope
 * that this will set the state to OK.
 * if an okay state has occurred then we can get the next data in the
 * current message or go to the cache.
 *
 ****************************************************************************/

void checkForCommandCompleted (const VgmuxChannelNumber entity)
{
  ChannelContext_t         *channelContext_p    = ptrToChannelContext (entity);
  StkEntityGenericData_t   *stkGenericContext_p = ptrToStkGenericContext ();

#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (channelContext_p != PNULL, entity, 0, 0);
  FatalCheck (stkGenericContext_p != PNULL, entity, 0, 0);
#endif
  if (getEntityState (entity) == ENTITY_RUNNING)
  {
    switch (getResultCode (entity))
    {
      case RESULT_CODE_PROCEEDING: /* just wait around */
      {
         break;
      }

      case RESULT_CODE_RING:
      case RESULT_CODE_CONNECT:
      case RESULT_CODE_CUSTOM_CONNECT:
      {
        /* these are intermediate/unsolicited result codes */
        FatalParam (entity, getResultCode (entity), 0);
        break;
      }

      case RESULT_CODE_NO_CARRIER:
      case RESULT_CODE_INV:
      case RESULT_CODE_NO_DIALTONE:
      case RESULT_CODE_BUSY:
      case RESULT_CODE_NO_ANSWER:
      case RESULT_CODE_OK:
      case RESULT_CODE_NULL:
      {
        vgChManReleaseControl (entity);

#if defined (FEA_PHONEBOOK)
        /* if the phone was temporarily set for the last command then reset it */
        resetTemporaryPhoneBook (entity);
#endif /* FEA_PHONEBOOK */

        if ((stkGenericContext_p->runAtCmdState == STATE_RUNNING_COMMAND) &&
           (entity == stkGenericContext_p->atCommandData.cmdEntity ))
        {
          stkGenericContext_p->runAtCmdState = STATE_COMMAND_COMPLETED;
        }

        /* if the command has finished then execute the next action */
        cimuxPerformNextAction (entity);

        break;
      }
      case RESULT_CODE_ERROR: /* bin the data and then get some more */
      default: /* cme/cms error codes */
      {
        /* release control of all BL procedures */
        vgChManReleaseControl (entity);


#if defined (FEA_PHONEBOOK)
        /* if the phone was temporarily set for the last command then reset it */
        resetTemporaryPhoneBook (entity);
#endif /* FEA_PHONEBOOK */

        if ((stkGenericContext_p->runAtCmdState == STATE_RUNNING_COMMAND) &&
           (entity == stkGenericContext_p->atCommandData.cmdEntity ))
        {
          stkGenericContext_p->runAtCmdState = STATE_COMMAND_COMPLETED;
        }

        /* send the result code to the cr manager for the current transaction */
        sendResultCodeToCrm (getResultCode (entity), entity);

        /* reset the command buffer data and the entity state flags */
        setEntityState    (entity, ENTITY_IDLE);
        setCommandId      (entity, VG_AT_NO_COMMAND);
        setCommandName    (entity, (Char *) "");
        setConnectionType (entity, CONNECTION_TERMINATOR);
        setTaskInitiated  (entity, FALSE);

        /* Reset the scan buffer */
        resetTheScanBuffer (entity);

        flushTheCache (entity);

        break;
      }
    }
  }
}

/****************************************************************************
 *
 * Function:    initialiseEntities
 *
 * Parameters:  void
 *
 * Returns:     Nothing
 *
 * Description: Initialises the runtime data.
 *
 ****************************************************************************/

void initialiseEntities (void)
{
  Int8         eIndex;
  Entity_t     *entityPtr    = PNULL;
  MuxContext_t *muxContext_p = ptrToMuxContext ();

  for (eIndex = 0; eIndex < CI_MAX_ENTITIES; eIndex++)
  {
    initialiseMemToEntity ((VgmuxChannelNumber)eIndex);

    setEntityState ((VgmuxChannelNumber)eIndex, ENTITY_NOT_ENABLED);
  }

  entityPtr = ptrToAtciContextData();

  /* Clear fixed memory allocation */
  for (eIndex = 0; eIndex < ATCI_MAX_NUM_ENABLED_AT_CHANNELS; eIndex++)
  {
    entityPtr->inUseEntityMemDataItem[eIndex].inUse = FALSE;
  }

  /* Clear the registered flag - i.e. we have not yet registered with
   * AB.
   * NOTE: For wakeup from deep sleep this gets reset again to show
   * we have registered.
   */
  muxContext_p->atciRegisteredWithABPRocedures = FALSE;

  /* Clear the Phone Functionality read flag.
   * NOTE: For wakeup from deep sleep this gets reset again to show
   * we have already got this information.
   */
  muxContext_p->atciHaveReadPhoneFunctionality = FALSE;
  
  
}

/*--------------------------------------------------------------------------
 *
 * Function:    ackDataInd
 *
 * Parameters:  in:  Void
 *
 * Returns:     Void
 *
 * Description:  We have finished processing a SIG_CIMUX_AT_DATA_IND and if
 *               this is a version 3 multiplexer then we must send a response
 *
 *-------------------------------------------------------------------------*/

void ackDataInd (const VgmuxChannelNumber entity)
{
  SignalBuffer signal = kiNullBuffer;
  CiMuxAtDataRsp  *newSig_p;
  StkEntityGenericData_t *stkGenericContext_p = ptrToStkGenericContext ();
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (stkGenericContext_p != PNULL, entity, 0, 0);
#endif
    /* no need to send anything to the MUX if this is for the internal STK channel */
  if (!(((stkGenericContext_p->runAtCmdState == STATE_RUNNING_COMMAND) ||
       (stkGenericContext_p->runAtCmdState == STATE_COMMAND_COMPLETED)) &&
       (entity == stkGenericContext_p->atCommandData.cmdEntity)))
  {
    KiCreateZeroSignal(SIG_CIMUX_AT_DATA_RSP, sizeof(CiMuxAtDataRsp), &signal);
    newSig_p = &signal.sig->ciMuxAtDataRsp;
    newSig_p->channelNumber = entity;
#ifdef UE_SIMULATOR
    KiSendSignal (VMMI_TASK_ID, &signal);
#else
    KiSendSignal (MUX_TASK_ID, &signal);
#endif
  }
  
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgMuxCMUXACTIVATIONTimerExpiry
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Activate 27.010 MUX after user has entered AT+CMUX command
 *              to output "OK" before the MUX is activated.
 *-------------------------------------------------------------------------*/
void vgMuxCMUXACTIVATIONTimerExpiry (const VgmuxChannelNumber entity)
{
  ChannelContext_t    *channelContext_p     = ptrToChannelContext (entity);
  SignalBuffer         signal               = kiNullBuffer;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (channelContext_p != PNULL, entity, 0, 0);
#endif
  /*
   * Reset the timer running flag and then send the MUX configuration.
   */

  KiCreateZeroSignal (SIG_CIMUX_START_27010MUX_REQ,
                      sizeof (CiMuxStart27010MuxReq),
                      &signal);

  signal.sig->ciMuxStart27010MuxReq.channelNumber = entity;
  signal.sig->ciMuxStart27010MuxReq.cmuxCmdParams = channelContext_p->cmuxCmdParams;

  KiSendSignal (MUX_TASK_ID,&signal);
}

/****************************************************************************
 *
 * Function:    deleteSolicitedSignalRecordForEntity
 *
 * Parameters:  entity
 *
 * Returns:     Nothing
 *
 * Description: Deletes the solicited signal record for the specified entity
 *
 ****************************************************************************/
/* added for job129859 */
void deleteSolicitedSignalRecordForEntity (const VgmuxChannelNumber entity)
{
  Int8    index;

  for (index = 0; index < SOLICITED_MAX_NUM_ITEMS; index++)
  {
    solicitedBuffer[entity][index].type = NON_SIGNAL;
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCiMuxInterfaceController
 *
 * Parameters:  in:  signal, entity
 *
 * Returns:    whether the signal is processed by the module.
 *
 * Description:  The interface to the MUX section of the CI task.   Handles signals received
 *                 from Generic MUX task.
 *
 *-------------------------------------------------------------------------*/
Boolean vgMuxInterfaceController (const SignalBuffer *signal_p,
                                   const VgmuxChannelNumber entity)
{
  Boolean                 accepted  = FALSE;
  OpmanGenericContext_t*  opManGenericContext_p = ptrToOpManGenericContext ();
  MuxContext_t *muxContext_p = ptrToMuxContext ();

  switch (*signal_p->type)
  {
    case SIG_INITIALISE:
    {
      initialiseEntities ();
      /* cannot accept this signal as it may be needed by all the sub-systems */
      break;
    }
    case SIG_CIMUX_CONFIGURE_MUX_CNF:
    {
      handleCiMuxConfigureMuxCnf (entity, signal_p->sig->ciMuxConfigureMuxCnf);
      accepted = TRUE;                        /* this signal is only for the CIMUX */
      break;
    }
    case SIG_CIMUX_CHECK_CMUX_CMD_PARAMS_CNF:
    {
      handleCiMuxCheckCmuxCmdParamsCnf (entity, signal_p->sig->ciMuxCheckCmuxCmdParamsCnf);
      accepted = TRUE;                        /* this signal is only for the CIMUX */
      break;
    }
    case SIG_CIMUX_READ_CMUX_CMD_PARAMS_CNF:
    {
      handleCiMuxReadCmuxCmdParamsCnf (entity, signal_p->sig->ciMuxReadCmuxCmdParamsCnf);
      accepted = TRUE;                        /* this signal is only for the CIMUX */
      break;
    }
    case SIG_CIMUX_SWITCHED_TO_CMD_MODE_IND:
    {
      handleCiMuxSwitchedToCmdModeInd (entity, signal_p->sig->ciMuxSwitchedToCmdModeInd);
      accepted = TRUE;                        /* this signal is only for the CIMUX */
      break;
    }
    case SIG_CIMUX_AT_DATA_IND:
    {
      ChannelContext_t   *channelContext_p = ptrToChannelContext (entity);
      muxDataAvailable (signal_p);
      accepted = TRUE;  /* this signal is only for the CIMUX */
      if(entity == 0)
      {

#if defined (ATCI_SLIM_DISABLE)
          FatalCheck (channelContext_p != PNULL, entity, 0, 0); 
#endif
          if((channelContext_p->at.fromMuxCacheCount<CIVAL_CIMUX_AT_QUEUE_FLOWCONTROL_ACTIVE_SIZE))
          {
              ackDataInd (entity);
          }
          else
          {
             channelContext_p->actDataIndNoReply = TRUE;
#ifndef NO_EMMI_INTERFACE
             KiSetEmmiLogByPass(TRUE);
#endif
          }
      }
      else
      {
           ackDataInd (entity);     
      }
      break;
    }
    case SIG_CI_RUN_AT_COMMAND_IND:
    {
      if (getEntityState (entity) != ENTITY_RUNNING)
      {
        setEntityState (entity, ENTITY_RUNNING);
        accepted = parseCommandBuffer (mxAtCommandTable, entity);
      }

      break;
    }
    case SIG_CIMUX_CHANNEL_ENABLE_IND:
    {
      opManGenericContext_p->channelEnableIndChannelNumber[entity] = (Int8)signal_p->sig->ciMuxChannelEnableInd.channelNumber;
      /* Save if this channel is used for external connection */
      muxContext_p->isExternalChannel[entity] = signal_p->sig->ciMuxChannelEnableInd.isExtConnection;
      accepted = processEnableEntitySignal (entity);
      break;
    }
    case SIG_CIMUX_CHANNEL_DISABLED_IND:
    {
      handleCiMuxChannelDisableInd (entity);
      break;
    }
    case SIG_CIMUX_CLOSED_DATA_CONN_IND:
    {
      handleCiMuxClosedDataConnInd (entity, signal_p->sig->ciMuxClosedDataConnInd);
      accepted = TRUE;                        /* this signal is only for the CIMUX */
      break;
    }  

    case SIG_CI_DATA_ENTRY_MODE_IND:
    {
      switch ((VgDataEntryMode)signal_p->sig->ciDataEntryModeInd.dataEntryMode)
      {
        case DATA_ENTRY_AT_COMMAND:
        {
          /* don't process AT command data when a command is already running */
          break;
        }
        case DATA_ENTRY_SMS_MESSAGE:
        {
          initialiseSmsInput (entity);
          flushSmsInput (entity);
          break;
        }
        default:
        {
          FatalParam (entity, (VgDataEntryMode)signal_p->sig->ciDataEntryModeInd.dataEntryMode, 0);
          break;
        }
      }
      accepted = TRUE;
      break;
    }
    case SIG_CIMUX_OPEN_DATA_CONN_CNF:
    {
      accepted = processOpenDataConnCnf (entity, signal_p->sig->ciMuxOpenDataConnCnf.success);
      accepted = TRUE;                        /* this signal is only for the CIMUX */
      break;
    }
    case SIG_VGCI_SS_REGISTRATION_IND:
    {
      doSsRegistrationInd (signal_p);
      accepted = TRUE;                        /* this signal is only for the CIMUX */
      break;
    }
    default:
    {
     /* not interested in this signal so do not process or accept it */
      break;
    }
  }

  return (accepted);
}

/*************************************************************************
*
* Function:     vgAllocateRamToEntity
*
* Parameters:   None
*
* Returns:      Nothing
*
* Description:  Assigns static RAM to an entity
*
*************************************************************************/

EntityContextData_t* vgAllocateRamToEntity (void)
{

  Int8                     inUseEntityDataItemCount;
  EntityContextData_t      *dataPtr = PNULL;
  Entity_t                 *atciContextDataPtr;
  Boolean                  found = FALSE;

  atciContextDataPtr = ptrToAtciContextData();
  
  /* Search for entity data item not in use */
  for (inUseEntityDataItemCount=0; !found && (inUseEntityDataItemCount < ATCI_MAX_NUM_ENABLED_AT_CHANNELS); inUseEntityDataItemCount++)
  {
    if (atciContextDataPtr->inUseEntityMemDataItem[inUseEntityDataItemCount].inUse == FALSE)
    {
      atciContextDataPtr->inUseEntityMemDataItem[inUseEntityDataItemCount].inUse = TRUE;
      dataPtr = &(atciContextDataPtr->inUseEntityMemDataItem[inUseEntityDataItemCount].entityContextData);
      found = TRUE;
    }          
  }

  return (dataPtr);
}

/*************************************************************************
*
* Function:     vgFreeRamForEntity
*
* Parameters:   Pointer to be freed
*
* Returns:      nothing
*
* Description:  Frees static RAM for the pointer
*
*************************************************************************/
void vgFreeRamForEntity             (EntityContextData_t **dataPtr)
{
  Int8                     inUseEntityDataItemCount;
  Entity_t                 *atciContextDataPtr;
  Boolean                  found = FALSE;

  if (*dataPtr != PNULL)
  {

    atciContextDataPtr = ptrToAtciContextData();

    /* Search for entity data item in use matching pointer */
    for (inUseEntityDataItemCount=0; !found && (inUseEntityDataItemCount < ATCI_MAX_NUM_ENABLED_AT_CHANNELS); inUseEntityDataItemCount++)
    {
      if ((atciContextDataPtr->inUseEntityMemDataItem[inUseEntityDataItemCount].inUse == TRUE) &&
          (&(atciContextDataPtr->inUseEntityMemDataItem[inUseEntityDataItemCount].entityContextData) == *dataPtr))
      {
        atciContextDataPtr->inUseEntityMemDataItem[inUseEntityDataItemCount].inUse = FALSE;
        found = TRUE;
      }          
    }

    if (found)
    {
      *dataPtr = PNULL;
    }

    /* Must find something */
    FatalAssert(found);
  }
}
/*--------------------------------------------------------------------------
 *
 * Function:    ackDataInd
 *
 * Parameters:  in:  Void
 *
 * Returns:     Void
 *
 * Description:  We have finished processing a SIG_CIMUX_AT_READY_IND
 *
 *-------------------------------------------------------------------------*/

void atReadyInd (void)
{  
    SignalBuffer signal = kiNullBuffer;
    KiCreateZeroSignal(SIG_CIMUX_AT_READY_IND, sizeof(EmptySignal), &signal);
    KiSendSignal (MUX_TASK_ID, &signal);

}

#if defined (ATCI_ENABLE_DYN_AT_BUFF)
/*************************************************************************
*
* Function:     vgAllocAtCmdBuffer
*
* Parameters:   Entity
*
* Returns:      nothing
*
* Description:  Allocates static RAM buffer for AT command string.
*
*************************************************************************/
void vgAllocAtCmdBuffer             (const VgmuxChannelNumber entity)
{
  Int8 counter;
  Boolean found = FALSE;
  ScanParseContext_t *scanParseContext_p = ptrToScanParseContext (entity);
  CrManagerGenericContext_t   *crManagerGenericContext     = ptrToCrManagerGenericContext();

#if defined(ATCI_DEBUG)
  printf ("ATCI: Alloc AT CMD buffer: %d", entity);
#endif

  FatalAssert(scanParseContext_p->nextCommand.character == PNULL);
  /* Search for entity data item not in use */
  for (counter=0; !found && (counter < ATCI_MAX_NUM_AT_CMD_BUFFERS); counter++)
  {
    if (crManagerGenericContext->atCmdBufferList[counter].inUse == FALSE)
    {
      crManagerGenericContext->atCmdBufferList[counter].inUse = TRUE;
      
      scanParseContext_p->nextCommand.character = &(crManagerGenericContext->atCmdBufferList[counter].charBuff[0]);
      found = TRUE;
    }          
  }

  if (!found)
  {
    /* Must find buffer */
    FatalParam(found, entity, scanParseContext_p->nextCommand.character);
  }
  FatalAssert(found);
}

/*************************************************************************
*
* Function:     vgFreeAtCmdBuffer
*
* Parameters:   Entity
*
* Returns:      nothing
*
* Description:  Allocates static RAM buffer for AT command string.
*
*************************************************************************/
void vgFreeAtCmdBuffer              (const VgmuxChannelNumber entity)
{
  Int8 counter;
  Boolean found = FALSE;
  ScanParseContext_t *scanParseContext_p = ptrToScanParseContext (entity);
  CrManagerGenericContext_t   *crManagerGenericContext     = ptrToCrManagerGenericContext();

  WarnAssert(scanParseContext_p->nextCommand.character != PNULL);

#if defined(ATCI_DEBUG)
  printf ("ATCI: Free AT CMD buffer: %d", entity);
#endif

  if (scanParseContext_p->nextCommand.character != PNULL)
  {
    /* Search for entity data item not in use */
    for (counter=0; !found && (counter < ATCI_MAX_NUM_AT_CMD_BUFFERS); counter++)
    {
      if ((crManagerGenericContext->atCmdBufferList[counter].inUse == TRUE) &&
          (scanParseContext_p->nextCommand.character == &(crManagerGenericContext->atCmdBufferList[counter].charBuff[0])))
      {
        /* Found the one to free */
        crManagerGenericContext->atCmdBufferList[counter].inUse = FALSE;
        scanParseContext_p->nextCommand.character = PNULL;
        found = TRUE;
      }          
    }

    if (!found)
    {
      /* Must find buffer */
      FatalParam(found, entity, scanParseContext_p->nextCommand.character);
    }
  }  
}

#endif

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */



