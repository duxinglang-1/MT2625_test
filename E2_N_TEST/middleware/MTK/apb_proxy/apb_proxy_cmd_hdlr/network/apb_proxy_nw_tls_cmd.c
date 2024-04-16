/* Copyright Statement:
 *
 * (C) 2005-2017 MediaTek Inc. All rights reserved.
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
 * the License Agreement ("Permitted User"). If you are not a Permitted User,
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

#include "apb_proxy_nw_tls_cmd.h"
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#endif

#if TLS_AT_CMD_DEBUG
log_create_module(apb_tls, PRINT_LEVEL_INFO);
#endif


#if !defined(MBEDTLS_BIGNUM_C) || !defined(MBEDTLS_ENTROPY_C) ||  \
    !defined(MBEDTLS_SSL_TLS_C) || !defined(MBEDTLS_SSL_CLI_C) || \
    !defined(MBEDTLS_NET_C) || !defined(MBEDTLS_RSA_C) ||         \
    !defined(MBEDTLS_CERTS_C) || !defined(MBEDTLS_PEM_PARSE_C) || \
    !defined(MBEDTLS_CTR_DRBG_C) || !defined(MBEDTLS_X509_CRT_PARSE_C)

#ifdef TLS_AT_CMD_SUPPORT
#undef TLS_AT_CMD_SUPPORT
#endif

#else
#include "tel_conn_mgr_bearer_util.h"
#include "task_def.h"
#include "mbedtls/net.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
//#include "ethernetif.h"
//#include "sockets.h"
//#include "netdb.h"
//#include <string.h>
#include <task.h>
#include <queue.h>
#include "apb_proxy_nw_cmd_util.h"

#ifdef MBEDTLS_BASE64_C
#include "mbedtls/base64.h"
#endif

#define TLS_AT_CMD_SUPPORT

#endif


#define TLS_AT_CMD_INT_STRING_MAX_LEN (10)


#ifdef TLS_AT_CMD_SUPPORT

typedef enum
{
    APB_PROXY_TLS_STATE_NONE = 0x0,
    APB_PROXY_TLS_STATE_CONNECTING = 0x01,
    APB_PROXY_TLS_STATE_CONNECTED = 0x02,
    APB_PROXY_TLS_STATE_SENDING = 0x04,
    APB_PROXY_TLS_STATE_RECEIVING = 0x08,
    APB_PROXY_TLS_STATE_CLOSING = 0x10,

    APB_PROXY_TLS_STATE_MAX = 0xFF
}apb_proxy_tls_state_enum;


typedef enum
{
    APB_PROXY_TLS_NONE = 0,
    APB_PROXY_TLS_SVR_CA_SET = 0x1,
    APB_PROXY_TLS_SVR_CA_COMPLETE = 0x2,
    APB_PROXY_TLS_CLI_CERT_SET = 0x4,
    APB_PROXY_TLS_CLI_CERT_COMPLETE = 0x8,
    APB_PROXY_TLS_CLI_PKEY_SET = 0x10,
    APB_PROXY_TLS_CLI_PKEY_COMPLETE = 0x20
}apb_proxy_tls_cert_key_status_enum;


typedef enum
{
    APB_PROXY_TLS_PARAM_TYPE_NONE = 0,
    APB_PROXY_TLS_PARAM_TYPE_HOST_NAME,
    APB_PROXY_TLS_PARAM_TYPE_PORT,
    APB_PROXY_TLS_PARAM_TYPE_SOC_TYPE,
    APB_PROXY_TLS_PARAM_TYPE_AUTH_MODE,
    APB_PROXY_TLS_PARAM_TYPE_DEBUG_LEVEL,
    APB_PROXY_TLS_PARAM_TYPE_SERVER_CA,
    APB_PROXY_TLS_PARAM_TYPE_CLIENT_CERT,
    APB_PROXY_TLS_PARAM_TYPE_CLIENT_PRIVATE_KEY,
    
    APB_PROXY_TLS_PARAM_TYPE_MAX
}apb_proxy_tls_param_type_enum;


#ifdef TLS_AT_CMD_SUPPORT_DATA_MODE
typedef enum
{
    APB_PROXY_TLS_DATA_MODE_TYPE_NONE,
    APB_PROXY_TLS_DATA_MODE_TYPE_SERVER_CA,
    APB_PROXY_TLS_DATA_MODE_TYPE_CLIENT_CERT,
    APB_PROXY_TLS_DATA_MODE_TYPE_CLIENT_PRIVATE_KEY,
    APB_PROXY_TLS_DATA_MODE_TYPE_SEND_DATA,
    APB_PROXY_TLS_DATA_MODE_TYPE_RECV_DATA,

    APB_PROXY_TLS_DATA_MODE_TYPE_MAX
}apb_proxy_tls_data_mode_type_enum;
#endif

/* The value range for valid encode type should not overlap to apb_proxy_tls_param_type_enum */
typedef enum
{
    APB_PROXY_TLS_ENCODE_TYPE_NONE = 800,
    APB_PROXY_TLS_ENCODE_TYPE_STRING,
    APB_PROXY_TLS_ENCODE_TYPE_HEX,
    APB_PROXY_TLS_ENCODE_TYPE_BASE64,

    APB_PROXY_TLS_ENCODE_TYPE_DEFAULT = APB_PROXY_TLS_ENCODE_TYPE_STRING,

    APB_PROXY_TLS_ENCODE_TYPE_MAX = 0xffff
}apb_proxy_tls_encode_type_enum;


typedef enum
{
    APB_PROXY_TLS_OP_NONE,
    APB_PROXY_TLS_OP_CONN,
    APB_PROXY_TLS_OP_SEND,
    APB_PROXY_TLS_OP_READ,
    APB_PROXY_TLS_OP_CLOSE,
    APB_PROXY_TLS_OP_CONN_HANDSHAKE,
    
    APB_PROXY_TLS_OP_MAX
}apb_proxy_tls_operation_enum;


typedef struct apb_proxy_tls_template
{
    struct apb_proxy_tls_template *next;
    int tid;
}apb_proxy_tls_template_struct;



/* Used only in APB PROXY TLS task. */
typedef struct apb_proxy_tls_conn
{
    struct apb_proxy_tls_conn *next;
    int tid;
    int cid;
    apb_proxy_tls_state_enum state;
    mbedtls_ssl_context ssl_ctx;    /* mbedtls ssl context */
    mbedtls_net_context net_ctx;    /* Fill in socket id */
    mbedtls_ssl_config ssl_conf;    /* SSL configuration */
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    //mbedtls_x509_crt_profile profile;
    mbedtls_x509_crt cacert;    /* AppleHomekit CA */
    mbedtls_x509_crt clicert;   /* Accessory Certification */
    mbedtls_pk_context pkey;    /* Accessory LTSK */
}apb_proxy_tls_conn_struct;

#define APB_PROXY_TLS_HOST_NAME_MAX_LEN  (63)
#define APB_PROXY_TLS_PORT_MAX_LEN (5)

/* Used both in APB PROXY task and APB PROXY TLS task. */
typedef struct apb_proxy_tls_param
{
    struct apb_proxy_tls_param *next;
    int tid;
    int is_using;
    char host_name[APB_PROXY_TLS_HOST_NAME_MAX_LEN + 1];
    char port[APB_PROXY_TLS_PORT_MAX_LEN + 1];
    char soc_type;
    char auth_mode;
    char debug_level;
    char *svr_ca;   // server ca
    unsigned int svr_ca_size;    // the size of svr_ca -1. When svr_ca is reused, it may not equal to <size> in ETLSCFG
    unsigned int svr_ca_buff_size;
    char *cli_cert;    // client certificate
    unsigned int cli_cert_size;    // the size of cli_cert -1. When cli_cert is reused, it may not equal to <size> in ETLSCFG
    unsigned int cli_cert_buff_size;
    char *cli_pkey;    // client private key
    unsigned int cli_pkey_size;    // the size of cli_pkey -1. When cli_pkey is reused, it may not equal to <size> in ETLSCFG
    unsigned int cli_pkey_buff_size;
    int cert_key_status;
}apb_proxy_tls_param_struct;


/* Avoid string copy when setting cert or key. */
typedef struct
{
    int size;
    char more;
    char *cert;
    size_t cert_len;
    apb_proxy_tls_encode_type_enum encode_type;
}apb_proxy_tls_param_cert_key_struct;


#define APB_PROXY_SERVER_PORT  "443"

#define APB_PROXY_TLS_QUEUE_MAX_SIZE (5)





#define APB_PROXY_TLS_MSG_ID_BASE    (0xE000)

#define APB_PROXY_TLS_MSG_ID_OP_REQ    (APB_PROXY_TLS_MSG_ID_BASE + 1)


typedef struct
{
    unsigned int msg_id;
    int tid;
    int cid;
    apb_proxy_tls_operation_enum op;
    char *data;
    int len;
    apb_proxy_tls_encode_type_enum encode_type;  /* encode type for received data. */
#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
    int time_out;
#endif
    apb_proxy_cmd_id_t cmd_id;
}apb_proxy_tls_msg_struct;


typedef struct
{
    TaskHandle_t task_hdl;
    QueueHandle_t queue_hdl;
    SemaphoreHandle_t mutex_hdl;
    apb_proxy_tls_conn_struct *tls_conn_list;
#ifdef TLS_AT_CMD_SUPPORT_DATA_MODE
    apb_proxy_data_conn_id_t data_mode_conn_id;
    int data_mode_tid;
    apb_proxy_tls_data_mode_type_enum data_mode_type;
    apb_proxy_cmd_id_t data_mode_cmd_id;
    apb_proxy_cmd_id_t data_mode_data_len;
#endif
} apb_proxy_tls_cntx_struct;


apb_proxy_tls_cntx_struct g_apb_tls_cntx = {0};
apb_proxy_tls_cntx_struct *g_apb_tls_cntx_ptr = NULL;

static apb_proxy_tls_param_struct *g_apb_tls_param_list = NULL;


#define APB_PROXY_TLS_MUTEX_LOCK {do \
{ if (g_apb_tls_cntx_ptr && g_apb_tls_cntx_ptr->mutex_hdl) \
    while (xSemaphoreTake(g_apb_tls_cntx_ptr->mutex_hdl, portMAX_DELAY) != pdPASS); \
} while (0);}
#define APB_PROXY_TLS_MUTEX_UNLOCK {do \
{ if (g_apb_tls_cntx_ptr && g_apb_tls_cntx_ptr->mutex_hdl) \
  xSemaphoreGive(g_apb_tls_cntx_ptr->mutex_hdl); \
} while (0);}


apb_proxy_tls_state_enum apb_proxy_tls_conn_get_state(int tid);
void apb_proxy_tls_conn_clear_state(int tid, apb_proxy_tls_state_enum state);
void apb_proxy_tls_conn_set_state(int tid, apb_proxy_tls_state_enum state);


/* -------------------Common APIs------------------- */
#define APB_PROXY_TLS_COMMON_API
bool apb_proxy_tls_is_valid_tid(int tid)
{
    if (1 <= tid && 255 >= tid)
    {
        return true;
    }

    TLS_AT_CMD_LOG_INFO("tid:%d not valid", tid);
    return false;
}


bool apb_proxy_tls_list_insert(apb_proxy_tls_template_struct **list, apb_proxy_tls_template_struct *list_node)
{
    apb_proxy_tls_template_struct *list_node_tmp = NULL;
        
    if (!list || !list_node || list_node->next)
    {
        TLS_AT_CMD_LOG_INFO("%s Invalid param", __FUNCTION__);
        return false;
    }    
    
    if (!(*list))
    {
        *list = list_node;
    }
    else
    {
        list_node_tmp = *list;
        while (list_node_tmp->next)
        {
            list_node_tmp = list_node_tmp->next;
        }

        list_node_tmp->next = list_node;                
    }

    TLS_AT_CMD_LOG_INFO("Insert succeed");
    return true;    
}


/* The node will not be freed in this API. */
bool apb_proxy_tls_list_remove(apb_proxy_tls_template_struct **list, apb_proxy_tls_template_struct *list_node)
{
    apb_proxy_tls_template_struct *list_node_tmp = NULL;

    if (!list || !(*list) || !list_node)
    {
        TLS_AT_CMD_LOG_INFO("%s Invalid Param", __FUNCTION__);
        return false;
    }

    if (*list == list_node)
    {
        *list = (*list)->next;
    }
    else
    {
        list_node_tmp = *list;
        while (list_node_tmp && list_node_tmp->next != list_node)
        {
            list_node_tmp = list_node_tmp->next;
        }

        if (list_node_tmp)
        {
            list_node_tmp->next = list_node->next;
        }
        else
        {
            TLS_AT_CMD_LOG_INFO("%s Not Found", __FUNCTION__);
            return false;
        }
    }

    list_node->next = NULL;
    
    return true;
}


apb_proxy_tls_template_struct *apb_proxy_tls_list_find(apb_proxy_tls_template_struct *list,
                                                       int tid)
{
    apb_proxy_tls_template_struct *list_node = list;

    while (list_node)
    {
        if (list_node->tid == tid)
        {
            return list_node;
        }

        list_node = list_node->next;
    }
    
    TLS_AT_CMD_LOG_ERR("%s not found. tid:%d",
        ((apb_proxy_tls_template_struct *)g_apb_tls_param_list == list) ? "tls_info" : "tls_conn",
        tid);
    return NULL;
}


bool apb_proxy_is_printable_char(char chr)
{
	if ((chr >= ' ' && chr <= '~'))
	{
		return true;
	}

	return false;
}


#if 1
typedef struct
{
	char character;
	char letter;
}ende_letter_escape_struct;


ende_letter_escape_struct letter_escape_table[]= {
    {'\a', 'a'},
    {'\b', 'b'},
    {'\f', 'f'},
    {'\n', 'n'},
    {'\r', 'r'},
    {'\t', 't'},
    {'\v', 'v'},
    {'\\', '\\'},
    {'\'', '\''},
    {'\"', '\"'},
    {'\?', '?'},
};


bool apb_proxy_is_digital(char character)
{
    if ('0' <= character && '9' >= character)
    {
        return true;
    }

    return false;
}


bool apb_proxy_is_oct(char character)
{
    if ('0' <= character && '7' >= character)
    {
        return true;
    }

    return false;
}


bool apb_proxy_is_hex(char character)
{
    if (apb_proxy_is_digital(character) ||
        ('A' <= character && 'F' >= character) ||
        ('a' <= character && 'f' >= character))
    {
        return true;
    }

    return false;
}


char apb_proxy_is_letter_escape_char(char character)
{
    int i = 0, table_item_num = sizeof(letter_escape_table) / sizeof(ende_letter_escape_struct);

    while (i < table_item_num && character != letter_escape_table[i++].character);

    return i < table_item_num ? letter_escape_table[i - 1].letter : 0;
}


char apb_proxy_is_letter_escape_letter(char letter)
{
    int i = 0, table_item_num = sizeof(letter_escape_table) / sizeof(ende_letter_escape_struct);

    while (i < table_item_num && letter != letter_escape_table[i++].letter);

    return i < table_item_num ? letter_escape_table[i - 1].character : 0;
}


int apb_proxy_chara_oct_escape_encode(char character,
                                                  char *escape_buf,
                                                  int escape_buf_size,
                                                  int *out_escape_buf_size,
                                                  bool is_next_digital)
{
    char _1st = 0, _2nd = 0, _3th = 0;

    if (!out_escape_buf_size)
    {
        return -1;
    }

    _3th = character % 8 + '0';
    _2nd = (character / 8) % 8 + '0';
    _1st = (character / 8) / 8 + '0';

    if (is_next_digital)
    {
        *out_escape_buf_size = 3;
        if (escape_buf && escape_buf_size >= 3)
        {
            escape_buf[0] = _1st;
            escape_buf[1] = _2nd;
            escape_buf[2] = _3th;
            return 0;
        }
        else if (!escape_buf || !escape_buf_size)
        {
            return 0;
        }
    }
    else
    {
        *out_escape_buf_size = 3;
        if ('0' == _1st)
        {
            (*out_escape_buf_size)--;
        }

        if ('0' == _2nd)
        {
            (*out_escape_buf_size)--;
        }

        if (escape_buf && escape_buf_size >= *out_escape_buf_size)
        {
            int i = 0;
            if ('0' != _1st)
            {
                escape_buf[i++] = _1st;
            }

            if ('0' != _2nd)
            {
                escape_buf[i++] = _2nd;
            }

            escape_buf[i++] = _3th;
            return 0;
        }
        else if (!escape_buf || !escape_buf_size)
        {
            return 0;
        }            
    }

    return -2;
}


/* Return the number of bytes oct_escape presentation ocuppied. */
int apb_proxy_chara_oct_escape_decode(char *character,
                                                  char *src,
                                                  unsigned int src_len)
{
    unsigned int i = 0, value = 0;
    char oct_str[4] = {0};
        
    if (!src || !src_len || !character)
    {
        return -1;
    }

    for (i = 0; i < 3 && i < src_len; i++)
    {
        if (!apb_proxy_is_oct(src[i]))
        {
            break;
        }
        oct_str[i] = src[i] - '0';
    }

    if (3 == i)
    {
        value = oct_str[0] * 8 * 8 + oct_str[1] * 8 + oct_str[2];

        if (value <= 255)
        {
            *character = value;
            return 3;
        }
        else
        {
            i--;
        }
    }

    if (2 == i)
    {
        value = oct_str[0] * 8 + oct_str[1];
        *character = value;
        return 2;
    }

    if (1 == i)
    {
        *character = oct_str[0];
        return 1;
    }

    return 0;
}


