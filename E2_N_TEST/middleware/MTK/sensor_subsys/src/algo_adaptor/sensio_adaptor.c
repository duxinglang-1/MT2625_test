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
#include <stdint.h>
#include "sensor_alg_interface.h"
#include "algo_adaptor/algo_adaptor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ring_buffer.h"
#include "nvdm.h"

#define LOGE(fmt,arg...) LOG_E(sensor, fmt,##arg)
#define LOGI(fmt,arg...) LOG_I(sensor, fmt,##arg)
#define LOGD(fmt,arg...) LOG_D(sensor, fmt,##arg)

#define MINIMUM(a,b) ((a) < (b) ? (a) : (b))

#if defined(MTK_SENSOR_BIO_USE_MT6381)
#include "vsm_sensor_subsys_adaptor.h"

#include "lib_bsp_alg.h"
#define MT6381_ALGORITHM_SEGMENT_LENGTH (12)
#define SNR10_PPG1 SNR10_MED_PPG1
#define SNR10_PPG2 SNR10_MED_PPG2
#define SNR40_ECG SNR40_LOW_ECG
#define LOW_SNR_TIMEOUT_IN_US (2000000UL) // 2 seconds

#define MT6381_ALGORITHM_CALCULATE_TIME_IN_US (56000000UL)

#define SENSIO_EKG_DELAY 50
#define SENSIO_EKG_POLLING_TIME 400
#define SENSIO_PPG1_DELAY 50
#define SENSIO_PPG1_POLLING_TIME 400
#define SENSIO_PPG2_DELAY 50
#define SENSIO_PPG2_POLLING_TIME 400

#define __RAW_DATA2__
#ifdef __RAW_DATA2__
#define EKG_Data_Size (36000) // 40960 = 122880 / 3 //  (122880) // 122880 = 120KB = 120 * 1024
#define PPG1_Data_Size (36000)
#define PPG2_Data_Size (36000)
uint32_t EKG_Data[EKG_Data_Size] = {0};
uint32_t EKG_Data_Index = 0;
uint32_t EKG_Data_Counter = 0;

uint32_t PPG1_Data[PPG1_Data_Size] = {0};
uint32_t PPG1_Data_Index = 0;
uint32_t PPG1_Data_Counter = 0;

uint32_t PPG2_Data[PPG2_Data_Size] = {0};
uint32_t PPG2_Data_Index = 0;
uint32_t PPG2_Data_Counter = 0;

#define __USING_RING_BUFFER__
#ifdef __USING_RING_BUFFER__
#define SENSIO_RAW_SIZE_IN_BYTE (4)
#define SENSIO_RAW_BUFFER_COUNT (256)
int32_t int32_ppg1_buffer[SENSIO_RAW_BUFFER_COUNT] = {0};
int32_t int32_ppg2_buffer[SENSIO_RAW_BUFFER_COUNT] = {0};
int32_t int32_ekg_buffer[SENSIO_RAW_BUFFER_COUNT] = {0};

ring_buffer_t ring_buff_ppg1 = {0};
ring_buffer_t ring_buff_ppg2 = {0};
ring_buffer_t ring_buff_ekg = {0};

static int32_t int32_free_space = 0;
#endif

extern uint32_t dc_offset;

