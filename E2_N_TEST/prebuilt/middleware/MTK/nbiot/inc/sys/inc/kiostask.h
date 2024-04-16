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
 * Internal GKI header for task related data and functions.
 **************************************************************************/

#ifndef KIOSTASK_H
#define KIOSTASK_H

/** \addtogroup PrdGki
 * @{
 */

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <kitqid.h>
#include <kios.h>
#include <kisystyp.h>
#include <ki_typ.h>
#include <errno.h>

#if defined (KI_ENABLE_OWN_QUEUEING)
#include <kiossem.h>
#endif
#include <kiosspec.h>

/***************************************************************************
 * Config
 **************************************************************************/

/** \def KI_TASK_USE_INT16_SET_INDEX
 * This conditional controls the size of the #KiTaskSetIndex type. This type
 * is normally an Int8 but if there are a large number of tasks it may need
 * to be an Int16. There is a check in #KiInitialiseTasks that will assert
 * if this conditional is required. */

/** \def KI_OS_EXTRA_TASK_INIT_INFO
 * This conditional allows OS specific items to be added to the
 * #KiTaskInitDef type. This adds a member of type #KiOsExtraTaskInitInfo
 * into the #KiTaskInitDef structure. The #KiOsExtraTaskInitInfo type must
 * be defined in the OS specific GKI header file included from KIOS.H. This
 * conditional can also be enabled in the same OS specific GKI header file. */

/***************************************************************************
 * Typedefs
 **************************************************************************/

/** Split system task information.
 * This structure is used to hold information used when running a split
 * system. It is only used when the #ENABLE_SPLIT_SYSTEM conditional compile
 * is switched on. */
typedef struct KiTaskSplitDefTag
{
    /** Indicates whether the task is active on target or PC. */
    Boolean         onTarget;
    /** The OS task handle for the associated task. */
    KiOsTaskHandle  osTaskId;
}
KiTaskSplitDef;

/** Constant task configuration data.
 * This structure is used to hold information about a GKI task. This is
 * the type of the #kiTaskInitTable array.
 *
 * \note This must hold only things that are known at compile time as
 *           the #kiTaskInitTable array is marked as constant. */
typedef struct KiTaskInitDefTag
{
    /** The GKI task id of this task. */
    TaskId                  taskId;
    /** Pointer to entry point function for this task. */
    void                    (*entryPoint)();
    /** A pointer to the memory to use for this tasks stack. */
    void                    *stack;
    /** The number bytes in this tasks stack. */
    Int16                   stackSize;
    /** The priority of this task. */
    Int16                   priority;

#if defined (KI_OS_EXTRA_TASK_INIT_INFO)
    /** A GKI-OS port specific type that can be used to store OS
     * specific information. This is only present if
     * #KI_OS_EXTRA_TASK_INIT_INFO is defined. The
     * #KiOsExtraTaskInitInfo type must be defined in an OS specific
     * GKI file that is included from KIOS.H. This type will be filled
     * in by the #KI_TASK_DEF macro (normally when processing the
     * KI_TASK_SETUP step). */
    KiOsExtraTaskInitInfo   osExtraInfo;
#endif

} KiTaskInitDef;

/***** EK *****/
typedef struct KiTaskNameDefTag
{
    /** Pointer to name for this task. */
    char                    *taskName;
} KiTaskNameDef;
/***** EK *****/

/** Count number of task sets.
 * This enumeration is only used to get the number of task sets, it is used
 * to dimension various GKI arrays. */
#define KI_TASK_DEF_TYPE   KI_TASK_SETS
#include <kitskdef.h>
typedef enum KiTaskSetsTag
{

#include <kisystsk.h>

    /** The total number of task sets in the system. */
    KI_NUM_OF_TASK_SETS

} KiTaskSets;

/** Task set index type.
 * This type is used for the #kiTaskSetIndexes table, it depends on the total
 * number of unique task id's defined (#KI_NUM_UNIQUE_TASK_IDS_DEFINED) but
 * unfortunately this cannot be calculated at compile time (some compilers do
 * not let you use enumerated values in pre-processor commands). There is a
 * check at run time in #KiInitialiseTasks to make sure it is big enough, if
 * not the #KI_TASK_USE_INT16_SET_INDEX conditional will need to be defined. */
#if defined (KI_TASK_USE_INT16_SET_INDEX)
typedef Int16 KiTaskSetIndex;
#else
typedef Int8 KiTaskSetIndex;
#endif

/** Task signal queue type.
 * This structure defines a task queue when #KI_ENABLE_OWN_QUEUEING is
 * enabled. */