/* Return the number of bytes oct_escape presentation ocuppied. */
int apb_proxy_chara_hex_escape_decode(char *character,
                                                  char *src,
                                                  unsigned int src_len)
{
    unsigned int i = 0, value = 0;
    char hex_str[3] = {0};
        
    if (!src || !src_len || !character)
    {
        return -1;
    }

    for (i = 0; i < 2 && i < src_len; i++)
    {
        if (!apb_proxy_is_hex(src[i]))
        {
            break;
        }
        hex_str[i] = apb_proxy_is_digital(src[i]) ? (src[i] - '0') : (('A' <= src[i] && 'F' >= src[i]) ? (src[i] - 'A' + 10) : (src[i] - 'a' + 10));
    }

    if (2 == i)
    {
        value = hex_str[0] * 16 + hex_str[1];
        *character = value;
        return 2;
    }

    if (1 == i)
    {
        *character = hex_str[0];
        return 1;
    }

    return 0;
}


int apb_proxy_tls_ascii_escape_encode(char *dst, size_t dlen, size_t *olen,
                                                const char *src, size_t slen)
{
	unsigned int i = 0, j = 0;
	int ret = 0;
    char letter = 0;
	
	if (!olen || !src || !slen)
	{
		return -1;
	}

	for (i = 0; i < slen && src[i] && (!dlen || j < dlen); i++)
	{
		if (apb_proxy_is_printable_char(src[i]))
		{
			if (dst && dlen)
			{
				dst[j++] = src[i];
			}
			else
			{
				j++;
			}			
		}
        else
        {
            letter = apb_proxy_is_letter_escape_char(src[i]);
            if (0 != letter)
            {
                if (dst && dlen && j + 1 < dlen)
                {
                    dst[j++] = '\\';
                    dst[j++] = letter;
                }
                else if (!dst || !dlen)
                {
                    j += 2;
                }
                else
                {
                    break;
                }
            }
            else
            {
                char escape_buf[4] = {0};
                int escape_buf_size = 4;
                ret = apb_proxy_chara_oct_escape_encode(src[i],
                                                  escape_buf,
                                                  escape_buf_size,
                                                  &escape_buf_size,
                                                  (i + 1 < slen) ? apb_proxy_is_digital(src[i + 1]) : false);
                if (0 == ret)
                {
                    if (dst && j + escape_buf_size < dlen)
                    {
                        dst[j++] = '\\';
                        memcpy((void *)&dst[j], escape_buf, escape_buf_size);
                        j += escape_buf_size;
                    }
                    else if (!dst || !dlen)
                    {
                        j += (escape_buf_size + 1);
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    ret = -3;
                    break;
                }
            }
        }        
	}

	if (0 == ret)
	{
		if (!dst || !dlen)
		{
			*olen = j + 1;
		}
		else
		{
			*olen = j;
		}

        return i;
	}

	return ret;
}


int apb_proxy_tls_ascii_escape_decode(char *dst, size_t dlen, size_t *olen,
                                                const char *src, size_t slen)
{
    unsigned int i = 0, j = 0;
    int ret = 0;
    
    if (!olen || !src || !slen)
    {
        return -1;
    }

    for (i = 0; i < slen && src[i] && (!dlen || j < dlen); i++)
    {
    	if (src[i] == '\r' || src[i] == '\n')
		{
			continue;
		}
		
        if (src[i] == '\\' && i + 1 < slen)
        {
            char character = apb_proxy_is_letter_escape_letter(src[i + 1]);
            if (0 != character)
            {
                if (dst && dlen)
                {
                    dst[j++] = character;
                }
                else
                {
                    j++;
                }
                i++;
            }
            else if('x' == src[i + 1] || apb_proxy_is_oct(src[i + 1]))
            {
                if (apb_proxy_is_oct(src[i + 1]))
                {
                    int oct_escape_len = apb_proxy_chara_oct_escape_decode(&character,
                                                                           (char *)&src[i + 1],
                                                                           slen - (i + 1));
                    if (oct_escape_len > 0)
                    {
                        if (dst && dlen)
                        {
                            dst[j++] = character;
                        }
                        else
                        {
                            j++;
                        }
                        i += oct_escape_len;
                    }
                    else if (oct_escape_len == 0)
                    {
                        if (dst && dlen)
                        {
                            dst[j++] = src[i];
                        }
                        else
                        {
                            j++;
                        }
                    }
                    else
                    {
                        ret = -2;
                        break;
                    }
                }
                else
                {
                    int hex_escape_len = apb_proxy_chara_hex_escape_decode(&character,
                                                                           (char *)&src[i + 2],
                                                                           slen - (i + 2));
                    if (hex_escape_len > 0)
                    {
                        if (dst && dlen)
                        {
                            dst[j++] = character;
                        }
                        else
                        {
                            j++;
                        }
                        i += (hex_escape_len + 1);
                    }
                    else if (hex_escape_len == 0)
                    {
                        if (dst && dlen)
                        {
                            dst[j++] = src[i];
                        }
                        else
                        {
                            j++;
                        }
                    }
                    else
                    {
                        ret = -3;
                        break;
                    }
                }
            }
            else
            {
                if (dst && dlen)
                {
                    dst[j++] = src[i];
                }
                else
                {
                    j++;
                }
            }
        }
        else
        {
            if (dst && dlen)
            {
                dst[j++] = src[i];
            }
            else
            {
                j++;
            }
        }
    }

    if (0 == ret)
    {
        if (!dst || !dlen)
        {
            *olen = j + 1;
        }
        else
        {
            *olen = j;
        }

        return i;
    }

    return ret;
}
#endif


int apb_proxy_tls_string_decode(char *dst, size_t dlen, size_t *olen,
                                 const char *src, size_t slen)
{
	unsigned int i = 0, j = 0;
	int ret = 0;
	
	if (!olen || !src || !slen)
	{
		return -1;
	}

	for (i = 0; i < slen && (!dlen || j < dlen); i++)
	{
		if (!apb_proxy_is_printable_char(src[i]) &&
			(src[i] != '\r' && src[i] != '\n'))
		{
			break;
		}

		/*if (src[i] == '\r' || src[i] == '\n')
		{
			continue;
		}*/		

		if (src[i] == '\\' && 
			(i + 1 < slen && (src[i + 1] == 'r' || src[i + 1] == 'n' || src[i + 1] == '\\')))
		{
			if (dst && dlen)
			{
				if (src[i + 1] == 'r')
				{
					dst[j++] = '\r';
				}
				else if (src[i + 1] == 'n')
				{
					dst[j++] = '\n';
				}
				else
				{
					dst[j++] = '\\';
				}
			}
			else
			{
				j++;
			}

			i++;
		}		
		else
		{
			if (dst && dlen)
			{
				dst[j++] = src[i];
			}
			else
			{
				j++;
			}
		}
	}

	if (i >= slen)
	{
		if (!dst || !dlen)
		{
			*olen = j + 1;
		}
		else
		{
			*olen = j;
		}

		return 0;
	}

	return ret;
}


int apb_proxy_tls_string_encode(char *dst, size_t dlen, size_t *olen,
                                 const char *src, size_t slen)
{
	unsigned int i = 0, j = 0;
	int ret = 0;
	
	if (!olen || !src || !slen)
	{
		return -1;
	}

	for (i = 0; i < slen && (!dlen || j < dlen); i++)
	{
		if (!apb_proxy_is_printable_char(src[i]) &&
			(src[i] != '\r' && src[i] != '\n'))
		{
			break;
		}

		if (src[i] == '\\' && (i + 1 < slen && (src[i + 1] == 'r' || src[i + 1] == 'n')))
		{
			if (dst && (j + 1 < dlen))
			{
				dst[j++] = '\\';
				dst[j++] = '\\';
			}
			else if (!dst || !dlen)
			{
				j += 2;
			}
			else
			{
				ret = -2;
				break;
			}
		}
		else if (src[i] == '\r')
		{
			if (dst && (j + 1 < dlen))
			{
				dst[j++] = '\\';
				dst[j++] = 'r';
			}
			else if (!dst || !dlen)
			{
				j += 2;
			}
			else
			{
				ret = -3;
				break;
			}
		}
		else if (src[i] == '\n')
		{
			if (dst && (j + 1 < dlen))
			{
				dst[j++] = '\\';
				dst[j++] = 'n';
			}
			else if (!dst || !dlen)
			{
				j += 2;
			}
			else
			{
				ret = -4;
				break;
			}
		}
		else
		{
			if (dst && j < dlen)
			{
				dst[j++] = src[i];
			}
			else if (!dst || !dlen)
			{
				j++;
			}
			else
			{
				ret = -5;
				break;
			}
		}
	}

	if (i >= slen)
	{
		if (!dst || !dlen)
		{
			*olen = j + 1;
		}
		else
		{
			*olen = j;
		}

		return 0;
	}

	return ret;
}


bool apb_proxy_tls_char2hex(char *dst , char src)
{
    if ('a' <= src && 'f' >= src)
    {
        *dst = src - 'a' + 0x0A;
        return true;
    }

    if ('A' <= src && 'F' >= src)
    {
        *dst = src - 'A' + 0x0A;
        return true;
    }

    if ('0' <= src && '9' >= src)
    {
        *dst = src - '0';
        return true;
    }

    return false;
}


/* When dlen is 0 or dst is NULL, return the memory size required in olen which will include the last NULL-terminator. 
  * olen: number of bytes written. */
int apb_proxy_tls_hex_decode(char *dst, size_t dlen, size_t *olen,
                                     const char *src, size_t slen)
{
    bool ret = false;
    unsigned int i = 0, j = 0;
    char hhex = 0, lhex = 0;

    if (!olen || !src || !slen || slen % 2)
    {
        return -1;
    }

    if (!dst || !dlen)
    {
        *olen = slen / 2 + 1;
        return 0;
    }

    for (i = 0, j = 0; i < slen; i++)
    {
        ret = apb_proxy_tls_char2hex(&hhex, src[i++]);
        if (!ret)
        {
            *olen = j;
            return -2;
        }
        ret = apb_proxy_tls_char2hex(&lhex, src[i]);
        if (!ret)
        {
            *olen = j;
            return -3;
        }
        dst[j++] = (hhex & 0x0F) << 4 | (lhex & 0x0F);
    }

    *olen = j;
    return 0;
}


/* When dlen is 0 or dst is NULL, return the memory size required in olen which will include the last NULL-terminator. 
  * When olen returns the actual size of the encoded content, it does not include the last NULL-terminator.
  */
int apb_proxy_tls_hex_encode(char *dst, size_t dlen, size_t *olen,
                                     const char *src, size_t slen)
{
    unsigned int i = 0;

    if (!olen || !src || !slen || (dlen && dlen < slen * 2 + 1))
    {
        return -1;
    }

    if (!dlen || !dst)
    {
        *olen = slen * 2 + 1;
        return 0;
    }

    for (i = 0; i < slen && (i * 2 + 1 < dlen); i++)
    {
        dst[i * 2] = ((src[i] & 0xF0) >= 0xA0) ? (((src[i] & 0xF0) >> 4) - 0x0A + 'A') : (((src[i] & 0xF0) >> 4) + '0');
        dst[i * 2 + 1] = ((src[i] & 0x0F) >= 0x0A) ? ((src[i] & 0x0F) - 0x0A + 'A') : ((src[i] & 0x0F) + '0');
    }

    *olen = i * 2;
    return 0;
}


/* When the type is STRING, no new buffer is allocated. */
int apb_proxy_tls_decode(char *dst, size_t dlen, size_t *olen,
                         const char *src, size_t slen,
                         apb_proxy_tls_encode_type_enum encode_type)
{
    int ret = -1;

    if (!src || !slen || !olen ||
        (APB_PROXY_TLS_ENCODE_TYPE_STRING != encode_type &&
         APB_PROXY_TLS_ENCODE_TYPE_BASE64 != encode_type &&
         APB_PROXY_TLS_ENCODE_TYPE_HEX != encode_type))
    {
        TLS_AT_CMD_LOG_INFO("Invalid param. encode_type: %d", encode_type);
        return -1;
    }

    if (APB_PROXY_TLS_ENCODE_TYPE_STRING == encode_type)
    {
    #if 1
        ret = apb_proxy_tls_ascii_escape_decode(dst,
                                                dlen,
                                                  olen,
                                                  src,
                                                  slen);

            ret = ret > 0 ? 0 : -1;
    #else
        ret = apb_proxy_tls_string_decode(dst,

                                          dlen,
                                          olen,
                                          src,
                                          slen);
    #endif
    }
    else if (APB_PROXY_TLS_ENCODE_TYPE_HEX == encode_type)
    {
        ret = apb_proxy_tls_hex_decode(dst,
                                       dlen,
                                       olen,
                                       src,
                                       slen);
    } 
    else if (APB_PROXY_TLS_ENCODE_TYPE_BASE64 == encode_type)
    {
#ifdef MBEDTLS_BASE64_C
        ret = mbedtls_base64_decode((unsigned char*)dst,
                                    dlen,
                                    olen,
                                    (const unsigned char*)src,
                                    slen);
        if ((!dst || !dlen) && MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL == ret)
        {
            ret = 0;
            (*olen)++;
        }
#endif
    }

    if (0 == ret && dst && dlen)
    {
        if (dlen > *olen)
        {
            dst[*olen] = '\0';
        }
        else
        {
            ret = -2;
        }
    }

    return ret;
}


int apb_proxy_tls_encode(char *dst, size_t dlen, size_t *olen,
                                const char *src, size_t slen,
                                apb_proxy_tls_encode_type_enum encode_type)
{
    int ret = -1;

    if (!src || !slen || !olen ||
        (APB_PROXY_TLS_ENCODE_TYPE_STRING != encode_type &&
         APB_PROXY_TLS_ENCODE_TYPE_BASE64 != encode_type &&
         APB_PROXY_TLS_ENCODE_TYPE_HEX != encode_type))
    {
        TLS_AT_CMD_LOG_INFO("Invalid param. encode_type: %d", encode_type);
        return -1;
    }

    /* Encode */
    if (APB_PROXY_TLS_ENCODE_TYPE_STRING == encode_type)
    {
    #if 1
        ret = apb_proxy_tls_ascii_escape_encode(dst,
                                                dlen,
                                                olen,
                                                src,
                                                slen);
        ret = ret > 0 ? 0 : -1;
    #else
        ret = apb_proxy_tls_string_encode(dst,
                                          dlen,
                                          olen,
                                          src,
                                          slen);
    #endif    
    }
    else if (APB_PROXY_TLS_ENCODE_TYPE_HEX == encode_type)
    {
        ret = apb_proxy_tls_hex_encode(dst,
                                       dlen,
                                       olen,
                                       src,
                                       slen);
    } 
    else if (APB_PROXY_TLS_ENCODE_TYPE_BASE64 == encode_type)
    {
#ifdef MBEDTLS_BASE64_C
        ret = mbedtls_base64_encode((unsigned char*)dst,
                                    dlen,
                                    olen,
                                    (const unsigned char*)src,
                                    slen);
        if ((!dst || !dlen) && MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL == ret)
        {
            ret = 0;
        }
#endif
    }

    if (0 == ret && dst && dlen)
    {
        if (dlen > *olen)
        {
            dst[*olen] = '\0';
        }
        else
        {
            ret = -2;
        }
    }

    return ret;
}


bool apb_proxy_tls_send_msg(int tid,
                                    apb_proxy_tls_operation_enum op,                                    
                                    int cid,
                                    char *data,
                                    int len,
                                    apb_proxy_tls_encode_type_enum encode_type,
#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
                                    int time_out,
#endif
                                    apb_proxy_cmd_id_t cmd_id)
{
    apb_proxy_tls_msg_struct *msg = NULL;

    if (!g_apb_tls_cntx_ptr ||
        !apb_proxy_tls_is_valid_tid(tid) ||
        APB_PROXY_TLS_OP_NONE >= op ||
        APB_PROXY_TLS_OP_MAX <= op ||
        (APB_PROXY_TLS_OP_SEND == op && (!data || !len)) ||
        (APB_PROXY_TLS_OP_READ== op && !len) ||
        (APB_PROXY_TLS_OP_READ == op &&
         APB_PROXY_TLS_ENCODE_TYPE_STRING != encode_type &&
         APB_PROXY_TLS_ENCODE_TYPE_HEX != encode_type &&
         APB_PROXY_TLS_ENCODE_TYPE_BASE64 != encode_type))
    {
        TLS_AT_CMD_LOG_WARN("Invalid param. tid:%d, op:%d, encode_type:%d.", tid, op, encode_type);
        return false;
    }

    msg = pvPortCalloc(1, sizeof(apb_proxy_tls_msg_struct));
    if (!msg)
    {
        TLS_AT_CMD_LOG_WARN("Not enough memory");
        return false;
    }

    msg->msg_id = APB_PROXY_TLS_MSG_ID_OP_REQ;
    msg->tid = tid;
    msg->op = op;
    msg->data = data;
    msg->len = len;
    msg->encode_type = encode_type;
#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
    msg->time_out = time_out;
#endif
    msg->cmd_id = cmd_id;

    if (xQueueSend(g_apb_tls_cntx_ptr->queue_hdl, (void *)&msg, (TickType_t)0) != pdPASS)
    {
        vPortFree(msg);
        return false;
    }

    TLS_AT_CMD_LOG_INFO("Succeed to send msg. len:%d, op:%d.", msg->len, msg->op);

    return true;
}


/* -------------------tls_param APIs------------------- */
#define APB_PROXY_TLS_PARAM_API

void apb_proxy_tls_free_cert_key(apb_proxy_tls_param_struct *tls_param,
                                        apb_proxy_tls_param_type_enum type)
{
    if (!tls_param)
    {
        return;
    }

    switch ((int)type)
    {
        case APB_PROXY_TLS_PARAM_TYPE_SERVER_CA:
        {
            if (tls_param->svr_ca)
            {
                vPortFree(tls_param->svr_ca);
                tls_param->svr_ca = NULL;
            }

            tls_param->svr_ca_buff_size = 0;
            tls_param->cert_key_status &= ~(APB_PROXY_TLS_SVR_CA_COMPLETE | APB_PROXY_TLS_SVR_CA_SET);
            break;
        }

        case APB_PROXY_TLS_PARAM_TYPE_CLIENT_CERT:
        {
            if (tls_param->cli_cert)
            {
                vPortFree(tls_param->cli_cert);
                tls_param->cli_cert = NULL;
            }

            tls_param->cli_cert_buff_size = 0;
            tls_param->cert_key_status &= ~(APB_PROXY_TLS_CLI_CERT_COMPLETE | APB_PROXY_TLS_CLI_CERT_SET);
            break;
        }

        case APB_PROXY_TLS_PARAM_TYPE_CLIENT_PRIVATE_KEY:
        {
            if (tls_param->cli_pkey)
            {
                vPortFree(tls_param->cli_pkey);
                tls_param->cli_pkey = NULL;
            }

            tls_param->cli_pkey_buff_size = 0;
            tls_param->cert_key_status &= ~(APB_PROXY_TLS_CLI_PKEY_COMPLETE | APB_PROXY_TLS_CLI_PKEY_SET);
            break;
        }

        default:
            break;
    }
}


void apb_proxy_tls_reset_cert_key(apb_proxy_tls_param_struct *tls_param,
                                          apb_proxy_tls_param_type_enum type,
                                          unsigned cert_pkey_size)
{
    if (!tls_param)
    {
        return;
    }

    switch ((int)type)
    {
        case APB_PROXY_TLS_PARAM_TYPE_SERVER_CA:
        {
            if (tls_param->svr_ca && tls_param->svr_ca_buff_size >= cert_pkey_size)
            {
                memset(tls_param->svr_ca, 0, tls_param->svr_ca_buff_size + 1);
                tls_param->svr_ca_size = cert_pkey_size;
                tls_param->cert_key_status &= ~APB_PROXY_TLS_SVR_CA_COMPLETE;
                tls_param->cert_key_status |= APB_PROXY_TLS_SVR_CA_SET;
            }
            break;
        }

        case APB_PROXY_TLS_PARAM_TYPE_CLIENT_CERT:
        {
            if (tls_param->cli_cert && tls_param->cli_cert_buff_size >= cert_pkey_size)
            {
                memset(tls_param->cli_cert, 0, tls_param->cli_cert_buff_size + 1);
                tls_param->cli_cert_size = cert_pkey_size;
                tls_param->cert_key_status &= ~APB_PROXY_TLS_CLI_CERT_COMPLETE;
                tls_param->cert_key_status |= APB_PROXY_TLS_CLI_CERT_SET;
            }
            break;
        }

        case APB_PROXY_TLS_PARAM_TYPE_CLIENT_PRIVATE_KEY:
        {
            if (tls_param->cli_pkey && tls_param->cli_pkey_buff_size >= cert_pkey_size)
            {
                memset(tls_param->cli_pkey, 0, tls_param->cli_pkey_buff_size + 1);
                tls_param->cli_pkey_size = cert_pkey_size;
                tls_param->cert_key_status &= ~APB_PROXY_TLS_CLI_PKEY_COMPLETE;
                tls_param->cert_key_status |= APB_PROXY_TLS_CLI_PKEY_SET;
            }
            break;
        }

        default:
            break;
    }
}


char *apb_proxy_tls_create_cert_key(apb_proxy_tls_param_struct *tls_param,
                                           apb_proxy_tls_param_type_enum type,
                                           unsigned int size)
{
    char *cert_key_buf = NULL;
    
    if (!tls_param || !size)
    {
        return NULL;
    }

    switch ((int)type)
    {
        case APB_PROXY_TLS_PARAM_TYPE_SERVER_CA:
        {
            if (tls_param->svr_ca)
            {
                return NULL;
            }

            tls_param->svr_ca = pvPortCalloc(1, size + 1);
            if (!tls_param->svr_ca)
            {
                return NULL;
            }

            cert_key_buf = tls_param->svr_ca;
            tls_param->svr_ca_buff_size = size;
            tls_param->svr_ca_size = size;
            tls_param->cert_key_status &= ~APB_PROXY_TLS_SVR_CA_COMPLETE;
            tls_param->cert_key_status |= APB_PROXY_TLS_SVR_CA_SET;
            break;
        }

        case APB_PROXY_TLS_PARAM_TYPE_CLIENT_CERT:
        {
            if (tls_param->cli_cert)
            {
                return NULL;
            }

            tls_param->cli_cert = pvPortCalloc(1, size);
            if (!tls_param->cli_cert)
            {
                return NULL;
            }
            
            cert_key_buf = tls_param->cli_cert;
            tls_param->cli_cert_buff_size = size;
            tls_param->cli_cert_size = size;
            tls_param->cert_key_status &= ~APB_PROXY_TLS_CLI_CERT_COMPLETE;
            tls_param->cert_key_status |= APB_PROXY_TLS_CLI_CERT_SET;
            break;
        }

        case APB_PROXY_TLS_PARAM_TYPE_CLIENT_PRIVATE_KEY:
        {
            if (tls_param->cli_pkey)
            {
                return NULL;
            }

            tls_param->cli_pkey = pvPortCalloc(1, size);
            if (!tls_param->cli_pkey)
            {
                return NULL;
            }

            cert_key_buf = tls_param->cli_pkey;
            tls_param->cli_pkey_buff_size = size;
            tls_param->cli_pkey_size = size;
            tls_param->cert_key_status &= ~APB_PROXY_TLS_CLI_PKEY_COMPLETE;
            tls_param->cert_key_status |= APB_PROXY_TLS_CLI_PKEY_SET;
            break;
        }

        default:
            return NULL;
    }

    return cert_key_buf;
}


#ifdef TLS_AT_CMD_SUPPORT_DATA_MODE
void apb_proxy_tls_data_mode_event_callback(apb_proxy_event_type_t event, void *pdata)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    static int recv_len = 0;

    configASSERT(pdata != NULL);
    cmd_result.result_code = APB_PROXY_RESULT_OK;
    
    switch (event) {
        case APB_PROXY_USER_DATA_IND: {
            apb_proxy_user_data_t *p_user_data = (apb_proxy_user_data_t *)(pdata);
            
            char *buf = NULL;
            int buf_len = 0, buf_size = 0;
            
            if (!g_apb_tls_cntx_ptr || !g_apb_tls_cntx_ptr->data_mode_conn_id)
            {
                TLS_AT_CMD_LOG_INFO("Invalid state. data_mode_conn_id:%d", g_apb_tls_cntx_ptr ? g_apb_tls_cntx_ptr->data_mode_conn_id : -1);
                break;
            }

            if (APB_PROXY_TLS_DATA_MODE_TYPE_SERVER_CA == g_apb_tls_cntx_ptr->data_mode_type ||
                APB_PROXY_TLS_DATA_MODE_TYPE_CLIENT_CERT == g_apb_tls_cntx_ptr->data_mode_type ||
                APB_PROXY_TLS_DATA_MODE_TYPE_CLIENT_PRIVATE_KEY == g_apb_tls_cntx_ptr->data_mode_type)
            {
                apb_proxy_tls_param_struct *tls_param = NULL;

                tls_param = (apb_proxy_tls_param_struct *)apb_proxy_tls_list_find((apb_proxy_tls_template_struct *)g_apb_tls_param_list, g_apb_tls_cntx_ptr->data_mode_tid);
                if (!tls_param)
                {
                    TLS_AT_CMD_LOG_INFO("tls_param was not found, tid:%d", g_apb_tls_cntx_ptr->data_mode_tid);
                    break;
                }

                TLS_AT_CMD_LOG_INFO("tid:%d, type:%d, cmd_id:%d",
                                    g_apb_tls_cntx_ptr->data_mode_tid,
                                    g_apb_tls_cntx_ptr->data_mode_type,
                                    g_apb_tls_cntx_ptr->data_mode_cmd_id);

#ifdef TLS_AT_CMD_LOG_DUMP_MEM
                {
                    char *pbuf = p_user_data->pbuffer;
                    int i = 0, len = p_user_data->length;

                    while (i < len)
                    {
                        if (len - i > 10)
                        {
                            TLS_AT_CMD_LOG_INFO("%x, %x, %x, %x, %x, %x, %x, %x, %x, %x\r\n",
                                pbuf[i], pbuf[i + 1], pbuf[i + 2], pbuf[i + 3], pbuf[i + 4],
                                pbuf[i + 5], pbuf[i + 6], pbuf[i + 7], pbuf[i + 8], pbuf[i + 9]);
                            i += 10;
                        }
                        else
                        {
                            TLS_AT_CMD_LOG_INFO("%x, %x, %x, %x, %x, %x, %x, %x, %x, %x\r\n",
                                pbuf[i],
                                i + 1 < len ? pbuf[i + 1] : 0xFF,
                                i + 2 < len ? pbuf[i + 2] : 0xFF,
                                i + 3 < len ? pbuf[i + 3] : 0xFF,
                                i + 4 < len ? pbuf[i + 4] : 0xFF,
                                i + 5 < p_user_data->length ? pbuf[i + 5] : 0xFF,
                                i + 6 < p_user_data->length ? pbuf[i + 6] : 0xFF,
                                i + 7 < p_user_data->length ? pbuf[i + 7] : 0xFF,
                                i + 8 < p_user_data->length ? pbuf[i + 8] : 0xFF,
                                i + 9 < p_user_data->length ? pbuf[i + 9] : 0xFF);
                            break;
                        }
                    }
                }
#endif

                if (APB_PROXY_TLS_DATA_MODE_TYPE_SERVER_CA == g_apb_tls_cntx_ptr->data_mode_type)
                {
                    buf = tls_param->svr_ca;
                    buf_size = tls_param->svr_ca_size;
                }
                else if (APB_PROXY_TLS_DATA_MODE_TYPE_CLIENT_CERT == g_apb_tls_cntx_ptr->data_mode_type)
                {
                    buf = tls_param->cli_cert;
                    buf_size = tls_param->cli_cert_size;
                }
                else
                {
                    buf = tls_param->cli_pkey;
                    buf_size = tls_param->cli_pkey_size;
                }

                if (!buf)
                {
                    break;
                }

                buf_len = strlen((const char*)buf);

                if (buf_len + p_user_data->length > buf_size)
                {
                    // TODO: terminate data mode
                    TLS_AT_CMD_LOG_INFO("Buffer size:%d does not match data total length:%d.", buf_size, buf_len + p_user_data->length);
                    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                }
                else
                {
                    memcpy(buf + buf_len, p_user_data->pbuffer, p_user_data->length);
                    TLS_AT_CMD_LOG_INFO("Expected size:%d Current length:%d", buf_size, strlen((const char*)buf));

                    if (buf_len + p_user_data->length == buf_size)
                    {
                        /* Exit data mode. */
                        TLS_AT_CMD_LOG_INFO("Data has been received. length:%d", strlen((const char*)buf));
                        cmd_result.result_code = APB_PROXY_RESULT_NO_CARRIER;
                    }
                }
                
            }
            else if (APB_PROXY_TLS_DATA_MODE_TYPE_SEND_DATA == g_apb_tls_cntx_ptr->data_mode_type)
            {
                apb_proxy_tls_state_enum tls_conn_state = apb_proxy_tls_conn_get_state(g_apb_tls_cntx_ptr->data_mode_tid);
                bool ret = false;
                char *data = pvPortCalloc(1, p_user_data->length + 1);
                static int data_len = 0;

                if (!data)
                {
                    // TODO: terminate data mode
                    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                }
                else
                {
                    memcpy(data, p_user_data->pbuffer, p_user_data->length);
                    data_len += p_user_data->length;
                    if (data_len == g_apb_tls_cntx_ptr->data_mode_data_len)
                    {
                        cmd_result.result_code = APB_PROXY_RESULT_NO_CARRIER;
                    }
                    apb_proxy_tls_conn_set_state(g_apb_tls_cntx_ptr->data_mode_tid, APB_PROXY_TLS_STATE_SENDING);

                    ret = apb_proxy_tls_send_msg(g_apb_tls_cntx_ptr->data_mode_tid,
                                                 APB_PROXY_TLS_OP_SEND,                                         
                                                 0,
                                                 data,
                                                 p_user_data->length,
                                                 APB_PROXY_TLS_ENCODE_TYPE_NONE,
#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
                                                 0,
#endif
                                                 0);
                    if (!ret)
                    {
                        apb_proxy_tls_conn_clear_state(g_apb_tls_cntx_ptr->data_mode_tid, APB_PROXY_TLS_STATE_SENDING);
                        cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                        vPortFree(data);
                    }
                }
            }

            if (APB_PROXY_RESULT_OK != cmd_result.result_code)
            {
                cmd_result.pdata = NULL;
                cmd_result.length = 0;
                cmd_result.cmd_id = g_apb_tls_cntx_ptr->data_mode_cmd_id;
                configASSERT(apb_proxy_close_data_mode(g_apb_tls_cntx_ptr->data_mode_conn_id) == APB_PROXY_STATUS_OK);
                configASSERT(APB_PROXY_STATUS_OK == apb_proxy_send_at_cmd_result(&cmd_result));
                /* Lazy free */
                g_apb_tls_cntx_ptr->data_mode_conn_id = 0;
            }
            break;
        }
        case APB_PROXY_STOP_SEND_USER_DATA: {
            break;
        }
        case APB_PROXY_RESUME_SEND_USER_DATA: {
            break;
        }
        default: {
            configASSERT(0);
            break;
        }
    }
}


