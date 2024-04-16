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


long long change_num(char *str, int length)
{
   char revstr[16] = {0}; //根据十六进制字符串的长度，这里注意数组不要越界
   long long num[16] = {0};
   long long count = 1;
   long long result = 0;

   strcpy(revstr, str);

   for(int i = length - 1; i >= 0; i--)
   {
      if((revstr[i] >= '0') && (revstr[i] <= '9'))
      {
         num[i] = revstr[i] - 48;   //字符0的ASCII值为48
      }
      else if((revstr[i] >= 'a') && (revstr[i] <= 'f'))
      {
         num[i] = revstr[i] - 'a' + 10;
      }
      else if((revstr[i] >= 'A') && (revstr[i] <= 'F'))
      {
         num[i] = revstr[i] - 'A' + 10;
      }
      else
      {
         num[i] = 0;
      }

      result += num[i] * count;
      count *= 16; //十六进制(如果是八进制就在这里乘以8)
   }

   return result;
}


#define BLE_DBG(fmt,arg...)   LOG_I(sbit_demo, "[sbit]: "fmt,##arg)


#define BLE_UART_VFIFO_SIZE         1200
#define BLE_UART_ALERT_LENGTH       0
hal_uart_port_t ble_uart = HAL_UART_1;
TimerHandle_t ble_timer = NULL;

ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t BLE_uart_tx_vfifo[BLE_UART_VFIFO_SIZE];
ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t BLE_uart_rx_vfifo[BLE_UART_VFIFO_SIZE];

