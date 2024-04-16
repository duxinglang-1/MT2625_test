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
 * GKI header file that defines the signal structures used in the SYS
 * signal base.
 **************************************************************************/

#ifndef KI_SIG_H
#define KI_SIG_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include  <system.h>
#include  <kitqid.h>
#include  <ki_typ.h>

/***************************************************************************
 * Defines
 **************************************************************************/

/** \ingroup SigSysMisc
 * TestFileOut string length.
 * This gives the maximum number of characters that can be contained in the
 * #TestFileOut signal.
 */
#define     MAX_TEST_FILE_OUT_STRING     255

/** \ingroup SigSysStatistics
 * Maximum number of interrupts.
 * This define gives the number of interrupts that will listed in the
 * #KiStatisticsInd signal.
 */
#if !defined (KI_STAT_MAX_NUM_INTERRUPTS)
# define KI_STAT_MAX_NUM_INTERRUPTS      3
#endif

/** \ingroup SigSysLogging
 * Old signal mapping.
 * This defines the old target out of logging memory signal id
 * (\a SIG_OUT_OF_MEMORY) to be the same as the new out of logging memory signal
 * (\a SIG_KI_LOGGING_MEM_FULL_IND). The old signal did not have a
 * signal body whereas the new signal does (#KiLoggingMemFullInd), this should
 * not cause anything recieving this signal a problem and the GKI is the only
 * thing that creates this signal. This is primarily provided to allow PassThru
 * tasks to work without being changed (although they should be changed
 * eventually). */
#define SIG_OUT_OF_MEMORY                SIG_KI_LOGGING_MEM_FULL_IND

/***************************************************************************
 * Kernel Signals
 **************************************************************************/

/** \defgroup SigSys SYS Signaling Interface
 * \ingroup Sig
 * This section defines the SYS GKI signalling interface.
 *
 * This interface contains GKI signals and common system wide signals.
 * @{
 */

/** \defgroup SigSysMisc General system signals
 * These signals are general system utility signals and can be used
 * anywhere in the system.
 * @{
 */

/** Trigger a task to run.
 * Some tasks in the system may wait for this signal to be received
 * before entering their normal GKI signalling loop. Tasks that
 * require complex initialisation or send signals to other tasks
 * should use this mechanism. There needs to be a controlling task
 * in the system that will send these signals in the correct order
 * at start up time. */
typedef struct KiInitialiseTaskTag
{
    /** Startup reason.
     * This indicates to the task why the system was started. It is up to
     * the controlling task as to whether the initial cause is propagated
     * to all other tasks. */
    ControlResetCause cause;
}
KiInitialiseTask;

/** Send string information (printf).
 * This signal is used to transport string information. This is most commonly
 * used by the GKI implementation of printf. */
typedef struct TestFileOutTag
{
    /** String characters.
     * This gives the characters of the string and this must include the
     * terminating NULL character. 
     * \assoc STRING */
    char string[MAX_TEST_FILE_OUT_STRING];
}
TestFileOut;

/** Provides GPS location information.
 * This signal is used to provide GPS location information in GKI logs. */
typedef struct KiGpsPositionIndTag
{
    /** Latitude. 
     * Latitude in degrees (not radians).*/
    SignedInt32 latitude;

    /** Longitude. 
     * Longitude in degrees (not radians).*/
    SignedInt32 longitude;

    /** Altitude. 
     * Altitude centimiters.*/
    SignedInt32 altitude;

    /** Visible satelites. 
     * Number of satelites used to obtain the fix.*/
    Int16 satelitesNo;

    /** Fix quality. 
     * 0 - no fix, 1 - GPS fix, 2 - DGPS fix, 3 - PPSFix, 4 - RealTimeKinematic, 5 - FloatRTK, 6 - DeadReckoning*/
    Int8 fixQuality;

    /** Horizontal Dilution of Precision
     * Relative accuracy of horizontal position. 
     * As GKI has no floats value here is x10 what was received from GPS */
    Int16 hdop;

    /** Time Stamp.
     * Hour part (local). */
    Int8  tsHour;

    /** Time Stamp.
     * Minute part (local). */
    Int8  tsMinute;

    /** Time Stamp.
     * Second part (local). */
    Int8  tsSecond;
}
KiGpsPositionInd;

/** State change monitoring.
 * This signal is used to log a change in a tasks internal state. Use of this
 * signal should normally only be enabled under conditional compile e.g.
 * ENABLE_MY_TASK_STATE_MONITOR to prevent these signals being generated when
 * not needed. */
typedef struct KiStateChangeIndTag
{
    /** Source task.
     * This gives the GKI task id of the task whose state is being logged. */
    TaskId task;

    /** New state.
     * This gives the new state of the task. */
    Int16  newState;

    /** Old state.
     * This gives the previous state of the task. */
    Int16  oldState;

    /** Other data.
     * This allows the task to provide some task specific data. */
    Int32  other;

} TstStateChangeInd, KiStateChangeInd;

/** @} */ /* End of SigSysMisc group */

/** \defgroup SigSysTimer GKI Timer signals
 * \ingroup PrdGki
 * These signals are used by the GKI timer system.
 *
 * Only the #KiTimerExpiry signal is used outside the GKI all other signals are
 * internal to the GKI.
 * @{
 */

/** Timer Expiry.
 * This signal is sent when a GKI timer expiry occurs. The signal is sent
 * to the task set in the myTaskId field of the #KiTimer structure passed
 * in the call to #KiStartTimer, #KiStartExactTimer or #KiStartCoarseTimer. */
typedef struct KiTimerExpiryTag
{
    /** Timer reference number.
     * This is the timer id of the timer that has expired. The receiving task
     * should check that this matches the timer id in the locally stored
     * #KiTimer structure for this timer. If it doesn't match then this is
     * an expiry for a previous instance of the timer. */
    KiTimerId timerId;

    /** Task specific data.
     * This is a copy of the task specific data passed in the #KiTimer
     * structure when starting the timer. This is normally used to identify
     * between different timers within the same task. Normally a task will
     * switch on this value before checking the timerId field so that it
     * knows which local #KiTimer structure to compare the id with. */
    KiTimerUserValue userValue;
}
KiTimerExpiry;

/** Internal start timer signal.
 * This signal is used internally within the GKI to inform the Timer Task
 * that a new timer has been started. This signal is sent as a result of
 * calling one of the #KiStartTimer, #KiStartExactTimer or #KiStartCoarseTimer
 * functions. This signal is cannot be logged on target but it can be
 * logged for tasks running on the PC. The structure changes when compiled
 * on PC to provide more useful debug information. */
