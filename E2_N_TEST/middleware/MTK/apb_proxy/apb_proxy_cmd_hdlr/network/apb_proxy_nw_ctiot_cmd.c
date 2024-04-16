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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "syslog.h"

#if defined(MTK_CTIOT_SUPPORT)

#include "apb_proxy.h"
#include "apb_proxy_nw_ctiot_cmd.h"
#include "apb_proxy_nw_cmd_gprot.h"
#include "sockets.h"
#include "liblwm2m.h"
#include "lwm2m_app_context_manager.h"
#include "lwm2m_objects.h"
#include "memory_attribute.h"
#include "hal_rtc_external.h"
#include "hal_rtc_internal.h"
#include "nvdm.h"
#include "ril.h"
#include "ril_cmds_def.h"
#include "timers.h"
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_platform.h"

log_create_module(ctiot_at, PRINT_LEVEL_INFO);

#if !defined(MTK_DEBUG_LEVEL_NONE)
#define CTIOT_AT_LOGI(fmt, args...)     LOG_I(ctiot_at, "[CTIOT_AT] "fmt, ##args)
#define CTIOT_AT_LOGW(fmt, args...)     LOG_W(ctiot_at, "[CTIOT_AT] "fmt, ##args)
#define CTIOT_AT_LOGE(fmt, args...)     LOG_E(ctiot_at, "[CTIOT_AT] "fmt, ##args)
#else
#define CTIOT_AT_LOGI(fmt, args...)
#define CTIOT_AT_LOGW(fmt, args...)
#define CTIOT_AT_LOGE(fmt, args...)
#endif

#define CTIOT_AT_TASK_NAME              "CTIOT_AT"
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
#define CTIOT_AT_TASK_STACK_SIZE        (1024 * 6)
#else
#define CTIOT_AT_TASK_STACK_SIZE        (1024 * 4)
#endif
#define CTIOT_AT_TASK_PRIORITY          (TASK_PRIORITY_NORMAL)

#define CTIOT_AT_CMD_PARAM_NUM          (10)
#define CTIOT_AT_RESPONSE_DATA_LEN      (50)
#define CTIOT_AT_MIN_LIFETIME           (20)

#define CTIOT_AT_MAX_SERVER_IP_LEN      (64)
#define CTIOT_AT_MAX_ENDPOINT_NAME_LEN  (32)
#define CTIOT_AT_MAX_PSKID_LEN          (32)
#if defined(WITH_MBEDTLS)
#define CTIOT_AT_MAX_PSK_LEN            (MBEDTLS_PSK_MAX_LEN * 2)
#else
#define CTIOT_AT_MAX_PSK_LEN            (32)
#endif
#define CTIOT_AT_MAX_OBJECT_COUNT       (6)
#define CTIOT_AT_MAX_DATA_BUFFER_SIZE   (1024)

#define CTIOT_AT_SERVER_ID              (100)

#define CTIOT_AT_ERRID_SUCCESS                      (0)
#define CTIOT_AT_ERRID_OTHER_ERROR                  (1)
#define CTIOT_AT_ERRID_PARAMETER_NUMBER_ERROR       (2)
#define CTIOT_AT_ERRID_PARAMETER_VALUE_ERROR        (3)
#define CTIOT_AT_ERRID_NOT_REGISTER_ERROR           (4)
#define CTIOT_AT_ERRID_DISABLE_ERROR                (7)
#define CTIOT_AT_ERRID_DATA_LENGTH_ODD_ERROR        (13)
#define CTIOT_AT_ERRID_DATA_LENGTH_OVERFLOW_ERROR   (14)
#define CTIOT_AT_ERRID_NOT_READY_RECEIVE_ERROR      (15)
#define CTIOT_AT_ERRID_KEEP_CONNECTING_ERROR        (32)
#define CTIOT_AT_ERRID_ALREADY_REGISTERED_ERROR     (33)
#define CTIOT_AT_ERRID_CREATE_LWM2M_ERROR           (34)

typedef enum {
    CTIOT_AT_REGSTATE_NOT_REGISTER = 0,
    CTIOT_AT_REGSTATE_REGISTERING = 1,
    CTIOT_AT_REGSTATE_REGISTERED = 2,
    CTIOT_AT_REGSTATE_DEREGISTERING = 3
} ctiot_at_regstate_t;

typedef struct {
    uint32_t rtc_handle;
} ctiot_at_rentention_t;

typedef struct {
    client_data_t client_data;
    lwm2m_context_t *lwm2m_handle;
    char server_ip[CTIOT_AT_MAX_SERVER_IP_LEN + 1];
    uint32_t port;
    char endpoint_name[CTIOT_AT_MAX_ENDPOINT_NAME_LEN + 1];
    uint32_t lifetime;
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    char pskid[CTIOT_AT_MAX_PSKID_LEN + 1];
    char psk[CTIOT_AT_MAX_PSK_LEN + 1];
#endif /* WITH_TINYDTLS */
    bool is_task_running;
    bool is_need_update;
    bool is_need_deep_sleep_handling;
    TimerHandle_t ping_timer;
    uint8_t sleep_handle;
    bool is_locking_sleep;
    ctiot_at_regstate_t regstate;
    bool observed;
    bool fota_update;
    ctiot_at_callback user_callback;
    void *user_data;
    ctiot_at_rentention_t *rentention;
} ctiot_at_context_t;

static ctiot_at_context_t g_ctiot_at_context;
static ATTR_ZIDATA_IN_RETSRAM ctiot_at_rentention_t g_ctiot_at_rentention;
static lwm2m_object_t *g_ctiot_at_objects[CTIOT_AT_MAX_OBJECT_COUNT];

/* NVDM Group Name */
#define CTIOT_AT_NVDM_GROUP_NAME            "ctiot_at"

/* NVDM Item Name */
#define CTIOT_AT_NVDM_ITEM_NAME_IS_USED     "is_used"
#define CTIOT_AT_NVDM_ITEM_NAME_SERVER_IP   "server_ip"
#define CTIOT_AT_NVDM_ITEM_NAME_PORT        "port"
#define CTIOT_AT_NVDM_ITEM_NAME_ENDPOINT    "endpoint"
#define CTIOT_AT_NVDM_ITEM_NAME_LIFETIME    "lifetime"
#define CTIOT_AT_NVDM_ITEM_NAME_PSKID       "pskid"
#define CTIOT_AT_NVDM_ITEM_NAME_PSK         "psk"
#define CTIOT_AT_NVDM_ITEM_NAME_REGSTATE    "regstate"
#define CTIOT_AT_NVDM_ITEM_NAME_OBSERVED    "observed"
#define CTIOT_AT_NVDM_ITEM_NAME_FOTA_U      "fotaU"

