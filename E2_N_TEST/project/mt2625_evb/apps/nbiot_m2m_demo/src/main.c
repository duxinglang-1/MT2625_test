/* Copyright Statement:
 *
 * (C) 2005-2017  MediaTek Inc. All rights reserved.
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

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* device.h includes */
#include "mt2625.h"

/* hal includes */
#include "hal.h"
#include "hal_rtc_internal.h"
#include "memory_attribute.h"

#include "nvdm.h"

#include "sys_init.h"
#include "task_def.h"
#include "tel_conn_mgr_app_api.h"
#include "ril_task.h"
#ifdef MTK_FOTA_ENABLE
#include "fota.h"
#include "apb_proxy_fota_cmd.h"
#endif
#ifdef MTK_USB_DEMO_ENABLED
#include "usb.h"
#endif

#ifdef __RS_FOTA_SUPPORT__
#include "rs_sdk_api.h"
#endif

//#include "sensor_demo.h"
#ifdef SENSOR_DEMO
#include "sensor_alg_interface.h"
#endif

#ifdef MTK_GNSS_ENABLE
#include "gnss_app.h"
extern void gnss_demo_main();
#endif

/* for tracing and assert function prototypes */
#include "frhsl.h"
#include "system.h"

/* mux ap includes */
#include "mux_ap.h"
/* ril includes */
#include "ril.h"
/* AP Bridge Proxy inlcudes*/
#include "apb_proxy_task.h"
#include "auto_register.h"

#include "lwip/tcpip.h"
#ifdef MTK_TCPIP_FOR_NB_MODULE_ENABLE
#include "nbnetif.h"
#endif
#include "nidd_gprot.h"
#ifdef MTK_ATCI_APB_PROXY_NETWORK_ENABLE
#include "apb_proxy_nw_cmd_gprot.h"
#endif

#include "h10_mmi.h"
#include "hal_wdt.h"

#ifdef HAL_KEYPAD_MODULE_ENABLED
#include "keypad_custom.h"
#ifdef HAL_SLEEP_MANAGER_ENABLED
#include "hal_sleep_manager.h"
#include "hal_sleep_driver.h"
#include "hal_gpt.h"
#define POWER_KEY_LOCK_SLEEP_IN_SEC 15
const static char *powerkey_lock_sleep_name = "Powerkey_main";
static uint8_t powerkey_lock_sleep_handle;
static bool power_key_timer = false;
uint32_t timer_handle;
#endif
#endif

#if( configAPPLICATION_ALLOCATED_HEAP == 1 )
ATTR_NONINIT_DATA_IN_RAM uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
#endif

extern void md_init_phase_2(void);
extern void simat_proxy_init();

#ifdef MTK_CTIOT_SUPPORT
extern void lwm2m_restore_result_callback(bool result, void *user_data);
#endif

#ifdef MTK_COAP_SUPPORT
extern void nw_coap_init(void);
#endif

struct assert_user_var_t global_assert_user_var_t = {0};

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
//#define FREERTOS_TEST

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

#if !defined (MTK_DEBUG_LEVEL_NONE)
LOG_CONTROL_BLOCK_DECLARE(atci_serialport);
LOG_CONTROL_BLOCK_DECLARE(atcmd);
LOG_CONTROL_BLOCK_DECLARE(common);
LOG_CONTROL_BLOCK_DECLARE(hal);
LOG_CONTROL_BLOCK_DECLARE(httpclient);
LOG_CONTROL_BLOCK_DECLARE(iperf);
LOG_CONTROL_BLOCK_DECLARE(ping);
LOG_CONTROL_BLOCK_DECLARE(RTC_ATCI);
LOG_CONTROL_BLOCK_DECLARE(fota_http_dl);
LOG_CONTROL_BLOCK_DECLARE(fota);
LOG_CONTROL_BLOCK_DECLARE(apb_proxy);
LOG_CONTROL_BLOCK_DECLARE(mux_ap);
LOG_CONTROL_BLOCK_DECLARE(auto_reg);


