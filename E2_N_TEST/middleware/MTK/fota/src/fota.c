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

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "string.h"
#include "fota_http_dl.h"
#include "stdlib.h"
#include "stdio.h"
#include "fota.h"
#include "fota_timer.h"
#include "syslog.h"
#include "hal_flash.h"
#include "fota_protocol.h"
#include "fota_nw_manager.h"
#ifdef MTK_GNSS_FOTA_ENABLE
#include "gnss_api.h"
#include "gnss_fota_main.h"
#include "gnss_fota_bin.h"
#endif
#include "fota_coap_dl.h"

log_create_module(fota, PRINT_LEVEL_INFO);

/******************************************************************************************
 *                               Local Macro Definition                                   *
 *****************************************************************************************/
#define FOTA_ERR(fmt,arg...)   LOG_E(fota, "[FOTA]: "fmt,##arg)
#define FOTA_WARN(fmt,arg...)  LOG_W(fota, "[FOTA]: "fmt,##arg)
#define FOTA_DBG(fmt,arg...)   LOG_I(fota,"[FOTA]: "fmt,##arg)

#define FOTA_RESERVED_PARTITION_END_ADDRESS  (FOTA_RESERVED_BASE + FOTA_RESERVED_LENGTH - FLASH_BASE)
#define FOTA_HEADER_MAGIC_PATTERN            0x004D4D4D
#define FOTA_TRIGGER_FLAG_ADDRESS            (FOTA_RESERVED_PARTITION_END_ADDRESS - 512)
#define FOTA_UPDATE_INFO_RESERVE_SIZE        (512)      /*512byts */
#define FLASH_BLOCK_SIZE                     (1 << 12)
#define FOTA_DL_4K_FLASH_BLOCK               (1024 << 2)
#define FOTA_FILE_MD5_LEN                    16 /* The unit is byte.*/
#define FOTA_CLINET_MAX_COUNT                8
/******************************************************************************************
 *                               Type Definitions                                         *
 *****************************************************************************************/

typedef struct {
    uint32_t total_len;
    uint32_t received_len;
    uint32_t erased_len;
    uint32_t md5_len;
    uint8_t* p_md5_data;
} fota_download_context_t;
static fota_download_context_t g_fota_download_context = {0};
typedef enum {
    FOTA_IDLE        = 0,
    FOTA_DOWNLOAD    = 1,
    FOTA_UPGRADE     = 2,
    FOTA_REBOOTING   = 3,
    FOTA_REGISTER_NW = 4
}fota_state_t;

typedef struct {
    xSemaphoreHandle        fota_mutex;
    QueueHandle_t           msg_queue;
    fota_state_t            current_state;
    fota_event_indication_t event_ind[FOTA_CLINET_MAX_COUNT];
}fota_context_t;

typedef enum {
    FOTA_MSG_DOWNLOAD_IMAGE_REQ,
    FOTA_MSG_TRIGGER_UPGRADE_REQ,
    FOTA_MSG_TRIGGER_REBOOT,
}fota_internal_msg_event_t;

typedef struct {
    fota_image_t image_type;
    uint32_t uri_len;
    uint8_t* p_uri;
}fota_image_info_t;

typedef struct {
    fota_internal_msg_event_t event;
    union {
        fota_image_t image_type;
        fota_image_info_t image_info;
    } fota_event_info;
}fota_internal_msg_t;

static fota_context_t g_fota_context;
/******************************************************************************************
 *                               Local Function's Definitions                             *
 ******************************************************************************************/
static void fota_task_main(void);
static void fota_image_download(fota_image_info_t* p_image_info);
static void fota_image_upgrade(fota_image_t image_type);
static bool fota_http_dl_event_call_back(fota_http_dl_event_t event, const void* p_event_data);
static void fota_set_current_state(fota_state_t state);
static void fota_send_event_ind(fota_msg_event_t event, fota_msg_event_info_t* info);
static fota_result_t get_fota_result_code(fota_bl_status_t result_code);
static bool fota_coap_dl_event_call_back(fota_coap_dl_event_t event, const void* p_event_data);
#ifdef MTK_GNSS_FOTA_ENABLE
static void gnss_fota_progress(gnss_fota_state_t fota_state, uint32_t percent);
static void gnss_fota_final_result_ind(gnss_fota_result_t result);
static void gnss_fota_upgrade(void);
#endif
/******************************************************************************************
 *                               Public Function's Implementations                        *
 ******************************************************************************************/
