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

#include "tel_conn_mgr_platform.h"
#include "tel_conn_mgr_bearer_at_cmd_util.h"
#include "tel_conn_mgr_bearer_util.h"
#include "tel_conn_mgr_bearer_timer.h"
#include "tel_conn_mgr_bearer_cache.h"
#include "tel_conn_mgr_bearer_urc.h"
#include "tel_conn_mgr_bearer_api.h"


/* Only when return value is TEL_CONN_MGR_RET_OK, will the callback be invoked.
 * CONN_MGR will start a GUARD Timer of 2 min for activation.
 */
tel_conn_mgr_ret_enum tel_conn_mgr_bearer_activate(int cid,
                                       tel_conn_mgr_activate_callback callback)
{
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_OK;
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    tel_conn_mgr_bearer_cntx_struct *bearer_cntx = tel_conn_mgr_bearer_get_cntx();
#ifdef TEL_CONN_MGR_ENABLE_INACTIVATE
    tel_conn_mgr_bearer_state_enum *sp_cid_bearer_state = tel_conn_mgr_get_bearer_state_by_cid(TEL_CONN_MGR_SPECIAL_CID);
#endif
    
    TEL_CONN_MGR_LOG_INFO("cid:%d, cb:%x", cid, (unsigned int)callback);

    if (!callback || !bearer_state ||
#ifdef TEL_CONN_MGR_ENABLE_INACTIVATE
        !sp_cid_bearer_state ||
#endif
        !tel_conn_mgr_is_cid_valid(cid) ||
        tel_conn_mgr_is_special_cid(cid))
    {
        TEL_CONN_MGR_LOG_INFO("%s, Invalid Param", __FUNCTION__);
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

#ifdef TEL_CONN_MGR_ENABLE_INACTIVATE
    TEL_CONN_MGR_LOG_INFO("bearer_cntx:%x, bearer_state:%x, sp_cid_bearer_state:%x",
                          (unsigned int)bearer_cntx, (unsigned int)*bearer_state, (unsigned int)*sp_cid_bearer_state);
#else
    TEL_CONN_MGR_LOG_INFO("bearer_cntx:%x, bearer_state:%x",
                             (unsigned int)bearer_cntx, (unsigned int)*bearer_state);
#endif

    /* Listen to modem EIND? */
    if (!bearer_cntx || TEL_CONN_MGR_MODEM_STATE_READY != bearer_cntx->modem_state)
    {
        TEL_CONN_MGR_LOG_INFO("%s, State Error", __FUNCTION__);
        return TEL_CONN_MGR_RET_WRONG_STATE;
    }

    if (tel_conn_mgr_is_bearer_activating(*bearer_state))
    {
        /* Drop the msg */
        TEL_CONN_MGR_LOG_WARN("Duplicate act req. bearer_state:%x", *bearer_state);
        return TEL_CONN_MGR_RET_DUPLICATION;
    }
    else if (tel_conn_mgr_is_bearer_active(*bearer_state))
    {
        return TEL_CONN_MGR_RET_OK;
    }
    else if (tel_conn_mgr_is_bearer_inactive(*bearer_state) ||
             tel_conn_mgr_is_bearer_deactivating(*bearer_state))
    {
        if (tel_conn_mgr_is_nw_registration_failed())
        {
          #ifdef MTK_CREATE_DEFAULT_APN
          if(!tel_conn_mgr_default_get_apn_flag())

          #endif
            return TEL_CONN_MGR_RET_ERROR;
        }

        if (tel_conn_mgr_is_bearer_deactivating(*bearer_state))
        {            
            /* Turn into activation immediately */
            tel_conn_mgr_bearer_terminate_deactivating_state(cid, TEL_CONN_MGR_FALSE, TEL_CONN_MGR_ERR_CAUSE_ACT_REQ);
        }

#ifdef TEL_CONN_MGR_ENABLE_INACTIVATE
        if (!(TEL_CONN_MGR_BEARER_STATE_INACTIVE & (*sp_cid_bearer_state)))
        {
            if (!(TEL_CONN_MGR_BEARER_STATE_INACTIVATING & (*sp_cid_bearer_state)))
            {
                /* Start to send at cmds in INACTIVATE class. The status of NW REG does not effect INACTIVATE. 
                                * However, 
                                */
                ret = tel_conn_mgr_send_at_cmds_by_class(TEL_CONN_MGR_SPECIAL_CID,
                                                         TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE,
                                                         TEL_CONN_MGR_AT_CMD_TYPE_NONE,
                                                         NULL);
            }

            /* Cache the request. */
            if (TEL_CONN_MGR_RET_OK == ret)
            {
                ret = tel_conn_mgr_cache_add_class(cid,
                                                   TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE,
                                                   TEL_CONN_MGR_AT_CMD_TYPE_NONE,
                                                   (void *)callback,
                                                   TEL_CONN_MGR_BEARER_WAIT_EVENT_INACTIVE_DONE);
                /* When INACTIVATE is done, checking nw reg status is needed. Only when NW REGED,
                            * can ACT AT CMDs be sent. This is beacause, NW may be REGED before INACTIVATE is
                            * done. However, it may be turned to NW REG Fail when INACTIVATE is done. */
            }
        }
        else
#endif
        {
            if (tel_conn_mgr_is_nw_registered() 
                  #ifdef MTK_CREATE_DEFAULT_APN
                || tel_conn_mgr_default_get_apn_flag()
                  #endif
                )
            {
                ret = tel_conn_mgr_send_at_cmds_by_class(cid,
                                                       TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE,
                                                       TEL_CONN_MGR_AT_CMD_TYPE_NONE,
                                                       (void *)callback);
            }
            else
            {
#ifdef TEL_CONN_MGR_ENABLE_CHECK_CEREG
                ret = tel_conn_mgr_send_at_cmds_by_class(cid,
                                                       TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE,
                                                       TEL_CONN_MGR_AT_CMD_TYPE_NONE,
                                                       (void *)callback);
#else
                ret = tel_conn_mgr_cache_add_class(cid,
                                                       TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE,
                                                       TEL_CONN_MGR_AT_CMD_TYPE_NONE,
                                                       (void *)callback,
                                                       TEL_CONN_MGR_BEARER_WAIT_EVENT_NW_REG_RESULT);
#endif
            }
        }

        if (TEL_CONN_MGR_RET_OK == ret)
        {
            ret = TEL_CONN_MGR_RET_WOULDBLOCK;

#ifdef TEL_CONN_MGR_ACTIVATION_GUARD_TIMER_SUPPORT
            tel_conn_mgr_timer_start(TEL_CONN_MGR_MODME_TIMER_ID_ACTIVATION, cid,
                                   TEL_CONN_MGR_ACTIVATION_GUARD_TIMER_EXPIRY_TIME,
                                   tel_conn_mgr_activation_timeout_hdlr);
#endif
        }
    }
    else
    {
        /* Invalid state. */
        ret = TEL_CONN_MGR_RET_WRONG_STATE;
    }

    return ret;
}


/* Only when return value is TEL_CONN_MGR_RET_WOULDBLOCK, will the callback be invoked. 
  * If app state is deacting, app state will be updated to inactive no matter the value of ret.
  * At the next activation/deactivation, last_key_at_cmd and at+cgact? will handle it properly.
  */
tel_conn_mgr_ret_enum tel_conn_mgr_bearer_deactivate(int cid,
                                          tel_conn_mgr_deactivate_callback callback)
{
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_OK;
    unsigned int bearer_info_idx = tel_conn_mgr_convt_cid_to_bearer_info_idx(cid);
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    tel_conn_mgr_bearer_cntx_struct *bearer_cntx = tel_conn_mgr_bearer_get_cntx();
    tel_conn_mgr_bool is_activate_cached = TEL_CONN_MGR_FALSE;

    TEL_CONN_MGR_LOG_INFO("deactivate cid: %d, cb:%x", cid, (unsigned int)callback);

    if (!callback || !bearer_state ||
        !tel_conn_mgr_is_idx_valid(bearer_info_idx) ||
        tel_conn_mgr_is_special_idx(bearer_info_idx))
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    if (!bearer_cntx || TEL_CONN_MGR_MODEM_STATE_READY != bearer_cntx->modem_state)
    {
        return TEL_CONN_MGR_RET_WRONG_STATE;
    }

    TEL_CONN_MGR_LOG_INFO("bearer_state:%x", *bearer_state);

    if (tel_conn_mgr_is_bearer_inactive(*bearer_state))
    {
        return TEL_CONN_MGR_RET_OK;
    }
    else if (tel_conn_mgr_is_bearer_deactivating(*bearer_state))
    {
        /* DEACTIVATE REQ should not be sent during deactivating. */
        return TEL_CONN_MGR_RET_DUPLICATION;
    }
    else if (tel_conn_mgr_is_bearer_active(*bearer_state) ||
             tel_conn_mgr_is_bearer_activating(*bearer_state) ||
             TEL_CONN_MGR_BEARER_STATE_NEED_DEACT & (*bearer_state))
    {
        if (TEL_CONN_MGR_BEARER_STATE_NEED_DEACT & (*bearer_state))
        {
            (*bearer_state) &= ~TEL_CONN_MGR_BEARER_STATE_NEED_DEACT;
        }
        
        if (tel_conn_mgr_is_bearer_activating(*bearer_state))
        {
            is_activate_cached = tel_conn_mgr_cache_is_class_cached(cid, TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE);

            TEL_CONN_MGR_LOG_INFO("is_activate_cached:%d", is_activate_cached);
            
            tel_conn_mgr_bearer_terminate_activating_state(cid,
                                                           TEL_CONN_MGR_FALSE,
                                                           TEL_CONN_MGR_ERR_CAUSE_DEACT_REQ,
                                                           TEL_CONN_MGR_FALSE);
        }

        if (is_activate_cached)
        {
            TEL_CONN_MGR_LOG_INFO("ACTIVATE is cached. Return OK for deactivation directly.");
            return TEL_CONN_MGR_RET_OK;
        }
        
        ret = tel_conn_mgr_send_at_cmds_by_class(cid,
                                                 TEL_CONN_MGR_AT_CMD_CLASS_DEACTIVATE,
                                                 TEL_CONN_MGR_AT_CMD_TYPE_NONE,
                                                 (void *)callback);
        if (TEL_CONN_MGR_RET_OK == ret)
        {
            ret = TEL_CONN_MGR_RET_WOULDBLOCK;

#ifdef TEL_CONN_MGR_DEACTIVATION_GUARD_TIMER_SUPPORT
            tel_conn_mgr_timer_start(TEL_CONN_MGR_MODME_TIMER_ID_DEACTIVATION, cid,
                                   TEL_CONN_MGR_DEACTIVATION_GUARD_TIMER_EXPIRY_TIME,
                                   tel_conn_mgr_deactivation_timeout_hdlr);
#endif
        }
    }
    else
    {
        /* Invalid state. */
        ret = TEL_CONN_MGR_RET_WRONG_STATE;
    }

    return ret;
}


/* deinit function should be called manually if init fails. */
tel_conn_mgr_ret_enum tel_conn_mgr_bearer_init(void)
{
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_OK;

    TEL_CONN_MGR_LOG_INFO("%s", __FUNCTION__);

    ret = tel_conn_mgr_at_cmd_init();

    if (TEL_CONN_MGR_RET_OK != ret)
    {
        return ret;
    }

    ret = tel_conn_mgr_bearer_urc_init();
    if (TEL_CONN_MGR_RET_OK != ret)
    {
        return ret;
    }

#if defined(TEL_CONN_MGR_ACTIVATION_GUARD_TIMER_SUPPORT) || defined(TEL_CONN_MGR_DEACTIVATION_GUARD_TIMER_SUPPORT)
    tel_conn_mgr_timer_init();
#endif

    return ret;
}


void tel_conn_mgr_bearer_deinit(void)
{
    tel_conn_mgr_at_cmd_deinit();
    tel_conn_mgr_bearer_urc_deinit();
    
#if defined(TEL_CONN_MGR_ACTIVATION_GUARD_TIMER_SUPPORT) || defined(TEL_CONN_MGR_DEACTIVATION_GUARD_TIMER_SUPPORT)
    tel_conn_mgr_timer_deinit();
#endif
}

