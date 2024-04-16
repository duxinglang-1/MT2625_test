/*******************************************************************************
 *
 * Copyright (c) 2014 Bosch Software Innovations GmbH, Germany.
 *
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
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *
 ******************************************************************************/
/*! \file
  LWM2M object "Watermeter" implementation

  \author Mingliang Xu
*/

/*
 *  Object     |      | Multiple  |     | Description                                                                                                |
 *  Name       |  ID  | Instances |Mand.|                                                                                                            |
 *-------------+------+-----------+-----+------------------------------------------------------------------------------------------------------------+
 *  Barometer  | 3315 |    Yes    |  No |  This IPSO object should be used with an air pressure sensor to report a barometer measurement. It also    |
 *             |      |           |     |  provides resources for minimum/maximum measured values and the minimum/maximum range that can be measured |
 *             |      |           |     |  by the barometer sensor. An example measurement unit is kPa (ucum:kPa)                                    |
 *
 *  Resources:
 *  Name               | ID  | Oper.|Instances|Mand.|  Type  | Range | Units    | Description                                                        |
 * --------------------+-----+------+---------+-----+--------+-------+----------+--------------------------------------------------------------------+
 *  Sensor Value       | 5700|  R   |Multiple | Yes | Float  |       | ref 5701 | Last or Current Measured Value from the Sensor.                    |
 *  Min Measured Value | 5601|  R   | Single  | No  | Float  |       | ref 5701 | The minimum value measured by the sensor since power ON or reset.  |
 *  Max Measured Value | 5602|  R   | Single  | No  | Float  |       | ref 5701 | The maximum value measured by the sensor since power ON or reset.  |
 *  Min Range Value    | 5603|  R   | Single  | No  | Float  |       | ref 5701 | The minimum value that can be measured by the sensor               |
 *  Max Range Value    | 5604|  R   | Single  | No  | Float  |       | ref 5701 | The maximum value that can be measured by the sensor               |
 *  Sensor Units       | 5701|  R   | Single  | No  | String |       |          | If present, the type of sensor defined as the UCUM Unit Definition |
 *                     |     |      |         |     |        |       |          | e.g. "Cel" for Temperature in Celcius.                             |
 *                     |     |      |         |     |        |       |          | Pa - pascal - pressure                                             |
 *                     |     |      |         |     |        |       |          | [in_i'H2O] - inch of water column - pressure                       |
 *                     |     |      |         |     |        |       |          | [in_i'Hg] - inch of mercury column - pressure                      |
 *                     |     |      |         |     |        |       |          | [psi] - pound per sqare inch - pressure                            |
 *                     |     |      |         |     |        |       |          | atm - standard atmosphere - pressure                               |
 *                     |     |      |         |     |        |       |          | att - technical atmosphere - pressure                              |
 *                     |     |      |         |     |        |       |          | bar - bar - pressure                                               |
 *                     |     |      |         |     |        |       |          | m[H2O] - meter of water column - pressure                          |
 *                     |     |      |         |     |        |       |          | m[Hg] - meter of mercury column - pressure                         |
 *  Reset Min and Max  | 5605|  E   | Single  | No  | Opaque |       |          | Reset the Min and Max Measured Values to Current Value.            |
 *   Measured Values   |     |      |         |     |        |       |          |
 */

#include "liblwm2m.h"
#include "internals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include "FreeRTOS.h"
#include "semphr.h"

#ifdef LWM2M_CLIENT_MODE


// ---- private "object watermeter" specific defines ----
// Resource Id's:
#define RES_M_WATERMETER_DATA           0
#define RES_O_WATERMETER_PRIORITY       1
#define RES_O_WATERMETER_CREATION_TIME  2
#define RES_O_WATERMETER_DESCRIPTION    3
#define RES_O_WATERMETER_FORMAT         4
#define RES_O_WATERMETER_APP_ID         5


#define STR_OCTETS   (32)


