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

#include "hal_pmu.h"
#include "hal_gpt.h"
#ifdef HAL_SLEEP_MANAGER_ENABLED

#ifdef  PMU_DEBUG_ENABLE
#define log_debug(_message,...) printf(_message, ##__VA_ARGS__)
#else
#define log_debug(_message,...)
#endif

extern bool chip_is_mt2625E3(void);// For E3

void pmu_init(void)
{
    dump_power_on_event();

    pmu_efuse_sw_load();

    /* disable VBATSNS for AUXADC */
    pmu_set_register_value(PMU_RG_EN_VBATSNS_ROUT_ADDR  , PMU_RG_EN_VBATSNS_ROUT_MASK    , PMU_RG_EN_VBATSNS_ROUT_SHIFT   , 0);

    /* disable Temperature Sensing Output for AUXADC */
    pmu_set_register_value(PMU_RG_ADC_TS_EN_ADDR        , PMU_RG_ADC_TS_EN_MASK          , PMU_RG_ADC_TS_EN_SHIFT         , 0);

    /* Set Vcore Voltage to 0.7V in sleep mode */
    pmu_set_register_value(PMU_RG_VCORE_VOSEL_LPM_ADDR  , PMU_RG_VCORE_VOSEL_LPM_MASK    , PMU_RG_VCORE_VOSEL_LPM_SHIFT   , 1);
    hal_gpt_delay_us(200);

#ifdef LONG_PRESS_SHUTDOWN_ENABLE
    pmu_set_register_value(PMU_RG_T_LONGPRESS_SEL_ADDR  , PMU_RG_T_LONGPRESS_SEL_MASK   , PMU_RG_T_LONGPRESS_SEL_SHIFT  , LONG_PRESS_SHUTDOWN_TIME_SEL);
    pmu_ctrl_longpress_shutdown(PMU_CTL_ENABLE);

#ifdef LONG_PRESS_SHUTDOWN_EDGE_TRIGGER
    pmu_set_register_value(PMU_RG_LONGPRESS_TYPE_SEL_ADDR   , PMU_RG_LONGPRESS_TYPE_SEL_MASK    , PMU_RG_LONGPRESS_TYPE_SEL_SHIFT   , 1);
#endif

#else
    pmu_ctrl_longpress_shutdown(PMU_CTL_DISABLE);
#endif

    if (pmu_get_register_value(PMU_RGS_VSRAM_EN_ADDR     , PMU_RGS_VSRAM_EN_MASK      , PMU_RGS_VSRAM_EN_SHIFT) == 0x01) {
        pmu_ctrl_power(PMU_VSRAM, PMU_CTL_ENABLE);
    }

    pmu_set_register_value(PMU_RG_VPA_VHSEL_ADDR                , PMU_RG_VPA_VHSEL_MASK                 , PMU_RG_VPA_VHSEL_SHIFT                , 2);   
    pmu_set_register_value(PMU_RG_BUCK_VPA_OC_SDN_EN_ADDR       , PMU_RG_BUCK_VPA_OC_SDN_EN_MASK        , PMU_RG_BUCK_VPA_OC_SDN_EN_SHIFT       , 1);
    pmu_set_register_value(PMU_RG_BUCK_VPA_MSFG_RRATE0_ADDR     , PMU_RG_BUCK_VPA_MSFG_RRATE0_MASK      , PMU_RG_BUCK_VPA_MSFG_RRATE0_SHIFT     , 22);
    pmu_set_register_value(PMU_RG_BUCK_VPA_MSFG_RRATE1_ADDR     , PMU_RG_BUCK_VPA_MSFG_RRATE1_MASK      , PMU_RG_BUCK_VPA_MSFG_RRATE1_SHIFT     , 22);
    pmu_set_register_value(PMU_RG_BUCK_VPA_MSFG_RRATE2_ADDR     , PMU_RG_BUCK_VPA_MSFG_RRATE2_MASK      , PMU_RG_BUCK_VPA_MSFG_RRATE2_SHIFT     , 22);
    pmu_set_register_value(PMU_RG_BUCK_VPA_MSFG_RRATE3_ADDR     , PMU_RG_BUCK_VPA_MSFG_RRATE3_MASK      , PMU_RG_BUCK_VPA_MSFG_RRATE3_SHIFT     , 22);
    pmu_set_register_value(PMU_RG_BUCK_VPA_MSFG_RRATE4_ADDR     , PMU_RG_BUCK_VPA_MSFG_RRATE4_MASK      , PMU_RG_BUCK_VPA_MSFG_RRATE4_SHIFT     , 22);
    pmu_set_register_value(PMU_RG_BUCK_VPA_MSFG_RRATE5_ADDR     , PMU_RG_BUCK_VPA_MSFG_RRATE5_MASK      , PMU_RG_BUCK_VPA_MSFG_RRATE5_SHIFT     , 22);
    pmu_set_register_value(PMU_RG_BUCK_VPA_MSFG_FRATE0_ADDR     , PMU_RG_BUCK_VPA_MSFG_FRATE0_MASK      , PMU_RG_BUCK_VPA_MSFG_FRATE0_SHIFT     , 63);
    pmu_set_register_value(PMU_RG_BUCK_VPA_MSFG_FRATE1_ADDR     , PMU_RG_BUCK_VPA_MSFG_FRATE1_MASK      , PMU_RG_BUCK_VPA_MSFG_FRATE1_SHIFT     , 63);
    pmu_set_register_value(PMU_RG_BUCK_VPA_MSFG_FRATE2_ADDR     , PMU_RG_BUCK_VPA_MSFG_FRATE2_MASK      , PMU_RG_BUCK_VPA_MSFG_FRATE2_SHIFT     , 63);
    pmu_set_register_value(PMU_RG_BUCK_VPA_MSFG_FRATE3_ADDR     , PMU_RG_BUCK_VPA_MSFG_FRATE3_MASK      , PMU_RG_BUCK_VPA_MSFG_FRATE3_SHIFT     , 63);
    pmu_set_register_value(PMU_RG_BUCK_VPA_MSFG_FRATE4_ADDR     , PMU_RG_BUCK_VPA_MSFG_FRATE4_MASK      , PMU_RG_BUCK_VPA_MSFG_FRATE4_SHIFT     , 63);
    pmu_set_register_value(PMU_RG_BUCK_VPA_MSFG_FRATE5_ADDR     , PMU_RG_BUCK_VPA_MSFG_FRATE5_MASK      , PMU_RG_BUCK_VPA_MSFG_FRATE5_SHIFT     , 63);
    pmu_set_register_value(PMU_RG_BUCK_VPA_DVS_TRANST_ONCE_ADDR , PMU_RG_BUCK_VPA_DVS_TRANST_ONCE_MASK  , PMU_RG_BUCK_VPA_DVS_TRANST_ONCE_SHIFT , 0);
    pmu_set_register_value(PMU_RG_BUCK_VPA_DVS_TRANST_TD_ADDR   , PMU_RG_BUCK_VPA_DVS_TRANST_TD_MASK    , PMU_RG_BUCK_VPA_DVS_TRANST_TD_SHIFT   , 3);
    pmu_set_register_value(PMU_RG_DEG_WND_OC_VPA_ADDR           , PMU_RG_DEG_WND_OC_VPA_MASK            , PMU_RG_DEG_WND_OC_VPA_SHIFT           , 3);
    pmu_set_register_value(PMU_RG_VPA_CSMIR_ADDR                , PMU_RG_VPA_CSMIR_MASK                 , PMU_RG_VPA_CSMIR_SHIFT                , 0);
    pmu_set_register_value(PMU_RG_VPA_CSL_ADDR                  , PMU_RG_VPA_CSL_MASK                   , PMU_RG_VPA_CSL_SHIFT                  , 3);/**/
    pmu_set_register_value(PMU_RG_VPA_RSV2_ADDR                 , PMU_RG_VPA_RSV2_MASK                  , PMU_RG_VPA_RSV2_SHIFT                 , 0x8C);
    pmu_set_register_value(PMU_RG_VCORE_IPK_OS_LPM_ADDR         , PMU_RG_VCORE_IPK_OS_LPM_MASK          , PMU_RG_VCORE_IPK_OS_LPM_SHIFT         , 2);
    pmu_set_register_value(PMU_RG_VRF_RSV1_ADDR                 , PMU_RG_VRF_RSV1_MASK                  , PMU_RG_VRF_RSV1_SHIFT                 , 0x08);
    pmu_set_register_value(PMU_RG_VCORE_DVS_STEP_SEL_ADDR       , PMU_RG_VCORE_DVS_STEP_SEL_MASK        , PMU_RG_VCORE_DVS_STEP_SEL_SHIFT       , 1);
    
    /* Normal Mode VCORE=1.3V~0.9V control, It is used to enable the MIN_ON function of Buck_DIG. */
    pmu_set_register_value(PMU_RG_VCORE_RSV1_ADDR               , PMU_RG_VCORE_RSV1_MASK                , PMU_RG_VCORE_RSV1_SHIFT               , 0x08);

    if(chip_is_mt2625E3() == true) {
        //E3 - LPBG Enable Settings
        pmu_set_register_value(((uint32_t)(0xA2070408)) , 1 , 14 , 1);
    }

    /* Check 0xA20A0200[7:0] for apply Ibias * 1.5 */
    if ((*((volatile unsigned int *)0xA20A0200) & 0xFF) <=  0x03) {
        uint32_t value;
        value = pmu_get_register_value(PMU_RG_LPBG_IBTRIM_ADDR  , PMU_RG_LPBG_IBTRIM_MASK    , PMU_RG_LPBG_IBTRIM_SHIFT);
        if (value <= (0x1F - 0x0A)) {
            value = value + 0x0A;
            pmu_set_register_value(PMU_RG_LPBG_IBTRIM_ADDR, PMU_RG_LPBG_IBTRIM_MASK , PMU_RG_LPBG_IBTRIM_SHIFT , value);
        } else {
            pmu_set_register_value(PMU_RG_LPBG_IBTRIM_ADDR, PMU_RG_LPBG_IBTRIM_MASK , PMU_RG_LPBG_IBTRIM_SHIFT , 0x1F);
        }
    }

    /* VRF voltage increased to 1.3V to fix Tx spurious emissions (36.521 6.6.3F.2) */
    pmu_set_vrf_voltage(PMU_VRF_VOL_1P30V);

    pmu_set_ao_latch();
}

