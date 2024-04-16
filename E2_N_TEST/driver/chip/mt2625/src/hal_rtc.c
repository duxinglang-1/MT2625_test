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

#include "hal_rtc.h"

#if defined(HAL_RTC_MODULE_ENABLED)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hal_rtc_external.h"
#include "hal_rtc_internal.h"
#include "hal_eint.h"
#include "hal_log.h"
#include "hal_gpt.h"
#include "hal_platform.h"
#include "hal_pmu.h"
#include "hal_nvic.h"
#include "hal_nvic_internal.h"
#include "memory_attribute.h"
#ifdef HAL_WDT_MODULE_ENABLED
#include "hal_wdt.h"
#endif

#ifdef __UBL__
#include "bl_common.h"
#endif

extern bool chip_is_mt2625E3(void);

#define DRV_Reg32(addr) (*(volatile uint32_t*)(addr))
#define DRV_WriteReg32(addr, data) ((*(volatile uint32_t *)(addr)) = (uint32_t)(data))
#define CHANGE_CALENDAR_DEVIATION 2

static void rtc_init_eint(void);
static void rtc_handle_irq_sta(void);
static void rtc_system_boot_log(void);
static hal_rtc_status_t rtc_get_sleep_tick(bool is_after_sleep);
static void rtc_unlock_PROT(void);
static void rtc_write_trigger_wait(void);
static void rtc_reload(void);
static uint32_t rtc_read_osc32con0(void);
static void rtc_write_osc32con0(uint32_t value, uint32_t operation_type);
static uint32_t rtc_read_osc32con1(void);
static void rtc_write_osc32con1(uint32_t value);
static uint32_t rtc_read_osc32con2(void);
static void rtc_write_osc32con2(uint32_t value, uint32_t operation_type);
static void rtc_lpd_init(void);
static void rtc_set_power_key(void);
static void rtc_set_xosc_mode(void);
static void rtc_register_init(void);
static void rtc_dump_register(char *tag);
//static void rtc_check_alarm_power_on(void);
void rtc_xosc_cali(void);
static void f32k_eosc32_calibration(void);
static void f32k_osc32_Init(void);
uint32_t f32k_measure_clock(uint32_t fixed_clock, uint32_t tested_clock, uint32_t window_setting);
#ifdef HAL_RTC_FEATURE_ELAPSED_TICK
static void rtc_power_on_status_analysis();
#endif
void rtc_register_shut_down_callback(f_rtc_callback_t cb);
void rtc_trigger_shut_down_callback(void);

#ifdef HAL_RTC_FEATURE_TIME_CALLBACK
ATTR_ZIDATA_IN_RETSRAM static hal_rtc_time_callback_t rtc_time_callback_function;
ATTR_ZIDATA_IN_RETSRAM static void *rtc_time_user_data;
#endif

static hal_rtc_alarm_callback_t rtc_alarm_callback_function;
static void *rtc_alarm_user_data;
#ifdef HAL_RTC_FEATURE_ELAPSED_TICK
static hal_rtc_alarm_callback_t rtc_md_alarm_callback_function;
static void *rtc_md_alarm_user_data;
#endif

ATTR_ZIDATA_IN_RETSRAM static hal_rtc_eint_callback_t rtc_eint_callback_function;
ATTR_ZIDATA_IN_RETSRAM static void *rtc_eint_user_data;

static bool rtc_in_test = false;
static char rtc_spare_register_backup[HAL_RTC_BACKUP_BYTE_NUM_MAX];
bool retention_flag = false;
bool rtc_mode_flag = false;
static rtc_cali_ctr_t rtc_cali_ctr = RTC_CALI_CTR_LOCK;
static uint32_t rtc_irq_sta;
char system_boot_log[60];
static f_rtc_callback_t g_rtc_callback = NULL;

typedef struct {
    bool change_calendar;
    hal_rtc_time_t sleep_time;
    hal_rtc_time_t pre_calendar_time;
    hal_rtc_time_t cur_calendar_time;
}rtc_time_modem_t;
ATTR_ZIDATA_IN_RETSRAM volatile rtc_time_modem_t rtc_time_modem;

RTC_REGISTER_T *rtc_register = (RTC_REGISTER_T *)RTC_BASE;
ABIST_FQMTR_REGISTER_T *abist_fqmtr_register = (ABIST_FQMTR_REGISTER_T *)ABIST_FQMTR_BASE;

struct rtc_spare_register_information {
    uint8_t *address;
};
/* Spar2 to Spar5 are reserved for MD usage. */
static struct rtc_spare_register_information rtc_spare_register_table[HAL_RTC_BACKUP_BYTE_NUM_MAX] = {
    {(uint8_t *) &(((RTC_REGISTER_T *)RTC_BASE)->RTC_SPAR0_UNION.RTC_SPAR0_CELLS.RTC_SPAR0_0)},
    {(uint8_t *) &(((RTC_REGISTER_T *)RTC_BASE)->RTC_SPAR0_UNION.RTC_SPAR0_CELLS.RTC_SPAR0_1)},
    {(uint8_t *) &(((RTC_REGISTER_T *)RTC_BASE)->RTC_SPAR1_UNION.RTC_SPAR1_CELLS.RTC_SPAR1_0)},
    {(uint8_t *) &(((RTC_REGISTER_T *)RTC_BASE)->RTC_SPAR1_UNION.RTC_SPAR1_CELLS.RTC_SPAR1_1)}
};

#define FQMTR_FCKSEL_RTC_32K            1
#define FQMTR_FCKSEL_EOSC_F32K_CK       4
#define FQMTR_FCKSEL_DCXO_F32K_CK       5
#define FQMTR_FCKSEL_XOSC_F32K_CK       6

#define FQMTR_TCKSEL_XO_CK      1

#define NUMBER_OF_POLLING_32K     10

ATTR_TEXT_IN_TCM static bool rtc_judge_32K_alignment(void)
{
    uint16_t int_cnt, int_cnt_pre;
    uint16_t count = 0;
    
    int_cnt_pre = (uint16_t)(rtc_register->RTC_INT_CNT);
    int_cnt_pre &= RTC_INT_CNT_MASK;
    
    do {
        count++;
        int_cnt = (uint16_t)(rtc_register->RTC_INT_CNT);
        int_cnt &= RTC_INT_CNT_MASK;
    } while ((int_cnt == int_cnt_pre) && (count < NUMBER_OF_POLLING_32K));

    if(int_cnt == int_cnt_pre) {
        return false;
    } else {
        return true;
    }
}

ATTR_TEXT_IN_TCM bool rtc_query_cbusy(void)
{
    return (bool)(RTC_CBUSY_MASK & rtc_register->RTC_WRTGR_UNION.RTC_WRTGR_CELLS.RTC_STA);
}

void rtc_wait_busy(void)
{
    uint32_t count = 0;

    while (count < 0x6EEEEE) {
        if (rtc_query_cbusy() == false) {
            break;
        }
        count++;
    }

    if (count >= 0x6EEEEE) {
        RTC_LOG_ERROR("rtc_wait_busy timeout, RTC_BBPU = %x!\r\n", rtc_register->RTC_BBPU);
        RTC_LOG_ERROR("rtc_wait_busy timeout, RTC_32K = %u, EOSC = %u, DCXO = %u, XOSC = %u\r\n",
                      (unsigned int)f32k_measure_clock(FQMTR_FCKSEL_RTC_32K, FQMTR_TCKSEL_XO_CK, 99),
                      (unsigned int)f32k_measure_clock(FQMTR_FCKSEL_EOSC_F32K_CK, FQMTR_TCKSEL_XO_CK, 99),
                      (unsigned int)f32k_measure_clock(FQMTR_FCKSEL_DCXO_F32K_CK, FQMTR_TCKSEL_XO_CK, 99),
                      (unsigned int)f32k_measure_clock(FQMTR_FCKSEL_XOSC_F32K_CK, FQMTR_TCKSEL_XO_CK, 99));
    }
}

static void rtc_unlock_PROT(void)
{
    rtc_register->RTC_PROT = RTC_PROTECT1;
    rtc_write_trigger_wait();

    rtc_register->RTC_PROT = RTC_PROTECT2;
    rtc_write_trigger_wait();
}

ATTR_TEXT_IN_TCM void rtc_write_trigger(void)
{
    rtc_register->RTC_WRTGR_UNION.RTC_WRTGR_CELLS.WRTGR = RTC_WRTGR_MASK;
}

static void rtc_write_trigger_wait(void)
{
    rtc_write_trigger();
    rtc_wait_busy();
}

static void rtc_reload(void)
{
    uint32_t mask;
    
    mask = save_and_set_interrupt_mask();
    rtc_register->RTC_BBPU = RTC_KEY_BBPU_RELOAD;
    
    rtc_write_trigger();
    restore_interrupt_mask(mask);
    rtc_wait_busy();
}

#if RTC_LOG_DEBUG
static uint32_t rtc_read_osc32con0(void)
{
    rtc_reload();
    return rtc_register->RTC_OSC32CON0_UNION.RTC_OSC32CON0;
}
#endif
static void rtc_write_osc32con0(uint32_t value, uint32_t operation_type)
{
    rtc_register->RTC_OSC32CON0_UNION.RTC_OSC32CON0 = RTC_OSC32CON0_MAGIC_KEY_1;
    rtc_wait_busy();
    rtc_register->RTC_OSC32CON0_UNION.RTC_OSC32CON0 = RTC_OSC32CON0_MAGIC_KEY_2;
    rtc_wait_busy();
    switch(operation_type) {
        case RTC_LOW_BYTE_OPERATION:
            rtc_register->RTC_OSC32CON0_UNION.RTC_OSC32CON0_CELLS.RTC_32K_SEL = value;
            break;
        case RTC_HIGH_BYTE_OPERATION:
            rtc_register->RTC_OSC32CON0_UNION.RTC_OSC32CON0_CELLS.RTC_TIMER_CG_EN = value;
            break;
        case RTC_WORD_OPERATION:
            rtc_register->RTC_OSC32CON0_UNION.RTC_OSC32CON0 = value;
            break;
        default:
            break;
    }
    rtc_wait_busy();
}

static uint32_t rtc_read_osc32con1(void)
{
    rtc_reload();
    return rtc_register->RTC_OSC32CON1;
}

static void rtc_write_osc32con1(uint32_t value)
{
    rtc_register->RTC_OSC32CON1 = RTC_OSC32CON1_MAGIC_KEY_1;
    rtc_wait_busy();
    rtc_register->RTC_OSC32CON1 = RTC_OSC32CON1_MAGIC_KEY_2;
    rtc_wait_busy();
    rtc_register->RTC_OSC32CON1 = value;
    rtc_wait_busy();
}

static uint32_t rtc_read_osc32con2(void)
{
    rtc_reload();
    return rtc_register->RTC_OSC32CON2_UNION.RTC_OSC32CON2;
}

static void rtc_write_osc32con2(uint32_t value, uint32_t operation_type)
{
    rtc_register->RTC_OSC32CON2_UNION.RTC_OSC32CON2 = RTC_OSC32CON2_MAGIC_KEY_1;
    rtc_wait_busy();
    rtc_register->RTC_OSC32CON2_UNION.RTC_OSC32CON2 = RTC_OSC32CON2_MAGIC_KEY_2;
    rtc_wait_busy();
    switch(operation_type) {
        case RTC_LOW_BYTE_OPERATION:
            rtc_register->RTC_OSC32CON2_UNION.RTC_OSC32CON2_CELLS.OSC32_PW_DB = value;
            break;
        case RTC_HIGH_BYTE_OPERATION:
            rtc_register->RTC_OSC32CON2_UNION.RTC_OSC32CON2_CELLS.SRAM_ISO_EN = value;
            break;
        case RTC_WORD_OPERATION:
            rtc_register->RTC_OSC32CON2_UNION.RTC_OSC32CON2 = value;
            break;
        default:
            break;
    }
    rtc_wait_busy();
}

static uint32_t rtc_read_sramcon(void)
{
    rtc_reload();
    return rtc_register->RTC_SRAM_CON_UNION.RTC_SRAM_CON;
}

static void rtc_write_sramcon(uint32_t value, uint32_t operation_type)
{
    rtc_register->RTC_SRAM_CON_UNION.RTC_SRAM_CON = RTC_SRAMCON_MAGIC_KEY_1;
    rtc_wait_busy();
    rtc_register->RTC_SRAM_CON_UNION.RTC_SRAM_CON = RTC_SRAMCON_MAGIC_KEY_2;
    rtc_wait_busy();
    switch(operation_type) {
        case RTC_LOW_BYTE_OPERATION:
            rtc_register->RTC_SRAM_CON_UNION.RTC_SRAM_CON_CELLS.SRAM_PD = value;
            break;
        case RTC_HIGH_BYTE_OPERATION:
            rtc_register->RTC_SRAM_CON_UNION.RTC_SRAM_CON_CELLS.SRAM_SLEEPB = value;
            break;
        case RTC_WORD_OPERATION:
            rtc_register->RTC_SRAM_CON_UNION.RTC_SRAM_CON = value;
            break;
        default:
            break;
    }
    rtc_wait_busy();
}
static void rtc_lpd_init(void)
{
    rtc_register->RTC_LPD_CON_UNION.RTC_LPD_CON_CELLS.LPD_CON = RTC_EOSC32_LPEN_MASK;
    rtc_write_trigger_wait();

    rtc_register->RTC_LPD_CON_UNION.RTC_LPD_CON_CELLS.LPD_CON = RTC_EOSC32_LPEN_MASK | RTC_LPRST_MASK;
    rtc_write_trigger_wait();

    /* designer suggests delay at least 1 ms */
    hal_gpt_delay_us(1000);

    rtc_register->RTC_LPD_CON_UNION.RTC_LPD_CON_CELLS.LPD_CON = RTC_EOSC32_LPEN_MASK;
    rtc_write_trigger_wait();

    if ((rtc_register->RTC_LPD_CON_UNION.RTC_LPD_CON & RTC_LPSTA_RAW_MASK) != 0) {
        RTC_LOG_ERROR("rtc_lpd_init fail : RTC_LPD_CON = %x!\r\n", rtc_register->RTC_LPD_CON_UNION.RTC_LPD_CON);
        rtc_dump_register("rtc_lpd_init fail\r\n");
    }
}
static void rtc_set_power_key(void)
{
    rtc_dump_register("Before Set Power Key");
    /* Set powerkey0 and powerkey1 */
    rtc_register->RTC_POWERKEY0 = RTC_POWERKEY0_KEY;
    rtc_register->RTC_POWERKEY1 = RTC_POWERKEY1_KEY;
    rtc_write_trigger_wait();
    /* delay for 200us for clock switch */
    hal_gpt_delay_us(200);

    /* Inicialize LPD */
    rtc_lpd_init();

    /* Set powerkey0 and powerkey1 */
    rtc_register->RTC_POWERKEY0 = RTC_POWERKEY0_KEY;
    rtc_register->RTC_POWERKEY1 = RTC_POWERKEY1_KEY;
    rtc_write_trigger_wait();
    /* delay for 200us for clock switch */
    hal_gpt_delay_us(200);

    rtc_reload();
    if ((rtc_register->RTC_WRTGR_UNION.RTC_WRTGR_CELLS.RTC_STA & RTC_POWER_DETECTED_MASK) == 0) {
        RTC_LOG_ERROR("rtc_set_power_key fail : rtc_wrtgr = %x!\r\n", rtc_register->RTC_WRTGR_UNION.RTC_WRTGR);
    }

    rtc_dump_register("After Set Power Key");
}

static void rtc_set_xosc_mode(void)
{
    uint32_t value;
    // XOSC init setting: OSC32CON1[6:5] = 0x0 -> 0x3
    value = rtc_read_osc32con1();
    value |= (RTC_AMP_GSEL_MASK | RTC_AMPCTL_EN_MASK);
    rtc_write_osc32con1(value);
}
extern void dbg_print(char *fmt,...);

