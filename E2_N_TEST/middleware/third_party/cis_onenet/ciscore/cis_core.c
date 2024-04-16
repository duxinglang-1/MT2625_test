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
 *    Baijie & Longrong, China Mobile - Please refer to git log
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
#include "cis_internals.h"
#include "cis_api.h"
#include "cis_if_sys.h"
#include "cis_if_net.h"
#include "std_object/std_object.h"
#include "cis_log.h"
#include "cis_config.h"
#include "ril.h"
#include "ril_internal_use.h"
#include "ril_cmds_def.h"

#if CIS_ENABLE_DM

#include "dm_endpoint.h"
#endif

#if CIS_ENABLE_MEMORYTRACE
static cis_time_t g_tracetime = 0;
#endif//CIS_ENABLE_MEMORY_TRACE

#include "FreeRTOS.h"
#include "task.h"

//private function.
static void     prv_localDeinit(st_context_t** context);
static cis_data_t* prv_dataDup(const cis_data_t* src);
//static st_object_t* prv_findObject(st_context_t* context,cis_oid_t objectid);
static void     prv_deleteTransactionList(st_context_t* context);
static void     prv_deleteMessageList(st_context_t *context);
static void     prv_deleteObservedList(st_context_t* context);
static void     prv_deleteRequestList(st_context_t* context);
static void     prv_deleteNotifyList(st_context_t* context);
static void     prv_deleteObjectList(st_context_t* context);
static void     prv_deleteServer(st_context_t* context);

static cis_ret_t prv_onNetEventHandler(cisnet_t netctx,cisnet_event_t id,void* param,void* context);
static int       prv_makeDeviceName(char** name);
cis_ret_t prv_onSySEventHandler(cissys_event_t id,void* param,void* userData,int *len);

#if CIS_ENABLE_NOTIFY_FROM_BS
static int32_t prv_ril_urc_notify_callback(ril_urc_id_t event_id, void *param, uint32_t param_len);
#endif

cis_ret_t    cis_uri_make         (cis_oid_t oid,cis_iid_t iid,cis_rid_t rid, cis_uri_t* uri)
{
    if (uri == NULL) 
    {
        return CIS_RET_ERROR;
    }

    cis_memset(uri, 0, sizeof(st_uri_t));
    uri->objectId = 0;
    uri->instanceId = 0;
    uri->resourceId = 0;

    if (oid > 0 && oid <= URI_MAX_ID) 
    {
        uri->objectId = (cis_oid_t)oid;
        uri->flag |= URI_FLAG_OBJECT_ID;
    }
    
    if (iid >= 0 && iid <= URI_MAX_ID) 
    {
        uri->instanceId = (cis_iid_t)iid;
        uri->flag |= URI_FLAG_INSTANCE_ID;
    }

    if (rid > 0 && rid <= URI_MAX_ID) 
    {
        uri->resourceId = (cis_rid_t)rid;
        uri->flag |= URI_FLAG_RESOURCE_ID;
    }

    return CIS_RET_OK;
}

cis_ret_t    cis_uri_update       (cis_uri_t* uri)
{
    return cis_uri_make(uri->objectId,uri->instanceId,uri->resourceId,uri);
}



cis_ret_t    cis_version         (cis_version_t* version)
{
    if(version == NULL)
    {
        return CIS_RET_PARAMETER_ERR;
    }
    version->major = CIS_VERSION_MAJOR;
    version->minor = CIS_VERSION_MINOR;
    version->micro = CIS_VERSION_MICRO;
#if CIS_ENABLE_DM
    version->dm_major = DM_VERSION_MAJOR;
    version->dm_minor = DM_VERSION_MINOR;
    version->dm_micro = DM_VERSION_MICRO;
#endif
    return CIS_RET_OK;
}
#if CIS_ENABLE_DM
char * cis_get_version(char *ver)
{
    char i=0,j=15;
    snprintf(ver+(i++),j--,"%c",'V');
	snprintf(ver+(i++),j--,"%d",CIS_VERSION_MAJOR);
	snprintf(ver+(i++),j--,"%c",'.');
    snprintf(ver+(i++),j--,"%d",CIS_VERSION_MINOR);
	snprintf(ver+(i++),j--,"%c",'.');
	snprintf(ver+(i++),j--,"%d",CIS_VERSION_MICRO);
	snprintf(ver+(i++),j--,"%c",'_');
	snprintf(ver+(i++),j--,"%c",'D');
	snprintf(ver+(i++),j--,"%d",DM_VERSION_MAJOR);
	snprintf(ver+(i++),j--,"%c",'.');
    snprintf(ver+(i++),j--,"%d",DM_VERSION_MINOR);
	snprintf(ver+(i++),j--,"%c",'.');
	snprintf(ver+(i++),j--,"%d",DM_VERSION_MICRO);
	snprintf(ver+(i++),j--,"%c",'\0');
	return ver;
}        
#endif

void *g_lockImei;
void *g_lockImsi;

#if CIS_ENABLE_NOTIFY_FROM_BS
static st_context_t* g_onenet_ctx;
#endif

