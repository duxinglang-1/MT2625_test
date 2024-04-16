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

#include <stdio.h>
#include <string.h>
#include "vsm_driver.h"
#include "vsm_signal_reg.h"
#include "vsm_platform_function.h"
#ifdef MT6381_USE_SPI
#include "vsm_spi_operation.h"
#else
#include "vsm_i2c_operation.h"
#endif
#include "ppg_control.h"
#include "assert.h"
#include "hal_i2c_master.h"
#include "vsm_sensor_subsys_adaptor.h"

#if defined(MTK_DEBUG_LEVEL_NONE)
#define LOGE(fmt,arg...)   printf("[vsm_driver]"fmt,##arg)
#define LOGW(fmt,arg...)   printf("[vsm_driver]"fmt,##arg)
#define LOGI(fmt,arg...)   printf("[vsm_driver]"fmt,##arg)
//#define LOGD(fmt,arg...)   printf("[vsm_driver]"fmt,##arg)
#define LOGD
#else
#include "syslog.h"

log_create_module(vsm_driver, PRINT_LEVEL_INFO);

#define LOGE(fmt,arg...)   LOG_E(vsm_driver, fmt,##arg)
#define LOGW(fmt,arg...)   LOG_W(vsm_driver, fmt,##arg)
#define LOGI(fmt,arg...)   LOG_I(vsm_driver, fmt,##arg)
#define LOGD(fmt,arg...)   LOG_D(vsm_driver, fmt, ##arg)
#endif

#define DBG                 0
#define DBG_READ            0
#define DBG_WRITE           0

//two slave addr
#define MT6381_SLAVE_I  0x23
#define MT6381_SLAVE_II 0x33

//sram type addr
#define SRAM_EKG_ADDR   0xC8
#define SRAM_PPG1_ADDR  0xD8
#define SRAM_PPG2_ADDR  0xE8
#define SRAM_BISI_ADDR  0xF8

//read counter addr
#define SRAM_EKG_READ_COUNT_ADDR    0xC0
#define SRAM_PPG1_READ_COUNT_ADDR   0xD0
#define SRAM_PPG2_READ_COUNT_ADDR   0xE0
#define SRAM_BISI_READ_COUNT_ADDR   0xF0

//write counter addr
#define SRAM_EKG_WRITE_COUNT_ADDR   0xCC
#define SRAM_PPG1_WRITE_COUNT_ADDR  0xDC
#define SRAM_PPG2_WRITE_COUNT_ADDR  0xEC
#define SRAM_BISI_WRITE_COUNT_ADDR  0xFC

#define SRAM_COUNTER_RESET_MASK     0x20000000
#define SRAM_COUNTER_OFFSET         29


#define UPDATE_COMMAND_ADDR     0x2328
#define SEC_UPDATE_COMMAND_ADDR 0x2728

#define PPG1_GAIN_ADDR      0x3318
#define PPG1_GAIN_MASK      0x00000007
#define PPG1_GAIN_OFFSET    0

#define PPG2_GAIN_ADDR      PPG1_GAIN_ADDR
#define PPG2_GAIN_MASK      0x00000038
#define PPG2_GAIN_OFFSET    3

#define PPG_AMDAC_ADDR      PPG1_GAIN_ADDR
#define PPG_AMDAC_MASK      0x3C00000
#define PPG_AMDAC_OFFSET    22

#define PPG_AMDAC1_MASK      0x1C00000
#define PPG_AMDAC1_OFFSET    22
#define PPG_AMDAC2_MASK      0xE000000
#define PPG_AMDAC2_OFFSET    25


#define PPG_PGA_GAIN_ADDR      PPG1_GAIN_ADDR
#define PPG_PGA_GAIN_MASK      0x1C0000
#define PPG_PGA_GAIN_OFFSET    18

#define PPG1_CURR_ADDR      0x332C
#define PPG1_CURR_MASK      0x000000FF
#define PPG1_CURR_OFFSET    0
#define PPG2_CURR_ADDR      PPG1_CURR_ADDR
#define PPG2_CURR_MASK      0x0000FF00
#define PPG2_CURR_OFFSET    8

#define LED_TIA_CF_ADDR     PPG1_GAIN_ADDR
#define LED_TIA_CF1_MASK    0x7C0
#define LED_TIA_CF1_OFFSET  6
#define LED_TIA_CF2_MASK    0xF800
#define LED_TIA_CF2_OFFSET  11

#define LED_FULL_SCALE_RANGE_MASK   0xE0
#define LED_FULL_SCALE_RANGE_OFFSET 5

#define CHIP_VERSION_ADDR       0x23AC
#define CHIP_VERSION_E1         0X1
#define CHIP_VERSION_E2         0X2
#define CHIP_VERSION_UNKNOWN    0XFF

#define DIGITAL_START_ADDR 0x3360

#define EKG_SAMPLE_RATE_ADDR1 0x3364
#define EKG_SAMPLE_RATE_ADDR2 0x3310
#define EKG_DEFAULT_SAMPLE_RATE 256

#define PPG_SAMPLE_RATE_ADDR 0x232C
#define PPG_FSYS             1048576
#define PPG_DEFAULT_SAMPLE_RATE 125

#define VSM_BISI_SRAM_LEN   256

#define MAX_WRITE_LENGTH 4
#define DUMMY_EKGPPG_REG 0x23A8

#define BOOST_ON     1
#define BOOST_OFF    0

#define time_after(a,b)  ((long)(b) - (long)(a) < 0)
#define time_before(a,b) time_after(b,a)

// Assign default capabilities setting
int32_t current_signal = 0;
static bool EKG_first_use = true;
uint32_t pre_ppg1_timestamp = 0;
uint32_t pre_ppg2_timestamp = 0;
uint32_t pre_ekg_timestamp = 0;
static uint32_t enable_time;
uint32_t previous_timestamp[VSM_SRAM_PPG2+1];
int64_t numOfData[3] = {0};
static int32_t numOfEKGDataNeedToDrop;
static int32_t vsm_chip_version = -1;
int8_t vsm_init_driver = 0;
int32_t vsm_driver_chip_version_get(void);

bool ppg1_led_status, ppg2_led_status;

static bool inCali = false;

uint32_t agc_ppg1_buf_len, agc_ppg2_buf_len;
uint32_t ppg1_buf2_len, ppg2_buf2_len;
int64_t dc_offset;

#define DEFAULT_PGA6        6002
#define DEFAULT_AMBDAC5_5   21570977
#define CALI_DATA_STABLE_LEN    100
#define CALI_DATA_LEN       200
static int32_t cali_sram_buf[CALI_DATA_LEN];
static int32_t pga6;
static int32_t ambdac5_5;
static uint32_t cali_pga, cali_ambdac_amb, cali_ambdac_led;

static vsm_status_t vsm_driver_idle(void);
vsm_status_t vsm_driver_set_led(vsm_signal_t signal, bool enable);

#define ARRAY_SIZE(arr)    (sizeof(arr) / sizeof((arr)[0]))

void vsm_driver_clk_turn_on(bool on)
{
    LOGD("on:%d, current_signal:%d\n\r", (int)on, (int)current_signal);
    if (current_signal == 0 && on) {
        //turn on 32k
        vsm_platform_gpio_set_pinmux(GPIO_MT6381_32K, 3);   /*Mode3:CLKO0*/
        //For LPHQA
        vsm_platform_gpio_set_output(GPIO_MT6381_RST_PORT_PIN, HAL_GPIO_DATA_LOW);
        ms_delay(5);
        vsm_platform_gpio_set_output(GPIO_MT6381_RST_PORT_PIN, HAL_GPIO_DATA_HIGH);
        ms_delay(5);
        vsm_platform_gpio_set_output(GPIO_MT6381_AFE_PWD_PIN, HAL_GPIO_DATA_LOW);
        ms_delay(45);
    } else if (on == 0){
        current_signal = 0;
        EKG_first_use = true;
        //turn off 32k
        vsm_platform_gpio_set_pinmux(GPIO_MT6381_32K, 0);
        //For LPHQA
        vsm_platform_gpio_set_output(GPIO_MT6381_AFE_PWD_PIN, HAL_GPIO_DATA_HIGH);
    }
}

#ifdef USE_EXTERNAL_BOOST
void vsm_driver_set_boost(int32_t on)
{
    if (on) {
//        vsm_platform_gpio_set_output(GPIO_MT6381_PPG_VDRV_EN, HAL_GPIO_DATA_HIGH);
    } else {
//        vsm_platform_gpio_set_output(GPIO_MT6381_PPG_VDRV_EN, HAL_GPIO_DATA_LOW);
    }
}
#endif

