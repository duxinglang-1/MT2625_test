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

#include "tel_conn_mgr_common.h"
#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
#include "hal_rtc_external.h"
#endif
#ifdef TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM
#include "memory_attribute.h"
#endif
#include "ril_cmds_def.h"
#include "ril.h"
#include "tel_conn_mgr_bearer_timer.h"
#include "tel_conn_mgr_bearer_util.h"
#include "tel_conn_mgr_bearer_cache.h"
#include "tel_conn_mgr_bearer_at_cmd_util.h"


#ifndef TEL_CONN_MGR_INFORM_TCPIP_NIDD
#include "tel_conn_mgr_ut.h"
#else
#include "nbnetif.h"
#endif


#if 0
#define TEL_CONN_MGR_DEFAULT_PDP_TYPE   ("IP")
#define TEL_CONN_MGR_DEFAULT_APN        ("internet")
#endif


typedef struct {
    char *at_cmd_str;
    tel_conn_mgr_at_cmd_class_enum cmd_class;
    tel_conn_mgr_at_cmd_type_enum cmd_type;
    tel_conn_mgr_bool send_on_data_channel;
} tel_conn_mgr_at_cmd_info_struct;


tel_conn_mgr_at_cmd_info_struct tel_conn_mgr_at_cmd_info[] = {
#ifdef TEL_CONN_MGR_ENABLE_INACTIVATE
    /* (1)Init */
    //{"ate0\r\n", TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_ATE0, TEL_CONN_MGR_FALSE},
    //{"at+ctzr=1\r\n", TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_CTZR_ON, TEL_CONN_MGR_FALSE},
    //{"at+eslp=0\r\n", TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_ESLP_DISABLE, TEL_CONN_MGR_FALSE},
    //{"at+cmee=0\r\n", TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_CMEE, TEL_CONN_MGR_FALSE},
    //{"at+cgreg=2\r\n", TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_CGREG, TEL_CONN_MGR_FALSE},
    {"at+cereg=2\r\n", TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_CEREG, TEL_CONN_MGR_FALSE},
    {"at+cgerep=1\r\n", TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_CGEREP, TEL_CONN_MGR_FALSE},
    //{"at+cfun=1\r\n", TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_CFUN_ON, TEL_CONN_MGR_FALSE},
    //{"at+eind=255\r\n", TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_EIND, TEL_CONN_MGR_FALSE},    
    //{"at+erat=2,2\r\n", TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_ERAT, TEL_CONN_MGR_FALSE}, /* 3G preferred */
#endif /* TEL_CONN_MGR_ENABLE_INACTIVATE */

    /* (2)Activate */
#ifdef TEL_CONN_MGR_ENABLE_CHECK_CEREG
    {"at+cereg?\r\n", TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_CEREG_QUERY, TEL_CONN_MGR_FALSE},
    {"at+cereg=2\r\n", TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_CEREG, TEL_CONN_MGR_FALSE},
#endif
    
    //{"at+cgact?\r\n", TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_CGACT_QUERY, TEL_CONN_MGR_FALSE},
    
    /* PDP_type(IP, IPV4V6), apn(Internet, 3gnet) */
    {"at+cgdcont=%d,\"%s\",\"%s\"\r\n", TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_CGDCONT, TEL_CONN_MGR_TRUE},    
    
    /* cid, 1, username, password */
    {"at+cgauth=%d,\"%s\",\"%s\"\r\n", TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_CGAUTH, TEL_CONN_MGR_TRUE},    
    
    /* The last AT CMD for ATCIVATE_CLASS shall be cgdata, for ME ACT URC is triggered by cgdata. 
        * Only when the last AT CMD is cgdata, can ACTING_URC be set after CLASS_ACTIVATE is done,
        * without the worry of receiving URC when CLASS_ACTIVATE is on-going.
        */
    {"at+cgdata=\"M-PT\",%d\r\n", TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_CGDATA, TEL_CONN_MGR_TRUE},

    /* (3)Get relevent informations such as IP, DNS.... */
    {"at+cgcontrdp=%d\r\n", TEL_CONN_MGR_AT_CMD_CLASS_GET_RELEVANT_INFO, TEL_CONN_MGR_AT_CMD_TYPE_CGCONTRDP, TEL_CONN_MGR_FALSE},

    /* (4)Deactivate */
    /* The last AT CMD for DEATCIVATE_CLASS shall be cgact=0, for ME DEACT URC is triggered by cgact=0. 
        * Only when the last AT CMD is cgact=0, can DEACTING_URC be set after CLASS_DEACTIVATE is done,
        * without the worry of receiving URC when CLASS_DEACTIVATE is on-going.
        */
    {"at+cgact?\r\n", TEL_CONN_MGR_AT_CMD_CLASS_DEACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_CGACT_QUERY, TEL_CONN_MGR_FALSE},
    {"at+cgact=0, %d\r\n", TEL_CONN_MGR_AT_CMD_CLASS_DEACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_CGACT_DEACT, TEL_CONN_MGR_FALSE},

    /* Add new item above */
    {NULL, TEL_CONN_MGR_AT_CMD_CLASS_NONE, TEL_CONN_MGR_AT_CMD_TYPE_NONE, TEL_CONN_MGR_FALSE}
};


char *tel_conn_mgr_pdp_type_str[] = 
{
    "IP",
    "IPV6",  
    "IPV4V6",
    "Non-IP",
};

#define TEL_CONN_MGR_BEARER_SLEEP_MANAGER_NAME "tel_conn_mgr_bearer_sleep_manager"

tel_conn_mgr_bearer_cntx_struct *tel_conn_mgr_bearer_cntx = NULL;

#ifdef TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM
ATTR_RWDATA_IN_RETSRAM tel_conn_mgr_nw_reg_status_struct g_nw_reg_status = {0};
#endif


tel_conn_mgr_bool tel_conn_mgr_is_data_channel_at_cmd_type(tel_conn_mgr_at_cmd_type_enum at_cmd_type)
{
    int i = -1;

    while (TEL_CONN_MGR_AT_CMD_TYPE_NONE != tel_conn_mgr_at_cmd_info[++i].cmd_type)
    {
        if (at_cmd_type == tel_conn_mgr_at_cmd_info[i].cmd_type)
        {
            return tel_conn_mgr_at_cmd_info[i].send_on_data_channel;
        }
    }

    return TEL_CONN_MGR_FALSE;
}


char *tel_conn_mgr_pdp_type_cov2str(tel_conn_mgr_pdp_type_enum pdp_type)
{
    if (TEL_CONN_MGR_PDP_TYPE_NONE >= pdp_type ||
        TEL_CONN_MGR_PDP_TYPE_MAX <= pdp_type)
    {
        return NULL;
    }

    return tel_conn_mgr_pdp_type_str[(int)pdp_type - 1];
}


void tel_conn_mgr_bearer_ip_info_free(tel_conn_mgr_bearer_ip_info_struct *ip_info)
{
    tel_conn_mgr_bearer_ip_info_struct *ip_info_next = NULL;

    if (!ip_info)
    {
        return;
    }

    while (ip_info) 
    {
        ip_info_next = ip_info->next;
        tel_conn_mgr_free(ip_info);
        ip_info = ip_info_next;
    }
}


void tel_conn_mgr_bearer_rsp_detail_free(tel_conn_mgr_at_cmd_type_enum at_cmd_type,
                                                   void *rsp_detail)
{
    if (!rsp_detail)
    {
        return;
    }

    switch (at_cmd_type)
    {
        case TEL_CONN_MGR_AT_CMD_TYPE_CGCONTRDP:
        {
            tel_conn_mgr_bearer_ip_info_free((tel_conn_mgr_bearer_ip_info_struct *)rsp_detail);            
            break;
        }
        
#ifdef TEL_CONN_MGR_ENABLE_CHECK_CEREG
        case TEL_CONN_MGR_AT_CMD_TYPE_CEREG_QUERY:
        {
            tel_conn_mgr_free(rsp_detail);
        }
#endif

        default:
            break;
    }
}


void tel_conn_mgr_bearer_at_cmd_rsp_ind_free(tel_conn_mgr_bearer_at_cmd_rsp_ind_struct *at_cmd_rsp_ind)
{
    if (!at_cmd_rsp_ind)
    {
        return;
    }

    tel_conn_mgr_bearer_rsp_detail_free(at_cmd_rsp_ind->at_cmd_type, at_cmd_rsp_ind->rsp_detail);

    tel_conn_mgr_free(at_cmd_rsp_ind);
}


tel_conn_mgr_bearer_at_cmd_rsp_ind_struct *tel_conn_mgr_bearer_gen_at_cmd_rsp_ind(ril_result_code_t code,
                                                                                               tel_conn_mgr_at_cmd_type_enum at_cmd_type,
                                                                                               void *user_data)
{
    int cid = user_data ? (int)user_data : TEL_CONN_MGR_MIN_CID - 1;
    tel_conn_mgr_bearer_at_cmd_rsp_ind_struct *at_cmd_rsp_ind = NULL;

    if (!tel_conn_mgr_is_cid_valid(cid))
    {
        return NULL;
    }

    at_cmd_rsp_ind = tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_bearer_at_cmd_rsp_ind_struct));
    //assert(at_cmd_rsp_ind);
    // TODO: Add at cmd sending try maybe
    if (!at_cmd_rsp_ind)
    {
        return NULL;
    }

    at_cmd_rsp_ind->msg_id = MSG_ID_TEL_CONN_MGR_BEARER_AT_CMD_RSP_IND;
    at_cmd_rsp_ind->cid = cid;
    at_cmd_rsp_ind->at_cmd_type = at_cmd_type;
    if (RIL_RESULT_CODE_OK == code || RIL_RESULT_CODE_CONNECT == code)
    {
        at_cmd_rsp_ind->result = TEL_CONN_MGR_TRUE;
    }
    else
    {
        at_cmd_rsp_ind->result = TEL_CONN_MGR_FALSE;
    }    

    return at_cmd_rsp_ind;
}


tel_conn_mgr_ret_enum tel_conn_mgr_bearer_general_at_cmd_rsp_callback(ril_result_code_t code,                                               
                                                                       tel_conn_mgr_at_cmd_type_enum at_cmd_type,
                                                                       void *user_data)
{
    tel_conn_mgr_bearer_at_cmd_rsp_ind_struct *at_cmd_rsp_ind = NULL;

    TEL_CONN_MGR_LOG_INFO("cmd_type:%d, result_code:%d, cid:%d", at_cmd_type, code, (int)user_data);

    at_cmd_rsp_ind = tel_conn_mgr_bearer_gen_at_cmd_rsp_ind(code,
                                                            at_cmd_type,
                                                            user_data);

    if (at_cmd_rsp_ind)
    {
        if (tel_conn_mgr_send_msg_to_tcm_task((tel_conn_mgr_msg_struct *)at_cmd_rsp_ind))
        {
            TEL_CONN_MGR_LOG_INFO("ret: %d", TEL_CONN_MGR_RET_OK);
            return TEL_CONN_MGR_RET_OK;
        }

        tel_conn_mgr_free(at_cmd_rsp_ind);
    }

    TEL_CONN_MGR_LOG_INFO("ret: %d", TEL_CONN_MGR_RET_ERROR);
    return TEL_CONN_MGR_RET_ERROR;
}


int32_t tel_conn_mgr_bearer_cgdcont_callback(ril_result_code_t code,
                                                        ril_request_mode_t mode,
                                                        char *payload,
                                                        uint32_t payload_len,
                                                        ril_define_pdp_context_rsp_t *param,
                                                        void *user_data)
{
    return tel_conn_mgr_bearer_general_at_cmd_rsp_callback(code,
                                                            TEL_CONN_MGR_AT_CMD_TYPE_CGDCONT,
                                                            user_data);
}


int32_t tel_conn_mgr_bearer_cgauth_callback(ril_result_code_t code,
                                                                  ril_request_mode_t mode,
                                                                 char *payload,
                                                                 uint32_t payload_len,
                                                                 ril_define_pdp_context_authentication_parameters_rsp_t *param,
                                                                 void *user_data)
{
    return tel_conn_mgr_bearer_general_at_cmd_rsp_callback(code,
                                                           TEL_CONN_MGR_AT_CMD_TYPE_CGAUTH,
                                                           user_data);
}


int32_t tel_conn_mgr_bearer_cgdata_callback(ril_result_code_t code,
                                                                  ril_request_mode_t mode,
                                                                 char *payload,
                                                                 uint32_t payload_len,
                                                                 void *param,
                                                                 void *user_data)
{
    return tel_conn_mgr_bearer_general_at_cmd_rsp_callback(code,
                                                           TEL_CONN_MGR_AT_CMD_TYPE_CGDATA,
                                                           user_data);
}


int32_t tel_conn_mgr_bearer_cereg_callback(ril_result_code_t code,
                                                     ril_request_mode_t mode,
                                                     char *payload,
                                                     uint32_t payload_len,
                                                     ril_eps_network_registration_status_rsp_t *param,
                                                     void* user_data)
{
    return tel_conn_mgr_bearer_general_at_cmd_rsp_callback(code,
                                                           TEL_CONN_MGR_AT_CMD_TYPE_CEREG,
                                                           user_data);
}


#ifdef TEL_CONN_MGR_ENABLE_CHECK_CEREG
int32_t tel_conn_mgr_bearer_cereg_query_callback(ril_result_code_t code,
                                                              ril_request_mode_t mode,
                                                              char *payload,
                                                              uint32_t payload_len,
                                                              ril_eps_network_registration_status_rsp_t *param,
                                                              void *user_data)
{
    tel_conn_mgr_nw_reg_status_struct *nw_reg_status = NULL;
    tel_conn_mgr_bearer_at_cmd_rsp_ind_struct *at_cmd_rsp_ind = NULL;

    if ((RIL_RESULT_CODE_OK == code && !param) || !user_data)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    TEL_CONN_MGR_LOG_INFO("cmd_type:%d, result_code:%d, cid:%d", 
                          TEL_CONN_MGR_AT_CMD_TYPE_CEREG_QUERY, 
                          code, 
                          (int)user_data);

    at_cmd_rsp_ind = tel_conn_mgr_bearer_gen_at_cmd_rsp_ind(code,
                                                            TEL_CONN_MGR_AT_CMD_TYPE_CEREG_QUERY,
                                                            user_data);

    if (at_cmd_rsp_ind)
    {
        if (at_cmd_rsp_ind->result)
        {
            TEL_CONN_MGR_LOG_INFO("nw status n:%d, act:%d, stat:%d", 
                                  (int)param->n, (int)param->act, (int)param->stat);
            
            nw_reg_status = tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_nw_reg_status_struct));
            if (!nw_reg_status)
            {
                at_cmd_rsp_ind->result = TEL_CONN_MGR_FALSE;
            }
            else
            {
                nw_reg_status->n = param->n;
                nw_reg_status->act = param->act;
                nw_reg_status->stat = param->stat;

                at_cmd_rsp_ind->rsp_detail = (void *)nw_reg_status;
            }
        }
        
        if (tel_conn_mgr_send_msg_to_tcm_task((tel_conn_mgr_msg_struct *)at_cmd_rsp_ind))
        {
            return TEL_CONN_MGR_RET_OK;
        }
        
        if (nw_reg_status)
        {
            tel_conn_mgr_free(nw_reg_status);
        }
        
        tel_conn_mgr_free(at_cmd_rsp_ind);
    }

    return TEL_CONN_MGR_RET_ERROR;
}
#endif