cis_ret_t    cis_init(void** context,void* configPtr,uint16_t configLen,void *DMconf,void *auth_code)
{
    cis_cfgret_t configRet;
    cisnet_callback_t netCallback;
	cissys_callback_t sys_callback;
    st_context_t * contextP;
    char* targetServerHost;
    char* deviceName=NULL;
    cisnet_config_t netConfig;
    st_serialize_t serialize;
    void*configContext = NULL;

    if(*context != NULL)
    {
        return CIS_RET_EXIST;
    }

    LOGD("api cis_init");
    LOGI("version:%d.%d.%d",CIS_VERSION_MAJOR,CIS_VERSION_MINOR,CIS_VERSION_MICRO);

    contextP = (st_context_t*)cis_malloc(sizeof(st_context_t));
    if (NULL == contextP)
    {
        return CIS_RET_ERROR;
    }
    cis_memset(contextP, 0, sizeof(st_context_t));
    contextP->nextMid = (uint16_t)cissys_rand();
    (*context) = contextP;
    
#if CIS_ENABLE_AUTH
 if(auth_code!=NULL&& contextP->isDM!=TRUE){
	contextP->authCode=(char *)cis_malloc(strlen((const char *)auth_code)+1);
	if(contextP->authCode==NULL)
	{
	    cis_free(contextP);
        return CIS_RET_ERROR;
	  
	}
    cis_memcpy(contextP->authCode,auth_code,strlen((const char *)auth_code)+1);
 }
#endif
    
    //deal with configuration data 
    if(cis_config_init(&configContext,configPtr, configLen) < 0)
    {
        cis_free(contextP);
        return CIS_RET_ERROR;
    }

    if (g_lockImei == NULL) cissys_lockcreate(&g_lockImei);
    if (g_lockImsi == NULL) cissys_lockcreate(&g_lockImsi);

    cissys_request_endpoint_name();

    cissys_lock(g_lockImei, CIS_CONFIG_LOCK_INFINITY);
    cissys_unlock(g_lockImei);
    cissys_lock(g_lockImsi, CIS_CONFIG_LOCK_INFINITY);
    cissys_unlock(g_lockImsi);
    
    LOGD("----------------\n");
    LOGD("DEBUG CONFIG INIT INFORMATION.");

#if CIS_ENABLE_DM
  if(NULL!=DMconf)
  {
        dmSdkInit(DMconf);
		printf("For Dm\n");
		int ret=genDmRegEndpointName(&deviceName,DMconf);
		if(ret<0)
		{
		    LOGE("ERROR:Get device name error from IMEI/IMSI.");
              cis_free(contextP);
             if(deviceName != NULL){
                cis_free(deviceName);
               }
           return CIS_RET_ERROR;
		}
		char* dmUpdeviceName=NULL;
		ret=genDmUpdateEndpointName(&dmUpdeviceName,DMconf);
	    if(ret<0)
		{
		    LOGE("ERROR:Get device name error from IMEI/IMSI.");
            cis_free(contextP);
            if(deviceName != NULL){
              cis_free(dmUpdeviceName);
             }
             return CIS_RET_ERROR;
		}
	     contextP->isDM =TRUE; 
		 contextP->DMprivData = (char*)dmUpdeviceName;
	    LOGD("INIT:endpoint name: %s", contextP->endpointName);
	    LOGD("INIT:endpointInfo: %s",  contextP->DMprivData);

  }else{

     if(prv_makeDeviceName(&deviceName) <= 0){
	  LOGE("ERROR:Get device name error from IMEI/IMSI.");
	  cis_free(contextP);
	  if(deviceName != NULL){
		  cis_free(deviceName);
	  }
	  return CIS_RET_ERROR;
  }
}
#else
    if(prv_makeDeviceName(&deviceName) <= 0){
        LOGE("ERROR:Get device name error from IMEI/IMSI.");
        cis_free(contextP);
        if(deviceName != NULL){
            cis_free(deviceName);
        }
        return CIS_RET_ERROR;
    }
#endif
#if CIS_ENABLE_UPDATE
	contextP->created = false;
    sys_callback.onEvent = prv_onSySEventHandler;
	sys_callback.userData = contextP;
#endif	


    cissys_lockcreate(&contextP->lockRequest);
    cissys_lockcreate(&contextP->lockNotify);
    cissys_lockcreate(&contextP->lockSocket);
    
    contextP->nextObserveNum = 0;
    contextP->endpointName = (char*)deviceName;
    LOGD("INIT:endpoint name: %s", contextP->endpointName);

    /*Get sys parameter and init*/
//	sys_callback.onEvent = prv_onSySEventHandler;
//	sys_callback.userData = contextP;
    cis_config_get(configContext,cis_cfgid_sys, &configRet);
    cissys_init(contextP,configRet.data.cfg_sys,&sys_callback);
    log_config(configRet.data.cfg_sys->log_enabled,
        configRet.data.cfg_sys->log_ext_output,
        configRet.data.cfg_sys->log_output_level,
        configRet.data.cfg_sys->log_buffer_size);

    /*Get net parameter and create net*/
    netCallback.onEvent = prv_onNetEventHandler;
    cis_config_get(configContext,cis_cfgid_net,&configRet);
    
    netConfig.mtu = configRet.data.cfg_net->mtu;
    netConfig.data = configRet.data.cfg_net->user_data.data;
    netConfig.datalen = configRet.data.cfg_net->user_data.len;
    netConfig.bsEanbled = configRet.data.cfg_net->bs_enabled;

    cisnet_init(contextP,&netConfig,netCallback);//yyl 18/5/31
    
    //set bootstrap flag to context;
    contextP->bootstrapEanbled = netConfig.bsEanbled != 0 ? true : false;

    targetServerHost =  (char*)configRet.data.cfg_net->host.data;
    if(targetServerHost == NULL || strlen(targetServerHost) <= 0){
        cis_free(contextP);
        cis_free(deviceName);
        return CIS_RET_ERROR;
    }

    cis_config_destory(&configContext);


    cis_addobject(contextP,std_object_security,NULL,NULL);
    st_object_t* securityObj = prv_findObject(contextP,std_object_security);
    if(securityObj == NULL)
    {
        LOGE("ERROR:Failed to init security object");
        cis_free(contextP);
        cis_free(deviceName);
        return CIS_RET_ERROR;
    }
    
    //load serialize from firmware memory
    cissys_load((uint8_t*)&serialize,sizeof(serialize));
    
    if(contextP->bootstrapEanbled)
    {
        std_security_create(contextP,0,"",false,securityObj);
        std_security_create(contextP,1,targetServerHost,true,securityObj); 
    }
    else
    {
        std_security_create(contextP,0,targetServerHost,false,securityObj);
    }

    contextP->stateStep = PUMP_STATE_INITIAL;
#if CIS_ENABLE_NOTIFY_FROM_BS
    if (!contextP->isDM) {
        ril_register_event_callback(RIL_ALL, prv_ril_urc_notify_callback);
        g_onenet_ctx = contextP;
    }
#endif /* CIS_ENABLE_NOTIFY_FROM_BS */
    return CIS_RET_OK;
}

cis_ret_t    cis_deinit(void** context)
{
    st_context_t* ctx = *(st_context_t**)context;
    cissys_assert(context!=NULL);
    if(context == NULL)
    {
        return CIS_RET_PARAMETER_ERR;
    }

    if(ctx->registerEnabled)
    {
        return CIS_RET_PENDING;
    }

    LOGD("api cis_deinit");

#if CIS_ENABLE_NOTIFY_FROM_BS
    if (!ctx->isDM) {
        g_onenet_ctx = NULL;
        ril_indicate_packet_to_track_req_t req_param;
        req_param.mode = 0;
        req_param.pattern_id = ctx->pattern_id;
        ril_request_indicate_packet_to_track(RIL_EXECUTE_MODE, &req_param, NULL, NULL);
    }
#endif /* CIS_ENABLE_NOTIFY_FROM_BS */

    prv_localDeinit((st_context_t**)context);

    if (g_lockImei != NULL) cissys_lockdestory(g_lockImei);
    g_lockImei = NULL;
    if (g_lockImsi != NULL) cissys_lockdestory(g_lockImsi);
    g_lockImsi = NULL;

    return CIS_RET_OK;
}

cis_ret_t    cis_register(void* context,cis_time_t lifetime,const cis_callback_t* cb)
{
    st_context_t* ctx = (st_context_t*)context;
    cissys_assert(context!=NULL);
    if(context == NULL)
    {
        return CIS_RET_PARAMETER_ERR;
    }
    LOGD("api cis_register");

    if(cb->onEvent == NULL || cb->onExec == NULL  || cb->onObserve == NULL || 
        cb->onRead == NULL || cb->onSetParams == NULL || cb->onWrite == NULL ||
        cb->onDiscover == NULL)
    {
        LOGE("ERROR:cis_register request failed.invalid parameters");
        return CIS_RET_ERROR;
    }

    if(lifetime < LIFETIME_LIMIT_MIN ||
        lifetime > LIFETIME_LIMIT_MAX)
    {
        LOGE("ERROR:invalid lifetime parameter");
        return CIS_RET_ERROR;
    }
    ctx->lifetime = lifetime;
    ctx->registerEnabled = true;
    cis_memcpy(&ctx->callback,cb,sizeof(cis_callback_t));
    return CIS_RET_OK;
}

