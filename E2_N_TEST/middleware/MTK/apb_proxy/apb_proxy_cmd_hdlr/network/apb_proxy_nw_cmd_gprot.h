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

#ifndef __APB_PROXY_NW_CMD_GPROT_H__
#define __APB_PROXY_NW_CMD_GPROT_H__

void socket_atcmd_init_task(void);
void apb_upsm_init_task(void);
void lwm2m_atcmd_init_task(void);
void onenet_at_init(void);

#ifdef MTK_LWM2M_CT_SUPPORT
void ctiot_lwm2m_client_init(void);
#endif

#ifdef MTK_CTIOT_SUPPORT
typedef void (*ctiot_at_callback)(bool result, void *user_data);

/* This API is designed to send FOTA update result to lwm2m server. */
void ctiot_at_backup(void);
bool ctiot_at_restore(ctiot_at_callback user_callback, void *user_data);
bool ctiot_at_is_restoring(void);

void ctiot_at_init(void);
#endif

#ifdef MTK_CTM2M_SUPPORT
typedef void (*ctm2m_at_callback)(bool result, void *user_data);

/* This API is designed to send FOTA update result to lwm2m server. */
void ctm2m_at_backup(void);
bool ctm2m_at_restore(ctm2m_at_callback user_callback, void *user_data);
bool ctm2m_at_is_restoring(void);

void ctm2m_at_task_init(void);
#endif

#ifdef MTK_ONENET_SUPPORT
void dm_at_init(void);
int32_t dm_at_register(void);
int32_t dm_at_deregister(void);
#endif

#ifdef MTK_TMO_CERT_SUPPORT
void tmo_at_init(void);
int32_t tmo_at_register(void);
int32_t tmo_at_deregister(void);
#endif

uint32_t onenet_at_hex_to_bin(uint8_t *dest, const char *source, uint32_t max_dest_len);
uint32_t onenet_at_bin_to_hex(char *dest, const uint8_t *source, uint32_t max_dest_len);
void onenet_at_remove_backslash(const char *value, char *new_value);
uint32_t onenet_at_parse_cmd(const char *cmd_string, const char *param_buffer, char *param_list[], uint32_t param_max_num);
uint32_t onenet_at_parse_param(const char *param_string, const char *field_buffer, const char *field_key[], char *field_list[], uint32_t field_max_num, const char *delim);

#endif /* __APB_PROXY_NW_CMD_GPROT_H__ */

