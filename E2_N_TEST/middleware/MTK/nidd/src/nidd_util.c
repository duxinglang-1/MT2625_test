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
#include "string.h"
#include "nidd_internal.h"

extern nidd_context_struct* g_nidd_cnt;

log_create_module(nidd_util, PRINT_LEVEL_INFO);
#define LOGE(fmt,arg...)   LOG_E(nidd_util, "[NIDD UTIL]: "fmt,##arg)
#define LOGW(fmt,arg...)   LOG_W(nidd_util, "[NIDD UTIL]: "fmt,##arg)
#define LOGI(fmt,arg...)   LOG_I(nidd_util, "[NIDD UTIL]: "fmt,##arg)

nidd_channel_struct* nidd_get_channel_by_apn(char* apn)
{
    nidd_channel_struct* nidd_channel = NULL;
    uint32_t index = 0;
    LOGI("nidd_get_channel_by_apn apn %s \r\n", apn);

    nidd_channel = g_nidd_cnt->channel_list;

    for(index = 0; index < g_nidd_cnt->count; index ++) {
        if (nidd_channel == NULL) {
            continue;
        }
        if (0 == strncasecmp(nidd_channel->apn, apn, strlen(apn))) {
            LOGI("nidd_get_channel_by_apn return %d \r\n", nidd_channel->nidd_id);
            return nidd_channel;
        } else {
            nidd_channel = nidd_channel->next;
        }
    }

    return NULL;
}

nidd_channel_struct* nidd_get_channel_by_channelid(uint32_t channel_id)
{
    nidd_channel_struct* nidd_channel = NULL;
    uint32_t index = 0;
    LOGI("nidd_get_channel_by_channelid channel_id %d \r\n", channel_id);

    nidd_channel = g_nidd_cnt->channel_list;

    for(index = 0; index < g_nidd_cnt->count; index ++) {
        if (nidd_channel == NULL) {
            continue;
        }
        if (nidd_channel->channel_id == channel_id) {
            LOGI("nidd_get_channel_by_channelid return %d \r\n", nidd_channel->nidd_id);
            return nidd_channel;
        } else {
            nidd_channel = nidd_channel->next;
        }
    }

    return NULL;
}

nidd_channel_struct* nidd_get_channel_by_id(uint32_t nidd_id)
{
    nidd_channel_struct* nidd_channel = NULL;
    uint32_t index = 0;
    LOGI("nidd_get_channel_by_id nidd_id %d \r\n", nidd_id);

    nidd_channel = g_nidd_cnt->channel_list;

    for(index = 0; index < g_nidd_cnt->count; index ++) {
        if (nidd_channel == NULL) {
            continue;
        }
        if (nidd_channel->nidd_id == nidd_id) {
            LOGI("nidd_get_channel_by_id return %d \r\n", nidd_channel->channel_id);
            return nidd_channel;
        } else {
            nidd_channel = nidd_channel->next;
        }
    }
    return NULL;
}

void *nidd_mem_alloc(uint32_t size)
{
    void *pvReturn = NULL;
    uint32_t  free_size;
    free_size = xPortGetFreeHeapSize();
    if (free_size > size) {
        pvReturn = pvPortCalloc(1, size);
    } else {
        LOGW("nidd_mem_alloc, error \r\n");
    }

    return pvReturn;
}

void nidd_mem_free(void *buf)
{
    if (buf != NULL) {
        vPortFree(buf);
    } else {
        LOGW("nidd_mem_free, error \r\n");
    }
}