void pmu_set_register_value(uint32_t address, uint32_t mask, uint32_t shift, uint32_t value)
{
    uint32_t mask_buffer,target_value;
    mask_buffer = (~(mask << shift));    
    target_value = *((volatile uint32_t *)(address));
    target_value &= mask_buffer;
    target_value |= (value << shift);
    *((volatile uint32_t *)(address)) = target_value;
}

uint32_t pmu_get_register_value(uint32_t address, uint32_t mask, uint32_t shift)
{
    uint32_t change_value, mask_buffer;
    mask_buffer = (mask << shift);
    change_value = *((volatile uint32_t *)(address));
    change_value &= mask_buffer;
    change_value = (change_value >> shift);
    return change_value;
}

pmu_vcore_voltage_t pmu_get_vcore_voltage(void)
{
    uint32_t vol;
    vol = pmu_get_register_value(PMU_RG_VCORE_VOSEL_NM_ADDR  , PMU_RG_VCORE_VOSEL_NM_MASK    , PMU_RG_VCORE_VOSEL_NM_SHIFT);
    vol = (vol - 1) / 5;
    return ((pmu_vcore_voltage_t)vol);
}

pmu_vcore_voltage_t pmu_set_vcore_voltage(pmu_vcore_lock_ctrl_t lock, pmu_vcore_voltage_t vcore_vosel)
{
    static unsigned char Vcore_Resource_Ctrl[7], origin_voltage, old_vcore, init = 0;
    int vol_index;

    if (init == 0) {
        origin_voltage = pmu_get_vcore_voltage();
        old_vcore = origin_voltage;
        init = 1;
    }

    /* parameter check */
    if ((vcore_vosel > PMU_VCORE_VOL_1P3V) || (vcore_vosel < PMU_VCORE_VOL_0P9V)) {
        return (PMU_VCORE_VOL_ERROR);
    }

    if (lock == PMU_VCORE_LOCK) {
        Vcore_Resource_Ctrl[vcore_vosel]++;
    } else {
        if (Vcore_Resource_Ctrl[vcore_vosel] != 0) {
            Vcore_Resource_Ctrl[vcore_vosel]--;
        }
    }
    //Find Highest Vcore Voltage
    for (vol_index = 6; vol_index >= 0; vol_index--) {
        if (Vcore_Resource_Ctrl[vol_index] != 0) {
            break;
        }
    }
    if (vol_index < 0) {
        //Cna't find any Vcore Ctrl request
        vol_index = origin_voltage;
    }

    if (vol_index != old_vcore) {
        old_vcore = vol_index;

        /*set sfio_all_tdsel to 4'b1111 to increase spiflash io driving when Vcore voltage <= 0.9V,*/
        if (vol_index <= PMU_VCORE_VOL_0P9V) {
            TOP_MISC_CFG->SFIO_CFG0 = TOP_MISC_CFG->SFIO_CFG0 | 0x0000f000;
        } else {
            TOP_MISC_CFG->SFIO_CFG0 = TOP_MISC_CFG->SFIO_CFG0 & (~0x0000f000);
        }

        pmu_set_register_value(PMU_RG_VCORE_VOSEL_NM_ADDR  , PMU_RG_VCORE_VOSEL_NM_MASK    , PMU_RG_VCORE_VOSEL_NM_SHIFT    , (1 + vol_index * 5));
        hal_gpt_delay_us(200);
        while (pmu_get_register_value(PMU_RG_VCORE_DVS_ACK_ADDR  , PMU_RG_VCORE_DVS_ACK_MASK    , PMU_RG_VCORE_DVS_ACK_SHIFT) != 1);
    }
    return ((pmu_vcore_voltage_t)vol_index);
}

