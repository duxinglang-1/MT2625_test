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

#include "stdio.h"
#include "string.h"
#include "apb_proxy.h"
#include "syslog.h"

#ifdef MTK_LWM2M_CT_SUPPORT
#include "liblwm2m.h"
#include "apb_proxy_nw_cmd_gprot.h"
#include "app_common_header.h"

#include "hal_rtc_external.h"
#include "nvdm.h"
#include "ril.h"

#define LWM2M_CLI_LOG(fmt, args...)    LOG_I(lwm2m_cli, "[LWM2M_CLI]"fmt, ##args)
log_create_module(lwm2m_cli, PRINT_LEVEL_INFO);

#define CTIOT_LWM2M_CLIENT_NVDM_GROUP_NAME          "ct_lwm2m"
#define CTIOT_LWM2M_CLIENT_NVDM_ITEM_NAME_CONFIG    "config"
#define CTIOT_LWM2M_CLIENT_MAX_CONFIG_LEN           128

#ifndef __H10_TEMP__
#include "h10_mmi.h"
#endif

extern nb_app_context g_nb_app_context;
extern void lwm2m_client_init();
extern void lwm2m_client_quit(char *title, int quit);
extern void lwm2m_client_deregister(void);
extern bool lwm2m_client_is_connected(void);
extern void sbit_m2m_ct_send_massege(int msg,char *str,int len,bool isr);

void ctiot_lwm2m_client_send_response(char *pdata)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    response.cmd_id = APB_PROXY_INVALID_CMD_ID;
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;
    response.pdata = pdata;
    response.length = strlen(pdata);
    apb_proxy_send_at_cmd_result(&response);
}

void ctiot_lwm2m_client_observe_success(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:observe success";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot_lwm2m_client_send_response(response_data);
    sbit_m2m_ct_send_massege(M2M_CT_SEND,NULL,0,0);
}

void ctiot_lwm2m_client_notify_success(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:notify success";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot_lwm2m_client_send_response(response_data);    
	sbit_m2m_ct_send_massege(M2M_CT_NOTIFY,NULL,0,0);

}

void ctiot_lwm2m_client_notify_failed(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:notify failed";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot_lwm2m_client_send_response(response_data);
}

void ctiot_lwm2m_client_handshake_success(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:handshake success";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot_lwm2m_client_send_response(response_data);
}

void ctiot_lwm2m_client_handshake_failed(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:handshake failed";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot_lwm2m_client_send_response(response_data);
    if (g_nb_app_context.activated_from_command == true) {
        lwm2m_client_quit("LWM2M_CLI", 3);
        g_nb_app_context.activated_from_command = false;
    }
}

void ctiot_lwm2m_client_register_success(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:register success";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot_lwm2m_client_send_response(response_data);
}

void ctiot_lwm2m_client_register_failed(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:register failed";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot_lwm2m_client_send_response(response_data);
	sbit_m2m_ct_send_massege(M2M_CT_REGISTER_FAIL,NULL,0,0);
}

void ctiot_lwm2m_client_register_update_success(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:register update success";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	ctiot_lwm2m_client_send_response(response_data);   
	sbit_m2m_ct_send_massege(M2M_CT_UPDATE,NULL,0,0);

}

void ctiot_lwm2m_client_register_update_failed(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:register update failed";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot_lwm2m_client_send_response(response_data);
}

void ctiot_lwm2m_client_deregister_success(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:deregister success";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot_lwm2m_client_send_response(response_data);
    if (g_nb_app_context.wait_for_deleted == true) {
        lwm2m_client_quit("LWM2M_CLI", 3);
        g_nb_app_context.wait_for_deleted = false;
    }   
	//sbit_m2m_ct_send_massege(M2M_CT_DEL_SUCCESS,NULL,0,0);
    sbit_m2m_ct_send_massege(M2M_CT_CFUN_OFF,NULL,0,0);
}

void ctiot_lwm2m_client_deregister_failed(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:deregister failed";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot_lwm2m_client_send_response(response_data);
    if (g_nb_app_context.wait_for_deleted == true) {
        lwm2m_client_quit("LWM2M_CLI", 3);
        g_nb_app_context.wait_for_deleted = false;
    }
}

void ctiot_lwm2m_client_session_abnormal(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:lwm2m session abnormal";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot_lwm2m_client_send_response(response_data);
    if (g_nb_app_context.activated_from_command == true) {
        lwm2m_client_quit("LWM2M_CLI", 3);
        g_nb_app_context.activated_from_command = false;
    }
}

