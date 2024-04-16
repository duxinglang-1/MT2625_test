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
#include "ring_buffer.h"
#include "syslog.h"

#define LOGE(fmt,arg...) LOG_E(sensor, fmt,##arg)
#define LOGI(fmt,arg...) LOG_I(sensor, fmt,##arg)
#define LOGD(fmt,arg...) LOG_D(sensor, fmt,##arg)

#define MINIMUM(a,b)            ((a) < (b) ? (a) : (b))

int32_t ring_buffer_set_buffer(ring_buffer_t *ring_buff, void *buffer_base, int32_t length_in_byte);
int32_t ring_buffer_get_free_space(ring_buffer_t *ring_buff, int32_t *free_space_in_byte);
int32_t ring_buffer_get_data_count(ring_buffer_t *ring_buff, int32_t *data_count_in_byte);
int32_t ring_buffer_write_buffer(ring_buffer_t *ring_buff, void *source_buffer, int32_t src_len_in_byte);
int32_t ring_buffer_read_buffer(ring_buffer_t *ring_buff, void *dest_buffer, int32_t length_in_byte);
int32_t ring_buffer_get_write_buffer(ring_buffer_t *ring_buff, uint8_t **buffer, int32_t *length_in_byte);
int32_t ring_buffer_get_read_buffer(ring_buffer_t *ring_buff, uint8_t **buffer, int32_t *length_in_byte);
int32_t ring_buffer_write_data_done(ring_buffer_t *ring_buff, int32_t length_in_byte);
int32_t ring_buffer_read_data_done(ring_buffer_t *ring_buff, int32_t length_in_byte);
int32_t ring_buffer_set_buffer(ring_buffer_t *ring_buff, void *buffer_base, int32_t length_in_byte)
{
    ring_buff->buffer_base = (uint8_t *)buffer_base;
    ring_buff->buffer_size = length_in_byte;
    ring_buff->write = 0;
    ring_buff->read = 0;

    LOGI("[%s]ring_buff = %x, buffer_base = %x, length_in_byte = %d", \
         __func__, ring_buff, ring_buff->buffer_base, length_in_byte);

    return 0;
}

int32_t ring_buffer_get_free_space(ring_buffer_t *ring_buff, int32_t *free_space_in_byte)
{
    *free_space_in_byte = ring_buff->read - ring_buff->write - 1;
    if (*free_space_in_byte < 0) {
        *free_space_in_byte += ring_buff->buffer_size;
    }

    return 0;
}

int32_t ring_buffer_get_data_count(ring_buffer_t *ring_buff, int32_t *data_count_in_byte)
{
    *data_count_in_byte = ring_buff->write - ring_buff->read;
    if (*data_count_in_byte < 0) {
        *data_count_in_byte += ring_buff->buffer_size;
    }

    return 0;
}

int32_t ring_buffer_get_write_buffer(ring_buffer_t *ring_buff, uint8_t **buffer, int32_t *length_in_byte)
{
    int32_t count = 0;

    if (ring_buff->read > ring_buff->write) {
        count = ring_buff->read - ring_buff->write - 1;
    } else if (ring_buff->read == 0) {
        count = ring_buff->buffer_size - ring_buff->write - 1;
    } else {
        count = ring_buff->buffer_size - ring_buff->write;
    }
    *buffer = ring_buff->buffer_base + ring_buff->write;
    *length_in_byte = count;

    return 0;
}

int32_t ring_buffer_write_data_done(ring_buffer_t *ring_buff, int32_t length_in_byte)
{
    ring_buff->write += length_in_byte;
    if (ring_buff->write == ring_buff->buffer_size) {
        ring_buff->write = 0;
    }

    return 0;
}

int32_t ring_buffer_read_data_done(ring_buffer_t *ring_buff, int32_t length_in_byte) // mp3_codec_share_buffer_read_data_done
{
    ring_buff->read += length_in_byte;
    if (ring_buff->read == ring_buff->buffer_size) {
        ring_buff->read = 0;
    }

    return 0;
}

