#include "../cis_api.h"
#include "../cis_internals.h"
#include "std_object.h"

extern cis_coapret_t std_security_read(st_context_t * contextP,cis_iid_t instanceId,int * numDataP,st_data_t ** dataArrayP,st_object_t * objectP);
extern cis_coapret_t std_security_write(st_context_t * contextP,cis_iid_t instanceId,int numData,st_data_t * dataArray,st_object_t * objectP);
extern cis_coapret_t std_security_discover(st_context_t * contextP,uint16_t instanceId,int * numDataP,st_data_t ** dataArrayP,st_object_t * objectP);

#if CIS_ENABLE_UPDATE
extern cis_coapret_t std_device_read(st_context_t * contextP,cis_iid_t instanceId,int * numDataP,st_data_t ** dataArrayP,st_object_t * objectP);
extern cis_coapret_t std_device_write(st_context_t * contextP,cis_iid_t instanceId,int numData,st_data_t * dataArray,st_object_t * objectP);
extern cis_coapret_t std_device_discover(st_context_t * contextP,uint16_t instanceId,int * numDataP,st_data_t ** dataArrayP,st_object_t * objectP);

extern cis_coapret_t std_conn_moniter_read(st_context_t * contextP,cis_iid_t instanceId,int * numDataP,st_data_t ** dataArrayP,st_object_t * objectP);
extern cis_coapret_t std_conn_moniter_write(st_context_t * contextP,cis_iid_t instanceId,int numData,st_data_t * dataArray,st_object_t * objectP);
extern cis_coapret_t std_conn_moniter_discover(st_context_t * contextP,uint16_t instanceId,int * numDataP,st_data_t ** dataArrayP,st_object_t * objectP);

extern cis_coapret_t std_firmware_read(st_context_t * contextP,cis_iid_t instanceId,int * numDataP,st_data_t ** dataArrayP,st_object_t * objectP);
extern cis_coapret_t std_firmware_write(st_context_t * contextP,cis_iid_t instanceId,int numData,st_data_t * dataArray,st_object_t * objectP);
extern cis_coapret_t std_firmware_execute(st_context_t * contextP,uint16_t instanceId,uint16_t resourceId,uint8_t * buffer,int length,st_object_t * objectP);
extern cis_coapret_t std_firmware_discover(st_context_t * contextP,uint16_t instanceId,int * numDataP,st_data_t ** dataArrayP,st_object_t * objectP);
#endif

#if CIS_ENABLE_UPDATE
#define PROLOAD_OBJECT_NUMBER  (4)
#else
#define PROLOAD_OBJECT_NUMBER  (1)
#endif
static const struct st_std_object_callback_mapping std_object_callback_mapping[PROLOAD_OBJECT_NUMBER] ={
    {CIS_SECURITY_OBJECT_ID,std_security_read,std_security_write,NULL,std_security_discover},
#if CIS_ENABLE_UPDATE
	{CIS_DEVICE_OBJECT_ID,std_device_read,std_device_write,NULL,std_device_discover},
	{CIS_CONNECTIVITY_OBJECT_ID,std_conn_moniter_read,std_conn_moniter_write,NULL,std_conn_moniter_discover},
	{CIS_FIRMWARE_OBJECT_ID,std_firmware_read,std_firmware_write,std_firmware_execute,std_firmware_discover},
#endif
};

cis_list_t* std_object_get_securitys(st_context_t * contextP)
{
    return contextP->instSecurity;
}

cis_list_t* std_object_put_securitys(st_context_t * contextP,cis_list_t* targetP)
{
	contextP->instSecurity = CIS_LIST_ADD(contextP->instSecurity, targetP);
    return contextP->instSecurity;
}

void std_object_remove_securitys(st_context_t * contextP,cis_list_t* targetP)
{
	 contextP->instSecurity = CIS_LIST_RM(contextP->instSecurity,targetP->id,NULL);
}

#if CIS_ENABLE_UPDATE
cis_list_t* std_object_get_firmware(st_context_t * contextP,cis_iid_t instanceId)
{
	contextP->firmware_inst = CIS_LIST_FIND(contextP->firmware_inst, instanceId);
	return contextP->firmware_inst;
}

cis_list_t* std_object_get_conn(st_context_t * contextP,cis_iid_t instanceId)
{
	contextP->conn_inst = CIS_LIST_FIND(contextP->conn_inst, instanceId);
	return contextP->conn_inst;
}

cis_list_t* std_object_get_device(st_context_t * contextP,cis_iid_t instanceId)
{
	contextP->device_inst = CIS_LIST_FIND(contextP->device_inst, instanceId);
	return contextP->device_inst;
}