int32_t tel_conn_mgr_bearer_cgerep_callback(ril_result_code_t code,
                                                       ril_request_mode_t mode,
                                                       char *payload,
                                                       uint32_t payload_len,
                                                       ril_packet_domain_event_reporting_rsp_t *param,
                                                       void *user_data)
{
    return tel_conn_mgr_bearer_general_at_cmd_rsp_callback(code,
                                                           TEL_CONN_MGR_AT_CMD_TYPE_CGEREP,
                                                           user_data);
}


int32_t tel_conn_mgr_bearer_cfun_on_callback(ril_result_code_t code,
                                                       ril_request_mode_t mode,
                                                       char *payload,
                                                       uint32_t payload_len,
                                                       ril_packet_domain_event_reporting_rsp_t *param,
                                                       void *user_data)
{
    return tel_conn_mgr_bearer_general_at_cmd_rsp_callback(code,
                                                           TEL_CONN_MGR_AT_CMD_TYPE_CFUN_ON,
                                                           user_data);
}


/* Do not check if the format of the ip_netmask string is valid. */
tel_conn_mgr_bool tel_conn_mgr_bearer_parse_ip_netmask(char *ip_netmask,
                                                                      char *ip_buf,
                                                                      int ip_buf_size,
                                                                      char *netmask_buf,
                                                                      int netmask_buf_size,
                                                                      int *is_ipv6)
{
    int i = 0, count = 0;
    char *_4th_dot = NULL, *_16th_dot = NULL;
    char *ip_end = NULL, *netmask_start = NULL;

    if (!ip_netmask || !ip_buf || !netmask_buf || !is_ipv6)
    {
        return TEL_CONN_MGR_FALSE;
    }

    for (i = 0; i < strlen(ip_netmask); i++)
    {
        if ('.' == ip_netmask[i])
        {
            count++;
            if (4 == count)
            {
                _4th_dot = ip_netmask + i;
            }
            else if (16 == count)
            {
                _16th_dot = ip_netmask + i;
                break;
            }
        }
    }

    if (!_4th_dot)
    {
        return TEL_CONN_MGR_FALSE;
    }

    if (!_16th_dot)
    {
        /* IPv4 */
        if (TEL_CONN_MGR_IPV4_STR_MAX_LEN + 1 > ip_buf_size ||
            TEL_CONN_MGR_IPV4_STR_MAX_LEN + 1 > netmask_buf_size)
        {
            return TEL_CONN_MGR_FALSE;
        }
        ip_end = _4th_dot - 1;
        netmask_start = _4th_dot + 1;
        *is_ipv6 = 0;
    }
    else
    {
        /* IPv6 */
        if (TEL_CONN_MGR_IPV6_STR_MAX_LEN + 1 > ip_buf_size ||
            TEL_CONN_MGR_IPV6_STR_MAX_LEN + 1 > netmask_buf_size)
        {
            return TEL_CONN_MGR_FALSE;
        }
        ip_end = _16th_dot - 1;
        netmask_start = _16th_dot + 1;
        *is_ipv6 = 1;
    }

    if (TEL_CONN_MGR_IPV6_STR_MAX_LEN < ip_end - ip_netmask + 1 ||
        TEL_CONN_MGR_IPV6_STR_MAX_LEN < strlen(netmask_start))
    {
        return TEL_CONN_MGR_FALSE;
    }

    memset(ip_buf, 0, ip_buf_size);
    memset(netmask_buf, 0, netmask_buf_size);

    strncpy(ip_buf, ip_netmask, ip_end - ip_netmask + 1);
    strncpy(netmask_buf, netmask_start, strlen(netmask_start));

    return TEL_CONN_MGR_TRUE;
}


int32_t tel_conn_mgr_bearer_cgcontrdp_callback(ril_result_code_t code,
                                                           ril_request_mode_t mode,
                                                            char *payload,
                                                            uint32_t payload_len,
                                                            ril_pdp_context_read_dynamic_parameters_rsp_t *param,
                                                            void *user_data)
{
    tel_conn_mgr_bearer_at_cmd_rsp_ind_struct *at_cmd_rsp_ind = NULL;
    int cid = TEL_CONN_MGR_MIN_CID - 1, i = 0, is_ipv6 = 0;
    tel_conn_mgr_bearer_ip_info_struct *ip_info_header = NULL, *ip_info_new = NULL, *ip_info_curr = NULL;    
    tel_conn_mgr_bool ret = TEL_CONN_MGR_FALSE;
    
    if (( RIL_RESULT_CODE_OK == code && !param) || !user_data)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    cid = (int)user_data;

    if (RIL_RESULT_CODE_OK == code)
    {
        for (i = 0; i < param->array_num; i++)
        {
            if (cid == param->pdp_context[i].cid && param->pdp_context[i].local_addr_and_subnet_mask)
            {
                ip_info_new = tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_bearer_ip_info_struct));
                if (!ip_info_new)
                {
                    break;
                }
                
                ret = tel_conn_mgr_bearer_parse_ip_netmask(param->pdp_context[i].local_addr_and_subnet_mask,
                                                           ip_info_new->local_addr,
                                                           TEL_CONN_MGR_IPV6_STR_MAX_LEN + 1,
                                                           ip_info_new->subnet_mask,
                                                           TEL_CONN_MGR_IPV6_STR_MAX_LEN + 1,
                                                           &is_ipv6);
                if (!ret)
                {
                    tel_conn_mgr_free(ip_info_new);
                    ip_info_new = NULL;
                    continue;
                }

                ip_info_new->is_ipv6 = is_ipv6;

                if (param->pdp_context[i].gw_addr)
                {
                    strncpy(ip_info_new->gw_addr, param->pdp_context[i].gw_addr, TEL_CONN_MGR_IPV6_STR_MAX_LEN);
                }
                if (param->pdp_context[i].dns_prim_addr)
                {
                    strncpy(ip_info_new->dns_prim_addr, param->pdp_context[i].dns_prim_addr, TEL_CONN_MGR_IPV6_STR_MAX_LEN);
                }
                if (param->pdp_context[i].dns_sec_addr)
                {
                    strncpy(ip_info_new->dns_sec_addr, param->pdp_context[i].dns_sec_addr, TEL_CONN_MGR_IPV6_STR_MAX_LEN);
                }
                
                TEL_CONN_MGR_LOG_INFO("ip_info_new->ipv4_mtu:%d, nidd:%d", ip_info_new->ipv4_mtu, ip_info_new->nonip_mtu);
                ip_info_new->ipv4_mtu = param->pdp_context[i].ipv4_mtu;
                ip_info_new->nonip_mtu = param->pdp_context[i].nonip_mtu;

                if (NULL == ip_info_header)
                {
                    ip_info_header = ip_info_new;                    
                }
                else
                {
                    assert(ip_info_curr && !ip_info_curr->next);
                    ip_info_curr->next = ip_info_new;
                }

                ip_info_curr = ip_info_new;
            }
        }

        if (!ip_info_header)
        {
            TEL_CONN_MGR_LOG_ERR("cid:%d is not found in cgcontrdp rsp", cid);
            code = RIL_RESULT_CODE_ERROR;
        }
    }
    
    at_cmd_rsp_ind = tel_conn_mgr_bearer_gen_at_cmd_rsp_ind(code,
                                                            TEL_CONN_MGR_AT_CMD_TYPE_CGCONTRDP,
                                                            user_data);
    if (at_cmd_rsp_ind)
    {
        if (at_cmd_rsp_ind->result)
        {
            if (ip_info_header)
            {
                at_cmd_rsp_ind->rsp_detail = (void *)ip_info_header;
            }
            else
            {
                at_cmd_rsp_ind->result = TEL_CONN_MGR_FALSE;
            }
        }
        
        if (tel_conn_mgr_send_msg_to_tcm_task((tel_conn_mgr_msg_struct *)at_cmd_rsp_ind))
        {
            return TEL_CONN_MGR_RET_OK;
        }

        tel_conn_mgr_free(at_cmd_rsp_ind);
    }

    tel_conn_mgr_bearer_ip_info_free(ip_info_header);

    return TEL_CONN_MGR_RET_ERROR;

}


int32_t tel_conn_mgr_bearer_cgact_query_callback(ril_result_code_t code,
                                                             ril_request_mode_t mode,
                                                             char *payload,
                                                             uint32_t payload_len,
                                                             ril_pdp_context_activate_or_deactivate_rsp_t *param,
                                                             void *user_data)
{
    tel_conn_mgr_bearer_at_cmd_rsp_ind_struct *at_cmd_rsp_ind = NULL;
    int cid = TEL_CONN_MGR_MIN_CID - 1, i = 0;

    if ((RIL_RESULT_CODE_OK == code && !param) || !user_data)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    cid = (int)user_data;

    if (RIL_RESULT_CODE_OK == code)
    {
        for (i = 0; i < param->array_num; i++)
        {
            if (cid == param->cid_state[i].cid)
            {
                break;
            }
        }

        if (i >= param->array_num)
        {
            TEL_CONN_MGR_LOG_ERR("cid:%d is not found in cgact query rsp", cid);
        }
    }

    TEL_CONN_MGR_LOG_INFO("cid:%d, result_code:%d", cid, code);
    
    at_cmd_rsp_ind = tel_conn_mgr_bearer_gen_at_cmd_rsp_ind(code,
                                                            TEL_CONN_MGR_AT_CMD_TYPE_CGACT_QUERY,
                                                            user_data);

    if (at_cmd_rsp_ind)
    {
        if (RIL_RESULT_CODE_OK == code)
        {
            if (i >= param->array_num)
            {
                at_cmd_rsp_ind->rsp_detail = (void *)TEL_CONN_MGR_FALSE;
            }
            else
            {
                at_cmd_rsp_ind->rsp_detail = (void *)(param->cid_state[i].state ? TEL_CONN_MGR_TRUE : TEL_CONN_MGR_FALSE);
            }
        }

        TEL_CONN_MGR_LOG_INFO("rsp_detail:%x", (unsigned int)at_cmd_rsp_ind->rsp_detail);
        
        if (tel_conn_mgr_send_msg_to_tcm_task((tel_conn_mgr_msg_struct *)at_cmd_rsp_ind))
        {
            return TEL_CONN_MGR_RET_OK;
        }

        tel_conn_mgr_free(at_cmd_rsp_ind);
    }

    return TEL_CONN_MGR_RET_ERROR;

}


int32_t tel_conn_mgr_bearer_cgact_deact_callback(ril_result_code_t code,
                                                                   ril_request_mode_t mode,
                                                                   char *payload,
                                                                   uint32_t payload_len,
                                                                   ril_pdp_context_activate_or_deactivate_rsp_t *param,
                                                                   void *user_data)
{
    return tel_conn_mgr_bearer_general_at_cmd_rsp_callback(code,
                                                           TEL_CONN_MGR_AT_CMD_TYPE_CGACT_DEACT,
                                                           user_data);
}


int32_t tel_conn_mgr_bearer_at_cmd_rsp_callback(ril_cmd_response_t *response)
{
    if (!response)
    {
        return -1;
    }

    switch (response->cmd_id)
    {
        case RIL_CMD_ID_CEREG:
        {
            if (RIL_EXECUTE_MODE == response->mode)
            {
                tel_conn_mgr_bearer_cereg_callback(response->res_code,
                                                   response->mode,
                                                   NULL,
                                                   0,
                                                   (ril_eps_network_registration_status_rsp_t *)response->cmd_param,
                                                   response->user_data);
            }
            else if (RIL_READ_MODE == response->mode)
            {
#ifdef TEL_CONN_MGR_ENABLE_CHECK_CEREG
                tel_conn_mgr_bearer_cereg_query_callback(response->res_code,
                                                         response->mode,
                                                         NULL,
                                                         0,
                                                         (ril_eps_network_registration_status_rsp_t *)response->cmd_param,
                                                         response->user_data);
#endif
            }
            break;
        }

        case RIL_CMD_ID_CGEREP:
        {
            tel_conn_mgr_bearer_cgerep_callback(response->res_code,
                                                response->mode,
                                                NULL,
                                                0,
                                                (ril_packet_domain_event_reporting_rsp_t *)response->cmd_param,
                                                response->user_data);
            break;
        }

        case RIL_CMD_ID_CGDCONT:
        {            
            tel_conn_mgr_bearer_cgdcont_callback(response->res_code,
                                                 response->mode,
                                                 NULL,
                                                 0,
                                                 (ril_define_pdp_context_rsp_t *)response->cmd_param,
                                                 response->user_data);
            break;
        }

        case RIL_CMD_ID_CGAUTH:
        {
            tel_conn_mgr_bearer_cgauth_callback(response->res_code,
                                                response->mode,
                                                NULL,
                                                0,
                                                (ril_define_pdp_context_authentication_parameters_rsp_t *)response->cmd_param,
                                                response->user_data);
            break;
        }

        case RIL_CMD_ID_CGDATA:
        {
            tel_conn_mgr_bearer_cgdata_callback(response->res_code,
                                                response->mode,
                                                NULL,
                                                0,
                                                response->cmd_param,
                                                response->user_data);
            break;
        }

        case RIL_CMD_ID_CGCONTRDP:
        {
            tel_conn_mgr_bearer_cgcontrdp_callback(response->res_code,
                                                   response->mode,
                                                   NULL,
                                                   0,
                                                   (ril_pdp_context_read_dynamic_parameters_rsp_t *)response->cmd_param,
                                                   response->user_data);
            break;
        }

        case RIL_CMD_ID_CGACT:
        {
            if (RIL_EXECUTE_MODE == response->mode)
            {
                tel_conn_mgr_bearer_cgact_deact_callback(response->res_code,
                                                         response->mode,
                                                         NULL,
                                                         0,
                                                         (ril_pdp_context_activate_or_deactivate_rsp_t *)response->cmd_param,
                                                         response->user_data);
            }
            else if (RIL_READ_MODE == response->mode)
            {
                tel_conn_mgr_bearer_cgact_query_callback(response->res_code,
                                                         response->mode,
                                                         NULL,
                                                         0,
                                                         (ril_pdp_context_activate_or_deactivate_rsp_t *)response->cmd_param,
                                                         response->user_data);
            }
            break;
        }

        default:
            return -2;
    }

    return 0;
}


