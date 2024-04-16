/*******************************************************************************
 *
 * Copyright (c) 2014 Bosch Software Innovations GmbH Germany. 
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
 *******************************************************************************/

/*
 *  This Connectivity Monitoring object is optional and has a single instance
 * 
 *  Resources:
 *
 *          Name             | ID | Oper. | Inst. | Mand.|  Type   | Range | Units |
 *  Network Bearer           |  0 |  R    | Single|  Yes | Integer |       |       |
 *  Available Network Bearer |  1 |  R    | Multi |  Yes | Integer |       |       |
 *  Radio Signal Strength    |  2 |  R    | Single|  Yes | Integer |       | dBm   |
 *  Link Quality             |  3 |  R    | Single|  No  | Integer | 0-100 |   %   |
 *  IP Addresses             |  4 |  R    | Multi |  Yes | String  |       |       |
 *  Router IP Addresses      |  5 |  R    | Multi |  No  | String  |       |       |
 *  Link Utilization         |  6 |  R    | Single|  No  | Integer | 0-100 |   %   |
 *  APN                      |  7 |  R    | Multi |  No  | String  |       |       |
 *  Cell ID                  |  8 |  R    | Single|  No  | Integer |       |       |
 *  SMNC                     |  9 |  R    | Single|  No  | Integer | 0-999 |   %   |
 *  SMCC                     | 10 |  R    | Single|  No  | Integer | 0-999 |       |
 *
 */

#include "liblwm2m.h"
#include "internals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ril.h"
#include "syslog.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "netif.h"
#include "timers.h"

// Resource Id's:
#define RES_M_NETWORK_BEARER            0
#define RES_M_AVL_NETWORK_BEARER        1
#define RES_M_RADIO_SIGNAL_STRENGTH     2
#define RES_O_LINK_QUALITY              3
#define RES_M_IP_ADDRESSES              4
#define RES_O_ROUTER_IP_ADDRESS         5
#define RES_O_LINK_UTILIZATION          6
#define RES_O_APN                       7
#define RES_O_CELL_ID                   8
#define RES_O_SMNC                      9
#define RES_O_SMCC                      10

#define VALUE_NETWORK_BEARER_IOT    7   //NB-IoT 
#define VALUE_AVL_NETWORK_BEARER_1  7   //NB-IoT
#define VALUE_AVL_NETWORK_BEARER_2  21  //WLAN
#define VALUE_AVL_NETWORK_BEARER_3  41  //Ethernet
#define VALUE_AVL_NETWORK_BEARER_4  42  //DSL
#define VALUE_AVL_NETWORK_BEARER_5  43  //PLC
#define VALUE_IP_ADDRESS_1              "192.168.178.101"
#define VALUE_IP_ADDRESS_2              "192.168.178.102"
#define VALUE_ROUTER_IP_ADDRESS_1       "192.168.178.001"
#define VALUE_ROUTER_IP_ADDRESS_2       "192.168.178.002"
#define VALUE_APN_1                     "web.vodafone.de"
#define VALUE_APN_2                     "cda.vodafone.de"
#define VALUE_CELL_ID                   69696969
#define VALUE_RADIO_SIGNAL_STRENGTH     80                  //dBm
#define VALUE_LINK_QUALITY              98     
#define VALUE_LINK_UTILIZATION          666
#define VALUE_SMNC                      33
#define VALUE_SMCC                      44

typedef struct
{
    char ipAddresses[3][40];        // limited to 3! maybe ipv4, ipv6, local address
    char routerIpAddresses[2][16];  // limited to 2!
    int linkUtilization;
} conn_m_data_t;

static SemaphoreHandle_t connectivity_moni_mutex_handle;
static int connectivity_moni_signalStrength;
static int connectivity_moni_linkQuality;
static int connectivity_moni_cellId;
static int connectivity_moni_smnc;
static int connectivity_moni_smcc;

