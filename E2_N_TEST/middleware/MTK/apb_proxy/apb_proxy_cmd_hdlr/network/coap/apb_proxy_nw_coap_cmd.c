/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
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

#include "syslog.h"

#include "lwip/sockets.h"
#include "coap.h"

#include "apb_proxy.h"
#include "apb_proxy_nw_cmd.h"
#include "apb_proxy_nw_cmd_util.h"
#include "apb_proxy_nw_coap.h"
#include "apb_proxy_nw_coap_cmd.h"

extern uint8_t g_nw_coap_buf[NW_COAP_MAX_PDU_LENGTH];

extern uint32_t g_nw_coap_debug_flag;

/* CoAP client AT cmd handler */
apb_proxy_status_t apb_proxy_hdlr_coap_new_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    char *par_str = NULL;
    char *temp = NULL;
    apb_proxy_at_cmd_result_t cmd_result;
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;
    uint8_t sa_family = 0;
    coap_dev_info_t *dev = NULL;
    int32_t port = 0;
    uint32_t calc_len = 0;
    uint8_t pdn = NW_COAP_INVALID_PDN;
    char result_str[16] = {0};
    int32_t coap_ret = 0;
    coap_dev_role_t role = COAP_DEV_ROLE_CLIENT;

    memset(&cmd_result, 0x00, sizeof(apb_proxy_at_cmd_result_t));
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    NW_COAP_LOG("new_cmd--str: %s", p_parse_cmd->string_ptr);

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }

        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }

        case APB_PROXY_CMD_MODE_EXECUTION: {
            par_str = p_parse_cmd->string_ptr + p_parse_cmd->name_len + 1;
            dev = nw_coap_find_dev(COAP_DEV_TYPE_UNUSED, &role);

            if (!dev) {
                NW_COAP_LOG("new_cmd--no dev resource");
                break;
            }

            NW_COAP_LOG("new_cmd--str_len: %d, name_len: %d, par_len: %d",
                p_parse_cmd->string_len, p_parse_cmd->name_len, strlen(par_str));

            /* Process ip_addr */
            temp = parse_next_string(par_str, &par_str);
            if (!temp) {
                NW_COAP_LOG("new_cmd--parse ip_add error");
                break;
            }

            NW_COAP_LOG("new_cmd--ip_len: %d", strlen(temp));

            if (strlen(temp) < 7 || strlen(temp) > 15) {
                NW_COAP_LOG("new_cmd--ip len error");
                break;
            }

            sa_family = nw_coap_get_sa_family(temp);

            if (sa_family == AF_INET) { /* IPV4 */
                inet_pton(AF_INET, temp, &(dev->dst.addr.sin.sin_addr.s_addr));
                dev->dst.size = sizeof(struct sockaddr_in);
                dev->dst.addr.sin.sin_family = AF_INET;
                dev->dst.addr.sin.sin_len = sizeof(struct sockaddr_in);
            } else if (sa_family == AF_INET6) { /* IPV6 */
                ;
            } else {
                NW_COAP_LOG("new_cmd--unknown ip_addr");
                break;
            }

            calc_len = p_parse_cmd->name_len + 1 + strlen(temp) + 1;

            /* Process port */
            temp = parse_next_string(par_str, &par_str);
            if (!temp) {
                NW_COAP_LOG("new_cmd--parse port error");
                break;
            }

            NW_COAP_LOG("new_cmd--port_len: %d", strlen(temp));

            if (strlen(temp) > 5) {
                NW_COAP_LOG("new_cmd--port len error");
                break;
            }

            port = _atoi(temp);

            if (port < 1 || port > 0xffff) {
                NW_COAP_LOG("new_cmd--invalid port: %d", port);
                break;
            }

            NW_COAP_LOG("new_cmd--port: %d", port);
            dev->dst.addr.sin.sin_port = htons(port);

            /* Process pdn */
            calc_len += strlen(temp);
            NW_COAP_LOG("new_cmd--calc: %d", calc_len);
            if (calc_len + 1 == p_parse_cmd->string_len) {
                dev->pdn = NW_COAP_INVALID_PDN;
            } else {
                temp = par_str;
                if (temp[strlen(temp) - 1] == '\r' || temp[strlen(temp) - 1] == '\n') {
                    /* Remove \r, \n */
                    temp[strlen(temp) - 1] = '\0';
                }
                pdn =_atoi(temp);
                dev->pdn = pdn;
            }

            nw_coap_show_dev(dev);

            coap_ret = nw_coap_new_context(dev);

            if (coap_ret != NW_COAP_RET_SUCCESS) {
                NW_COAP_LOG("new_cmd--new coap_ctx fail");
                break;
            }

            nw_coap_show_dev(dev);

            /* Retention handle */
            nw_coap_retention_set(dev);

            snprintf(result_str, 16, "+ECOAPNEW: %d", dev->cid);

            NW_COAP_LOG("new_cmd--cmd result: %s", result_str);

            cmd_result.pdata = result_str;
            cmd_result.result_code = APB_PROXY_RESULT_OK;
            ret = APB_PROXY_STATUS_OK;
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }
        default: {
            break;
        }
    }

    if (ret != APB_PROXY_STATUS_OK && dev) {
        /* Reset coap dev */
        nw_coap_reset_dev(dev);
    }

    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) + 1 : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);

    return ret;
}


