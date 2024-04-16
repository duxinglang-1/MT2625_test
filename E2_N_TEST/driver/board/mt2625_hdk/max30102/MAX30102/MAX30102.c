/** \file max30102.cpp ******************************************************
*
* Project: MAXREFDES117#
* Filename: max30102.cpp
* Description: This module is an embedded controller driver for the MAX30102
*
*
* --------------------------------------------------------------------
*
* This code follows the following naming conventions:
*
* char              ch_pmod_value
* char (array)      s_pmod_s_string[16]
* float             f_pmod_value
* int32_t           n_pmod_value
* int32_t (array)   an_pmod_value[16]
* int16_t           w_pmod_value
* int16_t (array)   aw_pmod_value[16]
* uint16_t          uw_pmod_value
* uint16_t (array)  auw_pmod_value[16]
* uint8_t           uch_pmod_value
* uint8_t (array)   auch_pmod_buffer[16]
* uint32_t          un_pmod_value
* int32_t *         pn_pmod_value
*
* ------------------------------------------------------------------------- */
/*******************************************************************************
* Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************
*/
#include "MAX30102.h"
#include "hal_gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

#include "memory_attribute.h"
#include "syslog.h"

#include "hal_i2c_master.h"
#include "hal_gpio.h"
#include "hal_eint.h"
#include "apb_proxy.h"

#include "algorithm.h"
log_create_module(max30102_demo, PRINT_LEVEL_INFO);

#define SBIT_ERR(fmt,arg...)   LOG_E(max30102_demo, "[max30102]: "fmt,##arg)
#define SBIT_WARN(fmt,arg...)  LOG_W(max30102_demo, "[max30102]: "fmt,##arg)
#define SBIT_DBG(fmt,arg...)   LOG_I(max30102_demo, "[max30102]: "fmt,##arg)

//I2C i2c(I2C_SDA, I2C_SCL);//SDA-PB9,SCL-PB8
#define MAXIM_SCL_PIN  HAL_GPIO_16
#define MAXIM_SDA_PIN  HAL_GPIO_17

#define MAX30102_EINT_PIN  HAL_GPIO_15
#define MAX30102_EINT_MODE 7
#define MAX30102_EINT_NUM  HAL_EINT_NUMBER_15

#define MAXIM_SCL_SET_H         hal_gpio_set_output(MAXIM_SCL_PIN, HAL_GPIO_DATA_HIGH)
#define MAXIM_SCL_SET_L         hal_gpio_set_output(MAXIM_SCL_PIN, HAL_GPIO_DATA_LOW)
#define MAXIM_SDA_SET_H         hal_gpio_set_output(MAXIM_SDA_PIN, HAL_GPIO_DATA_HIGH)
#define MAXIM_SDA_SET_L         hal_gpio_set_output(MAXIM_SDA_PIN, HAL_GPIO_DATA_LOW)

#define MAXIN_SDA_INPUT         hal_gpio_set_direction(MAXIM_SDA_PIN, HAL_GPIO_DIRECTION_INPUT);
#define MAXIN_SDA_OUTPUT        hal_gpio_set_direction(MAXIM_SDA_PIN, HAL_GPIO_DIRECTION_OUTPUT);

#define MAXIN_SDA_READ(data)    hal_gpio_get_input(MAXIM_SDA_PIN, (hal_gpio_data_t *)data)

max_i2c_handle_struct max_i2c;

uint32_t aun_ir_buffer[500]; //IR LED sensor data
int32_t n_ir_buffer_length;    //data length
uint32_t aun_red_buffer[500];    //Red LED sensor data
int32_t n_sp02; //SPO2 value
int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid
int32_t n_heart_rate;   //heart rate value
int8_t  ch_hr_valid,max_calc;    //indicator to show if the heart rate calculation is valid

typedef struct
{
    uint32_t msgid;
    uint8_t  *param;
}maxim_queue_msg_struct;

xQueueHandle     max30102_queue_handle;
TimerHandle_t    max30102_timer;

//extern void maxim_heart_rate_and_oxygen_saturation(uint32_t *pun_ir_buffer ,  int32_t n_ir_buffer_length, uint32_t *pun_red_buffer ,   int32_t *pn_spo2, int8_t *pch_spo2_valid ,  int32_t *pn_heart_rate , int8_t  *pch_hr_valid);


