#include <stdio.h>
#include <stdbool.h>
#include "hrs3300.h"
#include "syslog.h"
#include "hrs3300_reg_init.h"
#include "hal_gpio.h"
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
#include "h10_mmi.h"
#include <stdlib.h>
#ifdef MTK_SWLA_ENABLE
#include "swla.h"
#endif

#include "semphr.h"
#include "hal_i2c_master.h"
#include "hal_eint.h"
#include "hal_gpt.h"
#include "hal_platform.h"


// hrs3300 customer config
const uint8_t  hrs3300_bp_timeout_grade = 0;  // max 15
const uint8_t  hrs3300_agc_init_stage = 0x04;  // init AGC state  
const uint8_t  hrs3300_bp_power_grade = 0;
const uint8_t  hrs3300_accurate_first_shot = 0;
const uint8_t  hrs3300_up_factor = 7;//3;
const uint8_t  hrs3300_up_shift = 3;//2;
const uint16_t hrs3300_AMP_LTH = 120;
const uint16_t hrs3300_hr_AMP_LTH = 150;
const uint16_t hrs3300_hr_PVAR_LTH = 10;
// hrs3300 customer config end

//20161117 added by ericy for "low power in no_touch state"
bool hrs3300_power_up_flg = 0 ;
uint8_t reg_0x7f ;
uint8_t reg_0x80 ;
uint8_t reg_0x81 ;
uint8_t reg_0x82 ;

//20161117 added by ericy for "low power in no_touch state"

typedef char kal_char;
typedef unsigned short kal_wchar;
typedef unsigned char kal_uint8;
typedef signed char kal_int8;
typedef unsigned short int kal_uint16;
typedef signed short int kal_int16;
typedef unsigned int kal_uint32;
typedef signed int kal_int32;
#define _HR3300_
#define BP_CUSTDOWN_ALG_LIB

#ifdef _HR3300_

#if defined(MD_WATCH_SUPPORT)
#define HR3300_SDA_PIN  HAL_GPIO_7 
#define HR3300_SCL_PIN  HAL_GPIO_6
#define HR3300_EN_PIN  HAL_GPIO_14

#elif defined(MTK_BLE_SUPPORT)
#define HR3300_SDA_PIN	HAL_GPIO_7 
#define HR3300_SCL_PIN	HAL_GPIO_6
#define HR3300_EN_PIN  HAL_GPIO_28
	
#elif defined(MTK_WIFI_SUPPORT)
#define HR3300_SDA_PIN  HAL_GPIO_17 
#define HR3300_SCL_PIN  HAL_GPIO_16
#define HR3300_EN_PIN  HAL_GPIO_28
#else
#define HR3300_SDA_PIN  HAL_GPIO_17   
#define HR3300_SCL_PIN  HAL_GPIO_16
#define HR3300_EN_PIN  HAL_GPIO_13
#endif

#if (defined(MD_WATCH_SUPPORT)||defined(MTK_BLE_SUPPORT))
#define HR3300_SDA_MODE 4
#define HR3300_SCL_MODE 4
#else
#define HR3300_SDA_MODE 3
#define HR3300_SCL_MODE 3
#endif

#define HR3300_EINT_PIN  HAL_GPIO_15
#define HR3300_EINT_MODE 7
#define HR3300_EINT_NUM  HAL_EINT_NUMBER_15

#define HR3300_REG_CHIPID          0x00
#define HR3300_SLAVE_ADDR  0x44

#if (defined(MD_WATCH_SUPPORT)||defined(MTK_BLE_SUPPORT))
#define HR3300_I2C_PORT    HAL_I2C_MASTER_0
#else
#define HR3300_I2C_PORT    HAL_I2C_MASTER_1
#endif

#define HR3300_USA_DMA    1

#define Hrs3300_DBG(fmt,arg...)   LOG_I(sbit_demo, "[Hrs3300]: "fmt,##arg)
extern void hal_HR3300_driver_i2c_init(void);
void heart_rate_meas_timeout_handler(void);
TimerHandle_t Hrs3300_timer = NULL;

