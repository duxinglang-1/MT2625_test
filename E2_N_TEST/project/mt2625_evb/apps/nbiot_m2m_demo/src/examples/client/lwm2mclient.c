/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
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
 *    Benjamin Cabé - Please refer to git log
 *    Fabien Fleutot - Please refer to git log
 *    Simon Bernard - Please refer to git log
 *    Julien Vermillard - Please refer to git log
 *    Axel Lorente - Please refer to git log
 *    Toby Jaffey - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *    Christian Renz - Please refer to git log
 *    Ricky Liu - Please refer to git log
 *
 *******************************************************************************/

/*
 Copyright (c) 2013, 2014 Intel Corporation

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

 David Navarro <david.navarro@intel.com>
 Bosch Software Innovations GmbH - Please refer to git log

*/

#include "lwm2mclient.h"
#include "liblwm2m.h"
#include "commandline.h"
#if defined(WITH_MBEDTLS)
#include "mbedtlsconnection.h"
#elif defined(WITH_TINYDTLS)
#include "dtlsconnection.h"
#else
#include "connection.h"
#endif

#include "netdb.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/sockets.h"
//#include "ethernetif.h"
#include "lwip/sockets.h"
#include "netif/etharp.h"
#include "lwip/stats.h"
#include "syslog.h"
#include "util.h"
#include "app_common_header.h"
#include "lwip/sockets.h"
#include <ctype.h>



#define MAX_PACKET_SIZE 			(1024)
#define DEFAULT_SERVER_IPV6 		"[::1]"
#define DEFAULT_SERVER_IPV4 	    "127.0.0.1"

#define LWM2M_CLIENT_NAME_PREFIX	"sensorClient"

#define fprintf(...)

log_create_module(lwm2m_c, PRINT_LEVEL_INFO);


#define LWM2MAPPLOG(fmt, args...) LOG_I(lwm2m_c, "[lwm2m_c]"fmt, ##args);

#define LWM2MAPPPRINT printf

int g_reboot = 0;
static int g_quit = 0;
extern nb_app_context g_nb_app_context;

#define OBJ_COUNT 9 /* remove test & barometer object */
lwm2m_object_t * objArray[OBJ_COUNT];

// only backup security and server objects
#define BACKUP_OBJECT_COUNT 2
lwm2m_object_t * backupObjectArray[BACKUP_OBJECT_COUNT];

typedef struct
{
    lwm2m_object_t * securityObjP;
    lwm2m_object_t * serverObject;
    int sock;
#if defined(WITH_MBEDTLS)
    mbedtls_connection_t * connList;
    lwm2m_context_t * lwm2mH;
#elif defined(WITH_TINYDTLS)
    dtls_connection_t * connList;
    lwm2m_context_t * lwm2mH;
#else
    connection_t * connList;
#endif
    int addressFamily;
} client_data_t;

void prv_c_quit(char * buffer,
                     void * user_data)
{
    g_quit = 1;
}

static void handle_sigint(int signum)
{
    g_quit = 2;
}


void handle_value_changed(lwm2m_context_t * lwm2mH,
                          lwm2m_uri_t * uri,
                          const char * value,
                          size_t valueLength)
{
    lwm2m_data_notify(lwm2mH, uri, value, valueLength);
}

#if defined(WITH_MBEDTLS)
void * lwm2m_connect_server(uint16_t secObjInstID,
                            void * userData)
{
  client_data_t * dataP;
  lwm2m_list_t * instance;
  mbedtls_connection_t * newConnP = NULL;
  dataP = (client_data_t *)userData;
  lwm2m_object_t  * securityObj = dataP->securityObjP;

  instance = LWM2M_LIST_FIND(dataP->securityObjP->instanceList, secObjInstID);
  if (instance == NULL) return NULL;

  newConnP = connection_create(dataP->connList, dataP->sock, securityObj, instance->id, dataP->lwm2mH, dataP->addressFamily);
  if (newConnP == NULL)
  {
      LWM2MAPPLOG("Connection creation failed.\n");
      return NULL;
  }

  dataP->connList = newConnP;
  return (void *)newConnP;
}
#elif defined(WITH_TINYDTLS)
void * lwm2m_connect_server(uint16_t secObjInstID,
                            void * userData)
{
  client_data_t * dataP;
  lwm2m_list_t * instance;
  dtls_connection_t * newConnP = NULL;
  dataP = (client_data_t *)userData;
  lwm2m_object_t  * securityObj = dataP->securityObjP;

  instance = LWM2M_LIST_FIND(dataP->securityObjP->instanceList, secObjInstID);
  if (instance == NULL) return NULL;


  newConnP = connection_create(dataP->connList, dataP->sock, securityObj, instance->id, dataP->lwm2mH, dataP->addressFamily);
  if (newConnP == NULL)
  {
      LWM2MAPPLOG("Connection creation failed.\n");
      return NULL;
  }

  dataP->connList = newConnP;
  return (void *)newConnP;
}
#else
void * lwm2m_connect_server(uint16_t secObjInstID,
                            void * userData)
{
    client_data_t * dataP;
    char * uri;
    char * host;
    char * port;
    connection_t * newConnP = NULL;

    dataP = (client_data_t *)userData;

    uri = get_server_uri(dataP->securityObjP, secObjInstID);
    LWM2MAPPLOG("App uri: %s, id: %d", uri, secObjInstID);
    if (uri == NULL) return NULL;

    // parse uri in the form "coaps://[host]:[port]"
    if (0==strncmp(uri, "coaps://", strlen("coaps://"))) {
        host = uri+strlen("coaps://");
    }
    else if (0==strncmp(uri, "coap://",  strlen("coap://"))) {
        host = uri+strlen("coap://");
    }
    else {
        goto exit;
    }
    port = strrchr(host, ':');
    if (port == NULL) goto exit;
    // remove brackets
    if (host[0] == '[')
    {
        host++;
        if (*(port - 1) == ']')
        {
            *(port - 1) = 0;
        }
        else goto exit;
    }
    // split strings
    *port = 0;
    port++;

    LWM2MAPPLOG("App Opening connection to server at %s:%s\r\n", host, port);
    newConnP = connection_create(dataP->connList, dataP->sock, host, port, dataP->addressFamily);
    if (newConnP == NULL) {
        LWM2MAPPLOG("Connection creation failed.\r\n");
    }
    else {
        dataP->connList = newConnP;
    }

exit:
    lwm2m_free(uri);
    return (void *)newConnP;
}
#endif

