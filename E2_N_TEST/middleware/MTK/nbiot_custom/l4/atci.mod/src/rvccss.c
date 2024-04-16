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
 * vgccss.c
 * Handler for Call Control AT Commands
 **************************************************************************/

#define MODULE_NAME "RVCCSS"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif
#if !defined (CICI_SIG_H)
#  include <cici_sig.h>
#endif
#if !defined (RVUTIL_H)
#  include <rvutil.h>
#endif
#if !defined (RVDATA_H)
#  include <rvdata.h>
#endif
#if !defined (RVCCSS_H)
#  include <rvccss.h>
#endif
#if !defined (RVCCUT_H)
#  include <rvccut.h>
#endif
#if !defined (RVOMUT_H)
#  include <rvomut.h>
#endif
#if !defined (RVCCSIGI_H)
#  include <rvccsigi.h>
#endif
#if !defined (VGMX_SIG_H)
#  include <vgmx_sig.h>
#endif
#if !defined (RVSSPARS_H)
#  include <rvsspars.h>
#endif

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  CiRunAtCommandInd     ciRunAtCommandInd;
};

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* Call control command information table */

static const AtCmdControl cvAtCommandTable[] =
{

#if defined (FEA_MT_PDN_ACT)
  { ATCI_CONST_CHAR_STR "A",            vgCcA,           VG_AT_CC_A,            AT_CMD_ACCESS_CFUN_1},
#endif /* FEA_MT_PDN_ACT */

  { ATCI_CONST_CHAR_STR "DL",           vgCcDL,          VG_AT_CC_DL,           AT_CMD_ACCESS_NONE},
  { ATCI_CONST_CHAR_STR "H",            vgCcH,           VG_AT_CC_H,            AT_CMD_ACCESS_CFUN_1},
  { ATCI_CONST_CHAR_STR "+CHUP",        vgCcCHUP,        VG_AT_CC_H,            AT_CMD_ACCESS_CFUN_1},
  { ATCI_CONST_CHAR_STR "O",            vgCcO,           VG_AT_CC_O,            AT_CMD_ACCESS_NONE},
  { ATCI_CONST_CHAR_STR PNULL,   PNULL,            VG_AT_LAST_CODE,   AT_CMD_ACCESS_NONE}
};

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/


/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

 /****************************************************************************
 *
 * Function:    initialiseCcSsEntitySpecific
 *
 * Parameters:  none
 *
 * Returns:     Nothing
 *
 * Description: initiates Call Control sub-system entity SPECIFIC data
 *
 ****************************************************************************/

void initialiseCcSsEntitySpecific (const VgmuxChannelNumber entity)
{
  CallContext_t  *callContext_p;

  callContext_p = ptrToCallContext (entity);

  FatalCheck (callContext_p != PNULL, entity, 0, 0);

  callContext_p->vgApexCallReleaseError = APEX_CAUSE_OK;
  callContext_p->vgErrorType            = CI_CALL_RELEASE_ERROR_APEX;
}


/***************************************************************************
 * Global Functions
 ***************************************************************************/

 /****************************************************************************
 *
 * Function:    vgCcssInterfaceController
 *
 * Parameters:  SignalBuffer       - structure containing incoming signal
 *              VgmuxChannelNumber - mux channel number
 *
 * Returns:     Boolean - indicates whether the sub-system has recognised and
 *                        procssed the signal given.
 *
 * Description: determines action for received signals
 *
 ****************************************************************************/

Boolean vgCcssInterfaceController (const SignalBuffer *signal_p,
                                    const VgmuxChannelNumber entity)
{
  Boolean  accepted = FALSE;

  /* Signal Handler */

  switch (*signal_p->type)
  {
    case SIG_CI_RUN_AT_COMMAND_IND:
    {
      accepted = parseCommandBuffer (cvAtCommandTable, entity);
      break;
    }
    case SIG_INITIALISE:
    {
      break;
    }
    case SIG_CIMUX_CHANNEL_ENABLE_IND:
    {
      initialiseCcSsEntitySpecific (entity);
      break;
    }
    default:
    { 
      /* Do nothing  */
      break;
    }
  }

  return (accepted);

}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