tel_conn_mgr_ret_enum tel_conn_mgr_general_send_at_cmd(tel_conn_mgr_at_cmd_type_enum at_cmd_type,                                                                  
                                                                  int cid)
{
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    tel_conn_mgr_bearer_info_struct *bearer_info = NULL;
    tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep = NULL;
    tel_conn_mgr_bool send_on_data_channel = TEL_CONN_MGR_FALSE;
    
    if (!bearer_state || !at_cmd_flow_helper || TEL_CONN_MGR_AT_CMD_TYPE_NONE == at_cmd_type)
    {
        TEL_CONN_MGR_LOG_ERR("%d", TEL_CONN_MGR_RET_INVALID_PARAM);
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    if (TEL_CONN_MGR_AT_CMD_TYPE_NONE != at_cmd_flow_helper->current_at_cmd_type ||
        !tel_conn_mgr_bearer_cntx ||
        TEL_CONN_MGR_MODEM_STATE_READY != tel_conn_mgr_bearer_cntx->modem_state)
    {
        TEL_CONN_MGR_LOG_ERR("%d, cur_at_cmd_type:%d", TEL_CONN_MGR_RET_WRONG_STATE, at_cmd_flow_helper->current_at_cmd_type);
        return TEL_CONN_MGR_RET_WRONG_STATE;
    }
    
    send_on_data_channel = tel_conn_mgr_is_data_channel_at_cmd_type(at_cmd_type);
    TEL_CONN_MGR_LOG_INFO("at_cmd_type:%d, send_on_data_channel:%d", at_cmd_type, send_on_data_channel);
    
    if (send_on_data_channel)
    {
        bearer_info = tel_conn_mgr_bearer_info_find_by_cid(cid);
        ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_cid(cid);
        if (!bearer_info || !ds_keep)
        {
            return TEL_CONN_MGR_RET_ERROR;
        }

        if (TEL_CONN_MGR_MIN_VALID_CHANNEL_ID > ds_keep->data_channel_id)
        {
#ifdef TEL_CONN_MGR_UT
            ds_keep->data_channel_id = TEL_CONN_MGR_MIN_VALID_CHANNEL_ID;

            if (2 == cid)
            {
                ds_keep->data_channel_id = TEL_CONN_MGR_MIN_VALID_CHANNEL_ID + 1;
            }
#else
            ds_keep->data_channel_id = ril_get_free_data_channel();

            if (TEL_CONN_MGR_MIN_VALID_CHANNEL_ID > ds_keep->data_channel_id ||
                RIL_STATUS_SUCCESS != ril_set_data_channel_reserved(ds_keep->data_channel_id, TEL_CONN_MGR_TRUE))
            {
                return TEL_CONN_MGR_RET_ERROR;
            }
#endif /* TEL_CONN_MGR_UT */
            TEL_CONN_MGR_LOG_INFO("new data chn cid:%d, data_channel_id:%d", cid, ds_keep->data_channel_id );
        }

        TEL_CONN_MGR_LOG_INFO("cid:%d, data_channel_id:%d", cid, ds_keep->data_channel_id );
        
        switch (at_cmd_type)
        {
            case TEL_CONN_MGR_AT_CMD_TYPE_CGDCONT:
            {
                ril_pdp_context_entity_t req = {0};

                req.cid = cid;
                req.pdp_type = tel_conn_mgr_pdp_type_cov2str(ds_keep->pdp_type);
                req.apn = ds_keep->apn;
                
                TEL_CONN_MGR_LOG_INFO("[ATCMD]AT+CGDCONT=%d,\"%s\",\"%s\"", cid, req.pdp_type, req.apn);

#ifdef TEL_CONN_MGR_UT
                ret = ril_request_define_pdp_context_ut(RIL_EXECUTE_MODE, 
                                               &req,
                                               tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                               (void *)cid,
                                               ds_keep->data_channel_id);
                /*tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                                                           3,
                                                                                                           0,
                                                                                                           TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                                                           NULL);*/

#else
                ret = ril_request_define_pdp_context(RIL_EXECUTE_MODE, 
                                               &req,
                                               tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                               (void *)cid,
                                               ds_keep->data_channel_id);
#endif

                break;
            }

            case TEL_CONN_MGR_AT_CMD_TYPE_CGAUTH:
            {
                TEL_CONN_MGR_LOG_INFO("[ATCMD]AT+CGAUTH=%d,1,\"%s\",\"%s\"", cid, bearer_info->username, bearer_info->password);
#ifdef TEL_CONN_MGR_UT
                ret = ril_request_define_pdp_context_authentication_parameters_ut(RIL_EXECUTE_MODE,
                                                         cid,
                                                         RIL_OMITTED_INTEGER_PARAM,
                                                         bearer_info->username,
                                                         bearer_info->password,
                                                         tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                                         (void *)cid,
                                                         ds_keep->data_channel_id);
                /*tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                                                                           3,
                                                                                                           0,
                                                                                                           TEL_CONN_MGR_PDP_TYPE_NONE,
                                                                                                           NULL);*/
                //if (17 == current_test_case_num)
                if (0)
                {
                    tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_DEACT_REQ,
                                                              17,
                                                              0,
                                                              TEL_CONN_MGR_PDP_TYPE_NONE,
                                                              NULL);

                    vTaskDelay(50);
                    tel_conn_mgr_ut_app_send_msg_to_app_task1(MSG_ID_TEL_CONN_MGR_UT_APP_ACT_REQ,
                                                              17,
                                                              -1,
                                                              TEL_CONN_MGR_PDP_TYPE_IPV6,
                                                              "app_task1_apn1");
                }
#else
#ifdef TEL_CONN_MGR_SKIP_CGAUTH_WHEN_NO_AUTH
                // If username is empty, CGAUTH is skipped.
                assert(bearer_info->username[0]);
#else
                if (!bearer_info->username[0])
                {
                    ret = ril_request_define_pdp_context_authentication_parameters(RIL_EXECUTE_MODE,
                                                                                   cid,
                                                                                   0,
                                                                                   RIL_OMITTED_STRING_PARAM,
                                                                                   RIL_OMITTED_STRING_PARAM,                                                                               
                                                                                   tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                                                                   (void *)cid,
                                                                                   ds_keep->data_channel_id);
                    
                }
                else
#endif
                {
                    ret = ril_request_define_pdp_context_authentication_parameters(RIL_EXECUTE_MODE,
                                                                                   cid,
                                                                                   1,
                                                                                   bearer_info->username,
                                                                                   bearer_info->password[0] ? bearer_info->password : RIL_OMITTED_STRING_PARAM,                                                                               
                                                                                   tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                                                                   (void *)cid,
                                                                                   ds_keep->data_channel_id);
                }
#endif
                break;
            }

            case TEL_CONN_MGR_AT_CMD_TYPE_CGDATA:
            {
                ril_enter_data_state_req_t req = {0};
                req.l2p = "M-PT";
                req.cid_array = (int32_t *)&cid;
                req.cid_array_len = 1;                
                
                TEL_CONN_MGR_LOG_INFO("[ATCMD]AT+CGDATA=\"%s\",%d", req.l2p, cid);
#ifdef TEL_CONN_MGR_UT
                ret = ril_request_enter_data_state_ut(RIL_EXECUTE_MODE,
                                             &req,
                                             tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                             (void *)cid,
                                             ds_keep->data_channel_id);
#else
                ret = ril_request_enter_data_state(RIL_EXECUTE_MODE,
                                             &req,
                                             tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                             (void *)cid,
                                             ds_keep->data_channel_id);
#endif
                
                if (RIL_STATUS_SUCCESS == (ril_status_t)ret)
                {
                    (*bearer_state) |= TEL_CONN_MGR_BEARER_STATE_ACTIVATING_WAIT_ACT_URC;
                }
                
                break;
            }

            default:
                break;
        }

    }
    else
    {
        switch (at_cmd_type)
        {
            case TEL_CONN_MGR_AT_CMD_TYPE_CEREG:
            {
                TEL_CONN_MGR_LOG_INFO("[ATCMD]AT+CEREG=2");
#ifdef TEL_CONN_MGR_UT
                ret = ril_request_eps_network_registration_status_ut(RIL_EXECUTE_MODE,
                                                                     2,
                                                                     tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                                                     (void *)cid);
#else
                ret = ril_request_eps_network_registration_status(RIL_EXECUTE_MODE,
                                                                  2,
                                                                  tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                                                  (void *)cid);
#endif
                break;
            }

#ifdef TEL_CONN_MGR_ENABLE_CHECK_CEREG
            case TEL_CONN_MGR_AT_CMD_TYPE_CEREG_QUERY:
            {
                TEL_CONN_MGR_LOG_INFO("[ATCMD]AT+CEREG?");
#ifdef TEL_CONN_MGR_UT
                ret = ril_request_eps_network_registration_status_ut(RIL_READ_MODE,
                                                                     RIL_OMITTED_INTEGER_PARAM,
                                                                     tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                                                     (void *)cid);
#else
                ret = ril_request_eps_network_registration_status(RIL_READ_MODE,
                                                                  RIL_OMITTED_INTEGER_PARAM,
                                                                  tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                                                  (void *)cid);
#endif
                break;
            }
#endif

            case TEL_CONN_MGR_AT_CMD_TYPE_CGEREP:
            {
                TEL_CONN_MGR_LOG_INFO("[ATCMD]AT+CGEREP=1");
#ifdef TEL_CONN_MGR_UT
                ret = ril_request_packet_domain_event_reporting_ut(RIL_EXECUTE_MODE,
                                                                   1,
                                                                   RIL_OMITTED_INTEGER_PARAM,
                                                                   tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                                                   (void *)cid);
#else
                ret = ril_request_packet_domain_event_reporting(RIL_EXECUTE_MODE,
                                                                1,
                                                                RIL_OMITTED_INTEGER_PARAM,
                                                                tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                                                (void *)cid);
#endif
                break;
            }

            case TEL_CONN_MGR_AT_CMD_TYPE_CFUN_ON:
            {
                TEL_CONN_MGR_LOG_INFO("[ATCMD]AT+CFUN=1");
#ifdef TEL_CONN_MGR_UT
                ret = ril_request_set_phone_functionality_ut(RIL_EXECUTE_MODE,
                                                             1,
                                                             RIL_OMITTED_INTEGER_PARAM,
                                                             tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                                             (void *)cid);
#else
                ret = ril_request_set_phone_functionality(RIL_EXECUTE_MODE,
                                                          1,
                                                          RIL_OMITTED_INTEGER_PARAM,
                                                          tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                                          (void *)cid);
                
#endif
                break;
            }

            case TEL_CONN_MGR_AT_CMD_TYPE_CGACT_QUERY:
            {
                TEL_CONN_MGR_LOG_INFO("[ATCMD]AT+CGACT?");
#ifdef TEL_CONN_MGR_UT
                ret = ril_request_pdp_context_activate_or_deactivate_ut(RIL_READ_MODE,
                                                                     NULL,
                                                                     tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                                                     (void *)cid);
#else
                ret = ril_request_pdp_context_activate_or_deactivate(RIL_READ_MODE,
                                                                     NULL,
                                                                     tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                                                     (void *)cid);
#endif
                break;
            }

            case TEL_CONN_MGR_AT_CMD_TYPE_CGCONTRDP:
            {
                TEL_CONN_MGR_LOG_INFO("[ATCMD]AT+CGCONTRDP=%d", cid);
#ifdef TEL_CONN_MGR_UT
                ret = ril_request_pdp_context_read_dynamic_parameters_ut(RIL_EXECUTE_MODE,
                                                                      cid,
                                                                      tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                                                      (void *)cid);
#else
                ret = ril_request_pdp_context_read_dynamic_parameters(RIL_EXECUTE_MODE,
                                                                      cid,
                                                                      tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                                                      (void *)cid);
#endif
                break;
            }

            case TEL_CONN_MGR_AT_CMD_TYPE_CGACT_DEACT:
            {
                ril_pdp_context_activate_or_deactivate_req_t req = {0};
                req.state = 0;
                req.cid_array = (int32_t *)&cid;
                req.cid_array_len = 1;

                TEL_CONN_MGR_LOG_INFO("[ATCMD]AT+CGACT=0, %d", cid);
#ifdef TEL_CONN_MGR_UT
                ret = ril_request_pdp_context_activate_or_deactivate_ut(RIL_EXECUTE_MODE,
                                                                     &req,
                                                                     tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                                                     (void *)cid);
#else
                ret = ril_request_pdp_context_activate_or_deactivate(RIL_EXECUTE_MODE,
                                                                     &req,
                                                                     tel_conn_mgr_bearer_at_cmd_rsp_callback,
                                                                     (void *)cid);
#endif
                break;
            }

            default:
                break;
        }
        
        if (TEL_CONN_MGR_RET_OK != ret)
        {
            //at_cmd_flow_helper->ctrl_channel_id = TEL_CONN_MGR_MIN_VALID_CHANNEL_ID - 1;
        }
        else
        {
            //at_cmd_flow_helper->ctrl_channel_id = x;
            
            if (TEL_CONN_MGR_AT_CMD_TYPE_CGACT_DEACT == at_cmd_type)
            {
                (*bearer_state) |= TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_WAIT_DEACT_URC;
            }
            else if (TEL_CONN_MGR_AT_CMD_TYPE_CGACT_QUERY == at_cmd_type)
            {
                /* If bearer state changes after modem sends rsp for at+cgact?, urc is supposed to be received after 
                            * rsp. However, the lower level does not guarantee this. Waiting for URC flag is set in case that urc arrives
                            * before rsp.
                            */
                if (TEL_CONN_MGR_BEARER_STATE_ACTIVATING & (*bearer_state))
                {
                    (*bearer_state) |= TEL_CONN_MGR_BEARER_STATE_ACTIVATING_WAIT_ACT_URC;
                }
                else if (TEL_CONN_MGR_BEARER_STATE_DEACTIVATING & (*bearer_state))
                {
                    (*bearer_state) |= TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_WAIT_DEACT_URC;
                }
            }
        }
    }

    if (TEL_CONN_MGR_RET_OK == ret)
    {
        at_cmd_flow_helper->current_at_cmd_type = at_cmd_type;
    }
    else
    {
        TEL_CONN_MGR_LOG_INFO("[Error]Failed to send AT CMD.");
    }

    return ret;
}


