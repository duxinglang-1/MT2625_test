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

/* includes. */
#include "string.h"
#include "mt2625.h"
#include "hal_log.h"
#include "hal_platform.h"
#include "hal_gpt.h"
#include "hal_clock.h"

/* register definition. */
#define SENSORUP_POWER_ON_VAL0             ((volatile uint32_t *)(SENSOR_UP_BASE + 0x010))
#define SENSORUP_POWER_ON_VAL1             ((volatile uint32_t *)(SENSOR_UP_BASE + 0x014))
#define SENSORUP_CON0                      ((volatile uint32_t *)(SENSOR_UP_BASE + 0x300))
#define SENSORUP_CON1                      ((volatile uint32_t *)(SENSOR_UP_BASE + 0x304))
#define SENSORUP_IM_PTR                    ((volatile uint32_t *)(SENSOR_UP_BASE + 0x320))
#define SENSORUP_IM_LEN                    ((volatile uint32_t *)(SENSOR_UP_BASE + 0x324))
#define SENSORUP_IM_HOST_RW_PTR            ((volatile uint32_t *)(SENSOR_UP_BASE + 0x330))
#define SENSORUP_IM_HOST_RW_DAT            ((volatile uint32_t *)(SENSOR_UP_BASE + 0x334))
#define SENSORUP_REG_DATA_INI              ((volatile uint32_t *)(SENSOR_UP_BASE + 0x338))
#define SENSORUP_EVENT_VECTOR0             ((volatile uint32_t *)(SENSOR_UP_BASE + 0x340))
#define SENSORUP_EVENT_VECTOR1             ((volatile uint32_t *)(SENSOR_UP_BASE + 0x344))
#define SENSORUP_EVENT_VECTOR2             ((volatile uint32_t *)(SENSOR_UP_BASE + 0x348))
#define SENSORUP_EVENT_VECTOR3             ((volatile uint32_t *)(SENSOR_UP_BASE + 0x34C))
#define SENSORUP_EVENT_VECTOR4             ((volatile uint32_t *)(SENSOR_UP_BASE + 0x350))
#define SENSORUP_EVENT_VECTOR5             ((volatile uint32_t *)(SENSOR_UP_BASE + 0x354))
#define SENSORUP_EVENT_VECTOR6             ((volatile uint32_t *)(SENSOR_UP_BASE + 0x358))
#define SENSORUP_EVENT_VECTOR7             ((volatile uint32_t *)(SENSOR_UP_BASE + 0x35C))
#define SENSORUP_PWR_IO_EN                 ((volatile uint32_t *)(SENSOR_UP_BASE + 0x360))
#define SENSORUP_SLEEP_WAKEUP_EVENT_MASK   ((volatile uint32_t *)(SENSOR_UP_BASE + 0x380))
#define SENSORUP_CLK_SETTLE                ((volatile uint32_t *)(SENSOR_UP_BASE + 0x3C0))
#define SENSORUP_DEBUG_SELECT              ((volatile uint32_t *)(SENSOR_UP_BASE + 0x3D0))
#define SENSORUP_DEBUG_CON                 ((volatile uint32_t *)(SENSOR_UP_BASE + 0x3D4))
#define SENSORUP_FSM_STA                   ((volatile uint32_t *)(SENSOR_UP_BASE + 0x9E4))
#define SENSORUP_RESERVE                   ((volatile uint32_t *)(SENSOR_UP_BASE + 0xB00))
#define SENSORUP_RESERVE2                  ((volatile uint32_t *)(SENSOR_UP_BASE + 0xB04))

#define SENSOR_IRQ_FLAG                    ((volatile uint32_t *)(SENSOR_FIFO_BASE + 0x000))
#define SENSOR_IRQ_SET                     ((volatile uint32_t *)(SENSOR_FIFO_BASE + 0x004))
#define SENSOR_IRQ_CLR                     ((volatile uint32_t *)(SENSOR_FIFO_BASE + 0x008))
#define SENSOR_IRQ_MASK                    ((volatile uint32_t *)(SENSOR_FIFO_BASE + 0x00C))
#define SENSOR_IRQ_WAKEUP_AP_EN            ((volatile uint32_t *)(SENSOR_FIFO_BASE + 0x010))

#define SENSOR_AO_DEBUG_CON                ((volatile uint32_t *)(SENSOR_FIFO_BASE + 0x600))
#define SENSOR_AO_DEBUG_SELECT             ((volatile uint32_t *)(SENSOR_FIFO_BASE + 0x604))

