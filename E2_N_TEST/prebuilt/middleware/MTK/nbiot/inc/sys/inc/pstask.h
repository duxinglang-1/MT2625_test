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
 * File Description : Protocol Stack task definitions
 **************************************************************************/

/*
** This file deliberately does not have a multiple inclusion protection
** as it can be legitimately included twice.
**
** This must never be included directly by any file outside of the GKI.
*/

#define EXCLUDE_GSM_PS_AS
#define EXCLUDE_GPRS_PS_AS

/*
**   MACRO           GKI Task ID,        GKI Queue ID,       Entry Point   Stack, Queue, Priority
*/

#if !defined (NB_PC_HW_SIM)

  KI_TASK_DEF      ( MM_TASK_ID,         MM_QUEUE_ID,        MmTask,         2800,  30, MM_TASK_PRIO         )
  KI_TASK_DEF      ( SIM_TASK_ID,        SIM_QUEUE_ID,       SimManagerTask, 2640,  30, SIM_TASK_PRIO        )
  KI_TASK_DEF      ( SMTL_TASK_ID,       SMTL_QUEUE_ID,      SmtlTask,       400,  30,  SMTL_TASK_PRIO       )
  KI_TASK_DEF      ( SMCM_TASK_ID,       SMCM_QUEUE_ID,      SmcmTask,      2640,  30,  SMCM_TASK_PRIO       )
  KI_TASK_DEF      ( SMRL_TASK_ID,       SMRL_QUEUE_ID,      SmrlTask,      1750,  30,  SMRL_TASK_PRIO       )


KI_START_ALIAS     ( PS_NAS_ALIASES )
  KI_ALIAS_DEF     ( MM_TASK_ID,         GP_GMM_TASK_ID,     GP_GMM_QUEUE_ID   )
  KI_ALIAS_DEF     ( SMCM_TASK_ID,       GP_SMS_TASK_ID,     GP_SMS_QUEUE_ID   )
KI_STOP_ALIAS      ( PS_NAS_ALIASES )

#endif // NB_PC_HW_SIM


#ifdef FEA_LPP_SUPPORT
  KI_TASK_DEF      ( LPP_TASK_ID,         LPP_QUEUE_ID,        LppTask,         2800,  30, LPP_TASK_PRIO         )
  KI_TASK_DEF      ( LBS_TASK_ID,         LBS_QUEUE_ID,        LbsTask,         2800,  30, LBS_TASK_PRIO         )
#endif

#if !defined (NB_PC_HW_SIM)
#if defined (ENABLE_AT_ENG_MODE)
#if !defined (VG_CI_TASK_QUEUE_INCREASE)
#define VG_CI_TASK_QUEUE_INCREASE 0
#endif
  KI_TASK_DEF      ( VG_CI_TASK_ID,      VG_CI_QUEUE_ID,     VgCiTask,      5716,  (50 + VG_CI_TASK_QUEUE_INCREASE), VG_CI_TASK_PRIO )
#else
  KI_TASK_DEF      ( VG_CI_TASK_ID,      VG_CI_QUEUE_ID,     VgCiTask,      5716,  30,  VG_CI_TASK_PRIO      )
#endif
#else
  KI_RESERVED_TASK ( VG_CI_TASK_ID,      VG_CI_QUEUE_ID)
#endif

#if !defined (NB_PC_HW_SIM)
  KI_TASK_DEF      ( TASK_BL_ID,         BL_QUEUE_ID,        abshTask,     15360,  30,  TASK_BL_PRIO         )
#else
  KI_RESERVED_TASK ( TASK_BL_ID,         BL_QUEUE_ID)
#endif

#if !defined (NB_PC_HW_SIM)

#ifdef ENABLE_AP_BRIDGE_FEATURE
  KI_TASK_DEF      ( VG_APB_TASK_ID,     VG_APB_QUEUE_ID,    VgApbTask,     1536,  30,  VG_APB_TASK_PRIO )