typedef struct KiTaskQueueTag
{
    /** The signal list for this queue. */
    KiFifoSigQueue      queue;
    /** The controlling semaphore for this queue. */
    KiSemDef            sema;

} KiTaskQueue;

#if defined (KI_OS_TASK_WRAPPERS_REQUIRED)
/** Task entry point type.
 * This type defines the OS required task entry point format. This type is
 * only present if #KI_OS_TASK_WRAPPERS_REQUIRED gets defined
 * (automatically) by #kiosspec.h. This only happens if the OS specific
 * header file included in kiosspec.h indicates that it needs a task entry
 * point in a different format from the normal GKI format. */
typedef KI_OS_ENTRY_POINT (*KiOsEntryPoint) (KI_OS_TASK_ARGS);
#endif

/***************************************************************************
 * Macros
 **************************************************************************/

/** Task offset within task set.
 * This macro returns the offset of the given task within the task set that
 * the task belongs to.
 *
 * \param[in] tASKiD The #TaskId of the task to get the offset for.
 *
 * \return The offset within the task set for the given task. */
#define M_KiTaskGetSetOffset(tASKiD)     ((tASKiD) & KI_TASK_SET_INDEX_MASK)

/** Get task set.
 * This macro returns the Task set of the given task.
 *
 * \param[in] tASKiD A GKI #TaskId for which to get the set.
 *
 * \return The set number of the task. */
#define M_KiTaskGetSet(tASKiD)           ((tASKiD) >> KI_TASK_SET_SIZE_POWER_OF_2)

/** Get index in a #KI_NUM_UNIQUE_TASK_IDS_DEFINED array for a task.
 * This macro returns the index into an array dimensioned by
 * #KI_NUM_UNIQUE_TASK_IDS_DEFINED for the given task id.
 *
 * \param[in] tASKiD A GKI #TaskId to get the index for.
 *
 * \return The index into an array dimensioned by
 *             #KI_NUM_UNIQUE_TASK_IDS_DEFINED for this task id. */
#define M_KiTaskGetIndex(tASKiD)         (kiTaskSetIndexes[M_KiTaskGetSet(tASKiD)] + M_KiTaskGetSetOffset(tASKiD))

/** Convert a task index to a TaskId.
 * This macro returns the TaskId that corresponds to the given index
 * in an array dimensioned by #KI_NUM_UNIQUE_TASK_IDS_DEFINED.
 *
 * \param[in] iNDEX An index into an array dimensioned by
 *                #KI_NUM_UNIQUE_TASK_IDS_DEFINED.
 *
 * \return The GKI #TaskId that corresponds to the given index. */
#define M_KiTaskIndexToId(iNDEX)     kiTaskIndexesToIds[(iNDEX)]

/** Get the init index for a task.
 * This macro returns the index into an array dimensioned by
 * #KI_NUM_OF_REAL_TASKS for the given task id. If the input GKI task id
 * is not a real task (i.e. reserved task) then the returned value is
 * #KI_NUM_OF_REAL_TASKS.
 *
 * \param[in] A GKI #TaskId to get the init index for.
 *
 * \return The index into an array dimensioned by
 *             #KI_NUM_OF_REAL_TASKS for the given task id. */
#define M_KiTaskIdToInitIndex(tASKiD)  kiTaskIndexesToInitIndex[M_KiTaskGetIndex(tASKiD)]

/** Round a value.
 * This macro rounds a value up to the nearest nBYTES boundary.
 *
 * \param[in] sIZE The number to round up, normally the size of some object.
 * \param[in] nBYTES Once sIZE is rounded up it should be divisable by this
 *                number with no remainder.
 * \return The input value rounded up to the nearest nBYTES boundary. */
#define M_KiRoundToN(sIZE, nBYTES)    ((((sIZE) + ((nBYTES) - 1)) / (nBYTES)) * (nBYTES))

/** Round a value up to nearest 4 byte boundary.
 *
 * \param[in] sIZE The number to round up, normally the size of some object.
 *
 * \return The input value rounded up to the nearest 4 byte boundary. */
#define M_KiRoundToLong(sIZE)        M_KiRoundToN(sIZE, 4)

/***************************************************************************
 * Global Data
 **************************************************************************/

#if defined (ENABLE_SPLIT_SYSTEM)
/** Task information needed for split-system builds.
 * A table which contains information on all tasks when running a split
 * system. It is only used when the #ENABLE_SPLIT_SYSTEM conditional
 * compile is switched on. The table is filled in when the tasks are
 * created, see #KiInitialiseTasks(). */
extern KiTaskSplitDef       kiTaskSplitTable[KI_NUM_UNIQUE_TASK_IDS_DEFINED];
#endif

