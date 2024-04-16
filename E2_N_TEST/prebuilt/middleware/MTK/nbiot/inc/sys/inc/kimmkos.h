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
/** @file
 * GKI header file for the MMK implementation of the GKI OS
 * abstraction macros.
 **************************************************************************/

#ifndef KIMMKOS_H
#define KIMMKOS_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include  <system.h>
#include  <gkitask.h>
#include  <mmk_scheduler.h>
#include  <mmk_semaphore.h>
#include  <mmk_event.h>
#include  <mmk_block.h>

/***************************************************************************
 *  Global Data
 **************************************************************************/

extern MmkScheduler gGkiMmkScheduler;
#if defined (KI_STATISTICS)
extern Int16        * kiIntStackPtr;
#endif

/***************************************************************************
 * Config
 **************************************************************************/

/*******************************************************************************
* Define      : KI_OS_TASK_STACK_END_MARKER_BYTES
* Group       : GKI, Config, MMK
* Description : This defines the number of bytes that the OS places at the
*                   end of an task stack and that shouldn't be modified
*                   by the GKI.
*******************************************************************************/
#define KI_OS_TASK_STACK_END_MARKER_BYTES     (0)

#if defined (__arm)
/* bytes ; MMK_CONFIG_ARM_IRQ_STACK_WORDS includes the stack increase
 * for development stacks. Fix bug 0001342.
 */
#define INTERRUPT_STACK_SIZE                  (MMK_CONFIG_ARM_IRQ_STACK_WORDS * 4)
#endif

#if defined (__GNUC__)
#define INTERRUPT_STACK_SIZE                  (MMK_CONFIG_R2_IRQ_STACK_WORDS * 4)
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/* KI_OS_MEM_BLOCK_OVERHEAD
**
** The OS overhead for each block in a memory pool.
*/
#define KI_OS_MEM_BLOCK_OVERHEAD              (sizeof (MmkBlock))

/***************************************************************************
* Macros
**************************************************************************/

/* KiOsAssert
**
** Macro used to handle OS specific errors.
*/
#if defined (KI_CHECK_ASSERTIONS)
#  define KiOsAssert(cODE, pARAM1, pARAM2)     FatalCheck (cODE == CJ_EROK, cODE, pARAM1, pARAM2)
#else
# define KiOsAssert(cODE, pARAM1, pARAM2)
#endif

/***************************************************************************
 *  KiOs Macros
 **************************************************************************/

/***************************************************************************
 ***************************************************************************
 **  KiOs Signal Related Macros
 ***************************************************************************
 **************************************************************************/

#if !defined (KI_ENABLE_OWN_QUEUEING)
#define Queuing     not implemented in MMK.Define KI_ENABLE_OWN_QUEUEING.
#endif

/***************************************************************************
 ***************************************************************************
 **  KiOs Memory Related Macros
 ***************************************************************************
 **************************************************************************/

#if !defined (KI_DISABLE_FIXED_BLOCK_MEMORY)
/* M_KiOsGetMemBlock (bLOCKpTR, pOOLnUM)
**
** bLOCKpTR :- A pointer in that will be set to the allocated memory.
** pOOLnUM  :- The memory pool number.
**
** This macro will try to allocate a block from the indicated pool storing
** a pointer to the allocated block in bLOCKpTR. Depending on whether
** asserts are enabled the OS may or may not wait for the memory.
*/
#if defined (KI_CHECK_ASSERTIONS)
#define M_KiOsGetMemBlock(bLOCKpTR, pOOLnUM, AddRess)                                \
    {                                                                       \
        FatalCheck (pOOLnUM < KI_POOL_NUM_USER, pOOLnUM, 0, 0);             \
        bLOCKpTR = MmkBlockTryAlloc (&kiMemoryPoolIdTable[pOOLnUM]);        \
        FatalCheck (NULL != bLOCKpTR, bLOCKpTR, pOOLnUM, KiThisTask ());    \
        M_KiAccumulateStatsGet (pOOLnUM);                                   \
    }
#else
#define M_KiOsGetMemBlock(bLOCKpTR, pOOLnUM, AddRess)                           \
    {                                                                  \
        bLOCKpTR = MmkBlockAlloc (&kiMemoryPoolIdTable[pOOLnUM]);      \
        M_KiAccumulateStatsGet (pOOLnUM);                              \
    }
