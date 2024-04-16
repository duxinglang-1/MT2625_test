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
 *************************************************************************/
/** \file
 * GKI low level system types
 **************************************************************************/

#ifndef KISYSTYP_H
#define KISYSTYP_H

/** \addtogroup PrdGki
 * @{
 */

/******************************************************************************
* Includes
******************************************************************************/

#include <system.h>
#include <kicfg.h>
#include <kitqid.h>

/******************************************************************************
* Config
******************************************************************************/

/** \def KI_ENABLE_OWN_QUEUEING
 * GKI own queueing mechanism.
 * This conditional enables the GKI's own queuing mechanism rather than
 * depending on the OS to provide a queuing mechanism. When this is defined
 * the OS port does not need to provide implementations for the following
 * macros:
 *
 * - #M_KiOsSendSignalInt()
 * - #M_KiOsSendSignal()
 * - #M_KiOsReceiveSignal()
 * - #M_KiOsReceiveSignalPoll()
 * - #M_KiOsCreateInputQueue()
 *
 * Queue statistics can be enabled using the #KI_QUEUE_STATISTICS conditional.
 */

/** \def KI_MEMORY_USAGE
 * Enable memory usage debug.
 * This conditional enables additional debug code that can be used to help
 * debug memory problems. When this is enabled all GKI memory allocations
 * have the #KiMemDebugInfo structure added to their header. Some of this
 * information can be extracted by sending a #KiDynamicMemUsageReq signal
 * into the system. All the information will be present in a RAM log (if
 * enabled, see #KI_ENABLE_RAM_LOGGING). */

#if defined (KI_MEM_POOL_STATISTICS) && !defined (KI_MEMORY_USAGE)
# error KI_MEM_POOL_STATISTICS requires KI_MEMORY_USAGE to be defined
#endif

/******************************************************************************
* Typedefs
******************************************************************************/

/** GKI memory priority.
 * This enumeration indicates whether or not the associated memory block was
 * allocated or requested. Used when freeing the block to know whether or not
 * to decrement the kiBlocksRequested[] entry for the associated pool. */
typedef enum KiMemoryPriorityTag
{
    KI_ALLOC_MEMORY,
    KI_REQUEST_MEMORY,

    KI_NUM_MEMORY_PRIORITIES
}
KiMemoryPriority;

/** Internal signal format.
 * This enumeration identifies the internal format of a signal. */
typedef enum KiSignalFormatTag
{
    KI_NO_FORMAT,
    KI_SIGNAL_FORMAT,
    KI_COMMAND_FORMAT,
    KI_LOGGED_SIGNAL_FORMAT,
    #if defined (KI_ENABLE_LOGGING_SIGNALS_OVER_HSL)
    KI_LOGGED_SIGNAL_FORMAT_HSL,
    #if !defined (KI_ENABLE_LOGGING_SIGNALS_OVER_HSL_DMA)
    KI_LOGGED_SIGNAL_FORMAT_HSL_NEXT,
    #endif
	#endif
    KI_NUM_SIGNAL_FORMATS

} KiSignalFormat;

/** Memory allocation source id.
 * This enumeration identifies all the possible sources for memory block
 * allocation. */
typedef enum KiMemAllocSourceTag
{
    KI_MAS_ENQUEUE,
    KI_MAS_REQUEST_MEM,
    KI_MAS_ALLOC_MEM,
    KI_MAS_REALLOC_MEM,
    KI_MAS_REREQUEST_MEM,
    KI_MAS_ALLOC_POLL_MEM,

    /* Keep all id's that indicate signal memory at the end of this
     * enumeration. This is used to identify a block's use when dumping
     * information from a memory pool, see #KiOsIsMemorySignal. It is
     * useful to be able to distinguish between signal memory and
     * other memory. */
    KI_MAS_SIGNAL_MEM_START   = 0x80,
    KI_MAS_CREATE_SIGNAL      = KI_MAS_SIGNAL_MEM_START,
    KI_MAS_CREATE_INT_SIGNAL,
    KI_MAS_REQUEST_SIGNAL,
    KI_MAS_GET_SIGNAL_BLOCK,

    KI_MAS_NUM_ALLOC_SRCS

} KiMemAllocSource;

