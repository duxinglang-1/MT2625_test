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
 * GKI header file that defines the RAM logging API.
 **************************************************************************/

#ifndef KIRLGUTL_H
#define KIRLGUTL_H

/** \addtogroup PrdGkiRlg
 * @{
 */

/***************************************************************************
 * Nested Include Files
 **************************************************************************/
#include <system.h>

#if defined (KI_ENABLE_RAM_LOGGING)

#if defined (TTC_PLATFORM_RPV4_MCU)
/* For the RPV4 MCU specific hardware timer for M_KiRlgReadProfTime */
#include <rp4mhwt.h>
#endif

#if defined (KI_RLG_USE_KERNEL_TIME)
#include <gkitimer.h>
#endif

#include <kirlgtyp.h> /* RAM Logging typedefs */

/***************************************************************************
 * Config
 **************************************************************************/

/** \def KI_RLG_ENABLE_INTERNAL_LOGGING
 * Enables the internal GKI RAM log points.
 * This conditional enables the #M_KiRlgLogEventInternal macro that is used
 * within the GKI to log internal activity not normally seen in a RAM log. */

/** \def KI_RLG_ENABLE_LOG_PROF_STATS
 * Enable additional statistics on profile sets.
 * This switch logs the maximum, minimum and average times of code within
 * a profile set. */

/** \def KI_RLG_USE_KERNEL_TIME
 * Use GKI ticks for RAM log timestamps.
 * This switch enables the use of GKI ticks as the timestamp for RAM Log
 * events. The default timstamp is the system frame number as stored in
 * #l1FrameNumber. The #KI_RLG_USE_PROFILE_TIME option will normally
 * provide timestamp with more accuracy. */

/** \def KI_RLG_USE_PROFILE_TIME
 * Use profile timer for RAM log timestamps.
 * This switch enables the use of the profile timer as the timestamp for
 * RAM Log events. The default timstamp is the system frame number as
 * stored in #l1FrameNumber. */

/***************************************************************************
 * Type Definitions
 **************************************************************************/
/* See kirlgtyp.h */

/***************************************************************************
 * Variables Prototypes
 ***************************************************************************/

extern Int32        l1FrameNumber;

/***************************************************************************
 *  Macros
 **************************************************************************/

/** Get a timestamp for the RAM log.
 * This macro allows RAM logging to use different timer sources for it's
 * timebase (kernel ticks, profile timer or frame ticks - default). */
#if defined (KI_RLG_USE_KERNEL_TIME)
/* Use the GKI ticks as the timebase */
#define M_KiRlgGetTime(tIME)    KiGetRelativeTime (&(tIME))

#elif defined (KI_RLG_USE_PROFILE_TIME)
/* Use the profile timer as the timebase */
#define M_KiRlgGetTime(tIME)    tIME = M_KiRlgReadProfTime ()

#else
/* By default, use the GSM frame number as the timebase */
#define M_KiRlgGetTime(tIME)    tIME = l1FrameNumber

#endif

#if defined (TTC_PLATFORM_RPV4_MCU)
/* There is no 32K timer that can be easily accessed from the RPV4
 * MCU processor so use a general timer that has been set to run
 * at 38750Hz, see rp4mhwt.c. */
#define M_KiRlgReadProfTime() ((Int32)(*((volatile Int16 *)S3C2410X_TCNTO4_REG)))
#endif

#if !defined (M_KiRlgReadProfTime)
extern Int32 ReadRawTime(void);
/* For all other platforms, a call to a generic function is used. */
#define M_KiRlgReadProfTime() ReadRawTime()
#endif

/** Internal macro to for logging a RAM log event.
 * This macro is used to access the #KiRlgLogEvent() function. */
#define M_KiRlgLogEvent( eVENT, vAL1, vAL2, vAL3) \
          KiRlgLogEvent (eVENT, (Int32) vAL1, (Int32) vAL2, (Int32) vAL3)

