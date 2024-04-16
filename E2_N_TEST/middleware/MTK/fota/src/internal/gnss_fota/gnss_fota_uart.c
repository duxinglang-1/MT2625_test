/* Copyright Statement:
 *
 * (C) 2005-2017  MediaTek Inc. All rights reserved.
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

#include "gnss_fota_uart.h"
#include "hal_pinmux_define.h"
#include "hal_gpio.h"
#include "hal_uart.h"

/*************************************************************************************************
*                             Macro Definitions                                                  *
**************************************************************************************************/

/* UART3 Register address */
#define UART3_base       (0xA00F0000)
#define UART3_RBR        (UART3_base+0x0000)
#define UART3_THR        (UART3_base+0x0004)
#define UART3_DLL        (UART3_base+0x0008)
#define UART3_IER        (UART3_base+0x000C)
#define UART3_IIR        (UART3_base+0x0010)
#define UART3_FCR        (UART3_base+0x0014)
#define UART3_EFR        (UART3_base+0x0018)
#define UART3_LCR        (UART3_base+0x001C)
#define UART3_MCR        (UART3_base+0x0020)
#define UART3_LSR        (UART3_base+0x0028)
#define UART3_SCR        (UART3_base+0x002C)
#define UART3_HIGHSPEED  (UART3_base+0x0034)
#define UART3_SAMPLE     (UART3_base+0x0038)
#define UART3_RATEFIX    (UART3_base+0x0040)

/* LSR */
#define   UART_LSR_DR                0x0001
#define   UART_LSR_OE               (0x1<<1)
#define   UART_LSR_PE               (0x1<<2)
#define   UART_LSR_FE               (0x1<<3)
#define   UART_LSR_BI               (0x1<<4)
#define   UART_LSR_THRE             (0x1<<5)
#define   UART_LSR_TEMT             (0x1<<6)
#define   UART_LSR_FIFOERR          (0x1<<7)

#define UART_ReadReg(_addr)         (uint16_t)(*(volatile uint8_t *)_addr)
#define UART_WriteReg(_addr,_data)  *(volatile uint8_t *)_addr = (uint8_t)_data

/*GPT timer register*/
#define GPT_base                    (0xA2130000)
#define GPT_TIMER4_COUNT            ((volatile uint16_t *)(GPT_base + 0xE8))

/* UART Error threshold. */
#define err_threshold 0x1FFFFFF

/*************************************************************************************************
*                             Local Functions Definitions                                        *
**************************************************************************************************/
static bool gnss_fota_uart_get_byte_status(uint8_t *data);
/*************************************************************************************************
*                             Public Functions Implementation                                    *
**************************************************************************************************/
void gnss_fota_uart_init(void)
{
    hal_uart_config_t uart_config;
    hal_uart_deinit(HAL_UART_3);
    hal_pinmux_set_function(HAL_GPIO_15, HAL_GPIO_15_UART3_TXD);
    hal_pinmux_set_function(HAL_GPIO_14, HAL_GPIO_14_UART3_RXD);

    /* default baudrate is 115200 */
    uart_config.baudrate = HAL_UART_BAUDRATE_115200;
    uart_config.word_length = HAL_UART_WORD_LENGTH_8;
    uart_config.stop_bit = HAL_UART_STOP_BIT_1;
    uart_config.parity = HAL_UART_PARITY_NONE;
    hal_uart_init(HAL_UART_3, &uart_config);
}

uint8_t gnss_fota_uart_get_byte(void)
{
    return hal_uart_get_char(HAL_UART_3);
}

bool gnss_fota_uart_get_byte_no_timeout(uint8_t *data)
{
    uint16_t LSR;

    LSR = UART_ReadReg(UART3_LSR);
    if (LSR & UART_LSR_DR) {
        *data = (uint8_t)UART_ReadReg(UART3_RBR);
        return true;
    } else {
        return false;
    }
}

