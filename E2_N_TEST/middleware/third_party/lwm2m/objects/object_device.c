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
 *    domedambrosio - Please refer to git log
 *    Fabien Fleutot - Please refer to git log
 *    Axel Lorente - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
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

/*
 * This object is single instance only, and is mandatory to all LWM2M device as it describe the object such as its
 * manufacturer, model, etc...
 */

#include "liblwm2m.h"
#include "internals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "ril_internal_use.h"
#include "ril.h"
#include "hal_adc.h"

#define PRV_MANUFACTURER      "Mediatek"
#define PRV_MODEL_NUMBER      "NB-IOT Mobile Station"
#define PRV_SERIAL_NUMBER     "000000000000000"
#define PRV_FIRMWARE_VERSION  "1.0"
#define PRV_POWER_SOURCE_1    1
#define PRV_POWER_SOURCE_2    5
#define PRV_POWER_VOLTAGE_1   3800
#define PRV_POWER_VOLTAGE_2   5000
#define PRV_POWER_CURRENT_1   125
#define PRV_POWER_CURRENT_2   900
#define PRV_BATTERY_LEVEL     100
#define PRV_MEMORY_FREE       15
#define PRV_ERROR_CODE        0
#define PRV_TIME_ZONE         "Asia/Shanghai"
#if defined(MTK_CTM2M_SUPPORT) || defined(MTK_TMO_CERT_SUPPORT)
#define PRV_BINDING_MODE      "U,UQ"
#else
#define PRV_BINDING_MODE      "U"
#endif
#define PRV_DEVICE_TYPE       "MT2625"
#define PRV_HARDWARE_VERSION  "MT2625_V01"
#define PRV_SOFTWARE_VERSION  "geneva-mp3-nb.V1.4.0"

#define PRV_OFFSET_MAXLEN   7 //+HH:MM\0 at max
#define PRV_TLV_BUFFER_SIZE 128

// Resource Id's:
#define RES_O_MANUFACTURER          0
#define RES_O_MODEL_NUMBER          1
#define RES_O_SERIAL_NUMBER         2
#define RES_O_FIRMWARE_VERSION      3
#define RES_M_REBOOT                4
#define RES_O_FACTORY_RESET         5
#define RES_O_AVL_POWER_SOURCES     6
#define RES_O_POWER_SOURCE_VOLTAGE  7
#define RES_O_POWER_SOURCE_CURRENT  8
#define RES_O_BATTERY_LEVEL         9
#define RES_O_MEMORY_FREE           10
#define RES_M_ERROR_CODE            11
#define RES_O_RESET_ERROR_CODE      12
#define RES_O_CURRENT_TIME          13
#define RES_O_UTC_OFFSET            14
#define RES_O_TIMEZONE              15
#define RES_M_BINDING_MODES         16
// since TS 20141126-C:
#define RES_O_DEVICE_TYPE           17
#define RES_O_HARDWARE_VERSION      18
#define RES_O_SOFTWARE_VERSION      19
#define RES_O_BATTERY_STATUS        20
#define RES_O_MEMORY_TOTAL          21


typedef struct
{
    int64_t free_memory;
    int64_t error;
    int64_t time;
    uint8_t battery_level;
    char time_offset[PRV_OFFSET_MAXLEN];
} device_data_t;


static char device_manufacturer[20];
static char device_model_number[40];
static char device_serial_number[20];
static char device_hw_version[20];
static char device_sw_version[40];

static uint32_t device_get_battery_voltage(void)
{
    uint32_t adc_counts;
    uint32_t bat_voltage;

    hal_adc_init();
    hal_adc_get_data_polling(HAL_ADC_CHANNEL_6, &adc_counts);
    hal_adc_deinit();

    bat_voltage = ((adc_counts * 1400) / 4095) * 4;

    LOG_ARG("adc_counts = %d, bat_voltage = %d", adc_counts, bat_voltage);

    return bat_voltage;
}

static uint8_t device_get_battery_logic_value(uint32_t battery_vol)
{
    uint32_t battery_logic_value;

    if (battery_vol <= 3400) {
        battery_logic_value = 0;
    } else if (battery_vol >= 4100) {
        battery_logic_value = 100;
    } else {
        /* Scale to logic value */
        battery_logic_value = ((battery_vol - 3400 ) * 100) / (4100 - 3400);
    }

    LOG_ARG("battery_vol = %d, battery_logic_value = %d", battery_vol, battery_logic_value);

    return (uint8_t)battery_logic_value;
}