void sensio_raw_data_dump(uint32_t input_sensor_type)
{
    int *praw_data, raw_data_idx, raw_data_counter, raw_data_size, raw_offset;
    uint32_t idx = 0, jdx;
    char str_buf[256];

    if (input_sensor_type == SENSOR_TYPE_BIOSENSOR_PPG1) {
        raw_data_idx = PPG1_Data_Index;
        raw_data_counter = PPG1_Data_Counter;
        praw_data = (int *)&PPG1_Data[0];
        raw_data_size = PPG1_Data_Size;
        raw_offset = dc_offset;
    } else if (input_sensor_type == SENSOR_TYPE_BIOSENSOR_PPG2) {
        raw_data_idx = PPG2_Data_Index;
        raw_data_counter = PPG2_Data_Counter;
        praw_data = (int *)&PPG2_Data[0];
        raw_data_size = PPG2_Data_Size;
        raw_offset = dc_offset;
    } else if (input_sensor_type == SENSOR_TYPE_BIOSENSOR_EKG) {
        raw_data_idx = EKG_Data_Index;
        raw_data_counter = EKG_Data_Counter;
        praw_data = (int *)&EKG_Data[0];
        raw_data_size = EKG_Data_Size;
        raw_offset = 0;
    } else {
        return;
    }

    LOGI("type=%d, idx=%d, counter=%d, size=%d, dc_offset=%d\r\n",
         input_sensor_type, raw_data_idx, raw_data_counter,
         raw_data_size, raw_offset);

    for (idx=0,jdx=0;jdx < (raw_data_counter/12);idx+=14,jdx++) {
        sprintf(str_buf, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                input_sensor_type, praw_data[idx],
                praw_data[idx+1], praw_data[idx+2], praw_data[idx+3],
                praw_data[idx+4], praw_data[idx+5], praw_data[idx+6],
                praw_data[idx+7], praw_data[idx+8], praw_data[idx+9],
                praw_data[idx+10], praw_data[idx+11], praw_data[idx+12],
                praw_data[idx+13], jdx);

        LOGI("%s", str_buf);

        if ((jdx%5) == 0) {
            vTaskDelay(5);
        }
    }
}
#endif // #define __RAW_DATA2__

uint32_t sensio_start_time;
int32_t sensio_reported;
int32_t sensio_updated;
extern uint32_t xSMLastExecutionTimeInUS;

static sensor_input_list_t input_sensio_comp_ekg;
static sensor_input_list_t input_sensio_comp_ppg1;
static sensor_input_list_t input_sensio_comp_ppg2;