void pmu_ctrl_power(pmu_vr_t pmu_vr, pmu_power_ctrl_t enable)
{
    switch (pmu_vr) {
        case PMU_VRF:
            pmu_set_register_value(PMU_RG_VRF_EN_ADDR       , PMU_RG_VRF_EN_MASK         , PMU_RG_VRF_EN_SHIFT        , enable);
            break;

        case PMU_VSRAM:
            pmu_set_register_value(PMU_RG_VSRAM_EN_ADDR     , PMU_RG_VSRAM_EN_MASK       , PMU_RG_VSRAM_EN_SHIFT      , enable);
            pmu_set_ao_latch();
            break;

        case PMU_VPA:
            pmu_set_register_value(PMU_RG_BUCK_VPA_EN_ADDR  , PMU_RG_BUCK_VPA_EN_MASK    , PMU_RG_BUCK_VPA_EN_SHIFT   , enable);
            break;

        case PMU_VSIM:
            pmu_set_register_value(PMU_RG_VSIM_EN_ADDR      , PMU_RG_VSIM_EN_MASK        , PMU_RG_VSIM_EN_SHIFT       , enable);
            break;

        case PMU_VFEM:
            pmu_set_register_value(PMU_RG_VFEM_EN_ADDR      , PMU_RG_VFEM_EN_MASK        , PMU_RG_VFEM_EN_SHIFT       , enable);
            break;

        default :
            log_debug("INTERNAL PMU ERROR : No matching Power Control!\n");
            break;
    }
}

