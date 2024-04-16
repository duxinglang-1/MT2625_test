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

#ifndef __TOOL_AUTHENTICATION_H__
#define __TOOL_AUTHENTICATION_H__

/**
 * @addtogroup TOOL_AUTHENTICATION
 *      TODO:// add usage for doxygen later
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup TOOL_AUTH_enum Enum
  * @{
  */

/** @brief This enum defines return type of TOOL_AUTH APIs. */
typedef enum {
    TOOL_AUTH_STATUS_ERROR = -1,              /**< An unknown error occurred. */
    TOOL_AUTH_STATUS_OK = 0,                  /**< The operation was successful. */
} tool_auth_status_t;

/** @brief This enum defines status of checking response. */
typedef enum {
    TOOL_AUTH_NOT_REQUIRED = 0,               /**< The authentication is not required. */
    TOOL_AUTH_REQUIRED = 1,                   /**< The authentication is required. */
} tool_auth_response_t;

/** @brief This enum defines status of verify result. */
typedef enum {
    TOOL_AUTH_VERIFY_FAIL = 0,               /**< The correctness of certificate is invalid. */
    TOOL_AUTH_VERIFY_PASS = 1,               /**< The correctness of certificate is valid. */
} tool_auth_verify_result_t;

/**
  * @}
  */

/**
 * @brief     This function is used to check the tool authentication required in this connection.
 * @param[out] response   is the response after checking.
 * @return
 *                #TOOL_AUTH_STATUS_OK, if the operation completed successfully. \n
 *                #TOOL_AUTH_STATUS_ERROR, if an unknown error occurred. \n
 */
tool_auth_status_t tool_auth_is_required(tool_auth_response_t *response);

/**
 * @brief     This function is used to get the device UID.
 * @param[out] uid      is buffer of the device UID.
 * @param[in]  uid_len  is length of the device UID.
 * @return
 *                #TOOL_AUTH_STATUS_OK, if the operation completed successfully. \n
 *                #TOOL_AUTH_STATUS_ERROR, if an unknown error occurred. \n
 */
tool_auth_status_t tool_auth_get_uid(uint8_t *uid, uint32_t uid_len);

/**
 * @brief     This function is used to verify the correctness of certificate passed from tool side.
 * @param[in] certificate_body   is the data of this certificate.
 * @param[in] certificate_length   is the length of this certificate.
 * @param[out] verify_result   is the result after the certificate checking.
 * @return
 *                #TOOL_AUTH_STATUS_OK, if the operation completed successfully. \n
 *                #TOOL_AUTH_STATUS_ERROR, if an unknown error occurred. \n
 */
tool_auth_status_t tool_auth_verify_certificate(uint8_t *certificate_body,
        uint16_t certificate_length,
        tool_auth_verify_result_t *verify_result);

#ifdef __cplusplus
}
#endif

/**
* @}
*/

#endif
