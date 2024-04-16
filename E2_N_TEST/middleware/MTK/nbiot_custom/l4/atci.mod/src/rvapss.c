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
 * rvapss.c
 * Handler for AP Bridge AT Commands
 **************************************************************************/

#define MODULE_NAME "RVAPSS"


/***************************************************************************
* Include Files
***************************************************************************/
#include <system.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvapss.h>
#include <rvaput.h>
#include <rvapsigi.h>
#ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
#include "memory_map.h"
#include "hal_flash.h"
#include "hal_rtc_internal.h"
#include "hal_rtc_external.h"
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_platform.h"
#include <gkimem.h>
#endif

/***************************************************************************
* Type definitions
***************************************************************************/
#ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
typedef struct {
    Int32 checkSum;
    Int32 bufLen;
    Int32 dataSyncResult;
    Int32 cmdCount;
    Int32 cmdBaseId;
} rvapCmdDataContextHeader;
#endif
/***************************************************************************
* Macro definitions
***************************************************************************/
#ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
#define RVAP_AT_CMD_SYNC_READY           0xF0E1D2C3
#define RVAP_AT_CMD_DATA_OFFSET          20
#define RVAP_AT_CMD_CHECKSUM_OFFSET      4
#define RVAP_AT_CMD_CHECKSUM_FIELD_SIZE  4
#endif

/***************************************************************************
* Signal definitions
***************************************************************************/

union Signal
{
  CiRunAtCommandInd         ciRunAtCommandInd;
  CiApbAtResponseInd        ciApbAtResponseInd;
  CiApbRegisterAtCommandInd ciApbRegisterAtCommandInd;
  CiMuxChannelDisabledInd   ciMuxChannelDisabledInd;
};
/***************************************************************************
* Static variants definitions
***************************************************************************/
/* AT command lookup table */
static const AtCmdControl apbAtCommandTable[] =
{
    { ATCI_CONST_CHAR_STR "+EURCCONF", vgApbUrcConfigCmd, VG_AT_URCCONF, AT_CMD_ACCESS_NONE},

    { PNULL,                           PNULL,             VG_AT_LAST_CODE, AT_CMD_ACCESS_NONE}
};

#ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
static rvapCmdDataContextHeader contextHeader = {0};
#endif
/***************************************************************************
* Local Function Prototypes
***************************************************************************/

static void initialiseApSsGenericData(void);
static void initialiseApss(void);
static Int16 rvapCalcChecksum(Int8 *p_data, Int32 length);
/***************************************************************************
 * Local Functions
 ***************************************************************************/

 /****************************************************************************
 *
 * Function:    initialiseApSsEntitySpecific
 *
 * Parameters:  none
 *
 * Returns:     Nothing
 *
 * Description: initiates AP Bridge sub-system entity specific data
 *
 ****************************************************************************/

void initialiseApSsEntitySpecific (const VgmuxChannelNumber entity)
{
  ApBridgeContext_t* apBridgeContext_p = ptrToApBridgeContext (entity);
  apBridgeContext_p->operation = INVALID_EXTENDED_OPERATION;
}

#ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
/****************************************************************************
*
* Function:    vgApssRestoreAtData
*
* Parameters:  none
*
* Returns:     Nothing
*
* Description: Restore AP domain related AT contexts.
*
****************************************************************************/
void vgApssRestoreAtData(void)
{
  ApBridgeGenericContext_t* apBridgeGenericContext_p = ptrToApBridgeGenericContext();
  hal_flash_status_t flash_op_result;
  Int32 check_sum = 0;
  Int32 index = 0;
  Int8* pCommandsData = PNULL;
  CommandId_t cmdId = VG_AT_LAST_CODE;
  AtCmdControl* pCmdControl = PNULL;
  Int32 length = 0;

  flash_op_result = hal_flash_read((uint32_t)(APB_SLEEP_CONTEXT_BASE - FLASH_BASE),
                                   (uint8_t *)(&contextHeader), sizeof(contextHeader));
  if (flash_op_result != HAL_FLASH_STATUS_OK)
  {
    return;
  }
  if (contextHeader.dataSyncResult != RVAP_AT_CMD_SYNC_READY)
  {
    return;
  }
  if (contextHeader.bufLen > (APB_SLEEP_CONTEXT_LENGTH - RVAP_AT_CMD_CHECKSUM_OFFSET))
  {
    return;
  }

  check_sum = rvapCalcChecksum(APB_SLEEP_CONTEXT_BASE + RVAP_AT_CMD_CHECKSUM_OFFSET, contextHeader.bufLen);
  if (check_sum != contextHeader.checkSum)
  {
    return;
  }
  apBridgeGenericContext_p->apAtCommandTableItemSize = contextHeader.cmdCount + 1;
  /*sync the AT data to APB command table.*/
  KiAllocMemory( (apBridgeGenericContext_p->apAtCommandTableItemSize)*(sizeof(AtCmdControl))
                  , &(apBridgeGenericContext_p->apAtCommandTable_p));
  if (PNULL == apBridgeGenericContext_p->apAtCommandTable_p)
  {
    apBridgeGenericContext_p->cmdRegistered = FALSE;
    return;
  }
  length = (contextHeader.bufLen - (RVAP_AT_CMD_DATA_OFFSET - RVAP_AT_CMD_CHECKSUM_FIELD_SIZE));

  KiAllocMemory(length, &pCommandsData);
  if (PNULL == pCommandsData)
  {
    KiFreeMemory((void**)(&(apBridgeGenericContext_p->apAtCommandTable_p)));
  }
  flash_op_result = hal_flash_read(APB_SLEEP_CONTEXT_BASE - FLASH_BASE + RVAP_AT_CMD_DATA_OFFSET,
                                   pCommandsData, length);
  if (flash_op_result != HAL_FLASH_STATUS_OK)
  {
    KiFreeMemory((void**)(&(apBridgeGenericContext_p->apAtCommandTable_p)));
    KiFreeMemory((void**)(&pCommandsData));
  }

  cmdId = contextHeader.cmdBaseId;
  pCmdControl = (AtCmdControl*)(apBridgeGenericContext_p->apAtCommandTable_p);
  pCmdControl->string = pCommandsData;
  pCmdControl->commandId = cmdId;
  pCmdControl->procFunc = apBridgeCommandHandler;
  pCmdControl->accessMask = AT_CMD_ACCESS_NONE;
  for(index = 0; index < length; index++)
  {
    if ( ((*(pCommandsData + index)) == '\0')
          && ((index + 1) != length))
    {
      /*Another AT command beginning.*/
      pCmdControl++;
      cmdId++;
      pCmdControl->string = pCommandsData + index + 1;
      pCmdControl->commandId = cmdId;
      pCmdControl->procFunc = apBridgeCommandHandler;
      pCmdControl->accessMask = AT_CMD_ACCESS_NONE;
    }
  }

  pCmdControl++;
  pCmdControl->string = PNULL;
  pCmdControl->commandId = VG_AT_LAST_CODE;
  pCmdControl->procFunc = PNULL;
  pCmdControl->accessMask = AT_CMD_ACCESS_NONE;
  apBridgeGenericContext_p->cmdRegistered = TRUE;
  apBridgeGenericContext_p->registeredCmdCount = contextHeader.cmdCount;
}
#endif

/****************************************************************************
 *
 * Function:    initialiseApss
 *
 * Parameters:  none
 *
 * Returns:     Nothing
 *
 * Description: initiates AP Bridge sub-system;
 *
 ****************************************************************************/
static void initialiseApss(void)
{
  initialiseApSsGenericData();
}

/****************************************************************************
 *
 * Function:    initialiseApSsGenericData
 *
 * Parameters:  none
 *
 * Returns:     Nothing
 *
 * Description: initiates AP Bridge sub-system entity Generic data
 *
 ****************************************************************************/
static void initialiseApSsGenericData(void)
{
  Int32 index = 0;
  ApBridgeDataModeContext_t* dataModeCtx = PNULL;
  ApBridgeGenericContext_t* apBridgeGenericContext_p = ptrToApBridgeGenericContext();
  apBridgeGenericContext_p->registeredCmdCount = 0U;
  apBridgeGenericContext_p->apAtCommandTableItemSize = 0;
  apBridgeGenericContext_p->apAtCommandTable_p = PNULL;
  apBridgeGenericContext_p->cmdRegistered = FALSE;
  apBridgeGenericContext_p->pendingCmdCount = 0;
  memset(apBridgeGenericContext_p->pendingCmdChanelIds, 0, sizeof(apBridgeGenericContext_p->pendingCmdChanelIds));
  memset(apBridgeGenericContext_p->apbChannels, 0, sizeof(apBridgeGenericContext_p->apbChannels));
  for(index = 0; index < APBRIDGE_DATA_MODE_CHANNEL_COUNT; index ++)
  {
    dataModeCtx = apBridgeGenericContext_p->dataModeCtx + index;
    dataModeCtx->channelIdInDataMode = -1;
    dataModeCtx->dataModeCmdId = -1;
    dataModeCtx->dataModeState = RV_APB_DATA_MODE_DEACTIVATED;
  }
}

