#include <cis_def.h>
#if CIS_ENABLE_DM
#include <stdio.h>
#include <stdlib.h>
#include <cis_api.h>
#include <cis_if_sys.h>
#include <cis_log.h>
#include <cis_list.h>
#include <cis_api.h>
#include "cis_sample_defs.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"

static void     prv_dm_observeNotify     (void* context,cis_uri_t* uri,cis_mid_t mid);

static void     prv_dm_readResponse     (void* context,cis_uri_t* uri,cis_mid_t mid);
static void     prv_dm_discoverResponse (void* context,cis_uri_t* uri,cis_mid_t mid);
static void     prv_dm_writeResponse    (void* context,cis_uri_t* uri,const cis_data_t* value,cis_attrcount_t count,cis_mid_t mid);
static void     prv_dm_execResponse     (void* context,cis_uri_t* uri,const uint8_t* value,uint32_t length,cis_mid_t mid);
static void     prv_dm_paramsResponse   (void* context,cis_uri_t* uri,cis_observe_attr_t parameters,cis_mid_t mid);
static cis_data_t* prv_dm_dataDup(const cis_data_t* value,cis_attrcount_t attrcount);

static cis_coapret_t dm_onRead        (void* context,cis_uri_t* uri,cis_mid_t mid);
static cis_coapret_t dm_onWrite       (void* context,cis_uri_t* uri,const cis_data_t* value,cis_attrcount_t attrcount,cis_mid_t mid);
static cis_coapret_t dm_onExec        (void* context,cis_uri_t* uri,const uint8_t* value,uint32_t length,cis_mid_t mid);
static cis_coapret_t dm_onObserve     (void* context,cis_uri_t* uri,bool flag,cis_mid_t mid);
static cis_coapret_t dm_onParams      (void* context,cis_uri_t* uri,cis_observe_attr_t parameters,cis_mid_t mid);
static cis_coapret_t dm_onDiscover    (void* context,cis_uri_t* uri,cis_mid_t mid);
static void        dm_onEvent       (void* context,cis_evt_t eid,void* param);

static struct st_callback_info* g_dmcallbackList = NULL;
static struct st_observe_info*  g_dmobserveList = NULL;

static void*      g_dmcontext = NULL;
static bool       g_dmshutdown = false;
static bool       g_doUnregisterdm = false;
static bool       g_doRegisterdm = false;

static st_sample_object     g_objectListdm[SAMPLE_OBJECT_MAX];
static st_instance_a        g_dminstList_a[SAMPLE_A_INSTANCE_COUNT];
static st_instance_b        g_dminstList_b[SAMPLE_B_INSTANCE_COUNT];

static cis_time_t g_lifetime = 0;
static cis_callback_t callback;

void dmTaskThread(void* arg)
{
    while(!g_dmshutdown)
    {
        uint32_t pumpRet;
 
        /*
        *wait press keyboard for register test;
        *do register press 'o'
        *do unregister press 'r'
        **/
        {
            if(g_doRegisterdm)
            {
                g_doRegisterdm = false;
                cis_register(g_dmcontext,g_lifetime,&callback);//将回调函数注册进SDK内
            }
            if(g_doUnregisterdm){
                g_doUnregisterdm = false;
                cis_unregister(g_dmcontext);
                struct st_observe_info* delnode;
                while(g_dmobserveList != NULL){
                    g_dmobserveList =(struct st_observe_info *)CIS_LIST_RM((cis_list_t *)g_dmobserveList,g_dmobserveList->mid,(cis_list_t **)&delnode);
                    cis_free(delnode);
                }
				cissys_sleepms(1000);
                g_doRegisterdm = 1;
            }
        }
        
        /*pump function*/
        pumpRet = cis_pump(g_dmcontext);
        if(pumpRet == PUMP_RET_CUSTOM)
        {
            //printf("pump sleep(1000)\n");
            cissys_sleepms(1000);
        }
        struct st_callback_info* node;
        if(g_dmcallbackList == NULL){
            cissys_sleepms(100);
            continue;
        }
        node = g_dmcallbackList;
        g_dmcallbackList = g_dmcallbackList->next;
        
        switch (node->flag)
        {
            case 0:
                break;
            case SAMPLE_CALLBACK_READ:
                {
                    cis_uri_t uriLocal;
                    uriLocal = node->uri;
                    prv_dm_readResponse(g_dmcontext,&uriLocal,node->mid);
                }
                break;
            case SAMPLE_CALLBACK_DISCOVER:
                {
                    cis_uri_t uriLocal;
                    uriLocal = node->uri;
                    prv_dm_discoverResponse(g_dmcontext,&uriLocal,node->mid);
                }
                break;
            case SAMPLE_CALLBACK_WRITE:
                {
                    //write
                    prv_dm_writeResponse(g_dmcontext,&node->uri,node->param.asWrite.value,node->param.asWrite.count,node->mid);
                    cis_data_t* data = node->param.asWrite.value;
                    cis_attrcount_t count = node->param.asWrite.count;

                    for (int i=0;i<count;i++)
                    {
                        if(data[i].type == cis_data_type_string || data[i].type == cis_data_type_opaque)
                        {
                            if(data[i].asBuffer.buffer != NULL)
                            cis_free(data[i].asBuffer.buffer);
                        }
                    }
                    cis_free(data);
                }
                break;
            case SAMPLE_CALLBACK_EXECUTE:
                {
                    //exec and notify
                    prv_dm_execResponse(g_dmcontext,&node->uri,node->param.asExec.buffer,node->param.asExec.length,node->mid);
                    cis_free(node->param.asExec.buffer);
                }
                break;
            case SAMPLE_CALLBACK_SETPARAMS:
                {
                    //set parameters and notify
                    prv_dm_paramsResponse(g_dmcontext,&node->uri,node->param.asObserveParam.params,node->mid);
                }
                break;
            case SAMPLE_CALLBACK_OBSERVE:
                {
                    if(node->param.asObserve.flag){
                        uint16_t count = 0;
                        struct st_observe_info* observe_new = (struct st_observe_info*)cis_malloc(sizeof(struct st_observe_info));
                        cissys_assert(observe_new!=NULL);
                        observe_new->mid = node->mid;
                        observe_new->uri = node->uri;
                        observe_new->next = NULL;

                        g_dmobserveList = (struct st_observe_info*)cis_list_add((cis_list_t*)g_dmobserveList,(cis_list_t*)observe_new);
                       /*
                        LOGD("cis_on_observe set(%d): %d/%d/%d",
                            count,
                            uri->objectId,
                            CIS_URI_IS_SET_INSTANCE(uri)?uri->instanceId:-1,
                            CIS_URI_IS_SET_RESOURCE(uri)?uri->resourceId:-1);
											*/

                        cis_response(g_dmcontext,NULL,NULL,node->mid,CIS_RESPONSE_OBSERVE);
                    }else{
                        struct st_observe_info* delnode = NULL;

                        g_dmobserveList = (struct st_observe_info *)cis_list_remove((cis_list_t *)g_dmobserveList,node->mid,(cis_list_t **)&delnode);
						if(delnode == NULL)
						{
						   cis_free(node);
						   continue;
						
						}

                        cis_free(delnode);
                        /*
                        LOGD("cis_on_observe cancel: %d/%d/%d\n",
                            uri->objectId,
                            CIS_URI_IS_SET_INSTANCE(uri)?uri->instanceId:-1,
                            CIS_URI_IS_SET_RESOURCE(uri)?uri->resourceId:-1);
                         */
                        cis_response(g_dmcontext,NULL,NULL,node->mid,CIS_RESPONSE_OBSERVE);
                    }
                }
            default:
                break;
        }

        cis_free(node);
    }

    cis_deinit(&g_dmcontext);

    struct st_observe_info* delnode;
    while(g_dmobserveList != NULL){
        g_dmobserveList =(struct st_observe_info *)CIS_LIST_RM((cis_list_t *)g_dmobserveList,g_dmobserveList->mid,(cis_list_t **)&delnode);
        cis_free(delnode);
    }
    cissys_sleepms(2000);
}


