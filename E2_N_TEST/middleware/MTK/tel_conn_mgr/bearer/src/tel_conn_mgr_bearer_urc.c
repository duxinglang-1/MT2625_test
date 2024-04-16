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

#include "stdlib.h"
#include "ril_cmds_def.h"
#include "ril.h"
#include "tel_conn_mgr_platform.h"
#include "tel_conn_mgr_bearer_util.h"
#include "tel_conn_mgr_bearer_at_cmd_util.h"
#include "tel_conn_mgr_bearer_list_mgr.h"
#include "tel_conn_mgr_bearer_timer.h"
#include "tel_conn_mgr_bearer_cache.h"
#include "tel_conn_mgr_bearer_urc.h"

tel_conn_mgr_ret_enum tel_conn_mgr_bearer_act_urc_process(int cid, int reason)
{
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);
    tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_cid(cid);
    tel_conn_mgr_bool pdp_type_supported = TEL_CONN_MGR_TRUE;
    
    if (!at_cmd_flow_helper || !ds_keep || !bearer_state)
    {
        TEL_CONN_MGR_LOG_INFO("pdp not found. cid:%d", cid);
        return TEL_CONN_MGR_RET_UNKNOWN;
    }
    
    if (tel_conn_mgr_is_bearer_activating(*bearer_state) &&
        TEL_CONN_MGR_BEARER_STATE_ACTIVATING_WAIT_ACT_URC & (*bearer_state))
    {
        (*bearer_state) &= ~TEL_CONN_MGR_BEARER_STATE_ACTIVATING_WAIT_ACT_URC;
        (*bearer_state) |= TEL_CONN_MGR_BEARER_STATE_ACTIVATING_ACTED;
        ds_keep->activated_pdp_type = ds_keep->pdp_type;
        if (0 == reason)
        {
            TEL_CONN_MGR_LOG_INFO("IPV4 only allowed");
            ds_keep->activated_pdp_type = TEL_CONN_MGR_PDP_TYPE_IP;
            if (TEL_CONN_MGR_PDP_TYPE_IPV6 == ds_keep->pdp_type ||
                TEL_CONN_MGR_PDP_TYPE_NIDD == ds_keep->pdp_type)
            {
                pdp_type_supported = TEL_CONN_MGR_FALSE;
            }
        }
        else if (1 == reason)
        {
            TEL_CONN_MGR_LOG_INFO("IPV6 only allowed");
            ds_keep->activated_pdp_type = TEL_CONN_MGR_PDP_TYPE_IPV6;
            if (TEL_CONN_MGR_PDP_TYPE_IP == ds_keep->pdp_type ||
                TEL_CONN_MGR_PDP_TYPE_NIDD == ds_keep->pdp_type)
            {
                pdp_type_supported = TEL_CONN_MGR_FALSE;
            }
        }        
        else if (RIL_OMITTED_INTEGER_PARAM != reason)
        {
            TEL_CONN_MGR_LOG_INFO("reason:%d", reason);
            ds_keep->activated_pdp_type = TEL_CONN_MGR_PDP_TYPE_NONE;
            pdp_type_supported =  TEL_CONN_MGR_FALSE;
        }

        if (!pdp_type_supported)
        {
            (*bearer_state) = TEL_CONN_MGR_BEARER_STATE_NEED_DEACT;
            return tel_conn_mgr_bearer_terminate_activating_state(cid,
                                                                  TEL_CONN_MGR_FALSE,
                                                                  TEL_CONN_MGR_ERR_CAUSE_PDP_TYPE_NOT_SUPPORTED,
                                                                  TEL_CONN_MGR_TRUE);
        }

        if (TEL_CONN_MGR_AT_CMD_TYPE_NONE == at_cmd_flow_helper->current_at_cmd_type)
        {
            /* Get IP directly */
            TEL_CONN_MGR_LOG_INFO("Receive act urc after the at cmd rsp.");
            if (TEL_CONN_MGR_PDP_TYPE_NIDD == ds_keep->pdp_type)
            {
                return tel_conn_mgr_bearer_terminate_activating_state(cid,
                                                                      TEL_CONN_MGR_TRUE,
                                                                      TEL_CONN_MGR_ERR_CAUSE_NONE,
                                                                      TEL_CONN_MGR_FALSE);
            }
            
            ret = tel_conn_mgr_get_bearer_relevant_info(cid);
            if (TEL_CONN_MGR_RET_OK == ret)
            {
                return ret;
            }

            ret = tel_conn_mgr_bearer_terminate_activating_state(cid,
                                                                 TEL_CONN_MGR_FALSE,
                                                                 TEL_CONN_MGR_ERR_CAUSE_AT_CMD_EXCEPTION,
                                                                 TEL_CONN_MGR_TRUE);
        }
        else
        {
            /* AT CMD RSP process function will terminate activating. */
            TEL_CONN_MGR_LOG_INFO("Receive act urc before the at cmd rsp.");
        }
        
    }    
    else
    {
        /* ACTIVATING may be canceled by DEACTIVATE. */
        TEL_CONN_MGR_LOG_INFO("Drop ME ACT URC for it is unexpected.\r\n");
    }

    return ret;
}


