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

#define WIFI_DBG(fmt,arg...)   LOG_I(sbit_demo, "[wifi]: "fmt,##arg)

int wifi_send=0;
#define WIFI_UART_VFIFO_SIZE         512
#define WIFI_UART_ALERT_LENGTH       0
hal_uart_port_t wifi_uart = HAL_UART_1;
TimerHandle_t wifi_timer = NULL;
TimerHandle_t RST_timer = NULL;
int uart_init_success = 0;

 

ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t WIFI_uart_tx_vfifo[WIFI_UART_VFIFO_SIZE];
ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t WIFI_uart_rx_vfifo[WIFI_UART_VFIFO_SIZE];
void WIFI_power_off();
void Wifi_At_Send(void)
{
    char buffer[100]; 
    int ret_len,i=0;
	
	memset(buffer,0,sizeof(buffer));
	if(wifi_send==1)
	sprintf(buffer,"AT+CWLAP\r\n");
	else
	sprintf(buffer,"AT+CWMODE_CUR=1\r\n");
    ret_len = hal_uart_send_dma(wifi_uart,buffer,strlen(buffer));
    WIFI_DBG("<<<<<<<<<<<< Wifi_At_Send	>>>>>>>>>>>> %s , ret_len %d \n",buffer , ret_len);
	
}

void Wifi_At_RST(void)
{
    char buffer[100]; 
    int ret_len,i=0;
	memset(buffer,0,sizeof(buffer));
	sprintf(buffer,"AT+RST\r\n");
    ret_len = hal_uart_send_dma(wifi_uart,buffer,strlen(buffer));
    WIFI_DBG("<<<<<<<<<<<< Wifi_At_RST	>>>>>>>>>>>> %s , ret_len %d \n",buffer , ret_len);
}

void WIFI_driver_uart_irq(hal_uart_callback_event_t status, void *parameter)
{
	uint8_t temp[WIFI_UART_VFIFO_SIZE]={0};
	uint8_t tmp_buf[128]={0};
 	uint32_t length, i,j;
 	char*pch;

	if (HAL_UART_EVENT_READY_TO_READ == status) 
	{
		length = hal_uart_get_available_receive_bytes(wifi_uart);
		hal_uart_receive_dma(wifi_uart, (uint8_t*)temp, length);	
		//if(((length == 9)||(strncmp(temp,"ready",5)==0))&&(wifi_send == 0))
		if((length == 9)||(strstr(temp,"ready") != NULL))
		{
			wifi_send = 0;
			uart_init_success = 1;
		}
		
		temp_info.Hw_Version = 1;
		
 		WIFI_DBG(" wifi uart_init_success	 %d : \r\n" ,uart_init_success);
 		WIFI_DBG(" wifi length	 %d : , %d : \r\n" ,length,temp_info.Hw_Version);
		
		i=((strlen(temp))/100)+1;
		
		for(j=0;j<i;j++)
		{
			memset(tmp_buf,0,sizeof(tmp_buf));
			strncpy(tmp_buf,temp+(100*j),100);
			WIFI_DBG(" wifi tmp_buf   %s : \r\n",tmp_buf );
 		}

        if((strncmp(temp,"AT+CWMODE_CUR=1",strlen("AT+CWMODE_CUR=1")) == 0)&&(wifi_send==0))
        {
			wifi_send = 1;
			Wifi_At_Send();
		} 
		//else if(((strncmp(temp,"AT+CWLAP",8)==0)||(((strpbrk (temp,"&"))!=NULL)&&((strpbrk (temp,"@"))!=NULL)))&&(temp_info.wifi_wlap_success==0))
		else if(((strncmp(temp,"AT+CWLAP",8)==0))&&(temp_info.wifi_wlap_success==0))
		{
			//wifi_send = 1;
			WIFI_DBG("############################ AT+CWLAP ############################\n");
			//Wifi_At_Send();
		}
		else if((strncmp(temp,"+CWLAP",6)==0) &&(temp_info.wifi_wlap_success==0))
		{
			memset(temp_info.total_buffer_wlap,0,sizeof(temp_info.total_buffer_wlap));
			strcat(temp_info.total_buffer_wlap,temp);
			WIFI_DBG(" temp_info.total_buffer_wlap1111  %s : \r\n",temp_info.total_buffer_wlap );
		    if((strpbrk (temp,"OK"))!=NULL)
			temp_info.wifi_wlap_success = 1;
  		}
		else if(strlen(temp_info.total_buffer_wlap) >= 10)
		{
			if(((strlen(temp_info.total_buffer_wlap))+length) < (sizeof(temp_info.total_buffer_wlap)))
			strcat(temp_info.total_buffer_wlap,temp);
		    if((strpbrk (temp,"OK"))!=NULL)
			temp_info.wifi_wlap_success = 1;
			WIFI_DBG(" temp_info.total_buffer_wlap2222  %s : \r\n",temp_info.total_buffer_wlap );
		}
		
 		WIFI_DBG(" temp_info.wifi_wlap_success	 %d : \r\n" ,temp_info.wifi_wlap_success);
		
	} 
	
}