#endif

/* M_KiOsGetMemBlockPoll (bLOCKpTR, pOOLnUM)
**
** bLOCKpTR :- A pointer in that will be set to the allocated memory.
** pOOLnUM  :- The memory pool number.
**
** Thie macro will try to allocate a block from the indicated pool storing
** a pointer to the allocated block in bLOCKpTR. If the allocation fails
** the bLOCKpTR is set to PNULL.
*/
#define M_KiOsGetMemBlockPoll(bLOCKpTR, pOOLnUM, AddRess)                       \
    {                                                                  \
        FatalCheck (pOOLnUM < KI_POOL_NUM_USER, pOOLnUM, 0, 0);        \
        bLOCKpTR = MmkBlockTryAlloc (&kiMemoryPoolIdTable[pOOLnUM]);   \
        if (bLOCKpTR != NULL)                                          \
            M_KiAccumulateStatsGet (pOOLnUM);                          \
    }

/* M_KiOsGetMemBlockInt (bLOCKpTR, pOOLnUM)
**
** bLOCKpTR :- A pointer in that will be set to the allocated memory.
** pOOLnUM  :- The memory pool number.
**
** This macro will try to allocate a block from the indicated pool storing
** a pointer to the allocated block in bLOCKpTR. The OS does not wait for a
** block to become available if there is none, this is an error.
*/
#define M_KiOsGetMemBlockInt(bLOCKpTR, pOOLnUM, AddRess)                        \
    {                                                                  \
        FatalCheck (pOOLnUM < KI_POOL_NUM_USER, pOOLnUM, 0, 0);        \
        bLOCKpTR = MmkBlockTryAlloc (&kiMemoryPoolIdTable[pOOLnUM]);   \
        FatalCheck (NULL != bLOCKpTR, bLOCKpTR, pOOLnUM, 0);           \
        M_KiAccumulateStatsGet (pOOLnUM);                              \
        M_KiSysOsIntFuncUsed();                                        \
    }

/* M_KiOsFreeMemBlock (bLOCKpTR)
**
** bLOCKpTR :- A pointer to a SignalStructure, CommandStructure,
**             LoggedSignalStructure, GenericStructure or KiMemoryBlock
**             structure which represents the memory to free.
**
** Free the memory block represented by the given pointer. This should cause
** the OS to check for resheduling tasks (that are waiting for memory).
*/
#if defined (KI_CHECK_ASSERTIONS)
#define M_KiOsFreeMemBlock(bLOCKpTR)                                \
    {                                                               \
        Bool eRRORcODE;                                             \
        eRRORcODE = MmkBlockIsAllocated (bLOCKpTR);                 \
        FatalCheck(eRRORcODE, eRRORcODE, bLOCKpTR, KiThisTask());   \
        MmkBlockRelease (bLOCKpTR);                                 \
    }
#else
#define M_KiOsFreeMemBlock(bLOCKpTR)               \
    {                                              \
        MmkBlockRelease (bLOCKpTR);                \
    }
#endif

/* M_KiOsFreeMemBlockInt (bLOCKpTR)
**
** bLOCKpTR :- A pointer to a SignalStructure, CommandStructure,
**             LoggedSignalStructure, GenericStructure or KiMemoryBlock
**             structure which represents the memory to free.
**
** Free the memory block represented by the given pointer in an interrupt. This
** should not cause any rescheduling until there are no active interrupts.
*/
#if defined (KI_CHECK_ASSERTIONS)
#define M_KiOsFreeMemBlockInt(bLOCKpTR)                 \
    {                                                   \
        Bool eRRORcODE;                                 \
        eRRORcODE = MmkBlockIsAllocated (bLOCKpTR);     \
        FatalCheck(eRRORcODE, eRRORcODE, bLOCKpTR, 0);  \
        MmkBlockRelease (bLOCKpTR);                     \
        M_KiSysOsIntFuncUsed();                         \
    }
#else
#define M_KiOsFreeMemBlockInt(bLOCKpTR)            \
    {                                              \
        MmkBlockRelease (bLOCKpTR);                \
        M_KiSysOsIntFuncUsed();                    \
    }
#endif

