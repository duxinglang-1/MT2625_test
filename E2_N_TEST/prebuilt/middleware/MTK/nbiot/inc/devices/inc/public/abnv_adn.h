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
 *  Background layer NVRAM data item names.
 **************************************************************************/


  NRAM2_MOBILE_EQUIPMENT_DATA = ANRM2_ABNV_BASE,     /* Mobile Equipment Data for ME_DATA_REQ */
  NRAM2_SMS_DATA,                  /* BL SMS Data (FL is in Local Config)   */
  NRAM2_ABCC_SESSION_DATA,         /* Call Control Session Data             */
  NRAM2_LOCAL_CALL_BARRING,        /* Call Control Local Call Barring       */
  NRAM2_SHELL_MISC_DATA,           /* Misc data managed by BL Shell         */

  NRAM2_ABMM_WRITEABLE_DATA,
#if defined(UPGRADE_TWGGE_NRW)
  NRAM2_ABMM_WRITEABLE_DATA_WGGE,
#endif
  NRAM2_EQUIV_PLMNS,
  NRAM2_USER_DEF_PLMN_LIST,         /* User defined Plmn networks */
  NRAM2_CB_SETTINGS,                /* Cell broadcast NVRAM file used in addition to SIM */
  NRAM2_CB_CBMI_FILE,               /* Cell broadcast CBMI filter list */
  NRAM2_ABEM_CONFIG_DATA,
  NRAM2_BAND_OPTIONS,
  NRAM2_ME_PERSONALISATION,
  NRAM2_SIM_ERROR,
  NRAM2_FATAL_ERROR,
  NRAM2_LND_FILE,
  NRAM2_LNR_FILE,
  NRAM2_LNM_FILE,
  NRAM2_ADR_FILE,
  NRAM2_BA_LIST_FILE,
  NRAM2_KNOWN_SIM_DATA,
  NRAM2_SMS_TIMESTAMPS,
  NRAM2_APN_STORE,
  NRAM2_PSD_MODE,
  NRAM2_CTM_SETTINGS,
  NRAM2_PDP_TABLE,
  NRAM2_FDD_LIST_DATA,
  NRAM2_TDD128_LIST_DATA,
  NRAM2_ABLM_SYNC_INFO,
  NRAM2_ABSI_MWI_DATA,
  NRAM2_AB_FEATURES_CONFIG,
  NRAM2_AB_SUPPORTED_CODECS,
  NRAM2_STK_MMI_SUPPORT_INFO,
  NRAM2_MBMS_KEY_TABLE,
#if defined(UPGRADE_TWGGE_NRW)
  NRAM2_EHPLMN_LIST,
#else
#if defined(ENABLE_EHPLMN) && defined(ENABLE_CMCC_FEATURE)
  NRAM2_EHPLMN_LIST,
#endif
#endif
#if defined (UPGRADE_3G) || defined(UPGRADE_TWGGE_NRW)
  /* CUSTOM_COMMAND */
  NRAM2_CUSTOM_COMMAND_CONFIG,
#endif
#if defined(UPGRADE_TWGGE_NRW)
  NRAM2_REL6_DYNAMICS_CAP,
#else
#if defined (UPGRADE_3G_FDD) && defined(UPGRADE_REL6) && defined(REL6_DYNAMICS_CAPABILITIES)
  NRAM2_REL6_DYNAMICS_CAP,
#endif
#endif

  NRAM2_EPS_LOCATION_INFO,
  NRAM2_EPS_NAS_SECURITY_CONTEXT,
  NRAM2_EPS_EMM_INFO_IMSI,

