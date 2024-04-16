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

#ifndef __HAL_I2S_INTERNAL_H__
#define __HAL_I2S_INTERNAL_H__

#include "hal_i2s.h"

#if defined(HAL_I2S_MODULE_ENABLED)

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "mt2625.h"
#include "hal_pdma_internal.h"

/*I2S state*/
#define     I2S_STATE_IDLE              0
#define     I2S_STATE_INIT              1
#define     I2S_STATE_RX_RUNNING        2
#define     I2S_STATE_TX_RUNNING        3

/*I2S EOF threshold*/
#define I2S_EOF_THRESHOLD               4//Send EOF event until less than 4 samples in tx vfifo

#define     DRV_Reg32(addr)             (*(volatile uint32_t*)(addr))
#define     DRV_Reg16(addr)             (*(volatile uint16_t *)(addr))
#define     DRV_Reg8(addr)              (*(volatile uint8_t*)(addr))
#define     I2S_CHECK_BIT(var,pos)       ((var) & (1<<(pos)))

/****************************************REGISTER***********************************************/
#define     I2S_0_TX_DMA_FIFO                                         (DMA_AHB_BASE+0x0600)//DMA22
#define     I2S_0_RX_DMA_FIFO                                         (DMA_AHB_BASE+0x0700)//DMA23
#define     I2S_1_TX_DMA_FIFO                                         (DMA_AHB_BASE+0x0800)//DMA24
#define     I2S_1_RX_DMA_FIFO                                         (DMA_AHB_BASE+0x0900)//DMA25