apb_proxy_tls_ret_enum apb_proxy_tls_cert_key_set_size(apb_proxy_tls_param_struct *tls_param,
                                             apb_proxy_tls_param_type_enum type,
                                             int size,
                                             apb_proxy_cmd_id_t cmd_id)
{
    char *cert_key_buf = NULL;
    unsigned int cert_key_buff_size = 0, cert_key_size = 0;
    apb_proxy_tls_cert_key_status_enum cert_key_complete = APB_PROXY_TLS_NONE;
    int ret_int = 0;

    TLS_AT_CMD_LOG_INFO("type:%d. size:%d", type, size);

    if (!tls_param)
    {
        return APB_PROXY_TLS_RET_INVALID_PARAM;
    }

    if (!g_apb_tls_cntx_ptr)
    {
        return APB_PROXY_TLS_RET_WRONG_STATE;
    }

    switch ((int)type)
    {
        case APB_PROXY_TLS_PARAM_TYPE_SERVER_CA:
        {
            cert_key_buf = tls_param->svr_ca;
            cert_key_buff_size = tls_param->svr_ca_buff_size;
            cert_key_size = tls_param->svr_ca_size;
            cert_key_complete = APB_PROXY_TLS_SVR_CA_COMPLETE;
            break;
        }

        case APB_PROXY_TLS_PARAM_TYPE_CLIENT_CERT:
        {
            cert_key_buf = tls_param->cli_cert;
            cert_key_buff_size = tls_param->cli_cert_buff_size;
            cert_key_size = tls_param->cli_cert_size;
            cert_key_complete = APB_PROXY_TLS_CLI_CERT_COMPLETE;
            break;
        }

        case APB_PROXY_TLS_PARAM_TYPE_CLIENT_PRIVATE_KEY:
        {
            cert_key_buf = tls_param->cli_pkey;
            cert_key_buff_size = tls_param->cli_pkey_buff_size;
            cert_key_size = tls_param->cli_pkey_size;
            cert_key_complete = APB_PROXY_TLS_CLI_PKEY_COMPLETE;
            break;
        }

        default:
            return APB_PROXY_TLS_RET_INVALID_PARAM;
    }

    if (!size)
    {
        /* Delete the server ca certificate. */
        if (cert_key_buf)
        {
            apb_proxy_tls_free_cert_key(tls_param, type);
        }
        else
        {
           TLS_AT_CMD_LOG_WARN("Remove unexisted cert/pkey.");
        }
        return APB_PROXY_TLS_RET_OK;
    }

    /* Check if it's a new certificate. */
    if (cert_key_buf &&
        (cert_key_size != size ||
         (tls_param->cert_key_status & cert_key_complete)))
    {
        if (size > cert_key_buff_size)
        {
            /* Need to allocate a new buffer */
            apb_proxy_tls_free_cert_key(tls_param, type);
            cert_key_buf = NULL;
            //cert_key_size = 0;
            //cert_key_buff_size = 0;
        }
        else
        {
            /* Reuse the buffer */
            apb_proxy_tls_reset_cert_key(tls_param, type, size);
        }
    }

    if (!cert_key_buf)
    {
        cert_key_buf = apb_proxy_tls_create_cert_key(tls_param, type, size);
        if (!cert_key_buf)
        {
            return APB_PROXY_TLS_RET_ERROR;
        }

        //cert_key_size = cert_key_buff_size = size;
    }

    if (g_apb_tls_cntx_ptr->data_mode_conn_id)
    {
        TLS_AT_CMD_LOG_INFO("data_mode_conn_id:%d %x is not 0.",
            g_apb_tls_cntx_ptr->data_mode_conn_id,
            &g_apb_tls_cntx_ptr->data_mode_conn_id);
        return APB_PROXY_TLS_RET_WRONG_STATE;
    }

    g_apb_tls_cntx_ptr->data_mode_tid = tls_param->tid;

    if (APB_PROXY_TLS_PARAM_TYPE_SERVER_CA == type)
    {
        g_apb_tls_cntx_ptr->data_mode_type = APB_PROXY_TLS_DATA_MODE_TYPE_SERVER_CA;        
    }
    else if (APB_PROXY_TLS_PARAM_TYPE_CLIENT_CERT == type)
    {
        g_apb_tls_cntx_ptr->data_mode_type = APB_PROXY_TLS_DATA_MODE_TYPE_CLIENT_CERT;        
    }
    else
    {
        g_apb_tls_cntx_ptr->data_mode_type = APB_PROXY_TLS_DATA_MODE_TYPE_CLIENT_PRIVATE_KEY;
    }

    g_apb_tls_cntx_ptr->data_mode_cmd_id = cmd_id;

    return APB_PROXY_TLS_RET_CONNECT;
}


