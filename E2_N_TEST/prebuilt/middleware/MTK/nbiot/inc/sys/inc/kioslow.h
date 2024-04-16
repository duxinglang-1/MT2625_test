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
 * GKI header for low level functions and types.
 **************************************************************************/

#ifndef KIOSLOW_H
#define KIOSLOW_H

/** \addtogroup PrdGki
 * @{
 */

#include    <stddef.h>

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include    <gkiqueue.h>
#include    <kitvcom.h>
#include    <kiosstat.h>
#include    <kiosmem.h>
#include    <kisystyp.h>

/***************************************************************************
 * Macros
 **************************************************************************/

/** Returns the size of the #SignalStructure header.
 * Defines the size of header in the #SignalStructure. Rather than just
 * using the sizeof the header this also takes into account any packing
 * between the header and the body.
 *
 * \return The size in bytes from the start of #SignalStructure to the
 *             body member of that structure. */
#define M_GetSignalHeaderLength()  (offsetof(SignalStructure, record.body))

/** Get the underlying #SignalStructure pointer from a #SignalBuffer.
 * Derives the #SignalStructure pointer from a #SignalBuffer pointer.
 *
 * \param[out] tHeSiG A #SignalStructure pointer.
 * \param[in] gKiSiG A #SignalBuffer pointer. */
#define M_GetStructureFromBuffer(tHeSiG,gKiSiG) \
            (tHeSiG) = (SignalStructure *) \
                ((Int8 *) ((gKiSiG)->type) - (offsetof(SignalStructure, record.id)))

/** Get the underlying #LoggedSignalStructure pointer from a #SignalBuffer.
 * Derives the #LoggedSignalStructure pointer from a #SignalBuffer pointer.
 *
 * \param[out] tHeSiG A #LoggedSignalStructure pointer.
 * \param[in] gKiSiG A #SignalBuffer pointer. */
#define M_GetLoggedStructureFromBuffer(tHeSiG,gKiSiG) \
            (tHeSiG) = (LoggedSignalStructure *) \
                ((Int8 *) ((gKiSiG)->type) - (offsetof(LoggedSignalStructure, record.id)))

/** Get the underlying #GenericStructure pointer from a #SignalBuffer.
 * Derives the #GenericStructure pointer from a #SignalBuffer pointer.
 *
 * \param[out] tHeSiG A #GenericStructure pointer.
 * \param[in] gKiSiG A #SignalBuffer pointer. */
#define M_KiGetGenStructureFromBuffer(tHeSiG,gKiSiG) \
            (tHeSiG) = (GenericStructure *) \
                ((Int8 *) ((gKiSiG)->type) - (offsetof(GenericStructure, signal.record.id)))

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/** Internal structure used to hold a GKI signal.
 * This structure is the underlying structure used to hold all the
 * information for a single GKI signal. */
typedef struct SignalStructureTag
{
    /** Header information. */
    KiSigHeader                     header;
    /** Signal specific data. */
    SignalRecord                    record;
}
SignalStructure;

/** Internal structure used to hold a GKI command.
 * This structure is the underlying structure used to hold all the
 * information for a single GKI command. */
typedef struct CommandStructureTag
{
    /** Header information. */
    KiSigHeader                     header;
    /** Length of command data. */
    Int16                           commandLength;
    /** Command specific data. */
    Command                         record;
}
CommandStructure;

/** Internal structure used to hold a GKI logged signal.
 * This structure is the underlying structure used to hold all the
 * information for a single GKI logged signal. */
typedef struct LoggedSignalStructureTag
{
    /** Header information. */
    KiSigHeader                     header;
    /** Logged signal specific data. */
    LoggedSignalRecord              record;
}
LoggedSignalStructure;

/** Generic union that can be used to access a signal, logged
 * signal or command.
 * This union can be used to access either a signal, logged signal
 * or command as they all have a common header. The header can be
 * accessed to identify which member of the union is valid. */