/* If there is at cmd waiting for response, add new at cmd into list and return.
 * Otherwise, send the first at cmd in waiting list if there's one. If there's no at cmd in list, send the new at cmd.
 * If the new at cmd has been sent successfully, return. In other situation, add the new at cmd into list.
 * RETURN: TEL_CONN_MGR_RET_OK: The new at cmd has been sent successfully.
 *              TEL_CONN_MGR_RET_WOULDBLOCK: The new at cmd has not been sent successfully.
 */
static tel_conn_mgr_ret_enum tel_conn_mgr_list_send_at_cmd(tel_conn_mgr_at_cmd_class_enum at_cmd_class,
                                                                   tel_conn_mgr_at_cmd_type_enum at_cmd_type,
                                                                   int cid)
{
    int ret = 0;
    tel_conn_mgr_at_cmd_list_struct *list_node = NULL;
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);

    TEL_CONN_MGR_LOG_INFO("class_type: %d, cmd_type:%d, cid:%d", at_cmd_class, at_cmd_type, cid);
    if (!at_cmd_flow_helper)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    /* If there's AT CMD in the at_cmd_to_send_list, send it. */
    tel_conn_mgr_send_next_at_cmd(cid);

    ret = tel_conn_mgr_general_send_at_cmd(at_cmd_type, cid);

    list_node = tel_conn_mgr_at_cmd_list_node_create(at_cmd_class, at_cmd_type, cid);

    /* Insert new AT CMD into list. */
    if (TEL_CONN_MGR_RET_OK != ret)
    {
        tel_conn_mgr_list_insert((tel_conn_mgr_template_list_struct **)&at_cmd_flow_helper->at_cmd_to_send_list, 
                               (tel_conn_mgr_template_list_struct *)list_node);
        ret = TEL_CONN_MGR_RET_WOULDBLOCK;
    }
    else
    {
        tel_conn_mgr_list_insert((tel_conn_mgr_template_list_struct **)&at_cmd_flow_helper->at_cmd_to_recv_list, 
                               (tel_conn_mgr_template_list_struct *)list_node);
        ret = TEL_CONN_MGR_RET_OK;
    }

    return (tel_conn_mgr_ret_enum)ret;
}

#if 0
/**
  * If AT CMD type is the same, the basic string for the AT CMD is the same.
  */
static char *tel_conn_mgr_get_basic_at_cmd_str(tel_conn_mgr_at_cmd_type_enum at_cmd_type)
{
    int i = -1;

    while (tel_conn_mgr_at_cmd_info[++i].at_cmd_str)
    {
        if (at_cmd_type == tel_conn_mgr_at_cmd_info[i].cmd_type)
        {
            return tel_conn_mgr_at_cmd_info[i].at_cmd_str;
        }
    }

    return NULL;
}


/**
  * If AT CMD type is the same, the string for the AT CMD is the same.
  */
tel_conn_mgr_ret_enum tel_conn_mgr_get_at_cmd_str(unsigned char *buf,
                                                  unsigned int buf_size,
                                                  tel_conn_mgr_at_cmd_type_enum at_cmd_type,
                                                  int cid)
{
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_OK;
    int ret_val = 0;
    
    if (!buf || !buf_size || !tel_conn_mgr_is_cid_valid(cid) ||
        !(TEL_CONN_MGR_AT_CMD_TYPE_NONE < at_cmd_type &&
          TEL_CONN_MGR_AT_CMD_TYPE_MAX > at_cmd_type))
    {
        TEL_CONN_MGR_LOG_INFO("%s, Invalid param", __FUNCTION__);
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    memset(buf, 0, buf_size);
    
    switch (at_cmd_type)
    {
        case TEL_CONN_MGR_AT_CMD_TYPE_CGACT_DEACT:
        case TEL_CONN_MGR_AT_CMD_TYPE_CGDATA:
        case TEL_CONN_MGR_AT_CMD_TYPE_CGCONTRDP:
        {
            ret_val = snprintf((char*)buf, (buf_size - 1), tel_conn_mgr_get_basic_at_cmd_str(at_cmd_type), cid);
            break;
        }

        case TEL_CONN_MGR_AT_CMD_TYPE_CGDCONT:
        {
            tel_conn_mgr_bearer_info_struct *bearer_info = NULL;
            char *pdp_type_str = NULL;
            
            bearer_info = tel_conn_mgr_bearer_info_find_by_cid(cid);
            if (bearer_info)
            {
                pdp_type_str = tel_conn_mgr_pdp_type_cov2str(bearer_info->pdp_type);
            }

            if (!pdp_type_str)
            {
                pdp_type_str = TEL_CONN_MGR_DEFAULT_PDP_TYPE;
            }
            
            ret_val = snprintf((char*)buf, (buf_size - 1), 
                                tel_conn_mgr_get_basic_at_cmd_str(at_cmd_type), 
                                cid,
                                pdp_type_str,
                                (bearer_info && strlen((const char *)bearer_info->apn)) ? bearer_info->apn : TEL_CONN_MGR_DEFAULT_APN);
            break;
        }

        case TEL_CONN_MGR_AT_CMD_TYPE_CGAUTH:
        {
            tel_conn_mgr_bearer_info_struct *bearer_info = NULL;
            
            bearer_info = tel_conn_mgr_bearer_info_find_by_cid(cid);
            if (!bearer_info)
            {
                ret = TEL_CONN_MGR_RET_NOT_FOUND;
                goto exit;
            }

            ret_val = snprintf((char*)buf, (buf_size - 1), 
                                tel_conn_mgr_get_basic_at_cmd_str(at_cmd_type), 
                                cid,
                                bearer_info->username,
                                bearer_info->password);
            
            break;
        }

        case TEL_CONN_MGR_AT_CMD_TYPE_CMEE:
        case TEL_CONN_MGR_AT_CMD_TYPE_ESLP_DISABLE:
        case TEL_CONN_MGR_AT_CMD_TYPE_EIND:
        case TEL_CONN_MGR_AT_CMD_TYPE_CFUN_ON:
        case TEL_CONN_MGR_AT_CMD_TYPE_ERAT:
        case TEL_CONN_MGR_AT_CMD_TYPE_CGEREP:
        case TEL_CONN_MGR_AT_CMD_TYPE_CTZR_ON:
        case TEL_CONN_MGR_AT_CMD_TYPE_CFUN_OFF:
        case TEL_CONN_MGR_AT_CMD_TYPE_CGREG:
        case TEL_CONN_MGR_AT_CMD_TYPE_CGREG_QUERY:
        case TEL_CONN_MGR_AT_CMD_TYPE_CEREG:
        case TEL_CONN_MGR_AT_CMD_TYPE_CEREG_QUERY:
        case TEL_CONN_MGR_AT_CMD_TYPE_CGACT_QUERY:
        case TEL_CONN_MGR_AT_CMD_TYPE_ATE0:
        {
            ret_val = snprintf((char*)buf, (buf_size - 1), tel_conn_mgr_get_basic_at_cmd_str(at_cmd_type));
            break;
        }

        default:
        {
            ret = TEL_CONN_MGR_RET_NOT_FOUND;
            break;
        }
    }

exit:
    if (TEL_CONN_MGR_RET_OK == ret)
    {
        if (ret_val > (buf_size - 1))
        {
            memset(buf, 0, buf_size);
            ret = TEL_CONN_MGR_RET_NOT_ENOUGH_MEMORY;
        }
    }
    else
    {
        TEL_CONN_MGR_LOG_INFO("%s. Failed. ret:%d", __FUNCTION__, ret);
    }

    return ret;
}
#endif


tel_conn_mgr_ret_enum tel_conn_mgr_send_next_at_cmd(int cid)
{
    int ret = 0;
    tel_conn_mgr_at_cmd_list_struct *at_cmd_list_node = NULL;
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);

    if (!at_cmd_flow_helper)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    at_cmd_list_node = at_cmd_flow_helper->at_cmd_to_send_list;    

    if (!at_cmd_list_node)
    {
        return TEL_CONN_MGR_RET_WRONG_STATE;
    }

    TEL_CONN_MGR_LOG_INFO("class_type: %d, cmd_type:%d, cid:%d", 
                            at_cmd_flow_helper->at_cmd_to_send_list->at_cmd_class,
                            at_cmd_flow_helper->at_cmd_to_send_list->at_cmd_type,
                            cid);

    ret = tel_conn_mgr_general_send_at_cmd(at_cmd_list_node->at_cmd_type, cid);        

    if (TEL_CONN_MGR_RET_OK == ret)
    {
        tel_conn_mgr_list_remove((tel_conn_mgr_template_list_struct **)&at_cmd_flow_helper->at_cmd_to_send_list, 
                               (tel_conn_mgr_template_list_struct *)at_cmd_list_node);        
        tel_conn_mgr_list_insert((tel_conn_mgr_template_list_struct **)&at_cmd_flow_helper->at_cmd_to_recv_list, 
                               (tel_conn_mgr_template_list_struct *)at_cmd_list_node);
    }

    return (tel_conn_mgr_ret_enum)ret;
}


tel_conn_mgr_ret_enum tel_conn_mgr_send_at_cmd(tel_conn_mgr_at_cmd_class_enum at_cmd_class,
                                                        tel_conn_mgr_at_cmd_type_enum at_cmd_type,
                                                        int cid)
{
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_OK;

    TEL_CONN_MGR_LOG_INFO("%s, cid:%d, class:%d, type:%d\r\n", __FUNCTION__,
                          cid, at_cmd_class, at_cmd_type);

    if (!tel_conn_mgr_is_cid_valid(cid) ||
        !(TEL_CONN_MGR_AT_CMD_TYPE_NONE < at_cmd_type &&
          TEL_CONN_MGR_AT_CMD_TYPE_MAX > at_cmd_type))
    {
        TEL_CONN_MGR_LOG_INFO("%s, Invalid param", __FUNCTION__);
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    if (!tel_conn_mgr_bearer_cntx || tel_conn_mgr_bearer_cntx->modem_state != TEL_CONN_MGR_MODEM_STATE_READY)
    {
        return TEL_CONN_MGR_RET_WRONG_STATE;
    }

    /* data is inserted into list, or freed. */
    ret = tel_conn_mgr_list_send_at_cmd(at_cmd_class, at_cmd_type, cid);
    
    return ret;
}


/**
  * @brief     Get next AT CMD by class
  * @param[in] tel_conn_mgr_at_cmd_class_enum cmd_class: AT CMD class
  * @param[in] tel_conn_mgr_at_cmd_type_enum cmd_type: AT CMD type
  * @retval    tel_conn_mgr_at_cmd_type_enum
  */
tel_conn_mgr_at_cmd_type_enum tel_conn_mgr_get_next_at_cmd_by_class(tel_conn_mgr_at_cmd_class_enum cmd_class,
                                                                                 tel_conn_mgr_at_cmd_type_enum cmd_type)
{
    int i = -1;

    while (TEL_CONN_MGR_AT_CMD_TYPE_NONE != tel_conn_mgr_at_cmd_info[++i].cmd_type)
    {
        if (cmd_type == tel_conn_mgr_at_cmd_info[i].cmd_type &&
            cmd_class == tel_conn_mgr_at_cmd_info[i].cmd_class)
        {
            if (cmd_class == tel_conn_mgr_at_cmd_info[i + 1].cmd_class)
            {
                return tel_conn_mgr_at_cmd_info[i + 1].cmd_type;
            }

            break;
        }
    }

    return TEL_CONN_MGR_AT_CMD_TYPE_NONE;
}


tel_conn_mgr_bool tel_conn_mgr_is_at_cmd_in_class_group(tel_conn_mgr_at_cmd_type_enum at_cmd_type,
                                                       tel_conn_mgr_at_cmd_class_enum at_cmd_class)
{
    int i = -1;
    
    TEL_CONN_MGR_LOG_INFO("%s", __FUNCTION__);
    
    while (TEL_CONN_MGR_AT_CMD_TYPE_NONE != tel_conn_mgr_at_cmd_info[++i].cmd_type)
    {
        if (at_cmd_type == tel_conn_mgr_at_cmd_info[i].cmd_type &&
            at_cmd_class == tel_conn_mgr_at_cmd_info[i].cmd_class)
        {
            TEL_CONN_MGR_LOG_INFO("%s, TEL_CONN_MGR_TRUE", __FUNCTION__);
            return TEL_CONN_MGR_TRUE;
        }
    }

    TEL_CONN_MGR_LOG_INFO("%s, TEL_CONN_MGR_FALSE", __FUNCTION__);
    return TEL_CONN_MGR_FALSE;
}


/**
  * @brief     Get first AT CMD by class
  * @param[in] tel_conn_mgr_at_cmd_class_enum cmd_class: AT CMD class
  * @retval    tel_conn_mgr_at_cmd_type_enum
  */
tel_conn_mgr_at_cmd_type_enum tel_conn_mgr_get_1st_cmd_for_class(int cid,
                                                                            tel_conn_mgr_at_cmd_class_enum cmd_class,
                                                                            tel_conn_mgr_at_cmd_type_enum specified_at_cmd)
{
    int i = -1;
    tel_conn_mgr_at_cmd_type_enum at_cmd_type = TEL_CONN_MGR_AT_CMD_TYPE_NONE;    
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);

    if (!at_cmd_flow_helper)
    {
        return TEL_CONN_MGR_AT_CMD_TYPE_NONE;
    }

    if (TEL_CONN_MGR_AT_CMD_TYPE_NONE != specified_at_cmd)
    {
        if (!tel_conn_mgr_is_at_cmd_in_class_group(specified_at_cmd, cmd_class))
        {
            return TEL_CONN_MGR_AT_CMD_TYPE_NONE;
        }

        at_cmd_type = specified_at_cmd;
    }
    else
    {
        while (TEL_CONN_MGR_AT_CMD_TYPE_NONE != tel_conn_mgr_at_cmd_info[++i].cmd_type)
        {
            if (cmd_class == tel_conn_mgr_at_cmd_info[i].cmd_class)
            {
                TEL_CONN_MGR_LOG_INFO("%s, Found. cmd_class:%d, cmd_type:%d", __FUNCTION__, cmd_class, tel_conn_mgr_at_cmd_info[i].cmd_type);
                at_cmd_type = tel_conn_mgr_at_cmd_info[i].cmd_type;
                break;
            }
        }
    }

    if (TEL_CONN_MGR_AT_CMD_TYPE_NONE == at_cmd_type)
    {
        TEL_CONN_MGR_LOG_INFO("%s, Not Found. cmd_class:%d", __FUNCTION__, cmd_class);
        return TEL_CONN_MGR_AT_CMD_TYPE_NONE; 
    }

    if (TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE == cmd_class &&
        TEL_CONN_MGR_AT_CMD_TYPE_CGACT_QUERY == at_cmd_type)
    {
        if (TEL_CONN_MGR_KEY_AT_CMD_TYPE_DEACT == at_cmd_flow_helper->last_key_at_cmd_type)
        {
            /* Skip AT+CGACT? */
            at_cmd_type = tel_conn_mgr_get_next_at_cmd_by_class(cmd_class,
                                                                TEL_CONN_MGR_AT_CMD_TYPE_CGACT_QUERY);
        }        
    }
    else if (TEL_CONN_MGR_AT_CMD_CLASS_DEACTIVATE == cmd_class &&
             TEL_CONN_MGR_AT_CMD_TYPE_CGACT_QUERY == at_cmd_type)
    {
        if (TEL_CONN_MGR_KEY_AT_CMD_TYPE_ACT == at_cmd_flow_helper->last_key_at_cmd_type)
        {
            /* Skip AT+CGACT? */
            at_cmd_type = tel_conn_mgr_get_next_at_cmd_by_class(cmd_class,
                                                                TEL_CONN_MGR_AT_CMD_TYPE_CGACT_QUERY);
        }
    }
#ifdef TEL_CONN_MGR_ENABLE_CHECK_CEREG
    else if (TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE == cmd_class &&
             TEL_CONN_MGR_AT_CMD_TYPE_CEREG_QUERY == at_cmd_type)
    {
        /* Always send cereg query for other module may disable +CEREG URC. */
        #if 0
        /* Skip CEREG AT CMDs for network registration status has been updated. */
        if (tel_conn_mgr_is_nw_registered() ||
            tel_conn_mgr_is_nw_registration_failed())
        {
            at_cmd_type = tel_conn_mgr_get_next_at_cmd_by_class(cmd_class,
                                                                TEL_CONN_MGR_AT_CMD_TYPE_CEREG)
        }
        #endif
    }
#endif

    return at_cmd_type;
}