void lwm2m_close_connection(void * sessionH,
                            void * userData)
{
    client_data_t * app_data;
#if defined(WITH_MBEDTLS)
    mbedtls_connection_t * targetP;
#elif defined(WITH_TINYDTLS)
    dtls_connection_t * targetP;
#else
    connection_t * targetP;
#endif

    app_data = (client_data_t *)userData;
#if defined(WITH_MBEDTLS)
    targetP = (mbedtls_connection_t *)sessionH;
#elif defined(WITH_TINYDTLS)
    targetP = (dtls_connection_t *)sessionH;
#else
    targetP = (connection_t *)sessionH;
#endif

    if (targetP == app_data->connList)
    {
        app_data->connList = targetP->next;
        lwm2m_free(targetP);
    }
    else
    {
#if defined(WITH_MBEDTLS)
        mbedtls_connection_t * parentP;
#elif defined(WITH_TINYDTLS)
        dtls_connection_t * parentP;
#else
        connection_t * parentP;
#endif

        parentP = app_data->connList;
        while (parentP != NULL && parentP->next != targetP)
        {
            parentP = parentP->next;
        }
        if (parentP != NULL)
        {
            parentP->next = targetP->next;
            lwm2m_free(targetP);
        }
    }
}

void prv_output_servers(char * buffer,
                               void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *) user_data;
    lwm2m_server_t * targetP;

    targetP = lwm2mH->bootstrapServerList;

    if (lwm2mH->bootstrapServerList == NULL)
    {
        fprintf(stdout, "No Bootstrap Server.\r\n");
    }
    else
    {
        fprintf(stdout, "Bootstrap Servers:\r\n");
        for (targetP = lwm2mH->bootstrapServerList ; targetP != NULL ; targetP = targetP->next)
        {
            fprintf(stdout, " - Security Object ID %d", targetP->secObjInstID);
            fprintf(stdout, "\tHold Off Time: %lu s", (unsigned long)targetP->lifetime);
            fprintf(stdout, "\tstatus: ");
            switch(targetP->status)
            {
            case STATE_DEREGISTERED:
                fprintf(stdout, "DEREGISTERED\r\n");
                break;
            case STATE_BS_HOLD_OFF:
                fprintf(stdout, "CLIENT HOLD OFF\r\n");
                break;
            case STATE_BS_INITIATED:
                fprintf(stdout, "BOOTSTRAP INITIATED\r\n");
                break;
            case STATE_BS_PENDING:
                fprintf(stdout, "BOOTSTRAP PENDING\r\n");
                break;
            case STATE_BS_FINISHED:
                fprintf(stdout, "BOOTSTRAP FINISHED\r\n");
                break;
            case STATE_BS_FAILED:
                fprintf(stdout, "BOOTSTRAP FAILED\r\n");
                break;
            default:
                fprintf(stdout, "INVALID (%d)\r\n", (int)targetP->status);
            }
        }
    }

    if (lwm2mH->serverList == NULL)
    {
        fprintf(stdout, "No LWM2M Server.\r\n");
    }
    else
    {
        fprintf(stdout, "LWM2M Servers:\r\n");
        for (targetP = lwm2mH->serverList ; targetP != NULL ; targetP = targetP->next)
        {
            fprintf(stdout, " - Server ID %d", targetP->shortID);
            fprintf(stdout, "\tstatus: ");
            switch(targetP->status)
            {
            case STATE_DEREGISTERED:
                fprintf(stdout, "DEREGISTERED\r\n");
                break;
            case STATE_REG_PENDING:
                fprintf(stdout, "REGISTRATION PENDING\r\n");
                break;
            case STATE_REGISTERED:
                fprintf(stdout, "REGISTERED\tlocation: \"%s\"\tLifetime: %lus\r\n", targetP->location, (unsigned long)targetP->lifetime);
                break;
            case STATE_REG_UPDATE_PENDING:
                fprintf(stdout, "REGISTRATION UPDATE PENDING\r\n");
                break;
            case STATE_DEREG_PENDING:
                fprintf(stdout, "DEREGISTRATION PENDING\r\n");
                break;
            case STATE_REG_FAILED:
                fprintf(stdout, "REGISTRATION FAILED\r\n");
                break;
            default:
                fprintf(stdout, "INVALID (%d)\r\n", (int)targetP->status);
            }
        }
    }
}

void prv_change(char * buffer,
                       void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *) user_data;
    lwm2m_uri_t uri;
    char * end = NULL;
    int result;

    end = get_end_of_arg(buffer);
    if (end[0] == 0) goto syntax_error;

    result = lwm2m_stringToUri(buffer, end - buffer, &uri);
    if (result == 0) goto syntax_error;

    buffer = get_next_arg(end, &end);

    if (buffer[0] == 0)
    {
        fprintf(stderr, "report change!\n");
        lwm2m_resource_value_changed(lwm2mH, &uri);
    }
    else
    {
        handle_value_changed(lwm2mH, &uri, buffer, end - buffer);
    }
    return;