apb_proxy_status_t apb_proxy_hdlr_coap_del_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result;
    uint8_t cid = 0;
    coap_dev_info_t *dev = NULL;
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;

    memset(&cmd_result, 0x00, sizeof(apb_proxy_at_cmd_result_t));
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    NW_COAP_LOG("del_cmd--str: %s", p_parse_cmd->string_ptr);

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }

        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }

        case APB_PROXY_CMD_MODE_EXECUTION: {
            cid = _atoi(p_parse_cmd->string_ptr + p_parse_cmd->name_len + 1);
            NW_COAP_LOG("del_cmd--cid: %d", cid);

            if (cid > 0 && cid < (COAP_CLIENT_MAX_NUM + 1)) {
                dev = nw_coap_find_dev(COAP_DEV_TYPE_CID, &cid);
            }

            if (dev) {
                nw_coap_free_context(dev);
                cmd_result.result_code = APB_PROXY_RESULT_OK;
                ret = APB_PROXY_STATUS_OK;
            }
            break;
        }

        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }

        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }

        default:
            break;
    }

    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) + 1 : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);

    return ret;
}


apb_proxy_status_t apb_proxy_hdlr_coap_send_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result;
    char *par_str = NULL;
    char *temp = NULL;
    uint8_t cid = 0;
    uint32_t data_len = 0;
    char *data_buf = NULL;
    int32_t result = 0;
    coap_dev_info_t *dev = NULL;
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;

    memset(&cmd_result, 0x00, sizeof(apb_proxy_at_cmd_result_t));
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    NW_COAP_LOG("send_cmd--str: %s", p_parse_cmd->string_ptr);

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }

        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }

        case APB_PROXY_CMD_MODE_EXECUTION: {
            par_str = p_parse_cmd->string_ptr + p_parse_cmd->name_len + 1;

            /* Process coap id (cid) */
            temp = parse_next_string(par_str, &par_str);
            if (!temp) {
                NW_COAP_LOG("send_cmd--parse cid error");
                break;
            }

            cid = _atoi(temp);
            if (cid < 1 || cid > COAP_CLIENT_MAX_NUM) {
                NW_COAP_LOG("send_cmd--invalid cid: %d", cid);
                break;
            }

            NW_COAP_LOG("send_cmd--cid: %d", cid);
            dev = nw_coap_find_dev(COAP_DEV_TYPE_CID, &cid);
            if (!dev) {
                NW_COAP_LOG("send_cmd--can't find cid: %d", cid);
                break;
            }

            /* Process data_len */
            temp = parse_next_string(par_str, &par_str);
            if (!temp) {
                NW_COAP_LOG("send_cmd--parse data_len error");
                break;
            }

            data_len = _atoi(temp);
            if (data_len > COAP_CLIENT_DATA_MAX_LEN) {
                NW_COAP_LOG("send_cmd--invalid data_len: %d", data_len);
                break;
            }

            NW_COAP_LOG("send_cmd--data_len: %d", data_len);

            /* Process data */
            temp = par_str;
            if (!temp) {
                NW_COAP_LOG("send_cmd--parse data error");
                break;
            }

            if (temp[strlen(temp) - 1] == '\r' || temp[strlen(temp) - 1] == '\n') {
                /* Remove \r, \n */
                temp[strlen(temp) - 1] = '\0';
            }

            NW_COAP_LOG("send_cmd--buf: %s", temp);

            if ((data_len << 1) != strlen(temp)) {
                NW_COAP_LOG("send_cmd--data len match fail: %d != %d", data_len << 1, strlen(temp));
                {
                    int32_t i = 0, k = 0;
                    k = strlen(temp);
                    for (i = 0; i < k; ++i) {
                        NW_COAP_LOG("send_cmd--data[%d]: 0x%02x", i, temp[i]);
                    }
                }
                break;
            }

            data_buf = (char *)g_nw_coap_buf;
            if (!data_buf) {
                NW_COAP_LOG("send_cmd--alloc data buf error");
                break;
            }

            _get_data_from_hex(data_buf, temp, data_len);
            //nw_coap_dump_buff("convert data", data_buf, data_len);
            result = nw_coap_send_request(cid, data_buf, data_len);

            if (result == NW_COAP_RET_SUCCESS) {
                cmd_result.result_code = APB_PROXY_RESULT_OK;
                ret = APB_PROXY_STATUS_OK;
            }

            break;
        }

        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }

        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }

        default:
            break;
    }

    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) + 1 : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);

    return ret;
}