/* M_KiOsCreateMemPool (pOOLnUM)
**
** pOOLnUM :- The index into the kiMemoryPoolTable that holds the information
**            on the memory pool to create. The kiMemoryPoolTable array is
**            filled in by the KI_MEM_POOL_DEF macro.
**
** This macro creates the indicated memory pool storing any required
** run-time information in the kiMemoryPoolIdTable array.
*/
#define M_KiOsCreateMemPool(pOOLnUM)                                            \
    {                                                                           \
        unsigned blockNb;                                                       \
        FatalCheck (pOOLnUM < KI_POOL_NUM_USER, pOOLnUM, 0, 0);                 \
        blockNb = MmkBlockCreatePool (&kiMemoryPoolIdTable[pOOLnUM],            \
                                      &gGkiMmkScheduler,                        \
                                      kiMemoryPoolTable[pOOLnUM].poolPtr,       \
                                      kiMemoryPoolTable[pOOLnUM].poolSize,      \
                                      kiMemoryPoolTable[pOOLnUM].blockSize,     \
                                      4);                                       \
        FatalCheck (blockNb == kiMemoryPoolTable[pOOLnUM].numBlocks,            \
                    blockNb, kiMemoryPoolTable[pOOLnUM].numBlocks, pOOLnUM);    \
    }
#endif /* KI_DISABLE_FIXED_BLOCK_MEMORY */

/***************************************************************************
 ***************************************************************************
 **  KiOs Dynamic Memory Related Macros
 ***************************************************************************
 **************************************************************************/
#if !defined (KI_DISABLE_DYNAMIC_MEMORY)
#if defined (KI_DISABLE_KM_DYNAMIC_MEMORY)
#error MMK does not support dynamic memory. Donot define KI_DISABLE_KM_DYNAMIC_MEMORY.
#endif /* KI_DISABLE_KM_DYNAMIC_MEMORY */
#endif /* !KI_DISABLE_DYNAMIC_MEMORY */

/***************************************************************************
 ***************************************************************************
 **  KiOs Task Related Macros
 ***************************************************************************
 **************************************************************************/

/* M_KiOsThisTask (tASKiD)
**
** tASKiD :- Storage for the task id of type TaskId.
**
** Return the TaskId of the calling task.
*/
#define M_KiOsThisTask(tASKiD)                                                                  \
    {                                                                                           \
        if (gGkiMmkScheduler.interruptNest == 0)                                                                                   \
        {                                                                                       \
            tASKiD = (TaskId)(int) MmkTaskGetUserData (MmkSchedGetCurrentTask (&gGkiMmkScheduler)); \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            tASKiD = UNKNOWN_TASK_ID;                                                           \
        }                                                                                       \
    }

/* M_KiOsCreateTask (tASKiNDEX)
**
** tASKiNDEX :- Index in the kiTaskInitTable array that contains the task
**              parameters.
**
** Create the task defined by the data stored in the tASKiNDEX member of the
** kiTaskInitTable array. The kiTaskInitTable array is set up by the
** KI_TASK_DEF macro. If Split-System is enabled setup the split system
** parameters.
*/
typedef void (*MmkEntryPoint)(void *);
#if defined(MIPS_LOADING_INFO)
#define M_KiOsCreateTask(tASKiNDEX)                                                          \
    {                                                                                        \
        pMmkTask task = MmkTaskCreate (&gGkiMmkScheduler,                                    \
                                       (MmkEntryPoint)kiTaskInitTable[tASKiNDEX].entryPoint, \
                                       NULL,                                                 \
                                       kiTaskInitTable[tASKiNDEX].stack,                     \
                                       kiTaskInitTable[tASKiNDEX].stackSize / sizeof(long),  \
                                       kiTaskInitTable[tASKiNDEX].priority);                 \
        task->totaltime    = &MipsStatResult[tASKiNDEX].Time;                         \
        MipsStatResult[tASKiNDEX].TaskName = kiTaskInitTable[tASKiNDEX].taskId;              \
        MmkTaskSetUserData (task, (void *)(kiTaskInitTable[tASKiNDEX].taskId));              \
        M_KiOsStoreSplitTaskInfo ((tASKiNDEX), task);                                        \
    }