#define MTK_BLE_SCHOOL
#ifdef MTK_BLE_SCHOOL
int current_strongest_index = 0;
int ble_scand_num = 0;
int signal_index_last = (-1);
int current_ble_index = 0;
int previous_ble_index = -1;
int door_ble_prev_index = 0;
int door_ble_curr_index = 0;
int door_in_out_flag = 0;
BLE_DEVICE BLE_DEVICE_ARRAY[BLE_NUM_MAX] = {0};
BLE_DEVICE MY_BLE_DEVICE_ARRAY[BLE_NUM_MAX] = 
{
    {"c9:35:3d:d0:c9:15",0,0},
	{"3d:19:16:e:b0:89",0,1},
	{"57:b5:6b:5d:d2:b",0,2},
	{"d5:c5:57:39:de:c1",0,3},
	{"0:f8:8:8:5f:c1",0,4},
	{"d6:45:37:9c:54:82",0,5},
	{"3:a2:10:27:45:c1",0,6},
	{"da:45:1f:5d:55:4d",0,7},
	{"6e:89:63:a8:dc:e4",0,8},
	{"6c:c3:39:f7:60:17",0,9},
	{"3:45:d0:2e:1b:39",0,10},
	{"28:e5:5f:1d:50:73",0,11},
	{"0:4d:0:6:b1:a0",0,12},
	{"d2:c7:2f:d0:8b:b8",0,13},
	{"1c:4b:75:94:df:87",0,14},
	{"ea:8f:65:70:30:52",0,15},
	{"5:f:50:78:2e:6e",0,16},
	{"2:d3:c0:3b:f8:7a",0,17},
	{"18:40:8c:f8:3e:9c",0,18},
	{"5:63:88:7d:88:b5",0,19},
	{"3:30:60:2a:e3:f",0,20},
	{"4e:77:3a:cd:d9:8b",0,21},
	{"6:a5:c0:5f:a6:bf",0,22},
	{"2:ec:48:39:fb:4b",0,23},
	{"47:4d:3a:16:8d:4b",0,24},
	{"18:fd:25:3b:9f:b",0,25},
	{"6:a3:18:5f:fd:24",0,26},
	{"7:e2:38:41:ce:1",0,27},
	{"63:ba:17:fd:54:2d",0,28}, //my here
	{"68:58:26:7c:61:ab",0,29},
	{"7b:27:b0:39:e:c0",0,30},
	{"0:e1:70:18:3c:85",0,31},
	{"7:3b:c0:4a:85:7e",0,32},
	{"5:61:f8:7d:bc:7b",0,33},
	{"1:40:c0:1e:22:19",0,34},
	{"d4:9f:4f:06:fb:5b",0,35},
	{"4b:2b:12:d2:d0:55",0,36},
	{"5:b4:0:76:56:84",0,37},
	{"6:40:10:56:c9:85",0,38},
	{"7:75:b8:4c:18:f1",0,39},
	{"7:bd:10:46:ce:24",0,40},
	{"2:7a:d8:34:34:19",0,41},
	{"2:5d:60:37:76:ae",0,42},
	{"c9:11:36:90:5b:7c",0,43},
	{"3:63:28:2d:47:a6",0,44},
	{"FF:FF:FF:FF:FF:FF",0,45},
	{"FF:FF:FF:FF:FF:FF",0,46},
	{"FF:FF:FF:FF:FF:FF",0,47},
	{"FF:FF:FF:FF:FF:FF",0,48},
	{"FF:FF:FF:FF:FF:FF",0,49},
};
#define BLE_FILTER_SIGNAL_VALUE	(-88)
BLE_DEVICE_FILTER BLE_DEVICE_FILTER_ARRAY[BLE_NUM_MAX] = 
{
    {"c9:35:3d:d0:c9:15",-94},
	{"3d:19:16:e:b0:89",-94},
	{"57:b5:6b:5d:d2:b",-80},
	{"d5:c5:57:39:de:c1",BLE_FILTER_SIGNAL_VALUE},
	{"0:f8:8:8:5f:c1",BLE_FILTER_SIGNAL_VALUE},
	{"d6:45:37:9c:54:82",-82},
	{"3:a2:10:27:45:c1",-82},
	{"da:45:1f:5d:55:4d",-73},
	{"6e:89:63:a8:dc:e4",-84}, 
	{"6c:c3:39:f7:60:17",BLE_FILTER_SIGNAL_VALUE},
	{"3:45:d0:2e:1b:39",-76},
	{"28:e5:5f:1d:50:73",BLE_FILTER_SIGNAL_VALUE},
	{"0:4d:0:6:b1:a0",-82},
	{"d2:c7:2f:d0:8b:b8",BLE_FILTER_SIGNAL_VALUE},
	{"1c:4b:75:94:df:87",BLE_FILTER_SIGNAL_VALUE},
	{"ea:8f:65:70:30:52",BLE_FILTER_SIGNAL_VALUE},
	{"5:f:50:78:2e:6e",BLE_FILTER_SIGNAL_VALUE},
	{"2:d3:c0:3b:f8:7a",BLE_FILTER_SIGNAL_VALUE},
	{"18:40:8c:f8:3e:9c",-86},
	{"5:63:88:7d:88:b5",-92},
	{"3:30:60:2a:e3:f",-82},
	{"4e:77:3a:cd:d9:8b",-80},
	{"6:a5:c0:5f:a6:bf",-86},
	{"2:ec:48:39:fb:4b",-80},
	{"47:4d:3a:16:8d:4b",-86},
	{"18:fd:25:3b:9f:b",BLE_FILTER_SIGNAL_VALUE},
	{"6:a3:18:5f:fd:24",BLE_FILTER_SIGNAL_VALUE},
	{"7:e2:38:41:ce:1",BLE_FILTER_SIGNAL_VALUE},
	{"63:ba:17:fd:54:2d",BLE_FILTER_SIGNAL_VALUE}, //my here
	{"68:58:26:7c:61:ab",-50},
	{"7b:27:b0:39:e:c0",BLE_FILTER_SIGNAL_VALUE},
	{"0:e1:70:18:3c:85",-50},
	{"7:3b:c0:4a:85:7e",-50},
	{"5:61:f8:7d:bc:7b",BLE_FILTER_SIGNAL_VALUE},
	{"1:40:c0:1e:22:19",-50},
	{"d4:9f:4f:06:fb:5b",BLE_FILTER_SIGNAL_VALUE},
	{"4b:2b:12:d2:d0:55",BLE_FILTER_SIGNAL_VALUE},
	{"5:b4:0:76:56:84",-50},
	{"6:40:10:56:c9:85",BLE_FILTER_SIGNAL_VALUE},
	{"7:75:b8:4c:18:f1",BLE_FILTER_SIGNAL_VALUE},
	{"7:bd:10:46:ce:24",BLE_FILTER_SIGNAL_VALUE},
	{"2:7a:d8:34:34:19",BLE_FILTER_SIGNAL_VALUE},
	{"2:5d:60:37:76:ae",BLE_FILTER_SIGNAL_VALUE},
	{"c9:11:36:90:5b:7c",BLE_FILTER_SIGNAL_VALUE},
	{"3:63:28:2d:47:a6",-60},
	{"FF:FF:FF:FF:FF:FF",BLE_FILTER_SIGNAL_VALUE},
	{"FF:FF:FF:FF:FF:FF",BLE_FILTER_SIGNAL_VALUE},
	{"FF:FF:FF:FF:FF:FF",BLE_FILTER_SIGNAL_VALUE},
	{"FF:FF:FF:FF:FF:FF",BLE_FILTER_SIGNAL_VALUE},
	{"FF:FF:FF:FF:FF:FF",BLE_FILTER_SIGNAL_VALUE},
};
#define BLE_STEPS_GROUP_NAME    "BLE_STEPS_CFG"
#define BLE_STEPS_ITEM_NAME    "BLE_STEPS_NVRAM"
void ble_mac_nvram_read(void)
{
    nvdm_status_t nvdm_status;
    uint32_t size = (sizeof(BLE_DEVICE))*BLE_NUM_MAX;
	int temp=0,i=0; 
	//FF FF 01 00
	//血压计 ：发扫描 然后发送 ff ff 04 00 0a 0d
	//血糖仪 ：发扫描 然后发送 ff ff 07 00 0a 0d
	
	memset(&MY_BLE_DEVICE_ARRAY,0,size);
	nvdm_status = nvdm_read_data_item(BLE_STEPS_GROUP_NAME, BLE_STEPS_ITEM_NAME,(uint8_t *)&MY_BLE_DEVICE_ARRAY, &size);

	if (nvdm_status != NVDM_MODEM_STATUS_OK)
	{
		BLE_DBG("===============================================================::ble_mac_nvram_read : error");
	}
	else
	{
		BLE_DBG("===============================================================::ble_mac_nvram_read : succeed");
	}
	
	BLE_DBG("========== MY_BLE_DEVICE_ARRAY[0].mac_str========= %s \r\n",MY_BLE_DEVICE_ARRAY[0].mac_str);
	
}