/*  I2S0  REG */
#define     I2S_0_I2S1_GLOBAL_CONTROL                                 (I2S_0_BASE+0x0000) // apb32
#define     I2S_0_I2S1_GLOBAL_CONTROL__F_CR_I2S1_GLOBAL_CONTROL       (I2S_0_BASE+0x0000) // apb32:32
#define     I2S_0_I2S1_DL_CONTROL                                     (I2S_0_BASE+0x0004) // apb32
#define     I2S_0_I2S1_DL_CONTROL__F_CR_I2S1_DL_CONTROL               (I2S_0_BASE+0x0004) // apb32:32
#define     I2S_0_I2S1_UL_CONTROL                                     (I2S_0_BASE+0x0008) // apb32
#define     I2S_0_I2S1_UL_CONTROL__F_CR_I2S1_UL_CONTROL               (I2S_0_BASE+0x0008) // apb32:32
#define     I2S_0_I2S1_SOFT_RESET                                     (I2S_0_BASE+0x000c) // apb32
#define     I2S_0_I2S1_SOFT_RESET__F_CR_I2S1_SOFT_RSTB                (I2S_0_BASE+0x000c) // apb8:1
#define     I2S_0_I2S1_DL_FIFO                                        (I2S_0_BASE+0x0010) // apb32
#define     I2S_0_I2S1_DL_FIFO__F_CR_I2S1_FIFO_DL_RCLEAR              (I2S_0_BASE+0x0010) // apb8:1
#define     I2S_0_I2S1_DL_FIFO__F_CR_I2S1_FIFO_DL_R_THRESHOLD         (I2S_0_BASE+0x0011) // apb8:4
#define     I2S_0_I2S1_DL_FIFO__F_CR_I2S1_FIFO_DL_WCLEAR              (I2S_0_BASE+0x0012) // apb8:1
#define     I2S_0_I2S1_DL_FIFO__F_CR_I2S1_FIFO_DL_W_THRESHOLD         (I2S_0_BASE+0x0013) // apb8:4
#define     I2S_0_I2S1_UL_FIFO                                        (I2S_0_BASE+0x0014) // apb32
#define     I2S_0_I2S1_UL_FIFO__F_CR_I2S1_FIFO_UL_RCLEAR              (I2S_0_BASE+0x0014) // apb8:1
#define     I2S_0_I2S1_UL_FIFO__F_CR_I2S1_FIFO_UL_R_THRESHOLD         (I2S_0_BASE+0x0015) // apb8:4
#define     I2S_0_I2S1_UL_FIFO__F_CR_I2S1_FIFO_UL_WCLEAR              (I2S_0_BASE+0x0016) // apb8:1
#define     I2S_0_I2S1_UL_FIFO__F_CR_I2S1_FIFO_UL_W_THRESHOLD         (I2S_0_BASE+0x0017) // apb8:4
#define     I2S_0_I2S1_DL_FIFO_STATUS                                 (I2S_0_BASE+0x0018) // apb32
#define     I2S_0_I2S1_DL_FIFO_STATUS__F_CR_I2S1_FIFO_DL_STATUS       (I2S_0_BASE+0x0018) // apb8:6
#define     I2S_0_I2S1_DL_FIFO_STATUS__F_CR_I2S1_FIFO_DL_RFIFO_CNT    (I2S_0_BASE+0x0019) // apb8:4
#define     I2S_0_I2S1_DL_FIFO_STATUS__F_CR_I2S1_FIFO_DL_WFIFO_CNT    (I2S_0_BASE+0x001a) // apb8:4
#define     I2S_0_I2S1_UL_FIFO_STATUS                                 (I2S_0_BASE+0x001c) // apb32
#define     I2S_0_I2S1_UL_FIFO_STATUS__F_CR_I2S1_FIFO_UL_STATUS       (I2S_0_BASE+0x001c) // apb8:6
#define     I2S_0_I2S1_UL_FIFO_STATUS__F_CR_I2S1_FIFO_UL_RFIFO_CNT    (I2S_0_BASE+0x001d) // apb8:4
#define     I2S_0_I2S1_UL_FIFO_STATUS__F_CR_I2S1_FIFO_UL_WFIFO_CNT    (I2S_0_BASE+0x001e) // apb8:4
#define     I2S_0_I2S1_SCAN_RSV                                       (I2S_0_BASE+0x0020) // apb32
#define     I2S_0_I2S1_SCAN_RSV__F_CR_I2S1_RESERVED                   (I2S_0_BASE+0x0020) // apb32:32
#define     I2S_0_I2S1_GLOBAL_EN_CONTROL                              (I2S_0_BASE+0x0030) // apb32
#define     I2S_0_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_ENABLE            (I2S_0_BASE+0x0030) // apb8:1
#define     I2S_0_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_DL_FIFO_EN        (I2S_0_BASE+0x0031) // apb8:1
#define     I2S_0_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_UL_FIFO_EN        (I2S_0_BASE+0x0032) // apb8:1
#define     I2S_0_I2S1_GLOBAL_EN_CONTROL__F_CR_PDN_AUD_26M            (I2S_0_BASE+0x0033) // apb8:1
#define     I2S_0_I2S1_DL_SR_EN_CONTROL                               (I2S_0_BASE+0x0034) // apb32
#define     I2S_0_I2S1_DL_SR_EN_CONTROL__F_CR_I2S1_OUT_EN             (I2S_0_BASE+0x0034) // apb8:1
#define     I2S_0_I2S1_DL_SR_EN_CONTROL__F_CR_I2S1_OUT_SR             (I2S_0_BASE+0x0035) // apb8:5
#define     I2S_0_I2S1_DL_SR_EN_CONTROL__F_CR_PDN_I2SO1               (I2S_0_BASE+0x0036) // apb8:1
#define     I2S_0_I2S1_UL_SR_EN_CONTROL                               (I2S_0_BASE+0x0038) // apb32
#define     I2S_0_I2S1_UL_SR_EN_CONTROL__F_CR_I2S1_IN_EN              (I2S_0_BASE+0x0038) // apb8:1
#define     I2S_0_I2S1_UL_SR_EN_CONTROL__F_CR_I2S1_IN_SR              (I2S_0_BASE+0x0039) // apb8:5
#define     I2S_0_I2S1_UL_SR_EN_CONTROL__F_CR_PDN_I2SIN1              (I2S_0_BASE+0x003a) // apb8:1
#define     I2S_0_I2S_MONITOR                                         (I2S_0_BASE+0x003c) // apb32
#define     I2S_0_I2S_MONITOR__F_CR_I2S_MONITOR                       (I2S_0_BASE+0x003c) // apb32:32
#define     I2S_0_I2S1_DL_INT_CONTROL                                 (I2S_0_BASE+0x0040) // apb32
#define     I2S_0_I2S1_DL_INT_CONTROL__F_CR_DL_CONTRL_ITEN            (I2S_0_BASE+0x0040) // apb8:1
#define     I2S_0_I2S1_DL_INT_CONTROL__F_CR_DL_INTSTS_INT             (I2S_0_BASE+0x0041) // apb8:1
#define     I2S_0_I2S1_UL_INT_CONTROL                                 (I2S_0_BASE+0x0044) // apb32
#define     I2S_0_I2S1_UL_INT_CONTROL__F_CR_UL_CONTRL_ITEN            (I2S_0_BASE+0x0044) // apb8:1
#define     I2S_0_I2S1_UL_INT_CONTROL__F_CR_UL_INTSTS_INT             (I2S_0_BASE+0x0045) // apb8:1
#define     I2S_0_I2S1_INT_ACK_CONTROL                                (I2S_0_BASE+0x0048) // apb32
#define     I2S_0_I2S1_INT_ACK_CONTROL__F_CR_UL_ACKINT_ACK            (I2S_0_BASE+0x0048) // apb8:1
#define     I2S_0_I2S1_INT_ACK_CONTROL__F_CR_DL_ACKINT_ACK            (I2S_0_BASE+0x0049) // apb8:1