static int32_t connectivity_moni_ril_callback(ril_cmd_response_t *cmd_response)
{
    switch (cmd_response->cmd_id) {
        case RIL_CMD_ID_CESQ: {
            ril_extended_signal_quality_rsp_t *response = (ril_extended_signal_quality_rsp_t *)cmd_response->cmd_param;
            if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                connectivity_moni_signalStrength = response->rsrp - 140;
                connectivity_moni_linkQuality = response->rsrq;
                LOG_ARG("signalStrength = %d, linkQuality = %d", connectivity_moni_signalStrength, connectivity_moni_linkQuality);
            }
            xSemaphoreGive(connectivity_moni_mutex_handle);
        }
        break;

        case RIL_CMD_ID_CEREG: {
            ril_eps_network_registration_status_rsp_t *response = (ril_eps_network_registration_status_rsp_t *)cmd_response->cmd_param;
            if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                connectivity_moni_cellId = response->ci;
                LOG_ARG("cellId = %d", connectivity_moni_cellId);
            }
            xSemaphoreGive(connectivity_moni_mutex_handle);
        }
        break;

        case RIL_CMD_ID_MHOMENW: {
            ril_display_home_network_information_rsp_t *response = (ril_display_home_network_information_rsp_t *)cmd_response->cmd_param;
            if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                if (response->oper_numeric <= 999) {
                    connectivity_moni_smnc = 0;
                    connectivity_moni_smcc = response->oper_numeric;
                } else if (response->oper_numeric <= 9999) {
                    /* MCC (3) + MNC (1) */
                    connectivity_moni_smnc = response->oper_numeric % 10;
                    connectivity_moni_smcc = response->oper_numeric / 10;
                } else if (response->oper_numeric <= 99999) {
                    /* MCC (3) + MNC (2) */
                    connectivity_moni_smnc = response->oper_numeric % 100;
                    connectivity_moni_smcc = response->oper_numeric / 100;
                } else {
                    /* MCC (3) + MNC (3) */
                    connectivity_moni_smnc = response->oper_numeric % 1000;
                    connectivity_moni_smcc = response->oper_numeric / 1000;
                }
                LOG_ARG("smnc = %d, smcc = %d", connectivity_moni_smnc, connectivity_moni_smcc);
            }
            xSemaphoreGive(connectivity_moni_mutex_handle);
        }
        break;

        default:
            break;
    }

    return 0;
}

static void connectivity_moni_get_signal_quality(void)
{
    ril_status_t status;

    xSemaphoreTake(connectivity_moni_mutex_handle, portMAX_DELAY);
    status = ril_request_extended_signal_quality(RIL_ACTIVE_MODE, connectivity_moni_ril_callback, NULL);
    if (status != RIL_STATUS_SUCCESS) {
        xSemaphoreGive(connectivity_moni_mutex_handle);
        LOG("get signal quality failed");
        return;
    }

    /* wait for RIL callback */
    xSemaphoreTake(connectivity_moni_mutex_handle, portMAX_DELAY);
    xSemaphoreGive(connectivity_moni_mutex_handle);
}

static void connectivity_moni_get_cell_id(void)
{
    ril_status_t status;

    xSemaphoreTake(connectivity_moni_mutex_handle, portMAX_DELAY);
    status = ril_request_eps_network_registration_status(RIL_READ_MODE, RIL_OMITTED_INTEGER_PARAM, connectivity_moni_ril_callback, NULL);
    if (status != RIL_STATUS_SUCCESS) {
        xSemaphoreGive(connectivity_moni_mutex_handle);
        LOG("get cell id failed");
        return;
    }

    /* wait for RIL callback */
    xSemaphoreTake(connectivity_moni_mutex_handle, portMAX_DELAY);
    xSemaphoreGive(connectivity_moni_mutex_handle);
}

static void connectivity_moni_get_home_plmn(void)
{
    ril_status_t status;

    xSemaphoreTake(connectivity_moni_mutex_handle, portMAX_DELAY);
    status = ril_request_display_home_network_information(RIL_ACTIVE_MODE, connectivity_moni_ril_callback, NULL);
    if (status != RIL_STATUS_SUCCESS) {
        xSemaphoreGive(connectivity_moni_mutex_handle);
        LOG("get home plmn failed");
    }

    /* wait for RIL callback */
    xSemaphoreTake(connectivity_moni_mutex_handle, portMAX_DELAY);
    xSemaphoreGive(connectivity_moni_mutex_handle);
}