#define DATA_RECV_MAX 256
char DATA_RECV[DATA_RECV_MAX] = {0};
void NB_IOT_HextoChs (char* ascii)
{
	int len = strlen (ascii);
	int  i = 0 ;
	char ch[2] = {0};
	
	
	if(len%2!=0)
	{
		return;
	}
	memset(DATA_RECV,0,sizeof(DATA_RECV));
	while( i < len )
	{
		ch[0] = ( (int)ascii[i] > 64 ) ? ( ascii[i]%16 + 9 ) : ascii[i]%16 ;
		ch[1] = ( (int)ascii[i + 1] > 64 ) ? ( ascii[i + 1]%16 + 9 ) : ascii[i + 1]%16 ;

		DATA_RECV[i/2] = (char)( ch[0]*16 + ch[1] );
		i += 2;
	}
}
int T29_send_fail = 0;
void ctiot_lwm2m_client_receive_data(uint8_t *data, uint32_t data_len)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/
	char *prefix = "+M2MCLIRECV:";
	char *response_data;
	int i;

	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
	LWM2M_CLI_LOG("data = 0x%x, data_len = %d", data, data_len);
	for (i = 0; i < data_len; i++) {
		LWM2M_CLI_LOG("0x%x", data[i]);
	}
	// +M2MCLIRECV:<Data>
#ifdef USE_HAIWEI_RAWDATA_PLUGIN
	response_data = (char *)pvPortMalloc(strlen(prefix) + 2 * data_len + 1);
#else
	response_data = (char *)pvPortMalloc(strlen(prefix) + data_len + 1);
#endif
    if (response_data == NULL) 
	{
		ctiot_lwm2m_client_send_response("response_data");
        return;
    }

	strcpy(response_data, prefix);
#ifdef USE_HAIWEI_RAWDATA_PLUGIN
	onenet_at_bin_to_hex(response_data + strlen(prefix), data, 2 * data_len);
#else
	if (data_len > 3) 
	{
		/* 1 byte message_id, 2 bytes length */
		memcpy(response_data + strlen(prefix), data + 3, data_len - 3);
		response_data[strlen(prefix) + data_len - 3] = '\0';
	}
#endif
 		LWM2M_CLI_LOG("===receive_data:%s", response_data);
 		ctiot_lwm2m_client_send_response(response_data);
	#if defined(MTK_SEND_QUEUE_FIFO)
		i = get_m2m_send_Queue_Fifo();
		if((i >= 0)&&((strstr(temp_info1.m2m_send_temp_Queue[i],temp_info.send_temp_Queue))!=NULL)&&(strstr(response_data,"AABB0000") != NULL))
		{
			del_m2m_send_Queue_Fifo();
			memset(temp_info.send_temp_Queue, 0, sizeof(temp_info.send_temp_Queue));
		}
	#else
		i = get_m2m_send_Queue();
		if((i >= 0)&&((strstr(temp_info1.m2m_send_temp_Queue[i],temp_info.send_temp_Queue))!=NULL)&&(strstr(response_data,"AABB0000") != NULL))
		{
			memset(temp_info1.m2m_send_temp_Queue[i],0,sizeof(temp_info1.m2m_send_temp_Queue[i]));
			memset(temp_info.send_temp_Queue, 0, sizeof(temp_info.send_temp_Queue));
		}
	#endif
		sbit_m2m_ct_send_massege(M2M_CT_RECV,data,data_len,0);
		NVRAM_info.network_fail_record = 0;
		
		if(NVRAM_info.movement_flag == 1)
		temp_info.reset_time = 5;
		else
		temp_info.reset_time = 30;
		
		temp_info.network_time_cumulative = 0;
		{
			char *p = NULL;
			p = response_data + strlen(prefix);
			NB_IOT_HextoChs(p);
			LWM2M_CLI_LOG("===receive_data:%s", DATA_RECV);
		
			ctiot_lwm2m_client_send_response(DATA_RECV);
			ctiot_lwm2m_command_handle(DATA_RECV);
		}
		
		if(strstr(response_data,"AABB0000")==NULL)
		{
			temp_info.recv_flag = 65;
		}
		
		vPortFree(response_data);
 
 }


