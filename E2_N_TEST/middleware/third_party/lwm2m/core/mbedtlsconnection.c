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
 *    David Navarro, Intel Corporation - initial API and implementation
 *    Christian Renz - Please refer to git log
 *
 *******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "liblwm2m.h"
#include "internals.h"

#ifdef WITH_MBEDTLS
#include "mbedtlsconnection.h"

#define COAP_PORT "5683"
#define COAPS_PORT "5684"
#define URI_LENGTH 256

/********************* Security Obj Helpers **********************/
char * security_get_uri(lwm2m_object_t * obj, int instanceId, char * uriBuffer, int bufferSize){
    int size = 1;
    lwm2m_data_t * dataP = lwm2m_data_new(size);
    dataP->id = 0; // security server uri

    obj->readFunc(instanceId, &size, &dataP, obj);
    if (dataP != NULL &&
            dataP->type == LWM2M_TYPE_STRING &&
            dataP->value.asBuffer.length > 0)
    {
        if (bufferSize > dataP->value.asBuffer.length){
            memset(uriBuffer,0,dataP->value.asBuffer.length+1);
            strncpy(uriBuffer,(char*)dataP->value.asBuffer.buffer,dataP->value.asBuffer.length);
            lwm2m_data_free(size, dataP);
            return uriBuffer;
        }
    }
    lwm2m_data_free(size, dataP);
    return NULL;
}

int64_t security_get_mode(lwm2m_object_t * obj, int instanceId){
    int64_t mode;
    int size = 1;
    lwm2m_data_t * dataP = lwm2m_data_new(size);
    dataP->id = 2; // security mode

    obj->readFunc(instanceId, &size, &dataP, obj);
    if (0 != lwm2m_data_decode_int(dataP,&mode))
    {
        lwm2m_data_free(size, dataP);
        return mode;
    }

    lwm2m_data_free(size, dataP);
    LOG("Unable to get security mode : use not secure mode");
    return LWM2M_SECURITY_MODE_NONE;
}

char * security_get_public_id(lwm2m_object_t * obj, int instanceId, int * length){
    int size = 1;
    lwm2m_data_t * dataP = lwm2m_data_new(size);
    dataP->id = 3; // public key or id

    obj->readFunc(instanceId, &size, &dataP, obj);
    if (dataP != NULL &&
        dataP->type == LWM2M_TYPE_OPAQUE)
    {
        char * buff;

        buff = (char*)lwm2m_malloc(dataP->value.asBuffer.length);
        if (buff != 0)
        {
            memcpy(buff, dataP->value.asBuffer.buffer, dataP->value.asBuffer.length);
            *length = dataP->value.asBuffer.length;
        }
        lwm2m_data_free(size, dataP);

        return buff;
    } else {
        return NULL;
    }
}


char * security_get_secret_key(lwm2m_object_t * obj, int instanceId, int * length){
    int size = 1;
    lwm2m_data_t * dataP = lwm2m_data_new(size);
    dataP->id = 5; // secret key

    obj->readFunc(instanceId, &size, &dataP, obj);
    if (dataP != NULL &&
        dataP->type == LWM2M_TYPE_OPAQUE)
    {
        char * buff;

        buff = (char*)lwm2m_malloc(dataP->value.asBuffer.length);
        if (buff != 0)
        {
            memcpy(buff, dataP->value.asBuffer.buffer, dataP->value.asBuffer.length);
            *length = dataP->value.asBuffer.length;
        }
        lwm2m_data_free(size, dataP);

        return buff;
    } else {
        return NULL;
    }
}

/********************* Security Obj Helpers Ends **********************/

int send_data(mbedtls_connection_t *connP,
                    uint8_t * buffer,
                    size_t length)
{
    int nbSent;
    size_t offset;

#ifdef WITH_LOGS
    char s[INET6_ADDRSTRLEN];
    in_port_t port;

    s[0] = 0;

    if (AF_INET == connP->addr.sin6_family)
    {
        struct sockaddr_in *saddr = (struct sockaddr_in *)&connP->addr;
        inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET6_ADDRSTRLEN);
        port = saddr->sin_port;
    }
    else if (AF_INET6 == connP->addr.sin6_family)
    {
        struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)&connP->addr;
        inet_ntop(saddr->sin6_family, &saddr->sin6_addr, s, INET6_ADDRSTRLEN);
        port = saddr->sin6_port;
    }

    LOG_ARG("Sending %d bytes to [%s]:%hu\r\n", length, s, ntohs(port));

    output_buffer(stderr, buffer, length, 0);
