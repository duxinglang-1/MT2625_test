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
 * Outgoing signal handlers for the General Sub-System.
 *
 * Procedures simply send a signal. The signal is created, its expected
 * returning signal is registered, contents filled and then it is sent.
 **************************************************************************/

#define MODULE_NAME "RVGNSIGO"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <gkisig.h>

#include <dmpm_typ.h>
#include <dmpm_sig.h>
#include <abgl_sig.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvgnsigo.h>
#include <rvchman.h>
#include <rvcfg.h>
#include <dmrtc_sig.h>
#include <n1cd_sig.h>
#include <n1tst_sig.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  ApexPmModeChangeReq         apexPmModeChangeReq;
  ApexSimReadSpnReq           apexSimReadSpnReq;
  DmRtcReadDateAndTimeReq     dmRtcReadDateAndTimeReq;
  DmRtcSetDateAndTimeReq      dmRtcSetDateAndTimeReq;
  ApexSimChvFunctionReq       apexSimChvFunctionReq;
  ApexSimPinFunctionReq       apexSimPinFunctionReq;
  ApexSimMepReq               apexSimMepReq;
  ApexSimMepStatusReq         apexSimMepStatusReq;
  ApexSimEmuSimReq            apexSimEmuSimReq;
  ApexMmWriteMobileIdReq      apexMmWriteMobileIdReq;
  ApexMmReadMobileIdReq       apexMmReadMobileIdReq;
#if !defined (N1_REMOVE_CALDEV)  
  N1CdEnterReq                n1CdEnterReq;
  N1CdRfTestReq               n1CdRfTestReq;
#endif  
#if defined (DM_EXCLUDE_RTC_DEVICE_MANAGER)
  DmRtcReadDateAndTimeCnf     dmRtcReadDateAndTimeCnf;
  DmRtcSetDateAndTimeCnf      dmRtcSetDateAndTimeCnf;
#endif
#if defined (FEA_PHONEBOOK)
  ApexLmFindDialNumReq        apexLmFindDialNumReq;
  ApexLmWriteDialNumReq       apexLmWriteDialNumReq;
  ApexLmFixedDialFindReq      apexLmFixedDialFindReq;
  ApexLmGetAlphaReq           apexLmGetAlphaReq;
  ApexLmDialNumStatusReq      apexLmDialNumStatusReq;
  ApexLmReadDialNumReq        apexLmReadDialNumReq;
  ApexLmDeleteDialNumReq      apexLmDeleteDialNumReq;
  ApexLmFixedDialReq          apexLmFixedDialReq;
  ApexLmBarredDialReq         apexLmBarredDialReq;
#endif /* FEA_PHONEBOOK */
};

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

#if defined (FEA_SIMLOCK)
#define VG_MEP_COMMAND_REF ((VG_CI_TASK_ID<<8) & 0xFF00)
#endif /* FEA_SIMLOCK */

#if defined (FEA_PHONEBOOK)
/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static LmAlphaListEntryType vgConvertPhoneBookToAlphaList (const LmDialNumberFile phoneBook);
/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/
static LmAlphaListEntryType vgConvertPhoneBookToAlphaList (const LmDialNumberFile phoneBook)
{
  LmAlphaListEntryType alphaList;

  switch (phoneBook)
  {
    case DIAL_LIST_ADN_GLB:
      alphaList = LM_ALPHA_LIST_ADN_GLB;
      break;
    case DIAL_LIST_ADN_APP:
      alphaList = LM_ALPHA_LIST_ADN_APP;
      break;
    case DIAL_LIST_FDN:
      alphaList = LM_ALPHA_LIST_FDN;
      break;
    case DIAL_LIST_ADR:
      alphaList = LM_ALPHA_LIST_ADR;
      break;
    case DIAL_LIST_MSISDN:
      alphaList = LM_ALPHA_LIST_MSISDN;
      break;
    case DIAL_LIST_CPHS_MN:
      alphaList = LM_ALPHA_LIST_CPHS_MN;
      break;
    case DIAL_LIST_SDN:
      alphaList = LM_ALPHA_LIST_SDN;
      break;
    case DIAL_LIST_BDN:
      alphaList = LM_ALPHA_LIST_BDN;
      break;
    default:
      FatalParam (phoneBook, 0, 0);
      alphaList = LM_ALPHA_LIST_ALL;
      break;
  }

  return (alphaList);
}
#endif /* FEA_PHONEBOOK */

/***************************************************************************
 * Global Functions
 ***************************************************************************/

#if defined (FEA_PHONEBOOK)
/*--------------------------------------------------------------------------
*
* Function:        vgSigApexLmDialNumStatusReq
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*
* Returns:         Nothing
*
* Description:     Sends a SIG_APEX_LM_DIALNUM_STATUS_REQ to the AB. This
*                  returns information about the available phonebooks.
*-------------------------------------------------------------------------*/

void vgSigApexLmDialNumStatusReq (const VgmuxChannelNumber entity)
{
  SignalBuffer           sigBuff = kiNullBuffer;
  ApexLmDialNumStatusReq *request_p;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_DIALNUM_STATUS_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_DIALNUM_STATUS_REQ,
                       sizeof (ApexLmDialNumStatusReq),
                        &sigBuff);

  request_p             = (ApexLmDialNumStatusReq *) sigBuff.sig;
  request_p->taskId     = VG_CI_TASK_ID;
  request_p->commandRef = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

 /*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexLmFindDialNumReq
 *
 * Parameters:      VgmuxChannelNumber - entity which sent request
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_LM_FIND_DIALNUM_REQ to the
 *                  background layer which searches for a specified alpha id
 *
 *-------------------------------------------------------------------------*/


void vgSigApexLmFindDialNumReq (const VgmuxChannelNumber entity)
{
  SignalBuffer         sigBuff = kiNullBuffer;
  ApexLmFindDialNumReq *request_p;
  GeneralContext_t     *generalContext_p = ptrToGeneralContext (entity);

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                             entity,
                              SIG_APEX_LM_FIND_DIALNUM_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_FIND_DIALNUM_REQ,
                       sizeof (ApexLmFindDialNumReq),
                        &sigBuff);

  request_p = (ApexLmFindDialNumReq *) sigBuff.sig;

  request_p->taskId         = VG_CI_TASK_ID;
  request_p->commandRef     = (Int16)entity;
  request_p->findMode       = LM_FIND_EXACT;
  request_p->alphaIndex     = 0;
  request_p->alphaId.length = generalContext_p->vgLmData.alphaLength;
  request_p->entryType = vgConvertPhoneBookToAlphaList (generalContext_p->vgLmData.phoneBook);

  memcpy (&request_p->alphaId.data[0],
           generalContext_p->vgLmData.alpha,
           request_p->alphaId.length);

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}



/*--------------------------------------------------------------------------
*
* Function:        vgSigApexLmReadDialNumReq
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*
* Returns:         Nothing
*
* Description:     Sends a SIG_APEX_LM_READ_DIALNUM_REQ to the
*                  background layer. This reads an entry in the current
*                  phone book.
*-------------------------------------------------------------------------*/