typedef struct KiTimerStartTag
{
#if defined(ON_PC) || defined (GX2)
    /** Timer reference number.
     * This field contains a reference number allocated by the GKI which
     * uniquely identifies the instance of the timer. If the timer is stopped
     * and then started again, the new timerId value will be different. The
     * value is used to filter rogue timeout signals belonging to a previous
     * instance of the timer. The task receiving a timeout signal must compare
     * the timerId value in the timeout signal with the timerId value within
     * the local KiTimer structure. If they are different, the timeout signal
     * belongs to a previous instance of the timer that has already been
     * stopped. Only present when compiled on PC. */
    TimerId timerId;

    /** Timer duration.
     * This field defines the duration, in kernel ticks, for which the timer
     * is required to run. The GKI macros #SECONDS_TO_TICKS() and
     * #MILLISECONDS_TO_TICKS() convert between seconds or milliseconds and
     * number of kernel ticks. The macros will round up to the nearest tick.
     * The smallest unit of time is one system tick. For a system based on the
     * GSM frame rate this is 4.615ms (the GSM TDMA frame period). Only
     * present when compiled on PC. */
    FrameTicks timeoutPeriod;

    /** Task specific data.
     * This field contains a reference number to allow a task to identify a
     * timer when using multiple timers. The GKI makes no use of this variable
     * except to include it in the timer expiry signal. Only present when
     * compiled on PC. */
    KiTimerUserValue userValue;

#endif /* ON_PC || GX2 */
#if !defined (ON_PC)
    /** Internal timer index.
     * Index into the internal GKI timer array that holds information on the
     * timer to start. */
    KiTimerIndex timerIndex;
#endif /* !ON_PC */
}
KiTimerStart;

/** Internal stop timer signal.
 * This signal is used internally within the GKI to inform the Timer Task
 * that a timer should be stopped. This signal is sent as a result of
 * calling the #KiStopTimer function. This signal is cannot be logged on
 * target but it can be logged for tasks running on the PC. The structure
 * changes when compiled on PC to provide more useful debug information. */
typedef struct KiTimerStopTag
{
    /** Timer reference number.
     * This field contains a reference number allocated by the GKI which
     * uniquely identifies the instance of the timer. This identifies which
     * timer should be stopped. */
    KiTimerId timerId;

#if defined(ON_PC)
    /** Stopping task id.
     * This field indicates the task id of the task calling #KiStopTimer.
     * This field is only present when compiling on PC.*/
    TaskId taskId;
#endif
}
KiTimerStop;

/** @} */ /* End of SigSysTimer group */


/** \defgroup SigSysAssert GKI Warning and Assert signals
 * \ingroup PrdGki
 * These signals can be used by the GKI warning and assert system.
 *
 * Signals for asserts are enabled by switching on the #KI_USE_ASSERT_SIGNALS
 * conditional compile. When a #DevFail occurs a #KiDevFailInd is sent, for
 * #DevAssert a #KiDevAssertInd is sent and for #DevCheck and #DevParam a
 * #KiDevCheckInd is sent. This will only work succesfully if the asserts
 * being logged are non-fatal as the signalling takes a finite amount of
 * time to get transferred to the host by the comms system.
 *
 * Warnings default to sending signals. When a #WarnFail occurs a
 * #KiWarnFailInd is sent, for #WarnAssert a #KiWarnAssertInd is sent,
 * for #WarnCheck a #KiWarnCheckInd is sent and for #WarnParam a
 * #KiWarnParamInd is sent.
 * @{
 */

/** Maximum Assert Text Length.
 * This defines the maximum amount of assert text that will be contained in
 * a #KiDevAssertInd, #KiDevFailInd or #KiDevCheckInd signal. */
#define KI_MAX_ASSERT_TEXT          (20)

/** Maximum File Name Length.
 * This defines the maximum length of the file name that will be contained in
 * a #KiDevAssertInd, #KiDevFailInd or #KiDevCheckInd signal. This should be
 * sufficient as long as the MODULE_NAME define is present in the source file. */
#define KI_MAX_ASSERT_FILE_NAME     (20)

/** DevAssert, DevFail, WarnAssert, WarnFail signal.
 * The #KiDevAssertInd and #KiDevFailInd signals are used to transport the
 * information from a DevAssert() or DevFail() that occurs when the
 * #KI_USE_ASSERT_SIGNALS conditional compile is enabled. */
typedef struct KiDevAssertIndTag
{
    /** The GKI tick time at which the assert occurred. */
    KernelTicks kernelTime;
    /** The name of the file in which the assert occurred. 
\assoc STRING   */
    Int8        file[KI_MAX_ASSERT_FILE_NAME];
    /** The line number within the file at which the assert occurred. */
    Int16       line;
    /** The text of the assert. This may be empty if SHORT_DEV_ASSERTS is
     * defined in the build. 
\assoc STRING   */
    Int8        text[KI_MAX_ASSERT_TEXT];

} KiDevAssertInd, KiDevFailInd, KiWarnAssertInd, KiWarnFailInd;

/** DevCheck, DevParam, WarnCheck, WarnParam signal.
 * The #KiDevCheckInd signal is used to transport the
 * information from a DevCheck() or DevParam() that occurs when the
 * #KI_USE_ASSERT_SIGNALS conditional compile is enabled. */
typedef struct KiDevCheckIndTag
{
    /** The GKI tick time at which the assert occurred. */
    KernelTicks kernelTime;
    /** The name of the file in which the assert occurred. 
      * \assoc STRING */
    Int8        file[KI_MAX_ASSERT_FILE_NAME];
    /** The line number within the file at which the assert occurred. */
    Int16       line;
    /** The text of the assert. 
      * This may be empty if SHORT_DEV_ASSERTS is defined in the build. 
      * \assoc STRING */
    Int8        text[KI_MAX_ASSERT_TEXT];
    /** The first parameter of the assert. */
    Int32       param1;
    /** The second parameter of the assert. */
    Int32       param2;
    /** The third parameter of the assert. */
    Int32       param3;

} KiDevCheckInd, KiWarnCheckInd, KiWarnParamInd;

/** Configure warnings in the target.
 * This signal is used to set any run-time configuration options
 * for warnings (#WarnCheck, #WarnParam, #WarnAssert, #WarnFail) in
 * the target. */
typedef struct KiWarnConfigReqTag
{
    /** Set the kiFailAssertOnWarn boolean in the target.
     * This field sets the kiFailAssertOnWarn variable in the target.
     * This controls whether when a warning is hit the system asserts
     * or send a KI_WARN_*_IND signal. */
    Boolean assertOnWarn;

} KiWarnConfigReq;

/** @} */ /* End of SigSysAssert group */

/** \defgroup SigSysStatistics GKI Statistics signals
 * \ingroup PrdGki
 * These signals are used to get GKI system statistics.
 * @{
 */

