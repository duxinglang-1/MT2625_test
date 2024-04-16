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
 **************************************************************************/
/** \file
 * L1AL Task Configuration Data and Code
 **************************************************************************/

#if !defined (L1ALCFG_H)
#    define   L1ALCFG_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <kitqid.h>     /* For TaskId type. */
#include <ki_sigbuf.h>  /* for SignalBuffer */

#if defined(UPGRADE_COARSE_TIMERS)
# include <l1timers.h>
#endif



/********************** DAC ***********************/

typedef enum DacReferenceTag
{
  AUX_DAC_0,
  NUM_DAC_REF
}
DacReference;

/********************** PWM ***************************/
typedef enum PwmReferenceTag
{
  PWM_0,
  PWM_1,
  NUM_PWM_REF
}
PwmReference;

/********************** AUDIO **************************/
typedef enum    L1PredefinedMelodyTag
{
    MOZART,
    RPCALLALERT,
    BOLERO,
    B10TH,
    CANCAN,
    FIRELISE,
    ENTERTAINER,
    BINKYBONKY,
    TOTAL_MELODIES
}
L1PredefinedMelody;

/****************************************************************************
 * Configurable Data
 ****************************************************************************/

#if defined(UPGRADE_COARSE_TIMERS)
/* Coarse Timer inaccuracy percentage for layer 1 timers */
extern const Int8 cfL1TimerInaccuracyPercentage [ NUM_OF_L1_TIMERS ];
#endif

/* Queues for layer 1 tasks that other layers send signals to. */
extern QueueId cfL1AlQueueId;

/* Tasks in other layers that L1 sends signals to. */
extern TaskId  cfL1AlTaskId;
extern TaskId  cfL1AlBgTaskId;
extern TaskId  cfFlTaskId;
extern TaskId  cfFlNewTaskId;        /* 0109-12740 */
extern TaskId  cfFlStatusIndTaskId;  /* 0109-12740 */
extern TaskId  cfClockTaskId;

/****************************************************************************
 * Configurable Functions
 ****************************************************************************/
extern Boolean CfL1AlHandleOtherSignals (SignalBuffer *signal);

#endif

/* END OF FILE */
