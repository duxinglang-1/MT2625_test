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


#if defined(MTK_LWM2M_SUPPORT) && defined(MTK_LWM2M_AT_CMD_SUPPORT)

#include "apb_proxy.h"
#include "apb_proxy_nw_cmd.h"
#include "apb_proxy_nw_cmd_util.h"
#include "lwm2m_app_context_manager.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sockets.h"
#include "liblwm2m.h"
#include "lwm2m_objects.h"
#include <stdlib.h>
#include <ctype.h>
#include "syslog.h"
#include "apb_proxy_nw_cmd_gprot.h"
#include "memory_attribute.h"
#include "hal_rtc_external.h"
#include "hal_rtc_internal.h"
#include "nvdm.h"
#include "timers.h"
#include "hal_sleep_manager.h"

// read & notify value
// observe indicate

#ifdef APBSOC_MODULE_PRINTF
#define NW_LOG(fmt, ...)               printf("[APB LWM2M] "fmt, ##__VA_ARGS__)
#else
#define NW_LOG(fmt, ...)               LOG_I(apbnw, "[APB LWM2M] "fmt, ##__VA_ARGS__)
#endif

#define MAX_PACKET_SIZE 1024
#define DEFAULT_SERVER_IPV6 "[::1]"
#define DEFAULT_SERVER_IPV4 "127.0.0.1"
#define LWM2MAPPLOG printf
#define READ_WRITE_TIMEOUT          (5000)
#define DEFAULT_LIFE_TIME           (100)
#define OBJ_COUNT 5

lwm2m_object_t * lwm2m_objArray[SUPPORT_LWM2M_ATCMD_INSTANCE_MAX_NUM][OBJ_COUNT];

typedef struct _prv_instance_
{
    /*
     * The first two are mandatories and represent the pointer to the next instance and the ID of this one. The rest
     * is the instance scope user data (uint8_t test in this case)
     */
    struct _prv_instance_ * next;   // matches lwm2m_list_t::next
    uint16_t shortID;               // matches lwm2m_list_t::id
    uint16_t resource_count;
    uint16_t *resource_ids;
} prv_instance_t;

/* Group Name */
#define LWM2M_ATCMD_NVDM_GROUP_NAME_BASE        "lwm2m_%d"

/* Item Name */
#define LWM2M_ATCMD_NVDM_ITEM_NAME_IS_USED      "is_used"
#define LWM2M_ATCMD_NVDM_ITEM_NAME_LOCAL_PORT   "local_port"
#define LWM2M_ATCMD_NVDM_ITEM_NAME_SERVER       "server"
#define LWM2M_ATCMD_NVDM_ITEM_NAME_SERVER_PORT  "server_port"
#define LWM2M_ATCMD_NVDM_ITEM_NAME_DEVICE       "name"
#define LWM2M_ATCMD_NVDM_ITEM_NAME_PSKID        "pskid"
#define LWM2M_ATCMD_NVDM_ITEM_NAME_PSK          "psk"
#define LWM2M_ATCMD_NVDM_ITEM_NAME_LIFETIME     "lifetime"
#define LWM2M_ATCMD_NVDM_ITEM_NAME_ADDR_FAMILY  "addr_family"

static ATTR_ZIDATA_IN_RETSRAM lwm2m_client_state_t g_lwm2m_atcmd_state;

static uint8_t g_lwm2m_atcmd_number = 0;

static uint8_t g_is_lwm2m_task_started = 0;

static bool g_lwm2m_need_update;

static bool g_lwm2m_need_deep_sleep_handling;

static TimerHandle_t g_lwm2m_ping_timer;

static uint8_t g_lwm2m_sleep_handle;

static bool g_lwm2m_is_locking;

struct lwm2m_atcmd_t g_lwm2m_atcmd_t[SUPPORT_LWM2M_ATCMD_INSTANCE_MAX_NUM] = {
        { 0 },
};

#define LWM2M_ATCMD_OBJECT_MAX_COUNT    (2)
#define LWM2M_ATCMD_RESOURCE_MAX_COUNT  (10)

typedef struct {
    bool is_used;
    uint16_t object_id;
    uint16_t instance_id;
    uint16_t resource_count;
    uint16_t resource_ids[LWM2M_ATCMD_RESOURCE_MAX_COUNT];
} lwm2m_atcmd_object_info_t;

typedef struct {
    lwm2m_atcmd_object_info_t object_info[LWM2M_ATCMD_OBJECT_MAX_COUNT];
    uint32_t rtc_handle;
    bool rtc_enable;
} lwm2m_atcmd_rentention_t;

static ATTR_ZIDATA_IN_RETSRAM lwm2m_atcmd_rentention_t g_lwm2m_atcmd_rentention_t[SUPPORT_LWM2M_ATCMD_INSTANCE_MAX_NUM];

static void lwm2m_atcmd_retention_save_object(uint8_t lwm2m_id, uint16_t object_id, uint16_t instance_id, uint16_t resource_count, uint16_t *resource_ids)
{
    lwm2m_atcmd_rentention_t *lwm2m_saved = &g_lwm2m_atcmd_rentention_t[lwm2m_id];
    for (int i = 0; i < LWM2M_ATCMD_OBJECT_MAX_COUNT; i++) {
        lwm2m_atcmd_object_info_t *object_info = &lwm2m_saved->object_info[i];
        if (object_info->is_used == 0) {
            object_info->is_used = 1;
            object_info->object_id = object_id;
            object_info->instance_id = instance_id;
            if (resource_count > LWM2M_ATCMD_RESOURCE_MAX_COUNT) {
                resource_count = LWM2M_ATCMD_RESOURCE_MAX_COUNT;
                NW_LOG("not support so many resources");
            }
            object_info->resource_count = resource_count;
            memcpy(object_info->resource_ids, resource_ids, resource_count * sizeof(uint16_t));
            return;
        }
    }
}

static void lwm2m_atcmd_retention_delete_object(uint8_t lwm2m_id, uint16_t object_id)
{
    lwm2m_atcmd_rentention_t *lwm2m_saved = &g_lwm2m_atcmd_rentention_t[lwm2m_id];
    for (int i = 0; i < LWM2M_ATCMD_OBJECT_MAX_COUNT; i++) {
        lwm2m_atcmd_object_info_t *object_info = &lwm2m_saved->object_info[i];
        if (object_info->is_used == 1 && object_info->object_id == object_id) {
            object_info->is_used = 0;
            return;
        }
    }
}

static void lwm2m_atcmd_retention_delete_all_objects(uint8_t lwm2m_id)
{
    lwm2m_atcmd_rentention_t *lwm2m_saved = &g_lwm2m_atcmd_rentention_t[lwm2m_id];
    for (int i = 0; i < LWM2M_ATCMD_OBJECT_MAX_COUNT; i++) {
        lwm2m_atcmd_object_info_t *object_info = &lwm2m_saved->object_info[i];
        if (object_info->is_used == 1) {
            object_info->is_used = 0;
        }
    }
}

static void lwm2m_atcmd_rtc_timer_callback(void *user_data)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    g_lwm2m_need_update = true;
    xTimerStartFromISR(g_lwm2m_ping_timer, &xHigherPriorityTaskWoken);
    hal_sleep_manager_acquire_sleeplock(g_lwm2m_sleep_handle, HAL_SLEEP_LOCK_DEEP);
    g_lwm2m_is_locking = true;
}

static void lwm2m_atcmd_start_rtc_timer(struct lwm2m_atcmd_t *lwm2m_t)
{
    lwm2m_atcmd_rentention_t *lwm2m_saved = &g_lwm2m_atcmd_rentention_t[lwm2m_t->lwm2m_id];
    if (lwm2m_saved->rtc_enable == false) {
        rtc_sw_timer_status_t status = rtc_sw_timer_create(&lwm2m_saved->rtc_handle, lwm2m_t->lifetime * 8, true, lwm2m_atcmd_rtc_timer_callback);
        NW_LOG("rtc_sw_timer_create = %d", (int)status);
        status = rtc_sw_timer_start(lwm2m_saved->rtc_handle);
        NW_LOG("rtc_sw_timer_start = %d", (int)status);
        lwm2m_saved->rtc_enable = true;
    } else {
        NW_LOG("lwm2m_atcmd_start_rtc_timer already");
    }
}

static void lwm2m_atcmd_stop_rtc_timer(struct lwm2m_atcmd_t *lwm2m_t)
{
    lwm2m_atcmd_rentention_t *lwm2m_saved = &g_lwm2m_atcmd_rentention_t[lwm2m_t->lwm2m_id];
    if (lwm2m_saved->rtc_enable == true) {
        rtc_sw_timer_status_t status = rtc_sw_timer_stop(lwm2m_saved->rtc_handle);
        NW_LOG("rtc_sw_timer_stop = %d", (int)status);
        status = rtc_sw_timer_delete(lwm2m_saved->rtc_handle);
        NW_LOG("rtc_sw_timer_delete = %d", (int)status);
        lwm2m_saved->rtc_enable = false;
    } else {
        NW_LOG("lwm2m_atcmd_stop_rtc_timer already");
    }
}

uint8_t lwm2m_atcmd_object_observe_callback (uint16_t code, uint16_t msg_id, 
    lwm2m_uri_t * uriP, void* user_data)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};  
    char* result_buf = NULL;  
    struct lwm2m_atcmd_t* lwm2m_t = NULL;
    uint8_t ret = 0;

    lwm2m_t = (struct lwm2m_atcmd_t*)user_data;
    if (NULL == lwm2m_t)
        return COAP_404_NOT_FOUND;

    if (lwm2m_t->is_used == 0)
        return COAP_404_NOT_FOUND;

    result_buf = _alloc_buffer(100);
    if (result_buf == NULL) {
        return COAP_412_PRECONDITION_FAILED;
    } else {
        if (uriP->flag & LWM2M_URI_FLAG_OBJECT_ID) {
            snprintf(result_buf, 100, "+ELMOBSERVE:%d,%d,%d,%d,%d", lwm2m_t->lwm2m_id, code,
                    uriP->objectId,
                    (uriP->flag & LWM2M_URI_FLAG_INSTANCE_ID) ? uriP->instanceId : -1,
                    (uriP->flag & LWM2M_URI_FLAG_RESOURCE_ID) ? uriP->resourceId : -1);

            cmd_result.pdata = result_buf;

            cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
            cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
            cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
            apb_proxy_send_at_cmd_result(&cmd_result);

            ret = COAP_NO_ERROR;
        } else {
            ret = COAP_400_BAD_REQUEST;
        }
    }

    if (result_buf) {
        _free_buffer(result_buf);
    }
    return ret;
}

