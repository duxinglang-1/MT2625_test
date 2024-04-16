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

//#include <stdio.h>
//#include <string.h>
#include "vsm_driver.h"
// #include "vsm_signal_reg.h"
#include <stdint.h> // #include "vsm_platform_function.h"
// #ifdef MT2511_USE_SPI
// #include "vsm_spi_operation.h"
// #else
// #include "vsm_i2c_operation.h"
// #endif

#include "ppg_control.h"
#include "ppg_control_lib_setting.h"
#include "vsm_sensor_subsys_adaptor.h"

#if defined(MTK_DEBUG_LEVEL_NONE)
log_create_module(mt6381_ppg, PRINT_LEVEL_INFO);

#define LOGE(fmt,arg...)   LOG_E(mt6381_ppg, "[vsm_ppg]"fmt, ##arg)
#define LOGW(fmt,arg...)   LOG_W(mt6381_ppg, "[vsm_ppg]"fmt, ##arg)
#define LOGI(fmt,arg...)   LOG_I(mt6381_ppg, "[vsm_ppg]"fmt, ##arg)
#define LOGD(fmt,arg...)   LOG_D(mt6381_ppg, "[vsm_ppg]"fmt, ##arg)
#else
#define LOGE(fmt,arg...)   printf("[vsm_ppg]"fmt,##arg)
#define LOGW(fmt,arg...)   printf("[vsm_ppg]"fmt,##arg)
#define LOGI(fmt,arg...)   printf("[vsm_ppg]"fmt,##arg)
#define LOGD(fmt,arg...)   printf("[vsm_ppg]"fmt,##arg)
#endif

#define DBG                 0
#define DBG_READ            0
#define DBG_WRITE           0
#define DBG_SRAM            0

/* Setting variables */
int32_t ppg_control_adjust_flag[PPG_CTRL_CH_MAX];
int32_t ppg_control_led_current[PPG_CTRL_CH_MAX];

int32_t ppg_dc_limit_h;
int32_t ppg_dc_limit_l;

/* data input buffer */
int32_t ppg_ctrl_buf_idx[PPG_CTRL_CH_MAX];  /* buffer index */
int32_t ppg_ctrl_in_count[PPG_CTRL_CH_MAX]; /* down sample use */
int32_t ppg_ctrl_buf[PPG_CTRL_CH_MAX][PPG_CTRL_BUF_SIZE];

/* saturate check */
int32_t ppg_ctrl_cnt_pos_edge[PPG_CTRL_CH_MAX];
int32_t ppg_ctrl_cnt_neg_edge[PPG_CTRL_CH_MAX];

/* Filter variables */
int32_t ppg_control_hpf[PPG_CTRL_CH_MAX];

/* state */
int32_t ppg_ctrl_cur_state[PPG_CTRL_CH_MAX];
int32_t ppg_ctrl_pre_state[PPG_CTRL_CH_MAX];

/* timer control */
int32_t ppg_control_timer[PPG_CTRL_CH_MAX];
int32_t ppg_ctrl_time_limit;
int32_t ppg_control_init_flag[PPG_CTRL_CH_MAX];

ppg_control_status_t ppg_control_init(void)
{
    int32_t j, ch;

    for (ch = 0; ch < PPG_CTRL_CH_MAX; ch++) {
        ppg_ctrl_cur_state[ch] = PPG_CONTROL_STATE_RESET;
        ppg_ctrl_pre_state[ch] = PPG_CONTROL_STATE_RESET;
        ppg_control_timer[ch] = 0;
        ppg_control_init_flag[ch] = 0;
        ppg_ctrl_cnt_pos_edge[ch] = 0;
        ppg_ctrl_cnt_neg_edge[ch] = 0;

        /* ppg buffer control */
        ppg_ctrl_buf_idx[ch] = 0;
        ppg_ctrl_in_count[ch] = 0;

        for (j = 0; j < PPG_CTRL_BUF_SIZE; j++) {
            ppg_ctrl_buf[ch][j] = 0;
        }
    }

    /* set AFE boundary */
    ppg_dc_limit_h = PPG_DC_MAXP;
    ppg_dc_limit_l = PPG_DC_MAXN;

    /* set time limit */
    ppg_ctrl_time_limit = PPG_CONTROL_TIME_LIMIT;

    /* initial setting */
    ppg_control_led_current[0] = PPG_INIT_LED1;
    ppg_control_led_current[1] = PPG_INIT_LED2;

    /* set driver */
#if defined(PPG_CTRL_DRIVER_ON)
    ppg_ctrl_set_driver_ch(0);
#endif              /* #if defined(PPG_CTRL_DRIVER_ON) */

    return PPG_CONTROL_STATUS_OK;
}

