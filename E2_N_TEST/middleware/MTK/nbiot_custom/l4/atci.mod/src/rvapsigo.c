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
 * Outgoing signal handlers for AP Bridge Sub-System.
 *
 * Procedures simply send a signal. The signal is created, its expected
 * returning signal is registered, contents filled and then it is sent.
 **************************************************************************/

#define MODULE_NAME "RVAPSIGO"


/***************************************************************************
* Include Files
***************************************************************************/

#include <system.h>
#include <gkisig.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvapsigo.h>
#include <rvchman.h>
#include <rvcfg.h>
#include <rvccut.h>
#include <gkimem.h>

/***************************************************************************
* Signal definitions
***************************************************************************/

union Signal
{
  CiApbRegisterAtCommandRsp ciApbRegisterAtCommandRsp;
  CiApbAtCommandReq         ciApbAtCommandReq;
  CiApbOpenDataConnReq      ciApbOpenDataConnReq;
  CiApbCloseDataConnReq     ciApbCloseDataConnReq;
};

/***************************************************************************
* Macro Definition
***************************************************************************/
/*Define the length of "AT" string.*/
#define LENGTH_OF_AT_STRING 2
#define LENGTH_OF_NULL_STRING 1
#define LENGTH_OF_MAX_AT      1460

/***************************************************************************
* Local Functions Prototype
***************************************************************************/

#ifdef ENABLE_AP_BRIDGE_FEATURE_DEBUG
static Boolean setAtCmdString(Int8* pString, Int32 length, CommandId_t cmdId);
#endif
static Int32 getAtCmdRequestType(ExtendedOperation_t operation);

/***************************************************************************
* Local Functions Implementation
***************************************************************************/
#ifdef ENABLE_AP_BRIDGE_FEATURE_DEBUG
/*--------------------------------------------------------------------------
*
* Function:    setAtCmdString
*
* Parameters:  pString - points the buffer used to save AT command's header.
*              length - buffer length.
*              cmdId - the command id for specific AT Command.
* Returns:     TRUE : the command's header is successfully putted into the buffer.
*
* Description: Put the command's header into pString buffer.
*-------------------------------------------------------------------------*/
static Boolean setAtCmdString(Int8* pString, Int32 length, CommandId_t cmdId)
{
  ApBridgeGenericContext_t* apBridgeGenericContext_p = ptrToApBridgeGenericContext();
  AtCmdControl* atCmdControl_p = apBridgeGenericContext_p->apAtCommandTable_p;
  Int32 index = 0;
  Boolean result = FALSE;
  Int32 atCmdLength = 0;

  for(index = 0; index < apBridgeGenericContext_p->registeredCmdCount; index++)
  {
    if(atCmdControl_p->commandId == cmdId)
    {
      atCmdLength = strlen(atCmdControl_p->string);
      /*Guarantee the pString is not overflow.*/
      if(atCmdLength > length)
      {
        atCmdLength = length-1;
      }
      memcpy(pString,atCmdControl_p->string,atCmdLength);
      *(pString+atCmdLength) = '\0';
      result = TRUE;
    }
    atCmdControl_p++;
  }

  return result;
}
#endif
/*--------------------------------------------------------------------------
*
* Function:    getAtCmdRequestType
*
* Parameters:  operation - AT command's operation type defined in ATCI.
*
* Returns:     the corresponding operation value defined in AP Bridge.
*
* Description: convert the operation type value from ATCI to AP Bridge.
*-------------------------------------------------------------------------*/
static Int32 getAtCmdRequestType(ExtendedOperation_t operation)
{
   Int32 result = 0xFF;/*invalid type.*/
   switch(operation)
   {
     case EXTENDED_ACTION:
     {
       result = APBRIDGE_REQUEST_TYPE_ACTION;
       break;
     }
     case EXTENDED_QUERY:
     {
       result = APBRIDGE_REQUEST_TYPE_READ;
       break;
     }
     case EXTENDED_ASSIGN:
     {
       result = APBRIDGE_REQUEST_TYPE_SET;
       break;
     }
     case EXTENDED_RANGE:
     {
       result = APBRIDGE_REQUEST_TYPE_TEST;
       break;
     }
     default:
     {
       FatalAssert(0);
       break;
     }
   }
   return result;
}


