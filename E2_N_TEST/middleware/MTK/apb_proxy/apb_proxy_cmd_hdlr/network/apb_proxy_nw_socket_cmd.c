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

#include "apb_proxy.h"
#include "apb_proxy_nw_cmd.h"
#include "apb_proxy_nw_cmd_gprot.h"
#include "apb_proxy_nw_cmd_util.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sockets.h"
#include "memory_attribute.h"
#include "hal_rtc_internal.h"
#include "hal_rtc_external.h"
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_platform.h"
#include "syslog.h"
#include "netdb.h"
#include "lwip/ip_addr.h"
#include "hal_wdt.h"
#include "netif.h"

log_create_module(apb_soc, PRINT_LEVEL_INFO);

#if defined(__MTK_NBIOT_SLIM__)
#define SOC_LOG(fmt, arg...)
#define SOC_LOGW(fmt, arg...)
#define SOC_LOGE(fmt, arg...)
#else
#ifdef APBSOC_MODULE_PRINTF
#define SOC_LOG(fmt, args...)               printf("[APB SOC] "fmt, ##args)
#else
#define SOC_LOG(fmt, args...)               LOG_I(apb_soc, "[APB SOC] "fmt, ##args)
#define SOC_LOGW(fmt, args...)              LOG_W(apb_soc, "[APB SOC] "fmt, ##args)
#define SOC_LOGE(fmt, args...)              LOG_E(apb_soc, "[APB SOC] "fmt, ##args)
#endif
#endif

#define SUPPORT_SOCKET_ATCMD_MAX_NUM     5
#define SUPPORT_SOCKET_RAW_DATA_LENGTH   512
#define SUPPORT_SOCKET_HEX_DATA_LENGTH   ((SUPPORT_SOCKET_RAW_DATA_LENGTH << 1) + 1)
#define SUPPORT_SOCKET_URC_HEAD_LENGTH   24
#define SUPPORT_UDP_PACKET_MAX_LENGTH    2048
#define IP_ADDRESS_MAX_COUNT             8
#define SOCKET_SEND_TIMEOUT              (2 * 60)/*The unit is seconds.*/

typedef enum {
    SOC_DATA_MODE_ACTIVE,
    SOC_DATA_MODE_STOP_SEND,
    SOC_DATA_MODE_TEMP_DEACTIVATED,
    SOC_DATA_MODE_TEMP_CLOSED
}soc_data_mode_status_t;
uint8_t g_soc_data_channel_count = 0;

typedef struct _socket_atcmd_t
{
    bool is_used;
    int socket_id;
    uint8_t domain;
    uint8_t type;
    uint8_t protocol;
    uint8_t cid;
    uint16_t remote_port;
    struct in_addr remote_address;
    struct in6_addr remote_v6_addr;
    bool is_connected;
    apb_proxy_data_conn_id_t soc_data_mode_conn_id;
    apb_proxy_cmd_id_t soc_data_mode_cmd_id;
    soc_data_mode_status_t soc_data_mode_status;
}socket_atcmd_t;

typedef enum {
    SOCKET_NW_COMMON_ERROR         = -1,
    SOCKET_NW_NO_ERROR             =  0,
    SOCKET_NW_ROUTE_ERROR          =  1,
    SOCKET_NW_CONN_ABORT_ERROR     =  2,
    SOCKET_NW_CONN_RESET_ERROR     =  3,
    SOCKET_NW_NOT_CONNECTED_ERROR  =  4,
    SOCKET_NW_ILLEGAL_VALUE_ERROR  =  5,
    SOCKET_NW_BUFFER_ERROR         =  6,
    SOCKET_NW_WOULD_BLOCK_ERROR    =  7,
    SOCKET_NW_ADDR_IN_USE_ERROR    =  8,
    SOCKET_NW_ALR_CONNECTING_ERROR =  9,
    SOCKET_NW_ALR_CONNECTED_ERROR  =  10,
    SOCKET_NW_NETIF_ERROR          =  11,
    SOCKET_NW_PARAMETER_ERROR      =  12
}soc_nw_result_t;

typedef enum {
    SOC_MSG_DONTWAIT = 1
}soc_flag_t;

TaskHandle_t g_soc_atcmd_task_handle = NULL;
TaskHandle_t g_dns_task_handle = NULL;

static ATTR_ZIDATA_IN_RETSRAM socket_atcmd_t g_socket_atcmd_t[SUPPORT_SOCKET_ATCMD_MAX_NUM];

static int socket_atcmd_retention_socket()
{
    for(int i = 0; i < SUPPORT_SOCKET_ATCMD_MAX_NUM; i++){
        if (g_socket_atcmd_t[i].is_used == 1 && g_socket_atcmd_t[i].type == SOCK_DGRAM) {
            SOC_LOG("data retention %d\n", i);
            socket_atcmd_t* socket_t = &g_socket_atcmd_t[i];
            socket_t->socket_id = socket(socket_t->domain, socket_t->type, socket_t->protocol);
        }
    }
    return 0;
}

static socket_atcmd_t* socket_atcmd_get_free_socket()
{
    for(int i = 0; i < SUPPORT_SOCKET_ATCMD_MAX_NUM; i++){
        if (g_socket_atcmd_t[i].is_used == 0) {
            g_socket_atcmd_t[i].is_used = 1;
            return &g_socket_atcmd_t[i];
        }
    }
    return NULL;
}

static socket_atcmd_t* socket_atcmd_find_socket(int socket_id)
{
    for(int i = 0; i < SUPPORT_SOCKET_ATCMD_MAX_NUM; i++){
        if (g_socket_atcmd_t[i].socket_id == socket_id && g_socket_atcmd_t[i].is_used)
            return &g_socket_atcmd_t[i];
    }

    return NULL;
}

static socket_atcmd_t* socket_atcmd_find_socket_by_conn_id(apb_proxy_data_conn_id_t soc_data_mode_conn_id)
{
    for(int i = 0; i < SUPPORT_SOCKET_ATCMD_MAX_NUM; i++){
        if (g_socket_atcmd_t[i].soc_data_mode_conn_id == soc_data_mode_conn_id && g_socket_atcmd_t[i].is_used)
            return &g_socket_atcmd_t[i];
    }

    return NULL;
}

static void socket_atcmd_remove_socket(int socket_id)
{
    for(int i = 0; i < SUPPORT_SOCKET_ATCMD_MAX_NUM; i++){
        if (g_socket_atcmd_t[i].socket_id == socket_id && g_socket_atcmd_t[i].is_used) {
            g_socket_atcmd_t[i].is_used = 0;
            return;
        }
    }
}

static int socket_atcmd_max_socket_id()
{
    int max_fd = -1;
    for(int i = 0; i < SUPPORT_SOCKET_ATCMD_MAX_NUM; i++){
        if (g_socket_atcmd_t[i].is_used == 1) {
            socket_atcmd_t* socket_t = &g_socket_atcmd_t[i];
            if(max_fd < socket_t->socket_id) {
                max_fd = socket_t->socket_id;
            }
        }
    }
    return max_fd;
}

static soc_nw_result_t socket_get_mapped_error_code(int error_code)
{
    soc_nw_result_t result_code = SOCKET_NW_COMMON_ERROR;
    if (error_code == 0){
        result_code = SOCKET_NW_NO_ERROR;
        return result_code;
    }
    switch(error_code){
        case ENOMEM:
        case ENOBUFS:{
            result_code = SOCKET_NW_BUFFER_ERROR;
            break;
        }
        case EHOSTUNREACH: {
            result_code = SOCKET_NW_ROUTE_ERROR;
            break;
        }
        case EWOULDBLOCK: {
            result_code = SOCKET_NW_WOULD_BLOCK_ERROR;
            break;
        }
        case EIO:
        case EINVAL:{
            result_code = SOCKET_NW_ILLEGAL_VALUE_ERROR;
            break;
        }
        case EADDRINUSE: {
            result_code = SOCKET_NW_ADDR_IN_USE_ERROR;
            break;
        }
        case EALREADY: {
            result_code = SOCKET_NW_ALR_CONNECTING_ERROR;
            break;
        }
        case EISCONN: {
            result_code = SOCKET_NW_ALR_CONNECTED_ERROR;
            break;
        }
        case ECONNABORTED: {
            result_code = SOCKET_NW_CONN_ABORT_ERROR;
            break;
        }
        case ECONNRESET: {
            result_code = SOCKET_NW_CONN_RESET_ERROR;
            break;
        }
        case ENOTCONN: {
            result_code = SOCKET_NW_NOT_CONNECTED_ERROR;
            break;
        }
        case -1: {
            result_code = SOCKET_NW_NETIF_ERROR;
            break;
        }
        default: {
            result_code = SOCKET_NW_COMMON_ERROR;
            break;
        }
    }

    return result_code;
}

