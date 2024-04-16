#include <stdio.h> 
#include <stdarg.h>
#include <stdint.h>

#include "ril.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "task_def.h"

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
#include "hal_sleep_manager.h"
#include "h10_mmi.h"
#include "atci_apb_proxy_adapter.h"
#include "hal_pmu.h"
#include "hal_wdt.h"
#include "type_def.h"
#include "m2m_timer.h"


Read_Write temp_info;
Read_Write1 temp_info1;
NVRAM_Read_Write	NVRAM_info;
NVRAM_Read_Write1	NVRAM_info1;
NVRAM_Read_Write2	NVRAM_info2;
NVRAM_Read_Write3	NVRAM_info3;
int show_imei_mark=0;
TimerHandle_t show_imei_mark_timer=NULL;
TimerHandle_t sbit_display_timer=NULL;
TimerHandle_t sbit_wtd_timer=NULL;
TimerHandle_t sbit_key_vib_timer=NULL;
TimerHandle_t sbit_key_timer=NULL;
TimerHandle_t sbit_shut_down_timer=NULL;
TimerHandle_t at_timer = NULL;
TimerHandle_t heart_bag_timer=NULL;

int SBIT_DISPLAY_BACKLIGHT_TIME  = 15;
int exception_auto_reboot_mark = 0;    /* 异常自动重启 标识*/
uint32_t UI_WIDTH,UI_HEIGHT;
uint8_t  sbit_deep_slee_lock = 0;
int imei_flag = 0;
int lan_swflag = 0;

extern void sleep_manager_dump_all_user_lock_sleep_handle(void);

ATTR_RWDATA_IN_NONCACHED_RAM_4BYTE_ALIGN uint8_t display_fb[GDI_LCD_WIDTH*GDI_LCD_HIGHT*2];

typedef struct
{
	unsigned int msg;
	void *param;
}display_msg_struct;


#define SBIT_STEPS_GROUP_NAME    "SBIT_STEPS_CFG"
#define SBIT_STEPS_ITEM_NAME    "SBIT_STEPS_NVRAM"

#define SBIT_STEPS_GROUP_NAME1    "SBIT_STEPS_CFG1"
#define SBIT_STEPS_ITEM_NAME1    "SBIT_STEPS_NVRAM1"

#define SBIT_STEPS_GROUP_NAME2    "SBIT_STEPS_CFG2"
#define SBIT_STEPS_ITEM_NAME2    "SBIT_STEPS_NVRAM2"

#define SBIT_STEPS_GROUP_NAME3    "SBIT_STEPS_CFG3"
#define SBIT_STEPS_ITEM_NAME3    "SBIT_STEPS_NVRAM3"


#define H10_VIBR_EN    HAL_GPIO_8
#define H10_WTD_EN     HAL_GPIO_24
#define H10_WTD_FEED   HAL_GPIO_25
log_create_module(sbit_demo, PRINT_LEVEL_INFO);

#define SBIT_ERR(fmt,arg...)   LOG_E(sbit_demo, "[sbit]: "fmt,##arg)
#define SBIT_WARN(fmt,arg...)  LOG_W(sbit_demo, "[sbit]: "fmt,##arg)
#define SBIT_DBG(fmt,arg...)   LOG_I(sbit_demo, "[sbit]: "fmt,##arg)

#define RGB888_RED      0x00ff0000
#define RGB888_GREEN    0x0000ff00
#define RGB888_BLUE     0x000000ff

extern bool clear_medicine_remind_flag;
extern char curr_remind_time_str[8];
extern int min_next_remin_num;
extern int medicine_remind_delay;
extern int medicine_lock_sec;
extern void gps_driver_init(void);

unsigned short RGB888ToRGB565(unsigned int n888Color)
{
	unsigned short n565Color = 0;

	// 获取RGB单色，并截取高位
	unsigned char cRed   = (n888Color & RGB888_RED)   >> 19;
	unsigned char cGreen = (n888Color & RGB888_GREEN) >> 10;
	unsigned char cBlue  = (n888Color & RGB888_BLUE)  >> 3;

	// 连接
	n565Color = (cRed << 11) + (cGreen << 5) + (cBlue << 0);
	return n565Color;
}

void lcd_backlight_offon(int mark)
{ 
	if(mark == true)
	hal_gpio_set_output(HAL_GPIO_32,HAL_GPIO_DATA_HIGH);
	else
	hal_gpio_set_output(HAL_GPIO_32,HAL_GPIO_DATA_LOW);
}

uint32_t sbit_nvram_read(void)
{
    nvdm_status_t nvdm_status;
    uint32_t size = sizeof(NVRAM_Read_Write);
    uint32_t size1 = sizeof(NVRAM_Read_Write1);
	int temp=0,i=0; 
		
    memset(&NVRAM_info,0,sizeof(NVRAM_info));
    nvdm_status = nvdm_read_data_item(SBIT_STEPS_GROUP_NAME, SBIT_STEPS_ITEM_NAME,(uint8_t *)&NVRAM_info, &size);

	if (nvdm_status != NVDM_MODEM_STATUS_OK)
	{
		SBIT_DBG("===============================================================::stk8323_reset_mark:error");
	}
	else
	{
		SBIT_DBG("============================================================::stk8323_reset_mark:%d", NVRAM_info.stk8323_reset_mark);
		SBIT_DBG("============================================================::NVRAM_info.value_steps:%d", NVRAM_info.value_steps);
		for(i = 0; i < 7; i++) 	
		{			
			SBIT_DBG("============================================================step_week_log:%d", NVRAM_info.step_week_log[i]);
		}
#ifdef MTK_fm78100_SUPPORT
		{
			extern int32_t ch1_raw_data_sum;
			extern int32_t ch2_raw_data_sum;
			//extern fm78100_chip_config_extern(void);
			//fm78100_chip_config_extern();//read nv value replace 
		}
#endif
	}

	
    memset(&NVRAM_info1,0,sizeof(NVRAM_info1));
    nvdm_status = nvdm_read_data_item(SBIT_STEPS_GROUP_NAME1, SBIT_STEPS_ITEM_NAME1,(uint8_t *)&NVRAM_info1, &size1);

	if (nvdm_status != NVDM_MODEM_STATUS_OK)
	{
		SBIT_DBG("===============================================================::NVRAM_info1:error");
	}
	else
	{
		SBIT_DBG("============================================================::NVRAM_info1 : 123456");
	}
	
    memset(&NVRAM_info2,0,sizeof(NVRAM_info2));
    nvdm_status = nvdm_read_data_item(SBIT_STEPS_GROUP_NAME2, SBIT_STEPS_ITEM_NAME2,(uint8_t *)&NVRAM_info2, &size1);

	if (nvdm_status != NVDM_MODEM_STATUS_OK)
	{
		SBIT_DBG("===============================================================::NVRAM_info2:error");
	}
	else
	{
		SBIT_DBG("============================================================::NVRAM_info2 : 123456");
	}

    memset(&NVRAM_info3,0,sizeof(NVRAM_info3));
    nvdm_status = nvdm_read_data_item(SBIT_STEPS_GROUP_NAME3, SBIT_STEPS_ITEM_NAME3,(uint8_t *)&NVRAM_info3, &size1);

	if (nvdm_status != NVDM_MODEM_STATUS_OK)
	{
		SBIT_DBG("===============================================================::NVRAM_info3:error");
	}
	else
	{
		SBIT_DBG("============================================================::NVRAM_info3 : 123456");
	}

	for(i = 0; i < 8; i++)	
	{
		strcpy(temp_info1.m2m_send_temp_Queue[i],NVRAM_info1.m2m_send_Queue1[i]); 
		strcpy(temp_info1.m2m_send_temp_Queue[8+i],NVRAM_info2.m2m_send_Queue2[i]); 
		strcpy(temp_info1.m2m_send_temp_Queue[16+i],NVRAM_info3.m2m_send_Queue3[i]); 
	}
	
	if(exception_auto_reboot_mark==0)
	NVRAM_info.exception_auto_reboot_num++;
	temp_info.step_cont_nvram = NVRAM_info.value_steps;	
	return temp_info.step_cont_nvram; 
	
}

void sbit_nvram_write(void)
{
    nvdm_status_t nvdm_status;
	int temp=0,i=0; 
	
	for(i = 0; i < 8; i++)	
	{
		strcpy(NVRAM_info1.m2m_send_Queue1[i],temp_info1.m2m_send_temp_Queue[i]); 
		strcpy(NVRAM_info2.m2m_send_Queue2[i],temp_info1.m2m_send_temp_Queue[8+i]); 
		strcpy(NVRAM_info3.m2m_send_Queue3[i],temp_info1.m2m_send_temp_Queue[16+i]); 
	}
		
	nvdm_status = nvdm_write_data_item(SBIT_STEPS_GROUP_NAME,
		SBIT_STEPS_ITEM_NAME,
		NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
		(uint8_t *)&NVRAM_info,
		sizeof(NVRAM_Read_Write));
	
	if (nvdm_status != NVDM_MODEM_STATUS_OK)
	{
		SBIT_DBG("=================================================================::stk8323_reset_mark:error");
	}
	else
	{
		SBIT_DBG("====================================================================::stk8323_reset_mark:123456");
	}	

		nvdm_status = nvdm_write_data_item(SBIT_STEPS_GROUP_NAME1,
		SBIT_STEPS_ITEM_NAME1,
		NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
		(uint8_t *)&NVRAM_info1,
		sizeof(NVRAM_Read_Write1));
	
	if (nvdm_status != NVDM_MODEM_STATUS_OK)
	{
		SBIT_DBG("=================================================================::NVRAM_Read_Write1 : error");
	}
	else
	{
		SBIT_DBG("====================================================================::NVRAM_Read_Write1 : 123456");
	}

		nvdm_status = nvdm_write_data_item(SBIT_STEPS_GROUP_NAME2,
		SBIT_STEPS_ITEM_NAME2,
		NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
		(uint8_t *)&NVRAM_info2,
		sizeof(NVRAM_Read_Write2));
	
	if (nvdm_status != NVDM_MODEM_STATUS_OK)
	{
		SBIT_DBG("=================================================================::NVRAM_Read_Write2 : error");
	}
	else
	{
		SBIT_DBG("====================================================================::NVRAM_Read_Write2 : 123456");
	}
	
		nvdm_status = nvdm_write_data_item(SBIT_STEPS_GROUP_NAME3,
		SBIT_STEPS_ITEM_NAME3,
		NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
		(uint8_t *)&NVRAM_info3,
		sizeof(NVRAM_Read_Write3));
	
	if (nvdm_status != NVDM_MODEM_STATUS_OK)
	{
		SBIT_DBG("=================================================================::NVRAM_Read_Write3 : error");
	}
	else
	{
		SBIT_DBG("====================================================================::NVRAM_Read_Write3 : 123456");
	}
	
	
}

void show_imei_mark_callback(void)
{	
	show_imei_mark=0;			
	SBIT_DBG("##############################################show_imei_mark_callback###################################################");
}
void sbit_key_vib_callback(void)
{		

    if((get_sbit_show_meun_id())==POWEROFF_SCREEN)	
	{			    
		show_imei_mark++;		
		if(show_imei_mark==1)		
		{			
			if(show_imei_mark_timer == NULL)
			show_imei_mark_timer = xTimerCreate("mark_timer",
			500 / portTICK_PERIOD_MS, 		    
			pdFALSE, 		    
			NULL, 		    
			show_imei_mark_callback);    			
			xTimerStart(show_imei_mark_timer,1);		
		}	
		
		SBIT_DBG("show_imei_mark %d \r\n",show_imei_mark);

		if(show_imei_mark>=2)		
		{			
			temp_info.show_idle_flag=IMEI_SCREEN;
			SBIT_DBG("show idle_flag %d [LINE:%d]",temp_info.show_idle_flag,__LINE__);
			show_imei_mark=0;
			if(show_imei_mark_timer!=NULL)		    
			xTimerStop(show_imei_mark_timer,0);		
			show_imei_mark_timer = NULL;
		}	
		
	}		
	
	hal_gpio_set_output(H10_VIBR_EN,HAL_GPIO_DATA_LOW);	
	if(sbit_key_vib_timer!=NULL)			
	xTimerStop(sbit_key_vib_timer,0); 
	
    if(temp_info.animation_flag == 0)
    {
		sbit_demo_display_send_msg(DISPLAY_ANIMATION,0);
	}
	else
		sbit_show_meun();
	
}

void sbit_key_vib(void)
{
	if(exception_auto_reboot_mark==1)
	hal_gpio_set_output(H10_VIBR_EN,HAL_GPIO_DATA_HIGH);
	if(sbit_key_vib_timer == NULL)
    sbit_key_vib_timer = xTimerCreate("key_vib", 
    100 / portTICK_PERIOD_MS, 
    pdFALSE, 
    NULL, 
    sbit_key_vib_callback);
	
    xTimerStart(sbit_key_vib_timer,1);
}

void sbit_set_vib_time(int conunt,int mark)
{   
    hal_rtc_time_t curtime;

	//hal_rtc_get_utc_time(&curtime);
    //applib_dt_utc_sec_2_mytime(utc_time,&curtime,0);

	hal_rtc_get_time(&curtime);//获取rtctime

   temp_info.vib_conunt=conunt;
   temp_info.vib_mark=mark;

   if(temp_info.vib_conunt==0)
   {
	   	hal_pinmux_set_function(H10_VIBR_EN, HAL_GPIO_1_GPIO1);			 /*   set dierection to be output  */	//马达控制		   
	    hal_gpio_set_direction(H10_VIBR_EN, HAL_GPIO_DIRECTION_OUTPUT);   
	    hal_gpio_set_output(H10_VIBR_EN,HAL_GPIO_DATA_LOW);
		return;
   }
   
   if(mark==1)
   {
		if(curtime.rtc_sec%2)
		{
			if(exception_auto_reboot_mark==1)
		    hal_gpio_set_output(H10_VIBR_EN,HAL_GPIO_DATA_HIGH);
		}
	    else
	   	{
		    hal_gpio_set_output(H10_VIBR_EN,HAL_GPIO_DATA_LOW);
	    }

   }
   else if(mark==2)
   {
	   	hal_pinmux_set_function(H10_VIBR_EN, HAL_GPIO_1_GPIO1);			 /*   set dierection to be output  */	//马达控制		   
	    hal_gpio_set_direction(H10_VIBR_EN, HAL_GPIO_DIRECTION_OUTPUT);
		if(exception_auto_reboot_mark==1)
	    hal_gpio_set_output(H10_VIBR_EN,HAL_GPIO_DATA_HIGH);
   }

}

void sbit_shut_down_callback(void)
{
    atci_response_t response = {{0}};
	hal_rtc_gpio_control_t gpio_control;
	
	// Control RTC GPIO 0 output low by software.
	task_stop_timer(TaskTimer_shutdown_ponit);
	if(sbit_shut_down_timer != NULL)
	xTimerStop(sbit_shut_down_timer,0);
	hal_gpio_set_output(H10_VIBR_EN,HAL_GPIO_DATA_LOW);
	
	gpio_control.is_sw_control = true;
	gpio_control.is_sw_output_high = false;
	hal_rtc_configure_gpio(HAL_RTC_GPIO_0, &gpio_control);
	hal_rtc_enter_forced_shut_down_mode(HAL_RTC_DISABLE, HAL_RTC_DEFAULT, HAL_RTC_DISABLE);//HAL_RTC_ENABLE
	response.response_len = strlen((char *) response.response_buf);
	response.response_flag |= ATCI_RESPONSE_FLAG_APPEND_OK;
	atci_send_response(&response);
	
}

void sbit_progress_points(uint8_t flag)
{
	uint8_t index;
    hal_rtc_time_t curtime;
	
	
	hal_rtc_get_time(&curtime);//获取rtctime
	

	if(flag == 0)
	{
		index = curtime.rtc_sec%3;
	}
	else
	{
		index = 0;
	}
	switch(index)
	{
		case 0:
			gdi_draw_image(80,212,IMAGE_RED_POINT);
			gdi_draw_image(114,212,IMAGE_BLUE_POINT);
			gdi_draw_image(148,212,IMAGE_YELLOW_POINT);
			break;
		case 1:
			gdi_draw_image(80,212,IMAGE_BLUE_POINT);
			gdi_draw_image(114,212,IMAGE_RED_POINT);
			gdi_draw_image(148,212,IMAGE_YELLOW_POINT);
			break;
		case 2:
			gdi_draw_image(80,212,IMAGE_BLUE_POINT);
			gdi_draw_image(114,212,IMAGE_YELLOW_POINT);
			gdi_draw_image(148,212,IMAGE_RED_POINT);
			break;
		default:
			break;
	}
}

void sbit_shut_down(void)
{
	if(exception_auto_reboot_mark==1)
	hal_gpio_set_output(H10_VIBR_EN,HAL_GPIO_DATA_HIGH);
	sbit_demo_display_send_msg(DISPLAY_POWEROFF,0);
	if(sbit_shut_down_timer == NULL)
    sbit_shut_down_timer = xTimerCreate("shut_down", 
    3000 / portTICK_PERIOD_MS, 
    pdFALSE, 
    NULL, 
    sbit_shut_down_callback);
    xTimerStart(sbit_shut_down_timer,1);
	SBIT_DBG("+++111M2M_SHUTDOWN");
}
void sbit_show_shutdown_points()
{
	static int ponits_flag =0; 
	SBIT_DBG("sbit_show_shutdown_points");
	sbit_progress_points(0);	
	task_start_timer(TaskTimer_shutdown_ponit,1000,sbit_show_shutdown_points);	
}
void sbit_show_power()
{
    gdi_clean();
	gdi_draw_fill_rectangle(0,0,UI_WIDTH,UI_HEIGHT,0x0000);
	if(temp_info.Low_battery_flag == 1)
	gdi_draw_image(9,105,IMAGE_INDEX_Low_battery);
	else
	{	
	gdi_draw_image(78,72,IMAGE_INDEX_POWEROFF_IDLE);		
	sbit_progress_points(0);	
	SBIT_DBG("sbit_show_power");
	task_start_timer(TaskTimer_shutdown_ponit,1000,sbit_show_power);	/*确认关机红蓝黄点走进度条指示*/
#if 0

#if defined(LOGO_SUPPORT) || defined(LOGO_SUPPORT_7)
	gdi_draw_image(9,105,IMAGE_GE_gepower_on_logo);
#endif

#if defined(LOGO_SUPPORT_1) 
	gdi_draw_image(0,0,IMAGE_INDEX_animation);
#endif
#if defined(LOGO_SUPPORT_2) || defined(LOGO_SUPPORT_3) || defined(LOGO_SUPPORT_4) || defined(LOGO_SUPPORT_5)|| defined(LOGO_SUPPORT_8)
	gdi_draw_image(0,0,IMAGE_INDEX_animation);
#endif
#if defined(LOGO_SUPPORT_6) 
	gdi_draw_image(0,0,IMAGE_INDEX_animation);
#endif
#endif

	}


	bsp_lcd_update_screen(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1);
}