bool rtc_is_time_valid(const hal_rtc_time_t *time)
{
    bool result = true;

    if (time->rtc_year > 127) {
        dbg_print("Invalid year : %d\r\n", time->rtc_year);
        result = false;
    }

    if ((time->rtc_mon == 0) || (time->rtc_mon > 12)) {
        dbg_print("Invalid month : %d\r\n", time->rtc_mon);
        result = false;
    }

    if (time->rtc_week > 6) {
        dbg_print("Invalid day of week : %d\r\n", time->rtc_week);
        result = false;
    }

    if ((time->rtc_day == 0) || (time->rtc_day > 31)) {
        dbg_print("Invalid day of month : %d\r\n", time->rtc_day);
        result = false;
    }

    if (time->rtc_hour > 23) {
        dbg_print("Invalid hour : %d\r\n", time->rtc_hour);
        result = false;
    }

    if (time->rtc_min > 59) {
        dbg_print("Invalid minute : %d\r\n", time->rtc_min);
        result = false;
    }

    if (time->rtc_sec > 59) {
        dbg_print("Invalid second : %d\r\n", time->rtc_sec);
        result = false;
    }

    return result;
}

static void rtc_isr(hal_nvic_irq_t irq_number)
{
    if(rtc_irq_sta & RTC_TCSTA_MASK)
    {
        if (rtc_time_callback_function != NULL) {
            rtc_time_callback_function(rtc_time_user_data);
        }
    }

    if(rtc_irq_sta & RTC_EINTSTA_MASK)
    {
        if (rtc_eint_callback_function != NULL) {
            rtc_eint_callback_function(rtc_eint_user_data);
        }
    }

    NVIC_DisableIRQ(RTC_IRQn);
}

static void rtc_handle_irq_sta(void)
{
    /* read clear interrupt status */
    rtc_irq_sta = rtc_register->RTC_IRQ_STA;

    if(rtc_irq_sta & (RTC_TCSTA_MASK | RTC_EINTSTA_MASK))
    {
        hal_nvic_register_isr_handler( RTC_IRQn, (hal_nvic_isr_t)rtc_isr);
        NVIC_SetPriority(RTC_IRQn, EINT_PRIORITY);
        NVIC_EnableIRQ(RTC_IRQn);
        hal_nvic_set_pending_irq(RTC_IRQn);
    }
}

static void rtc_eint_callback(void *user_data)
{
    uint32_t irq_sta;
    uint32_t ap_md_alarm_value;
    uint32_t mask;

    hal_eint_mask(HAL_EINT_NUMBER_28);
    /* read clear interrupt status */
    irq_sta = rtc_register->RTC_IRQ_STA;
    ap_md_alarm_value = rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG;
    RTC_LOG_INFO("[rtc eint callback]irq_sta = %x\r\n", irq_sta);
    RTC_LOG_INFO("[rtc eint callback]ap_md_alarm_value = %x\r\n", ap_md_alarm_value);

     /* clear alarm power on */
    mask = save_and_set_interrupt_mask();
    rtc_register->RTC_BBPU = RTC_KEY_BBPU_1 | RTC_RTC_PU_MASK;
    rtc_write_trigger();
    restore_interrupt_mask(mask);
    rtc_wait_busy();

    if ((irq_sta & RTC_ALSTA_MASK) != 0) {
#ifdef HAL_RTC_FEATURE_ELAPSED_TICK
        if ((ap_md_alarm_value & RTC_MD_ALARM_MASK) != 0) {
            RTC_LOG_INFO("[rtc eint callback]MD alarm happened\r\n");
            
#ifdef HAL_RTC_FEATURE_SW_TIMER
            rtc_sw_alarm_isr();
#endif            
            /*MD alarm happen*/
            /* Can't disable MD alarm interrupt here, since AP AL_EN is connect with MD_EN */
            //rtc_register->RTC_MD_AL_MASK_UNION.RTC_MD_AL_MASK_CELLS.MD_AL_EN = 0;
            mask = save_and_set_interrupt_mask();
            rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG &= (~RTC_MD_ALARM_MASK);
            rtc_write_trigger();
            restore_interrupt_mask(mask);
            rtc_wait_busy();
            
            if (rtc_md_alarm_callback_function != NULL) {
                rtc_md_alarm_callback_function(rtc_md_alarm_user_data);
            }
        }
#endif
        if ((ap_md_alarm_value & RTC_AP_ALARM_MASK) != 0) {
            RTC_LOG_INFO("[rtc eint callback]AP alarm happened\r\n");
            
#ifdef HAL_RTC_FEATURE_SW_TIMER
            rtc_sw_timer_isr();
#endif
            /*AP alarm happen*/
            /* Can't disable AP alarm interrupt here, , since AP AL_EN is connect with MD_EN */
            //rtc_register->RTC_AL_MASK_UNION.RTC_AL_MASK_CELLS.AL_EN = 0;
            mask = save_and_set_interrupt_mask();
            rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG &= (~RTC_AP_ALARM_MASK);
            rtc_write_trigger();
            restore_interrupt_mask(mask);
            rtc_wait_busy();
            
            if (rtc_alarm_callback_function != NULL) {
                rtc_alarm_callback_function(rtc_alarm_user_data);
            }
        }
    }

#ifdef HAL_RTC_FEATURE_TIME_CALLBACK
    if ((irq_sta & RTC_TCSTA_MASK) != 0) {
        RTC_LOG_INFO("[rtc eint callback]TC alarm happened\r\n");
        if (rtc_time_callback_function != NULL) {
            rtc_time_callback_function(rtc_time_user_data);
        }
    }
#endif

    if ((irq_sta & RTC_EINTSTA_MASK) != 0) {
        RTC_LOG_INFO("[rtc eint callback]RTC EINT happened\r\n");
        if (rtc_eint_callback_function != NULL) {
            rtc_eint_callback_function(rtc_eint_user_data);
        }
    }

    hal_eint_unmask(HAL_EINT_NUMBER_28);

}

static void rtc_init_eint(void)
{
    hal_eint_config_t eint_config;
    hal_eint_status_t result;
    hal_rtc_eint_config_t rtc_eint_config;

    eint_config.trigger_mode = HAL_EINT_LEVEL_LOW;
    eint_config.debounce_time = 0;
#ifdef HAL_EINT_FEATURE_FIRQ
    eint_config.firq_enable = false;
#endif   
    result = hal_eint_init(HAL_EINT_NUMBER_28, &eint_config);
    if (result != HAL_EINT_STATUS_OK) {
        log_hal_error("hal_eint_init fail: %d", result);
        return;
    }

    result = hal_eint_register_callback(HAL_EINT_NUMBER_28, rtc_eint_callback, NULL);
    if (result != HAL_EINT_STATUS_OK) {
        log_hal_error("hal_eint_register_callback fail: %d", result);
        return;
    }

    result = hal_eint_unmask(HAL_EINT_NUMBER_28);
    if (result != HAL_EINT_STATUS_OK) {
        log_hal_error("hal_eint_unmask fail: %d", result);
        return;
    }

    rtc_eint_config.is_enable_rtc_eint = true;
    rtc_eint_config.is_falling_edge_active = true;
    rtc_eint_config.is_enable_debounce = true;
    hal_rtc_eint_init(&rtc_eint_config);

}

static void rtc_system_boot_log(void)
{
    uint32_t boot_reason;
    char tmp_str[50];
    char trace_buffer[50];
    boot_reason = rtc_power_on_result();

    switch (boot_reason)
    {
        case 0:
            snprintf(tmp_str, 50, "1st boot up");
            break;
        case 1:
            snprintf(tmp_str, 50, "back from deep sleep");
            break;
        case 2:
            snprintf(tmp_str, 50, "back from deeper sleep");
            break;
        case 3:
            snprintf(tmp_str, 50, "back from long press shutdown or sys_reset");
            break;
        case 4:
            snprintf(tmp_str, 50, "WDT hw reset happened");
            break;
        case 5:
            snprintf(tmp_str, 50, "WDT sw reset happened");
            break;
        case 6:
            snprintf(tmp_str, 50, "back from forced shut down");
            break;
        case 7:
            snprintf(tmp_str, 50, "back from forced reset");
            break;
        default:
            snprintf(tmp_str, 50, "unknown");
            break;
    }

    if((boot_reason == 1) || (boot_reason == 2))
    {
        if(rtc_irq_sta & RTC_TCSTA_MASK)
        {
            snprintf(trace_buffer, 50, "%s, rtc tc wake up", tmp_str);
        }
        else if(rtc_irq_sta & RTC_EINTSTA_MASK)
        {
            snprintf(trace_buffer, 50, "%s, rtc eint wake up", tmp_str);
        }
        else if(rtc_irq_sta & RTC_ALSTA_MASK)
        {
            snprintf(trace_buffer, 50, "%s, rtc alarm wake up", tmp_str);
        }
        else
        {
            snprintf(trace_buffer, 50, "%s, powerkey wake up", tmp_str);
        }
    }
    else
    {
        memcpy(trace_buffer, tmp_str, 50);
    }
    
    snprintf(system_boot_log, 60, "System boot: %s", trace_buffer);
}

    
void rtc_init_phase_2(void)
{
    rtc_handle_irq_sta();
    rtc_system_boot_log();
    rtc_init_eint();
}

static void rtc_register_init(void)
{
    uint32_t rtc_irq_sta;
    /* Clear ALARM_PU */
    rtc_register->RTC_BBPU = RTC_KEY_BBPU_1 | RTC_RTC_PU_MASK;
    /* Read clear */
    rtc_irq_sta = rtc_register->RTC_IRQ_STA;
    rtc_irq_sta = rtc_irq_sta;
    rtc_register->RTC_IRQ_EN_UNION.RTC_IRQ_EN = 0x0;
    rtc_register->RTC_CII_EN = 0x0;
    rtc_register->RTC_AL_MASK_UNION.RTC_AL_MASK = 0x0;
    rtc_register->RTC_TC0_UNION.RTC_TC0 = 0x0;
    rtc_register->RTC_TC1_UNION.RTC_TC1 = 0x100;
    rtc_register->RTC_TC2_UNION.RTC_TC2 = 0x101;
    rtc_register->RTC_TC3 = 0x0;
    rtc_register->RTC_AL0_UNION.RTC_AL0 = 0x0;
    rtc_register->RTC_AL1_UNION.RTC_AL1 = 0x100;
    rtc_register->RTC_AL2_UNION.RTC_AL2 = 0x101;
    rtc_register->RTC_AL3_UNION.RTC_AL3 = 0x0;
    rtc_register->RTC_LPD_CON_UNION.RTC_LPD_CON = 0x101;
    rtc_register->RTC_EINT_UNION.RTC_EINT = 0x178a;

    rtc_write_trigger_wait();
    rtc_write_osc32con0(0x101, RTC_WORD_OPERATION);
    if (true == chip_is_mt2625E3()) {
        rtc_write_osc32con1(0xf0e);
    } else {
        rtc_write_osc32con1(0xf07);
    }
    rtc_write_osc32con2(0x3f03, RTC_WORD_OPERATION);
    rtc_write_sramcon(0xf0f, RTC_WORD_OPERATION);

    rtc_register->RTC_SPAR0_UNION.RTC_SPAR0 = 0x0;
    rtc_register->RTC_SPAR1_UNION.RTC_SPAR1 = 0x0;
    rtc_register->RTC_SPAR2_UNION.RTC_SPAR2 = 0x0;
    rtc_register->RTC_SPAR3_UNION.RTC_SPAR3 = 0x0;
    rtc_register->RTC_SPAR4_UNION.RTC_SPAR4 = 0x0;
    rtc_register->RTC_SPAR5_UNION.RTC_SPAR5 = 0x0;
    /* Power on reason and CG control should not be cleared */
    rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG &= (RTC_SPAR_POWERON_REASON_MASK | RTC_SPAR_CG_CONTROL_MASK);
    rtc_register->RTC_DIFF = 0x0;
    rtc_register->RTC_CALI = 0x0;
    rtc_register->RTC_GPIO_CON0_UNION.RTC_GPIO_CON0 = 0xCACA;
    rtc_register->RTC_GPIO_CON1_UNION.RTC_GPIO_CON1 = 0xB600;
    rtc_register->RTC_MD_AL_MASK_UNION.RTC_MD_AL_MASK = 0x0;
    rtc_register->RTC_MD_AL0_UNION.RTC_MD_AL0 = 0x0;
    rtc_register->RTC_MD_AL1_UNION.RTC_MD_AL1 = 0x100;
    rtc_register->RTC_MD_AL2_UNION.RTC_MD_AL2 = 0x101;
    rtc_register->RTC_MD_AL3_UNION.RTC_MD_AL3 = 0x0;
    rtc_write_trigger_wait();
}

static void rtc_dump_register(char *tag)
{
#if RTC_LOG_DEBUG
    uint32_t value_key1, value_key2, value_diff, value_osc32con0, value_osc32con1, value_osc32con2;
    uint32_t value_bbpu, value_lpd, value_wrtgr, value_sramcon, value_cali, value_spar;
    rtc_reload();
    value_key1 = rtc_register->RTC_POWERKEY0;
    value_key2 = rtc_register->RTC_POWERKEY1;
    value_diff = rtc_register->RTC_DIFF;
    value_osc32con0 = rtc_read_osc32con0();
    value_osc32con1 = rtc_read_osc32con1();
    value_osc32con2 = rtc_read_osc32con2();
    value_sramcon = rtc_read_sramcon();
    value_bbpu = rtc_register->RTC_BBPU;
    value_lpd = rtc_register->RTC_LPD_CON_UNION.RTC_LPD_CON;
    value_wrtgr = rtc_register->RTC_WRTGR_UNION.RTC_WRTGR;
    value_cali = rtc_register->RTC_CALI;
    value_spar = rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG;
    RTC_LOG_WARNING("rtc_dump_register[%s], RTC_POWERKEY1 = %x, RTC_POWERKEY2 = %x, RTC_DIFF = %x, RTC_OSC32CON0 = %x\r\n",
                    tag, value_key1, value_key2, value_diff, value_osc32con0);
    RTC_LOG_WARNING("RTC_BBPU = %x, RTC_LPD_CON = %x, RTC_CON32CON1 = %x, RTC_CON32CON2 = %x, RTC_WRTGR = %x, SRAMCON = %x, CALI = %x, SPAR = %x\r\n",
                    value_bbpu, value_lpd, value_osc32con1, value_osc32con2, value_wrtgr, value_sramcon, value_cali, value_spar);
#endif
}
#if 0
static void rtc_check_alarm_power_on(void)
{
    hal_rtc_time_t time;

    if ((rtc_register->RTC_BBPU & RTC_RTC_PU_MASK) != 0) {
        hal_rtc_get_time(&time);
        RTC_LOG_WARNING("time : %d/%d/%d %d:%d:%d (%d)\r\n", time.rtc_year, time.rtc_mon, time.rtc_day,
                        time.rtc_hour, time.rtc_min, time.rtc_sec, (time.rtc_milli_sec & 0x7fff));
        hal_rtc_get_alarm(&time);
        RTC_LOG_WARNING("alarm : %d/%d/%d %d:%d:%d (%d)\r\n", time.rtc_year, time.rtc_mon, time.rtc_day,
                        time.rtc_hour, time.rtc_min, time.rtc_sec, time.rtc_milli_sec);
        RTC_LOG_WARNING("Alarm power on, %x\r\n", rtc_register->RTC_BBPU);
    }
}
#endif
static void f32k_eosc32_calibration(void)
{
    uint32_t value;

    uint32_t low_xosccali = 0x00;
    uint32_t high_xosccali = 0x1f;
    uint32_t medium_xosccali;

    uint32_t low_frequency = 0;
    uint32_t high_frequency = 0;
    uint32_t medium_frequency;

    value = rtc_read_osc32con1();
    value &= ~RTC_XOSCCALI_MASK;
    value |= (low_xosccali << RTC_XOSCCALI_OFFSET);
    rtc_write_osc32con1(value);
    high_frequency = f32k_measure_clock(FQMTR_FCKSEL_EOSC_F32K_CK, FQMTR_TCKSEL_XO_CK, 99);
    if (high_frequency <= 32768) {
        RTC_LOG_INFO("high_frequency <= 32768, frequency = %u, xosccali = %d\r\n", (unsigned int)high_frequency, (unsigned int)low_xosccali);
        return;
    }

    value = rtc_read_osc32con1();
    value &= ~RTC_XOSCCALI_MASK;
    value |= (high_xosccali << RTC_XOSCCALI_OFFSET);
    rtc_write_osc32con1(value);
    low_frequency = f32k_measure_clock(FQMTR_FCKSEL_EOSC_F32K_CK, FQMTR_TCKSEL_XO_CK, 99);
    if (low_frequency >= 32768) {
        RTC_LOG_INFO("low_frequency >= 32768, frequency = %u, xosccali = %d\r\n", (unsigned int)low_frequency, (unsigned int)high_xosccali);
        return;
    }

    while ((high_xosccali - low_xosccali) > 1) {
        medium_xosccali = (low_xosccali + high_xosccali) / 2;
        value = rtc_read_osc32con1();
        value &= ~RTC_XOSCCALI_MASK;
        value |= (medium_xosccali << RTC_XOSCCALI_OFFSET);
        rtc_write_osc32con1(value);
        medium_frequency = f32k_measure_clock(FQMTR_FCKSEL_EOSC_F32K_CK, FQMTR_TCKSEL_XO_CK, 99);
        if (medium_frequency > 32768) {
            low_xosccali = medium_xosccali;
            high_frequency = medium_frequency;
        } else if (medium_frequency < 32768) {
            high_xosccali = medium_xosccali;
            low_frequency = medium_frequency;
        } else {
            RTC_LOG_INFO("xosccali = %d\r\n", medium_xosccali);
            return;
        }
    }

    if ((32768 - low_frequency) < (high_frequency - 32768)) {
        value = rtc_read_osc32con1();
        value &= ~RTC_XOSCCALI_MASK;
        value |= (high_xosccali << RTC_XOSCCALI_OFFSET);
        rtc_write_osc32con1(value);
        RTC_LOG_INFO("frequency = %u, xosccali = %d\r\n", (unsigned int)low_frequency, (unsigned int)high_xosccali);
    } else {
        value = rtc_read_osc32con1();
        value &= ~RTC_XOSCCALI_MASK;
        value |= (low_xosccali << RTC_XOSCCALI_OFFSET);
        rtc_write_osc32con1(value);
        RTC_LOG_INFO("frequency = %u, xosccali = %d\r\n", (unsigned int)high_frequency, (unsigned int)low_xosccali);
    }
}

