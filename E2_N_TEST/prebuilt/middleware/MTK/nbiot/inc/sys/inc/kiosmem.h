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
 * GKI header for types and functions used to manage GKI memory.
 **************************************************************************/

#ifndef KIOSMEM_H
#define KIOSMEM_H

/** \addtogroup PrdGki
 * @{
 */

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <kios.h>
#include <ki_typ.h>
#include <kiosstat.h>
#include <kioslow.h>
#include <kisystyp.h>

/***************************************************************************
 * Config
 **************************************************************************/

/** \def KI_DISABLE_DYNAMIC_MEMORY
 * Disable use of dynamic heap memory.
 * This option causes all GKI memory allocations to use the fixed block
 * pools instead of the dynamic heap pool. If #KI_DISABLE_FIXED_BLOCK_MEMORY
 * is also defined an compile time error will occur.
 *
 * Dynamic heap memory is slower than fixed block memory to allocate and
 * free but has less percentage wastage (as allocations exactly match the
 * requested size). It is also susceptible to memory fragmentation. It is
 * relatively easy to configure even in a complex system. */

/** \def KI_DISABLE_FIXED_BLOCK_MEMORY
 * Disable use of fixed block memory.
 * This option causes all GKI memory allocations to use the dynamic/heap pool
 * instead of any fixed size block pools. If #KI_DISABLE_DYNAMIC_MEMORY is
 * also defined an compile time error will occur.
 *
 * Fixed block memory is fast to allocate and free but has a relatively
 * high percentage wastage since the block sizes will not always be an
 * exact fit to the requested allocation size. It can be complex to
 * configure if it is a big system with lots of different block sizes. */

#if !defined (KI_DISABLE_FIXED_BLOCK_MEMORY)
/** Fixed block memory OS overhead per block.
 * This define controls the amount of overhead to add to every block within a
 * fixed block memory mool above the user defined block length. This is OS
 * specific and can be overridden by defining this value in the OS specific
 * GKI header file. The default, if not defined in the OS specific file, is
 * 0. To add a constant overhead to the whole pool see
 * #KI_OS_MEM_POOL_OVERHEAD. The total memory pool size is given by
 * the #M_KiMemCalcPoolSize() macro. */
# if !defined (KI_OS_MEM_BLOCK_OVERHEAD)
#  define KI_OS_MEM_BLOCK_OVERHEAD   (0)
# endif

/** Fixed block memory OS overhead per pool.
 * This define controls the amount of overhead to add to every fixed block
 * memory pool above the user defined size. This is OS specific and can be
 * overridden by defining this value in the OS specific GKI header file.
 * The default, if not defined in the OS specific file, is 0. To add a
 * constant overhead to each block in a memory pool see
 * #KI_OS_MEM_BLOCK_OVERHEAD. The total memory pool size is given by
 * #M_KiMemCalcPoolSize() macro. */
# if !defined (KI_OS_MEM_POOL_OVERHEAD)
#  define KI_OS_MEM_POOL_OVERHEAD   (0)
# endif
#endif /* KI_DISABLE_FIXED_BLOCK_MEMORY */

/** \def KI_ALL_MEMORY_SCHEMES
 * All memory schemes are active.
 * This define is automatically enabled when all memory allocation schemes are
 * active in the build. This must never be enabled manually. */
#if !defined (KI_DISABLE_DYNAMIC_MEMORY) && !defined (KI_DISABLE_FIXED_BLOCK_MEMORY)
# define KI_ALL_MEMORY_SCHEMES
#endif

/* Check there is a memory allocation scheme defined. */
#if defined (KI_DISABLE_DYNAMIC_MEMORY) && defined (KI_DISABLE_FIXED_BLOCK_MEMORY)
# error No memory allocation scheme defined.
#endif

/** \def KI_MEMORY_TIMESTAMP
 * Timestamp memory allocations.
 * This option causes GKI memory allocations to be timestamped if the
 * #KI_MEMORY_USAGE option is also on. The timestamp is currently only dumped
 * out in a RAM log. */

/***************************************************************************
 * Macros
 **************************************************************************/

/** Length of header for the #KiMemoryBlock structure.
 * This provides the number of bytes between the start of the #KiMemoryBlock
 * structure and the start of the body member within that structure. */
#define  KI_BLOCK_HEADER_LENGTH         (offsetof(KiMemoryBlock, body))

#if !defined (KI_DISABLE_FIXED_BLOCK_MEMORY)

