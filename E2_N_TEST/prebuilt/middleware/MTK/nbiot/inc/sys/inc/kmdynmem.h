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
 * GKI dynamic memory manager.
 * This file defines the API to the GKI's own dynamic memory manager.
 **************************************************************************/

#ifndef KMDYNMEM_H
#define KMDYNMEM_H

/****************************************************************************
 * Include Files
 ****************************************************************************/

#include <system.h>
#include <kicfg.h>

/****************************************************************************
 * Macros
 ****************************************************************************/

/** Calculate the actual size of a dynamic memory pool
 * This macro adds an overhead for the data needed to manage the pool. There
 * is no easy way to do this accurately as the size of the storage needed
 * to manage the pool is dependant on the highest bit set in the required user
 * size. The pre-processor does not support any way to get the highest bit set
 * in a value. So just use a hardcoded overhead value of 1536 bytes. This is
 * roughly enough size to manage up to 256KBytes. Beyond this size the
 * management data will be using a bit of user data. */
#define M_KmMemCalcRealPoolSize(uSERsIZE)  ((uSERsIZE) + 1536)

/** KmMemoryGet assert handling.
 * This macro generates an apprioriate assert based on the error returned from
 * KmMemoryGet() when it fails. */
#if defined (KI_CHECK_ASSERTIONS)
#define M_KmMemoryGetFail(eRRORcODE, mEMaREA, bYTEsIZE, mEMpTR, fILEnAME, lINEnUMBER) \
                KmMemoryGetFail (eRRORcODE, mEMaREA, bYTEsIZE, mEMpTR, fILEnAME, lINEnUMBER)
#else
#define M_KmMemoryGetFail(eRRORcODE, mEMaREA, bYTEsIZE, mEMpTR, fILEnAME, lINEnUMBER)
#endif

/****************************************************************************
 * Typedefs
 ****************************************************************************/

/** Error Codes.
 * This enumeration defines all the error codes that are used by the dynamic
 * memory manager. */
typedef enum KmErrorCodeTag
{
    /** No error (0). */
    KM_ERR_OK,
    /** Dynamic memory pool alignment error (1). */
    KM_ERR_ALIGNMENT,
    /** Dynamic memory pool is too small (2). */
    KM_ERR_MIN_SIZE_DYN_MEM_POOL,
    /** Could not allocate requested size (3). */
    KM_ERR_ALLOCATION_FAILED,
    /** Memory pool pointer is invalid (4). */
    KM_ERR_BAD_DYN_MEM_POINTER,
    /** Memory allocation before pool initialisation (5). */
    KM_ERR_DYN_MEM_NOT_INITIALISED,
    /** Resizing a block failed (6). */
    KM_ERR_RESIZE_FAILED,
    /** Invalid parameter to function (7). */
    KM_ERR_INVALID_PARAMETER,

    KM_ERR_UNKNOWN = 127
}
KmErrorCode;

/** Memory walk data structure.
 * This structure is used to hold information about a used memory block
 * when walking through a memory pool. */
typedef struct KmMemoryWalkDataTag
{
    /** Address of header data.
     * This is the address of the header used by the memory manager. */
    void * headerAddr;
    /** Address of user data.
     * This is the address of the user data. */
    void * userAddr;
    /** Size of block.
     * This is the size of the block including the header and any tail. */
    Int32  size;
    /** Indicates if the block is free (0), in use (1), pending (2), or
     * fence (3). */
    Int8   memUsage;
} KmMemoryWalkData;

/****************************************************************************
 * Function Prototypes
 ****************************************************************************/

KmErrorCode KmMemoryCreatePool (void * memArea, Int32 memByteSize);
KmErrorCode KmMemoryGet (void * memArea, Int32 allocSize, void **memPtr);
KmErrorCode KmMemoryFree (void * memArea, void *memoryToFree);
KmErrorCode KmMemoryReSize (void * memArea, void  *memoryToResize, Int32 newSize);
KmErrorCode KmMemoryGetSize (void *memoryToSize, Int32 *sizeOfMemory);
void        KmMemoryGetFail (KmErrorCode errorCode, void * memArea, Int32 getByteSize, void **memPtr, const char * fileName, Int32 lineNumber);

Boolean     KmMemoryWalkStart (void * memArea, KmMemoryWalkData * walkData);
Boolean     KmMemoryWalk      (void * memArea, KmMemoryWalkData * walkData);

Boolean     KmMemoryCheckPtrInRange (void *memArea, void *ptrToCheck);

#endif /* KMDYNMEM_H */
/* END OF FILE */
