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

#include "hal_uart.h"
#if MTK_SWLA_ENABLE
#include "swla.h"
#endif
#ifdef HAL_UART_MODULE_ENABLED

#include "memory_attribute.h"
#ifdef HAL_SLEEP_MANAGER_ENABLED
#include "hal_gpt.h"
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_internal.h"
#include "core_cm4.h"
#endif
//#include "hal_pdma_internal.h"
#include "hal_nvic.h"
#include "hal_nvic_internal.h"
#include "hal_clock.h"
#include "hal_uart_internal.h"
#include "hal_eint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UART_HWSTATUS_UNINITIALIZED,
    UART_HWSTATUS_POLL_INITIALIZED,
    UART_HWSTATUS_DMA_INITIALIZED,
} uart_hwstatus_t;

typedef struct {
    hal_uart_callback_t func;
    void *arg;
} uart_callback_t;

#define UART_WAKE_UP_LOCK_SEC 10

static bool g_uart_global_data_initialized = false;
static hal_uart_baudrate_t g_baudrate[HAL_UART_MAX];
#ifdef HAL_SLEEP_MANAGER_ENABLED
static bool g_uart_frist_send_complete_interrupt[HAL_UART_MAX];
static bool g_uart_send_lock_status[HAL_UART_MAX];
static hal_uart_flowcontrol_t g_uart_flowcontrol_status[HAL_UART_MAX];
static hal_uart_sw_flowcontrol_config_t g_uart_sw_flowcontrol_config[HAL_UART_MAX];
static hal_uart_config_t g_uart_config[HAL_UART_MAX];
static sleep_management_lock_request_t uart_sleep_handle[HAL_UART_MAX] = {SLEEP_LOCK_UART0, SLEEP_LOCK_UART1, SLEEP_LOCK_UART2, SLEEP_LOCK_UART3};
static bool g_uart_irq_pending_status[HAL_UART_MAX];
static hal_eint_number_t uart_eint_number[HAL_UART_MAX] = {HAL_EINT_NUMBER_24, HAL_EINT_NUMBER_25, HAL_EINT_NUMBER_26, HAL_EINT_NUMBER_27};
#ifdef MT2503_2625_DUAL
static bool g_uart_wakeup_source[HAL_UART_MAX] = {false, false, true, false};
#else
static bool g_uart_wakeup_source[HAL_UART_MAX] = {true, false, false, false};
#endif
#endif
static volatile uart_hwstatus_t g_uart_hwstatus[HAL_UART_MAX];
static uart_callback_t g_uart_callback[HAL_UART_MAX];
static uart_dma_callback_data_t g_uart_dma_callback_data[HAL_UART_MAX * 2];
static hal_uart_dma_config_t g_uart_dma_config[HAL_UART_MAX];
#ifdef HAL_UART_FEATURE_3M_BAUDRATE
static const uint32_t g_uart_baudrate_map[] = {110, 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600, 3000000, 1500000};
#else
static const uint32_t g_uart_baudrate_map[] = {110, 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};
#endif
static bool g_uart_is_auto_baud[HAL_UART_MAX];
static bool g_uart_recover_auto_baud[HAL_UART_MAX] = {false, false, false, false};
extern UART_REGISTER_T *const g_uart_regbase[];
extern hal_clock_cg_id g_uart_port_to_pdn[];
extern hal_nvic_irq_t g_uart_port_to_irq_num[];
uint32_t uart_wakeup_timer_handle = NULL;
uint32_t g_uart_reg_dump[HAL_UART_MAX][32];
bool b_start_auto_baud[HAL_UART_MAX] = {false, false, false, false};
static const hal_uart_baudrate_t g_uart_auto_baudrate_map[] = {
    HAL_UART_BAUDRATE_115200,
    HAL_UART_BAUDRATE_57600,
    HAL_UART_BAUDRATE_38400,
    HAL_UART_BAUDRATE_19200,
    HAL_UART_BAUDRATE_9600,
    HAL_UART_BAUDRATE_4800,
    HAL_UART_BAUDRATE_2400,
    HAL_UART_BAUDRATE_1200,
    HAL_UART_BAUDRATE_300
};

static bool uart_port_is_valid(hal_uart_port_t uart_port)
{
    return (uart_port < HAL_UART_MAX);
}

static bool uart_baudrate_is_valid(hal_uart_baudrate_t baudrate)
{
    return (baudrate < HAL_UART_BAUDRATE_MAX);
}

static bool uart_config_is_valid(const hal_uart_config_t *config)
{
    return ((config->baudrate < HAL_UART_BAUDRATE_MAX) &&
            (config->word_length <= HAL_UART_WORD_LENGTH_8) &&
            (config->stop_bit <= HAL_UART_STOP_BIT_2) &&
            (config->parity <= HAL_UART_PARITY_EVEN));
}

static bool uart_check_auto_baudrate_done(hal_uart_port_t uart_port)
{
    UART_REGISTER_T *uartx;
    bool b_check_done;
#ifdef HAL_SLEEP_MANAGER_ENABLED
    hal_uart_callback_t uart_callback;
    void *uart_callback_arg;
#endif

    if (!uart_port_is_valid(uart_port)) {
        return false;
    }

    if(b_start_auto_baud[uart_port] == false)
        return false;

    uartx = g_uart_regbase[uart_port];

    b_check_done = uart_get_auto_baudrate_done(uartx);

#ifdef HAL_SLEEP_MANAGER_ENABLED
    if(b_check_done) {
        uart_callback = g_uart_callback[uart_port].func;
        uart_callback_arg = g_uart_callback[uart_port].arg;
        uart_callback(HAL_UART_EVENT_SAVE_AUTO_BAUD_RATE, uart_callback_arg);
        g_uart_config[uart_port].baudrate = g_uart_auto_baudrate_map[hal_uart_get_auto_baudrate_result(uart_port)];
        g_uart_is_auto_baud[uart_port] = false;
        b_start_auto_baud[uart_port] = false;
    }
#endif

    return b_check_done;
}