tel_conn_mgr_ret_enum tel_conn_mgr_bearer_me_deact_urc_process(int cid)
{
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);

    if (!at_cmd_flow_helper || !bearer_state)
    {
        TEL_CONN_MGR_LOG_INFO("%s, pdp not found. cid:%d", __FUNCTION__, cid);
        return TEL_CONN_MGR_RET_NOT_FOUND;
    }
    
    TEL_CONN_MGR_LOG_INFO("cid:%d, bearer_state:%x", cid, (*bearer_state));

    if (tel_conn_mgr_is_bearer_deactivating(*bearer_state) &&
        TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_WAIT_DEACT_URC & (*bearer_state))
    {
        (*bearer_state) &= ~TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_WAIT_DEACT_URC;
        (*bearer_state) |= TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_DEACTED;
        if (TEL_CONN_MGR_AT_CMD_TYPE_NONE == at_cmd_flow_helper->current_at_cmd_type)
        {
            /* Deactivation is done successfully. */
            TEL_CONN_MGR_LOG_INFO("Receive deact urc after the at cmd rsp.");
            tel_conn_mgr_bearer_terminate_deactivating_state(cid, TEL_CONN_MGR_TRUE, TEL_CONN_MGR_ERR_CAUSE_NONE);
        }
        else
        {
            /* AT CMD RSP process function will terminate deactivating. */
            TEL_CONN_MGR_LOG_INFO("Receive deact urc before the at cmd rsp.");
        }
    }
    else
    {
        TEL_CONN_MGR_LOG_WARN("unexpected ME DEACT URC.\r\n");
        tel_conn_mgr_bearer_turn_into_inactive_state_by_cid(cid,
                                                            TEL_CONN_MGR_ERR_CAUSE_NW_DEACT,
                                                            TEL_CONN_MGR_FALSE);
    }

    return TEL_CONN_MGR_RET_OK;
}


tel_conn_mgr_ret_enum tel_conn_mgr_cereg_urc_process(tel_conn_mgr_nw_reg_status_struct *nw_reg_status)
{
    tel_conn_mgr_nw_reg_status_struct *nw_reg_sta = tel_conn_mgr_bearer_get_nw_reg_status();
    
    if (!nw_reg_status || !nw_reg_sta)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    TEL_CONN_MGR_LOG_INFO("act:%d, stat:%d", nw_reg_status->act, nw_reg_status->stat);

    nw_reg_sta->act = nw_reg_status->act;
    nw_reg_sta->stat = nw_reg_status->stat;

#if defined(TEL_CONN_MGR_SUPPORT_DEEP_SLEEP) && !defined(TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM)
    tel_conn_mgr_nvdm_write(TEL_CONN_MGR_NVDM_COMMON_GROUP,
                         TEL_CONN_MGR_NVDM_COMMON_NW_REG_STATUS,
                         NVDM_DATA_ITEM_TYPE_RAW_DATA,
                         (const uint8_t *)nw_reg_sta,
                         sizeof(tel_conn_mgr_nw_reg_status_struct));
#endif

    /* Registered == attached */
    if (tel_conn_mgr_is_nw_registered())
    {

        /* INIT is done. Excute the cached ACTIVATEs.
                 * Move nodes in class_to_finish list into class_to_excute list */
        tel_conn_mgr_cache_wake_up_class(TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE,
                                         TEL_CONN_MGR_BEARER_WAIT_EVENT_NW_REG_RESULT);
    }
    else if (tel_conn_mgr_is_nw_registration_failed())
    {
        /* Clear class_to_excute list */
        tel_conn_mgr_cache_remove_class(TEL_CONN_MGR_AT_CMD_CLASS_NONE,
                                        TEL_CONN_MGR_BEARER_WAIT_EVENT_ALL,
                                        TEL_CONN_MGR_ERR_CAUSE_NW_REGISTRATION_FAILED);
    }

    return TEL_CONN_MGR_RET_OK;
}