void vgSigApexLmReadDialNumReq (const VgmuxChannelNumber entity)
{
  SignalBuffer         sigBuff = kiNullBuffer;
  GeneralContext_t     *generalContext_p = ptrToGeneralContext (entity);
  ApexLmReadDialNumReq *request_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                             entity,
                              SIG_APEX_LM_READ_DIALNUM_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_READ_DIALNUM_REQ,
                       sizeof (ApexLmReadDialNumReq),
                        &sigBuff);

  request_p = (ApexLmReadDialNumReq *) sigBuff.sig;

  request_p->taskId       = VG_CI_TASK_ID;
  request_p->commandRef   = (Int16)entity;
  request_p->recordNumber = generalContext_p->vgLmData.phoneIndex1;
  request_p->readMode     = generalContext_p->vgLmData.readMode;
  request_p->file         = generalContext_p->vgLmData.phoneBook;
  if (request_p->file == DIAL_LIST_ICI)
  {
    request_p->iciType      = generalContext_p->vgLmData.iciType;
  }
  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:        vgSigApexLmWriteDialNumReq
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*
* Returns:         Nothing
*
* Description:     Sends a SIG_APEX_LM_WRITE_DIALNUM_REQ to the background
*                  layer. This creates of deletes an entry into the current
*                  phonebook.
*-------------------------------------------------------------------------*/
void vgSigApexLmWriteDialNumReq (const VgmuxChannelNumber entity)
{
    SignalBuffer          sigBuff = kiNullBuffer;
    GeneralContext_t      *generalContext_p = ptrToGeneralContext (entity);
    VgLmData              *vgLmData;
    ApexLmWriteDialNumReq *request_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData = &generalContext_p->vgLmData;

    FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal(   GENERAL,
                                entity,
                                SIG_APEX_LM_WRITE_DIALNUM_CNF);

    KiCreateZeroSignal( SIG_APEX_LM_WRITE_DIALNUM_REQ,
                        sizeof (ApexLmWriteDialNumReq),
                        &sigBuff);

    request_p               = (ApexLmWriteDialNumReq *) sigBuff.sig;
    request_p->taskId       = VG_CI_TASK_ID;
    request_p->writeMode    = vgLmData->writeMode;
    request_p->recordNumber = vgLmData->phoneIndex1;
    request_p->file         = vgLmData->phoneBook;
    request_p->commandRef   = (Int16)entity;
    request_p->forReplace   = vgLmData->forReplace;

    vgConvTextToBcd(    vgLmData->writeNum,
                        vgLmData->writeNumLength,
                        &request_p->dialNumber.typeOfNumber,
                        &request_p->dialNumber.numberPlan,
                        request_p->dialNumber.dialString,
                        &request_p->dialNumber.dialStringLength,
                        entity);

    if (vgLmData->phoneBookNumTypePresent == TRUE)
    {
        request_p->dialNumber.typeOfNumber = vgLmData->phoneBookNumType;
    }

    vgLmData->phoneBookNumTypePresent      = FALSE;
    request_p->dialNumber.isDiallingNumber        = TRUE;
    request_p->dialNumber.bearerCapabilityDefined = FALSE;
    request_p->dialNumber.subAddressDefined       = FALSE;
    request_p->dialNumber.alphaId.length          = vgLmData->alphaLength;

    memcpy( &request_p->dialNumber.alphaId.data[0],
            &vgLmData->alpha[0],
            request_p->dialNumber.alphaId.length);

    request_p->hiddenEntry                        = vgLmData->hiddenEntry;

    if( (vgLmData->phoneBook == DIAL_LIST_OCI) ||
        (vgLmData->phoneBook == DIAL_LIST_ICI))
    {
        request_p->dataTimePresent = vgLmData->usimCallInfo.dataTimePresent;
        request_p->dateTime = vgLmData->usimCallInfo.dateTime;
        request_p->callDuration = vgLmData->usimCallInfo.callDuration;
        if( vgLmData->phoneBook == DIAL_LIST_ICI)
        {
            request_p->callAnswered = vgLmData->usimCallInfo.callAnswered;
        }
    }

    /* time and date are not specified when we write to a phonebook */

    KiSendSignal (cfRvAbTaskId, &sigBuff);
}


/*--------------------------------------------------------------------------
*
* Function:        vgSigApexLmDeleteDialNumReq
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*
* Returns:         Nothing
*
* Description:     Sends a SIG_APEX_LM_DELETE_DIALNUM_REQ to the background
*                  layer. This deletes an entry from the current phonebook.
*-------------------------------------------------------------------------*/


void vgSigApexLmDeleteDialNumReq (const VgmuxChannelNumber entity)
{
  SignalBuffer           sigBuff = kiNullBuffer;
  GeneralContext_t       *generalContext_p = ptrToGeneralContext (entity);
  ApexLmDeleteDialNumReq *request_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                             entity,
                              SIG_APEX_LM_DELETE_DIALNUM_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_DELETE_DIALNUM_REQ,
                       sizeof (ApexLmDeleteDialNumReq),
                        &sigBuff);

  request_p               = (ApexLmDeleteDialNumReq *) sigBuff.sig;
  request_p->taskId       = VG_CI_TASK_ID;
  request_p->recordNumber = generalContext_p->vgLmData.phoneIndex1;
  request_p->deleteMode   = LM_DELETE_ABSOLUTE;
  request_p->file         = generalContext_p->vgLmData.phoneBook;
  request_p->commandRef   = (Int16)entity;
  request_p->forReplace   = generalContext_p->vgLmData.forReplace;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:        vgSigApexLmGetAlphaReq
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*
* Returns:         Nothing
*
* Description:     Sends a SIG_APEX_LM_GET_ALPHA_REQ which checks to see if
*                  a dial string has a matching alpha ID on the SIM.  Due
*                  to multiple call ability, we need to be able to send
*                  multiple requests.
*-------------------------------------------------------------------------*/

void vgSigApexLmGetAlphaReq (const VgmuxChannelNumber entity)
{
  SignalBuffer          sigBuff = kiNullBuffer;
  ApexLmGetAlphaReq     *request_p;
  Int8                  callIndex;
  GeneralContext_t      *generalContext_p = ptrToGeneralContext (entity);
  VgAlphaLookup         *vgAlphaLookup_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  /* Necessary to send more than one alpha ID request.  Stored in array
   * element, each element's index corresponds to a call ID (-1)....
   */
  for (callIndex = 0; callIndex < VG_MAX_USER_CALL_ID; callIndex++)
  {
    vgAlphaLookup_p = &generalContext_p->vgAlphaLookup[callIndex];

    if (vgAlphaLookup_p->active == TRUE)
    {
      sendSsRegistrationSignal (GENERAL,
                                entity,
                                SIG_APEX_LM_GET_ALPHA_CNF);

      KiCreateZeroSignal (SIG_APEX_LM_GET_ALPHA_REQ,
                           sizeof (ApexLmGetAlphaReq),
                            &sigBuff);

      request_p              = (ApexLmGetAlphaReq *) sigBuff.sig;
      request_p->taskId      = VG_CI_TASK_ID;
      request_p->magicNumber = 0;
      request_p->bcdNumber   = vgAlphaLookup_p->dialledBcdNum;
      request_p->callId      = (callIndex + 1);  /* This corresponds to call ID
                                                 since array 0 indexed */

      vgAlphaLookup_p->active = FALSE;

      KiSendSignal (cfRvAbTaskId, &sigBuff);

      generalContext_p->pendingAlphaReq++;
    }
  }
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmFixedDialReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends Fixed Dial Number enable/disable
*              request (SIG_APEX_LM_FIXED_DIAL_REQ) to LM.
*-------------------------------------------------------------------------*/

void vgSigApexLmFixedDialReq (const VgmuxChannelNumber entity)
{
  SignalBuffer        signalToSend      = kiNullBuffer;
  ApexLmFixedDialReq  *request_p;
  GeneralContext_t    *generalContext_p = ptrToGeneralContext (entity);

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                             entity,
                              SIG_APEX_LM_FIXED_DIAL_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_FIXED_DIAL_REQ,
                       sizeof(ApexLmFixedDialReq),
                        &signalToSend);

  request_p             = (ApexLmFixedDialReq *) signalToSend.sig;

  request_p->taskId     = VG_CI_TASK_ID;
  request_p->commandRef = (Int16)entity;
  request_p->enableFdn  = generalContext_p->enableFdn;

  KiSendSignal (cfRvAbTaskId, &signalToSend);
}


/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmBarredDialReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends Barred Dial Number enable/disable
*              request (SIG_APEX_LM_BARRED_DIAL_REQ) to LM.
*-------------------------------------------------------------------------*/