#else
bool apb_proxy_tls_set_cert_key(apb_proxy_tls_param_struct *tls_param,
                                       apb_proxy_tls_param_type_enum type,
                                       apb_proxy_tls_param_cert_key_struct *cert_key_param)
{
    char *cert_key_buf = NULL;
    unsigned int cert_key_buff_size = 0, cert_key_size = 0;
    apb_proxy_tls_cert_key_status_enum cert_key_complete = APB_PROXY_TLS_NONE;
    int ret_int = 0;

    TLS_AT_CMD_LOG_INFO("type:%d. more:%d", type, cert_key_param ? cert_key_param->more : -1);

    if (!tls_param || !cert_key_param)
    {
        return false;
    }

    switch ((int)type)
    {
        case APB_PROXY_TLS_PARAM_TYPE_SERVER_CA:
        {
            cert_key_buf = tls_param->svr_ca;
            cert_key_buff_size = tls_param->svr_ca_buff_size;
            cert_key_size = tls_param->svr_ca_size;
            cert_key_complete = APB_PROXY_TLS_SVR_CA_COMPLETE;
            break;
        }

        case APB_PROXY_TLS_PARAM_TYPE_CLIENT_CERT:
        {
            cert_key_buf = tls_param->cli_cert;
            cert_key_buff_size = tls_param->cli_cert_buff_size;
            cert_key_size = tls_param->cli_cert_size;
            cert_key_complete = APB_PROXY_TLS_CLI_CERT_COMPLETE;
            break;
        }

        case APB_PROXY_TLS_PARAM_TYPE_CLIENT_PRIVATE_KEY:
        {
            cert_key_buf = tls_param->cli_pkey;
            cert_key_buff_size = tls_param->cli_pkey_buff_size;
            cert_key_size = tls_param->cli_pkey_size;
            cert_key_complete = APB_PROXY_TLS_CLI_PKEY_COMPLETE;
            break;
        }

        default:
            return false;
    }

    if (!cert_key_param->size)
    {
        /* Delete the server ca certificate. */
        if (cert_key_buf)
        {
            apb_proxy_tls_free_cert_key(tls_param, type);
        }
        else
        {
            TLS_AT_CMD_LOG_WARN("Remove unexisted cert/pkey.");
        }

        return true;
    }

    /* Check the fields */
    if (!cert_key_param->cert || !cert_key_param->cert_len ||
        cert_key_param->cert_len > cert_key_param->size ||
        (cert_key_param->cert_len == cert_key_param->size && 1 == cert_key_param->more))
    {
        return false;
    }

    /* Check if it's a new certificate. */
    if (cert_key_buf &&
        (cert_key_size != cert_key_param->size ||
         (tls_param->cert_key_status & cert_key_complete)))
    {
        if (cert_key_param->size > cert_key_buff_size)
        {
            /* Need to allocate a new buffer */
            apb_proxy_tls_free_cert_key(tls_param, type);
            cert_key_buf = NULL;
            cert_key_size = 0;
            cert_key_buff_size = 0;
        }
        else
        {
            /* Reuse the buffer */
            apb_proxy_tls_reset_cert_key(tls_param, type, cert_key_param->size);
        }
    }

    if (cert_key_buf &&
        (cert_key_param->cert_len + strlen((const char*)cert_key_buf) > cert_key_param->size ||
        (cert_key_param->cert_len + strlen((const char*)cert_key_buf) == cert_key_param->size && 1 == cert_key_param->more)))
    {
        apb_proxy_tls_free_cert_key(tls_param, type);
        return false;
    }

    if (!cert_key_buf)
    {
        cert_key_buf = apb_proxy_tls_create_cert_key(tls_param, type, cert_key_param->size);
        if (!cert_key_buf)
        {
            return false;
        }

        cert_key_size = cert_key_buff_size = cert_key_param->size;
    }

    strncpy(cert_key_buf + strlen((const char*)cert_key_buf),
            (const char *)cert_key_param->cert,
            cert_key_param->cert_len);
    TLS_AT_CMD_LOG_INFO("total cert_len:%d, curr cert_len:%d", strlen((const char*)cert_key_buf), cert_key_param->cert_len);
#ifdef TLS_AT_CMD_LOG_DUMP_MEM
    {
        char *pbuf = cert_key_param->cert;
        int i = 0, len = cert_key_param->cert_len;

        while (i < len)
        {
            if (len - i > 10)
            {
                TLS_AT_CMD_LOG_INFO("%x, %x, %x, %x, %x, %x, %x, %x, %x, %x\r\n",
                    pbuf[i], pbuf[i + 1], pbuf[i + 2], pbuf[i + 3], pbuf[i + 4],
                    pbuf[i + 5], pbuf[i + 6], pbuf[i + 7], pbuf[i + 8], pbuf[i + 9]);
                i += 10;
            }
            else
            {
                TLS_AT_CMD_LOG_INFO("%x, %x, %x, %x, %x, %x, %x, %x, %x, %x\r\n",
                    pbuf[i],
                    i + 1 < len ? pbuf[i + 1] : 0xFF,
                    i + 2 < len ? pbuf[i + 2] : 0xFF,
                    i + 3 < len ? pbuf[i + 3] : 0xFF,
                    i + 4 < len ? pbuf[i + 4] : 0xFF,
                    i + 5 < len ? pbuf[i + 5] : 0xFF,
                    i + 6 < len ? pbuf[i + 6] : 0xFF,
                    i + 7 < len ? pbuf[i + 7] : 0xFF,
                    i + 8 < len ? pbuf[i + 8] : 0xFF,
                    i + 9 < len ? pbuf[i + 9] : 0xFF);
                break;
            }
        }
    }
#endif

    if (!cert_key_param->more)
    {        
        TLS_AT_CMD_LOG_INFO("cert_len:%d, encode_type:%d\r\n%s", strlen((const char*)cert_key_buf), cert_key_param->encode_type, cert_key_buf);
        /* Reuse the buffer pointed by cert_key_param->cert for the plain buffer size is less than encoded buffer size. */
        ret_int = apb_proxy_tls_decode((char *)cert_key_buf,
                                       cert_key_buff_size,
                                       &cert_key_buff_size,
                                       (const char *)cert_key_buf,
                                       strlen((const char*)cert_key_buf),
                                       cert_key_param->encode_type);

        if (0 == ret_int)
        {
            TLS_AT_CMD_LOG_INFO("cert_len:%d, %s", strlen((const char*)cert_key_buf), cert_key_buf);
            tls_param->cert_key_status |= cert_key_complete;
        }
        else
        {
            apb_proxy_tls_free_cert_key(tls_param, type);
        }

        TLS_AT_CMD_LOG_INFO("ret_int:%d, return:%s", ret_int, (0 == ret_int) ? "true" : "false");
    }
    else
    {
        TLS_AT_CMD_LOG_INFO("Need more cert/pkey");
    }

    return (0 == ret_int) ? true : false;
}
#endif


apb_proxy_tls_param_struct *apb_proxy_tls_param_node_create(int tid)
{
    apb_proxy_tls_param_struct *node = NULL;
    bool ret = false;

    TLS_AT_CMD_LOG_INFO("tid:%d.", tid);

    if (!apb_proxy_tls_is_valid_tid(tid))
    {
        return NULL;
    }

    node = pvPortCalloc(1, sizeof(apb_proxy_tls_param_struct));
    if (!node)
    {
        TLS_AT_CMD_LOG_WARN("Failed to create a new node for no memory.");
        return NULL;
    }

    node->tid = tid;

    /* Set default values. */
    strcpy(node->port, "443");
    node->auth_mode = MBEDTLS_SSL_VERIFY_REQUIRED;    

    ret = apb_proxy_tls_list_insert((apb_proxy_tls_template_struct **)&g_apb_tls_param_list,
                                    (apb_proxy_tls_template_struct *)node);
    if (!ret)
    {
        TLS_AT_CMD_LOG_WARN("Failed to create a new node for insert failed.");
        vPortFree(node);
        node = NULL;
    }
    else
    {
        TLS_AT_CMD_LOG_INFO("node created");
    }

    return node;
}


void apb_proxy_tls_param_node_free(apb_proxy_tls_param_struct *node)
{
    if (!node)
    {
        return;
    }

    apb_proxy_tls_list_remove((apb_proxy_tls_template_struct **)&g_apb_tls_param_list,
                              (apb_proxy_tls_template_struct *)node);

    if (node->svr_ca)
    {
        vPortFree(node->svr_ca);
        node->svr_ca = NULL;
    }

    if (node->cli_cert)
    {
        vPortFree(node->cli_cert);
        node->cli_cert= NULL;
    }

    if (node->cli_pkey)
    {
        vPortFree(node->cli_pkey);
        node->cli_pkey= NULL;
    }

    vPortFree(node);
    TLS_AT_CMD_LOG_INFO("node freed");
}


bool apb_proxy_tls_param_set(int type, void *value, apb_proxy_tls_param_struct *tls_param)
{
    bool ret = false;

    TLS_AT_CMD_LOG_INFO("type:%d.", type);

    if (!value || !tls_param)
    {
        TLS_AT_CMD_LOG_INFO("Invalid parameter");
        return false;
    }

    switch (type)
    {
        case APB_PROXY_TLS_PARAM_TYPE_HOST_NAME:
        {
            ret = apb_proxy_nw_cpy_without_quote(tls_param->host_name, (char *)value, APB_PROXY_TLS_HOST_NAME_MAX_LEN + 1);
            if (!ret)
            {
                memset(tls_param->host_name, 0, APB_PROXY_TLS_HOST_NAME_MAX_LEN + 1);
                return false;
            }

            TLS_AT_CMD_LOG_INFO("host_name:%s.", tls_param->host_name);
            break;
        }

        case APB_PROXY_TLS_PARAM_TYPE_PORT:
        {
            int port_len = strlen((const char*)value);

            if (APB_PROXY_TLS_PORT_MAX_LEN < port_len)
            {
                return false;
            }

            strncpy(tls_param->port,(char *)value, port_len);
            tls_param->port[port_len] = '\0';
            TLS_AT_CMD_LOG_INFO("port:%s.", tls_param->port);
            ret = true;
            break;
        }

        case APB_PROXY_TLS_PARAM_TYPE_SOC_TYPE:
        {
            char soc_type = atoi((char *)value);

            if (soc_type)
            {
                /* Only TCP is supported and it's the default value. */
                return false;
            }

            tls_param->soc_type = soc_type;
            TLS_AT_CMD_LOG_INFO("soc_type:%d.", tls_param->soc_type);
            ret = true;
            break;
        }

        case APB_PROXY_TLS_PARAM_TYPE_AUTH_MODE:
        {
            char auth_mode = atoi((char *)value);

            if (MBEDTLS_SSL_VERIFY_NONE != auth_mode &&
                MBEDTLS_SSL_VERIFY_OPTIONAL != auth_mode &&
                MBEDTLS_SSL_VERIFY_REQUIRED != auth_mode)
            {
                return false;
            }

            tls_param->auth_mode= auth_mode;
            TLS_AT_CMD_LOG_INFO("auth_mode:%d.", tls_param->auth_mode);
            ret = true;
            break;
        }

        case APB_PROXY_TLS_PARAM_TYPE_DEBUG_LEVEL:
        {
            char debug_level = atoi((char *)value);

            if (4 < debug_level)
            {
                return false;
            }

            tls_param->debug_level= debug_level;
            TLS_AT_CMD_LOG_INFO("debug_level:%d.", tls_param->debug_level);
            ret = true;
            break;
        }

#ifndef TLS_AT_CMD_SUPPORT_DATA_MODE
        case APB_PROXY_TLS_PARAM_TYPE_SERVER_CA:
        {
            ret = apb_proxy_tls_set_cert_key(tls_param,
                                       APB_PROXY_TLS_PARAM_TYPE_SERVER_CA,
                                       (apb_proxy_tls_param_cert_key_struct *)value);
            TLS_AT_CMD_LOG_INFO("ret:%d, %s", ret, ret ? "true" : "false");
            break;
        }

        case APB_PROXY_TLS_PARAM_TYPE_CLIENT_CERT:
        {
            ret = apb_proxy_tls_set_cert_key(tls_param,
                                       APB_PROXY_TLS_PARAM_TYPE_CLIENT_CERT,
                                       (apb_proxy_tls_param_cert_key_struct *)value);
            break;
        }

        case APB_PROXY_TLS_PARAM_TYPE_CLIENT_PRIVATE_KEY:
        {
            ret = apb_proxy_tls_set_cert_key(tls_param,
                                       APB_PROXY_TLS_PARAM_TYPE_CLIENT_PRIVATE_KEY,
                                       (apb_proxy_tls_param_cert_key_struct *)value);
            break;
        }
#endif
        default:
            break;
    }

    return ret;
}


