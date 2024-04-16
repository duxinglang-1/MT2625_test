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
 * GKI header file for low level system code.
 **************************************************************************/

#ifndef KISYS_H
#define KISYS_H

/** \addtogroup PrdGki
 * @{
 */

/******************************************************************************
* Includes
******************************************************************************/
#include "nbiot_modem_common_config.h"
#include <system.h>
#include <kisystyp.h>

/******************************************************************************
* Macros
******************************************************************************/


/** Clear queue counts.
 * This clears the statistics counters in a #KiFifoSigQueue structure if
 * #KI_QUEUE_STATISTICS is defined (otherwise it does nothing). This
 * is only available when #KI_ENABLE_OWN_QUEUEING is defined.
 *
 * \param[out] fIFOqUEUE This is the FIFO sig queue to clear. */
# if defined (KI_QUEUE_STATISTICS)
#  define M_KiSysClearQueueCounts(fIFOqUEUE) \
         {                                   \
             fIFOqUEUE.count    = 0;         \
             fIFOqUEUE.maxCount = 0;         \
         }
# else
#  define M_KiSysClearQueueCounts(fIFOqUEUE)
# endif



/** Create a tasks input signal queue.
 * This macro creates and initialises the input signal queue for the given task.
 * This is only available when #KI_ENABLE_OWN_QUEUEING is defined.
 *
 * \param[in] tASKiNITiNDEX This is the index into the #kiTaskInitTable for
 *                this task. */
#  define M_KiSysCreateInputQueue(tASKiNITiNDEX)    \
    {                                               \
        Int32 tASKiNDEX = M_KiTaskGetIndex(kiTaskInitTable[tASKiNITiNDEX].taskId); \
        /* Initialise the queue */                  \
        kiTaskQueues[tASKiNDEX].queue.head = NULL;  \
        kiTaskQueues[tASKiNDEX].queue.tail = &(kiTaskQueues[tASKiNDEX].queue.head); \
        M_KiSysClearQueueCounts(kiTaskQueues[tASKiNDEX].queue); \
        /* Create the queue semaphore */            \
        M_KiOsCreateSemaphoreEx(kiTaskQueues[tASKiNDEX].sema); \
    }


/** @} */ /* End of PrdGki group */

/******************************************************************************
* Prototypes
******************************************************************************/


void KiSysSendSignal       (KiTaskId taskId, KiSigHeader *headerPtr);
void KiSysSendSignalInt    (KiTaskId taskId, KiSigHeader *headerPtr);
void KiSysReceiveSignal    (KiTaskId taskId, void **blockPtr);
void KiSysReceiveSignalPoll(KiTaskId taskId, void **blockPtr);
void KiSysReceiveIntSignalPoll(KiTaskId taskId, void **blockPtr);
void KiTriggerWakeup(bool int_context);
#ifdef KI_ENABLE_SUBSCRIBE_EVENT_SIGNAL
typedef void (*SignalActivityCB)(void);
void KiSysSendActiveSignal(KiTaskId taskId, KiSigHeader *headerPtr);
void KiSysSendActiveSignalInt(KiTaskId taskId, KiSigHeader *headerPtr);
void Ki_subscribeEventSignal(bool suspendSignal);
void KiRegisterSignalActivityCallback(SignalActivityCB normalCB, SignalActivityCB interruptCB);
#endif


#endif /* KISYS_H */

