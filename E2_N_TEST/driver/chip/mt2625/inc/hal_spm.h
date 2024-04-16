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

#ifndef __HAL_SPM_H__
#define __HAL_SPM_H__
#include "mt2625.h"
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_internal.h"
#include "hal_sleep_manager_platform.h"

#ifdef HAL_SLEEP_MANAGER_ENABLED
//#define SPM_BASE             0xA2110000 /*System Power Manager */
#define SPM_WAKEUP_SOURCE_MASK              ((volatile uint32_t*)(SPM_BASE + 0x0000))
#define SPM_POWER_ON_VAL0                   ((volatile uint32_t*)(SPM_BASE + 0x0010))
#define SPM_POWER_ON_VAL1                   ((volatile uint32_t*)(SPM_BASE + 0x0014))
#define SPM_SEQUENCER_32K_REG_0             ((volatile uint32_t*)(SPM_BASE + 0x0100))
#define SPM_SEQUENCER_32K_REG_1             ((volatile uint32_t*)(SPM_BASE + 0x0104))
#define SPM_SEQUENCER_32K_REG_2             ((volatile uint32_t*)(SPM_BASE + 0x0108))
#define SPM_SEQUENCER_32K_REG_3             ((volatile uint32_t*)(SPM_BASE + 0x010C))
#define SPM_SEQUENCER_26M_REG_0             ((volatile uint32_t*)(SPM_BASE + 0x0140))
#define SPM_SEQUENCER_26M_REG_1             ((volatile uint32_t*)(SPM_BASE + 0x0144))
#define SPM_CM4_PWR_CON                     ((volatile uint32_t*)(SPM_BASE + 0x0200))
#define SPM_INFRA_PWR_CON                   ((volatile uint32_t*)(SPM_BASE + 0x0204))
#define SPM_MD_PWR_CON                      ((volatile uint32_t*)(SPM_BASE + 0x0208))
#define SPM_SDIO_SLV_PWR_CON                ((volatile uint32_t*)(SPM_BASE + 0x020C))
#define SPM_CM4_TCM_CONTROL_0               ((volatile uint32_t*)(SPM_BASE + 0x0220))
#define SPM_CM4_TCM_CONTROL_1               ((volatile uint32_t*)(SPM_BASE + 0x0224))
#define SPM_CM4_TCM_CONTROL_2               ((volatile uint32_t*)(SPM_BASE + 0x0228))
#define SPM_CM4_TCM_CONTROL_3               ((volatile uint32_t*)(SPM_BASE + 0x022C))
#define SPM_CM4_ROM_CONTROL                 ((volatile uint32_t*)(SPM_BASE + 0x0230))
#define SPM_CM4_TAGRAM_CONTROL              ((volatile uint32_t*)(SPM_BASE + 0x0234))
#define SPM_INFRA_SYSRAM_CONTROL_0          ((volatile uint32_t*)(SPM_BASE + 0x0250))
#define SPM_INFRA_SYSRAM_CONTROL_1          ((volatile uint32_t*)(SPM_BASE + 0x0254))
#define SPM_INFRA_SYSRAM_CONTROL_2          ((volatile uint32_t*)(SPM_BASE + 0x0258))
#define SPM_INFRA_SYSRAM_CONTROL_3          ((volatile uint32_t*)(SPM_BASE + 0x025C))
#define SPM_INFRA_SENSOR_IM_control         ((volatile uint32_t*)(SPM_BASE + 0x0260))
#define SPM_SDIO_SLV_SRAM_PD                ((volatile uint32_t*)(SPM_BASE + 0x0270))
#define SPM_SLEEP_CM4_PWR_MASK_B            ((volatile uint32_t*)(SPM_BASE + 0x280))
#define SPM_SLEEP_INFRA_PWR_MASK_B          ((volatile uint32_t*)(SPM_BASE + 0x284))
#define SPM_SLEEP_MD_PWR_MASK_B             ((volatile uint32_t*)(SPM_BASE + 0x288))
#define SPM_SLEEP_SDIO_SLV_PWR_MASK_B       ((volatile uint32_t*)(SPM_BASE + 0x28C))
#define SPM_GENERAL_SRAM_CONTROL            ((volatile uint32_t*)(SPM_BASE + 0x2A0))
#define SPM_SFC_SRAM_CONTROL                ((volatile uint32_t*)(SPM_BASE + 0x2B0))
#define SPM_USB_SRAM_CONTROL                ((volatile uint32_t*)(SPM_BASE + 0x2B4))
#define SPM_MD_SRCCLKENA_MASK               ((volatile uint32_t*)(SPM_BASE + 0x2C0))
#define SPM_MD_SRCCLKENA_SW_MUX             ((volatile uint32_t*)(SPM_BASE + 0x2C4))
#define SPM_PCM_CON0                        ((volatile uint32_t*)(SPM_BASE + 0x0300))
#define SPM_PCM_CON1                        ((volatile uint32_t*)(SPM_BASE + 0x0304))
#define SPM_PCM_IM_PTR                      ((volatile uint32_t*)(SPM_BASE + 0x0320))
#define SPM_PCM_IM_LEN                      ((volatile uint32_t*)(SPM_BASE + 0x0324))
#define SPM_PCM_IM_HOST_RW_PTR              ((volatile uint32_t*)(SPM_BASE + 0x0330))
#define SPM_PCM_IM_HOST_RW_DAT              ((volatile uint32_t*)(SPM_BASE + 0x0334))
#define SPM_PCM_REG_DATA_INI                ((volatile uint32_t*)(SPM_BASE + 0x0338))
#define SPM_PCM_EVENT_VECTOR0               ((volatile uint32_t*)(SPM_BASE + 0x0340))
#define SPM_PCM_EVENT_VECTOR1               ((volatile uint32_t*)(SPM_BASE + 0x0344))
#define SPM_PCM_EVENT_VECTOR2               ((volatile uint32_t*)(SPM_BASE + 0x0348))
#define SPM_PCM_EVENT_VECTOR3               ((volatile uint32_t*)(SPM_BASE + 0x034C))
#define SPM_PCM_EVENT_VECTOR4               ((volatile uint32_t*)(SPM_BASE + 0x0350))
#define SPM_PCM_EVENT_VECTOR5               ((volatile uint32_t*)(SPM_BASE + 0x0354))
#define SPM_PCM_EVENT_VECTOR6               ((volatile uint32_t*)(SPM_BASE + 0x0358))
#define SPM_PCM_EVENT_VECTOR7               ((volatile uint32_t*)(SPM_BASE + 0x035C))
#define SPM_PCM_PWR_IO_EN                   ((volatile uint32_t*)(SPM_BASE + 0x0360))
#define SPM_PCM_TIMER_VAL                   ((volatile uint32_t*)(SPM_BASE + 0x0364))
#define SPM_PCM_REGC_WAKEUP_MASK            ((volatile uint32_t*)(SPM_BASE + 0x0368))
#define SPM_SLEEP_WAKEUP_EVENT_MASK         ((volatile uint32_t*)(SPM_BASE + 0x0380))
#define SPM_SW_DEEP_SLEEP                   ((volatile uint32_t*)(SPM_BASE + 0x0384))
#define SPM_SLEEP_ISR_MASK                  ((volatile uint32_t*)(SPM_BASE + 0x0390))
#define SPM_CSYSPWRREQ_MASK                 ((volatile uint32_t*)(SPM_BASE + 0x0394))
#define SPM_SIDEBAND_CONTROL_0              ((volatile uint32_t*)(SPM_BASE + 0x03A0))
#define SPM_SIDEBAND_CONTROL_1              ((volatile uint32_t*)(SPM_BASE + 0x03A4))
#define SPM_SIDEBAND_CONTROL_2              ((volatile uint32_t*)(SPM_BASE + 0x03A8))
#define SPM_RESOURCE_CONTROL_0              ((volatile uint32_t*)(SPM_BASE + 0x03AC))
#define SPM_RESOURCE_CONTROL_1              ((volatile uint32_t*)(SPM_BASE + 0x03B0))
#define SPM_EVENT_CONTROL_0                 ((volatile uint32_t*)(SPM_BASE + 0x03B4))
#define SPM_EVENT_CONTROL_1                 ((volatile uint32_t*)(SPM_BASE + 0x03B8))
#define SPM_CLK_SETTLE                      ((volatile uint32_t*)(SPM_BASE + 0x03C0))
#define SPM_SPM_DEBUG_SELECT                ((volatile uint32_t*)(SPM_BASE + 0x03D0))
#define SPM_PCM_DEBUG_CON                   ((volatile uint32_t*)(SPM_BASE + 0x03D4))
#define SPM_MONITOR_WAKEUP_EVENT_EN         ((volatile uint32_t*)(SPM_BASE + 0x03D8))
#define SPM_SPM_ERROR_FLAG                  ((volatile uint32_t*)(SPM_BASE + 0x03E0))
#define SPM_EDGE_WAKEUP_SOURCE              ((volatile uint32_t*)(SPM_BASE + 0x03F0))
#define SPM_BBPLL_CONTROL_0                 ((volatile uint32_t*)(SPM_BASE + 0x0700))
#define SPM_BBPLL_CONTROL_1                 ((volatile uint32_t*)(SPM_BASE + 0x0704))
#define SPM_BBPLL_CONTROL_2                 ((volatile uint32_t*)(SPM_BASE + 0x0708))
#define SPM_MD_VRF_REQ_CONTROL              ((volatile uint32_t*)(SPM_BASE + 0x0740))
#define SPM_VRF_DELAY_CONTROL               ((volatile uint32_t*)(SPM_BASE + 0x0750))
#define SPM_VRF_DELAY_VAL                   ((volatile uint32_t*)(SPM_BASE + 0x0754))
#define SPM_WAKEUP_SOURCE_STA               ((volatile uint32_t*)(SPM_BASE + 0x0800))
#define SPM_WAKEUP_SOURCE_STA_32K           ((volatile uint32_t*)(SPM_BASE + 0x0804))
#define SPM_PWR_STATUS                      ((volatile uint32_t*)(SPM_BASE + 0x0808))
#define SPM_PWR_STATUS_2ND                  ((volatile uint32_t*)(SPM_BASE + 0x080C))
#define SPM_STATE_STATUS                    ((volatile uint32_t*)(SPM_BASE + 0x0810))
#define SPM_SRCLKENA_ACK_TO_MD              ((volatile uint32_t*)(SPM_BASE + 0x0814))
#define SPM_SLEEP_ISR_STATUS                ((volatile uint32_t*)(SPM_BASE + 0x0820))
#define SPM_SLEEP_ISR_RAW_STA               ((volatile uint32_t*)(SPM_BASE + 0x0830))
#define SPM_PCM_REG0_DATA                   ((volatile uint32_t*)(SPM_BASE + 0x0900))
#define SPM_PCM_REG1_DATA                   ((volatile uint32_t*)(SPM_BASE + 0x0904))
#define SPM_PCM_REG2_DATA                   ((volatile uint32_t*)(SPM_BASE + 0x0908))
#define SPM_PCM_REG3_DATA                   ((volatile uint32_t*)(SPM_BASE + 0x090C))
#define SPM_PCM_REG4_DATA                   ((volatile uint32_t*)(SPM_BASE + 0x0910))
#define SPM_PCM_REG5_DATA                   ((volatile uint32_t*)(SPM_BASE + 0x0914))
#define SPM_PCM_REG6_DATA                   ((volatile uint32_t*)(SPM_BASE + 0x0918))
#define SPM_PCM_REG7_DATA                   ((volatile uint32_t*)(SPM_BASE + 0x091C))
#define SPM_PCM_REG8_DATA                   ((volatile uint32_t*)(SPM_BASE + 0x0920))
#define SPM_PCM_REG9_DATA                   ((volatile uint32_t*)(SPM_BASE + 0x0924))
#define SPM_PCM_REG10_DATA                  ((volatile uint32_t*)(SPM_BASE + 0x0928))
#define SPM_PCM_REG11_DATA                  ((volatile uint32_t*)(SPM_BASE + 0x092C))
#define SPM_PCM_REG12_DATA                  ((volatile uint32_t*)(SPM_BASE + 0x0930))
#define SPM_PCM_REG13_DATA                  ((volatile uint32_t*)(SPM_BASE + 0x0934))
#define SPM_PCM_REG14_DATA                  ((volatile uint32_t*)(SPM_BASE + 0x0938))
#define SPM_PCM_REG15_DATA                  ((volatile uint32_t*)(SPM_BASE + 0x093C))
#define SPM_PCM_TIMER_OUT                   ((volatile uint32_t*)(SPM_BASE + 0x0980))
#define SPM_PCM_EVENT_REG_STA               ((volatile uint32_t*)(SPM_BASE + 0x09E0))
#define SPM_PCM_FSM_STA                     ((volatile uint32_t*)(SPM_BASE + 0x09E4))
#define SPM_PCM_RESERVE                     ((volatile uint32_t*)(SPM_BASE + 0x0B00))
#define SPM_PCM_RESERVE2                    ((volatile uint32_t*)(SPM_BASE + 0x0B04))
#define SPM_PCM_FLAGS                       ((volatile uint32_t*)(SPM_BASE + 0x0B10))
#define SPM_SPM_AP_SEMA                     ((volatile uint32_t*)(SPM_BASE + 0x0B40))
#define SPM_SPM_SPM_SEMA                    ((volatile uint32_t*)(SPM_BASE + 0x0B44))
#define SPM_PCM_SW_INT_SET                  ((volatile uint32_t*)(SPM_BASE + 0x0B90))
#define SPM_PCM_SW_INT_CLEAR                ((volatile uint32_t*)(SPM_BASE + 0x0B94))
#define SPM_SLEEP_TWAM_CON_0                ((volatile uint32_t*)(SPM_BASE + 0x0C80))
#define SPM_SLEEP_TWAM_CON_1                ((volatile uint32_t*)(SPM_BASE + 0x0C84))
#define SPM_SLEEP_TWAM_CON_2                ((volatile uint32_t*)(SPM_BASE + 0x0C88))
#define SPM_SLEEP_TWAM_LAST_STATUS0         ((volatile uint32_t*)(SPM_BASE + 0x0C90))
#define SPM_SLEEP_TWAM_LAST_STATUS1         ((volatile uint32_t*)(SPM_BASE + 0x0C94))
#define SPM_SLEEP_TWAM_LAST_STATUS2         ((volatile uint32_t*)(SPM_BASE + 0x0C98))
#define SPM_SLEEP_TWAM_LAST_STATUS3         ((volatile uint32_t*)(SPM_BASE + 0x0C9C))
#define SPM_SLEEP_TWAM_CURR_STATUS0         ((volatile uint32_t*)(SPM_BASE + 0x0CA0))
#define SPM_SLEEP_TWAM_CURR_STATUS1         ((volatile uint32_t*)(SPM_BASE + 0x0CA4))
#define SPM_SLEEP_TWAM_CURR_STATUS2         ((volatile uint32_t*)(SPM_BASE + 0x0CA8))
#define SPM_SLEEP_TWAM_CURR_STATUS3         ((volatile uint32_t*)(SPM_BASE + 0x0CAC))
#define SPM_SLEEP_TWAM_TIMER_OUT            ((volatile uint32_t*)(SPM_BASE + 0x0CB0))
#define SPM_SLEEP_TWAM_WINDOW_LEN           ((volatile uint32_t*)(SPM_BASE + 0x0CB4))