apb_proxy_tls_ret_enum apb_proxy_tls_process_cfg_params(char *param_str, apb_proxy_cmd_id_t cmd_id)
{
    char *param = NULL, *param_str_tmp = NULL;
    int tid = -1, type = 0, next_type = 0, param_str_len = 0;
    apb_proxy_tls_param_struct *tls_param = NULL;
    bool is_new = false;
    apb_proxy_tls_ret_enum ret = APB_PROXY_TLS_RET_OK;
    apb_proxy_tls_param_cert_key_struct cert_key = {0};
    
    if (!param_str)
    {
        return APB_PROXY_TLS_RET_INVALID_PARAM;
    }

    param_str_len = strlen((const char*)param_str);

    /* Parse <tid> */
    param = strtok(param_str, (const char *)",");
    if (!param)
    {
        return APB_PROXY_TLS_RET_INVALID_FORMAT;
    }

    tid = (unsigned int)atoi(param);    
    if (!apb_proxy_tls_is_valid_tid(tid))
    {
        return APB_PROXY_TLS_RET_WRONG_STATE;
    }

    /* Parse <type> */
    param = strtok(NULL, (const char *)",");

    /* Try to find tls_param with the same tid. */
    tls_param = (apb_proxy_tls_param_struct *)apb_proxy_tls_list_find((apb_proxy_tls_template_struct *)g_apb_tls_param_list, tid);
    if (!param)
    {
        /* Free tls parameters. */
        TLS_AT_CMD_LOG_INFO("Free tls parameters.");
        if (tls_param && !tls_param->is_using)
        {
            apb_proxy_tls_param_node_free(tls_param);
            return APB_PROXY_TLS_RET_OK;
        }

        /* tls parameters with the same tid was not set before or it's using. */
        return APB_PROXY_TLS_RET_WRONG_STATE;
    }

    /* Set tls parameters. */
    TLS_AT_CMD_LOG_INFO("Set tls parameters.");
    if (!tls_param)
    {
        tls_param = apb_proxy_tls_param_node_create(tid);
        if (!tls_param)
        {
            return APB_PROXY_TLS_RET_ERROR;
        }
        is_new = true;
    }
    else if (tls_param->is_using)
    {
        /* There is tls connection using the parameter. */
        TLS_AT_CMD_LOG_INFO("tls_param is in use.");
        return APB_PROXY_TLS_RET_WRONG_STATE;
    }

    while (param)
    {
        if (next_type)
        {
            type = next_type;
            next_type = 0;
            
        }
        else
        {
            type = atoi(param);
        }
        
        if (APB_PROXY_TLS_PARAM_TYPE_SERVER_CA != type &&
            APB_PROXY_TLS_PARAM_TYPE_CLIENT_CERT != type &&
            APB_PROXY_TLS_PARAM_TYPE_CLIENT_PRIVATE_KEY != type)
        {
            /* Parse <value>. */
            param = strtok(NULL, (const char *)",");
            if (!param)
            {
                ret = APB_PROXY_TLS_RET_INVALID_FORMAT;
                break;
            }

            /* Set parameters. */
            if (!apb_proxy_tls_param_set(type, (void *)param, tls_param))
            {
                ret = APB_PROXY_TLS_RET_ERROR;
                break;
            }

            /* Parse next <type>. */
            param = strtok(NULL, (const char *)",");
        }
        else
        {
            /* Parse certificate or private key. */
            int size = 0;
            
            /* Parse <size>. */
            param = strtok(NULL, (const char *)",");
            if (!param)
            {
                ret = APB_PROXY_TLS_RET_INVALID_FORMAT;
                break;
            }
            size = atoi(param);

#ifdef TLS_AT_CMD_SUPPORT_DATA_MODE
            ret = apb_proxy_tls_cert_key_set_size(tls_param, type, size, cmd_id);
            break;
#else
            memset(&cert_key, 0, sizeof(apb_proxy_tls_param_cert_key_struct));
            cert_key.size = size;
            if (!size)
            {
                if (!apb_proxy_tls_param_set(type, (void *)&cert_key, tls_param))
                {
                    ret = APB_PROXY_TLS_RET_ERROR;
                    break;
                }

                /* Parse next <type>. */
                param = strtok(NULL, (const char *)",");
                continue;
            }

            /* Parse <more>. */
            param = strtok(NULL, (const char *)",");
            if (!param)
            {
                TLS_AT_CMD_LOG_INFO("Invalid format %d", __LINE__);
                ret = APB_PROXY_TLS_RET_INVALID_FORMAT;
                break;
            }
            cert_key.more = atoi(param);
            if (0 != cert_key.more && 1 != cert_key.more)
            {
                ret = APB_PROXY_TLS_RET_INVALID_FORMAT;
                break;
            }

            /* Parse <cert/pkey>. */
            param_str_tmp = param + strlen((const char*)param) + 1;
            if (param_str_tmp > param_str + param_str_len ||
                *param_str_tmp != '"')
            {
                TLS_AT_CMD_LOG_INFO("Invalid format %d", __LINE__);
                ret = APB_PROXY_TLS_RET_INVALID_FORMAT;
                break;
            }

#if 1
            param = apb_proxy_nw_parse_string_param(&param_str_tmp, param_str + param_str_len - 1);
            if (!param)
            {
                TLS_AT_CMD_LOG_INFO("Invalid format %d", __LINE__);
                ret = APB_PROXY_TLS_RET_INVALID_FORMAT;
                break;
            }

            cert_key.cert = (char *)param;
            cert_key.cert_len = strlen((const char*)cert_key.cert);
#else

            /* Skip " */
            param = ++param_str_tmp;

            while (param_str_tmp < param_str + param_str_len &&
                   !(*(param_str_tmp - 1) != '\\' && *param_str_tmp == '"' &&
                   (*(param_str_tmp + 1) == ',' || *(param_str_tmp + 1) == '\0' ||
                    *(param_str_tmp + 1) == '\r' || *(param_str_tmp + 1) == '\n')))
            {
                param_str_tmp++;
            }

            if (param_str_tmp < param_str + param_str_len &&
                (*(param_str_tmp - 1) != '\\' && *param_str_tmp == '"' &&
                (*(param_str_tmp + 1) == ',' || *(param_str_tmp + 1) == '\0' ||
                 *(param_str_tmp + 1) == '\r' || *(param_str_tmp + 1) == '\n')))
            {
                /* Skip " */
                *param_str_tmp = '\0';
                if (*(param_str_tmp + 1) == ',')
                {
                    param_str_tmp += 2;
                }
                else
                {
                    param_str_tmp = NULL;
                }

                cert_key.cert = param;
                cert_key.cert_len = strlen((const char*)cert_key.cert);
            }
            else
            {
                TLS_AT_CMD_LOG_INFO("%d,param_str_tmp:%x,param_str:%x,param_str_len:%d, ", __LINE__, param_str_tmp, param_str, param_str_len);
                TLS_AT_CMD_LOG_INFO("*(param_str_tmp - 1):%d,*(param_str_tmp):%d,*(param_str_tmp + 1):%d, ", *(param_str_tmp - 1), *(param_str_tmp), *(param_str_tmp + 1));
                ret = APB_PROXY_TLS_RET_INVALID_FORMAT;
                break;
            }
#endif
            /* Parse <encode_type>. */
            /* Set default encode_type for cert/pkey. */
            cert_key.encode_type = APB_PROXY_TLS_ENCODE_TYPE_DEFAULT;
            if (param_str_tmp && *param_str_tmp != '\0')
            {
                param = strtok(param_str_tmp, (const char *)",");
            }
            else
            {
                param = NULL;
            }
            
            if (param)
            {
                cert_key.encode_type = atoi(param);
                if ((APB_PROXY_TLS_ENCODE_TYPE_STRING <= cert_key.encode_type && 
                    APB_PROXY_TLS_ENCODE_TYPE_MAX > cert_key.encode_type) ||
                    (APB_PROXY_TLS_PARAM_TYPE_NONE < (apb_proxy_tls_param_type_enum)cert_key.encode_type &&
                    APB_PROXY_TLS_PARAM_TYPE_MAX > (apb_proxy_tls_param_type_enum)cert_key.encode_type))
                {
                    if (APB_PROXY_TLS_PARAM_TYPE_NONE < (apb_proxy_tls_param_type_enum)cert_key.encode_type &&
                        APB_PROXY_TLS_PARAM_TYPE_MAX > (apb_proxy_tls_param_type_enum)cert_key.encode_type)
                    {
                        /* It's next type and encode_type is ommitted. */
                        next_type = cert_key.encode_type;
                        
                        /* Set default encode_type for cert/pkey. */
                        cert_key.encode_type = APB_PROXY_TLS_ENCODE_TYPE_DEFAULT;
                    }
                    /* else, it's encode_type. */
                }                
                else
                {
                    /* Invalid parameter. */
                    ret = APB_PROXY_TLS_RET_INVALID_FORMAT;
                    break;
                }

                param = strtok(NULL, (const char *)",");
            }

            if (!apb_proxy_tls_param_set(type, (void *)&cert_key, tls_param))
            {
                ret = APB_PROXY_TLS_RET_ERROR;
                break;
            }
            TLS_AT_CMD_LOG_INFO("ret:%d", ret);
#endif
        }
    }

    if (is_new &&
#ifdef TLS_AT_CMD_SUPPORT_DATA_MODE
        APB_PROXY_TLS_RET_CONNECT != ret &&
#endif
        APB_PROXY_TLS_RET_OK != ret)
    {
        apb_proxy_tls_param_node_free(tls_param);
    }
    
    return ret;
}


bool apb_proxy_tls_param_is_param_ready(apb_proxy_tls_param_struct *tls_param)
{
    if (!tls_param)
    {
        return false;
    }

    /* Check parameters: 1) mandatory 2) if cert/key are completed 3) confict parameters */
    if ((tls_param->svr_ca && !(tls_param->cert_key_status & APB_PROXY_TLS_SVR_CA_COMPLETE)) ||
        (tls_param->cli_cert && !(tls_param->cert_key_status & APB_PROXY_TLS_CLI_CERT_COMPLETE)) ||
        (tls_param->cli_pkey && !(tls_param->cert_key_status & APB_PROXY_TLS_CLI_PKEY_COMPLETE)) ||
        (tls_param->cli_cert && !tls_param->cli_pkey) ||
        (!tls_param->cli_cert && tls_param->cli_pkey) ||
        !tls_param->host_name[0] ||
        (tls_param->auth_mode == MBEDTLS_SSL_VERIFY_REQUIRED && !tls_param->svr_ca))
    {
        return false;
    }

    return true;
}

#define APB_PROXY_TLS_CONN_API
/* -------------------tls_conn APIs------------------- */
static void my_debug(void *ctx, int level,
                         const char *file, int line,
                         const char *str )
{
    TLS_AT_CMD_LOG_INFO("mbedtls[%d]%s:%04d: %s\r\n", level, file ? file : "", line, str ? str : "");
}


apb_proxy_tls_state_enum apb_proxy_tls_conn_get_state(int tid)
{
    apb_proxy_tls_conn_struct *tls_conn = NULL;

    if (!g_apb_tls_cntx_ptr)
    {
        return APB_PROXY_TLS_STATE_MAX;
    }

    tls_conn = (apb_proxy_tls_conn_struct *)apb_proxy_tls_list_find((apb_proxy_tls_template_struct *)g_apb_tls_cntx_ptr->tls_conn_list, tid);
    if (tls_conn)
    {
        return tls_conn->state;
    }

    return APB_PROXY_TLS_STATE_MAX;
}


void apb_proxy_tls_conn_clear_state(int tid, apb_proxy_tls_state_enum state)
{
    apb_proxy_tls_conn_struct *tls_conn = NULL;

    if (!g_apb_tls_cntx_ptr)
    {
        return;
    }

    tls_conn = (apb_proxy_tls_conn_struct *)apb_proxy_tls_list_find((apb_proxy_tls_template_struct *)g_apb_tls_cntx_ptr->tls_conn_list, tid);
    if (tls_conn)
    {
        tls_conn->state &= (~state);
    }
}


void apb_proxy_tls_conn_set_state(int tid, apb_proxy_tls_state_enum state)
{
    apb_proxy_tls_conn_struct *tls_conn = NULL;

    if (!g_apb_tls_cntx_ptr)
    {
        return;
    }

    tls_conn = (apb_proxy_tls_conn_struct *)apb_proxy_tls_list_find((apb_proxy_tls_template_struct *)g_apb_tls_cntx_ptr->tls_conn_list, tid);
    if (tls_conn)
    {
        tls_conn->state |= state;
    }
}


apb_proxy_tls_conn_struct *apb_proxy_tls_conn_node_create(int tid, int cid)
{
    apb_proxy_tls_conn_struct *node = NULL;
    bool ret = false;

    if (!apb_proxy_tls_is_valid_tid(tid) || !g_apb_tls_cntx_ptr)
    {
        return NULL;
    }

    node = pvPortCalloc(1, sizeof(apb_proxy_tls_conn_struct));
    if (!node)
    {
        TLS_AT_CMD_LOG_INFO("Failed to create a new tls conn node.");
        return NULL;
    }

    node->tid = tid;
    node->cid = cid;
    node->state = APB_PROXY_TLS_STATE_CONNECTING;

    ret = apb_proxy_tls_list_insert((apb_proxy_tls_template_struct **)&g_apb_tls_cntx_ptr->tls_conn_list,
                                    (apb_proxy_tls_template_struct *)node);
    if (!ret)
    {
        vPortFree(node);
        node = NULL;
    }

    return node;
}


void apb_proxy_tls_conn_node_free(apb_proxy_tls_conn_struct *node)
{
    if (!node || !g_apb_tls_cntx_ptr)
    {
        return;
    }

    apb_proxy_tls_list_remove((apb_proxy_tls_template_struct **)&g_apb_tls_cntx_ptr->tls_conn_list,
                              (apb_proxy_tls_template_struct *)node); 

    vPortFree(node);
}

void apb_proxy_tls_conn_close(apb_proxy_tls_conn_struct *tls_conn)
{
    if (!tls_conn)
    {
        return;
    }

    mbedtls_ssl_close_notify(&tls_conn->ssl_ctx);

    mbedtls_net_free(&tls_conn->net_ctx);
    mbedtls_x509_crt_free(&tls_conn->cacert);
    mbedtls_x509_crt_free(&tls_conn->clicert);
    mbedtls_pk_free(&tls_conn->pkey);
    mbedtls_ssl_free(&tls_conn->ssl_ctx);
    mbedtls_ssl_config_free(&tls_conn->ssl_conf);
    mbedtls_ctr_drbg_free(&tls_conn->ctr_drbg);
    mbedtls_entropy_free(&tls_conn->entropy);
}


#if 1