/* triggered by vfifo dma rx thershold interrupt or UART receive timeout interrupt.
 * 1. When vfifo dma rx thershold interrupt happen,
 * this function is called with is_timeout=false.
 * then call suer's callback to notice that data can be fetched from receive buffer.
 * 2. When UART receive timeout interrupt happen,
 * this function is called with is_timeout=true.
 * then call suer's callback to notice that data can be fetched from receive buffer.
 */
void uart_receive_handler(hal_uart_port_t uart_port, bool is_timeout)
{
    vdma_channel_t channel;
    uint32_t avail_bytes;
    hal_uart_callback_t callback;
    void *arg;
    UART_REGISTER_T *uartx;
    vdma_status_t status;

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_DMA_INITIALIZED) {
        UART_ASSERT();
        return;
    }

    uartx = g_uart_regbase[uart_port];
    channel = uart_port_to_dma_channel(uart_port, 1);

    if (is_timeout == true) {
        uart_clear_timeout_interrupt(uartx);
    }

    status = vdma_get_available_receive_bytes(channel, &avail_bytes);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return;
    }
    if (avail_bytes == 0) {
        return;
    }

    callback = g_uart_callback[uart_port].func;
    arg = g_uart_callback[uart_port].arg;
    if (callback == NULL) {
        UART_ASSERT();
        return;
    }

    callback(HAL_UART_EVENT_READY_TO_READ, arg);

    status = vdma_get_available_receive_bytes(channel, &avail_bytes);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return;
    }
    if (avail_bytes >= g_uart_dma_config[uart_port].receive_vfifo_threshold_size) {
        status = vdma_disable_interrupt(channel);
        if (status != VDMA_OK) {
            UART_ASSERT();
            return;
        }
    }
}

/* triggered by vfifo dma tx thershold interrupt or uart transmit complete interrupt.
 * 1. When vfifo dma tx thershold interrupt happen,
 * this function is called with is_send_complete_trigger=false.
 * then call suer's callback to notice that data can be put in send buffer again.
 * 2. When UART transmit complete interrupt happen,
 * this function is called with is_send_complete_trigger=true.
 * Now all user data has been sent out, so we call hal_sleep_manager_release_sleeplock()
 * to unlock sleep.
 */
void uart_send_handler(hal_uart_port_t uart_port, bool is_send_complete_trigger)
{
    vdma_channel_t channel;
    vdma_status_t status;
    uint32_t compare_space, avail_space;
    hal_uart_callback_t callback;
    void *arg;
#ifdef HAL_SLEEP_MANAGER_ENABLED
    uint32_t irq_status;
#endif

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_DMA_INITIALIZED) {
        UART_ASSERT();
        return;
    }

    channel = uart_port_to_dma_channel(uart_port, 0);
    if (is_send_complete_trigger == true) {
#ifdef HAL_SLEEP_MANAGER_ENABLED
        if (g_uart_frist_send_complete_interrupt[uart_port] == false) {
            irq_status = save_and_set_interrupt_mask();
            status = vdma_get_available_send_space(channel, &avail_space);
            if (status != VDMA_OK) {
                UART_ASSERT();
                return;
            }
            if (g_uart_dma_config[uart_port].send_vfifo_buffer_size - avail_space == 0) {
                if (g_uart_send_lock_status[uart_port] == true) {
                    if( hal_sleep_manager_is_sleep_handle_alive(uart_sleep_handle[uart_port]) == true) {
                        hal_sleep_manager_release_sleeplock(uart_sleep_handle[uart_port], HAL_SLEEP_LOCK_ALL);
                    }

                    g_uart_send_lock_status[uart_port] = false;
                }
            }
            restore_interrupt_mask(irq_status);
        } else {
            g_uart_frist_send_complete_interrupt[uart_port] = false;
        }
        return;
#endif
    } else {
        callback = g_uart_callback[uart_port].func;
        arg = g_uart_callback[uart_port].arg;
        if (callback == NULL) {
            UART_ASSERT();
            return;
        }

        callback(HAL_UART_EVENT_READY_TO_WRITE, arg);

        status = vdma_get_available_send_space(channel, &avail_space);
        if (status != VDMA_OK) {
            UART_ASSERT();
            return;
        }
        compare_space = g_uart_dma_config[uart_port].send_vfifo_buffer_size
                        - g_uart_dma_config[uart_port].send_vfifo_threshold_size;
        if (avail_space >= compare_space) {
            status = vdma_disable_interrupt(channel);
            if (status != VDMA_OK) {
                UART_ASSERT();
                return;
            }
        }
    }
}

/* Only triggered by UART error interrupt */
void uart_error_handler(hal_uart_port_t uart_port)
{
    UART_REGISTER_T *uartx;
    hal_uart_callback_t callback;
    void *arg;

    uartx = g_uart_regbase[uart_port];

    if (!uart_verify_error(uartx)) {
        uart_purge_fifo(uartx, 1);
        uart_purge_fifo(uartx, 0);
        callback = g_uart_callback[uart_port].func;
        arg = g_uart_callback[uart_port].arg;
        if (callback == NULL) {
            UART_ASSERT();
            return;
        }
        callback(HAL_UART_EVENT_TRANSACTION_ERROR, arg);
    }
}

static void uart_dma_callback_handler(vdma_event_t event, void  *user_data)
{
    uart_dma_callback_data_t *callback_data = (uart_dma_callback_data_t *)user_data;

    if (callback_data->is_rx == true) {
        uart_receive_handler(callback_data->uart_port, false);
    } else {
        uart_send_handler(callback_data->uart_port, false);
    }
}

