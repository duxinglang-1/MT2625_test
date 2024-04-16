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
 * GKI header file for statistics functions.
 **************************************************************************/

#ifndef KISTAT_H
#define KISTAT_H

/** \addtogroup PrdGki
 * @{
 */

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#include <system.h>
#include <pssignal.h>
#include <kitqid.h>
#include <ki_typ.h>
#include <ki_sig.h>

/***************************************************************************
 * Config
 **************************************************************************/

/** \def KI_STATISTICS
 * Enable statistics gathering within the GKI.
 * This conditional enables the statistics gathering code within the GKI.
 * This allows statistics on Task and Interrupt stack usage, memory pool
 * usage and timer usage to be collected.
 *
 * The information gathered can be extracted by sending a #KiStatisticsReq
 * signal into the system. This will cause a #KiStatisticsInd signal to be
 * filled in and logged. This signal can also triggered from the target by
 * calling the #M_KiSendStatisticsInd macro. */

#if defined (KI_ENABLE_OWN_QUEUEING)
/** \def KI_QUEUE_STATISTICS
 * Enable queue statistics.
 * This conditional enables statistics for the task signal queues. This is
 * only available if #KI_ENABLE_OWN_QUEUEING is defined. The GKI monitors
 * the maximum number of signals on a queue and the current number of
 * signals.
 *
 * The information gathered can be extracted by sending a #KiQueueStatsReq
 * signal into the system. This will cause a #KiQueueStatsInd signal to be
 * filled in and logged. This signal can also triggered from the target by
 * calling the #M_KiSendQueueStatsInd macro. */
#endif

/** \def KI_MEM_POOL_STATISTICS
 * This conditional enables fixed size block memory pool statistics to be
 * collected. The #KI_STATISTICS conditional must also be enabled.
 * This provides information on the size of allocations made within
 * the fixed size block pools.
 *
 * The information gathered can be stracted by sending a
 * #KiMemPoolStatisticsReq signal into the system. This will cause a
 * #KiMemPoolStatisticsCnf signal to be filled in and logged. This
 * signal can also be triggered from the target by calling the
 * #M_KiSendMemPoolStatisticsCnf macro. */

#if defined (KI_MEM_POOL_STATISTICS)
# if !defined (KI_STATISTICS)
#  error Need KI STATISTICS enabled to use KI MEM POOL STATISTICS
# endif
#endif

/***************************************************************************
 * Externals
 **************************************************************************/
#if defined(MIPS_LOADING_INFO)
extern MipsStatMode KiMipsStatMode;
extern TaskId       KiMipsRequestTaskId;

extern void         KiInitialiseMipsStatistics (void);
extern void         KiSendMipsStatisticsInd (void);
#endif /* MIPS_LOADING_INFO */

/****************************************************************************
 * Macros
 ****************************************************************************/

/** Send #KiStatisticsInd to AL_TASK_ID.
 * \deprecated This macro should not be used anymore, use
 *                 #M_KiSendStatisticsInd instead. */
#if defined (KI_STATISTICS)
# define KiSendStatistics()                                     KiSendStatisticsInd (AL_TASK_ID)
#else
# define KiSendStatistics()
#endif

/** Send #KiStatisticsInd to a task.
 * This macro will create, fill in and send a #KiStatisticsInd
 * signal to the given task. This will only have any affect if
 * #KI_STATISTICS is defined.
 *
 * \param[in] dESTtASK GKI task id indicating the task to send the
 *                #KiStatisticsInd signal to. */
#if defined (KI_STATISTICS)
# define M_KiSendStatisticsInd(dESTtASK)                        KiSendStatisticsInd (dESTtASK)
#else
# define M_KiSendStatisticsInd(dESTtASK)
#endif

/** Send #KiFixStatisticsInd to a task.
 * This macro will create, fill in and send a #KiFixStatisticsInd
 * signal to the given task. This will only have any affect if
 * #KI_STATISTICS is defined.
 *
 * \param[in] dESTtASK GKI task id indicating the task to send the
 *                #KiFixStatisticsInd signal to. */
