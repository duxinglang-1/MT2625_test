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
 * SMS Output signals.
 *
 ***************************************************************************
 *
 * Notes
 * -----
 *
 * All functions return a result code either:
 *
 * RESULT_CODE_PROCEEDING - the signal was sent successfully
 *
 * RV_CME_UNABLE_TO_GET_CONTROL - signal could not be sent because
 *                                could not get control of BL.
 **************************************************************************/

#define MODULE_NAME "RVMSSIGO"


/***************************************************************************
 * Include Files
 ***************************************************************************/
#include <system.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvmssigo.h>
#include <rvomut.h>
#include <rvmsut.h>
#include <cici_sig.h>
#include <rvchman.h>
#include <rvmsdata.h>
#include <rvcfg.h>
#include <rvomtime.h>
#include <simdec.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  ApexSmDeleteReq       apexSmDeleteReq;
  ApexSmReadReq         apexSmReadReq;
  ApexSmReadSmsrReq     apexSmReadSmsrReq;
  ApexSmReadSmspReq     apexSmReadSmspReq;
  ApexSmWriteSmspReq    apexSmWriteSmspReq;
  ApexSmSendFromSimReq  apexSmSendFromSimReq;
  ApexSmStatusReq       apexSmStatusReq;
  ApexSmSendReq         apexSmSendReq;
  ApexSmSendMoreReq     apexSmSendMoreReq;
  ApexSmStoreReq        apexSmStoreReq;
  ApexSmCommandReq      apexSmCommandReq;
};

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/


/*--------------------------------------------------------------------------
 *
 * Function:        createSignalApexSendSmsTpdu
 *
 * Parameters:      entity - SMS context entity number
 *                  sig - pointer to send req structure
 *
 * Returns:         Nothing
 *
 * Description:     Create SIG_APEX_SM_SEND_REQ signal ready to send
 *                  to the background layer.
 *
 *                  Used by vgSmsSigOutApexSendSmsTpdu function.
 *
 *-------------------------------------------------------------------------*/
static void createSignalApexSendSmsTpdu (const VgmuxChannelNumber entity, const TsSubmitReq *sig)
{
    SmsContext_t   *smsContext_p = ptrToSmsContext (entity);
    ApexSmSendReq  *request_p;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    smsContext_p->sigApexSmSendReq = kiNullBuffer;

    KiCreateZeroSignal( SIG_APEX_SM_SEND_REQ,
                        sizeof (ApexSmSendReq),
                        &(smsContext_p->sigApexSmSendReq));

    request_p = (ApexSmSendReq *)   (smsContext_p->sigApexSmSendReq).sig;

    request_p->taskId                   = VG_CI_TASK_ID;
    request_p->commandRef               = 0; /* unused */
    request_p->statusReportReq          = sig->statusReportReq;
    request_p->replyPath                = sig->replyPath;
    request_p->scAddr                   = sig->scAddr;
    request_p->smeAddr                  = sig->smeAddr;
    request_p->protocolId               = sig->smsProtocolId;
    request_p->smsDataCodingScheme      = sig->smsDataCodingScheme;
    request_p->useRawDcs                = sig->useRawDcs;
    request_p->rawDcs                   = sig->rawDcs;
    request_p->validityPeriodFormat     = sig->validityPeriodFormat;
    request_p->validityPeriodAsValue    = sig->validityPeriodAsValue;
    request_p->shortMsgLen              = sig->shortMsgLen;
    request_p->rejectDuplicates         = sig->rejectDuplicates;
    request_p->userDataHeaderPresent    = sig->userDataHeaderPresent;
    request_p->concatSmsPresent         = FALSE;
    /*job100892 Enable FDN checking for SMS sending*/
    request_p->doFdnCheck               = TRUE;

    memcpy( request_p->validityPeriodAsTime, 
            sig->validityPeriodAsTime, 
            sizeof(SmsTimeStamp));
    memcpy( request_p->shortMsgData, 
            sig->shortMsgData, 
            sig->shortMsgLen);
}



/*--------------------------------------------------------------------------
 *
 * Function:        createSignalApexSendSmsNormal
 *
 * Parameters:      entity - SMS context entity number
 *                  sig - pointer to send req structure
 *
 * Returns:         Nothing
 *
 * Description:     Create SIG_APEX_SM_SEND_REQ signal ready to send
 *                  to the background layer.
 *
 *                  Used by vgSmsSigOutApexSendSmsNormal function.
 *
 *-------------------------------------------------------------------------*/
static void createSignalApexSendSmsNormal(const VgmuxChannelNumber entity, const CiapSms *sig)
{
  SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext ();
  SmsContext_t    *smsContext_p = ptrToSmsContext (entity);
  ApexSmSendReq   *request_p;
  /* job115421: remove compile switch */
  Boolean         rejectDuplicates  = (Boolean)DECODE_RD ( smsCommonContext_p->firstOctet),
                  statusReportReq   = (Boolean)DECODE_SRR( smsCommonContext_p->firstOctet),
                  userDataHeader    = (Boolean)DECODE_UDH( smsCommonContext_p->firstOctet),
                  replyPath         = (Boolean)DECODE_RP ( smsCommonContext_p->firstOctet);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  smsContext_p->sigApexSmSendReq = kiNullBuffer;

  KiCreateZeroSignal (SIG_APEX_SM_SEND_REQ,
                       sizeof (ApexSmSendReq),
                        &(smsContext_p->sigApexSmSendReq));

  request_p = (ApexSmSendReq *)(smsContext_p->sigApexSmSendReq).sig;
  request_p->taskId          = VG_CI_TASK_ID;

  /* job115421: remove compile switch */
  request_p->statusReportReq       = statusReportReq;
  request_p->replyPath             = replyPath;
  request_p->rejectDuplicates      = rejectDuplicates;
  request_p->userDataHeaderPresent = userDataHeader;

  memcpy (&request_p->scAddr,  &sig->sca,  sizeof(SmsAddress));
  memcpy (&request_p->smeAddr, &sig->dest, sizeof(SmsAddress));

  request_p->protocolId.protocolMeaning = smsCommonContext_p->protocolId.protocolMeaning;
  request_p->protocolId.protocolId      = smsCommonContext_p->protocolId.protocolId;

  request_p->smsDataCodingScheme.msgCoding   = smsCommonContext_p->dataCodingScheme.msgCoding;
  request_p->smsDataCodingScheme.msgClass    = smsCommonContext_p->dataCodingScheme.msgClass;

  /* see 23.038 section 4 for details on 01xx messages and auto-deletion group */
  request_p->smsDataCodingScheme.markedForAutomaticDeletion = 
    smsCommonContext_p->dataCodingScheme.markedForAutomaticDeletion;
  request_p->useRawDcs = FALSE;
  request_p->rawDcs = 0;
  vgSmsGetValidityPeriod (&request_p->validityPeriodFormat,
                          &request_p->validityPeriodAsValue,
                           request_p->validityPeriodAsTime);


  request_p->shortMsgLen = (Int8)(sig->msgLength);
  memcpy (request_p->shortMsgData, sig->message, sig->msgLength);

  request_p->concatSmsPresent = sig->isConcat;

  if ( TRUE == sig->isConcat)
  {
    request_p->concatSmsRefNum  = sig->concatRef;
    request_p->concatSmsLength  = (Int8)sig->seqLength;
    request_p->concatSmsSeqNum  = sig->seqNumber;
  }

  /*job100892 Enable FDN checking on SMS sending*/
  request_p->doFdnCheck = TRUE;
}