void tel_conn_mgr_cgev_urc_process(tel_conn_mgr_cgev_urc_struct *cgev_urc)
{
    if (!cgev_urc)
    {
        return;
    }

    switch (cgev_urc->cgev_urc_type)
    {
        case TEL_CONN_MGR_BEARER_CGEV_URC_TYPE_NW_DETACH:
        {
            /* Do not update tel_conn_mgr_bearer_get_nw_reg_status() here, for it should be updated by CGREG URC. */
            tel_conn_mgr_bearer_turn_into_inactive_state_all(TEL_CONN_MGR_ERR_CAUSE_NW_DETACH, TEL_CONN_MGR_FALSE);
            break;
        }
        
        case TEL_CONN_MGR_BEARER_CGEV_URC_TYPE_ME_DETACH:
        {
            /* Do not update tel_conn_mgr_bearer_get_nw_reg_status() here, for it should be updated by CGREG URC. */
            tel_conn_mgr_bearer_turn_into_inactive_state_all(TEL_CONN_MGR_ERR_CAUSE_ME_DETACH, TEL_CONN_MGR_FALSE);
            break;
        }
        
        case TEL_CONN_MGR_BEARER_CGEV_URC_TYPE_NW_PDN_DEACT:
        {
            tel_conn_mgr_bearer_turn_into_inactive_state_by_cid(cgev_urc->cid,
                                                                TEL_CONN_MGR_ERR_CAUSE_NW_DEACT,
                                                                TEL_CONN_MGR_FALSE);
            break;
        }
        
        case TEL_CONN_MGR_BEARER_CGEV_URC_TYPE_ME_PDN_DEACT:
        {
            tel_conn_mgr_bearer_me_deact_urc_process(cgev_urc->cid);
            break;
        }

        case TEL_CONN_MGR_BEARER_CGEV_URC_TYPE_ME_PDN_ACT:
        {                
#ifdef TEL_CONN_MGR_TEMP_IT
            TEL_CONN_MGR_LOG_INFO("[TCM]cid:%d reason:%d", cgev_urc->cid, cgev_urc->reason);
            //cgev_urc->cid = 1;
            //cgev_urc->reason = RIL_OMITTED_INTEGER_PARAM;
#endif
            tel_conn_mgr_bearer_act_urc_process(cgev_urc->cid, cgev_urc->reason);
            break;
        }
        
        default:
            break;
    }
}