/* NVDM Item ID */
#define CTIOT_AT_NVDM_ITEM_ID_IS_USED       (0x1)
#define CTIOT_AT_NVDM_ITEM_ID_SERVER_IP     (0x2)
#define CTIOT_AT_NVDM_ITEM_ID_PORT          (0x4)
#define CTIOT_AT_NVDM_ITEM_ID_ENDPOINT      (0x8)
#define CTIOT_AT_NVDM_ITEM_ID_LIFETIME      (0x10)
#define CTIOT_AT_NVDM_ITEM_ID_PSKID         (0x20)
#define CTIOT_AT_NVDM_ITEM_ID_PSK           (0x40)
#define CTIOT_AT_NVDM_ITEM_ID_REGSTATE      (0x80)
#define CTIOT_AT_NVDM_ITEM_ID_OBSERVED      (0x100)
#define CTIOT_AT_NVDM_ITEM_ID_FOTA_UPDATE   (0x200)

static void ctiot_at_read_nvdm(uint32_t flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctiot_at_context_t *ctiot;
    uint32_t len;
    nvdm_status_t status;
    bool is_used = false;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot = &g_ctiot_at_context;

    // is_used
    len = sizeof(bool);
    status = nvdm_read_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_IS_USED, (uint8_t*)&is_used, &len);
    CTIOT_AT_LOGI("read nvdm [%s: %d] = %d", CTIOT_AT_NVDM_ITEM_NAME_IS_USED, is_used, status);
    if (!is_used) {
        return;
    }

    if ((flag & CTIOT_AT_NVDM_ITEM_ID_SERVER_IP) == CTIOT_AT_NVDM_ITEM_ID_SERVER_IP) {
        // server_ip
        len = CTIOT_AT_MAX_SERVER_IP_LEN;
        status = nvdm_read_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_SERVER_IP, (uint8_t*)ctiot->server_ip, &len);
        CTIOT_AT_LOGI("read nvdm [%s: %s] = %d", CTIOT_AT_NVDM_ITEM_NAME_SERVER_IP, ctiot->server_ip, status);
    }

    if ((flag & CTIOT_AT_NVDM_ITEM_ID_PORT) == CTIOT_AT_NVDM_ITEM_ID_PORT) {
        // port
        len = sizeof(uint32_t);
        status = nvdm_read_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_PORT, (uint8_t*)&ctiot->port, &len);
        CTIOT_AT_LOGI("read nvdm [%s: %d] = %d", CTIOT_AT_NVDM_ITEM_NAME_PORT, ctiot->port, status);
    }

    if ((flag & CTIOT_AT_NVDM_ITEM_ID_ENDPOINT) == CTIOT_AT_NVDM_ITEM_ID_ENDPOINT) {
        // endpoint
        len = CTIOT_AT_MAX_ENDPOINT_NAME_LEN;
        status = nvdm_read_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_ENDPOINT, (uint8_t*)ctiot->endpoint_name, &len);
        CTIOT_AT_LOGI("read nvdm [%s: %s] = %d", CTIOT_AT_NVDM_ITEM_NAME_ENDPOINT, ctiot->endpoint_name, status);
    }

    if ((flag & CTIOT_AT_NVDM_ITEM_ID_LIFETIME) == CTIOT_AT_NVDM_ITEM_ID_LIFETIME) {
        // lifetime
        len = sizeof(uint32_t);
        status = nvdm_read_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_LIFETIME, (uint8_t*)&ctiot->lifetime, &len);
        CTIOT_AT_LOGI("read nvdm [%s: %d] = %d", CTIOT_AT_NVDM_ITEM_NAME_LIFETIME, ctiot->lifetime, status);
    }

#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    if ((flag & CTIOT_AT_NVDM_ITEM_ID_PSKID) == CTIOT_AT_NVDM_ITEM_ID_PSKID) {
        // pskid
        len = CTIOT_AT_MAX_PSKID_LEN;
        status = nvdm_read_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_PSKID, (uint8_t*)ctiot->pskid, &len);
        CTIOT_AT_LOGI("read nvdm [%s: %s] = %d", CTIOT_AT_NVDM_ITEM_NAME_PSKID, ctiot->pskid, status);
    }

    if ((flag & CTIOT_AT_NVDM_ITEM_ID_PSK) == CTIOT_AT_NVDM_ITEM_ID_PSK) {
        // psk
        len = CTIOT_AT_MAX_PSK_LEN;
        status = nvdm_read_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_PSK, (uint8_t*)ctiot->psk, &len);
        CTIOT_AT_LOGI("read nvdm [%s: %s] = %d", CTIOT_AT_NVDM_ITEM_NAME_PSK, ctiot->psk, status);
    }
#endif /* WITH_TINYDTLS */

    if ((flag & CTIOT_AT_NVDM_ITEM_ID_REGSTATE) == CTIOT_AT_NVDM_ITEM_ID_REGSTATE) {
        // regstate
        len = sizeof(ctiot_at_regstate_t);
        status = nvdm_read_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_REGSTATE, (uint8_t*)&ctiot->regstate, &len);
        CTIOT_AT_LOGI("read nvdm [%s: %d] = %d", CTIOT_AT_NVDM_ITEM_NAME_REGSTATE, ctiot->regstate, status);
    }

    if ((flag & CTIOT_AT_NVDM_ITEM_ID_OBSERVED) == CTIOT_AT_NVDM_ITEM_ID_OBSERVED) {
        // observed
        len = sizeof(bool);
        status = nvdm_read_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_OBSERVED, (uint8_t*)&ctiot->observed, &len);
        CTIOT_AT_LOGI("read nvdm [%s: %d] = %d", CTIOT_AT_NVDM_ITEM_NAME_OBSERVED, ctiot->observed, status);
    }

    if ((flag & CTIOT_AT_NVDM_ITEM_ID_FOTA_UPDATE) == CTIOT_AT_NVDM_ITEM_ID_FOTA_UPDATE) {
        // fota_update
        len = sizeof(bool);
        status = nvdm_read_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_FOTA_U, (uint8_t*)&ctiot->fota_update, &len);
        CTIOT_AT_LOGI("read nvdm [%s: %d] = %d", CTIOT_AT_NVDM_ITEM_NAME_FOTA_U, ctiot->fota_update, status);
    }
}

