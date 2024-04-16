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
 * GKI header that includes the OS specific file defining the main GKI OS
 * abstaraction macros.
 **************************************************************************/

#ifndef KIOS_H
#define KIOS_H

/** \addtogroup PrdGki
 * @{
 */

/** \defgroup GkiOsAbs GKI OS Abstraction
 * \ingroup Cfg
 * This section provides information on the OS abstraction within the GKI.
 * @{
 */

/***************************************************************************
 * Config
 **************************************************************************/

/** \def KI_OS_DEBUG
 * Enable OS specific debug in GKI macros.
 * This condtional can be used to enable OS specific debug within the GKI
 * OS abstraction macros. Exactly what debug this will enable depends on the
 * OS GKI port being used. A common feature is to decode OS error codes into
 * strings for display during an assert. */

/***************************************************************************
 * Nested Include Files
 **************************************************************************/
#include "nbiot_modem_common_config.h"
#include <system.h>
#include <kicfg.h>
#include <kicfgos.h>
#include <kisys.h>
#include <ki_event.h>
#include <sys_trace.h>

/* Include OS specific GKI header to get the OS abstraction macros. */
#if defined (MMK_KERNEL)
#  include <kimmkos.h>
#endif

#if defined (FRT_KERNEL)
#  include <kifrtos.h>
#endif

#if defined (RHINO_KERNEL)
#  include <kirhinoos.h>
#endif


#if !defined (KI_DISABLE_KM_DYNAMIC_MEMORY)
# include  <kmdynmem.h>
# if !defined (ON_PC) && !defined (ON_TARGET_UNIT_TEST)
#  include  <kiossem.h>
# endif
#endif

/***************************************************************************
 * Config
 **************************************************************************/

/** The number of OS reserved bytes at the end of a task stack.
 * This defines the number of bytes that the OS might place at the end of a
 * task stack and that shouldn't be modified by the GKI. The default value
 * is 0 but this can be overriden by the OS specific header file. */
#if !defined (KI_OS_TASK_STACK_END_MARKER_BYTES)
# define KI_OS_TASK_STACK_END_MARKER_BYTES      (0)
#endif

/** The number of OS reserved bytes at the end of an interrupt stack.
 * This defines the number of bytes that the OS might place at the end of an
 * interrupt stack and that shouldn't be modified by the GKI. The default
 * value is 0 but this can be overriden by the OS specific header file. */
#if !defined (KI_OS_INTERRUPT_STACK_END_MARKER_BYTES)
# define KI_OS_INTERRUPT_STACK_END_MARKER_BYTES (0)
#endif

/** Assert message length.
 * This define controls the maximum length of the assert message string
 * produced by the assert handling functions (assertfail and
 * checkDevParam). */
#if !defined (KI_ASSERT_MESSAGE_LEN)
# define KI_ASSERT_MESSAGE_LEN     64
#endif

/***************************************************************************
 * Macros
 **************************************************************************/

/* Some overrides depending on what conditionals are active. */

/***************************************************************************
 ***************************************************************************
 **  M_KiOs* Signalling Related Macros
 ***************************************************************************
 **************************************************************************/

/** Send a signal from an interrupt context.
 * Send the given signal data to the given task. This may cause the OS to
 * perform a task reschedule when all nested interrupts are complete.
 * This implementation is used when #KI_ENABLE_OWN_QUEUEING is defined.
 *
 * \param[in] tASKiD The #TaskId of the task which the signal is being sent to.
 * \param[in] bLOCKpTR Pointer to a #SignalStructure, #CommandStructure,
 *                #LoggedSignalStructure or #GenericStructure which is to be
 *                sent to the indicated task. The actual structure pointed to
 *                does not matter the pointer value itself is what should be
 *                transferred. */
# if defined (M_KiOsSendSignalInt)
#  undef M_KiOsSendSignalInt
# endif
#if defined(KI_ENABLE_SUBSCRIBE_EVENT_SIGNAL)
  #define M_KiOsSendSignalInt(tASKiD, bLOCKpTR)       KiSysSendActiveSignalInt(tASKiD, &(bLOCKpTR->header))
#else
  #define M_KiOsSendSignalInt(tASKiD, bLOCKpTR)       KiSysSendSignalInt(tASKiD, &(bLOCKpTR->header))
#endif

/** Send a signal from a task context.
 * Send the given signal data to the given task. This may cause the OS to
 * perform a task reschedule. This implementation is used when
 * #KI_ENABLE_OWN_QUEUEING is defined.
 *
 * \param[in] tASKiD The #TaskId of the task which the signal is being sent to.
 * \param[in] bLOCKpTR Pointer to a #SignalStructure, #CommandStructure,
 *                #LoggedSignalStructure or #GenericStructure which is to be
 *                sent to the indicated task. The actual structure pointed to
 *                does not matter the pointer value itself is what should be
 *                transferred. */
# if defined (M_KiOsSendSignal)
#  undef M_KiOsSendSignal
# endif
#if defined(KI_ENABLE_SUBSCRIBE_EVENT_SIGNAL)
  #define M_KiOsSendSignal(tASKiD, bLOCKpTR)          KiSysSendActiveSignal(tASKiD, &(bLOCKpTR->header))
#else /* KI_ENABLE_SUBSCRIBE_EVENT */
  #define M_KiOsSendSignal(tASKiD, bLOCKpTR)          KiSysSendSignal(tASKiD, &(bLOCKpTR->header))
#endif /* KI_ENABLE_SUBSCRIBE_EVENT */

/** Receive a signal and block if none available.
 * Wait for a signal arriving on the indicated input queue. If no signal is
 * available wait until there is, this will cause a task reschedule.
 * This implementation is used when #KI_ENABLE_OWN_QUEUEING is defined.
 *
 * \param[in] qUEUEiD The #QueueId of the queue on which to wait for a signal.
 * \param[out] bLOCKpTR Pointer to a #SignalStructure, #CommandStructure,
 *                 #LoggedSignalStructure or #GenericStructure which will be
 *                 set by this macro. The actual structure does not matter
 *                 it should just be treated as a pointer by this macro. */
