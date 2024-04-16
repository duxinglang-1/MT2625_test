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
 * GKI header for unit queue API.
 **************************************************************************/

#ifndef GKIQUEUE_H
#define GKIQUEUE_H

/** \addtogroup PrdGki
 * @{
 */

#if defined (__cplusplus)
extern "C"
{
#endif

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <gkisig.h>
#include <ki_unitq.h>

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/** Initialisation value for a #KiUnitQueue.
 * All unit queues must be initialised with this value before use with any
 * of the unit queue functions. */
extern const KiUnitQueue  kiNullUnitQueue;

/***************************************************************************
 * Kernel Function redirections
 **************************************************************************/

/** Add a signal to a unit queue.
 * This function add the given signal to the given unit queue.
 *
 * \param[in] Q A pointer to the #KiUnitQueue which will have the signal
 *                 added to it.
 * \param[in] B A pointer to a #SignalBuffer identifying the signal to be
 *                 added to the queue. The signal buffer will be set to
 *                 #kiNullBuffer on exit from this function. */
#define     KiEnqueue(Q,B)                KiOsEnqueue (Q,B)

/** Remove a signal from a unit queue.
 * This function is used to remove a signal from a unit queue. The caller
 * must check that there is a signal on the unit queue before calling this
 * function (use either #KiOnQueue() or #KiNumOnQueue()).
 *
 * \param[in] Q A pointer to the #KiUnitQueue which will have the signal
 *                 removed from it.
 * \param[out] B A pointer to a #SignalBuffer to store the removed signal
 *                 in. */
#define     KiDequeue(Q,B)                KiOsDequeue (Q,B)

/** Check if there are any signals on a unit queue.
 * This function is used to check if there are any signals in a unit queue.
 *
 * \param[in] Q A pointer to the #KiUnitQueue to check.
 *
 * \return A #Boolean indicating whether there is at least one signal (TRUE)
 *             or not (FALSE) on the unit queue. */
#define     KiOnQueue(Q)                  KiOsOnQueue (Q)

/** Get the number of signals on a unit queue.
 * This function is used to get the number of signals on a unit queue.
 *
 * \param[in] Q A pointer to the #KiUnitQueue to get the signal count for.
 *
 * \return A #Int32 giving the number of signals on the queue. */
#define     KiNumOnQueue(Q)               KiOsNumOnQueue (Q)

/** Flush a unit queue.
 * This function is used to delete all signals on a unit queue and delete
 * any memory used for the queue storage. If a unit queue is created
 * locally (e.g. on the stack or allocated) rather than globally then this
 * function must be called before the #KiUnitQueue structure goes out of
 * scope or is deleted. This is required to prevent any memory leaks.
 *
 * \note If any of the signals held on the queue contain pointers to
 * allocated memory this function will not know about these pointers.
 * In this case the user should manually remove and delete all signals
 * (and embedded pointers) on the queue and then call KiFlushQueue().
 *
 * \param[in] Q A pointer to the #KiUnitQueue to flush. */
#define     KiFlushQueue(Q)               KiOsFlushQueue (Q)

/***************************************************************************
 * Function prototypes
 **************************************************************************/

void       KiOsEnqueue             (KiUnitQueue         *queue,
                                    SignalBuffer        *signal);

void       KiOsDequeue             (KiUnitQueue         *queue,
                                    SignalBuffer        *signal);

Boolean    KiOsOnQueue             (KiUnitQueue         *queue);

Int32      KiOsNumOnQueue          (KiUnitQueue         *queue);

void       KiOsFlushQueue          (KiUnitQueue         *queue);

#if defined (__cplusplus)
}
#endif

/** @} */ /* End of PrdGki group */

#endif

/* END OF FILE */