/*--------------------------------------------------------------------------
 *
 * Function:        createApexStoreSmsTpdu
 *
 * Parameters:      entity - SMS context entity number
 *                  sig - pointer to send req structure
 *
 * Returns:         Nothing
 *
 * Description:     Create SIG_APEX_SM_STORE_REQ signal ready to send
 *                  to the background layer.
 *
 *                  Used by vgSmsSigOutApexStoreSmsTpdu function.
 *
 *-------------------------------------------------------------------------*/
static void createApexStoreSmsTpdu(const VgmuxChannelNumber entity,
                                   SimSmTpduType            tpduType,
                                   const SimSmTpdu          *sig)
{
    ApexSmStoreReq   *request_p;
    SmsContext_t     *smsContext_p = ptrToSmsContext (entity);

    FatalCheck( (tpduType == SIM_SMTPDU_DELIVER) || (tpduType == SIM_SMTPDU_SUBMIT), (Int8)tpduType, 0, 0);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    smsContext_p->sigApexSmStoreReq = kiNullBuffer;

    KiCreateZeroSignal( SIG_APEX_SM_STORE_REQ,
                        sizeof (ApexSmStoreReq),
                        &(smsContext_p->sigApexSmStoreReq));

    request_p = (ApexSmStoreReq *) (smsContext_p->sigApexSmStoreReq).sig;

    request_p->taskId       = VG_CI_TASK_ID;
    request_p->commandRef   = 0; /* unused */
    request_p->recordNumber = 0;
    request_p->writeType    = SM_WRITE_FREE;
    request_p->type         = tpduType;
    request_p->tpdu         = *sig;
    request_p->smStatus     = smsContext_p->smStatus;

    if( vgSmsSetDefaultSmStatus( tpduType, &request_p->smStatus) == FALSE)
    {
        FatalParam( tpduType, request_p->smStatus, 0);
    }

    if( (request_p->smStatus == SIM_SMREC_RECEIVED_READ) ||
        (request_p->smStatus == SIM_SMREC_ORIGINATED_SENT))
    {
        request_p->isReadOrSent = TRUE;
    }
    else
    {
        request_p->isReadOrSent = FALSE;
    }

    request_p->concatSmsPresent = FALSE;
}




/*--------------------------------------------------------------------------
 *
 * Function:        createApexStoreSmsNormal
 *
 * Parameters:      entity - SMS context entity number
 *                  sig - pointer to send req structure
 *
 * Returns:         Nothing
 *
 * Description:     Create SIG_APEX_SM_STORE_REQ signal ready to send
 *                  to the background layer.
 *
 *                  Used by vgSmsSigOutApexStoreSmsNormal function.
 *
 * Notes from apex_sms.doc:
 *
 * "To store a new message the first segment is stored with a writeType
 *  set to SM_WRITE_FREE and record number set to zero. Subsequent
 *  segments are stored with a writeType of SM_WRITE_FREE and record
 *  number set to the location of the first segment."
 *-------------------------------------------------------------------------*/
