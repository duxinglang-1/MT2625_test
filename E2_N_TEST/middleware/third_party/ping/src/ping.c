/**
 * @file
 * Ping sender module
 *
 */

/*
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 */

/**
 * This is an example of a "ping" sender (with raw API and socket API).
 * It can be used as a start point to maintain opened a network connection, or
 * like a network "watchdog" for your device.
 *
 */

#include "lwip/opt.h"

#if LWIP_RAW /* don't build if not configured for use in lwipopts.h */

#if PING_USE_SOCKETS
#include "lwip/sockets.h"
#include "lwip/inet.h"
#endif /* PING_USE_SOCKETS */
#include "task_def.h"
#include "syslog.h"
#include "ril_internal_use.h"
#include "netdb.h"
#include "lwip/ip_addr.h"
#include "lwip/mem.h"
#include "lwip/raw.h"
#include "lwip/icmp.h"
#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/timers.h"
#include "lwip/inet_chksum.h"
#include "ping.h"

/**
 * PING_DEBUG: Enable debugging for PING.
 */
#ifndef PING_DEBUG
#define PING_DEBUG     LWIP_DBG_ON
#endif

/** ping target - should be a "ip_addr_t" */
#ifndef PING_TARGET
#define PING_TARGET   (netif_default?netif_default->gw:ip_addr_any)
#endif

/** ping receive timeout - in milliseconds */
#ifndef PING_RCV_TIMEO
#ifdef MTK_TCPIP_FOR_NB_MODULE_ENABLE
#define PING_RCV_TIMEO 10000
#else
#define PING_RCV_TIMEO 1000
#endif
#endif

/** ping delay - in milliseconds */
#ifndef PING_DELAY
#define PING_DELAY     1000
#endif

/** ping identifier - must fit on a u16_t */
#ifndef PING_ID
#define PING_ID        0xAFAF
#endif

/** ping additional data size to include in the packet */
#ifndef PING_DATA_SIZE
#define PING_DATA_SIZE 32
#endif

/** ping result action - no default action */
#ifndef PING_RESULT
#define PING_RESULT(ping_ok)
#endif

#if !PING_USE_SOCKETS
static struct raw_pcb *ping_pcb;
#endif /* PING_USE_SOCKETS */

typedef enum {
    PING_IDLE,
    PING_RUN,
    PING_CLOSE
} ping_state_t;

static ping_state_t g_ping_state = PING_IDLE;
static sys_mbox_t ping_mbox = 0U;
#define PING_MBOX_SIZE 10
#define PING_APP_ID ((int32_t)(&ping_mbox))

typedef enum {
    PING_MSG_CLOSE_REQ,
    PING_MSG_DATA_SERVICE_IND
}ping_msg_type_t;

ping_arg_t g_ping_arg;

typedef struct _ping_static
{
    u32_t ping_time;
    u32_t ping_min_time;
    u32_t ping_max_time;
    u32_t ping_avg_time;
    u32_t ping_done;
    u32_t ping_lost_num;
    u32_t ping_recv_num;
    u32_t count;
    u32_t size;
    u32_t ping_timeout;
    u16_t ping_seq_num;
    u8_t addr[16];
} ping_static_t;

#ifdef PING_MODULE_PRINTF
#define PING_LOGE(fmt,arg...)   printf(("[ping]: "fmt), ##arg)
#define PING_LOGW(fmt,arg...)   printf(("[ping]: "fmt), ##arg)
#define PING_LOGI(fmt,arg...)   printf(("[ping]: "fmt), ##arg)
#else
log_create_module(ping, PRINT_LEVEL_INFO);
#define PING_LOGE(fmt,arg...)   LOG_E(ping, "[ping]: "fmt,##arg)
#define PING_LOGW(fmt,arg...)   LOG_W(ping, "[ping]: "fmt,##arg)
#define PING_LOGI(fmt,arg...)   LOG_I(ping, "[ping]: "fmt,##arg)
#endif

