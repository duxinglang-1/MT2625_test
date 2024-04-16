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

#include "fota_coap_dl.h"
#include "FreeRTOS.h"
#include "string.h"
#include "lwip/sockets.h"
#include "coap.h"
#include "netdb.h"

/*************************************************************************************************
*                             Macro's Definitions                                                *
**************************************************************************************************/
log_create_module(fota_coap_dl, PRINT_LEVEL_INFO);
#define FOTA_COAP_DL_ERR(fmt,arg...)   LOG_E(fota_coap_dl, "[CoAP DL]: "fmt,##arg)
#define FOTA_COAP_DL_WARN(fmt,arg...)  LOG_W(fota_coap_dl, "[CoAP DL]: "fmt,##arg)
#define FOTA_COAP_DL_DBG(fmt,arg...)   LOG_I(fota_coap_dl, "[CoAP DL]: "fmt,##arg)
#define FOTA_COAP_DL_BUF_SIZE     1028 /*Currently ,the MAX packet size is less than 1024.*/
#define min(a,b) ((a) < (b) ? (a) : (b))

/*************************************************************************************************
*                             Internal Type Definitions                                          *
**************************************************************************************************/
typedef struct {
    void* this;
    fota_coap_dl_t downloader_handler;
    char* p_coap_url;
    fota_coap_dl_event_call_back_t caller_callback;
    bool cancelled;
    uint32_t download_buf_len;
    uint8_t* p_download_buf;
}fota_coap_dl_context_t;

typedef uint8_t method_t;
/*************************************************************************************************
*                             Static Variants Definitions                                        *
**************************************************************************************************/
static uint32_t    fota_coap_wait_seconds = 90; /* default timeout in seconds */
static coap_tick_t fota_coap_max_wait     = 0; /* global timeout (changed by set_timeout()) */
static uint32_t    fota_coap_obs_seconds  = 30; /* default observe time */
static coap_tick_t fota_coap_obs_wait     = 0; /* timeout for current subscription */
static uint8_t fota_coap_token_data[32] = {0};
static str fota_coap_token = { 0, fota_coap_token_data };
static str fota_coap_payload = { 0, NULL }; /* optional payload to send */
static coap_block_t fota_coap_block = { .num = 0, .m = 0, .szx = 6 };
static coap_list_t *coap_optlist = NULL;
static bool coap_running = true;
static fota_coap_dl_event_call_back_t fota_coap_call_back = NULL;
static fota_coap_dl_result_t coap_last_error = FOTA_COAP_DL_OK;
/*************************************************************************************************
*                             Local Function's Definitions                                       *
**************************************************************************************************/
static bool fota_coap_dl_set_parameter(void* self, const char* p_url,
                                       fota_coap_dl_event_call_back_t event_callback);
static fota_coap_dl_result_t fota_coap_dl_run(void* self);
static void fota_coap_dl_cancel(void* self);
static fota_coap_dl_result_t fota_coap_dl_run_internal(fota_coap_dl_context_t* p_downloader);
static void fota_coap_dl_option_list(coap_list_t *options);
static void fota_coap_dl_set_token(uint32_t len, const unsigned char *data);
/*************************************************************************************************
*                             Public Function's Implementation                                   *
**************************************************************************************************/
/*************************************************************************************************
 * @brief      Init the FOTA COAP downloader.
 * @param[in]  None
 * @return     When the FOTA COAP downloader is created successfully, none-NULL value will be returned.
 *************************************************************************************************/
fota_coap_dl_t* fota_coap_dl_init(void)
{
    fota_coap_dl_context_t* p_context = (fota_coap_dl_context_t *)pvPortMalloc(sizeof(fota_coap_dl_context_t));
    configASSERT(p_context != NULL);
    memset(p_context, 0, sizeof(fota_coap_dl_context_t));
    p_context->this = (void*)p_context;
    p_context->downloader_handler.fota_coap_dl_cancel = fota_coap_dl_cancel;
    p_context->downloader_handler.fota_coap_dl_run = fota_coap_dl_run;
    p_context->downloader_handler.fota_coap_dl_set_parameter = fota_coap_dl_set_parameter;
    p_context->cancelled = false;
    p_context->p_coap_url = NULL;
    return &(p_context->downloader_handler);
}
/*************************************************************************************************
 * @brief      Destory the FOTA COAP downloader when the download completes.
 * @param[in]  self  - The pointer points to fota_coap_dl_t, allocated by fota_coap_dl_init().
 * @return     None
 *************************************************************************************************/