/* CoAP server AT cmd handler */
apb_proxy_status_t apb_proxy_hdlr_coap_sta_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    char *par_str = NULL;
    char *temp = NULL;
    apb_proxy_at_cmd_result_t cmd_result;
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;
    uint8_t sa_family = 0;
    coap_dev_info_t *dev = NULL;
    int32_t port = 0;
    uint32_t calc_len = 0;
    uint8_t pdn = NW_COAP_INVALID_PDN;
    char result_str[16] = {0};
    int32_t coap_ret = 0;
    coap_dev_role_t role = COAP_DEV_ROLE_SERVER;

    memset(&cmd_result, 0x00, sizeof(apb_proxy_at_cmd_result_t));
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    NW_COAP_LOG("sta_cmd--str: %s", p_parse_cmd->string_ptr);

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }

        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }

        case APB_PROXY_CMD_MODE_EXECUTION: {
            par_str = p_parse_cmd->string_ptr + p_parse_cmd->name_len + 1;
            dev = nw_coap_find_dev(COAP_DEV_TYPE_UNUSED, &role);

            if (!dev) {
                NW_COAP_LOG("sta_cmd--no dev resource");
                break;
            }

            NW_COAP_LOG("sta_cmd--str_len: %d, name_len: %d, par_len: %d",
                p_parse_cmd->string_len, p_parse_cmd->name_len, strlen(par_str));

            /* Process ip_addr */
            temp = parse_next_string(par_str, &par_str);
            if (!temp) {
                NW_COAP_LOG("sta_cmd--parse ip_add error");
                break;
            }

            NW_COAP_LOG("sta_cmd--ip_len: %d", strlen(temp));

            if (strlen(temp) < 7 || strlen(temp) > 15) {
                NW_COAP_LOG("sta_cmd--ip len error");
                break;
            }

            sa_family = nw_coap_get_sa_family(temp);

            if (sa_family == AF_INET) { /* IPV4 */
                //inet_pton(AF_INET, temp, &(dev->dst.addr.sin.sin_addr.s_addr));
                dev->dst.addr.sin.sin_addr.s_addr = htonl(INADDR_ANY);
                dev->dst.size = sizeof(struct sockaddr_in);
                dev->dst.addr.sin.sin_family = AF_INET;
                dev->dst.addr.sin.sin_len = sizeof(struct sockaddr_in);
            } else if (sa_family == AF_INET6) { /* IPV6 */
                ;
            } else {
                NW_COAP_LOG("sta_cmd--unknown ip_addr");
                break;
            }

            calc_len = p_parse_cmd->name_len + 1 + strlen(temp) + 1;

            /* Process port */
            temp = parse_next_string(par_str, &par_str);
            if (!temp) {
                NW_COAP_LOG("sta_cmd--parse port error");
                break;
            }

            NW_COAP_LOG("sta_cmd--port_len: %d", strlen(temp));

            if (strlen(temp) > 5) {
                NW_COAP_LOG("sta_cmd--port len error");
                break;
            }

            port = _atoi(temp);

            if (port < 1 || port > 0xffff) {
                NW_COAP_LOG("sta_cmd--invalid port: %d", port);
                break;
            }

            NW_COAP_LOG("sta_cmd--port: %d", port);
            dev->dst.addr.sin.sin_port = htons(port);

            /* Process pdn */            
            calc_len += strlen(temp);
            NW_COAP_LOG("sta_cmd--calc: %d", calc_len);
            if (calc_len + 1 == p_parse_cmd->string_len) {
                dev->pdn = NW_COAP_INVALID_PDN;
            } else {
                temp = par_str;
                if (temp[strlen(temp) - 1] == '\r' || temp[strlen(temp) - 1] == '\n') {
                    /* Remove \r, \n */
                    temp[strlen(temp) - 1] = '\0';
                }
                pdn =_atoi(temp);
                dev->pdn = pdn;
            }

            nw_coap_show_dev(dev);

            coap_ret = nw_coap_new_context(dev);

            if (coap_ret != NW_COAP_RET_SUCCESS) {
                NW_COAP_LOG("sta_cmd--new coap_ctx fail");
                break;
            }

            nw_coap_show_dev(dev);

            /* Retention handle */
            nw_coap_retention_set(dev);

            snprintf(result_str, 16, "+ECOAPSTA: %d", dev->cid);

            NW_COAP_LOG("sta_cmd--cmd result: %s", result_str);

            cmd_result.pdata = result_str;
            cmd_result.result_code = APB_PROXY_RESULT_OK;
            ret = APB_PROXY_STATUS_OK;
            break;
        }
        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }
        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }
        default: {
            break;
        }
    }

    if (ret != APB_PROXY_STATUS_OK && dev) {
        /* Reset coap dev */
        nw_coap_reset_dev(dev);
    }

    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) + 1 : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);

    return ret;
}


