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
 * GKI header for signal API.
 **************************************************************************/

#ifndef GKISIG_H
#define GKISIG_H

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

#include <pssignal.h>
#include <kitqid.h>
#include <ki_sigbuf.h>

#if defined (KI_MEMORY_DEBUG) && (defined (ON_PC) || defined (GX2))
# define KI_MEM_DBG_PC_EXTRA_ARGS_DEF       , const char *file, int line
# define KI_MEM_DBG_PC_EXTRA_ARGS_USE       , MODULE_NAME, __LINE__
# define KI_MEM_DBG_PC_EXTRA_ARGS_PASS      , file, line
#else
# define KI_MEM_DBG_PC_EXTRA_ARGS_DEF
# define KI_MEM_DBG_PC_EXTRA_ARGS_USE
# define KI_MEM_DBG_PC_EXTRA_ARGS_PASS
#endif

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/** Initialisation value for a #SignalBuffer.
 * All signal buffers must be initialised with this value before use with any
 * of the signal functions. */
extern const SignalBuffer kiNullBuffer;

/***************************************************************************
 * Kernel Function redirections
 **************************************************************************/

/** Create a GKI signal from a task.
 * This function is used to allocate memory for a new GKI signal from a
 * task context. The input signal buffer contents must be equal to
 * #kiNullBuffer (#KiSendSignal and #KiDestroySignal set the passed
 * signal buffer ot kiNullBuffer so this normally only needs to be done
 * explicitly when first initialising the #SignalBuffer). When assertions
 * are being check the system will assert if there is not enough memory
 * to allocate the signal. If assertions are not being checked the
 * function will wait for memory to become available. The memory used
 * (fixed block or dynamic heap) for the signal depends on the value
 * of the #KI_DYNAMIC_MEM_SIG_THRESHOLD define.
 *
 * \param[in] S The #SignalId of the signal to create.
 * \param[in] Z The size of the signal body in bytes (max 0xFFFF).
 * \param[out] B A pointer to a SignalBuffer to be filled in. */
#define     KiCreateSignal(S,Z,B)         KiOsCreateSignal (S,Z,B KI_MEM_DBG_PC_EXTRA_ARGS_USE)

/** Create a GKI signal from a task with the body memory set to zero.
 * This function is basically the same as #KiCreateSignal() except that
 * it sets the signal body contents to zero.
 *
 * \param[in] S The #SignalId of the signal to create.
 * \param[in] Z The size of the signal body in bytes (max 0xFFFF).
 * \param[out] B A pointer to a SignalBuffer to be filled in. */
#define     KiCreateZeroSignal(S,Z,B)     KiOsCreateZeroSignal (S,Z,B KI_MEM_DBG_PC_EXTRA_ARGS_USE)

/** Create a GKI signal from an interrupt.
 * This function is basically the same as #KiCreateSignal() excpet that
 * is may use different RTOS functions intended for use from an
 * interrupt. All signals created using this function will use fixed
 * block memory no matter what the setting of #KI_DYNAMIC_MEM_SIG_THRESHOLD
 * is.
 *
 * \param[in] S The #SignalId of the signal to create.
 * \param[in] Z The size of the signal body in bytes (max 0xFFFF).
 * \param[out] B A pointer to a SignalBuffer to be filled in. */
#define     KiCreateIntSignal(S,Z,B)      KiOsCreateIntSignal (S,Z,B KI_MEM_DBG_PC_EXTRA_ARGS_USE)

/** Create a GKI signal from a task using request priority memory.
 * This function is basically the same as #KiCreateSignal() except
 * the memory for the signal is only allocated if the request limit
 * for the memory pool being used will not be exceeded. This function
 * will return a #Boolean to indicate whether it suceeded or failed.
 *
 * \param[in] S The #SignalId of the signal to create.
 * \param[in] Z The size of the signal body in bytes (max 0xFFFF).
 * \param[out] B A pointer to a SignalBuffer to be filled in.
 *
 * \returns A #Boolean value indicating whether the required memory
 *              could be allocated (TRUE) or not (FALSE). */
#define     KiRequestSignal(S,Z,B)        KiOsRequestSignal (S,Z,B KI_MEM_DBG_PC_EXTRA_ARGS_USE)

/** Create a GKI signal from a task using request priority memory
 * with the body memory set to zero.
 * This function is basically the same as #KiRequestSignal() except that
 * it sets the signal body contents to zero.
 *
 * \param[in] S The #SignalId of the signal to create.
 * \param[in] Z The size of the signal body in bytes (max 0xFFFF).
 * \param[out] B A pointer to a SignalBuffer to be filled in.
 *
 * \returns A #Boolean value indicating whether the required memory
 *              could be allocated (TRUE) or not (FALSE). */
#define     KiRequestZeroSignal(S,Z,B)    KiOsRequestZeroSignal (S,Z,B KI_MEM_DBG_PC_EXTRA_ARGS_USE)

