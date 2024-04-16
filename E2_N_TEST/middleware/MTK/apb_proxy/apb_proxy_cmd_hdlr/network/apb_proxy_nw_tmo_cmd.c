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

#if defined(MTK_TMO_CERT_SUPPORT)

#include "apb_proxy.h"
#include "apb_proxy_nw_tmo_cmd.h"
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
#include "hal_wdt.h"

#if defined(WITH_MBEDTLS)
#include "mbedtls/ssl.h"
#include "mbedtls/sha256.h"
#define AUTO_PSK_GENERATOR
#endif

#define AUTO_UPDATE_REGISTRATION

log_create_module(tmo_at, PRINT_LEVEL_INFO);

#if !defined(MTK_DEBUG_LEVEL_NONE)
#define TMO_AT_LOGI(fmt, args...)     LOG_I(tmo_at, "[TMO_AT] "fmt, ##args)
#define TMO_AT_LOGW(fmt, args...)     LOG_W(tmo_at, "[TMO_AT] "fmt, ##args)
#define TMO_AT_LOGE(fmt, args...)     LOG_E(tmo_at, "[TMO_AT] "fmt, ##args)
#else
#define TMO_AT_LOGI(fmt, args...)
#define TMO_AT_LOGW(fmt, args...)
#define TMO_AT_LOGE(fmt, args...)
#endif

#define TMO_AT_TASK_NAME              "TMO_AT"
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
#define TMO_AT_TASK_STACK_SIZE        (1024 * 6)
#else
#define TMO_AT_TASK_STACK_SIZE        (1024 * 4)
#endif
#define TMO_AT_TASK_PRIORITY          (TASK_PRIORITY_NORMAL)

#define TMO_AT_CMD_PARAM_NUM          (10)
#define TMO_AT_RESPONSE_DATA_LEN      (50)
#define TMO_AT_MIN_LIFETIME           (20)

#define TMO_AT_MAX_SERVER_IP_LEN      (64)
#define TMO_AT_MAX_ENDPOINT_NAME_LEN  (32)
#define TMO_AT_MAX_PSKID_LEN          (32)
#if defined(WITH_MBEDTLS)
#define TMO_AT_MAX_PSK_LEN            (MBEDTLS_PSK_MAX_LEN * 2)
#else
#define TMO_AT_MAX_PSK_LEN            (32)
#endif
#define TMO_AT_MAX_IMEI_LEN           (16)
#define TMO_AT_MAX_OBJECT_COUNT       (7)

#define TMO_AT_INSTANCE_ID            (0)
#define TMO_AT_SERVER_ID              (100)
#define TMO_AT_BINDING                ("U")
#define TMO_AT_BOOTSTARP              (1)
#define TMO_AT_SERVER_IP              "bootp.iot.t-mobile.com"
#define TMO_AT_PORT                   (5584)
#define TMO_AT_ENDPOINT_NAME          "urn:imei:123456789067233"
#define TMO_AT_LIFETIME               (86400)
#define TMO_AT_PSKID                  "urn:imei:123456789067233"
#define TMO_AT_PSK                    "5453784478336172416B777443312D293F25226C30487B555C383F6247227E2E754A257C71435D74707D244F7C7A39412C4D492A235C4361423821676D7935505B395F4F253235466C5C6E3648312B326A31575562642E6F5D3074782D5E4279552F64647C64657A312B3273275C3D334D4974672577723E31266E4B5357634A"

#define TMO_AT_ERRID_SUCCESS                      (0)
#define TMO_AT_ERRID_OTHER_ERROR                  (1)
#define TMO_AT_ERRID_PARAMETER_NUMBER_ERROR       (2)
#define TMO_AT_ERRID_PARAMETER_VALUE_ERROR        (3)
#define TMO_AT_ERRID_NOT_REGISTER_ERROR           (4)
#define TMO_AT_ERRID_DISABLE_ERROR                (7)
#define TMO_AT_ERRID_DATA_LENGTH_ODD_ERROR        (13)
#define TMO_AT_ERRID_NOT_READY_RECEIVE_ERROR      (15)
#define TMO_AT_ERRID_KEEP_CONNECTING_ERROR        (32)
#define TMO_AT_ERRID_ALREADY_REGISTERED_ERROR     (33)
#define TMO_AT_ERRID_CREATE_LWM2M_ERROR           (34)

typedef enum {
    TMO_AT_SECMODE_PRE_SHARED_KEY = 0,
    TMO_AT_SECMODE_RAW_PUBLIC_KEY = 1,
    TMO_AT_SECMODE_CERTIFICATE = 2,
    TMO_AT_SECMODE_NO_SECURITY = 3
} tmo_at_secmode_t;

typedef enum {
    TMO_AT_REGSTATE_NOT_REGISTER = 0,
    TMO_AT_REGSTATE_REGISTERING = 1,
    TMO_AT_REGSTATE_REGISTERED = 2,
    TMO_AT_REGSTATE_DEREGISTERING = 3
} tmo_at_regstate_t;

typedef enum {
    TMO_AT_BINDING_U = 0,
    TMO_AT_BINDING_UQ = 1,
    TMO_AT_BINDING_TOTAL
} tmo_at_binding_t;

typedef enum {
    TMO_AT_MODE_LIFETIME = 0,
    TMO_AT_MODE_BINDING = 1,
    TMO_AT_MODE_TOTAL
} tmo_at_mode_t;

typedef struct {
    uint32_t rtc_handle;
    bool disable;
} tmo_at_rentention_t;

typedef struct {
    client_data_t client_data;
    lwm2m_context_t *lwm2m_handle;
    SemaphoreHandle_t mutex_handle;
    bool bootstrap;
    char server_ip[TMO_AT_MAX_SERVER_IP_LEN + 1];
    uint32_t port;
    char endpoint_name[TMO_AT_MAX_ENDPOINT_NAME_LEN + 1];
    uint32_t lifetime;
    tmo_at_secmode_t secmode;
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    char pskid[TMO_AT_MAX_PSKID_LEN + 1];
    char psk[TMO_AT_MAX_PSK_LEN + 1];
#endif /* WITH_TINYDTLS */
    char imei[TMO_AT_MAX_IMEI_LEN + 1];
    bool is_task_running;
    bool is_need_update;
    bool is_need_register;
    bool is_need_deregister;
    bool is_need_reboot;
    bool is_need_deep_sleep_handling;
    bool is_need_update_registration_after_reboot;
    TimerHandle_t ping_timer;
    TimerHandle_t disable_timer;
    TimerHandle_t register_timer;
    uint8_t sleep_handle;
    bool is_locking_sleep;
    tmo_at_regstate_t regstate;
    uint32_t disable_timeout;
    uint32_t min_period;
    uint32_t max_period;
    uint16_t instance_id;
    uint16_t server_id;
    char binding[4];
    tmo_at_rentention_t *rentention;
} tmo_at_context_t;

static tmo_at_context_t g_tmo_at_context;
static ATTR_ZIDATA_IN_RETSRAM tmo_at_rentention_t g_tmo_at_rentention;
static lwm2m_object_t *g_tmo_at_objects[TMO_AT_MAX_OBJECT_COUNT];

/* NVDM Group Name */
#define TMO_AT_NVDM_GROUP_NAME            "tmo_at"

/* NVDM Item Name */
#define TMO_AT_NVDM_ITEM_NAME_IS_USED     "is_used"
#define TMO_AT_NVDM_ITEM_NAME_BOOTSTRAP   "bootstrap"
#define TMO_AT_NVDM_ITEM_NAME_SERVER_IP   "server_ip"
#define TMO_AT_NVDM_ITEM_NAME_PORT        "port"
#define TMO_AT_NVDM_ITEM_NAME_ENDPOINT    "endpoint"
#define TMO_AT_NVDM_ITEM_NAME_LIFETIME    "lifetime"
#define TMO_AT_NVDM_ITEM_NAME_SECMODE     "secmode"
#define TMO_AT_NVDM_ITEM_NAME_PSKID       "pskid"
#define TMO_AT_NVDM_ITEM_NAME_PSK         "psk"
#define TMO_AT_NVDM_ITEM_NAME_REGSTATE    "regstate"
#define TMO_AT_NVDM_ITEM_NAME_DISABLE_T   "disableT"
#define TMO_AT_NVDM_ITEM_NAME_MIN_P       "minP"
#define TMO_AT_NVDM_ITEM_NAME_MAX_P       "maxP"
#define TMO_AT_NVDM_ITEM_NAME_INSTANCE_ID "instance_id"
#define TMO_AT_NVDM_ITEM_NAME_SERVER_ID   "server_id"
#define TMO_AT_NVDM_ITEM_NAME_BINDING     "binding"

/* NVDM Item ID */
#define TMO_AT_NVDM_ITEM_ID_IS_USED       (0x1)
#define TMO_AT_NVDM_ITEM_ID_BOOTSTRAP     (0x2)
#define TMO_AT_NVDM_ITEM_ID_SERVER_IP     (0x4)
#define TMO_AT_NVDM_ITEM_ID_PORT          (0x8)
#define TMO_AT_NVDM_ITEM_ID_ENDPOINT      (0x10)
#define TMO_AT_NVDM_ITEM_ID_LIFETIME      (0x20)
#define TMO_AT_NVDM_ITEM_ID_SECMODE       (0x40)
#define TMO_AT_NVDM_ITEM_ID_PSKID         (0x80)
#define TMO_AT_NVDM_ITEM_ID_PSK           (0x100)
#define TMO_AT_NVDM_ITEM_ID_REGSTATE      (0x200)
#define TMO_AT_NVDM_ITEM_ID_DISABLE_T     (0x400)
#define TMO_AT_NVDM_ITEM_ID_MIN_PERIOD    (0x800)
#define TMO_AT_NVDM_ITEM_ID_MAX_PERIOD    (0x1000)
#define TMO_AT_NVDM_ITEM_ID_INSTANCE_ID   (0x2000)
#define TMO_AT_NVDM_ITEM_ID_SERVER_ID     (0x4000)
#define TMO_AT_NVDM_ITEM_ID_BINDING       (0x8000)

