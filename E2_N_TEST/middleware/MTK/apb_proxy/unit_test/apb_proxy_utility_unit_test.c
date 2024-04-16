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

#include "apb_proxy_unit_test_framework.h"
#include "apb_proxy_utility.h"
#include "apb_proxy_utility_unit_test.h"

bool apb_proxy_set_uint32_unit_test(void)
{
    bool result = true;
    uint32_t index = 0;
    uint8_t encode_stream[sizeof(uint32_t)] = {0};
    uint32_t data = 0x12345678;
    uint8_t expected_stream[sizeof(uint32_t)] =
    {0x78, 0x56, 0x34, 0x12};
    uint32_t offset = 0;
    apb_proxy_set_uint32(encode_stream, &offset, data, sizeof(encode_stream));
    configASSERT(offset == sizeof(uint32_t));

    for (index = 0; index < sizeof(encode_stream); index ++) {
        if (encode_stream[index] != expected_stream[index]) {
            result = false;
            configASSERT(0);
            break;
        }
    }
    return result;
}
bool apb_proxy_set_int32_unit_test(void)
{
    bool result = true;
    uint32_t index = 0;
    uint8_t encode_stream[sizeof(int32_t)] = {0};
    int32_t data = 0x12345678;
    uint8_t expected_stream[sizeof(int32_t)] =
    {0x78, 0x56, 0x34, 0x12};
    uint32_t offset = 0;
    apb_proxy_set_int32(encode_stream, &offset, data, sizeof(encode_stream));
    configASSERT(offset == sizeof(int32_t));

    for (index = 0; index < sizeof(encode_stream); index ++) {
        if (encode_stream[index] != expected_stream[index]) {
            result = false;
            configASSERT(0);
            break;
        }
    }
    return result;

}
bool apb_proxy_set_uint64_unit_test(void)
{
    bool result = true;
    uint32_t index = 0;
    uint8_t encode_stream[sizeof(uint64_t)] = {0};
    uint64_t data = 0x1234567878563412ULL;
    uint8_t expected_stream[sizeof(uint64_t)] =
    {0x12, 0x34, 0x56, 0x78, 0x78, 0x56, 0x34, 0x12};
    uint32_t offset = 0;
    apb_proxy_set_uint64(encode_stream, &offset, data, sizeof(encode_stream));
    configASSERT(offset == sizeof(uint64_t));

    for (index = 0; index < sizeof(encode_stream); index ++) {
        if (encode_stream[index] != expected_stream[index]) {
            result = false;
            configASSERT(0);
            break;
        }
    }
    return result;

}
bool apb_proxy_set_int64_unit_test(void)
{
    bool result = true;
    uint32_t index = 0;
    uint8_t encode_stream[sizeof(int64_t)] = {0};
    int64_t data = 0x1234567878563412LL;
    uint8_t expected_stream[sizeof(int64_t)] =
    {0x12, 0x34, 0x56, 0x78, 0x78, 0x56, 0x34, 0x12};
    uint32_t offset = 0;
    apb_proxy_set_int64(encode_stream, &offset, data, sizeof(encode_stream));
    configASSERT(offset == sizeof(int64_t));

    for (index = 0; index < sizeof(encode_stream); index ++) {
        if (encode_stream[index] != expected_stream[index]) {
            result = false;
            configASSERT(0);
            break;
        }
    }
    return result;

}
bool apb_proxy_get_uint32_unit_test(void)
{
    uint8_t input_stream[sizeof(uint32_t)] =
    {0x78, 0x56, 0x34, 0x12};
    uint32_t expected_value = 0x12345678U;
    uint32_t offset = 0;
    uint32_t returned_value = 0;
    returned_value = apb_proxy_get_uint32(input_stream, &offset);
    configASSERT(offset == sizeof(uint32_t));
    configASSERT(returned_value = expected_value);
    return true;
}
bool apb_proxy_get_uint64_unit_test(void)
{
    uint8_t input_stream[sizeof(uint64_t)] =
    {0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12};
    uint64_t expected_value = 0x1234567812345678ULL;
    uint32_t offset = 0;
    uint64_t returned_value = 0;
    returned_value = apb_proxy_get_uint64(input_stream, &offset);
    configASSERT(offset == sizeof(uint64_t));
    configASSERT(returned_value = expected_value);
    return true;
}
bool apb_proxy_get_int32_unit_test(void)
{
    uint8_t input_stream[sizeof(int32_t)] =
    {0x78, 0x56, 0x34, 0x12};
    int32_t expected_value = 0x12345678;
    uint32_t offset = 0;
    int32_t returned_value = 0;
    returned_value = apb_proxy_get_int32(input_stream, &offset);
    configASSERT(offset == sizeof(int32_t));
    configASSERT(returned_value = expected_value);
    return true;

}
bool apb_proxy_get_int64_unit_test(void)
{
    uint8_t input_stream[sizeof(int64_t)] =
    {0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12};
    int64_t expected_value = 0x1234567812345678LL;
    uint32_t offset = 0;
    int64_t returned_value = 0;
    returned_value = apb_proxy_get_int64(input_stream, &offset);
    configASSERT(offset == sizeof(int64_t));
    configASSERT(returned_value = expected_value);
    return true;
}