Options dm_config = {"CMEI_IMEI","IMSI", "v1.0","M100000100","hW59p448439g1jX7qe7ArM276IKAu0fl",4,"",5683,300,0,""};
int dm_sample_dm_entry(const uint8_t * config_bin,uint32_t config_size)
{
	char ver[15];
    int index = 0;
	callback.onRead = dm_onRead;
	callback.onWrite = dm_onWrite;
	callback.onExec = dm_onExec;
    callback.onObserve = dm_onObserve;
    callback.onSetParams = dm_onParams;
    callback.onEvent = dm_onEvent;
	callback.onDiscover = dm_onDiscover;

     g_lifetime = dm_config.nLifetime;
    /*init sample data*/
		if(g_lifetime<120)
		{
			g_lifetime=120;
		}
	// prv_make_sample_data();
	 	printf("lifetime:%d\n",g_lifetime);
		printf("version %s\r\n",cis_get_version(ver));
		cis_memcpy(dm_config.szDMv,ver,strlen(ver)+1);
#define SAMPLE_DEFAULT_CONFIG 0
#if SAMPLE_DEFAULT_CONFIG
    if(cis_init(&g_dmcontext,NULL,0,&dm_config,NULL) != CIS_RET_OK){
        printf("cis entry init failed.\n");
        return -1;
    }
#else
		
    if(cis_init(&g_dmcontext,(void *)config_bin,config_size,&dm_config,NULL) != CIS_RET_OK){
        printf("cis entry init failed.\n");
        return -1;
    }
#endif  
	    
		log_config(true,1,10,1024);//For debug 
		
		
    g_doUnregisterdm = false;
    
    //register enabled
    g_doRegisterdm = true;
	
    xTaskCreate(dmTaskThread,
                 "dm_process",
                 2048 / sizeof(portSTACK_TYPE),
                 NULL,
                 TASK_PRIORITY_NORMAL,
                 NULL);
		
	return 0;
}