/*  I2S1 REG */
#define     I2S_1_I2S1_GLOBAL_CONTROL                                 (I2S_1_BASE+0x0000) // apb32
#define     I2S_1_I2S1_GLOBAL_CONTROL__F_CR_I2S1_GLOBAL_CONTROL       (I2S_1_BASE+0x0000) // apb32:32
#define     I2S_1_I2S1_DL_CONTROL                                     (I2S_1_BASE+0x0004) // apb32
#define     I2S_1_I2S1_DL_CONTROL__F_CR_I2S1_DL_CONTROL               (I2S_1_BASE+0x0004) // apb32:32
#define     I2S_1_I2S1_UL_CONTROL                                     (I2S_1_BASE+0x0008) // apb32
#define     I2S_1_I2S1_UL_CONTROL__F_CR_I2S1_UL_CONTROL               (I2S_1_BASE+0x0008) // apb32:32
#define     I2S_1_I2S1_SOFT_RESET                                     (I2S_1_BASE+0x000c) // apb32
#define     I2S_1_I2S1_SOFT_RESET__F_CR_I2S1_SOFT_RSTB                (I2S_1_BASE+0x000c) // apb8:1
#define     I2S_1_I2S1_DL_FIFO                                        (I2S_1_BASE+0x0010) // apb32
#define     I2S_1_I2S1_DL_FIFO__F_CR_I2S1_FIFO_DL_RCLEAR              (I2S_1_BASE+0x0010) // apb8:1
#define     I2S_1_I2S1_DL_FIFO__F_CR_I2S1_FIFO_DL_R_THRESHOLD         (I2S_1_BASE+0x0011) // apb8:4
#define     I2S_1_I2S1_DL_FIFO__F_CR_I2S1_FIFO_DL_WCLEAR              (I2S_1_BASE+0x0012) // apb8:1
#define     I2S_1_I2S1_DL_FIFO__F_CR_I2S1_FIFO_DL_W_THRESHOLD         (I2S_1_BASE+0x0013) // apb8:4
#define     I2S_1_I2S1_UL_FIFO                                        (I2S_1_BASE+0x0014) // apb32
#define     I2S_1_I2S1_UL_FIFO__F_CR_I2S1_FIFO_UL_RCLEAR              (I2S_1_BASE+0x0014) // apb8:1
#define     I2S_1_I2S1_UL_FIFO__F_CR_I2S1_FIFO_UL_R_THRESHOLD         (I2S_1_BASE+0x0015) // apb8:4
#define     I2S_1_I2S1_UL_FIFO__F_CR_I2S1_FIFO_UL_WCLEAR              (I2S_1_BASE+0x0016) // apb8:1
#define     I2S_1_I2S1_UL_FIFO__F_CR_I2S1_FIFO_UL_W_THRESHOLD         (I2S_1_BASE+0x0017) // apb8:4
#define     I2S_1_I2S1_DL_FIFO_STATUS                                 (I2S_1_BASE+0x0018) // apb32
#define     I2S_1_I2S1_DL_FIFO_STATUS__F_CR_I2S1_FIFO_DL_STATUS       (I2S_1_BASE+0x0018) // apb8:6
#define     I2S_1_I2S1_DL_FIFO_STATUS__F_CR_I2S1_FIFO_DL_RFIFO_CNT    (I2S_1_BASE+0x0019) // apb8:4
#define     I2S_1_I2S1_DL_FIFO_STATUS__F_CR_I2S1_FIFO_DL_WFIFO_CNT    (I2S_1_BASE+0x001a) // apb8:4
#define     I2S_1_I2S1_UL_FIFO_STATUS                                 (I2S_1_BASE+0x001c) // apb32
#define     I2S_1_I2S1_UL_FIFO_STATUS__F_CR_I2S1_FIFO_UL_STATUS       (I2S_1_BASE+0x001c) // apb8:6
#define     I2S_1_I2S1_UL_FIFO_STATUS__F_CR_I2S1_FIFO_UL_RFIFO_CNT    (I2S_1_BASE+0x001d) // apb8:4
#define     I2S_1_I2S1_UL_FIFO_STATUS__F_CR_I2S1_FIFO_UL_WFIFO_CNT    (I2S_1_BASE+0x001e) // apb8:4
#define     I2S_1_I2S1_SCAN_RSV                                       (I2S_1_BASE+0x0020) // apb32
#define     I2S_1_I2S1_SCAN_RSV__F_CR_I2S1_RESERVED                   (I2S_1_BASE+0x0020) // apb32:32
#define     I2S_1_I2S1_GLOBAL_EN_CONTROL                              (I2S_1_BASE+0x0030) // apb32
#define     I2S_1_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_ENABLE            (I2S_1_BASE+0x0030) // apb8:1
#define     I2S_1_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_DL_FIFO_EN        (I2S_1_BASE+0x0031) // apb8:1
#define     I2S_1_I2S1_GLOBAL_EN_CONTROL__F_CR_I2S1_UL_FIFO_EN        (I2S_1_BASE+0x0032) // apb8:1
#define     I2S_1_I2S1_GLOBAL_EN_CONTROL__F_CR_PDN_AUD_26M            (I2S_1_BASE+0x0033) // apb8:1
#define     I2S_1_I2S1_DL_SR_EN_CONTROL                               (I2S_1_BASE+0x0034) // apb32
#define     I2S_1_I2S1_DL_SR_EN_CONTROL__F_CR_I2S1_OUT_EN             (I2S_1_BASE+0x0034) // apb8:1
#define     I2S_1_I2S1_DL_SR_EN_CONTROL__F_CR_I2S1_OUT_SR             (I2S_1_BASE+0x0035) // apb8:5
#define     I2S_1_I2S1_DL_SR_EN_CONTROL__F_CR_PDN_I2SO1               (I2S_1_BASE+0x0036) // apb8:1
#define     I2S_1_I2S1_UL_SR_EN_CONTROL                               (I2S_1_BASE+0x0038) // apb32
#define     I2S_1_I2S1_UL_SR_EN_CONTROL__F_CR_I2S1_IN_EN              (I2S_1_BASE+0x0038) // apb8:1
#define     I2S_1_I2S1_UL_SR_EN_CONTROL__F_CR_I2S1_IN_SR              (I2S_1_BASE+0x0039) // apb8:5
#define     I2S_1_I2S1_UL_SR_EN_CONTROL__F_CR_PDN_I2SIN1              (I2S_1_BASE+0x003a) // apb8:1
#define     I2S_1_I2S_MONITOR                                         (I2S_1_BASE+0x003c) // apb32
#define     I2S_1_I2S_MONITOR__F_CR_I2S_MONITOR                       (I2S_1_BASE+0x003c) // apb32:32
#define     I2S_1_I2S1_DL_INT_CONTROL                                 (I2S_1_BASE+0x0040) // apb32
#define     I2S_1_I2S1_DL_INT_CONTROL__F_CR_DL_CONTRL_ITEN            (I2S_1_BASE+0x0040) // apb8:1
#define     I2S_1_I2S1_DL_INT_CONTROL__F_CR_DL_INTSTS_INT             (I2S_1_BASE+0x0041) // apb8:1
#define     I2S_1_I2S1_UL_INT_CONTROL                                 (I2S_1_BASE+0x0044) // apb32
#define     I2S_1_I2S1_UL_INT_CONTROL__F_CR_UL_CONTRL_ITEN            (I2S_1_BASE+0x0044) // apb8:1
#define     I2S_1_I2S1_UL_INT_CONTROL__F_CR_UL_INTSTS_INT             (I2S_1_BASE+0x0045) // apb8:1
#define     I2S_1_I2S1_INT_ACK_CONTROL                                (I2S_1_BASE+0x0048) // apb32
#define     I2S_1_I2S1_INT_ACK_CONTROL__F_CR_UL_ACKINT_ACK            (I2S_1_BASE+0x0048) // apb8:1
#define     I2S_1_I2S1_INT_ACK_CONTROL__F_CR_DL_ACKINT_ACK            (I2S_1_BASE+0x0049) // apb8:1