static void tmo_at_read_nvdm(uint32_t flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tmo_at_context_t *tmo;
    uint32_t len;
    nvdm_status_t status;
    bool is_used = false;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo = &g_tmo_at_context;

    // is_used
    len = sizeof(bool);
    status = nvdm_read_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_IS_USED, (uint8_t*)&is_used, &len);
    TMO_AT_LOGI("read nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_IS_USED, is_used, status);
    if (!is_used) {
        tmo->bootstrap = TMO_AT_BOOTSTARP;
        memcpy(tmo->server_ip, TMO_AT_SERVER_IP, TMO_AT_MAX_SERVER_IP_LEN);
        tmo->port = TMO_AT_PORT;
#if !defined(AUTO_PSK_GENERATOR)
        memcpy(tmo->endpoint_name, TMO_AT_ENDPOINT_NAME, TMO_AT_MAX_ENDPOINT_NAME_LEN);
#endif
        tmo->lifetime = TMO_AT_LIFETIME;
        tmo->secmode = TMO_AT_SECMODE_PRE_SHARED_KEY;
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
#if !defined(AUTO_PSK_GENERATOR)
        memcpy(tmo->pskid, TMO_AT_PSKID, TMO_AT_MAX_PSKID_LEN);
        memcpy(tmo->psk, TMO_AT_PSK, TMO_AT_MAX_PSK_LEN);
#endif
#endif /* WITH_TINYDTLS */
        return;
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_BOOTSTRAP) == TMO_AT_NVDM_ITEM_ID_BOOTSTRAP) {
        // bootstrap
        len = sizeof(bool);
        status = nvdm_read_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_BOOTSTRAP, (uint8_t*)&tmo->bootstrap, &len);
        TMO_AT_LOGI("read nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_BOOTSTRAP, tmo->bootstrap, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_SERVER_IP) == TMO_AT_NVDM_ITEM_ID_SERVER_IP) {
        // server_ip
        len = TMO_AT_MAX_SERVER_IP_LEN;
        status = nvdm_read_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_SERVER_IP, (uint8_t*)tmo->server_ip, &len);
        TMO_AT_LOGI("read nvdm [%s: %s] = %d", TMO_AT_NVDM_ITEM_NAME_SERVER_IP, tmo->server_ip, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_PORT) == TMO_AT_NVDM_ITEM_ID_PORT) {
        // port
        len = sizeof(uint32_t);
        status = nvdm_read_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_PORT, (uint8_t*)&tmo->port, &len);
        TMO_AT_LOGI("read nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_PORT, tmo->port, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_ENDPOINT) == TMO_AT_NVDM_ITEM_ID_ENDPOINT) {
        // endpoint
        len = TMO_AT_MAX_ENDPOINT_NAME_LEN;
        status = nvdm_read_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_ENDPOINT, (uint8_t*)tmo->endpoint_name, &len);
        TMO_AT_LOGI("read nvdm [%s: %s] = %d", TMO_AT_NVDM_ITEM_NAME_ENDPOINT, tmo->endpoint_name, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_LIFETIME) == TMO_AT_NVDM_ITEM_ID_LIFETIME) {
        // lifetime
        len = sizeof(uint32_t);
        status = nvdm_read_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_LIFETIME, (uint8_t*)&tmo->lifetime, &len);
        TMO_AT_LOGI("read nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_LIFETIME, tmo->lifetime, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_SECMODE) == TMO_AT_NVDM_ITEM_ID_SECMODE) {
        // secmode
        len = sizeof(tmo_at_secmode_t);
        status = nvdm_read_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_SECMODE, (uint8_t*)&tmo->secmode, &len);
        TMO_AT_LOGI("read nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_SECMODE, tmo->secmode, status);
    }

#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    if (tmo->secmode == TMO_AT_SECMODE_PRE_SHARED_KEY) {
        if ((flag & TMO_AT_NVDM_ITEM_ID_PSKID) == TMO_AT_NVDM_ITEM_ID_PSKID) {
            // pskid
            len = TMO_AT_MAX_PSKID_LEN;
            status = nvdm_read_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_PSKID, (uint8_t*)tmo->pskid, &len);
            TMO_AT_LOGI("read nvdm [%s: %s] = %d", TMO_AT_NVDM_ITEM_NAME_PSKID, tmo->pskid, status);
        }

        if ((flag & TMO_AT_NVDM_ITEM_ID_PSK) == TMO_AT_NVDM_ITEM_ID_PSK) {
            // psk
            len = TMO_AT_MAX_PSK_LEN;
            status = nvdm_read_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_PSK, (uint8_t*)tmo->psk, &len);
            TMO_AT_LOGI("read nvdm [%s: %s] = %d", TMO_AT_NVDM_ITEM_NAME_PSK, tmo->psk, status);
        }
    }
#endif /* WITH_TINYDTLS */

    if ((flag & TMO_AT_NVDM_ITEM_ID_REGSTATE) == TMO_AT_NVDM_ITEM_ID_REGSTATE) {
        // regstate
        len = sizeof(tmo_at_regstate_t);
        status = nvdm_read_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_REGSTATE, (uint8_t*)&tmo->regstate, &len);
        TMO_AT_LOGI("read nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_REGSTATE, tmo->regstate, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_DISABLE_T) == TMO_AT_NVDM_ITEM_ID_DISABLE_T) {
        // disable_timeout
        len = sizeof(uint32_t);
        status = nvdm_read_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_DISABLE_T, (uint8_t*)&tmo->disable_timeout, &len);
        TMO_AT_LOGI("read nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_DISABLE_T, tmo->disable_timeout, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_MIN_PERIOD) == TMO_AT_NVDM_ITEM_ID_MIN_PERIOD) {
        // min_period
        len = sizeof(uint32_t);
        status = nvdm_read_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_MIN_P, (uint8_t*)&tmo->min_period, &len);
        TMO_AT_LOGI("read nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_MIN_P, tmo->min_period, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_MAX_PERIOD) == TMO_AT_NVDM_ITEM_ID_MAX_PERIOD) {
        // max_period
        len = sizeof(uint32_t);
        status = nvdm_read_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_MAX_P, (uint8_t*)&tmo->max_period, &len);
        TMO_AT_LOGI("read nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_MAX_P, tmo->max_period, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_INSTANCE_ID) == TMO_AT_NVDM_ITEM_ID_INSTANCE_ID) {
        // instance_id
        len = sizeof(uint16_t);
        status = nvdm_read_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_INSTANCE_ID, (uint8_t*)&tmo->instance_id, &len);
        TMO_AT_LOGI("read nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_INSTANCE_ID, tmo->instance_id, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_SERVER_ID) == TMO_AT_NVDM_ITEM_ID_SERVER_ID) {
        // server_id
        len = sizeof(uint16_t);
        status = nvdm_read_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_SERVER_ID, (uint8_t*)&tmo->server_id, &len);
        TMO_AT_LOGI("read nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_SERVER_ID, tmo->server_id, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_BINDING) == TMO_AT_NVDM_ITEM_ID_BINDING) {
        // binding
        len = sizeof(tmo->binding);
        status = nvdm_read_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_BINDING, (uint8_t*)tmo->binding, &len);
        TMO_AT_LOGI("read nvdm [%s: %s] = %d", TMO_AT_NVDM_ITEM_NAME_BINDING, tmo->binding, status);
    }
}

static void tmo_at_write_nvdm(uint32_t flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tmo_at_context_t *tmo;
    uint32_t len;
    nvdm_status_t status;
    bool is_used = true;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo = &g_tmo_at_context;

    if ((flag & TMO_AT_NVDM_ITEM_ID_IS_USED) == TMO_AT_NVDM_ITEM_ID_IS_USED) {
        // is_used
        len = sizeof(bool);
        status = nvdm_write_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_IS_USED, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&is_used, len);
        TMO_AT_LOGI("write nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_IS_USED, is_used, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_BOOTSTRAP) == TMO_AT_NVDM_ITEM_ID_BOOTSTRAP) {
        // bootstrap
        len = sizeof(bool);
        status = nvdm_write_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_BOOTSTRAP, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&tmo->bootstrap, len);
        TMO_AT_LOGI("write nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_BOOTSTRAP, tmo->bootstrap, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_SERVER_IP) == TMO_AT_NVDM_ITEM_ID_SERVER_IP) {
        // server_ip
        len = TMO_AT_MAX_SERVER_IP_LEN;
        status = nvdm_write_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_SERVER_IP, NVDM_DATA_ITEM_TYPE_STRING, (uint8_t*)tmo->server_ip, len);
        TMO_AT_LOGI("write nvdm [%s: %s] = %d", TMO_AT_NVDM_ITEM_NAME_SERVER_IP, tmo->server_ip, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_PORT) == TMO_AT_NVDM_ITEM_ID_PORT) {
        // port
        len = sizeof(uint32_t);
        status = nvdm_write_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_PORT, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&tmo->port, len);
        TMO_AT_LOGI("write nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_PORT, tmo->port, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_ENDPOINT) == TMO_AT_NVDM_ITEM_ID_ENDPOINT) {
        // endpoint
        len = TMO_AT_MAX_ENDPOINT_NAME_LEN;
        status = nvdm_write_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_ENDPOINT, NVDM_DATA_ITEM_TYPE_STRING, (uint8_t*)tmo->endpoint_name, len);
        TMO_AT_LOGI("write nvdm [%s: %s] = %d", TMO_AT_NVDM_ITEM_NAME_ENDPOINT, tmo->endpoint_name, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_LIFETIME) == TMO_AT_NVDM_ITEM_ID_LIFETIME) {
        // lifetime
        len = sizeof(uint32_t);
        status = nvdm_write_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_LIFETIME, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&tmo->lifetime, len);
        TMO_AT_LOGI("write nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_LIFETIME, tmo->lifetime, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_SECMODE) == TMO_AT_NVDM_ITEM_ID_SECMODE) {
        // secmode
        len = sizeof(tmo_at_secmode_t);
        status = nvdm_write_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_SECMODE, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&tmo->secmode, len);
        TMO_AT_LOGI("write nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_SECMODE, tmo->secmode, status);
    }

#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    if (tmo->secmode == TMO_AT_SECMODE_PRE_SHARED_KEY) {
        if ((flag & TMO_AT_NVDM_ITEM_ID_PSKID) == TMO_AT_NVDM_ITEM_ID_PSKID) {
            // pskid
            len = TMO_AT_MAX_PSKID_LEN;
            status = nvdm_write_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_PSKID, NVDM_DATA_ITEM_TYPE_STRING, (uint8_t*)tmo->pskid, len);
            TMO_AT_LOGI("write nvdm [%s: %s] = %d", TMO_AT_NVDM_ITEM_NAME_PSKID, tmo->pskid, status);
        }

        if ((flag & TMO_AT_NVDM_ITEM_ID_PSK) == TMO_AT_NVDM_ITEM_ID_PSK) {
            // psk
            len = TMO_AT_MAX_PSK_LEN;
            status = nvdm_write_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_PSK, NVDM_DATA_ITEM_TYPE_STRING, (uint8_t*)tmo->psk, len);
            TMO_AT_LOGI("write nvdm [%s: %s] = %d", TMO_AT_NVDM_ITEM_NAME_PSK, tmo->psk, status);
        }
    }