#define SENSOR_TIMER_0_ENABLE              ((volatile uint32_t *)(SENSOR_TIMER_BASE + 0x000))
#define SENSOR_TIMER_0_EXPIRING_PERIOD     ((volatile uint16_t *)(SENSOR_TIMER_BASE + 0x004))
#define SENSOR_TIMER_0_WAKEUP_LATENCY      ((volatile uint8_t *)(SENSOR_TIMER_BASE + 0x020))
#define SENSOR_TIMER_0_TIMER_CNT           ((volatile uint16_t *)(SENSOR_TIMER_BASE + 0x030))
#define SENSOR_TIMER_0_WAKEUP_EN           ((volatile uint16_t *)(SENSOR_TIMER_BASE + 0x040))
#define SENSOR_TIMER_1_ENABLE              ((volatile uint32_t *)(SENSOR_TIMER_BASE + 0x100))
#define SENSOR_TIMER_1_EXPIRING_PERIOD     ((volatile uint16_t *)(SENSOR_TIMER_BASE + 0x104))
#define SENSOR_TIMER_1_WAKEUP_LATENCY      ((volatile uint8_t *)(SENSOR_TIMER_BASE + 0x120))
#define SENSOR_TIMER_1_TIMER_CNT           ((volatile uint16_t *)(SENSOR_TIMER_BASE + 0x130))
#define SENSOR_TIMER_1_WAKEUP_EN           ((volatile uint16_t *)(SENSOR_TIMER_BASE + 0x140))

#define INFRA_CFG_DBGMON__AO_DBGMON_SEL    ((volatile uint8_t *)0xA21F0014)
#define INFRA_CFG_DBGMON__PD_DBGMON_SEL    ((volatile uint8_t *)0xA21F0015)
#define TOP_DEBUG                          ((volatile uint32_t *)0xA2010060)

#define XO_PDN_CLRD0                       ((volatile uint32_t *)(0xA2030B20))

/* Event vector and instruction. */
const unsigned int sensor_ctrl_event_vector_parameter[8] = {0x30, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, };

static const unsigned int sensor_ctrl_im[525] = {
0x18c0001f, 0xa219002c, 0xe0e00001, 0x1910001f, 0xa2190028, 0xd8200064, 0x17c07c1f, 0xe0e00000,
0x18c0001f, 0xa219000c, 0xe0e00001, 0x1910001f, 0xa2190008, 0xd8200164, 0x17c07c1f, 0xe0e00000,
0x0a000008, 0x00000001, 0x18c0001f, 0xa0160b10, 0xe0c00008, 0x68e00008, 0x00000004, 0xc8c003a3,
0x17c07c1f, 0x1b00001f, 0x00070000, 0xf0000000, 0x17c07c1f, 0x18c0001f, 0xa2180004, 0xe0e00001,
0x1a00001f, 0x00000000, 0x19c0001f, 0xa0160810, 0xe1e00024, 0xf0000000, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f, 0x17c07c1f,
0x1840001f, 0x00000001, 0x19c0001f, 0xa0160810, 0xe1e00011, 0x1b00001f, 0x00070000, 0x1b80001f,
0x90000000, 0x19c0001f, 0xa0160810, 0xe1e00015, 0xf0000000, };

static uint32_t _sensor_ctrl_read_im(uint32_t index)
{
    uint32_t data;

    *SENSORUP_IM_HOST_RW_PTR = 0x01000000 | index;
    data = *SENSORUP_IM_HOST_RW_DAT;
    *SENSORUP_IM_HOST_RW_PTR = 0;
    return (data);
}

static void _sensor_ctrl_manual_write_im(uint32_t index, uint32_t data)
{
    *SENSORUP_IM_HOST_RW_PTR = 0x01010000 | index;
    *SENSORUP_IM_HOST_RW_DAT = data;
    return;
}

static void _sensor_ctrl_uP_reset(void)
{
    /* bit 4: Resets sensor ctrl. */
    *SENSORUP_CON0 = 0x010C0010;
    hal_gpt_delay_us(10);
    *SENSORUP_CON0 = 0x010C0000;
    hal_gpt_delay_us(10);
}

