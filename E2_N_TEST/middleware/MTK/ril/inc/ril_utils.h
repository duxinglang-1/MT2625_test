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

#ifndef __RIL_UTILS_H__
#define __RIL_UTILS_H__
#include "ril_general_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "ril.h"

/* AT cmd parameter pass*/
typedef enum {
    RIL_PARAM_TYPE_INTEGER,
    RIL_PARAM_TYPE_STRING,
    RIL_PARAM_TYPE_VOID,
    RIL_PARAM_TYPE_HEXINTEGER,
    RIL_PARAM_TYPE_DIGITAL_STRING,
    RIL_PARAM_TYPE_NONE
} ril_param_type_t;


typedef struct {
    ril_param_type_t data_type;
    union {
        int32_t val;
        char *str;
    } data;
} ril_param_desc_t;


typedef struct _ril_param_node_t {
    ril_param_desc_t param;
    struct _ril_param_node_t *next;
} ril_param_node_t;

/* AT cmd string parsing helper */
extern void skip_white_space(char **p_cur);
extern void skip_next_comma(char **p_cur);
extern int32_t at_tok_start(char **p_cur);
extern int32_t at_tok_nextint(char **p_cur, int32_t *p_out);
extern int32_t at_tok_nextbinint(char **p_cur, int32_t *p_out);
extern int32_t at_tok_nexthexint(char **p_cur, int32_t *p_out);
extern int32_t at_tok_nextbool(char **p_cur, char *p_out);
extern int32_t at_tok_nextstr(char **p_cur, char **out);
extern int32_t at_tok_hasmore(char **p_cur);
extern ril_param_type_t at_tok_next_param_type(char *p_cur);
extern int32_t at_tok_param_num(char *p_cur);
//extern int32_t at_tok_get_at_head(char *buf, char **head, uint32_t *head_len);
extern int32_t at_tok_get_at_head(char *buf, char **head, uint32_t *head_len, ril_request_mode_t *mode);

extern uint16_t at_caculate_hash_value(uint8_t *at_name, uint32_t head_len, uint32_t *hash_value1, uint32_t *hash_value2);

extern int32_t ril_param_list_create(ril_param_node_t **list_head);
extern int32_t ril_param_list_add_int(const ril_param_node_t *list_head, int32_t param);
extern int32_t ril_param_list_add_hexint(const ril_param_node_t *list_head, int32_t param);
extern int32_t ril_param_list_add_string(const ril_param_node_t *list_head, char *param, bool quotation_mark);
extern int32_t ril_param_list_add_void(const ril_param_node_t *list_head);

extern int32_t ril_param_list_get(ril_param_node_t **curr_node, ril_param_desc_t **param);
extern int32_t ril_param_list_get_total_num(ril_param_node_t *curr_node);
extern int32_t ril_param_list_delete(ril_param_node_t *list_head);

extern int32_t str_starts_with(const char *line, const char *prefix);
extern char *at_get_one_line(char **p_cur);
extern char *at_get_one_line2(char **p_cur);
extern char *at_get_next_packet(const char *buf);


extern int32_t is_final_response_success(const char *line, ril_result_code_t *res_code);
extern int32_t is_final_response_error(const char *line, ril_result_code_t *res_code);
extern int32_t is_final_response(const char *line, ril_result_code_t *res_code);
extern bool get_final_response(char *line, ril_result_code_t *res_code);
extern int32_t is_default_enabled_urc(char *line);
extern bool is_cmd_response(const char *line, ril_result_code_t *res_code);
extern bool is_cmd_echo(char *cmd_buf, char *cmd_item);

extern void ril_cmd_string_param_int_append(char *buf, int32_t param_int, bool prefix_comma);
extern void ril_cmd_string_param_hexint_append(char *buf, int32_t param_int, bool prefix_comma);
extern void ril_cmd_string_param_string_append(char *buf, char *param_str, bool prefix_comma);
extern void ril_cmd_string_param_digital_string_append(char *buf, char *param_str, bool prefix_comma);
extern void ril_cmd_string_param_void_append(char *buf, bool prefix_comma);



#define ril_mem_malloc(_wanted_size)    ril_mem_malloc_ext(_wanted_size, __FUNCTION__, __LINE__)
#define ril_mem_free(_mem_ptr)    ril_mem_free_ext(_mem_ptr, __FUNCTION__, __LINE__)
#define ril_mem_realloc(_mem_ptr, _wanted_size)    ril_mem_realloc_ext(_mem_ptr, _wanted_size, __FUNCTION__, __LINE__)
extern void *ril_mem_malloc_ext(uint32_t wanted_size, const char *func, uint32_t line);
extern void ril_mem_free_ext(void *mem_ptr, const char *func, uint32_t line);
extern void *ril_mem_realloc_ext(void *mem_ptr, uint32_t wanted_size, const char *func, uint32_t line);


#ifdef __cplusplus
}
#endif
#endif /* __RIL_UTILS_H__ */