void pmu_set_vrf_voltage(pmu_vrf_voltage_t vosel)
{
    pmu_set_register_value(PMU_RG_VRF_VOSEL_ADDR  , PMU_RG_VRF_VOSEL_MASK    , PMU_RG_VRF_VOSEL_SHIFT    , vosel);
}

pmu_vrf_voltage_t pmu_get_vrf_voltage(void)
{
    pmu_vrf_voltage_t vol;
    vol = (pmu_vrf_voltage_t)pmu_get_register_value(PMU_RG_VRF_VOSEL_ADDR  , PMU_RG_VRF_VOSEL_MASK    , PMU_RG_VRF_VOSEL_SHIFT);
    return (vol);
}

void pmu_set_vsim_voltage(pmu_vsim_voltage_t vosel)
{
    pmu_set_register_value(PMU_RG_VSIM_VOSEL_ADDR  , PMU_RG_VSIM_VOSEL_MASK    , PMU_RG_VSIM_VOSEL_SHIFT    , vosel);
}

pmu_vsim_voltage_t pmu_get_vsim_voltage(void)
{
    pmu_vsim_voltage_t vol;
    vol = (pmu_vsim_voltage_t)pmu_get_register_value(PMU_RG_VSIM_VOSEL_ADDR  , PMU_RG_VSIM_VOSEL_MASK    , PMU_RG_VSIM_VOSEL_SHIFT);
    return (vol);
}

