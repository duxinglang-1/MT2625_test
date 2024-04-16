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
 * GKI header for memory API.
 **************************************************************************/

#ifndef GKIMEM_H
#define GKIMEM_H

/** \addtogroup PrdGki
 * @{
 */

#if defined (__cplusplus)
extern "C"
{
#endif

/***************************************************************************
 * Config
 **************************************************************************/

/** \def KI_MEMORY_DEBUG
 * Enable file and line information to be passed to memory functions.
 * This conditional adds file and line parameters to all of the GKI memory
 * function calls. This allows the GKI memory functions to display the
 * file and line of the calling code if they assert.
 *
 * Because the GKI memory API is a set of macros that map onto functions
 * the extra parameters are hidden from the user, but enabling this
 * option will require all code using the API to be recompiled. */

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <kicfg.h>

/***************************************************************************
 * Defines
 **************************************************************************/

#if defined (KI_MEMORY_DEBUG)
# define KI_MEM_DBG_EXTRA_ARGS_DEF       , const char *file, int line
# define KI_MEM_DBG_EXTRA_ARGS_USE       , MODULE_NAME, __LINE__
# define KI_MEM_DBG_EXTRA_ARGS_PASS      , file, line
#else
# define KI_MEM_DBG_EXTRA_ARGS_DEF
# define KI_MEM_DBG_EXTRA_ARGS_USE
# define KI_MEM_DBG_EXTRA_ARGS_PASS
#endif

/** New Dynamic Memory API enabled.
 * This define indicates that this version of the GKI has the new dynamic
 * memory API. This can be used to conditionally compile code that needs
 * to support older versions of the API. This is used in geniexec.c to
 * detect the new API. */
#define KI_NEW_DYN_MEM_API

/***************************************************************************
 * Kernel Function redirections
 **************************************************************************/

/** Allocate some memory.
 * This function is used to allocate the requested amount of memory from the
 * dynamic heap pool. If there is not enough free memory to allocated the
 * required amount and if asserts are being checked (#KI_CHECK_ASSERTIONS)
 * this function will assert. If asserts are not being checked it will block
 * the calling task and wait for memory to become available. This is a
 * recovery mode and should not be relied upon in normal operation. If the
 * caller wants to handle out of memory conditions itself use either
 * #KiAllocPollMemory() or #KiRequestMemory(). The contents of the
 * allocated memory is undefined, use #KiAllocZeroMemory if you need the
 * memory cleared.
 *
 * \note Passing a size of zero for the allocation will cause this function
 *           to allocate a block with zero user bytes but a valid pointer.
 *           It is unclear why this was done but it has not been changed
 *           due to the risk of some legacy code depending on this
 *           functionality.
 *
 * \param[in] N An #Int32 indicating the number of bytes required.
 * \param[out] B A pointer to a void pointer in which to store the address
 *                   of the allocated memory. If *B is not NULL on entry
 *                   to this function it will assert as this can indicate
 *                   a memory leak. */
#define     KiAllocMemory(N,B)            KiOsAllocMemoryEx     ((N),(B) KI_MEM_DBG_EXTRA_ARGS_USE)

/** Allocate some memory and zero its contents.
 * This function is basically the same as #KiAllocMemory except when
 * the memory is allocated it is set to all zeroes.
 *
 * \param[in] N An #Int32 indicating the number of bytes required.
 * \param[out] B A pointer to a void pointer in which to store the address
 *                   of the allocated memory. If *B is not NULL on entry
 *                   to this function it will assert as this can indicate
 *                   a memory leak. */
#define     KiAllocZeroMemory(N,B)        KiOsAllocZeroMemoryEx ((N),(B) KI_MEM_DBG_EXTRA_ARGS_USE)

/** Attempt to allocate some memory.
 * This function is similar to #KiAllocMemory except if the required
 * amount of memory cannot be allocated it will return rather than
 * assert or block.
 *
 * \param[in] N An #Int32 indicating the number of bytes required.
 * \param[out] B A pointer to a void pointer in which to store the address
 *                   of the allocated memory. If *B is not NULL on entry
 *                   to this function it will assert as this can indicate
 *                   a memory leak.
 *
 * \return A #Boolean indicating if the memory was allocated (TRUE) or
 *             not (FALSE). */
#define     KiAllocPollMemory(N,B)        KiOsAllocPollMemory   ((N),(B) KI_MEM_DBG_EXTRA_ARGS_USE)

/** Attempt to resize a memory block.
 * This function attempts to resize the given block. Resizing is different
 * from re-allocation as it does not require the copying of any data and
 * the user pointer stays the same. Only the size of the block associated
 * with the user pointer is affected. This is much quicker than
 * re-allocation if it is successful.
 *
 * Reducing the size of a block should always work but increasing the size
 * depends on the underlying allocation scheme and whether there is enough
 * free memory after the current block of memory.
 *
 * \param[in] N An #Int32 indicating the new number of bytes required.
 * \param[in] B A void pointer to a currently allocated block of memory. If
 *                 this does not point to a valid allocated block of memory
 *                 the function will assert.
 *
 * \return A #Boolean indicating if the memory was resized (TRUE) or
 *             not (FALSE). */
#define     KiResizeMemory(N,B)           KiOsResizeMemory      ((N),(B) KI_MEM_DBG_EXTRA_ARGS_USE)

/** Request some memory.
 * This function is similar to #KiAllocMemory except that the allocation
 * will only succeed if there is enough free memory and this allocation
 * does not cause the request limit (see #KI_DYNAMIC_MEM_REQUEST_LIMIT)
 * for the memory pool to be exceeded. Also if the allocation fails the
 * function does not assert or block it just returns leaving the output
 * buffer pointer set to NULL.
 *
 * \param[in] N An #Int32 indicating the number of bytes required.
 * \param[out] B A pointer to a void pointer in which to store the address
 *                   of the allocated memory. If *B is not NULL on entry
 *                   to this function it will assert as this can indicate
 *                   a memory leak. */
#define     KiRequestMemory(N,B)          KiOsRequestMemoryEx   ((N),(B) KI_MEM_DBG_EXTRA_ARGS_USE)

/** Re-allocate a memory block.
 * This function will re-allocate an existing memory block copy over
 * the contents from the old block to the new block. If there is not
 * enough free memory to allocated the required amount and if asserts
 * are being checked (#KI_CHECK_ASSERTIONS) this function will assert.
 * If asserts are not being checked it will block the calling task and
 * wait for memory to become available. This is a recovery mode and
 * should not be relied upon in normal operation. If the input buffer
 * is NULL this function operates like #KiAllocMemory.
 *
 * \param[in] N An #Int32 indicating the number of bytes required.
 * \param[in,out] B A pointer to a void pointer giving the buffer to
 *                    re-allocate. This buffer must have been
 *                    allocated using any of the other GKI memory
 *                    allocation functions. */
#define     KiReallocMemory(N,B)          KiOsReallocMemoryEx   ((N),(B) KI_MEM_DBG_EXTRA_ARGS_USE)

/** Request re-allocation of memory.
 * This function is similar to #KiReallocMemory except that the re-allocation
 * will only succeed if there is enough free memory and this allocation
 * does not cause the request limit (see #KI_DYNAMIC_MEM_REQUEST_LIMIT)
 * for the memory pool to be exceeded. Also if the re-allocation fails the
 * function does not assert or block it just returns a #Boolean indicating
 * this and leaves the current buffer untouched.
 *
 * \param[in] N An #Int32 indicating the number of bytes required.
 * \param[in,out] B A pointer to a void pointer giving the buffer to
 *                    re-allocate. This buffer must have been
 *                    allocated using any of the other GKI memory
 *                    allocation functions.
 *
 * \return A #Boolean indicating if the re-allocation succeeded (TRUE)
 *             or not (FALSE). */
#define     KiRerequestMemory(N,B)        KiOsRerequestMemoryEx ((N),(B) KI_MEM_DBG_EXTRA_ARGS_USE)

/** Free a GKI memory block.
 * This function frees the given GKI memory block.
 *
 * \param[in] B A pointer to a void pointer indicating the memory block
 *                to free. The pointer will be set to NULL on exit from
 *                this function. */
#define     KiFreeMemory(B)               KiOsFreeMemory        ((B) KI_MEM_DBG_EXTRA_ARGS_USE)

/** Set the caller address for a memory block.
 * This is a debug function that allows user code to overide the default
 * caller address stored for each allocation when #KI_MEMORY_USAGE is
 * enabled. This can be useful if the GKI allocation functions are being
 * called from another abstraction layer e.g. an implementation of new
 * in C++. The caller address is normally collected using the
 * #M_KiGetCallerAddress() macro.
 *
 * \param[in] U An #Int32 giving the new caller address to store.
 * \param[in] B A pointer to a void pointer indicating the associated
 *                 GKI memory. */
#define     KiSetMemoryCaller(U,B)        KiOsSetMemoryCaller   ((U),(B) KI_MEM_DBG_EXTRA_ARGS_USE)

/** Attempt to allocate some memory from non-cacheable dynamic memory pool.
 * This function will return rather than assert or block if the required
 * amount of memory cannot be allocated.
 *
 * \param[in] N An #Int32 indicating the number of bytes required.
 * \param[out] B A pointer to a void pointer in which to store the address
 *                   of the allocated memory. If *B is not NULL on entry
 *                   to this function it will assert as this can indicate
 *                   a memory leak.
 *
 * \return A #Boolean indicating if the memory was allocated (TRUE) or
 *             not (FALSE). */
#define     KiAllocPollMemoryNonCacheable(N,B)        KiOsAllocPollMemoryNonCacheable   ((N),(B) KI_MEM_DBG_EXTRA_ARGS_USE)
#define     KiAllocPollIntMemoryNonCacheable(N,B)     KiOsAllocPollIntMemoryNonCacheable ((N),(B) KI_MEM_DBG_EXTRA_ARGS_USE)

/** Attempt to resize a memory block in non-cacheable dynamic memory pool.
 * This function attempts to resize the given block. Resizing is different
 * from re-allocation as it does not require the copying of any data and
 * the user pointer stays the same. Only the size of the block associated
 * with the user pointer is affected. This is much quicker than
 * re-allocation if it is successful.
 *
 * Reducing the size of a block should always work but increasing the size
 * depends on the underlying allocation scheme and whether there is enough
 * free memory after the current block of memory.
 *
 * \param[in] N An #Int32 indicating the new number of bytes required.
 * \param[in] B A void pointer to a currently allocated block of memory. If
 *                 this does not point to a valid allocated block of memory
 *                 the function will assert.
 *
 * \return A #Boolean indicating if the memory was resized (TRUE) or
 *             not (FALSE). */
#define     KiResizeMemoryNonCacheable(N,B)           KiOsResizeMemoryNonCacheable      ((N),(B) KI_MEM_DBG_EXTRA_ARGS_USE)
#define     KiResizeIntMemoryNonCacheable(N,B)        KiOsResizeIntMemoryNonCacheable   ((N),(B) KI_MEM_DBG_EXTRA_ARGS_USE)

/** Free a GKI memory block from non-cacheable dynamic memory pool.
 * This function frees the given GKI memory block.
 *
 * \param[in] B A pointer to a void pointer indicating the memory block
 *                to free. The pointer will be set to NULL on exit from
 *                this function. */
#define     KiFreeMemoryNonCacheable(B)   KiOsFreeMemoryNonCacheable        ((B) KI_MEM_DBG_EXTRA_ARGS_USE)
#define     KiFreeIntMemoryNonCacheable(B)   KiOsFreeIntMemoryNonCacheable     ((B) KI_MEM_DBG_EXTRA_ARGS_USE)

/***************************************************************************
 * Function prototypes
 **************************************************************************/

void       KiOsAllocMemoryEx       (Int32                 numberOfBytesRequired,
                                    void                **buffer KI_MEM_DBG_EXTRA_ARGS_DEF);

void       KiOsAllocZeroMemoryEx   (Int32                 numberOfBytesRequired,
                                    void                **buffer KI_MEM_DBG_EXTRA_ARGS_DEF);

Boolean    KiOsAllocPollMemory     (Int32                 numberOfBytesRequired,
                                    void                **buffer KI_MEM_DBG_EXTRA_ARGS_DEF);

Boolean    KiOsResizeMemory        (Int32                 numberOfBytesRequired,
                                    void                 *buffer KI_MEM_DBG_EXTRA_ARGS_DEF);

void       KiOsRequestMemoryEx     (Int32                 numberOfBytesRequired,
                                    void                **buffer KI_MEM_DBG_EXTRA_ARGS_DEF);

void       KiOsReallocMemoryEx     (Int32                 numberOfBytesRequired,
                                    void                **buffer KI_MEM_DBG_EXTRA_ARGS_DEF);

Boolean    KiOsRerequestMemoryEx   (Int32                 numberOfBytesRequired,
                                    void                **buffer KI_MEM_DBG_EXTRA_ARGS_DEF);

void       KiOsFreeMemory          (void                **buffer KI_MEM_DBG_EXTRA_ARGS_DEF);

void       KiOsSetMemoryCaller     (Int32                 memoryUsageCaller,
                                    void                **buffer KI_MEM_DBG_EXTRA_ARGS_DEF);

Boolean    KiOsAllocPollMemoryNonCacheable     (Int32                 numberOfBytesRequired,
                                    void                **buffer KI_MEM_DBG_EXTRA_ARGS_DEF);

Boolean    KiOsResizeMemoryNonCacheable        (Int32                 numberOfBytesRequired,
                                    void                 *buffer KI_MEM_DBG_EXTRA_ARGS_DEF);

void       KiOsFreeMemoryNonCacheable          (void                **buffer KI_MEM_DBG_EXTRA_ARGS_DEF);

Boolean    KiOsAllocPollIntMemoryNonCacheable  (Int32                 numberOfBytesRequired,
                                    void                **buffer KI_MEM_DBG_EXTRA_ARGS_DEF);

Boolean    KiOsResizeIntMemoryNonCacheable     (Int32                 numberOfBytesRequired,
                                    void                 *buffer KI_MEM_DBG_EXTRA_ARGS_DEF);

void       KiOsFreeIntMemoryNonCacheable       (void                **buffer KI_MEM_DBG_EXTRA_ARGS_DEF);

#if defined (__cplusplus)
}
#endif

/** @} */ /* End of PrdGki group */

#endif

/* END OF FILE */