#if defined (KI_RLG_ENABLE_TASK_TRACKING)
/** Task enter/exit logging.
 * This macro is used to track the entry to, and exit from a task. */
#define M_KiRlgLogTaskEnterExit( eVENT, sIGNALiD)                 \
{                                                                 \
    TaskId tHIStASK = KiThisTask ();                              \
    KiRlgLogEvent (eVENT, tHIStASK, sIGNALiD, 0);                 \
    KiRlgLogTaskEnterExit (eVENT, tHIStASK, (SignalId) sIGNALiD); \
}
#else
#define M_KiRlgLogTaskEnterExit( eVENT, sIGNALiD)                 \
{                                                                 \
    KiRlgLogEvent (eVENT, KiThisTask (), sIGNALiD, 0);            \
}
#endif

#if !defined (KI_RLG_OUTPUT_PRINTF_LOG_POINTS_ONLY)
/** Log an event without calling KiRlgLogEvent.
 * This macro logs an event directly and assumes that interrupts are
 * already disabled. */
#define M_KiRlgLogEventFast( eVENT, vAL1, vAL2, vAL3)                 \
{                                                                     \
    KiRlgGkeRecord * nEWrECORD = KiRlgGetFreeGkeRecord();             \
    nEWrECORD->event = eVENT;                                         \
    nEWrECORD->val1  = (Int32)(vAL1);                                 \
    nEWrECORD->val2  = (Int32)(vAL2);                                 \
    nEWrECORD->val3  = (Int32)(vAL3);                                 \
}
#else
#define M_KiRlgLogEventFast( eVENT, vAL1, vAL2, vAL3)
#endif

/** Profile start macro.
 * This macro logs entry into code to be profiled. This should be matched
 * with a call to #M_KiRlgLogProfStop on exit from the code to be profiled. */
#if defined (KI_RLG_ENABLE_LOG_PROF_STATS)
#define M_KiRlgLogProfStart( pROFiD, uSERvAL)                             \
{                                                                         \
    Int32 tIMERrEADING = M_KiRlgReadProfTime ();                          \
    KiRlgLogEvent (KI_RLG_GKE_PROF_START, pROFiD, uSERvAL, tIMERrEADING); \
    KiRlgUpdateProfStats (KI_RLG_GKE_PROF_START, pROFiD, tIMERrEADING);   \
}
#else
#define M_KiRlgLogProfStart( pROFiD, uSERvAL)  \
    KiRlgLogEvent (KI_RLG_GKE_PROF_START, pROFiD, uSERvAL, M_KiRlgReadProfTime ());
#endif

/** Profile stop macro.
 * This macro logs exit from code to be profiled. This should be matched
 * with a call to #M_KiRlgLogProfStart on entry to the code to be profiled. */
#if defined (KI_RLG_ENABLE_LOG_PROF_STATS)
#define M_KiRlgLogProfStop( pROFiD, uSERvAL)                            \
{                                                         \
    Int32 tIMERrEADING = M_KiRlgReadProfTime ();          \
    KiRlgLogEvent (KI_RLG_GKE_PROF_END, pROFiD, uSERvAL, tIMERrEADING); \
    KiRlgUpdateProfStats (KI_RLG_GKE_PROF_END, pROFiD, tIMERrEADING);   \
}

#else
#define M_KiRlgLogProfStop( pROFiD, uSERvAL)  \
    KiRlgLogEvent (KI_RLG_GKE_PROF_END, pROFiD, uSERvAL, M_KiRlgReadProfTime ());
#endif

/** Dump memory to the RAM log.
 * This macro outputs the contents of a block of memory to the RAM log. */
#define M_KiRlgLogMemDump( aDDRESS_P, sIZE)                             \
    KiRlgLogEvent (KI_RLG_GKE_MEM_DUMP, (Int32) aDDRESS_P, sIZE, 0);


