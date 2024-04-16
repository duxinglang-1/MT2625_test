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

#include "hal_clock.h"
#include "hal_clock_internal.h"

#ifdef HAL_CLOCK_MODULE_ENABLED

#include "hal_nvic_internal.h"

//#include <stdio.h>
#include <assert.h>
#include "hal_log.h"
#include "hal_gpt.h"

//#define CLK_DCM_TEST
//#define CLK_SWITCH_TEST
//#define CLK_REALTIME_TEST
//#define CLK_HOPPING_TEST

//#define CLOCK_DEBUG 1

#define USB_USE_UPLL

#define DOWN_HOPPING_LIMIT  7
#define SOFT_START_HOPPING_TARGET  1

ATTR_RWDATA_IN_TCM clock_vcore_voltage vcore_voltage = CLK_VCORE_1P1V;

/*************************************************************************
 * Clock mux select API definition part
 * 1. Enable individual clock divider
 * 2. Force clock on th prevent clock can't switch to target clock
 * 3. Set CSW to target clock freq. and set change bit
 * 4. After clock change to target freq. Change bit will be cleared to0 and release clock gating
 * 5. Disable forced on clock
 *************************************************************************/
ATTR_TEXT_IN_TCM hal_clock_status_t clock_mux_sel(clock_mux_sel_id mux_id, uint32_t mux_sel)
{
    uint32_t pll1_en = 0;
    uint32_t pll2_en = 0;
    volatile uint8_t * sel = NULL;
    volatile uint8_t * chg = NULL;
    volatile uint8_t * chg_ok = NULL;
    volatile uint8_t * force_on = NULL;
//  volatile uint8_t * force_off = NULL;

    if (mux_id < NR_MUXS) {
        /* TODO do something relations setting for mux_sel */
        switch (mux_id) {
            case CLK_SYS_SEL:
                if (mux_sel == 0) { /* XO_CK, 26 MHz */
                } else if (mux_sel == 1) { /* MPLL2_D2_D2, 156 MHz */
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D2_EN = 1;
                    pll2_en = 1;
                } else if (mux_sel == 2) { /* MPLL2_D5, 124.8 MHz */
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D5_EN = 1;
                    pll2_en = 1;
                } else if (mux_sel == 3) { /* MPLL2_D3_D2, 104 MHz */
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D3_EN = 1;
                    pll2_en = 1;
                } else if (mux_sel == 4) { /* MPLL2_D7, 89.14 MHz */
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D7_EN = 1;
                    pll2_en = 1;
                } else if (mux_sel == 5) { /* MPLL2_D2_D4, 78 MHz */
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D2_EN = 1;
                    pll2_en = 1;
                }
                sel = CKSYS_CLK_CFG_0__F_CLK_SYS_SEL;
                force_on = CKSYS_CLK_FORCE_ON_0__F_CLK_SYS_FORCE_ON;
                chg = CKSYS_CLK_UPDATE_0__F_CHG_SYS;
                chg_ok = CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SYS_OK;
                break;

            case CLK_SFC_SEL:
                if (mux_sel == 0) { /* XO_CK, 26 MHz */
                } else if (mux_sel == 1) { /* MPLL2_D2_D4, 78 MHz*/
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D2_EN = 1;
                    pll2_en = 1;
                } else if (mux_sel == 2) { /* MPLL2_D5_D2, 62.4 MHz*/
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D5_EN = 1;
                    pll2_en = 1;
                } else if (mux_sel == 3) { /* MPLL2_48M_CK, 48 MHz */
                    pll2_en = 1;
                }
                sel = CKSYS_CLK_CFG_0__F_CLK_SFC_SEL;
                force_on = CKSYS_CLK_FORCE_ON_0__F_CLK_SFC_FORCE_ON;
                chg = CKSYS_CLK_UPDATE_0__F_CHG_SFC;
                chg_ok = CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SFC_OK;
                break;

            case CLK_DSP_SEL:
                if (mux_sel == 0) { /* XO_CK, 26 MHz */
                } else if (mux_sel == 1) { /* MPLL2_D5, 124.8 MHz */
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D5_EN = 1;
                    pll2_en = 1;
                } else if (mux_sel == 2) { /* MPLL2_D3_D2, 104 MHz */
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D3_EN = 1;
                    pll2_en = 1;
                } else if (mux_sel == 3) { /* MPLL2_D2_D4, 78 MHz */
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D2_EN = 1;
                    pll2_en = 1;
                } else if (mux_sel == 4) { /* MPLL2_D3_D4, 52 MHz */
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D3_EN = 1;
                    pll2_en = 1;
                }
                sel = CKSYS_CLK_CFG_0__F_CLK_DSP_SEL;
                force_on = CKSYS_CLK_FORCE_ON_0__F_CLK_DSP_FORCE_ON;
                chg = CKSYS_CLK_UPDATE_0__F_CHG_DSP;
                chg_ok = CKSYS_CLK_UPDATE_STATUS_0__F_CHG_DSP_OK;
                break;

            case CLK_SPIMST_SEL:
                if (mux_sel == 0) { /* XO_CK, 26 MHz */
                } else if (mux_sel == 1) { /* MPLL2_D3_D4, 52 MHz*/
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D3_EN = 1;
                    pll2_en = 1;
                } else if (mux_sel == 2) { /* MPLL2_D2_D4, 78 MHz*/
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D2_EN = 1;
                    pll2_en = 1;
                } else if (mux_sel == 3) { /* MPLL2_D3_D2, 104 MHz */
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D3_EN = 1;
                    pll2_en = 1;
                }
                sel = CKSYS_CLK_CFG_0__F_CLK_SPIMST_SEL;
                force_on = CKSYS_CLK_FORCE_ON_0__F_CLK_SPIMST_FORCE_ON;
                chg = CKSYS_CLK_UPDATE_0__F_CHG_SPIMST;
                chg_ok = CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SPIMST_OK;
                break;

            case CLK_BSIBPI_SEL:
                if (mux_sel == 0) { /* XO_CK, 26 MHz */
                } else if (mux_sel == 1) { /* MPLL2_D5, 124.8 MHz*/
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D5_EN = 1;
                    pll2_en = 1;
                } else if (mux_sel == 2) { /* MPLL2_D5_D2, 62.4 MHz*/
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D5_EN = 1;
                    pll2_en = 1;
                }
                sel = CKSYS_CLK_CFG_1__F_CLK_BSIBPI_SEL;
                force_on = CKSYS_CLK_FORCE_ON_1__F_CLK_BSIBPI_FORCE_ON;
                chg = CKSYS_CLK_UPDATE_1__F_CHG_BSIBPI;
                chg_ok = CKSYS_CLK_UPDATE_STATUS_1__F_CHG_BSIBPI_OK;
                break;

            case CLK_SDIOMST0_SEL:
                if (mux_sel == 0) { /* XO_CK, 26 MHz */
                } else if (mux_sel == 1) { /* MPLL2_48M_CK, 48 MHz*/
                    pll2_en = 1;
                } else if (mux_sel == 2) { /* MPLL2_D7_D2, 44.57 MHz*/
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D7_EN = 1;
                    pll2_en = 1;
                } else if (mux_sel == 3) { /* MPLL2_D3_D4, 52 MHz */
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D3_EN = 1;
                    pll2_en = 1;
                }
                sel = CKSYS_CLK_CFG_1__F_CLK_SDIOMST0_SEL;
                force_on = CKSYS_CLK_FORCE_ON_1__F_CLK_SDIOMST0_FORCE_ON;
                chg = CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST0;
                chg_ok = CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SDIOMST0_OK;
                break;

            case CLK_SDIOMST1_SEL:
                if (mux_sel == 0) { /* XO_CK, 26 MHz */
                } else if (mux_sel == 1) { /* MPLL2_48M_CK, 48 MHz*/
                    pll2_en = 1;
                } else if (mux_sel == 2) { /* MPLL2_D7_D2, 44.57 MHz*/
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D7_EN = 1;
                    pll2_en = 1;
                } else if (mux_sel == 3) { /* MPLL2_D3_D4, 52 MHz */
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D3_EN = 1;
                    pll2_en = 1;
                }
                sel = CKSYS_CLK_CFG_1__F_CLK_SDIOMST1_SEL;
                force_on = CKSYS_CLK_FORCE_ON_1__F_CLK_SDIOMST1_FORCE_ON;
                chg = CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST1;
                chg_ok = CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SDIOMST1_OK;
                break;

            case CLK_SPISLV_SEL:
                if (mux_sel == 0) { /* XO_CK, 26 MHz */
                } else if (mux_sel == 1) { /* MPLL2_D3_D4, 52 MHz*/
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D3_EN = 1;
                    pll2_en = 1;
                } else if (mux_sel == 2) { /* MPLL2_D2_D4, 78 MHz*/
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D2_EN = 1;
                    pll2_en = 1;
                } else if (mux_sel == 3) { /* MPLL2_D3_D2, 104 MHz */
                    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D3_EN = 1;
                    pll2_en = 1;
                }
                sel = CKSYS_CLK_CFG_1__F_CLK_SPISLV_SEL;
                force_on = CKSYS_CLK_FORCE_ON_1__F_CLK_SPISLV_FORCE_ON;
                chg = CKSYS_CLK_UPDATE_1__F_CHG_SPISLV;
                chg_ok = CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SPISLV_OK;
                break;

            case CLK_48M_SEL:
                *XO_PDN_SETD0 = 0x1; // CKSYS_XO_CLK_BASE (0XA2030000)+ 0x0b10, turn off 48M CG to prevent glitch
                if (mux_sel == 0) { /* XO_CK, 26 MHz */
                } else if (mux_sel == 1) { /* UPLL1_48M_CK, 48 MHz*/
                    pll1_en = 1;
                } else if (mux_sel == 2) { /* MPLL2_48M_CK, 48 MHz*/
                    pll2_en = 1;
                }
                sel = CKSYS_CLK_CFG_2__F_CLK_48m_SEL;
                break;

            default:
                break;
        }

        if (pll1_en != 0) {
            *CKSYS_CLK_DIV_2__F_CLK_PLL1_DIV_EN = 1;
        }
        if (pll2_en != 0) {
            *CKSYS_CLK_DIV_2__F_CLK_PLL2_DIV_EN = 1;
        }

        if (sel) {
            *sel = mux_sel;
        }
        if (force_on) {
            *force_on = 1;
        }
        if (chg) {
            *chg = 1;
             while (*chg == 1);
        }
        if (chg_ok) {
            while (*chg_ok == 1);
        }
        if (force_on) {
            *force_on = 0;
        }

        return HAL_CLOCK_STATUS_OK;
    } else {
        /* TODO return fail id information */
#ifdef CLK_DEBUG_ERR
        log_hal_error("%s, error mux id(%d) sel(%d)\r\n", __FUNCTION__, mux_id, mux_sel);
#endif
        return HAL_CLOCK_STATUS_INVALID_PARAMETER;
    }
}

ATTR_TEXT_IN_TCM int8_t clock_mux_get_state(clock_mux_sel_id mux_id)
{
    if (mux_id < NR_MUXS) {
        /* TODO do something relations setting for mux_sel */
#ifdef CLK_DEBUG
        log_hal_info("%s\n", __FUNCTION__);
#endif
        switch (mux_id) {
            case CLK_SYS_SEL:
                return *CKSYS_CLK_CFG_0__F_CLK_SYS_SEL;
            case CLK_SFC_SEL:
                return *CKSYS_CLK_CFG_0__F_CLK_SFC_SEL;
            case CLK_DSP_SEL:
                return *CKSYS_CLK_CFG_0__F_CLK_DSP_SEL;
            case CLK_SPIMST_SEL:
                return *CKSYS_CLK_CFG_0__F_CLK_SPIMST_SEL;
            case CLK_BSIBPI_SEL:
                return *CKSYS_CLK_CFG_1__F_CLK_BSIBPI_SEL;
            case CLK_SDIOMST0_SEL:
                return *CKSYS_CLK_CFG_1__F_CLK_SDIOMST0_SEL;
            case CLK_SDIOMST1_SEL:
                return *CKSYS_CLK_CFG_1__F_CLK_SDIOMST1_SEL;
            case CLK_SPISLV_SEL:
                return *CKSYS_CLK_CFG_1__F_CLK_SPISLV_SEL;
            case CLK_48M_SEL:
                return *CKSYS_CLK_CFG_2__F_CLK_48m_SEL;
            default:
                return HAL_CLOCK_STATUS_INVALID_PARAMETER;
        }
    } else {
        /* TODO return fail id information */
#ifdef CLK_DEBUG_ERR
        log_hal_error("%s, error mux id(%d)\r\n", __FUNCTION__, mux_id);
#endif
        return HAL_CLOCK_STATUS_INVALID_PARAMETER;
    }
}

struct cg_clock {
    uint32_t cnt;
};

ATTR_ZIDATA_IN_TCM static struct cg_clock clocks[NR_CLOCKS];

/*************************************************************************
 * CG Clock API definition
 *************************************************************************/