syntax_error:
    fprintf(stdout, "Syntax error !\n");
}

void prv_object_list(char * buffer,
                            void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *)user_data;
    lwm2m_object_t * objectP;

    for (objectP = lwm2mH->objectList; objectP != NULL; objectP = objectP->next)
    {
        if (objectP->instanceList == NULL)
        {
            fprintf(stdout, "/%d ", objectP->objID);
        }
        else
        {
            lwm2m_list_t * instanceP;

            for (instanceP = objectP->instanceList; instanceP != NULL ; instanceP = instanceP->next)
            {
                fprintf(stdout, "/%d/%d  ", objectP->objID, instanceP->id);
            }
        }
        fprintf(stdout, "\r\n");
    }
}

static void prv_instance_dump(lwm2m_object_t * objectP,
                              uint16_t id)
{
    int numData;
    lwm2m_data_t * dataArray;
    uint16_t res;

    numData = 0;
    res = objectP->readFunc(id, &numData, &dataArray, objectP);
    if (res != COAP_205_CONTENT)
    {
        printf("Error ");
        print_status(stdout, res);
        printf("\r\n");
        return;
    }

    dump_tlv(stdout, numData, dataArray, 0);
}


void prv_object_dump(char * buffer,
                            void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *) user_data;
    lwm2m_uri_t uri;
    char * end = NULL;
    int result;
    lwm2m_object_t * objectP;

    end = get_end_of_arg(buffer);
    if (end[0] == 0) goto syntax_error;

    result = lwm2m_stringToUri(buffer, end - buffer, &uri);
    if (result == 0) goto syntax_error;
    if (uri.flag & LWM2M_URI_FLAG_RESOURCE_ID) goto syntax_error;

    objectP = (lwm2m_object_t *)LWM2M_LIST_FIND(lwm2mH->objectList, uri.objectId);
    if (objectP == NULL)
    {
        fprintf(stdout, "Object not found.\n");
        return;
    }

    if (uri.flag & LWM2M_URI_FLAG_INSTANCE_ID)
    {
        prv_instance_dump(objectP, uri.instanceId);
    }
    else
    {
        lwm2m_list_t * instanceP;

        for (instanceP = objectP->instanceList; instanceP != NULL ; instanceP = instanceP->next)
        {
            fprintf(stdout, "Instance %d:\r\n", instanceP->id);
            prv_instance_dump(objectP, instanceP->id);
            fprintf(stdout, "\r\n");
        }
    }

    return;

syntax_error:
    fprintf(stdout, "Syntax error !\n");
}

void prv_update(char * buffer,
                       void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *)user_data;
    if (buffer[0] == 0) goto syntax_error;

    uint16_t serverId = (uint16_t) atoi(buffer);
    int res = lwm2m_update_registration(lwm2mH, serverId, false);
    if (res != 0)
    {
        fprintf(stdout, "Registration update error: ");
        print_status(stdout, res);
        fprintf(stdout, "\r\n");
    }
    return;

syntax_error:
    fprintf(stdout, "Syntax error !\n");
}

static void update_battery_level(lwm2m_context_t * context)
{
    static time_t next_change_time = 0;
    time_t tv_sec;

    tv_sec = lwm2m_gettime();
    if (tv_sec < 0) return;

    if (next_change_time < tv_sec)
    {
        char value[15];
        int valueLength;
        lwm2m_uri_t uri;
        int level = rand() % 100;

        if (0 > level) level = -level;
        if (lwm2m_stringToUri("/3/0/9", 6, &uri))
        {
            valueLength = sprintf(value, "%d", level);
            fprintf(stderr, "New Battery Level: %d\n", level);
            handle_value_changed(context, &uri, value, valueLength);
        }
        level = rand() % 20;
        if (0 > level) level = -level;
        next_change_time = tv_sec + level + 10;
    }
}

void prv_add(char * buffer,
                    void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *)user_data;
    lwm2m_object_t * objectP;
    int res;

    objectP = get_test_object();
    if (objectP == NULL)
    {
        fprintf(stdout, "Creating object 1024 failed.\r\n");
        return;
    }
    res = lwm2m_add_object(lwm2mH, objectP);
    if (res != 0)
    {
        fprintf(stdout, "Adding object 1024 failed: ");
        print_status(stdout, res);
        fprintf(stdout, "\r\n");
    }
    else
    {
        fprintf(stdout, "Object 1024 added.\r\n");
    }
    return;
}

void prv_remove(char * buffer,
                       void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *)user_data;
    int res;

    res = lwm2m_remove_object(lwm2mH, 1024);
    if (res != 0)
    {
        fprintf(stdout, "Removing object 1024 failed: ");
        print_status(stdout, res);
        fprintf(stdout, "\r\n");
    }
    else
    {
        fprintf(stdout, "Object 1024 removed.\r\n");
    }
    return;
}

#ifdef LWM2M_BOOTSTRAP

void prv_initiate_bootstrap(char * buffer,
                                   void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *)user_data;
    lwm2m_server_t * targetP;

    // HACK !!!
    lwm2mH->state = STATE_BOOTSTRAP_REQUIRED;
    targetP = lwm2mH->bootstrapServerList;
    while (targetP != NULL)
    {
        targetP->lifetime = 0;
        targetP = targetP->next;
    }
}