#ifdef HAL_SLEEP_MANAGER_ENABLED
static hal_uart_port_t get_eint_port(hal_eint_number_t index)
{
    if(index == uart_eint_number[HAL_UART_0]) {
        return HAL_UART_0;
    } else if (index == uart_eint_number[HAL_UART_1]) {
        return HAL_UART_1;
    } else if (index == uart_eint_number[HAL_UART_2]) {
        return HAL_UART_2;
    } else if (index == uart_eint_number[HAL_UART_3]) {
        return HAL_UART_3;
    }

    return HAL_UART_MAX;
}

void uart_wake_up_timer_callback(void *user_data)
{
    hal_uart_port_t uart_port;

    uart_port = (hal_uart_port_t)user_data;

    /*Stop gpt timer*/
    hal_gpt_sw_stop_timer_ms(uart_wakeup_timer_handle);
    /*free gpt timer*/
    hal_gpt_sw_free_timer(uart_wakeup_timer_handle);

    uart_wakeup_timer_handle = NULL;
    /*unlock sleep*/
    if (g_uart_send_lock_status[uart_port] == true) {
        hal_sleep_manager_release_sleeplock(uart_sleep_handle[uart_port], HAL_SLEEP_LOCK_ALL);
        g_uart_send_lock_status[uart_port] = false;
    }
}

static void eint_uart_handler(hal_eint_number_t *eint_index)
{
    volatile hal_eint_number_t index;
    hal_uart_port_t uart_port;
    hal_gpt_status_t ret_gpt;

    index = *eint_index;
    hal_eint_deinit(index);
    uart_port = get_eint_port(index);

    if(uart_port == HAL_UART_MAX)
        return;

    if(uart_wakeup_timer_handle == NULL) {
        ret_gpt = hal_gpt_sw_get_timer(&uart_wakeup_timer_handle);
        if(ret_gpt != HAL_GPT_STATUS_OK) {
            UART_ASSERT();
        }
    }

    if (g_uart_send_lock_status[uart_port] == false) {
        hal_sleep_manager_acquire_sleeplock(uart_sleep_handle[uart_port], HAL_SLEEP_LOCK_ALL);
        g_uart_send_lock_status[uart_port] = true;
    }

    hal_gpt_sw_stop_timer_ms(uart_wakeup_timer_handle);

    ret_gpt = hal_gpt_sw_start_timer_ms(uart_wakeup_timer_handle,
                              UART_WAKE_UP_LOCK_SEC * 1000, //10 sec
                              (hal_gpt_callback_t)uart_wake_up_timer_callback, (void*)uart_port);

    if(ret_gpt != HAL_GPT_STATUS_OK){
        return;
    }
}

static void uart_eint_init(hal_uart_port_t uart_port)
{
    uint32_t eint_number, *peint_number;
    hal_eint_config_t config_eint;
    config_eint.trigger_mode = HAL_EINT_EDGE_FALLING;
    config_eint.debounce_time = 0;
    config_eint.firq_enable = 0;
    eint_number = uart_eint_number[uart_port];
    peint_number = &uart_eint_number[uart_port];
    hal_eint_init(eint_number, &config_eint);
    hal_eint_register_callback(eint_number, eint_uart_handler, peint_number);
    hal_eint_unmask(eint_number);
}

void uart_backup_all_registers(void)
{
    hal_uart_port_t uart_port;
    uart_hwstatus_t uart_hwstatus;
    hal_uart_flowcontrol_t uart_flowcontrol;
    hal_uart_callback_t uart_callback;
    void *uart_callback_arg;

    for (uart_port = HAL_UART_0; uart_port < HAL_UART_MAX; uart_port++) {
        if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_UNINITIALIZED) {
            uart_hwstatus = g_uart_hwstatus[uart_port];
            uart_flowcontrol = g_uart_flowcontrol_status[uart_port];
            uart_callback = g_uart_callback[uart_port].func;
            uart_callback_arg = g_uart_callback[uart_port].arg;
            hal_uart_deinit(uart_port);
            g_uart_hwstatus[uart_port] = uart_hwstatus;
            g_uart_flowcontrol_status[uart_port] = uart_flowcontrol;
            g_uart_callback[uart_port].func = uart_callback;
            g_uart_callback[uart_port].arg = uart_callback_arg;
            g_uart_irq_pending_status[uart_port] = NVIC_GetPendingIRQ(g_uart_port_to_irq_num[uart_port]);
            if (g_uart_wakeup_source[uart_port]) {
                uart_eint_init(uart_port);
            }
        }
    }
}

void uart_restore_all_registers(void)
{
    uint32_t delay;
    hal_uart_port_t uart_port;
    UART_REGISTER_T *uartx;
    uart_hwstatus_t uart_hwstatus;
    bool sleep_lock_status;
    uint32_t current_baudrate;

    for (uart_port = HAL_UART_0; uart_port < HAL_UART_MAX; uart_port++) {
        uart_hwstatus = g_uart_hwstatus[uart_port];
        if (uart_hwstatus != UART_HWSTATUS_UNINITIALIZED) {
            g_uart_hwstatus[uart_port] = UART_HWSTATUS_UNINITIALIZED;
            hal_uart_init(uart_port, &g_uart_config[uart_port]);
        }
        /* Because of hardware limitation, we have to send XON manually
         * when software flow control is turn on for that port.
         */

        if (g_uart_flowcontrol_status[uart_port] == HAL_UART_FLOWCONTROL_SOFTWARE) {
            uartx = g_uart_regbase[uart_port];
            uart_put_char_block(uartx, g_uart_sw_flowcontrol_config[uart_port].xon);
            /* must delay until xon character is sent out, to avoid xon error with escape */
                while(!(uartx->LSR & UART_LSR_THRE_MASK));
//            delay = ((1000000 * 12) / g_uart_config[uart_port].baudrate) + 1;
//            hal_gpt_delay_us(delay);
            hal_uart_set_software_flowcontrol(uart_port,
                                              g_uart_sw_flowcontrol_config[uart_port].xon,
                                              g_uart_sw_flowcontrol_config[uart_port].xoff,
                                              g_uart_sw_flowcontrol_config[uart_port].escape_character);
        } else if (g_uart_flowcontrol_status[uart_port] == HAL_UART_FLOWCONTROL_HARDWARE) {
            hal_uart_set_hardware_flowcontrol(uart_port);
        }

        if (uart_hwstatus == UART_HWSTATUS_DMA_INITIALIZED) {
            hal_uart_set_dma(uart_port, &g_uart_dma_config[uart_port]);
            hal_uart_register_callback(uart_port, g_uart_callback[uart_port].func, g_uart_callback[uart_port].arg);
        }

        if(g_uart_recover_auto_baud[uart_port] == true) {
            hal_uart_set_auto_baudrate(uart_port, true);
        }

        /* Workaround for sleep flow debug requirement */
        sleep_lock_status = hal_sleep_manager_is_sleep_handle_alive(uart_sleep_handle[uart_port]);
        if ((sleep_lock_status == true) && (g_uart_irq_pending_status[uart_port] != 0)) {
            /* sleep is locked and uart irq pending is active, then unlock one time */
            hal_sleep_manager_release_sleeplock(uart_sleep_handle[uart_port], HAL_SLEEP_LOCK_ALL);
        }
    }
}