cis_ret_t    cis_unregister(void* context)
{
    st_context_t* ctx = (st_context_t*)context;
    cissys_assert(context!=NULL);
    if(context == NULL)
    {
        return CIS_RET_PARAMETER_ERR;
    }
    LOGD("api cis_unregister");
	core_updatePumpState(ctx,PUMP_STATE_UNREGISTER);
    return CIS_RET_OK;
}



cis_ret_t    cis_addobject        (void* context,cis_oid_t objectid,const cis_inst_bitmap_t* bitmap,const cis_res_count_t* resource)
{
    st_object_t * targetP;
    uint16_t index;
    cis_instcount_t instValidCount;
    cis_instcount_t instCount;
    cis_instcount_t instBytes;
    uint8_t* instPtr;
    bool noneBitmap;

    st_context_t* ctx = (st_context_t*)context;
    cissys_assert(context!=NULL);
    if(context == NULL)
    {
        return CIS_RET_PARAMETER_ERR;
    }

    LOGD("api cis_addobject");

    if(bitmap == NULL || bitmap->instanceBitmap == NULL){
        instCount = 1;
        instBytes = 1;
        noneBitmap = true;
    }else{
        noneBitmap = false;
        instCount = bitmap->instanceCount;
        instBytes = bitmap->instanceBytes;

        if(instBytes <= 0 || instCount <= 0)
            return CIS_RET_ERROR;
        if(instBytes != ((instCount - 1) / 8) + 1)
            return CIS_RET_ERROR;

    }
    

    targetP = (st_object_t *)CIS_LIST_FIND(ctx->objectList,objectid);
    if (targetP != NULL)
    {
         return CIS_RET_EXIST;
    }

    targetP = (st_object_t *)cis_malloc(sizeof(st_object_t));
    if(targetP == NULL)
    {
         return CIS_RET_MEMORY_ERR;
    }
	//targetP = (st_object_t *)cis_malloc(sizeof(st_object_t));
    cis_memset(targetP,0,sizeof(st_object_t));
    targetP->objID = objectid;
    
    instPtr = (uint8_t*)cis_malloc(instBytes);
    if(instPtr == NULL){
        cis_free(targetP);
        return CIS_RET_MEMORY_ERR;
    }
    cis_memset(instPtr,0,instBytes);

    if(noneBitmap)
    {
        instPtr[0] = 0x80;
    }
    else
    {
        cis_memcpy(instPtr,bitmap->instanceBitmap,instBytes);
    }
    
    instValidCount = 0;
    for (index=0;index < instCount;index++)
    {
        if(object_checkInstExisted(instPtr,index))instValidCount++;
    }
    
    if(instValidCount == 0){
        LOGD("Error:instance bitmap invalid.");
        cis_free(targetP);
        cis_free(instPtr);
        return CIS_RET_ERROR;
    }

    targetP->instBitmapBytes = instBytes;
    targetP->instBitmapPtr = instPtr;
    targetP->instBitmapCount = instCount;
    if(resource != NULL)
    {
        targetP->attributeCount = resource->attrCount;
        targetP->actionCount = resource->actCount;
    }
    targetP->instValidCount = instValidCount;
    
    ctx->objectList = (st_object_t *)CIS_LIST_ADD(ctx->objectList, targetP);

    return CIS_RET_NO_ERROR;

}


cis_ret_t    cis_delobject        (void* context,cis_oid_t objectid)
{
    st_object_t * targetP;
    st_context_t* ctx = (st_context_t*)context;
    cissys_assert(context!=NULL);
    if(context == NULL)
    {
        return CIS_RET_PARAMETER_ERR;
    }
    LOGD("api cis_delobject");
    ctx->objectList = (st_object_t *)CIS_LIST_RM(ctx->objectList, objectid, &targetP);

    if (targetP == NULL) return COAP_404_NOT_FOUND;
    if(targetP->instBitmapPtr != NULL)
    {
        cis_free(targetP->instBitmapPtr);
    }
    cis_free(targetP);

    return CIS_RET_NO_ERROR;

}

