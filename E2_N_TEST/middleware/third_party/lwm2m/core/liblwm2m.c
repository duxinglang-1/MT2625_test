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
 *    Fabien Fleutot - Please refer to git log
 *    Simon Bernard - Please refer to git log
 *    Toby Jaffey - Please refer to git log
 *    Pascal Rieux - Please refer to git log
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

*/

#include "internals.h"
#include "hal_rtc_external.h"
#include "lwm2m_objects.h"
#include "nvdm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

lwm2m_context_t * lwm2m_init(void * userData)
{
    lwm2m_context_t * contextP;

    LOG("lwm2m_init");
    contextP = (lwm2m_context_t *)lwm2m_malloc(sizeof(lwm2m_context_t));
    if (NULL != contextP)
    {
        memset(contextP, 0, sizeof(lwm2m_context_t));
        contextP->userData = userData;
        srand((int)lwm2m_gettime());
        contextP->nextMID = rand();
    }

    return contextP;
}

#ifdef LWM2M_CLIENT_MODE
void lwm2m_update_lifetime(lwm2m_context_t * context, time_t lifetime)
{
    lwm2m_server_t * server = context->serverList;

    LOG("lwm2m_update_lifetime");
    while (NULL != server)
    {
        server->lifetime = lifetime;
        server->is_lifetime_updating = true;
        set_server_lifetime(context, lifetime, server->secObjInstID);
        lwm2m_update(context, server);
        server = server->next;
    }
}

void lwm2m_update_binding(lwm2m_context_t * context, lwm2m_binding_t binding)
{
    lwm2m_server_t * server = context->serverList;

    LOG("lwm2m_update_binding");
    while (NULL != server)
    {
        server->binding = binding;
        server->is_lifetime_updating = false;
        set_server_binding(context, binding, server->secObjInstID);
        lwm2m_update(context, server);
        server = server->next;
    }
}

void lwm2m_deregister(lwm2m_context_t * context)
{
    lwm2m_server_t * server = context->serverList;

    LOG("lwm2m_deregister");
    while (NULL != server)
    {
        lwm2m_registration_deregister(context, server);
        server = server->next;
    }
}

static void prv_deleteServer(lwm2m_server_t * serverP)
{
    // TODO parse transaction and observation to remove the ones related to this server
    if (NULL != serverP->location)
    {
        lwm2m_free(serverP->location);
    }
    lwm2m_free_block1_buffer(serverP->block1Data);
    lwm2m_free(serverP);
}

static void prv_deleteServerList(lwm2m_context_t * context)
{
    while (NULL != context->serverList)
    {
        lwm2m_server_t * server;
        server = context->serverList;
        context->serverList = server->next;
        prv_deleteServer(server);
    }
}

static void prv_deleteBootstrapServer(lwm2m_server_t * serverP)
{
    // TODO should we free location as in prv_deleteServer ?
    // TODO should we parse transaction and observation to remove the ones related to this server ?
    lwm2m_free_block1_buffer(serverP->block1Data);
    lwm2m_free(serverP);
}

static void prv_deleteBootstrapServerList(lwm2m_context_t * context)
{
    while (NULL != context->bootstrapServerList)
    {
        lwm2m_server_t * server;
        server = context->bootstrapServerList;
        context->bootstrapServerList = server->next;
        prv_deleteBootstrapServer(server);
    }
}

static void prv_deleteObservedList(lwm2m_context_t * contextP)
{
    while (NULL != contextP->observedList)
    {
        lwm2m_observed_t * targetP;
        lwm2m_watcher_t * watcherP;

        targetP = contextP->observedList;
        contextP->observedList = contextP->observedList->next;

        for (watcherP = targetP->watcherList ; watcherP != NULL ; watcherP = watcherP->next)
        {
            if (watcherP->parameters != NULL) lwm2m_free(watcherP->parameters);
        }
        LWM2M_LIST_FREE(targetP->watcherList);

        lwm2m_free(targetP);
    }
    lwm2m_observe_delete_all_data(contextP);
}

