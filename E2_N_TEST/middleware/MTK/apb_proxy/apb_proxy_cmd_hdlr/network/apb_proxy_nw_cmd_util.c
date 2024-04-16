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

#include "FreeRTOS.h"
#include "apb_proxy_nw_cmd_util.h"
#include "syslog.h"
#include <string.h>
#include <stdlib.h>

log_create_module(apbnw, PRINT_LEVEL_INFO);

#ifdef APBSOC_MODULE_PRINTF
#define NW_LOG(fmt, ...)               printf("[APB UTIL] "fmt, ##__VA_ARGS__)
#else
#define NW_LOG(fmt, ...)               LOG_I(apbnw, "[APB UTIL] "fmt, ##__VA_ARGS__)
#endif

void* _alloc_buffer(int buffer_size)
{
    return pvPortCalloc(1, buffer_size);
}

void _free_buffer(void* buffer)
{
    if (buffer) {
        vPortFree(buffer);
        buffer = NULL;
    }
}

char* _itoa(int value, char* string, int radix)
{
    char zm[37] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char aa[10] = {0};

    int sum = value;
    char* cp = string;
    int i = 0;

    if (value < 0) {
        return string;
    } else if (0 == value) {
        char *res_str = "0";

        strncpy(string, res_str, strlen(res_str));
        return string;
    } else {
        while (sum > 0) {
            aa[i++] = zm[sum%radix];
            sum/=radix;
        }
    }

    for (int j=i-1; j>=0;j--) {
        *cp++ = aa[j];
    }

    *cp='\0';
    return string;
}


// TODO: _atof
// TODO: _ftoa

int _atoi(const char *str)
{
    int s = 0;
    int flag = 0;

    while (*str == ' ')
    {
        str++;
    }

    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            flag = 1;
         str++;
    }

    while (*str >= '0' && *str <= '9')
    {
        s = s*10 + (*str - '0');
        str++;
        if (s < 0) {
            s = 2147483647;
            break;
        }
    }

    return s * (flag ? -1 : 1);
}


int _integet_length(int value)
{
    int count = 0;
    while (value) {
        count++;
        value = value / 10;
    }

    return count;
}

char *parse_next_string(char* str, char** next_start)
{
    char *end_pos = 0;
    char *ret = NULL;
    int i;
    char temp;
    char *temp_pos = NULL;

    if (str == NULL) {
        *next_start = NULL;
        return NULL;
    }

    if (*str == '\"') {
        end_pos = strchr(str + 1, '\"');
        while (end_pos != NULL) {
            for (i = 1; ; i++) {
                temp = *(end_pos + i);
                if (temp != '\r' && temp != '\n' && temp != ' ') { // ignore
                    temp_pos = end_pos + i;
                    break;
                }
            }
            if (temp == ',' || temp == '\0') {
                *end_pos = '\0';
                ret = str + 1;
                if (temp == '\0') {
                    *next_start = NULL;
                } else {
                    *next_start = end_pos + i + 1;
                }
                break;
            } else {
                end_pos = strchr(end_pos + 1, '\"'); // the " is not follow a ',' or '\0', need search next
            }
        }
        if (end_pos != NULL) {
            *end_pos = '\0';
            ret = str + 1;
            if (temp != '\0') {
                *next_start = temp_pos + 1;
            } else {
                *next_start = NULL;
            }
        } else {
            *next_start = NULL;
            NW_LOG("Cannot parse string with \" from %s\n", str);
        }
    } else {
        end_pos = strchr(str, ',');
        if (end_pos != NULL) {
            *next_start = end_pos + 1;
            *end_pos = '\0';
        } else {
            *next_start = NULL;
            end_pos = str + strlen(str); // end_pos point to '\0'
            // remove the \r or \n at the end of the string
            for (i = end_pos - str - 1; i >= 0; i--) {
                if (*(str + i) != '\r' && *(str + i) != '\n') {
                    break;
                } else {
                    *(str + i) = '\0';
                    NW_LOG("remove the last \\r or \\n");
                }
            }
        }
        ret = str;
    }
    return ret;
}

char *parse_next_string_with_length(char* str, char** next_start, int length) {
    char *end_pos = NULL;

    if (str == NULL) {
        return NULL;
    }
    if (*str == '\"') {
        str = str + 1;
        if (*(str + length) == '\"') {
            end_pos = str + length + 1;
        } else {
            return NULL;
        }
    } else {
        end_pos = str + length;
    }
    if (*end_pos == ',') {
        *next_start = end_pos + 1;
    } else {
        *next_start = NULL;
    }

    *(str + length) = '\0';
    return str;

}

