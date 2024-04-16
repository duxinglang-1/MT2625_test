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
 * GKI header that allows RTOS specific configuration of how task entry
 * points are defined.
 **************************************************************************/

#ifndef KIOSSPEC_H
#define KIOSSPEC_H

/** \addtogroup GkiOsAbs
 * @{
 */

/***************************************************************************
 *  Config
 **************************************************************************/

/** \page GkiOsAbsInfo1 GKI OS Abstraction.
 * \section TaskEntryPoint Task Entry Point Definition
 * The GKI defines a Task entry point as being
 *
 * \code
 *     KI_ENTRY_POINT MyTask (KI_TASK_ARGS)
 * \endcode
 *
 * This maps to being
 *
 * \code
 *     void MyTask (void)
 * \endcode
 *
 * If the underlying OS requires a different task entry point definition then
 * one or more of the #KI_OS_ENTRY_POINT, #KI_OS_TASK_ARGS and
 * #M_KiOsEntryPointReturn() configuration items may need to be defined by
 * the OS specific file included in kiosspec.h.
 *
 * If either #KI_OS_ENTRY_POINT or #KI_OS_TASK_ARGS is defined then the GKI
 * will create wrapper functions that match the OS task entry point
 * requirements. Pointer to these entry points will be stored in the
 * kiOsEntryPointWrappers[] array. These need to be used in the OS
 * specific implementation of #M_KiOsCreateTask() instead of the GKI
 * compliant entry point stored in kiTaskInitTable[].entryPoint (the index
 * to both arrays is the same).
 *
 * If wrapper functions are needed it will end up using a bit more stack per
 * task and some extra ROM space. In most cases wrapper functions should not
 * be needed even when the OS requires arguments or a return value. Any
 * arguments would be ignored by the GKI entry point and GKI tasks normally
 * don't return.
 *
 * Some examples assuming the OS does require a different entry point
 * definition:
 *
 * Example 1: OS requires - int EntryPoint (void)
 *
 * \code
 *     #define KI_OS_ENTRY_POINT         int
 *     #define M_KiOsEntryPointReturn()  return (0)
 * \endcode
 *
 * Example 2: OS requires - void EntryPoint (int param1, int param2)
 *
 * \code
 *     #define KI_OS_TASK_ARGS           int param1, int param2
 *     #define M_KiOsEntryPointReturn() \
 *             {                        \
 *                 PARAMETER_NOT_USED(param1); \
 *                 PARAMETER_NOT_USED(param2); \
 *             }
 * \endcode
 *
 * Example 3: OS requires - int EntryPoint (int param1, int param2)
 *
 * \code
 *     #define KI_OS_ENTRY_POINT         int
 *     #define KI_OS_TASK_ARGS           int param1, int param2
 *     #define M_KiOsEntryPointReturn() \
 *             {                        \
 *                 PARAMETER_NOT_USED(param1); \
 *                 PARAMETER_NOT_USED(param2); \
 *                 return (0);          \
 *             }
 * \endcode
 *
 * But as detailed above even if the OS normally requires one of these forms
 * you can often get away without configuring these items. */

/** \def KI_OS_ENTRY_POINT
 * OS required entry point type.
 * This is used to define the type that a task entry point needs to be for the
 * underlying OS e.g.
 *
 * KI_OS_ENTRY_POINT MyTask (KI_OS_TASK_ARGS).
 *
 * If this is defined then the GKI will create wrapper functions
 * (kiOsEntryPointWrappers[]) for all the entry points that should then be
 * used instead of the entry points given in kiTaskInitTable[].entryPoint.
 * The #M_KiOsEntryPointReturn() macro will also need to be defined if it is
 * a non-void return type. */

/** \def KI_OS_TASK_ARGS
 * OS required entry point arguments.
 * This is used to define the arguments that a task entry point needs to have
 * for the underlying OS e.g.
 *
 * KI_OS_ENTRY_POINT MyTask (KI_OS_TASK_ARGS).
 *
 * If this is defined then the GKI will create wrapper functions
 * (kiOsEntryPointWrappers[]) for all the entry points that should then be
 * used instead of the entry points given in kiTaskInitTable[].entryPoint.
 * The #M_KiOsEntryPointReturn() macro may also need to be defined to
 * call PARAMETER_NOT_USED() to prevent warnings for the unused arguments. */

/** \def M_KiOsEntryPointReturn()
 * Macro called at the end of a GKI task wrapper to clean up.
 * This macro adds code at the end of any GKI OS entry point wrappers. This
 * should include any call to return if needed due to #KI_OS_ENTRY_POINT
 * not being void. This may also need to call PARAMETER_NOT_USED() for any
 * unused parameters introduced from #KI_OS_TASK_ARGS. */

/** \def KI_OS_TASK_WRAPPERS_REQUIRED
 * Internal conditional indicating task wrappers are required.
 * This conditional compile is automatically enabled if the OS specific file
 * included in #kiosspec.h defines either #KI_OS_ENTRY_POINT or
 * #KI_OS_TASK_ARGS (or both). This should not be enabled manually in the build.
 * This is used internally to enable generation of the OS specific task entry
 * point wrapper functions. */

/***************************************************************************
 *  Includes
 **************************************************************************/

#include <kicfgos.h>

/***************************************************************************
 *  Include OS specific stuff
 **************************************************************************/

#if defined (AMX_KERNEL)
#  include <kiamx.h>
#endif

#if defined (NUCLEUS_KERNEL)
#  include <kinucles.h>
#endif

#if defined (RTXC_QUADROS_KERNEL)
#  include <kirq.h>
#endif

#if defined (MQX_KERNEL)
#  include <kimqx.h>
#endif

#if defined (FJORD_KERNEL)
#  include <kifjord.h>
#endif

#if defined (PC_KERNEL)
#  include <kipc.h>
#endif

#if defined (EX_KERNEL)
#  include <kiex.h>
#endif

#if defined (EXS_KERNEL)
#  include <kiexs.h>
#endif

/***************************************************************************
 * Check config
 **************************************************************************/

#if defined (KI_OS_ENTRY_POINT) || defined (KI_OS_TASK_ARGS)
/* The underlying OS requires different return type or arguments
** for the entry point so we need to provide a wrapper function
** that matches those requirements. */
# define KI_OS_TASK_WRAPPERS_REQUIRED
/* Only one may be defined so make sure the other is set to the
** GKI default. */
# if !defined (KI_OS_ENTRY_POINT)
#  define      KI_OS_ENTRY_POINT   KI_ENTRY_POINT
# endif
# if !defined (KI_OS_TASK_ARGS)
#  define      KI_OS_TASK_ARGS     KI_TASK_ARGS
# endif
# if !defined (M_KiOsEntryPointReturn)
#  define      M_KiOsEntryPointReturn()
# endif
#endif

/** @} */ /* End of PrdGki group */

#endif

/* END OF FILE */
