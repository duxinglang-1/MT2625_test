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

#include "fota_http_dl.h"
#include "FreeRTOS.h"
#include "string.h"
#include "httpclient.h"
/*************************************************************************************************
*                             Macro's Definitions                                                *
**************************************************************************************************/
log_create_module(fota_http_dl, PRINT_LEVEL_INFO);
#define FOTA_DL_ERR(fmt,arg...)   LOG_E(fota_http_dl, fmt,##arg)
#define FOTA_DL_WARN(fmt,arg...)  LOG_W(fota_http_dl, fmt,##arg)
#define FOTA_DL_DBG(fmt,arg...)   LOG_I(fota_http_dl, fmt,##arg)
#define FOTA_HTTP_DL_BUF_SIZE     516 /*Currently ,the MAX packet size is less than 516.*/
#define FOTA_DL_RECV_TIMEOUT      180 /*The unit is second.*/
/*************************************************************************************************
*                             Internal Type Definitions                                          *
**************************************************************************************************/
typedef struct {
    void* this;
    fota_http_dl_t downloader_handler;
    char* p_http_url;
    fota_http_dl_event_call_back_t caller_callback;
    bool cancelled;
    httpclient_t http_client;
}fota_http_dl_context_t;
/*************************************************************************************************
*                             Local Function's Definitions                                       *
**************************************************************************************************/
static bool fota_http_dl_set_parameter(void* self, const char* p_url,
                                       fota_http_dl_event_call_back_t event_callback);
static fota_http_dl_result_t fota_http_dl_run(void* self);
static void fota_http_dl_cancel(void* self);
static fota_http_dl_result_t fota_http_dl_run_internal(fota_http_dl_context_t* p_downloader,
                                                       uint8_t* p_download_buf, uint32_t buf_len);
static fota_http_dl_result_t fota_http_dl_result_map(HTTPCLIENT_RESULT result);

/*************************************************************************************************
*                             Public Function's Implementation                                   *
**************************************************************************************************/
/*************************************************************************************************
 * @brief      Init the FOTA HTTP downloader.
 * @param[in]  None
 * @return     When the FOTA HTTP downloader is created successfully, none-NULL value will be returned.
 *************************************************************************************************/
fota_http_dl_t* fota_http_dl_init(void)
{
    fota_http_dl_context_t* p_context = (fota_http_dl_context_t *)pvPortMalloc(sizeof(fota_http_dl_context_t));
    configASSERT(p_context != NULL);
    memset(p_context, 0, sizeof(fota_http_dl_context_t));
    p_context->this = (void*)p_context;
    p_context->downloader_handler.fota_http_dl_cancel = fota_http_dl_cancel;
    p_context->downloader_handler.fota_http_dl_run = fota_http_dl_run;
    p_context->downloader_handler.fota_http_dl_set_parameter = fota_http_dl_set_parameter;
    p_context->cancelled = false;
    p_context->p_http_url = NULL;
    return &(p_context->downloader_handler);
}
/*************************************************************************************************
 * @brief      Destory the FOTA HTTP downloader when the download completes.
 * @param[in]  self  - The pointer points to fota_http_dl_t, allocated by fota_http_dl_init().
 * @return     None
 *************************************************************************************************/
void fota_http_dl_destroy(fota_http_dl_t** self)
{
    fota_http_dl_context_t* p_downloader = NULL;
    configASSERT(self != NULL);
    configASSERT(*self != NULL);
    p_downloader = (fota_http_dl_context_t*)((uint8_t*)(*self) -sizeof(void*));

    if (p_downloader == NULL){
        configASSERT(0);
    }

    if(p_downloader->this != p_downloader){
        configASSERT(0);
    }

    if (p_downloader->p_http_url != NULL){
        vPortFree(p_downloader->p_http_url);
        p_downloader->p_http_url = NULL;
    }
    vPortFree(p_downloader);
    *self = NULL;
}
/*************************************************************************************************
*                             Local Function's Implementation                                    *
**************************************************************************************************/
/*************************************************************************************************
 * @brief      Set the related paramters for FOTA HTTP downloader.
 * @param[in]  self  - The pointer points to fota_http_dl_t, allocated by fota_http_dl_init().
 * @param[in]  p_url - Points to the URL which the FOTA file locates at on the remote server.
 * @param[in]  event_callback - The event call back funtion implemented by the user.
 * @return     When the parameter is set successfully, true will be returned.
 ************************************************************************************************/
