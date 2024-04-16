/* Copyright Statement:
 *
 * (C) 2005-2017  MediaTek Inc. All rights reserved.
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

#include "apb_proxy_context_manager.h"
#include "apb_proxy_at_cmd_tbl.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"
#include "apb_proxy_log.h"
#include "string.h"
#include "apb_proxy_utility.h"
#include "apb_proxy_data_type.h"
#ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
#include "memory_map.h"
#include "hal_flash.h"
#include "hal_rtc_internal.h"
#include "hal_rtc_external.h"
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_platform.h"
#endif

/*************************************************************
 *********Local Macro Definition *****************************
 *************************************************************/
#define APB_PROXY_AT_CMD_INIT_CAPACITY 8U
#define APB_PROXY_AT_CMD_CONTEXT_MEMORY_INC 8U
#define APB_PROXY_AT_CMD_REGISTER_MSG_MAX_COUNT 24U
#ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
#define APB_PROXY_AT_CMD_SYNC_READY  0xF0E1D2C3
#define APB_PROXY_AT_CMD_DATA_OFFSET 20
#define APB_PROXY_AT_CMD_CHECKSUM_OFFSET 4
#define APB_PROXY_AT_CMD_CHECKSUM_FIELD_SIZE 4
#endif
/*************************************************************
 *********Local Data Structures Definition *******************
 *************************************************************/
typedef struct {
    uint16_t                    capacity;
    uint16_t                    registered_num;
    apb_proxy_at_cmd_context_t *p_at_cmd_context_tbl;
} apb_proxy_at_cmd_context_manager_t;

#ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
typedef struct {
    uint32_t check_sum;
    uint32_t buf_len;
    uint32_t data_sync_result;
    uint32_t cmd_count;
    uint32_t cmd_base_id;
    uint8_t* p_cmd_data;
} apb_proxy_at_cmd_data_t;
#endif
/*************************************************************
 *********Local Static variants Definition *******************
 *************************************************************/
static apb_proxy_at_cmd_context_manager_t  g_apb_proxy_at_cmd_context_manager;
static apb_proxy_context_t                 g_apb_proxy_context;
static apb_proxy_at_cmd_reg_cxt_t          g_apb_proxy_register_context;
static xSemaphoreHandle                    g_apb_proxy_mutex;
#ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
static apb_proxy_at_cmd_data_t             g_apb_proxy_at_data;
#endif
/*************************************************************
 *********Local Static Function Declare **********************
 *************************************************************/
static apb_proxy_status_t apb_proxy_at_cmd_context_manager_init(void);
#ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
static bool apb_proxy_load_cmd_data(apb_proxy_at_cmd_data_t* p_cmd_data);
#endif
/*************************************************************
 *********Local Static Function Implementation ***************
 *************************************************************/