// basic check that the time offset value is at ISO 8601 format
// bug: +12:30 is considered a valid value by this function
static int prv_check_time_offset(char * buffer,
                                 int length)
{
    int min_index;

    if (length != 3 && length != 5 && length != 6) return 0;
    if (buffer[0] != '-' && buffer[0] != '+') return 0;
    switch (buffer[1])
    {
    case '0':
        if (buffer[2] < '0' || buffer[2] > '9') return 0;
        break;
    case '1':
        if (buffer[2] < '0' || buffer[2] > '2') return 0;
        break;
    default:
        return 0;
    }
    switch (length)
    {
    case 3:
        return 1;
    case 5:
        min_index = 3;
        break;
    case 6:
        if (buffer[3] != ':') return 0;
        min_index = 4;
        break;
    default:
        // never happen
        return 0;
    }
    if (buffer[min_index] < '0' || buffer[min_index] > '5') return 0;
    if (buffer[min_index+1] < '0' || buffer[min_index+1] > '9') return 0;

    return 1;
}

static uint8_t prv_set_value(lwm2m_data_t * dataP,
                             device_data_t * devDataP)
{
    // a simple switch structure is used to respond at the specified resource asked
    switch (dataP->id)
    {
    case RES_O_MANUFACTURER:
        if (device_manufacturer[0] != '\0')
        {
            lwm2m_data_encode_string(device_manufacturer, dataP);
        }
        else
        {
            lwm2m_data_encode_string(PRV_MANUFACTURER, dataP);
        }
        return COAP_205_CONTENT;

    case RES_O_MODEL_NUMBER:
        if (device_model_number[0] != '\0')
        {
            lwm2m_data_encode_string(device_model_number, dataP);
        }
        else
        {
            lwm2m_data_encode_string(PRV_MODEL_NUMBER, dataP);
        }
        return COAP_205_CONTENT;

    case RES_O_SERIAL_NUMBER:
        if (device_serial_number[0] != '\0')
        {
            lwm2m_data_encode_string(device_serial_number, dataP);
        }
        else
        {
            lwm2m_data_encode_string(PRV_SERIAL_NUMBER, dataP);
        }
        return COAP_205_CONTENT;

    case RES_O_FIRMWARE_VERSION:
        lwm2m_data_encode_string(PRV_FIRMWARE_VERSION, dataP);
        return COAP_205_CONTENT;

    case RES_M_REBOOT:
        return COAP_405_METHOD_NOT_ALLOWED;

    case RES_O_FACTORY_RESET:
        return COAP_405_METHOD_NOT_ALLOWED;

    case RES_O_AVL_POWER_SOURCES:
    {
        lwm2m_data_t * subTlvP;

        subTlvP = lwm2m_data_new(2);

        subTlvP[0].id = 0;
        lwm2m_data_encode_int(PRV_POWER_SOURCE_1, subTlvP);
        subTlvP[1].id = 1;
        lwm2m_data_encode_int(PRV_POWER_SOURCE_2, subTlvP + 1);

        lwm2m_data_encode_instances(subTlvP, 2, dataP);

        return COAP_205_CONTENT;
    }

    case RES_O_POWER_SOURCE_VOLTAGE:
    {
        lwm2m_data_t * subTlvP;

        subTlvP = lwm2m_data_new(2);

        subTlvP[0].id = 0;
    #if 0
        lwm2m_data_encode_int(PRV_POWER_VOLTAGE_1, subTlvP);
    #else
        uint32_t battery_vol = device_get_battery_voltage();
        lwm2m_data_encode_int(battery_vol, subTlvP);
    #endif
        subTlvP[1].id = 1;
        lwm2m_data_encode_int(PRV_POWER_VOLTAGE_2, subTlvP + 1);

        lwm2m_data_encode_instances(subTlvP, 2, dataP);

        return COAP_205_CONTENT;
    }

    case RES_O_POWER_SOURCE_CURRENT:
    {
        lwm2m_data_t * subTlvP;

        subTlvP = lwm2m_data_new(2);

        subTlvP[0].id = 0;
        lwm2m_data_encode_int(PRV_POWER_CURRENT_1, &subTlvP[0]);
        subTlvP[1].id = 1;
        lwm2m_data_encode_int(PRV_POWER_CURRENT_2, &subTlvP[1]);

        lwm2m_data_encode_instances(subTlvP, 2, dataP);

        return COAP_205_CONTENT;
    }

    case RES_O_BATTERY_LEVEL:
        lwm2m_data_encode_int(devDataP->battery_level, dataP);
        return COAP_205_CONTENT;

    case RES_O_MEMORY_FREE:
        lwm2m_data_encode_int(devDataP->free_memory, dataP);
        return COAP_205_CONTENT;

    case RES_M_ERROR_CODE:
    {
        lwm2m_data_t * subTlvP;

        subTlvP = lwm2m_data_new(1);

        subTlvP[0].id = 0;
        lwm2m_data_encode_int(devDataP->error, subTlvP);

        lwm2m_data_encode_instances(subTlvP, 1, dataP);

        return COAP_205_CONTENT;
    }
    case RES_O_RESET_ERROR_CODE:
        return COAP_405_METHOD_NOT_ALLOWED;

    case RES_O_CURRENT_TIME:
        lwm2m_data_encode_int(time(NULL) + devDataP->time, dataP);
        return COAP_205_CONTENT;

    case RES_O_UTC_OFFSET:
        lwm2m_data_encode_string(devDataP->time_offset, dataP);
        return COAP_205_CONTENT;

    case RES_O_TIMEZONE:
        lwm2m_data_encode_string(PRV_TIME_ZONE, dataP);
        return COAP_205_CONTENT;

    case RES_M_BINDING_MODES:
        lwm2m_data_encode_string(PRV_BINDING_MODE, dataP);
        return COAP_205_CONTENT;

    case RES_O_DEVICE_TYPE:
        lwm2m_data_encode_string(PRV_DEVICE_TYPE, dataP);
        return COAP_205_CONTENT;

    case RES_O_HARDWARE_VERSION:
        if (device_hw_version[0] != '\0')
        {
            lwm2m_data_encode_string(device_hw_version, dataP);
        }
        else
        {
            lwm2m_data_encode_string(PRV_HARDWARE_VERSION, dataP);
        }
        return COAP_205_CONTENT;

    case RES_O_SOFTWARE_VERSION:
        if (device_sw_version[0] != '\0')
        {
            lwm2m_data_encode_string(device_sw_version, dataP);
        }
        else
        {
            lwm2m_data_encode_string(PRV_SOFTWARE_VERSION, dataP);
        }
        return COAP_205_CONTENT;

    default:
        return COAP_404_NOT_FOUND;
    }
}

