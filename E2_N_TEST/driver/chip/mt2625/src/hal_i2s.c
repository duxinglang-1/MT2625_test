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


#include "hal_i2s.h"
#ifdef HAL_I2S_MODULE_ENABLED

#include "hal_log.h"
#include "hal_clock.h"
#include "hal_gpio.h"
#include "hal_gpt.h"
#include "hal_pdma_internal.h"
#include "hal_i2s_internal.h"

#include "memory_attribute.h"
#ifdef HAL_SLEEP_MANAGER_ENABLED
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_internal.h"
#endif

static hal_clock_cg_id i2s_clock_cg[HAL_I2S_MAX] = {HAL_CLOCK_CG_I2S0, HAL_CLOCK_CG_I2S1};
static vdma_channel_t i2s_vdma_tx_ch[HAL_I2S_MAX] = {VDMA_I2S0TX, VDMA_I2S1TX};
static vdma_channel_t i2s_vdma_rx_ch[HAL_I2S_MAX] = {VDMA_I2S0RX, VDMA_I2S1RX};
static i2s_internal_t i2s_internal_cfg[HAL_I2S_MAX];

#ifdef HAL_SLEEP_MANAGER_ENABLED
static uint32_t i2s_global_cfg[HAL_I2S_MAX] = {0};
static uint32_t i2s_dl_cfg[HAL_I2S_MAX] = {0};
static uint32_t i2s_ul_cfg[HAL_I2S_MAX] = {0};
static uint8_t i2s_ul_fs_cfg[HAL_I2S_MAX] = {0};
static uint8_t i2s_dl_fs_cfg[HAL_I2S_MAX] = {0};
static bool is_register_sleep_callback[HAL_I2S_MAX] = {false};
static bool is_lock_sleep[HAL_I2S_MAX] = {false};
#endif



#ifdef HAL_SLEEP_MANAGER_ENABLED

static void i2s_suspend(hal_i2s_port_t i2s_port)
{
    if (i2s_internal_cfg[i2s_port].i2s_state == I2S_STATE_IDLE) {
        return;
    }
}

static void i2s_resume(hal_i2s_port_t i2s_port)
{
    if (i2s_internal_cfg[i2s_port].i2s_state == I2S_STATE_IDLE) {
        return;
    }

    if (i2s_port == HAL_I2S_0) {
        //I2S
        DRV_Reg32(I2S_0_I2S1_GLOBAL_CONTROL) = (uint32_t)i2s_global_cfg[HAL_I2S_0];
        DRV_Reg32(I2S_0_I2S1_DL_CONTROL) = (uint32_t)i2s_dl_cfg[HAL_I2S_0];
        DRV_Reg32(I2S_0_I2S1_UL_CONTROL) = (uint32_t)i2s_ul_cfg[HAL_I2S_0];
        DRV_Reg8(I2S_0_I2S1_DL_SR_EN_CONTROL__F_CR_I2S1_OUT_SR) = (uint8_t)i2s_dl_fs_cfg[HAL_I2S_0];
        DRV_Reg8(I2S_0_I2S1_UL_SR_EN_CONTROL__F_CR_I2S1_IN_SR) = (uint8_t)i2s_ul_fs_cfg[HAL_I2S_0];

    } else {
        //I2S
        DRV_Reg32(I2S_1_I2S1_GLOBAL_CONTROL) = (uint32_t)i2s_global_cfg[HAL_I2S_0];
        DRV_Reg32(I2S_1_I2S1_DL_CONTROL) = (uint32_t)i2s_dl_cfg[HAL_I2S_0];
        DRV_Reg32(I2S_1_I2S1_UL_CONTROL) = (uint32_t)i2s_ul_cfg[HAL_I2S_0];
        DRV_Reg8(I2S_1_I2S1_DL_SR_EN_CONTROL__F_CR_I2S1_OUT_SR) = (uint8_t)i2s_dl_fs_cfg[HAL_I2S_0];
        DRV_Reg8(I2S_1_I2S1_UL_SR_EN_CONTROL__F_CR_I2S1_IN_SR) = (uint8_t)i2s_ul_fs_cfg[HAL_I2S_0];
    }

    //VDMA
    if (i2s_internal_cfg[i2s_port].i2s_vfifo.tx_dma_configured) {
        hal_i2s_dma_config_t dma_config;
        dma_config.buffer = i2s_internal_cfg[i2s_port].i2s_vfifo.tx_vfifo_base;
        dma_config.threshold = i2s_internal_cfg[i2s_port].i2s_vfifo.tx_vfifo_threshold;
        dma_config.buffer_length = i2s_internal_cfg[i2s_port].i2s_vfifo.tx_vfifo_length;
        hal_i2s_set_tx_dma_ex(i2s_port, &dma_config);
    }
    if (i2s_internal_cfg[i2s_port].i2s_vfifo.rx_dma_configured) {
        hal_i2s_dma_config_t dma_config;
        dma_config.buffer = i2s_internal_cfg[i2s_port].i2s_vfifo.rx_vfifo_base;
        dma_config.threshold = i2s_internal_cfg[i2s_port].i2s_vfifo.rx_vfifo_threshold;
        dma_config.buffer_length = i2s_internal_cfg[i2s_port].i2s_vfifo.rx_vfifo_length;        
        hal_i2s_set_rx_dma_ex(i2s_port, &dma_config);
    }
}

static void i2s0_enter_suspend(void *data,uint32_t mode)
{
    i2s_suspend(HAL_I2S_0);
}

static void i2s1_enter_suspend(void *data,uint32_t mode)
{
    i2s_suspend(HAL_I2S_1);
}

static void i2s0_enter_resume(void *data,uint32_t mode)
{
    i2s_resume(HAL_I2S_0);
}

static void i2s1_enter_resume(void *data,uint32_t mode)
{
    i2s_resume(HAL_I2S_1);
}
#endif

/*Get data count in tx vfifo*/
static uint32_t i2s_get_tx_dma_data_count(hal_i2s_port_t i2s_port)
{
    if (i2s_port == HAL_I2S_0) {
        return DRV_Reg32(VDMA22_FFCNT);
    } else {
        return DRV_Reg32(VDMA24_FFCNT);
    }
}

static void i2s_flush_tx_dma(hal_i2s_port_t i2s_port)
{
    uint32_t previous_count = 0, current_count = 0;

    while ((I2S_CHECK_BIT(i2s_internal_cfg[i2s_port].i2s_state, I2S_STATE_TX_RUNNING) != 0) &&
            (i2s_internal_cfg[i2s_port].i2s_vfifo.tx_dma_configured == true) &&
            (i2s_internal_cfg[i2s_port].i2s_audiotop_enabled == true)) {

            if (i2s_port == HAL_I2S_0) {
                current_count = DRV_Reg32(VDMA22_FFCNT);
            } else {
                current_count = DRV_Reg32(VDMA24_FFCNT);
            }
            hal_gpt_delay_ms(1);
            if (current_count == 0 || (current_count == previous_count)) {
                break;
            }
            previous_count = current_count;            
    }
}