TickType_t       HR3300_i2c_timeout_tick;
xSemaphoreHandle HR3300_i2c_dma_semaphore;
hal_i2c_config_t HR3300_i2c_config;
xQueueHandle     HR3300_queue_handle;

ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t  HR3300_dam_rx[256];
ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t  HR3300_dam_tx[10];

void hal_HR3300_driver_dma_callback(uint8_t slave_address, hal_i2c_callback_event_t event, void *user_data)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;;

    switch(event)
    {
        case HAL_I2C_EVENT_ACK_ERROR:
            //Hrs3300_DBG("HAL_I2C_EVENT_ACK_ERROR\r\n");
            break;
        case HAL_I2C_EVENT_NACK_ERROR:
            //Hrs3300_DBG("HAL_I2C_EVENT_NACK_ERROR\r\n");
            break;
        case HAL_I2C_EVENT_TIMEOUT_ERROR:
            //Hrs3300_DBG("HAL_I2C_EVENT_TIMEOUT_ERROR\r\n");
            break;
        case HAL_I2C_EVENT_SUCCESS:
            //Hrs3300_DBG("HAL_I2C_EVENT_SUCCESS\r\n");
            break;
        default:
            break;
    }

    hal_i2c_master_deinit(HR3300_I2C_PORT);
    xSemaphoreGiveFromISR(HR3300_i2c_dma_semaphore, &xHigherPriorityTaskWoken);
    
    if ( xHigherPriorityTaskWoken ) {
        // Actual macro used here is port specific.
        portYIELD_FROM_ISR (xHigherPriorityTaskWoken);
    }
    
}


int hal_HR3300_driver_send_data(uint8_t *data,uint32_t len)
{
    hal_i2c_status_t statu;
    hal_i2c_send_config_t config;
    
    if(len == 0 || data== NULL){
        Hrs3300_DBG("input error!\r\n");
        return -1;
    }

    statu = hal_i2c_master_init(HR3300_I2C_PORT,&HR3300_i2c_config);
    if(statu != HAL_I2C_STATUS_OK){
        Hrs3300_DBG("hal_i2c_master_init error:%d\r\n",statu);
        return statu;
    }
    
#if HR3300_USA_DMA //DMA
    memset(HR3300_dam_tx,0,sizeof(HR3300_dam_tx));
    memcpy(HR3300_dam_tx,data,len);

    config.send_data                = HR3300_dam_tx;
    config.slave_address            = HR3300_SLAVE_ADDR;
    config.send_packet_length       = 1;
    config.send_bytes_in_one_packet = len;
    
    statu = hal_i2c_master_register_callback(HR3300_I2C_PORT, hal_HR3300_driver_dma_callback, NULL);
    
    if (statu < 0) {
        Hrs3300_DBG("register callback failed. (%ld)\r\n", statu);
    } else {
        statu = hal_i2c_master_send_dma_ex(HR3300_I2C_PORT,&config);
        if(statu != HAL_I2C_STATUS_OK){
            Hrs3300_DBG("hal_i2c_master_send_dma error:%d\r\n",statu);
            return statu;
        }else{
            if (pdFALSE == xSemaphoreTake(HR3300_i2c_dma_semaphore, HR3300_i2c_timeout_tick)) {
                    Hrs3300_DBG("xSemaphoreTake timeout.\r\n");
                    statu = HAL_I2C_STATUS_ERROR;
                    return statu;
            }
        }
    }

    if (statu < 0) {
        hal_i2c_master_deinit(HR3300_I2C_PORT);
        return statu;
    }
#else
     statu = hal_i2c_master_send_polling(HR3300_I2C_PORT,HR3300_SLAVE_ADDR,data,len);

     if (statu != HAL_I2C_STATUS_OK) {
        Hrs3300_DBG("hal_i2c_master_send_polling error:%d\r\n",statu);
     }
     hal_i2c_master_deinit(HR3300_I2C_PORT);

#endif
    return 0;
}