/* return 0 successful, otherwise failure. */
static int apb_proxy_tls_conn_conn(apb_proxy_tls_conn_struct *tls_conn,
                                             apb_proxy_tls_param_struct *tls_param)
{
    const char *pers = "apb_tls";    
    int ret = 0;
    size_t len = 0;

    if (!tls_conn || !tls_param)
    {
        TLS_AT_CMD_LOG_INFO("Invalid parameter.");
        return -1;
    }

    
#if defined(MBEDTLS_DEBUG_C)
    mbedtls_debug_set_threshold(tls_param->debug_level);
#endif

    mbedtls_net_init(&tls_conn->net_ctx);
    mbedtls_ssl_init(&tls_conn->ssl_ctx);
    mbedtls_ssl_config_init(&tls_conn->ssl_conf);
    mbedtls_x509_crt_init(&tls_conn->cacert);
    mbedtls_x509_crt_init(&tls_conn->clicert);
    mbedtls_pk_init(&tls_conn->pkey);
    mbedtls_ctr_drbg_init(&tls_conn->ctr_drbg);

    /*
        * 0. Initialize the RNG and the session data
        */
    mbedtls_entropy_init(&tls_conn->entropy);
    if ((ret = mbedtls_ctr_drbg_seed(&tls_conn->ctr_drbg,
                mbedtls_entropy_func, &tls_conn->entropy,
                (const uint8_t*)pers, strlen((const char*)pers))) != 0)
    {
        TLS_AT_CMD_LOG_INFO( "mbedtls_ctr_drbg_seed() failed, value:-0x%x\r\n", -ret);
        goto exit;
    }

    /*
        * 0. Load the Client certificate
        */
    if (tls_param->cli_cert)
    {
        len = strlen((const char*)tls_param->cli_cert) + 1;
        ret = mbedtls_x509_crt_parse(&tls_conn->clicert, (const uint8_t *)tls_param->cli_cert, len);
        if(ret < 0)
        {
            TLS_AT_CMD_LOG_INFO( "Loading cli_cert failed\n  !  mbedtls_x509_crt_parse returned -0x%x\n\n", -ret);
            goto exit;
        }

        len = strlen((const char*)tls_param->cli_pkey) + 1;
        ret = mbedtls_pk_parse_key(&tls_conn->pkey, (const uint8_t *)tls_param->cli_pkey, len, NULL, 0);
        if(ret != 0)
        {
            TLS_AT_CMD_LOG_INFO( " failed\n  !  mbedtls_pk_parse_key returned -0x%x\n\n", -ret);
            goto exit;
        }
    }

    /*
        * 1. Load the trusted CA
        */

    /* cert_len passed in is gotten from sizeof not strlen */
    if (tls_param->svr_ca)
    {
        len = strlen((const char*)tls_param->svr_ca) + 1;
        ret = mbedtls_x509_crt_parse(&tls_conn->cacert, (const uint8_t *)tls_param->svr_ca, len);
        if (ret < 0)
        {
            TLS_AT_CMD_LOG_INFO( "mbedtls_x509_crt_parse() failed, value:-0x%x\r\n", -ret);
            goto exit;
        }
    }

#ifndef TLS_AT_CMD_UT
    if((ret = mbedtls_net_connect(&tls_conn->net_ctx, tls_param->host_name, tls_param->port, MBEDTLS_NET_PROTO_TCP)) != 0)
    {
        TLS_AT_CMD_LOG_INFO( " failed\n  ! mbedtls_net_connect returned %d, port:%s\n\n", ret, tls_param->port);
        goto exit;
    }

#if 0
    if((ret = mbedtls_net_set_block(&tls_conn->net_ctx)) != 0)
    {
        TLS_AT_CMD_LOG_INFO( " failed\n  ! net_set_(non)block() returned -0x%x\n\n", -ret );
        goto exit;
    }
#endif

    if((ret = mbedtls_net_set_nonblock(&tls_conn->net_ctx)) != 0)
    {
        TLS_AT_CMD_LOG_INFO( " failed\n  ! net_set_(non)block() returned -0x%x\n\n", -ret );
        goto exit;
    }
#endif
    /*
        * 2. Setup stuff
        */
    if((ret = mbedtls_ssl_config_defaults(&tls_conn->ssl_conf,
        MBEDTLS_SSL_IS_CLIENT,
        MBEDTLS_SSL_TRANSPORT_STREAM,
        MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
        TLS_AT_CMD_LOG_INFO( "mbedtls_ssl_config_defaults() failed, value:-0x%x\r\n", -ret);
        goto exit;
    }

    // memcpy(&tls_conn->profile, tls_conn->ssl_conf.cert_profile, sizeof(mbedtls_x509_crt_profile));
    // tls_conn->profile.allowed_mds = tls_conn->profile.allowed_mds | MBEDTLS_X509_ID_FLAG( MBEDTLS_MD_MD5 );
    // mbedtls_ssl_conf_cert_profile(&tls_conn->ssl_conf, &tls_conn->profile);

    mbedtls_ssl_conf_authmode(&tls_conn->ssl_conf, tls_param->auth_mode);
    mbedtls_ssl_conf_ca_chain(&tls_conn->ssl_conf, &tls_conn->cacert, NULL);

    if((ret = mbedtls_ssl_conf_own_cert(&tls_conn->ssl_conf, &tls_conn->clicert, &tls_conn->pkey)) != 0)
    {
        TLS_AT_CMD_LOG_INFO( " failed\n  ! mbedtls_ssl_conf_own_cert returned %d\n\n", ret);
        goto exit;
    }

    mbedtls_ssl_conf_rng(&tls_conn->ssl_conf,
                         mbedtls_ctr_drbg_random,
                         &tls_conn->ctr_drbg);

    mbedtls_ssl_conf_dbg(&tls_conn->ssl_conf, my_debug, NULL);
    mbedtls_ssl_conf_read_timeout(&tls_conn->ssl_conf, 10000);

    if ((ret = mbedtls_ssl_setup(&tls_conn->ssl_ctx,
        &tls_conn->ssl_conf)) != 0)
    {
        TLS_AT_CMD_LOG_INFO( "mbedtls_ssl_setup() failed, value:-0x%x\r\n", -ret);
        goto exit;
    }

    mbedtls_ssl_set_bio(&tls_conn->ssl_ctx,
                        &tls_conn->net_ctx,
                        mbedtls_net_send,
                        //mbedtls_net_recv,
                        //NULL);
                        NULL,
                        mbedtls_net_recv_timeout);

    /*
        * 3. Handshake
        */

exit:
    tls_param->is_using--;

    if (0 != ret)
    {
        apb_proxy_tls_conn_close(tls_conn);
    }

    return ret;
}


static int apb_proxy_tls_conn_handshake(apb_proxy_tls_conn_struct *tls_conn)
{
    int ret = 0;
    unsigned int flags = 0;

    if (!tls_conn)
    {
        return -1;
    }
    
#ifndef TLS_AT_CMD_UT
    ret = mbedtls_ssl_handshake(&tls_conn->ssl_ctx);
#endif

    if (0 == ret)
    {
        TLS_AT_CMD_LOG_INFO( "Handshake done.");

        mbedtls_ssl_conf_read_timeout(&tls_conn->ssl_conf, 1000);

        mbedtls_ssl_set_bio(&tls_conn->ssl_ctx,
                            &tls_conn->net_ctx,
                            mbedtls_net_send,
                            mbedtls_net_recv,
                            mbedtls_net_recv_timeout);    

#if 0   
#ifndef TLS_AT_CMD_UT
        if((ret = mbedtls_net_set_block(&tls_conn->net_ctx)) != 0)
        {
            TLS_AT_CMD_LOG_INFO( " failed\n  ! net_set_block() returned -0x%x\n\n", -ret );
            goto exit;
        }
#endif
#endif

        /*
            * 4. Verify the server certificate
            */

        /* In real life, we would have used MBEDTLS_SSL_VERIFY_REQUIRED so that the
            * handshake would not succeed if the peer's cert is bad.  Even if we used
            * MBEDTLS_SSL_VERIFY_OPTIONAL, we would bail out here if ret != 0
            */
        if((flags = mbedtls_ssl_get_verify_result(&tls_conn->ssl_ctx)) != 0)
        {
            char vrfy_buf[512];

            TLS_AT_CMD_LOG_INFO( "svr_cert varification failed\n");

            mbedtls_x509_crt_verify_info(vrfy_buf, sizeof( vrfy_buf ), "  ! ", flags);

            TLS_AT_CMD_LOG_INFO( "%s\n", vrfy_buf);
        }
        else
            TLS_AT_CMD_LOG_INFO( "svr_cert varification ok\n");

    }

    return ret;
}
#else


/* return 0 successful, otherwise failure. */
static int apb_proxy_tls_conn_conn(apb_proxy_tls_conn_struct *tls_conn,
                                             apb_proxy_tls_param_struct *tls_param)
{
    const char *pers = "apb_tls";
    unsigned int flags = 0;
    bool has_decreased = 0;
    int ret = 0;
    size_t len = 0;

    if (!tls_conn || !tls_param)
    {
        TLS_AT_CMD_LOG_INFO("Invalid parameter.");
        return -1;
    }

    
#if defined(MBEDTLS_DEBUG_C)
    mbedtls_debug_set_threshold(tls_param->debug_level);
#endif

    mbedtls_net_init(&tls_conn->net_ctx);
    mbedtls_ssl_init(&tls_conn->ssl_ctx);
    mbedtls_ssl_config_init(&tls_conn->ssl_conf);
    mbedtls_x509_crt_init(&tls_conn->cacert);
    mbedtls_x509_crt_init(&tls_conn->clicert);
    mbedtls_pk_init(&tls_conn->pkey);
    mbedtls_ctr_drbg_init(&tls_conn->ctr_drbg);

    /*
        * 0. Initialize the RNG and the session data
        */
    mbedtls_entropy_init(&tls_conn->entropy);
    if ((ret = mbedtls_ctr_drbg_seed(&tls_conn->ctr_drbg,
                mbedtls_entropy_func, &tls_conn->entropy,
                (const uint8_t*)pers, strlen((const char*)pers))) != 0)
    {
        TLS_AT_CMD_LOG_INFO( "mbedtls_ctr_drbg_seed() failed, value:-0x%x\r\n", -ret);
        goto exit;
    }

    /*
        * 0. Load the Client certificate
        */
    if (tls_param->cli_cert)
    {
        len = strlen((const char*)tls_param->cli_cert) + 1;
        ret = mbedtls_x509_crt_parse(&tls_conn->clicert, (const uint8_t *)tls_param->cli_cert, len);
        if(ret < 0)
        {
            TLS_AT_CMD_LOG_INFO( "Loading cli_cert failed\n  !  mbedtls_x509_crt_parse returned -0x%x\n\n", -ret);
            goto exit;
        }

        len = strlen((const char*)tls_param->cli_pkey) + 1;
        ret = mbedtls_pk_parse_key(&tls_conn->pkey, (const uint8_t *)tls_param->cli_pkey, len, NULL, 0);
        if(ret != 0)
        {
            TLS_AT_CMD_LOG_INFO( " failed\n  !  mbedtls_pk_parse_key returned -0x%x\n\n", -ret);
            goto exit;
        }
    }

    /*
        * 1. Load the trusted CA
        */

    /* cert_len passed in is gotten from sizeof not strlen */
    if (tls_param->svr_ca)
    {
        len = strlen((const char*)tls_param->svr_ca) + 1;
        ret = mbedtls_x509_crt_parse(&tls_conn->cacert, (const uint8_t *)tls_param->svr_ca, len);
        if (ret < 0)
        {
            TLS_AT_CMD_LOG_INFO( "mbedtls_x509_crt_parse() failed, value:-0x%x\r\n", -ret);
            goto exit;
        }
    }

#ifndef TLS_AT_CMD_UT
    if((ret = mbedtls_net_connect(&tls_conn->net_ctx, tls_param->host_name, tls_param->port, MBEDTLS_NET_PROTO_TCP)) != 0)
    {
        TLS_AT_CMD_LOG_INFO( " failed\n  ! mbedtls_net_connect returned %d, port:%s\n\n", ret, tls_param->port);
        goto exit;
    }

    if((ret = mbedtls_net_set_block(&tls_conn->net_ctx)) != 0)
    {
        TLS_AT_CMD_LOG_INFO( " failed\n  ! net_set_(non)block() returned -0x%x\n\n", -ret );
        goto exit;
    }
#endif
    /*
        * 2. Setup stuff
        */
    if((ret = mbedtls_ssl_config_defaults(&tls_conn->ssl_conf,
        MBEDTLS_SSL_IS_CLIENT,
        MBEDTLS_SSL_TRANSPORT_STREAM,
        MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
        TLS_AT_CMD_LOG_INFO( "mbedtls_ssl_config_defaults() failed, value:-0x%x\r\n", -ret);
        goto exit;
    }

    // memcpy(&tls_conn->profile, tls_conn->ssl_conf.cert_profile, sizeof(mbedtls_x509_crt_profile));
    // tls_conn->profile.allowed_mds = tls_conn->profile.allowed_mds | MBEDTLS_X509_ID_FLAG( MBEDTLS_MD_MD5 );
    // mbedtls_ssl_conf_cert_profile(&tls_conn->ssl_conf, &tls_conn->profile);

    mbedtls_ssl_conf_authmode(&tls_conn->ssl_conf, tls_param->auth_mode);
    mbedtls_ssl_conf_ca_chain(&tls_conn->ssl_conf, &tls_conn->cacert, NULL);

    if((ret = mbedtls_ssl_conf_own_cert(&tls_conn->ssl_conf, &tls_conn->clicert, &tls_conn->pkey)) != 0)
    {
        TLS_AT_CMD_LOG_INFO( " failed\n  ! mbedtls_ssl_conf_own_cert returned %d\n\n", ret);
        goto exit;
    }

    mbedtls_ssl_conf_rng(&tls_conn->ssl_conf,
                         mbedtls_ctr_drbg_random,
                         &tls_conn->ctr_drbg);

    mbedtls_ssl_conf_dbg(&tls_conn->ssl_conf, my_debug, NULL);
    mbedtls_ssl_conf_read_timeout(&tls_conn->ssl_conf, 0);

    if ((ret = mbedtls_ssl_setup(&tls_conn->ssl_ctx,
        &tls_conn->ssl_conf)) != 0)
    {
        TLS_AT_CMD_LOG_INFO( "mbedtls_ssl_setup() failed, value:-0x%x\r\n", -ret);
        goto exit;
    }

    mbedtls_ssl_set_bio(&tls_conn->ssl_ctx,
                        &tls_conn->net_ctx,
                        mbedtls_net_send,
                        mbedtls_net_recv,
                        mbedtls_net_recv_timeout);

    /*
        * 3. Handshake
        */
    tls_param->is_using--;
    has_decreased = true;
#ifndef TLS_AT_CMD_UT
    while ((ret = mbedtls_ssl_handshake(&tls_conn->ssl_ctx)) != 0)
    {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            TLS_AT_CMD_LOG_INFO( "mbedtls_ssl_handshake() failed, ret:-0x%x\r\n", -ret);
            goto exit;
        }
    }
#endif
    TLS_AT_CMD_LOG_INFO( "Handshake done.");

    mbedtls_ssl_conf_read_timeout(&tls_conn->ssl_conf, 1000);

#ifndef TLS_AT_CMD_UT
    if((ret = mbedtls_net_set_nonblock(&tls_conn->net_ctx)) != 0)
    {
        TLS_AT_CMD_LOG_INFO( " failed\n  ! net_set_(non)block() returned -0x%x\n\n", -ret );
        goto exit;
    }
#endif
    /*
        * 4. Verify the server certificate
        */

    /* In real life, we would have used MBEDTLS_SSL_VERIFY_REQUIRED so that the
        * handshake would not succeed if the peer's cert is bad.  Even if we used
        * MBEDTLS_SSL_VERIFY_OPTIONAL, we would bail out here if ret != 0
        */
    if((flags = mbedtls_ssl_get_verify_result(&tls_conn->ssl_ctx)) != 0)
    {
        char vrfy_buf[512];

        TLS_AT_CMD_LOG_INFO( "svr_cert varification failed\n");

        mbedtls_x509_crt_verify_info(vrfy_buf, sizeof( vrfy_buf ), "  ! ", flags);

        TLS_AT_CMD_LOG_INFO( "%s\n", vrfy_buf);
    }
    else
        TLS_AT_CMD_LOG_INFO( "svr_cert varification ok\n");

exit:
    if (!has_decreased)
    {
        tls_param->is_using--;
    }

    if (0 != ret)
    {
        apb_proxy_tls_conn_close(tls_conn);
    }

    return ret;
}
#endif


int apb_proxy_tls_conn_send(apb_proxy_tls_conn_struct *tls_conn,
                                    const char *buf,
                                    int len)
{
    int ret = 0, sent_len = 0;

    if (!tls_conn || !(APB_PROXY_TLS_STATE_CONNECTED & tls_conn->state) || !buf || !len)
    {
        return -1;
    }

#ifndef TLS_AT_CMD_UT
    do 
    {
        while((ret = mbedtls_ssl_write(&tls_conn->ssl_ctx, (const unsigned char*)buf + sent_len, len - sent_len)) <= 0)
        {
            if( ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE )
            {
                TLS_AT_CMD_LOG_INFO( " failed\n  ! mbedtls_ssl_write returned -0x%x\n\n", -ret );
                break;
            }
        }

        if (ret > 0)
        {
            sent_len += ret;
        }

        TLS_AT_CMD_LOG_INFO("sent_len:%d, ret:%d", sent_len, ret);
    } while (sent_len < len && ret > 0);
#else
        sent_len = ret = strlen((const char*)buf);
#endif
    TLS_AT_CMD_LOG_INFO("sent_len:%d, ret:%d", sent_len, ret);

    return ret > 0 ? sent_len : ret;
}


int apb_proxy_tls_conn_recv(apb_proxy_tls_conn_struct *tls_conn,
                                   char *buf,
                                   int max_num,
                                   apb_proxy_tls_encode_type_enum encode_type)
{
    int ret = 0;

    if (!tls_conn || !buf || !max_num ||
        APB_PROXY_TLS_ENCODE_TYPE_STRING > encode_type ||
        APB_PROXY_TLS_ENCODE_TYPE_BASE64 < encode_type)
    {
        return -1;
    }

#ifndef TLS_AT_CMD_UT
    while ((ret = mbedtls_ssl_read(&tls_conn->ssl_ctx, (unsigned char *)buf, max_num)) <= 0)
    {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE )
        {
            TLS_AT_CMD_LOG_INFO( " failed\n  ! mbedtls_ssl_write returned -0x%x\n\n", -ret );
            break;
        }
    }
#else
    strncpy(buf, "This is UT Test Response.\377\377\376\376\xff\xeb", max_num);
    ret = strlen((const char*)buf);
    TLS_AT_CMD_LOG_INFO("ret:%d, max_num:%d", ret, max_num);
#endif
    return ret;
}


void apb_proxy_tls_send_etlssend_urc(int tid, int ret)
{
    char result_str[25] = {0};
    apb_proxy_at_cmd_result_t response = {0};

    sprintf(result_str, "+ETLSSEND:%d,%d", tid, ret);

    response.pdata = result_str;
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;
    response.length = strlen((const char*)response.pdata);
    apb_proxy_send_at_cmd_result(&response);
}


void apb_proxy_tls_send_etlsrecv_urc(int tid, int ret, int encode_type, char *data)
{
    char result_buf[25] = {0}, *result_str = NULL;
    apb_proxy_at_cmd_result_t response = {0};
    int str_len = 0, data_len = 0, urc_buf_size = 0;
    bool is_new = false;

    TLS_AT_CMD_LOG_INFO("ret:%d, data:%x", ret, data);

    if (0 < ret && data)
    {
        sprintf(result_buf, "+ETLSRECV:%d,%d,", tid, ret);
        str_len = strlen((const char*)result_buf);
        data_len = strlen((const char*)data);
        urc_buf_size = str_len + data_len + 6 + 1;
        TLS_AT_CMD_LOG_INFO("result_str size:%d", urc_buf_size);
        if (urc_buf_size >= 25)
        {
            result_str = pvPortCalloc(1, urc_buf_size);
            
            if (!result_str)
            {
                TLS_AT_CMD_LOG_INFO("Lost ETLSRECV URC.");
                return;
            }
            is_new = true;
            memcpy(result_str, result_buf, str_len);
            if (APB_PROXY_TLS_ENCODE_TYPE_DEFAULT == encode_type)
            {
                sprintf(result_str + str_len, "\"%s\"", data);
            }
            else
            {
                sprintf(result_str + str_len, "\"%s\",%d", data, encode_type);
            }
        }
        else
        {
            if (APB_PROXY_TLS_ENCODE_TYPE_DEFAULT == encode_type)
            {
                sprintf(result_buf + str_len, "\"%s\"", data);
            }
            else
            {
                sprintf(result_buf + str_len, "\"%s\",%d", data, encode_type);
            }
            result_str = result_buf;
        }
    }
    else
    {
        sprintf(result_buf, "+ETLSRECV:%d,%d", tid, ret);
        result_str = result_buf;
    }

    response.pdata = result_str;
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;
    response.length = strlen((const char*)response.pdata);
    TLS_AT_CMD_LOG_INFO("response.length:%d", response.length);
    apb_proxy_send_at_cmd_result(&response);

    if (is_new)
    {
        vPortFree(result_str);
    }
}


void apb_proxy_tls_send_etlsconn_urc(int tid, int ret)
{
    char result_str[25] = {0};
    apb_proxy_at_cmd_result_t response = {0};

    sprintf(result_str, "+ETLSCONN:%d,%d", tid, ret);

    response.pdata = result_str;
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;
    response.length = strlen((const char*)response.pdata);
    apb_proxy_send_at_cmd_result(&response);
}


void apb_proxy_tls_send_etlsclose_urc(int tid, int ret)
{
    char result_str[25] = {0};
    apb_proxy_at_cmd_result_t response = {0};

    sprintf(result_str, "+ETLSCLOSE:%d,%d", tid, ret);

    response.pdata = result_str;
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;
    response.length = strlen((const char*)response.pdata);
    apb_proxy_send_at_cmd_result(&response);
}


void apb_proxy_tls_conn_conn_handshake_hdlr(apb_proxy_tls_msg_struct *msg)
{
    apb_proxy_tls_conn_struct *tls_conn = NULL;
    int ret = 0;

    if (!msg || !g_apb_tls_cntx_ptr)
    {
        return;
    }

    tls_conn = (apb_proxy_tls_conn_struct *)apb_proxy_tls_list_find((apb_proxy_tls_template_struct *)g_apb_tls_cntx_ptr->tls_conn_list, msg->tid);
    if (!tls_conn)
    {
        ret = -1;
        goto exit;
    }

    ret = apb_proxy_tls_conn_handshake(tls_conn);
    if (0 == ret)
    {
        tls_conn->state &= ~APB_PROXY_TLS_STATE_CONNECTING;
        tls_conn->state |= APB_PROXY_TLS_STATE_CONNECTED;
    }
    else if (MBEDTLS_ERR_SSL_WANT_READ == ret || MBEDTLS_ERR_SSL_WANT_WRITE == ret)
    {
        apb_proxy_tls_send_msg(msg->tid,
                               APB_PROXY_TLS_OP_CONN_HANDSHAKE,
                               msg->cid,
                               NULL,
                               0,
                               APB_PROXY_TLS_ENCODE_TYPE_NONE,
#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
                               0,
#endif
                               msg->cmd_id);
        return;
    }

exit:
    if (0 == ret)
    {
        apb_proxy_tls_send_etlsconn_urc(msg->tid, 1);
    }
    else
    {
        if (tls_conn)
        {
            apb_proxy_tls_conn_node_free(tls_conn);
        }
        apb_proxy_tls_send_etlsconn_urc(msg->tid, ret);
    }
}


void apb_proxy_tls_conn_conn_hdlr(apb_proxy_tls_msg_struct *msg)
{
    apb_proxy_tls_param_struct *tls_param = NULL;
    apb_proxy_tls_conn_struct *tls_conn = NULL;
    int ret = 0;

    if (!msg || !g_apb_tls_cntx_ptr)
    {
        return;
    }

    /* Duplicate connection request. */
    tls_conn = (apb_proxy_tls_conn_struct *)apb_proxy_tls_list_find((apb_proxy_tls_template_struct *)g_apb_tls_cntx_ptr->tls_conn_list, msg->tid);
    if (tls_conn)
    {
        ret = -1;
        goto exit;
    }

    tls_conn = apb_proxy_tls_conn_node_create(msg->tid, msg->cid);
    if (!tls_conn)
    {
        ret = -2;
        goto exit;
    }

    tls_param = (apb_proxy_tls_param_struct *)apb_proxy_tls_list_find((apb_proxy_tls_template_struct *)g_apb_tls_param_list, msg->tid);
    if (!tls_param)
    {
        ret = -3;
        goto exit;
    }

    ret = apb_proxy_tls_conn_conn(tls_conn, tls_param);

    if (0 != ret)
    {
        goto exit;
    }

    ret = apb_proxy_tls_conn_handshake(tls_conn);
    if (0 == ret)
    {
        tls_conn->state &= ~APB_PROXY_TLS_STATE_CONNECTING;
        tls_conn->state |= APB_PROXY_TLS_STATE_CONNECTED;
    }
    else if (MBEDTLS_ERR_SSL_WANT_READ == ret || MBEDTLS_ERR_SSL_WANT_WRITE == ret)
    {
        apb_proxy_tls_send_msg(msg->tid,
                               APB_PROXY_TLS_OP_CONN_HANDSHAKE,
                               msg->cid,
                               NULL,
                               0,
                               APB_PROXY_TLS_ENCODE_TYPE_NONE,
   #ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
                               0,
   #endif
                               msg->cmd_id);
        return;
    }

exit:
    if (0 == ret)
    {
        apb_proxy_tls_send_etlsconn_urc(msg->tid, 1);
    }
    else
    {
        apb_proxy_tls_conn_node_free(tls_conn);
        apb_proxy_tls_send_etlsconn_urc(msg->tid, ret);
    }
}


void apb_proxy_tls_conn_close_hdlr(apb_proxy_tls_msg_struct *msg)
{
    apb_proxy_tls_conn_struct *tls_conn = NULL;

    if (!msg || !g_apb_tls_cntx_ptr)
    {
        return;
    }

    tls_conn = (apb_proxy_tls_conn_struct *)apb_proxy_tls_list_find((apb_proxy_tls_template_struct *)g_apb_tls_cntx_ptr->tls_conn_list, msg->tid);
    if (tls_conn)
    {
        apb_proxy_tls_conn_close(tls_conn);
        apb_proxy_tls_conn_node_free(tls_conn);
    }

    apb_proxy_tls_send_etlsclose_urc(msg->tid, 1);
}


void apb_proxy_tls_conn_send_hdlr(apb_proxy_tls_msg_struct *msg)
{
    apb_proxy_tls_conn_struct *tls_conn = NULL;
    int ret = 0;
    
    if (!msg || !msg->data || !msg->len || !g_apb_tls_cntx_ptr)
    {
        return;
    }

    tls_conn = (apb_proxy_tls_conn_struct *)apb_proxy_tls_list_find((apb_proxy_tls_template_struct *)g_apb_tls_cntx_ptr->tls_conn_list, msg->tid);
    if (!tls_conn)
    {
        /* Failed */
        ret = -1;
        goto exit;
    }

    ret = apb_proxy_tls_conn_send(tls_conn, (const char *)msg->data, msg->len);
    tls_conn->state &= ~(APB_PROXY_TLS_STATE_SENDING);
exit:
    /* Free data */
    vPortFree(msg->data);

    apb_proxy_tls_send_etlssend_urc(msg->tid, ret);
}


void apb_proxy_tls_conn_recv_hdlr(apb_proxy_tls_msg_struct *msg)
{
    apb_proxy_tls_conn_struct *tls_conn = NULL;
    int ret = 0;
    char *buf = NULL;    
    char *encoded_buf = NULL;
    size_t olen = 0;
#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
    apb_proxy_at_cmd_result_t cmd_result = {0};
#endif

    if (!msg || !msg->len || !g_apb_tls_cntx_ptr)
    {
        return;
    }

    TLS_AT_CMD_LOG_INFO("msg->len:%d", msg->len);
    assert(msg->len <= APB_PROXY_MAX_DATA_SIZE);
#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
#endif
    tls_conn = (apb_proxy_tls_conn_struct *)apb_proxy_tls_list_find((apb_proxy_tls_template_struct *)g_apb_tls_cntx_ptr->tls_conn_list, msg->tid);
    if (!tls_conn)
    {
        /* Failed */
        ret = -1;
        goto exit;
    }

    buf = (char *)pvPortCalloc(1, msg->len + 1);
    if (!buf)
    {
        ret = -2;
        goto exit;
    }

#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
    mbedtls_ssl_conf_read_timeout(&tls_conn->ssl_conf, msg->time_out);
#endif

    ret = apb_proxy_tls_conn_recv(tls_conn,
                                  buf,
                                  msg->len,
                                  msg->encode_type);
    TLS_AT_CMD_LOG_INFO("recv_len:%d ret:-0x%x, msg->len:%d, msg->encode_type:%d", ret > 0 ? ret : 0, -ret, msg->len, msg->encode_type);

exit:
    if (MBEDTLS_ERR_SSL_TIMEOUT == ret)
    {
#ifndef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
        apb_proxy_tls_send_msg(tls_conn->tid,
                               APB_PROXY_TLS_OP_READ,
                               0,
                               NULL,
                               msg->len,
                               msg->encode_type,
                               msg->cmd_id);
        if (buf)
        {
            vPortFree(buf);
        }

        return;
#else
        cmd_result.result_code = APB_PROXY_RESULT_OK;
#endif
    }
    else if (0 < ret)
#ifndef TLS_AT_CMD_SUPPORT_DATA_MODE
    {
        int recv_len = ret;
        /* Encode data */
        ret = apb_proxy_tls_encode(NULL,
                                   0,
                                   &olen,
                                   (const char *)buf,
                                   recv_len,
                                   msg->encode_type);

        if (0 == ret)
        {
            encoded_buf = (char *)pvPortCalloc(1, olen);
            if (!encoded_buf)
            {
                TLS_AT_CMD_LOG_INFO("alloc failed line:%d", __LINE__);
                ret = -3;
            }
            else
            {
                ret = apb_proxy_tls_encode(encoded_buf,
                                           olen,
                                           &olen,
                                           (const char *)buf,
                                           recv_len,
                                           msg->encode_type);
                if (0 == ret)
                {
#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
                    cmd_result.result_code = APB_PROXY_RESULT_OK;
#endif
                }
            }
        }
    }
    
    /* Send URC */
    apb_proxy_tls_send_etlsrecv_urc(msg->tid,
                                    (0 == ret && encoded_buf) ? olen : ret,
                                    msg->encode_type,
                                    (0 == ret && encoded_buf) ? encoded_buf : NULL);
#else
    {
        cmd_result.result_code = APB_PROXY_RESULT_CONNECT;
    }

    /* Send URC */
    apb_proxy_tls_send_etlsrecv_urc(msg->tid,
                                    ret,
                                    msg->encode_type,
                                    NULL);
    
#endif

#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
    if (cmd_result.pdata)
    {
        cmd_result.length = strlen((const char*)cmd_result.pdata);
    }
    cmd_result.cmd_id = msg->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
#endif

    if (tls_conn)
    {
        tls_conn->state &= ~(APB_PROXY_TLS_STATE_RECEIVING);
    }

#ifdef TLS_AT_CMD_SUPPORT_DATA_MODE
    cmd_result.result_code = APB_PROXY_RESULT_OK;
    g_apb_tls_cntx_ptr->data_mode_conn_id = apb_proxy_create_data_mode(
                                                    apb_proxy_tls_data_mode_event_callback,
                                                    msg->cmd_id);
    if (g_apb_tls_cntx_ptr->data_mode_conn_id == 0)                
    {
        TLS_AT_CMD_LOG_INFO("apb_proxy_create_data_mode returned 0.");
        cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    }
    else
    {
        g_apb_tls_cntx_ptr->data_mode_type = APB_PROXY_TLS_DATA_MODE_TYPE_RECV_DATA;
        g_apb_tls_cntx_ptr->data_mode_tid = msg->tid;
        g_apb_tls_cntx_ptr->data_mode_cmd_id = msg->cmd_id;
        
        apb_proxy_user_data_t p_user_data = {0};
        p_user_data.length = strlen((const char*)buf);
        p_user_data.pbuffer = (void *)buf;
        
        if(apb_proxy_send_user_data(g_apb_tls_cntx_ptr->data_mode_conn_id, &p_user_data) != APB_PROXY_STATUS_OK){
           TLS_AT_CMD_LOG_INFO("Send user data failed.");
           cmd_result.result_code = APB_PROXY_RESULT_ERROR;
        }

        configASSERT(apb_proxy_close_data_mode(g_apb_tls_cntx_ptr->data_mode_conn_id) == APB_PROXY_STATUS_OK);
        g_apb_tls_cntx_ptr->data_mode_conn_id = 0;        
    } 
    if (APB_PROXY_RESULT_OK != cmd_result.result_code)
    {
        apb_proxy_send_at_cmd_result(&cmd_result);
    }
#endif

    if (buf)
    {
        vPortFree(buf);
    }

    if (encoded_buf)
    {
        vPortFree(encoded_buf);
    }
}


static void apb_proxy_tls_task_main(void *param)
{
    apb_proxy_tls_msg_struct *msg = NULL;

    while (g_apb_tls_cntx_ptr)
    {
        if (xQueueReceive(g_apb_tls_cntx_ptr->queue_hdl, &msg, portMAX_DELAY) == pdPASS)
        {
            switch (msg->msg_id)
            {
                case APB_PROXY_TLS_MSG_ID_OP_REQ:
                {
                    switch (msg->op)
                    {
                        case APB_PROXY_TLS_OP_CONN:
                        {
                            apb_proxy_tls_conn_conn_hdlr(msg);
                            break;
                        }

                        case APB_PROXY_TLS_OP_CONN_HANDSHAKE:
                        {
                            apb_proxy_tls_conn_conn_handshake_hdlr(msg);
                            break;
                        }                        

                        case APB_PROXY_TLS_OP_CLOSE:
                        {
                            apb_proxy_tls_conn_close_hdlr(msg);
                            break;
                        }

                        case APB_PROXY_TLS_OP_SEND:
                        {
                            apb_proxy_tls_conn_send_hdlr(msg);
                            break;
                        }

                        case APB_PROXY_TLS_OP_READ:
                        {
                            apb_proxy_tls_conn_recv_hdlr(msg);
                            break;
                        }

                        default:
                            break;
                    }
                    break;
                }

                default:
                    break;
            }

            /* Free message */
            vPortFree(msg);
            msg = NULL;
        }
    }
}


static void apb_proxy_tls_task_create(void)
{
    if (g_apb_tls_cntx_ptr &&!g_apb_tls_cntx_ptr->queue_hdl)
    {
        g_apb_tls_cntx_ptr->queue_hdl = xQueueCreate(APB_PROXY_TLS_QUEUE_MAX_SIZE, sizeof(apb_proxy_tls_msg_struct *));
    }

    if (g_apb_tls_cntx_ptr &&!g_apb_tls_cntx_ptr->task_hdl)
    {
        xTaskCreate(apb_proxy_tls_task_main,
                    APB_PROXY_TLS_TASK_NAME,
                    APB_PROXY_TLS_TASK_STACKSIZE / sizeof(portSTACK_TYPE),
                    NULL,
                    APB_PROXY_TLS_TASK_PRIO,
                    &g_apb_tls_cntx_ptr->task_hdl);
    }
}


void apb_proxy_tls_cntx_init(void)
{
    if (!g_apb_tls_cntx_ptr)
    {
        g_apb_tls_cntx_ptr = &g_apb_tls_cntx;
        TLS_AT_CMD_LOG_INFO("Initialize cntx for tls at cmd.");
        memset(g_apb_tls_cntx_ptr, 0, sizeof(apb_proxy_tls_cntx_struct));
        g_apb_tls_cntx_ptr->mutex_hdl = xSemaphoreCreateMutex();
#ifdef TLS_AT_CMD_SUPPORT_DATA_MODE        
        TLS_AT_CMD_LOG_INFO("data_mode_conn_id:%d %x",
            g_apb_tls_cntx_ptr->data_mode_conn_id,
            &g_apb_tls_cntx_ptr->data_mode_conn_id);
#endif
    }
}
#endif


apb_proxy_status_t apb_proxy_hdlr_etlscfg_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};

    TLS_AT_CMD_LOG_INFO("%s, p_parse_cmd:%x", __FUNCTION__, p_parse_cmd);
    configASSERT(p_parse_cmd != NULL);
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

#ifdef TLS_AT_CMD_SUPPORT
    switch (p_parse_cmd->mode)
    {
        case APB_PROXY_CMD_MODE_READ:   // rec: AT+ETLSCFG?
        {
            TLS_AT_CMD_LOG_INFO("Read mode is not supported.");            
            break;
        }

        case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+ETLSCFG=?
        {
            TLS_AT_CMD_LOG_INFO("TEST mode is not supported.");
            break;
        }

        case APB_PROXY_CMD_MODE_ACTIVE: // rec: AT+ETLSCFG
        {
            TLS_AT_CMD_LOG_INFO("ACTIVE mode is not supported.");
            break;
        }

        case APB_PROXY_CMD_MODE_INVALID:
        {
            TLS_AT_CMD_LOG_INFO("INVALID mode is not supported.");
            break;
        }

        case APB_PROXY_CMD_MODE_EXECUTION: // rec: AT+ETLSCFG=<tid>,<type>,<value>  the handler need to parse the parameters
        {
            apb_proxy_tls_ret_enum ret = false;
            char *param = NULL;

            param = p_parse_cmd->string_ptr + p_parse_cmd->name_len + 1;
            TLS_AT_CMD_LOG_INFO("param to parse: len:%d, content:%s", p_parse_cmd->string_len, param);

            apb_proxy_tls_cntx_init();

            ret = apb_proxy_tls_process_cfg_params(param, p_parse_cmd->cmd_id);
            TLS_AT_CMD_LOG_INFO("ret:%d", ret);

            if (APB_PROXY_TLS_RET_OK == ret)
            {
                cmd_result.result_code = APB_PROXY_RESULT_OK;
            }
#ifdef TLS_AT_CMD_SUPPORT_DATA_MODE
            else if (APB_PROXY_TLS_RET_CONNECT == ret)
            {
                cmd_result.result_code = APB_PROXY_RESULT_CONNECT;
                cmd_result.cmd_id = p_parse_cmd->cmd_id;
                if (APB_PROXY_STATUS_OK != apb_proxy_send_at_cmd_result(&cmd_result))
                {
                    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                    break;
                }

                g_apb_tls_cntx_ptr->data_mode_conn_id = apb_proxy_create_data_mode(
                                                        apb_proxy_tls_data_mode_event_callback,
                                                        p_parse_cmd->cmd_id);
                if (g_apb_tls_cntx_ptr->data_mode_conn_id == 0)                
                {
                    TLS_AT_CMD_LOG_INFO("apb_proxy_create_data_mode returned 0.");
                    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                    break;
                }
                else
                {
                    return APB_PROXY_STATUS_OK;
                }
            }
#endif
            break;
        }

        default:
            break;
    }
#else
    cmd_result.pdata = "Not enabled";
#endif

    if (cmd_result.pdata)
    {
        cmd_result.length = strlen((const char*)cmd_result.pdata);
    }
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}


