/* Copyright Statement:
 *
 * (C) 2005-2017  MediaTek Inc. All rights reserved.
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
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
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

#include "fota_nw_manager.h"
#include "tel_conn_mgr_common.h"
#include "tel_conn_mgr_common_def.h"
#include "tel_conn_mgr_app_api_ext.h"
#include "tel_conn_mgr_bearer_iprot.h"
#include "ril.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

log_create_module(fota_nw, PRINT_LEVEL_INFO);

/******************************************************************************************
 *                               Local Macro Definition                                   *
 *****************************************************************************************/
#define FOTA_NW_ERR(fmt,arg...)   LOG_E(fota_nw, "[FOTA_NW]: "fmt,##arg)
#define FOTA_NW_WARN(fmt,arg...)  LOG_W(fota_nw, "[FOTA_NW]: "fmt,##arg)
#define FOTA_NW_DBG(fmt,arg...)   LOG_I(fota_nw,"[FOTA_NW]: "fmt,##arg)
#define AUTO_CONNECT_PDN_GROUP_NAME        ("tel_conn_mgr")
#define AUTO_CONNECT_PDN_PDN_DATA_ITEM_NAME    ("default_pdn_flag")
/******************************************************************************************
 *                               Type Definitions                                         *
 ******************************************************************************************/
typedef struct {
    fota_nw_status_t nw_status;
    bool cpin_ready;
    tel_conn_mgr_pdp_type_enum pdp_type;
    tel_conn_mgr_pdp_type_enum actived_pdp_type;
    char *apn;
    char *username;
    char *password;
    tel_conn_mgr_register_handle_t reg_handle;
    uint32_t app_id;
    bool need_auto_connect_nw;
}fota_nw_manager_context_t;
/******************************************************************************************
 *                               Local Variants                                           *
 ******************************************************************************************/
static fota_nw_manager_context_t g_fota_nw_context = {0};
/******************************************************************************************
 *                               Local Function's Definitions                             *
 ******************************************************************************************/
static void fota_tel_conn_mgr_cb(unsigned int app_id,
                                 tel_conn_mgr_info_type_enum info_type,
                                 tel_conn_mgr_bool result,
                                 tel_conn_mgr_err_cause_enum cause,
                                 tel_conn_mgr_pdp_type_enum pdp_type);
static int32_t fota_nw_manager_ril_urc_callback(ril_urc_id_t event_id, void *param, uint32_t param_len);
/******************************************************************************************
 *                               Public Function's Implementation                         *
 ******************************************************************************************/
fota_nw_status_t fota_nw_manager_nw_status(void)
{
    return g_fota_nw_context.nw_status;
}

bool fota_nw_auto_connect_network_enabled(void)
{
    uint8_t auto_create_pdn_falg = 0;
#ifdef MTK_NVDM_ENABLE
    uint32_t len = 1;
    nvdm_status_t status = NVDM_STATUS_OK;
    status = nvdm_read_data_item(AUTO_CONNECT_PDN_GROUP_NAME, AUTO_CONNECT_PDN_PDN_DATA_ITEM_NAME, (uint8_t *)&auto_create_pdn_falg, &len);
    if (status!= NVDM_STATUS_OK ) {
        return false;
    }
#endif
    FOTA_NW_DBG("auto_create_pdn_falg = %d\r\n", auto_create_pdn_falg);
    if (auto_create_pdn_falg == 0x01) {
        return true;
    }
    return false;
}

void fota_nw_manager_init(void)
{
    ril_status_t ril_status = RIL_STATUS_FAIL;
    memset(&g_fota_nw_context, 0, sizeof(g_fota_nw_context));
    g_fota_nw_context.nw_status = FOTA_NW_DEACTIVED;
    g_fota_nw_context.need_auto_connect_nw = fota_nw_auto_connect_network_enabled();
    FOTA_NW_DBG("fota_nw_manager_init\r\n");
    g_fota_nw_context.reg_handle = tel_conn_mgr_register_callback(fota_tel_conn_mgr_cb);
    if (g_fota_nw_context.reg_handle == NULL){
        /*Register failed.*/
        configASSERT(0);
    }

    if (g_fota_nw_context.need_auto_connect_nw == true){
        ril_status = ril_register_event_callback(RIL_GROUP_MASK_ALL, fota_nw_manager_ril_urc_callback);
        if (ril_status != RIL_STATUS_SUCCESS){
            /*Register failed.*/
            configASSERT(0);
        }
    }
}

void fota_nw_manager_active_network(void)
{
    tel_conn_mgr_pdp_type_enum pdp_type = TEL_CONN_MGR_PDP_TYPE_IPV4V6;
    tel_conn_mgr_pdp_type_enum activated_pdp_type = TEL_CONN_MGR_PDP_TYPE_NONE;
    tel_conn_mgr_ret_enum ret;
    unsigned int app_id = 0;

    g_fota_nw_context.nw_status = FOTA_NW_ACTIVING;
    FOTA_NW_DBG("fota_nw_manager_active_network\r\n");
    ret = tel_conn_mgr_activate_ext(TEL_CONN_MGR_BEARER_TYPE_NBIOT,
                                    TEL_CONN_MGR_SIM_ID_1,
                                    pdp_type,
                                    "",
                                    "",
                                    "",
                                    g_fota_nw_context.reg_handle,
                                    &app_id,
                                    &activated_pdp_type);
    if (TEL_CONN_MGR_RET_OK == ret) {
        FOTA_NW_DBG("FOTA Active Bear successfully\r\n");
        g_fota_nw_context.nw_status = FOTA_NW_ACTIVED;
    } else {//TEL_CONN_MGR_RET_WOULDBLOCK, need listen msg.
        FOTA_NW_DBG("FOTA waiting for bear ready, %d\r\n", ret);
    }
}