static void prv_deleteMessageList(lwm2m_context_t * contextP)
{
    while (NULL != contextP->messageList)
    {
        lwm2m_message_t * targetP;

        targetP = contextP->messageList;
        contextP->messageList = contextP->messageList->next;

        lwm2m_free(targetP);
    }
}
#endif

void prv_deleteTransactionList(lwm2m_context_t * context)
{
    while (NULL != context->transactionList)
    {
        lwm2m_transaction_t * transaction;

        transaction = context->transactionList;
        context->transactionList = context->transactionList->next;
        lwm2m_transaction_free(transaction);
    }
}

void lwm2m_close(lwm2m_context_t * contextP)
{
#ifdef LWM2M_CLIENT_MODE
    if (contextP->mode == CLIENT_MODE) {
        LOG("lwm2m_close");
        lwm2m_deregister(contextP);
        prv_deleteServerList(contextP);
        prv_deleteBootstrapServerList(contextP);
        prv_deleteObservedList(contextP);
        prv_deleteMessageList(contextP);
        lwm2m_free(contextP->endpointName);
        if (contextP->msisdn != NULL)
        {
            lwm2m_free(contextP->msisdn);
        }
        if (contextP->altPath != NULL)
        {
            lwm2m_free(contextP->altPath);
        }
        if (contextP->apnName != NULL)
        {
            lwm2m_free(contextP->apnName);
        }
    }
#endif

#ifdef LWM2M_SERVER_MODE
    if(contextP->mode == SERVER_MODE) {
        while (NULL != contextP->clientList)
        {
            lwm2m_client_t * clientP;

            clientP = contextP->clientList;
            contextP->clientList = contextP->clientList->next;

            registration_freeClient(clientP);
        }
    }
#endif

    prv_deleteTransactionList(contextP);
    lwm2m_free(contextP);
}

void lwm2m_close_not_deregister(lwm2m_context_t * contextP)
{
#ifdef LWM2M_CLIENT_MODE
    if (contextP->mode == CLIENT_MODE) {
        LOG("lwm2m_close_not_deregister");
        prv_deleteServerList(contextP);
        prv_deleteBootstrapServerList(contextP);
        prv_deleteObservedList(contextP);
        prv_deleteMessageList(contextP);
        lwm2m_free(contextP->endpointName);
        if (contextP->msisdn != NULL)
        {
            lwm2m_free(contextP->msisdn);
        }
        if (contextP->altPath != NULL)
        {
            lwm2m_free(contextP->altPath);
        }
        if (contextP->apnName != NULL)
        {
            lwm2m_free(contextP->apnName);
        }
    }
#endif

    prv_deleteTransactionList(contextP);
    lwm2m_free(contextP);
}

#ifdef LWM2M_CLIENT_MODE
static int prv_refreshServerList(lwm2m_context_t * contextP)
{
    lwm2m_server_t * targetP;
    lwm2m_server_t * nextP;

    // Remove all servers marked as dirty
    targetP = contextP->bootstrapServerList;
    contextP->bootstrapServerList = NULL;
    while (targetP != NULL)
    {
        nextP = targetP->next;
        targetP->next = NULL;
        if (!targetP->dirty)
        {
            targetP->status = STATE_DEREGISTERED;
            contextP->bootstrapServerList = (lwm2m_server_t *)LWM2M_LIST_ADD(contextP->bootstrapServerList, targetP);
        }
        else
        {
            prv_deleteServer(targetP);
        }
        targetP = nextP;
    }
    targetP = contextP->serverList;
    contextP->serverList = NULL;
    while (targetP != NULL)
    {
        nextP = targetP->next;
        targetP->next = NULL;
        if (!targetP->dirty)
        {
            // TODO: Should we revert the status to STATE_DEREGISTERED ?
            contextP->serverList = (lwm2m_server_t *)LWM2M_LIST_ADD(contextP->serverList, targetP);
        }
        else
        {
            prv_deleteServer(targetP);
        }
        targetP = nextP;
    }

    return object_getServers(contextP);
}

