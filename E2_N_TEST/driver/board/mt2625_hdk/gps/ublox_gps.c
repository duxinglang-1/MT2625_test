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

#define SENSOR_DBG(fmt,arg...)   LOG_I(sbit_demo, "[UBLOX GPS]: "fmt,##arg)



#define GPS_UART_VFIFO_SIZE         512
#define GPS_UART_ALERT_LENGTH       0
hal_uart_port_t gps_uart = HAL_UART_2;
TimerHandle_t gps_timer = NULL;
char gps_db[32]={0};

void gps_power_off();
ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t GPS_uart_tx_vfifo[GPS_UART_VFIFO_SIZE];
ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t GPS_uart_rx_vfifo[GPS_UART_VFIFO_SIZE];

char *get_gps_db(void)
{
	return gps_db;
}

void Latitude_change(char *input,char *output)
{
	double temp1=0,temp2=0,temp3=0;
	char Latitude_buf[20]={0};
	char Latitude_buf1[20]={0};
	
	//3001.460

	strncpy(Latitude_buf,input,2);
	strncat(Latitude_buf1,input+2,(strlen(input))-2);
	
	temp1=atof(Latitude_buf);
	temp2=(atof(Latitude_buf1))/60;
	temp3=temp2+temp1;
	
	sprintf(Latitude_buf,"%f",temp3);
	strcat(output,Latitude_buf);
	
}

void Longitude_change(char *input,char *output)
{
	double temp1=0,temp2=0,temp3=0;
	char Longitude_buf[20]={0};
	char Longitude_buf1[20]={0};
	
	//12031.460

	strncpy(Longitude_buf,input,3);
	strncat(Longitude_buf1,input+3,(strlen(input))-3);
	
	temp1=atof(Longitude_buf);
	temp2=(atof(Longitude_buf1))/60;
	temp3=temp2+temp1;
	memset(Longitude_buf, 0, sizeof(Longitude_buf) );		
	sprintf(Longitude_buf,"%0.5f",temp3);
	
	strcat(output,Longitude_buf);
	
}
#define __GPS_GPGSV__
#if defined(__GPS_GPGSV__)
char TD11_GPGSV_Buff[256]={0};
uint8_t gps_temp[GPS_UART_VFIFO_SIZE]={0};
char *gpgsv_p[16] = {0};
char gpgsv_data[32][128] = {0};