static void socket_dispatch_network_error_indication(int socket_id)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    int error_code = 0;
    char result_buf[32] = {0};
    char* urc_head = "+ESOERR=";
    uint32_t len = sizeof(error_code);
    soc_nw_result_t soc_error = SOCKET_NW_COMMON_ERROR;
    socket_atcmd_t* socket_t = NULL;

    SOC_LOG("socketID:%d error!\r\n", socket_id);
    if (getsockopt(socket_id, SOL_SOCKET, SO_ERROR, &error_code, &len) == 0){
        SOC_LOG("socket error code: %d\r\n", error_code);
        soc_error = socket_get_mapped_error_code(error_code);
    }else{
        SOC_LOG("get error code failed\r\n");
    }
    if (soc_error != SOCKET_NW_NO_ERROR){
        snprintf(result_buf, sizeof(result_buf), "%s%d,%d",urc_head, socket_id, soc_error);
        socket_t = socket_atcmd_find_socket(socket_id);
        if (socket_t != NULL) {
            if (socket_t->soc_data_mode_conn_id != 0 && socket_t->soc_data_mode_status != SOC_DATA_MODE_TEMP_DEACTIVATED) {
                SOC_LOG("socket_dispatch_network_error_indication, %s", result_buf);
                return;
            }
        }
        cmd_result.pdata = result_buf;
        cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
        cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
        cmd_result.length = strlen(cmd_result.pdata);
        apb_proxy_send_at_cmd_result(&cmd_result);
    }
}

static void socket_close_data_mode(int socket_id)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    socket_atcmd_t*  socket_t = NULL;

    socket_t = socket_atcmd_find_socket(socket_id);
    if (socket_t == NULL) {
        SOC_LOG("close data mode: socket_id parameter error !!");
        return;
    }

    cmd_result.result_code = APB_PROXY_RESULT_OK;
    cmd_result.pdata = NULL;
    cmd_result.length = 0;
    cmd_result.cmd_id = socket_t->soc_data_mode_cmd_id;
    cmd_result.result_code = APB_PROXY_RESULT_OK;
    apb_proxy_send_at_cmd_result(&cmd_result);

    if (apb_proxy_close_data_mode(socket_t->soc_data_mode_conn_id) == APB_PROXY_STATUS_OK) {
        socket_t->soc_data_mode_conn_id = 0;
        socket_t->soc_data_mode_status = SOC_DATA_MODE_TEMP_CLOSED;
        cmd_result.result_code = APB_PROXY_RESULT_NO_CARRIER;
        cmd_result.pdata = NULL;
        cmd_result.length = 0;
        cmd_result.cmd_id = socket_t->soc_data_mode_cmd_id;
        apb_proxy_send_at_cmd_result(&cmd_result);
        g_soc_data_channel_count--;
        SOC_LOG("close data mode: current socket data channel count = %d", g_soc_data_channel_count);
    }
}

