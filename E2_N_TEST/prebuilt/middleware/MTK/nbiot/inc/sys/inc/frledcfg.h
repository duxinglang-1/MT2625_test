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
 * \ingroup CfgCBEcorePLFservicesFrLed
 * \brief This is the platform configuration header file for the PLF LED
 * module (FrLed).
 * This is the platform configuration header file for the PLF LED module
 * (FrLed).  This file includes the LED configuration file frXXXled.h
 * for a particular platform, which in turn defines platform-specific macros.
 * Default (null) values of the LED driving macros are defined here, and these
 * should be overridden in the platform-specific file.
 */

#ifndef FRLEDCFG_H
#define FRLEDCFG_H

/*******************************************************************************
** Includes
*******************************************************************************/

#include <system.h>

#if !defined (FRLED_H)
# error This file must only be included directly from FRLED.H
#endif

/*******************************************************************************
** Platform specific include files
** Conditionally include any platform specific files here. These can override
** the configurable defines and macros in this file. Only one of these should be
** active at a time.
*******************************************************************************/

#if defined (TTC_PLATFORM_RPV4_PLP)
# include <frrp4pled.h>
#endif

#if defined (TTC_PLATFORM_RPV4_MCU)
# include <frrp4mled.h>
#endif

#if defined (TTC_PLATFORM_RPV2_MCU)
# include <frrp2mled.h>
#endif

#if defined (TTC_PLATFORM_S9C) || defined (TTC_PLATFORM_S9D) || defined (TTC_PLATFORM_S9M) || defined (TTC_PLATFORM_S9A)
# include <frs9led.h>
#endif

#if defined (TTC_PLATFORM_S3D) || defined (TTC_PLATFORM_S3M)
# include <frs3led.h>
#endif

/**
 * FIXME: [ALE] The frt1led.h should be a common entry to all
 * led headers for T1 subprojects.
 *
 */
#if defined (TTC_PLATFORM_T1D) || defined (TTC_PLATFORM_T1M)
# if defined (PLATFORM_POLEAXMCU) || defined (PLATFORM_POLEAXDSP)
#  include <frplxled.h>
# elif defined (PLATFORM_S1MCU) || defined (PLATFORM_S1DSP)
#  if defined (PLATFORM_S1_ON_PLXGPP)
#   include <frplxled.h>
#  endif
# elif defined (UPGRADE_DRACO_PLATFORM)
#  include <frt1led.h>
# elif defined (UPGRADE_CAPELLA_PLATFORM)
#  include <frcplled.h>
# elif defined (UPGRADE_G1_V1)
#  include <frg1v1led.h>
# elif defined (PLATFORM_G2DSP) || defined(PLATFORM_G2MCU)
#  if !defined (UPGRADE_G2P_ON_GPP) && !defined(UPGRADE_G2E_ON_GPP)
#  include <frg2led.h>
#  endif
# endif
#endif

#if defined (TTC_PLATFORM_M25G_PLP)
# include <frm25gpled.h>
#endif

#if defined (TTC_PLATFORM_M25G_MCU)
# include <frm25gmled.h>
#endif

#if defined (TTC_PLATFORM_EX_PLP)
# include <frexpled.h>
#endif

#if defined (TTC_PLATFORM_EX_MCU)
# include <frexmled.h>
#endif

#if defined (TTC_PLATFORM_PC_SIM)
/* Nothing yet */
#endif

#if defined (TTC_PLATFORM_CAMD)
# include <frcamdled.h>
#endif

#if defined (TTC_PLATFORM_CAMM)
# include <frcammled.h>
#endif

#if defined (TTC_PLATFORM_MHN)
# include <frmhnled.h>
#endif

/*******************************************************************************
** Macros
*******************************************************************************/

/** \addtogroup CfgCBEcorePLFservicesFrLed
 * @{ */

#if defined (FR_LED_ENABLE)

#if !defined (M_FrLedOn)
/** Switch a LED on.
 *  Switch a LED on.
 *  \param Int8 lEDnUM = The number of the LED to switch on.
 */
# define M_FrLedOn(lEDnUM)
#endif

#if !defined (M_FrLeds16On)
/** Switch a 16 bitmask of LEDs on.
 *  Switch a 16 bitmask of LEDs on.
 *  \param Int16 bITmASK = The 16bit bitmask of LEDs to switch on.
 */
# define M_FrLeds16On(bITmASK)
#endif

#if !defined (M_FrLedOff)
/** Switch a LED off.
 *  Switch a LED off.
 *  \param Int8 lEDnUM = The number of the LED to switch off.
 */
# define M_FrLedOff(lEDnUM)
#endif

#if !defined (M_FrLeds16Off)
/** Switch a 16 bitmask of LEDs off.
 *  Switch a 16 bitmask of LEDs off.
 *  \param Int16 bITmASK = The 16bit bitmask of LEDs to switch off.
 */
# define M_FrLeds16Off(bITmASK)
#endif


#if !defined (M_FrLedToggle)
/** Toggle a LED.
 *  Toggle a LED.
 *  \param Int8 lEDnUM = The number of the LED to toggle.
 */
# define M_FrLedToggle(lEDnUM)
#endif


#if !defined (M_FrLedInitialise)
/** Initialise the LEDs.
 *  Initialise the LEDs.
 */
# define M_FrLedInitialise()
#endif

#if !defined (M_FrWrite7SegDisplay)
/** Write to a 7 segment display.
 *  Write a hex value to a 7 segment display.
 *  \param Int8 vALUE = Hex digits to write to a 7 segment display.
 */