static void check_snr(int snr)
{
    static uint32_t low_ppg1_snr10_timestamp = 0;
    static uint32_t low_ppg2_snr10_timestamp = 0;
    static uint32_t low_ecg_snr40_timestamp = 0;
    int ppg1_snr10, ppg2_snr10, ecg_snr40;

    ppg1_snr10 = (snr & 0x000000FF);
    ppg2_snr10 = (snr & 0x0000FF00) >> 8;
    ecg_snr40 = (snr & 0x00FF0000) >> 16;

    if (ppg1_snr10 >= SNR10_PPG1) {
        low_ppg1_snr10_timestamp = 0;
    } else {
        if (low_ppg1_snr10_timestamp == 0) {
            low_ppg1_snr10_timestamp = xSMLastExecutionTimeInUS;
        } else if ((xSMLastExecutionTimeInUS - low_ppg1_snr10_timestamp) > LOW_SNR_TIMEOUT_IN_US) { // 2 seconds
            sensor_data_t bio_ppg1_data_snr;
            sensor_data_unit_t bio_ppg1_data_unit_snr;

            memset(&bio_ppg1_data_snr, 0, sizeof(bio_ppg1_data_snr));
            memset(&bio_ppg1_data_unit_snr, 0, sizeof(bio_ppg1_data_unit_snr));

            bio_ppg1_data_unit_snr.sensor_type = SENSOR_TYPE_BIOSENSOR_PPG1;
            bio_ppg1_data_unit_snr.bio_data.status = SENSOR_STATUS_UNRELIABLE;

            bio_ppg1_data_snr.data_exist_count = 1;
            bio_ppg1_data_snr.data = &bio_ppg1_data_unit_snr;

            LOGI("ppg1 snr LOW: %d", ppg1_snr10);
#ifdef __ENABLE_SNR_CALLBACK__
            sensor_fusion_algorithm_send_to_subscriber(SENSOR_TYPE_BIOSENSOR_PPG1, bio_ppg1_data_snr);
#endif
        }
    }

    if (ppg2_snr10 >= SNR10_PPG2) {
        low_ppg2_snr10_timestamp = 0;
    } else {
        if (low_ppg2_snr10_timestamp == 0) {
            low_ppg2_snr10_timestamp = xSMLastExecutionTimeInUS;
        } else if ((xSMLastExecutionTimeInUS - low_ppg2_snr10_timestamp) > LOW_SNR_TIMEOUT_IN_US) { // 2 seconds
            sensor_data_t bio_ppg2_data_snr;
            sensor_data_unit_t bio_ppg2_data_unit_snr;

            memset(&bio_ppg2_data_snr, 0, sizeof(bio_ppg2_data_snr));
            memset(&bio_ppg2_data_unit_snr, 0, sizeof(bio_ppg2_data_unit_snr));

            bio_ppg2_data_unit_snr.sensor_type = SENSOR_TYPE_BIOSENSOR_PPG2;
            bio_ppg2_data_unit_snr.bio_data.status = SENSOR_STATUS_UNRELIABLE;

            bio_ppg2_data_snr.data_exist_count = 1;
            bio_ppg2_data_snr.data = &bio_ppg2_data_unit_snr;

            LOGI("ppg2 snr LOW: %d", ppg2_snr10);
#ifdef __ENABLE_SNR_CALLBACK__
            sensor_fusion_algorithm_send_to_subscriber(SENSOR_TYPE_BIOSENSOR_PPG2, bio_ppg2_data_snr);
#endif
        }
    }

    if (ecg_snr40 >= SNR40_ECG) {
        low_ecg_snr40_timestamp = 0;
    } else {
        if (low_ecg_snr40_timestamp == 0) {
            low_ecg_snr40_timestamp = xSMLastExecutionTimeInUS;
        } else if ((xSMLastExecutionTimeInUS - low_ecg_snr40_timestamp) > LOW_SNR_TIMEOUT_IN_US) { // 2 seconds
            sensor_data_t bio_ekg_data_snr;
            sensor_data_unit_t bio_ekg_data_unit_snr;

            memset(&bio_ekg_data_snr, 0, sizeof(bio_ekg_data_snr));
            memset(&bio_ekg_data_unit_snr, 0, sizeof(bio_ekg_data_unit_snr));

            bio_ekg_data_unit_snr.sensor_type = SENSOR_TYPE_BIOSENSOR_EKG;
            bio_ekg_data_unit_snr.bio_data.status = SENSOR_STATUS_UNRELIABLE;

            bio_ekg_data_snr.data_exist_count = 1;
            bio_ekg_data_snr.data = &bio_ekg_data_unit_snr;

            LOGI("ekg snr LOW: %d", ecg_snr40);
#ifdef __ENABLE_SNR_CALLBACK__
            sensor_fusion_algorithm_send_to_subscriber(SENSOR_TYPE_BIOSENSOR_EKG, bio_ekg_data_snr);
#endif
        }
    }

    LOGD("snr = %x, ppg1_snr10 = %d, ppg2_snr10 = %d, ecg_snr40 = %d", snr, ppg1_snr10, ppg2_snr10, ecg_snr40);
}