ATTR_TEXT_IN_TCM hal_clock_status_t hal_clock_enable(hal_clock_cg_id clock_id)
{
    hal_clock_status_t ret = HAL_CLOCK_STATUS_OK;
    uint32_t irq_mask = save_and_set_interrupt_mask();  /* disable interrupt */
    volatile uint32_t * clr_addr = NULL;
    volatile uint32_t * sta_addr = NULL;
    uint32_t bit_idx = clock_id%32;

    (void)sta_addr;

#ifdef CLK_DEBUG
    /* TODO cannot print log before log_hal_info init done */
    log_hal_info("%s: clock_id=%d\r\n", __FUNCTION__, clock_id);
#endif /* ifdef CLK_DEBUG */

    if (clock_id <= HAL_CLOCK_CG_SDIOMST1_BUS) {
        sta_addr = PDN_COND0_F_PDR_COND0;
        if (clocks[clock_id].cnt == 0) {
            clr_addr = PDN_CLRD0_F_PDR_CLRD0;
            *(clr_addr) = (0x1 << bit_idx);    /* HW Register is write 1 clear */
        }
        if (clocks[clock_id].cnt < 32767) {
            clocks[clock_id].cnt++;
        }
    } else if ((clock_id >= HAL_CLOCK_CG_48M) && (clock_id <= HAL_CLOCK_CG_MD_XO)) {
        sta_addr = XO_PDN_COND0;
        if (clocks[clock_id].cnt == 0) {
            clr_addr = XO_PDN_CLRD0;
            *(clr_addr) = (0x1 << bit_idx);    /* HW Register is write 1 clear */
        }
        if (clocks[clock_id].cnt < 32767) {
            clocks[clock_id].cnt++;
        }
    } else {
        ret = HAL_CLOCK_STATUS_INVALID_PARAMETER;
    }

#ifdef CLK_DEBUG
    log_hal_info("ref_cnt = %d, @0x%x = %x\r\n", clocks[clock_id].cnt, sta_addr, *(sta_addr));
#endif

    restore_interrupt_mask(irq_mask);  /* restore interrupt */
    return ret;
}

ATTR_TEXT_IN_TCM hal_clock_status_t hal_clock_disable(hal_clock_cg_id clock_id)
{
    hal_clock_status_t ret = HAL_CLOCK_STATUS_OK;
    uint32_t irq_mask = save_and_set_interrupt_mask();  /* disable interrupt */
    volatile uint32_t * set_addr = NULL;
    volatile uint32_t * sta_addr = NULL;
    uint32_t bit_idx = clock_id%32;

    (void)sta_addr;

#ifdef CLK_DEBUG
    /* TODO cannot print log before log_hal_info init done */
    log_hal_info("%s: clock_id=%d\n", __FUNCTION__, clock_id);
#endif


    if (clock_id<=HAL_CLOCK_CG_SDIOMST1_BUS) { /* >=HAL_CLOCK_CG_DMA is true for comparison to unsigned zero */
        sta_addr = PDN_COND0_F_PDR_COND0;
        if (clocks[clock_id].cnt > 0) {
            clocks[clock_id].cnt--;
        }
        if (clocks[clock_id].cnt == 0) {
            set_addr = PDN_SETD0_F_PDR_SETD0;
            *(set_addr) = (0x1 << bit_idx);    /* HW Register is write 1 clear */
        }
    } else if ((clock_id>=HAL_CLOCK_CG_48M) && (clock_id<=HAL_CLOCK_CG_MD_XO)) {
        sta_addr = XO_PDN_COND0;
        if (clocks[clock_id].cnt > 0) {
            clocks[clock_id].cnt--;
        }
        if (clocks[clock_id].cnt == 0) {
            set_addr = XO_PDN_SETD0;
            *(set_addr) = (0x1 << bit_idx);    /* HW Register is write 1 clear */
        }
    } else {
        ret = HAL_CLOCK_STATUS_INVALID_PARAMETER;
    }

    /* TODO cannot use log_hal_info print log before syslog init */
#ifdef CLK_DEBUG
    log_hal_info("ref_cnt = %d, @0x%x = %x\n", clocks[clock_id].cnt, sta_addr, *(sta_addr));
#endif

    restore_interrupt_mask(irq_mask);  /* restore interrupt */
    return ret;
}

ATTR_TEXT_IN_TCM bool hal_clock_is_enabled(hal_clock_cg_id clock_id)
{
    uint32_t bit_idx = clock_id%32;
    volatile uint32_t * sta_addr = NULL;

    if (clock_id<=HAL_CLOCK_CG_SDIOMST1_BUS) { /* >=HAL_CLOCK_CG_DMA is true for comparison to unsigned zero */
        sta_addr = PDN_COND0_F_PDR_COND0;
    } else if ((clock_id>=HAL_CLOCK_CG_48M) && (clock_id<=HAL_CLOCK_CG_MD_XO)) {
        sta_addr = XO_PDN_COND0;
    } else {
        return false;
    }

    if (((*sta_addr) & (0x1 << bit_idx)) != 0x0) {
        /* TODO cannot use log_hal_info print log before syslog init */
#ifdef CLK_DEBUG
        log_hal_info("%s: %d: bit = %d: clock is disabled\n", __FUNCTION__, clock_id, bit_idx);
#endif
        return false;
    } else {
        /* TODO cannot use log_hal_info print log before syslog init */
#ifdef CLK_DEBUG
        log_hal_info("%s: %d: bit = %d: clock is enabled\n", __FUNCTION__, clock_id, bit_idx);
#endif
        return true;
    }
}

/*************************************************************************
 * Suspend/Resume API definition
 *************************************************************************/
ATTR_TEXT_IN_TCM hal_clock_status_t clock_suspend(void)
{
    return HAL_CLOCK_STATUS_OK;
}

ATTR_TEXT_IN_TCM hal_clock_status_t clock_resume(void)
{
    return HAL_CLOCK_STATUS_OK;
}

/*
 * Funtion: Query frequency meter
 * tcksel: TESTED clock selection. 1: f_fxo_ck, 19: hf_fsys_ck.
 * fcksel: FIXED clock selection. 0: f_fxo_ck, 1: f_frtc_ck, 6: XOSC32K_CK.
 * return frequency unit: KHz
 */

#define FREQ_METER_WINDOW   4096

ATTR_TEXT_IN_TCM uint32_t hal_clock_get_freq_meter(uint8_t tcksel, uint8_t fcksel)
{
    uint32_t target_freq = 0;
    uint64_t target_freq_64 = 0;

    *ABIST_FQMTR_CON0 = 0xC000; // CKSYS_BASE (0XA2020000)+ 0x0400, to reset meter
    hal_gpt_delay_us(1);
    while ((*ABIST_FQMTR_CON1 & 0x8000) != 0); // CKSYS_BASE (0XA2020000)+ 0x0404, wait busy

    //CKSYS_BASE (0XA2020000)+ 0x0224, [10:8] fixed_clock, [4:0] tested_clock
    *CKSYS_TST_SEL_1_F_TCKSEL = tcksel;
    *CKSYS_TST_SEL_1_F_FCKSEL = fcksel;

    *ABIST_FQMTR_CON0 = FREQ_METER_WINDOW-1; // CKSYS_BASE (0XA2020000)+ 0x0400, set window 4096 T
    *ABIST_FQMTR_CON0 = (0x8000|(FREQ_METER_WINDOW-1)); // CKSYS_BASE (0XA2020000)+ 0x0400, to enable meter
    //*ABIST_FQMTR_CON0 = 0x8063; // CKSYS_BASE (0XA2020000)+ 0x0400, to enable meter, window 100T
#ifdef  CLK_HOPPING_TEST
#else
    hal_gpt_delay_us(5); //wait meter start
#endif

    while ((*ABIST_FQMTR_CON1 & 0x8000) != 0); // CKSYS_BASE (0XA2020000)+ 0x0404, wait busy

    /* fqmtr_ck = fixed_ck*fqmtr_data/winset, */
    target_freq = *PLL_ABIST_FQMTR_DATA__F_FQMTR_DATA; // CKSYS_BASE (0XA2020000)+ 0x040C, read meter data

#ifdef  CLK_HOPPING_TEST
    return target_freq;
#else
    #if CLOCK_DEBUG
        log_hal_info("0x%08x\r\n", *PLL_ABIST_FQMTR_DATA__F_FQMTR_DATA);
    #endif
    target_freq_64 = (uint64_t)target_freq;
    target_freq_64 = 26 * 1000 * target_freq_64 / FREQ_METER_WINDOW;
    target_freq = (uint32_t)target_freq_64;

    return target_freq;
#endif
}

/*
 * Funtion: Query frequency meter measurement cycles
 * tcksel: TESTED clock selection. 1: f_fxo_ck, 19: hf_fsys_ck.
 * fcksel: FIXED clock selection. 0: f_fxo_ck, 1: f_frtc_ck, 6: XOSC32K_CK.
 * winset: measurement window setting (number of fixed clock cycles)
 * return frequency unit: KHz
 */
ATTR_TEXT_IN_TCM uint32_t hal_clock_get_freq_meter_cycle(uint8_t tcksel, uint8_t fcksel, uint16_t winset)
{
    uint32_t cycles = 0;

    return cycles;
}

ATTR_TEXT_IN_TCM bool hal_clock_set_vore_voltage(clock_vcore_voltage _vcore_voltage)
{
    if(_vcore_voltage < NR_CLKVCORE_VOLTAGE) {
        vcore_voltage = _vcore_voltage;
        return HAL_CLOCK_STATUS_OK;
    }else {
#ifdef CLK_DEBUG_ERR
        log_hal_error("%s, error _vcore_voltage (%d)\r\n", __FUNCTION__, _vcore_voltage);
#endif
        return HAL_CLOCK_STATUS_INVALID_PARAMETER;
    }
}

ATTR_TEXT_IN_TCM uint8_t hal_clock_get_vore_voltage(void)
{
    return vcore_voltage;
}

