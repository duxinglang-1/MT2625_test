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

#ifndef __HAL_PMU_H__
#define __HAL_PMU_H__
#include <stdio.h>
#include <stdlib.h>
#include "hal_platform.h"
#include "hal_pmu_platform_mt2625.h"

#ifdef HAL_SLEEP_MANAGER_ENABLED

//#define PMU_DEBUG_ENABLE

#define LONG_PRESS_SHUTDOWN_ENABLE

#define LONG_PRESS_SHUTDOWN_SEL_5_SEC   0
#define LONG_PRESS_SHUTDOWN_SEL_8_SEC   1
#define LONG_PRESS_SHUTDOWN_SEL_11_SEC  2
#define LONG_PRESS_SHUTDOWN_SEL_25_SEC  3

#define LONG_PRESS_SHUTDOWN_TIME_SEL LONG_PRESS_SHUTDOWN_SEL_8_SEC

/* #define LONG_PRESS_SHUTDOWN_EDGE_TRIGGER */

#define PMU_EFUSE_BASE_ADDR ((uint32_t)(0xA20A0300))

typedef enum {
    PMU_STATUS_ERROR_EINT_NUMBER  = -3,     /**< pmu error number */
    PMU_STATUS_INVALID_PARAMETER  = -2,     /**< pmu error invalid parameter */
    PMU_STATUS_ERROR              = -1,     /**< pmu undefined error */
    PMU_STATUS_SUCCESS            = 0       /**< pmu function ok */
} pmu_status_t;

typedef enum {
    PMU_VCORE_VOL_0P7V = 0,
    PMU_VCORE_VOL_0P8V = 1,
    PMU_VCORE_VOL_0P9V = 2,
    PMU_VCORE_VOL_1P0V = 3,
    PMU_VCORE_VOL_1P1V = 4,
    PMU_VCORE_VOL_1P2V = 5,
    PMU_VCORE_VOL_1P3V = 6,
    PMU_VCORE_VOL_ERROR = 255
} pmu_vcore_voltage_t;

typedef enum {
    PMU_VCORE_UNLOCK = 0,
    PMU_VCORE_LOCK = 1
} pmu_vcore_lock_ctrl_t;

typedef enum {
    PMU_VCORE,
    PMU_VRF,
    PMU_VSRAM,
    PMU_VPA,
    PMU_VSIM,
    PMU_VFEM
} pmu_vr_t;

typedef enum {
    PMU_CTL_DISABLE   = 0,
    PMU_CTL_ENABLE    = 1
} pmu_power_ctrl_t;

typedef enum {
    PMU_VRF_VOL_1P00V = 0,
    PMU_VRF_VOL_1P02V = 1,
    PMU_VRF_VOL_1P04V = 2,
    PMU_VRF_VOL_1P06V = 3,
    PMU_VRF_VOL_1P08V = 4,
    PMU_VRF_VOL_1P10V = 5,
    PMU_VRF_VOL_1P12V = 6,
    PMU_VRF_VOL_1P14V = 7,
    PMU_VRF_VOL_1P16V = 8,
    PMU_VRF_VOL_1P18V = 9,
    PMU_VRF_VOL_1P20V = 10,
    PMU_VRF_VOL_1P22V = 11,
    PMU_VRF_VOL_1P24V = 12,
    PMU_VRF_VOL_1P26V = 13,
    PMU_VRF_VOL_1P28V = 14,
    PMU_VRF_VOL_1P30V = 15,
    PMU_VRF_VOL_ERROR = 255
} pmu_vrf_voltage_t;

typedef enum {
    PMU_VSIM_VOL_1P2V = 0,
    PMU_VSIM_VOL_1P8V = 1,
    PMU_VSIM_VOL_2P8V = 2,
    PMU_VSIM_VOL_3P0V = 3,
    PMU_VSIM_VOL_ERROR = 255
} pmu_vsim_voltage_t;

void pmu_init(void);
void pmu_set_register_value(uint32_t address, uint32_t mask, uint32_t shift, uint32_t value);
uint32_t pmu_get_register_value(uint32_t address, uint32_t mask, uint32_t shift);
pmu_vcore_voltage_t pmu_get_vcore_voltage(void);
pmu_vcore_voltage_t pmu_set_vcore_voltage(pmu_vcore_lock_ctrl_t lock, pmu_vcore_voltage_t vosel);
void pmu_ctrl_power(pmu_vr_t pmu_vr, pmu_power_ctrl_t enable);
void pmu_set_vrf_voltage(pmu_vrf_voltage_t vosel);
pmu_vrf_voltage_t pmu_get_vrf_voltage(void);
void pmu_set_vsim_voltage(pmu_vsim_voltage_t vosel);
pmu_vsim_voltage_t pmu_get_vsim_voltage(void);
void pmu_set_vpa_voltage(uint32_t vosel);
uint32_t pmu_get_vpa_voltage(void);
void pmu_enter_ship_mode(void);
void pmu_efuse_sw_load(void);
void dump_power_on_event(void);
void pmu_set_ao_latch(void);
void pmu_ctrl_longpress_shutdown(pmu_power_ctrl_t enable);
void pmu_shutdown_rfsys_supplies(void);

#endif /* HAL_SLEEP_MANAGER_ENABLED */
#endif /* __HAL_PMU_H__ */
