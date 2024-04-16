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
**************************************************************************
 ** @file
 * GKI header file for the FreeRTOS implementation of the GKI OS
 * abstraction macros.
 **************************************************************************/

#ifndef KIFRTOS_H
#define KIFRTOS_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include  <system.h>
#include  <gkitask.h>
#include  <FreeRTOS.h>
#include  <task.h>
#include  <semphr.h>
#include  <event_groups.h>
#include  <portable.h>
#if (PRODUCT_VERSION == 2625)
#include  <mt2625.h>
#endif

#define dlDisableIrq() __disable_irq()
#define dlEnableIrq() __enable_irq()


/***************************************************************************
 *  Global Data
 **************************************************************************/

/* Block set defs */
typedef struct FrtBlock * pFrtBlock;

/** Memory pool object.
 * @ingroup FrtBlock
 */
typedef struct FrtBlockPool {
  uint32_t count;
  pFrtBlock freeBlockList;
  uint8_t pool_number;
  uint32_t block_size;
} FrtBlockPool, *pFrtBlockPool;

/** @internal Memory block header.
 */
typedef struct FrtBlock
{
  pFrtBlock next;
  pFrtBlockPool pool;
} FrtBlock;


extern unsigned FrtBlockCreatePool (pFrtBlockPool pool, /**< Pointer to memory pool. */
                            void * buffer, /**< Heap for the memory pool. */
                            unsigned numberOfBlocks, /**< Number of blocks. */
                            unsigned blockBytes /**< Desired block size in bytes. */
                            );
                            
extern void * FrtBlockAlloc (pFrtBlockPool pool, uint32_t address);
extern void * FrtBlockTryAlloc (pFrtBlockPool pool, uint32_t address);
extern void FrtBlockRelease (void * ptr);
extern int FrtBlockIsAllocated (const void * ptr);

/* The freeRTOS task id holder */
extern int pxCurrentTCB;
/* The GKI task id holder */
extern TaskId Modem_task_list[40];

//extern FrtScheduler gGkiFrtScheduler;
#if defined (KI_STATISTICS)
extern Int16        * kiIntStackPtr;


/***************************************************************************
 * Config
 **************************************************************************/

/*******************************************************************************
* Define      : KI_OS_TASK_STACK_END_MARKER_BYTES
* Group       : GKI, Config, FreeRTOS
* Description : This defines the number of bytes that the OS places at the
*               end of an task stack and that shouldn't be modified
*               by the GKI.
*		This depends heavily from cfg
* This is not used if kiStats is not used.
*******************************************************************************/
#define INTERRUPT_STACK_INCREASE 512

#define KI_OS_TASK_STACK_END_MARKER_BYTES     (16)

#if defined (__arm)
#define INTERRUPT_STACK_SIZE                  (4000 * 4)
#endif

#if defined (__GNUC__)
#define INTERRUPT_STACK_SIZE                  (4000 * 4)
#endif
#endif
/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/* KI_OS_MEM_BLOCK_OVERHEAD
**
** The OS overhead for each block in a memory pool.
*/
#define KI_OS_MEM_BLOCK_TAIL_SIZE   4
#define KI_OS_MEM_BLOCK_OVERHEAD    (sizeof(FrtBlock) + KI_OS_MEM_BLOCK_TAIL_SIZE)

#define KI_OS_MEM_POOL_OVERHEAD     (0)

#define KI_OS_CHECK_TAIL_VALUE      (0xEEEEEEEE)
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
#error Signals not used from FreeRTOS. Define KI_ENABLE_OWN_QUEUEING.
#endif

/***************************************************************************
 ***************************************************************************
 **  KiOs Memory Related Macros
 ***************************************************************************
 **************************************************************************/

#if defined (KI_DISABLE_FIXED_BLOCK_MEMORY) && !defined (KI_DISABLE_DYNAMIC_MEMORY)
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
#define M_KiOsGetMemBlock(bLOCKpTR, pOOLnUM)                                \
    {                                                                       \
        bLOCKpTR = pvPortMalloc(kiMemoryPoolTable[pOOLnUM].blockSize);      \
    }