vsm_status_t vsm_driver_read_register(bus_data_t *data)
{
    vsm_status_t ret = VSM_STATUS_ERROR;
    int32_t i = 0;

    if(data == NULL){
        LOGE("NULL data parameter\r\n");
        return VSM_STATUS_INVALID_PARAMETER;
    }

    if (vsm_init_driver == 0) {
        LOGE("[%s]:driver not initialized\r\n", __func__);
        return ret;
    }

#ifdef MT6381_USE_SPI
    ret = vsm_spi_write_read(data->addr, data->reg, data->data_buf, data->length);
    if (ret != VSM_STATUS_OK) {
        /* make sure it's not sram register */
        if (!((data->addr == MT6381_SLAVE_II) &&
              (data->reg == SRAM_EKG_ADDR || data->reg == SRAM_PPG1_ADDR ||
               data->reg == SRAM_PPG2_ADDR || data->reg == SRAM_BISI_ADDR))) {
            for (i = 0; i < 5; i ++)  {
                ret = vsm_spi_write_read_retry(data->addr, data->reg, data->data_buf, data->length);
                if (ret == VSM_STATUS_OK) {
                    break;
                }
                ms_delay(1);
            }
        }
    }
#else
    for (i = 0; i < 10; i ++)  {
        ret = vsm_i2c_write_read(data->addr, data->reg, data->data_buf, data->length);
        if (ret == VSM_STATUS_OK) {
            break;
        }
        ms_delay(2);
    }
#endif

    if (DBG_READ) {
        LOGD("[%s]:addr 0x%x, reg 0x%x, len %d, value 0x%x, ret=%d\r\n",
            __func__, data->addr, data->reg, data->length, *(unsigned int *)data->data_buf, ret);
    }

    if (ret < VSM_STATUS_OK) {
        LOGE("[%s]error (%d), addr 0x%x, reg 0x%x\r\n", __func__, ret, data->addr, data->reg);
    }
    return ret;
}

vsm_status_t vsm_driver_write_register(bus_data_t *data)
{
#ifndef MT6381_USE_SPI
    unsigned char txbuffer[MAX_WRITE_LENGTH * 2];
#endif
    unsigned char reg_addr;
    unsigned char data_len;
    int32_t ret, i = 0;

    if(data == NULL){
        LOGE("NULL data parameter\n");
        return VSM_STATUS_INVALID_PARAMETER;
    } else {
        reg_addr = data->reg;
        data_len = data->length;
    }

#ifdef MT6381_USE_SPI
    for (i = 0; i < 10; i ++)  {
        ret = vsm_spi_write(data->addr, reg_addr, data->data_buf, data_len);
        if (ret == VSM_STATUS_OK) {
            break;
        }
        ms_delay(1);
    }
#else
    txbuffer[0] = reg_addr;
    memcpy(txbuffer + 1, data->data_buf, data_len);
    for (i = 0; i < 10; i ++)  {
        ret = vsm_i2c_write(data->addr, txbuffer, data_len + 1);
        if (ret == VSM_STATUS_OK) {
            break;
        }
        ms_delay(1);
    }
#endif
    if (DBG_WRITE) {
        LOGD("[%s]addr 0x%x, reg 0x%x, value 0x%x, len %d, ret=%d\r\n",
            __func__, data->addr, reg_addr, *(unsigned int *)(data->data_buf), data_len, (int)ret);
    }

    if (ret < 0) {
        LOGE("Bus Trasmit error(%d),addr 0x%x, reg 0x%x, value 0x%x\r\n",
                (int)ret, data->addr, reg_addr, *(unsigned int *)(data->data_buf));
        return VSM_STATUS_ERROR;
    } else {
        return VSM_STATUS_OK;
    }
}

vsm_status_t vsm_driver_write_signal(signal_data_t *reg_addr, int32_t len, uint32_t *enable_data)
{
    bus_data_t data;
    uint32_t data_buf;
    int32_t i = 0;
    vsm_status_t err = VSM_STATUS_OK;

    if (!reg_addr || !enable_data) {
        err = VSM_STATUS_INVALID_PARAMETER;
        return err;
    }

    for (i = 0; i < len; i++) {
        data.addr = ((reg_addr + i)->addr & 0xFF00) >> 8;
        data.reg = ((reg_addr + i)->addr & 0xFF);
        data.length = sizeof(data_buf);
        data.data_buf = (uint8_t *) &data_buf;

        /* process with combo signal */
        data_buf = (reg_addr + i)->value;

        if (inCali && ((reg_addr + i)->addr == 0x3318)) {
            data_buf &= ~(PPG_AMDAC2_MASK | PPG_AMDAC1_MASK | PPG_PGA_GAIN_MASK);
            data_buf |= (cali_ambdac_amb << PPG_AMDAC2_OFFSET);
            data_buf |= (cali_ambdac_led << PPG_AMDAC1_OFFSET);
            data_buf |= (cali_pga << PPG_PGA_GAIN_OFFSET);
            LOGI("0x3318 = %x, %d, %d, %d\n", data_buf,
                    cali_ambdac_amb, cali_ambdac_led, cali_pga);
        } else if (inCali && (cali_ambdac_amb == 0) && ((reg_addr + i)->addr == 0x331C)) {
            data_buf &= ~(0x800);
        } else if (inCali && ((reg_addr + i)->addr == 0x332C)) {
            data_buf = 0;
            LOGI("0x332C = %x\n", data_buf);
        }

        data.length = sizeof(data_buf);
        LOGD("%d:vsm_driver_write, addr:0x%x, reg:0x%x, data 0x%x, length %d \r\n",
             i, data.addr, data.reg, data_buf, data.length);
        err = vsm_driver_write_register(&data);

        if (((reg_addr + i)->addr == 0x3300) && ((reg_addr + i + 1)->addr == 0x3300))
            ms_delay(50);

    }


    return err;
}

vsm_status_t vsm_driver_set_led(vsm_signal_t signal, bool enable)
{
    vsm_status_t err = VSM_STATUS_OK;
    bus_data_t data;
    uint32_t reg_data;

    if (signal == VSM_SIGNAL_PPG1 && ppg1_led_status != enable) {
        ppg1_led_status = enable;
        data.addr = (0x2330 & 0xFF00) >> 8;
        data.reg = (0x2330 & 0xFF);
        if (enable)
            reg_data = 0x00C10182;
        else
            reg_data = 0x0;
        data.length = sizeof(reg_data);
        data.data_buf = (uint8_t *) &reg_data;
        err = vsm_driver_write_register(&data);

        /* update register setting */
        vsm_driver_update_register();
    } else if (signal == VSM_SIGNAL_PPG2 && ppg2_led_status != enable) {
        ppg2_led_status = enable;
        data.addr = (0x2334 & 0xFF00) >> 8;
        data.reg = (0x2334 & 0xFF);
        if (enable)
            reg_data = 0x02430304;
        else
            reg_data = 0x0;
        data.length = sizeof(reg_data);
        data.data_buf = (uint8_t *) &reg_data;
        err = vsm_driver_write_register(&data);

        /* update register setting */
        vsm_driver_update_register();
    }

    return err;
}

vsm_status_t vsm_driver_set_signal(vsm_signal_t signal)
{
    uint32_t enable_data, signal_enable_data = 0;
    uint32_t temp_combo_signal = 0;
    vsm_status_t err = VSM_STATUS_OK;
    int32_t len, i = 0;
    signal_data_t *temp;
    bus_data_t data;

    if (current_signal == 0) {
        LOGI("vsm_driver_set_signal(%x) +\r\n", signal);
        vsm_platform_gpio_set_output(GPIO_MT6381_RST_PORT_PIN, HAL_GPIO_DATA_LOW);
        ms_delay(20);
        vsm_platform_gpio_set_output(GPIO_MT6381_RST_PORT_PIN, HAL_GPIO_DATA_HIGH);
        ms_delay(20);

        len = sizeof(VSM_SIGNAL_INIT_array)/sizeof(VSM_SIGNAL_INIT_array[0]);
        temp = VSM_SIGNAL_INIT_array;
        vsm_driver_write_signal(temp, len, &enable_data);

        enable_time = vsm_platform_get_us_tick();
        previous_timestamp[VSM_SRAM_EKG] = enable_time;
        previous_timestamp[VSM_SRAM_PPG1] = enable_time;
        previous_timestamp[VSM_SRAM_PPG2] = enable_time;

        if (inCali == false) {
            ppg_control_init();
        }

        numOfData[VSM_SRAM_EKG] = 0;
        numOfData[VSM_SRAM_PPG1] = 0;
        numOfData[VSM_SRAM_PPG2] = 0;
        numOfEKGDataNeedToDrop = 2;
        ppg1_buf2_len = 0;
        ppg2_buf2_len = 0;
        agc_ppg1_buf_len = 0;
        agc_ppg2_buf_len = 0;
    }

    /* Turn on PPG1 led for finger on/off detection. */
    /* PPG2 led will be turned on later after finger detected. */
    if ((signal == VSM_SIGNAL_PPG1) || (signal == VSM_SIGNAL_PPG2)) {
        vsm_driver_set_led(VSM_SIGNAL_PPG1, true);
    }

    current_signal |= signal;

    LOGI("current_signal %x\r\n", current_signal);

    return err;
}

