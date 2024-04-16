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

#include <stdio.h>
#include <stdlib.h>
#include "hal_platform.h"
#include "hal_clock_platform_mt2625.h"
#include "hal_clock.h"
#include "hal_clock_internal.h"
#include "hal_log.h"
#include "hal_gpt.h"

#ifdef MTK_NVDM_MODEM_ENABLE
#include "nvdm.h"
#include "nvdm_modem.h"

#define DCXO_RFIC_CALIBRATION

typedef struct {
    /** Capacitor bank code for coarce DCXO freqeuncy control */
    uint16_t cap_id; /* 0 - 511 */
    /** Fine frequency error (controlled outside DCXO) */
    int16_t init_ppm; /* s7.8 */
} N1RfAfcCalData;
#endif

//#define BL_LOG_ENBALE
//#define DCXO_DUMP_CAPID

#ifdef BL_LOG_ENBALE
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_CRIT,
    LOG_NONE,
} bl_log_level_t;

extern void bl_print(bl_log_level_t level, char *fmt, ...);
#define DBGMSG bl_print
#else
#define LOG_DEBUG 0
#define DBGMSG(level, fmt, arg...) printf(fmt, ##arg)
#endif /* BL_LOG_ENBALE */

uint32_t g_current_capid = 0;

void hal_dcxo_init()
{
    log_hal_info("DCXO init\r\n");
    *DCXO_PCON4__F_DCXO_PWR_EN_TD  = 0x1;               // DCXO_PWR_CTRL_BASE (0XA2060000) + 0x0010, 1T 32K to turn on DCXO power after receive enable from SPM
    *DCXO_PCON3__F_DCXO_EN_TD = 0xA;                    // DCXO_PWR_CTRL_BASE (0XA2060000) + 0x000f, set DCXO power settle time to 305.1us
    *DCXO_PCON3__F_DCXO_BUF_EN_TD = 0x52;               // DCXO_PWR_CTRL_BASE (0XA2060000) + 0x000e, set DCXO settle time to 2500us
    *DCXO_PCON3__F_DCXO_BUF_EN_TD_1612 = 0x10;          // DCXO_PWR_CTRL_BASE (0XA2060000) + 0x000d, set DCXO state transition time to 510us (>450us)
    *DCXO_PCON0__F_GSM_DCXO_CTL_EN = 0x1;               // DCXO_PWR_CTRL_BASE (0XA2060000) + 0x0000, enable baseband control
    *DCXO_PCON1__F_EXT_DCXO_CTL_EN = 0x1;               // DCXO_PWR_CTRL_BASE (0XA2060000) + 0x0007, enable external control
    *DCXO_PCON5__F_DCXO_ACAL_EFUSE = 0x1;
    *DCXO_PCON5__F_DCXO_ACAL_EFUSE_SEL = 0x1;
}

void hal_dcxo_capid_init(void)
{
    uint32_t dcxo_cap_id = 0;

    /* set default val */
    dcxo_cap_id = *DCXO_CAP_ID;
    dcxo_cap_id &= (~DCXO_CAP_ID_MASK);
    dcxo_cap_id |= (DCXO_CAPID_DEFAULT_VALUE << DCXO_CAP_ID_BIT);
    *DCXO_CAP_ID = dcxo_cap_id;

    /* set sel = 1 */
    *DCXO_CAP_ID = (0x00000001 | dcxo_cap_id);
    DBGMSG(LOG_DEBUG, "CAPID init done, default value = %lu \r\n", ((*DCXO_CAP_ID) & DCXO_CAP_ID_MASK) >> DCXO_CAP_ID_BIT);
    g_current_capid = DCXO_CAPID_DEFAULT_VALUE;
}