# if defined (M_KiOsReceiveSignal)
#  undef M_KiOsReceiveSignal
# endif
# define M_KiOsReceiveSignal(qUEUEiD, bLOCKpTR)      KiSysReceiveSignal((KiTaskId)qUEUEiD, (void **)&(bLOCKpTR))

/** Poll for reception of a signal.
 * This macro is similar to #M_KiOsReceiveSignal() except that if no signal
 * is available it does not block, it just sets bLOCKpTR to NULL and finishes.
 * This implementation is used when #KI_ENABLE_OWN_QUEUEING is defined.
 *
 * \param[in] qUEUEiD The #QueueId of the queue on which to wait for a signal.
 * \param[out] bLOCKpTR Pointer to a #SignalStructure, #CommandStructure,
 *                 #LoggedSignalStructure or #GenericStructure which will be
 *                 set by this macro. The actual structure does not matter
 *                 it should just be treated as a pointer by this macro.
 *                 This pointer will be set to NULL if no signal is available. */
# if defined (M_KiOsReceiveSignalPoll)
#  undef M_KiOsReceiveSignalPoll
# endif
# define M_KiOsReceiveSignalPoll(qUEUEiD, bLOCKpTR)  KiSysReceiveSignalPoll((KiTaskId)qUEUEiD, (void **)&(bLOCKpTR))

/** Create a tasks input queue.
 * This macro is used to create an input signalling queue for the indicated
 * task. This implementation is used when #KI_ENABLE_OWN_QUEUEING is defined.
 * This is called as part of #KiReset at system initialisation time.
 *
 * \param[in] tASKiNDEX The index into the #kiTaskInitTable array indicating
 *                which task to create an input signalling queue for. */

/** Poll for reception of a signal from interrupt context.
 * This macro is similar to #M_KiOsReceiveSignalPoll() except that it can be used from the interrupt context */

# if defined (M_KiOsReceiveIntSignalPoll)
#  undef M_KiOsReceiveIntSignalPoll
# endif
# define M_KiOsReceiveIntSignalPoll(qUEUEiD, bLOCKpTR)  KiSysReceiveIntSignalPoll((KiTaskId)qUEUEiD, (void **)&(bLOCKpTR))

# if defined (M_KiOsCreateInputQueue)
#  undef M_KiOsCreateInputQueue
# endif
# define M_KiOsCreateInputQueue(tASKiNDEX)           M_KiSysCreateInputQueue(tASKiNDEX)


/***************************************************************************
 ***************************************************************************
 **  M_KiOs* Dynamic Heap Memory Related Macros
 ***************************************************************************
 **************************************************************************/

#if !defined (KI_DISABLE_DYNAMIC_MEMORY)
#if !defined (KI_DISABLE_KM_DYNAMIC_MEMORY)

/** Dynamic Heap memory manager overhead.
 * This is the number of bytes required by the OS at the start of the memory
 * pool that the memory manager needs to reserve to store its state
 * information. */
#define KI_DYNAMIC_MEM_MAN_OVERHEAD_BYTES  KM_MEMORY_CTRL_BLOCK_SIZE

/** Create the GKI dynamic heap memory pool.
 * This macro creates the dynamic memory pool. This is the implementation
 * used when #KI_DISABLE_KM_DYNAMIC_MEMORY is not defined. This is called
 * as part of #KiReset at system initialisation time.*/
#if defined (KI_CHECK_ASSERTIONS)
#define M_KiOsCreateDynamicMemPool()                                                  \
{                                                                                     \
    KmErrorCode      eRRORcODE;                                                       \
    /* Create the memory pool */                                                      \
    eRRORcODE = KmMemoryCreatePool (kiDynamicMemoryPool,                              \
                                    M_KmMemCalcRealPoolSize(KI_DYNAMIC_MEM_SIZE));    \
    /* Check that the memory pool was successfully allocated */                       \
    DevCheck (eRRORcODE == KM_ERR_OK, eRRORcODE, KI_DYNAMIC_MEM_SIZE, 0);             \
    eRRORcODE = KmMemoryCreatePool (kiDynamicNonCacheableMemoryPool,                                \
                                    M_KmMemCalcRealPoolSize(KI_DYNAMIC_NON_CACHEABLE_MEM_SIZE));    \
    /* Check that the memory pool was successfully allocated */                                     \
    DevCheck (eRRORcODE == KM_ERR_OK, eRRORcODE, KI_DYNAMIC_NON_CACHEABLE_MEM_SIZE, 0);             \
    eRRORcODE = KmMemoryCreatePool (kiDynamicNonCacheableIntMemoryPool,                                \
                                    M_KmMemCalcRealPoolSize(KI_DYNAMIC_NON_CACHEABLE_INT_MEM_SIZE));    \
    /* Check that the memory pool was successfully allocated */                                     \
    DevCheck (eRRORcODE == KM_ERR_OK, eRRORcODE, KI_DYNAMIC_NON_CACHEABLE_INT_MEM_SIZE, 0);             \
}
#else
#define M_KiOsCreateDynamicMemPool()                                                  \
{                                                                                     \
    /* Create the memory pool */                                                      \
    KmMemoryCreatePool (kiDynamicMemoryPool,                                          \
                                    M_KmMemCalcRealPoolSize(KI_DYNAMIC_MEM_SIZE));    \
    KmMemoryCreatePool (kiDynamicNonCacheableMemoryPool,                                            \
                                    M_KmMemCalcRealPoolSize(KI_DYNAMIC_NON_CACHEABLE_MEM_SIZE));    \
    KmMemoryCreatePool (kiDynamicNonCacheableIntMemoryPool,                                            \
                                    M_KmMemCalcRealPoolSize(KI_DYNAMIC_NON_CACHEABLE_INT_MEM_SIZE));    \
}
#endif