uint8_t lwm2m_atcmd_object_parameter_callback (lwm2m_uri_t * uriP, lwm2m_attributes_t * attrP, void* user_data)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};  
    char* result_buf = NULL;  
    struct lwm2m_atcmd_t* lwm2m_t = NULL;
    uint8_t ret = 0;

    lwm2m_t = (struct lwm2m_atcmd_t*)user_data;
    if (NULL == lwm2m_t)
        return COAP_404_NOT_FOUND;

    if (lwm2m_t->is_used == 0)
        return COAP_404_NOT_FOUND;
    
    result_buf = _alloc_buffer(150);

    if (result_buf == NULL) {
        ret = COAP_412_PRECONDITION_FAILED;
    } else {
        if (uriP->flag & LWM2M_URI_FLAG_OBJECT_ID) {
            snprintf(result_buf, 100, "+ELMPARAMETER:%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f",
                    lwm2m_t->lwm2m_id,
                    uriP->objectId,
                    (uriP->flag & LWM2M_URI_FLAG_INSTANCE_ID) ? uriP->instanceId : -1,
                    (uriP->flag & LWM2M_URI_FLAG_RESOURCE_ID) ? uriP->resourceId : -1,
                    attrP->toSet, attrP->toClear, (int)attrP->minPeriod, (int)attrP->maxPeriod,
                    attrP->greaterThan, attrP->lessThan, attrP->step);

            cmd_result.pdata = result_buf;
            cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
            cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
            cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
            apb_proxy_send_at_cmd_result(&cmd_result);
            ret = COAP_NO_ERROR;
        } else {
            ret = COAP_400_BAD_REQUEST;
        }
    }

    if (result_buf) {
        _free_buffer(result_buf);
    }
    return ret;
}