#if defined (KI_MEMORY_USAGE)
/** Memory debug info.
 * This structure is used to hold useful information for debugging memory
 * related issues. This is only defined if #KI_MEMORY_USAGE is defined. */
typedef struct KiMemDebugInfoTag
{
    /** Address of calling code.
     * This is used to store the address of the code that called the GKI
     * function that allocated the associated memory. */
    Int32                           callerAddress;

# if defined (KI_MEMORY_TIMESTAMP)
    /** Time at which the memory was allocated.
     * This is a timestamp of when the associated memory was allocated.
     * Nominally this will be in kernel ticks (allows alignment with
     * GKI logs) but there may be other timebases used in the future. */
    Int32                           timestamp;
# endif

    /** Caller GKI task id.
     * This is the GKI task id of the task that called the GKI function
     * that allocated the associated memory. */
    TaskId                          taskId;

    /** Function used to allocate the memory.
     * This is an id that indicates what function allocated the
     * associated memory. */
    KiMemAllocSource                allocSource;
}
KiMemDebugInfo;
#endif

/** Generic memory header.
 * This header is shared between the signal and memory headers. */
typedef struct KiGenMemHeaderTag
{
#if defined (KI_MEMORY_USAGE) /* For memory leak debuging */
    /* Must be first field */
    /** Memory debug info.
     * This is used to store information required for debugging memory related
     * problems. This is only present if #KI_MEMORY_USAGE is defined. */
    KiMemDebugInfo                  debugInfo;
#endif

    /** Memory pool id.
     * This is actually set to a member of the #KiMemoryPoolId enumeration.
     * This indicates what memory resource the signal memory was allocated
     * from. */
    Int8                            pool;

    /** Memory priority.
     * This indicates whether the associated signal memory was allocated
     * or requested. */
    KiMemoryPriority                priority;

} KiGenMemHeader;

/** Internal Signal header.
 * This structure is the header added by the GKI to signals. Application code
 * does not have access to this. */
typedef struct KiSigHeaderTag
{
    /** Generic header.
     * This is a generic memory header containing debug, pool and priority
     * information. Must be first in this structure. */
    KiGenMemHeader                  genHeader;

#if defined (KI_MEM_POOL_STATISTICS)
    /** Length of signal.
     * This is the length of the signal body. This is only present if
     * #KI_MEM_POOL_STATISTICS is defined.*/
    Int16                           length;
#endif

    /** Signal body format.
     * This indicates the format of the signal body e.g. signal, logged
     * signal or command. */
    KiSignalFormat                  format;

#if defined (KI_ENABLE_OWN_QUEUEING)
    /** Signal queue.
     * Pointer to the next signal in a FIFO signal queue. */
    volatile struct KiSigHeaderTag *queue;
#endif
}
KiSigHeader;

#if defined (KI_ENABLE_OWN_QUEUEING)
/** Signal Queue
 * This structure is used to store a queue of GKI signals. */
typedef struct KiFifoSigQueueTag
{
    /** Head of queue.
     * This is the head of the signal queue. If this is NULL there are no signals
     * on the queue. */
    volatile KiSigHeader     *head;
    /** Tail of queue.
     * This is a pointer to the tail. If this is NULL then the queue is invalid.
     * The value in *tail should always be NULL. For an empty (valid) queue
     * this member points to the head member. */
    volatile KiSigHeader    **tail;

    /** Queue signal count.
     * This is the count of the number of signals currently on the queue. */
    Int32            count;

#if defined (KI_QUEUE_STATISTICS)
    /** Queue maximum signal count.
     * This is the count of the maximum number of signals that have been on the
     * queue. This is only present if #KI_QUEUE_STATISTICS is defined. */
    Int32            maxCount;
#endif

} KiFifoSigQueue;
#endif

/** @} */ /* End of PrdGki group */

#endif /* KISYSTYP_H */