void prv_display_objects(char * buffer,
                                void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *)user_data;
    lwm2m_object_t * object;

    for (object = lwm2mH->objectList; object != NULL; object = object->next){
        if (NULL != object) {
            switch (object->objID)
            {
            case LWM2M_SECURITY_OBJECT_ID:
                display_security_object(object);
                break;
            case LWM2M_SERVER_OBJECT_ID:
                display_server_object(object);
                break;
            case LWM2M_ACL_OBJECT_ID:
                break;
            case LWM2M_DEVICE_OBJECT_ID:
                display_device_object(object);
                break;
            case LWM2M_CONN_MONITOR_OBJECT_ID:
                break;
            case LWM2M_FIRMWARE_UPDATE_OBJECT_ID:
                display_firmware_object(object);
                break;
            case LWM2M_LOCATION_OBJECT_ID:
                display_location_object(object);
                break;
            case LWM2M_CONN_STATS_OBJECT_ID:
                break;
            case TEST_OBJECT_ID:
                display_test_object(object);
                break;
            }
        }
    }
}

void prv_display_backup(char * buffer,
        void * user_data)
{
   int i;
   for (i = 0 ; i < BACKUP_OBJECT_COUNT ; i++) {
       lwm2m_object_t * object = backupObjectArray[i];
       if (NULL != object) {
           switch (object->objID)
           {
           case LWM2M_SECURITY_OBJECT_ID:
               display_security_object(object);
               break;
           case LWM2M_SERVER_OBJECT_ID:
               display_server_object(object);
               break;
           default:
               break;
           }
       }
   }
}

static void prv_backup_objects(lwm2m_context_t * context)
{
    uint16_t i;

    for (i = 0; i < BACKUP_OBJECT_COUNT; i++) {
        if (NULL != backupObjectArray[i]) {
            switch (backupObjectArray[i]->objID)
            {
            case LWM2M_SECURITY_OBJECT_ID:
                clean_security_object(backupObjectArray[i]);
                lwm2m_free(backupObjectArray[i]);
                break;
            case LWM2M_SERVER_OBJECT_ID:
                clean_server_object(backupObjectArray[i]);
                lwm2m_free(backupObjectArray[i]);
                break;
            default:
                break;
            }
        }
        backupObjectArray[i] = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));
        memset(backupObjectArray[i], 0, sizeof(lwm2m_object_t));
    }

    /*
     * Backup content of objects 0 (security) and 1 (server)
     */
    copy_security_object(backupObjectArray[0], (lwm2m_object_t *)LWM2M_LIST_FIND(context->objectList, LWM2M_SECURITY_OBJECT_ID));
    copy_server_object(backupObjectArray[1], (lwm2m_object_t *)LWM2M_LIST_FIND(context->objectList, LWM2M_SERVER_OBJECT_ID));
}

static void prv_restore_objects(lwm2m_context_t * context)
{
    lwm2m_object_t * targetP;

    /*
     * Restore content  of objects 0 (security) and 1 (server)
     */
    targetP = (lwm2m_object_t *)LWM2M_LIST_FIND(context->objectList, LWM2M_SECURITY_OBJECT_ID);
    // first delete internal content
    clean_security_object(targetP);
    // then restore previous object
    copy_security_object(targetP, backupObjectArray[0]);

    targetP = (lwm2m_object_t *)LWM2M_LIST_FIND(context->objectList, LWM2M_SERVER_OBJECT_ID);
    // first delete internal content
    clean_server_object(targetP);
    // then restore previous object
    copy_server_object(targetP, backupObjectArray[1]);

    // restart the old servers
    fprintf(stdout, "[BOOTSTRAP] ObjectList restored\r\n");
}

static void update_bootstrap_info(lwm2m_client_state_t * previousBootstrapState,
        lwm2m_context_t * context)
{
    if (*previousBootstrapState != context->state)
    {
        *previousBootstrapState = context->state;
        switch(context->state)
        {
            case STATE_BOOTSTRAPPING:
#ifdef WITH_LOGS
                fprintf(stdout, "[BOOTSTRAP] backup security and server objects\r\n");
#endif
                prv_backup_objects(context);
                break;
            default:
                break;
        }
    }
}

static void close_backup_object()
{
    int i;
    for (i = 0; i < BACKUP_OBJECT_COUNT; i++) {
        if (NULL != backupObjectArray[i]) {
            switch (backupObjectArray[i]->objID)
            {
            case LWM2M_SECURITY_OBJECT_ID:
                clean_security_object(backupObjectArray[i]);
                lwm2m_free(backupObjectArray[i]);
                break;
            case LWM2M_SERVER_OBJECT_ID:
                clean_server_object(backupObjectArray[i]);
                lwm2m_free(backupObjectArray[i]);
                break;
            default:
                break;
            }
        }
    }
}
#endif

static void print_usage(void)
{
    fprintf(stdout, "Usage: lwm2mclient [OPTION]\r\n");
    fprintf(stdout, "Launch a LWM2M client.\r\n");
    fprintf(stdout, "Options:\r\n");
    fprintf(stdout, "  -n NAME\tSet the endpoint name of the Client. Default: testlwm2mclient\r\n");
    fprintf(stdout, "  -l PORT\tSet the local UDP port of the Client. Default: 56830\r\n");
    fprintf(stdout, "  -h HOST\tSet the hostname of the LWM2M Server to connect to. Default: localhost\r\n");
    fprintf(stdout, "  -p PORT\tSet the port of the LWM2M Server to connect to. Default: "LWM2M_STANDARD_PORT_STR"\r\n");
    fprintf(stdout, "  -4\t\tUse IPv4 connection. Default: IPv6 connection\r\n");
    fprintf(stdout, "  -t TIME\tSet the lifetime of the Client. Default: 300\r\n");
    fprintf(stdout, "  -b\t\tBootstrap requested.\r\n");
    fprintf(stdout, "  -c\t\tChange battery level over time.\r\n");
#ifdef WITH_TINYDTLS
    fprintf(stdout, "  -i STRING\tSet the device management or bootstrap server PSK identity. If not set use none secure mode\r\n");
    fprintf(stdout, "  -s HEXSTRING\tSet the device management or bootstrap server Pre-Shared-Key. If not set use none secure mode\r\n");
#endif
    fprintf(stdout, "\r\n");
}

