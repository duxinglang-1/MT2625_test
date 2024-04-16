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
 **************************************************************************/
/****************************************************************************
 * This file is used for generate NVDM database
 ****************************************************************************/
#ifndef __NVDM_EDITOR_H__
#define __NVDM_EDITOR_H__

#include <system.h>
#include "serial_port.h"
#undef NVDM_AP_ITEM_DEF
#undef NVDM_AP_ITEM_DEF_NOGEN_DESCRIPTION
#define NVDM_AP_ITEM_DEF(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VERNO) NV_NAME,
#define NVDM_AP_ITEM_DEF_NOGEN_DESCRIPTION(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VERNO) NV_NAME,
typedef enum
{
    NVDM_AP_START_LID = 0x0,
    #include "nvdm_data_item_table.h"
    NVDM_AP_ITEM_MAX
}NVDM_AP_LID;

#ifdef GEN_NVDM_DATABASE_FOR_PC
#define VER_LID(x) "000" x


/****************************************************************************
 * Structure description define
 ****************************************************************************/
BEGIN_NVRAM_DATA
#undef NVDM_AP_ITEM_DEF
#define NVDM_AP_ITEM_DEF(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VERNO) LID_BIT "000" NV_NAME \
                                                     STRUCTURE * 1 \
                                                     {             \
                                                     };
#include "nvdm_data_item_table.h"
END_NVRAM_DATA
#endif/* GEN_NVDM_DATABASE_FOR_PC */

#endif
