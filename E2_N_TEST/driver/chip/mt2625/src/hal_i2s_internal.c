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

#include "hal_i2s_internal.h"
#ifdef HAL_I2S_MODULE_ENABLED
#include "hal_gpt.h"

static volatile bool bl_xpll_open = false;

void i2s_xpll_close(void)
{
    if (!bl_xpll_open) {
        return;
    }
    log_hal_info("[I2S]XPLL CLOSE\r\n");
    DRV_Reg32(XPLL_CTL1) = 0x00000000;
    DRV_Reg32(XPLL_CTL11) = 0x00030000;
    DRV_Reg32(XPLL_CTL0) = 0x01000101;
    DRV_Reg32(XPLL_CTL0) = 0x01000100;
    DRV_Reg32(XPLL_CTL0) = 0x00000100;
    DRV_Reg32(XPLL_CTL12) = 0x00000000;
    hal_gpt_delay_us(5);
    bl_xpll_open = false;
}

void i2s_xpll_open(i2s_internal_t *hal_i2s)
{
    if ((hal_i2s->i2s_clock_source == HAL_I2S_CLOCK_XO_26M)) {
        return;
    }

    if (bl_xpll_open) {
        log_hal_info("[I2S%d]XPLL ALREADY OPENED\r\n", hal_i2s->i2s_port);
        return;
    } else {
        log_hal_info("[I2S%d]XPLL OPEN\r\n", hal_i2s->i2s_port);
    }

    DRV_Reg32(XPLL_CTL3) = 0x00040000;
    DRV_Reg32(XPLL_CTL8) = 0x00000000;

    if ((hal_i2s->i2s_clock_source == HAL_I2S_CLOCK_XPLL_24_576M)) {
        DRV_Reg32(XPLL_CTL10) = 0x0F1FAA4C;
    }
    if ((hal_i2s->i2s_clock_source == HAL_I2S_CLOCK_XPLL_22_5792M)) {
        DRV_Reg32(XPLL_CTL10) = 0x0DE517AA;
    }

    DRV_Reg32(XPLL_CTL0) = 0x01000100;
    DRV_Reg32(XPLL_CTL0) = 0x01000101;
    hal_gpt_delay_us(5);
    DRV_Reg32(XPLL_CTL0) = 0x01000001;
    DRV_Reg32(XPLL_CTL1) = 0x00000101;
    DRV_Reg32(XPLL_CTL11) = 0x00030001;
    hal_gpt_delay_us(3);
    DRV_Reg32(XPLL_CTL12) = 0x00000001;
    hal_gpt_delay_us(30);
    bl_xpll_open = true;
}


void i2s_set_initial_parameter(i2s_internal_t *hal_i2s)
{
    memset(hal_i2s, 0 , sizeof(i2s_internal_t));

    hal_i2s->i2s_initial_type = HAL_I2S_TYPE_MAX;
    hal_i2s->i2s_port = HAL_I2S_0;
    hal_i2s->i2s_clock_source = HAL_I2S_CLOCK_MAX;
    hal_i2s->i2s_vfifo.tx_vfifo_length = 0;
    hal_i2s->i2s_vfifo.tx_vfifo_threshold = 0;
    hal_i2s->i2s_vfifo.rx_vfifo_length = 0;
    hal_i2s->i2s_vfifo.rx_vfifo_threshold = 0;

    hal_i2s->i2s_user_config.clock_mode = HAL_I2S_MASTER;
    hal_i2s->i2s_user_config.frame_sync_width = HAL_I2S_FRAME_SYNC_WIDTH_32;
    hal_i2s->i2s_user_config.sample_width = HAL_I2S_SAMPLE_WIDTH_16BIT;
    hal_i2s->i2s_user_config.tx_mode = HAL_I2S_TX_DUPLICATE_DISABLE;

    hal_i2s->i2s_user_config.i2s_in.channel_number = HAL_I2S_STEREO;
    hal_i2s->i2s_user_config.i2s_in.sample_rate = HAL_I2S_SAMPLE_RATE_48K;
    hal_i2s->i2s_user_config.i2s_in.msb_offset = 0;
    hal_i2s->i2s_user_config.i2s_in.lr_swap = HAL_I2S_LR_SWAP_DISABLE;
    hal_i2s->i2s_user_config.i2s_in.word_select_inverse = HAL_I2S_WORD_SELECT_INVERSE_DISABLE;

    hal_i2s->i2s_user_config.i2s_out.channel_number = HAL_I2S_STEREO;
    hal_i2s->i2s_user_config.i2s_out.sample_rate = HAL_I2S_SAMPLE_RATE_48K;
    hal_i2s->i2s_user_config.i2s_out.msb_offset = 0;
    hal_i2s->i2s_user_config.i2s_out.lr_swap = HAL_I2S_LR_SWAP_DISABLE;
    hal_i2s->i2s_user_config.i2s_out.word_select_inverse = HAL_I2S_WORD_SELECT_INVERSE_DISABLE;

}