static uint8_t prv_set_value(lwm2m_data_t * dataP,
                             conn_m_data_t * connDataP)
{
    switch (dataP->id)
    {
    case RES_M_NETWORK_BEARER:
        lwm2m_data_encode_int(VALUE_NETWORK_BEARER_IOT, dataP);
        return COAP_205_CONTENT;

    case RES_M_AVL_NETWORK_BEARER:
    {
        int riCnt = 1;   // reduced to 1 instance to fit in one block size
        lwm2m_data_t * subTlvP;
        subTlvP = lwm2m_data_new(riCnt);
        subTlvP[0].id    = 0;
        lwm2m_data_encode_int(VALUE_AVL_NETWORK_BEARER_1, subTlvP);
        lwm2m_data_encode_instances(subTlvP, riCnt, dataP);
        return COAP_205_CONTENT ;
    }

    case RES_M_RADIO_SIGNAL_STRENGTH: //s-int
        connectivity_moni_get_signal_quality();
        lwm2m_data_encode_int(connectivity_moni_signalStrength, dataP);
        return COAP_205_CONTENT;

    case RES_O_LINK_QUALITY: //s-int
        connectivity_moni_get_signal_quality();
        lwm2m_data_encode_int(connectivity_moni_linkQuality, dataP);
        return COAP_205_CONTENT ;

    case RES_M_IP_ADDRESSES:
    {
        uint32_t i, j, index = 0, count = netif_get_if_count();
        netifreq netif_req;
        if (count > 3) {
            count = 3; // limited to 3! maybe ipv4, ipv6, local address
        }
        for (i = 0; i < count; i++) {
            if (netif_get_network_if_by_num(&netif_req, i) == true) {
                if (netif_req.be_ip4 == true) {
                    sprintf(connDataP->ipAddresses[index], "%d.%d.%d.%d",
                            ip4_addr1_16(&(netif_req.ip_addr)), ip4_addr2_16(&(netif_req.ip_addr)),
                            ip4_addr3_16(&(netif_req.ip_addr)), ip4_addr4_16(&(netif_req.ip_addr)));
                    index++;
                } else {
                    for (j = 0; j < netif_req.ip6_count; j++) {
                        sprintf(connDataP->ipAddresses[index], "%x:%x:%x:%x:%x:%x:%x:%x",
                                IP6_ADDR_BLOCK1(netif_req.ip6_addr + j),
                                IP6_ADDR_BLOCK2(netif_req.ip6_addr + j),
                                IP6_ADDR_BLOCK3(netif_req.ip6_addr + j),
                                IP6_ADDR_BLOCK4(netif_req.ip6_addr + j),
                                IP6_ADDR_BLOCK5(netif_req.ip6_addr + j),
                                IP6_ADDR_BLOCK6(netif_req.ip6_addr + j),
                                IP6_ADDR_BLOCK7(netif_req.ip6_addr + j),
                                IP6_ADDR_BLOCK8(netif_req.ip6_addr + j));
                        index++;
                    }
                }
            }
        }
        int ri, riCnt = (index == 0) ? 1 : index;
        lwm2m_data_t* subTlvP = lwm2m_data_new(riCnt);
        for (ri = 0; ri < riCnt; ri++)
        {
            subTlvP[ri].id = ri;
            lwm2m_data_encode_string(connDataP->ipAddresses[ri], subTlvP + ri);
        }
        lwm2m_data_encode_instances(subTlvP, riCnt, dataP);
        return COAP_205_CONTENT ;
    }
        break;

    case RES_O_ROUTER_IP_ADDRESS:
    {
        int ri, riCnt = 1;   // reduced to 1 instance to fit in one block size
        lwm2m_data_t* subTlvP = lwm2m_data_new(riCnt);
        for (ri=0; ri<riCnt; ri++)
        {
            subTlvP[ri].id = ri;
            lwm2m_data_encode_string(connDataP->routerIpAddresses[ri], subTlvP + ri);
        }
        lwm2m_data_encode_instances(subTlvP, riCnt, dataP);
        return COAP_205_CONTENT ;
    }
        break;

    case RES_O_LINK_UTILIZATION:
        lwm2m_data_encode_int(connDataP->linkUtilization, dataP);
        return COAP_205_CONTENT;

    case RES_O_APN:
    {
        int riCnt = 1;   // reduced to 1 instance to fit in one block size
        lwm2m_data_t * subTlvP;
        subTlvP = lwm2m_data_new(riCnt);
        subTlvP[0].id     = 0;
        lwm2m_data_encode_string(VALUE_APN_1, subTlvP);
        lwm2m_data_encode_instances(subTlvP, riCnt, dataP);
        return COAP_205_CONTENT;
    }
        break;

    case RES_O_CELL_ID:
        connectivity_moni_get_cell_id();
        lwm2m_data_encode_int(connectivity_moni_cellId, dataP);
        return COAP_205_CONTENT ;

    case RES_O_SMNC:
        connectivity_moni_get_home_plmn();
        lwm2m_data_encode_int(connectivity_moni_smnc, dataP);
        return COAP_205_CONTENT ;

    case RES_O_SMCC:
        connectivity_moni_get_home_plmn();
        lwm2m_data_encode_int(connectivity_moni_smcc, dataP);
        return COAP_205_CONTENT ;

    default:
        return COAP_404_NOT_FOUND ;
    }
}