uint32_t    cis_pump(void* context)
{
    uint32_t notifyCount;
    uint32_t requestCount;
    bool readHasData;
    cis_time_t tv_sec;
    st_context_t* ctx = (st_context_t*)context;
    cissys_assert(context!=NULL);
    
    tv_sec = utils_gettime_s(); 
    if (tv_sec <= 0)
    {
        return PUMP_RET_CUSTOM;
    }

#if CIS_ENABLE_MEMORYTRACE
    if(tv_sec - g_tracetime > CIS_CONFIG_MEMORYTRACE_TIMEOUT)
    {
        g_tracetime = tv_sec;
        int block,i;
        size_t size;
        trace_status(&block,&size);
        for(i=0;i<block;i++)
        {
            trace_print(i,1);
        }
    }
#endif//CIS_ENABLE_MEMORY_TRACE


    if(!ctx->registerEnabled)
    {
        return PUMP_RET_CUSTOM;
    }

	if(!cisnet_attached_state(ctx))
    {
        return PUMP_RET_CUSTOM;
    }

    switch (ctx->stateStep)
    {
    case PUMP_STATE_HALT:
        {
            core_callbackEvent(ctx,CIS_EVENT_STATUS_HALT,NULL);
            prv_localDeinit(&ctx);
            cissys_fault(0);
            core_updatePumpState(ctx,PUMP_STATE_INITIAL);
            return PUMP_RET_CUSTOM;
        }
        break;
    case PUMP_STATE_INITIAL:
        {
            core_updatePumpState(ctx,PUMP_STATE_BOOTSTRAPPING);
            return PUMP_RET_NOSLEEP;
        }
        break;
    case PUMP_STATE_BOOTSTRAPPING:
        {

            if(!ctx->bootstrapEanbled)
            {
                LOGD("CIS_ENABLE_BOOTSTRAP is disabled");
                core_updatePumpState(ctx,PUMP_STATE_CONNECTING);
                return PUMP_RET_NOSLEEP;

            }

            if(ctx->bootstrapServer == NULL)
            {
                core_callbackEvent(ctx,CIS_EVENT_BOOTSTRAP_START,NULL);
                bootstrap_init(ctx);
                if(ctx->bootstrapServer == NULL)
                {
                    return PUMP_RET_CUSTOM;
                }
            }

            switch (bootstrap_getStatus(ctx))
            {
            case STATE_UNCREATED:
                {
                    ctx->lasttime = tv_sec;
                    bootstrap_create(ctx);
                    return PUMP_RET_NOSLEEP;
                }
                break;
            case STATE_CREATED:
                {
                    ctx->lasttime = tv_sec;
                    bootstrap_connect(ctx);
                    return PUMP_RET_NOSLEEP;
                }
                break;
            case STATE_CONNECT_PENDING:
                {
                    if(ctx->lasttime == 0 || tv_sec - ctx->lasttime > CIS_CONFIG_CONNECT_RETRY_TIME)
                    {
                        ctx->lasttime = tv_sec;
                        bootstrap_connect(ctx);
                    }
                    return PUMP_RET_NOSLEEP;
                }
                break;
            case STATE_BS_INITIATED:
                {
                    //wait;
                }
                break;
            case STATE_BS_PENDING:
                {
                    if(ctx->lasttime == 0 || tv_sec - ctx->lasttime > CIS_CONFIG_BOOTSTRAP_TIMEOUT)
                    {
                        ctx->lasttime = tv_sec;
                        LOGD("bootstrap pending timeout.");
                        if(ctx->bootstrapServer != NULL)
                        {
                            ctx->bootstrapServer->status = STATE_BS_FAILED;
                        }
                    }
                }
                break;
            case STATE_BS_FINISHED:
                {
                    LOGI("Bootstrap finish.");
                    bootstrap_destory(ctx);
                    core_updatePumpState(ctx,PUMP_STATE_CONNECTING);
                    core_callbackEvent(ctx,CIS_EVENT_BOOTSTRAP_SUCCESS,NULL);
                    return PUMP_RET_NOSLEEP;
                }
                break;
            case STATE_BS_FAILED:
                {
                    LOGE("Bootstrap failed.");
                    bootstrap_destory(ctx);
                    core_updatePumpState(ctx,PUMP_STATE_INITIAL);
                    core_callbackEvent(ctx,CIS_EVENT_BOOTSTRAP_FAILED,NULL);
                    return PUMP_RET_CUSTOM;
                }
                break;
            default:
                {
                    bootstrap_step(ctx, tv_sec);
                }
                break;
            }
        }
        break;
    case PUMP_STATE_CONNECTING:
        {
            if(ctx->server == NULL)
            {
                ctx->server = management_makeServerList(ctx,false);
                if(ctx->server == NULL)
                {
                    LOGE("ERROR:makeServer failed.");
                    core_updatePumpState(ctx,PUMP_STATE_INITIAL);
                    return PUMP_RET_CUSTOM;
                }

                if (ctx->bootstrapEanbled && ctx->ignoreRegistration)
                {
                    if (ctx->server->host != NULL) cis_free(ctx->server->host);
                    ctx->server->host = (char *)cis_malloc(20);
                    if (ctx->server->host != NULL)
                    {
                        extern char cis_server_host[];
                        cis_memset(ctx->server->host, 0, 20);
                        if (cis_server_host[0] != '\0')
                        {
                            cis_memcpy(ctx->server->host, cis_server_host, 20);
                            LOGI("cis_server_host = %s", cis_server_host);
                        }
                        else
                        {
                            cis_memcpy(ctx->server->host, NBCFG_DEFAULT_BOOTSTRAP, 20);
                            LOGE("ERROR:cis_server_host.");
                        }
                    }
                }
            }

            switch(ctx->server->status)
            {
            case STATE_UNCREATED:
                {
                    if(ctx->server == NULL)
                    {
                        core_updatePumpState(ctx,PUMP_STATE_INITIAL);
                        return PUMP_RET_CUSTOM;
                    }

                    ctx->lasttime = tv_sec;
                    if(!management_createNetwork(ctx,ctx->server))
                    {
                        core_updatePumpState(ctx,PUMP_STATE_INITIAL);
                        return PUMP_RET_CUSTOM;
                    }
                    return PUMP_RET_NOSLEEP;
                }
                break;

            case STATE_CREATED:
                {
                    if(ctx->server == NULL || ctx->server->sessionH == NULL)
                    {
                        core_updatePumpState(ctx,PUMP_STATE_INITIAL);
                        return PUMP_RET_CUSTOM;
                    }
                    ctx->lasttime = tv_sec;

                    if(!management_connectServer(ctx,ctx->server))
                    {
                        core_updatePumpState(ctx,PUMP_STATE_INITIAL);
                        return PUMP_RET_CUSTOM;
                    }
                    return PUMP_RET_NOSLEEP;
                }
                break;
            case STATE_CONNECT_PENDING:
                {
                    if(ctx->server == NULL || ctx->server->sessionH == NULL)
                    {
                        core_updatePumpState(ctx,PUMP_STATE_INITIAL);
                        return PUMP_RET_CUSTOM;
                    }

                    if(ctx->lasttime == 0 || tv_sec - ctx->lasttime > CIS_CONFIG_CONNECT_RETRY_TIME)
                    {
                        ctx->lasttime = tv_sec;
                        if(!management_connectServer(ctx,ctx->server))
                        {
                            core_updatePumpState(ctx,PUMP_STATE_INITIAL);
                            return PUMP_RET_CUSTOM;
                        }
                    }
                    return PUMP_RET_NOSLEEP;
                }
                break;
            case STATE_CONNECTED:
                {
                    ctx->lasttime = 0;
                    if (ctx->ignoreRegistration)
                    {
                        extern cis_time_t g_onenet_lifetime[];
                        extern cis_time_t g_onenet_lifetimeWarnningTime[];
                        if (ctx->isDM) {
                            ctx->server->registration = g_onenet_lifetime[1];
                            ctx->lifetimeWarnningTime = g_onenet_lifetimeWarnningTime[1];
                        } else {
                            ctx->server->registration = g_onenet_lifetime[0];
                            ctx->lifetimeWarnningTime = g_onenet_lifetimeWarnningTime[0];
                        }
                        core_updatePumpState(ctx,PUMP_STATE_READY);
                        ctx->server->status = STATE_REGISTERED;
                        if (ctx->isDM)
                        {
                            extern char g_onenet_location[32];
                            ctx->server->location = (char *)cissys_malloc(strlen(g_onenet_location) + 1); // for String terminator
                            strcpy(ctx->server->location, g_onenet_location);
                        }
                        else
                        {
                            ctx->server->location = (char *)cissys_malloc(NBSYS_IMEI_MAXLENGTH + 5);
                            sprintf(ctx->server->location, "/rd/");
                            uint8_t imei = cissys_getIMEI(ctx->server->location + 4, NBSYS_IMEI_MAXLENGTH);
                            *((ctx->server->location + 4 + imei)) = '\0';
                        }
                    }
                    else
                    {
                        core_updatePumpState(ctx,PUMP_STATE_REGISTER_REQUIRED);
                        observe_delete_all_retention_data(ctx);
                    }
                    core_callbackEvent(ctx,CIS_EVENT_CONNECT_SUCCESS,NULL);
#if CIS_ENABLE_NOTIFY_FROM_BS
                    if (!ctx->isDM) {
                    ril_indicate_packet_to_track_req_t req_param;
                    req_param.mode = 0;
                    req_param.pattern_id = ctx->pattern_id;
                    ril_request_indicate_packet_to_track(RIL_EXECUTE_MODE, &req_param, NULL, NULL);
                    req_param.mode = 1;
                    req_param.ip_type = "IP";
                    req_param.src_ip = "";
                    req_param.src_udp_port = RIL_OMITTED_INTEGER_PARAM;
                    req_param.dst_ip = ctx->server->host;
                    req_param.dst_udp_port = 5683;
                    req_param.msg_id_offset = 2;
                    req_param.msg_id_size = 2;
                    ril_request_indicate_packet_to_track(RIL_EXECUTE_MODE, &req_param, NULL, NULL);
                    }
#endif /* CIS_ENABLE_NOTIFY_FROM_BS */
                    return PUMP_RET_NOSLEEP;
                }
                break;
            case STATE_CONNECT_FAILED:
                {
                    LOGE("server connect failed.");
                    prv_deleteServer(ctx);
                    core_updatePumpState(ctx,PUMP_STATE_INITIAL);
                    core_callbackEvent(ctx,CIS_EVENT_CONNECT_FAILED,NULL);
                    return PUMP_RET_CUSTOM;
                }
                break;
            }
        }
        break;
    case PUMP_STATE_DISCONNECTED:
        {
       #if CIS_ENABLE_UPDATE //yyl 2018/5/19
          if (ctx->conn_inst!=NULL)
	      {
		     std_conn_moniter_clean(ctx);
	      }
	       if (ctx->device_inst!=NULL)
	      {
		     std_device_clean(ctx);
	      }
	      if (ctx->firmware_inst!=NULL)
	       {
		    std_firmware_clean(ctx);
	      }
		  ctx->created=0;
      #endif
            prv_deleteServer(ctx);
            prv_deleteObservedList(ctx);
            prv_deleteMessageList(ctx);
            prv_deleteTransactionList(ctx);
            prv_deleteRequestList(ctx);
            prv_deleteNotifyList(ctx);

            ctx->ignoreRegistration = false;
            core_updatePumpState(ctx,PUMP_STATE_INITIAL);
            return PUMP_RET_NOSLEEP;
        }
        break;
    case PUMP_STATE_UNREGISTER:
        {
            registration_deregister(ctx);
       #if CIS_ENABLE_UPDATE //yyl 2018/5/19
          if (ctx->conn_inst!=NULL)
	      {
		     std_conn_moniter_clean(ctx);
	      }
	       if (ctx->device_inst!=NULL)
	      {
		     std_device_clean(ctx);
	      }
	      if (ctx->firmware_inst!=NULL)
	       {
		    std_firmware_clean(ctx);
	      }
		  ctx->created=0;
      #endif
            prv_deleteServer(ctx);
            prv_deleteObservedList(ctx);
            prv_deleteMessageList(ctx);
            prv_deleteTransactionList(ctx);
            prv_deleteRequestList(ctx);
            prv_deleteNotifyList(ctx);

            ctx->registerEnabled = false;
            ctx->ignoreRegistration = false;
            core_updatePumpState(ctx,PUMP_STATE_INITIAL);
            core_callbackEvent(ctx,CIS_EVENT_UNREG_DONE,NULL);
            return PUMP_RET_NOSLEEP;
        }
        break;
    case PUMP_STATE_REGISTER_REQUIRED:  //from waiting connection to here
        {
            if(ctx->lasttime == 0 || tv_sec - ctx->lasttime > CIS_CONFIG_REG_INTERVAL_TIME)
            {
                uint32_t result;
                result = registration_start(ctx);
                if (COAP_NO_ERROR != result)
                {
                    return PUMP_RET_CUSTOM;
                }
                core_updatePumpState(ctx,PUMP_STATE_REGISTERING);
                ctx->lasttime = tv_sec;
            }
        }
        break;
    case PUMP_STATE_REGISTERING:
        {
            switch (registration_getStatus(ctx))
            {
            case STATE_REGISTERED:
                {
                    st_serialize_t serialize = {0};
                    ctx->lasttime = 0;
                    core_updatePumpState(ctx,PUMP_STATE_READY);

                    serialize.size = sizeof(serialize);
                    utils_stringCopy((char*)&serialize.host,sizeof(serialize.host),ctx->server->host);
                    cissys_save((uint8_t*)&serialize,sizeof(serialize));

                    return PUMP_RET_NOSLEEP;
                }
                break;
            case STATE_REG_FAILED:
                {
                    core_updatePumpState(ctx,PUMP_STATE_DISCONNECTED);
                    return PUMP_RET_CUSTOM;
                }
                break;
            case STATE_REG_PENDING:
            default:
                // keep on waiting
                break;
            }
        }
        break;
    case PUMP_STATE_READY:
        {
            if(registration_getStatus(ctx) == STATE_REG_FAILED){
                LOGE("ERROR:pump got STATE_REG_FAILED");
                core_updatePumpState(ctx,PUMP_STATE_DISCONNECTED);
                return PUMP_RET_NOSLEEP;
            }
        }
        break;
    default:
        {
            //
        }
        break;
    }

    registration_step(ctx, tv_sec);
    transaction_step(ctx, tv_sec);
    packet_step(ctx, tv_sec);

    readHasData = packet_read(ctx);
    if(readHasData || ctx->stateStep != PUMP_STATE_READY || transaction_count(ctx) != 0)
    {
        return PUMP_RET_NOSLEEP;
    }
    
    if(tv_sec - ctx->lasttime < 1)
    {
        return PUMP_RET_NOSLEEP;
    }

    cissys_lock(ctx->lockNotify,CIS_CONFIG_LOCK_INFINITY);
    notifyCount = CIS_LIST_COUNT((cis_list_t*)ctx->notifyList);
    cissys_unlock(ctx->lockNotify);

    cissys_lock(ctx->lockRequest,CIS_CONFIG_LOCK_INFINITY);
    requestCount = CIS_LIST_COUNT((cis_list_t*)ctx->requestList);
    cissys_unlock(ctx->lockRequest);

    if(requestCount != 0 || notifyCount != 0){
        return PUMP_RET_NOSLEEP;
    }

    ctx->lasttime = tv_sec;
    return PUMP_RET_CUSTOM;
}