#else
#define M_KiOsGetMemBlock(bLOCKpTR, pOOLnUM)                           \
    {                                                                  \
        bLOCKpTR = pvPortMalloc(kiMemoryPoolTable[pOOLnUM].blockSize);      \
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
#define M_KiOsGetMemBlockPoll(bLOCKpTR, pOOLnUM)                       \
    {                                                                  \
        bLOCKpTR = pvPortMalloc(kiMemoryPoolTable[pOOLnUM].blockSize);      \
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
#define M_KiOsGetMemBlockInt(bLOCKpTR, pOOLnUM)                        \
    {                                                                  \
        *((unsigned int*)0) = 0xDEAD; \
    }
//        M_KiSysOsIntFuncUsed();

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
        vPortFree(bLOCKpTR);                \
    }
#else
#define M_KiOsFreeMemBlock(bLOCKpTR)               \
    {                                              \
        vPortFree(bLOCKpTR);                \
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
        vPortFree(bLOCKpTR);                \
    }
#else
#define M_KiOsFreeMemBlockInt(bLOCKpTR)            \
    {                                              \
       *((unsigned int*)0) = 0xDEAD; \
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
    }

#else //if defined own block system and dynamic from KM
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
        bLOCKpTR = FrtBlockAlloc (&kiMemoryPoolIdTable[pOOLnUM], AddRess);      \
    }
#else 
#define M_KiOsGetMemBlock(bLOCKpTR, pOOLnUM)                           \
    {                                                                  \
        bLOCKpTR = FrtBlockAlloc (&kiMemoryPoolIdTable[pOOLnUM]);      \
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
        bLOCKpTR = FrtBlockTryAlloc (&kiMemoryPoolIdTable[pOOLnUM], AddRess);   \
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
        bLOCKpTR = FrtBlockTryAlloc (&kiMemoryPoolIdTable[pOOLnUM], AddRess);   \
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
        FrtBlockRelease (bLOCKpTR);                     \
            }
#else
#define M_KiOsFreeMemBlock(bLOCKpTR)               \
    {                                              \
        FrtBlockRelease (bLOCKpTR);                     \
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
        FrtBlockRelease (bLOCKpTR);                     \
        M_KiSysOsIntFuncUsed();                         \
    }
#else
#define M_KiOsFreeMemBlockInt(bLOCKpTR)            \
    {                                              \
        FrtBlockRelease (bLOCKpTR);                \
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
        FatalCheck (pOOLnUM < KI_POOL_NUM_USER, pOOLnUM, 0, 0);                 \
        FrtBlockCreatePool (&kiMemoryPoolIdTable[pOOLnUM],            \
                                      kiMemoryPoolTable[pOOLnUM].poolPtr,       \
                                      kiMemoryPoolTable[pOOLnUM].numBlocks,      \
                                      kiMemoryPoolTable[pOOLnUM].blockSize);     \
    }



#endif /* KI_DISABLE_FIXED_BLOCK_MEMORY */

/***************************************************************************
 ***************************************************************************
 **  KiOs Dynamic Memory Related Macros
 ***************************************************************************
 **************************************************************************/
#if !defined (KI_DISABLE_DYNAMIC_MEMORY)
#if defined (KI_DISABLE_KM_DYNAMIC_MEMORY)
#define M_KiOsCreateDynamicMemPool()

#define M_KiOsGetDynamicMemBlockPoll(nUMBERoFbYTES, bLOCKpTR) \
    {                                                                  \
        bLOCKpTR = pvPortMalloc(nUMBERoFbYTES);      \
    }

#define M_KiOsGetDynamicMemBlock(nUMBERoFbYTES, bLOCKpTR) \
    {                                                                  \
        bLOCKpTR = pvPortMalloc(nUMBERoFbYTES);      \
    }

#define M_KiOsResizeDynamicMemBlockEx(nUMBERoFbYTES, bLOCKpTR, rESIZED)


#define M_KiOsGetDynamicMemSize(bLOCKpTR, sIZE) \
    {                                                                  \
        bLOCKpTR = bLOCKpTR;      \
    }

#define M_KiOsFreeDynamicMemBlock(bLOCKpTR) \
    {                                                   \
        vPortFree(bLOCKpTR);                \
    }

#define M_KmMemCalcRealPoolSize(uSERsIZE)  ((uSERsIZE) + 1536)

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
** Return the (GKI) TaskId of the calling task.		
	
*/
 
/* Get the GKI task using it's priority */

#define M_KiOsThisTask(tASKiD)                       \
    {	\
		if ((portNVIC_INT_CTRL_REG & 0xFFUL ) != 0 ) \
		{ \
			tASKiD=UNKNOWN_TASK; \
		}\
		else \
		{ \
    	uint32_t pRio=uxTaskPriorityGet(NULL); \
			tASKiD = Modem_task_list[pRio]; \
			if (tASKiD == 0) \
			{ \
       tASKiD=UNKNOWN_TASK; \
			} \
		} \
  	}