static int32_t sensio_get_result(sensor_data_t *const output)
{
    int32_t sbp = 0, dbp = 0, bpm = 0, spo2 = 0, fatigueIndex = 0, pressureIndex = 0;
    sensor_data_unit_t *psensio_data = output->data;
    sensio_nvdm_hm_info_struct hm_info;
    nvdm_status_t status;
    uint32_t size;
    int32_t bp_cal_data_out[6];

    sbp = bp_alg_get_sbp(); // 25 sec output the first non-zero value (update only once), if output -1, it means the signal quality is bad, need re-start lib
    dbp = bp_alg_get_dbp(); // 25 sec output the first non-zero value (update only once), if output -1, it means the signal quality is bad, need re-start lib
    bpm = spo2_get_bpm();   // 6-10 sec output the first non-zero value, after the first value, each sec output an new value
    spo2 = spo2_get_spo2(); // 6-10 sec output the first non-zero value, after the first value, each sec output an new value
    fatigueIndex = bp_alg_get_hrv_fatigue();   // 55 sec output the non-zero first value (update only once)
    pressureIndex = bp_alg_get_hrv_pressure(); // 55 sec output the non-zero first value (update only once)

    LOGI("sbp:%d,dbp:%d,bpm:%d,spo2:%d,fatigue:%d,pressure:%d\r\n",
         sbp, dbp, bpm, spo2, fatigueIndex, pressureIndex);

    /* BP Calibration */
    size = sizeof(hm_info);
    status = nvdm_read_data_item(SENSIO_NVDM_GROUP_NAME, SENSIO_NVDM_HM_INFO_NAME, &hm_info, &size);
    if (status == NVDM_STATUS_OK) {
        if (2 == hm_info.bp_mode) { // 1: normal mode, 2: calibration mode, 3: personal mode
            LOGI("BP Calibration Mode: sbp = %d", sbp);

            if ((sbp != 0) || (sbp != -1)) {
                bp_alg_get_calibration_data(bp_cal_data_out, 6); // this data is for calibration; 
                LOGI("BP Calibration Data: %d, %d, %d", bp_cal_data_out[0], bp_cal_data_out[1], bp_cal_data_out[2]);
                hm_info.calibrate_para[4] = bp_cal_data_out[0]; // calibrate_para[7];   //sbp1, dbp1, sbp2, dbp2, calibration_para1, calibration_para2, calibration_para3
                hm_info.calibrate_para[5] = bp_cal_data_out[1];
                hm_info.calibrate_para[6] = bp_cal_data_out[2];
                nvdm_write_data_item(SENSIO_NVDM_GROUP_NAME, SENSIO_NVDM_HM_INFO_NAME, \
                                     NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t *)&hm_info, size);
            }
        }
    }

    psensio_data->sensor_type = SENSOR_TYPE_SENSIO;
    psensio_data->time_stamp = xSMLastExecutionTimeInUS;
    psensio_data->sensio_t.sbp = sbp;
    psensio_data->sensio_t.dbp = dbp;
    psensio_data->sensio_t.bpm = bpm;
    psensio_data->sensio_t.spo2 = spo2;
    psensio_data->sensio_t.fatigueIndex = fatigueIndex;
    psensio_data->sensio_t.pressureIndex = pressureIndex;
    sensio_reported = 1; /* one-shot notification */

    if ((sbp > 0) && (dbp > 0) && (bpm > 0) && (spo2 > 0) && (fatigueIndex > 0) && (pressureIndex > 0)) 
    {
        psensio_data->sensio_t.status = SENSOR_STATUS_ACCURACY_HIGH;
    } else {
        psensio_data->sensio_t.status = SENSOR_STATUS_UNRELIABLE;
    }

    return 1;
}

