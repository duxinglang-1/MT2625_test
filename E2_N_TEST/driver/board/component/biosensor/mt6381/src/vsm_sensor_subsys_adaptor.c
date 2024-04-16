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
#include "FreeRTOS.h"
#include "task.h"
#include "vsm_driver.h"
#include "vsm_sensor_subsys_adaptor.h"
#include "sensor_alg_interface.h"
#include "algo_adaptor/algo_adaptor.h"
#include "sensor_manager.h"
#include "hal_eint.h"
#include "nvdm.h"

#define BIO_MAX_BUF_CNT 128
//#define ENABLE_DISABLE_TEST

log_create_module(vsm_sensor, PRINT_LEVEL_INFO);

#define LOGE(fmt,arg...)   LOG_E(vsm_sensor, fmt,##arg)
#define LOGW(fmt,arg...)   LOG_W(vsm_sensor, fmt,##arg)
#define LOGI(fmt,arg...)   LOG_I(vsm_sensor, fmt,##arg)
#define LOGD(fmt,arg...)   LOG_D(vsm_sensor, fmt, ##arg)

#define PPG_SAMPLE_PERIOD_MS    8
#define EKG_SAMPLE_PERIOD_MS    2
#define SENSOR_SPP_DATA_MAGIC       54321
#define SENSOR_SPP_DATA_RESERVED    12345
#define INTR_CTRL_ADDR      0x334C
#define INTR_STATUS_ADDR    0x3354
#define EKG_IRQ_STATUS_MASK    0xE
#define PPG1_IRQ_STATUS_MASK   0x70
#define PPG2_IRQ_STATUS_MASK   0x380
#define BISI_IRQ_STATUS_MASK   0x1C00

//#define DUMP_PPG_REG

uint32_t sram_data[VSM_SRAM_LEN];
uint8_t sram_type = VSM_SRAM_PPG1;

static uint8_t vsm_eint_num;
cmd_event_t vsm_event;
#ifdef ENABLE_DISABLE_TEST
bool ppg_first_use = true;
#endif

#if  defined ( __GNUC__ )
  #ifndef __weak
    #define __weak   __attribute__((weak))
  #endif /* __weak */
#endif /* __GNUC__ */

#include "ppg_control.h"
/* threshold = 6050 */
/* code = 6050 * (2 ^ 16) / 3200 - 129583 = -5679 */
/* 129583 is ambdac offset code, measured and provided by DE */
#define PPG_THRESHOLD (-5679)

/* used to store the dispatched ppg data */
uint32_t ppg2_buf2[VSM_SRAM_LEN];
int32_t ppg1_buf2[VSM_SRAM_LEN];
uint32_t amb_temp_buf[VSM_SRAM_LEN];

uint32_t ppg1_agc_buf[VSM_SRAM_LEN], ppg2_agc_buf[VSM_SRAM_LEN];
uint32_t ppg1_amb_buf[VSM_SRAM_LEN], ppg2_amb_buf[VSM_SRAM_LEN];
uint32_t agc_ppg1_buf[VSM_SRAM_LEN/32], agc_ppg1_amb_buf[VSM_SRAM_LEN/32];
uint32_t agc_ppg2_buf[VSM_SRAM_LEN/32], agc_ppg2_amb_buf[VSM_SRAM_LEN/32];

extern int64_t numOfData[];
extern uint32_t agc_ppg1_buf_len, agc_ppg2_buf_len;
extern uint32_t ppg1_buf2_len, ppg2_buf2_len;
extern int32_t current_signal;
extern uint32_t dc_offset;

extern int8_t vsm_init_driver;

#define ARRAY_SIZE(arr)    (sizeof(arr) / sizeof((arr)[0]))

#ifdef DUMP_PPG_REG
uint16_t ppg_addr[] = {0x2308, 0x230C, 0x2324, 0x3344, 0x3348, 0x232C, 0x2330, 0x2334, 0x2338, 0x233c,
                       0x2340, 0x2344, 0x2348, 0x234C, 0x2350, 0x2354, 0x2358, 0x235C, 0x2360, 0x2364,
                       0x2368, 0x236C, 0x2370, 0x3368, 0x334C, 0x3300, 0x3318, 0x3320, 0x3324, 0x3328,
                       0x332C, 0x3334, 0x33DC, 0x33D0, 0x3360};