void ble_mac_nvram_write(void)
{
    nvdm_status_t nvdm_status;
    uint32_t size = (sizeof(BLE_DEVICE))*BLE_NUM_MAX;
	
	//FF FF 02 00
	nvdm_status = nvdm_write_data_item(BLE_STEPS_GROUP_NAME,
	BLE_STEPS_ITEM_NAME,
	NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
	(uint8_t *)&MY_BLE_DEVICE_ARRAY,
	size);
	
	if (nvdm_status != NVDM_MODEM_STATUS_OK)
	{
		BLE_DBG("===============================================================::ble_mac_nvram_write : error");
	}
	else
	{
		BLE_DBG("===============================================================::ble_mac_nvram_write : succeed");
	}
	
}

bool is_my_ble(BLE_DEVICE ble_dev)
{
	return true;
	
}

int get_ble_device_num(void)
{
	int num = 0;
	int i = 0;

	for(i = 0; i < BLE_NUM_MAX; i++)
	{
		if(strlen(BLE_DEVICE_ARRAY[i].mac_str) != 0)
		{
			num += 1;
		}
	}
	BLE_DBG("============================:get_ble_device_num==(%d)===[%s](%d)\r\n",num);
	return num;
}
int get_ble_device_index(void)
{
	int ret = 0;
	int i = 0;
	for(i = 0; i < BLE_NUM_MAX; i++)
	{
		if(strlen(BLE_DEVICE_ARRAY[i].mac_str) == 0)
		{
			ret = i;
		}
	}

	return ret;
}
int get_my_ble_device_index(void)
{
	int ret = 0;
	int i = 0;

	for(i = 0; i < BLE_NUM_MAX; i++)
	{
		if(strlen(MY_BLE_DEVICE_ARRAY[i].mac_str) == 0)
	//if(1)
	{
			ret = i;
			break;
		}
	}

	return ret;
}
void add_ble_by_index(BLE_DEVICE *ble_dev_p, int index)
	{
	if((index >= 0) && (index < BLE_NUM_MAX))
		{
		memset(&BLE_DEVICE_ARRAY[index], 0, sizeof(BLE_DEVICE_ARRAY[index]));
		memcpy(BLE_DEVICE_ARRAY[index].mac_str, 0, ble_dev_p->mac_str);
		BLE_DEVICE_ARRAY[index].signal_db_value = ble_dev_p->signal_db_value;
		BLE_DEVICE_ARRAY[index].index = ble_dev_p->index;
	}
}
			