static apb_proxy_status_t apb_proxy_at_cmd_context_manager_init(void)
{
    bool power_from_deep = false;
    g_apb_proxy_at_cmd_context_manager.capacity = APB_PROXY_AT_CMD_INIT_CAPACITY;
    g_apb_proxy_at_cmd_context_manager.p_at_cmd_context_tbl = (apb_proxy_at_cmd_context_t *)pvPortMalloc(
                g_apb_proxy_at_cmd_context_manager.capacity * sizeof(apb_proxy_at_cmd_context_t));

    memset(g_apb_proxy_at_cmd_context_manager.p_at_cmd_context_tbl,
           0, g_apb_proxy_at_cmd_context_manager.capacity * sizeof(apb_proxy_at_cmd_context_t));
    if (NULL == g_apb_proxy_at_cmd_context_manager.p_at_cmd_context_tbl) {
        configASSERT(0);
        return APB_PROXY_STATUS_ERROR;
    }
    apb_proxy_log_info("apb_proxy_at_cmd_context_manager_init \r\n");
    g_apb_proxy_at_cmd_context_manager.registered_num = 0U;
#ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
    if ((rtc_power_on_result_external() == DEEP_SLEEP) ||
        (rtc_power_on_result_external() == DEEPER_SLEEP)){
        power_from_deep = true;
    }
    if (power_from_deep == true) {
        memset(&g_apb_proxy_at_data, 0, sizeof(g_apb_proxy_at_data));
        if (apb_proxy_load_cmd_data(&g_apb_proxy_at_data) == true) {
            apb_proxy_cmd_hdlr_table_t* table = apb_proxy_get_cmd_hdlr_table_info();
            uint32_t index = 0;
            if (g_apb_proxy_at_data.cmd_count != table->item_size) {
                apb_proxy_log_error("cmd count does not match\r\n");
                return false;
            }
            g_apb_proxy_register_context.registered_count = g_apb_proxy_at_data.cmd_count;
            g_apb_proxy_register_context.is_all_cmd_registered = true;
            for(index = 0; index < g_apb_proxy_at_data.cmd_count; index ++) {
                (table->p_item_tbl[index]).cmd_id = g_apb_proxy_at_data.cmd_base_id + index;
            }
            apb_proxy_log_info("cmd sync ok\r\n");
        } else {
            apb_proxy_log_error("load command data error\r\n");
        }
    } else {
        apb_proxy_log_info("apb cold start\r\n");
    }
#endif
    return APB_PROXY_STATUS_OK;
}
#ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
static bool apb_proxy_load_cmd_data(apb_proxy_at_cmd_data_t* p_cmd_data)
{
    hal_flash_status_t flash_op_result;
    uint32_t check_sum = 0;

    if (p_cmd_data == NULL) {
        return false;
    }
    flash_op_result = hal_flash_read(APB_SLEEP_CONTEXT_BASE - FLASH_BASE,
                                     p_cmd_data, sizeof(apb_proxy_at_cmd_data_t));
    if (flash_op_result != HAL_FLASH_STATUS_OK) {
        apb_proxy_log_error("read flash error:%d", flash_op_result);
        return false;
    }
    apb_proxy_log_info("check_sum=0x%x, length:0x%x, sync:0x%x,cmd_count:0x%x, cmd_base:0x%x\r\n",
                       p_cmd_data->check_sum, p_cmd_data->buf_len, p_cmd_data->data_sync_result,
                       p_cmd_data->cmd_count, p_cmd_data->cmd_base_id);
    p_cmd_data->p_cmd_data = (uint8_t*)(APB_SLEEP_CONTEXT_BASE - APB_PROXY_AT_CMD_DATA_OFFSET);
    if (p_cmd_data->data_sync_result != APB_PROXY_AT_CMD_SYNC_READY) {
        return false;
    }
    if (p_cmd_data->buf_len > (APB_SLEEP_CONTEXT_LENGTH - APB_PROXY_AT_CMD_CHECKSUM_OFFSET)) {
        return false;
    }

    check_sum = apb_proxy_calc_checksum(APB_SLEEP_CONTEXT_BASE + APB_PROXY_AT_CMD_CHECKSUM_OFFSET, p_cmd_data->buf_len);
    if (check_sum != p_cmd_data->check_sum) {
        return false;
    }
    return true;
}

static void apb_proxy_erase_flash(uint32_t erase_addr, uint32_t length)
{
    uint32_t block_count = 0;
    uint32_t index = 0;
    uint32_t complement = 0;
    hal_flash_status_t erase_result;

    block_count = length >> 12;
    complement = length - (block_count << 12);
    if (complement != 0) {
        block_count = block_count + 1;
    }

    if (erase_addr >= FLASH_BASE){
        erase_addr = erase_addr - FLASH_BASE;
    }
    /*erase the block.*/
    for (index = 0; index < block_count; index ++) {
        erase_result = hal_flash_erase(erase_addr, HAL_FLASH_BLOCK_4K);
        if (HAL_FLASH_STATUS_OK != erase_result) {
            return;
        }
        erase_addr = erase_addr + (1024 << 2);
    }
}

