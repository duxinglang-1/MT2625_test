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
 * This module contains all the terminal profile encode routines.  The
 * module is called from vgstk.c when a response needs to be sent back
 * to the sim.
 **************************************************************************/

#define MODULE_NAME "RVSTKTP"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif


#include <gkisig.h>
#include <gkimem.h>
#if !defined (RVSTKTP_H)
#  include <rvstktp.h>
#endif
#if !defined (AFSA_SIG_H)
#  include <afsa_sig.h>
#endif
#if !defined (ABST_SIG_H)
#  include <abst_sig.h>
#endif
#if !defined (RVSTKRD_H)
#  include <rvstkrd.h>
#endif
#if !defined (RVUTIL_H)
#  include <rvutil.h>
#endif
#if !defined (RVDATA_H)
#  include <rvdata.h>
#endif
#if !defined (RVOMAN_H)
#  include <rvoman.h>
#endif
#if !defined (RVCRHAND_H)
#  include <rvcrhand.h>
#endif

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

typedef ResultCode_t (*StkEncodeProcFunc)(CommandLine_t *commandBuffer_p,
                                          const Boolean  alwaysSendTp,
                                          const VgmuxChannelNumber entity );

typedef struct StkEncodeSignalEntityTag
{
   SignalId           signalType;
   StkCommandId       commandId;
   StkCommandType     commandType;
   StkEncodeProcFunc  stkEncodeProcFunc;
}
StkEncodeSignalEntityControl;

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static Boolean getDisplayTextTpResult (CommandLine_t *commandBuffer_p,
                                        SimatGeneralResult  *generalResult,
                                         Boolean  *meProblem,
                                          SimatMeProblemAddInfo  *additionalInfo);
static Boolean getGetSelectItemTpResult (CommandLine_t *commandBuffer_p,
                                          SimatGeneralResult  *generalResult,
                                           Boolean  *meProblem,
                                            SimatMeProblemAddInfo  *additionalInfo);
static Boolean getGetSetUpMenuTpResult (CommandLine_t *commandBuffer_p,
                                         SimatGeneralResult  *generalResult,
                                          Boolean  *meProblem,
                                           SimatMeProblemAddInfo  *additionalInfo);
static ResultCode_t vgSigAfsaEncodeDisplayTextRsp (CommandLine_t *commandBuffer_p,
                                                    const Boolean  alwaysSendTp,
                                                     const VgmuxChannelNumber entity);
static ResultCode_t vgAfsaRefreshRsp (CommandLine_t *commandBuffer_p,
                                       const Boolean  alwaysSendTp,
                                        const VgmuxChannelNumber entity);
static ResultCode_t vgSigAfsaEncodeSelectItemRsp (CommandLine_t *commandBuffer_p,
                                                   const Boolean  alwaysSendTp,
                                                    const VgmuxChannelNumber entity);
static ResultCode_t vgSigAfsaEncodeSetUpMenuRsp (CommandLine_t *commandBuffer_p,
                                                  const Boolean  alwaysSendTp,
                                                   const VgmuxChannelNumber entity);
static ResultCode_t vgApexStDisplayAlphaIdRsp (CommandLine_t *commandBuffer_p,
                                                const Boolean  alwaysSendTp,
                                                 const VgmuxChannelNumber entity);
static Boolean getGetInputTpResult (CommandLine_t *commandBuffer_p,
                                     SimatGeneralResult  *generalResult,
                                      Boolean  *meProblem,
                                       SimatMeProblemAddInfo  *additionalInfo);
static Boolean getGetToneTpResult (CommandLine_t *commandBuffer_p,
                                    SimatGeneralResult  *generalResult,
                                     Boolean  *meProblem,
                                      SimatMeProblemAddInfo  *additionalInfo);

static ResultCode_t vgSigAfsaEncodeInkeyRsp (CommandLine_t *commandBuffer_p,
                                              const Boolean  alwaysSendTp,
                                               const VgmuxChannelNumber entity);
static ResultCode_t vgSigAfsaEncodeGetInputRsp (CommandLine_t *commandBuffer_p,
                                                 const Boolean  alwaysSendTp,
                                                  const VgmuxChannelNumber entity);
static ResultCode_t vgSigAfsaEncodeToneRsp (CommandLine_t *commandBuffer_p,
                                             const Boolean  alwaysSendTp,
                                              const VgmuxChannelNumber entity);
static Boolean getGetIdleModeTextTpResult (CommandLine_t *commandBuffer_p,
                                            SimatGeneralResult  *generalResult,
                                             Boolean  *meProblem,
                                              SimatMeProblemAddInfo  *additionalInfo);
static ResultCode_t vgAfsaSetUpIdleModeTextRsp (CommandLine_t *commandBuffer_p,
                                                 const Boolean  alwaysSendTp,
                                                  const VgmuxChannelNumber entity);
static ResultCode_t vgSigSetUpEventListRsp (CommandLine_t *commandBuffer_p,
                                             const Boolean  alwaysSendTp,
                                              const VgmuxChannelNumber entity);
static Boolean getGetLaunchBrowserTpResult (CommandLine_t *commandBuffer_p,
                                             SimatGeneralResult  *generalResult,
                                              Boolean  *meProblem,
                                               SimatMeProblemAddInfo  *additionalInfo,
                                                 SimatLauBrProblemAddInfo  *browserProblem,
                                                  Boolean  *browserProbPresent);
static ResultCode_t vgAfsaLaunchBrowserRsp (CommandLine_t *commandBuffer_p,
                                             const Boolean  alwaysSendTp,
                                              const VgmuxChannelNumber entity);
static ResultCode_t vgAfsaLanguageNotificationRsp (CommandLine_t *commandBuffer_p,
                                                    const Boolean  alwaysSendTp,
                                                     const VgmuxChannelNumber entity);


static Int16   vgStkGetArrayFromHex(CommandLine_t *commandBuffer_p, Char  *data_p);

static StkCommandId getAlphaCmdId (const SignalBuffer signal,
                                    Boolean  *result);

static SimatAlphaIdType getCmdIdAlphaType (Int32  cmdId);

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* given an IND create the RSP and send it to the SIM AT */
static const StkEncodeSignalEntityControl stkEncodeSignalEntityTable[] =
{
   {SIG_AFSA_DISPLAY_TEXT_IND,          STK_CMD_DISPLAY_TEXT,
     STK_PROTOCOL_REQ,                   vgSigAfsaEncodeDisplayTextRsp},
   {SIG_AFSA_REFRESH_IND,               STK_CMD_SETUP_REFRESH,
     STK_UNSOLICITED_REQ,                vgAfsaRefreshRsp},
   {SIG_AFSA_SELECT_ITEM_IND,           STK_CMD_SELECT_ITEM,
     STK_PROTOCOL_REQ,                   vgSigAfsaEncodeSelectItemRsp},
   {SIG_AFSA_SET_UP_MENU_IND,           STK_CMD_SETUP_MENU,
     STK_PROTOCOL_REQ,                   vgSigAfsaEncodeSetUpMenuRsp},
   {SIG_APEX_ST_DISPLAY_ALPHA_ID_IND,   STK_CMD_SETUP_DISPLAY_ALPHA_ID,
     STK_DISPLAY_ALPHA_ID_REQ,           vgApexStDisplayAlphaIdRsp},
   {SIG_AFSA_GET_INKEY_IND,             STK_CMD_GET_INKEY,
     STK_PROTOCOL_REQ,                   vgSigAfsaEncodeInkeyRsp},
   {SIG_AFSA_GET_INPUT_IND,             STK_CMD_GET_INPUT,
     STK_PROTOCOL_REQ,                   vgSigAfsaEncodeGetInputRsp},
   {SIG_AFSA_PLAY_TONE_IND,             STK_CMD_PLAY_TONE,
     STK_PROTOCOL_REQ,                   vgSigAfsaEncodeToneRsp},
   {SIG_AFSA_SET_UP_IDLE_MODE_TEXT_IND, STK_CMD_SETUP_IDLE_MODE_TEXT,
     STK_PROTOCOL_REQ,                   vgAfsaSetUpIdleModeTextRsp},
   {SIG_AFSA_SET_UP_EVENT_LIST_IND,     STK_CMD_SETUP_EVENT_LIST,
     STK_PROTOCOL_REQ,                   vgSigSetUpEventListRsp},
   {SIG_AFSA_LAUNCH_BROWSER_IND,        STK_CMD_SETUP_LAUNCH_BROWSER,
     STK_PROTOCOL_REQ,                   vgAfsaLaunchBrowserRsp},
   {SIG_AFSA_LANGUAGE_NOTIFICATION_IND, STK_CMD_SETUP_LANGUAGE_NOTIF,
     STK_UNSOLICITED_REQ,                vgAfsaLanguageNotificationRsp},
   {SIG_SYS_DUMMY, STK_NOT_PROACTIVE, STK_COMMAND_ERROR, PNULL}
};

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  AfsaStRegisterTaskReq        afsaStRegisterTaskReq;
  AfsaStRegisterTaskCnf        afsaStRegisterTaskCnf;
  AfsaDisplayTextRsp           afsaDisplayTextRsp;
  AfsaSelectItemRsp            afsaSelectItemRsp;
  ApexStSetUpSsRsp             apexStSetUpSsRsp;
  AfsaSetUpMenuRsp             afsaSetUpMenuRsp;
  AfsaRefreshRsp               afsaRefreshRsp;
  ApexStDisplayAlphaIdInd      apexStDisplayAlphaIdInd;
  AfsaGetInkeyRsp              afsaGetInkeyRsp;
  AfsaGetInputRsp              afsaGetInputRsp;
  AfsaPlayToneRsp              afsaPlayToneRsp;
  AfsaRunAtCommandRsp          afsaRunAtCommandRsp;
  AfsaSetUpIdleModeTextRsp     afsaSetUpIdleModeTextRsp;
  ApexStCancelCommandReq       apexStCancelCommandReq;
  ApexStSetUpUssdRsp           apexStSetUpUssdRsp;
  ApexStCallSetupGetAckRsp     apexStCallSetupGetAckRsp;
  ApexStSetUpCallRsp           apexStSetUpCallRsp;
  AfsaSetUpEventListRsp        afsaSetUpEventListRsp;
  AfsaLaunchBrowserRsp         afsaLaunchBrowserRsp;
  AfsaLanguageNotificationRsp  afsaLanguageNotificationRsp;
};

 static union Signal RspSignals;

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:    getDisplayTextTpResult
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              generalResult   - result of indication
 *              meProblem       - problem flag
 *              additionalInfo  - more information on problem
 *
 * Returns:     Boolean - display not possible
 *
 * Description: Sets the terminal profile result flags for the indication
 *
 *-------------------------------------------------------------------------*/

