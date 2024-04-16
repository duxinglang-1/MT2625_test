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
 * GKI header for timer API.
 * This file contains the API functions and macros used for controlling GKI
 * timers.
 **************************************************************************/

#ifndef GKITIMER_H
#define GKITIMER_H

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
#include "nbiot_modem_common_config.h"
#include <system.h>

#include <kitqid.h>
#include <ki_sig.h>
#include <ki_typ.h>
#include <ki_time.h>
#include <gkitask.h>

/***************************************************************************
 * function prototypes
 **************************************************************************/

/***************************************************************************
 * Defines
 **************************************************************************/

/*The number of seconds after which the timeline wraps.*/
#define KITIMENOWS_MAX_TICK_COUNT   42949672

/** Helper macro to supply debug parameters.
 * If #KI_TIM_DEBUG is defined, this macro expands to provide the extra
 * three parameters that are expected by some functions for extra debug.
 */
#if defined(KI_TIM_DEBUG)
#define KI_TIM_DEBUG_PARAMS ,MODULE_NAME,__LINE__,0
#else
#define KI_TIM_DEBUG_PARAMS
#endif


/* timer inaccuracy is in terms of percentage*/
#define DEFAULT_TIMER_INACCURACY    20

/** \def KI_NEW_TIMER_API
 * Indicates that this version of the GKI has the new timer API.
 * This can be used to conditionally compile code that needs to support older
 * versions of the API, and is used in \a geniexec.c to detect whether the
 * new timer macros are defined or not.
 *
 * \warning This define is not configurable and should not be set or unset.
 */
#define KI_NEW_TIMER_API

/** \def KI_ARBITRARY_TIMER_EXPIRY_SIGNAL
 * Indicates that this version of the GKI supports the definition of
 * any signal id for to send on expiry of a timer.
 * This can be used to conditionally compile code that needs to support older
 * versions of the API, and is used in \a geniexec.c to detect whether this
 * functionality is present or not.
 *
 * \warning This define is not configurable and should not be set or unset.
 */
#define KI_ARBITRARY_TIMER_EXPIRY_SIGNAL

/***************************************************************************
 * Typedefs
 **************************************************************************/

/***************************************************************************
 * Kernel Function redirections
 **************************************************************************/

/** \defgroup CfgGkiTimer GKI Timer Configuration
 * \ingroup Cfg
 * GKI Timer Configuration Options.
 * Generally the Timer services in the GKI need no configuration.  However,
 * since timer usage can greatly affect power consumption of the device a
 * number of options are provided to aid in finding and reporting timers
 * that impact power consumption.
 *
 * \see \ref TechTimerOptimisation "GKI Timer Optimisation Techniques"
 * @{
 */

/** \def KI_TIM_DEBUG
 * Additional debug for timers.
 * This adds the source module name and file line as well as an additional
 * parameter to the GKI timer function macros.  These macros pass these
 * parameters to the timer task which stores these additions along side
 * the timer data such that at any point, the location in the code where
 * some timer was started can be reported.  This uses more memory, but
 * is very useful when attempting to optimise timer expiry patterns as it
 * makes it trivial to find timer allocation sites.
 *
 * \see \ref TechTimerOptimisation "GKI Timer Optimisation Techniques"
 */

/** \def KI_TIM_DEBUG_POWERSAVING
 * Debug for timers with respect to power saving.
 * This causes LEDS 0 or 1 to be pulsed briefly on when a timer expires,
 * and LED 2 to pulse when a timer prevents optimum slow clocking.
 * LED 0 is used to indicate the expiry of a coarse timer, while LED 1 pulses
 * in response to an exact timer.  When #KiMaximumSleep is called, LED 2
 * is pulsed if some timer prevented the full requested duration for sleep
 * from being returned, and additionally details of the timer that caused
 * the greatest reduction in sleep duration are dumped by #M_KiTimErrantRpt.
 *
 * \note LEDs will only work if the \a M_LedOn and \a M_LedOff macros are
 *       appropriately defined for the platform.
 *
 * \see \ref TechTimerOptimisation "GKI Timer Optimisation Techniques"
 */

