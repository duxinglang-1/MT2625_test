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

#ifndef __APB_PROXY_CONTEXT_MANAGER__H__
#define __APB_PROXY_CONTEXT_MANAGER__H__

#include "apb_proxy.h"
#include "apb_proxy_at_cmd_tbl.h"
#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef APB_PROXY_SUPPORT_DATA_MODE_MAX
#define APB_PROXY_SUPPORT_DATA_MODE_MAX 3
#endif

typedef uint8_t apb_proxy_sleep_handle_t;

typedef enum {
    APB_PROXY_DATA_MODE_DEACTIVATED      = 0,
    APB_PROXY_DATA_MODE_ACTIVATING       = 1,
    APB_PROXY_DATA_MODE_ACTIVATED        = 2,
    APB_PROXY_DATA_MODE_TEMP_DEACTIVATED = 3
} apb_proxy_data_mode_state_t;

typedef struct {
    apb_proxy_data_mode_state_t data_mode_state;
    bool         request_close_data_mode;
    uint32_t     data_mode_cmd_id;
    uint32_t     data_mode_channel_id;
    apb_proxy_data_mode_event_callback_t data_mode_callback;
    bool         apb_proxy_request_stop_data;
    bool         modem_request_stop_data;
    bool         app_request_stop_data;
    apb_proxy_at_cmd_result_code_t data_mode_res_code;
    uint32_t     data_mode_custom_result_len;
    uint8_t      data_mode_custom_resp[40];
    void*        data_rx_queue_from_md;
    void*        data_tx_queue_from_ap;
} apb_proxy_data_mode_t;

typedef struct {
    uint32_t     channel_id;
    bool         apb_proxy_channel_enabled;
    apb_proxy_sleep_handle_t apb_sleep_handle;
    bool         be_sleep_locked;
    apb_proxy_data_mode_t apb_data_mode_context[APB_PROXY_SUPPORT_DATA_MODE_MAX];
} apb_proxy_context_t;

typedef struct {
    bool                     used;
    uint32_t                 cmd_id;     /* command id in modem*/
    uint32_t                 channel_id; /* channel id in modem.*/
    apb_proxy_at_cmd_handler callback;
} apb_proxy_at_cmd_context_t;

typedef struct {
    uint16_t base_item_index; /* current registering at cmd base index in AT cmd table.*/
    uint16_t item_count;/* current reistering at cmd count.*/
    uint32_t registered_count;/* successfully registerred AT command's count.*/
    bool     is_all_cmd_registered;
} apb_proxy_at_cmd_reg_cxt_t;

/*********************************************************************************
 * @brief      Get the data mode context for specific channel ID.
 * @param[in]  channel_id: the specific channel's contexts.
 * @return     The pointer points to the data mode contexts.
 *********************************************************************************/
apb_proxy_data_mode_t* apb_proxy_get_data_mode_context(uint32_t channel_id);
/*********************************************************************************
 * @brief      save the data mode callback function pointer.
 * @param[in]  callback : data mode event call back.
 *             cmd_id : which command will try to go to data mode.
 *             channel_id: which channel will try to go to data mode.
 * @return     APB_PROXY_STATUS_ERROR  : more than one commands try to go to data mode.
 *             According to the design, only one channel can go to data mode.
 *********************************************************************************/
apb_proxy_status_t apb_proxy_set_data_mode_context(apb_proxy_data_mode_event_callback_t callback,
                                                   uint32_t cmd_id, uint32_t channel_id);
/*********************************************************************************
 * @brief      clear the data mode related data.
 * @param[in]  None.
 * @return     APB_PROXY_STATUS_ERROR  : error happens.
 *********************************************************************************/
apb_proxy_status_t apb_proxy_clear_data_mode_context(uint32_t channel_id);
/*********************************************************************************
 * @brief      Get the data mode state for specific channel.
 * @param[in]  channel_id - channel id in mdoem.
 * @return     true  : the data mode is actived.
 *********************************************************************************/