#endif /* WITH_TINYDTLS */

    if ((flag & TMO_AT_NVDM_ITEM_ID_REGSTATE) == TMO_AT_NVDM_ITEM_ID_REGSTATE) {
        // regstate
        len = sizeof(tmo_at_regstate_t);
        status = nvdm_write_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_REGSTATE, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&tmo->regstate, len);
        TMO_AT_LOGI("write nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_REGSTATE, tmo->regstate, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_DISABLE_T) == TMO_AT_NVDM_ITEM_ID_DISABLE_T) {
        // disable_timeout
        len = sizeof(uint32_t);
        status = nvdm_write_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_DISABLE_T, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&tmo->disable_timeout, len);
        TMO_AT_LOGI("write nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_DISABLE_T, tmo->disable_timeout, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_MIN_PERIOD) == TMO_AT_NVDM_ITEM_ID_MIN_PERIOD) {
        // min_period
        len = sizeof(uint32_t);
        status = nvdm_write_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_MIN_P, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&tmo->min_period, len);
        TMO_AT_LOGI("write nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_MIN_P, tmo->min_period, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_MAX_PERIOD) == TMO_AT_NVDM_ITEM_ID_MAX_PERIOD) {
        // max_period
        len = sizeof(uint32_t);
        status = nvdm_write_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_MAX_P, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&tmo->max_period, len);
        TMO_AT_LOGI("write nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_MAX_P, tmo->max_period, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_INSTANCE_ID) == TMO_AT_NVDM_ITEM_ID_INSTANCE_ID) {
        // instance_id
        len = sizeof(uint16_t);
        status = nvdm_write_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_INSTANCE_ID, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&tmo->instance_id, len);
        TMO_AT_LOGI("write nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_INSTANCE_ID, tmo->instance_id, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_SERVER_ID) == TMO_AT_NVDM_ITEM_ID_SERVER_ID) {
        // server_id
        len = sizeof(uint16_t);
        status = nvdm_write_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_SERVER_ID, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&tmo->server_id, len);
        TMO_AT_LOGI("write nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_SERVER_ID, tmo->server_id, status);
    }

    if ((flag & TMO_AT_NVDM_ITEM_ID_BINDING) == TMO_AT_NVDM_ITEM_ID_BINDING) {
        // binding
        len = sizeof(tmo->binding);
        status = nvdm_write_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_BINDING, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)tmo->binding, len);
        TMO_AT_LOGI("write nvdm [%s: %s] = %d", TMO_AT_NVDM_ITEM_NAME_BINDING, tmo->binding, status);
    }
}

static void tmo_at_rtc_timer_callback(void *user_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tmo_at_context_t *tmo;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo = &g_tmo_at_context;
    if (tmo->rentention->disable) {
        tmo->rentention->disable = false;
        xTimerStartFromISR(tmo->disable_timer, &xHigherPriorityTaskWoken);
    } else if (tmo->regstate == TMO_AT_REGSTATE_REGISTERED) {
        tmo->is_need_update = true;
        if (!tmo->is_locking_sleep) {
            xTimerStartFromISR(tmo->ping_timer, &xHigherPriorityTaskWoken);
            tmo->is_locking_sleep = true;
            hal_sleep_manager_acquire_sleeplock(tmo->sleep_handle, HAL_SLEEP_LOCK_DEEP);
        }
    }
}

static void tmo_at_start_rtc_timer(tmo_at_context_t *tmo)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    rtc_sw_timer_status_t status;
    uint32_t lifetime;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tmo->rentention->disable) {
        lifetime = tmo->disable_timeout;
    } else {
        if (tmo->lifetime <= 30) {
            lifetime = tmo->lifetime / 2; /* lifetime 30s -> update 15s */
        } else if (tmo->lifetime <= 50) {
            lifetime = 15 + (tmo->lifetime - 30) * 3 / 4; /* lifetime 50s -> update 30s */
        } else if (tmo->lifetime <= 100) {
            lifetime = 30 + (tmo->lifetime - 50) * 4 / 5; /* lifetime 100s -> update 70s */
        } else if (tmo->lifetime <= 300) {
            lifetime = 70 + (tmo->lifetime - 100) * 9 / 10; /* lifetime 300s -> update 250s */
        } else {
            lifetime = 250 + (tmo->lifetime - 300) * 19 / 20;
        }
    }

    status = rtc_sw_timer_create(&tmo->rentention->rtc_handle, lifetime * 10, true, tmo_at_rtc_timer_callback);
    TMO_AT_LOGI("rtc_sw_timer_create = %d, time_out_100ms = %d", (int)status, (int)(lifetime * 10));
    status = rtc_sw_timer_start(tmo->rentention->rtc_handle);
    TMO_AT_LOGI("rtc_sw_timer_start = %d", (int)status);
}

static void tmo_at_stop_rtc_timer(tmo_at_context_t *tmo)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    rtc_sw_timer_status_t status;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    status = rtc_sw_timer_stop(tmo->rentention->rtc_handle);
    TMO_AT_LOGI("rtc_sw_timer_stop = %d", (int)status);
    status = rtc_sw_timer_delete(tmo->rentention->rtc_handle);
    TMO_AT_LOGI("rtc_sw_timer_delete = %d", (int)status);
}

static int32_t tmo_at_pdn_context_notify_callback(ril_urc_id_t event_id, void *param, uint32_t param_len);
static int32_t tmo_at_register_internal(bool from_deep_sleep);
static int32_t tmo_at_update(tmo_at_mode_t mode, uint32_t lifetime, tmo_at_binding_t binding);

static void tmo_at_ping_timeout_callback(TimerHandle_t xTimer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tmo_at_context_t *tmo;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo = &g_tmo_at_context;
    if (tmo->is_locking_sleep) {
        tmo->is_locking_sleep = false;
        hal_sleep_manager_release_sleeplock(tmo->sleep_handle, HAL_SLEEP_LOCK_DEEP);
    }
}

static void tmo_at_disable_timeout_callback(TimerHandle_t xTimer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tmo_at_context_t *tmo;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo = &g_tmo_at_context;
    tmo_at_stop_rtc_timer(tmo);
    tmo_at_register();
}

static void tmo_at_register_timeout_callback(TimerHandle_t xTimer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo_at_register();
}

static void tmo_at_task_processing(void *arg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tmo_at_context_t *tmo;
    lwm2m_context_t *lwm2mH;
    client_data_t *data;
    int result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo = &g_tmo_at_context;

    if (tmo->is_need_deep_sleep_handling) {
        tmo->is_need_deep_sleep_handling = false;
        // register with ready state
        configASSERT(tmo_at_register_internal(true) == TMO_AT_ERRID_SUCCESS);
        lwm2mH = tmo->lwm2m_handle;
        configASSERT(lwm2mH != NULL);
        lwm2mH->ext_reg_update = 1;
    } else if (tmo->is_need_update_registration_after_reboot) {
        tmo->is_need_update_registration_after_reboot = false;
        // register with ready state
        configASSERT(tmo_at_register_internal(true) == TMO_AT_ERRID_SUCCESS);
        lwm2mH = tmo->lwm2m_handle;
        configASSERT(lwm2mH != NULL);
        lwm2mH->ext_reg_update = 1;
        tmo_at_start_rtc_timer(tmo);
        tmo->is_need_update = true;
    } else {
        lwm2mH = tmo->lwm2m_handle;
        configASSERT(lwm2mH != NULL);
        lwm2mH->ext_reg_update = 1;
        tmo_at_start_rtc_timer(tmo);
    }

    data = (client_data_t*)lwm2mH->userData;
    configASSERT(data != NULL);

    while (tmo->is_task_running) {
        struct timeval tv;
        fd_set readfds;

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        if (tmo->is_need_update) {
            tmo->is_need_update = false;
            lwm2m_update_registration(lwm2mH, 0, false);
        } else if (tmo->is_need_deregister) {
            tmo->is_need_deregister = false;
            tmo_at_deregister();
        } else if (tmo->is_need_reboot) {
            hal_wdt_config_t wdt_config;
            // config wdt as RESET mode
            wdt_config.mode = HAL_WDT_MODE_RESET;
            wdt_config.seconds = 1;
            hal_wdt_init(&wdt_config);
            // trigger wdt software reset
            hal_wdt_software_reset();
            while(1);
        }

        /*
         * This function does two things:
         *  - first it does the work needed by liblwm2m (eg. (re)sending some packets).
         *  - Secondly it adjusts the timeout value (default 60s) depending on the state of the transaction
         *    (eg. retransmission) and the time between the next operation
         */
        result = lwm2m_step(lwm2mH, &(tv.tv_sec));
        if (result != COAP_NO_ERROR) {
            TMO_AT_LOGE("lwm2m_step() failed: 0x%X", result);
        }

#ifdef WITH_MBEDTLS
        mbedtls_connection_t *connP = data->connList;
        if (connP != NULL && connP->dtlsSession != 0) {
            int result = connection_handle_packet(connP);
            if (result < 0) {
                TMO_AT_LOGE("error handling message %d", result);
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
                TMO_AT_LOGE("Error in select(): %d %s", errno, strerror(errno));
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
                    TMO_AT_LOGE("Error in recvfrom(): %d %s", errno, strerror(errno));
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

                    TMO_AT_LOGI("%d bytes received from [%s]", numBytes, s);

                    connP = connection_find(data->connList, &addr, addrLen);
                    if (connP != NULL) {
                        /*
                         * Let liblwm2m respond to the query depending on the context
                         */
#ifdef WITH_TINYDTLS
                        int result = connection_handle_packet(connP, buffer, numBytes);
                        if (result != 0) {
                            TMO_AT_LOGE("error handling message %d", result);
                        }
#else
                        lwm2m_handle_packet(lwm2mH, buffer, numBytes, connP);
#endif
                    } else {
                        TMO_AT_LOGE("no connection found");
                    }
                }
            }
        }
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }

    tmo_at_stop_rtc_timer(tmo);

    lwm2m_close(lwm2mH);
    close(data->sock);
    connection_free(data->connList);

    clean_security_object(g_tmo_at_objects[0]);
    lwm2m_free(g_tmo_at_objects[0]);
    clean_server_object(g_tmo_at_objects[1]);
    lwm2m_free(g_tmo_at_objects[1]);
    acl_ctrl_free_object(g_tmo_at_objects[2]);
    free_object_device(g_tmo_at_objects[3]);
    free_object_conn_m(g_tmo_at_objects[4]);
    free_object_firmware(g_tmo_at_objects[5]);
    free_object_cellular_connectivity(g_tmo_at_objects[6]);

    if (tmo->rentention->disable && tmo->disable_timeout != 0) {
        tmo_at_start_rtc_timer(tmo);
    } else if (tmo->is_need_register) {
        tmo->is_need_register = false;
        if (tmo->register_timer == NULL) {
            tmo->register_timer = xTimerCreate("tmo_reg", 100 / portTICK_PERIOD_MS, pdFALSE, NULL, tmo_at_register_timeout_callback);
        }
        configASSERT(tmo->register_timer != NULL);
        xTimerStart(tmo->register_timer, 0);
    }

    /* Set not register state when leaving lwm2m task */
    tmo->regstate = TMO_AT_REGSTATE_NOT_REGISTER;
    tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_REGSTATE);

    TMO_AT_LOGI("delete tmo task");

    vTaskDelete(NULL);
}

