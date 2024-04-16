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
 *    Toby Jaffey - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
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
#include <stdio.h>

#include "syslog.h"
#include "sockets.h"

#include "memory_attribute.h"
#include "nvdm.h"
#include "lwm2m_objects.h"

#ifdef LWM2M_CLIENT_MODE
typedef struct _lwm2m_observe_backup_t
{
    uint8_t is_used;
    uint8_t app_id;
    //observe resource
    uint8_t flag;
    uint16_t objectId;
    uint16_t instanceId;
    uint16_t resourceId;
    //watcher
    uint8_t token[8];
    size_t tokenLen;
    time_t lastTime;
    uint32_t counter;
    uint16_t lastMid;
    union
    {
        int64_t asInteger;
        double  asFloat;
    } lastValue;
    //server id
    uint16_t secObjInstID;
    //other parameter
}lwm2m_observe_backup_t;

#ifdef MTK_TMO_CERT_SUPPORT
#define MAX_LWM2M_OBSERVE_COUNT  10
#else
#define MAX_LWM2M_OBSERVE_COUNT  5
#endif
static lwm2m_observe_backup_t g_lwm2m_observe_back[MAX_LWM2M_OBSERVE_COUNT];

/* Group Name */
#define LWM2M_OBSERVED_NVDM_GROUP_NAME_BASE        "lw_ob%d"

/* Item Name */
#define LWM2M_OBSERVED_NVDM_ITEM_NAME_CONTENT      "lw_observed"

static void lwm2m_observe_read_nvdm(int index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char group_name[16] = {0};
    uint32_t len;
    nvdm_status_t status;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (index < 0 || index >= MAX_LWM2M_OBSERVE_COUNT) return;

    sprintf(group_name, LWM2M_OBSERVED_NVDM_GROUP_NAME_BASE, index);
    len = sizeof(lwm2m_observe_backup_t);
    status = nvdm_read_data_item(group_name, LWM2M_OBSERVED_NVDM_ITEM_NAME_CONTENT, (uint8_t*)&g_lwm2m_observe_back[index], &len);
    LOG_ARG("read nvdm [%s: %d] = %d", LWM2M_OBSERVED_NVDM_ITEM_NAME_CONTENT, index, status);
}

static void lwm2m_observe_write_nvdm(int index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char group_name[16] = {0};
    uint32_t len;
    nvdm_status_t status;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (index < 0 || index >= MAX_LWM2M_OBSERVE_COUNT) return;

    sprintf(group_name, LWM2M_OBSERVED_NVDM_GROUP_NAME_BASE, index);
    len = sizeof(lwm2m_observe_backup_t);
    status = nvdm_write_data_item(group_name, LWM2M_OBSERVED_NVDM_ITEM_NAME_CONTENT, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t*)&g_lwm2m_observe_back[index], len);
    LOG_ARG("write nvdm [%s: %d] = %d", LWM2M_OBSERVED_NVDM_ITEM_NAME_CONTENT, index, status);
}

//char g_lwm2m_location[LWM2M_MAX_LOCATION_LEN + 1];

static lwm2m_observed_t * prv_findObserved(lwm2m_context_t * contextP,
                                           lwm2m_uri_t * uriP)
{
    lwm2m_observed_t * targetP;

    targetP = contextP->observedList;
    while (targetP != NULL
        && (targetP->uri.objectId != uriP->objectId
         || targetP->uri.flag != uriP->flag
         || (LWM2M_URI_IS_SET_INSTANCE(uriP) && targetP->uri.instanceId != uriP->instanceId)
         || (LWM2M_URI_IS_SET_RESOURCE(uriP) && targetP->uri.resourceId != uriP->resourceId)))
    {
        targetP = targetP->next;
    }

    return targetP;
}

static void prv_unlinkObserved(lwm2m_context_t * contextP,
                               lwm2m_observed_t * observedP)
{
    if (contextP->observedList == observedP)
    {
        contextP->observedList = contextP->observedList->next;
    }
    else
    {
        lwm2m_observed_t * parentP;

        parentP = contextP->observedList;
        while (parentP->next != NULL
            && parentP->next != observedP)
        {
            parentP = parentP->next;
        }
        if (parentP->next != NULL)
        {
            parentP->next = parentP->next->next;
        }
    }
}

static lwm2m_watcher_t * prv_findWatcher(lwm2m_observed_t * observedP,
                                         lwm2m_server_t * serverP)
{
    lwm2m_watcher_t * targetP;

    targetP = observedP->watcherList;
    while (targetP != NULL
        && targetP->server != serverP)
    {
        targetP = targetP->next;
    }

    return targetP;
}

static lwm2m_watcher_t * prv_getWatcher(lwm2m_context_t * contextP,
                                        lwm2m_uri_t * uriP,
                                        lwm2m_server_t * serverP)
{
    lwm2m_observed_t * observedP;
    bool allocatedObserver;
    lwm2m_watcher_t * watcherP;

    allocatedObserver = false;

    observedP = prv_findObserved(contextP, uriP);
    if (observedP == NULL)
    {
        observedP = (lwm2m_observed_t *)lwm2m_malloc(sizeof(lwm2m_observed_t));
        if (observedP == NULL) return NULL;
        allocatedObserver = true;
        memset(observedP, 0, sizeof(lwm2m_observed_t));
        memcpy(&(observedP->uri), uriP, sizeof(lwm2m_uri_t));
        observedP->next = contextP->observedList;
        contextP->observedList = observedP;
    }

    watcherP = prv_findWatcher(observedP, serverP);
    if (watcherP == NULL)
    {
        watcherP = (lwm2m_watcher_t *)lwm2m_malloc(sizeof(lwm2m_watcher_t));
        if (watcherP == NULL)
        {
            if (allocatedObserver == true)
            {
                lwm2m_free(observedP);
            }
            return NULL;
        }
        memset(watcherP, 0, sizeof(lwm2m_watcher_t));
        watcherP->active = false;
        watcherP->server = serverP;
        watcherP->next = observedP->watcherList;
        observedP->watcherList = watcherP;
    }

    return watcherP;
}

void lwm2m_observe_display_data(lwm2m_observe_backup_t *backup)
{
    LOG_ARG("uri: flag %d, /%d/%d/%d\n", backup->flag, backup->objectId, backup->instanceId, backup->resourceId);
    LOG_ARG("sec obj inst id %d\n", backup->secObjInstID);
    LOG_ARG("watcher token len %d, token: %s\n", backup->tokenLen, backup->token);
    LOG_ARG("watcher lt: %d, lm: %d, lv: %f, couter %d\n", backup->lastTime, backup->lastMid, backup->lastValue.asFloat, backup->counter);
}


