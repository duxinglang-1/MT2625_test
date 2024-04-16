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
 * SMS Interface controller
 **************************************************************************/

#define MODULE_NAME "RVMSSS"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvsystem.h>
#include <rvutil.h>
#include <cici_sig.h>
#include <rvmsss.h>
#include <rvmsut.h>
#include <rvmscmds.h>
#include <rvmssigi.h>
#include <rvmssigo.h>
#include <rvdata.h>
#include <rvmsdata.h>
#if defined (DEVELOPMENT_VERSION)
#  include <stdio.h>
#endif
#include <absm_sig.h>
#include <vgmx_sig.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  CiRunAtCommandInd      ciRunAtCommandInd;
};

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

typedef void (*sigCnfProc)(const VgmuxChannelNumber entity, const union Signal *signal);
typedef void (*sigCnfProcNoEntity)(const union Signal *signal);

typedef struct SignalFuncTag
{
  SignalId     signalId;  /* signal identifier */
  sigCnfProc   procFunc;  /* signal processing function */
}
SignalFunc;

typedef struct SignalFuncNoEntityTag
{
  SignalId             signalId;  /* signal identifier */
  sigCnfProcNoEntity   procFunc;  /* signal processing function */
}
SignalFuncNoEntity;

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/


/* AT command handlers */
const AtCmdControl msAtCommandTable[] =
{
    { ATCI_CONST_CHAR_STR "+CMGD", vgSmsCmgdDeleteSms,                  VG_AT_SMS_CMGD,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT}, /* Delete SMS message                          */
    { ATCI_CONST_CHAR_STR "+CMGR", vgSmsCmgrReadSms,                    VG_AT_SMS_CMGR,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT}, /* Read SMS message                            */
    { ATCI_CONST_CHAR_STR "*MCMGR", vgSmsMcmgrReadSms,                  VG_AT_SMS_MCMGR,    AT_CMD_ACCESS_SIM_READY_AND_PRESENT}, /* Read SMS message                            */    
    { ATCI_CONST_CHAR_STR "+CSCA", vgSmsCscaServiceCentreAddressSim,    VG_AT_SMS_CSCA,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT}, /* SMS service center address                  */
    { ATCI_CONST_CHAR_STR "+CMGS", vgSmsCmgsSendSms,                    VG_AT_SMS_CMGS,     AT_CMD_ACCESS_NONE}, /* Send SMS message                            */
    { ATCI_CONST_CHAR_STR "+CMGC", vgSmsCmgcSendSmsCommand,             VG_AT_SMS_CMGC,     AT_CMD_ACCESS_NONE}, /* Send SMS Command                            */
    { ATCI_CONST_CHAR_STR "+CMSS", vgSmsCmssSendFromSim,                VG_AT_SMS_CMSS,     AT_CMD_ACCESS_NONE}, /* Send SMS message from SIM                   */
    { ATCI_CONST_CHAR_STR "+CMGL", vgSmsCmglListSms,                    VG_AT_SMS_CMGL,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT}, /* List SMS messages from preferred store      */
    { ATCI_CONST_CHAR_STR "+CMGW", vgSmsCmgwStoreSms,                   VG_AT_SMS_CMGW,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT}, /* Write SMS message to memory                 */
    { ATCI_CONST_CHAR_STR "+CPMS", vgSmsCpmsPreferredSmsMessageStorage, VG_AT_SMS_CPMS,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT}, /* Preferred SMS Message Storage               */
    { ATCI_CONST_CHAR_STR "+CNMA", vgSmsCnmaNewMessageAcknowledgment,   VG_AT_SMS_CNMA,     AT_CMD_ACCESS_NONE}, /* New message acknowledgement                 */
    { ATCI_CONST_CHAR_STR "+CMMS", vgSmsCmmsSendMoreSms,                VG_AT_SMS_CMMS,     AT_CMD_ACCESS_NONE}, /* Enabling MoreMsgsToSend functionality       */
    { ATCI_CONST_CHAR_STR "*MSMSTATUS", vgSmsMSmStatus,                 VG_AT_SMS_MSMSTATUS, AT_CMD_ACCESS_CFUN_1},
    { ATCI_CONST_CHAR_STR "*MMGRW",vgSmsMmgrwSelLoc,                    VG_AT_SMS_MMGRW,    AT_CMD_ACCESS_CFUN_1 },/* Select a SMS location in SIM                */
    { ATCI_CONST_CHAR_STR "*MMGSC",vgSmsMmgscChangeLocStatus,           VG_AT_SMS_MMGSC,    AT_CMD_ACCESS_CFUN_1 },/* Change current SMS location stsuts          */
    { ATCI_CONST_CHAR_STR PNULL,   PNULL,                               VG_AT_LAST_CODE,    AT_CMD_ACCESS_NONE}    /* end of table marker */
};