typedef union GenericStructureTag
{
    /** Common header data. */
    KiSigHeader                     header;
    /** Signal data. */
    SignalStructure                 signal;
    /** Command data. */
    CommandStructure                command;
    /** Logged signal data. */
    LoggedSignalStructure           loggedSignal;
}
GenericStructure;

/***************************************************************************
 *  Macros
 **************************************************************************/

/** New v2 KiGetSignalBlock interface.
 * This define indicates that the new KiGetSignalBlock interface is active.
 * It can be used by calling code to allow compatibility with the old interface
 * if needed. */
#define     KI_LOW_GET_SIGNAL_BLOCK_V2

/** Allocate a signal block.
 * This function allocates a #GenericStructure big enough to store the
 * given signal size.
 *
 * \param[in] L Size of signal that needs to be stored.
 * \param[in] O Overhead to add on top of the signal length (e.g. for
 *                check bytes). This is kept seperate from the signal
 *                length to enable correct user memory statistics.
 * \param[out] B Pointer to a #GenericStructure pointer that will be
 *                 set to the allocated memory. */
#define     KiGetSignalBlock(L,O,B)         KiOsGetSignalBlock (L,O,B)

/** Receive a signal block.
 * This function is used to wait for a #GenericStructure on a given input
 * queue. This must only be called from a task.
 *
 * \param[in]  Q #QueueId of the queue to receive from.
 * \param[out] B Pointer to a #GenericStructure pointer that will be
 *                 set to the received signal or command data. */
#define     KiReceiveSignalBlock(Q,B)       KiOsReceiveSignalBlock (Q,B)

/** Send a signal block.
 * This function is used to send the signal (or command) contained
 * in the given #GenericStructure to the given task. This must only be
 * called from a task.
 *
 * \param[in] T #TaskId of the task to send the #GenericStructure to.
 * \param[in] B Pointer to a #GenericStructure that contains the
 *            signal or command. */
#define     KiSendSignalBlock(T,B)          KiOsSendSignalBlock (T,B)

/** Destroy a signal block.
 * This function is used to free the memory associated with the given
 * #GenericStructure. This must only be called from a task.
 *
 * \param[in] B Pointer to a #GenericStructure that needs to be freed. */
#define     KiDestroySignalBlock(B)         KiOsDestroySignalBlock (B)

/** Determine if GKI memory contains a signal.
 * This function is used to detect whether the given memory contains a
 * GKI signal. The input memory pointer must be known to point to valid in
 * use memory. If it points to free or random data there is a greater
 * chance of false detection.
 *
 * \param[in] M Pointer to the start of valid in use memory.
 *
 * \return A #Boolean indicating whether the given memory contains a
 *         signal (TRUE) or not (FALSE). */
#define     KiIsMemorySignal(M)             KiOsIsMemorySignal (M)

/** Add a signal block to a block unit queue.
 * This function add the given signal block to the given unit queue.
 *
 * \param[in] Q A pointer to the #KiUnitQueue which will have the signal
 *                 added to it.
 * \param[in] B A pointer to a #GenericStructure identifying the signal to be
 *                 added to the queue. */
#define     KiEnqueueBlock(Q,B)             KiOsEnqueueBlock (Q, B)

/** Remove a signal block from a block unit queue.
 * This function is used to remove a signal block from a unit queue. The caller
 * must check that there is a signal on the unit queue before calling this
 * function (use #KiOnBlockQueue()).
 *
 * \param[in] Q A pointer to the #KiUnitQueue which will have the signal
 *                 removed from it.
 * \param[out] B A pointer to a #GenericStructure pointer to store the
 *                 removed signal block in. */
#define     KiDequeueBlock(Q,B)             KiOsDequeueBlock (Q, B)

/** Check if there are any signals on a block unit queue.
 * This function is used to check if there are any signals in a block unit queue.
 *
 * \param[in] Q A pointer to the #KiUnitQueue to check.
 *
 * \return A #Boolean indicating whether there is at least one signal (TRUE)
 *             or not (FALSE) on the unit queue. */