/** Trigger dumping of the RAM log.
 * This macro is used to access the #KiRlgTriggerLogDump() function to
 * output the RAM logging data. RAM logging data is not dumped if
 * interactive logging is enabled. */
#if !defined (KI_ENABLE_INTERACTIVE_LOGGING)
#define M_KiRlgTriggerLogDump() \
          KiRlgTriggerLogDump()
#endif

#if defined (KI_RLG_ENABLE_TCU_LOGGING)
/** Log a TCU command write.
 * This macro is used to access the #KiRlgLogTcuWrite() function. It is used
 * to log commands being written to the Timing Control Unit (TCU). */
#define M_KiRlgLogTcuWrite( cODE, tIME, tDQ_P) \
          KiRlgLogTcuWrite (cODE, tIME, tDQ_P)

/** Log a TCU event.
 * This macro logs a TCU event directly and assumes that interrupts are
 * already disabled. */
#define M_KiRlgLogTcuEvent( eVENT, vAL1, vAL2, vAL3)               \
{                                                                     \
    KiRlgTcuRecord * nEWrECORD = KiRlgGetFreeTcuRecord();             \
    nEWrECORD->event = eVENT;                                         \
    nEWrECORD->val1  = (Int32)(vAL1);                                 \
    nEWrECORD->val2  = (Int32)(vAL2);                                 \
    nEWrECORD->val3  = (Int32)(vAL3);                                 \
}
#endif /* KI_RLG_ENABLE_TCU_LOGGING */

/** Initialise the RAM logging system.
 * Initialise the RAM logging data on system start. */
#define M_KiRlgInit() \
          KiRlgInit()

/** Log assert information.
 * Logs the assert fail information into the RAM log. */
#define M_KiRlgLogAssertFail( fILE, lINE, cOND, cOUNT) \
{ \
    /* Disable interrupts so that we are not interrupted when the
     * index is incremented and checked */ \
    KiDisableInterrupts (); \
 \
    /* Log the filename and line number. Clear flag to indicate
     * that debug vales are not available */ \
    M_KiRlgLogEventFast (KI_RLG_GKE_ASSERT_FAIL, lINE, (Int32) fILE, FALSE); \
 \
    /* Log the assert condition, assert count and the time of the assert */ \
    M_KiRlgLogEventFast (KI_RLG_GKE_ASSERT_INFO, (Int32) cOND, cOUNT, M_KiRlgReadProfTime ()); \
 \
    /* Re-enable the interrupts */ \
    KiEnableInterrupts (); \
}

/** Log DevCheck information.
 * Logs the DevCheck information into the RAM log. */
#define M_KiRlgLogDevCheck( fILE, lINE, cOND, cOUNT, v1, v2, v3) \
{ \
    /* Disable interrupts so that we are not interrupted when the
     * index is incremented and checked */ \
    KiDisableInterrupts (); \
 \
    /* Log the filename and line number. Set flag to indicate
     * that debug values are available */ \
    M_KiRlgLogEventFast (KI_RLG_GKE_ASSERT_FAIL, lINE, (Int32) fILE, TRUE); \
 \
    /* Log the debug values */ \
    M_KiRlgLogEventFast (KI_RLG_GKE_ASSERT_VAL, v1, v2, v3); \
 \
    /* Log the assert condition, assert count and the time of the assert */ \
    M_KiRlgLogEventFast (KI_RLG_GKE_ASSERT_INFO, (Int32) cOND, cOUNT, M_KiRlgReadProfTime ()); \
 \
    /* Re-enable the interrupts */ \
    KiEnableInterrupts (); \
}

/** Log WarnFail information.
 * Logs the SysWarnFail information into the RAM log. */