/** General Statistics Request.
 * The KiFixStatisticsReq signal is used to request a #KiFixStatisticsInd signal to
 * be created, filled in and sent to the indicated task.
 *
 * This should only be used by off target processes to request statistics,
 * on target processes should use the #M_KiSendFixStatisticsInd() macro. This
 * signal should be sent to the #KI_COMMS_TASK_ID. If the result needs to
 * be seen off target the returnTask should also be set to #KI_COMMS_TASK_ID.
 * The #KI_STATISTICS conditional compile needs to be enabled in the target
 * for this functionality to be available. */
typedef struct KiFixStatisticsReqTag
{
    /** Return task id. This is the GKI task id of the task to send the
     * resulting #KiStatisticsInd to. */
    TaskId returnTask;
} KiFixStatisticsReq;

/** General Statistics Indication.
 * The KiFixStatisticsInd signal is used to provide some simple statistics data
 * on number of blocks, block size and the amount of kiStackIncreasestack.
 *
 * This signal can be sent in response to a #KiFixStatisticsReq or a call to
 * #M_KiSendFixStatisticsInd() on the target. */
typedef struct KiFixStatisticsIndTag
{
    /** A flag to indicate if we use memory pools or not. */
    Boolean isFixMemoryPool;

    /** Number of memory pools.
    * This gives the size of the peakMemoryAllocated and memoryAllocated arrays.
    * This is useful for PC based tasks that need to decode this signal but do
    * not need to be recompiled every time the memory pool configuration is
    * changed. */
    Int16 poolNumUser;

    /** The number of blocks in this pool. */
    Int16 numBlocks[KI_POOL_NUM_USER];

    /** The size of a single block. */
    Int16 blockSize[KI_POOL_NUM_USER];

    /** Development task stack increase.
     * This is the amount that the task stacks should be increased by when
     * #DEVELOPMENT_STACKS is defined. The default value can also be over
     * ridden in the build file.
     * This is useful to show the real stack size. */
    Int16 kiStackIncrease;

} KiFixStatisticsInd;



/** General Statistics Request.
 * The KiStatisticsReq signal is used to request a #KiStatisticsInd signal to
 * be created, filled in and sent to the indicated task.
 *
 * This should only be used by off target processes to request statistics,
 * on target processes should use the #M_KiSendStatisticsInd() macro. This
 * signal should be sent to the #KI_COMMS_TASK_ID. If the result needs to
 * be seen off target the returnTask should also be set to #KI_COMMS_TASK_ID.
 * The #KI_STATISTICS conditional compile needs to be enabled in the target
 * for this functionality to be available. */
typedef struct KiStatisticsReqTag
{
    /** Return task id. This is the GKI task id of the task to send the
     * resulting #KiStatisticsInd to. */
    TaskId returnTask;

} KiStatisticsReq;

/** General Statistics Indication.
 * The KiStatisticsInd signal is used to provide some simple statistics data
 * on stack usage, number of timers and memory allocated.
 *
 * This signal can be sent in response to a #KiStatisticsReq or a call to
 * #M_KiSendStatisticsInd() on the target. */
typedef struct KiStatisticsIndTag
{


    /** Development task stack increase.
     * This is the amount of stack that has been added to each task stack
     * for development purposes (when #DEVELOPMENT_STACKS is defined). */
    Int16 taskStackIncrease;

    /** Number of real tasks.
     * This gives the size of the taskIds, minimumFreeTaskStackSpace and totalTaskStackSpace arrays.
     * This is useful for PC based tasks that need to decode this signal but
     * do not need to be recompiled every time the task configuration is
     * changed.
    * \assoc ARRAY \ref taskIds
    * \assoc ARRAY \ref totalTaskStackSpace
    * \assoc ARRAY \ref minimumFreeTaskStackSpace
    */
    Int16 numOfRealTasks;

    /** Task Ids.
     * This gives the GKI task id for the associated data with the same index
     * in the minimumFreeTaskStackSpace member. */
    TaskId taskIds[KI_NUM_OF_REAL_TASKS];

    Int16  totalTaskStackSpace[KI_NUM_OF_REAL_TASKS];

    /** Minimum free task stack space.
     * This array provides a list of the amount of stack currently left on each
     * tasks stack. If an entry is 0 the stack has overrun for the corresponding
     * task. The corresponding task id is given by the same index in the taskIds
     * array member. */
    Int16 minimumFreeTaskStackSpace[KI_NUM_OF_REAL_TASKS];

    /** Development interrupt stack increase.
     * This is the amount of stack that has been added to each interrupt stack
     * for development purposes (when #DEVELOPMENT_STACKS is defined). */
    Int16 interruptStackIncrease;

    /** Number of interrupts.
     *  This gives the size of the minimumFreeIntStackSpace array. This is
     * useful for PC based tasks that need to decode this signal but do not
     * need to be recompiled every time the interrupt stack configuration is
     * changed. 
    * \assoc ARRAY \ref minimumFreeIntStackSpace
    */
    Int16 statMaxNumInterrupts;

    /** Minimum free interrupt stack space.
     * This array provides a list of the amount of stack currently left on
     * each interrupts stack. If an entry is 0 the stack has overrun for the
     * corresponding interrupt. If the entry is 0xFFFF the corresponding
     * interrupt does not exist on the target. */
    Int16 minimumFreeIntStackSpace[KI_STAT_MAX_NUM_INTERRUPTS];

    /** Maximum number of GKI timers.
     * The maximum number of GKI timers active in the system at one time
     * since power on. */
    Int16 maximumNumberOfTimers;

    /** Current number of active GKI timers.
     * The current number of GKI timers active (awaiting expiry) in the system. */
    Int16 numberOfTimersRunning;

    /** Number of memory pools.
     * This gives the size of the peakMemoryAllocated and memoryAllocated arrays.
     * This is useful for PC based tasks that need to decode this signal but do
     * not need to be recompiled every time the memory pool configuration is
     * changed. 
    * \assoc ARRAY \ref peakMemoryAllocated
    * \assoc ARRAY \ref memoryAllocated
    */
    Int16 poolNumUser;

    /** Peak pool block usage.
     * This array gives the peak number of memory blocks used within each
     * memory pool in the system since power on. */
    Int16 peakMemoryAllocated[KI_POOL_NUM_USER];

    /** Current pool block usage.
     * This array gives the current number of memory blocks used within
     * each memory pool.*/
    Int16 memoryAllocated[KI_POOL_NUM_USER];

#if !defined(KI_DISABLE_DYNAMIC_MEMORY)
    /** Peak dynamic memory usage.
     * This field gives the peak amount of memory used in the system since
     * power on. The OS and GKI overhead is not included. */
    Int32 peakDynamicMemoryAllocated;

    /** Peak dynamic block count.
     * This field gives the peak number of dynamic memory blocks used in
     * the system since power on. */
    Int16 peakDynamicMemoryBlockCount;

    /** Current dynamic memory usage.
     * This field gives the current sum of dynamic memory used. The OS
     * and GKI overhead is not included. */
    Int32 dynamicMemoryAllocated;

    /** Current dynamic block count.
     * This field gives the current number of dynamic memory blocks. */
    Int16 dynamicMemoryBlockCount;

    /** Minimum free dynamic memory.
     * This field gives the minimum amount of free dynamic memory there
     * has been since power on. */
    Int32 minimumFreeDynamicMemorySpace;

    /** Peak dynamic non-cacheable memory usage.
     * This field gives the peak amount of memory used in the system since
     * power on. The OS and GKI overhead is not included. */
    Int32 peakDynamicNonCacheableMemoryAllocated;

    /** Peak dynamic non-cacheable block count.
     * This field gives the peak number of dynamic memory blocks used in
     * the system since power on. */
    Int16 peakDynamicNonCacheableMemoryBlockCount;

    /** Current dynamic non-cacheable memory usage.
     * This field gives the current sum of dynamic memory used. The OS
     * and GKI overhead is not included. */
    Int32 dynamicNonCacheableMemoryAllocated;

    /** Current dynamic non-cacheable memory block count.
     * This field gives the current number of dynamic memory blocks. */
    Int16 dynamicNonCacheableMemoryBlockCount;

    /** Minimum free dynamic non-cacheable memory.
     * This field gives the minimum amount of free dynamic memory there
     * has been since power on. */
    Int32 minimumFreeDynamicNonCacheableMemorySpace;
#endif
}
KiStatisticsInd;

