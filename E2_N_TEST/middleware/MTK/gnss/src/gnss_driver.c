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

#include <string.h>
#include <stdio.h>
#include "gnss_driver.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "hal_uart.h"
#include "hal_gpio.h"
#include "gnss_log.h"
#include "memory_attribute.h"
#include "hal_eint.h"
#if defined(MTK_PORT_SERVICE_ENABLE)
#include "serial_port.h"
#include "nb_custom_port.h"
static uint32_t gnss_serial_port_handle;
#endif

#define USING_CYCLE_BUFFER
#ifdef USING_CYCLE_BUFFER
#define RX_BUFF_SIZE 9216
#endif
#define MAX_CMD_LEN             16
#define MAX_OPT_LEN             128
#define MAX_DATA_LEN            (MAX_CMD_LEN + MAX_OPT_LEN)
#define RAW_DATA_QUEUE_SIZE     6
#define RESPONSE_BUFFER_SIZE    256
#define HANDLER_TASK_SIZE       512
#define HANDLER_TASK_PRIORITY   1
#define UART_IRQ_PRIORITY       configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
#ifdef USING_CYCLE_BUFFER
#define UART_VFIFO_SIZE         512
#define UART_THRESHOLD          496
#else
#define UART_VFIFO_SIZE         5120
#define UART_THRESHOLD          3000
#endif
#define UART_ALERT_LENGTH       0
#define UART_TIMEOUT_TIME       16000
#define UART_BAUDRATE           115200


typedef struct {
    size_t length;
    char data[MAX_DATA_LEN];
}data_t;

typedef struct cyclical_buffer     // Cyclical buffer
{
    char *next_write;     // next position to write to
    char *next_read;      // next position to read from
    char *start_buffer;   // start of buffer
    char *end_buffer;     // end of buffer + 1
    int32_t buffer_size;
} cyclical_buffer_t;

ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t g_uart_tx_vfifo[UART_VFIFO_SIZE];
ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t g_uart_rx_vfifo[UART_VFIFO_SIZE];
#ifdef USING_CYCLE_BUFFER
static int32_t notify_flag = 1;
static char rx_buffer[RX_BUFF_SIZE];
static cyclical_buffer_t g_cyclical_buffer;    // io - cyclic buffer
int32_t debug_put_data_size = 0;
int32_t debug_read_data_size = 0;
#endif
static gnss_driver_uart_callback gnss_callback_function;

int32_t gnss_output_data = 1;
#define GNSS_DEEP_SLEEP_HANDLER
#ifdef GNSS_DEEP_SLEEP_HANDLER
uint8_t deep_sleep_handler = 0xFF;
#endif

void buffer_initialize
                ( cyclical_buffer_t *buffer,           // buffer to initialize
                  unsigned    buffer_size )     // size of buffer to create
{
   // Set up buffer manipulation pointers
   // end_buffer points to the next byte after the buffer
   buffer->end_buffer   = buffer->start_buffer + buffer_size;
   buffer->next_read    = buffer->start_buffer;
   buffer->next_write   = buffer->start_buffer;
   buffer->buffer_size = buffer_size;
   return;
}