uint8_t lwm2m_atcmd_object_read_callback (uint16_t instanceId, int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP)
{ 
    prv_instance_t * targetP;
    apb_proxy_at_cmd_result_t cmd_result = {0};
    char* result_buf = NULL;
    char temp_str[20];
    struct lwm2m_atcmd_t* lwm2m_t = NULL;
    uint8_t ret = 0;
    int i, j;

    targetP = (prv_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    if (NULL == targetP)
        return COAP_404_NOT_FOUND;

    lwm2m_t = (struct lwm2m_atcmd_t*)objectP->userData;
    if (NULL == lwm2m_t)
        return COAP_404_NOT_FOUND;

    if (lwm2m_t->is_used == 0)
        return COAP_404_NOT_FOUND;

    result_buf = _alloc_buffer(100);
    if (result_buf == NULL) {
        NW_LOG("read data in select, memory error !!");
        ret = COAP_412_PRECONDITION_FAILED;
    } else {
        snprintf(result_buf, 100, "+ELMREAD:%d,%d,%d,%d", lwm2m_t->lwm2m_id, objectP->objID,
                targetP->shortID, *numDataP);

        if (*numDataP > 0) {
            for (i = 0; i < *numDataP; i++) {
                snprintf(temp_str, 20, ",%d", ((*dataArrayP) + i)->id);
                strcat(result_buf, temp_str);
            }
        }

        cmd_result.pdata = result_buf;
        cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
        cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
        cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
        apb_proxy_send_at_cmd_result(&cmd_result);

        if (result_buf) {
            _free_buffer(result_buf);
        }

        if (xSemaphoreTake(lwm2m_t->readWriteSemph, READ_WRITE_TIMEOUT / portTICK_PERIOD_MS) == pdTRUE) {
            NW_LOG("lwm2m_atcmd_object_read_callback take readWriteSemph, %d", *numDataP);
            if (*numDataP > 0) {
                if (lwm2m_t->read_result_cnt == *numDataP) {
                    for (i = 0; i < *numDataP; i++) {
                        for (j = 0; j < lwm2m_t->read_result_cnt; j++) {
                            if (((*dataArrayP) + i)->id == (lwm2m_t->read_result_list + j)->id) {
                                memcpy((*dataArrayP) + i, lwm2m_t->read_result_list + j, sizeof(lwm2m_data_t));
                                // because content is copied out, clean old
                                memset(lwm2m_t->read_result_list + j, 0, sizeof(lwm2m_data_t));
                                break;
                            }
                        }
                        if (j == lwm2m_t->read_result_cnt) {
                            NW_LOG("No matched id for read i=%d, id=%d\n", i, ((*dataArrayP) + i)->id);
                        }
                    }
                }
                lwm2m_data_free(lwm2m_t->read_result_cnt, lwm2m_t->read_result_list);
            } else {
                *dataArrayP = lwm2m_t->read_result_list;
                *numDataP = lwm2m_t->read_result_cnt;
            }
            lwm2m_t->read_result_cnt = 0;
            lwm2m_t->read_result_list = NULL;
            ret = COAP_205_CONTENT;
        } else {
            NW_LOG("take lwm2m_t->readWriteSemph failed, timeout");
            ret = COAP_412_PRECONDITION_FAILED;
        }
    }



    return ret;
}

uint8_t lwm2m_atcmd_object_discover_callback (uint16_t instanceId, int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP)
{
    uint8_t result;
    int i, j;
    prv_instance_t *instanceP;

    result = COAP_205_CONTENT;
    instanceP = (prv_instance_t *)LWM2M_LIST_FIND(objectP->instanceList, instanceId);

    NW_LOG("lwm2m_atcmd_object_discover_callback numDataP = %d", *numDataP);
    if (instanceP != NULL) {
        // is the server asking for the full object ?
        if (*numDataP == 0)
        {
            int nbRes = instanceP->resource_count;

            *dataArrayP = lwm2m_data_new(nbRes);
            if (*dataArrayP == NULL) {
                return COAP_500_INTERNAL_SERVER_ERROR;
            }
            *numDataP = nbRes;
            for (i = 0; i < nbRes; i++)
            {
                (*dataArrayP)[i].id = instanceP->resource_ids[i];
            }
        }
        else
        {
            for (i = 0; i < *numDataP && result == COAP_205_CONTENT; i++)
            {
                result = COAP_404_NOT_FOUND;
                for (j = 0; j < instanceP->resource_count; j++) {
                    if (instanceP->resource_ids[j] == (*dataArrayP)[i].id) {
                        result = COAP_205_CONTENT;
                        break;
                    }
                }
            }
        }
    }

    return result;
}

uint8_t lwm2m_atcmd_object_write_callback (void * contextP, uint16_t instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_object_t * objectP)
{
    prv_instance_t * targetP;
    apb_proxy_at_cmd_result_t cmd_result = {0};
    char* result_buf = NULL;
    char *num_string = NULL;
    char *opaque_string = NULL;
    int current_pos = 0;
    struct lwm2m_atcmd_t* lwm2m_t = NULL;
    int i;
    uint8_t ret;

    cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
    cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;

    targetP = (prv_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    if (NULL == targetP)
        return COAP_404_NOT_FOUND;

    lwm2m_t = (struct lwm2m_atcmd_t*)objectP->userData;
    if (NULL == lwm2m_t)
        return COAP_404_NOT_FOUND;

    if (lwm2m_t->is_used == 0)
        return COAP_404_NOT_FOUND;

    result_buf = _alloc_buffer(1024);
    //temp_buf = _alloc_buffer(50);
    num_string = _alloc_buffer(50);
    if (result_buf == NULL || num_string == NULL) {
        NW_LOG("read data in select, memory error !!");
        ret = COAP_412_PRECONDITION_FAILED;
    } else {
        snprintf(result_buf, 1024, "+ELMWRITE:%d,%d,%d,%d", lwm2m_t->lwm2m_id,
                            objectP->objID, instanceId, numData);
        current_pos = strlen(result_buf);
        for (i = 0; i < numData; i++) {
            snprintf(result_buf + current_pos, 1024 - current_pos, ",%d", dataArray->id);
            current_pos += strlen(result_buf + current_pos);
            switch (dataArray->type) {
                case LWM2M_TYPE_STRING:
                    snprintf(result_buf + current_pos, 1024 - current_pos, ",S,%d,\"%s\"",
                            dataArray->value.asBuffer.length,
                            dataArray->value.asBuffer.buffer);
                    current_pos += strlen(result_buf + current_pos);
                    break;
                case LWM2M_TYPE_OPAQUE:
                    opaque_string = (char *)_alloc_buffer(2 * dataArray->value.asBuffer.length * 2 + 1);
                    onenet_at_bin_to_hex(opaque_string, dataArray->value.asBuffer.buffer, dataArray->value.asBuffer.length * 2);
                    snprintf(result_buf + current_pos, 1024 - current_pos, ",D,%d,\"%s\"",
                            dataArray->value.asBuffer.length,
                            opaque_string);
                    current_pos += strlen(result_buf + current_pos);
                    _free_buffer(opaque_string);
                    break;
                case LWM2M_TYPE_INTEGER:
                    snprintf(num_string, 50, "%d", (int)dataArray->value.asInteger);
                    snprintf(result_buf + current_pos, 1024 - current_pos, ",I,%d,%s",
                                                strlen(num_string),
                                                num_string);
                    current_pos += strlen(result_buf + current_pos);
                    break;
                case LWM2M_TYPE_FLOAT:
                    snprintf(num_string, 50, "%f", dataArray->value.asFloat);
                    snprintf(result_buf + current_pos, 1024 - current_pos, ",F,%d,%s",
                                                strlen(num_string),
                                                num_string);
                    current_pos += strlen(result_buf + current_pos);
                    break;
            }
        }

        cmd_result.pdata = result_buf;
        cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
        cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
        cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
        apb_proxy_send_at_cmd_result(&cmd_result);

        if (xSemaphoreTake(lwm2m_t->readWriteSemph, READ_WRITE_TIMEOUT / portTICK_PERIOD_MS) == pdTRUE) {
            ret = lwm2m_t->write_result;
            NW_LOG("lwm2m_atcmd_object_write_callback take lwm2m_t->readWriteSemph result = %d", ret);
        } else {
            NW_LOG("lwm2m_atcmd_object_write_callback take lwm2m_t->readWriteSemph failed, timeout");
            ret = COAP_412_PRECONDITION_FAILED;
        }
    }

    if (result_buf) {
        _free_buffer(result_buf);
    }
    if (num_string) {
        _free_buffer(num_string);
    }
    return ret;
}

uint8_t lwm2m_atcmd_object_execute_callback (void * contextP, uint16_t instanceId, uint16_t resourceId, uint8_t * buffer, int length, lwm2m_object_t * objectP)
{
    prv_instance_t * targetP;
    apb_proxy_at_cmd_result_t cmd_result = {0};
    char* result_buf = NULL;
    char *buffer_str = NULL;
    int current_pos = 0;
    struct lwm2m_atcmd_t* lwm2m_t = NULL;
    uint8_t ret;

    cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
    cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;

    targetP = (prv_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    if (NULL == targetP)
        return COAP_404_NOT_FOUND;

    lwm2m_t = (struct lwm2m_atcmd_t*)objectP->userData;
    if (NULL == lwm2m_t)
        return COAP_404_NOT_FOUND;

    if (lwm2m_t->is_used == 0)
        return COAP_404_NOT_FOUND;

    result_buf = _alloc_buffer(1024);
    if (result_buf == NULL) {
        NW_LOG("lwm2m_atcmd_object_execute_callback result_buf == NULL, memory error !!");
        ret = COAP_412_PRECONDITION_FAILED;
    } else {
        snprintf(result_buf, 1024, "+ELMEXECUTE:%d,%d,%d,%d", lwm2m_t->lwm2m_id,
                            objectP->objID, instanceId, resourceId);
        current_pos = strlen(result_buf);

        if (length > 0) {
            buffer_str = _alloc_buffer(length * 2 + 1);
        }
        if (buffer_str == NULL) {
            NW_LOG("lwm2m_atcmd_object_execute_callback buffer_str == NULL, memory error !!");
            ret = COAP_412_PRECONDITION_FAILED;
        } else {
            _get_data_to_hex(buffer_str, buffer, length);

            snprintf(result_buf + current_pos, 1024 - current_pos, ",%d,\"%s\"", strlen(buffer_str),
                    buffer_str);
            current_pos = strlen(result_buf);

            cmd_result.pdata = result_buf;
            cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
            cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
            cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
            apb_proxy_send_at_cmd_result(&cmd_result);

            if (xSemaphoreTake(lwm2m_t->readWriteSemph, READ_WRITE_TIMEOUT / portTICK_PERIOD_MS) == pdTRUE) {
                ret = lwm2m_t->write_result;
            } else {
                NW_LOG("lwm2m_atcmd_object_execute_callback take lwm2m_t->readWriteSemph failed, timeout");
                ret = COAP_412_PRECONDITION_FAILED;
            }
        }

    }

    if (result_buf) {
        _free_buffer(result_buf);
    }
    return ret;
}

uint8_t lwm2m_atcmd_object_create_callback (void * contextP, uint16_t instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_object_t * objectP)
{
    return COAP_405_METHOD_NOT_ALLOWED;
}

uint8_t lwm2m_atcmd_object_delete_callback (uint16_t instanceId, lwm2m_object_t * objectP)
{
    return COAP_405_METHOD_NOT_ALLOWED;
}

static struct lwm2m_atcmd_t* lwm2m_atcmd_get_free_lwm2m()
{
    for(int i = 0; i < SUPPORT_LWM2M_ATCMD_INSTANCE_MAX_NUM; i++){
        if (g_lwm2m_atcmd_t[i].is_used == 0) {
            memset(&g_lwm2m_atcmd_t[i], 0, sizeof(struct lwm2m_atcmd_t));
            g_lwm2m_atcmd_t[i].is_used = 1;
            g_lwm2m_atcmd_t[i].lwm2m_id = i;
            g_lwm2m_atcmd_number++;
            {
            char lwm2m_group_name[20] = {0};
            sprintf(lwm2m_group_name, LWM2M_ATCMD_NVDM_GROUP_NAME_BASE, g_lwm2m_atcmd_t[i].lwm2m_id);
            // is_used
            nvdm_status_t status = nvdm_write_data_item(lwm2m_group_name,
                                 LWM2M_ATCMD_NVDM_ITEM_NAME_IS_USED,
                                 NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                 (const uint8_t *)&(g_lwm2m_atcmd_t[i].is_used),
                                 sizeof(bool));
            NW_LOG("write [%s][%s: %d] = %d", lwm2m_group_name, LWM2M_ATCMD_NVDM_ITEM_NAME_IS_USED, g_lwm2m_atcmd_t[i].is_used, status);
            }
            return &g_lwm2m_atcmd_t[i];
        }
    }

    return NULL;
}


static struct lwm2m_atcmd_t* lwm2m_atcmd_find_lwm2m(int lwm2m_id)
{
    for(int i = 0; i < SUPPORT_LWM2M_ATCMD_INSTANCE_MAX_NUM; i++){
        if (g_lwm2m_atcmd_t[i].lwm2m_id == lwm2m_id && g_lwm2m_atcmd_t[i].is_used)
            return &g_lwm2m_atcmd_t[i];
    }

    return NULL;
}

static void lwm2m_atcmd_remove_object(lwm2m_object_t *obj)
{
    if (obj->objID == 0) {
        clean_security_object(obj);
        _free_buffer(obj);
    } else if (obj->objID == 1) {
        clean_server_object(obj);
        _free_buffer(obj);
    } else if (obj->objID == 3) {
        free_object_device(obj);
    } else {
        prv_instance_t *instanceP = NULL;
        prv_instance_t *old_instanceP = NULL;
        instanceP = (prv_instance_t *)obj->instanceList;
        while (instanceP != NULL) {
            old_instanceP = instanceP;
            instanceP = instanceP->next;
            _free_buffer(old_instanceP->resource_ids);
            _free_buffer(old_instanceP);
        }
        _free_buffer(obj);
    }
}

static void lwm2m_atcmd_remove_lwm2m(int lwm2m_id)
{    
    for( int i = 0; i < SUPPORT_LWM2M_ATCMD_INSTANCE_MAX_NUM; i++){
        if (g_lwm2m_atcmd_t[i].lwm2m_id == lwm2m_id && g_lwm2m_atcmd_t[i].is_used) {
            lwm2m_context_t * lwm2mH = g_lwm2m_atcmd_t[i].context;
            client_data_t* data = NULL;
            struct lwm2m_atcmd_t* lwm2m_t = g_lwm2m_atcmd_t + i;
            lwm2m_object_t *object = NULL;
            lwm2m_object_t *old_object = NULL;

            lwm2m_t->is_used = 0;
            g_lwm2m_atcmd_number--;
            {
            char lwm2m_group_name[20] = {0};
            sprintf(lwm2m_group_name, LWM2M_ATCMD_NVDM_GROUP_NAME_BASE, lwm2m_t->lwm2m_id);
            // is_used
            nvdm_status_t status = nvdm_write_data_item(lwm2m_group_name,
                                 LWM2M_ATCMD_NVDM_ITEM_NAME_IS_USED,
                                 NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                 (const uint8_t *)&(lwm2m_t->is_used),
                                 sizeof(bool));
            NW_LOG("write [%s][%s: %d] = %d", lwm2m_group_name, LWM2M_ATCMD_NVDM_ITEM_NAME_IS_USED, lwm2m_t->is_used, status);
            }

            lwm2m_atcmd_stop_rtc_timer(lwm2m_t);
            lwm2m_atcmd_retention_delete_all_objects(lwm2m_t->lwm2m_id);

            // remove client_data_t* data and lwm2mH
            if (lwm2mH && (data = (client_data_t*)lwm2mH->userData)) {
                object = lwm2mH->objectList;
                while (object != NULL) {
                    old_object = object;
                    object = object->next;
                    lwm2m_atcmd_remove_object(old_object);
                }
                lwm2m_close(lwm2mH);
                close(data->sock);
                connection_free(data->connList);
            }

            _free_buffer(lwm2m_t->localPort);
            _free_buffer(lwm2m_t->server);
            _free_buffer(lwm2m_t->serverPort);
            _free_buffer(lwm2m_t->name);
            _free_buffer(lwm2m_t->pskId);
            _free_buffer(lwm2m_t->psk);
            _free_buffer(lwm2m_t->pskBuffer);
            if (lwm2m_t->readWriteSemph != NULL) {
                vSemaphoreDelete(lwm2m_t->readWriteSemph);
            }


            return;
        }
    }
}

lwm2m_object_t* lwm2m_atcmd_find_object(lwm2m_context_t* contextP, uint16_t id)
{
    lwm2m_object_t* object_t = contextP->objectList;

    while(object_t) {
        if (object_t->objID == id)
            return object_t;
        object_t = object_t->next;
    }

    return NULL;
}


static void lwm2m_atcmd_notify_callback(lwm2m_notify_type_t type, lwm2m_notify_code_t code, uint32_t option_info)
{
    NW_LOG("lwm2m_atcmd_notify_callback type = %d, code = %d, option_info = %d", (int)type, (int)code, (int)option_info);

    if (type == LWM2M_NOTIFY_TYPE_PING && code == LWM2M_NOTIFY_CODE_SUCCESS && g_lwm2m_is_locking) {
        xTimerStop(g_lwm2m_ping_timer, 0);
        hal_sleep_manager_release_sleeplock(g_lwm2m_sleep_handle, HAL_SLEEP_LOCK_DEEP);
        NW_LOG("hal_sleep_manager_release_sleeplock");
        g_lwm2m_is_locking = false;
    }
}

static int lwm2m_atcmd_main(struct lwm2m_atcmd_t* lwm2m_t)
{
    client_data_t *data;
    int result;
    lwm2m_context_t * lwm2mH = NULL;

    data = &lwm2m_t->client_data;
    memset(data, 0, sizeof(client_data_t));
    data->addressFamily = lwm2m_t->addressFamily;

    if (!(lwm2m_t->server))
    {
        lwm2m_t->server = (AF_INET == data->addressFamily ? DEFAULT_SERVER_IPV4 : DEFAULT_SERVER_IPV6);
    }

    /*
     *This call an internal function that create an IPV6 socket on the port 5683.
     */
    LWM2MAPPLOG("Trying to bind LWM2M Client to addressFamily: %d, port %s\r\n",
            data->addressFamily, lwm2m_t->localPort);
    data->sock = create_socket(lwm2m_t->localPort, data->addressFamily);
    LWM2MAPPLOG("sock id %d", data->sock);
    if (data->sock < 0)
    {
        LWM2MAPPLOG("Failed to open socket: %d %s\r\n", errno, strerror(errno));
        return -1;
    }

    /*
     * Now the main function fill an array with each object, this list will be later passed to liblwm2m.
     * Those functions are located in their respective object file.
     */
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    if (lwm2m_t->psk != NULL)
    {
        lwm2m_t->pskLen = strlen(lwm2m_t->psk) / 2;
        lwm2m_t->pskBuffer = malloc(lwm2m_t->pskLen);

        if (NULL == lwm2m_t->pskBuffer)
        {
            LWM2MAPPLOG("Failed to create PSK binary buffer\r\n");
            return -1;
        }
        // Hex string to binary
        char *h = lwm2m_t->psk;
        char *b = lwm2m_t->pskBuffer;
        char xlate[] = "0123456789ABCDEF";

        for ( ; *h; h += 2, ++b)
        {
            if (h - lwm2m_t->psk >= lwm2m_t->pskLen * 2) break;
            char *l = strchr(xlate, (char)toupper((unsigned char)*h));
            char *r = strchr(xlate, (char)toupper((unsigned char)*(h+1)));

            if (!r || !l)
            {
                LWM2MAPPLOG("Failed to parse Pre-Shared-Key HEXSTRING\r\n");
                return -1;
            }

            *b = ((l - xlate) << 4) + (r - xlate);
        }
    }
#endif

    char serverUri[50];
    int serverId = 123;
    sprintf (serverUri, "coap://%s:%s", lwm2m_t->server, lwm2m_t->serverPort);
    LWM2MAPPLOG("serverUri = %s", serverUri);

    lwm2m_objArray[lwm2m_t->lwm2m_id][0] = get_security_object(serverId, serverUri, lwm2m_t->pskId, lwm2m_t->pskBuffer, lwm2m_t->pskLen, false);
    if (NULL == lwm2m_objArray[lwm2m_t->lwm2m_id][0])
    {
        LWM2MAPPLOG("Failed to create security object\r\n");
        return -1;
    }
    data->securityObjP = lwm2m_objArray[lwm2m_t->lwm2m_id][0];
    
    lwm2m_objArray[lwm2m_t->lwm2m_id][1] = get_server_object(serverId, "U", lwm2m_t->lifetime, false);
    if (NULL == lwm2m_objArray[lwm2m_t->lwm2m_id][1])
    {
        LWM2MAPPLOG("Failed to create server object\r\n");
        return -1;
    }
    data->serverObject = lwm2m_objArray[lwm2m_t->lwm2m_id][1];

    lwm2m_objArray[lwm2m_t->lwm2m_id][2] = get_object_device();
    if (NULL == lwm2m_objArray[lwm2m_t->lwm2m_id][2])
    {
        LWM2MAPPLOG("Failed to create device object\r\n");
        return -1;
    }

    /*
     * The liblwm2m library is now initialized with the functions that will be in
     * charge of communication
     */
    lwm2mH = lwm2m_init(data);
    if (NULL == lwm2mH)
    {
        LWM2MAPPLOG("lwm2m_init() failed\r\n");
        return -1;
    }
    if (g_lwm2m_atcmd_state == STATE_READY) {
        lwm2mH->state = g_lwm2m_atcmd_state;
    }
    lwm2m_t->context = lwm2mH;
    lwm2mH->mode = CLIENT_MODE;
    data->lwm2mH = lwm2mH;

    /*
     * We configure the liblwm2m library with the name of the client - which shall be unique for each client -
     * the number of objects we will be passing through and the objects array
     */
    LWM2MAPPLOG("[lwm2m_t->%d] %x = %x, %x, %x", lwm2m_t->lwm2m_id, (unsigned int)lwm2m_objArray[lwm2m_t->lwm2m_id],
            (unsigned int)lwm2m_objArray[lwm2m_t->lwm2m_id][0],
            (unsigned int)lwm2m_objArray[lwm2m_t->lwm2m_id][1],
            (unsigned int)lwm2m_objArray[lwm2m_t->lwm2m_id][2]);

    /**
     * Initialize  callback.
     */ 
    lwm2m_t->context->observe_callback = lwm2m_atcmd_object_observe_callback;
    lwm2m_t->context->observe_userdata = (void*)lwm2m_t;
    lwm2m_t->context->parameter_callback = lwm2m_atcmd_object_parameter_callback;
    lwm2m_t->context->connect_server_callback = lwm2m_atcmd_connect_server;
    lwm2m_t->context->close_connection_callback = lwm2m_atcmd_close_connection;
    lwm2m_t->context->notify_callback = lwm2m_atcmd_notify_callback;

    result = lwm2m_configure(lwm2mH, lwm2m_t->name, NULL, NULL, 3, lwm2m_objArray[lwm2m_t->lwm2m_id]);

    if (result != 0)
    {
        lwm2m_close(lwm2mH);
        lwm2m_t->context = NULL;
        LWM2MAPPLOG("lwm2m_configure() failed: 0x%X\r\n", result);
        return -1;
    }
    
    LWM2MAPPLOG("LWM2M Client \"%s\" started on port %s\r\n", lwm2m_t->name, lwm2m_t->localPort);
    lwm2m_atcmd_start_rtc_timer(lwm2m_t);
    lwm2mH->ext_reg_update = 1;

    return 0;
}

static void lwm2m_atcmd_main_loop(void *not_used)
{
    int lwm2m_size = 0;
    if (g_lwm2m_need_deep_sleep_handling) {
        g_lwm2m_need_deep_sleep_handling = false;
        for (int i = 0; i < SUPPORT_LWM2M_ATCMD_INSTANCE_MAX_NUM; i++) {
            struct lwm2m_atcmd_t *lwm2m_t = &g_lwm2m_atcmd_t[i];
            if (lwm2m_t->is_used) {
                lwm2m_t->server = _alloc_buffer(LWM2M_ATCMD_MAX_SERVER_LEN + 1);
                lwm2m_t->serverPort = _alloc_buffer(LWM2M_ATCMD_MAX_PORT_LEN + 1);
                lwm2m_t->localPort = _alloc_buffer(LWM2M_ATCMD_MAX_PORT_LEN + 1);
                lwm2m_t->name = _alloc_buffer(LWM2M_ATCMD_MAX_NAME_LEN + 1);
            #if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
                lwm2m_t->pskId = _alloc_buffer(LWM2M_ATCMD_MAX_PSKID_LEN + 1);
                lwm2m_t->psk = _alloc_buffer(LWM2M_ATCMD_MAX_PSK_LEN + 1);
            #endif
                // read lwm2m config from NVDM
                {
                    char lwm2m_group_name[20] = {0};
                    uint32_t len;
                    sprintf(lwm2m_group_name, LWM2M_ATCMD_NVDM_GROUP_NAME_BASE, i);
                    // localPort
                    len = LWM2M_ATCMD_MAX_PORT_LEN;
                    nvdm_read_data_item(lwm2m_group_name,
                                        LWM2M_ATCMD_NVDM_ITEM_NAME_LOCAL_PORT,
                                        (uint8_t *)lwm2m_t->localPort,
                                        &len);
                    // server
                    len = LWM2M_ATCMD_MAX_SERVER_LEN;
                    nvdm_read_data_item(lwm2m_group_name,
                                        LWM2M_ATCMD_NVDM_ITEM_NAME_SERVER,
                                        (uint8_t *)lwm2m_t->server,
                                        &len);
                    // serverPort
                    len = LWM2M_ATCMD_MAX_PORT_LEN;
                    nvdm_read_data_item(lwm2m_group_name,
                                        LWM2M_ATCMD_NVDM_ITEM_NAME_SERVER_PORT,
                                        (uint8_t *)lwm2m_t->serverPort,
                                        &len);
                    // name
                    len = LWM2M_ATCMD_MAX_NAME_LEN;
                    nvdm_read_data_item(lwm2m_group_name,
                                        LWM2M_ATCMD_NVDM_ITEM_NAME_DEVICE,
                                        (uint8_t *)lwm2m_t->name,
                                        &len);
                #if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
                    // pskId
                    len = LWM2M_ATCMD_MAX_PSKID_LEN;
                    nvdm_read_data_item(lwm2m_group_name,
                                        LWM2M_ATCMD_NVDM_ITEM_NAME_PSKID,
                                        (uint8_t *)lwm2m_t->pskId,
                                        &len);
                    // psk
                    len = LWM2M_ATCMD_MAX_PSK_LEN;
                    nvdm_read_data_item(lwm2m_group_name,
                                        LWM2M_ATCMD_NVDM_ITEM_NAME_PSK,
                                        (uint8_t *)lwm2m_t->psk,
                                        &len);
                #endif
                    // lifetime
                    len = sizeof(int);
                    nvdm_read_data_item(lwm2m_group_name,
                                        LWM2M_ATCMD_NVDM_ITEM_NAME_LIFETIME,
                                        (uint8_t *)&(lwm2m_t->lifetime),
                                        &len);
                    // addressFamily
                    len = sizeof(int);
                    nvdm_read_data_item(lwm2m_group_name,
                                        LWM2M_ATCMD_NVDM_ITEM_NAME_ADDR_FAMILY,
                                        (uint8_t *)&(lwm2m_t->addressFamily),
                                        &len);
                }
                lwm2m_t->readWriteSemph = xSemaphoreCreateBinary();
                if (lwm2m_t->readWriteSemph == NULL) {
                    NW_LOG("Cannot create semph");
                    lwm2m_atcmd_remove_lwm2m(lwm2m_t->lwm2m_id);
                }
                if (lwm2m_atcmd_main(lwm2m_t) != 0) {
                    NW_LOG("Cannnot start lwm2m connection");
                    lwm2m_atcmd_remove_lwm2m(lwm2m_t->lwm2m_id);
                }
                lwm2m_atcmd_rentention_t *lwm2m_saved = &g_lwm2m_atcmd_rentention_t[lwm2m_t->lwm2m_id];
                for (int j = 0; j < LWM2M_ATCMD_OBJECT_MAX_COUNT; j++) {
                    lwm2m_atcmd_object_info_t *object_info = &lwm2m_saved->object_info[j];
                    if (object_info->is_used) {
                        int result, object_is_exist = 0;
                        lwm2m_object_t * objectP = NULL;
                        prv_instance_t * instanceP = NULL;
                        prv_instance_t * tempInstanceP = NULL;
                        objectP = lwm2m_atcmd_find_object(lwm2m_t->context, object_info->object_id);
                        if (objectP == NULL) {
                            objectP = _alloc_buffer(sizeof(lwm2m_object_t));
                            objectP->objID = object_info->object_id;
                        } else {
                            object_is_exist = 1;
                        }

                        tempInstanceP = _alloc_buffer(sizeof(prv_instance_t));
                        tempInstanceP->shortID = object_info->instance_id;
                        tempInstanceP->next = NULL;

                        NW_LOG("resource count: %d", object_info->resource_count);

                        tempInstanceP->resource_ids = _alloc_buffer(sizeof(uint16_t) * object_info->resource_count);
                        NW_LOG("[lwm2m_atcmd_main_loop] instanceP->resource_ids = %x", tempInstanceP->resource_ids);
                        memcpy(tempInstanceP->resource_ids, object_info->resource_ids, sizeof(uint16_t) * object_info->resource_count);

                        instanceP = (prv_instance_t *)lwm2m_list_find(objectP->instanceList, object_info->instance_id);
                        if (instanceP == NULL) {
                            objectP->instanceList = LWM2M_LIST_ADD(objectP->instanceList, tempInstanceP);
                            NW_LOG("add instance: %d", object_info->instance_id);
                        } else {
                            instanceP->resource_count = tempInstanceP->resource_count;
                            instanceP->resource_ids = tempInstanceP->resource_ids;
                            _free_buffer(tempInstanceP);
                        }

                        if (object_is_exist == 0) {
                            objectP->readFunc = lwm2m_atcmd_object_read_callback;
                            objectP->writeFunc = lwm2m_atcmd_object_write_callback;
                            objectP->executeFunc = lwm2m_atcmd_object_execute_callback;
                            objectP->createFunc = lwm2m_atcmd_object_create_callback;
                            objectP->deleteFunc = lwm2m_atcmd_object_delete_callback;
                            objectP->discoverFunc = lwm2m_atcmd_object_discover_callback;
                            objectP->objID = object_info->object_id;
                            objectP->userData = (void *)lwm2m_t;
                            result = lwm2m_add_object(lwm2m_t->context, objectP);
                            NW_LOG("[lwm2m_atcmd_main_loop] add object");
                        } else {
                            // old object
                            if (lwm2m_t->context->state == STATE_READY)
                            {
                                lwm2m_update_registration(lwm2m_t->context, 0, true);
                            }
                            result = COAP_NO_ERROR;
                        }

                        NW_LOG("add object result: %d", result);
                    }
                }
            }
        }
    }
    while (g_is_lwm2m_task_started)
    {
        lwm2m_size = 0;
        for(int i = 0; i < SUPPORT_LWM2M_ATCMD_INSTANCE_MAX_NUM; i++) {  
            int result;          
            struct timeval tv;
            fd_set readfds; 
            struct lwm2m_atcmd_t* lwm2m_t = &g_lwm2m_atcmd_t[i];
            lwm2m_context_t * lwm2mH = NULL;
            client_data_t* data = NULL;

            if (lwm2m_t->is_used == 0)
                continue;

            if (g_lwm2m_need_update) {
                g_lwm2m_need_update = false;
                lwm2m_update_registration(lwm2m_t->context, 0, false);
            }

            lwm2m_size++;
            lwm2mH = lwm2m_t->context;

            if (lwm2mH == NULL) {
                NW_LOG("lwm2m_t context null");
                break;
            }

            data = (client_data_t*)lwm2mH->userData;

            tv.tv_sec = 5;
            tv.tv_usec = 0;

            /*
                 * This function does two things:
                 *  - first it does the work needed by liblwm2m (eg. (re)sending some packets).
                 *  - Secondly it adjusts the timeout value (default 60s) depending on the state of the transaction
                 *    (eg. retransmission) and the time between the next operation
                 */

            result = lwm2m_step(lwm2mH, &(tv.tv_sec));
            LWM2MAPPLOG("App execute_data_reporting State: %d ", lwm2mH->state);

            switch (lwm2mH->state)
            {
            case STATE_INITIAL:
                LWM2MAPPLOG("STATE_INITIAL\r\n");
                break;
            case STATE_BOOTSTRAP_REQUIRED:
                LWM2MAPPLOG("STATE_BOOTSTRAP_REQUIRED\r\n");
                break;
            case STATE_BOOTSTRAPPING:
                LWM2MAPPLOG("STATE_BOOTSTRAPPING\r\n");
                break;
            case STATE_REGISTER_REQUIRED:
                LWM2MAPPLOG("STATE_REGISTER_REQUIRED\r\n");
                break;
            case STATE_REGISTERING:
                LWM2MAPPLOG("STATE_REGISTERING\r\n");
                break;
            case STATE_READY:
                LWM2MAPPLOG("STATE_READY\r\n");
                break;
            default:
                LWM2MAPPLOG("Unknown...\r\n");
                break;
            }
            if (result != 0)
            {
                LWM2MAPPLOG("lwm2m_step() failed: 0x%X\r\n", result);
            }

            g_lwm2m_atcmd_state = lwm2mH->state;

#ifdef WITH_MBEDTLS
            mbedtls_connection_t *connP = data->connList;
            if (connP != NULL && connP->dtlsSession != 0) {
                int result = connection_handle_packet(connP);
                if (result < 0) {
                    LWM2MAPPLOG("error handling message %d", result);
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

            if (result < 0)
            {
                if (errno != EINTR)
                {
                    LWM2MAPPLOG("Error in select(): %d %s\r\n", errno, strerror(errno));
                }
            }
            else if (result > 0)
            {
                uint8_t buffer[MAX_PACKET_SIZE];
                int numBytes;

                /*
                 * If an event happens on the socket
                 */
                if (FD_ISSET(data->sock, &readfds))
                {
                    struct sockaddr_storage addr;
                    socklen_t addrLen;

                    addrLen = sizeof(addr);

                    /*
                     * We retrieve the data received
                     */
                    numBytes = recvfrom(data->sock, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&addr, &addrLen);

                    if (0 > numBytes)
                    {
                        LWM2MAPPLOG("Error in recvfrom(): %d %s\r\n", errno, strerror(errno));
                    }
                    else if (0 < numBytes)
                    {
                        char s[INET6_ADDRSTRLEN];
#if defined(WITH_MBEDTLS)
                        mbedtls_connection_t *connP;
#elif defined(WITH_TINYDTLS)
                        dtls_connection_t *connP;
#else
                        connection_t *connP;
#endif
                        if (AF_INET == addr.ss_family)
                        {
                            struct sockaddr_in *saddr = (struct sockaddr_in *)&addr;
                            LWM2MAPPLOG("AF_INET saddr = %d, %d, %d, %d, ", saddr->sin_len, saddr->sin_family,
                                    saddr->sin_port, (int)saddr->sin_addr.s_addr);

                            inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET6_ADDRSTRLEN);
                        }
                        else if (AF_INET6 == addr.ss_family)
                        {
                            struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)&addr;
                            inet_ntop(saddr->sin6_family, &saddr->sin6_addr, s, INET6_ADDRSTRLEN);
                        }
                        LWM2MAPPLOG("%d bytes received from [%s]\r\n", numBytes, s);

                        connP = connection_find(data->connList, &addr, addrLen);

                        if (connP != NULL)
                        {
                            /*
                                             * Let liblwm2m respond to the query depending on the context
                                             */
                            lwm2m_t->connP = connP;
                        #ifdef WITH_TINYDTLS
                            int result = connection_handle_packet(connP, buffer, numBytes);
                            if (0 != result)
                            {
                                LWM2MAPPLOG("error handling message %d\n",result);
                            }
                        #else
                            lwm2m_handle_packet(lwm2mH, buffer, numBytes, connP);
                        #endif
                        }
                        else
                        {
                            LWM2MAPPLOG("received bytes ignored!\r\n");
                        }
                    }
                }
            }
        }
        }
        if (lwm2m_size) {
            vTaskDelay(1000 / portTICK_RATE_MS); // release CPU
        } else {
            g_is_lwm2m_task_started = 0;
            g_lwm2m_atcmd_state = STATE_INITIAL;
        }
    }

    NW_LOG("dhcp_network_changed_task deleted");
    vTaskDelete(NULL);

    return;
}

static void lwm2m_atcmd_timeout_callback(TimerHandle_t xTimer)
{
    if (g_lwm2m_is_locking) {
        hal_sleep_manager_release_sleeplock(g_lwm2m_sleep_handle, HAL_SLEEP_LOCK_DEEP);
        NW_LOG("hal_sleep_manager_release_sleeplock");
        g_lwm2m_is_locking = false;
    }
}

void lwm2m_atcmd_init_task(void)
{
    g_lwm2m_ping_timer = xTimerCreate("lwm2m_atcmd_timer",
                                      1000 * COAP_MAX_TRANSMIT_WAIT / portTICK_PERIOD_MS, 
                                      pdFALSE,
                                      (void *)0,
                                      lwm2m_atcmd_timeout_callback);
    g_lwm2m_sleep_handle = hal_sleep_manager_set_sleep_handle("lwm2m_at");
    NW_LOG("g_lwm2m_ping_timer = 0x%x, g_lwm2m_sleep_handle = 0x%x", g_lwm2m_ping_timer, g_lwm2m_sleep_handle);
    if (rtc_power_on_result_external() != DEEP_SLEEP && rtc_power_on_result_external() != DEEPER_SLEEP) {
        /* COLD-BOOT case: normal init */
        NW_LOG("normal init handling");
        for (int i = 0; i < SUPPORT_LWM2M_ATCMD_INSTANCE_MAX_NUM; i++) {
            char lwm2m_group_name[20] = {0};
            sprintf(lwm2m_group_name, LWM2M_ATCMD_NVDM_GROUP_NAME_BASE, i);
            // is_used
            bool is_used = false;
            nvdm_status_t status = nvdm_write_data_item(lwm2m_group_name,
                                 LWM2M_ATCMD_NVDM_ITEM_NAME_IS_USED,
                                 NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                 (const uint8_t *)&is_used,
                                 sizeof(bool));
            NW_LOG("write [%s][%s: %d] = %d", lwm2m_group_name, LWM2M_ATCMD_NVDM_ITEM_NAME_IS_USED, is_used, status);
        }
    } else {
        /* DEEP-SLEEP case: data retention process */
        NW_LOG("deep sleep handling");
        for (int i = 0; i < SUPPORT_LWM2M_ATCMD_INSTANCE_MAX_NUM; i++) {
            char lwm2m_group_name[20] = {0};
            uint32_t len;
            bool is_used = false;
            sprintf(lwm2m_group_name, LWM2M_ATCMD_NVDM_GROUP_NAME_BASE, i);
            // is_used
            len = sizeof(bool);
            nvdm_status_t status = nvdm_read_data_item(lwm2m_group_name, LWM2M_ATCMD_NVDM_ITEM_NAME_IS_USED, (uint8_t *)&is_used, &len);
            NW_LOG("read [%s][%s: %d] = %d", lwm2m_group_name, LWM2M_ATCMD_NVDM_ITEM_NAME_IS_USED, is_used, status);
            if (is_used) {
                g_lwm2m_need_deep_sleep_handling = true;
                struct lwm2m_atcmd_t *lwm2m_t = &g_lwm2m_atcmd_t[i];
                memset(lwm2m_t, 0, sizeof(struct lwm2m_atcmd_t));
                lwm2m_t->is_used = true;
                lwm2m_t->lwm2m_id = i;
                if (g_is_lwm2m_task_started == 0) {
                    g_is_lwm2m_task_started = 1;
                    xTaskCreate(lwm2m_atcmd_main_loop,
                            "lwm2m_atcmd",
                        #if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
                            1024 * 6 / sizeof(portSTACK_TYPE),
                        #else
                            1024 * 4 / sizeof(portSTACK_TYPE),
                        #endif
                            NULL,
                            TASK_PRIORITY_NORMAL,
                            NULL);
                    NW_LOG("create lwm2m task");
                }
            }
        }
    }
}

static struct lwm2m_atcmd_t* lwm2m_atcmd_handle_config(char* config)
{
    bool result = false;
    int32_t param_cnt;
    char *server;
    char *port;
    char *local_port;
    char *name;
    char *domain;
    int life_time = -1;
    char *pskId;
    char *psk;
    struct lwm2m_atcmd_t* lwm2m_t = NULL;

    if (config == NULL) {
        return NULL;
    }
    // <server>,<port>,<local_port>,<name>,<domain><lifetime>[,<pskId>,<psk>]
    param_cnt = apb_nw_util_parse_all_parameters(config, NULL, "s,l,s,s,l,s,s,l,s,s,l,s,s,l,s,s,l,i,s,l,s,s,l,s",
            NULL, NULL, &server,
            NULL, NULL, &port,
            NULL, NULL, &local_port,
            NULL, NULL, &name,
            NULL, NULL, &domain,
            NULL, NULL, &life_time,
            NULL, NULL, &pskId,
            NULL, NULL, &psk);

    if (param_cnt != 18 && param_cnt != 24) {
        NW_LOG("param_cnt != 18 error");
        goto exit;
    }

    if (!apb_nw_util_is_pure_int_string(port) || !apb_nw_util_is_pure_int_string(local_port)) {
        NW_LOG("port or local_port is not pure number");
        goto exit;
    }

    lwm2m_t = lwm2m_atcmd_get_free_lwm2m();
    if (lwm2m_t != NULL) {
        lwm2m_t->server = _alloc_buffer(strlen(server) + 1);
        lwm2m_t->serverPort = _alloc_buffer(strlen(port) + 1);
        lwm2m_t->localPort = _alloc_buffer(strlen(local_port) + 1);
        lwm2m_t->name = _alloc_buffer(strlen(name) + 1);
        if (param_cnt == 24)
        {
            lwm2m_t->pskId = _alloc_buffer(strlen(pskId) + 1);
            lwm2m_t->psk = _alloc_buffer(strlen(psk) + 1);
        }
        if (!(lwm2m_t->server && lwm2m_t->serverPort && lwm2m_t->localPort && lwm2m_t->name)) {
            NW_LOG("Cannot malloc memory in lwm2m_t");
            goto exit;
        }
        if (param_cnt == 24 && !(lwm2m_t->pskId && lwm2m_t->psk)) {
            NW_LOG("Cannot malloc psk memory in lwm2m_t");
            goto exit;
        }
        if (strlen(server) <= LWM2M_ATCMD_MAX_SERVER_LEN) {
            memset(lwm2m_t->server, 0, strlen(server) + 1);
            memcpy(lwm2m_t->server, server, strlen(server));
        } else {
            NW_LOG("parse server fail");
            goto exit;
        }
        if (strlen(port) <= LWM2M_ATCMD_MAX_PORT_LEN) {
            memset(lwm2m_t->serverPort, 0, strlen(port) + 1);
            memcpy(lwm2m_t->serverPort, port, strlen(port));
        } else {
            NW_LOG("parse port fail");
            goto exit;
        }
        if (strlen(local_port) <= LWM2M_ATCMD_MAX_PORT_LEN) {
            memset(lwm2m_t->localPort, 0, strlen(local_port) + 1);
            memcpy(lwm2m_t->localPort, local_port, strlen(local_port));
        } else {
            NW_LOG("parse local_port fail");
            goto exit;
        }
        if (strlen(name) <= LWM2M_ATCMD_MAX_NAME_LEN) {
            memset(lwm2m_t->name, 0, strlen(name) + 1);
            memcpy(lwm2m_t->name, name, strlen(name));
        } else {
            NW_LOG("parse name fail");
            goto exit;
        }
    #if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
        if (param_cnt == 24 && strlen(pskId) <= LWM2M_ATCMD_MAX_PSKID_LEN) {
            memset(lwm2m_t->pskId, 0, strlen(pskId) + 1);
            memcpy(lwm2m_t->pskId, pskId, strlen(pskId));
        } else if (param_cnt == 24) {
            NW_LOG("parse pskId fail");
            goto exit;
        }
        if (param_cnt == 24 && strlen(psk) <= LWM2M_ATCMD_MAX_PSK_LEN) {
            memset(lwm2m_t->psk, 0, strlen(psk) + 1);
            memcpy(lwm2m_t->psk, psk, strlen(psk));
        } else if (param_cnt == 24) {
            NW_LOG("parse psk fail");
            goto exit;
        }
    #endif
        // domain
        if (0 == strncasecmp(domain, "IPv4", strlen("IPv4"))) {
            lwm2m_t->addressFamily = AF_INET;
        } else if (0 == strncasecmp(domain, "IPv6", strlen("IPv6"))) {
            lwm2m_t->addressFamily = AF_INET6;
        } else {
            NW_LOG("parse domain fail");
            goto exit;
        }
        // lift_time
        if (life_time > 0) {
            lwm2m_t->lifetime = life_time;
        } else {
            lwm2m_t->lifetime = DEFAULT_LIFE_TIME; //default value
        }
        result = true;
        // save lwm2m config to NVDM
        {
            char lwm2m_group_name[20] = {0};
            sprintf(lwm2m_group_name, LWM2M_ATCMD_NVDM_GROUP_NAME_BASE, lwm2m_t->lwm2m_id);
            // localPort
            nvdm_write_data_item(lwm2m_group_name,
                                 LWM2M_ATCMD_NVDM_ITEM_NAME_LOCAL_PORT,
                                 NVDM_DATA_ITEM_TYPE_STRING,
                                 (const uint8_t *)lwm2m_t->localPort,
                                 LWM2M_ATCMD_MAX_PORT_LEN);
            // server
            nvdm_write_data_item(lwm2m_group_name,
                                 LWM2M_ATCMD_NVDM_ITEM_NAME_SERVER,
                                 NVDM_DATA_ITEM_TYPE_STRING,
                                 (const uint8_t *)lwm2m_t->server,
                                 LWM2M_ATCMD_MAX_SERVER_LEN);
            // serverPort
            nvdm_write_data_item(lwm2m_group_name,
                                 LWM2M_ATCMD_NVDM_ITEM_NAME_SERVER_PORT,
                                 NVDM_DATA_ITEM_TYPE_STRING,
                                 (const uint8_t *)lwm2m_t->serverPort,
                                 LWM2M_ATCMD_MAX_PORT_LEN);
            // name
            nvdm_write_data_item(lwm2m_group_name,
                                 LWM2M_ATCMD_NVDM_ITEM_NAME_DEVICE,
                                 NVDM_DATA_ITEM_TYPE_STRING,
                                 (const uint8_t *)lwm2m_t->name,
                                 LWM2M_ATCMD_MAX_NAME_LEN);
        #if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
            // pskId
            if (lwm2m_t->pskId != NULL)
            nvdm_write_data_item(lwm2m_group_name,
                                 LWM2M_ATCMD_NVDM_ITEM_NAME_PSKID,
                                 NVDM_DATA_ITEM_TYPE_STRING,
                                 (const uint8_t *)lwm2m_t->pskId,
                                 LWM2M_ATCMD_MAX_PSKID_LEN);
            // psk
            if (lwm2m_t->psk != NULL)
            nvdm_write_data_item(lwm2m_group_name,
                                 LWM2M_ATCMD_NVDM_ITEM_NAME_PSK,
                                 NVDM_DATA_ITEM_TYPE_STRING,
                                 (const uint8_t *)lwm2m_t->psk,
                                 LWM2M_ATCMD_MAX_PSK_LEN);
        #endif
            // lifetime
            nvdm_write_data_item(lwm2m_group_name,
                                 LWM2M_ATCMD_NVDM_ITEM_NAME_LIFETIME,
                                 NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                 (const uint8_t *)&(lwm2m_t->lifetime),
                                 sizeof(int));
            // addressFamily
            nvdm_write_data_item(lwm2m_group_name,
                                 LWM2M_ATCMD_NVDM_ITEM_NAME_ADDR_FAMILY,
                                 NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                 (const uint8_t *)&(lwm2m_t->addressFamily),
                                 sizeof(int));
        }
    } else {
        NW_LOG("Cannot get free lwm2m");
    }

    // TODO: add more config
    exit:
    if (!result && lwm2m_t) {
        lwm2m_atcmd_remove_lwm2m(lwm2m_t->lwm2m_id);
        lwm2m_t = NULL;
    }
    return lwm2m_t;
}


// TODO:  config too long
apb_proxy_status_t apb_proxy_hdlr_lwm2m_config_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;
    int config_length;
    char* result_buf = NULL;  
    char* start = NULL;
    char* config_buffer = NULL;
    struct lwm2m_atcmd_t* lwm2m_t = NULL;
    NW_LOG("config lwm2m");


    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            /* 1. check ATCMD parameter */
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;

            NW_LOG("start %s", start);  
            
            result_buf = _alloc_buffer(50);
            if (result_buf == NULL) {
                NW_LOG("memory error");
                goto exit;
            } 

            config_length = strlen(start) + 1;
            NW_LOG("config_length : %d", config_length);

            /* 2. alloc config buffer */
            config_buffer = _alloc_buffer(config_length);
            if (NULL == config_buffer) {
                NW_LOG("config buffer alloc fail");
                goto exit;
            }

            strncpy(config_buffer, start, config_length - 1);

            /* 3. create lwm2m_atcmd_t, handle config */
            lwm2m_t = lwm2m_atcmd_handle_config(config_buffer);

            if (NULL == lwm2m_t) {
                NW_LOG("lwm2m config error");
                goto exit;
            }

            lwm2m_t->readWriteSemph = xSemaphoreCreateBinary();
            if (lwm2m_t->readWriteSemph == NULL) {
                NW_LOG("Cannot create semph");
                goto exit;
            }

            /* 4. run lwm2m main loop */
            if (lwm2m_atcmd_main(lwm2m_t) != 0) {
                goto exit;
            }

            /* 5. create and run lwm2m atcmd task */
            if (g_is_lwm2m_task_started == 0) {
                g_is_lwm2m_task_started = 1;
                xTaskCreate(lwm2m_atcmd_main_loop,
                        "lwm2m_atcmd",
                    #if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
                        1024 * 6 / sizeof(portSTACK_TYPE),
                    #else
                        1024 * 4 / sizeof(portSTACK_TYPE),
                    #endif
                        NULL,
                        TASK_PRIORITY_NORMAL,
                        NULL);
                NW_LOG("create lwm2m task");
            } else {
                NW_LOG("already have lwm2m task");
            }

            // lwm2m id
            sprintf(result_buf, "+ELMCONF=%d", lwm2m_t->lwm2m_id);
            NW_LOG("result_buf %s ", result_buf);
            cmd_result.pdata = result_buf;
            cmd_result.result_code = APB_PROXY_RESULT_OK;
            ret = APB_PROXY_STATUS_OK;
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }
        default: {
            break;
        }
    }

    exit:
    if (cmd_result.result_code == APB_PROXY_RESULT_ERROR && lwm2m_t) {
        lwm2m_atcmd_remove_lwm2m(lwm2m_t->lwm2m_id);
    }

    cmd_result.length = cmd_result.pdata != NULL ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);

    if (result_buf != NULL) {
        _free_buffer(result_buf);
    }
    if (config_buffer != NULL) {
        _free_buffer(config_buffer);
    }
    return ret;
}

