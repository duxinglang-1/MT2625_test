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

#if defined(MTK_CTM2M_SUPPORT)

#include "apb_proxy.h"
#include "apb_proxy_nw_ctm2m_cmd.h"
#include "apb_proxy_nw_cmd_gprot.h"
#include "sockets.h"
#include "liblwm2m.h"
#include "internals.h"
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

#if 0
#define CTM2M_AT_TEST_WITH_CTIOT
#endif

log_create_module(ctm2m_at, PRINT_LEVEL_INFO);

#if !defined(MTK_DEBUG_LEVEL_NONE)
#define CTM2M_AT_LOGI(fmt, args...)     LOG_I(ctm2m_at, "[CTM2M_AT] "fmt, ##args)
#define CTM2M_AT_LOGW(fmt, args...)     LOG_W(ctm2m_at, "[CTM2M_AT] "fmt, ##args)
#define CTM2M_AT_LOGE(fmt, args...)     LOG_E(ctm2m_at, "[CTM2M_AT] "fmt, ##args)
#else
#define CTM2M_AT_LOGI(fmt, args...)
#define CTM2M_AT_LOGW(fmt, args...)
#define CTM2M_AT_LOGE(fmt, args...)
#endif

#define CTM2M_AT_TASK_NAME              "CTM2M_AT"
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
#define CTM2M_AT_TASK_STACK_SIZE        (1024 * 6)
#else
#define CTM2M_AT_TASK_STACK_SIZE        (1024 * 4)
#endif
#define CTM2M_AT_TASK_PRIORITY          (TASK_PRIORITY_NORMAL)

#define CTM2M_AT_CMD_PARAM_NUM          (10)
#define CTM2M_AT_RESPONSE_DATA_LEN      (50)
#define CTM2M_AT_MIN_LIFETIME           (300)

#define CTM2M_AT_MAX_SERVER_IP_LEN      (64)
#define CTM2M_AT_MAX_PSKID_LEN          (32)
#if defined(WITH_MBEDTLS)
#define CTM2M_AT_MAX_PSK_LEN            (MBEDTLS_PSK_MAX_LEN * 2)
#else
#define CTM2M_AT_MAX_PSK_LEN            (32)
#endif
#define CTM2M_AT_MAX_IMSI_LEN           (16)
#define CTM2M_AT_MAX_IMEI_LEN           (16)
#define CTM2M_AT_MAX_APN_NAME_LEN       (32)
#define CTM2M_AT_MAX_OBJECT_COUNT       (5)
#define CTM2M_AT_MAX_DATA_BUFFER_SIZE   (1024)

#define CTM2M_AT_SERVER_ID              (100)

#define CTM2M_AT_ERRID_SUCCESS                      (0)
#define CTM2M_AT_ERRID_OTHER_ERROR                  (1)
#define CTM2M_AT_ERRID_PARAMETER_NUMBER_ERROR       (2)
#define CTM2M_AT_ERRID_PARAMETER_VALUE_ERROR        (3)
#define CTM2M_AT_ERRID_NOT_REGISTER_ERROR           (4)
#define CTM2M_AT_ERRID_NOT_SUPPORT_SECMODE          (5)
#define CTM2M_AT_ERRID_UNINITIAL_ERROR              (8)
#define CTM2M_AT_ERRID_DATA_LENGTH_OVERFLOW_ERROR   (14)
#define CTM2M_AT_ERRID_NOT_READY_RECEIVE_ERROR      (15)
#define CTM2M_AT_ERRID_IMSI_ERROR                   (16)
#define CTM2M_AT_ERRID_DATA_LENGTH_ODD_ERROR        (17)
#define CTM2M_AT_ERRID_KEEP_CONNECTING_ERROR        (32)
#define CTM2M_AT_ERRID_CAN_NOT_REGISTER_ERROR       (33)
#define CTM2M_AT_ERRID_CREATE_LWM2M_ERROR           (34)

typedef enum {
    CTM2M_AT_ATTUI_NOT_SEND = 0,
    CTM2M_AT_ATTUI_SEND_TO_SERVER = 1,
    CTM2M_AT_ATTUI_SEND_TO_DEVICE = 2,
    CTM2M_AT_ATTUI_TOTAL
} ctm2m_at_attui_t;

typedef enum {
    CTM2M_AT_SECMODE_NO_SECURITY = 0,
    CTM2M_AT_SECMODE_PRE_SHARED_AES_128_CCM_8 = 1,
    CTM2M_AT_SECMODE_PRE_SHARED_AES_128_CBC_SHA256 = 2,
    CTM2M_AT_SECMODE_TOTAL
} ctm2m_at_secmode_t;

typedef enum {
    CTM2M_AT_REGSTATE_NOT_REGISTER = 0,
    CTM2M_AT_REGSTATE_REGISTERING = 1,
    CTM2M_AT_REGSTATE_REGISTERED = 2,
    CTM2M_AT_REGSTATE_OBSERVED = 3,
    CTM2M_AT_REGSTATE_DEREGISTERING = 4
} ctm2m_at_regstate_t;

typedef enum {
    CTM2M_AT_BINDING_U = 0,
    CTM2M_AT_BINDING_UQ = 1,
    CTM2M_AT_BINDING_TOTAL
} ctm2m_at_binding_t;

typedef enum {
    CTM2M_AT_MODE_CON = 0,
    CTM2M_AT_MODE_NON = 1,
    CTM2M_AT_MODE_TOTAL
} ctm2m_at_mode_t;

typedef struct {
    uint32_t rtc_handle;
    int32_t cid;
} ctm2m_at_rentention_t;

typedef struct {
    bool is_initial;
    client_data_t client_data;
    lwm2m_context_t *lwm2m_handle;
    SemaphoreHandle_t mutex_handle;
    char server_ip[CTM2M_AT_MAX_SERVER_IP_LEN + 1];
    uint32_t port;
    uint32_t lifetime;
    ctm2m_at_attui_t attui;
    ctm2m_at_secmode_t secmode;
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    char pskid[CTM2M_AT_MAX_PSKID_LEN + 1];
    char psk[CTM2M_AT_MAX_PSK_LEN + 1];
#endif /* WITH_TINYDTLS */
    char imsi[CTM2M_AT_MAX_IMSI_LEN + 1];
    char imei[CTM2M_AT_MAX_IMEI_LEN + 1];
    char apn_name[CTM2M_AT_MAX_APN_NAME_LEN + 1];
    bool is_task_running;
    bool is_need_update;
    bool is_need_deep_sleep_handling;
    TimerHandle_t ping_timer;
    uint8_t sleep_handle;
    bool is_locking_sleep;
    ctm2m_at_regstate_t regstate;
    bool observed;
    bool fota_update;
    ctm2m_at_callback user_callback;
    void *user_data;
    ctm2m_at_rentention_t *rentention;
} ctm2m_at_context_t;

static ctm2m_at_context_t g_ctm2m_at_context;
static ATTR_ZIDATA_IN_RETSRAM ctm2m_at_rentention_t g_ctm2m_at_rentention;
static lwm2m_object_t *g_ctm2m_at_objects[CTM2M_AT_MAX_OBJECT_COUNT];

/* NVDM Group Name */
#define CTM2M_AT_NVDM_GROUP_NAME            "ctm2m_at"

/* NVDM Item Name */
#define CTM2M_AT_NVDM_ITEM_NAME_SERVER_IP   "server_ip"
#define CTM2M_AT_NVDM_ITEM_NAME_PORT        "port"
#define CTM2M_AT_NVDM_ITEM_NAME_LIFETIME    "lifetime"
#define CTM2M_AT_NVDM_ITEM_NAME_ATTUI       "attui"
#define CTM2M_AT_NVDM_ITEM_NAME_SECMODE     "secmode"
#define CTM2M_AT_NVDM_ITEM_NAME_PSKID       "pskid"
#define CTM2M_AT_NVDM_ITEM_NAME_PSK         "psk"
#define CTM2M_AT_NVDM_ITEM_NAME_REGSTATE    "regstate"
#define CTM2M_AT_NVDM_ITEM_NAME_OBSERVED    "observed"
#define CTM2M_AT_NVDM_ITEM_NAME_FOTA_U      "fotaU"

/* NVDM Item ID */
#define CTM2M_AT_NVDM_ITEM_ID_SERVER_IP     (0x1)
#define CTM2M_AT_NVDM_ITEM_ID_PORT          (0x2)
#define CTM2M_AT_NVDM_ITEM_ID_ENDPOINT      (0x4)
#define CTM2M_AT_NVDM_ITEM_ID_LIFETIME      (0x8)
#define CTM2M_AT_NVDM_ITEM_ID_ATTUI         (0x10)
#define CTM2M_AT_NVDM_ITEM_ID_SECMODE       (0x20)
#define CTM2M_AT_NVDM_ITEM_ID_PSKID         (0x40)
#define CTM2M_AT_NVDM_ITEM_ID_PSK           (0x80)
#define CTM2M_AT_NVDM_ITEM_ID_REGSTATE      (0x100)
#define CTM2M_AT_NVDM_ITEM_ID_OBSERVED      (0x200)
#define CTM2M_AT_NVDM_ITEM_ID_FOTA_UPDATE   (0x400)

