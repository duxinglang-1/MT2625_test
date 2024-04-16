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
 *    Julien Vermillard - initial implementation
 *    Fabien Fleutot - Please refer to git log
 *    David Navarro, Intel Corporation - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *    
 *******************************************************************************/

/*
 * This object is single instance only, and provide firmware upgrade functionality.
 * Object ID is 5.
 */

/*
 * resources:
 * 0 package                   write
 * 1 package url               write
 * 2 update                    exec
 * 3 state                     read
 * 4 update supported objects  read/write
 * 5 update result             read
 */

#include "liblwm2m.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "syslog.h"
#ifdef MTK_FOTA_ENABLE
#include "fota.h"
#endif
#include "FreeRTOS.h"

log_create_module(obj_firmware, PRINT_LEVEL_INFO);

/******************************************************************************************
 *                               Local Macro Definition                                   *
 *****************************************************************************************/
#define OBJ_FIRMWARE_ERR(fmt,arg...)   LOG_E(obj_firmware, "[Obj_firmware]: "fmt,##arg)
#define OBJ_FIRMWARE_WARN(fmt,arg...)  LOG_W(obj_firmware, "[Obj_firmware]: "fmt,##arg)
#define OBJ_FIRMWARE_DBG(fmt,arg...)   LOG_I(obj_firmware,"[Obj_firmware]: "fmt,##arg)


// ---- private object "Firmware" specific defines ----
// Resource Id's:
#define RES_M_PACKAGE                   0
#define RES_M_PACKAGE_URI               1
#define RES_M_UPDATE                    2
#define RES_M_STATE                     3
#define RES_O_UPDATE_SUPPORTED_OBJECTS  4
#define RES_M_UPDATE_RESULT             5
#define RES_O_PKG_NAME                  6
#define RES_O_PKG_VERSION               7
#define RES_O_PROTOCOL_SUPPORT          8
#define RES_M_DELIVERY_METHOD           9

typedef enum {
    OBJ_FIRMWARE_UPDATE_RESULT_INIT                 = 0,
    OBJ_FIRMWARE_UPDATE_RESULT_OK                   = 1,
    OBJ_FIRMWARE_UPDATE_RESULT_NO_FLASH_MEM         = 2,
    OBJ_FIRMWARE_UPDATE_RESULT_OUT_OF_RAM           = 3,
    OBJ_FIRMWARE_UPDATE_RESULT_CONNECTION_LOST      = 4,
    OBJ_FIRMWARE_UPDATE_RESULT_INTEGRITY_FAIL       = 5,
    OBJ_FIRMWARE_UPDATE_RESULT_UNSUPPORTED_PACKAGE  = 6,
    OBJ_FIRMWARE_UPDATE_RESULT_INVALID_URI          = 7,
    OBJ_FIRMWARE_UPDATE_RESULT_UPDATE_FAILED        = 8,
    OBJ_FIRMWARE_UPDATE_RESULT_UNSUPPORTED_PROTOCOL = 9
}obj_firmware_update_result_t;

typedef enum {
    OBJ_FIRMWARE_PROTOCOL_COAP    = 0,
    OBJ_FIRMWARE_PROTOCOL_COAPS   = 1,
    OBJ_FIRMWARE_PROTOCOL_HTTP11  = 2,
    OBJ_FIRMWARE_PROTOCOL_HTTPS11 = 3
}obj_firmware_protocol_t;

typedef enum {
    OBJ_FIRMWARE_DELIVERY_METHOD_PULL = 0,
    OBJ_FIRMWARE_DELIVERY_METHOD_PUSH = 1,
    OBJ_FIRMWARE_DELIVERY_METHOD_BOTH = 2
}obj_firmware_delivery_method_t;


void firmware_set_instance(lwm2m_context_t* instance);

const char *obj_package_string         = "/5/0/0";
const char *obj_package_uri_string         = "/5/0/1";
const char *obj_update_uri_string          = "/5/0/2";
const char *obj_state_uri_string           = "/5/0/3";
const char *obj_update_result_uri_string   = "/5/0/5";
const char *obj_pkg_name_uri_string        = "/5/0/6";
const char *obj_pkg_version_uri_string     = "/5/0/7";
const char *obj_protocol_uri_string        = "/5/0/8";
const char *obj_delivery_method_uri_string = "/5/0/9";