void apb_proxy_store_cmd_data(void)
{
    apb_proxy_cmd_hdlr_table_t* table = apb_proxy_get_cmd_hdlr_table_info();
    uint32_t index = 0;
    apb_proxy_at_cmd_data_t at_data_context;
    uint32_t write_addr = APB_SLEEP_CONTEXT_BASE - FLASH_BASE;
    uint32_t data_len = 0;
    memset(&at_data_context, 0, sizeof(at_data_context));
    apb_proxy_erase_flash(APB_SLEEP_CONTEXT_BASE, APB_SLEEP_CONTEXT_LENGTH);
    at_data_context.cmd_count = table->item_size;
    at_data_context.cmd_base_id = table->p_item_tbl[0].cmd_id;
    at_data_context.buf_len = 0;
    at_data_context.check_sum = 0;
    at_data_context.data_sync_result = APB_PROXY_AT_CMD_SYNC_READY;
    write_addr = write_addr + APB_PROXY_AT_CMD_DATA_OFFSET;
    for(index = 0; index < at_data_context.cmd_count; index ++) {
        data_len = strlen(table->p_item_tbl[index].p_cmd_head + 2) + 1;
        if ((at_data_context.buf_len + data_len) > (APB_SLEEP_CONTEXT_LENGTH - APB_PROXY_AT_CMD_DATA_OFFSET)) {
            apb_proxy_log_error("length overflow \r\n");
            return;
        }
        apb_proxy_log_info("write_addr:0x%x, len:%d, string:%s\r\n",
                            write_addr, data_len, table->p_item_tbl[index].p_cmd_head);
        hal_flash_write(write_addr, table->p_item_tbl[index].p_cmd_head + 2, data_len);
        at_data_context.buf_len = at_data_context.buf_len + data_len;
        write_addr = write_addr + data_len;
    }
    at_data_context.buf_len = at_data_context.buf_len + APB_PROXY_AT_CMD_DATA_OFFSET - APB_PROXY_AT_CMD_CHECKSUM_FIELD_SIZE ;
    write_addr = APB_SLEEP_CONTEXT_BASE - FLASH_BASE + APB_PROXY_AT_CMD_CHECKSUM_FIELD_SIZE;
    hal_flash_write(write_addr, &(at_data_context.buf_len), sizeof(at_data_context.buf_len));
    write_addr = write_addr + 4;
    hal_flash_write(write_addr, &(at_data_context.data_sync_result), sizeof(at_data_context.data_sync_result));
    write_addr = write_addr + 4;
    hal_flash_write(write_addr, &(at_data_context.cmd_count), sizeof(at_data_context.cmd_count));
    write_addr = write_addr + 4;
    hal_flash_write(write_addr, &(at_data_context.cmd_base_id), sizeof(at_data_context.cmd_base_id));
    at_data_context.check_sum = apb_proxy_calc_checksum(APB_SLEEP_CONTEXT_BASE + APB_PROXY_AT_CMD_CHECKSUM_OFFSET,
                                at_data_context.buf_len);
    apb_proxy_log_info("store checksum:0x%x, buf_len:%d\r\n", at_data_context.check_sum,
                       at_data_context.buf_len);
    hal_flash_write(APB_SLEEP_CONTEXT_BASE - FLASH_BASE, &(at_data_context.check_sum), sizeof(at_data_context.check_sum));
}

#endif
/********************************************************************************
 **                  Global Public Function Implementation                     **
 *******************************************************************************/

apb_proxy_data_mode_t* apb_proxy_get_data_mode_context(uint32_t channel_id)
{
    apb_proxy_data_mode_t* data_mode_ctx = NULL;
    uint32_t index = 0;
    if (NULL != g_apb_proxy_mutex) {
        if (pdTRUE == xSemaphoreTake(g_apb_proxy_mutex, portMAX_DELAY)) {
            for(index = 0; index < APB_PROXY_SUPPORT_DATA_MODE_MAX; index ++) {
                data_mode_ctx = g_apb_proxy_context.apb_data_mode_context + index;
                if (data_mode_ctx->data_mode_channel_id == channel_id) {
                    break;
                }
            }
            xSemaphoreGive(g_apb_proxy_mutex);
        }
    }
    return data_mode_ctx;
}
#if 0
/*********************************************************************************
 * @brief      save the data mode callback function pointer.
 * @param[in]  callback : data mode event call back.
 *             cmd_id : which command will try to go to data mode.
 *             channel_id: which channel will try to go to data mode.
 * @return     APB_PROXY_STATUS_ERROR  : more than one commands try to go to data mode.
 *             According to the design, only one channel can go to data mode.
 *********************************************************************************/
