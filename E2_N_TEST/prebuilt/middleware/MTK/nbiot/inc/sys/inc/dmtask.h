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
 * File Description : Device Management task definitions
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

#if !defined (EXCLUDE_DM)
#  if !defined (SD_TASK_STACK_INCREASE)
#define SD_TASK_STACK_INCREASE 0
#  endif

#  if !defined (ENABLE_SIM_DRV_V2)
# if !defined(TTC_PLATFORM_T1M) || defined(SIM_EMULATION_ON)
  KI_TASK_DEF (L1_SD_TASK_ID,         L1_SD_QUEUE_ID,         L1SimDriverTask,   (512 + SD_TASK_STACK_INCREASE),    30, L1_SD_TASK_PRIO)
#endif
#  else
  KI_TASK_DEF (FR_SD_TASK_ID,         FR_SD_QUEUE_ID,         frSdDriverTask,   (512 + SD_TASK_STACK_INCREASE),    30, FR_SD_TASK_PRIO)
#  endif/*ENABLE_SIM_DRV_V2*/

#if defined (USE_L1AL)
# if defined (UPGRADE_SOFTWARE_MIDI) || defined (UPGRADE_SOFTWARE_MIDI_STUB)
  /* SwMidi SMAF playback L1AL stack usage measured at 2720 development build */
  KI_TASK_DEF (L1_AL_TASK_ID,         L1_AL_QUEUE_ID,         L1AlTask,   4072,    30, L1_AL_TASK_PRIO)
# else
#  if !defined (ENABLE_ULCH_BUILD)
  KI_TASK_DEF (L1_AL_TASK_ID,         L1_AL_QUEUE_ID,         L1AlTask,   2536,    30, L1_AL_TASK_PRIO)
#  else
  /* Reduced Stack size due to New DM being used */
  KI_TASK_DEF (L1_AL_TASK_ID,         L1_AL_QUEUE_ID,         L1AlTask,    536,    30, L1_AL_TASK_PRIO)
#  endif
# endif
#else  /* USE_L1AL */
  KI_RESERVED_TASK(WAS_L1_AL_TASK_ID,L1_AL_QUEUE_ID)
#endif /* USE_L1AL */

/* Redundant task - may be reused */
  KI_RESERVED_TASK (L1_BM_TASK_ID, L1_BM_QUEUE_ID)

/* Redundant task - may be reused */
  KI_RESERVED_TASK (L1_SP_TASK_ID, L1_SP_QUEUE_ID)

# if defined (UPGRADE_CTM)
  KI_TASK_DEF (L1_CTM_TASK_ID,        L1_CTM_QUEUE_ID,        L1CtmTask,         512,    30, L1_CTM_TASK_PRIO)
# else
  KI_RESERVED_TASK (L1_CTM_TASK_ID, L1_CTM_QUEUE_ID)
# endif

  KI_TASK_DEF (DM_TASK_ID,            DM_QUEUE_ID,            DmTask,            2048,    30, DM_TASK_PRIO)

#if defined (DTF_ENABLE_FRAMEWORK)

  KI_TASK_DEF (DTF_STIMULATE_TASK_ID,         DTF_STIMULATE_QUEUE_ID,  DtfStimulateTask,  512,  30, DTF_STIMULATE_TASK_PRIO)
  KI_TASK_DEF (DTF_SIMULATE_TASK_ID,          DTF_SIMULATE_QUEUE_ID,   DtfSimulateTask,   512,  30, DTF_SIMULATE_TASK_PRIO)

# if defined (DTF_ENABLE_SELF_TEST_TASK)
    KI_TASK_DEF (DTF_SELF_TEST_TASK_ID,       DTF_SELF_TEST_QUEUE_ID,  DtfSelfTestTask,   512,  30, DTF_SELF_TEST_TASK_PRIO)
# else /* Not defined (DTF_ENABLE_SELF_TEST_TASK) */
    KI_RESERVED_TASK (DTF_SELF_TEST_TASK_ID,  DTF_SELF_TEST_QUEUE_ID)
