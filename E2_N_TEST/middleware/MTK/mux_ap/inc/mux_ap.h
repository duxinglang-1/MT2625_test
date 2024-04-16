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

#ifndef __MUX_AP__H__
#define __MUX_AP__H__

/**
 * @addtogroup MUX_AP
 * @{
 * This section introduces the MUX APIs in AP domain.
 *
 * Both MUX in AP domain and MUX in MD domain use CSCI Messages to provide logical communication channels to higher layer entities.
 * These channels are then used for communication between higher entities in AP domain and higher layer entities in MD domain.
 * The present document describes the MUX entity in AP domain only.
 *
 * Terms and Acronyms
 * ======
 * |Terms                         |Details                                                                 |
 * |------------------------------|------------------------------------------------------------------------|
 * |\b MUX                        | MUX (Multiplexer) represents AP-MD adaption layer over CSCI, not 27.010 multiplexer (MUX). |
 * |\b CSCI                       | CSCI (Cross Side Communication Interface) provides a fixed-size data block exchange service between AP domain and MD domain. |
 * |\b CSCI \b Messages           | CSCI Messages are exchanged between the MUX entity in AP domain and the MUX entity in MD domain. |
 *
 * @section mux_ap_sw_architecture MUX-CSCI-MUX Architecture
 * The MUX-CSCI-MUX architecture is shown below:
 * @image html mux_ap_sw_architecture.png
 *
 * @section mux_ap_api_usage How to use this module
 * - In app_init().
 *    @code
 *       mux_ap_status_t status = mux_ap_register_callback(MUX_AP_CHANNEL_TYPE_COMMAND, app_callback, NULL);
 *       // if status is MUX_AP_STATUS_OK, then MUX_AP_EVENT_CHANNEL_ENABLED will be emitted by AP MUX if successful handshake with MD MUX, otherwise MUX_AP_EVENT_CHANNEL_DISABLED will be emitted
 *       // AP can send data to MD when it receives MUX_AP_EVENT_CHANNEL_ENABLED
 *    @endcode
 * - In app_callback().
 *    @code
 *       switch (event) {
 *       case MUX_AP_EVENT_CHANNEL_ENABLED:
 *       // save the channel_id
 *       break;
 *       case MUX_AP_EVENT_CHANNEL_DISABLED:
 *       // invalidate the channel_id
 *       break;
 *       case MUX_AP_EVENT_SEND_COMPLETED:
 *       // free the buffer
 *       break;
 *       case MUX_AP_EVENT_PREPARE_TO_RECEIVE:
 *       // malloc the buffer immediately
 *       break;
 *       case MUX_AP_EVENT_RECEIVE_COMPLETED:
 *       // send message to AP task and free the buffer
 *       break;
 *       }
 *    @endcode
 * - In app_send().
 *    @code
 *       // make sure it gets valid channel_id
 *       // malloc the buffer and construct AT command
 *       mux_ap_status_t status = mux_ap_send_data(channel_id, data_buffer, buffer_length);
 *       // if status is MUX_AP_STATUS_OK, then MUX_AP_EVENT_SEND_COMPLETED will be emitted
 *       // AP can free the buffer when it receives MUX_AP_EVENT_SEND_COMPLETED
 *    @endcode
 * - In app_receive().
 *    @code
 *       // as above, AP should send message to its task when it receives MUX_AP_EVENT_RECEIVE_COMPLETED
 *       // this is the message handler, AP can free the buffer here
 *    @endcode
 */

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup mux_ap_define Define
 * @{
 */

/** @brief This definition defines the number of the MUX channel. */
#define MUX_AP_AT_AND_DATA_CHANNEL_NUM  (7) /**< MUX provides 7 AT and Data channels. */
#define MUX_AP_AP_BRIDGE_CHANNEL_NUM    (1) /**< This is a RPC channel. */
#define MUX_AP_SIMAT_CHANNEL_NUM        (1) /**< This is a RPC channel. */
#define MUX_AP_SOFTSIM_CHANNEL_NUM      (1) /**< This is a RPC channel. */
#define MUX_AP_LBS_CHANNEL_NUM          (1) /**< This is a RPC channel. */

/**
 * @}
 */

/**
 * @defgroup mux_ap_enum Enum
 * @{
 */

/** @brief This enumeration defines the MUX channel type. */
typedef enum {
    MUX_AP_CHANNEL_TYPE_AT_AND_DATA,        /**< AT and Data channel. */
    MUX_AP_CHANNEL_TYPE_AP_BRIDGE,          /**< AP Bridge channel. */
    MUX_AP_CHANNEL_TYPE_SIMAT,              /**< SIMAT channel. */
    MUX_AP_CHANNEL_TYPE_SOFTSIM,            /**< SoftSIM channel. */
    MUX_AP_CHANNEL_TYPE_LBS,                /**< LBS channel. */
    MUX_AP_CHANNEL_TYPE_MAX
} mux_ap_channel_type_t;

/** @brief This enumeration defines the MUX status. */
typedef enum {
    MUX_AP_STATUS_OK = 0,                   /**< Success. */
    MUX_AP_STATUS_INVALID_PARAMETER = -1,   /**< Input parameter is not correct. */
    MUX_AP_STATUS_NO_FREE_CHANNEL = -2,     /**< No free channel can be assigned. */
    MUX_AP_STATUS_CHANNEL_NOT_FOUND = -3,   /**< The MUX channel is not found. */
    MUX_AP_STATUS_CHANNEL_NOT_READY = -4    /**< The MUX channel is not ready. */
} mux_ap_status_t;

/** @brief This enumeration defines the MUX event. */
typedef enum {
    MUX_AP_EVENT_CHANNEL_ENABLED,           /**< The MUX channel is enabled. In this time AP can send and receive data on that channel. */
    MUX_AP_EVENT_CHANNEL_DISABLED,          /**< The MUX channel is disabled. In this time AP can not send or receive data on that channel. */
    MUX_AP_EVENT_FLOW_CONTROL,              /**< The Flow Control is switched on or off. In this time AP can send data on that channel if it is switched on. */
    MUX_AP_EVENT_SEND_COMPLETED,            /**< AP can recycle the buffer which is provided to MUX when calling #mux_ap_send_data(). */
    MUX_AP_EVENT_PREPARE_TO_RECEIVE,        /**< AP should provide the buffer immediately. The data will be discarded by MUX if AP can't provide the buffer. */
    MUX_AP_EVENT_RECEIVE_COMPLETED,         /**< AP should send a message to its task and finally recycle the buffer. */
    MUX_AP_EVENT_MAX
} mux_ap_event_t;

/**
 * @}
 */

/**
 * @defgroup mux_ap_typedef Typedef
 * @{
 */

/** @brief This type defines the MUX channel ID which is assigned uniquely. */
typedef uint32_t mux_ap_channel_id_t;

/**
 * @brief                   This callback function type is registered by AP to listen to the MUX event.
 * @param[in] event_id      is the MUX event ID.
 * @param[in] param         is the parameter of the event.
 * @return                  None
 */
typedef void (*mux_ap_callback_t)(mux_ap_event_t event_id, void *param);

/**
 * @}
 */

/**
 * @defgroup mux_ap_struct Struct
 * @{
 */

/** @brief This structure defines the MUX event which notifies application when MUX channel is enabled. */
typedef struct {
    mux_ap_channel_id_t channel_id;     /**< The MUX channel ID which is assigned uniquely. */
    void *user_data;                    /**< The user data which is provided to MUX when calling #mux_ap_register_callback(). */
} mux_ap_event_channel_enabled_t;

/** @brief This structure defines the MUX event which notifies application when MUX channel is disabled. */
typedef struct {
    mux_ap_channel_id_t channel_id;     /**< The MUX channel ID which is assigned uniquely. */
    void *user_data;                    /**< The user data which is provided to MUX when calling #mux_ap_register_callback(). */
} mux_ap_event_channel_disabled_t;

/** @brief This structure defines the MUX event which notifies application when Flow Control is switched on or off. */
typedef struct {
    mux_ap_channel_id_t channel_id;     /**< The MUX channel ID which is assigned uniquely. */
    bool on_off;                        /**< 'true' means on; 'false' means off. */
} mux_ap_event_flow_control_t;

/** @brief This structure defines the MUX event which notifies application when data is sent to CSCI queue. */
typedef struct {
    mux_ap_channel_id_t channel_id;     /**< The MUX channel ID which is assigned uniquely. */
    uint8_t *data_buffer;               /**< The data buffer which is provided to MUX when calling #mux_ap_send_data(). */
    uint32_t buffer_length;             /**< The buffer length. */
    void *user_data;                    /**< The user data which is provided to MUX when calling #mux_ap_send_data(). */
} mux_ap_event_send_completed_t;

/** @brief This structure defines the MUX event which notifies application when MUX is prepare to receive data from CSCI queue. */
typedef struct {
    mux_ap_channel_id_t channel_id;     /**< The MUX channel ID which is assigned uniquely. */
    uint8_t *data_buffer;               /**< The data buffer which should be provided to MUX immediately. The data will be discarded by MUX if AP can't provide the buffer. */
    uint32_t buffer_length;             /**< The buffer length. */
    void *user_data;                    /**< The user data (AP defined) and will pass back in #mux_ap_event_receive_completed_t. */
} mux_ap_event_prepare_to_receive_t;

/** @brief This structure defines the MUX event which notifies application when data is received completedly. */
typedef struct {
    mux_ap_channel_id_t channel_id;     /**< The MUX channel ID which is assigned uniquely. */
    uint8_t *data_buffer;               /**< The data buffer which is provided to MUX when AP receives #MUX_AP_EVENT_PREPARE_TO_RECEIVE. */
    uint32_t buffer_length;             /**< The buffer length. */
    void *user_data;                    /**< The user data which is provided to MUX when AP receives #MUX_AP_EVENT_PREPARE_TO_RECEIVE. */
} mux_ap_event_receive_completed_t;

/**
 * @}
 */

/**
 * @brief                   This function initializes the MUX event task and context. Called by bootup AP only.
 * @return                  None.
 */
void mux_ap_init(void);

/**
 * @brief                   This function sets the MUX event callback for a registered AP.
 * @param[in] channel_type  is the MUX channel type.
 * @param[in] user_callback is the user callback to listen to the MUX event.
 * @param[in] user_data     is the user data and will pass back in #mux_ap_event_channel_enabled_t and #mux_ap_event_channel_disabled_t.
 * @return                  If successful, return #MUX_AP_STATUS_OK, otherwise return an error code defined in #mux_ap_status_t.
 */
mux_ap_status_t mux_ap_register_callback(mux_ap_channel_type_t channel_type, mux_ap_callback_t user_callback, void *user_data);

/**
 * @brief                   This function changes the MUX event callback for another registered AP.
 * @param[in] channel_id    is the MUX channel ID which is assigned uniquely.
 * @param[in] user_callback is the user callback to listen to the MUX event.
 * @return                  If successful, return #MUX_AP_STATUS_OK, otherwise return an error code defined in #mux_ap_status_t.
 */
mux_ap_status_t mux_ap_change_callback(mux_ap_channel_id_t channel_id, mux_ap_callback_t user_callback);

/**
 * @brief                   This function sends the data on the specified MUX channel.
 * @param[in] channel_id    is the MUX channel ID which is assigned uniquely.
 * @param[in] data_buffer   is the data buffer.
 * @param[in] buffer_length is the buffer length.
 * @param[in] user_data     is the user data (AP defined) and will pass back in #mux_ap_event_send_completed_t.
 * @return                  If successful, return #MUX_AP_STATUS_OK, otherwise return an error code defined in #mux_ap_status_t.
 *                          If successful, AP will also receive #MUX_AP_EVENT_SEND_COMPLETED.
 */
mux_ap_status_t mux_ap_send_data(mux_ap_channel_id_t channel_id, const uint8_t *data_buffer, uint32_t buffer_length, void *user_data);

/**
 * @brief                   This function sends the data on the specified MUX channel.
 * @param[in] channel_id    is the MUX channel ID which is assigned uniquely.
 * @param[in] data_buffer   is the data buffer.
 * @param[in] buffer_length is the buffer length.
 * @param[in] user_data     is the user data (AP defined) and will pass back in #mux_ap_event_send_completed_t.
 * @param[in] is_ip_data    is IP data or not.
 * @return                  If successful, return #MUX_AP_STATUS_OK, otherwise return an error code defined in #mux_ap_status_t.
 *                          If successful, AP will also receive #MUX_AP_EVENT_SEND_COMPLETED.
 */
mux_ap_status_t mux_ap_send_data_ex(mux_ap_channel_id_t channel_id, const uint8_t *data_buffer, uint32_t buffer_length, void *user_data, bool is_ip_data);

/**
 * @brief                   This function stops to receive the data on the specified MUX channel.
 * @note                    When AP can't provide the buffer to receive the data, it immediately calls this API optionally and MUX will buffer the data for this channel.
 *                          AP will also be responsible to call #mux_ap_resume_to_receive() when it detects available memory threshold is reached.
 *                          And MUX will notify AP the buffered user data one by one.
 * @param[in] channel_id    is the MUX channel ID which is assigned uniquely.
 * @return                  If successful, return #MUX_AP_STATUS_OK, otherwise return an error code defined in #mux_ap_status_t.
 * @sa                      mux_ap_resume_to_receive().
 */
mux_ap_status_t mux_ap_stop_to_receive(mux_ap_channel_id_t channel_id);

/**
 * @brief                   This function resumes to receive the data on the specified MUX channel.
 * @param[in] channel_id    is the MUX channel ID which is assigned uniquely.
 * @return                  If successful, return #MUX_AP_STATUS_OK, otherwise return an error code defined in #mux_ap_status_t.
 * @sa                      mux_ap_stop_to_receive().
 */
mux_ap_status_t mux_ap_resume_to_receive(mux_ap_channel_id_t channel_id);

/**
 * @brief                   This function resumes to send the data when waking up from deep sleep. Called by RIL when it receives *MATWAKEUP.
 * @return                  None.
 */
void mux_ap_resume_to_send(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __MUX_AP__H__ */