apb_proxy_status_t apb_proxy_hdlr_lwm2m_delete_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;
    int lwm2m_id;
    char* start = NULL;
    struct lwm2m_atcmd_t* lwm2m_t = NULL;

    NW_LOG("lwm2m instance delete: [%s]", p_parse_cmd->string_ptr);
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;

            lwm2m_id = atoi(start);
            NW_LOG("delete %d", lwm2m_id);

            lwm2m_t = lwm2m_atcmd_find_lwm2m(lwm2m_id);
            if (NULL == lwm2m_t) {
                goto exit;
            }

            lwm2m_atcmd_remove_lwm2m(lwm2m_id);

            cmd_result.result_code = APB_PROXY_RESULT_OK;
            ret = APB_PROXY_STATUS_OK;
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }
        default: {
            break;
        }
    }

    exit:
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    cmd_result.length = cmd_result.pdata != NULL ? strlen(cmd_result.pdata) : 0;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return ret;
}

apb_proxy_status_t apb_proxy_hdlr_lwm2m_addobj_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;
    int lwm2m_id, object_id, instance_id, resource_count, result, object_is_exist = 0;
    int i = 0;
    int32_t param_cnt;
    int temp_resource_id;
    char* start = NULL;
    struct lwm2m_atcmd_t* lwm2m_t = NULL;
    lwm2m_context_t* contextP = NULL;
    lwm2m_object_t * objectP = NULL;
    prv_instance_t * instanceP = NULL;
    prv_instance_t * tempInstanceP = NULL;

    NW_LOG("add object: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;
            NW_LOG("[apb_proxy_hdlr_lwm2m_addobj_cmd] lwm2m_id string: %s", start);

            param_cnt = apb_nw_util_parse_all_parameters(start, &start, "i,i,i,i,",
                    &lwm2m_id, &object_id, &instance_id, &resource_count);

            NW_LOG("add to lwm2m: %d, <%d/%d>, count = %d", lwm2m_id, object_id, instance_id, resource_count);

            lwm2m_t = lwm2m_atcmd_find_lwm2m(lwm2m_id);
            if (lwm2m_t == NULL || lwm2m_t->context == NULL) {
                NW_LOG("goto exit 2");
                goto exit;
            }
            contextP = lwm2m_t->context;

            if (object_id == -1 || object_id == 1 || object_id == 3 || object_id == 0) {
                NW_LOG("goto exit 4 : object_id = %d", object_id);
                goto exit;
            }
            if (instance_id == -1 || resource_count <= 0) {
                NW_LOG("goto exit 5");
                goto exit;
            }

            objectP = lwm2m_atcmd_find_object(contextP, object_id);
            if (objectP == NULL) {
                objectP = _alloc_buffer(sizeof(lwm2m_object_t));
            } else {
                object_is_exist = 1;
            }
            if (objectP == NULL) {
                NW_LOG("goto exit 6");
                goto exit;
            }
            objectP->objID = object_id;

            tempInstanceP = _alloc_buffer(sizeof(prv_instance_t));
            if (tempInstanceP == NULL) {
                NW_LOG("goto exit 7");
                goto exit;
            }
            tempInstanceP->shortID = instance_id;
            tempInstanceP->next = NULL;

            NW_LOG("resource count: %d", resource_count);

            tempInstanceP->resource_ids = _alloc_buffer(sizeof(uint16_t) * resource_count);
            NW_LOG("[apb_proxy_hdlr_lwm2m_addobj_cmd] instanceP->resource_ids = %x", tempInstanceP->resource_ids);
            if (tempInstanceP->resource_ids) {
                for (i = 0; i < resource_count; i++) {
                    param_cnt = apb_nw_util_parse_all_parameters(start, &start, "i", &temp_resource_id);
                    if (param_cnt == 1) {
                        *(tempInstanceP->resource_ids + i) = temp_resource_id;
                    } else {
                        NW_LOG("[apb_proxy_hdlr_lwm2m_addobj_cmd] paramerter error, resource count = %d, i = %d",
                                resource_count, i);
                        goto exit;
                    }
                }
            } else {
                NW_LOG("[apb_proxy_hdlr_lwm2m_addobj_cmd] paramerter error, cannot malloc resource size = %d", resource_count);
                goto exit;
            }

            instanceP = (prv_instance_t *)lwm2m_list_find(objectP->instanceList, instance_id);
            if (instanceP == NULL) {
                objectP->instanceList = LWM2M_LIST_ADD(objectP->instanceList, tempInstanceP);
                NW_LOG("add instance: %d", instance_id);
                lwm2m_atcmd_retention_save_object(lwm2m_id, object_id, instance_id, resource_count, tempInstanceP->resource_ids);
            } else {
                instanceP->resource_count = tempInstanceP->resource_count;
                instanceP->resource_ids = tempInstanceP->resource_ids;
                _free_buffer(tempInstanceP);
            }

            if (object_is_exist == 0) {
                objectP->readFunc = lwm2m_atcmd_object_read_callback;
                objectP->writeFunc = lwm2m_atcmd_object_write_callback;
                objectP->executeFunc = lwm2m_atcmd_object_execute_callback;
                objectP->createFunc = lwm2m_atcmd_object_create_callback;
                objectP->deleteFunc = lwm2m_atcmd_object_delete_callback;
                objectP->discoverFunc = lwm2m_atcmd_object_discover_callback;
                objectP->objID = object_id;
                objectP->userData = (void *)lwm2m_t;
                result = lwm2m_add_object(contextP, objectP);
                NW_LOG("[apb_proxy_hdlr_lwm2m_addobj_cmd] add object");
            } else {
                // old object
                if (contextP->state == STATE_READY)
                {
                    lwm2m_update_registration(contextP, 0, true);
                }
                result = COAP_NO_ERROR;
            }

            NW_LOG("add object result: %d", result);
            if (result == COAP_NO_ERROR)
            {
                cmd_result.result_code = APB_PROXY_RESULT_OK;
                ret = APB_PROXY_STATUS_OK;
            } else {
                NW_LOG("add object error !!");
            }
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }
        default: {
            break;
        }
    }

    exit:

    if (cmd_result.result_code != APB_PROXY_RESULT_OK) {
        if (tempInstanceP) {
            if (tempInstanceP->resource_ids) {
                _free_buffer(tempInstanceP->resource_ids);
            }
            _free_buffer(tempInstanceP);
        }
        if (object_is_exist == 0) {
            if (objectP) {
                _free_buffer(objectP);
            }
        }
    }
    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
    return ret;
}

