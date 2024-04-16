/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
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
 
#include "nidd_internal.h"

#ifdef NIDD_UT

#include "tel_conn_mgr_app_api.h"

log_create_module(nidd_ut, PRINT_LEVEL_INFO);
#define LOGE(fmt,arg...)   LOG_E(nidd_ut, "[NIDD UT]: "fmt,##arg)
#define LOGW(fmt,arg...)   LOG_W(nidd_ut, "[NIDD UT]: "fmt,##arg)
#define LOGI(fmt,arg...)   LOG_I(nidd_ut, "[NIDD UT]: "fmt,##arg)

extern nidd_context_struct* g_nidd_cnt;


void nidd_ut_event_handler(nidd_event_t event, void *data)
{
    LOGI("nidd_ut_event_handler event %d", event);
    switch(event)
    {
        case NIDD_EVENT_CHANNEL_ACTIVATE_IND:     
            LOGI("nidd received data activate !!!");       
            break;
            
        case NIDD_EVENT_CHANNEL_DEACTIVATE_IND:
            LOGI("nidd received data deactivate !!!");      
            break;
            
        case NIDD_EVENT_DATA_IND:
            LOGI("nidd received data %s", data);
            break;

        default:
            break;
    }
}

void nidd_ut_app_activate(char* apn)
{
    nidd_ret_t nidd_ret = NIDD_RET_ERROR;
    uint32_t nidd_id = 0;
    
    nidd_ret = nidd_create_nidd_channel(&nidd_id, apn, nidd_ut_event_handler);
    LOGI("nidd_create_nidd_channel ret: %d, apn: %s, nidd_id: %d", nidd_ret, apn, nidd_id);
}

void nidd_ut_app_send_data(char* apn, void* data, uint32_t length)
{    
    nidd_channel_struct* channel = NULL;
    LOGI("nidd_ut_app_send_data");
    channel = nidd_get_channel_by_apn(apn);  
    if (channel != NULL) {
        nidd_send_data(channel->nidd_id, data, length);
    } else {
        LOGI("nidd_ut_app_send_data not find channel: %s", apn);        
    }
}

#endif