static void socket_loop_task(void *parameter)
{
    int result;
    fd_set readfds;
    fd_set errorfds;
    int maxfd;
    bool power_from_deep = false;
    SOC_LOG("socket atcmd task");

    if ((rtc_power_on_result_external() == DEEP_SLEEP) ||
        (rtc_power_on_result_external() == DEEPER_SLEEP)){
        power_from_deep = true;
    }

    if ((true == power_from_deep) && (parameter == 1)) {
        socket_atcmd_retention_socket();
    }

    maxfd = socket_atcmd_max_socket_id();

    while(maxfd >= 0) {
        //wait forever
        struct timeval tv;
        int i = 0;
        FD_ZERO(&readfds);
        FD_ZERO(&errorfds);

        maxfd = -1;
        for(i = 0; i < SUPPORT_SOCKET_ATCMD_MAX_NUM; i++){
            if (g_socket_atcmd_t[i].is_used == 1) {
                socket_atcmd_t* socket_t = &g_socket_atcmd_t[i];
                if(maxfd < socket_t->socket_id) {
                    maxfd = socket_t->socket_id;
                }
                FD_SET(socket_t->socket_id, &readfds);
                FD_SET(socket_t->socket_id, &errorfds);
            }
        }
        if (maxfd < 0){
            break;
        }
        tv.tv_sec  = 5;
        tv.tv_usec = 0;
        result = select(maxfd + 1, &readfds, NULL, &errorfds, &tv);

        if (result > 0) {
            for(i = 0; i < SUPPORT_SOCKET_ATCMD_MAX_NUM; i++){
                if (g_socket_atcmd_t[i].is_used && FD_ISSET(g_socket_atcmd_t[i].socket_id, &errorfds)){
                    socket_dispatch_network_error_indication(g_socket_atcmd_t[i].socket_id);
                    if (g_socket_atcmd_t[i].soc_data_mode_conn_id != 0) {
                        socket_close_data_mode(g_socket_atcmd_t[i].socket_id);
                    }
                    close(g_socket_atcmd_t[i].socket_id);
                    g_socket_atcmd_t[i].is_connected = 0;
                    socket_atcmd_remove_socket(g_socket_atcmd_t[i].socket_id);
                }

                if (g_socket_atcmd_t[i].is_used && FD_ISSET(g_socket_atcmd_t[i].socket_id, &readfds)) {
                    apb_proxy_at_cmd_result_t cmd_result = {0};
                    uint8_t* result_buf = NULL;
                    uint8_t* data_buf = NULL;
                    uint8_t* hex_data_buf = NULL;
                    uint8_t socket_str[5] = {0};
                    uint8_t *res_str = "+ESONMI=";
                    uint32_t data_buf_len = 0;
                    uint32_t hex_data_buf_len = 0;
                    SOC_LOG("this is some data from socket_id: %d", g_socket_atcmd_t[i].socket_id);
                    if (g_socket_atcmd_t[i].type != SOCK_STREAM){
                        SOC_LOG("Socket type: %d", g_socket_atcmd_t[i].type);
                        result = recv(g_socket_atcmd_t[i].socket_id, socket_str, sizeof(socket_str), MSG_TRUNC | MSG_PEEK | MSG_DONTWAIT);
                        if (result <= 0){
                            socket_dispatch_network_error_indication(g_socket_atcmd_t[i].socket_id);
                            if (g_socket_atcmd_t[i].soc_data_mode_conn_id != 0) {
                                socket_close_data_mode(g_socket_atcmd_t[i].socket_id);
                            }
                            close(g_socket_atcmd_t[i].socket_id);
                            g_socket_atcmd_t[i].is_connected = 0;
                            socket_atcmd_remove_socket(g_socket_atcmd_t[i].socket_id);
                            continue;
                        }else if (result > SUPPORT_UDP_PACKET_MAX_LENGTH){
                            if (g_socket_atcmd_t[i].soc_data_mode_conn_id != 0 && g_socket_atcmd_t[i].soc_data_mode_status != SOC_DATA_MODE_TEMP_DEACTIVATED) {
                                SOC_LOG("socket data mode, UDP package is too large.");
                            } else {
                                cmd_result.pdata = "+ESONMI: UDP package is too large.";
                                cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
                                cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
                                cmd_result.length = strlen(cmd_result.pdata) + 1;
                                apb_proxy_send_at_cmd_result(&cmd_result);
                            }
                            result = recv(g_socket_atcmd_t[i].socket_id, socket_str, sizeof(socket_str), MSG_TRUNC | MSG_DONTWAIT);
                            SOC_LOG("Dropped data: %d\r\n", result);
                            if (result <= 0){
                                socket_dispatch_network_error_indication(g_socket_atcmd_t[i].socket_id);
                                if (g_socket_atcmd_t[i].soc_data_mode_conn_id != 0) {
                                    socket_close_data_mode(g_socket_atcmd_t[i].socket_id);
                                }
                                close(g_socket_atcmd_t[i].socket_id);
                                g_socket_atcmd_t[i].is_connected = 0;
                                socket_atcmd_remove_socket(g_socket_atcmd_t[i].socket_id);
                            }
                            continue;
                        }
                        data_buf_len = result;
                    }else{
                        data_buf_len = SUPPORT_SOCKET_RAW_DATA_LENGTH;
                    }

                    data_buf = _alloc_buffer(data_buf_len);
                    if (g_socket_atcmd_t[i].soc_data_mode_conn_id != 0 && g_socket_atcmd_t[i].soc_data_mode_status != SOC_DATA_MODE_TEMP_DEACTIVATED) {
                        if (g_socket_atcmd_t[i].soc_data_mode_status == SOC_DATA_MODE_STOP_SEND) {
                            vTaskDelay(50 / portTICK_PERIOD_MS);
                            continue;
                        }
                        if(data_buf != NULL){
                            apb_proxy_user_data_t new_user_data;
                            memset(data_buf, 0, data_buf_len);
                            result = recv(g_socket_atcmd_t[i].socket_id, data_buf, data_buf_len, MSG_DONTWAIT);
                            SOC_LOG("recv %s", data_buf);

                            SOC_LOG("Send user data: [%s], len: %d", data_buf, data_buf_len);
                            new_user_data.pbuffer = data_buf;
                            new_user_data.length = data_buf_len;
                            if(apb_proxy_send_user_data(g_socket_atcmd_t[i].soc_data_mode_conn_id, &new_user_data) != APB_PROXY_STATUS_OK){
                                SOC_LOG("Send user data failed.");
                            }
                            _free_buffer(data_buf);
                            data_buf = NULL;
                        }
                    } else {
                        if ((data_buf_len << 1) < SUPPORT_SOCKET_HEX_DATA_LENGTH){
                            hex_data_buf_len = (data_buf_len << 1) + 1;
                        }else{
                            hex_data_buf_len = SUPPORT_SOCKET_HEX_DATA_LENGTH;
                        }
                        SOC_LOG("allocated data_buf_len = %d, hex len=%d\r\n", data_buf_len, hex_data_buf_len);
                        hex_data_buf = _alloc_buffer(hex_data_buf_len);
                        result_buf = _alloc_buffer(hex_data_buf_len + SUPPORT_SOCKET_URC_HEAD_LENGTH);

                        if (result_buf == NULL || data_buf == NULL || hex_data_buf == NULL) {
                            if(result_buf != NULL){
                                _free_buffer(result_buf);
                            }
                            if(data_buf != NULL){
                                _free_buffer(data_buf);
                            }
                            if(hex_data_buf != NULL){
                                _free_buffer(hex_data_buf);
                            }
                            cmd_result.pdata = "read data in select, can't allocate memory!!";
                            cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
                            cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
                            cmd_result.length = strlen(cmd_result.pdata) + 1;
                            apb_proxy_send_at_cmd_result(&cmd_result);
                        } else {
                            uint32_t sent_len = 0;
                            memset(data_buf, 0, data_buf_len);
                            result = recv(g_socket_atcmd_t[i].socket_id, data_buf, data_buf_len, MSG_DONTWAIT);
                            SOC_LOG("recv %s", data_buf);

                            while((result > 0) && (sent_len < result)){
                                memset(hex_data_buf, 0, hex_data_buf_len);
                                memset(result_buf, 0, hex_data_buf_len + SUPPORT_SOCKET_URC_HEAD_LENGTH);
                                strncpy(result_buf, res_str, strlen(res_str));
                                memset(socket_str, 0, sizeof(socket_str));
                                _itoa(g_socket_atcmd_t[i].socket_id, socket_str, 10);
                                strcat(result_buf, socket_str);
                                strcat(result_buf, ",");
                                if ((result - sent_len) > SUPPORT_SOCKET_RAW_DATA_LENGTH){
                                    _get_data_to_hex(hex_data_buf, data_buf + sent_len, SUPPORT_SOCKET_RAW_DATA_LENGTH);
                                    memset(socket_str, 0, sizeof(socket_str));
                                    _itoa(SUPPORT_SOCKET_RAW_DATA_LENGTH, socket_str, 10);
                                    strcat(result_buf, socket_str);
                                    strcat(result_buf, ",");
                                    strcat(result_buf, hex_data_buf);
                                    sent_len += SUPPORT_SOCKET_RAW_DATA_LENGTH;
                                }else{
                                    _get_data_to_hex(hex_data_buf, data_buf + sent_len, result - sent_len);
                                    memset(socket_str, 0, sizeof(socket_str));
                                    _itoa(result - sent_len, socket_str, 10);
                                    strcat(result_buf, socket_str);
                                    strcat(result_buf, ",");
                                    strcat(result_buf, hex_data_buf);
                                    sent_len = result;
                                }
                                cmd_result.pdata = result_buf;
                                cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
                                cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
                                cmd_result.length = strlen(cmd_result.pdata);
                                apb_proxy_send_at_cmd_result(&cmd_result);
                            }
                            SOC_LOG("Data outputted all\r\n");
                            _free_buffer(data_buf);
                            _free_buffer(hex_data_buf);
                            _free_buffer(result_buf);
                            data_buf = NULL;
                            hex_data_buf = NULL;
                            result_buf = NULL;
                            if (result <= 0){
                                SOC_LOG("result = %d\r\n", result);
                                socket_dispatch_network_error_indication(g_socket_atcmd_t[i].socket_id);
                                if (g_socket_atcmd_t[i].soc_data_mode_conn_id != 0) {
                                    socket_close_data_mode(g_socket_atcmd_t[i].socket_id);
                                }
                                close(g_socket_atcmd_t[i].socket_id);
                                g_socket_atcmd_t[i].is_connected = 0;
                                socket_atcmd_remove_socket(g_socket_atcmd_t[i].socket_id);
                                continue;
                            }
                        }
                    }
                }
            }
        }
    }
    SOC_LOG("exit socket task");
    g_soc_atcmd_task_handle = NULL;
    vTaskDelete(NULL);
}

