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

#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__
#include <stdint.h>

typedef struct ring_buffer {
    uint8_t *buffer_base;         /**< Pointer to the ring buffer. */
    int32_t buffer_size;          /**< Size of the ring buffer. */
    int32_t write;                /**< Index of the ring buffer to write the data. */
    int32_t read;                 /**< Index of the ring buffer to read the data. */
    int32_t (*set_buffer)(struct ring_buffer *ring_buff, void *buffer_base, int32_t length_in_byte);
    int32_t (*get_free_space)(struct ring_buffer *ring_buff, int32_t *free_space_in_byte);
    int32_t (*get_data_count)(struct ring_buffer *ring_buff, int32_t *data_count_in_byte);
    int32_t (*write_buffer)(struct ring_buffer *ring_buff, void *source_buffer, int32_t length_in_byte);
    int32_t (*read_buffer)(struct ring_buffer *ring_buff, void *dest_buffer, int32_t length_in_byte);
    int32_t (*get_write_buffer)(struct ring_buffer *ring_buff, uint8_t **buffer, int32_t *length_in_byte);
    int32_t (*get_read_buffer)(struct ring_buffer *ring_buff, uint8_t **buffer, int32_t *length_in_byte);
    int32_t (*write_data_done)(struct ring_buffer *ring_buff, int32_t length_in_byte);
    int32_t (*read_data_done)(struct ring_buffer *ring_buff, int32_t length_in_byte);
} ring_buffer_t;

int32_t ring_buffer_function_init(ring_buffer_t *ring_buff);
int32_t ring_buffer_function_deinit(ring_buffer_t *ring_buff);

#endif /* __RING_BUFFER_H__ */