static Int16 rvapCalcChecksum(Int8 *p_data, Int32 length)
{
  Int16 check_sum = 0;
  Int32 index = 0;
  Int16 temp = 0;

  for (index = 0; index < (length / 2); index ++) {
    temp = p_data[index << 1];
    temp += p_data[(index << 1) + 1] << 8;
    check_sum ^= temp;
  }

  if ((length % 2) == 1) {
    check_sum ^= p_data[index * 2];
  }
  return check_sum;
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

 /****************************************************************************
 *
 * Function:    vgApssInterfaceController
 *
 * Parameters:  SignalBuffer - structure containing incoming signal
 *              VgmuxChannelNumber - mux channel number
 *
 * Returns:     Boolean - indicates whether the sub-system has recognised and
 *                        procssed the signal given.
 *
 * Description: determines action for received signals
 *
 ****************************************************************************/
Boolean vgApssInterfaceController (const SignalBuffer *signal_p,
                                   VgmuxChannelNumber entity)
{
  Boolean  accepted = FALSE;

  /* Signal Handler */

  switch (*signal_p->type)
  {
    case SIG_CI_RUN_AT_COMMAND_IND:
    {
      ApBridgeGenericContext_t* apBridgeGenericContext_p = ptrToApBridgeGenericContext();
      if (TRUE == apBridgeGenericContext_p->cmdRegistered)
      {
        /*The command sync between AP and MD is ready.*/
        if (apBridgeGenericContext_p->apAtCommandTable_p != PNULL)
        {
          AtCmdControl* pCmdTbl = (AtCmdControl*)(apBridgeGenericContext_p->apAtCommandTable_p);
          accepted = parseCommandBuffer (pCmdTbl, entity);
        }
        if (accepted == FALSE)
        {
          accepted = parseCommandBuffer(apbAtCommandTable, entity);
        }
      }
      else
      {
        /*The command sync between AP and MD is not ready.
          The commands are temporarily buffered until the sync is ready.*/
        if (apBridgeGenericContext_p->pendingCmdCount < CI_MAX_ENTITIES)
        {
          apBridgeGenericContext_p->pendingCmdChanelIds[apBridgeGenericContext_p->pendingCmdCount] = entity;
          setResultCode(entity, RESULT_CODE_PROCEEDING);
          accepted = TRUE;
          apBridgeGenericContext_p->pendingCmdCount ++;
        }
        else
        {
          FatalAssert(0);
        }
      }
      break;
    }
    case SIG_INITIALISE:
    {
      /* Don't accept this signal.*/
      initialiseApss();
      break;
    }
    case SIG_CIMUX_CHANNEL_ENABLE_IND:
    {
      initialiseApSsEntitySpecific (entity);
      break;
    }
    case SIG_CIMUX_CHANNEL_DISABLED_IND:
    {
      ApBridgeGenericContext_t* apBridgeGenericContext_p = ptrToApBridgeGenericContext();
      ApBridgeDataModeContext_t* apBridgeDataModeContext_p = vgApbGetDataModeContext(entity);
      apBridgeGenericContext_p->apbChannels[entity] = FALSE;
      if (apBridgeDataModeContext_p != PNULL)
      {
        apBridgeDataModeContext_p->channelIdInDataMode = -1;
        apBridgeDataModeContext_p->dataModeCmdId = -1;
        memset(apBridgeDataModeContext_p->dataModeCustomResult, 0, sizeof(apBridgeDataModeContext_p->dataModeCustomResult));
        apBridgeDataModeContext_p->dataModeResultCode = START_OF_RESULT_CODES;
        apBridgeDataModeContext_p->dataModeState = RV_APB_DATA_MODE_DEACTIVATED;
        apBridgeDataModeContext_p->pendingOpenDataConnCnf = FALSE;
      }
      break;
    }
    case SIG_CIAPB_REGISTER_AT_COMMAND_IND:
    {
      vgSigCiapbRegisterAtCommandInd(signal_p);
      accepted = TRUE;
      break;
    }
    case SIG_CIAPB_AT_RESPONSE_IND:
    {
      vgSigCiapbAtResponseInd(signal_p, entity);
      accepted = TRUE;
      break;
    }
    case SIG_CIAPB_DATA_MODE_RESUME_REQ:
    {
      printf("=====SIG_CIAPB_DATA_MODE_RESUME_REQ \r\n");
      vgCiapbDataModeResumeReqHandler(signal_p);
      accepted = TRUE;
      break;
    }
    default:
    {
      break;
    }

  }
  return (accepted);
}
/* END OF FILE */