/** Calculate the actual block length.
 * This macro calculates the actual block length for a block within a
 * fixed block memory pool. This takes into account any overheads
 * that the GKI adds onto the block. Since fixed block memory is now
 * only used for some signals the signal header is also taken into
 * account.
 *
 * \param[in] bLOCKlENGTH User defined block length.
 * \param[in] uNIT Unit size to round to.
 *
 * \return The actual block length required in bytes. */
#define M_KiMemCalcActualBlockLen(bLOCKlENGTH, uNIT)    \
            (((((bLOCKlENGTH) + sizeof(KiSigHeader) + KI_CHECK_VALUES_SIZE) + (uNIT) - 1) / (uNIT)) * (uNIT))

/** Calculate fixed block pool size.
 * This macro is used to calculate a fixed block pool size given the
 * block length and count. This takes into account any OS overheads.
 * This is normally used in the following way:
 *
 * \code
 *     pool_size = M_KiMemCalcPoolSize(M_KiMemCalcActualBlockLen(block_length, 4), block_count);
 * \endcode
 *
 * \note It could be incorrect for the KI_OS_MEM_BLOCK_OVERHEAD to be accounted
 *           for here. It may be better to move this to #M_KiMemCalcActualBlockLen().
 *
 * \param[in] bLOCKlEN The block length of each block in the pool.
 * \param[in] bLOCKcNT The number of blocks within the pool.
 *
 * \return The size of the pool in bytes. */
#define M_KiMemCalcPoolSize(bLOCKlEN,bLOCKcNT)   \
            ((((bLOCKlEN) + (KI_OS_MEM_BLOCK_OVERHEAD)) * (bLOCKcNT)) + (KI_OS_MEM_POOL_OVERHEAD))


/** Get the pool id of the smallest block pool that with fit the given size.
 *
 * \param[in] sIZE The size of memory required.
 * \param[out] pOOL The id of the smallest block pool that can store the
 *                 given size. */
#ifndef TEMP_FPGA_ML1S_HACKS
#ifdef FRT_KERNEL
#error Define headers better. At least as TEMP_FPGA_ML1S_HACKS is configured !!!
#endif
#define M_KiSizeToPool(sIZE, pOOL)                         \
{                                                          \
    for (pOOL = 0;                                         \
          ((pOOL) < KI_POOL_NUM_USER) &&                     \
          ((sIZE) > kiMemoryPoolTable[pOOL].blockSize);      \
         (pOOL)++)                                           \
    { }                                                    \
}
#else
/**/ 
/* Headers. TODO. Remove fixed sizes. Fix extra 8bit if debugging not on. */

#ifdef KI_MEMORY_USAGE
#define FRT_DEBUG_BLOCK_HEADER 0
#else
#define FRT_DEBUG_BLOCK_HEADER 8
#endif

/* Block set tracing aid */
extern Int16 last_numberOfBytesRequired;

#define M_KiSizeToPool(sIZE, pOOL)                         \
{                                                          \
	  last_numberOfBytesRequired=(Int16)(sIZE-KI_BLOCK_HEADER_LENGTH); \
    for (pOOL = 0;                                         \
          ((pOOL) < KI_POOL_NUM_USER) &&                     \
          ((sIZE + FRT_DEBUG_BLOCK_HEADER ) > kiMemoryPoolTable[pOOL].blockSize);      \
         (pOOL)++)                                           \
    { }                                                    \
}
#endif

/** Get the block size of a given pool.
 * This returns the block size for the given fixed block memory pool.
 *
 * \param[in] pOOL The id of the pool to get the block size for.
 *
 * \return The block size for the given pool. */
#define M_KiGetPoolBlockSize(pOOL)  kiMemoryPoolTable[pOOL].blockSize

#endif /* KI_DISABLE_FIXED_BLOCK_MEMORY */

/** Set memory allocation timestamp.
 * Set the timestamp field in the debug info header of a memory allocation.
 * Currently this is the GKI tick time as this makes it possible to align
 * when an allocation occurred with a GKI signal log. */
#if defined (KI_MEMORY_USAGE) && defined (KI_MEMORY_TIMESTAMP)
void KiOsGetRelativeTime (KernelTicks *time);
# define M_KiMemDebugTimestampSet(sTORAGE) KiOsGetRelativeTime ((KernelTicks *)&(sTORAGE));
#else
# define M_KiMemDebugTimestampSet(sTORAGE)
#endif

/** Save memory debug info.
 * This macro is used to save the memory debug info in the #KiMemDebugInfo
 * structure. This macro resolves to nothing when #KI_MEMORY_USAGE is not
 * defined. */