/******************************************************************************************
 *                               Local Function's Implementation                          *
 ******************************************************************************************/
static void fota_tel_conn_mgr_cb(unsigned int app_id,
                                 tel_conn_mgr_info_type_enum info_type,
                                 tel_conn_mgr_bool result,
                                 tel_conn_mgr_err_cause_enum cause,
                                 tel_conn_mgr_pdp_type_enum pdp_type)
{
    switch(info_type){
        case TEL_CONN_MGR_INFO_TYPE_ACTIVATION:{
            if (result == true){
                FOTA_NW_DBG("FOTA Active Bear successfully callback\r\n");
                g_fota_nw_context.nw_status = FOTA_NW_ACTIVED;
            }else{
                FOTA_NW_DBG("FOTA Active Bear failed:%d\r\n", cause);
                g_fota_nw_context.nw_status = FOTA_NW_DEACTIVED;
            }
            break;
        }
        case TEL_CONN_MGR_INFO_TYPE_ACTIVE_DEACTIVATION:{
            g_fota_nw_context.nw_status = FOTA_NW_DEACTIVED;
            break;
        }
        case TEL_CONN_MGR_INFO_TYPE_PASSIVE_DEACTIVATION:{
            g_fota_nw_context.nw_status = FOTA_NW_DEACTIVED;
            break;
        }
        default:{
            break;
        }
    }
}

static int32_t fota_nw_manager_ril_cmd_rsp_callback(ril_cmd_response_t *response)
{
    if (RIL_RESULT_CODE_OK == response->res_code) {
        ril_eps_network_registration_status_rsp_t *nw_reg_status = (ril_eps_network_registration_status_rsp_t *)(response->cmd_param);
        if (TEL_CONN_MGR_NW_REG_STAT_REGED_HMNW == nw_reg_status->stat ||
            TEL_CONN_MGR_NW_REG_STAT_REGED_ROAMING == nw_reg_status->stat ||
            TEL_CONN_MGR_NW_REG_STAT_SMS_ONLY_HMNW == nw_reg_status->stat ||
            TEL_CONN_MGR_NW_REG_STAT_SMS_ONLY_ROAMING == nw_reg_status->stat) {
            fota_nw_manager_active_network();
        }
    } else {
        /*Failed.*/
    }

    return 0;
}

static bool fota_nw_manager_is_nw_registered(ril_eps_network_registration_status_urc_t *nw_reg_status)
{
    if (TEL_CONN_MGR_NW_REG_STAT_REGED_HMNW == nw_reg_status->stat ||
        TEL_CONN_MGR_NW_REG_STAT_REGED_ROAMING == nw_reg_status->stat ||
        TEL_CONN_MGR_NW_REG_STAT_SMS_ONLY_HMNW == nw_reg_status->stat ||
        TEL_CONN_MGR_NW_REG_STAT_SMS_ONLY_ROAMING == nw_reg_status->stat)
    {
        return true;
    }

    return false;
}

static int32_t fota_nw_manager_ril_urc_callback(ril_urc_id_t event_id, void *param, uint32_t param_len)
{
    switch(event_id)
    {
       case RIL_URC_ID_CPIN: {
           ril_enter_pin_urc_t * pin = (ril_enter_pin_urc_t*)param;
           if ((strcmp(pin->code, "READY") == 0) && (g_fota_nw_context.cpin_ready == false)) {
               int32_t ret = 0;
               ret = ril_request_eps_network_registration_status(RIL_EXECUTE_MODE,
                                                                 2, NULL, NULL);
               g_fota_nw_context.cpin_ready = true;
           } else if (strcmp(pin->code, "NOT READY") == 0) {
               g_fota_nw_context.cpin_ready = false;
           }
           break;
       }

       case RIL_URC_ID_CEREG: {
            ril_eps_network_registration_status_urc_t *nw_reg_status = (ril_eps_network_registration_status_urc_t *)param;
            if (fota_nw_manager_is_nw_registered(nw_reg_status) == true) {
                if (g_fota_nw_context.nw_status == FOTA_NW_DEACTIVED){
                    fota_nw_manager_active_network();
                }
            }
            break;
       }

       case RIL_URC_ID_MATWAKEUP: {
           int32_t ret = 0;
           ret = ril_request_eps_network_registration_status(RIL_READ_MODE,
                                                             RIL_OMITTED_INTEGER_PARAM,
                                                             fota_nw_manager_ril_cmd_rsp_callback,
                                                             NULL);
           break;
       }
       default:
           break;

    }    
   return 0;
}

