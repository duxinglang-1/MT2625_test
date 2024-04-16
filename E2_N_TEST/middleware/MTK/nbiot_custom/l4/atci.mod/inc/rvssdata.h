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
 * Types definition for Supplementary Service Sub System.
 **************************************************************************/

#ifndef RVSSDATA_H
#define RVSSDATA_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif
#if !defined (KI_SIGBUF_H)
#  include <ki_sigbuf.h>

#endif
#include <rvsystem.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

#define MAX_FAC_ID_LEN                    (2)
#define MAX_CALL_CLASS_EVENTS             (4)

#define MAX_COMMAND_GROUP_LEN (3)

/* job121581: change the default DCS value to 0x0f (language unspecified) */
/* although the value 0 is as specified in 27.007 for AT+CUSD, it is actually */
/* the value for German language and as such causes networks to generate errors */
#define DEFAULT_DCS                        0x0f

/* L2P numbers used in ATD*99# */
#define VG_SS_L2P_PPP_PROTOCOL                (1)
#define VG_SS_L2P_CORE_PPP_PROTOCOL           (90000)
#define VG_SS_L2P_PACKET_TRANSPORT_PROTOCOL   (90001)

/***************************************************************************
 * Type Definitions
 **************************************************************************/


typedef enum VgSsModeTypeTag
{
  VG_AT_SS_MODE_MIN = 0,
  VG_AT_SS_MODE_DISABLE = VG_AT_SS_MODE_MIN,
  VG_AT_SS_MODE_ENABLE,
  VG_AT_SS_MODE_QUERY_STATUS,
  VG_AT_SS_MODE_REGISTRATION,
  VG_AT_SS_MODE_ERASURE,
  VG_AT_SS_MODE_NOT_SUPPLIED = 255,
  VG_AT_SS_MODE_MAX
}
VgSsModeType;

typedef Int16 VgSsClassType;

typedef enum VgSsClassTypeListTag
{
  VG_AT_SS_CLASS_VOICE = 1,
  VG_AT_SS_CLASS_DATA = 2,
  VG_AT_SS_CLASS_FAX = 4,
  VG_AT_SS_CLASS_SMS = 8,
  VG_AT_SS_CLASS_DATA_CIRCUIT_SYNC = 16,
  VG_AT_SS_CLASS_DATA_CIRCUIT_ASYNC = 32,
  VG_AT_SS_CLASS_DEDICATED_PACKET_ACCESS = 64,
  VG_AT_SS_CLASS_DEDICATED_PAD_ACCESS = 128,
  VG_AT_SS_EXTENDED_BEARER_SERVICES = 0x100,
  VG_AT_SS_EXTENDED_TELE_SERVICES = 0x200,
  VG_AT_SS_END_OF_EXTENDED_SERVICES = 0x300,
  VG_AT_SS_CLASS_DEFAULT = 7
}
VgSsClassTypeList;

typedef enum VgSsPresentationTypeTag
{
  VG_AT_SS_PRESENTATION_MIN = 0,
  VG_AT_SS_PRESENTATION_DISABLE = VG_AT_SS_PRESENTATION_MIN,
  VG_AT_SS_PRESENTATION_ENABLE,
  VG_AT_SS_PRESENTATION_NO_UPDATE = 255,
  VG_AT_SS_PRESENTATION_MAX
}
VgSsPresentationType;

typedef enum VgSsNetworkFacilityTypeTag
{
  VG_AT_SS_FAC_MIN = 0,
  VG_AT_SS_FAC_SC = VG_AT_SS_FAC_MIN,

  /**************************************/
  /* NOTE: For NB-IOT all call related  */
  /* <fac> are not supported            */
  /**************************************/

#if defined (FEA_PHONEBOOK)
  VG_AT_SS_FAC_FD,
  VG_AT_SS_FAC_BD,
#endif /* FEA_PHONEBOOK */

#if defined (FEA_SIMLOCK)
  VG_AT_SS_FAC_PS,
  VG_AT_SS_FAC_PN,
  VG_AT_SS_FAC_PU,
  VG_AT_SS_FAC_PP,
  VG_AT_SS_FAC_PC,
#endif /* FEA_SIMLOCK */

  /* Pin2 must ALWAYS be last! */
  VG_AT_SS_FAC_P2,

  VG_AT_SS_FAC_MAX
}
VgSsNetworkFacilityType;

typedef struct VgSsSignalParamsTag
{
  VgSsModeType              mode;
  /* added for job103015: needed for TC 31.8.1.2.3. */
  Char                      cnfNewPassword[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH];
  Int16                     cnfNewPasswordLength;
  Char                      newPassword[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH];
  Int16                     newPasswordLength;
  Char                      currentPassword[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH];
  Int16                     currentPasswordLength;
  VgSsNetworkFacilityType   currentFac;
  /* job109084: stored SS string (from ATD command) for general FDN/Call Control by SIM issues */
  Boolean                   ssOpFromATD;
  Int8                      ssString[MAX_ADDR_LEN];
  Int16                     ssStringLength;
}
VgSsSignalParams;

typedef struct VgSsNetworkFacilityDescriptorTag
{
  VgSsNetworkFacilityType vgFacId;
  Char                    vgFacStr[MAX_FAC_ID_LEN + NULL_TERMINATOR_LENGTH];
  Int8                    vgPasswordLen;
}
VgSsNetworkFacilityDescriptor;

typedef struct VgSsCallerIdDataTag
{
  Boolean             dialNumValid;
  DialledBcdNum       dialNum;
  Boolean             alphaIdValid;
  SimAlphaIdentifier  alphaId;
  Boolean             subAddressValid;
  SubAddress          subAddress;
}
VgSsCallerIdData;

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

#endif  /* RVSSDATA_H */

/* END OF FILE */