/**  
  * @brief     Send AT CMD
  * @param[in] tel_conn_mgr_at_cmd_class_enum cmd_class: AT CMD class
  * @param[in] tel_conn_mgr_at_cmd_type_enum cmd_type: AT CMD type
  * @retval    TEL_CONN_MGR_RET_OK = 0,
  *               TEL_CONN_MGR_RET_ERROR = -1
  */
tel_conn_mgr_ret_enum tel_conn_mgr_send_at_cmds_by_class(int cid,
                                               tel_conn_mgr_at_cmd_class_enum cmd_class,
                                               tel_conn_mgr_at_cmd_type_enum cmd_type_begin,
                                               void *callback)
{
    tel_conn_mgr_at_cmd_type_enum cmd_type = TEL_CONN_MGR_AT_CMD_TYPE_NONE;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    tel_conn_mgr_class_list_struct *cb_list_node = NULL;
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);

    TEL_CONN_MGR_LOG_INFO("cid:%d, cmd_class:%d, cmd_type_begin:%d, cb:%x", cid, cmd_class, cmd_type_begin, (unsigned int)callback);

    /* cmd_type_begin is unsigned integer. It is pointless to compare it with TEL_CONN_MGR_AT_CMD_TYPE_NONE. */
    if (!tel_conn_mgr_is_valid_at_cmd_class(cmd_class) ||
        TEL_CONN_MGR_AT_CMD_TYPE_MAX <= cmd_type_begin ||
        !tel_conn_mgr_is_cid_valid(cid) ||
#ifdef TEL_CONN_MGR_ENABLE_INACTIVATE
        (TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE == cmd_class &&
         !tel_conn_mgr_is_special_cid(cid)) ||
#endif
        !bearer_state || !at_cmd_flow_helper)
    {
        TEL_CONN_MGR_LOG_INFO("Invalid param. %s", __FUNCTION__);
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    if (!tel_conn_mgr_bearer_cntx ||
        TEL_CONN_MGR_MODEM_STATE_READY != tel_conn_mgr_bearer_cntx->modem_state)
    {
        TEL_CONN_MGR_LOG_INFO("state error");
        return TEL_CONN_MGR_RET_WRONG_STATE;
    }

    // TODO: duplicate detection maybe
    cmd_type = tel_conn_mgr_get_1st_cmd_for_class(cid,
                                                  cmd_class,
                                                  cmd_type_begin); 
    if (TEL_CONN_MGR_AT_CMD_TYPE_NONE == cmd_type)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    tel_conn_mgr_update_bearer_state_for_class_start(bearer_state, cmd_class);

    ret = tel_conn_mgr_send_at_cmd(cmd_class, cmd_type, cid);
    if (TEL_CONN_MGR_RET_OK == ret || TEL_CONN_MGR_RET_WOULDBLOCK == ret)
    {
        ret = TEL_CONN_MGR_RET_OK;

        if (callback)
        {
            cb_list_node = tel_conn_mgr_class_list_node_create(cid,
                                                               cmd_class,
                                                               callback,
                                                               TEL_CONN_MGR_BEARER_WAIT_EVENT_NONE);
            if (cb_list_node)
            {
                ret = tel_conn_mgr_list_insert((tel_conn_mgr_template_list_struct **)&at_cmd_flow_helper->class_to_finish,
                                               (tel_conn_mgr_template_list_struct *)cb_list_node);
            }
            else
            {
                ret = TEL_CONN_MGR_RET_NOT_ENOUGH_MEMORY;
            }
        }
    }

    if (TEL_CONN_MGR_RET_OK != ret)
    {
        tel_conn_mgr_revert_bearer_state_for_class_start(bearer_state, cmd_class);

        if (cb_list_node)
        {
            tel_conn_mgr_class_list_node_free(cb_list_node);
        }
    }

    TEL_CONN_MGR_LOG_INFO("%s class:%d, ret:%d \r\n", __FUNCTION__, cmd_class, ret);
    return ret;
}


#if 0
/**
  * @brief     Parse data
  * Return depends on the is_ok member of each item of tel_conn_mgr_at_cmd_rsp[].
  * @param[in] unsigned char *payload: data
  * @param[in] unsigned int length: data length
  * @retval    result
  */
tel_conn_mgr_response_type_enum tel_conn_mgr_parse_response_data(unsigned char *payload, unsigned int length)
{
    int i = -1;
    char response_str[16 + 1] = {0};
    tel_conn_mgr_bool ret = TEL_CONN_MGR_FALSE;

    if (!payload || !length)
    {
        return TEL_CONN_MGR_RESPONSE_INVALID;
    }

    ret = tel_conn_mgr_str_conv_to_upper_case(response_str, payload, 16);
    if (TEL_CONN_MGR_TRUE != ret)
    {
        return TEL_CONN_MGR_RESPONSE_INVALID;
    }

    /* Disable Echo */
    if (strstr(response_str, "ATE0")) {
        if (strstr(response_str, "OK")) {
            TEL_CONN_MGR_LOG_INFO("ATE0 Parse result is OK\r\n");
            return TEL_CONN_MGR_RESPONSE_OK;   
        } else {
            return TEL_CONN_MGR_RESPONSE_ERROR;   
        }
    }

    while (tel_conn_mgr_at_cmd_rsp[++i].urc_prefix)
    {
        if (0 == strncmp(response_str, tel_conn_mgr_at_cmd_rsp[i].urc_prefix, strlen((const char *)tel_conn_mgr_at_cmd_rsp[i].urc_prefix)))
        {
            return tel_conn_mgr_at_cmd_rsp[i].is_ok ? TEL_CONN_MGR_RESPONSE_OK : TEL_CONN_MGR_RESPONSE_ERROR;
        }
    }
    
    return TEL_CONN_MGR_RESPONSE_UNKNOW;
}


/**
  * @brief     Extract IP address.
  * @param[in] unsigned char *payload: data
  * @param[in/out] char *ip_buf: IP address buffer
  * @param[in/out] int ip_buf_len: IP address buffer length
  * @retval    Negative is fail. Zero is success.
  */
tel_conn_mgr_ret_enum tel_conn_mgr_extract_ip_addr(unsigned char *payload, unsigned char *ip_buf, int ip_buf_len)
{
    unsigned char *sub_str1 = NULL, *sub_str2 = NULL;
    int addr_len = 0;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;

    TEL_CONN_MGR_LOG_INFO("%s IP: %s", __FUNCTION__, payload);

    if (!payload || !ip_buf || !ip_buf_len ||
        !(TEL_CONN_MGR_MIN_IPV4_ADDR_LEN <= (ip_buf_len - 1) && TEL_CONN_MGR_MAX_IPV4_ADDR_LEN >= (ip_buf_len - 1))) {
        TEL_CONN_MGR_LOG_INFO("%s Invalid parameters.", __FUNCTION__);
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    /* Example Response: +CGPADDR: 1, "10.186.49.243"
        *                             +CGPRCO: 1, "183.221.253.100", "223.87.253.100", "", ""
        *                             +CGPRCO: 2, "172.21.120.6", "0.0.0.0", "", ""
        */

    /* sub_str1 points to the first byte of the IP, while sub_str2 points to the last byte of the IP. */
    sub_str1 = (unsigned char *)strchr((char *)payload, '\"');
    if (sub_str1)
    {
        ++sub_str1;
    }
    TEL_CONN_MGR_LOG_INFO("payload:%s\r\n*sub_str1:%c, *sub_str1:%d, sub_str1:%s\r\n", payload, *sub_str1, *sub_str1, sub_str1);

    sub_str2 = (unsigned char *)strchr((char *)sub_str1, '\"');
    if (sub_str2 && (sub_str2 > sub_str1))
    {
        --sub_str2;
    }
    if (sub_str1 && sub_str2) {
        addr_len = sub_str2 - sub_str1 + 1;
    }

    TEL_CONN_MGR_LOG_INFO("sub_str1:%x sub_str2:%x, addr_len:%d\r\n", sub_str1, sub_str2, addr_len);

    if (TEL_CONN_MGR_MIN_IPV4_ADDR_LEN <= addr_len &&
        TEL_CONN_MGR_MAX_IPV4_ADDR_LEN >= addr_len &&
        (ip_buf_len - 1) >= addr_len)
    {
        strncpy((char *)ip_buf, (char *)sub_str1, addr_len);
        ip_buf[addr_len] = '\0';

        TEL_CONN_MGR_LOG_INFO("Extracted IP Address:%s\r\n", ip_buf);
        ret = TEL_CONN_MGR_RET_OK;
    }

    return ret;
}


/* Fill default dns into bearer_info, and send the next AT CMD in GET_NW_INFO class. 
  * Return: Wouldblock - next AT CMD has been sent out or in at_cmd_flow_helper->at_cmd_to_send_list
  *            OK - No next AT CMD
  *            Otherwise - Error occored.
  */
tel_conn_mgr_ret_enum tel_conn_mgr_use_default_dns(tel_conn_mgr_bearer_info_struct *bearer_info, int cid)
{
    tel_conn_mgr_at_cmd_type_enum cmd_type = TEL_CONN_MGR_AT_CMD_TYPE_MAX;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_OK;

    strcpy((char *)bearer_info->pri_dns_addr, "8.8.8.8");
    bearer_info->pri_dns_addr[7] = '\0';
    strcpy((char *)bearer_info->sec_dns_addr, "8.8.4.4");
    bearer_info->sec_dns_addr[7] = '\0';

    cmd_type = tel_conn_mgr_get_next_at_cmd_by_class(TEL_CONN_MGR_AT_CMD_CLASS_GET_RELEVANT_INFO, TEL_CONN_MGR_AT_CMD_TYPE_CGPRCO);

    if (TEL_CONN_MGR_AT_CMD_TYPE_NONE == cmd_type)
    {
        return TEL_CONN_MGR_RET_OK;
    }

    ret = tel_conn_mgr_send_at_cmd(TEL_CONN_MGR_AT_CMD_CLASS_GET_RELEVANT_INFO, cmd_type, cid);
    if (TEL_CONN_MGR_RET_OK == ret)
    {
        ret = TEL_CONN_MGR_RET_WOULDBLOCK;
    }

    return ret;
}


unsigned int tel_conn_mgr_find_cid_by_channel_id(int channel_id)
{
    unsigned int bearer_info_idx = 0;
    int cid = TEL_CONN_MGR_MIN_CID - 1;
    tel_conn_mgr_bearer_info_struct *bearer_info = NULL;
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = NULL;
        
    if (TEL_CONN_MGR_MIN_VALID_CHANNEL_ID > channel_id)
    {
        return TEL_CONN_MGR_BEARER_TYPE_MAX_NUM;
    }
    
    for (bearer_info_idx = 0; TEL_CONN_MGR_BEARER_TYPE_MAX_NUM > bearer_info_idx; bearer_info_idx++)
    {
        /* Check ctrl channel. Different bearers may use the same channel id. However,
                 * RIL will not allow sending two AT CMDs on the same channel at the same time. */
        cid = tel_conn_mgr_convt_bearer_info_idx_to_cid(bearer_info_idx);
        at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);
       
        if (at_cmd_flow_helper &&
            TEL_CONN_MGR_AT_CMD_TYPE_NONE != at_cmd_flow_helper->current_at_cmd_type &&
            (//at_cmd_flow_helper->ctrl_channel_id == channel_id ||
             at_cmd_flow_helper->data_channel_id == channel_id))
        {
            return cid;
        }
    }
    
    at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(TEL_CONN_MGR_SPECIAL_CID);

    if (at_cmd_flow_helper &&
        (//at_cmd_flow_helper->ctrl_channel_id == channel_id ||
         at_cmd_flow_helper->data_channel_id == channel_id))
    {
        return TEL_CONN_MGR_SPECIAL_CID;
    }

    return TEL_CONN_MGR_MIN_CID - 1;
}
#endif


tel_conn_mgr_ret_enum tel_conn_mgr_get_bearer_relevant_info(int cid)
{
    tel_conn_mgr_class_list_struct *cb_list_node = NULL;
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;

    TEL_CONN_MGR_LOG_INFO("%d", cid);
    if (!at_cmd_flow_helper ||
        !tel_conn_mgr_is_cid_valid(cid) ||
        tel_conn_mgr_is_special_cid(cid))
    {
        return TEL_CONN_MGR_RET_ERROR;
    }

    cb_list_node = (tel_conn_mgr_class_list_struct *)tel_conn_mgr_find_list_node(
                        (tel_conn_mgr_bearer_template_list_struct *)at_cmd_flow_helper->class_to_finish,
                        cid,
                        TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE);

    /* Get IP directly */
    ret = tel_conn_mgr_send_at_cmds_by_class(cid,
                                         TEL_CONN_MGR_AT_CMD_CLASS_GET_RELEVANT_INFO,
                                         TEL_CONN_MGR_AT_CMD_TYPE_NONE,
                                         NULL);
    if (TEL_CONN_MGR_RET_OK == ret)
    {
        /* Reuse the cb_list_node of ACTIVATE class */
        cb_list_node->at_cmd_class = TEL_CONN_MGR_AT_CMD_CLASS_GET_RELEVANT_INFO;
    }

    return ret;
}


