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

#ifndef __NVDM_MODEM_H__
#define __NVDM_MODEM_H__

#ifdef MTK_NVDM_MODEM_ENABLE

/**
 * @addtogroup NVDM
 * @{
 * This section introduces the modem NVDM APIs including terms and acronyms,
 * supported features, software architecture, details on how to use the modem NVDM, enums, structures, typedefs and functions.
 *
 * @section NVDM_MODEM_Terms_Chapter Terms and acronyms
 *
 * |        Terms         |           Details                |
 * |----------------------|----------------------------------|
 * |\b NVDM             | Non-volatile Data Management is a middleware used to store user data. |
 *
 * @section NVDM_MODEM_Features_Chapter Supported features
 *
 * - \b Support \b retaining \b data \b after \b power \b is \b off. \n
 *   Modem NVDM stores modem data in a flash memory, so the data is preserved even after power is off. \n
 * - \b Support \b grouping \b data \b items. \n
 *   The NVDM stores data items based on group names.
 *   The user can define different data items with different group names.
 *   This enables the data item classification and keeps them in order.
 *
 * @section NVDM_MODEM_APIs_Usage_Chapter How to use the NVDM APIs
 *
 * - To use the modem NVDM services follow the steps, as shown below. \n
 *  - Step1: Call nvdm_modem_init() to initialize the NVDM module.
 *  - Step2: Call nvdm_modem_write_data_item() to write or update data item to the flash.
 *  - Step3: Call nvdm_modem_read_data_item() to read data item from the flash.
 *  - sample code:
 *    @code
 *       #define STA_IPADDR_DEFAULT_VALUE "192.168.0.1"
 *
 *       void nvdm_modem_application(void)
 *       {
 *       }
 *    @endcode
 *
 * @section NVDM_MODEM_Region_Adjust_Chapter How to adjust the NVDM region
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup NVDM_MODEM_define Define
  * @{
  */

/** @brief This macro defines max length of group name of data item
  */
#define GROUP_NAME_MAX_LENGTH (16)


/** @brief This macro defines max length of data item name of data item
  */
#define DATA_ITEM_NAME_MAX_LENGTH (32)

/**
  * @}
  */


/** @defgroup NVDM_MODEM_enum Enum
  * @{
  */

/** @brief This enum defines the display type of a data item. */
typedef enum {
    NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA = 0x01,   /**< Defines the display type with raw data. */
    NVDM_MODEM_DATA_ITEM_TYPE_STRING = 0x02,     /**< Defines the display type with string. */
} nvdm_modem_data_item_type_t;

/** @brief This enum defines return type of modem NVDM APIs. */
typedef enum {
    NVDM_MODEM_STATUS_NO_BACKUP = -6,          /**< No backup data found. */
    NVDM_MODEM_STATUS_INVALID_PARAMETER = -5,  /**< The user parameter is invalid. */
    NVDM_MODEM_STATUS_ITEM_NOT_FOUND = -4,     /**< The data item wasn't found by the NVDM. */
    NVDM_MODEM_STATUS_INSUFFICIENT_SPACE = -3, /**< No space is available in the flash. */
    NVDM_MODEM_STATUS_INCORRECT_CHECKSUM = -2, /**< The NVDM found a checksum error when reading the data item. */
    NVDM_MODEM_STATUS_ERROR = -1,              /**< An unknown error occurred. */
    NVDM_MODEM_STATUS_OK = 0,                  /**< The operation was successful. */
} nvdm_modem_status_t;

/** @brief This enum defines the partition of each NVDM area. */
typedef enum {
    NVDM_MODEM_AREA_NONE = 0x0,           /**< Define none area. */
    NVDM_MODEM_AREA_NORMAL = 0x01,        /**< Define normal area. */
    NVDM_MODEM_AREA_PROTECTED = 0x02,     /**< Define protected area. */
    NVDM_MODEM_AREA_BACKUP = 0x04,        /**< Defines backup area. */
    NVDM_MODEM_AREA_OTP = 0x08,           /**< Defines otp area. */
    NVDM_MODEM_AREA_END = NVDM_MODEM_AREA_BACKUP,
    NVDM_MODEM_AREA_MINIDUMP = 0xFF       /**< Defines mini-dump area. */
} nvdm_modem_area_t;