void tmo_at_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tmo_at_context_t *tmo;
    ril_status_t status;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo = &g_tmo_at_context;
    tmo->rentention = &g_tmo_at_rentention;
    tmo->mutex_handle = xSemaphoreCreateMutex();
    configASSERT(tmo->mutex_handle != NULL);

    tmo->ping_timer = xTimerCreate("tmo_ping", 1000 * COAP_MAX_TRANSMIT_WAIT / portTICK_PERIOD_MS, pdFALSE, NULL, tmo_at_ping_timeout_callback);
    configASSERT(tmo->ping_timer != NULL);

    tmo->disable_timer = xTimerCreate("tmo_disable", 100 / portTICK_PERIOD_MS, pdFALSE, NULL, tmo_at_disable_timeout_callback);
    configASSERT(tmo->disable_timer != NULL);

    tmo->sleep_handle = hal_sleep_manager_set_sleep_handle("tmo_at");
    configASSERT(tmo->sleep_handle != SLEEP_LOCK_INVALID_ID);

    status = ril_register_event_callback(RIL_ALL, tmo_at_pdn_context_notify_callback);
    configASSERT(status == RIL_STATUS_SUCCESS);

    if (rtc_power_on_result_external() != DEEP_SLEEP && rtc_power_on_result_external() != DEEPER_SLEEP) {
        /* COLD-BOOT case: normal init */
#ifdef AUTO_UPDATE_REGISTRATION
        tmo_at_read_nvdm(TMO_AT_NVDM_ITEM_ID_REGSTATE);
        TMO_AT_LOGI("auto update registration, regstate: %d", tmo->regstate);
        if (tmo->regstate == TMO_AT_REGSTATE_REGISTERED) {
            tmo->is_need_update_registration_after_reboot = true;
            if (!tmo->is_task_running) {
                tmo->is_task_running = true;
                xTaskCreate(tmo_at_task_processing,
                            TMO_AT_TASK_NAME,
                            TMO_AT_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                            NULL,
                            TMO_AT_TASK_PRIORITY,
                            NULL);
                TMO_AT_LOGI("create tmo task");
            }
            return;
        }
#endif /* AUTO_UPDATE_REGISTRATION */
        tmo->regstate = TMO_AT_REGSTATE_NOT_REGISTER;
        tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_REGSTATE);
        tmo->disable_timeout = LWM2M_SERVER_DISABLE_TIMEOUT;
        tmo->min_period = LWM2M_SERVER_DEFAULT_MIN_PERIOD;
        tmo->max_period = LWM2M_SERVER_DEFAULT_MAX_PERIOD;
        tmo->instance_id = TMO_AT_INSTANCE_ID;
        tmo->server_id = TMO_AT_SERVER_ID;
        sprintf(tmo->binding, "%s", TMO_AT_BINDING);
    } else {
        /* DEEP-SLEEP case: data retention process */
        tmo_at_read_nvdm(TMO_AT_NVDM_ITEM_ID_REGSTATE);
        TMO_AT_LOGI("deep sleep handling, regstate: %d", tmo->regstate);
        if (tmo->regstate != TMO_AT_REGSTATE_NOT_REGISTER) {
            tmo->is_need_deep_sleep_handling = true;
            if (!tmo->is_task_running) {
                tmo->is_task_running = true;
                xTaskCreate(tmo_at_task_processing,
                            TMO_AT_TASK_NAME,
                            TMO_AT_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                            NULL,
                            TMO_AT_TASK_PRIORITY,
                            NULL);
                TMO_AT_LOGI("create tmo task");
            }
        }
    }
}

static int32_t tmo_at_config(bool bootstrap, char *server_ip, uint32_t port, char *endpoint_name, uint32_t lifetime, tmo_at_secmode_t secmode, char *pskid, char *psk)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tmo_at_context_t *tmo;
    int32_t tmo_error = TMO_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo = &g_tmo_at_context;
    if (tmo->regstate != TMO_AT_REGSTATE_NOT_REGISTER) {
        return TMO_AT_ERRID_KEEP_CONNECTING_ERROR;
    }

    tmo->bootstrap = bootstrap;
    strcpy(tmo->server_ip, server_ip);
    tmo->port = port;
    strcpy(tmo->endpoint_name, endpoint_name);
    tmo->lifetime = lifetime;
    tmo->secmode = secmode;
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    if (pskid != NULL) {
        strcpy(tmo->pskid, pskid);
    } else {
        tmo->pskid[0] = '\0';
    }
    if (psk != NULL) {
        strcpy(tmo->psk, psk);
    } else {
        tmo->psk[0] = '\0';
    }
#endif /* WITH_TINYDTLS */

    tmo->disable_timeout = LWM2M_SERVER_DISABLE_TIMEOUT;
    tmo->min_period = LWM2M_SERVER_DEFAULT_MIN_PERIOD;
    tmo->max_period = LWM2M_SERVER_DEFAULT_MAX_PERIOD;
    tmo->instance_id = TMO_AT_INSTANCE_ID;
    tmo->server_id = TMO_AT_SERVER_ID;
    sprintf(tmo->binding, "%s", TMO_AT_BINDING);

    tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_IS_USED | TMO_AT_NVDM_ITEM_ID_BOOTSTRAP |
        TMO_AT_NVDM_ITEM_ID_SERVER_IP | TMO_AT_NVDM_ITEM_ID_PORT | TMO_AT_NVDM_ITEM_ID_ENDPOINT | TMO_AT_NVDM_ITEM_ID_LIFETIME |
        TMO_AT_NVDM_ITEM_ID_SECMODE | TMO_AT_NVDM_ITEM_ID_PSKID | TMO_AT_NVDM_ITEM_ID_PSK |
        TMO_AT_NVDM_ITEM_ID_DISABLE_T | TMO_AT_NVDM_ITEM_ID_MIN_PERIOD | TMO_AT_NVDM_ITEM_ID_MAX_PERIOD | TMO_AT_NVDM_ITEM_ID_INSTANCE_ID | TMO_AT_NVDM_ITEM_ID_SERVER_ID | TMO_AT_NVDM_ITEM_ID_BINDING);

    return tmo_error;
}

static int32_t tmo_at_config_query(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    char response_data[TMO_AT_MAX_SERVER_IP_LEN + TMO_AT_MAX_PSKID_LEN + TMO_AT_MAX_PSK_LEN + TMO_AT_RESPONSE_DATA_LEN];
#else
    char response_data[TMO_AT_MAX_SERVER_IP_LEN + TMO_AT_RESPONSE_DATA_LEN];
#endif
    tmo_at_context_t *tmo;
    int32_t tmo_error = TMO_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo_at_read_nvdm(TMO_AT_NVDM_ITEM_ID_BOOTSTRAP |
        TMO_AT_NVDM_ITEM_ID_SERVER_IP | TMO_AT_NVDM_ITEM_ID_PORT | TMO_AT_NVDM_ITEM_ID_ENDPOINT | TMO_AT_NVDM_ITEM_ID_LIFETIME |
        TMO_AT_NVDM_ITEM_ID_SECMODE | TMO_AT_NVDM_ITEM_ID_PSKID | TMO_AT_NVDM_ITEM_ID_PSK);
    tmo = &g_tmo_at_context;

    // +TMOCONFIG=<Bootstrap_Enabled>,<Sever_IP>,<Port>,<Endpoint_Name>,<lifetime>,<Security_Mode>[,<PSKID>,<PSK>]
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;

    switch (tmo->secmode) {
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
        case TMO_AT_SECMODE_PRE_SHARED_KEY:
            sprintf(response_data, "+TMOCONFIG:%d,%s,%d,\"%s\",%d,%d,%s,%s", tmo->bootstrap, tmo->server_ip, (int)tmo->port,
                    tmo->endpoint_name, (int)tmo->lifetime, (int)tmo->secmode, tmo->pskid, tmo->psk);
            break;
#endif /* WITH_TINYDTLS */

        case TMO_AT_SECMODE_NO_SECURITY:
            sprintf(response_data, "+TMOCONFIG:%d,%s,%d,\"%s\",%d,%d", tmo->bootstrap, tmo->server_ip, (int)tmo->port,
                    tmo->endpoint_name, (int)tmo->lifetime, (int)tmo->secmode);
            break;

        default:
            configASSERT(0);
            break;
    }

    response.pdata = response_data;
    response.length = strlen(response.pdata);

    response.cmd_id = APB_PROXY_INVALID_CMD_ID;
    apb_proxy_send_at_cmd_result(&response);

    return tmo_error;
}