void sbit_show_animation()
{
    gdi_clean();
	gdi_draw_fill_rectangle(0,0,UI_WIDTH,UI_HEIGHT,0x0000);
//#if defined(LOGO_SUPPORT) || defined(LOGO_SUPPORT_7) 
	gdi_draw_image(9,105,IMAGE_GE_gepower_on_logo);
//#endif
#if 0

#if defined(LOGO_SUPPORT_1) 
	gdi_draw_image(0,0,IMAGE_INDEX_animation);
#endif
#if defined(LOGO_SUPPORT_2) || defined(LOGO_SUPPORT_3) || defined(LOGO_SUPPORT_4) || defined(LOGO_SUPPORT_5) || defined(LOGO_SUPPORT_8)
	gdi_draw_image(0,0,IMAGE_INDEX_animation);
#endif
#if defined(LOGO_SUPPORT_6) 
	gdi_draw_image(0,0,IMAGE_INDEX_animation);
#endif

#endif

	bsp_lcd_update_screen(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1);
}
void off_vibe(void)
{
	hal_gpio_set_output(H10_VIBR_EN,HAL_GPIO_DATA_LOW);//kyb
}

void sbit_demo_display_send_msg(uint32_t msgid,void *msg)
{
	if (at_timer != NULL) 
	{
		switch(msgid)
		{
			case DISPLAY_BACKLIGHT:
				sbit_demo_backlight();
				break;
				
			case DISPLYA_UPDATE:
				temp_info.animation_flag = 1;
				if(temp_info.shut_down_flag == 0)  /* 防止关机中弹出时钟界面  Jerry  add */
				sbit_show_meun();
				break;
				
			case DISPLAY_ANIMATION:
				sbit_show_animation();
				break;
				
			case DISPLAY_POWEROFF:				
				sbit_show_power();
				break;
				
			case DISPLAY_FM:
				sbit_show_fm();
				break;

			default :
				break;
		 }
	}

}

void sbit_vib_timer_callback()
{
	if(temp_info.vib_tips_conunt>0)
	{
        temp_info.vib_tips_conunt--;
		sbit_set_vib_tips(temp_info.vib_tips_conunt,temp_info.vib_tips_mark);
	}
}

void sbit_set_vib_tips(int conunt,int mark)
{
    hal_rtc_time_t curtime;
	hal_rtc_get_time(&curtime);//获取rtctime
	
   temp_info.vib_tips_conunt=conunt;
   temp_info.vib_tips_mark=mark;

   sbit_demo_display_send_msg(DISPLAY_BACKLIGHT,0);  //点亮屏幕

   if(temp_info.vib_tips_conunt==0)
   {
	   	hal_pinmux_set_function(H10_VIBR_EN, HAL_GPIO_1_GPIO1);			 		   
	    hal_gpio_set_direction(H10_VIBR_EN, HAL_GPIO_DIRECTION_OUTPUT);   
	    hal_gpio_set_output(H10_VIBR_EN,HAL_GPIO_DATA_LOW);
		return;
   }
   
   if(mark==1)
   {
	 if(curtime.rtc_sec%2)
	 {
		if(exception_auto_reboot_mark==1)
	    hal_gpio_set_output(H10_VIBR_EN,HAL_GPIO_DATA_HIGH);
	 }
     else
   	 {
	    hal_gpio_set_output(H10_VIBR_EN,HAL_GPIO_DATA_LOW);
     }
   }
   else if(mark==2)
   {
	   	hal_pinmux_set_function(H10_VIBR_EN, HAL_GPIO_1_GPIO1);				   
	    hal_gpio_set_direction(H10_VIBR_EN, HAL_GPIO_DIRECTION_OUTPUT);  
		if(exception_auto_reboot_mark==1)
	    hal_gpio_set_output(H10_VIBR_EN,HAL_GPIO_DATA_HIGH);
   }

}


void H10_show_csq()
{
   int c=0;
    
   if(temp_info.registered_network_flag == 0)
   {
	   gdi_draw_image(16,14,signal_0);
	   
   }
   else if (((temp_info.csq_num>=0)&&(temp_info.csq_num<5))||temp_info.csq_num==99)
   {
	   gdi_draw_image(16,14,signal_0);
   }   
   else if(temp_info.csq_num>=5&&temp_info.csq_num<=8)
   {
	  gdi_draw_image(16,14,signal_0);
   }
   else if(temp_info.csq_num>8&&temp_info.csq_num<=12)
   {
	   gdi_draw_image(16,14,signal_2);
   }
   else if(temp_info.csq_num>12&&temp_info.csq_num<=20)
   {
	   gdi_draw_image(16,14,signal_3);
   }
   else if(temp_info.csq_num>20)
   {
	  gdi_draw_image(16,14,signal_4);
   }

}

void sbit_show_activity_idle(void)
{   
    int temp = 0;
	unsigned int color1 = 0 , color2 = 0 ,color3 = 0;
	int color_flag = 0;
	char test[12]={0};
	int maximum = 0, minimum = 0;
	int sleep_time = 0;
	hal_rtc_time_t curtime;	
	
	hal_rtc_get_time(&curtime);//获取rtctime
	
	//NVRAM_info.light_sleep_time = 367;
	//NVRAM_info.move_time = 200;
	
	gdi_clean();
	gdi_draw_fill_rectangle(0,0,UI_WIDTH,UI_HEIGHT,0x0000);
	
	gdi_draw_image(76,37,IMAGE_INDEX_sleep_time);

    if((NVRAM_info.deep_sleep_time+NVRAM_info.light_sleep_time)>180)
    //if(curtime.rtc_hour>=6 && curtime.rtc_hour <= 20)
    {
		temp = ((NVRAM_info.deep_sleep_time*1.2)+(NVRAM_info.light_sleep_time*1.2));
    }
	if(NVRAM_info.German_English_switch == 1)
	{
		gdi_draw_image(10,157,hr_x_0+(temp/60)/10);
		gdi_draw_image(42,157,hr_x_0+(temp/60)%10);
		gdi_draw_image(73,183,IMAGE_GE_GESLEEP_Std);
        gdi_draw_image(121,157,hr_x_0+(temp-((temp/60)*60))/10);
		gdi_draw_image(153,157,hr_x_0+temp%10);
		gdi_draw_image(185,184,IMAGE_GE_GESLEEP_MIN);
    }
	else
	{
		gdi_draw_image(19,157,hr_x_0+(temp/60)/10);
		gdi_draw_image(51,157,hr_x_0+(temp/60)%10);
		//draw_string("时",30+32*2,160+30,RGB888ToRGB565(0x00FFFF));


		gdi_draw_image(85,183,IMAGE_HOUR_CN);

		gdi_draw_image(111,157,hr_x_0+(temp-((temp/60)*60))/10);
		gdi_draw_image(143,157,hr_x_0+temp%10);
		//draw_string("分",30+32*5,160+30,RGB888ToRGB565(0x00FFFF));
		gdi_draw_image(175,183,IMAGE_MIN_CN);
    }	


	if((NVRAM_info.deep_sleep_time+NVRAM_info.light_sleep_time)>180)
	sleep_time = ((NVRAM_info.deep_sleep_time*1.2)+(NVRAM_info.light_sleep_time*1.2));
	else
	sleep_time = 0;

	/* 获取三个值中的最大值 */
	if(NVRAM_info.sedentary_time > NVRAM_info.move_time)
	{
		if(NVRAM_info.sedentary_time > sleep_time)
		color_flag = 1; /* 久坐 */
		else 
		color_flag = 2; /* 睡眠 */
	}
	else
	{
		if(NVRAM_info.move_time > sleep_time)
		color_flag = 3; /* 活动 */
		else 
		color_flag = 2; /* 睡眠 */
	}
    /* end */
	
	if(color_flag == 1)
	{
		temp = NVRAM_info.sedentary_time,
		color1 = 0xFF131D; 
	}
	else if(color_flag == 2)
	{
	    if((NVRAM_info.deep_sleep_time+NVRAM_info.light_sleep_time)>180)
		temp = sleep_time,
		color1 = 0x1AD5DE; 
	}
	else if(color_flag == 3)
	{
		temp = NVRAM_info.move_time,
		color1 = 0x9BFF04; 
	}
	
    if(color_flag > 0)
    {
		int temp2 = 0;
		int temp3 = 0;
		int test = 0;
		int angle = 0;
		/* 获取三个值中的最小值 */
		if(NVRAM_info.sedentary_time < NVRAM_info.move_time)
		{
			if(NVRAM_info.sedentary_time < sleep_time)
			color_flag = 1; /* 久坐 */
			else 
			color_flag = 2; /* 睡眠 */
		}
		else
		{
			if(NVRAM_info.move_time < sleep_time)
			color_flag = 3; /* 活动 */
			else 
			color_flag = 2; /* 睡眠 */
		}
		/* end */
		
		if(color_flag == 1)
		{
			temp3 = NVRAM_info.sedentary_time;
			color3 = 0xFF131D; 
		}
		else if(color_flag == 2)
		{
			temp3 = sleep_time;
			color3 = 0x1AD5DE; 
		}
		else if(color_flag == 3)
		{
			temp3 = NVRAM_info.move_time;
			color3 = 0x9BFF04; 
		}
		
		/* 获取三个值中的最中间值 */
		test = (NVRAM_info.sedentary_time + sleep_time + NVRAM_info.move_time)- temp3 - temp;
        
		if(test == NVRAM_info.sedentary_time)
		{
			color2 = 0xFF131D; 
		}
		else if(test == sleep_time)
		{
			color2 = 0x1AD5DE; 
		}
		else if(test == NVRAM_info.move_time)
		{
			color2 = 0x9BFF04; 
		}
		/* end */

		if(color_flag > 0)
		{
		    temp3 = (NVRAM_info.sedentary_time + sleep_time + NVRAM_info.move_time);
			if(temp3 >= (20*60))
			{
				angle = 360;
			}
			else
			{
				angle = temp3*360/(20*60);
			}
		}
		
		if(test > 0)
		{	
		    test = test + temp;
		    if(test >= (20*60))
			{
				angle = 360;
			}
			else
			{
				angle = test*360/(20*60);
			}
		}
		
		if(temp >= (20*60))
		{
			angle = 360;
		}
		else
		{
			angle = temp*360/(20*60);
		}
	}
	
	bsp_lcd_update_screen(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1);
	
}
uint8_t my_week = 0;


