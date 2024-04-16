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

#define WIFI_DBG(fmt,arg...)   LOG_I(sbit_demo, "[sbit]: "fmt,##arg)

int wifi_send=0;
#define WIFI_UART_VFIFO_SIZE         512
#define WIFI_UART_ALERT_LENGTH       0
hal_uart_port_t wifi_uart = HAL_UART_1;
TimerHandle_t wifi_timer = NULL;
int uart_init_success = 0;
bool wifi_rest_timer_flag = false;
 

ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t WIFI_uart_tx_vfifo[WIFI_UART_VFIFO_SIZE];
ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t WIFI_uart_rx_vfifo[WIFI_UART_VFIFO_SIZE];

extern int wifi_status_flag;
#define AP_NUM 3 
char wifi_str[128] = {0};
//char wifi_str[512] = {0}; AP_NUM 10
void WIFI_driver_uart_irq(uint8_t search_str[], uint32_t length)
{

	char *p0 = NULL;
	char *p1 = NULL;
	char *ap_p0 = NULL;
	char *ap_p1 = NULL;
	char ap_str[128] = {0};
	char mac_str[64] = {0};
	char signal_str[64] = {0};
	int i = 0;
	if((search_str == NULL) || (length <= 0))
	{
		return;
	}
	
	if((strstr(search_str,"CWLAP") == NULL)
		||(strstr(search_str,"$GPRMC") != NULL)
		||(strstr(search_str,"$GNRMC") != NULL)
		||(strstr(search_str,"$GNGGA") != NULL)
		||(strstr(search_str,"$GPTXT") != NULL)
	)
	{
		return;
	}
	else
	{
		WIFI_DBG("WIFIIRQ(%d)[%d][%d]:%s\r\n",wifi_status_flag,length, strlen(search_str),search_str);
		if(strlen(search_str) > 100)
		{
			WIFI_DBG("WIFIIRQ0(%d)[%d][%d]:%s\r\n",wifi_status_flag,length, strlen(search_str),&search_str[100]);
		}
		if(strlen(search_str) > 200)
		{
			WIFI_DBG("WIFIIRQ1(%d)[%d][%d]:%s\r\n",wifi_status_flag,length, strlen(search_str),&search_str[200]);
		}
	}
	p0 = search_str;
	p1 = search_str;

	p0 = strstr(search_str,"CWLAP");
	for(i = 0; i < AP_NUM; i++)
	{
		p0 = strstr(p0,"(");
		if(p0 != NULL)
		{
			p1 = strstr(p0,")");
		}
		if((p0 == NULL) || (p1 == NULL))
		{
			break;
		}
		memset(ap_str, 0, sizeof(ap_str));
		memcpy(ap_str,(p0+1),(p1 - p0 - 1));

		ap_p0 = strstr(ap_str,":");
		ap_p1 = strstr(ap_str,"-");
		if(ap_p1 != NULL)
		{
			if(('0' > ap_p1[1]) || ('9' < ap_p1[1]))
			{
				ap_p1++;
				ap_p1 = strstr(ap_p1,"-");
			}
		}
		if((ap_p0 == NULL) || (ap_p1 == NULL))
		{
			break;
		}
		memset(mac_str, 0, sizeof(mac_str));
		memset(signal_str, 0, sizeof(signal_str));
		memcpy(mac_str, ap_p0-2,strlen("a4:29:40:3c:a0:1e"));
		memcpy(signal_str, ap_p1,strlen("-76"));
		if(i == 0)
		{
			memset(wifi_str, 0, sizeof(wifi_str));
		}
		strcat(wifi_str, mac_str);
		strcat(wifi_str, "/");
		strcat(wifi_str, signal_str);
		if(i == 0)
		{
			strcat(wifi_str, ",");
		}
		else
		{
			strcat(wifi_str, "|");
		}
		WIFI_DBG("WIFIIRQ wifi_str:%s\r\n",wifi_str);
		p0++;
		p1++;
		temp_info.wifi_wlap_success = 1;
	}
}

void WIFI_driver_init(void)
{
	bool ble_init_flag = false;
	WIFI_DBG("==============BLE WIFI_driver_init:[%d, %d]\r\n",temp_info.wifi_off_on_flag, temp_info.gps_off_on_flag);
    if((temp_info.wifi_off_on_flag == 1) || (temp_info.gps_off_on_flag == 1))
    {
		return;
	}
	
	while(ble_init_flag == false)
	{
		BLE_power_off();
		vTaskDelay(20);
		ble_init_flag = BLE_driver_init();	
		vTaskDelay(120);
	}
	WIFI_DBG("==============BLE WIFI_driver_init:end");
	ble_uart_at_send("WIFI+OPEN\r\n");
	temp_info.wifi_off_on_flag = 1;
	temp_info.wifi_wlap_success = 0;
	if(wifi_timer == NULL)
	{
		wifi_timer = xTimerCreate("wifi_timer",15*1000/portTICK_PERIOD_MS, pdTRUE, (void *)0, WIFI_power_off);
	}
	xTimerStart(wifi_timer,1);
}

void WIFI_power_off(void)
{
	ble_uart_at_send("WIFI+CLOSE\r\n");
	temp_info.wifi_off_on_flag = 0;
	if(wifi_timer != NULL)
	{
		xTimerStop(wifi_timer,1);
	}
#if defined(MD_DEBUG_SUPPORT)
	gps_driver_init();
#else
#if defined(FUNCTION_OFF_SUPPORT)
	if(NVRAM_info.gps_off_flag == true)
	{
		memset(temp_info.MY_Latitude,0,sizeof(temp_info.MY_Latitude));
		memset(temp_info.MY_Longitude,0,sizeof(temp_info.MY_Longitude));
		BLE_power_off();
	}
	else 
#endif		
	if(temp_info.wifi_wlap_success == 0)
	{
		gps_driver_init();
	}
	else
	{
		BLE_power_off();
	}
#endif	
}

void wifi_reset_timer(void)
{
	if((wifi_timer != NULL) && (wifi_rest_timer_flag == true) && (temp_info.wifi_off_on_flag == 1) && (temp_info.wifi_wlap_success == 1))
	{
		xTimerStop(wifi_timer,1);
		xTimerChangePeriod(wifi_timer, 1*1000/portTICK_PERIOD_MS, 0);
		xTimerStart(wifi_timer,1);
		wifi_rest_timer_flag = false;
		WIFI_DBG("================================wifi_reset_timer\r\n");
	}
	else
	{
		//WIFI_DBG("================================wifi_reset_timer::[%d, %d], [%d, %d]\r\n", (wifi_timer != NULL), (wifi_rest_timer_flag == true), temp_info.wifi_off_on_flag, temp_info.wifi_wlap_success);
	}
}

