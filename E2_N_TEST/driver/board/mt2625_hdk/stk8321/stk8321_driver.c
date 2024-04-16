#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* device.h includes */
#include "mt2625.h"

/* hal includes */
#include "hal.h"
#include "memory_attribute.h"


#include "sys_init.h"
#include "task_def.h"
#include "atci.h"

#include "syslog.h"

#include "queue.h"
#include "semphr.h"

#include "memory_attribute.h"
#include "h10_mmi.h"


#include "stkMotion.h"
#include "hal_gpt.h"

#ifdef MTK_USB_DEMO_ENABLED
#include "usb.h"
#endif


typedef	uint8_t u8;/**< used for unsigned 8bit */
typedef	uint16_t u16;/**< used for unsigned 16bit */
typedef	uint32_t u32;/**< used for unsigned 32bit */
typedef	uint64_t u64;/**< used for unsigned 64bit */

/*signed integer types*/
typedef	int8_t s8;/**< used for signed 8bit */
typedef	int16_t s16;/**< used for signed 16bit */
typedef	int32_t s32;/**< used for signed 32bit */
typedef	int64_t s64;/**< used for signed 64bit */


/* Private typedef -----------------------------------------------------------*/
typedef	uint8_t u8;/**< used for unsigned 8bit */
typedef	uint16_t u16;/**< used for unsigned 16bit */
typedef	uint32_t u32;/**< used for unsigned 32bit */
typedef	uint64_t u64;/**< used for unsigned 64bit */

/*signed integer types*/
typedef	int8_t s8;/**< used for signed 8bit */
typedef	int16_t s16;/**< used for signed 16bit */
typedef	int32_t s32;/**< used for signed 32bit */
typedef	int64_t s64;/**< used for signed 64bit */
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
//#define FREERTOS_TEST

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static void vTestTask_polling(void *pvParameters);
static void vTestTask_interrupt(void *pvParameters);



/* Create the log control block for freertos module.
 * The initialization of the log is in the sys_init.c.
 * Please refer to the log dev guide under /doc folder for more details.
 */
log_create_module(freertos, PRINT_LEVEL_INFO);

#define mainCHECK_DELAY ( ( portTickType ) 1000 / portTICK_RATE_MS )




#define STK8321_SLAVE_ADDR  0x0F
#define STK8321_I2C_PORT    HAL_I2C_MASTER_0


#define STK8321_DEBUG

#ifdef STK8321_DEBUG
#define    stk8321_debug(fmt,arg...)     LOG_I(common, "[stk8321]:"fmt,##arg)
#else
#define    stk8321_debug(fmt,arg...)
#endif



typedef struct
{
    uint32_t msgid;
    uint8_t  *param;
}stk_queue_msg_struct;

xQueueHandle     stk8321_queue_handle;

#define	I2C_BUFFER_LEN 32

ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN uint8_t  array_dma[I2C_BUFFER_LEN];

void hal_stk8321_eint_callback(void *arg)
{
    stk_queue_msg_struct msg;
    BaseType_t xHigherPriorityTaskWoken;

    msg.msgid = 0;
    msg.param = NULL;

    hal_eint_mask(HAL_EINT_NUMBER_4);
    hal_eint_unmask(HAL_EINT_NUMBER_4);

    xQueueSendFromISR(stk8321_queue_handle,&msg,&xHigherPriorityTaskWoken);

	if( xHigherPriorityTaskWoken )
	{
		// Actual macro used here is port specific.
		portYIELD_FROM_ISR (xHigherPriorityTaskWoken);
	}

}

void hal_stk8321_driver_i2c_init(void)
{
    mems_i2c_init(HAL_I2C_MASTER_0, HAL_I2C_FREQUENCY_400K);
    hal_eint_config_t eint1_config;
    eint1_config.trigger_mode = HAL_EINT_EDGE_FALLING;
    eint1_config.debounce_time = 1;
    hal_eint_init(HAL_EINT_NUMBER_4, &eint1_config);
    hal_eint_register_callback(HAL_EINT_NUMBER_4, hal_stk8321_eint_callback, NULL);
    hal_eint_unmask(HAL_EINT_NUMBER_4);
}



