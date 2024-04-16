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

#if defined(MTK_ONENET_SUPPORT)

#include "apb_proxy.h"
#include "apb_proxy_nw_dm_cmd.h"
#include "apb_proxy_nw_cmd_gprot.h"
#include "cis_def.h"
#include "cis_api.h"
#include "cis_if_sys.h"
#include "cis_internals.h"
#include "memory_attribute.h"
#include "hal_rtc_external.h"
#include "hal_rtc_internal.h"
#include "nvdm.h"
#include "ril.h"
#include "ril_cmds_def.h"
#include "timers.h"
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_platform.h"

log_create_module(dm_at, PRINT_LEVEL_INFO);

#if !defined(MTK_DEBUG_LEVEL_NONE)
#define DM_AT_LOGI(fmt, args...)     LOG_I(dm_at, "[DM_AT] "fmt, ##args)
#define DM_AT_LOGW(fmt, args...)     LOG_W(dm_at, "[DM_AT] "fmt, ##args)
#define DM_AT_LOGE(fmt, args...)     LOG_E(dm_at, "[DM_AT] "fmt, ##args)
#else
#define DM_AT_LOGI(fmt, args...)
#define DM_AT_LOGW(fmt, args...)
#define DM_AT_LOGE(fmt, args...)
#endif

#define DM_AT_TASK_NAME              "DM_AT"
#define DM_AT_TASK_STACK_SIZE        (1024 * 2)
#define DM_AT_TASK_PRIORITY          (TASK_PRIORITY_NORMAL)

typedef struct {
    void *cis_context;
    uint8_t config_bin[64];
    uint16_t config_len;
    bool is_disabled;
    bool is_task_running;
    bool is_need_update;
    bool is_need_deep_sleep_handling;
    TimerHandle_t ping_timer;
    uint8_t sleep_handle;
    bool is_locking_sleep;
} dm_at_context_t;

typedef struct {
    uint32_t rtc_handle;
    et_client_state_t regstate;
} dm_at_rentention_t;

static dm_at_context_t g_dm_at_context;
static ATTR_ZIDATA_IN_RETSRAM dm_at_rentention_t g_dm_at_rentention;
static Options g_dm_at_dmconfig = {"CMEI_IMEI", "IMSI", "v1.0", "M100000100", "hW59p448439g1jX7qe7ArM276IKAu0fl", 4, "", 5683, 86400, 0, ""};
static const uint8_t g_dm_at_mainconfig[] = {0x13,0x00,0x32,
    0xf1,0x00,0x03,
    0xf2,0x00,0x20,0x05,0x00/*mtu*/,0x11/*Link&bind type*/,0x00/*BS ENABLED*/,0x00,0x00/*vpn*/,0x00,0x00/*username*/,0x00,0x00/*password*/,
    0x00,0x0b/*host length*/,0x31,0x31,0x37,0x2e,0x31,0x36,0x31,0x2e,0x32,0x2e,0x37,0x00,0x04,0x4e,0x55,0x4c,0x4c,
    0xf3,0x00,0x0d,0xea,0x04,0x00,0x00,0x04,0x4e,0x55,0x4c,0x4c};

/* NVDM Group Name */
#define DM_AT_NVDM_GROUP_NAME            "dm_at"

/* NVDM Item Name */
#define DM_AT_NVDM_ITEM_NAME_IS_USED     "is_used"
#define DM_AT_NVDM_ITEM_NAME_CONFIG_BIN  "config_bin"
#define DM_AT_NVDM_ITEM_NAME_CONFIG_LEN  "config_len"
#define DM_AT_NVDM_ITEM_NAME_APPKEY      "appkey"
#define DM_AT_NVDM_ITEM_NAME_PWD         "pwd"
#define DM_AT_NVDM_ITEM_NAME_LIFETIME    "lifetime"
#define DM_AT_NVDM_ITEM_NAME_IS_DISABLED "is_disabled"