#if defined(DCXO_CAPID_STORE_TO_EFUSE0) || defined(DCXO_CAPID_STORE_TO_EFUSE1) || defined(DCXO_CAPID_STORE_TO_EFUSE2)
static void write_efuse_capid(uint32_t cal_capid)
{
    uint32_t efuse_capid  = *DCXO_EFUSE_CAPID;
    uint32_t efuse_capid0 = (efuse_capid & DCXO_EFUSE_CAPID0_MASK) >> DCXO_EFUSE_CAPID0_BIT;
    uint32_t efuse_capid1 = (efuse_capid & DCXO_EFUSE_CAPID1_MASK) >> DCXO_EFUSE_CAPID1_BIT;
    uint32_t efuse_capid2 = (efuse_capid & DCXO_EFUSE_CAPID2_MASK) >> DCXO_EFUSE_CAPID2_BIT;

    DBGMSG(LOG_DEBUG, "EFUSE CAPID = %u \r\n", efuse_capid);
    DBGMSG(LOG_DEBUG, "EFUSE CAPID0 = %d \r\n", efuse_capid0);
    DBGMSG(LOG_DEBUG, "EFUSE CAPID1 = %d \r\n", efuse_capid1);
    DBGMSG(LOG_DEBUG, "EFUSE CAPID2 = %d \r\n", efuse_capid2);
    DBGMSG(LOG_DEBUG, "target CAPID = %d \r\n", cal_capid);

    //PMU VSIM
    *((volatile uint32_t*)(0xA2070000+0x0A44)) = 0x1;
    hal_gpt_delay_us(200);

#if defined(DCXO_CAPID_STORE_TO_EFUSE0)
    if( efuse_capid0 == 0 ){
        *DCXO_EFUSE_MAGIC = DCXO_EFUSE_MAGIC_CODE;
        *DCXO_EFUSE_CAPID = ((cal_capid << DCXO_EFUSE_CAPID0_BIT) & DCXO_EFUSE_CAPID0_MASK);
        *DCXO_EFUSE_REFRESH = DCXO_EFUSE_REFRESH_SET;
        DBGMSG(LOG_DEBUG, "CAPID Calibration DONE!!!! EFUSE CAPID0 write (calibration result = %d )\r\n", cal_capid);
    } else {
        DBGMSG(LOG_DEBUG, "CAPID Calibrated!!!!! \r\n");
        if(efuse_capid0 != cal_capid) {
            DBGMSG(LOG_DEBUG, "WARNING!!!! EFUSE CAPID0 is not match calibration result ( EFUSE CAPID0 = %d , calibration result = %d )\r\n", efuse_capid0, cal_capid);
        } else
            DBGMSG(LOG_DEBUG, "EFUSE CAPID0 is match calibration result = %d \r\n", efuse_capid0);
    }
    DBGMSG(LOG_DEBUG, "Please re-download the main bin \r\n");
    while(1);
#elif defined(DCXO_CAPID_STORE_TO_EFUSE1)
    if( efuse_capid1 == 0 ){
        *DCXO_EFUSE_MAGIC = DCXO_EFUSE_MAGIC_CODE;
        *DCXO_EFUSE_CAPID = ( (cal_capid << DCXO_EFUSE_CAPID1_BIT) & DCXO_EFUSE_CAPID1_MASK);
        *DCXO_EFUSE_REFRESH = DCXO_EFUSE_REFRESH_SET;
        DBGMSG(LOG_DEBUG, "CAPID Calibration DONE!!!! EFUSE CAPID1 write (calibration result = %d )\r\n",cal_capid);
    } else {
        DBGMSG(LOG_DEBUG, "CAPID Calibrated!!!!! \r\n");
        if(efuse_capid1 != cal_capid) {
            DBGMSG(LOG_DEBUG, "WARNING!!!! EFUSE CAPID1 is not match calibration result ( EFUSE CAPID1 = %d , calibration result = %d )\r\n",efuse_capid1,cal_capid);
        } else
            DBGMSG(LOG_DEBUG, "EFUSE CAPID1 is match calibration result = %d \r\n", efuse_capid1);
    }
    DBGMSG(LOG_DEBUG, "Please re-download the main bin \r\n");
    while(1);
#elif defined(DCXO_CAPID_STORE_TO_EFUSE2)
    if( efuse_capid2 == 0 ){
        *DCXO_EFUSE_MAGIC = DCXO_EFUSE_MAGIC_CODE;
        *DCXO_EFUSE_CAPID = ( (cal_capid << DCXO_EFUSE_CAPID2_BIT) & DCXO_EFUSE_CAPID2_MASK);
        *DCXO_EFUSE_REFRESH = DCXO_EFUSE_REFRESH_SET;
        DBGMSG(LOG_DEBUG, "CAPID Calibration DONE!!!! EFUSE CAPID2 write (calibration result = %d )\r\n", cal_capid);
    } else {
        DBGMSG(LOG_DEBUG, "CAPID Calibrated!!!!! \r\n");
        if(efuse_capid2 != cal_capid) {
            DBGMSG(LOG_DEBUG, "WARNING!!!! EFUSE CAPID2 is not match calibration result ( EFUSE CAPID2 = %d , calibration result = %d )\r\n", efuse_capid2, cal_capid);
        } else
            DBGMSG(LOG_DEBUG, "EFUSE CAPID2 is match calibration result = %d \r\n", efuse_capid2);
    }
    DBGMSG(LOG_DEBUG, "Please re-download the main bin \r\n");
    while(1);
#else
    DBGMSG(LOG_DEBUG, "Error, please specify EFUSE CAPID 0 or 1 or 2 \r\n");
    while(1);
#endif  /* DCXO_CAPID_STORE_TO_EFUSE0, 1, 2 */
}
#endif  /* DCXO_CAPID_STORE_TO_EFUSE0 or DCXO_CAPID_STORE_TO_EFUSE1 or DCXO_CAPID_STORE_TO_EFUSE2 */

