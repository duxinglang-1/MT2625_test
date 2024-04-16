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
 * GKI header for miscellaneous API.
 **************************************************************************/

#ifndef GKIMISC_H
#define GKIMISC_H

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
#include <kistat.h>
#include <ki_event.h>
#include <kicfg.h>

/***************************************************************************
 * Kernel Function redirections
 **************************************************************************/

/** GKI boot/reset/initialisation function.
 * This function should be called during system initialisation to create all
 * the GKI objects (tasks, queues, memory, semaphores). Typically an RTOS
 * has a state during startup where it allows RTOS objects to be created
 * but where scheduling is disabled, this is where this function should be
 * called. If the RTOS has no such state then some extra options may need
 * to be enabled. If KiReset() is called while shceduling is active the
 * #KI_CREATE_ALL_QUEUES_BEFORE_TASKS conditional may be needed to make
 * sure all GKI queues exist before tasks are created. Otherwise a task
 * may attempt to signal a non-existing queue. If the creation of queues
 * cannot be split from the task creation then another solution is to
 * disable scheduling around the call to KiReset or to call it from the
 * highest priority task in the system (may also have to disable
 * interrupts to prevent them sending any signals). */
#define     KiReset()                     KiOsReset ()

/** Check whether the GKI system has been initialised.
 * This function can be used to check that the GKI system has been
 * initialised before using any of its services. GKI tasks should not
 * need to use this as the fact that a GKI task is running
 * indicates that the GKI has been initialised.
 *
 * \return A #Boolean indicating that the GKI has been initialised (TRUE)
 *             or not (FALSE). */
#define     KiSystemIsInitialised()       KiOsSystemIsInitialised ()

/* The disable/enable logging functions are only implemented on target
** and only when the test task is present */
#if defined (ROM_TEST_TASK) && !defined (ON_PC)
/** Disable GKI signal logging.
 * This function disables target GKI signal logging. A call to this
 * should be matched with a call to #KiEnableLogging(). The state of
 * the active filter is not affected by calling this.
 *
 * \note This should be used with extreme caution as the target should
 *           not normally be disabling logging visibility. */
# define    KiDisableLogging()            KiOsDisableLogging()

/** Enable GKI signal logging.
 * This function enables target GKI signal logging after a previous
 * call to #KiDisableLogging(). */
# define    KiEnableLogging()             KiOsEnableLogging()
#else
# define    KiDisableLogging()
# define    KiEnableLogging()
#endif

/***************************************************************************
 * Function prototypes
 **************************************************************************/

void       KiOsReset               (void);

void       KiInitialiseOs          (void);

Boolean    KiOsSystemIsInitialised (void);

#if defined (ROM_TEST_TASK) && !defined (ON_PC)
void       KiOsDisableLogging      (void);

void       KiOsEnableLogging       (void);
#endif

#if defined (__cplusplus)
}
#endif

/** @} */ /* End of PrdGki group */

#endif

/* END OF FILE */
