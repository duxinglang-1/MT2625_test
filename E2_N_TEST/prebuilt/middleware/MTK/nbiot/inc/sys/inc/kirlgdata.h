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
 * GKI header file for the RAM logging main data structure.
 **************************************************************************/

#ifndef KIRLGDATA_H
#define KIRLGDATA_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>

#if defined (KI_ENABLE_RAM_LOGGING)
#include <kicfg.h>
#include <kirlgcfg.h>
#include <kirlgtyp.h>

#if !defined (EXCLUDE_GSM_L1)
# include <l1_typ.h>  /* L1 state information */
#endif

# if !defined (EXCLUDE_GPRS_PS)
#if defined (USE_BMM_ALLOC_MEMORY)
#  include <bmm.h>
#else
#  include <tmm.h>      /* Traffic memory manager */
#endif
# endif /* !EXCLUDE_GPRS_PS */

#if defined (UPGRADE_CORE_FR)
# include <frcfg.h>
# if defined (FR_RLG_REMOTE_ENABLE)
#  include <frdbg.h>
# endif
#endif

#if defined (KI_RLG_ENABLE_AUD_LOGGING)
# include <kirlgdmpaud.h>
#endif 

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Macros
 ***************************************************************************/
/***************************************************************************
 *
 * Macro:       M_KiRlgCheckAndLogLpTaskEntry
 *
 * Description: Checks and logs the entry into the Lowest Priority Task
 *
 ***************************************************************************/
#define M_KiRlgCheckAndLogLpTaskEntry(qUEUEiD)           \
{                                                        \
    if (qUEUEiD == LOW_PRIORITY_QUEUE_ID)                \
    {                                                    \
        M_KiRlgGetTime (kiRlgData.lastLpTaskEntryTime);  \
    }                                                    \
}

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/* This structure contains all that data that has been logged to RAM. */
typedef struct KiRlgDataTag
{
#if defined (KI_RLG_ENABLE_OUTPUT_AFTER_RESET)
    /* This flag indicates that there is RAM log data available after a reset has occurred. This
     * has been made into an Int32 so that we can indicate that a RAM log is available by
     * writing in a unique number. This is because this variable is not zero initialised, so
     * it may contain any value on a cold power-up. */
    Int32 ramLogAvailable;
#endif

#if !defined (KI_RLG_DISABLE_DATA_COMPRESSION)
    /* This flag indicates if the output should be compressed. */
    Boolean compressOutput;
#endif

    /* This is the last time that the lowest priority was run */
    Int32 lastLpTaskEntryTime;

    /* This is the number of nested interrupts */
    Int32 sysInterruptNestLevel;

    /* This is the number of times interrupts have been disabled in the system
     * before they have been reenabled. */
    Int16 irqDisableCount;

#if defined (KI_ENABLE_ADDRESS_ZERO_CHECK)
    /* This is an array of the current contents of memory starting from address zero. */
    Int32 currMemAddrZeroContents [KI_RLG_MAX_NUM_ADDRESS_ZERO_CONTENTS];
#endif

#if !defined (EXCLUDE_GSM_L1)
    /* This is the GSM Layer 1 state */
    L1State gsmLayer1State;
#endif

    /* This index holds the next index into the array to write GKE data to. ie it marks
     * the next record after the end of the circular buffer.  */
    Int32 gkeIndex;

    /* The following array stores the GKE log data. It is accessed as a circular buffer */
    KiRlgGkeRecord gkeRecBuff [KI_RLG_MAX_RLOG_GKE_ENTRIES];

    /* This index holds the location in the gkeRecBuff [] array that contains the
     * assert filename and line number. */
    Int32 assertFileLineIndex;

    /* This index holds the location in the gkeRecBuff [] array that contains the
     * assert debug values that are passed when a DevCheck or DevParam is hit. */
    Int32 assertValIndex;

    /* This index holds the location in the gkeRecBuff [] array that contains additional
     * assert information. */
    Int32 assertInfoIndex;

#if defined (KI_RLG_ENABLE_TCU_LOGGING)
    /* This index holds the next index into the array to write TCU data to. ie it marks
     * the next record after the end of the circular buffer.  */
    Int32 tcuIndex;

    /* The following array stores the TCU log data. It is accessed as a circular buffer */
    KiRlgTcuRecord tcuRecBuff [KI_RLG_MAX_RLOG_TCU_ENTRIES];
#endif /* KI_RLG_ENABLE_TCU_LOGGING */

#if defined (KI_RLG_ENABLE_TASK_TRACKING)
    /* This variable holds the number of tasks that have been nested by the OS */
    Int32 taskNesting;

    /* This buffer holds the frame number of the last time this task was entered.
     * If it is zero, then the task has completed */
    KiRlgTaskTrackRecord taskTracking [KI_NUM_UNIQUE_TASK_IDS_DEFINED];
#endif /* KI_RLG_ENABLE_TASK_TRACKING */

#if defined (KI_STATISTICS)
    /* GKI statistics */
    KiFixStatisticsInd fixStatistics;
    KiStatisticsInd statistics;
#endif /* KI_STATISTICS */

#if defined (KI_RLG_ENABLE_LOG_PROF_STATS)
    /* This buffer stores the profile statistics of specfic START/END points. It
     * is used to time the number of profile timer ticks between two points, useful
     * for measuring the time to execute sections of code. */
    KiRlgProfTrack profTrackBuff [KI_RLG_MAX_NUM_PROFS];
#endif /* KI_RLG_ENABLE_LOG_PROF_STATS */

#if !defined (KI_DISABLE_KM_DYNAMIC_MEMORY) && !defined (KI_DISABLE_DYNAMIC_MEMORY)
    /* This stores the overall Dynamic Memory usage in the system */
    KiDynamicMemUsageCnf dynMemoryUsage;
    KiDynamicMemUsageCnf dynNonCacheableMemoryUsage;
#endif

#if defined (UPGRADE_CORE_FR)
# if defined (FR_RLG_REMOTE_ENABLE)
    FrMpShmRlgAds plpPlfInfo;
    FrRlgStore    plpPlfLog;
# endif
#endif /* UPGRADE_CORE_FR */

#if defined (KI_RLG_OUTPUT_LOG_TO_CUSTOM_DEVICE)
    Boolean customDeviceWriteActive;
    Int32   customDeviceErrorCode;
#endif /* KI_RLG_OUTPUT_LOG_TO_CUSTOM_DEVICE */

#if defined (KI_RLG_ENABLE_AUD_LOGGING)  
    AudioRlgArray audioRlgArray;
#endif /* KI_RLG_ENABLE_AUD_LOGGING */
}
KiRlgData;

/***************************************************************************
 * Variables Prototypes
 ***************************************************************************/
extern KiRlgData    kiRlgData;

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

extern void KiRlgOutputLog (KiRlgData *rlgData_p);

#endif /* KI_ENABLE_RAM_LOGGING */

/***************************************************************************
 *  Default empty macro definitions.
 **************************************************************************/

#if !defined M_KiRlgCheckAndLogLpTaskEntry
#define M_KiRlgCheckAndLogLpTaskEntry(qUEUEiD)
#endif

#endif /* END OF FILE */