static err_t ping6_send(int s, ip6_addr_t *addr, ping_static_t *p_ping_static);
static void ping6_recv(int s, ip6_addr_t *addr, ping_static_t *p_ping_static, ping_request_result_t callback);
static err_t ping_send(int s, ip4_addr_t *addr, ping_static_t *p_ping_static);
static void ping_recv(int s, ip4_addr_t *addr, ping_static_t *p_ping_static, ping_request_result_t callback);
static void ping_prepare_echo( struct icmp_echo_hdr *iecho, u16_t len, ping_static_t *p_ping_static);
static void ping6_prepare_echo( struct icmp6_echo_hdr *iecho, u16_t len, ping_static_t *p_ping_static);
static bool ping_resolve_address(char* server, ip_addr_t* ip_addr);

/** get current system time in ms **/
u32_t time_now()
{
    uint32_t count = 0;
    uint64_t count64 = 0;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &count);
    count64 = ((uint64_t)count) * 1000 / 32768;
    return (uint32_t)count64;
}
/** get the duration from the input to now **/
u32_t get_duration(u32_t start_count)
{
    uint64_t end_count64 = 0;
    uint64_t start_count64 = 0;
    uint32_t end_count = 0;
    start_count64 = ((uint64_t)start_count) * 32768 / 1000;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &end_count);
    hal_gpt_get_duration_count((uint32_t)start_count64, end_count, &end_count);
    end_count64 = ((uint64_t)end_count) * 1000 / 32768;

    return (uint32_t)end_count64;
}

/** Prepare a echo ICMP request */
static void
ping_prepare_echo( struct icmp_echo_hdr *iecho, u16_t len, ping_static_t *p_ping_static)
{
    size_t i;
    size_t data_len = len - sizeof(struct icmp_echo_hdr);

    ICMPH_TYPE_SET(iecho, ICMP_ECHO);
    ICMPH_CODE_SET(iecho, 0);
    iecho->chksum = 0;
    iecho->id     = PING_ID;
    (p_ping_static->ping_seq_num) = (p_ping_static->ping_seq_num) + 1;
    iecho->seqno  = htons(p_ping_static->ping_seq_num);

    /* fill the additional data buffer with some data */
    for(i = 0; i < data_len; i++)
    {
        ((char*)iecho)[sizeof(struct icmp_echo_hdr) + i] = (char)i;
    }

    iecho->chksum = inet_chksum(iecho, len);
}

#if PING_USE_SOCKETS

/* Ping using the socket ip */
static err_t
ping_send(int s, ip4_addr_t *addr, ping_static_t *p_ping_static)
{
    int err;
    struct icmp_echo_hdr *iecho;
    struct sockaddr_in to;
    size_t ping_size = sizeof(struct icmp_echo_hdr) + (p_ping_static->size);
    LWIP_ASSERT("ping_size is too big", ping_size <= 0xffff);

    iecho = (struct icmp_echo_hdr *)mem_malloc((mem_size_t)ping_size);
    if (!iecho)
    {
        return ERR_MEM;
    }

    ping_prepare_echo(iecho, (u16_t)ping_size, p_ping_static);

    PING_LOGI("ping: send seq(0x%04X) %"U16_F".%"U16_F".%"U16_F".%"U16_F,     \
                        p_ping_static->ping_seq_num,\
                        ip4_addr1_16(addr),         \
                        ip4_addr2_16(addr),         \
                        ip4_addr3_16(addr),         \
                        ip4_addr4_16(addr));

    to.sin_len = sizeof(to);
    to.sin_family = AF_INET;
    to.sin_port = lwip_htons(10000);
    inet_addr_from_ipaddr(&to.sin_addr, addr);
    p_ping_static->ping_time = time_now();
    err = lwip_sendto(s, iecho, ping_size, 0, (struct sockaddr*)&to, sizeof(to));
    PING_LOGI("send result:%d\r\n", err);
    mem_free(iecho);

    return (err ? ERR_OK : ERR_VAL);
}