static void vsm_dump_ppg_register_setting(void)
{
    bus_data_t data;
    uint32_t read_data, i = 0;
    int len = sizeof(ppg_addr)/sizeof(ppg_addr[0]);
    data.data_buf = (uint8_t *)&read_data;
    data.length = sizeof(read_data);
    for (i = 0;i < len; i ++) {
        read_data = 0;
        data.reg  = (ppg_addr[i] & 0xFF);
        data.addr = (ppg_addr[i] & 0xFF00) >> 8;
        vsm_driver_read_register(&data);
        printf("0x%x,0x%x\r\n", ppg_addr[i], read_data);
    }
}
#endif

int32_t time_offset_compute(uint32_t current_tick, uint32_t pre_tick, int number_of_data)
{
    return (current_tick  - pre_tick)/(number_of_data + 1);
}

void vsm_data_drop(vsm_sram_type_t sram_type)
{
    int32_t output_len = 0, i = 5;

    // vsm_driver_read_sram(sram_type, (uint32_t *)sram_data, &output_len);
    LOGD("%s():output_len %d\r\n", __func__, output_len);
    while(output_len !=0 && i != 0){
        output_len = 0;
        // vsm_driver_read_sram(sram_type, (uint32_t *)sram_data, &output_len);
        i--;
        LOGD("%s():output_len %d\r\n", __func__, output_len);
    }
    LOGD("%s():--\r\n", __func__);
}

void vsm_revert_ppg_data(uint32_t *data_buf, int32_t len)
{
    int32_t i = 0;
    uint32_t temp = 0;
    for (i = 0; i < len; i ++){
        if (i % 2 == 1) {
            temp = *(data_buf + i - 1);
            *(data_buf + i - 1) = *(data_buf + i);
            *(data_buf + i) = temp;
        }
    }

}