void uart_check_lock_status(void)
{
    UART_REGISTER_T *uartx;
    hal_uart_port_t uart_port;
    bool sleep_lock_status;
    uint32_t irq_status;

    irq_status = save_and_set_interrupt_mask();
    for (uart_port = HAL_UART_0; uart_port < HAL_UART_MAX; uart_port++) {
        uartx = g_uart_regbase[uart_port];
        if ((g_uart_send_lock_status[uart_port] == true) && (uart_wakeup_timer_handle == NULL)) {
            if(uartx->LSR & UART_LSR_THRE_MASK) {
                if( hal_sleep_manager_is_sleep_handle_alive(uart_sleep_handle[uart_port]) == true) {
                    hal_sleep_manager_release_sleeplock(uart_sleep_handle[uart_port], HAL_SLEEP_LOCK_ALL);
                }

                g_uart_send_lock_status[uart_port] = false;
            }
        }
    }
    restore_interrupt_mask(irq_status);
}
#endif

hal_uart_status_t hal_uart_set_baudrate(hal_uart_port_t uart_port, hal_uart_baudrate_t baudrate)
{
    uint32_t actual_baudrate, irq_status;
    UART_REGISTER_T *uartx;

    if ((!uart_port_is_valid(uart_port)) ||
        (!uart_baudrate_is_valid(baudrate))) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    if (g_uart_hwstatus[uart_port] == UART_HWSTATUS_UNINITIALIZED) {
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }

    irq_status = save_and_set_interrupt_mask();
#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_uart_config[uart_port].baudrate = baudrate;
#endif
    g_baudrate[uart_port] = baudrate;
    restore_interrupt_mask(irq_status);

    uartx = g_uart_regbase[uart_port];
    actual_baudrate = g_uart_baudrate_map[baudrate];

    uart_set_baudrate(uartx, actual_baudrate);

    return HAL_UART_STATUS_OK;
}

hal_uart_status_t hal_uart_set_format(hal_uart_port_t uart_port,
                                      const hal_uart_config_t *config)
{
    uint32_t irq_status;
    UART_REGISTER_T *uartx;

    if ((!uart_port_is_valid(uart_port)) ||
        (!uart_config_is_valid(config))) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    if (g_uart_hwstatus[uart_port] == UART_HWSTATUS_UNINITIALIZED) {
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }

    irq_status = save_and_set_interrupt_mask();
#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_uart_config[uart_port].baudrate = config->baudrate;
    g_uart_config[uart_port].word_length = config->word_length;
    g_uart_config[uart_port].stop_bit = config->stop_bit;
    g_uart_config[uart_port].parity = config->parity;
#endif
    restore_interrupt_mask(irq_status);

    uartx = g_uart_regbase[uart_port];

    hal_uart_set_baudrate(uart_port, config->baudrate);
    uart_set_format(uartx, config->word_length, config->stop_bit, config->parity);

    return HAL_UART_STATUS_OK;
}

hal_uart_status_t hal_uart_init(hal_uart_port_t uart_port, hal_uart_config_t *uart_config)
{
    UART_REGISTER_T *uartx;
    uint32_t i, actual_baudrate, irq_status;
    hal_clock_status_t status;

    if ((!uart_port_is_valid(uart_port)) ||
        (!uart_config_is_valid(uart_config))) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    irq_status = save_and_set_interrupt_mask();
    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_UNINITIALIZED) {
        restore_interrupt_mask(irq_status);
        return HAL_UART_STATUS_ERROR_BUSY;
    }

    if (g_uart_global_data_initialized == false) {
        for (i = 0; i < HAL_UART_MAX; i++) {
            g_uart_hwstatus[i] = UART_HWSTATUS_UNINITIALIZED;
#ifdef HAL_SLEEP_MANAGER_ENABLED
            g_uart_flowcontrol_status[i] = HAL_UART_FLOWCONTROL_NONE;
            g_uart_frist_send_complete_interrupt[i] = false;
            g_uart_send_lock_status[i] = false;
#endif
            g_uart_callback[i].arg = NULL;
            g_uart_callback[i].func = NULL;
            g_uart_is_auto_baud[i] = false;
        }
        g_uart_global_data_initialized = true;
    }
#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_uart_config[uart_port].baudrate = uart_config->baudrate;
    g_uart_config[uart_port].word_length = uart_config->word_length;
    g_uart_config[uart_port].stop_bit = uart_config->stop_bit;
    g_uart_config[uart_port].parity = uart_config->parity;