apb_proxy_status_t apb_proxy_hdlr_socket_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    int value, cid = -1, has_cid = 0;
    char test_case_num_str[5] = {0};
    char socket_str[5] = {0};
    char result_str[10] = {0};
    char* start = NULL;
    char* temp = NULL;
    socket_atcmd_t* socket_t = NULL;
    uint8_t domain = AF_INET;
    uint8_t type = SOCK_STREAM;
    uint8_t protocol = IPPROTO_IP;

    SOC_LOG("create socket: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_OK;
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_TESTING:  // rec: AT+ESOC=?
            break;

        case APB_PROXY_CMD_MODE_READ: {   // rec: AT+ESOC?
            cmd_result.pdata = "+ESOC:<domain>,<type>,<protocol>";
            cmd_result.length = strlen((char *)cmd_result.pdata);
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {  // rec: AT+ESOC
            /*do something*/
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            char *res_str = "+ESOC=";
            struct timeval send_timeout = {0};
            const uint32_t SOCKET_CREATE_PARAMETER_SIZE = 8;

            if ((p_parse_cmd->string_len - p_parse_cmd->parse_pos -1) > SOCKET_CREATE_PARAMETER_SIZE){
                SOC_LOG("paramer invalid\r\n");
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                break;
            }

            start = strchr(p_parse_cmd->string_ptr, '=');
            start++;
            SOC_LOG("start %s", start);

            temp = strchr(start, ',');
            SOC_LOG("temp %s", temp);
            if (temp == NULL) {
                cmd_result.pdata = "domain parameter error";
                cmd_result.length = strlen((char *)cmd_result.pdata);
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                break;
            }
            strncpy(test_case_num_str, start, temp - start);
            SOC_LOG("test_case_num_str %s", test_case_num_str);
            value = _atoi(test_case_num_str);
            SOC_LOG("domain", value);
            if(value == 1) {
                domain = AF_INET;
            } else if (value == 2) {
                domain = AF_INET6;
            } else {
                cmd_result.pdata = "domain parameter error";
                cmd_result.length = strlen((char *)cmd_result.pdata);
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                break;
            }

            start = temp + 1;
            temp = strchr(start, ',');
            if (temp == NULL){
                cmd_result.pdata = "type parameter NULL";
                cmd_result.length = strlen((char *)cmd_result.pdata);
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                break;
            }
            strncpy(test_case_num_str, start, temp - start);
            value = _atoi(test_case_num_str);
            SOC_LOG("type %d", value);
            if(value == 1) {
                type = SOCK_STREAM;
            } else if(value == 2) {
                type = SOCK_DGRAM;
            } else if(value == 3) {
                type = SOCK_RAW;
            } else {
                cmd_result.pdata = "type parameter error";
                cmd_result.length = strlen((char *)cmd_result.pdata);
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                break;
            }

            start = temp + 1;
            temp = strchr(start, ',');
            if (temp) {
                has_cid = 1;
                strncpy(test_case_num_str, start, temp - start);

            } else {
                strncpy(test_case_num_str, start, strlen(start));

            }
            value = _atoi(test_case_num_str);
            SOC_LOG("protocol %d", value);
            if (value == 1) {
                protocol = IPPROTO_IP;
            } else if (value == 2) {
                protocol = IPPROTO_ICMP;
            } else {
                cmd_result.pdata = "protocol parameter error";
                cmd_result.length = strlen((char *)cmd_result.pdata);
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                break;
            }

            socket_t = socket_atcmd_get_free_socket();
            if (socket_t == NULL) {
                cmd_result.pdata = "too much socket instance";
                cmd_result.length = strlen((char *)cmd_result.pdata);
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                break;
            }

            socket_t->domain = domain;
            socket_t->type = type;
            socket_t->protocol = protocol;
            socket_t->socket_id = socket(socket_t->domain, socket_t->type, socket_t->protocol);
            SOC_LOG("socket_id %d", socket_t->socket_id);
            if (socket_t->socket_id < 0) {
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                socket_t->is_used = 0;
                break;
            }
            send_timeout.tv_sec = SOCKET_SEND_TIMEOUT;
            send_timeout.tv_usec = 0;
            lwip_setsockopt(socket_t->socket_id, SOL_SOCKET, SO_SNDTIMEO, &send_timeout, sizeof(send_timeout));
            if (has_cid) {
                start = temp + 1;
                strncpy(test_case_num_str, start, strlen(start));
                cid = _atoi(test_case_num_str);
                SOC_LOG("cid %d", cid);
            }
            if (cid != -1 && socket_t->socket_id >= 0) {
                socket_t->cid= cid;
                // please check the parameter for cid and sizeof (cid)
            #ifdef LWIP_SOCKET_OPTION_BINDTODEVICE
                if (setsockopt(socket_t->socket_id, SOL_SOCKET, SO_BINDTODEVICE, &cid, sizeof(cid)) < 0 ) {
                    SOC_LOG("set cid error");
                    socket_atcmd_remove_socket(socket_t->socket_id);
                    cmd_result.pdata = "set APN fail!";
                    cmd_result.length = strlen((char *)cmd_result.pdata);
                    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                    break;
                }
            #endif
            }

            if (g_soc_atcmd_task_handle == NULL) {
                xTaskCreate(socket_loop_task,
                        "socket_atcmd",
                        1024 * 4 / sizeof(portSTACK_TYPE),
                        NULL,
                        TASK_PRIORITY_NORMAL,
                        &g_soc_atcmd_task_handle);
                SOC_LOG("create socket task");
            }
            _itoa(socket_t->socket_id, socket_str, 10);
            SOC_LOG("socket_str [%d] %s ", strlen(socket_str), socket_str);
            strncpy(result_str, res_str, strlen(res_str));
            strcat(result_str, socket_str);
            cmd_result.pdata = result_str;

            cmd_result.length = strlen((char *)cmd_result.pdata);
            break;

        }

        default: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
    }

    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_bind_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    int port_number, socket_id, result, is_have_address = 0;
    char temp_buffer[64] = {0};
    char* start = NULL;
    char* temp = NULL;
    struct sockaddr_in addr;
    socket_atcmd_t* socket_t = NULL;
    soc_nw_result_t soc_result = SOCKET_NW_NO_ERROR;

    cmd_result.result_code = APB_PROXY_RESULT_OK;
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_TESTING: {
            cmd_result.pdata = "+ESOB=<socket_id>,<local_port>,[IP Address]";
            cmd_result.length = strlen((char *)cmd_result.pdata);
            break;
        }
        case APB_PROXY_CMD_MODE_READ:
        case APB_PROXY_CMD_MODE_ACTIVE: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;

            /*Parsing socket ID.*/
            temp = strchr(start, ',');
            temp = strchr(start, ',');
            if ((temp == NULL) || ((temp - start) >= sizeof(temp_buffer))) {
                soc_result = SOCKET_NW_PARAMETER_ERROR;
                break;
            }
            memset(temp_buffer, 0, sizeof(temp_buffer));
            memcpy(temp_buffer, start, temp - start);
            socket_id = _atoi(temp_buffer);
            SOC_LOG("socket id:%d", socket_id);
            socket_t = socket_atcmd_find_socket(socket_id);
            if (socket_t == NULL) {
                soc_result = SOCKET_NW_PARAMETER_ERROR;
                break;
            }

            /*Parsing port number.*/
            start = temp + 1;
            temp = strchr(start, ',');
            memset(temp_buffer, 0, sizeof(temp_buffer));
            if (temp) {
                is_have_address = 1;
                memcpy(temp_buffer, start, temp - start);
            } else {
                memcpy(temp_buffer, start, strlen(start));
            }
            port_number = _atoi(temp_buffer);
            SOC_LOG("port_number:%d", port_number);

            /*Parsing IP address.*/
            if (is_have_address) {
                start = temp + 1;
                SOC_LOG("start %s", start);
                if (start[0] != '\"') {
                    soc_result = SOCKET_NW_PARAMETER_ERROR;
                    break;
                }
                start = start + 1;
                temp = strchr(start, '\"');
                if (temp == NULL) {
                    soc_result = SOCKET_NW_PARAMETER_ERROR;
                    break;
                }
                if ((temp - start) >= sizeof(temp_buffer)){
                    soc_result = SOCKET_NW_PARAMETER_ERROR;
                    break;
                }
                memset(temp_buffer, 0, sizeof(temp_buffer));
                memcpy(temp_buffer, start, temp - start);
            }

            switch(socket_t->domain){
                case AF_INET: {
                    struct sockaddr_in addr;
                    memset(&addr, 0, sizeof(addr));
                    addr.sin_family = socket_t->domain;
                    addr.sin_port = lwip_htons(port_number);
                    if (is_have_address){
                        addr.sin_addr.s_addr = inet_addr(temp_buffer);
                    }
                    result = bind(socket_id, (struct sockaddr *)&addr, sizeof(addr));
                    SOC_LOG("bind result: %d", result);
                    if (result != 0){
                        soc_result = SOCKET_NW_COMMON_ERROR;
                    }
                    break;
                }
#if LWIP_IPV6
                case AF_INET6: {
                    struct sockaddr_in6 servaddr6;
                    ip6_addr_t ip6addr;
                    memset(&servaddr6, 0, sizeof(servaddr6));
                    memset(&ip6addr, 0, sizeof(ip6addr));
                    servaddr6.sin6_family = AF_INET6;
                    servaddr6.sin6_port = htons(port_number);
                    if (is_have_address){
                        if (ip6addr_aton(temp_buffer, &ip6addr) == 1){
                            inet6_addr_from_ip6addr(&servaddr6.sin6_addr, &ip6addr);
                        }else{
                             SOC_LOG("invalid IPV6 address");
                             soc_result = SOCKET_NW_PARAMETER_ERROR;
                             break;
                        }
                    }
                    result = bind(socket_id, (struct sockaddr *)&servaddr6, sizeof(servaddr6));
                    SOC_LOG("bind result: %d", result);
                    if (result != 0){
                        soc_result = SOCKET_NW_COMMON_ERROR;
                    }
                    break;
                }
#endif
                default: {
                    soc_result = SOCKET_NW_ILLEGAL_VALUE_ERROR;
                    break;
                }
            }
            break;
        }
        default: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
    }
    if (soc_result != SOCKET_NW_NO_ERROR){
        cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    }
    SOC_LOG("soc result=%d", soc_result);
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_connect_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    int port_num, socket_id, result;
    /*The buffer should be big enougth for IPV6 address.*/
    char temp_buffer[64] = {0};
    char* start = NULL;
    char* temp = NULL;
    socket_atcmd_t* socket_t = NULL;
    soc_nw_result_t soc_result = SOCKET_NW_NO_ERROR;

    SOC_LOG("connect to: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_OK;
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_TESTING: {
            cmd_result.pdata = "AT+ESOCON=<socket_id>,<remote_port>,<remote_address>";
            cmd_result.length = strlen((char *)cmd_result.pdata);
            break;
        }
        case APB_PROXY_CMD_MODE_READ:
        case APB_PROXY_CMD_MODE_ACTIVE: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;

            /*1. Parsing the socket ID.*/
            temp = strchr(start, ',');
            if ((temp == NULL) || ((temp - start) >= sizeof(temp_buffer))) {
                soc_result = SOCKET_NW_PARAMETER_ERROR;
                break;
            }
            memset(temp_buffer, 0, sizeof(temp_buffer));
            memcpy(temp_buffer, start, temp - start);
            socket_id = _atoi(temp_buffer);
            SOC_LOG("connect to socket: %d", socket_id);

            /*2. Parsing the port number.*/
            start = temp + 1;
            temp = strchr(start, ',');
            if ((temp == NULL) || ((temp - start) >= sizeof(temp_buffer))) {
                soc_result = SOCKET_NW_PARAMETER_ERROR;
                break;
            }
            memset(temp_buffer, 0, sizeof(temp_buffer));
            memcpy(temp_buffer, start, temp - start);
            port_num = _atoi(temp_buffer);
            SOC_LOG("connect to port: %d", port_num);

            /*3. Parsing the address.*/
            start = temp + 1;
            SOC_LOG("start %s", start);
            if (start[0] != '\"') {
                soc_result = SOCKET_NW_PARAMETER_ERROR;
                break;
            }
            start = start + 1;
            temp = strchr(start, '\"');
            if ((temp == NULL) || (temp - start >= sizeof(temp_buffer))) {
                soc_result = SOCKET_NW_PARAMETER_ERROR;
                break;
            }

            memcpy(temp_buffer, start, temp - start);
            SOC_LOG("connect to address: %s", temp_buffer);
            /*Parsing parameter end.*/

            socket_t = socket_atcmd_find_socket(socket_id);
            if (socket_t == NULL) {
                soc_result = SOCKET_NW_ILLEGAL_VALUE_ERROR;
                break;
            }

            switch(socket_t->domain){
                case AF_INET: {
                    struct sockaddr_in addr;
                    memset(&addr, 0, sizeof(addr));
                    addr.sin_family = socket_t->domain;
                    addr.sin_port = lwip_htons(port_num);
                    socket_t->remote_port = addr.sin_port;
                    addr.sin_addr.s_addr = inet_addr(temp_buffer);
                    memcpy(&(socket_t->remote_address), &(addr.sin_addr), sizeof(socket_t->remote_address));
                    result = connect(socket_id, (struct sockaddr *)&addr, sizeof(addr));
                    SOC_LOG("connect result: %d", result);
                    if (result != 0){
                        soc_result = SOCKET_NW_COMMON_ERROR;
                    } else {
                        socket_t->is_connected = 1;
                    }
                    break;
                }
#if LWIP_IPV6
                case AF_INET6: {
                    struct sockaddr_in6 servaddr6;
                    ip6_addr_t ip6addr;
                    memset(&servaddr6, 0, sizeof(servaddr6));
                    memset(&ip6addr, 0, sizeof(ip6addr));
                    servaddr6.sin6_family = AF_INET6;
                    if (ip6addr_aton(temp_buffer, &ip6addr) == 1){
                        inet6_addr_from_ip6addr(&servaddr6.sin6_addr, &ip6addr);
                        servaddr6.sin6_port = htons(port_num);
                        socket_t->remote_port = servaddr6.sin6_port;
                        memcpy(&(socket_t->remote_v6_addr), &(servaddr6.sin6_addr), sizeof(socket_t->remote_v6_addr));
                        result = connect(socket_id, (struct sockaddr *)&servaddr6, sizeof(servaddr6));
                        SOC_LOG("connect result: %d", result);
                        if (result != 0){
                            soc_result = SOCKET_NW_COMMON_ERROR;
                        } else {
                            socket_t->is_connected = 1;
                        }
                    }else{
                        SOC_LOG("invalid IPV6 address");
                        soc_result = SOCKET_NW_PARAMETER_ERROR;
                    }
                    break;
                }
#endif
                default: {
                    soc_result = SOCKET_NW_ILLEGAL_VALUE_ERROR;
                    break;
                }
            }
            break;
        }
        default: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
    }

    if(soc_result != SOCKET_NW_NO_ERROR){
        cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    }
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_send_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    int socket_id, data_len, copy_len, result = 0, has_flag = 0, flag = 0;
    char temp_buffer[64] = {0};
    char* start = NULL;
    char* temp = NULL;
    char* data_buf = NULL;
    char* raw_data_buf = NULL;
    socket_atcmd_t* socket_t = NULL;
    char* p_error = "parameter error !!";
    soc_nw_result_t soc_result = SOCKET_NW_NO_ERROR;

    cmd_result.result_code = APB_PROXY_RESULT_OK;
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_TESTING: {
            cmd_result.pdata = "+ESOSEND=<socket_id>,<data_len>,<data>";
            cmd_result.length = strlen((char *)cmd_result.pdata);
            break;
        }
        case APB_PROXY_CMD_MODE_READ:
        case APB_PROXY_CMD_MODE_ACTIVE: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            uint32_t left_len = 0;
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;
            data_len = p_parse_cmd->string_len - p_parse_cmd->parse_pos - 1;
            start[data_len] = '\0';

            temp = strchr(start, ',');
            if (temp == NULL){
                soc_result = SOCKET_NW_PARAMETER_ERROR;
                break;
            }
            memset(temp_buffer, 0, sizeof(temp_buffer));
            if ((temp - start) >= sizeof(temp_buffer)){
                soc_result = SOCKET_NW_PARAMETER_ERROR;
                break;
            }
            memcpy(temp_buffer, start, temp - start);
            socket_id = _atoi(temp_buffer);

            socket_t = socket_atcmd_find_socket(socket_id);
            if (socket_t == NULL) {
                soc_result = SOCKET_NW_PARAMETER_ERROR;
                break;
            }

            start = temp + 1;
            temp = strchr(start, ',');
            if (temp == NULL) {
                soc_result = SOCKET_NW_PARAMETER_ERROR;
                break;
            }
            if ((temp - start) >= sizeof(temp_buffer)){
                soc_result = SOCKET_NW_PARAMETER_ERROR;
                break;
            }
            memset(temp_buffer, 0, sizeof(temp_buffer));
            memcpy(temp_buffer, start, temp - start);
            data_len = _atoi(temp_buffer);

            start = temp + 1;
            temp = strchr(start, ',');
            left_len = strlen(start);
            if (left_len > (data_len * 2)) {
                temp = start + data_len * 2;
                if  (temp[0] == ',') {
                    has_flag = 1;
                    copy_len = temp - start;
                } else {
                    cmd_result.pdata = p_error;
                    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                    cmd_result.length = strlen((char *)cmd_result.pdata);
                    break;
                }
            } else {
                copy_len = strlen(start);
            }

            if (copy_len != (data_len << 1)){
                SOC_LOG("socket data length does not matched\r\n");
                soc_result = SOCKET_NW_PARAMETER_ERROR;
                break;
            }

            data_buf = _alloc_buffer(copy_len + 1);
            raw_data_buf = _alloc_buffer(data_len + 1);
            if (data_buf == NULL || raw_data_buf == NULL) {
                if(data_buf != NULL) {
                    _free_buffer(data_buf);
                }
                if(raw_data_buf != NULL) {
                    _free_buffer(raw_data_buf);
                }
                soc_result = SOCKET_NW_BUFFER_ERROR;
                break;
            }
            memcpy(data_buf, start, copy_len);
            data_buf[copy_len] = '\0';
            // TODO: hex data to raw data
            _get_data_from_hex((uint8_t *)raw_data_buf, data_buf, data_len);

            if (has_flag) {
                soc_flag_t soc_flag;
                start = temp + 1;
                if (strlen(start) > 5) {
                    soc_result = SOCKET_NW_PARAMETER_ERROR;
                    _free_buffer(data_buf);
                    _free_buffer(raw_data_buf);
                    break;
                }
                memset(temp_buffer, 0, sizeof(temp_buffer));
                memcpy(temp_buffer, start, strlen(start));
                soc_flag = (soc_flag_t)_atoi(temp_buffer);
                SOC_LOG("send flag %d", soc_flag);
                flag = 0;
                switch(soc_flag){
                    case SOC_MSG_DONTWAIT: {
                        flag = (flag | MSG_DONTWAIT);
                        break;
                    }
                    default: {
                        flag = 0;
                    }
                }
            }
            hal_wdt_feed(HAL_WDT_FEED_MAGIC);
            if (socket_t->type == SOCK_STREAM) {
                SOC_LOG("send %s", raw_data_buf);
                result = send(socket_id, raw_data_buf, data_len, flag);
            } else {
                switch(socket_t->domain){
                    case AF_INET: {
                         struct sockaddr_in to;
                         to.sin_family = socket_t->domain;
                         memcpy(&(to.sin_addr), &(socket_t->remote_address), sizeof(socket_t->remote_address));
                         to.sin_port = socket_t->remote_port;
                         result = sendto(socket_id, raw_data_buf, data_len, flag, (struct sockaddr *)&to, sizeof(to));
                         break;
                    }
#if LWIP_IPV6
                    case AF_INET6: {
                         struct sockaddr_in6 to;
                         to.sin6_family = socket_t->domain;
                         memcpy(&(to.sin6_addr), &(socket_t->remote_v6_addr), sizeof(to.sin6_addr));
                         to.sin6_port = socket_t->remote_port;
                         result = sendto(socket_id, raw_data_buf, data_len, flag, (struct sockaddr *)&to, sizeof(to));
                         break;
                    }
#endif
                    default: {
                         soc_result = SOCKET_NW_ILLEGAL_VALUE_ERROR;
                         break;
                    }
                }
            }
            
            SOC_LOG("result: %d", result);
            if (result != data_len){
                soc_result = SOCKET_NW_COMMON_ERROR;
            }
            _free_buffer(data_buf);
            _free_buffer(raw_data_buf);
            break;
        }
        default: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
    }
    if (soc_result != SOCKET_NW_NO_ERROR){
        cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    }
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_disconnect_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    int socket_id, flag, result, has_flag = 0;
    char test_case_num_str[5] = {0};
    char* start = NULL;
    char* temp = NULL;
    socket_atcmd_t* socket_t = NULL;

    SOC_LOG("disconnect: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_OK;
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_TESTING: {
            break;
        }
        case APB_PROXY_CMD_MODE_READ: {
            cmd_result.pdata = "+ESODIS=<socket_id>";
            cmd_result.length = strlen((char *)cmd_result.pdata);
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = strchr(p_parse_cmd->string_ptr, '=');
            start++;
            temp = strchr(start, ',');
            if (temp){
                has_flag = 1;
                strncpy(test_case_num_str, start, temp - start);
            } else {
                strncpy(test_case_num_str, start, strlen(start));
            }
            socket_id = _atoi(test_case_num_str);
            SOC_LOG("disconnect socket_id %d", socket_id);

            socket_t = socket_atcmd_find_socket(socket_id);
            if (socket_t == NULL) {
                cmd_result.pdata = "socket_id parameter error !!";
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                cmd_result.length = strlen((char *)cmd_result.pdata);
                break;
            }

            if (has_flag) {
                start = temp + 1;
                strncpy(test_case_num_str, start, strlen(start));
                flag = _atoi(test_case_num_str);
            } else
                flag = 0;

            SOC_LOG("disconnect flag %d", flag);

            if (socket_t->soc_data_mode_conn_id != 0) {
                socket_close_data_mode(socket_id);
            }

            result = close(socket_id);
            socket_t->is_connected = 0;
            SOC_LOG("disconnect result %d", result);
            if (result != 0)
            {
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            }
            cmd_result.length = 0;
            break;
        }
        default: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
    }

    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_close_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    int socket_id;
    char test_case_num_str[5] = {0};
    char* start = NULL;
    socket_atcmd_t*  socket_t = NULL;

    SOC_LOG("socket close: [%s]", p_parse_cmd->string_ptr);

    cmd_result.result_code = APB_PROXY_RESULT_OK;
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_TESTING: { //+ESOCL=?
            break;
        }
        case APB_PROXY_CMD_MODE_READ: {   //+ESOCL=<socket_id>
            cmd_result.pdata = "+ESOCL=<socket_id>";
            cmd_result.length = strlen((char *)cmd_result.pdata);
            break;
        }
        case APB_PROXY_CMD_MODE_ACTIVE: {  //+ESOCL
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = strchr(p_parse_cmd->string_ptr, '=');
            start++;
            strncpy(test_case_num_str, start, strlen(start));
            socket_id = _atoi(test_case_num_str);
            SOC_LOG("close socket_id %d", socket_id);
            socket_t = socket_atcmd_find_socket(socket_id);
            if (socket_t == NULL) {
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                cmd_result.pdata = "socket_id parameter error !!";
                cmd_result.length = strlen((char *)cmd_result.pdata);
                break;
            }

            if (socket_t->soc_data_mode_conn_id != 0) {
                socket_close_data_mode(socket_id);
            }

            close(socket_id);
            socket_t->is_connected = 0;
            socket_atcmd_remove_socket(socket_t->socket_id);

            cmd_result.length = 0;
            break;
        }
        default: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
    }

    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}



