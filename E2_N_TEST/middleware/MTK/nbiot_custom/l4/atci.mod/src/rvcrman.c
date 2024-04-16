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
 * RVCRMAN.C
 * Command response manager interface controller module
 **************************************************************************/

#define MODULE_NAME "RVCRMAN"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <gkisig.h>
#include <gkimem.h>
#include <vgmx_sig.h>
#include <rvsystem.h>
#include <rvutil.h>
#include <rvcimxut.h>
#include <rvcimxsot.h>
#include <rvcrman.h>
#include <rvprof.h>
#include <rvpdsigo.h>
#include <rvcimux.h>
#include <rvcrhand.h>
#include <rvccut.h>
#include <rvstkrnat.h>
#include <rvcmux.h>
#include <rvchman.h>
#include <rvaput.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  CirmDataInd            cirmDataInd;
  CiMuxAtDataReq         ciMuxAtDataReq;
  CiEmptyTxCacheInd      ciEmptyTxCacheInd;
};

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

#if defined (UPGRADE_RAVEN_NO_VERBOSE)
typedef struct ResultCodeInfoTag
{
  ResultCode_t enumerate; /* enumerate code */
  Int16        numeric;   /* numeric code   */
}
ResultCodeInfo_t;

#define RESULT_CODE_ENTRY(enumerate, numeric, text) \
{ enumerate, numeric }

#else
typedef struct ResultCodeInfoTag
{
  ResultCode_t enumerate; /* enumerate code */
  Int16        numeric;   /* numeric code   */
  const Char   *text;     /* text           */
}
ResultCodeInfo_t;

#define RESULT_CODE_ENTRY(enumerate, numeric, text) \
{ enumerate, numeric, (const Char*)text }

#endif

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* Result code table in the various formats */

const ResultCodeInfo_t resultCodes[] =
{
#if !defined (RVCRCME_H)
#  include <rvcrcme.h>
#endif
};

#define NUM_RESULT_CODES (sizeof (resultCodes) / sizeof (ResultCodeInfo_t))

extern RvNvramGenericData   mNvramGenericDataCache;
/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/
static void sendDelayedSignals( void);

static Boolean getResultCodeInfo (const ResultCode_t code,
                                   const ResultCodeInfo_t **codeInfo);

static void formatResultCode (const ResultCode_t code,
                                const VgmuxChannelNumber entity);
static void formatResultCodeIntermediate (const Char *resultText,
                                           const VgmuxChannelNumber entity);
static void formatCmeError (  const ResultCode_t code,
                              const VgmuxChannelNumber entity);
static void formatCmsError (  const ResultCode_t code,
                              const VgmuxChannelNumber entity);
static void vgSendStringToMux (Char *string,
                                const Int16 inputLength,
                                 const Boolean prefix,
                                  const Boolean postfix,
                                  const Boolean isUrc,
                                  const Boolean isSmsUrc,
                                   const VgmuxChannelNumber entity);

static Boolean resultCodeInterceptor (const SignalBuffer *signalBuffer);

static void initialiseCrManGenericData (void);

/***************************************************************************
 * Global Function Prototypes
 ***************************************************************************/

Boolean vgCrmanInterfaceController (const SignalBuffer *signal_p,
                                     const VgmuxChannelNumber entity);

/***************************************************************************
 * Variables
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

 /****************************************************************************
 *
 * Function:    sendDelayedSignals
 *
 * Parameters:  Nothing
 *
 * Returns:     Nothing
 *
 * Description: Send all the delayed signal because of the flow control
 *
 ****************************************************************************/
static void sendDelayedSignals( void)
{
    ChannelContext_t   *channelContext_p    = PNULL;
    VgmuxChannelNumber  entity              = 0;

    for( entity = 0; entity < CI_MAX_ENTITIES; entity++)
    {
        if( isEntityActive (entity) == TRUE)
        {
            channelContext_p = ptrToChannelContext(entity);
#if defined (ATCI_SLIM_DISABLE)
            FatalCheck( channelContext_p != PNULL, entity, 0, 0);
#endif
            /* Check if it exist a delayed signal for this channel*/
            if( channelContext_p->delayedSignalId != NON_SIGNAL)
            {
                if( getCommandId( entity) != VG_AT_NO_COMMAND)
                {
                    /* Terminate the command if the operation fails*/
                    setResultCode(  entity,
                                    vgChManContinueAction(  entity,
                                                            channelContext_p->delayedSignalId) );
                }
                else
                {
                    /* In this case, we can't do anything if the operation fails*/
                    vgChManContinueAction(  entity,
                                            channelContext_p->delayedSignalId);
                }

                /* Reset channel delayed signal*/
                channelContext_p->delayedSignalId = NON_SIGNAL;
            }
        }
    }
}

 /****************************************************************************
 *
 * Function:    getResultCodeInfo
 *
 * Parameters:  code - result code
 *
 * Returns:     In16 - index of result code record
 *
 * Description: find result code record in resultCodes table
 *
 ****************************************************************************/

static Boolean getResultCodeInfo (const ResultCode_t code,
                                   const ResultCodeInfo_t **codeInfo)
{
  Int32 resultIndex;

  Boolean found = FALSE;

  for (resultIndex = 0;
       (resultIndex < NUM_RESULT_CODES) && (found == FALSE);
        resultIndex++)
  {
    if (resultCodes[resultIndex].enumerate == code)
    {
      found = TRUE;
      *codeInfo = &resultCodes[resultIndex];
    }
  }

  return (found);
}