uint8_t wifi_deep_sleep_handler = 0xFF;
#if defined(MTK_H10S_SUPPORT) 
extern void BLE_power_off();
TimerHandle_t WIFI_delay_off_timer=NULL;
TimerHandle_t WIFI_driver_init_timer=NULL;
#endif
void WIFI_driver_init(void)
{

	if(temp_info.Hw_Version == 2)
	{
		if(temp_info.wifi_off_on_flag >= 3)
		{
			if(WIFI_driver_init_timer!=NULL)
			xTimerStop(WIFI_driver_init_timer,0);
			
			if(temp_info.fm_mode == 0)
			{
				if(WIFI_delay_off_timer == NULL)
				WIFI_delay_off_timer = xTimerCreate("WIFI_delay_off", 
				30*1000 / portTICK_PERIOD_MS,  /* 30??1?¡À? wifi */
				pdFALSE, 
				NULL,
				BLE_power_off);
				xTimerStart(WIFI_delay_off_timer,0); 
			}
			return;
		}
		
		if(temp_info.wifi_off_on_flag == 0)
		{
			BLE_power_off();	
		    memset(temp_info.total_buffer_wlap,0,sizeof(temp_info.total_buffer_wlap));
			WIFI_DBG("<<<<<<<<<<<<<<<<<<<<<<<<< BLE_power_off >>>>>>>>>>>>>>>>>>>>>>>>>> \n\r");
			temp_info.wifi_off_on_flag = 1;
		}
		else if(temp_info.wifi_off_on_flag == 1)
		{
			BLE_driver_init();	
			WIFI_DBG("<<<<<<<<<<<<<<<<<<<<<<<<< BLE_driver_init >>>>>>>>>>>>>>>>>>>>>>>>>> \n\r");
			temp_info.wifi_off_on_flag = 2;
		}
		else if(temp_info.wifi_off_on_flag == 2)
		{
			ble_uart_at_send("WIFI+OPEN\r\n");
			WIFI_DBG("<<<<<<<<<<<<<<<<<<<<<<<<< WIFI+OPEN >>>>>>>>>>>>>>>>>>>>>>>>>> \n\r");
			temp_info.wifi_off_on_flag = 3;
		}

		if(WIFI_driver_init_timer == NULL)
	    WIFI_driver_init_timer = xTimerCreate("wifi_init_timer", 
		2000 / portTICK_PERIOD_MS, 
		pdTRUE, 
		NULL,
		WIFI_driver_init);
	    xTimerStart(WIFI_driver_init_timer,0); 

	}
	else
	{
		hal_uart_config_t uart_config;
		hal_uart_dma_config_t dma_config;
		uint32_t left, snd_cnt, rcv_cnt;
		hal_uart_status_t ret = HAL_UART_STATUS_OK;
		
		if(temp_info.wifi_off_on_flag==1)
		{
		return;
		}
		
		hal_uart_deinit(wifi_uart);
		wifi_send = 0;
		temp_info.wifi_wlap_success = 0;
		temp_info.wifi_off_on_flag = 1;
		uart_init_success = 0;
		memset(temp_info.total_buffer_wlap,0,sizeof(temp_info.total_buffer_wlap));
		
		hal_pinmux_set_function(WIFI_EN, HAL_GPIO_10_GPIO10);			  /*   set dierection to be output	*/			  
		hal_gpio_set_direction(WIFI_EN, HAL_GPIO_DIRECTION_OUTPUT);
		hal_gpio_set_output(WIFI_EN,HAL_GPIO_DATA_HIGH);
		
		hal_gpio_init(WIFI_RX);
		hal_pinmux_set_function(WIFI_RX, HAL_GPIO_12_UART1_RXD);				  
		hal_gpio_set_direction(WIFI_RX, HAL_GPIO_DIRECTION_INPUT);
		
		hal_gpio_init(WIFI_TX);
		hal_pinmux_set_function(WIFI_TX, HAL_GPIO_13_UART1_TXD);				  
		hal_gpio_set_direction(WIFI_TX, HAL_GPIO_DIRECTION_OUTPUT);
		
		
		vTaskDelay(2);
		//9
		hal_pinmux_set_function(WIFI_CH, HAL_GPIO_9_GPIO9); 				  
		hal_gpio_set_direction(WIFI_CH, HAL_GPIO_DIRECTION_OUTPUT);
		hal_gpio_set_output(WIFI_CH,HAL_GPIO_DATA_HIGH);
		
		vTaskDelay(2);
		// 11
		hal_pinmux_set_function(WIFI_WORK_EN, HAL_GPIO_11_GPIO11);			 /* ??D?*/			  
		hal_gpio_set_direction(WIFI_WORK_EN, HAL_GPIO_DIRECTION_OUTPUT);
		hal_gpio_set_output(WIFI_WORK_EN,HAL_GPIO_DATA_LOW);
		
		vTaskDelay(20);
		
		hal_gpio_set_output(WIFI_WORK_EN,HAL_GPIO_DATA_HIGH);
		
		
		uart_config.baudrate = HAL_UART_BAUDRATE_115200;
		uart_config.parity = HAL_UART_PARITY_NONE;
		uart_config.stop_bit = HAL_UART_STOP_BIT_1;
		uart_config.word_length = HAL_UART_WORD_LENGTH_8;
		
		//Init UART
		dma_config.receive_vfifo_buffer = WIFI_uart_rx_vfifo;
		dma_config.receive_vfifo_buffer_size = WIFI_UART_VFIFO_SIZE;
		dma_config.receive_vfifo_alert_size = WIFI_UART_ALERT_LENGTH;
		dma_config.receive_vfifo_threshold_size = 200;
		dma_config.send_vfifo_buffer = WIFI_uart_tx_vfifo;
		dma_config.send_vfifo_buffer_size = WIFI_UART_VFIFO_SIZE;
		dma_config.send_vfifo_threshold_size = 8;
		
		ret = hal_uart_init(wifi_uart, &uart_config);
		if (HAL_UART_STATUS_OK == ret) 
		{
		   WIFI_DBG("[ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ]uart init success\n\r");
		} 
		else 
		{
		   WIFI_DBG("[ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ]uart init fail:%d\n\r", ret);
		}
		
		hal_uart_set_dma(wifi_uart, &dma_config);
		hal_uart_register_callback(wifi_uart, WIFI_driver_uart_irq, NULL);
		
		
		if(wifi_timer == NULL)
		wifi_timer = xTimerCreate("wifi_timer",    /* ??¡ä?WIFI¡Á?3¡è?a???¡§??30?? jerry */
		20*1000 / portTICK_PERIOD_MS, 
		pdTRUE, 
		(void *) 0, 
		WIFI_power_off);
		xTimerStart(wifi_timer,1);
		
		
		if(RST_timer == NULL)
		RST_timer = xTimerCreate("RST_timer",	 /* ?¦Ì¨ª3?¡ä?? jerry */
		100 / portTICK_PERIOD_MS, 
		pdFALSE, 
		(void *) 0, 
		Wifi_At_RST);
		xTimerStart(RST_timer,1);
		if (wifi_deep_sleep_handler == 0xFF)
		wifi_deep_sleep_handler = hal_sleep_manager_set_sleep_handle("WIFI");
		hal_sleep_manager_acquire_sleeplock(wifi_deep_sleep_handler, HAL_SLEEP_LOCK_ALL);

	}

}

