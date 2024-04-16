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
  LWM2M object "Accelerometer" implementation

  \author Yuan Yin
*/

/*
 ID      Name        Operations  Instances   Mandatory   Type    Range or Enumeration    Units   Description
 5702    X Value R   Single  Mandatory   Float       Defined by  resource.   The measured value along the X axis.
 5703    Y Value R   Single  Optional    Float       Defined by  resource.   The measured value along the Y axis.
 5704    Z Value R   Single  Optional    Float       Defined by  resource    The measured value along the Z axis.
 5701    Sensor Units    R   Single  Optional    String          Measurement Units Definition e.g. Temperature in Celsius.
 5603    Min Range Value R   Single  Optional    Float       Defined by resource.   The minimum value that can be measured by the sensor
 5604    Max Range Value R   Single  Optional    Float       Defined by resource.    The maximum value that can be measured by the sensor
 */

#include "liblwm2m.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <float.h>

#ifdef LWM2M_CLIENT_MODE


// ---- private "object barometer" specific defines ----
// Resource Id's:
#define RES_M_ACCELEROMETER_X_VALUE          5702
#define RES_M_ACCELEROMETER_Y_VALUE          5703
#define RES_M_ACCELEROMETER_Z_VALUE          5704
#define RES_O_ACCELEROMETER_MIN_RANGE        5603
#define RES_O_ACCELEROMETER_MAX_RANGE        5604
#define RES_O_ACCELEROMETER_SENSOR_UNITS     5701



#define UNIT_OCTETS (12)

#define INIT_X_VALUE               (0)
#define INIT_y_VALUE               (0)
#define INIT_Z_VALUE               (0)
#define MIN_ACCELEROMETER_RANGE    (0)
#define MAX_ACCELEROMETER_RANGE    (1000)
#define INIT_ACCELEROMETER_MIN     (MIN_ACCELEROMETER_RANGE - 0.1)
#define INIT_ACCELEROMETER_MAX     (MAX_ACCELEROMETER_RANGE + 0.1)
#define INIT_ACCELEROMETER_PREODIC (20)


#define ACCELEROMETER_OBJECT_INSTANCE_ID                    (105)

typedef struct
{
    float    x;                  /* R */
    float    y;                  /* R */
    float    z;                  /* R */
    float    min_range;          /* R */
    float    max_range;          /* R */
    char     units[UNIT_OCTETS]; /* R */
} accelerometer_data_t;


#define MAX_URI_BYTES                                       (100)

char res_uri_string[MAX_URI_BYTES] = {0};

void accelerometer_setBarometerValue(lwm2m_object_t* accelerometerObj,
                                 float x,
                                 float y,
                                 float z,
                                 float min,
                                 float max,
                                 char* units);