//////////////////////////////////////////////////////////////////////////
//private funcation;
static void prv_dm_observeNotify(void* context,cis_uri_t* uri,cis_mid_t mid)
{
    uint8_t index;
    st_sample_object* object = NULL;
    cis_data_t value;
    for (index = 0;index< SAMPLE_OBJECT_MAX;index++)
    {
        if(g_objectListdm[index].oid ==  uri->objectId){
            object = &g_objectListdm[index];
        }
    }

    if(object == NULL){
        return;
    }


    if(!CIS_URI_IS_SET_INSTANCE(uri) && !CIS_URI_IS_SET_RESOURCE(uri)) // one object
    {
        switch(uri->objectId)
        {
        case SAMPLE_OID_A:
            {
                for(index=0;index<SAMPLE_A_INSTANCE_COUNT;index++)
                {                   
                    st_instance_a *inst = &g_dminstList_a[index];
                    if(inst != NULL &&  inst->enabled == true)
                    {
                        cis_data_t tmpdata[4];
                        tmpdata[0].type = cis_data_type_integer;
                        tmpdata[0].value.asInteger = inst->instance.intValue;
                        uri->instanceId = inst->instId;
                        uri->resourceId = attributeA_intValue;
                        cis_uri_update(uri);
                        cis_notify(context,uri,&tmpdata[0],mid,CIS_NOTIFY_CONTINUE,false);

                        tmpdata[1].type = cis_data_type_float;
                        tmpdata[1].value.asFloat = inst->instance.floatValue;
                        uri->instanceId = inst->instId;
                        uri->resourceId = attributeA_floatValue;
                        cis_uri_update(uri);
                        cis_notify(context,uri,&tmpdata[1],mid,CIS_NOTIFY_CONTINUE,false);

                        tmpdata[2].type = cis_data_type_bool;
                        tmpdata[2].value.asBoolean = inst->instance.boolValue;
                        uri->resourceId = attributeA_boolValue;
                        uri->instanceId = inst->instId;
                        cis_uri_update(uri);
                        cis_notify(context,uri,&tmpdata[2],mid,CIS_NOTIFY_CONTINUE,false);

                        tmpdata[3].type = cis_data_type_string;
                        tmpdata[3].asBuffer.length = utils_strlen(inst->instance.strValue);
                        tmpdata[3].asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//utils_strdup(inst->instance.strValue);
                        uri->resourceId = attributeA_stringValue;
                        uri->instanceId = inst->instId;
                        cis_uri_update(uri);
                        cis_notify(context,uri,&tmpdata[3],mid,CIS_NOTIFY_CONTENT,false);
                    }
                }
            }
            break;
        case SAMPLE_OID_B:
            {
                for(index=0;index<SAMPLE_A_INSTANCE_COUNT;index++)
                {                   
                    st_instance_b *inst = &g_dminstList_b[index];
                    if(inst != NULL &&  inst->enabled == true)
                    {
                        cis_data_t tmpdata[4];

                        tmpdata[0].type = cis_data_type_integer;
                        tmpdata[0].value.asInteger = inst->instance.intValue;
                        uri->resourceId = attributeB_intValue;
                        cis_uri_update(uri);
                        cis_notify(context,uri,&tmpdata[0],mid,CIS_NOTIFY_CONTINUE,false);

                        tmpdata[1].type = cis_data_type_float;
                        tmpdata[1].value.asFloat = inst->instance.floatValue;
                        uri->resourceId = attributeB_floatValue;
                        cis_uri_update(uri);
                        cis_notify(context,uri,&tmpdata[1],mid,CIS_NOTIFY_CONTINUE,false);

                        tmpdata[2].type = cis_data_type_string;
                        tmpdata[2].asBuffer.length = utils_strlen(inst->instance.strValue);
                        tmpdata[2].asBuffer.buffer =(uint8_t*)(inst->instance.strValue); //(uint8_t*)utils_strdup(inst->instance.strValue);
                        uri->resourceId = attributeB_stringValue;
                        cis_uri_update(uri);
                        cis_notify(context,uri,&tmpdata[2],mid,CIS_NOTIFY_CONTENT,false);
                    }
                }
            }
            break;
        }
    }else if(CIS_URI_IS_SET_INSTANCE(uri))
    {
        switch(object->oid)
        {
        case SAMPLE_OID_A:
            {
                if(uri->instanceId > SAMPLE_A_INSTANCE_COUNT){
                    return;
                }
                st_instance_a *inst = &g_dminstList_a[uri->instanceId];
                if(inst == NULL || inst->enabled == false){
                    return;
                }

                if(CIS_URI_IS_SET_RESOURCE(uri)){
                    if(uri->resourceId == attributeA_intValue)
                    {
                        value.type = cis_data_type_integer;
                        value.value.asInteger = inst->instance.intValue;
                    }
                    else if(uri->resourceId == attributeA_floatValue)
                    {
                        value.type = cis_data_type_float;
                        value.value.asFloat = inst->instance.floatValue;
                    }
                    else if(uri->resourceId == attributeA_boolValue)
                    {
                        value.type = cis_data_type_bool;
                        value.value.asBoolean = inst->instance.boolValue;
                    }
                    else if(uri->resourceId == attributeA_stringValue)
                    {
                        value.type = cis_data_type_string;
                        value.asBuffer.length = utils_strlen(inst->instance.strValue);
                        value.asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                    }else{
                        return;
                    }

                    cis_notify(context,uri,&value,mid,CIS_NOTIFY_CONTENT,false);

                }else{
                    cis_data_t tmpdata[4];

                    tmpdata[0].type = cis_data_type_integer;
                    tmpdata[0].value.asInteger = inst->instance.intValue;
                    uri->resourceId = attributeA_intValue;
                    cis_uri_update(uri);
                    cis_notify(context,uri,&tmpdata[0],mid,CIS_NOTIFY_CONTINUE,false);

                    tmpdata[1].type = cis_data_type_float;
                    tmpdata[1].value.asFloat = inst->instance.floatValue;
                    uri->resourceId = attributeA_floatValue;
                    cis_uri_update(uri);
                    cis_notify(context,uri,&tmpdata[1],mid,CIS_NOTIFY_CONTINUE,false);

                    tmpdata[2].type = cis_data_type_bool;
                    tmpdata[2].value.asBoolean = inst->instance.boolValue;
                    uri->resourceId = attributeA_boolValue;
                    cis_uri_update(uri);
                    cis_notify(context,uri,&tmpdata[2],mid,CIS_NOTIFY_CONTINUE,false);

                    tmpdata[3].type = cis_data_type_string;
                    tmpdata[3].asBuffer.length = utils_strlen(inst->instance.strValue);
                    tmpdata[3].asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                    uri->resourceId = attributeA_stringValue;
                    cis_uri_update(uri);//更新uri信息
                    cis_notify(context,uri,&tmpdata[3],mid,CIS_NOTIFY_CONTENT,false);
                }
            }
            break;
        case SAMPLE_OID_B:
            {
                if(uri->instanceId > SAMPLE_B_INSTANCE_COUNT){
                    return;
                }
                st_instance_b *inst = &g_dminstList_b[uri->instanceId];
                if(inst == NULL || inst->enabled == false){
                    return;
                }

                if(CIS_URI_IS_SET_RESOURCE(uri)){
                    if(uri->resourceId == attributeB_intValue)
                    {
                        value.type = cis_data_type_integer;
                        value.value.asInteger = inst->instance.intValue;
                    }
                    else if(uri->resourceId == attributeB_floatValue)
                    {
                        value.type = cis_data_type_float;
                        value.value.asFloat = inst->instance.floatValue;
                    }
                    else if(uri->resourceId == attributeB_stringValue)
                    {
                        value.type = cis_data_type_string;
                        value.asBuffer.length = utils_strlen(inst->instance.strValue);
                        value.asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                    }else{
                        return;
                    }

                    cis_notify(context,uri,&value,mid,CIS_NOTIFY_CONTENT,false);

                }else{
                    cis_data_t tmpdata[4];

                    tmpdata[0].type = cis_data_type_integer;
                    tmpdata[0].value.asInteger = inst->instance.intValue;
                    uri->resourceId = attributeB_intValue;
                    cis_uri_update(uri);
                    cis_notify(context,uri,&tmpdata[0],mid,CIS_NOTIFY_CONTINUE,false);


                    tmpdata[1].type = cis_data_type_float;
                    tmpdata[1].value.asFloat = inst->instance.floatValue;
                    uri->resourceId = attributeB_floatValue;
                    cis_uri_update(uri);
                    cis_notify(context,uri,&tmpdata[1],mid,CIS_NOTIFY_CONTINUE,false);

                    tmpdata[2].type = cis_data_type_string;
                    tmpdata[2].asBuffer.length = utils_strlen(inst->instance.strValue);
                    tmpdata[2].asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                    uri->resourceId = attributeB_stringValue;
                    cis_uri_update(uri);
                    cis_notify(context,uri,&tmpdata[2],mid,CIS_NOTIFY_CONTENT,false);
                }
            }
            break;
        }
    }
}



