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
 *  type definitions used within AF NVRAM item structures.
 **************************************************************************/

#ifndef AFNV_TYP_H
#define AFNV_TYP_H

/* Note this was the content of afnv_typ_nonmmi2.h when afnv_typ.h selected
 * between variant files.  Now there is only one variant, it is here.
 */

/****************************************************************************
 * USE_ABAPP section. AB builds only need two foreground layer items.
 * The other items do not have meaning in an AB only build                  */

#if !defined (SITL_TYP_H)
#  include "sitl_typ.h"
#endif

#if !defined (SITS_TYP_H)
#  include "sits_typ.h"
#endif

#  if !defined (RVPROF_H)
#    include <rvprof.h>
#  endif

#if !defined (UPGRADE_GENERIC_MUX)
#if !defined (VGMX_SIG_H)
#  include <vgmx_sig.h>
#endif
#endif

typedef enum ReplyPathTag
{
  REPLY_PATH_ENABLED,
  REPLY_PATH_DISABLED,

  NUM_OF_REPLY_PATH,
  REPLY_PATH_NULL
}
ReplyPath;

typedef struct ProfileDataTag
{
  ProfileContext_t profile;
}
ProfileData;

typedef struct ProfileGenericDataTag
{
  ProfileGenericContext_t profile;
}
ProfileDataGeneric;

typedef struct AtIdentificationInfoTag
{
  AtIdentificationText  atIdentificationText;
}
AtIdentificationInfo;

typedef struct LocalConfigurationTag
{
  Int8  dummy;
}
LocalConfiguration;

#if !defined(UPGRADE_GENERIC_MUX) || defined(UPGRADE_TWGGE_NRW)

#include <vgmx_typ.h>

typedef struct MuxConfigDataTag
{
  VgmuxId             muxId[VG_NO_OF_MUX];
  VgmxComPortSettings comPortSettings[VG_NO_OF_MUX];
  Int16               validityCode;
} MuxConfigData;
#endif

#endif /* AFNV_TYP_H */

/*END OF FILE*/