void sbit_show_time_idle(void)
{
	unsigned long temp = 0;
	int year;
	uint32_t w,h,x,y,YX_jibu;
	Ch_step_cnt_t  step_cnt;
	hal_rtc_time_t curtime; 
	hal_rtc_time_t utctime;
	int c=0;
	int i=0 , hour = 0;
	static int show_bat_cnt = 0;
	hal_rtc_get_time(&curtime);
	
	{
		static int j=0;
		if(++j %5 == 0)
		{
			dbg_print("Time:%d-%d-%d %d:%d",curtime.rtc_year,curtime.rtc_mon,curtime.rtc_day,curtime.rtc_hour,curtime.rtc_min,curtime.rtc_sec);
		}
	}
	gdi_clean();
    
	imei_flag = 0;
#if defined(MTK_DATA_SYN_SETTING_SUPPORT)
	NVRAM_info.data_syn_flag = 0;
#endif
    NVRAM_info.German_English_switch = NVRAM_info.English_German_flag;//0是English,1 是german	
	if(temp_info.vib_conunt>0)
	{
	   temp_info.vib_conunt--;	 
	   sbit_set_vib_time(temp_info.vib_conunt,temp_info.vib_mark);
	   gdi_draw_image(51,92,IMAGE_INDEX_TIME_SOS);   
	}
	else
	{
#if 0
	   if(NVRAM_info.Power_saving_flag == 1)
	   gdi_draw_image(92,10,IMAGE_INDEX_Flight);
	   else if(NVRAM_info.location_interval>0)
	   gdi_draw_image(135,10,IMAGE_INDEX_location_flag);
#if defined(LOGO_SUPPORT_3) 
	   else if(((NVRAM_info.step_interval>0)>0)||(NVRAM_info.movement_flag == 1))
	   gdi_draw_image(135,10,IMAGE_INDEX_step_flag);
#else
		else if(NVRAM_info.step_intervallogo)//if((NVRAM_info.step_interval>0)>0)
		gdi_draw_image(135,10,IMAGE_INDEX_step_flag);
#endif

	  // if((temp_info.ble_off_on_flag == 1)||(temp_info.Hw_Version == 2))
	  // gdi_draw_image(90,10,IMAGE_INDEX_bt);
	   
	   if (temp_info.wifi_off_on_flag == 1 && temp_info.wifi_wlap_success == 0)
	   {
			gdi_draw_image(119,33,IMAGE_INDEX_GPS_OPEN);
	   }
	   else if (temp_info.Valid_status == 1 && temp_info.gps_off_on_flag != 0)
	   {
		   gdi_draw_image(119,33,IMAGE_INDEX_GPS_SUCCESS);
	   }
	   else if(temp_info.gps_off_on_flag==1)
	   {
		   if(get_gps_num(5)<10)
			   gdi_draw_image(113,30,IMAGE_INDEX_TIME_H00+(get_gps_num(5)));
		   else
		   {
			   gdi_draw_image(103,30,IMAGE_INDEX_TIME_H00+1);
			   gdi_draw_image(123,30,IMAGE_INDEX_TIME_H00+(get_gps_num(5))-10);
		   }
	   }

	   if(curtime.rtc_hour > 12)
	   hour = curtime.rtc_hour - 12;
	   else
	   hour = curtime.rtc_hour;
	   
	   gdi_draw_image(13,57,IMAGE_XiTuo_time0+(hour)/10);
	   gdi_draw_image(64,57,IMAGE_XiTuo_time0+(hour)%10);
	   
	   if(curtime.rtc_sec%2)
	   {
		   gdi_draw_image(113,67,IMAGE_XiTuo_point);
	   }

	   gdi_draw_image(129,57,IMAGE_XiTuo_time0+curtime.rtc_min/10);
	   gdi_draw_image(180,57,IMAGE_XiTuo_time0+curtime.rtc_min%10);
       
#endif



   
     
	   if(curtime.rtc_hour<10)
		   gdi_draw_image(13,57,IMAGE_XiTuo_time0);
	   else
	   {
		   YX_jibu=(curtime.rtc_hour/10);
		   if(YX_jibu==1)
		   gdi_draw_image(13,57,IMAGE_XiTuo_time1);
		   else
		   gdi_draw_image(13,57,IMAGE_XiTuo_time0+YX_jibu);
	   }
	   
	   if(curtime.rtc_hour<10)
		   gdi_draw_image(64,57,IMAGE_XiTuo_time0+curtime.rtc_hour);
	   else
	   {
		   YX_jibu=(curtime.rtc_hour%10);
		   gdi_draw_image(64,57,IMAGE_XiTuo_time0+YX_jibu);
	   }
	     if(curtime.rtc_sec%2)
	   {
		   gdi_draw_image(113,67,IMAGE_XiTuo_point);
	   }
  
	   if(curtime.rtc_min<10)
		   gdi_draw_image(129,57,IMAGE_XiTuo_time0);
	   else
	   {
		   YX_jibu=(curtime.rtc_min/10);
		   if(YX_jibu==1)
		   gdi_draw_image(129,57,IMAGE_XiTuo_time1);
		   else
		   gdi_draw_image(129,57,IMAGE_XiTuo_time0+YX_jibu);
	   }
	   
	   if(curtime.rtc_min<10)
		   gdi_draw_image(180,57,IMAGE_XiTuo_time0+curtime.rtc_min);
	   else
	   {
		   YX_jibu=(curtime.rtc_min%10);
		   gdi_draw_image(180,57,IMAGE_XiTuo_time0+YX_jibu);
	   }



 
    if((temp_info.Low_battery_warning_flag == 1)&&(temp_info.usb_connect_flag == 0))
       {
		   if(NVRAM_info.German_English_switch)
           	gdi_draw_image(47,145,IMAGE_GE_Bitte_Laden);
		   else
		   	gdi_draw_image(45,130,Low_battery_Please_charge);

		   
	   }
	   else
	   {
#if 1		   
		   if(temp_info.usb_connect_flag==1 && temp_info.charging_complete==1)
		   {
			   if(temp_info.charging_complete == 1)
			   {
                 if(NVRAM_info.German_English_switch)
                 	gdi_draw_image(35,140,IMAGE_GE_Akku_geladen);
				 else
				 	gdi_draw_image(37,140,IMAGE_XiTuo_complete);
              
			   }
			   #if 0
			   else
			   {
                if(NVRAM_info.German_English_switch)
					gdi_draw_image(35,140,IMAGE_GE_Wird_geladen);
				else
			    	gdi_draw_image(63,140,IMAGE_XiTuo_charging);

               }
              #endif

			   
		   }
		   else
		   {
			   uint8_t week = 0;
			   uint8_t mon = 0;
			   
			   //if((curtime.rtc_sec == 0)&&(curtime.rtc_year+2000 >= 2018))
			   my_week = CaculateWeekDay(curtime.rtc_year+2000,curtime.rtc_mon,curtime.rtc_day);
			  // gdi_draw_image(35,170,IMAGE_XiTuo_week0);
			   if(my_week == 0)
			   week = 6;
			   else 
			   week = my_week - 1;
			  
               mon = curtime.rtc_mon - 1;
			   
			   if(NVRAM_info.German_English_switch)
			   {
                gdi_draw_image(33,145,IMAGE_GE_mo01+week);
                gdi_draw_image(155,145,IMAGE_GE_Januar01+mon);
				gdi_draw_image(105,147,IMAGE_INDEX_STEP_S0+curtime.rtc_day/10);
			    gdi_draw_image(125,147,IMAGE_INDEX_STEP_S0+curtime.rtc_day%10);
			   }
			   else
			   {
                 gdi_draw_image(32,145,IMAGE_XiTuo_week1+week);
                 gdi_draw_image(155,145,jan_01+mon);
				 gdi_draw_image(114,145,IMAGE_INDEX_STEP_S0+curtime.rtc_day/10);
			     gdi_draw_image(134,145,IMAGE_INDEX_STEP_S0+curtime.rtc_day%10);
               }	
			   
		   }
#else
		   /* 日期 */
		   if(temp_info.usb_connect_flag==1)
		   {
			   if(temp_info.charging_complete == 1)
			   gdi_draw_image(45,135,IMAGE_XiTuo_complete);
			   else
			   gdi_draw_image(45,135,IMAGE_XiTuo_charging);
		   }
		   else
		   {
			   gdi_draw_image(30,155,IMAGE_INDEX_TIME_H00+curtime.rtc_year/1000+2);
			   gdi_draw_image(50,155,IMAGE_INDEX_TIME_H00+((curtime.rtc_year/100)%10));
			   gdi_draw_image(70,155,IMAGE_INDEX_TIME_H00+((curtime.rtc_year/10)%10));
			   gdi_draw_image(90,155,IMAGE_INDEX_TIME_H00+curtime.rtc_year%10);
			   
			   gdi_draw_line(112,165,118,165,0xFFFF);
			   
			   gdi_draw_image(123,155,IMAGE_INDEX_TIME_H00+curtime.rtc_mon/10);
			   gdi_draw_image(143,155,IMAGE_INDEX_TIME_H00+curtime.rtc_mon%10);
			   
			   gdi_draw_line(165,165,171,165,0xFFFF);
			   
			   gdi_draw_image(173,155,IMAGE_INDEX_TIME_H00+curtime.rtc_day/10);
			   gdi_draw_image(193,155,IMAGE_INDEX_TIME_H00+curtime.rtc_day%10);
			   year=curtime.rtc_year+2000;
			   curtime.rtc_week=(curtime.rtc_day+2*curtime.rtc_mon+3*(curtime.rtc_mon+1)/5+year+year/4-year/100+year/400)%7;
		   }
#endif

	   }

	   /* 步数 */
	   step_cnt.wan_wei=NVRAM_info.value_steps/10000;
	   step_cnt.qian_wei=(NVRAM_info.value_steps/1000)%10;
	   step_cnt.bai_wei=(NVRAM_info.value_steps/100)%10;
	   step_cnt.shi_wei=(NVRAM_info.value_steps/10)%10; 
	   step_cnt.ge_wei=NVRAM_info.value_steps%10;
	
	   gdi_draw_image(31,183,IMAGE_XiTuo_Step);
	   #if 1
	   gdi_draw_image(77,188,step0+step_cnt.wan_wei);
	   gdi_draw_image(102,188,step0+step_cnt.qian_wei);
	   gdi_draw_image(130,188,step0+step_cnt.bai_wei);
	   gdi_draw_image(155,188,step0+step_cnt.shi_wei);
	   gdi_draw_image(180,188,step0+step_cnt.ge_wei);
	   #else
	   gdi_draw_image(77,188,step0+temp_info.charging_complete);
	 
	   #endif
	   
       if(temp_info.usb_connect_flag==0)
	   {
		    if(temp_info.batterr_data==100)
			gdi_draw_image(180,10,IMAGE_XiTuo_battery10);
			else if(temp_info.batterr_data == 90)
			gdi_draw_image(180,10,IMAGE_XiTuo_battery9);
			else if(temp_info.batterr_data == 80)
			gdi_draw_image(180,10,IMAGE_XiTuo_battery8);
			else if(temp_info.batterr_data == 70)
			gdi_draw_image(180,10,IMAGE_XiTuo_battery7);
			else if(temp_info.batterr_data == 60)
			gdi_draw_image(180,10,IMAGE_XiTuo_battery6);
			else if(temp_info.batterr_data == 50)
			gdi_draw_image(180,10,IMAGE_XiTuo_battery5);
			else if(temp_info.batterr_data == 40)
			gdi_draw_image(180,10,IMAGE_XiTuo_battery4);
			else if(temp_info.batterr_data == 30) 
			gdi_draw_image(180,10,IMAGE_XiTuo_battery3);
			else if(temp_info.batterr_data == 20) 
			gdi_draw_image(180,10,IMAGE_XiTuo_battery3);
			else if(temp_info.batterr_data == 10) 
			gdi_draw_image(180,10,IMAGE_XiTuo_battery2);
			else
			gdi_draw_image(180,10,IMAGE_XiTuo_battery1);
     }
     else
   	 {
       if(temp_info.charging_complete==1)
         	gdi_draw_image(180,10,IMAGE_XiTuo_battery10);
	   else
	   	{
			show_bat_cnt++; 

			if(show_bat_cnt==1)
			{
			   gdi_draw_image(180,10,IMAGE_XiTuo_battery4);
			}
			else if(show_bat_cnt==2)
			{
			   gdi_draw_image(180,10,IMAGE_XiTuo_battery6);
			}
			else if (show_bat_cnt==3)
			{
			   gdi_draw_image(180,10,IMAGE_XiTuo_battery8);
			}
			else if (show_bat_cnt==4)
			{
			   gdi_draw_image(180,10,IMAGE_XiTuo_battery10);
			   show_bat_cnt=0;
			}

	    }
      }
       #if 0
	   if((get_m2m_send_Queue()) >= 0)
	   {
	   	gdi_draw_image(158,10,IMAGE_XiTuo_gps_flag1);
	   }
       #endif
	   H10_show_csq();
	   //sbit_check_battery_data();
	   
	}
	
	bsp_lcd_update_screen(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1);

}
int angle_step = 0;
void sbit_show_setp_idle(void)
{
#define STEP_PROGRESS_BAR_WIDTH		190
#define STEP_PROGRESS_BAR_HEIGHT	14
#define STEP_PROGRESS_BAR_START_X		24
#define STEP_PROGRESS_BAR_START_Y		200
    uint32_t progress_x0,progress_y0, progress_x1, progress_y1;
	Ch_step_cnt_t  step_cnt;
	
    gdi_clean();
	
	
    step_cnt.wan_wei=NVRAM_info.value_steps/10000;
	step_cnt.qian_wei=(NVRAM_info.value_steps/1000)%10;
	step_cnt.bai_wei=(NVRAM_info.value_steps/100)%10;
	step_cnt.shi_wei=(NVRAM_info.value_steps/10)%10;
	step_cnt.ge_wei=NVRAM_info.value_steps%10;
	
	gdi_draw_image(41,123,hr_x_0+step_cnt.wan_wei);
	gdi_draw_image(73,123,hr_x_0+step_cnt.qian_wei);
	gdi_draw_image(105,123,hr_x_0+step_cnt.bai_wei);
	gdi_draw_image(137,123,hr_x_0+step_cnt.shi_wei);
	gdi_draw_image(169,123,hr_x_0+step_cnt.ge_wei);

	gdi_draw_image(85,12,IMAGE_XiTuo_Step_bg);
#if 0
    if(NVRAM_info.German_English_switch == 1)
		gdi_draw_image(76,178,IMAGE_GE_STEPS);
	else
		gdi_draw_image(91,180,IMAGE_XiTuo_StepW);

#endif
	gdi_draw_fill_rectangle(STEP_PROGRESS_BAR_START_X,STEP_PROGRESS_BAR_START_Y,STEP_PROGRESS_BAR_START_X+STEP_PROGRESS_BAR_WIDTH,STEP_PROGRESS_BAR_START_Y+STEP_PROGRESS_BAR_HEIGHT,RGB888ToRGB565(0x133950));
	progress_x1 = STEP_PROGRESS_BAR_START_X+STEP_PROGRESS_BAR_WIDTH*(NVRAM_info.value_steps%STEP_MAX)/STEP_MAX;
	//SBIT_DBG("=============sbit_show_setp_idle:%d, %d\r\n",test_step, progress_x1);
	if(progress_x1 > STEP_PROGRESS_BAR_START_X)
	{
		progress_y1 = STEP_PROGRESS_BAR_START_Y+STEP_PROGRESS_BAR_HEIGHT+1;
		progress_x0 = STEP_PROGRESS_BAR_START_X;
		progress_y0 = STEP_PROGRESS_BAR_START_Y;
		gdi_draw_fill_rectangle(progress_x0,progress_y0,progress_x1,progress_y1,RGB888ToRGB565(0xFFFF00));

		gdi_draw_solid_circle(STEP_PROGRESS_BAR_START_X+STEP_PROGRESS_BAR_WIDTH,STEP_PROGRESS_BAR_START_Y+STEP_PROGRESS_BAR_HEIGHT/2,STEP_PROGRESS_BAR_HEIGHT/2,RGB888ToRGB565(0x133950));
		gdi_draw_solid_circle(STEP_PROGRESS_BAR_START_X,STEP_PROGRESS_BAR_START_Y+STEP_PROGRESS_BAR_HEIGHT/2,STEP_PROGRESS_BAR_HEIGHT/2,RGB888ToRGB565(0xFFFF00));
		gdi_draw_solid_circle(progress_x1,progress_y1-STEP_PROGRESS_BAR_HEIGHT/2-1,STEP_PROGRESS_BAR_HEIGHT/2,RGB888ToRGB565(0xFFFF00));
	}
	else
	{
		gdi_draw_solid_circle(STEP_PROGRESS_BAR_START_X,STEP_PROGRESS_BAR_START_Y+STEP_PROGRESS_BAR_HEIGHT/2,STEP_PROGRESS_BAR_HEIGHT/2,RGB888ToRGB565(0x133950));
		gdi_draw_solid_circle(STEP_PROGRESS_BAR_START_X+STEP_PROGRESS_BAR_WIDTH,STEP_PROGRESS_BAR_START_Y+STEP_PROGRESS_BAR_HEIGHT/2,STEP_PROGRESS_BAR_HEIGHT/2,RGB888ToRGB565(0x133950));
	}
	bsp_lcd_update_screen(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1);

}

void sbit_show_cal_dist_idle(void)
{
    uint32_t km_num = 0;
    uint32_t m_num = 0;
	uint32_t cal_num = 0;
	int re_num;
    gdi_clean();
	
	km_num = (NVRAM_info.value_steps*0.6)/10;
	m_num  =  NVRAM_info.value_steps*0.6;

	if(km_num > 0)
	cal_num = (65*NVRAM_info.value_steps*0.60*1.036)/1000;
	else
	cal_num = 0;
	
	gdi_draw_image(10,47,IMAGE_INDEX_CAL);
	gdi_draw_image(55,58,hr_x_0+(cal_num/1000)%10);
	gdi_draw_image(85,58,hr_x_0+(cal_num/100)%10);
	gdi_draw_image(115,58,hr_x_0+(cal_num/10)%10);
	gdi_draw_image(145,58,hr_x_0+(cal_num)%10);
	gdi_draw_image(185,84,IMAGE_KCALORIE_CN);
	
	gdi_draw_image(10,139,IMAGE_INDEX_DIST);

	if(km_num >= 100)
	{
		gdi_draw_image(54,146,hr_x_0+(km_num/1000)%10);
		gdi_draw_image(84,146,hr_x_0+(km_num/100)%10);
        gdi_draw_image(117,184,dian_1);
		gdi_draw_image(128,146,hr_x_0+(km_num/10)%10);
		//gdi_draw_fill_rectangle(42+38+42-2+10-20,126+26+24,42+38+42-2+10+8-20,126+26+8+24,RGB888ToRGB565(0xFFFFFF));
	
		gdi_draw_image(158,146,hr_x_0+km_num%10);
	
		//draw_string("千米",12+36+96+20,122+10+38,RGB888ToRGB565(0x00FFFF));
		gdi_draw_image(192,168,IMAGE_KM_CN);
	}
	else
	{
		gdi_draw_image(55,146,hr_x_0+(m_num/1000)%10);
		gdi_draw_image(85,146,hr_x_0+(m_num/100)%10);
		gdi_draw_image(117,146,hr_x_0+(m_num/10)%10);
		gdi_draw_image(145,146,hr_x_0+m_num%10);
		//draw_string("米",12+36+96+20,122+10+38,RGB888ToRGB565(0x00FFFF));
		gdi_draw_image(185,177,IMAGE_M_CN);
	}
	
	if(km_num >= ((8000*0.65)/100))
	{
		angle_step = 360;
	}
	else
	{
		angle_step = km_num*360/((8000*0.65)/100);
	}
	
	bsp_lcd_update_screen(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1);

}

int blood_step = 0;
void sbit_show_hrte_idle(void)
{
    uint32_t w,h,x,y,i=0;	
	int show_hartrate_cnt[12]={0};
	Ch_step_cnt_t  step_cnt;
	
#if defined(MTK_MAX30102_SUPPORT)
    if(temp_info.Hr_delay==2)
	hal_max30102_driver_task();
#else
    if(temp_info.Hr_delay==2)
	Hrs3300_chip_init();
#endif
		
	if(temp_info.Hr_delay<=10)
	temp_info.Hr_delay++;
	
    gdi_clean();
	
	gdi_draw_image(75,21,IMAGE_XiTuo_hr);
	
	if(temp_info.hartrate_cnt < 40)
	{
		sbit_progress_points(0);
	}
	else
	{
		sbit_progress_points(1);
	}
	
	gdi_draw_image(72,123,hr_x_0+temp_info.hartrate_cnt/100);
	gdi_draw_image(104,123,hr_x_0+(temp_info.hartrate_cnt/10)%10);
	gdi_draw_image(136,123,hr_x_0+temp_info.hartrate_cnt%10);

	gdi_draw_image(98,184,IMAGE_XiTuo_hr_bg);
	

	bsp_lcd_update_screen(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1);
	
}

void sbit_show_blood_idle()
{	
#if defined(MTK_MAX30102_SUPPORT)
    int sp02 = 0;
	if(temp_info.bp_delay==2)
	{
		temp_info.max30102_sp02 = 0;
		hal_max30102_driver_task();
	}
#else
	if(temp_info.bp_delay==2)
	Hrs3300_chip_init();
#endif

	
	if(temp_info.bp_delay<=10)
	temp_info.bp_delay++;

    gdi_clean();

	gdi_draw_image(100,15,IMAGE_XiTuo_bp);

	
#if defined(MTK_MAX30102_SUPPORT) 
	if(temp_info.max30102_sp02 == 0)
#else
	if((temp_info.blood_lbp == 0) || (temp_info.blood_hbp == 0))
#endif
	{
		sbit_progress_points(0);
	}
	else
	{
		sbit_progress_points(1);
	}
		
		
#if defined(MTK_MAX30102_SUPPORT) 
	sp02 = (int)temp_info.max30102_sp02;
    if(sp02 == 0)
    {
		gdi_draw_image(50,80,step0);
	}
	else 
	{
		gdi_draw_image(35,80,step0+(sp02/10));
		gdi_draw_image(65,80,step0+sp02%10);
	}

	gdi_draw_image(112,85,IMAGE_XiTuo_slash);
	
	sp02 = (int)((temp_info.max30102_sp02-sp02)*100);
	
    if(temp_info.max30102_sp02 == 0)
    {
		gdi_draw_image(162,80,step0);
	}
	else
	{
		gdi_draw_image(147,80,step0+(sp02/10));
		gdi_draw_image(177,80,step0+sp02%10);
	}
	
#else
	gdi_draw_image(17,123,hr_x_0+temp_info.blood_hbp/100);
	gdi_draw_image(47,123,hr_x_0+(temp_info.blood_hbp-(temp_info.blood_hbp/100)*100)/10);
	gdi_draw_image(77,123,hr_x_0+temp_info.blood_hbp%10);
	
	gdi_draw_image(110,122,IMAGE_XiTuo_slash);

	gdi_draw_image(133,123,hr_x_0+temp_info.blood_lbp/100);
	gdi_draw_image(163,123,hr_x_0+(temp_info.blood_lbp-(temp_info.blood_lbp/100)*100)/10);
	gdi_draw_image(193,123,hr_x_0+temp_info.blood_lbp%10);
#endif

	gdi_draw_image(84,178,IMAGE_XiTuo_bp_bg);

	bsp_lcd_update_screen(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1);
	
}

extern uint16_t address_info[128];

void draw_long_string(char *str, int offset_x, int offset_y, color c)
{
	int len=strlen(str);

	int font_size = 16;
	int device_width = 240;

	int n_nums_per_line = (device_width-offset_x)/font_size;


	
	int n_line = 0;
	#define LINE_GAP	6
	int i=0;
	dbg_print("========draw_long_string================");

	for(i=0;i<strlen(str);)
	{
		char tmp_buf[20]={0};
		memset(tmp_buf,0,sizeof(tmp_buf));
		strncpy(tmp_buf,&str[i],n_nums_per_line);
		draw_string(tmp_buf,offset_x,offset_y+n_line*(font_size+LINE_GAP),c);
		n_line++;
		i+= n_nums_per_line;
	}
}