/* Enable PLL and switch clock to PLL */
ATTR_TEXT_IN_TCM void hal_clock_set_pll_dcm_init(void)
{
    uint8_t efuse_cpu_78m = 0;

    *UPLL_CON5__F_RG_UPLL_IBANK_FINETUNE = 0x5; //MIXED_BASE (0XA2040000)+ 0x0156,  set to 3’b101 to insure the ICO no startup problem
    *MPLL_CON5__F_RG_MPLL_IBANK_FINETUNE = 0x5; //MIXED_BASE (0XA2040000)+ 0x0116,  set to 3’b101 to insure the ICO no startup problem
    *CLKSQ_CON0__F_DA_SRCLKENA = 0x1; //MIXED_BASE (0XA2040000)+ 0x0020, bit 0 set to 1’b1 to enable CLKSQ/IV-Gen of PLLGP
    // wait 6us for CLKSQ/IV-Gen stable
    hal_gpt_delay_us(6);
    *DPM_CON1__F_UPLL_SETTLE_TIME = 0x104; //MIXED_BASE (0XA2040000)+ 0x0096, UPLL settle time 20us
    *DPM_CPN2__F_MPLL_SETTLE_TIME = 0x104; //MIXED_BASE (0XA2040000)+ 0x0098, MPLL settling time 20us

#ifdef USB_USE_UPLL
    // enable UPLL
    *UPLL_CON0__F_DA_UPLL_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0140, bit 0 set to 1 to enable UPLL
    // wait 20us for UPLL stable
    hal_gpt_delay_us(20);
    // enable HW mode TOPSM control and clock CG of PLL control
    *PLL_CON2 = 0x0000; // MIXED_BASE (0XA2040000)+ 0x0048, to enable PLL TOPSM control and clock CG of controller
    *PLL_CON3 = 0x0000; // MIXED_BASE (0XA2040000)+ 0x004C, to enable DCXO 26M TOPSM control and clock CG of controller

    // enable delay control
    *PLLTD_CON0__F_BP_PLL_DLY= 0x0000; //MIXED_BASE (0XA2040000)+ 0x0700, bit 0 set to 0 to enable delay control
    *UPLL_CON0__F_RG_UPLL_RDY = 0x1; //MIXED_BASE (0XA2040000)+ 0x0143, bit 0 set to 1 to release UPLL clock

    // wait for 1us for TOPSM and delay (HW) control signal stable
    hal_gpt_delay_us(1);

    // select MPLL frequency
    *MPLL_CON1__F_RG_MPLL_POSTDIV = 0x0; // MIXED_BASE (0XA2040000)+ 0x0107, set post divider = /1

    // enable PLL_PGDET
    *PLL_CON4__F_RG_PLL_PGDET_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0050, bit 2 set to 1 to enable PGDET of MDDS

    // enable MDDS
    *MDDS_CON0__F_RG_MDDS_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0640, bit 0 set to 1 to enable MDDS

    // enable and reset MPLL
    *MPLL_CON0__F_DA_MPLL_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0100, bit 0 set to 1 to enable MPLL and generate reset of MPLL
    *MPLL_CON0__F_RG_MPLL_RDY = 0x1; //MIXED_BASE (0XA2040000)+ 0x0103, bit 0 set to 1 to release MPLL clock

    // wait 50us for PLL and DDS settle
    hal_gpt_delay_us(50);

    // 48M sel
    *XO_PDN_SETD0 = 0x1; // CKSYS_XO_CLK_BASE (0XA2030000)+ 0x0b10, turn off 48M CG to prevent glitch
    *CKSYS_CLK_CFG_2__F_CLK_48M_SEL = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0248, 0:26M, 1:UPLL_48M, 2:MPLL_48M
#else
    // CLKSQ/IV-Gen stable time
    *PLLTD_CON2__F_S1_STB_TIME = 0x4e; //MIXED_BASE (0XA2040000)+ 0x0708, 78*13M = 6us
    // select MPLL frequency
    *MPLL_CON1__F_RG_MPLL_POSTDIV = 0x0; // MIXED_BASE (0XA2040000)+ 0x0107, set post divider = /1

    // enable and reset MPLL
    *MPLL_CON0__F_DA_MPLL_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0100, bit 0 set to 1 to enable MPLL and generate reset of MPLL
    // wait 20us for PLL settle
    hal_gpt_delay_us(20);

    // enable HW mode TOPSM control and clock CG of PLL control
    *PLL_CON2 = 0x0000; // MIXED_BASE (0XA2040000)+ 0x0048, to enable PLL TOPSM control and clock CG of controller
    *PLL_CON3 = 0x0000; // MIXED_BASE (0XA2040000)+ 0x004C, to enable DCXO 26M TOPSM control and clock CG of controller

    // enable delay control
    *PLLTD_CON0__F_BP_PLL_DLY= 0x0000; //MIXED_BASE (0XA2040000)+ 0x0700, bit 0 set to 0 to enable delay control
    *MPLL_CON0__F_RG_MPLL_RDY = 0x1; //MIXED_BASE (0XA2040000)+ 0x0103, bit 0 set to 1 to release MPLL clock

    // wait 20us for for TOPSM and delay (HW) control signal stable
    hal_gpt_delay_us(20);

    // 48M sel
    *XO_PDN_SETD0 = 0x1; // CKSYS_XO_CLK_BASE (0XA2030000)+ 0x0b10, turn off 48M CG to prevent glitch
    *CKSYS_CLK_CFG_2__F_CLK_48M_SEL = 0x2; // CKSYS_BASE (0XA2020000)+ 0x0248, 0:26M, 1:UPLL_48M, 2:MPLL_48M
#endif

    // Sequence to switch to PLL clocks as below:
    // enable clock divider
    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D5_EN = 0x1;   // CKSYS_BASE (0XA2020000)+ 0x0286, to enable digital frequency divider
    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D3_EN = 0x1;   // CKSYS_BASE (0XA2020000)+ 0x0285, to enable digital frequency divider
    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D2_EN = 0x1;   // CKSYS_BASE (0XA2020000)+ 0x0284, to enable digital frequency divider
    *CKSYS_CLK_DIV_2__F_CLK_PLL2_DIV_EN = 0x1;   // CKSYS_BASE (0XA2020000)+ 0x0289, to enable digital frequency divider

    // set clock mux
    efuse_cpu_78m = (*HW_MISC0) & 0x2; // EFUSE_BASE (0XA20A0000)+ 0x0230, bit 1
    *CKSYS_CLK_FORCE_ON_0 = 0x01010101;   // CKSYS_BASE (0XA2020000)+ 0x0270, to force clock on
    *CKSYS_CLK_FORCE_ON_1 = 0x01010101;   // CKSYS_BASE (0XA2020000)+ 0x0274, to force clock on

    if(vcore_voltage == CLK_VCORE_0P9V){
        *CKSYS_CLK_CFG_0__F_CLK_SYS_SEL = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0240, SYS clock @ 26MHz
        *CKSYS_CLK_CFG_0__F_CLK_SFC_SEL = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0241, SFC clock @ 26MHz
        *CKSYS_CLK_CFG_0__F_CLK_DSP_SEL = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0242, DSP clock @ 26MHz
        *CKSYS_CLK_CFG_0__F_CLK_SPIMST_SEL = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0243, SPIMST clock @ 26MHz
        *CKSYS_CLK_CFG_1__F_CLK_BSIBPI_SEL = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0244, BSIBPI clock @ 26MHz
        *CKSYS_CLK_CFG_1__F_CLK_SDIOMST0_SEL = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0245, SDIOMST0 clock @ 26MHz
        *CKSYS_CLK_CFG_1__F_CLK_SDIOMST1_SEL = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0246, SDIOMST1 clock @ 26MHz
        *CKSYS_CLK_CFG_1__F_CLK_SPISLV_SEL = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0247, SPISLV clock @ 26MHz
    }else if(vcore_voltage == CLK_VCORE_1P3V) {
        *CKSYS_CLK_CFG_0__F_CLK_SYS_SEL = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0240, SYS clock @ 156MHz
        *CKSYS_CLK_CFG_0__F_CLK_SFC_SEL = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0241, SFC clock @ 78MHz
        *CKSYS_CLK_CFG_0__F_CLK_DSP_SEL = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0242, DSP clock @ 124.8MHz
        *CKSYS_CLK_CFG_0__F_CLK_SPIMST_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0243, SPIMST clock @ 104MHz
        *CKSYS_CLK_CFG_1__F_CLK_BSIBPI_SEL = 0x2; // CKSYS_BASE (0XA2020000)+ 0x0244, BSIBPI clock @ 62.4MHz
        *CKSYS_CLK_CFG_1__F_CLK_SDIOMST0_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0245, SDIOMST0 clock @ 52MHz
        *CKSYS_CLK_CFG_1__F_CLK_SDIOMST1_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0246, SDIOMST1 clock @ 52MHz
        *CKSYS_CLK_CFG_1__F_CLK_SPISLV_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0247, SPISLV clock @ 104MHz
    }else { //default VCORE Voltage = 1P1V
        if (efuse_cpu_78m)
            *CKSYS_CLK_CFG_0__F_CLK_SYS_SEL = 0x5; // CKSYS_BASE (0XA2020000)+ 0x0240, SYS clock @ 78MHz
        else
            *CKSYS_CLK_CFG_0__F_CLK_SYS_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0240, SYS clock @ 104MHz

            *CKSYS_CLK_CFG_0__F_CLK_SFC_SEL = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0241, SFC clock @ 78MHz
            *CKSYS_CLK_CFG_0__F_CLK_DSP_SEL = 0x2; // CKSYS_BASE (0XA2020000)+ 0x0242, DSP clock @ 104MHz
            *CKSYS_CLK_CFG_0__F_CLK_SPIMST_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0243, SPIMST clock @ 104MHz
            *CKSYS_CLK_CFG_1__F_CLK_BSIBPI_SEL = 0x2; // CKSYS_BASE (0XA2020000)+ 0x0244, BSIBPI clock @ 62.4MHz
            *CKSYS_CLK_CFG_1__F_CLK_SDIOMST0_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0245, SDIOMST0 clock @ 52MHz
            *CKSYS_CLK_CFG_1__F_CLK_SDIOMST1_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0246, SDIOMST1 clock @ 52MHz
            *CKSYS_CLK_CFG_1__F_CLK_SPISLV_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0247, SPISLV clock @ 104MHz
    }

    *CKSYS_CLK_UPDATE_0__F_CHG_SYS = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0250, SYS clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_SYS ==1); // CKSYS_BASE (0XA2020000)+ 0x0250, wait SYS clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SYS_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0260, wait SYS clock switch

    *CKSYS_CLK_UPDATE_0__F_CHG_SFC = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0251, SFC clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_SFC ==1); // CKSYS_BASE (0XA2020000)+ 0x0251, wait SFC clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SFC_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0261, wait SFC clock switch

    *CKSYS_CLK_UPDATE_0__F_CHG_DSP = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0252, DSP clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_DSP ==1); // CKSYS_BASE (0XA2020000)+ 0x0252, wait DSP clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_DSP_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0262, wait DSP clock switch

    *CKSYS_CLK_UPDATE_0__F_CHG_SPIMST = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0253, SPIMST clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_SPIMST ==1); // CKSYS_BASE (0XA2020000)+ 0x0253, wait SPIMST clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SPIMST_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0263, wait SPIMST clock switch

    *CKSYS_CLK_UPDATE_1__F_CHG_BSIBPI = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0254, BSIBPI clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_BSIBPI ==1); // CKSYS_BASE (0XA2020000)+ 0x0254, wait BSIBPI clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_BSIBPI_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0264, wait BSIBPI clock switch

    *CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST0 = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0255, SDIOMST0 clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST0 ==1); // CKSYS_BASE (0XA2020000)+ 0x0255, wait SDIOMST0 clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SDIOMST0_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0265, wait SDIOMST0 clock switch

    *CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST1 = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0256, SDIOMST1 clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST1 ==1); // CKSYS_BASE (0XA2020000)+ 0x0256, wait SDIOMST1 clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SDIOMST1_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0266, wait SDIOMST1 clock switch

    *CKSYS_CLK_UPDATE_1__F_CHG_SPISLV = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0257, SPISLV clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_SPISLV ==1); // CKSYS_BASE (0XA2020000)+ 0x0257, wait SPISLV clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SPISLV_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0267, wait SPISLV clock switch

    *CKSYS_CLK_FORCE_ON_0 = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0270, to disable force clock on
    *CKSYS_CLK_FORCE_ON_1 = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0274, to disable force clock on

    // set clock DCM
    *SFC_DCM_CON_0__F_RG_SFC_DCM_DBC_NUM = 0xFF; // CKSYS_BASE (0XA2020000)+ 0x0141, to set SFC DCM debounce cycle 255
    *SFC_DCM_CON_0__F_RG_SFC_DCM_DBC_EN = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0142, to enable SFC DCM debounce
    *SFC_DCM_CON_1__F_RG_SFC_CLKOFF_EN = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0144, to enable SFC DCM
    *SFC_DCM_CON_0__F_RG_SFC_DCM_APB_SEL = 0x6; // CKSYS_BASE (0XA2020000)+ 0x0143, to update SFC DCM setting
    *SFC_DCM_CON_1__F_RG_SFC_DCM_APB_TOG = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0147, to sync SFC DCM setting

    if(vcore_voltage == CLK_VCORE_1P3V) {
        *BUS_DCM_CON_0__F_RG_BUS_SFSEL = 0x0; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0100, BUS DCM, idle clock 78M/64
        *CM4_DCM_CON_0__F_RG_CM_SFSEL = 0x0; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0110, CM4 DCM, idle clock 156M/64
    }else if(vcore_voltage == CLK_VCORE_1P1V) {
        *BUS_DCM_CON_0__F_RG_BUS_SFSEL = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0100, BUS DCM, idle clock 52M/32 (efuse_cpu_78m: 39M/32)
        *CM4_DCM_CON_0__F_RG_CM_SFSEL = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0110, CM4 DCM, idle clock 104M/32 (efuse_cpu_78m: 78M/32)
    }else {//default VCORE_1P1V
        *BUS_DCM_CON_0__F_RG_BUS_SFSEL = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0100, BUS DCM, idle clock 52M/32
        *CM4_DCM_CON_0__F_RG_CM_SFSEL = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0110, CM4 DCM, idle clock 104M/32
    }

    *BUS_DCM_CON_1__F_RG_BUS_CLKSLOW_EN = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0105, to enable BUS DCM clock slow
    *BUS_DCM_CON_1__F_RG_BUS_CLKOFF_EN = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0104, to enable BUS DCM clock off
    *BUS_DCM_CON_0__F_RG_BUS_DCM_EN = 0x3; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0103, to enable EMI DCM clock off

    *CM4_DCM_CON_1__F_RG_CM_CLKSLOW_EN = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0115, to enable CMSYS DCM clock slow
    *XO_DCM_CON_1__F_RG_XO_CLKOFF_EN = 0x1; // CKSYS_XO_CLK_BASE (0XA2030000)+ 0x0C04, to enable XO DCM clock off
    *XO_DCM_CON_1__F_RG_XO_CLKSLOW_EN = 0x1; // CKSYS_XO_CLK_BASE (0XA2030000)+ 0x0C05, to enable XO DCM clock slow
    *CLK_FREQ_SWCH__F_RG_PLLCK_SEL = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0170, to enable DCM control
    //wait 1us for DCM settle
    hal_gpt_delay_us(1);
}   /* void clock_set_pll_dcm_init(void) */

ATTR_TEXT_IN_TCM void hal_clock_disable_pll(void)
{
#ifdef USB_USE_UPLL
    // 48M CG
    *XO_PDN_SETD0 = 0x1; // CKSYS_XO_CLK_BASE (0XA2030000)+ 0x0b10, turn off 48M CG to prevent glitch

    // disable and MPLL
    *MPLL_CON0__F_RG_MPLL_RDY = 0x0; //MIXED_BASE (0XA2040000)+ 0x0103, bit 0 set to 0 to gating MPLL clock
    *MPLL_CON0__F_DA_MPLL_EN = 0x0; //MIXED_BASE (0XA2040000)+ 0x0100, bit 0 set to 0 to disable MPLL

    // disable MDDS
    *MDDS_CON0__F_RG_MDDS_EN = 0x0; //MIXED_BASE (0XA2040000)+ 0x0640, bit 0 set to 0 to disable MDDS

    // disable PLL_PGDET
    *PLL_CON4__F_RG_PLL_PGDET_EN = 0x0; //MIXED_BASE (0XA2040000)+ 0x0050, bit 2 set to 0 to disable PGDET of MDDS

    // disable UPLL
    *UPLL_CON0__F_RG_UPLL_RDY = 0x0; //MIXED_BASE (0XA2040000)+ 0x0143, bit 0 set to 0 to gating UPLL clock
    *UPLL_CON0__F_DA_UPLL_EN = 0x0; //MIXED_BASE (0XA2040000)+ 0x0140, bit 0 set to 0 to disable UPLL
#else
    // 48M CG
    *XO_PDN_SETD0 = 0x1; // CKSYS_XO_CLK_BASE (0XA2030000)+ 0x0b10, turn off 48M CG to prevent glitch

    // disable and MPLL
    *MPLL_CON0__F_RG_MPLL_RDY = 0x0; //MIXED_BASE (0XA2040000)+ 0x0103, bit 0 set to 0 to gating MPLL clock
    *MPLL_CON0__F_DA_MPLL_EN = 0x0; //MIXED_BASE (0XA2040000)+ 0x0100, bit 0 set to 0 to disable MPLL
#endif

    // wait 6us for stable
    hal_gpt_delay_us(6);

    *CLKSQ_CON0__F_DA_SRCLKENA = 0x0; //MIXED_BASE (0XA2040000)+ 0x0020, bit 0 set to 1’b0 to disable CLKSQ/IV-Gen of PLLGP
    // wait 6us for CLKSQ/IV-Gen stable
    hal_gpt_delay_us(6);
}