apb_proxy_status_t apb_proxy_hdlr_lwm2m_delobj_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;
    int lwm2m_id, object_id, result;
    char* start = NULL;
    int32_t param_cnt;
    struct lwm2m_atcmd_t* lwm2m_t = NULL;
    lwm2m_context_t* contextP = NULL;
    lwm2m_object_t *obj = NULL;

    NW_LOG("delete object: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;

            param_cnt = apb_nw_util_parse_all_parameters(start, &start, "i,i,",
                    &lwm2m_id, &object_id);
            NW_LOG("delete object lwm2m_id %d, obj - %d", lwm2m_id, object_id);

            if (param_cnt < 2) {
                NW_LOG("param_cnt < 2");
                goto exit;
            }

            if (object_id == -1 || object_id == 1 || object_id == 3 || object_id == 0) {
                NW_LOG("Cannot remove basic objects");
                goto exit;
            }

            lwm2m_t = lwm2m_atcmd_find_lwm2m(lwm2m_id);
            if (lwm2m_t == NULL || lwm2m_t->context == NULL) {
                NW_LOG("lwm2m ID parameter error, goto exit 2");
                goto exit;
            }
            contextP = lwm2m_t->context;

            obj = lwm2m_atcmd_find_object(contextP, object_id);
            if (obj != NULL) {
                lwm2m_atcmd_retention_delete_object(lwm2m_id, object_id);
                result = lwm2m_remove_object(contextP, object_id);
                lwm2m_atcmd_remove_object(obj);
                NW_LOG("delete result %d", result);
                if (result == 0)
                {
                    cmd_result.result_code = APB_PROXY_RESULT_OK;
                    ret = APB_PROXY_STATUS_OK;
                }
            } else {
                NW_LOG("Cannot find object for id : %d", object_id);
            }
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }
        default: {
            break;
        }
    }

    exit:
    cmd_result.length = cmd_result.pdata != NULL ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
    return ret;
}