static void ctiot_at_write_nvdm(uint32_t flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctiot_at_context_t *ctiot;
    uint32_t len;
    nvdm_status_t status;
    bool is_used = true;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot = &g_ctiot_at_context;

    if ((flag & CTIOT_AT_NVDM_ITEM_ID_IS_USED) == CTIOT_AT_NVDM_ITEM_ID_IS_USED) {
        // is_used
        len = sizeof(bool);
        status = nvdm_write_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_IS_USED, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&is_used, len);
        CTIOT_AT_LOGI("write nvdm [%s: %d] = %d", CTIOT_AT_NVDM_ITEM_NAME_IS_USED, is_used, status);
    }

    if ((flag & CTIOT_AT_NVDM_ITEM_ID_SERVER_IP) == CTIOT_AT_NVDM_ITEM_ID_SERVER_IP) {
        // server_ip
        len = CTIOT_AT_MAX_SERVER_IP_LEN;
        status = nvdm_write_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_SERVER_IP, NVDM_DATA_ITEM_TYPE_STRING, (uint8_t*)ctiot->server_ip, len);
        CTIOT_AT_LOGI("write nvdm [%s: %s] = %d", CTIOT_AT_NVDM_ITEM_NAME_SERVER_IP, ctiot->server_ip, status);
    }

    if ((flag & CTIOT_AT_NVDM_ITEM_ID_PORT) == CTIOT_AT_NVDM_ITEM_ID_PORT) {
        // port
        len = sizeof(uint32_t);
        status = nvdm_write_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_PORT, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&ctiot->port, len);
        CTIOT_AT_LOGI("write nvdm [%s: %d] = %d", CTIOT_AT_NVDM_ITEM_NAME_PORT, ctiot->port, status);
    }

    if ((flag & CTIOT_AT_NVDM_ITEM_ID_ENDPOINT) == CTIOT_AT_NVDM_ITEM_ID_ENDPOINT) {
        // endpoint
        len = CTIOT_AT_MAX_ENDPOINT_NAME_LEN;
        status = nvdm_write_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_ENDPOINT, NVDM_DATA_ITEM_TYPE_STRING, (uint8_t*)ctiot->endpoint_name, len);
        CTIOT_AT_LOGI("write nvdm [%s: %s] = %d", CTIOT_AT_NVDM_ITEM_NAME_ENDPOINT, ctiot->endpoint_name, status);
    }

    if ((flag & CTIOT_AT_NVDM_ITEM_ID_LIFETIME) == CTIOT_AT_NVDM_ITEM_ID_LIFETIME) {
        // lifetime
        len = sizeof(uint32_t);
        status = nvdm_write_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_LIFETIME, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&ctiot->lifetime, len);
        CTIOT_AT_LOGI("write nvdm [%s: %d] = %d", CTIOT_AT_NVDM_ITEM_NAME_LIFETIME, ctiot->lifetime, status);
    }

#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    if ((flag & CTIOT_AT_NVDM_ITEM_ID_PSKID) == CTIOT_AT_NVDM_ITEM_ID_PSKID) {
        // pskid
        len = CTIOT_AT_MAX_PSKID_LEN;
        status = nvdm_write_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_PSKID, NVDM_DATA_ITEM_TYPE_STRING, (uint8_t*)ctiot->pskid, len);
        CTIOT_AT_LOGI("write nvdm [%s: %s] = %d", CTIOT_AT_NVDM_ITEM_NAME_PSKID, ctiot->pskid, status);
    }

    if ((flag & CTIOT_AT_NVDM_ITEM_ID_PSK) == CTIOT_AT_NVDM_ITEM_ID_PSK) {
        // psk
        len = CTIOT_AT_MAX_PSK_LEN;
        status = nvdm_write_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_PSK, NVDM_DATA_ITEM_TYPE_STRING, (uint8_t*)ctiot->psk, len);
        CTIOT_AT_LOGI("write nvdm [%s: %s] = %d", CTIOT_AT_NVDM_ITEM_NAME_PSK, ctiot->psk, status);
    }
#endif /* WITH_TINYDTLS */

    if ((flag & CTIOT_AT_NVDM_ITEM_ID_REGSTATE) == CTIOT_AT_NVDM_ITEM_ID_REGSTATE) {
        // regstate
        len = sizeof(ctiot_at_regstate_t);
        status = nvdm_write_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_REGSTATE, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&ctiot->regstate, len);
        CTIOT_AT_LOGI("write nvdm [%s: %d] = %d", CTIOT_AT_NVDM_ITEM_NAME_REGSTATE, ctiot->regstate, status);
    }

    if ((flag & CTIOT_AT_NVDM_ITEM_ID_OBSERVED) == CTIOT_AT_NVDM_ITEM_ID_OBSERVED) {
        // observed
        len = sizeof(bool);
        status = nvdm_write_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_OBSERVED, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&ctiot->observed, len);
        CTIOT_AT_LOGI("write nvdm [%s: %d] = %d", CTIOT_AT_NVDM_ITEM_NAME_OBSERVED, ctiot->observed, status);
    }

    if ((flag & CTIOT_AT_NVDM_ITEM_ID_FOTA_UPDATE) == CTIOT_AT_NVDM_ITEM_ID_FOTA_UPDATE) {
        // fota_update
        len = sizeof(bool);
        status = nvdm_write_data_item(CTIOT_AT_NVDM_GROUP_NAME, CTIOT_AT_NVDM_ITEM_NAME_FOTA_U, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&ctiot->fota_update, len);
        CTIOT_AT_LOGI("write nvdm [%s: %d] = %d", CTIOT_AT_NVDM_ITEM_NAME_FOTA_U, ctiot->fota_update, status);
    }
}

static void ctiot_at_restore_notify(bool result);

static void ctiot_at_rtc_timer_callback(void *user_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctiot_at_context_t *ctiot;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot = &g_ctiot_at_context;
    if (ctiot->regstate == CTIOT_AT_REGSTATE_REGISTERED) {
        ctiot->is_need_update = true;
        if (!ctiot->is_locking_sleep) {
            xTimerStartFromISR(ctiot->ping_timer, &xHigherPriorityTaskWoken);
            ctiot->is_locking_sleep = true;
            hal_sleep_manager_acquire_sleeplock(ctiot->sleep_handle, HAL_SLEEP_LOCK_DEEP);
        }
    }
}

static void ctiot_at_start_rtc_timer(ctiot_at_context_t *ctiot)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    rtc_sw_timer_status_t status;
    uint32_t lifetime;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (ctiot->lifetime <= 30) {
        lifetime = ctiot->lifetime / 2; /* lifetime 30s -> update 15s */
    } else if (ctiot->lifetime <= 50) {
        lifetime = 15 + (ctiot->lifetime - 30) * 3 / 4; /* lifetime 50s -> update 30s */
    } else if (ctiot->lifetime <= 100) {
        lifetime = 30 + (ctiot->lifetime - 50) * 4 / 5; /* lifetime 100s -> update 70s */
    } else if (ctiot->lifetime <= 300) {
        lifetime = 70 + (ctiot->lifetime - 100) * 9 / 10; /* lifetime 300s -> update 250s */
    } else {
        lifetime = 250 + (ctiot->lifetime - 300) * 19 / 20;
    }

    status = rtc_sw_timer_create(&ctiot->rentention->rtc_handle, lifetime * 10, true, ctiot_at_rtc_timer_callback);
    CTIOT_AT_LOGI("rtc_sw_timer_create = %d, time_out_100ms = %d", (int)status, (int)(lifetime * 10));
    status = rtc_sw_timer_start(ctiot->rentention->rtc_handle);
    CTIOT_AT_LOGI("rtc_sw_timer_start = %d", (int)status);
}

static void ctiot_at_stop_rtc_timer(ctiot_at_context_t *ctiot)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    rtc_sw_timer_status_t status;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    status = rtc_sw_timer_stop(ctiot->rentention->rtc_handle);
    CTIOT_AT_LOGI("rtc_sw_timer_stop = %d", (int)status);
    status = rtc_sw_timer_delete(ctiot->rentention->rtc_handle);
    CTIOT_AT_LOGI("rtc_sw_timer_delete = %d", (int)status);
}