/*
BaseType_t xGkiTaskCreate(
							  TaskFunction_t pvTaskCode,
							  const char * const pcName,
							  uint16_t usStackDepth,
							  void *pvParameters,
							  UBaseType_t uxPriority,
							  TaskHandle_t *pvCreatedTask
							  StackType_t * const puxStackBuffer
						  );</pre>
 *
 * Create a new task and add it to the list of tasks that are ready to run.
 *
 * xGkiTaskCreate() can only be used to create a gki task that has unrestricted
 * access to the entire microcontroller memory map.  Systems that include MPU
 * support can alternatively create an MPU constrained task using
 * xTaskCreateRestricted().
 *
 * @param pvTaskCode Pointer to the task entry function.  Tasks
 * must be implemented to never return (i.e. continuous loop).
 *
 * @param pcName A descriptive name for the task.  This is mainly used to
 * facilitate debugging.  Max length defined by configMAX_TASK_NAME_LEN - default
 * is 16.
 *
 * @param usStackDepth The size of the task stack specified as the number of
 * variables the stack can hold - not the number of bytes.  For example, if
 * the stack is 16 bits wide and usStackDepth is defined as 100, 200 bytes
 * will be allocated for stack storage.
 *
 * @param pvParameters Pointer that will be used as the parameter for the task
 * being created.
 *
 * @param uxPriority The priority at which the task should run.  Systems that
 * include MPU support can optionally create tasks in a privileged (system)
 * mode by setting bit portPRIVILEGE_BIT of the priority parameter.  For
 * example, to create a privileged task at priority 2 the uxPriority parameter
 * should be set to ( 2 | portPRIVILEGE_BIT ).
 *
 * @param pvCreatedTask Used to pass back a handle by which the created task
 * can be referenced.
 *
 *@param puxStackBuffer Used to pass a static task stack address to FreeRTOS
 * 
 * @return pdPASS if the task was successfully created and added to a ready
 * list, otherwise an error code defined in the file projdefs.h
*/
#define xGkiTaskCreate( pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask, puxStackBuffer ) xTaskGenericCreate( ( pvTaskCode ), ( pcName ), ( usStackDepth ), ( pvParameters ), ( uxPriority ), ( pxCreatedTask ), ( puxStackBuffer ), ( NULL ) )

  	
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
 
/* Stacksize ?*/
//typedef void (*FrtEntryPoint)(void *);

#define M_KiOsCreateTask(tASKiNDEX)                                                          	\
    {	\
	xGkiTaskCreate((TaskFunction_t)kiTaskInitTable[tASKiNDEX].entryPoint, \
								kiTaskNameTable[tASKiNDEX].taskName, \
								kiTaskInitTable[tASKiNDEX].stackSize / sizeof(long),  		\
                                     NULL, 						\
                                     kiTaskInitTable[tASKiNDEX].priority,                 		\
									 &kiTaskSplitTable[M_KiTaskGetIndex(kiTaskInitTable[tASKiNDEX].taskId)].osTaskId,      \
									 kiTaskInitTable[tASKiNDEX].stack); \
									 kiTaskSplitTable[M_KiTaskGetIndex(kiTaskInitTable[tASKiNDEX].taskId)].onTarget = TRUE; \
									 Modem_task_list[kiTaskInitTable[tASKiNDEX].priority]=kiTaskInitTable[tASKiNDEX].taskId; \
	}

/*
**
** TimEInMS :- How long to suspend current task.
**
** Suspend the indicated task for a period of time.
*/
#define M_KiOsTaskDelay(TimEInMS)  vTaskDelay(TimEInMS/portTICK_PERIOD_MS)
    
/*
**
** tASKiD :- The task id of the task to suspend.
**
** Suspend the indicated task.
*/
#define M_KiOsSuspendTask(tASKiD)                                              \
    { \
		int i=0;                                                                         \
		for (i=0; i<KI_NUM_OF_REAL_TASKS; i++)	\
			{ \
			if ((kiTaskInitTable[i].taskId) == tASKiD) \
			break; \
			} \
		vTaskSuspend((TaskHandle_t) (kiTaskSplitTable[M_KiTaskGetIndex(kiTaskInitTable[i].taskId)].osTaskId) ); \
    }

