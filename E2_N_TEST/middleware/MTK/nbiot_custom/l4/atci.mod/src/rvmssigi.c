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
 * SMS input signal handlers
 **************************************************************************/

#define MODULE_NAME "RVMSSIGI"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvmsut.h>
#include <rvmsprnt.h>
#include <rvmssigi.h>
#include <rvmssigo.h>
#include <rvnvram.h>
#include <rvomut.h>
#include <smencdec.h>
#include <ut_sm.h>
#include <rvgnsigo.h>
#if defined (DEVELOPMENT_VERSION)
#  include <stdio.h>
#endif
#include <rvmspars.h>
#include <rvomtime.h>
#include <rvchman.h>
#include <rvcrhand.h>
#include <rvcimxut.h>
#include <rvmscmds.h>
#include <rvgnut.h>
#include <rvcfg.h>
#include <gkimem.h>
#include <rvstkcc.h>
/***************************************************************************
 * Macros
 ***************************************************************************/


/* See comments below for more information on this define:
** #define VG_SMS_ENABLE_RESENDING
*/


/***************************************************************************
 * Local Functions
 **
*************************************************************************/

 /*--------------------------------------------------------------------------
 *
 * Function:        isLastPartOfConcatSms
 *
 * Parameters:      [in] sms_p - the SMS to display
 *
 * Returns:         Boolean
 *
 * Description:     Returns TRUE if this is the last part of a concatenated
 *                  SMS.
 *
 *-------------------------------------------------------------------------*/
static Boolean isLastPartOfConcatSms(const VgDisplaySmsParam* sms_p)
{
    Boolean isLastPart = FALSE;
    FatalAssert (TRUE == sms_p->concatSmsPresent);

    if (sms_p->concatSmsNextRec == sms_p->recordNumber)
    {
        isLastPart = TRUE;
    }
    return isLastPart;
}


 /*--------------------------------------------------------------------------
 *
 * Function:        isFirstPartOfConcatSms
 *
 * Parameters:      [in] sms_p - the SMS to display
 *
 * Returns:         Boolean
 *
 * Description:     Returns TRUE if this is the first part of a
 *                  concatenated SMS.
 *
 * The Apex interface documentation says: "The Previous concatenated SMS
 * record number (if present) is equal to current record if this read
 * contains the first record."
 *
 *-------------------------------------------------------------------------*/
static Boolean isFirstPartOfConcatSms (const VgDisplaySmsParam* sms_p)
{
    Boolean isFirstPart = FALSE;
    FatalAssert (TRUE == sms_p->concatSmsPresent);

    if ( sms_p->concatSmsPrevRec == sms_p->recordNumber)
    {
        isFirstPart = TRUE;
    }
    return isFirstPart;
}


 /*--------------------------------------------------------------------------
 *
 * Function:        isFirstMessageInList
 *
 * Parameters:      entity - SMS entity number
 *
 * Returns:         Boolean TRUE if this is the first SMS record
 *                  to be displayed.
 *
 * Description:     returns TRUE if this is the first SMS record to be
 *                  displayed by the CMGL command or by the CMGR command
 *                  in which case it should have a CR/LF output before
 *                  it to get onto the next line after entering the AT
 *                  command.
 *
 *-------------------------------------------------------------------------*/
static Boolean isFirstMessageInList (const VgmuxChannelNumber entity,
                                     const VgDisplaySmsParam* sms_p)
{
    SmsSimAccessType readState;
    SmsContext_t     *smsContext_p = ptrToSmsContext     (entity);
    Boolean          isFirstRecord = FALSE;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    readState = smsContext_p->readState;

    FatalAssert((getCommandId(entity) == VG_AT_SMS_CMGL) ||
                (getCommandId(entity) == VG_AT_SMS_CMGR) ||
                (getCommandId(entity) == VG_AT_SMS_MCMGR));

    if( (SM_FIRST_UNREAD              == readState) ||
        (SM_FIRST_READ                == readState) ||
        (SM_FIRST_ORIGINATED_NOT_SENT == readState) ||
        (SM_FIRST_ORIGINATED_SENT     == readState) ||
        (SM_FIRST_ANY                 == readState) ||
        (SM_RW_ABSOLUTE               == readState) ||
        (SM_PREVIEW_ONLY              == readState) )
    {
        /* However, if we're in PDU mode then we display each part of a
        concatenated SMS seperately, so we do not want to flag any
        but the first part as the first message in the list. */
        if( (TRUE == sms_p->concatSmsPresent) &&
            (FALSE == vgSmsUtilIsInTextMode(entity)) &&
            (FALSE == isFirstPartOfConcatSms(sms_p)) )
        {
            isFirstRecord = FALSE;
        }
        else
        {
            isFirstRecord = TRUE;
        }
    }
    return (isFirstRecord);
}


 /*--------------------------------------------------------------------------
 *
 * Function:        isFirstSmsrInList
 *
 * Parameters:      entity - SMS SR entity number
 *
 * Returns:         Boolean TRUE if this is the first SMS SR record
 *                  to be displayed.
 *
 * Description:     returns TRUE if this is the first SMS SR record to be
 *                  displayed by the CMGL command or by the CMGR command
 *                  in which case it should have a CR/LF output before
 *                  it to get onto the next line after entering the AT
 *                  command.
 *
 *-------------------------------------------------------------------------*/
static Boolean isFirstSmsrInList( const VgmuxChannelNumber  entity,
                                  const VgDisplaySmsSrParam *display_p )
{
  SmsSimAccessType  readState;
  SmsContext_t     *smsContext_p  = ptrToSmsContext(entity);
  Boolean           isFirstRecord = FALSE;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  readState = smsContext_p->readState;

  FatalAssert((getCommandId(entity) == VG_AT_SMS_CMGL) ||
              (getCommandId(entity) == VG_AT_SMS_CMGR) ||
              (getCommandId(entity) == VG_AT_SMS_MCMGR));

  if(    (SM_FIRST_READ  == readState)
      || (SM_FIRST_ANY   == readState)
      || (SM_RW_ABSOLUTE == readState)
      || (SM_PREVIEW_ONLY == readState))
  {
      isFirstRecord = TRUE;
  }

  return (isFirstRecord);
}


 /*--------------------------------------------------------------------------
 *
 * Function:       getShortMessageTpdu
 *
 * Parameters:     [in] sms_p - the SMS to display
 *                 [out] message_p - pointer to sms message data
 *
 * Returns:        The length of the SMS record.
 *
 * Description:    Returns pointer to sms message data and length and
 *                 strips off the UDH if it is present and in
 *                 in text mode.
 *
 *-------------------------------------------------------------------------*/

static Int8 getShortMessageTpdu (const VgmuxChannelNumber entity,
                                 const VgDisplaySmsParam* sms_p,
                                 const Int8** message_p)
{
    Int16   udhLength;
    Int16   smsMessageLength;
    const Int8    *smsMessage_p;
    Boolean userDataHeaderPresent;

    if (sms_p->recordStatus != SIM_SMREC_FREE)
    {
        switch (sms_p->tpduType)
        {
            case SIM_SMTPDU_DELIVER:
            {
                userDataHeaderPresent =  sms_p->shortMessageTpdu.deliver.userDataHeaderPresent;
                smsMessageLength      =  sms_p->shortMessageTpdu.deliver.shortMsgLen;
                smsMessage_p          = &sms_p->shortMessageTpdu.deliver.shortMsgData[0];
                break;
            }
            case SIM_SMTPDU_SUBMIT:
            {
                userDataHeaderPresent =  sms_p->shortMessageTpdu.submit.userDataHeaderPresent;
                smsMessageLength      =  sms_p->shortMessageTpdu.submit.shortMsgLen;
                smsMessage_p          = &sms_p->shortMessageTpdu.submit.shortMsgData[0];
                break;
            }
            case SIM_SMTPDU_COMMAND: /* fall through... */
            default:
            {
                userDataHeaderPresent = FALSE;
                smsMessage_p          = PNULL;
                smsMessageLength      = 0;
                break;
            }
        }

        /* If userDataHeaderPresent == TRUE, then check the length for illegal coding.
        * If illegal, then mark header as not present....
        */
        if( (userDataHeaderPresent == TRUE) &&
            (smsMessage_p != PNULL) &&
            (smsMessage_p[0] > smsMessageLength))
        {
            userDataHeaderPresent = FALSE;
        }

        /* If in text mode and the UserDataHeader is present
        then adjust pointer and length to strip it off. */
        if( (cfRvSmHandleConcatSms == TRUE) &&
            (smsMessage_p != PNULL) &&
            (0 != smsMessageLength) &&
            (vgSmsUtilIsInTextMode(entity)) &&
            (userDataHeaderPresent) )
        {
            udhLength = smsMessageLength - utsmGetUserMessageLen((Int8*)smsMessage_p, (Int8) smsMessageLength);
            smsMessage_p     += udhLength;
            smsMessageLength -= udhLength;
        }
    }
    else
    {
        smsMessage_p          = PNULL;
        smsMessageLength      = 0;
    }

    *message_p = smsMessage_p;
    return (Int8) smsMessageLength;
}

 /*--------------------------------------------------------------------------
 *
 * Function:       concatShortMessageTpduOntoSmsMessage
 *
 * Parameters:     [in] entity - sms context entity number
 *                 [in] sms_p - the SMS to display
 *
 * Returns:        nothing
 *
 * Description:    Concatenate the supplied message data onto the sms
 *                 message (only if there is enough room) if we're in
 *                 text mode.
 *                 If in PDU then copy the message data onto the sms
 *                 message.
 *
 *                 Also deals with stripping off the UDH if in text mode
 *                 - see the getShortMessageTpdu() function for details.
 *
 *-------------------------------------------------------------------------*/

static Boolean concatShortMessageTpduOntoSmsMessage( const VgmuxChannelNumber entity,
                                                     const VgDisplaySmsParam* sms_p)
{
    SmsContext_t *smsContext_p = ptrToSmsContext (entity);
    const Int8   *smsRecordData_p = PNULL;
    Int8         smsRecordLength;
    Boolean      ok = FALSE;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    smsRecordLength = getShortMessageTpdu (entity, sms_p, &smsRecordData_p);

    if( (cfRvSmHandleConcatSms == TRUE) &&
        (TRUE == sms_p->concatSmsPresent) )
    {
        if( (FALSE == vgSmsUtilIsInTextMode(entity)) ||
            (isFirstPartOfConcatSms(sms_p)) )
        {
            smsContext_p->smsLength = 0;
        }
    }
    else
    {
        /* set length to zero at start so we don't concatenate
        but just copy sms tpdu on sms message */
        smsContext_p->smsLength = 0;
    }

    if ( (smsContext_p->smsLength + smsRecordLength) <= VG_ARRAY_LENGTH(smsContext_p->smsMessage) )
    {
        ok = TRUE; /* not too big */
        if ( 0 != smsRecordLength )
        {
            FatalAssert (smsContext_p->smsLength < VG_ARRAY_LENGTH(smsContext_p->smsMessage) );

            memcpy (&smsContext_p->smsMessage[smsContext_p->smsLength], smsRecordData_p, smsRecordLength);
            smsContext_p->smsLength += (Int32)smsRecordLength;
        }
    }
    return (ok);
}




 /*--------------------------------------------------------------------------
 *
 * Function:        outputSms
 *
 * Parameters:      entity - SMS entity number
 *                  [in] sms_p - the SMS to display
 *                  alphaId - optional alpha id string
 *
 * Returns:         Nothing
 *
 * Description:     Ouputs SMS message to user according to
 *                  whether in text mode or PDU mode and whether
 *                  SMS is a Submit (sent) or Deliver (stored)
 *
 *-------------------------------------------------------------------------*/