cis_ret_t    cis_update_reg       (void* context,cis_time_t lifetime,bool withObjects)
{
    cissys_assert(context!=NULL);
    st_context_t* ctx = (st_context_t*)context;
    if(ctx->stateStep != PUMP_STATE_READY)
    {
        return CIS_RET_INVILID;
    }

    if(lifetime != LIFETIME_INVALID && 
        lifetime > LIFETIME_LIMIT_MIN && lifetime < LIFETIME_LIMIT_MAX)
    {
        ctx->lifetime = lifetime;
    }

    return registration_update_registration(ctx, withObjects);
}


CIS_API cis_ret_t    cis_response   	  (void* context,const cis_uri_t* uri,const cis_data_t* value,cis_mid_t mid,cis_coapret_t result)
{
    st_context_t* ctx = (st_context_t*)context;
    st_notify_t* notify = NULL;

    if(context == NULL)
    {
        return CIS_RET_PARAMETER_ERR;
    }

    if(ctx->stateStep != PUMP_STATE_READY)
    {
        return CIS_RET_INVILID;
    }

    if(result < CIS_COAP_204_CHANGED || result > CIS_COAP_503_SERVICE_UNAVAILABLE)
    {
        return CIS_RET_ERROR;
    }


    notify = (st_notify_t*)cis_malloc(sizeof(st_notify_t));
    cissys_assert(notify != NULL);
    if(notify == NULL)
    {
        return CIS_RET_MEMORY_ERR;
    }

    notify->isResponse = true;
    notify->next = NULL;
    notify->id = ++ctx->nextNotifyId;
    notify->mid = mid;
    notify->result = result;
    notify->value = NULL;
    if(value != NULL)
    {
        notify->value = prv_dataDup(value);
        if(notify->value == NULL)
        {
            cis_free(notify);
            return CIS_RET_MEMORY_ERR;
        }
    }
    if(uri != NULL)
    {
        notify->uri = *uri;
        LOGD("cis_response add index:%d mid:0x%x [%u/%u/%u:%u],result:(%s)",
            ctx->nextNotifyId,mid,notify->uri.objectId,notify->uri.instanceId,notify->uri.resourceId,notify->uri.flag,STR_COAP_CODE(result));
    }else
    {
        cis_uri_make(URI_INVALID,URI_INVALID,URI_INVALID,&notify->uri);
        LOGD("cis_response add index:%d mid:0x%x ,result:[%s]",ctx->nextNotifyId,mid,STR_COAP_CODE(result));
    }

    cissys_lock(ctx->lockNotify,CIS_CONFIG_LOCK_INFINITY);
    ctx->notifyList = (st_notify_t*)CIS_LIST_ADD(ctx->notifyList,notify);
    cissys_unlock(ctx->lockNotify);
    return CIS_RET_OK;
}

