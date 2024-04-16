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
#include "FreeRTOS.h"
#include "iperf_task.h"
#include "ping.h"
#include "tel_conn_mgr_common.h"
#include "nidd_gprot.h"
#include "apb_proxy_nw_cmd_gprot.h"
#include "apb_proxy_nw_cmd_util.h"

#ifdef TEL_CONN_MGR_APB_SUPPORT
#include "tel_conn_mgr_platform.h"
#include "tel_conn_mgr_ut.h"
#include "tel_conn_mgr_bearer_info.h"
#include "tel_conn_mgr_app_api.h"
#endif
#include "memory_attribute.h"
#include "hal_rtc_internal.h"
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_platform.h"
#include "hal_rtc_external.h"
#include "timers.h"
#include "syslog.h"

#define PING_PACKET_MIN_SIZE 8
#define PING_PACKET_MAX_SIZE 5120

#ifdef TEL_CONN_MGR_APB_SUPPORT
#define APB_NW_APP_QUEUE_MAX_SIZE (5)

typedef struct
{
    int op;
    tel_conn_mgr_pdp_type_enum pdp_type;
    int cid;
    tel_conn_mgr_bearer_type_enum bearer_type;
    tel_conn_mgr_sim_id_enum sim_id;
    char apn[TEL_CONN_MGR_APN_MAX_LEN + 1];
    char username[TEL_CONN_MGR_USERNAME_MAX_LEN + 1];
    char password[TEL_CONN_MGR_PASSWORD_MAX_LEN + 1];
}apb_proxy_nw_bearer_act_param_struct;


typedef struct
{
    tel_conn_mgr_queue_hdl_t queue_hdl;
    unsigned int app_id[TEL_CONN_MGR_BEARER_TYPE_MAX_NUM];
    int cid[TEL_CONN_MGR_BEARER_TYPE_MAX_NUM];
    bool is_used[TEL_CONN_MGR_BEARER_TYPE_MAX_NUM];
}apb_proxy_nw_egact_cntx_struct;

apb_proxy_nw_egact_cntx_struct *egact_cntx = NULL;

#endif
#define UPSM_AT_CMD_PARAM_NUM         (6)

typedef struct _upsm_arg
{
    u8_t is_ongoing;
    u8_t is_ipv6;
    u16_t port;
    u16_t size;
    u16_t total_num;
    int interval_time;
    char ipaddr[40];
    u32_t timer_handle;
}upsm_arg_t;

static ATTR_ZIDATA_IN_RETSRAM upsm_arg_t g_upsm_arg;
SemaphoreHandle_t g_upsm_need_send = NULL;
static bool g_ping_enable_debug_info = false;

static bool apb_proxy_iperf_server(char *param);
static bool apb_proxy_iperf_client(char *param);
static void apb_proxy_iperf_cb(iperf_result_t* iperf_result);
static void apb_proxy_ping_cb(ping_result_type_t type, void* ping_result);

log_create_module(apb_nw, PRINT_LEVEL_INFO);

#if defined(__MTK_NBIOT_SLIM__)
#define NW_LOG(fmt, args...) 
#define NW_LOGW(fmt, args...)
#define NW_LOGE(fmt, args...)
#else
#ifdef APBNW_MODULE_PRINTF
#define NW_LOG(fmt, args...)               printf("[APB NW] "fmt, ##args)
#else
#define NW_LOG(fmt, args...)               LOG_I(apb_nw, "[APB NW] "fmt, ##args)
#define NW_LOGW(fmt, args...)              LOG_W(apb_nw, "[APB NW] "fmt, ##args)
#define NW_LOGE(fmt, args...)              LOG_E(apb_nw, "[APB NW] "fmt, ##args)
#endif
#endif

apb_proxy_status_t apb_proxy_hdlr_iperf_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    apb_proxy_at_cmd_result_t response;
    char *param = NULL;

    NW_LOG("atci_cmd_hdlr_iperf\n");
    memset(&response, 0, sizeof(apb_proxy_at_cmd_result_t));
    response.result_code = APB_PROXY_RESULT_ERROR;
    response.cmd_id = parse_cmd->cmd_id;

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+IPERF=?
            response.result_code = APB_PROXY_RESULT_OK;
            break;

        case APB_PROXY_CMD_MODE_READ:   // rec: AT+IPERF?
            response.pdata = "+IPERF:<ip>";
            response.length = strlen((char *)response.pdata);
            response.result_code = APB_PROXY_RESULT_OK;
            break;

        case APB_PROXY_CMD_MODE_ACTIVE: // rec: AT+IPERF
            // assume the active mode is invalid and we will return "ERROR"
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;

        case APB_PROXY_CMD_MODE_EXECUTION: // rec: AT+IPERF=<p1>  the handler need to parse the parameters
            NW_LOG("AT CMD received: %s\r\n", parse_cmd->string_ptr);
            param = parse_cmd->string_ptr + parse_cmd->parse_pos;
            if (param != NULL) {
                //remove \r
                int len = parse_cmd->string_len - parse_cmd->parse_pos - 3 - 1;
                if(len < 0) {
                    len = 0;
                }
                char *str = (char *)pvPortMalloc(len + 1);
                if (str == NULL) {
                    break;
                }
                if(len > 0) {
                    memcpy(str, param + 3, len);
                }
                str[len] = '\0';
                NW_LOG("parameter: %s\r\n", str);
                if (param[0] == '-' && param[1] == 'c') {
                    if (apb_proxy_iperf_client(str)== true){
                        response.result_code = APB_PROXY_RESULT_OK;
                    }
                } else if (param[0] == '-' && param[1] == 's') {
                    if (apb_proxy_iperf_server(str) == true){
                        response.result_code = APB_PROXY_RESULT_OK;
                    }
                }
                vPortFree(str);
            }
            break;
        default :
            break;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);
    return APB_PROXY_STATUS_OK;
}