int hal_HR3300_driver_send_recieve_data(uint8_t addr,uint8_t *data,uint32_t len)
{
    uint8_t mems_i2c_send_reg;
    hal_i2c_status_t statu = HAL_I2C_STATUS_OK;
    hal_i2c_send_to_receive_config_ex_t config;

    if((len == 0) || (data == NULL)){
        Hrs3300_DBG("input error!\r\n");
        return -1;
    }
    
    statu = hal_i2c_master_init(HR3300_I2C_PORT,&HR3300_i2c_config);
    if(statu != HAL_I2C_STATUS_OK){
        Hrs3300_DBG("hal_i2c_master_init error:%d\r\n",statu);
        return statu;
    }
    
#if HR3300_USA_DMA //DMA
    memset(HR3300_dam_rx,0,sizeof(HR3300_dam_rx));
    memset(HR3300_dam_tx,0,sizeof(HR3300_dam_tx));
    
    HR3300_dam_tx[0] = addr;
    config.slave_address = HR3300_SLAVE_ADDR;
    config.send_data = HR3300_dam_tx;
    config.receive_buffer = HR3300_dam_rx;
    config.send_packet_length = 1;
    config.send_bytes_in_one_packet = 1;
    config.receive_packet_length = 1;
    config.receive_bytes_in_one_packet = len;

    statu = hal_i2c_master_register_callback(HR3300_I2C_PORT, hal_HR3300_driver_dma_callback, NULL);
    if (statu < 0) {
        Hrs3300_DBG("register callback failed. (%ld)\r\n", statu);
    } else {

        statu = hal_i2c_master_send_to_receive_dma_ex(HR3300_I2C_PORT, &config);

        if (statu < 0) {
            Hrs3300_DBG("write read dma failed (%ld).\r\n", statu);
        } else {
            if (pdFALSE == xSemaphoreTake(HR3300_i2c_dma_semaphore, HR3300_i2c_timeout_tick)) {
                Hrs3300_DBG("xSemaphoreTake timeout.\r\n");
                statu = HAL_I2C_STATUS_ERROR;
            }
            
            memcpy(data,HR3300_dam_rx,len);
        }
    }

    if (statu < 0) {
        hal_i2c_master_deinit(HR3300_I2C_PORT);
        return statu;
    }
#else
    statu = hal_i2c_master_send_to_receive_polling(HR3300_I2C_PORT,&config);
    if (statu != HAL_I2C_STATUS_OK) {
        Hrs3300_DBG("write read polling failed (%ld).\r\n", statu);
    }
    hal_i2c_master_deinit(HR3300_I2C_PORT);

#endif
    return statu;
}

void hal_HR3300_eint_callback(void *arg)
{

}

void hal_HR3300_driver_i2c_init(void)
{
    hal_eint_config_t config;
    
    hal_gpio_init(HR3300_SDA_PIN);
    hal_gpio_init(HR3300_SCL_PIN);
    hal_gpio_pull_up(HR3300_SDA_PIN);
    hal_gpio_pull_up(HR3300_SCL_PIN);
    hal_pinmux_set_function(HR3300_SDA_PIN,HR3300_SDA_MODE);
    hal_pinmux_set_function(HR3300_SCL_PIN,HR3300_SCL_MODE);

    hal_eint_mask(HR3300_EINT_NUM);
    hal_gpio_init(HR3300_EINT_PIN);
    hal_gpio_pull_up(HR3300_EINT_PIN);
    hal_gpio_set_direction(HR3300_EINT_PIN,HAL_GPIO_DIRECTION_INPUT);
    hal_pinmux_set_function(HR3300_EINT_PIN,HR3300_EINT_MODE);


    config.debounce_time = 10;
    config.trigger_mode  = HAL_EINT_EDGE_RISING;
    hal_eint_init(HR3300_EINT_NUM,&config);
    hal_eint_register_callback(HR3300_EINT_NUM,hal_HR3300_eint_callback,NULL);
    hal_eint_unmask(HR3300_EINT_NUM);

    HR3300_i2c_config.frequency = HAL_I2C_FREQUENCY_200K;
    
    HR3300_i2c_timeout_tick  = (TickType_t)(5000 / portTICK_PERIOD_MS);
    HR3300_i2c_dma_semaphore = xSemaphoreCreateBinary();
        
    Hrs3300_DBG("hal_HR3300_driver_i2c_init\r\n");
	
}