void prv_dm_readResponse(void* context,cis_uri_t* uri,cis_mid_t mid)
{
    uint8_t index;
    st_sample_object* object = NULL;
    cis_data_t value;
    for (index = 0;index< SAMPLE_OBJECT_MAX;index++)
    {
        if(g_objectListdm[index].oid ==  uri->objectId){
            object = &g_objectListdm[index];
        }
    }

    if(object == NULL){
        return;
    }


    if(!CIS_URI_IS_SET_INSTANCE(uri) && !CIS_URI_IS_SET_RESOURCE(uri)) // one object
    {
        switch(uri->objectId)
        {
        case SAMPLE_OID_A:
            {
                for(index=0;index<SAMPLE_A_INSTANCE_COUNT;index++)
                {                   
                    st_instance_a *inst = &g_dminstList_a[index];
                    if(inst != NULL &&  inst->enabled == true)
                    {
                        cis_data_t tmpdata[4];
                        tmpdata[0].type = cis_data_type_integer;
                        tmpdata[0].value.asInteger = inst->instance.intValue;
                        uri->instanceId = inst->instId;
                        uri->resourceId = attributeA_intValue;
                        cis_uri_update(uri);
                        cis_response(context,uri,&tmpdata[0],mid,CIS_RESPONSE_CONTINUE);

                        tmpdata[1].type = cis_data_type_float;
                        tmpdata[1].value.asFloat = inst->instance.floatValue;
                        uri->instanceId = inst->instId;
                        uri->resourceId = attributeA_floatValue;
                        cis_uri_update(uri);
                        cis_response(context,uri,&tmpdata[1],mid,CIS_RESPONSE_CONTINUE);

                        tmpdata[2].type = cis_data_type_bool;
                        tmpdata[2].value.asBoolean = inst->instance.boolValue;
                        uri->resourceId = attributeA_boolValue;
                        uri->instanceId = inst->instId;
                        cis_uri_update(uri);
                        cis_response(context,uri,&tmpdata[2],mid,CIS_RESPONSE_CONTINUE);

                        tmpdata[3].type = cis_data_type_string;
                        tmpdata[3].asBuffer.length = utils_strlen(inst->instance.strValue);
                        tmpdata[3].asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                        uri->resourceId = attributeA_stringValue;
                        uri->instanceId = inst->instId;
                        cis_uri_update(uri);
                        cis_response(context,uri,&tmpdata[3],mid,CIS_RESPONSE_CONTINUE);
                    }
                }
            }
            break;
        case SAMPLE_OID_B:
            {
                for(index=0;index<SAMPLE_A_INSTANCE_COUNT;index++)
                {                   
                    st_instance_b *inst = &g_dminstList_b[index];
                    if(inst != NULL &&  inst->enabled == true)
                    {
                        cis_data_t tmpdata[4];

                        tmpdata[0].type = cis_data_type_integer;
                        tmpdata[0].value.asInteger = inst->instance.intValue;
                        uri->resourceId = attributeB_intValue;
                        cis_uri_update(uri);
                        cis_response(context,uri,&tmpdata[0],mid,CIS_RESPONSE_CONTINUE);

                        tmpdata[1].type = cis_data_type_float;
                        tmpdata[1].value.asFloat = inst->instance.floatValue;
                        uri->resourceId = attributeB_floatValue;
                        cis_uri_update(uri);
                        cis_response(context,uri,&tmpdata[1],mid,CIS_RESPONSE_CONTINUE);

                        tmpdata[2].type = cis_data_type_string;
                        tmpdata[2].asBuffer.length = utils_strlen(inst->instance.strValue);
                        tmpdata[2].asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                        uri->resourceId = attributeB_stringValue;
                        cis_uri_update(uri);
                        cis_response(context,uri,&tmpdata[2],mid,CIS_RESPONSE_CONTINUE);
                    }
                }
            }
            break;
        }
        cis_response(context,NULL,NULL,mid,CIS_RESPONSE_READ);

    }else
    {
        switch(object->oid)
        {
        case SAMPLE_OID_A:
            {
                if(uri->instanceId > SAMPLE_A_INSTANCE_COUNT){
                    return;
                }
                st_instance_a *inst = &g_dminstList_a[uri->instanceId];
                if(inst == NULL || inst->enabled == false){
                    return;
                }

                if(CIS_URI_IS_SET_RESOURCE(uri)){
                    if(uri->resourceId == attributeA_intValue)
                    {
                        value.type = cis_data_type_integer;
                        value.value.asInteger = inst->instance.intValue;
                    }
                    else if(uri->resourceId == attributeA_floatValue)
                    {
                        value.type = cis_data_type_float;
                        value.value.asFloat = inst->instance.floatValue;
                    }
                    else if(uri->resourceId == attributeA_boolValue)
                    {
                        value.type = cis_data_type_bool;
                        value.value.asBoolean = inst->instance.boolValue;
                    }
                    else if(uri->resourceId == attributeA_stringValue)
                    {
                        value.type = cis_data_type_string;
                        value.asBuffer.length = utils_strlen(inst->instance.strValue);
                        value.asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                    }else{
                        return;
                    }

                    cis_response(context,uri,&value,mid,CIS_RESPONSE_READ);

                }else{
                    cis_data_t tmpdata[4];

                    tmpdata[0].type = cis_data_type_integer;
                    tmpdata[0].value.asInteger = inst->instance.intValue;
                    uri->resourceId = attributeA_intValue;
                    cis_uri_update(uri);
                    cis_response(context,uri,&tmpdata[0],mid,CIS_RESPONSE_CONTINUE);

                    tmpdata[1].type = cis_data_type_float;
                    tmpdata[1].value.asFloat = inst->instance.floatValue;
                    uri->resourceId = attributeA_floatValue;
                    cis_uri_update(uri);
                    cis_response(context,uri,&tmpdata[1],mid,CIS_RESPONSE_CONTINUE);

                    tmpdata[2].type = cis_data_type_bool;
                    tmpdata[2].value.asBoolean = inst->instance.boolValue;
                    uri->resourceId = attributeA_boolValue;
                    cis_uri_update(uri);
                    cis_response(context,uri,&tmpdata[2],mid,CIS_RESPONSE_CONTINUE);

                    tmpdata[3].type = cis_data_type_string;
                    tmpdata[3].asBuffer.length = utils_strlen(inst->instance.strValue);
                    tmpdata[3].asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                    uri->resourceId = attributeA_stringValue;
                    cis_uri_update(uri);
                    cis_response(context,uri,&tmpdata[3],mid,CIS_RESPONSE_READ);
                }
            }
            break;
        case SAMPLE_OID_B:
            {
                if(uri->instanceId > SAMPLE_B_INSTANCE_COUNT){
                    return;
                }
                st_instance_b *inst = &g_dminstList_b[uri->instanceId];
                if(inst == NULL || inst->enabled == false){
                    return;
                }

                if(CIS_URI_IS_SET_RESOURCE(uri)){
                    if(uri->resourceId == attributeB_intValue)
                    {
                        value.type = cis_data_type_integer;
                        value.value.asInteger = inst->instance.intValue;
                    }
                    else if(uri->resourceId == attributeB_floatValue)
                    {
                        value.type = cis_data_type_float;
                        value.value.asFloat = inst->instance.floatValue;
                    }
                    else if(uri->resourceId == attributeB_stringValue)
                    {
                        value.type = cis_data_type_string;
                        value.asBuffer.length = utils_strlen(inst->instance.strValue);
                        value.asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                    }else{
                        return;
                    }

                    cis_response(context,uri,&value,mid,CIS_RESPONSE_READ);

                }else{
                    cis_data_t tmpdata[4];

                    tmpdata[0].type = cis_data_type_integer;
                    tmpdata[0].value.asInteger = inst->instance.intValue;
                    uri->resourceId = attributeB_intValue;
                    cis_uri_update(uri);
                    cis_response(context,uri,&tmpdata[0],mid,CIS_RESPONSE_CONTINUE);
                    

                    tmpdata[1].type = cis_data_type_float;
                    tmpdata[1].value.asFloat = inst->instance.floatValue;
                    uri->resourceId = attributeB_floatValue;
                    cis_uri_update(uri);
                    cis_response(context,uri,&tmpdata[1],mid,CIS_RESPONSE_CONTINUE);

                    tmpdata[2].type = cis_data_type_string;
                    tmpdata[2].asBuffer.length = utils_strlen(inst->instance.strValue);
                    tmpdata[2].asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                    uri->resourceId = attributeB_stringValue;
                    cis_uri_update(uri);
                    cis_response(context,uri,&tmpdata[2],mid,CIS_RESPONSE_READ);
                }
            }
            break;
        }
    }
}


