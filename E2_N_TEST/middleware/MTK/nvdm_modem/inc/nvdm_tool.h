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

#ifndef __NVDM_TOOL_H__
#define __NVDM_TOOL_H__

#ifdef MTK_NVDM_MODEM_ENABLE

#include "nvdm_modem.h"

/**
 * @brief     This function is used to write or update a data item on protected area partition of flash.
 *            If there are existing data items on the flash, the NVDM invalidates old values and replaces with new ones.
 *            No other operation is performed while the data is being written to the flash.
 *            This function returns after the data is completely written to the flash.
 *            User should check the return status to verify whether the data is written to flash successfully.
 * @param[in] group_name   is the group name of the data item, such as "Bluetooth".
 * @param[in] data_item_name   is the name of the data item, such as "address" of the "Bluetooth".
 * @param[in] type   is the data item type, which defines how the value is displayed.
 * @param[in] buffer   is the pointer to the data item's content.
 * @param[in] size   is the size of the data item's content.
 * @param[in] attr   is the attribute of WRITE operation.
 * @return
 *                #NVDM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_STATUS_ERROR, if an unknown error occurred. \n
 *                #NVDM_STATUS_INVALID_PARAMETER, if the parameter is invalid. \n
 *                #NVDM_STATUS_INSUFFICIENT_SPACE, if the storage space is not enough. \n
 * @note      Call this API in tasks or function callbacks, but not in the interrupt handlers.
 */
nvdm_modem_status_t nvdm_modem_write_protected_data_item(const char *group_name,
                                   const char *data_item_name,
                                   nvdm_modem_data_item_type_t type,
                                   const uint8_t *buffer,
                                   uint32_t size,
                                   nvdm_modem_attr_enum_t attr);

/**
 * @brief     This function is used to delete the data item on protected area partition of flash.
 *            If any error found, it returns with error status.
 *            User should check the return status to verify whether the data item is invalidated successfully.
 * @param[in] group_name   is the group name of the data item, such as "Bluetooth".
 * @param[in] data_item_name   is the name of the data item, such as an "address" of "Bluetooth".
 * @return
 *                #NVDM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_STATUS_ERROR, if an unknown error occurred. \n
 *                #NVDM_STATUS_INVALID_PARAMETER, if the parameter is invalid. \n
 *                #NVDM_STATUS_ITEM_NOT_FOUND, if the data item is not found. \n
 * @note      Call this API in tasks or function callbacks, but not in the interrupt handlers.
 */
nvdm_modem_status_t nvdm_modem_invalidate_protected_data_item(const char *group_name,
                                  const char *data_item_name);

/**
 * @brief     This function is used to query which area the data item is located.
 *            If any error found, it returns with error status.
 *            User should check the return status to verify whether the data item is reset successfully.
 * @param[in] group_name   is the group name of the data item, such as "Bluetooth".
 * @param[in] data_item_name   is the name of the data item, such as an "address" of "Bluetooth".
 * @param[out] area   is the enumation type of area
 * @return
 *                #NVDM_MODEM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_MODEM_STATUS_ERROR, if an unknown error occurred. \n
 *                #NVDM_MODEM_STATUS_INVALID_PARAMETER, if the parameter is invalid. \n
 *                #NVDM_MODEM_STATUS_ITEM_NOT_FOUND, if the data item is not found. \n
 * @note      Call this API in tasks or function callbacks, but not in the interrupt handlers.
 */
nvdm_modem_status_t nvdm_modem_query_data_item_area(const char *group_name,
                                  const char *data_item_name,
                                  nvdm_modem_area_t *area);

/**
 * @brief     This function is used to query the number of data items on specific area.
 *            If any error found, it returns with error status.
 *            User should check the return status to verify whether the data item is reset successfully.
 * @param[in] area   is the enumation type of area
 * @param[out] data_number   is the number of data items.
 * @return
 *                #NVDM_MODEM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_MODEM_STATUS_ERROR, if an unknown error occurred. \n
 *                #NVDM_MODEM_STATUS_INVALID_PARAMETER, if the parameter is invalid. \n
 * @note      Call this API in tasks or function callbacks, but not in the interrupt handlers.
 */
nvdm_modem_status_t nvdm_modem_query_data_item_number(nvdm_modem_area_t area,
                                  uint32_t *data_number);

/**
 * @brief     This function is used to query all data item information available in NVDM.
 *            If any error found, it returns with error status.
 *            User should check the return status to verify whether the data item is reset successfully.
 * @param[in] area   is the enumation type of area
 * @param[out] info_list   is the pointer to the buffer of #nvdm_modem_data_item_info_t structure type that store all data item information
 * @param[in] count   is the number of info_list buffer
 * @return
 *                #NVDM_MODEM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_MODEM_STATUS_ERROR, if an unknown error occurred. \n
 *                #NVDM_MODEM_STATUS_INVALID_PARAMETER, if the parameter is invalid. \n
 *                #NVDM_MODEM_STATUS_INSUFFICIENT_SPACE, if the buffer is not enough for all data item information. \n
 * @note      Call this API in tasks or function callbacks, but not in the interrupt handlers.
 */
nvdm_modem_status_t nvdm_modem_query_all_data_item_info(nvdm_modem_area_t area,
                                    nvdm_modem_data_item_info_t *info_list,
                                    uint32_t count);


#ifdef __cplusplus
}
#endif

#endif

#endif /* __NVDM_TOOL_H__ */

