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
#include "memory_attribute.h"
#include "tel_conn_mgr_util.h"
#include "tel_conn_mgr_bearer_at_cmd_util.h"
#include "tel_conn_mgr_bearer_info.h"

#ifdef TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM
ATTR_RWDATA_IN_RETSRAM tel_conn_mgr_bearer_info_ds_keep_struct g_ds_keep[TEL_CONN_MGR_BEARER_TYPE_MAX_NUM];
#endif

tel_conn_mgr_bearer_info_struct *tel_conn_mgr_bearer_info_find_free_slot(unsigned int *bearer_info_idx)
{
    unsigned int idx = 0;    
    tel_conn_mgr_bearer_cntx_struct *bearer_cntx = tel_conn_mgr_bearer_get_cntx();

    //TEL_CONN_MGR_LOG_INFO("%s", __FUNCTION__);

    if (!bearer_cntx || !bearer_info_idx)
    {
        return NULL;
    }

    *bearer_info_idx = TEL_CONN_MGR_BEARER_TYPE_MAX_NUM;
    
    for (idx = 0; idx < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; idx++)
    {
        if (!bearer_cntx->bearer_info[idx].is_used)
        {
            *bearer_info_idx = idx;
            TEL_CONN_MGR_LOG_INFO("%s Found idx:%d", __FUNCTION__, idx);
            return &(bearer_cntx->bearer_info[idx]);
        }
    }

    TEL_CONN_MGR_LOG_INFO("%s not found", __FUNCTION__);
    return NULL;
}


/* Only when app state and bearer state are both INACTIVE, will freeing the bearer_info be allowed.
  * This API is usually be called just after the activate/deactivate callback for within callback app state may be updated into INACTIVE.
  * So before the callback function is invoked, all kinds of information should be freed first such as at cmd lists, class lists, and
  * current_at_cmd_type.
  */
void tel_conn_mgr_bearer_info_free(unsigned int bearer_info_idx)
{
    tel_conn_mgr_bearer_info_struct *bearer_info = tel_conn_mgr_bearer_info_find_by_idx(bearer_info_idx);
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_idx(bearer_info_idx);
    tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_idx(bearer_info_idx);
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_idx(bearer_info_idx);

    TEL_CONN_MGR_LOG_INFO("%s, idx:%d", __FUNCTION__, bearer_info_idx);
    if (!tel_conn_mgr_is_idx_valid(bearer_info_idx) ||
        !bearer_info || !at_cmd_flow_helper || !ds_keep || !bearer_state)
    {
        return;
    }

    if (bearer_info->is_used &&
        (TEL_CONN_MGR_APP_STATE_INACTIVE == tel_conn_mgr_app_get_app_state(bearer_info_idx) ||
        TEL_CONN_MGR_APP_STATE_MAX == tel_conn_mgr_app_get_app_state(bearer_info_idx)) &&
        (TEL_CONN_MGR_BEARER_STATE_MAX == (*bearer_state) ||
        TEL_CONN_MGR_BEARER_STATE_INACTIVE & (*bearer_state)) &&
        TEL_CONN_MGR_AT_CMD_TYPE_NONE == at_cmd_flow_helper->current_at_cmd_type &&
        !at_cmd_flow_helper->at_cmd_to_send_list &&
      //  !at_cmd_flow_helper->at_cmd_to_recv_list &&  //resolve the issue: passive deactive when actie the PDN link in process,
        !at_cmd_flow_helper->class_to_excute &&
        !at_cmd_flow_helper->class_to_finish)
    {
        TEL_CONN_MGR_LOG_INFO("bearer_info is freed. idx:%d", bearer_info_idx);

        bearer_info->is_used = TEL_CONN_MGR_FALSE;
        at_cmd_flow_helper->last_key_at_cmd_type = TEL_CONN_MGR_KEY_AT_CMD_TYPE_NONE;        
        ds_keep->data_channel_id = TEL_CONN_MGR_MIN_VALID_CHANNEL_ID - 1;
        /* RTC will check cid to determine if the bearer_info is used or not. */
        ds_keep->cid = TEL_CONN_MGR_MIN_CID - 1;
        if (at_cmd_flow_helper->at_cmd_to_recv_list) {
            
            TEL_CONN_MGR_LOG_INFO("recv_list free.!!!!");
            tel_conn_mgr_at_cmd_list_node_free(at_cmd_flow_helper->at_cmd_to_recv_list);
        }
    }
    else
    {
        TEL_CONN_MGR_LOG_INFO("Trying to free bearer_info failed. idx:%d", bearer_info_idx);
        TEL_CONN_MGR_LOG_INFO("is_used:%d, app_state:%x, bearer_state:%x, curr_atcmd_type:%d",
            bearer_info->is_used,
            tel_conn_mgr_app_get_app_state(bearer_info_idx),
            (*bearer_state),
            at_cmd_flow_helper->current_at_cmd_type);
        TEL_CONN_MGR_LOG_INFO("at_cmd_to_send_list:%x, at_cmd_to_recv_list:%x, class_to_excute:%x, class_to_finish:%x",
            (unsigned int)at_cmd_flow_helper->at_cmd_to_send_list,
            (unsigned int)at_cmd_flow_helper->at_cmd_to_recv_list,
            (unsigned int)at_cmd_flow_helper->class_to_excute,
            (unsigned int)at_cmd_flow_helper->class_to_finish);
    }
}