#endif
    g_uart_hwstatus[uart_port] = UART_HWSTATUS_POLL_INITIALIZED;
    restore_interrupt_mask(irq_status);

    status = hal_clock_enable(g_uart_port_to_pdn[uart_port]);
    if (status != HAL_CLOCK_STATUS_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }

    uartx = g_uart_regbase[uart_port];

    uart_reset_default_value(uartx);
    actual_baudrate = g_uart_baudrate_map[uart_config->baudrate];
    g_baudrate[uart_port] = uart_config->baudrate;
    uart_set_baudrate(uartx, actual_baudrate);
    uart_set_format(uartx, uart_config->word_length, uart_config->stop_bit, uart_config->parity);
    uart_set_fifo(uartx);
#ifdef HAL_SLEEP_MANAGER_ENABLED
    uart_set_sleep_mode(uartx);
//    sleep_management_register_suspend_callback(SLEEP_BACKUP_RESTORE_UART, (sleep_management_suspend_callback_t)uart_backup_all_registers, NULL, HAL_SLEEP_MODE_LIGHT_SLEEP|HAL_SLEEP_MODE_DEEP_SLEEP|HAL_SLEEP_MODE_DEEPER_SLEEP);
//    sleep_management_register_resume_callback(SLEEP_BACKUP_RESTORE_UART, (sleep_management_resume_callback_t)uart_restore_all_registers, NULL, HAL_SLEEP_MODE_LIGHT_SLEEP|HAL_SLEEP_MODE_DEEP_SLEEP|HAL_SLEEP_MODE_DEEPER_SLEEP);
#endif

    return HAL_UART_STATUS_OK;
}

hal_uart_status_t hal_uart_deinit(hal_uart_port_t uart_port)
{
    UART_REGISTER_T *uartx;
    vdma_channel_t tx_dma_channel, rx_dma_channel;
    uint32_t irq_status;
    vdma_status_t status;
    hal_clock_status_t clock_status;

    if (!uart_port_is_valid(uart_port)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    if (g_uart_hwstatus[uart_port] == UART_HWSTATUS_UNINITIALIZED) {
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }

    uartx = g_uart_regbase[uart_port];

    /* wait all left data sent out before deinit. */
    uart_query_empty(uartx);

    /* unregister vdma module */
    if (g_uart_hwstatus[uart_port] == UART_HWSTATUS_DMA_INITIALIZED) {
        tx_dma_channel = uart_port_to_dma_channel(uart_port, 0);
        rx_dma_channel = uart_port_to_dma_channel(uart_port, 1);

        status = vdma_disable_interrupt(tx_dma_channel);
        if (status != VDMA_OK) {
            return HAL_UART_STATUS_ERROR;
        }
        status = vdma_disable_interrupt(rx_dma_channel);
        if (status != VDMA_OK) {
            return HAL_UART_STATUS_ERROR;
        }
        status = vdma_stop(tx_dma_channel);
        if (status != VDMA_OK) {
            return HAL_UART_STATUS_ERROR;
        }
        status = vdma_stop(rx_dma_channel);
        if (status != VDMA_OK) {
            return HAL_UART_STATUS_ERROR;
        }
        status = vdma_deinit(tx_dma_channel);
        if (status != VDMA_OK) {
            return HAL_UART_STATUS_ERROR;
        }
        status = vdma_deinit(rx_dma_channel);
        if (status != VDMA_OK) {
            return HAL_UART_STATUS_ERROR;
        }
        NVIC_DisableIRQ(g_uart_port_to_irq_num[uart_port]);
    }
    uart_reset_default_value(uartx);

    clock_status = hal_clock_disable(g_uart_port_to_pdn[uart_port]);
    if (clock_status != HAL_CLOCK_STATUS_OK) {
        return HAL_UART_STATUS_ERROR;
    }

    irq_status = save_and_set_interrupt_mask();
    g_uart_callback[uart_port].func = NULL;
    g_uart_callback[uart_port].arg = NULL;

#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_uart_frist_send_complete_interrupt[uart_port] = false;
    g_uart_flowcontrol_status[uart_port] = HAL_UART_FLOWCONTROL_NONE;
#endif

    g_uart_hwstatus[uart_port] = UART_HWSTATUS_UNINITIALIZED;

    restore_interrupt_mask(irq_status);

    return HAL_UART_STATUS_OK;
}

void hal_uart_put_char(hal_uart_port_t uart_port, char byte)
{
    UART_REGISTER_T *uartx;

    if (!uart_port_is_valid(uart_port)) {
        return;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_POLL_INITIALIZED) {
        return;
    }

    uartx = g_uart_regbase[uart_port];
    uart_put_char_block(uartx, byte);
}

uint32_t hal_uart_send_polling(hal_uart_port_t uart_port, const uint8_t *data, uint32_t size)
{
    uint32_t i = 0;

    if ((!uart_port_is_valid(uart_port)) || (data == NULL)) {
        return 0;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_POLL_INITIALIZED) {
        return 0;
    }

    for (i = 0; i < size; i++) {
        hal_uart_put_char(uart_port, *data);
        data++;
    }

    return size;
}

uint32_t hal_uart_send_dma(hal_uart_port_t uart_port, const uint8_t *data, uint32_t size)
{
    vdma_channel_t channel;
    uint32_t i, real_count, avail_space;
    vdma_status_t status;
#ifdef HAL_SLEEP_MANAGER_ENABLED
    uint32_t irq_status;
    hal_gpt_status_t ret_gpt;
#endif

    if ((!uart_port_is_valid(uart_port)) || (data == NULL) || (size == 0)) {
        return 0;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_DMA_INITIALIZED) {
        return 0;
    }

#if MTK_SAVE_AUTO_BAUD_RATE_ENABLE
    if(g_uart_is_auto_baud[uart_port]) {
        uart_check_auto_baudrate_done(uart_port);
    }
#endif

#ifdef HAL_SLEEP_MANAGER_ENABLED
    irq_status = save_and_set_interrupt_mask();
    if (g_uart_send_lock_status[uart_port] == false) {
        hal_sleep_manager_acquire_sleeplock(uart_sleep_handle[uart_port], HAL_SLEEP_LOCK_ALL);
        g_uart_send_lock_status[uart_port] = true;
    }
    restore_interrupt_mask(irq_status);
#endif

    channel = uart_port_to_dma_channel(uart_port, 0);

    status = vdma_get_available_send_space(channel, &avail_space);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return 0;
    }

    if (avail_space >= size) {
        real_count = size;
    } else {
        real_count = avail_space;
    }

#ifdef HAL_UART_FEATURE_3M_BAUDRATE
    if(g_baudrate[uart_port] == HAL_UART_BAUDRATE_3000000) {
        if(real_count <= 100) {
            for (i = 0; i < real_count; i++) {
                status = vdma_push_data(channel, data[i]);
                if (status != VDMA_OK) {
                    UART_ASSERT();
                    return 0;
                }
            }
        } else {
#if MTK_SWLA_ENABLE
            SLA_CustomLogging("UAR", SA_START);
#endif
#ifdef HAL_SLEEP_MANAGER_ENABLED
                hal_nvic_disable_irq(uart_port+UART0_IRQn);
#endif
            for (i = 0; i < 100; i++) {
                status = vdma_push_data(channel, data[i]);
                if (status != VDMA_OK) {
                    UART_ASSERT();
                    return 0;
                }
            }
#ifdef HAL_SLEEP_MANAGER_ENABLED
            hal_nvic_enable_irq(uart_port+UART0_IRQn);
#endif
            for (i = 100; i < real_count; i++) {
                status = vdma_push_data(channel, data[i]);
                if (status != VDMA_OK) {
                    UART_ASSERT();
                    return 0;
                }
            }
        }
#if MTK_SWLA_ENABLE
        SLA_CustomLogging("UAR", SA_STOP);
#endif
    } else
#endif
    {
        for (i = 0; i < real_count; i++) {
            status = vdma_push_data(channel, data[i]);
            if (status != VDMA_OK) {
                UART_ASSERT();
                return 0;
            }
        }
    }

    /* If avail space is not enough, turn on TX IRQ
       * so that UART driver can notice user when user's data has been sent out.
       */
    if (real_count == avail_space) {
        status = vdma_enable_interrupt(channel);
        if (status != VDMA_OK) {
            UART_ASSERT();
            return 0;
        }
    }

    return real_count;
}

