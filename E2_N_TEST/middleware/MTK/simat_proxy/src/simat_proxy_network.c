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

// std io string part
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
// freertos part
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
//#include "syslog.h"

// lwip part
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/inet.h"
#include "lwip/ip_addr.h"
#include "lwip/api.h"
#include "nbnetif.h"

// bearer part
#include "tel_conn_mgr_app_api.h"
// cmux part
#include "mux_ap.h"
// simat proxt internal part
#include "simat_proxy_common.h"

////////////////////////////////////////////////////////////////////////////////////////////
// define internal macro for simat proxy test
#ifndef SIMAT_PROXY_INTERNAL_UT_TEST
//#define SIMAT_PROXY_INTERNAL_UT_TEST
#endif /* SIMAT_PROXY_INTERNAL_UT_TEST */

#define SIMAT_PROXY_LWIP_SEND_INTERNAL_MSG                  (100)
#define SIMAT_PROXY_LWIP_RECEIVE_INTERNAL_MSG               (101)
#define SIMAT_PROXY_LWIP_ERROR_INTERNAL_MSG                 (102)

#define SIMAT_PROXY_MODEM_TASK_QUEUE_SIZE                   (20)
#define SIMAT_PROXY_MODEM_TASK_NAME                         "SIMPM"
#define SIMAT_PROXY_MODEM_TASK_STACK_SIZE                   (1024 * 4)
#define SIMAT_PROXY_MODEM_TASK_PRIORITY                     (TASK_PRIORITY_NORMAL)

#define SIMAT_MAXIMUM_SIZE                                  (1024)

#define MAX_SOCKET_MEMORY_BLOCK_NUM                         (10)
#define MAX_SIMAT_SOCKET_NUM                                (7)
#define MAX_IP_ADDRESS_VAL_LEN                              (40)

#define MAXIMUM_PACKET_NUM                                  (10)

#define IPV4_SOCKET_INFO_FROM_MD_LEN                        (9)
#define IPV6_SOCKET_INFO_FROM_MD_LEN                        (21)
#define IPV4_SOCKET_LEN                                     (4)
#define IPV6_SOCKET_LEN                                     (MAX_IP_ADDRESS_SIZE)

#define PDP_TYPE_IPV4                                       (1)
#define PDP_TYPE_IPV6                                       (2)
#define PDP_TYPE_IPv4V6                                     (3)

/*  |---8(memory_block_index)---|---8(socket_management_index)---|--16(msg_id)--| */
#define GET_SIMAT_MSG_ID_FROM_CMUX(vargs)                   ((vargs) & 0xffff)
#define GET_SIMAT_SOCK_MANAGEMENT_INDEX_FROM_CMUX(vargs)    (((vargs) >> 16) & 0xff)
#define GET_SIMAT_SOCK_MEMORY_BLOCK_INDEX_FROM_CMUX(vargs)  (((vargs) >> 24) & 0xff)

#define SET_SIMAT_MSG_ID_FROM_CMUX(msg_id)                  (msg_id)
#define SET_SIMAT_SOCK_MANAGEMENT_FROM_CMUX(msg_id, sock_index, bk_index)	\
        ( (msg_id) + (((sock_index) << (16)) & 0xff0000) + (((bk_index) << (24)) & 0xff000000) )


/* for memory reference count & debug infomation */
/*  |-2(flag dynamic 10)-|-13(magic number:1080)-|-1(reserved)-|--16(msg_id)--|-16(reference count)-|-16(size)-| */
/*  |-2(flag static: 01)---|-13(magic numbe:0f7f)--|-1(in_use)--|--16(msg_id)--| */
#define DYNAMIC_HEAP                                        (0X03)
#define STATIC_HEAP                                         (0x01)
#define DYNAMIC_MAGIC_NUM                                   (0x1080)
#define STATIC_MAGIC_NUM                                    (0x0f7f)

#define SET_MEM_ALLOCATE_SOURCE(type, magic_num)            (((type) << 14) + ((magic_num) << 1))
#define GET_MEM_ALLOCATE_SOURCE(type)                       (((type) >> 14) & 0x3)
#define GET_MEM_ALLOCATE_MGAIC_NUM(var)                     (((var) >> 1) & 0x1fff)


// for static allocate
#define SET_SIMAT_PROXY_STATIC_MEM_IN_USE(var)              ((var) | 0x01)
#define SET_SIMAT_PROXY_STATIC_MEM_FREE(var)                ((var) & 0xfffe)
#define GET_SIMAT_PROXY_STATIC_MEM_STATUS(var)              ((var) & 0x01)
#define SET_SIMAT_PROXY_STATIC_MEM(var, message_id, in_use) \
    (var)->msg_id = message_id; \
    if(1 == in_use) { \
        (var)->type = SET_SIMAT_PROXY_STATIC_MEM_IN_USE(SET_MEM_ALLOCATE_SOURCE(STATIC_HEAP, STATIC_MAGIC_NUM));    \
    } else { \
        (var)->type = SET_MEM_ALLOCATE_SOURCE(STATIC_HEAP, STATIC_MAGIC_NUM);   \
    }

//#define SOCKET_REQUEST_OPEN_MAX_NUM                         (MAX_SIMAT_SOCKET_NUM)
//#define CMD_REJECT_MAX_NUM                                  (MAX_SIMAT_SOCKET_NUM)
//#define CONFIG_REQ_MAX_NUM                                  (1)
#define CONFIG_CNF_MAX_NUM                                  (4) // for four type msg, each type has only one msg
#define BEAR_REJECT_RSP_MAX_NUM                             (MAX_SIMAT_SOCKET_NUM + 2)
#define SOCKET_BASE_MAX_NUM                                 (MAX_SIMAT_SOCKET_NUM * 4) // for four type msg, each type has only MAX_SIMAT_SOCKET_NUM msg
#define SOCKET_CLOSE_MAX_NUM                                (MAX_SIMAT_SOCKET_NUM * 2) // for two type msg, each type has only MAX_SIMAT_SOCKET_NUM msg


//////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum {
    SOCKET_INIT = 0,
    SOCKET_CLOSE,
    SOCKET_CREATE,
    SOCKET_CONNECT
} SOCKET_STATUS;

typedef enum {
    AP_TO_MD = 0,
    MD_TO_AP,
} simat_direct_t;

typedef enum {
    real_to_request = 0,
    request_to_real,
} simat_ip_mapping_t;

typedef enum {
    real_type = 0,
    request_type,
} simat_socket_internal_t;

typedef enum {
    TCP_TYPE = 0,
    UDP_TYPE,
    RAW_TYPE
} SOCKET_TYPE;

typedef enum {
    SIMAT_PROXY_SUCCESS = 0,
    SIMAT_PROXY_FAILURE,
} simat_ret_t;

typedef enum {
    SIMAT_CMUX_UNITIALIAZED = 0,
    SIMAT_CMUX_CONNECT,
    SIMAT_CMUX_DISCONNECT
} simat_cumx_status_t;

typedef enum {
    AP_TO_MD_INIT = 0, /* when init create socket using */
    AP_TO_MD_SENDING = 1,
    AP_TO_MD_CONFIRM,
    AP_TO_MD_ERROR
} simat_ap_to_md_send_status_t;

typedef enum {
    MD_TO_AP_INIT = 0, /* when init create socket using */
    MD_TO_AP_SENDING = 1,
    MD_TO_AP_CONFIRM
} simat_md_to_ap_send_status_t;


///////////////////////////////////////////////////////////////////////////////////////////

typedef struct simat_proxy_modem_message_struct {
    unsigned int message_id;
    void *param;
} simat_proxy_modem_message_struct;


typedef void (*simat_proxy_hadler)(SIMAT_Base_Header *msg);

typedef struct simat_process_handler {
    unsigned int msg_name;
    simat_proxy_hadler msg_handler;
    const char *msg_string;
} simat_process_handler;

typedef struct memory_block {
    unsigned char *base_address;    /* reduce memory allocate times and cmux free pointer */
    unsigned char *start_address;   /* recv and send data pointer, it's base_address add offset */
    unsigned short total_size;      /* total size is only lwip data and can't offset for base_address */
    unsigned short send_size;       /* send size for lwip side also base on start_address */
} memory_block;


typedef struct socket_management_node {
    unsigned int status;
    unsigned int socket_type;
    unsigned int request_socket_id;
    struct netconn *create_conn;
    simat_ap_to_md_send_status_t recving_status;            //  AP -> MD
    simat_md_to_ap_send_status_t sending_status;            //  MD -> AP
    unsigned int incoming_recving_flag;                     // if internet recv buffer to AP, wait for previous recv complete then recv this packet
    unsigned int left_recving_flag;                         // if this recv has more than mux size, need split
    struct netbuf *left_recving_netbuf;                     // if left_recving_flag has set, left_recving_netbuf need save netbuf data pointer
    memory_block send_buffer[MAX_SOCKET_MEMORY_BLOCK_NUM];  // MD -> AP
    memory_block recv_buffer[MAX_SOCKET_MEMORY_BLOCK_NUM];  // AP -> MD    
} socket_management_node;


typedef struct simat_proxy_base_node {
    unsigned short type;                    /* flag + magic_num + use*/
    unsigned short msg_id;
} simat_proxy_base_node;

typedef struct simat_proxy_dynamic_node {
    simat_proxy_base_node   base_info;
    unsigned short          ref_count;
    unsigned short          size;           /*only for data size*/
} simat_proxy_dynamic_node;

typedef struct simat_open_req_mem {
    simat_proxy_base_node base_info;
    unsigned char data[sizeof(SIMAT_SOC_Open_Req)];
} simat_open_req_mem;

typedef struct simat_reject_mem {
    simat_proxy_base_node base_info;
    unsigned char data[sizeof(SIMAT_Cmd_Reject)];
} simat_reject_mem;

typedef struct simat_reject_rsp_mem {
    simat_proxy_base_node base_info;
    unsigned char data[sizeof(SIMAT_Reject_Rsp)];
} simat_reject_rsp_mem, simat_conf_reject_mem, simat_bring_down_ind_mem;


typedef struct simat_config_req_mem {
    simat_proxy_base_node base_info;
    unsigned char data[sizeof(SIMAT_Config_Req)];
} simat_config_req_mem;

typedef struct simat_config_cnf_mem {
    simat_proxy_base_node base_info;
    unsigned char data[sizeof(SIMAT_Config_Conf)];
} simat_config_cnf_mem, simat_bring_up_req_mem,
simat_bring_up_conf_mem, simat_bring_down_req_mem;


typedef struct simat_soc_base_mem {
    simat_proxy_base_node base_info;
    unsigned char data[sizeof(SIMAT_Soc_Base)];
} simat_soc_base_mem, simat_soc_open_conf_mem, simat_soc_close_req_mem,
simat_send_data_conf_mem, simat_recv_data_rsp_mem;


typedef struct simat_soc_close_ind_mem {
    simat_proxy_base_node base_info;
    unsigned char data[sizeof(SIMAT_Close_Ind)];
} simat_soc_close_ind_mem, simat_soc_send_rej_mem;


typedef struct simat_proxy_static_mem {
    //simat_open_req_mem          open_req_mem[SOCKET_REQUEST_OPEN_MAX_NUM];
    //simat_reject_mem            reject_mem[CMD_REJECT_MAX_NUM];
    //simat_config_req_mem        config_req_mem[CONFIG_REQ_MAX_NUM];
    simat_config_cnf_mem        config_cnf_mem[CONFIG_CNF_MAX_NUM];
    simat_reject_rsp_mem        reject_rsp_mem[BEAR_REJECT_RSP_MAX_NUM];
    simat_soc_base_mem          soc_base_mem[SOCKET_BASE_MAX_NUM];
    simat_soc_close_ind_mem     soc_close_ind_mem[SOCKET_CLOSE_MAX_NUM];
} simat_proxy_static_mem;


typedef struct simat_cmux {
    simat_cumx_status_t simat_cmux_status;
    unsigned int channel_id;
} simat_cmux;


typedef struct {
    unsigned char   netwotk_type;
    unsigned char   port_protocol;
    unsigned short  port_number;
    unsigned char   addr[MAX_IP_ADDRESS_SIZE];        //ip_addr_t addr;
} simat_remote_socket_info;

typedef struct {
    unsigned char   apn_val[MAX_APN_SIZE];
    unsigned char   user_val[MAX_USERNAME_SIZE];
    unsigned char   passwd_val[MAX_PASSWD_SIZE];
    unsigned char   pdp_val;
    unsigned int    valid_flag;
    unsigned int    app_id;
} simat_bearer_info;

typedef struct socket_management {
    simat_bearer_info   bearer_info;
    socket_management_node  management_node[MAX_SIMAT_SOCKET_NUM];
    simat_proxy_static_mem  static_mem;
} socket_management;

typedef struct LWIP_recv_data {
    unsigned char   *p_data;
    unsigned int    size;
    unsigned int    in_use;
} LWIP_recv_data;

/////////////////////////////////////////////////////////////////////////////////////////

unsigned char *simat_proxy_allocate(unsigned int size, unsigned int msg_id, int file_line);
unsigned char *simat_proxy_allocate_internal(unsigned int size, unsigned int msg_id, int32_t file_line);
unsigned char *simat_proxy_allocate_simple_internal(unsigned int size, int32_t file_line);
void simat_proxy_free(unsigned char *buffer_address, int32_t file_line);

#define SIMAT_PROXY_ALLOCATE(size, msg_id) simat_proxy_allocate(size, msg_id, __LINE__)
#define SIMAT_PROXY_SIMLPE_ALLOCATE(size)  simat_proxy_allocate_simple_internal(size, __LINE__)
#define SIMAT_PROXY_FREE(address)          simat_proxy_free(address, __LINE__)
#define SIMAT_PROXY_SIMLPE_FREE(size)      simat_proxy_free_simple_internal(size, __LINE__)



void simat_proxy_socket_open_request_handler(SIMAT_Base_Header *msg);
void simat_proxy_socket_open_confirm_handler(SIMAT_Base_Header *msg);
void simat_proxy_socket_close_ind_handler(SIMAT_Base_Header *msg);
void simat_proxy_cmd_reject_handler(SIMAT_Base_Header *msg);
void simat_proxy_socket_send_confirm_handler(SIMAT_Base_Header *msg);
void simat_proxy_socket_send_request_handler(SIMAT_Base_Header *msg);
void simat_proxy_socket_send_reject_handler(SIMAT_Base_Header *msg);
void simat_proxy_socket_recv_ind_handler(SIMAT_Base_Header *msg);
void simat_proxy_socket_recv_confirm_handler(SIMAT_Base_Header *msg);
void simat_proxy_socket_close_request_handler(SIMAT_Base_Header *msg);
void simat_proxy_bearer_open_request_handler(SIMAT_Base_Header *msg);
void simat_proxy_bearer_open_confirm_handler(SIMAT_Base_Header *msg);
void simat_proxy_bearer_open_reject_handler(SIMAT_Base_Header *msg);
void simat_proxy_bring_up_request_handler(SIMAT_Base_Header *msg);
void simat_proxy_bring_up_confirm_handler(SIMAT_Base_Header *msg);
void simat_proxy_bring_down_request_handler(SIMAT_Base_Header *msg);
void simat_proxy_bring_down_ind_handler(SIMAT_Base_Header *msg);
void simat_proxy_bearer_msg_process(simat_proxy_modem_message_struct *msg);

void process_connect_result(int request_socket_id, unsigned char reason, int line);
void process_send_to_lwip_success(int request_socket);
void process_send_to_lwip_failure(int request_socket, unsigned char cause);
void process_send_to_recv_ind(SIMAT_Data_Req *simat_data_request);
void process_left_socket_sending_data(socket_management_node *simat_soc_info);
void process_socket_recving_data(socket_management_node *simat_soc_info);
void process_socket_left_recving_data(socket_management_node *simat_soc_info);
void process_socket_close(unsigned int request_socket, unsigned char close_reason);
void process_bearer_config_reject(unsigned char bearer_reject_reason);
void process_bearer_confirm(unsigned int bearer_msg_type);
void process_bearer_bring_down(unsigned char bearer_bring_down_reason);


void simat_proxy_lwip_callback(struct netconn *, enum netconn_evt, u16_t len);

void simat_send_event_internal(unsigned int msg_id, SIMAT_Base_Header *param);
void simat_proxy_lwip_callback_internal(simat_proxy_modem_message_struct *msg);

#define PROCESS_CONNECT_RESULT(sock_id, reason) process_connect_result(sock_id, reason, __LINE__);


#ifdef SIMAT_PROXY_INTERNAL_UT_TEST
void simat_ut_send_data_req(uint32_t sock_id);
void simat_ut_recv_data_rsp(uint32_t sock_id);
void simat_ut_close_rsp(uint32_t sock_id);
void simat_ut_bearer_close(uint32_t sock_id);
#endif /* SIMAT_PROXY_INTERNAL_UT_TEST */
////////////////////////////////////////////////////////////////////////////////////////

QueueHandle_t simat_proxy_event_queue;
QueueHandle_t simat_proxy_others_event_queue;


socket_management g_socket_management = {0};

simat_cmux g_simat_cmux_status = {0};



nb_netif_status_t net_status = NB_NETWORK_AVAILABLE;
int g_socket_id[MAX_SIMAT_SOCKET_NUM];


static const simat_process_handler simat_proxy_process_table[] = {
    {   SIMAT_CODE_START,          (simat_proxy_hadler)NULL,                    "SIMAT_CODE_START"},
    {   SIMAT_CODE_REJ,             simat_proxy_cmd_reject_handler,             "PROXY_TO_SIMAT_CODE_REJ"},
    {   SIMAT_CONFIG_REQ,           simat_proxy_bearer_open_request_handler,    "SIMAT_TO_PROXY_BEAR_CONFIG_REQ"},
    {   SIMAT_CONFIG_CNF,           simat_proxy_bearer_open_confirm_handler,    "PROXY_TO_SIMAT_BEAR_CONFIG_CNF"},
    {   SIMAT_CONFIG_REJ,           simat_proxy_bearer_open_reject_handler,     "PROXY_TO_SIMAT_CONFIG_REJ"},
    {   SIMAT_BRING_UP_REQ,         simat_proxy_bring_up_request_handler,       "SIMAT_TO_PROXY_BRING_UP_REQ"},
    {   SIMAT_BRING_UP_CNF,         simat_proxy_bring_up_confirm_handler,       "PROXY_TO_SIMAT_BRING_UP_CNF"},
    {   SIMAT_BRING_DOWN_REQ,       simat_proxy_bring_down_request_handler,     "SIMAT_TO_PROXY_BRING_DOWN_REQ"},
    {   SIMAT_BRING_DOWN_IND,       simat_proxy_bring_down_ind_handler,         "PROXY_TO_SIMAT_BRING_DOWN_IND"},
    {   SIMAT_SOCKET_OPEN_REQ,      simat_proxy_socket_open_request_handler,    "SIMAT_TO_PROXY_SOCKET_OPEN_REQ"},
    {   SIMAT_SOCKET_OPEN_CNF,      simat_proxy_socket_open_confirm_handler,    "PROXY_TO_SIMAT_SOCKET_OPEN_CNF"},
    {   SIMAT_SOCKET_CLOSE_REQ,     simat_proxy_socket_close_request_handler,   "SIMAT_TO_PROXY_SOCKET_CLOSE_REQ"},
    {   SIMAT_SOCKET_CLOSE_IND,     simat_proxy_socket_close_ind_handler,       "PROXY_TO_SIMAT_SOCKET_CLOSE_IND"},
    {   SIMAT_SOCKET_SEND_REQ,      simat_proxy_socket_send_request_handler,    "SIMAT_TO_PROXY_SOCKET_SEND_REQ"},
    {   SIMAT_SOCKET_SEND_CNF,      simat_proxy_socket_send_confirm_handler,    "PROXY_TO_SIMAT_SOCKET_SEND_CNF"},
    {   SIMAT_SOCKET_SEND_REJ,      simat_proxy_socket_send_reject_handler,     "PROXY_TO_SIMAT_SOCKET_SEND_REJ"},
    {   SIMAT_SOCKET_RECV_IND,      simat_proxy_socket_recv_ind_handler,        "PROXY_TO_SIMAT_SOCKET_RECV_IND"},
    {   SIMAT_SOCKET_RECV_RSP,      simat_proxy_socket_recv_confirm_handler,    "SIMAT_TO_PROXY_SOCKET_RECV_RSP"},
    {   SIMAT_CODE_END,             (simat_proxy_hadler)NULL,                   "SIMAT_CODE_START"}
};