static void ctm2m_at_read_nvdm(uint32_t flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;
    uint32_t len;
    nvdm_status_t status;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;

    if ((flag & CTM2M_AT_NVDM_ITEM_ID_SERVER_IP) == CTM2M_AT_NVDM_ITEM_ID_SERVER_IP) {
        // server_ip
        len = CTM2M_AT_MAX_SERVER_IP_LEN;
        status = nvdm_read_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_SERVER_IP, (uint8_t*)ctm2m->server_ip, &len);
        CTM2M_AT_LOGI("read nvdm [%s: %s] = %d", CTM2M_AT_NVDM_ITEM_NAME_SERVER_IP, ctm2m->server_ip, status);
    }

    if ((flag & CTM2M_AT_NVDM_ITEM_ID_PORT) == CTM2M_AT_NVDM_ITEM_ID_PORT) {
        // port
        len = sizeof(uint32_t);
        status = nvdm_read_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_PORT, (uint8_t*)&ctm2m->port, &len);
        CTM2M_AT_LOGI("read nvdm [%s: %d] = %d", CTM2M_AT_NVDM_ITEM_NAME_PORT, ctm2m->port, status);
    }

    if ((flag & CTM2M_AT_NVDM_ITEM_ID_LIFETIME) == CTM2M_AT_NVDM_ITEM_ID_LIFETIME) {
        // lifetime
        len = sizeof(uint32_t);
        status = nvdm_read_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_LIFETIME, (uint8_t*)&ctm2m->lifetime, &len);
        CTM2M_AT_LOGI("read nvdm [%s: %d] = %d", CTM2M_AT_NVDM_ITEM_NAME_LIFETIME, ctm2m->lifetime, status);
    }

    if ((flag & CTM2M_AT_NVDM_ITEM_ID_ATTUI) == CTM2M_AT_NVDM_ITEM_ID_ATTUI) {
        // attui
        len = sizeof(ctm2m_at_attui_t);
        status = nvdm_read_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_ATTUI, (uint8_t*)&ctm2m->attui, &len);
        CTM2M_AT_LOGI("read nvdm [%s: %d] = %d", CTM2M_AT_NVDM_ITEM_NAME_ATTUI, ctm2m->attui, status);
    }

    if ((flag & CTM2M_AT_NVDM_ITEM_ID_SECMODE) == CTM2M_AT_NVDM_ITEM_ID_SECMODE) {
        // secmode
        len = sizeof(ctm2m_at_secmode_t);
        status = nvdm_read_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_SECMODE, (uint8_t*)&ctm2m->secmode, &len);
        CTM2M_AT_LOGI("read nvdm [%s: %d] = %d", CTM2M_AT_NVDM_ITEM_NAME_SECMODE, ctm2m->secmode, status);
    }

#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    if (ctm2m->secmode == CTM2M_AT_SECMODE_PRE_SHARED_AES_128_CCM_8 || ctm2m->secmode == CTM2M_AT_SECMODE_PRE_SHARED_AES_128_CBC_SHA256) {
        if ((flag & CTM2M_AT_NVDM_ITEM_ID_PSKID) == CTM2M_AT_NVDM_ITEM_ID_PSKID) {
            // pskid
            len = CTM2M_AT_MAX_PSKID_LEN;
            status = nvdm_read_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_PSKID, (uint8_t*)ctm2m->pskid, &len);
            CTM2M_AT_LOGI("read nvdm [%s: %s] = %d", CTM2M_AT_NVDM_ITEM_NAME_PSKID, ctm2m->pskid, status);
        }

        if ((flag & CTM2M_AT_NVDM_ITEM_ID_PSK) == CTM2M_AT_NVDM_ITEM_ID_PSK) {
            // psk
            len = CTM2M_AT_MAX_PSK_LEN;
            status = nvdm_read_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_PSK, (uint8_t*)ctm2m->psk, &len);
            CTM2M_AT_LOGI("read nvdm [%s: %s] = %d", CTM2M_AT_NVDM_ITEM_NAME_PSK, ctm2m->psk, status);
        }
    }
#endif /* WITH_TINYDTLS */

    if ((flag & CTM2M_AT_NVDM_ITEM_ID_REGSTATE) == CTM2M_AT_NVDM_ITEM_ID_REGSTATE) {
        // regstate
        len = sizeof(ctm2m_at_regstate_t);
        status = nvdm_read_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_REGSTATE, (uint8_t*)&ctm2m->regstate, &len);
        CTM2M_AT_LOGI("read nvdm [%s: %d] = %d", CTM2M_AT_NVDM_ITEM_NAME_REGSTATE, ctm2m->regstate, status);
    }

    if ((flag & CTM2M_AT_NVDM_ITEM_ID_OBSERVED) == CTM2M_AT_NVDM_ITEM_ID_OBSERVED) {
        // observed
        len = sizeof(bool);
        status = nvdm_read_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_OBSERVED, (uint8_t*)&ctm2m->observed, &len);
        CTM2M_AT_LOGI("read nvdm [%s: %d] = %d", CTM2M_AT_NVDM_ITEM_NAME_OBSERVED, ctm2m->observed, status);
    }

    if ((flag & CTM2M_AT_NVDM_ITEM_ID_FOTA_UPDATE) == CTM2M_AT_NVDM_ITEM_ID_FOTA_UPDATE) {
        // fota_update
        len = sizeof(bool);
        status = nvdm_read_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_FOTA_U, (uint8_t*)&ctm2m->fota_update, &len);
        CTM2M_AT_LOGI("read nvdm [%s: %d] = %d", CTM2M_AT_NVDM_ITEM_NAME_FOTA_U, ctm2m->fota_update, status);
    }
}

static void ctm2m_at_write_nvdm(uint32_t flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;
    uint32_t len;
    nvdm_status_t status;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;

    if ((flag & CTM2M_AT_NVDM_ITEM_ID_SERVER_IP) == CTM2M_AT_NVDM_ITEM_ID_SERVER_IP) {
        // server_ip
        len = CTM2M_AT_MAX_SERVER_IP_LEN;
        status = nvdm_write_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_SERVER_IP, NVDM_DATA_ITEM_TYPE_STRING, (uint8_t*)ctm2m->server_ip, len);
        CTM2M_AT_LOGI("write nvdm [%s: %s] = %d", CTM2M_AT_NVDM_ITEM_NAME_SERVER_IP, ctm2m->server_ip, status);
    }

    if ((flag & CTM2M_AT_NVDM_ITEM_ID_PORT) == CTM2M_AT_NVDM_ITEM_ID_PORT) {
        // port
        len = sizeof(uint32_t);
        status = nvdm_write_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_PORT, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&ctm2m->port, len);
        CTM2M_AT_LOGI("write nvdm [%s: %d] = %d", CTM2M_AT_NVDM_ITEM_NAME_PORT, ctm2m->port, status);
    }

    if ((flag & CTM2M_AT_NVDM_ITEM_ID_LIFETIME) == CTM2M_AT_NVDM_ITEM_ID_LIFETIME) {
        // lifetime
        len = sizeof(uint32_t);
        status = nvdm_write_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_LIFETIME, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&ctm2m->lifetime, len);
        CTM2M_AT_LOGI("write nvdm [%s: %d] = %d", CTM2M_AT_NVDM_ITEM_NAME_LIFETIME, ctm2m->lifetime, status);
    }

    if ((flag & CTM2M_AT_NVDM_ITEM_ID_ATTUI) == CTM2M_AT_NVDM_ITEM_ID_ATTUI) {
        // attui
        len = sizeof(ctm2m_at_attui_t);
        status = nvdm_write_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_ATTUI, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&ctm2m->attui, len);
        CTM2M_AT_LOGI("write nvdm [%s: %d] = %d", CTM2M_AT_NVDM_ITEM_NAME_ATTUI, ctm2m->attui, status);
    }

    if ((flag & CTM2M_AT_NVDM_ITEM_ID_SECMODE) == CTM2M_AT_NVDM_ITEM_ID_SECMODE) {
        // secmode
        len = sizeof(ctm2m_at_secmode_t);
        status = nvdm_write_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_SECMODE, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&ctm2m->secmode, len);
        CTM2M_AT_LOGI("write nvdm [%s: %d] = %d", CTM2M_AT_NVDM_ITEM_NAME_SECMODE, ctm2m->secmode, status);
    }

#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    if (ctm2m->secmode == CTM2M_AT_SECMODE_PRE_SHARED_AES_128_CCM_8 || ctm2m->secmode == CTM2M_AT_SECMODE_PRE_SHARED_AES_128_CBC_SHA256) {
        if ((flag & CTM2M_AT_NVDM_ITEM_ID_PSKID) == CTM2M_AT_NVDM_ITEM_ID_PSKID) {
            // pskid
            len = CTM2M_AT_MAX_PSKID_LEN;
            status = nvdm_write_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_PSKID, NVDM_DATA_ITEM_TYPE_STRING, (uint8_t*)ctm2m->pskid, len);
            CTM2M_AT_LOGI("write nvdm [%s: %s] = %d", CTM2M_AT_NVDM_ITEM_NAME_PSKID, ctm2m->pskid, status);
        }

        if ((flag & CTM2M_AT_NVDM_ITEM_ID_PSK) == CTM2M_AT_NVDM_ITEM_ID_PSK) {
            // psk
            len = CTM2M_AT_MAX_PSK_LEN;
            status = nvdm_write_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_PSK, NVDM_DATA_ITEM_TYPE_STRING, (uint8_t*)ctm2m->psk, len);
            CTM2M_AT_LOGI("write nvdm [%s: %s] = %d", CTM2M_AT_NVDM_ITEM_NAME_PSK, ctm2m->psk, status);
        }
    }