/* AT command handlers for configuration commands i.e. those that can be run
   before the SMS BL has been initialised. */
const AtCmdControl msAtConfigCommandTable[] =
{
    { ATCI_CONST_CHAR_STR "*MMGI", vgSmsMMGI,                           VG_AT_SMS_MMGI,     AT_CMD_ACCESS_NONE  }, /* Enable SMSPP unsolicited events             */

#if defined (FEA_PHONEBOOK)      
    { ATCI_CONST_CHAR_STR "*MSMALPHAID",   vgSmsMSmAlphaId,             VG_AT_SMS_MSMALPHAID, AT_CMD_ACCESS_NONE},
#endif /* FEA_PHONEBOOK */

    { ATCI_CONST_CHAR_STR "*MSMEXTRAINFO",  vgSmsMSmExtraInfo,          VG_AT_SMS_MSMEXTRAINFO,  AT_CMD_ACCESS_NONE},
    { ATCI_CONST_CHAR_STR "*MSMEXTRAUNSOL", vgSmsMSmExtraUnsol,         VG_AT_SMS_MSMEXTRAUNSOL, AT_CMD_ACCESS_NONE},
    { ATCI_CONST_CHAR_STR PNULL,           PNULL,                       VG_AT_LAST_CODE,       AT_CMD_ACCESS_NONE}  /* end of table marker */
};


/* Input confirmations signals */
const SignalFunc signalFuncsNormal[] =
{
    { SIG_APEX_SM_DELETE_CNF          , (sigCnfProc)vgSmsSigInApexSmDeleteCnf       },
    { SIG_APEX_SM_DELETE_SMSR_CNF     , (sigCnfProc)vgSmsSigInApexSmDeleteSmsrCnf     },
    { SIG_APEX_SM_READ_CNF            , (sigCnfProc)vgSmsSigInApexSmReadCnf         },
    { SIG_APEX_SM_SEND_CNF            , (sigCnfProc)vgSmsSigInApexSmSendCnf         },
    { SIG_APEX_SM_COMMAND_CNF         , (sigCnfProc)vgSmsSigInApexSmCommandCnf      },
    { SIG_APEX_SM_READ_SMSR_CNF       , (sigCnfProc)vgSmsSigInApexSmReadSmsrCnf       },
    { SIG_APEX_SM_READ_SMSP_CNF       , (sigCnfProc)vgSmsSigInApexSmReadSmspCnf     },
    { SIG_APEX_SM_WRITE_SMSP_CNF      , (sigCnfProc)vgSmsSigInApexSmWriteSmspCnf    },
    { SIG_APEX_SM_SEND_FROM_SIM_CNF   , (sigCnfProc)vgSmsSigInApexSmSendFromSimCnf  },
    { SIG_APEX_SM_STORE_CNF           , (sigCnfProc)vgSmsSigInApexSmStoreCnf        },
    { SIG_APEX_SM_STATUS_CNF          , (sigCnfProc)vgSmsSigInApexSmStatusCnf       },
    { SIG_APEX_SM_READ_NODATA_CNF     , (sigCnfProc)vgSmsSigInApexSmReadNoDataCnf   },
    { SIG_APEX_SM_READ_SMSR_NODATA_CNF, (sigCnfProc)vgSmsSigInApexSmReadSmsrNoDataCnf },
    { SIG_APEX_SM_STORE_LOC_CNF       , (sigCnfProc)vgSmsSigInApexSmStoreLocCnf     },
    { SIG_APEX_SM_SET_LOC_STATUS_CNF  , (sigCnfProc)vgSmsSigInApexSmSetLocStatusCnf },
    { SIG_APEX_SM_STK_INFO_IND        , (sigCnfProc)vgSmsSigInApexSmStkInfoInd      },
};