#define M_KiRlgLogWarnFail( fILE, lINE, cOND) \
{ \
    /* Disable interrupts so that we are not interrupted when the
     * index is incremented and checked */ \
    KiDisableInterrupts (); \
 \
    /* Log the filename and line number. Clear flag to indicate
     * that debug vales are not available */ \
    M_KiRlgLogEventFast (KI_RLG_GKE_WARN_FAIL, lINE, (Int32) fILE, FALSE); \
 \
    /* Log the assert condition and the time of the warning */ \
    M_KiRlgLogEventFast (KI_RLG_GKE_WARN_INFO, (Int32) cOND, 0, M_KiRlgReadProfTime ()); \
 \
    /* Re-enable the interrupts */ \
    KiEnableInterrupts (); \
}

/** Log WarnParam information.
 * Logs the SysWarnParam information into the RAM log. */
#define M_KiRlgLogWarnParam( fILE, lINE, cOND, v1, v2, v3) \
{ \
    /* Disable interrupts so that we are not interrupted when the
     * index is incremented and checked */ \
    KiDisableInterrupts (); \
 \
    /* Log the filename and line number. Set flag to indicate
     * that debug values are available */ \
    M_KiRlgLogEventFast (KI_RLG_GKE_WARN_FAIL, lINE, (Int32) fILE, TRUE); \
 \
    /* Log the debug values */ \
    M_KiRlgLogEventFast (KI_RLG_GKE_WARN_VAL, v1, v2, v3); \
 \
    /* Log the assert condition and the time of the assert */ \
    M_KiRlgLogEventFast (KI_RLG_GKE_WARN_INFO, (Int32) cOND, 0, M_KiRlgReadProfTime ()); \
 \
    /* Re-enable the interrupts */ \
    KiEnableInterrupts (); \
}

#if defined (KI_RLG_ENABLE_OUTPUT_AFTER_RESET)
/** Indicate normal power down.
 * This macro is used to access the #KiRlgNormalPowerDown() function. This
 * tells the RAM logging system that this is a normal power down rather than
 * a system crash. */
#define M_KiRlgNormalPowerDown() \
          KiRlgNormalPowerDown ()
#endif /* KI_RLG_ENABLE_OUTPUT_AFTER_RESET */

/** Check whether RAM logging has beein initialised. */
#define M_KiRlgHasInitialised() KiRlgHasInitialised()

/** Display a line of text.
 * Displays a line on the LCD. This can be defined to an empty macro if we
 * want to shield indications that RAM logging is active to the end user. */
#if !defined (KI_RLG_DISABLE_LCD_INFO)
#define M_KiRlgDisplayOnLcd( mSG) KiRlgLcdDisplayLine ( mSG)
#endif

#if defined (KI_RLG_ENABLE_INTERNAL_LOGGING)
/** Log internal GKI events.
 * This macro is used to log internal GKI events that would not normally be
 * present in a RAM log. */
#define M_KiRlgLogEventInternal( eVENT, vAL1, vAL2, vAL3) \
        M_KiRlgLogEvent( (eVENT), (Int32) (vAL1), (Int32) (vAL2), (Int32) (vAL3))
#endif

#if defined (KI_RLG_OUTPUT_LOG_TO_FS)
/** Check whether RAM log is being written to the File system while erasing.
 * This macro is used at the start of a {} block. It executes the contents
 * of the block if RAM logging is not writing a RAM log to the file system.
 * Otherwise, if it is writing a RAM log to the file system, then it needs
 * to run code to finalise an erase. */
#define M_If_KiRlgToFsIsNotActive_Erase()                      \
    if (KiRlgFsRamLogIsActive ())                              \
    {                                                          \
        /* If RAM logging is writing data to the file system,
         * then wait and check the operation status. */        \
        while (flashStateMachine == NVRAM_ERASE_IN_PROGRESS)   \
        {                                                      \
            KiRlgFsCheckNvEraseFlashOp ();                     \
        }                                                      \
    }                                                          \
    else