#endif /* WITH_TINYDTLS */

    if ((flag & CTM2M_AT_NVDM_ITEM_ID_REGSTATE) == CTM2M_AT_NVDM_ITEM_ID_REGSTATE) {
        // regstate
        len = sizeof(ctm2m_at_regstate_t);
        status = nvdm_write_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_REGSTATE, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&ctm2m->regstate, len);
        CTM2M_AT_LOGI("write nvdm [%s: %d] = %d", CTM2M_AT_NVDM_ITEM_NAME_REGSTATE, ctm2m->regstate, status);
    }

    if ((flag & CTM2M_AT_NVDM_ITEM_ID_OBSERVED) == CTM2M_AT_NVDM_ITEM_ID_OBSERVED) {
        // observed
        len = sizeof(bool);
        status = nvdm_write_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_OBSERVED, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&ctm2m->observed, len);
        CTM2M_AT_LOGI("write nvdm [%s: %d] = %d", CTM2M_AT_NVDM_ITEM_NAME_OBSERVED, ctm2m->observed, status);
    }

    if ((flag & CTM2M_AT_NVDM_ITEM_ID_FOTA_UPDATE) == CTM2M_AT_NVDM_ITEM_ID_FOTA_UPDATE) {
        // fota_update
        len = sizeof(bool);
        status = nvdm_write_data_item(CTM2M_AT_NVDM_GROUP_NAME, CTM2M_AT_NVDM_ITEM_NAME_FOTA_U, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&ctm2m->fota_update, len);
        CTM2M_AT_LOGI("write nvdm [%s: %d] = %d", CTM2M_AT_NVDM_ITEM_NAME_FOTA_U, ctm2m->fota_update, status);
    }
}

static void ctm2m_at_restore_notify(bool result);

static void ctm2m_at_rtc_timer_callback(void *user_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;
    if (ctm2m->regstate == CTM2M_AT_REGSTATE_REGISTERED || ctm2m->regstate == CTM2M_AT_REGSTATE_OBSERVED) {
        ctm2m->is_need_update = true;
        if (!ctm2m->is_locking_sleep) {
            xTimerStartFromISR(ctm2m->ping_timer, &xHigherPriorityTaskWoken);
            ctm2m->is_locking_sleep = true;
            hal_sleep_manager_acquire_sleeplock(ctm2m->sleep_handle, HAL_SLEEP_LOCK_DEEP);
        }
    }
}

static void ctm2m_at_start_rtc_timer(ctm2m_at_context_t *ctm2m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    rtc_sw_timer_status_t status;
    uint32_t lifetime;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (ctm2m->lifetime <= 30) {
        lifetime = ctm2m->lifetime / 2; /* lifetime 30s -> update 15s */
    } else if (ctm2m->lifetime <= 50) {
        lifetime = 15 + (ctm2m->lifetime - 30) * 3 / 4; /* lifetime 50s -> update 30s */
    } else if (ctm2m->lifetime <= 100) {
        lifetime = 30 + (ctm2m->lifetime - 50) * 4 / 5; /* lifetime 100s -> update 70s */
    } else if (ctm2m->lifetime <= 300) {
        lifetime = 70 + (ctm2m->lifetime - 100) * 9 / 10; /* lifetime 300s -> update 250s */
    } else {
        lifetime = 250 + (ctm2m->lifetime - 300) * 19 / 20;
    }

    status = rtc_sw_timer_create(&ctm2m->rentention->rtc_handle, lifetime * 10, true, ctm2m_at_rtc_timer_callback);
    CTM2M_AT_LOGI("rtc_sw_timer_create = %d, time_out_100ms = %d", (int)status, (int)(lifetime * 10));
    status = rtc_sw_timer_start(ctm2m->rentention->rtc_handle);
    CTM2M_AT_LOGI("rtc_sw_timer_start = %d", (int)status);
}

static void ctm2m_at_stop_rtc_timer(ctm2m_at_context_t *ctm2m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    rtc_sw_timer_status_t status;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    status = rtc_sw_timer_stop(ctm2m->rentention->rtc_handle);
    CTM2M_AT_LOGI("rtc_sw_timer_stop = %d", (int)status);
    status = rtc_sw_timer_delete(ctm2m->rentention->rtc_handle);
    CTM2M_AT_LOGI("rtc_sw_timer_delete = %d", (int)status);
}

static int32_t ctm2m_at_pdn_context_notify_callback(ril_urc_id_t event_id, void *param, uint32_t param_len);
static int32_t ctm2m_at_register(uint32_t lifetime, bool from_deep_sleep);

static void ctm2m_at_ping_timeout_callback(TimerHandle_t xTimer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;
    if (ctm2m->is_locking_sleep) {
        ctm2m->is_locking_sleep = false;
        hal_sleep_manager_release_sleeplock(ctm2m->sleep_handle, HAL_SLEEP_LOCK_DEEP);
    }
}

static void ctm2m_at_task_processing(void *arg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;
    lwm2m_context_t *lwm2mH;
    client_data_t *data;
    int result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;

    if (ctm2m->is_need_deep_sleep_handling) {
        ctm2m->is_need_deep_sleep_handling = false;
        // register with ready state
        configASSERT(ctm2m_at_register(ctm2m->lifetime, true) == CTM2M_AT_ERRID_SUCCESS);
        lwm2mH = ctm2m->lwm2m_handle;
        configASSERT(lwm2mH != NULL);
        lwm2mH->ext_reg_update = 1;
        // notify device or server if exiting from PSM state
        if (ctm2m->attui == CTM2M_AT_ATTUI_SEND_TO_SERVER) {
            ctm2m->is_need_update = false;
            lwm2m_update_registration(lwm2mH, CTM2M_AT_SERVER_ID, false);
        } else if (ctm2m->attui == CTM2M_AT_ATTUI_SEND_TO_DEVICE) {
            lwm2mH->notify_callback(LWM2M_NOTIFY_TYPE_PSM, LWM2M_NOTIFY_CODE_SUCCESS, 0);
        }
    } else if (ctm2m_at_is_restoring()) {
        // register with ready state
        configASSERT(ctm2m_at_register(ctm2m->lifetime, true) == CTM2M_AT_ERRID_SUCCESS);
        ctm2m_at_restore_notify(true);
        lwm2mH = ctm2m->lwm2m_handle;
        configASSERT(lwm2mH != NULL);
        lwm2mH->ext_reg_update = 1;
        ctm2m_at_start_rtc_timer(ctm2m);
        ctm2m->is_need_update = true;
    } else {
        lwm2mH = ctm2m->lwm2m_handle;
        configASSERT(lwm2mH != NULL);
        lwm2mH->ext_reg_update = 1;
        ctm2m_at_start_rtc_timer(ctm2m);
        ctm2m->regstate = CTM2M_AT_REGSTATE_REGISTERING;
        ctm2m_at_write_nvdm(CTM2M_AT_NVDM_ITEM_ID_REGSTATE);
    }

    data = (client_data_t*)lwm2mH->userData;
    configASSERT(data != NULL);

    while (ctm2m->is_task_running) {
        struct timeval tv;
        fd_set readfds;

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        if (ctm2m->is_need_update) {
            ctm2m->is_need_update = false;
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
            CTM2M_AT_LOGE("lwm2m_step() failed: 0x%X", result);
            lwm2mH->notify_callback(LWM2M_NOTIFY_TYPE_LWSTATUS, LWM2M_NOTIFY_CODE_SUCCESS, 0);
        }

#ifdef WITH_MBEDTLS
        mbedtls_connection_t *connP = data->connList;
        if (connP != NULL && connP->dtlsSession != 0) {
            int result = connection_handle_packet(connP);
            if (result < 0) {
                CTM2M_AT_LOGE("error handling message %d", result);
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
                CTM2M_AT_LOGE("Error in select(): %d %s", errno, strerror(errno));
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
                numBytes = recvfrom(data->sock, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&addr, &addrLen);

                if (numBytes < 0) {
                    CTM2M_AT_LOGE("Error in recvfrom(): %d %s", errno, strerror(errno));
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
                        struct sockaddr_in *saddr = (struct sockaddr_in *)&addr;
                        inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET6_ADDRSTRLEN);
                    } else if (AF_INET6 == addr.ss_family) {
                        struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)&addr;
                        inet_ntop(saddr->sin6_family, &saddr->sin6_addr, s, INET6_ADDRSTRLEN);
                    }

                    CTM2M_AT_LOGI("%d bytes received from [%s]", numBytes, s);

                    connP = connection_find(data->connList, &addr, addrLen);
                    if (connP != NULL) {
                        /*
                         * Let liblwm2m respond to the query depending on the context
                         */
#ifdef WITH_TINYDTLS
                        int result = connection_handle_packet(connP, buffer, numBytes);
                        if (result != 0) {
                            CTM2M_AT_LOGE("error handling message %d", result);
                        }
#else
                        lwm2m_handle_packet(lwm2mH, buffer, numBytes, connP);
#endif
                    } else {
                        CTM2M_AT_LOGE("no connection found");
                    }
                }
            }
        }
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }

    ctm2m_at_stop_rtc_timer(ctm2m);

    lwm2m_close(lwm2mH);
    ctm2m->regstate = CTM2M_AT_REGSTATE_NOT_REGISTER;
    ctm2m->observed = false;
    ctm2m_at_write_nvdm(CTM2M_AT_NVDM_ITEM_ID_REGSTATE | CTM2M_AT_NVDM_ITEM_ID_OBSERVED);
    close(data->sock);
    connection_free(data->connList);

    clean_security_object(g_ctm2m_at_objects[0]);
    lwm2m_free(g_ctm2m_at_objects[0]);
    clean_server_object(g_ctm2m_at_objects[1]);
    lwm2m_free(g_ctm2m_at_objects[1]);
    free_object_device(g_ctm2m_at_objects[2]);
    free_object_firmware(g_ctm2m_at_objects[3]);
    free_object_watermeter(g_ctm2m_at_objects[4]);

    vTaskDelete(NULL);
}

