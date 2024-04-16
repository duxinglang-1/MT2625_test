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

#include "mt2625_hdk_lcd.h"

#include "hal_gpt.h"
#include "hal_log.h"
#include "hal_spi_master.h"
#include "hal_gpio.h"

#ifdef FREERTOS_ENABLE
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
SemaphoreHandle_t lcd_sema = NULL;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_POLLING_TIMEOUT (20000)

volatile LCD_Funcs  *MainLCD = NULL;
uint32_t isInited = 0;
lcm_config_para_t lcm_setting;
bsp_lcd_callback_t bsp_lcd_cb = NULL;
uint32_t isTurnoff = 0;

extern LCD_Funcs LCD_func_ST7789H2_SPI;

volatile bool isUpdate = false;

#ifdef MTK_TE_ENABLE
bool backupTE = false;
volatile bool isTEFailed = false;
#endif

void bsp_lcd_callback(void)
{
#if (defined(MTK_BLE_SUPPORT) || defined(MTK_WIFI_SUPPORT)|| defined(MD_WATCH_SUPPORT))
	hal_gpio_set_output(HAL_GPIO_20 , HAL_GPIO_DATA_LOW);
#else
	hal_gpio_set_output(HAL_GPIO_11 , HAL_GPIO_DATA_LOW);
#endif
    hal_gpio_set_output(HAL_GPIO_19, HAL_GPIO_DATA_HIGH);    
    hal_pinmux_set_function(HAL_GPIO_19, HAL_GPIO_19_SPI_MST1_CS);// Set the pin to be used as CS signal of SPI.
    bsp_lcd_set_layer_to_default();
    bsp_lcd_power_off();
    bsp_lcd_unlock();

    if(bsp_lcd_cb != NULL) {
    bsp_lcd_cb(NULL);}
    isUpdate = false;
}

void bsp_spi_callback (hal_spi_master_callback_event_t event,void *user_data)
{
    bsp_lcd_callback();
}

bsp_lcd_status_t bsp_lcd_init(uint16_t bgcolor)
{
    uint32_t frame_rate, back_porch, front_porch, width, height;
    bsp_lcd_status_t ret = BSP_LCD_STATUS_OK;

    if(isInited){
        return BSP_LCD_STATUS_OK;
}

#ifdef FREERTOS_ENABLE
    if(lcd_sema == NULL) {
        vSemaphoreCreateBinary(lcd_sema);
        if(lcd_sema != NULL) {
            log_hal_info("lcd_sema create complete\n");
        } else {
            log_hal_info("Can't creare lcd_sema create\n");
            return BSP_LCD_STATUS_ERROR;
        }
    }
#endif

    bsp_lcd_power_on();
    bsp_lcd_lock();
    isUpdate = true;

    MainLCD = &LCD_func_ST7789H2_SPI;

    MainLCD->Init_lcd_interface();
    MainLCD->CheckID();
    MainLCD->Init(bgcolor);
    MainLCD->IOCTRL(LCM_IOCTRL_QUERY__FRAME_RATE, &frame_rate);
    MainLCD->IOCTRL(LCM_IOCTRL_QUERY__BACK_PORCH, &back_porch);
    MainLCD->IOCTRL(LCM_IOCTRL_QUERY__FRONT_PORCH, &front_porch);
    MainLCD->IOCTRL(LCM_IOCTRL_QUERY__LCM_WIDTH, &width);
    MainLCD->IOCTRL(LCM_IOCTRL_QUERY__LCM_HEIGHT, &height);

    bsp_lcd_unlock();
    bsp_lcd_power_off();
    isInited = true;
    isUpdate = false;

    hal_spi_master_register_callback(HAL_SPI_MASTER_1, (hal_spi_master_callback_t)bsp_spi_callback, NULL);

    return ret;
}

bsp_lcd_status_t bsp_lcd_set_layer_to_default(void)
{
    bsp_lcd_status_t ret = BSP_LCD_STATUS_OK;

    return ret;
}

bsp_lcd_status_t bsp_lcd_display_on(void)
{
    bsp_lcd_status_t ret = BSP_LCD_STATUS_OK;

    if (0 == isInited)
	{
	    bsp_lcd_init(0x0);
	}

    bsp_lcd_lock();
    bsp_lcd_power_on();
    MainLCD->ExitSleepMode();
    bsp_lcd_power_off();
    isTurnoff = false;
    bsp_lcd_unlock();
	
	log_hal_info("<<=============================bsp_lcd_display_on===============================>>");

    return ret;
	
}

bsp_lcd_status_t bsp_lcd_display_off(void)
{
    bsp_lcd_status_t ret = BSP_LCD_STATUS_OK;

    if (0 == isInited){
        bsp_lcd_init(0x0);}

    bsp_lcd_lock();
    bsp_lcd_power_on();
    MainLCD->EnterSleepMode();
    bsp_lcd_power_off();
    isTurnoff = true;
    bsp_lcd_unlock();
	
	log_hal_info("<<=============================bsp_lcd_display_off===============================>>");

    return ret;
}