void add_ble(BLE_DEVICE *ble_dev_p)
{
	int index = 0;
			
	index = get_ble_device_index();
	add_ble_by_index(ble_dev_p, index);
}
			
int get_my_ble_device_index_by_mac(char *mac_str)
{
	int ret = -1;
	int i = 0;
	char *p = NULL;
			
	for(i = 0; i < BLE_NUM_MAX; i++)
	{
		p = strstr(MY_BLE_DEVICE_ARRAY[i].mac_str, mac_str);
		if(p != NULL)
		{
			ret = i;
			break;
		}
	}

	return ret;
}
void clear_my_ble_device_index(int index)
{
	if((index < BLE_NUM_MAX) && (index >= 0))
	{
		memset(&MY_BLE_DEVICE_ARRAY[index], 0, sizeof(BLE_DEVICE));
	}
}

int get_strongest_signal(void) /* get 信号最强 */
{
	int i = 0;
	int num = 0;
	int signal_db_value = SIGNAL_MIN;
	int ret = 0;
	
	num = BLE_NUM_MAX;
	for(i = 0; i < num; i++)
	{
		if((BLE_DEVICE_ARRAY[i].signal_db_value != 0) && (BLE_DEVICE_ARRAY[i].signal_db_value > signal_db_value))
		{
			signal_db_value = BLE_DEVICE_ARRAY[i].signal_db_value;
	}
	}
	if(signal_db_value != SIGNAL_MIN)
	{
	//NVRAM_info.ble_mac
		ret = signal_db_value;
	}
	return ret;

}

int get_deputy_signal_index(void) /* get 信号第二强 */
{
	int i = 0;
	int num = 0;
	int signal_db_value = SIGNAL_MIN;
	int ret = (-1);
	
	num = BLE_NUM_MAX;

	//ff ff 04 00 0a 0d
	for(i = 0; i < num; i++)
	{
		if((BLE_DEVICE_ARRAY[i].signal_db_value != 0) && (BLE_DEVICE_ARRAY[i].signal_db_value > signal_db_value)
		&&((get_strongest_signal_index())!=i))
		{
			signal_db_value = BLE_DEVICE_ARRAY[i].signal_db_value;
		}
	}
	
	for(i = 0; (signal_db_value < 0)&&(i < num); i++)
	{
		if(BLE_DEVICE_ARRAY[i].signal_db_value == signal_db_value)
		{
			ret = BLE_DEVICE_ARRAY[i].index;
		}
	}
	
	return ret;
	
}