/*XPLL REG*/
#define     XPLL_CTL0                                                 (XPLL_CTRL_BASE+0x0000) // apb32
#define     XPLL_CTL0__F_RG_XPLL_PLL_DDS_PWR_ON                       (XPLL_CTRL_BASE+0x0000) // apb8:1
#define     XPLL_CTL0__F_RG_XPLL_PLL_DDS_ISO_EN                       (XPLL_CTRL_BASE+0x0001) // apb8:1
#define     XPLL_CTL0__F_AD_RGS_XPLL_PLL_DDS_PWR_ACK                  (XPLL_CTRL_BASE+0x0002) // apb8:1
#define     XPLL_CTL0__F_RG_XPLL_V2I_EN                               (XPLL_CTRL_BASE+0x0003) // apb8:1
#define     XPLL_CTL1                                                 (XPLL_CTRL_BASE+0x0004) // apb32
#define     XPLL_CTL1__F_RG_XPLL_PLL_EN                               (XPLL_CTRL_BASE+0x0004) // apb8:1
#define     XPLL_CTL1__F_RG_XPLL_PLL_LF                               (XPLL_CTRL_BASE+0x0005) // apb8:1
#define     XPLL_CTL1__F_RG_XPLL_PLL_POSDIV                           (XPLL_CTRL_BASE+0x0006) // apb8:2
#define     XPLL_CTL1__F_RG_XPLL_PLL_PREDIV                           (XPLL_CTRL_BASE+0x0007) // apb8:2
#define     XPLL_CTL2                                                 (XPLL_CTRL_BASE+0x0008) // apb32
#define     XPLL_CTL2__F_RG_XPLL_PLL_FBKDIV                           (XPLL_CTRL_BASE+0x0008) // apb8:8
#define     XPLL_CTL2__F_RG_XPLL_PLL_BR                               (XPLL_CTRL_BASE+0x0009) // apb8:1
#define     XPLL_CTL2__F_RG_XPLL_PLL_BP                               (XPLL_CTRL_BASE+0x000a) // apb8:1
#define     XPLL_CTL2__F_RG_XPLL_FBDIV_PREDIV2_EN                     (XPLL_CTRL_BASE+0x000b) // apb8:1
#define     XPLL_CTL3                                                 (XPLL_CTRL_BASE+0x000c) // apb32
#define     XPLL_CTL3__F_RG_XPLL_PLL_MONREF_EN                        (XPLL_CTRL_BASE+0x000c) // apb8:1
#define     XPLL_CTL3__F_RG_XPLL_PLL_MONVC_EN                         (XPLL_CTRL_BASE+0x000d) // apb8:1
#define     XPLL_CTL3__F_RG_XPLL_PLL_DIVEN                            (XPLL_CTRL_BASE+0x000e) // apb8:3
#define     XPLL_CTL3__F_RG_XPLL_PLL_MONCK_EN                         (XPLL_CTRL_BASE+0x000f) // apb8:1
#define     XPLL_CTL4                                                 (XPLL_CTRL_BASE+0x0010) // apb32
#define     XPLL_CTL4__F_RG_XPLL_PLL_DDSFBK_EN                        (XPLL_CTRL_BASE+0x0010) // apb8:1
#define     XPLL_CTL4__F_RG_XPLL_PLL_VOD_EN                           (XPLL_CTRL_BASE+0x0011) // apb8:1
#define     XPLL_CTL4__F_RG_XPLL_PLL_LVROD_EN                         (XPLL_CTRL_BASE+0x0012) // apb8:1
#define     XPLL_CTL4__F_RG_XPLL_PLL_RST_DLY                          (XPLL_CTRL_BASE+0x0013) // apb8:2
#define     XPLL_CTL5                                                 (XPLL_CTRL_BASE+0x0014) // apb32
#define     XPLL_CTL5__F_RG_XPLL_LDOOUT2_SEL                          (XPLL_CTRL_BASE+0x0014) // apb8:2
#define     XPLL_CTL5__F_RG_XPLL_LDOOUT1_SEL                          (XPLL_CTRL_BASE+0x0015) // apb8:2
#define     XPLL_CTL5__F_RG_XPLL_VREF_FC                              (XPLL_CTRL_BASE+0x0016) // apb8:1
#define     XPLL_CTL5__F_RG_XPLL_VREF_SEL                             (XPLL_CTRL_BASE+0x0017) // apb8:1
#define     XPLL_CTL6                                                 (XPLL_CTRL_BASE+0x0018) // apb32
#define     XPLL_CTL6__F_RG_XPLL_KVCO_SEL                             (XPLL_CTRL_BASE+0x0018) // apb8:2
#define     XPLL_CTL6__F_RG_XPLL_IBANK_FC                             (XPLL_CTRL_BASE+0x0019) // apb8:1
#define     XPLL_CTL6__F_RG_XPLL_IBANK_FINETUNE                       (XPLL_CTRL_BASE+0x001a) // apb8:3
#define     XPLL_CTL6__F_RG_XPLL_LDOOUT3_SEL                          (XPLL_CTRL_BASE+0x001b) // apb8:2
#define     XPLL_CTL7                                                 (XPLL_CTRL_BASE+0x001c) // apb32
#define     XPLL_CTL7__F_RG_XPLL_VTMON_WINDOW_SEL                     (XPLL_CTRL_BASE+0x001c) // apb8:2
#define     XPLL_CTL7__F_RG_XPLL_VTMON_EN                             (XPLL_CTRL_BASE+0x001d) // apb8:1
#define     XPLL_CTL7__F_RG_XPLL_LOCKDET_WINDOW_SEL                   (XPLL_CTRL_BASE+0x001e) // apb8:2
#define     XPLL_CTL7__F_RG_XPLL_LOCKDET_EN                           (XPLL_CTRL_BASE+0x001f) // apb8:1
#define     XPLL_CTL8                                                 (XPLL_CTRL_BASE+0x0020) // apb32
#define     XPLL_CTL8__F_RG_XPLL_TESTBUF_EN                           (XPLL_CTRL_BASE+0x0020) // apb8:1
#define     XPLL_CTL8__F_RG_XPLL_CLKMUX_SEL                           (XPLL_CTRL_BASE+0x0021) // apb8:2
#define     XPLL_CTL8__F_RG_XPLL_CP_ISEL                              (XPLL_CTRL_BASE+0x0022) // apb8:2
#define     XPLL_CTL8__F_RG_XPLL_CLK_BYPASS_EN                        (XPLL_CTRL_BASE+0x0023) // apb8:1
#define     XPLL_CTL9                                                 (XPLL_CTRL_BASE+0x0024) // apb32
#define     XPLL_CTL9__F_RG_XPLL_PLL_RESERVE                          (XPLL_CTRL_BASE+0x0024) // apb8:8
#define     XPLL_CTL9__F_RG_XPLL_TESTDIV2_EN                          (XPLL_CTRL_BASE+0x0025) // apb8:1
#define     XPLL_CTL9__F_RG_XPLL_TESTBUF_SUPPLY_SEL                   (XPLL_CTRL_BASE+0x0026) // apb8:2
#define     XPLL_CTL9__F_RG_XPLL_TESTBUF_DRIVING_SEL                  (XPLL_CTRL_BASE+0x0027) // apb8:2
#define     XPLL_CTL10                                                (XPLL_CTRL_BASE+0x0028) // apb32
#define     XPLL_CTL10__F_RG_XPLL_LCDDS_PCW_NCPO                      (XPLL_CTRL_BASE+0x0028) // apb32:32
#define     XPLL_CTL11                                                (XPLL_CTRL_BASE+0x002c) // apb32
#define     XPLL_CTL11__F_RG_XPLL_LCDDS_PWDB                          (XPLL_CTRL_BASE+0x002c) // apb8:1
#define     XPLL_CTL11__F_RG_XPLL_LCDDS_MONEN                         (XPLL_CTRL_BASE+0x002d) // apb8:1
#define     XPLL_CTL11__F_RG_XPLL_LCDDS_C                             (XPLL_CTRL_BASE+0x002e) // apb8:3
#define     XPLL_CTL11__F_RG_XPLL_LCDDS_PREDIV2                       (XPLL_CTRL_BASE+0x002f) // apb8:1
#define     XPLL_CTL12                                                (XPLL_CTRL_BASE+0x0030) // apb32
#define     XPLL_CTL12__F_RG_XPLL_LCDDS_PCW_NCPO_CHG                  (XPLL_CTRL_BASE+0x0030) // apb8:1
#define     XPLL_CTL12__F_RG_XPLL_LCDDS_SSC_EN                        (XPLL_CTRL_BASE+0x0031) // apb8:1
#define     XPLL_DBG_PROB                                             (XPLL_CTRL_BASE+0x0400) // apb32
#define     XPLL_DBG_PROB__F_XPLL_DBG_SEL                             (XPLL_CTRL_BASE+0x0400) // apb8:4
#define     XPLL_DBG_PROB__F_XPLL_FQMTR_EN                            (XPLL_CTRL_BASE+0x0403) // apb8:1
#define     XPLL_DBG_PROB_MON                                         (XPLL_CTRL_BASE+0x0404) // apb32
#define     XPLL_DBG_PROB_MON__F_XPLL_DBG_PROB                        (XPLL_CTRL_BASE+0x0404) // apb8:4

