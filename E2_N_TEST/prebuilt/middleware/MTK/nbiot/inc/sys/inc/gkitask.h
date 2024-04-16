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
 * GKI header for task API.
 **************************************************************************/

#ifndef GKITASK_H
#define GKITASK_H

/** \addtogroup PrdGki
 * @{
 */

#if defined (__cplusplus)
extern "C"
{
#endif

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <kitqid.h>
#ifndef GEN_NVDM_DATABASE_FOR_PC
#include <kicfg.h>
#endif

#if defined (PC_KERNEL) || defined (ON_PC) || defined (ON_TARGET_UNIT_TEST)
/* PC kernel is a special case and overrides some of the
** defaults currently. This is caused by PC builds not
** building a whole system, i.e. a subset of tasks so
** there needs to be a way to detect which tasks are
** present. */
#  include <kipc.h>
#endif

/***************************************************************************
 * Defines
 **************************************************************************/

/** Task entry point return type.
 * This is the type used to define a GKI Tasks entry point e.g.
 *
 * KI_ENTRY_POINT MyTask (KI_TASK_ARGS)
 */
#if !defined (KI_ENTRY_POINT)
# define      KI_ENTRY_POINT           void
#endif

/** Task entry point arguments.
 * This defines the arguments that a GKI Task function has to have e.g.
 *
 * KI_ENTRY_POINT MyTask (KI_TASK_ARGS)
 */
#if !defined (KI_TASK_ARGS)
# define      KI_TASK_ARGS             void
#endif

/** Task exit point return type.
 * This is the type used to define a GKI Tasks exit point for PC testing e.g.
 *
 * KI_EXIT_POINT MyTaskExitRoutine (void).
 *
 * The exit point only has to be present when the #ON_PC conditional compile is
 * on. Only the PC GKI port should override this if required. */
#if !defined (KI_EXIT_POINT)
# define      KI_EXIT_POINT            void
#endif

/** Interrupt entry point return type.
 * This is the type used to define a Interrupts entry point e.g.
 *
 * KI_INT_ENTRY_POINT MyInterrupt (void)
 */
#if !defined (KI_INT_ENTRY_POINT)
# define      KI_INT_ENTRY_POINT       void
#endif

/** Declare a task entry point.
 * This is the macro used to define a single task. This macro is required
 * to enable PC testing for the associated task. It may also be used to
 * define a target entry point although the eNTRYpOINT should be used
 * directly. This macro should be called outside of functions or types
 * i.e. at the top level of a file, and within the same file as the
 * eNTRYpOINT.
 *
 * \param[in] eNTRYpOINT Function representing the tasks entry point.
 * \param[in] qUEUEiD The GKI Queue identifier (#KiQueueId) as defined in
 *                the task definition (using the #KI_TASK_DEF macro, via
 *                a file included into KISYSTSK.H).
 * \param[in] tASKiD The GKI Task identifier (#KiTaskId) as defined in the
 *                task definition (using the #KI_TASK_DEF macro, via a file
 *                included into KISYSTSK.H). */
#if !defined (KI_SINGLE_TASK)
# define      KI_SINGLE_TASK(eNTRYpOINT,qUEUEiD,tASKiD)                   \
       extern KI_ENTRY_POINT eNTRYpOINT(KI_TASK_ARGS);                    \
       KI_ENTRY_POINT eNTRYpOINT##1 (KI_TASK_ARGS)                        \
       {                                                                  \
           eNTRYpOINT ();                                                 \
       }
#endif

/** Declare multipl task entry points using the same function.
 * This macro is used to define a single task in a set of multiple tasks
 * which use the same entry point. Every task must have one of these macros
 * in the same file as the task entry point. Each task has a wrapper function
 * declared that has the same name as the entry point but with the instance
 * number appended to the name. Each task has to have a unique task
 * and queue id.
 *
 * \deprecated This should not be used anymore. The same functionality can
 *                 be achieved by using several #KI_SINGLE_TASK() macros.
 *
 * \param[in] eNTRYpOINT Function representing the tasks entry point.
 * \param[in] iNSTANCE The instance of this task in the multi task group.
 * \param[in] qUEUEiD The GKI Queue identifier (#KiQueueId) as defined in
 *                the task definition (using the #KI_TASK_DEF macro, via
 *                a file included into KISYSTSK.H).
 * \param[in] tASKiD The GKI Task identifier (#KiTaskId) as defined in the
 *                task definition (using the #KI_TASK_DEF macro, via a file
 *                included into KISYSTSK.H). */
#if !defined (KI_MULTI_TASK)
# define      KI_MULTI_TASK(eNTRYpOINT,iNSTANCE,qUEUEiD,tASKiD)           \
       extern KI_ENTRY_POINT eNTRYpOINT(QueueId, TaskId);                 \
       KI_ENTRY_POINT eNTRYpOINT##iNSTANCE (KI_TASK_ARGS)                 \
       {                                                                  \
           eNTRYpOINT (qUEUEiD, tASKiD);                                  \
       }
#endif

/***************************************************************************
 * Global data
 **************************************************************************/

#if defined(UPGRADE_CORE_FR)

/** Framework manages the interrupt nest count. */
#define kiSysInterruptNestLevel  frIrqInterruptNestLevel

/* Make Framework nest count available without including frirq.h
 *  which would require a Framework platform to also be configured.
 */
extern Int32 frIrqInterruptNestLevel;

#else

/** GKI manages the interrupt nest count */
extern Int32 kiSysInterruptNestLevel;

#endif /* UPGRADE_CORE_FR */

/***************************************************************************
 * Kernel Function redirections
 **************************************************************************/

/*
**
** TimEInMS :- How long to suspend current task.
**
** Suspend the indicated task for a period of time.
**
** NOTE: Minimum delay depends on KiTick lenght. If delay is shorter than KiTick there will be trouble.
*/
#define KiTaskDelay(TimEInMS) M_KiOsTaskDelay(TimEInMS)


/** Return the calling GKI task id.
 * This function returns the GKI TaskId of the current task. If called from
 * an interrupt routine it will return UNKNOWN_TASK_ID.
 *
 * \return The GKI task id for the caller or UNKNOWN_TASK_ID if not a
 *             GKI task. */
#define     KiThisTask()                  KiOsThisTask ()

#if defined(MTK_MINI_DUMP_ENABLE)
/** Get gki task id and exception id
* This function allows to replace KiThisTask() to obtain current task id and exception id during
* the signal send or receive and memory allocate.
* Different from KiThisTask() function, when an interrupt try to create a signal or allocate a memory,
* signal structure's member header.genHeader.debugInfo.taskId can not record the interrupt id and mark
* this value as 0xFFFF (unknown task id). 
* This function can be used in interrupt context to record the interrupt actions, such as create or send signal
* and allocate memory.
 */
#define     KiThisThread()                KiOsThisThread()
#else
#define     KiThisThread()                KiOsThisTask()
#endif/*MTK_MINI_DUMP_ENABLE*/


#ifdef MTK_NBIOT_TARGET_BUILD
/** Disable all interrupts.
 * This function disables all interrupts. This should be followed by a call
 * to #KiEnableInterrupts(). The amount of time spent within a disable/enable
 * interrupts pair should be as small as possible to prevent unacceptable
 * delay in response to interrupts. Calls to KiDisableInterrupts()
 * can be nested as long as the same number of calls to #KiEnableInterrupts()
 * are made. */
#define     KiDisableInterrupts()                                               \
            {                                                                   \
                uint32_t gki_mask=0;                                            \
                hal_nvic_save_and_set_interrupt_mask (&gki_mask);

/** Enable all interrupts.
 * This function enables all interrupts if this is not a nested call. This
 * must be paired to an earlier call to #KiDisableInterrupts(). The amount
 * of time spent within a disable/enable interrupts pair should be as small
 * as possible to prevent unacceptable delay in response to interrupts. */
#define     KiEnableInterrupts()                                                \
                hal_nvic_restore_interrupt_mask (gki_mask);                     \
            }
#else
#define     KiDisableInterrupts()
#define     KiEnableInterrupts()
#endif

#ifndef WIN32
#define    IS_SYS_INTERRUPT  ((portNVIC_INT_CTRL_REG & 0xFFUL) != 0)
#else
#define    IS_SYS_INTERRUPT  kiSysInterruptNestLevel
#endif

/***************************************************************************
 * Function prototypes
 **************************************************************************/

TaskId     KiOsThisTask            (void);

uint32_t   KiOsThisThread        (void);

void       KiOsDisableInterrupts   (void);

void       KiOsEnableInterrupts    (void);


#if defined (__cplusplus)
}
#endif

/** @} */ /* End of PrdGki group */

#endif

/* END OF FILE */