void i2s_set_parameter(i2s_internal_t *hal_i2s)
{
    uint32_t i2s_temp_global_cfg[HAL_I2S_MAX] = {0x00020028};
    uint32_t i2s_temp_dl_cfg[HAL_I2S_MAX] = {0x00000000};
    uint32_t i2s_temp_ul_cfg[HAL_I2S_MAX] = {0x00000000};

    if (hal_i2s->i2s_clock_source == HAL_I2S_CLOCK_XO_26M) {
        i2s_temp_global_cfg[hal_i2s->i2s_port] = 0x20000028;
    } else if (hal_i2s->i2s_clock_source == HAL_I2S_CLOCK_XPLL_26M) {
        i2s_temp_global_cfg[hal_i2s->i2s_port] = 0x20040028;
    } else if (hal_i2s->i2s_clock_source == HAL_I2S_CLOCK_XPLL_24_576M) {
        i2s_temp_global_cfg[hal_i2s->i2s_port] = 0x00040028;
    } else { //HAL_I2S_CLOCK_XPLL_22_5792M
        i2s_temp_global_cfg[hal_i2s->i2s_port] = 0x10040028;
    }

    //data loopback mode
    if (hal_i2s->i2s_initial_type == HAL_I2S_TYPE_INTERNAL_LOOPBACK_MODE) {
        i2s_temp_global_cfg[hal_i2s->i2s_port] |= 0x80000000;
    }

    //mono mode
    if (hal_i2s->i2s_user_config.i2s_out.channel_number == HAL_I2S_MONO) {
        i2s_temp_global_cfg[hal_i2s->i2s_port] |= 0x00000100;
    }

    //mono duplication, duplicate L to R
    if (hal_i2s->i2s_user_config.tx_mode == HAL_I2S_TX_DUPLICATE_LEFT_CHANNEL) {
        i2s_temp_global_cfg[hal_i2s->i2s_port] |= 0x00000200;
    }

    //tx lr swap
    if (hal_i2s->i2s_user_config.i2s_out.lr_swap == HAL_I2S_LR_SWAP_ENABLE) {
        i2s_temp_global_cfg[hal_i2s->i2s_port] |= 0x00000080;
    }

    //rx lr swap
    if (hal_i2s->i2s_user_config.i2s_in.lr_swap == HAL_I2S_LR_SWAP_ENABLE) {
        i2s_temp_global_cfg[hal_i2s->i2s_port] |= 0x00000001;
    }


    /*bits per frame sync, bit[14:13]   00 = 32bits, 01 = 64bits, 10 =128bits*/
    if (hal_i2s->i2s_user_config.frame_sync_width == HAL_I2S_FRAME_SYNC_WIDTH_32) {
        log_hal_error("[I2S%d]FRAME_SYNC_WIDTH_32\r\n", hal_i2s->i2s_port);
    }
    if (hal_i2s->i2s_user_config.frame_sync_width == HAL_I2S_FRAME_SYNC_WIDTH_64) {
        i2s_temp_dl_cfg[hal_i2s->i2s_port] |= (uint32_t)0x2;
        i2s_temp_ul_cfg[hal_i2s->i2s_port] |= (uint32_t)0x2;
        log_hal_error("[I2S%d]FRAME_SYNC_WIDTH_64\r\n", hal_i2s->i2s_port);
    }

    //sample width, bit18 and bit1: 1 for 24bit mode, 0 for 16bit mode
    if (hal_i2s->i2s_user_config.sample_width == HAL_I2S_SAMPLE_WIDTH_24BIT) {
        if (hal_i2s->i2s_user_config.clock_mode == HAL_I2S_MASTER) {
            i2s_temp_dl_cfg[hal_i2s->i2s_port] |= (uint32_t)0x0006008B;//32bit  input master i2s mode
            i2s_temp_ul_cfg[hal_i2s->i2s_port] |= (uint32_t)0x0006800B;
            log_hal_error("[I2S%d]I2S_MASTER\r\n", hal_i2s->i2s_port);
        } else {
            i2s_temp_dl_cfg[hal_i2s->i2s_port] |= (uint32_t)0x0006008F;//32bit  input slave i2s mode
            i2s_temp_ul_cfg[hal_i2s->i2s_port] |= (uint32_t)0x0006800F;
            log_hal_error("[I2S%d]I2S_SLAVE\r\n", hal_i2s->i2s_port);
        }
        log_hal_error("[I2S%d]SAMPLE_WIDTH_24BIT\r\n", hal_i2s->i2s_port);

    } else {//16bit
        if (hal_i2s->i2s_user_config.clock_mode == HAL_I2S_MASTER) {
            i2s_temp_dl_cfg[hal_i2s->i2s_port] |= (uint32_t)0x00020009;//16bit  input master i2s mode
            i2s_temp_ul_cfg[hal_i2s->i2s_port] |= (uint32_t)0x00028009;
            log_hal_error("[I2S%d]I2S_MASTER\r\n", hal_i2s->i2s_port);
        } else {
            i2s_temp_dl_cfg[hal_i2s->i2s_port] |= (uint32_t)0x0002000D;//16bit  input slave i2s mode
            i2s_temp_ul_cfg[hal_i2s->i2s_port] |= (uint32_t)0x0002800D;
            log_hal_error("[I2S%d]I2S_SLAVE\r\n", hal_i2s->i2s_port);
        }
        log_hal_error("[I2S%d]SAMPLE_WIDTH_16BIT\r\n", hal_i2s->i2s_port);
    }
    if (hal_i2s->i2s_port == HAL_I2S_0) {
        DRV_Reg32(I2S_0_I2S1_GLOBAL_CONTROL) = i2s_temp_global_cfg[HAL_I2S_0];
        DRV_Reg32(I2S_0_I2S1_DL_CONTROL) = i2s_temp_dl_cfg[HAL_I2S_0];
        DRV_Reg32(I2S_0_I2S1_UL_CONTROL) = i2s_temp_ul_cfg[HAL_I2S_0];
        log_hal_info("[I2S0]GLOBAL_CONTROL=%x\r\n", (unsigned int)DRV_Reg32(I2S_0_I2S1_GLOBAL_CONTROL));
        log_hal_info("[I2S0]DL_CONTROL=%x\r\n", (unsigned int)DRV_Reg32(I2S_0_I2S1_DL_CONTROL));
        log_hal_info("[I2S0]UL_CONTROL=%x\r\n", (unsigned int)DRV_Reg32(I2S_0_I2S1_UL_CONTROL));
    } else {
        DRV_Reg32(I2S_1_I2S1_GLOBAL_CONTROL) = i2s_temp_global_cfg[HAL_I2S_1];
        DRV_Reg32(I2S_1_I2S1_DL_CONTROL) = i2s_temp_dl_cfg[HAL_I2S_1];
        DRV_Reg32(I2S_1_I2S1_UL_CONTROL) = i2s_temp_ul_cfg[HAL_I2S_1];
        log_hal_info("[I2S1]GLOBAL_CONTROL=%x\r\n", (unsigned int)DRV_Reg32(I2S_1_I2S1_GLOBAL_CONTROL));
        log_hal_info("[I2S1]DL_CONTROL=%x\r\n", (unsigned int)DRV_Reg32(I2S_1_I2S1_DL_CONTROL));
        log_hal_info("[I2S1]UL_CONTROL=%x\r\n", (unsigned int)DRV_Reg32(I2S_1_I2S1_UL_CONTROL));
    }

}