static void outputSms (const VgmuxChannelNumber entity,
                       const VgDisplaySmsParam *sms_p, const VgSmsAlphaId *alphaId)
{
    /* Output the SMS message in text or PDU mode */
    switch (sms_p->tpduType)
    {
        case SIM_SMTPDU_DELIVER:
            {
                if ( vgSmsUtilIsInTextMode(entity) )
                {
                    vgSmsPrintSmsInTextMode (entity, VG_SIM_SMTPDU_DELIVER, sms_p, alphaId);
                }
                else /* PDU mode */
                {
                    vgSmsPrintSmsInPduMode (entity, VG_SIM_SMTPDU_DELIVER, sms_p, alphaId);
                }
                break;
            }

        case SIM_SMTPDU_SUBMIT:
            {
                if ( vgSmsUtilIsInTextMode(entity) )
                {
                    vgSmsPrintSmsInTextMode (entity, VG_SIM_SMTPDU_SUBMIT, sms_p, alphaId);
                }
                else /* PDU mode */
                {
                    vgSmsPrintSmsInPduMode (entity, VG_SIM_SMTPDU_SUBMIT, sms_p, alphaId);
                }
                break;
            }

        case SIM_SMTPDU_STATUS_REPORT:
            {
                /* do nothing right now */
                vgSmsPrintNewline(entity);
                break;
            }
        default:
            {
                FatalParam (sms_p->tpduType, 0, 0);
                break;
            }
    }
}



 /*--------------------------------------------------------------------------
 *
 * Function:        displaySms
 *
 * Parameters:      entity - SMS entity number
 *                  [in] sms_p - the SMS to display
 *
 * Returns:         Nothing
 *
 * Description:     Ouputs SMS message to user along with prefix
 *                  of "CMGR" or "CMGL" depending on command.
 *
 *-------------------------------------------------------------------------*/

static void displaySms (const VgmuxChannelNumber entity,
                        const VgDisplaySmsParam *sms_p,
                        const VgSmsAlphaId *alphaId)
{

    Int16        recNum;

    SmsContext_t *smsContext_p       = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    FatalAssert( sms_p->recordStatus != SIM_SMREC_FREE);

    FatalAssert((getCommandId (entity) == VG_AT_SMS_CMGL) ||
                (getCommandId (entity) == VG_AT_SMS_CMGR) ||
                (getCommandId (entity) == VG_AT_SMS_MCMGR));

    /* If this is the first sms then do new line first - this is used by the
    ** CMGL command that lists all the SMS's out, subsequent SMS will not
    ** have the newline.
    */
    if (isFirstMessageInList(entity, sms_p))
    {
        vgSmsPrintNewline(entity);
    }

    if ( getCommandId (entity) == VG_AT_SMS_CMGR )
    {
        FatalAssert (smsContext_p->readState == SM_RW_ABSOLUTE );
        vgSmsPuts(entity, (const Char*)"+CMGR: ");
    }
    else if(getCommandId (entity) == VG_AT_SMS_MCMGR)
    {
        FatalAssert (smsContext_p->readState == SM_PREVIEW_ONLY );
        vgSmsPuts(entity, (const Char*)"*MCMGR: ");
    }
    else/* VG_AT_SMS_CMGL == getCommandId (entity) */
    {
        FatalAssert (VG_AT_SMS_CMGL == getCommandId (entity) );
        FatalAssert (smsContext_p->readState != SM_RW_ABSOLUTE );

        /* Display record number of the SMS e.g. +CMGL: 1, */
        vgSmsPuts(entity, (const Char*)"+CMGL: ");

        if( (TRUE == sms_p->concatSmsPresent) &&
            (vgSmsUtilIsInTextMode(entity) ) )
        {
            recNum = (Int16)sms_p->concatSmsFirstRec;
        }
        else
        {
            recNum = (Int16)sms_p->recordNumber;
        }

        vgSmsPrintInt16(entity, recNum);
        vgSmsPutc(entity, ',');
    }

    /* Output the SMS message in text or PDU mode */
    outputSms(entity, sms_p, alphaId);
    smsContext_p->smsLength = 0;
    vgSmsFlush(entity);
}


 /*--------------------------------------------------------------------------
 *
 * Function:        outputSmsr
 *
 * Parameters:      entity - SMS SR entity number
 *                  signal - pointer to ApexSmReadCnf structure
 *                  alphaId - optional alpha id string
 *
 * Returns:         Nothing
 *
 * Description:     Ouputs SMS message to user according to
 *                  whether in text mode or PDU mode and whether
 *                  SMS is a Submit (sent) or Deliver (stored)
 *
 *-------------------------------------------------------------------------*/
static void outputSmsr( const VgmuxChannelNumber  entity,
                        const VgDisplaySmsSrParam *disply_p,
                        const VgSmsAlphaId        *alphaId )
{
  /* Output the SMS SR message in text or PDU mode */
  if( vgSmsUtilIsInTextMode(entity) )
  {
      vgSmsPrintSmsrInTextMode(entity, disply_p);
  }
  else /* PDU mode */
  {
      vgSmsPrintSmsrInPduMode(entity, disply_p);
  }
}

 /*--------------------------------------------------------------------------
 *
 * Function:        displaySmsr
 *
 * Parameters:      entity - SMS entity number
 *                  signal - pointer to ApexSmReadCnf structure
 *
 * Returns:         Nothing
 *
 * Description:     Ouputs SMS Status Report to user along with prefix
 *                  of "CMGR" or "CMGL" depending on command.
 *
 *-------------------------------------------------------------------------*/

static void displaySmsr( const VgmuxChannelNumber  entity,
                         const VgDisplaySmsSrParam *display_p,
                         const VgSmsAlphaId        *alphaId)
{
  Int16         recNum;
  SmsContext_t *smsContext_p = ptrToSmsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  FatalAssert(display_p->recordNumber != SIM_SMREC_FREE);

  FatalAssert((getCommandId(entity) == VG_AT_SMS_CMGL) ||
              (getCommandId(entity) == VG_AT_SMS_CMGR) ||
              (getCommandId(entity) == VG_AT_SMS_MCMGR));

  /* If this is the first Status Report then do new line first - this is used by the
  ** CMGL command that lists all the SMS-SR's out, subsequent SMS SR will not
  ** have the newline.
  */
  if( isFirstSmsrInList(entity, display_p) )
  {
      vgSmsPrintNewline(entity);
  }

  if( getCommandId (entity) == VG_AT_SMS_CMGR )
  {
      FatalAssert (smsContext_p->readState == SM_RW_ABSOLUTE );
      vgSmsPuts(entity, (const Char*)"+CMGR: ");
  }
  else if ( getCommandId (entity) == VG_AT_SMS_MCMGR )
  {
      FatalAssert (smsContext_p->readState == SM_PREVIEW_ONLY );
      vgSmsPuts(entity, (const Char*)"*MCMGR: ");
  }
  else /* VG_AT_SMS_CMGL == getCommandId (entity) */
  {
      FatalAssert (VG_AT_SMS_CMGL == getCommandId (entity) );
      FatalAssert (smsContext_p->readState != SM_RW_ABSOLUTE );

      /* Display record number of the SMS e.g. +CMGL: 1, */
      vgSmsPuts(entity, (const Char*)"+CMGL: ");

      recNum = (Int16)display_p->recordNumber;

      vgSmsPrintInt16(entity, recNum);
      vgSmsPutc(entity, ',');
  }

  /* Output the SMS SR message in text or PDU mode */
  outputSmsr(entity, display_p, alphaId);
  smsContext_p->smsLength = 0;
  vgSmsFlush(entity);
}

 /*--------------------------------------------------------------------------
 *
 * Function:        requestNextPartOfConcat
 *
 * Parameters:      entity - SMS entity number
 *                  [in] sms_p - the SMS to display
 *
 * Returns:         Nothing
 *
 * Description:     Request a read of the next part of the concatenated
 *                  SMS message.
 *
 *-------------------------------------------------------------------------*/
static void requestNextPartOfConcat (const VgmuxChannelNumber entity, const VgDisplaySmsParam* sms_p)
{
    SmsContext_t *smsContext_p       = ptrToSmsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    FatalAssert( TRUE == sms_p->concatSmsPresent);
    FatalAssert( FALSE == isLastPartOfConcatSms(sms_p));
    FatalAssert( smsContext_p->cpmsReadDelete == VG_CPMS_STORAGE_SM);

    if(getCommandId (entity) == VG_AT_SMS_MCMGR)
    {
      setResultCode(  entity,
                      vgSmsSigOutApexReadSmsNoReadState(  entity,
                                                          (Int8)(sms_p->concatSmsNextRec),
                                                          SM_PREVIEW_ONLY));
    }
    else
    {
      setResultCode(  entity,
                      vgSmsSigOutApexReadSmsNoReadState(  entity,
                                                          (Int8)(sms_p->concatSmsNextRec),
                                                          SM_RW_ABSOLUTE));
    }
}


 /*--------------------------------------------------------------------------
 *
 * Function:        requestNextMessage
 *
 * Parameters:      entity - SMS entity number
 *                  [in] sms_p - the SMS to display
 *
 * Returns:         Nothing
 *
 * Description:     Requests further reads of messages from the BL.
 *                  1. If the message is concatenated then request next part
 *                     of it.
 *                  2. or, If executing the CMGL command read the next
 *                     message in the list from the SIM.
 *                  3. or, if CMGR then just finish.
 *
 *-------------------------------------------------------------------------*/