static void apb_proxy_socket_data_mode_cb(apb_proxy_event_type_t event, void *pdata)
{
    int socket_id, data_len, copy_len, result = 0, has_flag = 0, flag = MSG_DONTWAIT;
    soc_nw_result_t soc_result = SOCKET_NW_NO_ERROR;
    socket_atcmd_t*  socket_t = NULL;
    apb_proxy_data_mode_event_t *p_data_event = (apb_proxy_data_mode_event_t *)(pdata);
    apb_proxy_user_data_t *p_user_data;

    configASSERT(p_data_event != NULL);
    p_user_data = &(p_data_event->event_data.user_data);

    socket_t = socket_atcmd_find_socket_by_conn_id(p_data_event->conn_id);
    if (socket_t == NULL) {
        SOC_LOG("callback parameter error !!");
        return;
    }
    socket_id = socket_t->socket_id;
    switch (event) {
        case APB_PROXY_USER_DATA_IND: {
                SOC_LOG("APB_PROXY_USER_DATA_IND");
                SOC_LOG("Receive user data: %d", p_user_data->length);

                if (socket_t->type == SOCK_STREAM) {
                    result = send(socket_id, p_user_data->pbuffer, p_user_data->length, flag);
                } else {
                    switch(socket_t->domain){
                        case AF_INET: {
                             struct sockaddr_in to;
                             to.sin_family = socket_t->domain;
                             memcpy(&(to.sin_addr), &(socket_t->remote_address), sizeof(socket_t->remote_address));
                             to.sin_port = socket_t->remote_port;
                             result = sendto(socket_id, p_user_data->pbuffer, p_user_data->length, flag, (struct sockaddr *)&to, sizeof(to));
                             break;
                        }
#if LWIP_IPV6
                        case AF_INET6: {
                             struct sockaddr_in6 to;
                             to.sin6_family = socket_t->domain;
                             memcpy(&(to.sin6_addr), &(socket_t->remote_v6_addr), sizeof(to.sin6_addr));
                             to.sin6_port = socket_t->remote_port;
                             result = sendto(socket_id, p_user_data->length, p_user_data->length, flag, (struct sockaddr *)&to, sizeof(to));
                             break;
                        }
#endif
                        default: {
                             soc_result = SOCKET_NW_ILLEGAL_VALUE_ERROR;
                             break;
                        }
                    }
                }
                SOC_LOG("send result: %d", result);

                break;
            }
            case APB_PROXY_STOP_SEND_USER_DATA: {
                socket_t->soc_data_mode_status = SOC_DATA_MODE_STOP_SEND;
                SOC_LOG("APB_PROXY_STOP_SEND_USER_DATA");
                break;
            }
            case APB_PROXY_RESUME_SEND_USER_DATA: {
                socket_t->soc_data_mode_status = SOC_DATA_MODE_ACTIVE;
                SOC_LOG("APB_PROXY_RESUME_SEND_USER_DATA");
                break;
            }
            case APB_PROXY_DATA_MODE_TEMP_DEACTIVEED_IND: {
                socket_t->soc_data_mode_status = SOC_DATA_MODE_TEMP_DEACTIVATED;
                SOC_LOG("APB_PROXY_DATA_MODE_TEMP_DEACTIVEED_IND");
                break;
            }
            case APB_PROXY_DATA_MODE_RESUMED_IND: {
                socket_t->soc_data_mode_status = SOC_DATA_MODE_ACTIVE;
                SOC_LOG("APB_PROXY_DATA_MODE_RESUMED_IND");
                break;
            }
            case APB_PROXY_DATA_MODE_CLOSED_IND: {
                SOC_LOG("APB_PROXY_DATA_MODE_CLOSED_IND");
                break;
            }
            default: {
                configASSERT(0);
                break;
            }
        }

}

