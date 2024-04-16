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


// support TLS
// send data too long, need set more flag

#include "apb_proxy.h"
#include "apb_proxy_nw_cmd.h"
#include "apb_proxy_nw_cmd_util.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "httpclient.h"

#include "syslog.h"

#include "string.h"
#include "lwip/netdb.h"

log_create_module(httpc, PRINT_LEVEL_INFO);

#ifdef APBSOC_MODULE_PRINTF
#define NW_LOG(fmt, ...)               printf("[APB HTTPCLIENT] "fmt, ##__VA_ARGS__)
#else
#define NW_LOG(fmt, ...)               LOG_I(apbnw, "[APB HTTPCLIENT] "fmt, ##__VA_ARGS__)
#endif

#define SUPPORT_HTTPCLIENT_ATCMD_MAX_NUM        5
#define SUPPORT_HTTPCLIENT_ATCMD_MAX_SIZE       1000
#define SUPPORT_HTTPCLIENT_HEADER_BUFFER_SIZE   800
#define SUPPORT_HTTPCLIENT_RSP_BUFFER_SIZE      400

#define HTTP_SCHEME_PATTERN                         ("http")
#define HTTPS_SCHEME_PATTERN                        ("https")

typedef struct _httpclient_atcmd_ {
    bool is_used;
    bool is_receive_data;
    bool is_connected;
    uint8_t httpclient_id;   
    char* host;
    httpclient_t* client;
    httpclient_data_t* httpclient_data;
} httpclient_atcmd_t;
static uint8_t g_httpclient_atcmd_number = 0;

static bool s_task_running = false;
static httpclient_atcmd_t g_httpclient_atcmd_t[SUPPORT_HTTPCLIENT_ATCMD_MAX_NUM] = {
        { 0, 0 ,0, 0, NULL, NULL, NULL },
        { 0, 0 ,0, 0, NULL, NULL, NULL },
        { 0, 0 ,0, 0, NULL, NULL, NULL },
        { 0, 0 ,0, 0, NULL, NULL, NULL },
        { 0, 0 ,0, 0, NULL, NULL, NULL }
};


static SemaphoreHandle_t s_httpclient_mutex = NULL;
void httpclient_mutex_create(void)
{
    if (s_httpclient_mutex == NULL) {
        s_httpclient_mutex = xSemaphoreCreateMutex();
    }
    if (s_httpclient_mutex == NULL) {
        NW_LOG("httpclient_mutex_creat error");
        return;
    }
    NW_LOG("httpclient_mutex_creat success");
}


void httpclient_mutex_take(void)
{
    if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING && s_httpclient_mutex != NULL) {
        if (xSemaphoreTake(s_httpclient_mutex, portMAX_DELAY) == pdFALSE) {
            NW_LOG("httpclient_mutex_take error");
        }
        NW_LOG("httpclient_mutex_take success");
    }
}


void httpclient_mutex_give(void)
{
    if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING && s_httpclient_mutex != NULL) {
        if (xSemaphoreGive(s_httpclient_mutex) == pdFALSE) {
            NW_LOG("httpclient_mutex_give error");
        }
        NW_LOG("httpclient_mutex_give success");
    }
}


void httpclient_atcmd_error_indicate(uint8_t httpclient_id, HTTPCLIENT_RESULT error_code)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};  
    char* result_buf = NULL;

    result_buf = _alloc_buffer(50);
    if (result_buf == NULL) {
        cmd_result.pdata = "httpclient error, memory error !!";
        return;
    } else {
        snprintf(result_buf, 50, "+EHTTPERR:%d,%d", httpclient_id, error_code);
    }
    
    NW_LOG("this is some error from httpclient_id: %d, error_code: %d", httpclient_id, error_code);
    cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
    cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
    cmd_result.pdata = result_buf;
    cmd_result.length = strlen(cmd_result.pdata);
    apb_proxy_send_at_cmd_result(&cmd_result);

    _free_buffer(result_buf);
}

