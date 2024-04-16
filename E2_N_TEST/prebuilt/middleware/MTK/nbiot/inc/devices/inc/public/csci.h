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

#ifndef __CSCI_H__
#define __CSCI_H__


/**
 * @addtogroup CSCI
 * @{
 * This section introduces the CSCI APIs.
 *
 * @section CSCI_Architecture_Chapter Software architecture of CSCI
 *    CSCI provides a fixed-size data block exchange service between AP domain and MD domain.
 *    In this context AP domain and MD domain reside in the same SW load (i.e. are executed on the same CPU).
 *    Internally CSCI uses the Queue Management API provided by FreeRTOS.
 *    FreeRTOS supports message exchange across security domains using the Queue Management API.
 *    However, the Queue Management API allows only fixed-size data items per queue.
 *    CSCI supports only one user on the AP domain and one user on the MD domain.
 */


#ifdef __cplusplus
extern "C" {
#endif


/** @defgroup csci_enum Enum
  * @{
  */


/** @brief This enum defines return status of CSCI HAL public API. User should check return value after calling those APIs. */
typedef enum {
    CSCI_STATUS_OK = 0,                         /**< Indicates CSCI function executed successfully */
    CSCI_STATUS_ERROR,                          /**< Indicates CSCI driver detected a common error */
    CSCI_STATUS_ERROR_PARAMETER,                /**< Indicates user's parameter is invalid */
    CSCI_STATUS_ERROR_UNINITIALIZED,            /**< Indicates CSCI has not been initialized */
    CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_DATA,  /**< Indicates CSCI queue has insufficient data available */
    CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_SPACE  /**< Indicates CSCI queue has insufficient space to hold the data */
} csci_status_t;


/** @brief This enum defines CSCI event when an interrupt occurs */
typedef enum {
    CSCI_EVENT_READY_TO_READ = 1,          /**< Indicates that there are data blocks waiting to be received */
    CSCI_EVENT_READY_TO_WRITE = 2          /**< Indicates that there is space available to send data blocks */
} csci_callback_event_t;


/**
  * @}
  */


/** @defgroup csci_typedef Typedef
  * @{
  */


/** @brief This typedef defines user's callback function prototype.
 *             This callback function will be called in CSCI interrupt handler when CSCI interrupt is raised.
 *             User on the AP domain should call csci_ap_register_callback() to register a callback to CSCI.
 *             User on the MD domain should call csci_md_register_callback() to register a callback to CSCI.
 *             Note, that the callback function is not appropriate for time consuming operations. \n
 *             parameter "event" : for more information, please refer to description of #csci_callback_event_t.
 *             parameter "user_data" : a user defined data used in the callback function.
 */
typedef void (*csci_callback_t)(csci_callback_event_t event, void *user_data);


/**
  * @}
  */


/**
 * @brief     This function initializes CSCI.
 *               The CSCI block size is essentially the uxItemSize given in xQueueCreate().
 *               If this value is too small user data needs many messages to transfer.
 *               If this value is too large control data wasts excessive MIPS due to unnecessary copying.
 *               Recommended value for CSCI block size is 64.
 *               The CSCI queue length is essentially the uxQueueLength given in xQueueCreate().
 *               This should be large enough to hold at least 2 packets of MTU size (MTU is usually 1500 bytes).
 *               If the CSCI block size is set to 64 then the CSCI queue length is recommended to be at least 48.
 * @param[in] block_size     sets the size of the data blocks exchanged via CSCI.
 * @param[in] queue_length   sets the maximum number of data blocks CSCI can hold in a given direction (AP-to-MD or MD-to-AP) at any given point in time.
 * @return
 *                #CSCI_STATUS_OK if OK. \n
 *                #CSCI_STATUS_ERROR if an internal error occurred. \n
 *                #CSCI_STATUS_ERROR_PARAMETER if a parameter is invalid.
 */
csci_status_t csci_init(uint32_t block_size, uint32_t queue_length);


/**
 * @brief     This function registers the AP callback function with CSCI.
 *                This callback is used to indicate pending data blocks from MD.
 *                Also this callback is called in the CSCI interrupt handler.
 * @param[in] user_callback   specifies user's callback
 * @param[in] user_data   specifies user's data for this callback
 * @return
 *                #CSCI_STATUS_OK if OK. \n
 *                #CSCI_STATUS_ERROR if an internal error occurred. \n
 *                #CSCI_STATUS_ERROR_UNINITIALIZED if the CSCI has not been initialized. \n
 *                #CSCI_STATUS_ERROR_PARAMETER if a parameter is invalid.
 */
csci_status_t csci_ap_register_callback(csci_callback_t user_callback,
                                        void *user_data);


/**
 * @brief     This function registers the MD callback function with CSCI.
 *                This callback is used to indicate pending data blocks from AP.
 *                Also this callback is called in the CSCI interrupt handler.
 * @param[in] user_callback   specifies user's callback
 * @param[in] user_data   specifies user's data for this callback
 * @return
 *                #CSCI_STATUS_OK if OK. \n
 *                #CSCI_STATUS_ERROR if an internal error occurred. \n
 *                #CSCI_STATUS_ERROR_UNINITIALIZED if the CSCI has not been initialized. \n
 *                #CSCI_STATUS_ERROR_PARAMETER if a parameter is invalid.
 */
csci_status_t csci_md_register_callback(csci_callback_t user_callback,
                                        void *user_data);


/**
 * @brief     This function sends data blocks from AP to MD.
 *               The function expects the data blocks to be contiguous in memory.
 *               Internally CSCI puts the data block content in the AP-MD queue.
 *               The size of a data block was defined when CSCI was initialised (see csci_init()).
 *               If the AP-MD queue has insufficient space CSCI will not send any data to MD and
 *               CSCI will notify the user when there is sufficient space in the queue again.
 *               User should check the return value to make sure that data is successfully sent.
 * @param[in] first_data_block  specifies pointer to the user's data buffer.
 * @param[in] block_num         number of data blocks to send to MD.
 * @return
 *                #CSCI_STATUS_OK if OK. \n
 *                #CSCI_STATUS_ERROR if an internal error occurred. \n
 *                #CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_SPACE if the queue has insufficient space for the number of data blocks. \n
 *                #CSCI_STATUS_ERROR_UNINITIALIZED if the CSCI has not been initialized. \n
 *                #CSCI_STATUS_ERROR_PARAMETER if a parameter is invalid.
 * @note
 *            User must call csci_init() before calling this function.
 */
csci_status_t csci_ap_send_blocks_to_md(const uint8_t *first_data_block, uint32_t block_num);


/**
 * @brief     This function sends data blocks from MD to AP.
 *               The function expects the data blocks to be contiguous in memory.
 *               Internally CSCI puts the data block content in the MD-AP queue.
 *               The size of a data block was defined when CSCI was initialised (see csci_init()).
 *               If the MD-AP queue has insufficient space CSCI will not send any data to AP and
 *               CSCI will notify the user when there is sufficient space in the queue again.
 *               User should check the return value to make sure that data is successfully sent.
 * @param[in] first_data_block  specifies pointer to the user's data buffer.
 * @param[in] block_num         number of data blocks to send to AP.
 * @return
 *                #CSCI_STATUS_OK if OK. \n
 *                #CSCI_STATUS_ERROR if an internal error occurred. \n
 *                #CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_SPACE if the queue has insufficient space for the number of data blocks. \n
 *                #CSCI_STATUS_ERROR_UNINITIALIZED if the CSCI has not been initialized. \n
 *                #CSCI_STATUS_ERROR_PARAMETER if a parameter is invalid.
 * @note
 *            User must call csci_init() before calling this function.
 */
csci_status_t csci_md_send_blocks_to_ap(const uint8_t *first_data_block, uint32_t block_num);


/**
 * @brief     This function receives data blocks from MD.
 *               The function provides the data blocks to a contiguous memory area.
 *               Internally CSCI gets the data block content from the MD-AP queue.
 *               The size of a data block was defined when CSCI was initialised (see csci_init()).
 *               If the MD-AP queue has insufficient number of data blocks
 *               CSCI will not provide any data to AP and
 *               CSCI will notify the user when there are sufficient data blocks in the queue.
 *               User should check the return value to make sure that data is successfully received.
 * @param[in] first_data_block  specifies pointer to the user's data buffer.
 * @param[in] block_num         number of data blocks to receive from MD.
 * @return
 *                #CSCI_STATUS_OK if OK. \n
 *                #CSCI_STATUS_ERROR if an internal error occurred. \n
 *                #CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_DATA if the queue does not contain the requested number of data blocks. \n
 *                #CSCI_STATUS_ERROR_UNINITIALIZED if the CSCI has not been initialized. \n
 *                #CSCI_STATUS_ERROR_PARAMETER if a parameter is invalid.
 * @note
 *            User must call csci_init() before calling this function.
 */
csci_status_t csci_ap_receive_blocks_from_md(uint8_t *first_data_block, uint32_t block_num);


/**
 * @brief     This function receives data blocks from AP.
 *               The function provides the data blocks to a contiguous memory area.
 *               Internally CSCI gets the data block content from the AP-MD queue.
 *               The size of a data block was defined when CSCI was initialised (see csci_init()).
 *               If the AP-MD queue has insufficient number of data blocks
 *               CSCI will not provide any data to MD and
 *               CSCI will notify the user when there are sufficient data blocks in the queue.
 *               User should check the return value to make sure that data is successfully received.
 * @param[in] first_data_block  specifies pointer to the user's data buffer.
 * @param[in] block_num         number of data blocks to receive from AP.
 * @return
 *                #CSCI_STATUS_OK if OK. \n
 *                #CSCI_STATUS_ERROR if an internal error occurred. \n
 *                #CSCI_STATUS_ERROR_QUEUE_INSUFFICIENT_DATA if the queue does not contain the requested number of data blocks. \n
 *                #CSCI_STATUS_ERROR_UNINITIALIZED if the CSCI has not been initialized. \n
 *                #CSCI_STATUS_ERROR_PARAMETER if a parameter is invalid.
 * @note
 *            User must call csci_init() before calling this function.
 */
csci_status_t csci_md_receive_blocks_from_ap(uint8_t *first_data_block, uint32_t block_num);


/**
 * @brief     This function queries the number of free spaces available in the AP-MD queue.
 * @return
 *                The number of free spaces available in the AP-MD queue.
 */
uint32_t csci_ap_get_available_send_space();


/**
 * @brief     This function queries the number of free spaces available in the MD-AP queue.
 * @return
 *                The number of free spaces available in the MD-AP queue.
 */
uint32_t csci_md_get_available_send_space();


/**
 * @brief     This function queries the number of data blocks in the MD-AP queue.
 * @return
 *                The number of data blocks in the MD-AP queue.
 */
uint32_t csci_ap_get_number_of_waiting_receive_blocks();


/**
 * @brief     This function queries the number of data blocks in the AP-MD queue.
 * @return
 *                The number of data blocks in the AP-MD queue.
 */
uint32_t csci_md_get_number_of_waiting_receive_blocks();


/**
 * @brief     This function indicates to MD that there is data available on the AP-MD queue.
 *               Internally CSCI triggers a SW interrupt on the MD side when this function is called.
 *               User should call this function once data blocks are placed in the AP-MD queue using csci_ap_send_blocks_to_md().
 * @return
 *                #CSCI_STATUS_OK if OK. \n
 *                #CSCI_STATUS_ERROR_UNINITIALIZED if the CSCI has not been initialized. \n
 */
csci_status_t csci_ap_indicate_data_available_to_md();


/**
 * @brief     This function indicates to AP that there is data available on the MD-AP queue.
 *               Internally CSCI triggers a SW interrupt on the AP side when this function is called.
 *               User should call this function once data blocks are placed in the MD-AP queue using csci_md_send_blocks_to_ap().
 * @return
 *                #CSCI_STATUS_OK if OK. \n
 *                #CSCI_STATUS_ERROR_UNINITIALIZED if the CSCI has not been initialized. \n
 */
csci_status_t csci_md_indicate_data_available_to_ap();


#ifdef __cplusplus
}
#endif


/**
* @}
*/

#endif /* !__CSCI_H__ */