int32_t ppg_control_process(ppg_control_t *ppg_control_input, int32_t ppg_control_mode, int32_t *ppg_control_output)
{
    int32_t *input = ppg_control_input->input;
    int32_t *input_amb = ppg_control_input->input_amb;
    int32_t fs_input = ppg_control_input->input_fs;
    int32_t ppg_sample_length = ppg_control_input->input_length;
    int32_t ch = (ppg_control_input->input_source == 1) ? 0 : 1;

    /* variables */
    int32_t i, j;
    int32_t value = 0;
    int32_t value_ac = 0;
    int32_t down_ratio;

    /* timing control */
    int32_t flag_check_time_limit = 0;
    int32_t led_step = 0;

    /* window value */
    int32_t window_ac_min = PPG_DC_POS_EDGE << 1;
    int32_t window_ac_max = PPG_DC_NEG_EDGE << 1;
    int32_t window_dc_min = PPG_DC_POS_EDGE << 1;
    int32_t window_dc_max = PPG_DC_NEG_EDGE << 1;

    /* reset flag */
    ppg_control_adjust_flag[ch] = 0;

    /* timer update */
    ppg_control_timer[ch] += ppg_sample_length;

#if defined(LOG_PPG_CONTROL_ENABLE)
    LOGI("MT6381_AGC start, timer%d=%d, state=%d, led=%d, idx=%d, fs=%d, l=%d\n", ch,
           ppg_control_timer[ch], ppg_ctrl_cur_state[ch], ppg_control_led_current[ch],
           ppg_ctrl_buf_idx[ch], fs_input, ppg_sample_length);
#endif              /* #if defined(LOG_PPG_CONTROL_ENABLE) */

    /* down sample step size calc. */
    switch (fs_input) {
    case 128:
        down_ratio = 1 << 3;
        break;
    case 256:
        down_ratio = 1 << 4;
        break;
    case 512:
        down_ratio = 1 << 5;
        break;
    default:
        down_ratio = 1;
        break;
    }

    /* buffer control and DC&AC estimation */
    for (i = 0; i < ppg_sample_length; i++) {

        /* Downsampling. PPG DC estimation */
        if (ppg_ctrl_in_count[ch] == 0) {
            /* current input is LED-AMB, value will become LED phase */
            value = input[i] + input_amb[i];

#if defined(LOG_PPG_CONTROL_ENABLE)
            LOGI("MT6381_AGC input, PPG%d = %d, LED = %d, AMB = %d, idx = %d\n", ch,
                   input[i], value, input_amb[i], ppg_ctrl_buf_idx[ch]);
#endif

            /* buffer update */
            ppg_ctrl_buf[ch][ppg_ctrl_buf_idx[ch]] = value;

            /* saturate count */
            if (value > ppg_dc_limit_h)
                ppg_ctrl_cnt_pos_edge[ch]++;
            else if (value < ppg_dc_limit_l)
                ppg_ctrl_cnt_neg_edge[ch]++;
            /* init filter setting */
            if (ppg_control_init_flag[ch] == 0) {
                ppg_control_hpf[ch] = value << PPG_CONTROL_HPF_ORDER;
                ppg_control_init_flag[ch] = 1;
            }
            /* buffer pointer update */
            ppg_ctrl_buf_idx[ch]++;
            if (down_ratio == 1)
                ppg_ctrl_in_count[ch] = 0;
            else
                ppg_ctrl_in_count[ch]++;
        } else if (ppg_ctrl_in_count[ch] >= down_ratio - 1) {
            ppg_ctrl_in_count[ch] = 0;
        } else {
            ppg_ctrl_in_count[ch]++;
        }

        /* check saturate */
        if (ppg_ctrl_cnt_pos_edge[ch] > PPG_SATURATE_HANDLE_COUNT) {
            ppg_ctrl_cur_state[ch] = PPG_CONTROL_STATE_SAT_P;
            ppg_control_adjust_flag[ch] = 1;

        } else if (ppg_ctrl_cnt_neg_edge[ch] > PPG_SATURATE_HANDLE_COUNT) {
            ppg_ctrl_cur_state[ch] = PPG_CONTROL_STATE_SAT_N;
            ppg_control_adjust_flag[ch] = 1;

        } else if (ppg_ctrl_buf_idx[ch] == PPG_CTRL_FS_OPERATE) {
            ppg_ctrl_cur_state[ch] = PPG_CONTROL_STATE_NON_SAT;

            for (j = 0; j < PPG_CTRL_FS_OPERATE; j++) {
                /* find dc max */
                value = ppg_ctrl_buf[ch][j];
                if (value > window_dc_max)
                    window_dc_max = value;
                if (value < window_dc_min)
                    window_dc_min = value;
                /* find ac max */
                ppg_control_hpf[ch] +=
                    ((ppg_ctrl_buf[ch][j] * 4) -
                     ppg_control_hpf[ch]) >> PPG_CONTROL_HPF_ORDER;
                value_ac =
                    ppg_ctrl_buf[ch][j] -
                    (ppg_control_hpf[ch] >> PPG_CONTROL_HPF_ORDER);
                if (value_ac > window_ac_max)
                    window_ac_max = value_ac;
                if (value_ac < window_ac_min)
                    window_ac_min = value_ac;
            }   /* end AC update */
            value_ac = window_ac_max - window_ac_min;

            /* if (window_dc_max < PPG_DC_ENLARGE_BOUND && value_ac < PPG_AC_TARGET) {
             *  ppg_ctrl_cur_state[ch] = PPG_CONTROL_STATE_INC;
             *  ppg_control_adjust_flag[ch] = 1;
             * }    else if(window_dc_min > (PPG_DC_POS_EDGE - (PPG_DC_POS_EDGE>>3))) {
             *   ppg_ctrl_cur_state[ch] = PPG_CONTROL_STATE_DEC;
             *   ppg_control_adjust_flag[ch] = 1;
             *   }
             */
        }
        /* reset */
        if (ppg_control_adjust_flag[ch] == 1 || ppg_ctrl_buf_idx[ch] >= PPG_CTRL_FS_OPERATE) {
            ppg_ctrl_buf_idx[ch] = 0;
            ppg_ctrl_cnt_pos_edge[ch] = 0;
            ppg_ctrl_cnt_neg_edge[ch] = 0;
        }
    }           /* end i: buffer control and state check */

    /* timer check */
    if (ppg_ctrl_time_limit == PPG_CONTROL_ALWAYS_ON
        || (ppg_control_timer[ch] < (ppg_ctrl_time_limit * fs_input))) {
        flag_check_time_limit = 0;
    } else {
        flag_check_time_limit = 1;  /* stop adjustment */
    }

    /* Adjustment stage */
    if (flag_check_time_limit == 0 && ppg_control_adjust_flag[ch] == 1) {

        if (ppg_ctrl_cur_state[ch] == PPG_CONTROL_STATE_SAT_P) {
            /* LED decrease */
            if (ppg_ctrl_pre_state[ch] == PPG_CONTROL_STATE_SAT_N
                || ppg_ctrl_pre_state[ch] == PPG_CONTROL_STATE_INC) {
                led_step = -PPG_LED_STEP_FINE;
            } else {
                led_step = -PPG_LED_STEP_COARSE;
            }
        } else if (ppg_ctrl_cur_state[ch] == PPG_CONTROL_STATE_SAT_N) {
            /* LED increase */
            if (ppg_ctrl_pre_state[ch] == PPG_CONTROL_STATE_SAT_P
                || ppg_ctrl_pre_state[ch] == PPG_CONTROL_STATE_DEC) {
                led_step = PPG_LED_STEP_FINE;
            } else {
                led_step = PPG_LED_STEP_COARSE;
            }
        } else if (ppg_ctrl_cur_state[ch] == PPG_CONTROL_STATE_INC) {
            /* LED increase */
            if (window_dc_max < (PPG_DC_NEG_EDGE >> 2))
                led_step = PPG_LED_STEP_COARSE;
            else if (window_dc_max < (PPG_DC_NEG_EDGE >> 1))
                led_step = PPG_LED_STEP_FINE;
            else
                led_step = PPG_LED_STEP_MIN;
        } else if (ppg_ctrl_cur_state[ch] == PPG_CONTROL_STATE_DEC)
            ;
        /* reset buffers/timers */
        ppg_ctrl_buf_idx[ch] = 0;

        /* check upper bound */
        ppg_control_led_current[ch] += led_step;
        if (ppg_control_led_current[ch] > PPG_MAX_LED_CURRENT)
            ppg_control_led_current[ch] = PPG_MAX_LED_CURRENT;
        else if (ppg_control_led_current[ch] < PPG_MIN_LED_CURRENT)
            ppg_control_led_current[ch] = PPG_MIN_LED_CURRENT;

#if defined(LOG_PPG_CONTROL_ENABLE)
        LOGI("MT6381_AGC write, state%d = %d->%d, led = %d, step = %d\n", ch,
               ppg_ctrl_pre_state[ch], ppg_ctrl_cur_state[ch], ppg_control_led_current[ch],
               led_step);
#endif

#if defined(PPG_CTRL_DRIVER_ON)
        /*if(ch==0) { //PPG1
         * vsm_driver_set_led_current(VSM_LED_1, VSM_SIGNAL_PPG1, ppg_control_led_current[ch]);
         * } else {
         * vsm_driver_set_led_current(VSM_LED_2, VSM_SIGNAL_PPG2, ppg_control_led_current[ch]);
         * }
         */
        ppg_ctrl_set_driver_ch(ch);
        ppg_ctrl_pre_state[ch] = ppg_ctrl_cur_state[ch];
#endif              /* #if defined(PPG_CTRL_DRIVER_ON) */

    }

    return ppg_control_adjust_flag[ch];
}


#if defined(PPG_CTRL_DRIVER_ON)

void ppg_ctrl_set_driver_ch(int32_t ch)
{
    uint32_t ppg_reg_value;
    bus_data_t ppg_reg_info;

    /* write register: LED current (MT6381: 0x332C) */
    ppg_reg_value = ppg_control_led_current[0] + (ppg_control_led_current[1] << 8);
    ppg_reg_info.addr = 0x33;
    ppg_reg_info.reg = 0x2C;
    ppg_reg_info.data_buf = (uint8_t *) &ppg_reg_value;
    ppg_reg_info.length = sizeof(ppg_reg_value);
    vsm_driver_write_register(&ppg_reg_info);
    vsm_driver_update_register();
}

#endif