#endif

bool Hrs3300_write_reg(uint8_t addr, uint8_t data) 
{
    int result = 0;
    uint8_t setting[5];

    setting[0] = addr;
    setting[1] = data; /*7.81*/
    result = hal_HR3300_driver_send_data(setting,2);
	
	return result;
}


uint8_t Hrs3300_read_reg(uint8_t addr) 
{
	kal_uint8 result = 0;
	kal_uint8 data;
	
	hal_HR3300_driver_send_recieve_data(addr,&data,1);
	return data;
}

#ifdef MALLOC_MEMORY
void *hr_malloc(size_t size)
{
	return (void*)malloc(size);
}

void hr_free(void * ptr)
{
	free(ptr);
}
#endif

uint16_t Hrs3300_read_hrs(void)
{
	uint8_t  databuf[3];
	uint16_t data;

	databuf[0] = Hrs3300_read_reg(0x09);	// addr09, bit
	databuf[1] = Hrs3300_read_reg(0x0a);	// addr0a, bit
	databuf[2] = Hrs3300_read_reg(0x0f);	// addr0f, bit
	data = ((databuf[0]<<8)|((databuf[1]&0x0F)<<4)|(databuf[2]&0x0F));
	return data;
}

uint16_t Hrs3300_read_als(void)
{
	uint8_t  databuf[3];
	uint16_t data;

	databuf[0] = Hrs3300_read_reg(0x08);	// addr09, bit [10:3]
	databuf[1] = Hrs3300_read_reg(0x0d);	// addr0a, bit [17:11]
	databuf[2] = Hrs3300_read_reg(0x0e);	// addr0f, bit [2:0]
	data = ((databuf[0]<<3)|((databuf[1]&0x3F)<<11)|(databuf[2]&0x07));
	if (data > 32767) 
	data = 32767;  // prevent overflow of other function
	return data;
}
uint8_t Hrs3300_id =0;
QueueHandle_t xHeartQueue;
TaskHandle_t Hrs3300_xHandle = NULL;
typedef struct
{
    uint32_t msgid;
    uint8_t  *param;
}Hrs3300_queue_msg_struct;
void test_gpt_timer_cb(void *user_data)
{
//	  LOGE("test_gpt_timer_cb................................ \n");
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	BaseType_t status;
	int message = 0;
	Hrs3300_queue_msg_struct msg;
	
	status = xQueueSendFromISR(xHeartQueue, &msg, &xHigherPriorityTaskWoken);
	if( xHigherPriorityTaskWoken )
	{
		portYIELD_FROM_ISR (xHigherPriorityTaskWoken);
	}
	
	if (status != pdTRUE)
	Hrs3300_DBG("[test_gpt_timer_cb_1] xQueueSendFromISR xHeartQueue failed %ld.\n\r", status);
}
void heart_rate_task(void *pvParameters)
{
	Hrs3300_queue_msg_struct msg;
	
    xHeartQueue = xQueueCreate(200,sizeof(Hrs3300_queue_msg_struct));//100
    
	if (hal_gpt_init(HAL_GPT_1) != HAL_GPT_STATUS_OK) {
		Hrs3300_DBG("ERROR : HAL_GPT_1 Init failed\n\r");
	}
	if (hal_gpt_register_callback(HAL_GPT_1, (hal_gpt_callback_t)test_gpt_timer_cb, NULL) != HAL_GPT_STATUS_OK) {
		Hrs3300_DBG("ERROR : HAL_GPT_1 register callback failed\n\r");
	}
	if (hal_gpt_start_timer_us(HAL_GPT_1,41200,HAL_GPT_TIMER_TYPE_REPEAT) != HAL_GPT_STATUS_OK) 
	{//if (hal_gpt_start_timer_us(HAL_GPT_1, 10200, HAL_GPT_TIMER_TYPE_REPEAT) != HAL_GPT_STATUS_OK) {
		Hrs3300_DBG("ERROR : HAL_GPT_1 start timer failed\n\r");
	}
	
    while (1) 
	{
        if (pdTRUE == xQueueReceive(xHeartQueue, &msg, portMAX_DELAY))
        {
			heart_rate_meas_timeout_handler();
        }
    }

}
uint8_t Hrs3300_deep_sleep_handler = 0xFF;
bool Hrs3300_chip_init()
{
	int i =0 ;
    Hrs3300_queue_msg_struct msg;
	
    
#if defined(FUNCTION_OFF_SUPPORT)
	if(NVRAM_info.heartrate_off_flag == true)
	{
		return true;
	}
#endif
	if(temp_info.gHeartRatePwrOn == true)
	{
		return true;
	}
		
	hal_pinmux_set_function(HR3300_EN_PIN, 0); 		  /*   set dierection to be output	*/			  
	hal_gpio_set_direction(HR3300_EN_PIN, HAL_GPIO_DIRECTION_OUTPUT);
	hal_gpio_set_output(HR3300_EN_PIN,HAL_GPIO_DATA_HIGH);
	
    hal_HR3300_driver_i2c_init();

	for(i = 0; i < INIT_ARRAY_SIZE;i++)
	{
		if ( Hrs3300_write_reg( init_register_array[i][0],
								init_register_array[i][1]) != 0 )
		{
		   goto RTN;
		}
	}	
	
	//20161117 added by ericy for "low power in no_touch state" 
	if(hrs3300_power_up_flg == 0)
	{
		reg_0x7f=Hrs3300_read_reg(0x7f) ;
		reg_0x80=Hrs3300_read_reg(0x80) ;
		reg_0x81=Hrs3300_read_reg(0x81) ;
		reg_0x82=Hrs3300_read_reg(0x82) ;
		hrs3300_power_up_flg =	1; 
	}
	
	//20161117 added by ericy for "low power in no_touch state"
	Hrs3300_id = Hrs3300_read_reg(0x00);
	
	Hrs3300_DBG(" Hrs3300_id %d",Hrs3300_id);
	
	Hrs3300_chip_enable();
	Hrs3300_alg_open();
	
	temp_info.blood_lbp=0;
	temp_info.blood_hbp=0;
	
    if (Hrs3300_deep_sleep_handler == 0xFF)
    Hrs3300_deep_sleep_handler = hal_sleep_manager_set_sleep_handle("Hrs3300");
    hal_sleep_manager_acquire_sleeplock(Hrs3300_deep_sleep_handler, HAL_SLEEP_LOCK_ALL);
	
	if(Hrs3300_id == 33)
	{
		if(Hrs3300_xHandle == NULL)
			xTaskCreate(heart_rate_task, (const char *)"heart_rate_task", 1024*4, NULL, TASK_PRIORITY_HIGH,&Hrs3300_xHandle);//1024
        else
		{
			if (hal_gpt_start_timer_us(HAL_GPT_1,41200,HAL_GPT_TIMER_TYPE_REPEAT) != HAL_GPT_STATUS_OK) 
			{//if (hal_gpt_start_timer_us(HAL_GPT_1, 10200, HAL_GPT_TIMER_TYPE_REPEAT) != HAL_GPT_STATUS_OK) {
				Hrs3300_DBG("ERROR : HAL_GPT_1 start timer failed\n\r");
			}
		}
	}
		
	temp_info.gHeartRatePwrOn = true;

	return true;
RTN:
	return false; 
	  
}

