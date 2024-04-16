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

#include "FreeRTOS.h"
#include "task.h"
#include "syslog.h"

#include "hal_rtc_external.h"
#include "memory_attribute.h"
#include "lwip/sockets.h"
#include "coap.h"


#include "apb_proxy.h"
#include "apb_proxy_nw_cmd.h"
#include "apb_proxy_nw_cmd_util.h"
#include "apb_proxy_nw_coap.h"


log_create_module(COAP, PRINT_LEVEL_INFO);


static nw_coap_context_t g_nw_coap_ctx;

static ATTR_ZIDATA_IN_RETSRAM uint32_t g_nw_coap_retention_mask;
static ATTR_ZIDATA_IN_RETSRAM nw_coap_retention_data_t g_nw_coap_retention_data[COAP_MAX_NUM];

ATTR_NONINIT_DATA_IN_RAM uint8_t g_nw_coap_buf[NW_COAP_MAX_PDU_LENGTH];

uint32_t g_nw_coap_debug_flag = 0;

/* CoAP counter */
uint8_t g_nw_coap_counter = 0;

static TaskHandle_t g_nw_coap_cli_task_hd = NULL;

static void nw_coap_retention_handle(void)
{
    int i = 0;
    coap_dev_info_t *dev = NULL;
    int32_t coap_ret = 0;

    NW_COAP_LOG("retention_handle--mask: 0x%08x", g_nw_coap_retention_mask);

    for (i = 0; i < COAP_CLIENT_MAX_NUM; ++i) {
        if (g_nw_coap_retention_mask & (1 << i)) {
            dev = nw_coap_find_dev(COAP_DEV_TYPE_SET, &i);
            if (!dev) {
                NW_COAP_LOG("retention_handle--find dev fail: %d", i);
                NW_COAP_RESET_FLAG(g_nw_coap_retention_mask, (1 << i));
                continue;
            }

            dev->pdn = g_nw_coap_retention_data[i].pdn;
            /* Restore dst */
            dev->dst.size = sizeof(struct sockaddr_in);
            dev->dst.addr.sin.sin_family = g_nw_coap_retention_data[i].sa_family;
            dev->dst.addr.sin.sin_len = sizeof(struct sockaddr_in);
            dev->dst.addr.sin.sin_port = g_nw_coap_retention_data[i].port;
            dev->dst.addr.sin.sin_addr.s_addr = g_nw_coap_retention_data[i].ip_addr;

            nw_coap_show_dev(dev);

            coap_ret = nw_coap_new_context(dev);

            if (coap_ret != NW_COAP_RET_SUCCESS) {
                nw_coap_retention_reset(dev);
                nw_coap_reset_dev(dev);
                continue;
            }
        }
    }
}


static void nw_coap_cli_task_main(void *arg)
{
    fd_set readfds;
    uint32_t coap_num = 0;
    int32_t i = 0;
    COAP_ID cid = 0;
    coap_dev_info_t *dev = NULL;
    struct timeval tv;
    int32_t result = 0;
    int max_fd = -1;

    nw_coap_context_t *nw_coap_ctx_p = nw_coap_get_ctx();

    dev = (coap_dev_info_t *)arg;

    NW_COAP_LOG("cli_task_main(enter)--dev: 0x%08x, cid: %d, sock: %d, hd: 0x%08x",
                dev, dev->cid, dev->coap_ctx->sockfd, g_nw_coap_cli_task_hd);

    if (nw_coap_ctx_p->flag & NW_COAP_FLAG_DATA_RETENTION) {
        nw_coap_retention_handle();
        /* Reset retention flag */
        NW_COAP_RESET_FLAG(nw_coap_ctx_p->flag, NW_COAP_FLAG_DATA_RETENTION);
    }

    while (1) {
        FD_ZERO(&readfds);
        coap_num = 0;
        nw_coap_disable_log();
        for (i = 0; i < COAP_CLIENT_MAX_NUM; ++i) {
            cid = i + 1;
            dev = nw_coap_find_dev(COAP_DEV_TYPE_CID, &cid);
            if (dev) {
                FD_SET(dev->coap_ctx->sockfd, &readfds);
                ++coap_num;
                if (dev->coap_ctx->sockfd > max_fd) {
                    max_fd = dev->coap_ctx->sockfd;
                }
            }
        }
        nw_coap_enable_log();

        if (coap_num > 0) {
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            result = select(max_fd + 1, &readfds, 0, 0, &tv);
            if (result > 0) {  /* Read from socket readfs */
                nw_coap_disable_log();
                for (i = 0; i < COAP_CLIENT_MAX_NUM; ++i) {
                    cid = i + 1;
                    dev = nw_coap_find_dev(COAP_DEV_TYPE_CID, &cid);
                    if (dev && FD_ISSET(dev->coap_ctx->sockfd, &readfds)) {
                        NW_COAP_LOG("cli_task_main(rd)--cid: %d, fd: %d", dev->cid, dev->coap_ctx->sockfd);
                        coap_read(dev->coap_ctx);       /* Read received data */
                        coap_dispatch(dev->coap_ctx);   /* Dispatch PDUs from receivequeue */
                    }
                }
                nw_coap_enable_log();
            } else if (result == 0) { /* Select tv timeout */
                ;
            } else {
                NW_COAP_LOG("cli_task_main--select: %d", result);
            }
        } else {
            //vTaskDelay(NW_COAP_CLI_TASK_DELAY / portTICK_RATE_MS);
            break;
        }
    }

    NW_COAP_LOG("cli_task_main(delete)");
    g_nw_coap_cli_task_hd = NULL;
    vTaskDelete(NULL);
}


