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


#include "ril_general_types.h"

#include "ril_utils.h"
#include "ril_cmds_def.h"
#include "ril_log.h"


#define NUM_ELEMS(x)    (sizeof(x)/sizeof((x)[0]))

#define AT_CMD_INVALID_HASH_VALUE    (0xffffffff)
#define AT_CMD_HASH_TABLE_ROW            (37)
#define AT_CMD_HASH_TABLE_SPAN           (5)
#define AT_CMD_MAX_CMD_HEAD_LEN          (2*AT_CMD_HASH_TABLE_SPAN)

#define CHAR_IS_LOWER( alpha_char )    ( ( (alpha_char >= 'a') && (alpha_char <= 'z') ) ?  1 : 0 )
#define CHAR_IS_UPPER( alpha_char )    ( ( (alpha_char >= 'A') && (alpha_char <= 'Z') ) ? 1 : 0 )
#define CHAR_IS_NUMBER( alpha_char )    ( ( (alpha_char >= '0') && (alpha_char <= '9') ) ? 1 : 0 )
#define CHAR_IS_SPACE(c)    (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')

typedef struct {
    const char *pattern;
    ril_result_code_t code;
} ril_result_mapping_item_t;


static const ril_result_mapping_item_t s_final_responses_success_pattern[] = {
    {"OK"            , RIL_RESULT_CODE_OK},
    {"CONNECT"       , RIL_RESULT_CODE_CONNECT},
    {"RING"          , RIL_RESULT_CODE_RING},
    {"CONNECT 1200"  , RIL_RESULT_CODE_INV},
    {"> "            , RIL_RESULT_CODE_INTERMEDIATE}
};

static const ril_result_mapping_item_t s_final_responses_error_pattern[] = {
    {"ERROR"         , RIL_RESULT_CODE_ERROR},
    {"+CME ERROR:"   , RIL_RESULT_CODE_CME_ERROR},
    {"+CMS ERROR:"   , RIL_RESULT_CODE_CMS_ERROR},
    {"NO CARRIER"    , RIL_RESULT_CODE_NO_CARRIER},
    {"NO ANSWER"     , RIL_RESULT_CODE_NO_ANSWER},
    {"NO DIALTONE"   , RIL_RESULT_CODE_NO_DIALTONE},
    {"BUSY"          , RIL_RESULT_CODE_BUSY},
    {"PROCEEDING"    , RIL_RESULT_CODE_PROCEEDING},
    {"+FCERROR"      , RIL_RESULT_CODE_FCERROR},
};

static const char *s_default_enabled_urc_pattern[] = {
    "+CPIN:",
    "+CMTI:",
    "*MATREADY:",
};


/**
 * Starts tokenizing an AT response string
 * returns -1 if this is not a valid response string, 0 on success.
 * updates *p_cur with current position
 */
int32_t at_tok_start(char **p_cur)
{
    char *ret;
    if (*p_cur == NULL) {
        return -1;
    }

    // skip prefix
    // consume "^[^:]:"

    //*p_cur = strchr(*p_cur, ':');
    ret = *p_cur;
    while (*ret != '\0' && *ret != ':' && *ret != '=') {
        ret++;
    }

    if (ret == NULL || *ret == '\0') {
        return -1;
    }

    *p_cur = ++ret;

    return 0;
}