#endif

    offset = 0;
    while (offset != length)
    {
        nbSent = sendto(connP->sock, buffer + offset, length - offset, 0, (struct sockaddr *)&(connP->addr), connP->addrLen);
        if (nbSent == -1) return -1;
        offset += nbSent;
    }
    connP->lastSend = lwm2m_gettime();
    return 0;
}

int get_port(struct sockaddr *x)
{
   if (x->sa_family == AF_INET)
   {
       return ((struct sockaddr_in *)x)->sin_port;
   } else if (x->sa_family == AF_INET6) {
       return ((struct sockaddr_in6 *)x)->sin6_port;
   } else {
       LOG("non IPV4 or IPV6 address\n");
       return  -1;
   }
}

int sockaddr_cmp(struct sockaddr *x, struct sockaddr *y)
{
    int portX = get_port(x);
    int portY = get_port(y);

    // if the port is invalid of different
    if (portX == -1 || portX != portY)
    {
        return 0;
    }

    // IPV4?
    if (x->sa_family == AF_INET)
    {
        // is V4?
        if (y->sa_family == AF_INET)
        {
            // compare V4 with V4
            return ((struct sockaddr_in *)x)->sin_addr.s_addr == ((struct sockaddr_in *)y)->sin_addr.s_addr;
            // is V6 mapped V4?
        } else if (0){//IN6_IS_ADDR_V4MAPPED(&((struct sockaddr_in6 *)y)->sin6_addr)) {
            struct in6_addr* addr6 = &((struct sockaddr_in6 *)y)->sin6_addr;
            uint32_t y6to4 = addr6->s6_addr[15] << 24 | addr6->s6_addr[14] << 16 | addr6->s6_addr[13] << 8 | addr6->s6_addr[12];
            return y6to4 == ((struct sockaddr_in *)x)->sin_addr.s_addr;
        } else {
            return 0;
        }
    } else if (x->sa_family == AF_INET6 && y->sa_family == AF_INET6) {
        // IPV6 with IPV6 compare
        return memcmp(((struct sockaddr_in6 *)x)->sin6_addr.s6_addr, ((struct sockaddr_in6 *)y)->sin6_addr.s6_addr, 16) == 0;
    } else {
        // unknown address type
        LOG("non IPV4 or IPV6 address\n");
        return 0;
    }
}

int create_socket(const char * portStr, int ai_family)
{
    int s = -1;
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = ai_family;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if (0 != getaddrinfo(NULL, portStr, &hints, &res))
    {
        return -1;
    }

    for(p = res ; p != NULL && s == -1 ; p = p->ai_next)
    {
        s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s >= 0)
        {
            if (-1 == bind(s, p->ai_addr, p->ai_addrlen))
            {
                close(s);
                s = -1;
            }
        }
    }

    freeaddrinfo(res);

    return s;
}

static void my_debug( void *ctx, int level,
                      const char *file, int line,
                      const char *str )
{
    ((void) level);

    LOG_ARG("mbedtls[%d]%s:%04d: %s\r\n", level, file ? file : "", line, str ? str : "");
}

struct _hr_time
{
    struct timeval start;
};

unsigned long mbedtls_timing_get_timer( struct mbedtls_timing_hr_time *val, int reset )
{
    unsigned long delta;
    struct timeval offset;
    struct _hr_time *t = (struct _hr_time *) val;

    gettimeofday( &offset, NULL );

    if( reset )
    {
        t->start.tv_sec  = offset.tv_sec;
        t->start.tv_usec = offset.tv_usec;
        return( 0 );
    }

    delta = ( offset.tv_sec  - t->start.tv_sec  ) * 1000
          + ( offset.tv_usec - t->start.tv_usec ) / 1000;

    return( delta );
}

/*
 * Set delays to watch
 */
