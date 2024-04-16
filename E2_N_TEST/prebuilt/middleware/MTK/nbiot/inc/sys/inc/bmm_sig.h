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
 * File Description: Buffer Memory Manager Signal Definitions
 **************************************************************************/

#ifndef BMM_SIG_H
#define BMM_SIG_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/
#include <system.h>
#include <bmm_typ.h>
#include <bmm_cfg.h>

/***************************************************************************
 * Constants
 **************************************************************************/

/** Number of BMM operations to record before debug signal is sent.*/
#define MAX_BMM_MEM_OP_RECORDS 20     

/* 32 bit map showing status of a BMM buffer. */ 
#define BMM_BITMAP_LENGTH       4

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/** Above high watermark signal. */
typedef struct UtBmmAboveHwmIndTag
{
    /** Buffer identifier. */
    BmmPoolId  bmmPoolId;                 
    /** Current size of the buffer. */
    Int32      poolSize;
    /** Configured threshold value for High Watermark or max number of allocations. */
    Int32      hwmThreshold;
    /* Bitmap of 32 bits representing blocks that have been marked        */
    /* as containing one or more datagrams.                               */    
    BmmPoolId poolId[BMM_NUM_BUFFERS];
    Int32     bitMap[BMM_NUM_BUFFERS];
}
UtMemAboveHwmInd;

/** Below low watermark signal. */
typedef struct UtBmmBelowLwmIndTag
{
    /** Buffer identifier. */
    BmmPoolId  bmmPoolId;
    /** Current size of the buffer. */
    Int32      poolSize;
    /** Low Watermark threshold configured for the buffer. */
    Int32      lwmThreshold;
    /* Bitmap of 32 bits representing blocks that have been marked        */
    /* as containing one or more datagrams.                               */
    BmmPoolId  poolId[BMM_NUM_BUFFERS];
    Int32      bitMap[BMM_NUM_BUFFERS];
}
UtMemBelowLwmInd;

#if defined (BMM_DETAILED_LOG)
/** BMM debug information signal definition. */
typedef enum UtMemOperationTag
{
    BMM_NO_OP = 0,                    /* No operation                     */
    BMM_FREE_MEM,                     /* Free memory operation            */
    BMM_ALLOC_MEM,                    /* Allocate memory                  */
    BMM_HWM_EVENT,                    /* High watermark event             */
    BMM_LWM_EVENT                    /* Low watermark event              */
}
UtMemOperation;

typedef struct UtMemOpDebugEntryTag
{
    UtMemOperation operation;         /* Recent operation                 */
    TaskId         taskId;            /* Calling task identity            */
    BmmPoolId      bmmPoolId;         /* Identifies the Pool that is used */
    Int8          *mem_p;             /* Current memory pointer           */
    Int32          bufferId;
    Int32          head;              /* Current head                     */
    Int32          tail;              /* Current tail                     */
    Int32          reserve;           /* Current reserve                  */
    
    Int32          callerAddress;     /* Address of function calling BMM  */
    Int32          bitMap;            /* BitMap */
    /* Bitmap of 32 bits representing blocks that have been marked        */
    /* as containing one or more datagrams.                               */

}
UtMemOpDebugEntry;
#endif

typedef struct UtMemOpDebugIndTag
{
    Int16             numRecords;     /* Number of debug entires          */
                                      /* Array holding debug information  */
#if defined (BMM_DETAILED_LOG)
    UtMemOpDebugEntry record [MAX_BMM_MEM_OP_RECORDS];
#endif    
    BmmPoolId         poolId    [BMM_NUM_BUFFERS];
    Int32             bufferSize[BMM_NUM_BUFFERS];
    Int32             bitMap    [BMM_NUM_BUFFERS]; 
}
UtMemOpDebugInd;

#endif
/* END OF FILE */
