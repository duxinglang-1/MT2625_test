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
 * Internal GKI header for the assert handling routines.
 **************************************************************************/

#ifndef KIOSFAIL_H
#define KIOSFAIL_H

#include <system.h>


#include <ttconfig.h>

#if defined(__cplusplus)
extern "C" {
#endif

/****************************************************************************
 * Function prototypes
 ****************************************************************************/

# if defined (KI_CHECK_ASSERTIONS)

/* See KIOSFAIL.C for information on this function */
void asShowFailure          (DevAssertFailType  assertFailType,
                             const char*        cond,
                             const char*        file,
                             Int16              line);

/* See KIOSFAIL.C for information on this function */
void asStopSystem           (DevAssertFailType  assertFailType,
                             const char*        cond,
                             const char*        file,
                             Int16              line);

#  if defined (KI_ASSERTFAIL_ENABLE_CONFIG)
/** \ingroup CfgGkiAssert
 * Check whether normal assert show routine should be run.
 * This function is called before calling #asShowFailure() in #asserfail()
 * if #KI_ASSERTFAIL_ENABLE_CONFIG is defined. This allows a client to
 * add additional assert handling and to control whether the default
 * handling should be run. Clients have to implement this function in
 * their own files.
 *
 * \param[in] assertFailType The type of assert and whether the
 *                condition field is present or not.
 * \param[in] cond The condition field gives the text from a #FatalFail
 *                or the check statement as text from a #FatalAssert or
 *                #FatalCheck. This will be NULL if #SHORT_DEV_ASSERTS is
 *                enabled or for a #FatalParam.
 * \param[in] file The name of the file the assert occurred in.
 * \param[in] line The line that the assert occurred in.
 *
 * \return A #Boolean that indicates if the default #asShowFailure()
 *                function should be run (TRUE) or not (FALSE). */
Boolean cfAsShowFailure     (DevAssertFailType  assertFailType,
                             const char*        cond,
                             const char*        file,
                             Int16              line);

/** \ingroup CfgGkiAssert
 * Check whether normal assert stop routine should be run.
 * This function is called before calling #asStopSystem() in #asserfail()
 * if #KI_ASSERTFAIL_ENABLE_CONFIG is defined. This allows a client to
 * add additional assert handling and to control whether the default
 * handling should be run. Clients have to implement this function
 * in their own files.
 *
 * \param[in] assertFailType The type of assert and whether the
 *                condition field is present or not.
 * \param[in] cond The condition field gives the text from a #FatalFail
 *                or the check statement as text from a #FatalAssert or
 *                #FatalCheck. This will be NULL if #SHORT_DEV_ASSERTS is
 *                enabled or for a #FatalParam.
 * \param[in] file The name of the file the assert occurred in.
 * \param[in] line The line that the assert occurred in.
 *
 * \return A #Boolean that indicates if the default #asStopSystem()
 *                function should be run (TRUE) or not (FALSE). */
Boolean cfAsStopSystem      (DevAssertFailType  assertFailType,
                             const char*        cond,
                             const char*        file,
                             Int16              line);
#  endif /* KI_ASSERTFAIL_ENABLE_CONFIG */
# endif /* KI_CHECK_ASSERTIONS */


#if defined(ENABLE_ARM_STACK_UNWIND) || defined(ENABLE_R2_STACK_UNWIND)
# if defined(__arm) || defined(ENABLE_R2_STACK_UNWIND)
/** Display the call stack.
 * This will unwind the current call stack to help determine where the
 * failure occured.
 */
void KiDisplayCallStack(Int16 assertStrLen);
# endif
#endif

#if defined (KI_CHECK_WARNINGS)
void KiFailSetAssertOnWarn(Boolean assertOnWarn);
#endif

#if defined(__cplusplus)
}
#endif

#endif

/* END OF FILE */