static bool apb_proxy_iperf_server(char *param)
{
    int i;
    char **g_iperf_param = NULL;
    int offset = IPERF_COMMAND_BUFFER_SIZE / sizeof(char *);
    char *p = NULL;
    int is_create_task = 0;
    iperf_type_t iperf_type = IPERF_TYPE_MAX;

    if (param == NULL) {
        return false;
    }

    g_iperf_param = pvPortCalloc(1, IPERF_COMMAND_BUFFER_NUM * IPERF_COMMAND_BUFFER_SIZE);
    if (g_iperf_param == NULL) {
        return false;
    }

    i = 0;
    p = strtok(param, " ");
    while(p != NULL && i < 13) {
        strcpy((char *)&g_iperf_param[i * offset], p);
        i++;
        p = strtok(NULL, " ");
    }


    for (i = 0; i < 13; i++) {
        if (strncmp((char *)&g_iperf_param[i * offset], "-u", 2) == 0) {
            iperf_type = IPERF_UDP_SERVER;
            break;
        }
    }

    if (strncmp((char *)&g_iperf_param[i * offset], "-u", 2) != 0) {
        iperf_type = IPERF_TCP_SERVER;
    }

    if (is_iperf_running(iperf_type) == true){
        vPortFree(g_iperf_param);
        return false;
    }

    if (iperf_type == IPERF_UDP_SERVER){
        iperf_register_callback(IPERF_UDP_SERVER, apb_proxy_iperf_cb);
        NW_LOG("Iperf run UDP server\r\n");
        xTaskCreate((TaskFunction_t)iperf_udp_run_server, IPERF_TASK_NAME, IPERF_TASK_STACKSIZE / (( uint32_t )sizeof( StackType_t )), g_iperf_param, IPERF_TASK_PRIO, NULL);
    }else if (iperf_type == IPERF_TCP_SERVER) {
        iperf_register_callback(IPERF_TCP_SERVER, apb_proxy_iperf_cb);
        NW_LOG("Iperf run TCP server\r\n");
        xTaskCreate((TaskFunction_t)iperf_tcp_run_server, IPERF_TASK_NAME, IPERF_TASK_STACKSIZE / sizeof(portSTACK_TYPE), g_iperf_param, IPERF_TASK_PRIO , NULL);
    }else {
        vPortFree(g_iperf_param);
        return false;
    }
    return true;

}

static bool apb_proxy_iperf_client(char *param)
{
    int i;
    char **g_iperf_param = NULL;
    int offset = IPERF_COMMAND_BUFFER_SIZE / sizeof(char *);
    int is_create_task = 0;
    char *p = NULL;
    iperf_type_t iperf_type = IPERF_TYPE_MAX;

    if (param == NULL) {
        return false;
    }

    g_iperf_param = pvPortCalloc(1, IPERF_COMMAND_BUFFER_NUM * IPERF_COMMAND_BUFFER_SIZE);
    if (g_iperf_param == NULL) {
        return false;
    }

    i = 0;
    p = strtok(param, " ");
    while(p != NULL && i < 18) {
        strcpy((char *)&g_iperf_param[i * offset], p);
        i++;
        p = strtok(NULL, " ");
    }

    for (i = 0; i < 18; i++) {
        if (strncmp((char *)&g_iperf_param[i * offset], "-u", 2) == 0) {
            iperf_type = IPERF_UDP_CLIENT;
            break;
        }
    }

    if (strncmp((char *)&g_iperf_param[i * offset], "-u", 2) != 0) {
        iperf_type = IPERF_TCP_CLIENT;
    }


    if (is_iperf_running(iperf_type) == true){
        vPortFree(g_iperf_param);
        return false;
    }

    if (iperf_type == IPERF_UDP_CLIENT){
        iperf_register_callback(IPERF_UDP_CLIENT, apb_proxy_iperf_cb);
        NW_LOG("Iperf run UDP client\r\n");
        xTaskCreate((TaskFunction_t)iperf_udp_run_client, IPERF_TASK_NAME, IPERF_TASK_STACKSIZE / sizeof(portSTACK_TYPE), g_iperf_param, IPERF_TASK_PRIO , NULL);
    }else if (iperf_type == IPERF_TCP_CLIENT) {
        iperf_register_callback(IPERF_TCP_CLIENT, apb_proxy_iperf_cb);
        NW_LOG("Iperf run TCP client\r\n");
        xTaskCreate((TaskFunction_t)iperf_tcp_run_client, IPERF_TASK_NAME, IPERF_TASK_STACKSIZE / sizeof(portSTACK_TYPE), g_iperf_param, IPERF_TASK_PRIO , NULL);
    }else {
        vPortFree(g_iperf_param);
        return false;
    }
    return true;
}

static void apb_proxy_iperf_cb(iperf_result_t* iperf_result)
{
    apb_proxy_at_cmd_result_t response;
    char res[100] = {0};

    if (iperf_result) {
        sprintf((char *)&res, "+iperf: finish, %s, data_size = %d, total = %s, result = %s",
            iperf_result->report_title, (int)iperf_result->data_size, iperf_result->total_len, iperf_result->result);
        response.pdata = &res;
    } else {
        response.pdata = "+iperf: finish, no result!";
    }

    response.cmd_id = APB_PROXY_INVALID_CMD_ID;
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;
    response.length = strlen((char *)response.pdata);
    apb_proxy_send_at_cmd_result(&response);
}