void httpclient_atcmd_receive_indicate(httpclient_atcmd_t* httpclient_atcmd, uint8_t flag)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};  
    char* result_buf = NULL;
    int result_len = 0;

    result_buf = _alloc_buffer(SUPPORT_HTTPCLIENT_ATCMD_MAX_SIZE);
    NW_LOG("before httpclient_atcmd_receive_indicate, result_buf =%x, %d", result_buf, flag);
    if (result_buf == NULL) {
        NW_LOG("httpclient_atcmd_receive_indicate, memory error !!");
    } else {
        if (httpclient_atcmd->httpclient_data->header_buf_len
                && httpclient_atcmd->httpclient_data->header_buf_len <= SUPPORT_HTTPCLIENT_HEADER_BUFFER_SIZE
                && httpclient_atcmd->httpclient_data->header_buf
                && strlen(httpclient_atcmd->httpclient_data->header_buf) > 0) {
            snprintf(result_buf, SUPPORT_HTTPCLIENT_ATCMD_MAX_SIZE, "+EHTTPNMIH:%d,%d,%d,",
                    httpclient_atcmd->httpclient_id,
                    httpclient_atcmd->client->response_code,
                    strlen(httpclient_atcmd->httpclient_data->header_buf));
            result_len = strlen(result_buf);
            memcpy(result_buf + result_len, httpclient_atcmd->httpclient_data->header_buf,
                    httpclient_atcmd->httpclient_data->header_buf_len);
            cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
            cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
            cmd_result.pdata = result_buf;
            cmd_result.length = strlen(result_buf);
            apb_proxy_send_at_cmd_result(&cmd_result);
            // clear buffer
            memset(result_buf, 0, SUPPORT_HTTPCLIENT_ATCMD_MAX_SIZE);
        }
        if (httpclient_atcmd->httpclient_data->content_block_len > 0
                && httpclient_atcmd->httpclient_data->content_block_len <= SUPPORT_HTTPCLIENT_RSP_BUFFER_SIZE
                && httpclient_atcmd->httpclient_data->response_buf
                && strlen(httpclient_atcmd->httpclient_data->response_buf) > 0) {
            snprintf(result_buf, SUPPORT_HTTPCLIENT_ATCMD_MAX_SIZE, "+EHTTPNMIC:%d,%d,%d,%d,",
                    httpclient_atcmd->httpclient_id,
                    flag,
                    httpclient_atcmd->httpclient_data->response_content_len,
                    (httpclient_atcmd->httpclient_data->content_block_len) << 1); // *2
            result_len = strlen(result_buf);
            _get_data_to_hex(result_buf + result_len, (uint8_t *)httpclient_atcmd->httpclient_data->response_buf,
                    httpclient_atcmd->httpclient_data->content_block_len);
            result_len += (httpclient_atcmd->httpclient_data->content_block_len) << 1;
            cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
            cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
            cmd_result.pdata = result_buf;
            cmd_result.length = result_len;
            apb_proxy_send_at_cmd_result(&cmd_result);
        }
    }

    _free_buffer(result_buf);
}

void httpclient_release_client_data(httpclient_atcmd_t* httpclient_atcmd) {
    if (httpclient_atcmd && httpclient_atcmd->httpclient_data) {
        if (httpclient_atcmd->httpclient_data->response_buf) {
            _free_buffer(httpclient_atcmd->httpclient_data->response_buf);
        }
        if (httpclient_atcmd->httpclient_data->header_buf) {
            _free_buffer(httpclient_atcmd->httpclient_data->header_buf);
        }
        _free_buffer(httpclient_atcmd->httpclient_data);
        httpclient_atcmd->httpclient_data = NULL;
    }
}

int httpclient_create_client_data(httpclient_atcmd_t* httpclient_atcmd)
{
    httpclient_data_t* httpclient_data = NULL;
    int ret = 0;

    httpclient_atcmd->httpclient_data = _alloc_buffer(sizeof(httpclient_data_t));
    if (NULL == httpclient_atcmd->httpclient_data) {
        ret = -1;
        NW_LOG("Cannot malloc httpclient_data");
        goto exit;
    }

    httpclient_data = httpclient_atcmd->httpclient_data;

    httpclient_data->header_buf_len = SUPPORT_HTTPCLIENT_HEADER_BUFFER_SIZE;
    httpclient_data->header_buf = _alloc_buffer(SUPPORT_HTTPCLIENT_HEADER_BUFFER_SIZE);
    if (NULL == httpclient_data->header_buf) {
        ret = -1;
        goto exit;
    }

    httpclient_data->response_buf_len = SUPPORT_HTTPCLIENT_RSP_BUFFER_SIZE;
    httpclient_data->response_buf = _alloc_buffer(SUPPORT_HTTPCLIENT_RSP_BUFFER_SIZE);
    if (NULL == httpclient_data->response_buf) {
        ret = -1;
        goto exit;
    }

    exit:
    if (ret != 0) {
        httpclient_release_client_data(httpclient_atcmd);
        NW_LOG("Free httpclient_data");
    }
    return ret;
}