/***************************************************************************
* Global Functions Implementation.
***************************************************************************/

/*--------------------------------------------------------------------------
*
* Function:    vgSigCiapbRegisterAtCommandRsp
*
* Parameters:  request_p - points to response data buffer.
*
* Returns:     Nothing
*
* Description: Function which sends a SIG_CIAPB_REGISTER_AT_COMMAND_RSP
*              to the AP Bridge to inform the register results to AP modain.
*-------------------------------------------------------------------------*/
void vgSigCiapbRegisterAtCommandRsp(CiApbRegisterAtCommandRsp* request_p)
{

  SignalBuffer sigBuff = kiNullBuffer;
  CiApbRegisterAtCommandRsp* pRsp = PNULL;

  KiCreateZeroSignal (SIG_CIAPB_REGISTER_AT_COMMAND_RSP,
                      sizeof (CiApbRegisterAtCommandRsp),
                      &sigBuff);

  pRsp = (CiApbRegisterAtCommandRsp*)sigBuff.sig;
  pRsp->baseCmdId= request_p->baseCmdId;
  pRsp->registeredCount= request_p->registeredCount;
  pRsp->success = request_p->success;
  KiSendSignal (VG_APB_TASK_ID, &sigBuff);
}
/*--------------------------------------------------------------------------
*
* Function:    vgSigCiapbAtCommandReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*              pCmdString - the whole data for the specific AT Command.
*              length - the length of the command's data.
* Returns: Identify whether the signal is sent out or not.
*
* Description: Sending a SIG_CIAPB_AT_COMMAND_REQ to the AP Bridge to
*              deliver the AT Command request to AP modain to be executed.
*-------------------------------------------------------------------------*/
Boolean vgSigCiapbAtCommandReq(VgmuxChannelNumber entity,
                               const Char* pCmdString,
                               Int32 length)
{
  SignalBuffer sigBuff                               = kiNullBuffer;
  CiApbAtCommandReq* request_p                       = PNULL;
  CiApbTlv* tlv_p                                    = PNULL;
  ApBridgeContext_t* apBridgeContext_p               = ptrToApBridgeContext (entity);
  Int8* value_p =PNULL;


  if (length > LENGTH_OF_MAX_AT)
  {
    return FALSE;
  }

  KiCreateZeroSignal (SIG_CIAPB_AT_COMMAND_REQ,
                      sizeof (CiApbAtCommandReq),
                      &sigBuff);
  request_p = (CiApbAtCommandReq *)sigBuff.sig;
  request_p->channelId = entity;
#ifdef ENABLE_AP_BRIDGE_FEATURE_DEBUG
  if(setAtCmdString(request_p->atString,CIAPB_MAX_AT_DATA_LENGTH, getCommandId(entity)) == FALSE)
  {
    KiDestroySignal(&sigBuff);
    return FALSE;
  }
#endif
  request_p->commandId = getCommandId(entity);
  request_p->requestType = getAtCmdRequestType(apBridgeContext_p->operation);
  request_p->numberOfTlvs = 1U;
  tlv_p = request_p->tlv + 0;
  tlv_p->type = APBRIDGE_TLV_TYPE_STRING;
  tlv_p->length = length + LENGTH_OF_AT_STRING + LENGTH_OF_NULL_STRING;
#ifdef ENABLE_AP_BRIDGE_UNIT_TEST
  memcpy(tlv_p->data.string_p, "AT", LENGTH_OF_AT_STRING);
  memcpy(tlv_p->data.string_p + LENGTH_OF_AT_STRING, pCmdString, length);
  *(tlv_p->data.string_p + tlv_p->length - LENGTH_OF_NULL_STRING) = 0;
#else
  KiAllocMemory(tlv_p->length,(void**)(&value_p));
  if(PNULL == value_p)
  {
    KiDestroySignal(&sigBuff);
    return FALSE;
  }
  memcpy(value_p, "AT", LENGTH_OF_AT_STRING);
  memcpy(value_p + LENGTH_OF_AT_STRING, pCmdString, length);
  *(value_p + tlv_p->length - LENGTH_OF_NULL_STRING) = 0;
  /*The allocated memory will be handled by AP Bridge.*/
  tlv_p->data.string_p = value_p;
#endif
  KiSendSignal (VG_APB_TASK_ID, &sigBuff);
  return TRUE;
}
/*--------------------------------------------------------------------------
*
* Function:    vgSigCiapbOpenDataConnReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends a SIG_CIAPB_OPEN_DATA_CONN_REQ
*              to the AP Bridge to configure the data channel between MUX and AP Bridge.
*-------------------------------------------------------------------------*/
void vgSigCiapbOpenDataConnReq(VgmuxChannelNumber entity)
{
  SignalBuffer         sigBuff = kiNullBuffer;
  CiApbOpenDataConnReq *request_p = PNULL;

  KiCreateZeroSignal (SIG_CIAPB_OPEN_DATA_CONN_REQ,
                      sizeof (CiApbOpenDataConnReq),
                      &sigBuff);
  request_p                = (CiApbOpenDataConnReq *) sigBuff.sig;
  request_p->channelNumber = (Int32)entity;
  KiSendSignal (VG_APB_TASK_ID, &sigBuff);
}
/*--------------------------------------------------------------------------
*
* Function:    vgSigCiapbCloseDataConnReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends a SIG_CIAPB_CLOSE_DATA_CONN_REQ
*              to the AP Bridge to close the data channel between MUX and AP Bridge.
*-------------------------------------------------------------------------*/
void vgSigCiapbCloseDataConnReq(VgmuxChannelNumber entity)
{
  SignalBuffer          sigBuff = kiNullBuffer;
  CiApbCloseDataConnReq *request_p = PNULL;
  KiCreateZeroSignal (SIG_CIAPB_CLOSE_DATA_CONN_REQ,
                        sizeof (CiApbCloseDataConnReq),
                        &sigBuff);
  request_p                = (CiApbCloseDataConnReq *) sigBuff.sig;
  request_p->channelNumber = (Int32)entity;
  KiSendSignal (VG_APB_TASK_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSigCiapbDataModeTempDeactived
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends a SIG_CIAPB_DATA_MODE_TEMP_DEACTIVATED_IND
*              to the AP Bridge.
*-------------------------------------------------------------------------*/
void vgSigCiapbDataModeTempDeactived(VgmuxChannelNumber entity)
{
  SignalBuffer          sigBuff = kiNullBuffer;
  ApbDataModeTempDeactivatedInd *request_p = PNULL;
  KiCreateZeroSignal (SIG_CIAPB_DATA_MODE_TEMP_DEACTIVATED_IND,
                        sizeof (ApbDataModeTempDeactivatedInd),
                        &sigBuff);
  request_p                = (ApbDataModeTempDeactivatedInd *) sigBuff.sig;
  request_p->channelNumber = (Int32)entity;
  KiSendSignal (VG_APB_TASK_ID, &sigBuff);
}
/*--------------------------------------------------------------------------
*
* Function:    vgSigCiapbDataModeResumed
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends a SIG_CIAPB_DATA_MODE_RESUMED_IND
*              to the AP Bridge.
*-------------------------------------------------------------------------*/
void vgSigCiapbDataModeResumed(VgmuxChannelNumber entity)
{
  SignalBuffer          sigBuff = kiNullBuffer;
  ApbDataModeResumedInd *request_p = PNULL;
  KiCreateZeroSignal (SIG_CIAPB_DATA_MODE_RESUMED_IND,
                        sizeof (ApbDataModeResumedInd),
                        &sigBuff);
  request_p                = (ApbDataModeResumedInd *) sigBuff.sig;
  request_p->channelNumber = (Int32)entity;
  KiSendSignal (VG_APB_TASK_ID, &sigBuff);
}