//sensor manager operation call
int32_t vsm_ppg1_operate(void *self, uint32_t command, void *buff_out, int32_t size_out,
                         int32_t *actualout, void *buff_in, int32_t size_in)
{
    int err = 0;
    int32_t value = 0;

    switch (command) {
        case SENSOR_DELAY:
            if ((buff_in == NULL) || (size_in < sizeof(int32_t))) {
                LOGE("BIO PPG1 SENSOR_DELAY sensor parameter error!\n");
                err = -1;
            } else {
                value = *(int32_t *)buff_in;
                LOGI("BIO PPG1 SENSOR_DELAY ms (%d) \r\n", value);
            }
            break;

        case SENSOR_ENABLE:
            if ((buff_in == NULL) || (size_in < sizeof(int))) {
                LOGE("BIO PPG1 Enable sensor parameter error!\n");
                err = -1;
            } else {
                value = *(int32_t *)buff_in;
                LOGI("BIO PPG1 SENSOR_ENABLE (%d) \r\n", value);

                if (value) {
                    err = vsm_driver_init();
                    if (err != VSM_STATUS_OK) {
                        return err;
                    }

                    // spo2 calibration
                    sensio_nvdm_hm_info_struct hm_info;
                    biometric_cali cali;
                    nvdm_status_t status;
                    uint32_t size;

                    size = sizeof(hm_info);
                    status = nvdm_read_data_item(SENSIO_NVDM_GROUP_NAME, SENSIO_NVDM_HM_INFO_NAME, &hm_info, &size);
                    if (status == NVDM_STATUS_OK) {
                        cali.pga6 = hm_info.analogGain;        /**< Analog Gain.*/
                        cali.ambdac5_5 = hm_info.analogOffset; /**< Analog Offset.*/
                        LOGI("hm_info.analogGain(pga6) = %d, hm_info.analogOffset(ambdac5_5) = %d", hm_info.analogGain, hm_info.analogOffset);
                        if ((hm_info.analogGain > 0) && (hm_info.analogOffset > 0)) {
                            MT6381_WriteCalibration(&cali);
                        }
                    }

                    vsm_driver_set_signal(VSM_SIGNAL_PPG1);
                } else {
                    vsm_driver_disable_signal(VSM_SIGNAL_PPG1); // vsm_driver_clear_signal(VSM_SIGNAL_PPG1);
                    err = vsm_driver_deinit();
                    if (err != VSM_STATUS_OK) {
                        return err;
                    }
                }
            }
            break;

        case SENSOR_GET_DATA: {
            int32_t output_len = 0, i = 0;
            int32_t time_offset;
            #ifdef DUMP_PPG_REG
            static int32_t ppg1_print_cnt = 0;
            #endif
            int32_t ppg_control_fs = 16;            /* The sampling frequency of PPG input (Support 125Hz only). */
            int32_t ppg_control_cfg = 1;             /* The input configuration, default value is 1. */
            int32_t ppg_control_src = 1;             /* The input source, default value is 1 (PPG channel 1). */
            ppg_control_t ppg1_control_input;           /* Input structure for the #ppg_control_process(). */
            int64_t numOfSRAMPPG2Data;
            uint32_t leddrv_con1;

            if (vsm_init_driver == 0) {
                if (actualout) {
                    *actualout = 0;
                }
                err = -1;
                return err;
            }

            numOfSRAMPPG2Data = numOfData[VSM_SRAM_PPG2];

            if (current_signal & VSM_SIGNAL_PPG1) {
                int err = VSM_STATUS_OK;
                bus_data_t data;
                uint32_t read_data;
                data.addr = 0x33;
                data.reg  = 0x2C;
                data.data_buf = (uint8_t *)&leddrv_con1;
                data.length = sizeof(leddrv_con1);
                err = vsm_driver_read_register(&data);
                LOGD("leddrv_con1 = %d\r\n", leddrv_con1);

                /* PPG1 data is stored in sram ppg2 as well */
                vsm_driver_read_sram(VSM_SRAM_PPG2, sram_data, size_out,
                                     amb_temp_buf, &output_len);

                for (i = 0; i < output_len; i += 2) {
                    sram_data[i] = sram_data[i] >= 0x400000 ? sram_data[i] - 0x800000 : sram_data[i];
                    sram_data[i + 1] = sram_data[i + 1] >= 0x400000 ? sram_data[i + 1] - 0x800000 : sram_data[i + 1];
                    amb_temp_buf[i] = amb_temp_buf[i] >= 0x400000 ? amb_temp_buf[i] - 0x800000 : amb_temp_buf[i];

                    if (ppg1_buf2_len < VSM_SRAM_LEN) {
                        ppg1_agc_buf[ppg1_buf2_len] = leddrv_con1;
                        ppg1_amb_buf[ppg1_buf2_len] = amb_temp_buf[i];
                        ppg1_buf2[ppg1_buf2_len++] = sram_data[i];
                    }

                    if (ppg2_buf2_len < VSM_SRAM_LEN) {
                        ppg2_agc_buf[ppg2_buf2_len] = leddrv_con1;
                        ppg2_amb_buf[ppg2_buf2_len] = amb_temp_buf[i];
                        ppg2_buf2[ppg2_buf2_len++] = sram_data[i + 1];
                    }
                    /* downsample to 16hz for AGC */
                    if ((numOfSRAMPPG2Data + i) % 64 == 0) {
                        if (agc_ppg1_buf_len < ARRAY_SIZE(agc_ppg1_buf)) {
                            agc_ppg1_buf[agc_ppg1_buf_len] = sram_data[i];
                            agc_ppg1_amb_buf[agc_ppg1_buf_len++] = amb_temp_buf[i];
                        }
                        if (agc_ppg2_buf_len < ARRAY_SIZE(agc_ppg2_buf)) {
                            agc_ppg2_buf[agc_ppg2_buf_len] = sram_data[i + 1];
                            agc_ppg2_amb_buf[agc_ppg2_buf_len++] = amb_temp_buf[i]; /* use ppg1 amb data */
                        }
                    }
                }

                if ((current_signal & VSM_SIGNAL_PPG2) && output_len > 0) {
                    if ((int)sram_data[i - 2] < PPG_THRESHOLD) {
                        vsm_driver_set_led(VSM_SIGNAL_PPG2, false);
                } else {
                        vsm_driver_set_led(VSM_SIGNAL_PPG2, true);
                    }
                }

                LOGD("BIO PPG1 output (%d)\r\n", output_len);
            } else {
                ppg1_buf2_len = 0;
            }

            if (ppg1_buf2_len == VSM_SRAM_LEN) {
                LOGE("PPG1 data dropped\r\n");
            } else if (ppg1_buf2_len > size_out) {
                LOGE("ppg1_buf2_len=%d over %d\r\n", ppg1_buf2_len, size_out);
            } else if (ppg1_buf2_len > 0) {
                sensor_data_unit_t *start = (sensor_data_unit_t *)buff_out;

                for (i = 0; i < ppg1_buf2_len; i++) {
                    start->sensor_type = SENSOR_TYPE_BIOSENSOR_PPG1;
                    start->time_stamp = 0; // sensor alorithm and sensor app don't need timestamp
                    start->bio_data.data = ppg1_buf2[i] + dc_offset;
                    start->bio_data.amb_data = ppg1_amb_buf[i];
                    start->bio_data.agc_data = ppg1_agc_buf[i];
                    /* Only report finger on/off status by IR PPG */
                    if (ppg1_buf2[i] < PPG_THRESHOLD) {
                        start->bio_data.status = SENSOR_STATUS_UNRELIABLE;
                    }
                    else {
                        start->bio_data.status = SENSOR_STATUS_ACCURACY_HIGH;
                    }
                    start++;
                }
            }

            LOGD("ppg1_buf2[0] = (%x, %d), ppg1_amb_buf[0] = (%x, %d), ppg1_agc_buf[0] = %x\r\n",
                 ppg1_buf2[0], ppg1_buf2[0], ppg1_amb_buf[0], ppg1_amb_buf[0], ppg1_agc_buf[0]);

            *actualout = ppg1_buf2_len;
            ppg1_buf2_len = 0;

            ppg1_control_input.input = agc_ppg1_buf;
            ppg1_control_input.input_amb = agc_ppg1_amb_buf;
            ppg1_control_input.input_fs = ppg_control_fs;
            ppg1_control_input.input_length = agc_ppg1_buf_len;
            ppg1_control_input.input_config = ppg_control_cfg;
            ppg1_control_input.input_source = ppg_control_src;
            ppg_control_process(&ppg1_control_input, 0, NULL);
            agc_ppg1_buf_len = 0;

        }
        break;

        case SENSOR_CUST:
            if ((buff_in == NULL) || (size_in < sizeof(int32_t))) {
                LOGE("PPG1 SENSOR_CUST parameter error!\n");
                err = -1;
            } else {
                value = *(int32_t *)buff_in;
                LOGI("PPG1 SENSOR_CUST (%d) \r\n", value);
                //ToDo: Customization setting
            }
            break;
        default:
            LOGE("operate function no this parameter %d! \r\n", command);
            err = -1;
            break;
    }

    return err;
}