/** @brief This enum defines attribute of data item. */
typedef enum {
    NVDM_MODEM_ATTR_AVERAGE = 0x0,        /**< Default attribute. */
    NVDM_MODEM_ATTR_BACKUP = 0x01,        /**< Save a copy to backup area. */
} nvdm_modem_attr_enum_t;

/**
  * @}
  */


/**
 * @defgroup NVDM_MODEM_struct Struct
 * @{
 */

/**
 *  @brief This struct defines the basic information of data item.
 */
typedef struct {
    char group_name[GROUP_NAME_MAX_LENGTH+1];
    char data_item_name[DATA_ITEM_NAME_MAX_LENGTH+1];
    nvdm_modem_area_t area; 
} nvdm_modem_data_item_info_t;

/**
  * @}
  */


/**
 * @brief     This function initializes the modem NVDM module to enable the NVDM services.
 *            User should not call nvdm_modem_read_data_item() or nvdm_modem_write_data_item() before nvdm_modem_init().
 *            Call nvdm_modem_init() only once during the initialization.
 * @return
 *                #NVDM_MODEM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_MODEM_STATUS_ERROR, if an unknown error occurred. \n
 */
nvdm_modem_status_t nvdm_modem_init(void);

/**
 * @brief     This function is used to write or update a data item on normal area partition of flash.
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
 *                #NVDM_MODEM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_MODEM_STATUS_ERROR, if an unknown error occurred. \n
 *                #NVDM_MODEM_STATUS_INVALID_PARAMETER, if the parameter is invalid. \n
 *                #NVDM_MODEM_STATUS_INSUFFICIENT_SPACE, if the storage space is not enough. \n
 * @note      Call this API in tasks or function callbacks, but not in the interrupt handlers.
 */
nvdm_modem_status_t nvdm_modem_write_normal_data_item(const char *group_name,
                                   const char *data_item_name,
                                   nvdm_modem_data_item_type_t type,
                                   const uint8_t *buffer,
                                   uint32_t size,
                                   nvdm_modem_attr_enum_t attr);

/**
 * @brief     This function is used to read the data item from normal area partition of flash.
 *            If there is no such data item, it returns #NVDM_MODEM_STATUS_ITEM_NOT_FOUND.
 *            User should check the return status to verify whether the data read from the flash is correct.
 * @param[in] group_name   is the group name of the data item, such as "Bluetooth".
 * @param[in] data_item_name   is the name of the data item, such as an "address" of "Bluetooth".
 * @param[out] type   is the pointer to data item type, which defines how the value is displayed.
 * @param[out] buffer   is the pointer to the user buffer, that stores the data item content read from the flash.
 * @param[in,out] size   is the user buffer size when used as an input and is the size of actual data item's content read out when used as an output.
 * @return
 *                #NVDM_MODEM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_MODEM_STATUS_ERROR, if an unknown error occurred. \n
 *                #NVDM_MODEM_STATUS_INVALID_PARAMETER, if the parameter is invalid. \n
 *                #NVDM_MODEM_STATUS_ITEM_NOT_FOUND, if the data item is not found. \n
 *                #NVDM_MODEM_STATUS_INCORRECT_CHECKSUM, if the checksum of data item is invalid. \n
 * @note      Call this API in tasks or function callbacks, but not in the interrupt handlers.
 */
nvdm_modem_status_t nvdm_modem_read_normal_data_item(const char *group_name,
                                  const char *data_item_name,
                                  nvdm_modem_data_item_type_t *type,
                                  uint8_t *buffer,
                                  uint32_t *size);

