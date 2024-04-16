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

#ifndef __RIL_CMDS_DEF_H__
#define __RIL_CMDS_DEF_H__
#include "ril_general_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "ril.h"
#include "ril_internal_use.h"

#define AT_PREFIX    ("AT")

typedef struct {
    ril_request_mode_t mode;
    uint32_t request_id;
    void *request_param;
    void *rsp_callback;
    void *user_data;
    int32_t channel_id;
} ril_request_info_t;

typedef struct {
    uint32_t channel_id;
    char *cmd_buf;
    uint32_t cmd_buf_len;
} ril_response_info_t;


typedef int32_t (*ril_cmd_send_hdlr_t)(uint32_t channel_id, char *cmd_buf, ril_request_info_t *info);
typedef int32_t (*ril_cmd_rsp_hdlr_t)(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len);
typedef int32_t (*ril_cmd_urc_hdlr_t)(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len);

typedef enum {
    RIL_NONE = 0,
    RIL_PM   = 0x0000001,
    RIL_MM   = 0x00000002,
    RIL_SMS  = 0x00000004,
    RIL_SIM  = 0x00000008,
    RIL_PD   = 0x00000010,
    RIL_EM   = 0x00000020,
    RIL_STK  = 0x00000040,
    RIL_MUX  = 0x00000080,
    RIL_PB   = 0x00000100,
    RIL_ALL  = 0xffffffff
} ril_func_group_t;


typedef enum {
    RIL_CMD_TYPE_AT_DATA = 0x01,
    RIL_CMD_TYPE_URC = 0x02,
} ril_cmd_type_t;

typedef struct {
    uint32_t cmd_id;
    char *cmd_head;
    ril_func_group_t func_group;
    ril_cmd_type_t cmd_type;
    uint32_t timeout_msec;
    ril_cmd_send_hdlr_t send_hdlr;
    ril_cmd_rsp_hdlr_t rsp_hdlr;
    uint32_t info_rsp_mask;
    bool is_multiline;
} ril_cmd_item_t;


typedef struct {
    ril_urc_id_t event_id;
    char *cmd_head;
    ril_func_group_t func_group;
    ril_cmd_urc_hdlr_t urc_hdlr;
} ril_urc_cmd_item_t;


/* info response mask */
#define RIL_INFO_RSP_NONE                      ((1 << RIL_TEST_MODE))
#define RIL_INFO_RSP_EXECUTE_MODE              ((1 << RIL_EXECUTE_MODE) | (1 << RIL_TEST_MODE))
#define RIL_INFO_RSP_ACTIVE_MODE               ((1 << RIL_ACTIVE_MODE) | (1 << RIL_TEST_MODE))
#define RIL_INFO_RSP_READ_MODE                 ((1 << RIL_READ_MODE) | (1 << RIL_TEST_MODE))

#if defined(RIL_RESULT_CODE_NO_VERBOSE)
typedef struct {
    ril_result_code_t enumerate;
    int32_t numeric;
} ril_result_code_info_t;
#else
typedef struct {
    ril_result_code_t enumerate;
    int32_t numeric;
    const char *text;
} ril_result_code_info_t;
#endif



extern ril_cmd_item_t *get_at_cmd_table();

extern ril_urc_cmd_item_t *get_urc_cmd_table();

extern ril_cmd_item_t *get_at_cmd_item(ril_cmd_id_t cmd_id);

extern ril_urc_cmd_item_t *get_urc_cmd_item(ril_urc_id_t urc_id);

extern ril_cmd_id_t find_at_cmd_id(char *cmd_head, uint32_t cmd_head_len);
extern ril_urc_id_t find_urc_cmd_id(char *cmd_head, uint32_t cmd_head_len);

extern bool ril_cmds_order_is_valid();


#ifdef __cplusplus
}
#endif
#endif /* __RIL_CMDS_DEF_H__ */