apb_proxy_status_t apb_proxy_hdlr_coap_debug_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result;
    //coap_dev_info_t *dev = NULL;
    apb_proxy_status_t ret = APB_PROXY_STATUS_ERROR;
    uint32_t flag = 0;

    memset(&cmd_result, 0x00, sizeof(apb_proxy_at_cmd_result_t));
    cmd_result.result_code = APB_PROXY_RESULT_ERROR;

    NW_COAP_LOG("debug_cmd--str: %s", p_parse_cmd->string_ptr);

    switch (p_parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_READ: {
            /*do something*/
            break;
        }

        case APB_PROXY_CMD_MODE_ACTIVE: {
            /*do something*/
            break;
        }

        case APB_PROXY_CMD_MODE_EXECUTION: {
            #if 0
            extern void nb_lwm2m_sleep_debug_handle(void);
            extern void nb_lwm2m_wakeup_debug_handle(void);
            extern void nb_lwm2m_debug_init(void);
            #endif
            flag = _atoi(p_parse_cmd->string_ptr + p_parse_cmd->name_len + 1);
            NW_COAP_LOG("debug_cmd--flag: 0x%08d", flag);

            g_nw_coap_debug_flag = flag;
            #if 0
            /* flag: 0x01 sleep */
            if (g_nw_coap_debug_flag & 0x01) {
                nb_lwm2m_sleep_debug_handle();
            }

            /* flag: 0x02 wakeup */
            if (g_nw_coap_debug_flag & 0x02) {
                nb_lwm2m_wakeup_debug_handle();
            }

            /* flag: 0x04 lwm2m init */
            if (g_nw_coap_debug_flag & 0x04) {
                nb_lwm2m_debug_init();
            }
            #endif

            cmd_result.result_code = APB_PROXY_RESULT_OK;
            ret = APB_PROXY_STATUS_OK;

            break;
        }

        case APB_PROXY_CMD_MODE_TESTING: {
            /*do something*/
            break;
        }

        case APB_PROXY_CMD_MODE_INVALID: {
            /*do something*/
            break;
        }

        default:
            break;
    }

    cmd_result.length = cmd_result.pdata ? strlen(cmd_result.pdata) + 1 : 0;
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);

    return ret;
}