bsp_lcd_status_t bsp_lcd_enter_idle(void)
{
    bsp_lcd_status_t ret = BSP_LCD_STATUS_OK;

    if (0 == isInited){
        bsp_lcd_init(0x0);}

    bsp_lcd_lock();
    bsp_lcd_power_on();
    MainLCD->EnterIdleMode();
    bsp_lcd_power_off();

#ifdef MTK_TE_ENABLE
{
    bool enable_TE = false;
    backupTE = lcm_setting.te_enable;
    MainLCD->IOCTRL(LCM_IOCTRL_SET_TE, &enable_TE);
}
#endif
    bsp_lcd_unlock();

    return ret;
}

bsp_lcd_status_t bsp_lcd_exit_idle(void)
{
    bsp_lcd_status_t ret = BSP_LCD_STATUS_OK;

    if (0 == isInited){
        bsp_lcd_init(0x0);}

    bsp_lcd_lock();
    bsp_lcd_power_on();
    MainLCD->ExitIdleMode();
    bsp_lcd_power_off();

#ifdef MTK_TE_ENABLE
{
    MainLCD->IOCTRL(LCM_IOCTRL_SET_TE, &backupTE);
}
#endif
    bsp_lcd_unlock();

    return ret;
}

bsp_lcd_status_t bsp_lcd_update_screen(uint32_t start_x,  uint32_t start_y, uint32_t end_x, uint32_t end_y)
{
    bsp_lcd_status_t ret = BSP_LCD_STATUS_OK;

    if (0 == isInited){
        bsp_lcd_init(0x0);}

    if(isTurnoff)
        return ret;

    bsp_lcd_lock();

    isUpdate = true;

    bsp_lcd_power_on();

    MainLCD->BlockWrite(start_x, start_y, end_x, end_y);

    return ret;
}

bsp_lcd_status_t bsp_lcd_clear_screen(uint16_t color)
{
    bsp_lcd_status_t ret = BSP_LCD_STATUS_OK;

    if (0 == isInited){
        bsp_lcd_init(0x0);}

    return ret;
}

bsp_lcd_status_t bsp_lcd_clear_screen_bw(void)
{
    bsp_lcd_status_t ret = BSP_LCD_STATUS_OK;

    if (0 == isInited){
        bsp_lcd_init(0x0);}

    return ret;
}

bsp_lcd_status_t bsp_lcd_get_parameter(LCM_IOCTRL_ID_ENUM ID, void *parameters)
{
    bsp_lcd_status_t ret = BSP_LCD_STATUS_OK;

    if (0 == isInited){
        bsp_lcd_init(0x0);}

    MainLCD->IOCTRL(ID, parameters);

    return ret;
}

bsp_lcd_status_t bsp_lcd_set_index_color_table(uint32_t *index_table)
{
    bsp_lcd_status_t ret = BSP_LCD_STATUS_OK;

    return ret;
}

bsp_lcd_status_t bsp_lcd_register_callback(bsp_lcd_callback_t bsp_lcd_callback)
{
    bsp_lcd_status_t ret = BSP_LCD_STATUS_OK;

    bsp_lcd_cb = bsp_lcd_callback;

    return ret;
}

bsp_lcd_status_t bsp_lcd_power_on(void)
{
    bsp_lcd_status_t ret = BSP_LCD_STATUS_OK;

    return ret;
}
bsp_lcd_status_t bsp_lcd_power_off(void)
{
    bsp_lcd_status_t ret = BSP_LCD_STATUS_OK;

    return ret;
}

bsp_lcd_status_t bsp_lcd_lock(void)
{
    bsp_lcd_status_t ret = BSP_LCD_STATUS_OK;

#if defined (FREERTOS_ENABLE)
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        if(xSemaphoreTake(lcd_sema, portMAX_DELAY) != pdTRUE) {
            log_hal_info("Take Semaphore isn't failed\n");
            return BSP_LCD_STATUS_ERROR;
        }
    }
#else
    while(isUpdate);
#endif

    return ret;
}

bsp_lcd_status_t bsp_lcd_unlock(void)
{
    bsp_lcd_status_t ret = BSP_LCD_STATUS_OK;

#if defined (FREERTOS_ENABLE)
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
            if(xSemaphoreGiveFromISR(lcd_sema, &xHigherPriorityTaskWoken) != pdTRUE) {
                log_hal_info("Give Semaphore failed\n");
                return BSP_LCD_STATUS_ERROR;
            }
            if(xHigherPriorityTaskWoken){
                portYIELD_FROM_ISR (xHigherPriorityTaskWoken);}
        }
#endif

    return ret;
}

bsp_lcd_status_t bsp_lcd_set_frame_buffer(uint8_t* buffer_addr)
{
    bsp_lcd_status_t ret = BSP_LCD_STATUS_OK;

    MainLCD->IOCTRL(LCM_IOCTRL_SET_FB, buffer_addr);

    return ret;
}
#ifdef __cplusplus
}
#endif