int32_t at_tok_get_at_head(char *buf, char **head, uint32_t *head_len, ril_request_mode_t *mode)
{
    char *p_cur;
    bool mode_parse = false;
    if (buf == NULL || head == NULL || head_len == NULL) {
        return -1;
    }

    p_cur = buf;
    skip_white_space(&p_cur);
    if (str_starts_with(p_cur, "AT")) {
        p_cur += 2;
        mode_parse = true;
    }

    *head = p_cur;
    while (*p_cur != ':' && *p_cur != '\0' && *p_cur != '\r' && *p_cur != '\n' && *p_cur != '=' && *p_cur != '?') {
        p_cur++;
    }

    if ((*head_len = p_cur - *head) == 0) {
        return -1;
    }

    if (mode_parse && mode != NULL) {
        if (*p_cur == '\0' || *p_cur == '\r' || *p_cur == '\n') {
            *mode = RIL_ACTIVE_MODE;
        } else if (*p_cur == '?') {
            *mode = RIL_READ_MODE;
        } else if (*p_cur == '=') {
            if (*(p_cur + 1) == '?') {
                *mode = RIL_TEST_MODE;
            } else {
                *mode = RIL_EXECUTE_MODE;
            }
        }
    }
    return 0;
}


uint16_t at_caculate_hash_value(uint8_t *at_name, uint32_t head_len, uint32_t *hash_value1, uint32_t *hash_value2)
{
    uint16_t i = 0;
    char     ascii_char = 0;
    uint32_t value1 = 0;
    uint32_t value2 = 0;

    (*hash_value1) = AT_CMD_INVALID_HASH_VALUE;
    (*hash_value2) = AT_CMD_INVALID_HASH_VALUE;

    while (head_len) {
        ascii_char = AT_CMD_HASH_TABLE_ROW; /* for char '&' */
        if (CHAR_IS_UPPER(at_name[i])) {
            ascii_char = at_name[i] - 'A';
        } else if (CHAR_IS_LOWER(at_name[i])) {
            ascii_char = at_name[i] - 'a';
        } else if (CHAR_IS_NUMBER(at_name[i])) {
            ascii_char = at_name[i] - '0' + 'Z' - 'A' + 1;
        }
        if (i < (AT_CMD_HASH_TABLE_SPAN)) {
            value1 = value1 * (AT_CMD_HASH_TABLE_ROW + 1) + (ascii_char + 1); /* 0 ~ 4*/
        } else if (i < AT_CMD_MAX_CMD_HEAD_LEN) {
            value2 = value2 * (AT_CMD_HASH_TABLE_ROW + 1) + (ascii_char + 1); /* 5 ~ 9*/
        }

        head_len--;
        i++;
    }

    (*hash_value1) = value1;
    (*hash_value2) = value2;
    return 0;
}


void skip_white_space(char **p_cur)
{
    if (*p_cur == NULL) {
        return;
    }

    while (**p_cur != '\0' && CHAR_IS_SPACE(**p_cur)) {
        (*p_cur)++;
    }
}

void skip_next_comma(char **p_cur)
{
    if (*p_cur == NULL) {
        return;
    }

    while (**p_cur != '\0' && **p_cur != ',') {
        (*p_cur)++;
    }

    if (**p_cur == ',') {
        (*p_cur)++;
    }
}


void skip_next_cr_lf(char **p_cur)
{
    if (*p_cur == NULL) {
        return;
    }

    while (**p_cur != '\0' && **p_cur != '\r') {
        (*p_cur)++;
    }

    if ((**p_cur == '\r') && (*(*p_cur + 1) == '\n')) {
        (*p_cur) += 2;
    }
}


static char *next_tok(char **p_cur)
{
    char *ret = NULL;

    skip_white_space(p_cur);

    if (*p_cur == NULL) {
        ret = NULL;
    } else if (**p_cur == '"') {
        (*p_cur)++;
        ret = (char *)strsep(p_cur, "\"");
        skip_next_comma(p_cur);
    } else {
        ret = (char *)strsep(p_cur, ",");
    }

    return ret;
}


/**
 * Parses the next integer in the AT response line and places it in *p_out
 * returns 0 on success and -1 on fail
 * updates *p_cur
 * "base" is the same as the base param in strtol
 */

