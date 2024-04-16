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
 * RVCMUX.C
 * Handles the CMUX AT command.
 **************************************************************************/

#define MODULE_NAME "RVCMUX"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <gkisig.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <rvnvram.h>
#include <rvcmux.h>
#include <rvcimxut.h>
#include <rvomtime.h>
#include <rvoman.h>
#include <rvcrhand.h>
#include <rvtssigo.h>
#include <cimux_sig.h>
#include <stdio.h>
#include <frhsl.h>

#include "atci_gki_trace.h"
/* Define constants ************************************************/
/* The T1 value used to time replies (mSec)             */

#define GSM_07_010_T1_INTERVAL          100

#define GSM_07_010_T1_MIN_INTERVAL      10

#define GSM_07_010_T1_MAX_INTERVAL      2540

/* The N1 value which specifies maximum frame size      */

#define GSM_07_010_N1_FRAME_SIZE        31

#define GSM_07_010_N1_MAX_FRAME_SIZE    4096

/* The N2 value which specifies number of retransmissions */

#define GSM_07_010_N2_RETRANSMISSIONS     3

#define GSM_07_010_N2_MIN_RETRANSMISSIONS 0

#define GSM_07_010_N2_MAX_RETRANSMISSIONS 100

/* The T2 value used to time responses on DLCI 0 (mSec) */

#define GSM_07_010_T2_INTERVAL          300

#define GSM_07_010_T2_MIN_INTERVAL      20

#define GSM_07_010_T2_MAX_INTERVAL      2550

/* The T3 value used to time responses on wake up (mSec)*/

#define GSM_07_010_T3_INTERVAL        10000

#define GSM_07_010_T3_MIN_INTERVAL    1000

#define GSM_07_010_T3_MAX_INTERVAL    255000

/* The number of re-transmissions for error recovery mode*/

#define GSM_07_010_K                      2

#define GSM_07_010_K_MIN                  1

#define GSM_07_010_K_MAX                  7

/********************************************************/
/* AT+CMUX parameter scale values                       */
/********************************************************/
#define AT_CMUX_T1_SCALE               10
#define AT_CMUX_T2_SCALE               10
#define AT_CMUX_T3_SCALE             1000

/* Signal union ****************************************************/
union Signal
{
  CiMuxConfigureMuxReq       ciMuxConfigureMuxReq;
  CiMuxCheckCmuxCmdParamsReq ciMuxCheckCmuxCmdParamsReq;
  CiMuxReadCmuxCmdParamsReq  ciMuxReadCmuxCmdParamsReq;
  CiMuxVersionReq            ciMuxVersionReq;

};


/* Data structure ********************************************/