/** Attempt to allocate from the dynamic heap memory pool.
 * This macro will try to allocate a block from the dynamic heap memory pool,
 * storing a pointer to the allocated block in bLOCKpTR. If the allocation
 * fails the bLOCKpTR is set to PNULL. This is the implementation
 * used when #KI_DISABLE_KM_DYNAMIC_MEMORY is not defined.
 *
 * \param[in] nUMBERoFbYTES Number of bytes to be allocated.
 * \param[out] bLOCKpTR A pointer that will be set to the allocated memory
 *                 (or PNULL if allocation failed). */
#if defined (KI_CHECK_ASSERTIONS)
#define M_KiOsGetDynamicMemBlockPoll(nUMBERoFbYTES, bLOCKpTR)                    \
{                                                                                \
    KmErrorCode  eRRORcODE;                                                      \
    /* Attempt to allocate a memory block */                                     \
    eRRORcODE = KmMemoryGet (kiDynamicMemoryPool, (nUMBERoFbYTES), (void **) &(bLOCKpTR)); \
    /* Check that the return code does not indicate a serious error */           \
    DevCheck ((eRRORcODE == KM_ERR_OK) || (eRRORcODE == KM_ERR_ALLOCATION_FAILED), \
              eRRORcODE, (nUMBERoFbYTES), KiThisTask ());                        \
}
#else
#define M_KiOsGetDynamicMemBlockPoll(nUMBERoFbYTES, bLOCKpTR)                    \
{                                                                                \
    /* Attempt to allocate a memory block */                                     \
    KmMemoryGet (kiDynamicMemoryPool, (nUMBERoFbYTES), (void **) &(bLOCKpTR)); \
}
#endif

/** Allocate from the dynamic heap memory pool.
 * This macro will allocate a block from the indicated pool, storing a
 * pointer to the allocated block in bLOCKpTR. If the allocation fails and
 * assert checking is enabled the macro will assert. Otherwise if assert
 * checking is not enabled the calling task will be blocked until memory
 * becomes available. This is a recovery mode in release code and should
 * not be relied upon for normal operation. This is the implementation
 * used when #KI_DISABLE_KM_DYNAMIC_MEMORY is not defined.
 *
 * \param[in] nUMBERoFbYTES Number of bytes to be allocated.
 * \param[out] bLOCKpTR A pointer that will be set to the allocated
 *                 memory. */
#if defined (KI_CHECK_ASSERTIONS)
#define M_KiOsGetDynamicMemBlock(nUMBERoFbYTES, bLOCKpTR)         \
{                                                                 \
    KmErrorCode  eRRORcODE;                                       \
    /* Attempt to allocate a memory block */                      \
    eRRORcODE = KmMemoryGet (kiDynamicMemoryPool, (nUMBERoFbYTES), (void **) &(bLOCKpTR)); \
    /* Check that the allocation succeeded */                     \
    if (eRRORcODE != KM_ERR_OK)                                   \
    {                                                             \
        M_KmMemoryGetFail (eRRORcODE, kiDynamicMemoryPool,        \
               (nUMBERoFbYTES), (void **) &(bLOCKpTR),            \
               MODULE_NAME, __LINE__);                            \
    }                                                             \
}
#else /* KI_CHECK_ASSERTIONS */
/* This version of M_KiOsGetDynamicMemBlock suspends the task if
 * a memory block could not be allocated */
#define M_KiOsGetDynamicMemBlock(nUMBERoFbYTES, bLOCKpTR)               \
{                                                                       \
    /* This task is not wait for memory (yet) */                        \
    Boolean wAITINGfORmEMORY = FALSE;                                   \
                                                                        \
    bLOCKpTR = PNULL;                                                   \
    while (bLOCKpTR == PNULL)                                           \
    {                                                                   \
        /* Attempt to allocate a memory block */                        \
        M_KiOsGetDynamicMemBlockPoll(nUMBERoFbYTES, bLOCKpTR);          \
        /* If the allocation failed, then suspend the task by
         * waiting on a semaphore */                                    \
        if (bLOCKpTR == PNULL)                                          \
        {                                                               \
            if (!wAITINGfORmEMORY)                                      \
            {                                                           \
                /* Now this task starts waiting for memory */           \
                KiDisableInterrupts();                                  \
                kiWaitingForMemoryTaskCount++;                          \
                KiEnableInterrupts();                                   \
                wAITINGfORmEMORY = TRUE;                                \
            }                                                           \
            M_KiOsWaitSemaphoreEx(kiSemDefTable[KI_MEM_SEMAPHORE]);     \
            /* The task will be resumed here once memory becomes
             * available... */                                          \
        }                                                               \
        else if (wAITINGfORmEMORY)                                      \
        {                                                               \
            KiDisableInterrupts();                                      \
            kiWaitingForMemoryTaskCount--;                              \
            KiEnableInterrupts();                                       \
            /* If memory becomes available but not enough for this task,
             * set a semaphore for other waiting tasks in case there is
             * enough memory to allocate for those waiting tasks */     \
            if (kiWaitingForMemoryTaskCount > 0)                        \
            {                                                           \
                M_KiOsSetSemaphoreEx(kiSemDefTable[KI_MEM_SEMAPHORE]);  \
            }                                                           \
        }                                                               \
    }                                                                   \
}
#endif /* KI_CHECK_ASSERTIONS */

/** Resize a block of memory from the dynamic heap memory pool.
 * Resizes a block of memory that was previouslly allocated by
 * #M_KiOsGetDynamicMemBlockPoll or #M_KiOsGetDynamicMemBlock.
 * This is the implementation used when
 * #KI_DISABLE_KM_DYNAMIC_MEMORY is not defined.
 *
 * \param[in] nUMBERoFbYTES The new size of the memory block.
 * \param[in,out] bLOCKpTR A pointer to the memory block to be resized.
 * \param[out] rESIZED A #Boolean that indicates whether the block was
 *                 resized (TRUE) or not (FALSE). */