static uint8_t prv_device_read(uint16_t instanceId,
                               int * numDataP,
                               lwm2m_data_t ** dataArrayP,
                               lwm2m_object_t * objectP)
{
    uint8_t result;
    int i;

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }

    // is the server asking for the full object ?
    if (*numDataP == 0)
    {
        uint16_t resList[] = {
                RES_O_MANUFACTURER,
                RES_O_MODEL_NUMBER,
                RES_O_SERIAL_NUMBER,
                RES_O_FIRMWARE_VERSION,
                //E: RES_M_REBOOT,
                //E: RES_O_FACTORY_RESET,
                RES_O_AVL_POWER_SOURCES,
                RES_O_POWER_SOURCE_VOLTAGE,
                RES_O_POWER_SOURCE_CURRENT,
                RES_O_BATTERY_LEVEL,
                RES_O_MEMORY_FREE,
                RES_M_ERROR_CODE,
                //E: RES_O_RESET_ERROR_CODE,
                RES_O_CURRENT_TIME,
                RES_O_UTC_OFFSET,
                RES_O_TIMEZONE,
                RES_M_BINDING_MODES,
                RES_O_DEVICE_TYPE,
                RES_O_HARDWARE_VERSION,
                RES_O_SOFTWARE_VERSION
        };
        int nbRes = sizeof(resList)/sizeof(uint16_t);

        *dataArrayP = lwm2m_data_new(nbRes);
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
        result = prv_set_value((*dataArrayP) + i, (device_data_t*)(objectP->userData));
        i++;
    } while (i < *numDataP && result == COAP_205_CONTENT);

    return result;
}

