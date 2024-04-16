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
#include "nvdm_modem.h"

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
#include "type_def.h"

#define NET_DBG(fmt,arg...)  //LOG_I(sbit_demo, "[sbit]: "fmt,##arg)
unsigned int Gutc_time;

#define IPPORTSTR "180.101.147.115,5683"
#define LIFETIME	"90"
apb_proxy_parse_cmd_param_t m2mclinew_cmd = {0};
extern apb_proxy_status_t apb_proxy_check_cmd_id(apb_proxy_cmd_id_t proxy_cmd_id);  

#define APB_PROXY_M2MCLINEW_CMD_ID 0x20C
#define APB_PROXY_M2MCLIDEL_CMD_ID 0x20D
#define APB_PROXY_M2MCLISEND_CMD_ID 0x20E

TimerHandle_t sbit_client_create_timer=NULL;
TimerHandle_t sbit_cfun_off_timer=NULL;
int32_t phone_func = 1;
int lwm2m_client_create_mark=0;

#define  DT_MONTH_PER_YEAR    12
#define DT_UTC_BASE_YEAR 1970
#define DT_DAY_PER_YEAR 365
#define DT_SEC_PER_DAY 86400
#define DT_SEC_PER_HOUR 3600
#define DT_SEC_PER_MIN 60
const unsigned char g_dt_day_per_mon[DT_MONTH_PER_YEAR] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};





unsigned char applib_dt_is_leap_year(unsigned short int year)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((year % 400) == 0)
    {
        return 1;
    }
    else if ((year % 100) == 0)
    {
        return 0;
    }
    else if ((year % 4) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
unsigned char applib_dt_last_day_of_mon(unsigned char month, unsigned short int year)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((month == 0) || (month > 12))
    {
		return g_dt_day_per_mon[1] + applib_dt_is_leap_year(year);
  	}
  	
    if (month != 2)
    {
        return g_dt_day_per_mon[month - 1];
    }
    else
    {
        return g_dt_day_per_mon[1] + applib_dt_is_leap_year(year);
    }
}

unsigned int applib_dt_mytime_2_utc_sec(hal_rtc_time_t *currTime)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    unsigned short int i;
    unsigned int no_of_days = 0;
    int utc_time;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	//NET_DBG("======::rtc(%d, %d, %d)(%d, %d, %d)", (currTime->rtc_year+2000), currTime->rtc_mon, currTime->rtc_day, currTime->rtc_hour, currTime->rtc_min, currTime->rtc_sec);
	
    if ((currTime->rtc_year+2000) < DT_UTC_BASE_YEAR)
    {
        return 0;
    }

    /* year */
    for (i = DT_UTC_BASE_YEAR; i < (currTime->rtc_year+2000); i++)
    {
        no_of_days += (365 + applib_dt_is_leap_year(i));
    }

    /* month */
    for (i = 1; i < currTime->rtc_mon; i++)
    {
        no_of_days += applib_dt_last_day_of_mon((unsigned char) i, (currTime->rtc_year+2000));
    }

    /* day */
    no_of_days += (currTime->rtc_day- 1);

    /* sec */
    utc_time =
        (unsigned int) no_of_days *DT_SEC_PER_DAY + (unsigned int) ((currTime->rtc_hour)* DT_SEC_PER_HOUR +
                                                                currTime->rtc_min* DT_SEC_PER_MIN + currTime->rtc_sec);

    //return utc_time+(8*3600);
    return utc_time;
}

unsigned int ch_get_utc_sec()
{
    hal_rtc_time_t curtime;
	unsigned int utc_time;
	hal_rtc_get_time(&curtime);//获取rtctime
	utc_time = (applib_dt_mytime_2_utc_sec(&curtime));
	return utc_time;
}


typedef struct
{
    kal_uint16 nYear;
    kal_uint8 nMonth;
    kal_uint8 nDay;
    kal_uint8 nHour;
    kal_uint8 nMin;
    kal_uint8 nSec;
    kal_uint8 DayIndex; /* 0=Sunday */
} applib_time_struct;
unsigned char g_dst;

kal_bool applib_dt_is_dst_enabled(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    return (kal_bool) g_dst;

}

kal_uint8 applib_dt_utc_sec_1_mytime(kal_uint32 utc_sec, applib_time_struct *result, kal_bool daylightSaving)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/
	kal_int32 sec, day;
	kal_uint16 y;
	kal_uint8 m;
	kal_uint16 d;
	kal_uint8 dst;

	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
	dst = applib_dt_is_dst_enabled();

	if (dst && daylightSaving)
	{
		utc_sec += DT_SEC_PER_HOUR;
	}

	/* hour, min, sec */
	/* hour */
	sec = utc_sec % DT_SEC_PER_DAY;
	result->nHour = sec / DT_SEC_PER_HOUR;

	/* min */
	sec %= DT_SEC_PER_HOUR;
	result->nMin = sec / DT_SEC_PER_MIN;

	/* sec */
	result->nSec = sec % DT_SEC_PER_MIN;

	/* year, month, day */
	/* year */
	/* year */
	day = utc_sec / DT_SEC_PER_DAY;
	for (y = DT_UTC_BASE_YEAR; day > 0; y++)
	{
		d = (DT_DAY_PER_YEAR + applib_dt_is_leap_year(y));
		if (day >= d)
		{
			day -= d;
		}
		else
		{
			break;
		}
	}
	result->nYear = y;

	for (m = 1; m < DT_MONTH_PER_YEAR; m++)
	{
		d = applib_dt_last_day_of_mon(m, y);
		if (day >= d)
		{
			day -= d;
		}
		else
		{
			break;
		}
	}

	result->nMonth = m;
	result->nDay = (kal_uint8) (day + 1);
	result->DayIndex = 0;
	return KAL_TRUE;
}

void time_test(hal_rtc_time_t *dest_rtc_time,int seceond)
{
     applib_time_struct result_t = {0};
	 applib_dt_utc_sec_1_mytime(seceond, &result_t, 0);
     dest_rtc_time->rtc_sec   = result_t.nSec;
	 dest_rtc_time->rtc_min   = result_t.nMin;
	 dest_rtc_time->rtc_hour  = result_t.nHour;
	 dest_rtc_time->rtc_year  = result_t.nYear-2000;
	 dest_rtc_time->rtc_mon = result_t.nMonth;
	 dest_rtc_time->rtc_day   = result_t.nDay;
	 result_t.DayIndex =(result_t.nDay+2*result_t.nMonth+3*(result_t.nMonth+1)/5+result_t.nYear+result_t.nYear/4-result_t.nYear/100+result_t.nYear/400)%7;
	 dest_rtc_time->rtc_week=  result_t.DayIndex;
 
	 NET_DBG("applib_dt_utc_time:%d-%d-%d-%d-%d-%d\r\n", result_t.nYear,result_t.nMonth, result_t.nDay,result_t.nHour,result_t.nMin,result_t.nSec);


}
#if 1

kal_uint8 applib_dt_utc_sec_2_mytime(kal_uint32 utc_sec, hal_rtc_time_t *result, kal_bool daylightSaving)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_int32 sec, day;
    kal_uint16 y;
    kal_uint8 m;
    kal_uint16 d;
    kal_uint8 dst;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dst = 0;//applib_dt_is_dst_enabled();

    //if (daylightSaving)
    {
     //   utc_sec += DT_SEC_PER_HOUR*daylightSaving;
    }

    /* hour, min, sec */
    /* hour */
    sec = utc_sec % DT_SEC_PER_DAY;
    result->rtc_hour= sec / DT_SEC_PER_HOUR;

    /* min */
    sec %= DT_SEC_PER_HOUR;
    result->rtc_min= sec / DT_SEC_PER_MIN;

    /* sec */
    result->rtc_sec= sec % DT_SEC_PER_MIN;

    /* year, month, day */
    /* year */
    /* year */
    day = utc_sec / DT_SEC_PER_DAY;
    for (y = DT_UTC_BASE_YEAR; day > 0; y++)
    {
        d = (DT_DAY_PER_YEAR + applib_dt_is_leap_year(y));
        if (day >= d)
        {
            day -= d;
        }
        else
        {
            break;
        }
    }
    result->rtc_year= (uint8_t)(y-2000);

    for (m = 1; m < DT_MONTH_PER_YEAR; m++)
    {
        d = applib_dt_last_day_of_mon(m, y);
        if (day >= d)
        {
            day -= d;
        }
        else
        {
            break;
        }
    }

    result->rtc_mon= m;
    result->rtc_day= (kal_uint8) (day + 1);
    return KAL_TRUE;
}