int get_strongest_signal_index_last(void) /* 上一次最强信号终端编码 */
{
	return signal_index_last + 1;
}
int get_strongest_signal_index(void)
{
	int i = 0;
	int num = 0;
	int index = -1;
	static int index_last = (-1); 
	int signal_db_value = SIGNAL_MIN;
	num = BLE_NUM_MAX;
	signal_db_value = get_strongest_signal();
	for(i = 0; (signal_db_value < 0)&&(i < num); i++)
{
		if(BLE_DEVICE_ARRAY[i].signal_db_value == signal_db_value)
		{
			index = BLE_DEVICE_ARRAY[i].index;
			if(index_last != index)
			{
			    if(index_last >= 0)
			    {
					signal_index_last = index_last;
			    }
				index_last = index;
			}
		}
	}
	if((index == 0) && (door_in_out_flag == 0))
	{
		BLE_DBG("===========================get_strongest_signal_index:1[%d, %d]", door_ble_prev_index, door_ble_curr_index);
		if((door_ble_prev_index == 0) && (door_ble_curr_index == 0))
		{
			door_ble_prev_index = 1;
		}
		else if((door_ble_prev_index == 0) && (door_ble_curr_index == 1))
		{
			door_ble_prev_index = 2;
		}
	}
	else if((index == 1) && (door_in_out_flag == 0))
	{
		BLE_DBG("===========================get_strongest_signal_index:2[%d, %d]", door_ble_prev_index, door_ble_curr_index);
		if((door_ble_prev_index == 1) && (door_ble_curr_index == 0))
		{
			door_ble_curr_index = 2;
		}
		else if((door_ble_prev_index == 0) && (door_ble_curr_index == 0))
		{
			door_ble_curr_index = 1;
		}
	}
	else if(index > 1)
	{
		door_ble_prev_index = 0;
		door_ble_curr_index = 0;
		door_in_out_flag = 0;
		BLE_DBG("===========================get_strongest_signal_index:door_ble_prev_index000");
	}
	if((door_in_out_flag == 0) && (door_ble_prev_index == 1) && (door_ble_curr_index == 2))
	{
		door_in_out_flag = 1;
		BLE_DBG("===========================get_strongest_signal_index:door_in_out_flag = 1");
	}
	else if((door_in_out_flag == 0) && (door_ble_prev_index == 2) && (door_ble_curr_index == 1))
	{
		door_in_out_flag = 2;
		BLE_DBG("===========================get_strongest_signal_index:door_in_out_flag = 2");
	}
	return index;
	
}
int move_direction(void)
{
	int index = 0;
	int ret = 0;
	
	index = get_strongest_signal_index();
	if(current_strongest_index > index)
	{
		ret = 1;
	}
	else if(current_strongest_index < index) 
	{
		ret = -1;
	}
	current_strongest_index = index;
	return ret;
}

int get_students_flag(void)  /* 学生校门进出状态 ，1为进入学校，2为离开学校 */
{
	static int shake_flag = 0;
	int flag = 0;
	
	current_ble_index = get_strongest_signal_index();
	BLE_DBG("===========================get_students_flag:%d, (%d, %d)", current_ble_index, BLE_DEVICE_ARRAY[0].signal_db_value, BLE_DEVICE_ARRAY[1].signal_db_value);
	if((current_ble_index != 0) && (current_ble_index != 1))
	{
	    shake_flag = 0;
		previous_ble_index = -1;
		return 0;
	}
	else if(previous_ble_index == -1)
	{
		previous_ble_index = current_ble_index;
	}
		
	if((previous_ble_index == 0) && (current_ble_index == 1))
	{
	    if(shake_flag >= 3)
	    {
			flag = 2;
	    }
		shake_flag ++;
	}
	else if((previous_ble_index == 1) && (current_ble_index == 0))
	{
	    if(shake_flag >= 3)
	    {
			flag = 1;
	    }
		shake_flag ++;
	}
	
	BLE_DBG("===========================get_students_flag:%d,[%d]{%d}", current_ble_index, flag, previous_ble_index);
	return flag;
	
}

#endif
void ble_uart_at_send(char *cmd_str)
{
    char buffer[100] = {0}; 
    int ret = 0;
	
	strcat(buffer,cmd_str);
    ret = hal_uart_send_dma(ble_uart,buffer,strlen(buffer));
	
    BLE_DBG("===================ble_uart_at_send:%s, ret:%d \n",buffer , ret);
	
}