typedef struct
{
    uint8_t  *data;                     /* RW */
    uint32_t data_length;
    uint8_t  priority;                  /* RW */
    uint32_t time;                      /* RW */
    char     desc[STR_OCTETS + 1];      /* RW */
    char     format[STR_OCTETS + 1];    /* RW */
    uint16_t app_id;                    /* RW */
    SemaphoreHandle_t mutex_handle;
} watermeter_data_t;

/**
implementation for all read-able resources
*/
static uint8_t prv_watermeter_res2tlv(lwm2m_data_t* dataP,
                                watermeter_data_t* locDataP)
{
    //-------------------------------------------------------------------- JH --
    uint8_t ret = COAP_205_CONTENT;
    switch (dataP->id)     // watermeter resourceId
    {
    case RES_M_WATERMETER_DATA:
        lwm2m_data_encode_opaque(locDataP->data, locDataP->data_length, dataP);
        if (locDataP->data_length == 0) ret = COAP_NO_ERROR;
        break;
    case RES_O_WATERMETER_PRIORITY:
        lwm2m_data_encode_int(locDataP->priority, dataP);
        break;
    case RES_O_WATERMETER_CREATION_TIME:
        lwm2m_data_encode_int(locDataP->time, dataP);
        break;
    case RES_O_WATERMETER_DESCRIPTION:
        lwm2m_data_encode_string(locDataP->desc, dataP);
        break;
    case RES_O_WATERMETER_FORMAT:
        lwm2m_data_encode_string(locDataP->format, dataP);
        break;
    case RES_O_WATERMETER_APP_ID:
        lwm2m_data_encode_int(locDataP->app_id, dataP);
        break;
    default:
        ret = COAP_404_NOT_FOUND;
        break;
    }

    return ret;
}


/**
  * Implementation (callback-) function of reading object resources. For whole
  * object, single resources or a sequence of resources
  * see 3GPP TS 23.032 V11.0.0(2012-09) page 23,24.
  * implemented for: HORIZONTAL_VELOCITY_WITH_UNCERTAINT
  * @param objInstId    in,     instances ID of the barometer object to read
  * @param numDataP     in/out, pointer to the number of resource to read. 0 is the
  *                             exception for all readable resource of object instance
  * @param tlvArrayP    in/out, TLV data sequence with initialized resource ID to read
  * @param objectP      in,     private barometer data structure
  */