apb_proxy_status_t apb_proxy_hdlr_datamode_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    int socket_id, data_mode;
    char test_case_num_str[5] = {0};
    char temp_buffer[64] = {0};
    char* start = NULL;
    char* temp = NULL;
    socket_atcmd_t*  socket_t = NULL;
    soc_nw_result_t soc_result = SOCKET_NW_NO_ERROR;

    SOC_LOG("socket data mode: [%s], cmd_id: [%d]", p_parse_cmd->string_ptr, p_parse_cmd->cmd_id);

    cmd_result.result_code = APB_PROXY_RESULT_OK;
    cmd_result.pdata = NULL;
    cmd_result.length = 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_TESTING: { //+ESODATAMODE=?
            cmd_result.pdata = "AT+ESODATAMODE=<socket_id>,<data_mode>";
            cmd_result.length = strlen((char *)cmd_result.pdata);
            break;
        }
        case APB_PROXY_CMD_MODE_READ:
        case APB_PROXY_CMD_MODE_ACTIVE: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION: {
            start = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;

            /*1. Parsing the socket ID.*/
            temp = strchr(start, ',');
            if ((temp == NULL) || ((temp - start) >= sizeof(temp_buffer))) {
                soc_result = SOCKET_NW_PARAMETER_ERROR;
                break;
            }
            memset(temp_buffer, 0, sizeof(temp_buffer));
            memcpy(temp_buffer, start, temp - start);
            SOC_LOG("temp_buffer: %s", temp_buffer);
            socket_id = _atoi(temp_buffer);
            SOC_LOG("switch data mode for socket: %d", socket_id);
            socket_t = socket_atcmd_find_socket(socket_id);
            if (socket_t == NULL) {
                cmd_result.pdata = "socket_id parameter error !!";
                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                cmd_result.length = strlen((char *)cmd_result.pdata);
                break;
            }

            /*2. Parsing the data mode.*/
            start = temp + 1;
            //temp = strchr(start, ',');
            //if ((temp == NULL) || ((temp - start) >= sizeof(temp_buffer))) {
            //    soc_result = SOCKET_NW_PARAMETER_ERROR;
            //    break;
            //}
            memset(temp_buffer, 0, sizeof(temp_buffer));
            memcpy(temp_buffer, start, strlen(start));
            data_mode = _atoi(temp_buffer);
            SOC_LOG("data mode: %d", data_mode);

            if (data_mode == 1) {
                if (socket_t->is_connected != 1) {
                    SOC_LOG("create data mode failed: socket[%d] is not connected", socket_id);
                    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                    break;
                }
                if (socket_t->soc_data_mode_conn_id != 0) {
                    if (socket_t->soc_data_mode_status == SOC_DATA_MODE_TEMP_DEACTIVATED) {
                        // resume data mode
                            if (apb_proxy_resume_data_mode(socket_t->soc_data_mode_conn_id) == APB_PROXY_STATUS_OK) {
                                socket_t->soc_data_mode_status = SOC_DATA_MODE_ACTIVE;
                                SOC_LOG("resume data mode: socket[%d] data mode temporary closed, resume it", socket_id);
                                return APB_PROXY_STATUS_OK;
                            } else {
                                SOC_LOG("resume data mode failed");
                                cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                            }

                    } else {
                        SOC_LOG("create data mode failed: socket[%d] already in data mode", socket_id);
                        cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                    }
                    break;
                }
                // create data mode
                cmd_result.result_code = APB_PROXY_RESULT_CONNECT;
                if (apb_proxy_send_at_cmd_result(&cmd_result) == APB_PROXY_STATUS_OK){
                    SOC_LOG("create data mode");
                    socket_t->soc_data_mode_cmd_id =  p_parse_cmd->cmd_id;
                    SOC_LOG("create data mode: socket_t->soc_data_mode_cmd_id = %d", socket_t->soc_data_mode_cmd_id);
                    SOC_LOG("create data mode: current socket data channel count = %d", g_soc_data_channel_count);
                    socket_t->soc_data_mode_conn_id = apb_proxy_create_data_mode(apb_proxy_socket_data_mode_cb, p_parse_cmd->cmd_id);
                    if (socket_t->soc_data_mode_conn_id == 0){
                        SOC_LOG("create data mode failed: can't go to data mode");
                        cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                    } else {
                        socket_t->soc_data_mode_status = SOC_DATA_MODE_ACTIVE;
                        g_soc_data_channel_count++;
                    }
                }else {
                    SOC_LOG("send at command result failed");
                    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                }
                break;
            } else {
                if (socket_t->is_connected != 1) {
                    SOC_LOG("close data mode failed: socket[%d] is not connected", socket_id);
                    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                    break;
                }
                if (socket_t->soc_data_mode_conn_id == 0) {
                    SOC_LOG("close data mode failed: socket[%d] already in command mode", socket_id);
                    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                    break;
                }
                // close data mode
                SOC_LOG("close data mode");
                socket_close_data_mode(socket_id);
                return APB_PROXY_STATUS_OK;
            }
            cmd_result.length = 0;
            break;
        }
        default: {
            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
            break;
        }
    }

    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}