int32_t ring_buffer_write_buffer(ring_buffer_t *ring_buff, void *source_buffer, int32_t src_len_in_byte)
{
    int32_t loop_idx = 0;
    int32_t loop_cnt = 2;
    uint8_t *dest_buff = 0;
    uint32_t dest_len = 0;
    int32_t write_len = 0;
    int32_t free_space = 0;
    uint8_t *src_buff = (uint8_t *)source_buffer;

    ring_buff->get_free_space(ring_buff, &free_space);
    if (src_len_in_byte > free_space) {
        return -1;
    }

    for (loop_idx = 0; (loop_idx < loop_cnt) && (src_len_in_byte > 0); loop_idx++) {
        ring_buff->get_write_buffer(ring_buff, &dest_buff, &dest_len);
        write_len = (dest_len > src_len_in_byte)?src_len_in_byte:dest_len;
        memcpy(dest_buff, src_buff, write_len);
        ring_buff->write_data_done(ring_buff, write_len);
        src_len_in_byte -= write_len;
        src_buff += write_len;
    }

    return 0;
}

int32_t ring_buffer_get_read_buffer(ring_buffer_t *ring_buff, uint8_t **buffer, int32_t *length_in_byte)
{
    int32_t count = 0;

    if (ring_buff->write >= ring_buff->read) {
        count = ring_buff->write - ring_buff->read;
    } else {
        count = ring_buff->buffer_size - ring_buff->read;
    }
    *buffer = ring_buff->buffer_base + ring_buff->read;
    *length_in_byte = count;

    return 0;
}

int32_t ring_buffer_read_buffer(ring_buffer_t *ring_buff, void *void_dest_buff, int32_t dest_len_in_byte) // mp3_codec_get_bytes_from_share_buffer
{
    uint8_t *src_buf;
    uint8_t *dest_buff;
    int32_t src_len;
    int32_t loop_idx = 0;

    if (dest_len_in_byte <= 0) {
        return -1;
    }

    dest_buff = (uint8_t *)void_dest_buff;
    for (loop_idx = 0; loop_idx < 2; loop_idx++) {
        ring_buff->get_read_buffer(ring_buff, &src_buf, &src_len);
        if ((src_len > 0) && (dest_len_in_byte > 0)) {
            int32_t read_bytes = MINIMUM(dest_len_in_byte, src_len);
            memcpy(dest_buff, src_buf, read_bytes);
            dest_len_in_byte -= read_bytes;
            dest_buff += read_bytes;
            ring_buff->read_data_done(ring_buff, read_bytes);
        }
    }

    return 0;
}

int32_t ring_buffer_function_init(ring_buffer_t *ring_buff)
{
    ring_buff->set_buffer = ring_buffer_set_buffer;
    ring_buff->get_free_space = ring_buffer_get_free_space;
    ring_buff->get_data_count = ring_buffer_get_data_count;
    ring_buff->write_buffer = ring_buffer_write_buffer;
    ring_buff->get_write_buffer = ring_buffer_get_write_buffer;
    ring_buff->write_data_done = ring_buffer_write_data_done;
    ring_buff->read_buffer = ring_buffer_read_buffer;
    ring_buff->get_read_buffer = ring_buffer_get_read_buffer;
    ring_buff->read_data_done = ring_buffer_read_data_done;

    return 0;
}

int32_t ring_buffer_function_deinit(ring_buffer_t *ring_buff)
{
    memset(ring_buff, 0, sizeof(ring_buffer_t));

    return 0;
}

#ifdef __RING_BUFFER_UT__
#define UT_RING_BUFFER_SIZE_IN_BYTE (80) // 20 elements for int32_t

int32_t ut_buffer[(UT_RING_BUFFER_SIZE_IN_BYTE >> 2)] = {0};

ring_buffer_t ut_ring_buff = {0};