char hal_uart_get_char(hal_uart_port_t uart_port)
{
    char data;
    UART_REGISTER_T *uartx;

    if (!uart_port_is_valid(uart_port)) {
        return 0;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_POLL_INITIALIZED) {
        return 0;
    }

    uartx = g_uart_regbase[uart_port];

    data = uart_get_char_block(uartx);

    return data;
}

uint32_t hal_uart_get_char_unblocking(hal_uart_port_t uart_port)
{
    uint32_t data;
    UART_REGISTER_T *uartx;

    if (!uart_port_is_valid(uart_port)) {
        return 0;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_POLL_INITIALIZED) {
        return 0;
    }

    uartx = g_uart_regbase[uart_port];

    data = uart_get_char_unblocking(uartx);

    return data;
}

uint32_t hal_uart_receive_polling(hal_uart_port_t uart_port, uint8_t *buffer, uint32_t size)
{
    uint32_t i;
    uint8_t *pbuf = buffer;

    if ((!uart_port_is_valid(uart_port)) ||
        (buffer == NULL)) {
        return 0;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_POLL_INITIALIZED) {
        return 0;
    }

    for (i = 0; i < size; i++) {
        pbuf[i] = hal_uart_get_char(uart_port);
    }

    return size;
}

uint32_t hal_uart_receive_dma(hal_uart_port_t uart_port, uint8_t *buffer, uint32_t size)
{
    vdma_channel_t channel;
    uint32_t receive_count, avail_count;
    uint32_t index;
    vdma_status_t status;

    if ((!uart_port_is_valid(uart_port)) ||
        (buffer == NULL) ||
        (size == 0)) {
        return 0;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_DMA_INITIALIZED) {
        return 0;
    }

    channel = uart_port_to_dma_channel(uart_port, 1);

    status = vdma_get_available_receive_bytes(channel, &avail_count);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return 0;
    }

    if (avail_count < size) {
        receive_count = avail_count;
    } else {
        receive_count = size;
    }
    for (index = 0; index < receive_count; index++) {
        status = vdma_pop_data(channel, &buffer[index]);
        if (status != VDMA_OK) {
            UART_ASSERT();
            return 0;
        }
    }

    /* If avail bytes is not enough, turn on RX IRQ
       * so that UART driver can notice user when new user's data has been received.
       */
    if (receive_count == avail_count) {
        status = vdma_enable_interrupt(channel);
        if (status != VDMA_OK) {
            UART_ASSERT();
            return 0;
        }
    }

    return receive_count;
}

static void uart_start_dma_transmission(hal_uart_port_t uart_port)
{
    UART_REGISTER_T *uartx;
    vdma_channel_t tx_dma_channel, rx_dma_channel;
    vdma_status_t status;

    uartx = g_uart_regbase[uart_port];
    tx_dma_channel = uart_port_to_dma_channel(uart_port, 0);
    rx_dma_channel = uart_port_to_dma_channel(uart_port, 1);

#ifdef HAL_SLEEP_MANAGER_ENABLED
    uart_unmask_send_interrupt(uartx);
    g_uart_frist_send_complete_interrupt[uart_port] = true;
#endif
    uart_unmask_receive_interrupt(uartx);
    NVIC_EnableIRQ(g_uart_port_to_irq_num[uart_port]);
#if 0
    status = vdma_enable_interrupt(tx_dma_channel);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return;
    }
