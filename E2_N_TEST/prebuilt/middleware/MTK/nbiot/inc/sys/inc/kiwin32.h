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
 * GKI header file for Win32 specific code.
 **************************************************************************/

#ifndef KIWIN32_H
#define KIWIN32_H

/** \addtogroup PrdGki
 * @{
 */

#if defined (__WIN32__) || defined (_WIN32)
/******************************************************************************
* Includes
******************************************************************************/

#include <system.h>

/******************************************************************************
* Macros
******************************************************************************/

/** Win32 implementation of interrupt context disable.
 * This macro is used to save the current interrupt context and then disable
 * all interrupts. It should always be matched with a corresponding call to
 * #M_KiTgtIntContextRestore(). This is currently not used on Win32.
 *
 * \param[out] iNTcONTEXT This is storage for the interrupt context before
 *                 interrupts are disabled. */
#define M_KiTgtIntContextDisable(iNTcONTEXT)

/** Win32 implementation of interrupt context restore.
 * This macro is used to restore the interrupt context to what it was before
 * the matching call to #M_KiTgtIntContextDisable() was made. This is not
 * currently used on Win32.
 *
 * \param[in] iNTcONTEXT This is storage that holds the interrupt context
 *                before interrupts were disabled. */
#define M_KiTgtIntContextRestore(iNTcONTEXT)

/******************************************************************************
* Typedefs
******************************************************************************/

/** Win32 Interrupt context storage type.
 * This defines the type required to store interrupt context. This is not
 * currently used on Win32. */
typedef Int32 KiTgtInterruptContext;

# endif /* ON_PC */

/** @} */ /* End of PrdGki group */

#endif /* KIWIN32_H */