# define M_FrWrite7SegDisplay(vALUE)
#endif

#if !defined (M_FrClear7SegDisplay)
/** Clear a 7 segment display.
 *  Clear a 7 segment display.
 */
# define M_FrClear7SegDisplay()
#endif

#else /* FR_LED_ENABLE */

/*******************************************************************************
* If LED's are not enabled have to make sure that any platform specific
* versions of the macro's are not let through. So undefine any macros that are
* defined and replace with blank macros. This means FR_LED_ENABLE overrides
* any platform specific configuration.
*******************************************************************************/

#if defined (M_FrLedOn)
# undef M_FrLedOn
#endif
/** Macro to turn an LED on.
 *  The macro to turn an LED on is stubbed out if #FR_LED_ENABLE is
 *  not defined.  To use, define an appropriate macro in the
 *  platform-specific LED configuration file, and enable
 *  #FR_LED_ENABLE.
 */
#define M_FrLedOn(lEDnUM)

#if defined (M_FrLeds16On)
# undef M_FrLeds16On
#endif
/** Macro to turn a bitMask of LEDs on.
 *  The macro to turn a bitmask of LEDs on is stubbed out if #FR_LED_ENABLE is
 *  not defined.  To use, define an appropriate macro in the
 *  platform-specific LED configuration file, and enable
 *  #FR_LED_ENABLE.
 */
#define M_FrLeds16On(lEDnUM)

#if defined (M_FrLedOff)
# undef M_FrLedOff
#endif
/** Macro to turn an LED off.
 *  The macro to turn an LED off is stubbed out if #FR_LED_ENABLE is
 *  not defined.  To use, define an appropriate macro in the
 *  platform-specific LED configuration file, and enable
 *  #FR_LED_ENABLE.
 */
#define M_FrLedOff(lEDnUM)

#if defined (M_FrLeds16Off)
# undef M_FrLeds16Off
#endif
/** Macro to turn a bitMask of LEDs off.
 *  The macro to turn a bitmask of LEDs off is stubbed out if #FR_LED_ENABLE is
 *  not defined.  To use, define an appropriate macro in the
 *  platform-specific LED configuration file, and enable
 *  #FR_LED_ENABLE.
 */
#define M_FrLeds16Off(lEDnUM)

#if defined (M_FrLedToggle)
# undef M_FrLedToggle
#endif
/** Macro to toggle an LED.
 *  The macro to toggle an LED is stubbed out if #FR_LED_ENABLE is not
 *  defined.  To use, define an appropriate macro in the
 *  platform-specific LED configuration file, and enable
 *  #FR_LED_ENABLE.
 */
#define M_FrLedToggle(lEDnUM)

#if defined (M_FrLedInitialise)
# undef M_FrLedInitialise
#endif
/** Macro to initialise the LEDs.
 *  The macro to initialise the LEDs is stubbed out if #FR_LED_ENABLE
 *  is not defined.  To use, define an appropriate macro in the
 *  platform-specific LED configuration file, and enable
 *  #FR_LED_ENABLE.
 */
#define M_FrLedInitialise()

#if defined (M_FrWrite7SegDisplay)
# undef M_FrWrite7SegDisplay
#endif
/** Macro to write to the 7 segment display.
 *  The macro to write to the 7 segment display is stubbed out if
 *  #FR_LED_ENABLE is not defined.  To use, define an appropriate
 *  macro in the platform-specific LED configuration file, and enable
 *  #FR_LED_ENABLE.
 */
#define M_FrWrite7SegDisplay(vALUE)

#if defined (M_FrClear7SegDisplay)
# undef M_FrClear7SegDisplay
#endif
/** Macro to clear the 7 segment display.
 *  The macro to clear the 7 segment display is stubbed out if
 *  #FR_LED_ENABLE is not defined.  To use, define an appropriate
 *  macro in the platform-specific LED configuration file, and enable
 *  #FR_LED_ENABLE.
 */
#define M_FrClear7SegDisplay()

#endif /* FR_LED_ENABLE */


/** Switch an LED on.
 *  Macro to switch an LED on.  Required for backwards compatibility
 *  with PLK.
 *  \param Int8 lEDnUM = The number of the LED to switch on.
 */
#define M_LedOn(lEDnUM)           M_FrLedOn(lEDnUM)

/** Switch an LED off.
 *  Macro to switch an LED off.  Required for backwards compatibility
 *  with PLK.
 *  \param Int8 lEDnUM = The number of the LED to switch off.
 */
#define M_LedOff(lEDnUM)          M_FrLedOff(lEDnUM)

/** Toggle an LED.
 *  Macro to toggle an LED.  Required for backwards compatibility
 *  with PLK.
 *  \param Int8 lEDnUM = The number of the LED to toggle.
 */
#define M_LedToggle(lEDnUM)       M_FrLedToggle(lEDnUM)

/** Write to a 7 segment display.
 *  Macro to write to a 7 segment display.  Required for backwards
 *  compatibility with PLK.
 *  \param Int8 vALUE = Hex digits to write to a 7 segment display.
 */
#define M_Write7SegDisplay(vALUE) M_FrWrite7SegDisplay(vALUE)

/** Clear a 7 segment display.
 *  Macro to clear a 7 segment display.  Required for backwards
 *  compatibility with PLK.
 */
#define M_Clear7SegDisplay()      M_FrClear7SegDisplay()

/** @} */ /* End of addtogroup CfgCBEcorePLFservicesFrLed */

#endif
/* END OF FILE */