static vsm_status_t vsm_driver_set_read_counter(vsm_sram_type_t sram_type, uint32_t *counter)
{
    int err = VSM_STATUS_OK;
    bus_data_t data;

    if (counter == NULL) {
        err = VSM_STATUS_INVALID_PARAMETER;
        return err;
    }

    switch (sram_type) {
    case VSM_SRAM_EKG:
        data.reg = SRAM_EKG_READ_COUNT_ADDR;
        break;

    case VSM_SRAM_PPG1:
        data.reg = SRAM_PPG1_READ_COUNT_ADDR;
        break;

    case VSM_SRAM_PPG2:
        data.reg = SRAM_PPG2_READ_COUNT_ADDR;
        break;

    case VSM_SRAM_BISI:
        data.reg = SRAM_BISI_READ_COUNT_ADDR;
        break;

    case VSM_SRAM_NUMBER:
    default:
        err = VSM_STATUS_INVALID_PARAMETER;
        return err;
    }

    *counter |= 0x60000000;
    data.addr = MT6381_SLAVE_II;
    data.data_buf = (uint8_t *)counter;
    data.length = sizeof(uint32_t);
    err = vsm_driver_write_register(&data);

    if (err != VSM_STATUS_OK)
        LOGE("vsm_driver_set_read_counter fail : %d\n", err);

    return err;
}

static vsm_status_t vsm_driver_get_read_counter(vsm_sram_type_t sram_type, uint32_t *counter)
{
    int err = VSM_STATUS_OK;
    bus_data_t data;

    if (counter == NULL) {
        err = VSM_STATUS_INVALID_PARAMETER;
        return err;
    }

    switch (sram_type) {
    case VSM_SRAM_EKG:
        data.reg = SRAM_EKG_READ_COUNT_ADDR;
        break;

    case VSM_SRAM_PPG1:
        data.reg = SRAM_PPG1_READ_COUNT_ADDR;
        break;

    case VSM_SRAM_PPG2:
        data.reg = SRAM_PPG2_READ_COUNT_ADDR;
        break;

    case VSM_SRAM_BISI:
        data.reg = SRAM_BISI_READ_COUNT_ADDR;
        break;

    case VSM_SRAM_NUMBER:
    default:
        err = VSM_STATUS_INVALID_PARAMETER;
        return err;
    }

    data.addr = MT6381_SLAVE_II;
    data.data_buf = (uint8_t *)counter;
    data.length = sizeof(uint32_t);
    err = vsm_driver_read_register(&data);

    if (err == VSM_STATUS_OK) {
        *counter =
            ((*counter & 0x01ff0000) >> 16) >
            VSM_SRAM_LEN ? 0 : ((*counter & 0x01ff0000) >> 16);
        LOGD("vsm_driver_get_read_counter 0x%x \r\n", *counter);
    }
    return err;
}

vsm_status_t vsm_driver_clear_signal(vsm_signal_t signal)
{
    vsm_status_t err = VSM_STATUS_OK;
    vsm_driver_disable_signal(signal);
    current_signal &= ~(signal);
    if (current_signal == 0) {
        bus_data_t data;
        uint32_t reg_data = 0;
        data.addr = (DIGITAL_START_ADDR & 0xFF00) >> 8;
        data.reg  = (DIGITAL_START_ADDR & 0xFF);
        data.data_buf = (uint8_t *)&reg_data;
        data.length = sizeof(reg_data);
        vsm_driver_write_register(&data);
        #ifndef BIO_TUNNING_TOOL_USE
        vsm_driver_idle();
        #endif
    }
#ifdef USE_EXTERNAL_BOOST
    if (!((current_signal & VSM_SIGNAL_PPG1) || (current_signal & VSM_SIGNAL_PPG2) || 
          (current_signal & VSM_SIGNAL_BISI) || (current_signal & VSM_SIGNAL_PPG1_512HZ))) {
        vsm_driver_set_boost(BOOST_OFF);
    }
#endif
    return err;
}

static vsm_status_t vsm_driver_read_counter(vsm_sram_type_t sram_type, uint32_t *counter)
{
    vsm_status_t err = VSM_STATUS_OK;
    bus_data_t data;
    if (counter == NULL) {
        err = VSM_STATUS_INVALID_PARAMETER;
        return err;
    }

    switch(sram_type){
        case VSM_SRAM_EKG:
            data.reg = SRAM_EKG_READ_COUNT_ADDR;
            break;

        case VSM_SRAM_PPG1:
            data.reg = SRAM_PPG1_READ_COUNT_ADDR;
            break;

        case VSM_SRAM_PPG2:
            data.reg = SRAM_PPG2_READ_COUNT_ADDR;
            break;

        case VSM_SRAM_BISI:
            data.reg = SRAM_BISI_READ_COUNT_ADDR;
            break;

        case VSM_SRAM_NUMBER:
        default:
            err = VSM_STATUS_INVALID_PARAMETER;
            return err;
    }

    data.addr = MT6381_SLAVE_II;
    data.data_buf = (uint8_t *)counter;
    data.length  = sizeof(uint32_t);
    err = vsm_driver_read_register(&data);
    if (err == VSM_STATUS_OK){
        *counter = ((*counter & 0x01ff0000) >> 16) > VSM_SRAM_LEN ? 0 : ((*counter & 0x01ff0000) >> 16);
        LOGD( "vsm_driver_read_counter 0x%lx \r\n", *counter);
    }
    return err;
}

static vsm_status_t vsm_driver_correct_read_counter(vsm_sram_type_t sram_type, uint32_t counter)
{
    vsm_status_t err = VSM_STATUS_OK;
    bus_data_t data;

    switch(sram_type){
        case VSM_SRAM_EKG:
            data.reg = SRAM_EKG_READ_COUNT_ADDR;
            break;

        case VSM_SRAM_PPG1:
            data.reg = SRAM_PPG1_READ_COUNT_ADDR;
            break;

        case VSM_SRAM_PPG2:
            data.reg = SRAM_PPG2_READ_COUNT_ADDR;
            break;

        case VSM_SRAM_BISI:
            data.reg = SRAM_BISI_READ_COUNT_ADDR;
            break;

        case VSM_SRAM_NUMBER:
        default:
            err = VSM_STATUS_INVALID_PARAMETER;
            return err;
    }

    data.addr = MT6381_SLAVE_II;
    data.data_buf = (uint8_t *)&counter;
    data.length  = sizeof(uint32_t);
    counter = (counter & 0x1ff) | 0x60000000;
    err = vsm_driver_write_register(&data);
    LOGD( "[%s]err=%d, 0x%lx \r\n", err, counter);

    return err;
}

static vsm_status_t vsm_driver_write_counter(vsm_sram_type_t sram_type, uint32_t *write_counter)
{
    vsm_status_t err = VSM_STATUS_OK;
    bus_data_t data;
    uint32_t counter = 0;

    if (write_counter == NULL) {
        err = VSM_STATUS_INVALID_PARAMETER;
        return err;
    }

    switch(sram_type){
    case VSM_SRAM_EKG:
        data.reg = SRAM_EKG_WRITE_COUNT_ADDR;
        break;

    case VSM_SRAM_PPG1:
        data.reg = SRAM_PPG1_WRITE_COUNT_ADDR;
        break;

    case VSM_SRAM_PPG2:
        data.reg = SRAM_PPG2_WRITE_COUNT_ADDR;
        break;

    case VSM_SRAM_BISI:
        data.reg = SRAM_BISI_WRITE_COUNT_ADDR;
        break;

    case VSM_SRAM_NUMBER:
    default:
        err = VSM_STATUS_INVALID_PARAMETER;
        return err;
    }

    data.addr = MT6381_SLAVE_II;
    data.data_buf = (uint8_t *)&counter;
    data.length  = sizeof(uint32_t);
    err = vsm_driver_read_register(&data);

    if (err == VSM_STATUS_OK){
        counter = (counter & 0x01ff0000) >> 16;
        LOGD( "vsm_driver_write_counter 0x%lx \r\n", counter);
    }

    *write_counter = counter;

    return err;
}