/** \def KI_TIM_DEBUG_POWERSAVING_TRAP
 * Assert when poorly configured timers are found.
 * Defining this macro causes newly started coarse timers to be checked to
 * ensure that the parameters are compatible with slow clocking.  The checks
 * simply ensure that the expiry window is sufficiently large not to prevent
 * truncation of sleep periods requested via #KiMaximumSleep, and to also
 * ensure that the earliest expiry values for timers are not sufficiently
 * small to cause rapid expiry of timers.
 *
 * Care needs to be taken with this option, as many timers may violate the
 * imposed checks without causing any detriment to power saving.  For example,
 * timers operating in modes where power saving is not possible may violate
 * the constraints without causing any issue.
 *
 * Since this trap will call #DevAssert, the build should be configured with
 * #CONTINUE_ON_ASSERTFAIL and possibly #KI_USE_ASSERT_SIGNALS.
 *
 * \see \ref TechTimerOptimisation "GKI Timer Optimisation Techniques"
 */

/** \def M_KiTimErrantRpt(sM, sL, sV, timID, tskID, uSRvAL, eE, lE)
 * Marco to report timers that prevent or reduce power saving.
 * This macro is called if #KI_TIM_DEBUG_POWERSAVING is defined and a timer
 * reduces the requested duration for slow clocking.  The macro takes a
 * default implementation if it is not already defined in #kicfg.h - the
 * default implementation simply calls printf() to dump the parameters.
 *
 * \param sM     The source module where the timer was started (taken from
 *                MODULE NAME).
 * \param sL     Source line where the timer was started.
 * \param sV     Source value used for advanced debugging.
 * \param timID  The TimerId for the errant timer.
 * \param tskID  The TaskId to which the timer expiry will be sent.
 * \param uSRvAL The user value stored with the timer.
 *
 * \note The parameters \a sM, \a sL and \a sV are only present if
 *        #KI_TIM_DEBUG is defined.
 *
 * \see \ref TechTimerOptimisation "GKI Timer Optimisation Techniques"
 */

/** @} */

/** \defgroup ApiGkiTimer GKI Timer API
 * \ingroup Api
 * The functional interface for timer services provided by the GKI.
 * The GKI timer API provides functions for creating single-shot delays in the
 * range of 5ms through to hours and days.  Timers are started and manipulated
 * using this functional interface, and expired timers are indicated to tasks
 * by the reception of a #KiTimerExpiry signal allowing tasks to block on
 * #KiRecieveSignal until the timer has expired.
 *
 * The timer task is also involved in power saving activities, and the use of
 * coarse timers, whose expiry may occur at any point in a range of timer, is
 * encouraged for timers that maybe active in states when slow clocking may
 * be desired.
 *
 * The GKI Timer task measures time in #KernelTicks, which may have a varying
 * relation to real time depending on the exact configuration of the system.
 * Therefore a number of macros are provided to convert between absolute time
 * measures and #KernelTicks.
 *
 *   - #MILLISECONDS_TO_TICKS and #TICKS_TO_MILLISECONDS
 *   - #KI_GSM_FRAME_TO_KERNEL_TICKS and #KI_KERNEL_TO_GSM_FRAME_TICKS
 *
 * Some of the function for starting a timer can accept a signal id parameter
 * that sets the id of the timer expiry signal. This signal must be defined to
 * have a body of type #KiTimerExpiry, for example:
 *
 * \code
 * SIG_DEF(SIG_XYZ_TIMER_EXPIRY, KiTimerExpiry xyzTimerExpiry)
 * \endcode
 *
 * \see \ref TechTimerOptimisation "GKI Timer Optimisation Techniques"
 * @{
 */

/** Start a Timer to expire in a window described by fixed offsets.
 * This function will start a timer that will expire within some defined
 * window.  For timers operating in modes where power saving is possible
 * by powering down the MCU, the window is used by the timer task to relax
 * the requirements for the MCU to be awake such that most timers expire
 * while the system is awake due to other activities.
 *
 * A timer is always guaranteed to expire at some time described by the
 * following:
 *
 * \code
 *   E <= expiryTime <= L;
 * \endcode
 *
 * The parameter \a E describes the minimum number of #KernelTicks that must
 * pass before the timer may expire, and generally a timer will expire as soon
 * as this count has been reached unless the MCU is asleep, in which case the
 * timer will expire the next time the system is awake, which is guaranteed to
 * not exceed the \a L.  Note that generally \a E will be less than or equal
 * to \a L, but this is not a requirement since when the time line wraps the
 * latest expiry value may be less than the earliest even though it is
 * chronologically later; the timer task will handle this correctly.
 *
 * The function does not return a value directly, but will put a valid
 * #KiTimerId value into the passed #KiTimer structure.  This ID value can
 * be used such that when a #KiTimerExpiry signal is received, the expired timer
 * can be identified, or such that further timer functions such as #KiStopTimer
 * can be used to control the started timer.
 *
 * An alternative method of identifying timers is to use their \a userValue
 * to carry pre-defined values that can be used in switch statements and other
 * comparisons, although the #KiTimer structure is needed if the timer is to be
 * stopped at any point.
 *
 * \note On the target, the \a myTaskId field of the passed #KiTimer structure
 *        is checked to ensure that it is non-zero and also different to the
 *        special value UNKNOWN_TASK_ID.
 *
 * \param T  Pointer to the #KiTimer structure describing the timer.
 * \param E  The minimum number of #KernelTicks that must pass before the timer may
 *            expire.
 * \param L  The maximum number of #KernelTicks that may pass before the timer must
 *            expire.
 *
 * \retval void  Nothing is returned directly.
 *
 * \see KiTimerExpiryTag, KernelTicks
 */