static void dm_at_read_nvdm(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_at_context_t *dm;
    uint32_t len;
    nvdm_status_t status;
    bool is_used = false;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm = &g_dm_at_context;

    // is_used
    len = sizeof(bool);
    status = nvdm_read_data_item(DM_AT_NVDM_GROUP_NAME, DM_AT_NVDM_ITEM_NAME_IS_USED, (uint8_t*)&(is_used), &len);
    DM_AT_LOGI("read nvdm [%s: %d] = %d", DM_AT_NVDM_ITEM_NAME_IS_USED, is_used, status);
    if (!is_used) {
        memcpy(dm->config_bin, g_dm_at_mainconfig, sizeof(g_dm_at_mainconfig));
        dm->config_len = sizeof(g_dm_at_mainconfig);
#ifndef MTK_DM_ENABLED
        dm->is_disabled = true;
#endif
        return;
    }

    // config_bin
    len = 64;
    status = nvdm_read_data_item(DM_AT_NVDM_GROUP_NAME, DM_AT_NVDM_ITEM_NAME_CONFIG_BIN, (uint8_t*)dm->config_bin, &len);
    DM_AT_LOGI("read nvdm [%s: %s] = %d", DM_AT_NVDM_ITEM_NAME_CONFIG_BIN, dm->config_bin, status);

    // config_len
    len = sizeof(uint16_t);
    status = nvdm_read_data_item(DM_AT_NVDM_GROUP_NAME, DM_AT_NVDM_ITEM_NAME_CONFIG_LEN, (uint8_t*)&(dm->config_len), &len);
    DM_AT_LOGI("read nvdm [%s: %d] = %d", DM_AT_NVDM_ITEM_NAME_CONFIG_LEN, dm->config_len, status);

    // appkey
    len = 64;
    status = nvdm_read_data_item(DM_AT_NVDM_GROUP_NAME, DM_AT_NVDM_ITEM_NAME_APPKEY, (uint8_t*)g_dm_at_dmconfig.szAppKey, &len);
    DM_AT_LOGI("read nvdm [%s: %s] = %d", DM_AT_NVDM_ITEM_NAME_APPKEY, g_dm_at_dmconfig.szAppKey, status);

    // pwd
    len = 64;
    status = nvdm_read_data_item(DM_AT_NVDM_GROUP_NAME, DM_AT_NVDM_ITEM_NAME_PWD, (uint8_t*)g_dm_at_dmconfig.szPwd, &len);
    DM_AT_LOGI("read nvdm [%s: %s] = %d", DM_AT_NVDM_ITEM_NAME_PWD, g_dm_at_dmconfig.szPwd, status);

    // lifetime
    len = sizeof(int);
    status = nvdm_read_data_item(DM_AT_NVDM_GROUP_NAME, DM_AT_NVDM_ITEM_NAME_LIFETIME, (uint8_t*)&(g_dm_at_dmconfig.nLifetime), &len);
    DM_AT_LOGI("read nvdm [%s: %d] = %d", DM_AT_NVDM_ITEM_NAME_LIFETIME, g_dm_at_dmconfig.nLifetime, status);

    // is_disabled
    len = sizeof(bool);
    status = nvdm_read_data_item(DM_AT_NVDM_GROUP_NAME, DM_AT_NVDM_ITEM_NAME_IS_DISABLED, (uint8_t*)&(dm->is_disabled), &len);
    DM_AT_LOGI("read nvdm [%s: %d] = %d", DM_AT_NVDM_ITEM_NAME_IS_DISABLED, dm->is_disabled, status);
}

static void dm_at_write_nvdm(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_at_context_t *dm;
    uint32_t len;
    nvdm_status_t status;
    bool is_used = true;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm = &g_dm_at_context;

    // is_used
    len = sizeof(bool);
    status = nvdm_write_data_item(DM_AT_NVDM_GROUP_NAME, DM_AT_NVDM_ITEM_NAME_IS_USED, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&(is_used), len);
    DM_AT_LOGI("write nvdm [%s: %d] = %d", DM_AT_NVDM_ITEM_NAME_IS_USED, is_used, status);

    // config_bin
    len = 64;
    status = nvdm_write_data_item(DM_AT_NVDM_GROUP_NAME, DM_AT_NVDM_ITEM_NAME_CONFIG_BIN, NVDM_DATA_ITEM_TYPE_STRING, (uint8_t*)dm->config_bin, len);
    DM_AT_LOGI("write nvdm [%s: %s] = %d", DM_AT_NVDM_ITEM_NAME_CONFIG_BIN, dm->config_bin, status);

    // config_len
    len = sizeof(uint16_t);
    status = nvdm_write_data_item(DM_AT_NVDM_GROUP_NAME, DM_AT_NVDM_ITEM_NAME_CONFIG_LEN, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&(dm->config_len), len);
    DM_AT_LOGI("write nvdm [%s: %d] = %d", DM_AT_NVDM_ITEM_NAME_CONFIG_LEN, dm->config_len, status);

    // appkey
    len = 64;
    status = nvdm_write_data_item(DM_AT_NVDM_GROUP_NAME, DM_AT_NVDM_ITEM_NAME_APPKEY, NVDM_DATA_ITEM_TYPE_STRING, (uint8_t*)g_dm_at_dmconfig.szAppKey, len);
    DM_AT_LOGI("write nvdm [%s: %s] = %d", DM_AT_NVDM_ITEM_NAME_APPKEY, g_dm_at_dmconfig.szAppKey, status);

    // pwd
    len = 64;
    status = nvdm_write_data_item(DM_AT_NVDM_GROUP_NAME, DM_AT_NVDM_ITEM_NAME_PWD, NVDM_DATA_ITEM_TYPE_STRING, (uint8_t*)g_dm_at_dmconfig.szPwd, len);
    DM_AT_LOGI("write nvdm [%s: %s] = %d", DM_AT_NVDM_ITEM_NAME_PWD, g_dm_at_dmconfig.szPwd, status);

    // lifetime
    len = sizeof(int);
    status = nvdm_write_data_item(DM_AT_NVDM_GROUP_NAME, DM_AT_NVDM_ITEM_NAME_LIFETIME, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&(g_dm_at_dmconfig.nLifetime), len);
    DM_AT_LOGI("write nvdm [%s: %d] = %d", DM_AT_NVDM_ITEM_NAME_LIFETIME, g_dm_at_dmconfig.nLifetime, status);

    // is_disabled
    len = sizeof(bool);
    status = nvdm_write_data_item(DM_AT_NVDM_GROUP_NAME, DM_AT_NVDM_ITEM_NAME_IS_DISABLED, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&(dm->is_disabled), len);
    DM_AT_LOGI("write nvdm [%s: %d] = %d", DM_AT_NVDM_ITEM_NAME_IS_DISABLED, dm->is_disabled, status);
}