int ble_School_flag = 0; /* 放学后连续30分钟没有搜到合法的mac地址判断离校 */
int cumulative_num = 0;  
uint8_t curr_rct_sec = 0;
static int gps_flag = 0;
int ble_flag = 0;
void BLE_driver_uart_irq(hal_uart_callback_event_t status, void *parameter)
{
	uint8_t temp[BLE_UART_VFIFO_SIZE]={0};
	uint32_t length, i,j;
	uint8_t buff[BLE_UART_VFIFO_SIZE]={0};
	char*pch;
#ifdef MTK_BLE_SCHOOL
	hal_rtc_time_t curtime;	
	int ble_i = 0;
	char ble_db_value[4] = {0};
	int my_ble_device_num = 0;
	static int ble_state[BLE_NUM_MAX] = {0};
	uint8_t tmp_buf[128]={0};
	my_ble_device_num = BLE_NUM_MAX;
#endif

	ble_flag = 0;

	
	
	hal_rtc_get_time(&curtime);//获取rtctime
	
	if (HAL_UART_EVENT_READY_TO_READ == status) 
	{
		length = hal_uart_get_available_receive_bytes(ble_uart);
		hal_uart_receive_dma(ble_uart, (uint8_t*)temp, length);	
		

		//BLE_DBG("######################### temp length ############################ : %d \r\n",strlen(temp));
		//连接上设备：FF  FF 04 01 01
		if((strstr(temp,"+CWLAP:")!=NULL)&&(temp_info.wifi_off_on_flag > 0))
		{
		    temp_info.wifi_wlap_success = 1;
			i=((strlen(temp))/100)+1;
			for(j=0;j<i;j++)
			{
				memset(tmp_buf,0,sizeof(tmp_buf));
				strncpy(tmp_buf,temp+(100*j),100);
				BLE_DBG(" wifi tmp_buf   %s : \r\n",tmp_buf );
			}
			memset(temp_info.total_buffer_wlap,0,sizeof(temp_info.total_buffer_wlap));
			strncpy(temp_info.total_buffer_wlap,temp,1024);
 		}
		else if((temp_info.gps_off_on_flag > 0)&&(strstr(temp,",A,")!=NULL))
		{	
		    /*
			  $GNGGA,123134.000,2233.68576,N,11353.47947,E,1,07,4.5,34.7,M,-3.7,M,,*6D
			  $GPGSV,3,1,12,01,08,183,,07,17,32 
			  $GNGGA,103012.000,2233.68222,N,11353.48454,E,1,11,2.9,24.6,M,-3.7,M,,*61
			  $GNGGA,110044.028,,,,,0,00,25.5,,,,,,*70
			*/
			gps_flag ++;
			BLE_DBG("################# GPS OK #################\r\n");
			if((strstr(temp,"$GNGGA,")!=NULL)&&(gps_flag >= 10))
			{
				temp_info.Valid_status = 1;
				if((strstr(temp,",N,"))!=NULL)
				{
					pch=strstr(temp,",N," );
					*pch++='\0';
					*pch++='\0';
					*pch++='\0';
					memset(buff,0,sizeof(buff));
					strcpy(buff,temp);
					memset(temp_info.MY_Latitude,0,sizeof(temp_info.MY_Latitude));
					strncpy(temp_info.MY_Latitude,buff+((strlen(buff))-10),10);
					memset(buff,0,sizeof(buff));
					strcpy(buff,pch);						
					BLE_DBG("################# temp_info.MY_Latitude ################# : %s \r\n",temp_info.MY_Latitude);
					if((strstr(buff,",E,"))!=NULL)
					{
						pch=strstr(buff,",E," );
						*pch++='\0';
						*pch++='\0';
						*pch++='\0';
						memset(temp_info.MY_Longitude,0,sizeof(temp_info.MY_Longitude));
						strncpy(temp_info.MY_Longitude,buff,20);
						BLE_DBG("################# temp_info.MY_Longitude ################# : %s \r\n",temp_info.MY_Longitude);
					}
				}
				
			}
		}
		else if((temp_info.BLE_search_off_on_flag > 0)&&(strstr(temp,"MAC:")!=NULL))
		{
#ifdef MTK_BLE_SCHOOL
			ble_scand_num = 0;
		
			for(i = 0; i < my_ble_device_num; i++)
			{
				if(curr_rct_sec != curtime.rtc_sec)
				{
					ble_state[i]++;
					if(i == (my_ble_device_num - 1))
					{
						curr_rct_sec = curtime.rtc_sec;
					}
				}
				memset(ble_db_value, 0, sizeof(ble_db_value));
				if(((strstr(temp,MY_BLE_DEVICE_ARRAY[i].mac_str))!=NULL) && (strlen(MY_BLE_DEVICE_ARRAY[i].mac_str) > 10))
				{
					pch=strstr(temp,MY_BLE_DEVICE_ARRAY[i].mac_str);
					*pch++='\0';
					memset(buff,0,sizeof(buff));
					strcpy(buff,pch);
					memset(&BLE_DEVICE_ARRAY[i], 0, sizeof(BLE_DEVICE));
					strcpy(BLE_DEVICE_ARRAY[i].mac_str,MY_BLE_DEVICE_ARRAY[i].mac_str);
					BLE_DEVICE_ARRAY[i].index = i;
					if((strstr(buff,"-"))!=NULL)
					{
						pch=strstr(buff,"-" );
						*pch++='\0';
						strncpy(ble_db_value,pch,2);
						BLE_DEVICE_ARRAY[i].signal_db_value = atoi(ble_db_value)*(-1);
						BLE_DBG("========:uart_irq==(%d)===[%s](%d)\r\n",BLE_DEVICE_ARRAY[i].index, BLE_DEVICE_ARRAY[i].mac_str, BLE_DEVICE_ARRAY[i].signal_db_value);
						ble_scand_num++;
						if(BLE_DEVICE_ARRAY[i].signal_db_value < BLE_DEVICE_FILTER_ARRAY[i].signal_db_value)
						{
							memset(&BLE_DEVICE_ARRAY[i], 0, sizeof(BLE_DEVICE));
						}
		
					}
					else
					{
	} 
					ble_School_flag = 0; 
					ble_state[i] = 0;
				}
	
				if(ble_state[i] >= 20)
				{
					memset(&BLE_DEVICE_ARRAY[i], 0, sizeof(BLE_DEVICE));
				}
				else
				{
				}
			}
			for(i = 0; i < my_ble_device_num; i++)
			{
			    if(strlen(BLE_DEVICE_ARRAY[i].mac_str) > 0)
			    {
			    }
			}
#endif	
		    temp_info.BLE_search_success = 1;
			hal_gpio_init(HAL_GPIO_13);
			hal_pinmux_set_function(HAL_GPIO_13, HAL_GPIO_13_GPIO13);				  
			hal_gpio_set_direction(HAL_GPIO_13, HAL_GPIO_DIRECTION_INPUT);
			cumulative_num ++;
			BLE_DBG("################# temp ################# : %d , %s \r\n",cumulative_num,temp);
		}
		else
		{
			BLE_DBG("################# temp #################:%s \r\n",temp);
		}
	} 
	
}