ATTR_TEXT_IN_TCM void hal_clock_set_switch_to_0P9V(void)        //all 26MHz
{
    *CKSYS_CLK_FORCE_ON_0 = 0x01010101;   // CKSYS_BASE (0XA2020000)+ 0x0270, to force clock on
    *CKSYS_CLK_FORCE_ON_1 = 0x01010101;   // CKSYS_BASE (0XA2020000)+ 0x0274, to force clock on

    //set clock mux
    *CKSYS_CLK_CFG_0__F_CLK_SYS_SEL = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0240, SYS clock @ 26MHz
    *CKSYS_CLK_CFG_0__F_CLK_SFC_SEL = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0241, SFC clock @ 26MHz
    *CKSYS_CLK_CFG_0__F_CLK_DSP_SEL = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0242, DSP clock @ 26MHz
    *CKSYS_CLK_CFG_0__F_CLK_SPIMST_SEL = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0243, SPIMST clock @ 26MHz
    *CKSYS_CLK_CFG_1__F_CLK_BSIBPI_SEL = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0244, BSIBPI clock @ 26MHz
    *CKSYS_CLK_CFG_1__F_CLK_SDIOMST0_SEL = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0245, SDIOMST0 clock @ 26MHz
    *CKSYS_CLK_CFG_1__F_CLK_SDIOMST1_SEL = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0246, SDIOMST1 clock @ 26MHz
    *CKSYS_CLK_CFG_1__F_CLK_SPISLV_SEL = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0247, SPISLV clock @ 26MHz

    //48M sel
    *XO_PDN_SETD0 = 0x1; // CKSYS_XO_CLK_BASE (0XA2030000)+ 0x0b10, turn off 48M CG to prevent glitch
    *CKSYS_CLK_CFG_2__F_CLK_48M_SEL = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0248, 0:26M, 1:UPLL_48M, 2:MPLL_48M

    *CKSYS_CLK_UPDATE_0__F_CHG_SYS = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0250, SYS clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_SYS ==1); // CKSYS_BASE (0XA2020000)+ 0x0250, wait SYS clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SYS_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0260, wait SYS clock switch

    *CKSYS_CLK_UPDATE_0__F_CHG_SFC = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0251, SFC clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_SFC ==1); // CKSYS_BASE (0XA2020000)+ 0x0251, wait SFC clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SFC_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0261, wait SFC clock switch

    *CKSYS_CLK_UPDATE_0__F_CHG_DSP = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0252, DSP clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_DSP ==1); // CKSYS_BASE (0XA2020000)+ 0x0252, wait DSP clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_DSP_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0262, wait DSP clock switch

    *CKSYS_CLK_UPDATE_0__F_CHG_SPIMST = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0253, SPIMST clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_SPIMST ==1); // CKSYS_BASE (0XA2020000)+ 0x0253, wait SPIMST clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SPIMST_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0263, wait SPIMST clock switch

    *CKSYS_CLK_UPDATE_1__F_CHG_BSIBPI = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0254, BSIBPI clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_BSIBPI ==1); // CKSYS_BASE (0XA2020000)+ 0x0254, wait BSIBPI clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_BSIBPI_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0264, wait BSIBPI clock switch

    *CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST0 = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0255, SDIOMST0 clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST0 ==1); // CKSYS_BASE (0XA2020000)+ 0x0255, wait SDIOMST0 clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SDIOMST0_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0265, wait SDIOMST0 clock switch

    *CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST1 = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0256, SDIOMST1 clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST1 ==1); // CKSYS_BASE (0XA2020000)+ 0x0256, wait SDIOMST1 clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SDIOMST1_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0266, wait SDIOMST1 clock switch

    *CKSYS_CLK_UPDATE_1__F_CHG_SPISLV = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0257, SPISLV clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_SPISLV ==1); // CKSYS_BASE (0XA2020000)+ 0x0257, wait SPISLV clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SPISLV_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0267, wait SPISLV clock switch

    *CKSYS_CLK_FORCE_ON_0 = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0270, to disable force clock on
    *CKSYS_CLK_FORCE_ON_1 = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0274, to disable force clock on

    //set DCM
    *CLK_FREQ_SWCH__F_RG_PLLCK_SEL = 0x0; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0170, to disable DCM control
    *BUS_DCM_CON_0__F_RG_BUS_SFSEL = 0x2; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0100, BUS DCM, idle clock 13M/16
    *CM4_DCM_CON_0__F_RG_CM_SFSEL = 0x2; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0100, CM4 DCM, idle clock 26M/16
    *CLK_FREQ_SWCH__F_RG_PLLCK_SEL = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0170, to enable DCM control
    hal_gpt_delay_us(1);

    // disable clock divider
    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D5_EN = 0x0;   // CKSYS_BASE (0XA2020000)+ 0x0286, to disable digital frequency divider
    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D3_EN = 0x0;   // CKSYS_BASE (0XA2020000)+ 0x0285, to disable digital frequency divider
    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D2_EN = 0x0;   // CKSYS_BASE (0XA2020000)+ 0x0284, to disable digital frequency divider
    *CKSYS_CLK_DIV_2__F_CLK_PLL2_DIV_EN = 0x0;   // CKSYS_BASE (0XA2020000)+ 0x0289, to disable digital frequency divider

    hal_clock_disable_pll();
}

ATTR_TEXT_IN_TCM void hal_clock_set_switch_to_1P1V(void)    // 104 MHz
{
    //uint8_t efuse_cpu_78m = 0;

    // Sequence to enable PLL
    *UPLL_CON5__F_RG_UPLL_IBANK_FINETUNE = 0x5; //MIXED_BASE (0XA2040000)+ 0x0156,  set to 3’b101 to insure the ICO no startup problem
    *MPLL_CON5__F_RG_MPLL_IBANK_FINETUNE = 0x5; //MIXED_BASE (0XA2040000)+ 0x0116,  set to 3’b101 to insure the ICO no startup problem
    *CLKSQ_CON0__F_DA_SRCLKENA = 0x1; //MIXED_BASE (0XA2040000)+ 0x0020, bit 0 set to 1’b1 to enable CLKSQ/IV-Gen of PLLGP
    // wait 6us for CLKSQ/IV-Gen stable
    hal_gpt_delay_us(6);
    *DPM_CON1__F_UPLL_SETTLE_TIME = 0x104; //MIXED_BASE (0XA2040000)+ 0x0096, UPLL settle time 20us
    *DPM_CPN2__F_MPLL_SETTLE_TIME = 0x104; //MIXED_BASE (0XA2040000)+ 0x0098, MPLL settling time 20us

#ifdef USB_USE_UPLL
    // enable UPLL
    *UPLL_CON0__F_DA_UPLL_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0140, bit 0 set to 1 to enable UPLL
    // wait 20us for UPLL stable
    hal_gpt_delay_us(20);
    // enable HW mode TOPSM control and clock CG of PLL control
    *PLL_CON2 = 0x0000; // MIXED_BASE (0XA2040000)+ 0x0048, to enable PLL TOPSM control and clock CG of controller
    *PLL_CON3 = 0x0000; // MIXED_BASE (0XA2040000)+ 0x004C, to enable DCXO 26M TOPSM control and clock CG of controller

    // enable delay control
    *PLLTD_CON0__F_BP_PLL_DLY= 0x0000; //MIXED_BASE (0XA2040000)+ 0x0700, bit 0 set to 0 to enable delay control
    *UPLL_CON0__F_RG_UPLL_RDY = 0x1; //MIXED_BASE (0XA2040000)+ 0x0143, bit 0 set to 1 to release UPLL clock

    // wait for 1us for TOPSM and delay (HW) control signal stable
    hal_gpt_delay_us(1);

    // select MPLL frequency
    *MPLL_CON1__F_RG_MPLL_POSTDIV = 0x0; // MIXED_BASE (0XA2040000)+ 0x0107, set post divider = /1

    // enable PLL_PGDET
    *PLL_CON4__F_RG_PLL_PGDET_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0050, bit 2 set to 1 to enable PGDET of MDDS

    // enable MDDS
    *MDDS_CON0__F_RG_MDDS_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0640, bit 0 set to 1 to enable MDDS

    // enable and reset MPLL
    *MPLL_CON0__F_DA_MPLL_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0100, bit 0 set to 1 to enable MPLL and generate reset of MPLL
    *MPLL_CON0__F_RG_MPLL_RDY = 0x1; //MIXED_BASE (0XA2040000)+ 0x0103, bit 0 set to 1 to release MPLL clock

    // wait 50us for PLL and DDS settle
    hal_gpt_delay_us(50);

    // 48M sel
    *XO_PDN_SETD0 = 0x1; // CKSYS_XO_CLK_BASE (0XA2030000)+ 0x0b10, turn off 48M CG to prevent glitch
    *CKSYS_CLK_CFG_2__F_CLK_48M_SEL = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0248, 0:26M, 1:UPLL_48M, 2:MPLL_48M
#else
    // CLKSQ/IV-Gen stable time
    *PLLTD_CON2__F_S1_STB_TIME = 0x4e; //MIXED_BASE (0XA2040000)+ 0x0708, 78*13M = 6us
    // select MPLL frequency
    *MPLL_CON1__F_RG_MPLL_POSTDIV = 0x0; // MIXED_BASE (0XA2040000)+ 0x0107, set post divider = /1

    // enable and reset MPLL
    *MPLL_CON0__F_DA_MPLL_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0100, bit 0 set to 1 to enable MPLL and generate reset of MPLL
    // wait 20us for PLL settle
    hal_gpt_delay_us(20);

    // enable HW mode TOPSM control and clock CG of PLL control
    *PLL_CON2 = 0x0000; // MIXED_BASE (0XA2040000)+ 0x0048, to enable PLL TOPSM control and clock CG of controller
    *PLL_CON3 = 0x0000; // MIXED_BASE (0XA2040000)+ 0x004C, to enable DCXO 26M TOPSM control and clock CG of controller

    // enable delay control
    *PLLTD_CON0__F_BP_PLL_DLY= 0x0000; //MIXED_BASE (0XA2040000)+ 0x0700, bit 0 set to 0 to enable delay control
    *MPLL_CON0__F_RG_MPLL_RDY = 0x1; //MIXED_BASE (0XA2040000)+ 0x0103, bit 0 set to 1 to release MPLL clock

    // wait 20us for for TOPSM and delay (HW) control signal stable
    hal_gpt_delay_us(20);

    // 48M sel
    *XO_PDN_SETD0 = 0x1; // CKSYS_XO_CLK_BASE (0XA2030000)+ 0x0b10, turn off 48M CG to prevent glitch
    *CKSYS_CLK_CFG_2__F_CLK_48M_SEL = 0x2; // CKSYS_BASE (0XA2020000)+ 0x0248, 0:26M, 1:UPLL_48M, 2:MPLL_48M
#endif

    // Sequence to switch to PLL clocks as below:
    // enable clock divider
    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D5_EN = 0x1;   // CKSYS_BASE (0XA2020000)+ 0x0286, to enable digital frequency divider
    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D3_EN = 0x1;   // CKSYS_BASE (0XA2020000)+ 0x0285, to enable digital frequency divider
    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D2_EN = 0x1;   // CKSYS_BASE (0XA2020000)+ 0x0284, to enable digital frequency divider
    *CKSYS_CLK_DIV_2__F_CLK_PLL2_DIV_EN = 0x1;   // CKSYS_BASE (0XA2020000)+ 0x0289, to enable digital frequency divider

    // set clock mux
    //efuse_cpu_78m = (*HW_MISC0) & 0x2; // EFUSE_BASE (0XA20A0000)+ 0x0230, bit 1
    *CKSYS_CLK_FORCE_ON_0 = 0x01010101;   // CKSYS_BASE (0XA2020000)+ 0x0270, to force clock on
    *CKSYS_CLK_FORCE_ON_1 = 0x01010101;   // CKSYS_BASE (0XA2020000)+ 0x0274, to force clock on

    //if (efuse_cpu_78m)
        //*CKSYS_CLK_CFG_0__F_CLK_SYS_SEL = 0x5; // CKSYS_BASE (0XA2020000)+ 0x0240, SYS clock @ 78MHz
    //else
        *CKSYS_CLK_CFG_0__F_CLK_SYS_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0240, SYS clock @ 104MHz

    *CKSYS_CLK_CFG_0__F_CLK_SFC_SEL = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0241, SFC clock @ 78MHz
    *CKSYS_CLK_CFG_0__F_CLK_DSP_SEL = 0x2; // CKSYS_BASE (0XA2020000)+ 0x0242, DSP clock @ 104MHz
    *CKSYS_CLK_CFG_0__F_CLK_SPIMST_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0243, SPIMST clock @ 104MHz
    *CKSYS_CLK_CFG_1__F_CLK_BSIBPI_SEL = 0x2; // CKSYS_BASE (0XA2020000)+ 0x0244, BSIBPI clock @ 62.4MHz
    *CKSYS_CLK_CFG_1__F_CLK_SDIOMST0_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0245, SDIOMST0 clock @ 52MHz
    *CKSYS_CLK_CFG_1__F_CLK_SDIOMST1_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0246, SDIOMST1 clock @ 52MHz
    *CKSYS_CLK_CFG_1__F_CLK_SPISLV_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0247, SPISLV clock @ 104MHz

    *CKSYS_CLK_UPDATE_0__F_CHG_SYS = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0250, SYS clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_SYS ==1); // CKSYS_BASE (0XA2020000)+ 0x0250, wait SYS clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SYS_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0260, wait SYS clock switch

    *CKSYS_CLK_UPDATE_0__F_CHG_SFC = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0251, SFC clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_SFC ==1); // CKSYS_BASE (0XA2020000)+ 0x0251, wait SFC clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SFC_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0261, wait SFC clock switch

    *CKSYS_CLK_UPDATE_0__F_CHG_DSP = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0252, DSP clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_DSP ==1); // CKSYS_BASE (0XA2020000)+ 0x0252, wait DSP clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_DSP_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0262, wait DSP clock switch

    *CKSYS_CLK_UPDATE_0__F_CHG_SPIMST = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0253, SPIMST clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_SPIMST ==1); // CKSYS_BASE (0XA2020000)+ 0x0253, wait SPIMST clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SPIMST_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0263, wait SPIMST clock switch

    *CKSYS_CLK_UPDATE_1__F_CHG_BSIBPI = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0254, BSIBPI clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_BSIBPI ==1); // CKSYS_BASE (0XA2020000)+ 0x0254, wait BSIBPI clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_BSIBPI_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0264, wait BSIBPI clock switch

    *CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST0 = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0255, SDIOMST0 clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST0 ==1); // CKSYS_BASE (0XA2020000)+ 0x0255, wait SDIOMST0 clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SDIOMST0_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0265, wait SDIOMST0 clock switch

    *CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST1 = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0256, SDIOMST1 clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST1 ==1); // CKSYS_BASE (0XA2020000)+ 0x0256, wait SDIOMST1 clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SDIOMST1_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0266, wait SDIOMST1 clock switch
    *CKSYS_CLK_UPDATE_1__F_CHG_SPISLV = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0257, SPISLV clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_SPISLV ==1); // CKSYS_BASE (0XA2020000)+ 0x0257, wait SPISLV clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SPISLV_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0267, wait SPISLV clock switch

    *CKSYS_CLK_FORCE_ON_0 = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0270, to disable force clock on
    *CKSYS_CLK_FORCE_ON_1 = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0274, to disable force clock on

    //set DCM
    *CLK_FREQ_SWCH__F_RG_PLLCK_SEL = 0x0; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0170, to disable DCM control

    *BUS_DCM_CON_0__F_RG_BUS_SFSEL = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0100, BUS DCM, idle clock 52M/32 (efuse_cpu_78m: 39M/32)
    *CM4_DCM_CON_0__F_RG_CM_SFSEL = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0100, CM4 DCM, idle clock 104M/32 (efuse_cpu_78m: 78M/32)

    *CLK_FREQ_SWCH__F_RG_PLLCK_SEL = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0170, to enable DCM control
    hal_gpt_delay_us(1);
}

