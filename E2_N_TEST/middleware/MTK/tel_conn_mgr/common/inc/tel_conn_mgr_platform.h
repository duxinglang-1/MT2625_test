/* Copyright Statement:
 *
 * (C) 2005-2017 MediaTek Inc. All rights reserved.
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
 * the License Agreement ("Permitted User"). If you are not a Permitted User,
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

#ifndef __TEL_CONN_MGR_PLATFORM_H__
#define __TEL_CONN_MGR_PLATFORM_H__

/* FreeRTOS */
#include "string.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "syslog.h"
#include "task_def.h"
#include "hal_sleep_manager.h"
#include "nvdm.h"
#include "hal_sleep_manager_platform.h"

// /* tel_conn task */
#define TEL_CONN_TASK_NAME              ("tel_conn")
#define TEL_CONN_TASK_STACKSIZE         (4 * 1024)
#define TEL_CONN_TASK_PRIO              TASK_PRIORITY_ABOVE_NORMAL

#define TEL_CONN_MGR_PORT_STACK_TYPE portSTACK_TYPE

typedef SemaphoreHandle_t tel_conn_mgr_mutex_hdl_t;
typedef QueueHandle_t tel_conn_mgr_queue_hdl_t;
typedef TaskHandle_t tel_conn_mgr_task_hdl_t;
typedef unsigned char tel_conn_mgr_bool;
#define TEL_CONN_MGR_TRUE (1)
#define TEL_CONN_MGR_FALSE (0)

//typedef TickType_t tel_conn_mgr_tick_type_t;

/* Define 0 to disable logging, define 1 to enable logging. */
#if defined(__MTK_NBIOT_SLIM)
#define TEL_CONN_MGR_DEBUG 0

#elif !defined(MTK_DEBUG_LEVEL_NONE)
#define TEL_CONN_MGR_DEBUG 1
//#define TEL_CONN_MGR_DEBUG 0
#else
#define TEL_CONN_MGR_DEBUG 0
#endif


/* The following content is used inside the TEL_CONN_MGR module. */
#if TEL_CONN_MGR_DEBUG
#define USE_SYSLOG
#ifndef USE_SYSLOG
//#define TEL_CONN_MGR_LOG_INFO(fmt, ...) do {tel_conn_mgr_log_mutex_lock; printf("%s ", __FUNCTION__); printf(fmt, ##__VA_ARGS__); printf("\r\n"); vTaskDelay(10); tel_conn_mgr_log_mutex_unlock;} while(0);
//#define TEL_CONN_MGR_LOG_WARN(fmt, ...) do {tel_conn_mgr_log_mutex_lock; printf("%s ", __FUNCTION__); printf(fmt, ##__VA_ARGS__); printf("\r\n"); vTaskDelay(10); tel_conn_mgr_log_mutex_unlock;} while(0);
//#define TEL_CONN_MGR_LOG_ERR(fmt, ...) do {tel_conn_mgr_log_mutex_lock; printf("%s ", __FUNCTION__); printf(fmt, ##__VA_ARGS__); printf("\r\n"); vTaskDelay(10); tel_conn_mgr_log_mutex_unlock;} while(0);
#ifdef TEL_CONN_MGR_UT
#define TEL_CONN_MGR_LOG_INFO(fmt, ...) do {vTaskDelay(10); printf("%s ", __FUNCTION__); printf(fmt, ##__VA_ARGS__); printf("\r\n");} while(0);
#define TEL_CONN_MGR_LOG_WARN(fmt, ...) do {vTaskDelay(10); printf("%s ", __FUNCTION__); printf(fmt, ##__VA_ARGS__); printf("\r\n");} while(0);
#define TEL_CONN_MGR_LOG_ERR(fmt, ...) do {vTaskDelay(10); printf("%s ", __FUNCTION__); printf(fmt, ##__VA_ARGS__); printf("\r\n");} while(0);
#else
#define TEL_CONN_MGR_LOG_INFO(fmt, ...) do {printf("%s ", __FUNCTION__); printf(fmt, ##__VA_ARGS__); printf("\r\n");} while(0);
#define TEL_CONN_MGR_LOG_WARN(fmt, ...) do {printf("%s ", __FUNCTION__); printf(fmt, ##__VA_ARGS__); printf("\r\n");} while(0);
#define TEL_CONN_MGR_LOG_ERR(fmt, ...) do {printf("%s ", __FUNCTION__); printf(fmt, ##__VA_ARGS__); printf("\r\n");} while(0);
#endif
#else
#ifdef TEL_CONN_MGR_UT
#define TEL_CONN_MGR_LOG_INFO(x, ...) do {vTaskDelay(10); LOG_I(tel_conn_mgr, x, ##__VA_ARGS__);} while(0)
#define TEL_CONN_MGR_LOG_WARN(x, ...) do {vTaskDelay(10); LOG_W(tel_conn_mgr, x, ##__VA_ARGS__);} while(0)
#define TEL_CONN_MGR_LOG_ERR(x, ...) do {vTaskDelay(10); LOG_E(tel_conn_mgr, x, ##__VA_ARGS__);} while(0)
#else
#define TEL_CONN_MGR_LOG_INFO(x, ...) do {LOG_I(tel_conn_mgr, x, ##__VA_ARGS__);} while(0)
#define TEL_CONN_MGR_LOG_WARN(x, ...) do {LOG_W(tel_conn_mgr, x, ##__VA_ARGS__);} while(0)
#define TEL_CONN_MGR_LOG_ERR(x, ...) do {LOG_E(tel_conn_mgr, x, ##__VA_ARGS__);} while(0)
#endif
#endif