typedef enum {
    OBJECT_FIRMWARE_STATE_IDLE               = 0,
    OBJECT_FIRMWARE_STATE_DOWNLOADING        = 1,
    OBJECT_FIRMWARE_STATE_DOWNLOAD_COMPLETED = 2,
    OBJECT_FIRMWARE_STATE_UPGRADING          = 3
}object_firmware_state_t;

typedef struct
{
    uint8_t state;
    bool supported;
    uint8_t result;
} firmware_data_t;

typedef struct
{
    object_firmware_state_t firmware_state;
    #ifdef MTK_FOTA_ENABLE
    fota_result_t result;
    #endif
    char* pkg_uri;
    uint32_t progress_info;
} firmware_cur_state_t;

static lwm2m_context_t* g_lwm2m_instance = NULL;
static firmware_cur_state_t g_firmware_cur_state = {0};
#define LWM2M_INSTANCE g_lwm2m_instance

void firmware_set_instance(lwm2m_context_t* instance)
{
    OBJ_FIRMWARE_DBG("new lwm2m instance: 0x%x\r\n", instance);
    g_lwm2m_instance = instance;
}

static void object_firmware_send_data(lwm2m_context_t *lwm2mH, char* obj_uri, uint32_t data)
{
    lwm2m_uri_t uri;
    lwm2m_stringToUri(obj_uri, strlen(obj_uri), &uri);
    char temp[32] = {0};
    OBJ_FIRMWARE_DBG("object firmware broadcast data\r\n");
    if (lwm2mH == NULL) {
        return;
    } else {
        snprintf(temp, sizeof(temp), "%d", data);
        handle_value_changed(lwm2mH, &uri, (const char *)temp, strlen(temp));
        OBJ_FIRMWARE_DBG("object firmware broadcast data OK\r\n");
    }
}
static void object_firmware_send_string_data(lwm2m_context_t *lwm2mH, char* obj_uri, char* string)
{
    lwm2m_uri_t uri;
    lwm2m_stringToUri(obj_uri, strlen(obj_uri), &uri);
    if (lwm2mH == NULL) {
        return;
    } else {
        handle_value_changed(lwm2mH, &uri, (const char *)string, strlen(string));
    }
}