void ctm2m_at_backup(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;
    ctm2m->fota_update = true;
    ctm2m_at_write_nvdm(CTM2M_AT_NVDM_ITEM_ID_FOTA_UPDATE);
}

bool ctm2m_at_restore(ctm2m_at_callback user_callback, void *user_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;
    ctm2m->user_callback = user_callback;
    ctm2m->user_data = user_data;

    CTM2M_AT_LOGI("user_callback = 0x%x, user_data = 0x%x", (unsigned int)user_callback, (unsigned int)user_data);

    if (ctm2m->regstate == CTM2M_AT_REGSTATE_REGISTERED || ctm2m->regstate == CTM2M_AT_REGSTATE_OBSERVED) {
        ctm2m_at_restore_notify(true);
    }

    return true;
}

static void ctm2m_at_restore_notify(bool result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;
    ctm2m->fota_update = false;
    ctm2m_at_write_nvdm(CTM2M_AT_NVDM_ITEM_ID_FOTA_UPDATE);
    if (ctm2m->user_callback != NULL) {
        ctm2m->user_callback(result, ctm2m->user_data);
        ctm2m->user_callback = NULL;
    }
}

bool ctm2m_at_is_restoring(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;
    return ctm2m->fota_update;
}

void ctm2m_at_task_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;
    ril_status_t status;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;
    ctm2m->rentention = &g_ctm2m_at_rentention;
    ctm2m->mutex_handle = xSemaphoreCreateMutex();
    configASSERT(ctm2m->mutex_handle != NULL);

    ctm2m->ping_timer = xTimerCreate("ctm2m_ping", 1000 * COAP_MAX_TRANSMIT_WAIT / portTICK_PERIOD_MS, pdFALSE, NULL, ctm2m_at_ping_timeout_callback);
    configASSERT(ctm2m->ping_timer != NULL);

    ctm2m->sleep_handle = hal_sleep_manager_set_sleep_handle("ctm2m_at");
    configASSERT(ctm2m->sleep_handle != SLEEP_LOCK_INVALID_ID);

    status = ril_register_event_callback(RIL_ALL, ctm2m_at_pdn_context_notify_callback);
    configASSERT(status == RIL_STATUS_SUCCESS);

    if (rtc_power_on_result_external() != DEEP_SLEEP && rtc_power_on_result_external() != DEEPER_SLEEP) {
        ctm2m_at_read_nvdm(CTM2M_AT_NVDM_ITEM_ID_FOTA_UPDATE);
        CTM2M_AT_LOGI("fota update handling, fota_update: %d", ctm2m->fota_update);
        /* COLD-BOOT case: normal init */
        if (ctm2m->fota_update == true) {
            ctm2m_at_read_nvdm(CTM2M_AT_NVDM_ITEM_ID_REGSTATE | CTM2M_AT_NVDM_ITEM_ID_OBSERVED);
        } else {
            ctm2m->regstate = CTM2M_AT_REGSTATE_NOT_REGISTER;
            ctm2m->observed = false;
            ctm2m_at_write_nvdm(CTM2M_AT_NVDM_ITEM_ID_REGSTATE | CTM2M_AT_NVDM_ITEM_ID_OBSERVED);
        }
    } else {
        /* DEEP-SLEEP case: data retention process */
        ctm2m_at_read_nvdm(CTM2M_AT_NVDM_ITEM_ID_REGSTATE | CTM2M_AT_NVDM_ITEM_ID_OBSERVED);
        CTM2M_AT_LOGI("deep sleep handling, regstate: %d", ctm2m->regstate);
        if (ctm2m->regstate != CTM2M_AT_REGSTATE_NOT_REGISTER) {
            ctm2m->is_need_deep_sleep_handling = true;
            if (!ctm2m->is_task_running) {
                ctm2m->is_task_running = true;
                xTaskCreate(ctm2m_at_task_processing,
                            CTM2M_AT_TASK_NAME,
                            CTM2M_AT_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                            NULL,
                            CTM2M_AT_TASK_PRIORITY,
                            NULL);
                CTM2M_AT_LOGI("create ctm2m task");
            }
        }
    }
}

static int32_t ctm2m_at_initial(char *server_ip, uint32_t port, uint32_t lifetime, ctm2m_at_attui_t attui, ctm2m_at_secmode_t secmode, char *pskid, char *psk)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;
    int32_t ctm2m_error = CTM2M_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;
    if (ctm2m->regstate != CTM2M_AT_REGSTATE_NOT_REGISTER) {
        return CTM2M_AT_ERRID_KEEP_CONNECTING_ERROR;
    }

    ctm2m->is_initial = true;
    strcpy(ctm2m->server_ip, server_ip);
    ctm2m->port = port;
    ctm2m->lifetime = lifetime;
    ctm2m->attui = attui;
    ctm2m->secmode = secmode;
    if (pskid != NULL) {
        strcpy(ctm2m->pskid, pskid);
    } else {
        ctm2m->pskid[0] = '\0';
    }
    if (psk != NULL) {
        strcpy(ctm2m->psk, psk);
    } else {
        ctm2m->psk[0] = '\0';
    }

    ctm2m_at_write_nvdm(
        CTM2M_AT_NVDM_ITEM_ID_SERVER_IP | CTM2M_AT_NVDM_ITEM_ID_PORT | CTM2M_AT_NVDM_ITEM_ID_ENDPOINT | CTM2M_AT_NVDM_ITEM_ID_LIFETIME |
        CTM2M_AT_NVDM_ITEM_ID_ATTUI | CTM2M_AT_NVDM_ITEM_ID_SECMODE | CTM2M_AT_NVDM_ITEM_ID_PSKID | CTM2M_AT_NVDM_ITEM_ID_PSK);

    return ctm2m_error;
}

static int32_t ctm2m_at_initial_query(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    char response_data[CTM2M_AT_MAX_SERVER_IP_LEN + CTM2M_AT_MAX_PSKID_LEN + CTM2M_AT_MAX_PSK_LEN + CTM2M_AT_RESPONSE_DATA_LEN];
#else
    char response_data[CTM2M_AT_MAX_SERVER_IP_LEN + CTM2M_AT_RESPONSE_DATA_LEN];
#endif
    ctm2m_at_context_t *ctm2m;
    int32_t ctm2m_error = CTM2M_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;
    if (ctm2m->is_initial == false) {
        return CTM2M_AT_ERRID_UNINITIAL_ERROR;
    }

    // +CTM2MINIT:<Sever_IP>,<Port>,<lifetime>,<Auto TAU Timer Update Indication>,<Security mode>[,(<PSKID>,<PSK>)]
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;

    switch (ctm2m->secmode) {
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
        case CTM2M_AT_SECMODE_PRE_SHARED_AES_128_CCM_8:
        case CTM2M_AT_SECMODE_PRE_SHARED_AES_128_CBC_SHA256:
            sprintf(response_data, "+CTM2MINIT:%s,%d,%d,%d,%d,%s,%s", ctm2m->server_ip, (int)ctm2m->port, (int)ctm2m->lifetime,
                    (int)ctm2m->attui, (int)ctm2m->secmode, ctm2m->pskid, ctm2m->psk);
            break;
#endif /* WITH_TINYDTLS */

        case CTM2M_AT_SECMODE_NO_SECURITY:
            sprintf(response_data, "+CTM2MINIT:%s,%d,%d,%d,%d", ctm2m->server_ip, (int)ctm2m->port, (int)ctm2m->lifetime,
                    (int)ctm2m->attui, (int)ctm2m->secmode);
            break;

        default:
            configASSERT(0);
            break;
    }

    response.pdata = response_data;
    response.length = strlen(response.pdata);

    response.cmd_id = APB_PROXY_INVALID_CMD_ID;
    apb_proxy_send_at_cmd_result(&response);

    return ctm2m_error;
}