apb_proxy_status_t apb_proxy_set_data_mode_context(apb_proxy_data_mode_event_callback_t callback,
                                                   uint32_t cmd_id, uint32_t channel_id)
{
    apb_proxy_status_t result = APB_PROXY_STATUS_ERROR;
    uint32_t index = 0;
    apb_proxy_data_mode_t* data_mode_ctx = NULL;
    configASSERT(callback != NULL);
    if (NULL != g_apb_proxy_mutex) {
        if (pdTRUE == xSemaphoreTake(g_apb_proxy_mutex, portMAX_DELAY)) {
            for(index = 0; index < APB_PROXY_SUPPORT_DATA_MODE_MAX; index ++) {
                data_mode_ctx = g_apb_proxy_context.apb_data_mode_context + index;
                if (data_mode_ctx->data_mode_state == APB_PROXY_DATA_MODE_DEACTIVATED) {
                    data_mode_ctx->data_mode_cmd_id = cmd_id;
                    data_mode_ctx->data_mode_channel_id = channel_id;
                    data_mode_ctx->data_mode_callback = callback;
                    result = APB_PROXY_STATUS_OK;
                    break;
                }
            }
            xSemaphoreGive(g_apb_proxy_mutex);
        }
    }
    return result;
}
#endif
/*********************************************************************************
 * @brief      clear the data mode related data.
 * @param[in]  NONE.
 * @return     APB_PROXY_STATUS_ERROR  : error happens.
 *********************************************************************************/
apb_proxy_status_t apb_proxy_clear_data_mode_context(uint32_t channel_id)
{
    apb_proxy_status_t result = APB_PROXY_STATUS_ERROR;
    uint32_t index = 0;
    apb_proxy_data_mode_t* data_mode_ctx = NULL;
    if (NULL != g_apb_proxy_mutex) {
        if (pdTRUE == xSemaphoreTake(g_apb_proxy_mutex, portMAX_DELAY)) {
            for(index = 0; index < APB_PROXY_SUPPORT_DATA_MODE_MAX; index ++) {
                data_mode_ctx = g_apb_proxy_context.apb_data_mode_context + index;
                if (data_mode_ctx->data_mode_channel_id == channel_id) {
                    data_mode_ctx->data_mode_state = APB_PROXY_DATA_MODE_DEACTIVATED;
                    data_mode_ctx->data_mode_callback = NULL;
                    data_mode_ctx->data_mode_cmd_id = APB_INVALID_CMD_ID;
                    data_mode_ctx->data_mode_channel_id = APB_INVALID_CHANNEL_ID;
                    data_mode_ctx->request_close_data_mode = false;
                    result = APB_PROXY_STATUS_OK;
                    break;
                }
            }
            xSemaphoreGive(g_apb_proxy_mutex);
        }
    }
    return result;
}
/*********************************************************************************
 * @brief      set the flag that data mode is closing in progress.
 * @param[in]  NONE.
 * @return     APB_PROXY_STATUS_ERROR  : error happens.
 *********************************************************************************/
