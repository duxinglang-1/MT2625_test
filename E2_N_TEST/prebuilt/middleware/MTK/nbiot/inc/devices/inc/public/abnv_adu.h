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
 *  Background layer NVRAM data item types.
 **************************************************************************/
  MobileEquipmentData    mobileEquipmentData;
  AbsmDataArea          absmDataArea;
#ifndef GEN_NVDM_DATABASE_FOR_PC
  NvRamSessionData      nvRamSessionData;
  LocalCallBarring      localCallBarring;
#endif
  AbshDataArea          abshDataArea;
  AbmmWriteableData     abmmWriteableData;
  EquivPlmns            equivalentPlmns;
  UserDefPlmnList       userDefPlmnList;
  CbSettings            cbSettings;
  CbCmbiFileRecord      cbmiRecord;
  AbemConfigData        abemConfigData;
  BandOptions           bandOptions;
  MePersonalisationData mepData;
  ApexSimFaultInd       simError;
  FatalError            fatalError;

  BaListInfo                  baListFile;

  KnownSimData               knownSimData;
  SmsTimeStampData           smsTimeStampData;

  AbpdApn                    apnStore;
  AbpdPsdMode                psdMode;

#if defined (UPGRADE_CTM)
  CtmSettings                ctmSettings;
#else /* !(UPGRADE_CTM) */
  Int32                      ctmSettingsNotPresent;
#endif /* !(UPGRADE_CTM) */
#ifndef GEN_NVDM_DATABASE_FOR_PC
  AblmSyncInfoArea           ablmSyncInfoArea;
#endif
  AbsiMwiData                absiMwiData;
  AbglFeatureConfigDataArea  abglFeatureConfigDataArea;
  UtVoiceCodingType          abglSupportedCodecsDataArea;
  SimatMmiSupportProfile     simatMmiSupportProfile;
#if !defined(UPGRADE_TWGGE_NRW)
#if defined (UPGRADE_3G) && defined (UPGRADE_3G_MBMS) && defined (UPGRADE_3G_TDD128)
  AbMbmsSecurityControlRecord abMbmsSecurityControlRecord;
#else
  Int32                      abMbmsSecurityControlNotPresent;
#endif /* UPGRADE_3G */
#endif
#if defined(UPGRADE_TWGGE_NRW)
  MeEhplmnList               meEhplmnList;
#else
#if defined(ENABLE_EHPLMN) && defined(ENABLE_CMCC_FEATURE)
  MeEhplmnList               meEhplmnList;
#endif
#endif
#if defined(UPGRADE_TWGGE_NRW)

  Anrm2Rel6DynamicsCap       anrm2Rel6DynamicsCap;

#else
#if defined (UPGRADE_3G_FDD) && defined(UPGRADE_REL6) && defined(REL6_DYNAMICS_CAPABILITIES)
  Anrm2Rel6DynamicsCap       anrm2Rel6DynamicsCap;
#endif
#endif
  UsimEpsLocInfo             epsLocInfo;
  UsimEpsNasSecurity         epsNasSecurityContext;
  Imsi                       epsEmmInfoImsi;