void fota_init(void)
{
    memset(&g_fota_context, 0, sizeof(g_fota_context));
    g_fota_context.fota_mutex = xSemaphoreCreateMutex();
    if (g_fota_context.fota_mutex == NULL){
        configASSERT(0);
    }
    g_fota_context.msg_queue = xQueueCreate(FOTA_QUEUE_LENGTH, sizeof(fota_internal_msg_t));
    if( g_fota_context.msg_queue == NULL ) {
        configASSERT(0);
    }

    xTaskCreate(fota_task_main,
                FOTA_TASK_NAME,
                FOTA_TASK_STACKSIZE / ((uint32_t)sizeof(StackType_t)),
                NULL, FOTA_TASK_PRIORITY, NULL);
}

fota_event_handle_t fota_register_event(fota_event_indication_t call_back)
{
    fota_event_handle_t handle = 0;
    uint32_t index = 0;
    if (NULL == g_fota_context.fota_mutex){
        return handle;
    }
    if (pdTRUE == xSemaphoreTake(g_fota_context.fota_mutex, 0U)) {
        for(index = 0; index < FOTA_CLINET_MAX_COUNT; index ++){
            if (g_fota_context.event_ind[index] == NULL){
                handle = index + 1;
                g_fota_context.event_ind[index] = call_back;
                break;
            }
        }
        xSemaphoreGive(g_fota_context.fota_mutex);
    }
    return handle;
}

void fota_deregister_event(fota_event_handle_t handle)
{
    uint32_t index = (uint32_t)handle - 1;
    if (NULL == g_fota_context.fota_mutex){
        return handle;
    }
    if (index >= FOTA_CLINET_MAX_COUNT){
        return;
    }

    if (pdTRUE == xSemaphoreTake(g_fota_context.fota_mutex, 0U)) {
        g_fota_context.event_ind[index] = NULL;
        xSemaphoreGive(g_fota_context.fota_mutex);
    }
    return handle;
}

fota_result_t fota_download_image(fota_image_t image, uint8_t* image_uri)
{
    bool need_download = false;
    fota_internal_msg_t fota_event = {0};
    FOTA_DBG("image URI:%s", image_uri);

    if (NULL == g_fota_context.fota_mutex){
        return FOTA_COMMON_ERROR;
    }
    if (pdTRUE == xSemaphoreTake(g_fota_context.fota_mutex, 0U)) {
        if (g_fota_context.current_state == FOTA_IDLE){
            need_download = true;
            g_fota_context.current_state = FOTA_DOWNLOAD;
        }
        xSemaphoreGive(g_fota_context.fota_mutex);
    }else{
        return FOTA_COMMON_ERROR;
    }

    if (false == need_download){
        return FOTA_BUSY;
    }

    /*Send the message to FOTA task.*/
    if (g_fota_context.msg_queue == NULL){
        return FOTA_COMMON_ERROR;
    }
    fota_event.event = FOTA_MSG_DOWNLOAD_IMAGE_REQ;
    fota_event.fota_event_info.image_info.image_type = image;
    fota_event.fota_event_info.image_info.uri_len = strlen(image_uri) + 1;
    fota_event.fota_event_info.image_info.p_uri = (uint8_t *)pvPortMalloc(fota_event.fota_event_info.image_info.uri_len);
    if (fota_event.fota_event_info.image_info.p_uri == NULL) {
        return FOTA_COMMON_ERROR;
    }
    strcpy(fota_event.fota_event_info.image_info.p_uri, image_uri);
    fota_event.fota_event_info.image_info.p_uri[fota_event.fota_event_info.image_info.uri_len -1] = 0;

    if (xQueueSend(g_fota_context.msg_queue, &fota_event, 0U) != pdPASS) {
        configASSERT(0);
        vPortFree(fota_event.fota_event_info.image_info.p_uri);
        return FOTA_COMMON_ERROR;
    }
    return FOTA_OK;

}