#if defined (KI_STATISTICS)
#  define M_KiSendFixStatisticsInd(dESTtASK)                    KiSendFixStatisticsInd (dESTtASK)
#else
#  define M_KiSendFixStatisticsInd(dESTtASK)
#endif

/** Initialise the statistics data.
 * This is called by #KiReset to initialise the GKI statistics system.
 * This must not be called by any other code. */
#if defined (KI_STATISTICS)
# define M_KiInitialiseStatistics()                             KiInitialiseStatistics ()
#else
# define M_KiInitialiseStatistics()
#endif

#if defined (KI_ENABLE_OWN_QUEUEING)
/** Send a #KiQueueStatsInd to a task.
 * This macro will create, fill in and send a #KiQueueStatsInd
 * signal to the given task. This is only available if
 * #KI_ENABLE_OWN_QUEUEING is defined.
 *
 * \param[in] dESTtASK GKI task id indicating the task to send the
 *                #KiQueueStatsInd signal to.*/
# if defined (KI_QUEUE_STATISTICS)
#  define M_KiSendQueueStatsInd(dESTtASK)                       KiSendQueueStatsInd (dESTtASK)
# else
#  define M_KiSendQueueStatsInd(dESTtASK)
# endif
#endif

/** Send a #KiMemPoolStatisticsCnf to a task.
 * This macro can be used to create, fill in and send a
 * #KiMemPoolStatisticsCnf to a given task. This will only have an
 * effect if #KI_MEM_POOL_STATISTICS is defined.
 *
 * \param[in] kImEMpOOLsTATSrEQpTR A pointer to a #KiMemPoolStatisticsReq
 *                structure providing the required configuration. */
#if defined (KI_MEM_POOL_STATISTICS)
# define M_KiSendMemPoolStatisticsCnf(kImEMpOOLsTATSrEQpTR)     KiSendMemPoolStatisticsCnf (kImEMpOOLsTATSrEQpTR)
#else
# define M_KiSendMemPoolStatisticsCnf(kImEMpOOLsTATSrEQpTR)
#endif

#if defined(MIPS_LOADING_INFO)
# define M_KiInitialiseMipsStatistics()                         KiInitialiseMipsStatistics ()
# define M_KiSendMipsStatisticsInd()                            KiSendMipsStatisticsInd ()
# define M_StartPlpProfiling()                                  M_FrPkiSendPlpXswiSignal (PKI_ACTION_ID_PROFILING_START, \
                                                                                          PNULL);
# define M_StopPlpProfiling()                                   M_FrPkiSendPlpXswiSignal (PKI_ACTION_ID_PROFILING_STOP, \
                                                                                          PNULL);
#else /* MIPS_LOADING_INFO */
# define M_KiInitialiseMipsStatistics()
# define M_KiSendMipsStatisticsInd()
# define M_StartPlpProfiling()
# define M_StopPlpProfiling()
#endif /* MIPS_LOADING_INFO */

/** Dump memory into a signal.
 * This macro is used to copy memory into a #KiDumpMemCnf
 * signal and send it to the given task.
 *
 * \param[in] kIdUMPmEMrEQpTR A pointer to a #KiDumpMemReq structure
 *                that provides the required configuration. */
#define M_KiHandleDumpMemReq(kIdUMPmEMrEQpTR)                   KiHandleDumpMemReq (kIdUMPmEMrEQpTR)

/** Dump memory into a signal.
 * This macro is used to copy memory into a #KiDumpMemCnf
 * signal and send it to the given task.
 *
 * \param[in] sTARTaDDRESS The address of memory to start the dump from.
 * \param[in] cOUNT The number of bytes to dump.
 * \param[in] rETURNtASK The GKI task id of the task to send the
 *                #KiDumpMemCnf signal to. */
#define M_KiSendDumpMemReq(sTARTaDDRESS, cOUNT, rETURNtASK) KiSendDumpMemReq ((Int32)sTARTaDDRESS, cOUNT, rETURNtASK)