int get_gps_num(int criterion)
{		
	temp_info.gps_num=0;
	
	if(gps_db[0]>=criterion)
	{
		temp_info.gps_num += 1;
	}
	if(gps_db[1]>=criterion)
	{
		temp_info.gps_num += 1;
	}
	if(gps_db[2]>=criterion)
	{
		temp_info.gps_num += 1;
	}
	if(gps_db[3]>=criterion)
	{
		temp_info.gps_num += 1;
	}
	if(gps_db[4]>=criterion)
	{
		temp_info.gps_num += 1;
	}
	if(gps_db[5]>=criterion)
	{
		temp_info.gps_num += 1;
	}
	if(gps_db[6]>=criterion)
	{
		temp_info.gps_num += 1;
	}
	if(gps_db[7]>=criterion)
	{
		temp_info.gps_num += 1;
	}	
	if(gps_db[8]>=criterion)
	{
		temp_info.gps_num += 1;
	}
	if(gps_db[9]>=criterion)
	{
		temp_info.gps_num += 1;
	}
	
	if(gps_db[10]>=criterion)
	{
		temp_info.gps_num += 1;
	}
	
	if(gps_db[11]>=criterion)
	{
		temp_info.gps_num += 1;
	}
	
	if(gps_db[12]>=criterion)
	{
		temp_info.gps_num += 1;
	}
	
	if(gps_db[13]>=criterion)
	{
		temp_info.gps_num += 1;
	}
	
	if(gps_db[14]>=criterion)
	{
		temp_info.gps_num += 1;
	}
	
	return temp_info.gps_num;
	
}
int gps_gpgsv_count(char *str1, char *str2)
{
	int ret = 0, i = 0;
	char *p = str1;
	char *d1 = NULL, *d2 = NULL;
	if(p == NULL)
	{
		return 0;
	}
	memset(gpgsv_p, 0, sizeof(gpgsv_p));
	memset(gpgsv_data, 0, sizeof(gpgsv_data));
	while(*p != '\0')
	{
		p = strstr(p, str2);
		if(p == NULL)
		{
			break;
		}
		else
		{
			gpgsv_p[i] = p;
			d1 = gpgsv_p[i];
			d2 = strstr((char *)&d1[6], "$");
			if(d2 != NULL)
			{
				strncpy(gpgsv_data[i],d1,(d2-d1));
				//SENSOR_DBG("[========]GPS data aa i:%d:%s\r\n",i, gpgsv_data[i]);
			}
			else
			{
				strcpy(gpgsv_data[i],(char*)d1);
				//SENSOR_DBG("[========]GPS data bb i:%d:%s\r\n",i, gpgsv_data[i]);
			}
			//SENSOR_DBG("[========]GPS retp i:%d:0x%x\r\n",i, p);
			i += 1;
			ret += 1;
			p += 6;
		}
	}
	//SENSOR_DBG("[========]GPS ret :%d\r\n",ret);
	return ret;
}
void gps_paser_db()
{
	int i = 0;
	char TD11_buff1[256]={0}; 
	char TD11_buff2[256]={0}; 
	char*pch;
	
	for(i=0 ; i<32 ; i++)
	  { 	
		if(i==0)
		{
		  strncpy(TD11_buff1,TD11_GPGSV_Buff,256);
		  //SENSOR_DBG("[========]TD11_GPGSV_Buff[%s]\r\n",TD11_GPGSV_Buff);
		}
		if(((strpbrk (TD11_buff1,","))!=NULL))
		  {
			  pch=strstr(TD11_buff1,"," );
			  *pch++='\0'; 
			  memset(TD11_buff2,0,sizeof(TD11_buff2));
			  strncpy(TD11_buff2,pch,256);
			  if(((strpbrk (TD11_buff2,","))!=NULL))
				{
					pch=strstr(TD11_buff2,"," );
					*pch++='\0';		  
					memset(TD11_buff1,0,sizeof(TD11_buff1));
					strncpy(TD11_buff1,pch,256);
				}
			  else
				  break;
			  if(((strpbrk (TD11_buff1,","))!=NULL))
				{
					pch=strstr(TD11_buff1,"," );
					*pch++='\0';		  
					memset(TD11_buff2,0,sizeof(TD11_buff2));
					strncpy(TD11_buff2,pch,256);
				}
			  else
				  break;
			  if(((strpbrk (TD11_buff2,","))!=NULL))
				{
					pch=strstr(TD11_buff2,"," );
					*pch++='\0';		  
					memset(TD11_buff1,0,sizeof(TD11_buff1));
					strncpy(TD11_buff1,pch,256);
					gps_db[i]=atoi(TD11_buff2);
					//SENSOR_DBG("[========]gps_dbAA[%d] :%d\r\n",i, gps_db[i]);
				}
			  else
				{
					gps_db[i]=atoi(TD11_buff2);
					//SENSOR_DBG("[========]gps_dbbb[%d]:%d\r\n",i, gps_db[i]);
					break;
				}
		   }
	  }
}
void gps_get_gpgsv(char *buffer)
{
   char TD11_buff1[256]={0}; 
   char TD11_buff2[256]={0}; 
   char TD11_buff3[256]={0}; 
   int len,i;
   char *pch;
   char *p;
   
   //SENSOR_DBG("[========]gps_get_gpgsv %s\r\n", buffer);
   
   if(strlen(buffer) <= strlen("$GPGSV"))
   {
	return;
   }
   if((strlen(buffer) > strlen("$GPGSV")) && (strncmp(buffer,"$GPGSV",strlen("$GPGSV")) == 0))
   {
	   //SENSOR_DBG("[========]gps_get_gpgsv dd\r\n");
     p = strstr((char *)&buffer[6], "$");
	 if(p != NULL)
 	 {
		 strncpy(TD11_buff1,buffer,(p-buffer));
		 strncpy(TD11_buff3,buffer,(p-buffer));
		 //SENSOR_DBG("[========]GPS $GPGSV L:%d\r\n",strlen(TD11_buff1));
		 //SENSOR_DBG("[========]GPS $GPGSV C:%s\r\n",TD11_buff1);
	 }
	 else
	 {
		 strcpy(TD11_buff1,(char*)buffer);
		 strcpy(TD11_buff3,(char*)buffer);
		 //SENSOR_DBG("[========]GPS $GPGSV L3:%d\r\n",strlen(TD11_buff3));
		 //SENSOR_DBG("[========]GPS $GPGSV C3:%s\r\n",TD11_buff3);
	 }
   }
    if(strncmp(TD11_buff1,"$GPGSV",strlen("$GPGSV")) == 0)
   	{
   	
	   //SENSOR_DBG("[========]$GPGSV:%s\r\n",TD11_buff1);
	   
	  if(((strpbrk (TD11_buff1,","))!=NULL))
	    {
			pch=strstr(TD11_buff1,"," );
			*pch++='\0'; 
			memset(TD11_buff2,0,sizeof(TD11_buff2));
			strcpy(TD11_buff2,pch);
			//SENSOR_DBG("[========]$GPGSV aa:%s\r\n",TD11_buff1);
			if(((strpbrk (TD11_buff2,","))!=NULL))
			  {
				  pch=strstr(TD11_buff2,"," );
				  *pch++='\0'; 			
				  memset(TD11_buff1,0,sizeof(TD11_buff1));
				  strcpy(TD11_buff1,pch);
			  }
			//SENSOR_DBG("[========]$GPGSV bb:%s\r\n",TD11_buff1);
			if(((strpbrk (TD11_buff1,","))!=NULL))
			  {
				  pch=strstr(TD11_buff1,"," );
				  *pch++='\0'; 			
				  memset(TD11_buff2,0,sizeof(TD11_buff2));
				  strcpy(TD11_buff2,pch);
			  }
			//SENSOR_DBG("[========]$GPGSV cc:%s\r\n",TD11_buff1);
			if(((strpbrk (TD11_buff2,","))!=NULL))
			  {
				  pch=strstr(TD11_buff2,"," );
				  *pch++='\0'; 			
				  memset(TD11_buff1,0,sizeof(TD11_buff1));
				  strcpy(TD11_buff1,pch);
			  }
			
			temp_info.gps_num=0;
			temp_info.gps_num = atoi(TD11_buff2);
			
			SENSOR_DBG("[=========================================]$GPGSV dd:%d\r\n",temp_info.gps_num);
			//SENSOR_DBG("[========]$GPGSV dd:%s\r\n",TD11_buff2);
			//SENSOR_DBG("[========]$GPGSV dd:%s\r\n",TD11_buff1);
			if(((strpbrk (TD11_buff1,"*"))!=NULL))
			  {
				  pch=strstr(TD11_buff1,"*" );
				  *pch++='\0'; 
				  if(strlen(TD11_GPGSV_Buff)>0)
				  {
				  	strcat(TD11_GPGSV_Buff,",");
				  }
				  strcat(TD11_GPGSV_Buff,TD11_buff1);
				  //SENSOR_DBG("[========]TD11_GPGSV_BuffBB[%s]\r\n",TD11_GPGSV_Buff);
			  }
	  	}
    }
}
#endif
void gps_driver_uart_irq(hal_uart_callback_event_t status, void *parameter)
{
	uint8_t temp[GPS_UART_VFIFO_SIZE]={0};
	uint32_t length, i;
	uint8_t buff[GPS_UART_VFIFO_SIZE]={0};
	uint8_t temp1[20]={0};
	char*pch;
	SENSOR_DBG("gps_driver_uart_irq:%d\r\n",status);
	if ((HAL_UART_EVENT_READY_TO_READ == status))
	{
		length = hal_uart_get_available_receive_bytes(gps_uart);
		hal_uart_receive_dma(gps_uart, (uint8_t*)temp, length);	
		
		SENSOR_DBG("temp :%s\r\n",temp);
#if defined(__GPS_GPGSV__)
		memset(gps_temp,0,sizeof(gps_temp));
		memcpy(gps_temp,temp,sizeof(gps_temp));
		memset(TD11_GPGSV_Buff,0,sizeof(TD11_GPGSV_Buff));
#endif	
		if((strncmp(temp,"$GPRMC",strlen("$GPRMC")) == 0) || (strncmp(temp,"$GNRMC",strlen("$GNRMC")) == 0))
		{
			 if((strncmp(temp+17,"A",1) == 0) || (strncmp(temp+18,"A",1) == 0))
			  {
				temp_info.Valid_status=1;
				NVRAM_info.fm_gps_flag = 1;
				temp_info.gps_sos_flag++;
				if(((strpbrk (temp,"A"))!=NULL))
				 {
					pch=strstr(temp,"A" );
					*pch++='\0'; 
					*pch++='\0'; 
					memset(buff,0,sizeof(buff));
					strcpy(buff,pch);
				#if defined(MD_WATCH_SUPPORT)
					if(strstr(temp, ",N,") != NULL)
					{
						strcpy(temp_info.north_south,"N");
					}
					else if(strstr(temp, ",S,") != NULL)
					{
						strcpy(temp_info.north_south,"S");
					}
					if(strstr(temp, ",E,") != NULL)
					{
						strcpy(temp_info.east_west,"E");
					}
					else if(strstr(temp, ",W,") != NULL)
					{
						strcpy(temp_info.east_west,"W");
					}
				#endif	
					if(((strpbrk (buff,","))!=NULL))
					  {
						  pch=strstr(buff,"," );
						  *pch++='\0'; 
						  *pch++='\0'; 
						  *pch++='\0';			
						  memset(temp,0,sizeof(temp));
						  strcpy(temp,pch);
						  memset(temp1,0,sizeof(temp1));
						  memset(temp_info.MY_Latitude,0,sizeof(temp_info.MY_Latitude));
						  strncpy(temp_info.MY_Latitude,buff,20);
						  SENSOR_DBG("MY_Latitude :%s\r\n",temp_info.MY_Latitude);
					  }
					
					if(((strpbrk (temp,","))!=NULL))
					  {
						  pch=strstr(temp,"," );
						  *pch++='\0';			
						  *pch++='\0';			
						  *pch++='\0';			
						  memset(buff,0,sizeof(buff));
						  strcpy(buff,pch);
						  memset(temp1,0,sizeof(temp1));
						  memset(temp_info.MY_Longitude,0,sizeof(temp_info.MY_Longitude));
						  strncpy(temp_info.MY_Longitude,temp,20);
						  SENSOR_DBG("MY_Longitude :%s\r\n",temp_info.MY_Longitude);
					  }
					
					if(((strpbrk (buff,","))!=NULL))
					  {
						  pch=strstr(buff,"," );
						  *pch++='\0'; 
						  memset(temp_info.gps_speed,0,sizeof(temp_info.gps_speed));
						  strncpy(temp_info.gps_speed,buff,10);
						  SENSOR_DBG("temp_info.gps_speed :%s\r\n",temp_info.gps_speed);
					  }
		
				 }
			    if((temp_info.gps_sos_flag>=10)/* GPS定位成功后10秒后自动关闭GPS*/
					/*&&(get_sbit_show_meun_id() != 5*/)		/* 在定位界面不自动关闭GPS jerry */
				    {
						temp_info.gps_sos_flag++;
						temp_info.gps_off_on_flag=0;
						temp_info.Valid_status=0;


						if(strlen(temp_info.MY_Latitude)>0 &&  strlen(temp_info.MY_Longitude)>0 )
						{
							gps_power_off();
							sbit_m2m_ct_send_massege(M2M_CT_gps_data_T29,NULL,0,TRUE);
						}
						
						#if 0
						if (HAL_GPIO_STATUS_OK != hal_gpio_set_direction(HAL_GPIO_29, HAL_GPIO_DIRECTION_OUTPUT))
						{
							//SENSOR_DBG("GPIO29 hal_gpio_set_direction fail");
							return;
						}
						if (HAL_GPIO_STATUS_OK != hal_gpio_set_output(HAL_GPIO_29, HAL_GPIO_DATA_LOW))
						{
							//SENSOR_DBG("GPIO29 hal_gpio_set_output fail");
							return;
						}
						if (HAL_EINT_STATUS_OK != hal_eint_deinit(HAL_GPIO_29)) 
						{
							//SENSOR_DBG("GPIO29 deinit fail");
						}
						#endif
					}
			  }
			 else
				 temp_info.Valid_status=0;
		}
		else if(strncmp(temp,"$GNGGA",strlen("$GNGGA")) == 0)
		{
			//$GNGGA,094747.000,2233.5643,N,11353.4674,E,1,07,1.1,17.4,M,0.0,M,,*4E
			//095710.351,,,,,0,00,25.5,,,,,,*77
			#if defined(MD_WATCH_SUPPORT)
				if(strstr(temp, ",N,") != NULL)
				{
					strcpy(temp_info.north_south,"N");
				}
				else if(strstr(temp, ",S,") != NULL)
				{
					strcpy(temp_info.north_south,"S");
				}
				if(strstr(temp, ",E,") != NULL)
				{
					strcpy(temp_info.east_west,"E");
				}
				else if(strstr(temp, ",W,") != NULL)
				{
					strcpy(temp_info.east_west,"W");
				}
			#endif	
			 if((strncmp(temp+28,"N",1) == 0) || (strncmp(temp+29,"N",1) == 0))
			  {
				temp_info.Valid_status=1;
				NVRAM_info.fm_gps_flag = 1;
				temp_info.gps_sos_flag++;
				if(((strpbrk (temp+10,","))!=NULL))
				 {
					pch=strstr(temp+10,"," );
					*pch++='\0'; 
					memset(buff,0,sizeof(buff));
					strcpy(buff,pch);
					
					if(((strpbrk (buff,","))!=NULL))
					  {
						  pch=strstr(buff,"," );
						  *pch++='\0'; 
						  *pch++='\0'; 
						  *pch++='\0';			
						  memset(temp,0,sizeof(temp));
						  strcpy(temp,pch);
						  memset(temp1,0,sizeof(temp1));
						  memset(temp_info.MY_Latitude,0,sizeof(temp_info.MY_Latitude));
						  strncpy(temp_info.MY_Latitude,buff,20);
						  SENSOR_DBG("MY_Latitude_0 :%s\r\n",temp_info.MY_Latitude);
					  }
					
					if(((strpbrk (temp,","))!=NULL))
					  {
						  pch=strstr(temp,"," );
						  *pch++='\0';			
						  memset(buff,0,sizeof(buff));
						  strcpy(buff,pch);
						  memset(temp1,0,sizeof(temp1));
						  memset(temp_info.MY_Longitude,0,sizeof(temp_info.MY_Longitude));
						  strncpy(temp_info.MY_Longitude,temp,20);
						  SENSOR_DBG("MY_Longitude :%s\r\n",temp_info.MY_Longitude);
					  }
					
					if(((strpbrk (buff,","))!=NULL))
					  {
						  pch=strstr(buff,"," );
						  *pch++='\0'; 
						  memset(temp,0,sizeof(temp));
						  strcpy(temp,pch);

					  }
					if(((strpbrk (temp,","))!=NULL))
					  {
						  pch=strstr(temp,"," );
						  *pch++='\0'; 
						  memset(buff,0,sizeof(buff));
						  strcpy(buff,pch);
					  }
					if(((strpbrk (buff,","))!=NULL))
					  {
						  pch=strstr(buff,"," );
						  *pch++='\0'; 
						  memset(temp,0,sizeof(temp));
						  strcpy(temp,pch);

					  }
					if(((strpbrk (temp,","))!=NULL))
					  {
						  pch=strstr(temp,"," );
						  *pch++='\0'; 
						  memset(temp_info.gps_speed,0,sizeof(temp_info.gps_speed));
						  strncpy(temp_info.gps_speed,temp,10);
						  SENSOR_DBG("temp_info.gps_speed :%s\r\n",temp_info.gps_speed);
					  }
		
				 }
			    if((temp_info.gps_sos_flag>=10)/* GPS定位成功后10秒后自动关闭GPS*/
					&&(get_sbit_show_meun_id() != 5)		/* 在定位界面不自动关闭GPS jerry */
					) 
			    {
					temp_info.gps_sos_flag++;
					temp_info.gps_off_on_flag=0;
					temp_info.Valid_status=0;
					
					sbit_m2m_ct_send_massege(M2M_CT_gps_data_T29,NULL,0,TRUE);
					
					if (HAL_GPIO_STATUS_OK != hal_gpio_set_direction(HAL_GPIO_29, HAL_GPIO_DIRECTION_OUTPUT))
					{
						//SENSOR_DBG("GPIO29 hal_gpio_set_direction fail");
						return;
					}
					if (HAL_GPIO_STATUS_OK != hal_gpio_set_output(HAL_GPIO_29, HAL_GPIO_DATA_LOW))
					{
						//SENSOR_DBG("GPIO29 hal_gpio_set_output fail");
						return;
					}
					if (HAL_EINT_STATUS_OK != hal_eint_deinit(HAL_GPIO_29)) 
					{
						//SENSOR_DBG("GPIO29 deinit fail");
					}
				}
			  }
			 else
				 temp_info.Valid_status=0;
			 
		}
#if defined(__GPS_GPGSV__)
		{
			int i = 0, count = 0;
			count = gps_gpgsv_count(gps_temp, "$GPGSV");
			
			for(i = 0; i < count; i++)
			{
				SENSOR_DBG("[========]GPS count :%d, %s\r\n",count, gpgsv_data[i]);
				gps_get_gpgsv(gpgsv_data[i]);
			}
			
			if(strlen(TD11_GPGSV_Buff)>0)
			{
				gps_paser_db();
			}
		}
#endif
	} 
	
}
uint8_t gps_deep_sleep_handler = 0xFF;
#if defined(MTK_H10S_SUPPORT) 
TimerHandle_t gps_driver_init_timer=NULL;
#endif
void gps_driver_init(void)
{
	
    hal_uart_config_t uart_config;
	hal_uart_dma_config_t dma_config;

	
	return;
	//因我司欧洲有个重要客户需求，将N1的海外版固件在最后的版本N1_2022_0526基础上UI再将睡眠和血压屏蔽掉，
	//也不要检测和上报这两个数据，同时SOS启动时还是保留发定位信息，谢谢！2022_0804
	if((temp_info.gps_sos_mark != 1))
    return;
	
	if(temp_info.gps_off_on_flag==1)
	{
		return;
	}
	
	hal_uart_deinit(gps_uart);
	
	temp_info.gps_sos_flag=0;
	temp_info.gps_off_on_flag=1;
	memset(gps_db,0,sizeof(gps_db));
	memset(temp_info.MY_Latitude,0,sizeof(temp_info.MY_Latitude));
	memset(temp_info.MY_Longitude,0,sizeof(temp_info.MY_Longitude));
	hal_pinmux_set_function(HAL_GPIO_29, HAL_GPIO_29_GPIO29);			  /*   set dierection to be output	*/			  
	hal_gpio_set_direction(HAL_GPIO_29, HAL_GPIO_DIRECTION_OUTPUT);
	hal_gpio_set_output(HAL_GPIO_29,HAL_GPIO_DATA_HIGH);
		 
#if defined(MTK_WIFI_SUPPORT) ||defined(MTK_BLE_SUPPORT)
	hal_gpio_init(HAL_GPIO_3);
	hal_pinmux_set_function(HAL_GPIO_3, HAL_GPIO_3_UART2_RXD);			  	  
	hal_gpio_set_direction(HAL_GPIO_3, HAL_GPIO_DIRECTION_INPUT);
	hal_gpio_init(HAL_GPIO_4);
	hal_pinmux_set_function(HAL_GPIO_4, HAL_GPIO_4_UART2_TXD);			  	  
	hal_gpio_set_direction(HAL_GPIO_4, HAL_GPIO_DIRECTION_OUTPUT);
#else
	hal_gpio_init(HAL_GPIO_31);
	hal_pinmux_set_function(HAL_GPIO_31, HAL_GPIO_31_UART2_RXD);				  
	hal_gpio_set_direction(HAL_GPIO_31, HAL_GPIO_DIRECTION_INPUT);
	hal_gpio_init(HAL_GPIO_32);
	hal_pinmux_set_function(HAL_GPIO_32, HAL_GPIO_32_UART2_TXD);				  
	hal_gpio_set_direction(HAL_GPIO_32, HAL_GPIO_DIRECTION_OUTPUT);
#endif

	uart_config.baudrate = HAL_UART_BAUDRATE_9600;
	uart_config.parity = HAL_UART_PARITY_NONE;
	uart_config.stop_bit = HAL_UART_STOP_BIT_1;
	uart_config.word_length = HAL_UART_WORD_LENGTH_8;
	//Init UART
	dma_config.receive_vfifo_buffer = GPS_uart_rx_vfifo;
	dma_config.receive_vfifo_buffer_size = GPS_UART_VFIFO_SIZE;
	dma_config.receive_vfifo_alert_size = GPS_UART_ALERT_LENGTH;
	dma_config.receive_vfifo_threshold_size = 200;
	dma_config.send_vfifo_buffer = GPS_uart_tx_vfifo;
	dma_config.send_vfifo_buffer_size = GPS_UART_VFIFO_SIZE;
	dma_config.send_vfifo_threshold_size = 8;

	if (HAL_UART_STATUS_OK == hal_uart_init(gps_uart, &uart_config)) 
	{
	   SENSOR_DBG("[ZZZZZZ]uart init success\n\r");
	} 
	else 
	{
	   SENSOR_DBG("[ZZZZZZ]uart init fail\n\r");
	}


	hal_uart_set_dma(gps_uart, &dma_config);
	hal_uart_register_callback(gps_uart, gps_driver_uart_irq, NULL);

	if(gps_timer == NULL)
	{
	    gps_timer = xTimerCreate("gps_timer",    /* 每次GPS最长开启定位180秒 jerry */
	    900*1000 / portTICK_PERIOD_MS, 
	    pdFALSE, 
	    (void *) 0, 
	    gps_power_off);
	}
	
//	if(get_sbit_show_meun_id() != 5)        /* 在定位界面不自动关闭GPS jerry */
    xTimerStart(gps_timer,1);
    if (gps_deep_sleep_handler == 0xFF)
    gps_deep_sleep_handler = hal_sleep_manager_set_sleep_handle("GPS");
    hal_sleep_manager_acquire_sleeplock(gps_deep_sleep_handler, HAL_SLEEP_LOCK_ALL);

	

}