cis_list_t* std_object_put_device(st_context_t * contextP,cis_list_t* targetP)
{
	contextP->device_inst = CIS_LIST_ADD(contextP->device_inst, targetP);
	return contextP->device_inst;
}

cis_list_t* std_object_put_conn(st_context_t * contextP,cis_list_t* targetP)
{
	contextP->conn_inst = CIS_LIST_ADD(contextP->conn_inst, targetP);
	return contextP->conn_inst;
}

cis_list_t* std_object_put_firmware(st_context_t * contextP,cis_list_t* targetP)
{
	contextP->firmware_inst = CIS_LIST_ADD(contextP->firmware_inst, targetP);
	return contextP->firmware_inst;
}

void std_object_remove_firmware(st_context_t * contextP,cis_list_t* targetP)
{
	 contextP->firmware_inst = CIS_LIST_RM(contextP->firmware_inst,targetP->id,NULL);
}

void std_object_remove_conn(st_context_t * contextP,cis_list_t* targetP)
{
	contextP->conn_inst = CIS_LIST_RM(contextP->conn_inst,targetP->id,NULL);
}

void std_object_remove_device(st_context_t * contextP,cis_list_t* targetP)
{
	 contextP->device_inst = CIS_LIST_RM(contextP->device_inst,targetP->id,NULL);
}
#endif


cis_coapret_t std_object_read_handler(st_context_t * contextP,cis_iid_t instanceId,int * numDataP, st_data_t ** dataArrayP,st_object_t * objectP)
{
    int i = 0;
    for(i=0;i<sizeof(std_object_callback_mapping) / sizeof(struct st_std_object_callback_mapping);i++)
    {
        if(std_object_callback_mapping[i].onRead == NULL)continue;
        if(objectP->objID == std_object_callback_mapping[i].stdObjectId){
            std_object_callback_mapping[i].onRead(contextP,instanceId,numDataP,dataArrayP,objectP);
            return COAP_205_CONTENT;
        }
    }

    return COAP_503_SERVICE_UNAVAILABLE;
}

cis_coapret_t std_object_exec_handler(st_context_t * contextP,cis_iid_t instanceId,uint16_t resourceId,uint8_t * buffer,int length,st_object_t * objectP)
{
    int i = 0;
    for(i=0;i<sizeof(std_object_callback_mapping) / sizeof(struct st_std_object_callback_mapping);i++)
    {
        if(std_object_callback_mapping[i].onExec == NULL)continue;
        if(objectP->objID == std_object_callback_mapping[i].stdObjectId){
            std_object_callback_mapping[i].onExec(contextP,instanceId,resourceId,buffer,length,objectP);
            return CIS_COAP_204_CHANGED;
        }
    }

    return COAP_503_SERVICE_UNAVAILABLE;
}

cis_coapret_t std_object_write_handler(st_context_t * contextP,cis_iid_t instanceId,int numData,st_data_t * dataArray,st_object_t * objectP)
{
    int i = 0;
    for(i=0;i<sizeof(std_object_callback_mapping) / sizeof(struct st_std_object_callback_mapping);i++)
    {
        if(std_object_callback_mapping[i].onWrite == NULL)continue;
        if(objectP->objID == std_object_callback_mapping[i].stdObjectId){
            std_object_callback_mapping[i].onWrite(contextP,instanceId,numData,dataArray,objectP);
            return CIS_COAP_204_CHANGED;
        }
    }
    return COAP_503_SERVICE_UNAVAILABLE;
}

cis_coapret_t std_object_discover_handler(st_context_t * contextP,cis_iid_t instanceId,int * numDataP, st_data_t ** dataArrayP,st_object_t * objectP)
{
	int i = 0;
	for(i=0;i<sizeof(std_object_callback_mapping) / sizeof(struct st_std_object_callback_mapping);i++)
	{
		if(std_object_callback_mapping[i].onDiscover == NULL)continue;
		if(objectP->objID == std_object_callback_mapping[i].stdObjectId){
			std_object_callback_mapping[i].onDiscover(contextP,instanceId,numDataP,dataArrayP,objectP);
			return COAP_205_CONTENT;
		}
	}

	return COAP_503_SERVICE_UNAVAILABLE;
}

cis_coapret_t std_object_writeInstance(st_context_t * contextP,st_uri_t * uriP, st_data_t * dataP){
    st_object_t * targetP;
    cis_coapret_t result;

    targetP = NULL;
    if(uriP->objectId == std_object_security){
        targetP = (st_object_t *)CIS_LIST_FIND(contextP->objectList, uriP->objectId);
    }

    if (NULL == targetP) return CIS_COAP_404_NOT_FOUND;
    result = std_object_write_handler(contextP,uriP->instanceId,1,dataP,targetP);
    return result;
}