#if defined(MTK_PORT_SERVICE_ENABLE)
void gnss_serial_port_callback(serial_port_dev_t device, serial_port_callback_event_t event, void *parameter)
{
    uint8_t temp[UART_VFIFO_SIZE];
    uint32_t length, i;
    switch(event)
    {
        GNSSLOGD("gnss_serial_port_callback, event = %d", event);
        case SERIAL_PORT_EVENT_READY_TO_READ:
        {
#ifdef USING_CYCLE_BUFFER
            serial_port_read_data_t read_data;
            serial_port_status_t status = SERIAL_PORT_STATUS_FAIL;
            uint32_t data_len = 0;

            length = hal_uart_get_available_receive_bytes(gnss_uart);
            read_data.buffer = (uint8_t*)temp;
            read_data.size = length;
            status = serial_port_control(gnss_serial_port_handle, SERIAL_PORT_CMD_READ_DATA, (serial_port_ctrl_para_t*)&read_data);
            if (status == SERIAL_PORT_STATUS_OK) {
                length = read_data.ret_size;
            }

            debug_put_data_size += length;
            if (gnss_output_data) {
                if ((g_cyclical_buffer.next_write == g_cyclical_buffer.next_read) && (length > 0)) {
                    gnss_callback_function(GNSS_UART_CALLBACK_TYPE_CAN_READ);
                } else if ((g_cyclical_buffer.next_write > g_cyclical_buffer.next_read
                            && ((g_cyclical_buffer.next_write - g_cyclical_buffer.next_read) > (g_cyclical_buffer.buffer_size << 1)))
                            || (g_cyclical_buffer.next_read > g_cyclical_buffer.next_write
                            && ((g_cyclical_buffer.next_read - g_cyclical_buffer.next_write) < (g_cyclical_buffer.buffer_size << 1)))) {
                    if (notify_flag == 1) {
                        notify_flag = 0;
                        gnss_callback_function(GNSS_UART_CALLBACK_TYPE_CAN_READ);
                    }
                }

                for (i = 0; i < length; i++)
                {
                    *(g_cyclical_buffer.next_write++) = temp[i];

                    if ( g_cyclical_buffer.next_write >= g_cyclical_buffer.end_buffer )
                    {
                        g_cyclical_buffer.next_write = g_cyclical_buffer.start_buffer;
                    }
                }
            }
#else
            gnss_callback_function(GNSS_UART_CALLBACK_TYPE_CAN_READ);
#endif
            break;
        }

        case SERIAL_PORT_EVENT_READY_TO_WRITE:
        {
            gnss_callback_function(GNSS_UART_CALLBACK_TYPE_CAN_WRITE);
            break;
        }

        case SERIAL_PORT_EVENT_USB_CONNECTION:
        {
            break;
        }
        case SERIAL_PORT_EVENT_USB_DISCONNECTION:
        {
            break;
        }

        default:
            break;
   }

}
#else
void gnss_driver_uart_irq(hal_uart_callback_event_t status, void *parameter)
{
    uint8_t temp[UART_VFIFO_SIZE];
    uint32_t length, i;

    if (HAL_UART_EVENT_READY_TO_READ == status) {
        #ifdef USING_CYCLE_BUFFER
        length = hal_uart_get_available_receive_bytes(gnss_uart);
        hal_uart_receive_dma(gnss_uart, temp, length);
        debug_put_data_size += length;
        if (gnss_output_data) {
            if ((g_cyclical_buffer.next_write == g_cyclical_buffer.next_read) && (length > 0)) {
                gnss_callback_function(GNSS_UART_CALLBACK_TYPE_CAN_READ);
            } else if ((g_cyclical_buffer.next_write > g_cyclical_buffer.next_read 
                        && ((g_cyclical_buffer.next_write - g_cyclical_buffer.next_read) > (g_cyclical_buffer.buffer_size << 1)))
                        || (g_cyclical_buffer.next_read > g_cyclical_buffer.next_write 
                        && ((g_cyclical_buffer.next_read - g_cyclical_buffer.next_write) < (g_cyclical_buffer.buffer_size << 1)))) {
                if (notify_flag == 1) {
                    notify_flag = 0;
                    gnss_callback_function(GNSS_UART_CALLBACK_TYPE_CAN_READ);
                }
            }

            for (i = 0; i < length; i++)
            {
                *(g_cyclical_buffer.next_write++) = temp[i];

                if ( g_cyclical_buffer.next_write >= g_cyclical_buffer.end_buffer )
                {
                    g_cyclical_buffer.next_write = g_cyclical_buffer.start_buffer;
                }
            }
        }
        #else
        gnss_callback_function(GNSS_UART_CALLBACK_TYPE_CAN_READ);
        #endif
    } else if (HAL_UART_EVENT_READY_TO_WRITE == status) {
        gnss_callback_function(GNSS_UART_CALLBACK_TYPE_CAN_WRITE);
    }
}
#endif

void gnss_driver_eint_irq(void *parameter)
{
    if (!gnss_callback_function) {
        return;
    }
    hal_eint_mask(gnss_eint);
    gnss_callback_function(GNSS_UART_CALLBACK_TYPE_WAKEUP);
}