static void requestNextMessage (const VgmuxChannelNumber entity, const VgDisplaySmsParam* sms_p)
{
    SmsContext_t       *smsContext_p    = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    /* If
    1. We need to handle concat SMS,
    2. we're executing the CMGL command,
    3. this is a concatenated SMS and
    4. we haven't got the last part of it yet, then read the next part of it. */
    if( (cfRvSmHandleConcatSms == TRUE) &&
        (VG_AT_SMS_CMGL == getCommandId (entity)) &&
        (TRUE == sms_p->concatSmsPresent) &&
        (FALSE == isLastPartOfConcatSms(sms_p)) )
    {
        requestNextPartOfConcat(entity, sms_p);
    }
    else
    {
        /*
        ** If we're running the CMGL command we need to request
        ** the next SMS in the list from the SIM.
        **
        ** If we're running CMGR (read SMS) or CMSS (send from SIM) then we just
        ** need to finish the command by setting the result code to OK.
        */
        if (VG_AT_SMS_CMGL == getCommandId (entity))
        {
            FatalAssert (SM_RW_ABSOLUTE != smsContext_p->readState);
            FatalAssert (VG_SMS_INVALID_RECORD_NUMBER != smsContext_p->cmglRecordNumberToSearchFrom);

            if( (SM_FIRST_UNREAD              == smsContext_p->readState) ||
                (SM_FIRST_READ                == smsContext_p->readState) ||
                (SM_FIRST_ORIGINATED_NOT_SENT == smsContext_p->readState) ||
                (SM_FIRST_ORIGINATED_SENT     == smsContext_p->readState) ||
                (SM_FIRST_ANY                 == smsContext_p->readState) )
            {
                /* move from FIRST_XXXX to NEXT_XXXX i.e. read the next SMS of the
                specified type. */
                smsContext_p->readState = (SmsSimAccessType)(smsContext_p->readState + 1);
            }

            switch( smsContext_p->cpmsReadDelete)
            {
                case VG_CPMS_STORAGE_SM:
                    {
                        /* send off a read request for the next SMS in the list */
                        setResultCode(
                            entity,
                            vgSmsSigOutApexReadSmsNoReadState(
                                entity,
                                (Int8)smsContext_p->cmglRecordNumberToSearchFrom /*index*/,
                                smsContext_p->readState) );

                    }
                    break;

                default:
                    {
                        /* Invalid enum value*/
                        FatalParam( smsContext_p->cpmsReadDelete, 0, 0);
                    }
                    break;
            }
            /* We've used the value, not going to use it again, so reset to check value. */
            smsContext_p->cmglRecordNumberToSearchFrom = VG_SMS_INVALID_RECORD_NUMBER;
        }
        else
        {
            if(getCommandId (entity) ==VG_AT_SMS_CMGR)
            {
              FatalAssert ( smsContext_p->readState ==  SM_RW_ABSOLUTE);
            }
            else
            {
              FatalAssert (getCommandId (entity) ==VG_AT_SMS_MCMGR);
              FatalAssert (smsContext_p->readState == SM_PREVIEW_ONLY);
              smsContext_p->readState = SM_RW_ABSOLUTE;
            }

            setResultCode (entity, RESULT_CODE_OK);
        }
    }
}

 /*--------------------------------------------------------------------------
 *
 * Function:        displaySmsAndRequestNextOne
 *
 * Parameters:      entity - SMS entity number
 *                  [in] sms_p - the SMS to display
 *
 * Returns:         Nothing
 *
 * Description:     Displays the current SMS and requests the next one
 *                  in the list if this is CMGL command.
 *
 *                  If we are supporting AlphaId's then instead of just
 *                  displaying the sms etc we need to send off a signal
 *                  to retrieve the phonebook entry, this means that
 *                  we have to save a copy of the current signal (with
 *                  the sms data in it) because it will get freed.
 *                  Then in the phonebook Cnf signal we carry on as
 *                  normal.
 *
 *-------------------------------------------------------------------------*/

static void displaySmsAndRequestNextOne( const VgmuxChannelNumber entity, const VgDisplaySmsParam *sms_p )
{
    SmsContext_t     *smsContext_p  = ptrToSmsContext(entity);


#if defined (FEA_PHONEBOOK)      
    DialledBcdNum     dialledNum;
    const SmsAddress *smsAddress    = vgSmsGetShortMessageSmeAddress( sms_p);
    ResultCode_t      result        = RESULT_CODE_ERROR;
#endif /* FEA_PHONEBOOK */   

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);

    /* If
    1. We need to handle concat SMS,
    2. it's a concatenated SMS,
    3. we're in text mode,
    4. and we haven't read all of it then
    ask for the next part of it, and do not display it yet. */
    if( (cfRvSmHandleConcatSms == TRUE) &&
        (TRUE == sms_p->concatSmsPresent) &&
        (TRUE == vgSmsUtilIsInTextMode(entity)) &&
        (FALSE == isLastPartOfConcatSms( sms_p)) )
    {
        requestNextPartOfConcat(entity, sms_p);
    }
    else
    {

#if defined (FEA_PHONEBOOK)      
        /* Do AlphaId lookup if enabled, if this message isn't blank, and if
        ** the telephone 'number' isn't already alphanumeric.
        */
        if( (vgSmsUtilIsAlphaIdLookupEnabled()) &&
            (SIM_SMREC_FREE != sms_p->recordStatus) &&
            (NUM_ALPHANUMERIC != smsAddress->typeOfNumber) )
        {
            dialledNum = vgSmsUtilConvertSmsAddressToDialledBcdNum( smsAddress);

            /* Copy the ApexSmReadCnf so it's preserved for use in the
            ** phonebook cnf handler.
            */
            smsContext_p->savedSmDisplayParam = *sms_p;

            /* do the lookup in the phonebook */
            result = vgSmsSigOutApexLmGetAlphaReq (entity, VG_SMS_LMGETALPHA_COMMAND_CALL_ID, &dialledNum);
            if (RESULT_CODE_PROCEEDING != result)
            {
                /* failed to send the phonebook lookup so carry on without it... */
                displaySms         (entity, sms_p, PNULL/*alphaId*/);
                requestNextMessage (entity, sms_p);
            }
        }
        else
#endif /* FEA_PHONEBOOK */          
        {
            /* If the "number" is already alphanumeric, AlphaId lookup is disabled,
            ** or the message it blank, we don't want to do an AlphaId lookup.
            ** If the message is blank then we don't want to display it.
            */
            if (SIM_SMREC_FREE != sms_p->recordStatus)
            {
                displaySms( entity, sms_p, PNULL/*alphaId*/);
            }
            requestNextMessage( entity, sms_p);
        }
    }
}

 /*--------------------------------------------------------------------------
 *
 * Function:        requestNextSmsrMessage
 *
 * Parameters:      entity - SMS entity number
 *                  signal - pointer to ApexSmReadCnf structure
 *
 * Returns:         Nothing
 *
 * Description:     Requests further reads of messages from the BL.
 *                  1. If executing the CMGL command read the next
 *                     message in the list from the SIM.
 *                  2. or, if CMGR then just finish.
 *
 *-------------------------------------------------------------------------*/
static void requestNextSmsrMessage( const VgmuxChannelNumber entity, const VgDisplaySmsSrParam *display_p )
{
    SmsContext_t       *smsContext_p    = ptrToSmsContext(entity);
    ResultCode_t        resultCode;

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);

    /*
    ** If we're running the CMGL command we need to request
    ** the next SMS in the list from the SIM.
    **
    ** If we're running CMGR (read SMS) or CMSS (send from SIM) then we just
    ** need to finish the command by setting the result code to OK.
    */
    if( VG_AT_SMS_CMGL == getCommandId(entity) )
    {
        FatalAssert(SM_RW_ABSOLUTE != smsContext_p->readState);
        FatalAssert(VG_SMS_INVALID_RECORD_NUMBER != smsContext_p->cmglRecordNumberToSearchFrom);

        if(    (SM_FIRST_READ == smsContext_p->readState)
            || (SM_FIRST_ANY  == smsContext_p->readState) )
        {
            /* move from FIRST_XXXX to NEXT_XXXX i.e. read the next SMS SR of the
               specified type. */
            smsContext_p->readState = (SmsSimAccessType)(smsContext_p->readState + 1);
        }

        if( display_p->processingMe == TRUE)
        {
        
            /* TODO: Remove above if statement.  We do not support this - shouldn't get here */
            resultCode = VG_CMS_ERROR_OP_NOT_SUPPORTED;
        }
        else
        {
            /* send off a read request for the next SMS in the list */
            resultCode = vgSmsSigOutApexReadSmsrNoReadState(
                            entity,
                            (Int8)smsContext_p->cmglRecordNumberToSearchFrom /*index*/,
                            smsContext_p->readState);
        }

        setResultCode(entity, resultCode);

        /* We've used the value, not going to use it again, so reset to check value. */
        smsContext_p->cmglRecordNumberToSearchFrom = VG_SMS_INVALID_RECORD_NUMBER;
    }
    else if( getCommandId (entity) == VG_AT_SMS_CMGR )
    {
        FatalAssert(getCommandId (entity) == VG_AT_SMS_CMGR);
        FatalAssert(smsContext_p->readState == SM_RW_ABSOLUTE);

        setResultCode(entity, RESULT_CODE_OK);
    }
    else
    {
        FatalAssert(getCommandId (entity) == VG_AT_SMS_MCMGR );
        FatalAssert(smsContext_p->readState == SM_PREVIEW_ONLY);

        smsContext_p->readState = SM_RW_ABSOLUTE;
        setResultCode(entity, RESULT_CODE_OK);
    }
}

 /*--------------------------------------------------------------------------
 *
 * Function:        displaySmsrAndRequestNextOne
 *
 * Parameters:      entity - SMS entity number
 *                  signal - pointer to ApexSmReadCnf structure
 *
 * Returns:         Nothing
 *
 * Description:     Displays the current SMS and requests the next one
 *                  in the list if this is CMGL command.
 *
 *                  If we are supporting AlphaId's then instead of just
 *                  displaying the sms etc we need to send off a signal
 *                  to retrieve the phonebook entry, this means that
 *                  we have to save a copy of the current signal (with
 *                  the sms data in it) because it will get freed.
 *                  Then in the phonebook Cnf signal we carry on as
 *                  normal.
 *
 *-------------------------------------------------------------------------*/

static void displaySmsrAndRequestNextOne( const VgmuxChannelNumber entity, const VgDisplaySmsSrParam* display_p)
{
  /* If the "number" is already alphanumeric, AlphaId lookup is disabled,
  ** or the message it blank, we don't want to do an AlphaId lookup.
  ** If the message is blank then we don't want to display it.
  */
  if( SIM_SMREC_FREE != display_p->recordNumber )
  {
      displaySmsr(entity, display_p, PNULL);
  }

  requestNextSmsrMessage( entity, display_p );
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

#if defined (FEA_PHONEBOOK)
/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigInApexLmGetAlphaCnf
 *
 * Parameters:  entity   - entity number
 *              signal_p - ApexLmGetAlphaCnf
 *
 * Returns:     Nothing
 *
 * Description: Handles the vgSmsSigInApexLmGetAlphaCnf signal received
 *              as a result of sending off a phonebook lookup request.
 *
 *-------------------------------------------------------------------------*/

void vgSmsSigInApexLmGetAlphaCnf (const VgmuxChannelNumber entity, const ApexLmGetAlphaCnf *signal_p)
{
    SmsContext_t       *smsContext_p = ptrToSmsContext (entity);
    SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext ();
    const VgSmsAlphaId *alphaId_p;
    VgmuxChannelNumber profileEntity = 0;

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);

    FatalAssert(  (VG_SMS_LMGETALPHA_COMMAND_CALL_ID     == signal_p->callId) ||
                (VG_SMS_LMGETALPHA_UNSOLICITED_CALL_ID == signal_p->callId) );

    if( (LM_REQ_OK == signal_p->requestStatus) &&
        (TRUE == signal_p->alphaStringValid) )
    {
        alphaId_p = &signal_p->alphaId;
    }
    else
    {
        alphaId_p = PNULL;
    }

    if (VG_SMS_LMGETALPHA_COMMAND_CALL_ID == signal_p->callId)
    {
        /* CMGR, CMGL etc phonebook lookup */
        if(SIM_SMREC_FREE != smsContext_p->savedSmDisplayParam.recordStatus)
        {
            displaySms         (entity, &(smsContext_p->savedSmDisplayParam), alphaId_p);
        }
        requestNextMessage (entity, &(smsContext_p->savedSmDisplayParam));
    }
    else
    {
        for(    profileEntity =0;
                profileEntity < CI_MAX_ENTITIES;
                profileEntity++)
        {
            /* SM Delivery Ind - unsolicited phonebook lookup */
            if( (isEntityActive (profileEntity)) && (smsCommonContext_p->printCmtOnlyOnMmi == FALSE))
            {
                vgSmsPrintUnsolicitedSmDeliveryIndWithAlphaId (profileEntity, &(smsCommonContext_p->savedUnsolSmDeliveryInd), alphaId_p);
            }
        }

        smsCommonContext_p->printCmtOnlyOnMmi = FALSE;
        /* Now we need to explicitely release control of the
        unsolicited channel. */
        vgSmsUtilSetUnsolicitedEventHandlerFree ();
        vgChManReleaseControl (entity);
    }
}
#endif /* FEA_PHONEBOOK */

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigInApexSmReadCnf
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmReadCnf contining the information
 *              for the SIG_APEX_SM_READ_CNF signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_READ_CNF signal received
 *              from the BL task's shell process.
 *
 * Notes:       These is some pseudo 'C' code that explains how this
 *              functions behaves for the various AT commands:
 *
 *    switch ( currentAtCommand )
 *    {
 *      case VG_AT_SMS_CMGL:
 *        if a concat sms then
 *        {
 *          if not the last part then
 *             get the next part
 *          else if is the last part
 *            display it & request the next sms in the list
 *        }
 *        else if not a concat sms
 *        {
 *          display it & request the next sms in the list
 *        }
 *      break;
 *
 *      case VG_AT_SMS_CMGR:
 *        if a concat SMS then
 *        {
 *          if not the last part then
 *            get the next part
 *          else if is the last part
 *            display it
 *        }
 *        else
 *        {
 *          display it
 *        }
 *      break;
 *   }
 *
 *-------------------------------------------------------------------------*/

