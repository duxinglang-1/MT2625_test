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
 * GKI header file that defines types used in GKI signals.
 **************************************************************************/

#ifndef KI_TYP_H
#define KI_TYP_H

/** \addtogroup SigSys
 * @{
 */

/***************************************************************************
 * Config
 **************************************************************************/

/** \def KI_TIMER_USER_VALUE_INT32
 * Enable 32 bit timer user value.
 * This conditional causes the user value in GKI timers (see #KiTimer) to
 * be 32 bits rather than 16 bits. */

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <stddef.h>
#include <system.h>
#include <kitqid.h>
#include <kisystim.h>
#include <pssignal.h>

/***************************************************************************
 * Kernel Signal Types
 **************************************************************************/

#define KI_MEM_POOL_DEF_TYPE KI_POOL_IDS
#include <kimemdef.h>
/** Memory type identifier.
 * This enumeration defines all the sources of memory used by the GKI.
 * The #KiSigHeader type uses this to track where the memory for a signal has
 * been allocated from. This enumeration is also used to count the number
 * of user defined memory pools. This count is then used to dimension several
 * arrays. */
typedef enum KiMemoryPoolIdTag
{

#include <kisysmem.h>

    KI_POOL_NUM_USER,

    KI_DYNAMIC_MEM_POOL,
    KI_DYNAMIC_NON_CACHEABLE_MEM_POOL,
    KI_DYNAMIC_NON_CACHEABLE_INT_MEM_POOL,

    KI_POOL_SMALLEST = 0,
    KI_POOL_LARGEST  = KI_POOL_NUM_USER - 1,

    KI_POOL_LOGGING  = 0xFE,
    KI_POOL_FIXED    = 0xFF

} KiMemoryPoolId;


/** Application specific tick type.
 * This type is used to store an application specific timing tick. For
 * example in GSM this is used to store a GSM frame count. */
typedef Int32 FrameTicks;

/** GKI tick type.
 * This type is used to store the GKI timer tick values. */
typedef Int32 KernelTicks;

/** Empty signal type.
 * This type can be used when a signal has no body. */
typedef Int8 EmptySignal;

/** To be defined signal type.
 * This type can be used for the body of a signal that has not been
 * defined yet (for work in progress). */
typedef Int8 SignalNotYetDefined;

/** System startup reason.
 * This gives the reason for the system starting up. It is used
 * in the #KiInitialiseTask type. */
typedef enum ControlResetCauseTag
{
    RESET_NORMAL_POWER_ON,
    RESET_WATCH_DOG_TIMEOUT,
    RESET_SOFTWARE_GENERATED,
    RESET_CHARGING_BATTERY,
    RESET_LOW_BATTERY,
    RESET_ALARM_POWER_ON,
    RESET_POWER_CUT_POWER_ON,
    RESET_REMOTE_POWER_ON,
    RESET_SECURITY_VIOLATION_POWER_ON,
    RESET_FAILURE_OCCURRED,
    RESET_WAKEUP_FROM_DEEP_SLEEP,
    RESET_CUSTOMER_DEFINED_1,
    RESET_CUSTOMER_DEFINED_2,
    RESET_CUSTOMER_DEFINED_3,
    RESET_CUSTOMER_DEFINED_4,
    RESET_CUSTOMER_DEFINED_5,
    RESET_CUSTOMER_DEFINED_6,
    RESET_CUSTOMER_DEFINED_7,
    RESET_CUSTOMER_DEFINED_8,
    RESET_CUSTOMER_DEFINED_9,
    RESET_CUSTOMER_DEFINED_10,
    RESET_CUSTOMER_DEFINED_11,
    RESET_CUSTOMER_DEFINED_12,
    RESET_CUSTOMER_DEFINED_13,
    RESET_CUSTOMER_DEFINED_14,
    NumOfResetCauses
}
ControlResetCause;

/*
 * Types Used by Timer Module
 */


/** Number of timer index bits within KiTimerId.
 * These checks are used to determine the type sizes needed for the
 * #KiTimerIndex and #KiTimerId types.  This define is only used
 * internally within the GKI, although it affects the size of an
 * externally visible type. */