void hal_max30102_eint_callback(void *arg)
{
    maxim_queue_msg_struct msg;
    BaseType_t xHigherPriorityTaskWoken;

    msg.msgid = 0;
    msg.param = NULL;
    
    hal_eint_mask(MAX30102_EINT_NUM);
    xQueueSendFromISR(max30102_queue_handle,&msg,&xHigherPriorityTaskWoken);
    //SBIT_DBG("=============================max30102 hal_max30102_eint_callback\r\n");
	if( xHigherPriorityTaskWoken )
	{
		// Actual macro used here is port specific.
		portYIELD_FROM_ISR (xHigherPriorityTaskWoken);
	}
    
    hal_eint_unmask(MAX30102_EINT_NUM);

}




void maxim_i2c_delay(uint32_t us)
{
    uint32_t i;

    for(i= 0; i<60*us;i++);
}

void maxim_i2c_start(void)
{
    MAXIM_SCL_SET_H;
    maxim_i2c_delay(10);
    MAXIM_SDA_SET_H;
    maxim_i2c_delay(20);
    MAXIM_SDA_SET_L;
    maxim_i2c_delay(10);
    MAXIM_SCL_SET_L;
    maxim_i2c_delay(10);
}

void maxim_i2c_stop(void)
{
    MAXIM_SCL_SET_H;
    maxim_i2c_delay(5);    
    MAXIM_SDA_SET_L;
    maxim_i2c_delay(10);
    MAXIM_SDA_SET_H;
    maxim_i2c_delay(10);
    MAXIM_SCL_SET_L;
    maxim_i2c_delay(10);
}

bool maxim_i2c_check_ack(void)
{
    char data;
    bool result;
    
    MAXIN_SDA_INPUT;
    maxim_i2c_delay(10);
    
    MAXIM_SCL_SET_H;
    maxim_i2c_delay(5);    
    MAXIN_SDA_READ(&data);
    if(data == HAL_GPIO_DATA_LOW){
        result = true;
    }else{
        result = false;
    }
    MAXIM_SCL_SET_L;
    maxim_i2c_delay(20);
    MAXIN_SDA_OUTPUT;
    maxim_i2c_delay(10);
    MAXIM_SDA_SET_L;
    
    return result;
}

void maxim_i2c_send_byte(uint8_t data)
{
    uint8_t i;
        
    for(i=0;i<8;i++)
    {
        if(data&(1<<(7-i))){
            MAXIM_SDA_SET_H;
        }else{
            MAXIM_SDA_SET_L;
        }
        maxim_i2c_delay(10);
        MAXIM_SCL_SET_H;
        maxim_i2c_delay(10);
        MAXIM_SCL_SET_L;
        maxim_i2c_delay(10);
    }
}

uint8_t maxim_i2c_read_byte(bool ack)
{
    int i;
    uint8_t read,data = 0;

    MAXIN_SDA_INPUT;
    maxim_i2c_delay(10);
    data = 0;
    for(i=7;i>=0;i--)
    {
        MAXIN_SDA_READ(&read);
        if(read == HAL_GPIO_DATA_HIGH){
            data |=(0x01<<i);
        }
        MAXIM_SCL_SET_H;
        maxim_i2c_delay(10);
        MAXIM_SCL_SET_L;
        maxim_i2c_delay(15);
    }

    MAXIN_SDA_OUTPUT;
    maxim_i2c_delay(10);
    if(ack){
        MAXIM_SDA_SET_L;
    }else{
        MAXIM_SDA_SET_H;
    }
    maxim_i2c_delay(10);
    
    MAXIM_SCL_SET_H;
    maxim_i2c_delay(10);
    MAXIM_SCL_SET_L;
    maxim_i2c_delay(10);

    return data;
}

uint8_t maxim_i2c_write(uint8_t addr,uint8_t *data,uint8_t len,bool ack)
{
    uint8_t i;

    maxim_i2c_start();  
    maxim_i2c_send_byte(addr); 
    if(maxim_i2c_check_ack() == false){
        SBIT_DBG("[write]addr write ack error");
        return 1;
    }

    for(i=0;i<len;i++){
        maxim_i2c_send_byte(*(data+i)); 
        if(maxim_i2c_check_ack()== false){
            SBIT_DBG("[write]addr write ack error");
            return 1;
        }
    }
    
    if(ack != true){
        maxim_i2c_stop();
    }

    return 0;
}