void fota_coap_dl_destroy(fota_coap_dl_t** self)
{
    fota_coap_dl_context_t* p_downloader = NULL;
    configASSERT(self != NULL);
    configASSERT(*self != NULL);
    p_downloader = (fota_coap_dl_context_t*)((uint8_t*)(*self) -sizeof(void*));

    if (p_downloader == NULL){
        configASSERT(0);
    }

    if(p_downloader->this != p_downloader){
        configASSERT(0);
    }

    if (p_downloader->p_coap_url != NULL){
        vPortFree(p_downloader->p_coap_url);
        p_downloader->p_coap_url = NULL;
    }
    vPortFree(p_downloader);
    *self = NULL;
}
/*************************************************************************************************
*                             Local Function's Implementation                                    *
**************************************************************************************************/
/*************************************************************************************************
 * @brief      Set the related paramters for FOTA COAP downloader.
 * @param[in]  self  - The pointer points to fota_coap_dl_t, allocated by fota_coap_dl_init().
 * @param[in]  p_url - Points to the URL which the FOTA file locates at on the remote server.
 * @param[in]  event_callback - The event call back funtion implemented by the user.
 * @return     When the parameter is set successfully, true will be returned.
 ************************************************************************************************/
static bool fota_coap_dl_set_parameter(void* self, const char* p_url,
                                       fota_coap_dl_event_call_back_t event_callback)
{
    uint32_t url_len = 0;
    fota_coap_dl_context_t* p_downloader = NULL;
    configASSERT(self != NULL);
    configASSERT(p_url != NULL);
    configASSERT(event_callback != NULL);
    p_downloader = (fota_coap_dl_context_t*)((uint8_t*)(self) -sizeof(void*));

    if (p_downloader->this != p_downloader){
        return false;
    }
    /*The user wants to use another url to download.*/
    if (p_downloader->p_coap_url != NULL){
        vPortFree(p_downloader->p_coap_url);
        p_downloader->p_coap_url = NULL;
    }

    url_len = strlen(p_url) + 1;
    p_downloader->p_coap_url = (char*)pvPortMalloc(url_len);
    if (NULL == p_downloader->p_coap_url){
        return false;
    }
    memcpy(p_downloader->p_coap_url, p_url, url_len);
    p_downloader->caller_callback = event_callback;
    fota_coap_call_back = event_callback;
    return true;
}
/*************************************************************************************************
 * @brief      When the function is called, FOTA COAP downloader will begin to download the data.
 * @param[in]  self  - The pointer points to fota_coap_dl_t, allocated by fota_coap_dl_init().
 * @return     FOTA COAP downloader final result.
 *************************************************************************************************/
static fota_coap_dl_result_t fota_coap_dl_run(void* self)
{
    fota_coap_dl_context_t* p_downloader = NULL;
    uint8_t* p_download_buf = NULL;
    fota_coap_dl_result_t result = FOTA_COAP_DL_ERROR;
    configASSERT(self != NULL);

    p_downloader = (fota_coap_dl_context_t*)((uint8_t*)(self) -sizeof(void*));
    configASSERT(p_downloader->this == p_downloader);

    FOTA_COAP_DL_DBG("COAP URI::%s", p_downloader->p_coap_url);
    p_download_buf = (uint8_t *)pvPortMalloc(FOTA_COAP_DL_BUF_SIZE);
    if (NULL == p_download_buf){
        return FOTA_COAP_DL_NO_ENOUGH_MEMORY;
    }
    p_downloader->p_download_buf = p_download_buf;
    p_downloader->download_buf_len = FOTA_COAP_DL_BUF_SIZE;
    result = fota_coap_dl_run_internal(p_downloader);
    FOTA_COAP_DL_DBG("COAP download result code: %d", result);
    vPortFree(p_download_buf);
    p_download_buf = NULL;
    p_downloader->p_download_buf = NULL;
    p_downloader->download_buf_len = 0;
    return result;
}
/*************************************************************************************************
 * @brief      When the FOTA COAP downloader is in another task, the caller can try to call this
 *             function to cancel the download process.
 * @param[in]  self  - The pointer points to fota_coap_dl_t, allocated by fota_coap_dl_init().
 * @return     None
 *************************************************************************************************/
static void fota_coap_dl_cancel(void* self)
{
    fota_coap_dl_context_t* p_downloader = NULL;
    configASSERT(self != NULL);
    p_downloader = (fota_coap_dl_context_t*)((uint8_t*)(self) -sizeof(void*));
    configASSERT(p_downloader->this == p_downloader);
    /* Set the cancel flag for cancel the downloading process. */
    p_downloader->cancelled = true;
}