/** Task signal queue statistics request
 * The KiQueueStatsReq signal is used to request a #KiQueueStatsInd signal to
 * be created, filled in and sent to the indicated task.
 *
 * The task queue statistics provide and indication of the peak and current
 * number of signals on all the GKI tasks queues.
 *
 * This should only be used by off target processes to request statistics, on
 * target processes should use the #M_KiSendQueueStatsInd() macro. This signal
 * should be sent to the KI_COMMS_TASK_ID. If the result needs to be seen off
 * target the returnTask should also be set to KI_COMMS_TASK_ID.
 *
 * This signal is only available if KI_ENABLE_OWN_QUEUEING is enabled. */
typedef struct KiQueueStatsReqTag
{
    /** Task to send response to.
     * This is the GKI task id of the task to send the resulting #KiQueueStatsInd to.*/
    TaskId returnTask;

} KiQueueStatsReq;

/** Task signal queue statistics indication
 * The KiQueueStatsInd signal is used to provide information on the peak and
 * current number of signals on the GKI task input queues.
 *
 * This signal is sent in response to a call to the #M_KiSendQueueStatsInd()
 * macro on target or a #KiQueueStatsReq being sent in from off target. */
typedef struct KiQueueStatsIndTag
{
    /** Number of real GKI tasks.
     * This gives the size of the taskIds, currentQueueCount and maxQueueCount
     * arrays. This is useful for PC based tasks that need to decode this
     * signal but do not need to be recompiled every time the task configuration
     * is changed. */
    Int16 numOfRealTasks;

    /** Task ids.
     * This gives the GKI task id for the associated data with the same index
     * in the currentQueueCount and maxQueueCount members. */
    TaskId taskIds[KI_NUM_OF_REAL_TASKS];

    /** Current queue signal count.
     * This array provides a list of the number of signals currently on each tasks
     * input queue. The corresponding task id is given by the same index in the
     * taskIds array member. */
    Int32 currentQueueCount[KI_NUM_OF_REAL_TASKS];

    /** Maximum queue signal count.
     * This array provides a list of the maximum number of signals each task has
     * had on its input queue since power on. The corresponding task id is given
     * by the same index in the taskIds array member. */
    Int32 maxQueueCount[KI_NUM_OF_REAL_TASKS];

} KiQueueStatsInd;

/** Maximum sub-pool entries in #KiMemPoolStatisticsCnf.
 * This gives the maximum number of sub-pool statistic entries that the
 * #KiMemPoolStatisticsCnf signal can hold. */
#define KI_MAX_MEM_POOL_STATS_ENTRIES     50

/** Memory Pool statistics request.
 * The KiMemPoolStatisticsReq signal is used to request a
 * #KiMemPoolStatisticsCnf signal to be created, filled in and sent
 * to the indicated task.
 *
 * The memory pool statistics signal gives information about the peak and
 * current usage within sub divisions of the GKI memory pools.
 *
 * This should only be used by off target processes to request statistics, on
 * target processes should use the #M_KiSendMemPoolStatisticsCnf() macro. This
 * signal should be sent to the #KI_COMMS_TASK_ID. If the result needs to be
 * seen off target the returnTask should also be set to #KI_COMMS_TASK_ID.
 * These stastics are only available if #KI_MEM_POOL_STATISTICS is enabled
 * in the target build. */
typedef struct KiMemPoolStatisticsReqTag
{
    /** Task to send response to.
     * This is the GKI task id of the task to send the resulting #KiMemPoolStatisticsCnf to.*/
    TaskId returnTask;

    /** First entry to report on.
     * This indicates which entry from the memory pool statistics information to start
     * reporting from. The #KiMemPoolStatisticsCnf can only report on
     * #KI_MAX_MEM_POOL_STATS_ENTRIES at a time but there may be more than
     * #KI_MAX_MEM_POOL_STATS_ENTRIES available on target. The first
     * KiMemPoolStatisticsReq should have a startEntry of 0, the next should
     * have #KI_MAX_MEM_POOL_STATS_ENTRIES and so on until the numberOfEntries
     * field of the #KiMemPoolStatisticsCnf signal is not equal to
     * #KI_MAX_MEM_POOL_STATS_ENTRIES. */
    Int16 startEntry;

} KiMemPoolStatisticsReq;

/** Memory Pool statistics confirm.
 * The KiMemPoolStatisticsCnf signal is used to provide more in-depth information on
 * memory pool usage.
 *
 * This signal is sent in response to a call to the #M_KiSendMemPoolStatisticsCnf()
 * macro on target or the #KiMemPoolStatisticsReq signal being sent from off target.
 *
 * When the #KI_MEM_POOL_STATISTICS conditional is enabled the GKI will store
 * information about sub pool memory usage. The GKI stores information about
 * allocations within a defined granularity up to a maximum size. The granularity
 * is by default 16 bytes but this can be overridden by setting the #KI_MPS_GRANULARITY_PWR
 * conditional (granularity as a power of 2, e.g. granularity of 16 comes from
 * #KI_MPS_GRANULARITY_PWR being set to 4). The maximum size is by default 4096 bytes
 * but this can be overridden by setting the #KI_MPS_MAX_SIZE conditional. The number
 * of entries that the GKI will store on target is given by the maximum size
 * divided by the granularity.
 *
 * Each entry provides the peak and current number of allocations in the
 * corresponding range and the SignalId of the last allocation (if it was for
 * a signal). */