void prv_dm_discoverResponse(void* context,cis_uri_t* uri,cis_mid_t mid)
{
    uint8_t index;
    st_sample_object* object = NULL;

    for (index = 0;index< SAMPLE_OBJECT_MAX;index++)
    {
        if(g_objectListdm[index].oid ==  uri->objectId){
            object = &g_objectListdm[index];
        }
    }

    if(object == NULL){
        return;
    }


    switch(uri->objectId)
    {
    case SAMPLE_OID_A:
        {
            uri->objectId = URI_INVALID;
            uri->instanceId = URI_INVALID;
            uri->resourceId = attributeA_intValue;
            cis_uri_update(uri);
            cis_response(context,uri,NULL,mid,CIS_RESPONSE_CONTINUE);

            uri->objectId = URI_INVALID;
            uri->instanceId = URI_INVALID;
            uri->resourceId = attributeA_floatValue;
            cis_uri_update(uri);
            cis_response(context,uri,NULL,mid,CIS_RESPONSE_CONTINUE);

            uri->objectId = URI_INVALID;
            uri->instanceId = URI_INVALID;
            uri->resourceId = attributeA_boolValue;
            cis_uri_update(uri);
            cis_response(context,uri,NULL,mid,CIS_RESPONSE_CONTINUE);

            uri->objectId = URI_INVALID;
            uri->instanceId = URI_INVALID;
            uri->resourceId = attributeA_stringValue;
            cis_uri_update(uri);
            cis_response(context,uri,NULL,mid,CIS_RESPONSE_CONTINUE);

            uri->objectId = URI_INVALID;
            uri->instanceId = URI_INVALID;
            uri->resourceId = actionA_1;
            cis_uri_update(uri);
            cis_response(context,uri,NULL,mid,CIS_RESPONSE_CONTINUE);

        }
        break;
    case SAMPLE_OID_B:
        {
            uri->objectId = URI_INVALID;
            uri->instanceId = URI_INVALID;
            uri->resourceId = attributeB_intValue;
            cis_uri_update(uri);
            cis_response(context,uri,NULL,mid,CIS_RESPONSE_CONTINUE);

            uri->objectId = URI_INVALID;
            uri->instanceId = URI_INVALID;
            uri->resourceId = attributeB_floatValue;
            cis_uri_update(uri);
            cis_response(context,uri,NULL,mid,CIS_RESPONSE_CONTINUE);

            uri->objectId = URI_INVALID;
            uri->instanceId = URI_INVALID;
            uri->resourceId = attributeB_stringValue;
            cis_uri_update(uri);
            cis_response(context,uri,NULL,mid,CIS_RESPONSE_CONTINUE);

            uri->objectId = URI_INVALID;
            uri->instanceId = URI_INVALID;
            uri->resourceId = actionB_1;
            cis_uri_update(uri);
            cis_response(context,uri,NULL,mid,CIS_RESPONSE_CONTINUE);
        }
        break;
    }
    cis_response(context,NULL,NULL,mid,CIS_RESPONSE_DISCOVER);
}