#ifdef DCXO_CAPID_LOAD_FROM_EFUSE
static uint32_t read_efuse_capid()
{
    uint32_t efuse_capid  = *DCXO_EFUSE_CAPID;
    uint32_t efuse_capid0 = (efuse_capid & DCXO_EFUSE_CAPID0_MASK) >> DCXO_EFUSE_CAPID0_BIT;
    uint32_t efuse_capid1 = (efuse_capid & DCXO_EFUSE_CAPID1_MASK) >> DCXO_EFUSE_CAPID1_BIT;
    uint32_t efuse_capid2 = (efuse_capid & DCXO_EFUSE_CAPID2_MASK) >> DCXO_EFUSE_CAPID2_BIT;

    if(efuse_capid2 != 0){
        DBGMSG(LOG_DEBUG, "read EFUSE CAPID2 = %d \r\n", efuse_capid2);
        return efuse_capid2;
    }else if(efuse_capid1 != 0){
        DBGMSG(LOG_DEBUG, "read EFUSE CAPID1 = %d \r\n", efuse_capid1);
        return efuse_capid1;
    }else if(efuse_capid0 != 0){
        DBGMSG(LOG_DEBUG, "read EFUSE CAPID0 = %d \r\n", efuse_capid0);
        return efuse_capid0;
    }else {
        DBGMSG(LOG_DEBUG, "EFUSE CAPID not exists , read golden value\r\n");
        return DCXO_CAPID_GOLDEN_RF_VALUE;
    }
}
#endif  /* DCXO_CAPID_LOAD_FROM_EFUSE */