static int32_t ctiot_lwm2m_client_ril_urc_callback(ril_urc_id_t event_id, void *param, uint32_t param_len)
{
    switch (event_id) {
        case RIL_URC_ID_MATWAKEUP:
            LWM2M_CLI_LOG("receive RIL wakeup urc");
            if (lwm2m_client_is_connected() == true) {
                uint32_t len;
                char *cmd_string = (char *)pvPortMalloc(CTIOT_LWM2M_CLIENT_MAX_CONFIG_LEN);
                if (cmd_string == NULL) {
                    return -1;
                }
                char *param_buffer = (char *)pvPortMalloc(CTIOT_LWM2M_CLIENT_MAX_CONFIG_LEN);
                if (param_buffer == NULL) {
                    vPortFree(cmd_string);
                    return -1;
                }
                len = CTIOT_LWM2M_CLIENT_MAX_CONFIG_LEN;
                nvdm_status_t status = nvdm_read_data_item(CTIOT_LWM2M_CLIENT_NVDM_GROUP_NAME,
                                    CTIOT_LWM2M_CLIENT_NVDM_ITEM_NAME_CONFIG,
                                    (uint8_t *)cmd_string,
                                    &len);
                if (status != NVDM_STATUS_OK) {
                    vPortFree(param_buffer);
                    vPortFree(cmd_string);
                    return -1;
                }
                char *param_list[6];
                uint32_t param_num = onenet_at_parse_cmd(cmd_string, param_buffer, param_list, 6);
                char *server = param_list[0];
                char *port = param_list[1];
                char *name = param_list[2];
                int lifetime = atoi(param_list[3]);
                g_nb_app_context.server = (char *)pvPortMalloc(strlen(server) + 1);
                if (g_nb_app_context.server == NULL) {
                    vPortFree(param_buffer);
                    vPortFree(cmd_string);
                    return -1;
                }
                strcpy(g_nb_app_context.server, server);
                g_nb_app_context.port = (char *)pvPortMalloc(strlen(port) + 1);
                if (g_nb_app_context.port == NULL) {
                    vPortFree(g_nb_app_context.server);
                    vPortFree(param_buffer);
                    vPortFree(cmd_string);
                    return -1;
                }
                strcpy(g_nb_app_context.port, port);
                g_nb_app_context.name = (char *)pvPortMalloc(strlen(name) + 1);
                if (g_nb_app_context.name == NULL) {
                    vPortFree(g_nb_app_context.port);
                    vPortFree(g_nb_app_context.server);
                    vPortFree(param_buffer);
                    vPortFree(cmd_string);
                    return -1;
                }
                strcpy(g_nb_app_context.name, name);
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
                if (param_num > 5) {
                    char *pskid = param_list[4];
                    char *psk = param_list[5];
                    g_nb_app_context.pskid = (char *)pvPortMalloc(strlen(pskid) + 1);
                    if (g_nb_app_context.pskid == NULL) {
                        vPortFree(g_nb_app_context.name);
                        vPortFree(g_nb_app_context.port);
                        vPortFree(g_nb_app_context.server);
                        vPortFree(param_buffer);
                        vPortFree(cmd_string);
                        return -1;
                    }
                    strcpy(g_nb_app_context.pskid, pskid);
                    g_nb_app_context.psk = (char *)pvPortMalloc(strlen(psk) + 1);
                    if (g_nb_app_context.psk == NULL) {
                        vPortFree(g_nb_app_context.pskid);
                        vPortFree(g_nb_app_context.name);
                        vPortFree(g_nb_app_context.port);
                        vPortFree(g_nb_app_context.server);
                        vPortFree(param_buffer);
                        vPortFree(cmd_string);
                        return -1;
                    }
                    strcpy(g_nb_app_context.psk, psk);
                }
#endif
                g_nb_app_context.lifetime = lifetime;
                g_nb_app_context.activated_from_command = true;
                lwm2m_client_init();
                vPortFree(param_buffer);
                vPortFree(cmd_string);
            }
            break;

        default:
            break;
    }

    return 0;
}

void ctiot_lwm2m_client_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (rtc_power_on_result_external() != DEEP_SLEEP && rtc_power_on_result_external() != DEEPER_SLEEP) {
        /* COLD-BOOT case: normal init */
    } else {
        /* DEEP-SLEEP case: data retention process */
        LWM2M_CLI_LOG("deep sleep handling");
#if 0
        ril_register_event_callback(RIL_GROUP_MASK_ALL, ctiot_lwm2m_client_ril_urc_callback);
#else
        ctiot_lwm2m_client_ril_urc_callback(RIL_URC_ID_MATWAKEUP, NULL, 0);
#endif
    }
}

