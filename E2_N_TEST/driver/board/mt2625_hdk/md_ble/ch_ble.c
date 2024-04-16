#include <stdio.h> 
#include <stdarg.h>
#include <stdint.h>

#include "ril.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "task_def.h"
#include "timers.h"

#include "hal_gpio.h"
#include "hal_rtc_external.h"
#include "hal_rtc_internal.h"
#include "hal_sleep_driver.h"

#include "memory_attribute.h"

#include "tel_conn_mgr_common.h"
#include "tel_conn_mgr_app_api.h"
#include "tel_conn_mgr_bearer_iprot.h"

#include "syslog.h"
#include "mt2625_hdk_lcd.h"

#include "gdi.h"
#include "image_info.h"
#include "ril.h"
#include "hal_sleep_manager.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "at_command.h"
#include "syslog.h"

#include "hal_sleep_manager.h"
#include "hal_sleep_driver.h"
#include "n1_md_sleep_api.h"
#include "hal_spm.h"
#include "hal_pmu.h"
#include "nvdm.h"
#include "hal_rtc.h"
#include "hal_rtc_external.h"
#include "hal_clock_internal.h"
#include "hal_keypad.h"
#include "h10_mmi.h"
#include <stdlib.h>
#ifdef MTK_SWLA_ENABLE
#include "swla.h"
#endif

#include "semphr.h"
#include "hal_i2c_master.h"
#include "hal_eint.h"

#define BLE_DBG(fmt,arg...)   LOG_I(sbit_demo, "[BLE]: "fmt,##arg)


#define BLE_UART_VFIFO_SIZE         1024
#define BLE_UART_ALERT_LENGTH       0
hal_uart_port_t ble_uart = HAL_UART_1;
TimerHandle_t ble_timer = NULL;
int ble_off_on_mark=0;

ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t BLE_uart_tx_vfifo[BLE_UART_VFIFO_SIZE];
ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t BLE_uart_rx_vfifo[BLE_UART_VFIFO_SIZE];

void ble_uart_at_send(char *cmd_str)
{
    char buffer[100] = {0}; 
    int ret = 0;
	
	strcat(buffer,cmd_str);
    ret = hal_uart_send_dma(ble_uart,buffer,strlen(buffer));
    BLE_DBG("===================ble_uart_at_send:%s, ret:%d \n",buffer , ret);
}

uint32_t ble_uart_send_search(void)
{
    int ret_len,i=0;
	uint8_t buff[24]={0};
	
	//FF FF 01 00
	
	buff[i++]=0xFF;
	buff[i++]=0xFF;
	buff[i++]=0x01;
	buff[i++]=0x00;

    ret_len = hal_uart_send_dma(ble_uart,buff,4);
    BLE_DBG("ble_uart_send_search #######################ret_len:%d\r\n", ret_len);
    return ret_len;
}

uint32_t ble_uart_send_search_stop(void)
{
    int ret_len,i=0;
	uint8_t buff[24]={0};
	
	//FF FF 02 00
	
	buff[i++]=0xFF;
	buff[i++]=0xFF;
	buff[i++]=0x02;
	buff[i++]=0x00;

    ret_len = hal_uart_send_dma(ble_uart,buff,4);
    BLE_DBG("ble_uart_send_search_stop #######################ret_len:%d\r\n", ret_len);
    return ret_len;
}
uint32_t ble_uart_send_mac(void)
{
    int ret_len,i=0;
	uint8_t buff[24]={0};
	
	//FF FF 05 06 F0 14 03 31 CC EA
	
	buff[i++]=0xFF;
	buff[i++]=0xFF;
	buff[i++]=0x05;
	buff[i++]=0x06;
	buff[i++]=0xA0;
	buff[i++]=0xE6;
	buff[i++]=0xF8;
	buff[i++]=0x6D;
	buff[i++]=0x04;
	buff[i++]=0x69;
	
    ret_len = hal_uart_send_dma(ble_uart,buff,10);
    BLE_DBG("ble_uart_send_mac #######################ret_len:%d\r\n", ret_len);
	
    return ret_len;
}
uint32_t ble_uart_send_recording_start(void)
{
	int ret_len,i=0;
	uint8_t buff[24]={0};
	
	//FF FF 0A 0x01 0x01
	
	buff[i++]=0xFF;
	buff[i++]=0xFF;
	buff[i++]=0x0a;
	buff[i++]=0x01;
	buff[i++]=0x01;

	ret_len = hal_uart_send_dma(ble_uart,buff,5);
	BLE_DBG("ble_uart_send_recording_start #######################ret_len:%d\r\n", ret_len);
	return ret_len;
}