#define KiStartWinTimer(T,E,L) KiStartWinTimerSig(T,E,L, SIG_TIMER_EXPIRY)

/** Start a Timer to expire with a user defined signal id in a window
 * described by fixed offsets.
 * This function is similar to #KiStartWinTimer except it allows the user
 * to specify the SignalId of the timer expiry signal.
 *
 * \param T  Pointer to the #KiTimer structure describing the timer.
 * \param E  The minimum number of #KernelTicks that must pass before the timer may
 *            expire.
 * \param L  The maximum number of #KernelTicks that may pass before the timer must
 *            expire.
 * \param S  The signal id of the signal to send on timer expiry.
 *
 * \retval void  Nothing is returned directly.
 *
 * \see KiTimerExpiryTag, KernelTicks
 */
#if !defined(ON_PC)
# if defined (GX2)
#include <gkisig.h>
#define KiStartWinTimerSig(T,E,L,S)                                                \
{                                                                                  \
    KiTimerStart timerToLog;                                                       \
    DevCheck((T)->myTaskId != UNKNOWN_TASK_ID &&                                   \
             (T)->myTaskId != PASS_THRU_TASK_ID,                                   \
             (T)->myTaskId, (T)->userValue, KiThisTask());                         \
                                                                                   \
    KiOsStartTimer(T,E,L,S KI_TIM_DEBUG_PARAMS);                                   \
    timerToLog.timerId       = T->timerId;                                         \
    timerToLog.timeoutPeriod = T->timeoutPeriod;                                   \
    timerToLog.userValue     = T->userValue;                                       \
    timerToLog.timerIndex    = 0;                                                  \
    KiLogSignalStruct(SIG_START_TIMER, sizeof(KiTimerStart), (void *) &timerToLog);\
    GxStartTimerEx (T,E,L,S KI_TIM_DEBUG_PARAMS, FALSE);                           \
}
# else /* GX2 */
#define KiStartWinTimerSig(T,E,L,S)                                 \
{                                                                   \
    DevCheck((T)->myTaskId != UNKNOWN_TASK_ID &&                    \
             (T)->myTaskId != PASS_THRU_TASK_ID,                    \
             (T)->myTaskId, (T)->userValue, KiThisTask());          \
                                                                    \
    KiOsStartTimer(T,E,L,S KI_TIM_DEBUG_PARAMS);                    \
}
# endif /* else GX2 */

#else
#define KiStartWinTimerSig(T,E,L,S) KiOsStartTimer(T,E,L,S KI_TIM_DEBUG_PARAMS);
#endif

/** Start a Timer to expire in a window described by a point and percentage inaccuracy.
 * This function is similar to #KiStartWinTimer, except that the parameters
 * are passed as a pointer to a #KiTimer structure and the expiry window is
 * described by the mid-point \a timeoutPeriod and allowable inaccuracy
 * percentage.  This macro actually resolves to use #KiStartWinTimer
 * internally, locally performing the arithmetic to compute the earliest and
 * latest expiry times.
 *
 * For some cases this type of timer allows the expiry window to be described
 * more simply, where as other cases may find #KiStartWinTimer provides an easier
 * way to manage the possible expiry range.
 *
 * \note Inaccuracies of 100% or more have little meaning as the timer may expire
 *       immediately providing no delay.  This type of timer should be avoided.
 *
 * \param T  Pointer to a #KiTimer structure giving all the timer parameters,
 *            including the \a timeoutPeriod.
 * \param I  Inaccuracy percentage value ranging from \a 0 to \a 100.
 * \retval void  Nothing is returned directly; although the #KiTimer
 *                structure's \a timerId field is updated with the ID of the
 *                started timer.
 * \see KiTimerExpiryTag, KiStartWinTimer, KernelTicks
 *
 */
#define     KiStartCoarseTimer(T,I)  KiStartCoarseTimerSig(T,I, SIG_TIMER_EXPIRY)