/*--------------------------------------------------------------------------
 *
 * Function:        formatResultCode
 *
 * Parameters:      (in)code - result code
 *
 * Returns:         (inout)outputLine - string to be filled with error code
 *
 * Description:     formats result code depending on profile parameters
 *
 *
 *-------------------------------------------------------------------------*/

static void formatResultCode (const ResultCode_t code,
                                const VgmuxChannelNumber entity)

{
  Char                   format[2]      = {0};
  const ResultCodeInfo_t *codeInfo      = PNULL;

  /* get result code information */
  if (getResultCodeInfo (code, &codeInfo) == FALSE)
  {
    FatalParam(entity, code, 0);
  }

  /* check result code is in the valid range */

  FatalAssert (code != RESULT_CODE_PROCEEDING);

  /* if verbose reporting is on use long result format (text): */
  /*   <CR><LF><verbose code><CR><LF>                          */
  /* otherwise use short result formatting (numeric):          */
  /*   <numeric code><CR>                                      */

  /* job 118304; if the code is RESULT_CODE_NULL, nothing should
     be displayed on the AT shell */
  if (code != RESULT_CODE_NULL)
  {
    if (getProfileValue (entity, PROF_VERBOSE) == REPORTING_ENABLED )
    {
#if defined (UPGRADE_RAVEN_NO_VERBOSE)
      FatalParam(entity, code, 0);
#else
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"%s", codeInfo->text);
      vgPutNewLine (entity);
#endif
    }
    else
    {
      format[0] = getProfileValue (entity, PROF_S3);
      format[1] = NULL_CHAR;

      vgPrintf (entity,
                (const Char*)"%d%s",
                codeInfo->numeric,
                format);
    }

    /* For result codes - we must always transmit to MUX regardless of
     * AT or data mode */
    vgSetCirmDataIndForceTransmit(entity);
    
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        formatResultCodeIntermediate
 *
 * Parameters:      (in)resultText_p - result code text
 *                  (in)entity       - entity
 *
 * Returns:         (inout)outputLine - string to be filled with error code
 *
 * Description:     formats intermediate result codes depending on profile
 *                  parameters
 *-------------------------------------------------------------------------*/


static void formatResultCodeIntermediate (const Char *resultText_p,
                                           const VgmuxChannelNumber entity)

{
  /* check result code is in the valid range */

  /* if verbose reporting is on use long result format (text): */
  /*   <CR><LF><verbose code><CR><LF>                          */
  /* otherwise use formatting style:                           */
  /*   <verbose code><CR><LF>                                  */

  if (getProfileValue (entity, PROF_VERBOSE) == REPORTING_ENABLED )
  {
    vgPutNewLine (entity);
  }

  vgPuts (entity, (const Char*)resultText_p);

  /* For intermediate result codes - we must always transmit to MUX regardless of
   * AT or data mode */
  vgSetCirmDataIndForceTransmit(entity);
}

/*--------------------------------------------------------------------------
 *
 * Function:        formatCmeError
 *
 * Parameters:      code - cme error code
 *
 * Returns:         outputLine - string to be filled with error code
 *
 * Description:     creates CME error string depending on cme setting format
 *                  setting
 *
 *-------------------------------------------------------------------------*/

static void formatCmeError (const ResultCode_t code,
                              const VgmuxChannelNumber entity)

{
  const ResultCodeInfo_t *codeInfo;

  /* get result code information, if not found use unknown error */
  if (getResultCodeInfo (code, &codeInfo) == FALSE)
  {
    if (getResultCodeInfo (VG_CME_UNKNOWN, &codeInfo) == FALSE)
    {
      FatalParam(entity, code, 0);
    }
  }

  switch ((VgCmeSetting)getProfileValue (entity, PROF_CME))
  {
    case VG_CME_NUMERIC: /* use numeric values */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"+CME ERROR: %d", (Int16)codeInfo->numeric);
      vgPutNewLine (entity);
      /* For result codes - we must always transmit to MUX regardless of
       * AT or data mode */
      vgSetCirmDataIndForceTransmit(entity);
      break;
    }
    case VG_CME_VERBOSE: /* use verbose values */
    {
#if defined (UPGRADE_RAVEN_NO_VERBOSE)
      FatalParam(entity, code, getProfileValue (entity, PROF_CME));
#else
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"+CME ERROR: %s", codeInfo->text);
      vgPutNewLine (entity);
      /* For result codes - we must always transmit to MUX regardless of
       * AT or data mode */
      vgSetCirmDataIndForceTransmit(entity);
#endif
      break;
    }
    case VG_CME_OFF:     /* result code disabled */
    {
      break;
    }
    default:
    {
      FatalParam(entity, code, 0);
      break;
    }
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:        formatCmsError
 *
 * Parameters:      code - cms error code
 *
 * Returns:         outputLine - string to be filled with error code
 *
 * Description:     creates CMS error string depending on cme setting format
 *                  setting
 *
 *-------------------------------------------------------------------------*/