apb_proxy_status_t apb_proxy_hdlr_lwm2m_read_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;
    int lwm2m_id, object_id, instance_id, resource_id, resource_cnt = 0;
    int value_len, result, i = 0;
    char value_type;
    char* start = NULL;
    char* value_str = NULL;
    uint8_t *buffer = NULL;
    int32_t param_cnt = 0;
    struct lwm2m_atcmd_t* lwm2m_t = NULL; 
    lwm2m_context_t* contextP = NULL; 
    lwm2m_data_t * dataP = NULL;

    NW_LOG("object read: [%s]", p_parse_cmd->string_ptr);
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;

            param_cnt = apb_nw_util_parse_all_parameters(start, &start, "i,i,i,i", &lwm2m_id, &object_id,
                    &instance_id, &resource_cnt);

            if (param_cnt != 4) {
                NW_LOG("Check parameter number != 4");
                goto exit;
            }

            NW_LOG("lwm2m_id:%d, object_id:%d, instance_id:%d, resource_cnt:%d",
                    lwm2m_id, object_id, instance_id, resource_cnt);
            lwm2m_t = lwm2m_atcmd_find_lwm2m(lwm2m_id);
            if (lwm2m_t == NULL){
                NW_LOG("goto exit 2");
                goto exit;
            }

            contextP = lwm2m_t->context;
            if (contextP == NULL) {
                NW_LOG("goto exit 3");
                goto exit;
            }

            dataP = lwm2m_data_new(resource_cnt);
            if (dataP == NULL) {
                NW_LOG("Cannot create dataP");
                goto exit;
            }

            for (i = 0; i < resource_cnt; i++) {

                param_cnt = apb_nw_util_parse_all_parameters(start, &start, "i,c,l,s",
                        &resource_id, &value_type, &value_len, &value_str);
                if (param_cnt != 4) {
                    NW_LOG("param_cnt != 4, error");
                    goto exit;
                }

                NW_LOG("resource_id = %d, type = %c, value_len = %d, value string  = %s",
                        resource_id, value_type, value_len, value_str);

                (dataP + i)->id = resource_id;
                switch (value_type) {
                    case 'S':
                        lwm2m_data_encode_string(value_str, dataP + i);
                        break;
                    case 'D':
                        if (value_len > 0) {
                            buffer = _alloc_buffer(value_len / 2);
                            if (buffer != NULL) {
                                _get_data_from_hex(buffer, value_str, value_len / 2);
                            } else {
                                value_len = 0; // avoid error
                            }
                        }
                        lwm2m_data_encode_opaque(buffer, value_len / 2, dataP + i);
                        _free_buffer(buffer);
                        break;
                    case 'I':
                        if (apb_nw_util_is_pure_int_string(value_str)) {
                            lwm2m_data_encode_int(atoi(value_str), dataP + i);
                        } else {
                            goto exit;
                        }
                        break;
                    case 'F':
                        lwm2m_data_encode_float(atof(value_str), dataP + i);
                        break;
                    case 'B':
                        lwm2m_data_encode_bool((atoi(value_str) == 0) ? false : true, dataP + i);
                        break;
                    default:
                        (dataP + i)->type = LWM2M_TYPE_UNDEFINED;
                        break;
                }
            }

            // TODO: flag
            /*
            result = message_send_value_by_app(contextP, 
                                    g_lwm2m_atcmd_t[0].connP,
                                    msg_id, 
                                    object_id, 
                                    instance_id, 
                                    resource_id, 
                                    dataP);
                                    */
            if (lwm2m_t->read_result_list != NULL) {
                lwm2m_data_free(lwm2m_t->read_result_cnt, lwm2m_t->read_result_list);
            }

            lwm2m_t->read_result_list = dataP;
            lwm2m_t->read_result_cnt = resource_cnt;

            result = (xSemaphoreGive(lwm2m_t->readWriteSemph) == pdTRUE) ? 0 : -1;

            if (result == 0)
            {    
                cmd_result.result_code = APB_PROXY_RESULT_OK;
                ret = APB_PROXY_STATUS_OK;
                dataP = NULL;
            }
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }
        default: {
            break;
        }
    }

    exit:
    if (dataP) {
        lwm2m_data_free(resource_cnt, dataP);
        lwm2m_t->read_result_cnt = 0;
        lwm2m_t->read_result_list = NULL;
    }

    cmd_result.length = cmd_result.pdata != NULL ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);

    return ret;
}

