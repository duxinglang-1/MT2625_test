#include "../cis_api.h"
#include "../cis_internals.h"
#include "std_object.h"

#define CIS_SECURITY_URI_ID                 0
#define CIS_SECURITY_BOOTSTRAP_ID           1
#define CIS_SECURITY_MODE_ID                2
#define CIS_SECURITY_SHORT_SERVER_ID        10
#define CIS_SECURITY_HOLD_OFF_ID            11

#include "memory_attribute.h"
ATTR_ZIDATA_IN_RETSRAM char cis_server_host[20];



typedef struct st_security_instance
{
    struct st_security_instance * next;        // matches lwm2m_list_t::next
    cis_listid_t                 instanceId;  // matches lwm2m_list_t::id
    char *                       host;        // ip address;
    bool                         isBootstrap;
    uint16_t                     shortID;
    uint32_t                     clientHoldOffTime;
    uint8_t                      securityMode;
} security_instance_t;


static uint8_t prv_get_value(st_context_t * contextP,
	                         st_data_t * dataP,
                             security_instance_t * targetP)
{
    switch (dataP->id)
    {
    case CIS_SECURITY_URI_ID:
        data_encode_string(targetP->host, dataP);
        return COAP_205_CONTENT;

    case CIS_SECURITY_BOOTSTRAP_ID:
        data_encode_bool(targetP->isBootstrap, dataP);
        return COAP_205_CONTENT;

    case CIS_SECURITY_SECURITY_ID:
        data_encode_int(targetP->securityMode, dataP);
        return COAP_205_CONTENT;
    case CIS_SECURITY_SHORT_SERVER_ID:
        data_encode_int(targetP->shortID, dataP);
        return COAP_205_CONTENT;

    case CIS_SECURITY_HOLD_OFF_ID:
        data_encode_int(targetP->clientHoldOffTime, dataP);
        return COAP_205_CONTENT;
    default:
        return COAP_404_NOT_FOUND;
    }
}


static security_instance_t * prv_security_find(st_context_t * contextP,cis_iid_t instanceId)
{
    security_instance_t * targetP;
    targetP = (security_instance_t *)cis_list_find(std_object_get_securitys(contextP), instanceId);
    if (NULL != targetP)
    {
        return targetP;
    }

    return NULL;
}



cis_coapret_t std_security_read(st_context_t * contextP,cis_iid_t instanceId,
                                 int * numDataP,
                                 st_data_t ** dataArrayP,
                                 st_object_t * objectP)
{
    security_instance_t * targetP;
    uint8_t result;
    int i;

    targetP = prv_security_find(contextP,instanceId);
    if (NULL == targetP) return COAP_404_NOT_FOUND;

    // is the server asking for the full instance ?
    if (*numDataP == 0)
    {
        uint16_t resList[] = {CIS_SECURITY_URI_ID,
                              CIS_SECURITY_BOOTSTRAP_ID,
                              CIS_SECURITY_SHORT_SERVER_ID,
                              CIS_SECURITY_HOLD_OFF_ID,};
        int nbRes = sizeof(resList)/sizeof(uint16_t);

        *dataArrayP = data_new(nbRes);
        if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
        *numDataP = nbRes;
        for (i = 0 ; i < nbRes ; i++)
        {
            (*dataArrayP)[i].id = resList[i];
        }
    }

    i = 0;
    do
    {
        result = prv_get_value(contextP,(*dataArrayP) + i, targetP);
        i++;
    } while (i < *numDataP && result == COAP_205_CONTENT);

    return result;
}

cis_coapret_t std_security_write(st_context_t * contextP,
								 cis_iid_t instanceId,
                                 int numData,
                                 st_data_t * dataArray,
                                 st_object_t * objectP)
{
    security_instance_t * targetP;
    int i;
    uint8_t result = COAP_204_CHANGED;

    targetP = prv_security_find(contextP,instanceId);
    if (NULL == targetP)
    {
        return COAP_404_NOT_FOUND;
    }

    i = 0;
    do {
        switch (dataArray[i].id)
        {
        case CIS_SECURITY_URI_ID:
            if (targetP->host != NULL) cis_free(targetP->host);
            targetP->host = (char *)cis_malloc(dataArray[i].asBuffer.length + 1);
            if (targetP->host != NULL)
            {
				cis_memset(targetP->host, 0, dataArray[i].asBuffer.length + 1);
                utils_stringCopy(targetP->host, dataArray[i].asBuffer.length-URI_HEADER_HOST_LEN-URI_TAILER_HOST_LEN,(char*)dataArray[i].asBuffer.buffer+URI_HEADER_HOST_LEN);
                cis_memcpy(cis_server_host, targetP->host, 20);
                result = COAP_204_CHANGED;
            }
            else
            {
                result = COAP_500_INTERNAL_SERVER_ERROR;
            }
            break;

        case CIS_SECURITY_SECURITY_ID:
        {
            int64_t value;

            if (1 == data_decode_int(dataArray + i, &value))
            {
                if (value >= 0 && value <= 3)
                {
                    targetP->securityMode = (uint8_t)value;
                    result = COAP_204_CHANGED;
                }
                else
                {
                    result = COAP_406_NOT_ACCEPTABLE;
                }
            }
            else
            {
                result = COAP_400_BAD_REQUEST;
            }
        }
        break;
        case CIS_SECURITY_BOOTSTRAP_ID:
            if (1 == data_decode_bool(dataArray + i, &(targetP->isBootstrap)))
            {
                result = COAP_204_CHANGED;
            }
            else
            {
                result = COAP_400_BAD_REQUEST;
            }
            break;

        case CIS_SECURITY_SHORT_SERVER_ID:
        {
            int64_t value;

            if (1 == data_decode_int(dataArray + i, &value))
            {
                if (value >= 0 && value <= 0xFFFF)
                {
                    targetP->shortID = (uint16_t)value;
                    result = COAP_204_CHANGED;
                }
                else
                {
                    result = COAP_406_NOT_ACCEPTABLE;
                }
            }
            else
            {
                result = COAP_400_BAD_REQUEST;
            }
        }
        break;

        case CIS_SECURITY_HOLD_OFF_ID:
        {
            int64_t value;

            if (1 == data_decode_int(dataArray + i, &value))
            {
                if (value >= 0 && value <= 0xFFFF)
                {
                    targetP->clientHoldOffTime = (uint32_t)value;
                    result = COAP_204_CHANGED;
                }
                else
                {
                    result = COAP_406_NOT_ACCEPTABLE;
                }
            }
            else
            {
                result = COAP_400_BAD_REQUEST;
            }
            break;
        }
        default:
            return COAP_404_NOT_FOUND;
        }
        i++;
    } while (i < numData && result == COAP_204_CHANGED);

    return result;
}