#if defined (KI_MEMORY_USAGE)
#define M_KiMemDebugInfoSet(dEBUGiNFO, cALLERaDDR, tASKiD, aLLOCsRC) \
    {                                                                \
        dEBUGiNFO.callerAddress = cALLERaDDR;                        \
        dEBUGiNFO.taskId        = tASKiD;                            \
        dEBUGiNFO.allocSource   = aLLOCsRC;                          \
        M_KiMemDebugTimestampSet(dEBUGiNFO.timestamp);               \
    }
#else
#define M_KiMemDebugInfoSet(dEBUGiNFO, cALLERaDDR, tASKiD, aLLOCsRC)
#endif

/***************************************************************************
 *  Memory overwrite checking
 **************************************************************************/

/** \def KI_DONT_CHECK_MEMORY_OVERWRITE
 * Prevent checking of memory overwrite data.
 * When #DEVELOPMENT_VERSION is defined the default operation is to add
 * check bytes to the end of memory and then check these bytes whenever
 * the memory is accessed using GKI functions. Defining this conditional
 * will remove the check bytes and code. */
#if !defined (DEVELOPMENT_VERSION)
# define KI_DONT_CHECK_MEMORY_OVERWRITE
#endif

/** Number of memory check bytes.
 * This defines the number of check bytes to add to the end of every GKI
 * memory allocation. */
#if defined (KI_DONT_CHECK_MEMORY_OVERWRITE)
# define KI_MEMORY_CHECK_VALUES_SIZE (0)
#else
# define KI_MEMORY_CHECK_VALUES_SIZE 4
#endif

/** Memory check byte value.
 * This is the value to write in the memory check bytes. */
#if !defined (KI_DONT_CHECK_MEMORY_OVERWRITE)
# define KI_MEMORY_CHECK_VALUE       0x85
#endif

/** Add check bytes to memory.
 * This macro is used to add the check bytes to a block of memory.
 *
 * \param[in] bLOCKpTR Memory block to add the check bytes to.
 * \param[in] bYTESrEQUESTED The number of user bytes in the block. The
 *                check bytes will be added after the user bytes. */
#if defined (KI_DONT_CHECK_MEMORY_OVERWRITE)
# define M_KiAddMemoryOverwriteCheckData(bLOCKpTR, bYTESrEQUESTED)
#else
# define M_KiAddMemoryOverwriteCheckData(bLOCKpTR, bYTESrEQUESTED)           \
            memset ((Int8 *)((Int8*)(bLOCKpTR)) + (KI_BLOCK_HEADER_LENGTH + (bYTESrEQUESTED)), \
                    KI_MEMORY_CHECK_VALUE, KI_MEMORY_CHECK_VALUES_SIZE);
#endif

/** Check a memory blocks check bytes.
 * This macro will check the given memory blocks check bytes to
 * make sure they are valid. If they are not valid an assert will occur.
 *
 * \param[in] bLOCKpTR Memory block to check.
 * \param[in] bLOCKlENGTH Length of user data in the block. */
#if defined (KI_DONT_CHECK_MEMORY_OVERWRITE)
# define M_KiCheckMemoryOverwriteCheckData(bLOCKpTR, bLOCKlENGTH)
#else
# if defined(KI_MEMORY_DEBUG)
#  define M_KiCheckMemoryOverwriteCheckData(bLOCKpTR, bLOCKlENGTH)           \
            {                                                                \
                Int16 i;                                                     \
                Int8 *ptr;                                                   \
                ptr = (Int8*)(bLOCKpTR) + KI_BLOCK_HEADER_LENGTH + bLOCKlENGTH; \
                for (i = 0; i < KI_MEMORY_CHECK_VALUES_SIZE; i++)            \
                {                                                            \
                    if (*(ptr + i) != (KI_MEMORY_CHECK_VALUE))               \
                    {                                                        \
                        char msg[48];                                        \
                        snprintf(msg, sizeof(msg), "Lost memory block tailer %s#%d",file,line); \
                        DevFail(msg);                                        \
                    }                                                        \
                }                                                            \
            }
# else
#  define M_KiCheckMemoryOverwriteCheckData(bLOCKpTR, bLOCKlENGTH)           \
            {                                                                \
                Int16 i;                                                     \
                Int8 *ptr;                                                   \
                ptr = (Int8*)(bLOCKpTR) + KI_BLOCK_HEADER_LENGTH + bLOCKlENGTH; \
                for (i = 0; i < KI_MEMORY_CHECK_VALUES_SIZE; i++)            \
                {                                                            \
                    DevCheck(*(ptr + i) == (KI_MEMORY_CHECK_VALUE),          \
                             (bLOCKpTR), ptr + i, *(ptr + i));               \
                }                                                            \
            }