static void tmo_at_save_config(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tmo_at_context_t *tmo;
    lwm2m_context_t *lwm2mH;
    lwm2m_server_t *server;
    client_data_t *data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo = &g_tmo_at_context;
    lwm2mH = tmo->lwm2m_handle;
    configASSERT(lwm2mH != NULL);

    server = lwm2mH->serverList;
    configASSERT(server != NULL);

    data = (client_data_t*)lwm2mH->userData;
    configASSERT(data != NULL);

#define COAP_PORT "5683"
#define COAPS_PORT "5684"
#define URI_LENGTH 256
    char uriBuf[URI_LENGTH];
    char * uri;
    char * host;
    char * port;
    uri = security_get_uri(data->securityObjP, server->secObjInstID, uriBuf, URI_LENGTH);
    if (uri != NULL) {
        // parse uri in the form "coaps://[host]:[port]"
        char * defaultport;
        if (0 == strncmp(uri, "coaps://", strlen("coaps://"))) {
            host = uri + strlen("coaps://");
            defaultport = COAPS_PORT;
        } else if (0 == strncmp(uri, "coap://", strlen("coap://"))) {
            host = uri + strlen("coap://");
            defaultport = COAP_PORT;
        } else {
            TMO_AT_LOGE("parse host failed");
            return;
        }
        port = strrchr(host, ':');
        if (port == NULL) {
            port = defaultport;
        } else {
            // remove brackets
            if (host[0] == '[') {
                host++;
                if (*(port - 1) == ']') {
                    *(port - 1) = 0;
                } else {
                    TMO_AT_LOGE("remove brackets failed");
                    return;
                }
            }
            // split strings
            *port = 0;
            port++;
        }
        tmo->bootstrap = false; /* skip bootstrap flow */
        memcpy(tmo->server_ip, host, TMO_AT_MAX_SERVER_IP_LEN);
        tmo->port = atoi(port);
    }

#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    tmo->secmode = (tmo_at_secmode_t)security_get_mode(data->securityObjP, server->secObjInstID);
#else
    tmo->secmode = TMO_AT_SECMODE_NO_SECURITY;
#endif
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    int idLen;
    char * id;
    id = security_get_public_id(data->securityObjP, server->secObjInstID, &idLen);
    if (id != NULL) {
        memcpy(tmo->pskid, id, idLen);
        tmo->pskid[idLen] = '\0';
        lwm2m_free(id);
    } else {
        tmo->pskid[0] = '\0';
    }

    int keyLen;
    char * key;
    key = security_get_secret_key(data->securityObjP, server->secObjInstID, &keyLen);
    if (key != NULL) {
        char *psk = lwm2m_malloc(2 * keyLen + 1);
        onenet_at_bin_to_hex(psk, (uint8_t*)key, 2 * keyLen); /* psk with '\0' */
        memcpy(tmo->psk, psk, 2 * keyLen);
        lwm2m_free(psk);
        lwm2m_free(key);
    } else {
        tmo->psk[0] = '\0';
    }
#endif /* WITH_TINYDTLS */

    tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_IS_USED | TMO_AT_NVDM_ITEM_ID_BOOTSTRAP |
        TMO_AT_NVDM_ITEM_ID_SERVER_IP | TMO_AT_NVDM_ITEM_ID_PORT | TMO_AT_NVDM_ITEM_ID_ENDPOINT | TMO_AT_NVDM_ITEM_ID_LIFETIME |
        TMO_AT_NVDM_ITEM_ID_SECMODE | TMO_AT_NVDM_ITEM_ID_PSKID | TMO_AT_NVDM_ITEM_ID_PSK);
}

static void tmo_at_delete_config(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    uint32_t len;
    nvdm_status_t status;
    bool is_used = false;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    // is_used
    len = sizeof(bool);
    status = nvdm_write_data_item(TMO_AT_NVDM_GROUP_NAME, TMO_AT_NVDM_ITEM_NAME_IS_USED, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&is_used, len);
    TMO_AT_LOGI("write nvdm [%s: %d] = %d", TMO_AT_NVDM_ITEM_NAME_IS_USED, is_used, status);
}

static void tmo_at_notify_callback(lwm2m_notify_type_t type, lwm2m_notify_code_t code, uint32_t option_info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[TMO_AT_RESPONSE_DATA_LEN];
    tmo_at_context_t *tmo;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TMO_AT_LOGI("type = %d, code = %d, option_info = %d", (int)type, (int)code, (int)option_info);

    tmo = &g_tmo_at_context;

    // +TMO:<Operation>,<status code>[,<Orig MsgID>]
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;

    switch (type) {
        case LWM2M_NOTIFY_TYPE_REG:
            sprintf(response_data, "+TMO:reg,%d", (int)code);
            if (code == LWM2M_NOTIFY_CODE_SUCCESS) {
                tmo->regstate = TMO_AT_REGSTATE_REGISTERED;
                tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_REGSTATE);
                tmo_at_save_config();
            } else {
                tmo->is_task_running = false;
                tmo_at_delete_config();
            }
            break;

        case LWM2M_NOTIFY_TYPE_UPDATE:
            sprintf(response_data, "+TMOUPDATE:%d", (int)option_info);
            break;

        case LWM2M_NOTIFY_TYPE_UPDATE_CONFIRM:
            sprintf(response_data, "+TMO:update,%d,%d", (int)code, (int)option_info);
            if (code == LWM2M_NOTIFY_CODE_FORBIDDEN || code == LWM2M_NOTIFY_CODE_NOT_FOUND) {
                tmo->is_task_running = false;
                tmo->regstate = TMO_AT_REGSTATE_DEREGISTERING;
                tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_REGSTATE);
            }
            break;

        case LWM2M_NOTIFY_TYPE_PING:
            sprintf(response_data, "+TMO:ping,%d", (int)code);
            if (code == LWM2M_NOTIFY_CODE_BAD_REQUEST || code == LWM2M_NOTIFY_CODE_FORBIDDEN || code == LWM2M_NOTIFY_CODE_NOT_FOUND) {
                tmo->is_task_running = false;
                tmo->regstate = TMO_AT_REGSTATE_DEREGISTERING;
                tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_REGSTATE);
            } else if ((code == LWM2M_NOTIFY_CODE_TIMEOUT || code == LWM2M_NOTIFY_CODE_SUCCESS) && tmo->is_locking_sleep) {
                xTimerStop(tmo->ping_timer, 0);
                tmo->is_locking_sleep = false;
                hal_sleep_manager_release_sleeplock(tmo->sleep_handle, HAL_SLEEP_LOCK_DEEP);
            }
            break;

        case LWM2M_NOTIFY_TYPE_DEREG:
            sprintf(response_data, "+TMO:dereg,%d", (int)code);
            if (code == LWM2M_NOTIFY_CODE_SUCCESS || code == LWM2M_NOTIFY_CODE_BAD_REQUEST || code == LWM2M_NOTIFY_CODE_FORBIDDEN || code == LWM2M_NOTIFY_CODE_NOT_FOUND) {
                tmo->is_task_running = false;
                tmo->regstate = TMO_AT_REGSTATE_DEREGISTERING;
                tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_REGSTATE);
            }
            break;

        case LWM2M_NOTIFY_TYPE_SEND:
            sprintf(response_data, "+TMOREPORT:%d", (int)option_info);
            break;

        case LWM2M_NOTIFY_TYPE_SEND_CONFIRM:
            sprintf(response_data, "+TMO:report,%d,%d", (int)code, (int)option_info);
            if (code == LWM2M_NOTIFY_CODE_RST) {
                tmo->is_task_running = false;
                tmo->regstate = TMO_AT_REGSTATE_DEREGISTERING;
                tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_REGSTATE);
            }
            break;

        case LWM2M_NOTIFY_TYPE_LWSTATUS:
            if (tmo->regstate != TMO_AT_REGSTATE_REGISTERED) {
                // do nothing
                return;
            } else {
                sprintf(response_data, "+TMO:lwstatus,%d", (int)option_info);
                tmo->is_task_running = false;
                tmo->regstate = TMO_AT_REGSTATE_DEREGISTERING;
                tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_REGSTATE);
            }
            break;

        case LWM2M_NOTIFY_TYPE_REGING:
            tmo->regstate = TMO_AT_REGSTATE_REGISTERING;
            tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_REGSTATE);
            return;

        case LWM2M_NOTIFY_TYPE_DEREGING:
            tmo->regstate = TMO_AT_REGSTATE_DEREGISTERING;
            tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_REGSTATE);
            return;

        case LWM2M_NOTIFY_TYPE_LIFETIME_CHANGED:
            sprintf(response_data, "+TMO:lifetime,%d", (int)option_info);
            if (tmo->regstate != TMO_AT_REGSTATE_REGISTERED) {
                tmo->lifetime = option_info;
                tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_LIFETIME);
                tmo_at_stop_rtc_timer(tmo);
                tmo_at_start_rtc_timer(tmo);
            } else {
                tmo_at_update(TMO_AT_MODE_LIFETIME, option_info, TMO_AT_BINDING_TOTAL);
            }
            break;

        case LWM2M_NOTIFY_TYPE_MIN_PERIOD_CHANGED:
            sprintf(response_data, "+TMO:min period,%d", (int)option_info);
            tmo->min_period = (uint32_t)option_info;
            tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_MIN_PERIOD);
            connectivity_moni_min_period_change(tmo->lwm2m_handle, tmo->min_period);
            break;

        case LWM2M_NOTIFY_TYPE_MAX_PERIOD_CHANGED:
            sprintf(response_data, "+TMO:max period,%d", (int)option_info);
            tmo->max_period = (uint32_t)option_info;
            tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_MAX_PERIOD);
            connectivity_moni_max_period_change(tmo->lwm2m_handle, tmo->max_period);
            break;

        case LWM2M_NOTIFY_TYPE_SERVER_INSTANCE_ID:
            sprintf(response_data, "+TMO:server instance id,%d", (int)option_info);
            tmo->instance_id = (uint16_t)option_info;
            tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_INSTANCE_ID);
            break;

        case LWM2M_NOTIFY_TYPE_SHORT_SERVER_ID:
            sprintf(response_data, "+TMO:short server id,%d", (int)option_info);
            tmo->server_id = (uint16_t)option_info;
            tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_SERVER_ID);
            break;

        case LWM2M_NOTIFY_TYPE_BINDING_CHANGED:
            sprintf(response_data, "+TMO:binding,%s", (char*)option_info);
            sprintf(tmo->binding, "%s", (char*)option_info);
            tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_BINDING);
            break;

        case LWM2M_NOTIFY_TYPE_DISABLE:
            sprintf(response_data, "+TMO:disable,%d", (int)option_info);
            tmo->is_need_deregister = true;
            tmo->rentention->disable = true;
            tmo->disable_timeout = (uint32_t)option_info;
            tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_DISABLE_T);
            break;

        case LWM2M_NOTIFY_TYPE_REBOOT:
            sprintf(response_data, "+TMO:reboot");
            tmo->is_need_reboot = true;
            break;

        case LWM2M_NOTIFY_TYPE_FACTORY_RESET:
            sprintf(response_data, "+TMO:factory reset");
            tmo->is_need_reboot = true;
            tmo_at_delete_config();
            break;

        case LWM2M_NOTIFY_TYPE_BS_FAILED_BACKOFF:
            sprintf(response_data, "+TMO:BS failed back off");
            tmo->is_task_running = false;
            tmo->regstate = TMO_AT_REGSTATE_DEREGISTERING;
            tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_REGSTATE);
            break;

        case LWM2M_NOTIFY_TYPE_REG_FAILED_BACKOFF:
            sprintf(response_data, "+TMO:reg failed back off");
            tmo->is_need_register = true;
            tmo->is_task_running = false;
            tmo->regstate = TMO_AT_REGSTATE_DEREGISTERING;
            tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_REGSTATE);
            tmo_at_delete_config();
            break;

        case LWM2M_NOTIFY_TYPE_PING_FAILED_BACKOFF:
            sprintf(response_data, "+TMO:ping failed back off");
            break;

        default:
            return;
    }

    response.pdata = response_data;
    response.length = strlen(response.pdata);

    response.cmd_id = APB_PROXY_INVALID_CMD_ID;
    apb_proxy_send_at_cmd_result(&response);
}