static void fota_coap_dl_set_token(uint32_t len, const unsigned char *data)
{
    FOTA_COAP_DL_DBG("set token..\r\n");
    memset(fota_coap_token_data, 0, sizeof(fota_coap_token_data));
    memcpy(fota_coap_token_data, data, len);
    fota_coap_token.length = len;
    FOTA_COAP_DL_DBG("The token is : %s\r\n", fota_coap_token_data);
}

static void fota_coap_dl_option_list(coap_list_t *options)
{
    coap_list_t *opt;
    for (opt = options; opt; opt = opt->next) {
        FOTA_COAP_DL_DBG("dump options, type:%d.\r\n", COAP_OPTION_KEY(*(coap_option *)opt->data));
    }
}

static coap_pdu_t* fota_coap_dl_coap_new_request(coap_context_t *ctx, method_t m, coap_list_t *options , bool add_block)
{
    coap_pdu_t *pdu = NULL;
    coap_list_t *opt;
    uint8_t msgtype = COAP_MESSAGE_CON;
    uint8_t token[8] = {0};
    uint16_t radom_value = 0;

    prng((unsigned char *)&radom_value, sizeof(radom_value));

    if ( ! ( pdu = coap_new_pdu() ) ) {
        return NULL;
    }
    pdu->hdr->type = msgtype;
    pdu->hdr->id = coap_new_message_id(ctx);
    pdu->hdr->code = m;
    token[0] = 0xAB;
    token[1] = (pdu->hdr->id) >> 8;
    token[2] = 0xBA;
    token[3] = (pdu->hdr->id) & 0xFF;
    token[4] = 0xCD;
    token[5] = (radom_value) >> 8;
    token[6] = 0xDC;
    token[7] = (radom_value) & 0xFF;

    fota_coap_dl_set_token(sizeof(token), token);

    pdu->hdr->token_length = fota_coap_token.length;
    if ( !coap_add_token(pdu, fota_coap_token.length, fota_coap_token.s)) {
        FOTA_COAP_DL_DBG("cannot add token to request\r\n");
    }

    //coap_show_pdu(pdu);
    FOTA_COAP_DL_DBG("creat coap new request\r\n");
    for (opt = options; opt; opt = opt->next) {
        FOTA_COAP_DL_DBG("added option, type:%d.\r\n", COAP_OPTION_KEY(*(coap_option *)opt->data));
        coap_add_option(pdu, COAP_OPTION_KEY(*(coap_option *)opt->data),
                        COAP_OPTION_LENGTH(*(coap_option *)opt->data),
                        COAP_OPTION_DATA(*(coap_option *)opt->data));
    }

    if (fota_coap_payload.length) {
        if (add_block == false)
            coap_add_data(pdu, fota_coap_payload.length, fota_coap_payload.s);
        else
            coap_add_block(pdu, fota_coap_payload.length, fota_coap_payload.s,
                           fota_coap_block.num, fota_coap_block.szx);
    }
    return pdu;
}

static int fota_coap_dl_check_token(coap_pdu_t *received)
{
    return received->hdr->token_length == fota_coap_token.length &&
    memcmp(received->hdr->token, fota_coap_token.s, fota_coap_token.length) == 0;
}

static void fota_coap_dl_set_timeout(coap_tick_t *timer, const uint32_t seconds)
{
    coap_ticks(timer);
    *timer += seconds * COAP_TICKS_PER_SECOND;
}

static coap_opt_t* fota_coap_dl_get_block(coap_pdu_t *pdu, coap_opt_iterator_t *opt_iter)
{
    coap_opt_filter_t f;
    memset(f, 0, sizeof(coap_opt_filter_t));
    coap_option_setb(f, COAP_OPTION_BLOCK1);
    coap_option_setb(f, COAP_OPTION_BLOCK2);
    coap_option_iterator_init(pdu, opt_iter, f);
    return coap_option_next(opt_iter);
}


