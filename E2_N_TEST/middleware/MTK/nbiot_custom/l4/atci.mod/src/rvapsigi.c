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
 * Incoming signal handlers for AP Bridge Sub-System.
 **************************************************************************/

#define MODULE_NAME "RVAPSIGI"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <stdio.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvcrhand.h>
#include <rvcrerr.h>
#include <rvcimxut.h>
#include <gkisig.h>
#include <ciapb_sig.h>
#include <rvaput.h>
#include <rvapsigo.h>
#include <gkimem.h>

/***************************************************************************
* Signal definitions
***************************************************************************/

union Signal
{
  CiApbRegisterAtCommandInd ciApbRegisterAtCommandInd;
  CiApbAtResponseInd        ciApbAtResponseInd;
  ApbDataModeResumeReq      apbDataModeResumeReq;
};

/***************************************************************************
* Macro definitions
***************************************************************************/
#ifndef APB_ERROR_NOT_USED
#define APB_ERROR_NOT_USED 0xFFFFFFFF
#endif
#ifndef APB_INVALID_CMD_ID
#define APB_INVALID_CMD_ID 0xFFFFFFFF
#endif
#define APB_URC_DEFAULT_CHANNEL_ID 0
/***************************************************************************
* Local Functions Prototypes
***************************************************************************/
static ResultCode_t apbGetResultCode(Int32 code);
static ResultCode_t apbGetErrorCode(Int32 code);
static void processResultCode(VgmuxChannelNumber entity, CommandId_t cmd_id, ResultCode_t resultCode);
/***************************************************************************
* Local Functions Implementations
***************************************************************************/
/*----------------------------------------------------------------------------
*
* Function:    apbGetErrorCode
*
* Parameters:  code - the error code defined in AT command response message.
*
* Returns:     The error code defined in ATCI.
*
* Description: Translating the Int32 error code to ATCI internal result type.
*----------------------------------------------------------------------------*/
static ResultCode_t apbGetErrorCode(Int32 code)
{
  ResultCode_t result = START_OF_RESULT_CODES;
  switch(code)
  {
    case APBRIDGE_ERROR_CME_PHONE_FAILURE:
    {
      result = VG_CME_PHONE_FAILURE;
      break;
    }
    case APBRIDGE_ERROR_CME_OPERATION_NOT_ALLOWED:
    {
      result = VG_CME_OPERATION_NOT_ALLOWED;
      break;
    }
    case APBRIDGE_ERROR_CME_OPERATION_NOT_SUPPORTED:
    {
      result = VG_CME_OPERATION_NOT_SUPPORTED;
      break;
    }
    case APBRIDGE_ERROR_CME_SIM_NOT_INSERTED:
    {
      result = VG_CME_SIM_NOT_INSERTED;
      break;
    }
    case APBRIDGE_ERROR_CME_INCORRECT_PASSWORD:
    {
      result = VG_CME_INCORRECT_PASSWORD;
      break;
    }
    case APBRIDGE_ERROR_CME_MEMORY_FULL:
    {
      result = VG_CME_MEMORY_FULL;
      break;
    }
    case APBRIDGE_ERROR_CME_MEMORY_FAILURE:
    {
      result = VG_CME_MEMORY_FAILURE;
      break;
    }
    case APBRIDGE_ERROR_CME_LONG_TEXT:
    {
      result = VG_CME_LONG_TEXT;
      break;
    }
    case APBRIDGE_ERROR_CME_INVALID_TEXT_CHARS:
    {
      result = VG_CME_INVALID_TEXT_CHARS;
      break;
    }
    case APBRIDGE_ERROR_CME_NO_NETWORK_SERVICE:
    {
      result = VG_CME_NO_NETWORK_SERVICE;
      break;
    }
    case APBRIDGE_ERROR_CME_NETWORK_TIMEOUT:
    {
      result = VG_CME_NETWORK_TIMEOUT;
      break;
    }
    case APBRIDGE_ERROR_CME_EMERGENCY_ONLY:
    {
      result = VG_CME_EMERGENCY_ONLY;
      break;
    }
    case APBRIDGE_ERROR_CME_UNKNOWN:
    {
      result = VG_CME_UNKNOWN;
      break;
    }
    case APBRIDGE_ERROR_CME_PSD_SERVICES_NOT_ALLOWED:
    {
      result = VG_CME_PSD_SERVICES_NOT_ALLOWED;
      break;
    }
    case APBRIDGE_ERROR_CME_PLMN_NOT_ALLOWED:
    {
      result = VG_CME_PLMN_NOT_ALLOWED;
      break;
    }
    case APBRIDGE_ERROR_CME_LOCATION_AREA_NOT_ALLOWED:
    {
      result = VG_CME_LOCATION_AREA_NOT_ALLOWED;
      break;
    }
    case APBRIDGE_ERROR_CME_ROAMING_NOT_ALLOWED:
    {
      result = VG_CME_ROAMING_NOT_ALLOWED;
      break;
    }
    case APBRIDGE_ERROR_CME_SERVICE_OPTION_NOT_SUPPORTED:
    {
      result = VG_CME_SERVICE_OPTION_NOT_SUPPORTED;
      break;
    }
    case APBRIDGE_ERROR_CME_SERVICE_OPTION_NOT_SUBSCRIBED:
    {
      result = VG_CME_SERVICE_OPTION_NOT_SUBSCRIBED;
      break;
    }
    case APBRIDGE_ERROR_CME_SERVICE_OPTION_OUT_OF_ORDER:
    {
      result = VG_CME_SERVICE_OPTION_OUT_OF_ORDER;
      break;
    }
    case APBRIDGE_ERROR_CME_UNSPECIFIED_PSD_ERROR:
    {
      result = VG_CME_UNSPECIFIED_PSD_ERROR;
      break;
    }
    case APBRIDGE_ERROR_CME_PDP_AUTHENTIFICATION_ERROR:
    {
      result = VG_CME_PDP_AUTHENTIFICATION_ERROR;
      break;
    }
    default:
    {
      break;
    }
  }

  return result;
}
/*----------------------------------------------------------------------------
*
* Function:    apbGetResultCode
*
* Parameters:  code - the result code defined in AT command response message.
*
* Returns:     The result code defined in ATCI.
*
* Description: Translating the Int32 result code to ATCI internal result type.
*----------------------------------------------------------------------------*/
static ResultCode_t apbGetResultCode(Int32 code)
{
  ResultCode_t result = START_OF_RESULT_CODES;
  switch(code)
  {
    case APBRIDGE_RESULT_OK:
    {
      result = RESULT_CODE_OK;
      break;
    }
    case APBRIDGE_RESULT_CONNECT:
    {
      result = RESULT_CODE_CONNECT;
      break;
    }
    case APBRIDGE_RESULT_RING:
    {
      result = RESULT_CODE_RING;
      break;
    }
    case APBRIDGE_RESULT_NO_CARRIER:
    {
      result = RESULT_CODE_NO_CARRIER;
      break;
    }
    case APBRIDGE_RESULT_ERROR:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
    case APBRIDGE_RESULT_NO_DIALTONE:
    {
      result = RESULT_CODE_NO_DIALTONE;
      break;
    }
    case APBRIDGE_RESULT_BUSY:
    {
      result = RESULT_CODE_BUSY;
      break;
    }
    case APBRIDGE_RESULT_NO_ANSWER:
    {
      result = RESULT_CODE_NO_ANSWER;
      break;
    }
    case APBRIDGE_RESULT_PROCEEDING:
    {
      result = RESULT_CODE_PROCEEDING;
      break;
    }
    case APBRIDGE_RESULT_CUSTOM_ERROR:
    {
      result = RESULT_CODE_NULL;
      break;
    }
    case APBRIDGE_RESULT_CUSTOM_CONNECT:
    {
      result = RESULT_CODE_CUSTOM_CONNECT;
      break;
    }
    case APBRIDGE_RESULT_UNSOLICITED:
    default:
    {
      break;
    }
  }

  return result;
}