//DMA_BASE 0xA0020000
#define     VDMA22_COUNT                                              (DMA_BASE+0x0D10) // apb32
#define     VDMA22_CON                                                (DMA_BASE+0x0D14) // apb32
#define     VDMA22_START                                              (DMA_BASE+0x0D18) // apb32
#define     VDMA22_PGMADDR                                            (DMA_BASE+0x0D2c) // apb32
#define     VDMA22_FFCNT                                              (DMA_BASE+0x0D38) // apb32
#define     VDMA22_FFSIZE                                             (DMA_BASE+0x0D44) // apb32
#define     VDMA23_COUNT                                              (DMA_BASE+0x0E10) // apb32
#define     VDMA23_CON                                                (DMA_BASE+0x0E14) // apb32
#define     VDMA23_START                                              (DMA_BASE+0x0E18) // apb32
#define     VDMA23_PGMADDR                                            (DMA_BASE+0x0E2c) // apb32
#define     VDMA23_FFCNT                                              (DMA_BASE+0x0E38) // apb32
#define     VDMA23_FFSIZE                                             (DMA_BASE+0x0E44) // apb32
#define     VDMA24_COUNT                                              (DMA_BASE+0x0F10) // apb32
#define     VDMA24_CON                                                (DMA_BASE+0x0F14) // apb32
#define     VDMA24_START                                              (DMA_BASE+0x0F18) // apb32
#define     VDMA24_PGMADDR                                            (DMA_BASE+0x0F2c) // apb32
#define     VDMA24_FFCNT                                              (DMA_BASE+0x0F38) // apb32
#define     VDMA24_FFSIZE                                             (DMA_BASE+0x0F44) // apb32
#define     VDMA25_COUNT                                              (DMA_BASE+0x1010) // apb32
#define     VDMA25_CON                                                (DMA_BASE+0x1014) // apb32
#define     VDMA25_START                                              (DMA_BASE+0x1018) // apb32
#define     VDMA25_PGMADDR                                            (DMA_BASE+0x102c) // apb32
#define     VDMA25_FFCNT                                              (DMA_BASE+0x1038) // apb32
#define     VDMA25_FFSIZE                                             (DMA_BASE+0x1044) // apb32

