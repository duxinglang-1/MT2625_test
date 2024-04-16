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

/* Includes ------------------------------------------------------------------*/
#include "hal_wdt.h"

#if defined(HAL_WDT_MODULE_ENABLED)
#include "hal_wdt_internal.h"
#include "hal_nvic.h"
#include "hal_gpt.h"

/* Global variable */
static hal_wdt_callback_t s_hal_wdt_callback = NULL;

static void hal_wdt_isr(void);

/*****************************************************************************
* Function
*****************************************************************************/
static uint32_t wdt_feed_time_1M;
#ifdef MTK_WDT_TIMEOUT_DEBUG
static uint32_t wdt_feed_time_32k[100];
static uint32_t wdt_feed_idx;
static uint32_t wdt_register_dump[16];
static uint32_t wdt_feed_num;
static uint32_t wdt_write_fail_num;
static void hal_wdt_feed_record(void)
{
     hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &wdt_feed_time_32k[wdt_feed_idx]);
     wdt_feed_idx = (wdt_feed_idx == 99) ? 0 : (wdt_feed_idx + 1);
}

static void hal_wdt_register_record(void)
{
    wdt_register_dump[0] = WDT_REGISTER->WDT_EN;
    wdt_register_dump[1] = WDT_REGISTER->WDT_LENGTH;
    wdt_register_dump[2] = WDT_REGISTER->WDT_INTERVAL;
    wdt_register_dump[3] = WDT_REGISTER->WDT_SW_RESTART;
    wdt_register_dump[4] = WDT_REGISTER->WDT_SW_RST;
    wdt_register_dump[5] = WDT_REGISTER->WDT_AUTO_RESTART_EN;
    wdt_register_dump[6] = WDT_REGISTER->WDT_IE;
    wdt_register_dump[7] = WDT_REGISTER->WDT_INT;
    wdt_register_dump[8] = WDT_REGISTER->WDT_STA;
    wdt_register_dump[9] = WDT_REGISTER->WDT_SW_RSTSYS;
    wdt_register_dump[10] = WDT_REGISTER->WDT_RST0_UNION.WDT_RST0;
    wdt_register_dump[11] = WDT_REGISTER->WDT_RST1_UNION.WDT_RST1;
    wdt_register_dump[12] = WDT_REGISTER->WDT_MASK0_UNION.WDT_MASK0;
    wdt_register_dump[13] = WDT_REGISTER->WDT_MASK1_UNION.WDT_MASK1;
    wdt_register_dump[14] = WDT_REGISTER->WDT_RETN_DAT0;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &wdt_register_dump[15]);
}

static void hal_wdt_check_rgu_register_write(void)
{
   volatile uint32_t temp;
   WDT_REGISTER->WDT_RETN_DAT0 = wdt_feed_num;
   temp = WDT_REGISTER->WDT_RETN_DAT0;
   if(temp != wdt_feed_num)
      wdt_write_fail_num = wdt_feed_num;

   wdt_feed_num = wdt_feed_num + 1;
}
#endif

hal_wdt_status_t hal_wdt_init(hal_wdt_config_t *wdt_config)
{
    /* parameter check */
    if (wdt_config == NULL || (HAL_WDT_MODE_RESET != wdt_config->mode && HAL_WDT_MODE_INTERRUPT != wdt_config->mode)) {
        return HAL_WDT_STATUS_INVALID_PARAMETER;
    }

    if (wdt_config->seconds > WDT_MAX_TIMEOUT_VALUE) {
        return HAL_WDT_STATUS_INVALID_PARAMETER;
    }

    s_hal_wdt_callback = NULL;

    /*set WDT length*/
    /* WDT_SW_RESTART may be set in wdt_set_length/wdt_set_restart.
           delay 150us is to make sure WDT_SW_RESTART won't be set in 4T*32k */
    hal_gpt_delay_us(150);
    wdt_set_length(wdt_config->seconds);
    hal_gpt_delay_us(150);

    /* set WDT mode */
    wdt_set_mode(wdt_config->mode);

    /* User can not call hal_wdt_register_callback */
    if (HAL_WDT_MODE_INTERRUPT == wdt_config->mode) {

        /* register NVIC */
        hal_nvic_register_isr_handler(RGU_IRQn, (hal_nvic_isr_t)hal_wdt_isr);
        NVIC_EnableIRQ(RGU_IRQn);
    }

    /* WDT always did cold reset if DVFS feature is enabled */
#ifdef HAL_DVFS_MODULE_ENABLED
    wdt_set_pmu_mask(false);
#endif

    return HAL_WDT_STATUS_OK;
}