static int32_t ctiot_at_pdn_context_notify_callback(ril_urc_id_t event_id, void *param, uint32_t param_len);
static int32_t ctiot_at_register_internal(bool need_read_nvdm);

static void ctiot_at_ping_timeout_callback(TimerHandle_t xTimer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctiot_at_context_t *ctiot;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot = &g_ctiot_at_context;
    if (ctiot->is_locking_sleep) {
        ctiot->is_locking_sleep = false;
        hal_sleep_manager_release_sleeplock(ctiot->sleep_handle, HAL_SLEEP_LOCK_DEEP);
    }
}

static void ctiot_at_task_processing(void *arg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctiot_at_context_t *ctiot;
    lwm2m_context_t *lwm2mH;
    client_data_t *data;
    int result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot = &g_ctiot_at_context;

    if (ctiot->is_need_deep_sleep_handling) {
        ctiot->is_need_deep_sleep_handling = false;
        // register with ready state
        configASSERT(ctiot_at_register_internal(true) == CTIOT_AT_ERRID_SUCCESS);
        lwm2mH = ctiot->lwm2m_handle;
        configASSERT(lwm2mH != NULL);
        lwm2mH->ext_reg_update = 1;
    } else if (ctiot_at_is_restoring()) {
        // register with ready state
        configASSERT(ctiot_at_register_internal(true) == CTIOT_AT_ERRID_SUCCESS);
        ctiot_at_restore_notify(true);
        lwm2mH = ctiot->lwm2m_handle;
        configASSERT(lwm2mH != NULL);
        lwm2mH->ext_reg_update = 1;
        ctiot_at_start_rtc_timer(ctiot);
        ctiot->is_need_update = true;
    } else {
        lwm2mH = ctiot->lwm2m_handle;
        configASSERT(lwm2mH != NULL);
        lwm2mH->ext_reg_update = 1;
        ctiot_at_start_rtc_timer(ctiot);
        ctiot->regstate = CTIOT_AT_REGSTATE_REGISTERING;
        ctiot_at_write_nvdm(CTIOT_AT_NVDM_ITEM_ID_REGSTATE);
    }

    data = (client_data_t*)lwm2mH->userData;
    configASSERT(data != NULL);

    while (ctiot->is_task_running) {
        struct timeval tv;
        fd_set readfds;

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        if (ctiot->is_need_update) {
            ctiot->is_need_update = false;
            lwm2m_update_registration(lwm2mH, 0, false);
        }

        /*
         * This function does two things:
         *  - first it does the work needed by liblwm2m (eg. (re)sending some packets).
         *  - Secondly it adjusts the timeout value (default 60s) depending on the state of the transaction
         *    (eg. retransmission) and the time between the next operation
         */
        result = lwm2m_step(lwm2mH, &(tv.tv_sec));
        if (result != COAP_NO_ERROR) {
            CTIOT_AT_LOGE("lwm2m_step() failed: 0x%X", result);
            lwm2mH->notify_callback(LWM2M_NOTIFY_TYPE_LWSTATUS, LWM2M_NOTIFY_CODE_SUCCESS, 0);
        }

#ifdef WITH_MBEDTLS
        mbedtls_connection_t *connP = data->connList;
        if (connP != NULL && connP->dtlsSession != 0) {
            int result = connection_handle_packet(connP);
            if (result < 0) {
                CTIOT_AT_LOGE("error handling message %d", result);
            }
        } else
#endif /* WITH_MBEDTLS */
        {
        FD_ZERO(&readfds);
        FD_SET(data->sock, &readfds);

        /*
         * This part will set up an interruption until an event happen on SDTIN or the socket until "tv" timed out (set
         * with the precedent function)
         */
        result = select(FD_SETSIZE, &readfds, NULL, NULL, &tv);
        if (result < 0) {
            if (errno != EINTR) {
                CTIOT_AT_LOGE("Error in select(): %d %s", errno, strerror(errno));
            }
        } else if (result > 0) {
#define MAX_PACKET_SIZE 1024
            uint8_t buffer[MAX_PACKET_SIZE];
            int numBytes;

            /*
             * If an event happens on the socket
             */
            if (FD_ISSET(data->sock, &readfds)) {
                struct sockaddr_storage addr;
                socklen_t addrLen;

                addrLen = sizeof(addr);

                /*
                 * We retrieve the data received
                 */
                numBytes = recvfrom(data->sock, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr*)&addr, &addrLen);

                if (numBytes < 0) {
                    CTIOT_AT_LOGE("Error in recvfrom(): %d %s", errno, strerror(errno));
                } else if (numBytes > 0) {
                    char s[INET6_ADDRSTRLEN];
#if defined(WITH_MBEDTLS)
                    mbedtls_connection_t *connP;
#elif defined(WITH_TINYDTLS)
                    dtls_connection_t *connP;
#else
                    connection_t *connP;
#endif
                    if (AF_INET == addr.ss_family) {
                        struct sockaddr_in *saddr = (struct sockaddr_in*)&addr;
                        inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET6_ADDRSTRLEN);
                    } else if (AF_INET6 == addr.ss_family) {
                        struct sockaddr_in6 *saddr = (struct sockaddr_in6*)&addr;
                        inet_ntop(saddr->sin6_family, &saddr->sin6_addr, s, INET6_ADDRSTRLEN);
                    }

                    CTIOT_AT_LOGI("%d bytes received from [%s]", numBytes, s);

                    connP = connection_find(data->connList, &addr, addrLen);
                    if (connP != NULL) {
                        /*
                         * Let liblwm2m respond to the query depending on the context
                         */
#ifdef WITH_TINYDTLS
                        int result = connection_handle_packet(connP, buffer, numBytes);
                        if (result != 0) {
                            CTIOT_AT_LOGE("error handling message %d", result);
                        }
#else
                        lwm2m_handle_packet(lwm2mH, buffer, numBytes, connP);
#endif
                    } else {
                        CTIOT_AT_LOGE("no connection found");
                    }
                }
            }
        }
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }

    ctiot_at_stop_rtc_timer(ctiot);

    lwm2m_close(lwm2mH);
    ctiot->regstate = CTIOT_AT_REGSTATE_NOT_REGISTER;
    ctiot->observed = false;
    ctiot_at_write_nvdm(CTIOT_AT_NVDM_ITEM_ID_REGSTATE | CTIOT_AT_NVDM_ITEM_ID_OBSERVED);
    close(data->sock);
    connection_free(data->connList);

    clean_security_object(g_ctiot_at_objects[0]);
    lwm2m_free(g_ctiot_at_objects[0]);
    clean_server_object(g_ctiot_at_objects[1]);
    lwm2m_free(g_ctiot_at_objects[1]);
    free_object_device(g_ctiot_at_objects[2]);
    free_object_conn_m(g_ctiot_at_objects[3]);
    free_object_firmware(g_ctiot_at_objects[4]);
    free_object_watermeter(g_ctiot_at_objects[5]);

    vTaskDelete(NULL);
}