int32_t hal_sensor_ctrl_init(void)
{
    uint32_t i, status;
    uint32_t im_length = 0;
    int32_t ret = 0;

  #ifdef HAL_CLOCK_MODULE_ENABLED
    hal_clock_enable(HAL_CLOCK_CG_SENSOR_CTRL_TOP_AO);
    hal_clock_enable(HAL_CLOCK_CG_SENSOR_uP);
  #else
    log_hal_error("HAL_CLOCK_MODULE_ENABLED must be defined for CG contorl!\r\n");
  #endif

    _sensor_ctrl_uP_reset();

    *SENSORUP_CON1 = 0x010C3C49;

    /* Load event vector 0-7. */
    *SENSORUP_EVENT_VECTOR0 = sensor_ctrl_event_vector_parameter[0];
    *SENSORUP_EVENT_VECTOR1 = sensor_ctrl_event_vector_parameter[1];
    *SENSORUP_EVENT_VECTOR2 = sensor_ctrl_event_vector_parameter[2];
    *SENSORUP_EVENT_VECTOR3 = sensor_ctrl_event_vector_parameter[3];
    *SENSORUP_EVENT_VECTOR4 = sensor_ctrl_event_vector_parameter[4];
    *SENSORUP_EVENT_VECTOR5 = sensor_ctrl_event_vector_parameter[5];
    *SENSORUP_EVENT_VECTOR6 = sensor_ctrl_event_vector_parameter[6];
    *SENSORUP_EVENT_VECTOR7 = sensor_ctrl_event_vector_parameter[7];

    *SENSORUP_POWER_ON_VAL0 = 0;
    *SENSORUP_POWER_ON_VAL1 = 0x44;

    *SENSORUP_REG_DATA_INI = *SENSORUP_POWER_ON_VAL0 | 0x1;

    *SENSORUP_PWR_IO_EN = 0x00010000; // for R0
    *SENSORUP_PWR_IO_EN = 0x00000000;
    *SENSORUP_REG_DATA_INI = *SENSORUP_POWER_ON_VAL1;
    *SENSORUP_PWR_IO_EN = 0x00800000; // for R7
    *SENSORUP_PWR_IO_EN = 0x00000000;
    *SENSORUP_CLK_SETTLE = 0x3;

    /* Init IM Length and pointer */
    im_length = sizeof(sensor_ctrl_im)/sizeof(sensor_ctrl_im[0]);
    *SENSORUP_IM_LEN = im_length;
    *SENSORUP_IM_PTR = (volatile uint32_t)sensor_ctrl_im;

    /* manual load sensor ctrl instruction. */
    for (i = 0; i < im_length; i++) {
        _sensor_ctrl_manual_write_im(i, sensor_ctrl_im[i]);
    }

    *SENSORUP_PWR_IO_EN = 0x0081; // enable R0 & R7 output

    /* Kick IM process */
    *SENSORUP_CON0 = 0x010C0002;

    log_hal_warning("sensor ctrl manual load\r\n");

    /* Wait ready state */
    do {
        status = (*SENSORUP_FSM_STA >> 9) & 0x1;
    } while (status != 0x01);

    /* Read back spm code */
    uint32_t im_check_buf[im_length];
    for (i = 0; i < im_length; i++) {
        im_check_buf[i] = _sensor_ctrl_read_im(i);
    }

    /* Check sensor ctrl instruction. */
    if (memcmp(im_check_buf, sensor_ctrl_im, im_length * 4) == 0) {
        log_hal_warning("sensor ctrl IM loading Success\r\n");
    } else {
        log_hal_warning("sensor ctrl IM loading Fail\r\n");
        ret = -1;
    }

    return ret;
}

void hal_sensor_ctrl_start_timer(uint32_t id, uint16_t period)
{
    uint16_t cnt, load = 0;

    if (id == 0) {
        *SENSOR_TIMER_0_ENABLE = 0x1; // enable
        *SENSOR_TIMER_0_EXPIRING_PERIOD = period;
        *SENSOR_TIMER_0_ENABLE = 0x101; // load and enable

        do {
            cnt = *SENSOR_TIMER_0_TIMER_CNT;
            load = *SENSOR_TIMER_0_EXPIRING_PERIOD;
        } while (cnt != load);
        *SENSOR_TIMER_0_WAKEUP_EN = 0x1; // wake-up event separate from expiry flag
        *SENSOR_TIMER_0_ENABLE = 0x10101; // run, load and enable
    } else {
        *SENSOR_TIMER_1_ENABLE = 0x1; // enable
        *SENSOR_TIMER_1_EXPIRING_PERIOD = period;
        *SENSOR_TIMER_1_ENABLE = 0x101; // load and enable

        do {
            cnt = *SENSOR_TIMER_1_TIMER_CNT;
            load = *SENSOR_TIMER_1_EXPIRING_PERIOD;
        } while (cnt != load);
        *SENSOR_TIMER_1_WAKEUP_EN = 0x1; // wake-up event separate from expiry flag
        *SENSOR_TIMER_1_ENABLE = 0x10101; // run, load and enable
    }

    // wake-up event (bit16-23) and this event will wake up the sleep procedure.
    *SENSORUP_SLEEP_WAKEUP_EVENT_MASK &= ~0x3FF0000;
    // kick uP
    *SENSORUP_CON0 = 0x010C0103;
}

void hal_sensor_ctrl_debug_bus_setting(void)
{
    // select sensor_ao (fifo, timer) from infra_ao
    *INFRA_CFG_DBGMON__PD_DBGMON_SEL = 0x14;
    // select sensor uP from infra_pd
    *INFRA_CFG_DBGMON__AO_DBGMON_SEL = 0x3;

    // select infra_ao (0x15) infra_pd (0x16) from top.
    *TOP_DEBUG = 0x15;

    // select infra_ao (0x15) infra_pd (0x16) from top.
    //*TOP_DEBUG = 0x16;

    // mux in sensor uP
    *SENSORUP_DEBUG_CON = 0x1;
    *SENSORUP_DEBUG_SELECT = 0x30303030;

    // mux in sensor_ao
    *SENSOR_AO_DEBUG_CON = 0x1;
    *SENSOR_AO_DEBUG_SELECT = 0x0b0a0908;
}

void hal_sensor_ctrl_isr(hal_nvic_irq_t irq_number)
{
    uint32_t irq_flag = 0;

    irq_flag = *SENSOR_IRQ_FLAG;
    log_hal_warning("hal_sensor_ctrl_isr(%d): %X\r\n", irq_number, irq_flag);
    *SENSOR_IRQ_CLR = irq_flag;

    do {
        irq_flag = *SENSOR_IRQ_FLAG;
    } while (irq_flag != 0);
}

