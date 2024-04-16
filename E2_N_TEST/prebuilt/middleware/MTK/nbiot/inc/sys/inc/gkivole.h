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
 * GKI Watchvole interface.
 **************************************************************************/

#ifndef GKIVOLE_H
#define GKIVOLE_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <ki_typ.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/** \defgroup CfgGkiVole GKI Watchvole Configuration
 * \ingroup PrdGki
 * \ingroup Cfg
 * Watchvole configuration.
 * The operation of the watchvole is described in the
 * \ref ApiGkiVole "GKI Watchvole API" section.  It has only one configuration
 * item, being the maximum watchvole count that can be reached before
 * KiWatchVoleUpdate() starts to return FALSE.
 * @{
 */

/** \def KI_MAX_WATCHVOLE_COUNT_MS
 * The maximum count that the watchvole may reach.
 * This defines the the maximum duration that may pass before
 * KiWatchVoleUpdate() starts to return FALSE.
 *
 * \note This define is given a default value of 10 seconds if
 *       KI_MAX_WATCHVOLE_COUNT_MS is not previously defined.
 *
 * \note Some products may cause the default value to be increased, or
 *       redefine the timeout value to increase it if needed.
 */

/** @} */ /* End of CfgGkiVole group */

#if !defined(KI_MAX_WATCHVOLE_COUNT_MS)
# define KI_MAX_WATCHVOLE_COUNT_MS   10000
#endif

/***************************************************************************
 * Type Definitions
 **************************************************************************/


/***************************************************************************
 *  Macros
 **************************************************************************/

/** \defgroup ApiGkiVole GKI Watchvole API
 * \ingroup PrdGki
 * \ingroup Api
 * Watchvole API.
 * The watchvole is a simple software counter that is used to ensure that all
 * tasks in the system are able to run in some amount of time.  The mechanism
 * is typically used to control a hardware watchdog and operates according
 * to the following rules:
 *
 *  - Whenever the lowest priority task runs, the watchvole count is reset by
 *     calling KiWatchVoleReset().
 *  - Each tick to the system increments the watchvole count by calling
 *     KiWatchVoleUpdate().
 *  - If the watchvole count is below some threshold, each tick to the system
 *     also resets the watchdog.  This is determined from the result returned
 *     by KiWatchVoleUpdate().
 *
 * This means that the watchdog will be prevented from resetting the system
 * so long as the periodic system tick is running, and that the lowest
 * priority task is able to run and reset the watchvole count before it
 * expires.
 *
 * \note A vole is a rodent that resembles a rat or mouse but with shorter
 *       limbs and tail as well as a heavier body.  While unable to cause
 *       much harm to a dog, a vole would be able to keep one awake.
 * @{
 */

/** Update the watchvole counter and determine if it has expired.
 * Updates the watchvole and returns whether or not the new count is
 * larger than the configured maximum value.  If the value is less than
 * the configured maximum, the system is healthy and any hardware watch
 * dogs or supervisors can be reset.
 *
 * \param  d  The passed duration in KernelTicks since the function
 *             was last called.
 * \retval FALSE If the watchvole count is larger than then maximum value.
 *
 * \see kiWatchVoleCountMax
 */
#define KiWatchVoleUpdate(D)    KiOsWatchVoleUpdate(D)

/** Reset the watchvole count.
 * This resets the watchvole count, and should be called from tasks to
 * indicate that the system is still running correctly and no tasks have
 * entered an infinte loop.  The typical usage is to call this from the
 * lowest priority task in the system.
 */
#define KiWatchVoleReset()      KiOsWatchVoleReset()

/** Get the current watchvole count.
 * This returns the accumulated duration since KiWatchVoleReset() was last
 * called.
 * \returns The passed duration in KernelTicks.
 */
#define KiWatchVoleCount()      KiOsWatchVoleCount()

/** @} */ /* End of ApiGkiVole group */

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

Boolean KiOsWatchVoleUpdate(KernelTicks d);
void    KiOsWatchVoleReset (void);
Int32   KiOsWatchVoleCount (void);

#endif

/* END OF FILE */
