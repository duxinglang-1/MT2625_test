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

#define SENSOR_DBG(fmt,arg...)   LOG_I(sbit_demo, "[GPS]: "fmt,##arg)



#define GPS_UART_VFIFO_SIZE         512
#define GPS_UART_ALERT_LENGTH       0
hal_uart_port_t gps_uart = HAL_UART_1;
TimerHandle_t gps_timer = NULL;
char gps_db[32]={0};
bool gps_rest_timer_flag = false;
int gps_stop_delay_sec = 0;

void gps_power_off(void);
void gps_reset_timer(void);
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

int get_gps_num(void)
{		
	temp_info.gps_num=0;
	
	if(gps_db[0]>=5)
	{
		temp_info.gps_num += 1;
	}
	if(gps_db[1]>=5)
	{
		temp_info.gps_num += 1;
	}
	if(gps_db[2]>=5)
	{
		temp_info.gps_num += 1;
	}
	if(gps_db[3]>=5)
	{
		temp_info.gps_num += 1;
	}
	if(gps_db[4]>=5)
	{
		temp_info.gps_num += 1;
	}
	if(gps_db[5]>=5)
	{
		temp_info.gps_num += 1;
	}
	if(gps_db[6]>=5)
	{
		temp_info.gps_num += 1;
	}
	if(gps_db[7]>=5)
	{
		temp_info.gps_num += 1;
	}	
	if(gps_db[8]>=5)
	{
		temp_info.gps_num += 1;
	}
	if(gps_db[9]>=5)
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
extern int gps_status_flag;

void gps_driver_uart_irq(uint8_t gps_str[], uint32_t length)
{
	//uint8_t temp[GPS_UART_VFIFO_SIZE]={0};
	uint8_t *temp = NULL;
	uint32_t i;
	uint8_t buff[GPS_UART_VFIFO_SIZE]={0};
	uint8_t temp1[20]={0};
	char*pch;
	if(strstr(gps_str,"CWLAP") != NULL)
	{
		return;
	}

	temp = strstr(gps_str,"$GPRMC");
	if(temp == NULL)
	{
		temp = strstr(gps_str,"$GNRMC");
	}
	if(temp == NULL)
	{
		temp = strstr(gps_str,"$GNGGA");
	}
	if(temp == NULL)
	{
		temp = strstr(gps_str,"$GPGSV");
	}
	if(temp != NULL)
	{
		SENSOR_DBG("GPSIRQ[%d][%d][%d]:%s\r\n",temp_info.gHeartRatePwrOn,length, strlen(temp),temp);
	}
	else
	{
		SENSOR_DBG("GPSIRQ temp == NULL");
	}

	if((length != 0) && (temp != NULL))
	{
		//memcpy(temp,gps_str,length);
		
		SENSOR_DBG("GPSIRQ(%d)[%d][%d]:%s\r\n",temp_info.Valid_status,length, strlen(temp),temp);
		if(temp_info.Valid_status == 1)
		{
			SENSOR_DBG("GPSS(%s)[%s]:[%s]\r\n",temp_info.gps_speed,temp_info.MY_Latitude, temp_info.MY_Longitude);
		}
#if defined(__GPS_GPGSV__)
		memset(gps_temp,0,sizeof(gps_temp));
		memcpy(gps_temp,temp,sizeof(gps_temp));
		memset(TD11_GPGSV_Buff,0,sizeof(TD11_GPGSV_Buff));
#endif	
		if((strncmp(temp,"$GPRMC",strlen("$GPRMC")) == 0) || (strncmp(temp,"$GNRMC",strlen("$GNRMC")) == 0))
		{
			  if((strstr(temp,",A,") != NULL) && ((strncmp(temp+17,"A",1) == 0) || (strncmp(temp+18,"A",1) == 0)))
			  {
				temp_info.Valid_status = 1;
				SENSOR_DBG("$GNRMC:Valid_status = 1");
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
				if(temp_info.gps_sos_flag >= 10) /* GPS?“??3谷1|o車10??o車℅??‘1?㊣?GPS*/
				{
					temp_info.gps_sos_flag++;
					//temp_info.gps_off_on_flag=0;
					temp_info.Valid_status = 0;
					SENSOR_DBG("sos0:Valid_status = 0");
				}
			  }
			 else
			 {
				 temp_info.Valid_status = 0;
				 SENSOR_DBG("aa:Valid_status = 0");
			 }
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
			 if((strstr(temp, ",N,") != NULL) || (strstr(temp, ",S,") != NULL))
			  {
				temp_info.Valid_status = 1;
				SENSOR_DBG("$GNGGA:Valid_status = 1");
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
						  SENSOR_DBG("MY_Latitude :%s\r\n",temp_info.MY_Latitude);
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
			    if(temp_info.gps_sos_flag >= 10) /* GPS?“??3谷1|o車10??o車℅??‘1?㊣?GPS*/
			    {
					temp_info.gps_sos_flag++;
					//temp_info.gps_off_on_flag=0;
					temp_info.Valid_status = 0;
					SENSOR_DBG("sos1:Valid_status = 0");
				}
			  }
			 else
			 {
				 temp_info.Valid_status = 0;
				 SENSOR_DBG("bb:Valid_status = 0");
			 }
			 
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
void gps_driver_init(void)
{
	
	return;
	bool ble_init_flag = false;
    if(temp_info.gps_off_on_flag == 1)
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
	ble_uart_at_send("GPS+OPEN\r\n");
	temp_info.gps_off_on_flag = 1;
	if(gps_timer == NULL)
	{
		if((strstr(temp_info.imei_num,"000000000000000"))!=NULL)
		{
			gps_timer = xTimerCreate("gps_timer",3600*1000/portTICK_PERIOD_MS, pdFALSE, (void *)0, gps_power_off);
		}
		else
		{
			gps_timer = xTimerCreate("gps_timer",300*1000/portTICK_PERIOD_MS, pdFALSE, (void *)0, gps_power_off);
		}
	}
	xTimerStart(gps_timer,1);
	gps_rest_timer_flag = true;
}

void gps_power_off(void)
{
	return;
	gps_rest_timer_flag = false;
	ble_uart_at_send("GPS+CLOSE\r\n");
	temp_info.gps_off_on_flag = 0;
	if(gps_timer != NULL)
	{
		xTimerStop(gps_timer,1);
	}
	BLE_power_off();
}

void gps_reset_timer(void)
{
	if(temp_info.Valid_status == 1)
	{
		gps_stop_delay_sec++;
	}
	else
	{
		gps_stop_delay_sec = 0;
	}
	
	if((gps_timer != NULL) && (gps_rest_timer_flag == true) && (temp_info.Valid_status == 1) && (gps_stop_delay_sec >= 8))
	{
		xTimerStop(gps_timer,1);
		xTimerChangePeriod(gps_timer, 1*1000/portTICK_PERIOD_MS, 0);
		xTimerStart(gps_timer,1);
		gps_rest_timer_flag = false;
		SENSOR_DBG("================================gps_reset_timer\r\n");
	}
	else
	{
		//SENSOR_DBG("================================gps_reset_timer::[%d, %d], [%d, %d]\r\n", (gps_timer != NULL), (gps_rest_timer_flag == true), temp_info.Valid_status, gps_stop_delay_sec);
	}
}

void gps_get_data(void)
{
	ble_uart_at_send("GPS+DATA\r\n");
}