uint32_t ble_uart_send_recording_stop(void)
{
	int ret_len,i=0;
	uint8_t buff[24]={0};
	
	//FF FF 0b 0x01 0x01
	
	buff[i++]=0xFF;
	buff[i++]=0xFF;
	buff[i++]=0x0b;
	buff[i++]=0x01;
	buff[i++]=0x01;

	ret_len = hal_uart_send_dma(ble_uart,buff,5);
	BLE_DBG("ble_uart_send_recording_stop #######################ret_len:%d\r\n", ret_len);
	return ret_len;
}

uint32_t ble_uart_send_recording_read(void)
{
	int ret_len,i=0;
	uint8_t buff[24]={0};
	
	//FF FF 0c 0x01 0x01
	
	buff[i++]=0xFF;
	buff[i++]=0xFF;
	buff[i++]=0x0c;
	buff[i++]=0x01;
	buff[i++]=0x01;

	ret_len = hal_uart_send_dma(ble_uart,buff,5);
	BLE_DBG("ble_uart_send_recording_read #######################ret_len:%d\r\n", ret_len);
	return ret_len;
}

uint32_t ble_uart_send_key(void)
{
    int ret_len,i=0;
	uint8_t buff[24]={0};
	
	//FF FF 03 0A F0 14 01 00 00 00 00 03 31 CC EA 00 00 00 00 00 00 00 00 EF
	buff[i++]=0xFF;
	buff[i++]=0xFF;
	buff[i++]=0x03;
	buff[i++]=0x14;
	buff[i++]=0xF0;
	buff[i++]=0x14;
	buff[i++]=0x01;
	buff[i++]=0x00;
	buff[i++]=0x00;
	buff[i++]=0x00;
	buff[i++]=0x00;
	buff[i++]=0x03;
	buff[i++]=0x31;
	buff[i++]=0xCC;
	buff[i++]=0xEA;
	buff[i++]=0x00;
	buff[i++]=0x00;
	buff[i++]=0x00;
	buff[i++]=0x00;
	buff[i++]=0x00;
	buff[i++]=0x00;
	buff[i++]=0x00;
	buff[i++]=0x00;
	buff[i++]=0xEF;
	
    //ret_len = hal_uart_send_dma(ble_uart,buf,(uint32_t) buf_len);
    ret_len = hal_uart_send_dma(ble_uart,buff,24);
    BLE_DBG("hal_uart_send_dma #######################ret_len:%d\r\n", ret_len);
    return ret_len;
}

extern int gps_status_flag;
extern int wifi_status_flag;
extern void gps_driver_uart_irq(uint8_t gps_str[], uint32_t length);
extern void WIFI_driver_uart_irq(uint8_t wifi_str[], uint32_t length);
extern void gps_get_data(void);

void BLE_driver_uart_irq(hal_uart_callback_event_t status, void *parameter)
{
	uint8_t temp[BLE_UART_VFIFO_SIZE]={0};
	char receive[BLE_UART_VFIFO_SIZE]={0};
	uint32_t length, i;
	uint8_t buff[BLE_UART_VFIFO_SIZE]={0};
	char*pch;

	
	//gps_driver_uart_irq(status, parameter);
	
	if (HAL_UART_EVENT_READY_TO_READ == status) 
	{
		length = hal_uart_get_available_receive_bytes(ble_uart);
		hal_uart_receive_dma(ble_uart, (uint8_t*)temp, length);	
		
		BLE_DBG("BLEIRQ(%d)[%d][%d]:%s\r\n",gps_status_flag,length, strlen(temp),temp);
		
		WIFI_driver_uart_irq(temp, length);
		gps_driver_uart_irq(temp, length);
		if((strstr(temp,"GPSON+OK")!=NULL))
		{
			gps_status_flag = 1;//open
		}
		if((strstr(temp,"GPSOFF+OK")!=NULL))
		{
			gps_status_flag = 0;//close
		}
		
		if((strstr(temp,"WIFION+OK")!=NULL))
		{
			wifi_status_flag = 1;
		}
		if((strstr(temp,"WIFIOFF+OK")!=NULL))
		{
			wifi_status_flag = 0;
		}

	} 
}