static void i2s_stop_rx_dma(hal_i2s_port_t i2s_port, bool disable_flag)
{
    if (i2s_internal_cfg[i2s_port].i2s_vfifo.rx_dma_configured != true) {
        return ;
    }

    vdma_status_t status;
    status = vdma_stop(i2s_vdma_rx_ch[i2s_port]);
    if (status != VDMA_OK) {
        log_hal_error("[I2S%d]STOP RX DMA ERROR, vdma_stop failed\r\n", i2s_port);
        return ;
    }
    status = vdma_deinit(i2s_vdma_rx_ch[i2s_port]);
    if (status != VDMA_OK) {
        log_hal_error("[I2S%d]STOP RX DMA ERROR, deinit vdma rx channel failed\r\n", i2s_port);
        return ;
    }
    if (disable_flag) {
        i2s_internal_cfg[i2s_port].i2s_vfifo.rx_dma_configured = false;
    }
}

static void i2s_stop_tx_dma(hal_i2s_port_t i2s_port, bool disable_flag)
{
    if (i2s_internal_cfg[i2s_port].i2s_vfifo.tx_dma_configured != true) {
        return ;
    }
    vdma_status_t status;
    status = vdma_stop(i2s_vdma_tx_ch[i2s_port]);
    if (status != VDMA_OK) {
        log_hal_error("[I2S%d]STOP TX DMA ERROR, vdma_stop fail\r\n", i2s_port);
        return ;
    }

    status = vdma_deinit(i2s_vdma_tx_ch[i2s_port]);
    if (status != VDMA_OK) {
        log_hal_error("[I2S%d]STOP TX DMA ERROR, deinit vdma tx channel failed\r\n", i2s_port);
        return ;
    }
    if (disable_flag) {
        i2s_internal_cfg[i2s_port].i2s_vfifo.tx_dma_configured = false;
    }
}

static void i2s_rx_dma_callback_handler(vdma_event_t event, void  *user_data)
{
    i2s_internal_t *i2s_internal = (i2s_internal_t *)user_data;
    i2s_internal->user_rx_callback_func(HAL_I2S_EVENT_DATA_NOTIFICATION, i2s_internal->user_rx_data);
}

static void i2s_tx_dma_callback_handler(vdma_event_t event, void  *user_data)
{
    i2s_internal_t *i2s_internal = (i2s_internal_t *)user_data;
    hal_i2s_port_t i2s_port = i2s_internal->i2s_port;

    if (i2s_internal_cfg[i2s_port].i2s_tx_eof) {
        if (i2s_get_tx_dma_data_count(i2s_port) == 0) {
            i2s_internal->user_tx_callback_func(HAL_I2S_EVENT_END, i2s_internal->user_tx_data);
            i2s_internal_cfg[i2s_port].i2s_tx_eof = false;
            vdma_set_threshold(i2s_vdma_tx_ch[i2s_port], i2s_internal_cfg[i2s_port].i2s_vfifo.rx_vfifo_threshold);
            hal_i2s_disable_tx_interrupt_ex(i2s_port);
            return;
        }
    }
    i2s_internal->user_tx_callback_func(HAL_I2S_EVENT_DATA_REQUEST, i2s_internal->user_tx_data);
}

hal_i2s_status_t hal_i2s_init_ex(hal_i2s_port_t i2s_port, hal_i2s_initial_type_t i2s_initial_type)
{
    log_hal_info("[I2S%d]INIT\r\n", i2s_port);

    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]INIT ERROR, invalid i2s port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (i2s_internal_cfg[i2s_port].i2s_state != I2S_STATE_IDLE) {
        log_hal_error("[I2S%d]INIT ERROR, i2s_state=%d\r\n", i2s_port, i2s_internal_cfg[i2s_port].i2s_state);
        return HAL_I2S_STATUS_ERROR;
    }

    i2s_set_initial_parameter(&i2s_internal_cfg[i2s_port]);

#ifdef HAL_SLEEP_MANAGER_ENABLED
    uint32_t register_mode;
    register_mode = HAL_SLEEP_MODE_LIGHT_SLEEP;
    if (is_register_sleep_callback[i2s_port] == false) {
        if (i2s_port == HAL_I2S_0) {
            sleep_management_register_suspend_callback(SLEEP_BACKUP_RESTORE_I2S0, (sleep_management_suspend_callback_t)i2s0_enter_suspend, NULL, register_mode);
            sleep_management_register_resume_callback(SLEEP_BACKUP_RESTORE_I2S0, (sleep_management_resume_callback_t)i2s0_enter_resume, NULL, register_mode);
        }
        if (i2s_port == HAL_I2S_1) {
            sleep_management_register_suspend_callback(SLEEP_BACKUP_RESTORE_I2S1, (sleep_management_suspend_callback_t)i2s1_enter_suspend, NULL, register_mode);
            sleep_management_register_resume_callback(SLEEP_BACKUP_RESTORE_I2S1, (sleep_management_resume_callback_t)i2s1_enter_resume, NULL, register_mode);
        }
        is_register_sleep_callback[i2s_port] = true;
    }
#endif

    switch (i2s_initial_type) {

        case HAL_I2S_TYPE_INTERNAL_LOOPBACK_MODE:
            log_hal_info("[I2S%d]INTERNAL LOOPBACK MODE\r\n", i2s_port);
            i2s_internal_cfg[i2s_port].i2s_initial_type = HAL_I2S_TYPE_INTERNAL_LOOPBACK_MODE;
            break;

        case HAL_I2S_TYPE_EXTERNAL_MODE:
            log_hal_info("[I2S%d]I2S EXTERNAL MODE\r\n", i2s_port);
            i2s_internal_cfg[i2s_port].i2s_initial_type = HAL_I2S_TYPE_EXTERNAL_MODE;
            break;

        default:
            return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    i2s_internal_cfg[i2s_port].i2s_state = I2S_STATE_INIT;
    i2s_internal_cfg[i2s_port].i2s_port = i2s_port;

    return HAL_I2S_STATUS_OK;

}

hal_i2s_status_t hal_i2s_deinit_ex(hal_i2s_port_t i2s_port)
{
    log_hal_info("[I2S%d]DEINIT\r\n", i2s_port);

    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]DEINIT ERROR, invalid i2s port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (i2s_internal_cfg[i2s_port].i2s_state != I2S_STATE_INIT) {
        log_hal_error("[I2S%d]DEINIT ERROR, i2s_state=%d\r\n", i2s_port, i2s_internal_cfg[i2s_port].i2s_state);
        return HAL_I2S_STATUS_ERROR;
    }
    i2s_internal_cfg[i2s_port].i2s_state = I2S_STATE_IDLE;
    i2s_internal_cfg[i2s_port].i2s_configured = false;
    i2s_internal_cfg[i2s_port].i2s_audiotop_enabled = false;
    //Stop DMA
    i2s_stop_rx_dma(i2s_port, true);
    i2s_stop_tx_dma(i2s_port, true);

    return HAL_I2S_STATUS_OK;

}