void ring_buffer_ut(void)
{
    int32_t data_count = 0;
    int32_t free_space = 0;
    int32_t input_data[30] = {0};
    int32_t output_data[(UT_RING_BUFFER_SIZE_IN_BYTE >> 2)] = {0};

    input_data[0]  = 0x76543200;
    input_data[1]  = 0x76543201;
    input_data[2]  = 0x76543202;
    input_data[3]  = 0x76543203;
    input_data[4]  = 0x76543204;
    input_data[5]  = 0x76543205;
    input_data[6]  = 0x76543206;
    input_data[7]  = 0x76543207;
    input_data[8]  = 0x76543208;
    input_data[9]  = 0x76543209;
    input_data[10] = 0x76543210;
    input_data[11] = 0x76543211;
    input_data[12] = 0x76543212;
    input_data[13] = 0x76543213;
    input_data[14] = 0x76543214;
    input_data[15] = 0x76543215;
    input_data[16] = 0x76543216;
    input_data[17] = 0x76543217;
    input_data[18] = 0x76543218;
    input_data[19] = 0x76543219;
    input_data[20] = 0x76543220;
    input_data[21] = 0x76543221;
    input_data[22] = 0x76543222;
    input_data[23] = 0x76543223;
    input_data[24] = 0x76543224;
    input_data[25] = 0x76543225;
    input_data[26] = 0x76543226;
    input_data[27] = 0x76543227;
    input_data[28] = 0x76543228;
    input_data[29] = 0x76543229;

    ring_buffer_function_init(&ut_ring_buff);

    LOGI("ut_buffer = %x, ut_buffer size in byte = %d", ut_buffer, UT_RING_BUFFER_SIZE_IN_BYTE);
    ut_ring_buff.set_buffer(&ut_ring_buff, (void *)ut_buffer, UT_RING_BUFFER_SIZE_IN_BYTE);

    ut_ring_buff.get_data_count(&ut_ring_buff, &data_count);
    LOGI("data_count1 = %d", data_count);
    ut_ring_buff.get_free_space(&ut_ring_buff, &free_space);
    LOGI("free_space1 = %d", free_space);

    if (15 * sizeof(input_data[0]) < free_space) {
        ut_ring_buff.write_buffer(&ut_ring_buff, input_data, 15 * sizeof(input_data[0]));
    }

    ut_ring_buff.get_data_count(&ut_ring_buff, &data_count);
    LOGI("data_count2 = %d", data_count);
    ut_ring_buff.get_free_space(&ut_ring_buff, &free_space);
    LOGI("free_space2 = %d", free_space);

    ut_ring_buff.read_buffer(&ut_ring_buff, output_data, 10 * sizeof(output_data[0]));
    LOGI("output_data[0] = %x", output_data[0]); // 0x76543200
    LOGI("output_data[1] = %x", output_data[1]);
    LOGI("output_data[2] = %x", output_data[2]);
    LOGI("output_data[3] = %x", output_data[3]);
    LOGI("output_data[4] = %x", output_data[4]);
    LOGI("output_data[5] = %x", output_data[5]);
    LOGI("output_data[6] = %x", output_data[6]);
    LOGI("output_data[7] = %x", output_data[7]);
    LOGI("output_data[8] = %x", output_data[8]);
    LOGI("output_data[9] = %x", output_data[9]); // 0x76543209

    ut_ring_buff.get_data_count(&ut_ring_buff, &data_count);
    LOGI("data_count3 = %d", data_count);
    ut_ring_buff.get_free_space(&ut_ring_buff, &free_space);
    LOGI("free_space3 = %d", free_space);

    if (12 * sizeof(input_data[0]) < free_space) {
        ut_ring_buff.write_buffer(&ut_ring_buff, input_data + 15, 12 * sizeof(input_data[0]));
    }

    ut_ring_buff.get_data_count(&ut_ring_buff, &data_count);
    LOGI("data_count4 = %d", data_count);
    ut_ring_buff.get_free_space(&ut_ring_buff, &free_space);
    LOGI("free_space4 = %d", free_space);

    ut_ring_buff.read_buffer(&ut_ring_buff, output_data, 13 * sizeof(output_data[0]));
    LOGI("output_data[0] = %x", output_data[0]); // 0x76543210
    LOGI("output_data[1] = %x", output_data[1]);
    LOGI("output_data[2] = %x", output_data[2]);
    LOGI("output_data[3] = %x", output_data[3]);
    LOGI("output_data[4] = %x", output_data[4]);
    LOGI("output_data[5] = %x", output_data[5]);
    LOGI("output_data[6] = %x", output_data[6]);
    LOGI("output_data[7] = %x", output_data[7]);
    LOGI("output_data[8] = %x", output_data[8]);
    LOGI("output_data[9] = %x", output_data[9]); // 0x76543219
    LOGI("output_data[10] = %x", output_data[10]); // 0x76543220
    LOGI("output_data[11] = %x", output_data[11]); // 0x76543221
    LOGI("output_data[12] = %x", output_data[12]); // 0x76543221

    ut_ring_buff.get_data_count(&ut_ring_buff, &data_count);
    LOGI("data_count5 = %d", data_count);
    ut_ring_buff.get_free_space(&ut_ring_buff, &free_space);
    LOGI("free_space5 = %d", free_space);

}
#endif // #ifdef __RING_BUFFER_UT__

