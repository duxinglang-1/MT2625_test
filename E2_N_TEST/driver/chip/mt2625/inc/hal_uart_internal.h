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

#ifndef __HAL_UART_INTERNAL_H__
#define __HAL_UART_INTERNAL_H__

#ifdef HAL_UART_MODULE_ENABLED

#include <assert.h>
#include "hal_pdma_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UART_INPUT_CLOCK_DCXO 26000000
#define UART_DMA_MAX_SETTING_VALUE (2<<16)
#ifndef __UBL__
#define UART_ASSERT() assert(0)
#else
#define UART_ASSERT() while(1)
#endif

typedef enum {
    UART_INTERRUPT_NONE,
    UART_INTERRUPT_RECEIVE_ERROR,
    UART_INTERRUPT_RECEIVE_TIMEOUT,
    UART_INTERRUPT_RECEIVE_BREAK,
    UART_INTERRUPT_RECEIVE_AVAILABLE,
    UART_INTERRUPT_SEND_AVAILABLE,
    UART_INTERRUPT_SOFTWARE_FLOWCONTROL,
    UART_INTERRUPT_HARDWARE_FLOWCONTROL
} uart_interrupt_type_t;

typedef struct {
    bool is_rx;
    hal_uart_port_t uart_port;
} uart_dma_callback_data_t;

vdma_channel_t uart_port_to_dma_channel(hal_uart_port_t uart_port, int32_t is_rx);
void uart_dma_channel_to_callback_data(vdma_channel_t dma_channel, uart_dma_callback_data_t *user_data);
void uart_enable_dma(UART_REGISTER_T *uartx);
void uart_interrupt_handler(hal_nvic_irq_t irq_number);


void uart_reset_default_value(UART_REGISTER_T *uartx);
void uart_query_empty(UART_REGISTER_T *uartx);
void uart_set_high_baudrate(UART_REGISTER_T *uartx, uint32_t uart_clock, uint32_t baudrate);
void uart_set_low_baudrate(UART_REGISTER_T *uartx, uint32_t uart_clock, uint32_t baudrate);
void uart_set_baudrate(UART_REGISTER_T *uartx, uint32_t actual_baudrate);
void uart_set_auto_baudrate(UART_REGISTER_T *uartx, bool is_enable, uint32_t current_baudrate);
void uart_set_format(UART_REGISTER_T *uartx,
                     hal_uart_word_length_t word_length,
                     hal_uart_stop_bit_t stop_bit,
                     hal_uart_parity_t parity);
void uart_set_timeout_value(UART_REGISTER_T *uartx, uint32_t value);
int uart_probe_char(UART_REGISTER_T *uartx);
void uart_put_char_block(UART_REGISTER_T *uartx, uint8_t byte);
uint8_t uart_get_char_block(UART_REGISTER_T *uartx);
uint32_t uart_get_char_unblocking(UART_REGISTER_T *uartx);
void uart_set_hardware_flowcontrol(UART_REGISTER_T *uartx);
void uart_set_software_flowcontrol(UART_REGISTER_T *uartx,
                                   uint8_t xon,
                                   uint8_t xoff,
                                   uint8_t escape_character);
void uart_disable_flowcontrol(UART_REGISTER_T *uartx);
#ifdef HAL_SLEEP_MANAGER_ENABLED
void uart_unmask_send_interrupt(UART_REGISTER_T *uartx);
void uart_set_sleep_mode(UART_REGISTER_T *uartx);
#endif
void uart_set_fifo(UART_REGISTER_T *uartx);
vdma_channel_t uart_port_to_dma_channel(hal_uart_port_t uart_port, int32_t is_rx);
void uart_enable_dma(UART_REGISTER_T *uartx);
void uart_unmask_receive_interrupt(UART_REGISTER_T *uartx);
void uart_purge_fifo(UART_REGISTER_T *uartx, int32_t is_rx);
uart_interrupt_type_t uart_query_interrupt_type(UART_REGISTER_T *uartx);
int32_t uart_verify_error(UART_REGISTER_T *uartx);
void uart_clear_timeout_interrupt(UART_REGISTER_T *uartx);
void uart_send_handler(hal_uart_port_t uart_port, bool is_send_complete_trigger);
void uart_receive_handler(hal_uart_port_t uart_port, bool is_timeout);
void uart_error_handler(hal_uart_port_t uart_port);
bool uart_get_auto_baudrate_done(UART_REGISTER_T *uartx);
uint32_t uart_get_auto_baudrate_result(UART_REGISTER_T *uartx);
void uart_send_xoff(UART_REGISTER_T *uartx);
void uart_send_xon(UART_REGISTER_T *uartx);

#ifdef __cplusplus
}
#endif

#endif

#endif