#define     KiOnBlockQueue(Q)               (((Q)->head != PNULL) ? TRUE : FALSE)
/** Flush a block unit queue.
 * This function is used to delete all signals blocks on a block unit
 * queue and delete any memory used for the queue storage. If a unit
 * queue is created locally (e.g. on the stack or allocated) rather
 * than globally then this function must be called before the
 * #KiUnitQueue structure goes out of scope or is deleted. This is
 * required to prevent any memory leaks.
 *
 * \note If any of the signals held on the queue contain pointers to
 * allocated memory this function will not know about these pointers.
 * In this case the user should manually remove and delete all signals
 * (and embedded pointers) on the queue and then call KiFlushBlockQueue().
 *
 * \param[in] Q A pointer to the #KiUnitQueue to flush. */
#define     KiFlushBlockQueue(Q)            KiOsFlushQueueBlock (Q)

/***************************************************************************
 *  Signal overwrite checking
 **************************************************************************/

/** \def KI_DONT_CHECK_SIGNAL_OVERWRITE
 * Prevent checking of signal overwrite data.
 * When #DEVELOPMENT_VERSION is defined the default operation is to add
 * check bytes to the end of signals and then check these bytes whenever
 * the signal is accessed using GKI functions. Defining this conditional
 * will remove the check bytes and code. */
#if !defined (DEVELOPMENT_VERSION) && !defined (KI_DONT_CHECK_SIGNAL_OVERWRITE)
#define KI_DONT_CHECK_SIGNAL_OVERWRITE
#endif

/** \def KI_CHECK_VALUES_SIZE
 * Number of check bytes.
 * This defines the number of check bytes to add to the end of signals. */
#if defined (KI_DONT_CHECK_SIGNAL_OVERWRITE)
# define KI_CHECK_VALUES_SIZE (0)
#else
# define KI_CHECK_VALUES_SIZE (4)  /* 4 byte values to check signal overwriting */
#endif

/** Macro to copy the check bytes to a signals memory.
 * This macro will copy over the check bytes to the end of the user signal
 * memory.
 *
 * \param[in,out] bLOCKpTR A pointer to a #SignalStructure to add the check
 *                    bytes to.
 * \param[in] sIGNALsIZE The size of the user part of the signal. The check
 *                    bytes will be added after this. */
#if defined (KI_DONT_CHECK_SIGNAL_OVERWRITE)
# define M_KiAddSignalOverwriteCheckData(bLOCKpTR, sIGNALsIZE)
#else
# define M_KiAddSignalOverwriteCheckData(bLOCKpTR, sIGNALsIZE)      \
            memcpy ((Int8 *)((Int8*)(bLOCKpTR)) + (M_GetSignalHeaderLength () + (sIGNALsIZE)), \
                    kiSignalCheckValues, KI_CHECK_VALUES_SIZE);
#endif

/** @} */ /* End of PrdGki group */

#if !defined (KI_DONT_CHECK_SIGNAL_OVERWRITE)
extern const Int8 kiSignalCheckValues[KI_CHECK_VALUES_SIZE];
#endif

/***************************************************************************
 *  Global function prototypes
 **************************************************************************/

void KiOsGetSignalBlock     (Int16                   signalLength,
                             Int16                   overhead,
                             GenericStructure      **theSignal);

void KiOsReceiveSignalBlock (QueueId                 thisQueueId,
                             GenericStructure      **theSignal);

void KiOsSendSignalBlock    (TaskId                  toWhere,
                             GenericStructure       *theSignal);

void KiOsDestroySignalBlock (GenericStructure       *theSignal);

Boolean KiOsIsMemorySignal  (void                  * startOfMem);

void    KiOsEnqueueBlock    (KiUnitQueue *queue, GenericStructure * genStruct);
void    KiOsDequeueBlock    (KiUnitQueue *queue, GenericStructure ** genStruct);
void    KiOsFlushBlockQueue (KiUnitQueue *queue);

#endif
/* END OF FILE */