static void dm_at_rtc_timer_callback(void *user_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_at_context_t *dm;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm = &g_dm_at_context;
    dm->is_need_update = true;
    if (!dm->is_locking_sleep) {
        xTimerStartFromISR(dm->ping_timer, &xHigherPriorityTaskWoken);
        dm->is_locking_sleep = true;
        hal_sleep_manager_acquire_sleeplock(dm->sleep_handle, HAL_SLEEP_LOCK_DEEP);
    }
}

static void dm_at_start_rtc_timer(dm_at_rentention_t *dm_rtcram)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    rtc_sw_timer_status_t status = rtc_sw_timer_create(&dm_rtcram->rtc_handle, g_dm_at_dmconfig.nLifetime * 9, true, dm_at_rtc_timer_callback);
    DM_AT_LOGI("rtc_sw_timer_create = %d", (int)status);
    status = rtc_sw_timer_start(dm_rtcram->rtc_handle);
    DM_AT_LOGI("rtc_sw_timer_start = %d", (int)status);
}

static void dm_at_stop_rtc_timer(dm_at_rentention_t *dm_rtcram)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    rtc_sw_timer_status_t status = rtc_sw_timer_stop(dm_rtcram->rtc_handle);
    DM_AT_LOGI("rtc_sw_timer_stop = %d", (int)status);
    status = rtc_sw_timer_delete(dm_rtcram->rtc_handle);
    DM_AT_LOGI("rtc_sw_timer_delete = %d", (int)status);
}

static void dm_at_task_processing(void *arg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_at_context_t *dm;
    dm_at_rentention_t *dm_rtcram;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm = &g_dm_at_context;
    dm_rtcram = &g_dm_at_rentention;

    if (dm->is_need_deep_sleep_handling) {
        dm->is_need_deep_sleep_handling = false;
        if (dm_rtcram->regstate == PUMP_STATE_READY) {
            dm->cis_context = NULL;
            dm_at_register();
            if (dm->cis_context != NULL) {
                ((st_context_t*)dm->cis_context)->stateStep = PUMP_STATE_CONNECTING;
                ((st_context_t*)dm->cis_context)->ignoreRegistration = true;
            } else {
                dm->is_task_running = false;
            }
        }
    } else {
        dm_at_start_rtc_timer(dm_rtcram);
    }

    while (dm->is_task_running) {
            cis_pump(dm->cis_context);
            dm_rtcram->regstate = ((st_context_t*)dm->cis_context)->stateStep;
            vTaskDelay(1000 / portTICK_RATE_MS);
    }

    dm_at_stop_rtc_timer(dm_rtcram);

    vTaskDelete(NULL);
}

static void dm_at_ping_timeout_callback(TimerHandle_t xTimer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_at_context_t *dm;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm = &g_dm_at_context;
    if (dm->is_locking_sleep) {
        dm->is_locking_sleep = false;
        hal_sleep_manager_release_sleeplock(dm->sleep_handle, HAL_SLEEP_LOCK_DEEP);
        DM_AT_LOGI("hal_sleep_manager_release_sleeplock");
    }
}