apb_proxy_status_t apb_proxy_hdlr_lwm2m_client_create_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    LWM2M_CLI_LOG("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char *)pvPortMalloc(parse_cmd->string_len);
            if (param_buffer == NULL) {
                LWM2M_CLI_LOG("memory error");
                response.result_code = APB_PROXY_RESULT_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    LWM2M_CLI_LOG("no parameter");
                    response.result_code = APB_PROXY_RESULT_ERROR;
                } else {
                    char *param_list[6];
                    nvdm_status_t status = nvdm_write_data_item(CTIOT_LWM2M_CLIENT_NVDM_GROUP_NAME,
                                 CTIOT_LWM2M_CLIENT_NVDM_ITEM_NAME_CONFIG,
                                 NVDM_DATA_ITEM_TYPE_STRING,
                                 (const uint8_t *)(cmd_string + 1),
                                 CTIOT_LWM2M_CLIENT_MAX_CONFIG_LEN);
                    if (status != NVDM_STATUS_OK) {
                        vPortFree(param_buffer);
                        response.result_code = APB_PROXY_RESULT_ERROR;
                        goto CREATE_EXIT;
                    }
                    LWM2M_CLI_LOG("cmd_string=%s", cmd_string);
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, 6);

                    // AT+M2MCLINEW=<server>,<port>,<name>,<lifetime>[,<pskid>,<psk>]
                    if (param_num < 4) {
                        LWM2M_CLI_LOG("parameter too short");
                        response.result_code = APB_PROXY_RESULT_ERROR;
                    } else if (g_nb_app_context.lwm2m_info != NULL) {
                        response.result_code = APB_PROXY_RESULT_ERROR;
                    } else {
                        char *server = param_list[0];
                        char *port = param_list[1];
                        char *name = param_list[2];
                        int lifetime = atoi(param_list[3]);
                        g_nb_app_context.server = (char *)pvPortMalloc(strlen(server) + 1);
                        if (g_nb_app_context.server == NULL) {
                            vPortFree(param_buffer);
                            response.result_code = APB_PROXY_RESULT_ERROR;
                            goto CREATE_EXIT;
                        }
                        strcpy(g_nb_app_context.server, server);
                        g_nb_app_context.port = (char *)pvPortMalloc(strlen(port) + 1);
                        if (g_nb_app_context.port == NULL) {
                            vPortFree(g_nb_app_context.server);
                            vPortFree(param_buffer);
                            response.result_code = APB_PROXY_RESULT_ERROR;
                            goto CREATE_EXIT;
                        }
                        strcpy(g_nb_app_context.port, port);
                        g_nb_app_context.name = (char *)pvPortMalloc(strlen(name) + 1);
                        if (g_nb_app_context.name == NULL) {
                            vPortFree(g_nb_app_context.port);
                            vPortFree(g_nb_app_context.server);
                            vPortFree(param_buffer);
                            response.result_code = APB_PROXY_RESULT_ERROR;
                            goto CREATE_EXIT;
                        }
                        strcpy(g_nb_app_context.name, name);
#if defined(WITH_MBEDTLS) || defined(WITH_TINYDTLS)
                        if (param_num > 5) {
                            char *pskid = param_list[4];
                            char *psk = param_list[5];
                            g_nb_app_context.pskid = (char *)pvPortMalloc(strlen(pskid) + 1);
                            if (g_nb_app_context.pskid == NULL) {
                                vPortFree(g_nb_app_context.name);
                                vPortFree(g_nb_app_context.port);
                                vPortFree(g_nb_app_context.server);
                                vPortFree(param_buffer);
                                response.result_code = APB_PROXY_RESULT_ERROR;
                                goto CREATE_EXIT;
                            }
                            strcpy(g_nb_app_context.pskid, pskid);
                            g_nb_app_context.psk = (char *)pvPortMalloc(strlen(psk) + 1);
                            if (g_nb_app_context.psk == NULL) {
                                vPortFree(g_nb_app_context.pskid);
                                vPortFree(g_nb_app_context.name);
                                vPortFree(g_nb_app_context.port);
                                vPortFree(g_nb_app_context.server);
                                vPortFree(param_buffer);
                                response.result_code = APB_PROXY_RESULT_ERROR;
                                goto CREATE_EXIT;
                            }
                            strcpy(g_nb_app_context.psk, psk);
                        }
#endif
                        g_nb_app_context.lifetime = lifetime;
                        g_nb_app_context.activated_from_command = true;
                        lwm2m_client_init();
                        response.result_code = APB_PROXY_RESULT_OK;
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }
            break;

        default:
            LWM2M_CLI_LOG("not support");
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;
    }