log_control_block_t *syslog_control_blocks[] = {
    &LOG_CONTROL_BLOCK_SYMBOL(atci_serialport),
    &LOG_CONTROL_BLOCK_SYMBOL(atcmd),
    &LOG_CONTROL_BLOCK_SYMBOL(common),
    &LOG_CONTROL_BLOCK_SYMBOL(hal),
    &LOG_CONTROL_BLOCK_SYMBOL(httpclient),
    &LOG_CONTROL_BLOCK_SYMBOL(iperf),
    &LOG_CONTROL_BLOCK_SYMBOL(ping),
    &LOG_CONTROL_BLOCK_SYMBOL(RTC_ATCI),
    &LOG_CONTROL_BLOCK_SYMBOL(fota_http_dl),
    &LOG_CONTROL_BLOCK_SYMBOL(fota),
    &LOG_CONTROL_BLOCK_SYMBOL(apb_proxy),
    &LOG_CONTROL_BLOCK_SYMBOL(mux_ap),
    &LOG_CONTROL_BLOCK_SYMBOL(auto_reg),
    NULL
};

static void syslog_config_save(const syslog_config_t *config)
{
    nvdm_status_t status;
    char *syslog_filter_buf;

    syslog_filter_buf = (char*)pvPortMalloc(SYSLOG_FILTER_LEN);
    configASSERT(syslog_filter_buf != NULL);
    syslog_convert_filter_val2str((const log_control_block_t **)config->filters, syslog_filter_buf);
    status = nvdm_write_data_item("common",
                                  "syslog_filters",
                                  NVDM_DATA_ITEM_TYPE_STRING,
                                  (const uint8_t *)syslog_filter_buf,
                                  strlen(syslog_filter_buf));
    vPortFree(syslog_filter_buf);
    LOG_I(common, "syslog config save, status=%d", status);
}

static uint32_t syslog_config_load(syslog_config_t *config)
{
    uint32_t sz = SYSLOG_FILTER_LEN;
    char *syslog_filter_buf;

    syslog_filter_buf = (char*)pvPortMalloc(SYSLOG_FILTER_LEN);
    configASSERT(syslog_filter_buf != NULL);
    if (nvdm_read_data_item("common", "syslog_filters", (uint8_t*)syslog_filter_buf, &sz) == NVDM_STATUS_OK) {
        syslog_convert_filter_str2val(config->filters, syslog_filter_buf);
    } else {
        /* popuplate the syslog nvdm with the image setting */
        syslog_config_save(config);
    }
    vPortFree(syslog_filter_buf);
    return 0;
}
#endif

void big_assert(void);