# endif
#endif /* KI_DONT_CHECK_MEMORY_OVERWRITE */

/***************************************************************************
 * Typedefs
 **************************************************************************/

#if !defined (KI_DISABLE_FIXED_BLOCK_MEMORY)
/** Information for a fixed block memory pool.
 * This structure holds information about a fixed block memory pool.
 * An array of these structures is filled in by #KI_MEM_POOL_DEF macros
 * in kisysmem.h. */
typedef struct KiMemoryPoolEntryTag
{
    /** The size of a single block. */
    Int16    blockSize;
    /** The number of blocks in this pool. */
    Int16    numBlocks;
    /** The total pool size. */
    Int32    poolSize;
    /** A pointer to the pool memory. */
    Int32   *poolPtr;
    /** The maximum number of requests that are allowed in this pool. */
    Int16    maxReqBlocks;

} KiMemoryPoolEntry;
#endif /* KI_DISABLE_FIXED_BLOCK_MEMORY */

/** Memory header.
 * This is the header for a #KiMemoryBlock. This contains information
 * required by the GKI for the associated memory block. */
typedef struct KiMemoryBlockHeaderTag
{
    /** Generic header.
     * This is a generic memory header containing debug, pool and priority
     * information. This must be first in this structure. */
    KiGenMemHeader                  genHeader;

#if defined (KI_MEM_POOL_STATISTICS) || !defined (KI_DISABLE_DYNAMIC_MEMORY)
    /** Size of data that was requested (not including overheads). */
    Int32                           length;
#endif

} KiMemoryBlockHeader;

/** Memory block structure.
 * This structure is the header that the GKI adds to every memory block
 * when it is allocated. It provides information for about the block that is
 * used both in normal operation and for debug purposes. The header for
 * this structure is calculated by the #KI_BLOCK_HEADER_LENGTH macro. */
typedef struct KiMemoryBlockTag
{
    /** Block header. */
    KiMemoryBlockHeader             header;
    /** Memory body.
     * Use SysBasicTypesUnion to ensure correct alignment. */
    SysBasicTypesUnion              body;
}
KiMemoryBlock;


/***************************************************************************
 * Global Data
 **************************************************************************/

#if !defined (KI_DISABLE_FIXED_BLOCK_MEMORY)

/** Fixed block memory pool table.
 * This is the constant table of information which is used to define
 * all the memory pools. It is filled in with the KI_MEM_POOL_DEF macro
 * in the KISYSMEM.H file. */
extern const  KiMemoryPoolEntry kiMemoryPoolTable[KI_POOL_NUM_USER];

/** Fixed block pool handle table.
 * This is the storage for the OS memory pool handles which are used
 * by the GKI memory allocation routines. */
extern KiOsPoolHandle           kiMemoryPoolIdTable[KI_POOL_NUM_USER];

/** Fixed block pool requested block count.
 * The number of blocks that that are currently requested in each pool.
 * The Allocs and Creates always get serviced but Requests only get serviced
 * if the number of currently requested blocks in a pool is below that pools
 * maximum requested blocks. */
extern Int16                    kiBlocksRequested[KI_POOL_NUM_USER];

#endif /* KI_DISABLE_FIXED_BLOCK_MEMORY */

#if !defined (KI_DISABLE_DYNAMIC_MEMORY)
# if defined (KI_DISABLE_KM_DYNAMIC_MEMORY)
/** Dynamic heap pool handle.
 * This is the storage for the OS memory pool handles which are used
 * by the GKI memory allocation routines. */
extern KiOsDynamicPoolHandle    kiDynamicMemoryPoolId;
# endif

/** Dynamic heap pool requested byte count.
 * The number of bytes that that are currently requested in the dynamic heap
 * pool. The Allocs and Creates always get serviced but Requests only get
 * serviced if the number of currently requested bytes in the pool is below
 * the maximum requested bytes limit (#KI_DYNAMIC_MEM_REQUEST_LIMIT). */
extern volatile Int32           kiDynamicMemoryRequested;

/** Number of tasks waiting on dynamic heap memory. */
extern Int16                    kiWaitingForMemoryTaskCount;
#endif /* KI_DISABLE_DYNAMIC_MEMORY */

/***************************************************************************
 * Global Functions
 **************************************************************************/

/** Initialise the GKI memory pools.
 * This function is called by #KiReset to initialise all the GKI memory
 * pools. */
void KiInitialiseMemoryPools(void);

/** @} */ /* End of PrdGki group */

#endif

/* END OF FILE */