static httpclient_atcmd_t* httpclient_atcmd_get_free_httpclient()
{
    for(int i = 0; i < SUPPORT_HTTPCLIENT_ATCMD_MAX_NUM; i++){
        if (g_httpclient_atcmd_t[i].is_used == 0) {
            memset(&g_httpclient_atcmd_t[i], 0, sizeof(httpclient_atcmd_t));
            g_httpclient_atcmd_t[i].is_used = 1;
            g_httpclient_atcmd_t[i].httpclient_id = i;
            g_httpclient_atcmd_number++;
            return &g_httpclient_atcmd_t[i];
        }
    }

    return NULL;
}

static httpclient_atcmd_t* httpclient_atcmd_find_httpclient(int httpclient_id)
{
    for(int i = 0; i < SUPPORT_HTTPCLIENT_ATCMD_MAX_NUM; i++){
        if (g_httpclient_atcmd_t[i].httpclient_id == httpclient_id && g_httpclient_atcmd_t[i].is_used)
            return &g_httpclient_atcmd_t[i];
    }

    return NULL;
}

static void httpclient_atcmd_remove_httpclient(int httpclient_id)
{    
    for(int i = 0; i < SUPPORT_HTTPCLIENT_ATCMD_MAX_NUM; i++){
        if (g_httpclient_atcmd_t[i].httpclient_id == httpclient_id && g_httpclient_atcmd_t[i].is_used) {
            _free_buffer(g_httpclient_atcmd_t[i].host);
            if (g_httpclient_atcmd_t[i].client) {
                _free_buffer(g_httpclient_atcmd_t[i].client->auth_user);
                _free_buffer(g_httpclient_atcmd_t[i].client->auth_password);
#ifdef MTK_HTTPCLIENT_SSL_ENABLE
                _free_buffer((char *)g_httpclient_atcmd_t[i].client->server_cert);
                _free_buffer((char *)g_httpclient_atcmd_t[i].client->client_cert);
                _free_buffer((char *)g_httpclient_atcmd_t[i].client->client_pk);
#endif
                _free_buffer(g_httpclient_atcmd_t[i].client);
            }
            if (g_httpclient_atcmd_t[i].httpclient_data) {
                httpclient_release_client_data(&g_httpclient_atcmd_t[i]);
            }
            memset(&g_httpclient_atcmd_t[i], 0, sizeof(httpclient_atcmd_t));
            return;
        }
    }
}

static int httpclient_atcmd_get_max_socket_id()
{
    int max_fd = -1;
    for(int i = 0; i < SUPPORT_HTTPCLIENT_ATCMD_MAX_NUM; i++){
        if (g_httpclient_atcmd_t[i].is_used && g_httpclient_atcmd_t[i].client) {
            if(max_fd < g_httpclient_atcmd_t[i].client->socket) {
                max_fd = g_httpclient_atcmd_t[i].client->socket;
            }
        }
    }
    return max_fd;
}


static bool httpclient_atcmd_is_valid_socket_id(int soc_id)
{
    for(int i = 0; i < SUPPORT_HTTPCLIENT_ATCMD_MAX_NUM; i++){
        if (g_httpclient_atcmd_t[i].is_used &&
            g_httpclient_atcmd_t[i].client &&
            g_httpclient_atcmd_t[i].client->socket == soc_id) {
            return true;
        }
    }
    return false;
}