static uint8_t tmo_at_observe_callback(uint16_t code, uint16_t msg_id, lwm2m_uri_t *uriP, void *user_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[TMO_AT_RESPONSE_DATA_LEN];
    tmo_at_context_t *tmo;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TMO_AT_LOGI("code = %d, msg_id = %d, uriP = %d/%d/%d|%d", (int)code, (int)msg_id, (int)uriP->objectId, (int)uriP->instanceId, (int)uriP->resourceId, (int)uriP->flag);

    if (uriP->flag & LWM2M_URI_FLAG_OBJECT_ID) {
        sprintf(response_data, "+TMO:observe,%d,%d,%d,%d", (int)code, (int)uriP->objectId,
                (uriP->flag & LWM2M_URI_FLAG_INSTANCE_ID) ? (int)uriP->instanceId : -1,
                (uriP->flag & LWM2M_URI_FLAG_RESOURCE_ID) ? (int)uriP->resourceId : -1);

        response.pdata = response_data;
        response.length = strlen(response.pdata);

        response.cmd_id = APB_PROXY_INVALID_CMD_ID;
        apb_proxy_send_at_cmd_result(&response);

        if (uriP->objectId == LWM2M_CONN_MONITOR_OBJECT_ID) {
            tmo = &g_tmo_at_context;
            connectivity_moni_observe_notify(tmo->lwm2m_handle, (code == 0) ? true : false, uriP);
        }
    }

    return COAP_NO_ERROR;
}

static uint8_t tmo_at_set_parameter_callback(lwm2m_uri_t *uriP, lwm2m_attributes_t *attrP, void *user_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[TMO_AT_RESPONSE_DATA_LEN];
    tmo_at_context_t *tmo;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TMO_AT_LOGI("uriP = %d/%d/%d|%d", uriP->objectId, uriP->instanceId, uriP->resourceId, uriP->flag);
    TMO_AT_LOGI("attrP = %d,%d,%d,%d,%f,%f,%f",
                (int)attrP->toSet, (int)attrP->toClear, (int)attrP->minPeriod, (int)attrP->maxPeriod,
                attrP->greaterThan, attrP->lessThan, attrP->step);

    if (uriP->flag & LWM2M_URI_FLAG_OBJECT_ID) {
        sprintf(response_data, "+TMO:set parameter,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f", (int)uriP->objectId,
                (uriP->flag & LWM2M_URI_FLAG_INSTANCE_ID) ? (int)uriP->instanceId : -1,
                (uriP->flag & LWM2M_URI_FLAG_RESOURCE_ID) ? (int)uriP->resourceId : -1,
                (int)attrP->toSet, (int)attrP->toClear, (int)attrP->minPeriod, (int)attrP->maxPeriod,
                attrP->greaterThan, attrP->lessThan, attrP->step);

        response.pdata = response_data;
        response.length = strlen(response.pdata);

        response.cmd_id = APB_PROXY_INVALID_CMD_ID;
        apb_proxy_send_at_cmd_result(&response);

        if (uriP->objectId == LWM2M_CONN_MONITOR_OBJECT_ID) {
            tmo = &g_tmo_at_context;
            connectivity_moni_set_parameter_notify(tmo->lwm2m_handle, uriP, attrP);
        }
    }

    return COAP_NO_ERROR;
}

static int32_t tmo_at_query_imei_callback(ril_cmd_response_t *response)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tmo_at_context_t *tmo;
    ril_serial_number_rsp_t *param;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo = &g_tmo_at_context;
    memset(tmo->imei, 0, sizeof(tmo->imei));

    if (!response || response->res_code != RIL_RESULT_CODE_OK) {
        xSemaphoreGive(tmo->mutex_handle);
        return 0;
    }

    param = (ril_serial_number_rsp_t*)response->cmd_param;
    if (param && param->value.imei) {
        memcpy(tmo->imei, param->value.imei, TMO_AT_MAX_IMEI_LEN);
    }

    xSemaphoreGive(tmo->mutex_handle);
    return 0;
}

static int32_t tmo_at_pdn_context_notify_callback(ril_urc_id_t event_id, void *param, uint32_t param_len)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tmo_at_context_t *tmo;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!param || !param_len) {
        return 0;
    }

    switch (event_id) {
        case RIL_URC_ID_CGEV: {
            ril_cgev_event_reporting_urc_t *cgev_urc = (ril_cgev_event_reporting_urc_t*)param;

            if (cgev_urc->response_type == RIL_URC_TYPE_ME_PDN_DEACT || cgev_urc->response_type == RIL_URC_TYPE_NW_PDN_DEACT) {
                tmo_at_notify_callback(LWM2M_NOTIFY_TYPE_LWSTATUS, LWM2M_NOTIFY_CODE_SUCCESS, LWM2M_NOTIFY_CODE_LWM2M_SESSION_INVALID);
            } else if (cgev_urc->response_type == RIL_URC_TYPE_ME_PDN_ACT) {
                /* do not use mutex in RIL callback */
                tmo = &g_tmo_at_context;
                if (tmo->register_timer == NULL) {
                    tmo->register_timer = xTimerCreate("tmo_reg", 100 / portTICK_PERIOD_MS, pdFALSE, NULL, tmo_at_register_timeout_callback);
                }
                configASSERT(tmo->register_timer != NULL);
                xTimerStart(tmo->register_timer, 0);
            }
            break;
        }

        default:
            break;
    }

    return 0;
}

static int32_t tmo_at_register_internal(bool from_deep_sleep)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tmo_at_context_t *tmo;
    lwm2m_context_t *lwm2mH;
    client_data_t *data;
    int32_t tmo_error = TMO_AT_ERRID_SUCCESS;
    int result;
    ril_status_t status;
    bool bootstrap;
    char server_uri[80];
    char port[20];
    char endpoint_name[40];
    char *pskid = NULL;
    char *psk_buffer = NULL;
    uint16_t psk_len = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo_at_read_nvdm(TMO_AT_NVDM_ITEM_ID_BOOTSTRAP |
        TMO_AT_NVDM_ITEM_ID_SERVER_IP | TMO_AT_NVDM_ITEM_ID_PORT | TMO_AT_NVDM_ITEM_ID_ENDPOINT | TMO_AT_NVDM_ITEM_ID_LIFETIME |
        TMO_AT_NVDM_ITEM_ID_SECMODE | TMO_AT_NVDM_ITEM_ID_PSKID | TMO_AT_NVDM_ITEM_ID_PSK |
        TMO_AT_NVDM_ITEM_ID_DISABLE_T | TMO_AT_NVDM_ITEM_ID_MIN_PERIOD | TMO_AT_NVDM_ITEM_ID_MAX_PERIOD | TMO_AT_NVDM_ITEM_ID_INSTANCE_ID | TMO_AT_NVDM_ITEM_ID_SERVER_ID | TMO_AT_NVDM_ITEM_ID_BINDING);

    tmo = &g_tmo_at_context;
    if (tmo->server_ip[0] == '\0' || tmo->port == 0) {
        TMO_AT_LOGE("Failed to register");
        return TMO_AT_ERRID_OTHER_ERROR;
    }

    data = &tmo->client_data;
    memset(data, 0, sizeof(client_data_t));
    data->addressFamily = AF_UNSPEC; /* lwip will identify ipv4 or ipv6 */

#if defined(AUTO_PSK_GENERATOR)
    if (tmo->psk[0] != '\0') {
        TMO_AT_LOGI("psk: %s", tmo->psk);
    } else if (tmo->secmode == TMO_AT_SECMODE_PRE_SHARED_KEY) {
        /* get imei */
        xSemaphoreTake(tmo->mutex_handle, portMAX_DELAY);
        status = ril_request_serial_number(RIL_EXECUTE_MODE, 1, tmo_at_query_imei_callback, NULL);
        if (status != RIL_STATUS_SUCCESS) {
            xSemaphoreGive(tmo->mutex_handle);
            return TMO_AT_ERRID_OTHER_ERROR;
        }

        /* wait for RIL callback */
        xSemaphoreTake(tmo->mutex_handle, portMAX_DELAY);
        xSemaphoreGive(tmo->mutex_handle);
        if (!tmo->imei[0]) {
            return TMO_AT_ERRID_CREATE_LWM2M_ERROR;
        } else {
            snprintf(tmo->endpoint_name, sizeof(tmo->endpoint_name), "urn:imei:%s", tmo->imei);
            TMO_AT_LOGI("endpoint_name: %s", tmo->endpoint_name);
            snprintf(tmo->pskid, sizeof(tmo->pskid), "urn:imei:%s", tmo->imei);
            TMO_AT_LOGI("pskid: %s", tmo->pskid);
            char intput[32];
            unsigned char output[32]; /* SHA-256 outputs 32 bytes */

            snprintf(intput, sizeof(intput), "IMEI:%s_TMOMTKDDI", tmo->imei);
            /* 0 here means use the full SHA-256, not the SHA-224 variant */
            mbedtls_sha256((unsigned char *)intput, strlen(intput), output, 0);
            onenet_at_bin_to_hex(tmo->psk, output, 64); /* psk with '\0' */
            TMO_AT_LOGI("psk: %s", tmo->psk);
        }
    }