ATTR_TEXT_IN_TCM void hal_clock_set_switch_to_1P1V_78M(void)    // 78 MHz
{
    //uint8_t efuse_cpu_78m = 0;

    // Sequence to enable PLL
    *UPLL_CON5__F_RG_UPLL_IBANK_FINETUNE = 0x5; //MIXED_BASE (0XA2040000)+ 0x0156,  set to 3’b101 to insure the ICO no startup problem
    *MPLL_CON5__F_RG_MPLL_IBANK_FINETUNE = 0x5; //MIXED_BASE (0XA2040000)+ 0x0116,  set to 3’b101 to insure the ICO no startup problem
    *CLKSQ_CON0__F_DA_SRCLKENA = 0x1; //MIXED_BASE (0XA2040000)+ 0x0020, bit 0 set to 1’b1 to enable CLKSQ/IV-Gen of PLLGP
    // wait 6us for CLKSQ/IV-Gen stable
    hal_gpt_delay_us(6);
    *DPM_CON1__F_UPLL_SETTLE_TIME = 0x104; //MIXED_BASE (0XA2040000)+ 0x0096, UPLL settle time 20us
    *DPM_CPN2__F_MPLL_SETTLE_TIME = 0x104; //MIXED_BASE (0XA2040000)+ 0x0098, MPLL settling time 20us

#ifdef USB_USE_UPLL
    // enable UPLL
    *UPLL_CON0__F_DA_UPLL_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0140, bit 0 set to 1 to enable UPLL
    // wait 20us for UPLL stable
    hal_gpt_delay_us(20);
    // enable HW mode TOPSM control and clock CG of PLL control
    *PLL_CON2 = 0x0000; // MIXED_BASE (0XA2040000)+ 0x0048, to enable PLL TOPSM control and clock CG of controller
    *PLL_CON3 = 0x0000; // MIXED_BASE (0XA2040000)+ 0x004C, to enable DCXO 26M TOPSM control and clock CG of controller

    // enable delay control
    *PLLTD_CON0__F_BP_PLL_DLY= 0x0000; //MIXED_BASE (0XA2040000)+ 0x0700, bit 0 set to 0 to enable delay control
    *UPLL_CON0__F_RG_UPLL_RDY = 0x1; //MIXED_BASE (0XA2040000)+ 0x0143, bit 0 set to 1 to release UPLL clock

    // wait for 1us for TOPSM and delay (HW) control signal stable
    hal_gpt_delay_us(1);

    // select MPLL frequency
    *MPLL_CON1__F_RG_MPLL_POSTDIV = 0x0; // MIXED_BASE (0XA2040000)+ 0x0107, set post divider = /1

    // enable PLL_PGDET
    *PLL_CON4__F_RG_PLL_PGDET_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0050, bit 2 set to 1 to enable PGDET of MDDS

    // enable MDDS
    *MDDS_CON0__F_RG_MDDS_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0640, bit 0 set to 1 to enable MDDS

    // enable and reset MPLL
    *MPLL_CON0__F_DA_MPLL_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0100, bit 0 set to 1 to enable MPLL and generate reset of MPLL
    *MPLL_CON0__F_RG_MPLL_RDY = 0x1; //MIXED_BASE (0XA2040000)+ 0x0103, bit 0 set to 1 to release MPLL clock

    // wait 50us for PLL and DDS settle
    hal_gpt_delay_us(50);

    // 48M sel
    *XO_PDN_SETD0 = 0x1; // CKSYS_XO_CLK_BASE (0XA2030000)+ 0x0b10, turn off 48M CG to prevent glitch
    *CKSYS_CLK_CFG_2__F_CLK_48M_SEL = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0248, 0:26M, 1:UPLL_48M, 2:MPLL_48M
#else
    // CLKSQ/IV-Gen stable time
    *PLLTD_CON2__F_S1_STB_TIME = 0x4e; //MIXED_BASE (0XA2040000)+ 0x0708, 78*13M = 6us
    // select MPLL frequency
    *MPLL_CON1__F_RG_MPLL_POSTDIV = 0x0; // MIXED_BASE (0XA2040000)+ 0x0107, set post divider = /1

    // enable and reset MPLL
    *MPLL_CON0__F_DA_MPLL_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0100, bit 0 set to 1 to enable MPLL and generate reset of MPLL
    // wait 20us for PLL settle
    hal_gpt_delay_us(20);

    // enable HW mode TOPSM control and clock CG of PLL control
    *PLL_CON2 = 0x0000; // MIXED_BASE (0XA2040000)+ 0x0048, to enable PLL TOPSM control and clock CG of controller
    *PLL_CON3 = 0x0000; // MIXED_BASE (0XA2040000)+ 0x004C, to enable DCXO 26M TOPSM control and clock CG of controller

    // enable delay control
    *PLLTD_CON0__F_BP_PLL_DLY= 0x0000; //MIXED_BASE (0XA2040000)+ 0x0700, bit 0 set to 0 to enable delay control
    *MPLL_CON0__F_RG_MPLL_RDY = 0x1; //MIXED_BASE (0XA2040000)+ 0x0103, bit 0 set to 1 to release MPLL clock

    // wait 20us for for TOPSM and delay (HW) control signal stable
    hal_gpt_delay_us(20);

    // 48M sel
    *XO_PDN_SETD0 = 0x1; // CKSYS_XO_CLK_BASE (0XA2030000)+ 0x0b10, turn off 48M CG to prevent glitch
    *CKSYS_CLK_CFG_2__F_CLK_48M_SEL = 0x2; // CKSYS_BASE (0XA2020000)+ 0x0248, 0:26M, 1:UPLL_48M, 2:MPLL_48M
#endif

    // Sequence to switch to PLL clocks as below:
    // enable clock divider
    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D5_EN = 0x1;   // CKSYS_BASE (0XA2020000)+ 0x0286, to enable digital frequency divider
    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D3_EN = 0x1;   // CKSYS_BASE (0XA2020000)+ 0x0285, to enable digital frequency divider
    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D2_EN = 0x1;   // CKSYS_BASE (0XA2020000)+ 0x0284, to enable digital frequency divider
    *CKSYS_CLK_DIV_2__F_CLK_PLL2_DIV_EN = 0x1;   // CKSYS_BASE (0XA2020000)+ 0x0289, to enable digital frequency divider

    // set clock mux
    //efuse_cpu_78m = (*HW_MISC0) & 0x2; // EFUSE_BASE (0XA20A0000)+ 0x0230, bit 1
    *CKSYS_CLK_FORCE_ON_0 = 0x01010101;   // CKSYS_BASE (0XA2020000)+ 0x0270, to force clock on
    *CKSYS_CLK_FORCE_ON_1 = 0x01010101;   // CKSYS_BASE (0XA2020000)+ 0x0274, to force clock on

    //if (efuse_cpu_78m)
        *CKSYS_CLK_CFG_0__F_CLK_SYS_SEL = 0x5; // CKSYS_BASE (0XA2020000)+ 0x0240, SYS clock @ 78MHz
    //else
        //*CKSYS_CLK_CFG_0__F_CLK_SYS_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0240, SYS clock @ 104MHz

    *CKSYS_CLK_CFG_0__F_CLK_SFC_SEL = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0241, SFC clock @ 78MHz
    *CKSYS_CLK_CFG_0__F_CLK_DSP_SEL = 0x2; // CKSYS_BASE (0XA2020000)+ 0x0242, DSP clock @ 104MHz
    *CKSYS_CLK_CFG_0__F_CLK_SPIMST_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0243, SPIMST clock @ 104MHz
    *CKSYS_CLK_CFG_1__F_CLK_BSIBPI_SEL = 0x2; // CKSYS_BASE (0XA2020000)+ 0x0244, BSIBPI clock @ 62.4MHz
    *CKSYS_CLK_CFG_1__F_CLK_SDIOMST0_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0245, SDIOMST0 clock @ 52MHz
    *CKSYS_CLK_CFG_1__F_CLK_SDIOMST1_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0246, SDIOMST1 clock @ 52MHz
    *CKSYS_CLK_CFG_1__F_CLK_SPISLV_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0247, SPISLV clock @ 104MHz

    *CKSYS_CLK_UPDATE_0__F_CHG_SYS = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0250, SYS clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_SYS ==1); // CKSYS_BASE (0XA2020000)+ 0x0250, wait SYS clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SYS_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0260, wait SYS clock switch

    *CKSYS_CLK_UPDATE_0__F_CHG_SFC = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0251, SFC clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_SFC ==1); // CKSYS_BASE (0XA2020000)+ 0x0251, wait SFC clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SFC_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0261, wait SFC clock switch

    *CKSYS_CLK_UPDATE_0__F_CHG_DSP = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0252, DSP clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_DSP ==1); // CKSYS_BASE (0XA2020000)+ 0x0252, wait DSP clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_DSP_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0262, wait DSP clock switch

    *CKSYS_CLK_UPDATE_0__F_CHG_SPIMST = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0253, SPIMST clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_SPIMST ==1); // CKSYS_BASE (0XA2020000)+ 0x0253, wait SPIMST clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SPIMST_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0263, wait SPIMST clock switch

    *CKSYS_CLK_UPDATE_1__F_CHG_BSIBPI = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0254, BSIBPI clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_BSIBPI ==1); // CKSYS_BASE (0XA2020000)+ 0x0254, wait BSIBPI clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_BSIBPI_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0264, wait BSIBPI clock switch

    *CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST0 = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0255, SDIOMST0 clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST0 ==1); // CKSYS_BASE (0XA2020000)+ 0x0255, wait SDIOMST0 clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SDIOMST0_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0265, wait SDIOMST0 clock switch

    *CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST1 = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0256, SDIOMST1 clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST1 ==1); // CKSYS_BASE (0XA2020000)+ 0x0256, wait SDIOMST1 clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SDIOMST1_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0266, wait SDIOMST1 clock switch
    *CKSYS_CLK_UPDATE_1__F_CHG_SPISLV = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0257, SPISLV clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_SPISLV ==1); // CKSYS_BASE (0XA2020000)+ 0x0257, wait SPISLV clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SPISLV_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0267, wait SPISLV clock switch

    *CKSYS_CLK_FORCE_ON_0 = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0270, to disable force clock on
    *CKSYS_CLK_FORCE_ON_1 = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0274, to disable force clock on

    //set DCM
    *CLK_FREQ_SWCH__F_RG_PLLCK_SEL = 0x0; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0170, to disable DCM control

    *BUS_DCM_CON_0__F_RG_BUS_SFSEL = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0100, BUS DCM, idle clock 52M/32 (efuse_cpu_78m: 39M/32)
    *CM4_DCM_CON_0__F_RG_CM_SFSEL = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0100, CM4 DCM, idle clock 104M/32 (efuse_cpu_78m: 78M/32)

    *CLK_FREQ_SWCH__F_RG_PLLCK_SEL = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0170, to enable DCM control
    hal_gpt_delay_us(1);
}