void mbedtls_timing_set_delay( void *data, uint32_t int_ms, uint32_t fin_ms )
{
    mbedtls_timing_delay_context *ctx = (mbedtls_timing_delay_context *) data;

    ctx->int_ms = int_ms;
    ctx->fin_ms = fin_ms;

    if( fin_ms != 0 )
        (void) mbedtls_timing_get_timer( &ctx->timer, 1 );
}

/*
 * Get number of delays expired
 */
int mbedtls_timing_get_delay( void *data )
{
    mbedtls_timing_delay_context *ctx = (mbedtls_timing_delay_context *) data;
    unsigned long elapsed_ms;

    if( ctx->fin_ms == 0 )
        return( -1 );

    elapsed_ms = mbedtls_timing_get_timer( &ctx->timer, 0 );

    if( elapsed_ms >= ctx->fin_ms )
        return( 2 );

    if( elapsed_ms >= ctx->int_ms )
        return( 1 );

    return( 0 );
}

int connection_init( mbedtls_connection_t * connP, char * host, char * port)
{
    int ret = 0;
    const char *pers = "dtls_client";
#if 0//defined(MBEDTLS_X509_CRT_PARSE_C)
    uint32_t flags;
#endif

    /*
     * Make sure memory references are valid.
     */
    mbedtls_net_init( &connP->server_fd );
    mbedtls_ssl_init( &connP->ssl );
    mbedtls_ssl_config_init( &connP->conf );
    mbedtls_ctr_drbg_init( &connP->ctr_drbg );
#if 0//defined(MBEDTLS_X509_CRT_PARSE_C)
    mbedtls_x509_crt_init( &connP->cacert );
    mbedtls_x509_crt_init( &connP->clicert );
    mbedtls_pk_init( &connP->pkey );
#endif

#if defined(MBEDTLS_DEBUG_C)
    mbedtls_debug_set_threshold( 4 );
#endif

    /*
     * 0. Initialize the RNG and the session data
     */
    LOG( "  . Seeding the random number generator..." );

    mbedtls_entropy_init( &connP->entropy );
    if( ( ret = mbedtls_ctr_drbg_seed( &connP->ctr_drbg, mbedtls_entropy_func, &connP->entropy,
                               (const unsigned char *) pers,
                               strlen( pers ) ) ) != 0 )
    {
        LOG_ARG( " failed  ! mbedtls_ctr_drbg_seed returned -0x%x\n", -ret );
        goto exit;
    }

    LOG( " ok\n" );

#if 0//defined(MBEDTLS_X509_CRT_PARSE_C)
    /*
     * 1.1. Load the trusted CA
     */
    LOG( "  . Loading the CA root certificate ..." );

#if defined(MBEDTLS_CERTS_C)
        for( i = 0; mbedtls_test_cas[i] != NULL; i++ )
        {
            ret = mbedtls_x509_crt_parse( &cacert,
                                  (const unsigned char *) mbedtls_test_cas[i],
                                  mbedtls_test_cas_len[i] );
            if( ret != 0 )
                break;
        }
#else
    {
        ret = 1;
        mbedtls_printf("MBEDTLS_CERTS_C not defined.");
    }
#endif
    if( ret < 0 )
    {
        LOG_ARG( " failed  !  mbedtls_x509_crt_parse returned -0x%x\n\n", -ret );
        goto exit;
    }

    LOG_ARG( " ok (%d skipped)\n", ret );

    /*
     * 1.2. Load own certificate and private key
     *
     * (can be skipped if client authentication is not required)
     */
    LOG( "  . Loading the client cert. and key..." );

#if defined(MBEDTLS_CERTS_C)
        ret = mbedtls_x509_crt_parse( &clicert, (const unsigned char *) mbedtls_test_cli_crt,
                mbedtls_test_cli_crt_len );
#else
    {
        ret = 1;
        LOG("MBEDTLS_CERTS_C not defined.");
    }
#endif
    if( ret != 0 )
    {
        LOG_ARG( " failed  !  mbedtls_x509_crt_parse returned -0x%x\n\n", -ret );
        goto exit;
    }

#if defined(MBEDTLS_CERTS_C)
        ret = mbedtls_pk_parse_key( &pkey, (const unsigned char *) mbedtls_test_cli_key,
                mbedtls_test_cli_key_len, NULL, 0 );
#else
    {
        ret = 1;
        LOG("MBEDTLS_CERTS_C not defined.");
    }
#endif
    if( ret != 0 )
    {
        LOG_ARG( " failed  !  mbedtls_pk_parse_key returned -0x%x\n\n", -ret );
        goto exit;
    }

    LOG( " ok\n" );
#endif /* MBEDTLS_X509_CRT_PARSE_C */

    /*
     * 2. Start the connection
     */
    LOG_ARG( "  . Connecting to udp/%s/%s...", host, port );

    if( ( ret = mbedtls_net_connect( &connP->server_fd, host,
                                         port, MBEDTLS_NET_PROTO_UDP ) ) != 0 )
    {
        LOG_ARG( " failed  ! mbedtls_net_connect returned -0x%x\n\n", -ret );
        goto exit;
    }

    ret = mbedtls_net_set_block( &connP->server_fd );
    if( ret != 0 )
    {
        LOG_ARG( " failed  ! net_set_block() returned -0x%x\n\n", -ret );
        goto exit;
    }

    LOG( " ok\n" );

    /*
     * 3. Setup stuff
     */
    LOG( "  . Setting up the DTLS structure..." );

    if( ( ret = mbedtls_ssl_config_defaults( &connP->conf,
                    MBEDTLS_SSL_IS_CLIENT,
                    MBEDTLS_SSL_TRANSPORT_DATAGRAM,
                    MBEDTLS_SSL_PRESET_DEFAULT ) ) != 0 )
    {
        LOG_ARG( " failed  ! mbedtls_ssl_config_defaults returned -0x%x\n\n", -ret );
        goto exit;
    }

    /* OPTIONAL is usually a bad choice for security, but makes interop easier
     * in this simplified example, in which the ca chain is hardcoded.
     * Production code should set a proper ca chain and use REQUIRED. */
    mbedtls_ssl_conf_authmode( &connP->conf, MBEDTLS_SSL_VERIFY_OPTIONAL );
    
#if defined(MBEDTLS_SSL_PROTO_DTLS)
    mbedtls_ssl_conf_handshake_timeout( &connP->conf, 10000, 30000 );
#endif /* MBEDTLS_SSL_PROTO_DTLS */

    mbedtls_ssl_conf_rng( &connP->conf, mbedtls_ctr_drbg_random, &connP->ctr_drbg );
    mbedtls_ssl_conf_dbg( &connP->conf, my_debug, NULL);

    mbedtls_ssl_conf_read_timeout( &connP->conf, 5000 );

#if 0//defined(MBEDTLS_X509_CRT_PARSE_C)
    mbedtls_ssl_conf_ca_chain( &connP->conf, &connP->cacert, NULL );
#endif

#if defined(MBEDTLS_KEY_EXCHANGE__SOME__PSK_ENABLED)
    int keyLen;
    char * key;
    key = security_get_secret_key(connP->securityObj, connP->securityInstId, &keyLen);

    if (MBEDTLS_PSK_MAX_LEN < keyLen)
    {
        LOG_ARG("buffer_length=%d, keyLen=%d\n", MBEDTLS_PSK_MAX_LEN, keyLen);
        LOG("cannot set psk -- buffer too small\n");
        lwm2m_free(key);
        goto exit;
    }

    int idLen;
    char * id;
    id = security_get_public_id(connP->securityObj, connP->securityInstId, &idLen);

    if( ( ret = mbedtls_ssl_conf_psk( &connP->conf, (unsigned char*)key, keyLen, (unsigned char*)id, idLen ) ) != 0 )
    {
        LOG_ARG( " failed  ! mbedtls_ssl_conf_psk returned %d\n\n", ret );
        lwm2m_free(key);
        lwm2m_free(id);
        goto exit;
    }

    lwm2m_free(key);
    lwm2m_free(id);
#endif

    if( ( ret = mbedtls_ssl_setup( &connP->ssl, &connP->conf ) ) != 0 )
    {
        LOG_ARG( " failed  ! mbedtls_ssl_setup returned %d\n\n", ret );
        goto exit;
    }

#if defined(MBEDTLS_X509_CRT_PARSE_C)
    if( ( ret = mbedtls_ssl_set_hostname( &connP->ssl, host ) ) != 0 )
    {
        LOG_ARG( " failed  ! mbedtls_ssl_set_hostname returned %d\n\n", ret );
        goto exit;
    }
#endif

    mbedtls_ssl_set_bio( &connP->ssl, &connP->server_fd,
                         mbedtls_net_send, NULL, mbedtls_net_recv_timeout );

    mbedtls_ssl_set_timer_cb( &connP->ssl, &connP->timer, mbedtls_timing_set_delay,
                                            mbedtls_timing_get_delay );

    LOG( " ok\n" );

    /*
     * 4. Handshake
     */
    LOG( "  . Performing the DTLS handshake..." );

    while( ( ret = mbedtls_ssl_handshake( &connP->ssl ) ) != 0 )
    {
        if( ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE )
        {
            LOG_ARG( " failed  ! mbedtls_ssl_handshake returned -0x%x\n", -ret );
            if( ret == MBEDTLS_ERR_X509_CERT_VERIFY_FAILED )
                LOG_ARG(
                    "    Unable to verify the server's certificate. "
                        "Either it is invalid,\n"
                    "    or you didn't set ca_file or ca_path "
                        "to an appropriate value.\n"
                    "    Alternatively, you may want to use "
                        "auth_mode=optional for testing purposes.\n" );
            LOG( "\n" );
            goto exit;
        }
    }

    LOG_ARG( " ok    [ Protocol is %s ]    [ Ciphersuite is %s ]\n",
            mbedtls_ssl_get_version( &connP->ssl ), mbedtls_ssl_get_ciphersuite( &connP->ssl ) );
#if 0//defined(MBEDTLS_X509_CRT_PARSE_C)
    /*
     * 5. Verify the server certificate
     */
    LOG( "  . Verifying peer X.509 certificate..." );

    /* In real life, we would have used MBEDTLS_SSL_VERIFY_REQUIRED so that the
     * handshake would not succeed if the peer's cert is bad.  Even if we used
     * MBEDTLS_SSL_VERIFY_OPTIONAL, we would bail out here if ret != 0 */
    if( ( flags = mbedtls_ssl_get_verify_result( &connP->ssl ) ) != 0 )
    {
        char vrfy_buf[512];

        LOG( " failed\n" );

        mbedtls_x509_crt_verify_info( vrfy_buf, sizeof( vrfy_buf ), "  ! ", flags );

        LOG_ARG( "%s\n", vrfy_buf );
    }
    else
        LOG( " ok\n" );

    if( mbedtls_ssl_get_peer_cert( &connP->ssl ) != NULL )
    {
        mbedtls_printf( "  . Peer certificate information    ...\n" );
        mbedtls_x509_crt_info( (char *) buf, sizeof( buf ) - 1, "      ",
                       mbedtls_ssl_get_peer_cert( &connP->ssl ) );
        mbedtls_printf( "%s\n", buf );
    }
#endif /* MBEDTLS_X509_CRT_PARSE_C */

    /*
     * 6. Write the echo request
     */

    /*
     * 7. Read the echo response
     */

    /*
     * 8. Done, cleanly close the connection
     */

    /*
     * 9. Final clean-ups and exit
     */
exit:

    return( ret );
}