#if   KI_TIM_MAX_NUMBER_OF_TIMERS < ((1 <<  6) - 1) /*   64 */
# define KI_TIM_TIMER_INDEX_BITS    6
#elif KI_TIM_MAX_NUMBER_OF_TIMERS < ((1 <<  7) - 1) /*  128 */
# define KI_TIM_TIMER_INDEX_BITS    7
#elif KI_TIM_MAX_NUMBER_OF_TIMERS < ((1 <<  8) - 1) /*  256 */
# define KI_TIM_TIMER_INDEX_BITS    8
#elif KI_TIM_MAX_NUMBER_OF_TIMERS < ((1 <<  9) - 1) /*  512 */
# define KI_TIM_TIMER_INDEX_BITS    9
#elif KI_TIM_MAX_NUMBER_OF_TIMERS < ((1 << 10) - 1) /* 1024 */
# define KI_TIM_TIMER_INDEX_BITS    10
#elif KI_TIM_MAX_NUMBER_OF_TIMERS < ((1 << 11) - 1) /* 2048 */
# define KI_TIM_TIMER_INDEX_BITS    11
#elif KI_TIM_MAX_NUMBER_OF_TIMERS < ((1 << 12) - 1) /* 4096 */
# define KI_TIM_TIMER_INDEX_BITS    12
#elif KI_TIM_MAX_NUMBER_OF_TIMERS < ((1 << 13) - 1) /* 8192 */
# define KI_TIM_TIMER_INDEX_BITS    13
#else
/* It is possible to support more timers, but to do so with a catch all
 *  case may waste bits in the TimerId that can be used to produce a
 *  wider range of TimerIds.  Therefore flag the error such that additional
 *  tests can be added if needed or the timer requirement decreased.
 */
# error Too large: KI_TIM_MAX_NUMBER_OF_TIMERS
#endif

/** Mask to extract index from #KiTimerId. */
# define KI_TIMER_INDEX_MASK        ((1 << KI_TIM_TIMER_INDEX_BITS) - 1)

/** Type used to reference a timer by index only.
 * This is used internally in the GKI, where safe, to save transporting
 * or storing the whole #KiTimerId. */
#if   KI_TIM_TIMER_INDEX_BITS <=  BITS_PER_INT8
typedef Int8  KiTimerIndex;
#elif KI_TIM_TIMER_INDEX_BITS <= BITS_PER_INT16
typedef Int16 KiTimerIndex;
#elif KI_TIM_TIMER_INDEX_BITS <= BITS_PER_INT32
typedef Int32 KiTimerIndex;
#else
# error Too large: KI_TIM_TIMER_INDEX_BITS
#endif


/** Unique TimerId bits.
 * The number of bits that must be present in the #TimerId to produce
 * random timer ID values.  This is used together with
 * #KI_TIM_TIMER_INDEX_BITS to dimension the #KiTimerId type.
 * 2^(this value) multiplied by the minimum number of free timers in the
 * system gives the worst case number of timers that must be started and
 * stopped before a timerId is recycled, assuming that the rest of the
 * timerId bits are occupied by the #KI_TIM_TIMER_INDEX_BITS. */
#define KI_TIM_MIN_ENTROPY_BITS  8   /* 2^8 = 256 */

/** Type for storing a timer id.
 * The type for a Timer Id, dimensioned from the entropy and timer
 * index bits. */
#if   (KI_TIM_TIMER_INDEX_BITS + KI_TIM_MIN_ENTROPY_BITS) <=  BITS_PER_INT8
typedef Int8  KiTimerId;
#elif (KI_TIM_TIMER_INDEX_BITS + KI_TIM_MIN_ENTROPY_BITS) <= BITS_PER_INT16
typedef Int16 KiTimerId;
#elif (KI_TIM_TIMER_INDEX_BITS + KI_TIM_MIN_ENTROPY_BITS) <= BITS_PER_INT32
typedef Int32 KiTimerId;
#else
# error Too large: KI_TIM_TIMER_INDEX_BITS + KI_TIM_MIN_ENTROPY_BITS
#endif