/**
 * @brief     This function is used to delete the data item on normal area partition of flash.
 *            If any error found, it returns with error status.
 *            User should check the return status to verify whether the data item is invalidated successfully.
 * @param[in] group_name   is the group name of the data item, such as "Bluetooth".
 * @param[in] data_item_name   is the name of the data item, such as an "address" of "Bluetooth".
 * @return
 *                #NVDM_MODEM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_MODEM_STATUS_ERROR, if an unknown error occurred. \n
 *                #NVDM_MODEM_STATUS_INVALID_PARAMETER, if the parameter is invalid. \n
 *                #NVDM_MODEM_STATUS_ITEM_NOT_FOUND, if the data item is not found. \n
 * @note      Call this API in tasks or function callbacks, but not in the interrupt handlers.
 */
nvdm_modem_status_t nvdm_modem_invalidate_normal_data_item(const char *group_name,
                                  const char *data_item_name);

/**
 * @brief     This function is used to reset the data item from backup area to normal area partition.
 *            If any error found, it returns with error status.
 *            User should check the return status to verify whether the data item is reset successfully.
 * @param[in] group_name   is the group name of the data item, such as "Bluetooth".
 * @param[in] data_item_name   is the name of the data item, such as an "address" of "Bluetooth".
 * @return
 *                #NVDM_MODEM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_MODEM_STATUS_ERROR, if an unknown error occurred. \n
 *                #NVDM_MODEM_STATUS_INVALID_PARAMETER, if the parameter is invalid. \n
 *                #NVDM_MODEM_STATUS_ITEM_NOT_FOUND, if the data item is not found. \n
 *                #NVDM_MODEM_STATUS_INSUFFICIENT_SPACE, if the storage space is not enough. \n
 *                #NVDM_MODEM_STATUS_INCORRECT_CHECKSUM, if the checksum of data item is invalid. \n
  *               #NVDM_MODEM_STATUS_NO_BACKUP, if no backup data to reset. \n
 * @note      Call this API in tasks or function callbacks, but not in the interrupt handlers.
 */
nvdm_modem_status_t nvdm_modem_reset_normal_data_item(const char *group_name,
                                  const char *data_item_name);

/**
 * @brief     This function is used to read the data item from protected area partition of flash.
 *            If there is no such data item, it returns #NVDM_MODEM_STATUS_ITEM_NOT_FOUND.
 *            User should check the return status to verify whether the data read from the flash is correct.
 * @param[in] group_name   is the group name of the data item, such as "Bluetooth".
 * @param[in] data_item_name   is the name of the data item, such as an "address" of "Bluetooth".
 * @param[out] type   is the pointer to data item type, which defines how the value is displayed.
 * @param[out] buffer   is the pointer to the user buffer, that stores the data item content read from the flash.
 * @param[in,out] size   is the user buffer size when used as an input and is the size of actual data item's content read out when used as an output.
 * @return
 *                #NVDM_MODEM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_MODEM_STATUS_ERROR, if an unknown error occurred. \n
 *                #NVDM_MODEM_STATUS_INVALID_PARAMETER, if the parameter is invalid. \n
 *                #NVDM_MODEM_STATUS_ITEM_NOT_FOUND, if the data item is not found. \n
 *                #NVDM_MODEM_STATUS_INCORRECT_CHECKSUM, if the checksum of data item is invalid. \n
 * @note      Call this API in tasks or function callbacks, but not in the interrupt handlers.
 */
nvdm_modem_status_t nvdm_modem_read_protected_data_item(const char *group_name,
                                  const char *data_item_name,
                                  nvdm_modem_data_item_type_t *type,
                                  uint8_t *buffer,
                                  uint32_t *size);