fota_result_t fota_trigger_upgrade(fota_image_t image)
{
    bool need_upgrade = false;
    fota_internal_msg_t fota_event = {0};
    FOTA_DBG("image type:%d", image);
    if (NULL == g_fota_context.fota_mutex){
        return FOTA_COMMON_ERROR;
    }
#ifndef MTK_GNSS_FOTA_ENABLE
    if (image == FOTA_GNSS_BIN){
        return FOTA_ERROR_OPERATION_INVALID;
    }
#endif
    if (pdTRUE == xSemaphoreTake(g_fota_context.fota_mutex, 0U)) {
        if (g_fota_context.current_state == FOTA_IDLE){
            need_upgrade = true;
            g_fota_context.current_state = FOTA_UPGRADE;
        }
        xSemaphoreGive(g_fota_context.fota_mutex);
    }else{
        return FOTA_COMMON_ERROR;
    }

    if (false == need_upgrade){
        return FOTA_BUSY;
    }

    /*Send the message to FOTA task.*/
    if (g_fota_context.msg_queue == NULL){
        return FOTA_COMMON_ERROR;
    }
    fota_event.event = FOTA_MSG_TRIGGER_UPGRADE_REQ;
    fota_event.fota_event_info.image_type = image;
    if (xQueueSend(g_fota_context.msg_queue, &fota_event, 0U) != pdPASS) {
        configASSERT(0);
        return FOTA_COMMON_ERROR;
    }
    return FOTA_OK;
}

fota_result_t fota_read_upgrade_result(void)
{
    fota_bl_status_t result_code;
    fota_result_t result = FOTA_COMMON_ERROR;
    if (fota_read_fota_result(&result_code) == true){
        result = get_fota_result_code(result_code);
    }
    return result;
}

fota_result_t fota_trigger_reboot(void)
{
    bool need_reboot = false;
    fota_internal_msg_t fota_event = {0};
    if (NULL == g_fota_context.fota_mutex){
        return FOTA_COMMON_ERROR;
    }
    if (pdTRUE == xSemaphoreTake(g_fota_context.fota_mutex, 0U)) {
        if (g_fota_context.current_state == FOTA_IDLE){
            need_reboot = true;
            g_fota_context.current_state = FOTA_REBOOTING;
        }
        xSemaphoreGive(g_fota_context.fota_mutex);
    }else{
        return FOTA_COMMON_ERROR;
    }

    if (false == need_reboot){
        return FOTA_BUSY;
    }

    /*Send the message to FOTA task.*/
    if (g_fota_context.msg_queue == NULL){
        return FOTA_COMMON_ERROR;
    }
    fota_event.event = FOTA_MSG_TRIGGER_REBOOT;
    if (xQueueSend(g_fota_context.msg_queue, &fota_event, 0U) != pdPASS) {
        configASSERT(0);
        return FOTA_COMMON_ERROR;
    }
    return FOTA_OK;
}

/******************************************************************************************
 *                               Local Function's Implementations                         *
 ******************************************************************************************/
static void fota_task_main(void)
{
    fota_internal_msg_t received_msg;
    fota_bl_status_t result_code;
    fota_msg_event_info_t event_info;
    fota_result_t result = FOTA_COMMON_ERROR;
    fota_nw_manager_init();
    if (fota_is_executed() == true){
        if (fota_read_fota_result(&result_code) == true){
            result = get_fota_result_code(result_code);
            event_info.fota_result = result;
            fota_send_event_ind(FOTA_MSG_UPGRADE_RESULT_IND, &event_info);
        }
    }

    while (1) {
        if (xQueueReceive(g_fota_context.msg_queue, &received_msg, portMAX_DELAY)) {
            switch(received_msg.event){
                case FOTA_MSG_DOWNLOAD_IMAGE_REQ:{
                    fota_image_info_t* p_image_info = &(received_msg.fota_event_info.image_info);
                    fota_nw_status_t nw_status = fota_nw_manager_nw_status();
                    if (nw_status == FOTA_NW_ACTIVED){
                        fota_image_download(p_image_info);
                    }else if (nw_status == FOTA_NW_ACTIVING){
                        event_info.fota_result = FOTA_ERROR_NW_ACTIVEING;
                        fota_send_event_ind(FOTA_MSG_DOWNLOAD_RESULT_IND, &event_info);
                    }else{
                        uint32_t index = 0, max_count = 60;
                        fota_nw_manager_active_network();
                        /*waiting for network ready.*/
                        for(index = 0; index < max_count; index ++){
                            nw_status = fota_nw_manager_nw_status();
                            if (nw_status != FOTA_NW_ACTIVING){
                                break;
                            }
                            /*Delay 1s.*/
                            vTaskDelay(100);
                        }
                        if (nw_status == FOTA_NW_ACTIVED){
                            fota_image_download(p_image_info);
                        }else{
                            event_info.fota_result = FOTA_ERROR_NW_DEACTIVE;
                            fota_send_event_ind(FOTA_MSG_DOWNLOAD_RESULT_IND, &event_info);
                        }
                    }
                    vPortFree(p_image_info->p_uri);
                    p_image_info->p_uri = NULL;
                    fota_set_current_state(FOTA_IDLE);
                    break;
                }
                case FOTA_MSG_TRIGGER_UPGRADE_REQ:{
                    fota_image_upgrade(received_msg.fota_event_info.image_type);
                    fota_set_current_state(FOTA_IDLE);
                    break;
                }
                case FOTA_MSG_TRIGGER_REBOOT:{
                    fota_reboot_device_timer(2000);
                    break;
                }
                default:{
                    break;
                }
            }
        }
    }
}