int32_t vsm_driver_check_sample_rate(vsm_sram_type_t sram_type)
{
    int32_t sample_rate;
    vsm_status_t err = VSM_STATUS_OK;
    bus_data_t data;

    switch(sram_type){
        case VSM_SRAM_EKG:
            {
                int32_t ekg_sample_data1, ekg_sample_data2;
                data.addr = (EKG_SAMPLE_RATE_ADDR1 & 0xFF00) >> 8;
                data.reg  = (EKG_SAMPLE_RATE_ADDR1 & 0xFF);
                data.data_buf = (uint8_t *)&ekg_sample_data1;
                data.length = sizeof(ekg_sample_data1);
                err = vsm_driver_read_register(&data);
                if (err == VSM_STATUS_OK) {
                    data.addr = (EKG_SAMPLE_RATE_ADDR2 & 0xFF00) >> 8;
                    data.reg  = (EKG_SAMPLE_RATE_ADDR2 & 0xFF);
                    data.data_buf = (uint8_t *)&ekg_sample_data2;
                    err = vsm_driver_read_register(&data);
                    LOGD("ekg_sample_data1 0x%x, ekg_sample_data2 0x%x\r\n", ekg_sample_data1, ekg_sample_data2);
                    if (err == VSM_STATUS_OK) {
                        if ((ekg_sample_data1 & 0x7 ) == 0 && ((ekg_sample_data2 & 0x1C0000) >> 18 ) == 0) {
                            sample_rate = 64;
                        } else if ((ekg_sample_data1 & 0x7 ) == 1 && ((ekg_sample_data2 & 0x1C0000) >> 18 ) == 1) {
                            sample_rate = 128;
                        } else if ((ekg_sample_data1 & 0x7 ) == 2 && ((ekg_sample_data2 & 0x1C0000) >> 18 ) == 2) {
                            sample_rate = 256;
                        } else if ((ekg_sample_data1 & 0x7 ) == 3 && ((ekg_sample_data2 & 0x1C0000) >> 18 ) == 3) {
                            sample_rate = 512;
                        } else if ((ekg_sample_data1 & 0x7 ) == 4 && ((ekg_sample_data2 & 0x1C0000) >> 18 ) == 3) {
                            sample_rate = 1024;
                        } else if ((ekg_sample_data1 & 0x7 ) == 5 && ((ekg_sample_data2 & 0x1C0000) >> 18 ) == 3) {
                            sample_rate = 2048;
                        } else if ((ekg_sample_data1 & 0x7 ) == 6 && ((ekg_sample_data2 & 0x1C0000) >> 18 ) == 3) {
                            sample_rate = 4096;
                        } else {
                            sample_rate = EKG_DEFAULT_SAMPLE_RATE;
                        }
                    } else {
                        sample_rate = EKG_DEFAULT_SAMPLE_RATE;
                    }
                } else {
                    sample_rate = EKG_DEFAULT_SAMPLE_RATE;
                }
            }
            break;

        case VSM_SRAM_PPG1:
        case VSM_SRAM_PPG2:
            {
                uint32_t ppg_sample_data;

                data.addr = (PPG_SAMPLE_RATE_ADDR & 0xFF00) >> 8;
                data.reg  = (PPG_SAMPLE_RATE_ADDR & 0xFF);
                data.data_buf = (uint8_t *)&ppg_sample_data;
                data.length = sizeof(ppg_sample_data);
                err = vsm_driver_read_register(&data);
                if (err == VSM_STATUS_OK) {
                    LOGD("ppg_sample_data 0x%x\r\n", ppg_sample_data);
                    sample_rate = PPG_FSYS/((ppg_sample_data & 0x3FFF) + 1);
                } else {
                    LOGE("ppg_sample_data bus error, err %d\r\n", err);
                    sample_rate = PPG_DEFAULT_SAMPLE_RATE;
                }
            }
            break;
        case VSM_SRAM_BISI:
        case VSM_SRAM_NUMBER:
        default:
            return VSM_STATUS_INVALID_PARAMETER;
    }
    return sample_rate;
}

vsm_status_t vsm_driver_check_write_counter(vsm_sram_type_t sram_type, uint32_t read_counter, uint32_t *write_counter)
{
    int i = 0;
    vsm_status_t err = VSM_STATUS_OK;
    uint32_t *pre_timestamp = NULL, cur_timestamp, sample_rate = 0;
    uint32_t expected_counter = 0, real_counter = 0;
    if (write_counter == NULL) {
        return VSM_STATUS_INVALID_PARAMETER;
    }

    switch(sram_type){
        case VSM_SRAM_EKG:
            pre_timestamp = &pre_ekg_timestamp;
            sample_rate = vsm_driver_check_sample_rate(sram_type);
            break;

        case VSM_SRAM_PPG1:
            pre_timestamp = &pre_ppg1_timestamp;
            sample_rate = vsm_driver_check_sample_rate(sram_type);
            break;

        case VSM_SRAM_PPG2:
            pre_timestamp = &pre_ppg2_timestamp;
            sample_rate = vsm_driver_check_sample_rate(sram_type);
            break;

        default:
            err = VSM_STATUS_INVALID_PARAMETER;
            break;
    }
    if (sram_type < VSM_SRAM_BISI) {
        cur_timestamp = vsm_platform_get_us_tick();

        if (*pre_timestamp == 0) {
            err = vsm_driver_write_counter(sram_type, write_counter);
        } else {
            expected_counter = (uint32_t)(((cur_timestamp - *pre_timestamp) * sample_rate)/ 1000000);
            for(i = 0; i < 10; i++) {
                vsm_driver_write_counter(sram_type, write_counter);
                real_counter = (*write_counter >= read_counter)? (*write_counter - read_counter) : (VSM_SRAM_LEN - read_counter + *write_counter);
                if (sram_type == VSM_SRAM_PPG1 || sram_type == VSM_SRAM_PPG2) {
                    real_counter /=2;
                }
                LOGD("sample_rate %lu, real_counter %lu,expected_counter %lu\r\n", sample_rate, real_counter, expected_counter);
                if (real_counter < (20 + expected_counter) &&
                    real_counter > (expected_counter > 20?(expected_counter - 20):0)) {
                    break;
                } else {
                    LOGD("try %d times\r\n", i);
                }
            }
        }
        LOGD("[%s]:cur_timestamp %lu, pre_timestamp %lu\r\n", __func__, cur_timestamp, *pre_timestamp);
        *pre_timestamp = cur_timestamp;
    }

    return err;
}

/* mt6381 spec relevant */
vsm_status_t vsm_driver_read_register_batch(vsm_sram_type_t sram_type, uint8_t *buf, uint16_t length)
{
    vsm_status_t err = VSM_STATUS_OK;
    bus_data_t data;
    uint32_t *data_buf = (uint32_t *)buf;
    int i = 0;

    if ((length > 0) && (buf > 0)) {
        switch(sram_type) {
            case VSM_SRAM_EKG:
                data.reg = SRAM_EKG_ADDR;
                break;

            case VSM_SRAM_PPG1:
                data.reg = SRAM_PPG1_ADDR;
                break;

            case VSM_SRAM_PPG2:
                data.reg = SRAM_PPG2_ADDR;
                break;

            case VSM_SRAM_BISI:
                data.reg = SRAM_BISI_ADDR;
                break;

            case VSM_SRAM_NUMBER:
            default:
                err = VSM_STATUS_INVALID_PARAMETER;
                return err;
        }
        data.addr = MT6381_SLAVE_II;
        data.length = 4;

        for (i = 0; i < length; i++) {
            data.data_buf = (uint8_t *)(data_buf + i);
            err = vsm_driver_read_register(&data);
        }
    } else {
        err = VSM_STATUS_INVALID_PARAMETER;
    }

    return err;
}

vsm_status_t vsm_driver_read_sram(vsm_sram_type_t sram_type, uint32_t *data_buf,
                                  uint32_t data_buf_size, uint32_t *amb_buf, int32_t *len)
{
    vsm_status_t err = VSM_STATUS_OK;
    uint32_t temp;
    uint32_t read_counter = 0;
    uint32_t write_counter = 0;
    uint32_t amb_read_counter = 0;
    uint32_t sram_len;
    uint32_t current_timestamp;
    int64_t rate[3] = {0};
    uint32_t i;
    static uint32_t pre_amb_data;

    if (data_buf == NULL || len == NULL) {
        err = VSM_STATUS_INVALID_PARAMETER;
        return err;
    }

    /* 1. compute how many sram data */
    /* read counter */
    vsm_driver_read_counter(sram_type, &read_counter);
    do {
        temp = read_counter;
        vsm_driver_read_counter(sram_type, &read_counter);
    } while (temp != read_counter);

    /* write counter */
    //vsm_driver_check_write_counter(sram_type, read_counter, &write_counter);
    vsm_driver_write_counter(sram_type, &write_counter);
    do {
        temp = write_counter;
        vsm_driver_write_counter(sram_type, &write_counter);
    } while (temp != write_counter);

    sram_len = (write_counter >= read_counter)
               ? (write_counter - read_counter)
               : (VSM_SRAM_LEN - read_counter + write_counter);

    if ((sram_type == VSM_SRAM_EKG) && (numOfEKGDataNeedToDrop > 0)) {
        /* drop fisrt two garbage bytes of EKG data after enable */
        while (numOfEKGDataNeedToDrop > 0 && sram_len > 0) {
            err = vsm_driver_read_register_batch(sram_type, (uint8_t *) data_buf, 1);
            if (err < 0) {
                err = VSM_STATUS_ERROR;
                *len = sram_len = 0;
            }
            numOfEKGDataNeedToDrop--;
            sram_len--;
        }
        *len = sram_len;
        if (sram_len <= 0)
            return err;
    }

    /* Check maximum output size */
    sram_len = (sram_len > data_buf_size) ? data_buf_size : sram_len;

    sram_len = sram_len - (sram_len % 24);
    *len = sram_len;

    current_timestamp = vsm_platform_get_us_tick();

    /* sram will wraparound after 375000000ns = VSM_SRAM_LEN * (1000000000 / 1024) */
    if (((current_timestamp - previous_timestamp[sram_type]) >= 768000) ||
        (((current_timestamp - previous_timestamp[sram_type]) >= 200000) && sram_len < 100)) {
        LOGE("MT6381! Data dropped!! %d, %d, %d\r\n", sram_type,
             current_timestamp - previous_timestamp[sram_type], sram_len);
             assert(0);
    }

    LOGD("Data read, %d, %d, %d, [%u, %u]\r\n",
         sram_type, current_timestamp - previous_timestamp[sram_type],
         sram_len, read_counter, write_counter);

    previous_timestamp[sram_type] = current_timestamp;

    if (sram_len > 0) {
        if (sram_type == VSM_SRAM_EKG) {
            /* drop fisrt two garbage bytes of EKG data after enable */
            while (numOfEKGDataNeedToDrop > 0 && sram_len > 0) {
                err = vsm_driver_read_register_batch(sram_type, (uint8_t *)data_buf, 1);
                if (err < 0) {
                    err = VSM_STATUS_ERROR;
                    *len = sram_len = 0;
                }
                numOfEKGDataNeedToDrop--;
                sram_len--;
            }

            *len = sram_len;
            if (sram_len <= 0) {
                return err;
            }
        }

        numOfData[sram_type] += sram_len;
        rate[sram_type] = numOfData[sram_type] * 1000 / (current_timestamp - enable_time);

        /* 2. get sram data to data_buf */
        /* get sram data */
        err = vsm_driver_read_register_batch(sram_type, (uint8_t *)data_buf, sram_len);
        if (err < 0) {
            err = VSM_STATUS_ERROR;
            *len = sram_len = 0;
        }

        /* Read out ambient data */
        if (sram_type == VSM_SRAM_PPG2 && amb_buf != NULL) {
            amb_read_counter = read_counter;
            for (i = 0; i < sram_len; i++) {
                /* down sample from 512Hz to 16Hz */
                if (amb_read_counter % 64 == 0) {
                    vsm_driver_set_read_counter(VSM_SRAM_PPG1, &amb_read_counter);
                    vsm_driver_read_register_batch(VSM_SRAM_PPG1, (uint8_t *)&amb_buf[i], 1);
                    pre_amb_data = amb_buf[i];
                    vsm_driver_get_read_counter(VSM_SRAM_PPG1, &read_counter);
                    do {
                        temp = read_counter;
                        vsm_driver_get_read_counter(VSM_SRAM_PPG1, &read_counter);
                    } while (temp != read_counter);
                } else {
                    amb_buf[i] = pre_amb_data;
                }

                amb_read_counter = (amb_read_counter + 1) % VSM_SRAM_LEN;
            }
        }
    }

    return err;
}

