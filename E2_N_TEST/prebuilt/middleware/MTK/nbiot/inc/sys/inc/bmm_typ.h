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
 * File Description: Buffer Memory Manager  (BMM) Types
 **************************************************************************/

#ifndef BMM_TYP_H
#define BMM_TYP_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/
#include <system.h>
#include <bmm_cfg.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/* The maximum number of flow control tasks that can be registered */
#define BMM_MAX_FLOW_CTRL_TASKS  5


/***************************************************************************
 * Type Definitions
 **************************************************************************/

/***************************************************************************
 * Name        : BmmId
 * Description : BMM Buffer Identifiers.
 ***************************************************************************/

typedef enum BmmPoolIdTag
{
    BMM_UL_POOL,                      /* Uplink user plane pool.           */
    BMM_DL_POOL,                      /* Downlink user plane pool.         */
    BMM_CONTROL_POOL,                 /* Extra Bmm Pool for signalling     */
    BMM_NUM_POOLS,                    /* Number of Bmm Pools               */
    BMM_FREE_POOL,
    BMM_INVALID_POOL = 0xFFFF         /* Invalid buffer identifier.        */
}
BmmPoolId;


/***************************************************************************
 * Name        : BmmWaterMark
 * Description : This structure describes the properties of each BMM buffer
 ***************************************************************************/
typedef enum BmmWaterMarkTag
{
    BMM_HWM,                          /* High watermark indication        */
    BMM_LWM                           /* Low watermark  indication        */
}
BmmWatermark;

/***************************************************************************
 * Name        : Bmm State
 * Description : This enumerates the pool state. It indicates if the specific
 *               buffer is normal or being flow controlled.
 ***************************************************************************/
typedef enum BmmStateTag
{
    BMM_STATE_NORMAL = 0,             /* Buffer is operating normally      */
    BMM_STATE_HWM_REGION              /* High Watermark exceeded           */
}
BmmState;

/***************************************************************************
 * Name        : BmmPool
 * Description : Record size of memory allocated for each pool along with
 *               it own watermark levels.
 ***************************************************************************/
typedef struct BmmPoolTag
{
    BmmState           state;
    Int32              totalSize;
    Int32              highWatermarkThreshold;
    Int32              lowWatermarkThreshold;
    TaskId             flowControlTask[BMM_MAX_FLOW_CTRL_TASKS];
}
BmmPool;


/***************************************************************************
 * Name        : BmmBufferDescription
 * Description : Each BMM buffer allocated has an associated descriptor. This
 *               holds information specific to the buffer
 ***************************************************************************/
typedef struct BmmBufferTag
{
    BmmPoolId           poolId;     /* identified the BMM POOL           */
    Int32               head;       /* header offset                     */
    Int32               tail;       /* Tail offset                       */
    Int32               reserve;    /* Reserve offset                    */
    Int32               freeSpace;  /* Amount of free memory             */
    Int32               newAlloc;   /* new allocation location           */
    Int32               bitMap;
    Int32               plpReserve; /* Reserve offset for DL PLP use     */

}
BmmBuffer;

#endif

/* END OF FILE */