void Hrs3300_chip_disable();
void my_fifo(char *input,char *output,int num)
{    
	int temp=0,i=0;	

	for(i = 0; i < num; i++)		
	{			
		if(i == (num-1))			
		output[i] = temp;			
		else			
		output[i] = input[i+1];		
	}		

}
void mmi_stop_heart_rate_count(void)
{
    int i=0;
	char input[24]={0};
	hal_rtc_time_t curtime;	
	
	
	if(temp_info.gHeartRatePwrOn == false)
	return;
	
	hal_rtc_get_time(&curtime);
	
	if(NVRAM_info.Hr_log[23]>0)
	{
	  memset(input,0,sizeof(input));
	  strcat(input,NVRAM_info.Hr_log);	
	  memset(NVRAM_info.Hr_log,0,sizeof(NVRAM_info.Hr_log));
	  my_fifo(input,NVRAM_info.Hr_log,24);
	}
	
	for(i=0;i<24;i++)
	{
	  if((NVRAM_info.Hr_log[i]==0)&&(temp_info.hartrate_cnt>40&&(temp_info.hartrate_cnt<180)))
	   {
		  NVRAM_info.Hr_log[i]=temp_info.hartrate_cnt;
		  break;
	   }
	}
	
	hal_gpt_stop_timer(HAL_GPT_1);
    hal_sleep_manager_release_sleeplock(Hrs3300_deep_sleep_handler, HAL_SLEEP_LOCK_ALL);

	Hrs3300_chip_disable();
	
	hal_gpio_set_output(HR3300_EN_PIN,HAL_GPIO_DATA_LOW);

#if defined(MD_WATCH_SUPPORT)
	if((curtime.rtc_min!=0) && (temp_info.show_idle_flag != HEART_RATE_SCREEN))// && (temp_info.show_idle_flag != BLOOD_SCREEN)
#else
	if(curtime.rtc_min!=0)
#endif		
	{
		if((temp_info.blood_lbp>=40)&&(temp_info.blood_lbp<=200))
		{
			sbit_m2m_ct_send_massege(M2M_CT_hartblood_data_T45,NULL,0,0);
		}
		else if((temp_info.hartrate_cnt>=40)&&(temp_info.hartrate_cnt<=200))
		{
			sbit_m2m_ct_send_massege(M2M_CT_hartrate_data_T28,NULL,0,0);
		}
	}
	
	hrs3300_power_up_flg =	0; 
	temp_info.gHeartRatePwrOn = false;

		
}