apb_proxy_data_mode_state_t apb_proxy_get_data_mode_state(uint32_t channel_id);
/*********************************************************************************
 * @brief      check whether closing data mode is in progress.
 * @param[in]  None.
 * @return     true  : the data mode is closing in progress.
 *********************************************************************************/
bool apb_proxy_is_processing_close_data_mode(uint32_t channel_id);
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
                                        uint32_t len, uint8_t* p_buf);
/*********************************************************************************
 * @brief      set the flag that data mode is closing in progress.
 * @param[in]  NONE.
 * @return     APB_PROXY_STATUS_ERROR  : error happens.
 *********************************************************************************/
apb_proxy_status_t apb_proxy_request_close_data_mode(uint32_t channel_id);
/*************************************************************
 * @brief     AP Bridge Proxy context initialization.
 * @param[in] None
 * @return    None
 *************************************************************/
void apb_proxy_context_manager_init(void);

/*************************************************************
 * @brief     Get AP Bridge proxy context pointer.
 *            The pointer value is always none-NULL.
 * @param[in] None
 * @return    The memory points to AP Bridge proxy context.
 *************************************************************/
apb_proxy_context_t *apb_proxy_get_apb_proxy_context_pointer(void);

/*************************************************************
 * @brief     Get AP Bridge proxy register context pointer.
 *            The pointer value is always none-NULL.
 * @param[in] None
 * @return    The memory points to AP Bridge proxy register context.
 *************************************************************/
apb_proxy_at_cmd_reg_cxt_t *apb_proxy_get_cmd_reg_cxt_pointer(void);

/*********************************************************************************
 * @brief      save the context for specific AT command.
 * @param[in]  p_cmd_context : the cmd's context.
 * @return     APB_PROXY_STATUS_OK  : successfully saved command context.
 *********************************************************************************/
apb_proxy_status_t apb_proxy_save_at_cmd_context(apb_proxy_at_cmd_context_t *p_cmd_context);
/*********************************************************************************
 * @brief      Delete the context for specific AT command.
 * @param[in]  cmd_id     : the command identifier.
 * @param[in]  channel_id : channel ID.
 * @return     None.
 *********************************************************************************/
void apb_proxy_delete_at_cmd_context(uint32_t cmd_id, uint32_t channel_id);
/*********************************************************************************
 * @brief      Get the command counts which are processing in AP Bridge.
 * @param[in]  None.
 * @return     current acitve command count.
 *********************************************************************************/
uint32_t apb_proxy_get_active_cmd_count(void);
/*********************************************************************************
 * @brief          Get the command's context for specific AT command.
 * @param[in]      command_id : AT command's id in modem.
 * @param[in]      channel_id : The current active channel ID.
 * @param[in/out]  pout_context : command's context for this AT command.
 * @return         APB_PROXY_STATUS_OK : successfully find the command context.
 **********************************************************************************/
apb_proxy_status_t apb_proxy_get_at_cmd_context(apb_proxy_at_cmd_context_t *pout_context,
                                                uint32_t cmd_id, uint32_t channel_id);

/*********************************************************************************
 * @brief          Get the command's handler item info for specific AT command.
 * @param[in]      command_id : AT command's id generated from modem.
 * @return         AT Command's handler info, if faile, NULL will be returned.
 **********************************************************************************/
apb_proxy_cmd_hdlr_item_t *apb_proxy_get_at_hdlr_item(uint32_t cmd_id);
/*********************************************************************************
 * @brief      Set the data mode state for specific channel.
 * @param[in]  channel_id - channel id in mdoem.
 * @return     true  : the data mode is actived.
 *********************************************************************************/
void apb_proxy_set_data_mode_state(uint32_t channel_id, apb_proxy_data_mode_state_t state);
#ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
/*********************************************************************************
 * @brief          Store the synced data.
 * @param[in]      None.
 * @return         None.
 **********************************************************************************/
void apb_proxy_store_cmd_data(void);
#endif

#ifdef __cplusplus
}
#endif

#endif/*__APB_PROXY_CONTEXT_MANAGER__H__*/