/** Send a #KiDevFailInd signal.
 * This macro is used to send a #KiDevFailInd to the given task. The
 * #KiDevFailInd signal is used to transport assert information when
 * #KI_USE_ASSERT_SIGNALS is defined.
 *
 * \param[in] tASKiD The GKI task id to send the signal to.
 * \param[in] fILE A string providing the file name which contains the assert.
 * \param[in] lINE The line number of the assert.
 * \param[in] tEXT Any text associated with the assert. */
#define M_KiSendDevFailInd(tASKiD, fILE, lINE, tEXT)    KiSendDevWarnStringInd(tASKiD, SIG_KI_DEV_FAIL_IND, (const Int8 *)fILE, lINE, (const Int8 *)tEXT)

/** Send a #KiDevAssertInd signal.
 * This macro is used to send a #KiDevAssertInd to the given task. The
 * #KiDevAssertInd signal is used to transport assert information when
 * #KI_USE_ASSERT_SIGNALS is defined.
 *
 * \param[in] tASKiD The GKI task id to send the signal to.
 * \param[in] fILE A string providing the file name which contains the assert.
 * \param[in] lINE The line number of the assert.
 * \param[in] tEXT Any text associated with the assert. */
#define M_KiSendDevAssertInd(tASKiD, fILE, lINE, tEXT)  KiSendDevWarnStringInd(tASKiD, SIG_KI_DEV_ASSERT_IND, (const Int8 *)fILE, lINE, (const Int8 *)tEXT)

/** Send a #KiDevCheckInd signal.
 * This macro is used to send a #KiDevCheckInd to the given task. The
 * #KiDevCheckInd signal is used to transport assert information when
 * #KI_USE_ASSERT_SIGNALS is defined.
 *
 * \param[in] tASKiD The GKI task id to send the signal to.
 * \param[in] fILE A string providing the file name which contains the assert.
 * \param[in] lINE The line number of the assert.
 * \param[in] tEXT Any text associated with the assert.
 * \param[in] pARAM1 A 32 bit value associated with the assert.
 * \param[in] pARAM2 A 32 bit value associated with the assert.
 * \param[in] pARAM3 A 32 bit value associated with the assert. */
#define M_KiSendDevCheckInd(tASKiD, fILE, lINE, tEXT, pARAM1, pARAM2, pARAM3) KiSendDevWarnParamInd (tASKiD, SIG_KI_DEV_CHECK_IND, (const Int8 *)fILE, lINE, \
                           (const Int8 *)tEXT, (Int32)pARAM1, (Int32)pARAM2, (Int32)pARAM3)

/** Send a #KiWarnFailInd signal.
 * This macro is used to send a #KiWarnFailInd to the given task. The
 * #KiWarnFailInd signal is used to transport warning information when
 * #KI_CHECK_WARNINGS is defined.
 *
 * \param[in] tASKiD The GKI task id to send the signal to.
 * \param[in] fILE A string providing the file name which contains the warning.
 * \param[in] lINE The line number of the warning.
 * \param[in] tEXT Any text associated with the warning. */
#define M_KiSendWarnFailInd(tASKiD, fILE, lINE, tEXT)   KiSendDevWarnStringInd(tASKiD, SIG_KI_WARN_FAIL_IND, (const Int8 *)fILE, lINE, (const Int8 *)tEXT)

/** Send a #KiWarnAssertInd signal.
 * This macro is used to send a #KiWarnAssertInd to the given task. The
 * #KiWarnAssertInd signal is used to transport warning information when
 * #KI_CHECK_WARNINGS is defined.
 *
 * \param[in] tASKiD The GKI task id to send the signal to.
 * \param[in] fILE A string providing the file name which contains the warning.
 * \param[in] lINE The line number of the warning.
 * \param[in] tEXT Any text associated with the warning. */
#define M_KiSendWarnAssertInd(tASKiD, fILE, lINE, tEXT) KiSendDevWarnStringInd(tASKiD, SIG_KI_WARN_ASSERT_IND, (const Int8 *)fILE, lINE, (const Int8 *)tEXT)

