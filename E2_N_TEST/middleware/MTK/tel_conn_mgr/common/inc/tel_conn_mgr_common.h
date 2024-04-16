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

#ifndef __TEL_CONN_MGR_COMMON_H__
#define __TEL_CONN_MGR_COMMON_H__

#include "tel_conn_mgr_features.h"
#include "tel_conn_mgr_platform.h"
#include "tel_conn_mgr_common_def.h"


/* 2 min */
#define TEL_CONN_MGR_ACTIVATION_GUARD_TIMER_EXPIRY_TIME  (2)

/* 1 min */
#define TEL_CONN_MGR_DEACTIVATION_GUARD_TIMER_EXPIRY_TIME  (1)


/* Message IDs for the messages of telephony connection manager */
#define MSG_ID_TEL_CONN_MGR_INTERNAL_BASE     (0x300)

#define MSG_ID_TEL_CONN_MGR_ACTIVATION_REQ          (MSG_ID_TEL_CONN_MGR_INTERNAL_BASE)
#define MSG_ID_TEL_CONN_MGR_DEACTIVATION_REQ        (MSG_ID_TEL_CONN_MGR_INTERNAL_BASE + 2)
#define MSG_ID_TEL_CONN_MGR_BEARER_AT_CMD_RSP_IND   (MSG_ID_TEL_CONN_MGR_INTERNAL_BASE + 6)
#define MSG_ID_TEL_CONN_MGR_BEARER_URC_IND          (MSG_ID_TEL_CONN_MGR_INTERNAL_BASE + 7)

#define MSG_ID_TEL_CONN_MGR_INTERNAL_MAX     (20)

#define MSG_ID_TEL_CONN_MGR_INTERNAL_END     (MSG_ID_TEL_CONN_MGR_INTERNAL_BASE + MSG_ID_TEL_CONN_MGR_INTERNAL_MAX - 1)


#define TEL_CONN_MGR_BEARER_TYPE_MAX_NUM    (3)



/** @brief This enumeration defines the information type. */
typedef enum
{
    TEL_CONN_MGR_INFO_TYPE_NONE,                    /**< Invalid information type mostly used for initialization. */
    TEL_CONN_MGR_INFO_TYPE_ACTIVATION,              /**< Information type for activation. */
    TEL_CONN_MGR_INFO_TYPE_ACTIVE_DEACTIVATION,     /**< Information type for active deactivation. */
    TEL_CONN_MGR_INFO_TYPE_PASSIVE_DEACTIVATION,    /**< Information type for passive deactivation. */
    
    TEL_CONN_MGR_INFO_TYPE_MAX                      /**< The maximum value of this enumeration. */
}tel_conn_mgr_info_type_enum;


typedef struct
{
    unsigned int msg_id;
}tel_conn_mgr_msg_struct;


typedef struct
{
    unsigned int msg_id;  /* MSG_ID_TEL_CONN_MGR_ACTIVATION_REQ */
    int cid;
}tel_conn_mgr_activation_req_struct;


typedef struct
{
    unsigned int msg_id;  /* MSG_ID_TEL_CONN_MGR_DEACTIVATION_REQ */
    int cid;
}tel_conn_mgr_deactivation_req_struct;


/** @brief Type definition for the register handler. */
typedef int *tel_conn_mgr_register_handle_t;


/**
 * @brief Type definition for the callback. The activation result and active deactivation result are returned by
 *        this callback if it is registered before. Also pass deactivate is notified by the callback too.
 * @param [in] app_id    A unique application ID returned by #tel_conn_mgr_activate_ext().
 * @param [in] info_type    Which kind of information does this callback telling.
 * @param [in] result    #TEL_CONN_MGR_TRUE, activation or active deactivation succeeds. Otherwise, fail.
 *         It is meaningless when the result_type is passive deactivation.
 * @param [in] cause    The cause of the failure if result is #TEL_CONN_MGR_FALSE or the result_type is for
 *          passive deactivation.
 * @param [in] pdp_type    The activated pdp type when the info_type is for activation. It is meaningless for
 *          active or passive deactivation.
 * @return None.
 * @sa #tel_conn_mgr_register_callback();
 */typedef void (*tel_conn_mgr_cb)(unsigned int app_id,
                                     tel_conn_mgr_info_type_enum info_type,
                                     tel_conn_mgr_bool result,
                                     tel_conn_mgr_err_cause_enum cause,
                                     tel_conn_mgr_pdp_type_enum pdp_type);
#endif /* __TEL_CONN_MGR_COMMON_H__ */

