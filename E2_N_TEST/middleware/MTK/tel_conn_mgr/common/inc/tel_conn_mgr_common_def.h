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

#ifndef __TEL_CONN_MGR_COMMON_DEF_H__
#define __TEL_CONN_MGR_COMMON_DEF_H__

/**
 * @addtogroup TEL_CONN_MGR
 * @{
 * This section introduces the TEL_CONN_MGR interfaces in AP domain.
 *
 * Telephony Connection Manager provides the interfaces for the users to activate or deactivate a Packet Data
 * Protocol (PDP) connection. To achieve this, it sends a series of AT commands from AP to modem in a specific
 * order. In addition, it supports multiple users to activate or deactivate the same PDP connection or other
 * PDP connections, simultaneously.
 *
 * Terms and Acronyms
 * ======
 * |Terms                         |Details                                                                 |
 * |------------------------------|------------------------------------------------------------------------|
 * |\b TEL_CONN_MGR               | Telephony Connection Manager. |
 * |\b PDP                        | Packet Data Protocol. |
 * |\b PS                         | Packet-switched. |
 * |\b APN                        | Access Point Name. |
 * |\b NB-IoT                     | Narrowband Internet of Things. |
 *
 * @section tel_conn_mgr_architecture TEL_CONN_MGR Architecture
 * The TEL_CONN_MGR architecture is shown below:
 * @image html tel_conn_mgr_architecture.png
 *
 * @section tel_conn_mgr_usage How to use this module
 * - Initialize the tel_conn_mgr module.
 *    @code
 *       tel_conn_mgr_init();
 *    @endcode
 * - Activate a PDP connection.
 *    @code
 *       tel_conn_mgr_queue_hdl_t queue_hdl;
 *       // Create a queue for the user task.
 *       // ...
 *       unsigned int app_id;
 *       tel_conn_mgr_pdp_type_enum activated_pdp_type;
 *       tel_conn_mgr_ret_enum ret;
 *       ret = tel_conn_mgr_activate(TEL_CONN_MGR_BEARER_TYPE_NBIOT,
 *                                                    TEL_CONN_MGR_SIM_ID_1,
 *                                                    TEL_CONN_MGR_PDP_TYPE_IPV4V6,
 *                                                    "apn_example",//if you want use the default pdn link, the apn should be "".
 *                                                    "user_name_example",//if you want use the default pdn link, the user name should be "".
 *                                                    "password_example", //if you want use the default pdn link, the password should be "".
 *                                                    queue_hdl,
 *                                                    &app_id,
 *                                                    &activated_pdp_type);
 *    @endcode
 * - Deactivate the PDP connection.
 *    @code
 *       tel_conn_mgr_ret_enum ret; 
 *       ret = tel_conn_mgr_deactivate(app_id);
 *       // The app_id is the one returned by tel_conn_mgr_activate() API
 *    @endcode
 */