static void
ping_recv(int s, ip4_addr_t *addr, ping_static_t *p_ping_static, ping_request_result_t callback)
{
    char buf[64];
    int fromlen, len;
    struct sockaddr_in from;
    struct ip_hdr *iphdr;
    struct icmp_echo_hdr *iecho;
    ping_packet_result_t packet_result = {0};
    struct timeval timeout = {0};

    timeout.tv_sec  = (p_ping_static->ping_timeout)/1000;
    timeout.tv_usec = ((p_ping_static->ping_timeout)%1000)*1000;
    fromlen = sizeof(struct sockaddr_in);
    lwip_setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    while((len = lwip_recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*)&from,
                               (socklen_t*)&fromlen)) > 0)
    {
        if (len >= (int)(sizeof(struct ip_hdr) + sizeof(struct icmp_echo_hdr)))
        {
            ip4_addr_t fromaddr;
            u32_t cur_time = get_duration(p_ping_static->ping_time);

            inet_addr_to_ipaddr(&fromaddr, &from.sin_addr);
            /* LWIP_DEBUGF( PING_DEBUG, ("ping: recv ")); */
            iphdr = (struct ip_hdr *)buf;
            iecho = (struct icmp_echo_hdr *)(buf + (IPH_HL(iphdr) * 4));

            /* ignore packet if it is not ping reply */
            if ((0 != (iecho->type)) || ((addr->addr) != (fromaddr.addr)))
            {
                if (cur_time > p_ping_static->ping_timeout)
                {
                    PING_LOGI("--- ping: timeout");
                    p_ping_static->ping_lost_num = p_ping_static->ping_lost_num + 1;
                    return;
                }
                else
                {
                    timeout.tv_sec  = (p_ping_static->ping_timeout - cur_time)/1000;
                    timeout.tv_usec = ((p_ping_static->ping_timeout - cur_time)%1000)*1000;
                    lwip_setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
                    PING_LOGI("reset recv timeout \r\n");
                    continue;
                }
            }

            if ((iecho->id == PING_ID) && (iecho->seqno == htons(p_ping_static->ping_seq_num)))
            {
                PING_LOGI("ping: recv seq(0x%04X) %"U16_F".%"U16_F".%"U16_F".%"U16_F", %"U32_F" ms", \
                                    htons(iecho->seqno),             \
                                    ip4_addr1_16(&fromaddr),         \
                                    ip4_addr2_16(&fromaddr),         \
                                    ip4_addr3_16(&fromaddr),         \
                                    ip4_addr4_16(&fromaddr),         \
                                    cur_time);
                if (cur_time > p_ping_static->ping_timeout)
                {
                    PING_LOGI("ping: timeout");
                    p_ping_static->ping_lost_num = p_ping_static->ping_lost_num + 1;
                    packet_result.is_timeout = true;
                    callback(PING_PACKET_RESULT, &packet_result);
                    return;
                }
                packet_result.ip_address[0] = ip4_addr1_16(&fromaddr);
                packet_result.ip_address[1] = ip4_addr2_16(&fromaddr);
                packet_result.ip_address[2] = ip4_addr3_16(&fromaddr);
                packet_result.ip_address[3] = ip4_addr4_16(&fromaddr);
                packet_result.is_ipv4 = true;
                packet_result.packet_size = ntohs(IPH_LEN(iphdr)) - (IPH_HL(iphdr) << 2) - sizeof(struct icmp_echo_hdr);
                packet_result.rtt = cur_time;
                packet_result.ttl = IPH_TTL(iphdr);
                packet_result.is_timeout = false;
                callback(PING_PACKET_RESULT, &packet_result);
                /* LWIP_DEBUGF( PING_DEBUG, (" %"U32_F" ms\n", (sys_now() - ping_time))); */
                if(p_ping_static->ping_min_time == 0 || p_ping_static->ping_min_time > cur_time)
                {
                    p_ping_static->ping_min_time = cur_time;
                }
                if(p_ping_static->ping_max_time == 0 || p_ping_static->ping_max_time < cur_time)
                {
                    p_ping_static->ping_max_time = cur_time;
                }
                p_ping_static->ping_avg_time = p_ping_static->ping_avg_time + cur_time;

                p_ping_static->ping_recv_num = p_ping_static->ping_recv_num + 1;

                /* do some ping result processing */
                PING_RESULT((ICMPH_TYPE(iecho) == ICMP_ER));
                return;
            }
            else
            {
                /*Only log that some other PING ACK is received now.*/
                PING_LOGI("ping: Get ping ACK seq(0x%04X), expected seq(0x%04X)", htons(iecho->seqno), p_ping_static->ping_seq_num);
                timeout.tv_sec  = (p_ping_static->ping_timeout - cur_time)/1000;
                timeout.tv_usec = ((p_ping_static->ping_timeout - cur_time)%1000)*1000;
                lwip_setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
                /* Can not return, due to there could be ping ack which has matched sequence num. */
            }

        }
    }

    if (-1 ==len)
    {
        PING_LOGI("ping: timeout");
        p_ping_static->ping_lost_num = p_ping_static->ping_lost_num + 1;
        packet_result.is_timeout = true;
        callback(PING_PACKET_RESULT, &packet_result);
    }

    /* do some ping result processing */
    PING_RESULT(0);
}