char show_times=0;
void sbit_show_gps_idle()
{	
	hal_rtc_time_t curtime; 
	char Latitude[30]={0};
	char Longitude[30]={0};
	char dest_src[30] = {0};
	
#if 0
	gdi_clean();
	uint8_t GPS_NUM = get_gps_num(20);
	
	 if (temp_info.Valid_status == 1 && temp_info.gps_off_on_flag != 0)
	   {
		   gdi_draw_image(119,33,IMAGE_INDEX_GPS_SUCCESS);
	   }
	draw_string("SAT:",10,35,RGB888ToRGB565(0xFFFFFF));

	if(get_gps_num()<10)
	{
		gdi_draw_image(12,60,IMAGE_INDEX_TIME_H00+(GPS_NUM));
	}
	else
	{
		gdi_draw_image(12,60,IMAGE_INDEX_TIME_H00+(GPS_NUM/10));
		gdi_draw_image(32,60,IMAGE_INDEX_TIME_H00+(GPS_NUM%10));
	}
    //if(strlen(temp_info.MY_Latitude)>0)
	{
	   Longitude_change(temp_info.MY_Longitude,Longitude);//temp_info.MY_Longitude
	   Latitude_change(temp_info.MY_Latitude,Latitude);
	   draw_string(Longitude,10,115,RGB888ToRGB565(0xFFFFFF));
	   draw_string(Latitude,10,165,RGB888ToRGB565(0xFFFFFF));

	
	   SBIT_DBG("========sbit_show_gps_idle================",temp_info.MY_Latitude);
	} 
	draw_string("LON:",10,95,RGB888ToRGB565(0xFFFFFF));
	draw_string("LAT:",10,145,RGB888ToRGB565(0xFFFFFF));
 
	



#endif

#if 0
	//hal_rtc_get_time(&curtime);
	gdi_clean(); 

	if((temp_info.gps_delay==3)&&(temp_info.gps_delay_flag == 0))
	{
		memset(address_info, 0, sizeof(address_info));
		WIFI_driver_init();
		
	}
	else if(temp_info.wifi_wlap_success==0 && temp_info.gps_delay >60)//获取热点失败,关闭wifi开启gps
	{
         WIFI_power_off();
         gps_driver_init();
	}
    else if(strlen(temp_info.MY_Latitude)==0 && temp_info.gps_delay == 180 && temp_info.wifi_wlap_success==0)//3//三分钟关闭gps
	{
         gps_power_off();
    }	
    if(temp_info.gps_delay <= 180)
	{
		temp_info.gps_delay++;
	}
    SBIT_DBG("==========================sbit_show_gps_idle[%d][%s]", temp_info.gps_delay, address_info);
	if((strlen(address_info) == 0) && (temp_info.gps_delay < 120))
	{
		sbit_progress_points(0);
		gdi_draw_image(96,36,IMAGE_XiTuo_gps_bg);
		if(NVRAM_info.German_English_switch)
		gdi_draw_image(33,145,IMAGE_GE_locationge);
		else
		gdi_draw_image(45,145,IMAGE_XiTuo_gps_font2);
	}
	else if((strlen(address_info) > 0) && (temp_info.gps_delay >= 10))
	{
		temp_info.gps_delay_flag = 150;
		draw_string("MY LOCATION",28,10,RGB888ToRGB565(0x1AD5DE));
		draw_long_string(address_info,4,55,RGB888ToRGB565(0xFFFFFF));//address_info  "King's Cross Square, Kings Cross, London N1C 4TB, UK"
	}
	else if((strlen(address_info) == 0) && (temp_info.gps_delay >= 120))//大于120秒提示定位失败
	{
		SBIT_DISPLAY_BACKLIGHT_TIME  = 120;
		sbit_progress_points(1);
		gdi_draw_image(96,36,IMAGE_XiTuo_gps_bg);
		if(NVRAM_info.German_English_switch)
        	gdi_draw_image(5,130,IMAGE_GE_location_nofound);
		else
			gdi_draw_image(7,145,IMAGE_XiTuo_gps_font);
	}
	else if((strlen(address_info) == 0) && (temp_info.gps_delay >= 40))
	{
		SBIT_DISPLAY_BACKLIGHT_TIME  = 120;
	}
#endif //目前暂不做同步数据功能只添加界面
	gdi_clean();
#if 1
	if(++show_times <60)
	{
		sbit_progress_points(0);
		gdi_draw_image(50,30,IMAGE_syn_ing);
	}
	else if(show_times>=60 && show_times<=63)
	{
		if(temp_info.registered_network_flag)
		gdi_draw_image(50,30,IMAGE_syn_sucess);
		else
		gdi_draw_image(50,30,IMAGE_syn_fail);
		sbit_progress_points(1);
	}
	

#else
    if(++times<=8)
    {
    	sbit_progress_points(0);
		gdi_draw_image(50,30,IMAGE_syn_ing);
    }
	if(times==1)
	sbit_m2m_ct_send_massege(M2M_CT_data_T100,NULL,0,0);
	if(times>8)
	{
		sbit_progress_points(1);		
		gdi_draw_image(50,30,IMAGE_syn_sucess);
	}
	if(times>=11)
	{
		times=0;
		temp_info.show_idle_flag=IDLE_SCREEN;
		sbit_demo_display_send_msg(DISPLYA_UPDATE,0);
	}

#endif
	bsp_lcd_update_screen(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1);
#if 1
  	if(show_times>63)
  	{  		
		temp_info.show_idle_flag=IDLE_SCREEN;		
		temp_info.backlight_counter=0;
		sbit_demo_display_send_msg(DISPLYA_UPDATE,0);
  	}
	SBIT_DBG("++++show_times: %d,show_idle_flag: %d\r\n",show_times,temp_info.show_idle_flag);
#endif
}



void sbit_show_weather_screen(void)
{
	int i = 0 , j = 0 , z = 0 ;
	#if 0 // n1 删除
    gdi_clean();
	gdi_draw_fill_rectangle(0,0,UI_WIDTH,UI_HEIGHT,RGB888ToRGB565(0x000000));

	if(strlen(NVRAM_info.weather_pic) > 0)
	{
		gdi_draw_image(60-16,20+10,IMAGE_XiTuo_bmweathericon0+(atoi(NVRAM_info.weather_pic)));
	}
	else
	{
		gdi_draw_image(60-16,20+10,IMAGE_XiTuo_bmweathericon0);
	}
	
    j = ((get_m2m_send_Queue())+1);
	
	if(j > 0)
	{
		for(i = 0; i < j; i++)	
		{
			if((strstr(temp_info1.m2m_send_temp_Queue[i],":T47:3,"))!=NULL)
            z = 1 ;
		}
	}

	if(((strlen(NVRAM_info.weather_info) == 0)||(temp_info.weather_updata_flag == 0))
	&&(z == 0)&&(temp_info.weather_delay_flag == 3))
	{
		WIFI_driver_init();
	}
	
	if(temp_info.weather_delay_flag<=10)
	{
		temp_info.weather_delay_flag++;
	}

	
	if(strlen(NVRAM_info.city_weather) > 0)
	{
	    if(strlen(NVRAM_info.city_weather) <= 2)
	    {
			draw_string_byuc(NVRAM_info.city_weather, 108+10+42+16, 80-26,RGB888ToRGB565(0xFFFFFF));
	    }
	    else if(strlen(NVRAM_info.city_weather) <= 4)
	    {
			draw_string_byuc(NVRAM_info.city_weather, 148+16, 80-16,RGB888ToRGB565(0xFFFFFF));
	    }
	    else if(strlen(NVRAM_info.city_weather) <= 6)
	    {
			draw_string_byuc(NVRAM_info.city_weather, 84+10+42, 80-26,RGB888ToRGB565(0xFFFFFF));
	    }
	    else if(strlen(NVRAM_info.city_weather) <= 8)
	    {
			draw_string_byuc(NVRAM_info.city_weather, 72+10+42, 80-26,RGB888ToRGB565(0xFFFFFF));
	    }
	    else if(strlen(NVRAM_info.city_weather) <= 10)
	    {
			draw_string_byuc(NVRAM_info.city_weather, 60+10+42, 80-26,RGB888ToRGB565(0xFFFFFF));
	    }
	    else if(strlen(NVRAM_info.city_weather) <= 12)
	    {
			draw_string_byuc(NVRAM_info.city_weather, 48+10+42, 80-26,RGB888ToRGB565(0xFFFFFF));
	    }
	}
	else
	{
		draw_string("未知", 148+16, 80-26,RGB888ToRGB565(0xFFFFFF));
	}
		
	if(strlen(NVRAM_info.low_weather) > 0)
	{
		draw_string_byuc(NVRAM_info.low_weather, 43, 120,RGB888ToRGB565(0xFFFFFF));
	}
	else
	{
		draw_string("22", 43, 120,RGB888ToRGB565(0xFFFFFF));
	}
	
	draw_string("~", 105, 128,RGB888ToRGB565(0xFFFFFF));
	
	if(strlen(NVRAM_info.hight_weather) > 0)
	{
		draw_string_byuc(NVRAM_info.hight_weather, 143, 120,RGB888ToRGB565(0xFFFFFF));
	}
	else
	{
		draw_string("25", 143, 120,RGB888ToRGB565(0xFFFFFF));
	}
	
	if(strlen(NVRAM_info.weather_info) == 0)
	{
		draw_string("多云转晴", 72, 155+8,RGB888ToRGB565(0xFFFFFF));
	}
	else if(strlen(NVRAM_info.weather_info) <= 2)
	{
		draw_string_byuc(NVRAM_info.weather_info, 108, 155+8,RGB888ToRGB565(0xFFFFFF));
	}
	else if(strlen(NVRAM_info.weather_info) <= 4)
	{
		draw_string_byuc(NVRAM_info.weather_info, 96, 155+8,RGB888ToRGB565(0xFFFFFF));
	}
	else if(strlen(NVRAM_info.weather_info) <= 6)
	{
		draw_string_byuc(NVRAM_info.weather_info, 84, 155+8,RGB888ToRGB565(0xFFFFFF));
	}
	else if(strlen(NVRAM_info.weather_info) <= 8)
	{
		draw_string_byuc(NVRAM_info.weather_info, 72, 155+8,RGB888ToRGB565(0xFFFFFF));
	}
	else if(strlen(NVRAM_info.weather_info) <= 10)
	{
		draw_string_byuc(NVRAM_info.weather_info, 60, 155+8,RGB888ToRGB565(0xFFFFFF));
	}

	
	if(NVRAM_info.weather_pm25[2] > 0)
	{
		draw_string("P",78-10, 188+20,RGB888ToRGB565(0xFFFFFF));
		draw_string("M",94-10, 188+20,RGB888ToRGB565(0xFFFFFF));
		draw_string(":",112-10, 184+20,RGB888ToRGB565(0xFFFFFF));
		draw_string_byuc(NVRAM_info.weather_pm25, 123-10, 188+20,RGB888ToRGB565(0xFFFFFF));
	}
	else
	{
		draw_string("P",78, 188+20,RGB888ToRGB565(0xFFFFFF));
		draw_string("M",94, 188+20,RGB888ToRGB565(0xFFFFFF));
		draw_string(":",112, 184+20,RGB888ToRGB565(0xFFFFFF));
		if(strlen(NVRAM_info.weather_pm25) > 0)
		{
			draw_string_byuc(NVRAM_info.weather_pm25, 123, 188+20,RGB888ToRGB565(0xFFFFFF));
		}
		else
		{
			draw_string("34", 123, 188+20,RGB888ToRGB565(0xFFFFFF));
		}
	}
	
	if(temp_info.weather_updata_flag >= (60*60*4))
	{
		angle_step = 360;
	}
	else
	{
		angle_step = temp_info.weather_updata_flag*360/(60*60*4);
	}
		
	bsp_lcd_update_screen(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1);
	#endif
}

void sbit_show_set_idle()
{		
    gdi_clean();
	gdi_draw_fill_rectangle(0,0,UI_WIDTH,UI_HEIGHT,0x0000);
    int crease = -6;
#if 0

#if defined(MTK_DATA_SYN_SETTING_SUPPORT)
	SBIT_DBG("=============sbit_show_set_idle %d\r\n",temp_info.set_mode_select);
	if(temp_info.set_mode_select == 2)
	{
       
       if(NVRAM_info.German_English_switch==1) //  语言切换
			gdi_draw_image(2,27,IMAGE_XiTuo_Sprachewr);
		else
			gdi_draw_image(2,27,IMAGE_XiTuo_Languagewr);  
		
       if(NVRAM_info.English_German_flag == 0)
		{
			gdi_draw_image(155,27,IMAGE_XiTuo_ENGE);
			gdi_draw_image(206,27,IMAGE_XiTuo_DEWR);
			
		}
		else
		{
			gdi_draw_image(155,27,IMAGE_XiTuo_ENWR);
			gdi_draw_image(206,27,IMAGE_XiTuo_DEGR);
		}
	   gdi_draw_image(8,80,IMAGE_model_line);

     if(NVRAM_info.German_English_switch==1)// 同步设置
			gdi_draw_image(2,103+crease,IMAGE_SyncEinstellungnw);
		else
			gdi_draw_image(2,103+crease,IMAGE_XiTuo_data_syn);
		
		if(NVRAM_info.data_syn_flag == 0)
		{
			gdi_draw_image(155,100,IMAGE_XiTuo_off);
		}
		else
		{
			gdi_draw_image(155,100,IMAGE_XiTuo_on);
		}
	 gdi_draw_image(8,168,IMAGE_model_line);
	 if(NVRAM_info.German_English_switch == 1)//抬手亮屏
			gdi_draw_image(2,173,IMAGE_GE_Auto_Bildschirm_aufwachen);
		else
			gdi_draw_image(2,173,IMAGE_XiTuo_Bright_screen_g);

		if(NVRAM_info.Bright_screen_flag == 0)
		{
			gdi_draw_image(155,185,IMAGE_XiTuo_off);
		}
		else
		{
			gdi_draw_image(155,185,IMAGE_XiTuo_on);
		}

	}
	else if(temp_info.set_mode_select >= 3)
	{

	  
      
	       if(NVRAM_info.German_English_switch==1)//同步设置
	       		gdi_draw_image(2,18+crease,IMAGE_SyncEinstellungnw);
		   else
		   		gdi_draw_image(2,18+crease,IMAGE_XiTuo_data_syn);
		   if(NVRAM_info.data_syn_flag == 0)
		   {
			   gdi_draw_image(155,15,IMAGE_XiTuo_off);
		   }
		   else
		   {
			   gdi_draw_image(155,15,IMAGE_XiTuo_on);
		   }

	       gdi_draw_image(8,80,IMAGE_model_line);
		   if(NVRAM_info.German_English_switch==1)//抬手亮屏
			   gdi_draw_image(2,90+crease,IMAGE_GE_s_Auto_Bildschim_aufwachen);
		   else
			   gdi_draw_image(2,110+crease,IMAGE_XiTuo_Bright_screen);
		   

		   if(NVRAM_info.Bright_screen_flag == 0)
			  {
				  gdi_draw_image(155,103+crease,IMAGE_XiTuo_off);
			  }
			  else
			  {
				  gdi_draw_image(155,103+crease,IMAGE_XiTuo_on);
			  }
			  
	        gdi_draw_image(8,165,IMAGE_model_line);
			if(NVRAM_info.German_English_switch == 1)
				gdi_draw_image(2,200+crease,IMAGE_GE_Zurucksetzenblue);// 恢复
			else
				gdi_draw_image(2,182+crease,IMAGE_Factory_Reset_g);
			if(NVRAM_info.Factory_Reset_flag==0)
			{
				gdi_draw_image(155,185,IMAGE_XiTuo_off);
			}
			else
			{

				gdi_draw_image(155,185,IMAGE_XiTuo_on);
			}
		
	
	   	
	}
	else
	{
#endif	
      if(temp_info.set_mode_select == 0)
		{
		    if(NVRAM_info.German_English_switch==1)
			gdi_draw_image(2,27,IMAGE_XiTuo_Sprachegr);
			else
			gdi_draw_image(2,27,IMAGE_XiTuo_Languagegr);   //  语言切换
		}
		else
		{
		    if(NVRAM_info.German_English_switch==1)
            gdi_draw_image(2,27,IMAGE_XiTuo_Sprachewr);
			else
			gdi_draw_image(2,27,IMAGE_XiTuo_Languagewr);
		}
		if(NVRAM_info.English_German_flag == 0)
		{
		    gdi_draw_image(155,27,IMAGE_XiTuo_ENGE);
			gdi_draw_image(206,27,IMAGE_XiTuo_DEWR);
			
		}
		else
		{
			gdi_draw_image(155,27,IMAGE_XiTuo_ENWR);
			gdi_draw_image(206,27,IMAGE_XiTuo_DEGR);
		}
		
        gdi_draw_image(8,80,IMAGE_model_line);

	   if(temp_info.set_mode_select == 1)
		{
          if(NVRAM_info.German_English_switch==1)
          	gdi_draw_image(2,103+crease,IMAGE_SyncEinstellungeng); 
		  else
          	gdi_draw_image(2,103+crease,IMAGE_XiTuo_data_syn_g); 
		}
		else
		{
          if(NVRAM_info.German_English_switch==1)
		  	gdi_draw_image(2,103+crease,IMAGE_SyncEinstellungnw);//  同步设置
		  else
			gdi_draw_image(2,103+crease,IMAGE_XiTuo_data_syn);//  同步设置
		}
		if(NVRAM_info.data_syn_flag == 0)
		{
			gdi_draw_image(155,100,IMAGE_XiTuo_off);
		}
		else
		{
			gdi_draw_image(155,100,IMAGE_XiTuo_on);
		}
		gdi_draw_image(8,168,IMAGE_model_line);

        if(NVRAM_info.German_English_switch == 1)
			gdi_draw_image(2,173,IMAGE_GE_s_Auto_Bildschim_aufwachen);// 抬手亮屏	
		else
			gdi_draw_image(2,173,IMAGE_XiTuo_Bright_screen);// 抬手亮屏	
		if(NVRAM_info.Bright_screen_flag == 0)
		{
			gdi_draw_image(155,185,IMAGE_XiTuo_off);
		}
		else
		{
		   
			gdi_draw_image(155,185,IMAGE_XiTuo_on);
		}

#if defined(MTK_DATA_SYN_SETTING_SUPPORT)
	}
#endif

#endif
//	kyb add
	  if(temp_info.set_mode_select == 0)
	  {
		  if(NVRAM_info.German_English_switch==1)
		  gdi_draw_image(2,27,IMAGE_XiTuo_Sprachegr);
		  else
		  gdi_draw_image(2,27,IMAGE_XiTuo_Languagegr);	 //  语言切换
	  }
	  else
	  {
		  if(NVRAM_info.German_English_switch==1)
		  gdi_draw_image(2,27,IMAGE_XiTuo_Sprachewr);
		  else
		  gdi_draw_image(2,27,IMAGE_XiTuo_Languagewr);
	  }
	  if(NVRAM_info.English_German_flag == 0)
	  {
		  gdi_draw_image(155,27,IMAGE_XiTuo_ENGE);
		  gdi_draw_image(206,27,IMAGE_XiTuo_DEWR);
		  
	  }
	  else
	  {
		  gdi_draw_image(155,27,IMAGE_XiTuo_ENWR);
		  gdi_draw_image(206,27,IMAGE_XiTuo_DEGR);
	  }
	  
	  gdi_draw_image(8,80,IMAGE_model_line);
//

	  if(temp_info.set_mode_select == 1)
	   {
		 if(NVRAM_info.German_English_switch==1)
		   gdi_draw_image(2,103+crease,IMAGE_GE_Zurucksetzenblue); 
		 else
		   gdi_draw_image(2,103+crease,IMAGE_Factory_Reset_g); 
	   }
	   else
	   {
		 if(NVRAM_info.German_English_switch==1)
		   gdi_draw_image(2,103+crease,IMAGE_GE_Zurucksetze);//	同步设置
		 else
		   gdi_draw_image(2,103+crease,IMAGE_Factory_Reset);//  同步设置
	   }
	   if(NVRAM_info.Factory_Reset_flag==0)
	   {
		   gdi_draw_image(155,100,IMAGE_XiTuo_off);
	   }
	   else
	   {
		   gdi_draw_image(155,100,IMAGE_XiTuo_on);
	   }
	   gdi_draw_image(8,168,IMAGE_model_line);


	bsp_lcd_update_screen(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1);
	
}