/** Send a #KiWarnCheckInd signal.
 * This macro is used to send a #KiWarnCheckInd to the given task. The
 * #KiWarnCheckInd signal is used to transport warning information when
 * #KI_CHECK_WARNINGS is defined.
 *
 * \param[in] tASKiD The GKI task id to send the signal to.
 * \param[in] fILE A string providing the file name which contains the warning.
 * \param[in] lINE The line number of the warning.
 * \param[in] tEXT Any text associated with the warning.
 * \param[in] pARAM1 A 32 bit value associated with the warning.
 * \param[in] pARAM2 A 32 bit value associated with the warning.
 * \param[in] pARAM3 A 32 bit value associated with the warning. */
#define M_KiSendWarnCheckInd(tASKiD, fILE, lINE, tEXT, pARAM1, pARAM2, pARAM3) KiSendDevWarnParamInd (tASKiD, SIG_KI_WARN_CHECK_IND, (const Int8 *)fILE, lINE, \
                           (const Int8 *)tEXT, (Int32)pARAM1, (Int32)pARAM2, (Int32)pARAM3)

/** Send a #KiWarnParamInd signal.
 * This macro is used to send a #KiWarnParamInd to the given task. The
 * #KiWarnParamInd signal is used to transport warning information when
 * #KI_CHECK_WARNINGS is defined.
 *
 * \param[in] tASKiD The GKI task id to send the signal to.
 * \param[in] fILE A string providing the file name which contains the warning.
 * \param[in] lINE The line number of the warning.
 * \param[in] tEXT Any text associated with the warning.
 * \param[in] pARAM1 A 32 bit value associated with the warning.
 * \param[in] pARAM2 A 32 bit value associated with the warning.
 * \param[in] pARAM3 A 32 bit value associated with the warning. */
#define M_KiSendWarnParamInd(tASKiD, fILE, lINE, tEXT, pARAM1, pARAM2, pARAM3) KiSendDevWarnParamInd (tASKiD, SIG_KI_WARN_PARAM_IND, (const Int8 *)fILE, lINE, \
                           (const Int8 *)tEXT, (Int32)pARAM1, (Int32)pARAM2, (Int32)pARAM3)

/** Check for stack overflow.
 * This macro can be called to check whether any of the
 * task stacks have exceeded their nominal allocation. The
 * function checks whether the any stack inside the
 * #DEVELOPMENT_STACKS increase has been used and asserts
 * if it has. Normally this would be called from the lowest
 * priority task in the system. */
#if defined (DEVELOPMENT_VERSION) && defined (DEVELOPMENT_STACKS) && defined (KI_STATISTICS)
#define M_KiCheckStackOverFlow()                                KiCheckStackOverFlow ()
#else
#define M_KiCheckStackOverFlow()
#endif

/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/

void KiHandleDumpMemReq (KiDumpMemReq * kiDumpMemReq);
void KiSendDumpMemReq   (Int32 startAddress, Int16 count, TaskId returnTask);

void KiSendDevWarnStringInd (TaskId taskId, SignalId sigId, const Int8 * file, Int16 line, const Int8 * text);
void KiSendDevWarnParamInd (TaskId taskId, SignalId sigId, const Int8 * file, Int16 line, const Int8 * text,
                            Int32 param1, Int32 param2, Int32 param3);
void KiHandleWarnConfigReq (KiWarnConfigReq * kiWarnConfigReq);


void KiSendFixStatisticsInd     (TaskId destTask);

void KiSendStatisticsInd        (TaskId destTask);
#if defined (KI_ENABLE_OWN_QUEUEING)
void KiSendQueueStatsInd        (TaskId destTask);
#endif
void KiInitialiseStatistics     (void);

void KiSendMemPoolStatisticsCnf (KiMemPoolStatisticsReq * kiMemPoolStatisticsReq);

#if defined (DEVELOPMENT_VERSION) && defined (DEVELOPMENT_STACKS) && defined (KI_STATISTICS)
void KiCheckStackOverFlow (void);
#endif /* DEVELOPMENT_VERSION && DEVELOPMENT_STACKS && KI_STATISTICS*/

/** @} */ /* End of PrdGki group */

#endif
/* END OF FILE */