/*----------------------------------------------------------------------------
*
* Function:    processResultCode
*
* Parameters:  code   - the result code defined in AT command response message.
*              entity - the AT command's channel ID.
*
* Returns:     The result code defined in ATCI.
*
* Description: According to the result code , check whether it needs to switch
*              to data mode or switch back to command mode.
*----------------------------------------------------------------------------*/
static void processResultCode(VgmuxChannelNumber entity, CommandId_t cmd_id, ResultCode_t resultCode)
{
  ApBridgeGenericContext_t* apBridgeGenericContext_p = ptrToApBridgeGenericContext();
  ApBridgeContext_t* apBridgeContext_p = ptrToApBridgeContext( entity );
  Int32 index = 0;
  ApBridgeDataModeContext_t* apBridgeDataModeContext_p = PNULL;
  Boolean found = FALSE;
  if ((resultCode == RESULT_CODE_CONNECT)
      ||(resultCode == RESULT_CODE_CUSTOM_CONNECT))
  {
    for(index = 0; index < APBRIDGE_DATA_MODE_CHANNEL_COUNT; index++)
    {
      apBridgeDataModeContext_p = apBridgeGenericContext_p->dataModeCtx + index;
      if (apBridgeDataModeContext_p->dataModeState == RV_APB_DATA_MODE_DEACTIVATED)
      {
        apBridgeDataModeContext_p->dataModeState = RV_APB_DATA_MODE_ACTIVATING;
        found = TRUE;
        break;
      }
    }

    if (TRUE == found)
    {
      setConnectionType(entity,APBRIDGE_CONNECTION);
      apBridgeDataModeContext_p->dataModeCmdId = getCommandId(entity);
      apBridgeDataModeContext_p->channelIdInDataMode = entity;
      apBridgeDataModeContext_p->dataModeResultCode = resultCode;
      apBridgeDataModeContext_p->dataModeOperationMode = apBridgeContext_p->operation;
    }
  }
}
/***************************************************************************
* Global Functions Implementation
***************************************************************************/
/*----------------------------------------------------------------------------
*
* Function:    vgSigCiapbRegisterAtCommandInd
*
* Parameters:  signalBuffer - points to a incoming  data structure
*
* Returns:     Nothing
*
* Description: Handles the SIG_CIAPB_REGISTER_AT_COMMAND_IND signal received
*              from AP Bridge. In the indication, several AT command's headers
*              are included in the message. These AT command will be registered
*              into AP Bridge subsystem.
*----------------------------------------------------------------------------*/
void  vgSigCiapbRegisterAtCommandInd (const SignalBuffer *signalBuffer)
{
  CommandId_t cmdId = VG_AT_LAST_CODE;
  AtCmdControl* pCmdControl = PNULL;
  CiApbRegisterAtCommandInd* sig_p = PNULL;
  Int8* pCommandsData = PNULL;
  ApBridgeGenericContext_t* apBridgeGenericContext_p = ptrToApBridgeGenericContext();
  Int32 index = 0;
  Int32 registeredCount = 0;
  CiApbRegisterAtCommandRsp registerCmdRsp;

  /*As signalBuffer is located in stack memory, please don't free the buffer. */

  registerCmdRsp.success = FALSE;
  registerCmdRsp.baseCmdId = 0;
  registerCmdRsp.registeredCount = 0;

  sig_p = &(signalBuffer->sig->ciApbRegisterAtCommandInd);

  if (sig_p->totalCmdCount > AP_BRIDGE_MAX_AT_CMD_COUNT)
  {
    registerCmdRsp.success = FALSE;
#ifndef ENABLE_AP_BRIDGE_UNIT_TEST
    KiFreeMemory((void**)(&(sig_p->pAtCmdsData)));
#endif
    vgSigCiapbRegisterAtCommandRsp(&registerCmdRsp);
    return;
  }

  /*When the first register indication msg is received,
    the tatal needed memory for all the commands will be allocated at one time.*/
  if (PNULL == apBridgeGenericContext_p->apAtCommandTable_p)
  {
    apBridgeGenericContext_p->apAtCommandTableItemSize = sig_p->totalCmdCount + 1;
    KiAllocMemory( (apBridgeGenericContext_p->apAtCommandTableItemSize)*(sizeof(AtCmdControl))
                    , &(apBridgeGenericContext_p->apAtCommandTable_p));
    if (PNULL == apBridgeGenericContext_p->apAtCommandTable_p)
    {
      registerCmdRsp.success = FALSE;
#ifndef ENABLE_AP_BRIDGE_UNIT_TEST
      KiFreeMemory((void**)(&(sig_p->pAtCmdsData)));
#endif
      vgSigCiapbRegisterAtCommandRsp(&registerCmdRsp);
      return;
    }
  }

#ifdef ENABLE_AP_BRIDGE_UNIT_TEST
  KiAllocMemory(sig_p->bufferLen, &pCommandsData);
  memcpy(pCommandsData, sig_p->pAtCmdsData, sig_p->bufferLen);
#else
  pCommandsData = sig_p->pAtCmdsData;
#endif
  cmdId = VG_AT_AP_BRIDGE_BASE + apBridgeGenericContext_p->registeredCmdCount;
  pCmdControl = (AtCmdControl*)(apBridgeGenericContext_p->apAtCommandTable_p);
  pCmdControl = pCmdControl + apBridgeGenericContext_p->registeredCmdCount;
  pCmdControl->string = pCommandsData;
  pCmdControl->commandId = cmdId;
  pCmdControl->procFunc = apBridgeCommandHandler;
  pCmdControl->accessMask = AT_CMD_ACCESS_NONE;
  registeredCount++;

  for(index = 0; index < sig_p->bufferLen; index++)
  {
     if ( ((*(pCommandsData + index)) == '\0')
           && ((index + 1) != sig_p->bufferLen))
     {
       /*Another AT command beginning.*/
       pCmdControl++;
       cmdId++;
       pCmdControl->string = pCommandsData + index + 1;
       pCmdControl->commandId = cmdId;
       pCmdControl->procFunc = apBridgeCommandHandler;
       pCmdControl->accessMask = AT_CMD_ACCESS_NONE;
       registeredCount++;
     }
  }

  pCmdControl++;
  pCmdControl->string = PNULL;
  pCmdControl->commandId = VG_AT_LAST_CODE;
  pCmdControl->procFunc = PNULL;
  pCmdControl->accessMask = AT_CMD_ACCESS_NONE;

  registerCmdRsp.success = TRUE;
  registerCmdRsp.baseCmdId = VG_AT_AP_BRIDGE_BASE + apBridgeGenericContext_p->registeredCmdCount;
  registerCmdRsp.registeredCount = registeredCount;
  apBridgeGenericContext_p->registeredCmdCount = apBridgeGenericContext_p->registeredCmdCount + registeredCount;

  /*When the at command is registered successfully, the buffer which sig_p->pAtCmdsData
    points to can not be freed. The buffer will keep in the memory used by command control table.*/
  vgSigCiapbRegisterAtCommandRsp(&registerCmdRsp);

  if (apBridgeGenericContext_p->registeredCmdCount == 
      (apBridgeGenericContext_p->apAtCommandTableItemSize - 1))
  {
    /*Processing the buffered AT commands.*/
    apBridgeGenericContext_p->cmdRegistered = TRUE;
    for(index = 0; index < apBridgeGenericContext_p->pendingCmdCount; index++)
    {
      AtCmdControl* pCmdTbl = (AtCmdControl*)(apBridgeGenericContext_p->apAtCommandTable_p);
      (void)parseCommandBuffer (pCmdTbl, apBridgeGenericContext_p->pendingCmdChanelIds[index]);
    }
  }
}
/*----------------------------------------------------------------------------
*
* Function:    vgSigCiapbAtResponseInd
*
* Parameters:  signalBuffer - Pointer to a incoming  data structure
*              entity - channel ID for the specific AT command.
* Returns:     Nothing
*
* Description: Handles the SIG_CIAPB_AT_RESPONSE_IND signal received
*              from AP Bridge. This is the AT Command execution result
*              in AP domain.
*----------------------------------------------------------------------------*/
void vgSigCiapbAtResponseInd (const SignalBuffer *signalBuffer,
                              VgmuxChannelNumber entity)
{
   /*As signalBuffer is located in stack memory, please don't free the buffer. */
   CiApbAtResponseInd* pCiApbAtResponseInd = PNULL;
   CiApbTlv* pTlv = PNULL;
   ResultCode_t atciResultCode = START_OF_RESULT_CODES;
   ApBridgeGenericContext_t* apBridgeGenericContext_p = ptrToApBridgeGenericContext();
   AtCmdControl* atCmdControl_p = apBridgeGenericContext_p->apAtCommandTable_p;
   Char* p_command_string = NULL;
   Char* p_urc_buf = NULL;
   Int32 urc_buf_len = 0;

   pCiApbAtResponseInd = &(signalBuffer->sig->ciApbAtResponseInd);

   if (APBRIDGE_RESULT_UNSOLICITED == pCiApbAtResponseInd->resultCode)
   {
     Int32 index = APB_URC_DEFAULT_CHANNEL_ID;
     Boolean be_urc_out = FALSE;

     if (pCiApbAtResponseInd->numberOfTlvs == 1)
     {
       pTlv = pCiApbAtResponseInd->tlv;
       if ((APB_INVALID_CMD_ID == pCiApbAtResponseInd->commandId)
           || (pCiApbAtResponseInd->commandId < VG_AT_AP_BRIDGE_BASE))
       {
         for(index = 0; index < sizeof(apBridgeGenericContext_p->apbChannels); index++)
         {
           if (TRUE == apBridgeGenericContext_p->apbChannels[index])
           {
             if (pTlv->data.string_p != PNULL)
             {
               /*generate \n\r*/
               vgPutNewLine(index);
               vgPutsWithLength(index, pTlv->data.string_p, pTlv->length);
               /*generate \n\r*/
               vgPutNewLine(index);
               vgFlushBuffer(index);
               be_urc_out = TRUE;
             }
             else
             {
               break;
             }
           }
         }
         if (FALSE == be_urc_out)
         {
           if(isEntityActive(APB_URC_DEFAULT_CHANNEL_ID) == TRUE)
           {
              /*generate \n\r*/
              vgPutNewLine(APB_URC_DEFAULT_CHANNEL_ID);
              vgPutsWithLength(APB_URC_DEFAULT_CHANNEL_ID,pTlv->data.string_p, pTlv->length);
              /*generate \n\r*/
              vgPutNewLine(APB_URC_DEFAULT_CHANNEL_ID);
              vgFlushBuffer(APB_URC_DEFAULT_CHANNEL_ID);
           }
         }
       }
       else
       {
         for(index = 0; index < apBridgeGenericContext_p->registeredCmdCount; index++)
         {
           if(atCmdControl_p->commandId == pCiApbAtResponseInd->commandId)
           {
              p_command_string = atCmdControl_p->string;
              break;
           }
          atCmdControl_p++;
         }
         /*URC Format: command header + ": " + urc info*/
         if (p_command_string != PNULL)
         {
           urc_buf_len = strlen((const char*)p_command_string) + strlen(": ")+ pTlv->length + 1;
           KiAllocMemory(urc_buf_len, (void**)(&(p_urc_buf)));
           if (p_urc_buf != PNULL)
           {
             memcpy(p_urc_buf, p_command_string, strlen((const char*)p_command_string));
             memcpy(p_urc_buf + strlen((const char*)p_command_string), ": ", strlen(": "));
             memcpy(p_urc_buf + strlen((const char*)p_command_string) + strlen(": "), pTlv->data.string_p, pTlv->length);
             p_urc_buf[urc_buf_len -1] = 0;
             for(index = 0; index < sizeof(apBridgeGenericContext_p->apbChannels); index++)
             {
               if (TRUE == apBridgeGenericContext_p->apbChannels[index])
               {
                 /*generate \n\r*/
                 vgPutNewLine(index);
                 vgPutsWithLength(index,p_urc_buf, urc_buf_len);
                 /*generate \n\r*/
                 vgPutNewLine(index);
                 vgFlushBuffer(index);
                 be_urc_out = TRUE;
               }
             }
             if (FALSE == be_urc_out)
             {
               if(isEntityActive(APB_URC_DEFAULT_CHANNEL_ID) == TRUE)
               {
                 /*generate \n\r*/
                 vgPutNewLine(APB_URC_DEFAULT_CHANNEL_ID);
                 vgPutsWithLength(APB_URC_DEFAULT_CHANNEL_ID,p_urc_buf, urc_buf_len);
                 /*generate \n\r*/
                 vgPutNewLine(APB_URC_DEFAULT_CHANNEL_ID);
                 vgFlushBuffer(APB_URC_DEFAULT_CHANNEL_ID);
               }
             }
             KiFreeMemory((void**)(&(p_urc_buf)));
           }
         }
       }
       #ifndef ENABLE_AP_BRIDGE_UNIT_TEST
       KiFreeMemory((void**)(&(pCiApbAtResponseInd->tlv[0].data.string_p)));
       #endif
     }
   }
   else
   {
     /* Guaranteeing the command id is right in the channel.*/
     if (APB_ERROR_NOT_USED == pCiApbAtResponseInd->errorCode)
     {
       atciResultCode = apbGetResultCode(pCiApbAtResponseInd->resultCode);
     }
     else
     {
       atciResultCode = apbGetErrorCode(pCiApbAtResponseInd->errorCode);
     }
     processResultCode(entity, pCiApbAtResponseInd->commandId, atciResultCode);
     /*Based on the design, the AT Command's result string saves in TLV,
       The number of TLV is always one.*/
     if (pCiApbAtResponseInd->numberOfTlvs == 1)
     {
       pTlv = pCiApbAtResponseInd->tlv;
       vgPutNewLine(entity);
       vgPutsWithLength(entity,(Char*)(pTlv->data.string_p),pTlv->length);
       vgPutNewLine(entity);
       vgFlushBuffer(entity);
       if (RESULT_CODE_CUSTOM_CONNECT == atciResultCode)
       {
         ApBridgeDataModeContext_t* apBridgeDataModeContext_p = vgApbGetDataModeContext(entity);
         memset(apBridgeDataModeContext_p->dataModeCustomResult, 0,
                sizeof(apBridgeDataModeContext_p->dataModeCustomResult));
         if (pTlv->length >= sizeof(apBridgeDataModeContext_p->dataModeCustomResult))
         {
           memcpy(apBridgeDataModeContext_p->dataModeCustomResult, pTlv->data.string_p,
                  sizeof(apBridgeDataModeContext_p->dataModeCustomResult) - 1);
         }
         else
         {
           memcpy(apBridgeDataModeContext_p->dataModeCustomResult, pTlv->data.string_p, pTlv->length);
         }
       }
       //sendResultCodeToCrm(atciResultCode, entity);
#ifndef ENABLE_AP_BRIDGE_UNIT_TEST
       KiFreeMemory((void**)(&(pCiApbAtResponseInd->tlv[0].data.string_p)));
#endif
     }
     if ((RESULT_CODE_CONNECT == atciResultCode)
         || (RESULT_CODE_CUSTOM_CONNECT == atciResultCode))
     {
       /*Trick thing: when the result code is CONNECT,
         we must use below function to keep the result code as RESULT_CODE_PROCEEDING.*/
       sendResultCodeToCrm(atciResultCode, entity);
     }
     else
     {
       setConnectionType(entity, CONNECTION_TERMINATOR);
       setResultCode(entity, atciResultCode);
       vgFlushBuffer(entity);
     }

   }
}
/*----------------------------------------------------------------------------
 *
 * Function:     vgApbDataModeReactiveTimerExpiry
 *
 * Parameters:   entity - mux channel number
 *
 * Returns:      nothing
 *
 * Description:  Handles AP Bridge data mode reactive timer expiry,to change into data mode.
 *
 *----------------------------------------------------------------------------*/