extern void       KiOsReset               (void);
void AssertHandlerNoReturn(unsigned flags,...)
{
	/* This is desperate trick to possible store registers before they are cleared/overrun/used */
	asm volatile ("nop" ::: "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12" );

    char *module_name = NULL;
    int line_number = 0;
    //char *function_name = NULL;
    //char *user_msg = NULL;
    uint16_t conditions = 0;
    va_list arg_list;
    //char trace_buffer[80];

    __disable_irq();

    // go to beginning of variable argument list
    va_start(arg_list, flags);

    // read argumets based on flag bits
    conditions = (uint16_t) flags;
    if (flags & AF_FLAGS_FILE_)
    {
      module_name = va_arg(arg_list, char *);
      line_number = va_arg(arg_list, int);
    }
    if (flags & AF_FLAGS_FUNC_)
    {
      //function_name = va_arg(arg_list, char *);
    }
    if (flags & AF_FLAGS_COND_)
    {
      conditions |= (1u << 8);
    }
    if (flags & AF_FLAGS_MSG_)
    {
      //user_msg = va_arg(arg_list, char *);
    }
    if (flags & AF_FLAGS_VARS_)
    {
      uint32_t loopc;
      global_assert_user_var_t.user_var_present = true;

      for(loopc = 0; loopc < 3; loopc++)
      {
        global_assert_user_var_t.user_var[loopc] = va_arg(arg_list, unsigned int);
      }
    }
    if (0 == (flags & AF_FLAGS_FATAL_))
    {
      conditions |= (1u << 9);
    }

/*
	// Trace error info to HLS
    FrHslString (0x000A, "\r\n-- AssertHandlerNoReturn called --");
    sprintf(trace_buffer, "\r\n   with Flags = %#04X",conditions);
    FrHslString (0x000A, trace_buffer);

//    M_FrHslPrintf1 (0x9515, DEFAULT_GROUP, "Assert: AssertHandlerNoReturn called. Flags=%{1}8.0b, condition checked=%{1}1.8b, error=%{1}1.9b, raw=%{1}#04X",
//      conditions)
    if (NULL != module_name)
    {
        sprintf(trace_buffer, "\r\n  Module name: %s",module_name);
        FrHslString (0x000A, trace_buffer);
        sprintf(trace_buffer, "\r\n  Line number: %lu",line_number);
        FrHslString (0x000A, trace_buffer);
//        M_FrHslString (0xCAF8, DEFAULT_GROUP, "  Module name: %s", module_name);
//        M_FrHslPrintf2 (0xBB0F, DEFAULT_GROUP, "  Line number: %lu", (uint16_t) (line_number >> 16), (uint16_t) line_number);
    }
    if (NULL != function_name)
    {
        sprintf(trace_buffer, "\r\n  Function name: %s", function_name);
        FrHslString (0x000A, trace_buffer);
//        M_FrHslString (0xCF58, DEFAULT_GROUP, "  Function name: %s", function_name);
    }

//  !!! Need to figure out still how to put this data out, will help debugging when we see this data in HSL traces as well !!!

    if (NULL != user_msg)
    {
        sprintf(trace_buffer, "\r\n  User message: %s", user_msg);
        FrHslString (0x000A, trace_buffer);
//        M_FrHslString (0xC724, DEFAULT_GROUP, "  User message: %s", user_msg);
    }

//        sprintf(trace_buffer, "\r\n  User variables: [0]=%#08lX, [1]=%#08lX, [2]=%#08lX", user_var[0], user_var[1], user_var[2]);
//        FrHslString (0x000A, trace_buffer);
//        M_FrHslPrintf6 (0xA3F8, DEFAULT_GROUP, "  User variables: [0]=%#08lX, [1]=%#08lX, [2]=%#08lX",
//        (uint16_t) (user_var[0] >> 16), (uint16_t) user_var[0],
//        (uint16_t) (user_var[1] >> 16), (uint16_t) user_var[1],
//        (uint16_t) (user_var[2] >> 16), (uint16_t) user_var[2]);
    }
*/

    // call platform assert to create full path towards memory dump
    platform_assert(0, module_name, line_number );

    // this function prototype is defined with noreturn pragma, if function returns to
    // caller system will crash in very mysterious ways.
    for(;;);
}

#define mainCHECK_DELAY ( ( portTickType ) 1000 / portTICK_RATE_MS )


/**
* @brief       Task main function
* @param[in]   pvParameters: Pointer that will be used as the parameter for the task being created.
* @return      None.
*/
/*
static void vTestTask(void *pvParameters)
{
    uint32_t idx = (int)pvParameters;

    portTickType xLastExecutionTime, xDelayTime;
    xLastExecutionTime = xTaskGetTickCount();
    xDelayTime = (1 << idx) * mainCHECK_DELAY;

    while (1) {
        vTaskDelayUntil(&xLastExecutionTime, xDelayTime);
        printf("Hello World from %u at %u \r\n", idx, xTaskGetTickCount());
    }
}
*/

#ifdef FREERTOS_TEST
#include "FreeRTOS_test.h"
void vApplicationTickHook(void)
{
    vFreeRTOS_TestISR();
}
#endif /* FREERTOS_TEST */

#ifdef MTK_SYSTEM_HANG_CHECK_ENABLE
void system_wdt_occur(hal_wdt_reset_status_t mode)
{
   if(HAL_WDT_TIMEOUT_RESET == mode)
      DevFail("Watchdog timeout");
   else
      DevFail("Invalid SW watchdog");
}

/* start watchdog, timeout is 30s. Feed it at vApplicationIdleHook
   when watchdog timeout occurs, WDT_ISR occurs, then invoke assert function */
void system_start_wdt(void)
{
    hal_wdt_config_t wdt_config;
    wdt_config.mode = SYSTEM_HANG_CHECK_WDT_MODE;
    wdt_config.seconds = SYSTEM_HANG_CHECK_TIMEOUT_DURATION;
    
    hal_wdt_disable(HAL_WDT_DISABLE_MAGIC);
    hal_wdt_init(&wdt_config);
    hal_wdt_register_callback(system_wdt_occur);
    hal_wdt_enable(HAL_WDT_ENABLE_MAGIC);
}
#endif

