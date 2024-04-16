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
 * Header file for rvccut.c
 *
 * Procedures for Call Control AT command execution
 * Handler for AT commands located in vgccss.c
 *
 * Implemented commands:
 *
 * ATDL     - Redials last number dialled
 * ATD      - Dials a given number
 * ATA      - Answers an incoming call
 * ATH      - Hangs up connected calls
 * ATO      - switches back to data mode
 **************************************************************************/

#define MODULE_NAME "RVCCUT"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <gkisig.h>
#include <gkimem.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvprof.h>
#include <rvccut.h>
#include <rvpdsigo.h>
#include <rvpdsigi.h>
#include <rvchman.h>
#include <rvoman.h>
#include <rvgncpb.h>
#include <rvomtime.h>
#include <rvccsigi.h>
#include <rvgnsigi.h>
#include <rvcimxsot.h>
#include <rvssdata.h>
#include <rvcrhand.h>
#include <rvcrconv.h>
#include <rvcimxut.h>
#if defined (UPGRADE_3G)
#include <rvmmut.h>
#endif
#include <rvcmux.h>
#include <rvcimxut.h>
#include <rvcimux.h>
#include <rvgput.h>
#include <rvaput.h>
#include <rvapsigo.h>

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

Boolean vgExtractDialDigits (CommandLine_t *commandBuffer_p,
                              Char *outputBuffer_p,
                               Int16 length,
                                const VgmuxChannelNumber entity);

Boolean vgExtractDialDigitsWithPause (CommandLine_t *commandBuffer_p,
                                       Char *outputBuffer_p,
                                        Int16 length,
                                         Char *outputBufferAfterPause_p,
                                          Int32 *pauseCount_p,
                                           const VgmuxChannelNumber entity);

#if defined (UPGRADE_3G)
ResultCode_t vgCcRejectSyncCSDCallWithCause(const VgmuxChannelNumber entity);
#endif

#define localGetProfileValue getProfileValue

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Variables
 ***************************************************************************/

/***************************************************************************
 * Macros
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/


#define NUM_OF_VALID_DIGITS             (16)

static const Char validDigits[NUM_OF_VALID_DIGITS] =
{
  '0','1','2','3','4','5','6','7','8','9','*','#','A','B','C','D'
};


/***************************************************************************
 * Signal definitions
 ***************************************************************************/
union Signal
{
  CiRunAtCommandInd           ciRunAtCommandInd;
};

/*--------------------------------------------------------------------------
 *
 * Function:    vgExtractDialDigits
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              outputBuffer_p  - string to contain extracted dial digits
 *              length          - maximum length of dial digits
 *              entity          - entity
 *
 * Returns:     Boolean - indicating whether dial digits were succesfully
 *                        extracted and characters present are valid.
 *
 * Description: Attempts to extract dial digits from the command buffer
 *              string.  Invalid characters are rejected.
 *
 *-------------------------------------------------------------------------*/