#endif /* defined(AUTO_PSK_GENERATOR) */

    bootstrap = tmo->bootstrap;
    snprintf(server_uri, sizeof(server_uri), "coap://%s:%d", tmo->server_ip, (int)tmo->port);
    sprintf(port, "%d", (int)tmo->port);
    snprintf(endpoint_name, sizeof(endpoint_name), "%s", tmo->endpoint_name);

    TMO_AT_LOGI("creat socket start: port = %s, addressFamily = %d", port, data->addressFamily);

    data->sock = create_socket(port, data->addressFamily);
    if (data->sock < 0) {
        TMO_AT_LOGE("Failed to open socket: %d %s", errno, strerror(errno));
        return TMO_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    TMO_AT_LOGI("creat socket start: sock = %d", data->sock);

#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    if (tmo->pskid[0] != '\0') {
        pskid = tmo->pskid;
    }

    if (tmo->psk[0] != '\0') {
        psk_len = strlen(tmo->psk) / 2;
        psk_buffer = pvPortMalloc(psk_len);
        if (psk_buffer == NULL) {
            TMO_AT_LOGE("Failed to create PSK binary buffer");
            close(data->sock);
            return TMO_AT_ERRID_OTHER_ERROR;
        }

        // Hex string to binary
        char *h = tmo->psk;
        char *b = psk_buffer;
        char xlate[] = "0123456789ABCDEF";

        for (; *h; h += 2, ++b) {
            if (h - tmo->psk >= psk_len * 2) break;
            char *l = strchr(xlate, (char)toupper((unsigned char)*h));
            char *r = strchr(xlate, (char)toupper((unsigned char)*(h + 1)));

            if (!r || !l) {
                TMO_AT_LOGE("Failed to parse Pre-Shared Keys");
                vPortFree(psk_buffer);
                close(data->sock);
                return TMO_AT_ERRID_OTHER_ERROR;
            }

            *b = ((l - xlate) << 4) + (r - xlate);
        }
    }