uint8_t gnss_driver_init(gnss_driver_uart_callback callback_function)
{
    hal_uart_config_t uart_config;
    hal_uart_dma_config_t dma_config;
    static bool is_uart_init = false;

    GNSSLOGD("gnss_driver_init\n\r");
    cust_gnss_gpio_config();

    gnss_callback_function = callback_function;
    #ifdef USING_CYCLE_BUFFER
    g_cyclical_buffer.start_buffer = (char *)&rx_buffer[0];
    buffer_initialize(&g_cyclical_buffer, sizeof(rx_buffer));
    #endif

    if (is_uart_init){
        return 0;
    }

#if defined(MTK_PORT_SERVICE_ENABLE)
    serial_port_dev_t          saved_port;
    serial_port_setting_uart_t uart_setting;
    serial_port_open_para_t    serial_port_open_para;
    serial_port_dev_t          port;
    serial_port_status_t       PortStatus = SERIAL_PORT_STATUS_FAIL;
    char                      *port_name = "gnss";

    port =  nb_custom_get_default_port( PORT_USER_GPS );
    serial_port_open_para.callback = gnss_serial_port_callback;

    if( SERIAL_PORT_STATUS_OK == serial_port_config_read_dev_number(port_name, &saved_port) )
    {
        if(SERIAL_PORT_TYPE_UART == serial_port_get_device_type(saved_port))
        {
            hal_uart_deinit(saved_port);
            PortStatus = serial_port_config_read_dev_setting(
                saved_port, (serial_port_dev_setting_t*)&uart_setting);

            if(SERIAL_PORT_STATUS_OK != PortStatus)
            {
                if( SERIAL_PORT_STATUS_OK != PortStatus ) {
                    uart_setting.baudrate = nb_custom_get_default_baud_rate(PORT_USER_GPS);
                }

                uart_setting.is_baud_rate_set = FALSE;
                uart_setting.flow_control     = HAL_UART_FLOWCONTROL_NONE;
                uart_setting.dma_config_flag = TRUE;
                uart_setting.port_service_dma_config.receive_vfifo_alert_size     = UART_ALERT_LENGTH;
                uart_setting.port_service_dma_config.receive_vfifo_buffer         = g_uart_rx_vfifo;
                uart_setting.port_service_dma_config.receive_vfifo_buffer_size    = UART_VFIFO_SIZE;
                uart_setting.port_service_dma_config.receive_vfifo_threshold_size = 200;
                uart_setting.port_service_dma_config.send_vfifo_buffer            = g_uart_tx_vfifo;
                uart_setting.port_service_dma_config.send_vfifo_buffer_size       = UART_VFIFO_SIZE;
                uart_setting.port_service_dma_config.send_vfifo_threshold_size    = 8;

                serial_port_config_write_dev_setting(saved_port, (serial_port_dev_setting_t*)&uart_setting);
            }
        }
    }
    else
    {
        hal_uart_deinit(port);
        serial_port_config_write_dev_number(port_name, port);

        uart_setting.baudrate = nb_custom_get_default_baud_rate(PORT_USER_GPS);

        // initial value
        uart_setting.dma_config_flag = TRUE;
        uart_setting.is_baud_rate_set = FALSE;
        uart_setting.flow_control     = HAL_UART_FLOWCONTROL_NONE;
        uart_setting.port_service_dma_config.receive_vfifo_alert_size     = UART_ALERT_LENGTH;
        uart_setting.port_service_dma_config.receive_vfifo_buffer         = g_uart_rx_vfifo;
        uart_setting.port_service_dma_config.receive_vfifo_buffer_size    = UART_VFIFO_SIZE;
        uart_setting.port_service_dma_config.receive_vfifo_threshold_size = 200;
        uart_setting.port_service_dma_config.send_vfifo_buffer            = g_uart_tx_vfifo;
        uart_setting.port_service_dma_config.send_vfifo_buffer_size       = UART_VFIFO_SIZE;
        uart_setting.port_service_dma_config.send_vfifo_threshold_size    = 8;

        serial_port_config_write_dev_setting(port, (serial_port_dev_setting_t*)&uart_setting);
        saved_port = port;
    }

    PortStatus = serial_port_open(saved_port, &serial_port_open_para, &gnss_serial_port_handle);
    if (SERIAL_PORT_STATUS_OK == PortStatus )
    {
        GNSSLOGD("serial port init success");
        is_uart_init = true;
    } else {
       GNSSLOGE("serial port init fail, PortStatus = %d", PortStatus);
    }
#else 
    uart_config.baudrate = HAL_UART_BAUDRATE_115200;
    uart_config.word_length = HAL_UART_WORD_LENGTH_8;
    uart_config.stop_bit = HAL_UART_STOP_BIT_1;
    uart_config.parity = HAL_UART_PARITY_NONE;
    //Init UART
    dma_config.receive_vfifo_buffer = g_uart_rx_vfifo;
    dma_config.receive_vfifo_buffer_size = UART_VFIFO_SIZE;
    dma_config.receive_vfifo_alert_size = UART_ALERT_LENGTH;
    dma_config.receive_vfifo_threshold_size = 200;
    dma_config.send_vfifo_buffer = g_uart_tx_vfifo;
    dma_config.send_vfifo_buffer_size = UART_VFIFO_SIZE;
    dma_config.send_vfifo_threshold_size = 8;

    if (HAL_UART_STATUS_OK == hal_uart_init(gnss_uart, &uart_config)) {
       GNSSLOGD("uart init success\n\r");
       is_uart_init = true;
    } else {
       GNSSLOGE("uart init fail\n\r");
    }


    hal_uart_set_dma(gnss_uart, &dma_config);
    hal_uart_register_callback(gnss_uart, gnss_driver_uart_irq, NULL);
#endif

    GNSSLOGD("gnss_driver_init done\n\r");

    return 0;
}