static void fota_image_download(fota_image_info_t* p_image_info)
{
    char* fota_uri = (char*)(p_image_info->p_uri);
    char* http_schem = "HTTP";
    char* coap_schem = "COAP";
    char* find_pos = NULL;
    char  uri_schem[8] = {0};
    fota_msg_event_info_t event_info;
    uint32_t index = 0;
    const uint32_t URI_SCHEM_LEN = 4;

    memcpy(uri_schem, p_image_info->p_uri, URI_SCHEM_LEN);
    for(index = 0; index < URI_SCHEM_LEN; index++ ) {
        uri_schem[index] = toupper(uri_schem[index]);
    }
    if (strstr(uri_schem, http_schem) != NULL) {
        fota_http_dl_result_t http_dl_result = FOTA_HTTP_DL_ERROR;
        fota_http_dl_t* p_fota_http = fota_http_dl_init();
        fota_http_dl_event_call_back_t p_dl_callback = fota_http_dl_event_call_back;
        if (p_fota_http->fota_http_dl_set_parameter(p_fota_http, p_image_info->p_uri, p_dl_callback)== false){
            FOTA_ERR("set param Err.");
        }else{
            http_dl_result = p_fota_http->fota_http_dl_run(p_fota_http);
            FOTA_DBG("DL result code: %d", http_dl_result);
            if (http_dl_result == FOTA_HTTP_DL_OK){
                event_info.fota_result = FOTA_OK;
            }else{
                event_info.fota_result = FOTA_ERROR_DOWNLOAD_FAILED;
            }
            fota_send_event_ind(FOTA_MSG_DOWNLOAD_RESULT_IND, &event_info);
        }
        fota_http_dl_destroy(&p_fota_http);
    } else if (strstr(uri_schem, coap_schem) != NULL) {
        fota_coap_dl_result_t coap_dl_result = FOTA_COAP_DL_ERROR;
        fota_coap_dl_t* p_fota_coap = fota_coap_dl_init();
        fota_coap_dl_event_call_back_t p_dl_callback = fota_coap_dl_event_call_back;
        g_fota_download_context.received_len = 0;

        if (p_fota_coap->fota_coap_dl_set_parameter(p_fota_coap, p_image_info->p_uri, p_dl_callback)== false){
            FOTA_ERR("fota coap set param Err.");
        }else{
            coap_dl_result = p_fota_coap->fota_coap_dl_run(p_fota_coap);
            FOTA_DBG("fota coap DL result code: %d", coap_dl_result);
            if (coap_dl_result == FOTA_COAP_DL_OK){
                event_info.fota_result = FOTA_OK;
            }else{
                event_info.fota_result = FOTA_ERROR_DOWNLOAD_FAILED;
            }
            fota_send_event_ind(FOTA_MSG_DOWNLOAD_RESULT_IND, &event_info);
        }
        fota_coap_dl_destroy(&p_fota_coap);
    } else {
        FOTA_DBG("unsupported URI format\r\n");
        event_info.fota_result = FOTA_COMMON_ERROR;
        fota_send_event_ind(FOTA_MSG_DOWNLOAD_RESULT_IND, &event_info);
    }

}