void prv_dm_writeResponse          (void* context,cis_uri_t* uri,const cis_data_t* value,cis_attrcount_t count,cis_mid_t mid)
{

    uint8_t index;
    st_sample_object* object = NULL;
    

    if(!CIS_URI_IS_SET_INSTANCE(uri))
    {
        return;
    }

    for (index = 0;index< SAMPLE_OBJECT_MAX;index++)
    {
        if(g_objectListdm[index].oid ==  uri->objectId){
            object = &g_objectListdm[index];
        }
    }

    if(object == NULL){
        return;
    }


    switch(object->oid)
    {
    case SAMPLE_OID_A:
        {
            if(uri->instanceId > SAMPLE_A_INSTANCE_COUNT){
                return;
            }
            st_instance_a *inst = &g_dminstList_a[uri->instanceId];
            if(inst == NULL || inst->enabled == false){
                return;
            }

            for (int i=0;i<count;i++)
            {
                printf("write %d/%d/%d\n",uri->objectId,uri->instanceId,value[i].id);
                switch(value[i].id)
                {
                case attributeA_intValue:
                    {
                        if(value[i].type == cis_data_type_string){
                            inst->instance.intValue = atoi((const char*)value[i].asBuffer.buffer);
                        }else{
                            inst->instance.intValue = value[i].value.asInteger;
                        }
                    }
                    break;
                case attributeA_floatValue:
                    {
                        if(value[i].type == cis_data_type_string){
                            inst->instance.floatValue = atof((const char*)value[i].asBuffer.buffer);
                        }else{
                            inst->instance.floatValue = value[i].value.asFloat;
                        }
                    }
                    break;
                case attributeA_boolValue:
                    {
                        if(value[i].type == cis_data_type_string){
                            inst->instance.boolValue = atoi((const char*)value[i].asBuffer.buffer);
                        }else{
                            inst->instance.boolValue = value[i].value.asBoolean;
                        }
                    }
                    break;
                case  attributeA_stringValue:
                    {
                        memset(inst->instance.strValue,0,sizeof(inst->instance.strValue));
                        strncpy(inst->instance.strValue,(char*)value[i].asBuffer.buffer,value[i].asBuffer.length);
                    }
                    break;
                }
            }
        }
        break;
    case SAMPLE_OID_B:
        {
            if(uri->instanceId > SAMPLE_B_INSTANCE_COUNT){
                return;
            }
            st_instance_b *inst = &g_dminstList_b[uri->instanceId];
            if(inst == NULL || inst->enabled == false){
                return;
            }

            for (int i=0;i<count;i++)
            {
                printf("write %d/%d/%d\n",uri->objectId,uri->instanceId,value[i].id);
                switch(value[i].id)
                {
                case attributeB_intValue:
                    {
                        if(value[i].type == cis_data_type_opaque){
                            inst->instance.intValue = atoi((const char*)value[i].asBuffer.buffer);
                        }else{
                            inst->instance.intValue = value[i].value.asInteger;
                        }
                    }
                    break;
                case attributeB_floatValue:
                    {
                        if(value[i].type == cis_data_type_opaque){
                            inst->instance.floatValue = atof((const char*)value[i].asBuffer.buffer);
                        }else{
                            inst->instance.floatValue = value[i].value.asFloat;
                        }
                    }
                    break;
                case  attributeB_stringValue:
                    {
                        memset(inst->instance.strValue,0,sizeof(inst->instance.strValue));
                        strncpy(inst->instance.strValue,(char*)value[i].asBuffer.buffer,value[i].asBuffer.length);
                    }
                    break;
                }
            }

        }
        break;
    }

    cis_response(context,NULL,NULL,mid,CIS_RESPONSE_WRITE);
}