static void set_capid(uint32_t target_capid, uint32_t current_capid)
{
    uint32_t dcxo_cap_id = 0;
    int32_t  duration = target_capid - current_capid;
    int32_t temp_capid = current_capid;

    if(duration > 0){
        /* increase CAPID */
        DBGMSG(LOG_DEBUG, "increase CAPID from %lu to %lu \r\n", current_capid, target_capid);
        while (duration > 0) {
            /* calculate temp capid */
            duration -= 4;
            temp_capid +=4;
            if(temp_capid > target_capid){
                temp_capid = target_capid;
            }

            /* wait 4 us */
            hal_gpt_delay_us(4);
            /* set new val */
            dcxo_cap_id = *DCXO_CAP_ID;
            dcxo_cap_id &= (~DCXO_CAP_ID_MASK);
            dcxo_cap_id |= (temp_capid << DCXO_CAP_ID_BIT);
            *DCXO_CAP_ID = dcxo_cap_id;
        }
    }else{
        /* decrease CAPID */
        DBGMSG(LOG_DEBUG, "decrease CAPID from %lu to %lu \r\n", current_capid, target_capid);
        while (duration < 0) {
            /* calculate temp capid */
            duration += 4;
            temp_capid -= 4;
            if(temp_capid < target_capid){
                temp_capid = target_capid;
            }

            if( temp_capid < 0 ) {
                temp_capid = (int32_t)target_capid;
            }

            /* wait 4 us */
            hal_gpt_delay_us(4);
            /* set new val */
            dcxo_cap_id = *DCXO_CAP_ID;
            dcxo_cap_id &= (~DCXO_CAP_ID_MASK);
            dcxo_cap_id |= (temp_capid << DCXO_CAP_ID_BIT);
            *DCXO_CAP_ID = dcxo_cap_id;
        }
    }
    /* record current capid */
    g_current_capid = target_capid;
}

#ifdef DCXO_CAPID_CAL
static uint32_t f32k_measure_clock_cal(uint32_t window)
{
    uint32_t target_freq = 0;

    *ABIST_FQMTR_CON0 = 0xC000; // CKSYS_BASE (0XA2020000)+ 0x0400, to reset meter
    hal_gpt_delay_us(1);
    while ((*ABIST_FQMTR_CON1 & 0x8000) != 0); // CKSYS_BASE (0XA2020000)+ 0x0404, wait busy

    //CKSYS_BASE (0XA2020000)+ 0x0224, [10:8] fixed_clock, [4:0] tested_clock
    *CKSYS_TST_SEL_1_F_TCKSEL = 1;      //f_fxo_ck
    *CKSYS_TST_SEL_1_F_FCKSEL = 6;      //XOSC32K_CK

    *ABIST_FQMTR_CON0 = window-1; // CKSYS_BASE (0XA2020000)+ 0x0400, set window 4096 T
    *ABIST_FQMTR_CON0 = (0x8000|(window-1)); // CKSYS_BASE (0XA2020000)+ 0x0400, to enable meter
    //*ABIST_FQMTR_CON0 = 0x8063; // CKSYS_BASE (0XA2020000)+ 0x0400, to enable meter, window 100T

    hal_gpt_delay_us(1000); //wait meter start

    while ((*ABIST_FQMTR_CON1 & 0x8000) != 0); // CKSYS_BASE (0XA2020000)+ 0x0404, wait busy

    /* fqmtr_ck = fixed_ck*fqmtr_data/winset, */
    target_freq = *PLL_ABIST_FQMTR_DATA__F_FQMTR_DATA; // CKSYS_BASE (0XA2020000)+ 0x040C, read meter data

    return target_freq;

}
#endif  /* DCXO_CAPID_CAL */