typedef struct KiMemPoolStatisticsCnfTag
{
    /** Peak pool block usage.
     * This array gives the peak number of memory blocks used within each
     * memory pool in the system since power on. */
    Int16 peakPoolUsage[KI_POOL_NUM_USER];

    /** Current pool block usage.
     * This array gives the current number of memory blocks used within
     * each memory pool.*/
    Int16 currentPoolUsage[KI_POOL_NUM_USER];

    /** Largest allocation request.
     * This gives the largest pool memory allocation request. */
    Int32 largestAlloc;

    /** Maximum pool stats range.
     * This is the limit to which memory pool statistics will be collected.
     * This is controlled by the #KI_MPS_MAX_SIZE define, the default is 4096. */
    Int32 maximumSize;

    /** Pool stats granularity.
     * This is the granularity of the memory information. This is controlled
     * by the #KI_MPS_GRANULARITY_PWR define, the default is 16. The total
     * number of entries that the GKI will maintain is given by the
     * maximumSize/granularity. */
    Int32 granularity;

    /** Start entry number.
     * This is the index within the sub pool statistics list on the target for
     * the first entry in the following entries array. This will correspond to
     * the requested first entry (from the #KiMemPoolStatisticsReq signal or the
     * #M_KiSendMemPoolStatisticsCnf() macro). */
    Int16 startEntry;

    /** Number of entries in this signal.
     * This gives the number of valid entries in the following entries array.
     * If this number is less than #KI_MAX_MEM_POOL_STATS_ENTRIES then there
     * are no more entries available from the target. 
     * \assoc ARRAY \ref entries */
    Int16 numberOfEntries;

    /** Sub-pool statistics entries.
     * This is a list of numOfEntries #KiMemSubPoolStatsEntry structures taken
     * from the target GKI list starting from startEntry. */
    KiMemSubPoolStatsEntry entries[KI_MAX_MEM_POOL_STATS_ENTRIES];

} KiMemPoolStatisticsCnf;

/** Dynamic memory usage request.
 * This signal is used to report dynamic memory usage (not pool memory usage). */
typedef struct KiDynamicMemUsageReqTag
{
    /** Task to send response to.
     * This is the GKI task id of the task to send the resulting #KiDynamicMemUsageCnf to.*/
    TaskId returnTask;

    /** Task specific details.
     * Report more detailed dynamic memory usage and caller information for this task.
     * This field is only used when #KI_MEMROY_DEBUG or #KI_MEMORY_USAGE is defined. */
    TaskId queryTask;

} KiDynamicMemUsageReq;

/** Maximum number of dynamic memory caller addresses.
 * This define limits the number of caller addresses that can be reported in a
 * #KiDynamicMemUsageCnf signal. */
#if !defined (KI_REPORT_CALLER_ADDRESSES)
/* The constant can be defined in the build file.. */
#define KI_REPORT_CALLER_ADDRESSES      32
#endif

/** Number of task specific entries in #KiDynamicMemUsageCnf
 * This defines the number of entries in the arrays used to report task
 * dynamic memory usage in the #KiDynamicMemUsageCnf signal. The value
 * is KI_NUM_OF_REAL_TASKS + 3. This is so there is extra storage in error
 * cases. Index KI_NUM_OF_REAL_TASKS is used for task id's that are not real
 * tasks (reserved tasks), KI_NUM_OF_REAL_TASKS + 1 is used for UNKNOWN_TASK_ID
 * (interrupts) and KI_NUM_OF_REAL_TASKS + 2 is used for tasks id's greater than
 * KI_MAX_TASK_ID. These entries should not be needed, but may help in debugging
 * if  something serious has gone wrong. */
#define KI_NUM_DYN_MEM_TASKS_IN_SIG     (KI_NUM_OF_REAL_TASKS + 3)

/** Memory Report Type.
 * This indicates the Report Type #CACHEABLE_REPORT or #NON_CACHEABLE_REPORT.
 * Report Type: Cacheable or nonCacheable memory report. */
typedef enum MemReportTypeTag
{
    CACHEABLE_REPORT,
    NON_CACHEABLE_REPORT
} MemReportType;

/** Dynamic memory usage confirm.
 * This signal is used to provide information on general dynamic memory usage as
 * well as some task specific dynamic memory usage. */