static int32_t sensio_process_data(const sensor_data_t *input_list, void *reserve)
{
    int32_t i, ret = 1;
    uint32_t count = input_list->data_exist_count;
    uint32_t seq_num = count / MT6381_ALGORITHM_SEGMENT_LENGTH;
    uint32_t input_sensor_type = input_list->data->sensor_type;
    uint32_t data[MT6381_ALGORITHM_SEGMENT_LENGTH];
#ifdef __USING_RING_BUFFER__
    ring_buffer_t *p_ring;
#endif
#ifdef __RAW_DATA2__
    uint32_t *praw_data, raw_data_idx, raw_data_counter, raw_data_size;
#endif
    uint32_t start_time;
    sensor_data_unit_t *data_start = input_list->data;
    int snr = 0, snr_check = 1;
    char str[200] = {0};

    if (sensio_reported == 1) {
        goto _out;
    }

    start_time = sensor_driver_get_us_tick();

    LOGD("[%s]++ type=%d, count=%d\r\n", __func__, input_sensor_type, count);

    sensio_updated |= (1U << input_sensor_type);

    if (input_sensor_type == SENSOR_TYPE_BIOSENSOR_PPG1) {
#ifndef __USING_RING_BUFFER__
        snr_check = 0;
#else
        p_ring = &ring_buff_ppg1;
#endif
#ifdef __RAW_DATA2__
        raw_data_idx = PPG1_Data_Index;
        raw_data_counter = PPG1_Data_Counter;
        praw_data = &PPG1_Data[0];
        raw_data_size = PPG1_Data_Size;
#endif
    } else if (input_sensor_type == SENSOR_TYPE_BIOSENSOR_PPG2) {
#ifdef __USING_RING_BUFFER__
        p_ring = &ring_buff_ppg2;
#endif
#ifdef __RAW_DATA2__
        raw_data_idx = PPG2_Data_Index;
        raw_data_counter = PPG2_Data_Counter;
        praw_data = &PPG2_Data[0];
        raw_data_size = PPG2_Data_Size;
#endif
    } else if (input_sensor_type == SENSOR_TYPE_BIOSENSOR_EKG) {
#ifdef __USING_RING_BUFFER__
        p_ring = &ring_buff_ekg;
#endif
#ifdef __RAW_DATA2__
        raw_data_idx = EKG_Data_Index;
        raw_data_counter = EKG_Data_Counter;
        praw_data = &EKG_Data[0];
        raw_data_size = EKG_Data_Size;
#endif
    } else {
        assert(0);
    }

    LOGD("idx=%d, counter=%d\r\n", raw_data_idx, raw_data_counter);

    // Todo: Need to check performance issue due to one by one input
    i = 0;
    while (seq_num != 0) {
        data[i] = data_start->bio_data.data;

#ifdef __RAW_DATA2__
        if (raw_data_idx < raw_data_size) {
            if (i == 0) {
                praw_data[raw_data_idx] = raw_data_counter;
                raw_data_idx++;
            }
            praw_data[raw_data_idx] = data[i];
            raw_data_idx++;
        }
#endif

#ifdef __USING_RING_BUFFER__
        p_ring->get_free_space(p_ring, &int32_free_space);
        if (int32_free_space > SENSIO_RAW_SIZE_IN_BYTE) {
            p_ring->write_buffer(p_ring, &data[i], SENSIO_RAW_SIZE_IN_BYTE);
        } else {
            LOGI("type=%d ring_buff buffer full: %d", input_sensor_type, int32_free_space);
        }
#endif

        data_start++;
        count--;
        i++;

        if (i >= MT6381_ALGORITHM_SEGMENT_LENGTH) {
#ifndef __USING_RING_BUFFER__
            if (input_sensor_type == SENSOR_TYPE_BIOSENSOR_PPG1) {
                snr = checkQuality(data, MT6381_ALGORITHM_SEGMENT_LENGTH, NULL, 0, NULL, 0);
            } else if (input_sensor_type == SENSOR_TYPE_BIOSENSOR_PPG2) {
                snr = checkQuality(NULL, 0, data, MT6381_ALGORITHM_SEGMENT_LENGTH, NULL, 0);
            } else if (input_sensor_type == SENSOR_TYPE_BIOSENSOR_EKG) {
                snr = checkQuality(NULL, 0, NULL, 0, data, MT6381_ALGORITHM_SEGMENT_LENGTH);
            }
#endif

#ifdef __RAW_DATA2__
            if (raw_data_idx < raw_data_size) {
                praw_data[raw_data_idx] = xSMLastExecutionTimeInUS;
                raw_data_idx++;
                raw_data_counter+=MT6381_ALGORITHM_SEGMENT_LENGTH;
            }
#endif

#ifndef __USING_RING_BUFFER__
            // Check SNR
            if (snr_check == 0 && snr != 0) {
                check_snr(snr);
                snr_check = 1;
            }
#endif
            i = 0;
            seq_num--;

            LOGD("DATA[0] = %d %d %d %d %d %d\r\n",
                 data[0], data[1], data[2], data[3], data[4], data[5]);
            LOGD("DATA[1] = %d %d %d %d %d\r\n",
                 data[6], data[7], data[8], data[9], data[10], data[11]);
        }
    }

    if (count > 0) {
        LOGI("type=%d, reminder=%d\r\n", input_sensor_type, count);
        for (i = 0; i < count; i++) {
            data[i] = data_start->bio_data.data;

            sprintf(str, "%s %d", str, data[i]);
            data_start++;
        }

        if (input_sensor_type == SENSOR_TYPE_BIOSENSOR_PPG1) {
            snr = checkQuality(data, count, NULL, 0, NULL, 0);
        } else if (input_sensor_type == SENSOR_TYPE_BIOSENSOR_PPG2) {
            snr = checkQuality(NULL, 0, data, count, NULL, 0);
        } else if (input_sensor_type == SENSOR_TYPE_BIOSENSOR_EKG) {
            snr = checkQuality(NULL, 0, NULL, 0, data, count);
        }

        LOGI("%s", str);
    }

#ifdef __RAW_DATA2__
    if (input_sensor_type == SENSOR_TYPE_BIOSENSOR_PPG1) {
        PPG1_Data_Index = raw_data_idx;
        PPG1_Data_Counter = raw_data_counter;
    } else if (input_sensor_type == SENSOR_TYPE_BIOSENSOR_PPG2) {
        PPG2_Data_Index = raw_data_idx;
        PPG2_Data_Counter = raw_data_counter;
    } else if (input_sensor_type == SENSOR_TYPE_BIOSENSOR_EKG) {
        EKG_Data_Index = raw_data_idx;
        EKG_Data_Counter = raw_data_counter;
    }
#endif

_exit:
#ifdef __USING_RING_BUFFER__
    if ((sensio_updated & (1U << SENSOR_TYPE_BIOSENSOR_EKG))
         && (sensio_updated & (1U << SENSOR_TYPE_BIOSENSOR_PPG1))
         && (sensio_updated & (1U << SENSOR_TYPE_BIOSENSOR_PPG2))) {
        int32_t int32_ppg1_data_count = 0, int32_ppg2_data_count = 0, int32_ekg_data_count = 0;
        int32_t count = 0;
        int32_t ppg1_raw_data[MT6381_ALGORITHM_SEGMENT_LENGTH] = {0};
        int32_t ppg2_raw_data[MT6381_ALGORITHM_SEGMENT_LENGTH] = {0};
        int32_t ekg_raw_data[MT6381_ALGORITHM_SEGMENT_LENGTH] = {0};

        ring_buff_ppg1.get_data_count(&ring_buff_ppg1, &int32_ppg1_data_count);
        ring_buff_ppg2.get_data_count(&ring_buff_ppg2, &int32_ppg2_data_count);
        ring_buff_ekg.get_data_count(&ring_buff_ekg, &int32_ekg_data_count);

        count = MINIMUM(int32_ppg1_data_count, int32_ppg2_data_count);
        count = MINIMUM(count, int32_ekg_data_count);
        count = (count / SENSIO_RAW_SIZE_IN_BYTE) / MT6381_ALGORITHM_SEGMENT_LENGTH;
        for (i = 0; i < count; i++) {
            ring_buff_ppg1.read_buffer(&ring_buff_ppg1, ppg1_raw_data, MT6381_ALGORITHM_SEGMENT_LENGTH * SENSIO_RAW_SIZE_IN_BYTE);
            ring_buff_ppg2.read_buffer(&ring_buff_ppg2, ppg2_raw_data, MT6381_ALGORITHM_SEGMENT_LENGTH * SENSIO_RAW_SIZE_IN_BYTE);
            ring_buff_ekg.read_buffer(&ring_buff_ekg, ekg_raw_data, MT6381_ALGORITHM_SEGMENT_LENGTH * SENSIO_RAW_SIZE_IN_BYTE);
            snr = checkQuality(ppg1_raw_data, MT6381_ALGORITHM_SEGMENT_LENGTH, ppg2_raw_data, MT6381_ALGORITHM_SEGMENT_LENGTH, ekg_raw_data, MT6381_ALGORITHM_SEGMENT_LENGTH);
            if (snr_check == 1 && snr != 0) {
                check_snr(snr);
                snr_check = 0;
            }
        }

        sensio_updated = 0;
    }
#endif

    LOGD("checkQuality excution time = %d microsecond\r\n", sensor_driver_get_us_tick() - start_time);
    LOGD("xSMLastExecutionTimeInUS-sensio_start_time = %d\r\n", xSMLastExecutionTimeInUS-sensio_start_time);

    if ((xSMLastExecutionTimeInUS-sensio_start_time) > MT6381_ALGORITHM_CALCULATE_TIME_IN_US) {
        /* send sensio output after subscription 56 seconds */
        sensor_fusion_algorithm_notify(SENSOR_TYPE_SENSIO);
    }

_out:
    LOGD("[%s]ret=%d sensio_reported=%d --\r\n", __func__, ret, sensio_reported);

    return ret;
}