int lwm2m_configure_ex(lwm2m_context_t * contextP,
                    const char * endpointName,
                    const char * msisdn,
                    const char * altPath,
                    const char * apnName,
                    uint16_t numObject,
                    lwm2m_object_t * objectList[])
{
    int i;
    uint8_t found;

    LOG_ARG("endpointName: \"%s\", msisdn: \"%s\", altPath: \"%s\", apnName: \"%s\", numObject: %d", 
			endpointName ? endpointName : "NULL", 
			msisdn ? msisdn : "NULL",
			altPath ? altPath : "NULL", apnName ? apnName : "NULL", numObject);
    // This API can be called only once for now
    if (contextP->endpointName != NULL || contextP->objectList != NULL) return COAP_400_BAD_REQUEST;

    if (endpointName == NULL) return COAP_400_BAD_REQUEST;
    if (numObject < 3) return COAP_400_BAD_REQUEST;
    // Check that mandatory objects are present
    found = 0;
    for (i = 0 ; i < numObject ; i++)
    {
        if (objectList[i]->objID == LWM2M_SECURITY_OBJECT_ID) found |= 0x01;
        if (objectList[i]->objID == LWM2M_SERVER_OBJECT_ID) found |= 0x02;
        if (objectList[i]->objID == LWM2M_DEVICE_OBJECT_ID) found |= 0x04;
    }
    if (found != 0x07) return COAP_400_BAD_REQUEST;
    if (altPath != NULL)
    {
        if (0 == lwm2m_utils_isAltPathValid(altPath))
        {
            return COAP_400_BAD_REQUEST;
        }
        if (altPath[1] == 0)
        {
            altPath = NULL;
        }
    }
    contextP->endpointName = lwm2m_strdup(endpointName);
    if (contextP->endpointName == NULL)
    {
        return COAP_500_INTERNAL_SERVER_ERROR;
    }

    if (msisdn != NULL)
    {
        contextP->msisdn = lwm2m_strdup(msisdn);
        if (contextP->msisdn == NULL)
        {
            return COAP_500_INTERNAL_SERVER_ERROR;
        }
    }

    if (altPath != NULL)
    {
        contextP->altPath = lwm2m_strdup(altPath);
        if (contextP->altPath == NULL)
        {
            return COAP_500_INTERNAL_SERVER_ERROR;
        }
    }

    if (apnName != NULL)
    {
        contextP->apnName = lwm2m_strdup(apnName);
        if (contextP->apnName == NULL)
        {
            return COAP_500_INTERNAL_SERVER_ERROR;
        }
    }

    for (i = 0; i < numObject; i++)
    {
        objectList[i]->next = NULL;
        contextP->objectList = (lwm2m_object_t *)LWM2M_LIST_ADD(contextP->objectList, objectList[i]);
    }

    if (contextP->state != STATE_READY) {
        lwm2m_observe_delete_all_data(contextP);
    } else {
        object_getServers(contextP);

        lwm2m_observe_retention_data(contextP);

        if (contextP->connect_server_callback) {
            contextP->serverList->sessionH = contextP->connect_server_callback(contextP->serverList->secObjInstID, contextP->userData);
        }

        char g_lwm2m_location[LWM2M_MAX_LOCATION_LEN + 1];
        // server_ip
        uint32_t len;
        nvdm_status_t status;
        len = LWM2M_MAX_LOCATION_LEN;
        status = nvdm_read_data_item(LWM2M_NVDM_GROUP_NAME, LWM2M_NVDM_ITEM_NAME_LOCATION, (uint8_t*)g_lwm2m_location, &len);
        LOG_ARG("read nvdm [%s: %s] = %d", LWM2M_NVDM_ITEM_NAME_LOCATION, g_lwm2m_location, status);
        /* retention, copy location */
        contextP->serverList->location = lwm2m_malloc(strlen(g_lwm2m_location) + 1); // for String terminator
        strcpy(contextP->serverList->location, g_lwm2m_location);
    }

    return COAP_NO_ERROR;
}