static bool ping_resolve_address(char* server, ip_addr_t* ip_addr)
{
    struct addrinfo *res, *ainfo;
    struct addrinfo hints;
    int error, len=-1;
    char port[12] = {0};
    bool find_ip = false;

    PING_LOGI("DNS resolve server: %s\r\n", server);
    memset ((char *)&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    snprintf(port, sizeof(port), "%d", 80);

    error = getaddrinfo(server, port, &hints, &res);

    if (error != 0) {
        PING_LOGI("DNS resolve server error:%d\r\n", error);
        return false;
    }

    for (ainfo = res; ainfo != NULL; ainfo = ainfo->ai_next) {
        switch (ainfo->ai_family) {
            case AF_INET6: {
              ip6_addr_t ip6_addr;
              struct sockaddr_in6* p_sockaddr_in = (struct sockaddr_in6 *)(ainfo->ai_addr);
              inet6_addr_to_ip6addr(&ip6_addr, &(p_sockaddr_in->sin6_addr));
              ip6_2_ip(&ip6_addr,ip_addr);
              find_ip = true;
              break;
            }
            case AF_INET: {
                ip4_addr_t ip4_addr;
                struct sockaddr_in* p_sockaddr_in = (struct sockaddr_in *)(ainfo->ai_addr);
                inet_addr_to_ipaddr(&ip4_addr, &(p_sockaddr_in->sin_addr));
                ip4_2_ip(&ip4_addr,ip_addr);
                find_ip = true;
                break;
            }
            default:
                break;
        }

        if (find_ip == true) {
            break;
        }
    }

    finish:
    freeaddrinfo(res);
    return find_ip;
}

static void
ping_thread(void *arg)
{
    int s;
    int recv_timeout;
    uint32_t interval = 1;
    u32_t ping_timeout = 0;
    uint32_t* p_msg = NULL;
    ping_msg_type_t msg;
    u32_t cur_time = 0;
    struct timeval timeout;
    u32_t residual_count = (((ping_arg_t *)arg)->count);
    g_ping_state = PING_RUN;
    ping_request_result_t callback = ((ping_arg_t *)arg)->callback;
    ping_static_t ping_static = {0};
    ping_result_t ping_result = {0};
    ping_arg_t * ping_context = (ping_arg_t *)arg;
    ip_addr_t server_ip;
    bool running = true;
    bool send_packet = false;
    recv_timeout = (((ping_arg_t *)arg)->recv_timeout);

    memset(&server_ip, 0, sizeof(server_ip));
    timeout.tv_sec  = recv_timeout/1000;
    timeout.tv_usec = (recv_timeout%1000)*1000;
    ping_static.size = (((ping_arg_t *)arg)->size);
    ping_static.ping_seq_num = rand();
    ping_static.count = (((ping_arg_t *)arg)->count);

    ping_static.ping_lost_num = 0;
    ping_static.ping_recv_num = 0;

    if (ping_resolve_address(ping_context->serv_addr, &server_ip)== false) {
        PING_LOGI("ping resolve address failed\r\n");
        running = false;
    } else {
        int soc_result = 0;
        int nw_type = 0;
        send_packet = true;
        if (nw_type == 0) {
            if (IP_IS_V6_VAL(server_ip)) {
                s = lwip_socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);
            } else {
                s = lwip_socket(AF_INET, SOCK_RAW, IP_PROTO_ICMP);
            }
        }
        if (s < 0) {
            PING_LOGI("create socket failed\r\n");
            running = false;
        }
    }

    while(running == true){
        ping_timeout = sys_arch_mbox_fetch(&ping_mbox, (void **)(&p_msg), interval);
        if(ping_timeout == SYS_ARCH_TIMEOUT) {
            PING_LOGI("SYS_ARCH_TIMEOUT\r\n");
            send_packet = true;
        } else {
            msg = (ping_msg_type_t)p_msg;
            switch(msg){
                case PING_MSG_CLOSE_REQ: {
                    send_packet = false;
                    PING_LOGI("PING_MSG_CLOSE_REQ\r\n");
                    g_ping_state = PING_CLOSE;
                    running = false;
                    continue;
                    break;
                }
                default: {
                    break;
                }
            }
        }
        if (send_packet == false) {
            continue;
        }

        cur_time = time_now();
        if (IP_IS_V6_VAL(server_ip)){
            if (ping6_send(s, ip_2_ip6(&server_ip), &ping_static) == ERR_OK)
            {
                ping_static.ping_timeout = recv_timeout;
                ping6_recv(s, ip_2_ip6(&server_ip), &ping_static, callback);
            }
            else
            {
                LWIP_DEBUGF( PING_DEBUG, ("ping: send "));
                ping_static.ping_lost_num = ping_static.ping_lost_num + 1;
                PING_LOGI(" - error");
            }
        } else {
            if (ping_send(s, ip_2_ip4(&server_ip), &ping_static) == ERR_OK)
            {
                ping_static.ping_timeout = recv_timeout;
                ping_recv(s, ip_2_ip4(&server_ip), &ping_static, callback);
            }
            else
            {
                LWIP_DEBUGF( PING_DEBUG, ("ping: send "));
                ping_static.ping_lost_num = ping_static.ping_lost_num + 1;
                PING_LOGI(" - error");
            }
        }

        if (0 != (ping_static.count))
        {
            residual_count--;
        }
        else
        {
            residual_count = 1;
        }
        if (residual_count == 0){
            running = false;
            PING_LOGI("ping will closed, residual_count == 0\r\n");
            g_ping_state = PING_CLOSE;
            break;
        }

        interval = (((ping_arg_t *)arg)->interval);
        cur_time = get_duration(cur_time);
        if ( cur_time >= interval)
        {
            interval = 1;
        }else
        {
            interval = interval - cur_time;
        }
    }

    ping_static.ping_avg_time = (int)((ping_static.ping_avg_time)/ping_static.ping_recv_num);
    ping_result.min_time = (int)ping_static.ping_min_time;
    ping_result.max_time = (int)ping_static.ping_max_time;
    ping_result.avg_time = (int)ping_static.ping_avg_time;
    ping_result.total_num = (int)ping_static.count - residual_count;
    ping_result.recv_num = (int)ping_static.ping_recv_num;
    ping_result.lost_num = (int)ping_static.ping_lost_num;
    if (!IP_IS_V6_VAL(server_ip)) {
        PING_LOGI("%"U16_F".%"U16_F".%"U16_F".%"U16_F", Packets: Sent = %d, Received =%d, Lost = %d (%d%% loss)",\
                          ip4_addr1_16(ip_2_ip4(&server_ip)),         \
                          ip4_addr2_16(ip_2_ip4(&server_ip)),         \
                          ip4_addr3_16(ip_2_ip4(&server_ip)),         \
                          ip4_addr4_16(ip_2_ip4(&server_ip)),         \
                          (int)ping_result.total_num,         \
                          (int)ping_result.recv_num,          \
                          (int)ping_result.lost_num,          \
                          (int)((ping_result.lost_num * 100)/ping_result.total_num));
    } else {
        PING_LOGI("%" X16_F ":%" X16_F ":%" X16_F ":%" X16_F ":%" X16_F ":%" X16_F ":%" X16_F ":%" X16_F", Packets: Sent = %d, Received =%d, Lost = %d (%d%% loss)",\
                          IP6_ADDR_BLOCK1(ip_2_ip6(&server_ip)),      \
                          IP6_ADDR_BLOCK2(ip_2_ip6(&server_ip)),      \
                          IP6_ADDR_BLOCK3(ip_2_ip6(&server_ip)),      \
                          IP6_ADDR_BLOCK4(ip_2_ip6(&server_ip)),      \
                          IP6_ADDR_BLOCK5(ip_2_ip6(&server_ip)),      \
                          IP6_ADDR_BLOCK6(ip_2_ip6(&server_ip)),      \
                          IP6_ADDR_BLOCK7(ip_2_ip6(&server_ip)),      \
                          IP6_ADDR_BLOCK8(ip_2_ip6(&server_ip)),      \
                          (int)ping_result.total_num,         \
                          (int)ping_result.recv_num,          \
                          (int)ping_result.lost_num,          \
                          (int)((ping_result.lost_num * 100)/ping_result.total_num));
    }
    PING_LOGI(" Packets: min = %d, max =%d, avg = %d", (int)ping_result.min_time, (int)ping_result.max_time, (int)ping_result.avg_time);
    if(callback != NULL)
    {
        callback(PING_TOTAL_RESULT, &ping_result);
    }
    lwip_close(s);
    vPortFree(arg);
    if (ping_mbox != 0U){
        while(1){
            if (SYS_MBOX_EMPTY == sys_mbox_tryfetch(&ping_mbox, (void **)(&p_msg))){
                break;
            }
        }
        sys_mbox_free(&ping_mbox);
        ping_mbox = 0U;
    }else{
        PING_LOGE("ping state wrong\r\n");
    }
    g_ping_state = PING_IDLE;
    vTaskDelete(NULL);
}


