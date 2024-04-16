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
 * Handler for Supplementary Services AT Commands and related signals
 **************************************************************************/

#define MODULE_NAME "RVSSSS"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <cici_sig.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvssss.h>
#include <rvssut.h>
#include <vgmx_sig.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  CiRunAtCommandInd ciRunAtCommandInd;
};

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* AT command lookup table */
static const AtCmdControl ssAtCommandTable[] =
{
  { ATCI_CONST_CHAR_STR "+CLCK",  vgSsCLCK,   VG_AT_SS_CLCK,   AT_CMD_ACCESS_SIM_PRESENT},
  { ATCI_CONST_CHAR_STR "+CPWD",  vgSsCPWD,   VG_AT_SS_CPWD,   AT_CMD_ACCESS_SIM_PRESENT},
  { PNULL,            PNULL,      VG_AT_LAST_CODE, AT_CMD_ACCESS_NONE}
};

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static void initialiseSsssGenericData (void);

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

 /****************************************************************************
 *
 * Function:    initialiseSsssGenericData
 *
 * Parameters:  none
 *
 * Returns:     Nothing
 *
 * Description: initiates supplementary services sub-system entity generic
 *              data
 *
 ****************************************************************************/

static void initialiseSsssGenericData (void)
{
  SsCallRelatedContext_t        *ssCallRelatedContext_p         = ptrToSsCallRelatedContext ();
  Int8                          callIndex;
  VgSsCallerIdData              *vgSsCallerIdData_p;

  /* Initialise the SS call related context data.... */
  for ( callIndex = 0; callIndex < MAX_USER_CALL_ID; callIndex++)
  {
    vgSsCallerIdData_p = &ssCallRelatedContext_p->vgSsCallerIdData[callIndex];

    vgSsCallerIdData_p->alphaIdValid = FALSE;
    vgSsCallerIdData_p->dialNumValid = FALSE;
    vgSsCallerIdData_p->subAddressValid = FALSE;
  }
}

 /****************************************************************************
 *
 * Function:    initialiseSsssEntitySpecificData
 *
 * Parameters:  none
 *
 * Returns:     Nothing
 *
 * Description: initiates supplementary services sub-system entity specific
 *              data
 *
 ****************************************************************************/

void initialiseSsssEntitySpecificData (const VgmuxChannelNumber entity)
{
  SupplementaryContext_t  *supplementaryContext_p;

  /* Initialise each supplementary services entities context.... */
  supplementaryContext_p = ptrToSupplementaryContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(supplementaryContext_p != PNULL, entity, 0, 0);
#endif
  supplementaryContext_p->ssParams.ssOpFromATD            = FALSE;  /* added for job109084 */
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

 /****************************************************************************
 *
 * Function:    vgSsssInterfaceController
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

Boolean vgSsssInterfaceController(  const SignalBuffer *signal_p,
                                    const VgmuxChannelNumber entity)
{
  Boolean accepted = FALSE;

  /* Signal Handler */

  switch (*signal_p->type)
  {
    case SIG_CI_RUN_AT_COMMAND_IND:
    {
      accepted = parseCommandBuffer (ssAtCommandTable, entity);
      break;
    }
    case SIG_INITIALISE:
    {
      initialiseSsssGenericData();
      break;
    }
    case SIG_CIMUX_CHANNEL_ENABLE_IND:
    {
      initialiseSsssEntitySpecificData (entity);
      break;
    }
    default:
    {
      /* No SS functions for NB-IOT */
      break;
    }
  }
  return (accepted);
}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