Boolean vgExtractDialDigits (CommandLine_t *commandBuffer_p,
                              Char *outputBuffer_p,
                               Int16 length,
                                const VgmuxChannelNumber entity)
{
  Char  character;
  Int8  index;
  Int16 position = 0;
  Boolean result = TRUE;
  const Int8 crChar = (Int8)getProfileValue(entity, PROF_S3);
  const Int8 lfChar = (Int8)getProfileValue(entity, PROF_S4);

  /* cycle through characters in command buffer */
  while ((commandBuffer_p->position < commandBuffer_p->length) &&
         (position < (length - 1)) &&
         ((commandBuffer_p->character[commandBuffer_p->position] != SEMICOLON_CHAR) &&
          (commandBuffer_p->character[commandBuffer_p->position] != crChar) &&
          (commandBuffer_p->character[commandBuffer_p->position] != lfChar)))
  {
    character = commandBuffer_p->character[commandBuffer_p->position];
    commandBuffer_p->position++;

    /* check for international prefix */
    if ((character == INTERNATIONAL_PREFIX) && (position == 0))
    {
      outputBuffer_p[position] = character;
      position++;
    }
    /* ignore space characters..... */
    else if (character == SPACE_CHAR)
    {
      /* We just strip out the space character */
    }
    else if (character == COMMA_CHAR)
    {
       result = FALSE;
    }
    else /* used valDig  */
    {
      index = 0;
      while ((index < NUM_OF_VALID_DIGITS) &&
            (character != validDigits[index]))
      {
        index++;
      }
      if (index < NUM_OF_VALID_DIGITS)
      {
        outputBuffer_p[position] = character;
        position++;
      }

      else
      {
        result = FALSE;
      }
    }
  }

  outputBuffer_p[position] = NULL_CHAR;

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgExtractDialDigitsWithPause
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              outputBuffer_p  - string to contain extracted dial digits
 *              length          - maximum length of dial digits
 *              entity          - entity
 *
 * Returns:     Boolean - indicating whether dial digits were succesfully
 *                        extracted and characters present are valid.
 *
 * Description: Attempts to extract dial digits from the command buffer
 *              string.  Invalid characters are rejected.
 *              Handles a comma in the dialing line to indicate the pause.
 *              Implemented as defined in V.250, 6.3.1.2.
 *              The digits before the comma are dialled immediately as a CSV call
 *              ( therefore, the comma is replaced by the semicolon ), the rest of
 *              the digits are copied into the buffer and dialled as DTMF digits over
 *              existing voice connection with a S8 timeout after the call is connected.
 *              As the specs are not clear whether  i, I, g and G characters should
 *              appear before of after the comma, we assume that both possibilities
 *              are valid, and therefore copy them to the dialled string even if they
 *              appear after the comma.
 *              We currently assume that only one comma can be present in the dialled
 *              string, therefore the second comma in the string would be considered
 *              invalid.
 *              The comma handling is implemented only for ATD command and not for AT+VTS.
 *              It does not make sense for special numbers such as *99#, which is why it
 *              is only implemented for ATD.
 *-------------------------------------------------------------------------*/

Boolean vgExtractDialDigitsWithPause (CommandLine_t *commandBuffer_p,
                                       Char *outputBuffer_p,
                                        Int16 length,
                                         Char *outputBufferAfterPause_p,
                                          Int32 *pauseCount_p,
                                           const VgmuxChannelNumber entity)
{
  Char  character;
  Int8  index;
  Int16 position = 0;
  Boolean result = TRUE;
  const Int8 crChar = (Int8)getProfileValue(entity, PROF_S3);
  const Int8 lfChar = (Int8)getProfileValue(entity, PROF_S4);
  Char *buffer_p = outputBuffer_p;
  Int16 commandBufferPausePosition = 0;

  Int32 pauseCount = 0;

  /* cycle through characters in command buffer */
  while ((commandBuffer_p->position < commandBuffer_p->length) &&
         (position < (length - 1)) &&
         ((commandBuffer_p->character[commandBuffer_p->position] != SEMICOLON_CHAR) &&
          (commandBuffer_p->character[commandBuffer_p->position] != crChar) &&
          (commandBuffer_p->character[commandBuffer_p->position] != lfChar)))
  {
    character = commandBuffer_p->character[commandBuffer_p->position];
    commandBuffer_p->position++;

    /* check for international prefix */
    if ((character == INTERNATIONAL_PREFIX) && (position == 0))
    {
      buffer_p[position] = character;
      position++;
    }
    /* ignore space characters..... */
    else if (character == SPACE_CHAR)
    {
      /* We just strip out the space character */
    }
    else if ((character == ATD_PAUSE_DTMF_UPPER) || (character == ATD_PAUSE_DTMF_LOWER))
    {
      if ( commandBufferPausePosition != 0)
      {
        /* Subsequent pause, assume pause are sent continuously */
        pauseCount++;
        /* Currently, maximux supported pause in ATD is MAX_PAUSE_SEPARATOR */
        if (pauseCount <= MAX_PAUSE_SEPARATOR)
        {
          buffer_p = &outputBufferAfterPause_p[(pauseCount - 1)* (MAX_CALLED_BCD_NO_LENGTH + NULL_TERMINATOR_LENGTH)];
          position = 0;
        }
        else
        {
          result = FALSE;
        }
      }
      else
      {
        /* First pause */
        commandBuffer_p->character[commandBuffer_p->position-1] = SEMICOLON_CHAR;
        buffer_p[position] = NULL_CHAR;
        commandBufferPausePosition = commandBuffer_p->position-1;

        pauseCount++;
        buffer_p = &outputBufferAfterPause_p[(pauseCount - 1) * (MAX_CALLED_BCD_NO_LENGTH + NULL_TERMINATOR_LENGTH)];
        position = 0;
      }
    }
    else /* used valDig  */
    {
      index = 0;
      while ((index < NUM_OF_VALID_DIGITS) &&
            (character != validDigits[index]))
      {
        index++;
      }
      if (index < NUM_OF_VALID_DIGITS)
      {
        buffer_p[position] = character;
        position++;
      }

      else
      {
        /* If character is comma, we ignore the character */
        if(COMMA_CHAR != character)
        {
          result = FALSE;
        }
      }
    }
  }

  buffer_p[position] = NULL_CHAR;
  if ( commandBufferPausePosition != 0)
  {
    if(commandBuffer_p->character[commandBuffer_p->position] != SEMICOLON_CHAR)
    {
      result = FALSE;
    }
    commandBuffer_p->position = commandBufferPausePosition;
  }

  *pauseCount_p = pauseCount;

  return (result);
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:        vgCharToBcdNumberType
 *
 * Parameters:      typeOfNumber - VG_DIAL_NUMBER_INTERNATIONAL or other
 *
 * Returns:         BcdNumberType for the character
 *
 * Description:     Return dial number type depending on number type.
 *
 *-------------------------------------------------------------------------*/

BcdNumberType vgCharToBcdNumberType (const VgDialNumberType typeOfNumber)
{
  BcdNumberType type = NUM_TYPE_UNKNOWN;

  switch (typeOfNumber)
  {
    case VG_DIAL_NUMBER_INTERNATIONAL:
    {
      type = NUM_TYPE_INTERNATIONAL;
      break;
    }
    case VG_DIAL_NUMBER_NATIONAL:
    {
      type = NUM_TYPE_NATIONAL;
      break;
    }
    case VG_DIAL_NUMBER_NET_SPECIFIC:
    {
      type = NUM_TYPE_NETWORK_SPEC;
      break;
    }
    case VG_DIAL_NUMBER_UNKNOWN:
    case VG_DIAL_NUMBER_RESTRICTED:
    default:
    {
      type = NUM_TYPE_UNKNOWN;
      break;
    }
  }

  return (type);
}

#if defined (FEA_MT_PDN_ACT)
/*--------------------------------------------------------------------------
 *
 * Function:    vgCcA
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: This function executes the ATA command which establishes
 *              answers an incoming call.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgCcA (CommandLine_t *commandBuffer_p,
                     const VgmuxChannelNumber entity)
{
  ResultCode_t            result                = RESULT_CODE_OK;
  OpmanContext_t          *opManContext_p       = ptrToOpManContext (entity);
  GprsGenericContext_t    *gprsGenericContext_p = ptrToGprsGenericContext ();
  Int32                   cid                   = 0;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(opManContext_p != PNULL, entity, 0, 0);
#endif
  PARAMETER_NOT_USED(commandBuffer_p);

  if (vgPdIncomingPdpContextActivation (entity) &&
           vgPdMayAnswerWithA (entity))
  {
    /* there's an outstanding request for mt pdp context activation
       which we should handle */
    /* job127550: use transient CID rather than defined CID */

    /* Check if a cid is configured by MMTPDPCID to be the default cid for
     * MT PDP contexts. If one is configured, use it. Otherwise use transient cid.
     */
    if (gprsGenericContext_p->vgMMTPDPCIDData.enabled)
    {
      cid = gprsGenericContext_p->vgMMTPDPCIDData.cid;
    }
    else
    {
      if(vgGetFreeCid(&cid, entity) == FALSE)
      {
        result = VG_CME_NO_FURTHER_CIDS_SUPPORTED;
      }
    }

    if(result == RESULT_CODE_OK)
    {
      result = vgPdAnswer (entity, cid,
                           (AbpdPdpConnType)getProfileValue(entity, PROF_MGMTPCACT));
    }
  }
  else /* no incoming call */
  {
    result = VG_CME_OPERATION_NOT_ALLOWED;
  }

  /* other commands are not allowed to be concatenated on the end of the line */
  commandBuffer_p->position = commandBuffer_p->length;

  return (result);
}
#endif /* FEA_MT_PDN_ACT */