static bool httpclient_atcmd_is_url_valid(char *url)
{
    char *host, *port, *temp;
    char *host_buf, *port_buf;
    int32_t host_len, port_len; 
    bool retval = false;
    struct addrinfo hints;
    struct addrinfo *addr_list = NULL;
    char default_port[4] = {0};
    int32_t debug_ret;
   
    host_buf = port_buf = NULL;
    host_len = port_len = 0;
    if (url == NULL) {
        return false;
    }
    if (!strncmp(url, HTTP_SCHEME_PATTERN, strlen(HTTP_SCHEME_PATTERN))) {
        strcpy(default_port, "80");
    } else if (!strncmp(url, HTTPS_SCHEME_PATTERN, strlen(HTTPS_SCHEME_PATTERN))) {
        strcpy(default_port, "443");
    } else {
        return false;
    }
    host = strstr(url, "://");
    if (host != NULL) {
        host += 3;
        if (*host == '\0' || *host == ':') {
            return false;
        }
    } else {
        return false;
    }
    port = strchr(host, ':');
    if (port != NULL) {
        host_len = port - host;
        port++;
        temp = strchr(port, '/');
        if (temp != NULL) {
            port_len = temp - port;
        } else {
            port_len = strlen(port);
        }
    } else {
        temp = strchr(host, '/');
        if (temp != NULL) {
            host_len = temp - host;
        } else {
            host_len = strlen(host);
        }
    }
    if (host_len > 0) {
        host_buf = _alloc_buffer(host_len + 1);
        if (host_buf == NULL) {
            retval = false;
            goto final;
        }
        memcpy(host_buf, host, host_len);
        host_buf[host_len] = '\0';
    }
    if (port_len > 0) {
        port_buf = _alloc_buffer(port_len + 1);
        if (port_buf == NULL) {
            retval = false;
            goto final;
        }
        memcpy(port_buf, port, port_len);
        port_buf[port_len] = '\0';
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if ((debug_ret = getaddrinfo(host_buf, port_buf == NULL ? default_port : port_buf, &hints, &addr_list)) != 0) {
        NW_LOG("invalid host or port, ret: %d", debug_ret);
        retval = false;
    } else {
        retval = true;
    }

final:
    freeaddrinfo(addr_list);
    _free_buffer(host_buf);
    _free_buffer(port_buf);
    return retval;
}


static void httpclient_loop_task(void *context)
{
    HTTPCLIENT_RESULT ret;
    httpclient_atcmd_t *httpclient_atcmd = NULL;
    fd_set readfds;
    fd_set errorfds;
    int maxfd;
    struct timeval tv;
    int i = 0;
    tv.tv_sec  = 5;
    tv.tv_usec = 0;
    int result;

    httpclient_mutex_create();
    while (1) {
        maxfd = httpclient_atcmd_get_max_socket_id();
        if (maxfd >= 0) {

            FD_ZERO(&readfds);
            FD_ZERO(&errorfds);
            for(i = 0; i < maxfd + 1; i++) {
                if (httpclient_atcmd_is_valid_socket_id(i)) {
                    FD_SET(i, &readfds);
                    FD_SET(i, &errorfds);
                }
            }
            //if (httpclient_atcmd->is_used && httpclient_atcmd->is_receive_data) {
            result = select(maxfd + 1, &readfds, NULL, &errorfds, &tv);
            NW_LOG("http select fd result: %d", result);
            if (result > 0) {
                httpclient_mutex_take();
                for (i = 0; i < SUPPORT_HTTPCLIENT_ATCMD_MAX_NUM; i++) {
                    httpclient_atcmd = &g_httpclient_atcmd_t[i];
                    if (httpclient_atcmd->is_used && httpclient_atcmd->client
                            && httpclient_atcmd->client->socket >= 0
                            && FD_ISSET(httpclient_atcmd->client->socket, &readfds)) {
                        NW_LOG("go to read: %d, socket: %d", i, httpclient_atcmd->client->socket);
                        do {
                            httpclient_atcmd->httpclient_data->header_buf_len = SUPPORT_HTTPCLIENT_HEADER_BUFFER_SIZE;
                            httpclient_atcmd->httpclient_data->response_buf_len = SUPPORT_HTTPCLIENT_RSP_BUFFER_SIZE;
                            memset(httpclient_atcmd->httpclient_data->header_buf, 0, httpclient_atcmd->httpclient_data->header_buf_len);
                            memset(httpclient_atcmd->httpclient_data->response_buf, 0, httpclient_atcmd->httpclient_data->response_buf_len);
                            ret = httpclient_recv_response(httpclient_atcmd->client, httpclient_atcmd->httpclient_data);

                            if (ret < 0) {
                                NW_LOG("Receive ret < 0 (%d)", ret);
                                /* need to print the last block buffer data. */
                                httpclient_atcmd_receive_indicate(httpclient_atcmd, 0);
                                
                                httpclient_atcmd_error_indicate(httpclient_atcmd->httpclient_id, ret);

                                httpclient_atcmd->is_receive_data = false;
                                if (httpclient_atcmd->is_connected) {
                                    httpclient_atcmd->is_connected = false;
                                    httpclient_close(httpclient_atcmd->client);
                                }
                                break;
                            }

                            if (ret == HTTPCLIENT_RETRIEVE_MORE_DATA)
                                httpclient_atcmd_receive_indicate(httpclient_atcmd, 1);
                            else
                                httpclient_atcmd_receive_indicate(httpclient_atcmd, 0);
                        } while (ret == HTTPCLIENT_RETRIEVE_MORE_DATA);
                        httpclient_atcmd->is_receive_data = 0;
                    }

                    if (httpclient_atcmd->is_used && httpclient_atcmd->client
                            && httpclient_atcmd->client->socket >= 0
                            && FD_ISSET(httpclient_atcmd->client->socket, &errorfds)) {
                        NW_LOG("selected error: %d, socket: %d", i, httpclient_atcmd->client->socket);
                        httpclient_atcmd_error_indicate(httpclient_atcmd->httpclient_id, -2);
                        httpclient_atcmd->is_receive_data = false;
                        if (httpclient_atcmd->is_connected) {
                            httpclient_atcmd->is_connected = false;
                            httpclient_close(httpclient_atcmd->client);
                        }
                        break;
                    }
                }
                httpclient_mutex_give();
            } else {
                vTaskDelay(1000 / portTICK_RATE_MS);  // this else-case is for select return -1.
            }
        } else {
            vTaskDelay(1000 / portTICK_RATE_MS);
        }

    }
    NW_LOG("task stoped for context : %x", httpclient_atcmd);
    vTaskDelete(NULL);
}

#define HTTP_CLIENT_RESULT_STR_MAX_LEN          18
static multi_cmd_package_info_t s_create_multi_pkg = { NULL, 0, 0 };

apb_proxy_status_t apb_proxy_hdlr_httpclient_create_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};

    char* start = NULL;
    char result_str[HTTP_CLIENT_RESULT_STR_MAX_LEN] = {0};

    int processed = 0;
    httpclient_atcmd_t* httpclient_atcmd = NULL;

    int32_t param_cnt = 0;
    char *host = NULL;
    char *user = NULL;
    char *password = NULL;
    uint32_t server_cert_len;
    char *server_cert;
    uint8_t *decoded_server_cert;
    uint32_t client_cert_len;
    char *client_cert;
    uint8_t *decoded_client_cert;
    uint32_t client_pk_len;
    char *client_pk;
    uint8_t *decoded_client_pk;

    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    httpclient_mutex_take();
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;
            NW_LOG("start %s", start);
            if (get_cmd_from_multi_pacakges_cmd(&s_create_multi_pkg, start, &start) != APB_NW_CMD_UTIL_SUCCESS) {
                NW_LOG("process multi package failed");
                goto exit;
            } else if (start == NULL) {
                // need wait for next package
                ret = APB_PROXY_STATUS_OK;
                cmd_result.result_code = APB_PROXY_RESULT_OK;
                goto exit;
            }

            processed = 1; // only when package is received, start process

            httpclient_atcmd = httpclient_atcmd_get_free_httpclient();

            if (NULL == httpclient_atcmd) {
                NW_LOG("threre are no more http client instance");
                goto exit;
            }

            httpclient_atcmd->client = _alloc_buffer(sizeof(httpclient_t));

            if (httpclient_atcmd->client == NULL) {
                NW_LOG("threre are no more http client");
                goto exit;
            } else {
                /* should initialize socket value of http client. */
                httpclient_atcmd->client->socket = -1;
                httpclient_atcmd->client->timeout_in_sec = 60;  // default recv timeout is 60 sec
            }

            // parse
            if (0 != httpclient_create_client_data(httpclient_atcmd)) {
                NW_LOG("data parameter error");
                goto exit;
            }
            //<host>[<auth_user>,<auth_password>,<server_cert_len>,<server_cert>,<client_cert_len>,<client_cert>,<client_pk_len>,<client_pk>]
            param_cnt = apb_nw_util_parse_all_parameters(start, NULL, "s,s,s,l,s,l,s,l,s", &host,
                    &user, &password,
                    &server_cert_len, &server_cert,
                    &client_cert_len, &client_cert,
                    &client_pk_len, &client_pk);
            if (param_cnt == 1 || param_cnt == 9) {
                if (!httpclient_atcmd_is_url_valid(host)) {
                    goto exit;
                }
                
                httpclient_atcmd->host = _alloc_buffer(strlen(host) + 1);
                if (httpclient_atcmd->host) {
                    memcpy(httpclient_atcmd->host, host, strlen(host));
                    NW_LOG("host: %s", httpclient_atcmd->host);
                } else {
                    NW_LOG("Cannot malloc httpclient_atcmd->host");
                    goto exit;
                }

                if (param_cnt == 9) {
                    if (user != NULL && strlen(user) > 0) {
                        httpclient_atcmd->client->auth_user = _alloc_buffer(strlen(user) + 1);
                        if (httpclient_atcmd->client->auth_user) {
                            memcpy(httpclient_atcmd->client->auth_user, user, strlen(user));
                        } else {
                            NW_LOG("cannot malloc memory for client->auth_user");
                            goto exit;
                        }
                    }
                    if (password != NULL && strlen(password) > 0) {
                        httpclient_atcmd->client->auth_password = _alloc_buffer(strlen(password) + 1);
                        if (httpclient_atcmd->client->auth_password) {
                            memcpy(httpclient_atcmd->client->auth_password, password, strlen(password));
                        } else {
                            NW_LOG("cannot malloc memory for client->auth_password");
                            goto exit;
                        }
                    }
#ifdef MTK_HTTPCLIENT_SSL_ENABLE
                    if (server_cert_len > 0 && server_cert) {
                        httpclient_atcmd->client->server_cert_len = server_cert_len / 2 + 1;
                        decoded_server_cert = _alloc_buffer(server_cert_len / 2 + 1);
                        if (decoded_server_cert) {
                            _get_data_from_hex(decoded_server_cert, server_cert, server_cert_len / 2);
                        } else {
                            NW_LOG("cannot malloc memory for client->server_cert");
                            goto exit;
                        }
                        httpclient_atcmd->client->server_cert = (char *)decoded_server_cert;
                    }
                    if (client_cert_len > 0 && client_cert) {
                        httpclient_atcmd->client->client_cert_len = client_cert_len / 2 + 1;
                        decoded_client_cert = _alloc_buffer(client_cert_len / 2 + 1);
                        if (decoded_client_cert) {
                            _get_data_from_hex(decoded_client_cert, client_cert, client_cert_len / 2);
                        } else {
                            NW_LOG("cannot malloc memory for client->server_cert");
                            goto exit;
                        }
                        httpclient_atcmd->client->client_cert = (char *)decoded_client_cert;
                    }
                    if (client_pk_len > 0 && client_pk) {
                        httpclient_atcmd->client->client_pk_len = client_pk_len / 2 + 1;
                        decoded_client_pk = _alloc_buffer(client_pk_len / 2 + 1);
                        if (decoded_client_pk) {
                            _get_data_from_hex(decoded_client_pk, client_pk, client_pk_len / 2);
                        } else {
                            NW_LOG("cannot malloc memory for client->server_cert");
                            goto exit;
                        }
                        httpclient_atcmd->client->client_pk = (char *)decoded_client_pk;
                    }
                    NW_LOG("server cert : %s", httpclient_atcmd->client->server_cert ? httpclient_atcmd->client->server_cert : "NULL");
                    NW_LOG("client cert : %s", httpclient_atcmd->client->client_cert ? httpclient_atcmd->client->client_cert : "NULL");
                    NW_LOG("client pk : %s", httpclient_atcmd->client->client_pk ? httpclient_atcmd->client->client_pk : "NULL");
#endif
                }
            } else {
                NW_LOG("invalid param cnt");
                goto exit;
            }

            snprintf(result_str, HTTP_CLIENT_RESULT_STR_MAX_LEN, "+EHTTPCREATE:%d",
                    httpclient_atcmd->httpclient_id);
            cmd_result.pdata = result_str;
            cmd_result.result_code = APB_PROXY_RESULT_OK;
            ret = APB_PROXY_STATUS_OK;
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }
        default: {
            break;
        }
    }

    exit:
    if (ret != APB_PROXY_STATUS_OK && httpclient_atcmd) {
            httpclient_atcmd_remove_httpclient(httpclient_atcmd->httpclient_id);
    }
    if (ret != APB_PROXY_STATUS_OK || processed == 1) {
        NW_LOG("Free pkg info");
        _free_buffer(s_create_multi_pkg.total_cmd);
        s_create_multi_pkg.total_cmd = NULL;
        s_create_multi_pkg.total_len = 0;
        s_create_multi_pkg.saved_len = 0;
    }
    httpclient_mutex_give();
    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
    return ret;
}