static void fota_coap_dl_message_handler(struct coap_context_t  *ctx,
                                         const coap_address_t *remote,
                                         coap_pdu_t *sent,
                                         coap_pdu_t *received,
                                         const coap_tid_t id)
{
    coap_pdu_t *pdu = NULL;
    coap_opt_t *block_opt;
    coap_opt_iterator_t opt_iter;
    unsigned char buf[4];
    coap_list_t *option;
    uint32_t len;
    unsigned char *databuf;
    coap_tid_t tid;
    method_t method = COAP_REQUEST_GET;
    coap_list_t *optlist = coap_optlist;

    FOTA_COAP_DL_DBG("fota coap dl process incoming %d.%02d response:\r\n",
                     (received->hdr->code >> 5), received->hdr->code & 0x1F);;

    /* check if this is a response to our original request */
    if (!fota_coap_dl_check_token(received)) {
        /* drop if this was just some message, or send RST in case of notification */
        if (!sent && (received->hdr->type == COAP_MESSAGE_CON ||
            received->hdr->type == COAP_MESSAGE_NON)) {
            coap_send_rst(ctx, remote, received);
        }
        FOTA_COAP_DL_DBG("drop the coap msg not for us \r\n");
        return;
    }

    if (received->hdr->type == COAP_MESSAGE_RST) {
        FOTA_COAP_DL_DBG("got RST\r\n");
        coap_last_error = FOTA_COAP_DL_SERVER_RSP_ERROR;
        return;
    }

    /* output the received data, if any */
    if (received->hdr->code == COAP_RESPONSE_CODE(205)) {

        /* set obs timer if we have successfully subscribed a resource */
        if (sent && coap_check_option(received, COAP_OPTION_SUBSCRIPTION, &opt_iter)) {
            FOTA_COAP_DL_DBG("observation relationship established, set timeout to %d\n", fota_coap_obs_seconds);
            fota_coap_dl_set_timeout(&fota_coap_obs_wait, fota_coap_obs_seconds);
        }

        /* Got some data, check if block option is set. Behavior is undefined if
         * both, Block1 and Block2 are present. */
        block_opt = fota_coap_dl_get_block(received, &opt_iter);
        if (!block_opt) {
            /* There is no block option set, just read the data and we are done. */
            if (coap_get_data(received, &len, &databuf)) {
                FOTA_COAP_DL_DBG("data received: len=%d\r\n", len);
                coap_last_error = FOTA_COAP_DL_OK;
                coap_running = false;
            }
        } else {
            unsigned short blktype = opt_iter.type;
            FOTA_COAP_DL_DBG("FOTA CoAP received block data \r\n");
            /* TODO: check if we are looking at the correct block number */
            if (coap_get_data(received, &len, &databuf)) {
                FOTA_COAP_DL_DBG("FOTA CoAP received block data, len;%d\r\n", len);
                //append_to_output(databuf, len);
                if ((fota_coap_call_back != NULL) && (databuf != NULL) && (len > 0)) {
                    fota_coap_dl_data_event_t data_event;
                    FOTA_COAP_DL_DBG(" FOTA CoAP event call back \r\n");
                    data_event.p_buf = databuf;
                    data_event.buf_len = len;
                    fota_coap_call_back(FOTA_COAP_DL_DATA_EVENT, &data_event);
                }
            }

            if (COAP_OPT_BLOCK_MORE(block_opt)) {
                /* more bit is set */
                FOTA_COAP_DL_DBG("found the M bit, block size is %u, block nr. %u\n",
                COAP_OPT_BLOCK_SZX(block_opt), coap_opt_block_num(block_opt));

                /* create pdu with request for next block */
                pdu = fota_coap_dl_coap_new_request(ctx, method, NULL, false); /* first, create bare PDU w/o any option  */
                if ( pdu ) {
                    /* add URI components from optlist */
                    for (option = optlist; option; option = option->next ) {
                        switch (COAP_OPTION_KEY(*(coap_option *)option->data)) {
                            case COAP_OPTION_URI_HOST :
                            case COAP_OPTION_URI_PORT :
                            case COAP_OPTION_URI_PATH :
                            case COAP_OPTION_URI_QUERY :
                                coap_add_option ( pdu, COAP_OPTION_KEY(*(coap_option *)option->data),
                                                  COAP_OPTION_LENGTH(*(coap_option *)option->data),
                                                  COAP_OPTION_DATA(*(coap_option *)option->data) );
                                break;
                            default:
                                break;
                        }
                    }

                    /* finally add updated block option from response, clear M bit */
                    /* blocknr = (blocknr & 0xfffffff7) + 0x10; */
                    FOTA_COAP_DL_DBG("query block %d, block type: %d\r\n", (coap_opt_block_num(block_opt) + 1), blktype);
                    coap_add_option(pdu, blktype, coap_encode_var_bytes(buf,
                                    ((coap_opt_block_num(block_opt) + 1) << 4) |
                                     COAP_OPT_BLOCK_SZX(block_opt)), buf);

                    if (received->hdr->type == COAP_MESSAGE_CON)
                        tid = coap_send_confirmed(ctx, remote, pdu);
                    else
                        tid = coap_send(ctx, remote, pdu);

                    if (tid == COAP_INVALID_TID) {
                        FOTA_COAP_DL_DBG("message_handler: error sending new request");
                        coap_delete_pdu(pdu);
                        coap_last_error = FOTA_COAP_DL_ERROR;
                        coap_running = false;
                    } else {
                        fota_coap_dl_set_timeout(&fota_coap_max_wait, fota_coap_wait_seconds);
                        if (received->hdr->type != COAP_MESSAGE_CON)
                            coap_delete_pdu(pdu);
                    }
                    return;
                }
            } else {
                FOTA_COAP_DL_DBG("==== FOTA CoAP download completed =======\r\n");
                coap_last_error = FOTA_COAP_DL_OK;
                coap_running = false;
            }
        }
    } else {			/* no 2.05 */

        /* check if an error was signaled and output payload if so */
        if (COAP_RESPONSE_CLASS(received->hdr->code) >= 4) {
            FOTA_COAP_DL_DBG("FOTA CoAP error: %d.%02d",
                             (received->hdr->code >> 5), received->hdr->code & 0x1F);
            if (coap_get_data(received, &len, &databuf)) {
                while(len--)
                    FOTA_COAP_DL_DBG("%c", *databuf++);
            }
            FOTA_COAP_DL_DBG("\n");
            coap_last_error = FOTA_COAP_DL_SERVER_RSP_ERROR;
            coap_running = false;
        }
    }

    /* finally send new request, if needed */
    if (pdu && coap_send(ctx, remote, pdu) == COAP_INVALID_TID) {
        FOTA_COAP_DL_DBG("message_handler: error sending response");
        coap_last_error = FOTA_COAP_DL_ERROR;
        coap_running = false;
    }
    coap_delete_pdu(pdu);

    /* our job is done, we can exit at any time */
//    ready = coap_check_option(received, COAP_OPTION_SUBSCRIPTION, &opt_iter) == NULL;
}