vsm_status_t vsm_driver_update_register(void)
{
    uint32_t write_data;
    bus_data_t data;
    vsm_status_t err = VSM_STATUS_OK;

    data.addr = (UPDATE_COMMAND_ADDR & 0xFF00) >> 8;
    data.reg  = (UPDATE_COMMAND_ADDR & 0xFF);
    write_data = (uint32_t)0xFFFF0002;
    data.data_buf = (uint8_t *)&write_data;
    data.length = sizeof(write_data);

    err = vsm_driver_write_register(&data);
    if (err == VSM_STATUS_OK) {
        write_data = 0xFFFF0000;
        err = vsm_driver_write_register(&data);
    }

    return err;
}

vsm_status_t vsm_driver_set_tia_gain(vsm_signal_t ppg_type, vsm_tia_gain_t input)
{
    vsm_status_t err = VSM_STATUS_OK;
    bus_data_t data;
    uint32_t read_data;

    if (ppg_type == VSM_SIGNAL_PPG1 || ppg_type == VSM_SIGNAL_PPG2) {

        switch(ppg_type){
        case VSM_SIGNAL_PPG1:
            data.reg  = (PPG1_GAIN_ADDR & 0xFF);
            data.addr = (PPG1_GAIN_ADDR & 0xFF00) >> 8;
            break;
        case VSM_SIGNAL_PPG2:
            data.reg  = (PPG2_GAIN_ADDR  & 0xFF);
            data.addr = (PPG2_GAIN_ADDR & 0xFF00) >> 8;
            break;
        default:
            break;
        }

        data.data_buf = (uint8_t *)&read_data;
        data.length = sizeof(read_data);
        err = vsm_driver_read_register(&data);

        if (err == VSM_STATUS_OK) {
            if (ppg_type == VSM_SIGNAL_PPG1) {
                read_data &= ~PPG1_GAIN_MASK;
                read_data |= (input & 0x7) << PPG1_GAIN_OFFSET;
            } else if (ppg_type == VSM_SIGNAL_PPG2) {
                read_data &= ~PPG2_GAIN_MASK;
                read_data |= (input & 0x7) << PPG2_GAIN_OFFSET;
            }
            err = vsm_driver_write_register(&data);
            /* update register setting */
            vsm_driver_update_register();
        }
    }else {
        err = VSM_STATUS_INVALID_PARAMETER;
    }

    return err;
}

vsm_status_t vsm_driver_set_tia_cf(vsm_led_type_t led_type, uint32_t input)
{
    vsm_status_t err = VSM_STATUS_OK;
    bus_data_t data;
    uint32_t read_data;

    if (led_type == VSM_LED_1 || led_type == VSM_LED_2) {
        data.reg  = (LED_TIA_CF_ADDR & 0xFF);
        data.addr = (LED_TIA_CF_ADDR & 0xFF00) >> 8;
        data.data_buf = (uint8_t *)&read_data;
        data.length = sizeof(read_data);
        err = vsm_driver_read_register(&data);

        if (err == VSM_STATUS_OK) {
            if (led_type == VSM_LED_1) {
                read_data &= ~LED_TIA_CF1_MASK;
                read_data |= (input & 0x1F) << LED_TIA_CF1_OFFSET;
            } else if (led_type == VSM_LED_2) {
                read_data &= ~LED_TIA_CF2_MASK;
                read_data |= (input & 0x1F) << LED_TIA_CF2_OFFSET;
            }
            err = vsm_driver_write_register(&data);
            /* update register setting */
            vsm_driver_update_register();
        }
    } else {
        err = VSM_STATUS_INVALID_PARAMETER;
    }

    return err;
}

vsm_status_t vsm_driver_set_pga_gain(vsm_pga_gain_t input)
{
    vsm_status_t err = VSM_STATUS_OK;
    bus_data_t data;
    uint32_t read_data;

    data.reg  = (PPG_PGA_GAIN_ADDR & 0xFF);
    data.addr = (PPG_PGA_GAIN_ADDR & 0xFF00) >> 8;
    data.data_buf = (uint8_t *)&read_data;
    data.length = sizeof(read_data);
    err = vsm_driver_read_register(&data);

    if (err == VSM_STATUS_OK) {
        if (input > VSM_PGA_GAIN_6) {
            input = VSM_PGA_GAIN_6;
        }
        read_data &= ~PPG_PGA_GAIN_MASK;
        read_data |= (input & 0x7) << PPG_PGA_GAIN_OFFSET;
        err = vsm_driver_write_register(&data);
        /* update register setting */
        vsm_driver_update_register();
    }

    return err;
}

vsm_status_t vsm_driver_set_led_current(vsm_led_type_t led_type, uint32_t input)
{
    vsm_status_t err = VSM_STATUS_OK;
    bus_data_t data;
    uint32_t read_data;

    if (led_type == VSM_LED_1 || led_type == VSM_LED_2) {
        switch(led_type){
            case VSM_LED_1:
                data.reg  = (PPG1_CURR_ADDR & 0xFF);
                data.addr = (PPG1_CURR_ADDR & 0xFF00) >> 8;
                break;
            case VSM_LED_2:
                data.reg  = (PPG2_CURR_ADDR  & 0xFF);
                data.addr = (PPG2_CURR_ADDR & 0xFF00) >> 8;
                break;
            default:
                break;
        }

        data.data_buf = (uint8_t *)&read_data;
        data.length = sizeof(read_data);
        err = vsm_driver_read_register(&data);

        if (err == VSM_STATUS_OK) {
            if (led_type == VSM_LED_1) {
                read_data &= ~PPG1_CURR_MASK;
                read_data |= (input & 0xFF) << PPG1_CURR_OFFSET;
            } else if (led_type == VSM_LED_2) {
                read_data &= ~PPG2_CURR_MASK;
                read_data |= (input & 0xFF) << PPG2_CURR_OFFSET;
            }
            err = vsm_driver_write_register(&data);
            /* update register setting */
            vsm_driver_update_register();
        }
    } else {
        err = VSM_STATUS_INVALID_PARAMETER;
    }

    return err;
}