apb_proxy_status_t apb_proxy_hdlr_ping_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    apb_proxy_at_cmd_result_t response;
    char *param = NULL;
    int count = 1;
    int pktsz = 64;
    char ipaddr[PING_SERVER_MAX_LEN] = {0};
    int addr_type = PING_IP_ADDR_V4;
    int recv_timeout = 60000;
    int interval = 0;
    int debug_info = 0;

    NW_LOG("atci_cmd_hdlr_ping\n");
    memset(&response, 0, sizeof(apb_proxy_at_cmd_result_t));

    response.result_code = APB_PROXY_RESULT_ERROR;
    response.cmd_id = parse_cmd->cmd_id;
    response.length = 0;
    response.pdata = NULL;
    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_TESTING:
            response.result_code = APB_PROXY_RESULT_OK;
            apb_proxy_send_at_cmd_result(&response);
            break;

        case APB_PROXY_CMD_MODE_READ:
            response.pdata = "+PING:<ip>";
            response.length = strlen((char *)response.pdata);
            response.result_code = APB_PROXY_RESULT_OK;
            apb_proxy_send_at_cmd_result(&response);
            break;

        case APB_PROXY_CMD_MODE_ACTIVE:
            apb_proxy_send_at_cmd_result(&response);
            break;

        case APB_PROXY_CMD_MODE_EXECUTION: {
            char *p = NULL;
            char *param_ptr = NULL;
            int len;
            bool need_ping = true;
            int nw_type = 0;

            NW_LOG("AT CMD received:%s %d, pos %d\r\n", parse_cmd->string_ptr, (int)parse_cmd->string_len, (int)parse_cmd->parse_pos);
            param = parse_cmd->string_ptr + parse_cmd->parse_pos;
            //remove \r
            len = parse_cmd->string_len - parse_cmd->parse_pos - 1;
            param_ptr = (char *)pvPortMalloc(len + 1);
            if (param_ptr == NULL) {
                apb_proxy_send_at_cmd_result(&response);
                break;
            }
            memcpy(param_ptr, param, len);
            param_ptr[len] = '\0';
            if (strcmp(param_ptr, "\"close\"") == 0){
                NW_LOG("Close ping thread.");
                if (ping_close() == PING_OK){
                    response.result_code = APB_PROXY_RESULT_OK;
                }else{
                    response.result_code = APB_PROXY_RESULT_ERROR;
                }
                apb_proxy_send_at_cmd_result(&response);
                vPortFree(param_ptr);
                param_ptr = NULL;
                break;
            }
            p = strtok(param_ptr, " ");
            if (p != NULL) {
                NW_LOG("addr: %s, %d\r\n", p, strlen(p));
                memcpy(ipaddr, p, strlen(p));
                while(p != NULL) {
                    NW_LOG("ptr: %s\r\n", p);
                    if ((strcmp(p, "-l") == 0) || (strcmp(p, "-L") == 0)) {
                        p = strtok(NULL, " ");
                        NW_LOG("pktsz: %s\r\n", p);
                        if (p != NULL) {
                            pktsz = atoi(p);
                        }
                    } else if ((strcmp(p, "-n") == 0) || (strcmp(p, "-N") == 0)) {
                        p = strtok(NULL, " ");
                        NW_LOG("count: %s\r\n", p);
                        if (p != NULL) {
                            count = atoi(p);
                        }
                    } else if ((strcmp(p, "-w") == 0) || (strcmp(p, "-W") == 0)) {
                        p = strtok(NULL, " ");
                        NW_LOG("timeout: %s\r\n", p);
                        if (p != NULL) {
                            recv_timeout = atoi(p);
                        }
                    } else if (strcmp(p, "-6") == 0) {
                        addr_type = PING_IP_ADDR_V6;
                    }else if (strcmp(p, "-i") == 0) {
                        p = strtok(NULL, " ");
                        if (p != NULL) {
                            interval = atoi(p);
                            NW_LOG("ping interval: %d\r\n", interval);
                        }
                    }else if ((strcmp(p, "-d") == 0) || (strcmp(p, "-D") == 0)){
                        p = strtok(NULL, " ");
                        if (p != NULL) {
                            debug_info = atoi(p);
                            NW_LOG("ping debug info function: %d\r\n", debug_info);
                        }
                    }
                    else if ((strcmp(p, "-g") == 0) || (strcmp(p, "-G") == 0)){
                        p = strtok(NULL, " ");
                        if (p != NULL) {
                            nw_type = atoi(p);
                            NW_LOG("ping nw_type: %d\r\n", nw_type);
                        }
                    }
                    p = strtok(NULL, " ");
                }
            } else {
                if (strlen(ipaddr) < sizeof(ipaddr)) {
                    memcpy(ipaddr, param, strlen(ipaddr));
                } else {
                    NW_LOG("input address is too long\r\n");
                    need_ping = false;
                }
            }
            vPortFree(param_ptr);
            NW_LOG("ping addr: %s, count %d, addrtype %d, pktsz %d, timeout %d\r\n",
                    ipaddr, count, addr_type, pktsz, recv_timeout);
            if ((pktsz >= PING_PACKET_MIN_SIZE) &&
                (pktsz <= PING_PACKET_MAX_SIZE) &&
                (count > 0) && (need_ping == true)){
                ping_status_t status;
                ping_arg_t ping_para;
                memset(&ping_para, 0, sizeof(ping_para));
                memcpy(ping_para.serv_addr, ipaddr, strlen(ipaddr));
                ping_para.callback = apb_proxy_ping_cb;
                ping_para.count = count;
                ping_para.size = pktsz;
                ping_para.recv_timeout = recv_timeout;
                ping_para.interval = (interval > 0 ? interval : 0);
                status = ping_request_ex(addr_type, &ping_para);
                if (status == PING_OK){
                    response.result_code = APB_PROXY_RESULT_OK;
                    if (debug_info == 1){
                        g_ping_enable_debug_info = true;
                    }else{
                        g_ping_enable_debug_info = false;
                    }
                }else if(status == PING_ERROR){
                    response.result_code = APB_PROXY_RESULT_ERROR;
                }else if(status == PING_RUNNING){
                    response.result_code = APB_PROXY_RESULT_BUSY;
                }
            }
            apb_proxy_send_at_cmd_result(&response);
            if (APB_PROXY_RESULT_OK == response.result_code){
                /*ping process will begin.*/
                char temp[160] = {0};
                snprintf(temp, sizeof(temp), "+ping: begin, %s, data size= %d", ipaddr, pktsz);
                response.cmd_id = APB_PROXY_INVALID_CMD_ID;
                response.result_code = APB_PROXY_RESULT_UNSOLICITED;
                response.pdata = temp;
                response.length = strlen((char *)response.pdata);
                apb_proxy_send_at_cmd_result(&response);
            }
            break;
        }
        default :
            response.result_code = APB_PROXY_RESULT_ERROR;
            apb_proxy_send_at_cmd_result(&response);
            break;
    }
    return APB_PROXY_STATUS_OK;
}


static void apb_proxy_ping_cb(ping_result_type_t type, void* ping_result)
{
    apb_proxy_at_cmd_result_t response = {0};
    char res[100] = {0};
    uint32_t offset = 0;

    switch(type){
        case PING_PACKET_RESULT: {
            ping_packet_result_t* p_ping_result = (ping_packet_result_t*)ping_result;
            if (g_ping_enable_debug_info == false){
                return;
            }
            if (p_ping_result == NULL){
                strcpy(res, "+ping: no result.");
            }else if (p_ping_result->is_timeout == true){
                strcpy(res, "+ping: time out");
            }else{
                if (p_ping_result->is_ipv4 == true){
                    snprintf((char *)res, sizeof(res), "+ping: %d.%d.%d.%d, ",p_ping_result->ip_address[0],
                           p_ping_result->ip_address[1], p_ping_result->ip_address[2], p_ping_result->ip_address[3]);
                }else{
                    snprintf((char *)res, sizeof(res), "+ping: %x:%x:%x:%x:%x:%x:%x:%x, ",p_ping_result->ip_address[0],
                         p_ping_result->ip_address[1], p_ping_result->ip_address[2], p_ping_result->ip_address[3],
                         p_ping_result->ip_address[4],p_ping_result->ip_address[5],
                         p_ping_result->ip_address[6],p_ping_result->ip_address[7]);
                }
                offset = strlen(res);
                snprintf(res + offset, sizeof(res) - offset, "received=%d bytes, rtt=%d ms, ttl=%d",
                        p_ping_result->packet_size, p_ping_result->rtt, p_ping_result->ttl);
            }
            break;
        }
        case PING_TOTAL_RESULT: {
            ping_result_t* p_ping_result = (ping_result_t*)ping_result;
            if (p_ping_result) {
                snprintf((char *)res, sizeof(res), "+ping: finish, Packets: Sent = %d, Received =%d, Lost = %d (%d%% loss)",
                    (int)p_ping_result->total_num, (int)p_ping_result->recv_num, (int)p_ping_result->lost_num, (int)((p_ping_result->lost_num * 100)/p_ping_result->total_num));
                response.cmd_id = APB_PROXY_INVALID_CMD_ID;
                response.result_code = APB_PROXY_RESULT_UNSOLICITED;
                response.pdata = res;
                response.length = strlen((char *)response.pdata);
                apb_proxy_send_at_cmd_result(&response);
                memset(res, 0, sizeof(res));
                snprintf((char *)res, sizeof(res), "+ping: RTT statistics: Minimum = %d, Maximum =%d, Average = %d ",
                         (int)p_ping_result->min_time, (int)p_ping_result->max_time, (int)p_ping_result->avg_time);
            } else {
                strcpy(res, "+ping: finish, no result!");
            }
            break;
        }
        default:{
            strcpy(res, "+ping: finish, no result!");
            break;
        }
    }
    response.cmd_id = APB_PROXY_INVALID_CMD_ID;
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;
    response.pdata = res;
    response.length = strlen((char *)response.pdata);
    apb_proxy_send_at_cmd_result(&response);
}