#if defined (KI_CHECK_ASSERTIONS)
#define M_KiOsResizeDynamicMemBlockEx(nUMBERoFbYTES, bLOCKpTR, rESIZED)           \
{                                                                                 \
    KmErrorCode eRRORcODE;                                                        \
    /* Check that the pointer references a valid memory block */                  \
    DevCheck (bLOCKpTR != PNULL, nUMBERoFbYTES, KiThisTask(), 0);                 \
    /* Resize the block */                                                        \
    eRRORcODE = KmMemoryReSize (kiDynamicMemoryPool, (void *) bLOCKpTR, nUMBERoFbYTES); \
    /* Check if the resize succeeded */                                           \
    if (eRRORcODE == KM_ERR_OK)                                                   \
    {                                                                             \
        rESIZED = TRUE;                                                           \
    }                                                                             \
    else                                                                          \
    {   /* Resize failed */                                                       \
        rESIZED = FALSE;                                                          \
        if (eRRORcODE != KM_ERR_RESIZE_FAILED)                                    \
        {                                                                         \
            DevParam (eRRORcODE, bLOCKpTR, nUMBERoFbYTES);                        \
        }                                                                         \
    }                                                                             \
}
#else /* KI_CHECK_ASSERTIONS */
#define M_KiOsResizeDynamicMemBlockEx(nUMBERoFbYTES, bLOCKpTR, rESIZED)           \
{                                                                                 \
    KmErrorCode eRRORcODE;                                                        \
    Int32 originalBlkSize;                                                        \
                                                                                  \
    /* Extract the original block size */                                         \
    M_KiOsGetDynamicMemSize (bLOCKpTR, originalBlkSize);                          \
    /* Resize the block */                                                        \
    eRRORcODE = KmMemoryReSize (kiDynamicMemoryPool, (void *) bLOCKpTR, nUMBERoFbYTES); \
    /* Check if the resize succeeded */                                           \
    if (eRRORcODE == KM_ERR_OK)                                                   \
    {                                                                             \
        rESIZED = TRUE;                                                           \
        /* If the original size of the block is greater than the new size,
         * then check if there are any tasks waiting for memory.. If so,
         * notify them using a semaphore */                                       \
        if ((originalBlkSize > nUMBERoFbYTES) &&                                  \
            (kiWaitingForMemoryTaskCount > 0))                                    \
        {                                                                         \
            M_KiOsSetSemaphoreEx(kiSemDefTable[KI_MEM_SEMAPHORE]);                \
        }                                                                         \
    }                                                                             \
    else                                                                          \
    {   /* Resize failed */                                                       \
        rESIZED = FALSE;                                                          \
    }                                                                             \
}
#endif /* KI_CHECK_ASSERTIONS */

/** Get the size of a dynamic heap memory block.
 * This macro gets the size of a memory block pointed to by bLOCKpTR and
 * writes the size of the block to the variable sIZE. This is the
 * implementation used when #KI_DISABLE_KM_DYNAMIC_MEMORY is not defined.
 *
 * \param[in] bLOCKpTR Pointer to a memory block to return the size of.
 * \param[out] sIZE Variable to write the size to. */
#define M_KiOsGetDynamicMemSize(bLOCKpTR, sIZE)                   \
{                                                                 \
    KmErrorCode     sZeRRORcODE;                                  \
    /* Check that the pointer references a valid memory block */  \
    DevCheck (bLOCKpTR != PNULL, KiThisTask(), 0, 0);             \
    /* Get the block size */                                      \
    sZeRRORcODE = KmMemoryGetSize ((void *) bLOCKpTR, &sIZE);     \
    /* Check that KmMemoryGetSize() succeeded */                  \
    if (sZeRRORcODE != KM_ERR_OK)                                 \
    {                                                             \
        DevParam (sZeRRORcODE, bLOCKpTR, KiThisTask());           \
        sIZE = 0;                                                 \
    }                                                             \
}

/** Free a dynamic heap memory block.
 * Free the dynamic heap memory block represented by the given pointer.
 * This is the implementation used when #KI_DISABLE_KM_DYNAMIC_MEMORY
 * is not defined.
 *
 * \param[in] bLOCKpTR A pointer to a memory block to free. */
#if defined (KI_CHECK_ASSERTIONS)
#define M_KiOsFreeDynamicMemBlock(bLOCKpTR)                         \
{                                                                   \
    KmErrorCode eRRORcODE;                                          \
    /* Check that the block pointer is valid */                     \
    DevCheck (bLOCKpTR != PNULL, KiThisTask (), 0, 0);              \
    /* Free the memory block */                                     \
    eRRORcODE = KmMemoryFree (kiDynamicMemoryPool, (void *)bLOCKpTR); \
    /* Check that the free succeeded */                             \
    DevCheck (eRRORcODE == KM_ERR_OK, eRRORcODE, KiThisTask(), 0);  \
}
#else
#define M_KiOsFreeDynamicMemBlock(bLOCKpTR)                                      \
{                                                                                \
    /* Free the memory block */                                                  \
    KmMemoryFree (kiDynamicMemoryPool, (void *)bLOCKpTR);                        \
    /* If any task is waiting for memory, they are notified by this semaphore */ \
    if (kiWaitingForMemoryTaskCount > 0)                                         \
    {                                                                            \
        M_KiOsSetSemaphoreEx(kiSemDefTable[KI_MEM_SEMAPHORE]);                   \
    }                                                                            \
}
#endif /* KI_CHECK_ASSERTIONS */

/** Set the use of a dynamic heap memory block.
 * \deprecated This macro is no longer used and can be removed. */
#define M_KiOsSetDynamicMemUse(bLOCKpTR, uSAGE)

/** Attempt to allocate from the dynamic non-cacheable heap memory pool.
 * This macro will try to allocate a block from the dynamic non-cacheable heap memory pool,
 * storing a pointer to the allocated block in bLOCKpTR. If the allocation
 * fails the bLOCKpTR is set to PNULL. This is the implementation
 * used when #KI_DISABLE_KM_DYNAMIC_MEMORY is not defined.
 *
 * \param[in] nUMBERoFbYTES Number of bytes to be allocated.
 * \param[out] bLOCKpTR A pointer that will be set to the allocated memory
 *                 (or PNULL if allocation failed). */