tel_conn_mgr_ret_enum tel_conn_mgr_at_cmd_rsp_process_inactivate(tel_conn_mgr_bool result,
                                                                              tel_conn_mgr_at_cmd_type_enum rsp_at_cmd_type,
                                                                              int cid,
                                                                              void *rsp_detail)
{
    tel_conn_mgr_at_cmd_type_enum next_at_cmd_type = TEL_CONN_MGR_AT_CMD_TYPE_NONE;
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;

    TEL_CONN_MGR_LOG_INFO("cid:%d, cmd_type:%d", cid, rsp_at_cmd_type);
    if (!at_cmd_flow_helper || !bearer_state)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }
    
    if (result)
    {
        next_at_cmd_type = tel_conn_mgr_get_next_at_cmd_by_class(TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE,
                                                                 rsp_at_cmd_type);

        if (TEL_CONN_MGR_AT_CMD_TYPE_NONE != next_at_cmd_type)
        {
            ret = tel_conn_mgr_send_at_cmd(TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE,
                                           next_at_cmd_type,
                                           cid);
            if (TEL_CONN_MGR_RET_OK != ret && TEL_CONN_MGR_RET_WOULDBLOCK != ret)
            {
                goto exit;
            }
            else
            {
                return TEL_CONN_MGR_RET_OK;
            }
        }

        ret = TEL_CONN_MGR_RET_OK;
    }

exit:
    (*bearer_state) &= ~TEL_CONN_MGR_BEARER_STATE_INACTIVATING;    
    if (TEL_CONN_MGR_RET_OK == ret)
    {
        *bearer_state = TEL_CONN_MGR_BEARER_STATE_INACTIVE;

        /* Wake up the cached classes for WAIT_FOR_INIT_DONE. */
        if (tel_conn_mgr_is_nw_registration_failed())
        {
            tel_conn_mgr_cache_remove_class(TEL_CONN_MGR_AT_CMD_CLASS_ALL,
                                            TEL_CONN_MGR_BEARER_WAIT_EVENT_INACTIVE_DONE,
                                            TEL_CONN_MGR_ERR_CAUSE_NW_REGISTRATION_FAILED);
        }
        else if (tel_conn_mgr_is_nw_registered())
        {
            tel_conn_mgr_cache_wake_up_class(TEL_CONN_MGR_AT_CMD_CLASS_ALL,
                                             TEL_CONN_MGR_BEARER_WAIT_EVENT_INACTIVE_DONE);
        }
        else
        {
#ifdef TEL_CONN_MGR_ENABLE_CHECK_CEREG
            /* Wake up the cached ACTIVATE class for cereg query will be sent. */
            tel_conn_mgr_cache_wake_up_class(TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE,
                                             TEL_CONN_MGR_BEARER_WAIT_EVENT_INACTIVE_DONE);
#else
            /* Update the wait event to NW_REG from INACTIVE_DONE. */
            tel_conn_mgr_cache_udpate_wait_event(TEL_CONN_MGR_AT_CMD_CLASS_ALL,
                                                 TEL_CONN_MGR_BEARER_WAIT_EVENT_INACTIVE_DONE,
                                                 TEL_CONN_MGR_BEARER_WAIT_EVENT_NW_REG_RESULT);
#endif
        }
    }    
    else
    {        
        tel_conn_mgr_cache_remove_class(TEL_CONN_MGR_AT_CMD_CLASS_ALL,
                                        TEL_CONN_MGR_BEARER_WAIT_EVENT_INACTIVE_DONE,
                                        TEL_CONN_MGR_ERR_CAUSE_INIT_FAILED);
    }

    return ret;
}


#ifdef TEL_CONN_MGR_TEMP_IT
void tel_conn_mgr_received_cgdata(int result)
{
    TEL_CONN_MGR_LOG_INFO("Received rsp for cgdata result:%d", result);
    //for (int i = 0; i < 10; i++);
}
#endif

tel_conn_mgr_ret_enum tel_conn_mgr_at_cmd_rsp_process_activate(tel_conn_mgr_bool result,
                                                                            tel_conn_mgr_at_cmd_type_enum rsp_at_cmd_type,
                                                                            int cid,                                                                            
                                                                            void *rsp_detail)
{
    tel_conn_mgr_at_cmd_type_enum next_at_cmd_type = TEL_CONN_MGR_AT_CMD_TYPE_NONE;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);
    tel_conn_mgr_bearer_info_struct *bearer_info = tel_conn_mgr_bearer_info_find_by_cid(cid);
    tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_cid(cid);
    tel_conn_mgr_bool activated = TEL_CONN_MGR_FALSE;
#ifdef TEL_CONN_MGR_ENABLE_CHECK_CEREG
    tel_conn_mgr_nw_reg_status_struct *nw_reg_status = NULL;
    tel_conn_mgr_class_list_struct *cb_list_node = NULL;
#endif

    TEL_CONN_MGR_LOG_INFO("cid:%d, cmd_type:%d, result:%d", cid, rsp_at_cmd_type, result);

#ifdef TEL_CONN_MGR_TEMP_IT
    if (TEL_CONN_MGR_AT_CMD_TYPE_CGDATA == rsp_at_cmd_type)
    {
        tel_conn_mgr_received_cgdata(result);
    }
#endif

    if (!at_cmd_flow_helper || !bearer_info || !bearer_state || !ds_keep)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    if (result)
    {
        next_at_cmd_type = tel_conn_mgr_get_next_at_cmd_by_class(TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE,
                                                                 rsp_at_cmd_type);        
        if (TEL_CONN_MGR_AT_CMD_TYPE_CGACT_QUERY == rsp_at_cmd_type)
        {
            activated = (tel_conn_mgr_bool)((unsigned int)rsp_detail);
            TEL_CONN_MGR_LOG_INFO("last_key_at_cmd_type:%d, activated:%d cid:%d",
                                  at_cmd_flow_helper->last_key_at_cmd_type,
                                  activated,
                                  cid);
            if ((activated &&
                 (TEL_CONN_MGR_KEY_AT_CMD_TYPE_NONE == at_cmd_flow_helper->last_key_at_cmd_type ||
                  TEL_CONN_MGR_KEY_AT_CMD_TYPE_ACT == at_cmd_flow_helper->last_key_at_cmd_type)) ||
                (!activated &&
                 TEL_CONN_MGR_KEY_AT_CMD_TYPE_ACT == at_cmd_flow_helper->last_key_at_cmd_type &&
                 TEL_CONN_MGR_BEARER_STATE_ACTIVATING_ACTED & (*bearer_state)))
            {
                /* Get IP directly */
                (*bearer_state) &= ~TEL_CONN_MGR_BEARER_STATE_ACTIVATING_WAIT_ACT_URC;
                if (TEL_CONN_MGR_PDP_TYPE_NIDD == ds_keep->pdp_type)
                {
                    ds_keep->activated_pdp_type = TEL_CONN_MGR_PDP_TYPE_NIDD;
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
                else
                {
                    goto exit;
                }
            }
            else if (!activated &&
                     TEL_CONN_MGR_KEY_AT_CMD_TYPE_ACT == at_cmd_flow_helper->last_key_at_cmd_type)
            {
                TEL_CONN_MGR_LOG_INFO("No at cmd needs to be sent before ACT URC received.");
                if (TEL_CONN_MGR_BEARER_STATE_ACTIVATING_WAIT_ACT_URC & (*bearer_state))
                {
                    /* Continue to wait for ACT URC */
                    TEL_CONN_MGR_LOG_INFO("Continue to wait for ACT URC.");
                    return TEL_CONN_MGR_RET_OK;
                }
                else
                {
                    TEL_CONN_MGR_LOG_ERR("ACT URC before cgact query rsp of deactivated.");
                    ret = TEL_CONN_MGR_RET_OK;
                    goto exit;
                }
            }
            else
            {
                /* Stop waiting for ACT URC. And send ACT AT CMDs*/
                TEL_CONN_MGR_LOG_INFO("Stop waiting for ACT URC. And send ACT AT CMDs.");
                assert(TEL_CONN_MGR_KEY_AT_CMD_TYPE_DEACT != at_cmd_flow_helper->last_key_at_cmd_type);
                (*bearer_state) &= ~TEL_CONN_MGR_BEARER_STATE_ACTIVATING_WAIT_ACT_URC;
            }
        }
#ifdef TEL_CONN_MGR_SKIP_CGAUTH_WHEN_NO_AUTH
        else if (TEL_CONN_MGR_AT_CMD_TYPE_CGAUTH == next_at_cmd_type)
        {
            if (!bearer_info->username[0])
            {
                TEL_CONN_MGR_LOG_INFO("Skip CGAUTH (CGAUTH should only be skipped when activating the default APN).");
                next_at_cmd_type = tel_conn_mgr_get_next_at_cmd_by_class(TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE,
                                                                         TEL_CONN_MGR_AT_CMD_TYPE_CGAUTH);        
            }
        }
#endif
        else if (TEL_CONN_MGR_AT_CMD_TYPE_CGDATA == rsp_at_cmd_type)
        {
            at_cmd_flow_helper->last_key_at_cmd_type = TEL_CONN_MGR_KEY_AT_CMD_TYPE_ACT;
#ifdef TEL_CONN_MGR_TEMP_IT
            //TEL_CONN_MGR_LOG_INFO("Skip CGEV URC");
            //(*bearer_state) |= TEL_CONN_MGR_BEARER_STATE_ACTIVATING_ACTED;
#endif
        }
#ifdef TEL_CONN_MGR_ENABLE_CHECK_CEREG
        else if (TEL_CONN_MGR_AT_CMD_TYPE_CEREG_QUERY == rsp_at_cmd_type)
        {
            tel_conn_mgr_nw_reg_status_struct *nw_reg_sta = tel_conn_mgr_bearer_get_nw_reg_status();
            nw_reg_status = (tel_conn_mgr_nw_reg_status_struct *)rsp_detail;
            
            if (!rsp_detail || !nw_reg_sta)
            {
                ret = TEL_CONN_MGR_RET_ERROR;
                goto exit;
            }

            nw_reg_sta->n = nw_reg_status->n;
            nw_reg_sta->stat= nw_reg_status->stat;
            nw_reg_sta->act= nw_reg_status->act;

#if defined(TEL_CONN_MGR_SUPPORT_DEEP_SLEEP) && !defined(TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM)
            tel_conn_mgr_nvdm_write(TEL_CONN_MGR_NVDM_COMMON_GROUP,
                                    TEL_CONN_MGR_NVDM_COMMON_NW_REG_STATUS,
                                    NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                    (const uint8_t *)nw_reg_sta,
                                    sizeof(tel_conn_mgr_nw_reg_status_struct));
#endif

            if (tel_conn_mgr_is_nw_registration_failed())
            {
                ret = TEL_CONN_MGR_RET_ERROR;
                goto exit;
            }
            else if (tel_conn_mgr_is_nw_registered())
            {
                /* If +CEREG URC has been enabled, skip it to continue to send AT CMDs to activate the PDN context. 
                                  Otherwise, enable +CEREG URC first before sending any AT CMDs to activate the PDN context. */
                if (2 == nw_reg_status->n &&
                    TEL_CONN_MGR_AT_CMD_TYPE_CEREG == next_at_cmd_type)
                {
                    next_at_cmd_type = tel_conn_mgr_get_next_at_cmd_by_class(TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_CEREG);
                }
            }
            else
            {
                /* If +CEREG URC has been enabled, cache the activation request and wait until the network registration is 
                               * done before sending any AT CMDs to activate the PDN context.  Otherwise, enable +CEREG URC and then
                               * cache the activation request and wait for the network registration. */
                if (2 == nw_reg_status->n &&
                    TEL_CONN_MGR_AT_CMD_TYPE_CEREG == next_at_cmd_type)
                {
                    /* Cache ACTIVATE class for CEREG URC */
                    next_at_cmd_type = tel_conn_mgr_get_next_at_cmd_by_class(TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_CEREG);
                    
                    TEL_CONN_MGR_LOG_INFO("Cache ACTIVATE class for NW REG STATUS. ");
                    cb_list_node = (tel_conn_mgr_class_list_struct *)tel_conn_mgr_find_list_node(
                                        (tel_conn_mgr_bearer_template_list_struct *)at_cmd_flow_helper->class_to_finish,
                                        cid,
                                        TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE);
                    if (!cb_list_node)
                    {
                        ret = TEL_CONN_MGR_RET_NOT_FOUND;
                        goto exit;
                    }
                    
                    cb_list_node->wait_event_group |= TEL_CONN_MGR_BEARER_WAIT_EVENT_NW_REG_RESULT;
                    cb_list_node->cmd_type_begin = next_at_cmd_type;
                    
                    /* Cache ACTIVATION class */
                    tel_conn_mgr_list_remove((tel_conn_mgr_template_list_struct **)&at_cmd_flow_helper->class_to_finish,
                                                (tel_conn_mgr_template_list_struct *)cb_list_node);
                    tel_conn_mgr_list_insert((tel_conn_mgr_template_list_struct **)&at_cmd_flow_helper->class_to_excute,
                                                (tel_conn_mgr_template_list_struct *)cb_list_node);
                    return TEL_CONN_MGR_RET_OK;
                }
            }            
        }
        else if (TEL_CONN_MGR_AT_CMD_TYPE_CEREG == rsp_at_cmd_type)
        {
            tel_conn_mgr_nw_reg_status_struct *nw_reg_sta = tel_conn_mgr_bearer_get_nw_reg_status();
            if (nw_reg_sta)
            {
                /* The value should be the same as the at+cereg=val in tel_conn_mgr_at_cmd_info[] for the activation. */
                nw_reg_sta->n = 2;

#if defined(TEL_CONN_MGR_SUPPORT_DEEP_SLEEP) && !defined(TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM)
                tel_conn_mgr_nvdm_write(TEL_CONN_MGR_NVDM_COMMON_GROUP,
                                        TEL_CONN_MGR_NVDM_COMMON_NW_REG_STATUS,
                                        NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                        (const uint8_t *)nw_reg_sta,
                                        sizeof(tel_conn_mgr_nw_reg_status_struct));
#endif
            }
            else
            {
                TEL_CONN_MGR_LOG_WARN("nw_reg_status was not found.");
            }

            if (tel_conn_mgr_is_nw_registration_failed())
            {
                ret = TEL_CONN_MGR_RET_ERROR;
                goto exit;
            }
            else if (!tel_conn_mgr_is_nw_registered())
            {
                /* Cache the activation request and wait until the network registration is done before sending any
                               * AT CMDs to activate the PDN context. */
            
                /* Cache ACTIVATE class for CEREG URC */
                next_at_cmd_type = tel_conn_mgr_get_next_at_cmd_by_class(TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE, TEL_CONN_MGR_AT_CMD_TYPE_CEREG);
                
                TEL_CONN_MGR_LOG_INFO("Cache ACTIVATE class for NW REG STATUS. ");
                cb_list_node = (tel_conn_mgr_class_list_struct *)tel_conn_mgr_find_list_node(
                                        (tel_conn_mgr_bearer_template_list_struct *)at_cmd_flow_helper->class_to_finish,
                                        cid,
                                        TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE);
                    if (!cb_list_node)
                    {
                        ret = TEL_CONN_MGR_RET_NOT_FOUND;
                        goto exit;
                    }
                cb_list_node->wait_event_group |= TEL_CONN_MGR_BEARER_WAIT_EVENT_NW_REG_RESULT;
                cb_list_node->cmd_type_begin = next_at_cmd_type;
                
                /* Cache ACTIVATION class */
                tel_conn_mgr_list_remove((tel_conn_mgr_template_list_struct **)&at_cmd_flow_helper->class_to_finish,
                                            (tel_conn_mgr_template_list_struct *)cb_list_node);
                tel_conn_mgr_list_insert((tel_conn_mgr_template_list_struct **)&at_cmd_flow_helper->class_to_excute,
                                            (tel_conn_mgr_template_list_struct *)cb_list_node);
                return TEL_CONN_MGR_RET_OK;            
            }
            /* else sending next AT CMD to activate the PDN context. */
        }        
#endif
        if (TEL_CONN_MGR_AT_CMD_TYPE_NONE != next_at_cmd_type)
        {
            ret = tel_conn_mgr_send_at_cmd(TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE,
                                           next_at_cmd_type,
                                           cid);
            if (TEL_CONN_MGR_RET_OK != ret && TEL_CONN_MGR_RET_WOULDBLOCK != ret)
            {
                goto exit;
            }
            else
            {
                return TEL_CONN_MGR_RET_OK;
            }
        }

        ret = TEL_CONN_MGR_RET_OK;
    }

exit:
    if (TEL_CONN_MGR_RET_OK == ret)
    {
        /* All ACTIVATE class AT CMDs have been processed. */
        if (TEL_CONN_MGR_BEARER_STATE_ACTIVATING_ACTED & (*bearer_state))
        {
            /* Get IP directly. */
            if (TEL_CONN_MGR_PDP_TYPE_NIDD == ds_keep->pdp_type)
            {
                return tel_conn_mgr_bearer_terminate_activating_state(cid,
                                                                      TEL_CONN_MGR_TRUE,
                                                                      TEL_CONN_MGR_ERR_CAUSE_NONE,
                                                                      TEL_CONN_MGR_FALSE);
            }
            
            TEL_CONN_MGR_LOG_INFO("Get IP directly.");
            ret = tel_conn_mgr_get_bearer_relevant_info(cid);
            if (TEL_CONN_MGR_RET_OK == ret)
            {
                return ret;
            }
        }
        else
        {
            /* Wait for ACT URC. */
            assert((*bearer_state) & TEL_CONN_MGR_BEARER_STATE_ACTIVATING_WAIT_ACT_URC);
            TEL_CONN_MGR_LOG_INFO("Wait for ACT URC.");
            return ret;
        }
    }

    ret = tel_conn_mgr_bearer_terminate_activating_state(cid,
                                                         TEL_CONN_MGR_FALSE,
                                                         TEL_CONN_MGR_ERR_CAUSE_AT_CMD_EXCEPTION,
                                                         TEL_CONN_MGR_KEY_AT_CMD_TYPE_ACT == at_cmd_flow_helper->last_key_at_cmd_type);

    return ret;
}