static void createApexStoreSmsNormal(const VgmuxChannelNumber entity,
                                     const CiapSms *sig,
                                     Int8 recordNumber)
{
    SmsContext_t       *smsContext_p = ptrToSmsContext (entity);
    ApexSmStoreReq     *request_p;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    smsContext_p->sigApexSmStoreReq = kiNullBuffer;

    KiCreateZeroSignal( SIG_APEX_SM_STORE_REQ,
                        sizeof (ApexSmStoreReq),
                        &(smsContext_p->sigApexSmStoreReq));

    request_p = (ApexSmStoreReq *) (smsContext_p->sigApexSmStoreReq).sig;

    request_p->taskId       = VG_CI_TASK_ID;
    request_p->recordNumber = recordNumber;
    request_p->commandRef   = 0; /*unused*/
    request_p->writeType    = SM_WRITE_FREE;
    request_p->smStatus     = sig->status;
    if( (request_p->smStatus == SIM_SMREC_RECEIVED_READ) ||
        (request_p->smStatus == SIM_SMREC_ORIGINATED_SENT))
    {
        request_p->isReadOrSent = TRUE;
    }
    else
    {
        request_p->isReadOrSent = FALSE;
    }

    request_p->concatSmsPresent = sig->isConcat;

    if ( TRUE == sig->isConcat)
    {
        request_p->concatSmsRefNum  = sig->concatRef;
        request_p->concatSmsLength  = (Int8)sig->seqLength;
        request_p->concatSmsSeqNum  = sig->seqNumber;
    }

    vgSmsUtilCreatePduFromText( entity, sig, &request_p->type, &request_p->tpdu);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmCommandReq
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_COMMAND_REQ to the background layer
 *                  indirectly; called by the change control manager.
 *
 *-------------------------------------------------------------------------*/
static void createSigApexSmCommandReqNormal(const VgmuxChannelNumber entity, const CiapSms* sms)
{
  SmsContext_t      *smsContext_p = ptrToSmsContext (entity);
  ApexSmCommandReq  *request_p;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  smsContext_p->sigApexSmCommandReq = kiNullBuffer;

  KiCreateZeroSignal (SIG_APEX_SM_COMMAND_REQ,
                       sizeof (ApexSmCommandReq),
                        &(smsContext_p->sigApexSmCommandReq) );

  request_p = (ApexSmCommandReq *) (smsContext_p->sigApexSmCommandReq).sig;

  memset(request_p, 0, sizeof(ApexSmCommandReq));
  request_p->taskId     = VG_CI_TASK_ID;

  memcpy(&request_p->protocolId, &smsContext_p->command.pid, sizeof(SmsProtocolId));
  request_p->commandType = smsContext_p->command.commandType;
  request_p->msgNum      = smsContext_p->command.msgNum;
  request_p->cmdDataLen  = sms->msgLength;
  memcpy(request_p->cmdData,  sms->message, sms->msgLength);
  memcpy(&request_p->smeAddr, &sms->dest,   sizeof(SmsAddress));
  memcpy(&request_p->scAddr,  &sms->sca,    sizeof(SmsAddress));
  /* Set the SRR flag - resident in Bit 5 of the firstOctet byte */
  request_p->statusReportReq = (Boolean)DECODE_SRR(smsContext_p->command.firstOctet);
  /* SC Address must be set or command will result in a PS ERROR */
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmCommandReqTpdu
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_COMMAND_REQ to the background layer,
 *                  indirectly; called by the change control manager.
 *                  Built from the SMS TPDU in PDU Mode.
 *
 *-------------------------------------------------------------------------*/
static void createSigApexSmCommandReqTpdu (const VgmuxChannelNumber entity, const TsCommandReq *sig)
{
  SmsContext_t      *smsContext_p = ptrToSmsContext (entity);
  ApexSmCommandReq  *request_p;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  smsContext_p->sigApexSmCommandReq = kiNullBuffer;

  KiCreateZeroSignal (SIG_APEX_SM_COMMAND_REQ,
                       sizeof (ApexSmCommandReq),
                        &(smsContext_p->sigApexSmCommandReq) );

  request_p = (ApexSmCommandReq *) smsContext_p->sigApexSmCommandReq.sig;

  memset (request_p, 0, sizeof(ApexSmCommandReq));
  request_p->taskId     = VG_CI_TASK_ID;

  memcpy (&request_p->protocolId, &sig->smsProtocolId, sizeof(SmsProtocolId));
  request_p->commandType = sig->commandType;
  request_p->msgNum      = sig->msgNum;
  request_p->cmdDataLen  = sig->cmdDataLen;
  memcpy (request_p->cmdData, sig->cmdData, sig->cmdDataLen);
  memcpy (&request_p->smeAddr, &sig->smeAddr, sizeof(SmsAddress));
  memcpy (&request_p->scAddr,  &sig->scAddr,  sizeof(SmsAddress));
  request_p->statusReportReq = sig->statusReportReq;
}

/*--------------------------------------------------------------------------
 *
 * Function:        createSigApexSmDeliveryWithReportRspTpdu
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_DELIVERY_WITH_REPORT_RSP to the background layer,
 *                  indirectly; called by the change control manager.
 *                  Built from the SMS TPDU in PDU Mode.
 *
 *-------------------------------------------------------------------------*/
static void createSigApexSmDeliveryWithReportRspTpdu(   const VgmuxChannelNumber entity, 
                                                        const TsDeliverReportReq *sig)
{
    SmsContext_t                 *smsContext_p = ptrToSmsContext (entity);
    ApexSmDeliveryWithReportRsp  *request_p;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    smsContext_p->sigApexSmDeliveryWithReportRsp = kiNullBuffer;

    KiCreateZeroSignal( SIG_APEX_SM_DELIVERY_WITH_REPORT_RSP,
                        sizeof (ApexSmDeliveryWithReportRsp),
                        &(smsContext_p->sigApexSmDeliveryWithReportRsp) );

    request_p = (ApexSmDeliveryWithReportRsp *) smsContext_p->sigApexSmDeliveryWithReportRsp.sig;

    memset( request_p, 0, sizeof(ApexSmDeliveryWithReportRsp));
    request_p->taskId     = VG_CI_TASK_ID;

    request_p->rawDcsValue = sig->rawDcsValue;
    request_p->shortMsgId = sig->shortMsgId;
    request_p->smsDataCodingScheme = sig->smsDataCodingScheme;
    request_p->smsDataCodingSchemePresent = sig->smsDataCodingSchemePresent;
    request_p->smsProtocolId = sig->smsProtocolId;
    request_p->smsProtocolIdPresent = sig->smsProtocolIdPresent;
    request_p->tpFailureCause = sig->tpFailureCause;
    request_p->statusOfReport = sig->statusOfReport;
    request_p->udlPresent = sig->udlPresent;
    request_p->userDataHeaderPresent = sig->userDataHeaderPresent;

    request_p->userDataLength = sig->userDataLength;
    memcpy( &request_p->userData[0],
            &sig->userData[0],
            SMS_DELIVER_REPORT_MAX_USER_DATA_LENGTH * sizeof(Int8));
}


/***************************************************************************
 * Global Functions
 ***************************************************************************/

#if defined (FEA_PHONEBOOK)
/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsSigOutApexLmGetAlphaReq
 *
 * Parameters:  entity - index to the SMS context to use
 *              callId - call id to use (VG_SMS_LMGETALPHA_COMMAND_CALL_ID
 *                       or VG_SMS_LMGETALPHA_UNSOLICITED_CALL_ID)
 *              DialledBcdNum - the telephone number to lookup.
 *
 * Returns:     result of operation
 *              (RESULT_CODE_PROCEEDING if ok)
 *
 * Description: Sends signal to lookup a phonebook entry, to convert a
 *              telephone number to a name.
 *
 *              See the following functions for more information:
 *
 *              vgmsut.c::vgSmsUtilConvertSmsAddressToDialledBcdNum()
 *              vgmssigi.c::vgSmsSigInApexLmGetAlphaCnf()
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsSigOutApexLmGetAlphaReq(const VgmuxChannelNumber entity, Int16 callId, const DialledBcdNum *srcDialledNum)
{
  GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);
  VgAlphaLookup    *vgAlphaLookup_p = PNULL;
  ResultCode_t     resultCode = RESULT_CODE_ERROR;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  FatalAssert( (VG_SMS_LMGETALPHA_COMMAND_CALL_ID == callId) ||
               (VG_SMS_LMGETALPHA_UNSOLICITED_CALL_ID == callId) );

  vgAlphaLookup_p = &generalContext_p->vgAlphaLookup[callId-1];

  vgAlphaLookup_p->dialledBcdNum = *srcDialledNum;
  vgAlphaLookup_p->active = TRUE;

  resultCode = vgChManContinueAction(entity, SIG_APEX_LM_GET_ALPHA_REQ);

  /* Sig out function should set the active status to FALSE
     after it as sent the signal */
  if ((RESULT_CODE_PROCEEDING == resultCode) && (TRUE == vgAlphaLookup_p->active))
  {
    /* didn't send GET_ALPHA_REQ */
    WarnParam(entity, 0, 0);
    
    resultCode = RESULT_CODE_ERROR;
  }

  return resultCode;
}
#endif /* FEA_PHONEBOOK */

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexDeleteSms
 *
 * Parameters:      entity - SMS context entity number
 *                  idx     - index of SMS to delet
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_DELETE_REQ to the background layer
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSmsSigOutApexDeleteSms(const VgmuxChannelNumber entity, Int8 idx)
{
  SmsContext_t     *smsContext_p = ptrToSmsContext     (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  smsContext_p->cmgdRecordNum = idx;
  return (vgChManContinueAction(entity, SIG_APEX_SM_DELETE_REQ));
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexDeleteSmsr
 *
 * Parameters:      entity - SMS SR context entity number
 *                  idx     - index of SMS to delet
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_DELETE_SMSR_REQ to the background layer
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSmsSigOutApexDeleteSmsr(const VgmuxChannelNumber entity, Int8 idx)
{
  SmsContext_t     *smsContext_p = ptrToSmsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  smsContext_p->cmgdRecordNum = idx;
  return (vgChManContinueAction(entity, SIG_APEX_SM_DELETE_SMSR_REQ));
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmDeleteReq
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_DELETE_REQ to the background layer
 *                  Called by the change control manager in vgchman.c
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmDeleteReq(const VgmuxChannelNumber entity)
{
  SignalBuffer      sigBuff = kiNullBuffer;
  ApexSmDeleteReq  *request_p;
  SmsContext_t     *smsContext_p = ptrToSmsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  sendSsRegistrationSignal(SMS, entity, SIG_APEX_SM_DELETE_CNF);

  KiCreateZeroSignal(SIG_APEX_SM_DELETE_REQ,
                     sizeof (ApexSmDeleteReq),
                     &sigBuff);

  request_p = (ApexSmDeleteReq *) sigBuff.sig;

  request_p->taskId       = VG_CI_TASK_ID;
  request_p->recordNumber = smsContext_p->cmgdRecordNum;
  request_p->commandRef   = 0; /*unused */

  KiSendSignal(cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmDeleteSmsrReq
 *
 * Parameters:      entity - SMS SR context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_DELETE_SMSR_REQ to the background layer
 *                  Called by the change control manager in vgchman.c
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmDeleteSmsrReq( const VgmuxChannelNumber entity )
{
  SignalBuffer          sigBuff = kiNullBuffer;
  ApexSmDeleteSmsrReq  *request_p;
  SmsContext_t         *smsContext_p = ptrToSmsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  sendSsRegistrationSignal(SMS, entity, SIG_APEX_SM_DELETE_SMSR_CNF);

  KiCreateZeroSignal(SIG_APEX_SM_DELETE_SMSR_REQ,
                     sizeof(ApexSmDeleteSmsrReq),
                     &sigBuff);

  request_p = (ApexSmDeleteSmsrReq *)sigBuff.sig;

  request_p->taskId       = VG_CI_TASK_ID;
  request_p->recordNumber = smsContext_p->cmgdRecordNum;
  request_p->commandRef   = 0; /*unused */

  KiSendSignal(cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexReadSms
 *
 * Parameters:      entity - SMS context entity number
 *                  sig - pointer to read req structure
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_READ_REQ to the background layer
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSmsSigOutApexReadSmsNoReadState(const VgmuxChannelNumber entity, Int8 idx, SmsSimAccessType readType)
{
  SmsContext_t  *smsContext_p = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  /* Save vars for sending of the signal */
  smsContext_p->smReadReq.record   = idx;
  smsContext_p->smReadReq.readType = readType;

  return( vgChManContinueActionFlowControl(entity, SIG_APEX_SM_READ_REQ) );
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexReadSms
 *
 * Parameters:      entity - SMS context entity number
 *                  sig - pointer to read req structure
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_READ_REQ to the background layer
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSmsSigOutApexReadSmsSetReadState(const VgmuxChannelNumber entity, Int8 idx, SmsSimAccessType readType)
{
  SmsContext_t  *smsContext_p = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  /* Save vars for the ReadCnf handler */
  smsContext_p->readState = readType;

  return( vgSmsSigOutApexReadSmsNoReadState(entity, idx, readType) );
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmReadReq
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_READ_REQ to the background layer
 *                  indirectly; called by the change control manager.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmReadReq(const VgmuxChannelNumber entity)
{
  SignalBuffer    sigBuff = kiNullBuffer;
  ApexSmReadReq  *request_p;
  SmsContext_t   *smsContext_p = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  sendSsRegistrationSignal(SMS, entity, SIG_APEX_SM_READ_CNF);

  KiCreateZeroSignal(SIG_APEX_SM_READ_REQ,
                     sizeof (ApexSmReadReq),
                     &sigBuff);

  request_p = (ApexSmReadReq *)sigBuff.sig;

  request_p->commandRef   = 0; /*unused */
  request_p->taskId       = VG_CI_TASK_ID;
  request_p->recordNumber = smsContext_p->smReadReq.record;
  request_p->readType     = smsContext_p->smReadReq.readType;

  KiSendSignal(cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexReadSmsrNodata
 *
 * Parameters:      entity - SMS SR context entity number
 *                  sig - pointer to read req structure
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_READ_SMSR_NODATA_REQ to the background layer
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSmsSigOutApexReadSmsrNoData( const VgmuxChannelNumber entity, SmsSimAccessType readType )
{
  SmsContext_t   *smsContext_p = ptrToSmsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  /* Save vars for sending of the signal */
  smsContext_p->smReadReq.readType = readType;
  return (vgChManContinueAction(entity, SIG_APEX_SM_READ_SMSR_NODATA_REQ));
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmReadSmsrNoDataReq
 *
 * Parameters:      entity - SMS SR context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_READ_SMSR_NODATA_REQ to the background layer
 *                  indirectly; called by the change control manager.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmReadSmsrNoDataReq( const VgmuxChannelNumber entity )
{
  SignalBuffer        sigBuff = kiNullBuffer;
  ApexSmReadSmsrReq  *request_p;
  SmsContext_t       *smsContext_p = ptrToSmsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  sendSsRegistrationSignal( SMS, entity, SIG_APEX_SM_READ_SMSR_NODATA_CNF);

  KiCreateZeroSignal(SIG_APEX_SM_READ_SMSR_NODATA_REQ,
                     sizeof(ApexSmReadSmsrReq),
                     &sigBuff);

  request_p = (ApexSmReadSmsrReq *)sigBuff.sig;
  request_p->taskId   = VG_CI_TASK_ID;
  request_p->readType = smsContext_p->smReadReq.readType;

  KiSendSignal(cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexReadSmsrNoReadState
 *
 * Parameters:      entity - SMS SR context entity number
 *                  sig - pointer to read req structure
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_READ_SMSR_REQ to the background layer
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSmsSigOutApexReadSmsrNoReadState(const VgmuxChannelNumber entity, Int8 idx, SmsSimAccessType readType)
{
  SmsContext_t  *smsContext_p = ptrToSmsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  /* Save vars for sending of the signal */
  smsContext_p->smReadReq.record   = idx;
  smsContext_p->smReadReq.readType = readType;

  return( vgChManContinueActionFlowControl(entity, SIG_APEX_SM_READ_SMSR_REQ) );
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexReadSmsSetReadState
 *
 * Parameters:      entity - SMS SR context entity number
 *                  sig - pointer to read req structure
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_READ_SMSR_REQ to the background layer
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSmsSigOutApexReadSmsrSetReadState(const VgmuxChannelNumber entity, Int8 idx, SmsSimAccessType readType)
{
  SmsContext_t  *smsContext_p = ptrToSmsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  /* Save vars for the ReadCnf handler */
  smsContext_p->readState = readType; 

  return( vgSmsSigOutApexReadSmsrNoReadState(entity, idx, readType) );
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmReadSmsrReq
 *
 * Parameters:      entity - SMS SR context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_READ_SMSR_REQ to the background layer
 *                  indirectly; called by the change control manager.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmReadSmsrReq(const VgmuxChannelNumber entity)
{
  SignalBuffer        sigBuff = kiNullBuffer;
  ApexSmReadSmsrReq  *request_p;
  SmsContext_t       *smsContext_p = ptrToSmsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  sendSsRegistrationSignal( SMS, entity, SIG_APEX_SM_READ_SMSR_CNF);

  KiCreateZeroSignal(SIG_APEX_SM_READ_SMSR_REQ,
                     sizeof(ApexSmReadSmsrReq),
                     &sigBuff);

  request_p = (ApexSmReadSmsrReq *)sigBuff.sig;

  request_p->commandRef   = 0; /*unused */
  request_p->taskId       = VG_CI_TASK_ID;
  request_p->recordNumber = smsContext_p->smReadReq.record;
  request_p->readType     = smsContext_p->smReadReq.readType;

  KiSendSignal(cfRvAbTaskId, &sigBuff);
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexReadSmspReq
 *
 * Parameters:      entity - SMS context entity number
 *                  record - index of record to read
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_READ_SMSP_REQ to the
 *                  background layer but first gains control of the
 *                  background layer using the change control manager.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsSigOutApexReadSmspReq(const VgmuxChannelNumber entity, VgSmspState smspState)
{
  SmsContext_t   *smsContext_p = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  smsContext_p->smspState = smspState;
  return (vgChManContinueAction(entity, SIG_APEX_SM_READ_SMSP_REQ));
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmReadSmspReq
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_READ_SMSP_REQ to the
 *                  background layer indirectly; called by the
 *                  change control manager.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmReadSmspReq(const VgmuxChannelNumber entity)
{
  SignalBuffer       sigBuff       = kiNullBuffer;
  ApexSmReadSmspReq  *request_p;
  SmsContext_t       *smsContext_p = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  sendSsRegistrationSignal (SMS, entity, SIG_APEX_SM_READ_SMSP_CNF);

  KiCreateZeroSignal (SIG_APEX_SM_READ_SMSP_REQ,
                       sizeof (ApexSmReadSmspReq),
                        &sigBuff);

  request_p = (ApexSmReadSmspReq *) sigBuff.sig;
  request_p->taskId       = VG_CI_TASK_ID;
  request_p->commandRef   = (Int16)(smsContext_p->smspState);
  request_p->recordNumber = 1; /* always 1 */

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexWriteSmspReq
 *
 * Parameters:      entity - SMS context entity number
 *                  sig - pointer to read confirmation structure
 *                  (holds current settings)
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_READ_REQ to the
 *                  background layer
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsSigOutApexWriteSmspReq(const VgmuxChannelNumber entity, const ApexSmReadSmspCnf *signal_p)
{
  SmsContext_t   *smsContext_p = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  smsContext_p->ApexSmReadSmspCnf_p = signal_p;
  return (vgChManContinueAction(entity, SIG_APEX_SM_WRITE_SMSP_REQ));
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmWriteSmspReq
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_READ_REQ to the
 *                  background layer
 *                  indirectly; called by the change control manager.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmWriteSmspReq(const VgmuxChannelNumber entity)
{
  const SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext ();
  const SmsContext_t       *smsContext_p = ptrToSmsContext (entity);
  ApexSmWriteSmspReq       *request_p;
  SignalBuffer             sigBuff   = kiNullBuffer;
  const ApexSmReadSmspCnf  *signal_p;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  signal_p = smsContext_p->ApexSmReadSmspCnf_p;

  sendSsRegistrationSignal( SMS, entity, SIG_APEX_SM_WRITE_SMSP_CNF);

  KiCreateZeroSignal (SIG_APEX_SM_WRITE_SMSP_REQ,
                       sizeof (ApexSmWriteSmspReq),
                        &sigBuff);

  request_p = (ApexSmWriteSmspReq *) sigBuff.sig;
  request_p->taskId       = VG_CI_TASK_ID;
  request_p->commandRef   = 0; /*unused */
  request_p->writeType    = SM_RW_ABSOLUTE;
  request_p->recordNumber = signal_p->recordNumber;
  request_p->smsp         = signal_p->smsp;


  /* Set Service Centre Address */
  request_p->smsp.scAddrPresent = TRUE;
  request_p->smsp.scAddr        = smsCommonContext_p->vgTempSca;

  /* Set validity period */
  request_p->smsp.validityPeriodPresent = TRUE;
  request_p->smsp.validityPeriodAsValue = smsCommonContext_p->validityPeriodValue;

  /* job120045: must indicate data present */
  request_p->smsp.protocolIdPresent = TRUE;
  memcpy (&request_p->smsp.smsProtocolId,
           &smsCommonContext_p->protocolId,
            sizeof(SmsProtocolId));

  /* job120045: must indicate data present */
  request_p->smsp.codingSchemePresent = TRUE;
  memcpy (&request_p->smsp.smsDataCodingScheme,
           &smsCommonContext_p->dataCodingScheme,
            sizeof(SmsDataCoding));

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexSendSmsTpdu
 *
 * Parameters:      entity - SMS context entity number
 *                  sig - pointer to send req structure
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_SEND_REQ to the background layer
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSmsSigOutApexSendSmsTpdu(const VgmuxChannelNumber entity, const TsSubmitReq *sig)
{
  /* job116776 Introducing moreMessagesToSend functionality as specified in 27.005 */
  vgCiStopTimer(TIMER_TYPE_SMS_CMMS);

  createSignalApexSendSmsTpdu(entity, sig);

  return (vgChManContinueAction(entity, SIG_APEX_SM_SEND_REQ));
}

/*--------------------------------------------------------------------------
 *
 * Function:       vgSmsSigOutApexSendSmsNormal
 *
 * Parameters:      entity - SMS context entity number
 *                  sig - pointer to send req structure
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_SEND_REQ to the background layer
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsSigOutApexSendSmsNormal (const VgmuxChannelNumber entity, const CiapSms *sig)
{
  /* job116776 Introducing moreMessagesToSend functionality as specified in 27.005 */
  vgCiStopTimer(TIMER_TYPE_SMS_CMMS);

  createSignalApexSendSmsNormal(entity, sig);
  return (vgChManContinueAction(entity, SIG_APEX_SM_SEND_REQ));
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgApexSmSendReq
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_SEND_REQ to the background layer
 *                  indirectly; called by the change control manager.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmSendReq (const VgmuxChannelNumber entity)
{
  SmsContext_t   *smsContext_p = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  sendSsRegistrationSignal( SMS, entity, SIG_APEX_SM_SEND_CNF);

  KiSendSignal (cfRvAbTaskId, &(smsContext_p->sigApexSmSendReq) );
}



/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexStoreSms
 *
 * Parameters:      entity - SMS context entity number
 *                  sig - pointer to store req structure
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_STORE_REQ to the
 *                  background layer
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsSigOutApexStoreSmsNormal(const VgmuxChannelNumber entity,
                                           const CiapSms *sig,
                                           Int8 recordNumber)
{
  createApexStoreSmsNormal( entity, sig, recordNumber);
  return (vgChManContinueAction(entity, SIG_APEX_SM_STORE_REQ));
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexStoreSmsTpdu
 *
 * Parameters:      entity - SMS context entity number
 *                  tpduType - TPDU type
 *                  sig - pointer to store req structure
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_STORE_REQ to the
 *                  background layer
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsSigOutApexStoreSmsTpdu(const VgmuxChannelNumber entity,
                                         SimSmTpduType            tpduType,
                                         const SimSmTpdu         *sig)
{
  createApexStoreSmsTpdu(entity, tpduType, sig);
  return (vgChManContinueAction(entity, SIG_APEX_SM_STORE_REQ));
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmStoreReq
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_STORE_REQ to the
 *                  background layer
 *                  indirectly; called by the change control manager.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmStoreReq(const VgmuxChannelNumber entity)
{
  SmsContext_t   *smsContext_p = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  sendSsRegistrationSignal (SMS, entity, SIG_APEX_SM_STORE_CNF);
  KiSendSignal (cfRvAbTaskId, &(smsContext_p->sigApexSmStoreReq));
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgApexSendSmDeliveryWithReportRsp
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_DELIVERY_WITH_REPORT_RSP to the
 *                  background layer indirectly; called by the change control manager.
 *
 *-------------------------------------------------------------------------*/
void vgApexSendSmDeliveryWithReportRsp(const VgmuxChannelNumber entity)
{
    SmsContext_t   *smsContext_p = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    KiSendSignal (cfRvAbTaskId, &(smsContext_p->sigApexSmDeliveryWithReportRsp));
}

 /*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexSendFromSim
 *
 * Parameters:      entity - SMS context entity number
 *                  sig - pointer to sim req structure
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_SEND_FROM_SIM_REQ to the
 *                  background layer
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsSigOutApexSendFromSim(const VgmuxChannelNumber entity)
{
  /* job116776 Introducing moreMessagesToSend functionality */
  vgCiStopTimer(TIMER_TYPE_SMS_CMMS);

  return (vgChManContinueAction (entity, SIG_APEX_SM_SEND_FROM_SIM_REQ));
}

 /*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmSendFromSimReq
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_SEND_FROM_SIM_REQ to the
 *                  background layer
 *                  indirectly; called by the change control manager.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmSendFromSimReq(const VgmuxChannelNumber entity)
{
  SignalBuffer          sigBuff             = kiNullBuffer;
  ApexSmSendFromSimReq  *request_p;
  SmsContext_t          *smsContext_p       = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  sendSsRegistrationSignal (SMS, entity, SIG_APEX_SM_SEND_FROM_SIM_CNF);

  KiCreateZeroSignal (SIG_APEX_SM_SEND_FROM_SIM_REQ,
                       sizeof (ApexSmSendFromSimReq),
                        &sigBuff);

  request_p = (ApexSmSendFromSimReq *) sigBuff.sig;

  memset(request_p, 0, sizeof(ApexSmSendFromSimReq));

  request_p->taskId          = VG_CI_TASK_ID;
  request_p->commandRef      = 0; /*unused */
  request_p->recordNumber    = (Int8)smsContext_p->atCmssParams.recordNumber;

  request_p->scAddrPresent   = TRUE;
  vgSmsUtilGetSca(&request_p->scAddr);

  /* Check to see if specified new sme address in AT+CMSS command */
  if ( smsContext_p->atCmssParams.useNewDest )
  {
    request_p->smeAddrPresent = TRUE;
    memcpy (&request_p->smeAddr, &smsContext_p->atCmssParams.dest ,sizeof(SmsAddress));
  }
  else
  {
    request_p->smeAddrPresent = FALSE;
  }

  /*job100892 Enable FDN checking on SMS send*/
  request_p->doFdnCheck = TRUE;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

 /*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmSendMoreReq
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         result code - RESULT_CODE_OK
 *
 * Description:     Sends a SIG_APEX_SM_SEND_MORE_REQ to the
 *                  background layer indirectly; job116776
 *                  called by the change control manager.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmSendMoreReq (const VgmuxChannelNumber entity)
{
  SignalBuffer          sigBuff = kiNullBuffer;
  ApexSmSendMoreReq     *request_p;
  SmsCommonContext_t    *smsCommonContext_p = ptrToSmsCommonContext ();

  PARAMETER_NOT_USED(entity);

  KiCreateZeroSignal (SIG_APEX_SM_SEND_MORE_REQ,
                       sizeof (ApexSmSendMoreReq),
                        &sigBuff);

  request_p = (ApexSmSendMoreReq *) sigBuff.sig;

  request_p->taskId          = VG_CI_TASK_ID;

  request_p->isEnabled       = (smsCommonContext_p->moreMsgsToSendMode > 0)? TRUE:FALSE;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexSendCommandSms
 *
 * Parameters:      entity - SMS context entity number
 *                  sig - pointer to send req structure
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_COMMAND_REQ to the background layer
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsSigOutApexSendCommandSmsNormal (const VgmuxChannelNumber entity, const CiapSms *sms)
{
  /* job116776 Introducing moreMessagesToSend functionality as specified in 27.005 */
  vgCiStopTimer(TIMER_TYPE_SMS_CMMS);

  createSigApexSmCommandReqNormal(entity, sms);

  return (vgChManContinueAction (entity, SIG_APEX_SM_COMMAND_REQ));
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexSendCommandSmsTpdu
 *
 * Parameters:      entity - SMS context entity number
 *                  sig - pointer to send req structure
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_COMMAND_REQ to the background layer,
 *                  built from the SMS TPDU in PDU Mode.
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsSigOutApexSendCommandSmsTpdu (const VgmuxChannelNumber entity, const TsCommandReq *sig)
{
  /* job116776 Introducing moreMessagesToSend functionality as specified in 27.005 */
  vgCiStopTimer(TIMER_TYPE_SMS_CMMS);

  createSigApexSmCommandReqTpdu(entity, sig);
  return (vgChManContinueAction (entity, SIG_APEX_SM_COMMAND_REQ));
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexSmDeliveryWithReportRspTpdu
 *
 * Parameters:      entity - SMS context entity number
 *                  sig - pointer to send req structure
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_DELIVERY_WITH_REPORT_RSP to the background layer,
 *                  built from the SMS TPDU in PDU Mode.
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsSigOutApexSmDeliveryWithReportRspTpdu(    const VgmuxChannelNumber entity, 
                                                            const TsDeliverReportReq *sig)
{
    /* stop the timer */
    vgCiStopTimer(TIMER_TYPE_SMS_TR2M);

    createSigApexSmDeliveryWithReportRspTpdu(entity, sig);
    return (vgChManContinueAction (entity, SIG_APEX_SM_DELIVERY_WITH_REPORT_RSP));
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmCommandReq
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_COMMAND_REQ to the background layer
 *                  indirectly; called by the change control manager.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmCommandReq(const VgmuxChannelNumber entity)
{
  SmsContext_t   *smsContext_p = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)
  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif

  /* job116776 Introducing moreMessagesToSend functionality as specified in 27.005 */
  vgCiStopTimer(TIMER_TYPE_SMS_CMMS);

  sendSsRegistrationSignal (SMS, entity, SIG_APEX_SM_COMMAND_CNF);
  KiSendSignal (cfRvAbTaskId, &(smsContext_p->sigApexSmCommandReq));
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexSmStatusReq
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_STATUS_REQ to the background layer
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSmsSigOutApexSmStatusReq (const VgmuxChannelNumber entity)
{
  return (vgChManContinueAction(entity, SIG_APEX_SM_STATUS_REQ));
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmStatusReq
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_STATUS_REQ to the background layer
 *                  Called by the change control manager in vgchman.c
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmStatusReq(const VgmuxChannelNumber entity)
{
  SignalBuffer     sigBuff = kiNullBuffer;
  ApexSmStatusReq  *request_p;

  sendSsRegistrationSignal( SMS, entity, SIG_APEX_SM_STATUS_CNF);

  KiCreateZeroSignal (SIG_APEX_SM_STATUS_REQ,
                       sizeof (ApexSmStatusReq),
                        &sigBuff);

  request_p = (ApexSmStatusReq *) sigBuff.sig;
  request_p->taskId = VG_CI_TASK_ID;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexReadSmsNodata
 *
 * Parameters:      entity - SMS context entity number
 *                  sig - pointer to read req structure
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_READ_NODATA_REQ to the background layer
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSmsSigOutApexReadSmsNoData (const VgmuxChannelNumber entity, SmsSimAccessType readType)
{
  SmsContext_t   *smsContext_p = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  /* Save vars for sending of the signal */
  smsContext_p->smReadReq.readType = readType;
  return (vgChManContinueAction(entity, SIG_APEX_SM_READ_NODATA_REQ));
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmReadNoDataReq
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_READ_NODATA_REQ to the background layer
 *                  indirectly; called by the change control manager.
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmReadNoDataReq(const VgmuxChannelNumber entity)
{
  SignalBuffer    sigBuff = kiNullBuffer;
  ApexSmReadReq  *request_p;
  SmsContext_t   *smsContext_p = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  sendSsRegistrationSignal( SMS, entity, SIG_APEX_SM_READ_NODATA_CNF);

  KiCreateZeroSignal(SIG_APEX_SM_READ_NODATA_REQ,
                     sizeof (ApexSmReadReq),
                     &sigBuff);

  request_p = (ApexSmReadReq *) sigBuff.sig;
  request_p->taskId       = VG_CI_TASK_ID;
  request_p->readType     = smsContext_p->smReadReq.readType;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexStoreLocReq
 *
 * Parameters:      entity - SMS context entity number
 *                  sig - pointer to read req structure
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_READ_NODATA_REQ to the background layer
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSmsSigOutApexStoreLocReq (const VgmuxChannelNumber entity, Int16 loc)
{
  SmsContext_t     *smsContext_p = ptrToSmsContext     (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  smsContext_p->smsStoreLoc = loc;
  return (vgChManContinueAction(entity, SIG_APEX_SM_STORE_LOC_REQ));

}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmStoreLocReq
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_STORE_LOC_REQ to the background layer
 *                  
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmStoreLocReq(const VgmuxChannelNumber entity)
{
  SignalBuffer        sigBuff = kiNullBuffer;
  ApexSmStoreLocReq  *request_p;
  SmsContext_t       *smsContext_p = ptrToSmsContext     (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (smsContext_p != PNULL, entity, 0, 0);
#endif
  sendSsRegistrationSignal( SMS, entity, SIG_APEX_SM_STORE_LOC_CNF);
  
  KiCreateZeroSignal (SIG_APEX_SM_STORE_LOC_REQ,
                       sizeof (ApexSmStoreLocReq),
                        &sigBuff);

  request_p = (ApexSmStoreLocReq *) sigBuff.sig;
  request_p->loc = smsContext_p->smsStoreLoc;
  request_p->taskId = VG_CI_TASK_ID;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSigOutApexSetLocStatusReq
 *
 * Parameters:      entity - SMS context entity number
 *                  sig - pointer to read req structure
 *
 * Returns:         result code - RESULT_CODE_PROCEEDING if ok, othersise
 *                  an error result code.
 *
 * Description:     Sends a SIG_APEX_SM_SET_LOC_STATUS_REQ to the background layer
 *                  but first gains control of the background layer using
 *                  the change control manager.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSmsSigOutApexSetLocStatusReq (const VgmuxChannelNumber entity, Int16 loc, SimSmRecordStatus status)
{
  SmsContext_t     *smsContext_p = ptrToSmsContext     (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  smsContext_p->smsMmgscLoc = loc;
  smsContext_p->smsMmgscStatus = status;
  return (vgChManContinueAction(entity, SIG_APEX_SM_SET_LOC_STATUS_REQ));

}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmSetLocStatusReq
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_SET_LOC_STATUS_REQ to the background layer
 *                  
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmSetLocStatusReq(const VgmuxChannelNumber entity)
{
  SignalBuffer        sigBuff = kiNullBuffer;
  ApexSmSetLocStatusReq  *request_p;
  SmsContext_t       *smsContext_p = ptrToSmsContext     (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (smsContext_p != PNULL, entity, 0, 0);
#endif
  sendSsRegistrationSignal( SMS, entity, SIG_APEX_SM_SET_LOC_STATUS_CNF);
  
  KiCreateZeroSignal(   SIG_APEX_SM_SET_LOC_STATUS_REQ,
                        sizeof (ApexSmSetLocStatusReq),
                        &sigBuff);

  request_p = (ApexSmSetLocStatusReq *) sigBuff.sig;
  request_p->loc    = smsContext_p->smsMmgscLoc;
  request_p->status = smsContext_p->smsMmgscStatus;
  request_p->taskId = VG_CI_TASK_ID;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmStoreRsp
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_STORE_RSP to the background layer
 *                  
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmStoreRsp(const VgmuxChannelNumber entity)
{
    SignalBuffer      sigBuff = kiNullBuffer;
    ApexSmStoreRsp   *response_p;
    SmsContext_t     *smsContext_p = ptrToSmsContext     (entity);
#if defined (ATCI_SLIM_DISABLE)
    FatalCheck (smsContext_p != PNULL, entity, 0, 0);
#endif
    KiCreateZeroSignal( SIG_APEX_SM_STORE_RSP,
                        sizeof (ApexSmStoreRsp),
                        &sigBuff);

    response_p = (ApexSmStoreRsp *) sigBuff.sig;
    *response_p = smsContext_p->apexSmStoreRsp;

    KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexSmSmsrStoreRsp
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_SMSR_STORE_RSP to the background layer
 *                  
 *
 *-------------------------------------------------------------------------*/
void vgSigApexSmSmsrStoreRsp(const VgmuxChannelNumber entity)
{
    SignalBuffer        sigBuff = kiNullBuffer;
    ApexSmSmsrStoreRsp *response_p;
    SmsContext_t       *smsContext_p = ptrToSmsContext     (entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck (smsContext_p != PNULL, entity, 0, 0);
#endif
    KiCreateZeroSignal( SIG_APEX_SM_SMSR_STORE_RSP,
                        sizeof (ApexSmSmsrStoreRsp),
                        &sigBuff);

    response_p = (ApexSmSmsrStoreRsp *) sigBuff.sig;
    *response_p = smsContext_p->apexSmSmsrStoreRsp;

    KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/* END OF FILE */