/** Check whether RAM log is being written to the File system.
 * This macro is used at the start of a {} block. It executes the contents
 * of the block if RAM logging is not writing a RAM log to the file system.
 * This macro calls #KiRlgFsSetFsLockUnlock() to determine if the file
 * system is within a critical region of the code. If an assert occurs
 * within this region, then the RAM log should not be written to the file
 * system.
 *
 * \param[in] lOCK A Flag to indicate if we have entered (TRUE) or
 *                exited (FALSE) a locked region. */
#define M_If_KiRlgToFsIsNotActive_Lock( lOCK) \
    if (!KiRlgFsSetFsLockUnlock (lOCK))

#endif /* KI_RLG_OUTPUT_LOG_TO_FS */


/** Fast RAM log printf.
 * This macro is used to access the #KiRlgFastPrintf() function. Please
 * read the comments above the #KiRlgFastPrintf() function for limitations
 * when calling this function. This function has been profiled to be 4.2x
 * quicker than the #KiRlgSlowPrintf() function on PC, so it is preferable to
 * call M_KiRlgFastPrintf() where possible. This macro does not require
 * double parentheses when it is being used. i.e:
 *
 *    M_KiRlgFastPrintf ("Hello", 0, 0);
 */
#define M_KiRlgFastPrintf( fORMAT, aRG1, aRG2) \
{ \
    /* This assignment checks that the fORMAT passed into the
     * macro is a constant pointer */ \
    static const char *format_p = fORMAT; \
    KiRlgFastPrintf (format_p, (Int32) aRG1, (Int32) aRG2); \
}

/** Slow RAM log printf.
 * This macro is used to access the #KiRlgSlowPrintf() function when RAM
 * logging is enabled. This macro must be called with double
 * parentheses. i.e.
 *
 *              M_KiRlgSlowPrintf (("Hello"));
 */
#define M_KiRlgSlowPrintf( x) KiRlgSlowPrintf x

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
#if defined (KI_RLG_ENABLE_OUTPUT_AFTER_RESET)
extern Boolean KiRlgRamLogIsAvailable (void);
#endif

extern void KiRlgLogEvent (KiRlgEvent event, Int32 val1, Int32 val2, Int32 val3);

extern KiRlgGkeRecord * KiRlgGetFreeGkeRecord(void);

#if defined (KI_RLG_ENABLE_TASK_TRACKING)
extern void KiRlgLogTaskEnterExit (KiRlgEvent event, TaskId taskId, SignalId sigId);
#endif

#if defined (KI_RLG_ENABLE_TCU_LOGGING)
extern KiRlgTcuRecord * KiRlgGetFreeTcuRecord(void);
extern void KiRlgLogTcuWrite (Int16 code, Int32 deltaTime, void * ads_p);
#endif /* KI_RLG_ENABLE_TCU_LOGGING */

#if defined (KI_RLG_ENABLE_LOG_PROF_STATS)
extern void KiRlgUpdateProfStats (KiRlgEvent event, Int32 profId, Int32 timerReading);
#endif

extern void KiRlgTriggerLogDump (void);

extern void KiRlgInit (void);

#if defined (KI_RLG_ENABLE_OUTPUT_AFTER_RESET)
extern void KiRlgNormalPowerDown (void);
#endif

extern void KiRlgOutputString (const char *format_p, ...);

extern Boolean KiRlgHasInitialised(void);

#if !defined (KI_RLG_DISABLE_LCD_INFO)
extern void KiRlgLcdDisplayLine (char *line_p);
#endif

#if defined (KI_RLG_OUTPUT_LOG_TO_FS)
extern Boolean KiRlgFsRamLogIsActive (void);
extern Boolean KiRlgFsSetFsLockUnlock (Boolean lock);
#if defined (USE_FLASH_NVRAM)
extern void KiRlgFsCheckNvEraseFlashOp (void);
#endif
#endif

extern void KiRlgFastPrintf (const char *format_p, Int32 arg1, Int32 arg2);
extern int KiRlgSlowPrintf (const char *format_p, ...);

extern void KiRlgOutputDataToDevice (Int8 *data_p, Int32 dataSize);