int32_t vsm_ppg2_operate(void *self, uint32_t command, void *buff_out, int32_t size_out,
                         int32_t *actualout, void *buff_in, int32_t size_in)
{
    int err = 0;
    int32_t value = 0;

    switch (command) {
        case SENSOR_DELAY:
            if ((buff_in == NULL) || (size_in < sizeof(int32_t))) {
                LOGE("BIO PPG2 SENSOR_DELAY sensor parameter error!\n");
                err = -1;
            } else {
                value = *(int32_t *)buff_in;
                LOGI("BIO PPG2 SENSOR_DELAY ms (%d) \r\n", value);
            }
            break;

        case SENSOR_ENABLE:
            if ((buff_in == NULL) || (size_in < sizeof(int))) {
                LOGE("BIO PPG2 Enable sensor parameter error!\n");
                err = -1;
            } else {
                value = *(int32_t *)buff_in;
                LOGI("BIO PPG2 SENSOR_ENABLE (%d) \r\n", value);

                if (value) {
                    // spo2 calibration
                    sensio_nvdm_hm_info_struct hm_info;
                    biometric_cali cali;
                    nvdm_status_t status;
                    uint32_t size;

                    err = vsm_driver_init();
                    if (err != VSM_STATUS_OK) {
                        return err;
                    }

                    size = sizeof(hm_info);
                    status = nvdm_read_data_item(SENSIO_NVDM_GROUP_NAME, SENSIO_NVDM_HM_INFO_NAME, &hm_info, &size);
                    if (status == NVDM_STATUS_OK) {
                        cali.pga6 = hm_info.analogGain;        /**< Analog Gain.*/
                        cali.ambdac5_5 = hm_info.analogOffset; /**< Analog Offset.*/
                        LOGI("hm_info.analogGain(pga6) = %d, hm_info.analogOffset(ambdac5_5) = %d", hm_info.analogGain, hm_info.analogOffset);
                        if ((hm_info.analogGain > 0) && (hm_info.analogOffset > 0)) {
                            MT6381_WriteCalibration(&cali);
                        }
                    }

                    vsm_driver_set_signal(VSM_SIGNAL_PPG2); //vsm_enable_mode();
                } else {
                    vsm_driver_disable_signal(VSM_SIGNAL_PPG2); // vsm_driver_clear_signal(VSM_SIGNAL_PPG2);
                    err = vsm_driver_deinit();
                    if (err != VSM_STATUS_OK) {
                        return err;
                    }
                }
            }
            break;

        case SENSOR_GET_DATA: {
            int32_t output_len = 0, i = 0;
            int32_t time_offset;

            int32_t ppg_control_fs = 16;            /* The sampling frequency of PPG input (Support 125Hz only). */
            int32_t ppg_control_cfg = 1;             /* The input configuration, default value is 1. */
            int32_t ppg_control_src = 2;             /* The input source, default value is 1 (PPG channel 1). */
            ppg_control_t ppg1_control_input;           /* Input structure for the #ppg_control_process(). */
            int64_t numOfSRAMPPG2Data;
            uint32_t leddrv_con1;

            if (vsm_init_driver == 0) {
                if (actualout) {
                    *actualout = 0;
                }
                err = -1;
                return err;
            }

            numOfSRAMPPG2Data = numOfData[VSM_SRAM_PPG2];

            /* avoid to accumulate numOfData[] */
            if (current_signal & VSM_SIGNAL_PPG2) {
                int err = VSM_STATUS_OK;
                bus_data_t data;
                uint32_t read_data;
                data.addr = 0x33;
                data.reg  = 0x2C;
                data.data_buf = (uint8_t *)&leddrv_con1;
                data.length = sizeof(leddrv_con1);
                err = vsm_driver_read_register(&data);
                LOGD("leddrv_con1 = %d\r\n", leddrv_con1);

                vsm_driver_read_sram(VSM_SRAM_PPG2, sram_data, size_out,
                                     amb_temp_buf, &output_len);

                for (i = 0; i < output_len; i += 2) {
                    sram_data[i] = sram_data[i] >= 0x400000 ? sram_data[i] - 0x800000 : sram_data[i];
                    sram_data[i + 1] = sram_data[i + 1] >= 0x400000 ? sram_data[i + 1] - 0x800000 : sram_data[i + 1];
                    amb_temp_buf[i] = amb_temp_buf[i] >= 0x400000 ? amb_temp_buf[i] - 0x800000 : amb_temp_buf[i];

                    if (ppg1_buf2_len < VSM_SRAM_LEN) {
                        ppg1_agc_buf[ppg1_buf2_len] = leddrv_con1;
                        ppg1_amb_buf[ppg1_buf2_len] = amb_temp_buf[i];
                        ppg1_buf2[ppg1_buf2_len++] = sram_data[i];
                    }
                    if (ppg2_buf2_len < VSM_SRAM_LEN) {
                        ppg2_agc_buf[ppg2_buf2_len] = leddrv_con1;
                        ppg2_amb_buf[ppg2_buf2_len] = amb_temp_buf[i];
                        ppg2_buf2[ppg2_buf2_len++] = sram_data[i + 1];
                    }

                    /* downsample to 16hz for AGC */
                    if ((numOfSRAMPPG2Data + i) % 64 == 0) {
                        if (agc_ppg1_buf_len < ARRAY_SIZE(agc_ppg1_buf)) {
                            agc_ppg1_buf[agc_ppg1_buf_len] = sram_data[i];
                            agc_ppg1_amb_buf[agc_ppg1_buf_len++] = amb_temp_buf[i];
                        }
                        if (agc_ppg2_buf_len < ARRAY_SIZE(agc_ppg2_buf)) {
                            agc_ppg2_buf[agc_ppg2_buf_len] = sram_data[i + 1];
                            agc_ppg2_amb_buf[agc_ppg2_buf_len++] = amb_temp_buf[i]; /* use ppg1 amb data */
                        }
                    }
                }

                if (output_len > 0) {
                    if ((int)sram_data[i - 2] < PPG_THRESHOLD)
                        vsm_driver_set_led(VSM_SIGNAL_PPG2, false);
                    else
                        vsm_driver_set_led(VSM_SIGNAL_PPG2, true);
                }
                LOGD("BIO PPG2 output (%d)\r\n", output_len);
            } else {
                ppg2_buf2_len = 0;
            }

            if (ppg2_buf2_len == VSM_SRAM_LEN) {
                LOGE("PPG2 data dropped\n");
	    }  else if (ppg2_buf2_len > size_out) {
                LOGE("ppg2_buf2_len=%d over %d\n", ppg2_buf2_len, size_out);
	    }  else if (ppg2_buf2_len > 0) {
                sensor_data_unit_t *start = (sensor_data_unit_t *)buff_out;
                for (i = 0; i < ppg2_buf2_len; i++) {
                    start->sensor_type = SENSOR_TYPE_BIOSENSOR_PPG2;
                    start->time_stamp = 0; // sensor alorithm and sensor app don't need timestamp
                    start->bio_data.data = ppg2_buf2[i] + dc_offset;
                    start->bio_data.amb_data = ppg2_amb_buf[i];
                    start->bio_data.agc_data = ppg2_agc_buf[i];
                    start->bio_data.status = SENSOR_STATUS_ACCURACY_HIGH;
                    start++;
                }
            }

            *actualout = ppg2_buf2_len;
            ppg2_buf2_len = 0;

            ppg1_control_input.input = agc_ppg2_buf;
            ppg1_control_input.input_amb = agc_ppg2_amb_buf;
            ppg1_control_input.input_fs = ppg_control_fs;
            ppg1_control_input.input_length = agc_ppg2_buf_len;
            ppg1_control_input.input_config = ppg_control_cfg;
            ppg1_control_input.input_source = ppg_control_src;
            ppg_control_process(&ppg1_control_input, 0, NULL);
            agc_ppg2_buf_len = 0;
        }
        break;

        case SENSOR_CUST:
            if ((buff_in == NULL) || (size_in < sizeof(int32_t))) {
                LOGE("PPG2 SENSOR_CUST parameter error!\n");
                err = -1;
            } else {
                value = *(int32_t *)buff_in;
                LOGI("PPG2 SENSOR_CUST (%d) \r\n", value);
                //ToDo: Customization setting
            }
            break;
        default:
            LOGE("operate function no this parameter %d! \r\n", command);
            err = -1;
            break;
    }

    return err;
}