int lwm2m_configure(lwm2m_context_t * contextP,
                    const char * endpointName,
                    const char * msisdn,
                    const char * altPath,
                    uint16_t numObject,
                    lwm2m_object_t * objectList[])
{
    return lwm2m_configure_ex(contextP, endpointName, msisdn, altPath, NULL, numObject, objectList);
}

int lwm2m_add_object(lwm2m_context_t * contextP,
                     lwm2m_object_t * objectP)
{
    lwm2m_object_t * targetP;

    LOG_ARG("lwm2m_add_object ID: %d", objectP->objID);
    targetP = (lwm2m_object_t *)LWM2M_LIST_FIND(contextP->objectList, objectP->objID);
    if (targetP != NULL) return COAP_406_NOT_ACCEPTABLE;
    objectP->next = NULL;

    contextP->objectList = (lwm2m_object_t *)LWM2M_LIST_ADD(contextP->objectList, objectP);

    if (contextP->state == STATE_READY)
    {
        return lwm2m_update_registration(contextP, 0, true);
    }

    return COAP_NO_ERROR;
}

int lwm2m_remove_object(lwm2m_context_t * contextP,
                        uint16_t id)
{
    lwm2m_object_t * targetP;

    LOG_ARG("lwm2m_remove_object ID: %d", id);
    contextP->objectList = (lwm2m_object_t *)LWM2M_LIST_RM(contextP->objectList, id, &targetP);

    if (targetP == NULL) return COAP_404_NOT_FOUND;

    if (contextP->state == STATE_READY)
    {
        return lwm2m_update_registration(contextP, 0, true);
    }

    return 0;
}

#endif