#if defined (KI_CHECK_ASSERTIONS)
#define M_KiOsGetDynamicMemBlockPollNonCacheable(nUMBERoFbYTES, bLOCKpTR)                              \
{                                                                                                      \
    KmErrorCode  eRRORcODE;                                                                            \
    /* Attempt to allocate a memory block */                                                           \
    eRRORcODE = KmMemoryGet (kiDynamicNonCacheableMemoryPool, (nUMBERoFbYTES), (void **) &(bLOCKpTR)); \
    /* Check that the return code does not indicate a serious error */                                 \
    DevCheck ((eRRORcODE == KM_ERR_OK) || (eRRORcODE == KM_ERR_ALLOCATION_FAILED),                     \
              eRRORcODE, (nUMBERoFbYTES), KiThisTask ());                                              \
}
#else
#define M_KiOsGetDynamicMemBlockPollNonCacheable(nUMBERoFbYTES, bLOCKpTR)                              \
{                                                                                                      \
    /* Attempt to allocate a memory block */                                                           \
    KmMemoryGet (kiDynamicNonCacheableMemoryPool, (nUMBERoFbYTES), (void **) &(bLOCKpTR));             \
}
#endif

#if defined (KI_CHECK_ASSERTIONS)
#define M_KiOsGetDynamicIntMemBlockPollNonCacheable(nUMBERoFbYTES, bLOCKpTR)                              \
{                                                                                                      \
    KmErrorCode  eRRORcODE;                                                                            \
    /* Attempt to allocate a memory block */                                                           \
    eRRORcODE = KmMemoryGet (kiDynamicNonCacheableIntMemoryPool, (nUMBERoFbYTES), (void **) &(bLOCKpTR)); \
    /* Check that the return code does not indicate a serious error */                                 \
    DevCheck ((eRRORcODE == KM_ERR_OK) || (eRRORcODE == KM_ERR_ALLOCATION_FAILED),                     \
              eRRORcODE, (nUMBERoFbYTES), KiThisTask ());                                              \
}
#else
#define M_KiOsGetDynamicIntMemBlockPollNonCacheable(nUMBERoFbYTES, bLOCKpTR)                              \
{                                                                                                      \
    /* Attempt to allocate a memory block */                                                           \
    KmMemoryGet (kiDynamicNonCacheableIntMemoryPool, (nUMBERoFbYTES), (void **) &(bLOCKpTR));             \
}
#endif
/** Resize a block of memory from the dynamic non-cacheable heap memory pool.
 * Resizes a block of memory that was previouslly allocated by
 * M_KiOsGetDynamicMemBlockPollNonCacheable.
 * This is the implementation used when
 * #KI_DISABLE_KM_DYNAMIC_MEMORY is not defined.
 *
 * \param[in] nUMBERoFbYTES The new size of the memory block.
 * \param[in,out] bLOCKpTR A pointer to the memory block to be resized.
 * \param[out] rESIZED A #Boolean that indicates whether the block was
 *                 resized (TRUE) or not (FALSE). */
#if defined (KI_CHECK_ASSERTIONS)
#define M_KiOsResizeDynamicMemBlockExNonCacheable(nUMBERoFbYTES, bLOCKpTR, rESIZED)                 \
{                                                                                                   \
    KmErrorCode eRRORcODE;                                                                          \
    /* Check that the pointer references a valid memory block */                                    \
    DevCheck (bLOCKpTR != PNULL, nUMBERoFbYTES, KiThisTask(), 0);                                   \
    /* Resize the block */                                                                          \
    eRRORcODE = KmMemoryReSize (kiDynamicNonCacheableMemoryPool, (void *) bLOCKpTR, nUMBERoFbYTES); \
    /* Check if the resize succeeded */                                                             \
    if (eRRORcODE == KM_ERR_OK)                                                                     \
    {                                                                                               \
        rESIZED = TRUE;                                                                             \
    }                                                                                               \
    else                                                                                            \
    {   /* Resize failed */                                                                         \
        rESIZED = FALSE;                                                                            \
        if (eRRORcODE != KM_ERR_RESIZE_FAILED)                                                      \
        {                                                                                           \
            DevParam (eRRORcODE, bLOCKpTR, nUMBERoFbYTES);                                          \
        }                                                                                           \
    }                                                                                               \
}
#else /* KI_CHECK_ASSERTIONS */
#define M_KiOsResizeDynamicMemBlockExNonCacheable(nUMBERoFbYTES, bLOCKpTR, rESIZED)                 \
{                                                                                                   \
    KmErrorCode eRRORcODE;                                                                          \
    Int32 originalBlkSize;                                                                          \
                                                                                                    \
    /* Extract the original block size */                                                           \
    M_KiOsGetDynamicMemSize (bLOCKpTR, originalBlkSize);                                            \
    /* Resize the block */                                                                          \
    eRRORcODE = KmMemoryReSize (kiDynamicNonCacheableMemoryPool, (void *) bLOCKpTR, nUMBERoFbYTES); \
    /* Check if the resize succeeded */                                                             \
    if (eRRORcODE == KM_ERR_OK)                                                                     \
    {                                                                                               \
        rESIZED = TRUE;                                                                             \
    }                                                                                               \
    else                                                                                            \
    {   /* Resize failed */                                                                         \
        rESIZED = FALSE;                                                                            \
    }                                                                                               \
}
#endif /* KI_CHECK_ASSERTIONS */