mbedtls_connection_t * connection_find(mbedtls_connection_t * connList,
                               const struct sockaddr_storage * addr,
                               size_t addrLen)
{
    mbedtls_connection_t * connP;

    connP = connList;
    while (connP != NULL)
    {

       if (sockaddr_cmp((struct sockaddr*) (&connP->addr),(struct sockaddr*) addr)) {
            return connP;
       }

       connP = connP->next;
    }

    return connP;
}

mbedtls_connection_t * connection_new_incoming(mbedtls_connection_t * connList,
                                       int sock,
                                       const struct sockaddr * addr,
                                       size_t addrLen)
{
    mbedtls_connection_t * connP;

    connP = (mbedtls_connection_t *)malloc(sizeof(mbedtls_connection_t));
    if (connP != NULL)
    {
        connP->sock = sock;
        memcpy(&(connP->addr), addr, addrLen);
        connP->addrLen = addrLen;
        connP->next = connList;

        connP->dtlsSession = 1;
        connP->lastSend = lwm2m_gettime();
    }

    return connP;
}

mbedtls_connection_t * connection_create(mbedtls_connection_t * connList,
                                 int sock,
                                 lwm2m_object_t * securityObj,
                                 int instanceId,
                                 lwm2m_context_t * lwm2mH,
                                 int addressFamily)
{
    struct addrinfo hints;
    struct addrinfo *servinfo = NULL;
    struct addrinfo *p;
    int s;
    struct sockaddr *sa;
    socklen_t sl;
    mbedtls_connection_t * connP = NULL;
    char uriBuf[URI_LENGTH];
    char * uri;
    char * host;
    char * port;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = addressFamily;
    hints.ai_socktype = SOCK_DGRAM;

    uri = security_get_uri(securityObj, instanceId, uriBuf, URI_LENGTH);
    if (uri == NULL) return NULL;

    // parse uri in the form "coaps://[host]:[port]"
    char * defaultport;
    if (0 == strncmp(uri, "coaps://", strlen("coaps://")))
    {
        host = uri+strlen("coaps://");
        defaultport = COAPS_PORT;
    }
    else if (0 == strncmp(uri, "coap://", strlen("coap://")))
    {
        host = uri+strlen("coap://");
        defaultport = COAP_PORT;
    }
    else
    {
        return NULL;
    }
    port = strrchr(host, ':');
    if (port == NULL)
    {
        port = defaultport;
    }
    else
    {
        // remove brackets
        if (host[0] == '[')
        {
            host++;
            if (*(port - 1) == ']')
            {
                *(port - 1) = 0;
            }
            else return NULL;
        }
        // split strings
        *port = 0;
        port++;
    }

    if (0 != getaddrinfo(host, port, &hints, &servinfo) || servinfo == NULL) return NULL;

    // we test the various addresses
    s = -1;
    for(p = servinfo ; p != NULL && s == -1 ; p = p->ai_next)
    {
        s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s >= 0)
        {
            sa = p->ai_addr;
            sl = p->ai_addrlen;
            if (-1 == connect(s, p->ai_addr, p->ai_addrlen))
            {
                close(s);
                s = -1;
            }
        }
    }
    if (s >= 0)
    {
        connP = connection_new_incoming(connList, sock, sa, sl);
        close(s);

        // do we need to start mbedtls?
        if (connP != NULL)
        {
            connP->securityObj = securityObj;
            connP->securityInstId = instanceId;
            connP->lwm2mH = lwm2mH;

            if (security_get_mode(connP->securityObj,connP->securityInstId)
                     != LWM2M_SECURITY_MODE_NONE)
            {
                if (connection_init(connP, host, port) != 0) {
                    if (NULL != servinfo) {
                        //free(servinfo);
                        freeaddrinfo(servinfo);
                    }
                    connection_free(connP);
                    return NULL;
                }
            }
            else
            {
                // no dtls session
                connP->dtlsSession = 0;
            }
        }
    }

    if (NULL != servinfo) {
        //free(servinfo);
        freeaddrinfo(servinfo);
    }

    return connP;
}

