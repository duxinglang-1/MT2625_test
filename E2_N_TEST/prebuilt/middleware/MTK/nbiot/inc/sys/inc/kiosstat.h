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
 * Internal GKI header for low level statistics code.
 **************************************************************************/

#ifndef KIOSSTAT_H
#define KIOSSTAT_H

/** \addtogroup PrdGki
 * @{
 */

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#include <system.h>
#include <pssignal.h>
#include <gkitask.h>
#include <kitqid.h>
#include <ki_typ.h>
#include <ki_sig.h>
#include <kisystyp.h>
#include <kicfgos.h>
#include <kiostask.h>

/***************************************************************************
 * Config
 **************************************************************************/

#if defined (KI_MEM_POOL_STATISTICS)
# if !defined (KI_STATISTICS)
#  error Need KI STATISTICS enabled to use KI MEM POOL STATISTICS
# endif
#endif

/****************************************************************************
 * Defines
 ****************************************************************************/

/** \def KI_UNUSED_STACK_MARKER
 * Unused stack marker word.
 * This is the 16 bit value to fill the task stacks with when the GKI is
 * initialising. This allows detection of used stack at a later date. */

#if defined (THREADX_KERNEL)
/* ThreadX requires it to be 0xefef for its internal stack checking. */
#define KI_UNUSED_STACK_MARKER   0xEFEF
#endif

#if defined (NUCLEUS_KERNEL)
/* Nucleus requires it to be 0xa5a5 for its internal stack checking. */
#define KI_UNUSED_STACK_MARKER   0xA5A5
#endif

#if defined (FJORD_KERNEL)
/* Fjord requires it to be 0xfefe for its internal stack checking. */
#define KI_UNUSED_STACK_MARKER   0xFEFE
#endif

#if !defined (KI_UNUSED_STACK_MARKER)
#define KI_UNUSED_STACK_MARKER   0xFACE
#endif

/***************************************************************************
 * Typedefs
 **************************************************************************/
#if defined(MIPS_LOADING_INFO)
typedef struct {
        TaskId TaskName;
        Int64  Time;
} MipsTaskStat;
#endif /* MIPS_LOADING_INFO */
/****************************************************************************
 * Macros
 ****************************************************************************/

#if defined(KI_MEMORY_USAGE)
/** Store dynamic heap memory usage for a task.
 * This macro is used to update a tasks dynamic memory usage entry in the
 * #KiDynamicMemUsageCnf signal. This macro may be called several times for
 * each task, once for every dynamic memory block currently allocated by the
 * associated task. This macro is only needed if #KI_MEMORY_USAGE is defined.
 *
 * \param[out] kIdYNmEMuSAGEcNFpTR Pointer to #KiDynamicMemUsageCnf signal
 *                 structure to have a tasks memory usage modified.
 * \param[in] tASKiD GKI #TaskId of the task whose memory usage
*                  entry is being updated.
 * \param[in] mEMuSAGE Amount of memory usage to add to this tasks
*                  dynamic memory usage entry. */
#define M_KiStatWriteDynMemTaskUsageEntry(kIdYNmEMuSAGEcNFpTR, tASKiD, mEMuSAGE)  \
        {                                                                       \
            Int16 initTaskIndex;                                                \
                                                                                \
            if ((tASKiD) == UNKNOWN_TASK_ID) /* Interrupt */                    \
                initTaskIndex = (TaskId)(KI_NUM_OF_REAL_TASKS + 1);             \
            else if ((tASKiD) >= KI_MAX_TASK_ID) /* memory pool error */        \
                initTaskIndex = (TaskId)(KI_NUM_OF_REAL_TASKS + 2);             \
            else                                                                \
                initTaskIndex = M_KiTaskIdToInitIndex(tASKiD);                  \
                                                                                \
            /* task memory usage */                                             \
            (kIdYNmEMuSAGEcNFpTR)->taskDynamicMemoryId[initTaskIndex]         = (tASKiD); \
            (kIdYNmEMuSAGEcNFpTR)->taskDynamicMemoryAllocated[initTaskIndex] += mEMuSAGE; \
            (kIdYNmEMuSAGEcNFpTR)->taskDynamicMemoryBlockCount[initTaskIndex]++; \
        }

