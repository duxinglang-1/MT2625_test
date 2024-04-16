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
 * This file defines the GKI and generic System tasks.
 **************************************************************************/

/*
** This file deiberately does not have a multiple inclusion protection
** as it can be legitimately included twice.
**
** This must never be included directly by any file outside of the GKI.
*/

/*
**           GKI Task ID,           GKI Queue ID,           Entry Point          Stack, Queue, Priority
*/

  KI_TASK_DEF (TIMER_TASK_ID,              TIMER_QUEUE_ID,         KiTimerTask,      1024,    50, TIMER_TASK_PRIO)

#if defined (ROM_TEST_TASK)
  KI_TASK_DEF (TEST_TASK_ID,               TEST_QUEUE_ID,          KiTestTask,       1400,    30, TEST_TASK_PRIO)
#else
  KI_RESERVED_TASK (TEST_TASK_ID,          TEST_QUEUE_ID)
#endif
#if !defined (NO_EMMI_INTERFACE) && defined (ROM_TEST_TASK)

  #if !defined(TTC_PLATFORM_T1M)
    KI_TASK_DEF (EMMI_LOW_PRI_TASK_ID,       EMMI_LOW_PRI_QUEUE_ID,  EmmiLowPriTask,   800,    60, EMMI_LOW_PRI_TASK_PRIO)
  #else
    KI_TASK_DEF (EMMI_LOW_PRI_TASK_ID,       EMMI_LOW_PRI_QUEUE_ID,  EmmiLowPriTask,   800,    60, EMMI_LOW_PRI_TASK_PRIO)
  #endif
  #if defined (KI_ENABLE_LOGGING_SIGNALS_OVER_HSL)
   #if !defined (KI_ENABLE_LOGGING_SIGNALS_OVER_HSL_DMA)
    #if !defined(TTC_PLATFORM_T1M)
     KI_TASK_DEF (EMMI_HSL_TASK_ID,               EMMI_HSL_QUEUE_ID,         EmmiHslTask,   600,    60, EMMI_HSL_TASK_PRIO )
    #else
     KI_TASK_DEF (EMMI_HSL_TASK_ID,               EMMI_HSL_QUEUE_ID,         EmmiHslTask,   1000,    60, EMMI_HSL_TASK_PRIO )
    #endif
   #endif
  #endif
  KI_TASK_DEF (EMMI_HIGH_PRI_TASK_ID,      EMMI_HIGH_PRI_QUEUE_ID, EmmiHighPriTask,  1400,    30, EMMI_HIGH_PRI_TASK_PRIO)
#else
  KI_RESERVED_TASK (EMMI_LOW_PRI_TASK_ID,  EMMI_LOW_PRI_QUEUE_ID)
  #if defined (KI_ENABLE_LOGGING_SIGNALS_OVER_HSL)
    #if !defined (KI_ENABLE_LOGGING_SIGNALS_OVER_HSL_DMA)
     KI_RESERVED_TASK (EMMI_HSL_TASK_ID,  EMMI_HSL_QUEUE_ID)
    #endif
  #endif
  KI_RESERVED_TASK (EMMI_HIGH_PRI_TASK_ID, EMMI_HIGH_PRI_QUEUE_ID)
#endif

#if defined (OPTIMIZED_GKI_LOG_ENABLE)
  KI_TASK_DEF(KI_LOG_TASK_ID, KI_LOG_QUEUE_ID, KiLogTask, 1500, 30, GKI_LOG_TASK_PRIO)
#else
  KI_RESERVED_TASK (KI_LOG_TASK_ID,  KI_LOG_QUEUE_ID)
#endif

/* END OF FILE */