#ifdef DCXO_RFIC_CALIBRATION
void hal_dcxo_factory_calibration(void)
{
    *DCXO_CAP_ID = (*DCXO_CAP_ID) & DCXO_CAP_ID_MASK;
}
#else
void hal_dcxo_factory_calibration(void)
{
#ifdef DCXO_CAPID_CAL
    uint32_t cal_capid = 0;
    int i = 8;
    uint32_t fqmtr_data;

    DBGMSG(LOG_DEBUG, "Start DCXO Calibration.\r\n");

#ifdef DCXO_DUMP_CAPID
    /* check 1 ~ 511 freq meter */
    uint32_t test_val = 0;
    for(test_val = 1;test_val <= 511;test_val += 10) {
        set_capid(test_val,g_current_capid);
        fqmtr_data = f32k_measure_clock_cal(1200);

        DBGMSG(LOG_DEBUG, "dcxo check capid = %u (%d) , fqmtr = %d \r\n", test_val, test_val, fqmtr_data);
    }
#endif  /* DCXO_DUMP_CAPID */

    for(i = 8;i >= 0;i--){
        cal_capid = cal_capid + (1<<i);
        set_capid(cal_capid,g_current_capid);
        fqmtr_data = f32k_measure_clock_cal(1200);
        DBGMSG(LOG_DEBUG, "dump capid = %u (%d) , fqmtr = %d \r\n", cal_capid, cal_capid, fqmtr_data);
        if (fqmtr_data == 952148) {
            DBGMSG(LOG_DEBUG, "target fqmtr 952148 found \r\n");
            break;
        }
        if (fqmtr_data > 952148) {
            cal_capid = cal_capid - (1<<i);
        }
        /* wait 1 ms */
        hal_gpt_delay_us(1000);
    }

    DBGMSG(LOG_DEBUG, "final cal capid = %d , fqmtr = %d \r\n",cal_capid,fqmtr_data);

    if(cal_capid == 0){
        DBGMSG(LOG_DEBUG, "final cal capid = 0 , change to 1 \r\n");
        cal_capid = 1;
    }

    /* write cal_capid to EFUSE */
#if defined(DCXO_CAPID_STORE_TO_EFUSE0) || defined(DCXO_CAPID_STORE_TO_EFUSE1) || defined(DCXO_CAPID_STORE_TO_EFUSE2)
    write_efuse_capid(cal_capid);
#else
    DBGMSG(LOG_DEBUG, "Please write the calibrated CAPID (%d) in SW, re-build, and re-download the main bin \r\n", cal_capid);
    while(1);
#endif  /* DCXO_CAPID_STORE_TO_EFUSE0 or DCXO_CAPID_STORE_TO_EFUSE1 or DCXO_CAPID_STORE_TO_EFUSE2 */

#endif  /* DCXO_CAPID_CAL */
}
#endif

void hal_dcxo_load_calibration(void)
{
    uint32_t expect_capid = 0;

    hal_dcxo_capid_init();

    /* read calibration from efuse or golden K val */
#ifdef DCXO_CAPID_LOAD_FROM_EFUSE
    DBGMSG(LOG_DEBUG, "Load CAPID from eFuse ...  \r\n");
    expect_capid = read_efuse_capid();
#else
#ifdef MTK_NVDM_MODEM_ENABLE
    nvdm_status_t status;
    nvdm_modem_data_item_type_t type;
    N1RfAfcCalData data;
    uint32_t size = sizeof(N1RfAfcCalData);

    status = nvdm_modem_read_normal_data_item("N1RF_CAL_DATA", "AFC", &type, (uint8_t *)&data, &size);
    if (status == NVDM_STATUS_OK)
        expect_capid = (uint32_t)data.cap_id;
#endif

    if ((expect_capid > 0) && (expect_capid < 512)) {
        DBGMSG(LOG_DEBUG, "Load CAPID from NVRAM ...  \r\n");
    } else {
        DBGMSG(LOG_DEBUG, "Load CAPID from golden value ...  \r\n");
        expect_capid = DCXO_CAPID_GOLDEN_RF_VALUE;
    }
#endif  /* DCXO_CAPID_LOAD_FROM_EFUSE */

    DBGMSG(LOG_DEBUG, "Expect CAPID = %lu \r\n", expect_capid);

    if (expect_capid == 0) {
        DBGMSG(LOG_DEBUG, "Error!!! Expect CAPID == 0!!!\r\n");
        while(1);
    }
    set_capid(expect_capid, g_current_capid);

    DBGMSG(LOG_DEBUG, "now CAPID = %lu \r\n", ((*DCXO_CAP_ID) & DCXO_CAP_ID_MASK) >> DCXO_CAP_ID_BIT);
}

//#define DCXO_TEST

#ifdef DCXO_TEST

#include "hal_gpio.h"
#include "hal_sleep_manager.h"