void vgSigApexLmBarredDialReq (const VgmuxChannelNumber entity)
{
  SignalBuffer         signalToSend      = kiNullBuffer;
  ApexLmBarredDialReq  *request_p;
  GeneralContext_t     *generalContext_p = ptrToGeneralContext (entity);

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                             entity,
                              SIG_APEX_LM_BARRED_DIAL_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_BARRED_DIAL_REQ,
                       sizeof(ApexLmBarredDialReq),
                        &signalToSend);

  request_p             = (ApexLmBarredDialReq *) signalToSend.sig;

  request_p->taskId     = VG_CI_TASK_ID;
  request_p->commandRef = (Int16)entity;
  request_p->enableBdn  = generalContext_p->enableBdn;

  KiSendSignal (cfRvAbTaskId, &signalToSend);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmPhoneBookStatusReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_PHONEBOOK_STATUS_REQ which returns
*              the general information about specific phonebook
*-------------------------------------------------------------------------*/

void vgSigApexLmPhoneBookStatusReq (const VgmuxChannelNumber entity)
{
  SignalBuffer             sigBuff = kiNullBuffer;
  ApexLmPhoneBookStatusReq *request_p;
  GeneralContext_t         *generalContext_p = ptrToGeneralContext (entity);
  VgLmData                 *vgLmData_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData_p = &generalContext_p->vgLmData;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_PHONEBOOK_STATUS_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_PHONEBOOK_STATUS_REQ,
                      sizeof (ApexLmPhoneBookStatusReq),
                      &sigBuff);

  request_p                  = (ApexLmPhoneBookStatusReq *) sigBuff.sig;
  request_p->taskId          = VG_CI_TASK_ID;
  request_p->file            = vgLmData_p->phoneBook;
  request_p->commandRef      = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
  *
  * Function:    vgSigApexLmHiddenKeyFunctionReq
  *
  * Parameters:  VgmuxChannelNumber - mux channel number
  *
  * Returns:     Nothing
  *
  * Description: Sends a SIG_APEX_LM_HIDDEN_KEY_FUNCTION_REQ to the ABSI task.
  *              This signal contains a generic command for the SIM card.
  *
  *-------------------------------------------------------------------------*/
void vgSigApexLmHiddenKeyFunctionReq (const VgmuxChannelNumber entity)
{
  SignalBuffer                 sigBuff = kiNullBuffer;
  ApexLmHiddenKeyFunctionReq   *request_p;
  GeneralContext_t             *generalContext_p = ptrToGeneralContext (entity);

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_HIDDEN_KEY_FUNCTION_CNF);

  /* Create the signal */
  KiCreateZeroSignal (SIG_APEX_LM_HIDDEN_KEY_FUNCTION_REQ,
                      sizeof (ApexLmHiddenKeyFunctionReq),
                      &sigBuff);

  /* initialise the body pointer */
  request_p = (ApexLmHiddenKeyFunctionReq *) sigBuff.sig;

  /* Fill in signal */
  request_p->taskId      = VG_CI_TASK_ID;
  request_p->commandRef  = (Int16)entity;
  request_p->hiddenKeyFunction = generalContext_p->hiddenKeyFunction;

  vgTextToBcd (generalContext_p->hiddenKey,
                HIDDEN_KEY_INOUT_STRING_LENGTH,
                 request_p->hiddenKey.value );

  vgTextToBcd (generalContext_p->newHiddenKey,
                HIDDEN_KEY_INOUT_STRING_LENGTH,
                 request_p->newHiddenKey.value );

  /* send to background layer */
  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmReadGroupReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_READ_GRP_REQ which returns
*              the information about the groups to which the phonebook entry
*              is assigned.
*-------------------------------------------------------------------------*/

void vgSigApexLmReadGrpReq (const VgmuxChannelNumber entity)
{
  SignalBuffer     sigBuff = kiNullBuffer;
  ApexLmReadGrpReq *request_p;
  GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);
  VgLmData         *vgLmData_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData_p = &generalContext_p->vgLmData;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_READ_GRP_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_READ_GRP_REQ,
                      sizeof (ApexLmReadGrpReq),
                      &sigBuff);

  request_p                  = (ApexLmReadGrpReq *) sigBuff.sig;
  request_p->taskId          = VG_CI_TASK_ID;
  request_p->adnRecordNumber = vgLmData_p->phoneIndex1;
  request_p->file            = vgLmData_p->phoneBook;
  request_p->commandRef      = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmReadGasReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_READ_GAS_REQ which requests
*              the group Alpha string
*-------------------------------------------------------------------------*/

void vgSigApexLmReadGasReq (const VgmuxChannelNumber entity)
{
  SignalBuffer     sigBuff = kiNullBuffer;
  ApexLmReadGasReq *request_p;
  GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);
  VgLmData         *vgLmData_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData_p = &generalContext_p->vgLmData;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_READ_GAS_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_READ_GAS_REQ,
                      sizeof (ApexLmReadGasReq),
                      &sigBuff);

  request_p                  = (ApexLmReadGasReq *) sigBuff.sig;
  request_p->taskId          = VG_CI_TASK_ID;
  request_p->file            = vgLmData_p->phoneBook;
  request_p->gasRecordNumber = vgLmData_p->grpInfo.grpData.grpList[0];
  request_p->commandRef      = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmReadGasReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_DELETE_GAS_REQ which requests to delete
*              a GAS record
*-------------------------------------------------------------------------*/
void vgSigApexLmDeleteGasReq (const VgmuxChannelNumber entity)
{
    SignalBuffer        sigBuff = kiNullBuffer;
    ApexLmDeleteGasReq *request_p;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal(   GENERAL,
                                entity,
                                SIG_APEX_LM_DELETE_GAS_CNF);

    KiCreateZeroSignal( SIG_APEX_LM_DELETE_GAS_REQ,
                        sizeof (ApexLmDeleteGasReq),
                        &sigBuff);

    request_p                  = (ApexLmDeleteGasReq *) sigBuff.sig;
    request_p->taskId          = VG_CI_TASK_ID;
    request_p->file            = vgLmData_p->phoneBook;
    request_p->gasRecordNumber = vgLmData_p->grpInfo.grpData.grpList[0];
    request_p->commandRef      = (Int16)entity;

    KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmClearGasReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_CLEAR_GAS_REQ which requests to delete
*              all GAS record for a specified phonebook
*-------------------------------------------------------------------------*/
void vgSigApexLmClearGasReq (const VgmuxChannelNumber entity)
{
    SignalBuffer        sigBuff = kiNullBuffer;
    ApexLmClearGasReq  *request_p;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal(   GENERAL,
                                entity,
                                SIG_APEX_LM_CLEAR_GAS_CNF);

    KiCreateZeroSignal( SIG_APEX_LM_CLEAR_GAS_REQ,
                        sizeof (ApexLmClearGasReq),
                        &sigBuff);

    request_p                  = (ApexLmClearGasReq *) sigBuff.sig;
    request_p->taskId          = VG_CI_TASK_ID;
    request_p->file            = vgLmData_p->phoneBook;
    request_p->commandRef      = (Int16)entity;

    KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmReadAnrReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_READ_ANR_REQ which requests
*              the Additional number
*-------------------------------------------------------------------------*/

void vgSigApexLmReadAnrReq (const VgmuxChannelNumber entity)
{
  SignalBuffer     sigBuff = kiNullBuffer;
  ApexLmReadAnrReq *request_p;
  GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);
  VgLmData         *vgLmData_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData_p = &generalContext_p->vgLmData;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_READ_ANR_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_READ_ANR_REQ,
                      sizeof (ApexLmReadAnrReq),
                      &sigBuff);

  request_p                  = (ApexLmReadAnrReq *) sigBuff.sig;
  request_p->taskId          = VG_CI_TASK_ID;
  request_p->file            = vgLmData_p->phoneBook;
  request_p->adnRecordNumber = vgLmData_p->phoneIndex1;
  request_p->anrEntry        = vgLmData_p->adNumInfo.anrIndex;
  request_p->commandRef      = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmReadSneReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_READ_SNE_REQ which requests
*              the second name Alpha string
*-------------------------------------------------------------------------*/

void vgSigApexLmReadSneReq (const VgmuxChannelNumber entity)
{
  SignalBuffer     sigBuff = kiNullBuffer;
  ApexLmReadSneReq *request_p;
  GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);
  VgLmData         *vgLmData_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData_p = &generalContext_p->vgLmData;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_READ_SNE_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_READ_SNE_REQ,
                      sizeof (ApexLmReadSneReq),
                      &sigBuff);

  request_p                  = (ApexLmReadSneReq *) sigBuff.sig;
  request_p->taskId          = VG_CI_TASK_ID;
  request_p->file            = vgLmData_p->phoneBook;
  request_p->adnRecordNumber = vgLmData_p->phoneIndex1;
  request_p->commandRef      = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
** Function:   vgSigApexLmReadEmailReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_READ_EMAIL_REQ which requests
*              the email Alpha string
*-------------------------------------------------------------------------*/

void vgSigApexLmReadEmailReq (const VgmuxChannelNumber entity)
{
  SignalBuffer       sigBuff = kiNullBuffer;
  ApexLmReadEmailReq *request_p;
  GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
  VgLmData           *vgLmData_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData_p = &generalContext_p->vgLmData;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_READ_EMAIL_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_READ_EMAIL_REQ,
                      sizeof (ApexLmReadEmailReq),
                      &sigBuff);

  request_p                  = (ApexLmReadEmailReq *) sigBuff.sig;
  request_p->taskId          = VG_CI_TASK_ID;
  request_p->file            = vgLmData_p->phoneBook;
  request_p->adnRecordNumber = vgLmData_p->phoneIndex1;
  request_p->emailEntry      = vgLmData_p->emailInfo.emailIndex;
  request_p->commandRef      = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmListGasReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_LIST_GAS_REQ which requests
*              the informtation on all exiting groups
*-------------------------------------------------------------------------*/

void vgSigApexLmListGasReq (const VgmuxChannelNumber entity)
{
  SignalBuffer     sigBuff = kiNullBuffer;
  ApexLmListGasReq *request_p;
  GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);
  VgLmData         *vgLmData_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData_p = &generalContext_p->vgLmData;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_LIST_GAS_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_LIST_GAS_REQ,
                      sizeof (ApexLmListGasReq),
                      &sigBuff);

  request_p              = (ApexLmListGasReq *) sigBuff.sig;
  request_p->taskId      = VG_CI_TASK_ID;
  request_p->startRecord = vgLmData_p->startRecord;
  request_p->file        = vgLmData_p->phoneBook;
  request_p->commandRef  = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmWriteGrpReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_WRITE_GRP_REQ which stores
*              information about the groups to which the phonebook entry
*              is assigned.
*-------------------------------------------------------------------------*/

void vgSigApexLmWriteGrpReq (const VgmuxChannelNumber entity)
{
  SignalBuffer      sigBuff = kiNullBuffer;
  ApexLmWriteGrpReq *request_p;
  GeneralContext_t  *generalContext_p = ptrToGeneralContext (entity);
  VgLmData          *vgLmData_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData_p = &generalContext_p->vgLmData;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_WRITE_GRP_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_WRITE_GRP_REQ,
                      sizeof (ApexLmWriteGrpReq),
                      &sigBuff);

  request_p                  = (ApexLmWriteGrpReq *) sigBuff.sig;
  request_p->taskId          = VG_CI_TASK_ID;
  request_p->file            = vgLmData_p->phoneBook;
  request_p->adnRecordNumber = vgLmData_p->phoneIndex1;
  memcpy (&request_p->groupingInfo, &vgLmData_p->grpInfo.grpData, sizeof (LmGrpData));
  request_p->commandRef      = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmWriteGasReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_WRITE_GAS_REQ which stores
*              the group Alpha string
*-------------------------------------------------------------------------*/

void vgSigApexLmWriteGasReq (const VgmuxChannelNumber entity)
{
  SignalBuffer      sigBuff = kiNullBuffer;
  ApexLmWriteGasReq *request_p;
  GeneralContext_t  *generalContext_p = ptrToGeneralContext (entity);
  VgLmData          *vgLmData_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData_p = &generalContext_p->vgLmData;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_WRITE_GAS_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_WRITE_GAS_REQ,
                      sizeof (ApexLmWriteGasReq),
                      &sigBuff);

  request_p                  = (ApexLmWriteGasReq *) sigBuff.sig;
  request_p->taskId          = VG_CI_TASK_ID;
  request_p->file            = vgLmData_p->phoneBook;
  request_p->gasRecordNumber = vgLmData_p->grpInfo.grpData.grpList[0];
  memcpy (&request_p->gasRecord, &vgLmData_p->grpInfo.grpAlphaId, sizeof (LmAlphaId));
  request_p->commandRef      = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}


/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmWriteAnrReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_WRITE_ANR_REQ which stores
*              the additional number information
*-------------------------------------------------------------------------*/

void vgSigApexLmWriteAnrReq (const VgmuxChannelNumber entity)
{
    SignalBuffer      sigBuff = kiNullBuffer;
    ApexLmWriteAnrReq *request_p;
    GeneralContext_t  *generalContext_p = ptrToGeneralContext (entity);
    VgLmData          *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_WRITE_ANR_CNF);

    KiCreateZeroSignal (SIG_APEX_LM_WRITE_ANR_REQ,
                      sizeof (ApexLmWriteAnrReq),
                      &sigBuff);

    request_p                       = (ApexLmWriteAnrReq *) sigBuff.sig;
    request_p->taskId               = VG_CI_TASK_ID;
    request_p->file                 = vgLmData_p->phoneBook;
    request_p->adnRecordNumber      = vgLmData_p->phoneIndex1;
    request_p->anrEntry             = vgLmData_p->adNumInfo.anrIndex;
    request_p->anr.isDiallingNumber = TRUE;
    request_p->commandRef           = (Int16)entity;
    request_p->aasRecordNumber      = vgLmData_p->aasInfo.aasIndex;

    vgConvTextToBcd(    vgLmData_p->adNumInfo.adNum.dialNum,
                        vgLmData_p->adNumInfo.adNum.dialNumLength,
                        &request_p->anr.typeOfNumber,
                        &request_p->anr.numberPlan,
                        request_p->anr.dialString,
                        &request_p->anr.dialStringLength,
                        entity);

    KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmWriteSneReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_WRITE_SNE_REQ which stores
*              second name of the entry
*-------------------------------------------------------------------------*/

void vgSigApexLmWriteSneReq (const VgmuxChannelNumber entity)
{
  SignalBuffer      sigBuff = kiNullBuffer;
  ApexLmWriteSneReq *request_p;
  GeneralContext_t  *generalContext_p = ptrToGeneralContext (entity);
  VgLmData          *vgLmData_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData_p = &generalContext_p->vgLmData;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_WRITE_SNE_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_WRITE_SNE_REQ,
                      sizeof (ApexLmWriteSneReq),
                      &sigBuff);

  request_p                  = (ApexLmWriteSneReq *) sigBuff.sig;
  request_p->taskId          = VG_CI_TASK_ID;
  request_p->file            = vgLmData_p->phoneBook;
  request_p->adnRecordNumber = vgLmData_p->phoneIndex1;
  memcpy (&request_p->secondName, &vgLmData_p->secondName, sizeof (LmAlphaId));
  request_p->commandRef      = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmWriteEmailReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_WRITE_EMAIL_REQ which stores