apb_proxy_status_t apb_proxy_hdlr_httpclient_connect_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};

    char* start = NULL;
    int httpclient_id = 0;
    int32_t param_cnt = 0;
    httpclient_atcmd_t* httpclient_atcmd;
    HTTPCLIENT_RESULT result = 0;
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;
            NW_LOG("start %s", start);

            param_cnt = apb_nw_util_parse_all_parameters(start, NULL, "i", &httpclient_id);

            if (param_cnt == 1) {
                NW_LOG("connected httpclient id: %d", httpclient_id);
                httpclient_atcmd = httpclient_atcmd_find_httpclient(httpclient_id);
            } else {
                NW_LOG("param error");
                goto exit;
            }

            if (NULL == httpclient_atcmd) {
                NW_LOG("Cannot find httpclient_atcmd");
                goto exit;
            }

            if (httpclient_atcmd->is_connected) {
                NW_LOG("Already connected");
                goto exit;
            }

            // connect
            result = httpclient_connect(httpclient_atcmd->client, httpclient_atcmd->host);

            if (result != HTTPCLIENT_OK) {
                httpclient_close(httpclient_atcmd->client);  // should close client socket when connect failed.
                NW_LOG("Cannot connect http, err = %d", result);
                goto exit;
            }

            httpclient_atcmd->is_connected = true;
            if (!s_task_running) {
                s_task_running = true;
                xTaskCreate(httpclient_loop_task,
                        "httpclient_atcmd",
                        1024 * 8 / sizeof(portSTACK_TYPE),
                        httpclient_atcmd,
                        TASK_PRIORITY_NORMAL,
                        NULL);
            }
            NW_LOG("create httpclient atcmd task");

            cmd_result.result_code = APB_PROXY_RESULT_OK;
            ret = APB_PROXY_STATUS_OK;
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }
        default: {
            break;
        }
    }

    exit:
    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
    return ret;
}