#if defined (KI_CHECK_ASSERTIONS)
#define M_KiOsResizeDynamicIntMemBlockExNonCacheable(nUMBERoFbYTES, bLOCKpTR, rESIZED)                 \
{                                                                                                   \
    KmErrorCode eRRORcODE;                                                                          \
    /* Check that the pointer references a valid memory block */                                    \
    DevCheck (bLOCKpTR != PNULL, nUMBERoFbYTES, KiThisTask(), 0);                                   \
    /* Resize the block */                                                                          \
    eRRORcODE = KmMemoryReSize (kiDynamicNonCacheableIntMemoryPool, (void *) bLOCKpTR, nUMBERoFbYTES); \
    /* Check if the resize succeeded */                                                             \
    if (eRRORcODE == KM_ERR_OK)                                                                     \
    {                                                                                               \
        rESIZED = TRUE;                                                                             \
    }                                                                                               \
    else                                                                                            \
    {   /* Resize failed */                                                                         \
        rESIZED = FALSE;                                                                            \
        if (eRRORcODE != KM_ERR_RESIZE_FAILED)                                                      \
        {                                                                                           \
            DevParam (eRRORcODE, bLOCKpTR, nUMBERoFbYTES);                                          \
        }                                                                                           \
    }                                                                                               \
}
#else /* KI_CHECK_ASSERTIONS */
#define M_KiOsResizeDynamicIntMemBlockExNonCacheable(nUMBERoFbYTES, bLOCKpTR, rESIZED)                 \
{                                                                                                   \
    KmErrorCode eRRORcODE;                                                                          \
    Int32 originalBlkSize;                                                                          \
                                                                                                    \
    /* Extract the original block size */                                                           \
    M_KiOsGetDynamicMemSize (bLOCKpTR, originalBlkSize);                                            \
    /* Resize the block */                                                                          \
    eRRORcODE = KmMemoryReSize (kiDynamicNonCacheableIntMemoryPool, (void *) bLOCKpTR, nUMBERoFbYTES); \
    /* Check if the resize succeeded */                                                             \
    if (eRRORcODE == KM_ERR_OK)                                                                     \
    {                                                                                               \
        rESIZED = TRUE;                                                                             \
    }                                                                                               \
    else                                                                                            \
    {   /* Resize failed */                                                                         \
        rESIZED = FALSE;                                                                            \
    }                                                                                               \
}
#endif /* KI_CHECK_ASSERTIONS */

/** Free a dynamic heap non-cacheable memory block.
 * Free the dynamic heap non-cacheable memory block represented by the given pointer.
 * This is the implementation used when #KI_DISABLE_KM_DYNAMIC_MEMORY
 * is not defined.
 *
 * \param[in] bLOCKpTR A pointer to a memory block to free. */
#if defined (KI_CHECK_ASSERTIONS)
#define M_KiOsFreeDynamicMemBlockNonCacheable(bLOCKpTR)                           \
{                                                                                 \
    KmErrorCode eRRORcODE;                                                        \
    /* Check that the block pointer is valid */                                   \
    DevCheck (bLOCKpTR != PNULL, KiThisTask (), 0, 0);                            \
    /* Free the memory block */                                                   \
    eRRORcODE = KmMemoryFree (kiDynamicNonCacheableMemoryPool, (void *)bLOCKpTR); \
    /* Check that the free succeeded */                                           \
    DevCheck (eRRORcODE == KM_ERR_OK, eRRORcODE, KiThisTask(), 0);                \
}
#else
#define M_KiOsFreeDynamicMemBlockNonCacheable(bLOCKpTR)                           \
{                                                                                 \
    /* Free the memory block */                                                   \
    KmMemoryFree (kiDynamicNonCacheableMemoryPool, (void *)bLOCKpTR);             \
}
#endif /* KI_CHECK_ASSERTIONS */

#if defined (KI_CHECK_ASSERTIONS)
#define M_KiOsFreeDynamicIntMemBlockNonCacheable(bLOCKpTR)                        \
{                                                                                 \
    KmErrorCode eRRORcODE;                                                        \
    /* Check that the block pointer is valid */                                   \
    DevCheck (bLOCKpTR != PNULL, KiThisTask (), 0, 0);                            \
    /* Free the memory block */                                                   \
    eRRORcODE = KmMemoryFree (kiDynamicNonCacheableIntMemoryPool, (void *)bLOCKpTR); \
    /* Check that the free succeeded */                                           \
    DevCheck (eRRORcODE == KM_ERR_OK, eRRORcODE, KiThisTask(), 0);                \
}
#else
#define M_KiOsFreeDynamicIntMemBlockNonCacheable(bLOCKpTR)                           \
{                                                                                 \
    /* Free the memory block */                                                   \
    KmMemoryFree (kiDynamicNonCacheableIntMemoryPool, (void *)bLOCKpTR);             \
}
#endif /* KI_CHECK_ASSERTIONS */

#endif /* !KI_DISABLE_KM_DYNAMIC_MEMORY */
#endif /* !KI_DISABLE_DYNAMIC_MEMORY */

/***************************************************************************
 ***************************************************************************
 **  M_KiOs* Fixed Block Memory Related Macros
 ***************************************************************************
 **************************************************************************/

/** Allocate fixed block memory from a task context.
 * This macro will try to allocate a block from the indicated pool storing
 * a pointer to the allocated block in bLOCKpTR from a task context. When
 * asserts are enabled and the OS fails to allocate the memory it must
 * assert. If asserts are not enabled the OS should wait for the memory
 * to become available. This is a recovery mode for release code and
 * should not be relied upon for normal operation.
 *
 * \param[out] bLOCKpTR A pointer that will be set to the allocated memory.
 * \param[in] pOOLnUM The fixed block memory pool number (a value
 *                from #KiMemoryPoolId). */
#if !defined (M_KiOsGetMemBlock)
# define M_KiOsGetMemBlock(bLOCKpTR, pOOLnUM) FatalFail("M_KiOsGetMemBlock not implemented")
#endif

/** Attempt to allocate fixed block memory from a task context.
 * This macro will try to allocate a block from the indicated pool storing
 * a pointer to the allocated block in bLOCKpTR from a task context. If the
 * allocation fails the bLOCKpTR is set to PNULL.
 *
 * \param[out] bLOCKpTR A pointer that will be set to the allocated memory.
 * \param[in] pOOLnUM The fixed block memory pool number (a value
 *                from #KiMemoryPoolId). */
#if !defined (M_KiOsGetMemBlockPoll)
# define M_KiOsGetMemBlockPoll(bLOCKpTR, pOOLnUM) FatalFail("M_KiOsGetMemBlockPoll not implemented")
#endif