static coap_context_t* foa_coap_dl_get_coap_context(const char *node, const char *port)
{
    coap_context_t *ctx = NULL;
    int s;
    struct addrinfo hints;
    struct addrinfo* result = NULL;
    struct addrinfo* rp = NULL;

    FOTA_COAP_DL_DBG("foa_coap_dl_get_coap_context\r\n");
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Coap uses UDP */
    hints.ai_flags = AI_PASSIVE | AI_NUMERICHOST | AI_NUMERICSERV | AI_ALL;

    s = getaddrinfo(node, NULL, &hints, &result);
    if ( s != 0 ) {
        FOTA_COAP_DL_DBG("getaddrinfo: error:%d\r\n", s);
        return NULL;
    }
    FOTA_COAP_DL_DBG("getaddrinfo: result:0x%x\r\n", result);
    /* iterate through results until success */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        coap_address_t addr;
        FOTA_COAP_DL_DBG("rp->ai_addrlen:%d\r\n", rp->ai_addrlen);
        if (rp->ai_addrlen <= sizeof(addr.addr)) {
            coap_address_init(&addr);
            addr.size = rp->ai_addrlen;
            memcpy(&addr.addr, rp->ai_addr, rp->ai_addrlen);
            ctx = coap_new_context(&addr);
            if (ctx) {
                goto finish;
            }
        }
    }

    finish:
        freeaddrinfo(result);
    return ctx;
}

static int fota_coap_dl_resolve_address(char* server, struct sockaddr *dst)
{
    struct addrinfo *res, *ainfo;
    struct addrinfo hints;
    int error, len=-1;
    char port[12] = {0};

    FOTA_COAP_DL_DBG("DNS resolve server: %s\r\n", server);
    memset ((char *)&hints, 0, sizeof(hints));
    //hints.ai_socktype = SOCK_DGRAM;
    //hints.ai_family = AF_UNSPEC;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    snprintf(port, sizeof(port), "%d", 80);

    error = getaddrinfo(server, port, &hints, &res);

    if (error != 0) {
        FOTA_COAP_DL_DBG("DNS resolve server error:%d\r\n", error);
        return error;
    }

    for (ainfo = res; ainfo != NULL; ainfo = ainfo->ai_next) {
        switch (ainfo->ai_family) {
            case AF_INET6:
            case AF_INET:
                len = ainfo->ai_addrlen;
                memcpy(dst, ainfo->ai_addr, len);
                FOTA_COAP_DL_DBG("DNS resolve server OK\r\n");
                goto finish;
            default:
            ;
        }
    }

    finish:
    freeaddrinfo(res);
    return len;
}

