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

// For Register AT command handler
// System head file

#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
#include <stdio.h>
#include "at_command.h"
#include <stdlib.h>

#if (PRODUCT_VERSION == 2625)
#ifdef MTK_BUILD_SMT_LOAD
#ifdef HAL_ADC_MODULE_ENABLED
#include "hal_adc.h"
/*
 * sample code
*/


static uint16_t adc_sample_to_voltage(uint32_t source_code)
{
    uint16_t voltage = (source_code * 1400) / 4095;
    return voltage;
}

/*--- Function ---*/
atci_status_t atci_cmd_hdlr_auxadc(atci_parse_cmd_param_t *parse_cmd);

/*
AT+EWDT=<op>                |   "OK"
AT+EWDT=?                   |   "+EAUXADC=(1)","OK"


*/
/* AT command handler  */
atci_status_t atci_cmd_hdlr_auxadc(atci_parse_cmd_param_t *parse_cmd)
{
    atci_response_t response = {{0}};
    char *param = NULL;
    char param_val;

    uint32_t adc_data;
    uint32_t adc_voltage;

    printf("atci_cmd_hdlr_auxadc \r\n");

    response.response_flag = 0; /*    Command Execute Finish.  */
#ifdef ATCI_APB_PROXY_ADAPTER_ENABLE
    response.cmd_id = parse_cmd->cmd_id;
#endif

    switch (parse_cmd->mode) {
        case ATCI_CMD_MODE_TESTING:    /* rec: AT+EAUXADC=?   */
            strcpy((char *)response.response_buf, "+EAUXADC:1, measure voltage of CH2.");
            response.response_flag |= ATCI_RESPONSE_FLAG_APPEND_OK;
            response.response_len = strlen((char *)response.response_buf);
            atci_send_response(&response);
            break;

        case ATCI_CMD_MODE_EXECUTION: /* rec: AT+EAUXADC=<op>  the handler need to parse the parameters  */
            if (parse_cmd->parse_pos < parse_cmd->string_len) {
                if ((*(parse_cmd->string_ptr + parse_cmd->parse_pos)) == '1') {
                    //hal_pinmux_set_function(HAL_GPIO_32, 5);
                    hal_adc_init();
                    hal_adc_get_data_polling(HAL_ADC_CHANNEL_2, &adc_data);
                    hal_adc_deinit();

                    adc_voltage = adc_sample_to_voltage(adc_data);
                    if ((adc_voltage > 800) && (adc_voltage < 1000)) {
                        sprintf((char *)response.response_buf, "+EAUXADC:adc_data = 0x%04x, adc_voltage = %dmV", adc_data, adc_voltage);
                        response.response_flag |= ATCI_RESPONSE_FLAG_APPEND_OK;
                    } else {
                        sprintf((char *)response.response_buf, "+EAUXADC:adc_data = 0x%04x, adc_voltage = %dmV", adc_data, adc_voltage);
                        response.response_flag |= ATCI_RESPONSE_FLAG_APPEND_ERROR;
                    }
                    response.response_len = strlen((char *)response.response_buf);
                    atci_send_response(&response);
                } else {
                    response.response_flag |= ATCI_RESPONSE_FLAG_APPEND_ERROR;
                    response.response_len = 0;
                    atci_send_response(&response);
                }

            } else {
                response.response_flag |= ATCI_RESPONSE_FLAG_APPEND_ERROR;
                response.response_len = 0;
                atci_send_response(&response);
            }
            break;

        default :
            /* others are invalid command format */
            strcpy((char *)response.response_buf, "ERROR Command.\r\n");
            response.response_flag |= ATCI_RESPONSE_FLAG_APPEND_ERROR;
            response.response_len = strlen((char *)response.response_buf);
            atci_send_response(&response);
            break;
    }
    return ATCI_STATUS_OK;
}

#endif
#endif
#else
atci_status_t atci_cmd_hdlr_auxadc(atci_parse_cmd_param_t *parse_cmd)
{
		parse_cmd = parse_cmd;
		return ATCI_STATUS_OK;
}
#endif

#ifndef __H10_TEMP__
#include "syslog.h"
#include "gdi.h"
#include "h10_mmi.h"
#include "image_info.h"
#include "hal_rtc.h"
#include "hal_gpio.h"
typedef unsigned short color ;
//typedef unsigned int uint32_t;