apb_proxy_status_t apb_proxy_request_close_data_mode(uint32_t channel_id)
{
    apb_proxy_status_t result = APB_PROXY_STATUS_ERROR;
    uint32_t index = 0;
    apb_proxy_data_mode_t* data_mode_ctx = NULL;
    if (NULL != g_apb_proxy_mutex) {
        if (pdTRUE == xSemaphoreTake(g_apb_proxy_mutex, portMAX_DELAY)) {
            for(index = 0; index < APB_PROXY_SUPPORT_DATA_MODE_MAX; index ++) {
                data_mode_ctx = g_apb_proxy_context.apb_data_mode_context + index;
                if ((APB_PROXY_DATA_MODE_DEACTIVATED != data_mode_ctx->data_mode_state)
                    && (channel_id == data_mode_ctx->data_mode_channel_id)) {
                    data_mode_ctx->request_close_data_mode = true;
                    result = APB_PROXY_STATUS_OK;
                    break;
                }
            }
            xSemaphoreGive(g_apb_proxy_mutex);
        }
    }
    return result;
}

/*********************************************************************************
 * @brief      Get the data mode state for specific channel.
 * @param[in]  channel_id - channel id in mdoem.
 * @return     true  : the data mode is actived.
 *********************************************************************************/
apb_proxy_data_mode_state_t apb_proxy_get_data_mode_state(uint32_t channel_id)
{
    apb_proxy_data_mode_state_t result = APB_PROXY_DATA_MODE_DEACTIVATED;
    uint32_t index = 0;
    apb_proxy_data_mode_t* data_mode_ctx = NULL;
    if (NULL != g_apb_proxy_mutex) {
        if (pdTRUE == xSemaphoreTake(g_apb_proxy_mutex, portMAX_DELAY)) {
            for(index = 0; index < APB_PROXY_SUPPORT_DATA_MODE_MAX; index ++) {
                data_mode_ctx = g_apb_proxy_context.apb_data_mode_context + index;
                if (channel_id == data_mode_ctx->data_mode_channel_id){
                    result = data_mode_ctx->data_mode_state;
                    break;
                }
            }
            xSemaphoreGive(g_apb_proxy_mutex);
        }
    }
    return result;
}

/*********************************************************************************
 * @brief      Set the data mode state for specific channel.
 * @param[in]  channel_id - channel id in mdoem.
 * @return     true  : the data mode is actived.
 *********************************************************************************/
void apb_proxy_set_data_mode_state(uint32_t channel_id, apb_proxy_data_mode_state_t state)
{
    uint32_t index = 0;
    apb_proxy_data_mode_t* data_mode_ctx = NULL;
    if (NULL != g_apb_proxy_mutex) {
        if (pdTRUE == xSemaphoreTake(g_apb_proxy_mutex, portMAX_DELAY)) {
            for(index = 0; index < APB_PROXY_SUPPORT_DATA_MODE_MAX; index ++) {
                data_mode_ctx = g_apb_proxy_context.apb_data_mode_context + index;
                if (channel_id == data_mode_ctx->data_mode_channel_id){
                    data_mode_ctx->data_mode_state = state;
                    break;
                }
            }
            xSemaphoreGive(g_apb_proxy_mutex);
        }
    }
    return;
}

/*********************************************************************************
 * @brief      check whether closing data mode is in progress.
 * @param[in]  None.
 * @return     true  : the data mode is closing in progress.
 *********************************************************************************/
bool apb_proxy_is_processing_close_data_mode(uint32_t channel_id)
{
    bool result = false;
    uint32_t index = 0;
    apb_proxy_data_mode_t* data_mode_ctx = NULL;
    if (NULL != g_apb_proxy_mutex) {
        if (pdTRUE == xSemaphoreTake(g_apb_proxy_mutex, portMAX_DELAY)) {
            for(index = 0; index < APB_PROXY_SUPPORT_DATA_MODE_MAX; index ++) {
                data_mode_ctx = g_apb_proxy_context.apb_data_mode_context + index;
                if ((data_mode_ctx->data_mode_state != APB_PROXY_DATA_MODE_DEACTIVATED)
                    && (data_mode_ctx->data_mode_channel_id == channel_id)) {
                    result = data_mode_ctx->request_close_data_mode;
                }
            }
            xSemaphoreGive(g_apb_proxy_mutex);
        }
    }
    return result;
}
/*********************************************************************************
 * @brief      create data mode context.
 * @param[in]  cmd_id : indicates which command goes to data mode.
 * @param[in]  channel_id : indicates which channel goes to data mode.
 * @param[in]  code : data mode spefic response code.
 * @param[in]  len : data mode spefic response length.
 * @param[in]  p_buf : data mode spefic response.

 * @return     true   : the data mode is successfully set as active
 *             false  : the data mode has been active.
 *                      Only one channel can go to data mode.
 *********************************************************************************/