static int fota_coap_dl_order_opts(void *a, void *b)
{
    if (!a || !b)
        return a < b ? -1 : 1;

    if (COAP_OPTION_KEY(*(coap_option *)a) < COAP_OPTION_KEY(*(coap_option *)b))
        return -1;

    return COAP_OPTION_KEY(*(coap_option *)a) == COAP_OPTION_KEY(*(coap_option *)b);
}

static coap_list_t* fota_coap_dl_new_option_node(uint16_t key, uint32_t length, uint8_t *data)
{
    coap_option *option;
    coap_list_t *node;

    option = coap_malloc(sizeof(coap_option) + length);
    if ( !option )
        goto error;

    COAP_OPTION_KEY(*option) = key;
    COAP_OPTION_LENGTH(*option) = length;
    memcpy(COAP_OPTION_DATA(*option), data, length);

    /* we can pass NULL here as delete function since option is released automatically  */
    node = coap_new_listnode(option, NULL);

    if ( node )
        return node;

    error:
        coap_free( option );
    return NULL;
}

/*Split the URI information into URI options.*/
static void fota_coap_dl_parse_uri(coap_list_t **optlist, char* p_uri, coap_uri_t* p_coap_uri)
{
    #define BUFSIZE 256
    uint8_t portbuf[2] = {0};
    uint8_t _buf[BUFSIZE];
    uint8_t *buf = _buf;
    uint32_t buflen = 0;
    int32_t res;
    int32_t insert_result = 0;

    coap_split_uri((unsigned char *)p_uri, strlen(p_uri), p_coap_uri);
    FOTA_COAP_DL_DBG("CoAP port:%d\r\n", p_coap_uri->port);
    if (p_coap_uri->port != COAP_DEFAULT_PORT) {
        insert_result = coap_insert( optlist,
                        fota_coap_dl_new_option_node(COAP_OPTION_URI_PORT,
                        coap_encode_var_bytes(portbuf, p_coap_uri->port), portbuf),
                        fota_coap_dl_order_opts);
        if (insert_result == 0) {
            FOTA_COAP_DL_DBG(" insert port option failed.\r\n");
        }
    }
    FOTA_COAP_DL_DBG("CoAP parse, path:%s, length;%d\r\n", p_coap_uri->path.s, p_coap_uri->path.length);
    if (p_coap_uri->path.length) {
        buflen = BUFSIZE;
        res = coap_split_path(p_coap_uri->path.s, p_coap_uri->path.length, buf, &buflen);
        FOTA_COAP_DL_DBG("coap split path: res=%d \r\n", res);
        while (res--) {
            FOTA_COAP_DL_DBG("insert path\r\n");
            insert_result = coap_insert(optlist, fota_coap_dl_new_option_node(COAP_OPTION_URI_PATH,
                                                               COAP_OPT_LENGTH(buf),
                                                               COAP_OPT_VALUE(buf)),
                                        fota_coap_dl_order_opts);
            if (insert_result == 0) {
                FOTA_COAP_DL_DBG(" insert path option failed.\r\n");
            }
            buf += COAP_OPT_SIZE(buf);
        }
    }

    FOTA_COAP_DL_DBG("CoAP parse, query:%s, length;%d\r\n", p_coap_uri->query.s, p_coap_uri->query.length);

    if (p_coap_uri->query.length) {
        buflen = BUFSIZE;
        buf = _buf;
        res = coap_split_query(p_coap_uri->query.s, p_coap_uri->query.length, buf, &buflen);
        while (res--) {
            FOTA_COAP_DL_DBG("insert query\r\n");
            coap_insert(optlist, fota_coap_dl_new_option_node(COAP_OPTION_URI_QUERY,
                                                  COAP_OPT_LENGTH(buf),
                                                  COAP_OPT_VALUE(buf)),
                        fota_coap_dl_order_opts);

            buf += COAP_OPT_SIZE(buf);
        }
    }
}