static void socket_dns_task(void *parameter)
{
    char* p_url_buf = (char*)parameter;
    char ip_out[128] = {0};
    struct addrinfo hints, *addr_list, *cur;
    int ret = 0;
    char port[10] = {0};
    apb_proxy_at_cmd_result_t cmd_result = {0};
    cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
    cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
    cmd_result.pdata = NULL;
    cmd_result.length = 0;

    memset( &hints, 0, sizeof( hints ) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    snprintf(port, sizeof(port), "%d", 80);

    if ( getaddrinfo(p_url_buf, port , &hints, &addr_list ) != 0 ) {
        snprintf(ip_out, sizeof(ip_out), "%s", "+EDNS:ERROR");
        cmd_result.pdata = ip_out;
        cmd_result.length = strlen(ip_out);
    } else {
        for ( cur = addr_list; cur != NULL; cur = cur->ai_next ) {
        uint16_t ip_address[IP_ADDRESS_MAX_COUNT] = {0};
        memset(ip_out, 0, sizeof(ip_out));
        if (AF_INET == cur->ai_family){
            ip4_addr_t ip4_addr;
            struct sockaddr_in* p_sockaddr_in = (struct sockaddr_in *)(cur->ai_addr);
            inet_addr_to_ipaddr(&ip4_addr, &(p_sockaddr_in->sin_addr));
            ip_address[0] = ip4_addr1_16(&ip4_addr);
            ip_address[1] = ip4_addr2_16(&ip4_addr);
            ip_address[2] = ip4_addr3_16(&ip4_addr);
            ip_address[3] = ip4_addr4_16(&ip4_addr);
            snprintf(ip_out, sizeof(ip_out), "+EDNS:%d.%d.%d.%d", ip_address[0],
                     ip_address[1], ip_address[2], ip_address[3]);
            SOC_LOG("tranlated ip_address: %s\r\n", ip_out);
            cmd_result.pdata = ip_out;
            cmd_result.length = strlen(cmd_result.pdata);
            apb_proxy_send_at_cmd_result(&cmd_result);
        }
        else if (AF_INET6 == cur->ai_family){
            static ip6_addr_t ip6_addr;
            struct sockaddr_in6* p_sockaddr_in = (struct sockaddr_in6 *)(cur->ai_addr);
            inet6_addr_to_ip6addr(&ip6_addr, &(p_sockaddr_in->sin6_addr));
            ip_address[0] = IP6_ADDR_BLOCK1(&ip6_addr);
            ip_address[1] = IP6_ADDR_BLOCK2(&ip6_addr);
            ip_address[2] = IP6_ADDR_BLOCK3(&ip6_addr);
            ip_address[3] = IP6_ADDR_BLOCK4(&ip6_addr);
            ip_address[4] = IP6_ADDR_BLOCK5(&ip6_addr);
            ip_address[5] = IP6_ADDR_BLOCK6(&ip6_addr);
            ip_address[6] = IP6_ADDR_BLOCK7(&ip6_addr);
            ip_address[7] = IP6_ADDR_BLOCK8(&ip6_addr);
            snprintf(ip_out, sizeof(ip_out), "+EDNS: %x:%x:%x:%x:%x:%x:%x:%x",
                     ip_address[0], ip_address[1],ip_address[2], ip_address[3],
                     ip_address[4], ip_address[5], ip_address[6],ip_address[7]);
            SOC_LOG("tranlated ip_address: %s\r\n", ip_out);
            cmd_result.pdata = ip_out;
            cmd_result.length = strlen(cmd_result.pdata);
            apb_proxy_send_at_cmd_result(&cmd_result);
        }
    }

    }


    freeaddrinfo( addr_list );
    vPortFree(parameter);
    g_dns_task_handle = NULL;
    vTaskDelete(NULL);
}

apb_proxy_status_t apb_proxy_hdlr_dns_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    cmd_result.pdata = NULL;
    cmd_result.length = 0;
    if (g_dns_task_handle != NULL) {
        apb_proxy_send_at_cmd_result(&cmd_result);
        return APB_PROXY_STATUS_OK;
    }
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION: {
            uint32_t url_host_buf_len = p_parse_cmd->string_len - p_parse_cmd->parse_pos;
            char* p_parameter = p_parse_cmd->string_ptr + p_parse_cmd->parse_pos;
            uint32_t index = 0;
            char* p_url_buf = NULL;
            uint32_t p_url_len = 0;
            if (url_host_buf_len == 0){
                break;
            }
            if (*p_parameter == '\"'){
                p_url_buf = p_parameter + 1;
                for(index = 1; index < (p_parse_cmd->string_len - p_parse_cmd->parse_pos); index ++){
                    if (p_parameter[index] == '\"'){
                        p_parameter[index] = '\0';
                        break;
                    }
                    p_url_len ++;
                }
                if (p_url_len > 0){
                    char* parameter = (char *)pvPortMalloc(strlen(p_url_buf) + 1);
                    memcpy(parameter, p_url_buf, strlen(p_url_buf) + 1);
                    xTaskCreate(socket_dns_task,
                                "DNS",
                                1024 * 4 / sizeof(portSTACK_TYPE),
                                (void*)parameter,
                                TASK_PRIORITY_NORMAL,
                                &g_dns_task_handle);
                    cmd_result.result_code = APB_PROXY_RESULT_OK;
                }
            }
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING:
        case APB_PROXY_CMD_MODE_READ:
        case APB_PROXY_CMD_MODE_ACTIVE:
        default: {
            break;
        }
    }

    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}