apb_proxy_status_t apb_proxy_hdlr_lwm2m_write_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;
    int lwm2m_id, result;
    char* start = NULL;
    struct lwm2m_atcmd_t* lwm2m_t = NULL;
    int32_t param_cnt = 0;
    int32_t write_result = 0;

    NW_LOG("write cmd: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;

            param_cnt = apb_nw_util_parse_all_parameters(start, NULL, "i,i", &lwm2m_id, &write_result);
            if (param_cnt != 2) {
                NW_LOG("parameter cnt != 2");
                goto exit;
            }

            lwm2m_t = lwm2m_atcmd_find_lwm2m(lwm2m_id);
            if (lwm2m_t == NULL || lwm2m_t->context == NULL) {
                NW_LOG("goto exit 2");
                goto exit;
            }

            lwm2m_t->write_result = write_result;

            result = (xSemaphoreGive(lwm2m_t->readWriteSemph) == pdTRUE) ? 0 : -1;

            if (result == 0)
            {
                cmd_result.result_code = APB_PROXY_RESULT_OK;
                ret = APB_PROXY_STATUS_OK;
            } else {
                NW_LOG("apb_proxy_hdlr_lwm2m_write_cmd give semph error:  %d", result);
            }
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }
        default: {
            break;
        }
    }

    exit:

    cmd_result.length = cmd_result.pdata != NULL ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
    return ret;
}