/** Start a Timer to expire with a user defined signal id in a window
 * described by a point and percentage inaccuracy.
 * This function is similar to #KiStartCoarseTimer but it allows the
 * user to specify the signal id of the expiry signal.
 *
 * \param T  Pointer to a #KiTimer structure giving all the timer parameters,
 *            including the \a timeoutPeriod.
 * \param I  Inaccuracy percentage value ranging from \a 0 to \a 100.
 * \param S  The signal id of the signal to be sent on timer expiry.
 * \retval void  Nothing is returned directly; although the #KiTimer
 *                structure's \a timerId field is updated with the ID of the
 *                started timer.
 * \see KiStartCoarseTimer, KiTimerExpiryTag, KiStartWinTimer, KernelTicks
 *
 */
#define     KiStartCoarseTimerSig(T,I,S)                            \
{                                                                   \
    const KernelTicks winSize = ((T)->timeoutPeriod * (I)) / 100;   \
                                                                    \
    KiStartWinTimerSig((T),                                         \
                    (T)->timeoutPeriod - winSize,                   \
                    (T)->timeoutPeriod + winSize,                   \
                    (S));                                           \
}

/** Start a Timer to expire at some point in time.
 * This starts a timer that should expire exactly after some number of
 * #KernelTicks.  This type of timer will impact power saving activities
 * by requiring the MCU to be running at the point in time where such a
 * timer is to expire, and so care should be taken when using these timers.
 *
 * \param T  Pointer to a #KiTimer structure giving all the timer parameters.
 *
 * \note This function is equivalent to #KiStartCoarseTimer with an inaccuracy
 *        value of 0.
 *
 * \see KiStartCoarseTimer
 */
#define     KiStartExactTimer(T)       KiStartWinTimerSig((T),                \
                                                          (T)->timeoutPeriod, \
                                                          (T)->timeoutPeriod, \
                                                          SIG_TIMER_EXPIRY)

/** Start a Timer to expire at some point in time with user defined expiry signal.
 * This function is basically the same as #KiStartExactTimer but it allows the
 * user to specify the signal id of the expiry signal.
 *
 * \param T  Pointer to a #KiTimer structure giving all the timer parameters.
 * \param S  The signal id of the signal to be sent on timer expiry.
 *
 * \note This function is equivalent to #KiStartCoarseTimerSig with an inaccuracy
 *        value of 0.
 *
 * \see KiStartCoarseTimerSig
 */
#define     KiStartExactTimerSig(T, S) KiStartWinTimerSig((T),                \
                                                          (T)->timeoutPeriod, \
                                                          (T)->timeoutPeriod, \
                                                          (S))


/** Start a Timer to expire at some point in time.
 * This is a synonym for #KiStartExactTimer.
 *
 * \see KiStartCoarseTimer
 */
#define     KiStartTimer(T)               KiStartExactTimer(T)

/** Start a Timer to expire at some point in time using given expiry signal id.
 * This is a synonym for #KiStartExactTimerSig.
 *
 * \see KiStartCoarseTimerSig
 */
#define     KiStartTimerSig(T,S)          KiStartExactTimerSig(T,S)


/** Stop some timer.
 * This is a function for stopping some timer whose ID is stored in a
 * #KiTimer structure.
 *
 * If the timer has already expired, no action is taken, although care should
 * be taken to ensure that timers that are already known to have expired
 * are not stopped as timer IDs are recycled after some period.
 *
 * This function will also overwrite the timer ID in the passed structure
 * with #KI_TIMER_NOT_RUNNING such that it is deliberately difficult to
 * attempt to stop a timer more than once.
 *
 * \note Under DEVELOPMENT_VERSION, TimerId 0 is reserved and this macro
 *        will cause an assertion if it is passed a KiTimer whose
 *        \a timerId is 0.  This is deliberate to help detect the passing
 *        of uninitialised KiTimer structures to this macro.
 *
 * \note When GX2 is defined ON_PC must not be defined. Historically ON_PC was used for PC
 *        testing and PC testing was supported by geniexec.c. The compiler flag GX2 is also for
 *        PC testing but this kind of PC testing is supported by gx2.c. GX2 offers a simulation
 *        which is closer to the real time behavior on target.
 *
 * \param T  A pointer to a #KiTimer structure identifying the timer to stop.
 */
#if !defined (ON_PC) && defined (GX2)
#define KiStopTimer(T)                                                  \
{                                                                       \
    GxStopTimer(T);                                                     \
    KiOsStopTimer(T);                                                   \
}
#else /* !ON_PC && GX2 */
# if defined (DEVELOPMENT_VERSION)
#define KiStopTimer(T)                                                  \
{                                                                       \
    DevCheck((T)->timerId != 0, KiThisTask(), 0, 0);                    \
    KiOsStopTimer(T);                                                   \
}

