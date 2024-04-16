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

#ifndef __TEL_CONN_MGR_APP_API_H__
#define __TEL_CONN_MGR_APP_API_H__

/**
 * @addtogroup TEL_CONN_MGR
 * @{
 */

#include "tel_conn_mgr_platform.h"
#include "tel_conn_mgr_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This function is used to activate a specific bearer. Each time this function is called, it
 *        returns a unique app_id.
 * @param [in] bearer_type    The type of the bearer.
 * @param [in] sim_id    The ID of the SIM card.
 * @param [in] pdp_type    The PDP type to activate.
 * @param [in] apn    APN string, a null-terminated string. The maximum length allowed is
 *                            #TEL_CONN_MGR_APN_MAX_LEN.
 * @param [in] username    Username string, a null-terminated string. The maximum length
 *                                    allowed is #TEL_CONN_MGR_USERNAME_MAX_LEN.
 * @param [in] password    Password string, a null-terminated string. The maximum length
 *                                    allowed is #TEL_CONN_MGR_PASSWORD_MAX_LEN.
 * @param [in] queue_hdl    The queue handler of the thread within which the telephony connection manager messages are handled.
 * @param [out] app_id    A unique application ID. Valid value will be returned only if there is no error.
 * @param [out] activated_pdp_type    If #TEL_CONN_MGR_RET_OK is returned, the real activated pdp_type will
 *                                                    be returned.
 * @return #TEL_CONN_MGR_RET_OK, the bearer is activated and the input parameter pdp_type is compatible.
 *             #TEL_CONN_MGR_RET_WOULDBLOCK, the activation result is returned in a message.
 *             Other, an error occurred.
 * @sa #tel_conn_mgr_deactivate();
 */
tel_conn_mgr_ret_enum tel_conn_mgr_activate(tel_conn_mgr_bearer_type_enum bearer_type,
                                                  tel_conn_mgr_sim_id_enum sim_id,
                                                  tel_conn_mgr_pdp_type_enum pdp_type,
                                                  char *apn,
                                                  char *username,
                                                  char *password,
                                                  tel_conn_mgr_queue_hdl_t queue_hdl,
                                                  unsigned int *app_id,
                                                  tel_conn_mgr_pdp_type_enum *activated_pdp_type);


/**
 * @brief This function is used to deactivate an active bearer. If #MSG_ID_TEL_CONN_MGR_DEACTIVATION_IND
 *            is received, no need to call this function.
 * @param[in] app_id    The one returned by #tel_conn_mgr_activate().
 * @return #TEL_CONN_MGR_RET_OK, the bearer has been deactivated.
 *             #TEL_CONN_MGR_RET_WOULDBLOCK, the deactivation result is returned in a message.
 *             #TEL_CONN_MGR_RET_IS_HOLD, the connection is still in use. There is no need to try again.
 *             #TEL_CONN_MGR_RET_DUPLICATION, the function has been called with the same app_id.
 *             Other, an error occurred.
 * @sa #tel_conn_mgr_activate();
 */
tel_conn_mgr_ret_enum tel_conn_mgr_deactivate(unsigned int app_id);


/**
 * @brief This function initializes and creates the telephony connection manager task.
 * @return None.
 */
void tel_conn_mgr_init(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __TEL_CONN_MGR_APP_API_H__ */