# endif /* defined (DTF_ENABLE_SELF_TEST_TASK) */

#else /* Not defined (DTF_ENABLE_FRAMEWORK) */

  KI_RESERVED_TASK (DTF_STIMULATE_TASK_ID,    DTF_STIMULATE_QUEUE_ID)
  KI_RESERVED_TASK (DTF_SIMULATE_TASK_ID,     DTF_SIMULATE_QUEUE_ID)

  KI_RESERVED_TASK (DTF_SELF_TEST_TASK_ID,    DTF_SELF_TEST_QUEUE_ID)

#endif /* defined (DTF_ENABLE_FRAMEWORK) */

#if defined(UPGRADE_LOGGING_OVER_SHM)

  KI_TASK_DEF (SHM_LOG_TASK_ID,         SHM_LOG_QUEUE_ID,         ShmLogTask,   512,    30, SHM_LOG_TASK_PRIO)
#endif

#if defined(UPGRADE_SHMCL_SOLUTION)
  KI_TASK_DEF ( MTS_PARA_TASK_ID,         MTS_PARA_QUEUE_ID,         MtsParaTask,   512,    30, MTS_PARA_TASK_PRIO)
#endif  /* UPGRADE_SHMCL_SOLUTION */

#   if defined (USE_TTP_AL)

#if !defined (DM_NVM_TASK_STACK_INCREASE)
#define DM_NVM_TASK_STACK_INCREASE    (332)           /*add stack increment size for nvram task from 0 to 332*/
#endif

  KI_TASK_DEF (DM_NVM_TASK_ID,        DM_NVM_QUEUE_ID,        DmNvMTask,         (920 + DM_NVM_TASK_STACK_INCREASE),    30, DM_NVM_TASK_PRIO)
#   else
  KI_RESERVED_TASK (DM_NVM_TASK_ID,   DM_NVM_QUEUE_ID)
#   endif

  KI_RESERVED_TASK (DM_BM_TASK_ID,    DM_BM_QUEUE_ID)

  KI_RESERVED_TASK (DM_RTC_TASK_ID,   DM_RTC_QUEUE_ID) /* TODO remove when possible */
#if !defined (DM_EXCLUDE_NVRAM_DEVICE_MANAGER)

#if !defined (DM_NVP_TASK_STACK_INCREASE)
#define DM_NVP_TASK_STACK_INCREASE 0
#endif

#if !defined (DM_NVP_TASK_QUEUE_INCREASE)
#define DM_NVP_TASK_QUEUE_INCREASE 0
#endif

  /* Use NVRAM task unless explicitly removed */
#if !defined (DM_NVP_TASK_PRIORITY_VALUE)
  KI_TASK_DEF (DM_NVP_TASK_ID,        DM_NVP_QUEUE_ID,        DmNvPTask,         (800 + DM_NVP_TASK_STACK_INCREASE),   (30 + DM_NVP_TASK_QUEUE_INCREASE), DM_NVP_TASK_PRIO)
#else
  KI_TASK_DEF (DM_NVP_TASK_ID,        DM_NVP_QUEUE_ID,        DmNvPTask,         (800 + DM_NVP_TASK_STACK_INCREASE),   (30 + DM_NVP_TASK_QUEUE_INCREASE), DM_NVP_TASK_PRIORITY_VALUE)
#endif
#else
  KI_RESERVED_TASK (DM_NVP_TASK_ID,   DM_NVP_QUEUE_ID)
#endif

#else /* !EXCLUDE_DM */
#if !defined (ENABLE_SIM_DRV_V2)
  KI_RESERVED_TASK (L1_SD_TASK_ID,    L1_SD_QUEUE_ID)