/* payload is supposed to end with '\0' */
tel_conn_mgr_ret_enum tel_conn_mgr_urc_process(tel_conn_mgr_bearer_urc_ind_struct *urc_ind)
{
    tel_conn_mgr_bearer_cntx_struct *bearer_cntx = tel_conn_mgr_bearer_get_cntx();

    TEL_CONN_MGR_LOG_INFO("urc_type:%d", urc_ind ? urc_ind->urc_type : -1);
    
    if (!bearer_cntx)
    {
        TEL_CONN_MGR_LOG_INFO("Wrong state.");
        return TEL_CONN_MGR_RET_WRONG_STATE;
    }

    //TEL_CONN_MGR_LOG_INFO("bearer_state: %d", bearer_cntx->bearer_state);

    if (!urc_ind || !urc_ind->urc_detail)
    {
        TEL_CONN_MGR_LOG_INFO("Invalid param.");
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    //TEL_CONN_MGR_LOG_INFO("urc_type: %d", urc_ind->urc_type);

    switch (urc_ind->urc_type)
    {
        case TEL_CONN_MGR_BEARER_URC_TYPE_CGEV:
        {
            tel_conn_mgr_cgev_urc_process((tel_conn_mgr_cgev_urc_struct *)urc_ind->urc_detail);
            break;
        }

        case TEL_CONN_MGR_BEARER_URC_TYPE_CGREG:
        {
            tel_conn_mgr_cereg_urc_process((tel_conn_mgr_nw_reg_status_struct *)urc_ind->urc_detail);
            break;
        }

        default:
            break;
    }

    return TEL_CONN_MGR_RET_OK;
}


tel_conn_mgr_bearer_urc_ind_struct *tel_conn_mgr_bearer_gen_urc_ind(tel_conn_mgr_bearer_urc_type_enum urc_type,
                                                                              void *urc_detail)
{
    tel_conn_mgr_bearer_urc_ind_struct *urc_ind = NULL;

    urc_ind = tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_bearer_urc_ind_struct));
    if (!urc_ind)
    {
        return NULL;
    }

    urc_ind->msg_id = MSG_ID_TEL_CONN_MGR_BEARER_URC_IND;
    urc_ind->urc_type = urc_type;
    urc_ind->urc_detail = urc_detail;

    return urc_ind;
}