bool gnss_fota_uart_get_byte_buffer(uint32_t *buf, uint32_t length)
{
    bool ret;
    uint32_t i;
    uint8_t *buf8 = (uint8_t *)buf;

    for (i = 0; i < length; i++) {
        ret = gnss_fota_uart_get_byte_status(buf8 + i);
        if (ret == false) {
            return false;
        }
    }
    return true;
}

void gnss_fota_uart_put_byte(uint8_t data)
{
    hal_uart_put_char(HAL_UART_3, data);
}

void gnss_fota_uart_put_byte_buffer(uint8_t *buf, uint32_t length)
{
    uint32_t i;

    for (i = 0; i < length; i++) {
        gnss_fota_uart_put_byte(*(buf + i));
    }
}

uint16_t gnss_fota_uart_get_uint16(void)
{
    uint8_t tmp, index;
    uint16_t tmp16;
    uint16_t result = 0;

    for (index = 0; index < 2; index++) {
        tmp = gnss_fota_uart_get_byte();
        tmp16 = (uint16_t)tmp;
        result |= (tmp16 << (8 - 8 * index));
    }
    return result;
}

void gnss_fota_uart_put_uint16(uint16_t data)
{
    uint8_t tmp, index;
    uint16_t tmp16;

    for (index = 0; index < 2; index++) {
        tmp16 = (data >> (8 - 8 * index));
        tmp = (uint8_t)tmp16;
        gnss_fota_uart_put_byte(tmp);
    }
}

uint32_t gnss_fota_uart_get_uint32(void)
{
    uint8_t tmp, index;
    uint32_t tmp32;
    uint32_t result = 0;
    for (index = 0; index < 4; index++) {
        tmp = gnss_fota_uart_get_byte();
        tmp32 = (uint32_t)tmp;
        result |= (tmp32 << (24 - 8 * index));
    }
    return result;
}

void gnss_fota_uart_put_uint32(uint32_t data)
{
    uint8_t tmp, index;
    uint32_t tmp32;

    for (index = 0; index < 4; index++) {
        tmp32 = (data >> (24 - 8 * index));
        tmp = (uint8_t)tmp32;
        gnss_fota_uart_put_byte(tmp);
    }
}

void gnss_fota_uart_clear_buffer(void)
{
    gnss_fota_uart_delay();
    *((volatile uint32_t*)(UART3_base + 0x14)) = 0x0;
    *((volatile uint32_t*)(UART3_base + 0x14)) = 0x1010000;
    *((volatile uint32_t*)(UART3_base + 0x14)) = 0x1;
    gnss_fota_uart_delay();
}

void gnss_fota_uart_delay(void)
{
    volatile uint32_t index_loop;
    volatile uint32_t index;

    for (index_loop = 0; index_loop < (20 * 2 / 4); index_loop++) {
        for (index = 0; index < 10000; index++) {
            ;
        }
    }
}

void gnss_fota_delay_ms(uint16_t ms)
{
    uint32_t pre_tick = *GPT_TIMER4_COUNT;
    uint32_t wait_ticks = ms * 13000;

    volatile uint32_t now_tick = *GPT_TIMER4_COUNT;
    while (1) {
        now_tick = *GPT_TIMER4_COUNT;
        if ((now_tick - pre_tick) > wait_ticks) {
            break;
        }
    }
}
/*************************************************************************************************
*                             Local Functions Implementation                                     *
**************************************************************************************************/
static bool gnss_fota_uart_get_byte_status(uint8_t *data)
{
    uint16_t LSR;
    uint32_t err_count;

    err_count = 0;
    while (1) {
        LSR = UART_ReadReg(UART3_LSR);

        if ((LSR & UART_LSR_FIFOERR) ||
            (LSR & UART_LSR_OE) || (LSR & UART_LSR_PE) ) {
            err_count = 0;
        }

        if (LSR & UART_LSR_DR) {
            *data = (uint8_t)UART_ReadReg(UART3_RBR);
            return true;
        } else {
            err_count++;
            if (err_count > err_threshold) {
                return false;
            }
        }
    }
}