bool apb_proxy_create_data_mode_context(uint32_t cmd_id, uint32_t channel_id,
                                        apb_proxy_at_cmd_result_code_t code,
                                        uint32_t len, uint8_t* p_buf)
{
    bool result = false;
    uint32_t index = 0;
    apb_proxy_data_mode_t* data_mode_ctx = NULL;
    bool channel_in_data_mode = false;
    if (NULL != g_apb_proxy_mutex) {
        if (pdTRUE == xSemaphoreTake(g_apb_proxy_mutex, portMAX_DELAY)) {
            for(index = 0; index < APB_PROXY_SUPPORT_DATA_MODE_MAX; index ++) {
                data_mode_ctx = g_apb_proxy_context.apb_data_mode_context + index;
                if ((data_mode_ctx->data_mode_state != APB_PROXY_DATA_MODE_DEACTIVATED)
                    && (data_mode_ctx->data_mode_channel_id == channel_id)) {
                    channel_in_data_mode = true;
                }
            }
            if (true == channel_in_data_mode) {
                result = false;
            } else {
                for(index = 0; index < APB_PROXY_SUPPORT_DATA_MODE_MAX; index ++) {
                    data_mode_ctx = g_apb_proxy_context.apb_data_mode_context + index;
                    if (data_mode_ctx->data_mode_state == APB_PROXY_DATA_MODE_DEACTIVATED) {
                        data_mode_ctx->data_mode_state = APB_PROXY_DATA_MODE_ACTIVATING;
                        data_mode_ctx->data_mode_cmd_id = cmd_id;
                        data_mode_ctx->data_mode_channel_id = channel_id;
                        data_mode_ctx->data_mode_res_code = code;
                        data_mode_ctx->data_mode_custom_result_len = len;
                        if (len > sizeof(data_mode_ctx->data_mode_custom_resp)) {
                            memcpy(data_mode_ctx->data_mode_custom_resp, p_buf, sizeof(data_mode_ctx->data_mode_custom_resp));
                        } else {
                            memcpy(data_mode_ctx->data_mode_custom_resp, p_buf, len);
                        }
                        result = true;
                        break;
                    }
                }
            }
            xSemaphoreGive(g_apb_proxy_mutex);
        }
    }
    return result;
}
/*************************************************************
 * @brief     AP Bridge Proxy context initialization.
 * @param[in] None
 * @return    None
 *************************************************************/
void apb_proxy_context_manager_init(void)
{
    g_apb_proxy_context.channel_id = APB_INVALID_CHANNEL_ID;
    g_apb_proxy_context.apb_proxy_channel_enabled = false;
    g_apb_proxy_context.apb_sleep_handle = 0xFF;
    g_apb_proxy_context.be_sleep_locked = false;
    g_apb_proxy_register_context.base_item_index = 0;
    g_apb_proxy_register_context.is_all_cmd_registered = false;
    g_apb_proxy_register_context.item_count = 0;
    g_apb_proxy_register_context.registered_count = 0;
    g_apb_proxy_mutex = xSemaphoreCreateMutex();
    memset(g_apb_proxy_context.apb_data_mode_context, 0, sizeof(g_apb_proxy_context.apb_data_mode_context));
    apb_proxy_at_cmd_context_manager_init();
}

/*************************************************************
 * @brief     Get AP Bridge proxy context pointer.
 *            The pointer value is always none-NULL.
 * @param[in] None
 * @return    The memory points to AP Bridge proxy context.
 *************************************************************/
apb_proxy_context_t *apb_proxy_get_apb_proxy_context_pointer(void)
{
    return &g_apb_proxy_context;
}

