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
 * GKI header file that defines the Task and Qeueue id types.
 **************************************************************************/

#ifndef KITQID_H
#define KITQID_H

/** \addtogroup PrdGki
 * @{
 */

/***************************************************************************
 * Defines
 **************************************************************************/

/** GKI outbound comms task.
 * This configures what task is used for communicating between the target
 * (GKI) and a host PC. This task will be the route through which logged
 * signals will be sent to the host PC. It is also used to transport
 * inter-task signals as used during a split system build (or some diagnostic
 * signals e.g. #KiDevCheckInd). This defaults to the EMMI_LOW_PRI_TASK_ID. */
#if !defined (KI_COMMS_TASK_ID)
#ifdef OPTIMIZED_GKI_LOG_ENABLE
#define KI_COMMS_TASK_ID     KI_LOG_TASK_ID
#else
#define KI_COMMS_TASK_ID     EMMI_LOW_PRI_TASK_ID
#endif
#endif

/** Task set power of 2.
 * This defines the maximum size a single task set can be based on a power
 * of 2 (for simpler/quicker decoding). */
#define KI_TASK_SET_SIZE_POWER_OF_2   (8)

/** Task index within a set mask.
 * This defines the mask used to extract the offset of a task with a task
 * set from a TaskId. */
#define KI_TASK_SET_INDEX_MASK        ((1 << KI_TASK_SET_SIZE_POWER_OF_2) - 1)

/***************************************************************************
 * Type Definitions
 **************************************************************************/

#define KI_TASK_DEF_TYPE KI_TASK_IDS
#include <kitskdef.h>

/** Identifiers for all tasks in the system.
 * This defines the GKI task id's for use by the system software. This is
 * how a task is identified to the GKI. Internally this has to be translated
 * to the OS specific task id (or handle). */
typedef enum TaskIdTag  /* tasks sent to identified by their task ids */
{

#include <kisystsk.h>

    KI_TASK_ID_END,
    /* Task sets add one to the end of a set for the task set stop id,
    ** so need to subtract one to get the max id. */
    KI_MAX_TASK_ID              = KI_TASK_ID_END - 1,
    UNKNOWN_TASK_ID             = 0xFFFF,

    /** This is the task id used by PassThru tasks in Genie.
     * Do not send signals from target entities to this task
     * as it does not exist on the target. */
    PASS_THRU_TASK_ID           = 0,

    /** This is the task id for the GKI target to host comms task.
     * Logged and inter-task tasks are sent to this task.
     * The task will already have another GKI task id but the
     * name may be different depending on the transport mechanism.
     * This id allows a generic name to be used for it even if
     * the value may change between builds. This means host side tools
     * can use a known name to reference that task id. */
    GKI_COMMS_TASK_ID           = KI_COMMS_TASK_ID,

    UNKNOWN_TASK                = UNKNOWN_TASK_ID

    /* Any new TaskId which is to be recognised by GENIE should end in the
    ** string "_ID"; conversely, any value which is not to be displayed by
    ** GENIE should NOT end in this string. */

} TaskId, KiTaskId;

#define KI_TASK_DEF_TYPE   KI_QUEUE_IDS
#include <kitskdef.h>

/** Identifiers for all queues in the system.
 * This defines the GKI queue id's for use by the system software. This is
 * how a GKI task's input queue is identified to the GKI. Internally this
 * has to be translated to the OS specific queue/mailbox id. The numerical
 * values exactly matches the equivalent TaskId value.
 * \note This could be removed and TaskId used in it's place. */
typedef enum QueueIdTag
{

#include <kisystsk.h>

    KI_MAX_QUEUE_ID  = KI_MAX_TASK_ID

}
QueueId, KiQueueId;

/** Type used to count the number of real tasks.
 * This enumeration is only used to get the number of real i.e. not reserved
 * or aliased tasks. This number is then used to dimension various GKI arrays. */
#define KI_TASK_DEF_TYPE   KI_REAL_TASKS
#include <kitskdef.h>
typedef enum KiRealTasksTag
{

#include <kisystsk.h>

    /** Number of real tasks in the system. */
    KI_NUM_OF_REAL_TASKS

} KiRealTasks;

/** @} */ /* End of PrdGki group */

#endif

/* END OF FILE */
