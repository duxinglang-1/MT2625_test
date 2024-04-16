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
 * GKI header file for configuration items that affect the API.
 **************************************************************************/

#ifndef KICFG_H
#define KICFG_H

/** \addtogroup PrdGki
 * @{
 */

#include <ttconfig.h>

/***************************************************************************
 * Debug Macros
 **************************************************************************/

#if defined (UPGRADE_SYS_TOOLS)
#if defined(UT_ML_USE_MINILOGGER)
/***************************************************************************
 * utmlog.h needs to be included after the kernel has been defined but before
 * the following macro definitions. This allows the minilogger macros to be
 * inactive in library modules but expanded in non-library code
 ***************************************************************************/
# include <utmlog.h>
#endif
#endif

/** Task initialisation start debug hook.
 * This macro should be placed in a task at the start of its initialisation
 * code that runs before entering the main task/wait loop. The GKI does not
 * define any default functionality for this macro but it can be redefined to
 * provide debug visibility of task startup. A call to this macro should be
 * paired with a later call to the #M_KiDbgTaskInitEnd() macro. */
#if !defined (M_KiDbgTaskInitStart)
# define M_KiDbgTaskInitStart()
#endif

/** Task initialisation end debug hook.
 * This macro should be placed in a task at the end of its initialisation
 * code but before entering the main task/wait loop. The GKI does not define
 * any default functionality for this macro but it can be redefined to provide
 * debug visibility of task startup. A call to this macro should be paired
 * with an earlier call to the #M_KiDbgTaskInitStart() macro. */
#if !defined (M_KiDbgTaskInitEnd)
# define M_KiDbgTaskInitEnd()
#endif

/***************************************************************************
 * Platform specific Macros
 **************************************************************************/

#if defined (_SC100_)
# include <kisc.h>
#endif

#if defined (__TMS320C55X__)
# include <kic55xp.h>
#endif

#if defined (__arm)
# include <kigcc_arm.h>
#endif

#if defined (__WIN32__) || defined (_WIN32)
# include <kiwin32.h>
#endif

#if defined (__TMS470__)
# include <kitms470.h>
#endif

#if defined (_ARC)
# include <kiarc.h>
#endif

#if defined (__GNUC__) && !defined (ON_PC) && !defined (GX2) && !defined (__arm)
# include <kir2.h>
#endif

/** Get caller address.
 * This macro can be used in functions to get the address of where it was
 * called from. This macro must be the first bit of code within the function
 * calling it. This is usefull when adding #DevCheck()s in functions used in
 * several places. How this macro achieves this is compiler dependant.
 *
 * This is a default implementation of the macro that sets the address to 0.
 *
 * \param[out] aDDRESS Storage for writing the caller address into. */
#if !defined (M_KiGetCallerAddress)
# define M_KiGetCallerAddress(aDDRESS) aDDRESS = 0
#endif

/** Interrupt context disable.
 * This macro is used to save the current interrupt context and then disable
 * all interrupts. It should always be matched with a corresponding call to
 * #M_KiTgtIntContextRestore(). This macro is the GKI interface to the
 * processor specific macro or function.
 *
 * \param[out] iNTcONTEXT This is storage for the interrupt context before
 *                 interrupts are disabled. */
#if !defined (M_KiTgtIntContextDisable)
# if defined (KI_ENABLE_OWN_QUEUEING)
#  error The M_KiTgtIntContextDisable macro needs to be defined for GKI queueing
# endif /* KI_ENABLE_OWN_QUEUEING */
#endif

/** Interrupt context restore.
 * This macro is used to restore the interrupt context to what it was before
 * the matching call to #M_KiTgtIntContextDisable() was made. This macro is
 * the GKI interface to the processor specific macro or function.
 *
 * \param[in] iNTcONTEXT This is storage that holds the interrupt context
 *                before interrupts were disabled. */
#if !defined (M_KiTgtIntContextRestore)
# if defined (KI_ENABLE_OWN_QUEUEING)
#  error The M_KiTgtIntContextRestore macro needs to be defined for GKI queueing
# endif /* KI_ENABLE_OWN_QUEUEING */
#endif

#if !defined (M_KiTgtIntContextDisable)
typedef unsigned int KiTgtInterruptContext;
#define M_KiTgtIntContextDisable(iNTcONTEXT) (iNTcONTEXT = iNTcONTEXT)
#define M_KiTgtIntContextRestore(iNTcONTEXT)
#endif
/***************************************************************************
 * Compatibility Macros
 **************************************************************************/
#if defined (KI_MEMORY_DEBUG)
# if !defined (KI_MEMORY_USAGE)
#  define KI_MEMORY_USAGE  /* For backwards compatibility */
# endif
#endif

/***************************************************************************
 * Misc Config
 **************************************************************************/

/** \def DEVELOPMENT_STACKS
 * Cause all tasks to have their stack increased.
 * This conditional causes all task stacks to be increased by a fixed amount
 * given by the #KI_STACK_INCREASE value. This can be useful if it is
 * suspected that there is a stack corruption. Interrupt stack sizes may also
 * be increased by #INTERRUPT_STACK_INCREASE although this is platform
 * specific as to whether it is done.
 *
 * If #DEVELOPMENT_VERSION and #KI_STATISTICS is enabled the task stacks
 * will be checked in the lowest priority task to make sure tasks do not
 * use any of the extra development stack. */