/** Store dynamic heap memory caller address for a task.
 * This macro is used to update a caller address entry in a
 * #KiDynamicMemUsageCnf signal. A caller address entry is only updated if the
 * requested task equals the id of the task that allocated the current memory
 * block or if the requested task is UNKNOWN_TASK_ID (i.e. all tasks). This
 * macro is only needed if #KI_MEMORY_USAGE is defined.
 *
 * \param[out] kIdYNmEMuSAGEcNFpTR Pointer to #KiDynamicMemUsageCnf signal
 *                structure to have a caller address entry modified.
 * \param[in] rEQtASKiD GKI #TaskId of the task for which a memory
 *                report has been requested.
 * \param[in] tHIStASKiD GKI #TaskId of the task that allocated the
 *                current memory block.
 * \param[in] mEMuSAGE Amount of memory usage to add to this tasks dynamic
 *                memory usage entry.
 * \param[in] cALLaDDR The address of the function that called the memory
 *                allocation routine. */
#define M_KiStatWriteDynMemTaskCallerEntry(kIdYNmEMuSAGEcNFpTR, rEQtASKiD, tHIStASKiD, mEMuSAGE, cALLaDDR)  \
        {                                                                          \
            if (((tHIStASKiD) == (rEQtASKiD)) || ((rEQtASKiD) == UNKNOWN_TASK_ID)) \
            {                                                                      \
                Int16 counter;                                                     \
                for (counter = 0; counter < KI_REPORT_CALLER_ADDRESSES; counter++) \
                {                                                                  \
                    /* find if the caller address is alread recorded               \
                     * or an empty caller address */                               \
                    if (((kIdYNmEMuSAGEcNFpTR)->callerAddresses[counter] == (cALLaDDR)) || \
                        ((kIdYNmEMuSAGEcNFpTR)->callerAddresses[counter] == 0))    \
                        break;                                                     \
                }                                                                  \
                /* if there is enough report entries */                            \
                if (counter < KI_REPORT_CALLER_ADDRESSES)                          \
                {                                                                  \
                    /* Check for new entry */                                      \
                    if ((kIdYNmEMuSAGEcNFpTR)->callerAddresses[counter] == 0)      \
                        (kIdYNmEMuSAGEcNFpTR)->callerAddressesNumEntries = counter + 1; \
                    /* Update entry */                                             \
                    (kIdYNmEMuSAGEcNFpTR)->callerAddresses[counter] = (cALLaDDR);  \
                    (kIdYNmEMuSAGEcNFpTR)->callerDynamicMemoryBlockCount[counter]++; \
                    (kIdYNmEMuSAGEcNFpTR)->callerDynamicMemoryAllocated[counter] += (mEMuSAGE); \
                }                                                                  \
            }                                                                      \
        }
#endif /* KI_MEMORY_USAGE */

/****************************************************************************
 *
 ****************************************************************************/

#if defined (KI_STATISTICS)
# if !defined (KI_DISABLE_DYNAMIC_MEMORY)

/** Dynamic heap memory allocation statistics collection.
 * Updates the statistics data on a dynamic heap memory allocation.
 *
 * \param[in] sIZE Size of the memory allocated. */
#  define M_KiAccumulateStatsDynamicGet(sIZE)                               \
         {                                                                  \
             KiDisableInterrupts();                                         \
             kiDynamicMemoryAllocated += sIZE;                              \
             kiDynamicMemoryBlockCount++;                                   \
             if (kiDynamicMemoryAllocated > kiPeakDynamicMemoryAllocated)   \
                 kiPeakDynamicMemoryAllocated = kiDynamicMemoryAllocated;   \
             if (kiDynamicMemoryBlockCount > kiPeakDynamicMemoryBlockCount) \
                 kiPeakDynamicMemoryBlockCount = kiDynamicMemoryBlockCount; \
             KiEnableInterrupts();                                          \
         }

/** Dynamic heap memory free statistics collection.
 * Updates the statistics data on a dynamic heap memory free.
 *
 * \param[in] sIZE Size of the memory freed. */
#  define M_KiAccumulateStatsDynamicRelEx(sIZE)        \
         {                                             \
             KiDisableInterrupts();                    \
             kiDynamicMemoryAllocated -= sIZE;         \
             kiDynamicMemoryBlockCount--;              \
             KiEnableInterrupts();                     \
         }

/** Dynamic non-cacheable heap memory allocation statistics collection.
 * Updates the statistics data on a dynamic non-cacheable heap memory allocation.
 *
 * \param[in] sIZE Size of the memory allocated. */
