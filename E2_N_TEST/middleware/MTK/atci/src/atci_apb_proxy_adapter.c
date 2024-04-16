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

 #include "atci_apb_proxy_adapter.h"
 #include "FreeRTOS.h"
 #include "string.h"

 /**
 * @brief This function registers the AT command handler. It is used to receive, parse and handle the registered input command data.
 * @param[in] table is the registered handler parameter. For more details about this parameter, please refer to #atci_cmd_hdlr_item_t.
 * @param[in] hdlr_number is the registered table handler size.
 * @return    #ATCI_STATUS_OK the ATCI command handler is successfully registered. \n
 *            #ATCI_STATUS_REGISTRATION_FAILURE duplicate registration of the AT command or an unused registration table will cause failure.
 * @par       Example
 * @code
 *       ret = atci_register_handler(table, hdlr_number);
 *       if (ret == ATCI_STATUS_OK) {
 *          // AT CMD handler is successfully registered.
 *       } else {
 *          // AT CMD handler is failed to register because the duplcated registed AT command or none unused register table.
 *       }
 * @endcode
 */
 atci_status_t atci_register_handler(atci_cmd_hdlr_item_t *table, int32_t hdlr_number)
 {
     /*This function does not support for ATCI APB Proxy adapter.*/
     return ATCI_STATUS_REGISTRATION_FAILURE;
 }

/**
 * @brief This function sends the AT command response data or the URC data.
 * @param[in] response is the response data. For more details about this parameter, please refer to #atci_response_t.
 * @return    #ATCI_STATUS_OK the ATCI module sent data to the UART successfully. \n
 */
 atci_status_t atci_send_response(atci_response_t *response)
 {
     configASSERT(response != NULL);
     apb_proxy_at_cmd_result_t cmd_result;
     uint32_t response_flag = response->response_flag;
     cmd_result.cmd_id = response->cmd_id;
     cmd_result.length = response->response_len;
     cmd_result.pdata = response->response_buf;

     if (response_flag & ATCI_RESPONSE_FLAG_URC_FORMAT){
         /*The response is URC.*/
         cmd_result.result_code = APB_PROXY_RESULT_UNSOLICITED;
         if (apb_proxy_send_at_cmd_result(&cmd_result) != APB_PROXY_STATUS_OK){
             return ATCI_STATUS_ERROR;
         }
         cmd_result.pdata = NULL;
         cmd_result.length = 0;
         if (response_flag & ATCI_RESPONSE_FLAG_APPEND_OK){
             cmd_result.pdata = "OK";
             cmd_result.length = strlen((const char*)(cmd_result.pdata)) + 1;
             if (apb_proxy_send_at_cmd_result(&cmd_result) != APB_PROXY_STATUS_OK){
                 return ATCI_STATUS_ERROR;
             }
         }else if(response_flag & ATCI_RESPONSE_FLAG_APPEND_ERROR){
             cmd_result.pdata = "ERROR";
             cmd_result.length = strlen((const char*)(cmd_result.pdata)) + 1;
             if (apb_proxy_send_at_cmd_result(&cmd_result) != APB_PROXY_STATUS_OK){
                 return ATCI_STATUS_ERROR;
             }
         }
     }else{
         /*The response is normal command response.*/
         if (response_flag & ATCI_RESPONSE_FLAG_APPEND_OK){
             cmd_result.result_code = APB_PROXY_RESULT_OK;
         }else if(response_flag & ATCI_RESPONSE_FLAG_APPEND_ERROR){
             cmd_result.result_code = APB_PROXY_RESULT_ERROR;
         }else{
             cmd_result.result_code = APB_PROXY_RESULT_ERROR;
             cmd_result.pdata = "Wrong Implementation for AT Command handler, please fix it!";
             cmd_result.length = strlen((const char*)(cmd_result.pdata)) + 1;
             apb_proxy_send_at_cmd_result(&cmd_result);
             return ATCI_STATUS_ERROR;
         }
         if (apb_proxy_send_at_cmd_result(&cmd_result) != APB_PROXY_STATUS_OK){
             return ATCI_STATUS_ERROR;
         }
     }
     return ATCI_STATUS_OK;
 }