typedef struct KiDynamicMemUsageCnfTag
{
    /** Report valid.
     * This indicates if the rest of the signal is valid or not.
     * The report is generated by walking through the dynamic memory pool. If
     * the integrity of the dynamic memory pool is modified because of invalid
     * access, this field will be FALSE. If other higher priority task requests
     * dynamic memory during generation of this report, this field may be set
     * to FALSE. If this is FALSE the rest of the signal should be ignored.
     * If FALSE is returned issue the #KiDynamicMemUsageReq again. */
    Boolean validReport;

    /** Free dynamic memory.
     * This gives the current amount of dynamic memory that is free. */
    Int32   freeDynamicMemory;

    /** Free memory block count.
     * This is a field indicating fragmentation status of dynamic memory. The more blocks
     * that free memory is made up of the higher the fragmentation. The higher the
     * fragmentation the less likely that a large memory request can be successfully
     * completed (even if it is less than the available free memory). */
    Int16 freeDynamicMemoryBlockCount;

    /** Largest free block.
     * This gives the size of the largest contiguous free block of dynamic memory. */
    Int32 maxFreeBlockSize;

    /** Total dynamic memory allocated.
     * This gives the total dynamic memory currently allocated including any
     * GKI and OS overheads. This also includes requested memory. */
    Int32 totalDynamicMemoryAllocated;

    /** Total dynamic blocks allocated.
     * This gives the total number of dynamic memory blocks allocated. */
    Int16 totalDynamicMemoryBlockCount;

    /** Total requested dynamic memory allocated.
     * This gives the total dynamic memory currently allocated at the request
     * priority (KiRequestSignal(), KiRequestZeroSignal() & KiRequestMemory()
     * including any GKI and OS overheads. */
    Int32 requestDynamicMemoryAllocated;

    /** Total requested dynamic blocks allocated.
     * This gives the total number of dynamic memory blocks allocated at the
     * request priority. */
    Int16 requestDynamicMemoryBlockCount;

    /** Number of valid task entries.
     * This gives the number of valid entries in the following
     * taskDynamicMemoryId, taskDynamicMemoryAllocated and
     * taskDynamicMemoryBlockCount arrays. 
     * \assoc ARRAY \ref taskDynamicMemoryId
     * \assoc ARRAY \ref taskDynamicMemoryAllocated
     * \assoc ARRAY \ref taskDynamicMemoryBlockCount
     */
    Int16 taskDynamicMemoryNumEntries;

    /** Task entry GKI task id.
     * This gives the GKI TaskId for the corresponding entry in the
     * taskDynamicMemoryAllocated and taskDynamicMemoryBlockCount arrays. */
    TaskId taskDynamicMemoryId[KI_NUM_DYN_MEM_TASKS_IN_SIG];

    /** Task dynamic memory allocated.
     * This gives the current allocated dynamic memory including OS and GKI
     * overheads for the corresponding task. */
    Int32 taskDynamicMemoryAllocated[KI_NUM_DYN_MEM_TASKS_IN_SIG];

    /** Task dynamic memory block count.
     * This gives the current allocated dynamic memory block count for the
     * corresponding task. */
    Int16 taskDynamicMemoryBlockCount[KI_NUM_DYN_MEM_TASKS_IN_SIG];

    /** Number of valid caller entries.
     * This gives the number of valid entries in the following callerAddress,
     * callerDynamicMemoryAllocated and callerDynamicMemoryBlockCount arrays.
     * These arrays give information specific to the queryTask given in the
     * #KiDynamicMemUsageReq. 
     * \assoc ARRAY \ref callerAddresses 
     * \assoc ARRAY \ref callerDynamicMemoryAllocated 
     * \assoc ARRAY \ref callerDynamicMemoryBlockCount 
     */
    Int16 callerAddressesNumEntries;

    /** Caller addresses for the corresponding allocations.
     * Only the first 32 callerAddress (defined by #KI_REPORT_CALLER_ADDRESS) will
     * be reported. This gives the address of the code that calls a GKI function
     * that results in the allocation of dynamic memory. If these addresses are
     * all 0 then the #M_KiGetCallerAddress macro has not be configured for
     * this processor/toolset. */
    Int32 callerAddresses[KI_REPORT_CALLER_ADDRESSES];

    /** Caller dynamic memory allocated.
     * This gives the amount of dynamic memory currently allocated via the
     * corresponding caller. */
    Int32 callerDynamicMemoryAllocated[KI_REPORT_CALLER_ADDRESSES];

    /** Caller dynamic memory block count.
     * This gives the number of dynamic memory blocks currently allocated via
     * the corresponding caller. */
    Int16 callerDynamicMemoryBlockCount[KI_REPORT_CALLER_ADDRESSES];

    /** Amount of memory lost to overheads.
     * This is the amount of memory currently used in overheads to maintain
     * the memory pool. This includes the main pool information block plus
     * headers on individual blocks. */
    Int32 memoryPoolOverhead;

    /** Task id for caller addresses.
     * This is the GKI task id of the caller addresses. This equates to the
     * queryTask given in the corresponding #KiDynamicMemUsageReq. */
    KiTaskId queryTask;
    /** Memory Type for Report.
     * This is used to indicate the memory type. */
    MemReportType reportType;

} KiDynamicMemUsageCnf;

/** @} */ /* End of SigSysStatistics group */

/** \defgroup SigSysMemory GKI Memory access signals
 * \ingroup PrdGki
 * These signals are used to read and write target memory.
 * @{
 */

/** Maximum memory read/write in one signal.
 * This gives the maximum number of bytes that can be read or written
 * by the memory access signals. */
#define  KI_MAX_MEM_DUMP_BLOCK     256

/** Memory dump request.
 * The KiDumpMemReq signal can be used to request the GKI to dump the
 * given amount of memory from the given start address into a
 * #KiDumpMemCnf signal and send it to the return task. This signal
 * can only be sent from off target, the destination address does not
 * matter. To generate a #KiDumpMemCnf signal from target code use the
 * #M_KiSendDumpMemReq() macro. */
typedef struct KiDumpMemReqTag
{
    /** The target address to start the read from. */
    Int32 startAddress;

    /** The number of byte to read.
     * This can be from 1 to KI_MAX_MEM_DUMP_BLOCK bytes. If more data
     * is required issue another request once this one is complete. */
    Int16 count;

    /** Task to send response to.
     * This is the GKI task id of the task to send the resulting #KiDumpMemCnf
     * to. Setting this to KI_COMMS_TASK_ID will cause the signal to be sent
     * to the host whether or not this signal is in the filter. */
    TaskId returnTask;

} KiDumpMemReq;

/** Memory dump confirm.
 * This signal is used to log the contents of a section of target memory.
 * This signal is sent in response to a #KiDumpMemReq signal sent from
 * off target or a call to the #M_KiSendDumpMemReq() macro on target. */
typedef struct KiDumpMemCnfTag
{
    /** The target address of the first byte in the dump array. */
    Int32 startAddress;

    /** The number of valid bytes in the dump array. 
      * \assoc ARRAY \ref dump */
    Int16 count;

    /** The array of 'count' bytes read from the target memory from
     * 'startAddress'. */
    Int8  dump[KI_MAX_MEM_DUMP_BLOCK];

} KiDumpMemCnf;

#if defined (KI_ASSERT_TESTING)
/** KiAssertTestReq assert testing request signal
 * The #KiAssertTestReq signal is used to request the triggering
 * of a specific assert type when the #KI_ASSERT_TESTING conditional 
 * compile is enabled. */
typedef struct KiAssertTestReqTag
{
    /** Id for the type of assert to test */
    AssertTestingType        assertType;
    /** Test name of the file in which the assert occurred. 
     * only used when assert type specifically uses given 
     * values, others generate file/line automatically.
     * \assoc STRING
     */
    Int8        file[KI_MAX_ASSERT_FILE_NAME];
    /** Test line number within the file at which the assert occurred. 
     * only used when assert type specifically uses given 
     * values, others generate file/line automatically.
     */
    Int16       line;
    /** The text of the assert.
     * \assoc STRING
     */
    Int8        text[KI_MAX_ASSERT_TEXT];
    /** Boolean to indicate condition value */
    Boolean conditionValue;
    /** Value 1 */
    Int32       v1;
    /** Value 2 */
    Int32       v2;
    /** Value 3 */
    Int32       v3;
} KiAssertTestReq;
#endif /* KI_ASSERT_TESTING */

#if defined (UPGRADE_COMET)

typedef enum MemAccessTag
{
    MEM_BYTE_ACCESS,
    MEM_WORD_ACCESS,
    MEM_LONG_WORD_ACCESS

} MemAccess;

typedef union DataDumpTag
{
    Int8  dump8[KI_MAX_MEM_DUMP_BLOCK];
    Int16 dump16[KI_MAX_MEM_DUMP_BLOCK / sizeof(Int16)];
    Int32 dump32[KI_MAX_MEM_DUMP_BLOCK / sizeof(Int32)];

} DataDump;