#define SPM_WAKEUP_SOURCE_NUM               (11)
#define SPM_WAKEUP_SOURCE_GPT               (0x7FE)     // wakeup_source 0
#define SPM_WAKEUP_SOURCE_EINT              (0x7FD)     // wakeup_source 1
#define SPM_WAKEUP_SOURCE_SDIO_SLV          (0x7FB)     // wakeup_source 2
#define SPM_WAKEUP_SOURCE_SPI_SLAVE_A       (0x7F7)     // wakeup_source 3
#define SPM_WAKEUP_SOURCE_WDT               (0x7EF)     // wakeup_source 4
#define SPM_WAKEUP_SOURCE_KP                (0x7DF)     // wakeup_source 5
#define SPM_WAKEUP_SOURCE_MD                (0x7BF)     // wakeup_source 6
#define SPM_WAKEUP_SOURCE_OST               (0x77F)     // wakeup_source 7
#define SPM_WAKEUP_SOURCE_MD_DEBUGSYS       (0x6FF)     // wakeup_source 8
#define SPM_WAKEUP_SOURCE_DEBUGSYS          (0x5FF)     // wakeup_source 9
#define SPM_WAKEUP_SOURCE_SENSOR_CTRLER     (0x3FF)     // wakeup_source 10


typedef enum {
    SPM_MTCMOS_MDSYS        = 0,
    SPM_MTCMOS_SDIO_SLV     = 1
} spm_mtcmos_type_t;

typedef enum {
    SPM_MTCMOS_PWR_DISABLE  = 0,
    SPM_MTCMOS_PWR_ENABLE   = 1
} spm_mtcmos_ctrl_t;

void spm_kick_start(void);
void spm_init(uint32_t spm_auto_load);
uint32_t spm_control_mtcmos(spm_mtcmos_type_t mtcmos, spm_mtcmos_ctrl_t ctrl);
void spm_mask_wakeup_source(uint32_t wakeup_source);
void spm_unmask_wakeup_source(uint32_t wakeup_source);

//Just for FPGA
void Case_2_RL_SPM_Internal(void);

#endif
#endif