/*************************************************************
 * @brief     Get AP Bridge proxy register context pointer.
 *            The pointer value is always none-NULL.
 * @param[in] None
 * @return    The memory points to AP Bridge proxy register context.
 *************************************************************/
apb_proxy_at_cmd_reg_cxt_t *apb_proxy_get_cmd_reg_cxt_pointer(void)
{
    return &g_apb_proxy_register_context;
}

/*********************************************************************************
 * @brief      save the context for specific AT command.
 * @param[in]  p_cmd_context : the cmd's context.
 * @return     APB_PROXY_STATUS_OK  : successfully saved command context.
 *********************************************************************************/
apb_proxy_status_t apb_proxy_save_at_cmd_context(apb_proxy_at_cmd_context_t *p_cmd_context)
{
    apb_proxy_at_cmd_context_t *apb_proxy_at_cmd_context_p = NULL;
    uint32_t index = 0;
    configASSERT(p_cmd_context != NULL);
    configASSERT(g_apb_proxy_at_cmd_context_manager.p_at_cmd_context_tbl != NULL);

    if (g_apb_proxy_at_cmd_context_manager.registered_num == g_apb_proxy_at_cmd_context_manager.capacity) {
        uint32_t new_buffer_size = (g_apb_proxy_at_cmd_context_manager.capacity + APB_PROXY_AT_CMD_CONTEXT_MEMORY_INC) * sizeof(apb_proxy_at_cmd_context_t);
        apb_proxy_at_cmd_context_p = (apb_proxy_at_cmd_context_t *)pvPortMalloc(new_buffer_size);
        if (NULL == apb_proxy_at_cmd_context_p) {
            return APB_PROXY_STATUS_ERROR;
        }
        memset(apb_proxy_at_cmd_context_p, 0, new_buffer_size);
        memcpy(apb_proxy_at_cmd_context_p, g_apb_proxy_at_cmd_context_manager.p_at_cmd_context_tbl,
               g_apb_proxy_at_cmd_context_manager.capacity * sizeof(apb_proxy_at_cmd_context_t));
        g_apb_proxy_at_cmd_context_manager.capacity = g_apb_proxy_at_cmd_context_manager.capacity + APB_PROXY_AT_CMD_CONTEXT_MEMORY_INC;
        vPortFree(g_apb_proxy_at_cmd_context_manager.p_at_cmd_context_tbl);
        g_apb_proxy_at_cmd_context_manager.p_at_cmd_context_tbl = apb_proxy_at_cmd_context_p;
    }

    apb_proxy_at_cmd_context_p = g_apb_proxy_at_cmd_context_manager.p_at_cmd_context_tbl;
    for (index = 0; index < g_apb_proxy_at_cmd_context_manager.capacity; index++) {
        apb_proxy_at_cmd_context_p = g_apb_proxy_at_cmd_context_manager.p_at_cmd_context_tbl + index;
        if (false == apb_proxy_at_cmd_context_p->used) {
            break;
        }
    }
    g_apb_proxy_at_cmd_context_manager.registered_num++;
    apb_proxy_at_cmd_context_p->used = true;
    apb_proxy_at_cmd_context_p->cmd_id = p_cmd_context->cmd_id;
    apb_proxy_at_cmd_context_p->channel_id = p_cmd_context->channel_id;
    apb_proxy_at_cmd_context_p->callback = p_cmd_context->callback;
    return APB_PROXY_STATUS_OK;
}

/*********************************************************************************
 * @brief      Delete the context for specific AT command.
 * @param[in]  cmd_id     : the command identifier.
 * @param[in]  channel_id : channel ID.
 * @return     None.
 *********************************************************************************/