#if defined (KI_RLG_OUTPUT_PROCESSOR_SPECIFIC_DATA)
extern void KiRlgStoreProcessorSpecificData (void);
#endif

#endif /* KI_ENABLE_RAM_LOGGING */


/***************************************************************************
 *  Default empty macro definitions.
 **************************************************************************/
#if !defined M_KiRlgLogEventFast
#define M_KiRlgLogEventFast(eVENT, vAL1, vAL2, vAL3)
#endif

#if !defined M_KiRlgLogAssertFail
#define M_KiRlgLogAssertFail( fILE, lINE, cOND, cOUNT)
#endif

#if !defined M_KiRlgLogDevCheck
#define M_KiRlgLogDevCheck( fILE, lINE, cOND, cOUNT, v1, v2, v3)
#endif

#if !defined M_KiRlgLogWarnFail
#define M_KiRlgLogWarnFail( fILE, lINE, cOND)
#endif

#if !defined M_KiRlgLogWarnParam
#define M_KiRlgLogWarnParam( fILE, lINE, cOND, v1, v2, v3)
#endif

#if !defined M_KiRlgLogEvent
#define M_KiRlgLogEvent( eVENT, vAL1, vAL2, vAL3)
#endif

#if !defined M_KiRlgLogTaskEnterExit
#define M_KiRlgLogTaskEnterExit( eVENT, sIGNALiD)
#endif

#if !defined M_KiRlgLogTcuWrite
#define M_KiRlgLogTcuWrite( cODE, tIME, tDQ_P)
#endif

#if !defined M_KiRlgLogTcuEvent
#define M_KiRlgLogTcuEvent( eVENT, vAL1, vAL2, vAL3)
#endif

#if !defined M_KiRlgInit
#define M_KiRlgInit()
#endif

#if !defined M_KiRlgLogLpTaskEntry
#define M_KiRlgLogLpTaskEntry()
#endif

#if !defined M_KiRlgTriggerLogDump
#define M_KiRlgTriggerLogDump()
#endif

#if !defined M_KiRlgLogCallerAddress
#define M_KiRlgLogCallerAddress(eVENT, aDDRESS)
#endif

#if !defined M_KiRlgNormalPowerDown
#define M_KiRlgNormalPowerDown()
#endif

#if !defined M_KiRlgReadProfTime
#define M_KiRlgReadProfTime()
#endif

#if !defined M_KiRlgDisplayOnLcd
#define M_KiRlgDisplayOnLcd( mSG)
#endif

#if !defined M_KiRlgInitLcd
#define M_KiRlgInitLcd()
#endif

#if !defined M_KiRlgLogEventInternal
#define M_KiRlgLogEventInternal( eVENT, vAL1, vAL2, vAL3)
#endif

#if !defined M_KiRlgHasInitialised
#define M_KiRlgHasInitialised() FALSE
#endif

#if !defined M_If_KiRlgToFsIsNotActive_Erase
#define M_If_KiRlgToFsIsNotActive_Erase()
#endif

#if !defined M_If_KiRlgToFsIsNotActive_Lock
#define M_If_KiRlgToFsIsNotActive_Lock( lOCK)
#endif

#if !defined M_KiRlgFastPrintf
#define M_KiRlgFastPrintf( fORMAT, aRG1, aRG2)
#endif

#if !defined M_KiRlgSlowPrintf
#define M_KiRlgSlowPrintf( x)
#endif

#if !defined M_KiRlgLogProfStart
#define M_KiRlgLogProfStart( pROFiD, uSERvAL)
#endif

#if !defined M_KiRlgLogProfStop
#define M_KiRlgLogProfStop( pROFiD, uSERvAL)
#endif

#if !defined M_KiRlgLogMemDump
#define M_KiRlgLogMemDump( aDDRESS_P, sIZE)
#endif

/** @} */ /* End of PrdGki group */

#endif /* END OF FILE */