/** Allocate fixed block memory from an interrupt context.
 * This macro will try to allocate a block from the indicated pool storing
 * a pointer to the allocated block in bLOCKpTR from an interrupt context.
 * When asserts are enabled and the OS fails to allocate the memory it must
 * assert. If asserts are not enabled the OS must not wait for the memory
 * to become available but instead just set bLOCKpTR to NULL, this is a
 * fatal error.
 *
 * \param[out] bLOCKpTR A pointer that will be set to the allocated memory.
 * \param[in] pOOLnUM The fixed block memory pool number (a value
 *                from #KiMemoryPoolId). */
#if !defined (M_KiOsGetMemBlockInt)
# define M_KiOsGetMemBlockInt(bLOCKpTR, pOOLnUM) FatalFail("M_KiOsGetMemBlockInt not implemented")
#endif

/** Free fixed block memory from a task context.
 * Free the fixed block memory block represented by the given pointer from
 * a task context.
 *
 * \param[in] bLOCKpTR A pointer to the fixed block memory to be freed. */
#if !defined (M_KiOsFreeMemBlock)
# define M_KiOsFreeMemBlock(bLOCKpTR)  FatalFail("M_KiOsFreeMemBlock not implemented")
#endif

/** Free fixed block memory from an interrupt context.
 * Free the memory block represented by the given pointer in an interrupt.
 *
 * \param[in] bLOCKpTR A pointer to the fixed block memory to be freed. */
#if !defined (M_KiOsFreeMemBlockInt)
# define M_KiOsFreeMemBlockInt(bLOCKpTR)  FatalFail("M_KiOsFreeMemBlockInt not implemented")
#endif

/** Create a fixed block memory pool.
 * This macro creates the indicated fixed block memory pool storing any
 * required run-time information in the #kiMemoryPoolIdTable array. This
 * is called as part of #KiReset at system initialisation time.
 *
 * \param[in] pOOLnUM The index into the #kiMemoryPoolTable that holds
 *                the information on the memory pool to create (a value
 *                from the #KiMemoryPoolId enumeration). The
 *                #kiMemoryPoolTable array is filled in by the
 *                #KI_MEM_POOL_DEF macro. */
#if !defined (M_KiOsCreateMemPool)
# define M_KiOsCreateMemPool(pOOLnUM)     FatalFail("M_KiOsCreateMemPool not implemented")
#endif

/***************************************************************************
 ***************************************************************************
 **  M_KiOs* Task Related Macros
 ***************************************************************************
 **************************************************************************/

/** Get the #TaskId of the calling task.
 * Return the #TaskId of the calling task or UNKNOWN_TASK_ID if called from
 * a non-GKI task context (non GKI task or interrupt). Normally the OS will
 * store the GKI task id within a user field of the OS specific Task
 * Control Block when creating the task in #M_KiOsCreateTask.
 *
 * \param[out] tASKiD Storage for the task id of type #TaskId. */
#if !defined (M_KiOsThisTask)
# define M_KiOsThisTask(tASKiD)   FatalFail("M_KiOsThisTask not implemented")
#endif

/** Create a GKI task.
 * Create the task defined by the data stored in the tASKiNDEX member of
 * the #kiTaskInitTable array. The #kiTaskInitTable array is set up by
 * the #KI_TASK_DEF macro. This macro also calls the #M_KiOsStoreSplitTaskInfo
 * macro to setup parameters needed by the split-system mode of operation
 * (#ENABLE_SPLIT_SYSTEM). This is called as part of #KiReset at system
 * initialisation time.
 *
 * \param[in] tASKiNDEX Index in the #kiTaskInitTable array that contains
 *                the task parameters. */
#if !defined (M_KiOsCreateTask)
# define M_KiOsCreateTask(tASKiNDEX)   FatalFail("M_KiOsCreateTask not implemented")
#endif

/** Delay a GKI task.
 * This macro will suspend the indicated GKI task for a period in MS. 
 *
 * \param[in] Time in MS */
#if !defined (M_KiOsTaskDelay)
# define M_KiOsTaskDelay(TimEInMS)   FatalFail("M_KiOsTaskDelay not implemented")
#endif

/** Suspend a GKI task.
 * This macro will suspend the indicated GKI task. This functionality is only
 * used within the GKI when split system is being used (#ENABLE_SPLIT_SYSTEM).
 * This is called when the host tool indicates to the target GKI that a given
 * task is being run on the host, the target GKI then suspends that task on
 * the target to prevent any interaction. This shouldn't really be needed as
 * the GKI will already prevent any signalling going to the task, the only
 * case where it may be needed are if the task uses other GKI/OS features that
 * GKI does not redirect (e.g. semaphores). The task can be resumed by calling
 * M_KiOsResumeTask() although the GKI does not currently do this.
 *
 * \param[in] tASKiD The #TaskId of the task to suspend. */
#if !defined (M_KiOsSuspendTask)
# define M_KiOsSuspendTask(tASKiD)   FatalFail("M_KiOsSuspendTask not implemented")
#endif

/** Resume a suspended GKI task.
 * This macro will resume the indicated GKI task. This should only be called
 * when a task has previouslly been suspended by calling #M_KiOsSuspendTask.
 * This functionality is currently not used.
 *
 * \param[in] tASKiD The #TaskId of the task to resume. */
#if !defined (M_KiOsResumeTask)
# define M_KiOsResumeTask(tASKiD)    FatalFail("M_KiOsResumeTask not implemented")
#endif

/** Store split system task information.
 * This macro is used to store task information needed when the split-system
 * mode of operation is active (#ENABLE_SPLIT_SYSTEM). This macros stores the
 * OS task handle suitable for use with #M_KiOsSuspendTask() and
 * #M_KiOsResumeTask() as well as initialising the on target flag (used for
 * signal redirection).
 *
 * \param[in] tASKiNDEX Index into the #kiTaskInitTable array.
 * \param[in] oStASKiD OS specific task id (handle), needed if the task
 *                has to be suspended. */