void tel_conn_mgr_bearer_free_urc_ind(tel_conn_mgr_bearer_urc_ind_struct *urc_ind)
{
    if (!urc_ind)
    {
        return;
    }

    if (urc_ind->urc_detail)
    {
        tel_conn_mgr_free(urc_ind->urc_detail);
        urc_ind->urc_detail = NULL;
    }

    tel_conn_mgr_free(urc_ind);
}
int32_t tel_conn_mgr_bearer_urc_callback(ril_urc_id_t event_id, void *param, uint32_t param_len)
{
    tel_conn_mgr_bearer_urc_ind_struct *urc_ind = NULL;

    TEL_CONN_MGR_LOG_INFO("event_id:%d, param:%x, param_len:%d", event_id, (unsigned int)param, (int)param_len);
    
    if (!param || !param_len)
    {
        return -1;
    }
    
    switch (event_id)
    {
        // TODO: update the urc structure for RIL_CMD_ID_CEREG
        case RIL_URC_ID_CEREG://6
        {
            tel_conn_mgr_nw_reg_status_struct *nw_reg_status = tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_nw_reg_status_struct));

            if (!nw_reg_status)
            {
                TEL_CONN_MGR_LOG_ERR("Lost CGREG URC");
                break;
            }

            nw_reg_status->act = ((ril_eps_network_registration_status_urc_t *)param)->act;
            nw_reg_status->stat = ((ril_eps_network_registration_status_urc_t *)param)->stat;

            urc_ind = tel_conn_mgr_bearer_gen_urc_ind(TEL_CONN_MGR_BEARER_URC_TYPE_CGREG,
                                                      (void *)nw_reg_status);

            if (!urc_ind)
            {
                TEL_CONN_MGR_LOG_ERR("Lost CGREG URC");         
            }
            
            break;   
        }
        case RIL_URC_ID_CGEV:
        {
            tel_conn_mgr_cgev_urc_struct *cgev_urc = tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_cgev_urc_struct));

            if (!cgev_urc)
            {
                TEL_CONN_MGR_LOG_ERR("Lost CGEV URC");
                break;
            }

            cgev_urc->cid = TEL_CONN_MGR_MIN_CID - 1;
            switch (((ril_cgev_event_reporting_urc_t *)param)->response_type)
            {
                case RIL_URC_TYPE_NW_DETACH:
                {
                    cgev_urc->cgev_urc_type = TEL_CONN_MGR_BEARER_CGEV_URC_TYPE_NW_DETACH;                    
                    break;
                }

                case RIL_URC_TYPE_ME_DETACH:
                {
                    cgev_urc->cgev_urc_type = TEL_CONN_MGR_BEARER_CGEV_URC_TYPE_ME_DETACH;
                    break;
                }

                case RIL_URC_TYPE_ME_PDN_DEACT:
                {
                    cgev_urc->cgev_urc_type = TEL_CONN_MGR_BEARER_CGEV_URC_TYPE_ME_PDN_DEACT;
                    cgev_urc->cid = ((ril_cgev_event_reporting_urc_t *)param)->response1.cid;
                    break;
                }

                case RIL_URC_TYPE_NW_PDN_DEACT:
                {
                    cgev_urc->cgev_urc_type = TEL_CONN_MGR_BEARER_CGEV_URC_TYPE_NW_PDN_DEACT;
                    cgev_urc->cid = ((ril_cgev_event_reporting_urc_t *)param)->response1.cid;
                    break;
                }

                case RIL_URC_TYPE_ME_PDN_ACT:
                {
                    cgev_urc->cgev_urc_type = TEL_CONN_MGR_BEARER_CGEV_URC_TYPE_ME_PDN_ACT;
                    cgev_urc->cid = ((ril_cgev_event_reporting_urc_t *)param)->response1.cid;
                    cgev_urc->reason = ((ril_cgev_event_reporting_urc_t *)param)->response2.reason;
#ifdef TEL_CONN_MGR_TEMP_IT
                    TEL_CONN_MGR_LOG_INFO("[TCM]cid from ril:%d, cid of tcm:%d",
                        (int)((ril_cgev_event_reporting_urc_t *)param)->response1.cid,
                        cgev_urc->cid);
#endif
                    break;
                }

                default:
                {
                    TEL_CONN_MGR_LOG_ERR("Drop non-concern CGEV URC");
                    tel_conn_mgr_free(cgev_urc);
                    return 0;
                }
            }           

            urc_ind = tel_conn_mgr_bearer_gen_urc_ind(TEL_CONN_MGR_BEARER_URC_TYPE_CGEV,
                                                      (void *)cgev_urc);

            if (!urc_ind)
            {
                TEL_CONN_MGR_LOG_ERR("Lost CGEV URC");
                tel_conn_mgr_free(cgev_urc);
            }
            
            break;   
        }


        default:
            break;            
    }

    if (urc_ind)
    {
        if (tel_conn_mgr_send_msg_to_tcm_task((tel_conn_mgr_msg_struct *)urc_ind))
        {
            TEL_CONN_MGR_LOG_INFO("Send URC IND to TCM Task successfully. event_id:%d", event_id);
            return 0;
        }

        tel_conn_mgr_free(urc_ind);
    }

    return -2;
}


tel_conn_mgr_ret_enum tel_conn_mgr_bearer_urc_init(void)
{
    ril_status_t ret = RIL_STATUS_FAIL;

#ifdef TEL_CONN_MGR_UT
    ret = ril_register_event_callback_ut(RIL_ALL, tel_conn_mgr_bearer_urc_callback);
#else
    ret = ril_register_event_callback(RIL_ALL, tel_conn_mgr_bearer_urc_callback);
#endif
    
    return RIL_STATUS_SUCCESS == ret ? TEL_CONN_MGR_RET_OK : TEL_CONN_MGR_RET_ERROR;
}


tel_conn_mgr_ret_enum tel_conn_mgr_bearer_urc_deinit(void)
{
    ril_status_t ret = RIL_STATUS_FAIL;
    
#ifdef TEL_CONN_MGR_UT
    ret = ril_deregister_event_callback_ut(tel_conn_mgr_bearer_urc_callback);
#else
    ret = ril_deregister_event_callback(tel_conn_mgr_bearer_urc_callback);
#endif
    
    return RIL_STATUS_SUCCESS == ret ? TEL_CONN_MGR_RET_OK : TEL_CONN_MGR_RET_ERROR;
}