static int32_t sensio_operate(int32_t command, void *buffer_out, int32_t size_out,
                              void *buffer_in, int32_t size_in)
{
    return 0;
}

const sensor_descriptor_t sensio_desp = {
    SENSOR_TYPE_SENSIO, //output_type
    1, /* version */
    SENSOR_REPORT_MODE_ON_CHANGE, /* report_mode */
    &input_sensio_comp_ekg, /* sensor_input_list_t */
    sensio_operate,
    sensio_get_result,
    sensio_process_data,
    0 /* accumulate */
};

int sensio_register(void)
{
    int ret; /*return: fail=-1, pass>=0, which means the count of current register algorithm */

    input_sensio_comp_ekg.input_type = SENSOR_TYPE_BIOSENSOR_EKG;
    input_sensio_comp_ekg.sampling_delay = SENSIO_EKG_DELAY; // ms
    input_sensio_comp_ekg.timeout = SENSIO_EKG_POLLING_TIME; // ms

    input_sensio_comp_ppg1.input_type = SENSOR_TYPE_BIOSENSOR_PPG1;
    input_sensio_comp_ppg1.sampling_delay = SENSIO_PPG1_DELAY; // ms
    input_sensio_comp_ppg1.timeout = SENSIO_PPG1_POLLING_TIME; // ms

    input_sensio_comp_ppg2.input_type = SENSOR_TYPE_BIOSENSOR_PPG2;
    input_sensio_comp_ppg2.sampling_delay = SENSIO_PPG2_DELAY; // ms
    input_sensio_comp_ppg2.timeout = SENSIO_PPG2_POLLING_TIME; // ms

    input_sensio_comp_ekg.next_input = &input_sensio_comp_ppg1; // build as a signal linked list
    input_sensio_comp_ppg1.next_input = &input_sensio_comp_ppg2; // build as a signal linked list
    input_sensio_comp_ppg2.next_input = NULL;

    ret = sensor_fusion_algorithm_register_type(&sensio_desp);
    if (ret < 0) {
        LOGE("fail to register sensio ret=%d\r\n", ret);
    }
    ret = sensor_fusion_algorithm_register_data_buffer(SENSOR_TYPE_SENSIO, 1);
    if (ret < 0) {
        LOGE("fail to register sensio buffer ret=%d\r\n", ret);
    }
    return ret;
}

