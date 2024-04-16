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
#if defined(MTK_LWM2M_SUPPORT)

#include "liblwm2m.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "ril.h"
#include "syslog.h"
#include "timers.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_platform.h"

log_create_module(lwm2m_obj, PRINT_LEVEL_INFO);

static void object_cellular_timer_read(void);
static void object_cellular_edrx_param_read();
static void object_cellular_edrx_param_write(char *edrx_param);
static void object_cellular_timer_write_psm(char *req_psm_time);
static void object_cellular_timer_write_act(char *req_act_time);
static int32_t object_cellular_ril_cmd_response_callback(ril_cmd_response_t *cmd_response);
static void object_cellular_edrx_enable_urc(void);
static int32_t object_cellular_urc_callback(ril_urc_id_t event_id, void *param, uint32_t param_len);
static int64_t object_cellular_parse_timer(char *req_time, int is_act);

static int32_t binary_to_de(char *str, int count);
static char* object_cellular_convert_to_md_act(int64_t req_time);
static char* object_cellular_convert_to_md_psm(int64_t req_time);
static void binary_to_HEX(char *str, int count);
#if defined(__RIL_SMS_COMMAND_SUPPORT__)
static void object_cellular_smsc_addr_read(void);
static void object_cellular_smsc_addr_write(char *smsc_addr);

#endif
static void object_cellular_radio_timeout_callback(TimerHandle_t xTimer);


#define APN_PRIFILE_NAME      "AAAAA"
#define SMSC_ADDESS      "+8612345678901"
#define ACT_CODE      "abcde"


// Resource Id's:
#define RES_O_SMSC_ADDESS                  0//RW
#define RES_O_DISABE_RADIO_PERIOD          1//RW
#define RES_O_MODULE_ACT_CODE              2//RW
#define RES_O_VENDOR_SPECIFIC_EXTENSIONS   3      //R
#define RES_O_PSM_TIMER                    4//RW
#define RES_O_ACTIVE_TIMER                 5//RW
#define RES_O_SERVING_PLMN_RATE_CTR        6      //R
#define RES_O_EDRX_PARAM_LU                7//RW
#define RES_O_EDRX_PARAM_WBS1              8//RW
#define RES_O_EDRX_PARAM_NBS1              9//RW
#define RES_O_EDRX_PARAM_AGB               10//RW
#define RES_M_ACT_RROFILE_NANES            11//RW



typedef struct
{
    int active_timer; //2s~31min
    int psm_timer; //10min~992d
    char *edrx_param;
    
    char edrx_parameter[1];
    char edrx_value[5];
    char edrx_ptw[5];
    
    char smsc_addr[164];
    uint16_t radio_period;
    
    uint8_t sleep_handle;
} cellular_instance_t;
static cellular_instance_t *g_cellular_instance;
static SemaphoreHandle_t cellular_conn_mutex_handle;;
static uint8_t g_cellular_ds_lock = 0xFF;

TimerHandle_t radio_timer;

static uint8_t prv_get_value(lwm2m_data_t * dataP)
                            // cellular_instance_t * devDataP)
{
    // a simple switch structure is used to respond at the specified resource asked
    
      LOG_I(lwm2m_obj,"[cellular] prv_get_value:%d!!", dataP->id);
    switch (dataP->id)
    {
        case RES_M_ACT_RROFILE_NANES:
            lwm2m_data_encode_string(APN_PRIFILE_NAME, dataP);
            return COAP_205_CONTENT;

        case RES_O_ACTIVE_TIMER:
            
            object_cellular_timer_read();
            
            LOG_I(lwm2m_obj,"[cellular] active_timer:%d!!", g_cellular_instance->active_timer);
            lwm2m_data_encode_int(g_cellular_instance->active_timer, dataP);
            
            return COAP_205_CONTENT;
        case RES_O_PSM_TIMER:
            
            LOG_I(lwm2m_obj,"[cellular] @@@@psm_timer:%d!!", g_cellular_instance->psm_timer);
            
            object_cellular_timer_read();
            lwm2m_data_encode_int(g_cellular_instance->psm_timer, dataP);   
            
            LOG_I(lwm2m_obj,"[cellular] psm_timer:%d!!", g_cellular_instance->psm_timer);
            return COAP_205_CONTENT;

        case RES_O_EDRX_PARAM_NBS1:
            
            object_cellular_edrx_param_read();
           binary_to_HEX(g_cellular_instance->edrx_param, 8);

           LOG_I(lwm2m_obj,"[cellular]HEX edrx_parameter:%02x!!", g_cellular_instance->edrx_parameter[0]);
           lwm2m_data_encode_opaque((uint8_t*)&g_cellular_instance->edrx_parameter[0], 1, dataP);

            return COAP_205_CONTENT;

        case RES_O_EDRX_PARAM_AGB:
                {

            char *str = '0x35';
            lwm2m_data_encode_opaque((uint8_t*)str, 1, dataP);
            }
            return COAP_205_CONTENT;
        case RES_O_EDRX_PARAM_WBS1:
                {
                    char *str = '0x35';
            lwm2m_data_encode_opaque((uint8_t*)str, 1, dataP);
            }
            return COAP_205_CONTENT;
        case RES_O_EDRX_PARAM_LU:
                {
                    char *str = '0x35';
            lwm2m_data_encode_opaque((uint8_t*)str, 1, dataP);
            }
            return COAP_205_CONTENT;
        case RES_O_SERVING_PLMN_RATE_CTR:
            
            lwm2m_data_encode_int(100, dataP);
            return COAP_205_CONTENT;
       // case RES_O_VENDOR_SPECIFIC_EXTENSIONS:
            
          //  return COAP_205_CONTENT;
        case RES_O_MODULE_ACT_CODE:
            
            lwm2m_data_encode_string(ACT_CODE, dataP);
            return COAP_205_CONTENT;
        case RES_O_DISABE_RADIO_PERIOD:
            
            lwm2m_data_encode_int(g_cellular_instance->radio_period, dataP);
            return COAP_205_CONTENT;
        case RES_O_SMSC_ADDESS:
            
#if defined(__RIL_SMS_COMMAND_SUPPORT__)
           object_cellular_smsc_addr_read();
           LOG_I(lwm2m_obj,"[cellular]smsc_addr:%s!!", g_cellular_instance->smsc_addr);

           lwm2m_data_encode_string(g_cellular_instance->smsc_addr, dataP);

#else
           lwm2m_data_encode_string(SMSC_ADDESS, dataP);
#endif
            return COAP_205_CONTENT;


        default:
            return COAP_404_NOT_FOUND;
    }
}