int lwm2m_step(lwm2m_context_t * contextP,
               time_t * timeoutP)
{
    time_t tv_sec;
    int result;

    LOG_ARG("lwm2m_step timeoutP: %d", *timeoutP);
    tv_sec = lwm2m_gettime();
    if (tv_sec < 0) return COAP_500_INTERNAL_SERVER_ERROR;

#ifdef LWM2M_CLIENT_MODE
    if (contextP->mode == CLIENT_MODE) {
        LOG_ARG("State: %s", STR_STATE(contextP->state));
        // state can also be modified in bootstrap_handleCommand().

    next_step:
        switch (contextP->state)
        {
        case STATE_INITIAL:
            if (0 != prv_refreshServerList(contextP)) return COAP_503_SERVICE_UNAVAILABLE;
            if (contextP->serverList != NULL)
            {
                contextP->state = STATE_REGISTER_REQUIRED;
            }
            else
            {
                // Bootstrapping
                contextP->state = STATE_BOOTSTRAP_REQUIRED;
#ifdef MTK_TMO_CERT_SUPPORT
                contextP->bs_retry_times = 0;
                contextP->register_retry_times = 0;
#endif
            }
            goto next_step;
            break;

        case STATE_BOOTSTRAP_REQUIRED:
#ifdef LWM2M_BOOTSTRAP
            if (contextP->bootstrapServerList != NULL)
            {
                bootstrap_start(contextP);
                contextP->state = STATE_BOOTSTRAPPING;
                lwm2m_bootstrap_step(contextP, tv_sec, timeoutP);
            }
            else
#endif
            {
                return COAP_503_SERVICE_UNAVAILABLE;
            }
            break;

#ifdef LWM2M_BOOTSTRAP
        case STATE_BOOTSTRAPPING:
            switch (lwm2m_bootstrap_getStatus(contextP))
            {
            case STATE_BS_FINISHED:
                contextP->state = STATE_INITIAL;
                goto next_step;
                break;

            case STATE_BS_FAILED:
#ifdef MTK_TMO_CERT_SUPPORT
                contextP->bs_retry_times++;
                LOG_ARG("bs_retry_times: %d", contextP->bs_retry_times);
                if (contextP->bs_retry_times > 1) {
                    if (contextP->notify_callback != NULL) contextP->notify_callback(LWM2M_NOTIFY_TYPE_BS_FAILED_BACKOFF, LWM2M_NOTIFY_CODE_SUCCESS, 0);
                    return COAP_503_SERVICE_UNAVAILABLE;
                } else {
                    contextP->state = STATE_BOOTSTRAP_REQUIRED;
                    goto next_step;
                    break;
                }
#else /* MTK_TMO_CERT_SUPPORT */
                return COAP_503_SERVICE_UNAVAILABLE;
#endif

            default:
                // keep on waiting
                lwm2m_bootstrap_step(contextP, tv_sec, timeoutP);
                break;
            }
            break;
#endif
        case STATE_REGISTER_REQUIRED:
#ifdef MTK_TMO_CERT_SUPPORT
            contextP->bs_retry_times = 0;
#endif
            result = lwm2m_registration_start(contextP);
#ifdef MTK_TMO_CERT_SUPPORT
            if (COAP_NO_ERROR != result) {
                contextP->register_retry_times++;
                LOG_ARG("register_retry_times: %d", contextP->register_retry_times);
                if (contextP->register_retry_times > 1) {
                    // need to delete lwm2m config & current task and register with BS again
                    if (contextP->notify_callback != NULL) contextP->notify_callback(LWM2M_NOTIFY_TYPE_REG_FAILED_BACKOFF, LWM2M_NOTIFY_CODE_SUCCESS, 0);
                }
                return result;
            }
#else /* MTK_TMO_CERT_SUPPORT */
            if (COAP_NO_ERROR != result) return result;
#endif /* MTK_TMO_CERT_SUPPORT */
            contextP->state = STATE_REGISTERING;
            break;

        case STATE_REGISTERING:
        {
            switch (lwm2m_registration_getStatus(contextP))
            {
            case STATE_REGISTERED:
                contextP->state = STATE_READY;
#ifdef MTK_TMO_CERT_SUPPORT
                contextP->register_retry_times = 0;
#endif
                break;

            case STATE_REG_FAILED:
#ifdef MTK_TMO_CERT_SUPPORT
                contextP->register_retry_times++;
                LOG_ARG("register_retry_times: %d", contextP->register_retry_times);
                if (contextP->register_retry_times > 1) {
                    // need to delete lwm2m config & current task and register with BS again
                    if (contextP->notify_callback != NULL) contextP->notify_callback(LWM2M_NOTIFY_TYPE_REG_FAILED_BACKOFF, LWM2M_NOTIFY_CODE_SUCCESS, 0);
                } else {
                    contextP->state = STATE_REGISTER_REQUIRED;
                    goto next_step;
                }
#else /* MTK_TMO_CERT_SUPPORT */
                // TODO avoid infinite loop by checking the bootstrap info is different
                contextP->state = STATE_BOOTSTRAP_REQUIRED;
                goto next_step;
#endif /* MTK_TMO_CERT_SUPPORT */
                break;

            case STATE_REG_PENDING:
            default:
                // keep on waiting
                break;
            }
        }
        break;

        case STATE_READY:
            if (lwm2m_registration_getStatus(contextP) == STATE_REG_FAILED)
            {
                // TODO avoid infinite loop by checking the bootstrap info is different
                contextP->state = STATE_BOOTSTRAP_REQUIRED;
                goto next_step;
                break;
            }
            break;

        default:
            // do nothing
            break;
        }
        lwm2m_observe_step(contextP, tv_sec, timeoutP);
    }
#endif

    lwm2m_registration_step(contextP, tv_sec, timeoutP);
    lwm2m_transaction_step(contextP, tv_sec, timeoutP);

    LOG_ARG("Final timeoutP: %d", *timeoutP);
#ifdef LWM2M_CLIENT_MODE
    if (contextP->mode == CLIENT_MODE) {
        LOG_ARG("Final state: %s", STR_STATE(contextP->state));
    }
#endif
    return 0;
}