#define PELTEN_DBG(fmt,arg...)   LOG_I(sbit_demo, "[sbit]: "fmt,##arg)
//jerry add
/*This is adc_date   VOLTAGE_3400_V=3.4V */
#define VOLTAGE_FULL     3160
#define VOLTAGE_4350_V     3145
#define VOLTAGE_4300_V     3120
#define VOLTAGE_4200_V     3000
#define VOLTAGE_4100_V     2960
#define VOLTAGE_4000_V     2920
#define VOLTAGE_3900_V     2850
#define VOLTAGE_3800_V     2780 
#define VOLTAGE_3700_V     2680
#define VOLTAGE_3600_V     2590
#define VOLTAGE_3500_V     2520
#define VOLTAGE_3400_V     2420   
#define VOLTAGE_3300_V     2320   
extern unsigned short RGB888ToRGB565(unsigned int n888Color);
#if defined(YQ_WATCH_SPPPORT)
uint32_t mmi_adc_data = 0;
void sbit_check_battery_data(void)
{
    uint32_t adc_data;
    uint32_t adc_voltage;
	static int show_bat_cnt=0;;
	int c=0;
	static int mark=0;
	static int shutdown=0;
	hal_rtc_time_t curtime; 
	hal_gpio_data_t input_gpio_data = HAL_GPIO_DATA_LOW;
	
	hal_rtc_get_time(&curtime);
	//c = (curtime.rtc_hour*60*60)+(curtime.rtc_min*60)+curtime.rtc_sec;

    hal_adc_init();
    hal_adc_get_data_polling(HAL_ADC_CHANNEL_6, &adc_data);
    hal_adc_deinit();
	mmi_adc_data = adc_data;

	if((curtime.rtc_sec==0)||(curtime.rtc_sec==30))
    PELTEN_DBG("<<<<<adc_data>>>>>\r\n %d",adc_data);
	
	if (temp_info.usb_connect_flag==1)
	{
		//if (input_gpio_data == HAL_GPIO_DATA_HIGH)
		if (adc_data >= VOLTAGE_4350_V)
		mark++;
		else if(mark<300)
		mark=0;
		
		if ((adc_data >= VOLTAGE_4200_V)&&(mark>300))
		{
		#if defined(YQ_WATCH_SPPPORT)
		#else
			CH_gdi_draw_arc(120,120,116,35,240,0xCC33);
			CH_gdi_draw_arc(120,120,118,35,240,0xCC33);
			CH_gdi_draw_arc(120,120,117,35,240,0xCC33);
			CH_gdi_draw_arc(120,120,119,35,240,0xCC33);

			CH_gdi_draw_arc_again(120,120,118,0,120,0xCC33);
			CH_gdi_draw_arc_again(120,120,117,0,120,0xCC33);
			CH_gdi_draw_arc_again(120,120,116,0,120,0xCC33);
			CH_gdi_draw_arc_again(120,120,119,0,120,0xCC33);

			CH_gdi_draw_arc_high(120,120,116,0,120,0xCC33);
			CH_gdi_draw_arc_high(120,120,117,0,120,0xCC33);
			CH_gdi_draw_arc_high(120,120,118,0,120,0xCC33);
			CH_gdi_draw_arc_high(120,120,119,0,120,0xCC33);
		#endif	
			return;
		}
		
		   show_bat_cnt++;
	   if(show_bat_cnt==1)
	   	{
	   	}
	   else if(show_bat_cnt==2)
	   	{
	   	#if 0
	   	   CH_gdi_draw_arc(120,120,116,205,240,(RGB888ToRGB565(0x00BFFF)));
	       CH_gdi_draw_arc(120,120,118,205,240,(RGB888ToRGB565(0x00BFFF)));
	       CH_gdi_draw_arc(120,120,117,205,240,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc(120,120,119,205,240,(RGB888ToRGB565(0x00BFFF)));

		   CH_gdi_draw_arc_again(120,120,118,35,120,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc_again(120,120,117,35,120,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc_again(120,120,116,35,120,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc_again(120,120,119,35,120,(RGB888ToRGB565(0x00BFFF)));
	       gdi_draw_circle(37,203,1,0xFF99);
		   gdi_draw_image(35,200,IMAGE_INDEX_CIRCLE_DIAN);
		#endif		   
	   	 }
	   else if(show_bat_cnt==3)
	   	{
	   	#if 0
		   CH_gdi_draw_arc(120,120,116,120,240,(RGB888ToRGB565(0x00BFFF)));
	       CH_gdi_draw_arc(120,120,118,120,240,(RGB888ToRGB565(0x00BFFF)));
	       CH_gdi_draw_arc(120,120,117,120,240,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc(120,120,119,120,240,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc_again(120,120,118,0,120,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc_again(120,120,117,0,120,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc_again(120,120,116,0,120,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc_again(120,120,119,0,120,(RGB888ToRGB565(0x00BFFF)));
		   gdi_draw_image(33,200,IMAGE_INDEX_CIRCLE_DIAN);
		   gdi_draw_image(0,118,IMAGE_INDEX_CIRCLE_DIAN);
		  #endif 
	   }
	else if (show_bat_cnt==4)
	   {
	   #if 0
		   CH_gdi_draw_arc(120,120,116,35,240,(RGB888ToRGB565(0x00BFFF)));
	       CH_gdi_draw_arc(120,120,118,35,240,(RGB888ToRGB565(0x00BFFF)));
	       CH_gdi_draw_arc(120,120,117,35,240,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc(120,120,119,35,240,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc_again(120,120,118,0,120,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc_again(120,120,117,0,120,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc_again(120,120,116,0,120,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc_again(120,120,119,0,120,(RGB888ToRGB565(0x00BFFF)));
		   gdi_draw_image(33,200,IMAGE_INDEX_CIRCLE_DIAN);
		   gdi_draw_image(0,118,IMAGE_INDEX_CIRCLE_DIAN);
		   gdi_draw_image(35,35,IMAGE_INDEX_CIRCLE_DIAN);
		  #endif 
	   }
	else if (show_bat_cnt==5)
	   {
	   #if 0
		   CH_gdi_draw_arc(120,120,116,35,240,(RGB888ToRGB565(0x00BFFF)));
	       CH_gdi_draw_arc(120,120,118,35,240,(RGB888ToRGB565(0x00BFFF)));
	       CH_gdi_draw_arc(120,120,117,35,240,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc(120,120,119,35,240,(RGB888ToRGB565(0x00BFFF)));

		   CH_gdi_draw_arc_again(120,120,118,0,120,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc_again(120,120,117,0,120,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc_again(120,120,116,0,120,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc_again(120,120,119,0,120,(RGB888ToRGB565(0x00BFFF)));

		   CH_gdi_draw_arc_high(120,120,116,0,120,(RGB888ToRGB565(0x00BFFF)));
	 	   CH_gdi_draw_arc_high(120,120,117,0,120,(RGB888ToRGB565(0x00BFFF)));
	 	   CH_gdi_draw_arc_high(120,120,118,0,120,(RGB888ToRGB565(0x00BFFF)));
		   CH_gdi_draw_arc_high(120,120,119,0,120,(RGB888ToRGB565(0x00BFFF)));
		   gdi_draw_image(33,200,IMAGE_INDEX_CIRCLE_DIAN);
		   gdi_draw_image(0,118,IMAGE_INDEX_CIRCLE_DIAN);
		   gdi_draw_image(35,35,IMAGE_INDEX_CIRCLE_DIAN);
		  #endif 
	       show_bat_cnt=0;
	    }
	   	
	   	
	}
  else
	{
		mark=0;
		
		if((adc_data<=VOLTAGE_3400_V)&&(shutdown == 0))
		{
		    shutdown = 1;
			PELTEN_DBG("---111sbit_shut_down");
	        sbit_shut_down();
		}
		else if (adc_data>VOLTAGE_3400_V&&adc_data<=VOLTAGE_3700_V)
		{
		   temp_info.batterr_data = 15;
		  #if 0 
		   CH_gdi_draw_arc(120,120,116,205,240,(RGB888ToRGB565(0x00BFFF))+c);
	       CH_gdi_draw_arc(120,120,118,205,240,(RGB888ToRGB565(0x00BFFF))+c);
	       CH_gdi_draw_arc(120,120,117,205,240,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc(120,120,119,205,240,(RGB888ToRGB565(0x00BFFF))+c);

		   CH_gdi_draw_arc_again(120,120,118,35,120,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc_again(120,120,117,35,120,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc_again(120,120,116,35,120,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc_again(120,120,119,35,120,(RGB888ToRGB565(0x00BFFF))+c);
	       gdi_draw_circle(37,203,1,0xFF99);
		   gdi_draw_image(35,200,IMAGE_INDEX_CIRCLE_DIAN);
		  #endif 

		}
		else if (adc_data>VOLTAGE_3700_V&&adc_data<=VOLTAGE_3800_V)
		{
		   temp_info.batterr_data = 33;
		  #if 0 
		   CH_gdi_draw_arc(120,120,116,120,240,(RGB888ToRGB565(0x00BFFF))+c);
	       CH_gdi_draw_arc(120,120,118,120,240,(RGB888ToRGB565(0x00BFFF))+c);
	       CH_gdi_draw_arc(120,120,117,120,240,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc(120,120,119,120,240,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc_again(120,120,118,0,120,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc_again(120,120,117,0,120,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc_again(120,120,116,0,120,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc_again(120,120,119,0,120,(RGB888ToRGB565(0x00BFFF))+c);
		   gdi_draw_image(33,200,IMAGE_INDEX_CIRCLE_DIAN);
		   gdi_draw_image(0,118,IMAGE_INDEX_CIRCLE_DIAN);
		  #endif 

		}
		else if (adc_data>VOLTAGE_3800_V&&adc_data<=VOLTAGE_4000_V)
		{
		   temp_info.batterr_data = 66;
		 #if 0  
		   CH_gdi_draw_arc(120,120,116,35,240,(RGB888ToRGB565(0x00BFFF))+c);
	       CH_gdi_draw_arc(120,120,118,35,240,(RGB888ToRGB565(0x00BFFF))+c);
	       CH_gdi_draw_arc(120,120,117,35,240,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc(120,120,119,35,240,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc_again(120,120,118,0,120,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc_again(120,120,117,0,120,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc_again(120,120,116,0,120,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc_again(120,120,119,0,120,(RGB888ToRGB565(0x00BFFF))+c);
		   gdi_draw_image(33,200,IMAGE_INDEX_CIRCLE_DIAN);
		   gdi_draw_image(0,118,IMAGE_INDEX_CIRCLE_DIAN);
		   gdi_draw_image(35,35,IMAGE_INDEX_CIRCLE_DIAN);
		  #endif 

		}
		else if(adc_data>VOLTAGE_4000_V)
		{
		   temp_info.batterr_data = 99;
		  #if 0 
		   CH_gdi_draw_arc(120,120,116,35,240,(RGB888ToRGB565(0x00BFFF))+c);
	       CH_gdi_draw_arc(120,120,118,35,240,(RGB888ToRGB565(0x00BFFF))+c);
	       CH_gdi_draw_arc(120,120,117,35,240,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc(120,120,119,35,240,(RGB888ToRGB565(0x00BFFF))+c);

		   CH_gdi_draw_arc_again(120,120,118,0,120,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc_again(120,120,117,0,120,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc_again(120,120,116,0,120,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc_again(120,120,119,0,120,(RGB888ToRGB565(0x00BFFF))+c);

		   CH_gdi_draw_arc_high(120,120,116,0,120,(RGB888ToRGB565(0x00BFFF))+c);
	 	   CH_gdi_draw_arc_high(120,120,117,0,120,(RGB888ToRGB565(0x00BFFF))+c);
	 	   CH_gdi_draw_arc_high(120,120,118,0,120,(RGB888ToRGB565(0x00BFFF))+c);
		   CH_gdi_draw_arc_high(120,120,119,0,120,(RGB888ToRGB565(0x00BFFF))+c);
		   gdi_draw_image(33,200,IMAGE_INDEX_CIRCLE_DIAN);
		   gdi_draw_image(0,118,IMAGE_INDEX_CIRCLE_DIAN);
		   gdi_draw_image(35,35,IMAGE_INDEX_CIRCLE_DIAN);
		 #endif  
		}
	}
}
uint32_t sbit_get_battery_adc(void)
{
	return mmi_adc_data;
}

int sbit_get_battery_level(void)
{
	int ret = 0;
	uint32_t adc_data = 0;
	
	adc_data = sbit_get_battery_adc();

	if(adc_data >= VOLTAGE_4200_V)
	{
		ret = 4;
	}
	else if(adc_data >= VOLTAGE_4100_V)
	{
		ret = 3;
	}
	else if(adc_data >= VOLTAGE_3900_V)
	{
		ret = 2;
	}
	else if(adc_data >= VOLTAGE_3700_V)
	{
		ret = 1;
	}
	else if(adc_data >= VOLTAGE_3500_V)
	{
		ret = 0;
	}
	return ret;
}
#elif defined(XT_WATCH_SUPPORT) || defined(MD_WATCH_SUPPORT)
void sbit_check_battery_data(void)
{
    uint32_t adc_data;
    uint32_t adc_voltage;
	static int show_bat_cnt=0;;
	int c=0;
	static int mark=0;
	static int shutdown=0;
	hal_rtc_time_t curtime; 
	hal_gpio_data_t input_gpio_data = HAL_GPIO_DATA_LOW;
	
	hal_rtc_get_time(&curtime);
	//c = (curtime.rtc_hour*60*60)+(curtime.rtc_min*60)+curtime.rtc_sec;

    hal_adc_init();
    hal_adc_get_data_polling(HAL_ADC_CHANNEL_6, &adc_data);
    hal_adc_deinit();
	
	hal_gpio_init(HAL_GPIO_30);
	hal_pinmux_set_function(HAL_GPIO_30, HAL_GPIO_30_GPIO30);			  /*   set dierection to be output	*/			  
	hal_gpio_set_direction(HAL_GPIO_30, HAL_GPIO_DIRECTION_INPUT);
	hal_gpio_get_input((hal_gpio_pin_t)HAL_GPIO_30, &input_gpio_data);

	if((curtime.rtc_sec==0)||(curtime.rtc_sec==30))
	{
		PELTEN_DBG("<<<<<adc_data>>>>>\r\n %d",adc_data);
		PELTEN_DBG("<<<<<input_gpio_data>>>>>\r\n %d",input_gpio_data);
	}
	
	if (temp_info.usb_connect_flag==1)
	{
		if (input_gpio_data == HAL_GPIO_DATA_HIGH)
		mark++;
		else if(mark<300)
		mark=0;

		if ((adc_data >= VOLTAGE_4350_V)&&(mark>300))
		{
			shutdown = 0;
			temp_info.batterr_data = 100;
			temp_info.charging_complete = 1;
			gdi_draw_image(180,10,IMAGE_XiTuo_battery10);
			return;
		}
		   temp_info.charging_complete = 0;
		   show_bat_cnt++;
	   if(show_bat_cnt==1)
	   	{
		   gdi_draw_image(180,10,IMAGE_XiTuo_battery2);
	   	}
	   else if(show_bat_cnt==2)
	   	{
		   gdi_draw_image(180,10,IMAGE_XiTuo_battery4);
	   	}
	   else if(show_bat_cnt==3)
	   	{
		   gdi_draw_image(180,10,IMAGE_XiTuo_battery6);
	    }
	   else if (show_bat_cnt==4)
	    {
		   gdi_draw_image(180,10,IMAGE_XiTuo_battery8);
	    }
	   else if (show_bat_cnt==5)
	    {
		   gdi_draw_image(180,10,IMAGE_XiTuo_battery10);
		   show_bat_cnt=0;
	    }
	}
  	else
	{
		mark=0;
		
		if(adc_data<=VOLTAGE_3300_V)
		{
		    shutdown ++;
			gdi_draw_image(180,10,IMAGE_XiTuo_battery1);
			if(shutdown == 60)
			{
				temp_info.shut_down_flag=1;
				temp_info.Low_battery_flag = 1;
				PELTEN_DBG("---222sbit_shut_down");
				sbit_shut_down();
			}
		}
		else if(adc_data>VOLTAGE_4200_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 100;
		   gdi_draw_image(180,10,IMAGE_XiTuo_battery10);
		}
		else if(adc_data>VOLTAGE_4100_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 90;
		   gdi_draw_image(180,10,IMAGE_XiTuo_battery9);
		}
		else if(adc_data>VOLTAGE_4000_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 80;
		   gdi_draw_image(180,10,IMAGE_XiTuo_battery8);
		}
		else if(adc_data>VOLTAGE_3900_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 70;
		   gdi_draw_image(180,10,IMAGE_XiTuo_battery7);
		}
		else if(adc_data>VOLTAGE_3800_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 60;
		   gdi_draw_image(180,10,IMAGE_XiTuo_battery6);
		}
		else if(adc_data>VOLTAGE_3700_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 50;
		   gdi_draw_image(180,10,IMAGE_XiTuo_battery5);
		}
		else if(adc_data>VOLTAGE_3600_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 40;
		   gdi_draw_image(180,10,IMAGE_XiTuo_battery4);
		}
		else if(adc_data>VOLTAGE_3500_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 30;
		   gdi_draw_image(180,10,IMAGE_XiTuo_battery3);
		}
		else if(adc_data>VOLTAGE_3400_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 20;
		   gdi_draw_image(180,10,IMAGE_XiTuo_battery2);
		}
		else 
		{
		   shutdown = 0;
		   temp_info.batterr_data = 10;
		   gdi_draw_image(180,10,IMAGE_XiTuo_battery1);
		}

	}
}

#elif defined(MTK_BLE_SUPPORT)

void sbit_check_battery_data(void)
{
	uint32_t adc_voltage;
	static int show_bat_cnt=0;;
	int c=0;
	static int mark=0;
	static int usb_input_low=0 , usb_input_high=0;
	static int shutdown=0;
	hal_rtc_time_t curtime; 
	hal_gpio_data_t input_gpio_data = HAL_GPIO_DATA_LOW;
	
	hal_rtc_get_time(&curtime);
	//c = (curtime.rtc_hour*60*60)+(curtime.rtc_min*60)+curtime.rtc_sec;

	hal_adc_init();
	hal_adc_get_data_polling(HAL_ADC_CHANNEL_6, &temp_info.adc_data);
	hal_adc_deinit();
	
	hal_gpio_init(HAL_GPIO_30);
	hal_pinmux_set_function(HAL_GPIO_30, HAL_GPIO_30_GPIO30);			  /*   set dierection to be output	*/			  
	hal_gpio_set_direction(HAL_GPIO_30, HAL_GPIO_DIRECTION_INPUT);
	hal_gpio_get_input((hal_gpio_pin_t)HAL_GPIO_30, &input_gpio_data);

	if((curtime.rtc_sec==0)||(curtime.rtc_sec==30))
	{
		PELTEN_DBG("<<<<<temp_info.adc_data>>>>>\r\n %d",temp_info.adc_data);
		PELTEN_DBG("<<<<<input_gpio_data>>>>>\r\n %d",input_gpio_data);
	}
	
	if((temp_info.charging_complete == 1)&&(temp_info.usb_connect_flag==1))
	{
		temp_info.batterr_data = 100;
		gdi_draw_image(187,15,IMAGE_XiTuo_battery10);
		return;
	}
	if ((temp_info.usb_connect_flag==1)&&(temp_info.nbiot_long_connect_flag==0))
	{
		//if (input_gpio_data == HAL_GPIO_DATA_HIGH)
		if (temp_info.adc_data >= VOLTAGE_4300_V)
		mark++;
		else if(mark<300)
		mark=0;

		if ((temp_info.adc_data >= VOLTAGE_4200_V)&&(mark>300))
		{
			shutdown = 0;
			temp_info.batterr_data = 100;
			temp_info.charging_complete = 1;
			gdi_draw_image(187,15,IMAGE_XiTuo_battery10);
			return;
		}
		   temp_info.charging_complete = 0;
		   show_bat_cnt++;
	   if(show_bat_cnt==1)
		{
		   gdi_draw_image(187,15,IMAGE_XiTuo_battery2);
		}
	   else if(show_bat_cnt==2)
		{
		   gdi_draw_image(187,15,IMAGE_XiTuo_battery4);
		}
	   else if(show_bat_cnt==3)
		{
		   gdi_draw_image(187,15,IMAGE_XiTuo_battery6);
		}
	   else if (show_bat_cnt==4)
		{
		   gdi_draw_image(187,15,IMAGE_XiTuo_battery8);
		}
	   else if (show_bat_cnt==5)
		{
		   gdi_draw_image(187,15,IMAGE_XiTuo_battery10);
		   show_bat_cnt=0;
		}
		
	}
  else
	{
		mark=0;
		usb_input_low = 0;
		usb_input_high = 0;
		temp_info.charging_complete = 0;  
		if(temp_info.adc_data<=VOLTAGE_3300_V)
		{
			shutdown ++;
			temp_info.batterr_data = 10;
			if(shutdown == 60)
			{
				temp_info.shut_down_flag=1;
				temp_info.Low_battery_flag = 1;
				PELTEN_DBG("---333sbit_shut_down");
				sbit_shut_down();
			}
		}
		else if(temp_info.adc_data>VOLTAGE_4200_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 100;
		   gdi_draw_image(187,15,IMAGE_XiTuo_battery10);
		}
		else if(temp_info.adc_data>VOLTAGE_4100_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 90;
		   gdi_draw_image(187,15,IMAGE_XiTuo_battery9);
		}
		else if(temp_info.adc_data>VOLTAGE_4000_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 80;
		   gdi_draw_image(187,15,IMAGE_XiTuo_battery8);
		}
		else if(temp_info.adc_data>VOLTAGE_3900_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 70;
		   gdi_draw_image(187,15,IMAGE_XiTuo_battery7);
		}
		else if(temp_info.adc_data>VOLTAGE_3800_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 60;
		   gdi_draw_image(187,15,IMAGE_XiTuo_battery6);
		}
		else if(temp_info.adc_data>VOLTAGE_3700_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 50;
		   gdi_draw_image(187,15,IMAGE_XiTuo_battery5);
		}
		else if(temp_info.adc_data>VOLTAGE_3600_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 40;
		   gdi_draw_image(187,15,IMAGE_XiTuo_battery4);
		}
		else if(temp_info.adc_data>VOLTAGE_3500_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 30;
		   gdi_draw_image(187,15,IMAGE_XiTuo_battery3);
		}
		else if(temp_info.adc_data>VOLTAGE_3400_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 20;
		   gdi_draw_image(187,15,IMAGE_XiTuo_battery2);
		}
		else 
		{
		   shutdown = 0;
		   temp_info.batterr_data = 10;
		   gdi_draw_image(187,15,IMAGE_XiTuo_battery1);
		}

	}
}

#elif defined(MTK_WIFI_SUPPORT)

void sbit_check_battery_data(void)
{
	uint32_t adc_data;
	uint32_t adc_voltage;
	static int show_bat_cnt=0;
	int c=0,mid_value=0;
	static int voltage_full_flag = 0;
	static int voltage_full_delay = 0;
	static int usb_input_low=0 , usb_input_high=0;
	static int shutdown=0;
	static int Low_battery_flag=0,Low_battery_flag01=0;
	hal_rtc_time_t curtime; 
	hal_gpio_data_t input_gpio_data = HAL_GPIO_DATA_LOW;
	
	hal_rtc_get_time(&curtime);

	hal_adc_init();
	hal_adc_get_data_polling(HAL_ADC_CHANNEL_6, &adc_data);
	hal_adc_deinit();
	
	hal_gpio_init(HAL_GPIO_30);
	hal_pinmux_set_function(HAL_GPIO_30, HAL_GPIO_30_GPIO30);			  /*   set dierection to be output	*/			  
	hal_gpio_set_direction(HAL_GPIO_30, HAL_GPIO_DIRECTION_INPUT);
	hal_gpio_get_input((hal_gpio_pin_t)HAL_GPIO_30, &input_gpio_data);

	//if((curtime.rtc_sec==0)||(curtime.rtc_sec==30))
	{
		PELTEN_DBG("<<<<<adc_data>>>>>\r\n %d",adc_data);
		PELTEN_DBG("<<<<<input_gpio_data>>>>>\r\n %d",input_gpio_data);
		PELTEN_DBG("<<<<<voltage_full_flag>>>>>\r\n %d",voltage_full_flag);
		
	}
	
 
	if (temp_info.usb_connect_flag==1)
 	{
 		//if((curtime.rtc_sec%30)==0)
 		{
 			voltage_full_delay ++;
 		}
 		PELTEN_DBG("<<<<<voltage_full_rtc_min>>>>> %d,%d\r\n",voltage_full_delay,voltage_full_flag);
 		Low_battery_flag=0;
 		Low_battery_flag01 = 0;
        temp_info.Low_battery_warning_flag = 0;
 		if ((input_gpio_data == 0)||(adc_data >= VOLTAGE_FULL)||(voltage_full_delay > 120))
 		{
 			shutdown = 0;
 			//if((curtime.rtc_sec%30)==0)
 			{
 				voltage_full_flag ++; 
 			}
 			if(voltage_full_flag > 2)
 			{
 				temp_info.batterr_data = 100;
 				temp_info.charging_complete = 1;
 				return;
 			}
 		}
 	    else
 	    {
 	        if(temp_info.batterr_data==0)
             {
 	            if(adc_data>VOLTAGE_4200_V)
 		        temp_info.batterr_data = 100;
 				else if(adc_data>VOLTAGE_4100_V)
 				temp_info.batterr_data = 90;
 				else if(adc_data>VOLTAGE_4000_V)
 				temp_info.batterr_data = 80;
 				else if(adc_data>VOLTAGE_3900_V)
 				temp_info.batterr_data = 70;
 				else if(adc_data>VOLTAGE_3800_V)
 				temp_info.batterr_data = 60;
 				else if(adc_data>VOLTAGE_3700_V)
 				temp_info.batterr_data = 50;
 				else if(adc_data>VOLTAGE_3600_V)
 				temp_info.batterr_data = 40;
 				else if(adc_data>VOLTAGE_3500_V)
 				temp_info.batterr_data = 30;
 				else if(adc_data>VOLTAGE_3400_V)
 				temp_info.batterr_data = 20;
 				else
 		        temp_info.batterr_data = 10;
 	        }
 	    }
 		  
 		
 	}
  else
	{
		voltage_full_flag = 0;
		voltage_full_delay = 0;
		temp_info.charging_complete = 0;  
		if(adc_data<=VOLTAGE_3300_V)
		{
			shutdown ++;
			if(shutdown == 60)
			{
				temp_info.shut_down_flag=1;
				temp_info.Low_battery_flag = 1;
				PELTEN_DBG("---444sbit_shut_down");
				sbit_shut_down();
			}
			temp_info.batterr_data = 5;
			
		}
		else if(adc_data>VOLTAGE_4200_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 100;
		   temp_info.Low_battery_warning_flag = 0;
		  
		}
		else if(adc_data>VOLTAGE_4100_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 90;
		   temp_info.Low_battery_warning_flag = 0;
		  
		}
		else if(adc_data>VOLTAGE_4000_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 80;
		   temp_info.Low_battery_warning_flag = 0;
		   
		}
		else if(adc_data>VOLTAGE_3900_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 70;
		   temp_info.Low_battery_warning_flag = 0;
		  
		}
		else if(adc_data>VOLTAGE_3800_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 60;
		   temp_info.Low_battery_warning_flag = 0;
		   
		}
		else if(adc_data>VOLTAGE_3700_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 50;
		   temp_info.Low_battery_warning_flag = 0;
		   
		}
		else if(adc_data>VOLTAGE_3600_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 40;
		   temp_info.Low_battery_warning_flag = 0;
		   
		}
		else if(adc_data>VOLTAGE_3500_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 30;
		   temp_info.Low_battery_warning_flag = 0;
		   
		}
		else if(adc_data>VOLTAGE_3400_V)
		{
		   shutdown = 0;
		   temp_info.batterr_data = 20;
		   if((Low_battery_flag01 == 0)&&(curtime.rtc_year+2000>=2018))
		   {
		       Low_battery_flag01 = 1;
			   temp_info.Low_battery_warning_flag = 1;
			   sbit_m2m_ct_send_massege_isr(M2M_DISPLAY_BACKLIGHT,0,0,1);
		   }
		   
		}
		else 
		{
		   shutdown = 0;
		   temp_info.batterr_data = 10;
		   if((Low_battery_flag == 0)&&(curtime.rtc_year+2000>=2018))
		   {
		       Low_battery_flag = 1;
			   temp_info.Low_battery_warning_flag = 1;
			   sbit_m2m_ct_send_massege_isr(M2M_DISPLAY_BACKLIGHT,0,0,1);
		   }
		   
		}
		

	}
}

#endif
#endif