TimerHandle_t BLE_search_init_timer=NULL;
void BLE_search_send(void)
{
	ble_uart_at_send(("BT+SEARCH\r\n"));
}
void BLE_search_init(void)
{
	if(temp_info.BLE_search_off_on_flag == 1)
	return;
	
	gps_flag = 0;
	BLE_DBG("################################## BLE_search_init ##################################\r\n");
	BLE_power_off();	
	BLE_driver_init();	
	temp_info.BLE_search_off_on_flag = 1;

	if(BLE_search_init_timer == NULL)
    BLE_search_init_timer = xTimerCreate("BLE_search_init_timer", 
	500 / portTICK_PERIOD_MS, 
	pdFALSE, 
	NULL,
	BLE_search_send);
    xTimerStart(BLE_search_init_timer,0); 
	
}

void BLE_search_off(void)
{
	if(temp_info.BLE_search_off_on_flag == 0)
	return;
	BLE_DBG("################################## BLE_search_off ##################################\r\n");
	BLE_power_off();
}
uint8_t ble_deep_sleep_handler = 0xFF;
extern TimerHandle_t WIFI_delay_off_timer;
void BLE_driver_init(void)
{
	hal_uart_config_t uart_config;
	hal_uart_dma_config_t dma_config;
	uint32_t left, snd_cnt, rcv_cnt;

    if(temp_info.ble_off_on_flag==1)
	return;
	
	ble_flag ++;
	if(WIFI_delay_off_timer!=NULL)
	xTimerStop(WIFI_delay_off_timer,0);
	
	hal_uart_deinit(ble_uart);
	temp_info.ble_off_on_flag=1;
	hal_pinmux_set_function(HAL_GPIO_10, HAL_GPIO_10_GPIO10);			  /*   set dierection to be output	*/			  
	hal_gpio_set_direction(HAL_GPIO_10, HAL_GPIO_DIRECTION_OUTPUT);
	hal_gpio_set_output(HAL_GPIO_10,HAL_GPIO_DATA_HIGH);

	hal_gpio_init(HAL_GPIO_12);
	hal_pinmux_set_function(HAL_GPIO_12, HAL_GPIO_12_UART1_RXD);				  
	hal_gpio_set_direction(HAL_GPIO_12, HAL_GPIO_DIRECTION_INPUT);

	hal_gpio_init(HAL_GPIO_13);
	hal_pinmux_set_function(HAL_GPIO_13, HAL_GPIO_13_UART1_TXD);				  
	hal_gpio_set_direction(HAL_GPIO_13, HAL_GPIO_DIRECTION_OUTPUT);

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
	dma_config.send_vfifo_threshold_size = BLE_UART_VFIFO_SIZE;

	if (HAL_UART_STATUS_OK == hal_uart_init(ble_uart, &uart_config)) 
	{
	   BLE_DBG("[ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ]ble uart init success\n\r");
	} 
	else 
	{
	   BLE_DBG("[ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ]ble uart init fail\n\r");
	}

	hal_uart_set_dma(ble_uart, &dma_config);
	hal_uart_register_callback(ble_uart, BLE_driver_uart_irq, NULL);
    if (ble_deep_sleep_handler == 0xFF)
    ble_deep_sleep_handler = hal_sleep_manager_set_sleep_handle("BLE");
    hal_sleep_manager_acquire_sleeplock(ble_deep_sleep_handler, HAL_SLEEP_LOCK_ALL);
			
}