void Hrs3300_chip_enable()
{	
	Hrs3300_write_reg( 0x16, 0x78 );
	Hrs3300_write_reg( 0x01, 0xd0 );	
	Hrs3300_write_reg( 0x0c, 0x2e );
}

void Hrs3300_chip_disable()
{
	Hrs3300_write_reg( 0x01, 0x08 );
	Hrs3300_write_reg( 0x02, 0x80 );
	Hrs3300_write_reg( 0x0c, 0x4e );
	
	Hrs3300_write_reg( 0x16, 0x88 );
	
	Hrs3300_write_reg( 0x0c, 0x22 );
	Hrs3300_write_reg( 0x01, 0xf0 );
	Hrs3300_write_reg( 0x0c, 0x02 );

	Hrs3300_write_reg( 0x0c, 0x22 );
	Hrs3300_write_reg( 0x01, 0xf0 );
	Hrs3300_write_reg( 0x0c, 0x02 );
	
	Hrs3300_write_reg( 0x0c, 0x22 );
	Hrs3300_write_reg( 0x01, 0xf0 );
	Hrs3300_write_reg( 0x0c, 0x02 );
	
	Hrs3300_write_reg( 0x0c, 0x22 );
	Hrs3300_write_reg( 0x01, 0xf0 );
	Hrs3300_write_reg( 0x0c, 0x02 );
	
	return ;	
}
extern signed char stkMotion_chip_read_xyz(signed short *x,signed short *y,signed short *z);
void heart_rate_meas_timeout_handler(void)
{
	uint32_t        err_code;
	uint16_t        heart_rate;
	uint16_t hrm_raw_data;
	uint16_t als_raw_data;
	uint8_t gsen_data;	
	static uint8_t hartrate_test[6]={0};
	hrs3300_results_t alg_results;
	static int i=0;
	signed short x,y,z; 
#ifdef BP_CUSTDOWN_ALG_LIB		
	hrs3300_bp_results_t	bp_alg_results ;	
#endif	
	static uint16_t timer_index =0;

    {
		hrm_raw_data = Hrs3300_read_hrs();
		als_raw_data = Hrs3300_read_als();	// 20170430
		
		if(temp_info.stk8321_driver_task_flag == 1)
		{
			//stkMotion_chip_read_xyz(&x,&y,&z);
			//Hrs3300_alg_send_data(hrm_raw_data, als_raw_data, (8*x), (8*y), (8*z), 0); 
			Hrs3300_alg_send_data(hrm_raw_data, als_raw_data,0,0,0,0); 
		}
		else
		{
			Hrs3300_alg_send_data(hrm_raw_data, als_raw_data,0,0,0,0); 
		}
		
		alg_results = Hrs3300_alg_get_results();
		
		timer_index ++;
		
		if (timer_index >= 25)	
		{
			timer_index =0;
			
			if (alg_results.alg_status == MSG_NO_TOUCH)
			{
				i = 0;
				temp_info.hartrate_cnt = 0;
				temp_info.blood_lbp = 0;
				temp_info.blood_hbp = 0;
				memset(hartrate_test, 0, sizeof(hartrate_test));
				Hrs3300_DBG("================hartrate_cnt 3300: MSG_NO_TOUCH");
			}
			else if(alg_results.alg_status == MSG_PPG_LEN_TOO_SHORT)
			{
				i = 0;
				temp_info.hartrate_cnt = 0;
				temp_info.blood_lbp = 0;
				temp_info.blood_hbp = 0;
				memset(hartrate_test, 0, sizeof(hartrate_test));
				Hrs3300_DBG("================hartrate_cnt 3300: MSG_PPG_LEN_TOO_SHORT");
			}
			else if(alg_results.alg_status == MSG_HR_READY)
			{
				if(i == 6)
				{
					i = 0;
				}
				if((alg_results.hr_result >= 40)&&(alg_results.hr_result <= 180))
				{
					hartrate_test[i] = alg_results.hr_result;
				}
				i ++;
				if(hartrate_test[5] > 40)
				{
					temp_info.hartrate_cnt=(hartrate_test[0]+hartrate_test[1]+hartrate_test[2]+hartrate_test[3]+hartrate_test[4]+hartrate_test[5])/6;
					Hrs3300_DBG("================hartrate_cnt 3300: %d",temp_info.hartrate_cnt);
				}
				else
				{
					Hrs3300_DBG("================hartrate_cnt 3300: [%d]", hartrate_test[5]);
				}
#if defined(MD_WATCH_SUPPORT)
				if(true)
#else
				if(get_sbit_show_meun_id()!=2)
#endif		
				{
					bp_alg_results = Hrs3300_alg_get_bp_results(); 
					Hrs3300_DBG("================blood_hbp 3300: %d",temp_info.blood_hbp);
					if (bp_alg_results.sbp!= 0)
					{
					  if((atoi(NVRAM_info.blood_hbp) >= 80)&&(atoi(NVRAM_info.blood_hbp) <= 200))
					  {
						  temp_info.blood_lbp=(atoi(NVRAM_info.blood_lbp))+((bp_alg_results.sbp-125)/2);
						  temp_info.blood_hbp=(atoi(NVRAM_info.blood_hbp))+((bp_alg_results.dbp-75)/2);
					  }
					  else
					  {
						  temp_info.blood_lbp=bp_alg_results.dbp;
						  temp_info.blood_hbp=bp_alg_results.sbp;
					  }
					}
				}
			}
			
		}

	}


}