vsm_status_t vsm_driver_set_ambdac_current(vsm_ambdac_type_t ambdac_type, vsm_ambdac_current_t current)
{
    vsm_status_t err = VSM_STATUS_OK;
    bus_data_t data;
    uint32_t read_data;

    data.reg  = (PPG_AMDAC_ADDR & 0xFF);
    data.addr = (PPG_AMDAC_ADDR & 0xFF00) >> 8;
    data.data_buf = (uint8_t *)&read_data;
    data.length = sizeof(read_data);
    err = vsm_driver_read_register(&data);

    if (err == VSM_STATUS_OK) {
        if (current > VSM_AMBDAC_CURR_06_MA) {
            current = VSM_AMBDAC_CURR_06_MA;
        }
        if (vsm_driver_chip_version_get() == CHIP_VERSION_E1) {
            read_data &= ~PPG_AMDAC_MASK;
            read_data |= (current & 0xF) << PPG_AMDAC_OFFSET;
            err = vsm_driver_write_register(&data);
        } else if (vsm_driver_chip_version_get() == CHIP_VERSION_E2) {
            if (ambdac_type == VSM_AMBDAC_1) {
                read_data &= ~PPG_AMDAC1_MASK;
                read_data |= (current & 0x7) << PPG_AMDAC1_OFFSET;
                err = vsm_driver_write_register(&data);
            } else if (ambdac_type == VSM_AMBDAC_2) {
                read_data &= ~PPG_AMDAC2_MASK;
                read_data |= (current & 0x7) << PPG_AMDAC2_OFFSET;
                err = vsm_driver_write_register(&data);
            }
        }
        /* update register setting */
        vsm_driver_update_register();
    }
    return err;
}

int32_t vsm_driver_chip_version_get()
{
    int err = VSM_STATUS_OK;
    bus_data_t data;
    uint32_t read_data;

    if (vsm_chip_version == -1) {
        data.reg  = (CHIP_VERSION_ADDR & 0xFF);
        data.addr = (CHIP_VERSION_ADDR & 0xFF00) >> 8;
        data.data_buf = (uint8_t *)&read_data;
        data.length = sizeof(read_data);
        err = vsm_driver_read_register(&data);
        if (err == VSM_STATUS_OK) {
            LOGI("read back chip version: %x\r\n", read_data);
            if (read_data == 0x25110000) {
                vsm_chip_version = CHIP_VERSION_E2;
            } else if(read_data == 0xFFFFFFFF) {
                vsm_chip_version = CHIP_VERSION_E1;
            } else {
                vsm_chip_version = CHIP_VERSION_UNKNOWN;
            }
        } else {
            vsm_chip_version = CHIP_VERSION_UNKNOWN;
        }
    }

    return vsm_chip_version;
}

vsm_status_t vsm_driver_set_led_current_range(uint32_t cur_range)
{
    vsm_status_t err = VSM_STATUS_ERROR;
    uint32_t reg_val = 0;
    bus_data_t data;

    if (cur_range > 0x7) {
        return err;
    } else {
        data.addr = 0x33;
        data.reg = 0x28;
        data.data_buf = (uint8_t *)&reg_val;
        data.length = 4;
        vsm_driver_read_register(&data);
        reg_val &= ~(LED_FULL_SCALE_RANGE_MASK);
        reg_val |= ((cur_range << LED_FULL_SCALE_RANGE_OFFSET) & LED_FULL_SCALE_RANGE_MASK);
        vsm_driver_write_register(&data);
        err = vsm_driver_update_register();
    }

    return err;
}

vsm_status_t vsm_driver_set_register_map(vsm_signal_t signal, int32_t addr, uint32_t reg_data)
{
    vsm_status_t err = VSM_STATUS_ERROR;
    int32_t len = 0, i = 0;
    signal_data_t *temp = NULL;

    //check which signal and length    
    if (signal & VSM_SIGNAL_EKG) {
        len = sizeof(VSM_SIGNAL_EKG_array)/sizeof(VSM_SIGNAL_EKG_array[0]);
        temp = VSM_SIGNAL_EKG_array;
    }

    if (signal & VSM_SIGNAL_EEG) {
        len = sizeof(VSM_SIGNAL_EEG_array)/sizeof(VSM_SIGNAL_EEG_array[0]);
        temp = VSM_SIGNAL_EEG_array;
    }

    if (signal & VSM_SIGNAL_EMG) {
        len = sizeof(VSM_SIGNAL_EMG_array)/sizeof(VSM_SIGNAL_EMG_array[0]);
        temp = VSM_SIGNAL_EMG_array;
    }

    if (signal & VSM_SIGNAL_GSR) {
        len = sizeof(VSM_SIGNAL_GSR_array)/sizeof(VSM_SIGNAL_GSR_array[0]);
        temp = VSM_SIGNAL_GSR_array;
    }

    if (signal & VSM_SIGNAL_PPG1) {
        len = sizeof(VSM_SIGNAL_PPG1_array)/sizeof(VSM_SIGNAL_PPG1_array[0]);
        temp = VSM_SIGNAL_PPG1_array;
    }

    if (signal & VSM_SIGNAL_PPG2) {
        len = sizeof(VSM_SIGNAL_PPG2_array)/sizeof(VSM_SIGNAL_PPG2_array[0]);
        temp = VSM_SIGNAL_PPG2_array;
    }

    if (signal & VSM_SIGNAL_BISI) {
        len = sizeof(VSM_SIGNAL_BISI_array)/sizeof(VSM_SIGNAL_BISI_array[0]);
        temp = VSM_SIGNAL_BISI_array;
    }

    //signal not exist
    if (temp == NULL) {
        err = VSM_STATUS_INVALID_PARAMETER;
    } else {
        // find the register and modify its content
        for (i = 0; i < len; i++) {
            if (temp->addr == addr) {
                temp->value = reg_data;
                err = VSM_STATUS_OK;
                break;
            }
            temp++;
        }
    }

    return err;
}

vsm_status_t vsm_driver_enable_signal(vsm_signal_t signal)
{
    vsm_status_t err = VSM_STATUS_OK;
    bus_data_t data;
    uint32_t enable_data = 0, reg_data;

#ifdef USE_EXTERNAL_BOOST
    if ((signal & VSM_SIGNAL_PPG1) || (signal & VSM_SIGNAL_PPG2) || (signal & VSM_SIGNAL_BISI)||
        (signal & VSM_SIGNAL_PPG1_512HZ)) {
        vsm_driver_set_boost(BOOST_ON);
        ms_delay(2);
    }
#endif

    if (signal & VSM_SIGNAL_EKG) {
        enable_data |= 0x18;
    }
    if (signal & VSM_SIGNAL_PPG1) {
        enable_data |= 0x124;
    }
    if (signal & VSM_SIGNAL_PPG2) {
        enable_data |= 0x144;
    }

    data.addr = (DIGITAL_START_ADDR & 0xFF00) >> 8;
    data.reg  = (DIGITAL_START_ADDR & 0xFF);
    data.data_buf = (uint8_t *)&reg_data;
    data.length = sizeof(reg_data);
    err = vsm_driver_read_register(&data);
    if (err == VSM_STATUS_OK) {
        reg_data |= (enable_data);
        err = vsm_driver_write_register(&data);
    }
    current_signal |= (signal);

    LOGI("current_signal %x\r\n", current_signal);

    return err;
}

vsm_status_t vsm_driver_disable_signal(vsm_signal_t signal)
{
    int32_t len;
    signal_data_t *temp;
    uint32_t enable_data;
    vsm_status_t err = VSM_STATUS_OK;

    current_signal &= ~signal;

    if ((current_signal & VSM_SIGNAL_PPG1) == 0 && (current_signal & VSM_SIGNAL_PPG2) == 0)
        vsm_driver_set_led(VSM_SIGNAL_PPG1, false);
    if (signal == VSM_SIGNAL_PPG2)
        vsm_driver_set_led(signal, false);

    if (current_signal == 0) {
        len = ARRAY_SIZE(VSM_SIGNAL_IDLE_array);
        temp = VSM_SIGNAL_IDLE_array;
        vsm_driver_write_signal(temp, len, &enable_data);
    }

    return err;
}

vsm_status_t vsm_driver_reset_PPG_counter(void)
{
    vsm_status_t err = VSM_STATUS_OK;
    bus_data_t data;
    uint32_t reg_data = 0;
    /* step 1: (disable PPG function and reset PPG1 and PPG2 write counters to 0) */
    data.addr = (DIGITAL_START_ADDR & 0xFF00) >> 8;
    data.reg  = (DIGITAL_START_ADDR & 0xFF);
    data.data_buf = (uint8_t *)&reg_data;
    data.length = sizeof(reg_data);
    err = vsm_driver_write_register(&data);
    if (err == VSM_STATUS_OK) {
        /* step 2: 0x33D0 = 0x2000_0000 (reset PPG1 read counter to 0) */
        data.reg  = SRAM_PPG1_READ_COUNT_ADDR;
        reg_data = 0x60000000;
        err = vsm_driver_write_register(&data);
        if (err == VSM_STATUS_OK) {
            /* step 3: 0x33E0 = 0x2000_0000 (reset PPG2 read counter to 0) */
                data.reg  = SRAM_PPG2_READ_COUNT_ADDR;
                reg_data = 0x60000000;
                err = vsm_driver_write_register(&data);
        }
    }
    return err;
}