static void ctm2m_at_notify_callback(lwm2m_notify_type_t type, lwm2m_notify_code_t code, uint32_t option_info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[CTM2M_AT_RESPONSE_DATA_LEN];
    ctm2m_at_context_t *ctm2m;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CTM2M_AT_LOGI("type = %d, code = %d, option_info = %d", (int)type, (int)code, (int)option_info);

    ctm2m = &g_ctm2m_at_context;

    // +CTM2M:<Operation>,<status code>[,<Orig MsgID>]
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;

    switch (type) {
        case LWM2M_NOTIFY_TYPE_CONNECT:
            sprintf(response_data, "+CTM2M:conn,%d", (int)code);
            if (code == LWM2M_NOTIFY_CODE_FAILED) {
                ctm2m->is_task_running = false;
            }
            break;

        case LWM2M_NOTIFY_TYPE_REG:
            sprintf(response_data, "+CTM2M:reg,%d", (int)code);
            if (code == LWM2M_NOTIFY_CODE_SUCCESS) {
                ctm2m->regstate = CTM2M_AT_REGSTATE_REGISTERED;
                ctm2m_at_write_nvdm(CTM2M_AT_NVDM_ITEM_ID_REGSTATE);
            } else {
                ctm2m->is_task_running = false;
            }
            break;

        case LWM2M_NOTIFY_TYPE_UPDATE:
            sprintf(response_data, "+CTM2MUPDATE:%d", (int)option_info);
            break;

        case LWM2M_NOTIFY_TYPE_UPDATE_CONFIRM:
            sprintf(response_data, "+CTM2M:update,%d,%d", (int)code, (int)option_info);
            if (code == LWM2M_NOTIFY_CODE_FORBIDDEN || code == LWM2M_NOTIFY_CODE_NOT_FOUND) {
                ctm2m->is_task_running = false;
                ctm2m->regstate = CTM2M_AT_REGSTATE_DEREGISTERING;
                ctm2m_at_write_nvdm(CTM2M_AT_NVDM_ITEM_ID_REGSTATE);
            }
            break;

        case LWM2M_NOTIFY_TYPE_PING:
            sprintf(response_data, "+CTM2M:ping,%d", (int)code);
            if (code == LWM2M_NOTIFY_CODE_TIMEOUT || code == LWM2M_NOTIFY_CODE_BAD_REQUEST || code == LWM2M_NOTIFY_CODE_FORBIDDEN || code == LWM2M_NOTIFY_CODE_NOT_FOUND) {
                ctm2m->is_task_running = false;
                ctm2m->regstate = CTM2M_AT_REGSTATE_DEREGISTERING;
                ctm2m_at_write_nvdm(CTM2M_AT_NVDM_ITEM_ID_REGSTATE);
            } else if (code == LWM2M_NOTIFY_CODE_SUCCESS && ctm2m->is_locking_sleep) {
                xTimerStop(ctm2m->ping_timer, 0);
                ctm2m->is_locking_sleep = false;
                hal_sleep_manager_release_sleeplock(ctm2m->sleep_handle, HAL_SLEEP_LOCK_DEEP);
            }
            break;

        case LWM2M_NOTIFY_TYPE_DEREG:
            sprintf(response_data, "+CTM2M:dereg,%d", (int)code);
            if (code == LWM2M_NOTIFY_CODE_SUCCESS || code == LWM2M_NOTIFY_CODE_BAD_REQUEST || code == LWM2M_NOTIFY_CODE_FORBIDDEN || code == LWM2M_NOTIFY_CODE_NOT_FOUND) {
                ctm2m->is_task_running = false;
                ctm2m->regstate = CTM2M_AT_REGSTATE_DEREGISTERING;
                ctm2m_at_write_nvdm(CTM2M_AT_NVDM_ITEM_ID_REGSTATE);
            }
            break;

        case LWM2M_NOTIFY_TYPE_SEND:
            sprintf(response_data, "+CTM2MSEND:%d", (int)option_info);
            break;

        case LWM2M_NOTIFY_TYPE_SEND_CONFIRM:
            sprintf(response_data, "+CTM2M:send,%d,%d", (int)code, (int)option_info);
            if (code == LWM2M_NOTIFY_CODE_RST) {
                ctm2m->is_task_running = false;
                ctm2m->regstate = CTM2M_AT_REGSTATE_DEREGISTERING;
                ctm2m_at_write_nvdm(CTM2M_AT_NVDM_ITEM_ID_REGSTATE);
            }
            break;

        case LWM2M_NOTIFY_TYPE_PSM:
            sprintf(response_data, "+CTM2M:psm,%d", (int)code);
            break;

        case LWM2M_NOTIFY_TYPE_LWSTATUS:
            sprintf(response_data, "+CTM2M:lwstatus,%d", (int)code);
            break;

        case LWM2M_NOTIFY_TYPE_REGING:
            ctm2m->regstate = CTM2M_AT_REGSTATE_REGISTERING;
            ctm2m_at_write_nvdm(CTM2M_AT_NVDM_ITEM_ID_REGSTATE);
            return;

        case LWM2M_NOTIFY_TYPE_OBSERVE:
            sprintf(response_data, "+CTM2M:obsrv,%d", (int)code);
            if (code == LWM2M_NOTIFY_CODE_SUCCESS) {
                ctm2m->regstate = CTM2M_AT_REGSTATE_OBSERVED;
                ctm2m->observed = true;
            } else {
                ctm2m->regstate = CTM2M_AT_REGSTATE_REGISTERED;
                ctm2m->observed = false;
            }
            ctm2m_at_write_nvdm(CTM2M_AT_NVDM_ITEM_ID_REGSTATE | CTM2M_AT_NVDM_ITEM_ID_OBSERVED);
            break;

        case LWM2M_NOTIFY_TYPE_DEREGING:
            ctm2m->regstate = CTM2M_AT_REGSTATE_DEREGISTERING;
            ctm2m_at_write_nvdm(CTM2M_AT_NVDM_ITEM_ID_REGSTATE);
            return;

        default:
            return;
    }

    response.pdata = response_data;
    response.length = strlen(response.pdata);

    response.cmd_id = APB_PROXY_INVALID_CMD_ID;
    apb_proxy_send_at_cmd_result(&response);
}

static int32_t ctm2m_at_query_imsi_callback(ril_cmd_response_t *response)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;
    ril_imsi_rsp_t *param;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;
    memset(ctm2m->imsi, 0, sizeof(ctm2m->imsi));

    if (!response || response->res_code != RIL_RESULT_CODE_OK) {
        xSemaphoreGive(ctm2m->mutex_handle);
        return 0;
    }

    param = (ril_imsi_rsp_t*)response->cmd_param;
    if (param && param->imsi) {
        memcpy(ctm2m->imsi, param->imsi, CTM2M_AT_MAX_IMSI_LEN);
    }

    xSemaphoreGive(ctm2m->mutex_handle);
    return 0;
}

static int32_t ctm2m_at_query_imei_callback(ril_cmd_response_t *response)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;
    ril_serial_number_rsp_t *param;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;
    memset(ctm2m->imei, 0, sizeof(ctm2m->imei));

    if (!response || response->res_code != RIL_RESULT_CODE_OK) {
        xSemaphoreGive(ctm2m->mutex_handle);
        return 0;
    }

    param = (ril_serial_number_rsp_t*)response->cmd_param;
    if (param && param->value.imei) {
        memcpy(ctm2m->imei, param->value.imei, CTM2M_AT_MAX_IMEI_LEN);
    }

    xSemaphoreGive(ctm2m->mutex_handle);
    return 0;
}

static int32_t ctm2m_at_query_apn_name_callback(ril_cmd_response_t *response)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;
    ril_define_pdp_context_rsp_t *param;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;
    memset(ctm2m->apn_name, 0, sizeof(ctm2m->apn_name));

    if (!response || response->res_code != RIL_RESULT_CODE_OK) {
        xSemaphoreGive(ctm2m->mutex_handle);
        return 0;
    }

    param = (ril_define_pdp_context_rsp_t*)response->cmd_param;
    if (param && param->array_num > 0) {
        memcpy(ctm2m->apn_name, param->pdp_context[0].apn, CTM2M_AT_MAX_APN_NAME_LEN);
        ctm2m->rentention->cid = param->pdp_context[0].cid;
    }

    xSemaphoreGive(ctm2m->mutex_handle);
    return 0;
}

static int32_t ctm2m_at_pdn_context_notify_callback(ril_urc_id_t event_id, void *param, uint32_t param_len)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;

    if (!param || !param_len) {
        return 0;
    }

    switch (event_id) {
        case RIL_URC_ID_CGEV: {
            ril_cgev_event_reporting_urc_t *cgev_urc = (ril_cgev_event_reporting_urc_t*)param;

            if (cgev_urc->response_type == RIL_URC_TYPE_ME_PDN_DEACT || cgev_urc->response_type == RIL_URC_TYPE_NW_PDN_DEACT) {
                if (ctm2m->rentention->cid == cgev_urc->response1.cid) {
                    ctm2m_at_notify_callback(LWM2M_NOTIFY_TYPE_LWSTATUS, LWM2M_NOTIFY_CODE_SUCCESS, 0);
                }
            } else if (cgev_urc->response_type == RIL_URC_TYPE_ME_PDN_ACT && ctm2m_at_is_restoring()) {
                if (!ctm2m->is_task_running) {
                    ctm2m->is_task_running = true;
                    xTaskCreate(ctm2m_at_task_processing,
                                CTM2M_AT_TASK_NAME,
                                CTM2M_AT_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                                NULL,
                                CTM2M_AT_TASK_PRIORITY,
                                NULL);
                    CTM2M_AT_LOGI("create ctm2m task");
                }
            }
            break;
        }

        default:
            break;
    }

    return 0;
}