hal_i2s_status_t hal_i2s_get_config_ex(hal_i2s_port_t i2s_port, hal_i2s_config_t *config)
{
    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]GET CONFIG ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    *config = i2s_internal_cfg[i2s_port].i2s_user_config;
    return HAL_I2S_STATUS_OK;
}

hal_i2s_status_t hal_i2s_set_config_ex(hal_i2s_port_t i2s_port, const hal_i2s_config_t *config)
{
    log_hal_info("[I2S%d]CONFIG\r\n", i2s_port);

    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]CONFIG ERROR, invalid i2s port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (NULL == config) {
        log_hal_error("[I2S%d]CONFIG ERROR, config is null\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (i2s_internal_cfg[i2s_port].i2s_state != I2S_STATE_INIT) {
        log_hal_error("[I2S%d]CONFIG ERROR, i2s_state=%d\r\n", i2s_port, i2s_internal_cfg[i2s_port].i2s_state);
        return HAL_I2S_STATUS_ERROR;
    }

    /*Check both sample rates are same value */
    if ((config->i2s_out.sample_rate) != (config->i2s_in.sample_rate)) {
        log_hal_error("[I2S%d]CONFIG ERROR, tx sampling rate != rx sampling rate\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    //---Check whether both tx channerl number and tx mode are set to enable at same time
    if ((config->i2s_out.channel_number == HAL_I2S_STEREO) && (config->tx_mode != HAL_I2S_TX_DUPLICATE_DISABLE)) {
        log_hal_error("[I2S%d]CONFIG ERROR, tx is stereo and duplicate\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    /*Set TX channel number*/
    switch (config->i2s_out.channel_number) {
        case HAL_I2S_MONO:
            i2s_internal_cfg[i2s_port].i2s_user_config.i2s_out.channel_number = HAL_I2S_MONO;
            break;
        case HAL_I2S_STEREO:
            i2s_internal_cfg[i2s_port].i2s_user_config.i2s_out.channel_number = HAL_I2S_STEREO;
            break;
        default:
            log_hal_error("[I2S%d]CONFIG ERROR, invalid i2s_out.channel_number\r\n", i2s_port);
            return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    /*Set RX channel number*/
    switch (config->i2s_in.channel_number) {
        case HAL_I2S_MONO:
            i2s_internal_cfg[i2s_port].i2s_user_config.i2s_in.channel_number = HAL_I2S_MONO;
            break;
        case HAL_I2S_STEREO:
            i2s_internal_cfg[i2s_port].i2s_user_config.i2s_in.channel_number = HAL_I2S_STEREO;
            break;
        default:
            log_hal_error("[I2S%d]CONFIG ERROR, invalid i2s_in.channel_number\r\n", i2s_port);
            return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    /*Set clock mode*/
    switch (config->clock_mode) {
        case HAL_I2S_MASTER:
            i2s_internal_cfg[i2s_port].i2s_user_config.clock_mode = HAL_I2S_MASTER;
            break;
        case HAL_I2S_SLAVE:
            i2s_internal_cfg[i2s_port].i2s_user_config.clock_mode = HAL_I2S_SLAVE;
            break;
        default:
            log_hal_error("[I2S%d]CONFIG ERROR, invalid clock_mode\r\n", i2s_port);
            return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    /*Set tx mode*/
    switch (config->tx_mode) {
        case HAL_I2S_TX_DUPLICATE_LEFT_CHANNEL:
            /*duplicate L to R*/
            i2s_internal_cfg[i2s_port].i2s_user_config.tx_mode = HAL_I2S_TX_DUPLICATE_LEFT_CHANNEL;
            break;
        default:
            i2s_internal_cfg[i2s_port].i2s_user_config.tx_mode = HAL_I2S_TX_DUPLICATE_DISABLE;
            break;
    }

    /*Set TX lr swap*/
    switch (config->i2s_out.lr_swap) {
        case HAL_I2S_LR_SWAP_ENABLE:
            i2s_internal_cfg[i2s_port].i2s_user_config.i2s_out.lr_swap = HAL_I2S_LR_SWAP_ENABLE;
            break;
        default:
            i2s_internal_cfg[i2s_port].i2s_user_config.i2s_out.lr_swap = HAL_I2S_LR_SWAP_DISABLE;
            break;
    }

    /*Set RX lr swap*/
    switch (config->i2s_in.lr_swap) {
        case HAL_I2S_LR_SWAP_ENABLE:
            i2s_internal_cfg[i2s_port].i2s_user_config.i2s_in.lr_swap = HAL_I2S_LR_SWAP_ENABLE;
            break;
        default:
            i2s_internal_cfg[i2s_port].i2s_user_config.i2s_in.lr_swap = HAL_I2S_LR_SWAP_DISABLE;
            break;
    }

    /*Sample width*/
    switch (config->sample_width) {
        case HAL_I2S_SAMPLE_WIDTH_8BIT:
            return HAL_I2S_STATUS_INVALID_PARAMETER;
        case HAL_I2S_SAMPLE_WIDTH_16BIT:
            i2s_internal_cfg[i2s_port].i2s_user_config.sample_width = HAL_I2S_SAMPLE_WIDTH_16BIT;
            break;
        case HAL_I2S_SAMPLE_WIDTH_24BIT:
            i2s_internal_cfg[i2s_port].i2s_user_config.sample_width = HAL_I2S_SAMPLE_WIDTH_24BIT;
            break;
        default:
            i2s_internal_cfg[i2s_port].i2s_user_config.sample_width = HAL_I2S_SAMPLE_WIDTH_16BIT;
            break;
    }


    //I2S mode do not need MSB delay fucntion
    /*Set msb_offset, only work for TDM mode*/
    i2s_internal_cfg[i2s_port].i2s_user_config.i2s_in.msb_offset = 0;
    i2s_internal_cfg[i2s_port].i2s_user_config.i2s_out.msb_offset = 0;

    //Set TX word_select_inverse----
    switch (config->i2s_out.word_select_inverse) {
        case HAL_I2S_WORD_SELECT_INVERSE_ENABLE:
            i2s_internal_cfg[i2s_port].i2s_user_config.i2s_out.word_select_inverse = HAL_I2S_WORD_SELECT_INVERSE_ENABLE;
            break;
        default:
            i2s_internal_cfg[i2s_port].i2s_user_config.i2s_out.word_select_inverse = HAL_I2S_WORD_SELECT_INVERSE_DISABLE;
            break;
    }

    //Set RX word_select_inverse----
    switch (config->i2s_in.word_select_inverse) {
        case HAL_I2S_WORD_SELECT_INVERSE_ENABLE:
            i2s_internal_cfg[i2s_port].i2s_user_config.i2s_in.word_select_inverse = HAL_I2S_WORD_SELECT_INVERSE_ENABLE;
            break;
        default:
            i2s_internal_cfg[i2s_port].i2s_user_config.i2s_in.word_select_inverse = HAL_I2S_WORD_SELECT_INVERSE_DISABLE;
            break;
    }

    /*frame sync width*/
    switch (config->frame_sync_width) {
        case HAL_I2S_FRAME_SYNC_WIDTH_64:
            i2s_internal_cfg[i2s_port].i2s_user_config.frame_sync_width = HAL_I2S_FRAME_SYNC_WIDTH_64;
            break;
        default:
            i2s_internal_cfg[i2s_port].i2s_user_config.frame_sync_width = HAL_I2S_FRAME_SYNC_WIDTH_32;
            break;
    }

    if (i2s_set_clock(&i2s_internal_cfg[i2s_port], config->i2s_out.sample_rate) != HAL_I2S_STATUS_OK) {
        log_hal_error("[I2S%d]CONFIG ERROR, invalid sample_rate\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    i2s_internal_cfg[i2s_port].i2s_configured = true;

    if ((i2s_internal_cfg[HAL_I2S_0].i2s_configured == true) && (i2s_internal_cfg[HAL_I2S_1].i2s_configured == true)) {
#if !defined(HAL_I2S_FEATURE_XTAL_I2S0) && !defined(HAL_I2S_FEATURE_XTAL_I2S1)
        //Because both i2s0 and i2s1 share one internal xpll, the clock base shoud be same for both.
        if (i2s_internal_cfg[HAL_I2S_0].i2s_clock_source != i2s_internal_cfg[HAL_I2S_1].i2s_clock_source) {
            log_hal_error("[I2S%d] CONFIG ERROR,  clock source is different between i2s0 and i2s1 on xpll mode\r\n", i2s_port);
            return HAL_I2S_STATUS_ERROR;
        }
#endif

    }

    /*Configure I2S according to user`s settings*/
    i2s_set_parameter(&i2s_internal_cfg[i2s_port]);

#ifdef HAL_SLEEP_MANAGER_ENABLED
    /*Backup REG*/
    if (i2s_port == HAL_I2S_0) {
        //I2S0
        i2s_global_cfg[HAL_I2S_0] = (uint32_t)DRV_Reg32(I2S_0_I2S1_GLOBAL_CONTROL);
        i2s_dl_cfg[HAL_I2S_0] = (uint32_t)DRV_Reg32(I2S_0_I2S1_DL_CONTROL);
        i2s_ul_cfg[HAL_I2S_0] = (uint32_t)DRV_Reg32(I2S_0_I2S1_UL_CONTROL);
        i2s_dl_fs_cfg[HAL_I2S_0] = (uint8_t)DRV_Reg8(I2S_0_I2S1_DL_SR_EN_CONTROL__F_CR_I2S1_OUT_SR);
        i2s_ul_fs_cfg[HAL_I2S_0] = (uint8_t)DRV_Reg8(I2S_0_I2S1_UL_SR_EN_CONTROL__F_CR_I2S1_IN_SR);
    } else {
        //I2S1
        i2s_global_cfg[HAL_I2S_1] = (uint32_t)DRV_Reg32(I2S_1_I2S1_GLOBAL_CONTROL);
        i2s_dl_cfg[HAL_I2S_1] = (uint32_t)DRV_Reg32(I2S_1_I2S1_DL_CONTROL);
        i2s_ul_cfg[HAL_I2S_1] = (uint32_t)DRV_Reg32(I2S_1_I2S1_UL_CONTROL);
        i2s_dl_fs_cfg[HAL_I2S_1] = (uint8_t)DRV_Reg8(I2S_1_I2S1_DL_SR_EN_CONTROL__F_CR_I2S1_OUT_SR);
        i2s_ul_fs_cfg[HAL_I2S_1] = (uint8_t)DRV_Reg8(I2S_1_I2S1_UL_SR_EN_CONTROL__F_CR_I2S1_IN_SR);
    }
#endif

    return HAL_I2S_STATUS_OK;

}

hal_i2s_status_t hal_i2s_set_tx_dma_ex(hal_i2s_port_t i2s_port, const hal_i2s_dma_config_t *dma_config)
{
    if (NULL == dma_config->buffer) {
        log_hal_error("[I2S%d]SET TX DMA ERROR, buffer is NULL\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    /*address should be 4 bytes aligned*/
    if ((((uint32_t)dma_config->buffer) & 0x3) != 0) {
        log_hal_error("[I2S%d]SET TX DMA ERROR, buffer not 4 bytes aligned\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (dma_config->buffer_length == 0 || (dma_config->buffer_length & 0x1) != 0) {
        log_hal_error("[I2S%d]SET TX DMA ERROR, buffer_length == 0 || buffer_length is odd\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]SET TX DMA ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (i2s_internal_cfg[i2s_port].i2s_state != I2S_STATE_INIT) {
        log_hal_error("[I2S%d]SET TX DMA ERROR ERROR, i2s_state=%d\r\n", i2s_port, i2s_internal_cfg[i2s_port].i2s_state);
        return HAL_I2S_STATUS_ERROR;
    }

    i2s_stop_tx_dma(i2s_port, true);

    vdma_status_t i2s_vdma_status;
    vdma_config_t i2s_vdma_config;
    uint32_t i2s_threshold;

    i2s_vdma_config.base_address = (uint32_t)dma_config->buffer;
    i2s_vdma_config.size = dma_config->buffer_length;
    i2s_threshold = dma_config->threshold;

    i2s_vdma_status = vdma_init(i2s_vdma_tx_ch[i2s_port]);
    if (i2s_vdma_status != VDMA_OK) {
        log_hal_error("[I2S%d]SET TX DMA ERROR, vdma_init fail\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    i2s_vdma_status = vdma_configure(i2s_vdma_tx_ch[i2s_port], &i2s_vdma_config);
    if (i2s_vdma_status != VDMA_OK) {
        log_hal_error("[I2S%d]SET TX DMA ERROR, vdma_configure fail\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    i2s_vdma_status = vdma_set_threshold(i2s_vdma_tx_ch[i2s_port], i2s_threshold);
    if (i2s_vdma_status != VDMA_OK) {
        log_hal_error("[I2S%d]SET TX DMA ERROR, vdma_set_threshold fail\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    i2s_vdma_status = vdma_start(i2s_vdma_tx_ch[i2s_port]);
    if (i2s_vdma_status != VDMA_OK) {
        log_hal_error("[I2S%d]SET TX DMA ERROR, vdma_start fail\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    i2s_internal_cfg[i2s_port].i2s_vfifo.tx_vfifo_base = (uint32_t *)dma_config->buffer;
    i2s_internal_cfg[i2s_port].i2s_vfifo.tx_vfifo_length = dma_config->buffer_length;
    i2s_internal_cfg[i2s_port].i2s_vfifo.tx_vfifo_threshold = dma_config->threshold;
    i2s_internal_cfg[i2s_port].i2s_vfifo.tx_dma_configured = true;

    return HAL_I2S_STATUS_OK;
}

hal_i2s_status_t hal_i2s_set_rx_dma_ex(hal_i2s_port_t i2s_port, const hal_i2s_dma_config_t *dma_config)
{
    if (NULL == dma_config->buffer) {
        log_hal_error("[I2S%d]SET RX DMA ERROR, buffer is NULL\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    /*address should be 4 bytes aligned*/
    if ((((uint32_t)dma_config->buffer) & 0x3) != 0) {
        log_hal_error("[I2S%d]SET RX DMA ERROR, buffer not 4 bytes aligned\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (dma_config->buffer_length == 0 || (dma_config->buffer_length & 0x1) != 0) {
        log_hal_error("[I2S%d]SET RX DMA ERROR, buffer_length == 0 || buffer_length is odd\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]SET RX DMA ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (i2s_internal_cfg[i2s_port].i2s_state != I2S_STATE_INIT) {
        log_hal_error("[I2S%d]SET RX DMA ERROR ERROR, i2s_state=%d\r\n", i2s_port, i2s_internal_cfg[i2s_port].i2s_state);
        return HAL_I2S_STATUS_ERROR;
    }

    i2s_stop_rx_dma(i2s_port, true);

    vdma_status_t i2s_vdma_status;
    vdma_config_t i2s_vdma_config;
    uint32_t i2s_threshold;

    i2s_vdma_config.base_address = (uint32_t)dma_config->buffer;
    i2s_vdma_config.size = dma_config->buffer_length;
    i2s_threshold = dma_config->threshold;

    i2s_vdma_status = vdma_init(i2s_vdma_rx_ch[i2s_port]);
    if (i2s_vdma_status != VDMA_OK) {
        log_hal_error("[I2S%d]SET RX DMA ERROR, vdma_init fail\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    i2s_vdma_status = vdma_configure(i2s_vdma_rx_ch[i2s_port], &i2s_vdma_config);
    if (i2s_vdma_status != VDMA_OK) {
        log_hal_error("[I2S%d]SET RX DMA ERROR, vdma_configure fail\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    i2s_vdma_status = vdma_set_threshold(i2s_vdma_rx_ch[i2s_port], i2s_threshold);
    if (i2s_vdma_status != VDMA_OK) {
        log_hal_error("[I2S%d]SET RX DMA ERROR, vdma_set_threshold fail\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    i2s_vdma_status = vdma_start(i2s_vdma_rx_ch[i2s_port]);
    if (i2s_vdma_status != VDMA_OK) {
        log_hal_error("[I2S%d]SET RX DMA ERROR, vdma_start fail\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    i2s_internal_cfg[i2s_port].i2s_vfifo.rx_vfifo_base = (uint32_t *)dma_config->buffer;
    i2s_internal_cfg[i2s_port].i2s_vfifo.rx_vfifo_length = dma_config->buffer_length;
    i2s_internal_cfg[i2s_port].i2s_vfifo.rx_vfifo_threshold = dma_config->threshold;
    i2s_internal_cfg[i2s_port].i2s_vfifo.rx_dma_configured = true;

    return HAL_I2S_STATUS_OK;
}

hal_i2s_status_t hal_i2s_register_tx_callback_ex(hal_i2s_port_t i2s_port, hal_i2s_tx_callback_t tx_callback, void *user_data)
{
    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]REGISTER TX CALLBACK ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (NULL == tx_callback) {
        log_hal_error("[I2S%d]REGISTER TX CALLBACK ERROR, tx_callback is null\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    i2s_internal_cfg[i2s_port].user_tx_callback_func = tx_callback;
    i2s_internal_cfg[i2s_port].user_tx_data = user_data;

    vdma_status_t status;

    status = vdma_register_callback(i2s_vdma_tx_ch[i2s_port], i2s_tx_dma_callback_handler, &i2s_internal_cfg[i2s_port]);
    if (status != VDMA_OK) {
        log_hal_error("[I2S%d]REGISTER TX CALLBACK ERROR, vdma_register_callback failed\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    return HAL_I2S_STATUS_OK;

}

hal_i2s_status_t hal_i2s_register_rx_callback_ex(hal_i2s_port_t i2s_port, hal_i2s_rx_callback_t rx_callback, void *user_data)
{
    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]REGISTER RX CALLBACK ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (NULL == rx_callback) {
        log_hal_error("[I2S%d]REGISTER RX CALLBACK ERROR, rx_callback is null\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    i2s_internal_cfg[i2s_port].user_rx_callback_func = rx_callback;
    i2s_internal_cfg[i2s_port].user_rx_data = user_data;

    vdma_status_t status;

    status = vdma_register_callback(i2s_vdma_rx_ch[i2s_port], i2s_rx_dma_callback_handler, &i2s_internal_cfg[i2s_port]);
    if (status != VDMA_OK) {
        log_hal_error("[I2S%d]REGISTER RX CALLBACK ERROR, vdma_register_callback failed\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    return HAL_I2S_STATUS_OK;

}

hal_i2s_status_t  hal_i2s_enable_tx_ex(hal_i2s_port_t i2s_port)
{
    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]TX ENABLE ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    i2s_internal_cfg[i2s_port].i2s_state |= (uint8_t)(1 << I2S_STATE_TX_RUNNING);

    if (i2s_port == HAL_I2S_0) {
        DRV_Reg8(I2S_0_I2S1_DL_SR_EN_CONTROL__F_CR_I2S1_OUT_EN) = (uint8_t)0x1;
    } else {
        DRV_Reg8(I2S_1_I2S1_DL_SR_EN_CONTROL__F_CR_I2S1_OUT_EN) = (uint8_t)0x1;
    }
    return HAL_I2S_STATUS_OK;
}

hal_i2s_status_t  hal_i2s_disable_tx_ex(hal_i2s_port_t i2s_port)
{
    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]TX DISABLE ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (i2s_port == HAL_I2S_0) {
        i2s_flush_tx_dma(i2s_port);
        DRV_Reg8(I2S_0_I2S1_DL_SR_EN_CONTROL__F_CR_I2S1_OUT_EN) = (uint8_t)0x0;

    } else {//HAL_I2S_1
        i2s_flush_tx_dma(i2s_port);
        DRV_Reg8(I2S_1_I2S1_DL_SR_EN_CONTROL__F_CR_I2S1_OUT_EN) = (uint8_t)0x0;
    }

    i2s_internal_cfg[i2s_port].i2s_state &= (uint8_t)(~(1 << I2S_STATE_TX_RUNNING));

    return HAL_I2S_STATUS_OK;
}

hal_i2s_status_t  hal_i2s_enable_rx_ex(hal_i2s_port_t i2s_port)
{
    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]RX ENABLE ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (i2s_port == HAL_I2S_0) {
        DRV_Reg8(I2S_0_I2S1_UL_SR_EN_CONTROL__F_CR_I2S1_IN_EN) = (uint8_t)0x1;
    } else {
        DRV_Reg8(I2S_1_I2S1_UL_SR_EN_CONTROL__F_CR_I2S1_IN_EN) = (uint8_t)0x1;
    }

    i2s_internal_cfg[i2s_port].i2s_state |= (uint8_t)(1 << I2S_STATE_RX_RUNNING);

    return HAL_I2S_STATUS_OK;
}

hal_i2s_status_t hal_i2s_disable_rx_ex(hal_i2s_port_t i2s_port)
{
    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]RX DISABLE ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (i2s_port == HAL_I2S_0) {
        DRV_Reg8(I2S_0_I2S1_UL_SR_EN_CONTROL__F_CR_I2S1_IN_EN) = (uint8_t)0x0;
    } else {//HAL_I2S_1
        DRV_Reg8(I2S_1_I2S1_UL_SR_EN_CONTROL__F_CR_I2S1_IN_EN) = (uint8_t)0x0;
    }

    i2s_internal_cfg[i2s_port].i2s_state &= (uint8_t)(~(1 << I2S_STATE_RX_RUNNING));

    return HAL_I2S_STATUS_OK;
}

hal_i2s_status_t  hal_i2s_enable_clock_ex(hal_i2s_port_t i2s_port)
{
    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]ENABLE CLOCK ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    i2s_xpll_open(&(i2s_internal_cfg[i2s_port]));

    if (i2s_port == HAL_I2S_0) {
#ifdef HAL_SLEEP_MANAGER_ENABLED
        if (is_lock_sleep[i2s_port] == false) {
            hal_sleep_manager_acquire_sleeplock(SLEEP_LOCK_I2S0, HAL_SLEEP_LOCK_ALL);/*lock sleep mode*/
            is_lock_sleep[HAL_I2S_0] = true;
            log_hal_info("[I2S%d]Lock sleep successfully\r\n", i2s_port);
        }
#endif
        i2s_internal_cfg[i2s_port].i2s_audiotop_enabled = true;
        /*AHB clock*/
        hal_clock_enable(i2s_clock_cg[i2s_port]);
        //Enable I2S CG power
        DRV_Reg8(I2S_0_I2S1_GLOBAL_EN_CONTROL__F_CR_PDN_AUD_26M) = (uint8_t)0x0;
        DRV_Reg8(I2S_0_I2S1_DL_SR_EN_CONTROL__F_CR_PDN_I2SO1) = (uint8_t)0x0;
        DRV_Reg8(I2S_0_I2S1_UL_SR_EN_CONTROL__F_CR_PDN_I2SIN1) = (uint8_t)0x0;
        /*Soft reset I2S FIFO and in/out control*/
        DRV_Reg8(I2S_0_I2S1_SOFT_RESET) = (uint8_t)0x1;
        DRV_Reg8(I2S_0_I2S1_SOFT_RESET) = (uint8_t)0x0;
        //Enable I2S FIFO clock
        DRV_Reg8(I2S_0_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_DL_FIFO_EN) = (uint8_t)0x1;
        DRV_Reg8(I2S_0_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_UL_FIFO_EN) = (uint8_t)0x1;
        DRV_Reg8(I2S_0_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_ENABLE) = (uint8_t)0x1;

    } else {
#ifdef HAL_SLEEP_MANAGER_ENABLED
        if (is_lock_sleep[i2s_port] == false) {
            hal_sleep_manager_acquire_sleeplock(SLEEP_LOCK_I2S1, HAL_SLEEP_LOCK_ALL);/*lock sleep mode*/
            is_lock_sleep[HAL_I2S_1] = true;
            log_hal_info("[I2S%d]Lock sleep successfully\r\n", i2s_port);
        }
#endif
        i2s_internal_cfg[i2s_port].i2s_audiotop_enabled = true;
        /*Asystop AHB clock*/
        hal_clock_enable(i2s_clock_cg[i2s_port]);
        //Enable I2S CG power
        DRV_Reg8(I2S_1_I2S1_GLOBAL_EN_CONTROL__F_CR_PDN_AUD_26M) = (uint8_t)0x0;
        DRV_Reg8(I2S_1_I2S1_DL_SR_EN_CONTROL__F_CR_PDN_I2SO1) = (uint8_t)0x0;
        DRV_Reg8(I2S_1_I2S1_UL_SR_EN_CONTROL__F_CR_PDN_I2SIN1) = (uint8_t)0x0;
        /*Soft reset I2S FIFO and in/out control*/
        DRV_Reg8(I2S_1_I2S1_SOFT_RESET) = (uint8_t)0x1;
        DRV_Reg8(I2S_1_I2S1_SOFT_RESET) = (uint8_t)0x0;
        //Enable I2S FIFO clock
        DRV_Reg8(I2S_1_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_DL_FIFO_EN) = (uint8_t)0x1;
        DRV_Reg8(I2S_1_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_UL_FIFO_EN) = (uint8_t)0x1;
        DRV_Reg8(I2S_1_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_ENABLE) = (uint8_t)0x1;

    }

    return HAL_I2S_STATUS_OK;

}

hal_i2s_status_t  hal_i2s_disable_clock_ex(hal_i2s_port_t i2s_port)
{
    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]DISABLE CLOCK ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (i2s_internal_cfg[i2s_port].i2s_state != I2S_STATE_INIT) {
        log_hal_error("[I2S%d]DISABLE CLOCK ERROR, i2s_state=%d\r\n", i2s_port, i2s_internal_cfg[i2s_port].i2s_state);
        return HAL_I2S_STATUS_ERROR;
    }

    if (i2s_internal_cfg[i2s_port].i2s_audiotop_enabled != true) {
        return HAL_I2S_STATUS_ERROR;
    }

    if (i2s_port == HAL_I2S_0) {
#ifdef HAL_SLEEP_MANAGER_ENABLED
        if (is_lock_sleep[i2s_port] == true) {
            hal_sleep_manager_release_sleeplock(SLEEP_LOCK_I2S0, HAL_SLEEP_LOCK_ALL);/*lock sleep mode*/
            is_lock_sleep[i2s_port] = false;
            log_hal_info("[I2S%d]Unlock sleep successfully\r\n", i2s_port);
        }
#endif
        i2s_internal_cfg[i2s_port].i2s_audiotop_enabled = false;
        //Disable I2S FIFO clock
        DRV_Reg8(I2S_0_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_ENABLE) = (uint8_t)0x0;
        DRV_Reg8(I2S_0_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_DL_FIFO_EN) = (uint8_t)0x0;
        DRV_Reg8(I2S_0_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_UL_FIFO_EN) = (uint8_t)0x0;
        /*Soft reset I2S FIFO and in/out control*/
        DRV_Reg8(I2S_0_I2S1_SOFT_RESET) = (uint8_t)0x1;
        DRV_Reg8(I2S_0_I2S1_SOFT_RESET) = (uint8_t)0x0;
        //Disable I2S CG power
        DRV_Reg8(I2S_0_I2S1_DL_SR_EN_CONTROL__F_CR_PDN_I2SO1) = (uint8_t)0x1;
        DRV_Reg8(I2S_0_I2S1_UL_SR_EN_CONTROL__F_CR_PDN_I2SIN1) = (uint8_t)0x1;
        DRV_Reg8(I2S_0_I2S1_GLOBAL_EN_CONTROL__F_CR_PDN_AUD_26M) = (uint8_t)0x1;
        /*Audiotop AHB clock*/
        hal_clock_disable(i2s_clock_cg[i2s_port]);

    } else {
#ifdef HAL_SLEEP_MANAGER_ENABLED
        if (is_lock_sleep[i2s_port] == true) {
            hal_sleep_manager_release_sleeplock(SLEEP_LOCK_I2S1, HAL_SLEEP_LOCK_ALL);/*lock sleep mode*/
            is_lock_sleep[i2s_port] = false;
            log_hal_info("[I2S%d]Unlock sleep successfully\r\n", i2s_port);
        }
#endif
        i2s_internal_cfg[i2s_port].i2s_audiotop_enabled = false;
        //Disable I2S FIFO clock
        DRV_Reg8(I2S_1_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_ENABLE) = (uint8_t)0x0;
        DRV_Reg8(I2S_1_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_DL_FIFO_EN) = (uint8_t)0x0;
        DRV_Reg8(I2S_1_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_UL_FIFO_EN) = (uint8_t)0x0;
        /*Soft reset I2S FIFO and in/out control*/
        DRV_Reg8(I2S_1_I2S1_SOFT_RESET) = (uint8_t)0x1;
        DRV_Reg8(I2S_1_I2S1_SOFT_RESET) = (uint8_t)0x0;
        //Disable I2S CG power
        DRV_Reg8(I2S_1_I2S1_DL_SR_EN_CONTROL__F_CR_PDN_I2SO1) = (uint8_t)0x1;
        DRV_Reg8(I2S_1_I2S1_UL_SR_EN_CONTROL__F_CR_PDN_I2SIN1) = (uint8_t)0x1;
        DRV_Reg8(I2S_1_I2S1_GLOBAL_EN_CONTROL__F_CR_PDN_AUD_26M) = (uint8_t)0x1;

        /*Audiotop AHB clock*/
        hal_clock_disable(i2s_clock_cg[i2s_port]);
    }

#if defined(HAL_I2S_FEATURE_XTAL_I2S0) || defined(HAL_I2S_FEATURE_XTAL_I2S1)
    if ((i2s_internal_cfg[i2s_port].i2s_audiotop_enabled == false) && 
        (i2s_internal_cfg[i2s_port].i2s_clock_source != HAL_I2S_CLOCK_XO_26M) && 
        (i2s_internal_cfg[i2s_port].i2s_clock_source != HAL_I2S_CLOCK_MAX)) {
        i2s_xpll_close();
    }      
#else //Both i2s0 and i2s1 share on internal xpll
    if ((i2s_internal_cfg[HAL_I2S_0].i2s_audiotop_enabled == false) && (i2s_internal_cfg[HAL_I2S_1].i2s_audiotop_enabled == false)) {
        i2s_xpll_close();
    }
#endif


    return HAL_I2S_STATUS_OK;

}

hal_i2s_status_t  hal_i2s_enable_tx_interrupt_ex(hal_i2s_port_t i2s_port)
{

    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]ENABLE TX INTERRUPT ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (i2s_internal_cfg[i2s_port].user_tx_callback_func == NULL) {
        log_hal_error("[I2S%d]ENABLE TX INTERRUPT ERROR, user_tx_callback_func undefined\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    vdma_status_t status;

    status = vdma_enable_interrupt(i2s_vdma_tx_ch[i2s_port]);
    if (status != VDMA_OK) {
        log_hal_error("[I2S%d]ENABLE TX INTERRUPT ERROR, vdma_enable_interrupt failed\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    i2s_internal_cfg[i2s_port].i2s_vfifo.tx_dma_interrupt = true;

    return HAL_I2S_STATUS_OK;

}

hal_i2s_status_t  hal_i2s_disable_tx_interrupt_ex(hal_i2s_port_t i2s_port)
{

    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]DISABLE TX INTERRUPT ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (i2s_internal_cfg[i2s_port].user_tx_callback_func == NULL) {
        log_hal_error("[I2S%d]DISABLE TX INTERRUPT ERROR, user_tx_callback_func undefined\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    vdma_status_t status;

    status = vdma_disable_interrupt(i2s_vdma_tx_ch[i2s_port]);
    if (status != VDMA_OK) {
        log_hal_error("[I2S%d]DISABLE TX DMA INTERRUPT ERROR, vdma_disable_interrupt failed\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }
    i2s_internal_cfg[i2s_port].i2s_vfifo.tx_dma_interrupt = false;

    return HAL_I2S_STATUS_OK;

}

hal_i2s_status_t  hal_i2s_enable_rx_interrupt_ex(hal_i2s_port_t i2s_port)
{

    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]RX INTERRUPT ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (i2s_internal_cfg[i2s_port].user_rx_callback_func == NULL) {
        log_hal_error("[I2S%d]ENABLE RX INTERRUPT ERROR, user_rx_callback_func undefined\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    vdma_status_t status;

    status = vdma_enable_interrupt(i2s_vdma_rx_ch[i2s_port]);
    if (status != VDMA_OK) {
        log_hal_error("[I2S%d]ENABLE RX INTERRUPT ERROR, vdma_enable_interrupt failed\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }
    i2s_internal_cfg[i2s_port].i2s_vfifo.rx_dma_interrupt = true;

    return HAL_I2S_STATUS_OK;

}

hal_i2s_status_t  hal_i2s_disable_rx_interrupt_ex(hal_i2s_port_t i2s_port)
{

    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]DISABLE RX INTERRUPT ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (i2s_internal_cfg[i2s_port].user_rx_callback_func == NULL) {
        log_hal_error("[I2S%d]DISABLE RX INTERRUPT ERROR, user_rx_callback_func undefined\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    vdma_status_t status;

    status = vdma_disable_interrupt(i2s_vdma_rx_ch[i2s_port]);
    if (status != VDMA_OK) {
        log_hal_error("[I2S%d]DISABLE RX INTERRUPT ERROR, vdma_disable_interrupt failed\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }
    i2s_internal_cfg[i2s_port].i2s_vfifo.rx_dma_interrupt = false;

    return HAL_I2S_STATUS_OK;

}

ATTR_TEXT_IN_TCM hal_i2s_status_t hal_i2s_tx_write_ex(hal_i2s_port_t i2s_port, uint32_t data)
{
    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]WRITE ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (i2s_port == HAL_I2S_0) {
        DRV_Reg32(I2S_0_TX_DMA_FIFO) = data;
    } else {
        DRV_Reg32(I2S_1_TX_DMA_FIFO) = data;
    }
    return HAL_I2S_STATUS_OK;
}

ATTR_TEXT_IN_TCM hal_i2s_status_t hal_i2s_rx_read_ex(hal_i2s_port_t i2s_port, uint32_t *data)
{
    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]READ ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    if (i2s_port == HAL_I2S_0) {
        *data = DRV_Reg32(I2S_0_RX_DMA_FIFO);
    } else {
        *data = DRV_Reg32(I2S_1_RX_DMA_FIFO);
    }
    return HAL_I2S_STATUS_OK;
}

hal_i2s_status_t hal_i2s_get_tx_sample_count_ex(hal_i2s_port_t i2s_port, uint32_t *sample_count)
{
    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]GET TX SAMPLE COUNT ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    vdma_status_t status;
    if (i2s_internal_cfg[i2s_port].i2s_vfifo.tx_vfifo_length == 0) {
        log_hal_error("[I2S%d]GET TX SAMPLE COUNT ERROR, tx vfifo length == 0\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }
    status = vdma_get_available_send_space(i2s_vdma_tx_ch[i2s_port], sample_count);
    if (status != VDMA_OK) {
        log_hal_error("[I2S%d]GET TX SAMPLE COUNT ERROR,  vdma_get_available_send_space fail\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    return HAL_I2S_STATUS_OK;

}

hal_i2s_status_t hal_i2s_get_rx_sample_count_ex(hal_i2s_port_t i2s_port, uint32_t *sample_count)
{
    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]GET RX SAMPLE COUNT ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    vdma_status_t status;
    if (i2s_internal_cfg[i2s_port].i2s_vfifo.rx_vfifo_length == 0) {
        log_hal_error("[I2S%d]GET RX SAMPLE COUNT ERROR, rx vfifo length == 0\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    status = vdma_get_available_receive_bytes(i2s_vdma_rx_ch[i2s_port], sample_count);
    if (status != VDMA_OK) {
        log_hal_error("[I2S%d]GET RX SAMPLE COUNT ERROR, vdma_get_available_receive_bytes failed\r\n", i2s_port);
        return HAL_I2S_STATUS_ERROR;
    }

    return HAL_I2S_STATUS_OK;
}

hal_i2s_status_t  hal_i2s_set_eof_ex(hal_i2s_port_t i2s_port)
{
    if (i2s_port != HAL_I2S_0 && i2s_port != HAL_I2S_1) {
        log_hal_error("[I2S%d]SET EOF ERROR, invalid i2s_port\r\n", i2s_port);
        return HAL_I2S_STATUS_INVALID_PARAMETER;
    }
    if (i2s_internal_cfg[i2s_port].i2s_tx_eof == false) {
        vdma_set_threshold(i2s_vdma_tx_ch[i2s_port], I2S_EOF_THRESHOLD);
        i2s_internal_cfg[i2s_port].i2s_tx_eof = true;
    }
    return HAL_I2S_STATUS_OK;
}

hal_i2s_status_t hal_i2s_init(hal_i2s_initial_type_t i2s_initial_type)
{
    return hal_i2s_init_ex(HAL_I2S_0, i2s_initial_type);
}

hal_i2s_status_t hal_i2s_deinit(void)
{
    return hal_i2s_deinit_ex(HAL_I2S_0);
}

hal_i2s_status_t hal_i2s_get_config(hal_i2s_config_t *config)
{
    *config = i2s_internal_cfg[HAL_I2S_0].i2s_user_config;

    return HAL_I2S_STATUS_OK;
}

hal_i2s_status_t hal_i2s_set_config(const hal_i2s_config_t *config)
{
    return hal_i2s_set_config_ex(HAL_I2S_0, config);
}

hal_i2s_status_t hal_i2s_set_tx_dma(const hal_i2s_dma_config_t *dma_config)
{
    return hal_i2s_set_tx_dma_ex(HAL_I2S_0, dma_config);
}

hal_i2s_status_t hal_i2s_set_rx_dma(const hal_i2s_dma_config_t *dma_config)
{
    return hal_i2s_set_rx_dma_ex(HAL_I2S_0, dma_config);
}

hal_i2s_status_t hal_i2s_register_tx_callback(hal_i2s_tx_callback_t tx_callback, void *user_data)
{
    return hal_i2s_register_tx_callback_ex(HAL_I2S_0, tx_callback, user_data);
}

hal_i2s_status_t hal_i2s_register_rx_callback(hal_i2s_rx_callback_t rx_callback, void *user_data)
{
    return hal_i2s_register_rx_callback_ex(HAL_I2S_0, rx_callback, user_data);
}

hal_i2s_status_t  hal_i2s_enable_tx(void)
{
    return hal_i2s_enable_tx_ex(HAL_I2S_0);
}

hal_i2s_status_t  hal_i2s_disable_tx(void)
{
    return hal_i2s_disable_tx_ex(HAL_I2S_0);
}

hal_i2s_status_t  hal_i2s_enable_rx(void)
{
    return hal_i2s_enable_rx_ex(HAL_I2S_0);
}

hal_i2s_status_t hal_i2s_disable_rx(void)
{
    return hal_i2s_disable_rx_ex(HAL_I2S_0);
}

hal_i2s_status_t  hal_i2s_enable_clock(void)
{
    return hal_i2s_enable_clock_ex(HAL_I2S_0);
}

hal_i2s_status_t  hal_i2s_disable_clock(void)
{
    return hal_i2s_disable_clock_ex(HAL_I2S_0);
}

hal_i2s_status_t  hal_i2s_enable_tx_interrupt(void)
{
    return hal_i2s_enable_tx_interrupt_ex(HAL_I2S_0);
}

hal_i2s_status_t  hal_i2s_disable_tx_interrupt(void)
{
    return hal_i2s_disable_tx_interrupt_ex(HAL_I2S_0);
}

hal_i2s_status_t  hal_i2s_enable_rx_interrupt(void)
{
    return hal_i2s_enable_rx_interrupt_ex(HAL_I2S_0);
}

hal_i2s_status_t  hal_i2s_disable_rx_interrupt(void)
{
    return hal_i2s_disable_rx_interrupt_ex(HAL_I2S_0);
}


hal_i2s_status_t hal_i2s_tx_write(uint32_t data)
{
    DRV_Reg32(I2S_0_TX_DMA_FIFO) = data;

    return HAL_I2S_STATUS_OK;
}

hal_i2s_status_t hal_i2s_rx_read(uint32_t *data)
{
    *data = DRV_Reg32(I2S_0_RX_DMA_FIFO);

    return HAL_I2S_STATUS_OK;
}

hal_i2s_status_t hal_i2s_get_tx_sample_count(uint32_t *sample_count)
{
    return hal_i2s_get_tx_sample_count_ex(HAL_I2S_0, sample_count);
}

hal_i2s_status_t hal_i2s_get_rx_sample_count(uint32_t *sample_count)
{
    return hal_i2s_get_rx_sample_count_ex(HAL_I2S_0, sample_count);
}

hal_i2s_status_t  hal_i2s_set_eof(void)
{
    return hal_i2s_set_eof_ex(HAL_I2S_0);
}

#endif//#ifdef HAL_I2S_MODULE_ENABLED