static uint8_t prv_watermeter_read(uint16_t objInstId,
                                 int*  numDataP,
                                 lwm2m_data_t** tlvArrayP,
                                 lwm2m_object_t* objectP)
{
    //-------------------------------------------------------------------- JH --
    int     i;
    uint8_t result = COAP_500_INTERNAL_SERVER_ERROR;
    watermeter_data_t* locDataP = (watermeter_data_t*)(objectP->userData);

    LOG_ARG("prv_watermeter_read objInstId:%d, *numDataP:%d", objInstId, *numDataP);

    // defined as single instance object!
    if (objInstId != 0) return COAP_404_NOT_FOUND;

    if (*numDataP == 0)     // full object, readable resources!
    {
    #if 0
        uint16_t readResIds[] = {
                RES_M_WATERMETER_DATA,
                RES_O_WATERMETER_PRIORITY,
                RES_O_WATERMETER_CREATION_TIME,
                RES_O_WATERMETER_DESCRIPTION,
                RES_O_WATERMETER_FORMAT,
                RES_O_WATERMETER_APP_ID
        }; // readable resources!

        *numDataP  = sizeof(readResIds)/sizeof(uint16_t);
        *tlvArrayP = lwm2m_data_new(*numDataP);
        if (*tlvArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;

        // init readable resource id's
        for (i = 0 ; i < *numDataP ; i++)
        {
            (*tlvArrayP)[i].id = readResIds[i];
        }
    #else
        return COAP_404_NOT_FOUND;
    #endif
    }

    xSemaphoreTake(locDataP->mutex_handle, portMAX_DELAY);

    for (i = 0 ; i < *numDataP ; i++)
    {
        result = prv_watermeter_res2tlv((*tlvArrayP)+i, locDataP);
        if (result!=COAP_205_CONTENT) break;
    }
    xSemaphoreGive(locDataP->mutex_handle);
    LOG_ARG("result=%d", result);
    return result;
}

static uint8_t prv_watermeter_write(void * contextP,
                                   uint16_t instanceId,
                                   int numData,
                                   lwm2m_data_t * dataArray,
                                   lwm2m_object_t * objectP)
{
    uint8_t result = COAP_405_METHOD_NOT_ALLOWED;

    LOG_ARG("prv_watermeter_write instanceId:%d", instanceId);

    watermeter_data_t * pData = (watermeter_data_t *)objectP->userData;

    if (numData != 1)
    {
        return COAP_400_BAD_REQUEST;
    }

    switch (dataArray[0].id)
    {
        case RES_M_WATERMETER_DATA:
            {
                if (1 == instanceId)
                {
#ifdef MTK_LWM2M_CT_SUPPORT
                    extern void ctiot_lwm2m_client_receive_data(uint8_t *data, uint32_t data_len);
                    ctiot_lwm2m_client_receive_data(dataArray[0].value.asBuffer.buffer, dataArray[0].value.asBuffer.length);
#endif
#ifdef MTK_CTM2M_SUPPORT
                    extern void ctm2m_at_receive(uint8_t *data, uint32_t data_len);
                    ctm2m_at_receive(dataArray[0].value.asBuffer.buffer, dataArray[0].value.asBuffer.length);
#endif
#ifdef MTK_CTIOT_SUPPORT
                    extern void ctiot_at_receive(uint8_t *data, uint32_t data_len);
                    ctiot_at_receive(dataArray[0].value.asBuffer.buffer, dataArray[0].value.asBuffer.length);
#endif
                    result = COAP_204_CHANGED;
                }
                else
                {
                    result = COAP_400_BAD_REQUEST;
                }
            }
            break;           
        default:
            result = COAP_405_METHOD_NOT_ALLOWED;
            break;
        }

    LOG_ARG("result=%d", result);

    return result;
}

static uint8_t prv_watermeter_execute(void * contextP,
                                     uint16_t instanceId,
                                     uint16_t resourceId,
                                     uint8_t * buffer,
                                     int length,
                                     lwm2m_object_t * objectP)
{
    uint8_t result = COAP_405_METHOD_NOT_ALLOWED;

    LOG_ARG("prv_watermeter_execute instanceId:%d", instanceId);

    watermeter_data_t * pData = (watermeter_data_t *)objectP->userData;

    switch (resourceId)
    {
        case RES_M_WATERMETER_DATA:
            {
                if (1 == instanceId)
                {
#ifdef MTK_LWM2M_CT_SUPPORT
                    extern void ctiot_lwm2m_client_receive_data(uint8_t *data, uint32_t data_len);
                    ctiot_lwm2m_client_receive_data(buffer, length);
#endif
#ifdef MTK_CTM2M_SUPPORT
                    extern void ctm2m_at_receive(uint8_t *data, uint32_t data_len);
                    ctm2m_at_receive(buffer, length);
#endif
#ifdef MTK_CTIOT_SUPPORT
                    extern void ctiot_at_receive(uint8_t *data, uint32_t data_len);
                    ctiot_at_receive(buffer, length);
#endif
                    result = COAP_204_CHANGED;
                }
                else
                {
                    result = COAP_400_BAD_REQUEST;
                }
            }
            break;           
        default:
            result = COAP_405_METHOD_NOT_ALLOWED;
            break;
        }

    LOG_ARG("result=%d", result);

    return result;
}

void display_watermeter_object(lwm2m_object_t * object)
{
#ifdef WITH_LOGS
    watermeter_data_t * data = (watermeter_data_t *)object->userData;
    fprintf(stdout, "  /%u: Watermeter object:\r\n", object->objID);
    if (NULL != data)
    {
        fprintf(stdout, "    data: %s, priority: %d, time: %lu, desc: %s\r\n",
                data->data, data->priority, data->time, data->desc);
    }
#endif
}


uint8_t watermeter_change(lwm2m_data_t * dataArray,
                         lwm2m_object_t * objectP)
{
    uint8_t result;

    LOG("watermeter_change");

    watermeter_data_t * pData = (watermeter_data_t *)objectP->userData;

    xSemaphoreTake(pData->mutex_handle, portMAX_DELAY);

    switch (dataArray->id)
    {
        case RES_M_WATERMETER_DATA:
            {
                if (LWM2M_TYPE_OPAQUE == dataArray->type)
                {
                    if (pData->data != NULL)
                    {
                        lwm2m_free(pData->data);
                    }
                    if (dataArray->value.asBuffer.length == 0)
                    {
                        pData->data = NULL;
                        pData->data_length = 0;
                    }
                    else
                    {
                        pData->data = (uint8_t *)lwm2m_malloc(dataArray->value.asBuffer.length);
                        memcpy(pData->data, dataArray->value.asBuffer.buffer, dataArray->value.asBuffer.length);
                        pData->data_length = dataArray->value.asBuffer.length;
                    }
                    result = COAP_204_CHANGED;
                }
                else
                {
                    result = COAP_400_BAD_REQUEST;
                }
            }
            break;           
        default:
            result = COAP_405_METHOD_NOT_ALLOWED;
            break;
        }

    xSemaphoreGive(pData->mutex_handle);

    LOG_ARG("result=%d", result);

    return result;
}
/**
  * This function creates the LWM2M Watermeter.
  * @return gives back allocated LWM2M data object structure pointer. On error,
  * NULL value is returned.
  */
lwm2m_object_t * get_object_watermeter(void)
{
    //-------------------------------------------------------------------- JH --
    lwm2m_object_t * watermeterObj;

    LOG("get_object_watermeter");

    watermeterObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));
    if (NULL != watermeterObj)
    {
        int i;
        lwm2m_list_t * targetP;

        memset(watermeterObj, 0, sizeof(lwm2m_object_t));

        // It assigns its unique ID
        // The 19 is the standard ID for the optional object "Watermeter".
        watermeterObj->objID = LWM2M_WATERMETER_OBJECT_ID;

        // and its 2 instances
        for (i=0 ; i < 2 ; i++)
        {
            targetP = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
            if (NULL == targetP) {
                lwm2m_free(watermeterObj);
                return NULL;
            }
            memset(targetP, 0, sizeof(lwm2m_list_t));
            targetP->id = i;
            watermeterObj->instanceList = LWM2M_LIST_ADD(watermeterObj->instanceList, targetP);
        }

        // And the private function that will access the object.
        // Those function will be called when a read query is made by the server.
        // In fact the library don't need to know the resources of the object, only the server does.
        //
        watermeterObj->readFunc    = prv_watermeter_read;
        watermeterObj->writeFunc   = prv_watermeter_write;
        watermeterObj->executeFunc = prv_watermeter_execute;
        watermeterObj->userData    = lwm2m_malloc(sizeof(watermeter_data_t));

        // initialize private data structure containing the needed variables
        if (NULL != watermeterObj->userData)
        {
            watermeter_data_t* data = (watermeter_data_t*)watermeterObj->userData;
            memset(data, 0, sizeof(watermeter_data_t));
            data->mutex_handle = xSemaphoreCreateMutex();
        }
        else
        {
            lwm2m_free(watermeterObj);
            watermeterObj = NULL;
        }
    }

    return watermeterObj;
}

void free_object_watermeter(lwm2m_object_t * object)
{
    lwm2m_list_free(object->instanceList);
    watermeter_data_t *locDataP = (watermeter_data_t*)object->userData;
    if (locDataP->data != NULL) lwm2m_free(locDataP->data);
    if (locDataP->mutex_handle != NULL) vSemaphoreDelete(locDataP->mutex_handle);
    lwm2m_free(object->userData);
    lwm2m_free(object);
}

const char* get_watermeter_uri_string()
{
    return "/19/0/0";
}
#endif  //LWM2M_CLIENT_MODE