void prv_dm_execResponse              (void* context,cis_uri_t* uri,const uint8_t* value,uint32_t length,cis_mid_t mid)
{

    uint8_t index;
    st_sample_object* object = NULL;
    

    for (index = 0;index< SAMPLE_OBJECT_MAX;index++)
    {
        if(g_objectListdm[index].oid ==  uri->objectId){
            object = &g_objectListdm[index];
        }
    }

    if(object == NULL){
        return;
    }


    switch(object->oid)
    {
    case SAMPLE_OID_A:
        {
            if(uri->instanceId > SAMPLE_A_INSTANCE_COUNT){
                return;
            }
            st_instance_a *inst = &g_dminstList_a[uri->instanceId];
            if(inst == NULL || inst->enabled == false){
                return;
            }

            if(uri->resourceId == actionA_1)
            {
                /*
                *\call action;
                */
                printf("exec actionA_action\n");

                cis_response(context,NULL,NULL,mid,CIS_RESPONSE_EXECUTE);
            }else{
                return;
            }
        }
        break;
    case SAMPLE_OID_B:
        {
            if(uri->instanceId > SAMPLE_B_INSTANCE_COUNT){
                return;
            }
            st_instance_b *inst = &g_dminstList_b[uri->instanceId];
            if(inst == NULL || inst->enabled == false){
                return;
            }

            if(uri->resourceId == actionB_1)
            {
                /*
                *\call action;
                */
                 printf("exec actionB_1\n");
                 cis_response(context,NULL,NULL,mid,CIS_RESPONSE_EXECUTE);
            }else{
                return;
            }
        }
        break;
    }
};

void prv_dm_paramsResponse         (void* context,cis_uri_t* uri,cis_observe_attr_t parameters,cis_mid_t mid)
{
    uint8_t index;
    st_sample_object* object = NULL;


    if(CIS_URI_IS_SET_RESOURCE(uri)){
        printf("prv_params:(%d/%d/%d)\n",uri->objectId,uri->instanceId,uri->resourceId);
    }

    if(!CIS_URI_IS_SET_INSTANCE(uri))
    {
        return;
    }

    for (index = 0;index< SAMPLE_OBJECT_MAX;index++)
    {
        if(g_objectListdm[index].oid ==  uri->objectId){
            object = &g_objectListdm[index];
        }
    }

    if(object == NULL){
        return;
    }

    /*set parameter to observe resource*/
    /*do*/


    cis_response(context,NULL,NULL,mid,CIS_RESPONSE_OBSERVE_PARAMS);

}



static cis_data_t* prv_dataDup(const cis_data_t* value,cis_attrcount_t attrcount)
{
    cis_data_t* newData;
    newData =(cis_data_t*)cis_malloc(attrcount * sizeof(cis_data_t));
    if(newData == NULL)
    {
        return NULL;
    }
    cis_attrcount_t index;
    for (index =0;index<attrcount;index++)
    {
        newData[index].id = value[index].id;
        newData[index].type = value[index].type;
        newData[index].asBuffer.length = value[index].asBuffer.length;
        newData[index].asBuffer.buffer = (uint8_t*)cis_malloc(value[index].asBuffer.length);
		cissys_assert(newData[index].asBuffer.buffer !=NULL);
        memcpy(newData[index].asBuffer.buffer,value[index].asBuffer.buffer,value[index].asBuffer.length);


        memcpy(&newData[index].value.asInteger,&value[index].value.asInteger,sizeof(newData[index].value));
    }
    return newData;
}


//////////////////////////////////////////////////////////////////////////
cis_coapret_t dm_onRead		        (void* context,cis_uri_t* uri,cis_mid_t mid)
{
    struct st_callback_info* newNode = (struct st_callback_info*)cis_malloc(sizeof(struct st_callback_info));
	cissys_assert(newNode !=NULL);
    newNode->next = NULL;
    newNode->flag = SAMPLE_CALLBACK_READ;
    newNode->mid = mid;
    newNode->uri = *uri;
    g_dmcallbackList = (struct st_callback_info*)CIS_LIST_ADD(g_dmcallbackList,newNode);

    printf("cis_onRead:(%d/%d/%d)\n",uri->objectId,uri->instanceId,uri->resourceId);

   
	return CIS_CALLBACK_CONFORM;
}

cis_coapret_t dm_onDiscover(void* context,cis_uri_t* uri,cis_mid_t mid)
{

    struct st_callback_info* newNode = (struct st_callback_info*)cis_malloc(sizeof(struct st_callback_info));
	cissys_assert(newNode !=NULL);
    newNode->next = NULL;
    newNode->flag = SAMPLE_CALLBACK_DISCOVER;
    newNode->mid = mid;
    newNode->uri = *uri;
    g_dmcallbackList = (struct st_callback_info*)CIS_LIST_ADD(g_dmcallbackList,newNode);

    printf("cis_onDiscover:(%d/%d/%d)\n",uri->objectId,uri->instanceId,uri->resourceId);

	return CIS_CALLBACK_CONFORM;
}