static void fota_image_upgrade(fota_image_t image_type)
{
    switch(image_type){
        case FOTA_MAIN_BIN:{
            fota_result_t fota_result = fota_trigger_main_bin_update();
            if (fota_result == FOTA_OK){
                fota_reboot_device_timer(2000);
            }else{
                fota_msg_event_info_t event_info;
                event_info.fota_result = fota_result;
                fota_send_event_ind(FOTA_MSG_UPGRADE_RESULT_IND, &event_info);
            }
            break;
        }
#ifdef MTK_GNSS_FOTA_ENABLE
        case FOTA_GNSS_BIN:{
            gnss_fota_upgrade();
            break;
        }
#endif
        case FOTA_ALL_BIN: {
            fota_image_t image_type = fota_get_current_available_package();
            switch(image_type){
                case FOTA_MAIN_BIN: {
                    fota_result_t fota_result = fota_trigger_main_bin_update();
                    if (fota_result == FOTA_OK){
                        fota_reboot_device_timer(2000);
                    }else{
                        fota_msg_event_info_t event_info;
                        event_info.fota_result = fota_result;
                        fota_send_event_ind(FOTA_MSG_UPGRADE_RESULT_IND, &event_info);
                    }
                    break;
                }
#ifdef MTK_GNSS_FOTA_ENABLE
                case FOTA_GNSS_BIN: {
                    gnss_fota_upgrade();
                    break;
                }
#endif
                default: {
                    fota_msg_event_info_t event_info;
                    event_info.fota_result = FOTA_ERROR_WRONG_PACKAGE;
                    fota_send_event_ind(FOTA_MSG_UPGRADE_RESULT_IND, &event_info);
                    break;
                }
            }
        }
        default:{
            break;
        }
    }
}

static bool fota_coap_dl_event_call_back(fota_coap_dl_event_t event, const void* p_event_data)
{
    bool result = true;
    switch(event){
        case FOTA_COAP_DL_DATA_EVENT:{
            hal_flash_status_t flash_result;
            fota_msg_event_info_t event_info;
            uint32_t start_address = 0;
            uint32_t precent = 0;
            uint32_t temp_length = 0;
            fota_coap_dl_data_event_t* p_event = (fota_coap_dl_data_event_t*)(p_event_data);
#if 0 // todo: not ready.
            if (0 == g_fota_download_context.total_len){
                return false;
            }
#endif

            temp_length = g_fota_download_context.received_len + p_event->buf_len;
            if ((temp_length < (FOTA_RESERVED_LENGTH - FOTA_DL_4K_FLASH_BLOCK))
                && temp_length > g_fota_download_context.erased_len){
                FOTA_DBG("fota coap dl erase flash\r\n");
                fota_erase_flash(FOTA_RESERVED_BASE + g_fota_download_context.erased_len, FOTA_DL_4K_FLASH_BLOCK);
                g_fota_download_context.erased_len += FOTA_DL_4K_FLASH_BLOCK;
            }
            start_address = FOTA_RESERVED_BASE - FLASH_BASE + g_fota_download_context.received_len;
            flash_result = hal_flash_write(start_address, p_event->p_buf, p_event->buf_len);
            g_fota_download_context.received_len += p_event->buf_len;
            FOTA_DBG("dl data event");
            FOTA_DBG("writted len: %lu", g_fota_download_context.received_len);
            if (flash_result != HAL_FLASH_STATUS_OK){
                FOTA_DBG("error code: %d", flash_result);
                result = false;
            }
#if 0 // todo: not ready.
            precent = (uint32_t)((((float)(g_fota_download_context.received_len)) / (float)(g_fota_download_context.total_len)) * 100);
            event_info.progress = precent;
            fota_send_event_ind(FOTA_MSG_DOWNLOAD_PROGRESS_IND, &event_info);
#endif
            break;
        }
        case FOTA_COAP_DL_DATA_TOTAL_LEN_EVENT:{
            uint32_t* p_event = (uint32_t*)p_event_data;
            g_fota_download_context.received_len = 0;
            g_fota_download_context.total_len = *p_event;
            FOTA_DBG(" package total length: %lu", g_fota_download_context.total_len);
            if (g_fota_download_context.total_len > (FOTA_RESERVED_LENGTH - FOTA_DL_4K_FLASH_BLOCK)){
                FOTA_ERR("packet length is too large.");
                g_fota_download_context.total_len = 0;
                result = false;
            }else{
                fota_erase_flash(FOTA_RESERVED_BASE, FOTA_DL_4K_FLASH_BLOCK);
                g_fota_download_context.erased_len = FOTA_DL_4K_FLASH_BLOCK;
            }
            break;
        }
    }
    return result;
}