extern apb_proxy_status_t apb_proxy_hdlr_ipconfig_cmd(apb_proxy_parse_cmd_param_t * p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    cmd_result.pdata = NULL;
    cmd_result.length = 0;
    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_ACTIVE:{
            uint32_t index = 0;
            uint32_t netif_count = netif_get_if_count();
            netifreq netif_req;
            SOC_LOG("netif_count = %d\r\n", netif_count);
            if (netif_count != 0) {
                char response[128] = {0};
                for(index = 0; index < netif_count; index ++) {
                    bool result = netif_get_network_if_by_num(&netif_req, index);
                    if (result == true) {
                        memset(response, 0, sizeof(response));
                        if (netif_req.be_ip4 == true) {
                            snprintf((char *)response, sizeof(response), "+IPCONFIG: %d.%d.%d.%d",
                                     ip4_addr1_16(&(netif_req.ip_addr)), ip4_addr2_16(&(netif_req.ip_addr)),
                                     ip4_addr3_16(&(netif_req.ip_addr)), ip4_addr4_16(&(netif_req.ip_addr)));
                            cmd_result.pdata = response;
                            cmd_result.length = strlen(response);
                            cmd_result.result_code = APB_PROXY_RESULT_PROCEEDING;
                            apb_proxy_send_at_cmd_result(&cmd_result);
                        }else {
                            uint32_t i = 0;
                            for(i = 0; i < netif_req.ip6_count; i++) {
                                snprintf((char *)response, sizeof(response), "+IPCONFIG: %x:%x:%x:%x:%x:%x:%x:%x",
                                         IP6_ADDR_BLOCK1(netif_req.ip6_addr + i),
                                         IP6_ADDR_BLOCK2(netif_req.ip6_addr + i),
                                         IP6_ADDR_BLOCK3(netif_req.ip6_addr + i),
                                         IP6_ADDR_BLOCK4(netif_req.ip6_addr + i),
                                         IP6_ADDR_BLOCK5(netif_req.ip6_addr + i),
                                         IP6_ADDR_BLOCK6(netif_req.ip6_addr + i),
                                         IP6_ADDR_BLOCK7(netif_req.ip6_addr + i),
                                         IP6_ADDR_BLOCK8(netif_req.ip6_addr + i));
                                cmd_result.pdata = response;
                                cmd_result.length = strlen(response);
                                cmd_result.result_code = APB_PROXY_RESULT_PROCEEDING;
                                apb_proxy_send_at_cmd_result(&cmd_result);
                            }
                        }
                    }
                }
                cmd_result.pdata = NULL;
                cmd_result.length = 0;
                cmd_result.result_code = APB_PROXY_RESULT_OK;
            }
            break;
        }
        case APB_PROXY_CMD_MODE_EXECUTION:
        case APB_PROXY_CMD_MODE_TESTING:
        case APB_PROXY_CMD_MODE_READ:
        default: {
            break;
        }
    }

    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}

void socket_atcmd_init_task(void)
{
    bool power_from_deep = false;

    if ((rtc_power_on_result_external() == DEEP_SLEEP) ||
        (rtc_power_on_result_external() == DEEPER_SLEEP)){
        power_from_deep = true;
    }

    if (true == power_from_deep) {
        if(g_soc_atcmd_task_handle == NULL) {
            xTaskCreate(socket_loop_task,
                            "socket_atcmd",
                            1024 * 4 / sizeof(portSTACK_TYPE),
                            (void*)1,
                            TASK_PRIORITY_NORMAL,
                            &g_soc_atcmd_task_handle);
            SOC_LOG("create socket task");
        }
    }
}