#  define M_KiAccumulateStatsDynamicNonCacheableGet(sIZE)                                           \
         {                                                                                          \
             KiDisableInterrupts();                                                                 \
             kiDynamicNonCacheableMemoryAllocated += sIZE;                                          \
             kiDynamicNonCacheableMemoryBlockCount++;                                               \
             if (kiDynamicNonCacheableMemoryAllocated > kiPeakDynamicNonCacheableMemoryAllocated)   \
                 kiPeakDynamicNonCacheableMemoryAllocated = kiDynamicNonCacheableMemoryAllocated;   \
             if (kiDynamicNonCacheableMemoryBlockCount > kiPeakDynamicNonCacheableMemoryBlockCount) \
                 kiPeakDynamicNonCacheableMemoryBlockCount = kiDynamicNonCacheableMemoryBlockCount; \
             KiEnableInterrupts();                                                                  \
         }

/** Dynamic non-cacheable heap memory free statistics collection.
 * Updates the statistics data on a dynamic non-cacheable heap memory free.
 *
 * \param[in] sIZE Size of the memory freed. */
#  define M_KiAccumulateStatsDynamicNonCacheableRelEx(sIZE)        \
         {                                                         \
             KiDisableInterrupts();                                \
             kiDynamicNonCacheableMemoryAllocated -= sIZE;         \
             kiDynamicNonCacheableMemoryBlockCount--;              \
             KiEnableInterrupts();                                 \
         }

# endif /* !KI_DISABLE_DYNAMIC_MEMORY */

# if !defined (KI_DISABLE_FIXED_BLOCK_MEMORY)

/** Fixed block memory allocation statistics collection.
 * Updates the statistics data on a fixed block memory allocation.
 * It is called just once during initialisation for the statistics
 * do not change.
 *
 * \param[in] pOOLiD Id of the pool that memory was allocated from.
 * \param[in] nUMbLOCKS of the pool that memory was allocated from.
 * \param[in] bLOCKsIZE of the pool that memory was allocated from. */
#  define M_KiAccumulateFixStats(pOOLiD, nUMbLOCKS, bLOCKsIZE)                                      \
          {                                                                                         \
             KiDisableInterrupts();                                                                 \
             kiNumBlocks[pOOLiD] = nUMbLOCKS;                                                       \
             kiBlockSize[pOOLiD] = bLOCKsIZE;                                                       \
             KiEnableInterrupts();                                                                  \
          }

/** Fixed block memory allocation statistics collection.
 * Updates the statistics data on a fixed block memory allocation.
 *
 * \param[in] pOOLiD Id of the pool that memory was allocated from. */
#  define M_KiAccumulateStatsGet(pOOLiD)                                    \
         {                                                                  \
             KiDisableInterrupts();                                         \
             kiMemoryAllocated[pOOLiD]++;                                   \
             if (kiMemoryAllocated[pOOLiD] > kiPeakMemoryAllocated[pOOLiD]) \
                 kiPeakMemoryAllocated[pOOLiD] = kiMemoryAllocated[pOOLiD]; \
             KiEnableInterrupts();                                          \
         }

/** Fixed block memory free statistics collection.
 * Updates the statistics data on a fixed block memory free.
 *
 * \param[in] pOOLiD Id of the pool that memory was freed to. */
#  define M_KiAccumulateStatsRelEx(pOOLiD)\
         {                                \
             KiDisableInterrupts();       \
             kiMemoryAllocated[pOOLiD]--; \
             KiEnableInterrupts();        \
         }
# endif /* !KI_DISABLE_FIXED_BLOCK_MEMORY */

/** Timer start statistics. */
# define M_KiAccumulateStatsTimerStart()                    \
         {                                                  \
            kiNumberOfTimers++;                             \
            if (kiNumberOfTimers > kiMaximumNumberOfTimers) \
                kiMaximumNumberOfTimers = kiNumberOfTimers; \
         }

/** Timer end statistics. */
# define M_KiAccumulateStatsTimerEnd()       kiNumberOfTimers--