CIS_API cis_ret_t    cis_notify(void* context,const cis_uri_t* uri,const cis_data_t* value,cis_mid_t mid,cis_coapret_t result,bool needAck)
{
    return cis_notify_ex(context,uri,value,mid,result,-1);
}

CIS_API cis_ret_t    cis_notify_ex(void* context,const cis_uri_t* uri,const cis_data_t* value,cis_mid_t mid,cis_coapret_t result,cis_mid_t ackid)
{
    st_context_t* ctx = (st_context_t*)context;
    st_notify_t* notify = NULL;

    if(context == NULL)
    {
        return CIS_RET_PARAMETER_ERR;
    }


    if(ctx->stateStep != PUMP_STATE_READY)
    {
        return CIS_RET_INVILID;
    }

    if(result != CIS_NOTIFY_CONTENT && result != CIS_NOTIFY_CONTINUE)
    {
        return CIS_RET_PARAMETER_ERR;
    }


    notify = (st_notify_t*)cis_malloc(sizeof(st_notify_t));
    cissys_assert(notify != NULL);
    if(notify == NULL)
    {
        return CIS_RET_MEMORY_ERR;
    }

    notify->isResponse = false;
    notify->next = NULL;
    notify->id = ++ctx->nextNotifyId;
    notify->mid = mid;
    notify->result = result;
    notify->ackid = ackid;
    notify->value = NULL;
    if(value != NULL)
    {
        notify->value = prv_dataDup(value);
        if(notify->value == NULL)
        {
            cis_free(notify);
            return CIS_RET_MEMORY_ERR;
        }
    }

    if(uri != NULL)
    {
        notify->uri = *uri;
        LOGD("cis_notify add index:%d mid:0x%x [%d/%d/%d]",ctx->nextNotifyId,mid,uri->objectId,uri->instanceId,uri->resourceId);
    }else
    {
        cis_uri_make(URI_INVALID,URI_INVALID,URI_INVALID,&notify->uri);
        LOGD("cis_notify add index:%d mid:0x%x",ctx->nextNotifyId,mid);
    }
    
    cissys_lock(ctx->lockNotify,CIS_CONFIG_LOCK_INFINITY);
    ctx->notifyList = (st_notify_t*)CIS_LIST_ADD(ctx->notifyList,notify);
    cissys_unlock(ctx->lockNotify);
    return CIS_RET_OK;
}




//////////////////////////////////////////////////////////////////////////
void  core_callbackEvent(st_context_t* context,cis_evt_t id,void* param)
{
    if(context->callback.onEvent != NULL)
    {
        LOGD("callback event(%d):%s",id,STR_EVENT_CODE(id));
        context->callback.onEvent(context,id,param);
    }
}

void core_updatePumpState(st_context_t* context,et_client_state_t state)
{
    LOGD("Update State To %s(%d)",STR_STATE(state),state);
    if (context->stateStep == PUMP_STATE_UNREGISTER && state != PUMP_STATE_INITIAL) return;
    context->stateStep = state;
}


//////////////////////////////////////////////////////////////////////////
//static private function.
// 


void prv_localDeinit(st_context_t** context)
{
    st_context_t* ctx = *context;

    std_security_clean(ctx);

    prv_deleteObservedList(ctx);
    prv_deleteMessageList(ctx);
    prv_deleteTransactionList(ctx);
    prv_deleteRequestList(ctx);
    prv_deleteNotifyList(ctx);
    prv_deleteObjectList(ctx);
    prv_deleteServer(ctx);
#if CIS_ENABLE_UPDATE //yyl 2018/5/24
	if (ctx->conn_inst!=NULL)
	{
		std_conn_moniter_clean(ctx);
	}
	if (ctx->device_inst!=NULL)
	{
		std_device_clean(ctx);
	}
	if (ctx->firmware_inst!=NULL)
	{
		std_firmware_clean(ctx);
	}
		ctx->created=0;
 #endif


    cissys_lockdestory(ctx->lockRequest);
    cissys_lockdestory(ctx->lockNotify);
    cissys_lockdestory(ctx->lockSocket);
#if CIS_ENABLE_AUTH
	  if(ctx->authCode!=NULL)
    {
        cis_free(ctx->authCode);
        ctx->authCode = NULL;
    }

#endif
    if(ctx->endpointName != NULL)
    {
        cis_free(ctx->endpointName);
        ctx->endpointName = NULL;
    }
#if CIS_ENABLE_DM
      if(ctx->DMprivData)
    {
        cis_free(ctx->DMprivData);
        ctx->DMprivData = NULL;
    }

#endif

    if(ctx)
    {
        cis_free(ctx);
        (*context) = NULL;
    }

#if CIS_ENABLE_MEMORYTRACE
    g_tracetime = 0;
#endif//CIS_ENABLE_MEMORYTRACE
}



static cis_data_t* prv_dataDup(const cis_data_t* src)
{
    if(src == NULL)return NULL;
    cis_data_t* newData = (cis_data_t*)cis_malloc(sizeof(cis_data_t));
    if(newData == NULL)
    {
        return NULL;
    }

    if(src->type == cis_data_type_opaque || src->type == cis_data_type_string)
    {
        newData->asBuffer.buffer = (uint8_t*)cis_malloc(src->asBuffer.length);
        if(newData == NULL || newData->asBuffer.buffer == NULL)
        {
            return NULL;
        }
        newData->asBuffer.length = src->asBuffer.length;
        cissys_memcpy(newData->asBuffer.buffer,src->asBuffer.buffer,src->asBuffer.length);
    }
    cissys_memcpy(&newData->value.asInteger,&src->value.asInteger,sizeof(src->value.asInteger));
    newData->type = src->type;
    return newData;
}


st_object_t* prv_findObject(st_context_t* context,cis_oid_t objectid)
{
    st_object_t * targetP;
    targetP = (st_object_t *)CIS_LIST_FIND(context->objectList, objectid);
    return targetP;
}


static void prv_deleteTransactionList(st_context_t* context)
{
    LOGD("fall in transaction_removeAll\n");
    transaction_removeAll(context);
}