int32_t gnss_driver_uart_read(uint8_t port, int8_t *buffer, int32_t length)
{
#ifdef USING_CYCLE_BUFFER
    // calculate total send size
    char *next_write;
    int32_t u4_send_size, i;

    next_write = g_cyclical_buffer.next_write;

    if (g_cyclical_buffer.next_read == next_write)
    {
        return 0;
    }

    if (g_cyclical_buffer.next_read < next_write)
    {
        u4_send_size = (unsigned int)next_write - (unsigned int)g_cyclical_buffer.next_read;
    }
    else
    {
        u4_send_size = (unsigned int)g_cyclical_buffer.end_buffer - (unsigned int)g_cyclical_buffer.next_read +
                     (unsigned int)next_write - (unsigned int)g_cyclical_buffer.start_buffer;
    }

    if (u4_send_size > length )
    {
        u4_send_size = length;
    }

    for (i = 0; i < u4_send_size; i++)
    {
        buffer[i] = *(g_cyclical_buffer.next_read++);

        // Wrap check output circular buffer
        if ( g_cyclical_buffer.next_read >= g_cyclical_buffer.end_buffer )
        {
            g_cyclical_buffer.next_read = g_cyclical_buffer.start_buffer;
        }
    }

    if ((g_cyclical_buffer.next_write > g_cyclical_buffer.next_read 
        && ((g_cyclical_buffer.next_write - g_cyclical_buffer.next_read) < (g_cyclical_buffer.buffer_size << 4)))
        || (g_cyclical_buffer.next_read > g_cyclical_buffer.next_write 
        && ((g_cyclical_buffer.next_read - g_cyclical_buffer.next_write) > (g_cyclical_buffer.buffer_size - (g_cyclical_buffer.buffer_size << 4))))) {
        notify_flag = 1;
    }
    debug_read_data_size += u4_send_size;
    return u4_send_size;
#else
#if defined(MTK_PORT_SERVICE_ENABLE)
    serial_port_read_data_t read_data;
    serial_port_status_t status = SERIAL_PORT_STATUS_FAIL;
    uint32_t data_len = 0;

    read_data.buffer = (uint8_t*)buffer;
    read_data.size = length;
    status = serial_port_control(gnss_serial_port_handle, SERIAL_PORT_CMD_READ_DATA, (serial_port_ctrl_para_t*)&read_data);
    if (status == SERIAL_PORT_STATUS_OK) {
        data_len = read_data.ret_size;
    }
    return data_len;
#else
    return hal_uart_receive_dma(gnss_uart, (uint8_t *)buffer, (uint32_t)length);
#endif
#endif
}