static void formatCmsError (const ResultCode_t code,
                              const VgmuxChannelNumber entity)
{
  const ResultCodeInfo_t *codeInfo;

#ifdef FEA_NFM
   /* For NFM feature, should indicate NFM error and left time value to AP */
   if((code >= VG_CMS_ERROR_NFM_ERROR_START) &&
      (code <= VG_CMS_ERROR_NFM_ERROR_END))
   {
    switch ((VgCmeSetting)getProfileValue (entity, PROF_CME))
    {
      case VG_CME_NUMERIC: /* use numeric values */
      {
        vgPutNewLine (entity);
        vgPrintf (entity, (const Char*)"+CMS ERROR: %d", VG_NFM_ERROR_START_BASE + (code - VG_CMS_ERROR_NFM_ERROR_START));
        vgPutNewLine (entity);
        /* For result codes - we must always transmit to MUX regardless of
         * AT or data mode */
        vgSetCirmDataIndForceTransmit(entity);      
        break;
      }
      case VG_CME_VERBOSE: /* use verbose values */
      {
#if defined (UPGRADE_RAVEN_NO_VERBOSE)
        FatalParam(entity, code, getProfileValue (entity, PROF_CME));
#else
        vgPutNewLine (entity);
        vgPrintf (entity, (const Char*)"+CMS ERROR: Left time is %d seconds", VG_NFM_ERROR_START_BASE + (code - VG_CMS_ERROR_NFM_ERROR_START));
        vgPutNewLine (entity);
        /* For result codes - we must always transmit to MUX regardless of
         * AT or data mode */
        vgSetCirmDataIndForceTransmit(entity);

#endif
        break;
      }
      case VG_CME_OFF:     /* result code disabled */
      {
        break;
      }
      default:
      {
        FatalParam(entity, code, getProfileValue (entity, PROF_CME));
        break;
      }
     }
   }
   else
#endif
  {
    /* get result code information, if not found use unknown error */
    if (getResultCodeInfo (code, &codeInfo) == FALSE)
    {
      if (getResultCodeInfo (VG_CMS_ERROR_UNKNOWN, &codeInfo) == FALSE)
      {
        FatalParam(entity, code, 0);
      }
    }

  switch ((VgCmeSetting)getProfileValue (entity, PROF_CME))
  {
    case VG_CME_NUMERIC: /* use numeric values */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"+CMS ERROR: %d", (Int16)codeInfo->numeric);
      vgPutNewLine (entity);
      /* For result codes - we must always transmit to MUX regardless of
       * AT or data mode */
      vgSetCirmDataIndForceTransmit(entity);      
      break;
    }
    case VG_CME_VERBOSE: /* use verbose values */
    {
#if defined (UPGRADE_RAVEN_NO_VERBOSE)
      FatalParam(entity, code, getProfileValue (entity, PROF_CME));
#else
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"+CMS ERROR: %s", codeInfo->text);
      vgPutNewLine (entity);
      /* For result codes - we must always transmit to MUX regardless of
       * AT or data mode */
      vgSetCirmDataIndForceTransmit(entity);

#endif
      break;
    }
    case VG_CME_OFF:     /* result code disabled */
    {
      break;
    }
    default:
    {
      FatalParam(entity, code, getProfileValue (entity, PROF_CME));
      break;
    }
   }
  }
}

/*****************************************************************************
 * Function:    doAtDataCnf
 *
 * Parameters:  channelNumber - mux channel number
 *
 * Returns:     Nothing
 *
 * Description: The MUX has confirmed the receipt of AtDataReq signal.
 ****************************************************************************/