void apb_udp_send_urc(char *data){
    apb_proxy_at_cmd_result_t response;

    response.pdata = data;
    response.cmd_id = APB_PROXY_INVALID_CMD_ID;
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;
    response.length = strlen((char *)response.pdata);
    apb_proxy_send_at_cmd_result(&response);

}


static void apb_udp_send_packet()
{
    int sockfd = -1;
    if (g_upsm_arg.is_ipv6 == 0) {
        struct sockaddr_in servaddr;
        char *str = NULL;
        int nbytes = 0;
        int i;
        char data[30] = {0};

        NW_LOG("sent to server %s, port %d, size %d.\n", g_upsm_arg.ipaddr, g_upsm_arg.port, g_upsm_arg.size);
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(g_upsm_arg.port);
        servaddr.sin_addr.s_addr = inet_addr(g_upsm_arg.ipaddr);
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        str = pvPortCalloc(1, g_upsm_arg.size);
        if (str == NULL) {
            close(sockfd);
            vTaskDelete(NULL);
        }
        for(i = 0; i < g_upsm_arg.size; i++) {
            str[i] = (i % 10) + '0';
        }
        connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
        nbytes = send(sockfd, str, g_upsm_arg.size, 0);
        NW_LOG("sent data %d.\n", nbytes);
        sprintf((char *)&data, "+eupsm: sent data %d", nbytes);
        apb_udp_send_urc(data);
        close(sockfd);
        vPortFree(str);
    }
}

void apb_upsm_main_loop(void *arg)
{
    while(1) {
        xSemaphoreTake(g_upsm_need_send, portMAX_DELAY);
        apb_udp_send_packet();
        if(g_upsm_arg.total_num == 1) {
            char data[20];

            NW_LOG("sent done\n");
            sprintf((char *)&data, "+eupsm: sent done");
            apb_udp_send_urc(data);
            rtc_sw_timer_stop(g_upsm_arg.timer_handle);
            rtc_sw_timer_delete(g_upsm_arg.timer_handle);
            g_upsm_arg.is_ongoing = 0;
            //delete task and sem
            vSemaphoreDelete(g_upsm_need_send);
            vTaskDelete(NULL);
        }
    }
}

void apb_upsm_send_create_task()
{
    g_upsm_need_send = xSemaphoreCreateBinary();
    xTaskCreate(apb_upsm_main_loop,
                "upsm_atcmd",
                512 * 4 / sizeof(portSTACK_TYPE),
                NULL,
                TASK_PRIORITY_NORMAL,
                NULL);
    xSemaphoreGive(g_upsm_need_send);
}

void apb_upsm_init_task(void)
{
    bool power_from_deep = false;

    if ((rtc_power_on_result_external() == DEEP_SLEEP) ||
        (rtc_power_on_result_external() == DEEPER_SLEEP)){
        power_from_deep = true;
    }
    //from deep sleep.
    if ((true == power_from_deep) && (g_upsm_arg.is_ongoing == 1)) {
        g_upsm_need_send = xSemaphoreCreateBinary();
        xTaskCreate(apb_upsm_main_loop,
                    "upsm_atcmd",
                    512 * 4 / sizeof(portSTACK_TYPE),
                    NULL,
                    TASK_PRIORITY_NORMAL,
                    NULL);
    }
}

void apb_upsm_isr_callback(void *user_data)
{
    g_upsm_arg.total_num = g_upsm_arg.total_num - 1;
    NW_LOG("num: %d, interval: %d\n", g_upsm_arg.total_num, g_upsm_arg.interval_time);
    if(g_upsm_arg.total_num > 0) {
        if (portNVIC_INT_CTRL_REG & 0xff) { //isr context
            //from isr
            xSemaphoreGiveFromISR(g_upsm_need_send, pdFALSE);
        } else {
            xSemaphoreGive(g_upsm_need_send);
        }
    }
}

