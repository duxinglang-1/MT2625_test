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
 * File Description : Layer 1 task definitions
 **************************************************************************/

/*
** This file deiberately does not have a multiple inclusion protection
** as it can be legitimately included twice.
**
** This must never be included directly by any file outside of the GKI.
*/
#if !defined(UPGRADE_DUAL_MODE_DEVELOPMENT)
# if defined (UPGRADE_3G)
#  if !defined (UPGRADE_GGE)
#   if !defined (EXCLUDE_GSM_L1)
#    define EXCLUDE_GSM_L1       /* May already be defined in the build file */
#   endif /* EXCLUDE_GSM_L1 */
#  endif /* UPGRADE_GGE */
# endif /* UPGRADE_3G */
#endif /* UPGRADE_DUAL_MODE_DEVELOPMENT */

/*
**           GKI Task ID,           GKI Queue ID,           Entry Point                 Stack, Queue, Priority
*/

#if defined (ENABLE_NVRAM_STORAGE_IN_FS)
  /* If we store NVRAM using FTL allow bigger stack (mostly for M-Systems DOC) */
  #define NVRAM_FTL_OVERHEAD 1024
#else
  #define NVRAM_FTL_OVERHEAD 0
#endif

#if !defined (EXCLUDE_LP_TASK)
  KI_TASK_DEF (LOW_PRIORITY_TASK_ID,       LOW_PRIORITY_QUEUE_ID,  LowestPriorityTask,   NVRAM_FTL_OVERHEAD   +
                                                                                         2000,                         30, LOW_PRIORITY_TASK_PRIO )
#else
  KI_RESERVED_TASK (LOW_PRIORITY_TASK_ID,  LOW_PRIORITY_QUEUE_ID)
#endif

#if !defined (EXCLUDE_GSM_L1)
  KI_TASK_DEF (L1_BG_TASK_ID,              L1_BG_QUEUE_ID,         L1BackGroundTask,     NVRAM_FTL_OVERHEAD + 2000,    30, L1_BG_TASK_PRIO)
#else /* EXCLUDE_GSM_L1 */
  KI_RESERVED_TASK (L1_BG_TASK_ID,         L1_BG_QUEUE_ID)
#endif /* EXCLUDE_GSM_L1 */

#if !defined (EXCLUDE_GSM_L1)
  KI_TASK_DEF (L1_PH_TASK_ID,              L1_PH_QUEUE_ID,         L1PhTask,              458,    30, L1_PH_TASK_PRIO)
#else /* EXCLUDE_GSM_L1 */
  KI_RESERVED_TASK (L1_PH_TASK_ID,         L1_PH_QUEUE_ID)
#endif /* EXCLUDE_GSM_L1 */

#if defined(UPGRADE_SYS_TOOLS)
# if defined(ENABLE_L1_PROF)
  KI_TASK_DEF (L1_PROF_TASK_ID,       L1_PROF_QUEUE_ID,       L1ProfileTask,         400,     5, L1_PROF_TASK_PRIO)
# else
  KI_RESERVED_TASK (L1_PROF_TASK_ID,       L1_PROF_QUEUE_ID)
# endif
#else
  KI_RESERVED_TASK (L1_PROF_TASK_ID,       L1_PROF_QUEUE_ID)
#endif

#if defined (UPGRADE_3G) && !defined (MTK_NBIOT_TARGET_BUILD)
# if !defined (EXCLUDE_3G_L1)
#if !defined(ENABLE_PS_ONLY_TARGET)
  KI_TASK_DEF (UP_PHY_TASK_ID,             UP_PHY_QUEUE_ID,        UpPhyTask,            0x4000,    30, UP_PHY_TASK_PRIO)
#else
  KI_TASK_DEF (UP_PHY_TASK_ID,             UP_PHY_QUEUE_ID,        UpPhyTaskStub,         500,    30, UP_PHY_TASK_PRIO)  
#endif
# else
  KI_RESERVED_TASK (UP_PHY_TASK_ID,        UP_PHY_QUEUE_ID)
# endif
#else /* !UPGRADE_3G */
  KI_RESERVED_TASK (UP_PHY_TASK_ID,        UP_PHY_QUEUE_ID)
#endif /* UPGRADE_3G */

#if defined (UPGRADE_XO_ATC)
  KI_TASK_DEF (U1_XO_ATC_TASK_ID,          U1_XO_ATC_QUEUE_ID,     U1XoAtcTask,          3000,    30, U1_XO_ATC_TASK_PRIO)
#else
  KI_RESERVED_TASK (U1_XO_ATC_TASK_ID,     U1_XO_ATC_QUEUE_ID)
#endif /* UPGRADE_XO_ATC */

#if defined (UPGRADE_SYS_TOOLS)
# if defined (UT_ML_USE_MINILOGGER)
  KI_TASK_DEF (MINI_LOGGER_TASK_ID     ,   MINI_LOGGER_QUEUE_ID,      MlMiniLoggerTask,             1200,    30, MINI_LOGGER_TASK_PRIO)
  KI_TASK_DEF (ML_SIGNAL_DUMPER_TASK_ID,   ML_SIGNAL_DUMPER_QUEUE_ID, MlMiniLoggerSignalDumperTask, 400 ,    30, ML_SIGNAL_DUMPER_TASK_PRIO)