signed char stk_register_write( unsigned char i2cAddr, unsigned char regAddr, unsigned char *regValue, unsigned char cnt)
{
    s32 iError = 0, i = 0;;
    //u8 array[I2C_BUFFER_LEN];
    u8 stringpos = 0;

    memset(array_dma, 0, sizeof(array_dma));
    array_dma[0] = regAddr;
    for (stringpos = 0; stringpos < cnt; stringpos++) {
        array_dma[stringpos + 1] = *(regValue + stringpos);
    }
    for ( i = 0; i < 5; i ++) {
        iError = mems_i2c_write(STK8321_SLAVE_ADDR, array_dma, cnt+1);
        if (!iError) {
            break;
        }
        vTaskDelay(1);
    }

  //  stk8321_debug("stk_register_write: addr=0x%x,value=0x%x cnt=%d", regAddr, regValue[0], cnt);
    return (s8)iError;
}

signed char stk_register_read( unsigned char i2cAddr, unsigned char regAddr, unsigned char *regValue, unsigned char cnt )
{
    s32 iError = 0, i = 0;
    u8 stringpos = 0;

    /* Please take the below function as your reference
     * for read the data using I2C communication
     * add your I2C rad function here.
     * "IERROR = I2C_WRITE_READ_STRING(DEV_ADDR, ARRAY, ARRAY, 1, CNT)"
     * iError is an return value of SPI write function
     * Please select your valid return value
     * In the driver SUCCESS defined as 0
     * and FAILURE defined as -1
     */

    uint8_t regAddrTemp;
    uint8_t regValueTemp[256];




    if(0x3F == regAddr)
    {            
        for ( i = 0; i < cnt/6; i ++) {
            //iError = mems_i2c_write_read_dma(STK8321_SLAVE_ADDR, regAddr, array_dma, 6); 
            iError = mems_i2c_write_read(STK8321_SLAVE_ADDR, regAddr, array_dma, 6);        

            memcpy((regValueTemp+i*6),array_dma,6);                  
        }
    }
    else
    {
        iError = mems_i2c_write_read_dma(STK8321_SLAVE_ADDR, regAddr, array_dma, cnt%15); 
        memcpy(regValueTemp,array_dma,cnt%15);

        regAddrTemp = regAddr + cnt%15;

        for ( i = 0; i < cnt/15; i ++) {            
            iError = mems_i2c_write_read_dma(STK8321_SLAVE_ADDR, regAddrTemp, array_dma, 15); 
            memcpy((regValueTemp+i*15+cnt%15),array_dma,15);
            regAddrTemp  += 15;  
        }
    }

    for (stringpos = 0; stringpos < cnt; stringpos++) {
        *(regValue + stringpos) = regValueTemp[stringpos];
       // stk8321_debug("stk_register_read: addr=0x%x,value=0x%x cnt=%d", regAddr + stringpos, regValue[stringpos], cnt);
    }


    return (s8)iError;
}