#ifdef MTK_FOTA_ENABLE
static void prv_firmware_fota_event_indication(fota_msg_event_t event, fota_msg_event_info_t* p_info)
{
    if (p_info == NULL)
    {
        return;
    }
    switch (event)
    {
        case FOTA_MSG_DOWNLOAD_RESULT_IND:
        {
            if (g_firmware_cur_state.pkg_uri != NULL) {
                vPortFree(g_firmware_cur_state.pkg_uri);
                g_firmware_cur_state.pkg_uri = NULL;
                object_firmware_send_string_data(LWM2M_INSTANCE, obj_pkg_name_uri_string, " ");
                object_firmware_send_string_data(LWM2M_INSTANCE, obj_package_uri_string, " ");
            } 
            if (p_info->fota_result == FOTA_OK) {
                g_firmware_cur_state.firmware_state = OBJECT_FIRMWARE_STATE_DOWNLOAD_COMPLETED;
            } else {
                g_firmware_cur_state.firmware_state = OBJECT_FIRMWARE_STATE_IDLE;
            }
            OBJ_FIRMWARE_DBG("FOTA_MSG_DOWNLOAD_RESULT_IND");
            object_firmware_send_data(LWM2M_INSTANCE, obj_state_uri_string, g_firmware_cur_state.firmware_state);
            break;
        }
        case FOTA_MSG_UPGRADE_RESULT_IND:
        {
            g_firmware_cur_state.firmware_state = OBJECT_FIRMWARE_STATE_IDLE;
            if (p_info->fota_result == FOTA_OK) {
                g_firmware_cur_state.result = OBJ_FIRMWARE_UPDATE_RESULT_OK;
            } else {
                g_firmware_cur_state.result = OBJ_FIRMWARE_UPDATE_RESULT_UPDATE_FAILED;
            }
            OBJ_FIRMWARE_DBG("FOTA_MSG_UPGRADE_RESULT_IND");
            object_firmware_send_data(LWM2M_INSTANCE, obj_state_uri_string, g_firmware_cur_state.firmware_state);
            object_firmware_send_data(LWM2M_INSTANCE, obj_update_result_uri_string, g_firmware_cur_state.result);
            break;
        }
        case FOTA_MSG_DOWNLOAD_PROGRESS_IND:
        {
            //g_firmware_cur_state.firmware_state = OBJECT_FIRMWARE_STATE_DOWNLOADING;
            //g_firmware_cur_state.progress_info = p_info->progress;
            //object_firmware_send_data(LWM2M_INSTANCE, obj_state_uri_string, g_firmware_cur_state.firmware_state);
            break;
        }
        case FOTA_MSG_UPGRADE_PROGRESS_IND:
        {
            //g_firmware_cur_state.firmware_state = OBJECT_FIRMWARE_STATE_UPGRADING;
            //g_firmware_cur_state.progress_info = p_info->progress;
            //object_firmware_send_data(LWM2M_INSTANCE, obj_state_uri_string, g_firmware_cur_state.firmware_state);
            break;
        }
        default:
        {
            break;
        }
    }
}
#endif
static uint8_t prv_firmware_read(uint16_t instanceId,
                                 int * numDataP,
                                 lwm2m_data_t ** dataArrayP,
                                 lwm2m_object_t * objectP)
{
    int i;
    uint8_t result;
    firmware_data_t * data = (firmware_data_t*)(objectP->userData);
    data->state = g_firmware_cur_state.firmware_state;

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }


    if (*numDataP == 0) {
        uint16_t resList[] = {
            RES_M_PACKAGE,
            RES_M_PACKAGE_URI,
            RES_M_UPDATE,
            RES_M_STATE,
            RES_O_UPDATE_SUPPORTED_OBJECTS,
            RES_M_UPDATE_RESULT,
            RES_O_PKG_NAME,
            RES_O_PKG_VERSION,
            RES_O_PROTOCOL_SUPPORT,
            RES_M_DELIVERY_METHOD
        };
        int nbRes = sizeof(resList)/sizeof(uint16_t);

        *dataArrayP = lwm2m_data_new(nbRes);
        if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
        *numDataP = nbRes;
        for (i = 0 ; i < nbRes ; i++){
            (*dataArrayP)[i].id = resList[i];
        }
    }

    i = 0;
    do
    {
        switch ((*dataArrayP)[i].id)
        {
        case RES_M_PACKAGE:
        case RES_M_PACKAGE_URI:
        case RES_M_UPDATE:
            result = COAP_405_METHOD_NOT_ALLOWED;
            break;
        case RES_M_DELIVERY_METHOD: {
            lwm2m_data_encode_int(OBJ_FIRMWARE_DELIVERY_METHOD_PULL, *dataArrayP + i);
            result = COAP_205_CONTENT;
            break;
        }
        case RES_O_PROTOCOL_SUPPORT: {
            lwm2m_data_encode_int(OBJ_FIRMWARE_PROTOCOL_COAP, *dataArrayP + i);
            result = COAP_205_CONTENT;
            break;
        }
        case RES_M_STATE:
            // firmware update state (int)
            lwm2m_data_encode_int(data->state, *dataArrayP + i);
            result = COAP_205_CONTENT;
            break;

        case RES_O_UPDATE_SUPPORTED_OBJECTS:
            lwm2m_data_encode_bool(data->supported, *dataArrayP + i);
            result = COAP_205_CONTENT;
            break;

        case RES_M_UPDATE_RESULT: {
            OBJ_FIRMWARE_DBG("read upgrade result.");
            if (g_firmware_cur_state.firmware_state == OBJECT_FIRMWARE_STATE_IDLE)
            {
                #ifdef MTK_FOTA_ENABLE
                fota_result_t fota_result = fota_read_upgrade_result();
                data->result = (uint8_t)fota_result;
                lwm2m_data_encode_int(data->result, *dataArrayP + i);
                fota_clear_history();
                #endif
                result = COAP_205_CONTENT;
            }
            else
            {
                data->result = 0xFF;
                lwm2m_data_encode_int(data->result, *dataArrayP + i);
                result = COAP_205_CONTENT;
            }
            break;
        }
        default:
            result = COAP_404_NOT_FOUND;
        }

        i++;
    } while (i < *numDataP && result == COAP_205_CONTENT);

    return result;
}