apb_proxy_status_t apb_proxy_hdlr_etlsconn_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};

    TLS_AT_CMD_LOG_INFO("%s, p_parse_cmd:%x", __FUNCTION__, p_parse_cmd);
    configASSERT(p_parse_cmd != NULL);
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

#ifdef TLS_AT_CMD_SUPPORT
    switch (p_parse_cmd->mode)
    {
        case APB_PROXY_CMD_MODE_READ:   // rec: AT+ETLSCFG?
        {
            TLS_AT_CMD_LOG_INFO("Read mode is not supported.");            
            break;
        }

        case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+ETLSCFG=?
        {
            TLS_AT_CMD_LOG_INFO("TEST mode is not supported.");
            break;
        }

        case APB_PROXY_CMD_MODE_ACTIVE: // rec: AT+ETLSCFG
        {
            TLS_AT_CMD_LOG_INFO("ACTIVE mode is not supported.");
            break;
        }

        case APB_PROXY_CMD_MODE_INVALID:
        {
            TLS_AT_CMD_LOG_INFO("INVALID mode is not supported.");
            break;
        }

        case APB_PROXY_CMD_MODE_EXECUTION: // rec: AT+ETLSCONN=<tid>,<cid>  the handler need to parse the parameters
        {
            bool ret = false;
            char *param = NULL;
            int tid = 0, cid = 0;
            apb_proxy_tls_param_struct *tls_param = NULL;

            param = p_parse_cmd->string_ptr + p_parse_cmd->name_len + 1;
            TLS_AT_CMD_LOG_INFO("param to parse: len:%d, content:%s", p_parse_cmd->string_len, param);
            /* Parse <tid> */
            param = strtok(param, (const char *)",");
            if (!param)
            {
                TLS_AT_CMD_LOG_INFO("Invalid parameter. param:%s", param);
                break;
            }

            tid = atoi(param);
            if (!apb_proxy_tls_is_valid_tid(tid))
            {
                TLS_AT_CMD_LOG_INFO("Invalid tid:%d", tid);
                break;
            }

            /* Parse <cid> */
            param = strtok(NULL, (const char *)",");
            if (!param)
            {
                TLS_AT_CMD_LOG_INFO("No cid");
                break;
            }

            cid = atoi(param);
#ifndef TLS_AT_CMD_UT
            if (!tel_conn_mgr_is_cid_active(cid))
            {
                TLS_AT_CMD_LOG_ERR("The PDN Context is not active. cid:%d", cid);
                break;
            }
#endif
            tls_param = (apb_proxy_tls_param_struct *)apb_proxy_tls_list_find((apb_proxy_tls_template_struct *)g_apb_tls_param_list, tid);
            if (!tls_param)
            {
                break;
            }

            if (!apb_proxy_tls_param_is_param_ready(tls_param) ||
                tls_param->is_using)
            {
                /* Parameters are not ready or connection req has been sent before. */
                TLS_AT_CMD_LOG_INFO("TLS params are not ready or in use:%d.", tls_param->is_using);
                break;
            }

            if (APB_PROXY_TLS_STATE_MAX != apb_proxy_tls_conn_get_state(tid))
            {
                /* There is a tls conn of tid. */
                TLS_AT_CMD_LOG_INFO("State error:%x", apb_proxy_tls_conn_get_state(tid));
                break;
            }

            apb_proxy_tls_task_create();

            ret = apb_proxy_tls_send_msg(tid,
                                         APB_PROXY_TLS_OP_CONN,                                         
                                         cid,
                                         NULL,
                                         0,
                                         APB_PROXY_TLS_ENCODE_TYPE_NONE,
#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
                                         0,
#endif
                                         cmd_result.cmd_id);
            if (ret)
            {
                tls_param->is_using++;
                cmd_result.result_code = APB_PROXY_RESULT_OK;
            }
            
            break;
        }

        default:
            break;
    }