#if LWIP_IPV6
static void ping6_prepare_echo( struct icmp6_echo_hdr *iecho, u16_t len, ping_static_t *p_ping_static)
{
    size_t i;
    size_t data_len = len - sizeof(struct icmp6_echo_hdr);

    iecho->type   = ICMP6_TYPE_EREQ;
    iecho->code = 0;
    iecho->chksum = 0;
    iecho->id     = PING_ID;
    (p_ping_static->ping_seq_num) = (p_ping_static->ping_seq_num) + 1;
    iecho->seqno  = htons(p_ping_static->ping_seq_num);

    /* fill the additional data buffer with some data */
    for(i = 0; i < data_len; i++)
    {
        ((char*)iecho)[sizeof(struct icmp6_echo_hdr) + i] = (char)i;
    }
    //iecho->chksum = inet_chksum(iecho, len);
}


static err_t
ping6_send(int s, ip6_addr_t *addr, ping_static_t *p_ping_static)
{
    int err;
    struct icmp6_echo_hdr *iecho;
    struct sockaddr_in6 to;
    int cksum_offset = 2;
    size_t ping_size = sizeof(struct icmp6_echo_hdr) + (p_ping_static->size);
    LWIP_ASSERT("ping_size is too big", ping_size <= 0xffff);

    iecho = (struct icmp6_echo_hdr *)mem_malloc((mem_size_t)ping_size);
    if (!iecho)
    {
        return ERR_MEM;
    }

    ping6_prepare_echo(iecho, (u16_t)ping_size, p_ping_static);

    lwip_setsockopt(s, IPPROTO_RAW, IPV6_CHECKSUM, &cksum_offset, sizeof(int));

    PING_LOGI("ping: send seq(0x%04X) %" X16_F ":%" X16_F ":%" X16_F ":%" X16_F ":%" X16_F ":%" X16_F ":%" X16_F ":%" X16_F, \
                        p_ping_static->ping_seq_num,\
                        IP6_ADDR_BLOCK1(addr),         \
                        IP6_ADDR_BLOCK2(addr),         \
                        IP6_ADDR_BLOCK3(addr),         \
                        IP6_ADDR_BLOCK4(addr),         \
                        IP6_ADDR_BLOCK5(addr),         \
                        IP6_ADDR_BLOCK6(addr),         \
                        IP6_ADDR_BLOCK7(addr),         \
                        IP6_ADDR_BLOCK8(addr));

    to.sin6_len = sizeof(to);
    to.sin6_family = AF_INET6;
    inet6_addr_from_ip6addr(&to.sin6_addr, addr);
    p_ping_static->ping_time = time_now();

    err = lwip_sendto(s, iecho, ping_size, 0, (struct sockaddr*)&to, sizeof(to));

    mem_free(iecho);

    return (err ? ERR_OK : ERR_VAL);
}