tel_conn_mgr_ret_enum tel_conn_mgr_at_cmd_rsp_process_get_relevant_info(tel_conn_mgr_bool result,
                                                                                       tel_conn_mgr_at_cmd_type_enum rsp_at_cmd_type,
                                                                                       int cid,
                                                                                       void *rsp_detail)
{
    tel_conn_mgr_at_cmd_type_enum next_at_cmd_type = TEL_CONN_MGR_AT_CMD_TYPE_NONE;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    tel_conn_mgr_bearer_ip_info_struct *ip_info = NULL, *ip_info_tmp = NULL;
    tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_cid(cid);

    TEL_CONN_MGR_LOG_INFO("cid:%d, cmd_type:%d, result:%d", cid, rsp_at_cmd_type, result);
    if (!ds_keep)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    if (result)
    {
        next_at_cmd_type = tel_conn_mgr_get_next_at_cmd_by_class(TEL_CONN_MGR_AT_CMD_CLASS_GET_RELEVANT_INFO,
                                                                 rsp_at_cmd_type);        
        if (TEL_CONN_MGR_AT_CMD_TYPE_CGCONTRDP == rsp_at_cmd_type)                
        {
#ifdef TEL_CONN_MGR_INFORM_TCPIP_NIDD
            nbiot_bearer_info_struct netinfo = {0};
                
            ip_info = (tel_conn_mgr_bearer_ip_info_struct *)rsp_detail;
            TEL_CONN_MGR_LOG_INFO("Send IP info to TCPIP. ip_info:%x", (unsigned int)ip_info);
            assert(ip_info);
            
            netinfo.is_activated = 1;
            netinfo.channel_id = ds_keep->data_channel_id;
            netinfo.cid = ds_keep->cid;
            
            TEL_CONN_MGR_LOG_INFO("ip_info->ipv4_mtu:%d", ip_info->ipv4_mtu);
            if (ip_info->ipv4_mtu == RIL_OMITTED_INTEGER_PARAM) {
                netinfo.mtu = 1460;
            } else {
                netinfo.mtu = ip_info->ipv4_mtu;

            }

            ip_info_tmp = ip_info;
            do
            {
                memset(netinfo.ip_addr, 0, 64);
                memset(netinfo.pri_dns, 0, 64);
                memset(netinfo.snd_dns, 0, 64);
                strncpy((char *)netinfo.ip_addr, (const char *)ip_info_tmp->local_addr, TEL_CONN_MGR_IPV6_STR_MAX_LEN);
                strncpy((char *)netinfo.pri_dns, (const char *)ip_info_tmp->dns_prim_addr, TEL_CONN_MGR_IPV6_STR_MAX_LEN);
                strncpy((char *)netinfo.snd_dns, (const char *)ip_info_tmp->dns_sec_addr, TEL_CONN_MGR_IPV6_STR_MAX_LEN);
                netinfo.type = ip_info_tmp->is_ipv6;
                nb_netif_bearer_info_ind(&netinfo);
                ip_info_tmp = ip_info_tmp->next;
            } while (ip_info_tmp);
            /* ip_info will be freed in tel_conn_mgr_bearer_at_cmd_rsp_ind_free() */
#endif
            TEL_CONN_MGR_LOG_INFO("Send IP done.");
            // TODO: If IP addr is all zero, activation fail.
            // TODO: Save the IP for comparing the new received IP? If that, IP should be stored and not freed at tel_conn_mgr_task_main();
        }

        if (TEL_CONN_MGR_AT_CMD_TYPE_NONE != next_at_cmd_type)
        {
            ret = tel_conn_mgr_send_at_cmd(TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE,
                                           next_at_cmd_type,
                                           cid);
            if (TEL_CONN_MGR_RET_OK != ret && TEL_CONN_MGR_RET_WOULDBLOCK != ret)
            {
                goto exit;
            }
            else
            {
                return TEL_CONN_MGR_RET_OK;
            }
        }
        
        ret = TEL_CONN_MGR_RET_OK;            
    }

exit:
    if (TEL_CONN_MGR_RET_OK == ret)
    {
        tel_conn_mgr_bearer_terminate_activating_state(cid,
                                                       TEL_CONN_MGR_TRUE,
                                                       TEL_CONN_MGR_ERR_CAUSE_NONE,
                                                       TEL_CONN_MGR_FALSE);
    }
    else
    {
        tel_conn_mgr_bearer_terminate_activating_state(cid,
                                                       TEL_CONN_MGR_FALSE,
                                                       TEL_CONN_MGR_ERR_CAUSE_AT_CMD_EXCEPTION,
                                                       TEL_CONN_MGR_TRUE);
    }
    
    return ret;
}


tel_conn_mgr_ret_enum tel_conn_mgr_at_cmd_rsp_process_deactivate(tel_conn_mgr_bool result,
                                                                              tel_conn_mgr_at_cmd_type_enum rsp_at_cmd_type,
                                                                              int cid,
                                                                              void *rsp_detail)
{
    tel_conn_mgr_at_cmd_type_enum next_at_cmd_type = TEL_CONN_MGR_AT_CMD_TYPE_NONE;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    tel_conn_mgr_err_cause_enum cause = TEL_CONN_MGR_ERR_CAUSE_AT_CMD_EXCEPTION;
    tel_conn_mgr_bearer_state_enum *bearer_state = tel_conn_mgr_get_bearer_state_by_cid(cid);
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);
    tel_conn_mgr_bool activated = TEL_CONN_MGR_FALSE;

    TEL_CONN_MGR_LOG_INFO("cid:%d, cmd_type:%d, result:%d", cid, rsp_at_cmd_type, result);

    if (!bearer_state || !at_cmd_flow_helper)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }
    
    if (result)
    {      
        next_at_cmd_type = tel_conn_mgr_get_next_at_cmd_by_class(TEL_CONN_MGR_AT_CMD_CLASS_DEACTIVATE,
                                                                 rsp_at_cmd_type);        
        
        if (TEL_CONN_MGR_AT_CMD_TYPE_CGACT_QUERY == rsp_at_cmd_type)
        {
            TEL_CONN_MGR_LOG_INFO("last_key_at_cmd_type:%d", at_cmd_flow_helper->last_key_at_cmd_type);
            activated = (tel_conn_mgr_bool)((unsigned int)rsp_detail);
            if ((!activated &&
                 (TEL_CONN_MGR_KEY_AT_CMD_TYPE_NONE == at_cmd_flow_helper->last_key_at_cmd_type ||
                  TEL_CONN_MGR_KEY_AT_CMD_TYPE_DEACT == at_cmd_flow_helper->last_key_at_cmd_type)) ||
                (activated &&
                 TEL_CONN_MGR_KEY_AT_CMD_TYPE_DEACT == at_cmd_flow_helper->last_key_at_cmd_type &&
                 TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_DEACTED & (*bearer_state)))
            {
                /* Send DEACT MSG to apps with result of OK */
                (*bearer_state) &= ~TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_WAIT_DEACT_URC;
                (*bearer_state) |= TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_DEACTED;
                ret = TEL_CONN_MGR_RET_OK;
                cause = TEL_CONN_MGR_ERR_CAUSE_NONE;
                goto exit;
            }
            else if (activated &&
                     TEL_CONN_MGR_KEY_AT_CMD_TYPE_DEACT == at_cmd_flow_helper->last_key_at_cmd_type)
            {
                if (TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_WAIT_DEACT_URC & (*bearer_state))
                {
                    /* Continue to wait for DEACT URC */
                    TEL_CONN_MGR_LOG_INFO("Continue to wait for DEACT URC");
                    return TEL_CONN_MGR_RET_OK;
                }
                else
                {
                    TEL_CONN_MGR_LOG_ERR("DEACT URC before cgact query rsp of activated.");
                    ret = TEL_CONN_MGR_RET_OK;
                    goto exit;
                }
            }
            else
            {
                assert(TEL_CONN_MGR_KEY_AT_CMD_TYPE_ACT != at_cmd_flow_helper->last_key_at_cmd_type);
                /* Stop waiting for DEACT URC. And send DEACT AT CMDs*/
                (*bearer_state) &= ~TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_WAIT_DEACT_URC;
            }
        }
        else if (TEL_CONN_MGR_AT_CMD_TYPE_CGACT_DEACT == rsp_at_cmd_type)
        {
            at_cmd_flow_helper->last_key_at_cmd_type = TEL_CONN_MGR_KEY_AT_CMD_TYPE_DEACT;
        }
        
        if (TEL_CONN_MGR_AT_CMD_TYPE_NONE != next_at_cmd_type)
        {
            ret = tel_conn_mgr_send_at_cmd(TEL_CONN_MGR_AT_CMD_CLASS_DEACTIVATE,
                                           next_at_cmd_type,
                                           cid);
            if (TEL_CONN_MGR_RET_OK != ret && TEL_CONN_MGR_RET_WOULDBLOCK != ret)
            {
                goto exit;
            }
            else
            {
                return TEL_CONN_MGR_RET_OK;
            }
        }

        ret = TEL_CONN_MGR_RET_OK;
    }

exit:
    if (TEL_CONN_MGR_RET_OK == ret)
    {
        if (TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_DEACTED & (*bearer_state))
        {
            ret = tel_conn_mgr_bearer_terminate_deactivating_state(cid,
                                                                   TEL_CONN_MGR_TRUE,
                                                                   TEL_CONN_MGR_ERR_CAUSE_NONE);
        }
        else
        {
            assert((*bearer_state) & TEL_CONN_MGR_BEARER_STATE_DEACTIVATING_WAIT_DEACT_URC);
            TEL_CONN_MGR_LOG_INFO("Wait for DEACT URC.");
            return ret;
        }
    }
    else
    {
        ret = tel_conn_mgr_bearer_terminate_deactivating_state(cid,
                                                               TEL_CONN_MGR_FALSE,
                                                               cause);
    }
    
    return ret;
}