void vgSmsSigInApexSmReadCnf( const VgmuxChannelNumber entity, const ApexSmReadCnf *signal_p )
{
    SmsContext_t        *smsContext_p = ptrToSmsContext (entity);
    VgDisplaySmsParam   *smsDisplay_p = PNULL;

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);

    KiAllocZeroMemory(  sizeof(VgDisplaySmsParam),
                        (void **) &smsDisplay_p);

    /* Make sure we're executing a command */
    FatalAssert(getResultCode (entity) == RESULT_CODE_PROCEEDING);
    /* Make sure the old (Phase 1) way of sending a msg from the SIM is not used */
    FatalAssert(smsContext_p->readState != SM_WRITE_FREE);
    /* Make sure only the commands we expect */
    FatalAssert((getCommandId (entity) == VG_AT_SMS_CMGL) ||
                (getCommandId (entity) == VG_AT_SMS_CMGR) ||
                (getCommandId (entity) == VG_AT_SMS_MCMGR));

    if( signal_p->requestStatus == SM_REQ_OK )
    {
        /* Create display structure*/
        smsDisplay_p->tpduType             = signal_p->type;
        smsDisplay_p->shortMessageTpdu     = signal_p->shortMessageTpdu;
        smsDisplay_p->recordStatus         = signal_p->smRecordStatus;
        smsDisplay_p->recordNumber         = signal_p->recordNumber;
        smsDisplay_p->concatSmsPresent     = signal_p->concatSmsPresent;
        smsDisplay_p->concatSmsFirstRec    = signal_p->concatSmsFirstRec;
        smsDisplay_p->concatSmsPrevRec     = signal_p->concatSmsPrevRec;
        smsDisplay_p->concatSmsNextRec     = signal_p->concatSmsNextRec;
        smsDisplay_p->scAddr               = signal_p->scAddr;

        /*
        ** For the CMGL command, for a concatenated SMS we need to start
        ** searching for the next SMS from the first part of that concat SMS,
        ** not the last part of it, otherwise we may miss some records, so
        ** we can the index of it.
        ** (The SMS BL will skips parts of concatenated SMS's for us.)
        */
        if( (FALSE == signal_p->concatSmsPresent) ||
            (   (TRUE == signal_p->concatSmsPresent) &&
                (isFirstPartOfConcatSms( smsDisplay_p))) )
        {
            smsContext_p->cmglRecordNumberToSearchFrom = smsDisplay_p->recordNumber;
        }

        if( FALSE == concatShortMessageTpduOntoSmsMessage(entity, smsDisplay_p) )
        {
            /* This should never happen because the PS will never give us
            a concat SMS that is too big. */
            /* SMS too big */
            FatalParam(entity, 0, 0);
        }

        if(smsDisplay_p->scAddr.length > SMS_MAX_ADDR_LEN)
        {
            smsDisplay_p->scAddr.length = SMS_MAX_ADDR_LEN;
        }

        if(smsDisplay_p->shortMessageTpdu.command.scAddr.length > SMS_MAX_ADDR_LEN)
        {
            smsDisplay_p->shortMessageTpdu.command.scAddr.length = SMS_MAX_ADDR_LEN;
        }

        displaySmsAndRequestNextOne(entity, smsDisplay_p);
    }
    else /* not (signal_p->requestStatus == SM_REQ_OK) */
    {
        switch(getCommandId (entity))
        {
           case VG_AT_SMS_CMGL:
           {
             if( signal_p->requestStatus == SM_REQ_RECORD_NOT_FOUND &&
                 smsContext_p->readState != SM_RW_ABSOLUTE )
             {
                 /* we've got to the end of the CMGL */
                 setResultCode (entity, RESULT_CODE_OK);
             }
             else
             {
                 setResultCode (entity, vgSmsUtilConvertRequestError (entity, signal_p->requestStatus));
             }
           }
           break;


           case VG_AT_SMS_CMGR:
           case VG_AT_SMS_MCMGR:
           {
             setResultCode (entity, vgSmsUtilConvertRequestError (entity, signal_p->requestStatus));
           }
           break;

           default:
           break;
        }

        /* reset back to defaults... */
        smsContext_p->readState = SM_RW_ABSOLUTE;
        smsContext_p->smsLength = 0;
    }

    KiFreeMemory( (void**)&smsDisplay_p);
}

