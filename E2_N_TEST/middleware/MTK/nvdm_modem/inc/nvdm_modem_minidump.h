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

#ifndef __NVDM_MODEM_MINIDUMP_H__
#define __NVDM_MODEM_MINIDUMP_H__

#ifdef MTK_NVDM_MODEM_ENABLE

/**
 * @brief     This function is used to query the number of total mini dump on NVDM.
 *            If any error found, it returns with error status.
 *            User should check the return status to verify whether the query is successful.
 * @param[out] data_number   is the total number of mini dump.
 * @return
 *                #NVDM_MODEM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_MODEM_STATUS_ERROR, if an unknown error occurred. \n
 * @note      Call this API in tasks or function callbacks, but not in the interrupt handlers.
 */
nvdm_modem_status_t nvdm_modem_query_mini_dump_number(uint8_t *dump_number);

/**
 * @brief     This function is used to write mini dump on NVDM.
 *            If any error found, it returns with error status.
 *            User should check the return status to verify whether the mini dump is written to flash successfully.
 * @param[in] buffer   is the pointer to the data item's content.
  * @param[in] size   is the size of the data item's content.
 * @return
 *                #NVDM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_STATUS_ERROR, if an unknown error occurred. \n
 *                #NVDM_STATUS_INVALID_PARAMETER, if the parameter is invalid. \n
 * @note
 */
nvdm_modem_status_t nvdm_modem_write_mini_dump_data(uint8_t *buffer, 
                                uint16_t size);

/**
 * @brief     This function is used to read mini dump from NVDM.
 *            If any error found, it returns with error status.
 *            User should check the return status to verify whether the mini dump read from the flash is correct.
 * @param[in] dump_index   is the index of mini dump, index can be 0, 1, ..., N-1.
 * @param[in] offset   is the offset of mini dump data.
 * @param[in] buffer   is the pointer to the data item's content.
  * @param[in,out] size   is the user buffer size when used as an input and is the size of actual mini dump content read out when used as an output.
 * @return
 *                #NVDM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_STATUS_ERROR, if an unknown error occurred. \n
 *                #NVDM_STATUS_INVALID_PARAMETER, if the parameter is invalid. \n
 *                #NVDM_MODEM_STATUS_ITEM_NOT_FOUND, if the mini dump data is not found. \n
 * @note      Call this API in tasks or function callbacks, but not in the interrupt handlers.
 */
nvdm_modem_status_t nvdm_modem_read_mini_dump_data(uint8_t dump_index, 
                                uint16_t offset,
                                uint8_t *buffer,
                                uint16_t *size);

/**
 * @brief     This function is used to clean specific mini dump data on NVDM.
 *            If any error found, it returns with error status.
 *            User should check the return status to verify whether the mini dump is cleaned successfully.
 * @param[in] dump_index   is the index of total mini dump, index can be 0, 1, ..., N-1. The smaller index means the newest dump item.
 * @return
 *                #NVDM_STATUS_OK, if the operation completed successfully. \n
 *                #NVDM_STATUS_INVALID_PARAMETER, if the parameter is invalid. \n
 * @note      Call this API in tasks or function callbacks, but not in the interrupt handlers.
 */
nvdm_modem_status_t nvdm_modem_clean_mini_dump_data(uint8_t dump_index);


/**
 * @brief     This function is used to initialize the mini dump service

 * @return
 *                #NVDM_STATUS_OK, if the operation completed successfully. \n
 */
void nvdm_modem_mini_dump_init(void);

#endif

#endif /* __NVDM_MODEM_MINIDUMP_H__ */