/**
 * @brief     This function is used to reset the data item from backup area to protected area partition.
 *            If any error found, it returns with error status.
 *            User should check the return status to verify whether the data item is reset successfully.
 * @param[in] group_name   is the group name of the data item, such as "Bluetooth".
 * @param[in] data_item_name   is the name of the data item, such as an "address" of "Bluetooth".
 * @return
 *                #NVDM_MODEM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_MODEM_STATUS_ERROR, if an unknown error occurred. \n
 *                #NVDM_MODEM_STATUS_INVALID_PARAMETER, if the parameter is invalid. \n
 *                #NVDM_MODEM_STATUS_ITEM_NOT_FOUND, if the data item is not found. \n
 *                #NVDM_MODEM_STATUS_INSUFFICIENT_SPACE, if the storage space is not enough. \n
 *                #NVDM_MODEM_STATUS_INCORRECT_CHECKSUM, if the checksum of data item is invalid. \n
  *               #NVDM_MODEM_STATUS_NO_BACKUP, if no backup data to reset. \n
 * @note      Call this API in tasks or function callbacks, but not in the interrupt handlers.
 */
nvdm_modem_status_t nvdm_modem_reset_protected_data_item(const char *group_name,
                                  const char *data_item_name);

/**
 * @brief     This function is used to read the data item from otp area of flash.
 *            If there is no such data item, it returns #NVDM_MODEM_STATUS_ITEM_NOT_FOUND.
 *            User should check the return status to verify whether the data read from the flash is correct.
 * @param[in] group_name   is the group name of the data item, such as "Bluetooth".
 * @param[in] data_item_name   is the name of the data item, such as an "address" of "Bluetooth".
 * @param[out] buffer   is the pointer to the user buffer, that stores the data item content read from the flash.
 * @param[in,out] size   is the user buffer size when used as an input and is the size of actual data item's content read out when used as an output.
 * @return
 *                #NVDM_MODEM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_MODEM_STATUS_ERROR, if an unknown error occurred. \n
 *                #NVDM_MODEM_STATUS_INVALID_PARAMETER, if the parameter is invalid. \n
 *                #NVDM_MODEM_STATUS_ITEM_NOT_FOUND, if the data item is not found. \n
 *                #NVDM_MODEM_STATUS_INCORRECT_CHECKSUM, if the checksum of data item is invalid. \n
 * @note      Call this API in tasks or function callbacks, but not in the interrupt handlers.
 */
nvdm_modem_status_t nvdm_modem_read_otp_data_item(const char *data_item_name,
                                   uint8_t *buffer,
                                   uint32_t *size);

/**
 * @brief     This function is used to write or update a data item on otp area of flash.
 *            If there are existing data items on the flash, the NVDM invalidates old values and replaces with new ones.
 *            No other operation is performed while the data is being written to the flash.
 *            This function returns after the data is completely written to the flash.
 *            User should check the return status to verify whether the data is written to flash successfully.
 * @param[in] group_name   is the group name of the data item, such as "Bluetooth".
 * @param[in] data_item_name   is the name of the data item, such as "address" of the "Bluetooth".
 * @param[in] buffer   is the pointer to the data item's content.
 * @param[in] size   is the size of the data item's content.
 * @return
 *                #NVDM_MODEM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_MODEM_STATUS_ERROR, if an unknown error occurred. \n
 *                #NVDM_MODEM_STATUS_INVALID_PARAMETER, if the parameter is invalid. \n
 *                #NVDM_MODEM_STATUS_INSUFFICIENT_SPACE, if the storage space is not enough. \n
 * @note      Call this API in tasks or function callbacks, but not in the interrupt handlers.
 */
nvdm_modem_status_t nvdm_modem_write_otp_data_item(const char *data_item_name,
                                   const uint8_t *buffer,
                                   uint32_t size);

/**
 * @brief     This function is used to lock the otp area of flash.
 *            Once lock-down the otp area, any data on otp area cannot be changed any more
 * @return
 *                #NVDM_MODEM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_MODEM_STATUS_ERROR, if an unknown error occurred. \n
 * @note      Call this API in tasks or function callbacks, but not in the interrupt handlers.
 */
nvdm_modem_status_t nvdm_modem_lock_otp_area(void);




#ifdef __cplusplus
}
#endif

/**
* @}
*/

#endif

#endif /* __NVDM_MODEM_H__ */