#define DCXO_gpio_GPIO_MODE_G           ((volatile uint8_t*)(0xA20B0068))
#define GPIO_CLKO_CTRL_A_F_CLKO_MODE0   ((volatile uint8_t*)(0xA2010000))
#define GPIO_CLKO_CTRL_A_F_CLKO_MODE3   ((volatile uint8_t*)(0xA2010003))
#define DEBUG_MON_DCXO                  ((volatile uint32_t*)(0xA2060100))
#define TOP_DEBUG                       ((volatile uint32_t*)(0xA2010060))
#define CM4_INT_SET_ENABLE_0            ((volatile uint32_t*)(0xE000E100))
#define CM4_SYSTEM_CONTROL              ((volatile uint32_t*)(0xE000ED10))
#define CM4_SYSTICK_CTL                 ((volatile uint32_t*)(0xE000E010))
#define CM4_SYSTICK_RELOAD_VALUE        ((volatile uint32_t*)(0xE000E014))

#define __ENTER_IDLE() __asm volatile( \
    "DSB                                     \n"\
    "WFI                                     \n"\
    "ISB                                     \n"\
)

/*
 __asm volatile(code:output:intput:clobbers);
*/
static void __CPU_DELAY(uint32_t ptr)
{
    __asm volatile(
        "PUSH	{r0, lr}                    \n"
        "MOV    r0,         %0              \n"
        "LOOP:  SUBS    r0,r0,#1            \n"
        "CMP    r0,         0               \n"
        "BNE    LOOP                        \n"
        "POP	{r0, pc}                    \n"
        : :"r" (ptr):
    );
}

#define DCXO_MON_SEL    1   // 0, 1