void ctiot_at_backup(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctiot_at_context_t *ctiot;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot = &g_ctiot_at_context;
    ctiot->fota_update = true;
    ctiot_at_write_nvdm(CTIOT_AT_NVDM_ITEM_ID_FOTA_UPDATE);
}

bool ctiot_at_restore(ctiot_at_callback user_callback, void *user_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctiot_at_context_t *ctiot;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot = &g_ctiot_at_context;
    ctiot->user_callback = user_callback;
    ctiot->user_data = user_data;

    CTIOT_AT_LOGI("user_callback = 0x%x, user_data = 0x%x", (unsigned int)user_callback, (unsigned int)user_data);

    if (ctiot->regstate == CTIOT_AT_REGSTATE_REGISTERED) {
        ctiot_at_restore_notify(true);
    }

    return true;
}

static void ctiot_at_restore_notify(bool result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctiot_at_context_t *ctiot;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot = &g_ctiot_at_context;
    ctiot->fota_update = false;
    ctiot_at_write_nvdm(CTIOT_AT_NVDM_ITEM_ID_FOTA_UPDATE);
    if (ctiot->user_callback != NULL) {
        ctiot->user_callback(result, ctiot->user_data);
        ctiot->user_callback = NULL;
    }
}

bool ctiot_at_is_restoring(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctiot_at_context_t *ctiot;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot = &g_ctiot_at_context;
    return ctiot->fota_update;
}

void ctiot_at_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctiot_at_context_t *ctiot;
    ril_status_t status;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot = &g_ctiot_at_context;
    ctiot->rentention = &g_ctiot_at_rentention;
    ctiot->ping_timer = xTimerCreate("ctiot_ping", 1000 * COAP_MAX_TRANSMIT_WAIT / portTICK_PERIOD_MS, pdFALSE, NULL, ctiot_at_ping_timeout_callback);
    configASSERT(ctiot->ping_timer != NULL);

    ctiot->sleep_handle = hal_sleep_manager_set_sleep_handle("ctiot_at");
    configASSERT(ctiot->sleep_handle != SLEEP_LOCK_INVALID_ID);

    status = ril_register_event_callback(RIL_ALL, ctiot_at_pdn_context_notify_callback);
    configASSERT(status == RIL_STATUS_SUCCESS);

    if (rtc_power_on_result_external() != DEEP_SLEEP && rtc_power_on_result_external() != DEEPER_SLEEP) {
        ctiot_at_read_nvdm(CTIOT_AT_NVDM_ITEM_ID_FOTA_UPDATE);
        CTIOT_AT_LOGI("fota update handling, fota_update: %d", ctiot->fota_update);
        /* COLD-BOOT case: normal init */
        if (ctiot->fota_update == true) {
            ctiot_at_read_nvdm(CTIOT_AT_NVDM_ITEM_ID_REGSTATE | CTIOT_AT_NVDM_ITEM_ID_OBSERVED);
        } else {
            ctiot->regstate = CTIOT_AT_REGSTATE_NOT_REGISTER;
            ctiot->observed = false;
            ctiot_at_write_nvdm(CTIOT_AT_NVDM_ITEM_ID_REGSTATE | CTIOT_AT_NVDM_ITEM_ID_OBSERVED);
        }
    } else {
        /* DEEP-SLEEP case: data retention process */
        ctiot_at_read_nvdm(CTIOT_AT_NVDM_ITEM_ID_REGSTATE | CTIOT_AT_NVDM_ITEM_ID_OBSERVED);
        CTIOT_AT_LOGI("deep sleep handling, regstate: %d", ctiot->regstate);
        if (ctiot->regstate != CTIOT_AT_REGSTATE_NOT_REGISTER) {
            ctiot->is_need_deep_sleep_handling = true;
            if (!ctiot->is_task_running) {
                ctiot->is_task_running = true;
                xTaskCreate(ctiot_at_task_processing,
                            CTIOT_AT_TASK_NAME,
                            CTIOT_AT_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                            NULL,
                            CTIOT_AT_TASK_PRIORITY,
                            NULL);
                CTIOT_AT_LOGI("create ctiot task");
            }
        }
    }
}

static void ctiot_at_notify_callback(lwm2m_notify_type_t type, lwm2m_notify_code_t code, uint32_t option_info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[CTIOT_AT_RESPONSE_DATA_LEN];
    ctiot_at_context_t *ctiot;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CTIOT_AT_LOGI("type = %d, code = %d, option_info = %d", (int)type, (int)code, (int)option_info);

    ctiot = &g_ctiot_at_context;

    // +M2MCLI:notification
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;

    switch (type) {
        case LWM2M_NOTIFY_TYPE_CONNECT:
            if (code == LWM2M_NOTIFY_CODE_SUCCESS) {
                sprintf(response_data, "+M2MCLI:connect success");
            } else {
                sprintf(response_data, "+M2MCLI:connect failed");
                ctiot->is_task_running = false;
            }
            break;

        case LWM2M_NOTIFY_TYPE_REG:
            if (code == LWM2M_NOTIFY_CODE_SUCCESS) {
                sprintf(response_data, "+M2MCLI:register success");
                ctiot->regstate = CTIOT_AT_REGSTATE_REGISTERED;
                ctiot_at_write_nvdm(CTIOT_AT_NVDM_ITEM_ID_REGSTATE);
            } else {
                sprintf(response_data, "+M2MCLI:register failed");
                ctiot->is_task_running = false;
            }
            break;

        case LWM2M_NOTIFY_TYPE_PING:
            if (code == LWM2M_NOTIFY_CODE_SUCCESS) {
                sprintf(response_data, "+M2MCLI:register update success");
            } else {
                sprintf(response_data, "+M2MCLI:register update failed");
            }
            if (ctiot->is_locking_sleep) {
                xTimerStop(ctiot->ping_timer, 0);
                ctiot->is_locking_sleep = false;
                hal_sleep_manager_release_sleeplock(ctiot->sleep_handle, HAL_SLEEP_LOCK_DEEP);
            }
            break;

        case LWM2M_NOTIFY_TYPE_DEREG:
            if (code == LWM2M_NOTIFY_CODE_SUCCESS) {
                sprintf(response_data, "+M2MCLI:deregister success");
            } else {
                sprintf(response_data, "+M2MCLI:deregister failed");
            }
            ctiot->is_task_running = false;
            ctiot->regstate = CTIOT_AT_REGSTATE_DEREGISTERING;
            ctiot_at_write_nvdm(CTIOT_AT_NVDM_ITEM_ID_REGSTATE);
            break;

        case LWM2M_NOTIFY_TYPE_SEND:
            if (code == LWM2M_NOTIFY_CODE_SUCCESS) {
                sprintf(response_data, "+M2MCLI:notify success");
            } else {
                sprintf(response_data, "+M2MCLI:notify failed");
            }
            break;

        case LWM2M_NOTIFY_TYPE_LWSTATUS:
            if (option_info != LWM2M_NOTIFY_CODE_LWM2M_SESSION_INVALID) {
                return;
            }
            sprintf(response_data, "+M2MCLI:lwm2m session abnormal");
            ctiot->is_task_running = false;
            ctiot->regstate = CTIOT_AT_REGSTATE_DEREGISTERING;
            ctiot_at_write_nvdm(CTIOT_AT_NVDM_ITEM_ID_REGSTATE);
            break;

        case LWM2M_NOTIFY_TYPE_REGING:
            ctiot->regstate = CTIOT_AT_REGSTATE_REGISTERING;
            ctiot_at_write_nvdm(CTIOT_AT_NVDM_ITEM_ID_REGSTATE);
            return;

        case LWM2M_NOTIFY_TYPE_OBSERVE:
            if (code == LWM2M_NOTIFY_CODE_SUCCESS) {
                sprintf(response_data, "+M2MCLI:observe success");
                ctiot->observed = true;
            } else {
                sprintf(response_data, "+M2MCLI:observe canceled");
                ctiot->observed = false;
            }
            ctiot_at_write_nvdm(CTIOT_AT_NVDM_ITEM_ID_OBSERVED);
            break;

        case LWM2M_NOTIFY_TYPE_DEREGING:
            ctiot->regstate = CTIOT_AT_REGSTATE_DEREGISTERING;
            ctiot_at_write_nvdm(CTIOT_AT_NVDM_ITEM_ID_REGSTATE);
            return;

        default:
            return;
    }

    response.pdata = response_data;
    response.length = strlen(response.pdata);

    response.cmd_id = APB_PROXY_INVALID_CMD_ID;
    apb_proxy_send_at_cmd_result(&response);
}