vsm_status_t vsm_driver_upgrade_i2c(void)
{
    bus_data_t data;
    uint32_t enable_data = 0;
    vsm_status_t err = VSM_STATUS_ERROR;

    data.addr = (DUMMY_EKGPPG_REG & 0xFF00) >> 8;
    data.reg  = (DUMMY_EKGPPG_REG & 0xFF);
    data.data_buf = (uint8_t *)&enable_data;
    data.length = sizeof(enable_data);
    enable_data |= (uint32_t)(1U<<31);
    err = vsm_driver_write_register(&data);
    return err;
}

static vsm_status_t vsm_driver_idle(void)
{
    bus_data_t data;
    uint32_t enable_data = 0, len = 0;
    int i = 0;
    vsm_status_t err = VSM_STATUS_OK;
    len = sizeof(VSM_SIGNAL_IDLE_array)/sizeof(VSM_SIGNAL_IDLE_array[0]);
    data.data_buf = (uint8_t *)&enable_data;
    data.length = sizeof(enable_data);

    vsm_driver_clk_turn_on(true);

    for (i = 0; i < len; i ++) {
        if (vsm_driver_chip_version_get() == CHIP_VERSION_E1) {
            if ((VSM_SIGNAL_IDLE_array[i].addr == 0x3300 && VSM_SIGNAL_IDLE_array[i].value == 0xA8C71555) ||
                (VSM_SIGNAL_IDLE_array[i].addr == 0x331C && VSM_SIGNAL_IDLE_array[i].value == 0x0048C429)) {
                continue;
            }
            #ifndef USE_EXTERNAL_BOOST
            else if ((VSM_SIGNAL_IDLE_array[i].addr == 0x3334 && VSM_SIGNAL_IDLE_array[i].value == 0x00480000)) {
                VSM_SIGNAL_IDLE_array[i].value = 0x004C0000;
            } else if ((VSM_SIGNAL_IDLE_array[i].addr == 0x3334 && VSM_SIGNAL_IDLE_array[i].value == 0x00480100)) {
                VSM_SIGNAL_IDLE_array[i].value = 0x004C0100;
            }
            #endif
        }
        data.addr = (VSM_SIGNAL_IDLE_array[i].addr & 0xFF00) >> 8;
        data.reg  = (VSM_SIGNAL_IDLE_array[i].addr & 0xFF);
        enable_data = VSM_SIGNAL_IDLE_array[i].value;
        err = vsm_driver_write_register(&data);

        if (err != VSM_STATUS_OK) {
            break;
        }

        #ifndef USE_EXTERNAL_BOOST
        if ((VSM_SIGNAL_IDLE_array[i].addr == 0x3334)){
            if ( (VSM_SIGNAL_IDLE_array[i].value == 0x00480000) || (VSM_SIGNAL_IDLE_array[i].value == 0x004C0000)) {
                ms_delay(45);
            }
        }
        #endif
        if (vsm_driver_chip_version_get() == CHIP_VERSION_E2) {
            if (VSM_SIGNAL_IDLE_array[i].addr == 0x3300 && VSM_SIGNAL_IDLE_array[i].value == 0xA9C71555) {
                ms_delay(50);
            }
        }
    }

    vsm_driver_clk_turn_on(false);

    return err;
}

#define GPIO_PULL_ADDR  0x2314
#define DISABLE_I2C_PD  0x4
void vsm_driver_disable_i2c_pd(void)
{
    bus_data_t data;
    uint32_t enable_data = 0;

    data.addr = (GPIO_PULL_ADDR & 0xFF00) >> 8;
    data.reg  = (GPIO_PULL_ADDR & 0xFF);
    data.data_buf = (uint8_t *)&enable_data;
    data.length = 4;
    vsm_driver_read_register(&data);
    enable_data &= ~(DISABLE_I2C_PD);
    vsm_driver_write_register(&data);
}

vsm_status_t vsm_driver_init(void)
{
    int32_t dir;
    int32_t gpio_data;

    vsm_init_driver++;
    if (vsm_init_driver > 1) {
        return VSM_STATUS_OK;
    } else if (vsm_init_driver <= 0) {
        LOGE("[%s]counter is wrong\r\n", __func__);
        return VSM_STATUS_ERROR;
    }

    LOGI("[%s]++\r\n", __func__);

#ifdef MT6381_USE_SPI
    vsm_spi_init(0, 0);
#else
    i2c_init(HAL_I2C_MASTER_2, HAL_I2C_FREQUENCY_300K);
#endif
    current_signal = 0;
    EKG_first_use = true;
    vsm_platform_gpio_init(GPIO_MT6381_AFE_PWD_PIN);
    vsm_platform_gpio_init(GPIO_MT6381_RST_PORT_PIN);
    vsm_platform_gpio_init(GPIO_MT6381_32K);

    //turn on 32k
    vsm_platform_gpio_set_pinmux(GPIO_MT6381_32K, 3);   /*Mode3:CLKO0*/

    //config power down pin
    vsm_platform_gpio_set_pinmux(GPIO_MT6381_AFE_PWD_PIN, 0);

    vsm_platform_gpio_set_direction(GPIO_MT6381_AFE_PWD_PIN, HAL_GPIO_DIRECTION_OUTPUT);

    if (DBG) {
        vsm_platform_gpio_get_direction(GPIO_MT6381_AFE_PWD_PIN, &dir);
        LOGD("[%s]:GPIO_MT6381_AFE_PWD_PIN=%d dir:%d\r\n", __func__, GPIO_MT6381_AFE_PWD_PIN, dir);
    }

    //config reset pin
    vsm_platform_gpio_set_pinmux(GPIO_MT6381_RST_PORT_PIN, 0);

    vsm_platform_gpio_set_direction(GPIO_MT6381_RST_PORT_PIN, HAL_GPIO_DIRECTION_OUTPUT);

    if (DBG) {
        vsm_platform_gpio_get_direction(GPIO_MT6381_RST_PORT_PIN, &dir);
        LOGD("[%s]:GPIO_MT6381_RST_PORT_PIN dir:%d\r\n", __func__, dir);
    }

    //power down pin, 0:active, 1:inactive
    //high
    vsm_platform_gpio_set_output(GPIO_MT6381_AFE_PWD_PIN, HAL_GPIO_DATA_HIGH);

    if (DBG) {
        vsm_platform_gpio_get_output(GPIO_MT6381_AFE_PWD_PIN, &gpio_data);
        LOGD("[%s]:GPIO_MT6381_AFE_PWD_PIN data:%d\r\n", __func__, gpio_data);
    }
    ms_delay(5);

    //reset pin, 1:active, 0:inactive
    //high
    vsm_platform_gpio_set_output(GPIO_MT6381_RST_PORT_PIN, HAL_GPIO_DATA_HIGH);

    if (DBG) {
        vsm_platform_gpio_get_output(GPIO_MT6381_RST_PORT_PIN, &gpio_data);
        LOGD("[%s]:GPIO_MT6381_RST_PORT_PIN data:%d\r\n", __func__, gpio_data);
    }
    ms_delay(15);

    //low
    vsm_platform_gpio_set_output(GPIO_MT6381_RST_PORT_PIN, HAL_GPIO_DATA_LOW);

    if (DBG) {
        vsm_platform_gpio_get_output(GPIO_MT6381_RST_PORT_PIN, &gpio_data);
        LOGD("[%s]:GPIO_MT6381_RST_PORT_PIN data:%d\r\n", __func__, gpio_data);
    } 
    //delay 5ms
    ms_delay(5);

    //high
    vsm_platform_gpio_set_output(GPIO_MT6381_RST_PORT_PIN, HAL_GPIO_DATA_HIGH);

    if (DBG) {
        vsm_platform_gpio_get_output(GPIO_MT6381_RST_PORT_PIN, &gpio_data);
        LOGD("[%s]:GPIO_MT6381_RST_PORT_PIN data:%d\r\n", __func__, gpio_data);
    }

    ms_delay(5);

    //low
    vsm_platform_gpio_set_output(GPIO_MT6381_AFE_PWD_PIN, HAL_GPIO_DATA_LOW);

    if (DBG) {
        vsm_platform_gpio_get_output(GPIO_MT6381_AFE_PWD_PIN, &gpio_data);
        LOGD("[%s]:GPIO_MT6381_AFE_PWD_PIN data:%d\r\n", __func__, gpio_data);
    }
    ms_delay(45);
    //*****************************************************************************    
    vsm_driver_chip_version_get();

    ppg1_led_status = false;
    ppg2_led_status = false;

    inCali = false;

    MT6381_ResetCalibration(); // Set dc_offset to default value

    LOGI("[%s]--\r\n", __func__);

    return VSM_STATUS_OK;
}