static void nw_coap_srv_task_main(void *arg)
{
    fd_set readfds;
    uint32_t coap_num = 0;
    int32_t i = 0;
    COAP_ID cid = 0;
    coap_dev_info_t *dev = NULL;
    struct timeval tv;
    int32_t result = 0;
    uint32_t loop_count = 0;
    int max_fd = -1;

    nw_coap_context_t *nw_coap_ctx_p = nw_coap_get_ctx();

    dev = (coap_dev_info_t *)arg;

    NW_COAP_LOG("srv_task_main(enter)--dev: 0x%08x, cid: %d, sock: %d",
                dev, dev->cid, dev->coap_ctx->sockfd);

    if (nw_coap_ctx_p->flag & NW_COAP_FLAG_DATA_RETENTION) {
        nw_coap_retention_handle();
        /* Reset retention flag */
        NW_COAP_RESET_FLAG(nw_coap_ctx_p->flag, NW_COAP_FLAG_DATA_RETENTION);
    }

    while (1) {
        ++loop_count;
        if (loop_count >= NW_COAP_SRV_MAX_AGE) {
            ++g_nw_coap_counter;
            loop_count = 0;
        }

        FD_ZERO(&readfds);
        coap_num = 0;
        nw_coap_disable_log();
        for (i = COAP_CLIENT_MAX_NUM; i < COAP_MAX_NUM; ++i) {
            cid = i + 1;
            dev = nw_coap_find_dev(COAP_DEV_TYPE_CID, &cid);
            if (dev) {
                FD_SET(dev->coap_ctx->sockfd, &readfds);
                ++coap_num;
                if (dev->coap_ctx->sockfd > max_fd) {
                    max_fd = dev->coap_ctx->sockfd;
                }
            }
        }
        nw_coap_enable_log();

        if (coap_num > 0) {
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            result = select(max_fd + 1, &readfds, 0, 0, &tv);
            if (result > 0) {  /* Read from socket readfs */
                nw_coap_disable_log();
                for (i = COAP_CLIENT_MAX_NUM; i < COAP_MAX_NUM; ++i) {
                    cid = i + 1;
                    dev = nw_coap_find_dev(COAP_DEV_TYPE_CID, &cid);
                    if (dev && FD_ISSET(dev->coap_ctx->sockfd, &readfds)) {
                        NW_COAP_LOG("srv_task_main(rd)--cid: %d, fd: %d", dev->cid, dev->coap_ctx->sockfd);
                        coap_read(dev->coap_ctx);       /* Read received data */
                        coap_dispatch(dev->coap_ctx);   /* Dispatch PDUs from receivequeue */
                    }
                }
                nw_coap_enable_log();
            } else if (result == 0) { /* Select tv timeout */
                ;
            } else {
                NW_COAP_LOG("srv_task_main--select: %d", result);
            }
        } else {
            vTaskDelay(NW_COAP_SRV_TASK_DELAY / portTICK_RATE_MS);
        }
    }
}


