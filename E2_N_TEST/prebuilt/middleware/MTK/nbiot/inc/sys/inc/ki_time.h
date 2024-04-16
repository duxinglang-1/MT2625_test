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
 * GKI header file for Time related definitions.
 * This file defines items related to the GKI timebase and its relation
 * to the system timebase.
 **************************************************************************/

#ifndef KI_TIME_H
#define KI_TIME_H

/** \addtogroup PrdGki
 * @{
 */

/***************************************************************************
 * Kernel Tick conversion macros
 **************************************************************************/

/** \def KI_TIME_USE_MILLISECOND_TICK
 * Make GKI use one millisecond as its internal tick.
 * This conditional makes the GKI tick equal to 1 millisecond. This is not
 * normally used in a GSM or dual mode build as the GSM frame rate is not
 * a whole number of milliseconds. This is more applicable for
 * non-modem builds.
 *
 * When this is used the interrupt calling #KiTick can have any millisecond
 * period. The period of the interrupt in miiliseconds is then the value
 * passed to #KiTick.
 */

#if defined (KI_TIME_USE_TEN_MILLISECOND_TICK)

/** Numerator for Kernel tick to Millisecond conversion. */
# define KI_TICK_TO_MS_NUMERATOR      10
/** Denominator for Kernel tick to Millisecond conversion. */
# define KI_TICK_TO_MS_DENOMINATOR    1

#else
#if defined (KI_TIME_USE_MILLISECOND_TICK)

/** Numerator for Kernel tick to Millisecond conversion. */
# define KI_TICK_TO_MS_NUMERATOR      1
/** Denominator for Kernel tick to Millisecond conversion. */
# define KI_TICK_TO_MS_DENOMINATOR    1

#else
#if defined (NB_PC_HW_SIM)
/* on PC HW emulation, we make the ki tick be 1ms,
   1.92Mhz is possible in Hw emulation using the sim_timers.
   */

/** Numerator for Kernel tick to Millisecond conversion. */
# define KI_TICK_TO_MS_NUMERATOR      1
/** Denominator for Kernel tick to Millisecond conversion. */
# define KI_TICK_TO_MS_DENOMINATOR   1

#elif defined (UPGRADE_3G) || defined (UPGRADE_LTE)

/* To cope with a Dual Mode 3G/GSM system the KernelTick is defined as
 * 1/13 ms. This means there are exactly 60 KernelTicks in a GSM frame
 * and exactly 130 KernelTicks per 3G frame.
 *
 * The main tick provider in each system should use the relevant define
 * that specifies how many KernelTicks there are per frame when
 * informing the kernel how many kernel ticks have elapsed.
 */

/** Kernel Ticks per GSM Frame.
 * This is the number of kernel ticks in 1 GSM frame (60/13 ms). */
# define KI_GSM_KT_PER_FT            60

/** Kernel Ticks per 3G Frame.
 * This is the number of kernel ticks in 1 3G frame (10 ms). */
# define KI_3G_KT_PER_FT            130

/** Kernel Ticks per LTE sub-frame.
 * This is the number of kernel ticks in 1 LTE sub-frame (1 ms). */
# define KI_LTE_KT_PER_SFT           13


/** Kernel Ticks per half 3G Frame.
 * This is the number of kernel ticks in half a 3G frame (5 ms).
 * This may be used in a 3G system rather than KI_3G_KT_PER_FT to get
 * better timer accuracy and may improve slow clocking responsiveness
 * depending on implementation. */
# define KI_3G_KT_PER_HALF_FT        65

/** Numerator for Kernel tick to Millisecond conversion. */
# define KI_TICK_TO_MS_NUMERATOR      1
/** Denominator for Kernel tick to Millisecond conversion. */
#ifdef UE_SIMULATOR
# define KI_TICK_TO_MS_DENOMINATOR    1
#else
# define KI_TICK_TO_MS_DENOMINATOR   13
#endif

/* For 3G (DualMode) a Kernel tick is 1/13 milliseconds */

#if defined (UPGRADE_LTE)
/* LTE has same frame duration as a 3G frame (10ms) */
#define KI_LTE_KT_PER_FT KI_3G_KT_PER_FT
#endif

#else /* UPGRADE_3G */

/** Kernel Ticks per GSM Frame.
 * This is the number of kernel ticks in 1 GSM frame (60/13 ms). */
# define KI_GSM_KT_PER_FT             1

/** Numerator for Kernel tick to Millisecond conversion. */
# define KI_TICK_TO_MS_NUMERATOR     60
/** Denominator for Kernel tick to Millisecond conversion. */
# define KI_TICK_TO_MS_DENOMINATOR   13

/* Specific implementations of some of the conversion macros that
 * prevent overflow or rounding errors. */
# define SECONDS_TO_TICKS(S)      (KernelTicks)((((S) * 1300L) + 3L) / 6 )
# define TICKS_TO_SECONDS(T)      ((((T) * 6L) + 650L) / 1300L )
# define TICKS_TO_MILLISECONDS(T) ((((T) * 120L) + 13L)  / 26L )

