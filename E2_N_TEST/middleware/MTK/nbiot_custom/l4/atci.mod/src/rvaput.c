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
 * Header file for rvaput.c
 *
 * Procedures for AP Bridge subsystem's AT command execution
 *
 *
 * Implemented commands:
 *
 * All the AT Commands which will be executed in AP domain will be fowared by
 * the handler in AP Bridge subsystem/
 **************************************************************************/

#define MODULE_NAME "RVAPUT"

/***************************************************************************
 * Include Files
 ***************************************************************************/

 #include <system.h>
 #include <gkisig.h>
 #include <gkimem.h>
 #include <rvutil.h>
 #include <rvdata.h>
 #include <rvaput.h>
 #include <rvapsigo.h>
 #include <rvcimxut.h>
 #include <rvsystem.h>

/***************************************************************************
* Signal definitions
***************************************************************************/
union Signal
{
  CiRunAtCommandInd           ciRunAtCommandInd;
};

/*--------------------------------------------------------------------------
 *
 * Function:    apBridgeCommandHandler
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: This function will forward the AT Commands to AP modaim to be executed.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t apBridgeCommandHandler(CommandLine_t *commandBuffer_p,
                                    VgmuxChannelNumber entity)
{
  ApBridgeContext_t* apBridgeContext_p = ptrToApBridgeContext( entity );
  ApBridgeGenericContext_t* apBridgeGenericContext_p = ptrToApBridgeGenericContext();
  ScanParseContext_t* scanParseContext_p = ptrToScanParseContext(entity);
  /*As all the commands are executed in AP domain, the result is always proceeding.*/
  ResultCode_t result = RESULT_CODE_PROCEEDING;
  Int32 index = 0;
  Int32 parameter_len = 0;
  Boolean found_quotes = FALSE;


  apBridgeContext_p->operation = getExtendedOperation (commandBuffer_p);

  for(index = commandBuffer_p->position; index < commandBuffer_p->length; index ++)
  {
    if ((commandBuffer_p->character[index] == SEMICOLON_CHAR) && (FALSE == found_quotes))
    {
      break;
    }
    if (commandBuffer_p->character[index] == QUOTES_CHAR)
    {
      found_quotes = ((found_quotes == TRUE) ? FALSE : TRUE);
    }
    parameter_len ++;
  }
  commandBuffer_p->position += parameter_len;

  if (INVALID_EXTENDED_OPERATION != apBridgeContext_p->operation)
  {
    /*send the AT command request to AP domain to execute.*/
    if (vgSigCiapbAtCommandReq(entity, commandBuffer_p->character,
                               commandBuffer_p->position) == FALSE)
    {
      result = RESULT_CODE_ERROR;
    }
    else
    {
      /*memorize the channel id into context for URC case.*/
      apBridgeGenericContext_p->apbChannels[entity] = TRUE;
    }
  }
  else
  {
    result = RESULT_CODE_ERROR;
  }
  scanParseContext_p->nextCommand.position = commandBuffer_p->position;
  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    establishDataSessionForApBridge
 *
 * Parameters:  entity       - mux channel number
 *
 * Returns:     Resultcode_t - result of function
 *
 * Description: Establish data connection between MUX and AP Bridge.
 *
 *-------------------------------------------------------------------------*/
void establishDataSessionForApBridge(VgmuxChannelNumber entity)
{
  ApBridgeDataModeContext_t* apBridgeDataModeContext_p = vgApbGetDataModeContext(entity);

  apBridgeDataModeContext_p->pendingOpenDataConnCnf = TRUE;
  apBridgeDataModeContext_p->dataModeCmdId = getCommandId(entity);
  apBridgeDataModeContext_p->channelIdInDataMode = entity;
  vgCiMuxOpenDataConnection (entity, PSD_AP_BRIDGE);
  vgSigCiapbOpenDataConnReq(entity);
}
/*--------------------------------------------------------------------------
 *
 * Function:    vgApbUrcConfigCmd
 *
 * Parameters:  entity       - mux channel number
 *
 *              commandBuffer_p - command data
 *
 * Returns:     Resultcode_t - result of function
 *
 * Description: Config the URC channel.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgApbUrcConfigCmd ( CommandLine_t *commandBuffer_p,
                                const VgmuxChannelNumber entity)
{
  ResultCode_t result_code = RESULT_CODE_ERROR;
  ApBridgeGenericContext_t* apBridgeGenericContext_p = ptrToApBridgeGenericContext();

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_ASSIGN:
    {
      Int32 parameter = 0;
      if (getExtendedParameter (commandBuffer_p, &parameter, ULONG_MAX) == TRUE)
      {
        if (parameter == 1)
        {
          apBridgeGenericContext_p->apbChannels[entity] = TRUE;
          result_code = RESULT_CODE_OK;
        }
        else if (parameter == 0)
        {
          apBridgeGenericContext_p->apbChannels[entity] = FALSE;
          result_code = RESULT_CODE_OK;
        }
      }
      break;
    }
    case EXTENDED_RANGE:
    case EXTENDED_ACTION:
    case EXTENDED_QUERY:
    default:
    {
      break;
    }

  }
  return result_code;
}

ApBridgeDataModeContext_t* vgApbGetDataModeContext(const VgmuxChannelNumber entity)
{
  ApBridgeDataModeContext_t* apBridgeDataModeContext_p = PNULL;
  ApBridgeGenericContext_t* apBridgeGenericContext_p = ptrToApBridgeGenericContext();
  Boolean found = FALSE;
  Int32 index = 0;
  for(index = 0; index < APBRIDGE_DATA_MODE_CHANNEL_COUNT; index++)
  {
    apBridgeDataModeContext_p = apBridgeGenericContext_p->dataModeCtx + index;
    if ((apBridgeDataModeContext_p->dataModeState != RV_APB_DATA_MODE_DEACTIVATED)
      && (apBridgeDataModeContext_p->channelIdInDataMode == entity))
    {
      found = TRUE;
      break;
    }
  }

  if (found == TRUE)
  {
    return apBridgeDataModeContext_p;
  }
  else
  {
    return PNULL;
  }
}

void vgApbChannelSwitchedToCmdMode(VgmuxChannelNumber entity)
{
  ApBridgeDataModeContext_t* apBridgeDataModeContext_p = vgApbGetDataModeContext(entity);
  if (apBridgeDataModeContext_p != PNULL)
  {
    vgSigCiapbDataModeTempDeactived(entity);
  }
}