log_create_module(simat_proxy, PRINT_LEVEL_INFO);
log_create_module(simat_mem, PRINT_LEVEL_INFO);
log_create_module(simat_lwip, PRINT_LEVEL_INFO);


#if !defined (MTK_DEBUG_LEVEL_NONE)
#define SIMATPROXYLOG(fmt, args...)   LOG_I(simat_proxy, fmt, ##args)
#define SIMATMEMLOG(fmt, args...)   LOG_I(simat_mem, fmt, ##args)
#define SIMATLWIPLOG(fmt, args...)   LOG_I(simat_lwip, fmt, ##args)
#else
#define SIMATPROXYLOG(fmt, args...)   printf("[simat_proxy] "fmt"\r\n", ##args)
#define SIMATMEMLOG(fmt, args...)   printf("[simat_mem] "fmt"\r\n", ##args)
#define SIMATLWIPLOG(fmt, args...)   printf("[simat_lwip] "fmt"\r\n", ##args)
#endif

/////////////////////////////////////////////////////////////////////////////////////////

//parsing and construct package
// SIMATMEMLOG("msg id oom, num: %d %d\r\n", p_temp->base_info.type, GET_SIMAT_PROXY_STATIC_MEM_STATUS(p_temp->base_info.type)); 
/******************************************************************************
this macro only work in simat_proxy_allocate() function
*******************************************************************************/
#define GET_STATIC_MEM(pool_type, block_num, msg_id, line) \
    {\
        simat_##pool_type *p_mem = g_socket_management.static_mem.pool_type; \
        simat_##pool_type *p_temp = p_mem;  \
        simat_##pool_type *p_node_header = NULL;    \
        simat_proxy_base_node *p_base_node = NULL;  \
        int index = 0; \     
        for (index = 0; index < block_num; ++index) {   \
            if (0 == GET_SIMAT_PROXY_STATIC_MEM_STATUS(p_temp->base_info.type)) {    \
                break;  \
            }   \
            p_temp += 1;    \
        }   \
        if(block_num == index) {    \
             SIMATMEMLOG("msg id: %s oom, num: %d",  \
                simat_proxy_process_table[p_base_node->msg_id].msg_string, block_num);    \
            configASSERT(0);    \
        }   \        
        p_node_header = (simat_##pool_type *)(p_mem + index); \
        p_base_node = &(p_node_header->base_info); \
        p_data = p_node_header->data; \
        SET_SIMAT_PROXY_STATIC_MEM(p_base_node, msg_id, 1); \
        SIMATPROXYLOG("static allocate success, start address: %x, data address: %x, size:%d, msg:%s, line:%d, type: %d, msg_id: %d\n", \
                    p_mem,  \
                    p_data, \
                    size,   \
                    simat_proxy_process_table[msg_id].msg_string,   \
                    line,   \
                    p_base_node->type,  \
                    p_base_node->msg_id);  \
    }


/******************************************************************************
 * FUNCTION
 *  simat_proxy_allocate
 * DESCRIPTION
 *  simat_proxy_allocate
 * PARAMETERS
 *  unsigned int size, unsigned int msg_id, int line
 * RETURNS
 * user data pointer if success, otherwise will return NULL
 *******************************************************************************/
unsigned char *simat_proxy_allocate(unsigned int size, unsigned int msg_id, int line)
{
    simat_proxy_base_node *p_node_header = NULL;
    unsigned char *p_data = NULL;
    unsigned int index = 0;
    switch (msg_id) {
       /* case SIMAT_CONFIG_CNF:        
        case SIMAT_BRING_UP_CNF:        
            GET_STATIC_MEM(config_cnf_mem, CONFIG_CNF_MAX_NUM, msg_id, line);
            break;
        case SIMAT_BRING_DOWN_IND:
        case SIMAT_CONFIG_REJ:
            GET_STATIC_MEM(reject_rsp_mem, BEAR_REJECT_RSP_MAX_NUM, msg_id, line);
            break;
        case SIMAT_SOCKET_OPEN_CNF:
        //case SIMAT_SOCKET_SEND_CNF:
            GET_STATIC_MEM(soc_base_mem, SOCKET_BASE_MAX_NUM, msg_id, line);
            break;
        case SIMAT_SOCKET_CLOSE_IND:
        case SIMAT_SOCKET_SEND_REJ:
            GET_STATIC_MEM(soc_close_ind_mem, SOCKET_CLOSE_MAX_NUM, msg_id, line);
            break;*/
        case SIMAT_CONFIG_CNF:  // static memory check
        case SIMAT_BRING_UP_CNF:
        case SIMAT_BRING_DOWN_IND:
        case SIMAT_CONFIG_REJ:
        case SIMAT_SOCKET_OPEN_CNF:
        case SIMAT_SOCKET_CLOSE_IND:
        case SIMAT_SOCKET_SEND_REJ:
		case SIMAT_SOCKET_SEND_CNF:
        case SIMAT_SOCKET_SEND_REQ:
        case SIMAT_SOCKET_RECV_IND:
        case SIMAT_CONFIG_REQ:
        case SIMAT_BRING_UP_REQ:
        case SIMAT_BRING_DOWN_REQ:
        case SIMAT_SOCKET_OPEN_REQ:
        case SIMAT_SOCKET_CLOSE_REQ:
        case SIMAT_SOCKET_RECV_RSP:
        case SIMAT_CODE_REJ:
            p_data = simat_proxy_allocate_internal(size, msg_id, line);
            break;
        default:
            break;
    }
    return p_data;
}


/******************************************************************************
 * FUNCTION
 *  simat_proxy_allocate_internal
 * DESCRIPTION
 *  simat_proxy_allocate_internal
 * PARAMETERS
 *  unsigned int size, unsigned int msg_id, int32_t file_line
 * RETURNS
 * user data pointer if success, otherwise will return NULL
 *******************************************************************************/
unsigned char *simat_proxy_allocate_internal(unsigned int size, unsigned int msg_id, int32_t file_line)
{
    unsigned char *p_address = NULL, *p_data_address = NULL;
    unsigned int ref_count = 0;
    simat_proxy_dynamic_node *p_node_header = NULL;
    p_address = pvPortMalloc( sizeof(simat_proxy_dynamic_node) + sizeof(unsigned char) * (size) );
    if (NULL == p_address) {
        SIMATMEMLOG("allocate fail, size:%d, msg:%s, line:%d",
                    size,
                    simat_proxy_process_table[msg_id].msg_string,
                    file_line);
        configASSERT(0);
        return NULL;
    }

    p_data_address = p_address + sizeof(simat_proxy_dynamic_node);

    p_node_header = (simat_proxy_dynamic_node *)(p_address);
    p_node_header->base_info.msg_id = msg_id;
    p_node_header->base_info.type = SET_MEM_ALLOCATE_SOURCE(DYNAMIC_HEAP, DYNAMIC_MAGIC_NUM);
    p_node_header->ref_count = 1;
    p_node_header->size = size;


    SIMATMEMLOG("allocate success, msg_id: %d, type: %d, size: %d\n",
                p_node_header->base_info.msg_id,
                p_node_header->base_info.type,
                p_node_header->size);

    SIMATMEMLOG("allocate success, start address: %x, data address: %x, size:%d, msg:%s, line:%d\n",
                p_address,
                p_data_address,
                size,
                simat_proxy_process_table[msg_id].msg_string,
                file_line);

    return p_data_address;
}


/******************************************************************************
 * FUNCTION
 *  simat_proxy_allocate_simple_internal
 * DESCRIPTION
 *  simat_proxy_allocate_simple_internal
 * PARAMETERS
 *  unsigned int size, int32_t file_line
 * RETURNS
 * user data pointer if success, otherwise will return NULL
 *******************************************************************************/
unsigned char *simat_proxy_allocate_simple_internal(unsigned int size, int32_t file_line)
{
    unsigned char *p_address = NULL, *p_data_address = NULL;
    unsigned int ref_count = 0;
    simat_proxy_dynamic_node *p_node_header = NULL;
    p_address = pvPortMalloc( sizeof(simat_proxy_dynamic_node) + sizeof(unsigned char) * (size) );
    if (NULL == p_address) {
        SIMATMEMLOG("allocate fail, size:%d, line:%d\n",
                    size,
                    file_line);
        configASSERT(0);
        return NULL;
    }

    p_data_address = p_address + sizeof(simat_proxy_dynamic_node);

    p_node_header = (simat_proxy_dynamic_node *)(p_address);
    p_node_header->base_info.type = SET_MEM_ALLOCATE_SOURCE(DYNAMIC_HEAP, DYNAMIC_MAGIC_NUM);
    p_node_header->ref_count = 1;
    p_node_header->size = size;

    SIMATMEMLOG("allocate success, address: %x, size:%d, line:%d\n",
                p_data_address,
                size,
                file_line);

    return p_data_address;
}


/******************************************************************************
 * FUNCTION
 *  simat_proxy_free_simple_internal
 * DESCRIPTION
 *  simat_proxy_free_simple_internal
 * PARAMETERS
 *  buffer_address
 * RETURNS
 * None
 *******************************************************************************/
void simat_proxy_free_simple_internal(unsigned char *buffer_address)
{
    simat_proxy_dynamic_mem_free(buffer_address, __LINE__);
}


/******************************************************************************
 * FUNCTION
 *  simat_set_mem_msg_id
 * DESCRIPTION
 *  simat_set_mem_msg_id
 * PARAMETERS
 *  unsigned char *p_data_address, unsigned int msg_id
 * RETURNS
 *******************************************************************************/
void simat_set_mem_msg_id(unsigned char *p_data_address, unsigned int msg_id)
{
    simat_proxy_dynamic_node *p_address = (simat_proxy_dynamic_node *)(p_data_address - sizeof(simat_proxy_dynamic_node));

    // check dynamic memory status
    unsigned int heap_check_num = GET_MEM_ALLOCATE_SOURCE(p_address->base_info.type);
    unsigned int heap_magic_num = GET_MEM_ALLOCATE_MGAIC_NUM(p_address->base_info.type);

    p_address->base_info.msg_id = msg_id;
    
    SIMATMEMLOG("simat_set_mem_msg_id check_num:%x, magic_num: %x, msg_id:%s\n",
                heap_check_num,
                heap_magic_num,
                simat_proxy_process_table[p_address->base_info.msg_id].msg_string);

    // check data valid
    if ( (DYNAMIC_HEAP != heap_check_num) ||
            (DYNAMIC_MAGIC_NUM != heap_magic_num) ) {
        SIMATMEMLOG("simat_set_mem_msg_id error\n");
        configASSERT(0);
    }
}


/******************************************************************************
 * FUNCTION
 *  simat_proxy_dynamic_mem_free
 * DESCRIPTION
 *  simat_proxy_dynamic_mem_free
 * PARAMETERS
 *  unsigned char *buffer_address, int32_t file_line
 * RETURNS
 *******************************************************************************/
void simat_proxy_dynamic_mem_free(unsigned char *buffer_address, int32_t file_line)
{
    simat_proxy_dynamic_node *p_address = (simat_proxy_dynamic_node *)(buffer_address - sizeof(simat_proxy_dynamic_node));
    unsigned int msg_id = p_address->base_info.msg_id;
    unsigned int size = p_address->size;
    unsigned int ref_count = p_address->ref_count;

    // check dynamic memory status
    unsigned int heap_check_num = GET_MEM_ALLOCATE_SOURCE(p_address->base_info.type);
    unsigned int heap_magic_num = GET_MEM_ALLOCATE_MGAIC_NUM(p_address->base_info.type);


    SIMATMEMLOG("free, msg_id: %d, type: %d, size: %d\r\n",
                p_address->base_info.msg_id,
                p_address->base_info.type,
                p_address->size);

    // check data valid
    if ( (DYNAMIC_HEAP != heap_check_num) ||
            (DYNAMIC_MAGIC_NUM != heap_magic_num) ) {
        SIMATMEMLOG("dynamic mem free error: check_num: %d, magic_num%: %d, file_len: %d, msg_id: %d content: %s\n",
                    heap_check_num,
                    heap_magic_num,
                    file_line,
                    p_address->base_info.msg_id,
                    simat_proxy_process_table[p_address->base_info.msg_id].msg_string);
        //configASSERT(0);
    }

    --ref_count;

    if (0 == ref_count) {
         SIMATMEMLOG("free success, start address: %x, data address: %x, size:%d, msg:%s, line:%d\n",
                    p_address,
                    buffer_address,
                    size,
                    simat_proxy_process_table[msg_id].msg_string,
                    file_line);
         
        vPortFree(p_address);
        p_address = NULL;
       
    } else {
        p_address->ref_count = ref_count;
        SIMATMEMLOG("free fail, start address: %x, data address: %x, size:%d, msg:%s, count:%d, line:%d\n",
                    p_address,
                    buffer_address,
                    size,
                    simat_proxy_process_table[msg_id].msg_string,
                    ref_count,
                    file_line);
        configASSERT(0);
    }

    return;
}


/******************************************************************************
 * FUNCTION
 *  simat_proxy_static_mem_free
 * DESCRIPTION
 *  simat_proxy_static_mem_free
 * PARAMETERS
 *  unsigned char *buffer_address, int32_t file_line
 * RETURNS
 * void
 *******************************************************************************/
void simat_proxy_static_mem_free(unsigned char *buffer_address, int32_t file_line)
{
    simat_proxy_base_node *p_base_node_address = NULL;
    p_base_node_address = (simat_proxy_base_node *)(buffer_address - sizeof(simat_proxy_base_node));
    unsigned int heap_check_num = GET_MEM_ALLOCATE_SOURCE(p_base_node_address->type);
    unsigned int heap_magic_num = GET_MEM_ALLOCATE_MGAIC_NUM(p_base_node_address->type);

    SIMATMEMLOG("static mem free addr: %x, start: %x, check_num:%x, magic_num%x, msg_id:%s, line: %d, id: %x\r\n",
                buffer_address,
                p_base_node_address,
                heap_check_num,
                heap_magic_num,
                simat_proxy_process_table[p_base_node_address->msg_id].msg_string,
                file_line,
                p_base_node_address->msg_id);
    
    // check data valid
    if ( (STATIC_HEAP == heap_check_num) &&
            (STATIC_MAGIC_NUM == heap_magic_num) ) {
        SET_SIMAT_PROXY_STATIC_MEM_FREE(p_base_node_address->type);
    } else {
        SIMATMEMLOG("static mem free error\n");
        configASSERT(0);
    }
}


/******************************************************************************
 * FUNCTION
 *  simat_proxy_free
 * DESCRIPTION
 *  simat_proxy_free
 * PARAMETERS
 *  unsigned char *buffer_address, int32_t file_line
 * RETURNS
 * void
 *******************************************************************************/
void simat_proxy_free(unsigned char *buffer_address, int32_t file_line)
{
    simat_proxy_base_node *p_base_node_address = NULL;
    SIMAT_Base_Header *p_header = (SIMAT_Base_Header *)buffer_address;
    switch (p_header->simat_code) {            
     /*   case SIMAT_CONFIG_CNF:  // static memory check
        case SIMAT_BRING_UP_CNF:
        case SIMAT_BRING_DOWN_IND:
        case SIMAT_CONFIG_REJ:
        case SIMAT_SOCKET_OPEN_CNF:
        //case SIMAT_SOCKET_SEND_CNF:
        case SIMAT_SOCKET_CLOSE_IND:
        case SIMAT_SOCKET_SEND_REJ:
            simat_proxy_static_mem_free(buffer_address, file_line);
            break;*/
        case SIMAT_CONFIG_CNF:  // static memory check
        case SIMAT_BRING_UP_CNF:
        case SIMAT_BRING_DOWN_IND:
        case SIMAT_CONFIG_REJ:
        case SIMAT_SOCKET_OPEN_CNF:
        case SIMAT_SOCKET_CLOSE_IND:
        case SIMAT_SOCKET_SEND_REJ:
		case SIMAT_SOCKET_SEND_CNF:
        case SIMAT_SOCKET_SEND_REQ:
        case SIMAT_SOCKET_RECV_IND:
        case SIMAT_CONFIG_REQ:
        case SIMAT_BRING_UP_REQ:
        case SIMAT_BRING_DOWN_REQ:
        case SIMAT_SOCKET_OPEN_REQ:
        case SIMAT_SOCKET_CLOSE_REQ:
        case SIMAT_SOCKET_RECV_RSP:
        case SIMAT_CODE_REJ:
            simat_proxy_dynamic_mem_free(buffer_address, file_line);
            break;
        default:
             SIMATMEMLOG("simat_proxy_free msg id error :%d  addr: %x, line: %d\n", 
                         p_header->simat_code, buffer_address, file_line);
            break;
    }
}



/******************************************************************************
 * FUNCTION
 *  simat_conn_to_request_socket
 * DESCRIPTION
 *  simat_conn_to_request_socket
 * PARAMETERS
 *  struct netconn *param
 * RETURNS
 * success:request_socket_id
 * failure: -1
 *******************************************************************************/
int simat_conn_to_request_socket(struct netconn *param)
{
    int index = 0;

    if (NULL != param) {
        configASSERT(0);//  lwip need to  increase netconn buffer
    }

    for (index = 0; index < MAX_SIMAT_SOCKET_NUM; ++index) {
        if (param == g_socket_management.management_node[index].create_conn) {
            return g_socket_management.management_node[index].request_socket_id;
        }
    }

    return -1;
}


/******************************************************************************
 * FUNCTION
 *  simat_get_socket_type
 * DESCRIPTION
 *  simat_get_socket_type
 * PARAMETERS
 *  int lwip_soc_type
 * RETURNS
 * SOCKET_TYPE
 *******************************************************************************/
SOCKET_TYPE simat_get_socket_type(int lwip_soc_type)
{
    SOCKET_TYPE soc_type = TCP_TYPE;
    switch (lwip_soc_type) {
        case NETCONN_TCP:
        case NETCONN_TCP_IPV6:
            soc_type = TCP_TYPE;
            break;
        case NETCONN_UDP:
        case NETCONN_UDP_IPV6:
            soc_type = UDP_TYPE;
            break;
        default:
            soc_type = RAW_TYPE;
            break;
    }
    
    SIMATPROXYLOG("soc_type %d\r\n", soc_type);

    return soc_type;
}


/******************************************************************************
 * FUNCTION
 *  simat_socket_to_conn
 * DESCRIPTION
 *  simat_socket_to_conn
 * PARAMETERS
 *  int32_t request_socket_id
 * RETURNS
 * success:struct netconn *
 * failure: NULL
 *******************************************************************************/
struct netconn *simat_socket_to_conn(int32_t request_socket_id)
{
    int index = 0;

    if (0 >= request_socket_id) {
        configASSERT(0);// real assert, please MD check the reason
    }

    for (index = 0; index < MAX_SIMAT_SOCKET_NUM; ++index) {
        if (request_socket_id == g_socket_management.management_node[index].request_socket_id) {
            return g_socket_management.management_node[index].create_conn;
        }
    }

    return NULL;
}



/******************************************************************************
 * FUNCTION
 *  get_socket_index_from_manager
 * DESCRIPTION
 *  get_socket_index_from socket manager
 * PARAMETERS
 * socket_id / type
 * RETURNS
 * success: index of the socket manager
 * failure: -1
 *******************************************************************************/
int get_socket_index_from_manager(void *param, simat_socket_internal_t type)
{
    int index = -1, request_socket_id = -1;
    struct netconn *p_conn = NULL;


    //SIMATPROXYLOG("get_socket_index_from_manager netconn type :%d\r\n", type);

    if (real_type == type) {
        p_conn = (struct netconn *)param;

     
        SIMATPROXYLOG("get_socket_index_from_manager netconn :%x  %x\r\n",
                                p_conn,
                                g_socket_management.management_node[0].create_conn);
        

        for (index = 0; index < MAX_SIMAT_SOCKET_NUM; ++index) {
            if (p_conn == g_socket_management.management_node[index].create_conn) {
                return index;
            }
        }
    } else if (request_type == type) {
        request_socket_id = (int32_t)param;

        SIMATPROXYLOG("get_socket_index_from_manager request_socket_id :%d  %d\r\n",
                      request_socket_id,
                      g_socket_management.management_node[0].request_socket_id);

        //simat_print_socket_management_info();

        for (index = 0; index < MAX_SIMAT_SOCKET_NUM; ++index) {
            if (request_socket_id == g_socket_management.management_node[index].request_socket_id) {
                return index;
            }
        }
    }

    SIMATPROXYLOG("index error");
//    configASSERT(0); // simat_proxy internal error

    return -1;
}


/******************************************************************************
 * FUNCTION
 *  simat_socket_get_status_memory
 * DESCRIPTION
 *  simat_socket_get_status_memory
 * PARAMETERS
 * socket_management_node *simat_soc_info, simat_direct_t dir_type
 * RETURNS
 * success: index of the socket manager
 * failure: -1
 *******************************************************************************/
int32_t simat_socket_get_status_memory(socket_management_node *simat_soc_info, simat_direct_t dir_type)
{
    int index = 0;
    unsigned char *p_data = NULL;
    if (AP_TO_MD == dir_type) {
        for (index = 0; index < MAX_SOCKET_MEMORY_BLOCK_NUM; ++index) {
            if (NULL == simat_soc_info->recv_buffer[index].base_address) {
                return index;
            }
        }
    } else if (MD_TO_AP == dir_type) {
        for (index = 0; index < MAX_SOCKET_MEMORY_BLOCK_NUM; ++index) {
            if (NULL == simat_soc_info->send_buffer[index].base_address) {
                return index;
            }
        }
    }
    SIMATPROXYLOG("soc_get_memory req  id:%d, index:%d\r\n",
                  simat_soc_info->request_socket_id,
                  index);
    return -1;
}


/******************************************************************************
 * FUNCTION
 *  construct_simat_user_data
 * DESCRIPTION
 *  construct_simat_user_data for AP to MD side package
 * PARAMETERS
 * socket_id(request socket id by SIMAT side) / data / data_len / unsigned char* p_out_header
 * RETURNS
 * success:SIMAT_Data_Req pointer
 * failure: NULL
 *******************************************************************************/
SIMAT_Data_Req *construct_simat_user_data(unsigned int socket_id, LWIP_recv_data *data_unit, unsigned int num, unsigned char* p_out_header)
{
    SIMAT_Data_Req *p_simat_data = NULL;
    int temp_socket_id = socket_id;
    int total_len = 0;
    unsigned int index = 0;
    unsigned char *tmp_data = NULL;

    // pay atention if MD_TO_AP socket_id is the id that allocate by SIMAT side, not real send/recv socket id

    while (index <= num) {
        total_len += data_unit[index].size;
        ++index;
    }

    SIMATMEMLOG("merge_lwip_recv_data total num: %d total_len: %d\r\n",
                num, total_len);

    total_len += sizeof(SIMAT_Data_Req);

    p_simat_data = (SIMAT_Data_Req *)SIMAT_PROXY_ALLOCATE(total_len, SIMAT_SOCKET_RECV_IND);

    p_out_header = p_simat_data - sizeof(simat_proxy_dynamic_node);
    
    if (NULL == p_simat_data) {
        SIMATMEMLOG("recv p_simat_data NULL\r\n");
        configASSERT(0); //simat_proxy error.  change, or remove
        return NULL;
    }

    memset(p_simat_data, 0, total_len * sizeof(char));
    p_simat_data->soc_base.base_header.simat_code = SIMAT_SOCKET_RECV_IND;
    p_simat_data->soc_base.base_header.simat_length = total_len;
    p_simat_data->soc_base.socket_id = temp_socket_id;

    tmp_data = (unsigned char *)p_simat_data;
    tmp_data += sizeof(SIMAT_Data_Req);

    //copy data
    for (index = 0; index <= num; ++index) {
        memcpy(tmp_data, data_unit[index].p_data, data_unit[index].size);
        tmp_data += data_unit[index].size;
    }

    //SIMATMEMLOG("construct_simat_user_data p_simat_data %s\r\n", p_simat_data->data);

    return p_simat_data;
}


/******************************************************************************
 * FUNCTION
 *  close_socket_data
 * DESCRIPTION
 *  close_socket_data
 * PARAMETERS
 * sock_management_index
 * RETURNS
 * success: SIMAT_PROXY_SUCCESS
 * failure: SIMAT_PROXY_FAILURE
 *******************************************************************************/
simat_ret_t close_socket_data(int sock_management_index)
{
    memory_block *p_recv_block = NULL, *p_send_block = NULL;
    int index = 0, ret = 0;
    socket_management_node *p_node = NULL;
    int socket_type = TCP_TYPE;

    SIMATPROXYLOG("close_socket_data enter %d\r\n", sock_management_index);
    
    if ( (0 > sock_management_index) ||
            (MAX_SIMAT_SOCKET_NUM <= sock_management_index) ) {
        SIMATPROXYLOG("close_socket_data invalidate socket_management_index %d", sock_management_index);
        return SIMAT_PROXY_FAILURE;
    }

    p_node = &(g_socket_management.management_node[sock_management_index]);

    if (NULL == p_node->create_conn){
        SIMATPROXYLOG("close  create_conn NULL\r\n", sock_management_index);
        return SIMAT_PROXY_SUCCESS;        
    }

    p_recv_block = &(p_node->recv_buffer[0]);
    p_send_block = &(p_node->send_buffer[0]);

    socket_type = simat_get_socket_type(p_node->socket_type);

    SIMATPROXYLOG("close_socket_data soc status: %d\r\n",  p_node->status);

    // this code should be implement before netconn_delete
    // because EVT_RECV will be recv by task and logical will be wrong if not set this.

    p_node->status = SOCKET_CLOSE;   


    ret = netconn_delete(p_node->create_conn);
    if (ERR_OK != ret) {
        SIMATPROXYLOG("close_socket_data conn_delete error happened %d", ret);
        configASSERT(0); // could remove?   need lwip check the  reason
    }

    SIMATPROXYLOG("netconn_delete\r\n");

    for (index = 0; index < MAX_SOCKET_MEMORY_BLOCK_NUM; ++index) {
        if (NULL != p_recv_block->base_address) {
            SIMATPROXYLOG("close_socket base: %x, start: %x", 
                            p_recv_block->base_address, p_recv_block->start_address);
            SIMAT_PROXY_FREE(p_recv_block->start_address);
            p_recv_block->base_address = NULL;
        }
        if (NULL != p_send_block->base_address) {
            SIMAT_PROXY_FREE(p_send_block->start_address);
            p_send_block->base_address = NULL;
        }
    }

    memset(p_node, 0, sizeof(socket_management_node));

    p_node->status = SOCKET_INIT;  

    SIMATPROXYLOG("close_socket_data end %d\r\n", sock_management_index);

    return SIMAT_PROXY_SUCCESS;
}


/******************************************************************************
 * FUNCTION
 *  convert_chars_to_bearer_open_request_info
 * DESCRIPTION
 *  convert_chars_to_bearer_open_request_info
 * PARAMETERS
 * [out]simat_bearer_info *bearer_info
 * [in] unsigned char *bytes_data
 * RETURNS
 * success: SIMAT_PROXY_SUCCESS
 * failure: SIMAT_PROXY_FAILURE
 *******************************************************************************/
simat_ret_t convert_chars_to_bearer_open_request_info(simat_bearer_info *bearer_info, unsigned char *bytes_data)
{
    unsigned char *p_bytes = bytes_data;
    unsigned char *data_type = SIMAT_TLV_GET_TYPE(p_bytes);
    unsigned char data_length = SIMAT_TLV_GET_LENGTH(p_bytes);

    // clear bearer_info
    memset(bearer_info, 0, sizeof(simat_bearer_info));

    // obtain infomation from MD request
    if (BEARER_TYPE_APN_NAME != data_type) {
        SIMATPROXYLOG("BEARER_TYPE_APN_NAME type is wrong: %d\r\n", data_type);
        return SIMAT_PROXY_FAILURE;
    }
    SIMAT_TLV_GET_BEARER_APN(p_bytes, bearer_info->apn_val);
    p_bytes += data_length;

    data_type = SIMAT_TLV_GET_TYPE(p_bytes);
    data_length = SIMAT_TLV_GET_LENGTH(p_bytes);
    if (BEARER_TYPE_PDP_TYPE != data_type) {
        SIMATPROXYLOG("BEARER_TYPE_PDP_TYPE type is wrong: %d\r\n", data_type);
        return SIMAT_PROXY_FAILURE;
    }
    bearer_info->pdp_val = SIMAT_TLV_GET_PDP_TYPE(p_bytes);    
    p_bytes += data_length;

    data_type = SIMAT_TLV_GET_TYPE(p_bytes);
    data_length = SIMAT_TLV_GET_LENGTH(p_bytes);
    if (BEARER_TYPE_USER_NAME != data_type) {
        SIMATPROXYLOG("BEARER_TYPE_USER_NAME type is wrong: %d\r\n", data_type);
        return SIMAT_PROXY_FAILURE;
    }
    if (data_length != 2) {
        SIMAT_TLV_GET_BEARER_USER_NAME(p_bytes, bearer_info->user_val);
    } else {
        memcpy(bearer_info->user_val,"",1); //copy null string
    }
    p_bytes += data_length;
    
    data_type = SIMAT_TLV_GET_TYPE(p_bytes);
    data_length = SIMAT_TLV_GET_LENGTH(p_bytes);    
    if (BEARER_TYPE_PASSWORD != data_type) {
        SIMATPROXYLOG("BEARER_TYPE_PASSWORD type is wrong: %d\r\n", data_type);
        return SIMAT_PROXY_FAILURE;
    }
    if (data_length != 2) {
        SIMAT_TLV_GET_BEARER_PASSWORD(p_bytes, bearer_info->passwd_val);
    } else {
            memcpy(bearer_info->passwd_val,"",1); //copy null string
    }

    bearer_info->valid_flag = 1;

    return SIMAT_PROXY_SUCCESS;

}


/******************************************************************************
 * FUNCTION
 *  convert_chars_to_open_request_info
 * DESCRIPTION
 *  convert_chars_to_open_request_info
 * PARAMETERS
 * [out]simat_remote_socket_info *soc_data_info
 * [in] unsigned char *bytes_data
 * RETURNS
 * success: SIMAT_PROXY_SUCCESS
 * failure: SIMAT_PROXY_FAILURE
 *******************************************************************************/
simat_ret_t convert_chars_to_open_request_info(simat_remote_socket_info *soc_data_info, unsigned char *bytes_data)
{
    unsigned char data_type = SIMAT_TLV_GET_TYPE(bytes_data);
    unsigned char total_len = 0;
    unsigned char protocol_type = 0;
    unsigned short port_number = 0;
    unsigned char *ip_address = SIMAT_TLV_GET_REMOTE_SOCKET_IP_ADDRESS_START(bytes_data);
    if (TYPE_REMOTE_SOCKET != data_type) {
        SIMATPROXYLOG("socket open info type is wrong: %d\r\n", data_type);
        return SIMAT_PROXY_FAILURE;
    }
    // jugde netwotk type
    total_len = SIMAT_TLV_GET_LENGTH(bytes_data);
    if (IPV4_SOCKET_INFO_FROM_MD_LEN == total_len) {
        soc_data_info->netwotk_type = PDP_TYPE_IPV4;
		memset(soc_data_info->addr, 0, MAX_IP_ADDRESS_SIZE);
        memcpy(soc_data_info->addr, ip_address, IPV4_SOCKET_LEN * sizeof(unsigned char));
    } else if (IPV6_SOCKET_INFO_FROM_MD_LEN == total_len) {
        soc_data_info->netwotk_type = PDP_TYPE_IPV6;
        memcpy(soc_data_info->addr, ip_address, IPV6_SOCKET_LEN * sizeof(unsigned char));
    } else {
        SIMATPROXYLOG("wrong network type from simat_length: %d\r\n", total_len);
        return SIMAT_PROXY_FAILURE;
    }
    // obtain protocol tcp or udp
    protocol_type = SIMAT_TLV_GET_REMOTE_SOCKET_PORT_PROTOCOL(bytes_data);
    if ( (PORT_PROTOCOL_TCP != protocol_type) && (PORT_PROTOCOL_UDP != protocol_type ) ) {
        SIMATPROXYLOG("wrong netowork protocol type : %d\r\n", protocol_type);
        return SIMAT_PROXY_FAILURE;
    } else {
        soc_data_info->port_protocol = protocol_type;
    }
    // obtain port number
    port_number = SIMAT_TLV_GET_REMOTE_SOCKET_PORT_NUMBER(bytes_data);
    soc_data_info->port_number = port_number;

    SIMATPROXYLOG("netowork type:%d, protocol type:%d, portocol port: %d\r\n",
                  soc_data_info->netwotk_type,
                  soc_data_info->port_protocol,
                  soc_data_info->port_number);

    return SIMAT_PROXY_SUCCESS;
}


/******************************************************************************
 * FUNCTION
 *  simat_print_socket_management_info
 * DESCRIPTION
 *  for debug to print total infomation
 * PARAMETERS
 * RETURNS
 *******************************************************************************/
void simat_print_socket_management_info()
{
    int index = 0, socket_type_index = 0;
    const char *str_socket_type[] = {"None", "TCP_v4", "TCP_v6", "UDP_v4", "UDP_v6"};
    socket_management_node *p_node = NULL;
    SIMATPROXYLOG("index  socket_id  create_conn    type    status  send_buffer  recv_buffer\r\n");
    for (index = 0; index < MAX_SIMAT_SOCKET_NUM; ++index) {
        p_node = &(g_socket_management.management_node[index]);
        switch (p_node->socket_type) {
            case NETCONN_TCP:
                socket_type_index = 1;
                break;
            case NETCONN_TCP_IPV6:
                socket_type_index = 2;
                break;
            case NETCONN_UDP:
                socket_type_index = 3;
                break;
            case NETCONN_UDP_IPV6:
                socket_type_index = 4;
                break;
            default:
                socket_type_index = 0;
                break;
        }
        printf("%d        %d           %x         %s       %d         %x            %x\r\n",
                      index,
                      p_node->request_socket_id,
                      p_node->create_conn,
                      str_socket_type[socket_type_index],
                      p_node->status,
                      p_node->send_buffer[0].start_address,
                      p_node->recv_buffer[0].start_address);
    }
}


/*****************************************************************************
 * FUNCTION
 *  simat_send_event_internal
 * DESCRIPTION
 *  Send message from ap to md task for simat proxy internal
 * PARAMETERS
 *  event_id        [in]
 *  param1          [in]
 *  param2          [in]
 * RETURNS
 *  int32_t
 *****************************************************************************/
void simat_send_event_internal(unsigned int msg_id, SIMAT_Base_Header *param)
{
    simat_proxy_modem_message_struct *simat_proxy_message = NULL;
    //simat_proxy_message.message_id = msg_id;
    //simat_proxy_message.param = (void *)param;
    simat_proxy_message = pvPortMalloc(sizeof(simat_proxy_modem_message_struct) * sizeof(unsigned char));
    simat_proxy_message->message_id = msg_id;
    simat_proxy_message->param = (void *)param;

    SIMATPROXYLOG("simat_send_event_internal id: %d \n",msg_id);

    if (pdTRUE != xQueueSend(simat_proxy_event_queue, &simat_proxy_message, 10)) {
        SIMATPROXYLOG("simat_send_event_internal queue full\r\n");
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

//simat_modem_task:

int simat_connect(struct netconn *conn, ip_addr_t *ip_addr, unsigned short port)
{
    struct sockaddr_in sock_addr = {0};
    int flags = 0;
    int err = 0;
    int errlen = sizeof(err);
    unsigned int ret = ERR_OK;

    ret = netconn_connect(conn, ip_addr, port);

    SIMATLWIPLOG("netconn_connect ret:%d\n", ret);

    if ( (ERR_INPROGRESS == ret) || (ERR_WOULDBLOCK == ret) || (ERR_OK == ret)) {
        return SIMAT_PROXY_SUCCESS;
    } else if ( (ERR_MEM == ret) || (ERR_BUF == ret) ) {
        SIMATMEMLOG("netconn_connect mem fail\n");
        configASSERT(0); // could remove?  need lwip check the reason
        return SIMAT_PROXY_FAILURE;
    } else {
        return SIMAT_PROXY_FAILURE;
    }
}


void simat_proxy_socket_open_request_handler(SIMAT_Base_Header *msg)
{
    // create socket
    int simat_sock_fd = -1, index = 0, socket_type = 0;
    SIMAT_SOC_Open_Req *soc_open_req = (SIMAT_SOC_Open_Req *)msg;
    int request_socket_id = soc_open_req->soc_base.socket_id;
    simat_remote_socket_info soc_open_info = {0};
    char ip_address[MAX_IP_ADDRESS_VAL_LEN] = {0};
    ip_addr_t ip_addr = {0};
    simat_ret_t ret = SIMAT_PROXY_FAILURE;
    enum netconn_type net_type = NETCONN_INVALID;
    struct netconn *soc_conn = NULL;

    SIMATPROXYLOG("msg start :%x\r\n", msg);
    
    ret = convert_chars_to_open_request_info(&soc_open_info, soc_open_req->remote_socket_info);



    SIMATPROXYLOG("simat_proxy_socket_open_request_handler code: %d, len: %d, soc_id: %d,netwotk_type:%d, port_protocol:%d,port_number:%d, ip:%d.%d.%d.%d\r\n",
                  soc_open_req->soc_base.base_header.simat_code,
                  soc_open_req->soc_base.base_header.simat_length,
                  soc_open_req->soc_base.socket_id,
                  soc_open_info.netwotk_type,
                  soc_open_info.port_protocol,
                  soc_open_info.port_number,
                  soc_open_info.addr[0],
                  soc_open_info.addr[1],
                  soc_open_info.addr[2],
                  soc_open_info.addr[3]);
    // vPortFree memory
    if (NULL != msg) {
        SIMATPROXYLOG("msg :%x\r\n", msg);
        SIMAT_PROXY_FREE(msg);
        msg = NULL;
    } else {
        SIMATPROXYLOG("simat_proxy_socket_open_request_handler msg error\n");
        return;
    }

    if (SIMAT_PROXY_SUCCESS != ret) {
        SIMATPROXYLOG("parsing socket open request fail\n");
        return;
    }

    //check ip type
    if (PORT_PROTOCOL_TCP == soc_open_info.port_protocol) {
        if (PDP_TYPE_IPV4 == soc_open_info.netwotk_type) {
            net_type = NETCONN_TCP;
        } else if (PDP_TYPE_IPV6 == soc_open_info.netwotk_type) {
            net_type = NETCONN_TCP_IPV6;
        }
    } else if (PORT_PROTOCOL_UDP == soc_open_info.port_protocol) {
        if (PDP_TYPE_IPV4 == soc_open_info.netwotk_type) {
            net_type = NETCONN_UDP;
        } else if (PDP_TYPE_IPV6 == soc_open_info.netwotk_type) {
            net_type = NETCONN_UDP_IPV6;
        }
    }
    if (NETCONN_INVALID == net_type) {
        SIMATPROXYLOG("wrong network type");
        configASSERT(0); // real assert, need MD check the reason.
		return;
    }

    soc_conn = netconn_new_with_callback(net_type, simat_proxy_lwip_callback);
    if (NULL == soc_conn) {
        SIMATMEMLOG("netconn_new_with_callback no memory");
        configASSERT(0);// could remove? need lwip comment.
		return;
    }

    // TCP should don't set auto recv
    if (TCP_TYPE == simat_get_socket_type(net_type)) {
        SIMATPROXYLOG("yy netconn_set_noautorecved \r\n");
        netconn_set_noautorecved(soc_conn, 1);
    }

    // check socket management empty position
    for (index = 0; index < MAX_SIMAT_SOCKET_NUM; ++index) {
        if (SOCKET_INIT == g_socket_management.management_node[index].status) {
            break;
        }
    }

    SIMATMEMLOG("simat_proxy_socket_open_request_handler index %d\r\n", index);

    // add to socket management
    memset(g_socket_management.management_node + index, 0, 
    	  sizeof(socket_management_node));
    g_socket_management.management_node[index].status = SOCKET_CREATE;
    g_socket_management.management_node[index].request_socket_id = request_socket_id;
    g_socket_management.management_node[index].socket_type = net_type;
    g_socket_management.management_node[index].create_conn = soc_conn;

    // convert ipv4 or ipv6 address
    ip_addr.type = soc_open_info.netwotk_type;
    memcpy(&(ip_addr.u_addr), soc_open_info.addr, MAX_IP_ADDRESS_SIZE);
    ipaddr_ntoa_r(&ip_addr, ip_address, MAX_IP_ADDRESS_VAL_LEN);

    simat_print_socket_management_info();

    //connect
    ret = simat_connect(g_socket_management.management_node[index].create_conn, &ip_addr, soc_open_info.port_number);

    SIMATLWIPLOG("simat_connect ret:%d\n", ret);

    // if fail, send socket close indication to SIMAT and close socket
    if (SIMAT_PROXY_SUCCESS != ret) {        
        PROCESS_CONNECT_RESULT(request_socket_id, SOCKET_OPEN_FAILED);
    }else { // UDP has no connection, so if return success, need notify MD socket connect success
        if(UDP_TYPE == simat_get_socket_type(net_type)) {
            process_connect_success(request_socket_id);
        }
    }
}



void simat_cmux_send(SIMAT_Base_Header *data_ptr, unsigned int simat_length, unsigned int msg_type)
{
    mux_ap_status_t ret = MUX_AP_STATUS_OK;

    // data has already depth copied
    // send to CMUX to MD
    if (SIMAT_CMUX_CONNECT == g_simat_cmux_status.simat_cmux_status) {
        ret = mux_ap_send_data(g_simat_cmux_status.channel_id,
                               (const unsigned char *)data_ptr, simat_length, (void *)msg_type);
        if (MUX_AP_STATUS_OK != ret) {
            // this case will not happen unless param is invalidate
            SIMATPROXYLOG("cmux_send_data failed:%d \r\n", ret);
        }
    } else {
        SIMATPROXYLOG("simat_cmux_status wrong:%d \r\n", g_simat_cmux_status.simat_cmux_status);
    }

#ifdef SIMAT_PROXY_INTERNAL_UT_TEST
    mux_ap_event_send_completed_t cmux_send_test_data = {0};
    cmux_send_test_data.channel_id = MUX_AP_CHANNEL_TYPE_SIMAT;
    cmux_send_test_data.data_buffer = data_ptr;
    cmux_send_test_data.user_data = (void *)msg_type;
    simat_proxy_cmux_cb(MUX_AP_EVENT_SEND_COMPLETED, &cmux_send_test_data);
#endif /* SIMAT_PROXY_INTERNAL_UT TEST */
}


void simat_proxy_socket_open_confirm_handler(SIMAT_Base_Header *msg)
{
    SIMATPROXYLOG("connect success:id:%d \r\n", ((SIMAT_Open_Conf *)msg)->socket_id);
    simat_cmux_send(msg, sizeof(SIMAT_Open_Conf),
                    SET_SIMAT_MSG_ID_FROM_CMUX(SIMAT_SOCKET_OPEN_CNF));
}


void simat_proxy_socket_close_ind_handler(SIMAT_Base_Header *msg)
{
    unsigned int socket_management_index = -1;
    SIMAT_Close_Ind *p_soc_close_ind = (SIMAT_Close_Ind *)msg;

    SIMATPROXYLOG("close socket success:id:%d cause: %d\r\n",
                  p_soc_close_ind->soc_base.socket_id,
                  p_soc_close_ind->cause);

    simat_cmux_send(msg, sizeof(SIMAT_Close_Ind), 
                    SET_SIMAT_MSG_ID_FROM_CMUX(SIMAT_SOCKET_CLOSE_IND));

#ifdef SIMAT_PROXY_INTERNAL_UT_TEST
    simat_ut_bearer_close(p_soc_close_ind->soc_base.socket_id);        
#endif /* SIMAT_PROXY_INTERNAL_UT_TEST */
}


void simat_proxy_cmd_reject_handler(SIMAT_Base_Header *msg)
{
    SIMAT_Cmd_Reject *simat_cmd_error = (SIMAT_Cmd_Reject *)msg;

    SIMATPROXYLOG("cmd_reject_handler msg id:%d \r\n", simat_cmd_error->base_header.simat_code);
    simat_cmd_error->reject_code = simat_cmd_error->base_header.simat_code;
    simat_cmd_error->base_header.simat_code = SIMAT_CODE_REJ;

    simat_cmux_send(msg, sizeof(SIMAT_Cmd_Reject), SET_SIMAT_MSG_ID_FROM_CMUX(SIMAT_CODE_REJ));
}


/******************************************************************************
 * FUNCTION
 *  simat_proxy_bearer_open_request_handler
 * DESCRIPTION
 *  Process message in queue
 * PARAMETERS
 *  message         [in]
 * RETURNS
 *  void
 *****************************************************************************/
void simat_proxy_bearer_open_request_handler(SIMAT_Base_Header *msg)
{
    SIMAT_Config_Req *simat_config_req = (SIMAT_Config_Req *)msg;
    char *p = (char *)msg;
    simat_ret_t ret = SIMAT_PROXY_SUCCESS;
    simat_bearer_info *bearer_info = &g_socket_management.bearer_info;

    // obtain bearer infomation
    // print original data
    int log_total_len = sizeof(simat_config_req->bearer_info);
    int index = 0;
    for(; index < log_total_len; ++index) {
        SIMATPROXYLOG("bcr~%d~\r\n", 
                   (simat_config_req->bearer_info)[index]);
    }
	
    ret = convert_chars_to_bearer_open_request_info(bearer_info, simat_config_req->bearer_info);
    SIMATPROXYLOG("bearer_info valid_flag :%d\r\n", 
                   bearer_info->valid_flag);

    if (SIMAT_PROXY_FAILURE == ret) {
        // notify MD param is wrong
        process_bearer_config_reject(INVALIDAE_PARAMETER);
        return;
    }

    process_bearer_confirm(SIMAT_CONFIG_CNF);
}


/******************************************************************************
 * FUNCTION
 *  simat_proxy_bearer_open_confirm_handler
 * DESCRIPTION
 *  Process message in queue
 * PARAMETERS
 *  message         [in]
 * RETURNS
 *  void
 *****************************************************************************/
void simat_proxy_bearer_open_confirm_handler(SIMAT_Base_Header *msg)
{
    SIMATPROXYLOG("simat_proxy_bearer_open_confirm_handler\r\n");

    simat_cmux_send(msg, sizeof(SIMAT_Base_Header),
                    SET_SIMAT_MSG_ID_FROM_CMUX(msg->simat_code));
}


/******************************************************************************
 * FUNCTION
 *  simat_proxy_bearer_open_reject_handler
 * DESCRIPTION
 *  Process message in queue
 * PARAMETERS
 *  message         [in]
 * RETURNS
 *  void
 ******************************************************************************/
void simat_proxy_bearer_open_reject_handler(SIMAT_Base_Header *msg)
{
    SIMATPROXYLOG("simat_proxy_bearer_open_reject_handler\r\n");

    simat_cmux_send(msg, sizeof(SIMAT_Config_Reject),
                    SET_SIMAT_MSG_ID_FROM_CMUX(SIMAT_CONFIG_REJ));
}


/******************************************************************************
 * FUNCTION
 *  simat_proxy_bring_up_request_handler
 * DESCRIPTION
 *  Process message in queue
 * PARAMETERS
 *  message         [in]
 * RETURNS
 *  void
 ******************************************************************************/
void simat_proxy_bring_up_request_handler(SIMAT_Base_Header *msg)
{
    SIMAT_Bring_Up_Req *bring_up_msg = (SIMAT_Bring_Up_Req *)msg;
    simat_bearer_info *bearer_info = &(g_socket_management.bearer_info);
    unsigned int app_id = 0;
    tel_conn_mgr_pdp_type_enum activated_pdp_type = TEL_CONN_MGR_PDP_TYPE_NONE;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_OK;
	int log_size = 0,index = 0;

    if (1 != bearer_info->valid_flag) {
        SIMATPROXYLOG("simat_proxy_bring_up_request_handler flag error\r\n");
        // bearer info is worng, notify MD side, NetIf Down Indication
        process_bearer_bring_down(FAILED_BRING_UP);
        return;
    }

	SIMATPROXYLOG("pdp_val: %c\r\n", (bearer_info->pdp_val));

	log_size = sizeof(bearer_info->apn_val);
	for(index = 0; index < log_size; ++index){
		SIMATPROXYLOG("apn: %c\r\n", (bearer_info->apn_val)[index]);
	}
	log_size = sizeof(bearer_info->user_val);
	for(index = 0; index < log_size; ++index){
		SIMATPROXYLOG("user: %c\r\n", (bearer_info->user_val)[index]);
	}
	log_size = sizeof(bearer_info->passwd_val);
	for(index = 0; index < log_size; ++index){
		SIMATPROXYLOG("passwd: %c\r\n", (bearer_info->passwd_val)[index]);
	}
	
    ret = tel_conn_mgr_activate(TEL_CONN_MGR_BEARER_TYPE_NBIOT,
                                TEL_CONN_MGR_SIM_ID_1,
                                bearer_info->pdp_val, // this param need to be aligned with MD side ????
                                bearer_info->apn_val,
                                bearer_info->user_val,
                                bearer_info->passwd_val,
                                simat_proxy_event_queue,
                                &app_id,
                                &activated_pdp_type);


    /* test */
    #ifdef SIMAT_PROXY_INTERNAL_UT_TEST
    {
        tel_conn_mgr_activation_rsp_struct *bearer_active_param = NULL;
        bearer_active_param = pvPortMalloc(sizeof(tel_conn_mgr_activation_rsp_struct));
        bearer_active_param->msg_id = MSG_ID_TEL_CONN_MGR_ACTIVATION_RSP;
        
        memcpy(bearer_active_param->apn, 
               bearer_info->apn_val, 
               sizeof(bearer_info->apn_val));
        
        bearer_active_param->bearer_type = TEL_CONN_MGR_BEARER_TYPE_NBIOT;
        bearer_active_param->cause = 0;
        bearer_active_param->pdp_type = bearer_info->pdp_val;
        bearer_active_param->result = 1;

        SIMATPROXYLOG("tel_conn_mgr_activate\r\n");
        
        if (pdTRUE != xQueueSend(simat_proxy_event_queue, &bearer_active_param, 10)) {
            SIMATPROXYLOG("tel_conn_mgr_activate send: %x\r\n",bearer_active_param);
        }
    }
    #endif

    bearer_info->app_id = app_id;

    SIMATPROXYLOG("tel_conn_mgr_activate ret: %d app_id: %d", ret, app_id);

    if (TEL_CONN_MGR_RET_OK == ret) {
        // bearer connect success, notidy MD side
        process_bearer_confirm(SIMAT_BRING_UP_CNF);
        return;
    } else if ( (TEL_CONN_MGR_RET_OK != ret) && (TEL_CONN_MGR_RET_WOULDBLOCK != ret) ) {
        // bearer activate failure, norify MD side, NetIf Down Indication
        process_bearer_bring_down(FAILED_BRING_UP);
        return;
    }
}


/******************************************************************************
 * FUNCTION
 *  simat_proxy_bring_up_confirm_handler
 * DESCRIPTION
 *  Process message in queue
 * PARAMETERS
 *  message         [in]
 * RETURNS
 *  void
 ******************************************************************************/
void simat_proxy_bring_up_confirm_handler(SIMAT_Base_Header *msg)
{
    SIMATPROXYLOG("simat_proxy_bring_up_confirm_handler\r\n");

    simat_cmux_send(msg, sizeof(SIMAT_Base_Header),
                    SET_SIMAT_MSG_ID_FROM_CMUX(msg->simat_code));
}


/******************************************************************************
 * FUNCTION
 *  simat_proxy_bring_down_request_handler
 * DESCRIPTION
 *  Process message in queue
 * PARAMETERS
 *  message         [in]
 * RETURNS
 *  void
 ******************************************************************************/
void simat_proxy_bring_down_request_handler(SIMAT_Base_Header *msg)
{
    SIMAT_Bring_Down_Req *bring_down_req = (SIMAT_Bring_Down_Req *)msg;
    simat_bearer_info *bearer_info = &(g_socket_management.bearer_info);
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_OK;

    if (1 != bearer_info->valid_flag) {
        //bearer not exist, notify MD side
        SIMATPROXYLOG("process_bearer_bring_down invalid flag\r\n");
        process_bearer_bring_down(FAILED_BRING_DOWN);
        return;
    }

    SIMATPROXYLOG("simat_proxy_bring_down_request_handler app_id: %d", bearer_info->app_id);

    ret = tel_conn_mgr_deactivate(bearer_info->app_id);


    #ifdef SIMAT_PROXY_INTERNAL_UT_TEST
    {
        // UT will return TEL_CONN_MGR_RET_WOULDBLOCK to test bearer callback function
        tel_conn_mgr_deactivation_rsp_struct *bearer_deactive_param = NULL;
        bearer_deactive_param = pvPortMalloc(sizeof(tel_conn_mgr_deactivation_rsp_struct));
        bearer_deactive_param->msg_id = MSG_ID_TEL_CONN_MGR_DEACTIVATION_RSP;
        
        memcpy(bearer_deactive_param->apn, 
               bearer_info->apn_val, 
               sizeof(bearer_info->apn_val));
        
        bearer_deactive_param->bearer_type = TEL_CONN_MGR_BEARER_TYPE_NBIOT;
        bearer_deactive_param->cause = 0;
        bearer_deactive_param->pdp_type = bearer_info->pdp_val;
        bearer_deactive_param->result = 1;

        SIMATPROXYLOG("tel_conn_mgr_deactivate\r\n");
        
        if (pdTRUE != xQueueSend(simat_proxy_event_queue, &bearer_deactive_param, 10)) {
            SIMATPROXYLOG("tel_conn_mgr_activate send: %x\r\n",bearer_deactive_param);
        }
    }
    #endif

    if ( (TEL_CONN_MGR_RET_OK != ret) && (TEL_CONN_MGR_RET_WOULDBLOCK != ret) ) {
        // close bearer error happened, notify MD side
        memset(bearer_info, 0, sizeof(simat_bearer_info));
        process_bearer_bring_down(FAILED_BRING_DOWN);
        return;
    }else if(TEL_CONN_MGR_RET_OK == ret) { //already destructive
        memset(bearer_info, 0, sizeof(simat_bearer_info));
        process_bearer_bring_down(SUCCESSFUL_BRING_DOWN);
    }

}



/******************************************************************************
 * FUNCTION
 *  simat_proxy_bring_down_ind_handler
 * DESCRIPTION
 *  Process message in queue
 * PARAMETERS
 *  message         [in]
 * RETURNS
 *  void
 ******************************************************************************/
void simat_proxy_bring_down_ind_handler(SIMAT_Base_Header *msg)
{
    SIMAT_Bring_Down_Ind *bring_down_ind = (SIMAT_Bring_Down_Ind *)msg;

    SIMATPROXYLOG("simat_proxy_bring_down_ind_handler\r\n");

    simat_cmux_send(msg, sizeof(SIMAT_Bring_Down_Ind),
                    SET_SIMAT_MSG_ID_FROM_CMUX(msg->simat_code));
}



void simat_proxy_socket_send_request_handler(SIMAT_Base_Header *msg)
{
    SIMAT_Send_Data_Req *simat_send_req = (SIMAT_Send_Data_Req *)msg;
    int ret = 0, inner_ret = 0;
    struct conn *soc_conn = NULL;
    int send_total_size = 0;
    int request_socket_id = 0;
    struct sockaddr simat_soc_from;
    int simat_soc_len = sizeof(simat_soc_from);
    int block_index = -1;
    memory_block *mem_block = NULL;
    int index = -1, bytes_written = 0;
    socket_management_node *p_node = NULL;
    struct netbuf *p_netbuf = NULL;
    unsigned char *p_payload = NULL;
    unsigned char send_failure_reason = 0;

    // check socket vailidate
    // socket from MD to AP mapping
    request_socket_id = simat_send_req->soc_base.socket_id;
    soc_conn = simat_socket_to_conn(request_socket_id);
    if (NULL == soc_conn) {
        SIMATPROXYLOG("AP_TO_MD socket conn is wrong %d\r\n", request_socket_id);
        //send socket close ind to simat
        process_socket_close(request_socket_id, SOCKET_NONE_EXIST);
                SIMAT_PROXY_FREE(simat_send_req);
        simat_send_req = NULL;
        return;
    } else {
        // check socket vailidate
        index = get_socket_index_from_manager((void *)request_socket_id, request_type);
    }
	if(-1 == index){
		SIMATPROXYLOG("-1 == index");
        
        SIMAT_PROXY_FREE(simat_send_req);
        simat_send_req = NULL;
		return;
	}

    p_node = &(g_socket_management.management_node[index]);
    SIMATLWIPLOG("@@@@@@%d,%d,%d\n", send_total_size, p_node->status, request_socket_id);
    for (int i = 0; i < 7; i++){
        
        SIMATLWIPLOG("@@@@@@,0x%X, %d\n", p_node->create_conn, p_node->request_socket_id);

    }

    // TCP socket hasn't connect successful
    if ( (SOCKET_CREATE == p_node->status) &&
            (TCP_TYPE == simat_get_socket_type(p_node->socket_type)) ) {
        // store in manage node for request_socket_id
        block_index = simat_socket_get_status_memory(p_node, MD_TO_AP);
		if(-1 == block_index) {
			SIMATPROXYLOG("simat_socket_get_status_memory -1");
            
            SIMAT_PROXY_FREE(simat_send_req);
            simat_send_req = NULL;
			return;
		}
        mem_block = &(p_node->recv_buffer[block_index]);
        //no memory copy need, because cmux memory already allocated by simat proxy
        mem_block->base_address = simat_send_req;
        mem_block->start_address = simat_send_req + sizeof(SIMAT_Soc_Base);
        mem_block->send_size = (ret > 0) ? ret : 0;
        mem_block->total_size = send_total_size;
        SIMATPROXYLOG("send fail because socket status, socket_id:%d", request_socket_id);
        
        SIMAT_PROXY_FREE(simat_send_req);
        simat_send_req = NULL;
        return;
    } else if ( (SOCKET_INIT == p_node->status) || // socket already close or reject to MD, because sending busy
                (MD_TO_AP_SENDING == p_node->sending_status) ) {
        SIMATPROXYLOG("send fail because socket close, socket_id:%d", request_socket_id);
        // vPortFree send memory buffer
        SIMAT_PROXY_FREE(simat_send_req);
        simat_send_req = NULL;
        send_failure_reason = (MD_TO_AP_SENDING == p_node->sending_status) ? SIMAT_PROXY_BUSY : SIMAT_SEND_TRANSIMMSION_FAIL;
        process_send_to_lwip_failure(request_socket_id, (unsigned char)send_failure_reason);
        return;
    }

    p_node->sending_status = MD_TO_AP_SENDING;

    // prepare buffer send to lwip
    send_total_size = simat_send_req->soc_base.base_header.simat_length - sizeof(SIMAT_Soc_Base);

    //send to lwip
    if (UDP_TYPE == simat_get_socket_type(p_node->socket_type)) {
        
        SIMATLWIPLOG("state:%d\n", net_status);
        if (net_status == NB_NETWORK_AVAILABLE) {      
            p_netbuf = netbuf_new();
            if (NULL == p_netbuf) {
                SIMATLWIPLOG("netbuf_new allocate fail\n");
                configASSERT(0); 
    			return;
            }
            p_payload = (unsigned char *)netbuf_alloc(p_netbuf, send_total_size);
            if (NULL == p_payload) {
                SIMATLWIPLOG("netbuf_new allocate fail\n");
                configASSERT(0);
    			return;
            }
            memcpy(p_payload, simat_send_req->data, send_total_size);
            ret = netconn_send(p_node->create_conn, p_netbuf);
            bytes_written = ret;
            // free udp buffer
            netbuf_delete(p_netbuf);
            p_netbuf = NULL;
        } else {

          // oom need send reject
          SIMAT_PROXY_FREE(simat_send_req);
          simat_send_req = NULL;
          process_send_to_lwip_failure(request_socket_id, (unsigned char)SIMAT_PROXY_BUSY);
          g_socket_id[index] = request_socket_id;
          
          SIMATLWIPLOG("state:%d,%d\n", index, request_socket_id);
          return;
        }
    } else if (TCP_TYPE == simat_get_socket_type(p_node->socket_type)) {
        SIMATLWIPLOG("@@@%d\n", send_total_size);
        ret = netconn_write_partly(p_node->create_conn,
                                   simat_send_req->data,
                                   send_total_size,
                                   NETCONN_COPY,
                                   &bytes_written);  //????? last parameter should be?????
    }

    if (ERR_OK != ret) {
        SIMATLWIPLOG("netconn send error sock type:%d, ret: %d",
                     p_node->socket_type, ret);
        if ( (ERR_MEM == ret) || (ERR_BUF == ret) ) {
            SIMATMEMLOG("send memory is oom");
            configASSERT(0);// oom case in send data process, need send reject according to spec
			return;
        }
    }

    SIMATPROXYLOG("send ret:%d,t:%d,s:%d,size:%d\r\n",
                  ret, send_total_size, request_socket_id, bytes_written);

    // send complete
    if ( (bytes_written == send_total_size) ||
          ((ERR_OK == ret) && 
           (UDP_TYPE == simat_get_socket_type(p_node->socket_type)))) {
        // vPortFree send memory buffer
        SIMAT_PROXY_FREE(simat_send_req);
        simat_send_req = NULL;
        p_node->sending_status = MD_TO_AP_CONFIRM;
        // send AP to MD to notify send complete
        process_send_to_lwip_success(request_socket_id);
    } else {	// partial or not send
        // UDP wil not have this case
        // store in manage node for request_socket_id
        block_index = simat_socket_get_status_memory(&(g_socket_management.management_node[index]), MD_TO_AP);
		if(0 >= block_index) {
			SIMATPROXYLOG("block_index:%d\n", block_index);
		}else{
			mem_block = &(g_socket_management.management_node[index].recv_buffer[block_index]);
        	//no memory copy need, because cmux memory already allocated by simat proxy
        	mem_block->base_address = simat_send_req;
        	mem_block->start_address = simat_send_req + sizeof(SIMAT_Soc_Base);
        	mem_block->send_size = (bytes_written > 0) ? bytes_written : 0;
        	mem_block->total_size = send_total_size;
		}

        if ( (ERR_MEM == ret) || (ERR_WOULDBLOCK == ret) ) {
            SIMATPROXYLOG("socket send error id:%d, reason:%d", request_socket_id, ret);
            process_send_to_lwip_failure(request_socket_id, (unsigned char)SIMAT_SEND_TRANSIMMSION_FAIL);
        } else if ( (ERR_ABRT == ret) || (ERR_CLSD == ret) ) {
            PROCESS_CONNECT_RESULT(request_socket_id,  (unsigned char)SOCKET_PASSIVE_DESTRUCTION);
        }
    }

}


void simat_proxy_socket_send_reject_handler(SIMAT_Base_Header *msg)
{
    SIMAT_Send_Data_Reject *simat_send_reject_msg = (SIMAT_Send_Data_Reject *)msg;

    simat_cmux_send(simat_send_reject_msg, sizeof(SIMAT_Send_Data_Reject),
                    SET_SIMAT_MSG_ID_FROM_CMUX(SIMAT_SOCKET_SEND_REJ));
}


void simat_proxy_socket_close_request_handler(SIMAT_Base_Header *msg)
{
    SIMAT_Close_Req *simat_socket_close_req = (SIMAT_Close_Req *)msg;
    
    SIMATPROXYLOG("simat_proxy_socket_close_request_handler :%d\r\n",
                  simat_socket_close_req->socket_id);
    
    PROCESS_CONNECT_RESULT(simat_socket_close_req->socket_id,
                           SOCKET_SUCCESSFUL_CLOSE);
}

void simat_proxy_socket_send_confirm_handler(SIMAT_Base_Header *msg)
{
    simat_cmux_send(msg, sizeof(SIMAT_Send_Data_Conf),
                    SET_SIMAT_MSG_ID_FROM_CMUX(SIMAT_SOCKET_SEND_CNF));
}


void simat_proxy_socket_recv_ind_handler(SIMAT_Base_Header *msg)
{
    SIMAT_Recv_Data_Ind *simat_recv_msg = (SIMAT_Recv_Data_Ind *)msg;

    unsigned int socket_management_index = -1, user_data = 0;

    // because need to free relative buffer from socket management, so
    socket_management_index = get_socket_index_from_manager(
                                  simat_recv_msg->soc_base.socket_id,
                                  request_type);

    // because must free first block for that socket. so block_index is zero
    user_data = SET_SIMAT_SOCK_MANAGEMENT_FROM_CMUX(
                    SIMAT_SOCKET_RECV_IND,
                    socket_management_index,
                    0);

    // simat_recv_msg allocate by continues memory
    // and length shouldn't be sizeof(SIMAT_Recv_Data_Ind) because data length need to be inclueded.
    simat_cmux_send(simat_recv_msg,
                    simat_recv_msg->soc_base.base_header.simat_length,
                    user_data);

#ifdef SIMAT_PROXY_INTERNAL_UT_TEST
    simat_ut_recv_data_rsp(simat_recv_msg->soc_base.socket_id);
#endif
}


void simat_proxy_socket_recv_confirm_handler(SIMAT_Base_Header *msg)
{
    SIMAT_Recv_Data_Rsp *simat_recv_rsp = (SIMAT_Recv_Data_Rsp *)msg;
    socket_management_node *socket_management_node = NULL;
    memory_block *tmp_soc_block = NULL, *soc_block = NULL;
    int index = get_socket_index_from_manager(simat_recv_rsp->socket_id, request_type);
    int temp_index = 0;
    int sock_id = simat_recv_rsp->socket_id;


    SIMATPROXYLOG("simat_proxy_socket_recv_confirm_handler invoked %d\r\n", index);

    // vPortFree send memory buffer
    SIMAT_PROXY_FREE((void *)simat_recv_rsp);
    simat_recv_rsp = NULL;

    if (-1 != index) {
        // reset g_socket_management.management_node
        socket_management_node = &(g_socket_management.management_node[index]);
        soc_block = &(socket_management_node->recv_buffer[temp_index]);

        tmp_soc_block = soc_block;
        

        // stack behaviour FIFO, LEFT N-1 go ahead
        for (temp_index = 1; temp_index < MAX_SOCKET_MEMORY_BLOCK_NUM; ++temp_index) {
            tmp_soc_block += temp_index;
            if ( NULL != tmp_soc_block->base_address) {
                memcpy(soc_block, tmp_soc_block, sizeof(memory_block));
            }
            soc_block = tmp_soc_block;
        }
    } else {	// send to MD, socket closed indication because wrong socket id
        SIMATPROXYLOG("simat_proxy_socket_recv_confirm_handler wrong socket_id:%d \r\n", sock_id);
        process_socket_close(sock_id, SOCKET_NONE_EXIST);
        return;
    }

    // need to check if some recv data from netwotk has not send to MD
    if (1 == socket_management_node->left_recving_flag) {
        SIMATPROXYLOG("process recv left buffer sock: %d, netaddr: %x\r\n",
                      socket_management_node->request_socket_id,
                      socket_management_node->left_recving_netbuf);
        process_socket_left_recving_data(socket_management_node);
    } else if (1 == socket_management_node->incoming_recving_flag) {
        SIMATPROXYLOG("process_socket_recving_data again\r\n");
        process_socket_recving_data(socket_management_node);
    } else {
        //test code
#ifdef SIMAT_PROXY_INTERNAL_UT_TEST
        simat_ut_close_rsp(sock_id);        
#endif /* SIMAT_PROXY_INTERNAL_UT_TEST */
    }
    socket_management_node->recving_status = AP_TO_MD_INIT;
    socket_management_node->incoming_recving_flag = 0;
    
    SIMATPROXYLOG("simat_proxy_socket_recv_confirm_handler done!\r\n");
}



/******************************************************************************
 * FUNCTION
 *  simat_proxy_cmux_cb
 * DESCRIPTION
 *  Process simat proxy interaction with modem sim side
 * PARAMETERS
 * RETURNS
 *  void
 *******************************************************************************/
void simat_proxy_cmux_cb(mux_ap_event_t event, void *param)
{
    if (NULL == param) {
        SIMATPROXYLOG("simat_proxy_cmux_cb param null\r\n");
        return;
    }

    switch (event) {
        case MUX_AP_EVENT_CHANNEL_ENABLED:
            g_simat_cmux_status.simat_cmux_status = SIMAT_CMUX_CONNECT;
            g_simat_cmux_status.channel_id = ((mux_ap_event_channel_enabled_t *)param)->channel_id;
            SIMATPROXYLOG("cmux created success id: %d\r\n", g_simat_cmux_status.channel_id);
            break;
        case MUX_AP_EVENT_CHANNEL_DISABLED:	// this case shouldn't happen in normal case. if happened, should recover????
            if (((mux_ap_event_channel_disabled_t *)param)->channel_id == g_simat_cmux_status.channel_id) {
                g_simat_cmux_status.simat_cmux_status = SIMAT_CMUX_DISCONNECT;
                SIMATPROXYLOG("cmux disconnect id:%d\r\n", g_simat_cmux_status.channel_id);
            }
            break;
        case MUX_AP_EVENT_SEND_COMPLETED: {
            mux_ap_event_send_completed_t *p_data = (mux_ap_event_send_completed_t *)(param);
            int index = 0;
            int user_data = (unsigned int)p_data->user_data;
            int msg_id = 0;
            SIMAT_Base_Header *p_simat_data = NULL;
            memory_block *p_recv_data = NULL, *p_send_data = NULL;
            
            msg_id = GET_SIMAT_MSG_ID_FROM_CMUX(user_data);
            if (SIMAT_SOCKET_RECV_IND == msg_id) {
                // reset data for socket management
                int sock_management_index = GET_SIMAT_SOCK_MANAGEMENT_INDEX_FROM_CMUX(user_data);
                int block_mem_index = GET_SIMAT_SOCK_MEMORY_BLOCK_INDEX_FROM_CMUX(user_data);
                memory_block *p_block = &(g_socket_management.management_node[sock_management_index].recv_buffer[block_mem_index]);
                if (p_block->start_address != p_data->data_buffer) {
                    SIMATPROXYLOG("memory address for SIMAT_SOCKET_RECV_IND error base:%x,cmux:%x",
                                  p_block->start_address, p_data->data_buffer);
                    configASSERT(0); //real assert, simat_proxy internal reason
                }
            }

            if (NULL != p_data->data_buffer) {
                SIMAT_PROXY_FREE(p_data->data_buffer);
                p_data->data_buffer = NULL;
            }
            SIMATPROXYLOG("cmux send data completed\r\n");
        }
        break;
        case MUX_AP_EVENT_PREPARE_TO_RECEIVE: {
            mux_ap_event_prepare_to_receive_t *p_data = (mux_ap_event_prepare_to_receive_t *)(param);
            // need to prepare buffer for cmux to fill data
            int cmux_req_length = p_data->buffer_length;
            p_data->data_buffer = (unsigned char *)SIMAT_PROXY_SIMLPE_ALLOCATE(sizeof(unsigned char) * cmux_req_length);
            if (NULL == p_data->data_buffer) {
                SIMATPROXYLOG("SIMAT Proxy prepare recv has no buffer\r\n");
            }
        }
        break;
        case MUX_AP_EVENT_RECEIVE_COMPLETED: {
            mux_ap_event_receive_completed_t *p_data = (mux_ap_event_receive_completed_t *)(param);
            if (NULL == p_data->data_buffer) {
                return;
            }
            SIMAT_Base_Header *simat_header = NULL;
            simat_header = (SIMAT_Base_Header *)p_data->data_buffer;

            SIMATPROXYLOG("simat_header: %d,%d\r\n",simat_header->simat_code,
                simat_header->simat_length);

            if(SIMAT_CODE_REJ == simat_header->simat_code) {
                SIMATPROXYLOG("SIMAT_CODE_REJ recvd from MD\r\n");
                simat_proxy_free_simple_internal(simat_header);
                simat_header = NULL;
                return;
            }

            if ((SIMAT_CODE_START >= simat_header->simat_code) ||
                (SIMAT_CODE_END <= simat_header->simat_code)) {
                SIMATPROXYLOG("simat_proxy msg_id wrong: %d\n", simat_header->simat_code);
                // release allocate buffer
                unsigned short invalide_code = simat_header->simat_code;
                simat_proxy_free_simple_internal(simat_header);
                simat_header = NULL;
                // re-new buffer to response SIMAT to reject command
                {
                    SIMAT_Cmd_Reject *p_simat_reject_data = NULL;
                    p_simat_reject_data = (SIMAT_Cmd_Reject *)SIMAT_PROXY_ALLOCATE(
                                                        sizeof(SIMAT_Cmd_Reject), 
                                                        SIMAT_CODE_REJ);  
                    p_simat_reject_data->base_header.simat_code = SIMAT_CODE_REJ;
                    p_simat_reject_data->base_header.simat_length = sizeof(SIMAT_Cmd_Reject);
                    p_simat_reject_data->reject_code = invalide_code;;
                    simat_header = (SIMAT_Base_Header *)p_simat_reject_data;
                }       
            }else {
                simat_set_mem_msg_id(simat_header, simat_header->simat_code);
            }

            // send msg to simat_modem_task to process
            simat_send_event_internal(simat_header->simat_code, simat_header);
        }
        break;
        default:
            break;
    }
}


/******************************************************************************
 * FUNCTION
 *  ui_task_msg_handler
 * DESCRIPTION
 *  Process message in queue
 * PARAMETERS
 *  message         [in]
 * RETURNS
 *  void
 *****************************************************************************/
void simat_proxy_modem_msg_handler(simat_proxy_modem_message_struct *msg)
{
    if (NULL != msg) {

        SIMATPROXYLOG("simat_proxy_modem_msg_handler id: %d \n",msg->message_id);

        if ((SIMAT_PROXY_LWIP_SEND_INTERNAL_MSG <= msg->message_id) && 
            (msg->message_id <= SIMAT_PROXY_LWIP_ERROR_INTERNAL_MSG)) {
            simat_proxy_lwip_callback_internal(msg);

        } else if ((SIMAT_CODE_START < msg->message_id) &&
                (SIMAT_CODE_END > msg->message_id)) {
            SIMATPROXYLOG("simat_proxy_modem_msg_handler, message_id: %d, %s\n",
                          simat_proxy_process_table[msg->message_id].msg_name,
                          simat_proxy_process_table[msg->message_id].msg_string);
            simat_proxy_process_table[msg->message_id].msg_handler((SIMAT_Base_Header *)msg->param);
        } else {
            simat_proxy_bearer_msg_process(msg);
        }
        vPortFree(msg);
        msg = NULL;
    } else {
        SIMATPROXYLOG("simat_proxy_modem_msg_handler msg NULL");
    }
}


/******************************************************************************
 * FUNCTION
 *  simat_proxy_bearer_msg_process
 * DESCRIPTION
 *  Process message in queue
 * PARAMETERS
 *  message         [in]
 * RETURNS
 *  void
 *******************************************************************************/
void simat_proxy_bearer_msg_process(simat_proxy_modem_message_struct *msg)
{
    SIMATPROXYLOG("simat_proxy_bearer_msg_process message_id: %d\r\n", msg->message_id);

    switch (msg->message_id) {
        case MSG_ID_TEL_CONN_MGR_ACTIVATION_RSP: {
            tel_conn_mgr_activation_rsp_struct *bearer_active_msg = (tel_conn_mgr_activation_rsp_struct *)msg;
            unsigned int index = 0;
            SIMATPROXYLOG("bearer active: %x, result: %d, app_id: %d, cause: %d\r\n", 
                           bearer_active_msg,
                           bearer_active_msg->result,
                           bearer_active_msg->app_id,
                           bearer_active_msg->cause);
            if (1 == bearer_active_msg->result) {
                SIMATPROXYLOG("bearer active success app_id: %d\r\n", g_socket_management.bearer_info.app_id);
                // notify MD bearer has established
                process_bearer_confirm(SIMAT_BRING_UP_CNF);
                for (index = 0; index < TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM; ++index) {

                    //SIMATPROXYLOG("MSG_ID_TEL_CONN_MGR_ACTIVATION_RSP success app_id, %d\r\n",
                                  //bearer_active_msg->app_id);
                }
            } else {
                // notify bearer established failed
                SIMATPROXYLOG("bearer active failed app_id: %d\r\n", g_socket_management.bearer_info.app_id);
                process_bearer_bring_down(FAILED_BRING_UP);
            }
        }
        break;
        case MSG_ID_TEL_CONN_MGR_DEACTIVATION_RSP: {
            tel_conn_mgr_deactivation_rsp_struct *bearer_deactive_msg = (tel_conn_mgr_deactivation_rsp_struct *)msg;
            if (1 == bearer_deactive_msg->result) {
                g_socket_management.bearer_info.valid_flag = 0;
                SIMATPROXYLOG("bearer deactive success app_id: %d\r\n", g_socket_management.bearer_info.app_id);
                // notify MD bearer destruct successfully.
                process_bearer_bring_down(SUCCESSFUL_BRING_DOWN);
            } else {
                SIMATPROXYLOG("bearer deactive failed app_id: %d\r\n", g_socket_management.bearer_info.app_id);
                // notify bearer destruct failed
                process_bearer_bring_down(FAILED_BRING_UP);
            }
        }
        break;
        case MSG_ID_TEL_CONN_MGR_DEACTIVATION_IND: {
            tel_conn_mgr_deactivation_ind_struct *bearer_deactive_ind = (tel_conn_mgr_deactivation_ind_struct *)(msg->param);
            g_socket_management.bearer_info.valid_flag = 0;
            SIMATPROXYLOG("MSG_ID_TEL_CONN_MGR_DEACTIVATION_IND\r\n");
            // notify MD bearer destruct passively.
            process_bearer_bring_down(PASSIVE_NETWORK_DESTRUCTION);
        }
        break;
        default:
            SIMATPROXYLOG("simat_proxy_bearer_msg_process id: %id wrong\r\n", msg->message_id);
            break;
    }
}



/******************************************************************************
 * FUNCTION
 *  simat_proxy_modem_main_loop
 * DESCRIPTION
 *  simat_proxy_modem_main_loop
 * PARAMETERS
 *  void *arg
 * RETURNS
 *  void
 *******************************************************************************/
void simat_proxy_modem_main_loop(void *arg)
{
    simat_proxy_modem_message_struct *queue_item = NULL;
    while (1) {
        if (xQueueReceive(simat_proxy_event_queue, &queue_item, portMAX_DELAY)) {
            simat_proxy_modem_msg_handler(queue_item);
        }
    }
}

void simat_proxy_netif_network_status_callback(nb_netif_status_t status)
{
    socket_management_node *p_node = NULL;
    int index = -1;

    SIMATPROXYLOG("netif_network cb: %d\r\n", status);
    switch (status) {
        case NB_NETWORK_BUSY:
            net_status = NB_NETWORK_BUSY;
        break;
        case NB_NETWORK_AVAILABLE:
            for(int i=0; i< MAX_SIMAT_SOCKET_NUM; i++) {
                if (g_socket_id[i] != -1) {
                    index = get_socket_index_from_manager((void *)g_socket_id[i], request_type);
                    
                    if(-1 == index){
                            SIMATPROXYLOG("-1 == index");
                            break;
                        } 
                        p_node = &(g_socket_management.management_node[index]);
                        
                        SIMATPROXYLOG("%d,%d,%d\r\n", p_node->sending_status, index, g_socket_id[i]);
                        p_node->sending_status = MD_TO_AP_CONFIRM;
                        // send AP to MD to notify send complete
                        process_send_to_lwip_success(g_socket_id[i]);
                        g_socket_id[i] = -1;

                }
            }
            net_status = NB_NETWORK_AVAILABLE;

        break;
        default:
        break;

    }
}


/******************************************************************************
 * FUNCTION
 *  simat_proxy_init
 * DESCRIPTION
 *  simat_proxy_init
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *******************************************************************************/
void simat_proxy_init()
{
    mux_ap_status_t ret = MUX_AP_STATUS_OK;
    
    bool result = true;
    g_simat_cmux_status.simat_cmux_status = SIMAT_CMUX_UNITIALIAZED;
    TaskHandle_t task_handle = NULL;

    // register CMUX
    ret = mux_ap_register_callback(MUX_AP_CHANNEL_TYPE_SIMAT, simat_proxy_cmux_cb, NULL);
    if (MUX_AP_STATUS_OK != ret) {
        SIMATPROXYLOG("cmux_register_callback error: %d\r\n", ret);
        return;
    }
    result = nb_netif_register_network_status_indication(simat_proxy_netif_network_status_callback);
    for (int i = 0; i < MAX_SIMAT_SOCKET_NUM; i++) {
        g_socket_id[i] = -1;
    }
    if (true != result) {
            SIMATPROXYLOG("network_status_register_callback error: %d\r\n", result);
            return;
     }

    // create task queue
    simat_proxy_event_queue = xQueueCreate(SIMAT_PROXY_MODEM_TASK_QUEUE_SIZE,
                                           sizeof(simat_proxy_modem_message_struct *) );

    if (pdPASS != xTaskCreate(simat_proxy_modem_main_loop,
                              SIMAT_PROXY_MODEM_TASK_NAME,
                              SIMAT_PROXY_MODEM_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                              NULL,
                              SIMAT_PROXY_MODEM_TASK_PRIORITY,
                              &task_handle)) {
        SIMATPROXYLOG("SIMAT Task create failured\r\n");
        return;
    }
}

/////////////////////////////////////////////////////////////////////////////////////

//simat_lwip_task:

void simat_proxy_lwip_callback(struct netconn *conn, enum netconn_evt evt, u16_t len) 
{
    
    unsigned int msg_id;
    simat_proxy_modem_message_struct *simat_proxy_message = NULL;
    simat_proxy_message = pvPortMalloc(sizeof(simat_proxy_modem_message_struct) * sizeof(unsigned char));
    
    SIMATLWIPLOG("^^^^^^@@@@@@@@@evt: %d\r\n",evt);  
    if (evt == NETCONN_EVT_SENDPLUS) {
        msg_id = SIMAT_PROXY_LWIP_SEND_INTERNAL_MSG;

    } else if (evt == NETCONN_EVT_RCVPLUS) {
        msg_id = SIMAT_PROXY_LWIP_RECEIVE_INTERNAL_MSG;

    } else if (evt == NETCONN_EVT_ERROR) {
     SIMATLWIPLOG("^^^^^^@@@@@@@@@error: %d\r\n",conn->last_err);  
        msg_id = SIMAT_PROXY_LWIP_ERROR_INTERNAL_MSG;

    } else {
    
    SIMATLWIPLOG("[10.31]return!!!\r\n");  
        return;
    }
    simat_proxy_message->message_id = msg_id;
    simat_proxy_message->param = (void *)conn;
    if (pdTRUE != xQueueSend(simat_proxy_event_queue, &simat_proxy_message, 10)) {
        SIMATPROXYLOG("simat_send_event_internal queue full\r\n");
    }


}


/******************************************************************************
 * FUNCTION
 *  simat_proxy_lwip_callback
 * DESCRIPTION
 *  simat_proxy_lwip_callback
 * PARAMETERS
 *  struct netconn *conn, enum netconn_evt evt, u16_t len
 * RETURNS
 *  void
 *******************************************************************************/
void simat_proxy_lwip_callback_internal(simat_proxy_modem_message_struct *msg)//struct netconn *conn, enum netconn_evt evt, u16_t len) 
{

	socket_management_node *soc_node = NULL;
    struct netconn * conn = (struct netconn *)msg->param;
    int index = get_socket_index_from_manager((void *)conn, real_type);
	
	if(-1 != index){
    	soc_node = &(g_socket_management.management_node[index]);
	}else{
		SIMATLWIPLOG("no soc_node have found");
		return;
	}

    SIMATLWIPLOG("lwip_callback evt: %d index:%d  ava: %d  addr: %x  conn_state: %d\r\n", 
                  msg->message_id, index, soc_node->status, conn, conn->state);

    switch (msg->message_id)
    {
        case SIMAT_PROXY_LWIP_SEND_INTERNAL_MSG: {
            if ((SOCKET_CREATE == soc_node->status) &&
                (TCP_TYPE == simat_get_socket_type(soc_node->socket_type))) {
                // connect case only for TCP, UDP not happened
                soc_node->status = SOCKET_CONNECT;
                SIMATLWIPLOG("simat_proxy_lwip_callback connect success type: %d\r\n",
                             simat_get_socket_type(soc_node->socket_type));
                // send to SIMAT MD side
                process_connect_success(soc_node->request_socket_id);
            } else if( (SOCKET_CONNECT == soc_node->status) ||
                        ((SOCKET_CREATE == soc_node->status) &&
                        (UDP_TYPE == simat_get_socket_type(soc_node->socket_type))) ){
                SIMATLWIPLOG("process_left_socket_sending_data enter\r\n");
                // check lwip no-blocking flags
                if (!(conn->flags & NETCONN_FLAG_CHECK_WRITESPACE)) {
                    process_left_socket_sending_data(soc_node);
                } else {
                    // lwip internal bug occured
                    configASSERT(0); //could remove? can't happen? need lwip check
                }
            } else {
                SIMATLWIPLOG("send abnormal case happened \r\n");
            }
        }
        break;
        case SIMAT_PROXY_LWIP_RECEIVE_INTERNAL_MSG: {
            if ( (SOCKET_CONNECT == soc_node->status) ||
                   ((SOCKET_CREATE == soc_node->status) &&
                    (UDP_TYPE == simat_get_socket_type(soc_node->socket_type)))) {
                if (AP_TO_MD_SENDING == soc_node->recving_status) {
                    SIMATLWIPLOG("AP_TO_MD_SENDING status\r\n");
                    soc_node->incoming_recving_flag = 1;
                } else {
                    SIMATLWIPLOG("process NETCONN_EVT_RCVPLUS status\r\n");                
                    process_socket_recving_data(soc_node);
                }
            } else {
                SIMATLWIPLOG("recv abnormal case happened \r\n");
            }
        }
        break;
        case SIMAT_PROXY_LWIP_ERROR_INTERNAL_MSG: {
            
            SIMATLWIPLOG("^^^^^^error: %d\r\n",soc_node->create_conn->last_err);  
             if ( (SOCKET_CONNECT == soc_node->status) ||
                   ((SOCKET_CREATE == soc_node->status) &&
                    (UDP_TYPE == simat_get_socket_type(soc_node->socket_type))) ) {
                PROCESS_CONNECT_RESULT(soc_node->request_socket_id, SOCKET_NETWORK_ERROR);
                SIMATLWIPLOG("NETCONN_EVT_ERROR error recv from soceket id: %d", soc_node->request_socket_id);
            } else {
                soc_node->status = SOCKET_INIT;
                SIMATLWIPLOG("error abnormal case happened \r\n");
            }
        }
        break;
        default:
            break;
    }
}



/******************************************************************************
 * FUNCTION
 *  process_connect_success
 * DESCRIPTION
 *  process_connect_success
 * PARAMETERS
 *  int request_socket_id
 * RETURNS
 *  void
 *******************************************************************************/
void process_connect_success(int request_socket_id)
{
    SIMAT_Open_Conf *param = NULL;
    // send msg to simat_proxy_modem_main_loop task
    param = (SIMAT_Open_Conf *)SIMAT_PROXY_ALLOCATE(sizeof(SIMAT_Open_Conf), SIMAT_SOCKET_OPEN_CNF);
    if (NULL == param) {
        SIMATPROXYLOG("msg allocate memory error NULL\r\n");
        return;
    }
    SIMATPROXYLOG("process_connect_success socket_id: %d\r\n", request_socket_id);
    param->base_header.simat_code = SIMAT_SOCKET_OPEN_CNF;
    param->base_header.simat_length = sizeof(SIMAT_Open_Conf);
    param->socket_id = request_socket_id;
    simat_send_event_internal(SIMAT_SOCKET_OPEN_CNF, param);

#ifdef SIMAT_PROXY_INTERNAL_UT_TEST
    simat_ut_send_data_req(request_socket_id);
#endif
}


/******************************************************************************
 * FUNCTION
 *  process_bearer_confirm
 * DESCRIPTION
 *  process_bearer_confirm
 * PARAMETERS
 *  unsigned int bearer_msg_type
 * RETURNS
 *  void
 *******************************************************************************/
void process_bearer_confirm(unsigned int bearer_msg_type)
{
    SIMAT_Base_Header *param = NULL;
    // send msg to simat_proxy_modem_main_loop task
    param = (SIMAT_Base_Header *)SIMAT_PROXY_ALLOCATE(sizeof(SIMAT_Base_Header), bearer_msg_type);
    if (NULL == param) {
        SIMATPROXYLOG("process_bearer_confirm error NULL\r\n");
        return;
    }

    SIMATPROXYLOG("process_bearer_confirm type:%d \r\n", bearer_msg_type);
    param->simat_code = bearer_msg_type;
    param->simat_length = sizeof(SIMAT_Base_Header);

    simat_send_event_internal(bearer_msg_type, param);
}


/******************************************************************************
 * FUNCTION
 *  process_bearer_config_reject
 * DESCRIPTION
 *  process_bearer_confirm
 * PARAMETERS
 *  unsigned int bearer_msg_type
 * RETURNS
 *  void
 *******************************************************************************/
void process_bearer_config_reject(unsigned char bearer_reject_reason)
{
    SIMAT_Config_Reject *param = NULL;
    // send msg to simat_proxy_modem_main_loop task
    param = (SIMAT_Config_Reject *)SIMAT_PROXY_ALLOCATE(sizeof(SIMAT_Config_Reject), SIMAT_CONFIG_REJ);
    if (NULL == param) {
        SIMATPROXYLOG("msg allocate memory error NULL\r\n");
        return;
    }

    SIMATPROXYLOG("process_config_reject reason: %d\r\n", bearer_reject_reason);
    param->base_header.simat_code = SIMAT_CONFIG_REJ;
    param->base_header.simat_length = 5;
    param->cause = bearer_reject_reason;
    simat_send_event_internal(SIMAT_CONFIG_REJ, param);
}


/******************************************************************************
 * FUNCTION
 *  process_bearer_bring_down
 * DESCRIPTION
 *  process_bearer_bring_down
 * PARAMETERS
 * unsigned char bearer_bring_down_reason
 * RETURNS
 *  void
 *******************************************************************************/
void process_bearer_bring_down(unsigned char bearer_bring_down_reason)
{
    SIMAT_Bring_Down_Ind *param = NULL;
    // send msg to simat_proxy_modem_main_loop task
    param = (SIMAT_Bring_Down_Ind *)SIMAT_PROXY_ALLOCATE(sizeof(SIMAT_Bring_Down_Ind), SIMAT_BRING_DOWN_IND);
    if (NULL == param) {
        SIMATPROXYLOG("process_bearer_bring_down error NULL\r\n");
        return;
    }

    SIMATPROXYLOG("process_bearer_bring_down \r\n");
    param->base_header.simat_code = SIMAT_BRING_DOWN_IND;
    param->base_header.simat_length = sizeof(SIMAT_Bring_Down_Ind);
    param->cause = bearer_bring_down_reason;;
    simat_send_event_internal(SIMAT_BRING_DOWN_IND, param);
}



/******************************************************************************
 * FUNCTION
 *  process_connect_result
 * DESCRIPTION
 *  process_connect_result
 * PARAMETERS
 * int request_socket_id, unsigned char reason, int line
 * RETURNS
 *  void
 *******************************************************************************/
void process_connect_result(int request_socket_id, unsigned char reason, int line)
{
    SIMAT_Close_Ind *param = NULL;
    int sock_management_index = -1;
    socket_management_node *p_node = NULL;

    SIMATPROXYLOG("PROCESS_CONNECT_RESULT :%d  %d   %d\r\n",
                  request_socket_id, reason, line);

    if(SOCKET_NONE_EXIST != reason) {
        sock_management_index = get_socket_index_from_manager((void *)request_socket_id, request_type);

        if ( (0 > sock_management_index) ||
            (MAX_SIMAT_SOCKET_NUM <= sock_management_index) ) {
            SIMATPROXYLOG("close_socket_data invalidate socket_management_index %d", sock_management_index);
            return;
        }
    
        close_socket_data(sock_management_index);
    }

    simat_print_socket_management_info();

    
    // send msg to simat_proxy_modem_main_loop task
    param = (SIMAT_Close_Ind *)SIMAT_PROXY_ALLOCATE(sizeof(SIMAT_Close_Ind), SIMAT_SOCKET_CLOSE_IND);
    if (NULL == param) {
        SIMATPROXYLOG("msg allocate memory error NULL\r\n");
        return;
    }
    SIMATPROXYLOG("PROCESS_CONNECT_RESULT socket_id: %d reason: %d\r\n", request_socket_id, reason);
    param->soc_base.base_header.simat_code = SIMAT_SOCKET_CLOSE_IND;
    param->soc_base.base_header.simat_length = sizeof(SIMAT_Close_Ind);
    param->soc_base.socket_id = request_socket_id;
    param->cause = reason;
    simat_send_event_internal(SIMAT_SOCKET_CLOSE_IND, param);
}


/******************************************************************************
 * FUNCTION
 *  process_send_to_lwip_success
 * DESCRIPTION
 *  process_send_to_lwip_success
 * PARAMETERS
 * int request_socket
 * RETURNS
 *  void
 *******************************************************************************/
void process_send_to_lwip_success(int request_socket)
{
    SIMAT_Send_Data_Conf *param = NULL;
    // send msg to simat_proxy_modem_main_loop task
    param = (SIMAT_Send_Data_Conf *)SIMAT_PROXY_ALLOCATE(sizeof(SIMAT_Send_Data_Conf), SIMAT_SOCKET_SEND_CNF);
    if (NULL == param) {
        SIMATPROXYLOG("msg allocate memory error NULL\r\n");
        return;
    }
    SIMATPROXYLOG("simat_send_to_lwip_success request_socket_id:%d\r\n", request_socket);
    param->base_header.simat_code = SIMAT_SOCKET_SEND_CNF;
    param->base_header.simat_length = sizeof(SIMAT_Send_Data_Conf);
    param->socket_id = request_socket;
    simat_send_event_internal(SIMAT_SOCKET_SEND_CNF, param);
}



/******************************************************************************
 * FUNCTION
 *  process_send_to_lwip_failure
 * DESCRIPTION
 *  process_send_to_lwip_failure
 * PARAMETERS
 * int request_socket, unsigned char cause
 * RETURNS
 *  void
 *******************************************************************************/
void process_send_to_lwip_failure(int request_socket, unsigned char cause)
{
    SIMAT_Send_Data_Reject *param = NULL;
    // send msg to simat_proxy_modem_main_loop task
    param = (SIMAT_Send_Data_Reject *)SIMAT_PROXY_ALLOCATE(sizeof(SIMAT_Send_Data_Reject), SIMAT_SOCKET_SEND_REJ);
    if (NULL == param) {
        SIMATPROXYLOG("msg allocate memory error NULL\r\n");
        return;
    }
    SIMATPROXYLOG("process_send_to_lwip_failure request_socket_id:%d\r\n", request_socket);
    param->soc_base.base_header.simat_code = SIMAT_SOCKET_SEND_REJ;
    param->soc_base.base_header.simat_length = sizeof(SIMAT_Send_Data_Reject);
    param->soc_base.socket_id = request_socket;
    param->cause = cause;
    simat_send_event_internal(SIMAT_SOCKET_SEND_REJ, param);
}



/******************************************************************************
 * FUNCTION
 *  process_send_to_recv_ind
 * DESCRIPTION
 *  process_send_to_recv_ind
 * PARAMETERS
 * SIMAT_Data_Req *simat_data_request
 * RETURNS
 *  void
 *******************************************************************************/
void process_send_to_recv_ind(SIMAT_Data_Req *simat_data_request)
{
    //already allocate memory as below
    simat_send_event_internal(SIMAT_SOCKET_RECV_IND, (SIMAT_Base_Header *)simat_data_request);
}



/******************************************************************************
 * FUNCTION
 *  process_left_socket_sending_data
 * DESCRIPTION
 *  process_left_socket_sending_data
 * PARAMETERS
 * socket_management_node *simat_soc_info
 * RETURNS
 *  void
 *******************************************************************************/
void process_left_socket_sending_data(socket_management_node *simat_soc_info)
{
    int inner_index = -1, ret = 0;
    memory_block *soc_block = &(simat_soc_info->send_buffer[0]), *tmp_soc_block = soc_block;
    struct sockaddr simat_soc_from;
    int simat_soc_len = sizeof(simat_soc_from);
    unsigned int bytes_written = 0;

    if (NULL != soc_block->start_address) {
        int left_size = soc_block->total_size - soc_block->send_size;

        SIMATPROXYLOG("left_soc_send_data left:%d\r\n", left_size);

        if (0 < left_size) {
            //send to lwip
            if (UDP_TYPE == simat_get_socket_type(simat_soc_info->socket_type)) {
                SIMATPROXYLOG("send_left_data type is UDP error \r\n");
                configASSERT(0); // need lwip check, becasue udp need send all.  (can't happen?could remove?)
            } else if (TCP_TYPE == simat_get_socket_type(simat_soc_info->socket_type)) {
                ret = netconn_write_partly(simat_soc_info->create_conn,
                                           soc_block->start_address + soc_block->send_size,
                                           left_size,
                                           NETCONN_COPY,
                                           &bytes_written);
            }
        } else {
            SIMATPROXYLOG("send_data: left size error: %d\r\n", left_size);
            return;
        }

        if (ERR_OK != ret) {
            SIMATLWIPLOG("netconn send error sock type:%d, ret: %d",
                         simat_soc_info->socket_type, ret);
            if ( (ERR_MEM == ret) || (ERR_BUF == ret) ) {
                SIMATMEMLOG("send memory is oom");
                configASSERT(0); // oom case in send data process, need send reject according to spec
            }
        }

        SIMATPROXYLOG("process_left_socket_sending_data ret:%d,total:%d, send: %d, socket_id:%d",
                      ret, left_size,
                      bytes_written, simat_soc_info->request_socket_id);

        if (left_size > bytes_written) {	//data also hasn't send completely.
            SIMATPROXYLOG("send_data: %d, total_left: %d\r\n", bytes_written, left_size);
            soc_block->send_size += bytes_written;
        } else if (left_size == bytes_written) {	// send completely.
            SIMAT_PROXY_FREE(soc_block->start_address);
            memset(soc_block, 0 , sizeof(memory_block));
            // stack behaviour FIFO, LEFT N-1 go ahead
            for (inner_index = 1; inner_index < MAX_SOCKET_MEMORY_BLOCK_NUM; ++inner_index) {
                tmp_soc_block += inner_index;
                if ( NULL != tmp_soc_block->base_address) {
                    memcpy(soc_block, tmp_soc_block, sizeof(memory_block));
                }
                soc_block = tmp_soc_block;
            }
            simat_soc_info->sending_status = MD_TO_AP_CONFIRM;
            //  need send Ind to MD
            process_send_to_lwip_success(simat_soc_info->request_socket_id);
        }
    } else {
        SIMATPROXYLOG("left_soc_addr NULL \r\n");
    }
}


/******************************************************************************
 * FUNCTION
 *  merge_lwip_recv_data
 * DESCRIPTION
 *  process_left_socket_sending_data
 * PARAMETERS
 * socket_management_node *simat_soc_info
 * RETURNS
 *  void
 *******************************************************************************/
unsigned char *merge_lwip_recv_data(LWIP_recv_data *data_unit, unsigned int num)
{
    unsigned int index = 0, size = 0;
    unsigned char *p_data = NULL, tmp_data = NULL;
    if (0 < num) {
        if (1 < num) {
            size = (num - 1) * SIMAT_MAXIMUM_SIZE;
            size += data_unit[num].size;
        } else {
            size = data_unit[num].size;
        }
    } else {
        SIMATPROXYLOG("merge_lwip_recv_data total num < 0: %d", num);
        return NULL;
    }
    SIMATPROXYLOG("merge_lwip_recv_data total size: %d", size);

    p_data = SIMAT_PROXY_ALLOCATE(size, SIMAT_SOCKET_RECV_IND);
    if (NULL == p_data) {
        return NULL;
    }

    tmp_data = p_data;
    //copy data
    for (index = 0; index < num; ++index) {
        memcpy(tmp_data, data_unit[index].p_data, data_unit[index].size);
        tmp_data += data_unit[index].size;
    }

    return p_data;
}


void process_socket_left_recving_data(socket_management_node *simat_soc_info)
{
    struct netbuf *new_buf = simat_soc_info->left_recving_netbuf;
    unsigned char *p_recv_unit_data = NULL;
    unsigned short pbuf_length = 0;
    err_t ret = ERR_OK;
    LWIP_recv_data recv_uintes[MAXIMUM_PACKET_NUM] = {0};
    int index = 0, memory_index = 0;
    SIMAT_Data_Req *simat_data_request = NULL;
    unsigned char *p_original_allocate = NULL;
    memory_block *p_block = NULL;
    
    if(1 != simat_soc_info->left_recving_flag) {
         SIMATLWIPLOG("process_socket_left_recving_data enter wrong:%x\r\n", simat_soc_info);
         return;
    }

    recv_uintes[index].in_use = 0;
    memory_index = simat_socket_get_status_memory(simat_soc_info, AP_TO_MD);
	if(-1 == memory_index){
		 SIMATLWIPLOG("simat_socket_get_status_memory -1");
         return;
	}
	
    p_block = &(simat_soc_info->recv_buffer[memory_index]);

    ret = netbuf_data(new_buf, (void **)&p_recv_unit_data, &pbuf_length);
    if (ERR_OK != ret) {
        SIMATLWIPLOG("recv netbuf_data error happened :%d", ret);
        simat_soc_info->recving_status = AP_TO_MD_ERROR;
        configASSERT(0); // ask lwip add memory?  need lwip check
        return;
    }
    recv_uintes[index].p_data = p_recv_unit_data;
    recv_uintes[index].size = pbuf_length;
    recv_uintes[index].in_use = 1;
    ++index;

    SIMATLWIPLOG("recv netbuf_data size :%d \r\n", pbuf_length);

    // there is more data need to recv, because mux limitation, recv again when this time successful
    if (0 == netbuf_next(&new_buf)) {
        simat_soc_info->left_recving_flag = 1;
        simat_soc_info->left_recving_netbuf = new_buf;
        SIMATLWIPLOG("process_socket_left_recving_data from sock: %d addr: %x\r\n", 
                     simat_soc_info->request_socket_id,
                     simat_soc_info->left_recving_netbuf);
    }

    // construct to user data
    simat_data_request = construct_simat_user_data(simat_soc_info->request_socket_id,
                         recv_uintes,
                         index - 1,
                         p_original_allocate);

    if(0 == simat_soc_info->left_recving_flag) {
        netbuf_delete(new_buf);
    } else {
        SIMATLWIPLOG("process_socket_left_recving_data not deletet netbuf_delete\r\n");
    }

    // Assert is happened in this case in construct_simat_user_data, so don't care after.
    if (NULL == simat_data_request) {
        SIMATLWIPLOG("process_socket_left_recving_data NULL == simat_data_request\r\n");
        simat_soc_info->recving_status = AP_TO_MD_ERROR;
        return;
    }


    // need to shift memory pointer to user data position
    // because CMUX memory come from AP, so in order to avoid allocate memory many times using pointer to save.
    p_block->base_address = p_original_allocate;
    p_block->start_address = simat_data_request;

    // no usage for lwip > MD send
    // p_block->total_size = recv_length;
    // p_block->send_size = 0;

    // notify CMUX to send to MD data received
    process_send_to_recv_ind(simat_soc_info->recv_buffer[0].start_address);
    
}
   
void process_socket_recving_data(socket_management_node *simat_soc_info)
{
    struct sockaddr_in sin_recv = {0};
    socklen_t addrlen = sizeof(struct sockaddr);
    int memory_index = 0, recv_length = -1, index = 0;
    SIMAT_Data_Req *simat_data_request = NULL;
    unsigned char *p_original_allocate = NULL;
    LWIP_recv_data recv_uintes[MAXIMUM_PACKET_NUM] = {0};
    memory_block *p_block = NULL;
    unsigned char *socket_recv_data = recv_uintes[index].p_data;
    struct netbuf *new_buf = NULL;
    unsigned char *p_recv_unit_data = NULL;
    unsigned short pbuf_length = 0;
    err_t ret = ERR_OK;
    int need_recv_total_size = 0;
    void             *buf = NULL;
    

    recv_uintes[index].in_use = 0;

    memory_index = simat_socket_get_status_memory(simat_soc_info, AP_TO_MD);

	SIMATLWIPLOG("recv index:%d  %x\r\n", memory_index, simat_soc_info);
	
	if(-1 == memory_index){
		return;
	}    

    p_block = &(simat_soc_info->recv_buffer[memory_index]);

    // has data to send to MD
    if (AP_TO_MD_SENDING != simat_soc_info->sending_status) {
        // need to shift memory pointer to user data position
        simat_soc_info->recving_status = AP_TO_MD_SENDING;
    }

    if (-1 != memory_index) {
        SIMATLWIPLOG("netconn_recv aa %x\r\n", simat_soc_info->create_conn);
        if (TCP_TYPE == simat_get_socket_type(simat_soc_info->socket_type)) {
        //    ret = netconn_recv_tcp_pbuf(simat_soc_info->create_conn, (struct pbuf **)&(new_buf->p));
        
        SIMATLWIPLOG("recv_tcp_pbuf start\r\n");
         ret = netconn_recv_tcp_pbuf(simat_soc_info->create_conn, (struct pbuf **)&buf);
        
         SIMATLWIPLOG("recv_tcp_pbuf:%x,%d\r\n", buf, ret);
         new_buf = pvPortMalloc(sizeof(struct netbuf));
         if (NULL == new_buf) {
         SIMATMEMLOG("allocate fail!!");
         return;
        }
         new_buf->ptr = buf;
         
         SIMATLWIPLOG("recv_tcp_pbuf11:%x\r\n", new_buf->ptr);
        }else if (UDP_TYPE == simat_get_socket_type(simat_soc_info->socket_type)){
            ret = netconn_recv(simat_soc_info->create_conn, &new_buf);
        }

        SIMATLWIPLOG("recv:%d\r\n", ret);

        if (ERR_OK != ret) {
            SIMATLWIPLOG("recv netconn_recv error happened :%d\r\n", ret);
            if ( (ERR_MEM == ret) || (ERR_BUF == ret) ) {
                configASSERT(0); // lwip check the reason & add memory ?
            }
            simat_soc_info->recving_status = AP_TO_MD_ERROR;
            return;
        }
        if (UDP_TYPE == simat_get_socket_type(simat_soc_info->socket_type)){
            ret = netbuf_data(new_buf, (void **)&p_recv_unit_data, &pbuf_length);
            if (ERR_OK != ret) {
                SIMATLWIPLOG("recv netbuf_data error happened :%d", ret);
                simat_soc_info->recving_status = AP_TO_MD_ERROR;
                configASSERT(0);// lwip check the reason & add memory ?
                return;
            }
            recv_uintes[index].p_data = p_recv_unit_data;
            recv_uintes[index].size = pbuf_length;
            recv_uintes[index].in_use = 1;
            ++index;

            SIMATLWIPLOG("recv netbuf_data size :%d \r\n", pbuf_length);

            // there is more data need to recv, because mux limitation, recv again when this time successful
            if (0 == netbuf_next(&new_buf)) {
                simat_soc_info->left_recving_flag = 1;
                simat_soc_info->left_recving_netbuf = new_buf;
                SIMATLWIPLOG("more data need recv from sock: %d addr: %x\r\n", 
                             simat_soc_info->request_socket_id,
                             simat_soc_info->left_recving_netbuf);
            }
        } else if(TCP_TYPE == simat_get_socket_type(simat_soc_info->socket_type)) {
            recv_uintes[index].p_data = new_buf->ptr->payload;
            recv_uintes[index].size = new_buf->ptr->tot_len;
            recv_uintes[index].in_use = 1;
            ++index;
            SIMATLWIPLOG("new_buf->ptr->payload: %s\r\n", 
                         new_buf->ptr->payload);
            SIMATLWIPLOG("new_buf->ptr->tot_len: %d, %d\r\n", 
                                     new_buf->ptr->tot_len, new_buf->ptr->len);

        }
        // construct to user data
        simat_data_request = construct_simat_user_data(simat_soc_info->request_socket_id,
                             recv_uintes,
                             index - 1,
                             p_original_allocate);

        if(0 == simat_soc_info->left_recving_flag) {
            if (TCP_TYPE == simat_get_socket_type(simat_soc_info->socket_type)) {
                pbuf_free (new_buf->ptr);
                vPortFree(new_buf);
            } else {
            
                netbuf_delete(new_buf);
            }
        } else {
            SIMATLWIPLOG("not deletet netbuf_delete\r\n");
        }

        // Assert is happened in this case in construct_simat_user_data, so don't care after.
        if (NULL == simat_data_request) {
            SIMATLWIPLOG("NULL == simat_data_request\r\n");
            simat_soc_info->recving_status = AP_TO_MD_ERROR;
            return;
        }


        // need to shift memory pointer to user data position
        // because CMUX memory come from AP, so in order to avoid allocate memory many times using pointer to save.
        p_block->base_address = p_original_allocate;
        p_block->start_address = simat_data_request;

        // no usage for lwip > MD send
        //p_block->total_size = recv_length;
        //p_block->send_size = 0;

        // notify CMUX to send to MD data received
        process_send_to_recv_ind(simat_soc_info->recv_buffer[0].start_address);
    }
}


void process_socket_close(unsigned int request_socket, unsigned char close_reason)
{
    SIMAT_Close_Ind *param = NULL;
    // send msg to simat_proxy_modem_main_loop task
    param = (SIMAT_Close_Ind *)SIMAT_PROXY_ALLOCATE(sizeof(SIMAT_Close_Ind), SIMAT_SOCKET_CLOSE_IND);
    if (NULL == param) {
        SIMATPROXYLOG("msg allocate memory error NULL\r\n");
        return;
    }
    SIMATPROXYLOG("process_socket_close reason:%d\r\n", close_reason);
    param->soc_base.base_header.simat_code = SIMAT_SOCKET_CLOSE_IND;
    param->soc_base.base_header.simat_length = sizeof(SIMAT_Close_Ind);
    param->soc_base.socket_id = request_socket;
    param->cause = close_reason;
    simat_send_event_internal(SIMAT_SOCKET_CLOSE_IND, param);
}



///////////////////////////////////////////////////////////////////////////////////

//UT for test code
/* ------------------------------------------------------------------------------  */

uint8_t *test_recv_from_ap_mux(char *data, int size);

void test_bearer_config_req_2();


void test_send_soc_open_request_to_AP_1();
void test_soc_send_data_to_AP_1();
void test_soc_recv_data_rsp_to_AP_1();
void test_send_soc_close_request_to_AP_1();

void test_send_soc_udp_open_request_to_AP_1();
void test_soc_udp_send_data_to_AP_1();
void test_soc_udp_recv_data_rsp_to_AP_1();
void test_send_soc_udp_close_request_to_AP_1();

void test_wrong_simat_command();


/* ----------------------------------------------------------------------- */
// bearer test case:
char bearer_config_req[] = {SIMAT_CONFIG_REQ, 0, 44, 0,
                            1, 15, 'G','A','M','T','O','L','S','E','R','v','e','r','\0',
                            2, 3, 2,
                            3, 11, 'u','s','e','r','n','a','m','e','\0',
                            4, 11, 'p','a','s','s','w','o','r','d','\0'};

char bearer_config_wrong_req[] = {SIMAT_CONFIG_REQ, 0, 44, 0,
                            1, 15, 'G','A','M','T','O','L','S','E','R','v','e','r','\0',
                            6, 3, 2,                        // 6 type is error
                            3, 11, 'u','s','e','r','n','a','m','e','\0',
                            4, 11, 'p','a','s','s','w','o','r','d','\0'};

char bearer_bring_up_req[] = {SIMAT_BRING_UP_REQ, 0, 4, 0};

char bearer_bring_down_req[] = {SIMAT_BRING_DOWN_REQ, 0, 4, 0};



/* --------------------------------------------------------------------------  */
// TCP test case:
char soc_open_req_data[] = {9, 0, 17, 0,
                            0x01, 0x00, 0x00, 0x00,
                            0x05, 0x09, 0x06, 25,           // server port 6500
                            100,  192,  168, 0, 23
                           };                               // server ip 192.168.1.107


char soc_open_req_data_wrong_ip[] = {9, 0, 17, 0,
                                    0x01, 0x00, 0x00, 0x00,
                                    0x05, 0x09, 0x06, 25,           // server port 6500
                                    100,  192,  168, 1, 75
                                    };                               // server ip 192.168.1.107


char soc_send_data_req[] = {SIMAT_SOCKET_SEND_REQ, 0, 18, 0,
                            0x01, 0, 0, 0,
                            'h', 'e', 'l', 'l', 'o', 's', 'i', 'm', 'a', 't'
                           };                   //user data

char soc_invalid_send_data_cnf[] = {SIMAT_SOCKET_SEND_CNF + 28, 0, 8, 0,
                            0x01, 0, 0, 0                            
                           }; 


char *soc_recv_data_req = "hellosimat from internet";

char soc_recv_data_rsp[] = {17, 0, 8, 0,
                            0x01, 0, 0, 0
                           };                   // send data cnf from MD to AP

char soc_close_soc_req[] = {11, 0, 8, 0,
                            0x01, 0, 0, 0
                           };                   // close socket

/* --------------------------------------------------------------------------  */
// UDP test case:
char soc_open_udp_req_data[] = {9, 0, 17, 0,
                            0x02, 0x00, 0x00, 0x00,
                            0x05, 0x09, 0x11, 25,           // udp server port 6600
                            200,  192,  168, 0, 23
                           };                             // udp server ip 192.168.1.107

char soc_send_udp_data_req[] = {13, 0, 21, 0,
                            0x02, 0, 0, 0,
                            'h', 'e', 'l', 'l', 'o', 'u', 'd', 'p', 's', 'i', 'm', 'a', 't'
                           }; 

char soc_recv_udp_data_rsp[] = {17, 0, 8, 0,
                            0x02, 0, 0, 0
                           }; 

char soc_close_udp_soc_req[] = {11, 0, 8, 0,
                            0x02, 0, 0, 0
                           };                   // close socket

/* ------------------------------------------------------------------------ */
//wrong test case:
char wrong_req_data[] = {SIMAT_CODE_END, 0, 17, 0,
                            0x02, 0x00, 0x00, 0x00,
                            0x05, 0x09, 0x11, 25,           // udp server port 6600
                            200,  192,  168, 0, 23
                           };                             // udp server ip 192.168.1.107

/*--------------------------------------------------------------------------*/

void test_bearer_config_req_1()
{    
    SIMAT_Config_Req *bear_config_req = NULL;

    char *char_bear_config_req = NULL;

    SIMATPROXYLOG("test_bearer_config_req_1 start \r\n");

    char_bear_config_req = test_recv_from_ap_mux(bearer_config_req, 
                                               sizeof(bearer_config_req));
    
    bear_config_req = (SIMAT_SOC_Open_Req *)char_bear_config_req;
    

    SIMATPROXYLOG("test_bearer_config_req_1 code: %d, len: %d",
                  bear_config_req->base_header.simat_code,
                  bear_config_req->base_header.simat_length);
}


void test_bearer_bring_up_req_1()
{    
    SIMAT_Bring_Up_Req *bear_bring_up_req = NULL;

    char *char_bear_bring_up_req = NULL;

    SIMATPROXYLOG("test_bearer_bring_up_req_1 start \r\n");

    char_bear_bring_up_req = test_recv_from_ap_mux(bearer_bring_up_req, 
                                               sizeof(bearer_bring_up_req));
    
    bear_bring_up_req = (SIMAT_SOC_Open_Req *)char_bear_bring_up_req;
    

    SIMATPROXYLOG("test_bearer_bring_up_req_1 code: %d, len: %d",
                  bear_bring_up_req->simat_code,
                  bear_bring_up_req->simat_length);
}


void test_send_soc_open_request_to_AP_1()
{
    SIMAT_SOC_Open_Req *soc_open_req = NULL;

    char *char_soc_open_data = NULL;

    SIMATPROXYLOG("test test_send_soc_open_request_to_AP_1 start \r\n");

    char_soc_open_data = test_recv_from_ap_mux(soc_open_req_data, 
                                               sizeof(soc_open_req_data));
    
    soc_open_req = (SIMAT_SOC_Open_Req *)char_soc_open_data;
    

    SIMATPROXYLOG("test_send_soc_open_request_to_AP_1 code: %d, len: %d, soc_id: %d, -%d-%d-%d-%d-%d-\r\n",
                  soc_open_req->soc_base.base_header.simat_code,
                  soc_open_req->soc_base.base_header.simat_length,
                  soc_open_req->soc_base.socket_id,
                  soc_open_req->remote_socket_info[0],
                  soc_open_req->remote_socket_info[1],
                  soc_open_req->remote_socket_info[2],
                  soc_open_req->remote_socket_info[3],
                  soc_open_req->remote_socket_info[4]);
}


void test_send_soc_udp_open_request_to_AP_1()
{
    SIMAT_SOC_Open_Req *soc_open_req = NULL;

    // use SIMAT_PROXY_ALLOCATE to allocate memory
    char *char_soc_open_data = test_recv_from_ap_mux(soc_open_udp_req_data,
                                                     sizeof(soc_open_udp_req_data));

    soc_open_req = (SIMAT_SOC_Open_Req *)char_soc_open_data;

    SIMATPROXYLOG("test udp soc open start \r\n");


    SIMATPROXYLOG("test udp soc open code: %d, len: %d, soc_id: %d, -%d-%d-%d-%d-%d-\r\n",
                  soc_open_req->soc_base.base_header.simat_code,
                  soc_open_req->soc_base.base_header.simat_length,
                  soc_open_req->soc_base.socket_id,
                  soc_open_req->remote_socket_info[0],
                  soc_open_req->remote_socket_info[1],
                  soc_open_req->remote_socket_info[2],
                  soc_open_req->remote_socket_info[3],
                  soc_open_req->remote_socket_info[4]);

}

void simat_ut_send_data_req(uint32_t request_socket_id)
{
    if(0x01 == request_socket_id) {
        SIMATPROXYLOG("test_soc_send_data_to_AP_1\r\n");
        test_soc_send_data_to_AP_1();
    }else if(0x02 == request_socket_id) {
        SIMATPROXYLOG("test_soc_udp_send_data_to_AP_1\r\n");
        test_soc_udp_send_data_to_AP_1();
    }
}

void simat_ut_recv_data_rsp(uint32_t request_socket_id)
{
    SIMATPROXYLOG("simat_recv_msg id: %d\r\n", request_socket_id);    
    if(0x01 == request_socket_id) {
        test_soc_recv_data_rsp_to_AP_1();
    }else if(0x02 == request_socket_id) {
        test_soc_udp_recv_data_rsp_to_AP_1();
    }
}

void simat_ut_close_rsp(uint32_t request_socket_id)
{
    SIMATPROXYLOG("simat_ut_close_rsp %d\r\n", request_socket_id);
    if(0x01 == request_socket_id) {
        test_send_soc_close_request_to_AP_1();
    }else if(0x02 == request_socket_id) {
        test_send_soc_udp_close_request_to_AP_1();
    }
}

void simat_ut_bearer_close(uint32_t request_socket_id) {
    SIMATPROXYLOG("simat_ut_bearer_close %d\r\n", request_socket_id);
    if(0x02 == request_socket_id) {
        test_bearer_bring_down_req_1();
    }
}


void test_wrong_simat_command_from_MD()
{
    SIMAT_Config_Req *bear_config_req = NULL;

    char *char_bear_config_req = NULL;

    SIMATPROXYLOG("test_wrong_simat_command start \r\n");

    char_bear_config_req = test_recv_from_ap_mux(wrong_req_data, 
                                               sizeof(wrong_req_data));
    
    bear_config_req = (SIMAT_SOC_Open_Req *)char_bear_config_req;
    

    SIMATPROXYLOG("test_wrong_simat_command code: %d, len: %d",
                  bear_config_req->base_header.simat_code,
                  bear_config_req->base_header.simat_length);
}


void test_wrong_simat_command_from_AP()
{   
   SIMAT_Send_Data_Conf *soc_send_data = NULL;
   // use SIMAT_PROXY_ALLOCATE to allocate memory
   char *char_soc_send_data = SIMAT_PROXY_ALLOCATE(
                                sizeof(soc_invalid_send_data_cnf), 
                                SIMAT_SOCKET_SEND_CNF);
   
   memcpy(char_soc_send_data, soc_invalid_send_data_cnf, 
                              sizeof(soc_invalid_send_data_cnf));
   
   soc_send_data = (SIMAT_Send_Data_Conf *)char_soc_send_data;
   
   SIMATPROXYLOG("test test_wrong_simat_command_from_AP start \r\n");
   
   
   SIMATPROXYLOG("test_wrong_simat_command_from_AP code: %d, len: %d, soc_id: %d\r\n",
                 soc_send_data->base_header.simat_code,
                 soc_send_data->base_header.simat_length,
                 soc_send_data->socket_id);
   
   simat_send_event_internal(SIMAT_SOCKET_SEND_CNF, (SIMAT_Base_Header *)soc_send_data);
   
}



/* --------------------------------------------------------------------------  */

uint8_t *test_recv_from_ap_mux(char *data, int size)
{
    mux_ap_event_prepare_to_receive_t param;
    mux_ap_event_receive_completed_t recv_comp;
    param.channel_id = MUX_AP_CHANNEL_TYPE_SIMAT;
    param.user_data = 0;
    param.data_buffer = NULL;
    param.buffer_length = size;
    simat_proxy_cmux_cb(MUX_AP_EVENT_PREPARE_TO_RECEIVE, &param);
    if(NULL == param.data_buffer) {
        return 0;
    }

    memcpy(param.data_buffer, data, size);

	if(NULL != param.data_buffer) {
    	printf("param.data_buffer \r\n",param.data_buffer[0],
        	param.data_buffer[1],
        	param.data_buffer[2]);
	}else{
		printf("param.data_buffer NULL");
	}
    
    recv_comp.user_data = 0;
    recv_comp.channel_id = MUX_AP_CHANNEL_TYPE_SIMAT;
    recv_comp.data_buffer = param.data_buffer;
    recv_comp.buffer_length = param.buffer_length;
    simat_proxy_cmux_cb(MUX_AP_EVENT_RECEIVE_COMPLETED, &recv_comp);
    
    return recv_comp.data_buffer;
}


void test_soc_send_data_to_AP_1()
{
    SIMAT_Send_Data_Req *soc_send_data = NULL;
    
    // use SIMAT_PROXY_ALLOCATE to allocate memory
    char *char_soc_send_data = test_recv_from_ap_mux(soc_send_data_req,
                                                     sizeof(soc_send_data_req));

	soc_send_data = (SIMAT_Recv_Data_Rsp *)char_soc_send_data;
	
    SIMATPROXYLOG("test test_soc_send_data_to_AP_1 start \r\n");


    SIMATPROXYLOG("test_soc_send_data_to_AP_1 code: %d, len: %d, soc_id: %d\r\n",
                  soc_send_data->soc_base.base_header.simat_code,
                  soc_send_data->soc_base.base_header.simat_length,
                  soc_send_data->soc_base.socket_id);

    simat_send_event_internal(SIMAT_SOCKET_SEND_REQ, (SIMAT_Base_Header *)soc_send_data);
}


void test_soc_recv_data_rsp_to_AP_1()
{
    SIMAT_Recv_Data_Rsp *soc_recv_rsp = NULL;

    // use SIMAT_PROXY_ALLOCATE to allocate memory
    char *char_soc_recv_cnf_data = test_recv_from_ap_mux(soc_recv_data_rsp,
                                                        sizeof(soc_recv_data_rsp));

    soc_recv_rsp = (SIMAT_Recv_Data_Rsp *)char_soc_recv_cnf_data;

    SIMATPROXYLOG("test_soc_recv_data_rsp_to_AP_1 start \r\n");


    SIMATPROXYLOG("test_soc_recv_data_rsp_to_AP_1 code: %d, len: %d, soc_id: %d\r\n",
                  soc_recv_rsp->base_header.simat_code,
                  soc_recv_rsp->base_header.simat_length,
                  soc_recv_rsp->socket_id);
    
}

void test_send_soc_close_request_to_AP_1()
{
    SIMAT_Close_Req *soc_close_req = NULL;

    // use SIMAT_PROXY_ALLOCATE to allocate memory
    char *char_soc_close_req_data = test_recv_from_ap_mux(soc_close_soc_req,
                                                          sizeof(soc_close_soc_req));

    soc_close_req = (SIMAT_Send_Data_Req *)char_soc_close_req_data;

    SIMATPROXYLOG("test test_send_soc_close_request_to_AP_1 start \r\n");


    SIMATPROXYLOG("test_send_soc_close_request_to_AP_1 code: %d, len: %d, soc_id: %d\r\n",
                  soc_close_req->base_header.simat_code,
                  soc_close_req->base_header.simat_length,
                  soc_close_req->socket_id);

}

/*--------------------------------------------------------------------------------------*/

void test_soc_udp_send_data_to_AP_1()
{
    SIMAT_Send_Data_Req *soc_send_data = NULL;
    // use SIMAT_PROXY_ALLOCATE to allocate memory
    char *char_soc_send_data = test_recv_from_ap_mux(soc_send_udp_data_req,
                                                     sizeof(soc_send_udp_data_req));
     
    soc_send_data = (SIMAT_Send_Data_Req *)char_soc_send_data;

    SIMATPROXYLOG("test_soc_udp_send_data_to_AP_1 code: %d, len: %d, soc_id: %d\r\n",
                  soc_send_data->soc_base.base_header.simat_code,
                  soc_send_data->soc_base.base_header.simat_length,
                  soc_send_data->soc_base.socket_id);

}


void test_soc_udp_recv_data_rsp_to_AP_1()
{
    SIMAT_Recv_Data_Rsp *soc_recv_rsp = NULL;

    char *char_soc_recv_cnf_data = test_recv_from_ap_mux(soc_recv_udp_data_rsp,
                                                         sizeof(soc_recv_udp_data_rsp));

    soc_recv_rsp = (SIMAT_Recv_Data_Rsp *)char_soc_recv_cnf_data;

    SIMATPROXYLOG("test_soc_dp_recv_data_rsp_to_AP_1 start \r\n");


    SIMATPROXYLOG("test_soc_dp_recv_data_rsp_to_AP_1 code: %d, len: %d, soc_id: %d\r\n",
                  soc_recv_rsp->base_header.simat_code,
                  soc_recv_rsp->base_header.simat_length,
                  soc_recv_rsp->socket_id);
}


void test_send_soc_udp_close_request_to_AP_1()
{
    SIMAT_Close_Req *soc_close_req = NULL;

    // use SIMAT_PROXY_ALLOCATE to allocate memory
    char *char_soc_close_req_data = test_recv_from_ap_mux(soc_close_udp_soc_req,
                                                          sizeof(soc_close_udp_soc_req));

    soc_close_req = (SIMAT_Send_Data_Req *)char_soc_close_req_data;

    SIMATPROXYLOG("test_send_soc_udp_close_request_to_AP_1 start \r\n");


    SIMATPROXYLOG("test_send_soc_udp_close_request_to_AP_1 code: %d, len: %d, soc_id: %d\r\n",
                  soc_close_req->base_header.simat_code,
                  soc_close_req->base_header.simat_length,
                  soc_close_req->socket_id);
}

/*------------------------------------------------------------------------------------*/

// bearer case:

void test_bearer_bring_down_req_1()
{    
    SIMAT_Bring_Down_Req *bear_bring_done_req = NULL;

    char *char_bear_bring_done_req = NULL;

    SIMATPROXYLOG("test_bearer_bring_down_req_1 start \r\n");

    char_bear_bring_done_req = test_recv_from_ap_mux(bearer_bring_down_req, 
                                               sizeof(bearer_bring_down_req));
    
    bear_bring_done_req = (SIMAT_Bring_Down_Req *)char_bear_bring_done_req;
    

    SIMATPROXYLOG("test_bearer_bring_down_req_1 code: %d, len: %d",
                  bear_bring_done_req->simat_code,
                  bear_bring_done_req->simat_length);
}


/*------------------------------------------------------------------------------------*/

// exception case test:

void test_bearer_config_req_2()
{    
    SIMAT_Config_Req *bear_config_req = NULL;

    char *char_bear_config_req = NULL;

    SIMATPROXYLOG("test_bearer_config_req_2 start \r\n");

    char_bear_config_req = test_recv_from_ap_mux(bearer_config_wrong_req, 
                                               sizeof(bearer_config_wrong_req));
    
    bear_config_req = (SIMAT_SOC_Open_Req *)char_bear_config_req;
    

    SIMATPROXYLOG("test_bearer_config_req_2 code: %d, len: %d",
                  bear_config_req->base_header.simat_code,
                  bear_config_req->base_header.simat_length);
}


void test_send_soc_open_request_to_AP_2()
{
    SIMAT_SOC_Open_Req *soc_open_req = NULL;

    char *char_soc_open_data = NULL;

    SIMATPROXYLOG("test test_send_soc_open_request_to_AP_1 start \r\n");

    char_soc_open_data = test_recv_from_ap_mux(soc_open_req_data_wrong_ip, 
                                               sizeof(soc_open_req_data_wrong_ip));
    
    soc_open_req = (SIMAT_SOC_Open_Req *)char_soc_open_data;
    

    SIMATPROXYLOG("test_send_soc_open_request_to_AP_1 code: %d, len: %d, soc_id: %d, -%d-%d-%d-%d-%d-\r\n",
                  soc_open_req->soc_base.base_header.simat_code,
                  soc_open_req->soc_base.base_header.simat_length,
                  soc_open_req->soc_base.socket_id,
                  soc_open_req->remote_socket_info[0],
                  soc_open_req->remote_socket_info[1],
                  soc_open_req->remote_socket_info[2],
                  soc_open_req->remote_socket_info[3],
                  soc_open_req->remote_socket_info[4]);
}