void connection_free(mbedtls_connection_t * connList)
{
    while (connList != NULL)
    {
        mbedtls_connection_t * nextP;

        nextP = connList->next;
        if (connList->dtlsSession != 0) {
            mbedtls_net_free( &connList->server_fd );
#if 0//defined(MBEDTLS_X509_CRT_PARSE_C)
            mbedtls_x509_crt_free( &connList->cacert );
#endif
            mbedtls_ssl_free( &connList->ssl );
            mbedtls_ssl_config_free( &connList->conf );
            mbedtls_ctr_drbg_free( &connList->ctr_drbg );
            mbedtls_entropy_free( &connList->entropy );
        }
        free(connList);

        connList = nextP;
    }
}

int connection_send(mbedtls_connection_t *connP, uint8_t * buffer, size_t length){
    if (connP->dtlsSession == 0) {
        // no security
        if ( 0 != send_data(connP, buffer, length)) {
            return -1 ;
        }
    } else {
        if (DTLS_NAT_TIMEOUT > 0 && (lwm2m_gettime() - connP->lastSend) > DTLS_NAT_TIMEOUT)
        {
            LOG("connection_rehandshake\n");
            // we need to rehandhake because our source IP/port probably changed for the server
            if ( connection_rehandshake(connP, false) != 0 )
            {
                LOG("can't send due to rehandshake error\n");
                return -1;
            }
        }
        int ret;
        LOG( "  > Write to server:" );

        do ret = mbedtls_ssl_write( &connP->ssl, (unsigned char *) buffer, length );
        while( ret == MBEDTLS_ERR_SSL_WANT_READ ||
               ret == MBEDTLS_ERR_SSL_WANT_WRITE );

        if( ret < 0 )
        {
            LOG_ARG( " failed  ! mbedtls_ssl_write returned %d\n\n", length );
            return ret;
        }

        length = ret;
        LOG_ARG( " %d bytes written\n\n", length );

        LOG_ARG( " lwm2m_gettime() = %d, connP->lastSend = %d", lwm2m_gettime(), connP->lastSend );
        connP->lastSend = lwm2m_gettime();
    }

    return 0;
}

