/* Copyright Statement:
 *
 * (C) 2005-2017 MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
 * Without the prior written permission of MediaTek and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
 * if you have agreed to and been bound by the applicable license agreement with
 * MediaTek ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User"). If you are not a Permitted User,
 * please cease any access or use of MediaTek Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "nvdm.h"

#include "tel_conn_mgr_app_api.h"
#include "tel_conn_mgr_bearer_iprot.h"
#include "tel_conn_mgr_bearer_at_cmd_util.h"

#include "tel_conn_mgr_default_pdn_link.h"
#include "syslog.h"
#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
#include "hal_rtc_external.h"
#endif
#ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
#include "memory_attribute.h"
#endif

#define TEL_CONN_MGR_DEFAULT_PDN_MSG_QUEUE_MAX_SIZE (5)

static QueueHandle_t g_pdn_default_msg_queue_hdl = NULL;

#ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
ATTR_ZIDATA_IN_RETSRAM tel_conn_mgr_default_pdn_struct g_ds_default_pdn;
#else
tel_conn_mgr_default_pdn_struct g_default_pdn;
#endif

log_create_module(PDN_LINK, PRINT_LEVEL_INFO);
#ifndef MTK_MD_DEFERRED_WAKEUP_SUPPORT

#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP

static int32_t tel_conn_mgr_query_pdn_callback(ril_cmd_response_t *cmd_response);
#endif
#endif
tel_conn_mgr_default_pdn_struct tel_conn_mgr_get_default_pdn_content(void)
{
#ifdef MTK_MD_DEFERRED_WAKEUP_SUPPORT
    return g_ds_default_pdn;
#else
    return g_default_pdn;
#endif

}

int32_t tel_conn_mgr_is_create_default_pdn(void)
{
 
    uint8_t g_auto_create_pdn_falg = 0;
    
#ifdef MTK_NVDM_ENABLE
    uint32_t len = 1;

    nvdm_status_t status = NVDM_STATUS_OK;

    status = nvdm_read_data_item(TEL_CONN_MGR_NVDM_PDN_GROUP_NAME, TEL_CONN_MGR_NVDM_PDN_DATA_ITEM_NAME, (uint8_t *)&g_auto_create_pdn_falg, &len);
    
     LOG_I(PDN_LINK,"[default pdn]status:%d,falg:%d\r\n", status, g_auto_create_pdn_falg);
    if (status!= NVDM_STATUS_OK ) {
         
        g_auto_create_pdn_falg = 0x01;
        len = 1;
         nvdm_write_data_item(TEL_CONN_MGR_NVDM_PDN_GROUP_NAME,
                           TEL_CONN_MGR_NVDM_PDN_DATA_ITEM_NAME,
                           NVDM_DATA_ITEM_TYPE_STRING,
                           (uint8_t *)&g_auto_create_pdn_falg,
                                    len); 
        
         return 1;
        }
#endif
  if (g_auto_create_pdn_falg == 0x01 || g_auto_create_pdn_falg == 0x31) {
      return 1;
    }
    return 0;
}

int32_t tel_conn_mgr_default_pdn_urc_callback(ril_urc_id_t event_id, void *param, uint32_t param_len)
{
    
    tel_conn_default_pdn_message_t *pdn_msg = NULL;
    LOG_I(PDN_LINK,"[default pdn]event_id:%d", event_id);
    if (!param || !param_len)
    {
        return -1;
    }
    
    LOG_I(PDN_LINK,"[default pdn]event_id:%d, param:%x, param_len:%d", event_id, (unsigned int)param, (int)param_len);
    switch (event_id)
    {
        case RIL_URC_ID_MDPDNP:
        {
            ril_default_pdn_parameter_urc_t *pdn_param = (ril_default_pdn_parameter_urc_t *)param;
            
             tel_conn_default_pdn_message_t *msg = NULL;

            LOG_I(PDN_LINK,"[default pdn]pdn_type:%d, apn:%s", pdn_param->pdn_type, pdn_param->apn);
            
            
            #ifndef MTK_MD_DEFERRED_WAKEUP_SUPPORT
            //g_default_pdn.apn = pdn_param->apn;
            memcpy(g_default_pdn.apn,pdn_param->apn, sizeof(g_default_pdn.apn));
            if (strcmp(pdn_param->pdn_type, "IP") == 0){
                g_default_pdn.pdp_type = TEL_CONN_MGR_PDP_TYPE_IP;

            }else if (strcmp(pdn_param->pdn_type, "IPV6") == 0){
                g_default_pdn.pdp_type = TEL_CONN_MGR_PDP_TYPE_IPV6;

            }else if (strcmp(pdn_param->pdn_type, "IPV4V6") == 0){
                g_default_pdn.pdp_type = TEL_CONN_MGR_PDP_TYPE_IPV4V6;

            }else if (strcmp(pdn_param->pdn_type, "Non-IP") == 0){
                g_default_pdn.pdp_type = TEL_CONN_MGR_PDP_TYPE_NIDD;

            }
            g_default_pdn.flag = 1;
            LOG_I(PDN_LINK,"[default pdn]type:%d, apn:%s, flag:%d\n", g_default_pdn.pdp_type, g_default_pdn.apn, g_default_pdn.flag);
           
#else
           memcpy(g_ds_default_pdn.apn,pdn_param->apn, sizeof(g_ds_default_pdn.apn));
           if (strcmp(pdn_param->pdn_type, "IP") == 0){
               g_ds_default_pdn.pdp_type = TEL_CONN_MGR_PDP_TYPE_IP;

           }else if (strcmp(pdn_param->pdn_type, "IPV6") == 0){
               g_ds_default_pdn.pdp_type = TEL_CONN_MGR_PDP_TYPE_IPV6;

           }else if (strcmp(pdn_param->pdn_type, "IPV4V6") == 0){
               g_ds_default_pdn.pdp_type = TEL_CONN_MGR_PDP_TYPE_IPV4V6;

           }else if (strcmp(pdn_param->pdn_type, "Non-IP") == 0){
               g_ds_default_pdn.pdp_type = TEL_CONN_MGR_PDP_TYPE_NIDD;

           }
           g_ds_default_pdn.flag = 1;
           LOG_I(PDN_LINK,"[default pdn]type:%d, apn:%s, flag:%d\n", g_ds_default_pdn.pdp_type, g_ds_default_pdn.apn, g_ds_default_pdn.flag);
          

#endif
            msg = pvPortMalloc(sizeof(tel_conn_default_pdn_message_t));

            msg->message_id = TEL_CONN_MGR_DEFAULT_PDN_ACTIVE;
            msg->param = NULL;
            if (pdTRUE != xQueueSend(g_pdn_default_msg_queue_hdl, &msg, 0)) {
                
                LOG_I(PDN_LINK,"[default pdn] send msg fail!\n");
                configASSERT(0); 
            }

            break;   
        }
        
        default:
            break;
    }
    return 0;
    
}
int32_t tel_conn_mgr_default_apn_ignore()
{ 
    unsigned int idx = 0;
    tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep = NULL;
    tel_conn_mgr_bearer_cntx_struct *bearer_cntx = tel_conn_mgr_bearer_get_cntx();
    if (bearer_cntx) {
        for (idx = 0; idx < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; idx++)
        {
            if (bearer_cntx->bearer_info[idx].is_used)
            {
                ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_idx(idx);
                LOG_I(PDN_LINK,"[default pdn]tel_conn_mgr_default_apn_ignore:%s, %d!!\r\n", ds_keep->apn, idx);
                if (strcmp(ds_keep->apn,"") == 0){
                    LOG_I(PDN_LINK,"[default pdn]ignore!!\r\n");
                    
                    return 1;
                }
            }
        }
    }
    return 0;
}
void tel_conn_default_pdn_msg_handler(tel_conn_default_pdn_message_t *msg)
{
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    
    unsigned int app_id = 0;
    
    tel_conn_mgr_pdp_type_enum acted_pdp_type = TEL_CONN_MGR_PDP_TYPE_NONE;
    
    LOG_I(PDN_LINK,"[default pdn]msg_handler:%d!\r\n", msg->message_id);
    switch(msg->message_id) {
        case MSG_ID_TEL_CONN_MGR_ACTIVATION_RSP://513
        {
            
            LOG_I(PDN_LINK,"[default pdn]default pdn active done!\r\n");
        }
            break;

        case MSG_ID_TEL_CONN_MGR_DEACTIVATION_RSP://514
        {
            
            LOG_I(PDN_LINK,"[default pdn]default pdn deactive done!\r\n");

        }
            break;

        case MSG_ID_TEL_CONN_MGR_DEACTIVATION_IND://515
        {
            
            LOG_I(PDN_LINK,"[default pdn]default pdn passive deactive done!\r\n");

        }
             break;        
        case TEL_CONN_MGR_DEFAULT_PDN_ACTIVE://515
        {     
           
            if (tel_conn_mgr_is_create_default_pdn() == 1) {
                
                if (tel_conn_mgr_default_apn_ignore() == 0) {// if apn is "" have exsit.
                    ret = tel_conn_mgr_activate(TEL_CONN_MGR_BEARER_TYPE_NBIOT,
                                               TEL_CONN_MGR_SIM_ID_1,
                                               
#ifndef MTK_MD_DEFERRED_WAKEUP_SUPPORT
                                               g_default_pdn.pdp_type,
                                               g_default_pdn.apn,
#else
                                               g_ds_default_pdn.pdp_type,
                                               g_ds_default_pdn.apn,
#endif
                                               "",
                                               "",
                                               g_pdn_default_msg_queue_hdl,
                                               &app_id,
                                               &acted_pdp_type);
                               
                    LOG_I(PDN_LINK,"[default pdn]ret:%d\r\n", ret);
                }

            } else{
                 LOG_I(PDN_LINK,"[default pdn]nvdm null\r\n");

            }

        }
        break;  
        default:
            break;
            
    }
    
    vPortFree(msg);
    msg=NULL;
}


void tel_conn_mgr_default_pdn_task(void *arg)
{
    tel_conn_default_pdn_message_t *queue_item = NULL;
    while (1) {
        if (xQueueReceive(g_pdn_default_msg_queue_hdl, &queue_item, portMAX_DELAY)) {
            tel_conn_default_pdn_msg_handler(queue_item);
        }
    }
} 
void tel_conn_mgr_default_pdn_link_init(void)
{
    ril_status_t ret = RIL_STATUS_SUCCESS;

    LOG_I(PDN_LINK,"[default pdn]default_pdn_link_init!\r\n");
    

#ifndef MTK_MD_DEFERRED_WAKEUP_SUPPORT
    memset(&g_default_pdn,0,sizeof(tel_conn_mgr_default_pdn_struct));
#else
    rtc_power_on_result_t bootup = 0;
    bootup = rtc_power_on_result_external();
    LOG_I(PDN_LINK,"[default pdn]deep_sleep_bootup:d%\r\n",bootup);
    

    if (bootup != DEEP_SLEEP && bootup != DEEPER_SLEEP) {

        memset(&g_ds_default_pdn,0,sizeof(tel_conn_mgr_default_pdn_struct));
    }
#endif
    /* Create a message queue to store the pointer of the message. */
    g_pdn_default_msg_queue_hdl = xQueueCreate(TEL_CONN_MGR_DEFAULT_PDN_MSG_QUEUE_MAX_SIZE,
                                                   sizeof(tel_conn_default_pdn_message_t *));

    if (g_pdn_default_msg_queue_hdl)
    {
        xTaskCreate(tel_conn_mgr_default_pdn_task,
                    TEL_CONN_MGR_DEFAULT_PDN_TASK_NAME,
                    TEL_CONN_MGR_DEFAULT_PDN_TASK_STACKSIZE/sizeof(portSTACK_TYPE),
                    NULL,
                    TEL_CONN_MGR_DEFAULT_PDN_TASK_PRIO,
                    NULL);
    }
    ret = ril_register_event_callback(RIL_GROUP_MASK_ALL, tel_conn_mgr_default_pdn_urc_callback);
    if (ret != RIL_STATUS_SUCCESS) {
        LOG_I(PDN_LINK,"[default pdn]ril register error!\r\n");

    }
    #ifndef MTK_MD_DEFERRED_WAKEUP_SUPPORT
        {
    #ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
    rtc_power_on_result_t deep_sleep_bootup = 0;
    deep_sleep_bootup = rtc_power_on_result_external();
    
    LOG_I(PDN_LINK,"[default pdn]deep_sleep_bootup:d%\r\n",deep_sleep_bootup);
    if (deep_sleep_bootup == DEEP_SLEEP || deep_sleep_bootup == DEEPER_SLEEP) {
        
        ret = ril_request_define_pdp_context(RIL_READ_MODE, 
                                               RIL_OMITTED_INTEGER_PARAM,
                                               tel_conn_mgr_query_pdn_callback,
                                               RIL_OMITTED_INTEGER_PARAM,
                                               RIL_OMITTED_INTEGER_PARAM);
         if (ret != RIL_STATUS_SUCCESS) {
        LOG_I(PDN_LINK,"[default pdn]ril CGDCONT? error!\r\n");

    }
    }
    #endif
        }
    #endif
}