void dm_at_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_at_context_t *dm;
    dm_at_rentention_t *dm_rtcram;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm = &g_dm_at_context;
    dm->ping_timer = xTimerCreate("dm_at_timer", 1000 * COAP_MAX_TRANSMIT_WAIT / portTICK_PERIOD_MS, pdFALSE, NULL, dm_at_ping_timeout_callback);
    configASSERT(dm->ping_timer != NULL);
    dm->sleep_handle = hal_sleep_manager_set_sleep_handle("dm_at");
    configASSERT(dm->sleep_handle != SLEEP_LOCK_INVALID_ID);

    dm_rtcram = &g_dm_at_rentention;

    if (rtc_power_on_result_external() != DEEP_SLEEP && rtc_power_on_result_external() != DEEPER_SLEEP) {
        /* COLD-BOOT case: normal init */
        dm_rtcram->regstate = PUMP_STATE_UNREGISTER;
        char ver[15];
        char *result = cis_get_version(ver);
        DM_AT_LOGI("version %s", result);
        memcpy(g_dm_at_dmconfig.szDMv, ver, strlen(ver) + 1);
    } else {
        /* DEEP-SLEEP case: data retention process */
        DM_AT_LOGI("deep sleep handling, regstate: %d", dm_rtcram->regstate);
        if (dm_rtcram->regstate == PUMP_STATE_READY) {
            dm->is_need_deep_sleep_handling = true;
            if (!dm->is_task_running) {
                dm->is_task_running = true;
                xTaskCreate(dm_at_task_processing,
                            DM_AT_TASK_NAME,
                            DM_AT_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                            NULL,
                            DM_AT_TASK_PRIORITY,
                            NULL);
                DM_AT_LOGI("create dm task");
            }
        }
    }
}

static cis_coapret_t dm_at_read_callback(void *context, cis_uri_t *uri, cis_mid_t mid)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return CIS_CALLBACK_CONFORM;
}

static cis_coapret_t dm_at_write_callback(void *context, cis_uri_t *uri, const cis_data_t *value, cis_attrcount_t attrcount, cis_mid_t mid)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return CIS_CALLBACK_CONFORM;
}

static cis_coapret_t dm_at_execute_callback(void *context, cis_uri_t *uri, const uint8_t *buffer, uint32_t length, cis_mid_t mid)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return CIS_CALLBACK_CONFORM;
}

static cis_coapret_t dm_at_observe_callback(void *context, cis_uri_t *uri, bool flag, cis_mid_t mid)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return CIS_CALLBACK_CONFORM;
}

static cis_coapret_t dm_at_discover_callback(void *context, cis_uri_t *uri, cis_mid_t mid)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return CIS_CALLBACK_CONFORM;
}

static cis_coapret_t dm_at_set_parameter_callback(void *context, cis_uri_t *uri, cis_observe_attr_t parameters, cis_mid_t mid)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return CIS_CALLBACK_CONFORM;
}

static void dm_at_event_callback(void *context, cis_evt_t id, void* param)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_at_context_t *dm;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DM_AT_LOGI("id: %d, param: %d", (int)id, (int)param);
    dm = &g_dm_at_context;
    if (id == CIS_EVENT_UPDATE_SUCCESS && dm->is_locking_sleep) {
        xTimerStop(dm->ping_timer, 0);
        hal_sleep_manager_release_sleeplock(dm->sleep_handle, HAL_SLEEP_LOCK_DEEP);
        DM_AT_LOGI("hal_sleep_manager_release_sleeplock");
        dm->is_locking_sleep = false;
    }
    else if ((id == CIS_EVENT_CONNECT_SUCCESS || id == CIS_EVENT_UPDATE_NEED) && dm->is_need_update) {
        dm->is_need_update = false;
        DM_AT_LOGI("cis_update_reg start: 0x%x", (unsigned int)dm->cis_context);
        cis_ret_t cis_ret = cis_update_reg(dm->cis_context, LIFETIME_INVALID, false);
        DM_AT_LOGI("cis_update_reg: %d", (int)cis_ret);
    }
}