void vgApbDataModeReactiveTimerExpiry (const VgmuxChannelNumber entity)
{
  ApBridgeDataModeContext_t* apBridgeDataModeContext_p = vgApbGetDataModeContext(entity);

  if (getCommandId (entity) == apBridgeDataModeContext_p->dataModeCmdId)
  {
    /*Resume the data mode.*/
    vgSigCiMuxAtoCommandReq(entity);
  }
  else
  {
    setResultCode (entity, RESULT_CODE_ERROR);
  }
}
/*----------------------------------------------------------------------------
 *
 * Function:    vgApbCiCloseDataConnIndHandler
 *
 * Parameters:  signalBuffer - Pointer to a incoming  data structure
 *
 * Returns:     Nothing
 *
 * Description: process the data conneciton close event.
 *----------------------------------------------------------------------------*/
 void vgApbCiCloseDataConnIndHandler (Int32 channel)
{
  ApBridgeDataModeContext_t* apBridgeDataModeContext_p = vgApbGetDataModeContext(channel);
  if (apBridgeDataModeContext_p != PNULL)
  {
    vgFlushBuffer((VgmuxChannelNumber)channel);
    setEntityState (channel, ENTITY_RUNNING);
    setCommandId(channel, apBridgeDataModeContext_p->dataModeCmdId);
    apBridgeDataModeContext_p->dataModeState = RV_APB_DATA_MODE_DEACTIVATED;
    apBridgeDataModeContext_p->dataModeCmdId = VG_AT_LAST_CODE;
    apBridgeDataModeContext_p->channelIdInDataMode = VGMUX_CHANNEL_INVALID;
    memset(apBridgeDataModeContext_p->dataModeCustomResult, 0, sizeof(apBridgeDataModeContext_p->dataModeCustomResult));
    setConnectionType ((VgmuxChannelNumber)channel, CONNECTION_TERMINATOR);
    setResultCode((VgmuxChannelNumber)channel, RESULT_CODE_PROCEEDING);
  }
}

