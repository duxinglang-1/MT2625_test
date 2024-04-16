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

#ifndef __FOTA_COAP_DL__H__
#define __FOTA_COAP_DL__H__

#include "stdint.h"
#include "stdbool.h"

typedef enum {
    FOTA_COAP_DL_OK                        = 0,/**< Download successfully. */
    FOTA_COAP_DL_ERROR                     = 1,/**< Common error. */
    FOTA_COAP_DL_CANCELLED                 = 2,/**< User cancelled download process. */
    FOTA_COAP_DL_NO_ENOUGH_MEMORY          = 3,/**< No engough memory. */
    FOTA_COAP_DL_INCOMPLETED               = 4,/**< The data downloading is not completed. */
    FOTA_COAP_DL_DATA_NOT_FOUND            = 5,/**< The data is not found from server. */
    FOTA_COAP_DL_RESOLVE_DNS_ERROR         = 6,/**< Resolve DNS error.*/
    FOTA_COAP_DL_TIMEOUT                   = 7,/**< COAP time out error.*/
    FOTA_COAP_DL_SERVER_RSP_ERROR          = 8 /**< COAP server responses errors.*/
}fota_coap_dl_result_t;

typedef enum {
    FOTA_COAP_DL_DATA_EVENT,
    FOTA_COAP_DL_DATA_TOTAL_LEN_EVENT /*TODO: not ready.*/
}fota_coap_dl_event_t;

typedef struct {
    uint8_t* p_buf;
    uint32_t buf_len;
}fota_coap_dl_data_event_t;

/*********************************************************************************************
 * @brief      FOTA COAP downloader event call back function.
 * @param[in]  event        - the specific event which is occurred.
 * @param[in]  p_event_data - The pointer points to content for the specific event data.
 * @return     None
 ********************************************************************************************/
typedef bool (*fota_coap_dl_event_call_back_t)(fota_coap_dl_event_t event,
                                               const void* p_event_data);
/*********************************************************************************************
 * @brief      Set the related paramters for FOTA COAP downloader.
 * @param[in]  self  - The pointer points to fota_coap_dl_t, allocated by fota_coap_dl_init().
 * @param[in]  p_url - Points to the URL which the FOTA file locates at on the remote server.
 * @param[in]  event_callback - The event call back funtion implemented by the user.
 * @return     When the parameter is set successfully, true will be returned.
 ********************************************************************************************/
typedef bool (*fota_coap_dl_set_parameter_t)(void* self,const char* p_url,
                                             fota_coap_dl_event_call_back_t event_callback);
/*********************************************************************************************
 * @brief      When the function is called, FOTA COAP downloader will begin to download the data.
 * @param[in]  self  - The pointer points to fota_coap_dl_t, allocated by fota_coap_dl_init().
 * @return     FOTA COAP downloader final result.
 ********************************************************************************************/
typedef fota_coap_dl_result_t (*fota_coap_dl_run_t)(void* self);
/*********************************************************************************************
 * @brief      When the FOTA COAP downloader is in another task, the caller can try to call this
 *             function to cancel the download process.
 * @param[in]  self  - The pointer points to fota_coap_dl_t, allocated by fota_coap_dl_init().
 * @return     None
 ********************************************************************************************/
typedef void (*fota_coap_dl_cancel_t)(void* self);

typedef struct fota_coap_dl_t{
    fota_coap_dl_set_parameter_t fota_coap_dl_set_parameter;
    fota_coap_dl_run_t           fota_coap_dl_run;
    fota_coap_dl_cancel_t        fota_coap_dl_cancel;
}fota_coap_dl_t;

/*********************************************************************************************
 * @brief      Init the FOTA COAP downloader.
 * @param[in]  None
 * @return     When the FOTA COAP downloader is created successfully, none-NULL value will be returned.
 ********************************************************************************************/
fota_coap_dl_t* fota_coap_dl_init(void);
/*********************************************************************************************
 * @brief      Destory the FOTA COAP downloader when the download completes.
 * @param[in]  self  - The pointer points to fota_coap_dl_t, allocated by fota_coap_dl_init().
 * @return     None
 ********************************************************************************************/
void fota_coap_dl_destroy(fota_coap_dl_t** self);

#endif