cis_coapret_t std_security_discover(st_context_t * contextP,
								    uint16_t instanceId,
								    int * numDataP,
							        st_data_t ** dataArrayP,
								    st_object_t * objectP)
{
	return COAP_404_NOT_FOUND;
}

bool std_security_create(st_context_t * contextP,
						 int instanceId,
					     const char* serverHost,
                         bool isBootstrap,
                         st_object_t * securityObj)
{
    security_instance_t * instSecurity=NULL;
    security_instance_t * targetP = NULL;
    cis_instcount_t instBytes = 0;
    cis_instcount_t instCount = 0;
    cis_iid_t instIndex;
    if (NULL == securityObj)
    {
        return false;   
    }

    // Manually create a hard-code instance
    targetP = (security_instance_t *)cis_malloc(sizeof(security_instance_t));
    if (NULL == targetP)
    {
        return false;
    }

    cis_memset(targetP, 0, sizeof(security_instance_t));
    targetP->instanceId = (uint16_t)instanceId;
    targetP->host = (char*)cis_malloc(utils_strlen(serverHost)+1); 
	if(targetP->host == NULL)
	{
		cis_free(targetP);
		return false;
	}
    cis_memset(targetP->host,0,utils_strlen(serverHost)+1);
    utils_stringCopy(targetP->host,utils_strlen(serverHost),serverHost);

    targetP->isBootstrap = isBootstrap;
    targetP->shortID = 0;
    targetP->clientHoldOffTime = 10;

    instSecurity = (security_instance_t * )std_object_put_securitys(contextP,(cis_list_t*)targetP);
    
    instCount = (cis_instcount_t)CIS_LIST_COUNT(instSecurity);
    if(instCount == 0)
    {
        cis_free(targetP->host);//yyl 2018/5/31
        cis_free(targetP);//yyl 2018/5/31
        return false;
    }

    /*first security object instance
     *don't malloc instance bitmap ptr*/
    if(instCount == 1)
    {
        return true;
    }


    securityObj->instBitmapCount = instCount;
    instBytes = (instCount - 1) / 8 + 1;
    if(securityObj->instBitmapBytes < instBytes){
        if(securityObj->instBitmapBytes != 0 && securityObj->instBitmapPtr != NULL)
        {
            cis_free(securityObj->instBitmapPtr);//yyl 2018/5/31
        }
        securityObj->instBitmapPtr = (uint8_t*)cis_malloc(instBytes);
        securityObj->instBitmapBytes = instBytes;
    }
    cis_memset(securityObj->instBitmapPtr,0,instBytes);
    
    for (instIndex = 0;instIndex < instCount;instIndex++)
    {
        uint16_t instBytePos = (uint16_t)instSecurity->instanceId / 8;
        uint16_t instByteOffset = 7 - (instSecurity->instanceId % 8);
        securityObj->instBitmapPtr[instBytePos] += 0x01 << instByteOffset;
        instSecurity = instSecurity->next;
    }


    return true;
}



void std_security_clean(st_context_t * contextP)
{
    security_instance_t * deleteInst;
    security_instance_t * securityInstance = NULL;
	securityInstance = (security_instance_t *)(contextP->instSecurity);
	
    while (securityInstance != NULL)
    {
        deleteInst = securityInstance;
        securityInstance = securityInstance->next;

        std_object_remove_securitys(contextP,(cis_list_t*)deleteInst);
        if (NULL != deleteInst->host)
        {
            cis_free(deleteInst->host);
        }
        
        cis_free(deleteInst);
    }
}



char * std_security_get_host(st_context_t * contextP,cis_iid_t InstanceId)
{
    security_instance_t * targetP = prv_security_find(contextP,InstanceId);

    if (NULL != targetP)
    {
        return utils_strdup(targetP->host);
    }

    return NULL;
}