int32_t get_cmd_from_multi_pacakges_cmd(multi_cmd_package_info_t *p_info, char *cmd, char **result_cmd)
{
    char *start  = cmd;
    char *temp = NULL;
    int32_t ret = APB_NW_CMD_UTIL_PARAM_ERROR;
    int32_t current_total_len = 0;
    int32_t current_len = 0;
    int32_t flag = 0;
    char *current_package = NULL;


    if (p_info == NULL || cmd == NULL || result_cmd == NULL) {
        return APB_NW_CMD_UTIL_PARAM_ERROR;
    }
    *result_cmd = NULL;
    // flag
    temp = parse_next_string(start, &start);
    if (temp == NULL) {
        NW_LOG("Cannot get flag string");
        goto exit;
    }
    flag = atoi(temp);

    // total_len
    temp = parse_next_string(start, &start);
    if (temp == NULL) {
        NW_LOG("Cannot get total_len string");
        goto exit;
    }
    current_total_len = atoi(temp);

    // current package len
    temp = parse_next_string(start, &start);
    if (temp == NULL) {
        NW_LOG("Cannot get current_len string");
        goto exit;
    }
    current_len = atoi(temp);

    if (current_len > strlen(start)) {
        NW_LOG("Error! The length indicated by user is larger than incoming raw data");
        goto exit;
    }

    // current package
    temp = parse_next_string_with_length(start, &start, current_len);
    if (temp == NULL) {
        NW_LOG("Cannot get current_package string");
        goto exit;
    }
    current_package = temp;

    if (current_total_len <= 0 ||
        current_len < 0 ||
        current_len + p_info->saved_len > current_total_len) {
        NW_LOG("param error");
        goto exit;
    }

    if (current_len < current_total_len) {
        if (p_info->total_cmd && current_total_len == p_info->total_len && p_info->saved_len > 0) {

        } else if (!p_info->total_cmd && p_info->saved_len == 0 && p_info->total_len == 0 && flag == 1) {
            p_info->total_cmd = _alloc_buffer(current_total_len + 1);
            if (p_info->total_cmd) {
                p_info->total_len = current_total_len;
            } else {
                NW_LOG("Memory error");
                goto exit;
            }
        } else {
            NW_LOG("param error");
            goto exit;
        }
        memcpy(p_info->total_cmd + p_info->saved_len, current_package, current_len);
        p_info->saved_len += current_len;
    }

    if (flag == 1) {
        ret = APB_NW_CMD_UTIL_SUCCESS;
    } else {
        if (p_info->total_cmd && p_info->saved_len == p_info->total_len) {
            *result_cmd = p_info->total_cmd;
            ret = APB_NW_CMD_UTIL_SUCCESS;

        } else if (!p_info->total_cmd) {
            *result_cmd = current_package;
            ret = APB_NW_CMD_UTIL_SUCCESS;
        } else {
            NW_LOG("param error");
        }
    }
    exit:
    return ret;
}

void _get_data_to_hex(char *out, uint8_t *in, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        sprintf(out + (i * 2), "%02x", in[i]);
    }
}


void _get_data_from_hex(uint8_t* out, char* in, int len)
{
    int i;
    unsigned long c;
    char temp[3] = {0};

    for (i = 0; i < len; i++) {
        memcpy(temp, in + i * 2, 2);
        c = strtoul(temp, 0, 16);
        out[i] = (char)c;
    }
}

bool apb_nw_util_is_pure_int_string(char *str)
{
    if (str) {
        while (*str != '\0') {
            if (*str < '0' || *str > '9') {
                return false;
            }
            str++;
        }
        return true;
    } else {
        return false;
    }
}