# else /* DEVELOPMENT_VERSION */
#define     KiStopTimer(T)                   KiOsStopTimer(T)
# endif /* else DEVELOPMENT_VERSION */
#endif /* else !ON_PC && GX2 */


/** Get the number of ticks until a timer will expire.
 * Given the ID of some previously started timer and the task to which the
 * expiry signal will be sent, this returns the number of #KernelTicks that
 * remain until the timer can be expected to expire.
 *
 * If the timer has already expired, a value of \a 0 will be returned.  In
 * the case that the timer is a coarse timer and has a window in which to
 * expire, the greatest number of #KernelTicks that may pass before the time
 * may expire is returned.
 *
 * \note Under DEVELOPMENT_VERSION, TimerId 0 is reserved and this function
 *        will cause an assertion if KiGetRemainingTime() is passed a TimerId
 *        of 0.  This is deliberate to help detect the passing of
 *        uninitialised KiTimer structures.
 *
 * \param T  The ID of some previously started timer.
 * \param A  The task ID to which the expiry signal will be sent when the
 *            timer does expire.
 * \retval KernelTicks  The maximum number of ticks that can pass before
 *                       the timer will expire.
 */
#define     KiGetRemainingTime(T,A)       KiOsGetRemainingTime (T,A)

/* @} */ /* End of ApiGkiTimer group */


/** Provide a tick to the GKI Timer Task.
 * \warning In most systems from TTPCom, this will already have been
 *          configured and should not be called from application code.
 *
 * This indicates to the GKI Timer task that some number of #KernelTicks
 * has passed.  After this is called, the timer task is likely to run and
 * check it's internal structures to send and #KiTimerExpiry signals to
 * tasks as needed.
 *
 * \param T  The number of #KernelTicks that have passed.
 */
#define     KiTick(T)                     KiOsTick (T)


/** Determine the maximum duration before the timer task needs to run.
 * \warning In most systems from TTPCom, this will already have been
 *          configured and should not be called from application code.
 *
 * This function determines the largest duration before which the timer task
 * next needs to inspect its internal structures to meet the expiry
 * requirements of all started timers.  The timer task will use expiry window
 * values for coarse timers to attempt to determine the largest duration
 * for which power saving may occur.
 *
 * \param T  The maximum number of #KernelTicks for which sleeping could
 *            occur.  The returned value may never exceed this.
 * \param P  The point of GKI timer info structure returned to caller function.
 * \retval KernelTicks  The number of #KernelTicks that may pass before #KiTick
 *                      next needs to be called.
 */
#define     KiMaximumSleep(T, P)                    KiOsMaximumSleep (T, P)


/** Check if any timer actions are pending.
 * \warning In most systems from TTPCom, this will already have been
 *          configured and should not be called from application code.
 *
 * When the system is considering power saving, it is important that if there
 * are outstanding signals to be processed by the timer task that these get
 * processed before the MCU is put to sleep.  This function returns whether
 * there are outstanding requests to start or stop timers, and hence whether
 * it is safe to sleep.
 *
 * \retval Boolean  #TRUE if timer signals are pending and sleep should be
 *                   postponed.
 */
#define     KiTimersStartedOrStopped()    KiOsTimersStartedOrStopped ()

/** Get a time reference.
 * This returns the count of passed #KernelTicks since the system has
 * started, noting that after a very long time the returned value may
 * wrap according to the maximum count that can be held in the #KernelTicks
 * type.
 *
 * Generally it is better to measure the passage of time using the timer
 * functions, since these handle the wrapping of the time line and provide
 * a signaled interface that can be logged in Genie.
 *
 * If the system is being run under simulation with GX2 then update the
 * time base with that of genies before passing back the relative time.
 */
#if !defined (ON_PC) && defined (GX2)
#define     KiGetRelativeTime(T)                            \
{                                                           \
            GxUpdateKernelTime();                           \
            KiOsGetRelativeTime (T);                        \
}
#else /* !ON_PC && GX2 */
#define     KiGetRelativeTime(T)          KiOsGetRelativeTime (T)
#endif /* else !ON_PC && GX2 */


#ifdef TEMP_TIME_SAVING_DEVELOPMENT
#define KI_TIM_MAX_SAVED_TIMER_NUM   5
#define KI_TIM_TIMER_DATA_VALID_FALG 0xFEDCBA98