hal_i2s_status_t i2s_set_clock(i2s_internal_t *hal_i2s, hal_i2s_sample_rate_t sample_rate)
{
    uint8_t fs_value = 0;
    hal_i2s_fs_base_t fs_base;

    switch (sample_rate) {
        case HAL_I2S_SAMPLE_RATE_8K:
            log_hal_info("[I2S%d]FS 8K\r\n", hal_i2s->i2s_port);
            fs_value = (uint8_t)0x0;
            fs_base = HAL_I2S_FS_BASE_8KHZ;
            break;

        case HAL_I2S_SAMPLE_RATE_11_025K:
            log_hal_info("[I2S%d]FS 11.025K\r\n", hal_i2s->i2s_port);
            fs_value = (uint8_t)0x1;
            fs_base = HAL_I2S_FS_BASE_11_025KHZ;
            break;

        case HAL_I2S_SAMPLE_RATE_12K:
            log_hal_info("[I2S%d]FS 12K\r\n", hal_i2s->i2s_port);
            fs_value = (uint8_t)0x2;
            fs_base = HAL_I2S_FS_BASE_8KHZ;
            break;

        case HAL_I2S_SAMPLE_RATE_16K:
            log_hal_info("[I2S%d]FS 16K\r\n", hal_i2s->i2s_port);
            fs_value = (uint8_t)0x4;
            fs_base = HAL_I2S_FS_BASE_8KHZ;
            break;

        case HAL_I2S_SAMPLE_RATE_22_05K:
            log_hal_info("[I2S%d]FS 22.5K\r\n", hal_i2s->i2s_port);
            fs_value = (uint8_t)0x5;
            fs_base = HAL_I2S_FS_BASE_11_025KHZ;
            break;

        case HAL_I2S_SAMPLE_RATE_24K:
            log_hal_info("[I2S%d]FS 24K\r\n", hal_i2s->i2s_port);
            fs_value = (uint8_t)0x6;
            fs_base = HAL_I2S_FS_BASE_8KHZ;
            break;

        case HAL_I2S_SAMPLE_RATE_32K:
            log_hal_info("[I2S%d]FS 32K\r\n", hal_i2s->i2s_port);
            fs_value = (uint8_t)0x8;
            fs_base = HAL_I2S_FS_BASE_8KHZ;
            break;

        case HAL_I2S_SAMPLE_RATE_44_1K:
            log_hal_info("[I2S%d]FS 44.1K\r\n", hal_i2s->i2s_port);
            fs_value = (uint8_t)0x9;
            fs_base = HAL_I2S_FS_BASE_11_025KHZ;
            break;

        case HAL_I2S_SAMPLE_RATE_48K:
            log_hal_info("[I2S%d]FS 48K\r\n", hal_i2s->i2s_port);
            fs_value = (uint8_t)0xa;
            fs_base = HAL_I2S_FS_BASE_8KHZ;
            break;

        case HAL_I2S_SAMPLE_RATE_88_2K:
            log_hal_info("[I2S%d]FS 88.2K\r\n", hal_i2s->i2s_port);
            fs_value = (uint8_t)0xb;
            fs_base = HAL_I2S_FS_BASE_11_025KHZ;
            break;

        case HAL_I2S_SAMPLE_RATE_96K:
            log_hal_info("[I2S%d]FS 96K\r\n", hal_i2s->i2s_port);
            fs_value = (uint8_t)0xc;
            fs_base = HAL_I2S_FS_BASE_8KHZ;
            break;

        case HAL_I2S_SAMPLE_RATE_176_4K:
            log_hal_info("[I2S%d]FS 176.4K\r\n", hal_i2s->i2s_port);
            fs_value = (uint8_t)0xd;
            fs_base = HAL_I2S_FS_BASE_11_025KHZ;
            break;

        case HAL_I2S_SAMPLE_RATE_192K:
            log_hal_info("[I2S%d]FS 192K\r\n", hal_i2s->i2s_port);
            fs_value = (uint8_t)0xe;
            fs_base = HAL_I2S_FS_BASE_8KHZ;
            break;

        default:
            return HAL_I2S_STATUS_INVALID_PARAMETER;
    }

    hal_i2s->i2s_user_config.i2s_in.sample_rate = sample_rate;
    hal_i2s->i2s_user_config.i2s_out.sample_rate = sample_rate;

    /*Set XPLL or XTAL*/
    if (hal_i2s->i2s_port == HAL_I2S_0) {
#if defined (HAL_I2S_FEATURE_XTAL_I2S0)
        hal_i2s->i2s_clock_source = HAL_I2S_CLOCK_XO_26M;
        fs_value &= (~(uint8_t)0x10);
        log_hal_info("[I20]XTAL MODE(26M)\r\n");
#else
        if (fs_base == HAL_I2S_FS_BASE_8KHZ) {
            hal_i2s->i2s_clock_source = HAL_I2S_CLOCK_XPLL_24_576M;
            log_hal_info("[I20]XPLL MODE(24.576M)\r\n");
        } else {
            hal_i2s->i2s_clock_source = HAL_I2S_CLOCK_XPLL_22_5792M;
            log_hal_info("[I20]XPLL MODE(22.5792M)\r\n");
        }
        fs_value |= (uint8_t)0x10;
#endif
    } else {
#if defined (HAL_I2S_FEATURE_XTAL_I2S1)
        hal_i2s->i2s_clock_source = HAL_I2S_CLOCK_XO_26M;
        fs_value &= (~(uint8_t)0x10);
        log_hal_info("[I21]XTAL MODE(26M)\r\n");
#else
        if (fs_base == HAL_I2S_FS_BASE_8KHZ) {
            hal_i2s->i2s_clock_source = HAL_I2S_CLOCK_XPLL_24_576M;
            log_hal_info("[I21]XPLL MODE(24.576M)\r\n");
        } else {
            hal_i2s->i2s_clock_source = HAL_I2S_CLOCK_XPLL_22_5792M;
            log_hal_info("[I21]XPLL MODE(22.5792M)\r\n");
        }
        fs_value |= (uint8_t)0x10;
#endif
    }

    if (hal_i2s->i2s_port == HAL_I2S_0) {
        DRV_Reg8(I2S_0_I2S1_DL_SR_EN_CONTROL__F_CR_I2S1_OUT_SR) = fs_value;
        DRV_Reg8(I2S_0_I2S1_UL_SR_EN_CONTROL__F_CR_I2S1_IN_SR) = fs_value;
    } else {
        DRV_Reg8(I2S_1_I2S1_DL_SR_EN_CONTROL__F_CR_I2S1_OUT_SR) = fs_value;
        DRV_Reg8(I2S_1_I2S1_UL_SR_EN_CONTROL__F_CR_I2S1_IN_SR) = fs_value;
    }


    return HAL_I2S_STATUS_OK;
}


#endif /* HAL_I2S_MODULE_ENABLED */