static int32_t at_tok_nextint_base(char **p_cur, int32_t *p_out, int32_t base, int32_t  uns)
{
    char *ret;

    if (*p_cur == NULL || **p_cur == '\0') {
        return -1;
    }

    ret = next_tok(p_cur);

    if (ret == NULL) {
        return -1;
    } else {
        long l;
        char *end;

        if (uns) {
            l = strtoul(ret, &end, base);
        } else {
            l = strtol(ret, &end, base);
        }

        *p_out = (int32_t)l;

        if (end == ret) {
            return -1;
        }
    }

    return 0;
}

/**
 * Parses the next base 10 integer in the AT response line
 * and places it in *p_out
 * returns 0 on success and -1 on fail
 * updates *p_cur
 */
int32_t at_tok_nextint(char **p_cur, int32_t *p_out)
{
    return at_tok_nextint_base(p_cur, p_out, 10, 0);
}

/**
 * Parses the next base 16 integer in the AT response line
 * and places it in *p_out
 * returns 0 on success and -1 on fail
 * updates *p_cur
 */
int32_t at_tok_nexthexint(char **p_cur, int32_t *p_out)
{
    return at_tok_nextint_base(p_cur, p_out, 16, 1);
}

/**
 * Parses the next base 16 integer in the AT response line
 * and places it in *p_out
 * returns 0 on success and -1 on fail
 * updates *p_cur
 */
int32_t at_tok_nextbinint(char **p_cur, int32_t *p_out)
{
    return at_tok_nextint_base(p_cur, p_out, 2, 1);
}


int32_t at_tok_nextbool(char **p_cur, char *p_out)
{
    int32_t ret;
    int32_t result;

    ret = at_tok_nextint(p_cur, &result);

    if (ret < 0) {
        return -1;
    }

    // booleans should be 0 or 1
    if (!(result == 0 || result == 1)) {
        return -1;
    }

    if (p_out != NULL) {
        *p_out = (char)result;
    }

    return ret;
}

int32_t at_tok_nextstr(char **p_cur, char **p_out)
{
    if (*p_cur == NULL || **p_cur == '\0') {
        return -1;
    }

    *p_out = next_tok(p_cur);

    return 0;
}

/** returns 1 on "has more tokens" and 0 if no */
int32_t at_tok_hasmore(char **p_cur)
{
    return ! (*p_cur == NULL || **p_cur == '\0');
}


ril_param_type_t at_tok_next_param_type(char *p_cur)
{
    if (p_cur == NULL) {
        return RIL_PARAM_TYPE_NONE;
    }

    while (*p_cur != '\0' && CHAR_IS_SPACE(*p_cur)) {
        p_cur++;
    }

    if (*p_cur == '\0') {
        return RIL_PARAM_TYPE_NONE;
    } else {
        return (*p_cur == '\"') ? RIL_PARAM_TYPE_STRING :
               CHAR_IS_NUMBER(*p_cur) ? RIL_PARAM_TYPE_INTEGER : RIL_PARAM_TYPE_STRING;
    }
}


// TODO: should consider a comma in " "
int32_t at_tok_param_num(char *p_cur)
{
    char *temp;
    int32_t param_num = 0;
    if (p_cur == NULL || *p_cur == '\0') {
        return 0;
    }

    temp = strchr(p_cur, ',');
    param_num++;
    while (temp != NULL) {
        param_num++;
        temp++;
        p_cur = temp;
        temp = strchr(p_cur, ',');
    }

    return param_num;
}


int32_t at_tok_nextint_by_cr_lf(char **p_cur, int32_t *p_out)
{
    char *ret;
    long l;
    char *end;
    if (p_cur == NULL || *p_cur == NULL) {
        return -1;
    }

    skip_white_space(p_cur);

    ret = strstr(*p_cur, "\r\n");
    if (ret == NULL) {
        return -1;
    }

    *ret++ = '\0';
    *ret++ = '\0';

    l = strtoul(*p_cur, &end, 10);
    *p_out = l;
    *p_cur = ret;
    if (ret == end) {
        return -1;
    }

    return 0;
}