/**
implementation for all read-able resources
*/
static uint8_t prv_accelerometer_res2tlv(lwm2m_data_t* dataP,
                                accelerometer_data_t* locDataP)
{
    //-------------------------------------------------------------------- JH --
    uint8_t ret = COAP_205_CONTENT;
    switch (dataP->id)     //accelerometer resourceId
    {
    case RES_M_ACCELEROMETER_X_VALUE:
        lwm2m_data_encode_float(locDataP->x, dataP);
        break;
    case RES_M_ACCELEROMETER_Y_VALUE:
        lwm2m_data_encode_float(locDataP->y, dataP);
        break;
    case RES_M_ACCELEROMETER_Z_VALUE:
        lwm2m_data_encode_float(locDataP->z, dataP);
        break;
    case RES_O_ACCELEROMETER_MIN_RANGE:
        lwm2m_data_encode_float(locDataP->min_range, dataP);
        break;
    case RES_O_ACCELEROMETER_MAX_RANGE:
        lwm2m_data_encode_float(locDataP->max_range, dataP);
        break;
    case RES_O_ACCELEROMETER_SENSOR_UNITS:
        lwm2m_data_encode_string((const char*)locDataP->units, dataP);
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
static uint8_t prv_accelerometer_read(uint16_t objInstId,
                                 int*  numDataP,
                                 lwm2m_data_t** tlvArrayP,
                                 lwm2m_object_t* objectP)
{
    //-------------------------------------------------------------------- JH --
    int     i;
    uint8_t result = COAP_500_INTERNAL_SERVER_ERROR;
    accelerometer_data_t* locDataP = (accelerometer_data_t*)(objectP->userData);

    // defined as single instance object!
    if (objInstId != 105) return COAP_404_NOT_FOUND;

    if (*numDataP == 0)     // full object, readable resources!
    {
        uint16_t readResIds[] = {
                RES_M_ACCELEROMETER_X_VALUE,
                RES_M_ACCELEROMETER_Y_VALUE,
                RES_M_ACCELEROMETER_Z_VALUE,
                RES_O_ACCELEROMETER_MIN_RANGE,
                RES_O_ACCELEROMETER_MAX_RANGE,
                RES_O_ACCELEROMETER_SENSOR_UNITS
        }; // readable resources!

        *numDataP  = sizeof(readResIds)/sizeof(uint16_t);
        *tlvArrayP = lwm2m_data_new(*numDataP);
        if (*tlvArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;

        // init readable resource id's
        for (i = 0 ; i < *numDataP ; i++)
        {
            (*tlvArrayP)[i].id = readResIds[i];
        }
    }

    for (i = 0 ; i < *numDataP ; i++)
    {
        result = prv_accelerometer_res2tlv((*tlvArrayP)+i, locDataP);
        if (result!=COAP_205_CONTENT) break;
    }
    return result;
}

static uint8_t prv_accelerometer_write(void * contextP,
                                   uint16_t instanceId,
                                   int numData,
                                   lwm2m_data_t * dataArray,
                                   lwm2m_object_t * objectP)
{
    int i;
    uint8_t result = COAP_405_METHOD_NOT_ALLOWED;
    
    return result;
}

static uint8_t prv_accelerometer_execute(void * contextP,
                                     uint16_t instanceId,
                                     uint16_t resourceId,
                                     uint8_t * buffer,
                                     int length,
                                     lwm2m_object_t * objectP)
{
    accelerometer_data_t * data = (accelerometer_data_t*)(objectP->userData);

    switch (resourceId)
    {
    default:
        return COAP_405_METHOD_NOT_ALLOWED;
    }
}

void display_accelerometer_object(lwm2m_object_t * object)
{
#ifdef WITH_LOGS
    accelerometer_data_t * data = (accelerometer_data_t *)object->userData;
    fprintf(stdout, "  /%u: accelerometer object:\r\n", object->objID);
    if (NULL != data)
    {
        fprintf(stdout, "    x: %.6f, y: %.6f, z: %.6f, min_range: %.6f, max_range: %.6f, units: %.6f\r\n",
                data->x, data->y, data->z, data->min_range, data->max_range, data->units);
    }
#endif
}


uint8_t accelerometer_change(lwm2m_data_t * dataArray,
                         lwm2m_object_t * objectP)
{
    uint8_t result;

    accelerometer_data_t * pData = (accelerometer_data_t *)objectP->userData;

    switch (dataArray->id)
    {
        case RES_M_ACCELEROMETER_X_VALUE:      
            {
                double new_value;
                if (1 == lwm2m_data_decode_float(dataArray, &new_value))
                {
                    if ((MIN_ACCELEROMETER_RANGE <= new_value) && (MAX_ACCELEROMETER_RANGE >= new_value))
                    {
                        pData->x = new_value;
                        result = COAP_204_CHANGED;
                    }
                    else
                    {
                        result = COAP_400_BAD_REQUEST;
                    }
                }
                else
                {
                    result = COAP_400_BAD_REQUEST;
                }
            }
            break;       
         case RES_M_ACCELEROMETER_Y_VALUE:       
            {
                double new_value;
                if (1 == lwm2m_data_decode_float(dataArray, &new_value))
                {
                    if ((MIN_ACCELEROMETER_RANGE <= new_value) && (MAX_ACCELEROMETER_RANGE >= new_value))
                    {
                        pData->y = new_value;
                        result = COAP_204_CHANGED;
                    }
                    else
                    {
                        result = COAP_400_BAD_REQUEST;
                    }
                }
                else
                {
                    result = COAP_400_BAD_REQUEST;
                }
            }
            break;
         case RES_M_ACCELEROMETER_Z_VALUE:       
            {
                double new_value;
                if (1 == lwm2m_data_decode_float(dataArray, &new_value))
                {
                    if ((MIN_ACCELEROMETER_RANGE <= new_value) && (MAX_ACCELEROMETER_RANGE >= new_value))
                    {
                        pData->z = new_value;
                        result = COAP_204_CHANGED;
                    }
                    else
                    {
                        result = COAP_400_BAD_REQUEST;
                    }
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

    return result;
}
/**
  * This function creates the LWM2M Accelerometer.
  * @return gives back allocated LWM2M data object structure pointer. On error,
  * NULL value is returned.
  */
lwm2m_object_t * get_object_accelerometer(void)
{
    //-------------------------------------------------------------------- JH --
    lwm2m_object_t * accelerometerObj;

    accelerometerObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));
    if (NULL != accelerometerObj)
    {
        memset(accelerometerObj, 0, sizeof(lwm2m_object_t));

        // It assigns its unique ID
        // The 3313 is the standard ID for the optional object "Accelelerometer".
        accelerometerObj->objID = LWM2M_ACCELEROMTER_OBJECT_ID;

        // and its unique instance
        accelerometerObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
        if (NULL != accelerometerObj->instanceList)
        {
            memset(accelerometerObj->instanceList, 0, sizeof(lwm2m_list_t));
            accelerometerObj->instanceList->id = 105;
        }
        else
        {
            lwm2m_free(accelerometerObj);
            return NULL;
        }

        // And the private function that will access the object.
        // Those function will be called when a read query is made by the server.
        // In fact the library don't need to know the resources of the object, only the server does.
        //
        accelerometerObj->readFunc    = prv_accelerometer_read;
        accelerometerObj->writeFunc   = prv_accelerometer_write;
        accelerometerObj->executeFunc = prv_accelerometer_execute;
        accelerometerObj->userData    = lwm2m_malloc(sizeof(accelerometer_data_t));

        // initialize private data structure containing the needed variables
        if (NULL != accelerometerObj->userData)
        {
            static int i = 0;
            accelerometer_data_t* data = (accelerometer_data_t*)accelerometerObj->userData;
            data->x     =   INIT_X_VALUE;
            data->y       = INIT_y_VALUE;
            data->z       = INIT_Z_VALUE;
            data->min_range = INIT_ACCELEROMETER_MIN;
            data->max_range = INIT_ACCELEROMETER_MAX;
            strcpy(data->units, "m/s^2");
        }
        else
        {
            lwm2m_free(accelerometerObj);
            accelerometerObj = NULL;
        }
    }

    return accelerometerObj;
}


void free_object_accelerometer(lwm2m_object_t * object)
{
    lwm2m_list_free(object->instanceList);
    lwm2m_free(object->userData);
    lwm2m_free(object);
}


char* get_accelerometer_uri_string(char type)
{
    int partial_member = 0;
    
    switch (type)
    {
        case 'x':
            partial_member = RES_M_ACCELEROMETER_X_VALUE;           
        break;
        case 'y':
            partial_member = RES_M_ACCELEROMETER_Y_VALUE;   
        break;
        case 'z':
            partial_member = RES_M_ACCELEROMETER_Z_VALUE;   
        break;
        default:
        break;
    }

    sprintf(res_uri_string,"/%d/%d/%d", 
                LWM2M_ACCELEROMTER_OBJECT_ID,
                ACCELEROMETER_OBJECT_INSTANCE_ID,
                partial_member);

    return res_uri_string;

}

#endif  //LWM2M_CLIENT_MODE
