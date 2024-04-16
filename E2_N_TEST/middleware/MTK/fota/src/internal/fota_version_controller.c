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

#include "fota_version_controller.h"
#include "verno.h"
#include "syslog.h"

#define FOTA_ERR(fmt,arg...)   LOG_E(fota, "[FOTA]: "fmt,##arg)
#define FOTA_WARN(fmt,arg...)  LOG_W(fota, "[FOTA]: "fmt,##arg)
#define FOTA_DBG(fmt,arg...)   LOG_I(fota,"[FOTA]: "fmt,##arg)

typedef struct {
    uint32_t main_ver_no;
    uint32_t bl_ver_no;
    uint8_t sw_time[32];
    uint8_t sdk_version[64];
}fota_sw_version_t;

static const fota_sw_version_t g_fota_current_sw_version = {
    .main_ver_no = 11000,
    .bl_ver_no   = 11000,
    .sw_time     = __DATE__ "," __TIME__,
    .sdk_version = MTK_FW_VERSION
};


bool fota_version_controller_check(uint32_t version_no)
{
    bool result = false;
    FOTA_DBG("fota version control: cur_version=%d, new_versioin=%d",
             g_fota_current_sw_version.main_ver_no, version_no);
    FOTA_DBG("sw_time:%s,sdk_version:%s", g_fota_current_sw_version.sw_time,
                                          g_fota_current_sw_version.sdk_version);
    if (version_no > g_fota_current_sw_version.main_ver_no){
        result = true;
    }
    return result;
}