static uint8_t prv_cellular_read(uint16_t instanceId,
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
            RES_O_SMSC_ADDESS,   
            RES_O_DISABE_RADIO_PERIOD,
            RES_O_MODULE_ACT_CODE,
         //   RES_O_VENDOR_SPECIFIC_EXTENSIONS,
            RES_O_PSM_TIMER,
            RES_O_ACTIVE_TIMER,
            RES_O_SERVING_PLMN_RATE_CTR,
            RES_O_EDRX_PARAM_LU,
            RES_O_EDRX_PARAM_WBS1,
            RES_O_EDRX_PARAM_NBS1,
            RES_O_EDRX_PARAM_AGB,
            RES_M_ACT_RROFILE_NANES,
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
        result = prv_get_value((*dataArrayP) + i);
        i++;
    } while (i < *numDataP && result == COAP_205_CONTENT);

    return result;
}



static uint8_t prv_cellular_write(void * contextP, 
                                 uint16_t instanceId,
                                int numData,
                                lwm2m_data_t * dataArray,
                                lwm2m_object_t * objectP)
{
    int i;
    uint8_t result;
    char* act_timer;
    
    int64_t param;
    i = 0;
    
    do
    {
    
        LOG_I(lwm2m_obj,"[cellular] prv_cellular_write i,id:%d,%d,%d!!", i, dataArray[i].id, numData);
        switch (dataArray[i].id)
        {
        case RES_O_ACTIVE_TIMER:
            if (1 == lwm2m_data_decode_int(dataArray + i, &g_cellular_instance->active_timer))
            {
            
                LOG_I(lwm2m_obj,"[cellular] active_timer:%d!!", g_cellular_instance->active_timer);
                act_timer = object_cellular_convert_to_md_act(g_cellular_instance->active_timer);

                
                LOG_I(lwm2m_obj,"[cellular]to md--> act_timer:%s!!", act_timer);
                object_cellular_timer_write_act(act_timer);
                result = COAP_204_CHANGED;
            }
            else
            {
                result = COAP_400_BAD_REQUEST;
            }
            break;

        case RES_O_PSM_TIMER:
            
            if (1 == lwm2m_data_decode_int(dataArray + i, &g_cellular_instance->psm_timer))            
            {
               
               LOG_I(lwm2m_obj,"[cellular] psm_timer:%d!!", g_cellular_instance->psm_timer);
               act_timer = object_cellular_convert_to_md_psm(g_cellular_instance->psm_timer);

                
                LOG_I(lwm2m_obj,"[cellular]to md--> act_timer:%s!!", act_timer);
                
                object_cellular_timer_write_psm(act_timer);
                result = COAP_204_CHANGED;
            }
            else
            {
                result = COAP_400_BAD_REQUEST;
            }
            break;

        case RES_O_EDRX_PARAM_NBS1:
                {

         LOG_I(lwm2m_obj,"[cellular] @@@@@type:%d,%d!!", dataArray[i].type, dataArray[i].value.asBuffer.length);
        
         /*   LOG_I(lwm2m_obj,"[cellular] write:%02x!!", dataArray[i].value.asBuffer.buffer[0]);
            
            LOG_I(lwm2m_obj,"[cellular] write:%02x!!", dataArray[i].value.asBuffer.buffer[1]);
            LOG_I(lwm2m_obj,"[cellular] write:%02x!!", dataArray[i].value.asBuffer.buffer[2]);

        LOG_I(lwm2m_obj,"[cellular] write:%02x!!", dataArray[i].value.asBuffer.buffer[3]);*/
        _get_data_to_hex(g_cellular_instance->edrx_param, dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);

            object_cellular_edrx_param_write(g_cellular_instance->edrx_param);
            
            result = COAP_204_CHANGED;
            }
            break;
            
#if defined(__RIL_SMS_COMMAND_SUPPORT__)
        case RES_O_SMSC_ADDESS:
       
        {
            
            LOG_I(lwm2m_obj,"[cellular] @@@@@type:%d,%d!!", dataArray[i].type, dataArray[i].value.asBuffer.length);
            memcpy(g_cellular_instance->smsc_addr, dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
            LOG_I(lwm2m_obj,"[cellular] smsc_addr:%d!!", g_cellular_instance->smsc_addr);
          

           
            object_cellular_smsc_addr_write(g_cellular_instance->smsc_addr);
            result = COAP_204_CHANGED;
        }
        
        break;
#endif
        case RES_O_DISABE_RADIO_PERIOD:
            
        if (1 == lwm2m_data_decode_int(dataArray + i, &g_cellular_instance->radio_period))            
        {
           
            LOG_I(lwm2m_obj,"[cellular] radio_period:%d!!", g_cellular_instance->radio_period);
            //disconnect, start timer & reconnect when timer elapsed.
            
            ril_request_ps_attach_or_detach(RIL_EXECUTE_MODE, 0, object_cellular_ril_cmd_response_callback, (void *)0);
            // lock sleep.
            
            LOG_I(lwm2m_obj,"[cellular] lock sleep!!!");
            hal_sleep_manager_acquire_sleeplock(g_cellular_instance->sleep_handle, HAL_SLEEP_LOCK_DEEP);
            if (radio_timer == NULL) { //FIRST
            
                LOG_I(lwm2m_obj,"[cellular] FIRST radio_timer!!");
                radio_timer = xTimerCreate("radio_timer", 
                                            g_cellular_instance->radio_period * 60 * 1000/ portTICK_PERIOD_MS, 
                                            pdFALSE, 
                                            NULL, 
                                            object_cellular_radio_timeout_callback);
                configASSERT(radio_timer != NULL);  
                if (xTimerStart(radio_timer, 0) != pdPASS) {
                    LOG_I(lwm2m_obj,"[cellular] radio_timer fail!!");
                }

             } else {
             
                  LOG_I(lwm2m_obj,"[cellular] change radio_timer!!");
                  xTimerStop(radio_timer, 0);
                  xTimerChangePeriod(radio_timer, g_cellular_instance->radio_period * 60 * 1000/ portTICK_PERIOD_MS, 0);
                  xTimerReset(radio_timer, 0);
             }  
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

static void object_cellular_radio_timeout_callback(TimerHandle_t xTimer)
{    
    
    LOG_I(lwm2m_obj,"[cellular] radio_timeout_callback!!!");
    ril_request_ps_attach_or_detach(RIL_EXECUTE_MODE, 1, object_cellular_ril_cmd_response_callback, (void *)1);
    //unlock sleep;
    
    LOG_I(lwm2m_obj,"[cellular] unlock sleep!!!");
    hal_sleep_manager_release_sleeplock(g_cellular_instance->sleep_handle, HAL_SLEEP_LOCK_DEEP);
}

void display_cellular_connectivity_object(lwm2m_object_t * object)
{
#ifdef WITH_LOGS
    /*device_data_t * data = (device_data_t *)object->userData;
    fprintf(stdout, "  /%u: Device object:\r\n", object->objID);
    if (NULL != data)
    {
        fprintf(stdout, "    time: %lld, time_offset: %s\r\n",
                (long long) data->time, data->time_offset);
    }*/
#endif
}

static uint8_t prv_cellular_discover(uint16_t instanceId,
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
               RES_O_SMSC_ADDESS,   
            RES_O_DISABE_RADIO_PERIOD,
            RES_O_MODULE_ACT_CODE,
           // RES_O_VENDOR_SPECIFIC_EXTENSIONS,
            RES_O_PSM_TIMER,
            RES_O_ACTIVE_TIMER,
            RES_O_SERVING_PLMN_RATE_CTR,
            RES_O_EDRX_PARAM_LU,
            RES_O_EDRX_PARAM_WBS1,
            RES_O_EDRX_PARAM_NBS1,
            RES_O_EDRX_PARAM_AGB,
            RES_M_ACT_RROFILE_NANES,
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
                case RES_O_SMSC_ADDESS:
                case RES_O_DISABE_RADIO_PERIOD:
                case RES_O_MODULE_ACT_CODE:
                //case RES_O_VENDOR_SPECIFIC_EXTENSIONS:
                case RES_O_PSM_TIMER:
                case RES_O_ACTIVE_TIMER:
                case RES_O_SERVING_PLMN_RATE_CTR:
                case RES_O_EDRX_PARAM_LU:
                case RES_O_EDRX_PARAM_WBS1:
                case RES_O_EDRX_PARAM_NBS1:
                case RES_O_EDRX_PARAM_AGB:
                case RES_M_ACT_RROFILE_NANES:
                break;
            default:
                result = COAP_404_NOT_FOUND;
            }
        }
    }

    return result;
}

lwm2m_object_t * get_object_cellular_connectivity()
{
    /*
     * The lwm2m_get_object_cellular_connectivity function create the object itself and return a pointer to the structure that represent it.
     */
    lwm2m_object_t * cellconnObj;

    cellconnObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != cellconnObj)
    {
    
        object_cellular_init();
        memset(cellconnObj, 0, sizeof(lwm2m_object_t));

        /*
         * It assigns his unique ID
         * The 3 is the standard ID for the mandatory object "Object cellular connectivity".
         */
        cellconnObj->objID = LWM2M_CELLULAR_CONN_OBJECT_ID;

        /*
         * and its unique instance
         *
         */
        cellconnObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
        if (NULL != cellconnObj->instanceList)
        {
            memset(cellconnObj->instanceList, 0, sizeof(lwm2m_list_t));
        }
        else
        {
            lwm2m_free(cellconnObj);
            return NULL;
        }

        /*
         * And the private function that will access the object.
         * Those function will be called when a read/write/execute query is made by the server. In fact the library don't need to
         * know the resources of the object, only the server does.
         */
        cellconnObj->readFunc     = prv_cellular_read;
        cellconnObj->discoverFunc = prv_cellular_discover;
        cellconnObj->writeFunc    = prv_cellular_write;
       // cellconnObj->executeFunc  = prv_device_execute;
       // cellconnObj->userData = lwm2m_malloc(sizeof(device_data_t));

        /*
         * Also some user data can be stored in the object with a private structure containing the needed variables
         */
      /*  if (NULL != cellconnObj->userData)
        {
            ((device_data_t*)cellconnObj->userData)->battery_level = PRV_BATTERY_LEVEL;
            ((device_data_t*)cellconnObj->userData)->free_memory   = PRV_MEMORY_FREE;
            ((device_data_t*)cellconnObj->userData)->error = PRV_ERROR_CODE;
            ((device_data_t*)cellconnObj->userData)->time  = 1367491215;
            strcpy(((device_data_t*)cellconnObj->userData)->time_offset, "+01:00");
        }
        else
        {
            lwm2m_free(cellconnObj->instanceList);
            lwm2m_free(cellconnObj);
            cellconnObj = NULL;
        }*/
    }

    return cellconnObj;
}

void free_object_cellular_connectivity(lwm2m_object_t * objectP)
{
    /*if (NULL != objectP->userData)
    {
        lwm2m_free(objectP->userData);
        objectP->userData = NULL;
    }*/
    
    if (NULL != objectP->instanceList)
    {
        lwm2m_free(objectP->instanceList);
        objectP->instanceList = NULL;
    }

    lwm2m_free(objectP);

    if (cellular_conn_mutex_handle != NULL) vSemaphoreDelete(cellular_conn_mutex_handle);
}

static int32_t object_cellular_urc_callback(ril_urc_id_t event_id, void *param, uint32_t param_len)
{
    
      LOG_I(lwm2m_obj,"[cellular] event_id:%d", event_id);
    if (!param || !param_len)
    {
        return -1;
    }
    
     LOG_I(lwm2m_obj,"[cellular] event_id:%d, param:%x, param_len:%d", event_id, (unsigned int)param, (int)param_len);
    switch (event_id)
    {
        case RIL_URC_ID_CEDRXP:
        {
            
            ril_eDRX_setting_urc_t *edrx_urc = (ril_eDRX_setting_urc_t*)param;
            if (edrx_urc->act_type == 5) {
                
                memcpy(g_cellular_instance->edrx_param, edrx_urc->requested_eDRX_value, strlen(edrx_urc->requested_eDRX_value));
                memcpy(g_cellular_instance->edrx_ptw, edrx_urc->paging_time_window, strlen(edrx_urc->paging_time_window));
                strcat(g_cellular_instance->edrx_param, g_cellular_instance->edrx_ptw);
            }
            break;

        }
        
        default:
            break;
    }
    return 0;
    
}

void object_cellular_init(void)
{
    uint8_t result;
    ril_status_t ret = RIL_STATUS_SUCCESS;
    LOG_I(lwm2m_obj,"[cellular] lwm2m_object_cellular_init!!");

    g_cellular_instance = (cellular_instance_t *)lwm2m_malloc(sizeof(cellular_instance_t));
    radio_timer = NULL;
    if (NULL == g_cellular_instance) {
        
          LOG_I(lwm2m_obj,"[cellular] lwm2m_object_cellular_init fail");
        return;
    }
    memset(g_cellular_instance, 0, sizeof(cellular_instance_t));

    
    if (g_cellular_ds_lock == 0xFF) {
        g_cellular_instance->sleep_handle = hal_sleep_manager_set_sleep_handle("object_cellular");
        configASSERT(g_cellular_instance->sleep_handle != SLEEP_LOCK_INVALID_ID);
        g_cellular_ds_lock = g_cellular_instance->sleep_handle;

    }
    
    ret = ril_register_event_callback(RIL_GROUP_MASK_ALL, object_cellular_urc_callback);
    if (ret != RIL_STATUS_SUCCESS) {
          LOG_I(lwm2m_obj,"[cellular] register ril urc cb fail!!");
    }
    
    cellular_conn_mutex_handle = xSemaphoreCreateMutex();
    configASSERT(cellular_conn_mutex_handle != NULL);
   // object_cellular_timer_read();
    
   // object_cellular_edrx_param_read();

}
static int32_t object_cellular_ril_cmd_response_callback(ril_cmd_response_t *cmd_response)
{
    LOG_I(lwm2m_obj,"[cellular] ril_cmd_response_callback:%d!!", cmd_response->cmd_id);

    switch (cmd_response->cmd_id)
    {
        case RIL_CMD_ID_CPSMS:
        {
              LOG_I(lwm2m_obj,"[cellular] RIL_CMD_ID_CPSMS:%d:%d\n",cmd_response->mode,cmd_response->res_code);
            if(RIL_READ_MODE == cmd_response->mode && RIL_RESULT_CODE_OK == cmd_response->res_code){
                ril_power_saving_mode_setting_rsp_t *param = (ril_power_saving_mode_setting_rsp_t*)cmd_response->cmd_param;
               // g_cellular_instance->active_timer = param->req_act_time;
                //g_cellular_instance->psm_timer = param->req_prdc_tau;
                if (param->req_act_time) {
                    g_cellular_instance->active_timer = object_cellular_parse_timer(param->req_act_time, 1);
                }
                if (param->req_prdc_tau) {
                    g_cellular_instance->psm_timer = object_cellular_parse_timer(param->req_prdc_tau, 0);
                }
                LOG_I(lwm2m_obj,"[cellular] ril :act_timer:%s,psm_timer:%s\n",param->req_act_time, param->req_prdc_tau);
                LOG_I(lwm2m_obj,"[cellular] act_timer:%d,psm_timer:%d\n",g_cellular_instance->active_timer, g_cellular_instance->psm_timer);

               
            }else if(RIL_EXECUTE_MODE == cmd_response->mode && RIL_RESULT_CODE_OK == cmd_response->res_code){
 
                  LOG_I(lwm2m_obj,"[cellular] write done!!\n");
            }
            
            xSemaphoreGive(cellular_conn_mutex_handle);
            break;
        }
        case RIL_CMD_ID_MEDRXCFG:
        {
            LOG_I(lwm2m_obj,"[cellular] RIL_CMD_ID_MEDRXCFG:%d:%d\n",cmd_response->mode,cmd_response->res_code);
            if(RIL_READ_MODE == cmd_response->mode && RIL_RESULT_CODE_OK == cmd_response->res_code){
                ril_eDRX_configuration_rsp_t *param = (ril_eDRX_configuration_rsp_t*)cmd_response->cmd_param;

                if (param->requested_eDRX_value != NULL && param->requested_paging_time_window_value != NULL) {

                    if (g_cellular_instance->edrx_param) {
                        vPortFree(g_cellular_instance->edrx_param );
                        g_cellular_instance->edrx_param = NULL;

                        LOG_I(lwm2m_obj,"[cellular] vPortFree,edrx_param\n");
                    }
                    g_cellular_instance->edrx_param = (char *)pvPortMalloc(strlen(param->requested_paging_time_window_value)+strlen(param->requested_eDRX_value)+1);
                    strcpy(g_cellular_instance->edrx_param ,param->requested_paging_time_window_value);

                    LOG_I(lwm2m_obj,"[cellular] 111edrx_param:%s!!\n", g_cellular_instance->edrx_param );
                    strcat(g_cellular_instance->edrx_param ,param->requested_eDRX_value);

                    LOG_I(lwm2m_obj,"[cellular] edrx_param:%s!!\n", g_cellular_instance->edrx_param );
                }    
            }else if(RIL_EXECUTE_MODE == cmd_response->mode && RIL_RESULT_CODE_OK == cmd_response->res_code){
 
                  LOG_I(lwm2m_obj,"[cellular] write done!!\n");
            }
            
            xSemaphoreGive(cellular_conn_mutex_handle);
            break;
        }
#if defined(__RIL_SMS_COMMAND_SUPPORT__)
        case RIL_CMD_ID_CSCA:
        {
              LOG_I(lwm2m_obj,"[cellular] RIL_CMD_ID_CSCA:%d:%d\n",cmd_response->mode,cmd_response->res_code);
            if(RIL_READ_MODE == cmd_response->mode && RIL_RESULT_CODE_OK == cmd_response->res_code){
               
                ril_service_centre_address_rsp_t *param = (ril_service_centre_address_rsp_t*)cmd_response->cmd_param;
                //g_cellular_instance->smsc_addr = param->sca;
                memcpy(g_cellular_instance->smsc_addr, param->sca, strlen(param->sca));
               
            }else if(RIL_EXECUTE_MODE == cmd_response->mode && RIL_RESULT_CODE_OK == cmd_response->res_code){
 
                 
                 LOG_I(lwm2m_obj,"[cellular] write done!!\n");
            }
            
            xSemaphoreGive(cellular_conn_mutex_handle);
            break;
        }
#endif
        case RIL_CMD_ID_CGATT: 
        {
            if (RIL_RESULT_CODE_OK == cmd_response->res_code) {

                 if ((int32_t)cmd_response->user_data == 1) {
                     LOG_I(lwm2m_obj,"[cellular] attach done!!\n");

                 } else {
                     LOG_I(lwm2m_obj,"[cellular] detach done!!\n");

                 }
            } else{
                LOG_I(lwm2m_obj,"[cellular] detach/attach fail!!\n");

            }
                   
            break;
        }

        default:
            break;
    }
    return 0;
}

static void object_cellular_timer_write_act(char *req_act_time)
{
    ril_status_t ret;
    ril_power_saving_mode_setting_rsp_t param_set;
    
    xSemaphoreTake(cellular_conn_mutex_handle, portMAX_DELAY);
    memset(&param_set, 0, sizeof(ril_power_saving_mode_setting_rsp_t));
    param_set.mode = 1;
    param_set.req_act_time = req_act_time;  
    
    param_set.req_gprs_rdy_tmr = RIL_OMITTED_STRING_PARAM;
    param_set.req_prdc_rau = RIL_OMITTED_STRING_PARAM;
    param_set.req_prdc_tau = RIL_OMITTED_STRING_PARAM;
    //g_cellular_instance.active_timer = req_act_time;
    ret = ril_request_power_saving_mode_setting(RIL_EXECUTE_MODE, &param_set, object_cellular_ril_cmd_response_callback, NULL);
    if (ret != RIL_STATUS_SUCCESS) {
        
        xSemaphoreGive(cellular_conn_mutex_handle);
          LOG_I(lwm2m_obj,"[cellular] +CPSMS write fail!!");
    
    }
    /* wait for RIL callback */
    xSemaphoreTake(cellular_conn_mutex_handle, portMAX_DELAY);
    xSemaphoreGive(cellular_conn_mutex_handle);

}

static void object_cellular_timer_write_psm(char *req_psm_time)
{
    
    ril_status_t ret;
    ril_power_saving_mode_setting_rsp_t param_set;
    
    xSemaphoreTake(cellular_conn_mutex_handle, portMAX_DELAY);
    memset(&param_set, 0, sizeof(ril_power_saving_mode_setting_rsp_t));
    param_set.mode = 1;
    param_set.req_prdc_tau = req_psm_time;
    param_set.req_gprs_rdy_tmr = RIL_OMITTED_STRING_PARAM;
    param_set.req_prdc_rau = RIL_OMITTED_STRING_PARAM;
    param_set.req_act_time = RIL_OMITTED_STRING_PARAM;
    
    //g_cellular_instance.psm_timer = req_psm_time;
    ret = ril_request_power_saving_mode_setting(RIL_EXECUTE_MODE, &param_set, object_cellular_ril_cmd_response_callback, NULL);
    if (ret != RIL_STATUS_SUCCESS) {
        
        xSemaphoreGive(cellular_conn_mutex_handle);
          LOG_I(lwm2m_obj,"[cellular] +CPSMS write fail!!");
    
    }
    /* wait for RIL callback */
    xSemaphoreTake(cellular_conn_mutex_handle, portMAX_DELAY);
    xSemaphoreGive(cellular_conn_mutex_handle);

}

static void object_cellular_timer_read(void)
{
    ril_status_t ret;
    
    LOG_I(lwm2m_obj,"[cellular] lwm2m_object_cellular_timer_read!!");
    
    xSemaphoreTake(cellular_conn_mutex_handle, portMAX_DELAY);
    ret = ril_request_power_saving_mode_setting(RIL_READ_MODE,
            NULL,
            object_cellular_ril_cmd_response_callback,
            NULL);
    if (ret != RIL_STATUS_SUCCESS) {
        
        xSemaphoreGive(cellular_conn_mutex_handle);
          LOG_I(lwm2m_obj,"[cellular] +CPSMS read fail!!");
    
    }
    /* wait for RIL callback */
    xSemaphoreTake(cellular_conn_mutex_handle, portMAX_DELAY);
    xSemaphoreGive(cellular_conn_mutex_handle);

}

#if defined(__RIL_SMS_COMMAND_SUPPORT__)
static void object_cellular_smsc_addr_read(void)
{
    ril_status_t ret;
    
    LOG_I(lwm2m_obj,"[cellular] object_cellular_smsc_addr_read!!");
    
    xSemaphoreTake(cellular_conn_mutex_handle, portMAX_DELAY);
    ret = ril_request_service_centre_address(RIL_READ_MODE,        
            RIL_OMITTED_STRING_PARAM,
            RIL_OMITTED_INTEGER_PARAM,
            object_cellular_ril_cmd_response_callback,
            NULL);
    if (ret != RIL_STATUS_SUCCESS) {
        
        xSemaphoreGive(cellular_conn_mutex_handle);
          LOG_I(lwm2m_obj,"[cellular] +CPSMS read fail!!");
    
    }
    /* wait for RIL callback */
    xSemaphoreTake(cellular_conn_mutex_handle, portMAX_DELAY);
    xSemaphoreGive(cellular_conn_mutex_handle);

}


static void object_cellular_smsc_addr_write(char *smsc_addr)
{
    ril_status_t ret;
    
    xSemaphoreTake(cellular_conn_mutex_handle, portMAX_DELAY);
    ret = ril_request_service_centre_address(RIL_EXECUTE_MODE, 
                                        smsc_addr,
                                        RIL_OMITTED_INTEGER_PARAM, 
                                        object_cellular_ril_cmd_response_callback, 
                                        NULL);
    if (ret != RIL_STATUS_SUCCESS) {
        
        xSemaphoreGive(cellular_conn_mutex_handle);
          LOG_I(lwm2m_obj,"[cellular] +CSCA write fail!!");
    
    }
    /* wait for RIL callback */
    xSemaphoreTake(cellular_conn_mutex_handle, portMAX_DELAY);
    xSemaphoreGive(cellular_conn_mutex_handle);

}

#endif

static void object_cellular_edrx_param_write(char *edrx_param)
{
       ril_status_t ret;
       
       xSemaphoreTake(cellular_conn_mutex_handle, portMAX_DELAY);
       memcpy(g_cellular_instance->edrx_ptw, edrx_param, 4);
       memcpy(g_cellular_instance->edrx_value, edrx_param+4, 4);

       
       LOG_I(lwm2m_obj,"[cellular] write:%s,%s!!", g_cellular_instance->edrx_value, g_cellular_instance->edrx_ptw);
       ret = ril_request_eDRX_configuration(RIL_EXECUTE_MODE,
                   2,
                   5,
                   (char *)g_cellular_instance->edrx_value,
                   (char *)g_cellular_instance->edrx_ptw,
                   object_cellular_ril_cmd_response_callback,
                   NULL);

       if (ret != RIL_STATUS_SUCCESS) {
        
        xSemaphoreGive(cellular_conn_mutex_handle);
             LOG_I(lwm2m_obj,"[cellular] *MEDRXCFG write fail!!");
       
       }
       /* wait for RIL callback */
       xSemaphoreTake(cellular_conn_mutex_handle, portMAX_DELAY);
       xSemaphoreGive(cellular_conn_mutex_handle);


}

static void object_cellular_edrx_param_read()
{
    ril_status_t ret;
    
    xSemaphoreTake(cellular_conn_mutex_handle, portMAX_DELAY);
    ret = ril_request_eDRX_configuration(RIL_READ_MODE,
        RIL_OMITTED_INTEGER_PARAM,
        RIL_OMITTED_INTEGER_PARAM,
        RIL_OMITTED_STRING_PARAM,
        RIL_OMITTED_STRING_PARAM,
        object_cellular_ril_cmd_response_callback,
        NULL);
    if (ret != RIL_STATUS_SUCCESS) {
        
        xSemaphoreGive(cellular_conn_mutex_handle);
          LOG_I(lwm2m_obj,"[cellular] *MEDRXCFG read fail!!");
    
    }
    /* wait for RIL callback */
    xSemaphoreTake(cellular_conn_mutex_handle, portMAX_DELAY);
    xSemaphoreGive(cellular_conn_mutex_handle);


}
static int32_t object_cellular_edrx_enable_callback(ril_cmd_response_t *cmd_response)
{

    switch (cmd_response->cmd_id)
    {

        case RIL_CMD_ID_MEDRXCFG:
        {
              LOG_I(lwm2m_obj,"[cellular] RIL_CMD_ID_MEDRXCFG:%d:%d\n",cmd_response->mode,cmd_response->res_code);
  
            if(RIL_EXECUTE_MODE == cmd_response->mode && RIL_RESULT_CODE_OK == cmd_response->res_code){
 
                  LOG_I(lwm2m_obj,"[cellular] enable done!!\n");
            }
            
            break;
        }
        default:
            break;
    }
    return 0;
}

static void object_cellular_edrx_enable_urc(void)
{
    ril_status_t ret;
    ret = ril_request_eDRX_configuration(RIL_EXECUTE_MODE,
        2,
        RIL_OMITTED_INTEGER_PARAM,
        RIL_OMITTED_STRING_PARAM,
        RIL_OMITTED_STRING_PARAM,
        object_cellular_edrx_enable_callback,
        NULL);
    if (ret != RIL_STATUS_SUCCESS) {
          LOG_I(lwm2m_obj,"[cellular] *MEDRXCFG enable urc fail!!");
    
    }


}
static char* object_cellular_convert_to_md_act(int64_t req_time)
{
    int32_t pre_num = 0;// G 3bit num.
    int32_t beh_num = 0;// D 5bit num.
    char str1[3] = {0};
    
    char str2[5] = {0};
    char str3[9] = {0};

    
    if (req_time%360 == 0){
        //pre_num = 2;
        strcpy(str1, "010");
        beh_num = req_time/360;
    }else if (req_time%60 == 0){
       // pre_num = 1;
       
       strcpy(str1, "001");
        beh_num = req_time/60;

    }else if (req_time%2 == 0) {
        //pre_num = 0;      
        
        strcpy(str1, "000");
        beh_num = req_time/2;

    }
    LOG_I(lwm2m_obj,"[cellular] convert_to_md_act:%s,%d!!", str1, beh_num);
    //itoa(pre_num,str1,2);
    strcpy(str3, str1);
    
    LOG_I(lwm2m_obj,"[cellular] convert_to_md_act, str3:%s!!", str3);
    
    _itoa(beh_num,str2,2);
    LOG_I(lwm2m_obj,"[cellular] convert_to_md_act:%s!!",  str2);
    int num_bit = strlen(str2);
    
    LOG_I(lwm2m_obj,"[cellular] convert_to_md_act:%d!!",  num_bit);
  
       if (num_bit == 4) {
        strcat(str3, "0");
        } else if(num_bit == 3) {
           strcat(str3, "00");

        } else if (num_bit == 2) {
            strcat(str3, "000");

        } else if (num_bit == 1) {
        
          strcat(str3, "0000");
        }
        strcat(str3, str2);

    str3[9] = '\0';
    LOG_I(lwm2m_obj,"[cellular] convert_to_md_act, str3:%s!!", str3);
    return str3;
}

static char* object_cellular_convert_to_md_psm(int64_t req_time)
{
    int32_t pre_num = 0;// G 3bit num.
    int32_t beh_num = 0;// D 5bit num.
    char str1[3] = {0};
    
    char str2[5] = {0};
    char str3[9] = {0};

    
    if (req_time%1152000 == 0){
        //pre_num = 2;
        strcpy(str1, "110");
        beh_num = req_time/1152000;
    }else if (req_time%36000 == 0){
       // pre_num = 1;
       
       strcpy(str1, "010");
        beh_num = req_time/36000;

    }else if (req_time%3600 == 0) {
        //pre_num = 0;      
        
        strcpy(str1, "001");
        beh_num = req_time/3600;

    }else if (req_time%360 == 0){
       // pre_num = 1;
       
       strcpy(str1, "000");
        beh_num = req_time/360;

    }else if (req_time%60 == 0) {
        //pre_num = 0;      
        
        strcpy(str1, "101");
        beh_num = req_time/60;

    }else if (req_time%30 == 0){
       // pre_num = 1;
       
       strcpy(str1, "100");
        beh_num = req_time/30;

    }else if (req_time%2 == 0) {
        //pre_num = 0;      
        
        strcpy(str1, "011");
        beh_num = req_time/2;

    }
    LOG_I(lwm2m_obj,"[cellular] convert_to_md_act:%s,%d!!", str1, beh_num);
    //itoa(pre_num,str1,2);
    strcpy(str3, str1);
    
    LOG_I(lwm2m_obj,"[cellular] convert_to_md_act, str3:%s!!", str3);
    
    _itoa(beh_num,str2,2);
    LOG_I(lwm2m_obj,"[cellular] convert_to_md_act:%s!!",  str2);
    int num_bit = strlen(str2);
    
    LOG_I(lwm2m_obj,"[cellular] convert_to_md_act:%d!!",  num_bit);
  
       if (num_bit == 4) {
        strcat(str3, "0");
        } else if(num_bit == 3) {
           strcat(str3, "00");

        } else if (num_bit == 2) {
            strcat(str3, "000");

        } else if (num_bit == 1) {
        
          strcat(str3, "0000");
        }
        strcat(str3, str2);

    str3[9] = '\0';
    LOG_I(lwm2m_obj,"[cellular] convert_to_md_act, str3:%s!!", str3);
    return str3;
}

static int64_t object_cellular_parse_timer(char *req_time, int is_act)
{
    int32_t timer_value;
    
    int64_t timer_value_s = 0;
     int32_t temp;
    int32_t i = 0;
    char str1[3] = {0};
    
    char str2[5] = {0};
    
    LOG_I(lwm2m_obj,"[cellular] lwm2m_object_cellular_parse_timer:%s,%d!!", req_time, is_act);
   // memcpy(str1, req_time, 3);
   strncpy(str1, req_time, 3);
    
    LOG_I(lwm2m_obj,"[cellular] str1:%s!!", str1);
   // memcpy(str2, req_time+3, 5);
    strncpy(str2, req_time+3, 5);
   
    LOG_I(lwm2m_obj,"[cellular] str2:%s!!", str2);
    timer_value = binary_to_de(str2,5);
    
    LOG_I(lwm2m_obj,"[cellular] timer_value:%d!!", timer_value);
    i = binary_to_de(str1,3);
    
    LOG_I(lwm2m_obj,"[cellular] case:%d!!", i);
    if (is_act == 1) {
        switch (i)
            {
               case 0:
                
                  timer_value_s = timer_value * 2;
                
                  LOG_I(lwm2m_obj,"[cellular] ######:%d!!", timer_value_s);
                  
                  LOG_I(lwm2m_obj,"[cellular] ######:%ld!!", timer_value_s);
                  temp = timer_value * 2;
                  
                  LOG_I(lwm2m_obj,"[cellular] ######temp:%d!!", temp);
                  break;
        
               case 1:
                   
                   timer_value_s = timer_value * 60;
                  break;
                   
               case 2:
                   
                   timer_value_s = timer_value * 6 * 60;
                   break;

               default:
                   
                   break;
                   
            }

    } else {
    switch (i)
    {
       case 0:
        
          timer_value_s = timer_value * 10 * 60;
        
          LOG_I(lwm2m_obj,"[cellular] ######:%d!!", timer_value_s);
          
          LOG_I(lwm2m_obj,"[cellular] ######:%ld!!", timer_value_s);
          temp = timer_value * 10 * 60;
          
          LOG_I(lwm2m_obj,"[cellular] ######temp:%d!!", temp);
          break;

       case 1:
           
           timer_value_s = timer_value * 60 * 60;
          break;
           
       case 2:
           
           timer_value_s = timer_value * 600 * 60;
           break;
           

       case 3:
           
           timer_value_s = timer_value * 2;
           break;
        case 4:
           
           timer_value_s = timer_value * 30;
           break;   
       case 5:
           timer_value_s = timer_value * 60;
                  
          break;
      case 6:
          timer_value_s = timer_value * 320 * 60 *60;
                                 
         break;

       default:
           
           break;
           
    }
        }
   LOG_I(lwm2m_obj,"[cellular] timer_value_s:%d!!", timer_value_s);
   return timer_value_s;

}

static int32_t binary_to_de(char *str, int count)
{
   int32_t num = 0;
   int32_t i;
   
   LOG_I(lwm2m_obj,"[cellular]binary_to_de: str:%s!!", str);
   //for(; str[i]; (num*=2)+=str[i++]-'0');
   for(i = 0; i<count; i++)
    {
    num = num*2+str[i]-'0';
    
    LOG_I(lwm2m_obj,"[cellular] for: num:%d!, str[i]:%c,i:%d!", num, str[i], i);
    }
   LOG_I(lwm2m_obj,"[cellular] num:%d!!", num);

   return num;


}
static void binary_to_HEX(char *str, int count)
{

   int base=0,i;
    
   for(i=0;i<count;i++)
   {
       if((str[i]!='0') && (str[i]!='1'))
       {
           break;
       }
       int b=str[i]-'0';
       base = (base<<1) + b;
        
   }
   g_cellular_instance->edrx_parameter[0] = base;
  
   //sprintf(str,"%X",base);
/*   str[0] = '0';
   str[1] = 'x';  
   str[2] = '0'+(base>>4);
   str[3] = '0'+((base&0xf0)>>4);
   str[4] = '\0';*/

  
 // LOG_I(lwm2m_obj,"[cellular]HEX str:%s!!", str);
 
  
   LOG_I(lwm2m_obj,"[cellular]HEX str1:0x%x!!", g_cellular_instance->edrx_parameter[0]);

}

#endif
