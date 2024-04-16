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

#ifndef __NIDD_GPROT__
#define __NIDD_GPROT__

#define NIDD_UT

#define NIDD_APN_NAME_MAX_LEN (32)

typedef enum {
    NIDD_EVENT_CHANNEL_ACTIVATE_IND,
    NIDD_EVENT_CHANNEL_DEACTIVATE_IND,
    NIDD_EVENT_DATA_IND,

    NIDD_EVENT_MAX
} nidd_event_t;


typedef enum {
    NIDD_RET_OK = 0,                 /**< No error occurred during the function call. */
    NIDD_RET_ERROR = -1,             /**< Error occurred during the function call. */
    NIDD_RET_STATUS_ERROR = -2,
    NIDD_RET_BUSY = -3,
    NIDD_RET_NO_MEMORY = -4,
    NIDD_RET_WOULDBLOCK = -5,

    NIDD_RET_END = -100              /**< The end enum item. */
} nidd_ret_t;

typedef struct _nidd_receive_data_struct {
    uint32_t             channel_id;
    uint8_t*             data;
    uint32_t             length;
} nidd_receive_data_struct;

typedef struct _nidd_channel_activate_struct {
    char*                       apn;
    uint32_t                    nidd_id;
    uint32_t                    channel_id;
} nidd_channel_activate_struct;

typedef struct _nidd_channel_deactivate_struct {
    char*                       apn;
    uint32_t                    nidd_id;
    uint32_t                    channel_id;
} nidd_channel_deactivate_struct;


typedef struct _nidd_bearer_info_struct
{
    uint8_t        is_activated; //activated (1) and deactivated(0)
    char           apn[NIDD_APN_NAME_MAX_LEN + 1] ;
    uint32_t       channel_id;
} nidd_bearer_info_struct;

typedef void (* nidd_event_handler)(nidd_event_t event, void *data);

extern nidd_ret_t nidd_init();

// must call nidd channel create before activate this channel
// example:
//             nidd_create_nidd_channel(nidd_id, apn, callback);
//             tel_conn_mgr_activate(TEL_CONN_MGR_BEARER_TYPE_NBIOT,
//                                             TEL_CONN_MGR_SIM_ID_NONE,
//                                             TEL_CONN_MGR_PDP_TYPE_NIDD,   apn, ......
extern nidd_ret_t nidd_create_nidd_channel(uint32_t* nidd_id, char* apn, nidd_event_handler callback);
extern nidd_ret_t nidd_destory_nidd_channel(uint32_t nidd_id, char* apn, nidd_event_handler callback);

// must send data after received channel activate event: NIDD_EVENT_CHANNEL_ACTIVATE_IND
extern nidd_ret_t nidd_send_data(uint32_t channel, void* data, uint32_t length);

extern void nidd_bearer_info_ind(nidd_bearer_info_struct *bearer_info);

#ifdef NIDD_UT
void nidd_ut_app_activate(char* apn);

void nidd_ut_app_send_data(char* apn, void* data, uint32_t length);
#endif

#endif /* __NIDD_GPROT__ */
