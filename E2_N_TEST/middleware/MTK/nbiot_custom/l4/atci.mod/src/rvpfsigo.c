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
 * Profile sub-system output signal handlers
 **************************************************************************/

#define MODULE_NAME "RVPFSIGO"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <gkisig.h>

#include <rvutil.h>
#include <rvdata.h>
#include <rvpfsigo.h>
#include <cici_sig.h>
#include <vgmxdte.h>
#include <rvcfg.h>
#include <rvomtime.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  ApexSmProfileChangedReq   apexSmProfileChangedReq;
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

/***************************************************************************
 * Global Functions
 ***************************************************************************/

 /*--------------------------------------------------------------------------
 *
 * Function:        vgApexSendSmProfileChangeReq
 *
 * Parameters:      entity
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_PROFILE_CHANGED_REQ to the
 *                  background layer to instruct it to update its absmContext
 *                  flags to determine whether to route MT SMs to the CI Task.
 *                  As and addition to this message service information
 *                  is included which determines on which level SMS will
 *                  be acknowledged - from CI task or from ABSM
 *-------------------------------------------------------------------------*/

void vgApexSendSmProfileChangedReq (const VgmuxChannelNumber entity)
{
    SignalBuffer              sigBuff = kiNullBuffer;

    ApexSmProfileChangedReq  *request_p;
    Int8                      index;

    KiCreateZeroSignal( SIG_APEX_SM_PROFILE_CHANGED_REQ,
                        sizeof (ApexSmProfileChangedReq),
                        &sigBuff);

    request_p = (ApexSmProfileChangedReq *) sigBuff.sig;
    request_p->taskId = VG_CI_TASK_ID;

    for (index = 0; index < NUM_SMS_CLASSES_SUPPORTED; index++)
    {
        request_p->mtCnmiVal[index] = FALSE;
    }

    /* <mt> CNMI setting */
    switch (getProfileValue (entity, PROF_CNMI + 1))
    {
        case 2:
            {
                request_p->mtCnmiVal[0] = TRUE;
                request_p->mtCnmiVal[1] = TRUE;
                request_p->mtCnmiVal[3] = TRUE;
                break;
            }
        case 3:
            {
                request_p->mtCnmiVal[3] = TRUE;
                break;
            }
        default:
            {
                /*
                 * Other values - no action required.
                 */
                break;
            }    
    }

    /* <ds> CNMI setting */
    request_p->dsCnmiVal = getProfileValue (entity, PROF_CNMI + 3);

    /* <service> CSMS setting */
    request_p->msgService = getProfileValue (entity, PROF_CSMS);

    request_p->storeInMe = FALSE;

    KiSendSignal (cfRvAbTaskId, &sigBuff);
}


 /*--------------------------------------------------------------------------
 *
 * Function:        vgApexSendSmDeliveryRsp
 *
 * Parameters:      shortMsgId, success
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_SM_DELIVERY_RSP to the
 *                  background layer to acknowledge reception of a new message
 *                  SHOULD BE SENT DIRECTLY TO SMTL task
 *-------------------------------------------------------------------------*/

void vgApexSendSmDeliveryRsp (const Int8 shortMsgId, const Boolean success)
{
  SignalBuffer            sigBuff = kiNullBuffer;
  ApexSmDeliveryRsp       *request_p;


  /* stop the timer */
  vgCiStopTimer(TIMER_TYPE_SMS_TR2M);

  /* acknowledge SMS*/
  KiCreateZeroSignal (SIG_APEX_SM_DELIVERY_RSP,
                       sizeof (ApexSmDeliveryRsp),
                        &sigBuff);

  request_p = (ApexSmDeliveryRsp *) sigBuff.sig;
  request_p->taskId = VG_CI_TASK_ID;

  request_p->requestStatus  = success? SM_REQ_OK: SM_REQ_SM_NOT_READY;
  request_p->shortMsgId     = shortMsgId;

  KiSendSignal (cfRvAbTaskId, &sigBuff);
}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