extern apb_proxy_status_t apb_proxy_hdlr_upsm_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    apb_proxy_at_cmd_result_t response;
    char *param = NULL;
    char ipaddr[40] = {0};
    u16_t port = 5001;
    u16_t size = 64;
    int interval_time = 30;
    int total_num = 10;
    u8_t is_ipv6 = 0;

    NW_LOG("atci_cmd_hdlr_eupsm\n");
    memset(&response, 0, sizeof(apb_proxy_at_cmd_result_t));
    response.cmd_id = parse_cmd->cmd_id;
    response.length = 0;
    response.pdata = NULL;
    response.result_code = APB_PROXY_RESULT_ERROR;

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+EUPSM=?
            NW_LOG("AT TEST OK.\n");
            response.result_code = APB_PROXY_RESULT_OK;
            break;

        case APB_PROXY_CMD_MODE_READ:    // rec: AT+EUPSM?
            NW_LOG("AT Read done.\n");
            response.pdata = "+PING:<ip>";
            response.length = strlen((char *)response.pdata);
            response.result_code = APB_PROXY_RESULT_OK;
            break;

        case APB_PROXY_CMD_MODE_ACTIVE:  // rec: AT+EUPSM
            NW_LOG("AT Active OK.\n");
            // assume the active mode is invalid and we will return "ERROR"
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;

        case APB_PROXY_CMD_MODE_EXECUTION: {// rec: AT+EUPSM=<p1>  the handler need to parse the parameters
            char *param_ptr = NULL;
            int len;
            char *param_list[UPSM_AT_CMD_PARAM_NUM];
            int at_cmd_num;
            int i;

            NW_LOG("AT Executing...\r\n");
            NW_LOG("AT CMD received:%s %d, pos %d\r\n", parse_cmd->string_ptr, parse_cmd->string_len, parse_cmd->parse_pos);
            if(g_upsm_arg.is_ongoing == 1) {
                response.pdata = "+eupsm is running";
                response.length = strlen((char *)response.pdata);
                response.result_code = APB_PROXY_RESULT_ERROR;
                break;
            }
            param = parse_cmd->string_ptr + parse_cmd->parse_pos;
            //remove \r
            len = parse_cmd->string_len - parse_cmd->parse_pos - 1;
            param_ptr = (char *)pvPortMalloc(len + 1);
            if (param_ptr == NULL) {
                break;
            }
            at_cmd_num = onenet_at_parse_cmd(param, param_ptr, param_list, UPSM_AT_CMD_PARAM_NUM);
            for(i = 0; i < at_cmd_num; i++) {
                switch(i) {
                    case 0:
                        memcpy(ipaddr, param_list[i], strlen(param_list[i]));
                        break;
                    case 1:
                        port = atoi(param_list[i]);
                        break;
                    case 2:
                        size = atoi(param_list[i]);
                        break;
                    case 3:
                        total_num = atoi(param_list[i]);
                        break;
                    case 4:
                        interval_time = atoi(param_list[i]);
                        break;
                    case 5:
                        if(strncmp(param_list[i], "v6", 2) == 0){
                            is_ipv6 = 1;
                        }
                        break;
                    default:
                        break;
                }
            }
            vPortFree(param_ptr);
            NW_LOG("server addr: %s, port %d, size %d, total_num %d,interval_time %d, v6 %d.\r\n",
                ipaddr, port, size, total_num, interval_time, is_ipv6);
            memcpy(g_upsm_arg.ipaddr, ipaddr, strlen(ipaddr));
            g_upsm_arg.is_ipv6 = is_ipv6;
            g_upsm_arg.port = port;
            g_upsm_arg.size = size;
            g_upsm_arg.total_num = total_num;
            g_upsm_arg.interval_time = interval_time;
            apb_upsm_send_create_task();
            rtc_sw_timer_create(&(g_upsm_arg.timer_handle), interval_time * 10, true, apb_upsm_isr_callback);
            rtc_sw_timer_start(g_upsm_arg.timer_handle);
            //hal_rtc_enter_retention_mode();
            NW_LOG("rtc timer handler %d\n", g_upsm_arg.timer_handle);
            g_upsm_arg.is_ongoing = 1;
            response.result_code = APB_PROXY_RESULT_OK;
            break;
        }
        default :
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;
    }
    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);
    return APB_PROXY_STATUS_OK;
}


#ifdef TEL_CONN_MGR_APB_SUPPORT
static bool apb_nw_app_set_cid_app_id(unsigned int idx, int cid, unsigned int app_id)
{
    int i = 0;
    bool ret = false;

    for (i = 0; i < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; i++)
    {
        if (i == idx)
        {
            /* Store cid && app_id */
            egact_cntx->cid[idx] = cid;
            egact_cntx->app_id[idx] = app_id;
            if (cid && app_id)
            {
                egact_cntx->is_used[idx] = true;
            }
            ret = true;
        }
        else if ((app_id == egact_cntx->app_id[i] ||
                 cid == egact_cntx->cid[i]) &&
                 !egact_cntx->is_used[i])
        {
            egact_cntx->cid[i] = 0;
            egact_cntx->app_id[i] = 0;
        }
    }

    return ret;
}


void apb_nw_app_set_is_used_by_app_id(bool is_used, unsigned int app_id)
{
    int i = 0;

    for (i = 0; i < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; i++)
    {
        if (egact_cntx->app_id[i] == app_id)
        {
            egact_cntx->is_used[i] = is_used;
            return;
        }
    }
}


static unsigned int apb_nw_app_get_app_id(int cid)
{
    int i = 0;

    for (i = 0; i < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; i++)
    {
        if (egact_cntx) {
            if (egact_cntx->cid[i] == cid && egact_cntx->is_used[i])
            {
                return egact_cntx->app_id[i];
            }
        }
    }

    return 0;
}


static int apb_nw_app_get_cid(unsigned int app_id, bool check_is_used)
{
    int i = 0;

    for (i = 0; i < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; i++)
    {
        if (egact_cntx->app_id[i] == app_id &&
            (!check_is_used || egact_cntx->is_used[i]))
        {
            return egact_cntx->cid[i];
        }
    }

    return 0;
}


/* op: 0 active deactivation, 1 activation, 2 passive deactivation
  * result: 0 false, 1 true
  * cid: cid is actually the app_id returned by activatip API
  */
void apb_nw_app_send_result_urc(int op, int result, int cid, int pdp_type)
{
    char result_str[25] = {0};
    apb_proxy_at_cmd_result_t response = {0};

    if (1 == op && 1 == result)
    {
        sprintf(result_str, "+EGACT:%d,%d,%d,%d", cid, op, result, pdp_type);
    }
    else
    {
        if (2 == op)
        {
            sprintf(result_str, "+EGACT:%d,%d", cid, op);
        }
        else
        {
            sprintf(result_str, "+EGACT:%d,%d,%d", cid, op, result);
        }
    }

    response.pdata = result_str;
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;
    response.length = strlen((char *)response.pdata);
    apb_proxy_send_at_cmd_result(&response);
}


static tel_conn_mgr_ret_enum apb_nw_app_activate(apb_proxy_nw_bearer_act_param_struct *param)
{
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    tel_conn_mgr_pdp_type_enum activated_pdp_type = TEL_CONN_MGR_PDP_TYPE_NONE;
    unsigned int bearer_info_idx, app_id;
    int cid = 0;

    if (!param || !egact_cntx || !egact_cntx->queue_hdl)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    if (tel_conn_mgr_bearer_info_find_by_info(param->bearer_type,
                                              param->sim_id,
                                              param->pdp_type,
                                              param->apn,
                                              &bearer_info_idx))
    {
        /* Check if cid has been stored. */
        cid = tel_conn_mgr_convt_bearer_info_idx_to_cid(bearer_info_idx);
        app_id = apb_nw_app_get_app_id(cid);
        if (app_id)
        {
            /* Duplicate activation request. */
            NW_LOG("duplicate act req. app_id:%d, cid:%d");
            return TEL_CONN_MGR_RET_DUPLICATION;
        }
    }

    /* app_id is used as <cid> in +EGACT */
    ret = tel_conn_mgr_activate(param->bearer_type,
                                param->sim_id,
                                param->pdp_type,
                                param->apn,
                                param->username,
                                param->password,
                                egact_cntx->queue_hdl,
                                &app_id,
                                (tel_conn_mgr_pdp_type_enum *)&activated_pdp_type);
    if (TEL_CONN_MGR_RET_OK == ret || TEL_CONN_MGR_RET_WOULDBLOCK == ret)
    {
        tel_conn_mgr_get_cid_by_app_id(&cid, app_id);

        bearer_info_idx = tel_conn_mgr_convt_cid_to_bearer_info_idx(cid);

        if (!apb_nw_app_set_cid_app_id(bearer_info_idx, cid, app_id))
        {
            configASSERT(0);
        }

        NW_LOG("app_id:%d, cid:%d",cid, app_id);
        param->cid = cid;
        param->pdp_type = activated_pdp_type;
    }

    return ret;
}