#else /* MIPS_LOADING_INFO */
#define M_KiOsCreateTask(tASKiNDEX)                                                          \
    {                                                                                        \
        pMmkTask task = MmkTaskCreate (&gGkiMmkScheduler,                                    \
                                       (MmkEntryPoint)kiTaskInitTable[tASKiNDEX].entryPoint, \
                                       NULL,                                                 \
                                       kiTaskInitTable[tASKiNDEX].stack,                     \
                                       kiTaskInitTable[tASKiNDEX].stackSize / sizeof(long),  \
                                       kiTaskInitTable[tASKiNDEX].priority);                 \
        MmkTaskSetUserData (task, (void *)(kiTaskInitTable[tASKiNDEX].taskId));              \
        M_KiOsStoreSplitTaskInfo ((tASKiNDEX), task);                                        \
    }
#endif /* MIPS_LOADING_INFO */

/*
**
** TimEInMS :- How long to suspend current task.
**
** Suspend the indicated task for a period of time.
*/

/* NOTE: This is empty for PC UT */
#define M_KiOsTaskDelay(TimEInMS)
  
/* M_KiOsSuspendTask (tASKiD)
**
** tASKiD :- The task id of the task to suspend.
**
** Suspend the indicated task.
*/
#define M_KiOsSuspendTask(tASKiD)                                              \
    {                                                                          \
        MmkTaskSuspend (kiTaskSplitTable[M_KiTaskGetIndex (tASKiD)].osTaskId); \
    }

/* M_KiOsResumeTask (tASKiD)
**
** tASKiD :- The task id of the task to resume.
**
** Resume the indicated task.
*/
#define M_KiOsResumeTask(tASKiD)                                              \
    {                                                                         \
        MmkTaskResume (kiTaskSplitTable[M_KiTaskGetIndex (tASKiD)].osTaskId); \
    }

/* M_KiOsStoreSplitTaskInfo (tASKiNDEX, oStASKiD)
**
** tASKiNDEX :- Index into the kiTaskInitTable[]
** oStASKiD  :- OS specific task id (handle), needed if suspending the task
**
** If running split system need to fill in the kiTaskSplitTable
** with the OS task handle (allows suspend and resume) and set the
** onTarget flag to TRUE.
*/
#if defined (ENABLE_SPLIT_SYSTEM)
#define M_KiOsStoreSplitTaskInfo(tASKiNDEX, oStASKiD)                                                 \
    {                                                                                                 \
        kiTaskSplitTable[M_KiTaskGetIndex (kiTaskInitTable[tASKiNDEX].taskId)].osTaskId = (oStASKiD); \
        kiTaskSplitTable[M_KiTaskGetIndex (kiTaskInitTable[tASKiNDEX].taskId)].onTarget = TRUE;       \
    }
#else
#define M_KiOsStoreSplitTaskInfo(tASKiNDEX, oStASKiD)
#endif

/***************************************************************************
 ***************************************************************************
 **  KiOs Semaphore Related Macros
 ***************************************************************************
 **************************************************************************/

/*******************************************************************************
* Macro       : M_KiOsCreateSemaphoreEx
* Group       : GKI, Config, MMK
* Parameter   : KiSemDef sEMAdEF = This parameter gives the semaphore name and
*                   storage for the OS specific handle to the semaphore.
* Returns     : Nothing
* Description : This macro is used to create a counting semaphore and store the
*                   handle to it in the provided location.
*******************************************************************************/
#define M_KiOsCreateSemaphoreEx(sEMAdEF)                                \
    {                                                                   \
        MmkSemaphoreCreate (&(sEMAdEF).osHandle, &gGkiMmkScheduler, 0); \
    }

/*******************************************************************************
* Macro       : M_KiOsSetSemaphoreEx
* Group       : GKI, Config, MMK
* Parameter   : KiSemDef sEMAdEF = This parameter gives the semaphore name and
*                   the OS specific handle for the required semaphore.
* Returns     : Nothing
* Description : This macro is used to increment the count for the associated
*                   semaphore from a task context. If a higher priority task
*                   is waiting on this semaphore a task switch will occur
*                   immediately.
*******************************************************************************/
#define M_KiOsSetSemaphoreEx(sEMAdEF)           \
    {                                           \
        MmkSemaphoreGive (&(sEMAdEF).osHandle); \
    }