void lwm2m_observe_copy_data(lwm2m_observe_backup_t *backup,
                                    lwm2m_context_t * contextP,
                                    lwm2m_uri_t * uriP,
                                    lwm2m_server_t * serverP)
{
    lwm2m_watcher_t *watcherP;
    backup->app_id = contextP->app_id;
    backup->flag = uriP->flag;
    backup->objectId = uriP->objectId;
    backup->instanceId = uriP->instanceId;
    backup->resourceId = uriP->resourceId;
    backup->secObjInstID = serverP->secObjInstID;
    watcherP = prv_getWatcher(contextP, uriP, serverP);

    backup->tokenLen =  watcherP->tokenLen;
    memcpy(backup->token, watcherP->token, watcherP->tokenLen);
    backup->lastTime = watcherP->lastTime;
    backup->lastMid = watcherP->lastMid;
    backup->lastValue.asFloat = watcherP->lastValue.asFloat;
    backup->counter = watcherP->counter;
    lwm2m_observe_display_data(backup);
}

void lwm2m_observe_update_data(lwm2m_context_t * contextP,
                                        lwm2m_uri_t * uriP,
                                        lwm2m_watcher_t *watcherP)
{
    int i;

    LOG_ARG("update_data URI:flag %d,/%d/%d/%d\n", uriP->flag, uriP->objectId, uriP->instanceId, uriP->resourceId);
    for(i = 0; i < MAX_LWM2M_OBSERVE_COUNT; i++) {
        //first need to check save before
        if(g_lwm2m_observe_back[i].is_used == 1 &&
           g_lwm2m_observe_back[i].app_id == contextP->app_id &&
           g_lwm2m_observe_back[i].flag == uriP->flag &&
           g_lwm2m_observe_back[i].objectId == uriP->objectId &&
           g_lwm2m_observe_back[i].instanceId == uriP->instanceId &&
           g_lwm2m_observe_back[i].resourceId == uriP->resourceId) {
            lwm2m_observe_backup_t *backup = &g_lwm2m_observe_back[i];
            backup->lastTime = watcherP->lastTime;
            backup->lastMid = watcherP->lastMid;
            backup->lastValue.asFloat = watcherP->lastValue.asFloat;
            backup->counter = watcherP->counter;
            lwm2m_observe_display_data(backup);
            lwm2m_observe_write_nvdm(i);
            break;
        }
    }
}


void lwm2m_observe_save_data(lwm2m_context_t * contextP,
                                    lwm2m_uri_t * uriP,
                                    lwm2m_server_t * serverP)
{
    int i;
    for(i = 0; i < MAX_LWM2M_OBSERVE_COUNT; i++) {
        //first need to check save before
        if(g_lwm2m_observe_back[i].is_used == 1 &&
           g_lwm2m_observe_back[i].app_id == contextP->app_id &&
           g_lwm2m_observe_back[i].flag == uriP->flag &&
           g_lwm2m_observe_back[i].objectId == uriP->objectId &&
           g_lwm2m_observe_back[i].instanceId == uriP->instanceId &&
           g_lwm2m_observe_back[i].resourceId == uriP->resourceId) {
            //save before, just update
            LOG_ARG("update data retention.\n");
            lwm2m_observe_copy_data(&g_lwm2m_observe_back[i], contextP, uriP, serverP);
            lwm2m_observe_write_nvdm(i);
            break;
        }
    }
    if(i == MAX_LWM2M_OBSERVE_COUNT) {
        for(i = 0; i < MAX_LWM2M_OBSERVE_COUNT; i++) {
            //first save. need to check empty item
            if(g_lwm2m_observe_back[i].is_used == 0) {
                //save item.
                lwm2m_observe_copy_data(&g_lwm2m_observe_back[i], contextP, uriP, serverP);
                g_lwm2m_observe_back[i].is_used = 1;
                lwm2m_observe_write_nvdm(i);
                break;
            }
        }
        if (i == MAX_LWM2M_OBSERVE_COUNT) {
            LOG_ARG("can't save, up to max count.\n");
        }
    }
}

void lwm2m_observe_delete_data(lwm2m_context_t * contextP, lwm2m_uri_t * uriP)
{
    int i;
    for (i = 0; i < MAX_LWM2M_OBSERVE_COUNT; i++) {
        //first need to check if save before
        if (g_lwm2m_observe_back[i].is_used == 1 &&
           g_lwm2m_observe_back[i].app_id == contextP->app_id &&
           g_lwm2m_observe_back[i].flag == uriP->flag &&
           g_lwm2m_observe_back[i].objectId == uriP->objectId &&
           g_lwm2m_observe_back[i].instanceId == uriP->instanceId &&
           g_lwm2m_observe_back[i].resourceId == uriP->resourceId) {
            //save before, just delete
            LOG_ARG("delete data retention.\n");
            g_lwm2m_observe_back[i].is_used = 0;
            lwm2m_observe_write_nvdm(i);
            return;
        }
    }
}

void lwm2m_observe_delete_all_data(lwm2m_context_t * contextP)
{
    int i;
    for (i = 0; i < MAX_LWM2M_OBSERVE_COUNT; i++) {
        //first need to check if save before
        if (g_lwm2m_observe_back[i].is_used == 1 &&
           g_lwm2m_observe_back[i].app_id == contextP->app_id) {
            //save before, just delete
            LOG_ARG("delete data retention.\n");
            g_lwm2m_observe_back[i].is_used = 0;
            lwm2m_observe_write_nvdm(i);
        }
    }
}

void lwm2m_observe_retention_data(lwm2m_context_t * contextP)
{
    int i;
    for(i = 0; i < MAX_LWM2M_OBSERVE_COUNT; i++) {
        lwm2m_observe_read_nvdm(i);
        if (g_lwm2m_observe_back[i].is_used == 1 &&
           g_lwm2m_observe_back[i].app_id == contextP->app_id) {
            lwm2m_uri_t uriP;
            lwm2m_server_t *serP = NULL;

            uriP.flag = g_lwm2m_observe_back[i].flag;
            uriP.objectId = g_lwm2m_observe_back[i].objectId;
            uriP.instanceId = g_lwm2m_observe_back[i].instanceId;
            uriP.resourceId = g_lwm2m_observe_back[i].resourceId;

            LOG_ARG("lwm2m_observe_retention_data URI:/%d/%d/%d\n", uriP.objectId, uriP.instanceId, uriP.resourceId);
            serP = contextP->serverList;
            while(serP != NULL) {
                if(serP->secObjInstID == g_lwm2m_observe_back[i].secObjInstID) {
                    break;
                }
                serP = serP->next;
            }
            if(serP != NULL) {
                lwm2m_watcher_t *watcherP = NULL;
                watcherP = prv_getWatcher(contextP, &uriP, serP);
                if (watcherP == NULL) {
                    LOG_ARG("watcherP is NULL\n");
                    return;
                }
                LOG_ARG("Add watcherP\n");
                watcherP->tokenLen = g_lwm2m_observe_back[i].tokenLen;
                memcpy(watcherP->token, g_lwm2m_observe_back[i].token, g_lwm2m_observe_back[i].tokenLen);
                watcherP->active = true;
                watcherP->update = false;
                watcherP->lastTime = g_lwm2m_observe_back[i].lastTime;
                watcherP->lastMid = g_lwm2m_observe_back[i].lastMid;
                watcherP->lastValue.asFloat = g_lwm2m_observe_back[i].lastValue.asFloat;
                watcherP->counter = g_lwm2m_observe_back[i].counter;
            } else {
                LOG_ARG("server is NULL\n");
            }
        }
    }
}