static uint8_t prv_read(uint16_t instanceId,
                        int * numDataP,
                        lwm2m_data_t ** dataArrayP,
                        lwm2m_object_t * objectP)
{
    uint8_t result;
    int i;

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND ;
    }

    // is the server asking for the full object ?
    if (*numDataP == 0)
    {
        uint16_t resList[] = {
                RES_M_NETWORK_BEARER,
                RES_M_AVL_NETWORK_BEARER,
                RES_M_RADIO_SIGNAL_STRENGTH,
                RES_O_LINK_QUALITY,
                RES_M_IP_ADDRESSES,
                RES_O_ROUTER_IP_ADDRESS,
                RES_O_LINK_UTILIZATION,
                RES_O_APN,
                RES_O_CELL_ID,
                RES_O_SMNC,
                RES_O_SMCC
        };
        int nbRes = sizeof(resList) / sizeof(uint16_t);

        *dataArrayP = lwm2m_data_new(nbRes);
        if (*dataArrayP == NULL)
            return COAP_500_INTERNAL_SERVER_ERROR ;
        *numDataP = nbRes;
        for (i = 0; i < nbRes; i++)
        {
            (*dataArrayP)[i].id = resList[i];
        }
    }

    i = 0;
    do
    {
        result = prv_set_value((*dataArrayP) + i, (conn_m_data_t*) (objectP->userData));
        i++;
    } while (i < *numDataP && result == COAP_205_CONTENT );

    return result;
}

static uint8_t prv_discover(uint16_t instanceId,
                            int * numDataP,
                            lwm2m_data_t ** dataArrayP,
                            lwm2m_object_t * objectP)
{
    uint8_t result;
    int i;

    result = COAP_205_CONTENT;

    // is the server asking for the full object ?
    if (*numDataP == 0)
    {
        uint16_t resList[] = {
                RES_M_NETWORK_BEARER,
                RES_M_AVL_NETWORK_BEARER,
                RES_M_RADIO_SIGNAL_STRENGTH,
                RES_O_LINK_QUALITY,
                RES_M_IP_ADDRESSES,
                RES_O_ROUTER_IP_ADDRESS,
                RES_O_LINK_UTILIZATION,
                RES_O_APN,
                RES_O_CELL_ID,
                RES_O_SMNC,
                RES_O_SMCC
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
                case RES_M_NETWORK_BEARER:
                case RES_M_AVL_NETWORK_BEARER:
                case RES_M_RADIO_SIGNAL_STRENGTH:
                case RES_O_LINK_QUALITY:
                case RES_M_IP_ADDRESSES:
                case RES_O_ROUTER_IP_ADDRESS:
                case RES_O_LINK_UTILIZATION:
                case RES_O_APN:
                case RES_O_CELL_ID:
                case RES_O_SMNC:
                case RES_O_SMCC:
                break;
                default:
                    result = COAP_404_NOT_FOUND;
            }
        }
    }

    return result;
}

static void connectivity_moni_init(void)
{
    connectivity_moni_mutex_handle = xSemaphoreCreateMutex();
    configASSERT(connectivity_moni_mutex_handle != NULL);
    connectivity_moni_signalStrength = VALUE_RADIO_SIGNAL_STRENGTH;
    connectivity_moni_linkQuality = VALUE_LINK_QUALITY;
    connectivity_moni_cellId = VALUE_CELL_ID;
    connectivity_moni_smnc = VALUE_SMNC;
    connectivity_moni_smcc = VALUE_SMCC;
}