#if RTC_32K_DEBUG
static hal_rtc_status_t rtc_wait_second_changed()
{
    hal_rtc_time_t time;
    uint32_t second;

    hal_rtc_get_time(&time);
    second = time.rtc_sec;

    do {
        hal_rtc_get_time(&time);
    } while (second == time.rtc_sec);

    return HAL_RTC_STATUS_OK;
}
#endif

static void f32k_osc32_Init(void)
{
    uint32_t value;
    uint16_t int_counter;

    /* SYSTEM_INFOD: 0xa2010040
       SLOW_SRC_B = SYSTEM_INFOD & 0x00000020 */
    if ((*RTC_SLOW_SRC_B & 0x20) == 0) {
        RTC_LOG_INFO("Use 32k crystal\r\n");
    } else {
        RTC_LOG_INFO("No 32k crystal\r\n");
    }

    if ((rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG & RTC_SPAR_INIT_STATUS_MASK) != RTC_SPAR_INIT_STATUS_MASK) {
        value = rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG;
        RTC_LOG_WARNING("RTC not init, spar value is: %x\r\n", value);

        if(retention_flag == false) {
            rtc_register_init();
        }
        /* LPD related */
        /* workaround, disable LPD firstly */
        rtc_register->RTC_LPD_CON_UNION.RTC_LPD_CON = 0x101;
        rtc_write_trigger_wait();

        rtc_set_power_key();

        /* Set Power hold here. */
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_5 | RTC_PWRBB_MASK;
        rtc_write_trigger_wait();
        RTC_LOG_INFO("PWRHOLD:%x\r\n", rtc_register->RTC_BBPU);

        /* set EOSC32_STP_PWD */
        value = rtc_read_osc32con1();
        value |= 0x80;
        rtc_write_osc32con1(value);

        /* LPD init again */
        rtc_lpd_init();

        /* enable LPD */
        rtc_register->RTC_LPD_CON_UNION.RTC_LPD_CON_CELLS.RTC_LPD_OPT = 0x0;
        rtc_write_trigger_wait();

        if ((*RTC_SLOW_SRC_B & 0x20) == 0) {
            /* Use 32k crystal */
            rtc_set_xosc_mode();

            /* select 32k clock source for Top as XOSC*/
            rtc_write_osc32con0(0x2, RTC_LOW_BYTE_OPERATION);
            /* wait 200us for hw switch time */
            hal_gpt_delay_us(200);
            /* MT2625 XOSC need more stable time, not measure 32k here, move to hal_rtc_enable_alarm()*/

            /* turn off EOSC */
            rtc_write_osc32con2(RTC_XOSC_PWDB_MASK, RTC_LOW_BYTE_OPERATION);
        } else {
            RTC_ASSERT();
            /* Make sure EOSC runs as multiple of 62.5ms */
            do {
                int_counter = *(volatile uint16_t *)(0xa2080038);
            } while (((int_counter & 0x7fff) % 2048) != 0);
            /* select 32k clock source for Top: Top is DCXO for normal mode, EOSC for retention mode */
            rtc_write_osc32con0(0x0, RTC_LOW_BYTE_OPERATION);
            /* wait 200us for hw switch time */
            hal_gpt_delay_us(200);
            /* reset TC after switch clock */
            rtc_register->RTC_TC0_UNION.RTC_TC0 = 0x0;
            rtc_register->RTC_TC1_UNION.RTC_TC1 = 0x100;
            rtc_register->RTC_TC2_UNION.RTC_TC2 = 0x101;
            rtc_register->RTC_TC3 = 0x0;
            /* turn off XOSC*/
            rtc_write_osc32con2(RTC_EOSC_PWDB_MASK, RTC_LOW_BYTE_OPERATION);
        }
        rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG |= RTC_SPAR_INIT_STATUS_MASK;
        rtc_write_trigger_wait();
        rtc_dump_register("f32k_osc32_init done");
    }
}

uint32_t f32k_measure_clock(uint32_t fixed_clock, uint32_t tested_clock, uint32_t window_setting)
{
    uint32_t fqmtr_data;
    uint32_t frequency;

    /* 1) PLL_ABIST_FQMTR_CON0 = 0xCXXX */
    abist_fqmtr_register->ABIST_FQMTR_CON0 |= 0xC000;
    hal_gpt_delay_us(1000);
    while ((abist_fqmtr_register->ABIST_FQMTR_CON1 & 0x8000) != 0);
    /* 2) CKSYS_TST_SEL_1_BASE = 0x0 */
    *CKSYS_TST_SEL_1_BASE = 0;
    /* 3) CKSYS_TST_SEL_1_BASE = 0x0601 */
    *CKSYS_TST_SEL_1_BASE = (fixed_clock << 8) | tested_clock;
    abist_fqmtr_register->ABIST_FQMTR_CON2 = 0;
    /* 4) PLL_ABIST_FQMTR_CON0 = 0x8009 */
    abist_fqmtr_register->ABIST_FQMTR_CON0 = 0x8000 | window_setting;
    hal_gpt_delay_us(1000);
    /* 5) Wait PLL_ABIST_FQMTR_CON1 & 0x8000 == 0x8000 */
    while ((abist_fqmtr_register->ABIST_FQMTR_CON1 & 0x8000) != 0);
    /* 6) Read PLL_ABIST_FQMTR_DATA */
    fqmtr_data = abist_fqmtr_register->ABIST_FQMTR_DATA;
    //RTC_LOG_WARNING("fqmtr_data = %x\r\n", fqmtr_data);
    /* 7) Freq = fxo_clk*10/PLL_ABIST_FQMTR_DATA */
    /* MT2625 only has 26MHz */
    frequency = 26000000 * (window_setting + 1) / fqmtr_data;

    return frequency;
}

uint32_t f32k_measure_count(uint16_t fixed_clock, uint16_t tested_clock, uint16_t window_setting)
{
    uint32_t fqmtr_data;

    /* 1) PLL_ABIST_FQMTR_CON0 = 0xCXXX */
    abist_fqmtr_register->ABIST_FQMTR_CON0 |= 0xC000;
    hal_gpt_delay_us(1000);
    while ((abist_fqmtr_register->ABIST_FQMTR_CON1 & 0x8000) != 0);
    /* 2) CKSYS_TST_SEL_1_BASE = 0x0 */
    *CKSYS_TST_SEL_1_BASE = 0;
    /* 3) CKSYS_TST_SEL_1_BASE = 0x0601 */
    *CKSYS_TST_SEL_1_BASE = (fixed_clock << 8) | tested_clock;
    abist_fqmtr_register->ABIST_FQMTR_CON2 = 0;
    /* 4) PLL_ABIST_FQMTR_CON0 = 0x8009 */
    abist_fqmtr_register->ABIST_FQMTR_CON0 = 0x8000 | window_setting;
    hal_gpt_delay_us(1000);
    /* 5) Wait PLL_ABIST_FQMTR_CON1 & 0x8000 == 0x8000 */
    while ((abist_fqmtr_register->ABIST_FQMTR_CON1 & 0x8000) != 0);
    /* 6) Read PLL_ABIST_FQMTR_DATA */
    fqmtr_data = abist_fqmtr_register->ABIST_FQMTR_DATA;

    return fqmtr_data;
}