#endif /* WITH_TINYDTLS */

    g_tmo_at_objects[0] = get_security_object(bootstrap ? 0 : tmo->server_id, server_uri, pskid, psk_buffer, psk_len, bootstrap);
    if (g_tmo_at_objects[0] == NULL) {
        TMO_AT_LOGE("Failed to create security object");
        close(data->sock);
        if (psk_buffer != NULL) {
            vPortFree(psk_buffer);
        }
        return TMO_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    data->securityObjP = g_tmo_at_objects[0];

    /* should free psk_buffer here */
    if (psk_buffer != NULL) {
        vPortFree(psk_buffer);
    }

    g_tmo_at_objects[1] = get_server_object_ex(tmo->instance_id, tmo->server_id, tmo->binding, tmo->lifetime, false);
    if (g_tmo_at_objects[1] == NULL) {
        TMO_AT_LOGE("Failed to create server object");
        close(data->sock);
        return TMO_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    data->serverObject = g_tmo_at_objects[1];

    g_tmo_at_objects[2] = acc_ctrl_create_object();
    if (g_tmo_at_objects[2] == NULL) {
        TMO_AT_LOGE("Failed to create Access Control object");
        close(data->sock);
        return TMO_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    uint16_t object_ids[] = { LWM2M_SERVER_OBJECT_ID, LWM2M_DEVICE_OBJECT_ID, LWM2M_CONN_MONITOR_OBJECT_ID,
                              LWM2M_FIRMWARE_UPDATE_OBJECT_ID, LWM2M_CELLULAR_CONN_OBJECT_ID };
    uint16_t values[] = { 0x001F, 0x0007, 0x0001, 0x0007, 0x0003 };
    uint16_t i;

    for (i = 0; i < sizeof(object_ids) / sizeof(uint16_t); i++) {
        if (acc_ctrl_obj_add_inst(g_tmo_at_objects[2], i, object_ids[i], (object_ids[i] == LWM2M_SERVER_OBJECT_ID) ? tmo->instance_id : 0, tmo->server_id) == false) {
            TMO_AT_LOGE("Failed to create Access Control object instance %d", i);
            close(data->sock);
            return TMO_AT_ERRID_CREATE_LWM2M_ERROR;
        }

        if (acc_ctrl_oi_add_ac_val(g_tmo_at_objects[2], i, tmo->server_id, values[i]) == false) {
            TMO_AT_LOGE("Failed to add value for Access Control object instance %d", i);
            close(data->sock);
            return TMO_AT_ERRID_CREATE_LWM2M_ERROR;
        }
    }

    g_tmo_at_objects[3] = get_object_device();
    if (g_tmo_at_objects[3] == NULL) {
        TMO_AT_LOGE("Failed to create device object");
        close(data->sock);
        return TMO_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    g_tmo_at_objects[4] = get_object_conn_m();
    if (g_tmo_at_objects[4] == NULL) {
        TMO_AT_LOGE("Failed to create connectivity monitoring object");
        close(data->sock);
        return TMO_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    g_tmo_at_objects[5] = get_object_firmware();
    if (g_tmo_at_objects[5] == NULL) {
        TMO_AT_LOGE("Failed to create firmware object");
        close(data->sock);
        return TMO_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    g_tmo_at_objects[6] = get_object_cellular_connectivity();
    if (g_tmo_at_objects[6] == NULL) {
        TMO_AT_LOGE("Failed to create cellular connectivity object");
        close(data->sock);
        return TMO_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    lwm2mH = lwm2m_init(data);
    if (lwm2mH == NULL) {
        TMO_AT_LOGE("lwm2m_init() failed");
        close(data->sock);
        return TMO_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    /* notify the lwm2m instance if connectivity monitoring object supported */
    connectivity_moni_set_handle(lwm2mH);

    /* notify the lwm2m instance if firmware object supported */
    firmware_set_instance(lwm2mH);

    if (tmo->regstate == TMO_AT_REGSTATE_REGISTERED) {
        lwm2mH->state = STATE_READY;
    }
    tmo->lwm2m_handle = lwm2mH;
    data->lwm2mH = lwm2mH;

    lwm2mH->mode = CLIENT_MODE;
    lwm2mH->connect_server_callback = lwm2m_atcmd_connect_server;
    lwm2mH->close_connection_callback = lwm2m_atcmd_close_connection;
    lwm2mH->notify_callback = tmo_at_notify_callback;
    lwm2mH->observe_callback = tmo_at_observe_callback;
    lwm2mH->parameter_callback = tmo_at_set_parameter_callback;

    result = lwm2m_configure(lwm2mH, endpoint_name, NULL, NULL, TMO_AT_MAX_OBJECT_COUNT, g_tmo_at_objects);
    if (result != COAP_NO_ERROR) {
        TMO_AT_LOGE("lwm2m_configure() failed: 0x%X", result);
        lwm2m_close(lwm2mH);
        close(data->sock);
        return TMO_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    if (!tmo->is_task_running && !from_deep_sleep) {
        tmo->is_task_running = true;
        xTaskCreate(tmo_at_task_processing,
                    TMO_AT_TASK_NAME,
                    TMO_AT_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                    NULL,
                    TMO_AT_TASK_PRIORITY,
                    NULL);
        TMO_AT_LOGI("create tmo task");
    }

    return tmo_error;
}

int32_t tmo_at_register(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tmo_at_context_t *tmo;
    int32_t tmo_error = TMO_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo = &g_tmo_at_context;
    TMO_AT_LOGI("regstate: %d", tmo->regstate);
    if (tmo->regstate != TMO_AT_REGSTATE_NOT_REGISTER) {
        return TMO_AT_ERRID_ALREADY_REGISTERED_ERROR;
    } else {
        tmo->regstate = TMO_AT_REGSTATE_REGISTERING;
        tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_REGSTATE);
        tmo_error = tmo_at_register_internal(false);
        if (tmo_error != TMO_AT_ERRID_SUCCESS) {
            tmo->regstate = TMO_AT_REGSTATE_NOT_REGISTER;
            tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_REGSTATE);
        }
        return tmo_error;
    }
}

static int32_t tmo_at_register_query(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[TMO_AT_RESPONSE_DATA_LEN];
    tmo_at_context_t *tmo;
    int32_t tmo_error = TMO_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo = &g_tmo_at_context;

    // +TMOREG:<regstate>
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;

    sprintf(response_data, "+TMOREG:%d", (int)tmo->regstate);

    response.pdata = response_data;
    response.length = strlen(response.pdata);

    response.cmd_id = APB_PROXY_INVALID_CMD_ID;
    apb_proxy_send_at_cmd_result(&response);

    return tmo_error;
}

static int32_t tmo_at_update(tmo_at_mode_t mode, uint32_t lifetime, tmo_at_binding_t binding)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tmo_at_context_t *tmo;
    lwm2m_context_t *lwm2mH;
    int32_t tmo_error = TMO_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo = &g_tmo_at_context;
    if (tmo->regstate != TMO_AT_REGSTATE_REGISTERED) {
        return TMO_AT_ERRID_NOT_REGISTER_ERROR;
    }

    lwm2mH = tmo->lwm2m_handle;
    configASSERT(lwm2mH != NULL);

    switch (mode) {
        case TMO_AT_MODE_LIFETIME:
            lwm2m_update_lifetime(lwm2mH, (time_t)lifetime);
            tmo->lifetime = lifetime;
            tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_LIFETIME);
            tmo_at_stop_rtc_timer(tmo);
            tmo_at_start_rtc_timer(tmo);
            break;

        case TMO_AT_MODE_BINDING:
            if (binding == TMO_AT_BINDING_U) {
                lwm2m_update_binding(lwm2mH, BINDING_U);
            } else {
                lwm2m_update_binding(lwm2mH, BINDING_UQ);
            }
            break;

        default:
            break;
    }

    return tmo_error;
}

int32_t tmo_at_deregister(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tmo_at_context_t *tmo;
    int32_t tmo_error = TMO_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo = &g_tmo_at_context;
    if (tmo->regstate == TMO_AT_REGSTATE_NOT_REGISTER) {
        return TMO_AT_ERRID_NOT_REGISTER_ERROR;
    } else if (tmo->regstate == TMO_AT_REGSTATE_REGISTERING) {
        tmo->is_task_running = false;
        tmo->regstate = TMO_AT_REGSTATE_DEREGISTERING;
        tmo_at_write_nvdm(TMO_AT_NVDM_ITEM_ID_REGSTATE);
        return tmo_error;
    }

    lwm2m_deregister(tmo->lwm2m_handle);

    return tmo_error;
}

static int32_t tmo_at_report(uint16_t objectid, uint16_t instanceid, uint16_t resourceid, char *notify_data, bool ack)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tmo_at_context_t *tmo;
    lwm2m_context_t *lwm2mH;
    int32_t tmo_error = TMO_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmo = &g_tmo_at_context;
    if (tmo->regstate != TMO_AT_REGSTATE_REGISTERED) {
        return TMO_AT_ERRID_NOT_REGISTER_ERROR;
    }

    char obj_uri_string[20];
    sprintf(obj_uri_string, "/%d/%d/%d", objectid, instanceid, resourceid);
    lwm2m_uri_t uri;
    lwm2m_stringToUri(obj_uri_string, strlen(obj_uri_string), &uri);

    lwm2mH = tmo->lwm2m_handle;
    configASSERT(lwm2mH != NULL);

    lwm2m_data_notify(lwm2mH, &uri, notify_data, strlen(notify_data));

    if (ack == true) {
        lwm2mH->notify_with_confirm = 1;
    }
    lwm2m_step_quickly(lwm2mH);
    lwm2mH->notify_with_confirm = 0;

    return tmo_error;
}

apb_proxy_status_t apb_proxy_hdlr_tmo_config_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[TMO_AT_RESPONSE_DATA_LEN];
    char *param_buffer = NULL;
    int32_t tmo_error = TMO_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TMO_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char*)pvPortMalloc(parse_cmd->string_len + 1);
            if (param_buffer == NULL) {
                tmo_error = TMO_AT_ERRID_OTHER_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    tmo_error = TMO_AT_ERRID_PARAMETER_NUMBER_ERROR;
                } else {
                    char *param_list[TMO_AT_CMD_PARAM_NUM];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, TMO_AT_CMD_PARAM_NUM);

                    // AT+TMOCONFIG=<Bootstrap_Enabled>,<Sever_IP>,<Port>,<Endpoint_Name>,<lifetime>,<Security_Mode>[,<PSKID>,<PSK>]
                    if (param_num < 6) {
                        tmo_error = TMO_AT_ERRID_PARAMETER_NUMBER_ERROR;
                    } else {
                        uint32_t bootstrap = atoi(param_list[0]);
                        char *server_ip = param_list[1];
                        uint32_t port = atoi(param_list[2]);
                        char *endpoint_name = param_list[3];
                        uint32_t lifetime = atoi(param_list[4]);
                        uint32_t secmode = atoi(param_list[5]);
                        if (bootstrap != 0 && bootstrap != 1) {
                            tmo_error = TMO_AT_ERRID_PARAMETER_VALUE_ERROR;
                        } else if (strlen(server_ip) > TMO_AT_MAX_SERVER_IP_LEN) {
                            tmo_error = TMO_AT_ERRID_PARAMETER_VALUE_ERROR;
                        } else if (lifetime < TMO_AT_MIN_LIFETIME) {
                            tmo_error = TMO_AT_ERRID_PARAMETER_VALUE_ERROR;
                        } else if (secmode != TMO_AT_SECMODE_PRE_SHARED_KEY && secmode != TMO_AT_SECMODE_NO_SECURITY) {
                            /* support Pre-Shared Keys and NoSec mode */
                            tmo_error = TMO_AT_ERRID_PARAMETER_VALUE_ERROR;
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
                        } else if (secmode == TMO_AT_SECMODE_PRE_SHARED_KEY && param_num == 8) {
                            char *pskid = param_list[6];
                            char *psk = param_list[7];
                            if (strlen(pskid) == 0 || strlen(pskid) > TMO_AT_MAX_PSKID_LEN) {
                                tmo_error = TMO_AT_ERRID_PARAMETER_VALUE_ERROR;
                            } else if (strlen(psk) == 0 || strlen(psk) > TMO_AT_MAX_PSK_LEN) {
                                tmo_error = TMO_AT_ERRID_PARAMETER_VALUE_ERROR;
                            } else {
                                tmo_error = tmo_at_config((bool)bootstrap, server_ip, port, endpoint_name, lifetime, (tmo_at_secmode_t)secmode, pskid, psk);
                            }
#endif /* WITH_TINYDTLS */
                        } else if (secmode == TMO_AT_SECMODE_NO_SECURITY && param_num == 6) {
                            tmo_error = tmo_at_config((bool)bootstrap, server_ip, port, endpoint_name, lifetime, (tmo_at_secmode_t)secmode, NULL, NULL);
                        } else {
                            tmo_error = TMO_AT_ERRID_PARAMETER_NUMBER_ERROR;
                        }
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }
            break;

        case APB_PROXY_CMD_MODE_READ:
            // AT+TMOCONFIG?
            tmo_error = tmo_at_config_query();
            break;

        default:
            tmo_error = TMO_AT_ERRID_OTHER_ERROR;
            break;
    }

    if (tmo_error == TMO_AT_ERRID_SUCCESS) {
        response.result_code = APB_PROXY_RESULT_OK;
    } else {
        response.result_code = APB_PROXY_RESULT_CUSTOM_ERROR;
        sprintf(response_data, "+TMO ERROR:%d", (int)tmo_error);
        response.pdata = response_data;
        response.length = strlen(response.pdata);
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_tmo_register_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[TMO_AT_RESPONSE_DATA_LEN];
    int32_t tmo_error = TMO_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TMO_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_ACTIVE:
            // AT+TMOREG
            tmo_error = tmo_at_register();
            break;

        case APB_PROXY_CMD_MODE_READ:
            // AT+TMOREG?
            tmo_error = tmo_at_register_query();
            break;

        default:
            tmo_error = TMO_AT_ERRID_OTHER_ERROR;
            break;
    }

    if (tmo_error == TMO_AT_ERRID_SUCCESS) {
        response.result_code = APB_PROXY_RESULT_OK;
    } else {
        response.result_code = APB_PROXY_RESULT_CUSTOM_ERROR;
        sprintf(response_data, "+TMO ERROR:%d", (int)tmo_error);
        response.pdata = response_data;
        response.length = strlen(response.pdata);
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_tmo_update_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[TMO_AT_RESPONSE_DATA_LEN];
    char *param_buffer = NULL;
    int32_t tmo_error = TMO_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TMO_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char*)pvPortMalloc(parse_cmd->string_len + 1);
            if (param_buffer == NULL) {
                tmo_error = TMO_AT_ERRID_OTHER_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    tmo_error = TMO_AT_ERRID_PARAMETER_NUMBER_ERROR;
                } else {
                    char *param_list[TMO_AT_CMD_PARAM_NUM];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, TMO_AT_CMD_PARAM_NUM);

                    // AT+TMOUPDATE=<mode>,<lifetime or binding>
                    if (param_num != 2) {
                        tmo_error = TMO_AT_ERRID_PARAMETER_NUMBER_ERROR;
                    } else {
                        uint32_t mode = atoi(param_list[0]);
                        if (mode == TMO_AT_MODE_LIFETIME) {
                            uint32_t lifetime = atoi(param_list[1]);
                            if (lifetime < TMO_AT_MIN_LIFETIME) {
                                tmo_error = TMO_AT_ERRID_PARAMETER_VALUE_ERROR;
                            } else {
                                tmo_error = tmo_at_update((tmo_at_mode_t)mode, lifetime, TMO_AT_BINDING_TOTAL);
                            }
                        } else if (mode == TMO_AT_MODE_BINDING) {
                            uint32_t binding = atoi(param_list[1]);
                            if (binding >= TMO_AT_BINDING_TOTAL) {
                                tmo_error = TMO_AT_ERRID_PARAMETER_VALUE_ERROR;
                            } else {
                                tmo_error = tmo_at_update((tmo_at_mode_t)mode, 0, (tmo_at_binding_t)binding);
                            }
                        } else {
                            tmo_error = TMO_AT_ERRID_PARAMETER_VALUE_ERROR;
                        }
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }
            break;

        default:
            tmo_error = TMO_AT_ERRID_OTHER_ERROR;
            break;
    }

    if (tmo_error == TMO_AT_ERRID_SUCCESS) {
        response.result_code = APB_PROXY_RESULT_OK;
    } else {
        response.result_code = APB_PROXY_RESULT_CUSTOM_ERROR;
        sprintf(response_data, "+TMO ERROR:%d", (int)tmo_error);
        response.pdata = response_data;
        response.length = strlen(response.pdata);
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_tmo_deregister_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[TMO_AT_RESPONSE_DATA_LEN];
    int32_t tmo_error = TMO_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TMO_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_ACTIVE:
            // AT+TMODEREG
            tmo_error = tmo_at_deregister();
            break;

        default:
            tmo_error = TMO_AT_ERRID_OTHER_ERROR;
            break;
    }

    if (tmo_error == TMO_AT_ERRID_SUCCESS) {
        response.result_code = APB_PROXY_RESULT_OK;
    } else {
        response.result_code = APB_PROXY_RESULT_CUSTOM_ERROR;
        sprintf(response_data, "+TMO ERROR:%d", (int)tmo_error);
        response.pdata = response_data;
        response.length = strlen(response.pdata);
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_tmo_report_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[TMO_AT_RESPONSE_DATA_LEN];
    char *param_buffer = NULL;
    int32_t tmo_error = TMO_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TMO_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char*)pvPortMalloc(parse_cmd->string_len + 1);
            if (param_buffer == NULL) {
                tmo_error = TMO_AT_ERRID_OTHER_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    tmo_error = TMO_AT_ERRID_PARAMETER_NUMBER_ERROR;
                } else {
                    char *param_list[TMO_AT_CMD_PARAM_NUM];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, TMO_AT_CMD_PARAM_NUM);

                    // AT+TMOREPORT=<objectid>,<instanceid>,<resourceid>,<data>[,<ack>]
                    if (param_num < 4) {
                        tmo_error = TMO_AT_ERRID_PARAMETER_NUMBER_ERROR;
                    } else {
                        uint32_t objectid = atoi(param_list[0]);
                        uint32_t instanceid = atoi(param_list[1]);
                        uint32_t resourceid = atoi(param_list[2]);
                        char *notify_data = param_list[3];
                        if (objectid != LWM2M_CONN_MONITOR_OBJECT_ID) {
                            tmo_error = TMO_AT_ERRID_PARAMETER_VALUE_ERROR;
                        } else if (param_num == 4) {
                            tmo_error = tmo_at_report((uint16_t)objectid, (uint16_t)instanceid, (uint16_t)resourceid, notify_data, false);
                        } else if (param_num == 5) {
                            uint32_t ack = atoi(param_list[4]);
                            if (ack != 0 && ack != 1) {
                                tmo_error = TMO_AT_ERRID_PARAMETER_VALUE_ERROR;
                            } else {
                                tmo_error = tmo_at_report((uint16_t)objectid, (uint16_t)instanceid, (uint16_t)resourceid, notify_data, (bool)ack);
                            }
                        } else {
                            tmo_error = TMO_AT_ERRID_PARAMETER_NUMBER_ERROR;
                        }
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }
            break;

        default:
            tmo_error = TMO_AT_ERRID_OTHER_ERROR;
            break;
    }

    if (tmo_error == TMO_AT_ERRID_SUCCESS) {
        response.result_code = APB_PROXY_RESULT_OK;
    } else {
        response.result_code = APB_PROXY_RESULT_CUSTOM_ERROR;
        sprintf(response_data, "+TMO ERROR:%d", (int)tmo_error);
        response.pdata = response_data;
        response.length = strlen(response.pdata);
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

#endif /* MTK_TMO_CERT_SUPPORT */