static void prv_deleteMessageList(st_context_t *context)
{
    LOGD("fall in message_removeAll\n");
    while (NULL != context->messageList)
    {
        st_message_t * targetP;

        targetP = context->messageList;
        context->messageList = context->messageList->next;

        cis_free(targetP);
    }
}

static void prv_deleteObservedList( st_context_t* context)
{
    LOGD("fall in observe_removeAll\n");
    observe_removeAll(context);
	
}

static void     prv_deleteRequestList(st_context_t* context)
{
    LOGD("fall in packet_request_removeAll\n");
    packet_asynRemoveRequestAll(context);
}

static void     prv_deleteNotifyList(st_context_t* context)
{
    LOGD("fall in packet_notify_removeAll\n");
    packet_asynRemoveNotifyAll(context);
}

static void     prv_deleteObjectList(st_context_t* context)
{
    LOGD("fall in object_removeAll\n");
    object_removeAll(context);
}


static void     prv_deleteServer(st_context_t* context)
{
    if(context->server)
    {
        management_destoryServer(context,context->server);
        context->server = NULL;
    }
    if(context->bootstrapServer)
    {
        management_destoryServer(context,context->bootstrapServer);
        context->bootstrapServer = NULL;
    }
}

#if CIS_ENABLE_UPDATE
cis_ret_t reset_fotaIDIL(st_context_t* context,uint32_t msgid)
{
	cis_ret_t ret = 0;
	//save state and result to flash
	//cissys_setFwState();
	//cissys_setFwUpdateResult(msgid);

	//delete /3 /4 /5
	std_conn_moniter_clean(context);
	std_device_clean(context);
	std_firmware_clean(context);
    context->created=0;
	cis_update_reg(context,0,true);
	core_callbackEvent(context,msgid,NULL);	

	return ret;
}

cis_ret_t prv_onSySEventHandler(cissys_event_t id,void* param,void* userData,int *len)
{
	cis_ret_t ret = 0;
	st_context_t* context = (st_context_t*)userData;
	firmware_data_t * firmwareInstance = (firmware_data_t *)(context->firmware_inst);
	if (firmwareInstance == NULL)
	{
		LOGE("No firmware instance");
		return ret;
	}
	cis_fw_context_t* fw_info = (cis_fw_context_t*)(firmwareInstance->fw_info);
	if (fw_info == NULL)
	{
		LOGE("No fota information");
		return ret;
	}
	context->fw_request.tv_t = utils_gettime_s(); 
	switch(id)
	{
	case 	cissys_event_write_success: //
		{
			LOGD("writed package ok");
			context->fw_request.write_state = PACKAGE_WRITE_IDIL;

			//single write,wait for the validate result
			if (context->fw_request.single_packet_write == true)   
			{
				//wait for validation result to send ack
				if (!cissys_checkFwValidation(&(context->g_fotacallback)))
				{
					//set validate result
					fw_info->result = FOTA_RESULT_CHECKFAIL;
					cissys_setFwUpdateResult(fw_info->result);
					context->fw_request.code = COAP_500_INTERNAL_SERVER_ERROR;
					object_asynAckNodata(context,context->fw_request.response_ack,COAP_500_INTERNAL_SERVER_ERROR);
					reset_fotaIDIL(context,CIS_EVENT_FIRMWARE_DOWNLOAD_FAILED);
				}
				context->fw_request.code = COAP_204_CHANGED;
				return ret;
			}

			//block write result
			if (context->fw_request.block1_more == 0) //the last block for write
			{
				if (!cissys_checkFwValidation(&(context->g_fotacallback)))
				{
					fw_info->result = FOTA_RESULT_CHECKFAIL;
					cissys_setFwUpdateResult(fw_info->result);
					context->fw_request.code = COAP_204_CHANGED;
					object_asynAckBlockWrite(context,context->fw_request.response_ack,COAP_204_CHANGED,context->fw_request.ack_type);
					reset_fotaIDIL(context,CIS_EVENT_FIRMWARE_DOWNLOAD_FAILED);
				}
			}
			else//not the last write
			{
				context->fw_request.code = COAP_231_CONTINUE;
				object_asynAckBlockWrite(context,context->fw_request.response_ack,COAP_231_CONTINUE,context->fw_request.ack_type);
			}
		}
		break;
	case  	cissys_event_write_fail:
		{
			LOGD("writed package failed,return 500 to server");
			if (context->fw_request.single_packet_write == true)   //single write
			{
				context->fw_request.single_packet_write = false;
				context->fw_request.code = COAP_500_INTERNAL_SERVER_ERROR;
				object_asynAckNodata(context,context->fw_request.response_ack,COAP_500_INTERNAL_SERVER_ERROR);
			}
			else
			{	
				context->fw_request.code = COAP_231_CONTINUE;
				object_asynAckBlockWrite(context,context->fw_request.response_ack,COAP_500_INTERNAL_SERVER_ERROR,context->fw_request.ack_type);
			}

			fw_info->result = FOTA_RESULT_CHECKFAIL;
			cissys_setFwUpdateResult(fw_info->result);
			context->fw_request.write_state = PACKAGE_WRITE_IDIL;
			reset_fotaIDIL(context,CIS_EVENT_FIRMWARE_DOWNLOAD_FAILED);
		}
		break;	
	case  	cissys_event_fw_erase_success:
		{
			context->fw_request.write_state=PACKAGE_WRITE_IDIL;//yyl 2018/5/28
			fw_info->result = FOTA_RESULT_INIT;
			fw_info->state = FOTA_STATE_IDIL;
            fw_state_change(context,FOTA_STATE_IDIL);//yyl 2018/5/17
			cissys_setFwState(FOTA_STATE_IDIL);//yyl 2018/5/17
			fw_info->savebytes = cissys_getFwSavedBytes();
			//cissys_setFwSavedBytes(0);//yyl 2018/5/17
			cissys_setFwUpdateResult(fw_info->result);
		/*	
			context->fw_request.block1_mid= 0;//yyl 2018/5/17
	        context->fw_request.single_mid= 0;//yyl 2018/5/17
	        context->fw_request.block1_more = 0;//yyl 2018/5/17
	        context->fw_request.block1_num = 0;//yyl 2018/5/17
	        context->fw_request.block1_size = REST_MAX_CHUNK_SIZE;//yyl 2018/5/17
			context->fw_request.tv_t = 0;//yyl 2018/5/17
	        context->fw_request.write_state = PACKAGE_WRITE_IDIL;//yyl 2018/5/17
	        context->fw_request.single_packet_write = false;//yyl 2018/5/17
	        context->fw_request.need_async_ack = false;//yyl 2018/5/17
	      */
			object_asynAckNodata(context,context->fw_request.response_ack,COAP_204_CHANGED);
	        reset_fotaIDIL(context,CIS_EVENT_FIRMWARE_ERASE_SUCCESS);		
		}
		break;
	case  	cissys_event_fw_erase_fail:
		{
			context->fw_request.write_state= PACKAGE_WRITE_FAIL;//yyl 2018/5/28
			fw_info->result = FOTA_RESULT_UPDATEFAIL;
			cissys_setFwUpdateResult(fw_info->result);
			context->fw_request.code = COAP_500_INTERNAL_SERVER_ERROR;
			object_asynAckNodata(context,context->fw_request.response_ack,COAP_500_INTERNAL_SERVER_ERROR);
			reset_fotaIDIL(context,CIS_EVENT_FIRMWARE_ERASE_FAIL);
		}
		break;
	case  	cissys_event_fw_validate_success: //emit after validation
		{
			fw_state_change(context,FOTA_STATE_DOWNLOADED);
			cissys_setFwState(FOTA_STATE_DOWNLOADED);
			core_callbackEvent(context,CIS_EVENT_FIRMWARE_DOWNLOADED,NULL);	
			context->fw_request.code = COAP_204_CHANGED;
			if (context->fw_request.single_packet_write == true)   //single write validate
			{
				context->fw_request.single_packet_write = false;				
				object_asynAckNodata(context,context->fw_request.response_ack,COAP_204_CHANGED);
			}
			else
			{
				object_asynAckBlockWrite(context,context->fw_request.response_ack,COAP_204_CHANGED,context->fw_request.ack_type);				
			}
		}
		break;
	case  	cissys_event_fw_validate_fail:
		{
			fw_info->result = FOTA_RESULT_CHECKFAIL;
			cissys_setFwUpdateResult(fw_info->result);
			context->fw_request.write_state = PACKAGE_WRITE_FAIL;
			context->fw_request.code = COAP_500_INTERNAL_SERVER_ERROR;
			if (context->fw_request.single_packet_write == true)   //single write validate
			{
				context->fw_request.single_packet_write = false;
				object_asynAckNodata(context,context->fw_request.response_ack,COAP_500_INTERNAL_SERVER_ERROR);
			}
			else
			{
				object_asynAckBlockWrite(context,context->fw_request.response_ack,COAP_500_INTERNAL_SERVER_ERROR,context->fw_request.ack_type);				
			}

			reset_fotaIDIL(context,CIS_EVENT_FIRMWARE_DOWNLOAD_FAILED);
		}	
		break;
	case  	cissys_event_fw_update_success:
		{
			fw_info->result = FOTA_RESULT_SUCCESS;
			cissys_setFwUpdateResult(fw_info->result);
		}
		break;
	case  	cissys_event_fw_update_fail:
		{
			fw_info->result = FOTA_RESULT_UPDATEFAIL;
			cissys_setFwUpdateResult(fw_info->result);
			reset_fotaIDIL(context,CIS_EVENT_FIRMWARE_UPDATE_FAILED);
		}
		break;
	case 	cissys_event_unknow:
		{
			reset_fotaIDIL(context,CIS_EVENT_FIRMWARE_UPDATE_FAILED);
		}
		break;
	default:
		break;
	}
	return ret;
}
#endif