static bool fota_http_dl_set_parameter(void* self, const char* p_url,
                                       fota_http_dl_event_call_back_t event_callback)
{
    uint32_t url_len = 0;
    fota_http_dl_context_t* p_downloader = NULL;
    configASSERT(self != NULL);
    configASSERT(p_url != NULL);
    configASSERT(event_callback != NULL);
    p_downloader = (fota_http_dl_context_t*)((uint8_t*)(self) -sizeof(void*));

    if (p_downloader->this != p_downloader){
        return false;
    }
    /*The user wants to use another url to download.*/
    if (p_downloader->p_http_url != NULL){
        vPortFree(p_downloader->p_http_url);
        p_downloader->p_http_url = NULL;
    }

    url_len = strlen(p_url) + 1;
    p_downloader->p_http_url = (char*)pvPortMalloc(url_len);
    if (NULL == p_downloader->p_http_url){
        return false;
    }
    memcpy(p_downloader->p_http_url, p_url, url_len);
    p_downloader->caller_callback = event_callback;
    return true;
}
/*************************************************************************************************
 * @brief      When the function is called, FOTA HTTP downloader will begin to download the data.
 * @param[in]  self  - The pointer points to fota_http_dl_t, allocated by fota_http_dl_init().
 * @return     FOTA HTTP downloader final result.
 *************************************************************************************************/
static fota_http_dl_result_t fota_http_dl_run(void* self)
{
    fota_http_dl_context_t* p_downloader = NULL;
    uint8_t* p_download_buf = NULL;
    fota_http_dl_result_t result = FOTA_HTTP_DL_ERROR;
    configASSERT(self != NULL);

    p_downloader = (fota_http_dl_context_t*)((uint8_t*)(self) -sizeof(void*));
    configASSERT(p_downloader->this == p_downloader);

    FOTA_DL_DBG("URL::%s", p_downloader->p_http_url);
    p_download_buf = (uint8_t *)pvPortMalloc(FOTA_HTTP_DL_BUF_SIZE);
    if (NULL == p_download_buf){
        return FOTA_HTTP_DL_NO_ENOUGH_MEMORY;
    }
    result = fota_http_dl_run_internal(p_downloader, p_download_buf, FOTA_HTTP_DL_BUF_SIZE);
    httpclient_close(&(p_downloader->http_client));
    FOTA_DL_DBG("result code: %d", result);
    vPortFree(p_download_buf);
    p_download_buf = NULL;
    return result;
}
/*************************************************************************************************
 * @brief      When the FOTA HTTP downloader is in another task, the caller can try to call this
 *             function to cancel the download process.
 * @param[in]  self  - The pointer points to fota_http_dl_t, allocated by fota_http_dl_init().
 * @return     None
 *************************************************************************************************/