tel_conn_mgr_bool tel_conn_mgr_pdptype_equal(tel_conn_mgr_pdp_type_enum p1, tel_conn_mgr_pdp_type_enum p2)
{
    TEL_CONN_MGR_LOG_INFO("p1:%d, p2:%d\r\n", p1, p2);
    if ((p1 != TEL_CONN_MGR_PDP_TYPE_NIDD) && (p2!=TEL_CONN_MGR_PDP_TYPE_NIDD)) {
        if ((p1 == p2) || (p2 == TEL_CONN_MGR_PDP_TYPE_IPV4V6)){
            return TEL_CONN_MGR_TRUE;
        } else {
            return TEL_CONN_MGR_FALSE;
        }
    }
    return TEL_CONN_MGR_FALSE;

}

/* Return: <0 error
  *            Otherwise, the index of the bearer_info() array within cntx
  */
tel_conn_mgr_bearer_info_struct *tel_conn_mgr_bearer_info_find_by_info(tel_conn_mgr_bearer_type_enum bearer_type,
                                                                      tel_conn_mgr_sim_id_enum sim_id,
                                                                      tel_conn_mgr_pdp_type_enum pdp_type,
                                                                      char *apn,
                                                                      unsigned int *bearer_info_idx)
{
    unsigned int idx = 0;
    tel_conn_mgr_bearer_info_struct *bearer_info = NULL;
    tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep = NULL;
    
    if (TEL_CONN_MGR_BEARER_TYPE_NONE >= bearer_type || TEL_CONN_MGR_BEARER_TYPE_MAX <= bearer_type ||
        TEL_CONN_MGR_PDP_TYPE_NONE >= pdp_type || TEL_CONN_MGR_PDP_TYPE_MAX <= pdp_type ||
        TEL_CONN_MGR_SIM_ID_NONE >= sim_id || TEL_CONN_MGR_SIM_ID_MAX <= sim_id || !apn || !bearer_info_idx)
    {
        return NULL;
    }

    for (idx = 0; idx < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; idx++)
    {
        bearer_info = tel_conn_mgr_bearer_info_find_by_idx(idx);
        TEL_CONN_MGR_LOG_INFO("idx:%d, bearer_info:%x, is_used:%d\r\n", idx, bearer_info, bearer_info ? bearer_info->is_used : 0);
        ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_idx(idx);
        /* bearer_info is lazily freed. Only is_used is set to TEL_CONN_MGR_FALSE when it is freed. */
        /* PDP types IP/IPV6/IPV4V6 will be treated as the same. */
        if (bearer_info && ds_keep)
        {


            TEL_CONN_MGR_LOG_INFO("-----%d,%d,\r\n", bearer_info->bearer_type,bearer_type);
            TEL_CONN_MGR_LOG_INFO("-----%d,%d,%d\r\n", sim_id,pdp_type,ds_keep->pdp_type);

            TEL_CONN_MGR_LOG_INFO("-----%s,%s\r\n",ds_keep->apn,apn);
            {
                bool mm = tel_conn_mgr_str_case_equal(ds_keep->apn, apn, TEL_CONN_MGR_APN_MAX_LEN);

                TEL_CONN_MGR_LOG_INFO("-----%d\r\n",mm);
            }
{
            bool nn = tel_conn_mgr_pdptype_equal( pdp_type,  ds_keep->activated_pdp_type);
            TEL_CONN_MGR_LOG_INFO("act pdn type-----%d\r\n",nn);
                }
        }
          
        
        if (bearer_info && ds_keep &&
            bearer_info->is_used &&
            bearer_info->bearer_type == bearer_type &&
            bearer_info->sim_id == sim_id &&            
            (pdp_type == ds_keep->pdp_type || tel_conn_mgr_pdptype_equal( pdp_type,  ds_keep->activated_pdp_type))&&
            tel_conn_mgr_str_case_equal(ds_keep->apn, apn, TEL_CONN_MGR_APN_MAX_LEN))
        {
            *bearer_info_idx = idx;
            TEL_CONN_MGR_LOG_INFO("Found, cid:%d", ds_keep->cid);
            return bearer_info;
        }
    }

    TEL_CONN_MGR_LOG_INFO("Not Found");
    return NULL;
}