static void nw_coap_msg_hdr(coap_context_t  *ctx,
                            const coap_address_t *remote,
                            coap_pdu_t *sent,
                            coap_pdu_t *received,
                            const coap_tid_t id)
{
    apb_proxy_at_cmd_result_t cmd_result;
    char *data_buf = NULL;
    uint32_t data_len = 0;
    uint32_t fix_len = 32;               /* +ECOAPNMI=x,xxxx,  17+1'\0' */
    uint32_t alloc_size = 0, str_len = 0;
    coap_dev_info_t *dev = NULL;
    apb_proxy_status_t ret = APB_PROXY_STATUS_OK;

    /* Show addr */
    nw_coap_show_addr("msg_hdr", remote);

    nw_coap_show_pdu("msg_hdr", received);

    memset(&cmd_result, 0x00, sizeof(apb_proxy_at_cmd_result_t));

    dev = nw_coap_find_dev(COAP_DEV_TYPE_CTX, ctx);

    if (!dev) {
        NW_COAP_LOG("msg_hdr--can't find coap ctx: 0x%08x, sockfd: %d", ctx, ctx->sockfd);
        return;
    }

    NW_COAP_LOG("msg_hdr--sockfd: %d: cid: %d, len: %d", ctx->sockfd, dev->cid, received->length);
    data_len = received->length;

    alloc_size = fix_len + (data_len << 1);
    data_buf = (char *)g_nw_coap_buf;
    if (!data_buf) {
        NW_COAP_LOG("msg_hdr--alloc data buf error: %d", alloc_size);
        return;
    }

    memset(data_buf, 0x00, alloc_size);

    snprintf(data_buf, alloc_size, "+ECOAPNMI: %d,%d,", dev->cid, (int)data_len);
    str_len = strlen(data_buf);
    _get_data_to_hex(data_buf + str_len, (char *)(received->hdr), data_len);
    data_buf[str_len + (data_len << 1) + 1] = '\0';

    cmd_result.cmd_id = APB_PROXY_INVALID_CMD_ID;
    cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
    cmd_result.pdata = data_buf;
    //cmd_result.length = strlen(data_buf) + 1;
    cmd_result.length = strlen(data_buf);
    ret = apb_proxy_send_at_cmd_result(&cmd_result);
    NW_COAP_LOG("msg_hdr(e)--ret: %d", ret);
}


static void nw_coap_get_hdr(coap_context_t  *ctx, struct coap_resource_t *resource, 
            coap_address_t *peer, coap_pdu_t *request, str *token,
            coap_pdu_t *response) {
    coap_dev_info_t *dev = NULL;
    unsigned char buf[3];

    /* Show addr */
    nw_coap_show_addr("get_hdr", peer);
    /* Show request */
    nw_coap_show_pdu("get_hdr", request);

    dev = nw_coap_find_dev(COAP_DEV_TYPE_CTX, ctx);

    if (!dev) {
        NW_COAP_LOG("get_hdr--can't find coap ctx: 0x%08x, sockfd: %d", ctx, ctx->sockfd);
        return;
    }

    NW_COAP_LOG("get_hdr--sockfd: %d: cid: %d", ctx->sockfd, dev->cid);

    response->hdr->code = COAP_RESPONSE_CODE(205);

    coap_add_option(response, COAP_OPTION_CONTENT_TYPE,
        coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);

    coap_add_option(response, COAP_OPTION_MAXAGE,
        coap_encode_var_bytes(buf, NW_COAP_SRV_MAX_AGE), buf);

    buf[0] = g_nw_coap_counter / 100 + '0';
    buf[1] = g_nw_coap_counter % 100 / 10 + '0';
    buf[2] = g_nw_coap_counter % 10 + '0';

    coap_add_data(response, 3, buf);
}


static void nw_coap_post_hdr(coap_context_t  *ctx, struct coap_resource_t *resource, 
            coap_address_t *peer, coap_pdu_t *request, str *token,
            coap_pdu_t *response) {
    //unsigned char buf[3];

    /* Show request */
    nw_coap_show_pdu("post_hdr", request);
}