#endif /* UPGRADE_3G */
#endif /*KI_TIME_USE_TEN_MILLISECOND_TICK */
#endif /*KI_TIME_USE_TEN_MILLISECOND_TICK */
/** \def TICKS_TO_SECONDS(T)
 * Convert from KernelTicks to seconds.
 * This macro converts from KernelTicks to seconds.
 *
 * \param[in] T An integer count of KernelTicks
 * \return The integer number of seconds represented by \a T.
 *
 * \see SECONDS_TO_TICKS
 */
#if !defined (TICKS_TO_SECONDS)
# define TICKS_TO_SECONDS(T)      ((((Int64)(T) * KI_TICK_TO_MS_NUMERATOR) + (KI_TICK_TO_MS_DENOMINATOR / 2)) / (KI_TICK_TO_MS_DENOMINATOR * 1000))
#endif

/** \def TICKS_TO_MILLISECONDS(T)
 * Convert from KernelTicks to milliseconds.
 * This macro converts from KernelTicks to milliseconds.
 *
 * \param[in] T  An integer count of KernelTicks
 * \return   The integer number of milliseconds represented by \a T.
 *
 * \see MILLISECONDS_TO_TICKS
 */
#if !defined (TICKS_TO_MILLISECONDS)
# define TICKS_TO_MILLISECONDS(T) ((((Int64)(T) * KI_TICK_TO_MS_NUMERATOR) + (KI_TICK_TO_MS_DENOMINATOR / 2)) / (KI_TICK_TO_MS_DENOMINATOR))
#endif

/** \def SECONDS_TO_TICKS(S)
 * Convert a duration expressed in seconds to KernelTicks.
 * This macro converts from Milliseconds to Kernel Ticks.
 *
 * \param[in] S  An integer count of seconds.
 * \return   The integer number of KernelTicks represented by \a S.
 *
 * \see TICKS_TO_SECONDS
 */
#if !defined (SECONDS_TO_TICKS)
# define SECONDS_TO_TICKS(S)      (KernelTicks)((((Int64)(S) * KI_TICK_TO_MS_DENOMINATOR * 1000) + (KI_TICK_TO_MS_NUMERATOR / 2)) / KI_TICK_TO_MS_NUMERATOR )
#endif

/** \def MILLISECONDS_TO_TICKS(S)
 * Convert a duration expressed in milliseconds to KernelTicks.
 * This macro converts from milliseconds to KernelTicks.
 *
 * \param[in] S  An integer count of seconds.
 * \return   The integer number of KernelTicks represented by \a S.
 *
 * \see TICKS_TO_MILLISECONDS
 */
#if !defined (MILLISECONDS_TO_TICKS)
# define MILLISECONDS_TO_TICKS(S) (KernelTicks)((((Int64)(S) * KI_TICK_TO_MS_DENOMINATOR) + (KI_TICK_TO_MS_NUMERATOR / 2)) / KI_TICK_TO_MS_NUMERATOR )
#endif

#if defined (KI_GSM_KT_PER_FT)
/** Convert KernelTicks to GSM frame ticks.
 * This macro converts some quantity of #KernelTicks into GSM #FrameTicks.
 * \param[in] T  An integer count of KernelTicks.
 * \return   The integer count of GSM frames that represents \a T.
 * \see KI_GSM_FRAME_TO_KERNEL_TICKS
 */
# define KI_KERNEL_TO_GSM_FRAME_TICKS(T) ((FrameTicks)((T) / (KI_GSM_KT_PER_FT)))

/** Convert GSM frames to KernelTicks.
 * This macro converts some quantity of GSM #FrameTicks into #KernelTicks.
 * \param[in] T  An integer count of GSM frames.
 * \return   The integer count of KernelTicks that represents \a T.
 * \see KI_KERNEL_TO_GSM_FRAME_TICKS
 */
# define KI_GSM_FRAME_TO_KERNEL_TICKS(T) ((KernelTicks)((T) * (KI_GSM_KT_PER_FT)))
#endif /* KI_GSM_KT_PER_FT */

#if defined (KI_3G_KT_PER_FT)
/** Converts a count of KernelTicks frames to 3G frames.
 * \param[in] T  An integer count of KernelTicks.
 * \return   The integer count of 3G frames that represents \a T.
 * \see KI_3G_FRAME_TO_KERNEL_TICKS
 */
# define KI_KERNEL_TO_3G_FRAME_TICKS(T)  ((FrameTicks)((T) / (KI_3G_KT_PER_FT)))

/** Converts a count of 3G frames to KernelTicks.
 * \param[in] T  An integer count of 3G frames.
 * \return   The integer count of KernelTicks that represents \a T.
 * \see KI_KERNEL_TO_3G_FRAME_TICKS
 */
# define KI_3G_FRAME_TO_KERNEL_TICKS(T)  ((KernelTicks)((T) * (KI_3G_KT_PER_FT)))
#endif /* KI_3G_KT_PER_FT */

/** @} */ /* End of PrdGki group */

#endif

/* END Of FILE */
