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

#ifndef __APB_PROXY_UTILITY__H__
#define __APB_PROXY_UTILITY__H__

#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

void apb_proxy_set_uint32(uint8_t *encoded_stream, uint32_t *poffset,
                    uint32_t data, uint32_t buffer_len);
void apb_proxy_set_int32(uint8_t *encoded_stream, uint32_t *poffset,
                   int32_t data_value, uint32_t buffer_len);
void apb_proxy_set_uint64(uint8_t *encoded_stream, uint32_t *poffset,
                    uint64_t data_value, uint32_t buffer_len);
void apb_proxy_set_int64(uint8_t *encoded_stream, uint32_t *poffset,
                   int64_t data_value, uint32_t buffer_len);
uint32_t apb_proxy_get_uint32(uint8_t *pdata, uint32_t *poffset);
uint64_t apb_proxy_get_uint64(uint8_t *pdata, uint32_t *poffset);
int32_t apb_proxy_get_int32(uint8_t *pdata, uint32_t *poffset);
int64_t apb_proxy_get_int64(uint8_t *pdata, uint32_t *poffset);
#ifndef APB_PROXY_CMD_ID
/* Generate the AT command ID for application. */
#define APB_PROXY_CMD_ID(channel_id, cmd_id_in_modem) (((channel_id) << 24) | (cmd_id_in_modem))
#endif

#ifndef APB_PROXY_GET_CMD_ID_IN_MODEM
/* Generate the AT command ID for application. */
#define APB_PROXY_GET_CMD_ID_IN_MODEM(apb_proxy_cmd_id) ((apb_proxy_cmd_id) & 0xFFFFFF)
#endif

uint16_t apb_proxy_calc_checksum(uint8_t *p_data, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif/* __APB_PROXY_UTILITY__H__ */