#ifdef HAL_KEYPAD_FEATURE_POWERKEY
#ifdef HAL_SLEEP_MANAGER_ENABLED
void powerkey_timer_callback(void *user_data)
{
    /*unlock sleep*/
    hal_sleep_manager_release_sleeplock(powerkey_lock_sleep_handle, HAL_SLEEP_LOCK_ALL);
    /*Stop gpt timer*/
    hal_gpt_sw_stop_timer_ms(timer_handle);
    /*free gpt timer*/
    hal_gpt_sw_free_timer(timer_handle);
    power_key_timer = false;
    log_hal_info("[pwk_main]timer callback done\r\n");

}
#endif

hal_gpt_status_t H10_sleep_handler(void)
{
	hal_gpt_status_t ret_gpt = HAL_GPT_STATUS_INVALID_PARAMETER;
	/*get timer*/

	if (power_key_timer == false) 
	{
		ret_gpt = hal_gpt_sw_get_timer(&timer_handle);
		if (ret_gpt != HAL_GPT_STATUS_OK) {
			log_hal_info("[pwk_main]get timer handle error, ret = %d, handle = 0x%x\r\n",
						 (unsigned int)ret_gpt,
						 (unsigned int)timer_handle);
			return ret_gpt;
		}
		
		/*lock sleep*/
		hal_sleep_manager_acquire_sleeplock(powerkey_lock_sleep_handle, HAL_SLEEP_LOCK_ALL);
		log_hal_info("[pwk_main]start timer\r\n");
		/*start timer*/
		hal_gpt_sw_start_timer_ms(timer_handle,
								  POWER_KEY_LOCK_SLEEP_IN_SEC * 1000, //10 sec
								  (hal_gpt_callback_t)powerkey_timer_callback, NULL);
		power_key_timer = true;

	}
	
	return ret_gpt;
	
}

extern int medicine_remind_delay;
extern int medicine_lock_sec;
static void keypad_user_powerkey_handler(void)
{
    hal_keypad_status_t ret;
    hal_keypad_powerkey_event_t powekey_event;
    char *string[5] = {"release", "press", "longpress", "repeat", "pmu_longpress"};
	
	if(exception_auto_reboot_mark==0)
	hal_gpio_set_output(HAL_GPIO_32, HAL_GPIO_DATA_HIGH);
	
	exception_auto_reboot_mark = 1;

    while (1) 
	{
        ret = hal_keypad_powerkey_get_key(&powekey_event);

        /*If an error occurs, there is no key in the buffer*/
        if (ret == HAL_KEYPAD_STATUS_ERROR) {
            //log_hal_info("[pwk_main]powerkey no key in buffer\r\n\r\n");
            break;
        }

#ifdef HAL_SLEEP_MANAGER_ENABLED
        if (powekey_event.state == HAL_KEYPAD_KEY_PRESS || 
			powekey_event.state == HAL_KEYPAD_KEY_RELEASE ||
			powekey_event.state == HAL_KEYPAD_KEY_LONG_PRESS) 
		{
            if (power_key_timer == false) 
			{
                if ((H10_sleep_handler()) != HAL_GPT_STATUS_OK) 
				{
                     return;
                }
            }
			log_hal_info("KEY:%d",powekey_event.state);
			if(powekey_event.state == HAL_KEYPAD_KEY_LONG_PRESS)
			{
				if(get_sbit_show_meun_id() == POWEROFF_SCREEN)
				{
					temp_info.shut_down_flag=1;
	            	sbit_m2m_ct_send_massege_isr(M2M_SHUTDOWN,NULL,0,1);
				}
				else
				{
					if((temp_info.animation_flag>0)&&
					(temp_info.show_idle_flag == IDLE_SCREEN))
					{
						temp_info.set_mode_flag = 1;
			            sbit_m2m_ct_send_massege_isr(M2M_MENU_SET,NULL,0,1);
						log_hal_info("[[[[[ M2M_POWER_KEY ]]]]]\r\n");
					}
				}
			}
			else if(powekey_event.state == HAL_KEYPAD_KEY_RELEASE)
			{
		        if((temp_info.animation_flag == 1)&&(temp_info.sbit_backlight_flag == true))
				{
					if((temp_info.show_idle_flag<POWEROFF_SCREEN)&&
						(temp_info.set_mode_flag == 0)&&
						(temp_info.Sedentary_idle_flag == 0)&&
						(temp_info.weather_screen_flag == 0)&&
						(temp_info.activity_idle_flag == 0))
					 {
						 temp_info.show_idle_flag ++;
						 log_hal_info("+++111show idle_flag %d [LINE:%d]",temp_info.show_idle_flag,__LINE__);
						 //dbg_print("key up process");
					 }
					 else
					 {
					 	 log_hal_info("+++222shut_down_flag %d",temp_info.show_idle_flag);
					 	 if(temp_info.shut_down_flag == 0)
						 temp_info.show_idle_flag = IDLE_SCREEN;
						 else
						 temp_info.show_idle_flag = POWEROFF_ANIMATION_SCREEN;						 
						 log_hal_info("+++222show idle_flag %d [LINE:%d]",temp_info.show_idle_flag,__LINE__);
					 }
					 
					 blood_step = 0;
					if(medicine_remind_delay == 1)
					{
						temp_info.show_idle_flag = 0;
						log_hal_info("+++333show idle_flag %d [LINE:%d]",temp_info.show_idle_flag,__LINE__);
						sbit_set_vib_time(0,1);
					}
					 medicine_remind_delay = 0;
					 if(temp_info.set_mode_flag == 2)
					 temp_info.set_mode_flag = 0;
					 temp_info.Sedentary_idle_flag = 0;
					 temp_info.activity_idle_flag = 0;
					 temp_info.weather_screen_flag = 0;
					 temp_info.Low_battery_warning_flag = 0;
				}
				
				temp_info.stk8321_raise_mark=0;
				temp_info.backlight_counter = 0;				
		        sbit_m2m_ct_send_massege_isr(M2M_DISPLAY_BACKLIGHT,NULL,0,1);
			}
			else if(powekey_event.state == HAL_KEYPAD_KEY_PRESS)
			{
				if(temp_info.set_mode_flag == 1)
				temp_info.set_mode_flag = 2;
			}
			
        }
		else if(powekey_event.state == HAL_KEYPAD_KEY_REPEAT)
        {
            
        }
#endif
        log_hal_info("[pwk_main]powerkey data:[%d], state:[%s]\r\n", (int)powekey_event.key_data, (char *)string[powekey_event.state]);
    }
}