int32_t vsm_ekg_operate(void *self, uint32_t command, void *buff_out, int32_t size_out, int32_t *actualout,
                    void *buff_in, int32_t size_in)
{
    int err = 0;
    int32_t value = 0;

    switch (command) {
        case SENSOR_DELAY:
            if ((buff_in == NULL) || (size_in < sizeof(int32_t))) {
                LOGE("BIO EKG SENSOR_DELAY sensor parameter error!\n");
                err = -1;
            } else {
                value = *(int32_t *)buff_in;
                LOGI("BIO EKG SENSOR_DELAY ms (%d) \r\n", value);
            }
            break;

        case SENSOR_ENABLE:
            if ((buff_in == NULL) || (size_in < sizeof(int))) {
                LOGE("BIO EKG Enable sensor parameter error!\n");
                err = -1;
            } else {
                value = *(int32_t *)buff_in;
                LOGI("BIO EKG SENSOR_ENABLE (%d) \r\n", value);

                if (value) {
                    err = vsm_driver_init();
                    if (err != VSM_STATUS_OK) {
                        return err;
                    }

                    vsm_driver_set_signal(VSM_SIGNAL_EKG); // vsm_enable_mode();
                } else {
                    vsm_driver_disable_signal(VSM_SIGNAL_EKG); // vsm_driver_clear_signal(VSM_SIGNAL_EKG);
                    err = vsm_driver_deinit();
                    if (err != VSM_STATUS_OK) {
                        return err;
                    }
                }
            }
            break;

        case SENSOR_GET_DATA: {
            int32_t output_len, i = 0;
            int32_t time_offset;

            if (vsm_init_driver == 0) {
                if (actualout) {
                    *actualout = 0;
                }
                err = -1;
                return err;
            }

            if (current_signal & VSM_SIGNAL_EKG) {
                vsm_driver_read_sram(VSM_SRAM_EKG, (uint32_t *)sram_data, size_out,
                                     NULL, &output_len);
            } else {
                output_len = 0;
            }

            LOGD("BIO EKG output (%d)\r\n", output_len);

            //put sensor data to sensor manager
	    if (output_len == VSM_SRAM_LEN) {
                LOGE("EKG data dropped\n");
            } else if (output_len > size_out) {
                LOGE("ekg_data_len=%d over %d\n", output_len, size_out);
            } else if (output_len > 0) {
                sensor_data_unit_t *start = (sensor_data_unit_t *)buff_out;

                for (i = 0; i < output_len; i ++) {
                    if (sram_data[i] >= 0x400000) {
                        sram_data[i] = sram_data[i] - 0x800000;
                    }

                    /* When finger is not ready, ekg value should be saturated (0x3C0000). */
                    /* Set threshold to +/- 1000mV. 1000 * (2 ^ 23) / 4000 = 2097152 */
                    if ((sram_data[i] > 2097152) || (sram_data[i] < -2097152)) {
                        start->bio_data.status = SENSOR_STATUS_UNRELIABLE;
                    } else {
                        start->bio_data.status = SENSOR_STATUS_ACCURACY_HIGH;
                    }
                    start->bio_data.data = sram_data[i];
                    start->sensor_type = SENSOR_TYPE_BIOSENSOR_EKG;
                    start->time_stamp = 0; // sensor alorithm and sensor app don't need timestamp

                    start++;
                }
            }

            *actualout = output_len;
        }

        break;

        case SENSOR_CUST:
            if ((buff_in == NULL) || (size_in < sizeof(int32_t))) {
                LOGE("EKG SENSOR_CUST parameter error!\n");
                err = -1;
            } else {
                value = *(int32_t *)buff_in;
                LOGI("EKG SENSOR_CUST (%d) \r\n", value);
                //ToDo: Customization setting
            }
            break;
        default:
            LOGE("operate function no this parameter %d! \r\n", command);
            err = -1;
            break;
    }

    return err;
}