void WIFI_power_off(void)
{
	
	if(temp_info.wifi_off_on_flag==0)
	return;
	
	temp_info.wifi_off_on_flag=0;
	WIFI_DBG("<<<<<<<<<<<<<<<<<<<<<<<<<<<<WIFI_power_off>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	
	if(temp_info.Hw_Version == 2)
	{
		if(WIFI_driver_init_timer!=NULL)
		xTimerStop(WIFI_driver_init_timer,0);
	}
	else
	{
		if(wifi_timer!=NULL)
		xTimerStop(wifi_timer,0);
		wifi_send = 0;
		uart_init_success = 0;
			
		hal_gpio_init(WIFI_RX);
		hal_pinmux_set_function(WIFI_RX, HAL_GPIO_12_GPIO12);				  
		hal_gpio_set_direction(WIFI_RX, HAL_GPIO_DIRECTION_OUTPUT);
		hal_gpio_set_output(WIFI_RX, HAL_GPIO_DATA_LOW);
		
		hal_gpio_init(WIFI_TX);
		hal_pinmux_set_function(WIFI_TX, HAL_GPIO_12_GPIO12);				  
		hal_gpio_set_direction(WIFI_TX, HAL_GPIO_DIRECTION_OUTPUT);
		hal_gpio_set_output(WIFI_TX, HAL_GPIO_DATA_LOW);
		
		hal_pinmux_set_function(WIFI_CH, HAL_GPIO_9_GPIO9); 				  
		hal_gpio_set_direction(WIFI_CH, HAL_GPIO_DIRECTION_OUTPUT);
		hal_gpio_set_output(WIFI_CH,HAL_GPIO_DATA_LOW);
		
		hal_pinmux_set_function(WIFI_WORK_EN, HAL_GPIO_11_GPIO11);			 /* ??D?*/			  
		hal_gpio_set_direction(WIFI_WORK_EN, HAL_GPIO_DIRECTION_OUTPUT);
		hal_gpio_set_output(WIFI_WORK_EN,HAL_GPIO_DATA_LOW);
		
		if (HAL_GPIO_STATUS_OK != hal_gpio_set_direction(WIFI_EN, HAL_GPIO_DIRECTION_OUTPUT))
		{
			WIFI_DBG("GPIO10 hal_gpio_set_direction fail");
			return;
		}
		if (HAL_GPIO_STATUS_OK != hal_gpio_set_output(WIFI_EN, HAL_GPIO_DATA_LOW))
		{
			WIFI_DBG("GPIO10 hal_gpio_set_output fail");
			return;
		}
		
		hal_sleep_manager_release_sleeplock(wifi_deep_sleep_handler, HAL_SLEEP_LOCK_ALL);

	}
		//gps_driver_init();
	if((strlen(temp_info.total_buffer_wlap) == 0)&&
		((temp_info.wifi_timing_flag == 1)||
		(temp_info.sos_mark == 1)))
	{
	    if((temp_info.wifi_repeat_init_flag == 0)||
			(temp_info.wifi_repeat_init_flag == 1)||
			(temp_info.wifi_repeat_init_flag == 2))
	    {
			temp_info.wifi_repeat_init_flag ++;
			vTaskDelay(100);
			WIFI_driver_init();
			return;
		}
		else
		{
		    if(temp_info.wifi_timing_flag == 0)
		    {
				gps_driver_init();
			}
			else				
			temp_info.wifi_repeat_init_flag = 0;
		}

	}
	else if(strlen(temp_info.total_buffer_wlap) != 0)
	{
		//if(temp_info.fm_mode == 0)
	   // gps_power_off();
		//sbit_set_vib_tips(2,2); 
	}
	
	temp_info.sos_mark = 0;
	
	
}