void sbit_formate_time(kal_uint32 utc_sec)
{
    hal_rtc_time_t utc_time;
    applib_dt_utc_sec_2_mytime(utc_sec, &utc_time, KAL_FALSE);

}

#endif

void str_to_hex(char buffer_str[], char buffer_hex[])
{ 
	int i = 0, str_len = 0;
	char *buffer_hex_ptr;
	
	memset(temp_info.data_buffer_hex, 0, sizeof(temp_info.data_buffer_hex));
	memset(buffer_hex, 0, sizeof(temp_info.data_buffer_hex));
	strcat(buffer_hex,"AT+M2MCLISEND=4C53440D");
	str_len = strlen(buffer_hex);
	buffer_hex_ptr = &buffer_hex[str_len];
	
	for(i = 0; i < strlen(buffer_str); i++)
	{
		//sprintf(buffer_hex[i*2], "%x", buffer_str[i]);
		sprintf(&buffer_hex_ptr[i*2], "%2x", buffer_str[i]);
	}
}  

int32_t get_phone_func(void)
{
   return phone_func;
}  

void set_phone_functionality(int32_t fun )
{ 
    int ret = 1;

	return;
	
	if(phone_func!=fun)
	ret = ril_request_set_phone_functionality(RIL_EXECUTE_MODE,fun,RIL_OMITTED_INTEGER_PARAM,NULL,(void *)NULL);
	phone_func = fun;
	
	if(ret == 0)
	NET_DBG("################################## set phone cfun ############################# %d\r\n", ret );

	if(fun == 0)
	temp_info.registered_network_flag = 0;
	else if(fun == 1)
	temp_info.no_service_flag = 0;
	
}
void cfun_off_callback(void)
{
	if(sbit_cfun_off_timer!=NULL)			
	xTimerStop(sbit_cfun_off_timer,0); 
	set_phone_functionality(true);
}
void delay_cfun_OffOn(void)
{
	set_phone_functionality(false);
	if(sbit_cfun_off_timer == NULL)
    sbit_cfun_off_timer = xTimerCreate("cfun_off", 
    10000 / portTICK_PERIOD_MS, 
    pdTRUE, 
    (void *) 0, 
    cfun_off_callback);
    xTimerStart(sbit_cfun_off_timer,1);
}