int32_t str_starts_with(const char *line, const char *prefix)
{
    for ( ; *line != '\0' && *prefix != '\0' ; line++, prefix++) {
        if (*line != *prefix) {
            return 0;
        }
    }

    return *prefix == '\0';
}


int32_t is_final_response_success(const char *line, ril_result_code_t *res_code)
{
    size_t i;

    for (i = 0 ; i < NUM_ELEMS(s_final_responses_success_pattern) ; i++) {
        if (str_starts_with(line, s_final_responses_success_pattern[i].pattern)) {
            if (res_code) {
                *res_code = s_final_responses_success_pattern[i].code;
            }
            return 1;
        }
    }

    return 0;
}


int32_t is_final_response_error(const char *line, ril_result_code_t *res_code)
{
    size_t i;

    for (i = 0 ; i < NUM_ELEMS(s_final_responses_error_pattern) ; i++) {
        if (str_starts_with(line, s_final_responses_error_pattern[i].pattern)) {
            if (res_code) {
                *res_code = s_final_responses_error_pattern[i].code;
            }
            return 1;
        }
    }

    return 0;
}


int32_t is_final_response(const char *line, ril_result_code_t *res_code)
{
    if (is_final_response_success(line, res_code)) {
        return true;
    } else if (is_final_response_error(line, res_code)) {
        return true;
    } else {
        return false;
    }
}


bool get_final_response(char *line, ril_result_code_t *res_code)
{
    int32_t err_code;
    int32_t ret;
    char *cur_pos = line;
    if (is_final_response(cur_pos, res_code)) {
        if (*res_code == RIL_RESULT_CODE_CME_ERROR
                || *res_code == RIL_RESULT_CODE_CMS_ERROR) {

            ret = at_tok_start(&cur_pos);
            if (ret < 0) {
                goto error;
            }

            ret = at_tok_nextint(&cur_pos, &err_code);
            if (ret < 0) {
                goto error;
            }

            /* no need mapping err code, return directly */
            *res_code = (ril_result_code_t)(err_code);
        }
        return true;
    } else {
        return false;
    }

error:
    RIL_LOGE("invalid command string: %s\r\n", line);
    return false;
}


int32_t is_default_enabled_urc(char *line)
{
    size_t i;

    for (i = 0 ; i < NUM_ELEMS(s_default_enabled_urc_pattern); i++) {
        skip_white_space(&line);
        if (str_starts_with(line, s_default_enabled_urc_pattern[i])) {
            return 1;
        }
    }

    return 0;
}