int sensio_init(void)
{
    uint32_t age = 0, gender = 0, height = 0, weight = 0, size;
    sensio_nvdm_hm_info_struct hm_info;
    nvdm_status_t status;

    LOGI("[%s]++", __func__);

#ifdef __USING_RING_BUFFER__
    ring_buffer_function_init(&ring_buff_ppg1);
    ring_buffer_function_init(&ring_buff_ppg2);
    ring_buffer_function_init(&ring_buff_ekg);

    ring_buff_ppg1.set_buffer(&ring_buff_ppg1, (void *)int32_ppg1_buffer, 4 * SENSIO_RAW_BUFFER_COUNT);
    ring_buff_ppg2.set_buffer(&ring_buff_ppg2, (void *)int32_ppg2_buffer, 4 * SENSIO_RAW_BUFFER_COUNT);
    ring_buff_ekg.set_buffer(&ring_buff_ekg, (void *)int32_ekg_buffer, 4 * SENSIO_RAW_BUFFER_COUNT);
#endif // #ifdef __USING_RING_BUFFER__

    check_quality_init(1);
    mt6381_config_feature(0x1F); // Enable all functions

    size = sizeof(hm_info);
    status = nvdm_read_data_item(SENSIO_NVDM_GROUP_NAME, SENSIO_NVDM_HM_INFO_NAME, &hm_info, &size);
    if (status == NVDM_STATUS_OK) {
        age = hm_info.age;
        gender = hm_info.gender;
        height = hm_info.height;
        weight = hm_info.weight;
    }

    if (!((age > 0) && (height > 0) && (weight > 0) && ((gender == 1) || (gender == 2)))) {
       /* use default values */
        age = MT6381_DEFAULT_AGE;
        gender = MT6381_DEFAULT_GENDER;
        height = MT6381_DEFAULT_HEIGHT;
        weight = MT6381_DEFAULT_WEIGHT;
    }

    LOGI("status=%d, age=%d, gender=%d, height=%d, weight=%d",
         status, age, gender, height, weight);

    bp_alg_set_user_info(age, gender, height, weight, 0);// gender: 1 male, 2 female; height cm, weight kg

    if (3 == hm_info.bp_mode) { // 1: normal mode, 2: calibration mode, 3: personal mode
        int32_t bp_cal_data_in[18];

        LOGI("BP Personal Mode: %d, %d, %d, %d, %d, %d, %d", \
             hm_info.calibrate_para[0], hm_info.calibrate_para[1], hm_info.calibrate_para[2], \
             hm_info.calibrate_para[3], hm_info.calibrate_para[4], hm_info.calibrate_para[5], \
             hm_info.calibrate_para[6]);
        bp_cal_data_in[0] = hm_info.calibrate_para[0]; // sbp1
        bp_cal_data_in[1] = hm_info.calibrate_para[1]; // dbp1
        bp_cal_data_in[2] = hm_info.calibrate_para[2]; // sbp2
        bp_cal_data_in[3] = hm_info.calibrate_para[3]; // dbp2
        bp_cal_data_in[4] = 0;
        bp_cal_data_in[5] = 0;
        bp_cal_data_in[6] = hm_info.calibrate_para[4]; // bp_cal_data_out[0];
        bp_cal_data_in[7] = hm_info.calibrate_para[5]; // bp_cal_data_out[1];
        bp_cal_data_in[8] = hm_info.calibrate_para[6]; // bp_cal_data_out[2];
        bp_cal_data_in[9] = 0;
        bp_cal_data_in[10] = 0;
        bp_cal_data_in[11] = 0;
        bp_cal_data_in[12] = 0;
        bp_cal_data_in[13] = 0;
        bp_cal_data_in[14] = 0;
        bp_cal_data_in[15] = 0;
        bp_cal_data_in[16] = 0;
        bp_cal_data_in[17] = 0;
        bp_alg_set_calibration_data(bp_cal_data_in, 18);
    }

    sensio_reported = 0;
    sensio_start_time = sensor_driver_get_us_tick();

#ifdef __RAW_DATA2__
    EKG_Data_Index = 0;
    EKG_Data_Counter = 0;
    PPG1_Data_Index = 0;
    PPG1_Data_Counter = 0;
    PPG2_Data_Index = 0;
    PPG2_Data_Counter = 0;
#endif

    LOGI("[%s]--", __func__);

    return 1;
}

int sensio_deinit(void)
{
    LOGI("[%s]++", __func__);

    sensio_reported = 1;
    mt6381_config_feature(0); // Enable all functions
    check_quality_init(0);

#ifdef __USING_RING_BUFFER__
    ring_buffer_function_deinit(&ring_buff_ppg1);
    ring_buffer_function_deinit(&ring_buff_ppg2);
    ring_buffer_function_deinit(&ring_buff_ekg);
#endif

    LOGI("[%s]--", __func__);

    return 1;
}
#endif /* MTK_SENSOR_BIO_USE_MT6381 */
