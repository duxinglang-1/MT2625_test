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
 * GKI header file that defines the types for unit queues (#KiUnitQueue).
 **************************************************************************/

#ifndef KI_UNITQ_H
#define KI_UNITQ_H

/** \addtogroup PrdGki
 * @{
 */

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/** GKI Unit Queue structure.
 * Unit Queues are used to store signals within a task after they have been
 * removed from the tasks input queue. A Unit Queues must only be accessed
 * by a single task (accessing from multiple threads is not supported).
 * The fields within the unit queue structure should not be accessed directly
 * outside the GKI code. The API for manipulating unit queues is defined
 * in gkiqueue.h. */
typedef struct KiUnitQueueTag
{
    /* If this is enabled it will need a full re-compilation
    ** as unit queue's are created in task space data. */
    /** Head of queue.
     * This is the head of the signal queue. If this is NULL there are no signals
     * on the queue. */
    void            * head;
    /** Tail of queue.
     * This is a pointer to the tail. If this is NULL then the queue is invalid.
     * The value in *tail should always be NULL. */
    void           ** tail;
    /** Queue signal count.
     * This is the count of the number of signals currently on the queue. */
    Int32             count;
    /** Queue maximum signal count.
     * This is the count of the maximum number of signals that have been on the
     * queue. */
    Int32             maxCount;
}
KiUnitQueue;

/** @} */ /* End of PrdGki group */

#endif

/* END OF FILE */