static cis_ret_t prv_onNetEventHandler(cisnet_t netctx,cisnet_event_t id,void* param,void* context)
{
    st_context_t* contextP = (st_context_t*)context;
    switch(id)
    {
    case cisnet_event_unknow:
        {
            core_updatePumpState(contextP,PUMP_STATE_HALT);
        }
        break;
    case cisnet_event_connected:
        {
            if(contextP->stateStep == PUMP_STATE_BOOTSTRAPPING){
                contextP->bootstrapServer->status = STATE_CONNECTED;
                LOGI("bootstrap server connected.");
            }
            else if(contextP->stateStep == PUMP_STATE_CONNECTING){
                contextP->server->status = STATE_CONNECTED;
                LOGI("server connected.");
            }
        }
        break;
    case cisnet_event_disconnect:
        {
            if(contextP->stateStep == PUMP_STATE_READY){
                core_updatePumpState(contextP,PUMP_STATE_DISCONNECTED);
            }
            else if(contextP->stateStep == PUMP_STATE_BOOTSTRAPPING){
                contextP->bootstrapServer->status = STATE_BS_FAILED;
            }
            else if(contextP->stateStep == PUMP_STATE_CONNECTING){
                contextP->server->status = STATE_CONNECT_FAILED;
            }
        }
        break;
    default:
        break;
    }
    
    return CIS_RET_ERROR;
}

static int prv_makeDeviceName(char** name)
{
	(*name) = (char*)cis_malloc(NBSYS_IMEI_MAXLENGTH + NBSYS_IMSI_MAXLENGTH + 2);
    if((*name) == NULL)
    {
        return -1;
    }
	cis_memset((*name),0,NBSYS_IMEI_MAXLENGTH+NBSYS_IMSI_MAXLENGTH+1);
	uint8_t imei =  cissys_getIMEI((*name),NBSYS_IMEI_MAXLENGTH);
    *((char*)((*name)+imei)) = ';';
	uint8_t imsi =  cissys_getIMSI((*name) + imei + 1,NBSYS_IMSI_MAXLENGTH);
	if(imei <= 0 || imsi <=0 || utils_strlen((char*)(*name)) <= 0)
	{
		LOGE("ERROR:Get IMEI/IMSI ERROR.\n");
		return 0;
	}
	return imei+imsi;
}

#if CIS_ENABLE_NOTIFY_FROM_BS
static int32_t prv_ril_urc_notify_callback(ril_urc_id_t event_id, void *param, uint32_t param_len)
{
    if (!g_onenet_ctx || !param || !param_len) {
        return 0;
    }

    switch (event_id) {
        case RIL_URC_ID_MUPDIR: {
            ril_indicate_packet_to_track_urc_t *updir_urc = (ril_indicate_packet_to_track_urc_t*)param;

            if (updir_urc) {
                LOGI("[MUPDIR]pattern_id = %d", updir_urc->pattern_id);
                g_onenet_ctx->pattern_id = updir_urc->pattern_id;
            }
            break;
        }

        case RIL_URC_ID_MUPDI: {
            ril_indicate_packet_delivery_status_urc_t *updi_urc = (ril_indicate_packet_delivery_status_urc_t*)param;

            if (updi_urc && updi_urc->pattern_id == g_onenet_ctx->pattern_id) {
                int32_t i;
                LOGI("[MUPDI]status = %d, num_msg_id = %d", updi_urc->status, updi_urc->num_msg_id);
                if (updi_urc->status == 0) {
                    LOGI("[MUPDI]msg_id_list[%d] = %d", i, updi_urc->msg_id_list[i]);
                    st_message_t* observe = observe_findMessage(g_onenet_ctx,(uint16_t)updi_urc->msg_id_list[i]);
                    if (observe != NULL)
                    {
                        /* free when send failed from BS */
                        observe_freeMessage(g_onenet_ctx,(uint16_t)updi_urc->msg_id_list[i]);
                    }
                    break;
                }
                for (i = 0; i < updi_urc->num_msg_id; i++) {
                    LOGI("[MUPDI]msg_id_list[%d] = %d", i, updi_urc->msg_id_list[i]);
                    st_message_t* observe = observe_findMessage(g_onenet_ctx,(uint16_t)updi_urc->msg_id_list[i]);
                    if (observe != NULL)
                    {
                        core_callbackEvent(g_onenet_ctx,CIS_EVENT_NOTIFY_SUCCESS_FROM_BS,(void*)observe->ackid);
                    }
                }
            }
            break;
        }

        default:
            break;
    }

    return 0;
}
#endif /* CIS_ENABLE_NOTIFY_FROM_BS */