static void hal_powerkey_example(void)
{
    bool ret_bool;
    hal_keypad_status_t ret_state;

#ifdef HAL_SLEEP_MANAGER_ENABLED
    /*get sleep handle*/
    powerkey_lock_sleep_handle = hal_sleep_manager_set_sleep_handle(powerkey_lock_sleep_name);
    if (powerkey_lock_sleep_handle == INVALID_SLEEP_HANDLE) {
        log_hal_error("[pwk_main]:get sleep handle failed\r\n");
    }
#endif

    /*Initialize powerkey*/
    ret_bool = keypad_custom_powerkey_init();
    if (ret_bool == false) {
        log_hal_error("[pwk_main]keypad_custom_init init failed\r\n");
        return;
    }

    ret_state = hal_keypad_powerkey_register_callback((hal_keypad_callback_t)keypad_user_powerkey_handler, NULL);
    if (ret_state != HAL_KEYPAD_STATUS_OK) {
        log_hal_error("[pwk_main]hal_keypad_powerkey_register_callback failed, state = %d\r\n", ret_state);
    }
}
#endif
#include "serial_port.h"

void sbit_set_port_default(void)
{
    serial_port_dev_t saved_port;
    if( SERIAL_PORT_STATUS_OK != serial_port_config_read_dev_number("connl", &saved_port))
    {
        serial_port_config_write_dev_number("connl", SERIAL_PORT_DEV_USB_COM2);// connl
    }

    if( SERIAL_PORT_STATUS_OK != serial_port_config_read_dev_number("emmi", &saved_port))
    {
        serial_port_config_write_dev_number("emmi", SERIAL_PORT_DEV_USB_COM1);// uls
    }
	
    if( SERIAL_PORT_STATUS_OK != serial_port_config_read_dev_number("uls", &saved_port))
    {
        serial_port_config_write_dev_number("uls", SERIAL_PORT_DEV_UART_0);// uls
    }
}