void apb_proxy_delete_at_cmd_context(uint32_t cmd_id, uint32_t channel_id)
{
    apb_proxy_at_cmd_context_t *apb_proxy_at_cmd_context_p = NULL;
    uint32_t index = 0;
    bool found = false;
    configASSERT(g_apb_proxy_at_cmd_context_manager.p_at_cmd_context_tbl != NULL);

    for (index = 0; index < g_apb_proxy_at_cmd_context_manager.capacity; index++) {
        apb_proxy_at_cmd_context_p = g_apb_proxy_at_cmd_context_manager.p_at_cmd_context_tbl + index;
        if ((apb_proxy_at_cmd_context_p->cmd_id == cmd_id)
            && (apb_proxy_at_cmd_context_p->channel_id == channel_id)
            && (apb_proxy_at_cmd_context_p->used == true)) {
            /*found the command context.*/
            found = true;
            apb_proxy_at_cmd_context_p->used = false;
            apb_proxy_at_cmd_context_p->cmd_id = APB_INVALID_CMD_ID;
            apb_proxy_at_cmd_context_p->channel_id = APB_INVALID_CHANNEL_ID;
            apb_proxy_at_cmd_context_p->callback = NULL;
            apb_proxy_log_info("Del cmd:(cmd:%ld , ch:%ld)ctx.", cmd_id, channel_id);
            break;
        }
    }

    if (false == found) {
        return;
    }
    g_apb_proxy_at_cmd_context_manager.registered_num--;
}
/*********************************************************************************
 * @brief      Get the command counts which are processing in AP Bridge.
 * @param[in]  None.
 * @return     current acitve command count.
 *********************************************************************************/
uint32_t apb_proxy_get_active_cmd_count(void)
{
    return g_apb_proxy_at_cmd_context_manager.registered_num;
}
/*********************************************************************************
 * @brief          Get the command's context for specific AT command.
 * @param[in]      command_id : AT command's id in modem.
 * @param[in]      channel_id : The current active channel ID.
 * @param[in/out]  pout_context : command's context for this AT command.
 * @return         APB_PROXY_STATUS_OK : successfully find the command context.
 **********************************************************************************/
apb_proxy_status_t apb_proxy_get_at_cmd_context(apb_proxy_at_cmd_context_t *pout_context,
                                                uint32_t cmd_id, uint32_t channel_id)
{
    apb_proxy_at_cmd_context_t *apb_proxy_at_cmd_context_p = NULL;
    uint32_t index = 0;
    apb_proxy_status_t result = APB_PROXY_STATUS_ERROR;
    configASSERT(g_apb_proxy_at_cmd_context_manager.p_at_cmd_context_tbl != NULL);
    configASSERT(pout_context != NULL);
    for (index = 0; index < g_apb_proxy_at_cmd_context_manager.capacity; index++) {
        apb_proxy_at_cmd_context_p = g_apb_proxy_at_cmd_context_manager.p_at_cmd_context_tbl + index;
        if ((apb_proxy_at_cmd_context_p->used == true) &&
            (apb_proxy_at_cmd_context_p->cmd_id == cmd_id) &&
            (apb_proxy_at_cmd_context_p->channel_id == channel_id)) {
            /*found the command context.*/
            result = APB_PROXY_STATUS_OK;
            memcpy(pout_context, apb_proxy_at_cmd_context_p, sizeof(apb_proxy_at_cmd_context_t));
            break;
        }
    }
    return result;
}

/*********************************************************************************
 * @brief          Get the command's handler item info for specific AT command.
 * @param[in]      command_id : AT command's id generated from modem.
 * @return         AT Command's handler info, if faile, NULL will be returned.
 **********************************************************************************/
apb_proxy_cmd_hdlr_item_t *apb_proxy_get_at_hdlr_item(uint32_t cmd_id)
{
    apb_proxy_cmd_hdlr_table_t *p_cmd_tbl = apb_proxy_get_cmd_hdlr_table_info();
    apb_proxy_cmd_hdlr_item_t *p_cmd_hdlr_item = NULL;
    uint32_t index = 0;
    bool found = false;

    for (index = 0; index < p_cmd_tbl->item_size; index++) {
        p_cmd_hdlr_item = p_cmd_tbl->p_item_tbl + index;
        if (p_cmd_hdlr_item->cmd_id == cmd_id) {
            found = true;
            break;
        }
    }

    if (true == found) {
        return p_cmd_hdlr_item;
    } else {
        return NULL;
    }
}

