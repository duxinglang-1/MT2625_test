/*******************************************************************************
 *
 * Copyright (c) 2015 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Simon Bernard - initial API and implementation
 *    Christian Renz - Please refer to git log
 *
 *******************************************************************************/

#ifndef MBEDTLS_CONNECTION_H_
#define MBEDTLS_CONNECTION_H_

#include <stdio.h>
#include <unistd.h>
#include <lwip/inet.h>
#include <lwip/netdb.h>
#include <lwip/sockets.h>
#include <lwip/stats.h>
#include "netif/etharp.h"

#include "liblwm2m.h"

#include "mbedtls/net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
#include "mbedtls/timing.h"

#define LWM2M_STANDARD_PORT_STR "5683"
#define LWM2M_STANDARD_PORT      5683
#define LWM2M_DTLS_PORT_STR     "5684"
#define LWM2M_DTLS_PORT          5684
#define LWM2M_BSSERVER_PORT_STR "5685"
#define LWM2M_BSSERVER_PORT      5685

// after 40sec of inactivity we rehandshake
#define DTLS_NAT_TIMEOUT 90

typedef struct _mbedtls_connection_t
{
    struct _mbedtls_connection_t *  next;
    int                     sock;
    struct sockaddr_in6     addr;
    size_t                  addrLen;
    int              dtlsSession;
    lwm2m_object_t * securityObj;
    int securityInstId;
    lwm2m_context_t * lwm2mH;
    mbedtls_net_context server_fd;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_timing_delay_context timer;
#if 0//defined(MBEDTLS_X509_CRT_PARSE_C)
    mbedtls_x509_crt cacert;
    mbedtls_x509_crt clicert;
    mbedtls_pk_context pkey;
#endif
    time_t lastSend; // last time a data was sent to the server (used for NAT timeouts)
} mbedtls_connection_t;

/********************* Security Obj Helpers **********************/
char * security_get_uri(lwm2m_object_t * obj, int instanceId, char * uriBuffer, int bufferSize);
int64_t security_get_mode(lwm2m_object_t * obj, int instanceId);
char * security_get_public_id(lwm2m_object_t * obj, int instanceId, int * length);
char * security_get_secret_key(lwm2m_object_t * obj, int instanceId, int * length);

int create_socket(const char * portStr, int ai_family);

mbedtls_connection_t * connection_find(mbedtls_connection_t * connList, const struct sockaddr_storage * addr, size_t addrLen);
mbedtls_connection_t * connection_new_incoming(mbedtls_connection_t * connList, int sock, const struct sockaddr * addr, size_t addrLen);
mbedtls_connection_t * connection_create(mbedtls_connection_t * connList, int sock, lwm2m_object_t * securityObj, int instanceId, lwm2m_context_t * lwm2mH, int addressFamily);

void connection_free(mbedtls_connection_t * connList);

int connection_send(mbedtls_connection_t *connP, uint8_t * buffer, size_t length);
int connection_handle_packet(mbedtls_connection_t *connP);

// rehandshake a connection, useful when your NAT timed out and your client has a new IP/PORT
int connection_rehandshake(mbedtls_connection_t *connP, bool sendCloseNotify);

#endif