/*--------------------------------------------------------------------------
 *
 * Function:    vgCcCHUP
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: This function executes the AT+CHUP command which terminates
 *              circuit-switched voice and data connections.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgCcCHUP (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
  CallContext_t       *callContext_p  = ptrToCallContext (entity);
  ResultCode_t        result          = RESULT_CODE_OK;
  ExtendedOperation_t operation       = getExtendedOperation (commandBuffer_p);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(callContext_p != PNULL, entity, 0, 0);
#endif
  switch (operation)
  {
    case EXTENDED_RANGE:     /* AT+CHUP=? */
    {
      break;
    }
    case EXTENDED_ACTION:    /* AT+CHUP */
    {
#if defined (FEA_MT_PDN_ACT)
      /* If there's a current incoming request for mtpca, ATH rejects
      it. 27.007 10.2.2.3 says that ATH is used for gprs _only_ to
      reject an incoming request: it is not used to deactivate a pdp
      context as you might expect. */
      if (vgPdIncomingPdpContextActivation (entity) &&
          vgPdMayAnswerWithA (entity))
      {
        /* there's an outstanding request for mt pdp context activation
        which we should reject */
        result = vgPdReject (entity);
      }
      else
#endif /* FEA_MT_PDN_ACT */
      {
        /* Not pending any call status check */
        result = vgContinueHangupRequest(entity);
      }
      break;
    }

    case EXTENDED_ASSIGN:    /* AT+CHUP= */
    case EXTENDED_QUERY:     /* AT+CHUP? */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCcH
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: This function executes the ATH command which terminates
 *              circuit-switched voice and data connections.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgCcH (CommandLine_t *commandBuffer_p,
                    const VgmuxChannelNumber entity)
{
  CallContext_t *callContext_p  = ptrToCallContext (entity);
  ResultCode_t  result = RESULT_CODE_OK;
  Int32         value;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(callContext_p != PNULL, entity, 0, 0);
#endif
  getDecimalValueSafe (commandBuffer_p, &value);
  /*
   * User can enter ATH0 or ATH1 only
   */
  callContext_p->athParam = (Int8)value;

  if (ATH_PARAM_DISCONNECT_NORMAL == value)
  {

#if defined (FEA_MT_PDN_ACT)
    /* If there's a current incoming request for mtpca, ATH rejects
       it. 27.007 10.2.2.3 says that ATH is used for gprs _only_ to
       reject an incoming request: it is not used to deactivate a pdp
       context as you might expect. */

    if (vgPdIncomingPdpContextActivation (entity) &&
        vgPdMayAnswerWithA (entity))
    {
      /* there's an outstanding request for mt pdp context activation
         which we should reject */
      result = vgPdReject (entity);
    }

    else
#endif /* FEA_MT_PDN_ACT */

    { 
      /* Not pending any call status check */
      result = vgContinueHangupRequest(entity);
    }
  }
  else if (ATH_PARAM_DISCONNECT_MO == value)
  {
    result = vgContinueHangupRequest(entity);
  }
  else
  {
    result = VG_CME_INVALID_TEXT_CHARS;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCcDL
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: This function executes the ATDL command which dials the last
 *              number dialled again
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgCcDL (CommandLine_t *commandBuffer_p,
                      const VgmuxChannelNumber entity)
{
  ResultCode_t   result = RESULT_CODE_PROCEEDING;
  OpmanContext_t *opManContext_p = ptrToOpManContext (entity);

  FatalCheck(opManContext_p != PNULL, entity, 0, 0);

  PARAMETER_NOT_USED(commandBuffer_p);

  /* check if a call connection request has been made before */
  if (opManContext_p->vgLastCallConnectionType != CONNECTION_TERMINATOR)
  {
    /* allocate a call connection, if possible */
    if (vgOpManAllocateConnection (
         entity,
          opManContext_p->vgLastCallConnectionType) == TRUE)
    {
      /* reset last call release error */
      vgSetApexGsmCallReleaseError (APEX_CAUSE_OK, TRUE, CAUSE_NORMAL_CLEARING, PD_CC, entity);

      /* start connection process depending on type of last call */
      switch (opManContext_p->vgLastCallConnectionType)
      {
#if defined (FEA_PPP)        
        case PPP_CONNECTION:
#endif /* FEA_PPP */          
        case PT_CONNECTION:
        {
          /* send a PSD dial request */
          result = vgChManContinueAction (entity, SIG_APEX_ABPD_DIAL_REQ);
          break;
        }
        default:
        {
          /* last call connection type set to invalid value */
          FatalParam (opManContext_p->vgLastCallConnectionType, 0, 0);
          /* reset type to terminator */
          opManContext_p->vgLastCallConnectionType = CONNECTION_TERMINATOR;
          /* return an result code indicating an error has occurred */
          result = RESULT_CODE_ERROR;
          break;
        }
      }
    }
    else /* unable to allocate a connection */
    {
      result = VG_CME_PHONE_LINK_RESERVED;
    }
  }
  else /* no call has been made before on this channel */
  {
    result = VG_CME_NOT_FOUND;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCcO
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: This function executes the ATO command which switches back to
 *              data mode if a data connection is conncted
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgCcO (CommandLine_t *commandBuffer_p,
                     const VgmuxChannelNumber entity)
{
  ResultCode_t    result = RESULT_CODE_OK;
  OpmanContext_t  *opManContext_p = ptrToOpManContext (entity);
  Int32           value;  

  const ConnectionInformation_t *callInfo_p;
  CallContext_t   *callContext_p = ptrToCallContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(opManContext_p != PNULL, entity, 0, 0);
  FatalCheck (callContext_p != PNULL, entity, 0, 0);
#endif
  /* this only applies to the null mux when we are going back to data mode */

  /* ATO will result in no action for entities where the data channel
   * is separate in the MUX, as we remain in AT command state during the data
   * connection.
   */

  if (vgDoesEntityHaveSeparateDataChannel(entity) == FALSE)

  {
    getDecimalValueSafe(commandBuffer_p, &value);

    result = RESULT_CODE_ERROR;

    if (value == 0)
    {
      /* search for a data call connected by this channel */
      if (opManContext_p->numberOfCallConnections > 0)
      {
        callInfo_p = &(opManContext_p->callInfo);

        if ((callInfo_p->vgState == CONNECTION_ON_LINE) &&
            (
#if defined (FEA_PPP)          
             (callInfo_p->vgClass == PPP_CONNECTION) ||
#endif /* FEA_PPP */             
             (callInfo_p->vgClass == PT_CONNECTION)))
        {
          /* set up data channel appropriately */
          setConnectionType (entity, callInfo_p->vgClass);

          sendResultCodeToCrm (RESULT_CODE_CONNECT, entity);

          /* reset connection type */
          setConnectionType (entity, CONNECTION_TERMINATOR);

          /* if using a multiplexer that does not have a data mode then
           * set the final result code to allow further command to be entered.
           * Setting to RESULT_CODE_PROCEEDING will not allow any AT commands
           * to be entered whilst in data mode
           */
          result = RESULT_CODE_PROCEEDING;

          /* further processing occurs when the CONNECT string has been sent to the
           * mux, see rvcrman.c */
        }
      }
      else
      {
        ApBridgeContext_t* apBridgeContext_p = ptrToApBridgeContext( entity );
        ApBridgeDataModeContext_t* apBridgeDataModeContext_p = vgApbGetDataModeContext(entity);
        if (apBridgeDataModeContext_p == PNULL)
        {
          return VG_CME_INVALID_TEXT_CHARS;
        }
        /* set up data channel appropriately */
        setConnectionType (entity, APBRIDGE_CONNECTION);
        apBridgeContext_p->operation = apBridgeDataModeContext_p->dataModeOperationMode;
        if (apBridgeDataModeContext_p->dataModeResultCode == RESULT_CODE_CONNECT)
        {
          sendResultCodeToCrm (RESULT_CODE_CONNECT, entity);
        }
        else
        {
          if (strlen((char*)(apBridgeDataModeContext_p->dataModeCustomResult)) > 0)
          {
            vgPutNewLine(entity);
            vgPutsWithLength(entity,(Char*)(apBridgeDataModeContext_p->dataModeCustomResult),
              strlen((char*)(apBridgeDataModeContext_p->dataModeCustomResult)));
            vgPutNewLine(entity);
            vgFlushBuffer(entity);
          }
          sendResultCodeToCrm (RESULT_CODE_CUSTOM_CONNECT, entity);
        }
        vgSigCiapbDataModeResumed(entity);
        /* reset connection type */
        setConnectionType (entity, CONNECTION_TERMINATOR);
        /* if using a multiplexer that does not have a data mode then
         * set the final result code to allow further command to be entered.
         * Setting to RESULT_CODE_PROCEEDING will not allow any AT commands
         * to be entered whilst in data mode
         */
        result = RESULT_CODE_PROCEEDING;
        /* further processing occurs when the CONNECT string has been sent to the
         * mux, see rvcrman.c */
      }
    }
    else
    {
      result = VG_CME_INVALID_TEXT_CHARS;
    }

  } /* switchable mux */

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCcDROPPEDTimerExpiry
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     nothing
 *
 * Description: terminates the dropped call tone when a connection was dropped
 *              by the dialled party
 *
 *-------------------------------------------------------------------------*/

void vgCcDROPPEDTimerExpiry (const VgmuxChannelNumber entity)
{
  /* Do nothing */
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCcCONNECTTimerExpiry
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     nothing
 *
 * Description: if a connection has not been made after the connect timer limit
 *              has been reached then abort the dial attempt
 *
 *-------------------------------------------------------------------------*/

#define SHORT_TIME (10)

void vgCcCONNECTTimerExpiry (const VgmuxChannelNumber entity)
{
  Int8    eIndex;
  Boolean allEntitiesIdle = TRUE;

  /* check if all active entities are idle */
  for (eIndex = 0; eIndex < CI_MAX_ENTITIES; eIndex++ )
  {
    if (isEntityActive ((VgmuxChannelNumber)eIndex))
    {
      if (getEntityState ((VgmuxChannelNumber) eIndex) != ENTITY_IDLE)
      {
        if ((eIndex == entity)                                                 &&
            (getCommandId ((VgmuxChannelNumber) eIndex) != VG_AT_CC_O)         &&
#if defined (FEA_MT_PDN_ACT)
            (getCommandId ((VgmuxChannelNumber) eIndex) != VG_AT_CC_A)         &&
#endif /* FEA_MT_PDN_ACT */
            (getCommandId ((VgmuxChannelNumber) eIndex) != VG_AT_CC_D)         &&
            (getCommandId ((VgmuxChannelNumber) eIndex) != VG_AT_CC_DL))
        {
          allEntitiesIdle = FALSE;
        }
      }
    }
  }

  if (allEntitiesIdle)
  {
    /* set call release error */
    vgSetTimerCallReleaseError (CI_CONNECT_TIMER_EXPIRED, entity);

    /* abort the connecting call on this channel */
    vgCcAbortConnectingCall (entity);
  }
  else
  {
    /* We need to restart the timer because there is something going on */

    setTimeOutPeriod (SHORT_TIME, TIMER_TYPE_CONNECT);

    vgCiStartTimer (TIMER_TYPE_CONNECT, entity);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCcAbortConnectingCall
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     nothing
 *
 * Description: aborts the MO connecting call, executed when the CONNECT
 *              timer has expired or when a key is pressed when a dial command
 *              is running
 *
 *-------------------------------------------------------------------------*/

void vgCcAbortConnectingCall (const VgmuxChannelNumber entity)
{
  OpmanContext_t        *opManContext_p       = ptrToOpManContext (entity);
  GprsContext_t         *gprsContext_p        = ptrToGprsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(gprsContext_p != PNULL, entity, 0, 0);

  FatalCheck(opManContext_p != PNULL, entity, 0, 0);
#endif
  if (
#if defined (FEA_PPP)    
      (opManContext_p->vgLastCallConnectionType == PPP_CONNECTION) ||
#endif /* FEA_PPP */      
      (opManContext_p->vgLastCallConnectionType == PT_CONNECTION))
  {
    /* check for PSD call on this channel */
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
          case CONNECTION_DIALLING:
          case CONNECTION_CONNECTING:
          {
            /* send a hangup request */
            vgOpManSetConnectionStatus (entity,
                                         opManContext_p->callInfo.vgIdent,
                                          CONNECTION_DISCONNECTING);
            gprsContext_p->vgHangupType = VG_HANGUP_TYPE_ATH;
            gprsContext_p->vgHangupCid  = vgFindCidWithDataConnLinkedToEntity(entity);
            vgChManContinueAction (entity, SIG_APEX_ABPD_HANGUP_REQ);
            terminateDataSession (entity, opManContext_p->callInfo.vgClass);


            break;
          }
          default:
          {
            break;
          }
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCallIsOfflineState
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     Boolean
 *
 * Description: Confirm whether or not call is in OFF_LINE state
 *
 *
 *-------------------------------------------------------------------------*/
Boolean vgCallIsOfflineState(const VgmuxChannelNumber entity)
{
  OpmanContext_t        *opManContext_p       = ptrToOpManContext (entity);
  Boolean               isOffline             = TRUE;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(opManContext_p != PNULL, entity, 0, 0);
#endif
#if defined (FEA_PPP)
  if (opManContext_p->vgLastCallConnectionType == PPP_CONNECTION)
  {
    if (opManContext_p->numberOfCallConnections > 0)
    {
      switch (opManContext_p->callInfo.vgClass)
      {
        case PPP_CONNECTION:
        {
          switch(opManContext_p->callInfo.vgState)
          {
            case CONNECTION_OFF_LINE:
            {
              isOffline = TRUE;
              break;
            }
            default:
            {
              isOffline = FALSE;
              break;
            }
          }
          break;
        }

        default:
        {
          isOffline = TRUE;
          break;
        }
      }
    }
  }
#endif /* FEA_PPP */

  return (isOffline);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgClearEntity
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     Nothing
 *
 * Description: Deallocates all entity arrays, sends DISABLE_RSP and disables
 *              the entity
 *
 *-------------------------------------------------------------------------*/

void vgClearEntity ( const VgmuxChannelNumber entity)
{
  OpmanGenericContext_t   *opManGenericContext_p = ptrToOpManGenericContext ();

  EntityContextData_t     *dPtr = ptrToEntityContextData (entity);

  ChannelContext_t        *channelContext_p = ptrToChannelContext(entity);
#if defined (ATCI_SLIM_DISABLE)
  FatalCheck (channelContext_p != PNULL, entity, 0, 0);
#endif
  /* We don't want to send a response to the MUX if it didn't send the
   * indication in the first place
   */
  if (channelContext_p->channelDisablingLocally == FALSE)
  {
    vgSendCiMuxChannelDisabledRsp(entity);
  }  

  FatalAssert ( dPtr != PNULL );

  if ( dPtr != PNULL )
  {
    resetCrConversionBuffer  (entity);
    resetCrOutputBuffer      (entity);
#if defined (FEA_PHONEBOOK)    
    resetCrAlphaStringBuffer (entity);
    resetCrAlphaSearchBuffer (entity);
#endif /* FEA_PHONEBOOK */
    resetCirmDataBuffer      (entity);

    vgFreeRamForEntity  (&dPtr);
    initialiseMemToEntity (entity);
  }

  if (opManGenericContext_p->numberOfEnabledChannels > 0)
  {
    opManGenericContext_p->numberOfEnabledChannels--;
  }
  else
  {
    /*
     * Deactivating a channel that is not there.
     */
    WarnParam(entity, opManGenericContext_p->numberOfEnabledChannels, 0);
  }

  setEntityState (entity, ENTITY_NOT_ENABLED);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCheckDisconnectAllProgress
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     TRUE, if all the calls have been disconnected and the entity is cleared.
 *
 * Description:  Checks whether all outstanding connections have been cleared
 *               to decide whether to clear the entity.
 *-------------------------------------------------------------------------*/

Boolean vgCheckDisconnectAllProgress (const VgmuxChannelNumber entity)
{
  OpmanGenericContext_t    *opManGenericContext_p = ptrToOpManGenericContext ();
  GprsContext_t            *gprsContext_p         = ptrToGprsContext(entity);
  CallContext_t            *callContext_p         = ptrToCallContext(entity);
  Boolean                  allCallsDisconnected   = FALSE;
  Boolean                  psdCallsDisconnecting  = FALSE;
#if defined (ATCI_SLIM_DISABLE)
  FatalCheck (opManGenericContext_p != PNULL, entity, 0, 0);
  FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
  FatalCheck (callContext_p != PNULL, entity, 0, 0);
#endif
  if (opManGenericContext_p->channelState [entity].isDisconnecting == TRUE)
  {
    psdCallsDisconnecting = vgDeactivateNextContextOwnedByEntity(entity);

    if (psdCallsDisconnecting == FALSE)
    {
      opManGenericContext_p->channelState [entity].isDisconnecting = FALSE;
      vgClearEntity (entity);
      allCallsDisconnected = TRUE;
    }
  }
  return (allCallsDisconnected);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCcDisconnectAllCalls
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:   TRUE, if any disconnect procedures has been initiated.
 *
 * Description: Checks whether there are any active calls and initiates
 *    disconnect procedure for any such call.
 *
 *-------------------------------------------------------------------------*/

Boolean vgCcDisconnectAllCalls (const VgmuxChannelNumber entity)
{
  Boolean               callsStillActive         = FALSE;

  /* check for PSD sessions on this channel */
  callsStillActive = vgDeactivateNextContextOwnedByEntity(entity);

  return (callsStillActive);
}

/*--------------------------------------------------------------------------
 *
 * Function:    establishDataSession
 *
 * Parameters:  entity          - mux channel number
 *              connectionClass - type of data call disconnected
 *
 * Returns:     nothing
 *
 * Description: Assigns data channel when a data call is
 *              connected
 *
 *-------------------------------------------------------------------------*/

void establishDataSession (const VgmuxChannelNumber entity,
                            const ConnectionClass_t connectionClass)
{

  GprsContext_t           *gprsContext_p           = ptrToGprsContext(entity);
  CallContext_t           *callContext_p           = ptrToCallContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
  FatalCheck(callContext_p != PNULL, entity, 0, 0);
#endif
  switch (connectionClass)
  {
#if defined (FEA_PPP)    
    case PPP_CONNECTION:
    {
      /* assuming there is only one PS call at each moment, so no need to check the entity */
      if (gprsContext_p->connectionActive == TRUE)
      {
        vgSigCiMuxAtoCommandReq (entity);
      }
      else
      {

        gprsContext_p->pendingOpenDataConnCnf = TRUE;

        vgCiMuxOpenDataConnection (entity, PSD_PPP);
      }

      break;
    }
#endif /* FEA_PPP */
    case PT_CONNECTION:
    {
      /* assuming there is only one PS call at each moment, so no need to check the entity */
      if (gprsContext_p->connectionActive == TRUE)
      {
        vgSigCiMuxAtoCommandReq (entity);
      }
      else
      {
        gprsContext_p->pendingOpenDataConnCnf = TRUE;
        vgCiMuxOpenDataConnection (entity, PSD_PACKET_TRANSPORT);
      }
      break;
    }
    default:
    {
      break;
    }
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:    terminateDataSession
 *
 * Parameters:  entity          - mux channel number
 *              connectionClass - type of data call disconnected
 *
 * Returns:     nothing
 *
 * Description: resets DCD and deassigns data channel when a data call is
 *              disconnected
 *
 *-------------------------------------------------------------------------*/

void terminateDataSession (const VgmuxChannelNumber entity,
                            const ConnectionClass_t connectionClass)
{
  switch (connectionClass)
  {
#if defined (FEA_PPP)    
    case PPP_CONNECTION:
#endif /* FEA_PPP */      
    case PT_CONNECTION:
    {
      vgCiMuxCloseDataConnection(entity);
      break;
    }
    default:
    {
      break;
    }
  }
}

/* END OF FILE */