#if !defined (M_KiOsStoreSplitTaskInfo)
# define M_KiOsStoreSplitTaskInfo(tASKiNDEX, oStASKiD)   FatalFail("M_KiOsStoreSplitTaskInfo not implemented")
#endif

/***************************************************************************
 ***************************************************************************
 ** M_KiOs* Semaphore Related Macros
 ***************************************************************************
 **************************************************************************/

/** Create a semaphore.
 * This macro is used to create a counting semaphore and store the handle
 * to it in the provided location. This is used to create user defined
 * sempahores and any semaphores that the GKI uses internally (e.g. for
 * the #KI_ENABLE_OWN_QUEUEING functionality). This is called as part of
 * #KiReset at system initialisation time.
 *
 * \param[in,out] A #KiSemDef structure that provides the semaphore name and
*                   storage for the OS specific handle to the semaphore. */
#if !defined (M_KiOsCreateSemaphoreEx)
# define M_KiOsCreateSemaphoreEx(sEMAdEF)   FatalFail("M_KiOsCreateSemaphoreEx not implemented")
#endif

/** Increment semaphore count from a task context.
 * This macro is used to increment the count for the associated semaphore
 * from a task context. If a higher priority task is waiting on this
 * semaphore a task switch will occur immediately.
 *
 * \param[in] sEMAdEF A #KiSemDef structure for the associated semaphore. */
#if !defined (M_KiOsSetSemaphoreEx)
# define M_KiOsSetSemaphoreEx(sEMAdEF)   FatalFail("M_KiOsSetSemaphoreEx not implemented")
#endif

/** Increment semaphore count from an interrupt context.
 * This macro is used to increment the count for the associated semaphore from
 * an interrupt context. A task switch may occur after exiting all nested
 * interrupts if a task is waiting on this semaphore and is of a higher
 * priority than the interrupted task.
 *
 * \param[in] sEMAdEF A #KiSemDef structure for the associated semaphore. */
#if !defined (M_KiOsSetSemaphoreIntEx)
# define M_KiOsSetSemaphoreIntEx(sEMAdEF)   FatalFail("M_KiOsSetSemaphoreIntEx not implemented")
#endif

/** Wait on a semaphore.
 * This macro is used to wait for the associated semaphore to become non-zero.
 * This can only be called from a task context.
 *
 * \param[in] sEMAdEF A #KiSemDef structure for the associated semaphore. */
#if !defined (M_KiOsWaitSemaphoreEx)
# define M_KiOsWaitSemaphoreEx(sEMAdEF)   FatalFail("M_KiOsWaitSemaphoreEx not implemented")
#endif

/** Poll a semaphore.
 * This macro is used to check whether the given semphore has a non-zero count.
 * If the count is non-zero the count will be decremented by one and the
 * sUCCESS parameter will be set to TRUE. If the count is zero the semaphore
 * is unaffected and the sUCCESS parameter is set to FALSE. No task rescheduling
 * or blocking should occur as a result of calling this macro.
 *
 * \param[in] sEMAdEF A #KiSemDef structure for the associated semaphore.
 * \param[out] sUCCESS This parameter indicates whether or not the semaphore
 *                 was non-zero, TRUE indicates it was not zero, FALSE indicates
 *                 it was zero. */
#if !defined (M_KiOsPollSemaphoreEx)
# define M_KiOsPollSemaphoreEx(sEMAdEF, sUCCESS)   FatalFail("M_KiOsPollSemaphoreEx not implemented")
#endif

/***************************************************************************
 ***************************************************************************
 ** M_KiOs* Event Related Macros
 ***************************************************************************
 **************************************************************************/

/** Create an event group.
 * Create an Event group and store it's handle in the given index in the
 * #kiEventGroupIdTable array. This is called as part of #KiReset at system
 * initialisation time. Events are not accessable via the GKI user API
 *
 * \deprecated Events should not be used by any user code as they are not
 *     portable.
 *
 * \param[in] eVENTgROUP The index into the #kiEventGroupIdTable array for
 *                storing the event group handle. */
#if !defined (M_KiOsCreateEventGroup)
# define M_KiOsCreateEventGroup(eVENTgROUP)   FatalFail("M_KiOsCreateEventGroup not implemented")
#endif

/** Wait on an event group.
 * Wait on any event being set in the given event group.
 *
 * \deprecated Events should not be used by any user code as they are not
 *     portable.
 *
 * \param[in] eVENTgROUP The index into the #kiEventGroupIdTable array for
 *                the event group handle.
 * \param[out] eVENTS The variable in which to store the set events bitmap. */
#if !defined (M_KiOsWaitEventGroup)
# define M_KiOsWaitEventGroup(eVENTgROUP, eVENTS)   FatalFail("M_KiOsWaitEventGroup not implemented")
#endif

/** Set events in an event group.
 * Set the given events in the given event group.
 *
 * \deprecated Events should not be used by any user code as they are not
 *     portable.
 *
 * \param[in] eVENTgROUP The index into the #kiEventGroupIdTable array for
 *                the event group handle.
 * \param[in] eVENTS A bitmap of the events to set in the event group. */
#if !defined (M_KiOsSetEvent)
# define M_KiOsSetEvent(eVENTgROUP, eVENTS)   FatalFail("M_KiOsSetEvent not implemented")
#endif

/***************************************************************************
 * Typedefs
 **************************************************************************/

/***************************************************************************
 *  Global variables
 **************************************************************************/

/** @} */ /* End of GkiOsAbs group */
/** @} */ /* End of PrdGki group */

extern Int32 kiDynamicMemoryPool[];
extern Int32 kiDynamicNonCacheableMemoryPool[];
extern Int32 kiDynamicNonCacheableIntMemoryPool[];

#if !defined (ON_PC) && !defined (ON_TARGET_UNIT_TEST)
extern KiOsEventHandle       kiEventGroupIdTable[KI_NUM_EVENT_GROUPS];
#endif

#endif

/* END OF FILE */