static void fota_coap_dl_set_blocksize(coap_block_t* p_coap_block)
{
    unsigned char buf[4] = {0};
    unsigned short opt;
    method_t method = COAP_REQUEST_GET;
    FOTA_COAP_DL_DBG("set block size\r\n");

    if (method != COAP_REQUEST_DELETE) {
        opt = method == COAP_REQUEST_GET ? COAP_OPTION_BLOCK2 : COAP_OPTION_BLOCK1;
        coap_insert(&coap_optlist, fota_coap_dl_new_option_node(opt,
                    coap_encode_var_bytes(buf, (p_coap_block->num << 4 | p_coap_block->szx)), buf),
                    fota_coap_dl_order_opts);
    }
}

static coap_tid_t fota_coap_dl_clear_obs(coap_context_t *ctx, const coap_address_t *remote)
{
    coap_pdu_t *pdu;
    coap_tid_t tid = COAP_INVALID_TID;
    uint8_t msgtype = COAP_MESSAGE_CON;

    /* create bare PDU w/o any option  */
    pdu = coap_pdu_init(msgtype, COAP_RESPONSE_CODE(731),
                        coap_new_message_id(ctx),
                        sizeof(coap_hdr_t) + fota_coap_token.length);

    if (!pdu) {
        return tid;
    }

    if (!coap_add_token(pdu, fota_coap_token.length, fota_coap_token.s)) {
        coap_delete_pdu(pdu);
        return tid;
     }
    // coap_show_pdu(pdu);

    if (pdu->hdr->type == COAP_MESSAGE_CON)
        tid = coap_send_confirmed(ctx, remote, pdu);
    else
        tid = coap_send(ctx, remote, pdu);

    if (tid == COAP_INVALID_TID) {
        coap_delete_pdu(pdu);
    } else if (pdu->hdr->type != COAP_MESSAGE_CON)
        coap_delete_pdu(pdu);

    return tid;
}