/** \def KI_STACK_INCREASE
 * Development task stack increase.
 * This is the amount that the task stacks should be increased by when
 * #DEVELOPMENT_STACKS is defined. The default value can also be over
 * ridden in the build file. */
#if defined (DEVELOPMENT_STACKS)
# if !defined (KI_STACK_INCREASE)
#  define KI_STACK_INCREASE  512
# endif
#else
# define KI_STACK_INCREASE   0
#endif

/***************************************************************************
 * OS use in interrupt detection
 **************************************************************************/

/** \def KI_DETECT_OS_USE_IN_INT
 * Detect when an RTOS call has been made from an interrupt.
 * This enables a flag that is set whenever a GKI function for use in
 * interrupts is called that might affect the underlying RTOS. This may be
 * used to provide more efficient exit from an interrupt in the case where
 * no affected GKI functions were used. This is only needed if the
 * underlying RTOS does not efficiently exit interrupts when task rescheduling
 * is not required. This define can be enabled either when building the code
 * or in the RTOS specific GKI file e.g. kirq.h for RTXC Quadros.
 *
 * When this is defined a global called #kiSysOsIntFuncUsed exists which is
 * set to 1 whenever a GKI RTOS interrupt function is called inside an interrupt.
 * On exit from the interrupt context to the task context if the
 * #kiSysOsIntFuncUsed value is 1 then an task reschedule may be required.
 * The #kiSysOsIntFuncUsed value must be reset to 0 on exit from the interrupt
 * context (i.e. this needs to be done in the RTOS/Processor specific interrupt
 * handling code).
 *
 * The RTOS should always be able to do the same functionality more efficiently
 * and accurately as it can take into account priority and trigger event type
 * (e.g. signal, semaphore or event). This functionality is only to be used
 * when the RTOS is not detecting task reschedules on exit from interrupts
 * efficiently. */

/** Set the RTOS function used flag.
 * This is an internal macro used to set the #kiSysOsIntFuncUsed flag whenever
 * an RTOS function is called by the GKI from an interrupt. This macro iis
 * empty unless #KI_DETECT_OS_USE_IN_INT is enabled. */
#if defined (KI_DETECT_OS_USE_IN_INT)
# define M_KiSysOsIntFuncUsed()                        \
         {                                             \
             if (kiSysInterruptNestLevel != 0)         \
             {                                         \
                 kiSysOsIntFuncUsed = 1;               \
             }                                         \
         }
extern Int32 kiSysOsIntFuncUsed;
#else
# define M_KiSysOsIntFuncUsed()
#endif

/***************************************************************************
 * Address Zero checking
 **************************************************************************/

/** \def KI_ENABLE_ADDRESS_ZERO_CHECK
 * Enable checking of address zero corruption.
 * This enables checking of the value in address zero (NULL). The value at
 * address zero is stored when KiOsReset() is run. The value is then checked
 * when receiving and destroying signals. This can help narrow down code
 * that is writing to a NULL pointer. Using the
 * #KI_ENABLE_FULL_ADDRESS_ZERO_CHECK conditional compile will do more checks
 * at the expense of slower code. */

/** \def KI_ENABLE_FULL_ADDRESS_ZERO_CHECK
 * Enable more checking of address zero corruption.
 * This enables checking of the value in address zero (NULL). The value at
 * address zero is stored when KiOsReset() is run. The value is then checked
 * when creating, sending, receiving and destroying signals. This can help
 * narrow down code that is writing to a NULL pointer. To perform the checks
 * less often use the #KI_ENABLE_ADDRESS_ZERO_CHECK conditional instead. */
#if  defined (KI_CHECK_ASSERTIONS)

# if defined (KI_ENABLE_FULL_ADDRESS_ZERO_CHECK) && !defined (KI_ENABLE_ADDRESS_ZERO_CHECK)
#  define KI_ENABLE_ADDRESS_ZERO_CHECK
# endif

/* Add conditional for platforms that have it on all the time */
# if !defined (KI_ENABLE_ADDRESS_ZERO_CHECK)

#  if defined ( TTC_PLATFORM_S9C ) || defined ( TTC_PLATFORM_S9M )
#   define KI_ENABLE_ADDRESS_ZERO_CHECK
#  endif

# endif
#endif /* KI_CHECK_ASSERTIONS */

/***************************************************************************
 * Product specific configuration
 **************************************************************************/

/* S/W revision ID update*/
#if defined (UPGRADE_3G_TDD128)
# if defined (PLATFORM_G1MCU)
#define LABEL_ID "G1_INT_TDD_R5_V06Z04"
# else
#define LABEL_ID "G2T7V02Q"
# endif
#elif defined (UPGRADE_3G_FDD)
# if defined (PLATFORM_G1MCU)
#define LABEL_ID "G1_INT_FDD_R5_V03E"
# else
#define LABEL_ID "G2_INT_FDD_R6_V01L"
# endif
#endif
/** @} */ /* End of PrdGki group */

#define BUILD_ID "FD12100E"
#define VERSION_ID 0xABCD0008


#endif

/* END OF FILE */