static void nw_coap_put_hdr(coap_context_t  *ctx, struct coap_resource_t *resource, 
            coap_address_t *peer, coap_pdu_t *request, str *token,
            coap_pdu_t *response) {
    //unsigned char buf[3];
    size_t size = 0;
    unsigned char *data = NULL;

    /* Show addr */
    nw_coap_show_addr("put_hdr", peer);

    /* Show request */
    nw_coap_show_pdu("put_hdr", request);

    coap_get_data(request, &size, &data);
    nw_coap_dump_buff("put_hdr-data", (char *)data, size);
    if (size == 0x01) {/* 1 bytes */
        response->hdr->code = COAP_RESPONSE_CODE(204);
        /* Modify counter */
        g_nw_coap_counter = data[0];
    } else {
        response->hdr->code = COAP_RESPONSE_CODE(400);
    }
    NW_COAP_LOG("put_hdr--size: %d, counter: 0x%02x", size, g_nw_coap_counter);
}


static void nw_coap_delete_hdr(coap_context_t  *ctx, struct coap_resource_t *resource, 
            coap_address_t *peer, coap_pdu_t *request, str *token,
            coap_pdu_t *response) {
    //coap_dev_info_t *dev = NULL;
    //unsigned char buf[3];

    /* Show request */
    nw_coap_show_pdu("delete_hdr", request);
}


static void nw_coap_init_resource(coap_context_t *coap_ctx)
{
    coap_resource_t *r;

    r = coap_resource_init((unsigned char *)"counter", 7, 0);

    /* Register method hdr */
    coap_register_handler(r, COAP_REQUEST_GET, nw_coap_get_hdr);
    coap_register_handler(r, COAP_REQUEST_POST, nw_coap_post_hdr);
    coap_register_handler(r, COAP_REQUEST_PUT, nw_coap_put_hdr);
    coap_register_handler(r, COAP_REQUEST_DELETE, nw_coap_delete_hdr);

    coap_add_resource(coap_ctx, r);
    nw_coap_dump_buff("init_key", (char *)(r->key), 4);
}


void nw_coap_retention_set(coap_dev_info_t *dev)
{
    int32_t index = dev->cid - 1;

    /* Backup dst */
    g_nw_coap_retention_data[index].pdn = dev->pdn;
    g_nw_coap_retention_data[index].sa_family = dev->dst.addr.sin.sin_family;
    g_nw_coap_retention_data[index].port = dev->dst.addr.sin.sin_port;
    g_nw_coap_retention_data[index].ip_addr = dev->dst.addr.sin.sin_addr.s_addr;

    /* Set retention mask */
    NW_COAP_SET_FLAG(g_nw_coap_retention_mask, (1 << index));
}


void nw_coap_retention_reset(coap_dev_info_t *dev)
{
    NW_COAP_RESET_FLAG(g_nw_coap_retention_mask, (1 << (dev->cid - 1)));
}


int32_t nw_coap_new_context(coap_dev_info_t *dev)
{
    coap_address_t *dst = NULL, coap_addr;
    int32_t ret = 0;

    if (dev->sign & COAP_DEV_SIGN_SRV) {
        dst = &dev->dst;
    } else {
        /* CoAP client addr info */
        dst = &coap_addr;
        inet_pton(AF_INET, "0.0.0.0", &(coap_addr.addr.sin.sin_addr.s_addr));
        coap_addr.size = sizeof(struct sockaddr_in);
        coap_addr.addr.sin.sin_family = AF_INET;
        coap_addr.addr.sin.sin_len = sizeof(struct sockaddr_in);
        coap_addr.addr.sin.sin_port = htons(0);
    }

    dev->coap_ctx = coap_new_context(dst);
    if (!dev->coap_ctx) {
        NW_COAP_LOG("new_context--new coap_ctx fail");
        return NW_COAP_RET_FAIL;
    }

    NW_COAP_LOG("new_context--coap_ctx: 0x%08x, sign: 0x%02x", dev->coap_ctx, dev->sign);

    if (dev->sign & COAP_DEV_SIGN_SRV) {
        /* CoAP server setting */
        nw_coap_init_resource(dev->coap_ctx);
        xTaskCreate(nw_coap_srv_task_main,
                NW_COAP_SRV_TASK_NAME,
                NW_COAP_SRV_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                (void *)dev,
                NW_COAP_SRV_TASK_PRIORITY,
                &(dev->task_hd));
    } else {
        /* CoAP client setting */
        //coap_register_option(dev->coap_ctx, COAP_OPTION_BLOCK2);
        coap_register_response_handler(dev->coap_ctx, nw_coap_msg_hdr);
        if (!g_nw_coap_cli_task_hd) {
            xTaskCreate(nw_coap_cli_task_main,
                    NW_COAP_CLI_TASK_NAME,
                    NW_COAP_CLI_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                    (void *)dev,
                    NW_COAP_CLI_TASK_PRIORITY,
                    &g_nw_coap_cli_task_hd);
        }
    }

    if (dev->pdn != NW_COAP_INVALID_PDN) {
        ret = setsockopt(dev->coap_ctx->sockfd, SOL_SOCKET, SO_BINDTODEVICE, &(dev->pdn), sizeof(dev->pdn));
        if (ret < 0 ) {
            NW_COAP_LOG("new_context--bind dev fail ret: %d", ret);
        }
    }

    return NW_COAP_RET_SUCCESS;
}