ATTR_TEXT_IN_TCM void hal_clock_set_switch_to_1P3V(void)
{
    uint8_t efuse_cpu_78m = 0;

    // Sequence to enable PLL
    *UPLL_CON5__F_RG_UPLL_IBANK_FINETUNE = 0x5; //MIXED_BASE (0XA2040000)+ 0x0156,  set to 3’b101 to insure the ICO no startup problem
    *MPLL_CON5__F_RG_MPLL_IBANK_FINETUNE = 0x5; //MIXED_BASE (0XA2040000)+ 0x0116,  set to 3’b101 to insure the ICO no startup problem
    *CLKSQ_CON0__F_DA_SRCLKENA = 0x1; //MIXED_BASE (0XA2040000)+ 0x0020, bit 0 set to 1’b1 to enable CLKSQ/IV-Gen of PLLGP
    // wait 6us for CLKSQ/IV-Gen stable
    hal_gpt_delay_us(6);
    *DPM_CON1__F_UPLL_SETTLE_TIME = 0x104; //MIXED_BASE (0XA2040000)+ 0x0096, UPLL settle time 20us
    *DPM_CPN2__F_MPLL_SETTLE_TIME = 0x104; //MIXED_BASE (0XA2040000)+ 0x0098, MPLL settling time 20us

#ifdef USB_USE_UPLL
    // enable UPLL
    *UPLL_CON0__F_DA_UPLL_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0140, bit 0 set to 1 to enable UPLL
    // wait 20us for UPLL stable
    hal_gpt_delay_us(20);
    // enable HW mode TOPSM control and clock CG of PLL control
    *PLL_CON2 = 0x0000; // MIXED_BASE (0XA2040000)+ 0x0048, to enable PLL TOPSM control and clock CG of controller
    *PLL_CON3 = 0x0000; // MIXED_BASE (0XA2040000)+ 0x004C, to enable DCXO 26M TOPSM control and clock CG of controller

    // enable delay control
    *PLLTD_CON0__F_BP_PLL_DLY= 0x0000; //MIXED_BASE (0XA2040000)+ 0x0700, bit 0 set to 0 to enable delay control
    *UPLL_CON0__F_RG_UPLL_RDY = 0x1; //MIXED_BASE (0XA2040000)+ 0x0143, bit 0 set to 1 to release UPLL clock

    // wait for 1us for TOPSM and delay (HW) control signal stable
    hal_gpt_delay_us(1);

    // select MPLL frequency
    *MPLL_CON1__F_RG_MPLL_POSTDIV = 0x0; // MIXED_BASE (0XA2040000)+ 0x0107, set post divider = /1

    // enable PLL_PGDET
    *PLL_CON4__F_RG_PLL_PGDET_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0050, bit 2 set to 1 to enable PGDET of MDDS

    // enable MDDS
    *MDDS_CON0__F_RG_MDDS_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0640, bit 0 set to 1 to enable MDDS

    // enable and reset MPLL
    *MPLL_CON0__F_DA_MPLL_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0100, bit 0 set to 1 to enable MPLL and generate reset of MPLL
    *MPLL_CON0__F_RG_MPLL_RDY = 0x1; //MIXED_BASE (0XA2040000)+ 0x0103, bit 0 set to 1 to release MPLL clock

    // wait 50us for PLL and DDS settle
    hal_gpt_delay_us(50);

    // 48M sel
    *XO_PDN_SETD0 = 0x1; // CKSYS_XO_CLK_BASE (0XA2030000)+ 0x0b10, turn off 48M CG to prevent glitch
    *CKSYS_CLK_CFG_2__F_CLK_48M_SEL = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0248, 0:26M, 1:UPLL_48M, 2:MPLL_48M
#else
    // CLKSQ/IV-Gen stable time
    *PLLTD_CON2__F_S1_STB_TIME = 0x4e; //MIXED_BASE (0XA2040000)+ 0x0708, 78*13M = 6us
    // select MPLL frequency
    *MPLL_CON1__F_RG_MPLL_POSTDIV = 0x0; // MIXED_BASE (0XA2040000)+ 0x0107, set post divider = /1

    // enable and reset MPLL
    *MPLL_CON0__F_DA_MPLL_EN = 0x1; //MIXED_BASE (0XA2040000)+ 0x0100, bit 0 set to 1 to enable MPLL and generate reset of MPLL
    // wait 20us for PLL settle
    hal_gpt_delay_us(20);

    // enable HW mode TOPSM control and clock CG of PLL control
    *PLL_CON2 = 0x0000; // MIXED_BASE (0XA2040000)+ 0x0048, to enable PLL TOPSM control and clock CG of controller
    *PLL_CON3 = 0x0000; // MIXED_BASE (0XA2040000)+ 0x004C, to enable DCXO 26M TOPSM control and clock CG of controller

    // enable delay control
    *PLLTD_CON0__F_BP_PLL_DLY= 0x0000; //MIXED_BASE (0XA2040000)+ 0x0700, bit 0 set to 0 to enable delay control
    *MPLL_CON0__F_RG_MPLL_RDY = 0x1; //MIXED_BASE (0XA2040000)+ 0x0103, bit 0 set to 1 to release MPLL clock

    // wait 20us for for TOPSM and delay (HW) control signal stable
    hal_gpt_delay_us(20);

    // 48M sel
    *XO_PDN_SETD0 = 0x1; // CKSYS_XO_CLK_BASE (0XA2030000)+ 0x0b10, turn off 48M CG to prevent glitch
    *CKSYS_CLK_CFG_2__F_CLK_48M_SEL = 0x2; // CKSYS_BASE (0XA2020000)+ 0x0248, 0:26M, 1:UPLL_48M, 2:MPLL_48M
#endif

    // Sequence to switch to PLL clocks as below:
    // enable clock divider
    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D5_EN = 0x1;   // CKSYS_BASE (0XA2020000)+ 0x0286, to enable digital frequency divider
    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D3_EN = 0x1;   // CKSYS_BASE (0XA2020000)+ 0x0285, to enable digital frequency divider
    *CKSYS_CLK_DIV_1__F_CLK_PLL2_D2_EN = 0x1;   // CKSYS_BASE (0XA2020000)+ 0x0284, to enable digital frequency divider
    *CKSYS_CLK_DIV_2__F_CLK_PLL2_DIV_EN = 0x1;   // CKSYS_BASE (0XA2020000)+ 0x0289, to enable digital frequency divider

    // set clock mux
    *CKSYS_CLK_FORCE_ON_0 = 0x01010101;   // CKSYS_BASE (0XA2020000)+ 0x0270, to force clock on
    *CKSYS_CLK_FORCE_ON_1 = 0x01010101;   // CKSYS_BASE (0XA2020000)+ 0x0274, to force clock on

    *CKSYS_CLK_CFG_0__F_CLK_SYS_SEL = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0240, SYS clock @ 156MHz
    *CKSYS_CLK_CFG_0__F_CLK_SFC_SEL = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0241, SFC clock @ 78MHz
    *CKSYS_CLK_CFG_0__F_CLK_DSP_SEL = 0x2; // CKSYS_BASE (0XA2020000)+ 0x0242, DSP clock @ 104MHz
    *CKSYS_CLK_CFG_0__F_CLK_SPIMST_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0243, SPIMST clock @ 104MHz
    *CKSYS_CLK_CFG_1__F_CLK_BSIBPI_SEL = 0x2; // CKSYS_BASE (0XA2020000)+ 0x0244, BSIBPI clock @ 62.4MHz
    *CKSYS_CLK_CFG_1__F_CLK_SDIOMST0_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0245, SDIOMST0 clock @ 52MHz
    *CKSYS_CLK_CFG_1__F_CLK_SDIOMST1_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0246, SDIOMST1 clock @ 52MHz
    *CKSYS_CLK_CFG_1__F_CLK_SPISLV_SEL = 0x3; // CKSYS_BASE (0XA2020000)+ 0x0247, SPISLV clock @ 104MHz

    *CKSYS_CLK_UPDATE_0__F_CHG_SYS = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0250, SYS clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_SYS ==1); // CKSYS_BASE (0XA2020000)+ 0x0250, wait SYS clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SYS_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0260, wait SYS clock switch

    *CKSYS_CLK_UPDATE_0__F_CHG_SFC = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0251, SFC clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_SFC ==1); // CKSYS_BASE (0XA2020000)+ 0x0251, wait SFC clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SFC_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0261, wait SFC clock switch

    *CKSYS_CLK_UPDATE_0__F_CHG_DSP = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0252, DSP clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_DSP ==1); // CKSYS_BASE (0XA2020000)+ 0x0252, wait DSP clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_DSP_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0262, wait DSP clock switch

    *CKSYS_CLK_UPDATE_0__F_CHG_SPIMST = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0253, SPIMST clock switch
    while (*CKSYS_CLK_UPDATE_0__F_CHG_SPIMST ==1); // CKSYS_BASE (0XA2020000)+ 0x0253, wait SPIMST clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SPIMST_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0263, wait SPIMST clock switch

    *CKSYS_CLK_UPDATE_1__F_CHG_BSIBPI = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0254, BSIBPI clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_BSIBPI ==1); // CKSYS_BASE (0XA2020000)+ 0x0254, wait BSIBPI clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_BSIBPI_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0264, wait BSIBPI clock switch

    *CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST0 = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0255, SDIOMST0 clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST0 ==1); // CKSYS_BASE (0XA2020000)+ 0x0255, wait SDIOMST0 clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SDIOMST0_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0265, wait SDIOMST0 clock switch

    *CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST1 = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0256, SDIOMST1 clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST1 ==1); // CKSYS_BASE (0XA2020000)+ 0x0256, wait SDIOMST1 clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SDIOMST1_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0266, wait SDIOMST1 clock switch
    *CKSYS_CLK_UPDATE_1__F_CHG_SPISLV = 0x1; // CKSYS_BASE (0XA2020000)+ 0x0257, SPISLV clock switch
    while (*CKSYS_CLK_UPDATE_1__F_CHG_SPISLV ==1); // CKSYS_BASE (0XA2020000)+ 0x0257, wait SPISLV clock switch
    while (*CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SPISLV_OK ==1); // CKSYS_BASE (0XA2020000)+ 0x0267, wait SPISLV clock switch

    *CKSYS_CLK_FORCE_ON_0 = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0270, to disable force clock on
    *CKSYS_CLK_FORCE_ON_1 = 0x0; // CKSYS_BASE (0XA2020000)+ 0x0274, to disable force clock on

    efuse_cpu_78m = (*HW_MISC0) & 0x2; // EFUSE_BASE (0XA20A0000)+ 0x0230, bit 1

    //set DCM
    *CLK_FREQ_SWCH__F_RG_PLLCK_SEL = 0x0; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0170, to disable DCM control

    if (efuse_cpu_78m) {
        *BUS_DCM_CON_0__F_RG_BUS_SFSEL = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0100, BUS DCM, idle clock 52M/32 (efuse_cpu_78m: 39M/32)
        *CM4_DCM_CON_0__F_RG_CM_SFSEL = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0100, CM4 DCM, idle clock 104M/32 (efuse_cpu_78m: 78M/32)
    } else {
        *BUS_DCM_CON_0__F_RG_BUS_SFSEL = 0x0; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0100, BUS DCM, idle clock 78M/64
        *CM4_DCM_CON_0__F_RG_CM_SFSEL = 0x0; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0100, CM4 DCM, idle clock 156M/64
    }

    *CLK_FREQ_SWCH__F_RG_PLLCK_SEL = 0x1; // CKSYS_BUS_CLK_BASE (0XA21D0000)+ 0x0170, to enable DCM control
    hal_gpt_delay_us(1);
}

void clock_dump_log(void)
{
#if 0
    /* fix IAR build warning: undefined behavior, the order of volatile accesses is undefined in this statement. */
    uint32_t reg_tmp0, reg_tmp1, reg_tmp2, reg_tmp3, reg_tmp4, reg_tmp5;

    reg_tmp0 = *PDN_COND0_F_PDR_COND0;
    reg_tmp1 = *XO_PDN_COND0;

    /* clock_dump_cg */
    log_hal_info("%s: PDN_COND0=0x%x, XO_PDN_COND0=0x%x\r\n",
                 __FUNCTION__, reg_tmp0, reg_tmp1);

    reg_tmp0 = *CKSYS_CLK_CFG_0_F_CLK_SYS_SEL;
    reg_tmp1 = *CKSYS_CLK_CFG_0_F_CLK_SFC_SEL;
    reg_tmp2 = *CKSYS_CLK_CFG_0_F_CLK_CONN_SEL;
    reg_tmp3 = *CKSYS_CLK_CFG_0_F_CLK_SPIMST_SEL;
    reg_tmp4 = *CKSYS_CLK_CFG_1_F_CLK_XTALCTL_SEL;
    reg_tmp5 = *CKSYS_CLK_CFG_1_F_CLK_SDIOMST_SEL;

    /* clock_dump_mux */
    log_hal_info("%s: (mux)CLK_SYS_SEL=0x%x, (mux)CLK_SFC_SEL=0x%x, (mux)CLK_CONN_SEL=0x%x, (mux)CLK_SPIMST_SEL=0x%x, (mux)CLK_XTALCTL_SEL=0x%x, (mux)CKSYS_CLK_CFG_1_F_CLK_SDIOMST_SEL=0x%x\r\n",
                 __FUNCTION__, reg_tmp0, reg_tmp1, reg_tmp2, reg_tmp3, reg_tmp4, reg_tmp5);

    /* clock_dump_divider */
    reg_tmp0 = *CKSYS_CLK_DIV_0_F_CLK_PLL1_D2_EN;
    reg_tmp1 = *CKSYS_CLK_DIV_0_F_CLK_PLL1_D3_EN;
    reg_tmp2 = *CKSYS_CLK_DIV_0_F_CLK_PLL1_D5_EN;
    reg_tmp3 = *CKSYS_CLK_DIV_0_F_CLK_PLL1_D7_EN;
    reg_tmp4 = *CKSYS_CLK_DIV_2_F_CLK_PLL1_D15_EN;
    reg_tmp5 = *CKSYS_CLK_DIV_2_F_CLK_PLL1_DIV_EN;
    log_hal_info("%s: PLL1 DIV EN(%d), D2(%d), D3(%d), D5(%d), D7(%d), D15(%d)\r\n", __FUNCTION__, reg_tmp5, reg_tmp0, reg_tmp1, reg_tmp2, reg_tmp3, reg_tmp4);

    reg_tmp0 = *CKSYS_CLK_DIV_1_F_CLK_PLL2_D2_EN;
    reg_tmp1 = *CKSYS_CLK_DIV_1_F_CLK_PLL2_D3_EN;
    reg_tmp2 = *CKSYS_CLK_DIV_1_F_CLK_PLL2_D5_EN;
    reg_tmp3 = *CKSYS_CLK_DIV_1_F_CLK_PLL2_D7_EN;
    reg_tmp4 = *CKSYS_CLK_DIV_2_F_CLK_PLL2_D15_EN;
    reg_tmp5 = *CKSYS_CLK_DIV_2_F_CLK_PLL2_DIV_EN;
    log_hal_info("%s: PLL2 DIV EN(%d), D2(%d), D3(%d), D5(%d), D7(%d), D15(%d)\r\n", __FUNCTION__, reg_tmp5, reg_tmp0, reg_tmp1, reg_tmp2, reg_tmp3, reg_tmp4);

    reg_tmp0 = *CKSYS_CLK_DIV_3;
    reg_tmp1 = *CKSYS_CLK_DIV_4;
    reg_tmp2 = *CKSYS_CLK_DIV_5;
    reg_tmp3 = *CKSYS_XTAL_FREQ;
    log_hal_info("%s: CLK_DIV_3=0x%x, CLK_DIV_4=0x%x, CLK_DIV_5=0x%x, CLK_XTAL_FREQ=0x%x\r\n",
                 __FUNCTION__, reg_tmp0, reg_tmp1, reg_tmp2, reg_tmp3);
#else
    return;
#endif
}