static int32_t ctm2m_at_register(uint32_t lifetime, bool from_deep_sleep)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;
    lwm2m_context_t *lwm2mH;
    client_data_t *data;
    int32_t ctm2m_error = CTM2M_AT_ERRID_SUCCESS;
    int result;
    ril_status_t status;
    char server_uri[80];
    char port[20];
    char endpoint_name[60];
    char *pskid = NULL;
    char *psk_buffer = NULL;
    uint16_t psk_len = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (from_deep_sleep == true) {
        ctm2m_at_read_nvdm(
            CTM2M_AT_NVDM_ITEM_ID_SERVER_IP | CTM2M_AT_NVDM_ITEM_ID_PORT | CTM2M_AT_NVDM_ITEM_ID_ENDPOINT | CTM2M_AT_NVDM_ITEM_ID_LIFETIME |
            CTM2M_AT_NVDM_ITEM_ID_ATTUI | CTM2M_AT_NVDM_ITEM_ID_SECMODE | CTM2M_AT_NVDM_ITEM_ID_PSKID | CTM2M_AT_NVDM_ITEM_ID_PSK);
    }

    ctm2m = &g_ctm2m_at_context;
    if (!from_deep_sleep && ctm2m->is_initial == false) {
        return CTM2M_AT_ERRID_UNINITIAL_ERROR;
    }

    if (!from_deep_sleep && ctm2m->regstate != CTM2M_AT_REGSTATE_NOT_REGISTER) {
        return CTM2M_AT_ERRID_CAN_NOT_REGISTER_ERROR;
    }

    if (lifetime == 0) {
        lifetime = ctm2m->lifetime;
    } else {
        ctm2m->lifetime = lifetime;
        ctm2m_at_write_nvdm(CTM2M_AT_NVDM_ITEM_ID_LIFETIME);
    }

    data = &ctm2m->client_data;
    memset(data, 0, sizeof(client_data_t));
    data->addressFamily = AF_INET;

    snprintf(server_uri, sizeof(server_uri), "coap://%s:%d", ctm2m->server_ip, (int)ctm2m->port);
    sprintf(port, "%d", (int)ctm2m->port);

    /* get imsi */
    xSemaphoreTake(ctm2m->mutex_handle, portMAX_DELAY);
    status = ril_request_imsi(RIL_ACTIVE_MODE, ctm2m_at_query_imsi_callback, NULL);
    if (status != RIL_STATUS_SUCCESS) {
        xSemaphoreGive(ctm2m->mutex_handle);
        return CTM2M_AT_ERRID_OTHER_ERROR;
    }

    /* wait for RIL callback */
    xSemaphoreTake(ctm2m->mutex_handle, portMAX_DELAY);
    xSemaphoreGive(ctm2m->mutex_handle);
    if (!ctm2m->imsi[0]) {
        return CTM2M_AT_ERRID_IMSI_ERROR;
    }

    /* get imei */
    xSemaphoreTake(ctm2m->mutex_handle, portMAX_DELAY);
    status = ril_request_serial_number(RIL_EXECUTE_MODE, 1, ctm2m_at_query_imei_callback, NULL);
    if (status != RIL_STATUS_SUCCESS) {
        xSemaphoreGive(ctm2m->mutex_handle);
        return CTM2M_AT_ERRID_OTHER_ERROR;
    }

    /* wait for RIL callback */
    xSemaphoreTake(ctm2m->mutex_handle, portMAX_DELAY);
    xSemaphoreGive(ctm2m->mutex_handle);
    if (!ctm2m->imei[0]) {
        return CTM2M_AT_ERRID_CREATE_LWM2M_ERROR;
    } else {
#ifdef CTM2M_AT_TEST_WITH_CTIOT
        snprintf(endpoint_name, sizeof(endpoint_name), "urn:imei:%s", ctm2m->imei);
#else
        snprintf(endpoint_name, sizeof(endpoint_name), "urn:imei-imsi:%s-%s", ctm2m->imei, ctm2m->imsi);
#endif
    }

    /* get apn_name */
    xSemaphoreTake(ctm2m->mutex_handle, portMAX_DELAY);
    status = ril_request_define_pdp_context(RIL_READ_MODE, NULL, ctm2m_at_query_apn_name_callback, NULL, -1);
    if (status != RIL_STATUS_SUCCESS) {
        xSemaphoreGive(ctm2m->mutex_handle);
        return CTM2M_AT_ERRID_OTHER_ERROR;
    }

    /* wait for RIL callback */
    xSemaphoreTake(ctm2m->mutex_handle, portMAX_DELAY);
    xSemaphoreGive(ctm2m->mutex_handle);
    if (!ctm2m->apn_name[0]) {
        return CTM2M_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    data->sock = create_socket(port, data->addressFamily);
    if (data->sock < 0) {
        CTM2M_AT_LOGE("Failed to open socket: %d %s", errno, strerror(errno));
        return CTM2M_AT_ERRID_CREATE_LWM2M_ERROR;
    }

#if 0
    if (setsockopt(data->sock, SOL_SOCKET, SO_BINDTODEVICE, &ctm2m->rentention->cid, sizeof(ctm2m->rentention->cid)) < 0) {
        CTM2M_AT_LOGE("Failed to bind device: %d %s", errno, strerror(errno));
        close(data->sock);
        return CTM2M_AT_ERRID_CREATE_LWM2M_ERROR;
    }
#endif

#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    if (ctm2m->pskid[0] != '\0') {
        pskid = ctm2m->pskid;
    }

    if (ctm2m->psk[0] != '\0') {
        psk_len = strlen(ctm2m->psk) / 2;
        psk_buffer = pvPortMalloc(psk_len);
        if (psk_buffer == NULL) {
            CTM2M_AT_LOGE("Failed to create PSK binary buffer");
            close(data->sock);
            return CTM2M_AT_ERRID_OTHER_ERROR;
        }

        // Hex string to binary
        char *h = ctm2m->psk;
        char *b = psk_buffer;
        char xlate[] = "0123456789ABCDEF";

        for (; *h; h += 2, ++b) {
            if (h - ctm2m->psk >= psk_len * 2) break;
            char *l = strchr(xlate, (char)toupper((unsigned char)*h));
            char *r = strchr(xlate, (char)toupper((unsigned char)*(h + 1)));

            if (!r || !l) {
                CTM2M_AT_LOGE("Failed to parse Pre-Shared Keys");
                vPortFree(psk_buffer);
                close(data->sock);
                return CTM2M_AT_ERRID_OTHER_ERROR;
            }

            *b = ((l - xlate) << 4) + (r - xlate);
        }
    }