void stk_mdelay(unsigned int msec)
{
    /*Here you can write your own delay routine*/
    vTaskDelay(msec);   
}
extern void powerkey_unlock_sleep(void);
signed char sendEvent(STKMOTION_EVENT event,signed int data)
{
	static int step_flag=0;
	static int RAISE_flag=0;
	static int rolling_flag = 0;
	
#if defined(FUNCTION_OFF_SUPPORT)
	if(NVRAM_info.g_sensor_off_flag == true)
	{
		return 0;
	}
#endif	
  switch(event){
	case STK_EVENT_STEP_NOTIFY:
		NVRAM_info.value_steps = data + temp_info.step_cont_nvram;
		if(step_flag != NVRAM_info.value_steps)
		{
			temp_info.Sedentary_idle_flag = 0;
			step_flag = NVRAM_info.value_steps;
		    temp_info.Sedentary_Remind_Time = 0;
		}
	break;
	case STK_EVENT_FALL_NOTIFY:
		//stk8321_debug("STK8321_EVENT_FALL_NOTIFY(%d)", data);
	break;
	case STK_EVENT_SLEEP_NOTIFY:
	break;
	case STK_EVENT_ACTION_NOTIFY:
		//stk8321_debug("@@@@@@@@@@@@@@@@@@@@@@@ STK8321_EVENT_ACTION_NOTIFY @@@@@@@@@@@@@@@@@@@@@@@(%d)", data);
    break;
	case STK_EVENT_CALORIE_NOTIFY:
		//stk8321_debug("STK8321_EVENT_CALORIE_NOTIFY(%d)", data);
	break;
	case STK_EVENT_SEDENTARY_NOTIFY:
		//stk8321_debug("@@@@@@@@@@@@@@@@@@@@@@@ STK8321_EVENT_SEDENTARY_NOTIFY @@@@@@@@@@@@@@@@@@@@@@@(%d)", data);    
	break;
	case STK_EVENT_DISTANCE_NOTIFY:
		//stk8321_debug("STK8321_EVENT_DISTANCE_NOTIFY(%d)", data);
	break;
	case STK_EVENT_FLIP_NOTIFY:
		//stk8321_debug("@@@@@@@@@@@@@@@@@@@@@@@ STK8321_EVENT_FLIP_NOTIFY @@@@@@@@@@@@@@@@@@@@@@@(%d)", data); 
	break;
	case STK_EVENT_RAISE_NOTIFY:
		if((data==1)&&(RAISE_flag==0)&&(temp_info.backlight_counter==0)&&(temp_info.sbit_backlight_flag == false))
		{
			RAISE_flag=1;
			if(NVRAM_info.Bright_screen_flag == 1)
			{
				temp_info.backlight_counter = 12;
				temp_info.stk8321_raise_mark=1;
				sbit_m2m_ct_send_massege_isr(M2M_DISPLAY_BACKLIGHT,NULL,0,1);
			}
		}
		else
		{
			RAISE_flag=0;
			temp_info.stk8321_raise_mark=0;
		}
		//stk8321_debug("STK8321_EVENT_RAISE_NOTIFY(%d)", data);
	break;
	case STK_EVENT_SHAKE_NOTIFY:
		if(data == 1)
		{
			hal_rtc_time_t curtime; 
			hal_rtc_get_time(&curtime);
			temp_info.shake_flag ++;
			if(curtime.rtc_hour < 6)
			NVRAM_info.waggle_level[curtime.rtc_hour] ++;
		}

		if(rolling_flag != data)
		{
			if(NVRAM_info.deep_sleep_time > 0)
			{
				NVRAM_info.rolling_cumulative++;
			}
			else
			{
				NVRAM_info.rolling_cumulative = 0;
			}
		}
		//stk8321_debug("@@@@@@@@@@@@@@@@@@@@@@@ STK8321_EVENT_SHAKE_NOTIFY @@@@@@@@@@@@@@@@@@@@@@@(%d),(%d)",temp_info.shake_flag,data);   
	break;
	default:
	break;
  }

  return 0;
  
}
     

signed char stk_printf(char *fmt, ... )
{
    char Buffer[100];

    va_list list_ptr;
    va_start(list_ptr, fmt);
    vsprintf(Buffer, fmt, list_ptr);
    va_end(list_ptr);

	stk8321_debug("%s", Buffer);

    return 0;
}


struct stkMotion_op_s operators_handle = {sendEvent, stk_register_read, stk_register_write,stk_mdelay,NULL,NULL,stk_printf};