/*--------------------------------------------------------------------------
 *
 * Function:    sigApexSmDeleteCnf
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmDeleteCnf contining the information
 *              for the SIG_APEX_SM_DELETE_CNF signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_DELETE_CNF signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/

void vgSmsSigInApexSmDeleteCnf (const VgmuxChannelNumber entity, const ApexSmDeleteCnf *signal_p)
{
  SmsContext_t     *smsContext_p = ptrToSmsContext (entity);
  SmsSimAccessType  simAccessType;
  ResultCode_t      result;
  GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
  FatalCheck(smsContext_p != PNULL, entity, 0, 0);

  /* Make sure we're executing a command */
  FatalAssert(getResultCode(entity) == RESULT_CODE_PROCEEDING);

  if(    (signal_p->requestStatus == SM_REQ_OK)
      && (signal_p->shortMessageDeleted) )
  {
      /* job118116 adds use of <delflag> - if delFlag was set search for following set of SMS to delete */
      if( vgSmsUtilConvertDelFlagToSimAccessType(smsContext_p->delFlag, &simAccessType) )
      {
          result = vgSmsSigOutApexReadSmsNoData (entity, simAccessType);

          if( result == RESULT_CODE_PROCEEDING )
          {
              setResultCode (entity, result);
          }
      }
      /* there is no more messages to delete */
      else
      {
          setResultCode (entity, RESULT_CODE_OK);
      }
  }
  else
  {
      setResultCode (entity, vgSmsUtilConvertRequestError(entity, signal_p->requestStatus));
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:    sigApexSmSendCnf
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmSendCnf contining the information
 *              for the SIG_APEX_SM_SEND_CNF signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_SEND_CNF signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/
void vgSmsSigInApexSmSendCnf (const VgmuxChannelNumber entity, const ApexSmSendCnf *signal_p)
{
    SmsCommonContext_t    *smsCommonContext_p = ptrToSmsCommonContext();
    SmsContext_t          *smsContext_p = ptrToSmsContext (entity);
    CiapSms                sendSmsInfo;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    /* Make sure we're executing a command */
    FatalAssert (getResultCode (entity) == RESULT_CODE_PROCEEDING);

    /*job116776 introducing moreMessagesToSend functionality*/
    if (smsCommonContext_p->moreMsgsToSendMode == 1)
    {
        vgCiStartTimer(TIMER_TYPE_SMS_CMMS, entity);
    }

    if (signal_p->requestStatus == SM_REQ_OK)
    {
        if ( smsContext_p->sendRetries > 0 )
        {
            smsContext_p->sendRetries = 0;
        }

        /* if sending a concatenated sms... */
        if(smsContext_p->concatSms.sequenceNumber > 0)
        {
            FatalAssert( cfRvSmHandleConcatSms == TRUE);
            /*
            ** If we've sent the first part of the concat SMS (this is it's confirmation)
            ** then set the firstRecord for all subsequent parts to point to the first
            ** part (this part) which is stored in the messageRef.
            */
            if(smsContext_p->concatSms.sequenceNumber == 1)
            {
                /* save the record number so we can display it when
                the whole (concatenated SMS) message has been sent. */
                smsContext_p->firstRecord = (Int8)signal_p->messageRef;
            }

            /* send the next part of the concatenated SMS */
            vgSmsUtilFormatConcatSmsStruct(entity, &sendSmsInfo);

            setResultCode (entity, vgSmsSigOutApexSendSmsNormal(entity, &sendSmsInfo));
        }
        else /* either sent the last part of concat SMS or sent a non-concat SMS */
        {
            smsContext_p->smsLength = 0;

            if (smsContext_p->concatSms.sequenceLength == 0)
            {
                /* have sent non-concat SMS, set the record number
                because we're going to display it. */
                smsContext_p->firstRecord = (Int8)signal_p->messageRef;
            }
            else
            {
                /* this is the Cnf for the last part of concatenated SMS.
                reset sequence length to indicate non-concat SMS for
                next time round. */
                smsContext_p->concatSms.sequenceLength = 0;
            }

            vgSmsPrintNewline(entity);
            switch( getCommandId (entity))
            {
                case VG_AT_SMS_CMSS:
                    {
                        vgSmsPuts(entity, (const Char*)"+CMSS: ");
                        smsContext_p->readState = SM_RW_ABSOLUTE;
                    }
                    break;

                case VG_AT_SMS_CMGS:
                    {
                        vgSmsPuts(entity, (const Char*)"+CMGS: ");
                        smsContext_p->readState = SM_RW_ABSOLUTE;
                    }
                    break;

                default:
                    {
                        FatalParam( getCommandId (entity), 0, 0);
                    }
                    break;
            }

            vgSmsPrintInt16 (entity, smsContext_p->firstRecord);
            smsContext_p->firstRecord = 0;

            vgSmsPrintNewline(entity);
            vgSmsFlush(entity);

            setResultCode (entity, RESULT_CODE_OK);
        }
    }
#if defined (VG_SMS_ENABLE_RESENDING)
    /*
      At the moment the Apex interface returns SM_REQ_PS_ERROR when
      a SmrlReportInd (and TsReportInd) comes with the following error:
      body.smrlReportInd.statusOfReport = TRANSFER_ERROR 0x01
      body.smrlReportInd.rpCause.cause  = RESPONDING_TO_PAGING 0x3107
      Which means that the PS is busy receiving an SMS i.e. part of a
      concatenated SMS, once the background layer has been changed to
      return a more specific error i.e. SM_REQ_PS_BUSY_ERROR we can
      act on it and try resending until successful.
    */
    else if(    (signal_p->requestStatus == SM_REQ_PS_ERROR) &&
                (signal_p->rpCause.cause == NORMAL_RELEASE) &&
                (smsContext_p->sendRetries < VG_SMS_MAX_SEND_RETRIES) )
    {
        /* If failed to send SMS, or part of concat SMS, because the PS was busy
        ** (e.g. receiving an SMS) then resend it, up to maximum number of retries
        */

        /* If this is part of concat SMS then roll back the sequenceNumber
        ** so that we resend the same part again: because FormatConcatSmsStruct
        ** increments the sequence number.
        */
        if (0 != smsContext_p->concatSms.sequenceLength)
        {
            if (smsContext_p->concatSms.sequenceNumber > 0)
            {
                smsContext_p->concatSms.sequenceNumber--;
            }
            else
            {
                vgSmsUtilInitConcatSmsStruct (entity);
            }
        }

        smsContext_p->sendRetries++;
        vgSmsUtilFormatConcatSmsStruct(entity, &sendSmsInfo);
        setResultCode (entity, vgSmsSigOutApexSendSmsNormal(entity, &sendSmsInfo));
    }
#endif
    else
    {
        smsContext_p->sendRetries  = 0;
        smsContext_p->smsLength    = 0;
        smsContext_p->firstRecord  = 0;
        smsContext_p->readState    = SM_RW_ABSOLUTE;
        smsContext_p->concatSms.sequenceNumber = 0;
        smsContext_p->concatSms.sequenceLength = 0;

        /* job132548: add code to handle 24.011 E.2 errors */
        if( (signal_p->requestStatus == SM_REQ_PS_ERROR) &&
            (signal_p->statusOfReport == TRANSFER_ERROR))
        {
            setResultCode (entity, vgSmsUtilConvertNetworkRequestError (signal_p->rpCause.cause));
        }
        else
        {
            setResultCode (entity, vgSmsUtilConvertRequestError(entity, signal_p->requestStatus));
        }
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    sigApexSmStoreCnf
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmStoreCnf contining the information
 *              for the SIG_APEX_SM_STORE_CNF signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_STORE_CNF signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/

void vgSmsSigInApexSmStoreCnf (const VgmuxChannelNumber entity, const ApexSmStoreCnf *signal_p )
{
    SmsContext_t               *smsContext_p = ptrToSmsContext (entity);
    CiapSms                     store;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    /* Make sure we're executing a command */
    FatalAssert (getResultCode (entity) == RESULT_CODE_PROCEEDING);

    if( (signal_p->requestStatus == SM_REQ_OK) &&
        (signal_p->shortMessageWritten) )
    {

        if( (cfRvSmHandleConcatSms == TRUE) &&
            (smsContext_p->concatSms.sequenceNumber > 0))
        {
            /* We don't manage concat SMS on ME*/
            FatalCheck( smsContext_p->cpmsWriteSend == VG_CPMS_STORAGE_SM, smsContext_p->cpmsWriteSend, 0, 0);

            /*
            ** we have stored the first part of the concat SMS so now want all
            ** subsequent parts to have reference to the first part.
            */
            if(smsContext_p->concatSms.sequenceNumber == 1)
            {
                smsContext_p->firstRecord = (Int8)signal_p->recordNumber;
            }

            /* store next part of the concatenated SMS, the record number must be set
               to the record number of the first part that was sent i.e. smsContext_p->firstRecord */
            vgSmsUtilFormatConcatSmsStruct(entity, &store);
            setResultCode (entity, vgSmsSigOutApexStoreSmsNormal(entity, &store, smsContext_p->firstRecord));
        }
        else
        {
            smsContext_p->smsLength = 0;

            if(smsContext_p->concatSms.sequenceLength == 0)
            {
                /* this is non-concat SMS */
                smsContext_p->firstRecord = (Int8)signal_p->recordNumber;
            }
            else
            {
                /* this is concat SMS */
                WarnAssert( cfRvSmHandleConcatSms == TRUE);
                smsContext_p->concatSms.sequenceLength = 0;
            }

            vgSmsPrintNewline(entity);
            vgSmsPuts(entity, (const Char*)"+CMGW: ");
            vgSmsPrintInt16 (entity, smsContext_p->firstRecord);
            vgSmsPrintNewline(entity);
            vgSmsFlush(entity);

            smsContext_p->firstRecord = 0;

            setResultCode (entity, RESULT_CODE_OK);
        }
    }
    else
    {
        smsContext_p->concatSms.sequenceLength = 0;
        smsContext_p->concatSms.sequenceNumber = 0;
        smsContext_p->smsLength   = 0;
        smsContext_p->firstRecord = 0;

        setResultCode (entity, vgSmsUtilConvertRequestError(entity, signal_p->requestStatus));
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    sigApexSmSendFromSimCnf
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmSendFromSimCnf contining the information
 *              for the SIG_APEX_SM_SEND_FROM_SIM_CNF signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_SEND_FROM_SIM_CNF signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/

void vgSmsSigInApexSmSendFromSimCnf (const VgmuxChannelNumber entity, const ApexSmSendFromSimCnf *signal_p )
{
  Int16 i;
  SmsCommonContext_t   *smsCommonContext_p = ptrToSmsCommonContext();

  /* Make sure we're executing a command */
  FatalAssert (getResultCode (entity) == RESULT_CODE_PROCEEDING);

  /*job116776 introducing moreMessagesToSend functionality*/
  if (smsCommonContext_p->moreMsgsToSendMode == 1)
  {
    vgCiStartTimer(TIMER_TYPE_SMS_CMMS, entity);
  }

  if (signal_p->requestStatus == SM_REQ_OK)
  {
    vgSmsPrintNewline(entity);
    vgSmsPuts(entity, (Char *)("+CMSS: "));
    vgSmsPrintInt16 (entity,(Int16)signal_p->messageRef[0]);

    /* if extra info is enabled i.e. *MSMEXTRAINFO=1 then display the message ids
    ** of all parts of the concatenated SMS. These, together with status reports
    ** (+CNMI=2,1,0,1,0), tell you when the message has been delivered - IF the
    ** network supports it.
    */
    if ( vgSmsUtilIsExtraInformationEnabled() )
    {
      for (i=1; i<signal_p->numMessages; i++)
      {
        vgSmsPuts(entity, (const Char*)",");
        vgSmsPrintInt16 (entity,(Int16)signal_p->messageRef[i]);
      }
    }

    vgSmsPrintNewline(entity);
    vgSmsFlush(entity);

    setResultCode (entity, RESULT_CODE_OK);
  }
  else
  {
    /* job132548: add code to handle 24.011 E.2 errors */
    if ((signal_p->requestStatus == SM_REQ_PS_ERROR) &&
        (signal_p->statusOfReport == TRANSFER_ERROR))
    {
      setResultCode (entity, vgSmsUtilConvertNetworkRequestError (signal_p->rpCause.cause));
    }
    else
    {
      setResultCode (entity, vgSmsUtilConvertRequestError(entity, signal_p->requestStatus));
    }
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigInApexSmReadSmsrCnf
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmReadCnf contining the information
 *              for the SIG_APEX_SM_READ_SMSR_CNF signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_READ_SMSR_CNF signal received
 *              from the BL task's shell process.
 *
 * Notes:       These is some pseudo 'C' code that explains how this
 *              functions behaves for the various AT commands:
 *
 *    switch ( currentAtCommand )
 *    {
 *      case VG_AT_SMS_CMGL:
 *          display it & request the next sms in the list
 *      break;
 *
 *      case VG_AT_SMS_CMGR:
 *          display it
 *      break;
 *   }
 *
 *-------------------------------------------------------------------------*/

void vgSmsSigInApexSmReadSmsrCnf( const VgmuxChannelNumber entity, const ApexSmReadSmsrCnf *signal_p )
{
    SmsContext_t           *smsContext_p    = ptrToSmsContext (entity);
    VgDisplaySmsSrParam    *display_p = PNULL;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    /* Make sure we're executing a command */
    FatalAssert(getResultCode (entity) == RESULT_CODE_PROCEEDING);
    /* Make sure the old (Phase 1) way of sending a msg from the SIM is not used */
    FatalAssert(smsContext_p->readState != SM_WRITE_FREE);
    /* Make sure only the commands we expect */
    FatalAssert((getCommandId (entity) == VG_AT_SMS_CMGL) ||
                (getCommandId (entity) == VG_AT_SMS_CMGR) ||
                (getCommandId (entity) == VG_AT_SMS_MCMGR));

    KiAllocZeroMemory(  sizeof(VgDisplaySmsSrParam),
                        (void **) &display_p);

    switch( signal_p->requestStatus )
    {
        case SM_REQ_OK:
        {
            /* Remember from where we have to continue to read SMS-SR (used by CMGL command) */
            smsContext_p->cmglRecordNumberToSearchFrom = signal_p->recordNumber;

            /* Create display structure*/
            display_p->smStatusReport  = signal_p->smStatusReport;
            display_p->recordNumber    = signal_p->recordNumber;
            display_p->processingMe    = FALSE;

            displaySmsrAndRequestNextOne(entity, display_p);
        }
        break;

        case SM_REQ_RECORD_NOT_FOUND:
        {
            if( smsContext_p->readState != SM_RW_ABSOLUTE )
            {
                if(getCommandId (entity) == VG_AT_SMS_MCMGR)
                {
                    FatalAssert(smsContext_p->readState == SM_PREVIEW_ONLY);
                    setResultCode(entity, vgSmsUtilConvertRequestError (entity, signal_p->requestStatus));

                    /* reset back to defaults... */
                    smsContext_p->readState = SM_RW_ABSOLUTE;
                    smsContext_p->smsLength = 0;
                }
                else
                {
                    /* Got to end of CMGL */
                    /* TODO: Check this is correct */
                    setResultCode(entity, RESULT_CODE_OK);
                      
                    /* reset back to defaults... */
                    smsContext_p->readState = SM_RW_ABSOLUTE;
                    smsContext_p->smsLength = 0;
                }
            }
            else
            {
                setResultCode(entity, vgSmsUtilConvertRequestError (entity, signal_p->requestStatus));

                /* reset back to defaults... */
                smsContext_p->readState = SM_RW_ABSOLUTE;
                smsContext_p->smsLength = 0;
            }
        }
        break;

        case SM_REQ_RECORD_EMPTY_FOUND:
        {
            if(getCommandId (entity) == VG_AT_SMS_MCMGR)
            {
               if(smsContext_p->readState == SM_PREVIEW_ONLY)
               {
                 /* we've got to the end of the *MCMGR */
                 setResultCode(entity, RESULT_CODE_OK);
               }
               else
               {
                 setResultCode(entity, vgSmsUtilConvertRequestError (entity, signal_p->requestStatus));
               }
            }
            else
            {
               if( smsContext_p->readState == SM_RW_ABSOLUTE )
               {
                   /* we've got to the end of the CMGR */
                   setResultCode(entity, RESULT_CODE_OK);
               }
               else
               {
                   setResultCode(entity, vgSmsUtilConvertRequestError (entity, signal_p->requestStatus));
               }
            }

            /* reset back to defaults... */
            smsContext_p->readState = SM_RW_ABSOLUTE;
            smsContext_p->smsLength = 0;
        }
        break;

        default:
        {
            setResultCode(entity, vgSmsUtilConvertRequestError (entity, signal_p->requestStatus));

            /* reset back to defaults... */
            smsContext_p->readState = SM_RW_ABSOLUTE;
            smsContext_p->smsLength = 0;
        }
        break;
    }

    KiFreeMemory( (void**)&display_p);
}

/*--------------------------------------------------------------------------
 *
 * Function:    sigApexSmDeleteSmsrCnf
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmDeleteSmsrCnf contining the information
 *              for the SIG_APEX_SM_DELETE_SMSR_CNF signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_DELETE_SMSR_CNF signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/

void vgSmsSigInApexSmDeleteSmsrCnf( const VgmuxChannelNumber entity, const ApexSmDeleteSmsrCnf *signal_p )
{
    SmsContext_t           *smsContext_p        = ptrToSmsContext(entity);
    SmsCommonContext_t     *smsCommonContext_p  = ptrToSmsCommonContext ();
    SmsSimAccessType        simAccessType;
    ResultCode_t            result;
    GeneralContext_t       *generalContext_p    = ptrToGeneralContext (entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);
    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    /* Make sure we're executing a command */
    FatalAssert(getResultCode(entity) == RESULT_CODE_PROCEEDING);

    if( (signal_p->requestStatus == SM_REQ_OK) &&
        (signal_p->smsrDeleted) )
    {
        /* Update the file capacity*/
        smsCommonContext_p->smSimState.numberOfSmsrRecords  = signal_p->totalSmsrRecord;
        smsCommonContext_p->smSimState.usedSmsrRecords      = signal_p->usedSmsrRecord;

        /* All SMS SR are assumed to have a "REC READ" Status */
        if( vgSmsUtilConvertDelFlagToSimAccessType(smsContext_p->delFlag, &simAccessType) )
        {
            result = vgSmsSigOutApexReadSmsrNoData(entity, SM_NEXT_READ);

            if( result == RESULT_CODE_PROCEEDING )
            {
                setResultCode (entity, result);
            }
        }
        /* there is no more messages to delete */
        else
        {
            setResultCode(entity, RESULT_CODE_OK);
            smsContext_p->delFlag = 0;
        }
    }
    else
    {
        setResultCode(entity, vgSmsUtilConvertRequestError(entity, signal_p->requestStatus));
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    sigApexSmReadSmspCnf
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmReadSmspCnf contining the information
 *              for the SIG_APEX_SM_READ_SMSP_CNF signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_READ_SMSP_CNF signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/

void vgSmsSigInApexSmReadSmspCnf (const VgmuxChannelNumber entity, const ApexSmReadSmspCnf *signal_p )
{
  VgSmspState smspState = (VgSmspState)signal_p->commandRef;
  VgmuxChannelNumber   profileEntity;

  /* Retrieve the state information from the commandRef */
  if ( (signal_p->requestStatus == SM_REQ_OK) && (TRUE == signal_p->smspDataValid) )
  {
    switch (smspState)
    {
      case VG_SMSP_READ_QUERY_SCA:
      {
        FatalAssert (getResultCode (entity) == RESULT_CODE_PROCEEDING);

        vgSmsUtilSetSca (&signal_p->smsp);

        vgSmsPrintNewline(entity);
        vgSmsPrintSca (entity);
        vgSmsFlush(entity);
        setResultCode (entity, RESULT_CODE_OK);
        break;
      }

      case VG_SMSP_READ_QUERY_VP:
      {
        FatalAssert (getResultCode (entity) == RESULT_CODE_PROCEEDING);

        vgSmsSetValidityPeriod (&signal_p->smsp);

        vgSmsPrintNewline(entity);
        vgSmsPrintValidityPeriod (entity);
        vgSmsFlush(entity);
        setResultCode (entity, RESULT_CODE_OK);
        break;
      }

      case VG_SMSP_READ_WRITE_VP:  /* fall through... */
      case VG_SMSP_READ_WRITE_SCA:
      {
        FatalAssert (getResultCode (entity) == RESULT_CODE_PROCEEDING);
        setResultCode (entity, vgSmsSigOutApexWriteSmspReq (entity, signal_p));
        break;
      }

      case VG_SMSP_READ_UNSOLICITED:
      {
        /* We only get here from a unsolicited smready ind */
        vgSmsUtilSetSca        (&signal_p->smsp);
        vgSmsSetValidityPeriod (&signal_p->smsp);

        for ( profileEntity=0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
        {
          if (isEntityActive (profileEntity) && vgSmsUtilAreExtraUnsolicitedIndicationsEnabled (profileEntity))
          {
            vgSmsPrintSca (profileEntity);
          }
        }

        /* Now we need to explicitely release control of the
           entity which sent request */
        vgSmsUtilSetUnsolicitedEventHandlerFree();

        vgChManReleaseControlOfElement (CC_SHORT_MESSAGE_SERVICE, entity);

        break;
      }
      case VG_SMSP_READ_BLOCK_QUERY:
      {
        /* Last step in AT&v command displays SMSP read from BL*/
        vgSmsUtilSetSca         (&signal_p->smsp);
        vgSmsSetValidityPeriod (&signal_p->smsp);

        vgSmsPrintSca(entity);
        vgSmsPrintValidityPeriod(entity);

        /* Terminate the at&v command */
        setResultCode(entity, RESULT_CODE_OK);
        break;
      }
      default:
      {
        /* Unexpected smspState */
        FatalParam(entity, smspState, 0);
//        setResultCode (entity, RESULT_CODE_ERROR);
        break;
      }
    }
  }
  else
  {
    if (VG_SMSP_READ_UNSOLICITED == smspState)
    {
      /* Ignore the actual error code because we can't do
         anything about it anyway. */
      sendTSmsInfo (VG_CMS_ERROR_SMREADY_COULDNT_READ_SMSP);

      /* Now we need to explicitely release control of the
           entity which sent request */
      vgSmsUtilSetUnsolicitedEventHandlerFree();
      vgChManReleaseControlOfElement (CC_SHORT_MESSAGE_SERVICE, entity);
    }
    else
    {
      setResultCode (entity, vgSmsUtilConvertRequestError(entity, signal_p->requestStatus));
    }
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigInApexSmWriteSmspCnf
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmWriteSmspCnf contining the information
 *              for the SIG_APEX_SM_WRITE_SMSP_CNF signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_WRITE_SMSP_CNF signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/

void vgSmsSigInApexSmWriteSmspCnf (const  VgmuxChannelNumber entity, const ApexSmWriteSmspCnf *signal_p)
{
  SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext ();

  if (signal_p->requestStatus == SM_REQ_OK)
  {
    /* Update local copy of SCA with value written to SIM.... */
    memcpy(&smsCommonContext_p->sca, &smsCommonContext_p->vgTempSca, sizeof(SmsAddress));
    setResultCode (entity, RESULT_CODE_OK);
  }
  else
  {
    setResultCode (entity, vgSmsUtilConvertRequestError(entity, signal_p->requestStatus));
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigIn_ApexSmCommandCnf
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmCommandCnf contining the information
 *              for the SIG_APEX_SM_COMMAND_CNF signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_COMMAND_CNF signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/
void vgSmsSigInApexSmCommandCnf (const VgmuxChannelNumber entity, const ApexSmCommandCnf *signal_p )
{
  SmsContext_t        *smsContext_p       = ptrToSmsContext (entity);
  SmsCommonContext_t  *smsCommonContext_p = ptrToSmsCommonContext ();
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);

#endif


  /*job116776 introducing moreMessagesToSend functionality*/
  if (smsCommonContext_p->moreMsgsToSendMode == 1)
  {
    vgCiStartTimer(TIMER_TYPE_SMS_CMMS, entity);
  }

  if ( signal_p->requestStatus == SM_REQ_OK)
  {
    /* reset the SMS Length counter */
    smsContext_p->smsLength = 0;

    vgSmsPrintNewline(entity);
    vgSmsPuts(entity, (const Char*)"+CMGC: ");
    vgSmsPrintInt16(entity, (Int16)signal_p->messageRef);
    vgSmsPrintNewline(entity);
    vgSmsFlush(entity);

    setResultCode (entity, RESULT_CODE_OK);
  }
  else
  {
    setResultCode (entity, vgSmsUtilConvertRequestError(entity, signal_p->requestStatus));
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:    sigApexSmStatusCnf
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmStatusCnf contining the information
 *              for the SIG_APEX_SM_STATUS_CNF signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_STATUS_CNF signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/
void vgSmsSigInApexSmStatusCnf (const VgmuxChannelNumber entity, const ApexSmStatusCnf *signal_p)
{
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    const Int8          smRequestStatusCodeTable[] =
    {
        0,  /* SM_REQ_OK */
        1,  /* SM_REQ_SM_NOT_READY */
        2,  /* SM_REQ_SM_SEND_READY */
        3   /* SM_REQ_SM_SNDRCV_READY */
    };
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
    /* Make sure we're executing a command */
    FatalAssert (getResultCode (entity) == RESULT_CODE_PROCEEDING);

    switch (getCommandId (entity))
    {
        case VG_AT_SMS_MSMSTATUS:
            {
                vgSmsPrintNewline(entity);
                vgSmsPuts(entity, (const Char*)getCommandName(entity));
                vgSmsPuts(entity, (const Char*)": ");
                vgSmsPrintInt16(entity, (Int16)smRequestStatusCodeTable[signal_p->requestStatus]);
                vgSmsPutc(entity,',');
                vgSmsPrintInt16(entity, (Int16)signal_p->unreadRecords);
                vgSmsPutc(entity,',');
                vgSmsPrintInt16(entity, (Int16)signal_p->usedRecords);
                vgSmsPutc(entity,',');
                vgSmsPrintInt16(entity, (Int16)signal_p->memCapExceeded);
                vgSmsPutc(entity,',');
                vgSmsPrintInt16(entity, (Int16)signal_p->numberOfSmsRecords);
                vgSmsPutc(entity,',');
                vgSmsPrintInt16(entity, (Int16)signal_p->firstFreeRecord);
                vgSmsPutc(entity,',');
                vgSmsPrintInt16(entity, (Int16)signal_p->numberOfSmspRecords);
                vgSmsPutc(entity,',');
                vgSmsPrintInt16(entity, (Int16)signal_p->smsRoute);

                vgSmsPrintNewline(entity);
                vgSmsFlush(entity);

                setResultCode (entity, RESULT_CODE_OK);
                break;
            }

        case VG_AT_SMS_MMGRW:
            {
                vgSmsPrintNewline(entity);
                vgSmsPuts(entity, (const Char*)"*MMGRW: ");
                vgSmsPrintInt16(entity, (Int16)signal_p->smsStoreLoc);
                vgSmsPrintNewline(entity);
                vgSmsFlush(entity);
                setResultCode (entity, RESULT_CODE_OK);
                break;
            }

        default:
            {
                /* Unexpected CommandId */
                FatalParam(entity, getCommandId (entity), 0);
                break;
            }
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigInApexSmReadNoDataCnf
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmReadCnf contining the information
 *              for the SIG_APEX_SM_READ_NODATA_CNF signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_READ_NODATA_CNF signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/

void vgSmsSigInApexSmReadNoDataCnf( const VgmuxChannelNumber entity, const ApexSmReadCnf *signal_p )
{
  SmsContext_t       *smsContext_p = ptrToSmsContext (entity);
  SmsSimAccessType    simAccessType;
  GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  /* Make sure we're executing a command */
  FatalAssert (getResultCode (entity) == RESULT_CODE_PROCEEDING);
  /* Make sure only the commands we expect */
  FatalAssert (getCommandId (entity) == VG_AT_SMS_CMGD);

  if (signal_p->requestStatus == SM_REQ_OK)
  {
    /* as the record is there, delete it,
     * if we can not run delete, just stop and report an error */
    if( vgSmsSigOutApexDeleteSms( entity, signal_p->recordNumber) != RESULT_CODE_PROCEEDING)
    {
      setResultCode (entity, VG_CMS_ERROR_UNKNOWN);
      smsContext_p->delFlag = 0;
    }
  }
  else if (signal_p->requestStatus == SM_REQ_RECORD_NOT_FOUND)
  {
    /* convert next <delflag> to SmsSimAccessType - note -- */
    if (vgSmsUtilConvertDelFlagToSimAccessType(--smsContext_p->delFlag, &simAccessType))
    {
      /* if we can not run read just stop and return an error */
      if (vgSmsSigOutApexReadSmsNoData(entity, simAccessType) != RESULT_CODE_PROCEEDING)
      {
        setResultCode (entity, VG_CMS_ERROR_UNKNOWN);
        smsContext_p->delFlag = 0;
      }
    }
    else
    {
      /* we've got to the end of the CMGD */
      setResultCode (entity, RESULT_CODE_OK);
    }
  }
  else
  {
    setResultCode (entity, vgSmsUtilConvertRequestError (entity, signal_p->requestStatus));
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigInApexSmReadSmsrNoDataCnf
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmReadSmsrCnf contining the information
 *              for the SIG_APEX_SM_READ_SMSR_NODATA_CNF signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_READ_SMSR_NODATA_CNF signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/

void vgSmsSigInApexSmReadSmsrNoDataCnf( const VgmuxChannelNumber entity, const ApexSmReadSmsrCnf *signal_p )
{
    SmsContext_t       *smsContext_p        = ptrToSmsContext(entity);
    GeneralContext_t   *generalContext_p    = ptrToGeneralContext(entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);
    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    /* Make sure we're executing a command */
    FatalAssert(getResultCode(entity) == RESULT_CODE_PROCEEDING);
    /* Make sure only the commands we expect */
    FatalAssert(getCommandId(entity) == VG_AT_SMS_CMGD);

    if( signal_p->requestStatus == SM_REQ_OK )
    {
        /* as the record is there, delete it,
        * if we can not run delete, just stop and report an error */
        if( vgSmsSigOutApexDeleteSmsr( entity, signal_p->recordNumber) != RESULT_CODE_PROCEEDING )
        {
            setResultCode(entity, VG_CMS_ERROR_UNKNOWN);
            smsContext_p->delFlag = 0;
        }
    }
    else if( signal_p->requestStatus == SM_REQ_RECORD_NOT_FOUND )
    {
        setResultCode(entity, RESULT_CODE_OK);
        smsContext_p->delFlag = 0;
    }
    else
    {
        setResultCode(entity, vgSmsUtilConvertRequestError (entity, signal_p->requestStatus));
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigInApexSmStoreLocCnf
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmReadCnf contining the information
 *              for the SIG_APEX_SM_STORE_LOC_CNF signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_STORE_LOC_CNF signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/

void vgSmsSigInApexSmStoreLocCnf (const VgmuxChannelNumber entity, const ApexSmStoreLocCnf* signal_p)
{
  /* Make sure we're executing a command */
  FatalAssert (getResultCode (entity) == RESULT_CODE_PROCEEDING);
  /* Make sure only the commands we expect */
  FatalAssert (getCommandId (entity) == VG_AT_SMS_MMGRW);
  /* we've got to the end of the *MMGRW */
  setResultCode (entity, RESULT_CODE_OK);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigInApexSmSetLocStatusCnf
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmReadCnf contining the information
 *              for the SIG_APEX_SM_SET_LOC_STATUS_CNF signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_SET_LOC_STATUS_CNF signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/

void vgSmsSigInApexSmSetLocStatusCnf (const VgmuxChannelNumber entity, const ApexSmSetLocStatusCnf* signal_p)
{
  /* Make sure we're executing a command */
  FatalAssert (getResultCode (entity) == RESULT_CODE_PROCEEDING);
  /* Make sure only the commands we expect */
  FatalAssert (getCommandId (entity) == VG_AT_SMS_MMGSC);
  /* we've got to the end of the *MMGRW */
  if (signal_p->requestStatus == SM_REQ_OK)
  {
    setResultCode (entity, RESULT_CODE_OK);
  }
  else
  {
    setResultCode (entity, vgSmsUtilConvertRequestError (entity, signal_p->requestStatus));
  }

}
/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigInApexSmStkInfoInd
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmStkInfoInd containing MO SMS Call control on SIM info
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_SET_LOC_STATUS_CNF signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/

void vgSmsSigInApexSmStkInfoInd      (const VgmuxChannelNumber entity,
                                      const ApexSmStkInfoInd* signal_p)
{

  Boolean               found = FALSE;
  Int8                  entityIndex = (Int8)entity;
  SimatAlphaIdentifier  alphaId;


  for (entityIndex = 0; (entityIndex < CI_MAX_ENTITIES) && (found == FALSE); entityIndex++)
  {
    if (isEntityActive ((VgmuxChannelNumber)entityIndex) == TRUE)
    {

      if ((getResultCode ((VgmuxChannelNumber)entityIndex) == RESULT_CODE_PROCEEDING) &&
      ((getCommandId ((VgmuxChannelNumber)entityIndex) == VG_AT_SMS_CMGS ) ||
       (getCommandId ((VgmuxChannelNumber)entityIndex) == VG_AT_SMS_CMSS )))
      {
        found = TRUE;
        memcpy (&alphaId, &signal_p->alphaId, sizeof (SimatAlphaIdentifier));
        vgStkDisplayCallControlMessage(SIMAT_CC_SM_OPERATION, signal_p->ccStatus,
        signal_p->alphaIdPresent, &alphaId, entityIndex);
      }
    }
  }
}

/***************************************************************************
****************************************************************************
**
**                These are unsolicited message handlers
**
****************************************************************************
***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigInApexSmStatusReportInd
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmCommandCnf contining the information
 *              for the SIG_APEX_SM_STATUS_REPORT_IND signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_STATUS_REPORT_IND signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/
void vgSmsSigInApexSmStatusReportInd( const ApexSmStatusReportInd *signal_p )
{
    SmsCommonContext_t     *smsCommonContext_p = ptrToSmsCommonContext ();
    VgmuxChannelNumber      profileEntity      = 0;
    Boolean                 found              = FALSE;

    /* Update the file capacity*/
    smsCommonContext_p->smSimState.numberOfSmsrRecords  = signal_p->totalSmsrRecord;
    smsCommonContext_p->smSimState.usedSmsrRecords      = signal_p->usedSmsrRecord;

    for( profileEntity =0; (profileEntity < CI_MAX_ENTITIES) && (FALSE == found); profileEntity++ )
    {
        if( isEntityActive(profileEntity))
        {
            /* send Unsol Result Code only if <ds> parameter in +CNMI is set */
            switch( getProfileValue(profileEntity, PROF_CNMI+3) )
            {
                case 1:
                    {
                        vgSmsPrintUnsolicitedSmStatusReport (profileEntity, signal_p);

                        /* these messages needs to be acknowledged by CNMA if CSMS <service> = 1 */
                        if( getProfileValue(profileEntity, PROF_CSMS) == 1 )
                        {
                            smsCommonContext_p->msgToAck   = TRUE;
                            found = TRUE;
                            /* shortMsgId will be the same as in the SM */
                            /* start timer to make sure that the CI task respond if user doesnt */
                            vgCiStartTimer(TIMER_TYPE_SMS_TR2M, profileEntity);
                        }
                    }
                    break;

                case 2:
                    {
                        if( signal_p->storingLoc == ABSM_STORAGE_NONE)
                        {
                            vgSmsPrintUnsolicitedSmStatusReport( profileEntity, signal_p);
                        }
                        else
                        {
                            vgSmsPrintUnsolicitedSmStatusReportInd( profileEntity,
                                                                    (Int16)signal_p->smsrRecordNum);
                        }
                        /* Messages not routed to TE doesn't have to be acknowledged by CNMA*/
                    }
                    break;

                default:
                    break;
            }
        }
    }

    /* Now we need to set the unsol event to free */
    vgSmsUtilSetUnsolicitedEventHandlerFree();
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigInApexSmSmsrStoreInd
 *
 * Parameters:  (In) signal_p - a pointer to a structure contining
 *              the information for the SIG_APEX_SM_SMSR_STORE_IND signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_SMSR_STORE_IND signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/
void vgSmsSigInApexSmSmsrStoreInd( const ApexSmSmsrStoreInd *signal_p )
{
    /* Now we need to set the unsol event to free */
    vgSmsUtilSetUnsolicitedEventHandlerFree();
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigIn_ApexSmMsgReceivedInd
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmMsgreceivedInd contining the information
 *              for the SIG_APEX_SM_MSG_RECEIVED_IND signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_MSG_RECEIVED_IND signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/

void vgSmsSigInApexSmMsgReceivedInd ( const ApexSmMsgReceivedInd *signal_p)
{
    SmsCommonContext_t     *smsCommonContext_p          = ptrToSmsCommonContext ();
    VgmuxChannelNumber      profileEntity               =  0;
    Boolean                 found                       =  FALSE;
    Boolean                 munsol_mw_enabled           =  FALSE;

    /* MUST do this before we do vgSmsUtilHasSmsStatusSimOverflowChanged() */
    smsCommonContext_p->smSimState.unreadRecords  = signal_p->unreadRecords;
    smsCommonContext_p->smSimState.usedRecords    = signal_p->usedRecords;
    smsCommonContext_p->smSimState.memCapExceeded = signal_p->memCapExceeded;

    /* If the status has changed and we want to display it, do so */
    if( (vgSmsUtilHasSmsStatusSimOverflowChanged()) &&
        (signal_p->memCapExceeded) )
    {
        for(    profileEntity = 0;
                (   (profileEntity < CI_MAX_ENTITIES) &&
                    (TRUE != found) );
                profileEntity++)
        {
            if( (isEntityActive (profileEntity)) &&
                (getEntityState(profileEntity) == ENTITY_IDLE) &&
                (getProfileValue (profileEntity,PROF_CNMI + 1) != 0))
            {
                sendTSmsInfo(VG_CMS_ERROR_MEMORY_FULL);
                found = TRUE;
            }
        }
    }

    if ( signal_p->memCapExceeded == FALSE)
    {
        /* Output CMTI */
        for(    profileEntity = 0;
                profileEntity < CI_MAX_ENTITIES;
                profileEntity++)
        {
            if( (isEntityActive (profileEntity)) &&
                (getEntityState(profileEntity) == ENTITY_IDLE) &&
                (getProfileValue (profileEntity,PROF_CNMI + 1) != 0))
            {
                vgSmsPrintUnsolicitedSmMsgReceivedInd(  profileEntity,
                                                        signal_p->messageLoc,
                                                        signal_p->recordNumber);
            }
        }
    }

    if( (signal_p->memCapExceeded == FALSE) &&
        (signal_p->messageLoc == ABSM_STORAGE_SM) )
    {
        for(    profileEntity = 0;
                (   (profileEntity < CI_MAX_ENTITIES) &&
                    (TRUE != found) );
                profileEntity++)
        {
            if( (isEntityActive (profileEntity)) &&
                (isProfileValueBitEqual(    profileEntity,
                                            PROF_MUNSOL,
                                            PROF_BIT_MMWT,
                                            REPORTING_ENABLED)))
            {
                munsol_mw_enabled = TRUE;

                if (!vgMsSMSMsgFetch(profileEntity, signal_p->recordNumber))
                {
                    /* Can't read EFSMS - need to try again shortly.... */
                    vgCiStartTimer(TIMER_TYPE_SMS_MSG, profileEntity);
                    found = TRUE;
                }
            }
        }

        if(!munsol_mw_enabled)
        {
            /* Now we need to set the unsol event to free */
            vgSmsUtilSetUnsolicitedEventHandlerFree();
        }
    }
    else
    {
        /* Now we need to set the unsol event to free */
        vgSmsUtilSetUnsolicitedEventHandlerFree();
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigIn_ApexSmDeliveryInd
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmDeliveryInd contining the information
 *              for the SIG_APEX_SM_DELIVERY_IND signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_DELIVERY_IND signal received
 *              from the BL task's shell process.
 *              NB: Class 0 SMs are only sent to the CRM if the CNMI <mt>
 *              parameter = 2.
 *
 *-------------------------------------------------------------------------*/
void vgSmsSigInApexSmDeliveryInd( const ApexSmDeliveryInd *signal_p )
{
    SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext ();
    VgmuxChannelNumber entity              = 0;
    VgmuxChannelNumber activeEntity        = VGMUX_CHANNEL_INVALID;
    Boolean            found               = FALSE;

    /* Prepare context parameter*/
    smsCommonContext_p->printCmtOnlyOnMmi = FALSE;

    /* Record the new updated values - must do this before calling
    vgSmsUtilHasSmsStatusSimOverflowChanged()  */
    smsCommonContext_p->smSimState.unreadRecords  = signal_p->unreadRecords;
    smsCommonContext_p->smSimState.usedRecords    = signal_p->usedRecords;
    smsCommonContext_p->smSimState.memCapExceeded = signal_p->memCapExceeded;

    /*  Update the Sms SIM Status - we don't care whether
     *  status has changed so just throw away the result.
     */
    (void)vgSmsUtilHasSmsStatusSimOverflowChanged();

    /* Find a entity to get the profile */
    entity = 0;
    while(  (entity < CI_MAX_ENTITIES) &&
            (FALSE == found) )
    {
        if( isEntityActive(entity) == TRUE)
        {
            activeEntity = entity;

            if( ((2 == getProfileValue(entity, PROF_CNMI + 1)) &&
                 (MSG_CLASS2 != signal_p->smsDataCodingScheme.msgClass)) ||
                (signal_p->smsDataCodingScheme.msgClass == MSG_CLASS3) )
            {
                found = TRUE;
            }
            else
            {
                entity++;
            }
        }
        else
        {
            entity++;
        }
    }

    /* Always print notification for class 0 on MMI channel*/
    if( (found == FALSE) &&
        (signal_p->smsDataCodingScheme.msgClass == MSG_CLASS0) &&
        (activeEntity != VGMUX_CHANNEL_INVALID))
    {
        entity = activeEntity;
        smsCommonContext_p->printCmtOnlyOnMmi = TRUE;
        found = TRUE;
    }

    /* dont send further +CMT result code to TE before previous one is acknowledged */
    if( (smsCommonContext_p->msgToAck == FALSE) &&
        (found == TRUE))
    {
        /* We release control of the unsolicited channel in this function... */
        vgSmsUtilSendUnsolicitedSmDeliveryIndToCrm( entity, signal_p);
        /* these messages needs to be acknowledged by CNMA if CSMS <service> = 1 */
        if (getProfileValue(entity, PROF_CSMS) == 1)
        {
            smsCommonContext_p->msgToAck   = TRUE;
            smsCommonContext_p->shortMsgId = signal_p->shortMsgId;
            /* start timer to make sure that the CI task respond if user doesnt */
            vgCiStartTimer(TIMER_TYPE_SMS_TR2M, entity);
        }
    }
    else
    {
        /* Now we need to set the unsol event to free */
        vgSmsUtilSetUnsolicitedEventHandlerFree();
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    sigApexSmRecordChangedInd
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmReadyInd contining the information
 *              for the SIG_APEX_SM_RECORD_CHANGED_IND signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_RECORD_CHANGED_IND signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/

void vgSmsSigInApexSmRecordChangedInd (const ApexSmRecordChangedInd *signal_p)
{
    SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext ();
    VgmuxChannelNumber profileEntity       = 0;
    Boolean            found               = FALSE;

    for (   profileEntity =0, found = FALSE;
            (profileEntity < CI_MAX_ENTITIES) && (!found);
            profileEntity++)
    {
        if ((isEntityActive (profileEntity)) && (getProfileValue(profileEntity, PROF_CNMI + 1) != 0))
        {
            found = TRUE;
        }
    }

    /* Record the new updated values - must do this before calling
    vgSmsUtilHasSmsStatusSimOverflowChanged()  */
    smsCommonContext_p->smSimState.unreadRecords  = signal_p->unreadRecords;
    smsCommonContext_p->smSimState.usedRecords    = signal_p->usedRecords;
    smsCommonContext_p->smSimState.memCapExceeded = signal_p->memCapExceeded;

    /* if the SMS memory is full display unsolicited CMS error */
    if (    (vgSmsUtilHasSmsStatusSimOverflowChanged()) &&
            (TRUE == signal_p->memCapExceeded) &&
            found )
    {
        sendTSmsInfo(VG_CMS_ERROR_MEMORY_FULL);
    }

    /* Now we need to set the unsol event to free */
    vgSmsUtilSetUnsolicitedEventHandlerFree();
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigIn_ApexSmReadyInd
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmReadyInd contining the information
 *              for the SIG_APEX_SM_READY_IND signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_READY_IND signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/

void vgSmsSigInApexSmReadyInd (const ApexSmReadyInd *signal_p)
{
  SmsCommonContext_t  *smsCommonContext_p = ptrToSmsCommonContext ();
  ResultCode_t         resultCode         = RESULT_CODE_ERROR;
  VgmuxChannelNumber   entity;
  SmRequestStatus      oldSimStatus       = smsCommonContext_p->smSimState.requestStatus;
  VgmuxChannelNumber   profileEntity      = 0;

  /* do a memcpy because smSimState is an ApexSmReadyInd */
  memcpy(&smsCommonContext_p->smSimState, signal_p, sizeof(ApexSmReadyInd));

  if( signal_p->requestStatus == SM_REQ_OK )
  {
      for( profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++ )
      {
         if( (isEntityActive (profileEntity)) && (vgSmsUtilAreExtraUnsolicitedIndicationsEnabled (profileEntity)))
         {
             vgSmsPrintUnsolicitedSmReadyInd (profileEntity, &smsCommonContext_p->smSimState);
         }
      }
  }

  /*Print unsolicited notifications for MMGI command*/
  if(    (smsCommonContext_p->smSimState.requestStatus == SM_REQ_OK)
      && (!smsCommonContext_p->smSimStateInitialized || oldSimStatus!=SM_REQ_OK) )
  {
      vgMMGIPrintEvent(VG_MMGI_SIM_ACCESS_OK);
  }
  else if(    (smsCommonContext_p->smSimState.requestStatus != SM_REQ_OK)
           && (!smsCommonContext_p->smSimStateInitialized || oldSimStatus==SM_REQ_OK) )
  {
      vgMMGIPrintEvent(VG_MMGI_SIM_ACCESS_NOK);
  }

  /*Set simState state to initialized*/
  smsCommonContext_p->smSimStateInitialized = TRUE;

  /*  Update the Sms Status SIM Overflow. We don't care whether status has
   *  changed so just throw away the result.
   */
  (void)vgSmsUtilHasSmsStatusSimOverflowChanged();

  for( profileEntity =0; profileEntity < CI_MAX_ENTITIES; profileEntity++ )
  {
       if (isEntityActive (profileEntity))
       {
           /* if the SMS memory is full display unsolicited CMS error */
           if(    (TRUE == signal_p->memCapExceeded)
               && (0 != getProfileValue(profileEntity, PROF_CNMI + 1)) )
           {
               sendTSmsInfo(VG_CMS_ERROR_MEMORY_FULL);
           }

           /* if there are unread SMS messages display unsolicited CMS error */
           if(    (signal_p->unreadRecords > 0)
               && (0 != getProfileValue(profileEntity, PROF_CNMI + 1)) )
           {
               sendTSmsInfo(VG_CMS_UNREAD_RECORDS_ON_SIM);
           }
       }
  }

  /* If SIM is ready to send then request a read of the Service Centre Address from the SIM */
  if( (signal_p->requestStatus == SM_REQ_SM_SEND_READY) || (signal_p->requestStatus == SM_REQ_OK) )
  {
      entity = findFirstEnabledChannel();
      /*
      ** We will use the first active entity for sending/receiving the
      ** "Read SCA from SIM" messages.
      */
      if( VGMUX_CHANNEL_INVALID != entity )
      {
          if(smsCommonContext_p->sca.length == 0)
          {
              resultCode = vgSmsSigOutApexReadSmspReq(entity, VG_SMSP_READ_UNSOLICITED);
          }
      }

      if( RESULT_CODE_PROCEEDING != resultCode )
      {
          sendTSmsInfo(VG_CMS_ERROR_SMREADY_COULDNT_READ_SMSP);

          /* Now we need to set the unsol event to free */
          vgSmsUtilSetUnsolicitedEventHandlerFree();
      }
  }
  else
  {
      /* Now we need to set the unsol event to free */
      vgSmsUtilSetUnsolicitedEventHandlerFree();
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigInApexSmStatusChangedInd
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmStatusChangedInd contining the information
 *              for the SIG_APEX_SM_STATUS_CHANGED_IND signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_STATUS_CHANGED_IND signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/
void vgSmsSigInApexSmStatusChangedInd (const ApexSmStatusChangedInd *signal_p)
{
    /* Now we need to set the unsol event to free */
    vgSmsUtilSetUnsolicitedEventHandlerFree();
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigInApexSmSrDeletedInd
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmStatusChangedInd contining the information
 *              for the SIG_APEX_SM_SR_DELETED_IND signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_SR_DELETED_IND signal received
 *              from the BL task's shell process.
 *              Update the SIM SMS-SR capacity
 *
 *-------------------------------------------------------------------------*/
void vgSmsSigInApexSmSrDeletedInd (const ApexSmSrDeletedInd *signal_p)
{
    SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext ();

    /* Update the SMS-SR file information*/
    smsCommonContext_p->smSimState.numberOfSmsrRecords  = signal_p->totalSmsrRecord;
    smsCommonContext_p->smSimState.usedSmsrRecords      = signal_p->usedSmsrRecord;

    /* Now we need to set the unsol event to free */
    vgSmsUtilSetUnsolicitedEventHandlerFree();
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigInApexSmStoreInd
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmStoreInd contining the information
 *              for the SIG_APEX_SM_STORE_IND signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_STORE_IND signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/
void vgSmsSigInApexSmStoreInd ( const ApexSmStoreInd *signal_p)
{
    /* Now we need to set the unsol event to free */
    vgSmsUtilSetUnsolicitedEventHandlerFree();
}
/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigInApexTransactonEndInd
 *
 * Parameters:  (In) signal_p - a pointer to a structure of type
 *              ApexSmStoreInd contining the information
 *              for the SIG_APEX_SM_TRANSACTION_END_IND signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SM_TRANSACTION_END_IND signal received
 *              from the BL task's shell process.
 *
 *-------------------------------------------------------------------------*/
void vgSmsSigInApexTransactonEndInd ( const ApexSmTransactionEndInd *signal_p)
{
    vgSmsUtilSetUnsolicitedEventHandlerFree();
}
/* END OF FILE */