void gps_power_off()
{
	
	return;
	SENSOR_DBG("<<<<<<<<<<<<<<<<<<<<<<<<<<<<gps_power_off>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");	
	if((temp_info.gps_off_on_flag==0)||(temp_info.fm_mode == 1))
	return;
	
	if(gps_timer!=NULL)
	xTimerStop(gps_timer,0);
	
    hal_sleep_manager_release_sleeplock(gps_deep_sleep_handler, HAL_SLEEP_LOCK_ALL);
	
	hal_gpio_init(HAL_GPIO_3);
	hal_pinmux_set_function(HAL_GPIO_3, HAL_GPIO_3_GPIO3);			  	  
	hal_gpio_set_direction(HAL_GPIO_3, HAL_GPIO_DIRECTION_INPUT);
	
	hal_gpio_init(HAL_GPIO_4);
	hal_pinmux_set_function(HAL_GPIO_4, HAL_GPIO_4_GPIO4);			  	  
	hal_gpio_set_direction(HAL_GPIO_4, HAL_GPIO_DIRECTION_INPUT);

	
    if (HAL_GPIO_STATUS_OK != hal_gpio_set_direction(HAL_GPIO_29, HAL_GPIO_DIRECTION_OUTPUT)){
       // SENSOR_DBG("GPIO29 hal_gpio_set_direction fail");
        return;
    }
    if (HAL_GPIO_STATUS_OK != hal_gpio_set_output(HAL_GPIO_29, HAL_GPIO_DATA_LOW)){
        //SENSOR_DBG("GPIO29 hal_gpio_set_output fail");
        return;
    }
    if (HAL_EINT_STATUS_OK != hal_eint_deinit(HAL_GPIO_29)) {
        //SENSOR_DBG("GPIO29 deinit fail");
    }

	if((strlen(temp_info.MY_Latitude) != 0))
	{
		//	&&(temp_info.sos_mark == 0))
		sos_send_data();  //kyb add 2022_0804
		sbit_m2m_ct_send_massege(M2M_CT_gps_data_T29,NULL,0,0);
	}

	
	memset(gps_db,0,sizeof(gps_db));
	temp_info.gps_off_on_flag=0;
	temp_info.gps_sos_flag=0;   
	temp_info.Valid_status=0;
	temp_info.sos_mark = 0;
	temp_info.gps_sos_mark=0;
}


