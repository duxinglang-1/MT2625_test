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
 * File Description : Multimedia task definitions
 **************************************************************************/

/*
** This file deliberately does not have multiple inclusion protection
** as it can be legitimately included twice.
**
** This must never be included directly by any file outside of the GKI.
*/

/*
**                  GKI Task ID,                      GKI Queue ID,                     Entry Point                  Stack,  Queue,  Priority,  Stack Pool
*/

#if defined (UPGRADE_MMAC)
# if defined (UPGRADE_ATHENA_PLUS)
#  if defined (UPGRADE_MP3)
#   error "MP3 is not supported on the Athena Plus platform"
#  endif
   /* Use external memory for MMAC stack on AthenaPlus. MP3 is not supported due to
    * insufficient MCU mips. Smaller stack needed as MP3 decoder is not used. There
    * is no need for this to be in AthenaPlus internal memory as midiFM parser is
    * not very mips-intensive. */
#  define USE_SMALL_EXTERNAL_MMAC_STACK_FOR_ATHENA_PLUS
# else /* i.e. Not AthenaPlus silicon. */
   /* Fast internal memory required for large MMAC stack to support MP3 decoder.
    * Therefore do not define USE_SMALL_EXTERNAL_MMAC_STACK_FOR_ATHENA_PLUS. */
# endif /* UPGRADE_ATHENA_PLUS */
#endif /* UPGRADE_MMAC */


#if defined (UPGRADE_MMAC)
# if defined (USE_SMALL_EXTERNAL_MMAC_STACK_FOR_ATHENA_PLUS)
 KI_TASKEX_DEF (MMAC_TASK_ID,                     MMAC_QUEUE_ID,                    MmacTask,                     3000,     30, MMAC_TASK_PRIO,            0)
 KI_TASKEX_DEF (MMAC_HIGH_PRIORITY_TASK_ID,       MMAC_HIGH_PRIORITY_QUEUE_ID,      MmacHighPriorityTask,         1024,      4, MMAC_HIGH_PRIORITY_TASK_PRIO,            0)
# else /* USE_SMALL_EXTERNAL_MMAC_STACK_FOR_ATHENA_PLUS */
 KI_TASKEX_DEF (MMAC_TASK_ID,                     MMAC_QUEUE_ID,                    MmacTask,                    14000,     30, MMAC_TASK_PRIO,            1)
 KI_TASKEX_DEF (MMAC_HIGH_PRIORITY_TASK_ID,       MMAC_HIGH_PRIORITY_QUEUE_ID,      MmacHighPriorityTask,         1024,      4, MMAC_HIGH_PRIORITY_TASK_PRIO,            0)
#endif /* USE_SMALL_EXTERNAL_MMAC_STACK_FOR_ATHENA_PLUS */
#else
 KI_RESERVED_TASK  (MMAC_TASK_ID,                     MMAC_QUEUE_ID)
 KI_RESERVED_TASK  (MMAC_HIGH_PRIORITY_TASK_ID,       MMAC_HIGH_PRIORITY_QUEUE_ID)
#endif

#if defined (UPGRADE_MULTIMEDIA_FRAMEWORK)
 KI_TASKEX_DEF (MF_PROCS0_TASK_ID,                MF_PROCS0_QUEUE_ID,               MfProcs0Task,                 2048,     30, MF_PROCS0_TASK_PRIO,            0)
 KI_TASKEX_DEF (MF_PROCS1_TASK_ID,                MF_PROCS1_QUEUE_ID,               MfProcs1Task,                 2048,     30, MF_PROCS1_TASK_PRIO,            0)
 KI_TASKEX_DEF (MF_RM_TASK_ID,                    MF_RM_QUEUE_ID,                   MfRmTask,                     2048,     30, MF_RM_TASK_PRIO,            0)
 KI_TASKEX_DEF (MF_COMMS_TASK_ID,                 MF_COMMS_QUEUE_ID,                MfCommsTask,                   600,     30, MF_COMMS_TASK_PRIO,            0)