static void fota_http_dl_cancel(void* self)
{
    fota_http_dl_context_t* p_downloader = NULL;
    configASSERT(self != NULL);
    p_downloader = (fota_http_dl_context_t*)((uint8_t*)(self) -sizeof(void*));
    configASSERT(p_downloader->this == p_downloader);
    /* Set the cancel flag for cancel the downloading process. */
    p_downloader->cancelled = true;
}
static fota_http_dl_result_t fota_http_dl_run_internal(fota_http_dl_context_t* p_downloader,
                                                       uint8_t* p_download_buf,
                                                       uint32_t buf_len)
{
    fota_http_dl_event_call_back_t event_callback;
    httpclient_t* p_http_client = NULL;
    fota_http_dl_result_t dl_result = FOTA_HTTP_DL_ERROR;
    HTTPCLIENT_RESULT http_result = HTTPCLIENT_ERROR_CONN;
    httpclient_data_t client_data = {0};
    fota_http_dl_data_event_t data_event;
    uint32_t recv_temp = 0;
    uint32_t data_len = 0;
    uint32_t count = 0;
    configASSERT(p_downloader != NULL);
    configASSERT(p_download_buf != NULL);
    configASSERT(buf_len != 0);
    event_callback = p_downloader->caller_callback;
    client_data.response_buf = (char*)p_download_buf;
    client_data.response_buf_len = buf_len;
    p_http_client = &(p_downloader->http_client);

    p_http_client->timeout_in_sec = FOTA_DL_RECV_TIMEOUT;
    http_result = httpclient_connect(p_http_client, p_downloader->p_http_url);
    if (http_result != HTTPCLIENT_OK) {
        FOTA_DL_DBG("connect error.");
        return fota_http_dl_result_map(http_result);
    }
    http_result = httpclient_send_request(p_http_client, p_downloader->p_http_url,
                                          HTTPCLIENT_GET, &client_data);
    if (http_result < HTTPCLIENT_OK) {
        return fota_http_dl_result_map(http_result);
    }

    do {
        FOTA_DL_DBG("dl loop.");
        http_result = httpclient_recv_response(p_http_client, &client_data);
        if (http_result < HTTPCLIENT_OK) {
            FOTA_DL_ERR("http error: %d", http_result);
            return fota_http_dl_result_map(http_result);
        }

        if (-1 == client_data.response_content_len){
            return FOTA_HTTP_DL_DATA_NOT_FOUND;
        }

        if (recv_temp == 0){
            recv_temp = client_data.response_content_len;
            if (event_callback(FOTA_HTTP_DL_DATA_TOTAL_LEN_EVENT, (const void*)(&recv_temp)) == false){
                break;
            }
        }

        data_len = recv_temp - client_data.retrieve_len;
        data_event.p_buf = (uint8_t*)(client_data.response_buf);
        data_event.buf_len = data_len;
        if (event_callback(FOTA_HTTP_DL_DATA_EVENT, (const void*)(&data_event)) == false){
            break;
        }
        count += data_len;
        recv_temp = client_data.retrieve_len;
        FOTA_DL_DBG("total received: %lu\n", count);
        FOTA_DL_DBG("dl progress = %lu\n", count * 100 / client_data.response_content_len);
    } while ((http_result == HTTPCLIENT_RETRIEVE_MORE_DATA) && (p_downloader->cancelled == false));
    if (count != client_data.response_content_len || httpclient_get_response_code(p_http_client) != 200) {
        FOTA_DL_ERR("data received not completed, or invalid error code");
        dl_result = FOTA_HTTP_DL_INCOMPLETED;
    }else if (count == 0) {
        FOTA_DL_ERR("receive data length is zero, file not found");
        dl_result = FOTA_HTTP_DL_DATA_NOT_FOUND;
    }else {
        FOTA_DL_DBG("download success");
        dl_result = FOTA_HTTP_DL_OK;
    }
    httpclient_close(p_http_client);
    return dl_result;
}
static fota_http_dl_result_t fota_http_dl_result_map(HTTPCLIENT_RESULT result)
{
    fota_http_dl_result_t dl_result = FOTA_HTTP_DL_ERROR;
    switch(result){
        case HTTPCLIENT_ERROR_PARSE:{
             dl_result = FOTA_HTTP_DL_HTTPCLIENT_ERROR_PARSE;
             break;
        }
        case HTTPCLIENT_UNRESOLVED_DNS:{
             dl_result = FOTA_HTTP_DL_HTTPCLIENT_UNRESOLVED_DNS;
             break;
        }
        case HTTPCLIENT_ERROR_PRTCL:{
             dl_result = FOTA_HTTP_DL_HTTPCLIENT_ERROR_PRTCL;
             break;
        }
        case HTTPCLIENT_ERROR:{
             dl_result = FOTA_HTTP_DL_ERROR;
             break;
        }
        case HTTPCLIENT_CLOSED:{
             dl_result = FOTA_HTTP_DL_HTTPCLIENT_CLOSED;
             break;
        }
        case HTTPCLIENT_ERROR_CONN:{
             dl_result = FOTA_HTTP_DL_HTTPCLIENT_ERROR_CONN;
             break;
        }
    }
    return dl_result;
}