int connection_handle_packet(mbedtls_connection_t *connP){
    int ret, len;
    unsigned char buf[1024];
    LOG( "  < Read from server:" );

    len = sizeof( buf ) - 1;
    memset( buf, 0, sizeof( buf ) );

    do ret = mbedtls_ssl_read( &connP->ssl, buf, len );
    while( ret == MBEDTLS_ERR_SSL_WANT_READ ||
           ret == MBEDTLS_ERR_SSL_WANT_WRITE );

    if( ret <= 0 )
    {
        switch( ret )
        {
            case MBEDTLS_ERR_SSL_TIMEOUT:
                LOG( " timeout\n\n" );
                return ret;

            case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
                LOG( " connection was closed gracefully\n" );
                LOG( "  . Closing the connection..." );

                /* No error checking, the connection might be closed already */
                do ret = mbedtls_ssl_close_notify( &connP->ssl );
                while( ret == MBEDTLS_ERR_SSL_WANT_WRITE );
                ret = 0;

                LOG( " done\n" );
                return ret;

            default:
                LOG_ARG( " mbedtls_ssl_read returned -0x%x\n\n", -ret );
                return ret;
        }
    }

    len = ret;
    LOG_ARG( " %d bytes read\n\n", len );
    lwm2m_handle_packet(connP->lwm2mH, buf, len, (void*)connP);
    return len;
}

