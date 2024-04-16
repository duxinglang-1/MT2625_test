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
  LWM2M object "Barometer" implementation

  \author Wyatt Sun
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <float.h>

#ifdef LWM2M_CLIENT_MODE


// ---- private "object barometer" specific defines ----
// Resource Id's:
#define RES_M_BAROMETER_VALUE            5700
#define RES_O_BAROMETER_MIN_MEASURED     5601
#define RES_O_BAROMETER_MAX_MEASURED     5602
#define RES_O_BAROMETER_MIN_RANGE        5603
#define RES_O_BAROMETER_MAX_RANGE        5604
#define RES_O_BAROMETER_SENSOR_UNITS     5701
#define RES_O_BAROMETER_RESET_MEASURED   5605


#define UNIT_OCTETS (12)

#define INIT_VALUE   (0)
#define MIN_RANGE    (0)
#define MAX_RANGE    (1000)
#define INIT_MIN     (MIN_RANGE - 0.1)
#define INIT_MAX     (MAX_RANGE + 0.1)
#define INIT_PREODIC (20)


typedef struct
{
    float    value;     /* R */
    float    min;       /* R */
    float    max;       /* R */
    float    min_range; /* R */
    float    max_range; /* R */
    char     units[UNIT_OCTETS]; /* R */
} barometer_data_t;

void barometer_setBarometerValue(lwm2m_object_t* barometerObj,
                                 float value,
                                 float min_range, float max_range,
                                 char* units);