# if defined (KI_MEM_POOL_STATISTICS) && !defined (KI_DISABLE_FIXED_BLOCK_MEMORY)
/***************************************************************************
 * Macro       : M_KiAccumulatePoolStatsAllocSig
 * Group       : GKI, Internals
 * Parameter   : gENERICsTRUCTpTR :
 *                  Pointer to the signal
 *
 *               lENGTHrEQ :
 *                  Requested memory size
 *
 *               sIGNALiD :
 *                  The ID of the signal that will use the memory block
 *
 * Description : Updates the accumulated statistics for memory allocated for
 *               signalling via the fixed memory pools
 ***************************************************************************/
#  define M_KiAccumulatePoolStatsAllocSig(gENERICsTRUCTpTR, lENGTHrEQ, sIGNALiD) \
  {                                                     \
     (gENERICsTRUCTpTR)->header.length = lENGTHrEQ;     \
     KiAccumulatePoolStatsAlloc(lENGTHrEQ, sIGNALiD);   \
  }

/***************************************************************************
 * Macro       : M_KiAccumulatePoolStatsFreeSig
 * Group       : GKI, Internals
 * Parameter   : gENERICsTRUCTpTR :
 *                  Pointer to the signal that will be freed
 *
 * Description : Updates the accumulated statistics for memory freed for
 *               signalling via the fixed memory pools
 ***************************************************************************/
#  define M_KiAccumulatePoolStatsFreeSig(gENERICsTRUCTpTR)           \
  {                                                                 \
     SignalId  lOCALsIGNALiD = NON_SIGNAL;                          \
     if ((gENERICsTRUCTpTR)->header.format == KI_SIGNAL_FORMAT)     \
         lOCALsIGNALiD = (gENERICsTRUCTpTR)->signal.record.id;      \
     KiAccumulatePoolStatsFree((gENERICsTRUCTpTR)->header.length, lOCALsIGNALiD); \
  }

/***************************************************************************
 * Macro       : M_KiAccumulatePoolStatsAllocMem
 * Group       : GKI, Internals
 * Parameter   : mEMbLOCKpTR :
 *                  Pointer to the memory allocated
 *
 *               lENGTHrEQ :
 *                  The size of the requested memory
 *
 * Description : Updates the accumulated statistics for memory allocated for
 *               data via the fixed memory pools
 ***************************************************************************/
#  define M_KiAccumulatePoolStatsAllocMem(mEMbLOCKpTR, lENGTHrEQ)  \
  {                                                \
     (mEMbLOCKpTR)->header.length = lENGTHrEQ;     \
     KiAccumulatePoolStatsAlloc(lENGTHrEQ, NON_SIGNAL);   \
  }

/***************************************************************************
 * Macro       : M_KiAccumulatePoolStatsFreeMem
 * Group       : GKI, Internals
 * Parameter   : mEMbLOCKpTR :
 *                  Pointer to the memory that will be freed
 * Description : Updates the accumulated statistics for memory freed for
 *               data via the fixed memory pools
 ***************************************************************************/
#  define M_KiAccumulatePoolStatsFreeMem(mEMbLOCKpTR)         \
  {                                                          \
     KiAccumulatePoolStatsFree((mEMbLOCKpTR)->header.length, NON_SIGNAL);  \
  }

# endif /* KI_MEM_POOL_STATISTICS && !KI_DISABLE_FIXED_BLOCK_MEMORY */
#endif  /* KI_STATISTICS */


/* Define default macro definitions */
#if !defined (M_KiAccumulateStatsGet)
# define M_KiAccumulateStatsGet(pOOLiD)
#endif

#if !defined (M_KiAccumulateStatsRelEx)
# define M_KiAccumulateStatsRelEx(pOOLiD)
#endif

#if !defined (M_KiAccumulateStatsTimerStart)
# define M_KiAccumulateStatsTimerStart()
#endif

#if !defined (M_KiAccumulateStatsTimerEnd)
# define M_KiAccumulateStatsTimerEnd()
#endif

#if !defined (M_KiAccumulateStatsDynamicGet)
# define M_KiAccumulateStatsDynamicGet(sIZE)
#endif

#if !defined (M_KiAccumulateStatsDynamicRelEx)
# define M_KiAccumulateStatsDynamicRelEx(sIZE)
#endif

#if !defined (M_KiAccumulatePoolStatsAllocSig)
# define M_KiAccumulatePoolStatsAllocSig(gENERICsTRUCTpTR, lENGTHrEQ, sIGNALiD)
#endif

#if !defined (M_KiAccumulatePoolStatsFreeSig)
# define M_KiAccumulatePoolStatsFreeSig(gENERICsTRUCTpTR)
#endif

