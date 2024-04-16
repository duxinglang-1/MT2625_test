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
 * GKI header file for types used to transfer GKI data to and from
 * the host (PC).
 *
 * \note Do not change any of the types in this file as they are used by
 *           PC test tools. Existing record types should never be changed as
 *           this will break backwards compatibility. Add new record types
 *           if new features are required.
 **************************************************************************/

#ifndef KITVCOM_H
#define KITVCOM_H

/** \addtogroup PrdGki
 * @{
 */

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#include    <system.h>
#include    <pssignal.h>
#include    <kitqid.h>
#include    <ki_typ.h>

/****************************************************************************
 * Global Types
 ****************************************************************************/

#if !defined (TVSIGNAL_H) && !defined (KI_TEST_TOOL_SIGNAL_INFO)
/** Dummy Signal union.
 * This version of the Signal union is needed in cases where this file
 * is included into another file that does not define a Signal union. */
union Signal
{
    /** Ensure suitable alignment.
     * This is needed to ensure that the Signal union is correctly
     * aligned no matter the types it might hold. */
    SysBasicTypesUnion    alignmentEntry;
};
#endif

/** GKI wait script command.
 * This script command is used to wait for a signal to be received
 * that has the given attributes. */
typedef struct WaitCommandTag
{
    /** Source task id.
     * Source task of the signal or UNKNOWN_TASK_ID to match any task. */
    TaskId              from;
    /** Destination task id.
     * Destination task of the signal or UNKNOWN_TASK_ID to match any task. */
    TaskId              to;
    /** Signal id.
     * The signal id to wait for. */
    SignalId            id;

} WaitCommand;

/** GKI delay script command.
 * This script command is used to delay script execution for a number of
 * milliseconds. */
typedef struct DelayCommandTag
{
    /** Number of milliseconds to delay. */
    Int32               delay;

} DelayCommand;

/** GKI transparent script command.
 * This script command is used to insert a string into a script. This can
 * help to indicate the position in a script at runtime. */
typedef struct TransparentCommandTag
{
    /** Storage for the string. 
\assoc STRING   */
    char                string[256];
}
TransparentCommand;

/** GKI target signal routing script command.
 * This command it used to tell the target to redirect all signals
 * comming from the host to a specific task id.
 *
 * \deprecated This command is not used anymore. */
typedef struct SubsequentSigsToTaskCommand
{
    /** Enable signal redirection. */
    Boolean             on;
    /** Task id to forward signals to. */
    TaskId              taskId;
}
SubsequentSigsToTaskCommand;

/** GKI target task state command.
 * This command is used by host tools to tell the target to suspend
 * (or resume) a specific task. Genie uses this command in the split
 * system test environment to disable tasks in the target that are
 * being run on the PC. Genie only uses this to suspend tasks, they
 * are never resumed. */
typedef struct TaskStateCommandTag
{
    /** Suspend (TRUE) or resume (FALSE). */
    Boolean             suspend;
    /** Task to suspend or resume. */
    TaskId              taskId;
}
TaskStateCommand;

/** GKI empty command.
 * This an empty command type. This is used where command ids are
 * now unused or have no body. */
typedef Int8 EmptyCommand;

/** GKI command ids.
 * This enumeration gives the GKI command ids. Do not change the values
 * of any of the entries in this enumeration. If a command is no longer
 * needed mark it as unused/redundant but do not remove it. Only add to
 * the end of this list and ensure the order of entries is the same as
 * the order in the #CommandBody union. */
typedef enum CommandIdTag
{
    TC_WAIT,
    TC_DELAY,
    TC_START,
    TC_TRANSPARENT,
    TC_STOP,
    TC_UNUSED_1,
    TC_UNUSED_2,
    TC_UNUSED_3,
    TC_SUBSEQUENT_SIGS_TO_TASK,
    TC_TASK_STATE
}
CommandId;

/** GKI command body.
 * This union defines all the valid command bodies. This union has to
 * be kept in line with the #CommandId enumeration. */
typedef union CommandBodyTag
{
    WaitCommand                 wait;
    DelayCommand                delay;
    EmptyCommand                start;
    TransparentCommand          transparent;
    EmptyCommand                stop;
    EmptyCommand                unused1;
    EmptyCommand                unused2;
    EmptyCommand                unused3;
    SubsequentSigsToTaskCommand subsequentSigsToTask;
    TaskStateCommand            taskState;
}
CommandBody;

/** GKI command record.
 * This structure defines a GKI command. */
typedef struct Command
{
    /** Command id. */
    CommandId           id;
    /** Command body. */
    CommandBody         body;
}
Command;

/** GKI signal routing information.
 * This structure provides the source and destination task ids for
 * a signal. */
typedef struct SignalDirectivesTag
{
    /** Source task id. */
    TaskId              source;
    /** Destination task id. */
    TaskId              dest;
}
SignalDirectives;

/** GKI inter-task signal record.
 * This structure contains information about an inter-task
 * signal. */
typedef struct SignalRecord
{
    /** Source and destination tasks. */
    SignalDirectives    directives;
    /** The length of the body in bytes. */
    Int16               length;
    /** The signal id. 
\assoc UNION \ref body */
    SignalId            id;
    /** The signal body. */
#ifdef __GNUC__
    union Signal        body __attribute__ ((packed));
#else
    union Signal        body;
#endif
}
#if !defined (__cplusplus)
Signal,
#endif
SignalRecord;

/** GKI logged signal record.
 * This structure contains information about a logged signal. */
typedef struct LoggedSignalRecord
{
    /** Source and destination tasks. */
    SignalDirectives    directives;
    /** System specific frame number.
     * The exact meaning of this field depends on the system
     * being used. For example a GSM modem system will typically
     * use the GSM frame number. */
    FrameTicks          frameNumber;
    /** GKI kernel time. */
    KernelTicks         time;
    /** The length of the body in bytes. */
    Int16               length;
    /** The signal id. 
\assoc UNION \ref body */
    SignalId            id;
    /** The signal body. */
#ifdef __GNUC__
    union Signal        body __attribute__ ((packed));
#else
    union Signal        body;
#endif
}
#if !defined (__cplusplus)
SerialSignal,
#endif
LoggedSignalRecord;

/** GKI record type. */
#if !defined (RECORD_TYPE_DEFINED)
#define RECORD_TYPE_DEFINED   0
typedef enum RecordTypeTag
{
    /** Indicates a #SignalRecord. */
    SIGNAL_RECORD = 'S',
    /** Indicate a #LoggedSignalRecord. */
    SERIAL_SIGNAL_RECORD = '2',
    /** Indicates a #Command record. */
    COMMAND_RECORD = 'C',
    /** Indicates end of file. */
    END_OF_FILE_RECORD = 'E'
}
RecordType;

/** GKI record.
 * This structure is used to store information for an inter-task signal,
 * logged signal or a command. */
typedef struct RecordTag
{
    /** File record number.
     * Not stored in .tsb or .tlg files - only used within Genie. */
    Int32       recordNumber;
    /** Record type.
     * Indicates which member of the body union is present. 
\assoc UNION \ref body */
    RecordType  type;
    /** Record body length. */
    Int16       length;
    /** Record body. */
    union
    {
        SignalRecord            signal;
        LoggedSignalRecord      loggedSignal;
        Command                 command;
    }           body;
}
Record;
#endif

/** @} */ /* End of PrdGki group */

#endif
/* END OF FILE */