int32_t apb_nw_util_parse_all_parameters(char *cmd, char **next_start, char *format, ...)
{
    uint32_t i;
    uint32_t param_count;
    uint32_t result_count = 0;
    va_list va;
    int32_t *p_int;
    uint32_t *p_hex;
    float *p_float;
    char *p_char;
    uint32_t *p_length;
    char **p_string;
    int32_t temp_length = -1;
    char *temp;

    if (format == NULL || cmd == NULL) {
        NW_LOG("Error! format or cmd is NULL");
        goto exit;
    }
    param_count = (strlen(format) + 1) / 2;
    va_start(va, format);

    for (i = 0; i < param_count ; i++) {

        if (cmd == NULL) {
            break;
        }
        if (temp_length >= 0) {
            temp = parse_next_string_with_length(cmd, &cmd, temp_length);
            temp_length = -1;
        } else {
            temp = parse_next_string(cmd, &cmd);
        }
        switch (*(format + 2 * i)) {
            case APB_NW_CMD_FORMAT_INT:
                p_int = va_arg(va, int32_t *);
                if (temp) {
                    if (!apb_nw_util_is_pure_int_string(temp)) {
                        result_count = -1;
                        goto exit;
                    }
                    if (p_int) {
                        *p_int = atoi(temp);
                    }
                    result_count ++;
                }
                break;
            case APB_NW_CMD_FORMAT_FLOAT:
                p_float = va_arg(va, float *);
                if (temp) {
                    if (p_float) {
                        *p_float = atof(temp);
                    }
                    result_count ++;
                }
                break;
            case APB_NW_CMD_FORMAT_CHAR:
                p_char = va_arg(va, char *);
                if (temp) {
                    if (p_char) {
                        *p_char = *temp;
                    }
                    result_count ++;
                }
                break;
            case APB_NW_CMD_FORMAT_HEX:
                p_hex = va_arg(va, uint32_t *);
                if (temp) {
                    if (p_hex) {
                        sscanf(temp, "%X", (unsigned int *)p_hex);
                    }
                    result_count ++;
                }
                break;
            case APB_NW_CMD_FORMAT_LENGTH:
                p_length =  va_arg(va, uint32_t *);
                if (temp) {
                    temp_length = atoi(temp);
                    if (p_length) {
                        *p_length = temp_length;
                    }
                    result_count ++;
                }
                break;
            case APB_NW_CMD_FORMAT_STRING:
                p_string = va_arg(va, char **);
                if (temp != NULL) {
                    if (p_string) {
                        *p_string = temp;
                    }
                    result_count ++;
                }
                break;
            default:
                break;
        }
    }
    va_end(va);
    exit:
    if (next_start) {
        *next_start = cmd;
    }
    return result_count;
}


bool apb_proxy_nw_cpy_without_quote(char *dst, char *src, int dst_buf_size)
{
    int src_len = 0, i = 0;

    if (!src || !dst || !dst_buf_size)
    {
        return false;
    }

    src_len = strlen(src);

    /* Check format */
    if ('\"' != src[0])
    {
        return false;
    }

    /* Find the 2nd " */
    i = src_len - 1;
    while (i >= 0 && (src[i] == '\r' || src[i] == '\n'))
    {
        i--;
    }

    if (i <= 0 || src[i] != '\"' || dst_buf_size < i)
    {
        return false;
    }

    strncpy(dst, src + 1, i - 1);
    dst[i - 1] = '\0';
    if (strchr(dst, '\"'))
    {
        dst[0] = '\0';
        return false;
    }
    else
    {
        return true;
    }
}


char *apb_proxy_nw_parse_string_param(char **parse_str, char *parse_str_end)
{
    char *param = NULL, *param_str_tmp = NULL;

    if (!parse_str || !*(parse_str) || !parse_str_end ||
        *parse_str > parse_str_end)
    {
        return NULL;
    }

    param_str_tmp = *parse_str;

    /* Skip the first " */
    param = ++param_str_tmp;

    while (param_str_tmp <= parse_str_end &&
           !(*(param_str_tmp - 1) != '\\' && *param_str_tmp == '"' &&
           (*(param_str_tmp + 1) == ',' || *(param_str_tmp + 1) == '\0' ||
            *(param_str_tmp + 1) == '\r' || *(param_str_tmp + 1) == '\n')))
    {
        param_str_tmp++;
    }

    NW_LOG("line: %d", __LINE__);

    if (param_str_tmp <= parse_str_end &&
        (*(param_str_tmp - 1) != '\\' && *param_str_tmp == '"' &&
        (*(param_str_tmp + 1) == ',' || *(param_str_tmp + 1) == '\0' ||
         *(param_str_tmp + 1) == '\r' || *(param_str_tmp + 1) == '\n')))
    {
        /* Clear the second " */
        *param_str_tmp = '\0';
        if (*(param_str_tmp + 1) == ',')
        {
            param_str_tmp += 2;
        }
        else
        {
            param_str_tmp = NULL;
        }

        *parse_str = param_str_tmp;
        return param;
    }
    else
    {
        NW_LOG("Invalid format");
    }

    return NULL;
}