#else
#if defined (MTK_NBIOT_TARGET_BUILD)  || defined (UT_TARGET_CONFIG)
#  if !defined (SD_TASK_STACK_INCREASE)
#define SD_TASK_STACK_INCREASE 0
#  endif
  KI_TASK_DEF (FR_SD_TASK_ID,         FR_SD_QUEUE_ID,         frSdDriverTask,   (4096 + SD_TASK_STACK_INCREASE),    30, FR_SD_TASK_PRIO)
#else
  KI_RESERVED_TASK (FR_SD_TASK_ID,    FR_SD_QUEUE_ID)
#endif
#endif
  KI_RESERVED_TASK (L1_AL_TASK_ID,    L1_AL_QUEUE_ID)

  KI_RESERVED_TASK (L1_BM_TASK_ID,    L1_BM_QUEUE_ID)
  KI_RESERVED_TASK (L1_SP_TASK_ID,    L1_SP_QUEUE_ID)
  KI_RESERVED_TASK (L1_CTM_TASK_ID,   L1_CTM_QUEUE_ID)
  KI_RESERVED_TASK (DM_TASK_ID,       DM_QUEUE_ID)
#if defined (MTK_NBIOT_TARGET_BUILD)  || defined (UT_TARGET_CONFIG)
#if !defined (DM_NVM_TASK_STACK_INCREASE)
#define DM_NVM_TASK_STACK_INCREASE    (332)           /*add stack increment size for nvram task from 0 to 332*/
#endif
  KI_TASK_DEF (DM_NVM_TASK_ID,        DM_NVM_QUEUE_ID,        DmNvMTask,         (920 + DM_NVM_TASK_STACK_INCREASE),    30, DM_NVM_TASK_PRIO)
#else
  KI_RESERVED_TASK (DM_NVM_TASK_ID,   DM_NVM_QUEUE_ID)
#endif
  KI_RESERVED_TASK (DM_NVP_TASK_ID,   DM_NVP_QUEUE_ID)
  KI_RESERVED_TASK (DM_BM_TASK_ID,    DM_BM_QUEUE_ID)
  KI_RESERVED_TASK (DM_RTC_TASK_ID,   DM_RTC_QUEUE_ID) /* TODO remove when possible */
#endif /* EXCLUDE_DM */
#if defined (CONN_LAYER)
  KI_TASK_DEF      ( CONN_LAYER_TASK_ID,       CONN_LAYER_QUEUE_ID,            connLayerTask,        4096,   30, CONN_LAYER_TASK_PRIO        )
#endif /* CONN_LAYER */
#if defined (UPGRADE_USB)
# if defined (UPGRADE_NASMDL2)
  KI_TASK_DEF      ( USB_CTRL_TASK_ID,      USB_CTRL_QUEUE_ID,      usbCtrlTask,    4096,   30, USB_CTRL_TASK_PRIO      )
  KI_TASK_DEF      ( USB_PT_UL_TASK_ID,     USB_PT_UL_QUEUE_ID,     usbPtUlTask,    1024,   30, USB_PT_UL_TASK_PRIO     )
  KI_TASK_DEF      ( USB_PT_DL_TASK_ID,     USB_PT_DL_QUEUE_ID,     usbPtDlTask,    1024,   30, USB_PT_DL_TASK_PRIO     )
  KI_TASK_DEF      ( USB_PT_CTRL_TASK_ID,   USB_PT_CTRL_QUEUE_ID,   usbPtCtrlTask,  1024,   30, USB_PT_CTRL_TASK_PRIO   )
# else /* UPGRADE_NASMDL2 */
#  if !defined (USB_PARASITIC_TASK)
  KI_TASK_DEF (USB_TASK_ID,             USB_QUEUE_ID,           usbTask,        4096,     30, USB_TASK_PRIO)
#  else
  KI_RESERVED_TASK( USB_TASK_ID,        USB_QUEUE_ID )
#  endif    /* (USB_PARASITIC_TASK) */
#  if !defined (T1_USB)
  KI_TASK_DEF      ( USB_TARGET_TASK_ID,    USB_TARGET_QUEUE_ID,    usbTargetTask,  4096,     30, USB_TARGET_TASK_PRIO)
