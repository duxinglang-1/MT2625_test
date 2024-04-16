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
#include "tel_conn_mgr_bearer_util.h"
#include "tel_conn_mgr_bearer_list_mgr.h"

void tel_conn_mgr_at_cmd_list_node_free(tel_conn_mgr_at_cmd_list_struct *list_node)
{
    TEL_CONN_MGR_LOG_INFO("%s", __FUNCTION__);
    
    if (!list_node)
    {
        return;
    }

    tel_conn_mgr_free(list_node);
}


tel_conn_mgr_at_cmd_list_struct *tel_conn_mgr_at_cmd_list_node_create(tel_conn_mgr_at_cmd_class_enum at_cmd_class,
                                                                                 tel_conn_mgr_at_cmd_type_enum at_cmd_type,
                                                                                 int cid)
{
    tel_conn_mgr_at_cmd_list_struct *list_node = NULL;
    
    if (!tel_conn_mgr_is_cid_valid(cid) ||
        !(TEL_CONN_MGR_AT_CMD_TYPE_NONE < at_cmd_type &&
          TEL_CONN_MGR_AT_CMD_TYPE_MAX > at_cmd_type))
    {
        return NULL;
    }

    list_node = (tel_conn_mgr_at_cmd_list_struct *)tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_at_cmd_list_struct));
    memset(list_node, 0, sizeof(tel_conn_mgr_at_cmd_list_struct));
    
    list_node->at_cmd_class = at_cmd_class;
    list_node->at_cmd_type = at_cmd_type;
    list_node->cid = cid;

    return list_node;
}


void tel_conn_mgr_class_list_node_free(tel_conn_mgr_class_list_struct *list_node)
{
    if (!list_node)
    {
        return;
    }
    
    tel_conn_mgr_free(list_node);
}


tel_conn_mgr_class_list_struct *tel_conn_mgr_class_list_node_create(int cid,
                                                                   tel_conn_mgr_at_cmd_class_enum at_cmd_class,
                                                                   void *callback,
                                                                   int wait_event_group)
{
    tel_conn_mgr_class_list_struct *list_node = NULL;
    
    if (!tel_conn_mgr_is_cid_valid(cid) ||
        !tel_conn_mgr_is_valid_at_cmd_class(at_cmd_class))
    {
        return NULL;
    }

    list_node = (tel_conn_mgr_class_list_struct *)tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_class_list_struct));
    memset(list_node, 0, sizeof(tel_conn_mgr_class_list_struct));

    list_node->cid = cid;
    list_node->at_cmd_class = at_cmd_class;
    list_node->callback = callback;
    list_node->wait_event_group= wait_event_group;

    return list_node;
}


tel_conn_mgr_bearer_template_list_struct *tel_conn_mgr_find_list_node(tel_conn_mgr_bearer_template_list_struct *list,
                                                           int cid,
                                                           tel_conn_mgr_at_cmd_class_enum at_cmd_class)
{
    tel_conn_mgr_bearer_template_list_struct *list_node = list;
    
    if (!list || !tel_conn_mgr_is_cid_valid(cid) ||
        !tel_conn_mgr_is_valid_at_cmd_class(at_cmd_class))
    {
        return NULL;
    }

    while (list_node)
    {
        if (list_node->cid == cid && list_node->at_cmd_class == at_cmd_class)
        {
            return list_node;
        }

        list_node = list_node->next;
    }

    return NULL;
}