*              email address of the entry
*-------------------------------------------------------------------------*/

void vgSigApexLmWriteEmailReq (const VgmuxChannelNumber entity)
{
  SignalBuffer        sigBuff = kiNullBuffer;
  ApexLmWriteEmailReq *request_p;
  GeneralContext_t    *generalContext_p = ptrToGeneralContext (entity);
  VgLmData            *vgLmData_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData_p = &generalContext_p->vgLmData;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_WRITE_EMAIL_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_WRITE_EMAIL_REQ,
                      sizeof (ApexLmWriteEmailReq),
                      &sigBuff);

  request_p                  = (ApexLmWriteEmailReq *) sigBuff.sig;
  request_p->taskId          = VG_CI_TASK_ID;
  request_p->file            = vgLmData_p->phoneBook;
  request_p->adnRecordNumber = vgLmData_p->phoneIndex1;
  request_p->emailEntry      = vgLmData_p->emailInfo.emailIndex;
  memcpy (&request_p->emailAddress, &vgLmData_p->emailInfo.email, sizeof (LmEmailAddress));
  request_p->commandRef      = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmDeleteGrpReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_DELETE_GRP_REQ which deletes
*              group info of the entry
*-------------------------------------------------------------------------*/

void vgSigApexLmDeleteGrpReq (const VgmuxChannelNumber entity)
{
  SignalBuffer       sigBuff = kiNullBuffer;
  ApexLmDeleteGrpReq *request_p;
  GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
  VgLmData           *vgLmData_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData_p = &generalContext_p->vgLmData;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_DELETE_GRP_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_DELETE_GRP_REQ,
                      sizeof (ApexLmDeleteGrpReq),
                      &sigBuff);

  request_p                  = (ApexLmDeleteGrpReq *) sigBuff.sig;
  request_p->taskId          = VG_CI_TASK_ID;
  request_p->file            = vgLmData_p->phoneBook;
  request_p->adnRecordNumber = vgLmData_p->phoneIndex1;
  request_p->commandRef      = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmDeleteAnrReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_DELETE_ANR_REQ which deletes
*              additional number of the entry
*-------------------------------------------------------------------------*/

void vgSigApexLmDeleteAnrReq (const VgmuxChannelNumber entity)
{
  SignalBuffer       sigBuff = kiNullBuffer;
  ApexLmDeleteAnrReq *request_p;
  GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
  VgLmData           *vgLmData_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData_p = &generalContext_p->vgLmData;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_DELETE_ANR_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_DELETE_ANR_REQ,
                      sizeof (ApexLmDeleteAnrReq),
                      &sigBuff);

  request_p                  = (ApexLmDeleteAnrReq *) sigBuff.sig;
  request_p->taskId          = VG_CI_TASK_ID;
  request_p->file            = vgLmData_p->phoneBook;
  request_p->adnRecordNumber = vgLmData_p->phoneIndex1;
  request_p->anrEntry        = 0;
  request_p->commandRef      = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmDeleteSneReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_DELETE_SNE_REQ which deletes
*              second name of the entry
*-------------------------------------------------------------------------*/

void vgSigApexLmDeleteSneReq (const VgmuxChannelNumber entity)
{
  SignalBuffer       sigBuff = kiNullBuffer;
  ApexLmDeleteSneReq *request_p;
  GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
  VgLmData           *vgLmData_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData_p = &generalContext_p->vgLmData;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_DELETE_SNE_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_DELETE_SNE_REQ,
                      sizeof (ApexLmDeleteSneReq),
                      &sigBuff);

  request_p                  = (ApexLmDeleteSneReq *) sigBuff.sig;
  request_p->taskId          = VG_CI_TASK_ID;
  request_p->file            = vgLmData_p->phoneBook;
  request_p->adnRecordNumber = vgLmData_p->phoneIndex1;
  request_p->commandRef      = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}


/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmDeleteEmailReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_DELETE_EMAIL_REQ which deletes
*              email address of the entry
*-------------------------------------------------------------------------*/

void vgSigApexLmDeleteEmailReq (const VgmuxChannelNumber entity)
{
  SignalBuffer         sigBuff = kiNullBuffer;
  ApexLmDeleteEmailReq *request_p;
  GeneralContext_t     *generalContext_p = ptrToGeneralContext (entity);
  VgLmData             *vgLmData_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData_p = &generalContext_p->vgLmData;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_DELETE_EMAIL_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_DELETE_EMAIL_REQ,
                      sizeof (ApexLmDeleteEmailReq),
                      &sigBuff);

  request_p                  = (ApexLmDeleteEmailReq *) sigBuff.sig;
  request_p->taskId          = VG_CI_TASK_ID;
  request_p->file            = vgLmData_p->phoneBook;
  request_p->adnRecordNumber = vgLmData_p->phoneIndex1;
  request_p->emailEntry      = 0;
  request_p->commandRef      = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmGetPbSyncInfoReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_GET_PB_SYNC_INFO_REQ which read a
*              phonebook synchronization information
*-------------------------------------------------------------------------*/

void vgSigApexLmGetPbSyncInfoReq (const VgmuxChannelNumber entity)
{
  SignalBuffer            sigBuff = kiNullBuffer;
  ApexLmGetPbSyncInfoReq  *request_p;
  GeneralContext_t        *generalContext_p = ptrToGeneralContext (entity);
  VgLmData                *vgLmData_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData_p = &generalContext_p->vgLmData;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_GET_PB_SYNC_INFO_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_GET_PB_SYNC_INFO_REQ,
                      sizeof (ApexLmGetPbSyncInfoReq),
                      &sigBuff);

  request_p                  = (ApexLmGetPbSyncInfoReq *) sigBuff.sig;
  request_p->taskId          = VG_CI_TASK_ID;
  request_p->file            = vgLmData_p->phoneBook;
  request_p->commandRef      = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmGetSyncStatusReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_GET_SYNC_STATUS_REQ which ask AB for
*              the phonebooks synchronisation states and information.
*-------------------------------------------------------------------------*/
void vgSigApexLmGetSyncStatusReq (const VgmuxChannelNumber entity)
{
  SignalBuffer              sigBuff = kiNullBuffer;
  ApexLmGetSyncStatusReq *request_p;

  FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_LM_GET_SYNC_STATUS_CNF);

  KiCreateZeroSignal (SIG_APEX_LM_GET_SYNC_STATUS_REQ,
                      sizeof (ApexLmGetSyncStatusReq),
                      &sigBuff);

  request_p                  = (ApexLmGetSyncStatusReq *) sigBuff.sig;
  request_p->taskId          = VG_CI_TASK_ID;
  request_p->commandRef      = (Int16)entity;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmReadRecordUidReq
*
* Parameters:  VgmuxChannelNumber - entity to which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_READ_RECORD_UID_REQ signal to read
*              a record UID
*-------------------------------------------------------------------------*/
void vgSigApexLmReadRecordUidReq (const VgmuxChannelNumber entity)
{
    SignalBuffer              sigBuff = kiNullBuffer;
    ApexLmReadRecordUidReq   *request_p;
    GeneralContext_t         *generalContext_p = ptrToGeneralContext (entity);
    VgLmData                 *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal(   GENERAL,
                                entity,
                                SIG_APEX_LM_READ_RECORD_UID_CNF);

    KiCreateZeroSignal( SIG_APEX_LM_READ_RECORD_UID_REQ,
                        sizeof (ApexLmReadRecordUidReq),
                        &sigBuff);

    request_p                   = (ApexLmReadRecordUidReq *) sigBuff.sig;
    request_p->taskId           = VG_CI_TASK_ID;
    request_p->commandRef       = (Int16)entity;
    request_p->file             = vgLmData_p->phoneBook;
    request_p->readMode         = vgLmData_p->vgMupbsyncContext.uidReadMode;
    request_p->recordNumber     = vgLmData_p->vgMupbsyncContext.uidIndex;

    KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmReadAasReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_READ_AAS_REQ which requests
*              the additional number alpha string
*-------------------------------------------------------------------------*/
void vgSigApexLmReadAasReq (const VgmuxChannelNumber entity)
{
    SignalBuffer        sigBuff = kiNullBuffer;
    ApexLmReadAasReq   *request_p;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal(   GENERAL,
                                entity,
                                SIG_APEX_LM_READ_AAS_CNF);

    KiCreateZeroSignal( SIG_APEX_LM_READ_AAS_REQ,
                        sizeof (ApexLmReadAasReq),
                        &sigBuff);

    request_p                  = (ApexLmReadAasReq *) sigBuff.sig;
    request_p->taskId          = VG_CI_TASK_ID;
    request_p->file            = vgLmData_p->phoneBook;
    request_p->aasRecordNumber = vgLmData_p->aasInfo.aasIndex;
    request_p->commandRef      = (Int16)entity;

    KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmWriteAasReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_WRITE_AAS_REQ which stores
*              the additional number alpha string
*-------------------------------------------------------------------------*/
void vgSigApexLmWriteAasReq (const VgmuxChannelNumber entity)
{
    SignalBuffer        sigBuff = kiNullBuffer;
    ApexLmWriteAasReq  *request_p;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal(   GENERAL,
                                entity,
                                SIG_APEX_LM_WRITE_AAS_CNF);

    KiCreateZeroSignal( SIG_APEX_LM_WRITE_AAS_REQ,
                        sizeof (ApexLmWriteAasReq),
                        &sigBuff);

    request_p                  = (ApexLmWriteAasReq *) sigBuff.sig;
    request_p->taskId          = VG_CI_TASK_ID;
    request_p->file            = vgLmData_p->phoneBook;
    request_p->aasRecordNumber = vgLmData_p->aasInfo.aasIndex;

    memcpy (&request_p->aasRecord, &vgLmData_p->aasInfo.aasAlphaId, sizeof (LmAlphaId));
    request_p->commandRef      = (Int16)entity;

    KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmReadAasReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_DELETE_AAS_REQ which requests to delete
*              a AAS record
*-------------------------------------------------------------------------*/
void vgSigApexLmDeleteAasReq (const VgmuxChannelNumber entity)
{
    SignalBuffer        sigBuff = kiNullBuffer;
    ApexLmDeleteAasReq *request_p;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal(   GENERAL,
                                entity,
                                SIG_APEX_LM_DELETE_AAS_CNF);

    KiCreateZeroSignal( SIG_APEX_LM_DELETE_AAS_REQ,
                        sizeof (ApexLmDeleteAasReq),
                        &sigBuff);

    request_p                  = (ApexLmDeleteAasReq *) sigBuff.sig;
    request_p->taskId          = VG_CI_TASK_ID;
    request_p->file            = vgLmData_p->phoneBook;
    request_p->aasRecordNumber = vgLmData_p->aasInfo.aasIndex;
    request_p->commandRef      = (Int16)entity;

    KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmListAasReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_LIST_AAS_REQ which requests the informtation
*              on all exiting additional number alpha string
*-------------------------------------------------------------------------*/
void vgSigApexLmListAasReq (const VgmuxChannelNumber entity)
{
    SignalBuffer        sigBuff = kiNullBuffer;
    ApexLmListAasReq   *request_p;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal(   GENERAL,
                                entity,
                                SIG_APEX_LM_LIST_AAS_CNF);

    KiCreateZeroSignal( SIG_APEX_LM_LIST_AAS_REQ,
                        sizeof (ApexLmListAasReq),
                        &sigBuff);

    request_p              = (ApexLmListAasReq *) sigBuff.sig;
    request_p->taskId      = VG_CI_TASK_ID;
    request_p->startRecord = vgLmData_p->startRecord;
    request_p->file        = vgLmData_p->phoneBook;
    request_p->commandRef  = (Int16)entity;

    KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexLmClearAasReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEX_LM_CLEAR_AAS_REQ which requests to delete
*              all AAS record for a specified phonebook
*-------------------------------------------------------------------------*/
void vgSigApexLmClearAasReq (const VgmuxChannelNumber entity)
{
    SignalBuffer        sigBuff = kiNullBuffer;
    ApexLmClearAasReq  *request_p;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    FatalAssert (vgChManCheckHaveControl (CC_LIST_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal(   GENERAL,
                                entity,
                                SIG_APEX_LM_CLEAR_AAS_CNF);

    KiCreateZeroSignal( SIG_APEX_LM_CLEAR_AAS_REQ,
                        sizeof (ApexLmClearAasReq),
                        &sigBuff);

    request_p                  = (ApexLmClearAasReq *) sigBuff.sig;
    request_p->taskId          = VG_CI_TASK_ID;
    request_p->file            = vgLmData_p->phoneBook;
    request_p->commandRef      = (Int16)entity;

    KiSendSignal (cfRvAbTaskId, &sigBuff);
}

#endif /* FEA_PHONEBOOK */

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexSimReadSpnReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends a APEX_SIM_READ_SPN_REQ to BL in
*              order to read the SPN (Service Provider Name) from the SIM
*-------------------------------------------------------------------------*/

void vgSigApexSimReadSpnReq (const VgmuxChannelNumber entity)
{
  SignalBuffer      sendSignal = kiNullBuffer;
  ApexSimReadSpnReq *request_p;

  FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                             entity,
                              APEX_SIM_READ_SPN_CNF);

  KiCreateZeroSignal (APEX_SIM_READ_SPN_REQ,
                       sizeof (ApexSimReadSpnReq),
                        &sendSignal);

  request_p             = (ApexSimReadSpnReq *) sendSignal.sig;
  request_p->taskId     = VG_CI_TASK_ID;
  request_p->commandRef = (Int16)entity;

  KiSendSignal (TASK_BL_ID, &sendSignal);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexSimPinFunctionReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends a SIG_APEX_SIM_PIN_FUNCTION_REQ to BL in
*              order to check passcode for access to SIM functions
*
*-------------------------------------------------------------------------*/

void vgSigApexSimPinFunctionReq (const VgmuxChannelNumber entity)
{
  SignalBuffer          sigBuff = kiNullBuffer;
  ApexSimPinFunctionReq *request_p;
  GeneralContext_t      *generalContext_p = ptrToGeneralContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                            entity,
                            SIG_APEX_SIM_PIN_FUNCTION_CNF);

  KiCreateZeroSignal (SIG_APEX_SIM_PIN_FUNCTION_REQ,
                       sizeof (ApexSimPinFunctionReq),
                        &sigBuff);

  request_p              = (ApexSimPinFunctionReq *) sigBuff.sig;
  request_p->taskId      = VG_CI_TASK_ID;
  request_p->commandRef  = (Int16)entity;
  request_p->pinFunction = generalContext_p->pinFunction;
  request_p->pinKeyReference = generalContext_p->keyRef;
  if ((request_p->pinFunction == SIM_PIN_FUNCT_ENABLE) ||
       (request_p->pinFunction == SIM_PIN_FUNCT_DISABLE))
  {
    request_p->altPinKeyReference = generalContext_p->altPinKeyReference;
  }

  memcpy (request_p->pinValue.value,
           generalContext_p->password,
            SIM_PIN_LENGTH);

  memcpy (request_p->newPinValue.value,
           generalContext_p->newPassword,
            SIM_PIN_LENGTH);

  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexSimChvFunctionReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends a SIG_APEX_SIM_CHV_FUNCTION_REQ to BL in
*              order to check passcode for access to SIM functions
*
*-------------------------------------------------------------------------*/

void vgSigApexSimChvFunctionReq (const VgmuxChannelNumber entity)
{
  SignalBuffer          sigBuff = kiNullBuffer;
  ApexSimChvFunctionReq *request_p;
  GeneralContext_t      *generalContext_p = ptrToGeneralContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                             entity,
                              SIG_APEX_SIM_CHV_FUNCTION_CNF);

  KiCreateZeroSignal (SIG_APEX_SIM_CHV_FUNCTION_REQ,
                       sizeof (ApexSimChvFunctionReq),
                        &sigBuff);

  request_p              = (ApexSimChvFunctionReq *) sigBuff.sig;
  request_p->taskId      = VG_CI_TASK_ID;
  request_p->commandRef  = (Int16)entity;
  request_p->chvFunction = (SimChvFunction) generalContext_p->pinFunction;
  request_p->chvNum      = generalContext_p->chvNumber;

  memcpy (request_p->chvValue.value,
           generalContext_p->password,
            SIM_CHV_LENGTH);

  memcpy (request_p->newChvValue.value,
           generalContext_p->newPassword,
            SIM_CHV_LENGTH);

  KiSendSignal (TASK_BL_ID, &sigBuff);
}

#if defined (FEA_SIMLOCK)
/*--------------------------------------------------------------------------
*
* Function:    vgSigApexSimMepReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends a APEX_SIM_MEP_REQ to the background
*              for processing.  This enables/disables a Mobile Equipment
*              personalisation facility.
*-------------------------------------------------------------------------*/

void vgSigApexSimMepReq (const VgmuxChannelNumber entity)
{
  SignalBuffer          sigBuff = kiNullBuffer;
  ApexSimMepReq         *request_p;
  GeneralContext_t      *generalContext_p = ptrToGeneralContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                             entity,
                              APEX_SIM_MEP_CNF);

  KiCreateZeroSignal (APEX_SIM_MEP_REQ,
                       sizeof (ApexSimMepReq),
                        &sigBuff);

  request_p              = (ApexSimMepReq *) sigBuff.sig;
  request_p->taskId      = VG_CI_TASK_ID;
  request_p->commandRef  = (Int16)(VG_MEP_COMMAND_REF + entity);
  request_p->selector    = generalContext_p->mepSelector;
  request_p->operation   = generalContext_p->mepOperation;

  WarnCheck (generalContext_p->passwordLength <= MAX_PASSWORD_LENGTH, generalContext_p->passwordLength, 0, 0);
  if (generalContext_p->passwordLength > MAX_PASSWORD_LENGTH)
  {
    /*if we get there, it probably means that the password hasn' t been initialised...*/
    request_p->key.length = 0;
  }
  else
  {
    request_p->key.length = (Int8) generalContext_p->passwordLength;
    memcpy (request_p->key.number,
            generalContext_p->password,
            request_p->key.length);
  }

  if (CHANGE_PASSWORD == generalContext_p->mepOperation)
  {
    WarnCheck (generalContext_p->newPasswordLength <= MAX_PASSWORD_LENGTH, generalContext_p->newPasswordLength, 0, 0);
    if (generalContext_p->newPasswordLength > MAX_PASSWORD_LENGTH)
    {
      /*if we get there, it probably means that the password hasn' t been initialised...*/
      request_p->newkey.length = 0;
    }
    else
    {
      request_p->newkey.length = (Int8) generalContext_p->newPasswordLength;
      memcpy (request_p->newkey.number,
              generalContext_p->newPassword,
              request_p->newkey.length);
    }
  }

  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexSimMepStatusReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends a APEX_SIM_MEP_STATUS_REQ to the
*              background for processing.  This returns the status of the
*              Mobile Equipement Personalisation facilities.
*-------------------------------------------------------------------------*/

void vgSigApexSimMepStatusReq (const VgmuxChannelNumber entity)
{
  SignalBuffer        sigBuff = kiNullBuffer;
  ApexSimMepStatusReq *request_p;

  FatalAssert (vgChManCheckHaveControl (CC_SUBSCRIBER_IDENTITY_MODULE, entity) == TRUE);

  sendSsRegistrationSignal (GENERAL,
                             entity,
                              APEX_SIM_MEP_STATUS_CNF);

  KiCreateZeroSignal (APEX_SIM_MEP_STATUS_REQ,
                       sizeof (ApexSimMepStatusReq),
                        &sigBuff);

  request_p             = (ApexSimMepStatusReq *) sigBuff.sig;
  request_p->taskId     = VG_CI_TASK_ID;
  request_p->commandRef =  (Int16)(VG_MEP_COMMAND_REF + entity);
  KiSendSignal (TASK_BL_ID, &sigBuff);
}
#endif /* FEA_SIMLOCK */

#if defined(FEA_TEMP_INCLUDE_SIM_EMULATION) || defined (FEA_TEMP_ESIM_EMULATION)
void vgSigApexEmuSimCfgReq   (const VgmuxChannelNumber entity)
{
    SignalBuffer                   sigBuff = kiNullBuffer;
    ApexSimEmuSimReq               *request_p;
    SimLockGenericContext_t        *simLockGenericContext_p = ptrToSimLockGenericContext();

    sendSsRegistrationSignal(   SIM_LOCK,
                                entity,
                                SIG_APEX_SIM_EMUSIM_CNF);

    KiCreateZeroSignal( SIG_APEX_SIM_EMUSIM_REQ,
                        sizeof (ApexSimEmuSimReq),
                        &sigBuff);

    request_p                   = (ApexSimEmuSimReq *) sigBuff.sig;
    request_p->taskId           = VG_CI_TASK_ID;
    request_p->commandRef       = entity;
    request_p->ok               = simLockGenericContext_p->simEmulate;

    KiSendSignal (cfRvAbTaskId, &sigBuff);
}
#endif
/*--------------------------------------------------------------------------
*
* Function:    vgSigApexGlReadFeatureConfigReqTag
*
* Parameters:  VgmuxChannelNumber - entity to which sent request
*
* Returns:     Nothing
*
* Description: Sends a SIG_APEXGL_WRITE_FEATURE_CONFIG_REQ signal to read
*              the current feature configuration
*-------------------------------------------------------------------------*/
void vgSigApexGlWriteFeatureConfigReq (const VgmuxChannelNumber entity)
{
    SignalBuffer                    sigBuff = kiNullBuffer;
    ApexGlWriteFeatureConfigReq    *request_p;
    GeneralGenericContext_t        *generalGenericContext_p = ptrToGeneralGenericContext();
    FatalAssert (vgChManCheckHaveControl (CC_GENERAL_MODULE, entity) == TRUE);

    sendSsRegistrationSignal(   GENERAL,
                                entity,
                                SIG_APEXGL_WRITE_FEATURE_CONFIG_CNF);

    KiCreateZeroSignal( SIG_APEXGL_WRITE_FEATURE_CONFIG_REQ,
                        sizeof (ApexGlWriteFeatureConfigReq),
                        &sigBuff);

    request_p                   = (ApexGlWriteFeatureConfigReq *) sigBuff.sig;
    request_p->taskId           = VG_CI_TASK_ID;
    request_p->commandRef       = (Int16)entity;
    request_p->config           = generalGenericContext_p->vgMFtrCfgData.nvramCfgVar;

    KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:        vgSigN1CdEnterReq
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*
* Returns:         Nothing
*
* Description:     Function which sends signal for entering CALDEV state to L1
*-------------------------------------------------------------------------*/
void vgSigN1CdEnterReq (const VgmuxChannelNumber entity)
{
  SignalBuffer signalToSend = kiNullBuffer;

  sendSsRegistrationSignal(GENERAL,entity,SIG_N1CD_ENTER_CNF);

  /* Create signal. */
  KiCreateSignal (SIG_N1CD_ENTER_REQ, sizeof(N1CdEnterReq), &signalToSend);
  
  /* Populate signal. */
  signalToSend.sig->n1CdEnterReq.taskId = VG_CI_TASK_ID;  

  /* Send signal. */
  KiSendSignal (N1_MH_TASK_ID, &signalToSend);
}

/*--------------------------------------------------------------------------
*
* Function:        vgSigN1CdExitReq
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*
* Returns:         Nothing
*
* Description:     Function which sends signal for exiting CALDEV state to L1
*-------------------------------------------------------------------------*/
void vgSigN1CdExitReq (const VgmuxChannelNumber entity)
{
  SignalBuffer            signalToSend = kiNullBuffer;

  sendSsRegistrationSignal(GENERAL,entity,SIG_N1CD_EXIT_CNF);

  KiCreateZeroSignal (SIG_N1CD_EXIT_REQ,
                       sizeof(N1CdExitReq),
                        &signalToSend);

  KiSendSignal (N1_MH_TASK_ID, &signalToSend);
}

/*--------------------------------------------------------------------------
*
* Function:        vgSigN1CdRfTestReq
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*
* Returns:         Nothing
*
* Description:     Function which sends signal for calibration to L1.
*-------------------------------------------------------------------------*/
void vgSigN1CdRfTestReq (const VgmuxChannelNumber entity)
{
  SignalBuffer            signalToSend = kiNullBuffer;
  N1CdRfTestReq           *request_p;
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMcalContext           *mcalContext_p            = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMcalContext);

  sendSsRegistrationSignal(GENERAL,entity,SIG_N1CD_NRF_TEST_CNF);

  KiCreateZeroSignal (SIG_N1CD_NRF_TEST_REQ,
                       sizeof(N1CdRfTestReq),
                        &signalToSend);

  request_p         = (N1CdRfTestReq *) signalToSend.sig;
  request_p->token  = (uint16_t)mcalContext_p->token;
  request_p->cmd    = (uint32_t)mcalContext_p->command;
  memcpy (&(request_p->param),mcalContext_p->data,mcalContext_p->length);
  
  KiSendSignal (N1_MH_TASK_ID, &signalToSend);
}

/*--------------------------------------------------------------------------
*
* Function:        vgSigN1CdIdcTestReq
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*
* Returns:         Nothing
*
* Description:     Function which sends signal for IDC test to L1.
*-------------------------------------------------------------------------*/
void vgSigN1CdIdcTestReq(const VgmuxChannelNumber entity)
{
  SignalBuffer            signalToSend = kiNullBuffer;
  N1CdIdcTestReq           *request_p;
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMcalContext           *mcalContext_p            = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMcalContext);

  sendSsRegistrationSignal(GENERAL,entity,SIG_N1CD_IDC_TEST_CNF);

  KiCreateZeroSignal (SIG_N1CD_IDC_TEST_REQ,
                       sizeof(N1CdIdcTestReq),
                        &signalToSend);

  request_p         = (N1CdIdcTestReq *) signalToSend.sig;
  request_p->token  = (uint16_t)mcalContext_p->token;
  request_p->cmd    = (uint32_t)mcalContext_p->command;
  memcpy (&(request_p->param),mcalContext_p->data,mcalContext_p->length);
  
  KiSendSignal (N1_MH_TASK_ID, &signalToSend);
}

/*--------------------------------------------------------------------------
*
* Function:        vgSigN1TstAlgTuningSetReq
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*
* Returns:         Nothing
*
* Description:     Function which sends signal for L1 testing to L1.
*-------------------------------------------------------------------------*/
void vgSigN1TstAlgTuningSetReq (const VgmuxChannelNumber entity)
{
  SignalBuffer            signalToSend = kiNullBuffer;
  N1TstAlgTuningSetReq    *request_p;
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMcalContext           *mcalContext_p            = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMcalContext);

  KiCreateZeroSignal (SIG_N1TST_ALG_TUNING_SET_REQ,
                       sizeof(N1TstAlgTuningSetReq),
                        &signalToSend);

  request_p         = (N1TstAlgTuningSetReq *) signalToSend.sig;
  request_p->token  = (uint16_t)mcalContext_p->token;
  request_p->cmd    = (uint32_t)mcalContext_p->command;
  memcpy (&(request_p->param),mcalContext_p->data,mcalContext_p->length);
  
  KiSendSignal (N1_MH_TASK_ID, &signalToSend);
}


/*************************************************************************
*
* Function:     vgSigApexMmWriteMobileIdReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/
void vgSigApexMmWriteMobileIdReq(const VgmuxChannelNumber entity)
{
    SignalBuffer                  signalBuffer       = kiNullBuffer;
    ApexMmWriteMobileIdReq       *request_p;
    Int16                         mobileIdLen;

    GeneralContext_t     *generalContext_p   = ptrToGeneralContext(entity);
    VgMcgsnData           *vgReqMcgsnData    = &generalContext_p->mcgsnData;

    sendSsRegistrationSignal (GENERAL,
      entity,
      SIG_APEX_MM_WRITE_MOBILE_ID_CNF);

    KiCreateZeroSignal (SIG_APEX_MM_WRITE_MOBILE_ID_REQ,
      sizeof (ApexMmWriteMobileIdReq),
      &signalBuffer);

    request_p                    = &signalBuffer.sig->apexMmWriteMobileIdReq;
    request_p->taskId            = VG_CI_TASK_ID;
    request_p->digitImeiArraySize = vgReqMcgsnData->digitImeiArraySize;
    request_p->digitSNArraySize   = vgReqMcgsnData->digitSNArraySize;

    if (VG_MCGSN_SNT_IMEI == vgReqMcgsnData->mcgsnSnt)
    {
        mobileIdLen = vgReqMcgsnData->digitImeiArraySize;
    }
    else
    {
        mobileIdLen = MAX_UE_ID_LENGTH;
    }

    memcpy(request_p->digitMobileId, vgReqMcgsnData->digitMobileId, mobileIdLen);

    KiSendSignal (TASK_BL_ID, &signalBuffer);
} /* vgSigApexMmWriteMobileIdReq */

/*************************************************************************
*
* Function:     vgSigApexMmReadMobileIdReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/
void vgSigApexMmReadMobileIdReq(const VgmuxChannelNumber entity)
{
   SignalBuffer                  signalBuffer       = kiNullBuffer;
   ApexMmReadMobileIdReq       *request_p;
   GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
   sendSsRegistrationSignal (GENERAL,
     entity,
     SIG_APEX_MM_READ_MOBILE_ID_CNF);

   KiCreateZeroSignal (SIG_APEX_MM_READ_MOBILE_ID_REQ,
     sizeof (ApexMmReadMobileIdReq),
     &signalBuffer);

   request_p                    = &signalBuffer.sig->apexMmReadMobileIdReq;
   switch(getCommandId(entity))
   {
       case VG_AT_GN_GSN:
       {
         request_p->name =NV_IMEI;
         break;
       }
       case VG_AT_GN_CGSN:
       {

         switch (generalContext_p->cgsnSnt)
         {
           case VG_CGSN_SNT_SN:
           {
             request_p->name = NV_SN;
             break;
           }
           case VG_CGSN_SNT_IMEI:
           {
             request_p->name = NV_IMEI;
             break;
           }
           case VG_CGSN_SNT_IMEISV:
           {
             request_p->name = NV_IMEISV;
             break;
           }
           case VG_CGSN_SNT_SVN:
           {
             request_p->name = NV_SVN;
             break;
           }
           default:
           {
             FatalParam(entity, generalContext_p->cgsnSnt, 0);
             break;
           }
         }
         
       break;
       }  
       default:
       {
           FatalParam( getCommandId( entity), 0, 0);
       }
       break;
   }              
   
   request_p                    = &signalBuffer.sig->apexMmReadMobileIdReq;
   request_p->taskId            = VG_CI_TASK_ID;
  
   KiSendSignal (TASK_BL_ID, &signalBuffer);
} /* vgSigApexMmWriteMobileIdReq */

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