int32_t gnss_driver_uart_write(uint8_t port, int8_t *buffer, int32_t length)
{
    int ret_len = 0;
#if defined(MTK_PORT_SERVICE_ENABLE)
    serial_port_write_data_t send_data;
    serial_port_status_t status = SERIAL_PORT_STATUS_OK;

    send_data.data = buffer;
    send_data.size = length;
    status = serial_port_control(gnss_serial_port_handle, SERIAL_PORT_CMD_WRITE_DATA, (serial_port_ctrl_para_t*)&send_data);
    if (status == SERIAL_PORT_STATUS_OK)
    {
        ret_len = send_data.ret_size;
    }
#else
    ret_len = hal_uart_send_dma(gnss_uart, (uint8_t *)buffer, (uint32_t)length);
#endif
    return ret_len;
}

//Reserved
void gnss_driver_uart_deinit(uint8_t port)
{
    return;
}

void gnss_driver_power_on()
{
    hal_eint_config_t eint_config;

    //Init cycle buffer
    #ifdef USING_CYCLE_BUFFER
    g_cyclical_buffer.start_buffer = (char *)&rx_buffer[0];
    buffer_initialize(&g_cyclical_buffer, sizeof(rx_buffer));
    #endif

    eint_config.debounce_time = 1;
    eint_config.trigger_mode = HAL_EINT_EDGE_RISING;
#ifdef HAL_EINT_FEATURE_FIRQ
    eint_config.firq_enable = false;
#endif    
    if (HAL_EINT_STATUS_OK  != hal_eint_init(gnss_eint,  &eint_config))
    {
        GNSSLOGD("ENIT10 hal_eint_init fail");
    } else {
        hal_eint_register_callback(gnss_eint, gnss_driver_eint_irq, NULL);
        hal_eint_unmask(gnss_eint);
    }

    //Pull high LDO_EN Pin
    if (HAL_GPIO_STATUS_OK != hal_gpio_set_direction(gnss_ldo_en, HAL_GPIO_DIRECTION_OUTPUT)) {
        GNSSLOGD("GPIO10 hal_gpio_set_direction fail");
        return;
    }

    if (HAL_GPIO_STATUS_OK != hal_gpio_set_output(gnss_ldo_en, HAL_GPIO_DATA_HIGH)){
        GNSSLOGD("GPIO10 hal_gpio_set_output fail");
        return;
    }
    //Set 32K clock
    #if 0
    hal_pinmux_set_function(gnss_clock_pin, HAL_GPIO_13_CLKO3);
    if (HAL_GPIO_STATUS_OK != hal_gpio_set_clockout(gnss_32k_clock, HAL_GPIO_CLOCK_MODE_32K)){
        GNSSLOGD("GPIO13 hal_gpio_set_clockout fail");
        return;
    }
    #endif
    gnss_driver_init_coclock();
    // lock deep sleep
#ifdef GNSS_DEEP_SLEEP_HANDLER
    if (deep_sleep_handler == 0xFF)
        deep_sleep_handler = hal_sleep_manager_set_sleep_handle("GDSH");
    hal_sleep_manager_acquire_sleeplock(deep_sleep_handler, HAL_SLEEP_LOCK_DEEP);
#endif
    return;
}

void gnss_driver_power_off()
{
    if (HAL_GPIO_STATUS_OK != hal_gpio_set_direction(gnss_ldo_en, HAL_GPIO_DIRECTION_OUTPUT)){
        GNSSLOGD("GPIO10 hal_gpio_set_direction fail");
        return;
    }

    if (HAL_GPIO_STATUS_OK != hal_gpio_set_output(gnss_ldo_en, HAL_GPIO_DATA_LOW)){
        GNSSLOGD("GPIO10 hal_gpio_set_output fail");
        return;
    }
    if (HAL_EINT_STATUS_OK != hal_eint_deinit(gnss_eint)) {
        GNSSLOGD("ENIT10 deinit fail");
    }
    gnss_driver_deinit_coclock();
    // release deep sleep
#ifdef GNSS_DEEP_SLEEP_HANDLER
    hal_sleep_manager_release_sleeplock(deep_sleep_handler, HAL_SLEEP_LOCK_DEEP);
#endif
}