int CaculateWeekDay(int y,int m, int d)
{
	int iWeek = 0;
    if(m==1||m==2) {
        m+=12;
        y--;
    }
    iWeek=(d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7;
	if(iWeek == 6)
	{
		iWeek = 0;
	}
	else
	{
		iWeek = iWeek+1;
	}
	return iWeek;
} 
extern unsigned int stk_Pedometer_GetCount(void);
void H10_translate_step_or_hart_data()
{
	int i=0;
	apb_proxy_status_t	 cmd_ret = APB_PROXY_STATUS_ERROR;
	hal_rtc_time_t curtime; 
	static int mark=0;
	uint8_t rtc_week = 0;								 
	int temp=0;


	hal_rtc_get_time(&curtime);//获取rtctime
	
	
	if(curtime.rtc_year+2000>=2018)
	{
		temp = stk_Pedometer_GetCount();
		NVRAM_info.value_steps = temp + temp_info.step_cont_nvram;
		
		rtc_week = CaculateWeekDay(curtime.rtc_year+2000,curtime.rtc_mon,curtime.rtc_day);
		//if(NVRAM_info.stk8323_reset_mark==0)
		//NVRAM_info.stk8323_reset_mark = rtc_week;
	  
	  if((curtime.rtc_min==0)&&((curtime.rtc_sec==50)||(curtime.rtc_sec==51))&&(mark == 0))
		{
		  mark = 1;
		  sbit_m2m_ct_send_massege(M2M_CT_total_data_T50,NULL,0,0);
		}
	  else if(curtime.rtc_min==0)
		  mark = 0;

	  if((curtime.rtc_min==5)&&((curtime.rtc_sec==0)||(curtime.rtc_sec==1))&&(mark == 0))
		{
		  mark = 1;
		  NVRAM_info.total_steps[1]=NVRAM_info.value_steps-NVRAM_info.temp_step;
		  NVRAM_info.temp_step=NVRAM_info.value_steps;
		}
	  else if(curtime.rtc_min==5)
		  mark = 0;

	  if((curtime.rtc_min==10)&&((curtime.rtc_sec==0)||(curtime.rtc_sec==1))&&(mark == 0))
		{
		  mark = 1;
		  NVRAM_info.total_steps[2]=NVRAM_info.value_steps-NVRAM_info.temp_step;
		  NVRAM_info.temp_step=NVRAM_info.value_steps;
		}
	  else if(curtime.rtc_min==10)
		  mark = 0;

	  if((curtime.rtc_min==15)&&((curtime.rtc_sec==0)||(curtime.rtc_sec==1))&&(mark == 0))
		{
		  mark = 1;
		  NVRAM_info.total_steps[3]=NVRAM_info.value_steps-NVRAM_info.temp_step;
		  NVRAM_info.temp_step=NVRAM_info.value_steps;
		}
	  else if(curtime.rtc_min==15)
		  mark = 0;

	  if((curtime.rtc_min==20)&&((curtime.rtc_sec==0)||(curtime.rtc_sec==1))&&(mark == 0))
		{
		  mark = 1;
		  NVRAM_info.total_steps[4]=NVRAM_info.value_steps-NVRAM_info.temp_step;
		  NVRAM_info.temp_step=NVRAM_info.value_steps;
		}
	  else if(curtime.rtc_min==20)
		  mark = 0;

	  if((curtime.rtc_min==25)&&((curtime.rtc_sec==0)||(curtime.rtc_sec==1))&&(mark == 0))
		{
		  mark = 1;
		  NVRAM_info.total_steps[5]=NVRAM_info.value_steps-NVRAM_info.temp_step;
		  NVRAM_info.temp_step=NVRAM_info.value_steps;
		}
	  else if(curtime.rtc_min==25)
		  mark = 0;

	  if((curtime.rtc_min==30)&&((curtime.rtc_sec==0)||(curtime.rtc_sec==1))&&(mark == 0))
		{
		  mark = 1;
		  NVRAM_info.total_steps[6]=NVRAM_info.value_steps-NVRAM_info.temp_step;
		  NVRAM_info.temp_step=NVRAM_info.value_steps;
		}
	  else if(curtime.rtc_min==30)
		  mark = 0;

	  if((curtime.rtc_min==35)&&((curtime.rtc_sec==0)||(curtime.rtc_sec==1))&&(mark == 0))
		{
		  mark = 1;
		  NVRAM_info.total_steps[7]=NVRAM_info.value_steps-NVRAM_info.temp_step;
		  NVRAM_info.temp_step=NVRAM_info.value_steps;
		}
	  else if(curtime.rtc_min==35)
		  mark = 0;

	  if((curtime.rtc_min==40)&&((curtime.rtc_sec==0)||(curtime.rtc_sec==1))&&(mark == 0))
		{
		  mark = 1;
		  NVRAM_info.total_steps[8]=NVRAM_info.value_steps-NVRAM_info.temp_step;
		  NVRAM_info.temp_step=NVRAM_info.value_steps;
		}
	  else if(curtime.rtc_min==40)
		  mark = 0;
	  
	  if((curtime.rtc_min==45)&&((curtime.rtc_sec==0)||(curtime.rtc_sec==1))&&(mark == 0))
		{
		  mark = 1;
		  NVRAM_info.total_steps[9]=NVRAM_info.value_steps-NVRAM_info.temp_step;
		  NVRAM_info.temp_step=NVRAM_info.value_steps;
		}
	  else if(curtime.rtc_min==45)
		  mark = 0;
	  
	  if((curtime.rtc_min==50)&&((curtime.rtc_sec==0)||(curtime.rtc_sec==1))&&(mark == 0))
		{
		  mark = 1;
		  NVRAM_info.total_steps[10]=NVRAM_info.value_steps-NVRAM_info.temp_step;
		  NVRAM_info.temp_step=NVRAM_info.value_steps;
		}
	  else if(curtime.rtc_min==50)
		  mark = 0;
	  
	  if((curtime.rtc_min==55)&&((curtime.rtc_sec==0)||(curtime.rtc_sec==1))&&(mark == 0))
		{
		  mark = 1;
		  NVRAM_info.total_steps[11]=NVRAM_info.value_steps-NVRAM_info.temp_step;
		  NVRAM_info.temp_step=NVRAM_info.value_steps;
		}
	  else if(curtime.rtc_min==55)
		  mark = 0;
	  
		if(((curtime.rtc_hour==0)&&(curtime.rtc_min==0)&&((curtime.rtc_sec==1)||(curtime.rtc_sec==2)))
			||(rtc_week != NVRAM_info.stk8323_reset_mark))
		{
			NVRAM_info.stk8323_reset_mark = rtc_week;
			if(rtc_week==0)
			memset(NVRAM_info.total_steps,0,sizeof(NVRAM_info.total_steps));
			memset(NVRAM_info.step_week_log,0,sizeof(NVRAM_info.step_week_log));
			stkMotion_Clear_Pedometer_Value();
			temp_info.step_cont_nvram = 0;
			NVRAM_info.value_steps = 0;
			NVRAM_info.temp_step = 0;
	        for(i = 0; i < 12; i++)
	        {
		        NVRAM_info.total_steps[i] = 0;
	        }
			NVRAM_info.auto_wifi = 0;
			NVRAM_info.previous_steps = 0;
			NVRAM_info.exception_auto_reboot_num=0;
			NVRAM_info.move_time =0;
			NVRAM_info.sedentary_time =0;
			sbit_nvram_write();
		}

		NVRAM_info.step_week_log[rtc_week]=NVRAM_info.value_steps;
	}
	
}

void H10_send_hartblood_data_T45()
{
    char  blood_data_h[8];
	char  blood_data_l[8];
	char  hart_data[8];
	char time_buff[32] = {0};
	unsigned int utc_time;
	char total_buffer_str[256] = {0};
	
	//因我司欧洲有个重要客户需求，将N1的海外版固件在最后的版本N1_2022_0526基础上UI再将睡眠和血压屏蔽掉，
	//也不要检测和上报这两个数据，同时SOS启动时还是保留发定位信息，谢谢！2022_0804
	
	utc_time = ch_get_utc_sec();
	
	
	memset(time_buff, 0, sizeof(time_buff));
	sprintf(time_buff, "%lu", utc_time);
	
	memset(total_buffer_str, 0, sizeof(total_buffer_str));
	strcat(total_buffer_str,"{1:1:0:");	
	strcat(total_buffer_str,temp_info.build_time);	
	strcat(total_buffer_str,":");	
	strcat(total_buffer_str,temp_info.imei_num);	
	strcat(total_buffer_str,":");	
	strcat(total_buffer_str,"T45");	
	strcat(total_buffer_str,":");
	
#if 1
	memset(blood_data_h, 0, sizeof(blood_data_h));
//	sprintf(blood_data_h, "%d", temp_info.blood_lbp);	
	sprintf(blood_data_h, "%d", 0);
	
	strcat(total_buffer_str,blood_data_h);
	strcat(total_buffer_str,",");	

	memset(blood_data_l, 0, sizeof(blood_data_l));
//	sprintf(blood_data_l, "%d", temp_info.blood_hbp);
	sprintf(blood_data_l, "%d", 0);
	strcat(total_buffer_str,blood_data_l);
	strcat(total_buffer_str,",");
#endif

	memset(hart_data, 0, sizeof(hart_data));
	sprintf(hart_data, "%d", temp_info.hartrate_cnt);
	strcat(total_buffer_str,hart_data);
	strcat(total_buffer_str,",");	

	strcat(total_buffer_str,time_buff);
	strcat(total_buffer_str,"}");
	
	m2m_send_Queue_handle(total_buffer_str);
	
	temp_info.blood_lbp=0;
	temp_info.blood_hbp=0;
	temp_info.hartrate_cnt=0;

	NET_DBG("####################################T45#################################### %s",total_buffer_str);
}

#define WIFI_NUM_MAX	6
void Icare_wifi_send(char *buff)
{

	char*pch;
	int len,len1,len2,i,j;	
	uint8_t tmp_buf[128]={0};
	int ret;
  	static char QY_num_1[512]={0};
	static char QY_num_2[512]={0};
	static char QY_num_3[12]={'"'};
	hal_rtc_time_t curtime;	
 	static char total_buffer_str[1024]= {0};
	char time_buff[32] = {0};
   
	memset(QY_num_1,0,sizeof(QY_num_1));
	memset(QY_num_2,0,sizeof(QY_num_2));


 	

	memset(time_buff, 0, sizeof(time_buff));
	sprintf(time_buff, "%lu", ch_get_utc_sec());

/*
	{1:1:0:232322: 39238239329:T67 : 1，22:27:1d:20:08:d5&-55|5c:63:bf:a4:bf:56&-86,434343}
	{1:1:0:232322: 39238239329:T29:1,3,22:27:1d:20:08:d5&-55|5c:63:bf:a4:bf:56&-86,434343}
*/

	for(j=0;j < WIFI_NUM_MAX;j++)
	 {
	  if((strpbrk (buff,"("))!=NULL)
		{
			pch=strstr(buff,"(" );
			*pch++='\0';
			memset(total_buffer_str,0,sizeof(total_buffer_str));
			strcat(total_buffer_str,pch);

			if((strpbrk (total_buffer_str,QY_num_3))!=NULL)
			 {
				pch=strstr(total_buffer_str,QY_num_3 );
				*pch++='\0';
				memset(buff,0,sizeof(buff));
				strcat(buff,pch);

				if((strpbrk (buff,QY_num_3))!=NULL)
				 {
					pch=strstr(buff,QY_num_3 );
					*pch++='\0';
					memset(total_buffer_str,0,sizeof(total_buffer_str));
					strcat(total_buffer_str,pch);

					if((strpbrk (total_buffer_str,","))!=NULL)
					 {
						pch=strstr(total_buffer_str,"," );
						*pch++='\0';
						memset(buff,0,sizeof(buff));
						strcat(buff,pch);

						if((strpbrk (buff,","))!=NULL)
						 {
							pch=strstr(buff,"," );
							*pch++='\0';
							memset(total_buffer_str,0,sizeof(total_buffer_str));
							memset(QY_num_1,0,sizeof(QY_num_1));
							strcpy(QY_num_1,buff);/* 信号强度*/
							strcat(total_buffer_str,pch);


							if((strpbrk (total_buffer_str,QY_num_3))!=NULL)
							 {
								pch=strstr(total_buffer_str,QY_num_3 );
								*pch++='\0';
								memset(buff,0,sizeof(buff));
								strcat(buff,pch);


								if((strpbrk (buff,QY_num_3))!=NULL)
								 {
									pch=strstr(buff,QY_num_3 );
									*pch++='\0';
									memset(total_buffer_str,0,sizeof(total_buffer_str));
									strcat(QY_num_2,buff);
									strcat(QY_num_2,"&");
									strcat(QY_num_2,QY_num_1);
									strcat(QY_num_2,"&|");
									strcat(total_buffer_str,pch);

									memset(buff,0,sizeof(buff));
									strcat(buff,total_buffer_str);
 								 }

							 }

						 }

					 }

				 }

			 }
	    }

 		if(strlen(buff)<5)
		break;
	 }

	if(strlen(QY_num_2)==0)  /* WIFI 数据解析失败, 调用LBS数据*/
	{
		return;
	}

	memset(QY_num_1,0,sizeof(QY_num_1));
	strncpy(QY_num_1,QY_num_2,(strlen(QY_num_2)-1));
		
	i=((strlen(QY_num_1))/100)+1;
	
	for(j=0;j<i;j++)
	{
		memset(tmp_buf,0,sizeof(tmp_buf));
		strncpy(tmp_buf,QY_num_1+(100*j),100);
		NET_DBG(" wifi list buff   %s : \r\n",tmp_buf );
	}
	NET_DBG(" wifi list len   %d \r\n",strlen(QY_num_1));


	memset(total_buffer_str,0,sizeof(total_buffer_str));
	strcat(total_buffer_str,"{1:1:0:");	
	strcat(total_buffer_str,temp_info.build_time);	
	strcat(total_buffer_str,":");	
	strcat(total_buffer_str,temp_info.imei_num);	

	if(temp_info.sos_mark)
	{
		strcat(total_buffer_str,":T29:7,"); //SOS WIFI数据
		ctiot_lwm2m_client_send_response("Icare_wifi_send:T29:7");
	}
	else
	{
		strcat(total_buffer_str,":T29:3,"); //实时WIFI数据
		ctiot_lwm2m_client_send_response("Icare_wifi_send:T29:3");
	}

	
	
	strcat(total_buffer_str,QY_num_1);
	strcat(total_buffer_str,",");
	strcat(total_buffer_str,time_buff);
	strcat(total_buffer_str,"}");

	m2m_send_Queue_handle(total_buffer_str);
	
	memset(total_buffer_str,0,sizeof(total_buffer_str));
}

void H10_send_weather_data_T47(void)
{

	char*pch;
	int len,len1,len2,i,j;	
	uint8_t tmp_buf[128]={0};
	int ret;
	static char QY_num_1[1024]={0};
	static char QY_num_2[1024]={0};
	static char QY_num_3[12]={'"'};
	hal_rtc_time_t curtime; 
	char total_buffer_str[1024]= {0};
	char time_buff[32] = {0};
	unsigned int utc_time;

	char *buff = temp_info.total_buffer_wlap;
	
	return;
	if(strlen(buff) != NULL)
	{
		memset(QY_num_1,0,sizeof(QY_num_1));
		memset(QY_num_2,0,sizeof(QY_num_2));

		memset(time_buff, 0, sizeof(time_buff));
		sprintf(time_buff, "%lu", ch_get_utc_sec());

	/*
		{1:1:0:232322: 39238239329:T47:1,3,22:27:1d:20:08:d5&-55|5c:63:bf:a4:bf:56&-86,434343}
	*/

		for(j=0;j < 8;j++)
		 {
		  if((strpbrk (buff,"("))!=NULL)
			{
				pch=strstr(buff,"(" );
				*pch++='\0';
				memset(total_buffer_str,0,sizeof(total_buffer_str));
				strcat(total_buffer_str,pch);


				if((strpbrk (total_buffer_str,QY_num_3))!=NULL)
				 {
					pch=strstr(total_buffer_str,QY_num_3 );
					*pch++='\0';
					memset(buff,0,sizeof(buff));
					strcat(buff,pch);

					if((strpbrk (buff,QY_num_3))!=NULL)
					 {
						pch=strstr(buff,QY_num_3 );
						*pch++='\0';
						memset(total_buffer_str,0,sizeof(total_buffer_str));
						strcat(total_buffer_str,pch);

						if((strpbrk (total_buffer_str,","))!=NULL)
						 {
							pch=strstr(total_buffer_str,"," );
							*pch++='\0';
							memset(buff,0,sizeof(buff));
							strcat(buff,pch);

							if((strpbrk (buff,","))!=NULL)
							 {
								pch=strstr(buff,"," );
								*pch++='\0';
								memset(total_buffer_str,0,sizeof(total_buffer_str));
								memset(QY_num_1,0,sizeof(QY_num_1));
								strcpy(QY_num_1,buff);/* 信号强度*/
								strcat(total_buffer_str,pch);


								if((strpbrk (total_buffer_str,QY_num_3))!=NULL)
								 {
									pch=strstr(total_buffer_str,QY_num_3 );
									*pch++='\0';
									memset(buff,0,sizeof(buff));
									strcat(buff,pch);


									if((strpbrk (buff,QY_num_3))!=NULL)
									 {
										pch=strstr(buff,QY_num_3 );
										*pch++='\0';
										memset(total_buffer_str,0,sizeof(total_buffer_str));
										strcat(QY_num_2,buff);
										strcat(QY_num_2,"&");
										strcat(QY_num_2,QY_num_1);
										strcat(QY_num_2,"&|");
										strcat(total_buffer_str,pch);

										memset(buff,0,sizeof(buff));
										strcat(buff,total_buffer_str);

									 }

								 }

							 }

						 }

					 }

				 }
			}

			if(strlen(buff)<5)
			break;
		 }

		if(strlen(QY_num_2)==0)  /* WIFI 数据解析失败, 调用LBS数据*/
		{
			return;
		}


		memset(QY_num_1,0,sizeof(QY_num_1));
		strncpy(QY_num_1,QY_num_2,(strlen(QY_num_2)-1));
			
		i=((strlen(QY_num_1))/100)+1;
		
		for(j=0;j<i;j++)
		{
			memset(tmp_buf,0,sizeof(tmp_buf));
			strncpy(tmp_buf,QY_num_1+(100*j),100);
			NET_DBG(" wifi list buff   %s : \r\n",tmp_buf );
		}
		NET_DBG(" wifi list len   %d \r\n",strlen(QY_num_1));


		memset(total_buffer_str,0,sizeof(total_buffer_str));
		strcat(total_buffer_str,"{1:1:0:"); 
		strcat(total_buffer_str,temp_info.build_time);	
		strcat(total_buffer_str,":");	
		strcat(total_buffer_str,temp_info.imei_num);	
		//strcat(total_buffer_str,":T47:8,");
		strcat(total_buffer_str,QY_num_1);
		strcat(total_buffer_str,",");
		strcat(total_buffer_str,time_buff);
		strcat(total_buffer_str,"}");

		m2m_send_Queue_handle(total_buffer_str);
		ctiot_lwm2m_client_send_response(total_buffer_str);
	}
	
}


void H10_send_total_data_T50()
{
	int i = 0,POINT_MAX=12;
	hal_rtc_time_t curtime; 
	char time_buff[32] = {0};
	char bat[4]={0}; 
	char Latitude[20]={0};
	char Longitude[20]={0};
	int temp = 0 , test = 0;
	static int Total_steps=0;
	char num_to_str[8] = {0};
	char total_buffer_str[256] = {0};
	
	
	
	memset(time_buff, 0, sizeof(time_buff));
	memset(num_to_str, 0, sizeof(num_to_str));
	memset(Latitude, 0, sizeof(Latitude));
	memset(Longitude, 0, sizeof(Longitude));
	memset(total_buffer_str, 0, sizeof(total_buffer_str));
	
	sprintf(time_buff, "%lu", ch_get_utc_sec());
	
	strcat(total_buffer_str,"{1:1:0:");	
	
	if(strlen(temp_info.sim_num)>5)
	strcat(total_buffer_str,temp_info.sim_num);	
	else if(strlen(temp_info.imsi_num)>5)
	strcat(total_buffer_str,temp_info.imsi_num);	
	else
	strcat(total_buffer_str,"232322");	
	
	strcat(total_buffer_str,":");	
	strcat(total_buffer_str,temp_info.imei_num); 
	/*
 	   1|1|3|4|5|6|7|8|9|10|11|12|0:  
 	   1为当前手表显示计步总数、
 	   1为终端是否佩戴（1是佩戴、0为未佩戴）、
 	   3为终端一天累计久坐时间、
 	   4为终端一天累计活动时间、
 	   5为睡眠监测浅睡累计时间，
 	   6为睡眠监测深睡累计时间，
 	   7为睡眠监测翻身次数。
	   8为卡路里、单位为千卡，
	   9为距离、单位为米。
	*/
	strcat(total_buffer_str,":T50:");	
	
	memset(num_to_str, 0, sizeof(num_to_str));
	sprintf(num_to_str, "%d",NVRAM_info.temp_step);
	strcat(total_buffer_str,num_to_str); /*为当前手表显示计步总数*/	
	strcat(total_buffer_str,"|");	

	if(temp_info.watch_state <= 300) 
	strcat(total_buffer_str,"1"); 
	else
	strcat(total_buffer_str,"0"); 
	strcat(total_buffer_str,"|");	/*为终端是否佩戴（1是佩戴、0为未佩戴）*/
	
	memset(num_to_str, 0, sizeof(num_to_str));
	sprintf(num_to_str, "%d",NVRAM_info.sedentary_time);
	strcat(total_buffer_str,num_to_str); /*为终端一天累计久坐时间*/	
	strcat(total_buffer_str,"|");

	memset(num_to_str, 0, sizeof(num_to_str));
	sprintf(num_to_str, "%d",NVRAM_info.move_time);
	strcat(total_buffer_str,num_to_str); /*为终端一天累计活动时间*/	
	strcat(total_buffer_str,"|");
	
	//因我司欧洲有个重要客户需求，将N1的海外版固件在最后的版本N1_2022_0526基础上UI再将睡眠和血压屏蔽掉，
	//也不要检测和上报这两个数据，同时SOS启动时还是保留发定位信息，谢谢！2022_0804

#if 0	
	if((NVRAM_info.deep_sleep_time+NVRAM_info.light_sleep_time)>180)
	
	{
	    int sleep = 0;
		memset(num_to_str, 0, sizeof(num_to_str));
		sleep = (NVRAM_info.deep_sleep_time*1.2);
		sprintf(num_to_str, "%d",sleep);
		strcat(total_buffer_str,num_to_str); /*为睡眠监测浅睡累计时间*/ 
		strcat(total_buffer_str,"|");
		
		memset(num_to_str, 0, sizeof(num_to_str));
		sleep = (NVRAM_info.light_sleep_time*1.2);
		sprintf(num_to_str, "%d",sleep);
		strcat(total_buffer_str,num_to_str); /*为睡眠监测深睡累计时间*/ 
		strcat(total_buffer_str,"|");
		memset(num_to_str, 0, sizeof(num_to_str));
		sprintf(num_to_str, "%d",NVRAM_info.rolling_cumulative);
		strcat(total_buffer_str,num_to_str); /*翻身次数*/ 
		strcat(total_buffer_str,"|");
	}
	else

#endif
	{
		strcat(total_buffer_str,"0|0|0|");
	}


	
	test = (65*NVRAM_info.value_steps*0.60*1.036)/1000;
	memset(num_to_str, 0, sizeof(num_to_str));
	sprintf(num_to_str, "%d",test);
	strcat(total_buffer_str,num_to_str); /*为卡路里、单位为千卡*/ 
	strcat(total_buffer_str,"|");
	
	test = (NVRAM_info.value_steps*0.6);
	memset(num_to_str, 0, sizeof(num_to_str));
	sprintf(num_to_str, "%d",test);
	strcat(total_buffer_str,num_to_str); /*为距离、单位为米*/ 

	strcat(total_buffer_str,"|10|11|12|0,");

	//steps
	Total_steps = 0;
	for(i = 0; i < POINT_MAX; i++)
	{
		Total_steps = Total_steps + NVRAM_info.total_steps[i];
		memset(num_to_str, 0, sizeof(num_to_str));
		sprintf(num_to_str, "%d",NVRAM_info.total_steps[i]);
		
		strcat(total_buffer_str,num_to_str);
		strcat(total_buffer_str,"|");
	}
	memset(num_to_str, 0, sizeof(num_to_str));
	sprintf(num_to_str, "%d",Total_steps);
	strcat(total_buffer_str,num_to_str);
	strcat(total_buffer_str,",");
	strcat(total_buffer_str,"0|0|0|0|0|0|0|0|0|0|0|0|0,0|0|0|0|0|0|0|0|0|0|0|0|0,0.000000|0.000000");
	strcat(total_buffer_str,",");

	//heart
	for(i = 0; i < POINT_MAX; i++)
	{
		memset(num_to_str, 0, sizeof(num_to_str));
		sprintf(num_to_str, "%d",temp_info.hartrate_cnt);
		strcat(total_buffer_str,num_to_str);
		if(i == 11)
		{
			strcat(total_buffer_str,",");
		}
		else
		{
			strcat(total_buffer_str,"|");
		}
	}
	
	temp_info.hartrate_cnt = 0;
	sprintf(bat, "%d",temp_info.batterr_data);	 
	strcat(total_buffer_str,bat);		 
	strcat(total_buffer_str,",");
	strcat(total_buffer_str,time_buff);
	strcat(total_buffer_str,"}");
	
	m2m_send_Queue_handle(total_buffer_str);
	
	NVRAM_info.total_steps[0]=NVRAM_info.value_steps-NVRAM_info.temp_step;
	NVRAM_info.temp_step=NVRAM_info.value_steps;
	
	NET_DBG("####################################T50#################################### %s",total_buffer_str);
	
}

void H10_heart_bag_T51(void)
{
  /*
            T51 {1:1:0:0:39238239329:T51:4,0,1,79,98,434344}
	   
	   39238239329 : imei
	   1 充电状态: 0未充电 1充电中
	   0 佩戴状态: 0否 1戴
	   1 在线状态: 0否 1是 
	   98 电量
	   79：终端NB信号值RSRP
	   434344：utc时间戳
   */
    hal_rtc_time_t curtime;
	char sent_buff[400] = {0};
    char time_buff[200] = {0};
	char bat[4]={0}; 
    int charge_state = 0,Wear_state=0,network_state=1;
	
	memset(sent_buff, 0, sizeof(sent_buff));
	memset(time_buff, 0, sizeof(time_buff));
	
	


	
	if(temp_info.watch_state <= 300) 
	Wear_state = 1;
	else
	Wear_state = 0;
	

	
	if(temp_info.usb_connect_flag==1)
	   {
	    charge_state = 1;
		Wear_state = 0;
		
	   }
	else
	   charge_state = 0;	

    if(temp_info.registered_network_flag == 0)
    network_state = 0;

	sprintf(bat, "%d",temp_info.batterr_data);	 
	
    if(temp_info.usb_connect_flag==1)
	sprintf(time_buff,"%lu", ch_get_utc_sec());
	
	strcat(sent_buff,"{1:1:0:0:");
	strcat(sent_buff,temp_info.imei_num);
    sprintf(time_buff,":T51:%d,%d,%d,%s,%d,%lu}",charge_state,Wear_state,network_state,bat,temp_info.csq_num,ch_get_utc_sec());//充电状态.佩戴状态.在线状态.NB信号.电量.utc时间戳
	strcat(sent_buff,time_buff);  
    
    NET_DBG("H10_heart_bag_T51: %s",sent_buff);
	m2m_send_Queue_handle(sent_buff);
}

char t53_sent_buff[400] = {0};
char* H10_synctime_T53(void)
{

   /*{1:1:0:232322:111100000030019:T53:1,16654}16654: 发送报文时的时间戳*/
    return;
    char time_buff[20] = {0};
	char bat[4]={0}; 
    int charge_state = 0,Wear_state=0,network_state=1;
	
	memset(t53_sent_buff, 0, sizeof(t53_sent_buff));
	memset(time_buff, 0, sizeof(time_buff));

   
    strcat(t53_sent_buff,"{1:1:0:232322:");
	strcat(t53_sent_buff,temp_info.imei_num);
	strcat(t53_sent_buff,":T53:1,");
    sprintf(time_buff,"%lu", ch_get_utc_sec());
	strcat(t53_sent_buff,time_buff);
    strcat(t53_sent_buff,"}");
    m2m_send_Queue_handle(t53_sent_buff);
    NET_DBG("H10_synctime_T53: %d",Gutc_time);
    ctiot_lwm2m_client_send_response(t53_sent_buff);

	return t53_sent_buff;
}

TimerHandle_t Reset_timer = NULL;

void H10_Factory_Reset_flag(void)
{
   NVRAM_info.Factory_Reset_flag = 0;
   
   NVRAM_info.value_steps = 0;
   NVRAM_info.previous_steps = 0;
   NVRAM_info.temp_step = 0;

   

   NVRAM_info.sedentary_flag = 0;
   NVRAM_info.Factory_Reset_flag = 0;
   NVRAM_info.English_German_flag = 0;
   NVRAM_info.data_syn_flag = 0;
   NVRAM_info.move_time = 0;
   NVRAM_info.sedentary_time = 0;

   NVRAM_info.location_interval = 0;
   NVRAM_info.rolling_cumulative = 0;
   NVRAM_info.light_sleep_time = 0;
   NVRAM_info.deep_sleep_time = 0;
   NVRAM_info.sleep_reset_flag = 0;
   NVRAM_info.movement_flag = 0;

   memset(NVRAM_info.waggle_level,0,12*sizeof(NVRAM_info.waggle_level));
   memset(NVRAM_info.blood_hbp,0,sizeof(NVRAM_info.blood_hbp));
   memset(NVRAM_info.blood_lbp,0,sizeof(NVRAM_info.blood_lbp));
   
   strcpy(NVRAM_info.blood_hbp,"120");
   strcpy(NVRAM_info.blood_lbp,"75");
   
   memset(NVRAM_info.Hr_log,0,24);
   memset(NVRAM_info.step_week_log,0,7*sizeof(NVRAM_info.step_week_log));
   memset(NVRAM_info.total_steps,0,13*sizeof(NVRAM_info.total_steps));
   
   temp_info.hartrate_cnt = 0;
   temp_info.spo2_cnt = 0;
   temp_info.blood_lbp = 0;
   temp_info.blood_hbp = 0;
   memset(temp_info.MY_Latitude,0,sizeof(temp_info.MY_Latitude));
   memset(temp_info.MY_Longitude,0,sizeof(temp_info.MY_Longitude));
   NVRAM_info.Bright_screen_flag = 1;
   NVRAM_info.step_interval = 500;
   NVRAM_info.step_intervallogo = 0;
   temp_info.step_cont_nvram = 0;
   NET_DBG("H10_Factory_Reset_flag");
   sbit_nvram_write();
}


void H10_Factory_Reset_T52(void)
{
   /*T52 {1:1:0:232322:39238239329:T52:1,434344}
            39238239329 : imei
           1 : 恢复出厂设置标志 0-未恢复出厂设置
          434344 : utc时间戳
     */
   char sent_buff[200] = {0};
   char time_buff[32] = {0};

   
   memset(sent_buff, 0, sizeof(sent_buff));
   memset(time_buff, 0, sizeof(time_buff));
   
   sprintf(time_buff, "%lu", ch_get_utc_sec());

   strcat(sent_buff,"{1:1:0:");
   strcat(sent_buff,temp_info.build_time);  
   strcat(sent_buff,":");
   strcat(sent_buff,temp_info.imei_num);
   strcat(sent_buff,":T52:1,");
   strcat(sent_buff,time_buff);
   strcat(sent_buff,"}");
   
   clear_medicine_remind();
   NVRAM_info.Factory_Reset_flag = 1;
   if(Reset_timer == NULL)
	{
		Reset_timer = xTimerCreate("Reset_timer",3*1000/portTICK_PERIOD_MS, pdFALSE, (void *)0, H10_Factory_Reset_flag);
	}
   xTimerStart(Reset_timer,1);
  
   m2m_send_Queue_handle(sent_buff);

}

void H10_send_gps_data_T29(void)
{
	static char Latitude[20]={0};
	static char Longitude[20]={0};
	static char time_buff[32] = {0};
	static char total_buffer_str[256]={0};
	
	/*{1:1:0:232322: 39238239329:T29 : 4，23.3333|323.23232332,434343}*/
	
	
	
	memset(Latitude, 0, sizeof(Latitude));
	memset(Longitude, 0, sizeof(Longitude));
	memset(time_buff, 0, sizeof(time_buff));
	memset(total_buffer_str, 0, sizeof(total_buffer_str));
	
	strcat(total_buffer_str,"{1:1:0:");	
	strcat(total_buffer_str,temp_info.build_time);	
	strcat(total_buffer_str,":");
	strcat(total_buffer_str,temp_info.imei_num);
	if(temp_info.sos_mark || temp_info.gps_sos_mark)
	{
        temp_info.gps_sos_mark = 0;
		strcat(total_buffer_str,":T29:6,"); //SOS 定位数据
	}
	else
	{
		strcat(total_buffer_str,":T29:4,"); //实时GPS数据
	}
	
	memset(Latitude, 0, sizeof(Latitude));
    Latitude_change(temp_info.MY_Latitude,Latitude);
	strcat(total_buffer_str,Latitude);
	memset(temp_info.MY_Latitude,0,sizeof(temp_info.MY_Latitude));

	
	strcat(total_buffer_str,"|");	
	
    memset(Longitude, 0, sizeof(Longitude));
    Longitude_change(temp_info.MY_Longitude,Longitude);
	strcat(total_buffer_str,Longitude);
	memset(temp_info.MY_Longitude,0,sizeof(temp_info.MY_Longitude));
	
	strcat(total_buffer_str,",");	
	
	memset(time_buff, 0, sizeof(time_buff));
	sprintf(time_buff, "%lu", ch_get_utc_sec());
	strcat(total_buffer_str,time_buff);
	strcat(total_buffer_str,"}");	
	
	m2m_send_Queue_handle(total_buffer_str);
}

void H10_send_sos_data_T0(void)
{
	static char Latitude[20]={0};
	static char Longitude[20]={0};
	static char time_buff[32] = {0};
	static char total_buffer_str[256]={0};
	
	memset(Latitude, 0, sizeof(Latitude));
	memset(Longitude, 0, sizeof(Longitude));
	memset(time_buff, 0, sizeof(time_buff));
	memset(total_buffer_str, 0, sizeof(total_buffer_str));
	
	strcat(total_buffer_str,"{T0,");	
	strcat(total_buffer_str,temp_info.imei_num);	
	strcat(total_buffer_str,",[");	
	
	if(strlen(temp_info.MY_Latitude) == 0)
	{
		strcat(total_buffer_str,"0.000000");
	}
	else
	{
		memset(Latitude, 0, sizeof(Latitude));
	    Latitude_change(temp_info.MY_Latitude,Latitude);
		strcat(total_buffer_str,Latitude);
		memset(temp_info.MY_Latitude,0,sizeof(temp_info.MY_Latitude));
	}
	strcat(total_buffer_str,";");	
	if(strlen(temp_info.MY_Longitude) == 0)
	{
		strcat(total_buffer_str,"0.000000");
	}
	else
	{		
	    memset(Longitude, 0, sizeof(Longitude));
	    Longitude_change(temp_info.MY_Longitude,Longitude);
		strcat(total_buffer_str,Longitude);
		memset(temp_info.MY_Longitude,0,sizeof(temp_info.MY_Longitude));
	}
	
	strcat(total_buffer_str,";");	
	memset(time_buff, 0, sizeof(time_buff));
	sprintf(time_buff,"%d",temp_info.hartrate_cnt);
	strcat(total_buffer_str,time_buff);
	strcat(total_buffer_str,";");	
	
	memset(time_buff, 0, sizeof(time_buff));
	sprintf(time_buff, "%lu", ch_get_utc_sec());
	strcat(total_buffer_str,time_buff);
	strcat(total_buffer_str,"]}");	
	
	m2m_send_Queue_handle(total_buffer_str);
	
	ctiot_lwm2m_client_send_response(total_buffer_str);
	NET_DBG("####################################T0#################################### %s",total_buffer_str);
	
}

void H10_send_gettime_data()
{
	char total_buffer_str[20] = {0};
	
	memset(total_buffer_str, 0, sizeof(total_buffer_str));
	strcat(total_buffer_str,"[GETTIME]");	
	m2m_send_Queue_handle(total_buffer_str);
	NET_DBG("####################################temp_info.data_buffer_hex#################################### %s",temp_info.data_buffer_hex);
}

void H10_send_T71_data()
{
	char total_buffer_str[128] = {0};
	static char time_buff[128] = {0};
	return;
	
	memset(total_buffer_str, 0, sizeof(total_buffer_str));
	strcat(total_buffer_str,"{1:1:0:");	
	
	if(strlen(temp_info.sim_num)>5)
	strcat(total_buffer_str,temp_info.sim_num);	
	else if(strlen(temp_info.imsi_num)>5)
	strcat(total_buffer_str,temp_info.imsi_num);	
	else
	strcat(total_buffer_str,"232322");	
	strcat(total_buffer_str,":");	
	
	strcat(total_buffer_str,temp_info.imei_num);	
	strcat(total_buffer_str,":T71,");
	
	memset(time_buff, 0, sizeof(time_buff));
	sprintf(time_buff, "%lu", ch_get_utc_sec());
	strcat(total_buffer_str,time_buff);
	strcat(total_buffer_str,"}");	
	
	m2m_send_Queue_handle(total_buffer_str);
	NET_DBG("####################################H10_send_T71_data#################################### %s",temp_info.data_buffer_hex);
	
}
void H10_send_T15_ack()
{	
	static char time_buff[32] = {0};
	static char total_buffer_str[256] = {0};

	memset(total_buffer_str, 0, sizeof(total_buffer_str));
	strcat(total_buffer_str,"{1:1:0:0:");	
	strcat(total_buffer_str,temp_info.imei_num);
	strcat(total_buffer_str,":T15:");
	memset(time_buff, 0, sizeof(time_buff));
	sprintf(time_buff, "%lu", ch_get_utc_sec());
	strcat(total_buffer_str,time_buff);
	strcat(total_buffer_str,"}");	
	ctiot_lwm2m_client_send_response(total_buffer_str);
	m2m_send_Queue_handle(total_buffer_str);
}
void H10_send_hartrate_data_T28(void)
{

	static char heart_buff[16] = {0};
	static char time_buff[32] = {0};
	int i = 0, heart_num = 12;
	static char total_buffer_str[256] = {0};
	
	
	memset(time_buff, 0, sizeof(time_buff));
	memset(heart_buff, 0, sizeof(heart_buff));
	memset(total_buffer_str, 0, sizeof(total_buffer_str));
	
	sprintf(time_buff, "%lu", ch_get_utc_sec());
	
	strcat(total_buffer_str,"{1:1:0:");	
	if(strlen(temp_info.iccid)>5)
	strcat(total_buffer_str,temp_info.iccid);	
	else
	strcat(total_buffer_str,"232322");	
	strcat(total_buffer_str,":");	
	strcat(total_buffer_str,temp_info.imei_num);	
	strcat(total_buffer_str,":T28:1,");	
	
	for(i = 0; i < heart_num; i++)
	{
	    if(strlen(heart_buff)==0)
	    {
			memset(heart_buff,0,sizeof(heart_buff));
			sprintf(heart_buff,"%d",temp_info.hartrate_cnt);
		}
		
		strcat(total_buffer_str,heart_buff);
		if(i == 11)
		{
			strcat(total_buffer_str,",");
		}
		else
		{
			strcat(total_buffer_str,"|");
		}
	}
	strcat(total_buffer_str,time_buff);
	strcat(total_buffer_str,"}");	
	
	temp_info.hartrate_cnt=0;
	m2m_send_Queue_handle(total_buffer_str);
	NET_DBG("####################################T28#################################### %s",total_buffer_str);
	
	
}

void H10_send_data_T101(void)
{
	static char heart_buff[16] = {0};
	static char time_buff[32] = {0};
	int i = 0, heart_num = 12;
	static char total_buffer_str[256] = {0};
	/*
	3.5、终端上报运动状态（中金软件版本）
	3.5.1、终端上报手表模式状态指令
	格式：{1:1:0:232322::T101:100,60,1,3.5,1800,1300,113.123456|22.56789,45} 
	*/
	
	
	
	
	memset(heart_buff, 0, sizeof(heart_buff));
	memset(total_buffer_str, 0, sizeof(total_buffer_str));
	
	strcat(total_buffer_str,"{1:1:0:");	
	strcat(total_buffer_str,temp_info.build_time);	
	strcat(total_buffer_str,":");	
	strcat(total_buffer_str,temp_info.imei_num);
	
	strcat(total_buffer_str,":T101:100,60,1,3.5,1800,1300,113.123456|22.56789,45");	
	
	strcat(total_buffer_str,"&");
	memset(time_buff, 0, sizeof(time_buff));
	sprintf(time_buff, "%lu", ch_get_utc_sec());
	strcat(total_buffer_str,time_buff);
	strcat(total_buffer_str,"}");	
	
	m2m_send_Queue_handle(total_buffer_str);
	
	NET_DBG("####################################T101#################################### %s",total_buffer_str);
	
}

void H10_send_data_T100(void)
{
	static char heart_buff[16] = {0};
	static char time_buff[32] = {0};
	int i = 0, heart_num = 12;
	static char total_buffer_str[256] = {0};
	
	/*
	3.4.1、终端上报手表模式状态指令 
	报文类型：T100 
	格式：{1:1:0:232322:39238239329:T100:1} 
	描述： 1 为正常模式、2为省电模式。
	命令: T100
	*/
	
	
	memset(heart_buff, 0, sizeof(heart_buff));
	memset(total_buffer_str, 0, sizeof(total_buffer_str));
	
	strcat(total_buffer_str,"{1:1:0:");	
	strcat(total_buffer_str,temp_info.build_time);	
	strcat(total_buffer_str,":");	
	strcat(total_buffer_str,temp_info.imei_num);
	
	if(NVRAM_info.Power_saving_flag == 0)
	strcat(total_buffer_str,":T100:1,");	
	else if(NVRAM_info.Power_saving_flag == 1)
	strcat(total_buffer_str,":T100:2,");
	
	memset(time_buff, 0, sizeof(time_buff));
	sprintf(time_buff, "%lu", ch_get_utc_sec());
	strcat(total_buffer_str,time_buff);
	strcat(total_buffer_str,"}");	
	
	m2m_send_Queue_handle(total_buffer_str);
	
	NET_DBG("####################################T100#################################### %s",total_buffer_str);
	
}


void H10_send_ack_T86(void)
{
	//{1:1:0:232322:39238239329:T96:1}
	static char total_buffer_str[256] = {0};

	memset(total_buffer_str, 0, sizeof(total_buffer_str));
	strcat(total_buffer_str,"{1:1:0:232322:");	
	strcat(total_buffer_str,temp_info.imei_num);	
	strcat(total_buffer_str,":T86:");	
	if(NVRAM_info.medicine_set_flag == -1)
	{
		strcat(total_buffer_str,"0}");	
	}
	else if(NVRAM_info.medicine_set_flag == 1)
	{
		strcat(total_buffer_str,"1}");	
	}
	NET_DBG("=====H10_send_ack_T86:%d",NVRAM_info.medicine_set_flag);
	m2m_send_Queue_handle(total_buffer_str);
}

void H10_send_ack_T87(void)
{
	//?{1:1:0:232322:39238239329:T97:1}
	static char total_buffer_str[256] = {0};

	memset(total_buffer_str, 0, sizeof(total_buffer_str));
	strcat(total_buffer_str,"{1:1:0:232322:");	
	strcat(total_buffer_str,temp_info.imei_num);	
	strcat(total_buffer_str,":T87:");	
	strcat(total_buffer_str,"1}");	
	NET_DBG("=====H10_send_ack_T87");
	m2m_send_Queue_handle(total_buffer_str);
}

void m2m_send_Queue_handle(char *buff)
{
    int i=0;
	hal_rtc_time_t curtime;	
 
	
	NVRAM_info.network_fail_record = 0;
    #if 0
	hal_rtc_get_N1_utc_time(&curtime);
	if(curtime.rtc_year < 2020)
	return;
	#endif
	//for(i = 0; i < Queue_num; i++)	 
	//NET_DBG("+++++++++++++	temp_info.send_record_flag[%d] ++++++++++++++  %d",i,temp_info.send_record_flag[i]);

	if(((strstr(buff,"\r"))!=NULL)||((strstr(buff,"+CWLAP:"))!=NULL))
	{
		//NET_DBG("===================== \r =======================");
		return;
	}
	
	if(strlen(temp_info1.m2m_send_temp_Queue[(Queue_num-1)])>0)
	{
		for(i = 0; i < Queue_num; i++)	 
		{		 
			if(i == (Queue_num-1))  
			{
				memset(temp_info1.m2m_send_temp_Queue[i],0,sizeof(temp_info1.m2m_send_temp_Queue[i]));
			}
			else  
			{ 
				memset(temp_info1.m2m_send_temp_Queue[i],0,sizeof(temp_info1.m2m_send_temp_Queue[i]));
				strcat(temp_info1.m2m_send_temp_Queue[i],temp_info1.m2m_send_temp_Queue[i+1]);  
			}
		}	 
	}
	
	for(i=0;i<Queue_num;i++)
	{
	  if(strlen(temp_info1.m2m_send_temp_Queue[i])==0)
	   {
		  strcat(temp_info1.m2m_send_temp_Queue[i],buff);	
		  //NET_DBG("++++   temp_info1.m2m_send_temp_Queue[%d] +++++  %s",i,temp_info1.m2m_send_temp_Queue[i]);
		  break;
	   }
	}

}
int get_m2m_Queue_count(void)
{
    int i=0,num=0;
	
	for(i=0;i<Queue_num;i++)    //kyb add 先进先出
	{
		if(strlen(temp_info1.m2m_send_temp_Queue[i])>0)
		{
			//dbg_print("#################################### get_m2m_Queue_count [%d]#################################### %s",i,temp_info1.m2m_send_temp_Queue[i]);
			num++;
		}
	}

	return num;

}

signed short get_m2m_send_Queue(void)
{
    int i=0;
	for(i=0;i<Queue_num;i++) //sos报文优先级最高
	{
		if(strlen(temp_info1.m2m_send_temp_Queue[i])>0)
		{
			if(strstr(temp_info1.m2m_send_temp_Queue[i],"{T0,"))
			{				
				dbg_print("#################################### get_m2m_send_Queue [%d]#################################### %s",i,temp_info1.m2m_send_temp_Queue[i]);				
				return i ;
			}
		}
	}
	
	//for(i=(Queue_num-1);i>=0;i--)
	for(i=0;i<Queue_num;i++)    //kyb add 先进先出	
	{
		if(strlen(temp_info1.m2m_send_temp_Queue[i])>0)
		{
			return i ;
		}
	}

	return -1;
}

void sbit_debug_Queue(void)
{
    int i=0;

	ctiot_lwm2m_client_send_response("====sbit_debug_Queue begin");
	for(i = 0; i < Queue_num; i++)	
	{
		if(strlen(temp_info1.m2m_send_temp_Queue[i])>0)
		{
			ctiot_lwm2m_client_send_response(temp_info1.m2m_send_temp_Queue[i]);
		}
	}
	ctiot_lwm2m_client_send_response("====sbit_debug_Queue end");
}

#if defined(MTK_SEND_QUEUE_FIFO)
signed short get_m2m_send_Queue_Fifo(void)
{
	if(strlen(temp_info1.m2m_send_temp_Queue[0])>0)
	{
		return 0 ;
	}

	return -1;
}

void del_m2m_send_Queue_Fifo(void)
{
	int i = 0;

	for(i = 0; i < Queue_num-1;i++)
	{
		if(strlen(temp_info1.m2m_send_temp_Queue[i])>0)
		{
			memset(temp_info1.m2m_send_temp_Queue[i],0,sizeof(temp_info1.m2m_send_temp_Queue[i]));
		}
		else
		{
			break;
		}

		if(strlen(temp_info1.m2m_send_temp_Queue[i+1])>0)
		{
			strcat(temp_info1.m2m_send_temp_Queue[i],temp_info1.m2m_send_temp_Queue[i+1]);	
		}
	}
	
}
#endif
extern int T29_send_fail;
#define MESSAGE_LEN_MAX 128
uint16_t address_info[MESSAGE_LEN_MAX] = {0};
void ctiot_lwm2m_command_address(char *input_str, char *output_str)
{
	//{1:1:0:232322:111100000011209:S29:广东省 深圳市 宝安区 兴华一路 靠近华创达现代服务产业园}
	int i = 0;
	char *p0 = NULL;
	char *p1 = NULL;
	char *str_p = NULL;
	char *command_str = NULL;
	char dest_src[MESSAGE_LEN_MAX] = {0};
	char source_src[MESSAGE_LEN_MAX] = {0};
	uint16_t ucscode = 0x00; 
	
	
	
	if(strstr(input_str,"S29"))
	{
		command_str = strstr(input_str,"S29");
		p0 = strstr(command_str, ":");
		p1 = strstr(command_str, "}");
		
		#if 1
		if((p0 == NULL) || (p1 == NULL))
		{
			ctiot_lwm2m_client_send_response("S29 FAIL");
		}
		else
		{
			p0++;
			if((p1-p0) > MESSAGE_LEN_MAX)
			{
				memcpy(source_src,p0,MESSAGE_LEN_MAX);
			}
			else
			{
				memcpy(source_src,p0,(p1-p0));
			}
			str_p = source_src;

			#if 0//chinese
			mmi_chset_utf8_to_ucs2_string(dest_src, MESSAGE_LEN_MAX, str_p);
			memset(address_info, 0, sizeof(address_info));
			char_to_ucdigits(dest_src, address_info);
			#else//english
			memset(address_info, 0, sizeof(address_info));
			strcpy(address_info,source_src);
			#endif

			//temp_info.gps_delay = 20;
			dbg_print("address_info:%s,[%s]",source_src,dest_src);
			if((get_sbit_show_meun_id())==GPS_SCREEN)
			sbit_m2m_ct_send_massege_isr(M2M_DISPLAY_BACKLIGHT,0,0,1);
		}
		#endif
		T29_send_fail = 0; 
		i = get_m2m_send_Queue();
		if((i >= 0)&&(strstr(temp_info1.m2m_send_temp_Queue[i],":T29:3,") != NULL))
		{
			memset(temp_info1.m2m_send_temp_Queue[i],0,sizeof(temp_info1.m2m_send_temp_Queue[i]));
		}
		
	}

	
	
}


void ctiot_lwm2m_command_weather(char *input_str, char *output_str)
{
	//{1:1:0:232322:111100000011209:S47:24,28,多云,1,37,深圳}
	int i = 0;
	char *p0 = NULL;
	char *p1 = NULL;
	char *str_p = NULL;
	char *command_str = NULL;
	char dest_src[MESSAGE_LEN_MAX] = {0};
	char source_src[MESSAGE_LEN_MAX] = {0};
	uint16_t ucscode = 0x00; 
	//NET_DBG("============ctiot_lwm2m_command_weather:%s",input_str);
	if(strstr(input_str,"S47"))
	{
		command_str = strstr(input_str,"S47");
		p0 = strstr(command_str, ":");
		p1 = strstr(command_str, ",");
		if((p0 == NULL) || (p1 == NULL))
		{
			ctiot_lwm2m_client_send_response("S47 FAIL");
		}
		else
		{
			p0++;
			memset(source_src, 0, sizeof(source_src));
			if((p1-p0) > MESSAGE_LEN_MAX)
			{
				memcpy(source_src,p0,MESSAGE_LEN_MAX);
			}
			else
			{
				memcpy(source_src,p0,(p1-p0));
			}
			str_p = source_src;
			memset(dest_src, 0, sizeof(dest_src));
			mmi_chset_utf8_to_ucs2_string(dest_src, MESSAGE_LEN_MAX, str_p);
			memset(NVRAM_info.low_weather, 0, sizeof(NVRAM_info.low_weather));
			char_to_ucdigits(dest_src, NVRAM_info.low_weather);

			p0 = p1;
			p0++;
			p1 = strstr(p0, ",");
			if((p0 == NULL) || (p1 == NULL))
			{
				ctiot_lwm2m_client_send_response("S47 FAIL");
			}
			else
			{
				memset(source_src, 0, sizeof(source_src));
				memcpy(source_src,p0,(p1-p0));
				str_p = source_src;
				memset(dest_src, 0, sizeof(dest_src));
				mmi_chset_utf8_to_ucs2_string(dest_src, MESSAGE_LEN_MAX, str_p);
				memset(NVRAM_info.hight_weather, 0, sizeof(NVRAM_info.hight_weather));
				char_to_ucdigits(dest_src, NVRAM_info.hight_weather);

				p0 = p1;
				p0++;
				p1 = strstr(p0, ",");
				if((p0 == NULL) || (p1 == NULL))
				{
					ctiot_lwm2m_client_send_response("S47 FAIL");
				}
				else
				{
					memset(source_src, 0, sizeof(source_src));
					memcpy(source_src,p0,(p1-p0));
					str_p = source_src;
					memset(dest_src, 0, sizeof(dest_src));
					mmi_chset_utf8_to_ucs2_string(dest_src, MESSAGE_LEN_MAX, str_p);
					memset(NVRAM_info.weather_info, 0, sizeof(NVRAM_info.weather_info));
					char_to_ucdigits(dest_src,NVRAM_info.weather_info);

					p0 = p1;
					p0++;
					p1 = strstr(p0, ",");
					if((p0 == NULL) || (p1 == NULL))
					{
						ctiot_lwm2m_client_send_response("S47 FAIL");
					}
					else
					{
						memset(source_src, 0, sizeof(source_src));
						memcpy(source_src,p0,(p1-p0));
						str_p = source_src;
						memset(dest_src, 0, sizeof(dest_src));
						mmi_chset_utf8_to_ucs2_string(dest_src, MESSAGE_LEN_MAX, str_p);
						memset(NVRAM_info.weather_pic, 0, sizeof(NVRAM_info.weather_pic));
						char_to_ucdigits(dest_src, NVRAM_info.weather_pic);

						p0 = p1;
						p0++;
						p1 = strstr(p0, ",");
						if((p0 == NULL) || (p1 == NULL))
						{
							ctiot_lwm2m_client_send_response("S47 FAIL");
						}
						else
						{
							memset(source_src, 0, sizeof(source_src));
							memcpy(source_src,p0,(p1-p0));
							str_p = source_src;
							memset(dest_src, 0, sizeof(dest_src));
							mmi_chset_utf8_to_ucs2_string(dest_src, MESSAGE_LEN_MAX, str_p);
							memset(NVRAM_info.weather_pm25, 0, sizeof(NVRAM_info.weather_pm25));
							char_to_ucdigits(dest_src, NVRAM_info.weather_pm25);

							p0 = p1;
							p0++;
							p1 = strstr(p0, "}");
							if((p0 == NULL) || (p1 == NULL))
							{
								ctiot_lwm2m_client_send_response("S47 FAIL");
							}
							else
							{
								memset(source_src, 0, sizeof(source_src));
								memcpy(source_src,p0,(p1-p0));
								str_p = source_src;
								memset(dest_src, 0, sizeof(dest_src));
								mmi_chset_utf8_to_ucs2_string(dest_src, MESSAGE_LEN_MAX, str_p);
								memset(NVRAM_info.city_weather, 0, sizeof(NVRAM_info.city_weather));
								char_to_ucdigits(dest_src, NVRAM_info.city_weather);
							}
							
							temp_info.weather_updata_flag = 60*60*2;
						}
					}
				}
			}
		}
	}
}

void ctiot_lwm2m_command_handle(char *data_str)
{
	char str_buff[32] = {0};
	int str_len = 0;
	ctiot_lwm2m_client_send_response(data_str);
	ctiot_lwm2m_command_address(data_str, str_buff);
	ctiot_lwm2m_command_weather(data_str, str_buff);
}