void sbit_show_poweroff_idle()
{		
    gdi_clean();
	gdi_draw_image(78,72,IMAGE_INDEX_POWEROFF_IDLE);
#if 0
	if(NVRAM_info.German_English_switch == 1)
    gdi_draw_image(14,161,IMAGE_GE_Poweroff_logo);
	else
	gdi_draw_image(37,160,Power_off_bg);
#endif
	bsp_lcd_update_screen(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1);
}

void sbit_show_Sedentary_idle(void)
{	
    #if 0
    gdi_clean();
	gdi_draw_fill_rectangle(0,0,UI_WIDTH,UI_HEIGHT,0x0000);
	gdi_draw_image(79,29,IMAGE_INDEX_Sedentary_remind);
    gdi_draw_image(12,179,Sedentaryreminder_gg);
	
	bsp_lcd_update_screen(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1);
	#endif
}


void sbit_show_medicine_remind(void)
{
	char digit[4] = {0};
	int curr_hour = 0, curr_min = 0;
	int next_hour = 0, next_min = 0;
    #if 0  //n1 删除
	digit[0] = curr_remind_time_str[0];
	digit[1] = curr_remind_time_str[1];
	curr_hour = atoi(digit);
	//SBIT_DBG("=============sbit_show_medicine_remind hour:%s,[%s], %d\r\n",curr_remind_time_str, digit, curr_hour);
	digit[0] = curr_remind_time_str[2];
	digit[1] = curr_remind_time_str[3];
	curr_min = atoi(digit);
	//SBIT_DBG("=============sbit_show_medicine_remind min:[%s], %d\r\n",digit, curr_min);
	next_hour = curr_hour+min_next_remin_num/60;
	next_min = curr_min+min_next_remin_num%60;
	//SBIT_DBG("=============sbit_show_medicine_remind aa:%d, %d, %d\r\n",next_hour, next_min, min_next_remin_num);
	if(next_min >= 60)
	{
		next_hour += next_min/60;
		next_min = next_min%60;
	}
	//SBIT_DBG("=============sbit_show_medicine_remind bb:%d, %d\r\n",next_hour, next_min);
	gdi_clean();
	gdi_draw_fill_rectangle(0,0,UI_WIDTH,UI_HEIGHT,0x000000);
	gdi_draw_fill_rectangle(0,0,UI_WIDTH,UI_HEIGHT/2-1,RGB888ToRGB565(0x000000));
	gdi_draw_fill_rectangle(0,UI_HEIGHT/2-1,UI_WIDTH,UI_HEIGHT/2+1,RGB888ToRGB565(0xFFFFFF));
	gdi_draw_fill_rectangle(0,UI_HEIGHT/2+1,UI_WIDTH,UI_HEIGHT,RGB888ToRGB565(0x000000));
	gdi_draw_image(13,25,hr_x_0+curr_hour/10);
	gdi_draw_image(49,25,hr_x_0+curr_hour%10);
	gdi_draw_image(85,31,IMAGE_median);
	gdi_draw_image(104,25,hr_x_0+curr_min/10);
	gdi_draw_image(142,25,hr_x_0+curr_min%10);

	gdi_draw_image(180,21,IMAGE_XiTuo_medicine);
	
	gdi_draw_image(13,83,IMAGE_XiTuo_curr_medicine);
	gdi_draw_image(13,141,hr_x_0+(next_hour%24)/10);
	gdi_draw_image(49,141,hr_x_0+(next_hour%24)%10);
	gdi_draw_image(85,147,IMAGE_median);
	gdi_draw_image(106,141,hr_x_0+(next_min/10));
	gdi_draw_image(142,141,hr_x_0+(next_min%10));
	if((min_next_remin_num/60) >= 100)
	{
		gdi_draw_image(120,207,IMAGE_INDEX_TIME_H00+((min_next_remin_num/60)/10)/10);
		gdi_draw_image(132,207,IMAGE_INDEX_TIME_H00+((min_next_remin_num/60)/10)%10);
		gdi_draw_image(144,207,IMAGE_INDEX_TIME_H00+(min_next_remin_num/60)%10);
		//gdi_draw_image(190,205,IMAGE_XiTuo_medicine_hour_only);
		gdi_draw_image(157,205,IMAGE_XiTuo_medicine_hour);//hr

		gdi_draw_image(177,207,IMAGE_INDEX_TIME_H00+(min_next_remin_num%60)/10);
		gdi_draw_image(189,207,IMAGE_INDEX_TIME_H00+(min_next_remin_num%60)%10);
		gdi_draw_image(202,205,IMAGE_XiTuo_medicine_min);//min
		
	}
	else if((min_next_remin_num/60) >= 10 && (min_next_remin_num/60) < 100)
	{
		
		gdi_draw_image(120,207,IMAGE_INDEX_TIME_H00+(min_next_remin_num/60)/10);
		gdi_draw_image(132,207,IMAGE_INDEX_TIME_H00+(min_next_remin_num/60)%10);
		gdi_draw_image(145,205,IMAGE_XiTuo_medicine_hour);
		gdi_draw_image(165,207,IMAGE_INDEX_TIME_H00+(min_next_remin_num%60)/10);
		gdi_draw_image(177,207,IMAGE_INDEX_TIME_H00+(min_next_remin_num%60)%10);
		gdi_draw_image(190,205,IMAGE_XiTuo_medicine_min);//min
	}
	else if((min_next_remin_num/60) < 10)
	{
		
		gdi_draw_image(120,207,IMAGE_INDEX_TIME_H00+(min_next_remin_num/60));
		gdi_draw_image(133,205,IMAGE_XiTuo_medicine_hour);

		gdi_draw_image(153,207,IMAGE_INDEX_TIME_H00+(min_next_remin_num%60)/10);
		gdi_draw_image(165,207,IMAGE_INDEX_TIME_H00+(min_next_remin_num%60)%10);
		gdi_draw_image(178,205,IMAGE_XiTuo_medicine_min);//min
	}

	gdi_draw_image(14,205,IMAGE_XiTuo_medicine_next);
	bsp_lcd_update_screen(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1);
	
	if(temp_info.vib_conunt > 0)
	{
	   temp_info.vib_conunt--;	 
	   sbit_set_vib_time(temp_info.vib_conunt,temp_info.vib_mark);
	}
  #endif
}
char send_t100_flag=0; // 同步界面发送t100报文flag
void sbit_show_meun(void)
{	
    if(temp_info.fm_mode == 0)
	{

	SBIT_DBG("sbit_show_meun:[%d,%d],%d",(temp_info.activity_idle_flag == 1),(temp_info.set_mode_flag != 0),get_sbit_show_meun_id());
		if(temp_info.activity_idle_flag == 1)
		{
			sbit_show_activity_idle();
	    }
		else if(temp_info.set_mode_flag != 0)
		{
			temp_info.show_idle_flag=IDLE_SCREEN;
			SBIT_DBG("show idle_flag:%d [%s, LINE:%d]",temp_info.show_idle_flag,__FILE__,__LINE__);
			sbit_show_set_idle(); 
		}
		else
		{
			if((get_sbit_show_meun_id())==IDLE_SCREEN)
            { 
				SBIT_DISPLAY_BACKLIGHT_TIME  = 15;
				send_t100_flag=0;
				show_times=0;
				sbit_show_time_idle();
            }
			else if((get_sbit_show_meun_id())==STEP_SCREEN)
			{
				SBIT_DISPLAY_BACKLIGHT_TIME  = 15;
				temp_info.Hr_delay=0;
				sbit_show_setp_idle();
			}
			else if(get_sbit_show_meun_id() == CAL_DIST_SCREEN)
			{
				
				SBIT_DISPLAY_BACKLIGHT_TIME  = 15;
				temp_info.Hr_delay=0;//2022_080
				sbit_show_cal_dist_idle();
			}
#if 0
			else if((get_sbit_show_meun_id())==ACTIVITY_SCREEN)
			{
				SBIT_DISPLAY_BACKLIGHT_TIME  = 15;
				temp_info.Hr_delay=0;
				sbit_show_activity_idle();
			}
#endif
			else if((get_sbit_show_meun_id())==HEART_RATE_SCREEN)
			{
				SBIT_DISPLAY_BACKLIGHT_TIME  = 45;
				if(temp_info.Hr_delay == 0)
				{
					temp_info.blood_lbp=0;
					temp_info.blood_hbp=0;
					temp_info.hartrate_cnt=0;
				}
				temp_info.bp_delay=0;
				sbit_show_hrte_idle();
			}
	//因我司欧洲有个重要客户需求，将N1的海外版固件在最后的版本N1_2022_0526基础上UI再将睡眠和血压屏蔽掉，
	//也不要检测和上报这两个数据，同时SOS启动时还是保留发定位信息，谢谢！2022_0804
#if 0		
			else if((get_sbit_show_meun_id())==BLOOD_SCREEN)
			{
				if(temp_info.gps_delay>0)
				{
					temp_info.gps_delay=0;
					gps_power_off();
				}
				if(temp_info.bp_delay == 0)
				{
					temp_info.blood_lbp=0;
					temp_info.blood_hbp=0;
					
#if defined(MTK_MAX30102_SUPPORT)
					SBIT_DISPLAY_BACKLIGHT_TIME  = 60;
					temp_info.max30102_sp02 = 0;
					maxim_max30102_onoff(FALSE);
#else
					SBIT_DISPLAY_BACKLIGHT_TIME  = 45;
					mmi_stop_heart_rate_count();
#endif

				}
				temp_info.gps_delay = 0;
				memset(address_info, 0, sizeof(address_info));
				sbit_show_blood_idle();
			}
#endif

			else if((get_sbit_show_meun_id())==GPS_SCREEN)
			{
				//if(temp_info.bp_delay>0)
				if(temp_info.Hr_delay>0)
				{
					temp_info.bp_delay=0;
					temp_info.Hr_delay=0;
					mmi_stop_heart_rate_count();
				}
				SBIT_DISPLAY_BACKLIGHT_TIME  = 68;
				temp_info.weather_delay_flag = 0;
				if(!send_t100_flag)
				{
					send_t100_flag=1;
					sbit_m2m_ct_send_massege(M2M_CT_data_T100,NULL,0,0);
				}
				sbit_show_gps_idle();
			}
			#if 0
			else if((get_sbit_show_meun_id())==WEATHER_SCREEN)
			{
				SBIT_DISPLAY_BACKLIGHT_TIME  = 15;
				sbit_show_weather_screen();
			}
			#endif
			else if((get_sbit_show_meun_id())==POWEROFF_SCREEN)
			{
				send_t100_flag=0;
				show_times=0;
			    temp_info.gps_delay = 0;
				memset(address_info, 0, sizeof(address_info));
				temp_info.weather_delay_flag = 0;
				SBIT_DISPLAY_BACKLIGHT_TIME  = 15;
				if(temp_info.gps_delay>0)
				{
					temp_info.gps_delay=0;
					//gps_power_off();
				}
				sbit_show_poweroff_idle();
			}
			else if((get_sbit_show_meun_id())==POWEROFF_ANIMATION_SCREEN)
			{
				sbit_show_power();
			}
			else if((get_sbit_show_meun_id())==IMEI_SCREEN)
			{
				sbit_show_imei();
			}
		}
	}
	else
		sbit_show_fm();	
	
	//SBIT_DBG("=============>  sbit_show_meun <============= %d\r\n",temp_info.show_idle_flag);
	
}
extern uint8_t Hrs3300_id;
void sbit_show_fm(void)
{
		static int flag = 0;
		static int setp_flag = 0;
		static int sim_flag = 0;
		static int csq_flag = 0;
		static int ble_flag = 0;
		static int wifi_flag = 0;
	
		if(temp_info.Hw_Version == 2)
		{
			
			gdi_clean();
			gdi_draw_fill_rectangle(0,0,UI_WIDTH,UI_HEIGHT,0x0000);
			
			if(flag == 0)
			{
				WIFI_driver_init();
				Hrs3300_chip_init();
				SBIT_DISPLAY_BACKLIGHT_TIME  = 90;
			}
			
			if((temp_info.wifi_wlap_success == 0)&&(wifi_flag == 0))
			{
				if(flag == 20)
				BLE_power_off();
				else if(flag == 22)
				WIFI_driver_init();
			}
			
			flag ++;
			
			draw_string("计步:",15,28,RGB888ToRGB565(0xFFFFFF));
			
			if((NVRAM_info.value_steps > 0)||( temp_info.stk8321_chip_id == 0x23 ))
			{
				setp_flag = 1;
			}
			if(setp_flag == 1)
			gdi_draw_image(155,32,IMAGE_INDEX_GPS_SUCCESS);
			
			
			draw_string("心率:",15,28+(30*1),RGB888ToRGB565(0xFFFFFF));
			
			if(Hrs3300_id == 33)
			{
				temp_info.fm_hr_flag = 1;
				mmi_stop_heart_rate_count();
			}
			
			if(temp_info.fm_hr_flag == 1)
			gdi_draw_image(155,32+(30*1),IMAGE_INDEX_GPS_SUCCESS);
			
			draw_string("GPS:",12,28+(30*2),RGB888ToRGB565(0xFFFFFF));
			if((temp_info.Valid_status == 1)||(get_gps_num(36)>=6))
			{
				NVRAM_info.fm_gps_flag = 1;
				BLE_power_off();
			}
			
			if(NVRAM_info.fm_gps_flag == 1)
				gdi_draw_image(155,32+(30*2),IMAGE_INDEX_GPS_SUCCESS);
			else
			{
				if(get_gps_num(5)<10)
					gdi_draw_image(150,32+(30*2),IMAGE_INDEX_TIME_H00+(get_gps_num(5)));
				else
				{
					gdi_draw_image(150,32+(30*2),IMAGE_INDEX_TIME_H00+((get_gps_num(5))/10));
					gdi_draw_image(170,32+(30*2),IMAGE_INDEX_TIME_H00+((get_gps_num(5))%10));
				}
			}
			
			draw_string("WIFI:",12,28+(30*3),RGB888ToRGB565(0xFFFFFF));
			if((temp_info.wifi_wlap_success == 1)||(strlen(temp_info.total_buffer_wlap) >= 10))
			{
				if(wifi_flag == 0)
				BLE_search_init();
				wifi_flag = 1;
			}
			
			if(wifi_flag == 1)
			gdi_draw_image(155,32+(30*3),IMAGE_INDEX_GPS_SUCCESS);
			
			draw_string("SIM:",12,28+(30*4),RGB888ToRGB565(0xFFFFFF));
			if(temp_info.registered_network_flag == 1)
			{
				sim_flag = 1;
			}
			if(sim_flag == 1)
			gdi_draw_image(155,32+(30*4),IMAGE_INDEX_GPS_SUCCESS);
			
			
			draw_string("CSQ:",12,28+(30*5),RGB888ToRGB565(0xFFFFFF));
			if(temp_info.csq_num<10)
				gdi_draw_image(150,32+(30*5),IMAGE_INDEX_TIME_H00+(temp_info.csq_num));
			else
			{
				gdi_draw_image(150,32+(30*5),IMAGE_INDEX_TIME_H00+((temp_info.csq_num)/10));
				gdi_draw_image(170,32+(30*5),IMAGE_INDEX_TIME_H00+((temp_info.csq_num)%10));
			}
			
			SBIT_DBG("sbit_show_fm %d,%d,%d,%d\r\n",temp_info.BLE_search_success,temp_info.wifi_wlap_success,temp_info.total_buffer_wlap,temp_info.BLE_search_off_on_flag);
			draw_string("BLE:",12,28+(30*6),RGB888ToRGB565(0xFFFFFF));
			if(temp_info.BLE_search_success == 1)
			{
				if(ble_flag == 0)
				gps_driver_init();
				ble_flag = 1;
			}
			if(ble_flag == 1)
			gdi_draw_image(155,32+(30*6),IMAGE_INDEX_GPS_SUCCESS);
			
			bsp_lcd_update_screen(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1);
	
		}
		else
		{
			char test[12]={0};
			static int mark=0;
			static int connect_flag=0;
			
			gdi_clean();
			gdi_draw_fill_rectangle(0,0,UI_WIDTH,UI_HEIGHT,0x0000);
		
			if(flag == 0)
			{
				flag = 1;
				WIFI_driver_init();
				Hrs3300_chip_init();
			}
			draw_string("计步:",15,18,RGB888ToRGB565(0xFFFFFF));
			
			if((NVRAM_info.value_steps > 0)||( temp_info.stk8321_chip_id == 0x23 ))
			{
				setp_flag = 1;
				if(NVRAM_info.fm_gps_flag == 0)
				gps_driver_init();
			}
			if(setp_flag == 1)
			gdi_draw_image(155,22,IMAGE_INDEX_GPS_SUCCESS);
		
			
			draw_string("心率:",15,18+(30*1),RGB888ToRGB565(0xFFFFFF));
			if(Hrs3300_id == 33)
			{
				temp_info.fm_hr_flag = 1;
				mmi_stop_heart_rate_count();
			}
			
			if(temp_info.fm_hr_flag == 1)
			gdi_draw_image(155,22+(30*1),IMAGE_INDEX_GPS_SUCCESS);
			
			draw_string("GPS:",12,18+(30*2),RGB888ToRGB565(0xFFFFFF));
			if((temp_info.Valid_status == 1)||(get_gps_num(38)>=6))
			{
				NVRAM_info.fm_gps_flag = 1;
#if !defined(LOGO_SUPPORT_7) 
				gps_power_off();
#endif
			}
			
			if(NVRAM_info.fm_gps_flag == 1)
				gdi_draw_image(155,22+(30*2),IMAGE_INDEX_GPS_SUCCESS);
			else
			{
				if(get_gps_num(5)<10)
					gdi_draw_image(150,22+(30*2),IMAGE_INDEX_TIME_H00+(get_gps_num(5)));
				else
				{
					gdi_draw_image(150,22+(30*2),IMAGE_INDEX_TIME_H00+((get_gps_num(5))/10));
					gdi_draw_image(170,22+(30*2),IMAGE_INDEX_TIME_H00+((get_gps_num(5))%10));
				}
			}
			
			draw_string("WIFI:",12,18+(30*3),RGB888ToRGB565(0xFFFFFF));
			if((temp_info.wifi_wlap_success == 1)||(strlen(temp_info.total_buffer_wlap) >= 10))
			{
				temp_info.fm_wifi_flag = 1;
			}
			
			if(temp_info.fm_wifi_flag == 1)
			gdi_draw_image(155,22+(30*3),IMAGE_INDEX_GPS_SUCCESS);
			
			draw_string("SIM:",12,18+(30*4),RGB888ToRGB565(0xFFFFFF));
			if(temp_info.registered_network_flag == 1)
			{
				sim_flag = 1;
			}
			if(sim_flag == 1)
			gdi_draw_image(155,22+(30*4),IMAGE_INDEX_GPS_SUCCESS);
			
			draw_string("CSQ:",12,18+(30*5),RGB888ToRGB565(0xFFFFFF));
			if(temp_info.csq_num<10)
				gdi_draw_image(150,22+(30*5),IMAGE_INDEX_TIME_H00+(temp_info.csq_num));
			else
			{
				gdi_draw_image(150,22+(30*5),IMAGE_INDEX_TIME_H00+((temp_info.csq_num)/10));
				gdi_draw_image(170,22+(30*5),IMAGE_INDEX_TIME_H00+((temp_info.csq_num)%10));
			}
			
			draw_string("充电:",12,18+(30*6),RGB888ToRGB565(0xFFFFFF));
			if((temp_info.usb_connect_flag==1)||(connect_flag == 1))
			{
				connect_flag = 1;
				gdi_draw_image(155,22+(30*6),IMAGE_INDEX_GPS_SUCCESS);
			}
			
#if defined(LOGO_SUPPORT_4) 
    if(sim_flag == 0)
    {
		if(mark == 45)
			set_phone_functionality(false);
		else if(mark == 50)
		{
			set_phone_functionality(true); 
			mark = 0;
		}
		mark ++;
	}
#endif	
			
			bsp_lcd_update_screen(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1);
	
		}
}
void sbit_show_imei(void)
{	
	gdi_clean();
	SBIT_DBG("PELTEN=======>SHOW_IMEI_IDLE_entry %s\r\n",temp_info.imei_num);
	gdi_draw_fill_rectangle(0,0,UI_WIDTH,UI_HEIGHT,0x0000);

	gdi_draw_image(11,29,IMAGE_INDEX_TITLE);
	gdi_draw_image(10,67,step0+(temp_info.build_time[4]-'0'));
	gdi_draw_image(36,67,step0+(temp_info.build_time[5]-'0'));
	gdi_draw_image(62,67,step0+(temp_info.build_time[6]-'0'));
	gdi_draw_image(88,67,step0+(temp_info.build_time[7]-'0'));
	gdi_draw_image(114,67,step0+(temp_info.build_time[8]-'0'));
	gdi_draw_image(140,67,step0+(temp_info.build_time[9]-'0'));
	gdi_draw_image(166,67,step0+(temp_info.build_time[10]-'0'));
	gdi_draw_image(193,67,step0+(temp_info.build_time[11]-'0'));
	
	if(NVRAM_info.German_English_switch == 1)
		gdi_draw_image(10,148,IMAGE_GE_IMEI_No);
	else
		gdi_draw_image(10,148,IMAGE_INDEX_IMEIVS);

	gdi_draw_image(10,187,IMAGE_NUMM_0+(temp_info.imei_num[0]-'0'));
	gdi_draw_image(10+15,187,IMAGE_NUMM_0+(temp_info.imei_num[1]-'0'));
	gdi_draw_image(10+15*2,187,IMAGE_NUMM_0+(temp_info.imei_num[2]-'0'));
	gdi_draw_image(10+15*3,187,IMAGE_NUMM_0+(temp_info.imei_num[3]-'0'));
	gdi_draw_image(10+15*4,187,IMAGE_NUMM_0+(temp_info.imei_num[4]-'0'));
	gdi_draw_image(10+15*5,187,IMAGE_NUMM_0+(temp_info.imei_num[5]-'0'));
	gdi_draw_image(10+15*6,187,IMAGE_NUMM_0+(temp_info.imei_num[6]-'0'));
	gdi_draw_image(10+15*7,187,IMAGE_NUMM_0+(temp_info.imei_num[7]-'0'));
	gdi_draw_image(10+15*8,187,IMAGE_NUMM_0+(temp_info.imei_num[8]-'0'));
	gdi_draw_image(10+15*9,187,IMAGE_NUMM_0+(temp_info.imei_num[9]-'0'));
	gdi_draw_image(10+15*10,187,IMAGE_NUMM_0+(temp_info.imei_num[10]-'0'));
	gdi_draw_image(10+15*11,187,IMAGE_NUMM_0+(temp_info.imei_num[11]-'0'));
	gdi_draw_image(10+15*12,187,IMAGE_NUMM_0+(temp_info.imei_num[12]-'0'));
	gdi_draw_image(10+15*13,187,IMAGE_NUMM_0+(temp_info.imei_num[13]-'0'));
	gdi_draw_image(10+15*14,187,IMAGE_NUMM_0+(temp_info.imei_num[14]-'0'));

	bsp_lcd_update_screen(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1);
}

