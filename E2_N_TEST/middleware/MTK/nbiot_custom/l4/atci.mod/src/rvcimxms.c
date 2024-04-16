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
 * RVCIMXMS.C
 *
 *
 ***************************************************************************
 *
 * This module containes the CI mux support for SMS.
 **************************************************************************/

#define MODULE_NAME "RVCIMXMS"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <vgmx_sig.h>
#include <rvcrhand.h>
#include <rvcimxms.h>
#include <rvmsut.h>
#include <rvcimxut.h>
#include <rvcimxsot.h>
#include <rvcimxms.h>  

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

union Signal
{
  CiMuxAtDataInd  ciMuxAtDataInd;    
};


/*--------------------------------------------------------------------------
*
* Function:    processSmsInput
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Takes SIG_CIMUX_AT_DATA_IND off cache and treats as for SMS
*              message content.
*-------------------------------------------------------------------------*/

void processSmsInput (SignalBuffer *signal_p, const VgmuxChannelNumber entity)
{
  CiMuxAtDataInd     *ciMuxAtDataInd    = &signal_p->sig->ciMuxAtDataInd;
  ChannelContext_t   *channelContext_p   = ptrToChannelContext   (entity);
  SmsContext_t       *smsContext_p       = ptrToSmsContext       (entity);

#if defined (ENABLE_LONG_AT_CMD_RSP)
  Char    echoString[AT_LARGE_BUFF_SIZE] = {0};
#else
  Char    echoString[CIMUX_MAX_AT_DATA_LENGTH] = {0};
#endif
  Int16   echoLength = 0;
  Char    character;
  Int16   index;
  Boolean endOfSmsMessage = FALSE;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(channelContext_p != PNULL, entity, 0, 0);
  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  for (index = 0;
       (index < ciMuxAtDataInd->length) && (endOfSmsMessage == FALSE);
       index++)
  {
    character = ciMuxAtDataInd->data[index];
    switch (character)
    {
      case ESC_CHAR:    /* Abort */
      {
        channelContext_p->dataEntryMode = DATA_ENTRY_AT_COMMAND;
        setResultCode (entity, RESULT_CODE_OK);
        endOfSmsMessage = TRUE;
        break;
      }
      case CTRL_Z_CHAR: /* Process */
      {
        channelContext_p->dataEntryMode = DATA_ENTRY_AT_COMMAND;
        vgSmsUtilProcessSmsText( entity );
        endOfSmsMessage = TRUE;
        break;
      }
      default:
      {
        if (character == getProfileValue (entity, PROF_S5))  /* Backspace Char */
        {
          if (smsContext_p->smsLength > 0)
          {
            echoChar (echoString, getProfileValue (entity, PROF_S5), &echoLength, entity);
            echoChar (echoString, SPACE_CHAR, &echoLength, entity);
            echoChar (echoString, getProfileValue (entity, PROF_S5), &echoLength, entity);
            smsContext_p->smsLength -= 1;
          }
        }
        else if (character == getProfileValue (entity, PROF_S3)) /* Carriage Return Char */
        {
          if (smsContext_p->smsLength < (Int32)smsContext_p->maxSmsMessageLength)
          {
            echoChar (echoString, getProfileValue (entity, PROF_S3), &echoLength, entity);
            echoChar (echoString, getProfileValue (entity, PROF_S4), &echoLength, entity);
            echoChar (echoString, '>', &echoLength, entity);
            echoChar (echoString, SPACE_CHAR, &echoLength, entity);

            /* Do not add CR when in PDU mode */
            if ( vgSmsUtilIsValidMessageChar(entity, character) )
            {
              smsContext_p->smsMessage[smsContext_p->smsLength] = character;
              smsContext_p->smsLength++;
            }
          }
        }
        else  /* All other chars */
        {
          if (smsContext_p->smsLength < (Int32)smsContext_p->maxSmsMessageLength)
          {
            echoChar (echoString, character, &echoLength, entity);

            if ( vgSmsUtilIsValidMessageChar(entity, character) )
            {
              smsContext_p->smsMessage[smsContext_p->smsLength] = character;
              smsContext_p->smsLength++;
            }
          }
        }
        break;
      }
    }
  }

  if (index == ciMuxAtDataInd->length)
  {
    /* this signal is now empty */
    popFromAtCache (&channelContext_p->at.fromMuxCache[0],
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
 * Function:    initialiseSmsInput
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     Nothing
 *
 * Description: Prepares for SMS message entry, displaying prompt and
 *              reseting message variables
 *
 ****************************************************************************/

void initialiseSmsInput (const VgmuxChannelNumber entity)
{
  ChannelContext_t *channelContext_p = ptrToChannelContext (entity);
  SmsContext_t     *smsContext_p     = ptrToSmsContext     (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(channelContext_p != PNULL, entity, 0, 0);
  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  /* switch to SMS message entry mode */
  channelContext_p->dataEntryMode = DATA_ENTRY_SMS_MESSAGE;

  /* display prompt */
  vgPutNewLine (entity);
  vgPrintf     (entity, (const Char *)"> ");

  /* reset message length to zero */
  smsContext_p->smsLength = 0;

  /* determine maximum length of message */
  if ( getProfileValue (entity, PROF_CMGF) == PROF_CMGF_TEXT_MODE )
  {
    smsContext_p->maxSmsMessageLength = VG_SMS_MAX_TEXT_LENGTH;
  }
  else
  {
    if ( getProfileValue (entity, PROF_CSMS) == VG_SMS_PDU_TPDU_ONLY)
    {
      smsContext_p->maxSmsMessageLength = (smsContext_p->tpduLength * 2);
    }
    else
    {
      smsContext_p->maxSmsMessageLength = (smsContext_p->tpduLength * 2) + VG_SMS_MAX_LEN_SCA_INFO;
    }
  }

}


/****************************************************************************
 *
 * Function:    flushSmsInput
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     Nothing
 *
 * Description: Processes queued data as SMS message input
 *
 ****************************************************************************/

void flushSmsInput (const VgmuxChannelNumber entity)
{
  ChannelContext_t *channelContext_p = ptrToChannelContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(channelContext_p != PNULL, entity, 0, 0);
#endif
  if (channelContext_p->at.fromMuxCacheCount != 0)
  {
    do
    {
      processSmsInput (&channelContext_p->at.fromMuxCache[0], entity);
    }
    while ((channelContext_p->dataEntryMode == DATA_ENTRY_SMS_MESSAGE) &&
           (channelContext_p->at.fromMuxCacheCount != 0));
  }
}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

