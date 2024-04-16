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

#ifndef __RIL_LOG_H__
#define __RIL_LOG_H__

#include "ril_general_types.h"

#include "syslog.h"

/* There are 3 log levels for debugging, FULL, SLIM, LEAST. Defautl is SLIM level. */
//#define RIL_LOG_LEVEL_FULL
#define RIL_LOG_LEVEL_SLIM
//#define RIL_LOG_LEVEL_LEAST

/* printf buffer is limited to maximum 255 bytes, excpet trace title so only allow 200 bytes to print */
#define RPINT_TRACE_MAX_LEN    (200)

#ifndef MTK_DEBUG_LEVEL_NONE
#if defined(RIL_LOG_LEVEL_FULL)
#define RIL_LOGI(fmt, args...)               LOG_I(ril, "[RIL] "fmt, ##args)
#define RIL_LOGW(fmt, args...)               LOG_W(ril, "[RIL] "fmt, ##args)
#define RIL_LOGE(fmt, args...)               LOG_E(ril, "[RIL] "fmt, ##args)
#define RIL_LOGMEM(fmt, args...)             //LOG_I(ril, "[RIL][MEM] "fmt, ##args)
#define RIL_LOGCT(fmt, args...)              LOG_I(ril, "[RIL] "fmt, ##args)
#define RIL_LOGUT(fmt, args...)              LOG_I(ril, "[RIL] "fmt, ##args)
#define RIL_LOGDUMPSTR(fmt, len, args...)    LOG_I(ril, "[RIL] "fmt, ##args)
#define RIL_LOGDUMPSTRCT(fmt, len, args...)  LOG_I(ril, "[RIL] "fmt, ##args)
#define RIL_LOGDUMPSTRUT(fmt, len, args...)  LOG_I(ril, "[RIL] "fmt, ##args)
#elif defined(RIL_LOG_LEVEL_SLIM)
#define RIL_LOGI(fmt, args...)
#define RIL_LOGW(fmt, args...)               LOG_W(ril, "[RIL] "fmt, ##args)
#define RIL_LOGE(fmt, args...)               LOG_E(ril, "[RIL] "fmt, ##args)
#define RIL_LOGMEM(fmt, args...)
#define RIL_LOGCT(fmt, args...)              LOG_I(ril, "[RIL] "fmt, ##args)
#define RIL_LOGUT(fmt, args...)              LOG_I(ril, "[RIL] "fmt, ##args)
#define RIL_LOGDUMPSTR(fmt, len, args...)
#define RIL_LOGDUMPSTRCT(fmt, len, args...)  LOG_I(ril, "[RIL] "fmt, ##args)
#define RIL_LOGDUMPSTRUT(fmt, len, args...)  LOG_I(ril, "[RIL] "fmt, ##args)
#elif defined(RIL_LOG_LEVEL_LEAST)
#define RIL_LOGI(fmt, args...)
#define RIL_LOGW(fmt, args...)
#define RIL_LOGE(fmt, args...)
#define RIL_LOGMEM(fmt, args...)
#define RIL_LOGCT(fmt, args...)              LOG_I(ril, "[RIL] "fmt, ##args)
#define RIL_LOGUT(fmt, args...)
#define RIL_LOGDUMPSTR(fmt, len, args...)
#define RIL_LOGDUMPSTRCT(fmt, len, args...)  LOG_I(ril, "[RIL] "fmt, ##args)
#define RIL_LOGDUMPSTRUT(fmt, len, args...)
#else
#define RIL_LOGI(fmt, args...)
#define RIL_LOGW(fmt, args...)
#define RIL_LOGE(fmt, args...)
#define RIL_LOGMEM(fmt, args...)
#define RIL_LOGCT(fmt, args...)
#define RIL_LOGUT(fmt, args...)
#define RIL_LOGDUMPSTR(fmt, len, args...)
#define RIL_LOGDUMPSTRCT(fmt, len, args...)
#define RIL_LOGDUMPSTRUT(fmt, len, args...)
#endif

#else /* use printf to output traces */

#define RIL_LOGI(fmt, args...)
#define RIL_LOGW(fmt, args...)
#define RIL_LOGE(fmt, args...)
#define RIL_LOGMEM(fmt, args...)
#define RIL_LOGCT(fmt, args...)
#define RIL_LOGUT(fmt, args...)
#define RIL_LOGDUMPSTR(fmt, len, args)
#define RIL_LOGDUMPSTRCT(fmt, len, args...)
#define RIL_LOGDUMPSTRUT(fmt, len, args...)

#endif /* MTK_DEBUG_LEVEL_NONE */

#endif /* __RIL_LOG_H__ */