coap_status_t lwm2m_observe_handleRequest(lwm2m_context_t * contextP,
                                    lwm2m_uri_t * uriP,
                                    lwm2m_server_t * serverP,
                                    int size,
                                    lwm2m_data_t * dataP,
                                    coap_packet_t * message,
                                    coap_packet_t * response)
{
    lwm2m_watcher_t * watcherP;
    uint32_t count;

    LOG_ARG("lwm2m_observe_handleRequest Code: %02X, server status: %s", message->code, STR_STATUS(serverP->status));
    LOG_URI(uriP);

    coap_get_header_observe(message, &count);

    switch (count)
    {
    case 0:
        if (!LWM2M_URI_IS_SET_INSTANCE(uriP) && LWM2M_URI_IS_SET_RESOURCE(uriP)) return COAP_400_BAD_REQUEST;
        if (message->token_len == 0) return COAP_400_BAD_REQUEST;

        watcherP = prv_getWatcher(contextP, uriP, serverP);
        if (watcherP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;

        watcherP->tokenLen = message->token_len;
        memcpy(watcherP->token, message->token, message->token_len);
        watcherP->active = true;
        watcherP->lastTime = lwm2m_gettime();

        if (LWM2M_URI_IS_SET_RESOURCE(uriP))
        {
            switch (dataP->type)
            {
            case LWM2M_TYPE_INTEGER:
                if (1 != lwm2m_data_decode_int(dataP, &(watcherP->lastValue.asInteger))) return COAP_500_INTERNAL_SERVER_ERROR;
                break;
            case LWM2M_TYPE_FLOAT:
                if (1 != lwm2m_data_decode_float(dataP, &(watcherP->lastValue.asFloat))) return COAP_500_INTERNAL_SERVER_ERROR;
                break;
            default:
                break;
            }
        }

        coap_set_header_observe(response, watcherP->counter++);
        lwm2m_observe_save_data(contextP, uriP, serverP);

#ifdef MTK_LWM2M_CT_SUPPORT
        extern void ctiot_lwm2m_client_observe_success(void);
        if (uriP->objectId == LWM2M_WATERMETER_OBJECT_ID && uriP->instanceId == 0 && uriP->resourceId == 0)
        {
            if (LWM2M_URI_IS_SET_RESOURCE(uriP)) ctiot_lwm2m_client_observe_success();
        }
#endif
        if (uriP->objectId == LWM2M_WATERMETER_OBJECT_ID && uriP->instanceId == 0 && uriP->resourceId == 0)
        {
            if (LWM2M_URI_IS_SET_RESOURCE(uriP) && contextP->notify_callback != NULL) contextP->notify_callback(LWM2M_NOTIFY_TYPE_OBSERVE, LWM2M_NOTIFY_CODE_SUCCESS, 0);
        }

        // ADD FOR ATCMD
        if (contextP->observe_callback) {
            contextP->observe_callback(0, message->mid, uriP, contextP->observe_userdata);
        }

        return COAP_205_CONTENT;

    case 1:
        // cancellation
        lwm2m_observe_cancel(contextP, LWM2M_MAX_ID, serverP->sessionH);
        lwm2m_observe_delete_data(contextP, uriP);
        // ADD FOR ATCMD
        if (contextP->observe_callback) {
            contextP->observe_callback(1, message->mid, uriP, contextP->observe_userdata);
        }
        if (uriP->objectId == LWM2M_WATERMETER_OBJECT_ID && uriP->instanceId == 0 && uriP->resourceId == 0)
        {
            if (LWM2M_URI_IS_SET_RESOURCE(uriP))
            {
                if (contextP->notify_callback != NULL) contextP->notify_callback(LWM2M_NOTIFY_TYPE_OBSERVE, LWM2M_NOTIFY_CODE_CANCEL_OBSERVE, 0);
            }
        }
        return COAP_205_CONTENT;

    default:
        return COAP_400_BAD_REQUEST;
    }
}

void lwm2m_observe_cancel(lwm2m_context_t * contextP,
                    uint16_t mid,
                    void * fromSessionH)
{
    lwm2m_observed_t * observedP;

    LOG_ARG("lwm2m_observe_cancel mid: %d", mid);

    for (observedP = contextP->observedList;
         observedP != NULL;
         observedP = observedP->next)
    {
        lwm2m_watcher_t * targetP = NULL;

        if ((LWM2M_MAX_ID == mid || observedP->watcherList->lastMid == mid)
         && lwm2m_session_is_equal(observedP->watcherList->server->sessionH, fromSessionH, contextP->userData))
        {
            targetP = observedP->watcherList;
            observedP->watcherList = observedP->watcherList->next;
        }
        else
        {
            lwm2m_watcher_t * parentP;

            parentP = observedP->watcherList;
            while (parentP->next != NULL
                && (parentP->next->lastMid != mid
                 || lwm2m_session_is_equal(parentP->next->server->sessionH, fromSessionH, contextP->userData)))
            {
                parentP = parentP->next;
            }
            if (parentP->next != NULL)
            {
                targetP = parentP->next;
                parentP->next = parentP->next->next;
            }
        }
        if (targetP != NULL)
        {
            lwm2m_free(targetP);
            if (observedP->watcherList == NULL)
            {
                prv_unlinkObserved(contextP, observedP);
                lwm2m_free(observedP);
            }
            return;
        }
    }
}

coap_status_t lwm2m_observe_setParameters(lwm2m_context_t * contextP,
                                    lwm2m_uri_t * uriP,
                                    lwm2m_server_t * serverP,
                                    lwm2m_attributes_t * attrP)
{
    uint8_t result;
    lwm2m_watcher_t * watcherP;

    LOG_URI(uriP);
    //LOG_I(lwm2m_c, "toSet: %08X, toClear: %08X, minPeriod: %d, maxPeriod: %d, greaterThan: %f, lessThan: %f, step: %f",
    //        attrP->toSet, attrP->toClear, attrP->minPeriod, attrP->maxPeriod, attrP->greaterThan, attrP->lessThan, attrP->step);
    LOG_ARG("toSet: %08X, toClear: %08X, minPeriod: %d, maxPeriod: %d, greaterThan: %f, lessThan: %f, step: %f",
            attrP->toSet, attrP->toClear, attrP->minPeriod, attrP->maxPeriod, attrP->greaterThan, attrP->lessThan, attrP->step);

    if (!LWM2M_URI_IS_SET_INSTANCE(uriP) && LWM2M_URI_IS_SET_RESOURCE(uriP)) return COAP_400_BAD_REQUEST;

    result = object_checkReadable(contextP, uriP);
    if (COAP_205_CONTENT != result) return result;

    if (0 != (attrP->toSet & ATTR_FLAG_NUMERIC))
    {
        result = object_checkNumeric(contextP, uriP);
        if (COAP_205_CONTENT != result) return result;
    }

    watcherP = prv_getWatcher(contextP, uriP, serverP);
    if (watcherP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;

    // Check rule "lt" value + 2*"stp" values < "gt" value
    if ((((attrP->toSet | (watcherP->parameters?watcherP->parameters->toSet:0)) & ~attrP->toClear) & ATTR_FLAG_NUMERIC) == ATTR_FLAG_NUMERIC)
    {
        float gt;
        float lt;
        float stp;

        if (0 != (attrP->toSet & LWM2M_ATTR_FLAG_GREATER_THAN))
        {
            gt = attrP->greaterThan;
        }
        else
        {
            gt = watcherP->parameters->greaterThan;
        }
        if (0 != (attrP->toSet & LWM2M_ATTR_FLAG_LESS_THAN))
        {
            lt = attrP->lessThan;
        }
        else
        {
            lt = watcherP->parameters->lessThan;
        }
        if (0 != (attrP->toSet & LWM2M_ATTR_FLAG_STEP))
        {
            stp = attrP->step;
        }
        else
        {
            stp = watcherP->parameters->step;
        }

        if (lt + (2 * stp) >= gt) return COAP_400_BAD_REQUEST;
    }

    if (watcherP->parameters == NULL)
    {
        if (attrP->toSet != 0)
        {
            watcherP->parameters = (lwm2m_attributes_t *)lwm2m_malloc(sizeof(lwm2m_attributes_t));
            if (watcherP->parameters == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
            memcpy(watcherP->parameters, attrP, sizeof(lwm2m_attributes_t));
        }
    }
    else
    {
        watcherP->parameters->toSet &= ~attrP->toClear;
        if (attrP->toSet & LWM2M_ATTR_FLAG_MIN_PERIOD)
        {
            watcherP->parameters->minPeriod = attrP->minPeriod;
        }
        if (attrP->toSet & LWM2M_ATTR_FLAG_MAX_PERIOD)
        {
            watcherP->parameters->maxPeriod = attrP->maxPeriod;
        }
        if (attrP->toSet & LWM2M_ATTR_FLAG_GREATER_THAN)
        {
            watcherP->parameters->greaterThan = attrP->greaterThan;
        }
        if (attrP->toSet & LWM2M_ATTR_FLAG_LESS_THAN)
        {
            watcherP->parameters->lessThan = attrP->lessThan;
        }
        if (attrP->toSet & LWM2M_ATTR_FLAG_STEP)
        {
            watcherP->parameters->step = attrP->step;
        }
    }

    LOG_ARG("Final toSet: %08X, minPeriod: %d, maxPeriod: %d, greaterThan: %f, lessThan: %f, step: %f",
            watcherP->parameters->toSet, watcherP->parameters->minPeriod, watcherP->parameters->maxPeriod, watcherP->parameters->greaterThan, watcherP->parameters->lessThan, watcherP->parameters->step);

    // ADD FOR ATCMD
    if (contextP->parameter_callback) {
        contextP->parameter_callback(uriP, attrP, contextP->observe_userdata);
    }

    return COAP_204_CHANGED;
}

lwm2m_observed_t * lwm2m_observe_findByUri(lwm2m_context_t * contextP,
                                     lwm2m_uri_t * uriP)
{
    lwm2m_observed_t * targetP;

    LOG_URI(uriP);
    targetP = contextP->observedList;
    while (targetP != NULL)
    {
        if (targetP->uri.objectId == uriP->objectId)
        {
            if ((!LWM2M_URI_IS_SET_INSTANCE(uriP) && !LWM2M_URI_IS_SET_INSTANCE(&(targetP->uri)))
             || (LWM2M_URI_IS_SET_INSTANCE(uriP) && LWM2M_URI_IS_SET_INSTANCE(&(targetP->uri)) && (uriP->instanceId == targetP->uri.instanceId)))
             {
                 if ((!LWM2M_URI_IS_SET_RESOURCE(uriP) && !LWM2M_URI_IS_SET_RESOURCE(&(targetP->uri)))
                     || (LWM2M_URI_IS_SET_RESOURCE(uriP) && LWM2M_URI_IS_SET_RESOURCE(&(targetP->uri)) && (uriP->resourceId == targetP->uri.resourceId)))
                 {
                     LOG_ARG("Found one with%s observers.", targetP->watcherList ? "" : " no");
                     LOG_URI(&(targetP->uri));
                     return targetP;
                 }
             }
        }
        targetP = targetP->next;
    }

    LOG("Found nothing");
    return NULL;
}

lwm2m_message_t * lwm2m_observe_addMessage(lwm2m_context_t * contextP, uint16_t mid)
{
    lwm2m_message_t * messageP;

    messageP = (lwm2m_message_t *)lwm2m_malloc(sizeof(lwm2m_message_t));
    if (messageP == NULL) return NULL;
    memset(messageP, 0, sizeof(lwm2m_message_t));
    messageP->mid = mid;
    if (contextP->messageList == NULL) {
        contextP->messageList = messageP;
    } else {
        lwm2m_message_t *traverse = contextP->messageList;
        while (traverse->next != NULL) {
            traverse = traverse->next;
        }
        traverse->next = messageP;
    }
    LOG_ARG("lwm2m_observe add message id = %d", mid);
    return messageP;
}

void lwm2m_observe_freeMessage(lwm2m_context_t * contextP, uint16_t mid)
{
    lwm2m_message_t *traverse = contextP->messageList;
    lwm2m_message_t *previous = NULL;
    while (traverse != NULL) {
        if (traverse->mid == mid) {
            if (previous == NULL) {
                contextP->messageList = traverse->next;
            } else {
                previous->next = traverse->next;
            }
            LOG_ARG("lwm2m_observe free message id = %d", mid);
            lwm2m_free(traverse);
            break;
        }
        previous = traverse;
        traverse = traverse->next;
    }
}

lwm2m_message_t * lwm2m_observe_findMessage(lwm2m_context_t * contextP, uint16_t mid)
{
    lwm2m_message_t * traverse;
    traverse = contextP->messageList;
    while (traverse)
    {
        if (traverse->mid == mid)
        {
            LOG_ARG("lwm2m_observe find message id = %d", mid);
            return traverse;
        }
        traverse = traverse->next;
    }

    return NULL;
}

static bool watcher_flag = false;

void lwm2m_resource_value_changed(lwm2m_context_t * contextP,
                                  lwm2m_uri_t * uriP)
{
    lwm2m_observed_t * targetP;
    watcher_flag = false;

    LOG_URI(uriP);
    targetP = contextP->observedList;
    if(targetP == NULL)
    {
        LOG("no watcher found here");
    }else {
        while (targetP != NULL)
        {
            LOG_URI(&(targetP->uri));
            if (targetP->uri.objectId == uriP->objectId)
            {
                if (!LWM2M_URI_IS_SET_INSTANCE(uriP)
                 || (targetP->uri.flag & LWM2M_URI_FLAG_INSTANCE_ID) == 0
                 || uriP->instanceId == targetP->uri.instanceId)
                {
                    if (!LWM2M_URI_IS_SET_RESOURCE(uriP)
                     || (targetP->uri.flag & LWM2M_URI_FLAG_RESOURCE_ID) == 0
                     || uriP->resourceId == targetP->uri.resourceId)
                    {
                        lwm2m_watcher_t * watcherP;

                    LOG("Found an observation");
                    LOG_URI(&(targetP->uri));

                        for (watcherP = targetP->watcherList ; watcherP != NULL ; watcherP = watcherP->next)
                        {
                            if (watcherP->active == true)
                            {
                                LOG("Tagging a watcher");

                                watcherP->update = true;

                                watcher_flag = true;
                            }
                        }
                        //by Yy
                        if(false == watcher_flag) {
                            LOG("no watcher found here");
                        }
                    }
                }
            }
            targetP = targetP->next;
        }
    }
}

void lwm2m_data_notify(lwm2m_context_t * contextP,
                       lwm2m_uri_t * uriP,
                       const char * value,
                       size_t valueLength)
{
    lwm2m_object_t * object = (lwm2m_object_t *)LWM2M_LIST_FIND(contextP->objectList, uriP->objectId);

    if (NULL != object)
    {
        if (object->writeFunc != NULL)
        {
            lwm2m_data_t * dataP;
            int result;

            dataP = lwm2m_data_new(1);
            if (dataP == NULL)
            {
                LOG("Internal allocation failure !\n");
                return;
            }
            dataP->id = uriP->resourceId;
#if defined(MTK_LWM2M_CT_SUPPORT) || defined(MTK_CTM2M_SUPPORT) || defined(MTK_CTIOT_SUPPORT)
            if (uriP->objectId == LWM2M_WATERMETER_OBJECT_ID)
            {
                lwm2m_data_encode_opaque((uint8_t *)value, valueLength, dataP);
                result = watermeter_change(dataP, object);
            }
            else
            {
#endif
            lwm2m_data_encode_nstring(value, valueLength, dataP);

            result = object->writeFunc(contextP, uriP->instanceId, 1, dataP, object);
            if (COAP_405_METHOD_NOT_ALLOWED == result)
            {
                switch (uriP->objectId)
                {
                case LWM2M_DEVICE_OBJECT_ID:
                    result = device_change(dataP, object);
                    break;
                case LWM2M_BAROMETER_OBJECT_ID:
                    result = barometer_change(dataP, object);
                    break;
                case LWM2M_ACCELEROMTER_OBJECT_ID:
                    result = accelerometer_change(dataP, object);
                    break;
                default:
                    break;
                }
            }
#if defined(MTK_LWM2M_CT_SUPPORT) || defined(MTK_CTM2M_SUPPORT) || defined(MTK_CTIOT_SUPPORT)
            }
#endif

            if (COAP_204_CHANGED != result)
            {
                LOG("Failed to change value!\n");
            }
            else
            {
                LOG("value changed!\n");
                lwm2m_resource_value_changed(contextP, uriP);
            }
            lwm2m_data_free(1, dataP);
        }
        else
        {
            lwm2m_data_t * dataP;
            int result = COAP_405_METHOD_NOT_ALLOWED;

            dataP = lwm2m_data_new(1);
            if (dataP == NULL)
            {
                LOG("Internal allocation failure !\n");
                return;
            }
            dataP->id = uriP->resourceId;
            lwm2m_data_encode_nstring(value, valueLength, dataP);

            LOG_ARG("lwm2m_data_notify id = %d, type = %d, length = %d, buffer = %s", dataP->id, dataP->type, dataP->value.asBuffer.length, dataP->value.asBuffer.buffer);

            switch (uriP->objectId)
            {
            case LWM2M_CONN_MONITOR_OBJECT_ID:
                result = connectivity_moni_change(dataP, object);
                break;
            default:
                break;
            }

            if (COAP_204_CHANGED != result)
            {
                LOG("Failed to change value!\n");
            }
            else
            {
                LOG("value changed!\n");
                lwm2m_resource_value_changed(contextP, uriP);
            }
            lwm2m_data_free(1, dataP);
        }
    }
    else
    {
        LOG("Object not found !\n");
    }
}

void lwm2m_step_quickly(lwm2m_context_t * contextP)
{
    if (watcher_flag == true) {
        struct timeval tv;

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        lwm2m_observe_step(contextP, lwm2m_gettime(), &(tv.tv_sec));
        watcher_flag = false;
    }
}

void lwm2m_observe_step(lwm2m_context_t * contextP,
                  time_t currentTime,
                  time_t * timeoutP)
{
    lwm2m_observed_t * targetP;

    LOG("lwm2m_observe_step");
    for (targetP = contextP->observedList ; targetP != NULL ; targetP = targetP->next)
    {
        lwm2m_watcher_t * watcherP;
        uint8_t * buffer = NULL;
        size_t length = 0;
        lwm2m_data_t * dataP = NULL;
        int size = 0;
        double floatValue = 0;
        int64_t integerValue = 0;
        bool storeValue = false;
        lwm2m_media_type_t format = LWM2M_CONTENT_TEXT;
        coap_packet_t message[1];
        time_t interval;

        LOG_URI(&(targetP->uri));
        if (LWM2M_URI_IS_SET_RESOURCE(&targetP->uri))
        {
            if (COAP_205_CONTENT != object_readData(contextP, &targetP->uri, &size, &dataP)) {
                lwm2m_data_free(size, dataP);
                continue;
            }
            switch (dataP->type)
            {
            case LWM2M_TYPE_INTEGER:
                if (1 != lwm2m_data_decode_int(dataP, &integerValue)) continue;
                storeValue = true;
                break;
            case LWM2M_TYPE_FLOAT:
                if (1 != lwm2m_data_decode_float(dataP, &floatValue)) continue;
                storeValue = true;
                break;
            default:
                break;
            }
        }
        for (watcherP = targetP->watcherList ; watcherP != NULL ; watcherP = watcherP->next)
        {
            if (watcherP->active == true)
            {
                bool notify = false;

                if (watcherP->update == true)
                {
                    // value changed, should we notify the server ?

                    if (watcherP->parameters == NULL || watcherP->parameters->toSet == 0)
                    {
                        // no conditions
                        notify = true;
                        LOG("Notify with no conditions");
                        LOG_URI(&(targetP->uri));
                    }

                    if (notify == false
                     && watcherP->parameters != NULL
                     && (watcherP->parameters->toSet & ATTR_FLAG_NUMERIC) != 0)
                    {
                        if ((watcherP->parameters->toSet & LWM2M_ATTR_FLAG_LESS_THAN) != 0)
                        {
                            LOG("Checking lower treshold");
                            // Did we cross the lower treshold ?
                            switch (dataP->type)
                            {
                            case LWM2M_TYPE_INTEGER:
                                if ((integerValue <= watcherP->parameters->lessThan
                                  && watcherP->lastValue.asInteger > watcherP->parameters->lessThan)
                                 || (integerValue >= watcherP->parameters->lessThan
                                  && watcherP->lastValue.asInteger < watcherP->parameters->lessThan))
                                {
                                    LOG("Notify on lower treshold crossing");
                                    notify = true;
                                }
                                break;
                            case LWM2M_TYPE_FLOAT:
                                if ((floatValue <= watcherP->parameters->lessThan
                                  && watcherP->lastValue.asFloat > watcherP->parameters->lessThan)
                                 || (floatValue >= watcherP->parameters->lessThan
                                  && watcherP->lastValue.asFloat < watcherP->parameters->lessThan))
                                {
                                    LOG("Notify on lower treshold crossing");
                                    notify = true;
                                }
                                break;
                            default:
                                break;
                            }
                        }
                        if ((watcherP->parameters->toSet & LWM2M_ATTR_FLAG_GREATER_THAN) != 0)
                        {
                            LOG("Checking upper treshold");
                            // Did we cross the upper treshold ?
                            switch (dataP->type)
                            {
                            case LWM2M_TYPE_INTEGER:
                                if ((integerValue <= watcherP->parameters->greaterThan
                                  && watcherP->lastValue.asInteger > watcherP->parameters->greaterThan)
                                 || (integerValue >= watcherP->parameters->greaterThan
                                  && watcherP->lastValue.asInteger < watcherP->parameters->greaterThan))
                                {
                                    LOG("Notify on lower upper crossing");
                                    notify = true;
                                }
                                break;
                            case LWM2M_TYPE_FLOAT:
                                if ((floatValue <= watcherP->parameters->greaterThan
                                  && watcherP->lastValue.asFloat > watcherP->parameters->greaterThan)
                                 || (floatValue >= watcherP->parameters->greaterThan
                                  && watcherP->lastValue.asFloat < watcherP->parameters->greaterThan))
                                {
                                    LOG("Notify on lower upper crossing");
                                    notify = true;
                                }
                                break;
                            default:
                                break;
                            }
                        }
                        if ((watcherP->parameters->toSet & LWM2M_ATTR_FLAG_STEP) != 0)
                        {
                            LOG("Checking step");

                            switch (dataP->type)
                            {
                            case LWM2M_TYPE_INTEGER:
                            {
                                int64_t diff;

                                diff = integerValue - watcherP->lastValue.asInteger;
                                if ((diff < 0 && (0 - diff) >= watcherP->parameters->step)
                                 || (diff >= 0 && diff >= watcherP->parameters->step))
                                {
                                    LOG("Notify on step condition");
                                    notify = true;
                                }
                            }
                                break;
                            case LWM2M_TYPE_FLOAT:
                            {
                                double diff;

                                diff = floatValue - watcherP->lastValue.asFloat;
                                if ((diff < 0 && (0 - diff) >= watcherP->parameters->step)
                                 || (diff >= 0 && diff >= watcherP->parameters->step))
                                {
                                    LOG("Notify on step condition");
                                    notify = true;
                                }
                            }
                                break;
                            default:
                                break;
                            }
                        }
                    }

                    if (watcherP->parameters != NULL
                     && (watcherP->parameters->toSet & LWM2M_ATTR_FLAG_MIN_PERIOD) != 0)
                    {
                        LOG_ARG("Checking minimal period (%d s)", watcherP->parameters->minPeriod);

                        if (watcherP->lastTime + watcherP->parameters->minPeriod > currentTime)
                        {
                            // Minimum Period did not elapse yet
                            interval = watcherP->lastTime + watcherP->parameters->minPeriod - currentTime;
                            if (*timeoutP > interval) *timeoutP = interval;
                            notify = false;
                        }
                        else
                        {
                            LOG("Notify on minimal period");
                            notify = true;
                        }
                    }
                }

                // Is the Maximum Period reached ?
                if (notify == false
                 && watcherP->parameters != NULL
                 && (watcherP->parameters->toSet & LWM2M_ATTR_FLAG_MAX_PERIOD) != 0)
                {
                    LOG_ARG("Checking maximal period (%d s)", watcherP->parameters->minPeriod);

                    if (watcherP->lastTime + watcherP->parameters->maxPeriod <= currentTime)
                    {
                        LOG("Notify on maximal period");
                        notify = true;
                    }
                }

                if (notify == true)
                {
                    if (buffer == NULL)
                    {
                        if (dataP != NULL)
                        {
                            int res;

                            res = lwm2m_data_serialize(&targetP->uri, size, dataP, &format, &buffer);
                            if (res < 0)
                            {
                                break;
                            }
                            else
                            {
                                length = (size_t)res;
                            }

                        }
                        else
                        {
                            if (COAP_205_CONTENT != lwm2m_object_read(contextP, &targetP->uri, &format, &buffer, &length))
                            {
                                buffer = NULL;
                                break;
                            }
                        }
                        if (contextP->notify_with_confirm == 1)
                        {
                            coap_init_message(message, COAP_TYPE_CON, COAP_205_CONTENT, 0);
                        }
                        else
                        {
                            coap_init_message(message, COAP_TYPE_NON, COAP_205_CONTENT, 0);
                        }
                        coap_set_header_content_type(message, format);
                        coap_set_payload(message, buffer, length);
                    }
                    watcherP->lastTime = currentTime;
                    contextP->nextMID++;//avoid to use the same message ID
                    watcherP->lastMid = contextP->nextMID++;
                    message->mid = watcherP->lastMid;
                    coap_set_header_token(message, watcherP->token, watcherP->tokenLen);
                    coap_set_header_observe(message, watcherP->counter++);
                    coap_status_t status = message_send(contextP, message, watcherP->server->sessionH);
                    //update releated data
                    lwm2m_observe_update_data(contextP, &(targetP->uri), watcherP);
                    watcherP->update = false;
                    if (targetP->uri.objectId == LWM2M_WATERMETER_OBJECT_ID && targetP->uri.instanceId == 0 && targetP->uri.resourceId == 0)
                    {
                        if (LWM2M_URI_IS_SET_RESOURCE(&(targetP->uri)))
                        {
                            if (contextP->notify_callback != NULL) {
                                contextP->notify_callback(LWM2M_NOTIFY_TYPE_SEND, (status == COAP_NO_ERROR) ? LWM2M_NOTIFY_CODE_SUCCESS : LWM2M_NOTIFY_CODE_FAILED, message->mid);
                            }
                            if (status == COAP_NO_ERROR)
                            {
#ifdef MTK_LWM2M_CT_SUPPORT
                                extern void ctiot_lwm2m_client_notify_success(void);
                                ctiot_lwm2m_client_notify_success();
#endif
                                if (contextP->notify_with_confirm == 1) lwm2m_observe_addMessage(contextP, message->mid);
                            }
                            else
                            {
#ifdef MTK_LWM2M_CT_SUPPORT
                                extern void ctiot_lwm2m_client_notify_failed(void);
                                ctiot_lwm2m_client_notify_failed();
#endif
                                if (contextP->notify_callback != NULL) contextP->notify_callback(LWM2M_NOTIFY_TYPE_SEND_CONFIRM, LWM2M_NOTIFY_CODE_PACKET_NOT_SENT, message->mid);
                            }
                        }
                    }
#ifdef MTK_TMO_CERT_SUPPORT
                    else
                    {
                        if (contextP->notify_callback != NULL) contextP->notify_callback(LWM2M_NOTIFY_TYPE_SEND, LWM2M_NOTIFY_CODE_SUCCESS, message->mid);
                        if (status == COAP_NO_ERROR)
                        {
                            if (contextP->notify_with_confirm == 1) lwm2m_observe_addMessage(contextP, message->mid);
                        }
                        else
                        {
                            if (contextP->notify_callback != NULL) contextP->notify_callback(LWM2M_NOTIFY_TYPE_SEND_CONFIRM, LWM2M_NOTIFY_CODE_PACKET_NOT_SENT, message->mid);
                        }
                    }
#endif /* MTK_TMO_CERT_SUPPORT */
                }

                // Store this value
                if (notify == true && storeValue == true)
                {
                    switch (dataP->type)
                    {
                    case LWM2M_TYPE_INTEGER:
                        watcherP->lastValue.asInteger = integerValue;
                        break;
                    case LWM2M_TYPE_FLOAT:
                        watcherP->lastValue.asFloat = floatValue;
                        break;
                    default:
                        break;
                    }
                }

                if (watcherP->parameters != NULL && (watcherP->parameters->toSet & LWM2M_ATTR_FLAG_MAX_PERIOD) != 0)
                {
                    // update timers
                    interval = watcherP->lastTime + watcherP->parameters->maxPeriod - currentTime;
                    if (*timeoutP > interval) *timeoutP = interval;
                }
            }
        }
        if (dataP != NULL) lwm2m_data_free(size, dataP);
        if (buffer != NULL) lwm2m_free(buffer);
    }
}

#endif

#ifdef LWM2M_SERVER_MODE

typedef struct
{
    lwm2m_observation_t * observationP;
    lwm2m_result_callback_t callbackP;
    void * userDataP;
} cancellation_data_t;

static lwm2m_observation_t * prv_findObservationByURI(lwm2m_client_t * clientP,
                                                      lwm2m_uri_t * uriP)
{
    lwm2m_observation_t * targetP;

    targetP = clientP->observationList;
    while (targetP != NULL)
    {
        if (targetP->uri.objectId == uriP->objectId
         && targetP->uri.flag == uriP->flag
         && targetP->uri.instanceId == uriP->instanceId
         && targetP->uri.resourceId == uriP->resourceId)
        {
            return targetP;
        }

        targetP = targetP->next;
    }

    return targetP;
}

void observe_remove(lwm2m_observation_t * observationP)
{
    LOG("Entering");
    observationP->clientP->observationList = (lwm2m_observation_t *) LWM2M_LIST_RM(observationP->clientP->observationList, observationP->id, NULL);
    lwm2m_free(observationP);
}

static void prv_obsRequestCallback(lwm2m_transaction_t * transacP,
                                   void * message)
{
    lwm2m_observation_t * observationP = (lwm2m_observation_t *)transacP->userData;
    coap_packet_t * packet = (coap_packet_t *)message;
    uint8_t code;

    switch (observationP->status)
    {
    case STATE_DEREG_PENDING:
        // Observation was canceled by the user.
        observe_remove(observationP);
        return;

    case STATE_REG_PENDING:
        observationP->status = STATE_REGISTERED;
        break;

    default:
        break;
    }

    if (message == NULL)
    {
        code = COAP_503_SERVICE_UNAVAILABLE;
    }
    else if (packet->code == COAP_205_CONTENT
         && !IS_OPTION(packet, COAP_OPTION_OBSERVE))
    {
        code = COAP_405_METHOD_NOT_ALLOWED;
    }
    else
    {
        code = packet->code;
    }

    if (code != COAP_205_CONTENT)
    {
        observationP->callback(observationP->clientP->internalID,
                               &observationP->uri,
                               code,
                               LWM2M_CONTENT_TEXT, NULL, 0,
                               observationP->userData);
        observe_remove(observationP);
    }
    else
    {
        observationP->callback(observationP->clientP->internalID,
                               &observationP->uri,
                               0,
                               packet->content_type, packet->payload, packet->payload_len,
                               observationP->userData);
    }
}


static void prv_obsCancelRequestCallback(lwm2m_transaction_t * transacP,
                                         void * message)
{
    cancellation_data_t * cancelP = (cancellation_data_t *)transacP->userData;
    coap_packet_t * packet = (coap_packet_t *)message;
    uint8_t code;

    if (message == NULL)
    {
        code = COAP_503_SERVICE_UNAVAILABLE;
    }
    else
    {
        code = packet->code;
    }

    if (code != COAP_205_CONTENT)
    {
        cancelP->callbackP(cancelP->observationP->clientP->internalID,
                           &cancelP->observationP->uri,
                           code,
                           LWM2M_CONTENT_TEXT, NULL, 0,
                           cancelP->userDataP);
    }
    else
    {
        cancelP->callbackP(cancelP->observationP->clientP->internalID,
                           &cancelP->observationP->uri,
                           0,
                           packet->content_type, packet->payload, packet->payload_len,
                           cancelP->userDataP);
    }

    observe_remove(cancelP->observationP);

    lwm2m_free(cancelP);
}


int lwm2m_observe(lwm2m_context_t * contextP,
                  uint16_t clientID,
                  lwm2m_uri_t * uriP,
                  lwm2m_result_callback_t callback,
                  void * userData)
{
    lwm2m_client_t * clientP;
    lwm2m_transaction_t * transactionP;
    lwm2m_observation_t * observationP;
    uint8_t token[4];

    LOG_ARG("clientID: %d", clientID);
    LOG_URI(uriP);

    if (!LWM2M_URI_IS_SET_INSTANCE(uriP) && LWM2M_URI_IS_SET_RESOURCE(uriP)) return COAP_400_BAD_REQUEST;

    clientP = (lwm2m_client_t *)lwm2m_list_find((lwm2m_list_t *)contextP->clientList, clientID);
    if (clientP == NULL) return COAP_404_NOT_FOUND;

    observationP = (lwm2m_observation_t *)lwm2m_malloc(sizeof(lwm2m_observation_t));
    if (observationP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
    memset(observationP, 0, sizeof(lwm2m_observation_t));

    observationP->id = lwm2m_list_newId((lwm2m_list_t *)clientP->observationList);
    memcpy(&observationP->uri, uriP, sizeof(lwm2m_uri_t));
    observationP->clientP = clientP;
    observationP->status = STATE_REG_PENDING;
    observationP->callback = callback;
    observationP->userData = userData;

    token[0] = clientP->internalID >> 8;
    token[1] = clientP->internalID & 0xFF;
    token[2] = observationP->id >> 8;
    token[3] = observationP->id & 0xFF;

    transactionP = lwm2m_transaction_new(clientP->sessionH, COAP_GET, clientP->altPath, uriP, contextP->nextMID++, 4, token);
    if (transactionP == NULL)
    {
        lwm2m_free(observationP);
        return COAP_500_INTERNAL_SERVER_ERROR;
    }

    observationP->clientP->observationList = (lwm2m_observation_t *)LWM2M_LIST_ADD(observationP->clientP->observationList, observationP);

    coap_set_header_observe(transactionP->message, 0);
    coap_set_header_token(transactionP->message, token, sizeof(token));

    transactionP->callback = prv_obsRequestCallback;
    transactionP->userData = (void *)observationP;

    contextP->transactionList = (lwm2m_transaction_t *)LWM2M_LIST_ADD(contextP->transactionList, transactionP);

    return lwm2m_transaction_send(contextP, transactionP);
}

int lwm2m_observe_cancel(lwm2m_context_t * contextP,
                         uint16_t clientID,
                         lwm2m_uri_t * uriP,
                         lwm2m_result_callback_t callback,
                         void * userData)
{
    lwm2m_client_t * clientP;
    lwm2m_observation_t * observationP;

    LOG_ARG("clientID: %d", clientID);
    LOG_URI(uriP);

    clientP = (lwm2m_client_t *)lwm2m_list_find((lwm2m_list_t *)contextP->clientList, clientID);
    if (clientP == NULL) return COAP_404_NOT_FOUND;

    observationP = prv_findObservationByURI(clientP, uriP);
    if (observationP == NULL) return COAP_404_NOT_FOUND;

    switch (observationP->status)
    {
    case STATE_REGISTERED:
    {
        lwm2m_transaction_t * transactionP;
        cancellation_data_t * cancelP;

        transactionP = lwm2m_transaction_new(clientP->sessionH, COAP_GET, clientP->altPath, uriP, contextP->nextMID++, 0, NULL);
        if (transactionP == NULL)
        {
            return COAP_500_INTERNAL_SERVER_ERROR;
        }
        cancelP = (cancellation_data_t *)lwm2m_malloc(sizeof(cancellation_data_t));
        if (cancelP == NULL)
        {
            lwm2m_free(transactionP);
            return COAP_500_INTERNAL_SERVER_ERROR;
        }

        coap_set_header_observe(transactionP->message, 1);

        cancelP->observationP = observationP;
        cancelP->callbackP = callback;
        cancelP->userDataP = userData;

        transactionP->callback = prv_obsCancelRequestCallback;
        transactionP->userData = (void *)cancelP;

        contextP->transactionList = (lwm2m_transaction_t *)LWM2M_LIST_ADD(contextP->transactionList, transactionP);

        return lwm2m_transaction_send(contextP, transactionP);
    }

    case STATE_REG_PENDING:
        observationP->status = STATE_DEREG_PENDING;
        break;

    default:
        // Should not happen
        break;
    }

    return COAP_NO_ERROR;
}

bool observe_handleNotify(lwm2m_context_t * contextP,
                           void * fromSessionH,
                           coap_packet_t * message,
        				   coap_packet_t * response)
{
    uint8_t * tokenP;
    int token_len;
    uint16_t clientID;
    uint16_t obsID;
    lwm2m_client_t * clientP;
    lwm2m_observation_t * observationP;
    uint32_t count;

    LOG("Entering");
    token_len = coap_get_header_token(message, (const uint8_t **)&tokenP);
    if (token_len != sizeof(uint32_t)) return false;

    if (1 != coap_get_header_observe(message, &count)) return false;

    clientID = (tokenP[0] << 8) | tokenP[1];
    obsID = (tokenP[2] << 8) | tokenP[3];

    clientP = (lwm2m_client_t *)lwm2m_list_find((lwm2m_list_t *)contextP->clientList, clientID);
    if (clientP == NULL) return false;

    observationP = (lwm2m_observation_t *)lwm2m_list_find((lwm2m_list_t *)clientP->observationList, obsID);
    if (observationP == NULL)
    {
        coap_init_message(response, COAP_TYPE_RST, 0, message->mid);
        message_send(contextP, response, fromSessionH);
    }
    else
    {
        if (message->type == COAP_TYPE_CON ) {
            coap_init_message(response, COAP_TYPE_ACK, 0, message->mid);
            message_send(contextP, response, fromSessionH);
        }
        observationP->callback(clientID,
                               &observationP->uri,
                               (int)count,
                               message->content_type, message->payload, message->payload_len,
                               observationP->userData);
    }
    return true;
}
#endif