extern void lwm2m_client_notify_callback(lwm2m_notify_type_t type, lwm2m_notify_code_t code, uint32_t option_info);

int lwm2m_client_main_loop(void * args)
{
    client_data_t data;
    int result;
    lwm2m_context_t * lwm2mH = NULL;
    int i;
    struct sockaddr_in cli_addr;

#ifdef MTK_LWM2M_CT_SUPPORT
    const char * localPort = g_nb_app_context.port;
    const char * server = g_nb_app_context.server;
    const char * serverPort = g_nb_app_context.port;
    char name[32] = {0};
    int lifetime = g_nb_app_context.lifetime;
#else
    const char * localPort = "5683";
    const char * server = "182.150.27.21";
    //const char * server = "182.150.27.42";
    const char * serverPort = LWM2M_STANDARD_PORT_STR;
    char name[LWM2M_CLIENT_NAME_LENGTH] = {0};
    int lifetime = 7200;
#endif
    int batterylevelchanging = 1;
    time_t reboot_time = 0;
    int opt;
    bool bootstrapRequested = false;
    bool serverPortChanged = false;
	int tickCount = xTaskGetTickCount();

#ifdef LWM2M_BOOTSTRAP
    lwm2m_client_state_t previousState = STATE_INITIAL;
#endif

#ifdef MTK_LWM2M_CT_SUPPORT
    char * pskId = g_nb_app_context.pskid;
    char * psk = g_nb_app_context.psk;
    uint16_t pskLen = 0;
    char * pskBuffer = NULL;
#else
    char * pskId = NULL;
    char * psk = NULL;
    uint16_t pskLen = 0;
    char * pskBuffer = NULL;
#endif
    char serverUri[50];
    int serverId = 123;
	char *location = NULL;

    extern void nb_lwm2m_start_rtc_timer(void);

    extern void nb_lwm2m_stop_rtc_timer(void);

    //LWM2MAPPLOG("brfore enter mutex");
    // before create lwm2m client task need to confirm that bearer has actived and can go internet
    //lwm2m_app_mutex_take();
    LWM2MAPPLOG("after enter mutex");
    
    memset(&data, 0, sizeof(client_data_t));
    data.addressFamily = AF_INET;//AF_INET6;

    opt = 1;
    if (!server)
    {
        server = (AF_INET == data.addressFamily ? DEFAULT_SERVER_IPV4 : DEFAULT_SERVER_IPV6);
    }

    /*
     *This call an internal function that create an IPV6 socket on the port 5683.
     */
    fprintf(stderr, "Trying to bind LWM2M Client to port %s\r\n", localPort);
    data.sock = create_socket(localPort, data.addressFamily);
    LWM2MAPPLOG("sock id %d", data.sock);
    if (data.sock < 0)
    {
        LWM2MAPPLOG("Failed to open socket: %d %s\r\n", data.sock);
        //return -1;
    }

    /* Client bind fixed prot for deep sleep */
    {
        int32_t res = -123;

        bzero(&cli_addr, sizeof(struct sockaddr_in));
        cli_addr.sin_family = AF_INET;
        cli_addr.sin_port = htons(g_nb_app_context.cli_port);
        cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        cli_addr.sin_len = sizeof(struct sockaddr_in);

        res = bind(data.sock, (struct sockaddr *)&cli_addr, sizeof(struct sockaddr));

        if (res < 0) {
            LWM2MAPPLOG("Failed to bind fixed port: %d", res);
        }
        LWM2MAPPLOG("Bind fixed port--res: %d", res);
    }

    /*
     * Now the main function fill an array with each object, this list will be later passed to liblwm2m.
     * Those functions are located in their respective object file.
     */
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    if (psk != NULL)
    {
        pskLen = strlen(psk) / 2;
        pskBuffer = pvPortMalloc(pskLen);

        if (NULL == pskBuffer)
        {
            fprintf(stderr, "Failed to create PSK binary buffer\r\n");
            close(data.sock);
            return -1;
        }
        // Hex string to binary
        char *h = psk;
        char *b = pskBuffer;
        char xlate[] = "0123456789ABCDEF";

        for ( ; *h; h += 2, ++b)
        {
            if (h - psk >= pskLen * 2)
            {
                break;
            }
            char *l = strchr(xlate, (char)toupper((unsigned char)*h));
            char *r = strchr(xlate, (char)toupper((unsigned char)*(h+1)));

            if (!r || !l)
            {
                fprintf(stderr, "Failed to parse Pre-Shared-Key HEXSTRING\r\n");
                vPortFree(pskBuffer);
                close(data.sock);
                return -1;
            }

            *b = ((l - xlate) << 4) + (r - xlate);
        }
    }
#endif

    if(true == g_nb_app_context.is_deep_sleep && g_collect_data.state == STATE_READY)
    {
        serverId = g_collect_data.server_id;
        //memcpy(serverUri, "%s", g_collect_data.server_uri);
        //strcpy(serverUri, g_collect_data.server_uri);
        sprintf (serverUri, "coap://%s:%s", server, serverPort);
        lifetime = g_collect_data.life_time;
        //sprintf(name, "%s", g_collect_data.deice_name);
#ifdef MTK_LWM2M_CT_SUPPORT
        sprintf(name, "%s", g_nb_app_context.name);
#else
        sprintf(name, "%s_%d", LWM2M_CLIENT_NAME_PREFIX, g_collect_data.tickCount);
#endif
    } else {
        serverId = LWM2M_SENSOR_SRV_ID;
#ifdef MTK_LWM2M_CT_SUPPORT
        sprintf(name, "%s", g_nb_app_context.name);
#else
		sprintf(name, "%s_%d", LWM2M_CLIENT_NAME_PREFIX, tickCount);
        g_collect_data.tickCount = tickCount;
#endif
        //sprintf(g_collect_data.deice_name, "%s", name);
		LWM2MAPPLOG("fffffffffffffffff");
        sprintf (serverUri, "coap://%s:%s", server, serverPort);
        save_security_object(serverUri, serverId);
        save_server_object("U", lifetime);
    }

	LWM2MAPPLOG("device_name: %s\r\n", name);	
    
#ifdef LWM2M_BOOTSTRAP
    objArray[0] = get_security_object(serverId, serverUri, pskId, pskBuffer, pskLen, bootstrapRequested);
#else
    objArray[0] = get_security_object(serverId, serverUri, pskId, pskBuffer, pskLen, false);
#endif
    if (NULL == objArray[0])
    {
        LWM2MAPPLOG("Failed to create security object\r\n");
        //return -1;
    }
    data.securityObjP = objArray[0];

    /* should free pskBuffer here */
    if (pskBuffer != NULL) vPortFree(pskBuffer);

    objArray[1] = get_server_object(serverId, "U", lifetime, false);
    if (NULL == objArray[1])
    {
        LWM2MAPPLOG("Failed to create server object\r\n");
        //return -1;
    }

    objArray[2] = get_object_device();
    if (NULL == objArray[2])
    {
        LWM2MAPPLOG("Failed to create Device object\r\n");
        //return -1;
    }

    objArray[3] = get_object_firmware();
    if (NULL == objArray[3])
    {
        LWM2MAPPLOG("Failed to create Firmware object\r\n");
        //return -1;
    }

    objArray[4] = get_object_location();
    if (NULL == objArray[4])
    {
        LWM2MAPPLOG("Failed to create location object\r\n");
        //return -1;
    }

#if 0
    objArray[5] = get_test_object();
    if (NULL == objArray[5])
    {
        LWM2MAPPLOG("Failed to create test object\r\n");
        //return -1;
    }
#endif

#ifndef MTK_LWM2M_CT_SUPPORT
    // for accelerometer
    objArray[5] = get_object_accelerometer();
    if (NULL == objArray[5])
    {
        LWM2MAPPLOG("Failed to create accelerometer object\r\n");
        //return -1;
    }
#else
    // for watermeter
    objArray[5] = get_object_watermeter();
    if (NULL == objArray[5])
    {
        LWM2MAPPLOG("Failed to create watermeter object\r\n");
        //return -1;
    }
#endif

    objArray[6] = get_object_conn_m();
    if (NULL == objArray[6])
    {
        LWM2MAPPLOG("Failed to create connectivity monitoring object\r\n");
        //return -1;
    }

    objArray[7] = get_object_conn_s();
    if (NULL == objArray[7])
    {
        LWM2MAPPLOG("Failed to create connectivity statistics object\r\n");
        //return -1;
    }

    int instId = 0;
    objArray[8] = acc_ctrl_create_object();
    if (NULL == objArray[8])
    {
        LWM2MAPPLOG("Failed to create Access Control object\r\n");
        //return -1;
    }
    else if (acc_ctrl_obj_add_inst(objArray[8], instId, 3, 0, serverId)==false)
    {
        LWM2MAPPLOG("Failed to create Access Control object instance\r\n");
        //return -1;
    }
    else if (acc_ctrl_oi_add_ac_val(objArray[8], instId, 0, 0b000000000001111)==false)
    {
        LWM2MAPPLOG("Failed to create Access Control ACL default resource\r\n");
        //return -1;
    }
    else if (acc_ctrl_oi_add_ac_val(objArray[8], instId, 999, 0b000000000000001)==false)
    {
        LWM2MAPPLOG("Failed to create Access Control ACL resource for serverId: 999\r\n");
        //return -1;
    }

#if 0
     // for barometer
    objArray[9] = get_object_barometer();
    if (NULL == objArray[9])
    {
        LWM2MAPPLOG("Failed to create barometer object\r\n");
        //return -1;
    }
#endif
    
    /*
     * The liblwm2m library is now initialized with the functions that will be in
     * charge of communication
     */
    lwm2mH = lwm2m_init(&data);
    if (NULL == lwm2mH)
    {
        LWM2MAPPLOG("lwm2m_init() failed\r\n");
        close(data.sock);
        return -1;
    }

    if (true == g_nb_app_context.is_deep_sleep && g_collect_data.state == STATE_READY) {
        lwm2mH->state = g_collect_data.state;
        LWM2MAPPLOG("main_loop(retention)--state: %d", lwm2mH->state);
    }

    lwm2mH->mode = CLIENT_MODE;
    lwm2mH->connect_server_callback = lwm2m_connect_server;
    lwm2mH->close_connection_callback = lwm2m_close_connection;
    lwm2mH->notify_callback = lwm2m_client_notify_callback;
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
    data.lwm2mH = lwm2mH;
#endif

    /*
     * We configure the liblwm2m library with the name of the client - which shall be unique for each client -
     * the number of objects we will be passing through and the objects array
     */
    result = lwm2m_configure(lwm2mH, name, NULL, NULL, OBJ_COUNT, objArray);
    LWM2MAPPLOG("lwm2m_configure:result %d", result);
    if (result != 0)
    {
        LWM2MAPPLOG("lwm2m_configure() failed: 0x%X\r\n", result);
        //return -1;
    }


    /**
     * Initialize value changed callback.
     */
    init_value_change(lwm2mH);

    LWM2MAPPLOG("LWM2M Client \"%s\" started on port %s\r\n", name, localPort);
    
    //fprintf(stdout, "LWM2M Client \"%s\" started on port %s\r\n", name, localPort);
    //fprintf(stdout, "> "); fflush(stdout);

	g_nb_app_context.lwm2m_info = lwm2mH;

	/*
	if(true == g_nb_app_context.is_deep_sleep)
	{
		LWM2MAPPLOG("yyyyyyyyyyy\r\n");
		// location recover
	    location = g_collect_data.location;
		memset(g_nb_app_context.lwm2m_info->serverList->location, 0, MAX_LOCATION_LENGTH);
		memcpy(g_nb_app_context.lwm2m_info->serverList->location, location, MAX_LOCATION_LENGTH);
		// setting lwm2m status recover
		lwm2mH->state = g_collect_data.state;
	}else{
		location = g_nb_app_context.lwm2m_info->serverList->location;
		memset(g_collect_data.location, 0, MAX_LOCATION_LENGTH);
		LWM2MAPPLOG("sssssssssssssssssssssssss");
	    memcpy(g_collect_data.location, location, MAX_LOCATION_LENGTH);
	}
	*/

	
		
    /*
         * We now enter in a while loop that will handle the communications from the server
      */
    g_quit = 0;
    nb_lwm2m_start_rtc_timer();
    lwm2mH->ext_reg_update = 1;
    while (0 == g_quit)
    {
        struct timeval tv;
        fd_set readfds;        

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        /*
             * This function does two things:
             *  - first it does the work needed by liblwm2m (eg. (re)sending some packets).
             *  - Secondly it adjusts the timeout value (default 60s) depending on the state of the transaction
             *    (eg. retransmission) and the time between the next operation
             */
        result = lwm2m_step(lwm2mH, &(tv.tv_sec));
        LWM2MAPPLOG("main_loop--State: %d, name: %s, sec: %d", lwm2mH->state, name, tv.tv_sec);

        g_collect_data.state = lwm2mH->state;

        switch (lwm2mH->state)
        {
        case STATE_INITIAL:
            LWM2MAPPLOG("STATE_INITIAL\r\n");
            break;
        case STATE_BOOTSTRAP_REQUIRED:
            LWM2MAPPLOG("STATE_BOOTSTRAP_REQUIRED\r\n");
            break;
        case STATE_BOOTSTRAPPING:
            LWM2MAPPLOG("STATE_BOOTSTRAPPING\r\n");
            break;
        case STATE_REGISTER_REQUIRED:
            LWM2MAPPLOG("STATE_REGISTER_REQUIRED\r\n");
            break;
        case STATE_REGISTERING:
            LWM2MAPPLOG("STATE_REGISTERING\r\n");
            break;
        case STATE_READY:
            LWM2MAPPLOG("STATE_READY\r\n");
            break;
        default:
            LWM2MAPPLOG("Unknown...\r\n");
            break;
        }
	
        if (result != 0)
        {
            LWM2MAPPLOG("lwm2m_step() failed: 0x%X\r\n", result);
            lwm2mH->notify_callback(LWM2M_NOTIFY_TYPE_LWSTATUS, LWM2M_NOTIFY_CODE_SUCCESS, 0);
#ifdef LWM2M_BOOTSTRAP
            if(previousState == STATE_BOOTSTRAPPING)
            {
#ifdef WITH_LOGS
                LWM2MAPPLOG("[BOOTSTRAP] restore security and server objects\r\n");
#endif
                prv_restore_objects(lwm2mH);
                lwm2mH->state = STATE_INITIAL;
            }
#endif
        }
#ifdef LWM2M_BOOTSTRAP
        update_bootstrap_info(&previousState, lwm2mH);
#endif 

		if(true == g_nb_app_context.not_send_bearer_flag && STATE_READY == lwm2mH->state) {
			// execute_data_reporting();
			// change to reporting status
            app_send_msg(msg_app_state_change, 
                        (void *)(APP_DATA_REPORTING));
		}

        /*
                * This part will set up an interruption until an event happen on SDTIN or the socket until "tv" timed out (set
                * with the precedent function)
            */
#ifdef WITH_MBEDTLS
        mbedtls_connection_t *connP = data.connList;
        if (connP != NULL && connP->dtlsSession != 0) {
            int result = connection_handle_packet(connP);
            if (result < 0) {
                LWM2MAPPLOG("error handling message %d", result);
            }
        } else
#endif /* WITH_MBEDTLS */
        {
        FD_ZERO(&readfds);
        FD_SET(data.sock, &readfds);
        FD_SET(STDIN_FILENO, &readfds);

        result = select(FD_SETSIZE, &readfds, NULL, NULL, &tv);

        if (result < 0)
        {
            if (errno != EINTR)
            {
              LWM2MAPPLOG("Error in select(): %d \r\n", errno);
            }
        }
        else if (result > 0)
        {
            uint8_t buffer[MAX_PACKET_SIZE];
            int numBytes;

            /*
             * If an event happens on the socket
             */
            if (FD_ISSET(data.sock, &readfds))
            {
                struct sockaddr_storage addr;
                socklen_t addrLen;

                addrLen = sizeof(addr);

                /*
                 * We retrieve the data received
                 */
                numBytes = recvfrom(data.sock, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&addr, &addrLen);

                if (0 > numBytes)
                {
                    LWM2MAPPLOG("Error in recvfrom(): %d\r\n", errno);
                }
                else if (0 < numBytes)
                {
                    char s[INET6_ADDRSTRLEN];
                    in_port_t port;

#if defined(WITH_MBEDTLS)
                    mbedtls_connection_t *connP;
#elif defined(WITH_TINYDTLS)
                    dtls_connection_t *connP;
#else
                    connection_t * connP;
#endif
                    if (AF_INET == addr.ss_family)
                    {
                        struct sockaddr_in *saddr = (struct sockaddr_in *)&addr;
                        inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET6_ADDRSTRLEN);
                        port = saddr->sin_port;
                    }
                    else if (AF_INET6 == addr.ss_family)
                    {
                        struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)&addr;
                        inet_ntop(saddr->sin6_family, &saddr->sin6_addr, s, INET6_ADDRSTRLEN);
                        port = saddr->sin6_port;
                    }
                    LWM2MAPPLOG("%d bytes received from [%s]:%hu\r\n", numBytes, s, ntohs(port));

                    /*
                     * Display it in the STDERR
                     */
                    output_buffer(stderr, buffer, numBytes, 0);

                    connP = connection_find(data.connList, &addr, addrLen);
                    if (connP != NULL)
                    {
                        /*
                         * Let liblwm2m respond to the query depending on the context
                         */
#ifdef WITH_TINYDTLS
                        int result = connection_handle_packet(connP, buffer, numBytes);
                        if (0 != result)
                        {
                             LWM2MAPPLOG("error handling message %d\n",result);
                        }
#else
                        lwm2m_handle_packet(lwm2mH, buffer, numBytes, connP);
#endif
                        conn_s_updateRxStatistic(objArray[7], numBytes, false);
                    }
                    else
                    {
                        LWM2MAPPLOG("received bytes ignored!\r\n");
                    }
                }
            }

            /*
             * If the event happened on the SDTIN
             */
            else if (FD_ISSET(STDIN_FILENO, &readfds))
            {
                numBytes = read(STDIN_FILENO, buffer, MAX_PACKET_SIZE - 1);

                if (numBytes > 1)
                {
                    buffer[numBytes] = 0;
                    /*
                     * We call the corresponding callback of the typed command passing it the buffer for further arguments
                     */
                    //handle_command(commands, (char*)buffer);
                }
                if (g_quit == 0)
                {
                    LWM2MAPPLOG("\r\n> ");
                    //fflush(stdout);
                }
                else
                {
                    LWM2MAPPLOG("\r\n");
                }
            }
        }
        }
        vTaskDelay(1000 / portTICK_RATE_MS); // release CPU
    }

    nb_lwm2m_stop_rtc_timer();

    /*
     * Finally when the loop is left smoothly - asked by user in the command line interface - we unregister our client from it
     */
    if (g_quit == 1)
    {
#ifdef LWM2M_BOOTSTRAP
        close_backup_object();
#endif
        lwm2m_close(lwm2mH);
    }
    else if (g_quit == 3)
    {
#ifdef LWM2M_BOOTSTRAP
        close_backup_object();
#endif
        lwm2m_close_not_deregister(lwm2mH);
    }
    g_nb_app_context.lwm2m_info = NULL;
    close(data.sock);
    connection_free(data.connList);

    clean_security_object(objArray[0]);
    lwm2m_free(objArray[0]);
    clean_server_object(objArray[1]);
    lwm2m_free(objArray[1]);
    free_object_device(objArray[2]);
    free_object_firmware(objArray[3]);
    free_object_location(objArray[4]);