int32_t vsm_reenable_irq(uint32_t intr_ctrl)
{
    bus_data_t data;
    int32_t ret = 0;
    uint32_t read_data;

    data.addr = (INTR_CTRL_ADDR & 0xFF00)>>8;
    data.reg = (INTR_CTRL_ADDR & 0xFF);
    data.data_buf = (uint8_t *)&read_data;
    data.length = sizeof(read_data);
    ret = vsm_driver_write_register(&data);
    vsm_driver_update_register();
    return ret;
}

int32_t vsm_check_trigger_signal(uint32_t *signal, uint32_t *intr_ctrl)
{
    bus_data_t data;
    uint32_t status_data, ctrl_data;
    uint32_t bak_ctrl_data, bak_status_data;
    int32_t ret = 0;

    if (!signal) {
        return -1;
    }
    //1.check which source trigger interrupt
    data.addr = (INTR_STATUS_ADDR & 0xFF00)>>8;
    data.reg = (INTR_STATUS_ADDR & 0xFF);
    data.data_buf = (uint8_t *)&status_data;
    data.length = sizeof(status_data);
    vsm_driver_read_register(&data);
    bak_status_data = status_data;

    //2.backup ctrl register data
    data.addr = (INTR_CTRL_ADDR & 0xFF00)>>8;
    data.reg = (INTR_CTRL_ADDR & 0xFF);
    data.data_buf = (uint8_t *)&ctrl_data;
    data.length = sizeof(ctrl_data);
    vsm_driver_read_register(&data);
    bak_ctrl_data = ctrl_data;

    //3.clear ctrl rgister
    ctrl_data = 0;
    vsm_driver_write_register(&data);
    vsm_driver_read_register(&data);

    //4.write irq in status rgister
    data.addr = (INTR_STATUS_ADDR & 0xFF00)>>8;
    data.reg = (INTR_STATUS_ADDR & 0xFF);
    data.data_buf = (uint8_t *)&status_data;
    data.length = sizeof(status_data);
    status_data = (status_data & 0xFFF) << 16;

    vsm_driver_write_register(&data);
    status_data = 0;
    vsm_driver_read_register(&data);

    //5.clear irq in status rgister
    status_data = 0;
    vsm_driver_write_register(&data);

    vsm_driver_read_register(&data);

    vsm_driver_update_register();
    //LOGD("%s():bak_ctrl_data 0x%x, bak_status_data 0x%x\r\n", __func__, bak_ctrl_data, bak_status_data);
    //backup control regiser
    *intr_ctrl = bak_ctrl_data;
    /*By control register or by status register*/
    *signal = 0;
    if (bak_status_data & EKG_IRQ_STATUS_MASK) {
        *signal |= (1U << SENSOR_TYPE_BIOSENSOR_EKG);
    }

    if (bak_status_data & PPG1_IRQ_STATUS_MASK) {
        *signal |= (1U << SENSOR_TYPE_BIOSENSOR_PPG1);
    }

    if (bak_status_data & PPG2_IRQ_STATUS_MASK) {
        *signal |= (1U << SENSOR_TYPE_BIOSENSOR_PPG2);
    }

    if (bak_status_data & BISI_IRQ_STATUS_MASK) {
        *signal |= (1U << SENSOR_TYPE_BIOSENSOR_BISI);
    }
    //LOGD("%s():signal 0x%x\r\n", __func__, *signal);
    return ret;
}