/* Unsolicited input signals */
const SignalFuncNoEntity signalFuncUnsolicited[] =
{
    { SIG_APEX_SM_READY_IND           , (sigCnfProcNoEntity)vgSmsSigInApexSmReadyInd         },
    { SIG_APEX_SM_STATUS_REPORT_IND   , (sigCnfProcNoEntity)vgSmsSigInApexSmStatusReportInd  },
    { SIG_APEX_SM_SMSR_STORE_IND      , (sigCnfProcNoEntity)vgSmsSigInApexSmSmsrStoreInd     },
    { SIG_APEX_SM_SR_DELETED_IND      , (sigCnfProcNoEntity)vgSmsSigInApexSmSrDeletedInd     },
    { SIG_APEX_SM_RECORD_CHANGED_IND  , (sigCnfProcNoEntity)vgSmsSigInApexSmRecordChangedInd },
    { SIG_APEX_SM_MSG_RECEIVED_IND    , (sigCnfProcNoEntity)vgSmsSigInApexSmMsgReceivedInd   },
    { SIG_APEX_SM_STORE_IND           , (sigCnfProcNoEntity)vgSmsSigInApexSmStoreInd         },
    { SIG_APEX_SM_TRANSACTION_END_IND , (sigCnfProcNoEntity)vgSmsSigInApexTransactonEndInd   },
    { SIG_APEX_SM_DELIVERY_IND        , (sigCnfProcNoEntity)vgSmsSigInApexSmDeliveryInd      },
    { SIG_APEX_SM_STATUS_CHANGED_IND  , (sigCnfProcNoEntity)vgSmsSigInApexSmStatusChangedInd }
};



/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/


/***************************************************************************
 * Local Functions
 ***************************************************************************/


 /****************************************************************************
 *
 * Function:    processCommandSignals
 *
 * Parameters:  entity - mux channel number
 *              signal_p - signal to be processed
 *
 *
 * Returns:     Boolean - TRUE if signal was handled, or false if ignored.
 *
 * Description:
 *
 * If this is a normal confirmation signal pass it to the
 * correct handler.
 *
 ****************************************************************************/
static Boolean processCommandSignals (const VgmuxChannelNumber entity, const SignalBuffer *signal_p)
{
  Int32   aloop;
  Boolean accepted = FALSE;

  /*
  ** If this is a normal confirmation signal pass it to the
  ** correct handler.
  */
  for (aloop = 0; ( (aloop < VG_ARRAY_LENGTH(signalFuncsNormal)) &&
                   (accepted == FALSE) ); aloop++)
  {
    if (signalFuncsNormal[aloop].signalId == *(signal_p->type) )
    {
      (signalFuncsNormal[aloop].procFunc)(entity, signal_p->sig);
      accepted = TRUE;
    }
  }
  return (accepted);
}


 /****************************************************************************
 *
 * Function:    processUnsolicitedSignals
 *
 * Parameters:  entity - mux channel number
 *              signal_p - signal to be processed
 *              sendSignalBackToCi -
 *
 *
 * Returns:     Boolean - TRUE if signal was handled, or false if ignored.
 *
 * Description:
 *
 * If the signal was not previously accepted (above) then this must
 * be an unsolicited indication.
 *
 * When an unsolicited indication is received it may fire off other
 * signals (doing an alphaId lookup for example) in which case we
 * need to ensure we only process one unsolicited indication at a
 * time, and only process further signals once we have finished
 * with the current one. To do this we set a busy flag when we
 * start processing an unsolicited indication, if we are not busy
 * already, and clear it when we have finished (this also has the
 * effect of releasing control of any background layer procedures
 * we may have got control of). If we are busy already we send
 * the signal back to the CI task.
 *
 ****************************************************************************/