static multi_cmd_package_info_t s_send_multi_pkg = { NULL, 0, 0 };

apb_proxy_status_t apb_proxy_hdlr_httpclient_send_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    HTTPCLIENT_RESULT result = 0;
    char* start = NULL;
    int32_t param_cnt = 0;
    char* raw_data_buffer = NULL;
    char *cust_header_string = NULL;
    char* url = NULL;

    int32_t httpclient_id, method;
    uint32_t path_len, cust_header_len, content_type_len, content_string_len;
    char *path;
    char* cust_header;
    char *content_type;
    char *content_string;

    httpclient_atcmd_t* httpclient_atcmd = NULL;

    int processed = 0;
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;
            NW_LOG("start %s", start);


            if (get_cmd_from_multi_pacakges_cmd(&s_send_multi_pkg, start, &start) != APB_NW_CMD_UTIL_SUCCESS) {
                NW_LOG("process multi package failed");
                goto exit;
            } else if (start == NULL) {
                // need wait for next package
                ret = APB_PROXY_STATUS_OK;
                cmd_result.result_code = APB_PROXY_RESULT_OK;
                goto exit;
            }

            processed = 1;
            NW_LOG("process start %s", start);
            /*
             * <httpclient_id>,<method>,<path_len>,<path>,<customer_header_len>,<customer_header>,
             * <content_type_len>,<content_type_len>,<content_string_len>,<content_string>
             * */
            param_cnt = apb_nw_util_parse_all_parameters(start, NULL, "i,i,l,s,l,s,l,s,l,s",
                    &httpclient_id, &method,
                    &path_len, &path,
                    &cust_header_len, &cust_header,
                    &content_type_len, &content_type,
                    &content_string_len, &content_string);

            if (param_cnt != 10) {
                NW_LOG("param error, param_cnt = %d", param_cnt);
                goto exit;
            }
            httpclient_atcmd = httpclient_atcmd_find_httpclient(httpclient_id);
            if (httpclient_atcmd == NULL) {
                NW_LOG("httpclient id parameter error");
                goto exit;
            }

            if ((!httpclient_atcmd->is_connected) || httpclient_atcmd->is_receive_data) {
                NW_LOG("Must connect before send or now is receiving");
                goto exit;
            }

            if (path == NULL || *path != '/') {
                NW_LOG("path string parameter error");
                goto exit;
            }

            url = _alloc_buffer(strlen(httpclient_atcmd->host) + strlen(path));
            if (url) {
                strcpy(url, httpclient_atcmd->host);
                strcat(url, path + 1);
                NW_LOG("url = %s", url);
            } else {
                NW_LOG("Cannot malloc url memory");
                goto exit;
            }

            // cust_header length and cust_header string
            if (cust_header_len <= 0) {
                cust_header = NULL;
            } else {
                cust_header_string = _alloc_buffer(cust_header_len / 2 + 1);
                if (cust_header_string == NULL) {
                    NW_LOG("Cannot malloc cust_header_string");
                    goto exit;
                }
                _get_data_from_hex((uint8_t *)cust_header_string, cust_header, cust_header_len / 2);
                NW_LOG("cust header string = %s", cust_header_string);
            }

            // content_type length and content_type string
            if (content_type_len <= 0) {
                content_type = NULL;
            }

            // content string length and content uint8 array data
            if (content_string_len <= 0) {
                content_string = NULL;
            } else {
                raw_data_buffer = _alloc_buffer(content_string_len / 2 + 1);
                if (raw_data_buffer == NULL) {
                    NW_LOG("Cannot malloc raw_data_buffer");
                    goto exit;
                }
                _get_data_from_hex((uint8_t *)raw_data_buffer, content_string, content_string_len / 2);
                NW_LOG("buffer = %d, %s\n", content_string_len / 2, raw_data_buffer + content_string_len / 2 - 10);
            }


            httpclient_atcmd->client->header = cust_header_string;

            httpclient_atcmd->httpclient_data->is_chunked = false;
            httpclient_atcmd->httpclient_data->is_more = false;
            httpclient_atcmd->httpclient_data->retrieve_len = 0;
            httpclient_atcmd->httpclient_data->response_content_len = 0;
            // add post data
            if (method == HTTPCLIENT_POST || method == HTTPCLIENT_PUT) {
                httpclient_atcmd->httpclient_data->post_buf = raw_data_buffer;
                httpclient_atcmd->httpclient_data->post_content_type = content_type;
                httpclient_atcmd->httpclient_data->post_buf_len = content_string_len / 2;
            } else {
                httpclient_atcmd->httpclient_data->post_buf = NULL;
                httpclient_atcmd->httpclient_data->post_content_type = NULL;
                httpclient_atcmd->httpclient_data->post_buf_len = 0;
            }

            // send
            result = httpclient_send_request(httpclient_atcmd->client, url, method, httpclient_atcmd->httpclient_data);

            if (result != HTTPCLIENT_OK) {
                NW_LOG("Cannot send http request, err = %d", result);
                httpclient_atcmd_error_indicate(httpclient_atcmd->httpclient_id, result);
            #if 0  // should not free client data here
                if (httpclient_atcmd->httpclient_data) {
                    _free_buffer(httpclient_atcmd->httpclient_data->response_buf);
                    _free_buffer(httpclient_atcmd->httpclient_data->header_buf);
                    _free_buffer(httpclient_atcmd->httpclient_data);
                    httpclient_atcmd->httpclient_data = NULL;
                }
            #endif
                goto exit;
            } else {
                cmd_result.result_code = APB_PROXY_RESULT_OK;
                ret = APB_PROXY_STATUS_OK;
                httpclient_atcmd->is_receive_data = 1;
            }

            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }
        default: {
            break;
        }
    }

    exit:
    if (ret != APB_PROXY_STATUS_OK || processed == 1) {
        NW_LOG("Free pkg info");
        _free_buffer(s_send_multi_pkg.total_cmd);
        s_send_multi_pkg.total_cmd = NULL;
        s_send_multi_pkg.total_len = 0;
        s_send_multi_pkg.saved_len = 0;
    }
    _free_buffer(cust_header_string);
    _free_buffer(raw_data_buffer);
    _free_buffer(url);
    cmd_result.length = 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
    return ret;
}