void nw_coap_free_context(coap_dev_info_t *dev)
{
    /* Retention handle */
    nw_coap_retention_reset(dev);

    coap_free_context(dev->coap_ctx);
    nw_coap_reset_dev(dev);
}


void nw_coap_show_dev(coap_dev_info_t *dev)
{
    if (dev->coap_ctx) {
        NW_COAP_LOG("show_dev(1)--sign: 0x%02x, cid: %d, pdn: %d, coap_ctx: 0x%08x, sockfd: %d",
            dev->sign, dev->cid, dev->pdn, dev->coap_ctx, dev->coap_ctx->sockfd);
    } else {
        NW_COAP_LOG("show_dev(1)--sign: 0x%02x, cid: %d, pdn: %d",
            dev->sign, dev->cid, dev->pdn);
    }

    NW_COAP_LOG("show_dev(2)--size: %d, family: %d, len: %d, port: %d, addr: 0x%08x",
        dev->dst.size, dev->dst.addr.sin.sin_family, dev->dst.addr.sin.sin_len,
        dev->dst.addr.sin.sin_port, dev->dst.addr.sin.sin_addr.s_addr);
}


uint8_t nw_coap_get_sa_family(char *addr)
{
    uint8_t sa_family = AF_INET;

    return sa_family;
}


int32_t nw_coap_send_request(COAP_ID cid, char *data_buf, uint32_t data_len)
{
    coap_pdu_t *pdu = NULL;
    coap_dev_info_t *dev = NULL;
    coap_tid_t tid = COAP_INVALID_TID;

    pdu = coap_pdu_init(0, 0, 0, data_len);
    if (!pdu) {
        NW_COAP_LOG("send_request--new pdu fail");
        return NW_COAP_RET_FAIL;
    }

    dev = nw_coap_find_dev(COAP_DEV_TYPE_CID, &cid);
    /* Parse data buf */
    pdu->hdr->version = data_buf[0] >> 6;
    pdu->hdr->type = (data_buf[0] >> 4) & 0x03;
    pdu->hdr->token_length = data_buf[0] & 0x0f;
    pdu->hdr->code = data_buf[1];
    memcpy(((unsigned char *)(pdu->hdr) + 4), data_buf + 4, data_len - 4);
    pdu->length = data_len;
    pdu->hdr->id = coap_new_message_id(dev->coap_ctx); /* Generate new msg_id */

    NW_COAP_LOG("send_request--msg_id: %d", pdu->hdr->id);

    nw_coap_show_pdu("send_request", pdu);

    /* Check data vaild */
    if (pdu->hdr->version != COAP_DEFAULT_VERSION) {
        NW_COAP_LOG("send_request--unknown ver: %d", pdu->hdr->version);
        coap_delete_pdu(pdu);
        return NW_COAP_RET_FAIL;
    }

    if (pdu->hdr->code == 0) {
        if (data_len != sizeof(coap_hdr_t) || pdu->hdr->token_length) {
            NW_COAP_LOG("send_request--empty message error");
            coap_delete_pdu(pdu);
            return NW_COAP_RET_FAIL;
        }
    }

    if (pdu->hdr->type == COAP_MESSAGE_CON) {
        tid = coap_send_confirmed(dev->coap_ctx, &dev->dst, pdu);
    } else {
        tid = coap_send(dev->coap_ctx, &dev->dst, pdu);
    }

    if (tid == COAP_INVALID_TID) {
        NW_COAP_LOG("send_request--coap send fail");
        //return NW_COAP_RET_FAIL;
    }

    if (pdu->hdr->type != COAP_MESSAGE_CON || tid == COAP_INVALID_TID) {
        coap_delete_pdu(pdu);
    }

    return NW_COAP_RET_SUCCESS;
}