/* KiTimMiniActiveTimerData
 *  This structure contains the information of a active timer
 *  need to save.
 */
typedef struct KiTimMiniActiveTimerDataTag
{
   KernelTicks         earliestExpiry;
   KernelTicks         latestExpiry;
   SignalId            expiryId;
   KiTimerUserValue    userValue;
   TaskId              taskId;
   KiTimerId           timerId;
} KiTimMiniActiveTimerData;


#endif



/***************************************************************************
 * Function prototypes
 **************************************************************************/

void       KiOsStartTimer          (KiTimer              *timerToStart,
                                    KernelTicks           earliestExpiry,
                                    KernelTicks           latestExpiry,
                                    SignalId              expirySignalId
#if defined(KI_TIM_DEBUG)
                                   ,char                 *sourceModule,
                                    Int16                 sourceLine,
                                    Int16                 sourceValue
#endif
                                    );

#if !defined(ON_PC) && defined (GX2)
/** Start a GKI timer in genie to allow genie to log and control time.
 *
 * \return void
 */
void GxStartTimerEx(KiTimer     *timerToStart
#if defined(KI_NEW_TIMER_API)
                   ,KernelTicks  earliestExpiry,
                    KernelTicks  latestExpiry
#endif
#if defined (KI_ARBITRARY_TIMER_EXPIRY_SIGNAL)
                   ,SignalId     expirySignalId
#endif
                   ,Boolean logTimerStart);

/** Stop a GKI timer in genie to allow genie to log and control time.
 *
 * \return void
 */
void        GxStopTimer             (KiTimer             *timerToStop);

void GxUpdateKernelTime (void);
#endif /* ON_PC && GX2 */

void        KiOsStopTimer           (KiTimer             *timerToStop);

/* Called only by Layer one - informs timer task how many ticks since
** last called
*/
void        KiOsTick                (KernelTicks          increment);

/* Called only by Layer one - informs l1 sleep function of shortest timeout.
 *  This is not called directly, instead it is called via a macro that also
 *  translates between KernelTicks and the required unit of time (e.g. FrameTicks).
 */
#ifdef TEMP_TIME_SAVING_DEVELOPMENT
KernelTicks KiOsMaximumSleep        (KernelTicks  ticksUntilNextActivity,  KiTimMiniActiveTimerData* pSleepTimerData);
#else
KernelTicks KiOsMaximumSleep        (KernelTicks  ticksUntilNextActivity,  void* pSleepTimerData);  
#endif
 
/* Called only by Layer one - informs l1 sleep function if any timers have
** been started or stopped since the last tick
*/
Boolean     KiOsTimersStartedOrStopped (void);

void        KiOsGetRelativeTime     (KernelTicks         *time);

KernelTicks KiOsGetRemainingTime    (TimerId              timerId,
                                     TaskId               taskId);


/* Time from boot in seconds */
uint32_t KiTimeNowS (void);

#ifdef KI_ENABLE_SUBSCRIBE_EVENT_TIMER
/* Timer Activity callback function pointer type.
 * The PSC need to know timer expiry after Ki_subscribeEventTimer(true) to re-evaluate sleep opportunity.
 * If a timer expiry occurs, this callback function will be invoked to notify psc
 */
typedef void (*TimerActivityCB)(void);
void Ki_subscribeEventTimer(bool traceExpiry);
void KiRegisterTimerActivityCallback(TimerActivityCB notifyFunc);
#endif
#if defined (__cplusplus)
}
#endif

/** @} */ /* End of PrdGki group */


/***************************************************************************
 * Documentation Page about Optimising Timers
 ***************************************************************************/