/** Destroy a GKI signal from a task.
 * This function destroys the memory associated with the given SignalBuffer
 * from a task context.
 *
 * \param[in] B A pointer to the #SignalBuffer indicating the signal to be
 *                  destroyed. If the SignalBuffer is set to #kiNullBuffer
 *                  then nothing will be done (and no error will be raised).
 *                  The SignalBuffer will be set to kiNullBuffer on exit
 *                  from this function. */
#define     KiDestroySignal(B)            KiOsDestroySignal (B KI_MEM_DBG_PC_EXTRA_ARGS_USE)

/** Destroy a GKI signal from an interrupt.
 * This function destroys the memory associated with the given SignalBuffer
 * from an interrupt context.
 *
 * \param[in] B A pointer to the #SignalBuffer indicating the signal to be
 *                  destroyed. If the SignalBuffer is set to #kiNullBuffer
 *                  then nothing will be done (and no error will be raised).
 *                  The SignalBuffer will be set to kiNullBuffer on exit
 *                  from this function. */
#define     KiDestroyIntSignal(B)         KiOsDestroyIntSignal (B KI_MEM_DBG_PC_EXTRA_ARGS_USE) 

/** Send a GKI signal to a task from a task.
 * This function sends a GKI signal from a task context to another task.
 * This function may trigger a task switch if the destination task is of
 * higher priority than the calling task.
 *
 * \param[in] T The #KiTaskId for the task to send to.
 * \param[in] B A pointer to the #SignalBuffer representing the signal to
 *                  send. The function will assert if this is set to
 *                  #kiNullBuffer. Once the function returns the
 *                  SignalBuffer will be set to kiNullBuffer. */
#define     KiSendSignal(T,B)             KiOsSendSignal (T,B)

/** Send a GKI signal to a task from an interrupt.
 * This function sends a GKI signal from an interrupt context to a task.
 * This function may trigger a task switch after all nested interrupts
 * have completed if the destination task is of higher priority than
 * the task that was interrupted.
 *
 * \param[in] T The #KiTaskId for the task to send to.
 * \param[in] B A pointer to the #SignalBuffer representing the signal to
 *                  send. The function will assert if this is set to
 *                  #kiNullBuffer. Once the function returns the
 *                  SignalBuffer will be set to kiNullBuffer. */
#define     KiSendIntSignal(T,B)          KiOsSendIntSignal (T,B)

/** Wait for a GKI signal to be received.
 * This function is used by a GKI task to wait for a GKI signal. This
 * function will block the calling task if no signal is on it's queue.
 * The task will be blocked until a signal is on the queue and this
 * task is the highest priority ready task.
 *
 * \param[in] Q The #KiQueueId for the tasks input queue.
 * \param[out] B A pointer to a #SignalBuffer that the received signal
 *                   will be stored in. This function will assert if
 *                   the SignalBuffer is not equal to #kiNullBuffer on
 *                   entry to this function. */
#define     KiReceiveSignal(Q,B)          KiOsReceiveSignal (Q,B)

/** Poll for a GKI signal.
 * This function is used by a GKI task to check if there is a signal on
 * it's input queue. If there is a signal on the queue it will be
 * removed and returned to the calling task. This function does not
 * block the calling task.
 *
 * \param[in] Q The #KiQueueId for the tasks input queue.
 * \param[out] B A pointer to a #SignalBuffer that the received signal
 *                   will be stored in. This function will assert if
 *                   the SignalBuffer is not equal to #kiNullBuffer on
 *                   entry to this function.
 *
 * \returns A #Boolean indicating whether a signal was removed from the
 *              the queue (TRUE) or not (FALSE). */
#define     KiReceiveSignalPoll(Q,B)      KiOsReceiveSignalPoll (Q,B)

/** Poll for a GKI signal from interrupt.
 * This function is used by a GKI task to check if there is a signal on
 * it's input queue. If there is a signal on the queue it will be
 * removed and returned to the calling func.
 *
 * \param[in] Q The #KiQueueId for the tasks input queue.
 * \param[out] B A pointer to a #SignalBuffer that the received signal
 *                   will be stored in. This function will assert if
 *                   the SignalBuffer is not equal to #kiNullBuffer on
 *                   entry to this function.
 *
 * \returns A #Boolean indicating whether a signal was removed from the
 *              the queue (TRUE) or not (FALSE). */
#define     KiReceiveIntSignalPoll(Q,B)      KiOsReceiveIntSignalPoll (Q,B)


/** Get the length of a GKI signal.
 * This function returns the length of the given GKI signals body.
 *
 * \param[in] S A pointer to a #SignalBuffer whoose body length
 *                  is required.
 * \param[out] Z A pointer to an #Int16 in which to store the signal
 *                  body length. */
#define     KiSignalLength(S,Z)           KiOsSignalLength (S,Z)

/** Log a signal without sending it to another task.
 * This allows a signal to be logged without it having to be sent to another
 * task.  The signal will only be logged if a logging filter has been supplied
 * that allows the passed signal to be logged.
 *
 * After logging the signal, it can optionally be destroyed if the \a D
 * parameter is set to TRUE.  In such a case \a S cannot be sent after
 * \a KiLogSignal() has returned
 *
 * \param[in] S  Pointer to a SignalBuffer structure.
 * \param[in] D  Boolean to indicate whether the supplied signal structure
 *                should be destroyed.
 */