#include "tel_conn_mgr_features.h"
#include "tel_conn_mgr_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup tel_conn_mgr_define Define
 * @{
 */

/** @brief Definitions for the external message IDs. */
#define MSG_ID_TEL_CONN_MGR_EXTERN_BASE        (0x200) /**< Base message ID. */
#define MSG_ID_TEL_CONN_MGR_ACTIVATION_RSP     (MSG_ID_TEL_CONN_MGR_EXTERN_BASE + 1) /**< The message ID for the activation response. */
#define MSG_ID_TEL_CONN_MGR_DEACTIVATION_RSP   (MSG_ID_TEL_CONN_MGR_EXTERN_BASE + 2) /**< The message ID for the deactivation response. */
#define MSG_ID_TEL_CONN_MGR_DEACTIVATION_IND   (MSG_ID_TEL_CONN_MGR_EXTERN_BASE + 3) /**< The message ID for the deactivation indication. */
#define MSG_ID_TEL_CONN_MGR_EXTERN_END         (MSG_ID_TEL_CONN_MGR_DEACTIVATION_IND) /**< The last message ID. */


/** @brief Definition for the maximum length of the APN. */
#define TEL_CONN_MGR_APN_MAX_LEN (32)

/** @brief Definition for the maximum length of the user name. */
#define TEL_CONN_MGR_USERNAME_MAX_LEN (32)

/** @brief Definition for the maximum length of the password. */
#define TEL_CONN_MGR_PASSWORD_MAX_LEN (32)

/** @brief Definition for the maximum number of app_id supported per bearer within a thread. */
#define TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM   (5)

/**
 * @}
 */


/**
 * @defgroup tel_conn_mgr_enum Enum
 * @{
 */
 
/** @brief This enumeration defines the return values. */
typedef enum
{
    TEL_CONN_MGR_RET_OK = 0,                 /**< Successful operation. */
    TEL_CONN_MGR_RET_ERROR = -1,             /**< An error occurred. */
    TEL_CONN_MGR_RET_WOULDBLOCK = -2,        /**< Wouldblock. The result will be returned in a message. */
    TEL_CONN_MGR_RET_IS_HOLD = -3,           /**< There is still at least one application using the bearer right now. */
    TEL_CONN_MGR_RET_DUPLICATION = -4,       /**< Duplicated request was made. */
    TEL_CONN_MGR_RET_NOT_FOUND = -5,         /**< Not found. */
    TEL_CONN_MGR_RET_INVALID_PARAM = -6,     /**< At least one parameter is invalid. */
    TEL_CONN_MGR_RET_LIMIT_RESOURCE = -7,    /**< Insufficient resources. */
    TEL_CONN_MGR_RET_NOT_ENOUGH_MEMORY = -8, /**< Insufficient memory. */
    TEL_CONN_MGR_RET_INCOMPATIBLE = -9,      /**< The PDP types are not incompatible. */
    TEL_CONN_MGR_RET_WRONG_STATE = -10,      /**< Wrong state. */
    TEL_CONN_MGR_RET_UNKNOWN = -11,          /**< Unknown error. */
    TEL_CONN_MGR_RET_CANCELED = -12,         /**< Canceled. */
}tel_conn_mgr_ret_enum;

/** @brief This enumeration defines the causes. */
typedef enum
{
    TEL_CONN_MGR_ERR_CAUSE_NONE,                        /**< No error occurred. */
    TEL_CONN_MGR_ERR_CAUSE_UNKNOWN,                     /**< Unknown cause. */
    TEL_CONN_MGR_ERR_CAUSE_INIT_FAILED,                 /**< Initialization failed. */
    TEL_CONN_MGR_ERR_CAUSE_MODEM_EXCEPTION,             /**< Modem exception. */
    TEL_CONN_MGR_ERR_CAUSE_AT_CMD_EXCEPTION,            /**< AT command exception. */
    TEL_CONN_MGR_ERR_CAUSE_NW_DETACH,                   /**< The network has forced a packet-switched (PS) detachment. */
    TEL_CONN_MGR_ERR_CAUSE_ME_DETACH,                   /**< The mobile termination has forced a PS detachment. */
    TEL_CONN_MGR_ERR_CAUSE_NW_DEACT,                    /**< The network has forced a context deactivation. */
    TEL_CONN_MGR_ERR_CAUSE_ME_DEACT,                    /**< The mobile termination has forced a context deactivation. */
    TEL_CONN_MGR_ERR_CAUSE_NW_REGISTRATION_FAILED,      /**< The network registration failed. */
    TEL_CONN_MGR_ERR_CAUSE_DEACT_REQ,                   /**< The deactivation request was sent. */
    TEL_CONN_MGR_ERR_CAUSE_ACT_REQ,                     /**< The activation request was sent. */
    TEL_CONN_MGR_ERR_CAUSE_TIMEOUT,                     /**< The timer expired. */
    TEL_CONN_MGR_ERR_CAUSE_PDP_TYPE_NOT_SUPPORTED,      /**< The PDP type is not supported. */
    TEL_CONN_MGR_ERR_CAUSE_PDP_TYPE_NOT_COMPATIBLE,     /**< The PDP type is not compatible. */
    
    TEL_CONN_MGR_ERR_CAUSE_MAX                          /**< The maximum value of this enumeration. */
}tel_conn_mgr_err_cause_enum;

/** @brief This enumeration defines the PDP type. */
typedef enum
{
    TEL_CONN_MGR_PDP_TYPE_NONE = 0,    /**< Invalid PDP type mostly used for initialization. */
    TEL_CONN_MGR_PDP_TYPE_IP,          /**< IPV4 only. */
    TEL_CONN_MGR_PDP_TYPE_IPV6,        /**< IPV6 only. */
    TEL_CONN_MGR_PDP_TYPE_IPV4V6,      /**< IPV4V6 only. */
    TEL_CONN_MGR_PDP_TYPE_NIDD,        /**< Non-IP. */

    TEL_CONN_MGR_PDP_TYPE_MAX          /**< The maximum value of this enumeration. */
}tel_conn_mgr_pdp_type_enum;

/** @brief This enumeration defines the bearer type. If extended, it should be stored into the NVDM.*/
typedef enum
{
    TEL_CONN_MGR_BEARER_TYPE_NONE,       /**< Invalid bearer type mostly used for initialization. */
    TEL_CONN_MGR_BEARER_TYPE_NBIOT,      /**< NB-IoT bearer. */

    TEL_CONN_MGR_BEARER_TYPE_MAX         /**< The maximum value of this enumeration. */
}tel_conn_mgr_bearer_type_enum;

/** @brief This enumeration defines the SIM ID. If extended, it should be stored into the NVDM.*/
typedef enum
{
    TEL_CONN_MGR_SIM_ID_NONE,     /**< Invalid SIM ID mostly used for initialization. */
    TEL_CONN_MGR_SIM_ID_1,        /**< The ID for SIM card 1. */

    TEL_CONN_MGR_SIM_ID_MAX       /**< The maximum value of this enumeration. */
}tel_conn_mgr_sim_id_enum;

/**
 * @}
 */


/**
 * @defgroup tel_conn_mgr_struct Struct
 * @{
 */

/** @brief This structure defines the APN content. */
typedef struct {
    char apn_carrier[TEL_CONN_MGR_USERNAME_MAX_LEN];/**< The operator. */
    char apn[TEL_CONN_MGR_APN_MAX_LEN];             /**< The APN. */
    char user_name[TEL_CONN_MGR_USERNAME_MAX_LEN];  /**< The user name of the APN. */
    char password[TEL_CONN_MGR_PASSWORD_MAX_LEN];   /**< The password of the APN. */
}tel_conn_mgr_apn_struct;

/** @brief This structure defines the activation response. */
typedef struct
{
    unsigned int msg_id;                                     /**< It should be #MSG_ID_TEL_CONN_MGR_ACTIVATION_RSP. */
    tel_conn_mgr_bearer_type_enum bearer_type;               /**< The bearer type of the PDP connection. */
    tel_conn_mgr_sim_id_enum sim_id;                         /**< The SIM ID of the PDP connection. */
    tel_conn_mgr_pdp_type_enum pdp_type;                     /**< The actual pdp_type that is activated, if the result is true. */
    char apn[TEL_CONN_MGR_APN_MAX_LEN + 1];                      /**< The APN of the PDP connection. */
    tel_conn_mgr_bool result;                                /**< If true, the PDP connection is activated successfully, otherwise it's not. */
    tel_conn_mgr_err_cause_enum cause;                       /**< The cause of the failure if the result is false. */
    unsigned int app_id[TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM]; /**< The app_ids indicate the applications that require activation within the same task. */
}tel_conn_mgr_activation_rsp_struct;

/** @brief This structure defines the deactivation response. */
typedef struct
{
    unsigned int msg_id;                        /**< It should be #MSG_ID_TEL_CONN_MGR_DEACTIVATION_RSP. */
    tel_conn_mgr_bearer_type_enum bearer_type;  /**< The bearer type of the PDP connection. */
    tel_conn_mgr_sim_id_enum sim_id;            /**< The SIM ID of the PDP connection. */
    tel_conn_mgr_pdp_type_enum pdp_type;        /**< The pdp_type of the PDP connection. */
    char apn[TEL_CONN_MGR_APN_MAX_LEN + 1];         /**< The APN of the PDP connection. */
    tel_conn_mgr_bool result;                   /**< If true, the PDP connection is deactivated successfully, otherwise it's not. */
    tel_conn_mgr_err_cause_enum cause;          /**< The cause of the failure if the result is false. */
    unsigned int app_id;                        /**< The app_id indicates the application that triggered the deactivation. */
}tel_conn_mgr_deactivation_rsp_struct;

/** @brief This structure defines the deactivation indication. */
typedef struct
{
    unsigned int msg_id;                                     /**< It should be #MSG_ID_TEL_CONN_MGR_DEACTIVATION_IND. */
    tel_conn_mgr_bearer_type_enum bearer_type;               /**< The bearer type of the PDP connection. */
    tel_conn_mgr_sim_id_enum sim_id;                         /**< The SIM ID of the PDP connection. */
    tel_conn_mgr_pdp_type_enum pdp_type;                     /**< The pdp_type of the PDP connection. */
    char apn[TEL_CONN_MGR_APN_MAX_LEN + 1];                      /**< The apn of the PDP connection. */
    tel_conn_mgr_err_cause_enum cause;                       /**< The cause of passive deactivation. */
    unsigned int app_id[TEL_CONN_MGR_THREAD_MAX_APP_ID_NUM]; /**< The app_ids indicate the applications that require activation within the same task. */
}tel_conn_mgr_deactivation_ind_struct;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __TEL_CONN_MGR_COMMON_DEF_H__ */