static void doAtDataCnf (const VgmuxChannelNumber entity)
{
    ChannelContext_t  *channelContext_p = ptrToChannelContext (entity);
    char              *sendString       = PNULL;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(channelContext_p != PNULL, entity, 0, 0);
#endif
    /* Cover race condition.  In case channel available but not yet active in ATCI
     * and sent something to MUX
     */
    channelContext_p->at.waitingForCnf = FALSE;

    if (isEntityActive (entity) == TRUE)
    {

        /* Now send any data which is cached */
        if (channelContext_p->at.toMuxCacheCount > 0)
        {
            /*
            * We can only have one outstanding AT_DATA_REQ to the MUX.
            */
            channelContext_p->at.waitingForCnf = TRUE;

            KiRequestMemory (sizeof (char) * (CIMUX_MAX_AT_DATA_LENGTH+16), (void **)&sendString);
            if (PNULL != sendString)
            {
              memset (sendString, 0, sizeof (char) * (CIMUX_MAX_AT_DATA_LENGTH+16));
              snprintf(sendString,
                       CIMUX_MAX_AT_DATA_LENGTH+16,
                       "send string %s",
                       (char *)&channelContext_p->at.toMuxCache[0].sig->ciMuxAtDataReq.data[0]);
              writeHslString(sendString);
              KiFreeMemory ((void **)&sendString);
            }
            KiSendSignal (MUX_TASK_ID, &channelContext_p->at.toMuxCache[0]);
            popFromAtCache( &channelContext_p->at.toMuxCache [0],
                            &channelContext_p->at.toMuxCacheCount);
        }
        else
        {
            /* Send delayed signal*/
            sendDelayedSignals();
        }
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSendStringToMux
 *
 * Parameters:      string          - pointer to string of text to be sent
 *                  inputLength     - length of string
 *                  prefix, postfix - formatting information
 *                  entity          - mux channel for signal to be sent down
 *
 * Returns:         Nothing
 *
 * Description:     Sends text to specified mux channel
 *                  Adds prefix/postfix if required
 *                  If string is too long to fit into a single signal then
 *                  multiple signals are sent
 *
 *-------------------------------------------------------------------------*/

static void vgSendStringToMux (Char *string,
                                const Int16 inputLength,
                                 const Boolean prefix,
                                  const Boolean postfix, 
                                  const Boolean isUrc,
                                  const Boolean isSmsUrc,
                                  const VgmuxChannelNumber entity)
{
  Boolean          allDataSent = TRUE;
  Int8             *data;

  Int8             prefixLength = 0, postfixLength = 0;

  Int16            index, copyLength = inputLength;
  SignalBuffer     signal = kiNullBuffer;
  CiMuxAtDataReq   *newSig_p;
  ChannelContext_t *channelContext_p;

  StkEntityGenericData_t *stkGenericContext_p = ptrToStkGenericContext ();
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(stkGenericContext_p != PNULL, entity, 0, 0);
#endif
  {
    if (prefix == TRUE)
    {
      prefixLength = 2;
    }

    if (postfix == TRUE )
    {
      postfixLength = 2;
    }

    do
    {
      signal = kiNullBuffer;

      KiCreateSignal (SIG_CIMUX_AT_DATA_REQ,
                              sizeof (CiMuxAtDataReq),
                              &signal);

      newSig_p = &signal.sig->ciMuxAtDataReq;

      data = newSig_p->data;

      newSig_p->channelNumber = entity;

      newSig_p->isUrc = isUrc;
      newSig_p->isSmsUrc = isSmsUrc;

      channelContext_p = ptrToChannelContext ((VgmuxChannelNumber)newSig_p->channelNumber);
#if defined (ATCI_SLIM_DISABLE)

      FatalCheck (channelContext_p != PNULL, entity, 0, 0);
#endif
      /* add prefix if specified by request, but also that this is the first signal
       * to be sent for that request.
       */

      if ((allDataSent == TRUE) && (prefix == TRUE))
      {
        *data++ = getProfileValue ((VgmuxChannelNumber)newSig_p->channelNumber, PROF_S3);
        *data++ = getProfileValue ((VgmuxChannelNumber)newSig_p->channelNumber, PROF_S4);
      }

      /*
       * if the length of data to be sent is smaller than the maximum data length then
       * send signal and exit procedure
       */
      if (copyLength < (CIMUX_MAX_AT_DATA_LENGTH - prefixLength - postfixLength))
      {
        allDataSent = TRUE;
        for (index = 0; index < copyLength; index++ )
        {
          *data++ = *string++;
        }
        if (postfix == TRUE)
        {
          *data++ = getProfileValue ((VgmuxChannelNumber)newSig_p->channelNumber, PROF_S3);
          *data++ = getProfileValue ((VgmuxChannelNumber)newSig_p->channelNumber, PROF_S4);
        }
        newSig_p->length = (Int32)(copyLength + prefixLength + postfixLength);
      }

      /*
       * if the data is too long for one signal send a full signal and set flag to
       * indicate more data must be sent
       */

      else
      {
        allDataSent = FALSE;

        for (index = 0; index < (CIMUX_MAX_AT_DATA_LENGTH - prefixLength - 1); index++ )
        {
          *data++ = *string++;
        }
        newSig_p->length = CIMUX_MAX_AT_DATA_LENGTH - 1;
        copyLength = (copyLength - CIMUX_MAX_AT_DATA_LENGTH) + prefixLength + 1;
        prefixLength = 0;
      }

      *data = NULL_CHAR;

      if (((stkGenericContext_p->runAtCmdState == STATE_RUNNING_COMMAND) ||
      (stkGenericContext_p->runAtCmdState == STATE_COMMAND_COMPLETED))  &&
           (entity == stkGenericContext_p->atCommandData.cmdEntity ))
      {
         vgStkAddDataToStkAtResponseBuffer((Int8)newSig_p->length, newSig_p->data);
         /* don't want to send this signal to the MUX as it is for ATCI internal use */
         KiDestroySignal (&signal);
      }
      else
      {
        if ((channelContext_p->at.waitingForCnf == TRUE) ||
           (channelContext_p->at.toMuxCacheCount > 0))
        {
          crManAddToAtCache ((VgmuxChannelNumber)newSig_p->channelNumber,
                             &channelContext_p->at.toMuxCache[0],
                             &channelContext_p->at.toMuxCacheCount,
                             &signal);
          KiDestroySignal (&signal);
        }
        else
        {
          /* check there is something to send */
          if (newSig_p->length > 0)
          {
            /*
             * We can only send one AT_DATA_REQ at a time per channel to the MUX.
             */
#if !defined (DISABLE_ATCI_RESPONSE_FC)
#if !(defined (GX2) && !defined(ENABLE_ATCI_UNIT_TEST))
            channelContext_p->at.waitingForCnf = TRUE;
#endif
#endif

#ifdef UE_SIMULATOR
            KiSendSignal (VMMI_TASK_ID, &signal);
#else
            KiSendSignal (MUX_TASK_ID, &signal);
#endif
          }
          else
          {
            KiDestroySignal (&signal);
          }
        }
      }
    }
    while (allDataSent == FALSE);
  }
  if ((stkGenericContext_p->runAtCmdState == STATE_COMMAND_COMPLETED)  &&
     (entity == stkGenericContext_p->atCommandData.cmdEntity ))
  {
#if defined(ENABLE_CIRM_DATA_IND)
    if (stkGenericContext_p->atCommandData.cirmDataIndCount == 0)
    {
#endif
      vgAfsaRunAtCommandRsp();
#if defined(ENABLE_CIRM_DATA_IND)
    }
#endif
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        resultCodeInterceptor
 *
 * Parameters:      signalBuffer - pointer to cirmDataInd signal
 *
 * Returns:         Boolean - whether the signal should be considered for
 *                            sending to the mux
 *
 * Description:     intercepts final result codes sent on the unsolicited
 *                  channel (ATD - external make call, ATA - external
 *                  answer, RING - if external task wants to know)
 *
 *-------------------------------------------------------------------------*/

static Boolean resultCodeInterceptor (const SignalBuffer *signalBuffer)
{

  CirmDataInd *sig_p = &signalBuffer->sig->cirmDataInd;
  Boolean     sendSignal = TRUE;

  FatalAssert (sig_p->classOfData   == RESULT_CODE);

  switch (sig_p->commandId)
  {
    case VG_AT_NO_COMMAND:
    {
      /* Do nothing */
      break;
    }
#if defined (FEA_MT_PDN_ACT)
    case VG_AT_CC_A:
      /*FALLTHRU*/
    case VG_AT_GP_CGANS:
    {
      /* We only intercept result codes if they are not intermediate in
       * this case since - this is to ensure that CONNECT responses are
       * produced from task initiated commands (i.e. autoanswer)....
       */
      if ((sig_p->resultCode != RESULT_CODE_INTERMEDIATE) &&
          (sig_p->resultCode != RESULT_CODE_CONNECT))
      {
        sendSignal = FALSE;
      }
      break;
    }
#endif /* FEA_MT_PDN_ACT */
        
    case VG_AT_CC_D:
    {
      /* intercept all result codes for external task calls */
      sendSignal = FALSE;

      /* external task ATD was successful, intercept final result codes */
      break;
    }
    case VG_AT_CC_H:
    {
      /*
       * This must have come from a request from the fax protocol so simply
       * ignore it.
       */
      break;
    }
    default:
    {
      /*
       * RING can be generated in the middle of any AT command - so we just ignore
       * any other AT command.
       */
      break;
    }
  }

  return (sendSignal);
}

 /****************************************************************************
 *
 * Function:    processData
 *
 * Parameters:  signal_p - incoming signal to be processed
 *              entity - mux channel
 *
 * Returns:     none
 *
 * Description: Encodes and sends data ready to be sent to the mux.
 *              Seperates by class of data.
 *              Not all incoming data is necessarily sent.
 *
 ****************************************************************************/
void vgProcessCirmData (const SignalBuffer *signalBuffer,
                        const VgmuxChannelNumber entity)
{
  CirmDataInd *sig_p = &signalBuffer->sig->cirmDataInd;
  Boolean     sendSignal   = TRUE;
  Int16       outputLength = 0;
  Boolean     isNoCarrierResultCode = FALSE;

#if defined (ENABLE_LONG_AT_CMD_RSP)
    /* TODO: Store echoString in global data */
#endif    
  Char        outputLine[CIMUX_MAX_AT_DATA_LENGTH] = {0};

  StkEntityGenericData_t       *stkGenericContext_p     = ptrToStkGenericContext ();
  CrManagerGenericContext_t    *crManGenericContext_p   = ptrToCrManagerGenericContext();

  /* size of outputLine increased in size to accommodate conversions which can
     increase string length */
  if (((stkGenericContext_p->runAtCmdState == STATE_RUNNING_COMMAND) ||
      (stkGenericContext_p->runAtCmdState == STATE_COMMAND_COMPLETED))  &&
      (entity == stkGenericContext_p->atCommandData.cmdEntity ))

  {
    WarnAssert(stkGenericContext_p->atCommandData.cirmDataIndCount > 0);
    if ( stkGenericContext_p->atCommandData.cirmDataIndCount > 0)
    {
       --stkGenericContext_p->atCommandData.cirmDataIndCount;
    }
  }
  --crManGenericContext_p->cirmDataIndCountFlowControl;

  /* Data type handler */
  switch (sig_p->classOfData)
  {
    case RESULT_CODE:
    {
      /* Override URC setting - this cannot be a URC */
      sig_p->isUrc = FALSE;
      
      if (entity != VGMUX_CHANNEL_INVALID)
      {
        GeneralContext_t  *generalContext_p = ptrToGeneralContext(entity);
#if defined (ATCI_SLIM_DISABLE)
        FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
        generalContext_p->omitFirstNewLine = FALSE;
      }

      /* check to see if signal must be intercepted */
      if (sig_p->taskInitiated == TRUE ||
          sig_p->resultCode == RESULT_CODE_RING)
      {
        sendSignal = resultCodeInterceptor (signalBuffer);
      }

      if (sendSignal == TRUE)
      {
        /* check if result codes are being suppressed */
        if (getProfileValue (entity, PROF_QUIET) == REPORTING_DISABLED )
        {
          /* result codes */
          if ((sig_p->resultCode > START_OF_RESULT_CODES) &&
              (sig_p->resultCode < END_OF_RESULT_CODES))
          {
            if (sig_p->resultCode == RESULT_CODE_INTERMEDIATE)
            {
              formatResultCodeIntermediate (sig_p->data, entity);
              vgSetCirmDataIndIsUrc(entity, FALSE);
            }

            else
            {
              isNoCarrierResultCode = (sig_p->resultCode == RESULT_CODE_NO_CARRIER);
              
              formatResultCode (sig_p->resultCode, entity);
              
              /* For NO CARRIER - we set URC flag */
              if (isNoCarrierResultCode)
              {
                vgSetCirmDataIndIsUrc(entity, TRUE);
              }
              else
              {
                /* Otherwise result code are always NOT URCs */
                vgSetCirmDataIndIsUrc(entity, FALSE);
              } 
            }
          }
          else
          {
            /* check if CME/CMS error reporting is on */
            if (getProfileValue (entity, PROF_CME) != VG_CME_OFF)
            {
              /* CME error codes */
              if ((sig_p->resultCode > START_OF_CME_ERROR_CODES) &&
                  (sig_p->resultCode < END_OF_CME_ERROR_CODES))
              {
                formatCmeError (sig_p->resultCode, entity);
                vgSetCirmDataIndIsUrc(entity, FALSE);
              }
              /* CMS error codes */
              else if ((sig_p->resultCode > START_OF_CMS_ERROR_CODES) &&
                       (sig_p->resultCode < END_OF_CMS_ERROR_CODES))
              {
                formatCmsError (sig_p->resultCode, entity);
                vgSetCirmDataIndIsUrc(entity, FALSE);
              }
              else /* invalid result code */
              {
                FatalParam(entity, sig_p->resultCode, 0);
              }
            }
            else /* no CME/CMS error reporting so display ERROR result code */
            {
              formatResultCode (RESULT_CODE_ERROR, entity);
              vgSetCirmDataIndIsUrc(entity, FALSE);
            }
          }

          /* We don't pass string straight to MUX now - we buffer it in the
           * appropriate entity stream.  Response will come back as data.
           */
          sendSignal = FALSE;
        }
        else /* result codes are supressed */
        {
          sendSignal = FALSE;
        }
      }
      break;
    }
    case DATA:
    {
      FatalCheck(sig_p->length <= CIMUX_MAX_AT_DATA_LENGTH, sig_p->length, entity, sig_p->classOfData);
      memcpy (outputLine, sig_p->data, sig_p->length);
      outputLength = sig_p->length;
      break;
    }
    case ECHO_DATA:
    {
        
      ChannelContext_t   *channelContext_p = ptrToChannelContext(entity);  
      /* Override URC setting - this cannot be a URC */
      sig_p->isUrc = FALSE;
            
      if (((getProfileValue (entity, PROF_ECHO) == REPORTING_ENABLED)&&(!channelContext_p->actDataIndNoReply))
      && !(((stkGenericContext_p->runAtCmdState == STATE_RUNNING_COMMAND)||
       (stkGenericContext_p->runAtCmdState == STATE_COMMAND_COMPLETED))&&
           (entity == stkGenericContext_p->atCommandData.cmdEntity )))
      /* don't want to pass on echo characters if this is for processing of
       * internal STK RUN AT COMMAND proactive command */
      {
        FatalCheck(sig_p->length <= CIMUX_MAX_AT_DATA_LENGTH, sig_p->length, entity, sig_p->classOfData);
        memcpy (outputLine, sig_p->data, sig_p->length);
        outputLength = sig_p->length;
      }
      else /* not echoing characters */
      {
        sendSignal = FALSE;
      }
      break;
    }
    case LINE_DATA:
    {
      /* Override URC setting - this cannot be a URC */
      sig_p->isUrc = FALSE;

#ifdef ENABLE_AP_BRIDGE_FEATURE
      if ((getCommandId(entity) > (VG_AT_AP_BRIDGE_BASE - 1))
           && (getCommandId(entity) < (VG_AT_AP_BRIDGE_END + 1)))
      {
        /*The command is for AP Bridge.*/
        establishDataSessionForApBridge(entity);
      }
      else if((vgApbGetDataModeContext(entity) != PNULL)
               && (sig_p->pendingConnection == APBRIDGE_CONNECTION))
      {
        ApBridgeDataModeContext_t* apBridgeDataModeContext_p = vgApbGetDataModeContext(entity);
        setCommandId(entity, apBridgeDataModeContext_p->dataModeCmdId);
        vgCiStartTimer (TIMER_TYPE_APB_DATA_MODE_REACTIVE, entity);
      }
      else
#endif
      {
        /* assign data channel and update DCD status if required */
        establishDataSession (entity, sig_p->pendingConnection);
      }
      sendSignal = FALSE;
      break;
    }
    default:
    {
      /* unexpected class of data encountered */
      FatalParam (sig_p->classOfData, sig_p->channelNumber, sig_p->resultCode);
      sendSignal = FALSE;
      break;
    }
  }

  if (sendSignal == TRUE)
  {
    /* send string on to mux */
    vgSendStringToMux (outputLine,
                       outputLength,
                       sig_p->prefix,
                       sig_p->postfix,
                       sig_p->isUrc,
                       sig_p->isSmsUrc,
                       sig_p->channelNumber);
  }

  /* check if line data needs to be sent */
  if ((sig_p->classOfData       != LINE_DATA) &&
      (sig_p->pendingConnection != CONNECTION_TERMINATOR))
  {
    /* flush any previous data pending */
    vgFlushBuffer (entity);

    /* send line data */
    sendLineDataToCrm (sig_p->pendingConnection, sig_p->channelNumber);
  }

#if defined(ENABLE_CIRM_DATA_IND)
  /* If all the data are sent to mux but we will not wait/receive CNF signal,
  *  immediatly send the delayed signals to BL.*/
  if( (crManGenericContext_p->cirmDataIndCountFlowControl == 0) &&
      (isChannelWaitForMuxCnf() == FALSE) )
  {
    sendDelayedSignals();
  }
#endif

}


 /****************************************************************************
 *
 * Function:    initialiseCrMan
 *
 * Parameters:  none
 *
 * Returns:     nothing
 *
 * Description: Initialises the Command Response Manager entity specific data
 *
 ****************************************************************************/

void initialiseCrMan (const VgmuxChannelNumber entity)
{
  CrManagerContext_t  *crManagerContext_p = ptrToCrManagerContext(entity);

  setCommandId      (entity, mNvramGenericDataCache.entityNvData[entity].currentAtCommand);
  setCommandName    (entity, (Char *) "");
  setConnectionType (entity, CONNECTION_TERMINATOR);
  setTaskInitiated  (entity, FALSE);

  /* set result code directly (not using setResultCode) to avoid assertion
   * when trying to set a result code when the entity is not running
   * N.B. do not do this anywhere else! */

  crManagerContext_p->resultCode = RESULT_CODE_PROCEEDING;
  crManagerContext_p->crOutputStreamCtrl.signal = kiNullBuffer;
  crManagerContext_p->crChSetConversionBuffer_p = PNULL;
  crManagerContext_p->crChSetOutputBuffer_p = PNULL;

#if defined (FEA_PHONEBOOK)
  crManagerContext_p->crAlphaStringBuffer_p = PNULL;
  crManagerContext_p->crAlphaSearchBuffer_p = PNULL;
#endif /* FEA_PHONEBOOK */

}

 /*--------------------------------------------------------------------------
 *
 * Function:    initialiseCrManGenericData
 *
 * Parameters:  none
 *
 * Returns:     nothing
 *
 * Description: Initialise all the CR Manager generic context data.
 *
 *-------------------------------------------------------------------------*/

static void initialiseCrManGenericData (void)
{
  Int8 i;
  CrManagerGenericContext_t   *crManagerGenericContext     = ptrToCrManagerGenericContext();

  memset( crManagerGenericContext->vgPrintfBuffer, 0, VG_PRINTF_CONV_BUFFER + NULL_TERMINATOR_LENGTH);
  crManagerGenericContext->cirmDataIndCountFlowControl = 0;

  /* Initialise the RAM stores */
  for (i=0; i<ATCI_MAX_NUM_CONV_BUFF; i++)
  {
    crManagerGenericContext->inUseConvBuffDataItem[i].inUse = FALSE;
  }
  for (i=0; i<ATCI_MAX_NUM_OUTPUT_BUFF; i++)
  {
    crManagerGenericContext->inUseOutputBuffDataItem[i].inUse = FALSE;
  }
#if defined (FEA_PHONEBOOK)  
  for (i=0; i<ATCI_MAX_NUM_ALPHA_BUFF; i++)
  {
    crManagerGenericContext->inUseAlphaBuffDataItem[i].inUse = FALSE;
  }
  for (i=0; i<ATCI_MAX_NUM_ALPHA_SEARCH_BUFF; i++)
  {
    crManagerGenericContext->inUseAlphaSearchBuffDataItem[i].inUse = FALSE;
  }
#endif /* FEA_PHONEBOOK */

#if defined (ATCI_ENABLE_DYN_AT_BUFF)
  for (i=0; i<ATCI_MAX_NUM_AT_CMD_BUFFERS; i++)
  {
    crManagerGenericContext->atCmdBufferList[i].inUse = FALSE;
    memset (&crManagerGenericContext->atCmdBufferList[i].charBuff[0],
          NULL_CHAR,
           COMMAND_LINE_SIZE + NULL_TERMINATOR_LENGTH);
  }
#endif
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

 /****************************************************************************
 *
 * Function:    vgGetVerboseResultString
 *
 * Parameters:  code - the index for the verbose string
 *
 * Returns:     Char*
 *
 * Description: Given a result code, return the verbose string
 *
 ****************************************************************************/

Char* vgGetVerboseResultString ( const ResultCode_t code )
{
  const ResultCodeInfo_t *codeInfo = PNULL;

  if (getResultCodeInfo (code, &codeInfo) == FALSE)
  {
    if (getResultCodeInfo (VG_CME_UNKNOWN, &codeInfo) == FALSE)
    {
      FatalParam(code, 0, 0);
    }
  }

#if defined (UPGRADE_RAVEN_NO_VERBOSE)
  FatalParam(code, 0, 0);
  return (PNULL);
#else
  return ((Char*)codeInfo->text);
#endif
}

 /****************************************************************************
 *
 * Function:    vgGetNumericResultString
 *
 * Parameters:  code - the index for the verbose string
 *              txtString - String in to which to put the resultant text.
 *              This pointer also passed as the return value.  Must be
 *              size NUMERIC_RESULT_STRING_LENGTH.
 *
 * Returns:     Char*
 *
 * Description: Given a result code, return the numeric string
 *
 ****************************************************************************/

Char* vgGetNumericResultString ( const ResultCode_t code,
                                 Char  *txtString )
{
  const  ResultCodeInfo_t *codeInfo = PNULL;

  /*
   * Set to string to all 0's to be safe initially.
   */
  memset ((char *)txtString, NULL_CHAR, NUMERIC_RESULT_STRING_LENGTH);

  if (getResultCodeInfo (code, &codeInfo) == FALSE)
  {
    if (getResultCodeInfo (VG_CME_UNKNOWN, &codeInfo) == FALSE)
    {
      FatalParam(code, 0, 0);
    }
  }
#if defined (ATCI_SLIM_DISABLE)
  if ( code == RESULT_CODE_FCERROR )
  {
    strncpy ((char *)txtString, (char *)"F4", NUMERIC_RESULT_STRING_LENGTH);
  }
#endif
  snprintf ((char*)txtString, NUMERIC_RESULT_STRING_LENGTH, "%d", codeInfo->numeric);

  return (txtString);
}

/*****************************************************************************
 *
 * Function:    vgPrintNum
 *
 * Parameters:  (In)    num   - number to be appended
 *              (InOut) out_p - output string to be printed
 *
 * Returns:     Int16 - length of input string representation of num
 *
 * Description: Concatenates string representing num onto out
 *
 *****************************************************************************/

Char *vgPrintNum (Char *out_p, Int16 num)
{
  Int16 temp,
        i = 1;

  while (num >= (i * 10))
  {
    i *= 10;
  }

  do
  {
    temp = num / i;
    *out_p++ = ((Char)temp) + '0';
    num = num - (temp * i);
    i /= 10;
  } while (i > 0);

  *out_p = (Char)NULL_CHAR;

  return (out_p);
}

 /****************************************************************************
 *
 * Function:    vgCrmanInterfaceController
 *
 * Parameters:  signal_p - incoming signal to be processed
 *              entity   - mux channel number
 *
 * Returns:     Boolean, TRUE if signal for crman only, else FALSE
 *
 * Description: Determines action for recieved signals.
 *              Signals for initialisation, data, and flow control are handled
 *
 ****************************************************************************/

Boolean vgCrmanInterfaceController (const SignalBuffer *signal_p,
                                     const VgmuxChannelNumber entity)
{

  Boolean accepted = FALSE;

  /* Signal Handler */

  switch (*signal_p->type)
  {
    case SIG_INITIALISE:
    {
      initialiseCrManGenericData();
      break;
    }
    case SIG_CIMUX_CHANNEL_ENABLE_IND:
    {
      initialiseCrMan (entity);
      break;
    }
#if defined(ENABLE_CIRM_DATA_IND)
    case SIG_CIRM_DATA_IND:
    {
      vgProcessCirmData (signal_p, entity);
      accepted = TRUE;
      break;
    }
#endif
    case SIG_CIMUX_AT_DATA_CNF:
    {
      doAtDataCnf (entity);
      /* As we now wait for SIG_CIMUX_AT_DATA_CNF for the AT*MNVMGET command we do not set
       * accepted here anymore so that it can also be handled in the GNSS.
       * accepted = TRUE;
       */
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

/*****************************************************************************
 * Function:    sendTSmsInfo
 *
 * Parameters:  ResultCode_t resultCode - result to display
 *
 * Returns:     Nothing
 *
 * Description: Display the *MSMSINFO unsolicited result code. The format
 *              of this depends on AT+CMEE and it replaces the use of
 *              unsolicited CMS Errors
 *
 ****************************************************************************/

void sendTSmsInfo(ResultCode_t resultCode)
{
  const ResultCodeInfo_t *codeInfo_p = PNULL;
  VgmuxChannelNumber profileEntity =  0;

  for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
  {
    if ((isEntityActive(profileEntity)) &&
        (getProfileValueBit (profileEntity,
                             PROF_MUNSOL,
                             PROF_BIT_SMSINFO) == REPORTING_ENABLED))
    {
      /* get result code information, if not found use unknown error */
      if (!getResultCodeInfo (resultCode, &codeInfo_p))
      {
        if (!getResultCodeInfo (VG_CMS_ERROR_UNKNOWN, &codeInfo_p))
        {
          FatalParam(resultCode, 0, 0);
        }
      }

      switch ((VgCmeSetting) getProfileValue (profileEntity, PROF_CME))
      {
        case VG_CME_NUMERIC: /* use numeric values */
          vgPutNewLine (profileEntity);
          vgPrintf (profileEntity,
                    (const Char*) "*MSMSINFO: %d",
                    (Int16) codeInfo_p->numeric);
          vgPutNewLine (profileEntity);
          vgFlushBuffer (profileEntity);
          break;

        case VG_CME_VERBOSE: /* use verbose values */
#if defined (UPGRADE_RAVEN_NO_VERBOSE)
          FatalParam(profileEntity, resultCode, getProfileValue (profileEntity, PROF_CME));
#else
          vgPutNewLine (profileEntity);
          vgPrintf (profileEntity,
                    (const Char*) "*MSMSINFO: \"%s\"",
                    codeInfo_p->text);
          vgPutNewLine (profileEntity);
          vgFlushBuffer (profileEntity);
#endif
          break;

        case VG_CME_OFF:     /* result code disabled */
          break;

        default:
          FatalParam(profileEntity, resultCode, getProfileValue (profileEntity, PROF_CME));
          break;
      }
    }
  }
}

 /****************************************************************************
 *
 * Function:    isChannelWaitForMuxCnf
 *
 * Parameters:  Nothing
 *
 * Returns:     Whether a channel wait for a CNF signal from the MUX
 *
 * Description: Test if it exists a channel that wait for a CNF from the MUX
 *
 ****************************************************************************/
Boolean isChannelWaitForMuxCnf( void)
{
    ChannelContext_t   *channelContext_p    = PNULL;
    VgmuxChannelNumber  entity              = 0;
    Boolean             res                 = FALSE;

    for(    entity = 0;
            (   (entity < CI_MAX_ENTITIES) &&
                (res == FALSE) );
            entity++)
    {
        if( isEntityActive (entity) == TRUE)
        {
            channelContext_p = ptrToChannelContext(entity);
#if defined (ATCI_SLIM_DISABLE)

            FatalCheck( channelContext_p != PNULL, entity, 0, 0);
#endif
            /* Check if it exist a delayed signal for this channel*/
            if( channelContext_p->at.waitingForCnf == TRUE)
            {
               res = TRUE;
            }
        }
    }

    return res;
}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */


