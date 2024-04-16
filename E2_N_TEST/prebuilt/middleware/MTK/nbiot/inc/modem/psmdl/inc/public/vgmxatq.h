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
/*
 **************************************************************************/
/** \file
 * Definitions for the Voyager Multiplexer interface - AT Queue Functionality.
 **************************************************************************/
#ifndef VGMXATQ_H
#define VGMXATQ_H

/*****************************************************************************/
/* AT command queueing mechanism                                             */
/*****************************************************************************/


/** \def CFGVAL_VGMUX_AT_QUEUE_SIZE
 * The number of signals containing AT data that can be queued between the
 * CI Task and Multiplexer Task.
 */
#if !defined (CFGVAL_VGMUX_AT_QUEUE_SIZE)
#if defined (ENABLE_AT_ENG_MODE)
#define CFGVAL_VGMUX_AT_QUEUE_SIZE (60)   /* larger output queue required for AT*MENGMODE. */
#else
#define CFGVAL_VGMUX_AT_QUEUE_SIZE (64)
#endif
#endif /* !defined (CFGVAL_MUX_AT_QUEUE_SIZE) */

/** \def CFGVAL_VGMUX_AT_QUEUE_HWM
 * The high water mark for the AT signal queue which triggers the Xoff to
 * the CI task.
 */
#define CFGVAL_VGMUX_AT_QUEUE_HWM (CFGVAL_VGMUX_AT_QUEUE_SIZE - 3)

/** \def CFGVAL_VGMUX_AT_QUEUE_LWM
 * The low water mark for the AT signal queue which triggers the Xon to
 * the CI task.
 */
#define CFGVAL_VGMUX_AT_QUEUE_LWM 1

#if (CFGVAL_VGMUX_AT_QUEUE_LWM >= CFGVAL_VGMUX_AT_QUEUE_HWM)
# error "Error in Mux AT Queue high and low water marks. LWM must be less than the HWM."
#endif

/** AT data queue type.
 * This structure is used to contain a queue of AT data signals transmitted
 * between the CI Task and the Multiplexer Task.
 */
typedef struct VgmuxAtQueueTag
{
  /** An array of signals contained within the queue. */
  SignalBuffer     cache[CFGVAL_VGMUX_AT_QUEUE_SIZE];
  /** The number of signals contained in the cache array. */
  Int8             numberOfCachedSignals;
  /** Whether the queue is locked for access. */
  volatile Boolean locked;
  /** The start character in the current signal being read from. */
  Int8             startChar;
} VgmuxAtQueue;

/** \def VGMX_MAX_DATA_INDS
 * The maximum number of #VgmuxAtDataInd signals that may be allocated
 * simultaneously.
 */
#if !defined (VGMX_MAX_DATA_INDS)
# define VGMX_MAX_DATA_INDS (25)
#endif /* !defined (VGMX_MAX_ALLOCATED_DATA_INDS) */

/** \def VGMX_DATA_IND_LOW_WATER_MARK
 * The minimum number of #VgmuxAtDataInd signals that are allocated
 * before any new signals may be created after flow control measures
 * have been applied.
 */
#if !defined (VGMX_DATA_IND_LOW_WATER_MARK)
# define VGMX_DATA_IND_LOW_WATER_MARK (3)
#endif /* !defined (VGMX_DATA_IND_LOW_WATER_MARK) */

/** \def VGMX_DATA_IND_HIGH_WATER_MARK
 * The maximum number of #VgmuxAtDataInd signals that may be allocated
 * before flow control measures are applied and no new signals may be
 * created.
 */
#if !defined (VGMX_DATA_IND_HIGH_WATER_MARK)
# define VGMX_DATA_IND_HIGH_WATER_MARK (22)
#endif /* !defined (VGMX_DATA_IND_HIGH_WATER_MARK) */

#if VGMX_DATA_IND_LOW_WATER_MARK > VGMX_MAX_DATA_INDS
# error Data Ind low water mark too high
#endif /* VGMX_DATA_IND_LOW_WATER_MARK > VGMX_MAX_DATA_INDS */

#if VGMX_DATA_IND_HIGH_WATER_MARK > VGMX_MAX_DATA_INDS
# error Data Ind high water mark too high
#endif /* VGMX_DATA_IND_HIGH_WATER_MARK > VGMX_MAX_DATA_INDS*/

#if VGMX_DATA_IND_HIGH_WATER_MARK <= VGMX_DATA_IND_LOW_WATER_MARK
# error Mismatched high water marks
#endif /* VGMX_DATA_IND_HIGH_WATER_MARK <= VGMX_DATA_IND_LOW_WATER_MARK  */


/** Initialises the contents of an AT data queue.
 *
 * \param queue_p     [in] The AT data queue structure which holds the AT data.
 * \param firstTime   [in] Whether any data in the queue should be freed.
 * \param isInterrupt [in] Whether the signals in the queue are interrupt
 *                         signals.
 */
void vgmuxAtQueueInitialise (VgmuxAtQueue   *queue_p,
                             Boolean         firstTime,
                             Boolean         isInterrupt);

/** Adds a #VgmuxAtDataReq signal to an AT data queue.
 * This function adds a #VgmuxAtDataReq signal containing AT data to an AT
 * data queue. If the queue has passed the high water mark then the function
 * return value indicates whether flow controls measures need to be taken.
 *
 * \param queue_p  [in] The AT data queue on which to append an AT data signal.
 * \param signal_p [in] The signal (a #VgmuxAtDataReq) to append to the AT data
 *                      queue.
 *
 * \return Boolean - TRUE if we need to XOFF data transmission.
 */
Boolean vgmuxAtQueueAdd (VgmuxAtQueue   *queue_p,
                         SignalBuffer   *signal_p);

/** Adds a string to an AT data queue.
 * This function adds a specified AT data string (maximum length of 255 bytes)
 * to an AT data queue by creating a #VgmuxAtDataReq signal and appends it to
 * the AT data queue.
 *
 * \param queue_p  [in] The AT data queue on which to append an AT data string.
 * \param string_p [in] The AT data string to append to the AT data queue.
 * \param length   [in] The length of the AT data string.
 *
 * \return Boolean - TRUE if we need to XOFF data transmission.
 */
Boolean vgmuxAtQueueAddString (VgmuxAtQueue   *queue_p,
                               const Int8     *string_p,
                               Int8            length);

/** Removes a number of bytes from the front of an AT data queue.
 * This function retrieves a specified quantity of bytes (up to 65535) from
 * the AT data queue. Once the contents of any signals in the queue have been
 * retrieved then the empty signal is removed from the queue. If the queue now
 * has sufficient capacity for more signals, the return value indications
 * whether to allow further data transmissions.
 *
 * \param queue_p          [in]  The AT data queue to remove data from.
 * \param data_p           [in]  The location to copy AT data to.
 * \param length           [in]  The length of data that we should try and
 *                               retrieve.
 * \param returnedLength_p [out] The actual length of data retrieved, this will
 *                               never be greater than the length parameter
 *                               value.
 *
 * \return Boolean - TRUE if we are now able to XON.
 *
 * \see vgmuxAtQueueRemove
 */
Boolean vgmuxAtQueueRemoveInt16 (VgmuxAtQueue   *queue_p,
                                 Int8           *data_p,
                                 Int16           length,
                                 Int16          *returnedLength_p);

/** Removes a number of bytes from the front of an AT data queue.
 * This function retrieves a specified quantity of bytes (up to 255) from the
 * AT data queue. Once the contents of any signals in the queue have been
 * retrieved then the empty signal is removed from the queue. If the queue now
 * has sufficient capacity for more signals, the return value will indicate
 * whether to allow further data transmissions.
 *
 * \param queue_p          [in]  The AT data queue to remove data from.
 * \param data_p           [in]  The location to copy AT data to.
 * \param length           [in]  The length of data that we should try and
 *                               retrieve.
 * \param returnedLength_p [out] The actual length of data retrieved, this will
 *                               never be greater than the length parameter
 *                               value.
 *
 * \return Boolean - TRUE if we are now able to XON.
 *
 * \see vgmuxAtQueueRemoveInt16
 */
Boolean vgmuxAtQueueRemove (VgmuxAtQueue   *queue_p,
                            Int8           *data_p,
                            Int8            length,
                            Int8           *returnedLength_p);

/** Queries whether an AT data queue is empty.
 *
 * \param queue_p [in] The AT data queue to check for unprocessed AT data
 *                     signals.
 *
 * \return Boolean - TRUE if the queue is empty.
 */
Boolean vgmuxAtQueueIsEmpty (VgmuxAtQueue   *queue_p);
 /**************************************************************************/

/** Initialises the count of AT data indications which are allocated.
 * This function is used during multiplexer initialisation to ensure
 * the data indication signal count has been set to zero.
 *
 * \param isInterrupt [in] TRUE if interrupts are enabled.
 */
void vgmxAtInitialiseDataIndCount (Boolean isInterrupt);

/** Allocates a #VgmuxAtDataInd signal.
 * This function allocates a #VgmuxAtDataInd signal for AT data transmission
 * and increments the allocation count of such signals. The routine may not
 * return a signal at all if the maximum signal count allocation is exceeded.
 * The caller must cope with this.
 *
 * \param signal_p           [inout] The signal buffer structure to populate
 *                                   with a new #VgmuxAtDataInd signal.
 * \param isInterrupt        [in]    TRUE if we are operating in interrupt mode.
 * \param interruptsDisabled [in]    TRUE if interrupts are currently disabled,
 *                                   this is ignored if isInterrupt is TRUE.
 *
 * \return Boolean - TRUE if the number allocated now exceeds the high water
 *                   mark (#VGMX_DATA_IND_HIGH_WATER_MARK), that is when XOFF
 *                   should be sent.
 */
Boolean vgmuxAtAllocateDataInd (SignalBuffer *signal_p,
                                Boolean       isInterrupt,
                                Boolean       interruptsDisabled);

/** Indicate that the CI Task has finished with a #VgmuxAtDataInd signal.
 * This function is used when the CI Task has finished processing a
 * #VgmuxAtDataInd signal and it decrements the count of allocated
 * #VgmuxAtDataInd signals.
 *
 * \param isInterrupt [in] TRUE if we are operating in interrupt mode.
 *
 * \return Boolean - TRUE if the number of allocated signals has exceeded
 *                   the low water mark (#VGMX_DATA_IND_LOW_WATER_MARK) as
 *                   a result of this call, that is when XON should be sent.
 */
Boolean vgmuxAtFreeDataInd (Boolean isInterrupt);

#endif /* VGMXATQ_H*/
/* END OF FILE */