static uint8_t prv_device_discover(uint16_t instanceId,
                                   int * numDataP,
                                   lwm2m_data_t ** dataArrayP,
                                   lwm2m_object_t * objectP)
{
    uint8_t result;
    int i;

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }

    result = COAP_205_CONTENT;

    // is the server asking for the full object ?
    if (*numDataP == 0)
    {
        uint16_t resList[] = {
            RES_O_MANUFACTURER,
            RES_O_MODEL_NUMBER,
            RES_O_SERIAL_NUMBER,
            RES_O_FIRMWARE_VERSION,
            RES_M_REBOOT,
            RES_O_FACTORY_RESET,
            RES_O_AVL_POWER_SOURCES,
            RES_O_POWER_SOURCE_VOLTAGE,
            RES_O_POWER_SOURCE_CURRENT,
            RES_O_BATTERY_LEVEL,
            RES_O_MEMORY_FREE,
            RES_M_ERROR_CODE,
            RES_O_RESET_ERROR_CODE,
            RES_O_CURRENT_TIME,
            RES_O_UTC_OFFSET,
            RES_O_TIMEZONE,
            RES_M_BINDING_MODES,
            RES_O_DEVICE_TYPE,
            RES_O_HARDWARE_VERSION,
            RES_O_SOFTWARE_VERSION
        };
        int nbRes = sizeof(resList) / sizeof(uint16_t);

        *dataArrayP = lwm2m_data_new(nbRes);
        if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
        *numDataP = nbRes;
        for (i = 0; i < nbRes; i++)
        {
            (*dataArrayP)[i].id = resList[i];
        }
    }
    else
    {
        for (i = 0; i < *numDataP && result == COAP_205_CONTENT; i++)
        {
            switch ((*dataArrayP)[i].id)
            {
            case RES_O_MANUFACTURER:
            case RES_O_MODEL_NUMBER:
            case RES_O_SERIAL_NUMBER:
            case RES_O_FIRMWARE_VERSION:
            case RES_M_REBOOT:
            case RES_O_FACTORY_RESET:
            case RES_O_AVL_POWER_SOURCES:
            case RES_O_POWER_SOURCE_VOLTAGE:
            case RES_O_POWER_SOURCE_CURRENT:
            case RES_O_BATTERY_LEVEL:
            case RES_O_MEMORY_FREE:
            case RES_M_ERROR_CODE:
            case RES_O_RESET_ERROR_CODE:
            case RES_O_CURRENT_TIME:
            case RES_O_UTC_OFFSET:
            case RES_O_TIMEZONE:
            case RES_M_BINDING_MODES:
            case RES_O_DEVICE_TYPE:
            case RES_O_HARDWARE_VERSION:
            case RES_O_SOFTWARE_VERSION:
                break;
            default:
                result = COAP_404_NOT_FOUND;
                break;
            }
        }
    }

    return result;
}

static uint8_t prv_device_write(void * contextP,
                                uint16_t instanceId,
                                int numData,
                                lwm2m_data_t * dataArray,
                                lwm2m_object_t * objectP)
{
    int i;
    uint8_t result;

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }

    i = 0;

    do
    {
        switch (dataArray[i].id)
        {
        case RES_O_CURRENT_TIME:
            if (1 == lwm2m_data_decode_int(dataArray + i, &((device_data_t*)(objectP->userData))->time))
            {
                ((device_data_t*)(objectP->userData))->time -= time(NULL);
                result = COAP_204_CHANGED;
            }
            else
            {
                result = COAP_400_BAD_REQUEST;
            }
            break;

        case RES_O_UTC_OFFSET:
            if (1 == prv_check_time_offset((char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length))
            {
                strncpy(((device_data_t*)(objectP->userData))->time_offset, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
                ((device_data_t*)(objectP->userData))->time_offset[dataArray[i].value.asBuffer.length] = 0;
                result = COAP_204_CHANGED;
            }
            else
            {
                result = COAP_400_BAD_REQUEST;
            }
            break;

        case RES_O_TIMEZONE:
            //ToDo IANA TZ Format
            result = COAP_501_NOT_IMPLEMENTED;
            break;

        default:
            result = COAP_405_METHOD_NOT_ALLOWED;
        }

        i++;
    } while (i < numData && result == COAP_204_CHANGED);

    return result;
}

static uint8_t prv_device_execute(void * contextP,
                                  uint16_t instanceId,
                                  uint16_t resourceId,
                                  uint8_t * buffer,
                                  int length,
                                  lwm2m_object_t * objectP)
{
    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }

#if 0
    /* sometimes execute with value */
    if (length != 0) return COAP_400_BAD_REQUEST;
#endif

    switch (resourceId)
    {
    case RES_M_REBOOT:
        fprintf(stdout, "\n\t REBOOT\r\n\n");
        if (((lwm2m_context_t *)contextP)->notify_callback != NULL)
        {
            ((lwm2m_context_t *)contextP)->notify_callback(LWM2M_NOTIFY_TYPE_REBOOT, LWM2M_NOTIFY_CODE_SUCCESS, 0);
        }
        return COAP_204_CHANGED;
    case RES_O_FACTORY_RESET:
        fprintf(stdout, "\n\t FACTORY RESET\r\n\n");
        if (((lwm2m_context_t *)contextP)->notify_callback != NULL)
        {
            ((lwm2m_context_t *)contextP)->notify_callback(LWM2M_NOTIFY_TYPE_FACTORY_RESET, LWM2M_NOTIFY_CODE_SUCCESS, 0);
        }
        return COAP_204_CHANGED;
    case RES_O_RESET_ERROR_CODE:
        fprintf(stdout, "\n\t RESET ERROR CODE\r\n\n");
        ((device_data_t*)(objectP->userData))->error = 0;
        return COAP_204_CHANGED;
    default:
        return COAP_405_METHOD_NOT_ALLOWED;
    }
}