/* M_KiOsResumeTask (tASKiD)
**
** tASKiD :- The task id of the task to resume.
**
** Resume the indicated task.
*/
#define M_KiOsResumeTask(tASKiD)                                              \
    {                                                                         \
		int i=0;                                                                         \
		for (i=0; i<KI_NUM_OF_REAL_TASKS; i++)	\
			{ \
			if ((kiTaskInitTable[i].taskId) == tASKiD) \
			break; \
			} \
		vTaskResume((TaskHandle_t) (kiTaskSplitTable[M_KiTaskGetIndex(kiTaskInitTable[i].taskId)].osTaskId) ); \
    }


#define M_KiOsSuspendTaskAll vTaskSuspendAll
#define M_KiOsResumeTaskAll xTaskResumeAll

/* M_KiOsStoreSplitTaskInfo (tASKiNDEX, oStASKiD)
**
** tASKiNDEX :- Index into the kiTaskInitTable[]
** oStASKiD  :- OS specific task id (handle), needed if suspending the task
**
** If running split system need to fill in the kiTaskSplitTable
** with the OS task handle (allows suspend and resume) and set the
** onTarget flag to TRUE.
*/
#if !defined (ENABLE_SPLIT_SYSTEM)
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
* Macro       : KiCreateBinarySemaphore
* Group       : GKI, Config, FreeRTOS
* Parameter   : KiSemDef sEMAdEF = This parameter gives the semaphore name and
*                   storage for the OS specific handle to the semaphore.
* Returns     : Nothing
* Description : This macro is used to create a counting semaphore and store the
*                   handle to it in the provided location.
*******************************************************************************/

#define KiCreateBinarySemaphore(sEMAdEF)                                \
    {                                                                   \
	sEMAdEF = xSemaphoreCreateBinary (); \
    }



/*******************************************************************************
* Macro       : M_KiOsCreateSemaphoreEx
* Group       : GKI, Config, FreeRTOS
* Parameter   : KiSemDef sEMAdEF = This parameter gives the semaphore name and
*                   storage for the OS specific handle to the semaphore.
* Returns     : Nothing
* Description : This macro is used to create a counting semaphore and store the
*                   handle to it in the provided location.
*******************************************************************************/

#define M_KiOsCreateSemaphoreEx(sEMAdEF)                                \
    {                                                                   \
	(sEMAdEF).osHandle=xSemaphoreCreateCounting (1024, 0); \
    }

/*******************************************************************************
* Macro       : M_KiOsSetSemaphoreEx
* Group       : GKI, Config, FreeRTOS
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
        xSemaphoreGive ((sEMAdEF).osHandle); \
	}

/*******************************************************************************
* Macro       : M_KiOsSetSemaphoreIntEx
* Group       : GKI, Config, FreeRTOS
* Parameter   : KiSemDef sEMAdEF = This parameter gives the semaphore name and
*                   the OS specific handle for the required semaphore.
* Returns     : Nothing
* Description : This macro is used to increment the count for the associated
*                   semaphore from an interrupt context.
*                   A task switch may occur after exiting all nested interrupts
*                   if a task is waiting on this semaphore and is of a higher
*                   priority than the interrupted task.
*******************************************************************************/
#define M_KiOsSetSemaphoreIntEx(sEMAdEF) \
    {                                           \
        BaseType_t xHigherPriorityTaskWoken, xResult; \
        xHigherPriorityTaskWoken = pdFALSE;   \
        xResult = xSemaphoreGiveFromISR ((sEMAdEF).osHandle, &xHigherPriorityTaskWoken); \
        if(xResult != pdFAIL) { portYIELD_FROM_ISR( xHigherPriorityTaskWoken ); }  \
    }

/*******************************************************************************
* Macro       : M_KiOsWaitSemaphoreEx
* Group       : GKI, Config, FreeRTOS
* Parameter   : KiSemDef sEMAdEF = This parameter gives the semaphore name and
*                   the OS specific handle for the required semaphore.
* Returns     : Nothing
* Description : This macro is used to wait for the associated semaphore to
*                   become non-zero. This can only be called from a task context.
*******************************************************************************/
#define M_KiOsWaitSemaphoreEx(sEMAdEF)          \
    {                                           \
        xSemaphoreTake ((QueueHandle_t)(sEMAdEF).osHandle,portMAX_DELAY); \
    }

