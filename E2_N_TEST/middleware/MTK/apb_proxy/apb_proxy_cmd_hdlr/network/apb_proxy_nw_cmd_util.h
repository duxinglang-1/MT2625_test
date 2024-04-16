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

#ifndef __APB_PROXY_NW_CMD_UTIL_H__
#define __APB_PROXY_NW_CMD_UTIL_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct _multi_cmd_package_info_ {
    char *total_cmd;
    uint32_t total_len;
    uint32_t saved_len;
} multi_cmd_package_info_t;

#define APB_NW_CMD_UTIL_SUCCESS         0
#define APB_NW_CMD_UTIL_PARAM_ERROR     -1
#define APB_NW_CMD_UTIL_MEMORY_ERROR    -2

#define APB_NW_CMD_FORMAT_INT           'i'
#define APB_NW_CMD_FORMAT_HEX           'h'
#define APB_NW_CMD_FORMAT_FLOAT         'f'
#define APB_NW_CMD_FORMAT_LENGTH        'l'
#define APB_NW_CMD_FORMAT_STRING        's'
#define APB_NW_CMD_FORMAT_CHAR          'c'

extern char* _itoa(int value, char* string, int radix);
extern int _atoi(const char *str);

char *parse_next_string(char* str, char** next_start);
char *parse_next_string_with_length(char* str, char** next_start, int length);
bool parse_length_and_string(char* str, char** next_start, int *p_length, char **result_str);
bool parse_length_and_copy_string(char* str, char** next_start, int *p_length, char **result_str);
int _integet_length(int value);

void* _alloc_buffer(int buffer_size);
void _free_buffer(void* buffer);

void _get_data_to_hex(char *out, uint8_t *in, int len);
void _get_data_from_hex(uint8_t* out, char* in, int len);

int32_t get_cmd_from_multi_pacakges_cmd(multi_cmd_package_info_t *p_info, char *cmd, char **result_cmd);
bool apb_nw_util_is_pure_int_string(char *str);
int32_t apb_nw_util_parse_all_parameters(char *cmd, char **next_start, char *format, ...);
bool apb_proxy_nw_cpy_without_quote(char *dst, char *src, int dst_buf_size);

char *apb_proxy_nw_parse_string_param(char **parse_str, char *parse_str_end);

#endif /* __APB_PROXY_NW_CMD_UTIL_H__ */