int connection_rehandshake(mbedtls_connection_t *connP, bool sendCloseNotify) {
    int ret = 0;
    // if not a dtls connection we do nothing
    if (connP->dtlsSession == 0) {
        return 0;
    }

    LOG( "  . Restarting connection from same port..." );

    if( ( ret = mbedtls_ssl_session_reset( &connP->ssl ) ) != 0 )
    {
        LOG_ARG( " failed  ! mbedtls_ssl_session_reset returned -0x%x\n\n", -ret );
        return ret;
    }

    while( ( ret = mbedtls_ssl_handshake( &connP->ssl ) ) != 0 )
    {
        if( ret != MBEDTLS_ERR_SSL_WANT_READ &&
            ret != MBEDTLS_ERR_SSL_WANT_WRITE )
        {
            LOG_ARG( " failed  ! mbedtls_ssl_handshake returned -0x%x\n\n", -ret );
            return ret;
        }
    }

    LOG( " ok\n" );
    return ret;
}

uint8_t lwm2m_buffer_send(void * sessionH,
                          uint8_t * buffer,
                          size_t length,
                          void * userdata)
{
    mbedtls_connection_t * connP = (mbedtls_connection_t*) sessionH;

    if (connP == NULL)
    {
        LOG_ARG("#> failed sending %lu bytes, missing connection\r\n", length);
        return COAP_500_INTERNAL_SERVER_ERROR ;
    }

    if (-1 == connection_send(connP, buffer, length))
    {
        LOG_ARG("#> failed sending %lu bytes\r\n", length);
        return COAP_500_INTERNAL_SERVER_ERROR ;
    }

    return COAP_NO_ERROR;
}

bool lwm2m_session_is_equal(void * session1,
                            void * session2,
                            void * userData)
{
    return (session1 == session2);
}

#endif