#else
  KI_RESERVED_TASK ( VG_APB_TASK_ID,     VG_APB_QUEUE_ID)
#endif


  KI_TASK_DEF      ( MUX_TASK_ID,        MUX_QUEUE_ID,       MuxTask,       4192,  30,  MUX_TASK_PRIO        )

#ifdef UE_SIMULATOR
  KI_TASK_DEF      ( VMMI_TASK_ID,       VMMI_QUEUE_ID,      Vmmi_Task,     4192,  30,  VMMI_TASK_PRIO       )
#endif

#if !defined (MTK_NBIOT_TARGET_BUILD) && !defined (UT_TARGET_CONFIG)
  KI_TASK_DEF      ( MUXCONN_SERIAL_TASK_ID, MUXCONN_SERIAL_QUEUE_ID, MuxConnSerialTask, 1000, 30, MUXCONN_SERIAL_TASK_PRIO )
#else
  KI_RESERVED_TASK ( MUXCONN_SERIAL_TASK_ID, MUXCONN_SERIAL_QUEUE_ID                    )
#endif

/* Referred by L4 test cases. Please remove once L4 legacy test cases have been removed. */
KI_RESERVED_TASK ( GP_SNDCP_TASK_ID,   GP_SNDCP_QUEUE_ID     )

#if defined (UPGRADE_NASMDL2)
  KI_TASK_DEF      ( GP_SM_TASK_ID,      GP_SM_QUEUE_ID,     SmTask,         1536,  30,  GP_SM_TASK_PRIO      )
#else /* UPGRADE_NASMDL2 */
  KI_TASK_DEF      ( GP_SM_TASK_ID,      GP_SM_QUEUE_ID,     GpSmTask,       750,  30,  GP_SM_TASK_PRIO      )
#endif /* else UPGRADE_NASMDL2 */

#if defined (MTK_PPP_ENABLE)
#if defined (GPRS_LOG_PPP)
  KI_TASK_DEF      ( GP_PPP_TASK_ID,     GP_PPP_QUEUE_ID,    GpPppTask,     6050,  60,  GP_PPP_TASK_PRIO     )
#else
  KI_TASK_DEF      ( GP_PPP_TASK_ID,     GP_PPP_QUEUE_ID,    GpPppTask,     3072,  60,  GP_PPP_TASK_PRIO     )
#endif /* GPRS_LOG_PPP */
#else
  KI_RESERVED_TASK ( GP_PPP_TASK_ID,     GP_PPP_QUEUE_ID   )
#endif /* MTK_PPP_ENABLE */

  KI_TASK_DEF      ( DBM_TASK_ID,        DBM_TASK_QUEUE_ID,  DbmTask,       5000,  30,  DBM_TASK_PRIO      )

  KI_TASK_DEF      ( EL2_TASK_ID,        EL2_TASK_QUEUE_ID,  El2Task,       5000,  30,  EL2_TASK_PRIO      )
  KI_TASK_DEF      ( ERRC_TASK_ID,       ERRC_QUEUE_ID,      ErrcTask,     10000,  30,  ERRC_TASK_PRIO        )

  /* Please use ERRC_TASK instead of EP_RRC_TASK in code. EP_RRC_TASK definitions will be removed. */
  KI_START_ALIAS   ( PS_LTE_AS_ALIASES )
  KI_ALIAS_DEF     ( ERRC_TASK_ID,         EP_RRC_TASK_ID,     EP_RRC_QUEUE_ID   )
  KI_STOP_ALIAS    ( PS_LTE_AS_ALIASES )

  KI_START_ALIAS   ( USE_ANRM2 )
  KI_ALIAS_DEF     ( DM_NVM_TASK_ID,     TASK_ANRM2_ID, ANRM2_QUEUE_ID         )
  KI_STOP_ALIAS    ( USE_ANRM2 )

#endif // NB_PC_HW_SIM

/* END OF FILE */