#endif
    status = vdma_enable_interrupt(rx_dma_channel);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return;
    }
    status = vdma_start(tx_dma_channel);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return;
    }
    status = vdma_start(rx_dma_channel);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return;
    }
    uart_enable_dma(uartx);
}

hal_uart_status_t hal_uart_register_callback(hal_uart_port_t uart_port,
        hal_uart_callback_t user_callback,
        void *user_data)
{
    vdma_channel_t tx_dma_channel, rx_dma_channel;
    uint32_t irq_status;
    vdma_status_t status;
    hal_nvic_status_t nvic_status;

    if ((!uart_port_is_valid(uart_port)) ||
        (user_callback == NULL)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    irq_status = save_and_set_interrupt_mask();
    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_POLL_INITIALIZED) {
        restore_interrupt_mask(irq_status);
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }

    tx_dma_channel = uart_port_to_dma_channel(uart_port, 0);
    rx_dma_channel = uart_port_to_dma_channel(uart_port, 1);

    g_uart_callback[uart_port].func = user_callback;
    g_uart_callback[uart_port].arg = user_data;

    uart_dma_channel_to_callback_data(tx_dma_channel, &g_uart_dma_callback_data[uart_port * 2]);
    status = vdma_register_callback(tx_dma_channel, uart_dma_callback_handler, &g_uart_dma_callback_data[uart_port * 2]);
    if (status != VDMA_OK) {
        restore_interrupt_mask(irq_status);
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }
    uart_dma_channel_to_callback_data(rx_dma_channel, &g_uart_dma_callback_data[(uart_port * 2) + 1]);
    status = vdma_register_callback(rx_dma_channel, uart_dma_callback_handler, &g_uart_dma_callback_data[(uart_port * 2) + 1]);
    if (status != VDMA_OK) {
        restore_interrupt_mask(irq_status);
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }
    nvic_status = hal_nvic_register_isr_handler(g_uart_port_to_irq_num[uart_port], uart_interrupt_handler);
    if (nvic_status != HAL_NVIC_STATUS_OK) {
        restore_interrupt_mask(irq_status);
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }

    uart_start_dma_transmission(uart_port);

    g_uart_hwstatus[uart_port] = UART_HWSTATUS_DMA_INITIALIZED;

    restore_interrupt_mask(irq_status);

    return HAL_UART_STATUS_OK;
}

uint32_t hal_uart_get_available_send_space(hal_uart_port_t uart_port)
{
    vdma_channel_t channel;
    uint32_t roomleft;
    vdma_status_t status;

    if (!uart_port_is_valid(uart_port)) {
        return 0;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_DMA_INITIALIZED) {
        return 0;
    }

    channel = uart_port_to_dma_channel(uart_port, 0);
    status = vdma_get_available_send_space(channel, &roomleft);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return 0;
    }

    return roomleft;
}

uint32_t hal_uart_get_available_receive_bytes(hal_uart_port_t uart_port)
{
    vdma_channel_t channel;
    uint32_t avail;
    vdma_status_t status;

    if (!uart_port_is_valid(uart_port)) {
        return 0;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_DMA_INITIALIZED) {
        return 0;
    }

    channel = uart_port_to_dma_channel(uart_port, 1);
    status = vdma_get_available_receive_bytes(channel, &avail);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return 0;
    }

    return avail;
}