extern hal_gpt_status_t H10_sleep_handler(void);
bool BLE_driver_init(void)
{
	hal_uart_config_t uart_config;
	hal_uart_dma_config_t dma_config;
	uint32_t left, snd_cnt, rcv_cnt;
	bool ret = false;
	bool deinit_ret = false;
	hal_uart_status_t uart_init_status = HAL_UART_STATUS_OK;
	BLE_DBG("<<<<<<<<<<<<<<<<<<<<<<<<<<<<BLE_driver_init:%d", ble_off_on_mark);	
    if(ble_off_on_mark==1)
    {
		return;
	}

	H10_sleep_handler();
	hal_pinmux_set_function(HAL_GPIO_10, HAL_GPIO_10_GPIO10);			  /*   set dierection to be output	*/			  
	hal_gpio_set_direction(HAL_GPIO_10, HAL_GPIO_DIRECTION_OUTPUT);
	hal_gpio_set_output(HAL_GPIO_10,HAL_GPIO_DATA_HIGH);
	
	hal_gpio_init(HAL_GPIO_13);
	hal_pinmux_set_function(HAL_GPIO_13, HAL_GPIO_12_UART1_RXD);			  	  
	hal_gpio_set_direction(HAL_GPIO_13, HAL_GPIO_DIRECTION_INPUT);
	
	hal_gpio_init(HAL_GPIO_12);
	hal_pinmux_set_function(HAL_GPIO_12, HAL_GPIO_13_UART1_TXD);			  	  
	hal_gpio_set_direction(HAL_GPIO_12, HAL_GPIO_DIRECTION_OUTPUT);

	uart_config.baudrate = HAL_UART_BAUDRATE_9600;
	uart_config.parity = HAL_UART_PARITY_NONE;
	uart_config.stop_bit = HAL_UART_STOP_BIT_1;
	uart_config.word_length = HAL_UART_WORD_LENGTH_8;
	
	//Init UART
	dma_config.receive_vfifo_buffer = BLE_uart_rx_vfifo;
	dma_config.receive_vfifo_buffer_size = BLE_UART_VFIFO_SIZE;
	dma_config.receive_vfifo_alert_size = BLE_UART_ALERT_LENGTH;
	dma_config.receive_vfifo_threshold_size = BLE_UART_VFIFO_SIZE;
	dma_config.send_vfifo_buffer = BLE_uart_tx_vfifo;
	dma_config.send_vfifo_buffer_size = BLE_UART_VFIFO_SIZE;
	dma_config.send_vfifo_threshold_size = 512;

	uart_init_status = hal_uart_init(ble_uart, &uart_config);
	if (HAL_UART_STATUS_OK == uart_init_status) 
	{
	   ret = true;
	   BLE_DBG("==============BLE init success\n\r");
	} 
	else 
	{
	   deinit_ret = hal_uart_deinit(ble_uart);
	   ret = false;
	   if(HAL_UART_STATUS_OK == deinit_ret)
	   {
		   uart_init_status = hal_uart_init(ble_uart, &uart_config);
	   }
	   if (HAL_UART_STATUS_OK == uart_init_status) 
	   {
		   ret = true;
	   }
	   BLE_DBG("==============BLE init :%d, %d, %d\n\r", uart_init_status, deinit_ret, ret);
	}

	if(ret == true)
	{
		ble_off_on_mark=1;
		hal_uart_set_dma(ble_uart, &dma_config);
		hal_uart_register_callback(ble_uart, BLE_driver_uart_irq, NULL);
	}
	return ret;
}

void BLE_power_off()
{
	BLE_DBG("<<<<<<<<<<<<<<<<<<<<<<<<<<<<BLE_power_off:%d", ble_off_on_mark);	
	if(ble_off_on_mark==0)
	{
		return;
	}
	
	if(ble_timer!=NULL)
	{
		xTimerStop(ble_timer,0);
	}
	
	ble_off_on_mark=0;
	hal_gpio_init(HAL_GPIO_12);
	hal_pinmux_set_function(HAL_GPIO_12, HAL_GPIO_12_GPIO12);				  
	hal_gpio_set_direction(HAL_GPIO_12, HAL_GPIO_DIRECTION_INPUT);

	hal_gpio_init(HAL_GPIO_13);
	hal_pinmux_set_function(HAL_GPIO_13, HAL_GPIO_13_GPIO13);				  
	hal_gpio_set_direction(HAL_GPIO_13, HAL_GPIO_DIRECTION_INPUT);
	
    if (HAL_GPIO_STATUS_OK != hal_gpio_set_direction(HAL_GPIO_10, HAL_GPIO_DIRECTION_OUTPUT)){
        BLE_DBG("GPIO29 hal_gpio_set_direction fail");
        return;
    }
    if (HAL_GPIO_STATUS_OK != hal_gpio_set_output(HAL_GPIO_10, HAL_GPIO_DATA_LOW)){
        BLE_DBG("GPIO29 hal_gpio_set_output fail");
        return;
    }
    if (HAL_EINT_STATUS_OK != hal_eint_deinit(HAL_GPIO_10)) {
        BLE_DBG("GPIO29 deinit fail");
    }
}