void dcxo_test(void)
{
    uint32_t val = 0;
    log_hal_info("0xA2010040 = 0x%08x\r\n", *((volatile uint32_t*)(0xA2010040)));   //bit 5, 1->32k-less
    log_hal_info("DCXO_gpio_GPIO_MODE_G = 0x%08x\r\n", *DCXO_gpio_GPIO_MODE_G);
    log_hal_info("DCXO test start:\r\n");

#if 0   //GPIO TEST
    //////////////////////////////////

    hal_gpio_init(HAL_GPIO_8);
    hal_pinmux_set_function(HAL_GPIO_8, 0);

    hal_gpio_init(HAL_GPIO_9);
    hal_pinmux_set_function(HAL_GPIO_9, 0);

    hal_gpio_init(HAL_GPIO_10);
    hal_pinmux_set_function(HAL_GPIO_9, 0);

    hal_gpio_init(HAL_GPIO_11);
    hal_pinmux_set_function(HAL_GPIO_11, 0);

    hal_gpio_init(HAL_GPIO_12);
    hal_pinmux_set_function(HAL_GPIO_12, 0);

    hal_gpio_init(HAL_GPIO_13);
    hal_pinmux_set_function(HAL_GPIO_13, 0);

    hal_gpio_init(HAL_GPIO_14);
    hal_pinmux_set_function(HAL_GPIO_14, 0);

    hal_gpio_init(HAL_GPIO_32);
    hal_pinmux_set_function(HAL_GPIO_32, 0);

    hal_gpio_set_direction(HAL_GPIO_8, HAL_GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_direction(HAL_GPIO_9, HAL_GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_direction(HAL_GPIO_10, HAL_GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_direction(HAL_GPIO_11, HAL_GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_direction(HAL_GPIO_12, HAL_GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_direction(HAL_GPIO_13, HAL_GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_direction(HAL_GPIO_14, HAL_GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_direction(HAL_GPIO_32, HAL_GPIO_DIRECTION_OUTPUT);

    hal_gpio_set_output(HAL_GPIO_12, 1);
    hal_gpio_set_output(HAL_GPIO_8, 1);
    hal_gpio_set_output(HAL_GPIO_9, 1);
    hal_gpio_set_output(HAL_GPIO_10, 1);
    hal_gpio_set_output(HAL_GPIO_11, 1);
    hal_gpio_set_output(HAL_GPIO_13, 1);
    hal_gpio_set_output(HAL_GPIO_14, 1);
    hal_gpio_set_output(HAL_GPIO_32, 1);
    __CPU_DELAY(0xFFFF);
    hal_gpio_set_output(HAL_GPIO_12, 0);
    hal_gpio_set_output(HAL_GPIO_8, 0);
    hal_gpio_set_output(HAL_GPIO_9, 0);
    hal_gpio_set_output(HAL_GPIO_10, 0);
    hal_gpio_set_output(HAL_GPIO_11, 0);
    hal_gpio_set_output(HAL_GPIO_13, 0);
    hal_gpio_set_output(HAL_GPIO_14, 0);
    hal_gpio_set_output(HAL_GPIO_32, 0);
    __CPU_DELAY(0xFFFF);
    hal_gpio_set_output(HAL_GPIO_12, 1);
    hal_gpio_set_output(HAL_GPIO_8, 1);
    hal_gpio_set_output(HAL_GPIO_9, 1);
    hal_gpio_set_output(HAL_GPIO_10, 1);
    hal_gpio_set_output(HAL_GPIO_11, 1);
    hal_gpio_set_output(HAL_GPIO_13, 1);
    hal_gpio_set_output(HAL_GPIO_14, 1);
    hal_gpio_set_output(HAL_GPIO_32, 1);
    __CPU_DELAY(0xFFFF);
    hal_gpio_set_output(HAL_GPIO_12, 0);
    hal_gpio_set_output(HAL_GPIO_8, 0);
    hal_gpio_set_output(HAL_GPIO_9, 0);
    hal_gpio_set_output(HAL_GPIO_10, 0);
    hal_gpio_set_output(HAL_GPIO_11, 0);
    hal_gpio_set_output(HAL_GPIO_13, 0);
    hal_gpio_set_output(HAL_GPIO_14, 0);
    hal_gpio_set_output(HAL_GPIO_32, 0);
    __CPU_DELAY(0xFFFF);
    hal_gpio_set_output(HAL_GPIO_12, 1);
    hal_gpio_set_output(HAL_GPIO_8, 1);
    hal_gpio_set_output(HAL_GPIO_9, 1);
    hal_gpio_set_output(HAL_GPIO_10, 1);
    hal_gpio_set_output(HAL_GPIO_11, 1);
    hal_gpio_set_output(HAL_GPIO_13, 1);
    hal_gpio_set_output(HAL_GPIO_14, 1);
    hal_gpio_set_output(HAL_GPIO_32, 1);
    __CPU_DELAY(0xFFFF);
    hal_gpio_set_output(HAL_GPIO_12, 0);
    hal_gpio_set_output(HAL_GPIO_8, 0);
    hal_gpio_set_output(HAL_GPIO_9, 0);
    hal_gpio_set_output(HAL_GPIO_10, 0);
    hal_gpio_set_output(HAL_GPIO_11, 0);
    hal_gpio_set_output(HAL_GPIO_13, 0);
    hal_gpio_set_output(HAL_GPIO_14, 0);
    hal_gpio_set_output(HAL_GPIO_32, 0);
    __CPU_DELAY(0xFFFF);
    hal_gpio_set_output(HAL_GPIO_12, 1);
    hal_gpio_set_output(HAL_GPIO_8, 1);
    hal_gpio_set_output(HAL_GPIO_9, 1);
    hal_gpio_set_output(HAL_GPIO_10, 1);
    hal_gpio_set_output(HAL_GPIO_11, 1);
    hal_gpio_set_output(HAL_GPIO_13, 1);
    hal_gpio_set_output(HAL_GPIO_14, 1);
    hal_gpio_set_output(HAL_GPIO_32, 1);
    __CPU_DELAY(0xFFFF);
    hal_gpio_set_output(HAL_GPIO_12, 0);
    hal_gpio_set_output(HAL_GPIO_8, 0);
    hal_gpio_set_output(HAL_GPIO_9, 0);
    hal_gpio_set_output(HAL_GPIO_10, 0);
    hal_gpio_set_output(HAL_GPIO_11, 0);
    hal_gpio_set_output(HAL_GPIO_13, 0);
    hal_gpio_set_output(HAL_GPIO_14, 0);
    hal_gpio_set_output(HAL_GPIO_32, 0);

    //////////////////////////////////
#else
    *GPIO_CLKO_CTRL_A_F_CLKO_MODE0 = 0;     //      4'd0:  f_frtc_clkout_ck
    *GPIO_CLKO_CTRL_A_F_CLKO_MODE3 = 1;     //      4'd1:  f_fxo_ck

    // GPIO0 aux. 5 for O:CLKO0
    hal_gpio_init(HAL_GPIO_0);
    hal_pinmux_set_function(HAL_GPIO_0, 5);

    // GPIO24 aux. 5 for O:CLKO3
    hal_gpio_init(HAL_GPIO_24);
    hal_pinmux_set_function(HAL_GPIO_24, 5);

    // GPIO48 aux. 1 for SRCLKENAI
    hal_gpio_init(HAL_GPIO_48);
    hal_pinmux_set_function(HAL_GPIO_48, 1);

    // GPIO8 aux. 8 for DEBUGMON8
    hal_gpio_init(HAL_GPIO_8);
    hal_pinmux_set_function(HAL_GPIO_8, 8);

    // GPIO9 aux. 8 for DEBUGMON9
    hal_gpio_init(HAL_GPIO_9);
    hal_pinmux_set_function(HAL_GPIO_9, 8);

    // GPIO10 aux. 8 for DEBUGMON10
    hal_gpio_init(HAL_GPIO_10);
    hal_pinmux_set_function(HAL_GPIO_10, 8);

    // GPIO11 aux. 8 for DEBUGMON11
    hal_gpio_init(HAL_GPIO_11);
    hal_pinmux_set_function(HAL_GPIO_11, 8);

    // GPIO12 aux. 8 for DEBUGMON12
    hal_gpio_init(HAL_GPIO_12);
    hal_pinmux_set_function(HAL_GPIO_12, 8);

    // GPIO13 aux. 8 for DEBUGMON13
    hal_gpio_init(HAL_GPIO_13);
    hal_pinmux_set_function(HAL_GPIO_13, 8);

    // GPIO14 aux. 8 for DEBUGMON14
    hal_gpio_init(HAL_GPIO_14);
    hal_pinmux_set_function(HAL_GPIO_14, 8);

    // GPIO15 aux. 8 for DEBUGMON15
    hal_gpio_init(HAL_GPIO_15);
    hal_pinmux_set_function(HAL_GPIO_15, 8);

    // enable debug monitor for idle signal
    *TOP_DEBUG = 0x12;

    val = *DEBUG_MON_DCXO;
    val &= ~(0x00030000);
    val |= (DCXO_MON_SEL << 16);    //  mon_sel, bit[16:17]
    val |= (0x1);   //  mon_en, bit[0]
    *DEBUG_MON_DCXO = val;

#if 0
    log_hal_info("0X%08X\r\n", *DEBUG_MON_DCXO);
    //dump 0xA2060000~A2060018
    log_hal_info("0XA2060000 = 0x%08X:\r\n", *((volatile uint32_t*)(0xA2060000)));
    log_hal_info("0XA2060004 = 0x%08X:\r\n", *((volatile uint32_t*)(0XA2060004)));
    log_hal_info("0XA2060008 = 0x%08X:\r\n", *((volatile uint32_t*)(0XA2060008)));
    log_hal_info("0XA206000C = 0x%08X:\r\n", *((volatile uint32_t*)(0XA206000C)));
    log_hal_info("0XA2060010 = 0x%08X:\r\n", *((volatile uint32_t*)(0XA2060010)));
    log_hal_info("0XA2060014 = 0x%08X:\r\n", *((volatile uint32_t*)(0XA2060014)));
    log_hal_info("0XA2060018 = 0x%08X:\r\n", *((volatile uint32_t*)(0XA2060018)));
#endif
    //__CPU_DELAY(0xFFFF);
    __asm volatile("cpsid i");
    hal_sleep_manager_set_sleep_time(20000);
    //__CPU_DELAY(0xFFFFFF);
    hal_sleep_manager_enter_sleep_mode(HAL_SLEEP_MODE_LIGHT_SLEEP);
    __asm volatile("cpsie i");
#endif
    //log_hal_info("DCXO test end\r\n");
}

#endif  //DCXO_TEST
