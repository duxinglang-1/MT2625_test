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
 * File Description : Application Foreground task definitions
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

#if defined (UPGRADE_TCP_IP)
  KI_TASK_DEF      ( TCPIP_TASK_ID,     TCPIP_QUEUE_ID,     tcpipTask,      4096,  30,  TCPIP_TASK_PRIO)
#else
  /* Needed for unit testing */
  KI_RESERVED_TASK ( TCPIP_TASK_ID,     TCPIP_QUEUE_ID)
#endif

#if defined (UPGRADE_WAP_PPP)
  KI_TASK_DEF      ( WPPP_TASK_ID,       WPPP_QUEUE_ID,      wpppTask,      2048,  30,  WPPP_TASK_PRIO)
#else
  KI_RESERVED_TASK ( WPPP_TASK_ID,       WPPP_QUEUE_ID)
#endif


#if defined (USE_TTP_AL)
#if !defined (ENABLE_ULCH_BUILD)
  KI_TASK_DEF      (TASK_FL_ID,          FL_QUEUE_ID,        afshTask1,    12000,  30,  TASK_FL_PRIO)
#else
  KI_TASK_DEF      (TASK_FL_ID,          FL_QUEUE_ID,        afshTask1,     3000,  30,  TASK_FL_PRIO)
#endif

  KI_TASK_DEF      (SIMAT_TASK_ID,       SIMAT_QUEUE_ID,     simatTask,     1536,  30,  SIMAT_TASK_PRIO)

  KI_START_ALIAS   (AB_ALIASES)
  KI_ALIAS_DEF     (TASK_BL_ID,          ALSH_TASK_ID,      ALSH_QUEUE_ID)
  KI_ALIAS_DEF     (TASK_BL_ID,          AL_TASK_ID,        AL_QUEUE_ID)
  KI_ALIAS_DEF     (TASK_BL_ID,          GP_ABGP_TASK_ID,   GP_ABGP_QUEUE_ID)
  KI_STOP_ALIAS    (AB_ALIASES)


#else /* USE_TTP_AL */

  KI_TASK_DEF      (AL_TASK_ID,          AL_QUEUE_ID,       ApplicationLayerTask, 4096, 30, AL_TASK_PRIO)

  KI_RESERVED_TASK (SIMAT_TASK_ID,       SIMAT_QUEUE_ID)

KI_START_ALIAS (AB_ALIASES)
  KI_ALIAS_DEF     (AL_TASK_ID,           TASK_FL_ID,        FL_QUEUE_ID)
KI_STOP_ALIAS  (AB_ALIASES)

#endif /* USE_TTP_AL */

  KI_RESERVED_TASK (AP_BG_TASK_ID,        AP_BG_QUEUE_ID)

/* END OF FILE */
