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
 *  Foreground layer NVRAM data item types.
 **************************************************************************/

/* Note this was the content of afnv_adu_nonmmi2.h when afnv_adu.h selected
 * between variant files.  Now there is only one variant, it is here.
 */


/*SPECIAL cut down list for AB only builds */
  LocalConfiguration    localConfiguration;
  ProfileData           profileDataEntity0;
  ProfileData           profileDataEntity1;
  ProfileData           profileDataEntity2;
  ProfileData           profileDataEntity3;  
  ProfileData           profileDataEntity4;
  ProfileData           profileDataEntity5;
  ProfileData           profileDataEntity6;
  ProfileData           profileDataEntity7;
  ProfileData           profileDataEntity8;
  ProfileData           profileDataEntity9;
  ProfileData           profileDataEntity10;
  ProfileData           profileDataEntity11;
  ProfileData           profileDataEntity12;
  ProfileData           profileDataEntity13;
  ProfileDataGeneric    profileDataGeneric;
  AtIdentificationText  atIdentificationText;
  Int32                 afmsCustomMenusDataNotPresent;
  Int32                 userDefCbmiListNotPresent;
  Int32                 testMobileDataNotPresent;
  Int32                 bookmarksInfoNotPresent;
  Int32                 audioProfileDataNotPresent;
  Int32                 groupRingToneSettingsNotPresent;
  Int32                 afgaSettingsNotPresent;
  Int32                 gprsCounterDatanotPresent;
  Int32                 afwaConfigDataNotPresent;
#if !defined(UPGRADE_GENERIC_MUX)
  MuxConfigData         muxConfigData;
#else
  Int32                 muxConfigDataNotPresent;
#endif