void sbit_demo_backlight(void)
{
	SBIT_DBG("============================================================::temp_info.sbit_backlight_flag :%d",temp_info.sbit_backlight_flag);
    if(temp_info.sbit_backlight_flag == false)
	{
        temp_info.sbit_backlight_flag = true;
		if(sbit_display_timer != NULL)
        xTimerStart(sbit_display_timer,0);
		
		if(sbit_key_timer != NULL)
		xTimerStart(sbit_key_timer,0); 
		
        /*backlight*/
		if(temp_info.stk8321_raise_mark!=1)
		{
			sbit_key_vib();
			temp_info.stk8321_raise_mark = 0;
		}
		bsp_lcd_display_on();
		sbit_show_meun();
    }
	else
	{
        if(temp_info.backlight_counter < SBIT_DISPLAY_BACKLIGHT_TIME)
		{
			if(sbit_display_timer != NULL)
            xTimerReset(sbit_display_timer, 10);
			if(temp_info.stk8321_raise_mark!=1)
			{
				sbit_key_vib();
				temp_info.stk8321_raise_mark = 0;
			}
            return; 
        }
		temp_info.backlight_counter = 0;
        temp_info.sbit_backlight_flag = false;
		if(sbit_display_timer != NULL)
        xTimerStop(sbit_display_timer,0);
		
		if(sbit_key_timer != NULL)
		xTimerStop(sbit_key_timer,0); 
		
        /*backlight*/
		bsp_lcd_display_off();
		
    }

}

void sbit_gpio_init()
{
	hal_gpio_init(HAL_GPIO_10);
	hal_pinmux_set_function(HAL_GPIO_10, 0); // Set the pin to operate in GPIO mode.
	hal_gpio_set_direction(HAL_GPIO_10, HAL_GPIO_DIRECTION_OUTPUT);
	hal_gpio_set_output(HAL_GPIO_10, HAL_GPIO_DATA_LOW);
	hal_gpio_deinit(HAL_GPIO_10);

	hal_gpio_init(HAL_GPIO_13);
	hal_pinmux_set_function(HAL_GPIO_13, 0); // Set the pin to operate in GPIO mode.
	hal_gpio_set_direction(HAL_GPIO_13, HAL_GPIO_DIRECTION_OUTPUT);
	hal_gpio_set_output(HAL_GPIO_13, HAL_GPIO_DATA_LOW);
	hal_gpio_deinit(HAL_GPIO_13);

	hal_gpio_init(HAL_GPIO_29);
	hal_pinmux_set_function(HAL_GPIO_29, HAL_GPIO_29_GPIO29);			  /*   set dierection to be output	*/			  
	hal_gpio_set_direction(HAL_GPIO_29, HAL_GPIO_DIRECTION_OUTPUT);
	hal_gpio_set_output(HAL_GPIO_29,HAL_GPIO_DATA_LOW);
	hal_gpio_deinit(HAL_GPIO_29);
	
	hal_gpio_init(HAL_GPIO_30);
	hal_pinmux_set_function(HAL_GPIO_30, HAL_GPIO_30_GPIO30);				  
	hal_gpio_set_direction(HAL_GPIO_30, HAL_GPIO_DIRECTION_INPUT);
	
	hal_gpio_init(HAL_GPIO_31);
	hal_pinmux_set_function(HAL_GPIO_31, HAL_GPIO_31_GPIO31);				  
	hal_gpio_set_direction(HAL_GPIO_31, HAL_GPIO_DIRECTION_INPUT);
	
	hal_gpio_init(HAL_GPIO_35);
	hal_pinmux_set_function(HAL_GPIO_35, HAL_GPIO_35_GPIO35);				  
	hal_gpio_set_direction(HAL_GPIO_35, HAL_GPIO_DIRECTION_INPUT);
	
	hal_gpio_init(H10_WTD_EN);
	hal_pinmux_set_function(H10_WTD_EN, 0); // Set the pin to operate in GPIO mode.
	hal_gpio_set_direction(H10_WTD_EN, HAL_GPIO_DIRECTION_OUTPUT);
	hal_gpio_set_output(H10_WTD_EN, HAL_GPIO_DATA_LOW);
	
	hal_gpio_init(WIFI_RX);
	hal_pinmux_set_function(WIFI_RX, HAL_GPIO_12_GPIO12);			  	  
	hal_gpio_set_direction(WIFI_RX, HAL_GPIO_DIRECTION_INPUT);
	
	hal_gpio_init(WIFI_TX);
	hal_pinmux_set_function(WIFI_TX, HAL_GPIO_13_GPIO13);			  	  
	hal_gpio_set_direction(WIFI_TX, HAL_GPIO_DIRECTION_INPUT);
	
	hal_pinmux_set_function(WIFI_CH, HAL_GPIO_9_GPIO9);			  		  
	hal_gpio_set_direction(WIFI_CH, HAL_GPIO_DIRECTION_OUTPUT);
	hal_gpio_set_output(WIFI_CH,HAL_GPIO_DATA_LOW);
	
	hal_pinmux_set_function(WIFI_WORK_EN, HAL_GPIO_11_GPIO11);			 /* 唤醒*/   		  
	hal_gpio_set_direction(WIFI_WORK_EN, HAL_GPIO_DIRECTION_OUTPUT);
	hal_gpio_set_output(WIFI_WORK_EN,HAL_GPIO_DATA_LOW);

	hal_gpio_set_direction(WIFI_EN, HAL_GPIO_DIRECTION_OUTPUT);
	hal_gpio_set_output(WIFI_EN, HAL_GPIO_DATA_LOW);
	hal_eint_deinit(WIFI_EN);

	hal_gpio_init(HAL_GPIO_3);
	hal_pinmux_set_function(HAL_GPIO_3, HAL_GPIO_3_GPIO3);			  	  
	hal_gpio_set_direction(HAL_GPIO_3, HAL_GPIO_DIRECTION_INPUT);
	
	hal_gpio_init(HAL_GPIO_4);
	hal_pinmux_set_function(HAL_GPIO_4, HAL_GPIO_4_GPIO4);			  	  
	hal_gpio_set_direction(HAL_GPIO_4, HAL_GPIO_DIRECTION_INPUT);
	
	hal_gpio_init(HAL_GPIO_6);
	hal_pinmux_set_function(HAL_GPIO_6, 4); // Set the pin to operate in GPIO mode.
	hal_gpio_deinit(HAL_GPIO_6);

	hal_gpio_init(HAL_GPIO_7);
	hal_pinmux_set_function(HAL_GPIO_7, 4); // Set the pin to operate in GPIO mode.
	hal_gpio_deinit(HAL_GPIO_7);
	
	
}
void heart_bag_timer_sent()
{
  static int time = 0;
  time ++;
  if(time>120)
  	{
      time = 0;
	  H10_heart_bag_T51();
    
    }
  SBIT_DBG("heart_bag_timer_sent:%d,network:%d",time,temp_info.network_sending_flag); 
  
}

void sbit_display_timer_callback(TimerHandle_t handle)
{
	static int flag = 0;
	
    if((temp_info.animation_flag == 0)&&(flag <= 2)
	 &&(exception_auto_reboot_mark == 1))
    {
        flag++;
		sbit_demo_display_send_msg(DISPLAY_ANIMATION,0);
	}
	else
	{
		sbit_demo_display_send_msg(DISPLYA_UPDATE,0);
		temp_info.backlight_counter++;
		if(temp_info.backlight_counter > SBIT_DISPLAY_BACKLIGHT_TIME)
		{
			sbit_demo_display_send_msg(DISPLAY_BACKLIGHT,0);
		}
	}
}

void sibt_m2m_ct_Queue(void)
{
	static int rtc_sec = 0;
	hal_rtc_time_t curtime;	
	
	
	hal_rtc_get_time(&curtime);//获取rtctime
	
	
	if(NVRAM_info.movement_flag == 1)
	{
		if(((curtime.rtc_sec%20)==0)&&(rtc_sec == 0)&&(temp_info.recv_flag == 0))
		{
			SBIT_DBG("ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ:: sibt_m2m_ct_Queue111  ::ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ");
			rtc_sec = 1;
			if((get_m2m_send_Queue()>=0)&&(NVRAM_info.network_fail_record <= 2))
			{
				temp_info.no_service_flag = 0;
				temp_info.network_sending_flag = 1;
				NVRAM_info.network_fail_record ++;
				ctiot_lwm2m_client_send_response("sibt_m2m_ct_Queue1");
				sbit_m2m_ct_send_massege(M2M_CT_temp_send_Queue,NULL,0,0);
			}
			else if((temp_info.registered_network_flag==0)&&(temp_info.no_service_flag <= 2))
			{
				ctiot_lwm2m_client_send_response("sibt_m2m_ct_Queue2");
				if(temp_info.no_service_flag == 2)
				set_phone_functionality(false);
				temp_info.no_service_flag ++;
			}
		}
		else
			rtc_sec = 0;

	}
	else 
	{
		if((curtime.rtc_sec == ((temp_info.imei_num[14]-'0')*3))&&(rtc_sec == 0)&&(temp_info.recv_flag == 0))
		{
			SBIT_DBG("ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ:: sibt_m2m_ct_Queue222  ::ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ");
			rtc_sec = 1;
			if((get_m2m_send_Queue()>=0)&&(NVRAM_info.network_fail_record <= 2))
			{
				temp_info.no_service_flag = 0;
				temp_info.network_sending_flag = 1;
				NVRAM_info.network_fail_record ++;
				ctiot_lwm2m_client_send_response("sibt_m2m_ct_Queue1");
				sbit_m2m_ct_send_massege(M2M_CT_temp_send_Queue,NULL,0,0);
			}
			else if((temp_info.registered_network_flag==0)&&(temp_info.no_service_flag <= 2))
			{
				ctiot_lwm2m_client_send_response("sibt_m2m_ct_Queue2");
				if(temp_info.no_service_flag == 2)
				set_phone_functionality(false);
				temp_info.no_service_flag ++;
			}
		}
		else
			rtc_sec = 0;

	}

}

void sos_send_data(void)/* gps定位成功后补发SOS消息 jerry add */
{
	temp_info.gps_sos_flag=0;
	sbit_m2m_ct_send_massege(M2M_CT_sos_data_T0,NULL,0,0);
}
extern void sbit_m2m_ct_delect_delay_hanlde(void);
int sos_send_delay = 0;
TimerHandle_t sos_timer_init=NULL;