#else
#define TEL_CONN_MGR_LOG_INFO(x, ...)
#define TEL_CONN_MGR_LOG_WARN(x, ...)
#define TEL_CONN_MGR_LOG_ERR(x, ...)
#endif

#define tel_conn_mgr_calloc pvPortCalloc
#define tel_conn_mgr_free vPortFree

#define TEL_CONN_MAX_PORT_MAX_DELAY (portMAX_DELAY)

#define tel_conn_mgr_queue_create xQueueCreate
#define tel_conn_mgr_queue_delete vQueueDelete
#define tel_conn_mgr_queue_receive(a, b, c) (xQueueReceive((a), (b), (TickType_t)(c)))

#define tel_conn_mgr_task_create xTaskCreate
#define tel_conn_mgr_task_delete vTaskDelete

#define TEL_CONN_MGR_SLEEP_LOCK_INVALID_ID SLEEP_LOCK_INVALID_ID
#define TEL_CONN_MGR_SLEEP_MANAGER_OK HAL_SLEEP_MANAGER_OK

#define tel_conn_mgr_set_sleep_handle hal_sleep_manager_set_sleep_handle
#define tel_conn_mgr_lock_sleep(hdl) hal_sleep_manager_acquire_sleeplock((hdl), HAL_SLEEP_LOCK_DEEP)
#define tel_conn_mgr_unlock_sleep(hdl) hal_sleep_manager_release_sleeplock((hdl), HAL_SLEEP_LOCK_DEEP)

#define tel_conn_mgr_nvdm_write nvdm_write_data_item
#define tel_conn_mgr_nvdm_read nvdm_read_data_item

tel_conn_mgr_mutex_hdl_t tel_conn_mgr_mutex_create(void);

void tel_conn_mgr_mutex_free(tel_conn_mgr_mutex_hdl_t handle);

void tel_conn_mgr_mutex_lock(tel_conn_mgr_mutex_hdl_t handle);

void tel_conn_mgr_mutex_unlock(tel_conn_mgr_mutex_hdl_t handle);

tel_conn_mgr_bool tel_conn_mgr_send_msg(tel_conn_mgr_queue_hdl_t queue_hdl, void *msg);

#ifndef USE_SYSLOG
void tel_conn_mgr_log_mutex_lock(char *caller);

void tel_conn_mgr_log_mutex_unlock(char *caller);
#endif

#endif /* __TEL_CONN_MGR_PLATFORM_H__ */