int32_t dm_at_register(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_at_context_t *dm;
    cis_callback_t callback;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_at_read_nvdm();

    dm = &g_dm_at_context;
    if (dm->is_disabled) {
        DM_AT_LOGI("dm_at_register = -1");
        return -1;
    }

    DM_AT_LOGI("cis_init start: 0x%x, 0x%x, %d", (unsigned int)dm->cis_context, (unsigned int)dm->config_bin, (int)dm->config_len);
    cis_ret_t cis_ret = cis_init(&dm->cis_context, dm->config_bin, dm->config_len, &g_dm_at_dmconfig, NULL);
    DM_AT_LOGI("cis_init end: %d", (int)cis_ret);
    if (cis_ret != CIS_RET_OK) {
        DM_AT_LOGI("dm_at_register = -2");
        return -2;
    }

    DM_AT_LOGI("szCMEI_IMEI=%s, szIMSI=%s, szDMv=%s", g_dm_at_dmconfig.szCMEI_IMEI, g_dm_at_dmconfig.szIMSI, g_dm_at_dmconfig.szDMv);
    DM_AT_LOGI("szAppKey=%s, szPwd=%s", g_dm_at_dmconfig.szAppKey, g_dm_at_dmconfig.szPwd);
    DM_AT_LOGI("nAddressFamily=%d, nSrvPort=%d, nLifetime=%d, nBootstrap=%d", g_dm_at_dmconfig.nAddressFamily, g_dm_at_dmconfig.nSrvPort, g_dm_at_dmconfig.nLifetime, g_dm_at_dmconfig.nBootstrap);

    if (!dm->is_task_running) {
        dm->is_task_running = true;
        xTaskCreate(dm_at_task_processing,
                    DM_AT_TASK_NAME,
                    DM_AT_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                    NULL,
                    DM_AT_TASK_PRIORITY,
                    NULL);
        DM_AT_LOGI("create dm task");
    }

    callback.onRead = dm_at_read_callback;
    callback.onWrite = dm_at_write_callback;
    callback.onExec = dm_at_execute_callback;
    callback.onObserve = dm_at_observe_callback;
    callback.onDiscover = dm_at_discover_callback;
    callback.onSetParams = dm_at_set_parameter_callback;
    callback.onEvent = dm_at_event_callback;
    cis_register(dm->cis_context, g_dm_at_dmconfig.nLifetime, &callback);

    return 0;
}

int32_t dm_at_deregister(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_at_context_t *dm;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm = &g_dm_at_context;
    cis_unregister(dm->cis_context);

    return 0;
}

apb_proxy_status_t apb_proxy_hdlr_dm_config_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer = NULL;
    dm_at_context_t *dm;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DM_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char *)pvPortMalloc(parse_cmd->string_len + 1);
            if (param_buffer == NULL) {
                response.result_code = APB_PROXY_RESULT_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    response.result_code = APB_PROXY_RESULT_ERROR;
                } else {
                    char *param_list[5];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, 5);

                    // AT+DMCONFIG=<config>,<appkey>,<pwd>,<lifetime>[,<disable>]
                    if (param_num < 4 || param_num > 5) {
                        response.result_code = APB_PROXY_RESULT_ERROR;
                    } else {
                        char *config = param_list[0];
                        char *appkey = param_list[1];
                        char *pwd = param_list[2];
                        int32_t lifetime = atoi(param_list[3]);
                        if (strlen(config) % 2 != 0 || strlen(config) > 128 ||
                            strlen(appkey) > 64 ||
                            strlen(pwd) > 64 ||
                            (lifetime < 50 && param_num == 4)) {
                            response.result_code = APB_PROXY_RESULT_ERROR;
                        } else if (param_num == 5 && param_list[4][0] != '1' && param_list[4][0] != '0') {
                            response.result_code = APB_PROXY_RESULT_ERROR;
                        } else {
                            response.result_code = APB_PROXY_RESULT_OK;
                            dm = &g_dm_at_context;
                            if (strlen(config) == 0) {
                                memcpy(dm->config_bin, g_dm_at_mainconfig, sizeof(g_dm_at_mainconfig));
                                dm->config_len = (uint16_t)sizeof(g_dm_at_mainconfig);
                            } else {
                                dm->config_len = (uint16_t)onenet_at_hex_to_bin(dm->config_bin, config, strlen(config) / 2);
                            }
                            if (strlen(appkey) > 0) {
                                memcpy(g_dm_at_dmconfig.szAppKey, appkey, strlen(appkey));
                            }
                            if (strlen(pwd) > 0) {
                                memcpy(g_dm_at_dmconfig.szPwd, pwd, strlen(pwd));
                            }
                            g_dm_at_dmconfig.nLifetime = lifetime;
                            if (param_num == 5) {
                                dm->is_disabled = (bool)atoi(param_list[4]);
                            } else {
                                dm->is_disabled = false;
                            }
                            dm_at_write_nvdm();
                        }
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }
            break;

        default:
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

#endif /* MTK_ONENET_SUPPORT */