bool is_cmd_response(const char *line, ril_result_code_t *res_code)
{
    char *p = NULL;
    char *prev_pos = NULL;
    char *cur_pos = (char *)line;
    //ril_result_code_t res_code;

    if (str_starts_with(cur_pos, "\r\n")) {
        cur_pos += 2; // skip the first 2 bytes <CR><LF>
        prev_pos = cur_pos;
    }
    p = strstr(cur_pos, "\r\n");
    while (p != NULL) {
        prev_pos = cur_pos;
        cur_pos = p + 2;
        p = strstr(cur_pos, "\r\n");
    }

    if (prev_pos) {
        if (is_final_response_success(prev_pos, res_code) ||
                is_final_response_error(prev_pos, res_code)) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}


bool is_cmd_head(const char *line)
{
    char *cur_pos = (char *)line;
    skip_white_space(&cur_pos);

    if ((*cur_pos != '+') && (*cur_pos != '*') && (*cur_pos != '^')) {
        return false;
    }

    cur_pos++;
    while (CHAR_IS_LOWER(*cur_pos) || CHAR_IS_UPPER(*cur_pos) || CHAR_IS_NUMBER(*cur_pos)) {
        cur_pos++;
    }

    return *cur_pos == ':';
}


bool is_cmd_echo(char *cmd_buf, char *cmd_head)
{    
    char echo_prefix[20];
    memset(echo_prefix, 0x00, 20);
    snprintf(echo_prefix, 20, "%s%s", "AT", cmd_head);
    if (str_starts_with(cmd_buf, echo_prefix)) {
        return true;
    } else {
        return false;
    }
}


/* find <CR><LF>, if the next string is final response or pattern like "+CXXXX:", return this string, <CR><LF> will be replaced with '\0' for parsing */
char *at_get_one_line(char **p_cur)
{
    char *ret = NULL;
    char *delim_pos = NULL;
    char *next_pos = NULL;
    bool delim_found = false;

    if (p_cur == NULL || *p_cur == NULL) {
        return NULL;
    }

    /* skip <CR><LF> */
    while (str_starts_with(*p_cur, "\r\n")) {
        *p_cur += 2;
    }

    if (*p_cur == NULL || **p_cur == '\0') {
        return NULL;
    }

    ret = *p_cur;

    do {
        delim_pos = strstr(*p_cur, "\r\n");
        if (delim_pos != NULL) {
            next_pos = delim_pos + 2;
            while (str_starts_with(next_pos, "\r\n")) {
                //delim_pos = next_pos;
                next_pos += 2;
            }
            if (is_cmd_head(next_pos) ||
                    is_final_response(next_pos, NULL) ||
                    *next_pos == '\0') {
                //*delim_pos++ = '\0';
                //*delim_pos++ = '\0';
                memset(delim_pos, 0x00, next_pos - delim_pos);
                delim_found = true;
            }
            *p_cur = next_pos;
        } else {
            /* cannot find <CR><LF>, it should be the last line */
            *p_cur = NULL;
        }
    } while (!delim_found && *p_cur != NULL);

    return ret;
}


/* <CR><LF> as delimiter */
char *at_get_one_line2(char **p_cur)
{
    char *ret = NULL;
    char *delim_pos = NULL;
    char *next_pos = NULL;
    bool delim_found = false;

    if (p_cur == NULL) {
        RIL_LOGE("wrong param\r\n");
        return NULL;
    }

    if (str_starts_with(*p_cur, "\r\n")) {
        *p_cur += 2;
    }

    if (*p_cur == NULL || **p_cur == '\0') {
        return NULL;
    }

    ret = *p_cur;

    do {
        delim_pos = strstr(*p_cur, "\r\n");
        if (delim_pos != NULL) {
            next_pos = delim_pos + 2;
            while (str_starts_with(next_pos, "\r\n")) {
                delim_pos = next_pos;
                next_pos += 2;
            }

            *delim_pos++ = '\0';
            *delim_pos++ = '\0';
            delim_found = true;

            *p_cur = next_pos;
        } else {
            /* cannot find <CR><LF>, it should be the last line */
            *p_cur = NULL;
        }
    } while (!delim_found && *p_cur != NULL);

    return ret;
}



// two <CR><LF> is delim
char *at_get_next_packet(const char *buf)
{
    char *delim = NULL;

    if (buf == NULL || *buf == '\0') {
        return NULL;
    }

    /* two continuous "\r\n" is the delimiter for different packets, the one is the tail of prev packet, the other is the head of following packet */
    delim = strstr(buf, "\r\n\r\n");
    if (delim == NULL) {
        return NULL;
    } else if (*(delim + 4) == '\0') {
        return NULL;
    } else {
        delim += 2;
        return delim;
    }
}


/**
 * @brief    create a parameter list to pass parameters to ril task.
 * @param[in,out] list_head    point to the address of the head of parameter list.
 * @return    0 mean success, <0 mean failure.
 */
int32_t ril_param_list_create(ril_param_node_t **list_head)
{
    ril_param_node_t *head;
    if (list_head == NULL) {
        return -1;
    }

    head = (ril_param_node_t *)ril_mem_malloc(sizeof(ril_param_node_t));
    if (head == NULL) {
        return -1;
    }
    head->param.data_type = RIL_PARAM_TYPE_NONE;
    head->param.data.val = 0;
    head->next = NULL;

    *list_head = head;
    return 0;
}



/**
 * @brief    add a integer parameter to list.
 * @param[in] list_head    point to the head of parameter list.
 * @param[in] param    integer value.
 * @return    0 mean success, <0 mean failure.
 */
int32_t ril_param_list_add_int(const ril_param_node_t *list_head, int32_t param)
{
    ril_param_node_t *node, *p;
    if (list_head == NULL) {
        return -1;
    }
    node = (ril_param_node_t *)ril_mem_malloc(sizeof(ril_param_node_t));
    if (node == NULL) {
        return -1;
    }
    node->param.data_type = RIL_PARAM_TYPE_INTEGER;
    node->param.data.val = param;
    node->next = NULL;

    p = (ril_param_node_t *)list_head;
    /* find list tail */
    while (p->next) {
        p = p->next;
    }

    p->next = node;
    return 0;
}


/**
 * @brief    add a integer parameter to list.
 * @param[in] list_head    point to the head of parameter list.
 * @param[in] param    integer value.
 * @return    0 mean success, <0 mean failure.
 */
int32_t ril_param_list_add_hexint(const ril_param_node_t *list_head, int32_t param)
{
    ril_param_node_t *node, *p;
    if (list_head == NULL) {
        return -1;
    }
    node = (ril_param_node_t *)ril_mem_malloc(sizeof(ril_param_node_t));
    if (node == NULL) {
        return -1;
    }
    node->param.data_type = RIL_PARAM_TYPE_HEXINTEGER;
    node->param.data.val = param;
    node->next = NULL;

    p = (ril_param_node_t *)list_head;
    /* find list tail */
    while (p->next) {
        p = p->next;
    }

    p->next = node;
    return 0;
}


/**
 * @brief    add a string parameter to list.
 * @param[in] list_head    point to the head of parameter list.
 * @param[in] param    point to the string.
 * @return    0 mean success, <0 mean failure.
 */
int32_t ril_param_list_add_string(const ril_param_node_t *list_head, char *param, bool quotation_mark)
{
    ril_param_node_t *node, *p;
    if (list_head == NULL) {
        return -1;
    }

    node = ril_mem_malloc(sizeof(ril_param_node_t) + strlen(param) + 1);
    if (node == NULL) {
        return -1;
    }
    node->param.data_type = quotation_mark ? RIL_PARAM_TYPE_STRING : RIL_PARAM_TYPE_DIGITAL_STRING;
    strcpy((char *)(node + 1), param);
    node->param.data.str = (char *)(node + 1);
    node->next = NULL;

    p = (ril_param_node_t *)list_head;
    /* find list tail */
    while (p->next) {
        p = p->next;
    }

    p->next = node;
    return 0;
}


int32_t ril_param_list_add_void(const ril_param_node_t *list_head)
{
    ril_param_node_t *node, *p;
    if (list_head == NULL) {
        return -1;
    }

    node = ril_mem_malloc(sizeof(ril_param_node_t));
    if (node == NULL) {
        return -1;
    }
    node->param.data_type = RIL_PARAM_TYPE_VOID;
    node->param.data.val = 0;
    node->next = NULL;

    p = (ril_param_node_t *)list_head;
    /* find list tail */
    while (p->next) {
        p = p->next;
    }

    p->next = node;
    return 0;
}


/**
 * @brief    get parameter from list.
 * @param[in,out] curr_node    point to the address of the pointer to current node, if get param success, will point to the address of the pointer to the next node.
 * @param[out] param         point to param's address if get success.
 * @return    0 mean success, <0 mean failure.
 */
int32_t ril_param_list_get(ril_param_node_t **curr_node, ril_param_desc_t **param)
{
    ril_param_node_t *p;
    if (curr_node == NULL || *curr_node == NULL || (*curr_node)->next == NULL) {
        return -1;
    }

    p = (*curr_node)->next;
    *param = &(p->param);
    *curr_node = p;
    return 0;
}


int32_t ril_param_list_get_total_num(ril_param_node_t *curr_node)
{
    int32_t param_num = 0;
    if (curr_node == NULL || curr_node->next == NULL) {
        return 0;
    }

    while (curr_node->next != NULL) {
        curr_node = curr_node->next;
        param_num++;
    }

    return param_num;
}

/**
 * @brief    delete parameter list.
 * @param[in] list_head    point to the head of parameter list.
 * @return    0 mean success, <0 mean failure.
 */
int32_t ril_param_list_delete(ril_param_node_t *list_head)
{
    ril_param_node_t *p, *del;
    if (list_head == NULL) {
        return -1;
    }

    p = list_head;
    while (p) {
        del = p;
        p = p->next;
        ril_mem_free(del);
    }

    return 0;
}


void ril_cmd_string_param_int_append(char *buf, int32_t param_int, bool prefix_comma)
{
    char str_buf[20]; /* 20 bytes is enough */
    memset(str_buf, 0x00, 20);
    if (prefix_comma) {
        snprintf(str_buf, 20, ",%d", (int)param_int);
    } else {
        snprintf(str_buf, 20, "%d", (int)param_int);
    }

    strcat(buf, str_buf);
}


void ril_cmd_string_param_hexint_append(char *buf, int32_t param_int, bool prefix_comma)
{
    char str_buf[20]; /* 20 bytes is enough */
    memset(str_buf, 0x00, 20);
    if (prefix_comma) {
        snprintf(str_buf, 20, ",%.8lx", param_int);
    } else {
        snprintf(str_buf, 20, "%.8lx", param_int);
    }

    strcat(buf, str_buf);
}


void ril_cmd_string_param_string_append(char *buf, char *param_str, bool prefix_comma)
{
    if (prefix_comma) {
        strcat(buf, ",\"");
    } else {
        strcat(buf, "\"");
    }
    strcat(buf, param_str);
    strcat(buf, "\"");
}


void ril_cmd_string_param_digital_string_append(char *buf, char *param_str, bool prefix_comma)
{
    if (prefix_comma) {
        strcat(buf, ",");
    }
    strcat(buf, param_str);
}


void ril_cmd_string_param_void_append(char *buf, bool prefix_comma)
{
    if (prefix_comma) {
        strcat(buf, ",");
    }
}


void *ril_mem_malloc_ext(uint32_t wanted_size, const char *func, uint32_t line)
{
    void *ptr = pvPortMalloc(wanted_size);
    if (ptr == NULL) {
        RIL_LOGMEM("fail to alloc buffer, wanted_size: %d, file: %s, line: %u", wanted_size, func, line);
        configASSERT(0);
        return NULL;
    } else {
        RIL_LOGMEM("alloc buffer, buf_size: %d, buf_ptr: 0x%x, file: %s, line: %u", wanted_size, ptr, func, line);
        return ptr;
    }
}


void ril_mem_free_ext(void *mem_ptr, const char *func, uint32_t line)
{
    if (mem_ptr != NULL) {
        RIL_LOGMEM("free buffer, buf_ptr: 0x%x, func: %s, line: %u", mem_ptr, func, line);
        vPortFree(mem_ptr);
    }
}


void *ril_mem_realloc_ext(void *mem_ptr, uint32_t wanted_size, const char *func, uint32_t line)
{
    void *ptr = pvPortRealloc(mem_ptr, wanted_size);
    if (ptr == NULL) {
        RIL_LOGMEM("fail to realloc buffer, wanted_size: %d, func: %s, line: %u", wanted_size, func, line);
        configASSERT(0);
        return NULL;
    } else {
        RIL_LOGMEM("realloc buffer, buf_size: %d, buf_ptr: 0x%x, ori_buf_ptr: 0x%x, func: %s, line: %u", wanted_size, ptr, mem_ptr, func, line);
        return ptr;
    }
}