static uint8_t prv_firmware_write(void * contextP,
                                  uint16_t instanceId,
                                  int numData,
                                  lwm2m_data_t * dataArray,
                                  lwm2m_object_t * objectP)
{
    int i;
    uint8_t result;
    firmware_data_t * data = (firmware_data_t*)(objectP->userData);
    uint32_t uri_len = 0;
    char* p_uri = NULL;

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }

    i = 0;

    do
    {
        OBJ_FIRMWARE_DBG("dataArray[%d].type: %d, ID: %d\r\n", i, dataArray[i].type, dataArray[i].id);
        switch (dataArray[i].id)
        {
        case RES_M_PACKAGE:
            // inline firmware binary
            result = COAP_204_CHANGED;
            break;
        case RES_M_STATE:
            result = COAP_204_CHANGED;
            break;
        case RES_M_PACKAGE_URI:
            // URL for download the firmware
            uri_len = dataArray[i].value.asBuffer.length;
            p_uri = pvPortMalloc(uri_len + 1);
            
            if (p_uri != NULL){
                #ifdef MTK_FOTA_ENABLE
                fota_result_t fota_result;
                #endif
                memset(p_uri, 0, uri_len + 1);
                memcpy(p_uri, dataArray[i].value.asBuffer.buffer, uri_len);
                OBJ_FIRMWARE_DBG("the firmware download URI:%s\r\n", p_uri);
                if (g_firmware_cur_state.pkg_uri != NULL) {
                    vPortFree(g_firmware_cur_state.pkg_uri);
                    g_firmware_cur_state.pkg_uri = pvPortMalloc(uri_len + 1);
                    memset(g_firmware_cur_state.pkg_uri, 0, uri_len + 1);
                    memcpy(g_firmware_cur_state.pkg_uri, dataArray[i].value.asBuffer.buffer, uri_len);
                    object_firmware_send_string_data((lwm2m_context_t *)contextP, obj_pkg_name_uri_string, g_firmware_cur_state.pkg_uri);
                    object_firmware_send_string_data((lwm2m_context_t *)contextP, obj_package_uri_string, g_firmware_cur_state.pkg_uri);
                }
                #ifdef MTK_FOTA_ENABLE
                fota_result = fota_download_image(FOTA_ALL_BIN, p_uri);
                if (fota_result == FOTA_OK){
                    g_firmware_cur_state.firmware_state = OBJECT_FIRMWARE_STATE_DOWNLOADING;
                    data->state = g_firmware_cur_state.firmware_state;
                    data->result = fota_result;
                    object_firmware_send_data((lwm2m_context_t *)contextP, obj_state_uri_string, g_firmware_cur_state.firmware_state);
                    
                }else {
                    g_firmware_cur_state.firmware_state = OBJECT_FIRMWARE_STATE_IDLE;
                    data->state = g_firmware_cur_state.firmware_state;
                    data->result = OBJ_FIRMWARE_UPDATE_RESULT_INVALID_URI;
                    object_firmware_send_data((lwm2m_context_t *)contextP, obj_state_uri_string, g_firmware_cur_state.firmware_state);
                    object_firmware_send_string_data((lwm2m_context_t *)contextP, obj_pkg_name_uri_string, " ");
                }
                #endif
                vPortFree(p_uri);
                p_uri = NULL;
            }
            result = COAP_204_CHANGED;
            break;

        case RES_O_UPDATE_SUPPORTED_OBJECTS:
            if (lwm2m_data_decode_bool(&dataArray[i], &data->supported) == 1)
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
        }

        i++;
    } while (i < numData && result == COAP_204_CHANGED);

    return result;
}

