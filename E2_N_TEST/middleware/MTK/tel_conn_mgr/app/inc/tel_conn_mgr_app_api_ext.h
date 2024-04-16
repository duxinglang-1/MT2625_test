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

#ifndef __TEL_CONN_MGR_APP_API_EXT_H__
#define __TEL_CONN_MGR_APP_API_EXT_H__


#include "tel_conn_mgr_platform.h"
#include "tel_conn_mgr_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This function is used to register the callback for activation, active deactivation and passive deactivation.
 * @param [in] callback    The callback to registered. 
 * @return If NULL is returned, registeration failed. Otherwise, the register handler is returned.
 * @sa #tel_conn_mgr_deregister_callback();
 */
tel_conn_mgr_register_handle_t tel_conn_mgr_register_callback(tel_conn_mgr_cb callback);


/**
 * @brief This function is used to register the callback for activation, active deactivation and passive deactivation.
 * @param [in] reg_hdl    The register handler returned by # tel_conn_mgr_register_callback (). 
 * @return #TEL_CONN_MGR_TRUE, succeed. #TEL_CONN_MGR_FALSE, fail.
 * @sa #tel_conn_mgr_register_callback();
 */
tel_conn_mgr_bool tel_conn_mgr_deregister_callback(tel_conn_mgr_register_handle_t reg_hdl);


/**
 * @brief This function is used to activate a specific bearer. Each time this function is called, it returns
 *        a unique app_id. Only when TEL_CONN_MGR_SUPPORT_CALLBACK is enabled, will it works.
 * @param [in] bearer_type    The type of the bearer.
 * @param [in] sim_id    The ID of the SIM card.
 * @param [in] pdp_type    The PDP type to activate.
 * @param [in] apn    APN string, a null-terminated string. The maximum length allowed is
 *                            #TEL_CONN_MGR_APN_MAX_LEN.
 * @param [in] username    Username string, a null-terminated string. The maximum length
 *                                    allowed is #TEL_CONN_MGR_USERNAME_MAX_LEN.
 * @param [in] password    Password string, a null-terminated string. The maximum length
 *                                    allowed is #TEL_CONN_MGR_PASSWORD_MAX_LEN.
 * @param [in] reg_hdl    The register handler returned by #tel_conn_mgr_register_callback ().
 * @param [out] app_id    A unique application ID. Valid value will be returned only if there is no error.
 * @param [out] activated_pdp_type    If #TEL_CONN_MGR_RET_OK is returned, the real activated pdp_type will
 *                                                    be returned.
 * @return #TEL_CONN_MGR_RET_OK, the bearer is activated and the input parameter pdp_type is compatible.
 *             #TEL_CONN_MGR_RET_WOULDBLOCK, the activation result is returned in the callback.
 *             Other, an error occurred.
 * @sa #tel_conn_mgr_deactivate();
 */
tel_conn_mgr_ret_enum tel_conn_mgr_activate_ext(tel_conn_mgr_bearer_type_enum bearer_type,
                                                       tel_conn_mgr_sim_id_enum sim_id,
                                                       tel_conn_mgr_pdp_type_enum pdp_type,
                                                       char *apn,
                                                       char *username,
                                                       char *password,
                                                       tel_conn_mgr_register_handle_t reg_hdl,
                                                       unsigned int *app_id,
                                                       tel_conn_mgr_pdp_type_enum *activated_pdp_type);

#ifdef __cplusplus
}
#endif

#endif /* __TEL_CONN_MGR_APP_API_EXT_H__ */