#  else
  KI_RESERVED_TASK ( USB_TARGET_TASK_ID,    USB_TARGET_QUEUE_ID     )
#  endif
  KI_RESERVED_TASK ( USB_PT_DL_TASK_ID,     USB_PT_DL_QUEUE_ID      )
  KI_RESERVED_TASK ( USB_PT_CTRL_TASK_ID,   USB_PT_CTRL_QUEUE_ID    )
# endif /* else UPGRADE_NASMDL2 */
#else  /* UPGRADE_USB */
  KI_RESERVED_TASK ( USB_TASK_ID,           USB_QUEUE_ID            )
  KI_RESERVED_TASK ( USB_TARGET_TASK_ID,    USB_TARGET_QUEUE_ID     )
  KI_RESERVED_TASK ( USB_PT_DL_TASK_ID,     USB_PT_DL_QUEUE_ID      )
  KI_RESERVED_TASK ( USB_PT_CTRL_TASK_ID,   USB_PT_CTRL_QUEUE_ID    )
#endif /* else UPGRADE_USB */

/* Redundant tasks - may be reused */
  KI_RESERVED_TASK ( IRDA_TASK_ID,      IRDA_QUEUE_ID       )
  KI_RESERVED_TASK ( OBEX_TASK_ID,      OBEX_QUEUE_ID       )
  KI_RESERVED_TASK ( OBEX_TEST_TASK_ID, OBEX_TEST_QUEUE_ID  )

#if !defined (EXCLUDE_DM) && defined (USE_L1AL)
  /* A task priority of 9 is chosen as priority range 0-9 are reserved for interrupt launched
   * tasks such as L1_AL_HI_TASK_ID. This task is triggered by the L1 frame interrupt and
   * provides a periodic tick to the L1AL layer. */
  /* L1_AL_HI_TASK stack max usage measured at 660 on 430gpdev build 2005_June_08. Allow some
   * extra to support use of printf & other debugging => size=800 */
  KI_TASK_DEF (L1_AL_HI_TASK_ID,      L1_AL_HI_QUEUE_ID,      L1AlHiTask,     800,    1, L1_AL_HI_TASK_PRIO)
#else
  KI_RESERVED_TASK (L1_AL_HI_TASK_ID, L1_AL_HI_QUEUE_ID)

#endif
#if defined( UPGRADE_AD6532 )
  KI_TASK_DEF (SIM_IF_CHECK_TASK_ID,  SIM_IF_CHECK_QUEUE_ID,  SimIfCheckTask,    450,    5, SIM_IF_CHECK_TASK_PRIO)
#else
  KI_RESERVED_TASK (SIM_IF_CHECK_TASK_ID,  SIM_IF_CHECK_QUEUE_ID)
#endif


/* include as part of Camera Multiplex layer Test Task */
#if defined (UPGRADE_CAMERA_DEVICE)
# if defined (ENABLE_CAMERA_TESTTASK)
  KI_TASK_DEF (CATT_TASK_ID,      CATT_QUEUE_ID,      dmCameraTestTask,  1024,     30, CATT_TASK_PRIO)
# else /* ENABLE_CAMERA_TESTTASK */
  KI_RESERVED_TASK (CATT_TASK_ID,  CATT_QUEUE_ID)
# endif /* ENABLE_CAMERA_TESTTASK */
#else /* UPGRADE_CAMERA_DEVICE */
  KI_RESERVED_TASK (CATT_TASK_ID,  CATT_QUEUE_ID)
#endif /* UPGRADE_CAMERA_DEVICE */