vsm_status_t vsm_driver_deinit(void)
{
    vsm_init_driver--;
    if (vsm_init_driver > 0) {
        return VSM_STATUS_OK;
    } else if (vsm_init_driver < 0) {
        LOGE("[%s]counter is wrong\r\n", __func__);
        return VSM_STATUS_ERROR;
    }

    LOGI("[%s]++\r\n", __func__);

    //power pin, 0:active, 1:inactive
    //high
    vsm_platform_gpio_set_output(GPIO_MT6381_AFE_PWD_PIN, HAL_GPIO_DATA_HIGH);
    ms_delay(25);

    //reset pin, 1:active, 0:inactive
    //high
    vsm_platform_gpio_set_output(GPIO_MT6381_RST_PORT_PIN, HAL_GPIO_DATA_LOW);
    vsm_platform_gpio_set_output(GPIO_MT6381_AFE_PWD_PIN, HAL_GPIO_DATA_LOW);

    vsm_platform_gpio_deinit(GPIO_MT6381_32K);
    vsm_platform_gpio_deinit(GPIO_MT6381_RST_PORT_PIN);
    vsm_platform_gpio_deinit(GPIO_MT6381_AFE_PWD_PIN);

#ifdef MT6381_USE_SPI
    vsm_spi_deinit();
#else
    i2c_deinit();
#endif

    LOGI("[%s]--\r\n", __func__);

    return VSM_STATUS_OK;
}

static int32_t MT6381_CaliReadSram(vsm_sram_type_t sram_type, int32_t *data_buf, uint32_t len)
{
    uint32_t temp;
    uint32_t read_counter = 0;
    uint32_t write_counter = 0;
    uint32_t sram_len;
    int err = VSM_STATUS_OK;

    /* read counter */
    vsm_driver_get_read_counter(sram_type, &read_counter);
    do {
        temp = read_counter;
        vsm_driver_get_read_counter(sram_type, &read_counter);
    } while (temp != read_counter);

    /* write counter */
    /* vsm_driver_check_write_counter(sram_type, read_counter, &write_counter); */
    vsm_driver_write_counter(sram_type, &write_counter);
    do {
        temp = write_counter;
        vsm_driver_write_counter(sram_type, &write_counter);
    } while (temp != write_counter);

    sram_len =
        (write_counter >=
         read_counter) ? (write_counter - read_counter) : (VSM_SRAM_LEN - read_counter +
                                   write_counter);
    sram_len = ((sram_len % 2) == 0) ? sram_len : sram_len - 1;
    sram_len = (sram_len > len) ? len : sram_len;

    if (sram_len > 0) {
        /* get sram data */
        err = vsm_driver_read_register_batch(sram_type, (uint8_t *) data_buf, sram_len);
        if (err < 0)
            LOGE("vsm_driver_read_register_batch fail, %d\r\n", err);
    }

    return sram_len;
}

static int32_t MT6381_GetCalibrationData(int32_t *cali_sram1_buf, int32_t *cali_sram2_buf, uint16_t pga,
    uint16_t ambdac_amb, uint16_t ambdac_led, uint32_t len)
{
    uint32_t sram1_len = len;
    uint32_t sram2_len = len;
    uint32_t read_len = 0;

    inCali = true;

    cali_pga = pga;
    cali_ambdac_amb = ambdac_amb;
    cali_ambdac_led = ambdac_led;

    vsm_driver_set_signal(VSM_SIGNAL_PPG1);
    vsm_driver_set_signal(VSM_SIGNAL_PPG2);

    while ((cali_sram1_buf != NULL && sram1_len != 0) || (cali_sram2_buf != NULL && sram2_len != 0)) {
        if (cali_sram1_buf != NULL && sram1_len != 0) {
            read_len = MT6381_CaliReadSram(VSM_SRAM_PPG1, &cali_sram1_buf[len - sram1_len], sram1_len);
            if (read_len > sram1_len) {
                LOGE("Wrong len, sram1_len = %d, read_len = %d\r\n", sram1_len, read_len);
                return -1;
            }
            sram1_len -= read_len;
        }
        if (cali_sram2_buf != NULL && sram2_len != 0) {
            read_len = MT6381_CaliReadSram(VSM_SRAM_PPG2, &cali_sram2_buf[len - sram2_len], sram2_len);
            if (read_len > sram2_len) {
                LOGE("Wrong len, sram2_len = %d, read_len = %d\r\n", sram2_len, read_len);
                return -1;
            }
            sram2_len -= read_len;
        }
    }

    inCali = false;
    vsm_driver_disable_signal(VSM_SIGNAL_PPG1);
    vsm_driver_disable_signal(VSM_SIGNAL_PPG2);

    return 0;
}

vsm_status_t MT6381_DoCalibration(biometric_cali *cali)
{
    int32_t i;
    int32_t a = 0;
    int32_t b = 0;
    int32_t c = 0;
    int32_t count = 0;
    int64_t sumOfambdac = 0;

    vsm_driver_init();

    /* 1. set PGA=1, AMBDAC(AMB phase) = 0 to get AMB data A */
    MT6381_GetCalibrationData(cali_sram_buf, NULL, 0, 0, 7, CALI_DATA_LEN);
    for (i = CALI_DATA_STABLE_LEN; i < CALI_DATA_LEN; i += 2) {
        cali_sram_buf[i] = cali_sram_buf[i] >= 0x400000 ? cali_sram_buf[i] - 0x800000 : cali_sram_buf[i];
        a += cali_sram_buf[i];
        LOGE("[%d]a=%d\n", (i-CALI_DATA_STABLE_LEN+1), a);
    }
    /* 2. set PGA=1, AMBDAC(AMB phase) = 1 to get AMB data B */
    MT6381_GetCalibrationData(cali_sram_buf, NULL, 0, 1, 7, CALI_DATA_LEN);
    for (i = CALI_DATA_STABLE_LEN; i < CALI_DATA_LEN; i += 2) {
        cali_sram_buf[i] = cali_sram_buf[i] >= 0x400000 ? cali_sram_buf[i] - 0x800000 : cali_sram_buf[i];
        b += cali_sram_buf[i];
        LOGE("[%d]b=%d\n", (i-CALI_DATA_STABLE_LEN+1), b);
    }
    /* 3. set PGA=6, AMBDAC(AMB phase) = 1 to get AMB data C */
    MT6381_GetCalibrationData(cali_sram_buf, NULL, 6, 1, 7, CALI_DATA_LEN);
    for (i = CALI_DATA_STABLE_LEN; i < CALI_DATA_LEN; i += 2) {
        cali_sram_buf[i] = cali_sram_buf[i] >= 0x400000 ? cali_sram_buf[i] - 0x800000 : cali_sram_buf[i];
        c += cali_sram_buf[i];
        LOGE("[%d]c=%d\n", (i-CALI_DATA_STABLE_LEN+1), c);
    }
    /* 4. PGA6 = (C - A) / ( B - A) */
    /* 5. set PGA=1, AMBDAC(AMB phase) = 1, AMBDAC(LED phase) = 7 */
    MT6381_GetCalibrationData(NULL, cali_sram_buf, 0, 1, 7, CALI_DATA_LEN);
    for (i = CALI_DATA_STABLE_LEN; i < CALI_DATA_LEN; i += 2) {
        cali_sram_buf[i] = cali_sram_buf[i] >= 0x400000 ? cali_sram_buf[i] - 0x800000 : cali_sram_buf[i];
        sumOfambdac += cali_sram_buf[i];
        count++;
    }

    vsm_driver_deinit();

    cali->pga6 = (c - a) * 1000 / (b - a);
    cali->ambdac5_5 = sumOfambdac * -1000 / count;

    LOGE("a=%d, b=%d, c=%d\n", a, b, c);
    if (cali->pga6 < 6000 || cali->pga6 > 6060) {
        LOGE("pga fail, %d is not between 6000~6060.\r\n", cali->pga6);
        return VSM_STATUS_ERROR;
    }
    if (cali->ambdac5_5 < 20957234 || cali->ambdac5_5 > 22296629) {
        LOGE("ambdac fail, %d is not between 20957234~22296629.\r\n", cali->ambdac5_5);
        return VSM_STATUS_ERROR;
    }

    LOGI("SPO2 calibration finished: pga = %d, ambdac = %d%d\n", cali->pga6, cali->ambdac5_5);

    return VSM_STATUS_OK;
}

vsm_status_t MT6381_WriteCalibration(biometric_cali *cali)
{
    pga6 = cali->pga6;
    ambdac5_5 = cali->ambdac5_5;
    dc_offset = (int64_t)pga6 * (int64_t)ambdac5_5;
    dc_offset += 500000;
    dc_offset = dc_offset / 1000000; // do_div(dc_offset, 1000000);
    LOGI("pga6 = %d, ambdac5_5 = %d, dc_offset = %d\r\n", pga6, ambdac5_5, dc_offset); // pga6 = 6002, ambdac5_5 = 21570977, dc_offset = 129469

    return VSM_STATUS_OK;
}

vsm_status_t MT6381_ResetCalibration(void)
{
    biometric_cali data;

    data.pga6 = DEFAULT_PGA6;
    data.ambdac5_5 = DEFAULT_AMBDAC5_5;
    MT6381_WriteCalibration(&data);

    return VSM_STATUS_OK;
}

vsm_status_t MT6381_ReadCalibration(biometric_cali *cali)
{
    cali->pga6 = pga6;
    cali->ambdac5_5 = ambdac5_5;
    LOGI("pga6 = %d, ambdac5_5 = %d\r\n", pga6, ambdac5_5);
    return VSM_STATUS_OK;
}