static bool fota_http_dl_event_call_back(fota_http_dl_event_t event, const void* p_event_data)
{
    bool result = true;
    switch(event){
        case FOTA_HTTP_DL_DATA_EVENT:{
            hal_flash_status_t flash_result;
            fota_msg_event_info_t event_info;
            uint32_t start_address = 0;
            uint32_t precent = 0;
            uint32_t temp_length = 0;
            fota_http_dl_data_event_t* p_event = (fota_http_dl_data_event_t*)(p_event_data);
            if (0 == g_fota_download_context.total_len){
                return false;
            }

            temp_length = g_fota_download_context.received_len + p_event->buf_len;
            if ((temp_length < (FOTA_RESERVED_LENGTH - FOTA_DL_4K_FLASH_BLOCK))
                && temp_length > g_fota_download_context.erased_len){
                fota_erase_flash(FOTA_RESERVED_BASE + g_fota_download_context.erased_len, FOTA_DL_4K_FLASH_BLOCK);
                g_fota_download_context.erased_len += FOTA_DL_4K_FLASH_BLOCK;
            }
            start_address = FOTA_RESERVED_BASE - FLASH_BASE + g_fota_download_context.received_len;
            flash_result = hal_flash_write(start_address, p_event->p_buf, p_event->buf_len);
            g_fota_download_context.received_len += p_event->buf_len;
            FOTA_DBG("dl data event");
            FOTA_DBG("writted len: %lu", g_fota_download_context.received_len);
            if (flash_result != HAL_FLASH_STATUS_OK){
                FOTA_DBG("error code: %d", flash_result);
                result = false;
            }
            precent = (uint32_t)((((float)(g_fota_download_context.received_len)) / (float)(g_fota_download_context.total_len)) * 100);
            event_info.progress = precent;
            fota_send_event_ind(FOTA_MSG_DOWNLOAD_PROGRESS_IND, &event_info);
            break;
        }
        case FOTA_HTTP_DL_DATA_TOTAL_LEN_EVENT:{
            uint32_t* p_event = (uint32_t*)p_event_data;
            g_fota_download_context.received_len = 0;
            g_fota_download_context.total_len = *p_event;
            FOTA_DBG(" package total length: %lu", g_fota_download_context.total_len);
            if (g_fota_download_context.total_len > (FOTA_RESERVED_LENGTH - FOTA_DL_4K_FLASH_BLOCK)){
                FOTA_ERR("packet length is too large.");
                g_fota_download_context.total_len = 0;
                result = false;
            }else{
                fota_erase_flash(FOTA_RESERVED_BASE, FOTA_DL_4K_FLASH_BLOCK);
                g_fota_download_context.erased_len = FOTA_DL_4K_FLASH_BLOCK;
            }
            break;
        }
    }
    return result;
}

static void fota_set_current_state(fota_state_t state)
{
    if (NULL == g_fota_context.fota_mutex){
        return;
    }
    if (pdTRUE == xSemaphoreTake(g_fota_context.fota_mutex, portMAX_DELAY)) {
        g_fota_context.current_state = state;
        xSemaphoreGive(g_fota_context.fota_mutex);
    }
}

static void fota_send_event_ind(fota_msg_event_t event, fota_msg_event_info_t* info)
{
    fota_event_indication_t ind_callback = NULL;
    uint32_t index = 0;
    if ((NULL == g_fota_context.fota_mutex) || (info == NULL)){
        return;
    }
    if (pdTRUE == xSemaphoreTake(g_fota_context.fota_mutex, portMAX_DELAY)) {
        for(index = 0; index < FOTA_CLINET_MAX_COUNT; index ++){
            ind_callback = g_fota_context.event_ind[index];
            if (ind_callback != NULL){
                ind_callback(event, info);
            }
        }
        xSemaphoreGive(g_fota_context.fota_mutex);
    }
}