cis_coapret_t dm_onWrite		        (void* context,cis_uri_t* uri,const cis_data_t* value,cis_attrcount_t attrcount,cis_mid_t mid)
{

    st_sample_object* object = NULL;

    if(CIS_URI_IS_SET_RESOURCE(uri)){
        printf("cis_onWrite:(%d/%d/%d)\n",uri->objectId,uri->instanceId,uri->resourceId);
    }
    else{
        printf("cis_onWrite:(%d/%d)\n",uri->objectId,uri->instanceId);
    }
    printf("rcv write %s\r\n",value->asBuffer.buffer);

    struct st_callback_info* newNode = (struct st_callback_info*)cis_malloc(sizeof(struct st_callback_info));
  	cissys_assert(newNode !=NULL);
    newNode->next = NULL;
    newNode->flag = SAMPLE_CALLBACK_WRITE;
    newNode->mid = mid;
    newNode->uri = *uri;
    newNode->param.asWrite.count = attrcount;
    newNode->param.asWrite.value = prv_dataDup(value,attrcount);
    g_dmcallbackList = (struct st_callback_info*)CIS_LIST_ADD(g_dmcallbackList,newNode);
//    printf("type is %d\r\n",value->type);

    return CIS_CALLBACK_CONFORM;
   
}


cis_coapret_t dm_onExec              (void* context,cis_uri_t* uri,const uint8_t* value,uint32_t length,cis_mid_t mid)
{
    if(CIS_URI_IS_SET_RESOURCE(uri)){
        printf("cis_onExec:(%d/%d/%d)\n",uri->objectId,uri->instanceId,uri->resourceId);
    }
    else{
        return CIS_CALLBACK_METHOD_NOT_ALLOWED;
    }

    if(!CIS_URI_IS_SET_INSTANCE(uri))
    {
        return CIS_CALLBACK_BAD_REQUEST;
    }

    struct st_callback_info* newNode = (struct st_callback_info*)cis_malloc(sizeof(struct st_callback_info));
	  cissys_assert(newNode !=NULL);
    newNode->next = NULL;
    newNode->flag = SAMPLE_CALLBACK_EXECUTE;
    newNode->mid = mid;
    newNode->uri = *uri;
    newNode->param.asExec.buffer = (uint8_t*)cis_malloc(length);
	  cissys_assert(newNode->param.asExec.buffer !=NULL);
    newNode->param.asExec.length = length;
    memcpy(newNode->param.asExec.buffer,value,length);
    g_dmcallbackList = (struct st_callback_info*)CIS_LIST_ADD(g_dmcallbackList,newNode);
    return CIS_CALLBACK_CONFORM;
}


cis_coapret_t dm_onObserve         (void* context,cis_uri_t* uri,bool flag,cis_mid_t mid)
{
    st_sample_object* object = NULL;

    printf("cis_onObserve mid:%d uri:(%d/%d/%d)\n",mid,uri->objectId,uri->instanceId,uri->resourceId);

    if(!CIS_URI_IS_SET_INSTANCE(uri))
    {
        return CIS_CALLBACK_BAD_REQUEST;
    }
   
    struct st_callback_info* newNode = (struct st_callback_info*)cis_malloc(sizeof(struct st_callback_info));
		cissys_assert(newNode !=NULL);
    newNode->next = NULL;
    newNode->flag = SAMPLE_CALLBACK_OBSERVE;
    newNode->mid = mid;
    newNode->uri = *uri;
    newNode->param.asObserve.flag = flag;
    
    g_dmcallbackList = (struct st_callback_info*)CIS_LIST_ADD(g_dmcallbackList,newNode);

    return CIS_CALLBACK_CONFORM;
}

cis_coapret_t dm_onParams         (void* context,cis_uri_t* uri,cis_observe_attr_t parameters,cis_mid_t mid)
{
    if(CIS_URI_IS_SET_RESOURCE(uri)){
        printf("cis_on_params:(%d/%d/%d)\n",uri->objectId,uri->instanceId,uri->resourceId);
    }
    
    if(!CIS_URI_IS_SET_INSTANCE(uri))
    {
        return CIS_CALLBACK_BAD_REQUEST;
    }

    struct st_callback_info* newNode = (struct st_callback_info*)cis_malloc(sizeof(struct st_callback_info));
   	cissys_assert(newNode !=NULL);
    newNode->next = NULL;
    newNode->flag = SAMPLE_CALLBACK_SETPARAMS;
    newNode->mid = mid;
    newNode->uri = *uri;
    newNode->param.asObserveParam.params = parameters;
    g_dmcallbackList = (struct st_callback_info*)CIS_LIST_ADD(g_dmcallbackList,newNode);

    return CIS_CALLBACK_CONFORM;
}
#include "cis_internals.h"
#include "cis_api.h"
void dm_onEvent  (void* context,cis_evt_t eid,void* param)
{
		st_context_t * ctx ;
		printf("Dm dm_onEvent\r\n");
    switch(eid)
    {
        case CIS_EVENT_RESPONSE_FAILED:
            printf("cis_on_event response failed mid:%d\n",(int32_t)param);
            break;
        case CIS_EVENT_NOTIFY_FAILED:
            printf("cis_on_event notify failed mid:%d\n",(int32_t)param);
            break;
        case CIS_EVENT_UPDATE_NEED:
            printf("cis_on_event need to update,reserve time:%ds\n",(int32_t)param);
            cis_update_reg(g_dmcontext,LIFETIME_INVALID,false);
        default:
						ctx = (st_context_t*)context;
            printf("dm cis_on_event id:%s,%d\n",ctx->server->host,eid);
            break;
    }
    
}
#endif