#endif /* WITH_TINYDTLS */

    g_ctm2m_at_objects[0] = get_security_object(CTM2M_AT_SERVER_ID, server_uri, pskid, psk_buffer, psk_len, false);
    if (g_ctm2m_at_objects[0] == NULL) {
        CTM2M_AT_LOGE("Failed to create security object");
        close(data->sock);
        if (psk_buffer != NULL) {
            vPortFree(psk_buffer);
        }
        return CTM2M_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    data->securityObjP = g_ctm2m_at_objects[0];

    /* should free psk_buffer here */
    if (psk_buffer != NULL) {
        vPortFree(psk_buffer);
    }

    g_ctm2m_at_objects[1] = get_server_object(CTM2M_AT_SERVER_ID, "U", lifetime, false);
    if (g_ctm2m_at_objects[1] == NULL) {
        CTM2M_AT_LOGE("Failed to create server object");
        close(data->sock);
        return CTM2M_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    data->serverObject = g_ctm2m_at_objects[1];

    g_ctm2m_at_objects[2] = get_object_device();
    if (g_ctm2m_at_objects[2] == NULL) {
        CTM2M_AT_LOGE("Failed to create device object");
        close(data->sock);
        return CTM2M_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    g_ctm2m_at_objects[3] = get_object_firmware();
    if (g_ctm2m_at_objects[3] == NULL) {
        CTM2M_AT_LOGE("Failed to create firmware object");
        close(data->sock);
        return CTM2M_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    g_ctm2m_at_objects[4] = get_object_watermeter();
    if (g_ctm2m_at_objects[4] == NULL) {
        CTM2M_AT_LOGE("Failed to create object 19");
        close(data->sock);
        return CTM2M_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    lwm2mH = lwm2m_init(data);
    if (lwm2mH == NULL) {
        CTM2M_AT_LOGE("lwm2m_init() failed");
        close(data->sock);
        return CTM2M_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    /* notify firmware the lwm2m instance if firmware object supported */
    firmware_set_instance(lwm2mH);

    if (ctm2m->regstate == CTM2M_AT_REGSTATE_REGISTERED || ctm2m->regstate == CTM2M_AT_REGSTATE_OBSERVED) {
        lwm2mH->state = STATE_READY;
    }
    ctm2m->lwm2m_handle = lwm2mH;
    data->lwm2mH = lwm2mH;

    lwm2mH->mode = CLIENT_MODE;
    lwm2mH->connect_server_callback = lwm2m_atcmd_connect_server;
    lwm2mH->close_connection_callback = lwm2m_atcmd_close_connection;
    lwm2mH->notify_callback = ctm2m_at_notify_callback;

    result = lwm2m_configure_ex(lwm2mH, endpoint_name, NULL, NULL, ctm2m->apn_name, CTM2M_AT_MAX_OBJECT_COUNT, g_ctm2m_at_objects);
    if (result != COAP_NO_ERROR) {
        CTM2M_AT_LOGE("lwm2m_configure_ex() failed: 0x%X", result);
        lwm2m_close(lwm2mH);
        close(data->sock);
        return CTM2M_AT_ERRID_CREATE_LWM2M_ERROR;
    }

    if (!ctm2m->is_task_running) {
        ctm2m->is_task_running = true;
        xTaskCreate(ctm2m_at_task_processing,
                    CTM2M_AT_TASK_NAME,
                    CTM2M_AT_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                    NULL,
                    CTM2M_AT_TASK_PRIORITY,
                    NULL);
        CTM2M_AT_LOGI("create ctm2m task");
    }

    return ctm2m_error;
}

static int32_t ctm2m_at_register_query(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[CTM2M_AT_RESPONSE_DATA_LEN];
    ctm2m_at_context_t *ctm2m;
    int32_t ctm2m_error = CTM2M_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;

    // +CTM2MREG:<regstate>
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;

    sprintf(response_data, "+CTM2MREG:%d", (int)ctm2m->regstate);

    response.pdata = response_data;
    response.length = strlen(response.pdata);

    response.cmd_id = APB_PROXY_INVALID_CMD_ID;
    apb_proxy_send_at_cmd_result(&response);

    return ctm2m_error;
}

static int32_t ctm2m_at_update(ctm2m_at_binding_t binding)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;
    lwm2m_context_t *lwm2mH;
    int32_t ctm2m_error = CTM2M_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;
    if (ctm2m->regstate != CTM2M_AT_REGSTATE_REGISTERED && ctm2m->regstate != CTM2M_AT_REGSTATE_OBSERVED) {
        return CTM2M_AT_ERRID_NOT_REGISTER_ERROR;
    }

    lwm2mH = ctm2m->lwm2m_handle;
    configASSERT(lwm2mH != NULL);

    if (binding == CTM2M_AT_BINDING_U) {
        lwm2m_update_binding(lwm2mH, BINDING_U);
    } else {
        lwm2m_update_binding(lwm2mH, BINDING_UQ);
    }

    return ctm2m_error;
}

static int32_t ctm2m_at_deregister(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;
    int32_t ctm2m_error = CTM2M_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;
    if (ctm2m->regstate == CTM2M_AT_REGSTATE_NOT_REGISTER) {
        return CTM2M_AT_ERRID_NOT_REGISTER_ERROR;
    } else if (ctm2m->regstate == CTM2M_AT_REGSTATE_REGISTERING) {
        ctm2m->is_task_running = false;
        ctm2m->regstate = CTM2M_AT_REGSTATE_DEREGISTERING;
        ctm2m_at_write_nvdm(CTM2M_AT_NVDM_ITEM_ID_REGSTATE);
        return ctm2m_error;
    }

    lwm2m_deregister(ctm2m->lwm2m_handle);

    return ctm2m_error;
}

static int32_t ctm2m_at_send(char *send_data, ctm2m_at_mode_t mode)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ctm2m_at_context_t *ctm2m;
    lwm2m_context_t *lwm2mH;
    int32_t ctm2m_error = CTM2M_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;
    if (ctm2m->regstate == CTM2M_AT_REGSTATE_REGISTERED) {
        return CTM2M_AT_ERRID_NOT_READY_RECEIVE_ERROR;
    } else if (ctm2m->regstate != CTM2M_AT_REGSTATE_OBSERVED) {
        return CTM2M_AT_ERRID_NOT_REGISTER_ERROR;
    }

    char *obj_uri_string = "/19/0/0";
    lwm2m_uri_t uri;
    lwm2m_stringToUri(obj_uri_string, strlen(obj_uri_string), &uri);

    uint8_t *config_bin;
    uint32_t config_len = strlen(send_data) / 2;
#ifdef CTM2M_AT_TEST_WITH_CTIOT
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
#else /* CTM2M_AT_TEST_WITH_CTIOT */
    config_bin = (uint8_t *)pvPortMalloc(config_len + 1);
    config_len = onenet_at_hex_to_bin(config_bin, send_data, config_len);
#endif /* CTM2M_AT_TEST_WITH_CTIOT */

    ctm2m = &g_ctm2m_at_context;
    lwm2mH = ctm2m->lwm2m_handle;
    configASSERT(lwm2mH != NULL);

    lwm2m_data_notify(lwm2mH, &uri, (char*)config_bin, config_len);
    vPortFree(config_bin);

    if (mode == CTM2M_AT_MODE_CON) {
        lwm2mH->notify_with_confirm = 1;
    }
    lwm2m_step_quickly(lwm2mH);
    lwm2mH->notify_with_confirm = 0;

    return ctm2m_error;
}

void ctm2m_at_receive(uint8_t *data, uint32_t data_len)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char *response_data;
    ctm2m_at_context_t *ctm2m;
    lwm2m_context_t *lwm2mH;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctm2m = &g_ctm2m_at_context;
    lwm2mH = ctm2m->lwm2m_handle;
    if (lwm2mH == NULL) {
        return;
    }

    // +CTM2MRECV:<Data>
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;

#ifdef CTM2M_AT_TEST_WITH_CTIOT
#ifdef USE_HAIWEI_RAWDATA_PLUGIN
    response_data = (char*)pvPortMalloc(CTM2M_AT_RESPONSE_DATA_LEN + 2 * data_len);
#else
    response_data = (char*)pvPortMalloc(CTM2M_AT_RESPONSE_DATA_LEN + data_len);
#endif
#else
    response_data = (char*)pvPortMalloc(CTM2M_AT_RESPONSE_DATA_LEN + 2 * data_len);
#endif
    if (response_data == NULL) {
        return;
    }

    uint32_t prefix_len = sprintf(response_data, "+CTM2MRECV:");
#ifdef CTM2M_AT_TEST_WITH_CTIOT
#ifdef USE_HAIWEI_RAWDATA_PLUGIN
    onenet_at_bin_to_hex(response_data + prefix_len, data, 2 * data_len);
#else /* USE_HAIWEI_RAWDATA_PLUGIN */
    if (data_len > 3) {
        /* 1 byte message_id, 2 bytes length */
        memcpy(response_data + prefix_len, data + 3, data_len - 3);
        response_data[prefix_len + data_len - 3] = '\0';
    }
#endif /* USE_HAIWEI_RAWDATA_PLUGIN */
#else /* CTM2M_AT_TEST_WITH_CTIOT */
    onenet_at_bin_to_hex(response_data + prefix_len, data, 2 * data_len);
#endif /* CTM2M_AT_TEST_WITH_CTIOT */

    response.pdata = response_data;
    response.length = strlen(response.pdata);

    response.cmd_id = APB_PROXY_INVALID_CMD_ID;
    apb_proxy_send_at_cmd_result(&response);

    vPortFree(response_data);
}

apb_proxy_status_t apb_proxy_hdlr_ctm2m_initial_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[CTM2M_AT_RESPONSE_DATA_LEN];
    char *param_buffer = NULL;
    int32_t ctm2m_error = CTM2M_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CTM2M_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char*)pvPortMalloc(parse_cmd->string_len + 1);
            if (param_buffer == NULL) {
                ctm2m_error = CTM2M_AT_ERRID_OTHER_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    ctm2m_error = CTM2M_AT_ERRID_PARAMETER_NUMBER_ERROR;
                } else {
                    char *param_list[CTM2M_AT_CMD_PARAM_NUM];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, CTM2M_AT_CMD_PARAM_NUM);

                    // AT+CTM2MINIT=<Sever_IP>,<Port>,<lifetime>,<Auto TAU Timer Update Indication>,<Security mode>[,(<PSKID>,<PSK>)]
                    if (param_num < 5) {
                        ctm2m_error = CTM2M_AT_ERRID_PARAMETER_NUMBER_ERROR;
                    } else {
                        char *server_ip = param_list[0];
                        uint32_t port = atoi(param_list[1]);
                        uint32_t lifetime = atoi(param_list[2]);
                        uint32_t attui = atoi(param_list[3]);
                        uint32_t secmode = atoi(param_list[4]);
                        if (strlen(server_ip) > CTM2M_AT_MAX_SERVER_IP_LEN) {
                            ctm2m_error = CTM2M_AT_ERRID_PARAMETER_VALUE_ERROR;
                        } else if (lifetime < CTM2M_AT_MIN_LIFETIME) {
                            ctm2m_error = CTM2M_AT_ERRID_PARAMETER_VALUE_ERROR;
                        } else if (attui != CTM2M_AT_ATTUI_NOT_SEND) {
                            ctm2m_error = CTM2M_AT_ERRID_PARAMETER_VALUE_ERROR;
                        } else if (secmode != CTM2M_AT_SECMODE_PRE_SHARED_AES_128_CCM_8 && secmode != CTM2M_AT_SECMODE_PRE_SHARED_AES_128_CBC_SHA256 && secmode != CTM2M_AT_SECMODE_NO_SECURITY) {
                            /* support Pre-Shared Keys and NoSec mode */
                            ctm2m_error = CTM2M_AT_ERRID_PARAMETER_VALUE_ERROR;
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
                        } else if ((secmode == CTM2M_AT_SECMODE_PRE_SHARED_AES_128_CCM_8 || secmode == CTM2M_AT_SECMODE_PRE_SHARED_AES_128_CBC_SHA256) && param_num < 7) {
                            /* should provide PSKID & PSK in the same time */
                            ctm2m_error = CTM2M_AT_ERRID_PARAMETER_NUMBER_ERROR;
#if defined(WITH_MBEDTLS)
                        } else if (secmode == CTM2M_AT_SECMODE_PRE_SHARED_AES_128_CCM_8) {
                            ctm2m_error = CTM2M_AT_ERRID_NOT_SUPPORT_SECMODE;
#endif
#if defined(WITH_TINYDTLS)
                        } else if (secmode == CTM2M_AT_SECMODE_PRE_SHARED_AES_128_CBC_SHA256) {
                            ctm2m_error = CTM2M_AT_ERRID_NOT_SUPPORT_SECMODE;
#endif
                        } else if ((secmode == CTM2M_AT_SECMODE_PRE_SHARED_AES_128_CCM_8 || secmode == CTM2M_AT_SECMODE_PRE_SHARED_AES_128_CBC_SHA256) && param_num == 7) {
                            char *pskid = param_list[5];
                            char *psk = param_list[6];
                            if (strlen(pskid) > CTM2M_AT_MAX_PSKID_LEN) {
                                ctm2m_error = CTM2M_AT_ERRID_PARAMETER_VALUE_ERROR;
                            } else if (strlen(psk) > CTM2M_AT_MAX_PSK_LEN) {
                                ctm2m_error = CTM2M_AT_ERRID_PARAMETER_VALUE_ERROR;
                            } else {
                                ctm2m_error = ctm2m_at_initial(server_ip, port, lifetime, (ctm2m_at_attui_t)attui, (ctm2m_at_secmode_t)secmode, pskid, psk);
                            }
#endif /* WITH_TINYDTLS */
                        } else if (secmode == CTM2M_AT_SECMODE_NO_SECURITY && param_num == 5) {
                            ctm2m_error = ctm2m_at_initial(server_ip, port, lifetime, (ctm2m_at_attui_t)attui, (ctm2m_at_secmode_t)secmode, NULL, NULL);
                        } else {
                            ctm2m_error = CTM2M_AT_ERRID_PARAMETER_NUMBER_ERROR;
                        }
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }
            break;

        case APB_PROXY_CMD_MODE_READ:
            // AT+CTM2MINIT?
            ctm2m_error = ctm2m_at_initial_query();
            break;

        default:
            ctm2m_error = CTM2M_AT_ERRID_OTHER_ERROR;
            break;
    }

    if (ctm2m_error == CTM2M_AT_ERRID_SUCCESS) {
        response.result_code = APB_PROXY_RESULT_OK;
    } else {
        response.result_code = APB_PROXY_RESULT_CUSTOM_ERROR;
        sprintf(response_data, "+CTM2M ERROR:%d", (int)ctm2m_error);
        response.pdata = response_data;
        response.length = strlen(response.pdata);
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_ctm2m_register_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[CTM2M_AT_RESPONSE_DATA_LEN];
    char *param_buffer = NULL;
    int32_t ctm2m_error = CTM2M_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CTM2M_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char*)pvPortMalloc(parse_cmd->string_len + 1);
            if (param_buffer == NULL) {
                ctm2m_error = CTM2M_AT_ERRID_OTHER_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string) {
                    ctm2m_error = CTM2M_AT_ERRID_PARAMETER_NUMBER_ERROR;
                } else if (*(cmd_string + 1) == '\0') {
                    ctm2m_error = ctm2m_at_register(0, false);
                } else {
                    char *param_list[CTM2M_AT_CMD_PARAM_NUM];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, CTM2M_AT_CMD_PARAM_NUM);

                    // AT+CTM2MREG=[<Lifetime>]
                    if (param_num == 0) {
                        ctm2m_error = ctm2m_at_register(0, false);
                    } else if (param_num != 1) {
                        ctm2m_error = CTM2M_AT_ERRID_PARAMETER_NUMBER_ERROR;
                    } else {
                        uint32_t lifetime = atoi(param_list[0]);
                        if (lifetime < CTM2M_AT_MIN_LIFETIME) {
                            ctm2m_error = CTM2M_AT_ERRID_PARAMETER_VALUE_ERROR;
                        } else {
                            ctm2m_error = ctm2m_at_register(lifetime, false);
                        }
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }
            break;

        case APB_PROXY_CMD_MODE_ACTIVE:
            // AT+CTM2MREG
            ctm2m_error = ctm2m_at_register(0, false);
            break;

        case APB_PROXY_CMD_MODE_READ:
            // AT+CTM2MREG?
            ctm2m_error = ctm2m_at_register_query();
            break;

        default:
            ctm2m_error = CTM2M_AT_ERRID_OTHER_ERROR;
            break;
    }

    if (ctm2m_error == CTM2M_AT_ERRID_SUCCESS) {
        response.result_code = APB_PROXY_RESULT_OK;
    } else {
        response.result_code = APB_PROXY_RESULT_CUSTOM_ERROR;
        sprintf(response_data, "+CTM2M ERROR:%d", (int)ctm2m_error);
        response.pdata = response_data;
        response.length = strlen(response.pdata);
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_ctm2m_update_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[CTM2M_AT_RESPONSE_DATA_LEN];
    char *param_buffer = NULL;
    int32_t ctm2m_error = CTM2M_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CTM2M_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char*)pvPortMalloc(parse_cmd->string_len + 1);
            if (param_buffer == NULL) {
                ctm2m_error = CTM2M_AT_ERRID_OTHER_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    ctm2m_error = CTM2M_AT_ERRID_PARAMETER_NUMBER_ERROR;
                } else {
                    char *param_list[CTM2M_AT_CMD_PARAM_NUM];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, CTM2M_AT_CMD_PARAM_NUM);

                    // AT+CTM2MUPDATE=<Binding Mode>
                    if (param_num != 1) {
                        ctm2m_error = CTM2M_AT_ERRID_PARAMETER_NUMBER_ERROR;
                    } else {
                        uint32_t binding = atoi(param_list[0]);
                        if (binding >= CTM2M_AT_BINDING_TOTAL) {
                            ctm2m_error = CTM2M_AT_ERRID_PARAMETER_VALUE_ERROR;
                        } else {
                            ctm2m_error = ctm2m_at_update((ctm2m_at_binding_t)binding);
                        }
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }
            break;

        default:
            ctm2m_error = CTM2M_AT_ERRID_OTHER_ERROR;
            break;
    }

    if (ctm2m_error == CTM2M_AT_ERRID_SUCCESS) {
        response.result_code = APB_PROXY_RESULT_OK;
    } else {
        response.result_code = APB_PROXY_RESULT_CUSTOM_ERROR;
        sprintf(response_data, "+CTM2M ERROR:%d", (int)ctm2m_error);
        response.pdata = response_data;
        response.length = strlen(response.pdata);
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_ctm2m_deregister_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[CTM2M_AT_RESPONSE_DATA_LEN];
    int32_t ctm2m_error = CTM2M_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CTM2M_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_ACTIVE:
            // AT+CTM2MDEREG
            ctm2m_error = ctm2m_at_deregister();
            break;

        default:
            ctm2m_error = CTM2M_AT_ERRID_OTHER_ERROR;
            break;
    }

    if (ctm2m_error == CTM2M_AT_ERRID_SUCCESS) {
        response.result_code = APB_PROXY_RESULT_OK;
    } else {
        response.result_code = APB_PROXY_RESULT_CUSTOM_ERROR;
        sprintf(response_data, "+CTM2M ERROR:%d", (int)ctm2m_error);
        response.pdata = response_data;
        response.length = strlen(response.pdata);
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_ctm2m_send_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[CTM2M_AT_RESPONSE_DATA_LEN];
    char *param_buffer = NULL;
    int32_t ctm2m_error = CTM2M_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CTM2M_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char*)pvPortMalloc(parse_cmd->string_len + 1);
            if (param_buffer == NULL) {
                ctm2m_error = CTM2M_AT_ERRID_OTHER_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    ctm2m_error = CTM2M_AT_ERRID_PARAMETER_NUMBER_ERROR;
                } else {
                    char *param_list[CTM2M_AT_CMD_PARAM_NUM];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, CTM2M_AT_CMD_PARAM_NUM);

                    // AT+CTM2MSEND=<Data>[,<mode>]
                    if (param_num == 1) {
                        char *send_data = param_list[0];
                        if (strlen(send_data) % 2 != 0) {
                            ctm2m_error = CTM2M_AT_ERRID_DATA_LENGTH_ODD_ERROR;
                        } else if (strlen(send_data) > CTM2M_AT_MAX_DATA_BUFFER_SIZE) {
                            ctm2m_error = CTM2M_AT_ERRID_DATA_LENGTH_OVERFLOW_ERROR;
                        } else {
                            ctm2m_error = ctm2m_at_send(send_data, CTM2M_AT_MODE_CON);
                        }
                    } else if (param_num != 2) {
                        ctm2m_error = CTM2M_AT_ERRID_PARAMETER_NUMBER_ERROR;
                    } else {
                        char *send_data = param_list[0];
                        uint32_t mode = atoi(param_list[1]);
                        if (mode >= CTM2M_AT_MODE_TOTAL) {
                            ctm2m_error = CTM2M_AT_ERRID_PARAMETER_VALUE_ERROR;
                        } else if (strlen(send_data) % 2 != 0) {
                            ctm2m_error = CTM2M_AT_ERRID_DATA_LENGTH_ODD_ERROR;
                        } else if (strlen(send_data) > CTM2M_AT_MAX_DATA_BUFFER_SIZE) {
                            ctm2m_error = CTM2M_AT_ERRID_DATA_LENGTH_OVERFLOW_ERROR;
                        } else {
                            ctm2m_error = ctm2m_at_send(send_data, (ctm2m_at_mode_t)mode);
                        }
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }
            break;

        default:
            ctm2m_error = CTM2M_AT_ERRID_OTHER_ERROR;
            break;
    }

    if (ctm2m_error == CTM2M_AT_ERRID_SUCCESS) {
        response.result_code = APB_PROXY_RESULT_OK;
    } else {
        response.result_code = APB_PROXY_RESULT_CUSTOM_ERROR;
        sprintf(response_data, "+CTM2M ERROR:%d", (int)ctm2m_error);
        response.pdata = response_data;
        response.length = strlen(response.pdata);
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_ctm2m_version_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char response_data[CTM2M_AT_RESPONSE_DATA_LEN];
    int32_t ctm2m_error = CTM2M_AT_ERRID_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CTM2M_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_ACTIVE:
            // AT+CTM2MVER
            ctm2m_error = CTM2M_AT_ERRID_SUCCESS;
            break;

        case APB_PROXY_CMD_MODE_READ:
            // AT+CTM2MVER?
            ctm2m_error = CTM2M_AT_ERRID_SUCCESS;
            break;

        default:
            ctm2m_error = CTM2M_AT_ERRID_OTHER_ERROR;
            break;
    }

    if (ctm2m_error == CTM2M_AT_ERRID_SUCCESS) {
        response.result_code = APB_PROXY_RESULT_OK;
        snprintf(response_data, sizeof(response_data), "+CTM2MVER:%s,%s,%s,%s", LWM2M_VERSION, CT_VERSION, CT_MT, CT_MV);
    } else {
        response.result_code = APB_PROXY_RESULT_CUSTOM_ERROR;
        sprintf(response_data, "+CTM2M ERROR:%d", (int)ctm2m_error);
    }

    response.pdata = response_data;
    response.length = strlen(response.pdata);
    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

#endif /* MTK_CTM2M_SUPPORT */