#if !defined (KI_ENABLE_OWN_QUEUEING)
/** OS handles to signal queues.
 * This table holds the Mailbox/Queue handle for every task. The table is
 * filled in when the tasks are created, see #KiInitialiseTasks(). */
extern KiOsMailboxHandle    kiTaskMailboxIdTable[KI_NUM_UNIQUE_TASK_IDS_DEFINED];
#endif

/** Task queues.
 * This is the data used to implement the task queue. It is dimensioned by
 * #KI_NUM_UNIQUE_TASK_IDS_DEFINED rather than #KI_NUM_OF_REAL_TASKS to
 * provide better run-time performance. This will waste one #KiTaskQueue entry
 * per reserved task (i.e. a unique task id that isn't a real task). In a full
 * system there should be very few reserved tasks. This is only used when
 * #KI_ENABLE_OWN_QUEUEING  is defined. */
extern KiTaskQueue          kiTaskQueues[KI_NUM_UNIQUE_TASK_IDS_DEFINED];

#if defined (KI_OS_TASK_WRAPPERS_REQUIRED)
/** Task entry point wrapper functions array.
 * This data is used to store pointers to the OS specific task entry point
 * functions. These pointers need to be used in the OS specific
 * implementation of #M_KiOsCreateTask() instead of the usual GKI entry
 * point (kiTaskInitTable[].entryPoint). The index into this array is the
 * same as would be used for kiTaskInitTable[].entryPoint. This will only
 * be present if #KI_OS_TASK_WRAPPERS_REQUIRED gets automatically defined. */
extern const KiOsEntryPoint kiOsEntryPointWrappers[KI_NUM_OF_REAL_TASKS + 1];
#endif

/** Task initialisation table.
 * This is the task initialisation table which details all the information
 * about real tasks that is required to create the tasks, their stack and
 * mailbox/queue. NOTE: this array is marked as constant. */
extern KiTaskInitDef  kiTaskInitTable[KI_NUM_OF_REAL_TASKS];
/**** EK ***/
extern KiTaskNameDef kiTaskNameTable[KI_NUM_OF_REAL_TASKS];
/**** EK ***/

/** Array used for converting from a task index to a task init index.
 * This data is used to retrieve the init task index (into the
 * kiTaskInitTable array) given a task index. The task index can be
 * calculated from the GKI task id using the #M_KiTaskGetIndex() macro. So in
 * effect it is possible to convert from a GKI task id to an index in the
 * #kiTaskInitTable. If the GKI task id represents a task that does not exist
 * i.e. a reserved task, then the value stored in this array for that task
 * id will be #KI_NUM_OF_REAL_TASKS. */
extern const Int16          kiTaskIndexesToInitIndex[KI_NUM_UNIQUE_TASK_IDS_DEFINED + 1];

/** Task set index array.
 * This data is used to index into arrays that are dimensioned by
 * #KI_NUM_UNIQUE_TASK_IDS_DEFINED. For set X the value at index X in this
 * array gives the index in a #KI_NUM_UNIQUE_TASK_IDS_DEFINED dimensioned
 * array of the first task for this set. See #M_KiTaskGetIndex() for an
 * example of how this is used. If a 'too many initialisers error' occurs
 * on this type then the task sets are not defined correctly in KISYSTSK.H
 * (the set numbering needs to be sequential from 0 upwards). */
extern const KiTaskSetIndex kiTaskSetIndexes[KI_NUM_OF_TASK_SETS + 1];

/** Task indexes to TaskId array.
 * This data is used to retrieve the GKI task id given an index into an array
 * dimensioned by #KI_NUM_UNIQUE_TASK_IDS_DEFINED. */
extern const KiTaskId       kiTaskIndexesToIds[KI_NUM_UNIQUE_TASK_IDS_DEFINED + 1];

/***************************************************************************
 * Global Functions
 **************************************************************************/

/** Initialise GKI tasks.
 * This function is called from #KiReset to create all the defined tasks
 * and their associated signal queues. The split system information is
 * also initialised if the system is built with #ENABLE_SPLIT_SYSTEM. */
void       KiInitialiseTasks       (void);

/** @} */ /* End of PrdGki group */

/***************************************************************************
 * Defines
 **************************************************************************/

#if !defined (ON_PC)

#if defined errno
#undef errno
#endif /*errno*/
#define errno ( *KiGetErrnoAddr() )

#endif /*ON_PC*/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/** Get the errno pointer for the current task.
 * This function returns  a pointer to an int which is the type used in
 * the ANSI definition of errno. The pointer returned will be specific to
 * the current task. */
extern int *KiGetErrnoAddr ( void );

/** @} */ /* End of PrdGki group */

#endif

/* END OF FILE */
