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
 * File Description: Traffic Memory Manager Types
 **************************************************************************/

#ifndef TMMTYP_H
#define TMMTYP_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/
#include <system.h>
#include <ki_typ.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/
/* The maximum number of flow control tasks that can be registered per pool */
#define TMM_MAX_FLOW_CTRL_IDS 4

/* The maximum number of entities that can reference a particular TMM block */
#define TMM_MAX_REFERENCES    0xFF

/* This is the maximum number of memory blocks to cache to prevent excessive
 * allocations and deallocations */
#define TMM_MAX_CACHE_BLOCKS 20

/* This is the maximum allowed difference between the block held in the
 * cache and the requested block size */
#define TMM_CACHE_BLOCK_MAX_DIFF_SIZE 200

/***************************************************************************
 * Type Definitions
 **************************************************************************/
 /***************************************************************************
 * Name        : TmmProfileId
 * Description : Set the profiles for default TMM allocations
 ***************************************************************************/
typedef enum TmmProfileIdTag
{
/* Setup the profile Ids */
#define TMM_GET_PROFILE_NAME
    #include <tmmmem.h>
    TMM_NUM_PROFILES
}
TmmProfileId;

/***************************************************************************
 * Name        : TmmPoolId
 * Description : Pool Identifiers.
 ***************************************************************************/
typedef enum TmmPoolIdTag
{
/* Setup the TMM Pool IDs */
#define TMM_GET_VIRT_POOL_NAME
    #include <tmmmem.h>
    TMM_VIRTUAL_POOL_END,

#define TMM_GET_REAL_POOL_NAME
    #include <tmmmem.h>
    TMM_REAL_POOL_END,

#define TMM_GET_MAPPED_POOL_NAME
    #include <tmmmem.h>

    TMM_NUM_POOLS
}
TmmPoolId;

/***************************************************************************
 * Name        : TmmBlockHeader
 * Description : Each TMM block allocated has an associated header. This
 *               holds information specific to the block and is required by
 *               TMM for its internal management.
 ***************************************************************************/
typedef struct TmmBlockHeaderTag
{

    TmmPoolId           poolId;      /* The pool that the block was allocated from */
    Int32               blockLength; /* Size of the TMM block excluding this header */

#if defined (TMM_ENABLE_REFERENCES)
    Int32               references; /* The number of entities referring to this block */
#endif

#if defined (TMM_ENABLE_EXTENDED_DEBUG)
    void                *prevBlock_p;   /* To implement a linked list */
    void                *nextBlock_p;   /* To implement a linked list */
    TaskId              taskId;         /* Task ID of the function that requested the TMM memory */
    Int32               callerAddress;  /* Address of the function that requested the TMM memory */
#endif

/* DEVELOPMENT_VERSION */
    Int32               checkValue; /* A check value to make sure that the header was not overwritten */
/* DEVELOPMENT_VERSION */
}
TmmBlockHeader;

/***************************************************************************
 * Name        : TmmPoolProperties
 * Description : This structure is an element in a table. This table
 *               holds the properties of each particular pool.
 ***************************************************************************/
typedef struct TmmPoolPropertiesTag
{
    Int32               poolSize;      /* Size of the memory pool in bytes */
    Int32               hwm;           /* High water mark */
    Int32               lwm;           /* Low water mark */
    Int32               minTailLen;    /* The minimum tail length */
    Int32               maxAllocCount; /* This is the maximum number of allocations before flow control registered tasks are denied allocations */
    Int32               hwmAllocCount; /* The number of allocations from this pool before a HWM signal is sent */
    Int32               lwmAllocCount; /* If we are flow controlled off, then the number of
                                        * allocs must reach this level before we flow control back on */
    KiMemoryPoolId      kiMemPoolId;   /* GKI pool id */
}
TmmPoolProperties;

/***************************************************************************
 * Name        : TmmFlowCtrlData
 * Description : This structure holds a task registered to receive flow
 *               control signalling from a specific TMM pool.
 ***************************************************************************/
typedef struct TmmFlowCtrlDataTag
{
    /* The number of times the task ID has registered for flow control. */
    Int32  numInstances;

    /* The task ID registered for flow control. This field is only valid if
     * numInstances is non-zero. */
    TaskId taskId;
}
TmmFlowCtrlData;

/***************************************************************************
 * Name        : TmmPoolState
 * Description : This enumerates the pool state. It indicates if the specific
 *               pool is normal or being flow controlled.
 ***************************************************************************/
typedef enum TmmPoolStateTag
{
    TMM_POOL_STATE_NORMAL = 0, /* Pool is normal */
    TMM_POOL_STATE_OVERLOADED  /* The amount of free memory is less than the current high water mark */
}
TmmPoolState;

/***************************************************************************
 * Name        : TmmPoolData
 * Description : Holds data specific to each memory pool. Each memory pool
 *               that TMM uses has an instance of this structure.
 ***************************************************************************/
typedef struct TmmPoolDataTag
{
    Int32            allocatedMemory;                      /* The amount of memory allocated */
    TmmPoolState     state;                                /* The state of the pool */
    Int32            numFlowCtrlIds;                       /* The number of tasks registered to receive flow ctrl data from this pool */
    TmmFlowCtrlData  flowCtrlData [TMM_MAX_FLOW_CTRL_IDS]; /* Tasks registered for flow control from this pool */
    Int32            allocationCount;                      /* Number of allocations from this pool */
#if !defined (TMM_DISABLE_MEMORY_CACHE)
    Int32            lastCacheEntryIndex;                  /* This index is the index of the next element in *cache to replace */
    TmmBlockHeader   *cache [TMM_MAX_CACHE_BLOCKS];        /* This holds a cache of allocated blocks before they are returned to the memory mamanger */
#endif
}
TmmPoolData;

/***************************************************************************
 * Name        : TmmProfileData
 * Description : Profile data
 ***************************************************************************/
typedef struct TmmProfileDataTag
{
    TmmPoolId mappedPool [TMM_VIRTUAL_POOL_END];
}
TmmProfileData;

/***************************************************************************
 * Name        : TmmStateData
 * Description : This structure holds the main TMM state data. There is only
 *               one instance of this structure.
 ***************************************************************************/
typedef struct TmmStateDataTag
{
    /* This structure holds individual pool data. Only Virtual pools and
     * Real pools can be accessed by callers of TMM. Therefore, we do not
     * need to allocate space for Mapped pools. */
    TmmPoolData poolData [TMM_REAL_POOL_END];
}
TmmStateData;
#endif

/* END OF FILE */