static int32_t ctiot_at_pdn_context_notify_callback(ril_urc_id_t event_id, void *param, uint32_t param_len)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctiot_at_context_t *ctiot;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot = &g_ctiot_at_context;

    if (!param || !param_len) {
        return 0;
    }

    switch (event_id) {
        case RIL_URC_ID_CGEV: {
            ril_cgev_event_reporting_urc_t *cgev_urc = (ril_cgev_event_reporting_urc_t*)param;

            if (cgev_urc->response_type == RIL_URC_TYPE_ME_PDN_ACT && ctiot_at_is_restoring()) {
                if (!ctiot->is_task_running) {
                    ctiot->is_task_running = true;
                    xTaskCreate(ctiot_at_task_processing,
                                CTIOT_AT_TASK_NAME,
                                CTIOT_AT_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                                NULL,
                                CTIOT_AT_TASK_PRIORITY,
                                NULL);
                    CTIOT_AT_LOGI("create ctiot task");
                }
            }
            break;
        }

        default:
            break;
    }

    return 0;
}

static int32_t ctiot_at_register_internal(bool need_read_nvdm)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctiot_at_context_t *ctiot;
    lwm2m_context_t *lwm2mH;
    client_data_t *data;
    int32_t ctiot_error = CTIOT_AT_ERRID_SUCCESS;
    int result;
    char server_uri[80];
    char port[20];
    char endpoint_name[40];
    char *pskid = NULL;
    char *psk_buffer = NULL;
    uint16_t psk_len = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (need_read_nvdm == true) {
        ctiot_at_read_nvdm(
            CTIOT_AT_NVDM_ITEM_ID_SERVER_IP | CTIOT_AT_NVDM_ITEM_ID_PORT | CTIOT_AT_NVDM_ITEM_ID_ENDPOINT | CTIOT_AT_NVDM_ITEM_ID_LIFETIME |
            CTIOT_AT_NVDM_ITEM_ID_PSKID | CTIOT_AT_NVDM_ITEM_ID_PSK);
    }

    ctiot = &g_ctiot_at_context;

    data = &ctiot->client_data;
    memset(data, 0, sizeof(client_data_t));
    data->addressFamily = AF_INET;

    snprintf(server_uri, sizeof(server_uri), "coap://%s:%d", ctiot->server_ip, (int)ctiot->port);
    sprintf(port, "%d", (int)ctiot->port);
    snprintf(endpoint_name, sizeof(endpoint_name), "%s", ctiot->endpoint_name);

    data->sock = create_socket(port, data->addressFamily);
    if (data->sock < 0) {
        CTIOT_AT_LOGE("Failed to open socket: %d %s", errno, strerror(errno));
        return CTIOT_AT_ERRID_CREATE_LWM2M_ERROR;
    }

#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    if (ctiot->pskid[0] != '\0') {
        pskid = ctiot->pskid;
    }

    if (ctiot->psk[0] != '\0') {
        psk_len = strlen(ctiot->psk) / 2;
        psk_buffer = pvPortMalloc(psk_len);
        if (psk_buffer == NULL) {
            CTIOT_AT_LOGE("Failed to create PSK binary buffer");
            close(data->sock);
            return CTIOT_AT_ERRID_OTHER_ERROR;
        }

        // Hex string to binary
        char *h = ctiot->psk;
        char *b = psk_buffer;
        char xlate[] = "0123456789ABCDEF";

        for (; *h; h += 2, ++b) {
            if (h - ctiot->psk >= psk_len * 2) break;
            char *l = strchr(xlate, (char)toupper((unsigned char)*h));
            char *r = strchr(xlate, (char)toupper((unsigned char)*(h + 1)));

            if (!r || !l) {
                CTIOT_AT_LOGE("Failed to parse Pre-Shared Keys");
                vPortFree(psk_buffer);
                close(data->sock);
                return CTIOT_AT_ERRID_OTHER_ERROR;
            }

            *b = ((l - xlate) << 4) + (r - xlate);
        }
    }