tel_conn_mgr_ret_enum tel_conn_mgr_at_cmd_rsp_process(tel_conn_mgr_bearer_at_cmd_rsp_ind_struct *at_cmd_rsp_ind)
{
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_OK;
    tel_conn_mgr_at_cmd_class_enum cmd_class = TEL_CONN_MGR_AT_CMD_CLASS_NONE;
    tel_conn_mgr_at_cmd_type_enum rsp_at_cmd_type = TEL_CONN_MGR_AT_CMD_TYPE_MAX;
    tel_conn_mgr_at_cmd_list_struct *recv_list_node = NULL;
    int cid = TEL_CONN_MGR_MIN_CID - 1;
    tel_conn_mgr_at_cmd_flow_helper_struct *at_cmd_flow_helper = NULL;
    
    if (!at_cmd_rsp_ind)
    {
        TEL_CONN_MGR_LOG_ERR("%s Invalid param", __FUNCTION__);
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    cid = at_cmd_rsp_ind->cid;
    at_cmd_flow_helper = tel_conn_mgr_find_at_cmd_flow_helper_by_cid(cid);

    if (!at_cmd_flow_helper || !at_cmd_flow_helper->at_cmd_to_recv_list)
    {
        TEL_CONN_MGR_LOG_INFO("%s State Error. curr_atcmd_type:%d", __FUNCTION__,
            at_cmd_flow_helper ? at_cmd_flow_helper->current_at_cmd_type : TEL_CONN_MGR_AT_CMD_TYPE_NONE);
        
        if (at_cmd_flow_helper)
        {
            at_cmd_flow_helper->current_at_cmd_type = TEL_CONN_MGR_AT_CMD_TYPE_NONE;
        }
        return TEL_CONN_MGR_RET_WRONG_STATE;
    }

    recv_list_node = at_cmd_flow_helper->at_cmd_to_recv_list;
  //  assert(recv_list_node->at_cmd_type == at_cmd_rsp_ind->at_cmd_type);
    if(recv_list_node->at_cmd_type != at_cmd_rsp_ind->at_cmd_type) {
       TEL_CONN_MGR_LOG_ERR("abnormal msg from MD:%d, %d", recv_list_node->at_cmd_type, at_cmd_rsp_ind->at_cmd_type);
       return TEL_CONN_MGR_RET_WRONG_STATE;
    }
    assert(recv_list_node->is_expired ? true : recv_list_node->at_cmd_type == at_cmd_flow_helper->current_at_cmd_type);
    assert(at_cmd_rsp_ind->cid == recv_list_node->cid);


    /* Sending next at cmd is allowed. */
    at_cmd_flow_helper->current_at_cmd_type = TEL_CONN_MGR_AT_CMD_TYPE_NONE;    
    rsp_at_cmd_type = recv_list_node->at_cmd_type;    
    cmd_class = recv_list_node->at_cmd_class;
    TEL_CONN_MGR_LOG_INFO("cid:%d, result:%d", at_cmd_rsp_ind->cid, at_cmd_rsp_ind->result);
    TEL_CONN_MGR_LOG_INFO("rsp at cmd type: %d, class:%d\r\n", at_cmd_rsp_ind->at_cmd_type, cmd_class);
    tel_conn_mgr_list_remove((tel_conn_mgr_template_list_struct **)&at_cmd_flow_helper->at_cmd_to_recv_list, 
                             (tel_conn_mgr_template_list_struct *)recv_list_node);
    
    /* Expired by terminate APIs */
    if (recv_list_node->is_expired)
    {
        TEL_CONN_MGR_LOG_INFO("rsp is ignored.");
        tel_conn_mgr_at_cmd_list_node_free(recv_list_node);
        tel_conn_mgr_send_next_at_cmd(cid);
        return TEL_CONN_MGR_RET_CANCELED;
    }

    tel_conn_mgr_at_cmd_list_node_free(recv_list_node);
    recv_list_node = NULL;

    switch (cmd_class)
    {
#ifdef TEL_CONN_MGR_ENABLE_INACTIVATE
        case TEL_CONN_MGR_AT_CMD_CLASS_INACTIVATE:
        {
            ret = tel_conn_mgr_at_cmd_rsp_process_inactivate(at_cmd_rsp_ind->result,
                                                       rsp_at_cmd_type,
                                                       cid,
                                                       at_cmd_rsp_ind->rsp_detail);
            break;
        }
#endif
        case TEL_CONN_MGR_AT_CMD_CLASS_ACTIVATE:
        {
            ret = tel_conn_mgr_at_cmd_rsp_process_activate(at_cmd_rsp_ind->result,
                                                           rsp_at_cmd_type,
                                                           cid,
                                                           at_cmd_rsp_ind->rsp_detail);
            break;
        }

        case TEL_CONN_MGR_AT_CMD_CLASS_GET_RELEVANT_INFO:
        {
            ret = tel_conn_mgr_at_cmd_rsp_process_get_relevant_info(at_cmd_rsp_ind->result,
                                                              rsp_at_cmd_type,
                                                              cid,
                                                              at_cmd_rsp_ind->rsp_detail);
            break;
        }

        case TEL_CONN_MGR_AT_CMD_CLASS_DEACTIVATE:
        {
            ret = tel_conn_mgr_at_cmd_rsp_process_deactivate(at_cmd_rsp_ind->result,
                                                             rsp_at_cmd_type,
                                                             cid,
                                                             at_cmd_rsp_ind->rsp_detail);
            break;
        }

        default:
        {   
            ret = TEL_CONN_MGR_RET_ERROR;
            break;
        }
    }

    return ret;
}


/* If error is returned, cntx will be freed by tel_conn_mgr_at_cmd_deinit() in main.c */
tel_conn_mgr_ret_enum tel_conn_mgr_at_cmd_init(void)
{
    tel_conn_mgr_nw_reg_status_struct *nw_reg_sta = NULL;
#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
    rtc_power_on_result_t deep_sleep_bootup = 0;
    uint8_t active_bearer_count = 0;
    tel_conn_mgr_bearer_info_ds_keep_struct *ds_keep = NULL;
#ifndef TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM    
    char nvdm_bearer_info_group[20] = {0};
#endif
#endif

    TEL_CONN_MGR_LOG_INFO("%s", __FUNCTION__);
    
    /* Init tel_conn_mgr_bearer_cntx */
    if (tel_conn_mgr_bearer_cntx)
    {
        TEL_CONN_MGR_LOG_INFO("bearer_cntx has been initialized.");
        return TEL_CONN_MGR_RET_OK;
    }

    tel_conn_mgr_bearer_cntx = (tel_conn_mgr_bearer_cntx_struct*)tel_conn_mgr_calloc(1, sizeof(tel_conn_mgr_bearer_cntx_struct));
    if (!tel_conn_mgr_bearer_cntx)
    {
        return TEL_CONN_MGR_RET_NOT_ENOUGH_MEMORY;
    }
    
    memset(tel_conn_mgr_bearer_cntx, 0, sizeof(tel_conn_mgr_bearer_cntx_struct));
    tel_conn_mgr_bearer_cntx->modem_state = TEL_CONN_MGR_MODEM_STATE_READY;

    nw_reg_sta = tel_conn_mgr_bearer_get_nw_reg_status();
    assert(nw_reg_sta);
    nw_reg_sta->stat = TEL_CONN_MGR_NW_REG_STAT_MAX;
    nw_reg_sta->act = TEL_CONN_MGR_NW_REG_ACT_MAX;

#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
    tel_conn_mgr_bearer_cntx->sleep_manager_handle = tel_conn_mgr_set_sleep_handle(TEL_CONN_MGR_BEARER_SLEEP_MANAGER_NAME);
    if (TEL_CONN_MGR_SLEEP_LOCK_INVALID_ID == tel_conn_mgr_bearer_cntx->sleep_manager_handle)
    {
        TEL_CONN_MGR_LOG_ERR("Failed to create the sleep manager for bearer");
        return TEL_CONN_MGR_RET_ERROR;
    }

    deep_sleep_bootup = rtc_power_on_result_external();
    
    TEL_CONN_MGR_LOG_INFO("deep_sleep_bootup:d%\r\n",deep_sleep_bootup);
    if (deep_sleep_bootup == DEEP_SLEEP || deep_sleep_bootup == DEEPER_SLEEP)
    {
#ifdef TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM
        for (int i = 0; i < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; i++)
        {
            ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_idx_ds(i);
            if (ds_keep && tel_conn_mgr_is_cid_valid(ds_keep->cid) &&
                !tel_conn_mgr_is_special_cid(ds_keep->cid) &&
                tel_conn_mgr_is_bearer_active(ds_keep->bearer_state))
            {
                /* Bearer_info is used. */
                // TODO: if bearer_type or sim_id extended, bearer_type or sim_id should be stored into nvdm or rtc ram.
                tel_conn_mgr_bearer_cntx->bearer_info[i].at_cmd_flow_helper.last_key_at_cmd_type = TEL_CONN_MGR_KEY_AT_CMD_TYPE_ACT;
                tel_conn_mgr_bearer_cntx->bearer_info[i].bearer_type = TEL_CONN_MGR_BEARER_TYPE_NBIOT;
                tel_conn_mgr_bearer_cntx->bearer_info[i].sim_id = TEL_CONN_MGR_SIM_ID_1;
                tel_conn_mgr_bearer_cntx->bearer_info[i].is_used = TEL_CONN_MGR_TRUE;
                active_bearer_count++;
            }
        }
#else
        /* Restore bearer_cntx from nvdm */
        tel_conn_mgr_nvdm_read(TEL_CONN_MGR_NVDM_COMMON_GROUP,
                            TEL_CONN_MGR_NVDM_COMMON_NW_REG_STATUS,
                            (uint8_t *)&tel_conn_mgr_bearer_cntx->nw_reg_status,
                            sizeof(tel_conn_mgr_nw_reg_status_struct));

        for (int i = 0; i < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; i++)
        {
            memset(nvdm_bearer_info_group, 0, 20);
            sprintf(nvdm_bearer_info_group, 
                    TEL_CONN_MGR_NVDM_BEARER_INFO_GROUP_BASE,
                    i);

            tel_conn_mgr_nvdm_read(nvdm_bearer_info_group,
                                TEL_CONN_MGR_NVDM_BEARER_INFO_IS_USED,
                                (uint8_t *)&tel_conn_mgr_bearer_cntx->bearer_info[i].is_used,
                                 sizeof(tel_conn_mgr_bool));
            if (!tel_conn_mgr_bearer_cntx->bearer_info[i].is_used ||
                !(ds_keep = tel_conn_mgr_bearer_info_get_ds_keep_by_idx(i)))
            {
                continue;
            }

            active_bearer_count++;

            tel_conn_mgr_nvdm_read(nvdm_bearer_info_group,
                                 TEL_CONN_MGR_NVDM_BEARER_INFO_PDP_TYPE,
                                 (uint8_t *)&ds_keep->pdp_type,
                                 sizeof(tel_conn_mgr_pdp_type_enum));

            tel_conn_mgr_nvdm_read(nvdm_bearer_info_group,
                                 TEL_CONN_MGR_NVDM_BEARER_INFO_ACTIVATED_PDP_TYPE,
                                 (uint8_t *)&ds_keep->activated_pdp_type,
                                 sizeof(tel_conn_mgr_pdp_type_enum));

            tel_conn_mgr_nvdm_read(nvdm_bearer_info_group,
                                 TEL_CONN_MGR_NVDM_BEARER_INFO_APN,
                                 (uint8_t *)ds_keep->apn,
                                 TEL_CONN_MGR_APN_MAX_LEN);
            
            tel_conn_mgr_nvdm_read(nvdm_bearer_info_group,
                                 TEL_CONN_MGR_NVDM_BEARER_INFO_DATA_CHANNEL_ID,
                                 (uint8_t *)&ds_keep->data_channel_id,
                                 sizeof(int));
            
            tel_conn_mgr_nvdm_read(nvdm_bearer_info_group,
                                 TEL_CONN_MGR_NVDM_BEARER_INFO_BEARER_STATE,
                                 (uint8_t *)&ds_keep->bearer_state,
                                 sizeof(tel_conn_mgr_bearer_type_enum));

            tel_conn_mgr_bearer_cntx->bearer_info[i].at_cmd_flow_helper.last_key_at_cmd_type = TEL_CONN_MGR_KEY_AT_CMD_TYPE_ACT;
            // TODO: if bearer_type or sim_id extended, bearer_type or sim_id should be stored into nvdm.
            tel_conn_mgr_bearer_cntx->bearer_info[i].bearer_type = TEL_CONN_MGR_BEARER_TYPE_NBIOT;
            tel_conn_mgr_bearer_cntx->bearer_info[i].sim_id = TEL_CONN_MGR_SIM_ID_1;
        }
#endif
        if (active_bearer_count)
        {
            TEL_CONN_MGR_LOG_INFO("Total %d active bearer(s)",active_bearer_count);
            tel_conn_mgr_bearer_cntx->bearer_state = TEL_CONN_MGR_BEARER_STATE_INACTIVE;
        }
    }
    else
    {
#ifndef TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM
        tel_conn_mgr_bool is_used = TEL_CONN_MGR_FALSE;
        /* Cold bootup. Reset NVDM */
        tel_conn_mgr_nvdm_write(TEL_CONN_MGR_NVDM_COMMON_GROUP,
                             TEL_CONN_MGR_NVDM_COMMON_NW_REG_STATUS,
                             NVDM_DATA_ITEM_TYPE_RAW_DATA,
                             (const uint8_t *)&tel_conn_mgr_bearer_cntx->nw_reg_status,
                             sizeof(tel_conn_mgr_nw_reg_status_struct));        
        
        for (int i = 0; i < TEL_CONN_MGR_BEARER_TYPE_MAX_NUM; i++)
        {
            memset(nvdm_bearer_info_group, 0, 20);
            sprintf(nvdm_bearer_info_group, 
                    TEL_CONN_MGR_NVDM_BEARER_INFO_GROUP_BASE,
                    i);

            tel_conn_mgr_nvdm_write(nvdm_bearer_info_group,
                                    TEL_CONN_MGR_NVDM_BEARER_INFO_IS_USED,
                                    NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                    (const uint8_t *)&is_used,
                                    sizeof(tel_conn_mgr_bool));
        }
#else
        tel_conn_mgr_bearer_info_ds_keep_init();
#endif
    }
#endif

    return TEL_CONN_MGR_RET_OK;
}


void tel_conn_mgr_at_cmd_deinit(void)
{
    if (tel_conn_mgr_bearer_cntx)
    {
        tel_conn_mgr_free(tel_conn_mgr_bearer_cntx);
        tel_conn_mgr_bearer_cntx = NULL;
    }
}


#if 0
tel_conn_mgr_ret_enum tel_conn_mgr_at_cmd_set_app_id(int app_id)
{
    if (0 > app_id)
    {
        return TEL_CONN_MGR_RET_INVALID_PARAM;
    }

    if (!tel_conn_mgr_bearer_cntx)
    {
        return TEL_CONN_MGR_RET_WRONG_STATE;
    }

    tel_conn_mgr_bearer_cntx->app_id = app_id;

    return TEL_CONN_MGR_RET_OK;
}


int tel_conn_mgr_at_cmd_get_app_id(void)
{
    return tel_conn_mgr_bearer_cntx->app_id;
}
#endif

tel_conn_mgr_bearer_cntx_struct *tel_conn_mgr_bearer_get_cntx(void)
{
    return tel_conn_mgr_bearer_cntx;
}


tel_conn_mgr_nw_reg_status_struct *tel_conn_mgr_bearer_get_nw_reg_status(void)
{
#ifdef TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM
    return &g_nw_reg_status;
#else
    return tel_conn_mgr_bearer_cntx ? &tel_conn_mgr_bearer_cntx->nw_reg_status : NULL;
#endif
}