# else
  KI_RESERVED_TASK (MINI_LOGGER_TASK_ID     , MINI_LOGGER_QUEUE_ID)
  KI_RESERVED_TASK (ML_SIGNAL_DUMPER_TASK_ID, ML_SIGNAL_DUMPER_QUEUE_ID)
# endif
#else
  KI_RESERVED_TASK (MINI_LOGGER_TASK_ID     , MINI_LOGGER_QUEUE_ID)
  KI_RESERVED_TASK (ML_SIGNAL_DUMPER_TASK_ID, ML_SIGNAL_DUMPER_QUEUE_ID)
#endif

#if defined (UPGRADE_MIPS_MANAGER)
  KI_TASK_DEF (MIPS_MANAGER_TASK_ID,       MIPS_MANAGER_QUEUE_ID,     MipsManagerTask,              400 ,    30, MIPS_MANAGER_TASK_PRIO)
#else
  KI_RESERVED_TASK (MIPS_MANAGER_TASK_ID,  MIPS_MANAGER_QUEUE_ID)
#endif

#if defined (UPGRADE_VCXO)
  KI_TASK_DEF (L1_VCXO_TASK_ID,            L1_VCXO_QUEUE_ID,             L1VcxoTask,             700 ,    30, L1_VCXO_TASK_PRIO)
#else
  KI_RESERVED_TASK (L1_VCXO_TASK_ID,       L1_VCXO_QUEUE_ID)
#endif

#if defined (WL1_UPHY_STUB)
  KI_TASK_DEF      (WL1_AL_TASK_ID,        WL1_AL_QUEUE_ID,              W1ApTask,               4096, 30, WL1_AL_TASK_PRIO)
#else
  KI_RESERVED_TASK (WL1_AL_TASK_ID,       WL1_AL_QUEUE_ID)
#endif

#if defined (ENABLE_3G_FDD_L1_SIMU)
  KI_TASK_DEF      (DSP_TASK_ID,        DSP_QUEUE_ID,              DSPTask,               0x4000, 30, DSP_TASK_PRIO)
#   if defined (USE_3G_FDD_L1_NET_SIMU)
    KI_TASK_DEF      (NST_TASK_ID,        NST_QUEUE_ID,              NetworkTask,           0x4000, 30, NST_TASK_PRIO)
#   else
    KI_RESERVED_TASK (NST_TASK_ID,       NST_QUEUE_ID)
#   endif
#else
  KI_RESERVED_TASK (DSP_TASK_ID,       DSP_QUEUE_ID)
#endif

KI_START_ALIAS (L1_ALIASES)
KI_ALIAS_DEF   (L1_BG_TASK_ID,     L1_TASK_ID,     L1_QUEUE_ID)
KI_STOP_ALIAS  (L1_ALIASES)


#if defined (UPGRADE_LTE) && !defined (MTK_NBIOT_TARGET_BUILD)
# if !defined (EXCLUDE_LTE_L1)
#if !defined(ENABLE_PS_ONLY_TARGET)
  KI_TASK_DEF (LTE_PHY_TASK_ID,             LTE_PHY_QUEUE_ID,        LtePhyTask,            1000,    30, LTE_PHY_TASK_PRIO)
#else
  KI_TASK_DEF (LTE_PHY_TASK_ID,             LTE_PHY_QUEUE_ID,        LtePhyTaskStub,         500,    30, LTE_PHY_TASK_PRIO)
#endif
# else
  KI_RESERVED_TASK (LTE_PHY_TASK_ID,        LTE_PHY_QUEUE_ID)
# endif
#else /* !UPGRADE_LTE */
  KI_RESERVED_TASK (LTE_PHY_TASK_ID,        LTE_PHY_QUEUE_ID)
#endif /* UPGRADE_LTE */

/* define NB signal adapter task */
#if defined (UPGRADE_NB)
#   if defined (UE_SIMULATOR)
  KI_TASK_DEF (N_HL_HWSIM_TASK_ID,          N_HL_HWSIM_QUEUE_ID,     NHlHwsimTask,          1000,    30, N_HL_HWSIM_TASK_PRIO)
#   else /* !UE_SIMULATOR */
  KI_RESERVED_TASK (N_HL_HWSIM_TASK_ID,     N_HL_HWSIM_QUEUE_ID)
#   endif /* !UE_SIMULATOR */ 

#if (!defined (EXCLUDE_N1MHTASK) || defined (UE_SIMULATOR))
  /* 1000 -> 2048 due NVDM usage. */
  KI_TASK_DEF (N1_MH_TASK_ID,               N1_MH_QUEUE_ID,          N1MhTask,             2048,    30, N1_MH_TASK_PRIO)
#else
  KI_RESERVED_TASK (N1_MH_TASK_ID,          N1_MH_QUEUE_ID)
#endif
#endif /* UPGRADE_NB */
/* END OF FILE */