void vgCiapbDataModeResumeReqHandler (const SignalBuffer *signalBuffer)
{
  /*As signalBuffer is located in stack memory, please don't free the buffer. */
  ApbDataModeResumeReq* pApbDataModeResumeReq = PNULL;
  ApBridgeDataModeContext_t* apBridgeDataModeContext_p = PNULL;
  Int32 channelNumber = 0;
  pApbDataModeResumeReq = &(signalBuffer->sig->apbDataModeResumeReq);
  channelNumber = pApbDataModeResumeReq->channelNumber;
  apBridgeDataModeContext_p = vgApbGetDataModeContext(channelNumber);
  if (apBridgeDataModeContext_p != PNULL)
  {
    ApBridgeContext_t* apBridgeContext_p = ptrToApBridgeContext( channelNumber );
    /* set up data channel appropriately */
    setConnectionType (channelNumber, APBRIDGE_CONNECTION);
    apBridgeContext_p->operation = apBridgeDataModeContext_p->dataModeOperationMode;
    if (apBridgeDataModeContext_p->dataModeResultCode == RESULT_CODE_CONNECT)
    {
      sendResultCodeToCrm (RESULT_CODE_CONNECT, channelNumber);
    }
    else
    {
      if (strlen((char*)(apBridgeDataModeContext_p->dataModeCustomResult)) > 0)
      {
        vgPutNewLine(channelNumber);
        vgPutsWithLength(channelNumber,(Char*)(apBridgeDataModeContext_p->dataModeCustomResult),
          strlen((char*)(apBridgeDataModeContext_p->dataModeCustomResult)));
        vgPutNewLine(channelNumber);
        vgFlushBuffer(channelNumber);
      }
      sendResultCodeToCrm (RESULT_CODE_CUSTOM_CONNECT, channelNumber);
    }
    /* reset connection type */
    setConnectionType (channelNumber, CONNECTION_TERMINATOR);
    /* if using a multiplexer that does not have a data mode then
     * set the final result code to allow further command to be entered.
     * Setting to RESULT_CODE_PROCEEDING will not allow any AT commands
     * to be entered whilst in data mode
     */
    setResultCode (channelNumber, RESULT_CODE_PROCEEDING);
  }
}