/*******************************************************************************
* Macro       : M_KiOsPollSemaphoreEx
* Group       : GKI, Config, FreeRTOS
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
//Add taskENTER_CRITICAL(); depending of the usage
/* This is FreeRTOS 9.0.0 addition. Remove if FreeRTOS is updated */
#define uxSemaphoreGetCount( xSemaphore ) uxQueueMessagesWaiting( ( QueueHandle_t ) ( xSemaphore ) )
#define uxSemaphoreGetCountFromISR( xSemaphore ) uxQueueMessagesWaitingFromISR( ( QueueHandle_t ) ( xSemaphore ) )

/* The FreeRTOS version differs slightly. Not impressed with this design. //mro*/
#define M_KiOsPollSemaphoreEx(sEMAdEF, sUCCESS)                \
    {                                                          \
		if (uxSemaphoreGetCount((sEMAdEF).osHandle)>0) { xSemaphoreTake((sEMAdEF).osHandle, 0); sUCCESS = TRUE; } \
		else sUCCESS = FALSE; \
    }
		

/*******************************************************************************
* Macro       : M_KiOsPollSemaphoreIntEx
* Group       : GKI, Config, FreeRTOS
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
#define M_KiOsPollSemaphoreIntEx(sEMAdEF, sUCCESS)                \
	{                                                          \
if (uxSemaphoreGetCountFromISR((sEMAdEF).osHandle)>0) { xSemaphoreTakeFromISR((sEMAdEF).osHandle, 0); sUCCESS = TRUE; } \
		else sUCCESS = FALSE; \
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
    kiEventGroupIdTable[eVENTgROUP] = xEventGroupCreate();  \
    }

/* M_KiOsWaitEventGroup (eVENTgROUP, eVENTS)
**
** eVENTgROUP :- The index into the kiEventGroupIdTable array for the event
**               group handle.
** eVENTS     :- The variable in which to store the set events bitmap.
**
** Wait on any event being set in the given event group.
        (eVENTS) = (Int16)frtEventWait (&kiEventGroupIdTable[eVENTgROUP]); 
*/
#if configUSE_16_BIT_TICKS == 1
#define eVENT_bITS 0x00ffU
#else
#define eVENT_bITS 0x00ffffffUL
#endif
#define M_KiOsWaitEventGroup(eVENTgROUP, eVENTS)                           \
    {		\
    (eVENTS) = (Int16)xEventGroupWaitBits(kiEventGroupIdTable[eVENTgROUP], eVENT_bITS, pdTRUE,pdFALSE,portMAX_DELAY); \
    }

/* M_KiOsSetEvent (eVENTgROUP, eVENTS)
**
** eVENTgROUP :- The index into the kiEventGroupIdTable array for the event
**               group handle.
** eVENTS     :- The events to set in the event group.
**
** Set the given events in the given event group.
        frtEventSet (&kiEventGroupIdTable[eVENTgROUP], (eVENTS)); \
        M_KiSysOsIntFuncUsed ();                                  \
*/
#define M_KiOsSetEvent(eVENTgROUP, eVENTS)                        \
    {															  \
	xEventGroupSetBits(kiEventGroupIdTable[eVENTgROUP], eVENTS);						  \
    }

/* M_KiOsSetEventInt (eVENTgROUP, eVENTS)
**
** eVENTgROUP :- The index into the kiEventGroupIdTable array for the event
**               group handle.
** eVENTS     :- The events to set in the event group.
**
** Set the given events in the given event group.
    This one can be used from ISR
        frtEventSet (&kiEventGroupIdTable[eVENTgROUP], (eVENTS)); \
        M_KiSysOsIntFuncUsed ();                                  \
*/
#define M_KiOsSetEventInt(eVENTgROUP, eVENTS)                                   \
    {															                \
    BaseType_t xHigherPriorityTaskWoken, xResult;                               \
    xHigherPriorityTaskWoken = pdFALSE;                                         \
	xResult = xEventGroupSetBitsFromISR(kiEventGroupIdTable[eVENTgROUP], eVENTS, &xHigherPriorityTaskWoken);	\
    if(xResult != pdFAIL) { portYIELD_FROM_ISR(xHigherPriorityTaskWoken); }     \
    }

/***************************************************************************
 *  Typedefs
 **************************************************************************/

typedef EventGroupHandle_t KiOsEventHandle;

typedef SemaphoreHandle_t KiOsSemaphoreHandle;

typedef TaskHandle_t KiOsTaskHandle;

typedef FrtBlockPool KiOsPoolHandle;

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
#define INLINE __inline

#endif

/* END OF FILE */