void display_device_object(lwm2m_object_t * object)
{
#ifdef WITH_LOGS
    device_data_t * data = (device_data_t *)object->userData;
    fprintf(stdout, "  /%u: Device object:\r\n", object->objID);
    if (NULL != data)
    {
        fprintf(stdout, "    time: %lld, time_offset: %s\r\n",
                (long long) data->time, data->time_offset);
    }
#endif
}

static int32_t device_ril_callback(ril_cmd_response_t *cmd_response)
{
    switch (cmd_response->cmd_id) {
        case RIL_CMD_ID_CGMI: {
            ril_manufacturer_identification_rsp_t *response = (ril_manufacturer_identification_rsp_t *)cmd_response->cmd_param;
            if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                if (response->manufacturer) {
                    memcpy(device_manufacturer, response->manufacturer, sizeof(device_manufacturer));
                    LOG_ARG("device_manufacturer = %s", device_manufacturer);
                }
            }
        }
        break;

        case RIL_CMD_ID_CGMM: {
            ril_model_identification_rsp_t *response = (ril_model_identification_rsp_t *)cmd_response->cmd_param; 
            if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                if (response->model) {
                    memcpy(device_model_number, response->model, sizeof(device_model_number));
                    LOG_ARG("device_model_number = %s", device_model_number);
                }
            }
        }
        break;

        case RIL_CMD_ID_CGSN: {
            ril_serial_number_rsp_t *response = (ril_serial_number_rsp_t *)cmd_response->cmd_param;
            if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                if (response->value.imei) {
                    memcpy(device_serial_number, response->value.imei, sizeof(device_serial_number));
                    LOG_ARG("device_serial_number = %s", device_serial_number);
                }
            }
        }
        break;

        case RIL_CMD_ID_HVER: {
            ril_request_hw_version_rsp_t *response = (ril_request_hw_version_rsp_t *)cmd_response->cmd_param;
            if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                if (response->hw_version) {
                    memcpy(device_hw_version, response->hw_version, sizeof(device_hw_version));
                    LOG_ARG("device_hw_version = %s", device_hw_version);
                }
            }
        }
        break;

        case RIL_CMD_ID_CGMR: {
            ril_revision_identification_rsp_t *response = (ril_revision_identification_rsp_t *)cmd_response->cmd_param;
            if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                if (response->revision) {
                    memcpy(device_sw_version, response->revision, sizeof(device_sw_version));
                    LOG_ARG("device_sw_version = %s", device_sw_version);
                }
            }
        }
        break;

        default:
            break;
    }

    return 0;
}

static void device_init(void)
{
    ril_status_t status;

    device_manufacturer[0] = '\0';
    device_model_number[0] = '\0';
    device_serial_number[0] = '\0';
    device_hw_version[0] = '\0';
    device_sw_version[0] = '\0';

    status = ril_request_manufacturer_identification(RIL_ACTIVE_MODE, device_ril_callback, NULL);
    if (status != RIL_STATUS_SUCCESS) {
        LOG("get manufacturer failed");
    }

    status = ril_request_model_identification(RIL_ACTIVE_MODE, device_ril_callback, NULL);
    if (status != RIL_STATUS_SUCCESS) {
        LOG("get model number failed");
    }

    status = ril_request_serial_number(RIL_EXECUTE_MODE, 1, device_ril_callback, NULL);
    if (status != RIL_STATUS_SUCCESS) {
        LOG("get serial number failed");
    }

    status = ril_request_request_hw_version(RIL_ACTIVE_MODE, device_ril_callback, NULL);
    if (status != RIL_STATUS_SUCCESS) {
        LOG("get hardware version failed");
    }

    status = ril_request_revision_identification(RIL_ACTIVE_MODE, device_ril_callback, NULL);
    if (status != RIL_STATUS_SUCCESS) {
        LOG("get software version failed");
    }
}