uint8_t maxin_i2c_read(uint8_t addr,uint8_t *data,uint8_t len,bool ack)
{
    uint8_t i;

    maxim_i2c_start();  
    maxim_i2c_send_byte(addr); 
    if(maxim_i2c_check_ack()==false){
        SBIT_DBG("[read]addr write ack error");
        return 1;
    }

    for(i = 0;i<len;i++){
        if(i == len -1){
            *(data+i) = maxim_i2c_read_byte(0); 
        }else{
            *(data+i) = maxim_i2c_read_byte(1); 
        }
    }

    maxim_i2c_stop();
    
    return 0;
 }

bool maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data)
/**
* \brief        Write a value to a MAX30102 register
* \par          Details
*               This function writes a value to a MAX30102 register
*
* \param[in]    uch_addr    - register address
* \param[in]    uch_data    - register data
*
* \retval       true on success
*/
{
  char ach_i2c_data[2];
  ach_i2c_data[0]=uch_addr;
  ach_i2c_data[1]=uch_data;
  //SBIT_DBG("=============================max30102 maxim_max30102_write_reg\r\n");
  if((max_i2c.write == NULL) || (max_i2c.read == NULL)){
    SBIT_DBG("funtion null\r\n");
    return false;
  }
  
  if(max_i2c.write(I2C_WRITE_ADDR, ach_i2c_data, 2, false)==0)
    return true;
  else
    return false;
}

bool maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data)
/**
* \brief        Read a MAX30102 register
* \par          Details
*               This function reads a MAX30102 register
*
* \param[in]    uch_addr    - register address
* \param[out]   puch_data    - pointer that stores the register data
*
* \retval       true on success
*/
{
  char ch_i2c_data;
  ch_i2c_data=uch_addr;
  //SBIT_DBG("=============================max30102 maxim_max30102_read_reg\r\n");
  if((max_i2c.write== NULL) || (max_i2c.read == NULL)){
    SBIT_DBG("funtion null\r\n");
    return false;
  }
  
  if(max_i2c.write(I2C_WRITE_ADDR, &ch_i2c_data, 1, true)!=0)
  {
    SBIT_DBG("write error\r\n");
    return false;
  }
  
  if(max_i2c.read(I2C_READ_ADDR, &ch_i2c_data, 1, false)==0)
  {
    *puch_data=(uint8_t) ch_i2c_data;
    return true;
  }else{
    SBIT_DBG("read error\r\n");
    return false;
  }
}

bool maxim_max30102_init(void)
/**
* \brief        Initialize the MAX30102
* \par          Details
*               This function initializes the MAX30102
*
* \param        None
*
* \retval       true on success
*/
{
  unsigned char uch_temp;
  maxim_queue_msg_struct msg;
  SBIT_DBG("=============================max30102 maxim_max30102_init\r\n");
  //read and clear status register
  maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_temp);
  maxim_max30102_read_reg(REG_INTR_STATUS_2, &uch_temp);

  if(!maxim_max30102_write_reg(REG_INTR_ENABLE_1,0xc0)) // INTR setting
    return false;    
  if(!maxim_max30102_write_reg(REG_INTR_ENABLE_2,0x00))
    return false;
  if(!maxim_max30102_write_reg(REG_FIFO_WR_PTR,0x00))  //FIFO_WR_PTR[4:0]
    return false;
  if(!maxim_max30102_write_reg(REG_OVF_COUNTER,0x00))  //OVF_COUNTER[4:0]
    return false;
  if(!maxim_max30102_write_reg(REG_FIFO_RD_PTR,0x00))  //FIFO_RD_PTR[4:0]
    return false;
  if(!maxim_max30102_write_reg(REG_FIFO_CONFIG,0x0f))  //sample avg = 1, fifo rollover=false, fifo almost full = 17
    return false;
  if(!maxim_max30102_write_reg(REG_MODE_CONFIG,0x03))   //0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
    return false;
  if(!maxim_max30102_write_reg(REG_SPO2_CONFIG,0x27))  // SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (400uS)
    return false;
  
  if(!maxim_max30102_write_reg(REG_LED1_PA,0x24))   //Choose value for ~ 7mA for LED1
    return false;
  if(!maxim_max30102_write_reg(REG_LED2_PA,0x24))   // Choose value for ~ 7mA for LED2
    return false;
  if(!maxim_max30102_write_reg(REG_PILOT_PA,0x7f))   // Choose value for ~ 25mA for Pilot LED
    return false;
  
  msg.msgid = 0;
  msg.param =NULL;
  xQueueSend(max30102_queue_handle,&msg,0);
  
  SBIT_DBG("maxim_init ok\r\n");
  return true;  
}