#ifndef MTK_LWM2M_CT_SUPPORT
    free_object_accelerometer(objArray[5]);
#else
    free_object_watermeter(objArray[5]);
#endif
    free_object_conn_m(objArray[6]);
    free_object_conn_s(objArray[7]);
    acl_ctrl_free_object(objArray[8]);

#ifdef MEMORY_TRACE
    if (g_quit == 1)
    {
        trace_print(0, 1);
    }
#endif

    LWM2M_RESET_FLAG(g_nb_lwm2m_flag, LWM2M_FLAG_CLIENT_RUNNING);
    if (g_nb_lwm2m_flag & LWM2M_FLAG_CLIENT_RESTART) {
        nb_app_notify_lwm2m_client_restart();
        LWM2M_RESET_FLAG(g_nb_lwm2m_flag, LWM2M_FLAG_CLIENT_RESTART);
    }

    LWM2MAPPLOG("lwm2mc_quit");
    g_quit = 0;
    g_collect_data.state = STATE_INITIAL;

    vTaskDelete(NULL);

    return 0;
}


void lwm2m_client_quit(char *title, int quit)
{
    LWM2MAPPLOG("[lwm2mc_quit]--title: %s, quit: %d", title, quit);
    g_quit = quit;
}

void lwm2m_client_deregister(void)
{
    LWM2MAPPLOG("[lwm2m_client_deregister]--handle: 0x%x", g_nb_app_context.lwm2m_info);
    if (g_nb_app_context.lwm2m_info == NULL) return;
    if (STATE_READY == g_nb_app_context.lwm2m_info->state) {
        lwm2m_deregister(g_nb_app_context.lwm2m_info);
    } else {
        lwm2m_client_quit("LWM2M_CLI", 3);
    }
}

bool lwm2m_client_is_connected(void)
{
    return g_collect_data.state != STATE_INITIAL;
}