static void
ping6_recv(int s, ip6_addr_t *addr, ping_static_t *p_ping_static, ping_request_result_t callback)
{
    char buf[64];
    int fromlen, len;
    struct sockaddr_in6 from;
    struct ip6_hdr *ip6hdr;
    struct icmp6_echo_hdr *iecho;
    ping_packet_result_t packet_result = {0};
    struct timeval timeout = {0};

    timeout.tv_sec  = (p_ping_static->ping_timeout)/1000;
    timeout.tv_usec = ((p_ping_static->ping_timeout)%1000)*1000;
    lwip_setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    fromlen = sizeof(struct sockaddr_in6);
    while((len = lwip_recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*)&from,
                               (socklen_t*)&fromlen)) > 0)
    {
        if (len >= (int)(sizeof(struct ip6_hdr) + sizeof(struct icmp6_echo_hdr)))
        {
            ip6_addr_t fromaddr;
            u32_t cur_time = get_duration(p_ping_static->ping_time);

            inet6_addr_to_ip6addr(&fromaddr, &from.sin6_addr);
            /* LWIP_DEBUGF( PING_DEBUG, ("ping: recv ")); */
            ip6hdr = (struct ip6_hdr *)buf;
            iecho = (struct icmp6_echo_hdr *)(buf + IP6_HLEN);

            /* ignore packet if it is not ping reply */
            if ((IP6H_NEXTH(ip6hdr) != IP6_NEXTH_ICMP6) || (ip6_addr_cmp(addr, &fromaddr) == 0) ||
               (iecho->type != ICMP6_TYPE_EREP))
            {
                if (cur_time > p_ping_static->ping_timeout)
                {
                    PING_LOGI("--- ping: timeout");
                    p_ping_static->ping_lost_num = p_ping_static->ping_lost_num + 1;

                    return;
                }
                else
                {
                    if (IP6H_NEXTH(ip6hdr) != IP6_NEXTH_ICMP6) {
                        PING_LOGI("ping: not icmpv6");
                    }
                    if (ip6_addr_cmp(addr, &fromaddr) == 0) {
                        PING_LOGI("ping: not match");
                    }
                    timeout.tv_sec  = (p_ping_static->ping_timeout - cur_time)/1000;
                    timeout.tv_usec = ((p_ping_static->ping_timeout - cur_time)%1000)*1000;
                    lwip_setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
                    PING_LOGI("ping v6 reset timeout\r\n");
                    continue;
                }
            }


            if ((iecho->id == PING_ID) && (iecho->seqno == htons(p_ping_static->ping_seq_num)))
            {
                PING_LOGI("ping: recv seq(0x%04X) %" X16_F ":%" X16_F ":%" X16_F ":%" X16_F ":%" X16_F ":%" X16_F ":%" X16_F ":%" X16_F", %"U32_F" ms", \
                                    htons(iecho->seqno),             \
                                    IP6_ADDR_BLOCK1(&fromaddr),         \
                                    IP6_ADDR_BLOCK2(&fromaddr),         \
                                    IP6_ADDR_BLOCK3(&fromaddr),         \
                                    IP6_ADDR_BLOCK4(&fromaddr),         \
                                    IP6_ADDR_BLOCK5(&fromaddr),         \
                                    IP6_ADDR_BLOCK6(&fromaddr),         \
                                    IP6_ADDR_BLOCK7(&fromaddr),         \
                                    IP6_ADDR_BLOCK8(&fromaddr),         \
                                    cur_time);
                if (cur_time > p_ping_static->ping_timeout)
                {
                    PING_LOGI("ping6: timeout");
                    packet_result.is_timeout = true;
                    callback(PING_PACKET_RESULT, &packet_result);
                    p_ping_static->ping_lost_num = p_ping_static->ping_lost_num + 1;
                    return;
                }
                packet_result.ip_address[0] = IP6_ADDR_BLOCK1(&fromaddr);
                packet_result.ip_address[1] = IP6_ADDR_BLOCK2(&fromaddr);
                packet_result.ip_address[2] = IP6_ADDR_BLOCK3(&fromaddr);
                packet_result.ip_address[3] = IP6_ADDR_BLOCK4(&fromaddr);
                packet_result.ip_address[4] = IP6_ADDR_BLOCK5(&fromaddr);
                packet_result.ip_address[5] = IP6_ADDR_BLOCK6(&fromaddr);
                packet_result.ip_address[6] = IP6_ADDR_BLOCK7(&fromaddr);
                packet_result.ip_address[7] = IP6_ADDR_BLOCK8(&fromaddr);
                packet_result.is_ipv4 = false;
                packet_result.packet_size = IP6H_PLEN(ip6hdr) - sizeof(struct icmp6_echo_hdr);
                packet_result.rtt = cur_time;
                packet_result.ttl = IP6H_HOPLIM(ip6hdr);
                packet_result.is_timeout = false;
                callback(PING_PACKET_RESULT, &packet_result);

                /* LWIP_DEBUGF( PING_DEBUG, (" %"U32_F" ms\n", (sys_now() - ping_time))); */
                if(p_ping_static->ping_min_time == 0 || p_ping_static->ping_min_time > cur_time)
                {
                    p_ping_static->ping_min_time = cur_time;
                }
                if(p_ping_static->ping_max_time == 0 || p_ping_static->ping_max_time < cur_time)
                {
                    p_ping_static->ping_max_time = cur_time;
                }
                p_ping_static->ping_avg_time = p_ping_static->ping_avg_time + cur_time;

                p_ping_static->ping_recv_num = p_ping_static->ping_recv_num + 1;

                /* do some ping result processing */
                PING_RESULT((ICMPH_TYPE(iecho) == ICMP_ER));
                return;
            }
            else
            {
                /* Treat ping ack received after timeout as success */
                PING_LOGI("ping: Get ping ACK seq(0x%04X), expected seq(0x%04X)", htons(iecho->seqno), p_ping_static->ping_seq_num);
                /* Can not return, due to there could be ping ack which has matched sequence num. */
                timeout.tv_sec  = (p_ping_static->ping_timeout - cur_time)/1000;
                timeout.tv_usec = ((p_ping_static->ping_timeout - cur_time)%1000)*1000;
                lwip_setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
            }

        }
    }

    if (-1 ==len)
    {
        packet_result.is_timeout = true;
        callback(PING_PACKET_RESULT, &packet_result);
        p_ping_static->ping_lost_num = p_ping_static->ping_lost_num + 1;
    }

    /* do some ping result processing */
    PING_RESULT(0);
}