bool maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led)
/**
* \brief        Read a set of samples from the MAX30102 FIFO register
* \par          Details
*               This function reads a set of samples from the MAX30102 FIFO register
*
* \param[out]   *pun_red_led   - pointer that stores the red LED reading data
* \param[out]   *pun_ir_led    - pointer that stores the IR LED reading data
*
* \retval       true on success
*/
{
  uint32_t un_temp;
  unsigned char uch_temp;
  *pun_red_led=0;
  *pun_ir_led=0;
  char ach_i2c_data[6];
  
  //read and clear status register
  //SBIT_DBG("=============================max30102 maxim_max30102_read_fifo\r\n");
  maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_temp);
  maxim_max30102_read_reg(REG_INTR_STATUS_2, &uch_temp);
  
  ach_i2c_data[0]=REG_FIFO_DATA;
  if(max_i2c.write(I2C_WRITE_ADDR, ach_i2c_data, 1, true)!=0)
    return false;
  if(max_i2c.read(I2C_READ_ADDR, ach_i2c_data, 6, false)!=0)
  {
    return false;
  }
  un_temp=(unsigned char) ach_i2c_data[0];
  un_temp<<=16;
  *pun_red_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[1];
  un_temp<<=8;
  *pun_red_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[2];
  *pun_red_led+=un_temp;
  
  un_temp=(unsigned char) ach_i2c_data[3];
  un_temp<<=16;
  *pun_ir_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[4];
  un_temp<<=8;
  *pun_ir_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[5];
  *pun_ir_led+=un_temp;
  *pun_red_led&=0x03FFFF;  //Mask MSB [23:18]
  *pun_ir_led&=0x03FFFF;  //Mask MSB [23:18]
  
  
  return true;
}

bool maxim_max30102_reset()
/**
* \brief        Reset the MAX30102
* \par          Details
*               This function resets the MAX30102
*
* \param        None
*
* \retval       true on success
*/
{
	SBIT_DBG("=============================max30102 maxim_max30102_reset \r\n");
    if(!maxim_max30102_write_reg(REG_MODE_CONFIG,0x40))
        return false;
    else
        return true;    
}

void hal_max30102_driver_i2c_init(void)
{
    hal_eint_config_t config;
    
    hal_pinmux_set_function(MAXIM_SCL_PIN,0);
    hal_pinmux_set_function(MAXIM_SDA_PIN,0);
    hal_gpio_pull_up(MAXIM_SCL_PIN);
    hal_gpio_pull_up(MAXIM_SDA_PIN);
    hal_gpio_set_direction(MAXIM_SCL_PIN,HAL_GPIO_DIRECTION_OUTPUT);
    hal_gpio_set_direction(MAXIM_SDA_PIN,HAL_GPIO_DIRECTION_OUTPUT); 


    hal_eint_mask(MAX30102_EINT_NUM);
    hal_gpio_init(MAX30102_EINT_PIN);
    hal_gpio_pull_up(MAX30102_EINT_PIN);
    hal_gpio_set_direction(MAX30102_EINT_PIN,HAL_GPIO_DIRECTION_INPUT);
    hal_pinmux_set_function(MAX30102_EINT_PIN,MAX30102_EINT_MODE);

    config.debounce_time = 2;
    config.trigger_mode  = HAL_EINT_EDGE_FALLING;//HAL_EINT_EDGE_RISING;
    hal_eint_init(MAX30102_EINT_NUM,&config);
    hal_eint_register_callback(MAX30102_EINT_NUM,hal_max30102_eint_callback,NULL);
    hal_eint_unmask(MAX30102_EINT_NUM);
    
    max_i2c.read  = maxin_i2c_read;
    max_i2c.write = maxim_i2c_write;
        
    SBIT_DBG("=============================max30102 hal_max30102_driver_i2c_init\r\n");
}