static fota_coap_dl_result_t fota_coap_dl_run_internal(fota_coap_dl_context_t* p_downloader)
{
    coap_context_t* ctx = NULL;
    coap_address_t dst;
    char addr[INET6_ADDRSTRLEN] = {0};
    void *addrptr = NULL;
    fd_set readfds;
    struct timeval tv;
    int result;
    coap_tick_t now;
    coap_queue_t *nextpdu;
    coap_pdu_t  *pdu;
    char server[256] = {0};
    unsigned short port = COAP_DEFAULT_PORT;
    int opt, res;
    coap_tid_t tid = COAP_INVALID_TID;
    coap_uri_t coap_uri = {0};
    char port_str[32] = "0";
    method_t method = COAP_REQUEST_GET;
    fota_coap_dl_parse_uri(&coap_optlist, p_downloader->p_coap_url,
                           &coap_uri);
    memcpy(server, coap_uri.host.s, coap_uri.host.length);
    port = coap_uri.port;
    /* resolve destination address where server should be sent */
    res = fota_coap_dl_resolve_address(server, &dst.addr.sa);

    if (res < 0) {
        FOTA_COAP_DL_DBG("failed to resolve address\r\n");
        coap_last_error = FOTA_COAP_DL_RESOLVE_DNS_ERROR;
        return FOTA_COAP_DL_RESOLVE_DNS_ERROR;
    } else {
        FOTA_COAP_DL_DBG("succeed to resolve address\r\n");
    }

    dst.size = res;
    dst.addr.sin.sin_port = htons(port);

    /* add Uri-Host if server address differs from uri.host */
    switch (dst.addr.sa.sa_family) {
        case AF_INET:
            addrptr = &dst.addr.sin.sin_addr;
            dst.size = sizeof(struct sockaddr_in);
            dst.addr.sin.sin_len = sizeof(struct sockaddr_in);
            /* create context for IPv4 */
            ctx = foa_coap_dl_get_coap_context("0.0.0.0", port_str);
            break;
        case AF_INET6:
            addrptr = &dst.addr.sin6.sin6_addr;
            /* create context for IPv6 */
            ctx = foa_coap_dl_get_coap_context("::", port_str);
            break;
        default:
            break;
    }
    if (!ctx) {
        FOTA_COAP_DL_DBG("failed to create context\r\n");
        coap_last_error = FOTA_COAP_DL_ERROR;
        return FOTA_COAP_DL_ERROR;
    }

    coap_register_option(ctx, COAP_OPTION_BLOCK2);
    coap_register_response_handler(ctx, fota_coap_dl_message_handler);

    /* construct CoAP message */

    if (addrptr && (inet_ntop(dst.addr.sa.sa_family, addrptr, addr, sizeof(addr)) != 0)
        && (strlen(addr) != coap_uri.host.length
        || memcmp(addr, coap_uri.host.s, coap_uri.host.length) != 0)) {
        /* add Uri-Host */
        FOTA_COAP_DL_DBG("insert URI Host\r\n");
        coap_insert(&coap_optlist, fota_coap_dl_new_option_node(COAP_OPTION_URI_HOST, coap_uri.host.length, coap_uri.host.s),
                    fota_coap_dl_order_opts);
    }

    if (! (pdu = fota_coap_dl_coap_new_request(ctx, method, coap_optlist, false))) {
        coap_last_error = FOTA_COAP_DL_ERROR;
        return FOTA_COAP_DL_ERROR;
    }

    if (pdu->hdr->type == COAP_MESSAGE_CON) {
        FOTA_COAP_DL_DBG("coap send confirmed message\r\n");
        tid = coap_send_confirmed(ctx, &dst, pdu);
    }
    else {
        tid = coap_send(ctx, &dst, pdu);
    }

    if (pdu->hdr->type != COAP_MESSAGE_CON || tid == COAP_INVALID_TID) {
        coap_delete_pdu(pdu);
    }

    fota_coap_dl_set_timeout(&fota_coap_max_wait, fota_coap_wait_seconds);
    FOTA_COAP_DL_DBG("timeout is set to %d seconds\n", fota_coap_wait_seconds);

    while ( coap_running == true ) {
        FD_ZERO(&readfds);
        FD_SET( ctx->sockfd, &readfds );
        nextpdu = coap_peek_next( ctx );
        coap_ticks(&now);
        while (nextpdu && nextpdu->t <= now - ctx->sendqueue_basetime) {
            coap_retransmit( ctx, coap_pop_next( ctx ));
            nextpdu = coap_peek_next( ctx );
        }

        if (nextpdu && nextpdu->t < min(fota_coap_obs_wait ? fota_coap_obs_wait : fota_coap_max_wait, fota_coap_max_wait) - now) {
            /* set timeout if there is a pdu to send */
            tv.tv_usec = ((nextpdu->t) % COAP_TICKS_PER_SECOND) * 1000000 / COAP_TICKS_PER_SECOND;
            tv.tv_sec = (nextpdu->t) / COAP_TICKS_PER_SECOND;
        } else {
            /* check if obs_wait fires before max_wait */
            if (fota_coap_obs_wait && fota_coap_obs_wait < fota_coap_max_wait) {
                tv.tv_usec = ((fota_coap_obs_wait - now) % COAP_TICKS_PER_SECOND) * 1000000 / COAP_TICKS_PER_SECOND;
                tv.tv_sec = (fota_coap_obs_wait - now) / COAP_TICKS_PER_SECOND;
            } else {
                tv.tv_usec = ((fota_coap_max_wait - now) % COAP_TICKS_PER_SECOND) * 1000000 / COAP_TICKS_PER_SECOND;
                tv.tv_sec = (fota_coap_max_wait - now) / COAP_TICKS_PER_SECOND;
            }
        }

        result = select(ctx->sockfd + 1, &readfds, 0, 0, &tv);
        if ( result < 0 ) {/* error */
            FOTA_COAP_DL_DBG("FOTA coap select error \r\n");
        } else if ( result > 0 ) {/* read from socket */
            if ( FD_ISSET( ctx->sockfd, &readfds ) ) {
                FOTA_COAP_DL_DBG("FOTA coap socket:%d, received data\r\n");
                coap_read( ctx );/* read received data */
                coap_dispatch( ctx );	/* and dispatch PDUs from receivequeue */
            }
        } else { /* timeout */
            FOTA_COAP_DL_DBG("FOTA coap time out");
            coap_ticks(&now);
            if (fota_coap_max_wait <= now) {
                FOTA_COAP_DL_DBG(" coap timeout, exit\n");
                coap_last_error = FOTA_COAP_DL_TIMEOUT;
                break;
            }
            if (fota_coap_obs_wait && fota_coap_obs_wait <= now) {
                FOTA_COAP_DL_DBG("clear observation relationship\n");
                fota_coap_dl_clear_obs(ctx, &dst); /* FIXME: handle error case COAP_TID_INVALID */
                /* make sure that the obs timer does not fire again */
                fota_coap_obs_wait = 0;
                fota_coap_obs_seconds = 0;
            }
        }
    }
    coap_free_context( ctx );
    coap_delete(coap_optlist);
    coap_optlist = NULL;
    return coap_last_error;
}
