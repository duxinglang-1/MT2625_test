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
 * GKI header file for PC kernel overrides.
 **************************************************************************/

#ifndef KIPC_H
#define KIPC_H

/** \addtogroup PrdGki
 * @{
 */

/** \def KI_PC_NULL_TASK_MACROS
 * Use empty task decleration macros on PC.
 * Defining this forces the #KI_SINGLE_TASK and #KI_MULTI_TASK macros
 * to be empty. This removes the implementation of these macros that
 * restricts the system to one task per exe (as used by geniexec.c).
 * The new version of geniexec, gx2 uses this define, see #GX2. */

/** \def GX2
 * Required when using gx2 (geniexec v2).
 * This conditional is required if gx2 is being used rather than geniexec.
 * This changes the task entry point type so that the task entry points
 * can be detected at runtime. It also changes the task decleration macros
 * on PC to allow multiple tasks to exist within the same exe (see
 * #KI_PC_NULL_TASK_MACROS). */

/***************************************************************************
 * Macros
 **************************************************************************/

#if defined (GX2)
/** Task entry point type.
 * On PC the task entry point needs some special attributes to allow
 * it to be detected at run time (makes PC simulation configuration
 * easier). So these attributes are hidden behind a define. The basic
 * underlying type is still void.  */
# if defined (__BORLANDC__)
#  define KI_ENTRY_POINT void __declspec(dllexport) __stdcall
# else
#  define KI_ENTRY_POINT void __declspec(dllexport)
# endif
# define KI_EXIT_POINT KI_ENTRY_POINT
/* Disable the old style geniexec macro definitions. */
# if !defined (KI_PC_NULL_TASK_MACROS)
# define KI_PC_NULL_TASK_MACROS
#endif
#endif /* GX2 */

/** On PC version of task decleration macros.
 * Override defaults for KI_SINGLE_TASK and KI_MULTI_TASK to provide
 * required on PC functionality. */
#if defined (KI_PC_NULL_TASK_MACROS)
# define     KI_SINGLE_TASK(eNTRYpOINT,qUEUEiD,tASKiD)
# define     KI_MULTI_TASK(eNTRYpOINT,iNSTANCE,qUEUEiD,tASKiD)
#else
# define     KI_SINGLE_TASK(eNTRYpOINT,qUEUEiD,tASKiD)                      \
           TaskId sourceTask        = tASKiD;                               \
           void (*exitPoint)(void)  = eNTRYpOINT##ExitRoutine;              \
           void (*entryPoint)(void) = eNTRYpOINT;

# define     KI_MULTI_TASK(eNTRYpOINT,iNSTANCE,qUEUEiD,tASKiD)              \
           TaskId sourceTask        = tASKiD;                               \
           int  (*exitPoint)(void)  = eNTRYpOINT##ExitRoutine;              \
           void (*entryPoint)(void) = eNTRYpOINT;
#endif /* KI_PC_NULL_TASK_MACROS */

/** @} */ /* End of PrdGki group */

#endif

/* END OF FILE */