apb_proxy_status_t apb_proxy_hdlr_httpclient_disconnect_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    int32_t param_cnt;
    int httpclient_id;
    char* start = NULL;
    httpclient_atcmd_t* httpclient_atcmd = NULL;
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    httpclient_mutex_take();
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;
            NW_LOG("start %s", start);
            param_cnt = apb_nw_util_parse_all_parameters(start, NULL, "i", &httpclient_id);

            if (param_cnt == 1) {
                NW_LOG("connected httpclient id: %d", httpclient_id);
                httpclient_atcmd = httpclient_atcmd_find_httpclient(httpclient_id);
            } else {
                NW_LOG("param error");
                goto exit;
            }

            if (httpclient_atcmd == NULL) {
                NW_LOG("httpclient id parameter error");
                goto exit;
            }

            httpclient_atcmd->is_receive_data = false;
            if (httpclient_atcmd->is_connected) {
                httpclient_atcmd->is_connected = false;
                httpclient_close(httpclient_atcmd->client);
            }

            cmd_result.result_code = APB_PROXY_RESULT_OK;
            ret = APB_PROXY_STATUS_OK;
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }
        default: {
            break;
        }
    }

    exit:
    httpclient_mutex_give();
    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
    return ret;
}

apb_proxy_status_t apb_proxy_hdlr_httpclient_destroy_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    int32_t param_cnt;
    int httpclient_id;
    char* start = NULL;
    httpclient_atcmd_t* httpclient_atcmd = NULL;
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    httpclient_mutex_take();
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;
            NW_LOG("start %s", start);
            param_cnt = apb_nw_util_parse_all_parameters(start, NULL, "i", &httpclient_id);

            if (param_cnt == 1) {
                NW_LOG("connected httpclient id: %d", httpclient_id);
                httpclient_atcmd = httpclient_atcmd_find_httpclient(httpclient_id);
            } else {
                NW_LOG("param error");
                goto exit;
            }

            if (httpclient_atcmd == NULL) {
                NW_LOG("httpclient id parameter error");
                goto exit;
            }

            httpclient_atcmd->is_receive_data = false;
            if (httpclient_atcmd->is_connected) {
                httpclient_atcmd->is_connected = false;
                httpclient_close(httpclient_atcmd->client);
            }

            httpclient_atcmd_remove_httpclient(httpclient_atcmd->httpclient_id);
            cmd_result.result_code = APB_PROXY_RESULT_OK;
            ret = APB_PROXY_STATUS_OK;
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }
        default: {
            break;
        }
    }

    exit:
    httpclient_mutex_give();
    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    apb_proxy_send_at_cmd_result(&cmd_result);
    return ret;
}