void pmu_set_vpa_voltage(uint32_t vosel)
{
    /* VOUT=0.5V+0.05*vosel */
    pmu_set_register_value(PMU_RG_BUCK_VPA_VOSEL_ADDR  , PMU_RG_BUCK_VPA_VOSEL_MASK    , PMU_RG_BUCK_VPA_VOSEL_SHIFT    , vosel);
}

uint32_t pmu_get_vpa_voltage(void)
{
    /* VOUT=0.5V+0.05*vosel */
    uint32_t vol;
    vol = (uint32_t)pmu_get_register_value(PMU_RG_BUCK_VPA_VOSEL_ADDR  , PMU_RG_BUCK_VPA_VOSEL_MASK    , PMU_RG_BUCK_VPA_VOSEL_SHIFT);
    return (vol);
}

void pmu_enter_ship_mode(void)
{
    pmu_set_register_value(PMU_RG_EN_SHIP_ADDR  , PMU_RG_EN_SHIP_MASK   , PMU_RG_EN_SHIP_SHIFT  , 1);
}

void pmu_efuse_sw_load(void)
{
    uint32_t value;
    uint32_t pmu_efuse_valid = *((volatile uint32_t *)PMU_EFUSE_BASE_ADDR) & 0x1FFFFF; /* Efuse Valid bit 0 : 20 */

    /* pmu efuse PMU_EFUSE_BASE_ADDR+0x04 bit 0 : 29*/
    if ((pmu_efuse_valid >> 0) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x04, PMU_RG_VCORE_VOTRIM_NM_MASK, 0);
        pmu_set_register_value(PMU_RG_VCORE_VOTRIM_NM_ADDR, PMU_RG_VCORE_VOTRIM_NM_MASK, PMU_RG_VCORE_VOTRIM_NM_SHIFT, value);
    }
    if ((pmu_efuse_valid >> 1) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x04, PMU_RG_VCORE_VOTRIM_LPM_MASK, 4);
        pmu_set_register_value(PMU_RG_VCORE_VOTRIM_LPM_ADDR, PMU_RG_VCORE_VOTRIM_LPM_MASK, PMU_RG_VCORE_VOTRIM_LPM_SHIFT, value);
    }
    if ((pmu_efuse_valid >> 2) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x04, PMU_RG_BUCK_VPA_VREFH_TRIM_MASK, 8);
        pmu_set_register_value(PMU_RG_BUCK_VPA_VREFH_TRIM_ADDR, PMU_RG_BUCK_VPA_VREFH_TRIM_MASK, PMU_RG_BUCK_VPA_VREFH_TRIM_SHIFT, value);
    }
    if ((pmu_efuse_valid >> 3) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x04, PMU_RG_BUCK_VPA_VREFL_TRIM_MASK, 13);
        if(chip_is_mt2625E3() == true) {
            //E3 Version
            pmu_set_register_value(PMU_RG_BUCK_VPA_ZX_OS_REF_TRIM_ADDR, PMU_RG_BUCK_VPA_ZX_OS_REF_TRIM_MASK, PMU_RG_BUCK_VPA_ZX_OS_REF_TRIM_SHIFT, value);            
        }else {
            //E1,E2 Version
            pmu_set_register_value(PMU_RG_BUCK_VPA_VREFL_TRIM_ADDR, PMU_RG_BUCK_VPA_VREFL_TRIM_MASK, PMU_RG_BUCK_VPA_VREFL_TRIM_SHIFT, value);
        }
    }
    if ((pmu_efuse_valid >> 4) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x04, PMU_RG_VPA_ZXREF_MASK, 18);
        pmu_set_register_value(PMU_RG_VPA_ZXREF_ADDR, PMU_RG_VPA_ZXREF_MASK, PMU_RG_VPA_ZXREF_SHIFT, value);
    }
    if ((pmu_efuse_valid >> 5) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x04, PMU_RG_VPA_NLIM_SEL_MASK, 26);
        pmu_set_register_value(PMU_RG_VPA_NLIM_SEL_ADDR, PMU_RG_VPA_NLIM_SEL_MASK, PMU_RG_VPA_NLIM_SEL_SHIFT, value);
    }
    /* pmu efuse PMU_EFUSE_BASE_ADDR+0x08 bit 0 : 31*/
    if ((pmu_efuse_valid >> 6) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x08, PMU_RG_VRF_VOTRIM_MASK , 0);
        pmu_set_register_value(PMU_RG_VRF_VOTRIM_ADDR, PMU_RG_VRF_VOTRIM_MASK, PMU_RG_VRF_VOTRIM_SHIFT, value);
    }
    if ((pmu_efuse_valid >> 7) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x08, PMU_RG_VIO18_VOTRIM_NM_MASK , 4);
        pmu_set_register_value(PMU_RG_VIO18_VOTRIM_NM_ADDR, PMU_RG_VIO18_VOTRIM_NM_MASK, PMU_RG_VIO18_VOTRIM_NM_SHIFT, value);
    }
    if ((pmu_efuse_valid >> 8) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x08, PMU_RG_VIO18_VOTRIM_LPM_MASK , 8);
        pmu_set_register_value(PMU_RG_VIO18_VOTRIM_LPM_ADDR, PMU_RG_VIO18_VOTRIM_LPM_MASK, PMU_RG_VIO18_VOTRIM_LPM_SHIFT, value);
    }
    if ((pmu_efuse_valid >> 9) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x08, PMU_RG_VSIM_VOTRIM_NM_MASK , 12);
        pmu_set_register_value(PMU_RG_VSIM_VOTRIM_NM_ADDR, PMU_RG_VSIM_VOTRIM_NM_MASK, PMU_RG_VSIM_VOTRIM_NM_SHIFT, value);
    }
    if ((pmu_efuse_valid >> 10) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x08, PMU_RG_VSIM_VOTRIM_LPM_MASK , 16);
        pmu_set_register_value(PMU_RG_VSIM_VOTRIM_LPM_ADDR, PMU_RG_VSIM_VOTRIM_LPM_MASK, PMU_RG_VSIM_VOTRIM_LPM_SHIFT, value);
    }
    if ((pmu_efuse_valid >> 11) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x08, PMU_RG_VSRAM_VOTRIM_NM_MASK , 20);
        pmu_set_register_value(PMU_RG_VSRAM_VOTRIM_NM_ADDR, PMU_RG_VSRAM_VOTRIM_NM_MASK, PMU_RG_VSRAM_VOTRIM_NM_SHIFT, value);
    }
    if ((pmu_efuse_valid >> 12) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x08, PMU_RG_VSRAM_VOTRIM_LPM_MASK , 24);
        pmu_set_register_value(PMU_RG_VSRAM_VOTRIM_LPM_ADDR, PMU_RG_VSRAM_VOTRIM_LPM_MASK, PMU_RG_VSRAM_VOTRIM_LPM_SHIFT, value);
    }
    if ((pmu_efuse_valid >> 13) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x08, PMU_RG_VFEM_VOTRIM_MASK , 28);
        pmu_set_register_value(PMU_RG_VFEM_VOTRIM_ADDR, PMU_RG_VFEM_VOTRIM_MASK, PMU_RG_VFEM_VOTRIM_SHIFT, value);
    }
    /* pmu efuse PMU_EFUSE_BASE_ADDR+0x0C bit 0 : 31*/
    if ((pmu_efuse_valid >> 14) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x0C, PMU_RG_VREF_TRIM_HPBG_MASK , 0);
        pmu_set_register_value(PMU_RG_VREF_TRIM_HPBG_ADDR, PMU_RG_VREF_TRIM_HPBG_MASK, PMU_RG_VREF_TRIM_HPBG_SHIFT, value);
    }
    if ((pmu_efuse_valid >> 15) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x0C, PMU_RG_BGR_TCTRIM_HPBG_MASK , 4);
        pmu_set_register_value(PMU_RG_BGR_TCTRIM_HPBG_ADDR, PMU_RG_BGR_TCTRIM_HPBG_MASK, PMU_RG_BGR_TCTRIM_HPBG_SHIFT, value);
    }
    if ((pmu_efuse_valid >> 16) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x0C, PMU_RG_IBIAS_TRIM_HPBG_MASK , 8);
        pmu_set_register_value(PMU_RG_IBIAS_TRIM_HPBG_ADDR, PMU_RG_IBIAS_TRIM_HPBG_MASK, PMU_RG_IBIAS_TRIM_HPBG_SHIFT, value);
    }
    if ((pmu_efuse_valid >> 17) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x0C, PMU_RG_LPBG_VREFTRIM_MASK , 12);
        pmu_set_register_value(PMU_RG_LPBG_VREFTRIM_ADDR, PMU_RG_LPBG_VREFTRIM_MASK, PMU_RG_LPBG_VREFTRIM_SHIFT, value);
    }
    if ((pmu_efuse_valid >> 18) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x0C, PMU_RG_LPBG_TCTRIM_MASK , 17);
        pmu_set_register_value(PMU_RG_LPBG_TCTRIM_ADDR, PMU_RG_LPBG_TCTRIM_MASK, PMU_RG_LPBG_TCTRIM_SHIFT, value);
    }
    if ((pmu_efuse_valid >> 19) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x0C, PMU_RG_LPBG_IBTRIM_MASK , 22);
        pmu_set_register_value(PMU_RG_LPBG_IBTRIM_ADDR, PMU_RG_LPBG_IBTRIM_MASK, PMU_RG_LPBG_IBTRIM_SHIFT, value);
    }
    if ((pmu_efuse_valid >> 20) & 0x01) {
        value = pmu_get_register_value(PMU_EFUSE_BASE_ADDR + 0x0C, PMU_RG_CLK_TRIM_MASK , 27);
        pmu_set_register_value(PMU_RG_CLK_TRIM_ADDR, PMU_RG_CLK_TRIM_MASK, PMU_RG_CLK_TRIM_SHIFT, value);
    }
}