void BLE_power_off()
{
	if(temp_info.ble_off_on_flag==0)
	return;
	
	BLE_DBG("<<<<<<<<<<<<<<<<<<<<<<<<<<<<BLE_power_off>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");	
	
	if(WIFI_delay_off_timer!=NULL)
	xTimerStop(WIFI_delay_off_timer,0);

	if(temp_info.BLE_search_off_on_flag == 1)
	{
	}
	temp_info.ble_send_flag = 0;
	temp_info.ble_off_on_flag=0;
	temp_info.gps_off_on_flag = 0;
	temp_info.wifi_off_on_flag = 0;
	temp_info.BLE_search_off_on_flag = 0;
	
	if(ble_flag >= 3)
	temp_info.Hw_Version = 1;
	
	hal_gpio_init(HAL_GPIO_12);
	hal_pinmux_set_function(HAL_GPIO_12, HAL_GPIO_12_GPIO12);				  
	hal_gpio_set_direction(HAL_GPIO_12, HAL_GPIO_DIRECTION_OUTPUT);
	hal_gpio_set_output(HAL_GPIO_12, HAL_GPIO_DATA_LOW);

	hal_gpio_init(HAL_GPIO_13);
	hal_pinmux_set_function(HAL_GPIO_13, HAL_GPIO_13_GPIO13);				  
	hal_gpio_set_direction(HAL_GPIO_13, HAL_GPIO_DIRECTION_OUTPUT);
	hal_gpio_set_output(HAL_GPIO_13, HAL_GPIO_DATA_LOW);
	
    if (HAL_GPIO_STATUS_OK != hal_gpio_set_direction(HAL_GPIO_10, HAL_GPIO_DIRECTION_OUTPUT))
	{
        BLE_DBG("GPIO29 hal_gpio_set_direction fail");
        return;
    }
    if (HAL_GPIO_STATUS_OK != hal_gpio_set_output(HAL_GPIO_10, HAL_GPIO_DATA_LOW)){
        BLE_DBG("GPIO29 hal_gpio_set_output fail");
        return;
    }
    if (HAL_EINT_STATUS_OK != hal_eint_deinit(HAL_GPIO_10)) 
	{
        BLE_DBG("GPIO29 deinit fail");
    }
	
    hal_sleep_manager_release_sleeplock(ble_deep_sleep_handler, HAL_SLEEP_LOCK_ALL);
	
}


