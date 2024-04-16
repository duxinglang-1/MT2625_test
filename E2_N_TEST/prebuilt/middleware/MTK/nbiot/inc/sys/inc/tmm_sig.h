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
 * File Description: Traffic Memory Manager Flow Control Signal Definitions
 **************************************************************************/

#ifndef TMM_SIG_H
#define TMM_SIG_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/
#include <system.h>
#include <tmmtyp.h>

/***************************************************************************
 * Constants
 **************************************************************************/
#define MAX_TMM_MEM_OP_RECORDS 20

/***************************************************************************
 * Type Definitions
 **************************************************************************/
typedef enum TmmCauseTag
{
    reachedSizeRatio,
    reachedCountRatio,
    reachedMinTail,
    reachedCountMax,
    unknownCause
}
TmmCause; 
 
typedef struct UtMemAboveHwmIndTag
{
    TmmPoolId poolId;
    TmmCause  cause;
    Int32     threshold;
    Int32     currentAmount;
}
UtMemAboveHwmInd;

typedef struct UtMemBelowLwmIndTag
{
    TmmPoolId poolId;
    Int32       thresholdSize;
    Int32       currentSize;
    Int32       thresholdCount;
    Int32       currentCount;
}
UtMemBelowLwmInd;

typedef enum UtMemOperationTag
{
    TMM_NO_OP = 0,
    TMM_FREE_MEM,
    TMM_ALLOC_MEM,
    TMM_TAIL_MEM
}
UtMemOperation;

typedef struct UtMemOpDebugEntryTag
{
    UtMemOperation operation;
    TmmPoolId      poolId;
    Int32          frameNumber;
    Int32          kernelTicks;
    TaskId         taskId;

    Int8           *mem_p;
    Int32          reqBytes;
    Int32          minBytes;
    Int32          blkSize; /* bytes allocated, bytes freed or bytes remaining after a tail */

    Int32          allocdMemory;
    Int32          allocCount;
}
UtMemOpDebugEntry;

typedef struct UtMemOpDebugIndTag
{
/** \assoc ARRAY \ref record */
    Int16             numRecords;
    UtMemOpDebugEntry record [MAX_TMM_MEM_OP_RECORDS];
}
UtMemOpDebugInd;
#endif

/* END OF FILE */