typedef union DataDumpRTag
{
    Int8  dumpR8[KI_MAX_MEM_DUMP_BLOCK];
    Int16 dumpR16[KI_MAX_MEM_DUMP_BLOCK / sizeof(Int16)];
    Int32 dumpR32[KI_MAX_MEM_DUMP_BLOCK / sizeof(Int32)];

} DataDumpR;    /* to help Tvass.ass work */

typedef union DataAccessTag
{
    Int8  access8;
    Int16 access16;
    Int32 access32;

} DataAccess;

typedef union DataItemTag
{
    Int8  data8;
    Int16 data16;
    Int32 data32;

} DataItem;

typedef struct WriteRecTag
{
    Int32    address;
    DataItem data;

} WriteRec;

typedef struct KiMemReadReqTag
{
    Int32     startAddress;
    Int16     count;
    TaskId    returnTask;
    MemAccess access;

} KiMemReadReq;

typedef struct KiMemReadCnfTag
{
    Int32     startAddress;
    Int16     count;
    MemAccess access;
    DataDump  dump;

} KiMemReadCnf;

typedef struct KiMemWriteReqTag
{
    Int32     startAddress;
    Int16     count;
    TaskId    returnTask;
    MemAccess access;
    DataDump  dump;

} KiMemWriteReq;

typedef struct KiMemWriteCnfTag
{
    Int32     startAddress;
    Int16     count;
    MemAccess access;

} KiMemWriteCnf;

typedef struct KiMemReadMultiReqTag
{
    TaskId    returnTask;
    Int16     numReads;
    MemAccess access;
    Int32     address[KI_MAX_MEM_DUMP_BLOCK / sizeof(Int32)];

} KiMemReadMultiReq;

typedef struct KiMemReadMultiCnfTag
{
    Int16     numReads;
    MemAccess access;
    DataDumpR dumpR;

} KiMemReadMultiCnf;

typedef struct KiMemWriteMultiReqTag
{
    TaskId    returnTask;
    Int16     numWrites;
    MemAccess access;
    WriteRec  writeRec[KI_MAX_MEM_DUMP_BLOCK / sizeof(Int32)];

} KiMemWriteMultiReq;

typedef struct KiMemWriteMultiCnfTag
{
    Int16     numWrites;
    MemAccess access;

} KiMemWriteMultiCnf;
#else  /* UPGRADE_COMET */
typedef SignalNotYetDefined KiMemReadReq;
typedef SignalNotYetDefined KiMemReadCnf;
typedef SignalNotYetDefined KiMemWriteReq;
typedef SignalNotYetDefined KiMemWriteCnf;
typedef SignalNotYetDefined KiMemReadMultiReq;
typedef SignalNotYetDefined KiMemReadMultiCnf;
typedef SignalNotYetDefined KiMemWriteMultiReq;
typedef SignalNotYetDefined KiMemWriteMultiCnf;
#endif /* !UPGRADE_COMET */

/** @} */ /* End of SigSysMemory group */

/** \defgroup SigSysLogging GKI Logging signals
 * \ingroup PrdGki
 * These signals are used to control logging and provide information about
 * logging.
 *
 * There are filter matrix signals used to inform the target of what
 * signals to log:
 *
 * #NormalFilterMatrix,
 * #CompactFilterMatrix,
 * #TinyFilterData,
 * #KiSetsFilterMatrix
 *
 * There are signals that indicate when logging memory becomes full and
 * then available again:
 *
 * #KiLoggingMemFullInd,
 * #KiLoggingMemFreeInd
 *
 * @{
 */

/** Maximum signals per base for Normal and Compact matrices.
 * This define controls the number of signals in a base that can be controlled
 * by the #NormalFilterMatrix and #CompactFilterMatrix filter matrices.
 *
 * Both the #NormalFilterMatrix and #CompactFilterMatrix filter matrices are
 * redundant and should not be used. Therefore this value is also redundant. */
#define     MAX_NUM_SIGS_IN_BASE     100

/** Sets Filter Matrix.
 * The KiSetsFilterMatrix signal is used to send the signal logging filter
 * into the system.
 *
 * This type of filter is needed when signal sets are
 * being used. The actual size of this signal when sent may be different
 * from the compile time structure as the matrix array will change size
 * depending on the number of signal sets, bases and id's defined in the
 * system.
 *
 * \image html KiSetFilterFormat.png "Set Filter Format"
 * \image latex KiSetFilterFormat.eps "Set Filter Format" width=6cm
 *
 * The Sets filter is like an array of 2 dimensional arrays, one for
 * each signal set. This results in the Sets filter being a 3 dimensional
 * array (but with variable sized dimensions).
 *
 * DO NOT CHANGE THIS STRUCTURE'S NAME OR MEMBERS AS PC TEST TOOLS
 * MAY REFERENCE THESE. */
typedef struct KiSetsFilterMatrixTag
{
    /** The number of signal sets in the matrix array. */
    Int16 numSets;

    /** The number of signal bases in the matrix array. */
    Int32 numBases;

    /** The size in bytes of the matrix array. */
    Int32 matrixSize;

    /** The filter matrix.
     * The size of this array is given by the matrixSize array rather than
     * the statically defined dimension. This array has information on what
     * signals within each set and base should be logged.
     * The matrix array is split into 3 parts. The first part is an array
     * from 0 to (numSets - 1) of 4 byte indexes into the base offsets (the
     * 2nd part), each index represents the first base in the associated set.
     * The second part is an array from numSets to (numSets + numBases - 1)
     * of 4 byte indexes into the signal bitmap (the 3rd part), each index
     * represents the first signal in the associated base. The third part
     * is the signal bitmap, each bit represents the logging state of a
     * signal, the signals within a base always start on an Int8 boundary
     * (i.e. there is padding at the end of bases to get to an Int8 boundary).*/
    Int8 matrix[10];

} KiSetsFilterMatrix;

/** Logging memory full indication.
 * This signal is sent when the GKI logging buffer becomes full.
 *
 * Logging will be stopped until the amount of free logging memory is greater
 * than the restart threshold. The KiLoggingMemFullInd and #KiLoggingMemFreeInd
 * signals mark any discontinuities in a log caused by logging memory running
 * out. */
typedef struct KiLoggingMemFullIndTag
{
    /** Total defined logging memory.
     * This gives the total size of the logging memory buffer in bytes. */
    Int32 totalLoggingMemory;

    /** Amount of logging memory free.
     * This gives the amount of logging memory that is free (in bytes)
     * when the full condition occurs. */
    Int32 loggingMemoryFree;

    /** Number of pending logged signals.
     * This indicates how many signals are currently stored in the logging buffer. */
    Int32 numPendingSignals;

    /** Logging restart threshold.
     * This gives the amount of logging memory (in bytes) that needs to become
     * free before logging will be resumed. */
    Int32 restartThreshold;

} KiLoggingMemFullInd;