void sos_PopupBox(void) 
{
	static int mark=0;
	
	if((strstr(temp_info.imei_num,"000000000000000"))!=NULL)
	{
	    temp_info.fm_mode = 1;
		temp_info.show_idle_flag=IDLE_SCREEN;
		SBIT_DBG("show idle_flag %d [LINE:%d]",temp_info.show_idle_flag,__LINE__);
		sbit_show_fm();	
	}
	else //if (sos_send_delay == 0)
	{
		temp_info.sos_mark = 1;
		temp_info.gps_sos_mark = 1;
		temp_info.m2m_key_flag = 0;
		temp_info.weather_screen_flag = 0;
		temp_info.show_idle_flag=IDLE_SCREEN;
		SBIT_DBG("show idle_flag %d [LINE:%d]",temp_info.show_idle_flag,__LINE__);
		temp_info.wifi_timing_flag = 0;
		temp_info.network_time_cumulative = (temp_info.reset_time*57);
		temp_info.wifi_repeat_init_flag = 0;
		//sbit_m2m_ct_delect_delay_hanlde();
		WIFI_driver_init();
		mmi_stop_heart_rate_count();
		sbit_set_vib_time(1,2);
		sbit_m2m_ct_send_massege(M2M_CT_sos_data_T0,NULL,0,0);
		//sbit_m2m_ct_send_massege(M2M_CT_temp_send_Queue,NULL,0,0);
		sos_send_delay = 60;
		xTimerStart(sos_timer_init,0);
	}

}

void auto_exit_idle(void)
{
    if(temp_info.fm_mode == 0)
    {
		mmi_stop_heart_rate_count();
		//gps_power_off();
		temp_info.activity_idle_flag = 0;
		temp_info.weather_screen_flag = 0;
		temp_info.weather_delay_flag = 0;
		temp_info.show_idle_flag = IDLE_SCREEN;
		SBIT_DBG("show idle_flag %d [LINE:%d]",temp_info.show_idle_flag,__LINE__);
		sbit_demo_display_send_msg(DISPLYA_UPDATE,0);
	}
}

void exit_idle(void)
{
	mmi_stop_heart_rate_count();
	gps_power_off();
    temp_info.show_idle_flag = IDLE_SCREEN;
	SBIT_DBG("show idle_flag %d [LINE:%d]",temp_info.show_idle_flag,__LINE__);
}

extern pmu_vsim_voltage_t pmu_get_vsim_voltage(void);
/* 
	1、久坐提醒只有在早上8点到晚上22点之间执行；
	2、当  temp_info.shake_flag 为非0值时就认为是正常佩戴状态；
	3、当  temp_info.Sedentary_Remind_Time 等于 3600 ，temp_info.shake_flag 为非0值时触发久坐提醒 。
*/
void Sedentary_remind(void)
{
	static int shake_flag = 0;
	static int timekeeping = 0;

	if(shake_flag != temp_info.shake_flag)
	{
		timekeeping = 0;
		shake_flag = temp_info.shake_flag;
	}
	else
	{
		timekeeping ++;
		if(timekeeping >= (60*15)) /* 连续15分钟没有数值变化默认腕表脱落 */
		{
			shake_flag = 0;
			temp_info.shake_flag = 0;
			temp_info.Sedentary_Remind_Time = 0;	
		}
	}

	if(temp_info.Sedentary_Remind_Time >= (60*60)) /* 久坐提醒计时器 */
	{
	   #if 0
	    if(temp_info.shake_flag != 0)  
	    {
           /* 当  temp_info.Sedentary_Remind_Time 大于等于 3600 ，temp_info.shake_flag 为非0值时触发久坐提醒 。*/
		   temp_info.Sedentary_idle_flag = 1;
		   sbit_m2m_ct_send_massege_isr(M2M_DISPLAY_BACKLIGHT,0,0,1);
		}
	   #endif
		temp_info.Sedentary_Remind_Time = 0;	
	}
	else
		temp_info.Sedentary_Remind_Time ++;   
	

}

//extern unsigned char	 m_byModuleData[33][33];
//extern bool EncodeData(char *lpsSource);
static void key_eint_hisr(void);
extern char prtbuf[100];

extern signed char stkMotion_chip_read_xyz(signed short *x,signed short *y,signed short *z);

bool G_wifi_interval = false;

void wifi_interval_sub()
{
	
	static int wifi_tmp = 0;
	/*每秒跑一次*/
	if(temp_info.usb_connect_flag == 0)
	{
		if(wifi_tmp==0)
		  wifi_tmp = NVRAM_info.location_interval*60;
		if(wifi_tmp>0)
			--wifi_tmp;
		if(wifi_tmp== 0)
		  G_wifi_interval = true;
	}
 
}


void sibt_demo_cycle(void)
{
	static int mark = 0;
	static int patch = 0;
	static int functionality_mark = 0;
	static int rtc_sec = 0;
	static int backlight_flag = 0;
	static int forced_reset_flag = 0;
	static int gsensor = 0;
	static int move = 0;
	static int move_flag = 0;
	static int wifi_positioning_flag = 0;
	static int waggle_flag = 0;
	uint8_t rtc_week = 0;	
	int test = 0;
	static int wifi_flag = 0;
	signed short x,y,z; 
	static int reset_flag = 0;
	static int sedentary_time_temp = 0;
    static bool sync_time = true;
	int i = 0;
	int rtc_min = 0;
	hal_rtc_time_t curtime;	
	int state = 0;

	hal_rtc_get_time(&curtime);//获取rtctime
	SBIT_DBG("exception_auto_reboot_mark: %d,stk8321_raise_mark: %d,",exception_auto_reboot_mark,temp_info.stk8321_raise_mark);
	
	SBIT_DBG("vib_data [%d,%d,%d,%d]",temp_info.vib_conunt,temp_info.vib_mark,temp_info.vib_tips_conunt,temp_info.vib_tips_mark);
//	SBIT_DBG("time:[%d-%d-%d %d:%d  %d]",curtime.rtc_year,curtime.rtc_mon,curtime.rtc_day,curtime.rtc_hour,curtime.rtc_min,curtime.rtc_sec);

	SBIT_DBG(">>>show_meun_id=%d, shut_down_flag=%d, [%d,%d,%d,%d]",get_sbit_show_meun_id(),temp_info.shut_down_flag,
	temp_info.set_mode_flag,temp_info.Sedentary_idle_flag,temp_info.weather_screen_flag,temp_info.activity_idle_flag);

	
	SBIT_DBG("network_time_cumulative :%d, get_m2m_send_Queue() :%d, g_mqtt_id: %d",temp_info.network_time_cumulative,get_m2m_send_Queue(),g_mqtt_id);
	if(sos_send_delay > 0)
	{
		sos_send_delay--;
	}
	else
	{
		sos_send_delay = 0;
	}
	
	temp_info.csq_num= getCSQ();

	
	if((temp_info.sbit_backlight_flag == false)&&(temp_info.watch_state <= 60))
	key_eint_hisr();
	
	if(mark==1)
	{
	  if(exception_auto_reboot_mark==1)
	  {
	  	 hal_gpio_set_output(H10_VIBR_EN,HAL_GPIO_DATA_HIGH);//kyb 开机震动
	  	 task_start_timer(TaskTimer_mada_off,1000,off_vibe);
	  }	 
	  convert_build_time();
	  Hrs3300_chip_init();
	}
	else if(mark==2)
	{
		NVRAM_info.network_fail_record = 0; 
		ril_query_imei_number();
		mmi_stop_heart_rate_count();
		sbit_check_battery_data();
	}
	else if(mark==3)
	{
		/*充电开机控制*/
		ril_query_imsi_number();
		hal_gpio_init(HAL_GPIO_27);
		hal_pinmux_set_function(HAL_GPIO_27, 0); // Set the pin to operate in GPIO mode.
		hal_gpio_set_direction(HAL_GPIO_27, HAL_GPIO_DIRECTION_OUTPUT);
		hal_gpio_set_output(HAL_GPIO_27, HAL_GPIO_DATA_HIGH);
		hal_gpio_deinit(HAL_GPIO_27);
	}
	else if((mark==10)&&(exception_auto_reboot_mark==1))
		WIFI_driver_init();
	
	if(mark == 5)
	{
		hal_stk8321_driver_task();
		//if(exception_auto_reboot_mark==1)
		{
			//sbit_m2m_ct_send_massege(M2M_CT_T71,NULL,0,0);
			sbit_m2m_ct_send_massege(M2M_CT_data_T100,NULL,0,0);
		}
 	}
	
   if(mark == 60)
	{
	 if(temp_info.Hw_Version == 0)
		{
		 temp_info.Hw_Version = 2;
		 WIFI_driver_init();
		}
	}

	
	//if(mark == 45)
	//EncodeData("123456789");
	
	//if((mark > 45)&&(curtime.rtc_sec < 33))
	//ctiot_lwm2m_client_send_response(m_byModuleData[curtime.rtc_sec]);
	
    if(mark<256)
	mark++;
	
	if((temp_info.Hw_Version == 1)||(temp_info.Hw_Version == 0))
	{
		if(uart_init_success == 1)
		{
			if(wifi_flag == 3)
			{
				Wifi_At_Send();
				wifi_flag = 0;
				uart_init_success = 0;
			}
			wifi_flag ++;
		}
	}
	if(temp_info.wifi_wlap_success == 1)
	{
		int y = 0 , j = 0 , z = 0 ;
		#if 0
	    j = ((get_m2m_send_Queue())+1);
		if(j > 0)
		{
			for(y = 0; y < j; y++)	
			{
				if((strstr(temp_info1.m2m_send_temp_Queue[y],":T47:3,"))!=NULL)
	            z = 1 ;
			}
		}
 		
		
		if(((temp_info.weather_screen_flag == 1)||(temp_info.weather_updata_flag == 0))&&(z == 0))
		{
			//H10_send_weather_data_T47();
		}
		#endif
		Icare_wifi_send(temp_info.total_buffer_wlap);
		if(temp_info.Hw_Version == 2)
		{
			temp_info.wifi_wlap_success = 0;
		}
		else
		{
			if(temp_info.fm_mode == 0)
			temp_info.wifi_wlap_success = 0;
		}
		
		WIFI_power_off();
	}
	
	if((curtime.rtc_year+2000>=2018)&&(mark>15))
	{    
		
#ifdef MTK_H10_DEBUG   
	    if(((curtime.rtc_min%2)==0)&&((curtime.rtc_sec==0)||(curtime.rtc_sec==1)))
		{
			temp_info.hartrate_cnt=0;
			Hrs3300_chip_init();
	    } 
        else if(((curtime.rtc_min%2)==0)&&((curtime.rtc_sec==45)||(curtime.rtc_sec==46))&&(rtc_sec == 0))
		{
		    if(temp_info.hartrate_cnt == 0)
		    temp_info.hartrate_cnt=66;
		    rtc_sec = 1;
			if((get_sbit_show_meun_id()!=HEART_RATE_SCREEN)&&(get_sbit_show_meun_id()!=BLOOD_SCREEN))
			mmi_stop_heart_rate_count();
		}
		else
		    rtc_sec = 0; 
#else
        #if 0  //N1 删除
		if(NVRAM_info.medicine_set_flag == 1)
		{
		 
			if((check_medicine_remind() == true) && (medicine_lock_sec == 0))
			{
				memset(curr_remind_time_str, 0, sizeof(curr_remind_time_str));
				min_next_remin_num = 0;
				get_curr_time_medicine_remind(curr_remind_time_str);
				min_next_remin_num = get_min_next_medicine_remind();
				clear_curr_medicine_remind();
				medicine_remind_delay = 1;
				sbit_m2m_ct_send_massege_isr(M2M_DISPLAY_BACKLIGHT,NULL,0,1);
				sbit_set_vib_time(SBIT_DISPLAY_BACKLIGHT_TIME,1);
				medicine_lock_sec = 120;
			}
		 
			if(medicine_lock_sec > 0)
			{
				medicine_lock_sec--;
			}
			
		}
		if(clear_medicine_remind_flag == true)
		{
			clear_medicine_remind();
			sbit_m2m_ct_send_massege(M2M_CT_ack_T87,NULL,0,0);
			clear_medicine_remind_flag = false;
		}
		#else
		wifi_interval_sub();
		
		#endif
		if(NVRAM_info.movement_flag == 1)
		{
			if(((curtime.rtc_sec%30)==0)&&(temp_info.watch_state <= 1800))
			{
				temp_info.hartrate_cnt=0;
				if(temp_info.usb_connect_flag == 0) /* 充电时不发送和采集数据*/
				Hrs3300_chip_init();
			}
			else if(((curtime.rtc_sec%28)==0)&&(rtc_sec == 0))
			{
				rtc_sec = 1;
				if(((curtime.rtc_min%5)==0)&&(curtime.rtc_sec == 28))
					mmi_stop_heart_rate_count();
				else
				{
					if((temp_info.blood_lbp>=40)&&(temp_info.blood_lbp<=200))
					sbit_m2m_ct_send_massege(M2M_CT_hartblood_data_T45,NULL,0,0);
					else if((temp_info.hartrate_cnt>=40)&&(temp_info.hartrate_cnt<=200))
					sbit_m2m_ct_send_massege(M2M_CT_hartrate_data_T28,NULL,0,0);
				}
			}
			else
				rtc_sec = 0;
		}
		else
		{

			if((curtime.rtc_min==45)&&((curtime.rtc_sec==0)||(curtime.rtc_sec==1))&&(temp_info.watch_state <= 1800))
			{
				temp_info.hartrate_cnt=0;
				if(temp_info.usb_connect_flag == 0) /* 充电时不发送和采集数据*/
				Hrs3300_chip_init();
			}
			else if((curtime.rtc_min==45)&&((curtime.rtc_sec==45)||(curtime.rtc_sec==46))&&(rtc_sec == 0))
			{
				rtc_sec = 1;
				if((get_sbit_show_meun_id()!=HEART_RATE_SCREEN))//&&(get_sbit_show_meun_id()!=BLOOD_SCREEN)
				mmi_stop_heart_rate_count();
			}
			else
				rtc_sec = 0;

		}

		//if(((curtime.rtc_min%2) == 0)&&((curtime.rtc_sec == 0)||(curtime.rtc_sec == 1)))
		rtc_min = (curtime.rtc_hour*60)+curtime.rtc_min;
		
		//NVRAM_info.location_interval = 5;
		//NVRAM_info.step_interval = 100;
		if(((curtime.rtc_hour%6) == 0)&&(curtime.rtc_min == 30)
		&&((curtime.rtc_sec == 0)||(curtime.rtc_sec == 1))
		&&(NVRAM_info.Power_saving_flag == 1))/* 开启省电模式6个小时上报一次定位 */
		{
		    temp_info.wifi_timing_flag = 1;
			WIFI_driver_init();
		}
		else if(G_wifi_interval && (NVRAM_info.location_interval>0)&&
		//(NVRAM_info.Power_saving_flag == 0)&&
		(temp_info.usb_connect_flag == 0)&& /* 充电时不发送和采集数据*/
		(temp_info.watch_state <= 1800))  
		{
		   temp_info.wifi_timing_flag = 1;
		   G_wifi_interval = false;
		   WIFI_driver_init();
		}
		else if((((NVRAM_info.value_steps - NVRAM_info.auto_wifi) >= NVRAM_info.step_interval)) /* 每移动 x 步，就启动一次WIFI定位*/
	    &&(NVRAM_info.step_interval>0))//&&(NVRAM_info.Power_saving_flag == 0))
		{
		   NVRAM_info.auto_wifi = NVRAM_info.value_steps;
		   temp_info.wifi_timing_flag = 1;
		   WIFI_driver_init();
		}
		else if((curtime.rtc_min == 30)&&((curtime.rtc_sec == 0)||(curtime.rtc_sec == 1))
		&&((curtime.rtc_hour%2) == 0)&&((curtime.rtc_hour >=8 )&&(curtime.rtc_hour <= 20))&&
		(temp_info.watch_state <= (30*60)))  /* 早上7点到晚上10点每一个小时定位一次 */
		{
		    temp_info.wifi_timing_flag = 1;
			if((NVRAM_info.location_interval == 0)&&
			(NVRAM_info.step_interval == 0)&&
			(NVRAM_info.Power_saving_flag == 0)&&
			(wifi_positioning_flag == 0))
			{
				WIFI_driver_init();
			}
		}
		else if((curtime.rtc_min == 30)&&
		((curtime.rtc_hour%4) == 0)&&
		((temp_info.watch_state >= (30*60))||(curtime.rtc_hour < 8))&&
		((curtime.rtc_sec == 0)||(curtime.rtc_sec == 1)))/* 终端休眠半个小时后，定位数据每4个小时上报一次 */
		{
		    temp_info.wifi_timing_flag = 1;
			if((NVRAM_info.location_interval == 0)&&
			(NVRAM_info.step_interval == 0)&&
			(NVRAM_info.Power_saving_flag == 0))
			WIFI_driver_init();
		}
		else if((((NVRAM_info.value_steps - NVRAM_info.auto_wifi) >= 500)&& /* 每移动 500 步，就启动一次WIFI定位*/
		(NVRAM_info.step_interval == 0)&&(NVRAM_info.location_interval == 0))) 
		{
		    NVRAM_info.auto_wifi = NVRAM_info.value_steps;
		    temp_info.wifi_timing_flag = 1;
			if((NVRAM_info.location_interval == 0)&&
			(NVRAM_info.step_interval == 0)&&
			(NVRAM_info.Power_saving_flag == 0))
			{
				wifi_positioning_flag = (30*60);
				WIFI_driver_init();
			}
		}
		//sibt_m2m_ct_Queue();
#endif
		
		if(/*((curtime.rtc_hour==1)&&(curtime.rtc_min==5))||    /* 每天晚上定时重启 */
		((temp_info.network_time_cumulative >= (temp_info.reset_time*60))&&  /* 连续90分钟没有发送成功数据	*/
		(get_m2m_Queue_count()>= 2)&&					    /*  队列缓存数据大于等于 2条 */
		(temp_info.sbit_backlight_flag == false)))		    /*  灭屏状态下 */
		{
            if(((curtime.rtc_sec == 0) || (curtime.rtc_sec == 1)) && (forced_reset_flag == 0))
			{
			    forced_reset_flag = 1;
				sbit_nvram_write();				
				SBIT_DBG("---hal_rtc_enter_forced_reset_mode\r\n");
				hal_rtc_enter_forced_reset_mode();
			}
		} 
		if((temp_info.network_time_cumulative>=1800)&&(get_m2m_Queue_count()>= 2))
		{
			delay_cfun_OffOn();
		}
		
		//sibt_m2m_ct_Queue();
		
		if((curtime.rtc_hour <= 22)&&(curtime.rtc_hour >= 8)&&(NVRAM_info.sedentary_flag == 1))
		Sedentary_remind();   /* 久坐提醒 */

		if(NVRAM_info.inited_flag!=10086)
		{ 
	        NVRAM_info.inited_flag = 10086;
	        NVRAM_info.Bright_screen_flag = 1;
            NVRAM_info.location_interval = 120;
			NVRAM_info.step_interval = 500;
 	
        }
	}
#if 0
	else if(mark == 60)
		set_phone_functionality(false); 
	else if(mark == 65)
		set_phone_functionality(true); 
	else if(mark == 150)
		set_phone_functionality(false); 
#endif
	
    if(curtime.rtc_sec==0)
	{
		sbit_nvram_write();
		sbit_check_battery_data();
		SBIT_DBG("temp_info.Hw_Version	 %d : \r\n" ,temp_info.Hw_Version);
		SBIT_DBG("==============> temp_info.iccid <==============%s\r\n",temp_info.iccid);
		SBIT_DBG("==============> temp_info.imsi_num <==============%s\r\n",temp_info.imsi_num);
		SBIT_DBG("==============> temp_info.reset_time <==============%d\r\n",temp_info.reset_time);
		SBIT_DBG("==============> temp_info.watch_state <==============%d\r\n",temp_info.watch_state);
		SBIT_DBG("==============> temp_info.weather_updata_flag <==============%d\r\n",temp_info.weather_updata_flag);
		SBIT_DBG("==============> temp_info.network_time_cumulative <==============%d\r\n",temp_info.network_time_cumulative);
		SBIT_DBG("========= CSQ =========:%d,======== registered_network_flag ========= :%d", temp_info.csq_num,temp_info.registered_network_flag);
	}
	
    if((curtime.rtc_sec%10)==0)
	{
		SBIT_DBG("==============> temp_info.recv_flag <==============%d\r\n",temp_info.recv_flag);
		//sleep_manager_dump_all_user_lock_sleep_handle();
	}
	
	sbit_vib_timer_callback();
	H10_translate_step_or_hart_data();
	
/*	
	if((temp_info.gps_sos_flag>=11)    
	&&(temp_info.gps_sos_mark == 1))
	sos_send_data();  /* 定位成功后发送sos 经纬度数据  jerry  add                     2022_0804    kyb moditfy*/
	
	

	if((temp_info.sbit_backlight_flag == false)&&(get_sbit_show_meun_id() != IDLE_SCREEN))  /* 灭屏后1分钟自动返回idle界面 */
	{
	    if((backlight_flag == 60)&&(get_sbit_show_meun_id() != GPS_SCREEN))
		auto_exit_idle();
		
		if(backlight_flag <= 60)
		backlight_flag ++;
	}
	else
		backlight_flag = 0;
	
	hal_wdt_feed(HAL_WDT_FEED_MAGIC);
	
	stkMotion_chip_read_xyz(&x,&y,&z);
	
	test = abs(x+y+z);
	
	if((abs(gsensor-test))>=120)
	{
		temp_info.watch_state = 0;
	}
	else
	{
		temp_info.watch_state ++;
	}
	
	//SBIT_DBG("==============> (abs(gsensor-test)) <==============%d\r\n",(abs(gsensor-test)));
	//SBIT_DBG("==============> temp_info.watch_state <==============%d\r\n",temp_info.watch_state);

    if(curtime.rtc_sec == 0)
    {
        if(temp_info.usb_connect_flag == 0)/* 充电时不执行*/
        {
			if(((move <(stk_Pedometer_GetCount()))&&(move>0))||(move_flag > 0))  /*  move_flag 移动过后第一次静止默认为移动*/
			{
			    if((move_flag == 0)||((move <(stk_Pedometer_GetCount()))&&(move>0)))
				move_flag = 1;
				else if(move_flag > 0)
				move_flag --;
				NVRAM_info.move_time ++;
			    if(curtime.rtc_hour<8)
				sedentary_time_temp ++;
			}			
			else if((temp_info.watch_state <= 60)&&(move_flag == 0))
			{
			    if(curtime.rtc_hour<8)
				sedentary_time_temp ++;
				else
				sedentary_time_temp = 0;
				
				if(curtime.rtc_hour >= 8)
				NVRAM_info.sedentary_time ++;
			}

			if((curtime.rtc_hour>=20)||(curtime.rtc_hour<8))
			{
				rtc_week = CaculateWeekDay(curtime.rtc_year+2000,curtime.rtc_mon,curtime.rtc_day);
				if(NVRAM_info.sleep_reset_flag==0)
				NVRAM_info.sleep_reset_flag = rtc_week;

				if((curtime.rtc_hour >= 6)&&(waggle_flag == 0))
				{
					for(i = 0; i < 6; i++)	
					{
					   if(NVRAM_info.waggle_level[i]==0)
					   waggle_flag ++; /* 计算晚上0点到6点几个小时是静止状态 */
					}
				}
				
				if((temp_info.watch_state >= (3600*2))||(waggle_flag >= 3)||/* 晚上2个小时终端没有动，判断为晚上睡觉没有佩戴过，累计睡眠、久坐数据清空*/
					((rtc_week != NVRAM_info.sleep_reset_flag)&&(curtime.rtc_hour>=20))
					||((curtime.rtc_hour < 6)&&(sedentary_time_temp > 150)))  /* 晚上睡眠监测时间累加150分钟又走动或者久坐，默认睡眠监测数据为0*/
				{
				    sedentary_time_temp = 0;
					NVRAM_info.light_sleep_time = 0;				 
					NVRAM_info.deep_sleep_time = 0; 
				}
				else if((move == (stk_Pedometer_GetCount()))&&(temp_info.watch_state >= 60)) /* 在期间没有走动过 */
				{
				    if(temp_info.watch_state >= (60*15))/* 晚上15分钟以上未晃动过，判断为深度睡眠*/
				    {
						NVRAM_info.light_sleep_time++; 
				    }
					else
					{
						NVRAM_info.deep_sleep_time++;	
					}
					if((temp_info.watch_state <= 150)&&(NVRAM_info.sedentary_time > 0)&&(curtime.rtc_hour<8))
					{
						NVRAM_info.sedentary_time--;
					}
				}
			}
			else
			{
				waggle_flag = 0;
				memset(NVRAM_info.waggle_level,0,sizeof(NVRAM_info.waggle_level)); /* 晃动等级 */
			}
			
				move = stk_Pedometer_GetCount();
			
		}
		
		SBIT_DBG("==============> move_time <==============%d ,==========> sedentary_time <============== %d,%d,%d \r\n",NVRAM_info.move_time,NVRAM_info.sedentary_time,NVRAM_info.light_sleep_time,NVRAM_info.light_sleep_time);
		
	}
	
	gsensor = test;
	
	if((strlen(temp_info.MY_Latitude) != 0)&&
	((get_sbit_show_meun_id())==GPS_SCREEN)&&
	(temp_info.sos_mark == 0)&&
	((curtime.rtc_min%5)==0)&&
	(curtime.rtc_sec==0))
	sbit_m2m_ct_send_massege(M2M_CT_gps_data_T29,NULL,0,0);
	
	temp_info.network_time_cumulative ++;
	if(temp_info.gps_delay_flag > 0)
	temp_info.gps_delay_flag --; 
	
	if(temp_info.weather_updata_flag > 0)
	temp_info.weather_updata_flag --; 
	
	if(wifi_positioning_flag > 0)
	wifi_positioning_flag --;

	
	if(temp_info.recv_flag > 0)
	temp_info.recv_flag --;


}   