void hal_max30102_driver_init(void)
{
	SBIT_DBG("=============================max30102 hal_max30102_driver_init\r\n");
    hal_max30102_driver_i2c_init();
    maxim_max30102_init();
}

bool maxim_max30102_collect_data(uint32_t red,uint32_t ir)
{
    maxim_queue_msg_struct msg;
    if(n_ir_buffer_length == 0){
        memset(aun_ir_buffer,0,sizeof(aun_ir_buffer));
        memset(aun_red_buffer,0,sizeof(aun_red_buffer));
    }
    aun_ir_buffer[n_ir_buffer_length] = ir;
    aun_red_buffer[n_ir_buffer_length]= red;
    n_ir_buffer_length++;
	
	//SBIT_DBG("=============================max30102 n_ir_buffer_length %d \r\n",n_ir_buffer_length);

    if(n_ir_buffer_length >= 500)
	{
        max_calc = 1;
        msg.msgid = 1;
        msg.param =NULL;
        xQueueSend(max30102_queue_handle,&msg,0);
        return true;
    }

    return false;
}

void hal_max30102_driver_main_loop(void *arg)
{
	int result;
    uint8_t buffer[256]={0},read;
    uint32_t red,ir;
    maxim_queue_msg_struct msg;

    
    #if 1 /*just for test,auto start*/
    max30102_timer = xTimerCreate("max30102", 5000 / portTICK_PERIOD_MS,pdFALSE, NULL, hal_max30102_driver_init);
	xTimerStart(max30102_timer,5);	
	#else
    hal_max30102_driver_i2c_init();
    #endif
    max30102_queue_handle = xQueueCreate(50,sizeof(maxim_queue_msg_struct));
    
	while(1)
	{
        if(xQueueReceive(max30102_queue_handle,&msg,portMAX_DELAY))
        {
			//SBIT_DBG(" ==== max30102 hal_max30102_driver_main_loop\r\n");
            switch(msg.msgid)
            {
                    
                case 0:/*fifo data*/
                    maxim_max30102_read_fifo(&red,&ir);
                    if(max_calc == 0)
					{
                        maxim_max30102_collect_data(red,ir);
                    }
                    //SBIT_DBG("red %d ,ir %d",red,ir);
                    break;
                    
                case 1:/*calculation hr spo2 */
                    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer,n_ir_buffer_length,aun_red_buffer,&n_sp02,&ch_spo2_valid,&n_heart_rate,&ch_hr_valid);
                    n_ir_buffer_length = 0;
                    max_calc = 0;
                    SBIT_DBG("<<< --- >>>> sp02:%d,%d ,hr: %d,%d",n_sp02,ch_spo2_valid,n_heart_rate,ch_hr_valid);
                    break;
                    
                case 3:/*enable*/
                    maxim_max30102_init();
                    break;
                    
                case 4:/*disable*/
                    maxim_max30102_reset();
                    break;
                default:
                    break;
            }
        }
    }
}

void maxim_max30102_onoff(bool on)
{
    maxim_queue_msg_struct msg;
	SBIT_DBG("=============================max30102 maxim_max30102_onoff:%d\r\n", on);
    if(on)
	{
        msg.msgid = 3;
    }
	else
    {
        msg.msgid = 4;
    }
    msg.param =NULL;
    xQueueSend(max30102_queue_handle,&msg,0);
}

#define MAX30102_EN_PIN  HAL_GPIO_28
void hal_max30102_driver_task(void)
{
	SBIT_DBG("=============================max30102 hal_max30102_driver_task\r\n");
	hal_pinmux_set_function(MAX30102_EN_PIN, 0); 		  /*   set dierection to be output	*/			  
	hal_gpio_set_direction(MAX30102_EN_PIN, HAL_GPIO_DIRECTION_OUTPUT);
	hal_gpio_set_output(MAX30102_EN_PIN,HAL_GPIO_DATA_HIGH);
    xTaskCreate(hal_max30102_driver_main_loop, "max30102", 4800, NULL, 3, NULL);
}

