/* Copyright Statement:
*
* (C) 2005-2018  MediaTek Inc. All rights reserved.
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
*******************************************************************************/
/***************************************************************************//**
*  Includes
*******************************************************************************/
#ifdef MTK_NVDM_OTA_SUPPORT
#include "string.h"
#include "nvdm_verno.h"

#undef NVDM_AP_ITEM_DEF
#undef NVDM_AP_ITEM_DEF_NOGEN_DESCRIPTION

#define NVDM_AP_ITEM_DEF_NOGEN_DESCRIPTION(NV_NAME, structure, GROUP_NAME, ITEM_NAME, area, default, USE_VER, verno) NVDM_AP_ITEM_DEF_NOGEN_DESCRIPTION_##USE_VER(GROUP_NAME, ITEM_NAME, verno)
#define NVDM_AP_ITEM_DEF(NV_NAME, structure, GROUP_NAME, ITEM_NAME, area, default, USE_VER, verno) NVDM_AP_ITEM_DEF_##USE_VER(GROUP_NAME, ITEM_NAME, verno)

#define NVDM_AP_ITEM_DEF_NOGEN_DESCRIPTION_NO_VER(GROUP_NAME, ITEM_NAME, verno)
#define NVDM_AP_ITEM_DEF_NOGEN_DESCRIPTION_USE_VER(GROUP_NAME, ITEM_NAME, verno) {GROUP_NAME, ITEM_NAME, verno},
#define NVDM_AP_ITEM_DEF_NO_VER(GROUP_NAME, ITEM_NAME, verno)
#define NVDM_AP_ITEM_DEF_USE_VER(GROUP_NAME, ITEM_NAME, verno) {GROUP_NAME, ITEM_NAME, verno},

const nvdm_item_verno_info_t nvdm_ap_item_verno_table[] =
{
     #include "nvdm_data_item_table.h"
     {"", "", 0xFF}
};

static uint16_t nvdm_ap_item_name_value[sizeof(nvdm_ap_item_verno_table)/sizeof(nvdm_item_verno_info_t)];

nvdm_item_verno_table_info_t nvdm_ap_item_verno_table_info =
{
    &nvdm_ap_item_verno_table[0],
    (sizeof(nvdm_ap_item_verno_table)/sizeof(nvdm_item_verno_info_t) - 1),
    &nvdm_ap_item_name_value[0],
    false
};


/******************************************************************************
*  INTERFACE FUNCTIONS
*******************************************************************************/
uint8_t nvdm_get_ap_item_verno(char* group_name, char* item_name)
{
    return nvdm_get_item_verno(&nvdm_ap_item_verno_table_info, group_name, item_name);
}
#endif