/**************************Global variable***************************************************/

/** @MCLK SOURCE*/
typedef enum {
    HAL_I2S_CLOCK_XO_26M         = 0,   /**< XTAL 26MHZ. */
    HAL_I2S_CLOCK_XPLL_26M       = 1,   /**< XPLL 26MHZ. */
    HAL_I2S_CLOCK_XPLL_24_576M   = 2,   /**< XPLL 24.576MHZ. */
    HAL_I2S_CLOCK_XPLL_22_5792M  = 3,    /**< XPLL 22.5792MHZ. */
    HAL_I2S_CLOCK_MAX            = 4
} hal_i2s_clock_source_t;

typedef enum {
    HAL_I2S_FS_BASE_8KHZ         = 0,  /**< FS  is a multiple of 8khz. */
    HAL_I2S_FS_BASE_11_025KHZ    = 1   /**< FS  is a multiple of 11.025khz. */
} hal_i2s_fs_base_t;


typedef struct {
    uint32_t    *tx_vfifo_base;  //< Tx buffer- internal use
    uint32_t    tx_vfifo_length;
    uint32_t    tx_vfifo_threshold;
    bool        tx_dma_interrupt;
    bool        tx_dma_configured;

    uint32_t    *rx_vfifo_base;  //< Rx buffer- internal use
    uint32_t    rx_vfifo_length;
    uint32_t    rx_vfifo_threshold;
    bool        rx_dma_interrupt;
    bool        rx_dma_configured;
} i2s_vfifo_t;