static Boolean getDisplayTextTpResult (CommandLine_t   *commandBuffer_p,
                                        SimatGeneralResult    *generalResult,
                                         Boolean               *meProblem,
                                          SimatMeProblemAddInfo *additionalInfo)
{
  Int32   result;
  Boolean status = TRUE;

  if (getExtendedParameter (commandBuffer_p, &result, ULONG_MAX) == TRUE)
  {
    switch (result)
    {
      case ULONG_MAX:
      {
        status = FALSE;
        break;
      }
      case 0:
      {
        *generalResult = SIMAT_GR_COMM_PERF_OK;
        *meProblem = FALSE;
        break;
      }
      case 1:
      {
        *generalResult = SIMAT_GR_USER_TERM_SESSION;
        *meProblem = FALSE;
        break;
      }
      case 2:
      {
        *generalResult = SIMAT_GR_ME_CANNOT_PROC_COMMAND;
        *additionalInfo = SIMAT_INFO_SCREEN_BUSY;
        *meProblem = TRUE;
        break;           /* job 102143: added essential break statement */
      }
      case 3:
      {
        *generalResult = SIMAT_GR_USER_REQ_BACK_MOVE;
        *meProblem = FALSE;
        break;
      }
    case 4:
      {
        *generalResult = SIMAT_GR_COMM_PERF_OK_NO_ICON;
        *meProblem = FALSE;
        break;
      }
    case 5:
      {
        *generalResult = SIMAT_GR_USER_NO_RESPONSE;
        *meProblem = TRUE;
        *additionalInfo = SIMAT_INFO_NO_CAUSE_GIVEN;
        break;
      }
    default:
      {
        status = FALSE;
        break;
      }         
    }
  }
  else
  {
    status = FALSE; /* not optional */
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    getGetSelectItemTpResult
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              generalResult   - result of indication
 *              meProblem       - problem flag
 *              additionalInfo  - more information on problem
 *
 * Returns:     Boolean - display not possible
 *
 * Description: Sets the terminal profile result flags for the indication
 *
 *-------------------------------------------------------------------------*/

static Boolean getGetSelectItemTpResult (CommandLine_t *commandBuffer_p,
                                          SimatGeneralResult  *generalResult,
                                           Boolean  *meProblem,
                                            SimatMeProblemAddInfo  *additionalInfo)
{
  Int32   result = 0;
  Boolean status = TRUE;

  if (getExtendedParameter (commandBuffer_p, &result, ULONG_MAX) == TRUE)
  {
    switch (result)
    {
      case ULONG_MAX:
      {
        status = FALSE;
        break;
      }
      case 0:
      {
        *generalResult = SIMAT_GR_COMM_PERF_OK;
        *meProblem = FALSE;
        break;
      }
      case 1:
      {
        *generalResult = SIMAT_GR_USER_TERM_SESSION;
        *meProblem = FALSE;
        break;
      }
      case 2:
      {
        *generalResult = SIMAT_GR_USER_REQ_HELP_INFO;
        *meProblem = FALSE;
        break;
      }
      case 3:
      {
        *generalResult = SIMAT_GR_USER_REQ_BACK_MOVE;
        *meProblem = FALSE;
        break;
      }
      case 4:
      {        
      *generalResult = SIMAT_GR_ME_CANNOT_PROC_COMMAND;
        *additionalInfo = SIMAT_INFO_SCREEN_BUSY;
        *meProblem = TRUE;
        break; 
      }
      case 5:
      {
        *generalResult = SIMAT_GR_COMM_PERF_OK_NO_ICON;
        *meProblem = FALSE;
        break;
      }
      case 6:
      {
        *generalResult = SIMAT_GR_USER_NO_RESPONSE;
        *meProblem = TRUE;
        *additionalInfo = SIMAT_INFO_NO_CAUSE_GIVEN;
        break;
      }
      default:
      {
        status = FALSE;
        break;    
      }      
    }
  }
  else
  {
    status = FALSE; /* not optional */
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    getGetSetUpMenuTpResult
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              generalResult   - result of indication
 *              meProblem       - problem flag
 *              additionalInfo  - more information on problem
 *
 * Returns:     Boolean - display not possible
 *
 * Description: Sets the terminal profile result flags for the indication
 *
 *-------------------------------------------------------------------------*/

static Boolean getGetSetUpMenuTpResult (CommandLine_t *commandBuffer_p,
                                         SimatGeneralResult  *generalResult,
                                          Boolean  *meProblem,
                                           SimatMeProblemAddInfo  *additionalInfo)
{
  Int32   result = 0;
  Boolean status = TRUE;

  if (getExtendedParameter (commandBuffer_p, &result, ULONG_MAX) == TRUE)
  {
    switch (result)
    {
      case ULONG_MAX:
      {
        status = FALSE;
        break;
      }
      case 0:
      {
        *generalResult = SIMAT_GR_COMM_PERF_OK;
        *meProblem = FALSE;
        break;
      }
      case 1:
      {
        *generalResult = SIMAT_GR_ME_CANNOT_PROC_COMMAND;
        *meProblem = TRUE;
        *additionalInfo = SIMAT_INFO_NO_CAUSE_GIVEN;
        break;
      }
      case 2:
      {
        *generalResult = SIMAT_GR_COMM_PERF_OK_NO_ICON;
        *meProblem = FALSE;
        break;
      }
      default:
      {
        status = FALSE;
        break;
      }
    }
  }
  else
  {
    status = FALSE; /* not optional */
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigAfsaEncodeDisplayTextRsp
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              alwaysSendTp    - always send profile
 *              entity          - mux channel number
 *
 * Returns:     Boolean - response not valid
 *
 * Description: Decodes the data and sets up the data string.  The accessory
 *              must use the STK protocol to retreive the data string.  The
 *              terminal profile is sent to the SIM AT task is the encode is
 *              successful. If have timed out then we must send a profile
 *              response to the SIM AT task. The response will indicate that
 *              an error has occured.
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgSigAfsaEncodeDisplayTextRsp (CommandLine_t *commandBuffer_p,
                                                    const Boolean  alwaysSendTp,
                                                     const VgmuxChannelNumber entity)
{
  SignalBuffer           sendSignal = kiNullBuffer;
  Int32                  cmdId;
  SimatGeneralResult     generalResult = SIMAT_GR_COMM_PERF_OK;
  Boolean                meProblem = FALSE;
  SimatMeProblemAddInfo  additionalInfo = SIMAT_INFO_NO_CAUSE_GIVEN;
  ResultCode_t           result = RESULT_CODE_OK;
  Boolean                rspResult = TRUE;
  PARAMETER_NOT_USED (entity);

  /* access the union signal definition */
  RspSignals.afsaDisplayTextRsp.simatCommandRef = vgStkGetSimCmd ();

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_ASSIGN:   /* AT*MSTCR= */
    {
      /* decode data from the message */
      if (getExtendedParameter (commandBuffer_p, &cmdId, ULONG_MAX) == TRUE)
      {
        if (cmdId == STK_CMD_DISPLAY_TEXT)
        {
          rspResult = getDisplayTextTpResult (commandBuffer_p,
                                               &generalResult,
                                                &meProblem,
                                                 &additionalInfo);

          RspSignals.afsaDisplayTextRsp.generalResult = generalResult;
          RspSignals.afsaDisplayTextRsp.meProblemPresent = meProblem;
          RspSignals.afsaDisplayTextRsp.meProblem = additionalInfo;
          if (rspResult == FALSE)
          {
             result = VG_CME_STK_INVALID_RESPONSE_DATA;
          }
        }
        else
        {
          result = VG_CME_STK_INVALID_COMMAND_ID;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

   /* only send the tp if we have decode the data ok or a alwaysSendTp
      has occurred */
  if ((result == RESULT_CODE_OK) || (alwaysSendTp == TRUE))
  {
    /* handle any error cases */
    if (result != RESULT_CODE_OK)
    {
      RspSignals.afsaDisplayTextRsp.generalResult    = SIMAT_GR_USER_NO_RESPONSE;
      RspSignals.afsaDisplayTextRsp.meProblemPresent = TRUE;
      RspSignals.afsaDisplayTextRsp.meProblem        = SIMAT_INFO_NO_CAUSE_GIVEN;
    }

    /* now send the terminal response to the SIM */
    KiCreateZeroSignal (SIG_AFSA_DISPLAY_TEXT_RSP,
                         sizeof (AfsaDisplayTextRsp),
                          &sendSignal);

    memcpy (&sendSignal.sig->afsaDisplayTextRsp,
             &RspSignals.afsaDisplayTextRsp,
              sizeof (RspSignals.afsaDisplayTextRsp));

    KiSendSignal (SIMAT_TASK_ID, &sendSignal);
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgAfsaRefreshRsp
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              alwaysSendTp    - always send profile
 *              entity          - mux channel number
 *
 * Returns:     Boolean - response not valid
 *
 * Description: Sends the output data to the accessory unsolicited  followed
 *              by the terminal response.
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgAfsaRefreshRsp (CommandLine_t *commandBuffer_p,
                                       const Boolean  alwaysSendTp,
                                        const VgmuxChannelNumber entity)
{
  SignalBuffer  sendSignal = kiNullBuffer;
  ResultCode_t  rspResult = RESULT_CODE_OK;

  PARAMETER_NOT_USED (entity);
  PARAMETER_NOT_USED (alwaysSendTp);
  PARAMETER_NOT_USED (commandBuffer_p);

  RspSignals.afsaRefreshRsp.simatCommandRef = (Int16)vgStkGetSimCmd ();

  RspSignals.afsaRefreshRsp.generalResult    = SIMAT_GR_ME_CANNOT_PROC_COMMAND;
  RspSignals.afsaRefreshRsp.meProblemPresent = TRUE;
  RspSignals.afsaRefreshRsp.meProblem  = SIMAT_INFO_NO_CAUSE_GIVEN;

  if (vgStkIsDataValid () == FALSE)
  {
    RspSignals.afsaRefreshRsp.generalResult    = SIMAT_GR_ME_CANNOT_PROC_COMMAND;
    RspSignals.afsaRefreshRsp.meProblemPresent = TRUE;
    RspSignals.afsaRefreshRsp.meProblem        = SIMAT_INFO_NO_CAUSE_GIVEN;
  }
  else
  {
    RspSignals.afsaRefreshRsp.generalResult    = SIMAT_GR_COMM_PERF_OK;
    RspSignals.afsaRefreshRsp.meProblemPresent = FALSE;
  }

  /* now send the RSP signal to the SIM */
  KiCreateZeroSignal (SIG_AFSA_REFRESH_RSP,
                       sizeof (AfsaRefreshRsp),
                        &sendSignal);

  memcpy (&sendSignal.sig->afsaRefreshRsp,
           &RspSignals.afsaRefreshRsp,
            sizeof (RspSignals.afsaRefreshRsp));

  KiSendSignal (SIMAT_TASK_ID, &sendSignal);

  return (rspResult);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigAfsaEncodeSelectItemRsp
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              alwaysSendTp    - always send profile
 *              entity          - mux channel number
 *
 * Returns:     Boolean - response not valid
 *
 * Description: Decodes the data and sets up the data string.  The accessory
 *              must use the STK protocol to retreive the data string.  The
 *              terminal profile is sent to the SIM AT task is the encode is
 *              successful. If have timed out then we must send a profile
 *              response to the SIM AT task. The response will indicate that
 *              an error has occured.
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgSigAfsaEncodeSelectItemRsp (CommandLine_t *commandBuffer_p,
                                                   const Boolean alwaysSendTp,
                                                    const VgmuxChannelNumber entity)
{
  SignalBuffer           sendSignal = kiNullBuffer;
  Int32                  cmdId;
  ResultCode_t           result = RESULT_CODE_OK;
  Boolean                rspResult = TRUE;
  SimatGeneralResult     generalResult = SIMAT_GR_COMM_PERF_OK;
  Boolean                meProblem = FALSE;
  SimatMeProblemAddInfo  additionalInfo = SIMAT_INFO_NO_CAUSE_GIVEN; 
  Int32                  itemId;

  PARAMETER_NOT_USED (entity);

  /* access the union signal definition */
  RspSignals.afsaSelectItemRsp.simatCommandRef = vgStkGetSimCmd ();

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_ASSIGN:   /* AT*MSTCR= */
    {
      /* decode cmd id */
      if (getExtendedParameter (commandBuffer_p, &cmdId, ULONG_MAX) == TRUE)
      {
        if (cmdId == STK_CMD_SELECT_ITEM)
        {
          rspResult = getGetSelectItemTpResult (commandBuffer_p,
                                                 &generalResult,
                                                  &meProblem,
                                                   &additionalInfo);

          RspSignals.afsaSelectItemRsp.generalResult    = generalResult;
          RspSignals.afsaSelectItemRsp.meProblemPresent = meProblem;
          RspSignals.afsaSelectItemRsp.meProblem        = additionalInfo;
          if (rspResult == FALSE)
          {
             result = VG_CME_STK_INVALID_RESPONSE_DATA;
          }
          /* encode optional itemId field */
          if (getExtendedParameter (commandBuffer_p, &itemId, ULONG_MAX) == TRUE)
          {
            if (itemId != ULONG_MAX)
            {
              RspSignals.afsaSelectItemRsp.chosenItemId = (Int8)itemId;
            }
            else
            {
              RspSignals.afsaSelectItemRsp.chosenItemId = 0;
            }
          }
          else
          {
            result = RESULT_CODE_ERROR;
          }
        }
        else
        {
          result = VG_CME_STK_INVALID_COMMAND_ID;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  /* only send the tp if we have decode the data is ok or alwaysSendTp
     has occurred */
  if ((result == RESULT_CODE_OK) || (alwaysSendTp == TRUE))
  {
    /* handle detected error cases */
    if (result != RESULT_CODE_OK)
    {
      RspSignals.afsaSelectItemRsp.generalResult    = SIMAT_GR_USER_NO_RESPONSE;
      RspSignals.afsaSelectItemRsp.meProblemPresent = TRUE;
      RspSignals.afsaSelectItemRsp.meProblem        = SIMAT_INFO_NO_CAUSE_GIVEN;
    }

    /* now send the RSP signal to the SIM */
    KiCreateZeroSignal (SIG_AFSA_SELECT_ITEM_RSP,
                         sizeof (AfsaSelectItemRsp),
                          &sendSignal);

    memcpy (&sendSignal.sig->afsaSelectItemRsp,
             &RspSignals.afsaSelectItemRsp,
              sizeof (RspSignals.afsaSelectItemRsp));

    KiSendSignal (SIMAT_TASK_ID, &sendSignal);

  }

  return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSigAfsaEncodeSetUpMenuRsp
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              alwaysSendTp    - always send profile
 *              entity          - mux channel number
 *
 * Returns:     Boolean - response not valid
 *
 * Description: Decodes the data and sets up the data string.  The accessory
 *              must use the STK protocol to retreive the data string.  The
 *              terminal profile is sent to the SIM AT task is the encode is
 *              successful. If have timed out then we must send a profile
 *              response to the SIM AT task. The response will indicate that
 *              an error has occured.
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgSigAfsaEncodeSetUpMenuRsp (CommandLine_t *commandBuffer_p,
                                                  const Boolean  alwaysSendTp,
                                                   const VgmuxChannelNumber entity)
{
  SignalBuffer           sendSignal = kiNullBuffer;
  Int32                  cmdId;
  ResultCode_t           result = RESULT_CODE_OK;
  Boolean                rspResult = TRUE;
  SimatGeneralResult     generalResult = SIMAT_GR_COMM_PERF_OK;
  Boolean                meProblem = FALSE;
  SimatMeProblemAddInfo  additionalInfo = SIMAT_INFO_NO_CAUSE_GIVEN; /* shut up RVCT */

  PARAMETER_NOT_USED (entity);

  /* access the union signal definition */
  RspSignals.afsaSetUpMenuRsp.simatCommandRef = vgStkGetSimCmd ();

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_ASSIGN:   /* AT*MSTCR= */
    {
      /* decode cmd id */
      if (getExtendedParameter (commandBuffer_p, &cmdId, ULONG_MAX) == TRUE)
      {
        if (cmdId == STK_CMD_SETUP_MENU)
        {
          rspResult = getGetSetUpMenuTpResult (commandBuffer_p,
                                                &generalResult,
                                                 &meProblem,
                                                  &additionalInfo);

          RspSignals.afsaSetUpMenuRsp.generalResult    = generalResult;
          RspSignals.afsaSetUpMenuRsp.meProblemPresent = meProblem;
          RspSignals.afsaSetUpMenuRsp.meProblem        = additionalInfo;
          if (rspResult == FALSE)
          {
             result = VG_CME_STK_INVALID_RESPONSE_DATA;
          }
        }
        else
        {
           result = VG_CME_STK_INVALID_COMMAND_ID;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  /* only send the tp if we have decode the data is ok or alwaysSendTp
     has occurred */
  if ((result == RESULT_CODE_OK) || (alwaysSendTp == TRUE))
  {
    /* handle detected error cases */
    if (result != RESULT_CODE_OK)
    {
      RspSignals.afsaSetUpMenuRsp.generalResult    = SIMAT_GR_ME_CANNOT_PROC_COMMAND;
      RspSignals.afsaSetUpMenuRsp.meProblemPresent = TRUE;
      RspSignals.afsaSetUpMenuRsp.meProblem        = SIMAT_INFO_NO_CAUSE_GIVEN;
    }
      /* now send the RSP signal to the SIM */
    KiCreateZeroSignal (SIG_AFSA_SET_UP_MENU_RSP,
                         sizeof (AfsaSetUpMenuRsp),
                          &sendSignal);

    memcpy (&sendSignal.sig->afsaSetUpMenuRsp,
             &RspSignals.afsaSetUpMenuRsp,
              sizeof (RspSignals.afsaSetUpMenuRsp));

    KiSendSignal (SIMAT_TASK_ID, &sendSignal);
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexStDisplayAlphaIdRsp
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              alwaysSendTp    - always send profile
 *              entity          - mux channel number
 *
 * Returns:     Boolean - response not valid
 *
 * Description: The response to some alphs string passed from the SIM - may be cancelled
 * or accepted by the user.  For some commands they are only part of a user termination
 * of the proactive session - some the user can decide whther or not to accept the command *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgApexStDisplayAlphaIdRsp (CommandLine_t *commandBuffer_p,
                                                const Boolean  alwaysSendTp,
                                                 const VgmuxChannelNumber entity)
{
  ResultCode_t  result = RESULT_CODE_OK;
  Int32         resValue = 0;
  Int32         cmdId = 0;
  SignalBuffer  sendSignal = kiNullBuffer;
  SimatAlphaIdType  alphaIdType = SIMAT_ALPHA_ID_NUM;

  PARAMETER_NOT_USED (entity);


  if (commandBuffer_p->length == 0)
  { 
    /* handle where no response is required */

    cmdId = vgStkGetCmdId();
    RspSignals.apexStCancelCommandReq.type = getCmdIdAlphaType (cmdId);
    if ( RspSignals.apexStCancelCommandReq.type != SIMAT_ALPHA_ID_NUM)
    {
      if ( cmdId == STK_CMD_SETUP_OPEN_CHANNEL)
      {
        /* if nothing is received from the user assume they are not accepting
         * the command */
        RspSignals.apexStCancelCommandReq.cancelCommand = SIMAT_CANCEL_COMMAND;
      }
      else  
        /* for all other commands when there is no response then user is not 
         * terminating session */
      {
        RspSignals.apexStCancelCommandReq.cancelCommand = SIMAT_NO_CANCEL;
      }
    }
  }
  else
  {
    switch (getExtendedOperation (commandBuffer_p))
    {
      case EXTENDED_ASSIGN:   /* AT*MSTCR= */
      {
        /* decode cmd id */
        if (getExtendedParameter (commandBuffer_p, &cmdId, ULONG_MAX) == TRUE)
        {
          if ( (alphaIdType = getCmdIdAlphaType (cmdId)) != SIMAT_ALPHA_ID_NUM)
          {
            if (getExtendedParameter (commandBuffer_p,
                                       &resValue,
                                        ULONG_MAX) == TRUE)
            {
              RspSignals.apexStCancelCommandReq.type  = alphaIdType;
              if ( cmdId == STK_CMD_SETUP_OPEN_CHANNEL)
              {
                switch (resValue) 
                {
                case 0: /* user accepted command */
                  RspSignals.apexStCancelCommandReq.cancelCommand = SIMAT_NO_CANCEL;
                  break;
                case 1:/* user does not accept command */
                  RspSignals.apexStCancelCommandReq.cancelCommand = SIMAT_CANCEL_COMMAND;
                  break;
                case 2:/* user terminates session */
                  RspSignals.apexStCancelCommandReq.cancelCommand = SIMAT_CANCEL_SESSION;
                  break;
                case 3: /*command accepted but failed to display icon */
                  RspSignals.apexStCancelCommandReq.cancelCommand = SIMAT_NO_CANCEL;
                  RspSignals.apexStCancelCommandReq.failedToDisplayIcon = TRUE;
                  break;
                case 4: /* the user did not respons - we assume he doesn't accept */
                  RspSignals.apexStCancelCommandReq.cancelCommand = SIMAT_CANCEL_COMMAND; 
                  break; 
                default: /* not a valid response  */
                  result = VG_CME_STK_INVALID_RESPONSE_DATA;         
                  break;
                }
              }
              else
              {
                switch (resValue) 
                {
                  case 0: /* no termination of session */
                  case 3: /* user did not respond - assume that is acceptance */
                    RspSignals.apexStCancelCommandReq.cancelCommand = SIMAT_NO_CANCEL;
                    break;
                  case 1:/* user terminates session */
                    RspSignals.apexStCancelCommandReq.cancelCommand = SIMAT_CANCEL_SESSION;
                    break;
                  case 2: /*commnad accepted but failed to display icon */
                    RspSignals.apexStCancelCommandReq.cancelCommand = SIMAT_NO_CANCEL;
                    RspSignals.apexStCancelCommandReq.failedToDisplayIcon = TRUE;
                    break;
                  default: /* not a valid response  */
                    result = VG_CME_STK_INVALID_RESPONSE_DATA;         
                    break;
                }
              }
            }
            else
            {
              result = RESULT_CODE_ERROR;
            }
          } /* check cmd id */
          else
          {
            result = VG_CME_STK_INVALID_COMMAND_ID;
          }
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
        break;
      }
      default:
      {
        result = RESULT_CODE_ERROR;
        break;
      }
    }
  }

  /* only send the tp if we have decode the data is ok or alwaysSendTp
     has occurred */
  if (((result == RESULT_CODE_OK ) && (commandBuffer_p->length != 0)) ||
       (alwaysSendTp == TRUE))
  {
    if ( result !=  RESULT_CODE_OK)
    {
      RspSignals.apexStCancelCommandReq.type = getCmdIdAlphaType (cmdId);
    }
    KiCreateZeroSignal (SIG_APEX_ST_CANCEL_COMMAND_REQ,
                         sizeof (ApexStCancelCommandReq),
                          &sendSignal);

    memcpy (&sendSignal.sig->apexStCancelCommandReq,
             &RspSignals.apexStCancelCommandReq,
              sizeof (RspSignals.apexStCancelCommandReq));

    KiSendSignal (TASK_BL_ID, &sendSignal);
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    getGetInputTpResult
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              generalResult   - result of indication
 *              meProblem       - problem flag
 *              additionalInfo  - more information on problem
 *
 * Returns:     Boolean - response not valid
 *
 * Description: Special case of tp, we do not need to send a response
 *              back to the SIM AT task.
 *
 *-------------------------------------------------------------------------*/

static Boolean getGetInputTpResult (CommandLine_t         *commandBuffer_p,
                                     SimatGeneralResult    *generalResult,
                                      Boolean               *meProblem,
                                       SimatMeProblemAddInfo *additionalInfo)
{
  Int32   result;
  Boolean status = TRUE;

  if (getExtendedParameter (commandBuffer_p, &result, ULONG_MAX) == TRUE)
  {
    switch (result)
    {
      case ULONG_MAX:
      {
        status = FALSE;
        break;
      }
      case 0:
      {
        *generalResult = SIMAT_GR_COMM_PERF_OK;
        *meProblem = FALSE;
        break;
      }
      case 1:
      {
        *generalResult = SIMAT_GR_USER_TERM_SESSION;
        *meProblem = FALSE;
        break;
      }
      case 2:
      {
        *generalResult = SIMAT_GR_USER_REQ_HELP_INFO;
        *meProblem = FALSE;
        break;
      }
      case 3:
      {
        *generalResult = SIMAT_GR_USER_REQ_BACK_MOVE;
        *meProblem = FALSE;
        break;
      }
      case 4:
      {
        *generalResult = SIMAT_GR_ME_CANNOT_PROC_COMMAND;
        *additionalInfo = SIMAT_INFO_SCREEN_BUSY;
        *meProblem = TRUE;
        break;           
      }
      case 5:
      {
        *generalResult = SIMAT_GR_COMM_PERF_OK_NO_ICON;
        *meProblem = FALSE;
        break;
      }
      case 6:
      {
        *generalResult = SIMAT_GR_USER_NO_RESPONSE;
        *meProblem = TRUE;
        *additionalInfo = SIMAT_INFO_NO_CAUSE_GIVEN;
        break;
      }
      default:
      {
        status = FALSE;
        break; 
      }
    }
  }
  else
  {
    status = FALSE; /* not optional */
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    getGetToneTpResult
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              generalResult   - result of indication
 *              meProblem       - problem flag
 *              additionalInfo  - more information on problem
 *
 * Returns:     Boolean - response not valid
 *
 * Description: Sets the terminal profile result flags for the indication
 *
 *-------------------------------------------------------------------------*/

static Boolean getGetToneTpResult (CommandLine_t         *commandBuffer_p,
                                    SimatGeneralResult    *generalResult,
                                     Boolean               *meProblem,
                                      SimatMeProblemAddInfo *additionalInfo)
{
  Int32   result;
  Boolean status = TRUE;

  if (getExtendedParameter (commandBuffer_p, &result, ULONG_MAX) == TRUE)
  {
    switch (result)
    {
      case ULONG_MAX:
      {
        status = FALSE;
        break;
      }
      case 0:
      {
        *generalResult = SIMAT_GR_COMM_PERF_OK;
        *meProblem = FALSE;
        break;
      }
      case 1:
      {
        *generalResult = SIMAT_GR_USER_TERM_SESSION;
        *meProblem = FALSE;
        break;
      }
      case 2: /* tone failed */
      {
        *generalResult = SIMAT_GR_ME_CANNOT_PROC_COMMAND;
        *meProblem = TRUE;
        *additionalInfo = SIMAT_INFO_NO_CAUSE_GIVEN;
        break;
      }
      case 3: /* tone not supported */
      {
        *generalResult = SIMAT_GR_COMM_BEYOND_ME_CAP;
        *meProblem = FALSE;
        break;
      }
      case 4: /* icon display not supported */
      {
        *generalResult = SIMAT_GR_COMM_PERF_OK_NO_ICON;
        *meProblem = FALSE;
        break;
      }
      default:
      {
        status = FALSE;
        break;
      }
    }
  }
  else
  {
    status = FALSE; /* not optional */
  }

  return (status);
}


/*--------------------------------------------------------------------------
 *
 * Function:    getGetIdleModeTextTpResult
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              generalResult   - result of indication
 *              meProblem       - problem flag
 *              additionalInfo  - more information on problem
 *
 * Returns:     Boolean - response not valid
 *
 * Description: Sets the terminal profile result flags for the indication
 *
 *-------------------------------------------------------------------------*/

static Boolean getGetIdleModeTextTpResult (CommandLine_t         *commandBuffer_p,
                                            SimatGeneralResult    *generalResult,
                                             Boolean               *meProblem,
                                              SimatMeProblemAddInfo *additionalInfo)
{
  Int32   result = 0;
  Boolean status = TRUE;

  if (getExtendedParameter (commandBuffer_p, &result, ULONG_MAX) == TRUE)
  {
    switch (result)
    {
      case ULONG_MAX:
      {
        status = FALSE;
        break;
      }
      case 0:
      {
        *generalResult = SIMAT_GR_COMM_PERF_OK;
        *meProblem = FALSE;
        break;
      }
      case 1:
      {
        *generalResult = SIMAT_GR_ME_CANNOT_PROC_COMMAND; 
        *meProblem = TRUE;
        *additionalInfo = SIMAT_INFO_NO_CAUSE_GIVEN;
        break;
      }      
      default:
      {
        status = FALSE;        
        break;
      }
    }
  }
  else
  {
    status = FALSE; /* not optional */
  }

  return (status);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgAfsaSetUpIdleModeTextRsp
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              alwaysSendTp    - always send profile
 *              entity          - mux channel number
 *
 * Returns:     Boolean - response not valid
 *
 * Description: Decodes the data and sets up the data string.  The accessory
 *              must use the STK protocol to retreive the data string.  The
 *              terminal profile is sent to the SIM AT task is the encode is
 *              successful. If have timed out then we must send a profile
 *              response to the SIM AT task. The response will indicate that
 *              an error has occured.
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgAfsaSetUpIdleModeTextRsp (CommandLine_t *commandBuffer_p,
                                                 const Boolean  alwaysSendTp,
                                                  const VgmuxChannelNumber entity)
{
  SignalBuffer              sendSignal = kiNullBuffer;
  SimatGeneralResult        generalResult = SIMAT_GR_COMM_PERF_OK;
  Boolean                   meProblem = FALSE;
  SimatMeProblemAddInfo     additionalInfo = SIMAT_INFO_NO_CAUSE_GIVEN;
  Int32                     cmdId;
  Boolean                   rspResult = TRUE;
  ResultCode_t              result = RESULT_CODE_OK;
  PARAMETER_NOT_USED (entity);

  RspSignals.afsaSetUpIdleModeTextRsp.simatCommandRef = vgStkGetSimCmd ();

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_ASSIGN:   /* AT*MSTCR= */
    {
      /* decode cmd id */
      if (getExtendedParameter (commandBuffer_p, &cmdId, ULONG_MAX) == TRUE)
      {
        if (cmdId == STK_CMD_SETUP_IDLE_MODE_TEXT)
        {
          rspResult = getGetIdleModeTextTpResult (commandBuffer_p,
                                                   &generalResult,
                                                    &meProblem,
                                                     &additionalInfo);

          RspSignals.afsaSetUpIdleModeTextRsp.generalResult = generalResult;
          RspSignals.afsaSetUpIdleModeTextRsp.meProblemPresent = meProblem;
          RspSignals.afsaSetUpIdleModeTextRsp.meProblem = additionalInfo;
          if (rspResult == FALSE)
          {
            result = VG_CME_STK_INVALID_COMMAND_ID;
          }
        }
        else
        {
          result = VG_CME_STK_INVALID_RESPONSE_DATA;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  /* only send the tp if we have decode the data is ok or alwaysSendTp
     has occurred */
  if ((result == RESULT_CODE_OK) || (alwaysSendTp == TRUE))
  {
    /* handle detected error cases */
    if (result != RESULT_CODE_OK)
    {
      RspSignals.afsaSetUpIdleModeTextRsp.generalResult    = SIMAT_GR_ME_CANNOT_PROC_COMMAND;
      RspSignals.afsaSetUpIdleModeTextRsp.meProblemPresent = TRUE;
      RspSignals.afsaSetUpIdleModeTextRsp.meProblem        = SIMAT_INFO_NO_CAUSE_GIVEN;
    }

    /* now send the RSP signal to the SIM */
    KiCreateZeroSignal (SIG_AFSA_SET_UP_IDLE_MODE_TEXT_RSP,
                         sizeof (AfsaSetUpIdleModeTextRsp),
                          &sendSignal);

    memcpy (&sendSignal.sig->afsaSetUpIdleModeTextRsp,
             &RspSignals.afsaSetUpIdleModeTextRsp,
              sizeof (RspSignals.afsaSetUpIdleModeTextRsp));

    KiSendSignal (SIMAT_TASK_ID, &sendSignal);
  }

  return (result);
}



/*--------------------------------------------------------------------------
 *
 * Function:    vgSigAfsaEncodeInkeyRsp
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              alwaysSendTp    - always send profile
 *              entity          - mux channel number
 *
 * Returns:     Boolean - response not valid
 *
 * Description: Decodes the data and sets up the data string.  The accessory
 *              must use the STK protocol to retreive the data string.  The
 *              terminal profile is sent to the SIM AT task is the encode is
 *              successful. If have timed out then we must send a profile
 *              response to the SIM AT task. The response will indicate that
 *              an error has occured.
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t vgSigAfsaEncodeInkeyRsp (CommandLine_t *commandBuffer_p,
                                              const Boolean  alwaysSendTp,
                                               const VgmuxChannelNumber entity)
{
  #define MAX_CHARACTERS_SIMAT (274)
  #define MIN_DISPLAY_DURATION  (1)
  #define MAX_DISPLAY_DURATION  (255)

  SignalBuffer           sendSignal = kiNullBuffer;
  Int32                  cmdId;
  Boolean                rspResult = TRUE;
  ResultCode_t           result = RESULT_CODE_OK;
  Char                   tmpString [MAX_CHARACTERS_SIMAT + NULL_TERMINATOR_LENGTH] = {0};
  Int32                  dcs = MSG_CODING_RESERVED;
  SimatGeneralResult     generalResult = SIMAT_GR_COMM_PERF_OK;
  Boolean                meProblem = FALSE;
  SimatMeProblemAddInfo  additionalInfo = SIMAT_INFO_NO_CAUSE_GIVEN;
  Int16                  extendedStrLen = 0;
  StkEntityGenericData_t *stkGenericContext_p = ptrToStkGenericContext ();
  Int32                  durationUnit;
  Int32                  durationValue;

  /* access the union signal definition */
  RspSignals.afsaGetInkeyRsp.simatCommandRef = stkGenericContext_p->simCommandId;

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_ASSIGN:   /* AT*MSTCR= */
    {
      /* decode cmd id */
      if ( getExtendedParameter (commandBuffer_p, &cmdId, ULONG_MAX ) == TRUE)
      {
        if (cmdId == STK_CMD_GET_INKEY)
        {
          rspResult = getGetInputTpResult (commandBuffer_p,
                                            &generalResult,
                                             &meProblem,
                                              &additionalInfo);
          RspSignals.afsaGetInkeyRsp.generalResult = generalResult;
          RspSignals.afsaGetInkeyRsp.meProblemPresent = meProblem;
          RspSignals.afsaGetInkeyRsp.meProblem = additionalInfo;
          if (rspResult == FALSE)
          { 
            result = VG_CME_STK_INVALID_RESPONSE_DATA;
          }
        }
        else
        {
          result = VG_CME_STK_INVALID_COMMAND_ID;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  /* decode the dcs */
  if (result == RESULT_CODE_OK)
  {
    if (getExtendedParameter (commandBuffer_p, &dcs, ULONG_MAX) == TRUE)
    {
      if (dcs == ULONG_MAX)
      {
        /* decoded the result okay and the input was perfomed ok.  Special
           case is must supply a dcs and text */
        if (RspSignals.afsaGetInkeyRsp.generalResult == SIMAT_GR_COMM_PERF_OK)
        {
          result = VG_CME_STK_INVALID_RESPONSE_DATA;
        }

        RspSignals.afsaGetInkeyRsp.keyDigit.length = 0;

        memset (RspSignals.afsaGetInkeyRsp.keyDigit.textString,
                '\0',
                 sizeof(RspSignals.afsaGetInkeyRsp.keyDigit.textString));

        RspSignals.afsaGetInkeyRsp.keyDigit.codingScheme.msgCoding = (MsgCoding)0x00;
      }
      else
      {
        /*
          CLASS FIELD is not used, so needs to be set to CLASS_NOT_GIVEN
          for dcs encode function to work correctly
         */
        RspSignals.afsaGetInkeyRsp.keyDigit.codingScheme.msgClass = MSG_CLASS_NOT_GIVEN;

        switch (dcs)
        {
          case STK_DCS_DEFAULT:
          {
            RspSignals.afsaGetInkeyRsp.keyDigit.codingScheme.
             msgCoding = MSG_CODING_DEFAULT;
            break;
          }
          case STK_DCS_EIGHT_BIT:
          {
            RspSignals.afsaGetInkeyRsp.keyDigit.codingScheme.
             msgCoding = MSG_CODING_8BIT;
            break;
          }
          case STK_DCS_UCS2:
          {
            RspSignals.afsaGetInkeyRsp.keyDigit.codingScheme.
             msgCoding = MSG_CODING_UCS2;
            break;
          }
          default:
          {
            result = VG_CME_STK_INVALID_RESPONSE_DATA;
            break;
          }
        }

        /* now get the text string */
        if (vgStkOutputInTextMode (entity))
        {

          if (getExtendedString (commandBuffer_p,
                                 (Char *)tmpString,
                                  MAX_CHARACTERS_SIMAT,
                                   &extendedStrLen) == TRUE)
          {
            strncpy ((char*)RspSignals.afsaGetInkeyRsp.keyDigit.textString,
                      (char*)tmpString,
                       MAX_CHARACTERS_SIMAT);

            RspSignals.afsaGetInkeyRsp.keyDigit.length = (Int16)strlen ((char*)tmpString);
          }
          else
          {
            result = VG_CME_STK_INVALID_RESPONSE_DATA;
          }
        }
        else
        {
          Int16 sizeOfTextString = vgStkGetArrayFromHex (commandBuffer_p, tmpString);

          if (sizeOfTextString != 0)
          {
            memcpy ((void *)RspSignals.afsaGetInkeyRsp.keyDigit.textString,
                    (void *)tmpString,
                     sizeOfTextString);
            RspSignals.afsaGetInkeyRsp.keyDigit.length = sizeOfTextString;
          }
          else
          {
            result = VG_CME_STK_INVALID_RESPONSE_DATA;
          }
        }
        /* check for total display duration parameters  - optional parameter */
        /* assume that duration is not present unless the values are present and within the valid limits */
        
        RspSignals.afsaGetInkeyRsp.durationPresent = FALSE;
        if ( getExtendedParameter (commandBuffer_p, &durationUnit, ULONG_MAX ) == TRUE)
        {
          /* a valid parameter for durationUnit is 0 - 2 */
          if ( durationUnit <= SIMAT_TIME_UNIT_TENTHS ) 
          {
            if ( getExtendedParameter (commandBuffer_p, &durationValue, ULONG_MAX ) == TRUE)
            {
              if (( durationValue >= MIN_DISPLAY_DURATION ) && ( durationValue <= MAX_DISPLAY_DURATION ))
              {
                 RspSignals.afsaGetInkeyRsp.durationPresent = TRUE;
                 RspSignals.afsaGetInkeyRsp.durationUnit = (SimatTimeUnit)durationUnit;
                 RspSignals.afsaGetInkeyRsp.durationValue = (Int8)durationValue;
              }
            }  
          }
        }
      }
    }
  }

  /* only send the tp if we have decode the data ok or a alwaysSendTp
    has occurred */
  if ((result == RESULT_CODE_OK) || (alwaysSendTp == TRUE))
  {
    /* handle detected error cases */
    if (result != RESULT_CODE_OK)
    {
      RspSignals.afsaGetInkeyRsp.generalResult = SIMAT_GR_USER_NO_RESPONSE;
      RspSignals.afsaGetInkeyRsp.meProblemPresent = TRUE;
      RspSignals.afsaGetInkeyRsp.meProblem = SIMAT_INFO_NO_CAUSE_GIVEN;
    }

    /* now send the RSP signal to the SIM */
    KiCreateZeroSignal (SIG_AFSA_GET_INKEY_RSP,
                         sizeof (AfsaGetInkeyRsp),
                          &sendSignal);

    memcpy (&sendSignal.sig->afsaGetInkeyRsp,
             &RspSignals.afsaGetInkeyRsp,
              sizeof (RspSignals.afsaGetInkeyRsp));

    KiSendSignal (SIMAT_TASK_ID, &sendSignal);
  }

  return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSigAfsaEncodeGetInputRsp
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              alwaysSendTp    - always send profile
 *              entity          - mux channel number
 *
 * Returns:     Boolean - response not valid
 *
 * Description: Decodes the data and sets up the data string.  The accessory
 *              must use the STK protocol to retreive the data string.  The
 *              terminal profile is sent to the SIM AT task is the encode is
 *              successful. If have timed out then we must send a profile
 *              response to the SIM AT task .The response will indicate that
 *              an error has occured.
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgSigAfsaEncodeGetInputRsp (CommandLine_t *commandBuffer_p,
                                                 const Boolean  alwaysSendTp,
                                                  const VgmuxChannelNumber entity)
{
  #define MAX_STRING_LENGTH_160 (160)

  SignalBuffer           sendSignal = kiNullBuffer;
  Int32                  cmdId;
  Boolean                rspResult = TRUE;
  ResultCode_t           result = RESULT_CODE_OK;
  Char                   tmpString [MAX_STRING_LENGTH_160 + NULL_TERMINATOR_LENGTH] = {0};
  Int32                  dcs = MSG_CODING_RESERVED;
  SimatGeneralResult     generalResult = SIMAT_GR_COMM_PERF_OK;
  Boolean                meProblem = FALSE;
  SimatMeProblemAddInfo  additionalInfo = SIMAT_INFO_NO_CAUSE_GIVEN;
  Int16                  extendedStrLen = 0;
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  /* access the union signal definition */
  RspSignals.afsaGetInputRsp.simatCommandRef = stkGenericContext_p->simCommandId;

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_ASSIGN:   /* AT*MSTCR= */
    {
      /* decode cmd id */
      if (getExtendedParameter (commandBuffer_p, &cmdId, ULONG_MAX) == TRUE)
      {
        if (cmdId == STK_CMD_GET_INPUT)
        {
          rspResult = getGetInputTpResult (commandBuffer_p,
                                            &generalResult,
                                             &meProblem,
                                               &additionalInfo);

          RspSignals.afsaGetInputRsp.generalResult    = generalResult;
          RspSignals.afsaGetInputRsp.meProblemPresent = meProblem;
          RspSignals.afsaGetInputRsp.meProblem        = additionalInfo;
         
          if (rspResult == FALSE)
          {
            result = VG_CME_STK_INVALID_RESPONSE_DATA;
          }
        }
        else
        {
          result = VG_CME_STK_INVALID_COMMAND_ID;
        }
      } /* decode cmd id */
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  /* decode the optional dcs */
  if (result == RESULT_CODE_OK)
  {
    if (getExtendedParameter (commandBuffer_p, &dcs, ULONG_MAX) == TRUE)
    {
      if (dcs == ULONG_MAX)
      {
        /*when dcs is ULONG_MAX, maybe user don't input anything. stk should also work ok. compared to NOKIA ue*/
        RspSignals.afsaGetInputRsp.textString.length = 0;

        memset (RspSignals.afsaGetInputRsp.textString.textString,
                 '\0',
                  sizeof(RspSignals.afsaGetInputRsp.textString.textString));
        RspSignals.afsaGetInputRsp.textString.codingScheme.msgCoding = (MsgCoding)0x00;
      }
      else
      {
        /*
           CLASS FIELD is not used, so needs to be set to CLASS_NOT_GIVEN
           for dcs encode function to work correctly
         */

        RspSignals.afsaGetInputRsp.textString.codingScheme.msgClass = MSG_CLASS_NOT_GIVEN;

        switch (dcs)
        {
          case STK_DCS_DEFAULT:
          {
            RspSignals.afsaGetInputRsp.textString.codingScheme.
             msgCoding = MSG_CODING_DEFAULT;
            break;
          }
          case STK_DCS_EIGHT_BIT:
          {
            RspSignals.afsaGetInputRsp.textString.codingScheme.
             msgCoding = MSG_CODING_8BIT;
            break;
          }
          case STK_DCS_UCS2:
          {
            RspSignals.afsaGetInputRsp.textString.codingScheme.
             msgCoding = MSG_CODING_UCS2;
            break;
          }
          default:
          {
            result = VG_CME_STK_INVALID_RESPONSE_DATA;
            break;
          }
        }

        /* text */
        if (result == RESULT_CODE_OK)
        {
          if (vgStkOutputInTextMode (entity))
          {
            if (getExtendedString (commandBuffer_p, tmpString, MAX_STRING_LENGTH_160, &extendedStrLen) == TRUE)
            {
              if (stkGenericContext_p->minInputRange == stkGenericContext_p->maxInputRange)
              {
                if ((Int16)vgStrLen (tmpString) != (Int16)stkGenericContext_p->minInputRange)
                {
                  result = VG_CME_STK_INVALID_RESPONSE_DATA;
                }
              }
              else if (((Int16)vgStrLen (tmpString) < (Int16)stkGenericContext_p->minInputRange) ||
                       ((Int16)vgStrLen (tmpString) > (Int16)stkGenericContext_p->maxInputRange))
              {
                result = VG_CME_STK_INVALID_RESPONSE_DATA;
              }
              if (result == RESULT_CODE_OK)
              {
                strncpy ((char*)RspSignals.afsaGetInputRsp.textString.textString,
                          (char*)tmpString,
                           strlen ((char*)tmpString));
                RspSignals.afsaGetInputRsp.textString.length = (Int16)strlen ((char*)tmpString);
              }
            }
            else
            {
              result = RESULT_CODE_ERROR;
            }
          }
          else
          {
            Int16 sizeOfTextString = vgStkGetArrayFromHex((CommandLine_t *)commandBuffer_p, tmpString);

            if (sizeOfTextString != 0)
            { 
              memcpy((void *)RspSignals.afsaGetInputRsp.textString.textString, 
                     (void *)tmpString,
                     sizeOfTextString);
              RspSignals.afsaGetInputRsp.textString.length = sizeOfTextString;
            }
            else
            {
              result = VG_CME_STK_INVALID_RESPONSE_DATA;
            }
          }
        }
      }
    }
  }

  /* only send the tp if we have decode the data ok or a alwaysSendTp
     has occurred */
  if ((result == RESULT_CODE_OK) || (alwaysSendTp == TRUE))
  {
    /* handle detected error cases */
    if (result != RESULT_CODE_OK)
    {
      RspSignals.afsaGetInputRsp.generalResult    = SIMAT_GR_USER_NO_RESPONSE;
      RspSignals.afsaGetInputRsp.meProblemPresent = TRUE;
      RspSignals.afsaGetInputRsp.meProblem        = SIMAT_INFO_NO_CAUSE_GIVEN;
    }

    /* now send the RSP signal to the SIM */
    KiCreateZeroSignal (SIG_AFSA_GET_INPUT_RSP,
                         sizeof (AfsaGetInputRsp),
                          &sendSignal);

    memcpy (&sendSignal.sig->afsaGetInputRsp,
             &RspSignals.afsaGetInputRsp,
              sizeof (RspSignals.afsaGetInputRsp));

    KiSendSignal (SIMAT_TASK_ID, &sendSignal);
  }

  return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSigAfsaEncodeToneRsp
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              alwaysSendTp    - always send profile
 *              entity          - mux channel number
 *
 * Returns:     Boolean - response not valid
 *
 * Description: Decodes the data and sets up the data string.  The accessory
 *              must use the STK protocol to retreive the data string.  The
 *              terminal profile is sent to the SIM AT task is the encode is
 *              successful. If have timed out then we must send a profile
 *              response to the SIM AT task. The response will indicate that
 *              an error has occured.
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgSigAfsaEncodeToneRsp (CommandLine_t *commandBuffer_p,
                                             const Boolean alwaysSendTp,
                                              const VgmuxChannelNumber entity)
{
  SignalBuffer           sendSignal = kiNullBuffer;
  Int32                  cmdId;
  Boolean                rspResult = TRUE;
  ResultCode_t           result = RESULT_CODE_OK;
  SimatGeneralResult     generalResult = SIMAT_GR_COMM_PERF_OK;
  Boolean                meProblem = FALSE;
  SimatMeProblemAddInfo  additionalInfo = SIMAT_INFO_NO_CAUSE_GIVEN;

  PARAMETER_NOT_USED (entity);

  /* access the union signal definition */
  RspSignals.afsaPlayToneRsp.simatCommandRef = vgStkGetSimCmd ();

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_ASSIGN:   /* AT*MSTCR= */
    {
      /* decode cmd id */
      if (getExtendedParameter (commandBuffer_p, &cmdId, ULONG_MAX) == TRUE)
      {
        if (cmdId == STK_CMD_PLAY_TONE)
        {
          rspResult = getGetToneTpResult (commandBuffer_p,
                                           &generalResult,
                                            &meProblem,
                                             &additionalInfo);
          RspSignals.afsaPlayToneRsp.generalResult = generalResult;
          RspSignals.afsaPlayToneRsp.meProblemPresent = meProblem;
          RspSignals.afsaPlayToneRsp.meProblem = additionalInfo;
          if (rspResult == FALSE)
          {
            result = VG_CME_STK_INVALID_RESPONSE_DATA;
          }
        }
        else
        {
          result = VG_CME_STK_INVALID_COMMAND_ID;
        }
      } /* decode cmd id */
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  /* only send the tp if we have decode the data ok or a alwaysSendTp
     has occurred */
  if ((result == RESULT_CODE_OK) || (alwaysSendTp == TRUE))
  {
    /* handle detected error cases */
    if (result != RESULT_CODE_OK)
    {
      /** ME currently unable to process command */
      RspSignals.afsaPlayToneRsp.generalResult = SIMAT_GR_ME_CANNOT_PROC_COMMAND;
      RspSignals.afsaPlayToneRsp.meProblemPresent = TRUE;
      RspSignals.afsaPlayToneRsp.meProblem = SIMAT_INFO_NO_CAUSE_GIVEN;
    }

    /* now send the RSP signal to the SIM */
    KiCreateZeroSignal (SIG_AFSA_PLAY_TONE_RSP,
                         sizeof (AfsaPlayToneRsp),
                          &sendSignal);

    memcpy (&sendSignal.sig->afsaPlayToneRsp,
             &RspSignals.afsaPlayToneRsp,
              sizeof (RspSignals.afsaPlayToneRsp));

    KiSendSignal (SIMAT_TASK_ID, &sendSignal);
  }

  return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSigSetUpEventListRsp
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              alwaysSendTp    - always send profile
 *              entity          - mux channel number
 *
 * Returns:     Boolean - response not valid
 *
 * Description: Decodes the data and sets up the data string.  The accessory
 *              must use the STK protocol to retreive the data string.  The
 *              terminal profile is sent to the SIM AT task is the encode is
 *              successful. If have timed out then we must send a profile
 *              response to the SIM AT task. The response will indicate that
 *              an error has occured.
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgSigSetUpEventListRsp (CommandLine_t *commandBuffer_p,
                                             const Boolean  alwaysSendTp,
                                              const VgmuxChannelNumber entity)
{
  SignalBuffer sendSignal = kiNullBuffer;
  Int32        cmdId;
  ResultCode_t result = RESULT_CODE_OK;
  Int32        resValue = 0;
  StkEntityGenericData_t *stkGenericContext_p = ptrToStkGenericContext ();

  PARAMETER_NOT_USED (entity);

  /* access the union signal definition */
  RspSignals.afsaSetUpEventListRsp.simatCommandRef = vgStkGetSimCmd ();

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_ASSIGN:   /* AT*MSTCR= */
    {
      /* decode data from the message */
      if (getExtendedParameter (commandBuffer_p, &cmdId, ULONG_MAX) == TRUE)
      {
        if (cmdId == STK_CMD_SETUP_EVENT_LIST)
        {
          if (getExtendedParameter (commandBuffer_p, &resValue, ULONG_MAX) == TRUE)
          {
            if (resValue > 1)
            {
              result = VG_CME_STK_INVALID_RESPONSE_DATA;
            }
            else if (resValue == 1)
            {
              /* see 102.223 section 6.4.16 */
              RspSignals.afsaSetUpEventListRsp.generalResult    = SIMAT_GR_COMM_BEYOND_ME_CAP;  
              RspSignals.afsaSetUpEventListRsp.meProblemPresent = TRUE;
              RspSignals.afsaSetUpEventListRsp.meProblem        = SIMAT_INFO_NO_CAUSE_GIVEN;
              /* clear the currently mionitored list */
              memset(&stkGenericContext_p->currentEventList, FALSE, sizeof(StkMmiEventList));
            }
            else
            {
              RspSignals.afsaSetUpEventListRsp.generalResult    = SIMAT_GR_COMM_PERF_OK;
              RspSignals.afsaSetUpEventListRsp.meProblemPresent = FALSE;
            }
          }
          else
          {
            result = RESULT_CODE_ERROR;
          }
        }
        else
        {
          result = VG_CME_STK_INVALID_COMMAND_ID;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    default:
    {
       result = RESULT_CODE_ERROR;
       break;
    }
  }

  /* only send the tp if we have decode the data ok or a alwaysSendTp
     has occurred */

  if ((result == RESULT_CODE_OK) || (alwaysSendTp == TRUE))
  {
    /* handle any error cases */
    if (result != RESULT_CODE_OK)
    {
      RspSignals.afsaSetUpEventListRsp.generalResult    = SIMAT_GR_ME_CANNOT_PROC_COMMAND;
      RspSignals.afsaSetUpEventListRsp.meProblemPresent = TRUE;
      RspSignals.afsaSetUpEventListRsp.meProblem        = SIMAT_INFO_NO_CAUSE_GIVEN;
    }

    /* now send the terminal response to the SIM */
    KiCreateZeroSignal (SIG_AFSA_SET_UP_EVENT_LIST_RSP,
                         sizeof (AfsaSetUpEventListRsp),
                          &sendSignal);

    memcpy (&sendSignal.sig->afsaSetUpEventListRsp,
             &RspSignals.afsaSetUpEventListRsp,
              sizeof (RspSignals.afsaSetUpEventListRsp));

    KiSendSignal (SIMAT_TASK_ID, &sendSignal);
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    getGetLaunchBrowserTpResult
 *
 * Parameters:  commandBuffer_p    - pointer to cmmand line string
 *              generalResult      - result of indication
 *              meProblem          - problem flag
 *              additionalInfo     - more information on problem
 *              browserProblem     - problem code
 *              browserProbPresent - problem present
 *
 * Returns:     Boolean - response not valid
 *
 * Description: Sets the terminal profile result flags for the indication
 *
 *-------------------------------------------------------------------------*/

static Boolean getGetLaunchBrowserTpResult (CommandLine_t *commandBuffer_p,
                                             SimatGeneralResult  *generalResult,
                                              Boolean  *meProblem,
                                               SimatMeProblemAddInfo  *additionalInfo,
                                                SimatLauBrProblemAddInfo  *browserProblem,
                                                  Boolean  *browserProbPresent)
{
  Int32      result;
  Boolean    status = TRUE;

  if (getExtendedParameter (commandBuffer_p, &result, ULONG_MAX) == TRUE)
  {
    switch (result)
    {
      case ULONG_MAX:
      {
        status = FALSE;
        break;
      }
      case 0:
      {
        *generalResult      = SIMAT_GR_COMM_PERF_OK;
        *meProblem          = FALSE;
        *browserProbPresent = FALSE;
        break;
      }
      case 1:
      {
        *generalResult      = SIMAT_GR_BROWSER_GEN_ERROR;
        *meProblem          = FALSE;
        *browserProbPresent = TRUE;
        *browserProblem     = SIMAT_LB_INFO_BEARER_NOT_AVAIL;
        break;
      }
      case 2:
      {
        *generalResult      = SIMAT_GR_BROWSER_GEN_ERROR;
        *meProblem          = FALSE;
        *browserProbPresent = TRUE;
        *browserProblem     = SIMAT_LB_INFO_BROWSER_NOT_AVAIL;
        break;
      }
      case 3:
      {
        *generalResult      = SIMAT_GR_BROWSER_GEN_ERROR;
        *meProblem          = FALSE;
        *browserProbPresent = TRUE;
        *browserProblem     = SIMAT_LB_INFO_NO_PROV_DATA;
        break;
      }
      case 4:
      {
        *generalResult      = SIMAT_GR_BROWSER_GEN_ERROR;
        *meProblem          = FALSE;
        *browserProbPresent = TRUE;
        *browserProblem     = SIMAT_LB_INFO_NO_CAUSE_GIVEN;

        break;
      }
      default:
      {
        status = FALSE;      
        break;
      }
    }
  }
  else
  {
    status = FALSE; /* not optional */
  }

  return (status);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgAfsaSetUpIdleModeTextRsp
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              alwaysSendTp    - always send profile
 *              entity          - mux channel number
 *
 * Returns:     Boolean - response not valid
 *
 * Description: Sends the output data to the accessory unsolicited  followed
 *              by the terminal response.
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgAfsaLanguageNotificationRsp (CommandLine_t *commandBuffer_p,
                                                    const Boolean  alwaysSendTp,
                                                     const VgmuxChannelNumber entity)
{
  SignalBuffer  sendSignal = kiNullBuffer;
  ResultCode_t  result = RESULT_CODE_OK;

  PARAMETER_NOT_USED (entity);
  PARAMETER_NOT_USED (alwaysSendTp);
  PARAMETER_NOT_USED (commandBuffer_p);

  RspSignals.afsaLanguageNotificationRsp.simatCommandRef = vgStkGetSimCmd ();

  if (vgStkIsDataValid () == FALSE)
  {
    RspSignals.afsaLanguageNotificationRsp.generalResult    = SIMAT_GR_USER_NO_RESPONSE;
    RspSignals.afsaLanguageNotificationRsp.meProblemPresent = TRUE;
    RspSignals.afsaLanguageNotificationRsp.meProblem        = SIMAT_INFO_NO_CAUSE_GIVEN;
  }
  else
  {
    RspSignals.afsaLanguageNotificationRsp.generalResult    = SIMAT_GR_COMM_PERF_OK;
    RspSignals.afsaLanguageNotificationRsp.meProblemPresent = FALSE;
  }

  /* now send the RSP signal to the SIM */
  KiCreateZeroSignal (SIG_AFSA_LANGUAGE_NOTIFICATION_RSP,
                       sizeof (AfsaLanguageNotificationRsp),
                        &sendSignal);

  memcpy (&sendSignal.sig->afsaLanguageNotificationRsp,
           &RspSignals.afsaLanguageNotificationRsp,
            sizeof (RspSignals.afsaLanguageNotificationRsp));

  KiSendSignal (SIMAT_TASK_ID, &sendSignal);

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgAfsaLaunchBrowserRsp
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              alwaysSendTp    - always send profile
 *              entity          - mux channel number
 *
 * Returns:     Boolean - response not valid
 *
 * Description: Decodes the data and sets up the data string.  The accessory
 *              must use the STK protocol to retreive the data string.  The
 *              terminal profile is sent to the SIM AT task is the encode is
 *              successful. If have timed out then we must send a profile
 *              response to the SIM AT task. The response will indicate that
 *              an error has occured.
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgAfsaLaunchBrowserRsp (CommandLine_t *commandBuffer_p,
                                             const Boolean  alwaysSendTp,
                                              const VgmuxChannelNumber entity)
{
  SignalBuffer              sendSignal = kiNullBuffer;
  SimatGeneralResult        generalResult = SIMAT_GR_COMM_PERF_OK;
  Boolean                   meProblem;
  SimatLauBrProblemAddInfo  browserProblem = SIMAT_LB_INFO_NO_CAUSE_GIVEN;
  Boolean                   browserProbPresent = FALSE;
  SimatMeProblemAddInfo     additionalInfo = SIMAT_INFO_NO_CAUSE_GIVEN;
  Int32                     cmdId;
  Boolean                   rspResult = TRUE;
  ResultCode_t              result = RESULT_CODE_OK;
  PARAMETER_NOT_USED (entity);

  RspSignals.afsaLaunchBrowserRsp.simatCommandRef = vgStkGetSimCmd ();

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_ASSIGN:   /* AT*MSTCR= */
    {
      /* decode cmd id */
      if (getExtendedParameter (commandBuffer_p, &cmdId, ULONG_MAX) == TRUE)
      {
        if (cmdId == STK_CMD_SETUP_LAUNCH_BROWSER)
        {
          rspResult = getGetLaunchBrowserTpResult (commandBuffer_p,
                                                    &generalResult,
                                                     &meProblem,
                                                      &additionalInfo,
                                                       &browserProblem,
                                                        &browserProbPresent);

          if (rspResult == FALSE)
          {
            result = VG_CME_STK_INVALID_RESPONSE_DATA;
          }
          RspSignals.afsaLaunchBrowserRsp.generalResult         = generalResult;
          RspSignals.afsaLaunchBrowserRsp.meProblemPresent      = meProblem;
          RspSignals.afsaLaunchBrowserRsp.meProblem             = additionalInfo;
          RspSignals.afsaLaunchBrowserRsp.browserProblemPresent = browserProbPresent;
          RspSignals.afsaLaunchBrowserRsp.browserProblem        = browserProblem;
        }
        else
        {
          result = VG_CME_STK_INVALID_COMMAND_ID;
        }
      }
      else
      {
        result = RESULT_CODE_OK;
      }
      break;
    }
    default:
    {
      result = RESULT_CODE_OK;
      break;
    }
  }

  /* only send the tp if we have decode the data is ok or alwaysSendTp
     has occurred */
  if ((result == RESULT_CODE_OK) || (alwaysSendTp == TRUE))
  {
    /* handle detected error cases */
    if (result != RESULT_CODE_OK)
    {
      RspSignals.afsaLaunchBrowserRsp.generalResult = SIMAT_GR_BROWSER_GEN_ERROR;
      RspSignals.afsaLaunchBrowserRsp.meProblemPresent = FALSE;
      RspSignals.afsaLaunchBrowserRsp.browserProblemPresent = TRUE;
      RspSignals.afsaLaunchBrowserRsp.browserProblem = SIMAT_LB_INFO_NO_CAUSE_GIVEN; 
    }

    /* now send the RSP signal to the SIM */
    KiCreateZeroSignal (SIG_AFSA_LAUNCH_BROWSER_RSP,
                         sizeof (AfsaLaunchBrowserRsp),
                          &sendSignal);

    memcpy (&sendSignal.sig->afsaLaunchBrowserRsp,
             &RspSignals.afsaLaunchBrowserRsp,
              sizeof (RspSignals.afsaLaunchBrowserRsp));

    KiSendSignal (SIMAT_TASK_ID, &sendSignal);
  }

  return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:    getAlphaCmdId
 *
 * Parameters:  signal    - signal to be processed
 *              result    - success of id search
 *
 * Returns:     StkCommandId - the command id reference
 *
 * Description: The alpha id signal represents more than one signal, so
 *              we need to return the correst cmd id.
 *
 *-------------------------------------------------------------------------*/

static StkCommandId getAlphaCmdId (const SignalBuffer signal,
                                    Boolean  *result)
{
  ApexStDisplayAlphaIdInd signalData = signal.sig->apexStDisplayAlphaIdInd;
  StkCommandId            commandId  = STK_END_OF_TRANSACTION;

  *result = TRUE;

  switch (signalData.type)
  {
    case SIMAT_OP_ALPHA_ID: /* open channel */
    {
      commandId = STK_CMD_SETUP_OPEN_CHANNEL;
      break;
    }
    case SIMAT_SE_ALPHA_ID: /* send data */
    {
      commandId = STK_CMD_SETUP_SEND_DATA;
      break;
    }    
    case SIMAT_RE_ALPHA_ID: /* receive data */
    {
      commandId = STK_CMD_SETUP_RECEIVE_DATA;
      break;
    }
    case SIMAT_CL_ALPHA_ID: /* close channel */
    {
      commandId = STK_CMD_SETUP_CLOSE_CHANNEL;
      break;
    }
    case SIMAT_SD_ALPHA_ID: /* send dtmf */
    {
      commandId = STK_CMD_SETUP_SEND_DTMF;
      break;
    }
    case SIMAT_SM_ALPHA_ID: /* send sm */
    {
      commandId = STK_CMD_SETUP_SM;
      break;
    }
    case SIMAT_SS_ALPHA_ID: /* send ss */
    {
      commandId = STK_CMD_SETUP_SS;
      break;
    }
    case SIMAT_USSD_ALPHA_ID:/* send USSD */
    {
        commandId = STK_CMD_SETUP_USSD;
        break;
    }        
    /* job 137614 include Call control on SIM messages */
   case SIMAT_CC_ALPHA_ID:
   case SIMAT_CC_SECOND_ALPHA_ID:
    {
      commandId = STK_CMD_SETUP_CALL;
      break;
    }
    case SIMAT_GP_CC_ALPHA_ID :
    {
      commandId = STK_CMD_GPRS_CALL_CONTROL;
      break;
    }
    default:
    {
      *result = FALSE;
      break;
    }
  }

  return (commandId);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkSignalToCmdId
 *
 * Parameters:  signal    - signal to be processed
 *              commandId - command identifier match from signal
 *
 * Returns:     StkCommandType - the command type reference
 *
 * Description: This routine returns the type of command or an error
 *
 *-------------------------------------------------------------------------*/

StkCommandType vgStkSignalToCmdId (const SignalBuffer signal,
                                    StkCommandId *commandId)
{
  const StkEncodeSignalEntityControl *signalTable = &stkEncodeSignalEntityTable[0];
  Boolean        foundType    = FALSE;
  Boolean        foundId      = FALSE;
  StkCommandType commandType  = STK_COMMAND_ERROR;

  /* search through STK signal indications for a type match */
  while ((signalTable->signalType != SIG_SYS_DUMMY) &&
         (foundType == FALSE))
  {
    if (signalTable->signalType == *(signal.type))
    {
      foundType = TRUE;

      if (signalTable->commandId == STK_CMD_SETUP_DISPLAY_ALPHA_ID)
      {
        *commandId = getAlphaCmdId (signal, &foundId);
      }
      else
      {
        *commandId = signalTable->commandId;

        foundId = TRUE;
      }

      /* if a command id is matched then set return command type */
      if (foundId == TRUE)
      {
        commandType = signalTable->commandType;
      }
    }
    else
    {
      signalTable++;
    }
  }

  return (commandType);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSetStkRegFlag
 *
 * Parameters:  regReceived - registration with STK received flag
 *              entity     - mux channel number
 *
 * Returns:     nothing
 *
 * Description: sets the stk registration received flag
 *
 *-------------------------------------------------------------------------*/

static void vgSetStkRegFlag (const Boolean regReceived,
             const VgmuxChannelNumber entity)
{
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  stkGenericContext_p->registeredForStk [entity] = regReceived;
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgStkAfsaStRegisteredTaskReq
 *
 * Parameters:  entity         - mux channel number
 *              regRequired    - registration required
 *              downlinkTpData - profile data
 *
 * Returns:     nothing
 *
 * Description: Function sends the registration request signal to the SIM
 *              AT task.  This signal will only be sent after the SIM has sent
 *              CI an indication that is calpable of proactive behaviour.
 *              Furthermore, the acccessory must have sent the downlink
 *              terminal profile AT command.
 *
 *              An entity will only get to this routine if the STK is not
 *              locked. The guard on *MSTPD AT cmd in stk.c stops access once
 *              the STK is locked.
 *
 *-------------------------------------------------------------------------*/

void vgStkAfsaStRegisteredTaskReq (const VgmuxChannelNumber entity,
                                           const  Boolean  regRequired)
{
  SignalBuffer            sendSignal = kiNullBuffer;

  PARAMETER_NOT_USED (entity);

  /* send the registration request off */
  sendSsRegistrationSignal (SIM_TOOLKIT,  /* just the stk sub-sys */
                             entity,
                              SIG_AFSA_ST_REGISTER_TASK_CNF);

  KiCreateZeroSignal (SIG_AFSA_ST_REGISTER_TASK_REQ,
                       sizeof (AfsaStRegisterTaskReq),
                        &sendSignal);

  sendSignal.sig->afsaStRegisterTaskReq.isRegistered = regRequired;
  sendSignal.sig->afsaStRegisterTaskReq.taskId = VG_CI_TASK_ID;

  KiSendSignal (SIMAT_TASK_ID, &sendSignal);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkAfsaStRegisteredTaskCnf
 *
 * Parameters:  signalBuffer - structure containing signal:
 *                             AFSA_ST_REGISTER_TASK_CNF
 *              entity       - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Function is called when the SIM AT task sends CI the
 *              a confirmation following receipt of the registration signal.
 *              Just set the current result code to ok. If the isRegistered
 *              flag is false then we must have been registered and we are
 *              deregistering so inform the application. The only case where
 *              this does not apply is if the *MSTPD command is entered before
 *              the PUK.
 *
 *-------------------------------------------------------------------------*/

void vgStkAfsaStRegisteredTaskCnf (const SignalBuffer signalBuffer,
                                    const VgmuxChannelNumber entity)
{
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();
                                            
  setResultCode (entity, RESULT_CODE_OK);

  if (signalBuffer.sig->afsaStRegisterTaskCnf.isRegistered == FALSE)
  {
    if (vgOpManDropConnection (entity, STK_CONNECTION) == TRUE)
    {
      vgSetStkRegFlag (FALSE, entity);  /* STK is free */ 
      stkGenericContext_p->registeredStkEntity = VGMUX_CHANNEL_INVALID;
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkHandleAccessoryStateChange
 *
 * Parameters:  signalBuffer - mux channel number
 *              entity       - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Called when we receive an AT commnad to lock STK to an entity.  
 *              Function checks the previous states to decide whether we should 
 *              take the STK CI offline or bring it online.  
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgStkHandleAccessoryStateChange (const VgmuxChannelNumber entity,
                                               const Boolean  stcTpState)
{
  ResultCode_t             result = RESULT_CODE_OK;
  StkEntityGenericData_t   *stkGenericContext_p = ptrToStkGenericContext ();

  /* we are NOT currently attached to SIM AT */
  if (stkGenericContext_p->registeredForStk [entity] == FALSE)
  {
    vgSetStkRegFlag (stcTpState, entity);

    /* but can we attach now */
    if (stkGenericContext_p->registeredForStk [entity] == TRUE)
    {
      if (vgOpManAllocateConnection (entity, STK_CONNECTION) == TRUE)
      {
        /* set up the connection lookup table */
        stkGenericContext_p->registeredStkEntity = entity;

        result = RESULT_CODE_PROCEEDING;
        vgStkAfsaStRegisteredTaskReq (entity,
                                       TRUE);
      }
      else
      {
        stkGenericContext_p->registeredForStk [entity] = FALSE;
      }
    }
  }
  else /* we ARE currently attached to SIM AT */
  {
    if ( stcTpState == FALSE )  /*want to detach */
    /*  we can dettach now */
    {
      stkGenericContext_p->registeredForStk [entity] = FALSE;
      result = RESULT_CODE_PROCEEDING;
      stkGenericContext_p->registeredStkEntity = VGMUX_CHANNEL_INVALID;
      vgStkAfsaStRegisteredTaskReq (entity,
                                     FALSE);
    }
  }

  /* let the application know that we have not registered */
  if (stkGenericContext_p->registeredForStk [entity] == FALSE)
  {
    if ((result == RESULT_CODE_OK) || (result == RESULT_CODE_PROCEEDING))
    {

      vgPutNewLine (entity);
      vgPrintf     (entity,
                    (const Char*)"*MSTC: %u",
                     STK_NOT_PROACTIVE);
      vgPutNewLine  (entity);
      vgFlushBuffer (entity);
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkCreateSendRspSignal
 *
 * Parameters:  commandBuffer_p - pointer to cmmand line string
 *              alwaysSendTp    - send profile always
 *              entity          - mux channel number
 *
 * Returns:     ResultCode - command buffer decode and signal encode result
 * Description: Receives the command buffer and sends it to the appropriate
 *              encode routine.  The signal id is taken from the signal
 *              decode module. It is possible for an indication to have the same
 *              command reference id so we search on the signal id and not the
 *              command reference.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgStkCreateSendRspSignal (CommandLine_t *commandBuffer_p,
                                        const Boolean  alwaysSendTp,
                                         const VgmuxChannelNumber entity)
{
  const StkEncodeSignalEntityControl *signalTable = &stkEncodeSignalEntityTable[0];
  Boolean      foundType    = FALSE;
  ResultCode_t result       = RESULT_CODE_ERROR;


  /* search through STK signal indications for a type match */
  while ((signalTable->signalType != SIG_SYS_DUMMY) &&
         (foundType == FALSE))
  {
    if (signalTable->signalType == vgStkGetCurrentSignal ())
    {
      foundType  = TRUE;
      result = signalTable->stkEncodeProcFunc (commandBuffer_p,
                                               alwaysSendTp,
                                               entity);
    }
    else
    {
      signalTable++;
    }
  }

  /* release any memory we allocated to hold the command data */
  vgStkDestroyBuffer ();

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkGetArrayFromHex
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              data_p          - extracted hex data
 *
 * Returns:     Int16 - size of array
 *
 * Description: Given a buffer of hex values return the characters
 *
 *-------------------------------------------------------------------------*/

static Int16 vgStkGetArrayFromHex (CommandLine_t *commandBuffer_p,
                                    Char  *data_p)
{
  Int16    tpLength;
  Int16    size = 0;
  Int16    profiles;
  Int8     lsbHexValue;
  Int8     msbHexValue;

  /* calculate the length of the ascii string */
  tpLength = commandBuffer_p->length - commandBuffer_p->position;
  /* round up to nearest even number */
  tpLength += (tpLength & 1);
  /* therefore the number of bytes necessary is: */
  tpLength = tpLength >> 1;


  if ((tpLength > 0) &&
      (tpLength < SIMAT_MAX_NUM_CHARACTERS))
  {
    for (profiles = 0; profiles < tpLength; profiles++)
    {
      if (commandBuffer_p->position + 2 < commandBuffer_p->length)
      {
        /* parse byte at a time */
        if ((getHexaValue (commandBuffer_p->character[commandBuffer_p->position++], &msbHexValue)) &&
            (getHexaValue (commandBuffer_p->character[commandBuffer_p->position++], &lsbHexValue)))
        {
          *data_p++ = (16 * msbHexValue) + lsbHexValue;
          size ++;
        }
      }
      /* ignore any invalid data */
    }
    *data_p = (Char)0;
  }
  else
  {
    /* invalid command format */
    size = 0;
  }

  return (size);
}
/*--------------------------------------------------------------------------
 *
 * Function:    vgStkInitialiseStkUtil
 *
 * Parameters:  none
 *
 * Returns:     nothing
 *
 * Description: Calls the initialisation function.
 *
 *-------------------------------------------------------------------------*/

void vgStkInitialiseStkUtil (void)
{
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();
  Int8                    eIndex;

  /* initialise the context STK data */
  for (eIndex = 0; eIndex < CI_MAX_ENTITIES; eIndex++)
  {
    stkGenericContext_p->registeredForStk [eIndex] = FALSE;
  }

  /* initalise the generic STK sub-system data */
  stkGenericContext_p->simHasPassedPinCheck = FALSE;
  stkGenericContext_p->responseTimerEnabled = FALSE;  /* by default this is switched off */
  stkGenericContext_p->proactiveSessionStarted = FALSE;
  /* initialise the MMI Monitored event list to 'no events' */
  memset(&stkGenericContext_p->currentEventList, FALSE, sizeof(StkMmiEventList)); 
}
/*--------------------------------------------------------------------------
 *
 * Function:    getCmdIdAlphaType
 *
 * Parameters:  command id  *
 * Returns:     alpha id type                                         
 *
 * Description: Certain commands handle their repsonses based on a generic signal
 * with a specific alpha Id field this function links the alpha id type with the
 * command ID - only commands where this is valid have a valid response.
 *
 *-------------------------------------------------------------------------*/

static SimatAlphaIdType getCmdIdAlphaType (Int32  cmdId)
{
  SimatAlphaIdType  alphaIdType ;

  switch (cmdId)
  {
    case STK_CMD_SETUP_OPEN_CHANNEL: /* open channel */
    {
      alphaIdType = SIMAT_OP_ALPHA_ID;
      break;
    }
    case STK_CMD_SETUP_SEND_DATA: /* send data */
    {
      alphaIdType = SIMAT_SE_ALPHA_ID;
      break;
    }    
    case STK_CMD_SETUP_RECEIVE_DATA: /* receive data */
    {
      alphaIdType = SIMAT_RE_ALPHA_ID ;
      break;
    }
    case  STK_CMD_SETUP_SEND_DTMF: /* send dtmf */
    {
      alphaIdType = SIMAT_SD_ALPHA_ID;
      break;
    }
    default:
    {
      alphaIdType = SIMAT_ALPHA_ID_NUM; /* invalid id */
      break;
    }
  }

  return (alphaIdType);
}
/*--------------------------------------------------------------------------
 *
 * Function:    vgReinitialiseStk
 *
 * Parameters:  
 * Returns:    
 *   *
 * Description: If we do a soft reset we need to clear the MMI profile as 
 * this may not be correct - NVRAM values will be re-read after reset and
 * these may have been updated since the last download. 
 *
 *-------------------------------------------------------------------------*/

void vgReinitialiseStk (void)
{
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

   if ( stkGenericContext_p->currentMmiProfile_p != PNULL )
   {
      KiFreeMemory ((void **)& stkGenericContext_p->currentMmiProfile_p);
   }
}
/* END OF FILE */