static Boolean processUnsolicitedSignals (const VgmuxChannelNumber entity,
                                           const SignalBuffer *signal_p, Boolean *sendSignalBackToCi)
{
  Int32   aloop;
  Boolean accepted = FALSE;

  PARAMETER_NOT_USED(entity);

  for (aloop = 0; ( (aloop < VG_ARRAY_LENGTH(signalFuncUnsolicited)) &&
                    (accepted == FALSE) ); aloop++)
  {
    if (signalFuncUnsolicited[aloop].signalId == *(signal_p->type) )
    {
      if (vgSmsUtilIsUnsolicitedChannelBusy())
      {
        *sendSignalBackToCi = TRUE;
      }
      else
      {
        /* Not busy, so process indication... */
        vgSmsUtilSetUnsolicitedEventHandlerBusy();

        (signalFuncUnsolicited[aloop].procFunc)(signal_p->sig);
        *sendSignalBackToCi = FALSE;
      }
      accepted = TRUE;
    }
  }

  return (accepted);
}

 /****************************************************************************
 *
 * Function:    processSignal
 *
 * Parameters:  entity - mux channel number
 *              signal_p - signal to be processed
 *              sendSignalBackToCi - set to TRUE if signal is
 *              unsolicited and cannot be processed so should be
 *              sent back to the CI task for later processing.
 *
 * Returns:     Boolean - TRUE if signal was handled, or false if ignored.
 *
 * Description: Check to see if a command signal and if so process it
 *              otherwise check to see if an unsolicited signal and
 *              process it.
 *
 ****************************************************************************/
static Boolean processSignal (const VgmuxChannelNumber entity,
                              const SignalBuffer *signal_p,
                              Boolean *sendSignalBackToCi)
{
  /* Is this a command signal? */
  Boolean accepted = processCommandSignals (entity, signal_p);

  if (FALSE == accepted)
  {
    /* No, so is this an unsolicited signal? */
    accepted = processUnsolicitedSignals (entity, signal_p, sendSignalBackToCi);
  }
  else
  {
    *sendSignalBackToCi = FALSE;
  }

  return (accepted);
}


 /****************************************************************************
 *
 * Function:    compareNoCase
 *
 * Parameters:  s1, len1 - string with length
 *              s2, len2 - string with length
 *
 * Returns:     Boolean - TRUE if string are the same but not
 *                        if either is zero length
 *
 * Description: Compare strings for equality.
 *
 ****************************************************************************/
static Boolean compareNoCase(const Char *s1, Int16 len1, const Char *s2, Int16 len2)
{
  Boolean match = FALSE;
  Int32   i;
  if ((len1 == len2) && (0 != len1) && (0 != len2))
  {
    i = 0;
    match = TRUE;
    while ( (i<len1) && (TRUE == match) )
    {
      if ( toupper(s1[i]) != toupper(s2[i]) )
      {
        match = FALSE;
      }
      i++;
    }
  }
  return (match);
}

 /****************************************************************************
 *
 * Function:    getCommandLength
 *
 * Parameters:  commandBuffer_p - pointer to the command buffer
 *
 * Returns:     length of the command
 *
 * Description: gets the length of a command, which is the length of
 *              it until a non-alpha char is reached but skipping any
 *              non-alpha chars it may start with.
 *
 ****************************************************************************/