void dump_power_on_event(void)
{
#ifdef  PMU_DEBUG_ENABLE
    int power_on_event;
    power_on_event = pmu_get_register_value(PMU_RGS_DIG_STRUP_PWR_CON_ADDR  , PMU_RGS_DIG_STRUP_PWR_CON_MASK    , PMU_RGS_DIG_STRUP_PWR_CON_SHIFT);
    log_debug("[pmu power-on event:0x0%x]\r\n", power_on_event);

    switch (power_on_event & 0x1F) {
        case 1:
            log_debug("power-on by pwrkey\r\n");
            break;

        case 2:
            log_debug("power-on by pwrbb\r\n");
            break;

        case 4:
            log_debug("power-on by coldreset\r\n");
            break;

        case 8:
            log_debug("power-on by rtcwakeup\r\n");
            break;

        case 16:
            log_debug("power-on by ot\r\n");
            break;
    }

    switch ((power_on_event >> 8) & 0xF) {
        case 1:
            log_debug("power-on from ANA_PWROFF_STAT\r\n");
            break;

        case 2:
            log_debug("power-on from ANA_DSLP_STAT\r\n");
            break;

        case 4:
            log_debug("power-on from ANA_SHIP_STAT\r\n");
            break;

        case 8:
            log_debug("LSLP STAT from VCORE macro\r\n");
            break;
    }
#endif
}