#else
    cmd_result.pdata = "Not enabled";
#endif

    if (cmd_result.pdata)
    {
        cmd_result.length = strlen((const char*)cmd_result.pdata);
    }
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}


apb_proxy_status_t apb_proxy_hdlr_etlsclose_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};

    TLS_AT_CMD_LOG_INFO("%s, p_parse_cmd:%x", __FUNCTION__, p_parse_cmd);
    configASSERT(p_parse_cmd != NULL);
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

#ifdef TLS_AT_CMD_SUPPORT
    switch (p_parse_cmd->mode)
    {
        case APB_PROXY_CMD_MODE_READ:   // rec: AT+ETLSCFG?
        {
            TLS_AT_CMD_LOG_INFO("Read mode is not supported.");            
            break;
        }

        case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+ETLSCFG=?
        {
            TLS_AT_CMD_LOG_INFO("TEST mode is not supported.");
            break;
        }

        case APB_PROXY_CMD_MODE_ACTIVE: // rec: AT+ETLSCFG
        {
            TLS_AT_CMD_LOG_INFO("ACTIVE mode is not supported.");
            break;
        }

        case APB_PROXY_CMD_MODE_INVALID:
        {
            TLS_AT_CMD_LOG_INFO("INVALID mode is not supported.");
            break;
        }

        case APB_PROXY_CMD_MODE_EXECUTION: // rec: AT+ETLSCONN=<tid>,<cid>  the handler need to parse the parameters
        {
            bool ret = false;
            char *param = NULL;
            int tid = 0;
            apb_proxy_tls_state_enum tls_conn_state = APB_PROXY_TLS_STATE_NONE;

            param = p_parse_cmd->string_ptr + p_parse_cmd->name_len + 1;
            TLS_AT_CMD_LOG_INFO("param to parse: len:%d, content:%s", p_parse_cmd->string_len, param);

            /* Parse <tid> */
            tid = atoi(param);
            if (!apb_proxy_tls_is_valid_tid(tid))
            {
                break;
            }

            tls_conn_state = apb_proxy_tls_conn_get_state(tid);
            if (APB_PROXY_TLS_STATE_MAX == tls_conn_state ||
                tls_conn_state & APB_PROXY_TLS_STATE_CLOSING)
            {
                /* There is no tls conn of tid. */
                TLS_AT_CMD_LOG_INFO("Wrong state:%x", tls_conn_state);
                break;
            }

            apb_proxy_tls_conn_set_state(tid, APB_PROXY_TLS_STATE_CLOSING);

            ret = apb_proxy_tls_send_msg(tid,
                                         APB_PROXY_TLS_OP_CLOSE,                                         
                                         0,
                                         NULL,
                                         0,
                                         APB_PROXY_TLS_ENCODE_TYPE_NONE,
#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
                                         0,
#endif
                                         cmd_result.cmd_id);
            if (ret)
            {
                cmd_result.result_code = APB_PROXY_RESULT_OK;
                //return APB_PROXY_STATUS_OK;
            }
            else
            {
                apb_proxy_tls_conn_clear_state(tid, APB_PROXY_TLS_STATE_CLOSING);
            }                
            
            break;
        }

        default:
            break;
    }
#else
    cmd_result.pdata = "Not enabled";
#endif

    if (cmd_result.pdata)
    {
        cmd_result.length = strlen((const char*)cmd_result.pdata);
    }
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}


apb_proxy_status_t apb_proxy_hdlr_etlssend_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};

    TLS_AT_CMD_LOG_INFO("%s, p_parse_cmd:%x", __FUNCTION__, p_parse_cmd);
    configASSERT(p_parse_cmd != NULL);
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

#ifdef TLS_AT_CMD_SUPPORT
    switch (p_parse_cmd->mode)
    {
        case APB_PROXY_CMD_MODE_READ:   // rec: AT+ETLSCFG?
        {
            TLS_AT_CMD_LOG_INFO("Read mode is not supported.");            
            break;
        }

        case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+ETLSCFG=?
        {
            TLS_AT_CMD_LOG_INFO("TEST mode is not supported.");
            break;
        }

        case APB_PROXY_CMD_MODE_ACTIVE: // rec: AT+ETLSCFG
        {
            TLS_AT_CMD_LOG_INFO("ACTIVE mode is not supported.");
            break;
        }

        case APB_PROXY_CMD_MODE_INVALID:
        {
            TLS_AT_CMD_LOG_INFO("INVALID mode is not supported.");
            break;
        }

        case APB_PROXY_CMD_MODE_EXECUTION: // rec: AT+ETLSCONN=<tid>,<cid>  the handler need to parse the parameters
        {
            bool ret = false;
            char *param_str = NULL, *param = NULL, *param_str_tmp = NULL, *data = NULL;
            int tid = 0, data_len = 0, param_str_len = 0, ret_int = -1;
            apb_proxy_tls_encode_type_enum encode_type = APB_PROXY_TLS_ENCODE_TYPE_DEFAULT;
            apb_proxy_tls_state_enum tls_conn_state = APB_PROXY_TLS_STATE_NONE;
            char *decoded_buf = NULL;
            size_t olen = 0;

            param_str = p_parse_cmd->string_ptr + p_parse_cmd->name_len + 1;
            TLS_AT_CMD_LOG_INFO("param to parse: len:%d, content:%s", p_parse_cmd->string_len, param_str);

            param_str_len = strlen((const char*)param_str);
            
            /* Parse <tid> */
            param = strtok(param_str, (const char *)",");
            if (!param)
            {
                TLS_AT_CMD_LOG_INFO("Invalid parameter.");
                break;
            }

            tid = atoi(param);
            if (!apb_proxy_tls_is_valid_tid(tid))
            {
                TLS_AT_CMD_LOG_INFO("invalid tid:%d", tid);
                break;
            }
            
            tls_conn_state = apb_proxy_tls_conn_get_state(tid);

            if (!(APB_PROXY_TLS_STATE_CONNECTED & tls_conn_state) ||
                (APB_PROXY_TLS_STATE_SENDING & tls_conn_state))
            {
                TLS_AT_CMD_LOG_INFO("wrong tls_conn_state:%x", tls_conn_state);
                break;
            }

            /* Parse <data_len> */
            param = strtok(NULL, (const char *)",");
            if (!param)
            {
                TLS_AT_CMD_LOG_INFO("invalid format: data_len.");
                break;
            }

            data_len = atoi(param);
            if (!data_len)
            {
                TLS_AT_CMD_LOG_ERR("Invalid data_len:%d", data_len);
                break;
            }

#ifdef TLS_AT_CMD_SUPPORT_DATA_MODE
            cmd_result.result_code = APB_PROXY_RESULT_CONNECT;
            cmd_result.cmd_id = p_parse_cmd->cmd_id;
            if (APB_PROXY_STATUS_OK != apb_proxy_send_at_cmd_result(&cmd_result))
            {
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                break;
            }

            g_apb_tls_cntx_ptr->data_mode_conn_id = apb_proxy_create_data_mode(
                                                    apb_proxy_tls_data_mode_event_callback,
                                                    p_parse_cmd->cmd_id);
            if (g_apb_tls_cntx_ptr->data_mode_conn_id == 0)                
            {
                TLS_AT_CMD_LOG_INFO("apb_proxy_create_data_mode returned 0.");
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                break;
            }
            else
            {
                g_apb_tls_cntx_ptr->data_mode_type = APB_PROXY_TLS_DATA_MODE_TYPE_SEND_DATA;
                g_apb_tls_cntx_ptr->data_mode_tid = tid;
                g_apb_tls_cntx_ptr->data_mode_cmd_id = p_parse_cmd->cmd_id;
                g_apb_tls_cntx_ptr->data_mode_data_len = data_len;
                return APB_PROXY_STATUS_OK;
            }            
#else
            /* Parse <data> */
            param_str_tmp = param + strlen((const char*)param) + 1;
            if (param_str_tmp > param_str + param_str_len ||
                *param_str_tmp != '"')
            {
                TLS_AT_CMD_LOG_INFO("invalid format: data. param_str_tmp:%x, param_str + param_str_len:%x, param_str_tmp:%c",
                    param_str_tmp, param_str + param_str_len, *param_str_tmp);
                break;
            }

#if 1
            param = apb_proxy_nw_parse_string_param(&param_str_tmp, param_str + param_str_len - 1);
            if (!param)
            {
                TLS_AT_CMD_LOG_INFO("Invalid format.");
                break;
            }

            data = param;
#else                

            /* Skip " */
            param = ++param_str_tmp;


            while (param_str_tmp < param_str + param_str_len &&
                   !(*(param_str_tmp - 1) != '\\' && *param_str_tmp == '"' &&
                   (*(param_str_tmp + 1) == ',' || *(param_str_tmp + 1) == '\0' ||
                    *(param_str_tmp + 1) == '\r' || *(param_str_tmp + 1) == '\n')))
            {
                param_str_tmp++;
            }

            TLS_AT_CMD_LOG_INFO("line: %d", __LINE__);

            if (param_str_tmp < param_str + param_str_len &&
                (*(param_str_tmp - 1) != '\\' && *param_str_tmp == '"' &&
                (*(param_str_tmp + 1) == ',' || *(param_str_tmp + 1) == '\0' ||
                 *(param_str_tmp + 1) == '\r' || *(param_str_tmp + 1) == '\n')))
            {
                TLS_AT_CMD_LOG_INFO("line:%d, param_str_tmp:%x, *param_str_tmp:%d", __LINE__, data, param_str_tmp ? param_str_tmp : 0, *param_str_tmp);
                *param_str_tmp = '\0';
                if (*(param_str_tmp + 1) == ',')
                {
                    param_str_tmp += 2;
                }
                else
                {
                    param_str_tmp = NULL;
                }

                /* Skip " */
                data = param;
                TLS_AT_CMD_LOG_INFO("line:%d, data: %s, param_str_tmp:%x", __LINE__, data, param_str_tmp ? param_str_tmp : 0);
            }
            else
            {
                TLS_AT_CMD_LOG_INFO("invalid format of data line: %d", __LINE__);
                break;
            }
#endif
            /* Parse <encode_type>. */
            if (param_str_tmp && *param_str_tmp != '\0')
            {
                param = strtok(param_str_tmp, (const char *)",");
            }
            else
            {
                param = NULL;
            }

            if (param)
            {
                TLS_AT_CMD_LOG_INFO("line:%d, encode_type included", __LINE__);
                encode_type = (apb_proxy_tls_encode_type_enum)atoi(param);
            }

            TLS_AT_CMD_LOG_INFO("line:%d, encode_type: %d", __LINE__, encode_type);

            data_len = strlen((const char*)data);
        
            /* Decode */
            ret_int = apb_proxy_tls_decode(NULL,
                                           0,
                                           &olen,
                                           (const char *)data,
                                           data_len,
                                           encode_type);

            if (0 != ret_int)
            {
                TLS_AT_CMD_LOG_INFO("decode failed. ret_int:%d", ret_int);
                break;
            }

            decoded_buf = (char *)pvPortCalloc(1, olen);

            if (!decoded_buf)
            {
                break;
            }

            ret_int = apb_proxy_tls_decode(decoded_buf,
                                           olen,
                                           &olen,
                                           (const char *)data,
                                           data_len,
                                           encode_type);            

            if (0 != ret_int)
            {
                TLS_AT_CMD_LOG_INFO("decode failed.");
                vPortFree(decoded_buf);
                break;
            }

            apb_proxy_tls_conn_set_state(tid, APB_PROXY_TLS_STATE_SENDING);

            ret = apb_proxy_tls_send_msg(tid,
                                         APB_PROXY_TLS_OP_SEND,                                         
                                         0,
                                         decoded_buf,
                                         olen,
                                         APB_PROXY_TLS_ENCODE_TYPE_NONE,
#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
                                         0,
#endif
                                         cmd_result.cmd_id);
            if (ret)
            {                
                cmd_result.result_code = APB_PROXY_RESULT_OK;
            }
            else
            {
                apb_proxy_tls_conn_clear_state(tid, APB_PROXY_TLS_STATE_SENDING);

                vPortFree(decoded_buf);
            }
#endif
            break;
        }

        default:
            break;
    }
#else
    cmd_result.pdata = "Not enabled";
#endif

    if (cmd_result.pdata)
    {
        cmd_result.length = strlen((const char*)cmd_result.pdata);
    }
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}


apb_proxy_status_t apb_proxy_hdlr_etlsrecv_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};

    TLS_AT_CMD_LOG_INFO("%s, p_parse_cmd:%x", __FUNCTION__, p_parse_cmd);
    configASSERT(p_parse_cmd != NULL);
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

#ifdef TLS_AT_CMD_SUPPORT
    switch (p_parse_cmd->mode)
    {
        case APB_PROXY_CMD_MODE_READ:   // rec: AT+ETLSCFG?
        {
            TLS_AT_CMD_LOG_INFO("Read mode is not supported.");            
            break;
        }

        case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+ETLSCFG=?
        {
            TLS_AT_CMD_LOG_INFO("TEST mode is not supported.");
            break;
        }

        case APB_PROXY_CMD_MODE_ACTIVE: // rec: AT+ETLSCFG
        {
            TLS_AT_CMD_LOG_INFO("ACTIVE mode is not supported.");
            break;
        }

        case APB_PROXY_CMD_MODE_INVALID:
        {
            TLS_AT_CMD_LOG_INFO("INVALID mode is not supported.");
            break;
        }

        case APB_PROXY_CMD_MODE_EXECUTION: // rec: AT+ETLSCONN=<tid>,<cid>  the handler need to parse the parameters
        {
            bool ret = false;
            char *param = NULL;
            int tid = 0, max_num = 0;
#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
            int time_out = 0;
#endif
            apb_proxy_tls_encode_type_enum encode_type = APB_PROXY_TLS_ENCODE_TYPE_DEFAULT;
            apb_proxy_tls_state_enum tls_conn_state = APB_PROXY_TLS_STATE_NONE;

            param = p_parse_cmd->string_ptr + p_parse_cmd->name_len + 1;
            TLS_AT_CMD_LOG_INFO("param to parse: len:%d, content:%s", p_parse_cmd->string_len, param);
            
            /* Parse <tid> */
            param = strtok(param, (const char *)",");
            if (!param)
            {
                TLS_AT_CMD_LOG_INFO("Invalid parameter");
                break;
            }

            tid = atoi(param);
            if (!apb_proxy_tls_is_valid_tid(tid))
            {
                break;
            }
            
            tls_conn_state = apb_proxy_tls_conn_get_state(tid);

            if (!(APB_PROXY_TLS_STATE_CONNECTED & tls_conn_state) ||
                (APB_PROXY_TLS_STATE_RECEIVING & tls_conn_state))
            {
                TLS_AT_CMD_LOG_INFO("wrong tls_conn_state:%x", tls_conn_state);
                break;
            }

            /* Parse <max_num> */
            param = strtok(NULL, (const char *)",");
            if (!param)
            {
                break;
            }
            max_num = atoi(param);
            if (APB_PROXY_MAX_DATA_SIZE < max_num)
            {
                break;
            }

#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
            /* Parse <time_out> */
            param = strtok(NULL, (const char *)",");
            if (!param)
            {
                break;
            }
            time_out = atoi(param);
#endif

            /* Parse <encode_type> */
            param = strtok(NULL, (const char *)",");
            if (param)
            {
                encode_type = atoi(param);
            }

            apb_proxy_tls_conn_set_state(tid, APB_PROXY_TLS_STATE_RECEIVING);
            ret = apb_proxy_tls_send_msg(tid,
                                         APB_PROXY_TLS_OP_READ,                                         
                                         0,
                                         NULL,
                                         max_num,
                                         encode_type,
#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
                                         time_out,
#endif
                                         cmd_result.cmd_id);
            if (ret)
            {
#ifdef TLS_AT_CMD_SUPPORT_RECV_TIMEOUT
                return APB_PROXY_STATUS_OK;
#else
                cmd_result.result_code = APB_PROXY_RESULT_OK;
#endif
            }
            else
            {
                apb_proxy_tls_conn_clear_state(tid, APB_PROXY_TLS_STATE_RECEIVING);
            }
            break;
        }

        default:
            break;
    }
#else
    cmd_result.pdata = "Not enabled";
#endif

    if (cmd_result.pdata)
    {
        cmd_result.length = strlen((const char*)cmd_result.pdata);
    }

    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    configASSERT(APB_PROXY_STATUS_OK == apb_proxy_send_at_cmd_result(&cmd_result));
    return APB_PROXY_STATUS_OK;
}