/** Logging memory free indication.
 * This signal is sent when the GKI is about to restart logging because the
 * amount of free logging memory is greater than the restart threshold.
 *
 * This is only sent when a matching #KiLoggingMemFullInd has been sent.
 * The #KiLoggingMemFullInd and KiLoggingMemFreeInd signals mark any
 * discontinuities in a log caused by logging memory running out. */
typedef struct KiLoggingMemFreeIndTag
{
    /** Total defined logging memory.
     * This gives the total size of the logging memory buffer in bytes. */
    Int32 totalLoggingMemory;

    /** Amount of logging memory free.
     * This gives the amount of logging memory that is free (in bytes)
     * when the free condition occurs. */
    Int32 loggingMemoryFree;

    /** Number of pending logged signals.
     * This indicates how many signals are currently stored in the logging buffer. */
    Int32 numPendingSignals;

    /** Logging restart threshold.
     * This gives the amount of logging memory (in bytes) that needs to become
     * free before logging will be resumed. */
    Int32 restartThreshold;

    /** Number of lost signals.
     * This gives the number of signals that were lost during the period of
     * disabled logging. This may not be 100% accurate and is only intended
     * as a guide. */
    Int32 numLostSignals;

} KiLoggingMemFreeInd;

/** @} */ /* End of SigSysLogging group */

/** @} */ /* End of SigSys group */
typedef enum {
    LITE_MODE,
    FULL_MODE
} MipsStatMode;

typedef struct {
    TaskId TaskName;
    Int32  Time_LSB;
    Int32  Time_MSB;
} MipsTaskStatResult;

typedef struct KiMipsStatEnableReqTag
{
    MipsStatMode Mode;
} KiMipsLoggingEnableReq;

typedef struct KiMipsStatDisableReqTag
{
    TaskId ReturnTask;
} KiMipsLoggingDisableReq;

/** DSP loading profiling information. */
typedef struct DspProfileTag
{
    Int32 idleTime_MSB;            /**< Idle time accumulator MSB. */
    Int32 idleTime_LSB;            /**< Idle time accumulator LSB. */
    Int32 loadedTime_MSB;          /**< Accumulator for the DSP being active (loaded) MSB. */
    Int32 loadedTime_LSB;          /**< Accumulator for the DSP being active (loaded) LSB. */
    Int32 totalCall;               /**< Total of Dsp Profiling feature call. */
} DspProfile;

typedef struct KiMipsStatIndTag
{
    MipsStatMode       Mode;
    DspProfile         dspProfile;
    Int32              TotalTime_MSB;
    Int32              TotalTime_LSB;
    Int32              InterruptionTotalTime_MSB;
    Int32              InterruptionTotalTime_LSB;
    Int32              TotalCall;
    MipsTaskStatResult TaskStat[100];
} KiMipsLoggingInd;

typedef struct KiConfigureLoggingTaskReqTag
{
    TaskId             commsTask;
} KiConfigureLoggingTaskReq;



#if defined (UPGRADE_NVRAM_ENHANCED_TEST_ONLY)
/** Sets Nvram Integration Test.
 * The KiNvramItTestReq signal is used to send to the system.
 *
 * This type of Nvarm Integration Test Request signal is used to modify the 
flash and CIS.
 */
typedef struct KiNvramItTestReqTag
{
    Int16 type;  /* 0 for RO, 1 for RW */
    Int16 backup; /* 0 for NVRAM, 1 for CIS */

    /*the offset from the top of NVRAM/CIS paritition*/
    Int32 offset;

    /*the length of data*/
    Int32 length;

    /*new data to overwirte*/
    Int8 data[16];

} KiNvramItTestReq;

/** Sets Nvram Integration Test Confirm.
 * The KiNvramItTestCnf signal is used to confirm the signal Nvarm 
Integration Test
 * Request operate result.
 *
 * This type of Nvarm Integration Test  signal is used to confirm the result 
of modify the flash and CIS.
 */
typedef struct KiNvramItTestCnfTag
{
  /*Modify NVRAM paritition result. If success, result = 1, else result = 0 */
    Int32 result;
} KiNvramItTestCnf;
#endif

#ifdef OPTIMIZED_GKI_LOG_ENABLE
typedef struct KiLogSigEventTag
{
    uint32_t Event;
}KiLogSigEventT;

typedef struct KiLogBuffFullSigTag
{
    uint8_t  ch;
    uint32_t BufferSize;
    uint32_t DiscardCount;
    uint32_t AvaliavleSize;
}KiLogBuffFullSigT;

typedef enum
{
    KI_LOG_EVENT_ALLOC           = 0x0,
    KI_LOG_EVENT_ALLOC_FAIL      = 0x1,
    KI_LOG_EVENT_TRIGGER_INFO    = 0x2,
    KI_LOG_EVENT_PROCESS_CMD     = 0x3,
    KI_LOG_EVENT_FLUSH_START     = 0x4,
    KI_LOG_EVENT_FLUSH_CB        = 0x5,
    KI_LOG_EVENT_FLUSH           = 0x6,
    KI_LOG_EVENT_UPDATE_INDEX    = 0x7,
    KI_LOG_EVENT_RX_READ         = 0x8,
    KI_LOG_EVENT_RX_IDLE         = 0x9,
    KI_LOG_EVENT_RX_HEAD         = 0xa,
    KI_LOG_EVENT_RX_DATA         = 0xb,
    KI_LOG_EVENT_RX_TAIL         = 0xc,
    KI_LOG_EVENT_RX_END          = 0xd,
    KI_LOG_EVENT_SEARCH_BLOCK    = 0xe,
    KI_LOG_EVENT_BUFF_FULL       = 0xf,
}KiLogDbgEvent_t;

typedef struct KiHslDebugSymbolTag
{
    KiLogDbgEvent_t KiLogDebugEvent;
}KiHslDebugSymbol;
#endif /* OPTIMIZED_GKI_LOG_ENABLE */
typedef struct KiSysBootInfoTag
{
    /** modem build time. 
        \assoc STRING */
    char           ModemBuildTime[32];
    /** AP build time. 
        \assoc STRING */
    char           APBuildTime[32];
    /** version info
        \assoc STRING */
    char           Version[32];
    /** version info
        \assoc STRING */
    char           BootReason[96];
    uint32_t       MCUClock_MHz;
    uint8_t        CalStatus;
    uint32_t       BootTime_Ms;
}KiSysBootInfo;

#endif

/* END OF FILE */