void pmu_set_ao_latch(void)
{
    /* AO-Latch Registers */
    hal_gpt_delay_us(200);
    pmu_set_register_value(PMU_RG_AO_LATCH_SET_ADDR     , PMU_RG_AO_LATCH_SET_MASK      , PMU_RG_AO_LATCH_SET_SHIFT     , 1);
    hal_gpt_delay_us(200);
    pmu_set_register_value(PMU_RG_AO_LATCH_SET_ADDR     , PMU_RG_AO_LATCH_SET_MASK      , PMU_RG_AO_LATCH_SET_SHIFT     , 0);
}

void pmu_ctrl_longpress_shutdown(pmu_power_ctrl_t enable)
{
    if (enable == PMU_CTL_ENABLE) {
        if (pmu_get_register_value(PMU_RG_EN_LONGPRESS_ADDR     , PMU_RG_EN_LONGPRESS_MASK      , PMU_RG_EN_LONGPRESS_SHIFT) == 0) {
            pmu_set_register_value(PMU_RG_DIG_STRUP_CON0_ADDR   , 1   , 4,  1);
            hal_gpt_delay_us(200);
            pmu_set_register_value(PMU_RG_EN_LONGPRESS_ADDR     , PMU_RG_EN_LONGPRESS_MASK      , PMU_RG_EN_LONGPRESS_SHIFT     , 1);
            pmu_set_register_value(PMU_RG_DIG_STRUP_CON0_ADDR   , 1   , 4,  0);
        }
    } else {
        pmu_set_register_value(PMU_RG_EN_LONGPRESS_ADDR     , PMU_RG_EN_LONGPRESS_MASK      , PMU_RG_EN_LONGPRESS_SHIFT     , 0);
    }
}

/* Function to be used during abort/exception handling to kill all RF activities */
void pmu_shutdown_rfsys_supplies(void)
{
  /* Make VRF supply to be controlled by software */
  pmu_set_register_value(PMU_RG_VRF_EN_SW_SEL_ADDR , PMU_RG_VRF_EN_SW_SEL_MASK, PMU_RG_VRF_EN_SW_SEL_SHIFT,1);

  /* Shutdown all RF power supplies. */
  pmu_ctrl_power(PMU_VRF, PMU_CTL_DISABLE);
  pmu_ctrl_power(PMU_VPA, PMU_CTL_DISABLE);
  pmu_ctrl_power(PMU_VFEM, PMU_CTL_DISABLE);
}

#endif /* HAL_SLEEP_MANAGER_ENABLED */