lwm2m_object_t * get_object_device()
{
    /*
     * The get_object_device function create the object itself and return a pointer to the structure that represent it.
     */
    lwm2m_object_t * deviceObj;

    deviceObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != deviceObj)
    {
        device_init();
        memset(deviceObj, 0, sizeof(lwm2m_object_t));

        /*
         * It assigns his unique ID
         * The 3 is the standard ID for the mandatory object "Object device".
         */
        deviceObj->objID = LWM2M_DEVICE_OBJECT_ID;

        /*
         * and its unique instance
         *
         */
        deviceObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
        if (NULL != deviceObj->instanceList)
        {
            memset(deviceObj->instanceList, 0, sizeof(lwm2m_list_t));
        }
        else
        {
            lwm2m_free(deviceObj);
            return NULL;
        }

        /*
         * And the private function that will access the object.
         * Those function will be called when a read/write/execute query is made by the server. In fact the library don't need to
         * know the resources of the object, only the server does.
         */
        deviceObj->readFunc     = prv_device_read;
        deviceObj->discoverFunc = prv_device_discover;
        deviceObj->writeFunc    = prv_device_write;
        deviceObj->executeFunc  = prv_device_execute;
        deviceObj->userData = lwm2m_malloc(sizeof(device_data_t));

        /*
         * Also some user data can be stored in the object with a private structure containing the needed variables
         */
        if (NULL != deviceObj->userData)
        {
        #if 0
            ((device_data_t*)deviceObj->userData)->battery_level = PRV_BATTERY_LEVEL;
        #else
            uint32_t battery_vol = device_get_battery_voltage();
            ((device_data_t*)deviceObj->userData)->battery_level = device_get_battery_logic_value(battery_vol);
        #endif
            ((device_data_t*)deviceObj->userData)->free_memory   = PRV_MEMORY_FREE;
            ((device_data_t*)deviceObj->userData)->error = PRV_ERROR_CODE;
            ((device_data_t*)deviceObj->userData)->time  = lwm2m_gettime();
            strcpy(((device_data_t*)deviceObj->userData)->time_offset, "+08:00");
        }
        else
        {
            lwm2m_free(deviceObj->instanceList);
            lwm2m_free(deviceObj);
            deviceObj = NULL;
        }
    }

    return deviceObj;
}

void free_object_device(lwm2m_object_t * objectP)
{
    if (NULL != objectP->userData)
    {
        lwm2m_free(objectP->userData);
        objectP->userData = NULL;
    }
    if (NULL != objectP->instanceList)
    {
        lwm2m_free(objectP->instanceList);
        objectP->instanceList = NULL;
    }

    lwm2m_free(objectP);
}

uint8_t device_change(lwm2m_data_t * dataArray,
                      lwm2m_object_t * objectP)
{
    uint8_t result;

    switch (dataArray->id)
    {
    case RES_O_BATTERY_LEVEL:
            {
                int64_t value;
                if (1 == lwm2m_data_decode_int(dataArray, &value))
                {
                    if ((0 <= value) && (100 >= value))
                    {
                        ((device_data_t*)(objectP->userData))->battery_level = value;
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
        case RES_M_ERROR_CODE:
            if (1 == lwm2m_data_decode_int(dataArray, &((device_data_t*)(objectP->userData))->error))
            {
                result = COAP_204_CHANGED;
            }
            else
            {
                result = COAP_400_BAD_REQUEST;
            }
            break;
        case RES_O_MEMORY_FREE:
            if (1 == lwm2m_data_decode_int(dataArray, &((device_data_t*)(objectP->userData))->free_memory))
            {
                result = COAP_204_CHANGED;
            }
            else
            {
                result = COAP_400_BAD_REQUEST;
            }
            break;
        default:
            result = COAP_405_METHOD_NOT_ALLOWED;
            break;
        }

    return result;
}

char * device_get_hw_version(void)
{
    if (device_hw_version[0] != '\0')
    {
        return device_hw_version;
    }
    else
    {
        return PRV_HARDWARE_VERSION;
    }
}

char * device_get_sw_version(void)
{
    if (device_sw_version[0] != '\0')
    {
        return device_sw_version;
    }
    else
    {
        return PRV_SOFTWARE_VERSION;
    }
}