hal_rtc_status_t hal_rtc_init(void)
{
    char sys_oper_mode = DEFAULT_SYS_OPER_MODE;
    
    /* DE suggest first unprotect, then reload */
    rtc_unlock_PROT();
    /* Setting for 2625 E3, bit7 for enable/disable CK used for RTC setting register */
    if (true == chip_is_mt2625E3()) {
        rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG |= RTC_SPAR_CG_CONTROL_MASK;
        rtc_write_trigger_wait();
    }
    
    rtc_reload();

    /* put rtc power on status check here */
    rtc_power_on_status_analysis();

    if ((rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG & RTC_SPAR_INIT_STATUS_MASK) == RTC_SPAR_INIT_STATUS_MASK) {
        rtc_dump_register("RTC already init");
        
        /* Get the rtc time after system has waked up. */
        rtc_get_sleep_tick(true);
            
#ifdef HAL_RTC_FEATURE_RETENTION_SRAM
        hal_rtc_exit_retention_mode();
#endif
        sys_oper_mode = DEFAULT_SYS_OPER_MODE;        
        hal_rtc_set_data(SYS_OPER_MODE_ADDR, (const char *)(&sys_oper_mode), 1);

        rtc_cali_ctr = RTC_CALI_CTR_LOCK;
        return HAL_RTC_STATUS_OK;
    } else {
        RTC_LOG_INFO("RTC SPAR reg:%x\r\n", rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG);
    }

    f32k_osc32_Init();

    /*RTC->CORE ISO can only be configured after powerkey match */
#ifdef HAL_RTC_FEATURE_RETENTION_SRAM
    hal_rtc_exit_retention_mode();
#endif

    rtc_dump_register("hal_rtc_init done");
    RTC_LOG_INFO("init_done, RTC_32K = %u, EOSC = %u, DCXO = %u, XOSC = %u\r\n",
                 (unsigned int)f32k_measure_clock(FQMTR_FCKSEL_RTC_32K, FQMTR_TCKSEL_XO_CK, 99),
                 (unsigned int)f32k_measure_clock(FQMTR_FCKSEL_EOSC_F32K_CK, FQMTR_TCKSEL_XO_CK, 99),
                 (unsigned int)f32k_measure_clock(FQMTR_FCKSEL_DCXO_F32K_CK, FQMTR_TCKSEL_XO_CK, 99),
                 (unsigned int)f32k_measure_clock(FQMTR_FCKSEL_XOSC_F32K_CK, FQMTR_TCKSEL_XO_CK, 99));

    hal_rtc_set_quarter_hour(DEFAULT_QUARTER_HOUR_VAL);
    rtc_cali_ctr = RTC_CALI_CTR_UNLOCK;
    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t hal_rtc_deinit(void)
{
    rtc_dump_register("hal_rtc_deinit done");

    return HAL_RTC_STATUS_OK;
}

ATTR_TEXT_IN_RAM hal_rtc_status_t hal_rtc_set_time(const hal_rtc_time_t *time)
{
    uint32_t mask;
    hal_rtc_time_t pre_calendar_time, pre_basic_time, cur_calendar_time;
    rtc_sw_alarm_status_t alarm_status;
    rtc_sw_timer_status_t timer_status;
    
    if (!rtc_is_time_valid(time)) {
        return HAL_RTC_STATUS_INVALID_PARAM;
    }
    cur_calendar_time = *time;
    cur_calendar_time.rtc_milli_sec = 0;
    
    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(((rtc_query_cbusy() != true) \
                                         && (rtc_judge_32K_alignment() == true)), mask);
    hal_rtc_get_time(&pre_calendar_time);
    
    rtc_register->RTC_TC0_UNION.RTC_TC0 = (time->rtc_min << RTC_TC_MINUTE_OFFSET) | (time->rtc_sec);
    rtc_register->RTC_TC1_UNION.RTC_TC1 = (time->rtc_day << RTC_TC_DOM_OFFSET) | (time->rtc_hour);
    // day-of-week range is 1~7, header file is 0~6
    rtc_register->RTC_TC2_UNION.RTC_TC2 = (time->rtc_mon << RTC_TC_MONTH_OFFSET) | (time->rtc_week + 1);
    rtc_register->RTC_TC3 = time->rtc_year;
    rtc_write_trigger();

    pre_basic_time = pre_calendar_time;
    pre_basic_time.rtc_milli_sec = (pre_basic_time.rtc_milli_sec & RTC_INT_CNT_MASK) >> 11;
    rtc_sw_timer_update_basic_time("[set time]update basic time", &pre_basic_time);

    rtc_sw_context.sw_timer_basis_time = cur_calendar_time;
    rtc_sw_context.sw_timer_basis_time_value = rtc_sw_timer_time_to_ms(&cur_calendar_time);
    
    RTC_RESTORE_INT(mask);
    rtc_wait_busy();

    rtc_time_modem.pre_calendar_time = pre_calendar_time;
    rtc_time_modem.cur_calendar_time = cur_calendar_time;
    rtc_time_modem.change_calendar = true;

    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    timer_status = rtc_sw_timer_set_expiration_time("[set time]set sw timer expiration time");
    RTC_RESTORE_INT(mask);
    
    if(timer_status == RTC_SW_TIMER_STATUS_OK) {
        rtc_wait_busy();
    }

    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    alarm_status = rtc_sw_alarm_set_expiration_time("[set time]set sw alarm expiration time");
    RTC_RESTORE_INT(mask);
    
    if(alarm_status == RTC_SW_ALARM_STATUS_OK) {
        rtc_wait_busy();
    }
    
    rtc_xosc_cali();
    
    return HAL_RTC_STATUS_OK;
}

ATTR_TEXT_IN_TCM hal_rtc_status_t hal_rtc_get_time(hal_rtc_time_t *time)
{
    uint16_t value_tc3, value_tc2, value_tc1, value_tc0, int_cnt, int_cnt_pre;
    uint32_t mask;

    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    
    /* re-read time information if internal millisecond counter has carried */
    do {
        int_cnt_pre = rtc_register->RTC_INT_CNT;
        value_tc3 = rtc_register->RTC_TC3;
        value_tc2 = rtc_register->RTC_TC2_UNION.RTC_TC2;
        value_tc1 = rtc_register->RTC_TC1_UNION.RTC_TC1;
        value_tc0 = rtc_register->RTC_TC0_UNION.RTC_TC0;
        int_cnt = rtc_register->RTC_INT_CNT;
    } while ((int_cnt & RTC_INT_CNT_MASK) < (int_cnt_pre & RTC_INT_CNT_MASK));
    RTC_RESTORE_INT(mask);

    time->rtc_year = value_tc3 & RTC_TC_YEAR_MASK;
    time->rtc_week = (value_tc2 & RTC_TC_DOW_MASK) - 1;
    time->rtc_mon = (value_tc2 & RTC_TC_MONTH_MASK) >> RTC_TC_MONTH_OFFSET;
    time->rtc_hour = value_tc1 & RTC_TC_HOUR_MASK;
    time->rtc_day = (value_tc1 & RTC_TC_DOM_MASK) >> RTC_TC_DOM_OFFSET;
    time->rtc_min = (value_tc0 & RTC_TC_MINUTE_MASK) >> RTC_TC_MINUTE_OFFSET;
    time->rtc_sec = value_tc0 & RTC_TC_SECOND_MASK;
    time->rtc_milli_sec = int_cnt;

    return HAL_RTC_STATUS_OK;
}
extern void time_test(hal_rtc_time_t *,int);
int ch_fixed_lenth_parse(unsigned char *buf,unsigned char fix_len);


ATTR_TEXT_IN_RAM hal_rtc_status_t hal_rtc_set_utc_time(const hal_rtc_time_t *utc_time)
{
    hal_rtc_time_t temp_rtc_time = *utc_time;
    uint32_t seconds, diff_seconds;
    signed char temp_quarter_hour;
    char prtbuf[100] = {0};

	
	dbg_print("hal_rtc_set_utc_time555 %d,%d,%d,%d,%d,%d",temp_rtc_time.rtc_year,temp_rtc_time.rtc_mon,temp_rtc_time.rtc_day,temp_rtc_time.rtc_hour,temp_rtc_time.rtc_min,temp_rtc_time.rtc_sec);
    if (!rtc_is_time_valid(utc_time)) {
		dbg_print("rtc_is_time_valid");
        return HAL_RTC_STATUS_INVALID_PARAM;
    }
    //get quarter hour first
    hal_rtc_get_quarter_hour(&temp_quarter_hour);

    //change utc time into local time
    seconds = rtc_calendar_time_to_seconds(&temp_rtc_time);
    if(temp_quarter_hour < 0){
        diff_seconds = (uint32_t)(-temp_quarter_hour);
        diff_seconds *= 900;

        if(seconds < diff_seconds){
            assert(0);
        }else{
            seconds -= diff_seconds;
        }
    }else{
        diff_seconds = (uint32_t)temp_quarter_hour * 900;
        seconds += diff_seconds;
    }

	dbg_print("hal_rtc_set_utc_time 3333:%d,%d",seconds,temp_quarter_hour);

    temp_rtc_time.rtc_year = 0;
    temp_rtc_time.rtc_week = 1;
    temp_rtc_time.rtc_mon = 1;
    temp_rtc_time.rtc_day = 1;
    temp_rtc_time.rtc_hour = 0;
    temp_rtc_time.rtc_min = 0;
    temp_rtc_time.rtc_sec = 0;
    temp_rtc_time.rtc_milli_sec = 0;
    memset(prtbuf,0,100);
	rtc_forward_time(&temp_rtc_time, seconds);
	sprintf(prtbuf,"hal_rtc_set_utc_time222  :%d-%d-%d-%d-%d-%d\r\n",temp_rtc_time.rtc_year,temp_rtc_time.rtc_mon, temp_rtc_time.rtc_day,temp_rtc_time.rtc_hour,temp_rtc_time.rtc_min,temp_rtc_time.rtc_sec);
    dbg_print("%s",prtbuf);
	//time_test(&temp_rtc_time,seconds);
    hal_rtc_set_time(&temp_rtc_time);

    return HAL_RTC_STATUS_OK;
}
ATTR_TEXT_IN_RAM hal_rtc_status_t hal_rtc_chuhuiset_utc_time(char *timestr)
{
	//222prtbuf : 20221202091301
	
	dbg_print("%s",timestr);
	char *ptr=timestr;
	hal_rtc_time_t temp_rtc_time;
	
	
	temp_rtc_time.rtc_year=(ch_fixed_lenth_parse(ptr,4)-2000);
	ptr+=4;
	temp_rtc_time.rtc_mon=ch_fixed_lenth_parse(ptr,2);	
	ptr+=2;	
	temp_rtc_time.rtc_day=ch_fixed_lenth_parse(ptr,2);
	ptr+=2;	
	temp_rtc_time.rtc_hour=ch_fixed_lenth_parse(ptr,2);
	ptr+=2;	
	temp_rtc_time.rtc_min=ch_fixed_lenth_parse(ptr,2);
	ptr+=2;	
	temp_rtc_time.rtc_sec=ch_fixed_lenth_parse(ptr,2);
	temp_rtc_time.rtc_week=1;
	dbg_print("hal_rtc_chuhuiset_utc_time111 %d,%d,%d,%d,%d,%d",temp_rtc_time.rtc_year,temp_rtc_time.rtc_mon,temp_rtc_time.rtc_day,temp_rtc_time.rtc_hour,temp_rtc_time.rtc_min,temp_rtc_time.rtc_sec);
//	hal_rtc_set_utc_time(&temp_rtc_time);
	hal_rtc_set_time(&temp_rtc_time);
    return HAL_RTC_STATUS_OK;
}

ATTR_TEXT_IN_TCM hal_rtc_status_t hal_rtc_get_utc_time(hal_rtc_time_t *utc_time)
{
    signed char quarter_hour;
    uint32_t seconds, diff_seconds;



    hal_rtc_get_time(utc_time);//store local time in rtc_time
    hal_rtc_get_quarter_hour(&quarter_hour);

    seconds = rtc_calendar_time_to_seconds(utc_time);

    if(quarter_hour > 0){
        diff_seconds = (uint32_t)(quarter_hour) * 900;
        if(seconds < diff_seconds){
            //assert(0);
        }
        else{
            seconds -= diff_seconds;
        }
    }
    else{
        diff_seconds = (uint32_t)(-quarter_hour) * 900;
        seconds += diff_seconds;
    }

    utc_time->rtc_year = 0;
    utc_time->rtc_week = 1;
    utc_time->rtc_mon = 1;
    utc_time->rtc_day = 1;
    utc_time->rtc_hour = 0;
    utc_time->rtc_min = 0;
    utc_time->rtc_sec = 0;
    utc_time->rtc_milli_sec = 0;

    rtc_forward_time(utc_time, seconds);

    return HAL_RTC_STATUS_OK;
}

ATTR_TEXT_IN_TCM hal_rtc_status_t hal_rtc_get_N1_utc_time(hal_rtc_time_t *utc_time)
{
	signed char quarter_hour;
	uint32_t seconds, diff_seconds;

	hal_rtc_get_time(utc_time);//store local time in rtc_time
	hal_rtc_get_quarter_hour(&quarter_hour);
   
	seconds = rtc_calendar_time_to_seconds(utc_time);
	seconds =seconds-7200;
	if(quarter_hour > 0){
		diff_seconds = (uint32_t)(quarter_hour) * 900;
		if(seconds < diff_seconds){
			//assert(0);
		}
		else{
			seconds -= diff_seconds;
		}
	}
	else{
		diff_seconds = (uint32_t)(-quarter_hour) * 900;
		seconds += diff_seconds;
	}

	utc_time->rtc_year = 0;
	utc_time->rtc_week = 1;
	utc_time->rtc_mon = 1;
	utc_time->rtc_day = 1;
	utc_time->rtc_hour = 0;
	utc_time->rtc_min = 0;
	utc_time->rtc_sec = 0;
	utc_time->rtc_milli_sec = 0;

	rtc_forward_time(utc_time, seconds);

	return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t rtc_set_time_zone(rtc_time_zone_t time_zone)
{
    signed char quarter_hour;
    if ((time_zone > UTC_MAX) || (time_zone < UTC_MIN)) {
        RTC_LOG_ERROR("Invalid time_zone : %d\r\n", time_zone);
        return HAL_RTC_STATUS_INVALID_PARAM;
    }
    quarter_hour = (signed char)time_zone;
    quarter_hour = quarter_hour*4;
    hal_rtc_set_quarter_hour(quarter_hour);
    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t rtc_get_time_zone(rtc_time_zone_t *pTime_zone)
{
    signed char quarter_hour;
    if (pTime_zone == NULL) {
        RTC_LOG_ERROR("Invalid pTime_zone = NULL\r\n");
        return HAL_RTC_STATUS_INVALID_PARAM;
    }
    hal_rtc_get_quarter_hour(&quarter_hour);
    *pTime_zone = (rtc_time_zone_t)(quarter_hour/4);
    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t hal_rtc_set_quarter_hour(signed char quarter_hour)
{
    if ((quarter_hour > 48) || (quarter_hour < -48)) {
        RTC_LOG_ERROR("Invalid quarter_hour : %d\r\n", quarter_hour);
        return HAL_RTC_STATUS_INVALID_PARAM;
    }
    hal_rtc_set_data(RTC_QUARTER_HOUR_ADDR, (const char *)(&quarter_hour), 1);
    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t hal_rtc_get_quarter_hour(signed char *pQuarter_hour)
{
    if (pQuarter_hour == NULL) {
        RTC_LOG_ERROR("Invalid pQuarter_hour = NULL\r\n");
        return HAL_RTC_STATUS_INVALID_PARAM;
    }
    hal_rtc_get_data(RTC_QUARTER_HOUR_ADDR, (char *)pQuarter_hour, 1);
    return HAL_RTC_STATUS_OK;
}

#ifdef HAL_RTC_FEATURE_TIME_CALLBACK
hal_rtc_status_t hal_rtc_set_time_notification_period(hal_rtc_time_notification_period_t period)
{
    uint32_t enable;
    uint32_t cii_setting;
    uint32_t mask;

    switch (period) {
        case HAL_RTC_TIME_NOTIFICATION_NONE:
            enable = 0;
            cii_setting = 0;
            break;
        case HAL_RTC_TIME_NOTIFICATION_EVERY_SECOND:
            enable = 1;
            cii_setting = 3;
            break;
        case HAL_RTC_TIME_NOTIFICATION_EVERY_MINUTE:
            enable = 1;
            cii_setting = 4;
            break;
        case HAL_RTC_TIME_NOTIFICATION_EVERY_HOUR:
            enable = 1;
            cii_setting = 5;
            break;
        case HAL_RTC_TIME_NOTIFICATION_EVERY_DAY:
            enable = 1;
            cii_setting = 6;
            break;
        case HAL_RTC_TIME_NOTIFICATION_EVERY_MONTH:
            enable = 1;
            cii_setting = 8;
            break;
        case HAL_RTC_TIME_NOTIFICATION_EVERY_YEAR:
            enable = 1;
            cii_setting = 9;
            break;
        case HAL_RTC_TIME_NOTIFICATION_EVERY_SECOND_1_2:
            enable = 1;
            cii_setting = 2;
            break;
        case HAL_RTC_TIME_NOTIFICATION_EVERY_SECOND_1_4:
            enable = 1;
            cii_setting = 1;
            break;
        case HAL_RTC_TIME_NOTIFICATION_EVERY_SECOND_1_8:
            enable = 1;
            cii_setting = 0;
            break;
        case HAL_RTC_TIME_NOTIFICATION_EVERY_SECOND_1_16:
            enable = 1;
            cii_setting = 10;
            break;
        default:
            return HAL_RTC_STATUS_INVALID_PARAM;
    }

    mask = save_and_set_interrupt_mask();
    rtc_register->RTC_CII_EN = (enable << RTC_TC_EN_OFFSET) | cii_setting;
    /* set TICK_PWREN here */
    rtc_register->RTC_BBPU = RTC_KEY_BBPU_2 | (enable << RTC_TICK_PWREN_OFFSET);
    
    rtc_write_trigger();
    restore_interrupt_mask(mask);
    rtc_wait_busy();

    return HAL_RTC_STATUS_OK;
}
#endif

hal_rtc_status_t hal_rtc_get_alarm(hal_rtc_time_t *time)
{
    uint16_t value_al3, value_al2, value_al1, value_al0;
    uint32_t mask;

    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    
    value_al3 = rtc_register->RTC_AL3_UNION.RTC_AL3;
    value_al2 = rtc_register->RTC_AL2_UNION.RTC_AL2;
    value_al1 = rtc_register->RTC_AL1_UNION.RTC_AL1;
    value_al0 = rtc_register->RTC_AL0_UNION.RTC_AL0;
    RTC_RESTORE_INT(mask);

    time->rtc_year = value_al3 & RTC_AL_YEAR_MASK;
    time->rtc_milli_sec = (value_al3 & RTC_AL_MS_MASK) >> RTC_AL_MS_OFFSET;
    time->rtc_week = (value_al2 & RTC_AL_DOW_MASK) - 1;
    time->rtc_mon = (value_al2 & RTC_AL_MONTH_MASK) >> RTC_AL_MONTH_OFFSET;
    time->rtc_hour = value_al1 & RTC_AL_HOUR_MASK;
    time->rtc_day = (value_al1 & RTC_AL_DOM_MASK) >> RTC_AL_DOM_OFFSET;
    time->rtc_min = (value_al0 & RTC_AL_MINUTE_MASK) >> RTC_AL_MINUTE_OFFSET;
    time->rtc_sec = value_al0 & RTC_AL_SECOND_MASK;

    return HAL_RTC_STATUS_OK;
}

void rtc_xosc_cali(void)
{
    uint32_t frequency;
    int16_t cali;
    uint32_t mask;
    
    if (((*RTC_SLOW_SRC_B & 0x20) == 0) && (rtc_cali_ctr == RTC_CALI_CTR_UNLOCK)) {
        /* Measure 32K */
        frequency = f32k_measure_clock(FQMTR_FCKSEL_XOSC_F32K_CK, FQMTR_TCKSEL_XO_CK, 99);
        /* Set RTC_CALI */
        cali = (32768 - frequency) << 3;
        if (cali > (int16_t)0xFFF) {
            cali = 0xFFF;
        } else if (cali < (int16_t)0xF000) {
            cali = 0xF000;
        }
        
        mask = save_and_set_interrupt_mask();
        /* normal RTC_CALI */
        rtc_register->RTC_CALI = cali;
        
        rtc_write_trigger();
        restore_interrupt_mask(mask);
        rtc_wait_busy();
        
        RTC_LOG_INFO("!!xosc frequency = %u, RTC_CALI = %x\r\n", (unsigned int)frequency, rtc_register->RTC_CALI);
    }
}

ATTR_TEXT_IN_RAM hal_rtc_status_t rtc_set_alarm(const hal_rtc_time_t *time)
{
    if (!rtc_is_time_valid(time)) {
        return HAL_RTC_STATUS_INVALID_PARAM;
    }

    if (rtc_register->RTC_AL0_UNION.RTC_AL0_CELLS.AL_SECOND != time->rtc_sec) {
        rtc_register->RTC_AL0_UNION.RTC_AL0_CELLS.AL_SECOND = time->rtc_sec;
    }
    if (rtc_register->RTC_AL0_UNION.RTC_AL0_CELLS.AL_MINUTE != time->rtc_min) {
        rtc_register->RTC_AL0_UNION.RTC_AL0_CELLS.AL_MINUTE = time->rtc_min;
    }
    if (rtc_register->RTC_AL1_UNION.RTC_AL1_CELLS.AL_DOM != time->rtc_day) {
        rtc_register->RTC_AL1_UNION.RTC_AL1_CELLS.AL_DOM = time->rtc_day;
    }
    if (rtc_register->RTC_AL1_UNION.RTC_AL1_CELLS.AL_HOUR != time->rtc_hour) {
        rtc_register->RTC_AL1_UNION.RTC_AL1_CELLS.AL_HOUR = time->rtc_hour;
    }
    if (rtc_register->RTC_AL2_UNION.RTC_AL2_CELLS.AL_MONTH != time->rtc_mon) {
        rtc_register->RTC_AL2_UNION.RTC_AL2_CELLS.AL_MONTH = time->rtc_mon;
    }
    if (rtc_register->RTC_AL3_UNION.RTC_AL3_CELLS.AL_YEAR != time->rtc_year) {
        rtc_register->RTC_AL3_UNION.RTC_AL3_CELLS.AL_YEAR = time->rtc_year;
    }
    if (rtc_register->RTC_AL3_UNION.RTC_AL3_CELLS.AL_MS != time->rtc_milli_sec) {
        rtc_register->RTC_AL3_UNION.RTC_AL3_CELLS.AL_MS = time->rtc_milli_sec;
    }

    return HAL_RTC_STATUS_OK;
}

ATTR_TEXT_IN_RAM void rtc_enable_alarm(void)
{
    /* Enable alarm interrupt, mask DOW value for alarm detection */
    rtc_register->RTC_AL_MASK_UNION.RTC_AL_MASK = RTC_AL_EN_MASK | RTC_AL_MASK_DOW_MASK;
    /* Enable alarm power on */
    rtc_register->RTC_BBPU = RTC_KEY_BBPU_0 | RTC_ALARM_PWREN_MASK;
}

hal_rtc_status_t hal_rtc_set_alarm(const hal_rtc_time_t *time)
{
    uint32_t mask;
    
    if (!rtc_is_time_valid(time)) {
        return HAL_RTC_STATUS_INVALID_PARAM;
    }

    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    
    rtc_set_alarm(time);

    rtc_write_trigger();
    RTC_RESTORE_INT(mask);
    rtc_wait_busy();

    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t hal_rtc_enable_alarm(void)
{
    uint32_t mask;

    mask = save_and_set_interrupt_mask();

    rtc_enable_alarm();
    
    rtc_write_trigger();
    restore_interrupt_mask(mask);
    rtc_wait_busy();

    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t hal_rtc_disable_alarm(void)
{
    uint32_t mask;

    mask = save_and_set_interrupt_mask();
    rtc_register->RTC_AL_MASK_UNION.RTC_AL_MASK_CELLS.AL_EN = (0x0 << RTC_AL_EN_OFFSET);
    rtc_register->RTC_BBPU = RTC_KEY_BBPU_0;
    
    rtc_write_trigger();
    restore_interrupt_mask(mask);
    rtc_wait_busy();

    return HAL_RTC_STATUS_OK;
}

ATTR_TEXT_IN_RAM hal_rtc_status_t rtc_set_md_alarm(const hal_rtc_time_t *time)
{
    if (!rtc_is_time_valid(time)) {
        return HAL_RTC_STATUS_INVALID_PARAM;
    }

    rtc_register->RTC_MD_AL0_UNION.RTC_MD_AL0 = (time->rtc_min << RTC_AL_MINUTE_OFFSET) | time->rtc_sec;
    rtc_register->RTC_MD_AL1_UNION.RTC_MD_AL1 = (time->rtc_day << RTC_AL_DOM_OFFSET) | time->rtc_hour;
    rtc_register->RTC_MD_AL2_UNION.RTC_MD_AL2 = (time->rtc_mon << RTC_AL_MONTH_OFFSET) | (time->rtc_week + 1);
    rtc_register->RTC_MD_AL3_UNION.RTC_MD_AL3 = ((time->rtc_milli_sec << RTC_AL_MS_OFFSET) & RTC_AL_MS_MASK) | time->rtc_year;

    return HAL_RTC_STATUS_OK;
}

ATTR_TEXT_IN_RAM void rtc_enable_md_alarm(void)
{
    /* Enable MD alarm interrupt, mask DOW value for alarm detection */
    rtc_register->RTC_MD_AL_MASK_UNION.RTC_MD_AL_MASK = RTC_AL_EN_MASK | RTC_AL_MASK_DOW_MASK;

    /* Enable alarm power on */
    rtc_register->RTC_BBPU = RTC_KEY_BBPU_0 | RTC_ALARM_PWREN_MASK;
}

hal_rtc_status_t hal_rtc_set_md_alarm(const hal_rtc_time_t *time)
{
    uint32_t mask;
    
    if (!rtc_is_time_valid(time)) {
        return HAL_RTC_STATUS_INVALID_PARAM;
    }

    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    
    rtc_set_md_alarm(time);
        
    rtc_write_trigger();
    RTC_RESTORE_INT(mask);
    rtc_wait_busy();

    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t hal_rtc_get_md_alarm(hal_rtc_time_t *time)
{
    uint32_t mask;
    uint16_t value_al3, value_al2, value_al1, value_al0;

    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    
    value_al3 = rtc_register->RTC_MD_AL3_UNION.RTC_MD_AL3;
    value_al2 = rtc_register->RTC_MD_AL2_UNION.RTC_MD_AL2;
    value_al1 = rtc_register->RTC_MD_AL1_UNION.RTC_MD_AL1;
    value_al0 = rtc_register->RTC_MD_AL0_UNION.RTC_MD_AL0;
    RTC_RESTORE_INT(mask);

    time->rtc_year = value_al3 & RTC_AL_YEAR_MASK;
    time->rtc_milli_sec = (value_al3 & RTC_AL_MS_MASK) >> RTC_AL_MS_OFFSET;
    time->rtc_week = (value_al2 & RTC_AL_DOW_MASK) - 1;
    time->rtc_mon = (value_al2 & RTC_AL_MONTH_MASK) >> RTC_AL_MONTH_OFFSET;
    time->rtc_hour = value_al1 & RTC_AL_HOUR_MASK;
    time->rtc_day = (value_al1 & RTC_AL_DOM_MASK) >> RTC_AL_DOM_OFFSET;
    time->rtc_min = (value_al0 & RTC_AL_MINUTE_MASK) >> RTC_AL_MINUTE_OFFSET;
    time->rtc_sec = value_al0 & RTC_AL_SECOND_MASK;

    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t hal_rtc_enable_md_alarm(void)
{
    uint32_t mask;

    mask = save_and_set_interrupt_mask();
    
    rtc_enable_md_alarm();
    
    rtc_write_trigger();
    restore_interrupt_mask(mask);
    rtc_wait_busy();
    
    rtc_dump_register("After rtc_enable_md_alarm");

    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t hal_rtc_disable_md_alarm(void)
{
    uint32_t mask;

    mask = save_and_set_interrupt_mask();
    rtc_register->RTC_MD_AL_MASK_UNION.RTC_MD_AL_MASK = (0x0 << RTC_AL_EN_OFFSET);

    rtc_register->RTC_BBPU = RTC_KEY_BBPU_0;

    rtc_write_trigger();
    restore_interrupt_mask(mask);
    rtc_wait_busy();

    return HAL_RTC_STATUS_OK;
}

#ifdef HAL_RTC_FEATURE_TIME_CALLBACK
hal_rtc_status_t hal_rtc_set_time_callback(hal_rtc_time_callback_t callback_function, void *user_data)
{
    rtc_time_callback_function = callback_function;
    rtc_time_user_data = user_data;

    return HAL_RTC_STATUS_OK;
}
#endif

hal_rtc_status_t hal_rtc_set_alarm_callback(const hal_rtc_alarm_callback_t callback_function, void *user_data)
{
    rtc_alarm_callback_function = callback_function;
    rtc_alarm_user_data = user_data;

    return HAL_RTC_STATUS_OK;
}

#ifdef HAL_RTC_FEATURE_ELAPSED_TICK
hal_rtc_status_t hal_rtc_set_md_alarm_callback(const hal_rtc_alarm_callback_t callback_function, void *user_data)
{
    rtc_md_alarm_callback_function = callback_function;
    rtc_md_alarm_user_data = user_data;

    return HAL_RTC_STATUS_OK;
}
#endif

hal_rtc_status_t hal_rtc_set_eint_callback(const hal_rtc_eint_callback_t callback_function, void *user_data)
{
    rtc_eint_callback_function = callback_function;
    rtc_eint_user_data = user_data;

    return HAL_RTC_STATUS_OK;
}

#ifdef HAL_RTC_FEATURE_CALIBRATION
hal_rtc_status_t hal_rtc_set_one_shot_calibration(int16_t ticks)
{
    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t hal_rtc_get_one_shot_calibration(int16_t *ticks)
{
    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t hal_rtc_set_repeat_calibration(int16_t ticks_per_8_seconds)
{
    uint32_t value = 0;
    uint32_t mask;
    
    if (rtc_cali_ctr == RTC_CALI_CTR_UNLOCK)
    {
        if (ticks_per_8_seconds > (int16_t)0xFFF) {
            ticks_per_8_seconds = 0xFFF;
        } else if (ticks_per_8_seconds < (int16_t)0xF000) {
            ticks_per_8_seconds = 0xF000;
        }
        value = ((ticks_per_8_seconds << RTC_RTC_CALI_OFFSET) & RTC_RTC_CALI_MASK);

        mask = save_and_set_interrupt_mask();
        rtc_register->RTC_CALI = value;

        rtc_write_trigger();
        restore_interrupt_mask(mask);
        rtc_wait_busy();
    }
    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t hal_rtc_get_repeat_calibration(int16_t *ticks_per_8_seconds)
{
    rtc_reload();

    *ticks_per_8_seconds = (rtc_register->RTC_CALI & RTC_RTC_CALI_MASK) >> RTC_RTC_CALI_OFFSET;

    *ticks_per_8_seconds &= 0x1FFF;
    if (*ticks_per_8_seconds >= 0x1000) {
        *ticks_per_8_seconds -= 0x2000;
    }
    return HAL_RTC_STATUS_OK;
}
#endif

hal_rtc_status_t hal_rtc_get_f32k_frequency(uint32_t *frequency)
{
    *frequency = f32k_measure_clock(FQMTR_FCKSEL_RTC_32K, FQMTR_TCKSEL_XO_CK, 99);
    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t rtc_set_data(uint16_t offset, const char *buf, uint16_t len, bool access_hw)
{
    uint16_t i = 0;
    uint32_t mask;

    if ((offset >= (sizeof(rtc_spare_register_table) / sizeof(rtc_spare_register_table[0]))) ||
        (offset + len > (sizeof(rtc_spare_register_table) / sizeof(rtc_spare_register_table[0]))) || (buf == NULL)) {
        RTC_LOG_ERROR("Invalid parameter, offset = %d, len = %d, buf = %p", offset, len, buf);
        return HAL_RTC_STATUS_INVALID_PARAM;
    }

    for (i = 0; i < len; i++) {
        if (access_hw) {
            mask = save_and_set_interrupt_mask();
            *(rtc_spare_register_table[i + offset].address) = *(buf + i);
            
            rtc_write_trigger();
            restore_interrupt_mask(mask);
            rtc_wait_busy();
        } else {
            rtc_spare_register_backup[offset + i] = *(buf + i);
        }
    }

    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t hal_rtc_set_data(uint16_t offset, const char *buf, uint16_t len)
{
    if (rtc_in_test) {
        RTC_LOG_WARNING("%s: in rtc test mode.", __func__);
    }

    rtc_set_data(offset, buf, len, !rtc_in_test);

    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t rtc_get_data(uint16_t offset, char *buf, uint16_t len, bool access_hw)
{
    uint16_t i = 0;

    if ((offset >= (sizeof(rtc_spare_register_table) / sizeof(rtc_spare_register_table[0]))) ||
        (offset + len > (sizeof(rtc_spare_register_table) / sizeof(rtc_spare_register_table[0]))) || (buf == NULL)) {
        RTC_LOG_ERROR("Invalid parameter, offset = %d, len = %d, buf = %p", offset, len, buf);
        return HAL_RTC_STATUS_INVALID_PARAM;
    }

    for (i = 0; i < len; i++) {
        if (access_hw) {
            *(buf + i) = *(rtc_spare_register_table[i + offset].address);
        } else {
            *(buf + i) = rtc_spare_register_backup[offset + i];
        }
    }

    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t hal_rtc_get_data(uint16_t offset, char *buf, uint16_t len)
{
    if (rtc_in_test) {
        RTC_LOG_WARNING("%s: in rtc test mode.", __func__);
    }

    rtc_get_data(offset, buf, len, !rtc_in_test);

    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t rtc_clear_data(uint16_t offset, uint16_t len)
{
    char buf[HAL_RTC_BACKUP_BYTE_NUM_MAX];

    memset(buf, 0, sizeof(buf));

    rtc_set_data(offset, buf, len, true);

    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t hal_rtc_clear_data(uint16_t offset, uint16_t len)
{
    char buf[HAL_RTC_BACKUP_BYTE_NUM_MAX];

    if (rtc_in_test) {
        RTC_LOG_WARNING("%s: in rtc test mode.", __func__);
    }

    memset(buf, 0, sizeof(buf));

    rtc_set_data(offset, buf, len, !rtc_in_test);

    return HAL_RTC_STATUS_OK;
}

void rtc_set_register(uint16_t address, uint16_t value)
{
    uint32_t mask;
    
    if (address > (uint32_t) & (((RTC_REGISTER_T *)0)->RTC_SPAR5_UNION.RTC_SPAR5)) {
        RTC_LOG_ERROR("Invalid address");
    }

    mask = save_and_set_interrupt_mask();
    *(uint16_t *)((uint8_t *)rtc_register + address) = value;

    rtc_write_trigger();
    restore_interrupt_mask(mask);
    rtc_wait_busy();
}

uint16_t rtc_get_register(uint16_t address)
{
    if (address > (uint32_t) & (((RTC_REGISTER_T *)0)->RTC_SPAR5_UNION.RTC_SPAR5)) {
        RTC_LOG_ERROR("Invalid address");
    }

    rtc_reload();
    
    return *(uint16_t *)((uint8_t *)rtc_register + address);
}

ATTR_TEXT_IN_TCM void rtc_forward_time(hal_rtc_time_t *time, uint32_t second)
{
    uint32_t minute = 0;
    uint32_t hour = 0;
    uint32_t day = 0;
    uint32_t max_day;
    const uint32_t days_in_month[13] = {
        0,  /* Null */
        31, /* Jan */
        28, /* Feb */
        31, /* Mar */
        30, /* Apr */
        31, /* May */
        30, /* Jun */
        31, /* Jul */
        31, /* Aug */
        30, /* Sep */
        31, /* Oct */
        30, /* Nov */
        31  /* Dec */
    };

    second += time->rtc_sec;
    minute = time->rtc_min;
    hour = time->rtc_hour;
    day = time->rtc_day;

    if (second > 59) {
        /* min */
        minute += second / 60;
        second %= 60;
    }
    time->rtc_sec = second;
    
    if (minute > 59) {
        /* hour */
        hour += minute / 60;
        minute %= 60;
    }
    time->rtc_min = minute;
    
    if (hour > 23) {
        /* day of week */
        day += hour / 24;
        hour %= 24;
    }
    time->rtc_hour = hour;

    while(1) {
        max_day = days_in_month[time->rtc_mon];
        if (time->rtc_mon == 2) {
            if((((time->rtc_year % 4) == 0) && ((time->rtc_year % 100) != 0)) || ((time->rtc_year % 400) == 0)) {
                max_day++;
            }
        }
        if (day > max_day) {
            day -= max_day;

            /* month of year */
            time->rtc_mon++;
            if (time->rtc_mon > 12) {
                time->rtc_mon = 1;
                time->rtc_year++;
            }
        } else {
            break;
        }
    }
    time->rtc_day = day;
}

static void test_rtc_alarm_callback(void *parameter)
{
    hal_rtc_time_t *alarm_power_on_time;
    RTC_LOG_INFO("test_rtc_alarm_callback");

    alarm_power_on_time = (hal_rtc_time_t *)parameter;

    RTC_LOG_INFO("target alarm time: 20%d,%d,%d (%d) %d:%d:%d", alarm_power_on_time->rtc_year,
                 alarm_power_on_time->rtc_mon, alarm_power_on_time->rtc_day, alarm_power_on_time->rtc_week,
                 alarm_power_on_time->rtc_hour, alarm_power_on_time->rtc_min, alarm_power_on_time->rtc_sec);
    hal_rtc_set_alarm_callback(NULL, NULL);

    hal_rtc_set_alarm(alarm_power_on_time);
    hal_rtc_enable_alarm();
}

hal_rtc_status_t rtc_alarm_power_on_test(hal_rtc_time_t *time)
{
    static hal_rtc_time_t alarm_power_on_time;
    hal_rtc_time_t rtc_get_time;

    memcpy(&alarm_power_on_time, time, sizeof(hal_rtc_time_t));

    RTC_LOG_INFO("target alarm time: 20%d,%d,%d (%d) %d:%d:%d", time->rtc_year,
                 time->rtc_mon, time->rtc_day, time->rtc_week, time->rtc_hour, time->rtc_min, time->rtc_sec);
    hal_rtc_get_time(&rtc_get_time);
    RTC_LOG_INFO("get alarm time: 20%d,%d,%d (%d) %d:%d:%d", rtc_get_time.rtc_year,
                 rtc_get_time.rtc_mon, rtc_get_time.rtc_day, rtc_get_time.rtc_week - 1,
                 rtc_get_time.rtc_hour, rtc_get_time.rtc_min, rtc_get_time.rtc_sec);
    rtc_forward_time(&rtc_get_time, 10);
    RTC_LOG_INFO("set alarm time: 20%d,%d,%d (%d) %d:%d:%d", rtc_get_time.rtc_year,
                 rtc_get_time.rtc_mon, rtc_get_time.rtc_day, rtc_get_time.rtc_week - 1,
                 rtc_get_time.rtc_hour, rtc_get_time.rtc_min, rtc_get_time.rtc_sec);
    hal_rtc_set_alarm(&rtc_get_time);
    hal_rtc_set_alarm_callback(test_rtc_alarm_callback, &alarm_power_on_time);
    hal_rtc_enable_alarm();

    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t rtc_enter_test(bool enter)
{
    RTC_LOG_INFO("%s: %d", __func__, enter);

    if (enter) {
        rtc_get_data(0, rtc_spare_register_backup, HAL_RTC_BACKUP_BYTE_NUM_MAX, true);
    } else {
        rtc_set_data(0, rtc_spare_register_backup, HAL_RTC_BACKUP_BYTE_NUM_MAX, true);
    }
    rtc_in_test = enter;

    return HAL_RTC_STATUS_OK;
}

void rtc_register_shut_down_callback(f_rtc_callback_t cb)
{
    if(cb != NULL){
        g_rtc_callback = cb;
        RTC_LOG_INFO("rtc_callback: %x", g_rtc_callback);
    }else{
        RTC_ASSERT();
    }
}

void rtc_trigger_shut_down_callback(void)
{
    g_rtc_callback(NULL);
}

#ifdef HAL_RTC_FEATURE_RETENTION_SRAM
#if 0
static uint32_t rtc_get_register_value(uint32_t address, uint32_t mask, uint32_t shift)
{
    uint32_t change_value, mask_buffer;
    mask_buffer = (mask << shift);
    change_value = *((volatile uint32_t *)(address));
    change_value &= mask_buffer;
    change_value = (change_value >> shift);
    return change_value;
}
#endif
static void rtc_set_register_value(uint32_t address, uint32_t mask, uint32_t shift, uint32_t value)
{
    uint32_t mask_buffer,target_value;
    mask_buffer = (~(mask << shift));
    target_value = *((volatile uint32_t *)(address));
    target_value &= mask_buffer;
    target_value |= (value << shift);
    *((volatile uint32_t *)(address)) = target_value;
}

static void rtc_set_pmu_ao_latch(void)
{
    /* AO-Latch Registers */
    hal_gpt_delay_us(200);
    rtc_set_register_value(PMU_RG_AO_LATCH_SET_ADDR, PMU_RG_AO_LATCH_SET_MASK, PMU_RG_AO_LATCH_SET_SHIFT, 1);
    hal_gpt_delay_us(200);
    rtc_set_register_value(PMU_RG_AO_LATCH_SET_ADDR, PMU_RG_AO_LATCH_SET_MASK, PMU_RG_AO_LATCH_SET_SHIFT, 0);
}

uint32_t rtc_get_banks_in_active(void)
{
    uint32_t sramcon_low_byte, sramcon_high_byte, value_sramcon;

    value_sramcon = rtc_read_sramcon();
    sramcon_low_byte = value_sramcon & 0xff;
    sramcon_high_byte = (value_sramcon & 0xff00) >> 8;

    return ((sramcon_high_byte&(~sramcon_low_byte))&HAL_RTC_RETENTION_SRAM_NUMBER_MASK);
}

hal_rtc_status_t hal_rtc_retention_sram_config(uint32_t mask, hal_rtc_sram_mode_t mode)
{
    uint32_t sramcon_low_byte, sramcon_high_byte, value_sramcon;
    uint32_t value_osc32con2, osc32con2_high_byte;
    uint8_t i, banks_in_normal = 0;
    if ((mode > HAL_RTC_SRAM_PD_MODE) || (mask >= (1 << HAL_RTC_RETENTION_SRAM_NUMBER_MAX))) {
        return HAL_RTC_STATUS_INVALID_PARAM;
    }
    if (mask == 0) {
        /* User don't care about the SRAM configuration, just return. */
        return HAL_RTC_STATUS_OK;
    }

    value_sramcon = rtc_read_sramcon();
    sramcon_low_byte = value_sramcon & 0xff;
    sramcon_high_byte = (value_sramcon & 0xff00) >> 8;
    value_osc32con2 = rtc_read_osc32con2();
    osc32con2_high_byte = (value_osc32con2 & 0xff00) >> 8;

    RTC_LOG_INFO("[in] mask=%x, mode=%x, sramcon=%x, osc32con2=%x, A2070A94=%x, A207080C=%x\r\n",
                 mask, mode, value_sramcon, value_osc32con2, DRV_Reg32(0xA2070A94), DRV_Reg32(0xa207080c));
    switch (mode) {
        case HAL_RTC_SRAM_NORMAL_MODE:
            /* IF all bankds are PD mode, turn on LDO */
            if (sramcon_low_byte == HAL_RTC_RETENTION_SRAM_NUMBER_MASK) {
                rtc_set_register_value(PMU_RG_VSRAM_EN_ADDR, PMU_RG_VSRAM_EN_MASK, PMU_RG_VSRAM_EN_SHIFT, 1);
                rtc_set_pmu_ao_latch();
                hal_gpt_delay_us(1);
            }

            /* Gating CS, CK. One cycle delay/5ns before ISOINTB set. */
            osc32con2_high_byte |= 0x30;
            rtc_write_osc32con2(osc32con2_high_byte, RTC_HIGH_BYTE_OPERATION);

            /* switch retention SRAM power to core power, power on retention SRAM one by one */
            /* From power down mode to normal mode ; when exit from sleep mode, all should turn on */
            for (i = 0; i < HAL_RTC_RETENTION_SRAM_NUMBER_MAX; i++) {
                if (mask & (1 << i)) {
                    if (value_sramcon&(1<<i)) {
                        /* Power down to Normal: PD change from 1 to 0 */
                        sramcon_low_byte &= ~(1 << i);
                        rtc_write_sramcon(sramcon_low_byte, RTC_LOW_BYTE_OPERATION);
                    } else if (!(value_sramcon&(0x100<<i))) {
                        /* Sleep to Normal: SLEEPB change from 0 to 1;
                                            ISOINTB change from 0 to 1*/
                        sramcon_high_byte |= 0x1 << i;
                        rtc_write_sramcon(sramcon_high_byte, RTC_HIGH_BYTE_OPERATION);
                    }
                }
            }
            hal_gpt_delay_us(1);

            /* Set ISOINTB to 1. One cycle delay/5ns before release CS, CK. */
            osc32con2_high_byte |= mask;
            rtc_write_osc32con2(osc32con2_high_byte, RTC_HIGH_BYTE_OPERATION);
            hal_gpt_delay_us(1);

            /* Release CS, CK. */
            osc32con2_high_byte &= (~0x30);
            rtc_write_osc32con2(osc32con2_high_byte, RTC_HIGH_BYTE_OPERATION);
            break;
        case HAL_RTC_SRAM_SLEEP_MODE:
            /* IF all bankds are PD mode, turn on LDO */
            if (sramcon_low_byte == HAL_RTC_RETENTION_SRAM_NUMBER_MASK) {
                rtc_set_register_value(PMU_RG_VSRAM_EN_ADDR, PMU_RG_VSRAM_EN_MASK, PMU_RG_VSRAM_EN_SHIFT, 1);
                rtc_set_pmu_ao_latch();
            }

            /* Gating CS, CK. One cycle delay/5ns before ISOINTB set. */
            osc32con2_high_byte |= 0x30;
            rtc_write_osc32con2(osc32con2_high_byte, RTC_HIGH_BYTE_OPERATION);

            for (i = 0; i < HAL_RTC_RETENTION_SRAM_NUMBER_MAX; i++) {
                if (value_sramcon&(1<<i)) {
                    /* Bank in PD */
                    if (mask & (1 << i)) {
                        /* PD to Normal: PD change from 1 to 0 */
                        sramcon_low_byte &= ~(1 << i);
                        rtc_write_sramcon(sramcon_low_byte, RTC_LOW_BYTE_OPERATION);
                    } else {
                        continue;
                    }
                } else if (!(value_sramcon&(0x100<<i))) {
                    /* Bank in Sleep */
                    continue;
                } else if (!(mask & (1 << i))) {
                    /* Bank in Normal */
                    banks_in_normal++;
                    continue;
                }

                if (mask & (1 << i)) {
                    /* Set ISOINTB to 0. */
                    osc32con2_high_byte &= ~(1 << i);
                    rtc_write_osc32con2(osc32con2_high_byte, RTC_HIGH_BYTE_OPERATION);
                    hal_gpt_delay_us(1);

                    /* Normal to Sleep: SLEEPB change from 1 to 0 */
                    sramcon_high_byte &= ~(1 << i);
                    rtc_write_sramcon(sramcon_high_byte, RTC_HIGH_BYTE_OPERATION);
                }
            }

            /* IF some banks are in Normal, releasing CS, CK */
            if (banks_in_normal) {
                osc32con2_high_byte &= (~0x30);
                rtc_write_osc32con2(osc32con2_high_byte, RTC_HIGH_BYTE_OPERATION);
                hal_gpt_delay_us(1);
            }
            break;
        case HAL_RTC_SRAM_PD_MODE:
            /* Gating CS, CK. One cycle delay/5ns before ISOINTB set. */
            osc32con2_high_byte |= 0x30;
            rtc_write_osc32con2(osc32con2_high_byte, RTC_HIGH_BYTE_OPERATION);

            for (i = 0; i < HAL_RTC_RETENTION_SRAM_NUMBER_MAX; i++) {
                if (value_sramcon&(1<<i)) {
                    /* Bank in PD */
                    continue;
                } else if (!(value_sramcon&(0x100<<i))) {
                    /* Bank in Sleep */
                    if (mask & (1 << i)) {
                        /* Sleep to Normal: SLEEPB change from 0 to 1 */
                        sramcon_high_byte |= 0x1 << i;
                        rtc_write_sramcon(sramcon_high_byte, RTC_HIGH_BYTE_OPERATION);

                        /* Set ISOINTB to 1. */
                        osc32con2_high_byte |= (1 << i);
                        rtc_write_osc32con2(osc32con2_high_byte, RTC_HIGH_BYTE_OPERATION);
                        hal_gpt_delay_us(1);
                    }
                } else if (!(mask & (1 << i))) {
                    /* Bank in Normal */
                    banks_in_normal++;
                    continue;
                }

                if (mask & (1 << i)) {
                    sramcon_low_byte |= 0x1 << i;
                    rtc_write_sramcon(sramcon_low_byte, RTC_LOW_BYTE_OPERATION);
                }
            }

            /* IF all banks are PD mode, turn off LDO */
            if (sramcon_low_byte == HAL_RTC_RETENTION_SRAM_NUMBER_MASK) {
                rtc_set_register_value(PMU_RG_VSRAM_EN_ADDR, PMU_RG_VSRAM_EN_MASK, PMU_RG_VSRAM_EN_SHIFT, 0);
                rtc_set_pmu_ao_latch();
                hal_gpt_delay_us(1);
            } else if (banks_in_normal) {
                /* IF some banks are in Normal, releasing CS, CK */
                osc32con2_high_byte &= (~0x30);
                rtc_write_osc32con2(osc32con2_high_byte, RTC_HIGH_BYTE_OPERATION);
                hal_gpt_delay_us(1);
            }

            break;
        default:
            break;
    }

    RTC_LOG_INFO("[out] mask=%x, mode=%x, sramcon=%x, osc32con2=%x, A2070A94=%x, A207080C=%x\r\n",
                 mask, mode, rtc_read_sramcon(), rtc_read_osc32con2(), DRV_Reg32(0xA2070A94), DRV_Reg32(0xa207080c));

    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t hal_rtc_enter_retention_mode(void)
{
    int16_t cali;
    uint16_t int_counter;
    char sys_oper_mode = DEFAULT_SYS_OPER_MODE;

    hal_rtc_get_data(SYS_OPER_MODE_ADDR, (char *)(&sys_oper_mode), 1);

    /*Enter retention/rtc mode, HW will do the power switch */
    if ((*RTC_SLOW_SRC_B & 0x20) != 0) {
        RTC_ASSERT();
        /*1.switch to eosc from DCXO
              2.calibration
              3.measurement
              4.enter retention mode */
        hal_rtc_get_data(0, (char *)(&cali), 2);

        /* Make sure DCXO runs @62.5ms, so that EOSC can run @ multiple of 62.5ms */
        do {
            int_counter = *(volatile uint16_t *)(0xa2080038);
        } while (((int_counter & 0x7fff) % 2048) != 0);
        rtc_write_osc32con0(0x1, RTC_LOW_BYTE_OPERATION);
        /* wait 200us for hw switch time */
        hal_gpt_delay_us(200);

        /* K_EOSC32 RTC_CALI */
        rtc_register->RTC_CALI = cali | RTC_CALI_RW_SEL_MASK;
        rtc_write_trigger_wait();
        RTC_LOG_INFO("RTC_CALI = %x\r\n",rtc_register->RTC_CALI);
    }

    /* Back up the rtc time before system real sleep. */
    rtc_get_sleep_tick(false);
    
    if (rtc_mode_flag == true) {
        /* RTC mode flow start */
        RTC_LOG_INFO("hal_rtc_enter_retention_mode rtc_mode_flag flow\r\n");
        /* Set RTC SRAM in PD mode */
        hal_rtc_retention_sram_config(HAL_RTC_RETENTION_SRAM_NUMBER_MASK, HAL_RTC_SRAM_PD_MODE);
        /* Prepare flag for Bootrom */
        if(sys_oper_mode == FORCED_SHUT_DOWN_MODE) {
            rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG &= ~(0x1);
        } else {
            rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG |= 0x1;
        }
        rtc_write_trigger_wait();
        
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_1 | RTC_RTC_PU_MASK;
        rtc_write_trigger_wait();
        
        /* Setting for 2625 E3, bit7 for enable/disable CK used for RTC setting register */
        if (true == chip_is_mt2625E3()) {
            rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG &= (~RTC_SPAR_CG_CONTROL_MASK);
            rtc_write_trigger_wait();
        }
        
        rtc_dump_register("hal_rtc_enter_rtc_mode");
        /* Set BBPU[5] = 0 */
        while(1){
            rtc_register->RTC_BBPU = RTC_KEY_BBPU_5;
            rtc_write_trigger_wait();
            hal_gpt_delay_ms(1); // Just for chip enter rtc mode
        }
    } else {
        uint32_t banks_in_active;

        /* Retention flow start */
        banks_in_active = rtc_get_banks_in_active();
        if (banks_in_active) {
            /* Only make Normal banks sleep */
            hal_rtc_retention_sram_config(banks_in_active, HAL_RTC_SRAM_SLEEP_MODE);
        }

        rtc_dump_register("hal_rtc_enter_retention_mode");
        /*DE said 2625 has no risk condition for enter retention mode */
        /* Prepare flag for Bootrom */
        rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG |= 0x1;
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_1 | RTC_RTC_PU_MASK;
        rtc_write_trigger_wait();
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_3 | RTC_EINT_PWREN_MASK;
        rtc_write_trigger_wait();
        
        /* Setting for 2625 E3, bit7 for enable/disable CK used for RTC setting register */
        if (true == chip_is_mt2625E3()) {
            rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG &= (~RTC_SPAR_CG_CONTROL_MASK);
            rtc_write_trigger_wait();
        }
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_4 | RTC_RETENTION_MODE_MASK;
        rtc_write_trigger_wait();
        hal_gpt_delay_ms(1000); // Just for chip enter retention mode
    }
    return HAL_RTC_STATUS_ERROR;
}

void hal_rtc_exit_retention_mode(void)
{
    uint32_t value, frequency;
    int16_t cali;
    uint16_t int_counter;
    value = rtc_power_on_result();
    if (value == 0x1) {
        retention_flag = true;
    } else if ((value == 0x2) || (value == 0x6)) {
        rtc_mode_flag = true;
    }

    if ((rtc_mode_flag == true) || (retention_flag == true)) {
        /*1.switch back to xo_div32 from eosc
             */
        if ((*RTC_SLOW_SRC_B & 0x20) != 0) {
            RTC_ASSERT();
            /* Make sure EOSC runs @multiple of 62.5ms*/
            do {
                int_counter = *(volatile uint16_t *)(0xa2080038);
            } while (((int_counter & 0x7fff) % 2048) != 0);
            /* 32k-less mode, need switch Top 32k source from EOSC to XO_DIV32 */
            rtc_write_osc32con0(0x0, RTC_LOW_BYTE_OPERATION);
            /* wait 200us for hw switch time */
            hal_gpt_delay_us(200);
            /* clear RTC_CALI */
            rtc_register->RTC_CALI = 0x0;
            rtc_write_trigger_wait();
            //calibration for EOSC and cali here
            f32k_eosc32_calibration();
            frequency = f32k_measure_clock(FQMTR_FCKSEL_EOSC_F32K_CK, FQMTR_TCKSEL_XO_CK, 99);
            /* Set RTC_CALI */
            cali = (32768 - frequency);
            if (cali > (int16_t)0xFFF) {
                cali = 0xFFF;
            } else if (cali < (int16_t)0xF000) {
                cali = 0xF000;
            }
            hal_rtc_set_data(0, (const char *)(&cali), 2);
        }
    }
    if(rtc_mode_flag == true) {
        /* Back from rtc mode */
        /* unlock protect for write register */
        rtc_unlock_PROT();
        /* set power hold */
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_5 | RTC_PWRBB_MASK;
        rtc_write_trigger_wait();
        /* write SRAM_ISO=0x3f, SRAM_CON=0x0f0f */
        rtc_write_osc32con2(0x3f, RTC_HIGH_BYTE_OPERATION);
        rtc_write_sramcon(0x0f0f, RTC_WORD_OPERATION);
        /* Prepare flag for Bootrom */
        value = rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG;
        value &= (~0x1);
        rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG = value;
        /* clear BBPU[1] */
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_3 | RTC_EINT_PWREN_MASK;
        rtc_write_trigger_wait();
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_1 | RTC_RTC_PU_MASK;
        rtc_write_trigger_wait();
        rtc_mode_flag = false;
        RTC_LOG_INFO("leave rtc mode\r\n");
    }
    if (retention_flag == true) {
        /* unlock protect for write register */
        rtc_unlock_PROT();
        /* set power hold */
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_5 | RTC_PWRBB_MASK;
        rtc_write_trigger_wait();
        /* Prepare flag for Bootrom */
        value = rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG;
        value &= (~0x1);
        rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG = value;
        /* clear BBPU[4] and BBPU[1] */
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_4;
        rtc_write_trigger_wait();
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_3 | RTC_EINT_PWREN_MASK;
        rtc_write_trigger_wait();
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_1 | RTC_RTC_PU_MASK;
        rtc_write_trigger_wait();

        rtc_dump_register("hal_rtc_exit_retention_mode");
        //leave retention mode
        RTC_LOG_INFO("leave retention mode\r\n");
        /* PMU HW will help with the power domain switch */
    }

    /*Power on Retention SRAM one by one. */
    hal_rtc_retention_sram_config(HAL_RTC_RETENTION_SRAM_NUMBER_MASK, HAL_RTC_SRAM_NORMAL_MODE);
}
#endif


#ifdef HAL_RTC_FEATURE_ELAPSED_TICK
/* RTC 32k is on, RTC SRAM is powered off. */
hal_rtc_status_t hal_rtc_enter_rtc_mode(void)
{
    rtc_mode_flag = true;
    hal_rtc_enter_retention_mode();

    return HAL_RTC_STATUS_ERROR;
}

hal_rtc_status_t hal_rtc_enter_forced_shut_down_mode(hal_rtc_ctrl_attr_t eint_wakeup, hal_rtc_ctrl_attr_t tick_wakeup, hal_rtc_ctrl_attr_t alarm_wakeup)
{
    uint32_t mask;
    char forced_shut_down_mode = FORCED_SHUT_DOWN_MODE;

    mask = save_and_set_interrupt_mask();
    hal_rtc_set_data(SYS_OPER_MODE_ADDR, (const char *)(&forced_shut_down_mode), 1);

    if(eint_wakeup == HAL_RTC_ENABLE) {
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_3 | RTC_EINT_PWREN_MASK;
        rtc_write_trigger_wait();
    } else if(eint_wakeup == HAL_RTC_DISABLE) {
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_3;
        rtc_write_trigger_wait();
    }

    if(tick_wakeup == HAL_RTC_ENABLE) {
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_2 | RTC_TICK_PWREN_MASK;
        rtc_write_trigger_wait();
    } else if(tick_wakeup == HAL_RTC_DISABLE) {
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_2;
        rtc_write_trigger_wait();
    }

    if(alarm_wakeup == HAL_RTC_ENABLE) {
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_0 | RTC_ALARM_PWREN_MASK;
        rtc_write_trigger_wait();
    } else if(alarm_wakeup == HAL_RTC_DISABLE) {
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_0;
        rtc_write_trigger_wait();
    }

    rtc_mode_flag = true;
    hal_rtc_enter_retention_mode();
    restore_interrupt_mask(mask);
    
    return HAL_RTC_STATUS_ERROR;
}

hal_rtc_status_t hal_rtc_enter_forced_reset_mode(void)
{
#ifdef HAL_WDT_MODULE_ENABLED
    char forced_reset_mode = FORCED_RESET_MODE;
    hal_wdt_config_t wdt_config;

    hal_rtc_set_data(SYS_OPER_MODE_ADDR, (const char *)(&forced_reset_mode), 1);
    wdt_config.mode = HAL_WDT_MODE_RESET;
    wdt_config.seconds = 1;
    hal_wdt_disable(HAL_WDT_DISABLE_MAGIC);
    hal_wdt_init(&wdt_config);
    hal_wdt_software_reset();
    while(1);
#endif
    return HAL_RTC_STATUS_ERROR;
}


/* RTC spare register should be directly readable and writable */
static void rtc_power_on_status_analysis()
{
    uint32_t value_wrtgr, value_bbpu, value;
    char sys_oper_mode = DEFAULT_SYS_OPER_MODE;

    value = rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG;
    RTC_LOG_INFO("RTC_SPAR_REG:%x\r\n", value);

    /* For cold reset will clear PWRBB, but remaining SPAR_REG, need clear INIT_STATUS to figure out this case */
    value &= ~(RTC_SPAR_POWERON_REASON_MASK | RTC_SPAR_INIT_STATUS_MASK);

    value_wrtgr = rtc_register->RTC_WRTGR_UNION.RTC_WRTGR_CELLS.RTC_STA;
    value_bbpu = rtc_register->RTC_BBPU;
    hal_rtc_get_data(SYS_OPER_MODE_ADDR, (char *)(&sys_oper_mode), 1);

    if ((value_wrtgr & RTC_POWER_DETECTED_MASK) != RTC_POWER_DETECTED_MASK) {
        /* POWER_DETECTED equals 0, this is 1st boot up, spar_reg[13:11] already as 3'b000 */
        RTC_LOG_IMPORTANT("RTC 1st boot up\r\n");
    } else {
        if ((value_bbpu & RTC_RETENTION_MODE_MASK) == RTC_RETENTION_MODE_MASK) {
            /* BBPU[4] equals 1, this is back from deep sleep mode */
            RTC_LOG_IMPORTANT("RTC back from deep sleep\r\n");
            value |= (0x1 << RTC_SPAR_POWERON_REASON_OFFSET);
            /* This case shouldn't do RTC init any more */
            value |= RTC_SPAR_INIT_STATUS_MASK;
        } else if ((value_bbpu & RTC_PWRBB_MASK) == RTC_PWRBB_MASK) {
            /* BBPU[4] equals 0 but BBPU[5] equals 1, this is back from poweroff mode*/
            /* Note : firstly, need to check if wdt reset is happened, but INIT_STATUS as set. */
            /*          secondly, need to check for forced shut down, and if so, INIT_STATUS should not set. */
            if (DRV_Reg32(0xA2090020) == 0x2) {
                RTC_LOG_IMPORTANT("WDT hw reset happened\r\n");
                value |= (0x4 << RTC_SPAR_POWERON_REASON_OFFSET);
                value |= RTC_SPAR_INIT_STATUS_MASK;
            } else if (DRV_Reg32(0xA2090020) == 0x1) {
                if (sys_oper_mode == DEFAULT_SYS_OPER_MODE) {
                    RTC_LOG_IMPORTANT("WDT sw reset happened\r\n");
                    value |= (0x5 << RTC_SPAR_POWERON_REASON_OFFSET);
                    value |= RTC_SPAR_INIT_STATUS_MASK;
                } else {
                    RTC_LOG_IMPORTANT("RTC back from forced reset\r\n");
                    value |= (0x7 << RTC_SPAR_POWERON_REASON_OFFSET);
                    value |= RTC_SPAR_INIT_STATUS_MASK;
                }
            } else if (sys_oper_mode == DEFAULT_SYS_OPER_MODE){
                RTC_LOG_IMPORTANT("RTC back from deeper sleep\r\n");
                value |= (0x2 << RTC_SPAR_POWERON_REASON_OFFSET);
                value |= RTC_SPAR_INIT_STATUS_MASK;
            } else {
                RTC_LOG_IMPORTANT("RTC back from forced shut down\r\n");
                value |= (0x6 << RTC_SPAR_POWERON_REASON_OFFSET);
                value |= RTC_SPAR_INIT_STATUS_MASK;
            }
        } else {
            if (sys_oper_mode == DEFAULT_SYS_OPER_MODE) {
                if ((value_bbpu & RTC_EINT_PWREN_MASK) == RTC_EINT_PWREN_MASK) {
                    /* BBPU[3] equals 1, BBPU[4] and BBPU[5] equal 0, this is back from poweroff mode */
                    RTC_LOG_IMPORTANT("RTC back from deeper sleep\r\n");
                    value |= (0x2 << RTC_SPAR_POWERON_REASON_OFFSET);
                    value |= RTC_SPAR_INIT_STATUS_MASK;
                } else {
                    /* BBPU[3], BBPU[4] and BBPU[5] equal 0, this is back from long press shutdown or sys_reset */
                    RTC_LOG_IMPORTANT("RTC back from long press shutdown or sys_reset\r\n");
                    value |= (0x3 << RTC_SPAR_POWERON_REASON_OFFSET);
                }
            } else {
                    /* sys_oper_mode = FORCED_SHUT_DOWN_MODE */
                    RTC_LOG_IMPORTANT("RTC back from forced shut down\r\n");
                    value |= (0x6 << RTC_SPAR_POWERON_REASON_OFFSET);
                    value |= RTC_SPAR_INIT_STATUS_MASK;
            }
        }
    }
    rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG = value;
    rtc_write_trigger_wait();
}

uint32_t rtc_power_on_result()
{
    uint32_t value;
    rtc_reload();
    value = rtc_register->RTC_SPAR_REG_UNION.RTC_SPAR_REG;
    value = (value & RTC_SPAR_POWERON_REASON_MASK) >> RTC_SPAR_POWERON_REASON_OFFSET;
    //RTC_LOG_INFO("rtc_power_on_result value = %x\r\n", value);
    return value;
}

void rtc_set_gpio_output(uint8_t pin, uint8_t value)
{
    uint32_t mask;

    mask = save_and_set_interrupt_mask();
    rtc_register->RTC_GPIO_CON0_UNION.RTC_GPIO_CON0 = 0xCACA;
    /* 2625 only has RTC_GPIO_0 */
    /* 2625 + 2503 has RTC_GPIO_0 and RTC_GPIO_1*/
    if (0 == pin) {
        rtc_register->RTC_GPIO_CON1_UNION.RTC_GPIO_CON1 = (0x5C00 | (value & 0x01));
    } else if (1 == pin) {
        rtc_register->RTC_GPIO_CON1_UNION.RTC_GPIO_CON1 = (0x3700 | ((value & 0x01) << 2));
    } else {
        rtc_register->RTC_GPIO_CON1_UNION.RTC_GPIO_CON1 = 0xB600;
    }
    
    rtc_write_trigger();
    restore_interrupt_mask(mask);
    rtc_wait_busy();
    /* stable time is 5ms from DE's guide */
    hal_gpt_delay_ms(5);
    RTC_LOG_INFO("Set RTC GPIO[%d] value = %x\r\n", pin, value);
}

hal_rtc_status_t hal_rtc_configure_gpio(hal_rtc_gpio_t pin, hal_rtc_gpio_control_t *gpio_control)
{
    uint32_t value = 0xb600;
    uint32_t mask;
    
    if (gpio_control == NULL) {
        RTC_LOG_INFO("invalid parameter\r\n");
        return HAL_RTC_STATUS_INVALID_PARAM;
    }
    if (gpio_control->is_sw_control == true) {
        RTC_LOG_INFO("is_sw_control\r\n");

        mask = save_and_set_interrupt_mask();
        rtc_register->RTC_GPIO_CON0_UNION.RTC_GPIO_CON0 = 0xCACA;
        /* 2625 only has RTC_GPIO_0 */
        /* 2625 + 2503 has RTC_GPIO_0 and RTC_GPIO_1*/
        if (HAL_RTC_GPIO_0 == pin) {
            rtc_register->RTC_GPIO_CON1_UNION.RTC_GPIO_CON1 = (0x5C00 | ((gpio_control->is_sw_output_high) & 0x01));
        } else if (HAL_RTC_GPIO_1 == pin) {
            rtc_register->RTC_GPIO_CON1_UNION.RTC_GPIO_CON1 = (0x3700 | (((gpio_control->is_sw_output_high) & 0x01) << 2));
        } else {
            rtc_register->RTC_GPIO_CON1_UNION.RTC_GPIO_CON1 = 0xB600;
        }

        rtc_write_trigger();
        restore_interrupt_mask(mask);
        rtc_wait_busy();
        
        RTC_LOG_INFO("Set RTC GPIO[%d] value = %x\r\n", pin, (gpio_control->is_sw_output_high));
    } else {
        RTC_LOG_INFO("is not _sw_control\r\n");
        /* RTC GPIO output will be controlled by HW signals. */
        if (gpio_control->is_eint_output_high) {
            value |= 0x40;
        }
        if (gpio_control->is_tick_output_high) {
            value |= 0x20;
        }
        if (gpio_control->is_alarm_output_high) {
            value |= 0x10;
        }
        if (gpio_control->is_clear_output) {
            value |= 0x80;
        }
        if (HAL_RTC_GPIO_0 == pin) {
            value |= 0x02;
        } else if (HAL_RTC_GPIO_1 == pin) {
            value |= 0x08;
        }
        RTC_LOG_INFO("Set value = %x\r\n", value);

        mask = save_and_set_interrupt_mask();
        rtc_register->RTC_GPIO_CON1_UNION.RTC_GPIO_CON1 = value;

        rtc_write_trigger();
        restore_interrupt_mask(mask);
        rtc_wait_busy();
    }
    /* stable time is 5ms from DE's guide */
    hal_gpt_delay_ms(5);

    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t hal_rtc_eint_init(hal_rtc_eint_config_t *eint_config)
{
    uint32_t value = 0x8a;
    if (eint_config == NULL) {
        return HAL_RTC_STATUS_INVALID_PARAM;
    }
    if (eint_config->is_enable_rtc_eint == true) {
        /* EINT_EN, SYNC_EN, and EINT_IRQ_EN set to 1. */
        value |= 0x1500;
        if (eint_config->is_falling_edge_active == true) {
            /* If falling edge active setting, need set PU first, default is PD. */
            value |= 0x804;
        }
        if (eint_config->is_enable_debounce == true) {
            value |= 0x200;
        }
        rtc_register->RTC_EINT_UNION.RTC_EINT = value;
        rtc_register->RTC_BBPU = RTC_KEY_BBPU_3 | RTC_EINT_PWREN_MASK;
        rtc_write_trigger_wait();
    }

    return HAL_RTC_STATUS_OK;
}

/* Public version of rtc_power_on_result() */
rtc_power_on_result_t rtc_power_on_result_external()
{
    rtc_power_on_result_t result = 0;

    result = (rtc_power_on_result_t)rtc_power_on_result();
    return result;
}

ATTR_TEXT_IN_TCM uint32_t rtc_calendar_time_to_seconds(hal_rtc_time_t *rtc_time)
{
    /* seconds time max from 2000.1.1 0:0:0 is 4005072000
    2^32 = 4294967296 > above data */
    uint32_t year, result;
    static const uint32_t past_days[12] =
    {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    year = rtc_time->rtc_year + 2000; /* base year from RTC module */

    /* calculate days start from 2000.1.1 */
    result = (year - 2000) * 365 + past_days[rtc_time->rtc_mon-1];
    result += (year - 2000) / 4;
    result -= (year - 2000) / 100;
    result += (year - 2000) / 400;
    result += 1;

    /* minus one day if this is leap year, but the date is before Feb. 29th */
    if (((((year % 4) == 0) && ((year % 100) != 0)) || ((year % 400) == 0)) &&(rtc_time->rtc_mon < 3)) {
        result--;
    }
    result += rtc_time->rtc_day - 1;

    result *= 24;
    result += rtc_time->rtc_hour;
    result *= 60;
    result += rtc_time->rtc_min;
    result *= 60;
    result += rtc_time->rtc_sec;

    //RTC_LOG_INFO("second result: %d\r\n", result);
    return result;
}


uint32_t rtc_get_second_diff_from_two_time(hal_rtc_time_t *start_point, hal_rtc_time_t *end_point)
{
    uint32_t start_result, end_result;

    start_result = rtc_calendar_time_to_seconds(start_point);
    end_result = rtc_calendar_time_to_seconds(end_point);
    return (end_result - start_result);
}

/* Calibration algrithm implement */
static uint32_t rtc_tick_calculation(hal_rtc_time_t *start_point, hal_rtc_time_t *end_point, uint32_t cali_value)
{
    uint32_t int_cnt[2];
    bool cali_status[2];
    uint32_t elapsed_second;
    uint32_t tick_count;
    int32_t cali_value_signed;

    int_cnt[0] = start_point->rtc_milli_sec & 0x7fff;
    int_cnt[1] = end_point->rtc_milli_sec & 0x7fff;

    cali_status[0] = (start_point->rtc_milli_sec & 0x8000) >> 15;
    cali_status[1] = (end_point->rtc_milli_sec & 0x8000) >> 15;

    RTC_LOG_INFO("int_cnt[0]: result=%u\r\n", int_cnt[0]);
    RTC_LOG_INFO("int_cnt[1]: result=%u\r\n", int_cnt[1]);
    RTC_LOG_INFO("cali_status[0]: result=%u\r\n", cali_status[0]);
    RTC_LOG_INFO("cali_status[0]: result=%u\r\n", cali_status[0]);
    RTC_LOG_INFO("current time: %d-%d-%d %d:%d:%d\r\n",
                 start_point->rtc_year,
                 start_point->rtc_mon,
                 start_point->rtc_day,
                 start_point->rtc_hour,
                 start_point->rtc_min,
                 start_point->rtc_sec
                );

    RTC_LOG_INFO("end time: %d-%d-%d %d:%d:%d\r\n",
                 end_point->rtc_year,
                 end_point->rtc_mon,
                 end_point->rtc_day,
                 end_point->rtc_hour,
                 end_point->rtc_min,
                 end_point->rtc_sec
                );

    elapsed_second = rtc_get_second_diff_from_two_time(start_point, end_point);

    /* Change RTC_CALI from 2's complement value to signed integer value */
    if (cali_value >= HAL_RTC_NEGATIVE_CALI_MASK) {
        cali_value_signed = cali_value - (HAL_RTC_NEGATIVE_CALI_MASK << 1);
    } else {
        cali_value_signed = cali_value;
    }

    RTC_LOG_INFO("elapsed_second: result=%u\r\n", elapsed_second);
    RTC_LOG_INFO("cali_value:%x\r\n", cali_value);
    RTC_LOG_INFO("cali_value_signed:%x\r\n", cali_value_signed);
    RTC_LOG_INFO("cali_value_signed*(1/8):%x\r\n", (cali_value_signed >> 3));

    /* All below calculation is for XOSC that should be used in this project */
    if (int_cnt[1] >= int_cnt[0]) {
        /*
        Case1:
          int_cnt[1] >= int_cnt[0]
            CNT = (int_cnt[1] - int_cnt[0])
                  - 1/8 * cali_value * (cali_status[1] - cali_status[0])
                  + (elapsed_second) * (32768 - 1/8 * cali_value)
        */
        tick_count = (int_cnt[1] - int_cnt[0]) - (cali_value_signed >> 3) * (cali_status[1] - cali_status[0])
                     + elapsed_second * (32768 - (cali_value_signed >> 3));
    } else {
        /*
        Case2:
         int_cnt[1] < int_cnt[0]
           CNT = (int_cnt[1] - int_cnt[0] + 32768)
                  - 1/8 * cali_value * (cali_status[1] - cali_status[0] + 1)
                  + (elapsed_seconds - 1) * (32768 - 1/8 * cali_value)
         */
        tick_count = (int_cnt[1] - int_cnt[0] + 32768) - (cali_value_signed >> 3) * (cali_status[1] - cali_status[0] + 1)
                     + (elapsed_second - 1) * (32768 - (cali_value_signed >> 3));
    }
    return tick_count;
}
/**
 * @brief This function stores the current RTC time and internal counter information
 *        if the parameter is false, and calculates the elapsed 32k tick number from
 *        last call of this function with parameter as false to the current time point
 *        if the parameter is true.
 * @param[in] is_after_sleep is the flag to determine whether system already back
 *            from deep sleep mode.
 * @param[out] elapsed_tick is the elapsed 32k tick number. If is_after_sleep is false,
 *             elapsed_tick is 0, if is_after_sleep is true, elapsed_tick is the
 *             elapsed 32k tick number from last call of this function with parameter
 *             as false to the current time point.
 * @return #HAL_RTC_STATUS_INVALID_PARAM, an invalid parameter is given.
 *         #HAL_RTC_STATUS_OK, the operation completed successfully.
 * @par       Example
 * @code
 *
 *       static uint32_t elapsed_tick;
 *       // Stores the current RTC time and internal counter information before entering deep sleep mode.
 *       if(HAL_RTC_STATUS_OK != hal_rtc_get_elapsed_tick(false, &elapsed_tick)) {
 *           // Error handler
 *       }
 *
 * @endcode
 * @code
 *
 *       static uint32_t elapsed_tick;
 *       // Call function to get the elapsed 32k tick number after exiting deep sleep mode.
 *       if(HAL_RTC_STATUS_OK != hal_rtc_get_elapsed_tick(true, &elapsed_tick)) {
 *           // Error handler
 *       }
 *
 * @endcode
 */

hal_rtc_status_t hal_rtc_get_elapsed_tick(bool is_after_sleep, uint32_t *elapsed_tick)
{
    hal_rtc_time_t rtc_wakeup_time;
    uint32_t rtc_cali_value = 0, real_sleep_time = 0;
    static bool is_first_time = true;
    /* Now this API only for ext 32k mode */
    if (elapsed_tick == NULL) {
        return HAL_RTC_STATUS_INVALID_PARAM;
    }
    if ((*RTC_SLOW_SRC_B & 0x20) != 0) {
        RTC_ASSERT();
        /* not using XOSC, return error */
        return HAL_RTC_STATUS_INVALID_PARAM;
    }

    if (is_after_sleep == false) {
        /* This API is called before entering sleep mode, just record time value. */
        hal_rtc_get_time(&(rtc_time_modem.sleep_time));
        rtc_time_modem.change_calendar = false;
        RTC_LOG_INFO("backup time value: %d-%d-%d %d:%d:%d (%d)\r\n",
                     rtc_time_modem.sleep_time.rtc_year,
                     rtc_time_modem.sleep_time.rtc_mon,
                     rtc_time_modem.sleep_time.rtc_day,
                     rtc_time_modem.sleep_time.rtc_hour,
                     rtc_time_modem.sleep_time.rtc_min,
                     rtc_time_modem.sleep_time.rtc_sec,
                     rtc_time_modem.sleep_time.rtc_milli_sec
                    );

        *elapsed_tick = 0;
        *(elapsed_tick + 1) = 0;
        rtc_cali_ctr = RTC_CALI_CTR_LOCK;
    } else {
        /* This API is called after exiting sleep mode, calculate the elapsed tick number. */
        hal_rtc_get_time(&rtc_wakeup_time);
        RTC_LOG_INFO("restore time value: %d-%d-%d %d:%d:%d (%d)\r\n",
                     rtc_wakeup_time.rtc_year,
                     rtc_wakeup_time.rtc_mon,
                     rtc_wakeup_time.rtc_day,
                     rtc_wakeup_time.rtc_hour,
                     rtc_wakeup_time.rtc_min,
                     rtc_wakeup_time.rtc_sec,
                     rtc_wakeup_time.rtc_milli_sec
                    );
        
        rtc_cali_value = rtc_register->RTC_CALI;
        RTC_LOG_INFO("RTC_CALI:%x\r\n", rtc_cali_value);

        if(rtc_time_modem.change_calendar)
        {
            *elapsed_tick = rtc_tick_calculation(&(rtc_time_modem.sleep_time), &(rtc_time_modem.pre_calendar_time), rtc_cali_value);
            *elapsed_tick += rtc_tick_calculation(&(rtc_time_modem.cur_calendar_time), &rtc_wakeup_time, rtc_cali_value);
            *elapsed_tick += CHANGE_CALENDAR_DEVIATION;
        }
        else
        {
            *elapsed_tick = rtc_tick_calculation(&(rtc_time_modem.sleep_time), &rtc_wakeup_time, rtc_cali_value);
        }

        if(is_first_time)
        {
            is_first_time = false;
            real_sleep_time = rtc_register->RTC_SPAR2_UNION.RTC_SPAR2 & 0xFFFF;
            real_sleep_time |= ((rtc_register->RTC_SPAR3_UNION.RTC_SPAR3 & 0xFFFF) << 16);
            *(elapsed_tick + 1) = real_sleep_time;
        }
        else
        {
            *(elapsed_tick + 1) = 0;
        }
        
        rtc_cali_ctr = RTC_CALI_CTR_UNLOCK;
    }
    return HAL_RTC_STATUS_OK;
}


static hal_rtc_status_t rtc_get_sleep_tick(bool is_after_sleep)
{
    hal_rtc_time_t rtc_time, rtc_backup_time;
    uint32_t rtc_cali_value = 0, elapsed_tick = 0;
    if ((*RTC_SLOW_SRC_B & 0x20) != 0) {
        RTC_ASSERT();
        /* not using XOSC, return error */
        return HAL_RTC_STATUS_INVALID_PARAM;
    }

    if (is_after_sleep == false) {
        /* This API is called before entering sleep mode, just record time value. */
        hal_rtc_get_time(&rtc_time);
        rtc_register->RTC_SPAR2_UNION.RTC_SPAR2 = (rtc_time.rtc_year << 8) | rtc_time.rtc_mon;
        rtc_register->RTC_SPAR3_UNION.RTC_SPAR3 = (rtc_time.rtc_day << 8) | rtc_time.rtc_hour;
        rtc_register->RTC_SPAR4_UNION.RTC_SPAR4 = (rtc_time.rtc_min << 8) | rtc_time.rtc_sec;
        rtc_register->RTC_SPAR5_UNION.RTC_SPAR5 = rtc_time.rtc_milli_sec;
        rtc_write_trigger_wait();

        RTC_LOG_INFO("backup time value: %d-%d-%d %d:%d:%d (%d)\r\n",
                     rtc_time.rtc_year,
                     rtc_time.rtc_mon,
                     rtc_time.rtc_day,
                     rtc_time.rtc_hour,
                     rtc_time.rtc_min,
                     rtc_time.rtc_sec,
                     rtc_time.rtc_milli_sec
                    );
    } else {
        /* This API is called after exiting sleep mode, calculate the elapsed tick number. */
        hal_rtc_get_time(&rtc_time);
        rtc_cali_value = rtc_register->RTC_CALI;
        RTC_LOG_INFO("RTC_CALI:%x\r\n", rtc_cali_value);
        rtc_backup_time.rtc_year = rtc_register->RTC_SPAR2_UNION.RTC_SPAR2_CELLS.RTC_SPAR2_1;
        rtc_backup_time.rtc_mon = rtc_register->RTC_SPAR2_UNION.RTC_SPAR2_CELLS.RTC_SPAR2_0;
        rtc_backup_time.rtc_day = rtc_register->RTC_SPAR3_UNION.RTC_SPAR3_CELLS.RTC_SPAR3_1;
        rtc_backup_time.rtc_hour = rtc_register->RTC_SPAR3_UNION.RTC_SPAR3_CELLS.RTC_SPAR3_0;
        rtc_backup_time.rtc_min = rtc_register->RTC_SPAR4_UNION.RTC_SPAR4_CELLS.RTC_SPAR4_1;
        rtc_backup_time.rtc_sec = rtc_register->RTC_SPAR4_UNION.RTC_SPAR4_CELLS.RTC_SPAR4_0;
        rtc_backup_time.rtc_milli_sec = rtc_register->RTC_SPAR5_UNION.RTC_SPAR5;

        RTC_LOG_INFO("restore time value: %d-%d-%d %d:%d:%d (%d)\r\n",
                     rtc_backup_time.rtc_year,
                     rtc_backup_time.rtc_mon,
                     rtc_backup_time.rtc_day,
                     rtc_backup_time.rtc_hour,
                     rtc_backup_time.rtc_min,
                     rtc_backup_time.rtc_sec,
                     rtc_backup_time.rtc_milli_sec
                    );

        elapsed_tick = rtc_tick_calculation(&rtc_backup_time, &rtc_time, rtc_cali_value);

        rtc_register->RTC_SPAR2_UNION.RTC_SPAR2 = elapsed_tick & 0xFFFF;
        rtc_register->RTC_SPAR3_UNION.RTC_SPAR3 = (elapsed_tick >> 16) & 0xFFFF;
        rtc_write_trigger_wait();
        
    }
    return HAL_RTC_STATUS_OK;
}


ATTR_ZIDATA_IN_RETSRAM hal_rtc_time_t rtc_time_sram;
hal_rtc_status_t rtc_get_elapsed_tick_sram(bool is_after_sleep, uint64_t *elapsed_tick)
{
    hal_rtc_time_t rtc_wakeup_time;
    uint32_t elapsed_second;

    if (elapsed_tick == NULL) {
        return HAL_RTC_STATUS_INVALID_PARAM;
    }

    if (is_after_sleep == false) {
        /* This API is called before entering sleep mode, just record time value. */
        hal_rtc_get_time(&rtc_time_sram);

        RTC_LOG_INFO("backup time value: %d-%d-%d %d:%d:%d (%d)\r\n",
                     rtc_time_sram.rtc_year,
                     rtc_time_sram.rtc_mon,
                     rtc_time_sram.rtc_day,
                     rtc_time_sram.rtc_hour,
                     rtc_time_sram.rtc_min,
                     rtc_time_sram.rtc_sec,
                     rtc_time_sram.rtc_milli_sec
                    );

        *elapsed_tick = 0;
    } else {
        /* This API is called after exiting sleep mode, calculate the elapsed tick number. */
        hal_rtc_get_time(&rtc_wakeup_time);

        RTC_LOG_INFO("restore time value: %d-%d-%d %d:%d:%d (%d)\r\n",
                     rtc_time_sram.rtc_year,
                     rtc_time_sram.rtc_mon,
                     rtc_time_sram.rtc_day,
                     rtc_time_sram.rtc_hour,
                     rtc_time_sram.rtc_min,
                     rtc_time_sram.rtc_sec,
                     rtc_time_sram.rtc_milli_sec
                    );

        elapsed_second = rtc_get_second_diff_from_two_time(&rtc_time_sram, &rtc_wakeup_time);

        *elapsed_tick = ((uint64_t)elapsed_second) * 32768 + (rtc_wakeup_time.rtc_milli_sec & 0x7fff) - (rtc_time_sram.rtc_milli_sec & 0x7fff);
    }
    return HAL_RTC_STATUS_OK;
}

#endif /* end of HAL_RTC_FEATURE_ELAPSED_TICK */

#endif /* end of HAL_RTC_MODULE_ENABLED */