#else
 KI_RESERVED_TASK  (MF_PROCS0_TASK_ID,                MF_PROCS0_QUEUE_ID)
 KI_RESERVED_TASK  (MF_PROCS1_TASK_ID,                MF_PROCS1_QUEUE_ID)
 KI_RESERVED_TASK  (MF_RM_TASK_ID,                    MF_RM_QUEUE_ID)
 KI_RESERVED_TASK  (MF_COMMS_TASK_ID,                 MF_COMMS_QUEUE_ID)
#endif

#if defined (UPGRADE_MDS)
 KI_TASKEX_DEF (MDS_CONTROL_TASK_ID,              MDS_CONTROL_QUEUE_ID,             MdsControlTask,               8192,     30, MDS_CONTROL_TASK_PRIO,            1)
 KI_TASKEX_DEF (VIDEO_RENDER_TASK_ID,             VIDEO_RENDER_QUEUE_ID,            VideoRenderTask,               600,     30, VIDEO_RENDER_TASK_PRIO,            1)
#else
 KI_RESERVED_TASK  (MDS_CONTROL_TASK_ID,              MDS_CONTROL_QUEUE_ID)
 KI_RESERVED_TASK  (VIDEO_RENDER_TASK_ID,             VIDEO_RENDER_QUEUE_ID)
#endif

#if defined (UPGRADE_MES)
 KI_TASKEX_DEF (MES_CONTROL_TASK_ID,              MES_CONTROL_QUEUE_ID,             MesControlTask,               8192,     30, MES_CONTROL_TASK_PRIO,            0)
 KI_TASKEX_DEF (MES_CAPTURE_TASK_ID,              MES_CAPTURE_QUEUE_ID,             MesCaptureTask,               8192,     30, MES_CAPTURE_TASK_PRIO,            0)
#else
 KI_RESERVED_TASK  (MES_CONTROL_TASK_ID,              MES_CONTROL_QUEUE_ID)
 KI_RESERVED_TASK  (MES_CAPTURE_TASK_ID,              MES_CAPTURE_QUEUE_ID)
#endif

  KI_RESERVED_TASK (CME_SM_SETUP_TASK_ID,             CME_SM_SETUP_QUEUE_ID)
  KI_RESERVED_TASK (CME_AUDIO_LOW_PRI_TASK_ID,        CME_AUDIO_LOW_PRI_QUEUE_ID)

#if defined (UPGRADE_AUDIO_MANAGER_V2)
 KI_TASKEX_DEF (AM_TASK_ID,                       AM_QUEUE_ID,                      AmTask,                       4096,     30, AM_TASK_PRIO,            0)
#else
 KI_RESERVED_TASK  (AM_TASK_ID,                       AM_QUEUE_ID)
#endif

#if defined (UPGRADE_MMSC) || defined (UPGRADE_MDS_STREAMING)
 /* Picked some arbitrary numbers here, but should not be higher priority than MEDS. */
 KI_TASKEX_DEF (MMSC_CONTROLLER_TASK_ID,         MMSC_CONTROLLER_TASK_QUEUE_ID,   MmscControlTask,                2048,     30, MMSC_CONTROLLER_TASK_PRIO,            0)
#else
 KI_RESERVED_TASK  (MMSC_CONTROLLER_TASK_ID,         MMSC_CONTROLLER_TASK_QUEUE_ID)
#endif

#if defined (UPGRADE_AUDIO_STREAM_MANAGER)
 KI_TASKEX_DEF (AU_STREAM_MANAGER_TASK_ID,       AU_STREAM_MANAGER_QUEUE_ID,      AuStreamManagerTask,            4096,     30, AU_STREAM_MANAGER_TASK_PRIO,            0)
#else
 KI_RESERVED_TASK  (AU_STREAM_MANAGER_TASK_ID,       AU_STREAM_MANAGER_QUEUE_ID)
#endif