typedef struct {
    hal_i2s_port_t              i2s_port;
    hal_i2s_config_t            i2s_user_config;
    hal_i2s_initial_type_t      i2s_initial_type;
    hal_i2s_clock_source_t      i2s_clock_source;
    /* state control */
    volatile uint8_t            i2s_state;
    i2s_vfifo_t                 i2s_vfifo;
    bool                        i2s_configured;
    bool                        i2s_audiotop_enabled;
    bool                        i2s_tx_eof;
    /* user defined callback functions */
    hal_i2s_tx_callback_t       user_tx_callback_func;
    hal_i2s_rx_callback_t       user_rx_callback_func;
    void                        *user_tx_data;
    void                        *user_rx_data;
} i2s_internal_t;

void i2s_xpll_open(i2s_internal_t *hal_i2s);
void i2s_xpll_close(void);
void i2s_set_initial_parameter(i2s_internal_t *hal_i2s);
void i2s_set_parameter(i2s_internal_t *hal_i2s);
hal_i2s_status_t i2s_set_clock(i2s_internal_t *hal_i2s, hal_i2s_sample_rate_t sample_rate);

#ifdef __cplusplus
}
#endif

#endif  /* defined(HAL_I2S_MODULE_ENABLED)*/
#endif /* __HAL_I2S_INTERNAL_H__ */