/*--------------------------------------------------------------------------
*
* Function:        vgCimuxReadCmuxParamReq
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*
* Returns:         Nothing
*
* Description:     Function which sends signal for query CMUX parameters
*-------------------------------------------------------------------------*/
void vgCimuxReadCmuxParamReq (const VgmuxChannelNumber entity)
{
  SignalBuffer            signal = kiNullBuffer;

  KiCreateZeroSignal ( SIG_CIMUX_READ_CMUX_CMD_PARAMS_REQ,
                       sizeof (CiMuxReadCmuxCmdParamsReq),
                       &signal );
                       
  signal.sig->ciMuxReadCmuxCmdParamsReq.channelNumber = entity;
  
  KiSendSignal(MUX_TASK_ID,&signal);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgProcCMUX
 *
 * Parameters:  (InOut) commandBuffer - a pointer to a CommandLine_t
 *              structure.
 *
 * Returns:     ResultCode_t which is:
 *              - RESULT_CODE_OK if the command has been executed
 *                correctly,
 *              - RESULT_CODE_ERROR if the command has not been executed
 *                correctly.
 *
 * Description: This function executes the AT+CMUX command which switches the
 *              multiplexer into Standard Mode. SwitchMuxReq is sent to the MUX
 *              which performs the switch if it is valid.
 *-------------------------------------------------------------------------*/

ResultCode_t vgProcCMUX (CommandLine_t            *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
  CiMuxCheckCmuxCmdParamsReq*     request_p;
  SignalBuffer                    signal                 = kiNullBuffer;
  ResultCode_t                    result                 = RESULT_CODE_OK;
  ExtendedOperation_t             operation              = getExtendedOperation (commandBuffer_p);
  Int32                           mode, subset, portSpeed;
  Int32                           N1, T1, N2, T2, T3, k;

  /* Main switch */
  switch (operation)
  {
    case EXTENDED_RANGE:    /* +CMUX=? */
    {
      vgPutNewLine (entity);

      /* default range information (as per standard) */
      vgPrintf (entity,
                (const Char*)"+CMUX: (%d),(%d),(%d-%d),(%d-%d),(%d-%d),(%d-%d),(%d-%d),(%d-%d),(%d-%d)",
                 BASIC_OPTION,
                 UIH_FRAMES,
                 PORTSPEED_AUTO,
                 PORTSPEED_460800,
                 GSM_07_010_N1_FRAME_SIZE,
                 GSM_07_010_N1_MAX_FRAME_SIZE,
                (GSM_07_010_T1_MIN_INTERVAL / AT_CMUX_T1_SCALE),
                (GSM_07_010_T1_MAX_INTERVAL / AT_CMUX_T1_SCALE),
                 GSM_07_010_N2_MIN_RETRANSMISSIONS,
                 GSM_07_010_N2_MAX_RETRANSMISSIONS,
                (GSM_07_010_T2_MIN_INTERVAL / AT_CMUX_T2_SCALE),
                (GSM_07_010_T2_MAX_INTERVAL / AT_CMUX_T2_SCALE),
                (GSM_07_010_T3_MIN_INTERVAL / AT_CMUX_T3_SCALE),
                (GSM_07_010_T3_MAX_INTERVAL / AT_CMUX_T3_SCALE),
                 GSM_07_010_K_MIN,
                 GSM_07_010_K_MAX);

      vgPutNewLine (entity);

      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ASSIGN:   /* +CMUX=<mode>[,<subset>[,<port_speed>[,<N1>[,<T1>[,<N2>[,<T2>[,<T3>[,<k>]]]]]]]] */
    {
      if ((getExtendedParameter (commandBuffer_p, &mode,      BASIC_OPTION) == TRUE) &&
          (getExtendedParameter (commandBuffer_p, &subset,    UIH_FRAMES) == TRUE) &&
          (getExtendedParameter (commandBuffer_p, &portSpeed, PORTSPEED_AUTO) == TRUE) &&
          (getExtendedParameter (commandBuffer_p, &N1,        GSM_07_010_N1_FRAME_SIZE) == TRUE) &&
          (getExtendedParameter (commandBuffer_p, &T1,       (GSM_07_010_T1_INTERVAL / AT_CMUX_T1_SCALE)) == TRUE) &&
          (getExtendedParameter (commandBuffer_p, &N2,        GSM_07_010_N2_RETRANSMISSIONS) == TRUE) &&
          (getExtendedParameter (commandBuffer_p, &T2,       (GSM_07_010_T2_INTERVAL / AT_CMUX_T2_SCALE)) == TRUE) &&
          (getExtendedParameter (commandBuffer_p, &T3,       (GSM_07_010_T3_INTERVAL / AT_CMUX_T3_SCALE)) == TRUE) &&
          (getExtendedParameter (commandBuffer_p, &k,         GSM_07_010_K) == TRUE))
      {
        // M_FrGkiPrintf1 (0xFB00, ATCI, "pb improve: at+cmux=%d",mode);
        GKI_TRACE1 (PB_IMPROVE_AT_CMUX, GKI_ATCI_INFO, mode);
        {
          KiCreateZeroSignal (SIG_CIMUX_CHECK_CMUX_CMD_PARAMS_REQ,
                              sizeof(CiMuxCheckCmuxCmdParamsReq),
                              &signal);

          request_p = &signal.sig->ciMuxCheckCmuxCmdParamsReq;

          request_p->channelNumber           = entity;
          request_p->cmuxCmdParams.mode      = (MuxOperation) mode;
          request_p->cmuxCmdParams.subset    = (MuxSubset)    subset;
          request_p->cmuxCmdParams.portSpeed = (PortSpeed)    portSpeed;   /* CMUX and port speed values for generic MUX have
                                                                            * same values - so can just cast the value set.
                                                                            */
          request_p->cmuxCmdParams.n1        = N1;
          request_p->cmuxCmdParams.t1        = (Int8) T1;
          request_p->cmuxCmdParams.n2        = (Int8) N2;
          request_p->cmuxCmdParams.t2        = (Int8) T2;
          request_p->cmuxCmdParams.t3        = (Int8) T3;
          request_p->cmuxCmdParams.k         = (Int8) k;

          KiSendSignal(MUX_TASK_ID,&signal);

          result = RESULT_CODE_PROCEEDING;
        }
      }
      else
      {
        result = VG_CME_INVALID_INPUT_VALUE;
      }

      break;
    }
    case EXTENDED_QUERY:    /* +CMUX? */
    {
      vgCimuxReadCmuxParamReq(entity);

      result = RESULT_CODE_NULL;

      break;
    }
    case EXTENDED_ACTION:   /* AT+CMUX  */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
   }
  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgProcMMUX
 *
 * Parameters:  (InOut) commandBuffer - a pointer to a CommandLine_t
 *              structure.
 *
 * Returns:     ResultCode_t which is:
 *              - RESULT_CODE_OK if the command has been executed
 *                correctly,
 *              - RESULT_CODE_ERROR if the command has not been executed
 *                correctly.
 *
 * Description: This function executes the AT*MMUX command which changes MUX
 *              configuration. ConfigureMuxReq is sent to the MUX which applies
 *              the changes if the configuration is valid. 
 *-------------------------------------------------------------------------*/

ResultCode_t vgProcMMUX (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
  ResultCode_t          result = RESULT_CODE_OK;
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  Int32                 connDataSegmented, muxChannelDataSegmented, escMonitoring, fcsChecking; 
  SignalBuffer          signal = kiNullBuffer;
  CiMuxConfigureMuxReq* request_p;


  switch (operation)
  {
    case EXTENDED_ASSIGN:   /* AT*MMUX=<atpSegUl>,<stmSegUl>,<escMon>,<fcsCh> */
    {
      if ((getExtendedParameter (commandBuffer_p, &connDataSegmented,       0) == TRUE) &&
          (getExtendedParameter (commandBuffer_p, &muxChannelDataSegmented, 0) == TRUE) &&
          (getExtendedParameter (commandBuffer_p, &escMonitoring,           0) == TRUE) &&
          (getExtendedParameter (commandBuffer_p, &fcsChecking,             0) == TRUE))
      {
        KiCreateZeroSignal (SIG_CIMUX_CONFIGURE_MUX_REQ,
                              sizeof(CiMuxConfigureMuxReq),
                              &signal);

        request_p = &signal.sig->ciMuxConfigureMuxReq;
        
        request_p->channelNumber                            = entity; 
        request_p->muxConfigOptions.connDataSegmented       = (connDataSegmented       == 0) ? FALSE : TRUE;
        request_p->muxConfigOptions.muxChannelDataSegmented = (muxChannelDataSegmented == 0) ? FALSE : TRUE; 
        request_p->muxConfigOptions.escMonitoring           = (escMonitoring           == 0) ? FALSE : TRUE; 
        request_p->muxConfigOptions.fcsChecking             = (fcsChecking             == 0) ? FALSE : TRUE; 
        
        KiSendSignal(MUX_TASK_ID,&signal);

        result = RESULT_CODE_PROCEEDING; 
      }
      else
      {
        result = VG_CME_INVALID_INPUT_VALUE;
      }

      break;
    }
    /* 
     * AT*MMUX is only used to assign new values 
     */
    case EXTENDED_RANGE:     /* AT*MMUX=? */
    {
      vgPutNewLine (entity);

      /* default range information (as per standard) */
      vgPrintf (entity,
                (const Char*)"*MMUX: (0-1),(0-1),(0-1),(0-1)");       
      vgPutNewLine (entity);

      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_QUERY:     /* AT*MMUX? */
    case EXTENDED_ACTION:    /* AT*MMUX  */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
   }
  return (result);
}

/* END OF FILE */

