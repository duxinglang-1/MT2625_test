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
#include "tel_conn_mgr_common.h"
#include "tel_conn_mgr_bearer_msg_hdlr.h"
#include "tel_conn_mgr_bearer_urc.h"
#include "tel_conn_mgr_bearer_at_cmd_util.h"
#include "tel_conn_mgr_bearer_util.h"
#include "tel_conn_mgr_bearer_api.h"
#ifdef TEL_CONN_MGR_UT
#include "tel_conn_mgr_ut.h"
#endif

#define TEL_CONN_MGR_TASK_QUEUE_MAX_SIZE      (10)


tel_conn_mgr_queue_hdl_t tel_conn_mgr_task_queue_hdl = NULL;

tel_conn_mgr_task_hdl_t tel_conn_mgr_task_hdl = NULL;


tel_conn_mgr_ret_enum tel_conn_mgr_task_queue_create(void)
{
    if (tel_conn_mgr_task_queue_hdl)
    {
        TEL_CONN_MGR_LOG_WARN("Task queue has been created.");
        return TEL_CONN_MGR_RET_OK;
    }
    
    tel_conn_mgr_task_queue_hdl = tel_conn_mgr_queue_create(TEL_CONN_MGR_TASK_QUEUE_MAX_SIZE,
                                                            sizeof(tel_conn_mgr_msg_struct *));    

    if (!tel_conn_mgr_task_queue_hdl)
    {
        TEL_CONN_MGR_LOG_ERR("Failed to create queue.");
        return TEL_CONN_MGR_RET_LIMIT_RESOURCE;
    }

    return TEL_CONN_MGR_RET_OK;
}


void tel_conn_mgr_task_queue_free(void)
{
    if (tel_conn_mgr_task_queue_hdl)
    {
        tel_conn_mgr_queue_delete(tel_conn_mgr_task_queue_hdl);
        tel_conn_mgr_task_queue_hdl = NULL;
    }    
}


tel_conn_mgr_queue_hdl_t tel_conn_mgr_task_queue_get(void)
{
    return tel_conn_mgr_task_queue_hdl;
}


void tel_conn_mgr_task_main(void *param)
{
    tel_conn_mgr_msg_struct *msg = NULL;

#ifdef TEL_CONN_MGR_UT
    tel_conn_mgr_ut_init(2, 0);
#endif
    
    while (tel_conn_mgr_task_queue_hdl)
    {
        if (tel_conn_mgr_queue_receive(tel_conn_mgr_task_queue_hdl, &msg, TEL_CONN_MAX_PORT_MAX_DELAY))
        {
            switch (msg->msg_id)
            {
                case MSG_ID_TEL_CONN_MGR_ACTIVATION_REQ:
                {
                    tel_conn_mgr_act_req_hdlr((void *)msg);
                    break;
                }
                
                case MSG_ID_TEL_CONN_MGR_DEACTIVATION_REQ:
                {
                    tel_conn_mgr_deact_req_hdlr((void *)msg);
                    break;
                }

                case MSG_ID_TEL_CONN_MGR_BEARER_AT_CMD_RSP_IND:
                {
                    tel_conn_mgr_at_cmd_rsp_ind_hdlr((void *)msg);
                    break;
                }

                case MSG_ID_TEL_CONN_MGR_BEARER_URC_IND:
                {
                    tel_conn_mgr_bearer_urc_ind_hdlr((void *)msg);
                    break;
                }
                
                default:
                    break;
            }

            if (MSG_ID_TEL_CONN_MGR_BEARER_URC_IND == msg->msg_id)
            {
                tel_conn_mgr_bearer_free_urc_ind((tel_conn_mgr_bearer_urc_ind_struct *)msg);
            }
            else if (MSG_ID_TEL_CONN_MGR_BEARER_AT_CMD_RSP_IND == msg->msg_id)
            {
                tel_conn_mgr_bearer_at_cmd_rsp_ind_free((tel_conn_mgr_bearer_at_cmd_rsp_ind_struct *)msg);
            }
            else
            {
                tel_conn_mgr_free(msg);
            }
            msg = NULL;
        }
    }

    tel_conn_mgr_task_queue_free();

    tel_conn_mgr_task_hdl = NULL;
    tel_conn_mgr_task_delete(NULL);
}


void tel_conn_mgr_task_deinit(void)
{
    tel_conn_mgr_task_queue_free();
    tel_conn_mgr_app_deinit();
    tel_conn_mgr_bearer_deinit();
    tel_conn_mgr_util_deinit();
}


tel_conn_mgr_ret_enum tel_conn_mgr_task_init(void)
{
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;

    ret = tel_conn_mgr_task_queue_create();
    if (TEL_CONN_MGR_RET_OK != ret)
    {
        goto exit;
    }

    ret = tel_conn_mgr_bearer_init();
    if (TEL_CONN_MGR_RET_OK != ret)
    {
        goto exit;
    }

    ret = tel_conn_mgr_util_init();
    if (TEL_CONN_MGR_RET_OK != ret)
    {
        goto exit;
    }

    ret = tel_conn_mgr_app_init();    

exit:

    if (TEL_CONN_MGR_RET_OK != ret)
    {
        tel_conn_mgr_task_deinit();
    }

    return ret;
}



void tel_conn_mgr_init(void)
{
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    
    TEL_CONN_MGR_LOG_ERR("%s", __FUNCTION__);
    
    ret = tel_conn_mgr_task_init();

    if (TEL_CONN_MGR_RET_OK != ret)
    {
        TEL_CONN_MGR_LOG_ERR("tel conn mgr init failed. ret:%d", ret);
        return;
    }
    #ifdef MTK_CREATE_DEFAULT_APN
    tel_conn_mgr_default_pdn_link_init();
    #endif
    
    if (!tel_conn_mgr_task_hdl)
    {
        tel_conn_mgr_task_create(tel_conn_mgr_task_main,
                                 TEL_CONN_TASK_NAME,
                                 TEL_CONN_TASK_STACKSIZE/sizeof(TEL_CONN_MGR_PORT_STACK_TYPE),
                                 NULL,
                                 TEL_CONN_TASK_PRIO,
                                 &tel_conn_mgr_task_hdl);
    }

    if (!tel_conn_mgr_task_hdl)
    {
        tel_conn_mgr_task_deinit();
        TEL_CONN_MGR_LOG_ERR("Failed to create tel_conn_mgr task.");        
    }
    

}