#endif
#endif //#if PING_USE_SOCKETS

ping_status_t ping_close(void)
{
    ping_msg_type_t* p_msg = NULL;

    if (PING_RUN != g_ping_state){
        return PING_ERROR;
    }

    if (!sys_mbox_valid_val(ping_mbox)) {
        PING_LOGE("PING mailbox is invalid.");
        return PING_ERROR;
    }
    p_msg = (ping_msg_type_t*)PING_MSG_CLOSE_REQ;
    if (sys_mbox_trypost(&ping_mbox, p_msg) != ERR_OK) {
        PING_LOGE("Send msg fail.");
        return PING_ERROR;
    }
    g_ping_state = PING_CLOSE;
    return PING_OK;
}

ping_status_t ping_request_ex(uint8_t addr_type, ping_arg_t *para)
{
    int ret = 0;
    ping_arg_t *ping_arg = NULL;

    if (PING_IDLE != g_ping_state){
        return PING_RUNNING;
    }
    g_ping_state = PING_RUN;
    PING_LOGI("PING: %s\r\n", para->serv_addr);
    ping_arg = (ping_arg_t *)pvPortMalloc(sizeof(ping_arg_t));
    memcpy(ping_arg, para, sizeof(ping_arg_t));
    if(sys_mbox_new(&ping_mbox, PING_MBOX_SIZE) != ERR_OK) {
        PING_LOGI("Failed to create PING mbox.");
        vPortFree(ping_arg);
        g_ping_state = PING_IDLE;
        return PING_ERROR;
    }
    sys_thread_new(PING_TASK_NAME, ping_thread, (void *)ping_arg, PING_TASK_STACKSIZE / sizeof(portSTACK_TYPE), TASK_PRIORITY_ABOVE_NORMAL);
    return PING_OK;
}

#endif /* LWIP_RAW */
