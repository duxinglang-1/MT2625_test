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
 * GKI header for semaphore API.
 **************************************************************************/

#ifndef GKISEMA_H
#define GKISEMA_H

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
#include <ki_sema.h>

/***************************************************************************
 * Defines
 **************************************************************************/

/** Indicates that the new Semaphore API is present.
 * This define indicates that this version of the GKI has the new semaphore
 * API. This can be used to conditionally compile code that needs to
 * support older versions of the API. This is used in geniexec.c to detect
 * whether kisyssem.h is present or not. */
#define KI_NEW_SEMAPHORE_API

/***************************************************************************
 * Kernel Function redirections
 **************************************************************************/

/** Increment a semaphores count from a task context.
 * This function is used to increment the count of the given semaphore
 * from a task context. If a task of higher priority is waiting on this
 * semaphore a task switch will occur to the higher priority task before
 * returning from this function.
 *
 * \param[in] S The #KiSemaphoreId of the semaphore whose count is to be
 *                  increased. */
#define     KiIncSemaphore(S)             KiOsIncSemaphore (S)

/** Increment a semaphores count from an interrupt context.
 * This function is used to increment the count of the given semaphore
 * from an interrupt context. This function may trigger a task switch
 * after all nested interrupts have completed ia a task waiting on this
 * semaphore is of higher priority than the task that was interrupted.
 *
 * \param[in] S The #KiSemaphoreId of the semaphore whose count is to be
 *                  increased. */
#define     KiIncIntSemaphore(S)          KiOsIncIntSemaphore (S)

/** Wait for a semaphore from a task context.
 * This function is used to wait on a semaphore from a task context. If
 * the semaphore count is 0 the task will be blocked until the semaphore
 * is not zero and the calling task is the highest priority task waiting
 * on the semaphore. If the semaphore count is not zero the count will
 * be reduced by 1 and the function will return.
 *
 * \param[in] S The #KiSemaphoreId of the semaphore to wait for. */
#define     KiWaitSemaphore(S)            KiOsWaitSemaphore (S)

/** Poll a semaphore from a task context.
 * This function is used to poll a semaphore from a task context. If the
 * semaphore count is 0 the task will return FALSE. If the semaphore
 * count is not zero the count will be reduced by 1 and the function
 * will return TRUE. A call to this function will not block the calling
 * task.
 *
 * \param[in] S The #KiSemaphoreId of the semaphore to poll.
 *
 * \return A #Boolean that indicates whether the semaphore was 0 (FALSE)
 *             or not (TRUE). */
#define     KiPollSemaphore(S)            KiOsPollSemaphore (S)

/***************************************************************************
 * Function prototypes
 **************************************************************************/

void       KiOsIncSemaphore        (KiSemaphoreId kiSemId);

void       KiOsIncIntSemaphore     (KiSemaphoreId kiSemId);

void       KiOsWaitSemaphore       (KiSemaphoreId kiSemId);

Boolean    KiOsPollSemaphore       (KiSemaphoreId kiSemId);

#if defined (__cplusplus)
}
#endif

/** @} */ /* End of PrdGki group */

#endif

/* END OF FILE */