lwm2m_object_t * get_object_conn_m(void)
{
    /*
     * The get_object_conn_m() function create the object itself and return a pointer to the structure that represent it.
     */
    lwm2m_object_t * connObj;

    connObj = (lwm2m_object_t *) lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != connObj)
    {
        connectivity_moni_init();
        memset(connObj, 0, sizeof(lwm2m_object_t));

        /*
         * It assigns his unique ID
         */
        connObj->objID = LWM2M_CONN_MONITOR_OBJECT_ID;
        
        /*
         * and its unique instance
         *
         */
        connObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
        if (NULL != connObj->instanceList)
        {
            memset(connObj->instanceList, 0, sizeof(lwm2m_list_t));
        }
        else
        {
            lwm2m_free(connObj);
            return NULL;
        }
        
        /*
         * And the private function that will access the object.
         * Those function will be called when a read/write/execute query is made by the server. In fact the library don't need to
         * know the resources of the object, only the server does.
         */
        connObj->readFunc = prv_read;
        connObj->executeFunc = NULL;
        connObj->discoverFunc = prv_discover;
        connObj->userData = lwm2m_malloc(sizeof(conn_m_data_t));

        /*
         * Also some user data can be stored in the object with a private structure containing the needed variables
         */
        if (NULL != connObj->userData)
        {
            conn_m_data_t *myData = (conn_m_data_t*) connObj->userData;
            myData->linkUtilization = VALUE_LINK_UTILIZATION;
            strcpy(myData->ipAddresses[0],       VALUE_IP_ADDRESS_1);
            strcpy(myData->ipAddresses[1],       VALUE_IP_ADDRESS_2);
            strcpy(myData->routerIpAddresses[0], VALUE_ROUTER_IP_ADDRESS_1);
            strcpy(myData->routerIpAddresses[1], VALUE_ROUTER_IP_ADDRESS_2);
        }
        else
        {
            lwm2m_free(connObj);
            connObj = NULL;
        }
    }
    return connObj;
}

void free_object_conn_m(lwm2m_object_t * objectP)
{
    lwm2m_free(objectP->userData);
    lwm2m_list_free(objectP->instanceList);
    lwm2m_free(objectP);
    if (connectivity_moni_mutex_handle != NULL) vSemaphoreDelete(connectivity_moni_mutex_handle);
}