/* This is the USB EMMI task */
#if defined (USB_TTPCOM_EMMI_INTERFACE)
# if !defined (UPGRADE_USB)
#  error "Must have UPGRADE_USB defined with USB_TTPCOM_EMMI_INTERFACE"
# endif
# if defined (SWI_TTPCOM_EMMI_INTERFACE) || defined (SOC_TTPCOM_EMMI_INTERFACE) || defined (GX2_TTPCOM_EMMI_INTERFACE)
#  error "Must not have any other EMMI interface defined. USB interface already defined"
# endif
  KI_TASK_DEF (USB_EMMI_COMMS_TASK_ID,   USB_EMMI_COMMS_QUEUE,    UsbEmmiCommsTask, 512,    150, USB_EMMI_COMMS_TASK_PRIO)
#else /* USB_TTPCOM_EMMI_INTERFACE */
# if defined (SWI_TTPCOM_EMMI_INTERFACE)
#  if defined (SOC_TTPCOM_EMMI_INTERFACE) || defined (GX2_TTPCOM_EMMI_INTERFACE)
#   error "must not have any other EMMI interface defined. SWI interface already defined"
#  endif
    /* This is the SWI EMMI task. Used primarly on the armulator. */
    KI_TASK_DEF (SWI_EMMI_COMMS_TASK_ID,   SWI_EMMI_COMMS_QUEUE,    SwiEmmiCommsTask, 512,    150, SWI_EMMI_COMMS_TASK_PRIO)
# elif defined (SOC_TTPCOM_EMMI_INTERFACE)
#  if defined (GX2_TTPCOM_EMMI_INTERFACE)
#   error "must not have any other EMMI interface defined. SOC interface already defined"
#  endif
    /* This is the TCP\IP socket EMMI task. Used for PC simulation, should be portable to both Windows and Linux */
    KI_TASK_DEF (SOC_EMMI_COMMS_TASK_ID,   SOC_EMMI_COMMS_QUEUE,    SocEmmiCommsTask, 20000,  150, SWI_EMMI_COMMS_TASK_PRIO)
# elif defined (GX2_TTPCOM_EMMI_INTERFACE)
   /* This is the Windows Pipes EMMI task for direct communictaion with genie.
    * Used for PC simulation running under Geine's GX2 interface. Will only run on windows */
    KI_TASK_DEF (GX2_EMMI_COMMS_TASK_ID,   GX2_EMMI_COMMS_QUEUE,    Gx2EmmiCommsTask, 512,    150, SWI_EMMI_COMMS_TASK_PRIO)
# else
    /* Reserve this task ID */
    KI_RESERVED_TASK( USB_EMMI_COMMS_TASK_ID, USB_EMMI_COMMS_QUEUE )
# endif
#endif /* !USB_TTPCOM_EMMI_INTERFACE */

#if defined (ENABLE_TELEPHONY_AUDIO_CONTROLLER)
  KI_TASK_DEF (TAC_TASK_ID,         TAC_QUEUE_ID,     tacTask,         2536,    30, TAC_TASK_PRIO)
#else /* !defined (ENABLE_TELEPHONY_AUDIO_CONTROLLER) */
  KI_RESERVED_TASK (TAC_TASK_ID,    TAC_QUEUE_ID)
#endif /* !defined (ENABLE_TELEPHONY_AUDIO_CONTROLLER) */

KI_START_ALIAS (DM_ALIASES)
#if !defined (ENABLE_SIM_DRV_V2)
# if !defined(TTC_PLATFORM_T1M) || defined(SIM_EMULATION_ON)
  KI_ALIAS_DEF   (L1_SD_TASK_ID,     SIMDRV_TASK_ID, SIMDRV_QUEUE_ID)
#endif
#else
  KI_ALIAS_DEF   (FR_SD_TASK_ID,     SIMDRV_TASK_ID, SIMDRV_QUEUE_ID)
#endif
KI_STOP_ALIAS  (DM_ALIASES)

#if defined (MEMORY_TEST_ENABLE)
KI_TASK_DEF(MEMORY_ACCESS_TEST_TASK_ID,MEMORY_ACCESS_TEST_QUEUE_ID,MemoryAccessTestTask,512, 10, MEMORY_ACCESS_TEST_TASK_PRIO)
#endif /* MEMORY_TEST_ENABLE */
/* END OF FILE */