static fota_result_t get_fota_result_code(fota_bl_status_t result_code)
{
    fota_result_t result = FOTA_COMMON_ERROR;
    switch(result_code){
        case FOTA_BL_ERROR_NONE: {
            result = FOTA_OK;
            break;
        }
        case FOTA_BL_ERROR_LOADING_HEADER: {
            result = FOTA_ERROR_LOADING_HEADER;
            break;
        }
        case FOTA_BL_ERROR_HEADER_FORMAT: {
            result = FOTA_ERROR_HEADER_FORMAT;
            break;
        }
        case FOTA_BL_ERROR_LOADING_BODY: {
            result = FOTA_ERROR_LOADING_BODY;
            break;
        }
        case FOTA_BL_ERROR_WRITE_TO_PARTITON: {
            result = FOTA_ERROR_WRITE_TO_PARTITON;
            break;
        }
        case FOTA_BL_ERROR_LOADING_CHECKSUM: {
            result = FOTA_ERROR_LOADING_CHECKSUM;
            break;
        }
        case FOTA_BL_ERROR_CHECKSUM_VERIFY: {
            result = FOTA_ERROR_CHECKSUM_VERIFY;
            break;
        }
        case FOTA_BL_ERROR_LOADING_MARKER: {
            result = FOTA_ERROR_LOADING_MARKER;
            break;
        }
        case FOTA_BL_ERROR_ERASE_MARKER: {
            result = FOTA_ERROR_ERASE_MARKER;
            break;
        }
        case FOTA_BL_ERROR_SEC_CHECK_ERROR: {
            result = FOTA_ERROR_SEC_CHECK_ERROR;
            break;
        }
        default: {
            break;
        }
    }
    return result;
}


#ifdef MTK_GNSS_FOTA_ENABLE
static void gnss_fota_progress(gnss_fota_state_t fota_state, uint32_t percent)
{
    uint32_t total_percent = 0;
    fota_msg_event_info_t event_info;
    switch(fota_state){
        case GNSS_BROM_STATE: {
            total_percent = (percent >> 1);
            break;
        }
        case GNSS_DA_STATE: {
            total_percent = ((percent >> 1) + 50);
            break;
        }
        default: {
            break;
        }
    }
    event_info.progress = total_percent;
    fota_send_event_ind(FOTA_MSG_UPGRADE_PROGRESS_IND, &event_info);
}

static void gnss_fota_final_result_ind(gnss_fota_result_t result)
{
    fota_result_t result_code;
    fota_msg_event_info_t event_info;
    switch(result){
        case GNSS_FOTA_OK: {
            result_code = FOTA_OK;
            break;
        }
        case GNSS_FOTA_BROM_ERROR: {
            result_code = FOTA_ERROR_BROM_ERROR;
            break;
        }
        case GNSS_FOTA_DA_ERROR: {
            result_code = FOTA_ERROR_DA_ERROR;
            break;
        }
        case GNSS_FOTA_PARAMETER_ERROR: {
            result_code = FOTA_ERROR_PARAMETER_INVALID;
            break;
        }
        case GNSS_FOTA_INVLID_PACKAGE: {
            result_code = FOTA_ERROR_WRONG_PACKAGE;
            break;
        }
        default: {
            result_code = FOTA_COMMON_ERROR;
            break;
        }
    }
    event_info.fota_result = result_code;
    fota_send_event_ind(FOTA_MSG_UPGRADE_RESULT_IND, &event_info);
}

static void gnss_fota_upgrade(void)
{
    gnss_fota_parameter_t gnss_fota_param;
    gnss_fota_result_t result = GNSS_FOTA_PARAMETER_ERROR;
    uint32_t start_address = 0;
    uint32_t bin_length = 0;
    if (fota_get_gnss_binary_info(&start_address, &bin_length) == true){
        gnss_fota_set_gnss_bin_info(start_address, bin_length);
        gnss_fota_param.gnss_da_data.gnss_fota_bin_init = gnss_fota_da_bin_init;
        gnss_fota_param.gnss_da_data.gnss_fota_get_bin_length = gnss_fota_get_da_bin_length;
        gnss_fota_param.gnss_da_data.gnss_fota_get_data = gnss_fota_get_da_data;

        gnss_fota_param.gnss_update_data.gnss_fota_bin_init = gnss_fota_gnss_bin_init;
        gnss_fota_param.gnss_update_data.gnss_fota_get_bin_length = gnss_fota_get_gnss_bin_length;
        gnss_fota_param.gnss_update_data.gnss_fota_get_data = gnss_fota_get_gnss_data;
        /* update progress is NULL, no progress data output.*/
        gnss_fota_param.gnss_update_progress= gnss_fota_progress;
        gnss_power_off();
        result = gnss_fota_main(&gnss_fota_param);
        gnss_fota_final_result_ind(result);
    }else{
        gnss_fota_final_result_ind(GNSS_FOTA_INVLID_PACKAGE);
    }
}
#endif
