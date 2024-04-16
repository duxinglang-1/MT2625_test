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
#ifndef _L4MM_API_H_
#define _L4MM_API_H_

#include <system.h>

/**
 * @brief L4MM Pool identifiers.
 */
typedef enum L4mmPoolIdTag
{
    L4MM_UL_POOL,                      /* Uplink pool */
    L4MM_DL_POOL,                      /* Downlink pool */
    L4MM_AP_BRIDGE_POOL,               /* AP Bridge pool */
    L4MM_DEFAULT_POOL                  /* Default pool */
} L4mmPoolId;

/**
 * @brief Type for callback function pointer.
 *        Callback function is called by L4mm when:
 *          1) Occupancy threshold level is achieved,
 *          2) After 1), when occupancy hysteresis point is achieved.
 * @param transactionId Transaction ID that is used by client to determine
 *        if the callback event shall be handled or not. Only events which
 *        transaction ID is bigger than the biggest transaction ID received
 *        so far shall be handled.
 * @param isFillingUp TRUE if UL buffer is getting full (event 1 triggered),
          and FALSE if the memory occupation is decreasing (event 2 triggered).
 */
typedef void (*L4mmBufStatusCallback)(uint8_t transactionId, bool isFillingUp);

/**
 * @brief Function (re-)initializes L4MM. All allocations made on L4MM are
 *        freed, and configured callback function is cleared.
 *        NOTE: Transaction ID of UL buffer status callback is not reset on
 *              re-initialization!
 **/
void L4mmInitialize(void);

/**
 * @brief Function allocates a memory of a given size for data.
 * @param poolId PoolId to be be used when allocating memory.
 * @param length Length of the memory to be allocated.
 * @param mem_pp memory handle for allocated memory.
 * @return return status indicates whether the allocation was successful or not.
 */
bool L4mmAlloc(L4mmPoolId poolId, uint32_t requestedBytes, uint8_t **mem_pp);

/**
 * @brief Function frees a given memory allocated for data.
 *        Before this function deallocates the memory, the function
 *        checks which pool the memory belongs to.
 *        Note also that this function also sets given memory pointer to PNULL.
 * @param memPtr Handle to memory to be freed.
 * @param length Length of the memory to be freed.
 */
void L4mmFree(uint8_t **mem_pp, uint32_t length);

/**
 * @brief Function frees a given memory allocated for UL data.
 *        Unlike L4mmFree function, this free function doesn't set given
 *        memory pointer to PNULL. This function can be used in special cases,
 *        when a client wants to deallocate only a part of data block.
 * @param length Length of the memory to be freed.
 */
void L4mmUlFree(uint8_t* memPtr, uint32_t length);

/**
 * @brief Function is used for registering a callback function for flow
 *        control.
 *        The system support only UL and AP Bridge callbacks at the moment.
 *        - For AP bridge pool, only one callback function may be configured at a time,
 *        so new registration overwrites the last configured value.
 *        - For UL pool, maximum number of callback functions to be registered at a time
 *        can be configured by L4MM_MAX_NUM_UL_CALLBACKS defined in l4mm_src.h.
 * @param callbackFunction Pointer to a flow control callback function.
 * @param Pool Id.
 */
void L4mmRegCallback(L4mmBufStatusCallback callbackFunction, L4mmPoolId poolId);

/**
 * @brief Function is used for deregistering a callback function for flow
 *        control. The system support only deregistering of UL and AP Bridge
 *        callbacks at the moment.
 * @param callbackFunction Pointer to a flow control callback function.
 * @param Pool Id.
 */
void L4mmRemoveCallback(L4mmBufStatusCallback callbackFunction, L4mmPoolId poolId);

/**
 * @brief Function is used for checking whether a pool flow control level has been reached.
 * @param Pool Id.
 * @return return status indicates whether the flow control level has been reached or not.
 */
bool L4mmIsFlowControlLevelReached(L4mmPoolId poolId);

#ifdef UNIT_TEST
/**
 * @brief Function destroys L4MM, and dynamic memory allocated by L4MM is freed.
 */
void L4mmDestroy(void);
#endif /* UNIT_TEST */
#endif /* _L4MM_API_H_ */