void clock_dump_info(void)
{
    clock_dump_log();
}

/*ATTR_TEXT_IN_TCM int clk_init(void)
{
    if (clock_initialized) {
        return 0;
    }

    clock_initialized = true;

    return 0;
}*/

ATTR_TEXT_IN_TCM hal_clock_status_t hal_clock_init(void)
{
    //int32_t ret;

    /*ret = clk_init();

    return (hal_clock_status_t)(ret);*/

    hal_clock_disable(HAL_CLOCK_CG_TRNG);
    hal_clock_disable(HAL_CLOCK_CG_AESOTF);
    hal_clock_disable(HAL_CLOCK_CG_UART0);
    hal_clock_disable(HAL_CLOCK_CG_SDIOSLV);
    hal_clock_disable(HAL_CLOCK_CG_USB48M);
    hal_clock_disable(HAL_CLOCK_CG_USB_BUS);
    hal_clock_disable(HAL_CLOCK_CG_CIPHER);
    hal_clock_disable(HAL_CLOCK_CG_SEJ);
    hal_clock_disable(HAL_CLOCK_CG_AUXADC);
    hal_clock_disable(HAL_CLOCK_CG_CRYPTO);
    hal_clock_disable(HAL_CLOCK_CG_EFUSE);
    hal_clock_disable(HAL_CLOCK_CG_SPM);

    return  HAL_CLOCK_STATUS_OK;
}

hal_clock_status_t hal_clock_FREF_force_output(bool enable)
{
    *DCXO_PCON1__F_FRC_COCLK_EN = enable ? 1 : 0;

    return  HAL_CLOCK_STATUS_OK;
}

struct HOPPING_TABLE {
    int8_t range;
    int32_t soft_start;
    int32_t free_run_range;
};

struct HOPPING_TABLE hopping_range[9] = {
// range,   soft_start, free_run_range
    { 0,    0x800,      0x0},
    {-1,    0x83E,      0xF},
    {-2,    0x87D,      0x1F},
    {-3,    0x8BE,      0x2F},
    {-4,    0x900,      0x40},
    {-5,    0x943,      0x50},
    {-6,    0x988,      0x62},
    {-7,    0x9CE,      0x73},
    {-8,    0xA16,      0x85},
};

hal_clock_status_t hal_clock_set_mpll_hopping(int start, int range)
{
    if ((start > 0) || (start < -8) || (range > 0) || (start + range < -8))
        return HAL_CLOCK_STATUS_INVALID_PARAMETER;

    *FH_CON4__F_MPLL_FRDDS_EN = 0x0;        // MIXED_BASE(0xA2040000)+ 0x0512, disable free run

    // Sequence to enable MPLL soft start to -1% and free run -1%~-8%
    // MPLL have been enabled at boot loader clock initial flow
    // Need to check MPLL is on before change
    /*
    soft start -1% + free run 0% ~ -7% ---> -1% ~ -8%
    */
    *FH_CON3__F_MPLL_FRDDS_DTS = 0x1;       // MIXED_BASE(0xA2040000) + 0x050d, free run delta time 1us
    *FH_CON3__F_MPLL_FRDDS_DYS = 0x3;       // MIXED_BASE(0xA2040000) + 0x050e, free run step 16
    *FH_CON3__F_MPLL_SFSTR_DYS= 0x2;        // MIXED_BASE(0xA2040000) + 0x050c, soft start delta time 1us
    *FH_CON4__F_MPLL_SFSTR_DTS = 0x1;       // MIXED_BASE(0xA2040000) + 0x0513, soft start step 16
    *FH_CON4__F_MPLL_FHCTL_EN = 0x1;        // MIXED_BASE(0xA2040000) + 0x0510, change D2A interface to control by FHCTL module
    if (start == 0)
        *FH_CON4__F_MPLL_SFSTR_EN= 0x0;         // MIXED_BASE(0xA2040000) + 0x0511, enable soft start
    else
        *FH_CON4__F_MPLL_SFSTR_EN= 0x1;         // MIXED_BASE(0xA2040000) + 0x0511, enable soft start
    *FH_CON0__F_PLL_FHSET = hopping_range[0 - start].soft_start;          // MIXED_BASE(0xA2040000) + 0x0500, set PLL_FHSET
    *FH_CON0__F_PLL_FREQ_STR =  ~(*FH_CON0__F_PLL_FREQ_STR); // MIXED_BASE(0xA2040000) + 0x0502, toggle PLL_FHRQ_STR

#if 0
    // wait for 6us for soft start to -1%, (0x83E - 0x800)/16*1us ~= 6
    hal_gpt_delay_us(6);
#endif

    *FH_CON5__F_MPLL_FRDDS_DNLMT = 0x0;     // MIXED_BASE(0xA2040000) + 0x0514, up limit -1%
    *FH_CON5__F_MPLL_FRDDS_UPLMT = hopping_range[0 - range].free_run_range;    // MIXED_BASE(0xA2040000) + 0x0516, down limit -8%
    if (range != 0)
        *FH_CON4__F_MPLL_FRDDS_EN = 0x1;        // MIXED_BASE(0xA2040000)+ 0x0512, enable free run
/*
    log_hal_info("Set MPLL hopping to %d ~ %d\r\n",
                    hopping_range[SOFT_START_HOPPING_TARGET].range,
                    hopping_range[SOFT_START_HOPPING_TARGET].range + hopping_range[DOWN_HOPPING_LIMIT].range);
*/
    return  HAL_CLOCK_STATUS_OK;
}

#ifdef CLK_SWITCH_TEST

#include "syslog.h"
#include "hal_pmu.h"

#define CLOCK_BOTTOM_TO_TOP 0

struct CLOCK_SEL_TABLE{
    char name[128];
    unsigned char tcksel_num;
    char sel[6];    //0P9->0, 1P1->1, 1P3->2, if the voltage is less than this value, can not switch
};

struct CLOCK_SEL_TABLE clock[9] = {
        // name,            tcksel_num      sel_0, sel_1, sel_2, sel_3, sel_4, sel_5
        {"hf_fsys_ck",      19,             {0,     2,     2,     1,     1,     1}},
        {"hf_fsfc_ck",      17,             {0,     1,     1,     1,    -1,    -1}},
        {"hf_fdsp_ck",      18,             {0,     2,     1,     1,     1,    -1}},
        {"hf_fspimst_ck",   20,             {0,     1,     1,     1,    -1,    -1}},
        {"hf_fbsibpi_ck",   22,             {0,     1,     1,    -1,    -1,    -1}},
        {"hf_fsdiomst0_ck", 21,             {0,     1,     1,     1,    -1,    -1}},
        {"hf_fsdiomst1_ck", 28,             {0,     1,     1,     1,    -1,    -1}},
        {"hf_fspislv_ck",   29,             {0,     1,     1,     1,    -1,    -1}},
        {"f_f48m_ck",        5,             {0,     1,     1,    -1,    -1,    -1}},
};

void _48M_clock_enable(void)
{
    volatile uint32_t * clr_addr = NULL;
    clr_addr = XO_PDN_CLRD0;
    *(clr_addr) = (0x1 << 0);    /* HW Register is write 1 clear */
}

void _48M_clock_disable(void)
{
    volatile uint32_t * set_addr = NULL;
    set_addr = XO_PDN_SETD0;
    *(set_addr) = (0x1 << 0);    /* HW Register is write 1 clear */
}

void clock_switch_test(void)
{
    uint32_t freq = 0;
    int8_t _voltage = 0;
    uint8_t _clock = 0;
    uint8_t _mux_sel = 0;
    void (*_switch_clock[3])(void);

    _switch_clock[0] = hal_clock_set_switch_to_0P9V;
    _switch_clock[1] = hal_clock_set_switch_to_1P1V;
    _switch_clock[2] = hal_clock_set_switch_to_1P3V;

    log_hal_info("clock_switch_test start: \r\n");

    hal_clock_enable(HAL_CLOCK_CG_SPIMST0);     //default OFF
    hal_clock_enable(HAL_CLOCK_CG_SPIMST1);
    hal_clock_enable(HAL_CLOCK_CG_SDIOMST0);
    hal_clock_enable(HAL_CLOCK_CG_SDIOMST1);
    hal_clock_enable(HAL_CLOCK_CG_SPISLV);
    hal_clock_enable(HAL_CLOCK_CG_48M);

#if CLOCK_BOTTOM_TO_TOP
    for(_voltage = 0;_voltage < 3;_voltage++)
#else
    for(_voltage = 2;_voltage >= 0;_voltage--)
#endif
    {
        log_hal_info("voltage = %d\r\n", _voltage);

#if CLOCK_BOTTOM_TO_TOP
        pmu_set_vcore_voltage(PMU_VCORE_LOCK, (_voltage+1)*2);    //0P9V = 2, 1P1V = 4, 1P3V = 6,
        _switch_clock[_voltage]();
#else
        _switch_clock[_voltage]();
        pmu_set_vcore_voltage(PMU_VCORE_LOCK, (_voltage+1)*2);    //0P9V = 2, 1P1V = 4, 1P3V = 6,
#endif
        _48M_clock_enable();     //switch to 0P9/1P1/1P3V will turn off 48M CG to prevent glitch

        for(_clock = 0;_clock < 9;_clock++)
        {
            for(_mux_sel = 0;_mux_sel < 6;_mux_sel++)
            {
                if(clock[_clock].sel[_mux_sel] <= _voltage)
                {
                    clock_mux_sel(_clock, _mux_sel);
                    freq = hal_clock_get_freq_meter(clock[_clock].tcksel_num, 0);
                    log_hal_info("name = %s, sel = %d, freq = %d\r\n", clock[_clock].name, _mux_sel, freq);
                }
            }
        }
        pmu_set_vcore_voltage(PMU_VCORE_UNLOCK, (_voltage+1)*2);
    }

    hal_clock_disable(HAL_CLOCK_CG_SPIMST0);
    hal_clock_disable(HAL_CLOCK_CG_SPIMST1);
    hal_clock_disable(HAL_CLOCK_CG_SDIOMST0);
    hal_clock_disable(HAL_CLOCK_CG_SDIOMST1);
    hal_clock_disable(HAL_CLOCK_CG_SPISLV);
    hal_clock_disable(HAL_CLOCK_CG_48M);

    _48M_clock_disable();

    log_hal_info("clock_switch_test end\r\n");
}
#endif  //CLK_SWITCH_TEST

#ifdef CLK_DCM_TEST
#include "hal_platform.h"

#define GPIO_CLKO_CTRL_A_F_CLKO_MODE0   ((volatile uint8_t*)(0xA2010000))
#define GPIO_CLKO_CTRL_A_F_CLKO_MODE1   ((volatile uint8_t*)(0xA2010001))
#define GPIO_CLKO_CTRL_A_F_CLKO_MODE2   ((volatile uint8_t*)(0xA2010002))
#define GPIO_CLKO_CTRL_A_F_CLKO_MODE3   ((volatile uint8_t*)(0xA2010003))
#define GPIO_CLKO_CTRL_B_F_CLKO_MODE4   ((volatile uint8_t*)(0xA2010004))
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

#define __CPU_DELAY(ptr) __asm volatile(  \
    "PUSH   {r0-r12, lr}                \n"\
    "MOV    r2,         %0              \n"\
    "MOV    r0,         r2              \n"\
    "LOOP:  SUBS    r0,r0,#1            \n"\
    "CMP    r0,         0               \n"\
    "BNE    LOOP                        \n"\
    "POP    {r0-r12,lr}                   "\
    : :"r" (ptr):                          \
);

#define TEST_SET    0

static void __MAKE_BUS_BUSY(void)
{
    uint32_t volatile tmp = 0;
    int i = 0;
    for(i = 0;i< 0xAA;i++)
        tmp = *((volatile uint32_t*)(0x04200000));  //sysram start addr
}

static void __MAKE_SFC_BUSY(void)
{
    uint32_t volatile tmp = 0;
    int i = 0;
    for(i = 0;i< 0xAA;i++)
        tmp = *((volatile uint32_t*)(0x08002000));  //sysram start addr
}

