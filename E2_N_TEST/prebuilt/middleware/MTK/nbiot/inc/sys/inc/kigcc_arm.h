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
 * GKI header file for GCC ARM specific code.
 **************************************************************************/

#ifndef KIGCCARM_H
#define KIGCCARM_H

/** \addtogroup PrdGki
 * @{
 */

# if defined (__arm)
/******************************************************************************
* Includes
******************************************************************************/

#include <system.h>

#if defined (MTK_NBIOT_TARGET_BUILD)
#include <hal_nvic_internal.h>
#endif
/******************************************************************************
* Defines
******************************************************************************/

/** ARM CPSR interrupt mask.
 * This define gives the value ORed into the CPSR register when disabling all
 * interrupts. The define includes the # used in assembly to indicate an
 * immediate value as putting the # character directly in the macro causes a
 * compile error. */
#define KI_ARM_ASM_ALL_INTS_MASK   0xC0

/******************************************************************************
* Macros
******************************************************************************/

/** ARM implementation of get caller address.
 * This macro can be used in functions to get the address of where it was
 * called from. This macro must be the first bit of code within the function
 * calling it. This is usefull when adding #DevCheck()s in functions used in
 * several places. How this macro achieves this is compiler dependant.
 *
 * This assembler code gets the return address of the calling function.
 * Therefore the calling function can be traced as far as the module and
 * calling function name using the symbol file generated during the build.
 *
 * \param[out] aDDRESS Storage for writing the caller address into. */
#if (__ARMCC_VERSION >= 200000)
  /* RVCT 2.0 compatible version */
# define M_KiGetCallerAddress(aDDRESS) aDDRESS = __return_address();

#else

#define M_KiGetCallerAddress(aDDRESS) aDDRESS = (int)__builtin_return_address(0);

#endif

/** ARM specific interrupt context disable.
 * This macro is used to save the current interrupt context and then disable
 * all interrupts. It should always be matched with a corresponding call
 * to #M_KiArmIntContextRestore(). This macro should only be called via the
 * #M_KiTgtIntContextDisable() macro.
 *
 * \param[out] iNTcONTEXT This is storage for the interrupt context before
 *                 interrupts are disabled (this is the contents of the
 *                 CPSR register). */


#define M_KiArmIntContextDisable(iNTcONTEXT) \
{ \
        hal_nvic_save_and_set_interrupt_mask(&iNTcONTEXT); \
}

/** ARM specific interrupt context restore.
 * This macro is used to restore the interrupt context to what it was before
 * the matching call to #M_KiArmIntContextDisable() was made. This macro
 * should only be called via the #M_KiTgtIntContextRestore() macro.
 *
 * \param[in] iNTcONTEXT This is storage that holds the interrupt context
 *                before interrupts were disabled (this is the contents of
 *                the CPSR register). */
#define M_KiArmIntContextRestore(iNTcONTEXT)   \
    { \
    hal_nvic_restore_interrupt_mask(iNTcONTEXT); \
    }

/** ARM implementation of interrupt context disable.
 * This macro is used to save the current interrupt context and then
 * disable all interrupts. It should always be matched with a
 * corresponding call to #M_KiTgtIntContextRestore(). This macro is
 * the GKI interface to the processor specific macro.
 *
 * \param[out] iNTcONTEXT This is storage for the interrupt context
 *                 before interrupts are disabled. */

#if defined (__thumb)
# define M_KiTgtIntContextDisable(iNTcONTEXT)  iNTcONTEXT = KiArmIntContextDisable()
#else
# define M_KiTgtIntContextDisable(iNTcONTEXT)  M_KiArmIntContextDisable(iNTcONTEXT)
#endif

/** ARM implementation of interrupt context restore.
 * This macro is used to restore the interrupt context to what it was before
 * the matching call to #M_KiTgtIntContextDisable() was made. This macro is
 * the GKI interface to the processor specific macro.
 *
 * \param[out] iNTcONTEXT This is storage that holds the interrupt context
 *                 before interrupts were disabled. */
#if defined (__thumb)
# define M_KiTgtIntContextRestore(iNTcONTEXT)  KiArmIntContextRestore(iNTcONTEXT)
#else
# define M_KiTgtIntContextRestore(iNTcONTEXT)  M_KiArmIntContextRestore(iNTcONTEXT)
#endif

/******************************************************************************
* Typedefs
******************************************************************************/

/** ARM version of the type used to store an interrupt context.
 * This defines the type required to store interrupt context information for
 * an ARM processor for use in the #M_KiTgtIntContextDisable() and
 * #M_KiTgtIntContextRestore() macros. */
typedef uint32_t KiTgtInterruptContext;

/******************************************************************************
* Prototypes
******************************************************************************/

KiTgtInterruptContext KiArmIntContextDisable(void);
void                  KiArmIntContextRestore(KiTgtInterruptContext intContext);

extern hal_nvic_status_t hal_nvic_save_and_set_interrupt_mask(uint32_t * mask);
extern hal_nvic_status_t hal_nvic_restore_interrupt_mask(uint32_t mask);

# endif /* __arm */

/** @} */ /* End of PrdGki group */

#endif /* KIGCCARM_H */