uint32_t tel_conn_mgr_default_get_apn_flag(void)
{

#ifndef MTK_MD_DEFERRED_WAKEUP_SUPPORT
   LOG_I(PDN_LINK,"[default pdn]get_apn_flag:%d\n", g_default_pdn.flag);


   return g_default_pdn.flag;
#else
   LOG_I(PDN_LINK,"[default pdn]get_apn_flag:%d\n", g_ds_default_pdn.flag);


   return g_ds_default_pdn.flag;

#endif
}
#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
#ifndef MTK_MD_DEFERRED_WAKEUP_SUPPORT

static int32_t tel_conn_mgr_query_pdn_callback(ril_cmd_response_t *cmd_response)
{
    switch (cmd_response->cmd_id) {

    case RIL_CMD_ID_CGDCONT: {            
                   ril_define_pdp_context_rsp_t *response = (ril_define_pdp_context_rsp_t *)cmd_response->cmd_param;
                   if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {                
                       int32_t idx;
                       LOG_I(PDN_LINK,"[default pdn]array_num: %d\r\n", (int) response->array_num);
                       for (idx = 0; idx < response->array_num; idx++) {
                           
                           LOG_I(PDN_LINK,"[default pdn]cid: %d\r\n", (int) response->pdp_context[idx].cid);
                           LOG_I(PDN_LINK,"[default pdn]pdp_type: %s\r\n", response->pdp_context[idx].pdp_type);
                           LOG_I(PDN_LINK,"[default pdn]apn: %s\r\n", response->pdp_context[idx].apn);
                           LOG_I(PDN_LINK,"[default pdn]ipv4addralloc: %d\r\n", (int) response->pdp_context[idx].ipv4addralloc);
                           if (response->pdp_context[idx].cid == 1) {
     memcpy(g_default_pdn.apn,response->pdp_context[idx].apn, sizeof(response->pdp_context[idx].apn));
            if (strcmp(response->pdp_context[idx].pdp_type, "IP") == 0){
                g_default_pdn.pdp_type = TEL_CONN_MGR_PDP_TYPE_IP;

            }else if (strcmp(response->pdp_context[idx].pdp_type, "IPV6") == 0){
                g_default_pdn.pdp_type = TEL_CONN_MGR_PDP_TYPE_IPV6;

            }else if (strcmp(response->pdp_context[idx].pdp_type, "IPV4V6") == 0){
                g_default_pdn.pdp_type = TEL_CONN_MGR_PDP_TYPE_IPV4V6;

            }else if (strcmp(response->pdp_context[idx].pdp_type, "Non-IP") == 0){
                g_default_pdn.pdp_type = TEL_CONN_MGR_PDP_TYPE_NIDD;

            }
            g_default_pdn.flag = 1;
            LOG_I(PDN_LINK,"[default pdn]qery:type:%d, apn:%s, flag:%d\n", g_default_pdn.pdp_type, g_default_pdn.apn, g_default_pdn.flag);
           break;
                           }
                       }
                   }
                   break;

}
    default:
        break;
        }
    return 0;
}
#endif
#endif