#endif /* WITH_TINYDTLS */

    g_ctiot_at_objects[0] = get_security_object(CTIOT_AT_SERVER_ID, server_uri, pskid, psk_buffer, psk_len, false);
    if (g_ctiot_at_objects[0] == NULL) {
        CTIOT_AT_LOGE("Failed to create security object");
        close(data->sock);
        if (psk_buffer != NULL) {
            vPortFree(psk_buffer);
        }
        return CTIOT_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    data->securityObjP = g_ctiot_at_objects[0];

    /* should free psk_buffer here */
    if (psk_buffer != NULL) {
        vPortFree(psk_buffer);
    }

    g_ctiot_at_objects[1] = get_server_object(CTIOT_AT_SERVER_ID, "U", ctiot->lifetime, false);
    if (g_ctiot_at_objects[1] == NULL) {
        CTIOT_AT_LOGE("Failed to create server object");
        close(data->sock);
        return CTIOT_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    data->serverObject = g_ctiot_at_objects[1];

    g_ctiot_at_objects[2] = get_object_device();
    if (g_ctiot_at_objects[2] == NULL) {
        CTIOT_AT_LOGE("Failed to create device object");
        close(data->sock);
        return CTIOT_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    g_ctiot_at_objects[3] = get_object_conn_m();
    if (g_ctiot_at_objects[3] == NULL) {
        CTIOT_AT_LOGE("Failed to create connectivity monitoring object");
        close(data->sock);
        return CTIOT_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    g_ctiot_at_objects[4] = get_object_firmware();
    if (g_ctiot_at_objects[4] == NULL) {
        CTIOT_AT_LOGE("Failed to create firmware object");
        close(data->sock);
        return CTIOT_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    g_ctiot_at_objects[5] = get_object_watermeter();
    if (g_ctiot_at_objects[5] == NULL) {
        CTIOT_AT_LOGE("Failed to create object 19");
        close(data->sock);
        return CTIOT_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    lwm2mH = lwm2m_init(data);
    if (lwm2mH == NULL) {
        CTIOT_AT_LOGE("lwm2m_init() failed");
        close(data->sock);
        return CTIOT_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    /* notify firmware the lwm2m instance if firmware object supported */
    firmware_set_instance(lwm2mH);

    if (ctiot->regstate == CTIOT_AT_REGSTATE_REGISTERED) {
        lwm2mH->state = STATE_READY;
    }
    ctiot->lwm2m_handle = lwm2mH;
    data->lwm2mH = lwm2mH;

    lwm2mH->mode = CLIENT_MODE;
    lwm2mH->connect_server_callback = lwm2m_atcmd_connect_server;
    lwm2mH->close_connection_callback = lwm2m_atcmd_close_connection;
    lwm2mH->notify_callback = ctiot_at_notify_callback;

    result = lwm2m_configure(lwm2mH, endpoint_name, NULL, NULL, CTIOT_AT_MAX_OBJECT_COUNT, g_ctiot_at_objects);
    if (result != COAP_NO_ERROR) {
        CTIOT_AT_LOGE("lwm2m_configure() failed: 0x%X", result);
        lwm2m_close(lwm2mH);
        close(data->sock);
        return CTIOT_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    if (!ctiot->is_task_running) {
        ctiot->is_task_running = true;
        xTaskCreate(ctiot_at_task_processing,
                    CTIOT_AT_TASK_NAME,
                    CTIOT_AT_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                    NULL,
                    CTIOT_AT_TASK_PRIORITY,
                    NULL);
        CTIOT_AT_LOGI("create ctiot task");
    }

    return ctiot_error;
}

int32_t ctiot_at_register(char *server_ip, uint32_t port, char *endpoint_name, uint32_t lifetime, char *pskid, char *psk)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctiot_at_context_t *ctiot;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot = &g_ctiot_at_context;
    if (ctiot->regstate != CTIOT_AT_REGSTATE_NOT_REGISTER) {
        return CTIOT_AT_ERRID_ALREADY_REGISTERED_ERROR;
    } else {
        strcpy(ctiot->server_ip, server_ip);
        ctiot->port = port;
        strcpy(ctiot->endpoint_name, endpoint_name);
        ctiot->lifetime = lifetime;
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
        if (pskid != NULL) {
            strcpy(ctiot->pskid, pskid);
        } else {
            ctiot->pskid[0] = '\0';
        }
        if (psk != NULL) {
            strcpy(ctiot->psk, psk);
        } else {
            ctiot->psk[0] = '\0';
        }
#endif /* WITH_TINYDTLS */

        ctiot_at_write_nvdm(CTIOT_AT_NVDM_ITEM_ID_IS_USED |
            CTIOT_AT_NVDM_ITEM_ID_SERVER_IP | CTIOT_AT_NVDM_ITEM_ID_PORT | CTIOT_AT_NVDM_ITEM_ID_ENDPOINT | CTIOT_AT_NVDM_ITEM_ID_LIFETIME |
            CTIOT_AT_NVDM_ITEM_ID_PSKID | CTIOT_AT_NVDM_ITEM_ID_PSK);
        return ctiot_at_register_internal(false);
    }
}

int32_t ctiot_at_deregister(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctiot_at_context_t *ctiot;
    int32_t ctiot_error = CTIOT_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot = &g_ctiot_at_context;
    if (ctiot->regstate == CTIOT_AT_REGSTATE_NOT_REGISTER) {
        return CTIOT_AT_ERRID_NOT_REGISTER_ERROR;
    } else if (ctiot->regstate == CTIOT_AT_REGSTATE_REGISTERING) {
        ctiot->is_task_running = false;
        ctiot->regstate = CTIOT_AT_REGSTATE_DEREGISTERING;
        ctiot_at_write_nvdm(CTIOT_AT_NVDM_ITEM_ID_REGSTATE);
        return ctiot_error;
    }

    lwm2m_deregister(ctiot->lwm2m_handle);

    return ctiot_error;
}

static int32_t ctiot_at_send(char *send_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctiot_at_context_t *ctiot;
    lwm2m_context_t *lwm2mH;
    int32_t ctiot_error = CTIOT_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot = &g_ctiot_at_context;
    if (ctiot->regstate != CTIOT_AT_REGSTATE_REGISTERED) {
        return CTIOT_AT_ERRID_NOT_REGISTER_ERROR;
    } else if (!ctiot->observed) {
        return CTIOT_AT_ERRID_NOT_READY_RECEIVE_ERROR;
    }

    char *obj_uri_string = "/19/0/0";
    lwm2m_uri_t uri;
    lwm2m_stringToUri(obj_uri_string, strlen(obj_uri_string), &uri);

    uint8_t *config_bin;
    uint32_t config_len = strlen(send_data) / 2;
#ifdef USE_HAIWEI_RAWDATA_PLUGIN
    config_bin = (uint8_t *)pvPortMalloc(config_len + 1);
    config_len = onenet_at_hex_to_bin(config_bin, send_data, config_len);
#else /* USE_HAIWEI_RAWDATA_PLUGIN */
    /* 1 byte message_id, 2 bytes length */
    config_bin = (uint8_t *)pvPortMalloc(config_len + 4);
    config_bin[0] = 0;
    if (config_len <= 0xFF) {
        config_bin[1] = 0;
        config_bin[2] = config_len;
    } else {
        config_bin[1] = (config_len & 0xFF00) >> 8;
        config_bin[2] = (config_len & 0x00FF);
    }
    config_len = onenet_at_hex_to_bin(config_bin + 3, send_data, config_len);
    config_len += 3;
#endif /* USE_HAIWEI_RAWDATA_PLUGIN */

    ctiot = &g_ctiot_at_context;
    lwm2mH = ctiot->lwm2m_handle;
    if (lwm2mH == NULL) {
        vPortFree(config_bin);
        return CTIOT_AT_ERRID_OTHER_ERROR;
    }

    lwm2m_data_notify(lwm2mH, &uri, (char*)config_bin, config_len);
    vPortFree(config_bin);

    return ctiot_error;
}

void ctiot_at_receive(uint8_t *data, uint32_t data_len)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char *response_data;
    ctiot_at_context_t *ctiot;
    lwm2m_context_t *lwm2mH;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot = &g_ctiot_at_context;
    lwm2mH = ctiot->lwm2m_handle;
    if (lwm2mH == NULL) {
        return;
    }

    // +M2MCLIRECV:<Data>
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;

#ifdef USE_HAIWEI_RAWDATA_PLUGIN
    response_data = (char*)pvPortMalloc(CTIOT_AT_RESPONSE_DATA_LEN + 2 * data_len);
#else
    response_data = (char*)pvPortMalloc(CTIOT_AT_RESPONSE_DATA_LEN + data_len);
#endif
    if (response_data == NULL) {
        return;
    }

    uint32_t prefix_len = sprintf(response_data, "+M2MCLIRECV:");
#ifdef USE_HAIWEI_RAWDATA_PLUGIN
    onenet_at_bin_to_hex(response_data + prefix_len, data, 2 * data_len);
#else /* USE_HAIWEI_RAWDATA_PLUGIN */
    if (data_len > 3) {
        /* 1 byte message_id, 2 bytes length */
        memcpy(response_data + prefix_len, data + 3, data_len - 3);
        response_data[prefix_len + data_len - 3] = '\0';
    }
#endif /* USE_HAIWEI_RAWDATA_PLUGIN */

    response.pdata = response_data;
    response.length = strlen(response.pdata);

    response.cmd_id = APB_PROXY_INVALID_CMD_ID;
    apb_proxy_send_at_cmd_result(&response);

    vPortFree(response_data);
}

apb_proxy_status_t apb_proxy_hdlr_ctiot_register_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer = NULL;
    int32_t ctiot_error = CTIOT_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CTIOT_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char*)pvPortMalloc(parse_cmd->string_len + 1);
            if (param_buffer == NULL) {
                ctiot_error = CTIOT_AT_ERRID_OTHER_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    ctiot_error = CTIOT_AT_ERRID_PARAMETER_NUMBER_ERROR;
                } else {
                    char *param_list[CTIOT_AT_CMD_PARAM_NUM];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, CTIOT_AT_CMD_PARAM_NUM);

                    // AT+M2MCLINEW=<Sever_IP>,<Port>,<Endpoint_Name>,<lifetime>[,<pskid>,<psk>]
                    if (param_num < 4) {
                        ctiot_error = CTIOT_AT_ERRID_PARAMETER_NUMBER_ERROR;
                    } else {
                        char *server_ip = param_list[0];
                        uint32_t port = atoi(param_list[1]);
                        char *endpoint_name = param_list[2];
                        uint32_t lifetime = atoi(param_list[3]);
                        if (strlen(server_ip) > CTIOT_AT_MAX_SERVER_IP_LEN) {
                            ctiot_error = CTIOT_AT_ERRID_PARAMETER_VALUE_ERROR;
                        } else if (lifetime < CTIOT_AT_MIN_LIFETIME) {
                            ctiot_error = CTIOT_AT_ERRID_PARAMETER_VALUE_ERROR;
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
                        } else if (param_num == 6) {
                            char *pskid = param_list[4];
                            char *psk = param_list[5];
                            if (strlen(pskid) == 0 || strlen(pskid) > CTIOT_AT_MAX_PSKID_LEN) {
                                ctiot_error = CTIOT_AT_ERRID_PARAMETER_VALUE_ERROR;
                            } else if (strlen(psk) == 0 || strlen(psk) > CTIOT_AT_MAX_PSK_LEN) {
                                ctiot_error = CTIOT_AT_ERRID_PARAMETER_VALUE_ERROR;
                            } else {
                                ctiot_error = ctiot_at_register(server_ip, port, endpoint_name, lifetime, pskid, psk);
                            }
#endif /* WITH_TINYDTLS */
                        } else if (param_num == 4) {
                            ctiot_error = ctiot_at_register(server_ip, port, endpoint_name, lifetime, NULL, NULL);
                        } else {
                            ctiot_error = CTIOT_AT_ERRID_PARAMETER_NUMBER_ERROR;
                        }
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }
            break;

        default:
            ctiot_error = CTIOT_AT_ERRID_OTHER_ERROR;
            break;
    }

    if (ctiot_error == CTIOT_AT_ERRID_SUCCESS) {
        response.result_code = APB_PROXY_RESULT_OK;
    } else {
        response.result_code = APB_PROXY_RESULT_ERROR;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_ctiot_deregister_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    int32_t ctiot_error = CTIOT_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CTIOT_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_ACTIVE:
            // AT+M2MCLIDEL
            ctiot_error = ctiot_at_deregister();
            break;

        default:
            ctiot_error = CTIOT_AT_ERRID_OTHER_ERROR;
            break;
    }

    if (ctiot_error == CTIOT_AT_ERRID_SUCCESS) {
        response.result_code = APB_PROXY_RESULT_OK;
    } else {
        response.result_code = APB_PROXY_RESULT_ERROR;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_ctiot_send_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer = NULL;
    ctiot_at_context_t *ctiot;
    lwm2m_context_t *lwm2mH;
    int32_t ctiot_error = CTIOT_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CTIOT_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char*)pvPortMalloc(parse_cmd->string_len + 1);
            if (param_buffer == NULL) {
                ctiot_error = CTIOT_AT_ERRID_OTHER_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    ctiot_error = CTIOT_AT_ERRID_PARAMETER_NUMBER_ERROR;
                } else {
                    char *param_list[CTIOT_AT_CMD_PARAM_NUM];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, CTIOT_AT_CMD_PARAM_NUM);

                    // AT+M2MCLISEND=<Data>
                    if (param_num == 1) {
                        char *send_data = param_list[0];
                        if (strlen(send_data) % 2 != 0) {
                            ctiot_error = CTIOT_AT_ERRID_DATA_LENGTH_ODD_ERROR;
                        } else if (strlen(send_data) > CTIOT_AT_MAX_DATA_BUFFER_SIZE) {
                            ctiot_error = CTIOT_AT_ERRID_DATA_LENGTH_OVERFLOW_ERROR;
                        } else {
                            ctiot_error = ctiot_at_send(send_data);
                        }
                    } else {
                        ctiot_error = CTIOT_AT_ERRID_PARAMETER_NUMBER_ERROR;
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }
            break;

        default:
            ctiot_error = CTIOT_AT_ERRID_OTHER_ERROR;
            break;
    }

    if (ctiot_error == CTIOT_AT_ERRID_SUCCESS) {
        response.result_code = APB_PROXY_RESULT_OK;
    } else {
        response.result_code = APB_PROXY_RESULT_ERROR;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    if (ctiot_error == CTIOT_AT_ERRID_SUCCESS) {
        ctiot = &g_ctiot_at_context;
        lwm2mH = ctiot->lwm2m_handle;
        configASSERT(lwm2mH != NULL);
        lwm2m_step_quickly(lwm2mH);
    }

    return APB_PROXY_STATUS_OK;
}

#endif /* MTK_CTIOT_SUPPORT */