static Int16 getCommandLength (CommandLine_t *commandBuffer_p)
{
  Int16 cmdLength = 0;
  /* get length of command - number of alpha characters, but first
     skipping any non-alpha chars */
  while ( (cmdLength < commandBuffer_p->length) &&
          (!isalpha(commandBuffer_p->character[cmdLength])) )
  {
    cmdLength++;
  }
  while ( (cmdLength < commandBuffer_p->length) &&
          (isalpha(commandBuffer_p->character[cmdLength])) )
  {
    cmdLength++;
  }
  return (cmdLength);
}


 /****************************************************************************
 *
 * Function:    findCommand
 *
 * Parameters:  entity - entity number
 *              atCommandTable_p - pointer to the list of commands to
 *                                 check against
 *
 * Returns:     CommandId of an SMS command
 *
 * Description: Find command in supplied table i.e. check to see
 *              if it is an SMS command.
 *
 ****************************************************************************/
static CommandId_t findCommand (const VgmuxChannelNumber entity, AtCmdControl const *atCommandTable_p)
{
  Boolean            commandFound = FALSE;
  ScanParseContext_t *scanParseContext_p = ptrToScanParseContext (entity);
  CommandLine_t      *commandBuffer_p;
  Char*              atCommand;
  Int16              atCommandLength;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(scanParseContext_p != PNULL, entity, 0, 0);
#endif
  commandBuffer_p = &scanParseContext_p->nextCommand;
  atCommand = &commandBuffer_p->character[0];
  atCommandLength = getCommandLength(commandBuffer_p);

  while ( (PNULL != atCommandTable_p->string) && (!commandFound) )
  {
    commandFound = compareNoCase (atCommandTable_p->string, (Int16)strlen((char*)atCommandTable_p->string),
                                  atCommand, atCommandLength);
    if (!commandFound)
    {
      atCommandTable_p++;
    }
  }
  return (atCommandTable_p->commandId);
}


/***************************************************************************
 * Global Functions
 ***************************************************************************/



 /****************************************************************************
 *
 * Function:    vgMsssInterfaceController
 *
 * Parameters:  signal_p - signal to be processed
 *              entity - mux channel number
 *
 * Returns:     Boolean - TRUE if signal was handled, or false if ignored.
 *
 * Description: Determines action for received signals
 *
 ****************************************************************************/

Boolean vgMsssInterfaceController (const SignalBuffer *signal_p,
                                   const VgmuxChannelNumber entity)
{
  Boolean  accepted = FALSE;
  CommandId_t atCommand = VG_AT_LAST_CODE;
  SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext();

  /* Signal Handler */
  switch (*(signal_p->type))
  {
    case SIG_CI_RUN_AT_COMMAND_IND:
    {
      accepted = parseCommandBuffer (msAtConfigCommandTable, entity);
      if (FALSE == accepted)
      {
        atCommand = findCommand(entity, msAtCommandTable);

        /* If the command is not a config command then check to see if
        ** the SMS BL is ready (has been initialised), if not, and it
        ** is an SMS command return an error.
        */
        if ((smsCommonContext_p->smSimState.requestStatus == SM_REQ_SM_NOT_READY) ||
          /* we SIM is not ready we can accept only CMGS */
           ((smsCommonContext_p->smSimState.requestStatus != SM_REQ_OK) &&
            (atCommand != VG_AT_SMS_CMGS)))
        {
          if (atCommand != VG_AT_LAST_CODE)
          {
            setResultCode (entity, VG_CMS_ERROR_SM_NOT_READY);
            accepted = TRUE;
          }
        }
        else /* run AT command */
        {
          accepted = parseCommandBuffer (msAtCommandTable, entity);
        }
      }
      break;
    }

    case SIG_INITIALISE:
    {
      initCommonEntityData ();
      break;
    }

    case SIG_CIMUX_CHANNEL_ENABLE_IND:
    {
      vgSmsInitialiseData (entity);
      break;
    }

    default:
    {
      accepted = processSignal (entity, signal_p, &(smsCommonContext_p->requeueSignal));
      break;
    }
  }
  return (accepted);
}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