void nw_coap_reset_dev(coap_dev_info_t *dev)
{
    NW_COAP_LOG("reset_dev--dev: 0x%08x, cid: %d, task_hd: 0x%08x", dev, dev->cid, dev->task_hd);

    if (dev->task_hd) {
        vTaskDelete(dev->task_hd);
    }
    memset(dev, 0x00, sizeof(coap_dev_info_t));
}


nw_coap_context_t *nw_coap_get_ctx()
{
    return &g_nw_coap_ctx;
}


coap_dev_info_t *nw_coap_find_dev(coap_dev_type_t type, void *param)
{
    coap_dev_info_t *dev = NULL;
    int32_t i = 0, num = 0;
    nw_coap_context_t *nw_coap_ctx_p = nw_coap_get_ctx();

    switch (type) {
        case COAP_DEV_TYPE_UNUSED: {
            coap_dev_role_t role = *((coap_dev_role_t *)param);
            i = (role == COAP_DEV_ROLE_SERVER) ? (COAP_CLIENT_MAX_NUM) : (0);
            num = (role == COAP_DEV_ROLE_SERVER) ? (COAP_MAX_NUM) : (COAP_CLIENT_MAX_NUM);
            for (; i < num; ++i) {
                if (!(nw_coap_ctx_p->coap_dev[i].sign & COAP_DEV_SIGN_USED)) {
                    dev = &nw_coap_ctx_p->coap_dev[i];
                    dev->cid = i + 1;
                    NW_COAP_SET_FLAG(dev->sign, COAP_DEV_SIGN_USED);
                    if (dev->cid > COAP_CLIENT_MAX_NUM) { /* CoAP server dev */
                        NW_COAP_SET_FLAG(dev->sign, COAP_DEV_SIGN_SRV);
                    }
                    break;
                }
            }
            break;
        }

        case COAP_DEV_TYPE_SOCKET: {
            int sockfd = *((int *)param);
            for (i = 0; i < COAP_MAX_NUM; ++i) {
                if (sockfd == nw_coap_ctx_p->coap_dev[i].coap_ctx->sockfd) {
                    dev = &nw_coap_ctx_p->coap_dev[i];
                    break;
                }
            }
            break;
        }

        case COAP_DEV_TYPE_CID: {
            COAP_ID cid = *((uint8_t *)param);
            for (i = 0; i < COAP_MAX_NUM; ++i) {
                if (cid == nw_coap_ctx_p->coap_dev[i].cid) {
                    dev = &nw_coap_ctx_p->coap_dev[i];
                    break;
                }
            }
            break;
        }

        case COAP_DEV_TYPE_CTX: {
            coap_context_t *coap_ctx = (coap_context_t *)param;
            for (i = 0; i < COAP_MAX_NUM; ++i) {
                if ((coap_ctx) && (coap_ctx == nw_coap_ctx_p->coap_dev[i].coap_ctx)) {
                    dev = &nw_coap_ctx_p->coap_dev[i];
                    break;
                }
            }
            break;
        }

        case COAP_DEV_TYPE_SET: {
            int32_t index = *((int32_t *)param);
            if (nw_coap_ctx_p->flag & NW_COAP_FLAG_DATA_RETENTION) {
                dev = &nw_coap_ctx_p->coap_dev[index];
                dev->cid = index + 1;
                NW_COAP_SET_FLAG(dev->sign, COAP_DEV_SIGN_USED);
                if (dev->cid > COAP_CLIENT_MAX_NUM) { /* CoAP server dev */
                    NW_COAP_SET_FLAG(dev->sign, COAP_DEV_SIGN_SRV);
                }
            }
            break;
        }

        default:
            break;
    }

    if (nw_coap_ctx_p->enable_log) {
        NW_COAP_LOG("find_dev--type: %d, param: 0x%08x, dev: 0x%08x", type, param, dev);
    }

    return dev;
}


