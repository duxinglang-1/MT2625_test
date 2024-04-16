
#ifndef __PING_H__
#define __PING_H__


#include <stdint.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * PING_USE_SOCKETS: Set to 1 to use sockets, otherwise the raw api is used
 */
#ifndef PING_USE_SOCKETS
#define PING_USE_SOCKETS    LWIP_SOCKET
#endif

#define PING_IP_ADDR_V4      0
#define PING_IP_ADDR_V6      1

#ifndef PING_SERVER_MAX_LEN
#define PING_SERVER_MAX_LEN 128
#endif

typedef struct _ping_result
{
    uint32_t min_time;
    uint32_t max_time;
    uint32_t avg_time;
    uint32_t total_num;
    uint32_t lost_num;
    uint32_t recv_num;
} ping_result_t;

typedef struct {
    bool     is_timeout;     /*When it is true, other data is invalid.*/
    uint32_t rtt;            /*The unit is ms.*/
    uint32_t ttl;            /*The TTL value in ping response packet.*/
    uint32_t packet_size;    /*The unit is byte.*/
    bool     is_ipv4;        /*ipv4 is true, ipv6 is false.*/
    uint16_t ip_address[8];  /*The address has been translated by ping thread.*/
} ping_packet_result_t;

typedef enum {
    PING_PACKET_RESULT,
    PING_TOTAL_RESULT
} ping_result_type_t;

typedef void (* ping_request_result_t)(ping_result_type_t type, void* result);

typedef enum {
    PING_OK       = 0,
    PING_ERROR    = 1,
    PING_RUNNING  = 2
} ping_status_t;

typedef struct _ping_arg
{
    u32_t count;
    u32_t size;
    u32_t recv_timeout;
    u32_t interval;
    ping_request_result_t callback;
    char serv_addr[PING_SERVER_MAX_LEN];
} ping_arg_t;

ping_status_t ping_request_ex(uint8_t addr_type, ping_arg_t *para);

ping_status_t ping_close(void);

#ifdef __cplusplus
}
#endif

#endif /* __PING_H__ */