/**
implementation for all read-able resources
*/
static uint8_t prv_barometer_res2tlv(lwm2m_data_t* dataP,
                                barometer_data_t* locDataP)
{
    //-------------------------------------------------------------------- JH --
    uint8_t ret = COAP_205_CONTENT;
    switch (dataP->id)     // barometer resourceId
    {
    case RES_M_BAROMETER_VALUE:
        lwm2m_data_encode_float(locDataP->value, dataP);
        break;
    case RES_O_BAROMETER_MIN_MEASURED:
        lwm2m_data_encode_float(locDataP->min, dataP);
        break;
    case RES_O_BAROMETER_MAX_MEASURED:
        lwm2m_data_encode_float(locDataP->max, dataP);
        break;
    case RES_O_BAROMETER_MIN_RANGE:
        lwm2m_data_encode_float(locDataP->min_range, dataP);
        break;
    case RES_O_BAROMETER_MAX_RANGE:
        lwm2m_data_encode_float(locDataP->max_range, dataP);
        break;
    case RES_O_BAROMETER_SENSOR_UNITS:
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
static uint8_t prv_barometer_read(uint16_t objInstId,
                                 int*  numDataP,
                                 lwm2m_data_t** tlvArrayP,
                                 lwm2m_object_t* objectP)
{
    //-------------------------------------------------------------------- JH --
    int     i;
    uint8_t result = COAP_500_INTERNAL_SERVER_ERROR;
    barometer_data_t* locDataP = (barometer_data_t*)(objectP->userData);

    // defined as single instance object!
    if (objInstId != 100) return COAP_404_NOT_FOUND;

    if (*numDataP == 0)     // full object, readable resources!
    {
        uint16_t readResIds[] = {
                RES_M_BAROMETER_VALUE,
                RES_O_BAROMETER_MIN_MEASURED,
                RES_O_BAROMETER_MAX_MEASURED,
                RES_O_BAROMETER_MIN_RANGE,
                RES_O_BAROMETER_MAX_RANGE,
                RES_O_BAROMETER_SENSOR_UNITS
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
        result = prv_barometer_res2tlv((*tlvArrayP)+i, locDataP);
        if (result!=COAP_205_CONTENT) break;
    }
    return result;
}

static uint8_t prv_barometer_write(void * contextP,
                                   uint16_t instanceId,
                                   int numData,
                                   lwm2m_data_t * dataArray,
                                   lwm2m_object_t * objectP)
{
    int i;
    uint8_t result = COAP_405_METHOD_NOT_ALLOWED;
    
    return result;
}

static uint8_t prv_barometer_execute(void * contextP,
                                     uint16_t instanceId,
                                     uint16_t resourceId,
                                     uint8_t * buffer,
                                     int length,
                                     lwm2m_object_t * objectP)
{
    barometer_data_t * data = (barometer_data_t*)(objectP->userData);

    switch (resourceId)
    {
    case RES_O_BAROMETER_RESET_MEASURED:
        fprintf(stdout, "\n\t RESET BAROMETER MEASURED, min=%f, max=%f, value=%f\r\n\n", data->min, data->max, data->value);
        data->min = data->max = data->value;
        fprintf(stdout, "\n\t RESET BAROMETER MEASURED, min=%f, max=%f, value=%f\r\n\n", data->min, data->max, data->value);
        return COAP_204_CHANGED;
    default:
        return COAP_405_METHOD_NOT_ALLOWED;
    }
}

void display_barometer_object(lwm2m_object_t * object)
{
#ifdef WITH_LOGS
    barometer_data_t * data = (barometer_data_t *)object->userData;
    fprintf(stdout, "  /%u: Barometer object:\r\n", object->objID);
    if (NULL != data)
    {
        fprintf(stdout, "    value: %.6f, min: %.6f, max: %.6f, min_range: %.6f, max_range: %lu, units: %.6f\r\n",
                data->value, data->min, data->max, data->min_range, data->max_range, data->units);
    }
#endif
}


uint8_t barometer_change(lwm2m_data_t * dataArray,
                         lwm2m_object_t * objectP)
{
    uint8_t result;

    barometer_data_t * pData = (barometer_data_t *)objectP->userData;

    switch (dataArray->id)
    {
        case RES_M_BAROMETER_VALUE:
            {
                double new_value;
                if (1 == lwm2m_data_decode_float(dataArray, &new_value))
                {
                    if ((MIN_RANGE <= new_value) && (MAX_RANGE >= new_value))
                    {
                        pData->value = new_value;
                        result = COAP_204_CHANGED;
                        if (new_value < pData->min || pData->min < MIN_RANGE)
                            pData->min = new_value;
                        if (pData->max < new_value || MAX_RANGE < pData->max)
                            pData->max = new_value;
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
  * This function creates the LWM2M Barometer.
  * @return gives back allocated LWM2M data object structure pointer. On error,
  * NULL value is returned.
  */
lwm2m_object_t * get_object_barometer(void)
{
    //-------------------------------------------------------------------- JH --
    lwm2m_object_t * barometerObj;

    barometerObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));
    if (NULL != barometerObj)
    {
        memset(barometerObj, 0, sizeof(lwm2m_object_t));

        // It assigns its unique ID
        // The 3315 is the standard ID for the optional object "Barometer".
        barometerObj->objID = LWM2M_BAROMETER_OBJECT_ID;

        // and its unique instance
        barometerObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
        if (NULL != barometerObj->instanceList)
        {
            memset(barometerObj->instanceList, 0, sizeof(lwm2m_list_t));
            barometerObj->instanceList->id = 100;
        }
        else
        {
            lwm2m_free(barometerObj);
            return NULL;
        }

        // And the private function that will access the object.
        // Those function will be called when a read query is made by the server.
        // In fact the library don't need to know the resources of the object, only the server does.
        //
        barometerObj->readFunc    = prv_barometer_read;
        barometerObj->writeFunc   = prv_barometer_write;
        barometerObj->executeFunc = prv_barometer_execute;
        barometerObj->userData    = lwm2m_malloc(sizeof(barometer_data_t));

        // initialize private data structure containing the needed variables
        if (NULL != barometerObj->userData)
        {
            static int i = 0;
            barometer_data_t* data = (barometer_data_t*)barometerObj->userData;
            data->value     = INIT_VALUE;
            data->min       = INIT_MIN;
            data->max       = INIT_MAX;
            data->min_range = MIN_RANGE;
            data->max_range = MAX_RANGE;
            strcpy(data->units, "Pa");
        }
        else
        {
            lwm2m_free(barometerObj);
            barometerObj = NULL;
        }
    }

    return barometerObj;
}

void free_object_barometer(lwm2m_object_t * object)
{
    lwm2m_list_free(object->instanceList);
    lwm2m_free(object->userData);
    lwm2m_free(object);
}

const char* get_barometer_uri_string()
{
    return "/3315/100/5700";
}
#endif  //LWM2M_CLIENT_MODE