void nw_coap_init(void)
{
    memset(&g_nw_coap_ctx, 0x00, sizeof(nw_coap_context_t));

    NW_COAP_SET_FLAG(g_nw_coap_ctx.flag, NW_COAP_FLAG_INIT);

    /* Log enable */
    g_nw_coap_ctx.enable_log = 1;

    if (rtc_power_on_result_external() != DEEP_SLEEP && rtc_power_on_result_external() != DEEPER_SLEEP) {
        /* COLD-BOOT case: normal init */
        g_nw_coap_retention_mask = 0;
    } else {
        /* DEEP-SLEEP case: data retention process */
        NW_COAP_SET_FLAG(g_nw_coap_ctx.flag, NW_COAP_FLAG_DATA_RETENTION);
    }

#if 0
    xTaskCreate(nw_coap_cli_task_main,
            NW_COAP_CLI_TASK_NAME,
            NW_COAP_CLI_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
            NULL,
            NW_COAP_CLI_TASK_PRIORITY,
            NULL);

    xTaskCreate(nw_coap_srv_task_main,
            NW_COAP_SRV_TASK_NAME,
            NW_COAP_SRV_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
            NULL,
            NW_COAP_SRV_TASK_PRIORITY,
            NULL);
#endif
}


void nw_coap_enable_log(void)
{
    nw_coap_context_t *nw_coap_ctx_p = nw_coap_get_ctx();

    nw_coap_ctx_p->enable_log = 1;
}


void nw_coap_disable_log(void)
{
    nw_coap_context_t *nw_coap_ctx_p = nw_coap_get_ctx();

    nw_coap_ctx_p->enable_log = 0;
}


void nw_coap_dump_buff(char *title, char *buf, uint32_t size)
{
    int32_t i = 0;

    NW_COAP_LOG("%s", title);

    for (i = 0; i < size; ++i) {
        NW_COAP_LOG("buf[%d]: 0x%02x", i, buf[i]);
    }
}


void nw_coap_show_pdu(char *title, const coap_pdu_t *pdu)
{
    //unsigned char buf[COAP_MAX_PDU_SIZE]; /* need some space for output creation */
    //int encode = 0, have_options = 0;
    //coap_opt_iterator_t opt_iter;
    //coap_opt_t *option;

    NW_COAP_LOG("%s--show_pdu", title);

    NW_COAP_LOG("v:%d t:%d tkl:%d c:%d id:%u",
            pdu->hdr->version, pdu->hdr->type,
            pdu->hdr->token_length,
            pdu->hdr->code, ntohs(pdu->hdr->id));

#if 0
    /* show options, if any */
    coap_option_iterator_init((coap_pdu_t *)pdu, &opt_iter, COAP_OPT_ALL);

    while ((option = coap_option_next(&opt_iter))) {
    if (!have_options) {
        have_options = 1;
        fprintf(COAP_DEBUG_FD, " o: [");
    } else {
        fprintf(COAP_DEBUG_FD, ",");
    }

    if (opt_iter.type == COAP_OPTION_URI_PATH ||
        opt_iter.type == COAP_OPTION_PROXY_URI ||
        opt_iter.type == COAP_OPTION_URI_HOST ||
        opt_iter.type == COAP_OPTION_LOCATION_PATH ||
        opt_iter.type == COAP_OPTION_LOCATION_QUERY ||
        opt_iter.type == COAP_OPTION_URI_PATH ||
        opt_iter.type == COAP_OPTION_URI_QUERY) {
        encode = 0;
    } else {
        encode = 1;
    }

    if (print_readable(COAP_OPT_VALUE(option),
            COAP_OPT_LENGTH(option),
            buf, sizeof(buf), encode ))
        fprintf(COAP_DEBUG_FD, " %d:'%s'", opt_iter.type, buf);
    }

    if (have_options)
        fprintf(COAP_DEBUG_FD, " ]");

    if (pdu->data) {
        assert(pdu->data < (unsigned char *)pdu->hdr + pdu->length);
        print_readable(pdu->data,
            (unsigned char *)pdu->hdr + pdu->length - pdu->data,
            buf, sizeof(buf), 0 );
            fprintf(COAP_DEBUG_FD, " d:%s", buf);
    }

    fprintf(COAP_DEBUG_FD, "\n");
    fflush(COAP_DEBUG_FD);
#endif
}


void nw_coap_show_addr(char *title, const coap_address_t *addr)
{
    NW_COAP_LOG("%s--show_addr", title);

    NW_COAP_LOG("sa: %d, addr: 0x%08x, port: %d, len: %d",
                    addr->addr.sin.sin_family,
                    addr->addr.sin.sin_addr.s_addr,
                    addr->addr.sin.sin_port,
                    addr->addr.sin.sin_len);
}