/*******************************************************************************
* Macro       : M_KiOsSetSemaphoreIntEx
* Group       : GKI, Config, MMK
* Parameter   : KiSemDef sEMAdEF = This parameter gives the semaphore name and
*                   the OS specific handle for the required semaphore.
* Returns     : Nothing
* Description : This macro is used to increment the count for the associated
*                   semaphore from an interrupt context.
*                   A task switch may occur after exiting all nested interrupts
*                   if a task is waiting on this semaphore and is of a higher
*                   priority than the interrupted task.
*******************************************************************************/
#define M_KiOsSetSemaphoreIntEx(sEMAdEF)     M_KiOsSetSemaphoreEx (sEMAdEF)

/*******************************************************************************
* Macro       : M_KiOsWaitSemaphoreEx
* Group       : GKI, Config, MMK
* Parameter   : KiSemDef sEMAdEF = This parameter gives the semaphore name and
*                   the OS specific handle for the required semaphore.
* Returns     : Nothing
* Description : This macro is used to wait for the associated semaphore to
*                   become non-zero. This can only be called from a task context.
*******************************************************************************/
#define M_KiOsWaitSemaphoreEx(sEMAdEF)          \
    {                                           \
        MmkSemaphoreTake (&(sEMAdEF).osHandle); \
    }

/*******************************************************************************
* Macro       : M_KiOsPollSemaphoreEx
* Group       : GKI, Config, MMK
* Parameter   : KiSemDef sEMAdEF = This parameter gives the semaphore name and
*                   the OS specific handle for the required semaphore.
* Parameter   : Boolean sUCCESS = This parameter indicates whether or not the
*                   semaphore was non-zero, TRUE indicates it was not zero,
*                   FALSE indicates it was zero.
* Returns     : Nothing
* Description : This macro is used to check whether the given semphore has a
*                   non-zero count. If the count is non-zero the count will be
*                   decremented by one and the sUCCESS parameter will be set to
*                   TRUE. If the count is zero the semaphore is unaffected and
*                   the sUCCESS parameter is set to FALSE. No task rescheduling
*                   or blocking should occur as a result of calling this macro.
*******************************************************************************/
#define M_KiOsPollSemaphoreEx(sEMAdEF, sUCCESS)                \
    {                                                          \
        (sUCCESS) = MmkSemaphoreTryTake (&(sEMAdEF).osHandle); \
    }

/* Unused for MMK */
#define M_KiOsPollSemaphoreIntEx(sEMAdEF, sUCCESS)                \
	{                                                          \
	}


/***************************************************************************
 ***************************************************************************
 **  KiOs Event Related Macros
 ***************************************************************************
 **************************************************************************/

/* M_KiOsCreateEventGroup (eVENTgROUP)
**
** eVENTgROUP :- The index into the kiEventGroupIdTable array for storing
**               the event group handle.
**
** Create an Event group and store it's handle in the given index in the
** kiEventGroupIdTable array.
*/
#define M_KiOsCreateEventGroup(eVENTgROUP)                                    \
    {                                                                         \
        MmkEventCreate (&kiEventGroupIdTable[eVENTgROUP], &gGkiMmkScheduler); \
    }

/* M_KiOsWaitEventGroup (eVENTgROUP, eVENTS)
**
** eVENTgROUP :- The index into the kiEventGroupIdTable array for the event
**               group handle.
** eVENTS     :- The variable in which to store the set events bitmap.
**
** Wait on any event being set in the given event group.
*/
#define M_KiOsWaitEventGroup(eVENTgROUP, eVENTS)                           \
    {                                                                      \
        (eVENTS) = (Int16)MmkEventWait (&kiEventGroupIdTable[eVENTgROUP]); \
    }

/* M_KiOsSetEvent (eVENTgROUP, eVENTS)
**
** eVENTgROUP :- The index into the kiEventGroupIdTable array for the event
**               group handle.
** eVENTS     :- The events to set in the event group.
**
** Set the given events in the given event group.
*/
#define M_KiOsSetEvent(eVENTgROUP, eVENTS)                        \
    {                                                             \
        MmkEventSet (&kiEventGroupIdTable[eVENTgROUP], (eVENTS)); \
        M_KiSysOsIntFuncUsed ();                                  \
    }

/***************************************************************************
 *  Typedefs
 **************************************************************************/

typedef MmkEvent KiOsEventHandle;

typedef MmkSemaphore KiOsSemaphoreHandle;

typedef pMmkTask KiOsTaskHandle;

typedef MmkBlockPool KiOsPoolHandle;

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

#endif

/* END OF FILE */