/** Invalid timer id value.
 * This define specifies an invalid value for a #KiTimerId to be. This
 * is used to indicate when a timer is not running. */
#if   (KI_TIM_TIMER_INDEX_BITS + KI_TIM_MIN_ENTROPY_BITS) <=  BITS_PER_INT8
#define KI_TIMER_NOT_RUNNING       0xff
#elif (KI_TIM_TIMER_INDEX_BITS + KI_TIM_MIN_ENTROPY_BITS) <= BITS_PER_INT16
#define KI_TIMER_NOT_RUNNING     0xffff
#elif (KI_TIM_TIMER_INDEX_BITS + KI_TIM_MIN_ENTROPY_BITS) <= BITS_PER_INT32
#define KI_TIMER_NOT_RUNNING 0xffffffff
#else
# error Too large: KI_TIM_TIMER_INDEX_BITS + KI_TIM_MIN_ENTROPY_BITS
#endif

/** The size of the KiTimerId in bits.
 * This is used within the GKI timer task when calculating the storage
 * size of types related to the KiTimerId. */
#if   (KI_TIM_TIMER_INDEX_BITS + KI_TIM_MIN_ENTROPY_BITS) <=  BITS_PER_INT8
#define KI_TIMER_ID_BITS              8
#elif (KI_TIM_TIMER_INDEX_BITS + KI_TIM_MIN_ENTROPY_BITS) <= BITS_PER_INT16
#define KI_TIMER_ID_BITS             16
#elif (KI_TIM_TIMER_INDEX_BITS + KI_TIM_MIN_ENTROPY_BITS) <= BITS_PER_INT32
#define KI_TIMER_ID_BITS             32
#else
# error Too large: KI_TIM_TIMER_INDEX_BITS + KI_TIM_MIN_ENTROPY_BITS
#endif

/** Type for user value associated with a GKI timer.
 * This type is used to store a user value within the #KiTimer structure. */
#if defined (KI_TIMER_USER_VALUE_INT32)
typedef Int32 KiTimerUserValue;
#else
typedef Int16 KiTimerUserValue;
#endif

#if !defined(DISABLE_KI_TIMER_ID_COMPAT_MODE)
/** Old Timer ID type.
 * This manifest constant is used for compatibility with old code. */
#define TimerId  KiTimerId
#endif

/** Timer information.
 * This structure is used to pass the required information for a timer
 * into the GKI start timer functions. A task needs to keep a local
 * copy of this structure for every concurrent timer it intends to use. */
typedef struct KiTimerTag
{
    /** Timer duration.
     * This field defines the duration, in kernel ticks, for which the timer
     * is required to run. The GKI macros #SECONDS_TO_TICKS() and
     * #MILLISECONDS_TO_TICKS() convert between seconds or milliseconds and
     * number of kernel ticks. The macros will round up to the nearest tick.
     * The smallest unit of time is one system tick. For a system based on the
     * GSM frame rate this is 4.615ms (the GSM TDMA frame period). */
    FrameTicks          timeoutPeriod;

    /** Task to receive expiry signal.
     * This field specifies the task to which the timer expiry signal will be
     * sent. */
    TaskId              myTaskId;

    /** Timer reference number.
     * This field contains a reference number allocated by the GKI which
     * uniquely identifies the instance of the timer. If the timer is stopped
     * and then started again, the new timerId value will be different. The
     * value is used to filter rogue timeout signals belonging to a previous
     * instance of the timer. The task receiving a timeout signal must compare
     * the timerId value in the timeout signal with the timerId value within
     * the local KiTimer structure. If they are different, the timeout signal
     * belongs to a previous instance of the timer that has already been
     * stopped. */
    KiTimerId           timerId;

    /** Task specific data.
     * This field contains a reference number to allow a task to identify a
     * timer when using multiple timers. The GKI makes no use of this variable
     * except to include it in the timer expiry signal. */
    KiTimerUserValue    userValue;
}
KiTimer;

/*
 * Types used by KI MEM POOL STATISTICS
 */

/** One entry in a #KiMemPoolStatisticsCnf signal.
 * This type is used to store the current and maximum count of allocations for one
 * size range. */