tel_conn_mgr_bearer_info_struct *tel_conn_mgr_bearer_info_find_by_idx(int bearer_info_idx)
{
    tel_conn_mgr_bearer_cntx_struct *bearer_cntx = tel_conn_mgr_bearer_get_cntx();

    #if 0
    TEL_CONN_MGR_LOG_INFO("is_idx_valid: %d", tel_conn_mgr_is_idx_valid(bearer_info_idx));
    TEL_CONN_MGR_LOG_INFO("is_special_idx: %d", tel_conn_mgr_is_special_idx(bearer_info_idx));
    if (!tel_conn_mgr_is_special_idx(bearer_info_idx))
    {
        TEL_CONN_MGR_LOG_INFO("bearer_cntx:%x, is_used:%d", bearer_cntx, bearer_cntx->bearer_info[bearer_info_idx].is_used);
    }
    #endif
    
    if (tel_conn_mgr_is_idx_valid(bearer_info_idx) && !tel_conn_mgr_is_special_idx(bearer_info_idx) &&
        bearer_cntx && bearer_cntx->bearer_info[bearer_info_idx].is_used)
    {
        //TEL_CONN_MGR_LOG_INFO("%s Found idx:%d", __FUNCTION__, bearer_info_idx);
        return &(bearer_cntx->bearer_info[bearer_info_idx]);
    }

    //TEL_CONN_MGR_LOG_INFO("Not Found idx:%d", bearer_info_idx);
    return NULL;
}


tel_conn_mgr_bearer_info_struct *tel_conn_mgr_bearer_info_find_by_cid(int cid)
{
    unsigned int bearer_info_idx = tel_conn_mgr_convt_cid_to_bearer_info_idx(cid);

    //TEL_CONN_MGR_LOG_INFO("idx:%d, cid:%d", bearer_info_idx, cid);

    return tel_conn_mgr_bearer_info_find_by_idx(bearer_info_idx);
}


tel_conn_mgr_bearer_info_ds_keep_struct *tel_conn_mgr_bearer_info_get_ds_keep_by_idx(unsigned int idx)
{
    tel_conn_mgr_bearer_info_struct *bearer_info = tel_conn_mgr_bearer_info_find_by_idx(idx);

#ifdef TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM
    if (bearer_info)
    {
        return &g_ds_keep[idx];
    }

    return NULL;
#else
    return bearer_info ? &(bearer_info->ds_keep) : NULL;
#endif
}


tel_conn_mgr_bearer_info_ds_keep_struct *tel_conn_mgr_bearer_info_get_ds_keep_by_idx_ds(unsigned int idx)
{

#ifdef TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM    
    TEL_CONN_MGR_LOG_INFO("g_ds_keep[idx]11");
    if (0 <= idx && TEL_CONN_MGR_BEARER_TYPE_MAX_NUM > idx) {        
            return &g_ds_keep[idx];
    }
    
    return NULL;
#else
    return NULL;
#endif
}

tel_conn_mgr_bearer_info_ds_keep_struct *tel_conn_mgr_bearer_info_get_ds_keep_by_cid(int cid)
{
    unsigned int idx = tel_conn_mgr_convt_cid_to_bearer_info_idx(cid);

    return tel_conn_mgr_bearer_info_get_ds_keep_by_idx(idx);
}


tel_conn_mgr_bearer_state_enum *tel_conn_mgr_get_bearer_state_by_cid(int cid)
{
    tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_cid(cid);
    tel_conn_mgr_bearer_cntx_struct *bearer_cntx = tel_conn_mgr_bearer_get_cntx();

    if (ds_keep)
    {
        return &(ds_keep->bearer_state);
    }

    if (bearer_cntx && tel_conn_mgr_is_special_cid(cid))
    {
        return &(bearer_cntx->bearer_state);
    }

    return NULL;
}


tel_conn_mgr_bearer_state_enum *tel_conn_mgr_get_bearer_state_by_idx(unsigned int bearer_info_idx)

{
    int cid = tel_conn_mgr_convt_bearer_info_idx_to_cid(bearer_info_idx);    

    return tel_conn_mgr_get_bearer_state_by_cid(cid);
}


tel_conn_mgr_at_cmd_flow_helper_struct *tel_conn_mgr_find_at_cmd_flow_helper_by_cid(int cid)
{
    tel_conn_mgr_bearer_info_struct *bearer_info = tel_conn_mgr_bearer_info_find_by_cid(cid);
    tel_conn_mgr_bearer_cntx_struct *bearer_cntx = tel_conn_mgr_bearer_get_cntx();

    if (bearer_info)
    {
        //TEL_CONN_MGR_LOG_INFO("%s Found cid:%d", __FUNCTION__, cid);
        return &(bearer_info->at_cmd_flow_helper);
    }

    if (tel_conn_mgr_is_special_cid(cid))
    {
        //TEL_CONN_MGR_LOG_INFO("%s Found cid:%d", __FUNCTION__, cid);
        return bearer_cntx ? &(bearer_cntx->init_class_helper) : NULL;
    }

    //TEL_CONN_MGR_LOG_INFO("Not Found cid:%d", cid);
    return NULL;
}


tel_conn_mgr_at_cmd_flow_helper_struct *tel_conn_mgr_find_at_cmd_flow_helper_by_idx(unsigned int bearer_info_idx)
{
    int cid = tel_conn_mgr_convt_bearer_info_idx_to_cid(bearer_info_idx);    

    return tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);
}

void tel_conn_mgr_bearer_info_ds_keep_init(void)
{
#ifdef TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM
    memset(g_ds_keep, 0, sizeof(g_ds_keep));
#endif
}