/* 按键事件封装 jerry add */
hal_keypad_key_state_t get_m2m_key(void)
{
    hal_keypad_key_state_t ret=0;
	
	if(temp_info.m2m_key_flag == 1)
	ret = HAL_KEYPAD_KEY_PRESS;		
	else if(temp_info.m2m_key_flag >= 4)
	ret = HAL_KEYPAD_KEY_LONG_PRESS;
	
    return ret;
}

static void key_eint_hisr(void)//SOS KEY
{
	hal_gpio_data_t input_gpio_data = HAL_GPIO_DATA_LOW;
	hal_gpio_status_t status;
	static int key_eint_hisr_mark=0;
	
	status = hal_gpio_get_input((hal_gpio_pin_t)HAL_GPIO_35, &input_gpio_data);
   
	if(input_gpio_data == 0)
	{
	    if(temp_info.sbit_backlight_flag == false)
	    {
			if(key_eint_hisr_mark >= 0)
			key_eint_hisr_mark += 6;
		}
		else
		{
			if(key_eint_hisr_mark >= 0)
			key_eint_hisr_mark ++;
			temp_info.backlight_counter = 0;
		}
	}
	
	if((key_eint_hisr_mark >= 30)&&(input_gpio_data == 0))  /* 长按 */
	{
		SBIT_DBG("ZZZZZZ:: long key	:ZZZZZZ:%d,%d",NVRAM_info.English_German_flag,lan_swflag);
		SBIT_DBG(">>>show_meun_id : %d, shut_down_flag :%d",get_sbit_show_meun_id(),temp_info.shut_down_flag);
		
		if((temp_info.animation_flag>0)&&(temp_info.shut_down_flag == 0))
        {
			key_eint_hisr_mark = (-1); 	
			if(((get_sbit_show_meun_id() == POWEROFF_SCREEN)||(temp_info.fm_mode == 1))&&
				(temp_info.sbit_backlight_flag == true))
			{
				temp_info.shut_down_flag=1;
				sbit_m2m_ct_send_massege_isr(M2M_SHUTDOWN,NULL,0,1);
			}
			else
			{
			    if(temp_info.set_mode_flag == 0)
			    {
					if(temp_info.sbit_backlight_flag == false)
					sbit_m2m_ct_send_massege_isr(M2M_DISPLAY_BACKLIGHT,0,0,1);
					sbit_m2m_ct_send_massege_isr(M2M_SOS_KEY,NULL,0,1);
				}
				else
				{
				    if(temp_info.set_mode_select == 4)
				    {
				      #if 0
                      if(NVRAM_info.Power_saving_flag == 0)
					  NVRAM_info.Power_saving_flag = 1;
					  else
					  NVRAM_info.Power_saving_flag = 0;
					  sbit_m2m_ct_send_massege(M2M_CT_data_T100,NULL,0,0);
					  #endif
					  
					}
					else if(temp_info.set_mode_select == 3)
				    {
				      if(NVRAM_info.Factory_Reset_flag == 0)
						{
							 NVRAM_info.Factory_Reset_flag = 1;
							 H10_Factory_Reset_T52();
						}
					  else
							NVRAM_info.Factory_Reset_flag = 0;
					 
					}
				    else if(temp_info.set_mode_select == 1)
				    {
				    #if 0
						if(NVRAM_info.data_syn_flag == 0)
						{
							NVRAM_info.data_syn_flag = 1;
							sbit_m2m_ct_send_massege(M2M_CT_data_T100,NULL,0,0);
						}
						else
						{
							NVRAM_info.data_syn_flag = 0;
						}
					#endif 
					
						if(NVRAM_info.Factory_Reset_flag == 0)
						{
							 NVRAM_info.Factory_Reset_flag = 1;
							 H10_Factory_Reset_T52();
						}
					  	else
							NVRAM_info.Factory_Reset_flag = 0;
					}
					
				    else if(temp_info.set_mode_select == 2)
				    {
				#if defined(LOGO_SUPPORT_3) 
					  if(NVRAM_info.movement_flag == 0)
					  NVRAM_info.movement_flag = 1;
					  else
					  NVRAM_info.movement_flag = 0;
				//#else	
					  if(NVRAM_info.sedentary_flag == 0)
					  NVRAM_info.sedentary_flag = 1;
					  else
					  NVRAM_info.sedentary_flag = 0;
				#endif
					if(NVRAM_info.Bright_screen_flag == 0)
						NVRAM_info.Bright_screen_flag = 1;
					else
						NVRAM_info.Bright_screen_flag = 0;

                   }
				#if defined(MTK_DATA_SYN_SETTING_SUPPORT)
					else if(temp_info.set_mode_select == 0)
					{
						if(NVRAM_info.English_German_flag == 0)
							NVRAM_info.English_German_flag = 1;
						else
							NVRAM_info.English_German_flag = 0;

					}
				#endif	
					sbit_m2m_ct_send_massege_isr(M2M_DISPLAY_BACKLIGHT,0,0,1);
				}
			}
		}
	}
	else if((key_eint_hisr_mark >= 1)&&(input_gpio_data == 1)&&(temp_info.shut_down_flag == 0))  /* 短按 */
	{
		if(temp_info.set_mode_flag != 0)
		{
		
		/*  if(temp_info.set_mode_select <= 2)
		  {
          	temp_info.set_mode_select++;
		  }    */
		  if(!temp_info.set_mode_select )
		  {
		  	temp_info.set_mode_select = 1;      /*只有语言设置栏和恢复出厂设置*/
		  }
		  else
		  {
		    temp_info.set_mode_select = 0;
			
		  }
		}
		else if(((get_sbit_show_meun_id()) != IDLE_SCREEN)&&
			  ((get_sbit_show_meun_id()) != POWEROFF_SCREEN))
		{
			auto_exit_idle();
		}
		
		key_eint_hisr_mark = 0;
		sbit_m2m_ct_send_massege_isr(M2M_DISPLAY_BACKLIGHT,0,0,1);
		
	}
	else if(input_gpio_data == 1)
	{
		key_eint_hisr_mark = 0;
	}
	
}

int get_sbit_show_meun_id()
{
   return temp_info.show_idle_flag;
}

TimerHandle_t sbit_backlight_offon_timer=NULL;
void delay_lcd_backlight_on(void)
{
 	lcd_backlight_offon(true);
}
void lcd_backlight_on(void)
{	
    if(sbit_backlight_offon_timer == NULL)
    sbit_backlight_offon_timer = xTimerCreate("shut_backlight", 
    200 / portTICK_PERIOD_MS, 
    pdFALSE, 
    NULL, 
    delay_lcd_backlight_on);
    xTimerStart(sbit_backlight_offon_timer,0);
}

void sbit_demo_display_init(void)
{
	display_msg_struct queue_item;
	hal_eint_config_t config;
	
		
   	hal_pinmux_set_function(H10_VIBR_EN, HAL_GPIO_1_GPIO1);			 /*   set dierection to be output  */	//马达控制		   
    hal_gpio_set_direction(H10_VIBR_EN, HAL_GPIO_DIRECTION_OUTPUT);   
    hal_gpio_set_output(H10_VIBR_EN,HAL_GPIO_DATA_LOW);
	hal_gpio_deinit(H10_VIBR_EN);
	
	bsp_lcd_init(0x00); /* Show a black screen*/
	gdi_init(display_fb);
	bsp_lcd_get_parameter(LCM_IOCTRL_QUERY__LCM_WIDTH, &UI_WIDTH);
	bsp_lcd_get_parameter(LCM_IOCTRL_QUERY__LCM_HEIGHT, &UI_HEIGHT);
    bsp_lcd_set_frame_buffer((uint8_t *)&display_fb);
    
    /*backlight*/
	hal_gpio_init(HAL_GPIO_32);
	hal_pinmux_set_function(HAL_GPIO_32, 0); // Set the pin to operate in GPIO mode.
	hal_gpio_set_direction(HAL_GPIO_32, HAL_GPIO_DIRECTION_OUTPUT);
	if(exception_auto_reboot_mark==1)
	hal_gpio_set_output(HAL_GPIO_32, HAL_GPIO_DATA_HIGH);
	hal_gpio_deinit(HAL_GPIO_32);
	
	if(sbit_key_timer == NULL)
    sbit_key_timer = xTimerCreate("key_timer", 
	50 / portTICK_PERIOD_MS, 
	pdTRUE, 
	NULL,
	key_eint_hisr);
    xTimerStart(sbit_key_timer,0); 

	temp_info.m2m_send_flag=0;
	temp_info.Hr_delay=0;
	temp_info.bp_delay=0;
	temp_info.gps_delay=0;
    temp_info.sbit_backlight_flag=1;
	temp_info.animation_flag=0;
	temp_info.fm_mode = 0;
	temp_info.no_service_flag = 0;
	temp_info.reset_time = 90;

	if(at_timer == NULL)
	at_timer = xTimerCreate("at_command_timer",100,true,NULL,sibt_demo_cycle);// 500就是5S
	
	if (at_timer == NULL) 
	{
	  SBIT_DBG("ZZZZZZ::timer create fail");
	} 
	else 
	{
	  if (xTimerStart(at_timer, 0) != pdPASS) 
	  {
		 SBIT_DBG("ZZZZZZ::xTimerStart fail");
	  }
	}
    
	if(sbit_display_timer == NULL)
    sbit_display_timer = xTimerCreate("sbit_timer", 
	1000 / portTICK_PERIOD_MS, 
	pdTRUE, 
	NULL,
	sbit_display_timer_callback);
    
    xTimerStart(sbit_display_timer, 5); 
		
	sbit_m2m_ct_init();

    sbit_deep_slee_lock = hal_sleep_manager_set_sleep_handle("sbit_ds_lock");
    hal_sleep_manager_acquire_sleeplock(sbit_deep_slee_lock, HAL_SLEEP_LOCK_DEEP);
    sbit_gpio_init();

	if(heart_bag_timer == NULL)
    heart_bag_timer = xTimerCreate("heart_bag_timer", 
	30*1000  / portTICK_PERIOD_MS, 
	pdTRUE, 
	NULL,
	heart_bag_timer_sent);//900*1000
    xTimerStart(heart_bag_timer,0); 

	if(sos_timer_init == NULL)
		sos_timer_init = xTimerCreate("sos_timer_init", 
		5*1000 / portTICK_PERIOD_MS, 
		pdFALSE, 
		NULL,
	gps_driver_init);


	sbit_nvram_read();
}