typedef struct KiMemSubPoolStatsEntryTag
{
    /** This is the current number of blocks allocated within the associated
     * memory range for this entry. */
    Int16    currentCount;
    /** This is the maximum number of blocks allocated at one time within the
     * associated memory range for this entry. */
    Int16    maxCount;
    /** If the last block to be allocated within the associated memory range
     * for this entry was a signal this field gives the signal id otherwise
     * it is set to NON_SIGNAL. */
    SignalId lastSignal;

} KiMemSubPoolStatsEntry;


/** Task Set index calculation.
 * This type is only used internally to allow calculation of task set indexes,
 * see the kiTaskSetIndexes data array. */
#define KI_TASK_DEF_TYPE   KI_TASK_SET_INDEX_CALC
#include <kitskdef.h>
typedef enum KiTaskSetIndexCalcTag
{
#include <kisystsk.h>
    KI_TASK_SET_INDEX_MAX
} KiTaskSetIndexCalc;

/** Number of unique task id's.
 * This gives the number of unique task id's defined. These come from the
 * #KI_TASK_DEF, #KI_RESERVED_TASK and #KI_TASK_SET_START macros. This value is
 * used to dimension various arrays within the GKI. */
#define KI_NUM_UNIQUE_TASK_IDS_DEFINED   (KI_TASK_SET_INDEX_MAX + 1)

#if defined (KI_ASSERT_TESTING) || defined(UPGRADE_COOLART)
/** Assert Testing Types */
typedef enum AssertTestingTypeTag
{
    ASSERT_TYPE_WARN_ASSERT,
    ASSERT_TYPE_WARN_FAIL,
    ASSERT_TYPE_WARN_CHECK,
    ASSERT_TYPE_WARN_PARAM,
    ASSERT_TYPE_WARN_STR_PARAM,
    ASSERT_TYPE_WARN_CHECK_BOUNDS,
    ASSERT_TYPE_WARN_CHECK_RANGE,
    ASSERT_TYPE_FATAL_ASSERT,
    ASSERT_TYPE_FATAL_FAIL,
    ASSERT_TYPE_FATAL_CHECK,
    ASSERT_TYPE_FATAL_PARAM,
    ASSERT_TYPE_FATAL_STR_PARAM,
    ASSERT_TYPE_FATAL_CHECK_BOUNDS,
    ASSERT_TYPE_FATAL_CHECK_RANGE,
    ASSERT_TYPE_DEV_ASSERT_FILE,
    ASSERT_TYPE_DEV_PARAM_FILE,
    ASSERT_TYPE_FATAL_FAIL_FILE,
    ASSERT_TYPE_FATAL_PARAM_FILE,
    ASSERT_TYPE_INVALID
}
AssertTestingType;
#endif /* KI_ASSERT_TESTING */

#if defined (FR_HSL_2_GKI)
#define MAX_HSL_PRINTF_ARRAY_SIZE 1024

typedef struct KiHslPrintfWrapperIndTag
{

    /** \assoc VAR_ARRAY \ref rawData */
    uint16_t length;
    /** \assoc CUSTOM \opt "%CURRENT_GENIE_EXE_PATH%\decoders\GenieHslDecoder\GenieHslDecoder.dll" */
    uint8_t  rawData[MAX_HSL_PRINTF_ARRAY_SIZE];
}KiHslPrintfWrapperInd, KiHslPrintfLevel4Ind, KiHslPrintfAtciInd;
#endif

#if defined (MTK_NBIOT_ICD_ENABLE)
#define MAX_ICD_PRINTF_ARRAY_SIZE 1024

typedef struct KiIcdPrintfWrapperIndTag
{
    uint16_t sequence_no;
    /** \assoc VAR_ARRAY \ref rawData */
    uint16_t length;
    /** \assoc ICD \opt "%CURRENT_GENIE_EXE_PATH%\decoders\GenieIcdDecoder\GenieIcdDecoder.dll" */
    uint8_t  rawData[MAX_ICD_PRINTF_ARRAY_SIZE];
}KiIcdPrintfWrapperInd;
#endif

/** @} */ /* End of SigSys group */

#endif

/* END OF FILE */