/**
\page TechTimerOptimisation Timer Optimisation
\section Intro Introduction

This page gives information about the use of GKI timers with relation
to the power saving technique of slow clocking, whereby the main system
clock is temporarily suspended for some period to conserve battery life.

- For information about the timer API see the \ref ApiGkiTimer "GKI Timer API"
   documentation.
- For information about timer configuration switches see the
  \ref CfgGkiTimer "GKI Timer Configuration" documentation.

\section Background Background

When in idle mode, the system will normally be idle for long periods
between short bursts of activity.  For a GSM system, paging blocks will
be frequently received, and with careful selection of timer parameters,
these periods can be used to also process any timer based events such that
the system does not need to be awake at other times.

For GSM the paging rate is set by the network and is described by a
value between 2 and 9 meaning that paging blocks are to be received
that number of multi-frames apart.  A multiframe is 51 GSM frames, such
that the following gives the sleep duration in milliseconds for a
given paging rate:

\code
 gsmSleepDuration_ms = (60/13) * 51 * pagingRate
\endcode

Therefore where possible, timers should be configured such that at each paging
rate the expiry pattern can fall into the naturally occurring paging
reception blocks when the system will be awake.  If a timer cannot be made to
fit into a paging block, the timers will require the system to remain active
and may truncate or prevent sleep periods.


\section TimerStates System States

Since power saving only occurs in certain system states, it is only
timers that expire within these states that need consideration for
optimisation.  For example, the system will not have opportunity for slow
clocking while in a phone call (dedicated state), so timers that are only
used within dedicated mode can be exact or have low \a inaccuracy values.

Timers that have very long timeout periods or occur infrequently in
idle mode are also poor candidates for optimisation since they will have
little effect on power saving; for example, a timer that expires once every
10 hours will yield a very small saving over the optimisation of a timer
that expires every 10 seconds.


\section TimerConfig Timer Configuration

Timers are started using one of #KiStartWinTimer, #KiStartCoarseTimer,
#KiStartExactTimer, and #KiStartTimer.  #KiStartTimer and
#KiStartExactTimer are identical in function, and create timers that do
not allow for any optimisation with regard to power saving.

#KiStartCoarseTimer and #KiStartWinTimer are used to start a timer that
is allowed to expire within some window of values, and it is these timers
that allow for optimisation when power saving is active.  Both of these
macros call the same underlying GKI function, but allow for the window in
which the timer may expire to be expressed in different ways, each of which
may suit some applications more than others.


For #KiStartCoarsetimer, an \a inaccuracy value is expressed as a percentage
of the normal timeout period, and represents the percentage early or late in
which the timer may expire.  The following shows how the possible expiry
window is calculated:

\code
#define     KiStartCoarseTimer(T,I)                                 \
{                                                                   \
    const KernelTicks winSize = ((T)->timeoutPeriod * (I)) / 100;   \
                                                                    \
    KiStartWinTimer((T),                                            \
                    (T)->timeoutPeriod - winSize,                   \
                    (T)->timeoutPeriod + winSize,                   \
                    SIG_TIMER_EXPIRY);                              \
}
\endcode

\note If \a inaccuracy is specified as 0, the started timer will be exact,
      that is to say that calling KiStartCoarseTimer(x, 0) is the same as
      calling KiStartExactTimer(x) or even KiStartTimer(x).

#KiStartWinTimer is similar to #KiStartCoarseTimer, except that in this case
the earliest and latest timer expiry values are explicitly supplied as an
\a earliestExpiry value and a \a latestExpiry value.  It is important to note
that a timer will always be expired at some point between \a earliestExpiry
and \a latestExpiry, and if power saving is not active the \a earliestExpiry
value dictates where a timer will expire, which is important for a timer that
is used in a periodic manner.

When power saving is active, in the worst case a timer may expire after \a
latestExpiry ticks if needed, and power saving maybe prevented to ensure
that the system is awake at that #KernelTick to allow the timer to
expire.  Therefore it is important that the expiry window, either when
expressed as an \a inaccuracy or \a earliestExpiry to \a latestExpiry window
is large enough such that the expiry window covers a period in which the
system would naturally be awake.  This then prevents the timer task from
having to prevent slow clocking in case the expiry window does not cover
a situation where the system would be awake.

The implication is that for GSM systems to operate with good efficiency at
paging rate 9, the timer expiry window for timers in idle mode must be at
least 2119ms (see \ref Background section for calculation).


\section TimerOptimisation Timer Optimisation Techniques
\subsection TimerLogging Logging

A complication of attempting to detect and report timers that are
preventing power saving through slow clocking is that many of the normal
ways in which the system would normally be observed may also affect
slow clocking.  In particular the use of Genie logging or the AT command
interface will prevent or severely disrupt slow clocking, so care must
be taken when attempting to log any data.

The timer task code (in #kiostim.c) provides a number of features that can aid
in the detection of timers that prevent ideal slow clocking, but in general
these features will either #DevAssert or use \a printf to report errant
timers, and it is up to the user to use some logging mechanism that will
satisfactorily capture the data without interfering with slow clocking.

A good technique for capturing timer data is to create a RAM buffer which
can be filled with timer data, which can then be dumped or inspected
by connecting Genie or an AT interface after a profiling run has completed.
Such a mechanism is not provided by TTPCom, but can simply be implemented as
needed, allowing precise configuration to the individual needs of users.


\subsection TimerReporting Reporting Poor Timers

Defining the macro #KI_TIM_DEBUG causes the internal timer structure used by
the timer task to be extended such that it contains some information about
where the call to start the timer originated. It causes the first 7 characters
of the #MODULE_NAME, the file line and an additional unused parameter to be
stored along side the normal timer data, and although this uses a little more
memory and additionally requires additional string copying when starting a
timer, it allows for better reporting as to where in the code an errant timer
was started.

The unused parameter can be used if needed to allow timers that are
started by generic routines to be distinguished.  For example, the
following is the generic function \a L1StartTimer, used to start various
timers used within GSM Layer 1:

\code
  void L1StartTimer (L1TimerNum l1TimerNum)
  {
    L1StopTimer        (l1TimerNum);
    KiStartCoarseTimer (&l1Timer[ l1TimerNum ],
                        cfL1TimerInaccuracyPercentage[ l1TimerNum ]);
  }
\endcode

It can be seen that the MODULE_NAME and line number will not distinguish
timers started here, but that the \a l1TimerNum parameter is of interest as
it defines the timer configuration.  Altering this function and defining
#KI_TIM_DEBUG makes it simpler to distinguish which timers have been started
by this generic function, making it easier to identify exactly the timer that
maybe causing power saving issues:

\code
  void L1StartTimer (L1TimerNum l1TimerNum)
  {
    KernelTicks winSize;

    winSize = (l1Timer[ l1TimerNum ]->timeoutPeriod *
               cfL1TimerInaccuracyPercentage[ l1TimerNum ]) / 100;

    L1StopTimer   (l1TimerNum);
    KiOsStartTimer(&l1Timer[ l1TimerNum ],
                   l1Timer[ l1TimerNum ]->timeoutPeriod - winSize,
                   l1Timer[ l1TimerNum ]->timeoutPeriod + winSize,
                   MODULE_NAME,
                   __LINE__,
                   l1TimerNum);
  }
\endcode
\note Using GKI OS functions directly should only be considered for
      temporary changes used in debugging as the functions may change.
      When developing code, the defined macro interface in the form of
      the #KiStartTimer, #KiStartCoarseTimer, #KiStartWinTimer etc...
      must be used.

This technique can be used anywhere that timers are started through
generic functions where the filename and line number are not enough to
identify a timer.


\subsection TimerMaxSleep KiOsMaximumSleep

This function is called by the slow clocking sub-system before it attempts
to slow clock.  The purpose is for the timer task to indicate whether or
not the sleep period needs to be truncated to allow some timer to expire.

The macro #KI_TIM_DEBUG_POWERSAVING can be defined to cause the timer task
to print details of timers that cause the slow clocking duration to be
reduced.  When defined, the function \a KiOsMaximumSleep will call the macro
#M_KiTimErrantRpt with details of the timer that caused the greatest reduction
in slow clock duration.  When such a timer is found, it is likely that the
timer is either exact, or the expiry window is too small such that the timer
could not be postponed to the next period when the system was awake.

The default expansion of #M_KiTimErrantRpt will additionally pulse LED number
2 on and off when the full slow clock period requested by the power saving
sub-system cannot be met due to timer requirements.  This can be useful for
measuring on an oscilloscope or logic analyser.

\note By default #M_KiTimErrantRpt will expand to a simple printf(), but it
      is intended that the macro be defined to something that will collect
      and log the data in a non-intrusive manner such that slow clocking will
      not be affected by the logging itself.  This macro is only defined if
      not already defined, such that clients may implement the macro in kicfg.h
      if desired.


\subsection TimerExpiry The KiTimExpireTimer Function

This function is internal to the timer task and is called when a timer
has been determined to be ready for expiry.  When #KI_TIM_DEBUG_POWERSAVING
is defined, this function will pulse LED 0 when a coarse timer is expired
and LED 1 when an exact timer is expired, making it possible to log the
expiry pattern of timers using an oscilloscope.

The default expansion of the macro #M_KiTimErrantRpt will additionally pulse
LED 2 when a timer reduces the slow clocking duration passed into
#KiOsMaximumSleep.


\subsection TimerTrap Trapping Poorly Configured Timers

If the macro #KI_TIM_DEBUG_POWERSAVING_TRAP is defined, the system will assert
if a timer is started that fails to meet one of two requirements:

 - The window is too small, but it is not an exact timer.
   i.e. (latestExpiry - earliestExpiry) < 2119ms, but not 0.

 - The earliestExpiry is within 100ms of the present time.

This is designed to trap timers that may end up reducing power consumption by
either preventing slow clocking, or but expiring very rapidly.  In the case of
rapid timers, these may cause more work for the system, and if restarted after
each expiry, are likely to prevent slow clocking all together if their expiry
window is also small.

*/

#endif

/* END OF FILE */