#define     KiLogSignal(S,D)              KiOsLogSignal (S,D)

/** Log a signal structure.
 * This allows a signal structure to be logged as though it were a signal,
 * but without the need to allocate a SignalBuffer using KiCreateSignal().
 *
 * This operates in a similar manner to KiLogSignal(), but instead of
 * accepting a SignalBuffer, it takes a raw pointer and the SignalId
 * used to determine whether to log the structure as well as how the
 * structure will be interpreted by Genie.  In some circumstances this
 * can provide a more efficient method of logging since a SignalBuffer
 * does not need to be allocated and filled if the signal structure already
 * exists in memory.
 *
 * \param[in] S  The SignalId of the signal.
 * \param[in] Z  Size of the signal structure.
 * \param[in] B  Pointer to the buffer to be logged.
 */
#define     KiLogSignalStruct(S,Z,B)        KiOsLogSignalStruct (S,Z,B)

/** Check if a signal is active in the logging filter.
 * Check whether the given signal id is enabled in the current logging
 * filter. If ROM_TEST_TASK is not defined this will always return FALSE
 * as the logging system is not present. This function is useful if the
 * logging of data requires extra code, this extra code can now be
 * conditional on whether the signal is in the filter or not.
 *
 * \param[in] S The SignalId to check against the filter.
 * \return  TRUE if the signal is selected for logging in the filter,
 *            otherwise FALSE to indicate that the signal will not
 *            be logged.
 */
#define     KiIsSignalInFilter(S)           KiOsIsSignalInFilter (S)

#if defined (KI_ENABLE_LOGGING_SIGNALS_OVER_HSL)
#define     KiIsSignalInFilterHsl(S)      KiOsIsSignalInFilterHsl (S)
#endif
/***************************************************************************
 * Function prototypes
 **************************************************************************/

void       KiOsDestroySignal       (SignalBuffer        *signal KI_MEM_DBG_PC_EXTRA_ARGS_DEF);

void       KiOsCreateSignal        (SignalId             signalType,
                                    Int16                signalSize,
                                    SignalBuffer        *signal KI_MEM_DBG_PC_EXTRA_ARGS_DEF);

void       KiOsCreateZeroSignal    (SignalId             signalType,
                                    Int16                signalSize,
                                    SignalBuffer        *signal KI_MEM_DBG_PC_EXTRA_ARGS_DEF);

Boolean    KiOsRequestSignal       (SignalId             signalType,
                                    Int16                signalSize,
                                    SignalBuffer        *signal KI_MEM_DBG_PC_EXTRA_ARGS_DEF);

Boolean    KiOsRequestZeroSignal   (SignalId             signalType,
                                    Int16                signalSize,
                                    SignalBuffer        *signal KI_MEM_DBG_PC_EXTRA_ARGS_DEF);

void       KiOsSendSignal          (TaskId               destTask,
                                    SignalBuffer        *signal);

void       KiOsReceiveSignal       (QueueId              myInputQueue,
                                    SignalBuffer        *signal);

Boolean    KiOsReceiveSignalPoll   (QueueId              queue,
                                    SignalBuffer        *signal);

void       KiOsCreateIntSignal     (SignalId             signalType,
                                    Int16                signalSize,
                                    SignalBuffer        *signal KI_MEM_DBG_PC_EXTRA_ARGS_DEF);

void       KiOsSendIntSignal       (TaskId               destTask,
                                    SignalBuffer        *signal);

void       KiOsDestroyIntSignal    (SignalBuffer        *signal KI_MEM_DBG_PC_EXTRA_ARGS_DEF);

void       KiOsSignalLength        (SignalBuffer        *signal,
                                    Int16               *length);

void       KiOsLogSignal           (SignalBuffer        *signalToLog,
                                    Boolean              destroySignal);

void       KiOsLogSignalStruct     (SignalId             signalId,
                                    Int16                signalSize,
                                    void                *signal);

Boolean    KiOsIsSignalInFilter    (SignalId             signalId);
#if defined (KI_ENABLE_LOGGING_SIGNALS_OVER_HSL)
Boolean    KiOsIsSignalInFilterHsl (SignalId             signalId);
#endif

#define STATE_INDEX_TASK_MASK (0x00FFFFFF)
#define STATE_INDEX_MASK (0xFF000000)
#define STATE_INDEX_SHIFT (24)

#define M_InjectStateIndex(tASKiD, sTATEiNDEX)  (tASKiD)
#define M_ExtractStateIndex(tASKiD) (0)
#define M_ExtractTaskId(tASKiD)  (tASKiD)
#define STATE_INDEX_MAX_VALID_VALUE  0xFF
#if defined (__cplusplus)
}
#endif

/** @} */ /* End of PrdGki group */

#endif

/* END OF FILE */