uint8_t hal_stk8321_driver_get_chip_id(void)
{
    signed char result;
    uint8_t chip;
    
    result = stk_register_read(STK8321_SLAVE_ADDR,0x00,&chip,1);

    if(result != 0)
	{
        stk8321_debug("###############################################hal_stk8321_driver_get_chip_id error:%d\r\n",result);
    }
	else
    {
        temp_info.stk8321_chip_id = chip;
        stk8321_debug("##################################################hal_stk8321_driver_get_chip_id:0x%X\r\n",chip);
    }

    return chip;
}
void hal_stk8321_driver_init(void)
{
    uint8_t chip;
    char res=1;
	hal_rtc_time_t rtc_time;
    hal_rtc_status_t ret;
    struct stk2x2_time_t t_time;

    hal_stk8321_driver_i2c_init();

    chip = hal_stk8321_driver_get_chip_id();

	res = stkMotion_Init(&operators_handle); 

	//if(0 == res)
	{
        ret = hal_rtc_get_time(&rtc_time);
        if (HAL_RTC_STATUS_OK != ret) {
            configASSERT(0);
            return;
        } 
        t_time.t_RTC_Year = rtc_time.rtc_year;
        t_time.t_RTC_Month = rtc_time.rtc_mon;
        t_time.t_RTC_Day = rtc_time.rtc_day;
        t_time.t_RTC_Hour = rtc_time.rtc_hour;
        t_time.t_RTC_Minute = rtc_time.rtc_min;
        t_time.t_RTC_Second = rtc_time.rtc_sec;
        t_time.t_sys_milliSecond = sensor_driver_get_ms_tick();


        stkMotion_Control(STK_PEDOMETER_X,STK_ENABLE_X);
        stkMotion_Control(STK_RAISE_X,STK_ENABLE_X);
        stkMotion_Control(STK_FALL_X,STK_ENABLE_X);
        stkMotion_Control(STK_SLEEP_X,STK_ENABLE_X);

        stkMotion_Control(STK_ACTION_X, STK_ENABLE_X);
        stkMotion_Control(STK_SEDENTARY_X, STK_ENABLE_X);
        stkMotion_Control(STK_CALORIE_X, STK_ENABLE_X);
        stkMotion_Control(STK_DISTANCE_X, STK_ENABLE_X);
        stkMotion_Control(STK_FLIP_X, STK_ENABLE_X);
        stkMotion_Control(STK_SHAKE_X, STK_ENABLE_X);
		
		/* initialize algorithm */
		stkMotion_Clear_Pedometer_Value(); // reset and enable pedometer
		stkMotion_Set_Pedometer_Params(10, 1); // sensitivity, default: 2 (range 0~4), level 0 is most sensitive
		stkMotion_Clear_Step_Calorie_Distance_Value(); // reset calorie / distance data

		stkMotion_Set_Sedentary_Parma(30, 30); //monitor time: 59min (range 1~59min), step: 30
		stkMotion_Set_Calorie_Parma(180, 70, 30, 1); //height: 180cm, weight: 70kg, age: 30 years old, gender: male
		stkMotion_Set_Raise_Parma(1,5); // dir is 6 (0~7), sensitivity is 5 (1~5) ( 5 is most sensitive)
		stkMotion_Set_Fall_Parma(3); // sensitivity is 5 (1~10), level 1 is most sensitive
		stkMotion_Set_Sleep_Parma(t_time, 3); // the sensitive setting of sleep, range 1~10, level 1 is most sensitive
		stkMotion_Set_Shake_Parma(10); // the slope threshold of shake, unit: mg. default is 80mg (75 ~ 250mg)
		
        stkMotion_Set_Debug_Marker_Level(STK_DEBUG_ERR, STK_INFO);   
	}
	

}

/**
* @brief       Task main function
* @param[in]   pvParameters: Pointer that will be used as the parameter for the task being created.
* @return      None.
*/
static void vTestTask_interrupt(void *pvParameters)
{

    stk_queue_msg_struct msg;
    
    hal_stk8321_driver_init();    
    stk8321_queue_handle = xQueueCreate(50,sizeof(stk_queue_msg_struct));

    while(1)
	{
        if(xQueueReceive(stk8321_queue_handle,&msg,portMAX_DELAY))
        {
            switch(msg.msgid)
            {
                case 0:
                	//stk8321_debug( "stkMotion_INT_Process_Data...\n\r");
                    stkMotion_INT_Process_Data();
                    break;
                default:
                	stk8321_debug( "something wrong\n\r");                
                    break;
            }
        }
    }

}

static void vTestTask_polling(void *pvParameters)
{

    uint32_t idx = (int)pvParameters;

    hal_rtc_time_t rtc_time;
    hal_rtc_status_t ret;
    struct stk2x2_time_t t_time;
    portTickType xLastExecutionTime, xDelayTime;

    xLastExecutionTime = xTaskGetTickCount();
    xDelayTime = mainCHECK_DELAY;

    hal_stk8321_driver_init();  
    hal_eint_mask(HAL_EINT_NUMBER_4);

    while (1) 
	{          
        //stk8321_debug( "Hello, STK8321 interrupt Task\n\r");
		ret = hal_rtc_get_time(&rtc_time);
		if (HAL_RTC_STATUS_OK != ret) 
		{
			configASSERT(0);
			return;
		} 
		t_time.t_RTC_Year = rtc_time.rtc_year;
		t_time.t_RTC_Month = rtc_time.rtc_mon;
		t_time.t_RTC_Day = rtc_time.rtc_day;
		t_time.t_RTC_Hour = rtc_time.rtc_hour;
		t_time.t_RTC_Minute = rtc_time.rtc_min;
		t_time.t_RTC_Second = rtc_time.rtc_sec;
		t_time.t_sys_milliSecond = sensor_driver_get_ms_tick();
		stkMotion_Process_Data(t_time);
		vTaskDelayUntil(&xLastExecutionTime, xDelayTime);
    }

}

void hal_stk8321_driver_task(void)
{
    temp_info.stk8321_driver_task_flag = 1;
	xTaskCreate(vTestTask_polling,"stk8321",4800,NULL,3,NULL);
}