void hal_wdt_reinit_length(uint32_t second)
{
    hal_gpt_delay_us(200);
    wdt_set_length(second);
    hal_gpt_delay_us(200);
}
hal_wdt_status_t hal_wdt_deinit(void)
{
    /* disable wdt */
    wdt_set_enable(false);

    s_hal_wdt_callback = NULL;

    return HAL_WDT_STATUS_OK;
}

hal_wdt_callback_t hal_wdt_register_callback(hal_wdt_callback_t wdt_callback)
{
    hal_wdt_callback_t current_wdt_callback;
    /* no need error check. allow null register */
    current_wdt_callback = s_hal_wdt_callback;

    s_hal_wdt_callback = wdt_callback;

    return current_wdt_callback;
}

hal_wdt_status_t hal_wdt_feed(uint32_t magic)
{
    uint32_t current_1M_count;
    uint32_t mask;
    if (HAL_WDT_FEED_MAGIC != magic) {
        return HAL_WDT_STATUS_INVALID_MAGIC;
    }

    hal_nvic_save_and_set_interrupt_mask(&mask);
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &current_1M_count);
    if((current_1M_count - wdt_feed_time_1M) > 200)
    {
        #ifdef MTK_WDT_TIMEOUT_DEBUG
        /* check write RGU region before feed watchdog*/
        hal_wdt_check_rgu_register_write();
        #endif
        /* write wdt restart register */        
        wdt_set_restart();
        wdt_feed_time_1M = current_1M_count;
        hal_nvic_restore_interrupt_mask(mask);
        #ifdef MTK_WDT_TIMEOUT_DEBUG
        hal_wdt_feed_record();
        /* check write RGU region after feed watchdog*/
        hal_wdt_check_rgu_register_write();
        #endif
    }
    else
    {
       hal_nvic_restore_interrupt_mask(mask);
    }
    return HAL_WDT_STATUS_OK;
}

hal_wdt_status_t hal_wdt_software_reset(void)
{
    hal_wdt_disable(HAL_WDT_DISABLE_MAGIC);
    wdt_set_sw_rst();
    return HAL_WDT_STATUS_OK;
}

hal_wdt_status_t hal_wdt_enable(uint32_t magic)
{
    if (HAL_WDT_ENABLE_MAGIC != magic) {
        return HAL_WDT_STATUS_INVALID_MAGIC;
    }

    wdt_set_enable(true);
    /* restart wdt */
    hal_wdt_feed(HAL_WDT_FEED_MAGIC);

    return HAL_WDT_STATUS_OK;
}

hal_wdt_status_t hal_wdt_disable(uint32_t magic)
{
    if (HAL_WDT_DISABLE_MAGIC != magic) {
        return HAL_WDT_STATUS_INVALID_MAGIC;
    }

    wdt_set_enable(false);

    return HAL_WDT_STATUS_OK;
}

hal_wdt_reset_status_t hal_wdt_get_reset_status(void)
{
    return ((hal_wdt_reset_status_t)wdt_get_reset_status());
}

bool hal_wdt_get_enable_status(void)
{
    return (wdt_get_enable_status());
}

hal_wdt_mode_t hal_wdt_get_mode(void)
{
    return ((hal_wdt_mode_t)wdt_get_mode_status());
}

static void hal_wdt_isr(void)
{
    hal_wdt_reset_status_t wdt_reset_status;

    wdt_reset_status = (hal_wdt_reset_status_t)wdt_get_reset_status();
#ifdef MTK_WDT_TIMEOUT_DEBUG
    hal_wdt_register_record();
#endif
    if (NULL != s_hal_wdt_callback) {
        s_hal_wdt_callback(wdt_reset_status);
    }
    /* clear WDT IRQ */
    wdt_clear_irq();
}

#endif