int main(void)
{
    //int idx;

    /* Do system initialization, eg: hardware, clock. */
    system_init();

    /* MD init done here */
    sbit_set_port_default();
#ifdef MTK_USB_DEMO_ENABLED
	usb_boot_init();
#endif

    md_init_phase_2();

    log_init(syslog_config_save, syslog_config_load, syslog_control_blocks);

    tcpip_init(NULL, NULL);

#ifdef MTK_TCPIP_FOR_NB_MODULE_ENABLE
    nb_netif_init();
#endif

    mux_ap_init();
    ril_init();
    /*start up AP Bridge Proxy task*/
    apb_proxy_init();
#ifdef MTK_COAP_SUPPORT
    nw_coap_init();
#endif
    printf("start_conn\r\n");
    tel_conn_mgr_init();
    //tel_conn_mgr_ut_init();
    printf("end_conn\r\n");
#ifdef MTK_FOTA_ENABLE
    fota_init();
    fota_register_event(apb_proxy_fota_event_ind);
    #ifdef MTK_CTIOT_SUPPORT
    if (fota_is_executed() == true) {
        ctiot_at_restore(lwm2m_restore_result_callback, NULL);
    }
    #endif
#endif
    printf("nidd init start\r\n");
    nidd_init();
    printf("nidd init finished\r\n");
#ifdef MTK_ATCI_APB_PROXY_NETWORK_ENABLE
    socket_atcmd_init_task();
    apb_upsm_init_task();
    lwm2m_atcmd_init_task();
    onenet_at_init();
#ifdef MTK_ONENET_SUPPORT
    dm_at_init();
#endif
#ifdef MTK_TMO_CERT_SUPPORT
    tmo_at_init();
#endif
#ifdef MTK_CTM2M_SUPPORT
    ctm2m_at_task_init();
#endif
#endif

    printf("start simat_proxy_tast_init\r\n");
    simat_proxy_init();
    //test_wrong_simat_command_from_AP();
    printf("end simat_proxy_tast_init\r\n");

    /* Start the scheduler. */
    SysInitStatus_Set();
#ifdef HAL_TIME_CHECK_ENABLED
    hal_time_check_enable();
#endif
#ifdef HAL_RTC_FEATURE_SW_TIMER
    rtc_sw_timer_isr();
#endif
    auto_register_init();

#ifdef MTK_GNSS_ENABLE
    gnss_demo_main();
#endif
  //  auto_register_init();

    /* 2625 lwm2m app start */
    nb_app_enter();
#ifdef MTK_LWM2M_CT_SUPPORT
    ctiot_lwm2m_client_init();
#endif
#ifdef MTK_CTIOT_SUPPORT
    ctiot_at_init();
#endif

#ifdef SENSOR_DEMO
    printf("sensor_manager_init\r\n");
    sensor_manager_init();
#endif

#ifdef HAL_KEYPAD_FEATURE_POWERKEY
    hal_powerkey_example();
#endif

#ifdef __RS_FOTA_SUPPORT__
    printf("start  rs_sdk_start\r\n");
    rs_sdk_start(); 
#endif

	sbit_demo_display_init();

	m2m_timer_pro_task_create();


/* wdt is enabled in bootloader, if infinite loop occurs before this line,
   wdt reset will occurs. re-config wdt here for system hang check */
#ifdef MTK_SYSTEM_HANG_CHECK_ENABLE
    //enable wdt
#ifdef MTK_H10_DEBUG 
	hal_wdt_disable(HAL_WDT_DISABLE_MAGIC);
#else
	system_start_wdt();
#endif
#else
    hal_wdt_disable(HAL_WDT_DISABLE_MAGIC);
#endif
    vTaskStartScheduler();
    /* If all is well, the scheduler will now be running, and the following line
    will never be reached.  If the following line does execute, then there was
    insufficient FreeRTOS heap memory available for the idle and/or timer tasks
    to be created.  See the memory management section on the FreeRTOS web site
    for more details. */
    for (;;);
}

/* big_assert can be removed when every trace is coming in exception*/

void big_assert(void)
{
	for(uint8_t loopa = 0; loopa < 10; loopa++)
  {
  FrHslString (0x000A, "Assert! Assert! Assert! Assert! Assert! Assert! Assert! Assert!");
  }
FrHslString (0x000A, "Final line!");
}