uint8_t connectivity_moni_change(lwm2m_data_t * dataArray,
                                 lwm2m_object_t * objectP)
{
    int64_t value;
    uint8_t result;
    conn_m_data_t * data;

    data = (conn_m_data_t*) (objectP->userData);

    switch (dataArray->id)
    {
    case RES_M_RADIO_SIGNAL_STRENGTH:
        if (1 == lwm2m_data_decode_int(dataArray, &value))
        {
            connectivity_moni_signalStrength = value;
            result = COAP_204_CHANGED;
        }
        else
        {
            result = COAP_400_BAD_REQUEST;
        }
        break;

    case RES_O_LINK_QUALITY:
        if (1 == lwm2m_data_decode_int(dataArray, &value))
        {
            connectivity_moni_linkQuality = value;
            result = COAP_204_CHANGED;
        }
        else
        {
            result = COAP_400_BAD_REQUEST;
        }
        break;

    case RES_M_IP_ADDRESSES:
        if (sizeof(data->ipAddresses[0]) <= dataArray->value.asBuffer.length)
        {
            result = COAP_400_BAD_REQUEST;
        }
        else
        {
            memset(data->ipAddresses[0], 0, sizeof(data->ipAddresses[0]));
            memcpy(data->ipAddresses[0], dataArray->value.asBuffer.buffer, dataArray->value.asBuffer.length);
            data->ipAddresses[0][dataArray->value.asBuffer.length] = 0;
            result = COAP_204_CHANGED;
        }
        break;

    case RES_O_ROUTER_IP_ADDRESS:
        if (sizeof(data->routerIpAddresses[0]) <= dataArray->value.asBuffer.length)
        {
            result = COAP_400_BAD_REQUEST;
        }
        else
        {
            memset(data->routerIpAddresses[0], 0, sizeof(data->routerIpAddresses[0]));
            memcpy(data->routerIpAddresses[0], dataArray->value.asBuffer.buffer, dataArray->value.asBuffer.length);
            data->routerIpAddresses[0][dataArray->value.asBuffer.length] = 0;
            result = COAP_204_CHANGED;
        }
        break;

    case RES_O_CELL_ID:
        if (1 == lwm2m_data_decode_int(dataArray, &value))
        {
            connectivity_moni_cellId = value;
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

    return result;
}

static lwm2m_context_t *connectivity_moni_lwm2m_handle;
static TimerHandle_t connectivity_moni_timer;
static bool connectivity_moni_observed;
static bool connectivity_moni_present;
static uint32_t connectivity_moni_min_period = LWM2M_SERVER_DEFAULT_MIN_PERIOD;
static uint32_t connectivity_moni_max_period = LWM2M_SERVER_DEFAULT_MAX_PERIOD;

void connectivity_moni_set_handle(lwm2m_context_t *lwm2m_handle)
{
    connectivity_moni_lwm2m_handle = lwm2m_handle;
}

static void connectivity_moni_data_notify_callback(TimerHandle_t xTimer)
{
    char signal_quality[20];
    connectivity_moni_get_signal_quality();
    sprintf(signal_quality, "%d", connectivity_moni_linkQuality);

    char *obj_uri_string = "/4/0/2";
    lwm2m_uri_t uri;
    lwm2m_stringToUri(obj_uri_string, strlen(obj_uri_string), &uri);

    lwm2m_data_notify(connectivity_moni_lwm2m_handle, &uri, signal_quality, strlen(signal_quality));
}

void connectivity_moni_observe_notify(lwm2m_context_t *lwm2m_handle, bool observe, lwm2m_uri_t *uriP)
{
    connectivity_moni_observed = observe;
    if (connectivity_moni_observed) {
        if (connectivity_moni_timer == NULL) {
            connectivity_moni_timer = xTimerCreate("tmo_monitor", 500 * (connectivity_moni_min_period + connectivity_moni_max_period) / portTICK_PERIOD_MS, pdFALSE, NULL, connectivity_moni_data_notify_callback);
        }
        configASSERT(connectivity_moni_timer != NULL);
        xTimerStart(connectivity_moni_timer, 0);
    } else {
        xTimerStop(connectivity_moni_timer, 0);
    }
}

void connectivity_moni_set_parameter_notify(lwm2m_context_t *lwm2m_handle, lwm2m_uri_t *uriP, lwm2m_attributes_t *attrP)
{
    connectivity_moni_present = true;
    connectivity_moni_min_period = attrP->minPeriod;
    connectivity_moni_max_period = attrP->maxPeriod;
    if (connectivity_moni_observed) {
        xTimerStop(connectivity_moni_timer, 0);
        xTimerChangePeriod(connectivity_moni_timer, 500 * (connectivity_moni_min_period + connectivity_moni_max_period) / portTICK_PERIOD_MS, 0);
        xTimerStart(connectivity_moni_timer, 0);
    }
}

void connectivity_moni_min_period_change(lwm2m_context_t *lwm2m_handle, uint32_t min_period)
{
    if (!connectivity_moni_present) {
        connectivity_moni_min_period = min_period;
        if (connectivity_moni_observed) {
            xTimerStop(connectivity_moni_timer, 0);
            xTimerChangePeriod(connectivity_moni_timer, 500 * (connectivity_moni_min_period + connectivity_moni_max_period) / portTICK_PERIOD_MS, 0);
            xTimerStart(connectivity_moni_timer, 0);
        }
    }
}

void connectivity_moni_max_period_change(lwm2m_context_t *lwm2m_handle, uint32_t max_period)
{
    if (!connectivity_moni_present) {
        connectivity_moni_max_period = max_period;
        if (connectivity_moni_observed) {
            xTimerStop(connectivity_moni_timer, 0);
            xTimerChangePeriod(connectivity_moni_timer, 500 * (connectivity_moni_min_period + connectivity_moni_max_period) / portTICK_PERIOD_MS, 0);
            xTimerStart(connectivity_moni_timer, 0);
        }
    }
}

