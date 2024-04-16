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
 * GKI header file that includes all the task definition header files and
 * defines the task sets.
 * This must never be included directly by any file outside of the kernel.
 **************************************************************************/

/** \def EXS_TASKS
 * \ingroup PrdGki
 * Enable silicon client task set.
 * This conditional enables the silicon client specific task set. This can
 * be used by a silicon client to add their own tasks specific to their
 * platform. */

/*
** This file deiberately does not have a
** #if !defined KISYSTSK_H
** as it can be legitimately included twice
*/

#include "nbiot_modem_common_config.h"

KI_TASK_SET_START(0)
/*******************************************************************************
** External Silicon Client task set, not for TTPCom use
*******************************************************************************/
#if defined (EXS_TASKS)
# include <exstask.h>
#endif
KI_TASK_SET_STOP(0)

KI_TASK_SET_START(1)
/*******************************************************************************
** GKI/System Set
*******************************************************************************/
#include <kitask.h>
KI_TASK_SET_STOP(1)

KI_TASK_SET_START(2)
/*******************************************************************************
** Test Set
*******************************************************************************/
#include <testtask.h>

KI_TASK_SET_STOP(2)

#if !defined(MTK_NBIOT_COSIM_BUILD)

#if defined(VALIDATION_STUB)
#if 0
  KI_TASK_DEF (LOW_PRIORITY_TASK_ID,       LOW_PRIORITY_QUEUE_ID,  LowestPriorityTask,   1024   +
                                                                                         680,                         30, LOW_PRIORITY_TASK_PRIO )
  KI_TASK_DEF      (AL_TASK_ID,          AL_QUEUE_ID,       ApplicationLayerTask, 4096, 30, AL_TASK_PRIO)

  KI_START_ALIAS (L1_ALIASES)
  KI_ALIAS_DEF   (LOW_PRIORITY_TASK_ID,     L1_AL_TASK_ID,      L1_AL_QUEUE_ID)
  KI_ALIAS_DEF   (LOW_PRIORITY_TASK_ID,     L1_BG_TASK_ID,      L1_BG_QUEUE_ID)
  KI_ALIAS_DEF   (LOW_PRIORITY_TASK_ID,     TASK_FL_ID,         FL_QUEUE_ID)
  KI_STOP_ALIAS  (L1_ALIASES)

#endif
  KI_TASK_DEF      (VALID_TASK_ID,       VALID_QUEUE_ID,       ValidationTask, 4096, 30, EMMI_HIGH_PRI_TASK_PRIO)

#else

KI_TASK_SET_START(3)
/*******************************************************************************
** Layer 1 Set
*******************************************************************************/
#include <l1task.h>
KI_TASK_SET_STOP(3)

KI_TASK_SET_START(4)
/*******************************************************************************
** Device Management Set
*******************************************************************************/
#include <dmtask.h>
KI_TASK_SET_STOP(4)

KI_TASK_SET_START(5)
/*******************************************************************************
** Protocol Stack Set
*******************************************************************************/
#include <pstask.h>
KI_TASK_SET_STOP(5)

KI_TASK_SET_START(6)
/*******************************************************************************
** Application Layer Set
*******************************************************************************/
#include <aftask.h>
KI_TASK_SET_STOP(6)

KI_TASK_SET_START(7)
/*******************************************************************************
** Bluetooth Set
*******************************************************************************/
KI_TASK_SET_STOP(7)

KI_TASK_SET_START(8)
/*******************************************************************************
** Multimedia Set
*******************************************************************************/
#include <mediatask.h>
KI_TASK_SET_STOP(8)

KI_TASK_SET_START(9)

#ifdef OSDRIVERS_TEST_ENABLE
/*  Task add for driver test */



/*******************************************************************************
** Driver test Set
*******************************************************************************/
#include <osdrvtesttask.h>

#endif  /* IRQ_TEST_TASK */

#ifdef FEA_TEMP_PSC_DEVELOPMENT
KI_TASK_DEF      ( HIGH_PRIORITY_TASK_ID,       HIGH_PRIORITY_QUEUE_ID,      HighestPriorityTask,     2000,  30,  HIGH_PRIORITY_TASK_PRIO        )
#endif
KI_TASK_SET_STOP(9)

#endif

#endif
/* END OF FILE */