CREATE_EXIT:
    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_lwm2m_client_delete_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    LWM2M_CLI_LOG("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_ACTIVE:
            if (g_nb_app_context.lwm2m_info == NULL) {
                response.result_code = APB_PROXY_RESULT_ERROR;
            } else {
                // AT+M2MCLIDEL
                if (g_nb_app_context.server != NULL) {
                    vPortFree(g_nb_app_context.server);
                    g_nb_app_context.server = NULL;
                }
                if (g_nb_app_context.port != NULL) {
                    vPortFree(g_nb_app_context.port);
                    g_nb_app_context.port = NULL;
                }
                if (g_nb_app_context.name != NULL) {
                    vPortFree(g_nb_app_context.name);
                    g_nb_app_context.name = NULL;
                }
                if (g_nb_app_context.pskid != NULL) {
                    vPortFree(g_nb_app_context.pskid);
                    g_nb_app_context.pskid = NULL;
                }
                if (g_nb_app_context.psk != NULL) {
                    vPortFree(g_nb_app_context.psk);
                    g_nb_app_context.psk = NULL;
                }
                g_nb_app_context.lifetime = 0;
                g_nb_app_context.activated_from_command = false;
                g_nb_app_context.wait_for_deleted = true;
                lwm2m_client_deregister();
                response.result_code = APB_PROXY_RESULT_OK;
            }
            break;

        default:
            LWM2M_CLI_LOG("not support");
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_lwm2m_client_send_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer;
    bool need_send_quickly = false;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    LWM2M_CLI_LOG("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char *)pvPortMalloc(parse_cmd->string_len);
            if (param_buffer == NULL) {
                LWM2M_CLI_LOG("memory error");
                response.result_code = APB_PROXY_RESULT_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    LWM2M_CLI_LOG("no parameter");
                    response.result_code = APB_PROXY_RESULT_ERROR;
                } else {
                    char *param_list[1];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, 1);

                    // AT+M2MCLISEND=<Data>
                    if (param_num < 1) {
                        LWM2M_CLI_LOG("parameter too short");
                        response.result_code = APB_PROXY_RESULT_ERROR;
                    } else if (strlen(param_list[0]) % 2 != 0) {
                        LWM2M_CLI_LOG("length of <Data> is odd");
                        response.result_code = APB_PROXY_RESULT_ERROR;
                    } else {
                        char *obj_uri_string = "/19/0/0";
                        lwm2m_uri_t uri;
                        uint8_t *config_bin;
                        uint32_t config_len = strlen(param_list[0]) / 2;
#ifdef USE_HAIWEI_RAWDATA_PLUGIN
                        config_bin = (uint8_t *)pvPortMalloc(config_len + 1);
                        if (config_bin == NULL) {
                            vPortFree(param_buffer);
                            response.result_code = APB_PROXY_RESULT_ERROR;
                            goto SEND_EXIT;
                        }
                        config_len = onenet_at_hex_to_bin(config_bin, param_list[0], config_len);
#else
                        /* 1 byte message_id, 2 bytes length */
                        config_bin = (uint8_t *)pvPortMalloc(config_len + 4);
                        if (config_bin == NULL) {
                            vPortFree(param_buffer);
                            response.result_code = APB_PROXY_RESULT_ERROR;
                            goto SEND_EXIT;
                        }
                        config_bin[0] = 0;
                        if (config_len <= 0xFF) {
                            config_bin[1] = 0;
                            config_bin[2] = config_len;
                        } else {
                            config_bin[1] = (config_len & 0xFF00) >> 8;
                            config_bin[2] = (config_len & 0x00FF);
                        }
                        config_len = onenet_at_hex_to_bin(config_bin + 3, param_list[0], config_len);
                        config_len += 3;
                        LWM2M_CLI_LOG("config_bin[1-2] = 0x%x 0x%x", config_bin[1], config_bin[2]);
#endif
                        lwm2m_stringToUri(obj_uri_string, strlen(obj_uri_string), &uri);
                        if (g_nb_app_context.lwm2m_info == NULL) {
                            response.result_code = APB_PROXY_RESULT_ERROR;
                        } else {
                            handle_value_changed(g_nb_app_context.lwm2m_info, &uri, (const char *)config_bin, config_len);
                            response.result_code = APB_PROXY_RESULT_OK;
                            need_send_quickly = true;
                        }
                        vPortFree(config_bin);
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }
            break;

        default:
            LWM2M_CLI_LOG("not support");
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;
    }

SEND_EXIT:
    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    if (need_send_quickly == true) {
        extern void lwm2m_step_quickly(lwm2m_context_t * contextP);
        lwm2m_step_quickly(g_nb_app_context.lwm2m_info);
    }

    return APB_PROXY_STATUS_OK;
}

#endif /* MTK_LWM2M_CT_SUPPORT */