hal_uart_status_t hal_uart_set_hardware_flowcontrol(hal_uart_port_t uart_port)
{
    UART_REGISTER_T *uartx;

    if (!uart_port_is_valid(uart_port)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    if (g_uart_hwstatus[uart_port] == UART_HWSTATUS_UNINITIALIZED) {
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }

    uartx = g_uart_regbase[uart_port];

    uart_set_hardware_flowcontrol(uartx);

#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_uart_flowcontrol_status[uart_port] = HAL_UART_FLOWCONTROL_HARDWARE;
#endif

    return HAL_UART_STATUS_OK;
}

hal_uart_status_t hal_uart_set_software_flowcontrol(hal_uart_port_t uart_port,
        uint8_t xon,
        uint8_t xoff,
        uint8_t escape_character)
{
    UART_REGISTER_T *uartx;

    if (!uart_port_is_valid(uart_port)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    if (g_uart_hwstatus[uart_port] == UART_HWSTATUS_UNINITIALIZED) {
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }

    uartx = g_uart_regbase[uart_port];

    uart_set_software_flowcontrol(uartx, xon, xoff, escape_character);

#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_uart_flowcontrol_status[uart_port] = HAL_UART_FLOWCONTROL_SOFTWARE;
    g_uart_sw_flowcontrol_config[uart_port].xon = xon;
    g_uart_sw_flowcontrol_config[uart_port].xoff = xoff;
    g_uart_sw_flowcontrol_config[uart_port].escape_character = escape_character;
#endif

    return HAL_UART_STATUS_OK;
}

hal_uart_status_t hal_uart_disable_flowcontrol(hal_uart_port_t uart_port)
{
    UART_REGISTER_T *uartx;

    if (!uart_port_is_valid(uart_port)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    if (g_uart_hwstatus[uart_port] == UART_HWSTATUS_UNINITIALIZED) {
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }

    uartx = g_uart_regbase[uart_port];

    uart_disable_flowcontrol(uartx);

#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_uart_flowcontrol_status[uart_port] = HAL_UART_FLOWCONTROL_NONE;
#endif

    return HAL_UART_STATUS_OK;
}


hal_uart_status_t hal_uart_set_dma(hal_uart_port_t uart_port, const hal_uart_dma_config_t *dma_config)
{
    uint32_t irq_status;
    vdma_config_t internal_dma_config;
    vdma_channel_t tx_dma_channel, rx_dma_channel;
    vdma_status_t status;

    if (!uart_port_is_valid(uart_port)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }
    if ((dma_config->send_vfifo_buffer == NULL) ||
        (dma_config->receive_vfifo_buffer == NULL)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }
    if ((dma_config->send_vfifo_buffer_size >= UART_DMA_MAX_SETTING_VALUE) ||
        (dma_config->send_vfifo_threshold_size >= UART_DMA_MAX_SETTING_VALUE) ||
        (dma_config->send_vfifo_threshold_size > dma_config->send_vfifo_buffer_size)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }
    if ((dma_config->receive_vfifo_buffer_size >= UART_DMA_MAX_SETTING_VALUE) ||
        (dma_config->receive_vfifo_threshold_size >= UART_DMA_MAX_SETTING_VALUE) ||
        (dma_config->receive_vfifo_alert_size >= UART_DMA_MAX_SETTING_VALUE) ||
        (dma_config->receive_vfifo_threshold_size > dma_config->receive_vfifo_buffer_size) ||
        (dma_config->receive_vfifo_alert_size > dma_config->receive_vfifo_buffer_size)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_POLL_INITIALIZED) {
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }

    tx_dma_channel = uart_port_to_dma_channel(uart_port, 0);
    rx_dma_channel = uart_port_to_dma_channel(uart_port, 1);

    status = vdma_init(tx_dma_channel);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }
    internal_dma_config.base_address = (uint32_t)dma_config->send_vfifo_buffer;
    internal_dma_config.size = dma_config->send_vfifo_buffer_size;
    status = vdma_configure(tx_dma_channel, &internal_dma_config);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }
    status = vdma_set_threshold(tx_dma_channel, dma_config->send_vfifo_threshold_size);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }

    status = vdma_init(rx_dma_channel);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }
    internal_dma_config.base_address = (uint32_t)dma_config->receive_vfifo_buffer;
    internal_dma_config.size = dma_config->receive_vfifo_buffer_size;
    status = vdma_configure(rx_dma_channel, &internal_dma_config);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }
    status = vdma_set_threshold(rx_dma_channel, dma_config->receive_vfifo_threshold_size);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }
    status = vdma_set_alert_length(rx_dma_channel, dma_config->receive_vfifo_alert_size);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }

    irq_status = save_and_set_interrupt_mask();
    g_uart_dma_config[uart_port].send_vfifo_buffer = dma_config->send_vfifo_buffer;
    g_uart_dma_config[uart_port].send_vfifo_buffer_size = dma_config->send_vfifo_buffer_size;
    g_uart_dma_config[uart_port].send_vfifo_threshold_size = dma_config->send_vfifo_threshold_size;
    g_uart_dma_config[uart_port].receive_vfifo_alert_size = dma_config->receive_vfifo_alert_size;
    g_uart_dma_config[uart_port].receive_vfifo_buffer = dma_config->receive_vfifo_buffer;
    g_uart_dma_config[uart_port].receive_vfifo_buffer_size = dma_config->receive_vfifo_buffer_size;
    g_uart_dma_config[uart_port].receive_vfifo_threshold_size = dma_config->receive_vfifo_threshold_size;
    restore_interrupt_mask(irq_status);

    return HAL_UART_STATUS_OK;
}

hal_uart_status_t hal_uart_set_dma_timeout(hal_uart_port_t uart_port, uint32_t timeout)
{
    UART_REGISTER_T *uartx;

    if (!uart_port_is_valid(uart_port)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }
    if (timeout > HAL_UART_TIMEOUT_VALUE_MAX) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    uartx = g_uart_regbase[uart_port];
    uart_set_timeout_value(uartx, timeout);

    return HAL_UART_STATUS_OK;
}

hal_uart_status_t hal_uart_set_auto_baudrate(hal_uart_port_t uart_port, bool is_enable)
{
    UART_REGISTER_T *uartx;
    uint32_t current_baudrate;

    if (!uart_port_is_valid(uart_port)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    uartx = g_uart_regbase[uart_port];
    current_baudrate = g_uart_baudrate_map[g_baudrate[uart_port]];

    g_uart_recover_auto_baud[uart_port] = is_enable;
    g_uart_is_auto_baud[uart_port] = is_enable;
    uart_set_auto_baudrate(uartx, is_enable, current_baudrate);
    b_start_auto_baud[uart_port] = is_enable;
     return HAL_UART_STATUS_OK;
}

uint32_t hal_uart_get_auto_baudrate_result(hal_uart_port_t uart_port)
{
    UART_REGISTER_T *uartx;
    uartx = g_uart_regbase[uart_port];

    return uart_get_auto_baudrate_result(uartx);
}

hal_uart_status_t hal_uart_set_auto_baudrate_recover(hal_uart_port_t uart_port, bool is_enable)
{
    g_uart_recover_auto_baud[uart_port] = is_enable;

    return HAL_UART_STATUS_OK;
}

hal_uart_status_t hal_uart_send_xoff(hal_uart_port_t uart_port)
{
#ifdef HAL_SLEEP_MANAGER_ENABLED
    UART_REGISTER_T *uartx;

    if(HAL_UART_FLOWCONTROL_SOFTWARE != g_uart_flowcontrol_status[uart_port])
        return HAL_UART_STATUS_ERROR;

    uartx = g_uart_regbase[uart_port];
    uart_send_xoff(uartx);
#endif

    return HAL_UART_STATUS_OK;
}
hal_uart_status_t hal_uart_send_xon(hal_uart_port_t uart_port)
{
#ifdef HAL_SLEEP_MANAGER_ENABLED
    UART_REGISTER_T *uartx;

    if(HAL_UART_FLOWCONTROL_SOFTWARE != g_uart_flowcontrol_status[uart_port])
        return HAL_UART_STATUS_ERROR;

    uartx = g_uart_regbase[uart_port];
    uart_send_xon(uartx);
#endif

    return HAL_UART_STATUS_OK;
}

#ifdef __cplusplus
}
#endif

#endif