void clock_dcm_test(void)
{
    uint32_t val = 0;
    uint8_t test_set = TEST_SET;

    log_hal_info("clock_dcm_test start: \r\n");

    if(test_set == 0){                          // nothing to do with idle :
        *GPIO_CLKO_CTRL_A_F_CLKO_MODE0 = 0;     //      4'd0:  f_frtc_clkout_ck
        *GPIO_CLKO_CTRL_A_F_CLKO_MODE1 = 1;     //      4'd1:  f_fxo_ck
        *GPIO_CLKO_CTRL_A_F_CLKO_MODE2 = 2;     //      4'd2:  mpll_48m_ck
        *GPIO_CLKO_CTRL_A_F_CLKO_MODE3 = 3;     //      4'd3:  upll_48m_ck
        *GPIO_CLKO_CTRL_B_F_CLKO_MODE4 = 4;     //      4'd4:  f_fxo_d2_ck
    }else if (test_set == 1){                   // nothing to do with idle :
        *GPIO_CLKO_CTRL_A_F_CLKO_MODE0 = 5;     //      4'd5:  f_f39m_ck
        *GPIO_CLKO_CTRL_A_F_CLKO_MODE1 = 6;     //      4'd6:  AD_XPLL_CK
        *GPIO_CLKO_CTRL_A_F_CLKO_MODE3 = 7;     //      4'd7:  AD_26M_DBB
    }else if (test_set == 2){                   // cm_clk_idle :
        *GPIO_CLKO_CTRL_A_F_CLKO_MODE0 = 15;    //      4'd15: hf_fcm_mclk_ck div4
    }else if (test_set == 3){                   // emi_idle :
        *GPIO_CLKO_CTRL_A_F_CLKO_MODE0 = 9;     //      4'd9:  hf_fbus_emi1x_phy_hclk_ck div2
        *GPIO_CLKO_CTRL_A_F_CLKO_MODE1 = 14;    //      4'd14: hf_fsys_emi2x_phy_hclk_ck div4
    }else if (test_set == 4){                   // xo_clk_idle:
        *GPIO_CLKO_CTRL_A_F_CLKO_MODE0 = 12;    //      4'd12: f_fxo_hclk_ck div2
        *GPIO_CLKO_CTRL_A_F_CLKO_MODE1 = 13;    //      4'd13: f_fxo_mclk_ck div2
    }else if (test_set == 5){                   // bus_clk_idle:
        *GPIO_CLKO_CTRL_A_F_CLKO_MODE0 = 10;    //      4'd10: hf_fbus_hclk_ck div2
        *GPIO_CLKO_CTRL_A_F_CLKO_MODE1 = 11;    //      4'd11: hf_fbus_mclk_ck div2
    }else if (test_set == 6){                   // sfc_idle:
        *GPIO_CLKO_CTRL_A_F_CLKO_MODE0 = 8;     //      4'd8:  hf_fsfc_phy_hclk_ck div2
    }else if (test_set == 7){                   // test
        *GPIO_CLKO_CTRL_A_F_CLKO_MODE2 = 0;     //      4'd0:  f_frtc_clkout_ck
    }

    // GPIO0 aux. 5 for O:CLKO0
    hal_gpio_init(HAL_GPIO_0);
    hal_pinmux_set_function(HAL_GPIO_0, 5);

    // GPIO12 aux. 4 for O:CLKO1
    hal_gpio_init(HAL_GPIO_12);
    hal_pinmux_set_function(HAL_GPIO_12, 4);

    // GPIO14 aux. 5 for O:CLKO2
    hal_gpio_init(HAL_GPIO_14);
    hal_pinmux_set_function(HAL_GPIO_14, 5);

    // GPIO24 aux. 5 for O:CLKO3
    hal_gpio_init(HAL_GPIO_24);
    hal_pinmux_set_function(HAL_GPIO_24, 5);

    // GPIO29 aux. 5 for O:CLKO4
    hal_gpio_init(HAL_GPIO_29);
    hal_pinmux_set_function(HAL_GPIO_29, 5);

    // GPIO1 aux. 8 for DEBUGMON1:cm_clk_idle
    hal_gpio_init(HAL_GPIO_1);
    hal_pinmux_set_function(HAL_GPIO_1, 8);

    // GPIO3 aux. 8 for DEBUGMON3:bus_clk_idle
    hal_gpio_init(HAL_GPIO_3);
    hal_pinmux_set_function(HAL_GPIO_3, 8);

    // GPIO5 aux. 8 for DEBUGMON5:xo_clk_idle
    hal_gpio_init(HAL_GPIO_5);
    hal_pinmux_set_function(HAL_GPIO_5, 8);

    // GPIO6 aux. 8 for DEBUGMON6:emi_idle
    hal_gpio_init(HAL_GPIO_6);
    hal_pinmux_set_function(HAL_GPIO_6, 8);

    // GPIO8 aux. 8 for DEBUGMON8:sfc_idle
    hal_gpio_init(HAL_GPIO_8);
    hal_pinmux_set_function(HAL_GPIO_8, 8);

    // enable debug monitor for idle signal
    *TOP_DEBUG = 0x17;

    __CPU_DELAY(0xFFFFF);

    //disable systick and interrupt
    *CM4_SYSTICK_CTL = 0x0;

    //set CM4 deep sleep
    val = *CM4_SYSTEM_CONTROL;
    val |= 0x4;
    *CM4_SYSTEM_CONTROL = val;

    //set reload value
    *CM4_SYSTICK_RELOAD_VALUE = 0x1;

    //log_hal_info("enter idle\r\n");
    //printf("enter idle\r\n");

    //enable systick and interrupt
    *CM4_SYSTICK_CTL = 0x3;

    //__MAKE_BUS_BUSY();
    //__MAKE_SFC_BUSY();

    __ENTER_IDLE();

    //__MAKE_SFC_BUSY();
    //__MAKE_BUS_BUSY();

    //log_hal_info("exit idle\r\n");
    //printf("exit idle\r\n");
    log_hal_info("clock_dcm_test end\r\n");
}

void clock_efuse_test(void)
{
    //PMU VSIM
    *((volatile uint32_t*)(0xA2070000+0x0A44)) = 0x1;
    hal_gpt_delay_us(200);
    //printf("0xA2070000+0x0A44 = 0x%08x\r\n", *((volatile uint32_t*)(0xA2070000+0x0A44 )) );

    *((volatile uint32_t*)(0xA20A0008 )) = 0xA07923B6;   //magic key
    //printf("0xA20A0008 = 0x%08x\r\n", *((volatile uint32_t*)(0xA20A0008 )) );
#if 0
    *((volatile uint32_t*)(0XA20A0230 )) = 0x1; //efuse_dis_output_32k, bit 0
#else
    *((volatile uint32_t*)(0XA20A0230 )) = 0x2; //efuse_cpu_78m, bit 1
#endif
    *((volatile uint32_t*)(0xA20A0000 )) = 0x10000;

    printf("0XA20A0230 = 0x%08x\r\n", *((volatile uint32_t*)(0XA20A0230 )) );
}
#endif  //CLK_DCM_TEST

#ifdef CLK_REALTIME_TEST
void clock_disable_IRQ_time(void)
{
    uint32_t _test_time_1 = 0;
    uint32_t _test_time_2 = 0;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &_test_time_1);
    //hal_clock_enable(HAL_CLOCK_CG_UART3);
    hal_gpt_delay_us(100);
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &_test_time_2);
    printf("enable, %d, %d\r\n", _test_time_1, _test_time_2);
    ////////////////////////////////////////////////////////////////////
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &_test_time_1);
    //hal_clock_disable(HAL_CLOCK_CG_UART3);
    hal_gpt_delay_us(250);
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &_test_time_2);
    printf("disable, %d, %d\r\n", _test_time_1, _test_time_2);
}
#endif  //CLK_REALTIME_TEST

#ifdef CLK_HOPPING_TEST

#define HOPPING_MODE    2
/*
HOPPING_MODE
0 : free run only
1 : soft start only
2 : soft start + free run
*/

/*
DOWN_HOPPING_LIMIT & SOFT_START_HOPPING_TARGET
0 :  0%
1 : -1%
2 : -2%
3 : -3%
4 : -4%
5 : -5%
6 : -6%
7 : -7%
8 : -8%
*/

#define MEASURE_TIME 30

void __48M_clock_enable(void)
{
    volatile uint32_t * clr_addr = NULL;
    clr_addr = XO_PDN_CLRD0;
    *(clr_addr) = (0x1 << 0);    /* HW Register is write 1 clear */
}

void __48M_clock_disable(void)
{
    volatile uint32_t * set_addr = NULL;
    set_addr = XO_PDN_SETD0;
    *(set_addr) = (0x1 << 0);    /* HW Register is write 1 clear */
}

void clock_pll_hopping_test(void)
{
    uint8_t mode = HOPPING_MODE;
    uint32_t i = 0;
    uint32_t mpll_meter_record[MEASURE_TIME];
    uint32_t upll_meter_record[MEASURE_TIME];

    if(mode == 0){
        // Sequence to enable MPLL free run 0%~-8%
        *FH_CON5__F_MPLL_FRDDS_DNLMT = 0x0;     // MIXED_BASE(0xA2040000) + 0x0514, up limit 0%
        *FH_CON5__F_MPLL_FRDDS_UPLMT = hopping_range[DOWN_HOPPING_LIMIT].free_run_range; // MIXED_BASE(0xA2040000) + 0x0516, down limit
        *FH_CON3__F_MPLL_FRDDS_DTS = 0x1;       // MIXED_BASE(0xA2040000) + 0x050d, free run delta time 1us
        *FH_CON3__F_MPLL_FRDDS_DYS = 0x3;       // MIXED_BASE(0xA2040000) + 0x050e, free run step 16
        *FH_CON4__F_MPLL_FHCTL_EN = 0x1;        // MIXED_BASE(0xA2040000)+ 0x0510, change D2A interface to control by FHCTL module
        *FH_CON4__F_MPLL_FRDDS_EN = 0x1;        // MIXED_BASE(0xA2040000)+ 0x0512, enable free run
    } else if (mode == 1){
        *FH_CON3__F_MPLL_SFSTR_DYS= 0x2;        // MIXED_BASE(0xA2040000) + 0x050c, soft start delta time 1us
        *FH_CON4__F_MPLL_SFSTR_DTS = 0x1;       // MIXED_BASE(0xA2040000) + 0x0513, soft start step 16
        *FH_CON4__F_MPLL_FHCTL_EN = 0x1;        // MIXED_BASE(0xA2040000) + 0x0510, change D2A interface to control by FHCTL module
        *FH_CON4__F_MPLL_SFSTR_EN= 0x1;         // MIXED_BASE(0xA2040000) + 0x0511, enable soft start
        *FH_CON0__F_PLL_FHSET = hopping_range[SOFT_START_HOPPING_TARGET].soft_start;          // MIXED_BASE(0xA2040000) + 0x0500, set PLL_FHSET
        *FH_CON0__F_PLL_FREQ_STR =  ~(*FH_CON0__F_PLL_FREQ_STR); // MIXED_BASE(0xA2040000) + 0x0502, toggle PLL_FHRQ_STR

        // wait for 33 us for soft start to -8%, (0xA16 - 0x800)/16*1us ~= 33
        hal_gpt_delay_us(35);
    } else {
        // Sequence to enable MPLL soft start to -1% and free run -1%~-8%
        // MPLL have been enabled at boot loader clock initial flow
        // Need to check MPLL is on before change
        /*
        soft start -1% + free run 0% ~ -7% ---> -1% ~ -8%
        */
        *FH_CON3__F_MPLL_FRDDS_DTS = 0x1;       // MIXED_BASE(0xA2040000) + 0x050d, free run delta time 1us
        *FH_CON3__F_MPLL_FRDDS_DYS = 0x3;       // MIXED_BASE(0xA2040000) + 0x050e, free run step 16
        *FH_CON3__F_MPLL_SFSTR_DYS= 0x2;        // MIXED_BASE(0xA2040000) + 0x050c, soft start delta time 1us
        *FH_CON4__F_MPLL_SFSTR_DTS = 0x1;       // MIXED_BASE(0xA2040000) + 0x0513, soft start step 16
        *FH_CON4__F_MPLL_FHCTL_EN = 0x1;        // MIXED_BASE(0xA2040000) + 0x0510, change D2A interface to control by FHCTL module
        *FH_CON4__F_MPLL_SFSTR_EN= 0x1;         // MIXED_BASE(0xA2040000) + 0x0511, enable soft start
        *FH_CON0__F_PLL_FHSET = hopping_range[SOFT_START_HOPPING_TARGET].soft_start;          // MIXED_BASE(0xA2040000) + 0x0500, set PLL_FHSET
        *FH_CON0__F_PLL_FREQ_STR =  ~(*FH_CON0__F_PLL_FREQ_STR); // MIXED_BASE(0xA2040000) + 0x0502, toggle PLL_FHRQ_STR

        // wait for 6us for soft start to -1%, (0x83E - 0x800)/16*1us ~= 6
        hal_gpt_delay_us(6);

        *FH_CON5__F_MPLL_FRDDS_DNLMT = 0x0;     // MIXED_BASE(0xA2040000) + 0x0514, up limit -1%
        *FH_CON5__F_MPLL_FRDDS_UPLMT = hopping_range[DOWN_HOPPING_LIMIT].free_run_range;    // MIXED_BASE(0xA2040000) + 0x0516, down limit -8%
        *FH_CON4__F_MPLL_FRDDS_EN = 0x1;        // MIXED_BASE(0xA2040000)+ 0x0512, enable free run
    }

    clock_mux_sel(CLK_48M_SEL, 1);  // in order to measure UPLL_48M

    __48M_clock_enable();

    for(i = 0;i < MEASURE_TIME;i++){
        mpll_meter_record[i] = hal_clock_get_freq_meter(4, 0);  //4: clk_pll2_d2
    }

    for(i = 0;i < MEASURE_TIME;i++){
        upll_meter_record[i] = hal_clock_get_freq_meter(5, 0);  //5: f_f48m_ck
    }

    printf("mpll meter data:\r\n");
    for(i = 0;i < MEASURE_TIME;i++){
        printf("%d\r\n", mpll_meter_record[i]);
    }

    printf("upll meter data:\r\n");
    for(i = 0;i < MEASURE_TIME;i++){
        printf("%d\r\n", upll_meter_record[i]);
    }

    __48M_clock_disable();
}

#endif  //CLK_HOPPING_TEST

#endif /* HAL_CLOCK_MODULE_ENABLED */