static uint8_t prv_firmware_execute(void * contextP,
                                    uint16_t instanceId,
                                    uint16_t resourceId,
                                    uint8_t * buffer,
                                    int length,
                                    lwm2m_object_t * objectP)
{
    firmware_data_t * data = (firmware_data_t*)(objectP->userData);
    uint8_t* p_data = NULL;
    coap_status_t result = 0;

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }

    if (length != 0) return COAP_400_BAD_REQUEST;

    // for execute callback, resId is always set.
    switch (resourceId)
    {
    case RES_M_UPDATE:{
        #ifdef MTK_FOTA_ENABLE
        fota_result_t fota_result;
        #endif
        OBJ_FIRMWARE_DBG("trigger fota update.");
        if ((g_firmware_cur_state.firmware_state == OBJECT_FIRMWARE_STATE_DOWNLOAD_COMPLETED)
            || (g_firmware_cur_state.firmware_state == OBJECT_FIRMWARE_STATE_IDLE)){
            #ifdef MTK_FOTA_ENABLE
            fota_result = fota_trigger_upgrade(FOTA_MAIN_BIN);
            if (fota_result == FOTA_OK){
                g_firmware_cur_state.firmware_state = OBJECT_FIRMWARE_STATE_UPGRADING;
                g_firmware_cur_state.result = fota_result;
                object_firmware_send_data((lwm2m_context_t *)contextP, obj_state_uri_string, g_firmware_cur_state.firmware_state);
            }else{
                g_firmware_cur_state.result = fota_result;
                object_firmware_send_data((lwm2m_context_t *)contextP, obj_state_uri_string, g_firmware_cur_state.firmware_state);
            }
            #endif
            result = COAP_204_CHANGED;
        }else{
            result = COAP_405_METHOD_NOT_ALLOWED;
        }
        break;
    }
    default:
        result = COAP_405_METHOD_NOT_ALLOWED;
        break;
    }

    return result;
}

void display_firmware_object(lwm2m_object_t * object)
{
#ifdef WITH_LOGS
    firmware_data_t * data = (firmware_data_t *)object->userData;
    fprintf(stdout, "  /%u: Firmware object:\r\n", object->objID);
    if (NULL != data)
    {
        fprintf(stdout, "    state: %u, supported: %s, result: %u\r\n",
                data->state, data->supported?"true":"false", data->result);
    }
#endif
}

lwm2m_object_t * get_object_firmware(void)
{
    /*
     * The get_object_firmware function create the object itself and return a pointer to the structure that represent it.
     */
    lwm2m_object_t * firmwareObj;

    firmwareObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != firmwareObj)
    {
        memset(firmwareObj, 0, sizeof(lwm2m_object_t));

        /*
         * It assigns its unique ID
         * The 5 is the standard ID for the optional object "Object firmware".
         */
        firmwareObj->objID = LWM2M_FIRMWARE_UPDATE_OBJECT_ID;

        /*
         * and its unique instance
         *
         */
        firmwareObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
        if (NULL != firmwareObj->instanceList)
        {
            memset(firmwareObj->instanceList, 0, sizeof(lwm2m_list_t));
        }
        else
        {
            lwm2m_free(firmwareObj);
            return NULL;
        }

        /*
         * And the private function that will access the object.
         * Those function will be called when a read/write/execute query is made by the server. In fact the library don't need to
         * know the resources of the object, only the server does.
         */
        firmwareObj->readFunc    = prv_firmware_read;
        firmwareObj->writeFunc   = prv_firmware_write;
        firmwareObj->executeFunc = prv_firmware_execute;
        firmwareObj->userData    = lwm2m_malloc(sizeof(firmware_data_t));

        /*
         * Also some user data can be stored in the object with a private structure containing the needed variables
         */
        if (NULL != firmwareObj->userData)
        {
            ((firmware_data_t*)firmwareObj->userData)->state = 0;
            ((firmware_data_t*)firmwareObj->userData)->supported = false;
            ((firmware_data_t*)firmwareObj->userData)->result = 0;
            #ifdef MTK_FOTA_ENABLE
            fota_register_event(prv_firmware_fota_event_indication);
            #endif
        }
        else
        {
            lwm2m_free(firmwareObj);
            firmwareObj = NULL;
        }
    }

    return firmwareObj;
}

void free_object_firmware(lwm2m_object_t * objectP)
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

#ifdef MTK_CTIOT_SUPPORT
void lwm2m_restore_result_callback(bool result, void *user_data)
{
    OBJ_FIRMWARE_DBG("lwm2m restore result:%d\r\n", result);
    if (result == true) {
        #ifdef MTK_FOTA_ENABLE
        fota_result_t fota_result = fota_read_upgrade_result();
        g_firmware_cur_state.result = fota_result;
        object_firmware_send_data(LWM2M_INSTANCE, obj_state_uri_string, g_firmware_cur_state.firmware_state);
        object_firmware_send_data(LWM2M_INSTANCE, obj_update_result_uri_string, g_firmware_cur_state.result);
        fota_clear_history();
        #endif
    }
}
#endif


