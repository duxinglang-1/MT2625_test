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

#ifndef _HAL_NVIC_INTERNAL_H_
#define _HAL_NVIC_INTERNAL_H_
#include "hal_nvic.h"

#ifdef HAL_NVIC_MODULE_ENABLED
/* Please refer to configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY= (3)!! in freertos_config.h
   Everything under/equal DEFAULT_IRQ_PRIORITY(1) + (3) can use FreeRTOS APIs
*/
#define FREERTOS_ISR_API_PRIORITY  (1)          /* please keep the setting */
/* DEFAULT_IRQ_PRIORITY is equal to FREERTOS_ISR_API_PRIORITY to ensure all IRQs can use FreeRTOS APIs */
#define DEFAULT_IRQ_PRIORITY  (FREERTOS_ISR_API_PRIORITY) /* please keep the setting */

#define AP_SIDE_PRIORITY_START  OS_GPT_PRIORITY

#define THE_LOWEST_PRIORITY   ((1<<__NVIC_PRIO_BITS) -1)

#define MDSYS_IRQ_0_PRIORITY             (DEFAULT_IRQ_PRIORITY + 0)

#define MDSYS_IRQ_1_PRIORITY             (DEFAULT_IRQ_PRIORITY + 3)  /* Uses FreeRTOS API. Can't be better than configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY */
#define MDSYS_IRQ_2_PRIORITY             (DEFAULT_IRQ_PRIORITY + 4)
#define MDSYS_IRQ_3_PRIORITY             (DEFAULT_IRQ_PRIORITY + 5)
#define MDSYS_IRQ_4_PRIORITY              MDSYS_IRQ_1_PRIORITY  /* MDSYS 1, 4 and L1_SW_CRC_IRQ_PRIORITY must be on same priority (common data protected with this, cannot be changed) */
#define MDSYS_IRQ_5_PRIORITY             (DEFAULT_IRQ_PRIORITY + 6)
#define MDSYS_IRQ_6_PRIORITY             (DEFAULT_IRQ_PRIORITY + 6)  /* RF conflict interrupt */
#define MDSYS_IRQ_7_PRIORITY             (DEFAULT_IRQ_PRIORITY + 7)
#define L1_SW_IRQ_PRIORITY               (DEFAULT_IRQ_PRIORITY + 8)
#define L2_SW_IRQ_PRIORITY               (DEFAULT_IRQ_PRIORITY + 9)
#define L1_SW_CRC_IRQ_PRIORITY           (MDSYS_IRQ_1_PRIORITY) /* MDSYS 1, 4 and L1_SW_CRC_IRQ_PRIORITY must be on same priority (common data protected with this, cannot be changed) */
#define CM4_reserved1_PRIORITY           (THE_LOWEST_PRIORITY)
#define GKI_LOG_PRIORITY                 (DEFAULT_IRQ_PRIORITY + 45)
#define FIRQ_PRIORITY                    (DEFAULT_IRQ_PRIORITY + 13)
#define OS_GPT_PRIORITY                  (DEFAULT_IRQ_PRIORITY + 14)
#define DMA_MCU_PRIORITY                 (DEFAULT_IRQ_PRIORITY + 15)
#define DMA_SENSOR_PRIORITY              (DEFAULT_IRQ_PRIORITY + 16)
#define SPI_MST0_PRIORITY                (DEFAULT_IRQ_PRIORITY + 17)
#define SPI_MST1_PRIORITY                (DEFAULT_IRQ_PRIORITY + 17)
#define SPI_SLV_PRIORITY                 (DEFAULT_IRQ_PRIORITY + 18)
#define SDIO_SLV_PRIORITY                (DEFAULT_IRQ_PRIORITY + 19)
#define SDIO_MST0_PRIORITY               (DEFAULT_IRQ_PRIORITY + 20)
#define SDIO_MST1_PRIORITY               (DEFAULT_IRQ_PRIORITY + 20)
#define SDIO_MST0_WAKEUP_PRIORITY        (DEFAULT_IRQ_PRIORITY + 21)
#define SDIO_MST1_WAKEUP_PRIORITY        (DEFAULT_IRQ_PRIORITY + 21)
#define TRNG_PRIORITY                    (DEFAULT_IRQ_PRIORITY + 22)
#define CRYPTO_PRIORITY                  (DEFAULT_IRQ_PRIORITY + 23)
#define UART0_PRIORITY                   (DEFAULT_IRQ_PRIORITY + 24)
#define UART1_PRIORITY                   (DEFAULT_IRQ_PRIORITY + 24)
#define UART2_PRIORITY                   (DEFAULT_IRQ_PRIORITY + 24)
#define UART3_PRIORITY                   (DEFAULT_IRQ_PRIORITY + 24)
#define I2S0_PRIORITY                    (DEFAULT_IRQ_PRIORITY + 25)
#define I2S1_PRIORITY                    (DEFAULT_IRQ_PRIORITY + 25)
#define I2C0_PRIORITY                    (DEFAULT_IRQ_PRIORITY + 25)
#define I2C1_PRIORITY                    (DEFAULT_IRQ_PRIORITY + 25)
#define I2C2_PRIORITY                    (DEFAULT_IRQ_PRIORITY + 25)
#define RTC_PRIORITY                     (DEFAULT_IRQ_PRIORITY + 26)
#define GPT_PRIORITY                     (DEFAULT_IRQ_PRIORITY + 16)
#define SPM_PRIORITY                     (DEFAULT_IRQ_PRIORITY + 28)
#define RGU_PRIORITY                     (0)
#define PMU_PRIORITY                     (DEFAULT_IRQ_PRIORITY + 30)
#define EINT_PRIORITY                    (DEFAULT_IRQ_PRIORITY + 31)
#define SFC_PRIORITY                     (DEFAULT_IRQ_PRIORITY + 32)
#define SENSOR_PD_PRIORITY               (DEFAULT_IRQ_PRIORITY + 33)
#define SENSOR_AO_0_PRIORITY             (DEFAULT_IRQ_PRIORITY + 34)
#define SENSOR_AO_1_PRIORITY             (DEFAULT_IRQ_PRIORITY + 34)
#define SENSOR_AO_2_PRIORITY             (DEFAULT_IRQ_PRIORITY + 34)
#define SENSOR_AO_3_PRIORITY             (DEFAULT_IRQ_PRIORITY + 34)
#define KP_PRIORITY                      (DEFAULT_IRQ_PRIORITY + 16)
#define USB_PRIORITY                     (DEFAULT_IRQ_PRIORITY + 36)
#define USIM_PRIORITY                    (DEFAULT_IRQ_PRIORITY + 37)
#define CIPHER_PRIORITY                  (DEFAULT_IRQ_PRIORITY + 38)
#define DMA_PWM_PRIORITY                 (DEFAULT_IRQ_PRIORITY + 39)
#define ULS_PRIORITY                     (DEFAULT_IRQ_PRIORITY + 40)
#define MDSYS_IRQ_9_PRIORITY             (DEFAULT_IRQ_PRIORITY + 41)
#define MDSYS_IRQ_10_PRIORITY            (DEFAULT_IRQ_PRIORITY + 41)
#define MDSYS_IRQ_11_PRIORITY            (DEFAULT_IRQ_PRIORITY + 1)  /* T2F_F2T error interrupt */
#define MDSYS_IRQ_12_PRIORITY            (DEFAULT_IRQ_PRIORITY + 42)
#define MDSYS_IRQ_13_PRIORITY            (DEFAULT_IRQ_PRIORITY + 1)  /* DFE error interrupt */
#define MDSYS_IRQ_14_PRIORITY            (DEFAULT_IRQ_PRIORITY + 42)
#define MDSYS_IRQ_15_PRIORITY            (DEFAULT_IRQ_PRIORITY + 1) /* CS error interrupt */
#define MDSYS_IRQ_16_PRIORITY            (DEFAULT_IRQ_PRIORITY + 2) /* Viterbi CRC interrupt. Keep higher than MDSYS_IRQ1. Keep MDSYS 16 and 17 on same level */
#define MDSYS_IRQ_17_PRIORITY            (DEFAULT_IRQ_PRIORITY + 2) /* Viterbi CRC interrupt. Keep higher than MDSYS_IRQ1. Keep MDSYS 16 and 17 on same level */
#define MDSYS_IRQ_18_PRIORITY            (DEFAULT_IRQ_PRIORITY + 42)
#define MDSYS_IRQ_19_PRIORITY            (DEFAULT_IRQ_PRIORITY + 42)
#define MDSYS_IRQ_20_PRIORITY            (DEFAULT_IRQ_PRIORITY + 42)
#define MDSYS_IRQ_21_PRIORITY            (DEFAULT_IRQ_PRIORITY + 1)  /* AGC meas interrupt */
#define MDSYS_IRQ_22_PRIORITY            (DEFAULT_IRQ_PRIORITY + 42)
#define MDSYS_IRQ_23_PRIORITY            (DEFAULT_IRQ_PRIORITY + 42)
#define SECURITY_PRIORITY                (DEFAULT_IRQ_PRIORITY + 43)
#define BSIBPI0_PRIORITY                 (DEFAULT_IRQ_PRIORITY + 1)  /* BSIBPI error interrupt */
#define BSIBPI1_PRIORITY                 (DEFAULT_IRQ_PRIORITY + 2)  /* BSIBPI read interrupt. Keep higher than MDSYS_IRQ1 */
#define CM4_reserved2_PRIORITY           (THE_LOWEST_PRIORITY)
#define CM4_reserved3_PRIORITY           (THE_LOWEST_PRIORITY)
#define CSCI_MD_READ_PRIORITY            (DEFAULT_IRQ_PRIORITY + 45)
#define CSCI_MD_WRITE_PRIORITY           (DEFAULT_IRQ_PRIORITY + 45)
#define CSCI_AP_READ_PRIORITY            (DEFAULT_IRQ_PRIORITY + 45)
#define CSCI_AP_WRITE_PRIORITY           (DEFAULT_IRQ_PRIORITY + 45)



uint32_t save_and_set_interrupt_mask(void);
void restore_interrupt_mask(uint32_t mask);
hal_nvic_status_t nvic_irq_software_trigger(hal_nvic_irq_t irq_number);
uint32_t hal_nvic_query_exception_number(void);

#ifndef __UBL__
//#define NVIC_USE_BASEPRI
#endif

#ifdef NVIC_USE_BASEPRI
void nvic_check_interrupt_not_disable(void);
#else
#define nvic_check_interrupt_not_disable()
#endif

#endif //HAL_NVIC_MODULE_ENABLED
#endif //_HAL_NVIC_INTERNAL_H_