#if !defined (M_KiAccumulatePoolStatsAllocMem)
# define M_KiAccumulatePoolStatsAllocMem(mEMbLOCKpTR, lENGTHrEQ)
#endif

#if !defined (M_KiAccumulatePoolStatsFreeMem)
# define M_KiAccumulatePoolStatsFreeMem(mEMbLOCKpTR)
#endif

#if !defined (M_KiAccumulateFixStats)
# define M_KiAccumulateFixStats(pOOLiD, nUMbLOCKS, bLOCKsIZE)
#endif

#if !defined (M_KiAccumulateStatsDynamicNonCacheableGet)
# define M_KiAccumulateStatsDynamicNonCacheableGet(sIZE)
#endif

#if !defined (M_KiAccumulateStatsDynamicNonCacheableRelEx)
# define M_KiAccumulateStatsDynamicNonCacheableRelEx(sIZE)
#endif

/** Redundant macro.
 * This is an old redundant macro for a previous version of the stats interface. */
#define M_KiAccumulateStatsRel(pOOLiD)

/** Redundant macro.
 * This is an old redundant macro for a previous version of the stats interface. */
#define M_KiAccumulateStatsDynamicRel(bLOCKpTR)

/** @} */ /* End of PrdGki group */

/****************************************************************************
 * Global data
 ****************************************************************************/

#if defined (KI_STATISTICS)
extern Int16 kiMaximumNumberOfTimers;
extern Int16 kiNumberOfTimers;

# if !defined (KI_DISABLE_FIXED_BLOCK_MEMORY)
extern Int16 kiPeakMemoryAllocated[KI_POOL_NUM_USER];
extern Int16 kiMemoryAllocated[KI_POOL_NUM_USER];
extern Int16 kiNumBlocks[KI_POOL_NUM_USER];
extern Int16 kiBlockSize[KI_POOL_NUM_USER];
# endif

# if !defined (KI_DISABLE_DYNAMIC_MEMORY)
extern Int32  kiPeakDynamicMemoryAllocated;
extern Int32  kiDynamicMemoryAllocated;
extern Int16  kiPeakDynamicMemoryBlockCount;
extern Int16  kiDynamicMemoryBlockCount;

extern Int32  kiPeakDynamicNonCacheableMemoryAllocated;
extern Int32  kiDynamicNonCacheableMemoryAllocated;
extern Int16  kiPeakDynamicNonCacheableMemoryBlockCount;
extern Int16  kiDynamicNonCacheableMemoryBlockCount;
# endif
#endif

/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/

Boolean KiPoolBlockInUse( Int8 *gkiBlockPtr );

#if defined (KI_STATISTICS)

Int16 KiCalcOneFreeStackSpace        (Int16              *stackBasePointer,
                                      Int32               stackSize,
                                      Boolean             isTask);

# if defined (KI_MEM_POOL_STATISTICS) && !defined (KI_DISABLE_FIXED_BLOCK_MEMORY)
void KiAccumulatePoolStatsAlloc (Int16 lengthRequested, SignalId sigType);
void KiAccumulatePoolStatsFree  (Int16 lengthRequested, SignalId sigType);
# endif /* KI_MEM_POOL_STATISTICS && !KI_DISABLE_FIXED_BLOCK_MEMORY */

#endif /* KI_STATISTICS */

#if !defined (KI_DISABLE_DYNAMIC_MEMORY)
void KiSendDynamicMemUsageCnf (TaskId destTask, TaskId queryTask);
void KiReportDynamicMemUsage(KiDynamicMemUsageCnf *report, TaskId taskid, void *mempool);
void KiStatFillInDynamicMemUsageCnf (KiDynamicMemUsageCnf *report, TaskId queryTask);
void KiStatFillInDynamicNonCacheableMemUsageCnf (KiDynamicMemUsageCnf *report, TaskId queryTask);
#endif

#if defined (UPGRADE_COMET)
void KiHandleMemReadReq (KiMemReadReq *kiMemReadReq);
void KiHandleMemWriteReq (KiMemWriteReq *kiMemWriteReq);
void KiHandleMemReadMultiReq (KiMemReadMultiReq *kiMemReadMultiReq);
void KiHandleMemWriteMultiReq (KiMemWriteMultiReq *kiMemWriteMultiReq);
#endif

#endif
/* END OF FILE */