void vsm_eint_set(uint32_t eint_num)
{
    vsm_eint_num = (uint8_t)eint_num;
}

void vsm_eint_handler(void *parameter)
{
    BaseType_t xHigherPriorityTaskWoken;

    hal_eint_mask((hal_eint_number_t)vsm_eint_num);
    hal_eint_unmask((hal_eint_number_t)vsm_eint_num);

    vsm_event.event = SM_EVENT_DATA_READY;

    vsm_event.data_ready = (1U << SENSOR_TYPE_BIOSENSOR_PPG1);

    /*ToDo: assign correct sample rate */
    vsm_event.delay = 8; /* PPG delay */

#if 0
    // Post the event.
    xQueueSendFromISR(sm_queue_handle, &vsm_event, &xHigherPriorityTaskWoken);

    // Now the buffer is empty we can switch context if necessary.
    if ( xHigherPriorityTaskWoken ) {
        // Actual macro used here is port specific.
        portYIELD_FROM_ISR (xHigherPriorityTaskWoken);
    }
#endif
}

vsm_status_t vsm_driver_sensor_subsys_init(void)
{
    sensor_driver_object_t obj_vsm_ppg1;
    sensor_driver_object_t obj_vsm_ppg2;
    sensor_driver_object_t obj_vsm_ekg;
    vsm_status_t err = VSM_STATUS_OK;

    LOGI("[%s]++\r\n", __func__);

    /* register ekg to sensor manager */
    obj_vsm_ekg.self = (void *)&obj_vsm_ekg;
    obj_vsm_ekg.polling = 1; // polling mode
    obj_vsm_ekg.sensor_operate = vsm_ekg_operate;
    sensor_driver_attach(SENSOR_TYPE_BIOSENSOR_EKG, &obj_vsm_ekg);

    /* register ppg1 to sensor manager */
    obj_vsm_ppg1.self = (void *)&obj_vsm_ppg1;
    obj_vsm_ppg1.polling = 1; // polling mode
    obj_vsm_ppg1.sensor_operate = vsm_ppg1_operate;
    sensor_driver_attach(SENSOR_TYPE_BIOSENSOR_PPG1, &obj_vsm_ppg1);

    /* register ppg2 to sensor manager */
    obj_vsm_ppg2.self = (void *)&obj_vsm_ppg2;
    obj_vsm_ppg2.polling = 1; // polling mode
    obj_vsm_ppg2.sensor_operate = vsm_ppg2_operate;
    sensor_driver_attach(SENSOR_TYPE_BIOSENSOR_PPG2, &obj_vsm_ppg2);

    LOGI("[%s]--\r\n", __func__);
    return err;
}