static void apb_nw_app_task_main(void *param)
{
    tel_conn_mgr_msg_struct *msg = NULL;

    while (egact_cntx)
    {
        if (xQueueReceive(egact_cntx->queue_hdl, &msg, portMAX_DELAY) == pdPASS)
        {
            switch (msg->msg_id)
            {
                case MSG_ID_TEL_CONN_MGR_ACTIVATION_RSP:
                {
                    /* Activation result. */
                    tel_conn_mgr_activation_rsp_struct *act_msg = (tel_conn_mgr_activation_rsp_struct *)msg;
                    unsigned int i = 0;
                    int cid = 0;

                    for (i = 0; act_msg->app_id[i] && TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM > i; i++)
                    {
                        cid = apb_nw_app_get_cid(act_msg->app_id[i], false);
                        apb_nw_app_send_result_urc(1, act_msg->result, cid, act_msg->pdp_type);
                        if (!act_msg->result)
                        {
                            //apb_nw_app_reset_cid_app_id_by_app_id(act_msg->app_id[i]);
                            apb_nw_app_set_is_used_by_app_id(false, act_msg->app_id[i]);
                        }
                    }

                    break;
                }

                case MSG_ID_TEL_CONN_MGR_DEACTIVATION_RSP:
                {
                    /* Deactivation result. */
                    int cid = 0;
                    tel_conn_mgr_deactivation_rsp_struct *deact_msg = (tel_conn_mgr_deactivation_rsp_struct *)msg;

                    cid = apb_nw_app_get_cid(deact_msg->app_id, false);
                    apb_nw_app_send_result_urc(0, deact_msg->result, cid, 0);
                    if (deact_msg->result)
                    {
                        //apb_nw_app_reset_cid_app_id_by_app_id(deact_msg->app_id);
                        apb_nw_app_set_is_used_by_app_id(false, deact_msg->app_id);
                    }
                    break;
                }

                case MSG_ID_TEL_CONN_MGR_DEACTIVATION_IND:
                {
                    /* Passive deactivation. */
                    tel_conn_mgr_deactivation_ind_struct *deact_ind_msg = (tel_conn_mgr_deactivation_ind_struct *)msg;
                    unsigned int i = 0;
                    int cid = 0;

                    for (i = 0; deact_ind_msg->app_id[i] && TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM > i; i++)
                    {
                        cid = apb_nw_app_get_cid(deact_ind_msg->app_id[0], false);
                        apb_nw_app_send_result_urc(2, 0, cid, 0);
                        //apb_nw_app_reset_cid_app_id_by_app_id(deact_ind_msg->app_id[i]);
                        apb_nw_app_set_is_used_by_app_id(false, deact_ind_msg->app_id[i]);
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


tel_conn_mgr_ret_enum apb_nw_app_create(void)
{
    if (!egact_cntx)
    {
        egact_cntx = pvPortCalloc(1, sizeof(apb_proxy_nw_egact_cntx_struct));
        if (!egact_cntx)
        {
            return TEL_CONN_MGR_RET_NOT_ENOUGH_MEMORY;
        }

        egact_cntx->queue_hdl = xQueueCreate(APB_NW_APP_QUEUE_MAX_SIZE, sizeof(tel_conn_mgr_msg_struct *));

        xTaskCreate(apb_nw_app_task_main,
                    APB_NW_APP_TASK_NAME,
                    APB_NW_APP_TASK_STACKSIZE / sizeof(portSTACK_TYPE),
                    NULL,
                    APB_NW_APP_TASK_PRIO,
                    NULL);

    }

    return TEL_CONN_MGR_RET_OK;
}


tel_conn_mgr_ret_enum apb_proxy_nw_bearer_act_param_parse(char *at_cmd_str, apb_proxy_nw_bearer_act_param_struct *bearer_act_param)
{
    char *param = NULL;
    int count = 0, at_cmd_str_len = 0, param_len = 0;
    char *param_str_tmp = NULL;

    if (!at_cmd_str || !bearer_act_param)
    {
        NW_LOG("Invalid parameter. at_cmd_str:%x, bearer_act_param:%x",
               (unsigned int)at_cmd_str, (unsigned int)bearer_act_param);
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    at_cmd_str_len = strlen(at_cmd_str);

    param = strtok(at_cmd_str, (const char *)",");
    if (!param)
    {
        NW_LOG("Invalid parameter. param:%s", param);
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    while (param)
    {
        count++;
        switch (count)
        {
            case 1:
            {
                bearer_act_param->op = atoi(param);
                break;
            }
            case 2:
            {
                if (1 == bearer_act_param->op)
                {
                    bearer_act_param->pdp_type = (tel_conn_mgr_pdp_type_enum)atoi(param);
                    break;
                }
                else if (0 == bearer_act_param->op)
                {
                    bearer_act_param->cid = atoi(param);
                    return TEL_CONN_MGR_RET_OK;
                }
                else
                {
                    NW_LOG("Invalid op:%d", bearer_act_param->op);
                    return TEL_CONN_MGR_RET_ERROR;
                }
            }
            case 3:
            {
                param_len = strlen(param);
                if (TEL_CONN_MGR_APN_MAX_LEN < param_len)
                {
                    NW_LOG("Invalid format");
                    return TEL_CONN_MGR_RET_ERROR;
                }
                strncpy(bearer_act_param->apn, param, param_len);
                bearer_act_param->apn[param_len] = '\0';
                break;
            }
            case 4:
            {
                param_len = strlen(param);
                if (TEL_CONN_MGR_USERNAME_MAX_LEN < param_len)
                {
                    NW_LOG("Invalid format");
                    return TEL_CONN_MGR_RET_ERROR;
                }
                strncpy(bearer_act_param->username, param, param_len);
                bearer_act_param->username[param_len] = '\0';
                break;
            }
            case 5:
            {
                param_len = strlen(param);
                if (TEL_CONN_MGR_PASSWORD_MAX_LEN < param_len)
                {
                    NW_LOG("Invalid format");
                    return TEL_CONN_MGR_RET_ERROR;
                }
                strncpy(bearer_act_param->password, param, param_len);
                bearer_act_param->password[param_len] = '\0';
                break;
            }
            case 6:
            {
                bearer_act_param->bearer_type = (tel_conn_mgr_bearer_type_enum)atoi(param);
                if (TEL_CONN_MGR_BEARER_TYPE_NONE >= bearer_act_param->bearer_type ||
                    TEL_CONN_MGR_BEARER_TYPE_MAX <= bearer_act_param->bearer_type)
                {
                    NW_LOG("Invalid bearer_type:%d", bearer_act_param->bearer_type);
                    return TEL_CONN_MGR_RET_ERROR;
                }
                break;
            }
            case 7:
            {
                bearer_act_param->sim_id = (tel_conn_mgr_sim_id_enum)atoi(param);
                if (TEL_CONN_MGR_SIM_ID_NONE >= bearer_act_param->sim_id||
                    TEL_CONN_MGR_SIM_ID_MAX <= bearer_act_param->sim_id)
                {
                    NW_LOG("Invalid sim_id:%d", bearer_act_param->sim_id);
                    return TEL_CONN_MGR_RET_ERROR;
                }
                return TEL_CONN_MGR_RET_OK;
            }
            default:
            {
                goto exit;
            }
        }

        if (2 == count || 3 == count || 4 == count)
        {
            if (2 == count)
            {
                param_str_tmp = param + strlen(param) + 1;
                if (param_str_tmp > at_cmd_str + at_cmd_str_len ||
                    *param_str_tmp != '"')
                {
                    NW_LOG("Invalid format");
                    return TEL_CONN_MGR_RET_ERROR;
                }
            }
            else if (!param_str_tmp)
            {
                break;
            }

            param = apb_proxy_nw_parse_string_param(&param_str_tmp, at_cmd_str + at_cmd_str_len - 1);
            if (!param)
            {
                NW_LOG("Invalid format");
                return TEL_CONN_MGR_RET_ERROR;
            }
        }
        else if (5 == count)
        {
            if (param_str_tmp && *param_str_tmp != '\0')
            {
                param = strtok(param_str_tmp, (const char *)",");
            }
            else
            {
                param = NULL;
            }
        }
        else
        {
            param = strtok(NULL, (const char *)",");
        }
    }

exit:
    if (0 == bearer_act_param->op || 3 > count)
    {
        NW_LOG("Invalid param:%s, count:%d", param ? param : "", count);
        return TEL_CONN_MGR_RET_ERROR;
    }

    if (6 > count)
    {
        bearer_act_param->bearer_type = TEL_CONN_MGR_BEARER_TYPE_NBIOT;
    }

    if (7 > count)
    {
        bearer_act_param->sim_id = TEL_CONN_MGR_SIM_ID_1;
    }

    return TEL_CONN_MGR_RET_OK;
}
#endif /* TEL_CONN_MGR_APB_SUPPORT */


apb_proxy_status_t apb_proxy_hdlr_egact_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
#ifdef TEL_CONN_MGR_APB_SUPPORT
    char result_str[20] = {0};
#endif

    NW_LOG("%s", __FUNCTION__);
    configASSERT(p_parse_cmd != NULL);

    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
#ifdef TEL_CONN_MGR_APB_SUPPORT
    switch (p_parse_cmd->mode)
    {
        case APB_PROXY_CMD_MODE_READ:   // rec: AT+EGACT?
        {
            NW_LOG("Read mode is not supported.\n");
            break;
        }

        case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+EGACT=?
        {
            NW_LOG("TEST mode is not supported.\n");
            break;
        }

        case APB_PROXY_CMD_MODE_ACTIVE: // rec: AT+EGACT
        {
            NW_LOG("ACTIVE mode is not supported.\n");
            break;
        }

        case APB_PROXY_CMD_MODE_INVALID:
        {
            NW_LOG("INVALID mode is not supported.\n");
            break;
        }

        case APB_PROXY_CMD_MODE_EXECUTION: // rec: AT+EGACT=<op>...  the handler need to parse the parameters
        {
            tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_FALSE;
            apb_proxy_nw_bearer_act_param_struct bearer_act_param = {0};
            char *param = NULL;

            param = p_parse_cmd->string_ptr + p_parse_cmd->name_len + 1;
            NW_LOG("param to parse: %s\r\n", param);
            ret = apb_proxy_nw_bearer_act_param_parse(param, &bearer_act_param);
            NW_LOG("%d\r\n", ret);

            if (TEL_CONN_MGR_RET_OK == ret)
            {
                if (1 == bearer_act_param.op)
                {
                    /* Activation */
                    NW_LOG("Activation");
                    ret = apb_nw_app_create();
                    if (TEL_CONN_MGR_RET_OK == ret)
                    {
                        ret = apb_nw_app_activate(&bearer_act_param);
                    }
                }
                else
                {
                    unsigned int app_id = 0;
                    /* Deactivation */
                    NW_LOG("Deactivation");

                    app_id = apb_nw_app_get_app_id(bearer_act_param.cid);

                    if (app_id)
                    {
                        ret = tel_conn_mgr_deactivate(app_id);
                    }
                    else
                    {
                        ret = TEL_CONN_MGR_RET_NOT_FOUND;
                    }

                    if (TEL_CONN_MGR_RET_OK == ret || TEL_CONN_MGR_RET_IS_HOLD == ret)
                    {
                        if (TEL_CONN_MGR_RET_IS_HOLD == ret)
                        {
                            ret = TEL_CONN_MGR_RET_OK;
                        }

                        apb_nw_app_set_is_used_by_app_id(false, app_id);
                    }
                }

                NW_LOG("act/deact ret:%d", ret);
                if (TEL_CONN_MGR_RET_OK == ret)
                {
                    /* Should not happen if using AT CMD only. */
                    cmd_result.result_code = APB_PROXY_RESULT_OK;
                    if (1 == bearer_act_param.op)
                    {
                        /* Activation */
                        sprintf(result_str, "+EGACT:%d,%d,1,%d", bearer_act_param.cid, bearer_act_param.op, bearer_act_param.pdp_type);
                    }
                    else
                    {
                        /* Deactivation */
                        sprintf(result_str, "+EGACT:%d,%d,1", bearer_act_param.cid, bearer_act_param.op);
                    }
                    cmd_result.pdata = result_str;
                }
                else if (TEL_CONN_MGR_RET_WOULDBLOCK == ret)
                {
                    cmd_result.result_code = APB_PROXY_RESULT_OK;
                    sprintf(result_str, "+EGACT:%d", bearer_act_param.cid);
                    cmd_result.pdata = result_str;
                }
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
        cmd_result.length = strlen(cmd_result.pdata);
        NW_LOG("EGACT pdata:%s", cmd_result.pdata);
    }
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    NW_LOG("EGACT result_code:%d", cmd_result.result_code);
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}


apb_proxy_status_t apb_proxy_hdlr_tcmtest_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};

#ifdef TEL_CONN_MGR_APB_SUPPORT
    int test_case_num = 0;
    char test_case_num_str[5] = {0}, *start = NULL, *end = NULL;
#endif

    NW_LOG("%s", __FUNCTION__);
    configASSERT(p_parse_cmd != NULL);
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

#ifdef TEL_CONN_MGR_APB_SUPPORT
    switch (p_parse_cmd->mode)
    {
        case APB_PROXY_CMD_MODE_READ:   // rec: AT+TCMTEST?
        {
            NW_LOG("Read mode is not supported.\n");
            break;
        }

        case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+TCMTEST=?
        {
            NW_LOG("TEST mode is not supported.\n");
            break;
        }

        case APB_PROXY_CMD_MODE_ACTIVE: // rec: AT+TCMTEST
        {
            NW_LOG("ACTIVE mode is not supported.\n");
            break;
        }

        case APB_PROXY_CMD_MODE_INVALID:
        {
            NW_LOG("INVALID mode is not supported.\n");
            break;
        }

        case APB_PROXY_CMD_MODE_EXECUTION:
        {
            start = strchr(p_parse_cmd->string_ptr, '=');
            if (!start)
            {
                NW_LOG("wrong at cmd format.");
                break;
            }
            start++;
            end = strchr(p_parse_cmd->string_ptr, ',');
            if (!end)
            {
                end = p_parse_cmd->string_ptr + strlen(p_parse_cmd->string_ptr) - 1;
            }
            strncpy(test_case_num_str, start, end - start + 1 > 4 ? 4 : end - start + 1);
            test_case_num = atoi(test_case_num_str);

            NW_LOG("num_str:%s, num:%d", test_case_num_str, test_case_num);

            if (1 <= test_case_num && 7 >= test_case_num)
            {
                tel_conn_mgr_ut_set_apb_cmd_id(p_parse_cmd->cmd_id);

                if (!tel_conn_mgr_ut_is_test_ready())
                {
                    tel_conn_mgr_ut_init(2, test_case_num);
                }
                else
                {
                    tel_conn_mgr_ut_test_case_run(test_case_num);
                }
                cmd_result.result_code = APB_PROXY_RESULT_OK;
            }
            else
            {
                NW_LOG("Invalid test case num");
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
        cmd_result.length = strlen(cmd_result.pdata);
    }
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}


apb_proxy_status_t apb_proxy_hdlr_niddtest_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};
#ifdef TEL_CONN_MGR_APB_SUPPORT
    int test_case_num = 0;
    char test_case_num_str[5] = {0}, *start = NULL, *end = NULL;

    configASSERT(p_parse_cmd != NULL);
    cmd_result.pdata = "Unknown";

    NW_LOG("%s", __FUNCTION__);

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
            NW_LOG("at cmd received: AT  [%s]", p_parse_cmd->string_ptr);
            start = strchr(p_parse_cmd->string_ptr, '=');
            if (!start)
            {
                break;
            }
            start++;
            end = strchr(p_parse_cmd->string_ptr, ',');
            if (!end)
            {
                end = p_parse_cmd->string_ptr + strlen(p_parse_cmd->string_ptr) - 1;
            }
            strncpy(test_case_num_str, start, end - start + 1);
            test_case_num = atoi(test_case_num_str);

            NW_LOG("num_str:%s, num:%d", test_case_num_str, test_case_num);

            #if 0
            if (test_case_num == 3)
            {
                nidd_bearer_info_struct bearer_info;
                strncpy(bearer_info.apn, "APN", strlen("APN"));
                bearer_info.apn[strlen("APN")] = '\0';
                bearer_info.channel_id = 2;
                bearer_info.is_activated = 1;
                nidd_bearer_info_ind(&bearer_info);
                cmd_result.result_code = APB_PROXY_RESULT_OK;

                cmd_result.length = 0;
                cmd_result.pdata = NULL;
                cmd_result.cmd_id = p_parse_cmd->cmd_id;
                apb_proxy_send_at_cmd_result(&cmd_result);
                return;
            }
            #endif

            if (1 <= test_case_num && 2 >= test_case_num)
            {
                char* param2_val = NULL;
                int param2_len = 0;
                char* param3_val = NULL;
                NW_LOG("Run test case num: %d\r\n", test_case_num);

                param2_len = strlen(p_parse_cmd->string_ptr) - strlen((char*)"AT+NIDD=1,");

                if (param2_len < 1){
                    NW_LOG("parameter error");
                    cmd_result.pdata = "Invalid test case num";
                } else {
                    param2_val = p_parse_cmd->string_ptr + (strlen((char*)"AT+NIDD=1,"));
                    param3_val = strchr((char *)param2_val, ',');

                    if (param3_val == NULL) {
                        NW_LOG("apn: %s", param2_val);

                        if (1 == test_case_num) {
                            nidd_ut_app_activate(param2_val);
                            cmd_result.result_code = APB_PROXY_RESULT_OK;
                        } else {
                            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                        }
                    } else {
                        char apn[32];
                        NW_LOG("apn length: %d", param3_val - param2_val);
                        if (param3_val - param2_val > 32) {
                            NW_LOG("parameter error, APN name too long");
                            cmd_result.pdata = "APN name too long";
                            goto exit;
                        } else if (param3_val - param2_val < 0) {
                            NW_LOG("parameter error");
                            cmd_result.pdata = "error";
                            goto exit;
                        }

                        strncpy(apn, param2_val, param3_val - param2_val);
                        apn[param3_val - param2_val] = '\0';
                        NW_LOG("apn: %s", apn);
                        NW_LOG("data length: %d", strlen(param3_val + 1));
                        NW_LOG("data: %s", param3_val + 1);

                        if (2 == test_case_num) {
                            nidd_ut_app_send_data(apn, param3_val + 1, strlen(param3_val + 1));
                            cmd_result.result_code = APB_PROXY_RESULT_OK;
                        } else {
                            cmd_result.result_code = APB_PROXY_RESULT_ERROR;
                        }
                    }

                    cmd_result.length = 0;
                    cmd_result.pdata = NULL;
                    cmd_result.cmd_id = p_parse_cmd->cmd_id;
                    apb_proxy_send_at_cmd_result(&cmd_result);
                    return APB_PROXY_STATUS_OK;
                }
            }
            else
            {
                cmd_result.pdata = "Invalid test case num";
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
#else
    cmd_result.pdata = "Not enabled";
#endif
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
    cmd_result.length = strlen(cmd_result.pdata) + 1;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    NW_LOG("tcmtest failed. cause:%s\r\n", cmd_result.pdata ? (char *)cmd_result.pdata : (char *)"");
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}