apb_proxy_status_t apb_proxy_hdlr_lwm2m_execute_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;
    int lwm2m_id, result = 0;
    char* start = NULL;
    struct lwm2m_atcmd_t* lwm2m_t = NULL;
    int32_t param_cnt = 0;
    int32_t write_result = 0;

    NW_LOG("write cmd: [%s]", p_parse_cmd->string_ptr);
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;
            param_cnt = apb_nw_util_parse_all_parameters(start, NULL, "i,i", &lwm2m_id, &write_result);
            if (param_cnt != 2) {
                NW_LOG("parameter cnt != 2");
                goto exit;
            }

            lwm2m_t = lwm2m_atcmd_find_lwm2m(lwm2m_id);
            if (lwm2m_t == NULL || lwm2m_t->context == NULL) {
                NW_LOG("goto exit 2");
                goto exit;
            }

            lwm2m_t->write_result = write_result;

            result = (xSemaphoreGive(lwm2m_t->readWriteSemph) == pdTRUE) ? 0 : -1;
            if (result == 0)
            {
                cmd_result.result_code = APB_PROXY_RESULT_OK;
                ret = APB_PROXY_STATUS_OK;
            } else {
                NW_LOG("apb_proxy_hdlr_lwm2m_execute_cmd give semph error:  %d", result);
            }
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }
        default: {
            break;
        }
    }

    exit:
    cmd_result.length = cmd_result.pdata != NULL ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
    return ret;
}

apb_proxy_status_t apb_proxy_hdlr_lwm2m_notify_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    apb_proxy_status_t ret =APB_PROXY_STATUS_ERROR;
    int lwm2m_id, object_id, instance_id = -1, resource_id = -1;
    char* start = NULL;
    struct lwm2m_atcmd_t* lwm2m_t = NULL;
    lwm2m_uri_t uri = { 0 };
    int32_t param_cnt = 0;
    lwm2m_context_t *contextP = NULL;

    NW_LOG("object read: [%s]", p_parse_cmd->string_ptr);
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;

            param_cnt = apb_nw_util_parse_all_parameters(start, NULL, "i,i,i,i", &lwm2m_id, &object_id,
                    &instance_id, &resource_id);

            if (param_cnt < 2) {
                NW_LOG("param_cnt < 2");
                goto exit;
            }
            lwm2m_t = lwm2m_atcmd_find_lwm2m(lwm2m_id);
            if (lwm2m_t == NULL || lwm2m_t->context == NULL){
                NW_LOG("goto exit 2");
                goto exit;
            }
            contextP = lwm2m_t->context;

            uri.objectId = object_id;
            uri.flag |= LWM2M_URI_FLAG_OBJECT_ID;

            if (instance_id < 0) {
                NW_LOG("instance_id %d", instance_id);
                uri.instanceId = instance_id;
                uri.flag |= LWM2M_URI_FLAG_INSTANCE_ID;
            }

            if (resource_id < 0) {
                NW_LOG("resource_id = %d", instance_id);
                uri.resourceId = resource_id;
                uri.flag |= LWM2M_URI_FLAG_RESOURCE_ID;
            }

            lwm2m_resource_value_changed(contextP, &uri);
            cmd_result.result_code = APB_PROXY_RESULT_OK;
            ret = APB_PROXY_STATUS_OK;
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }
        default: {
            break;
        }
    }

    exit:
    cmd_result.length = cmd_result.pdata != NULL ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
    return ret;
}


apb_proxy_status_t apb_proxy_hdlr_lwm2m_query_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;
    NW_LOG("query lwm2m: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
    ret = APB_PROXY_STATUS_OK;
    return ret;
}

#else

#include "apb_proxy.h"
#include "apb_proxy_nw_cmd.h"

#define NW_LOG(fmt, args...)               printf("[APB LWM2M] "fmt, ##args)

void lwm2m_atcmd_init_task(void)
{
}

void apb_proxy_hdlr_lwm2m_config_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    
    NW_LOG("query lwm2m: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
}

void apb_proxy_hdlr_lwm2m_delete_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    
    NW_LOG("query lwm2m: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
}

void apb_proxy_hdlr_lwm2m_addobj_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    
    NW_LOG("query lwm2m: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
}

void apb_proxy_hdlr_lwm2m_delobj_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    
    NW_LOG("query lwm2m: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
}

void apb_proxy_hdlr_lwm2m_read_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    
    NW_LOG("query lwm2m: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
}

void apb_proxy_hdlr_lwm2m_write_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    
    NW_LOG("query lwm2m: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
}

void apb_proxy_hdlr_lwm2m_execute_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    
    NW_LOG("query lwm2m: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
}

void apb_proxy_hdlr_lwm2m_notify_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    
    NW_LOG("query lwm2m: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
}

void apb_proxy_hdlr_lwm2m_query_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    
    NW_LOG("query lwm2m: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
}

#endif


